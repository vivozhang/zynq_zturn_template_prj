/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        A0Rule.h
* Language:    TI 'C'
* Copyright:   1996..8 GE Industrial Control Systems, Salem VA
*==============================================================================
* 070612 SAB  Created for declaration of IRAM variables
* 070618 SAB  Vars for A0Stator and some more for S0LINE
* 070622 SAB  Add CI_L_IRAFEnbTm
* 070703 IB   Add five new IRAM copies of EE.
* 070705 SAB  Add more IRAM parameter copies for Y1Task.c30, tec
* 070716 SAB  Rename PwrErrIMagThr to PwrErrPwrThr
* 070717 BAG  remove redundant variables
* 070801 SAB  Add new open grid EE copies
* 071003 SAB  Add CI_L_XfmrPll
* 071031 SAB  Add S_VrfSyncScl
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 071220 SAB  Add iram for line current reg 'Prp2' path
* 080115 SAB  Add CI_L_FbkFilGn, others
* 080122 SAB  Add "Pk" suffix to some variables.  Add CI_IyLmPImbRPNWPk
* 080228 SAB  Add lvrt torque reduction variables
* 080305 SAB  Remove CI_GmPllIntFilW
* 080311 SAB  Remove CI_S_IxCompLim
* 080312 SAB  Add variables for ac voltage regulator dynamic clamp
* 080324 SAB  Rename R_IxVdcLimPk to R_IxCmd0LimPk
* 080519 SAB  Add CI_GmCvAsmMin1, Min2
* 080519 SAB  Add CI_GmAngShiftT2
* 080521 SAB  Add CI_R_WSpdFbkPLC
* 080528 SAB  Add vdc ov current reduction parameter copies
*             Add new field weakening parameter mirrors
* 080627 SAB  Add CI_T1MissedThr, CI_T2MissedThr
* 080716 SAB  Add CI_L_T1MissedTh and CI_R_T1MissedThr
* 080805 SAB  Add CI_R_IabcMskTm
* 080822 SAB  Add CI_ZVZeroIySpd, CI_ZVQckRcvyTm, CI_ZVStGtVdcThr, CI_ZVStopGtTm,
*             CI_ZVStopGtTm2, CI_ZVGtOfMxTm, CI_ZVPllPrmTm, CI_ZVGtOfMxTmSt,
*             CI_ZVPllPrmTmSt, CI_XStpAdr
* 080823 SAB  Add BpRiseHiFlGn
* 080901 SAB  Add CI_R_IocMskVdc, CI_R_IocMskVGm
* 080909 SAB  Add peak detection iram
* 080916 SAB  Add CI_IyAsySpdOn
* 080924 SAB  Add VrgIyLmNWAsmPk
* 080925 SAB  Add VrgIyLmGmAsmPk
* 080930 SAB  Add CI_VregNSqHiThr
* 081002 SAB  Add CI_R_VMaxIntGn, others
* 081006 SAB  Add RPNW open grid fault parameter copies and gains
* 080116 SAB  Add GmPllIntGnI1
* 081113 SAB  Add CI_LvTrqLmPBScl
* 090428 SAB  Add new WindBOOST vars
* 090805 SAB  Add CI_R_TorSimEnb and pulse genenerator parm copies
* 090810 SAB  Add GridStrength, GridCompType, GridHighZ
* 090817 SAB  Add CI_PulseKillThr
* 090820 SAB  Add more pulse generator vars
* 090826 SAB  Add new parms for fractional tach algorithm
*             Add DelTm2Rcp2x, DelTm2RcpSqr
* 090827 SAB  Add CI_TorTstSlwRat
* 090831 SAB  Remove R_WSpdFst, rename R_WSpdT1 to R_WSpdT2
* 090831 SAB  PulseTorque --> PulseGenPu
* 090901 SAB  Rename CI_VregTstCmd to VregTstCmd
* 110823 SAB  Bring in internal Iram vars from S0Rule and others
*______________________________________________________________________________
* Last Mod: July 03, 2007 08:19:25AM
*/

#ifndef X0IRAM_H
#define X0IRAM_H
#endif 
