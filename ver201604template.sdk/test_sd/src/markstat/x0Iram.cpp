/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        A0SPD.C30
* Language:    TI 'C'
* Copyright:   2003.... GE Drives and Controls, Inc., Salem VA
*
* Abstract: This module contains speed feedback related functions
* -----------------------------------------------------------------------------
* Functions in module:
*==============================================================================
* 070612 SAB  Created for declaration of IRAM variables
* 070619 SAB  Add line and stator pll IRAM parameter copies.
* 070622 SAB  Add CI_L_IRAFEnbTm
* 070703 IB   Add four IRAM copies under Y0 and one under P0.
* 070705 SAB  Add move IRAM parameter copies
* 070709 SAB  Add more IRAM parameter copies for Y1Task.c30, tec
* 070709 SAB  Add CI_PwrErrIMagThr
* 070716 SAB  Rename PwrErrIMagThr to PwrErrPwrThr
* 070717 BAG  remove redundant variables
* 070801 SAB  Add new open grid, etc EE copies
* 071003 SAB  Add CI_L_XfmrPll
* 071031 SAB  Add CI_S_VrfSyncScl
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 071220 SAB  Add iram for line current reg 'Prp2' path
* 080115 SAB  Add CI_L_FbkFilGn and CI_L_IDiDtAttn, others
* 080122 SAB  Add "Pk" suffix to some variables.  Add CI_IyLmPImbRPNWPk, others.
* 080228 SAB  Add lvrt torque reduction variables
* 080311 SAB  Remove CI_GmPllIntFilW
* 080311 SAB  Remove CI_S_IxCompLim
* 080312 SAB  Add variables for ac voltage regulator dynamic clamp
* 080324 SAB  Rename R_IxVdcLimPk to R_IxCmd0LimPk
* 080519 SAB  Add CI_GmCvAsmMin1, Min2
* 080519 SAB  Add CI_GmAngShiftT2
* 080521 SAB  Add CI_R_WSpdFbkPLC
* 080529 SAB  Add vdc ov current reduction parameter copies
*             Add new field weakening parameter mirrors
* 080627 SAB  Add CI_T1MissedThr, CI_T2MissedThr
* 080630 SAB  Remove extra declaration of CI_GmCVAsmMin1
* 080716 SAB  Add CI_L_T1MissedTh and CI_R_T1MissedThr
* 080805 SAB  Add CI_R_IabcMskTm
* 080822 SAB  Add CI_ZVZeroIySpd, CI_ZVQckRcvyTm, CI_ZVStGtVdcThr, CI_ZVStopGtTm,
*             CI_ZVStopGtTm2, CI_ZVGtOfMxTm, CI_ZVPllPrmTm, CI_ZVGtOfMxTmSt,
*             CI_ZVPllPrmTmSt, CI_XStpAdr
* 080823 SAB  Add BpRiseHiFlGn
* 080901 SAB  Add CI_R_IocMskVdc, CI_R_IocMskVGm
* 080909 SAB  Add excursion detection iram
* 080916 SAB  Add CI_IyAsySpdOn, ohters
* 080924 SAB  Add VrgIyLmNWAsmPk
* 080925 SAB  Add VrgIyLmGmAsmPk
* 080930 SAB  Add CI_VregNSqHiThr
* 081002 SAB  Add CI_R_VMaxIntGn, others
* 081006 SAB  Add RPNW open grid fault parameter copies and gains
* 080116 SAB  Add GmPllIntGnI1
* 081113 SAB  Remove extra occurrence of CI_GmPllPrpGnI1
* 081113 SAB  Add CI_LvTrqLmPBScl
* 090428 SAB  Add new WindBOOST vars
* 090730 SAB  Add CI_R_TorSimEnb
* 090810 SAB  Add GridStrength, GridCompType, GridHighZ
* 090817 SAB  Add CI_PulseKillThr
* 090820 SAB  Add more pulse generator vars
* 090826 SAB  Add new parms for fractional tach algorithm
*             Add DelTm2Rcp2x, DelTm2RcpSqr
* 090827 SAB  Add CI_TorTstSlwRat
* 090831 SAB  Remove R_WSpdFst, rename R_WSpdT1 to R_WSpdT2
* 090831 SAB  PulseTorque --> PulseGenPu
* 090901 SAB  Rename CI_VregTstCmd to VregTstCmd
* 110823 SAB  Bring in internal ram from S0Rule and others
*______________________________________________________________________________

* Last Mod: September 14, 2011 5:38:20PM
*/
#include "x0Framework.h"
#include "product.h"
#include "x0Iram.h"



/******************************************************************************
*
* Abstract:
*
*     This file is designed for the purpose of declaring IRAM varaibles for files
*  that normally reside in SRAM.  The macros IRAM_FLOAT(), IRAM_UINT(), etc were
*  desinged for this purpose, but they place the newly declared IRAM variables
*  in the IRAM section instead of the IRAM2 section, allowing them to compete
*  for space with the stack.  This allows eventual stack over-flows and may
*  prevent the CPU from booting up.
*     It is a good practice to separate the declarations by files, or at least
*  by layers (A0, S0, P1, P2, etc)
*     Note that for the "CI_" items that are declared here and populated in
*  a rule file, each correspoding tre file entry must have a 'calcrequired{yes}
*  and each CI_...  must be set equal to the corresponding EE in the rule file.
*
******************************************************************************/


///////////////////
// Global variables
///////////////////

///////////////////
//   P0FltChk.c30
//////////////////
CREATE_PUBVAR(IOpn1AbsNWGn        , float);

CREATE_PUBVAR(R_IxCmd0LimPk         , float) ;

///////////////////
//   A0DEMROT
//////////////////

CREATE_PUBVAR(R_AngEstFFGn      , float);
CREATE_PUBVAR(R_SpdT2RAFN    , unsigned);
CREATE_PUBVAR(R_SpdTcRAFN    , unsigned);


///////////////////
//   AVARCAL
//////////////////

CREATE_PUBVAR(R_IRatSSLimPk         , float);

///////////////////
//   A0STATOR
//////////////////
CREATE_PUBVAR(PllErPwrFlGn          , float);

CREATE_PUBVAR(OneMnTPllMeld         , float);





///////////////////
//   S0Vreg.c30
//////////////////
CREATE_PUBVAR(L_VdcIxIyFlGn       , float);


/////////////////////////////////////////////////////////////////////
//Y1 FILES
/////////////////////////////////////////////////////////////////////


///////////////////
//   X0Init.c30
//////////////////
CREATE_PUBVAR(DelTm2Rcp2x           , float);
CREATE_PUBVAR(DelTm2RcpSqr          , float);


