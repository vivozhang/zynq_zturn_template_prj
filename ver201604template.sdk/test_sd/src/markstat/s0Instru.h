/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        DSPX Running TMS320VC33
* File:        LVP2Src\S0INSTRU.H
* Language:    TI 'C'
* Copyright:   2003... GE Drives and Controls, Inc. Salem VA
*
* Abstract:  S0INSTRU.H contains constant definitions and function prototypes
*            used in the pwm source instrumentation functions.
*
* Functions in module:  None
*
*==============================================================================
* Last Mod: March 06, 192003 4:54:53PM
*==============================================================================
* 970324 SAB  Create
* 970325 SAB  Add more ram defs for s0instru.c30
* 970404 SAB  Add vars externs for diagnostic filters.
* 907408 SAB  Add I_Zero_Seq, Temp_Amb
* 970424 SAB  Make L_D_Pwr_Tot, Tr_Pwr_Tot available.
* 970424 SAB  Add Vab_Offs_Fil, Vbc_Offs_Fil, Brg_Loss_Tot
* 970501 SAB  Add Ix_Fbk_Fil_2, Iy_Fbk_Fil_2
*             Remove I_Mag_Sqr, Rename Dc_Pwr_Fil-->Dc_Bus_Pwr
*             Rename Brg_Loss_Tot--> L_Brg_Loss_Pwr
* 970506 SAB  Remove Temp_Amb
* 960520 SAB  Remove I_Gnd_Fil_Gn
* 971029 SAB  Add Var_Fil, Var_Fil_Gn
* 980114 DGF  Make Var_Calc global for LAN
* 980526 SAB  Remove Bridge power loss calc. variables.
* 980603 SAB  Remove L_Brg_Loss_Pwr
* 980924 DGF  Add extern Temp_Chg_Res
* 021024 SAB  Remove Temp_Res_T2, others
* 021029 SAB  Remove Vdc_Fbk_Z2...Vdc_Fbk_Z7
* 021031 SAB  Remove a,b,c diagnostic vars
* 021108 SAB  Add prefix "L_" in front of vars
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 030225 DS   remove L_LinePwrT2
* 030304 DS   add L_IyHrmFCal, L_VarHrmF
* 031201 SAB  Add calc of un-compensated line magnitude
* 060727 SAB  L_SrcOutPwr --> L_CnvOutPwr, others
* 070801 SAB  Rename old values of v, i to reflect last T1 sample
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 080528 SAB  Add scheme for calculation of L_VdcIxIyMod and L_IyVdc
* 110824 SAB  Add L_VMagT2Gn and L_VMagT2Fil
*==============================================================================
*/

#ifndef __S0INSTRU_H__
#define __S0INSTRU_H__

#endif
