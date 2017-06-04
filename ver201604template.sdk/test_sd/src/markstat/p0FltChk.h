/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        TMS320VC33 DSP
* File:        $P0FLTCHK.C30
* Language:    TI 'C'
* Copyright:   2003... GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file contains functions which pertain to fault checking in
*           all tasks.
*
*
* Functions in module:  None
*
*==============================================================================
* Last Mod: December 05, 02:41:11PM
*==============================================================================
* 030412 SAB  Create
* 030414 SAB  More reboot enable vars
* 030414 SAB  Move in PLCRebootCmd
* 030425 SAB  Add Flt_Summ_Typ prototype and extern for FaultSummaryCheck
* 030515 JMN  Restructure bits in Flt_Summ_Typ to allow separate trip and alarm
*             summary.
* 030524 JMN  Add CanError codes
* 031015 JMN Rename signals to support CAN user drawing
* 031018 SAB Add open phase loss functionality
* 031018 SAB Add more open phase loss vars and function "OpenPhaseCheckT2"
* 031018 SAB Correct spelling of 'SinEFbkT2'
* 031021 SAB Add more open phase vars and remove .....3 open phase vars.
* 031120 SAB Add open grid vars
* 031124 SAB Add more open grid vars
* 031202 SAB Add OpnGrdVMgfil
* 031202 SAB Remove OpnGrdVMgfil
* 040105 SAB Add new prototype
* 040119 SAB Add new vars and prototype for LowVoltageConfigProtectionT3.
* 040126 SAB Rename var VSecThr --> VSecDyn.  Add new var.
* 060228 SAB Add OpnGrdTmr
* 060403 SAB Add FiltCabOTTmr
* 061205 SAB Remove FiltCabOTTmr
* 070417 SAB Add GridMonitorTocDetection
* 070419 SAB  Add distortion filter vars
* 070508 SAB  Rename some distortion filter vars to accomodate new Y-eq current calcs
* 070601 SAB  Remove "Lvcg" items.
* 070612 SAB  Add device voltage mis-match protection variables and and
*             extern for OpenGridCheckT2()
* 060614 SAB  Add two grid open fault flags and stator fault flags.
* 070628 IB   Add references for the cooling system monitor
* 070629 SAB  Add declarations for new current error protection and rename some vars
* 070706 SAB  Add power imbalance grid loss protection code.
* 070801 SAB  Add new open grid vars
* 070913 IB   Add L_BrgLssComp, R_BrgLssComp, and reference BrgdTotalPowerLoss().
* 070930 IB   Add CoolTempTmr to delay modeled coolant temperature.
* 071104 IB   Add CoolTModRun and CoolStatSum
* 071119 IB   Add CoolSimFlag
* 071205 IB   Add L_BrgLssFil and R_BrgLssFil
* 080110 SAB  Remove GmSVMtchErFl
* 080131 SAB  Remove CosEFbkT2, SinEFbkT2
* 080701 SAB  Add OpnGrdTmDyn
* 080823 SAB  Add BpRiseHiFilW
* 080827 SAB  Create LnBrgOff1Dt and RtBrgOff1Dt
* 081006 SAB  Add vars for RPNW open phase detection
* 090805 SAB  Move in torsional detection gain vars from P0Rule
* 090810 SAB  Add  NV.R_TorSpdPTrp
* 090817 SAB  Add R_TorSpdBpf0, R_TorSpdBpf1, R_TorSpdZ1
* 090826 SAB  R_SpdMtr --> R_SpdGen
* 110825 SAB  Bring in Torsional torque changes from Wind-DFIG
* 100712 SAB  Add R_TorTrqFil
*
*==============================================================================
*/

#ifndef __P0FLTCHK_H__
#define __P0FLTCHK_H__

// Constants
//----------
#define GRIDFLT_NONE             0
#define GRIDFLT_1PHOPN_SEC6K_A   1
#define GRIDFLT_1PHOPN_SEC6K_B   2
#define GRIDFLT_1PHOPN_SEC6K_C   3
#define GRIDFLT_1PHOPN_PRIM_A    4
#define GRIDFLT_1PHOPN_PRIM_B    5
#define GRIDFLT_1PHOPN_PRIM_C    6
#define GRIDFLT_3PHOPN           7
#define GRIDFLT_FASTOV           8
#define GRIDFLT_VSECOV           9
#define GRIDFLT_FASTDIS          10
#define GRIDFLT_FASTBLK          11
#define GRIDFLT_PLLPHASE         12
#define GRIDFLT_TORSLIP          13

#define FASTD_NORMAL             0
#define FASTD_RAMP_DOWN          1
#define FASTD_Q1_OPEN_WAIT       2
#define FASTD_K1_OPEN_WAIT       3

#define HVAC_NORMAL              0
#define HVAC_HVDET               1
#define HVAC_WAIT                2
#define HVAC_FRQCHK              3
#define HVAC_HVRT_DET            4
#define HVAC_ISLAND              5
#define HVAC_Q1_OPEN_REQ         6
#define HVAC_Q1_OPEN_WAIT        7
#define HVAC_K1_OPEN             8

#endif
