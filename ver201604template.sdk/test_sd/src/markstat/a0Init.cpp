/*=============================================================================
* Product:     AccuWave Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        A0INIT.C30
* Language:    TI 'C'
* Copyright:   1996..1999 GE Industrial Systems, Salem VA
*
* Abstract: This file contains IMD Motor control specific initialization
*           functionality.
*
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* InitTasks
* ReInitMotorCtrl
*
*==============================================================================
* 961104 0001 KZB  Created from Z0TASKS.C30
* 021021 DS   modify for doubly-fed
* 030204 SAB  Branch from 'rotor' and include in 'combo'
*_______________________________________________________________________________
* Last Mod: June 19, 192003 1:51:13PM
*/

#include "x0FrameWork.h"
#include "Product.h"
#include "a0Init.h"

/*******************************************************************************
* Global variable declarations
*******************************************************************************/
extern void RotorInitSequencer()       ;
extern void RotorInitAutotune (void)   ;



/*****************************************************************************
*
*       external references
*
*****************************************************************************/


/*****************************************************************************
*
*       private variables
*
*****************************************************************************/

/******************************************************************************
* Function:         InitLCL
* Execution Level:  Initialization
* Execution Time:   Not measured
*
* Formal Inputs:
* Formal Outputs:
*
* Global Inputs:
* Global Outputs:
*
* Return Value:
*
* Function Calls:   ScaleCalcInit
*                   InitSequencer
*                   InitAutotune
*
* Abstract:         Initialize IMD motor controller functions
*
******************************************************************************/

void RotorInitLCL()

{
    /* IMD motor control specific init functions */

    RotorInitSequencer();      /* Initialize sequencer variables */
#if 0
    RotorInitAutotune();       /* initialize autotune */
#endif
    return;
}

