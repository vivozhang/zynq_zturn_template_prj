/*=============================================================================
* Product:     GE Power Conversion Using MACC
* File:        Y2INIT.C30
* Language:    TI 'C'
* Copyright:   2003... GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file contains PWM2 specific initialization functionality.
*
*==============================================================================
*/

#include "x0Framework.h"
#include "Product.h"



/*****************************************************************************
*       external references
*****************************************************************************/
extern float            DelTm1;
extern float            DelTm1Rcp;
extern int              T1FramesPerT2;
extern unsigned         Clk0PerQuant;
extern unsigned         QuantsPerMod;
extern unsigned         QuantsPerT1;



/*******************************************************************************
*  Global variable declarations
*******************************************************************************/
CREATE_PUBVAR(L_DelTm1          ,float);        // line - delta time - task1
CREATE_PUBVAR(L_DelTm1Rcp       ,float);        // line - delta time reciprocal - task1
CREATE_PUBVAR(L_T1FramesPerT2   ,int);          // line - task1 frames per task2
CREATE_PUBVAR(L_PwmQuant        ,int);          // line - number of scheduling quant in a pwm quant
CREATE_PUBVAR(L_UFull           ,int);          // line - number of pwm quants in a T1 frame
CREATE_PUBVAR(L_UHalf           ,float);        // line - L_UFull / 2
CREATE_PUBVAR(L_UHalfRcp        ,float);        // line - L_UHalf reciprocal




void L_CalculateTimebase()
{
    L_DelTm1        = DelTm1;
    L_DelTm1Rcp     = DelTm1Rcp;
    L_T1FramesPerT2 = T1FramesPerT2;
    L_PwmQuant      = Clk0PerQuant;
    L_UFull         = QuantsPerT1;
    L_UHalf         = 0.5F * L_UFull;
    L_UHalfRcp      = 1.0F / L_UHalf;

}
