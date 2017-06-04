/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        TMS320C31 DSP
* File:        P2Rule.h
* Language:    TI 'C'
* Copyright:   1996 GE Motors and Industrial Systems, Salem VA
*==============================================================================
* 030204 SAB  Create from P2Rule.h
* 040122 SAB  Add vars, LvCfgSlope, LvCfgYint
* 051209 SAB  Add L_PllWNomSim
* 070106 SAB  Add reciprocals, TempRMldRcp, TempRMldRcp2
* 070202 SAB  Add T1T2FbkScale and RAF vars
* 070210 SAB  Rename all "Line" and "Ln" stuff to "Gm"
* 070220 SAB  Add fast grid monitor over-voltage vars
* 070328 SAB  Add VlnNomRms
* 070414 SAB  Add CapRuleCntr
* 070423 SAB  Add new vars, IyUtilMaxDt, IyUtilMinDt
* 070425 SAB  Add reciprocals for R, S_IMagOL
* 070501 SAB  Add VregPrcFilGn, others
* 070508 SAB  Remove TempRMldRcp2
* 070508 IB   Add KDFtotTm.
* 070522 SAB  Add new grid monitor protection curve variables
* 070529 SAB  Add GmCnvOVMinx and GmCnvOVThrx
* 070604 SAB  Remove "Lvcg" items.
* 070608 SAB  Add vars for UV,OV, UF, OF config faults
* 070611 SAB  Add externs for device voltage mis-match faults
* 070628 IB   Reference vars that pre-process cooling syst monitor rules
* 070912 SAB  Add GmUVMaxInt, GmUVMaxFrc
* 070913 IB   Add CoolRHeatRcp.
* 071024 SAB  Add GmConfig
* 071106 SAB  Add R_RIndCorRcp, L_RIndCorRcp
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 071213 IB   Add L_BrgLssGn and R_BrgLssGn
* 080122 SAB  Remove VregIyLimPk. Add IyUtilMaxDt0, MinDt0
* 080320 SAB  Remove "IyUtility" vars
* 080519 SAB  Rename VlnNomRmsRcp to VlnPeakRcp, add conv asym vars
* 080520 IB   Rename KDFtotTm as KDFQPwrTm.
* 080710 IB   Reference new parms to improve coolant-rise estimation
* 080903 IB   Remove references to variables CoolantRth, CoolantOff, CoolLnRth,
*             and CoolLnOff.
* 090428 SAB  Add WindBOOST variables
* 090810 SAB  Add defines for grid characteristics
* 090922 SAB  Add WBgmILmSqr
* 090706 SAB  Add new var, CableLimType
* 110826 SAB  Bring in SC Wind-DFIG changes
*______________________________________________________________________________
* Last Mod: September 03, 2008 03:47:22PM
*/
#ifndef P0RULE_H
#define P0RULE_H

// Types and Classes
//------------------
// input structure to 2nd order filter coefficient calculator
struct filt2_in_type
{
    float A0;    // S-domain coefficients
    float A1;
    float A2;
    float B1;
    float B2;
    float dt;    // execution time
};
// output structure from 2nd order filter coefficient calculator
struct filt2_out_type
{
    float   a0;  // Z-domain coefficients
    float   a1;
    float   a2;
    float   b1;
    float   b2;
};


#endif
