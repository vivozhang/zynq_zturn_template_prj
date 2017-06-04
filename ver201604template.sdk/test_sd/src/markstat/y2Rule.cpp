///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rules for Bridge interface layer
//
// DESCRIPTION:
//      Execute source BIL specific rule sets.
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// framework
#include  "x0FrameWork.h"
// product
#include "Product.h"
#include "y2Rule.h"


// Externals
//----------

extern float        DelTm1Rcp;
extern float        DelTm2Rcp;
extern float        DelTm3;
extern float        L_DelTm1;
extern float        L_DelTm1Rcp;
extern float        L_IRAFCntFlt;
extern float        L_IRAFGnDiff;
extern float        L_IRAFGnInnr;
extern float        L_IRAFGnOutr;
extern int          L_IRAFCnt;
extern float        L_IMesCntRcp;
extern float        DelTm1;
extern float        DelTm1A;
extern unsigned     L_IMeasCntRq;
extern float        L_VllInRcp;
extern float        L_IxyGndGn;
extern float        L_IxIyGndGn;
extern float        L_DelTm1Rcp;
extern int          L_T1FramesPerT2;
extern int          T2FramesPerT3;

// Constants
//----------
#define TOC_ALM_SCL    1.0776F // = *SQRT( (1.0*1.0) + (1.15*1.15)/2.0 )  -- Heating 1/2 way from heated to trip


// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(L_VllMax,           float);     // Maximum line-line inverter output voltage
CREATE_PARM(L_VdcMax,           float);     // Maximum setting of overvoltage flt level
CREATE_PARM(L_VllOut,           float);     //
CREATE_PARM(L_VllInput,         float);     // Line to line input volts


// Diagnostics
//------------


// Variables
//----------
CREATE_PUBVAR(L_VllRule,        float);     // Vll calculated for rules using Vll
CREATE_PUBVAR(L_TempVcoCalcSt   ,int) = 48;
CREATE_PUBVAR(L_TempVcoDelTmRcp, float);


// Unpublished Variables
//----------------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------
int  LineBrgInfcRule(void);


// Data Passing
//--------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Select the Bridge-Interface-Layer ruleset to run.
//
// EXECUTION LEVEL
//      Initialization or ADL Thread
//
// RETURN VALUE
//      Status to indicate if calculations were successful (SUCCESS/FAIL).
//
///////////////////////////////////////////////////////////////////////////////
int RunLineBILRules(int Rule_Set)
{
    int     Status=SUCCESS;

    switch(Rule_Set)
    {
        // calculate main control final values
        case RULE_SET_MAIN:
            Status = LineBrgInfcRule();
            break;

        // Run again because dependent on main rule set.
        case (RULE_SET_MAIN + RULE_SET_SECOND_PASS):
            Status = LineBrgInfcRule();
            break;

        // set load control parameters based on final values
        case RULE_SET_DEFAULT_ALL:
            if (   ((Status = LineBrgInfcRule())     == SUCCESS)
               )
                Status = true;
            break;

        // Run again because BrgInfcRule dependent on MtrCtlRule set.
        case (RULE_SET_DEFAULT_ALL + RULE_SET_SECOND_PASS):
            Status = LineBrgInfcRule();


        // Set Default Ground Fault Settings
        case RULE_SET_DEFAULT_GROUND_FLT:
            break;

       // Default case
       default:
            break;
    }

    return (Status);
}   // RunLineBILRules()


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//     Rules for calculating bridge interface final values from
//     tuneup results. The converter must be stopped or the calculations
//     are not performed.
//     Do not alter the rule calculation sequence.
//
// EXECUTION LEVEL
//      Initialization or ADL Thread
//
// RETURN VALUE
//      Status to indicate if calculations were successful (SUCCESS/FAIL).
//
///////////////////////////////////////////////////////////////////////////////
int LineBrgInfcRule(void)
{
    float   Temp1;
    float   Temp2;
    int     Return_Code = SUCCESS;  // 1 = success, 0 = fail
    float   Vll_Temp;
    int     Rng_Sel_Dts;

    ///////////////////////////////////////////////////////////////////////
    //  Convert Line delta-vco counts to volts
    ///////////////////////////////////////////////////////////////////////

    // L_TempVcoCalcSt must be equal to 8*k (k = integer)
    // and less than DelTm3 - 8 task1's - range_select propagation time
    // TMT measurment shows 700us delay between ldpls and range change on AEAD
    Rng_Sel_Dts       = 2 + static_cast<int> (ceil(700e-6F * L_DelTm1Rcp));
    L_TempVcoCalcSt   = (((L_T1FramesPerT2 * T2FramesPerT3) - 8 - Rng_Sel_Dts) / 8) * 8;
    L_TempVcoDelTmRcp = 1.0F / ((static_cast<float> (L_TempVcoCalcSt))*L_DelTm1);

    SET_FV(L_VllMax,   PARM(SysVoltNom));
    SET_FV(L_VdcMax,   PARM(SysVoltNom) * 2.029F);
    SET_FV(L_VllOut,   PARM(SysVoltNom));
    SET_FV(L_VllInput, PARM(SysVoltNom));

    Vll_Temp = PARM(L_VllInput);  // Clamp L_VllIn below PARM(L_VllMax)
    if ( Vll_Temp >= PARM(L_VllMax))
    {
        Vll_Temp = PARM(L_VllMax);
    }
    SET_FV(L_VllIn, Vll_Temp);   //Careful! -- This is used later in calc of L_IregSpilGn
    L_VllInRcp = 1.0F/PARM(L_VllIn);

    //----------------------------------------------------
    // Maximum vll check and dc link feedback fault levels
    L_VllRule = PARM(L_VllOut);

    if (L_VllRule > PARM(L_VllMax))
        L_VllRule = PARM(L_VllMax);

    Temp1 = SQRT2 * 1.434709F *L_VllRule;
    Temp2 = SQRT2 * 0.5F * L_VllRule;

    if ( Temp1 > PARM(L_VdcMax) )
        Temp1 = PARM(L_VdcMax);

    // Max DC link voltage is 1400 volt, give 50 volt for margin
    if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
    {
       SET_FV(L_VdcOVLvl, 1400.0);
    }
    else
    {
       SET_FV(L_VdcOVLvl, 1350.0);
    }

    SET_FV(L_VdcUVLvl, Temp2);    // L_Vdc undervoltage threshold--same for both converters


    SET_FV(L_IocThrsReq, 2.59F*PARM(L_MaxCur)*SQRT2); // adjusted from 2.4 to 2.59 to give old value of 1,154 amps
    SET_FV(L_IocRtryReq, 0.010F);                     // (sec)
    SET_FV(L_IocRtryMax, 0.016F);                     // (sec)
    SET_FV(L_IocDwnRtio, 0.100F);                     // ratio of down timer to up timer


    SET_FV(L_IDiDtAttn, 1.0F);

    SET_FV( L_IabcFltThr, 0.4F   * PARM(L_ShuntRat));
    SET_FV( L_GndFltThrs, 0.10F  * PARM(L_ShuntRat));
    SET_FV( L_GndAlmOn,   0.03F  * PARM(L_ShuntRat));
    SET_FV( L_GndAlmOff,  0.02F  * PARM(L_ShuntRat));

    SET_FV( L_WIxIyGnd,   5.0F);
    SET_FV( L_WIxyGnd,    0.2F);
    SET_FV( L_WIabcGnd, 357.0F);

    // Calculate filter gains
    L_IxyGndGn  = PARM(L_WIxyGnd)  * DelTm3;
    L_IxIyGndGn = PARM(L_WIxIyGnd) * DelTm3;

    SET_FV( L_IabcDcFltThr, 0.05F * PARM(L_ShuntRat));
    SET_FV( L_WIabcDcGnd,  3.0F);


    SET_FV( L_FbkFilGn, 1.339F);


    // 0 = "Original"
    // 1 = "Ireg LP filter:
    // 2 = "Feedback LP filter;
    // 3 = "Both"
    SET_FV( L_IregFbkTyp, 3);


    SET_FV(L_MAClsVdcMn, 260.0F + 100.0F); //Turn off db 100volts before power-down threshold if MA not closed.



     // IntRate/FrqInput gives task 1's per line cycle.
     // 3 line AC ducy cycles. May want to change to to 6 cycles later
     // due to the DB is executed on the rotor side rate, a ratio is added to have the right line AC cycles
     SET_FV(L_MAClsIMsCt, ( (unsigned)( (0.5F + (3.0F * DelTm1Rcp / PARM(L_FrqInput))) ) ));

     L_IMeasCntRq = PARM(L_MAClsIMsCt);
     L_IMesCntRcp = 1.0F/(PARM(L_MAClsIMsCt)*L_DelTm1);

   //-----------------
   // End BIL DB rules


    //--------------------------
    // Iram copies of parameters


    // RAF for line and statot DC ireg
    SET_FV(L_IRAFEnbTm, (2.0F/PARM(L_FrqInput)));          //twice the period of the ac line
    L_IRAFCntFlt = 1.0F/(PARM(L_FrqInput))*DelTm2Rcp;      // L_DelTm1 is lower limited in Y0INIT.h to protect
    L_IRAFCnt    = ( ((int) (L_IRAFCntFlt))  + 2);        // this calculation
    if ( L_IRAFCnt > 22)
    {
         L_IRAFCnt = 22;
    }
    L_IRAFGnInnr = 1.0/L_IRAFCntFlt;
    L_IRAFGnOutr = (1.0F - (  L_IRAFGnInnr*(L_IRAFCnt - 2)  )   )/2.0F;
    L_IRAFGnDiff = L_IRAFGnInnr - L_IRAFGnOutr;




    return (Return_Code);
}   // LineBrgInfcRule()
