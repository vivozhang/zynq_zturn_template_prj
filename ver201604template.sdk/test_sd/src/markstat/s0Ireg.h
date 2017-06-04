/*=============================================================================
* Product:     PWM Source Using DSPX Based Control
* Host:        DSPX Running TMS320C31
* File:        LVP2Src\S0IREG.H
* Language:    TI 'C'
* Copyright:   1996 GE Motors and Industrial Systems, Salem VA
*
* Abstract:  SIREG.H contains constant definitions and function prototypes
*            used in the pwm source current regulator  function.
*
* Functions in module:  None
*
*==============================================================================
* Last Mod: October 21, 1998 11:35:19AM
*==============================================================================
* 970324 SAB  Create
* 970325 SAB  Add Src_Ctl_1
* 970423 SAB  Define Vars_Inc, Vars_Dec
* 970513 SAB  Remove Ixreg_Ff
* 970630 SAB  Add reactive current reference variables
* 971022 SAB  Rework reactive current reference generation
* 980807 SAB  Add Iyreg_V2_Ff
* 980901 SAB  Add L_Cos_2_Ang, Sin_2_Ang, L_Cos_Ang_Fbk, Sin_Ang_Fbk for ACMVRS6_A
* 980904 SAB  Add Res_Lim_Dyn, Ixreg_Rs_Out, Iyreg_Rs_Out
* 980916 SAB  Add Ld_Ix_Cmd_Ff for ACMVRS6-A only
* 981012 SAB  Add new resonator scheme
* 981016 SAB  Add Ld_Pwr_Cmd, Ld_Ix_Cmd
* 981021 SAB  ACMVRS6-A:  Move load feed-forward stuff to S0VREG.H
* 990622 SAB  Add diagnostic vars, Vx_Err_Fbk, Vy_Err_Fbk
* 000330 SAB  ACMVRS6-A: Add Iy_Cmd
* 010125 SAB  Add Mod_Index
* 021029 SAB  Add Spillover vars
* 021030 SAB  Remove Ixerg_V_Ff, other current delay model vars
* 021106 SAB  Add dc current regulator vars
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 021121 SAB  Fix wrongly renamed vars.
* 070219 SAB  Add L_IregYUc
* 070622 SAB  Add dc ireg vars for line and rotor
* 070801 SAB  Add L_IyregSpill
* 071120 Sab  Transfer in L_AcIMag
* 071220 SAB  Add 'propotional 2' path vars
*==============================================================================
*/

#ifndef __S0IREG_H__
#define __S0IREG_H__


#endif
