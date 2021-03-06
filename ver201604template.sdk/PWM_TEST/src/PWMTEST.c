/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "PWM.h"
#include "xil_types.h"
#include "xstatus.h"
#include "xparameters.h"
#include "stdio.h"
#include "xil_io.h"

/************************** Constant Definitions ***************************/
#define READ_WRITE_MUL_FACTOR 0x03



#define PWM_PWM_AXI_SLV_REG0_OFFSET 0
#define PWM_PWM_AXI_SLV_REG1_OFFSET 4
#define PWM_PWM_AXI_SLV_REG2_OFFSET 8
#define PWM_PWM_AXI_SLV_REG3_OFFSET 12

#define PWM_BASEADDRESS 0x43c00000
#define PWM_DUTYBASEADD 0x43c00040

XStatus PWM_Reg_SelfTest(void * baseaddr_p)
{
	u32 baseaddr;
	int write_loop_index;
	int read_loop_index;
	int Index;

	baseaddr = (u32) baseaddr_p;

	xil_printf("******************************\n\r");
	xil_printf("* User Peripheral Self Test\n\r");
	xil_printf("******************************\n\n\r");

	/*
	 * Write to user logic slave module register(s) and read back
	 */
	xil_printf("User logic slave module test...\n\r");

	for (write_loop_index = 0 ; write_loop_index < 6; write_loop_index++)
	  PWM_mWriteReg (baseaddr, write_loop_index*4, (write_loop_index+1)*READ_WRITE_MUL_FACTOR);

	for (read_loop_index = 0 ; read_loop_index < 6; read_loop_index++)
	  if ( PWM_mReadReg (baseaddr, read_loop_index*4) != (read_loop_index+1)*READ_WRITE_MUL_FACTOR){
	    xil_printf ("Error reading register value at address %x\n", (int)baseaddr + read_loop_index*4);
	    return XST_FAILURE;
	  }
	  else
		  xil_printf("Add %0x,data is %d\n",baseaddr+read_loop_index*4,PWM_mReadReg (baseaddr, read_loop_index*4));

	xil_printf("   - slave register write/read passed\n\n\r");

	return XST_SUCCESS;
}


static void delay(int dly)
{
	unsigned int i, j;
	for (i = 0; i < dly; i++) {
		for (j = 0; j < 0xffff; j++) {
			sleep(1000);
		}
	}
}

int main()
{
    init_platform();

    u32 baseaddr_p = (u32)XPAR_PWM_0_PWM_AXI_BASEADDR;
    u32 baseaddr_d = (u32)PWM_DUTYBASEADD;

    unsigned int ii=0;
    while(ii<5)
    {
    printf("Start test, %d\n\r",ii);
    ii++;
    sleep(1);
    }
    //PWM_Reg_SelfTest(baseaddr_d);

    printf("PWM test start\n\r");
    //enable PWM output
    PWM_mWriteReg (baseaddr_p, 0, 1);
    //set PWM period
    PWM_mWriteReg (baseaddr_p, 8, 100);

    // flushing LEDs

    int i=0;
    int j=0;
    int k=0;

    int sign_i = 1;
    int sign_j = 1;
    int sign_k = 1;

    while(1)
    {
    	PWM_mWriteReg (baseaddr_d, 0, i);
    	PWM_mWriteReg (baseaddr_d, 4, j);
    	PWM_mWriteReg (baseaddr_d, 8, k);

    	i = i+sign_i;
    	j = i+sign_j;
    	k = i+sign_k;


    	sleep(1);
printf("one more test\n");
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
    }


    print("End Test\n\r");

    cleanup_platform();
    return 0;
}
