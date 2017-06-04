/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* File:        Y0INIT.C30
* Language:    TI                                                       'C                                                      '
* Copyright:   2003... GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file contains PWM2 specific initialization functionality.
*
*==============================================================================
*/

#include  "Product.h"
#include  "x0FrameWork.h"




extern void  LineInitFpgaFlt();
extern void  RotorInitFpgaFlt();



/*****************************************************************************
*       external references
*****************************************************************************/


/*****************************************************************************
*  Constant definitions
*****************************************************************************/

/*******************************************************************************
*  Global variable declarations
*******************************************************************************/

CREATE_PUBVAR(AeadDSRFound,     int      ) = false;




/**************************** ***************************************************
*  Local variable declarations
*******************************************************************************/



void InitBIL(void)
{
    RotorInitFpgaFlt();
    LineInitFpgaFlt();

}
