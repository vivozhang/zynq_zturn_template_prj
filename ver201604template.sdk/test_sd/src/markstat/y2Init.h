/*=============================================================================
* Product:     Innovation Series Control Using DSPX
* Host:        DSPX Running TMS320C31
* File:        Y1INIT.H
* Language:    TI 'C'
* Copyright:   1996..1999 GE Industrial Control Systems, Salem VA
*
* Abstract:  Y1INIT.H contains constant definitions and function prototypes
*            used for pwm2 specific initializations.
*
* Functions in module: Pwm2CtlInit
*
*==============================================================================
*
* 961107 SAB  Create
* 970407 DGR  Add lockout time constant definitions.
* 970415 PSB  ConfigureEplds() prototype added.
* 970422 PSB  Rename Pwm2CtlInit -> PwmCtlInit
* 970423 PSB  Move BreadBoard Initialization from Y0PWMINI.C30
* 970521 DGR  Change lockout time constants.
* 970721 RCB  Add prototype for ValidateBoards().
* 970813 PSB  Add new prototype ReInitPwm(), remove RACK_MODE defines.
* 971104 RAS  Major mods to Timebase/FramePLL/Modulator configuration
* 971105 RAS  Final restructure & initial debug of previous item(s)
* 971125 DGR  Remove lockout time constants and comments.
* 971202 RAS  Add Pwm_Pls_Cmd
* 971217 DGR  Declare Brg_1_Init and Brg_2_Init.
* 980417 CWM  Rename PwmCtlInit to InitBIL.
* 980514 CWM  Rename Y1INIT.
* 990218 RAS  LVP2 Extended PWM: define "alternate base quantities";
*             increase PWM_CHOP_MAX from 2 to 4, T0_FRAMES_MAX from 1 to 2.
* 021120 SAB  Change naming convention: Remove all under-scores and add
*             "L_" on line converter names.
* 021210 SAB  Change name to Y2Init.h
* 021212 SAB  Rename T1Frames to L_T1Frames
* 021218 SAB  Rename CalculateTimebase to L_CalculateTimebase
* 030107 SAB  Remove L_LkoutAsic
* 030307 SAB  Compile out Brg_1_Init, Brg_2_Init stuff for MACC
* 030313 SAB  T2Frames_Max --> 20
* 030528 SAB  Remove Brg1Init, Brg2Init
* 030821 SAB  Widen limits on INT_RATE
*==============================================================================
* Last Mod: March 24, 192003 3:39:08PM
*==============================================================================
*/


#ifndef __Y2INIT_H__
#define __Y2INIT_H__



#endif
