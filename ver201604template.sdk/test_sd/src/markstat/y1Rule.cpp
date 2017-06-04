///////////////////////////////////////////////////////////////////////////////
//
// TITLE     Rules for the bridge interface layer
//
// DESCRIPTION
//           Contains code to set parameters related to BIL and detection of
//     default ground faults.
//
// COPYRIGHT    Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include  "x0FrameWork.h"
// product
#include "Product.h"
#include "y1Rule.h"


// Constants
//----------


// Externals
//----------
extern  float   DelTm3;
extern  float   R_DelTm1Rcp;
extern  float   R_GndAlmOff;
extern  float   R_GndAlmOn;
extern  float   R_GndFltThrs;
extern  float   R_IabcFltThr;
extern  float   R_IabcMskTm;
extern  float   R_IocMskVdc;
extern  float   R_IocMskVGm;
extern  float   R_LkoutGn;
extern  float   R_LkoutLim;
extern  float   R_ProtClass;
extern  float   R_TocAlmOff;
extern  float   R_TocAlmOn;
extern  float   R_TocTau;
extern  float   R_TocThrs;
extern  float   R_VdcNom;
extern  float   R_VdcUVLvl;
extern  float   R_WIabcGnd;
extern  float   R_WIxIyGnd;
extern  float   R_WIxyGnd;

extern float    R_VdcNomRcp;
extern float    R_DelTm1;
extern float    R_DelTm1Rcp;
extern int      R_T1FramesPerT2;
extern int      T2FramesPerT3;

// Types and Classes
//------------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------
int  RotorBrgInfcRule(void);
int  RotorDefaultGroundFlt(void);


// Class Instances
//----------------


// Parameters
//-----------


// Published Variables
//--------------------
CREATE_PUBVAR(R_VllRule,     float);          // Vll calculated for rules using Vll
CREATE_PUBVAR(R_VllRuleRgn,  float);          // Vll calculated value for regen bus regulation rules
CREATE_PUBVAR(R_CTCurLvl,    float);          // Target current for rotor cell test (Amps)
CREATE_PUBVAR(L_CTCurLvl,    float);          // Target current for line cell test (Amps)
CREATE_PUBVAR(R_TempVcoCalcSt,   int);
CREATE_PUBVAR(R_TempVcoDelTmRcp, float);


// Diagnostics
//--------------------


// Unpublished Variables
//----------------------


// Data Pass
//----------


// Functions
//----------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Select ruleset to run
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      Status to indicate if calculations were successful (SUCCESS/FAIL).
//
///////////////////////////////////////////////////////////////////////////////
int RunRotorBILRules(int Rule_Set)
{
    int     Status=SUCCESS;

    switch(Rule_Set)
    {
        // calculate main control final values
        case RULE_SET_MAIN:
            Status = RotorBrgInfcRule();
            break;

        // Run again because dependent on main rule set.
        case (RULE_SET_MAIN + RULE_SET_SECOND_PASS):
            Status = RotorBrgInfcRule();
            break;

        // set load control parameters based on final values
        case RULE_SET_DEFAULT_ALL:
            if (   ((Status = RotorBrgInfcRule())     == SUCCESS)
               )
                Status = RotorDefaultGroundFlt();
            break;

        // Run again because BrgInfcRule dependent on MtrCtlRule set.
        case (RULE_SET_DEFAULT_ALL + RULE_SET_SECOND_PASS):
            Status = RotorBrgInfcRule();
            break;


        // Set Default Ground Fault Settings
        case RULE_SET_DEFAULT_GROUND_FLT:
            Status = RotorDefaultGroundFlt();
            break;

        // Set motor control parameters
        case RULE_SET_MTR_CTL_SETUP:
        case (RULE_SET_MTR_CTL_SETUP + RULE_SET_SECOND_PASS):
                Status = RotorBrgInfcRule();
            break;

       // Default case
       default:
            break;
    }

    return (Status);
}   // RunRotorBILRules()


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rules for calculating bridge interface final values from
//      tuneup results.  The drive must be stopped or the
//      calculations are not performed.
// Note:  Do not alter the rule calculation sequence.
//        This function uses PARM(S_R1) and PARM(S_R2) for assumed resistances.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
int RotorBrgInfcRule(void)
{
    float   Temp1, Temp2;
    float   DC_Link;
    int     Return_Code = SUCCESS;  // 1 = success, 0 = fail
    int     Rng_Sel_Dts;

    ///////////////////////////////////////////////////////////////////////
    //  Convert Rotor delta-vco counts to volts
    ///////////////////////////////////////////////////////////////////////

    // R_TempVcoCalcSt must be equal to 8*k (k = integer)
    // and less than DelTm3 - 8 task1's - range_select propagation time
    // TMT measurment shows 700us delay between ldpls and range change on AEAD
    Rng_Sel_Dts       = 2 + static_cast<int> (ceil(700e-6F * R_DelTm1Rcp));
    R_TempVcoCalcSt   = (((R_T1FramesPerT2 * T2FramesPerT3) - 8 - Rng_Sel_Dts) / 8) * 8;
    R_TempVcoDelTmRcp = 1.0F / ((static_cast<float> (R_TempVcoCalcSt))*R_DelTm1);
    //FScratch4 = FScratch4 + DelTm3;

    // Maximum vll check and dc link feedback fault levels


    R_VllRule = PARM(R_VllOut);

    if (R_VllRule > PARM(R_VllMax))
    {
        R_VllRule = PARM(R_VllMax);
    }

    Temp2 = SQRT2 * 0.5F * R_VllRule;

    // max DC link voltage is 1400 volt, give 50 volt for margin
    SET_FV(R_VdcUVLvl, Temp2);                            // Vdc undervoltage threshold--same for both converters

    SET_FV(R_WVdcErr,   1.0F);      // Vdc error filter omega
    SET_FV(R_VdcErrThr, 0.012F);    // Vdc error alarm threshold (pu)
    SET_FV(R_CroBarVdcEnbThrs, (PARM(DBVdcThrs) + 5.0F));

    // R_VdcNom Calculation
    DC_Link = PARM(L_VdcRef);
    SET_FV(R_VdcNom, DC_Link);
    R_VdcNomRcp=1.0F/PARM(R_VdcNom);


    // Regen bus regulation

    Temp2 = 1.17F;                        // 1.17 highest DB kick in for PWM2
    Temp1 = SQRT2 * Temp2 * R_VllRule;

    R_VllRuleRgn = Temp1;



    // Set detection level for open circuit cell test. With .5 pu shunt as target
    // current level, line converter passes with CT_I_Thr_Pct at up to 13 %.
    // Note this is for an average measurement so detection level is not 1:1 with
    // target level (more like 1:3 on rotor and 1:4 on line. */

    L_CTCurLvl = 0.50F * PARM(L_ShuntRat);  // Line converter cell test target current level
                                            // this is only for display purposes, because Line
                                            // cell test instance does not use this signal

    R_CTCurLvl = 1.0F * PARM(R_ShuntRat);   // Rotor converter cell test target current level
                                            // note: 1 pu shunt is min needed to get
                                            // S/N on vfb compatable with CT_V_Thr_Pct.


    //Vll_Temp = PARM(Vll_Input);  // Clamp Vll_In below PARM(R_VllMax)//needs review
    //if ( Vll_Temp >= PARM(R_VllMax))
    //{
    //     Vll_Temp = PARM(R_VllMax);
    //}
    //SET_FV(Vll_In, Vll_Temp);   //Careful! -- This is used later in calc of Ireg_Spil_Gn


    SET_FV(R_WIabcGnd, 1000.0F);
    SET_FV(R_IabcFltThr,  0.4F  * PARM(R_ShuntRat)); // 970718 per AMK
    SET_FV(R_IabcMskTm,   0.008F);  //set from experience of Spain 2008 LVRT tests
    SET_FV(R_GndFltThrs,  0.10F * PARM(R_ShuntRat));
    SET_FV(R_GndAlmOn,    0.03F * PARM(R_ShuntRat));
    SET_FV(R_GndAlmOff,   0.02F * PARM(R_ShuntRat));


    // miscellaneous  - parms formerly not final values

    SET_FV(R_WIxIyGnd,   5.0F);
    SET_FV(R_WIxyGnd,    0.2F);

    return (Return_Code);

}   // RotorBrgInfcRule()


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rotor default ground fault rules
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
int RotorDefaultGroundFlt(void)
{
                                    // changed assignments to SET_FVS above
    return (0);                     // fail - shouldn't be calling this anymore
}   // RotorDefaultGroundFlt()
