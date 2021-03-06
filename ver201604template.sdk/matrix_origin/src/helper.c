/*
 * Copyright (c) 2014, Mentor Graphics Corporation
 * All rights reserved.
 *
 * Copyright (c) 2015 Xilinx, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the <ORGANIZATION> nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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

#include "xparameters.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "xil_cache.h"
#include "metal/sys.h"
#include "metal/irq.h"
#include "platform_info.h"
#include "xscutimer.h"

#define INTC_DEVICE_ID		XPAR_SCUGIC_0_DEVICE_ID

//timer interrupt setting
#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
//#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR
#define TIMER_LOAD_VALUE 0x13d92d40 //0x4ffffff

extern void TimerIntrHandler(void *CallBackRef);
static XScuGic xInterruptController;

extern XScuTimer Timer;
XScuTimer_Config *ConfigPtr;

/* Interrupt Controller setup */
static int app_gic_initialize(void)
{
	u32 Status;
	XScuGic_Config *IntcConfig;	/* The configuration parameters of the interrupt controller */

	Xil_ExceptionDisable();

	/*
	 * Initialize the interrupt controller driver
	 */
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&xInterruptController, IntcConfig,
			IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
	Status = XScuTimer_CfgInitialize(&Timer, ConfigPtr,
						ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {return XST_FAILURE;	}

	XScuGic_Enable(&xInterruptController, (u16)TIMER_IRPT_INTR);

	/*
	 * Register the interrupt handler to the hardware interrupt handling
	 * logic in the ARM processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler,
			&xInterruptController);

	Xil_ExceptionEnable();

	/* Connect IPI0 Interrupt ID with ISR */
	XScuGic_Connect(&xInterruptController, VRING0_IPI_INTR_VECT,
			(Xil_ExceptionHandler)metal_irq_isr,
			(void *)VRING0_IPI_INTR_VECT);

	/* Connect IPI1 Interrupt ID with ISR */
	XScuGic_Connect(&xInterruptController, VRING1_IPI_INTR_VECT,
			(Xil_ExceptionHandler)metal_irq_isr,
			(void *)VRING1_IPI_INTR_VECT);


	XScuGic_Connect(&xInterruptController, (u16)TIMER_IRPT_INTR,
					(Xil_ExceptionHandler)TimerIntrHandler,
					&Timer);
	XScuTimer_EnableAutoReload(&Timer);
	XScuTimer_LoadTimer(&Timer, (u32)TIMER_LOAD_VALUE);

	return 0;
}

/* Main hw machinery initialization entry point, called from main()*/
/* return 0 on success */
int init_system(void)
{
	struct metal_init_params metal_param = METAL_INIT_DEFAULTS;

	/* Low level abstraction layer for openamp initialization */
	metal_init(&metal_param);

	/* configure the global interrupt controller */
	app_gic_initialize();

	return 0;
}

void cleanup_system()
{
	metal_finish();

	Xil_DCacheDisable();
	Xil_ICacheDisable();
	Xil_DCacheInvalidate();
	Xil_ICacheInvalidate();
}
