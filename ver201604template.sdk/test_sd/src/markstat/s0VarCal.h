/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        DSPX Running TMS320C31
* File:        LVP2Src\S0VARCAL.H
* Language:    TI 'C'
* Copyright:   1996 GE Motors and Industrial Systems, Salem VA
*
* Abstract:  SVARCAL.H contains constant definitions and function prototypes
*            used in the pwm source variable calculation functions.
*
* Functions in module:  None
*
*==============================================================================
* Last Mod: April 2, 2003 5:22:53PM
*==============================================================================
* 970324 SAB  Create
* 970422 SAB  Remove Vln_Scl extern
* 970507 SAB  Rename Vdc_Nom --> Vdc_Nom_Cal
* 970520 SAB  Rename L_Line_Dt-->L_Tot_Dt, L_Line_Cal--> L_Tot_Cal
* 970627 SAB  Add Iyrf_I_Setpt, Iyrf_V_Setpt
* 970827 SAB  Add ram copy of final values for tool use only
* 971020 SAB  Remove Iyrf_PF_Stpt
* 971118 SAB  Remove L_W_Rcp_Sim
* 980126 SAB  Remove calc of C_Thrm_L_Rcp, R_Thrm_L_Rcp
* 980507 SAB  Add Vreg_Gn_Rcp
* 980610 SAB  Remove vars not associated whith LCL
* 981012 SAB  Add Res_WI_Rcp
* 981118 SAB  Add Ph_Imb_Spilx, Ph_Imb_Tmx
* 990701 SAB  ACMVRS6_A:  Add Nc_Inh_Lvl, Ld_Amps_Rcp
* 021030 SAB  Add Ns_X_Step, Ns_Y_Step
* 021030 SAB  Remove zero value current regulator conditioning vars
* 021106 SAB  Add dc regulator steps
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 021120 SAB  Add back missing #endif removed during last change
* 030124 SAB  ScaleCalc --> LineScaleCalc
* 030226 SCP  Add new vars.
* 030401 SCP  Remove L_RHrmFRcp and L_CHrmFDt
* 030402 SCP  Add L_RCHrmFRcp
* 031006 SAB  Add step vars for regulators
* 031106 SAB  Add freq step far for sim mode
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
*==============================================================================
*/

#ifndef __S0VARCAL_H__
#define __S0VARCAL_H__


#endif
