///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rotor side product rules
//
// DESCRIPTION:
//      This file contains rule calculations for rotor control final values
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// core
//framework
#include "x0FrameWork.h"
#include  "b0BridgeRebf.h"
// product
#include "Product.h"

// Constants
//----------
#define SAME_LIM    0
#define DIFF_LIM    1

// Clamp for R_CCDlyTm
#define CC_DELAY_LIMIT      2.0F     // 2.0 seconds


// Externals
//----------
extern float DelTm2;
extern float DelTm3;
extern float R_DelTm1;
extern float R_TrqRat;
extern float S_VoltsRcp;
extern float DelTm2Rcp;

extern float R_VbrgUpGn;
extern float R_VbrgDnGn;
extern float LclTrqRefx;
extern float PhiRefRem;
extern float TrqRefRem;
extern float VarRefRem;
extern float DTD1SldGnLcl;
extern float DTD2SldGnLcl;
extern float DTDGnTrqLmNm;
extern float R_TrqLimMtr;
extern float R_TrqLimGenS;
extern float R_ILimAdj;
extern float TurbineOvrldTrq;
extern float OpnDlyTime;
extern float EcnDlyTime;


extern unsigned VARModeCmd;
extern unsigned DTD1EnbLcl;
extern unsigned DTD2EnbLcl;
extern unsigned VarReqEgd;

extern unsigned SimAcclAct;
extern unsigned XsimDoeAct;

extern float R_TrqRefXsim;
extern float R_VARRefXsim;


//extern cBridgeRebf  R_BridgeRebf;

// Types and Classes
//------------------

// Published Variables
//--------------------
CREATE_PUBVAR(R_ZeroSpdVal, float);
CREATE_PUBVAR(R_IxLimCal      ,       float);           /* Ix limit */


// Unpublished Variables
//----------------------

// Local Prototypes (to resolve forward references)
//-------------------------------------------------
int     RotorDefaultMiscellaneous(void);
int     RotorTorqueLimitSet(void);
int     StatorRule(void);
int     SequenceRule(void);
int     SpeedRule(void);



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Select set of rules to run
//
// EXECUTION LEVEL
//      Rule execution
//
// RETURN VALUE
//      Status
//
///////////////////////////////////////////////////////////////////////////////
int RunRotorAILRules(int Rule_Set)
{
    int     Status=SUCCESS;

    switch(Rule_Set)
    {
        // calculate motor control final values
    case RULE_SET_MAIN:
        Status = StatorRule()   && Status;
        Status = SequenceRule() && Status;
        Status = SpeedRule()    && Status;
        break;

        // set motor control parameters based on final values
    case RULE_SET_DEFAULT_ALL:
        if ( ((Status = RotorTorqueLimitSet()) == SUCCESS))
            Status = RotorDefaultMiscellaneous();
        break;

        // Set Default Miscellaneous Settings
    case RULE_SET_DEFAULT_MISC:
        Status = RotorDefaultMiscellaneous();
        break;

        // Set torque limits
    case RULE_SET_TORQUE_LIMIT_SET:
        Status = RotorTorqueLimitSet();
        break;

        // Default case
    default:
        break;
    }


    return (Status);
}   // RunRotorRuleSet()



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rules for sequencer parameters
//
// EXECUTION LEVEL
//      Rule execution
//
// RETURN VALUE
//      Status
//
//////////////////////////////////////////////////////////////////////////////
int SequenceRule(void)
{
    SET_FV(RtrDualBrg, 0);

    if ( PARM(TestMode) == PUMP_BACK )
    {
        if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
        {
            SET_FV(R_AutoCTEnb, 0);      // needs eval for pumpback
        }
        else
        {
            SET_FV(R_AutoCTEnb, 1);      // needs eval for pumpback
        }
        SET_FV(R_CCChkCTEnb, 0);     // disable auto cell test after crowbar contactor pickup for pumpback
    }
    else
    {
        SET_FV(R_AutoCTEnb, 1);      // enable auto cell test during start  -  NEED cell test functional!
        SET_FV(R_CCChkCTEnb, 1);     // enable auto cell test after crowbar contactor pickup  -  NEED cell test functional!
    }




    if(SimAcclAct)
    {
        SET_FV(R_TrqEnDlyTm, 0.0F);  // Disable rotor torque enable delay in acceleration
    }
    else
    {
        SET_FV(R_TrqEnDlyTm, 1.0F);  // Delay between contactor close & enabling torque
    }
    SET_FV(R_TrqDlyZVTm, 0.05F); // Delay between flux enable  & enabling torque on ZVRT recovery

    // Crowbar contactor settings

    if (SimAcclAct)
    {
        SET_FV(R_CTChkTm,   0.05F); // Delay time during which CT's are checked
        SET_FV(R_CCEcnTm,   0.0F);  // Crowbar contactor economizer output delay
        SET_FV(R_CCDlyTm,   0.0F);  // Crowbar contactor pickup time
    }
    else
    {
        SET_FV(R_CTChkTm,   1.0F);  // Delay time during which CT's are checked
        SET_FV(R_CCEcnTm,   1.0F);  // Crowbar contactor economizer output delay
        SET_FV(R_CCDlyTm,   0.3F);  // Crowbar contactor pickup time
    }
    SET_FV(R_CCFbkEnb,     false); // Enable the Rotor 1 crowbar contactor status
    SET_FV(R_VbrgUpW,  100.0F);    // Up/Down filter omegas and gains
    SET_FV(R_VbrgDnW,   10.0F);


    // Clamp ROVC pick-up/dropout delay time if set to large value
    if ( PARM(R_CCDlyTm) > CC_DELAY_LIMIT )
    {
        OpnDlyTime = CC_DELAY_LIMIT;
    }
    else
    {
        OpnDlyTime = PARM(R_CCDlyTm);
    }



    // Clamp economizing contactor pick-up/dropout delay time if it is set
    // too large or too low
    if ( PARM(R_CCEcnTm) > CC_DELAY_LIMIT )
    {
        EcnDlyTime = CC_DELAY_LIMIT;
    }
    else if ( PARM(R_CCEcnTm) < OpnDlyTime )
    {
        EcnDlyTime = OpnDlyTime;
    }
    else
    {
        EcnDlyTime = PARM(R_CCEcnTm);
    }

    R_VbrgUpGn = DelTm2 * PARM(R_VbrgUpW);
    R_VbrgDnGn = DelTm2 * PARM(R_VbrgDnW);


    if ( PARM(TestMode) == PUMP_BACK )
    {
        if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
        {
            // Crowbar contactor settings
            SET_FV(R_CCDisDlyTm,   1.0F);  // Delay between shutdown & drop out crowbar
        }
        else
        {
            SET_FV(R_CCDisDlyTm,   0.05F);
        }
    }
    else
    {
        // Crowbar contactor settings
        SET_FV(R_CCDisDlyTm,   0.05F); // Delay between shutdown & drop out crowbar
    }
    if ( PARM(Net_Type) == NET_ABSENT)
    {
        if (PARM(TcSimEnb))
        {
            SET_FV(R_TrqRefAdr, (float * ) LADR_TcTrqCmdZ1);
            SET_FV(VARRefAdr, (float * )   LADR_TcVARCmdZ1);
        }
        else if(XsimDoeAct)
        {
            SET_FV(R_TrqRefAdr, (float * ) LADR_R_TrqRefXsim);
            SET_FV(VARRefAdr, (float * )   LADR_R_VARRefXsim);  //  TcVarCmd will point to the local keypad value in this case
        }
        else
        {
            SET_FV(R_TrqRefAdr, (float * ) LADR_LclTrqRefx);
            SET_FV(VARRefAdr, (float * )   LADR_TcVARCmd);  //  TcVarCmd will point to the local keypad value in this case
        }

    }
    else   // IPC interface
    {
        SET_FV(R_TrqRefAdr, (float * ) LADR_TrqRefRem);
        SET_FV(VARRefAdr, (float * )   LADR_VarRefRem);
    }






    SET_FV(VARPfModSel, VARMODE);
    VARModeCmd = PARM(VARPfModSel);


    /* leave as non-FV parm for now */

// if ( PARM(IPN_Frq) == 50 )
//   SET_FV(LvIyRefEnb, 1);       // Enable EON voltage support Iy
// else
//   SET_FV(LvIyRefEnb, 0);

    SET_FV(R_RevRotFlt, 1);      // enable
    SET_FV(R_RunPrmAdr, (uint * ) LADR_UnusedBool);
    SET_FV(R_StrtPrmAdr, (uint * ) LADR_UnusedBool);


    SET_FV(R_TrefEnbAdr, (uint * ) LADR_UnusedBool);

    SET_FV(R_TrqDTDolMrg, 0.01);




    if ( PARM(TestMode) == PUMP_BACK )
    {
        SET_FV(R_TrqLimMtr1, 3.50F);
        SET_FV(R_TrqLimMtr2, 4.10F);
        SET_FV(R_TrqLimGen1, 3.50F);
        SET_FV(R_TrqLimGen2, 4.10F);
        SET_FV(R_TrqRateLim, 20000.0F);
    }
    else if (PARM(SoftwareIPN) < DFIG02_2400_5060_A)
    {
        SET_FV(R_TrqLimMtr1, 0.05F);
        SET_FV(R_TrqLimMtr2, 0.05F);
        SET_FV(R_TrqLimGen1, 1.23F);
        SET_FV(R_TrqLimGen2, 1.23F);
        SET_FV(R_TrqRateLim, 9.0F*R_TrqRat);  // 0 means no rate limiting
    }
    else
    {
        SET_FV(R_TrqLimMtr1, 0.05F);
        SET_FV(R_TrqLimMtr2, 0.05F);
        if ( (PARM( L_FrqInput > 58.0F ) ) )
        {
           SET_FV(R_TrqLimGen1, 1.29F);
           SET_FV(R_TrqLimGen2, 1.29F);
        }
        else
        {
           SET_FV(R_TrqLimGen1, 1.32F);
           SET_FV(R_TrqLimGen2, 1.32F);
        }
        SET_FV(R_TrqRateLim, 9.0F*R_TrqRat);  // 0 means no rate limiting
    }


    R_TrqLimMtr =  PARM(R_TrqLimMtr1) * R_TrqRat;
    R_TrqLimGenS = PARM(R_TrqLimGen1) * R_TrqRat;

    SET_FV(R_TrqLmTtScl, 1.0F);  // scale factor between R_TrqLimGenS & R_TrqLmTtNeg

    R_ILimAdj    = PARM(R_ILimAdj1);
    if (R_ILimAdj > PARM(R_ILimMax))
    {
        R_IxLimCal = PARM(R_ILimMax);
    }
    else
    {
        R_IxLimCal = R_ILimAdj;
    }

    SET_FV(R_XStpCfg, 0);       // 0 means normal stop

    SET_FV(R_ZeroSpdDly, 0.0F);  // sec


    // Drive train damper
    // ------------------------         //Bachmann to ESS conversion
    SET_FV(DTD1KLcl,   8.40e-3);        //80.0  --> 2O.0 * PI/30           -- From P16.9
    SET_FV(DTD1FLcl,       1.6);        //1.6                              -- From P16.10
    SET_FV(DTD1ZLcl,     0.200);        //20.0  --> 1.0/100                -- From P16.11
    SET_FV(DTD1TLcl,     0.500);        //500.0 --> 1.0/1000               -- From P16.12
    SET_FV(DTD1SldGnLcl,   1.0);
    SET_FV(DTD1EnbLcl,   false);

    //Bachmann to ESS conversion
    SET_FV(DTD2KLcl,   1.26e-2);        //30.0  --> 3O.0/4  * PI/30/1,000  -- From P16.16
    SET_FV(DTD2FLcl,    2.8);           //2.8                              -- From P16.17
    SET_FV(DTD2ZLcl,    0.50);          //50.0  --> 1.0/100                -- From P16.18
    SET_FV(DTD2TLcl,    0.900);         //500.0 --> 1.0/1000               -- From P16.19
    SET_FV(DTD2SldGnLcl,0.0);
    SET_FV(DTD2EnbLcl, false);

    SET_FV(DTDStpAdr,(float *) LADR_FZero);
    SET_FV(DTDGenTrqLim, 0.20);
    SET_FV(DTDMtrTrqLim, -0.1);

    DTD1EnbLcl   = PARM(DTD1EnbLcl);
    DTD1SldGnLcl = PARM(DTD1SldGnLcl);
    DTD2EnbLcl   = PARM(DTD2EnbLcl);
    DTD2SldGnLcl = PARM(DTD2SldGnLcl);


    if ( PARM(Net_Type) == NET_ABSENT)
    {
        SET_FV(DTD1EnbAdr,  (unsigned * ) LADR_DTD1EnbLcl);
        SET_FV(DTD2EnbAdr,  (unsigned * ) LADR_DTD2EnbLcl);
        SET_FV(DTD1SldGnAdr,(float * ) LADR_DTD1SldGnLcl);
        SET_FV(DTD2SldGnAdr,(float * ) LADR_DTD2SldGnLcl);
    }
    else   // MarkIVe IOnet
    {
        SET_FV(DTD1EnbAdr,  (unsigned * ) LADR_DTD1EnbRem);
        SET_FV(DTD2EnbAdr,  (unsigned * ) LADR_DTD2EnbRem);
        SET_FV(DTD1SldGnAdr,(float * ) LADR_DTD1SldGnRem);
        SET_FV(DTD2SldGnAdr,(float * ) LADR_DTD2SldGnRem);
    }

    DTDGnTrqLmNm     = PARM(DTDGenTrqLim)*R_TrqRat;

    SET_FV(DelSpdTrqAct,true);

    SET_FV(R_SeqQ1TmDly,     0.010F);

    return (SUCCESS);

}   // SequenceRule()



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rules for parameters related to stator circuit
//
// EXECUTION LEVEL
//      Rule execution
//
// RETURN VALUE
//      Status
//
//////////////////////////////////////////////////////////////////////////////
int StatorRule(void)
{


    SET_FV(S_VabOffs, 0.0F);
    SET_FV(S_VbcOffs, 0.0F);

    SET_FV(S_IOffsPct, 0.8F);              // Offset limit = 0.8% full scale current

    SET_FV(S_IaOffs, 0.0F);
    SET_FV(S_IbOffs, 0.0F);
    SET_FV(S_IcOffs, 0.0F);

    SET_FV(S_IaVcoScl, 4.19216F/1.0e6F); //4.19216v/1,000,000hz   //see AEAA#BF elementary, sheet 9
    SET_FV(S_IbVcoScl, 4.19216F/1.0e6F); //4.19216v/1,000,000hz   //see AEAA#BF elementary, sheet 9
    SET_FV(S_IcVcoScl, 4.19216F/1.0e6F); //4.19216v/1,000,000hz   //see AEAA#BF elementary, sheet 9

    if ( PARM(SoftwareIPN) == DFIG02_1850_5060_E)
    {
        SET_FV(S_CTburdenR,0.1F);  //0.1 ohms
        SET_FV(S_CTratio, 1500.0F/1.0F);
    }
    else
    {
       if ( PARM(SoftwareIPN) <  DFIG02_2400_5060_A)
       {

           SET_FV(S_CTburdenR,0.5F);  //0.5 ohms
           SET_FV(S_CTratio, 1500.0F/1.0F); // 500:1 6KV CT ratio
       }
       else
       {
           SET_FV(S_CTburdenR,0.1F);  //0.1 ohms
           SET_FV(S_CTratio, 2500.0F/1.0F); // 2500:1 6KV CT ratio
       }
    }




    SET_FV(S_VOffsVlTm, 3600.0F);

    SET_FV(S_WIOffsFst, 5.0F);
    SET_FV(S_WIOffsSlw, 0.5F);

    SET_FV(S_WIabcGnd, 1000.0F);


    S_VoltsRcp = NoNANLim( 1.0F/(PARM(S_VxfmrSec)*SQRT2*SQRT3_RCP), 1.0e6F, -1.0e6F );


    ////////////////////////////////////////////
    // End Volt-Var Rules -- keep segregated
    ////////////////////////////////////////////


    ////////////////////////////////
    // IRAM copy population
    ////////////////////////////////

    // aux bus volt-var regs

    return (SUCCESS);
}   // StatorRule()



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rules for parameters associated with speed calculation
//
// EXECUTION LEVEL
//      Rule execution
//
// RETURN VALUE
//      Status
//
//////////////////////////////////////////////////////////////////////////////
int SpeedRule(void)
{

    float  Temp1, Temp2;

    if ( PARM(TestMode) == PUMP_BACK )
    {
        SET_FV(R_WSpdT2, 500.0F);
    }
    else
    {
        SET_FV(R_WSpdT2, 10.0F);
    }

    Temp1 = 0.025 * PARM(R_RPMSync);  /* Calculate 2.5% of Motor RPM */

    //                                rev            1          60 sec
    // R_ZeroSpdVal(RPM) = #quanta * ------ * --------------- * ------ * filter BW (100 rad/s)
    //                               quanta   time for which     min
    //                                        spdfbk is accum

    R_ZeroSpdVal =  2.0F * (1.0F/(4.0F * PARM(R_TachPPR))) * DelTm2Rcp * 60.0F * (R_DelTm1 * PARM(R_WSpdT2));

    // Clamp to 0.5% if R_ZeroSpdVal is less than that.
    if (R_ZeroSpdVal < Temp1)
    {
        R_ZeroSpdVal = Temp1;
    }

    SET_FV(R_ZeroSpdThr, R_ZeroSpdVal);


    if ( PARM(S_Hz) > 55.0F) //(60hz)
    {
        Temp2 =  1.07875F;
    }
    else//(50hz)
    {
        Temp2 =  1.0665F;
    }


    SET_FV(R_OvrSpdThr, Temp2*PARM(R_RPMMaxOp));  //1600-> 1726,   2000 -> 2133

// Temp1 = 0.05*PARM(S_RRatio) * PARM(R_RPMMaxOp);       ESS rule
    Temp1 = 25.0F;                                //   S_RRatio is 10 X bigger than ESS
    SET_FV(R_UndrSpdThr, PARM(R_RPMMinOp) - Temp1);




    return (SUCCESS);

}   // SpeedRule()



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rules for miscellaneous parameters
//
// EXECUTION LEVEL
//      Rule execution
//
// RETURN VALUE
//      Status
//
//////////////////////////////////////////////////////////////////////////////
int RotorDefaultMiscellaneous(void)
{

    return (SUCCESS);
}   // RotorDefaultMiscellaneous()



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rules for parameters associated with torque limits
//
// EXECUTION LEVEL
//      Rule execution
//
// RETURN VALUE
//      Status
//
//////////////////////////////////////////////////////////////////////////////
int RotorTorqueLimitSet(void)
{
    // moved rules, parms changed to final values
    return (0);
}
