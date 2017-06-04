/*=============================================================================
* Product:     AccuWave Power Conversion Using MACC
* Host:        TMS320CVC33 DSP
* File:        Y2FLTCK.H
* Language:    TI 'C'
* Copyright:   2003..... GE Industrial Control Systems, Salem VA
*
* Abstract:    Header File for Y2FLTCK.C30
*
*==============================================================================
* 970318 RCB  Created from X0FAULTS.H Move CheckAsicFlt(), ClrASICFlt(),
*             SimulateAdjustments() from X0FAULTS.C30
* 970331 DGR  Declare inverter ground fault detection variables as extern.
*             Convert file to inline format.
* 970421 DGR  Move inverter ground fault variables to Y0FLTCKA.H.
* 970421 RCB  Add InitASICFlt() proto.
* 970609 DGR  Define Asic_Rst_T3 and Bpib_Rst_T3 as externs.
* 971127 RCB  Add Bic_WDog_Lch
* 971217 DGR  Remove variable Bic_WDog_Lch.
* 980129 RCB  Add ClrASICHdw() prototype, Asic_Hdw_Rst, Clr_Asic_Hdw.
* 980129 DGR  Add I_Fbk_Invld.
* 980514 CWM  Rename to Y1FLTCK
* 021120 SAB  Change naming convention: Remove all under-scores and add
*             "L_" on line converter names.
* 021210 SAB  Change name to Y2Fltck.h
* 030326 SAB  Asci... -> Fpga...
* 030409 SAB  CheckFpgaFlt --> LineCheckFpgaFlt
* 030410 SAB  Rename FpgaRstT3 to FpgaRstT3B1
* 031119 SAB  Import vars from old Y2FltChk.h
* 070130 SAB  Add ClrFpgaCtrB1
* 070508 SAB  Add PwrDnVdcTmr, PwrUpVdcTmr,  and PwrDnFbkDmr7
* 070514 SAB  Add extern for  CommonFaultDetectionBIL
* 080909 SAB  Remove un-used var, L_IFbkIvdOR
* 110819 SAB  Add DB fault check stuff
*______________________________________________________________________________
* Last Mod: May 14, 1998 4:26:31PM
*/


#ifndef  __Y2FLTCK_H__
#define  __Y2FLTCK_H__





#endif
