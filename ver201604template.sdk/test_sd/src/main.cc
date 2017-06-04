/*
 * Empty C++ Application
 * read and write SD card ok, verified by jie 2017-05-11
 */
#include<stdio.h>
#include<iostream>

#include "xscugic.h"
#include "xil_exception.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xsdps.h"
#include "xil_printf.h"

//#include "xtmrctr.h"
#include "ff.h"
#include "xil_cache.h"
#include "sleep.h"


// Parameter Definitions
#define INTC_DEVICE_ID 			XPAR_PS7_SCUGIC_0_DEVICE_ID
#define BTNS_DEVICE_ID			XPAR_AXI_GPIO_0_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID  XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define BTN_INT 				XGPIO_IR_CH1_MASK // This is the interrupt mask for channel one
#define TMR_DEVICE_ID			XPAR_TMRCTR_0_DEVICE_ID
#define INTC_TMR_INTERRUPT_ID 	XPAR_FABRIC_AXI_TIMER_0_INTERRUPT_INTR
/* IMPORTANT NOTE: The AXI timer frequency is set to 50Mhz
 * the timer is set up to be counting UP, these two facts affect the value selected for
 * the load register to generate 1 Hz interrupts
 */
#define TMR_LOAD				0xFD050F7F


XGpio   BTNInst;
XScuGic INTCInst;
//XTmrCtr TMRInst;

//Lab7 addition start
static FATFS FS_instance; // File System instance
static FIL file1;		// File instance
FRESULT result;			// FRESULT variable
static char FileName[32] = "RECORDS2.txt"; // name of the log
static char *Log_File; // pointer to the log
char *Path = "0:/";  //  string pointer to the logical drive number
int counter_logger=0; // Counter for the push button
unsigned int BytesWr; // Bytes written
int len=0;			// length of the string
int accum=0;		//  variable holding the EOF
char Buffer_logger[64] __attribute__ ((aligned(32))); // Buffer should be word aligned (multiple of 4)
u32 Buffer_size = 64;

/*
int main()
{
	printf("Have a good try!!!");

	result = f_mount(&FS_instance,Path, 1);
		if (result != 0) {
			return XST_FAILURE;
		}

	Log_File = (char *)FileName;
	result = f_open(&file1, Log_File, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
		if (result!= 0) {
			return XST_FAILURE;
		}

		// Point to the end of log
		result = f_lseek(&file1,accum);
			if (result!=0) {
				return XST_FAILURE;
			}

		sprintf(Buffer_logger, "%d:%d:%d Event:Sensor X Triggered %d\r\n",11,22,33,5555);

		// Write to log
		result = f_write(&file1, (const void*)Buffer_logger, Buffer_size,&BytesWr);
			if (result!=0) {
				return XST_FAILURE;
			}

		// Increment counter
		counter_logger++;

		// Increment file EOF pointer
		len = strlen(Buffer_logger);
		accum=accum+len;

		 //Close file.
		result = f_close(&file1);
			if (result!=0) {
				return XST_FAILURE;
			}
		xil_printf("Data written to log Successfully\r\n");

	return 0;
}*/
