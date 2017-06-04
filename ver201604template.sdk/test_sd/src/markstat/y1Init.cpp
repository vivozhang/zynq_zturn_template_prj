/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* File:        Y1INIT.C30
* Language:    TI 'C'
* Copyright:   2003 GE Industrial Control Systems, Salem VA
*
* Abstract: This file contains PWM2 specific initialization functionality.
*
*==============================================================================
*/

#include  "product.h"
#include  "x0FrameWork.h"



 /*****************************************************************************
*       external references
*****************************************************************************/
extern float            DelTm1A;
extern float            DelTm1ARcp;
extern int              T1AFramesPerT2;
extern unsigned         Clk0PerQuant;
extern unsigned         QuantsPerMod1A;
extern unsigned         QuantsPerT1A;



/*******************************************************************************
*  Global variable declarations
*******************************************************************************/
CREATE_PUBVAR(R_DelTm1          ,float);        // rotor - delta time - task1
CREATE_PUBVAR(R_DelTm1Rcp       ,float);        // rotor - delta time reciprocal - task1
CREATE_PUBVAR(R_T1FramesPerT2   ,int);          // rotor - task1 frames per task2
CREATE_PUBVAR(R_PwmQuant        ,int);          // rotor - number of scheduling quant in a pwm quant
CREATE_PUBVAR(R_UFull           ,int);          // rotor - number of pwm quants in a T1 frame
CREATE_PUBVAR(R_UHalf           ,float);        // rotor - R_UFull / 2
CREATE_PUBVAR(R_UHalfRcp        ,float);        // rotor - R_UHalf reciprocal




void R_CalculateTimebase()
{
    R_DelTm1        = DelTm1A;
    R_DelTm1Rcp     = DelTm1ARcp;
    R_T1FramesPerT2 = T1AFramesPerT2;
    R_PwmQuant      = Clk0PerQuant;
    R_UFull         = QuantsPerT1A;
    R_UHalf         = 0.5F * R_UFull;
    R_UHalfRcp      = 1.0F / R_UHalf;

}
