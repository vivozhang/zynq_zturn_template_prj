/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        TMS320VC33 DSP
* File:        $\LVP2SRC\FW\S0VARCAL.C30
* Language:    TI 'C'
* Copyright:   1996 GE Motors and Industrial Systems, Salem VA
*
* Abstract: This file contains those functions which are associated with
*           variable conditioning.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
*
*==============================================================================
* 970120 0001 SAB Create for regulator control and parameter conditioning.
* 970120 0002 SAB Include S0EEEMUL.H, not A0EEEMUL.H
* 970129 0003 SAB Populate Vln_In_Sim, Vln_In, Vln_In_Rcp
* 970131 0004 SAB Populate Vreg_Cap_Gn
* 970203 0005 SAB Add current regulator conversions.
* 970205 0006 SAB Add plant simulator conversions.
* 970207 0007 SAB Rename Vreg_Cap_Gn to Vreg_Wo_Comp and and add EE.Vreg_Wo_Gn
*                 Add calc's for X_Lw_Pll, X_Ls_Pll
* 970214 0008 SAB Change calculations which used 2pi*Pll_Frq --> Pll_Frq.
*                 Rename EEs and variables appropriately.
* 970221 0009 SAB Add charge resistor themal model scalings.
* 970225 0010 SAB
* 970306 0011 SAB Rename Z_Thrm_R_Rcp to R_Thrm_R_Rcp.  Rename Z_Thrm_L_Rcp to
*                 R_thrm_L_Rcp.  Change parameter range check for Ioc_Thrs_Req
*                 from P.I_Rat*2.5 to I_Rat*sqrt(2)*2.5
* 970310 0012 SAB Form Gnd_Delt_Rcp
* 970318 SAB  Header file comments and use products.h
* 970321 SAB  Remove un-used vars
* 970324 SAB  Move ram defs internal
* 970422 SAB  Remove Vln_Scl calculation
* 970424 SAB  Remove Gnd_Delt_Rcp
* 970507 SAB  Rename Vdc_Nom --> Vdc_Nom_Cal
* 970520 SAB  Rename EE.CI.L_Line-->EE.CI.L_Tot, L_Line_Cal--> L_Tot_Cal
* 970626 SAB  Copy EE.Iy_Ref to V.Iy_Reference
* 970718 RCB  Enable Ram_Cfg_Cmd.Cur_Lim_Enb based on Ioc_Rtry_Req.
* 970827 SAB  Add ram copy of final values for tool use only
* 970910 SAB  Zero Vreg_Wo_Comp
* 970917 SAB  Add inint of Vdc_Nom_Cal
* 971020 SAB  Remove Iyrf_PF_Stpt
* 971118 SAB  Remove L_W_Rcp_Sim
* 971120 SAB  Zero variables at init
* 980105 SAB  Add SQRT2 to calculation of Ioc_Thres clamp
* 980226 SAB  Remove calc of C_Thrm_L_Rcp, R_Thrm_L_Rcp
* 980507 SAB  Add Vreg_Wo_Comp
* 980601 SAB  Remove extern for Sim_Mode
* 980610 SAB  Remove var calcs not part of LCL
* 981012 SAB  ACMVRS6_A: Add Res_WI_Rcp
* 981118 SAB  Add Ph_Imb_Spilx, Ph_Imb_Tmx
* 990701 SAB  ACMVRS6_A:  Add Nc_Inh_Lvl
* 990712 SAB  ACMVRS6_A:  Calc Iyrf_Int_Gnx
* 990715 SAB  Use Iyrf_Int_Gnx for both products and change definition to
*             calc it as P.Iyrf_Int_Gn * 0.001/Del_Tm_2
* 991119 SAB  Changed L_Tot_Cal and L_Pll_Dt to be based on new FV defined
* 021030 SAB  Add Ns_X_Step, Ns_Y_Step population
* 021030 SAB  Remove zero value current regulator conditioning vars
* 021106 SAB  Populate Dc_X_Step and Dc_Y_Step
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 030124 SAB  ScaleCalc --> LineScaleCalc
* 030226 SCP  Add new calculations for filter.
* 030401 SCP  Remove L_RHrmFRcp and L_CHrmFDt
* 030402 SCP  Add L_RCHrmFRcp
* 031006 SAB  Add step vars for regulators
* 031006 SAB  Populate step vars made in last change
* 031006 SAB  Add simulator step var for line frequency (and populate)
* 040219 SAB  Remove IRAM_FLOATS--
* 501010 SAB  Rename some phase imbalance parms to pllerr parms.
* 070109 IB   Add L_LeqvChg.
* 070604 SAB  Remove a number of calc to rule file
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 080110 SAB  Move some var calcs to S0RULE
*______________________________________________________________________________

* Last Mod: October 9, 2008 7:47:39AM
*/

#include "x0FrameWork.h"
#include "Product.h"


// Vars

CREATE_PUBVAR(L_DcXStep                   ,float);      // Dc current regulator x channel step
CREATE_PUBVAR(L_DcYStep                   ,float);      // Dc current regulator y channel step
CREATE_PUBVAR(L_IxCmdZ1Sc                 ,float);      // x current regulator int. delay scale 1
CREATE_PUBVAR(L_IxCmdZ2Sc                 ,float);      // x current regulator int. delay scale 2
CREATE_PUBVAR(L_IxregStep                 ,float);      // Ix current regulator step
CREATE_PUBVAR(L_IyCmdA0Sc                 ,float);      // x current regulator avg. delay scale 0
CREATE_PUBVAR(L_IyCmdA1Sc                 ,float);      // x current regulator avg. delay scale 1
CREATE_PUBVAR(L_IyCmdD0Sc                 ,float);      // x current regulator d/dt delay scale 0
CREATE_PUBVAR(L_IyCmdD1Sc                 ,float);      // x current regulator d/dt delay scale 1
CREATE_PUBVAR(L_IyCmdZ1Sc                 ,float);      // x current regulator int. delay scale 1
CREATE_PUBVAR(L_IyCmdZ2Sc                 ,float);      // x current regulator int. delay scale 2
CREATE_PUBVAR(L_IyregStep                 ,float);      // Iy current regulator step
CREATE_PUBVAR(L_LTotCal                   ,float);      // Fast ram copy of EE.L_LTot
CREATE_PUBVAR(L_LTotDt                    ,float);      // EE.L_LTot*L_DelTm1Rcp
CREATE_PUBVAR(L_NsXStep                   ,float);      // negative seqeunce reg x channel step
CREATE_PUBVAR(L_NsYStep                   ,float);      // negative seqeunce reg y channel step
CREATE_PUBVAR(L_VregGnRcp                 ,float);      // voltage regulator gain reciprocal
CREATE_PUBVAR(L_VregStep                  ,float);      // DC link voltage regulator step


/*****************************************************************************
*
*       external references
*
*****************************************************************************/



/* **************************************************************************
*
*                               Private Variables
*
*****************************************************************************/

/*=============================================================================
 * Function: LineScaleCalc
 * Execution Level: Task 3
 * Execution Time: Untimed
 * Formal Inputs: None
 * Formal Outputs: None
 *
 *  Global Inputs:
 *                 P.L_NsXStpAdr
 *                 P.L_NsYStpAdr
 *                 P.L_DcXStpAdr
 *                 P.L_DcYStpAdr
 *                 P.L_VrgStpAdr
 *                 P.L_IxrgStpAdr
 *                 P.L_IyrgStpAdr
 *                 P.L_WGrdStpAdr
 *
 * Global Outputs:
 *                 V.L_NsXStep
 *                 V.L_NyXStep
 *                 V.L_DcXStep
 *                 V.L_DcYStep
 *                 V.L_VregStep
 *                 V.L_IxregStep
 *                 V.L_IyregStep
 *                 V.L_WGrdFrqStp
 *
 * Locals:         none
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function calculates logical control variables which are used to
 * determine the sense of switches throughout the control algorithm.
 *
 *===========================================================================*/


void LineScaleCalc()
{
    L_NsXStep    = READ_PARMPTR(L_NsXStpAdr);
    L_NsYStep    = READ_PARMPTR(L_NsYStpAdr);

    L_DcXStep    = READ_PARMPTR(L_DcXStpAdr);
    L_DcYStep    = READ_PARMPTR(L_DcYStpAdr);

    L_VregStep   = READ_PARMPTR(L_VrgStpAdr);
    L_IxregStep  = READ_PARMPTR(L_IxrgStpAdr);
    L_IyregStep  = READ_PARMPTR(L_IyrgStpAdr);
}
