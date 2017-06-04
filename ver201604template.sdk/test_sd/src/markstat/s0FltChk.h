/*=============================================================================
* Product:     PWM Source Using DSPX Based Control
* Host:        DSPX Running TMS320C31
* File:        S0FLTCHK.H
* Language:    TI 'C'
* Copyright:   1996 GE Motors and Industrial Systems, Salem VA
*
* Abstract:  S0FLTCHK.H contains constant definitions and function prototypes
*            used in checking faults for the pwm source control.
*
* Functions in module:  None
*
*==============================================================================
* Last Mod: June 3, 1998 2:38:06PM
*==============================================================================
* 970527 SAB  Create
* 980603 SAB  Remove 0 from two externs
* 991214 SAB  Add prototype for RequiredPrmFlt()
* 991223 SAB  Remove RequiredPrmFlt function (move to s0rule)
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 021119 SAB  Fix bad comment line
* 021121 SAB  L_LSimMode --> SimMode
*==============================================================================
*/

#ifndef __S0FLTCHK_H__
#define __S0FLTCHK_H__


//Select RegLimFlgs needed for Regulator Limit fault and L_RegLimHist
// All but bits L_UxlimPFlg, L_UxlimNFlg (bits 10,11)
// bits: 0,1,2,3,4,5,6,7,8,9 --> /0000/0011/1111/1111 --> 03FF


#endif
