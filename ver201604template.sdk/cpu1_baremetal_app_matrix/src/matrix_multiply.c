/*
 * Copyright (c) 2014, Mentor Graphics Corporation
 * All rights reserved.
 *
 * Copyright (C) 2015 Xilinx, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of Mentor Graphics Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**************************************************************************************
* This is a sample demonstration application that showcases usage of remoteproc
* and rpmsg APIs on the remote core. This application is meant to run on the remote CPU
* running bare-metal code. It receives two matrices from the master,
* multiplies them and returns the result to the master core.
*
* The init_system is called in the main function which defines a shared memory region in
* MPU settings for the communication between master and remote using
* zynqMP_r5_map_mem_region API,it also initializes interrupt controller
* GIC and register the interrupt service routine for IPI using
* zynqMP_r5_gic_initialize API.
*
* The remoteproc_resource_init API is being called to create the virtio/RPMsg devices
* required for IPC with the master context. Invocation of this API causes remoteproc on
* the bare-metal to use the rpmsg name service announcement feature to advertise the
* rpmsg channels served by the application.
*
* The master receives the advertisement messages and performs the following tasks:
* 	1. Invokes the channel created callback registered by the master application
* 	2. Responds to remote context with a name service acknowledgement message
* After the acknowledgement is received from master, remoteproc on the bare-metal
* invokes the RPMsg channel-created callback registered by the remote application.
* The RPMsg channel is established at this point. All RPMsg APIs can be used subsequently
* on both sides for run time communications between the master and remote software contexts.
*
* Upon running the master application to send data to remote core, master will
* generate the matrices and send to remote (bare-metal) by informing the bare-metal with
* an IPI, the remote will compute the resulting matrix and send the data back to
* master. Once the application is ran and task by the
* bare-metal application is done, master needs to properly shut down the remote
* processor
*
* To shut down the remote processor, the following steps are performed:
* 	1. The master application sends an application-specific shutdown message
* 	   to the remote context
* 	2. This bare-metal application cleans up application resources,
* 	   sends a shutdown acknowledge to master, and invokes remoteproc_resource_deinit
* 	   API to de-initialize remoteproc on the bare-metal side.
* 	3. On receiving the shutdown acknowledge message, the master application invokes
* 	   the remoteproc_shutdown API to shut down the remote processor and de-initialize
* 	   remoteproc using remoteproc_deinit on its side.
*
**************************************************************************************/

#include "xil_printf.h"
#include "openamp/open_amp.h"
#include "rsc_table.h"
#include "platform_info.h"
#include "PWM.h"
#include "xparameters.h"
#include "xil_io.h"

#define	MAX_SIZE		6
#define NUM_MATRIX		2

#define READ_WRITE_MUL_FACTOR 0x03

#define PWM_PWM_AXI_SLV_REG0_OFFSET 0
#define PWM_PWM_AXI_SLV_REG1_OFFSET 4
#define PWM_PWM_AXI_SLV_REG2_OFFSET 8
#define PWM_PWM_AXI_SLV_REG3_OFFSET 12

#define PWM_BASEADDRESS 0x43c00000
#define PWM_DUTYBASEADD 0x43c00040


#define SHUTDOWN_MSG	0xEF56A55A

#define LPRINTF(format, ...) xil_printf(format, ##__VA_ARGS__)
#define LPERROR(format, ...) LPRINTF("ERROR: " format, ##__VA_ARGS__)

typedef struct _matrix {
	unsigned int size;
	unsigned int elements[MAX_SIZE][MAX_SIZE];
} matrix;

typedef struct DATA_ARRAY{
	unsigned int size;
	unsigned int elements[MAX_SIZE];
}data_array;
static data_array Recieved_data;
static data_array Send_data;

/* Local variables */
//static matrix matrix_array[NUM_MATRIX];
//static matrix matrix_result;

static struct rpmsg_endpoint *rp_ept;
static struct remote_proc *proc = NULL;
static struct rsc_table_info rsc_info;

static int evt_chnl_deleted = 0;
static unsigned int counter = 0;

/* Internal functions */
static void rpmsg_channel_created(struct rpmsg_channel *rp_chnl);
static void rpmsg_channel_deleted(struct rpmsg_channel *rp_chnl);
static void rpmsg_read_cb(struct rpmsg_channel *, void *, int, void *,
			  unsigned long);

/* External functions */
extern int init_system(void);
extern void cleanup_system(void);

/*-----------------------------------------------------------------------------*
 *  Application specific
 *-----------------------------------------------------------------------------*/
static void Matrix_Multiply(const matrix *m, const matrix *n, matrix *r)
{
	unsigned int i, j, k;

	memset(r, 0x0, sizeof(matrix));
	r->size = m->size;

	for (i = 0; i < m->size; ++i) {
		for (j = 0; j < n->size; ++j) {
			for (k = 0; k < r->size; ++k) {
				r->elements[i][j] += m->elements[i][k] * n->elements[k][j];
			}
		}
	}
}

static void delay(int dly)
{
	int i, j;
	for (i = 0; i < dly; i++) {
		for (j = 0; j < 0xffff; j++) {
			;
		}
	}
}

static unsigned int SUM_ALL(data_array *INPUT){

	unsigned temp = 0;

	for(int i=0; i<INPUT->size; i++)
		temp = temp + INPUT->elements[i];

	return temp;
}

int app(struct hil_proc *hproc)
{
	int status = 0;

	/* Initialize framework */
	counter = 21;
	LPRINTF("Try to init remoteproc resource\n");
	status = remoteproc_resource_init(&rsc_info, hproc,
				     rpmsg_channel_created,
				     rpmsg_channel_deleted, rpmsg_read_cb,
				     &proc, 0);

	if (RPROC_SUCCESS != status) {
		LPERROR("Failed  to initialize remoteproc resource.\n");
		return -1;
	}

	counter = 25;

	LPRINTF("Init remoteproc resource done\n");

	LPRINTF("Waiting for events...\n");

    u32 baseaddr_p = (u32)XPAR_PWM_0_PWM_AXI_BASEADDR;
    u32 baseaddr_d = (u32)PWM_DUTYBASEADD;

    //enable PWM output
    PWM_mWriteReg (baseaddr_p, 0, 1);
    delay(1);
    //set PWM period
    PWM_mWriteReg (baseaddr_p, 8, 100);



	int i=0;
	int j=0;
	int k=0;

	int sign_i = 1;
	int sign_j = 1;
	int sign_k = 1;


	do {
		PWM_mWriteReg (baseaddr_d, 0, i);
		PWM_mWriteReg (baseaddr_d, 4, j);
		PWM_mWriteReg (baseaddr_d, 8, k);

		i = i+sign_i;
		j = i+sign_j;
		k = i+sign_k;


		delay(100);
		counter++;

		if(i>100||i<0)
			sign_i=sign_i*(-1);
		if(j>100||j<30)
			sign_j=sign_j*(-1);
		if(k>100||k<0)
			sign_k=sign_k*(-1);

		if(i<0)
			i=0;
		else if(i>100)
			i=100;

		if(j<30)
			j=30;
		else if(j>100)
			j=100;

		if(k<0)
			k=0;
		else if(k>100)
			k=100;

		hil_poll(proc->proc, 0);
	} while (!evt_chnl_deleted);

	/* disable interrupts and free resources */
	LPRINTF("De-initializating remoteproc resource\n");
	remoteproc_resource_deinit(proc);

	return 0;
}

/*-----------------------------------------------------------------------------*
 *  RPMSG callbacks setup by remoteproc_resource_init()
 *-----------------------------------------------------------------------------*/
static void rpmsg_channel_created(struct rpmsg_channel *rp_chnl)
{
	rp_ept = rpmsg_create_ept(rp_chnl, rpmsg_read_cb, RPMSG_NULL, RPMSG_ADDR_ANY);
}

static void rpmsg_channel_deleted(struct rpmsg_channel *rp_chnl)
{
	(void)rp_chnl;

	rpmsg_destroy_ept(rp_ept);
	rp_ept = NULL;
}



static void rpmsg_read_cb(struct rpmsg_channel *rp_chnl, void *data, int len,
                void *priv, unsigned long src)
{
	(void)priv;
	(void)src;




	if ((*(unsigned int *)data) == SHUTDOWN_MSG) {
		evt_chnl_deleted = 1;
		return;
	}


	//memcpy(matrix_array, data, len);
	memcpy(&Recieved_data, data, len);
	/* Process received data and multiple matrices. */
	//Matrix_Multiply(&matrix_array[0], &matrix_array[1],
	//		&matrix_result);
	Send_data.elements[0]=SUM_ALL(&Recieved_data);
	Send_data.elements[1] = counter;
	Send_data.size = 2;


	/* Send the result of matrix multiplication back to master. */
	if (RPMSG_SUCCESS != rpmsg_send(rp_chnl, &Send_data, 3*sizeof(unsigned))) {
		LPERROR("rpmsg_send failed\n");
	}

	int temp = Send_data.elements[0];

	while(temp>0)
	{
		Send_data.elements[0] = temp;
		if (RPMSG_SUCCESS != rpmsg_send(rp_chnl, &Send_data, 3*sizeof(unsigned))) {
			LPERROR("rpmsg_send failed\n");
		}
		delay(10);
		temp--;
	}


}

/*-----------------------------------------------------------------------------*
 *  Application entry point
 *-----------------------------------------------------------------------------*/
int main(void)
{
	unsigned long proc_id = 0;
	unsigned long rsc_id = 0;
	struct hil_proc *hproc;
	int status = -1;

	LPRINTF("Starting application...\n");

	/* Initialize HW system components */
	init_system();

	/* Create HIL proc */
	hproc = platform_create_proc(proc_id);
	if (!hproc) {
		LPERROR("Failed to create hil proc.\n");
	} else {
		rsc_info.rsc_tab = get_resource_table((int)rsc_id, &rsc_info.size);
		if (!rsc_info.rsc_tab) {
			LPERROR("Failed to get resource table data.\n");
		} else {
			status = app(hproc);
		}
	}

	LPRINTF("Stopping application...\n");
	cleanup_system();

	/* Suspend processor execution */
	while (1) {
		__asm__("wfi\n\t");
	}

	return status;
}
