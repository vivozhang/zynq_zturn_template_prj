///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Source control Rules
//
// DESCRIPTION:
//      This file contains rule calculations for source control
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
#include "x0FrameWork.h"
// product
#include "Product.h"


// Constants
//----------
#define VDC_FIXED       0
#define VDC_FLOATING    1

#define L_TEST_BOTH     0
#define L_TEST_LINE     1

#define SQRT_1_2        1.095445115F   //Sqrt(1.2)
#define SQRT_5DIV52     0.308866F      //Sqrt(5/52.41201) -- gain reduction for PllPrpGnI


// Externals
//----------
extern float     DelTm2           ;
extern float     DelTm2Rcp        ;
extern float     GmFrqNomRcp      ;
extern float     GmVllNomRcp      ;
extern float     GmVlnNomRcp      ;
extern float     L_CnvPwrGn       ;
extern float     L_DcPwrGn        ;
extern float     L_DelTm1         ;
extern float     L_DelTm1Rcp      ;
extern float     L_IdcFilGn       ;
extern float     L_IMagFilGn      ;
extern float     L_IregEr2FGn     ;
extern float     L_IrgSpilGnx     ;
extern float     L_IxCmdZ1Sc      ;
extern float     L_IxCmdZ2Sc      ;
extern float     L_IxFilGn        ;
extern float     L_IxLimNx        ;
extern float     L_IxLimPx        ;
extern float     L_IxrgCmdGnx     ;
extern float     L_IxrgFbkGnx     ;
extern float     L_IxrgIntGnx     ;
extern float     L_IxrgIntLmx     ;
extern float     L_IxrgULimx      ;
extern float     L_IyCmdA0Sc      ;
extern float     L_IyCmdA1Sc      ;
extern float     L_IyCmdD0Sc      ;
extern float     L_IyCmdD1Sc      ;
extern float     L_IyCmdLimx      ;
extern float     L_IyCmdZ1Sc      ;
extern float     L_IyCmdZ2Sc      ;
extern float     L_IyFilGn        ;
extern float     L_IyRatSqr       ;
extern float     L_IyrfIntGnx     ;
extern float     L_IyrgCmdGnx     ;
extern float     L_IyrgFbkGnx     ;
extern float     L_IyrgIntGnx     ;
extern float     L_IyrgIntLmx     ;
extern float     L_IyrgULimx      ;
extern float     L_LineFrqGn      ;
extern float     L_LineFrqRcp     ;
extern float     L_LineMagGn      ;
extern float     L_LTotCal        ;
extern float     L_LTotDt         ;
extern float     L_PwrLineGn      ;
extern float     L_VarFilGn       ;
extern float     L_VarLineGn      ;
extern float     L_VdcFilGn       ;
extern float     L_VdcIxIyFlGn    ;
extern float     L_VdcMdRfRcp     ;
extern float     L_VdcNomCal      ;
extern float     L_VdcNomRcp      ;
extern float     L_VdcRefx        ;
extern float     L_VdcSlwx        ;
extern float     L_VlnInRcp       ;
extern float     L_VMagT2Gn       ;
extern float     L_VregGnRcp      ;
extern float     L_VregIntGnx     ;
extern float     L_VregPrpGnx     ;
extern float     L_VregWoComp     ;
extern float     L_VregWoGnx      ;
extern float     L_VxFilGn        ;
extern float     L_VyFilGn        ;
extern float     L_IyHrmFFlGn     ;
extern float     GmFrqNomWRcp     ;
extern unsigned SimAcclAct;
// For SeqDemodulateLine()
extern float     L_Q1DeltVoltGn;
// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(L_IyHrmFFilW,     float);   // Filter W for filtered reactive current in distortion filter


CREATE_PARM(DBVdcThrs,        float);   // DB Vdc threshold


// Variables
//----------
CREATE_PUBVAR(L_IregResMax,   float);
CREATE_PUBVAR(L_LXfmr,        float);
CREATE_PUBVAR(L_PllWNom,      float);
CREATE_PUBVAR(L_RBldCapRcp,   float);
CREATE_PUBVAR(L_RFeed,        float);
CREATE_PUBVAR(L_VlnIn,        float);  // Line to neutral input voltage magnitude
CREATE_PUBVAR(LBrgVMtchDt1,   float);

// Current Regulator final value copies
CREATE_PUBVAR(L_IrgSpilGnx,   float);      // Ram copy of EE.L_IregSpilGn
CREATE_PUBVAR(L_IxrgCmdGnx,   float);      // Ram copy of EE.L_IxregCmdGn
CREATE_PUBVAR(L_IxrgFbkGnx,   float);      // Ram copy of EE.L_IxregFbkGn
CREATE_PUBVAR(L_IxrgIntGnx,   float);      // Ram copy of EE.L_IxregIntGn
CREATE_PUBVAR(L_IxrgIntLmx,   float);      // Ram copy of EE.L_IxregIntLm
CREATE_PUBVAR(L_IxrgULimx,    float);      // Ram copy of EE.L_IxregULim
CREATE_PUBVAR(L_IyCmdLimx,    float);      // Ram copy of EE.L_IyCmdLim
CREATE_PUBVAR(L_IyrgCmdGnx,   float);      // Ram copy of EE.L_IyregCmdGn
CREATE_PUBVAR(L_IyrgFbkGnx,   float);      // Ram copy of EE.L_IyregFbkGn
CREATE_PUBVAR(L_IyrgIntGnx,   float);      // Ram copy of EE.L_IyregIntGn
CREATE_PUBVAR(L_IyrgIntLmx,   float);      // Ram copy of EE.L_IyregIntLm
CREATE_PUBVAR(L_IyrgULimx,    float);      // Ram copy of EE.L_IyregULim

// Volt Regulator final value copies
CREATE_PUBVAR(L_IxLimNx,      float);      // Ram copy of EE.L_IxLimNx
CREATE_PUBVAR(L_IxLimPx,      float);      // Ram copy of EE.L_IxLimPx
CREATE_PUBVAR(L_VdcRefx,      float);      // Ram copy of EE.L_VdcRef
CREATE_PUBVAR(L_VdcSlwx,      float);      // Ram copy of EE.L_VdcSlw
CREATE_PUBVAR(L_VregIntGnx,   float);      // Ram copy of EE.L_VregIntGnx
CREATE_PUBVAR(L_VregPrpGnx,   float);      // Ram copy of EE.L_VregPrpGnx
CREATE_PUBVAR(L_VregWoGnx,    float);      // Ram copy of EE.L_VregWoGnx
CREATE_PUBVAR(L_VregWoComp,   float);      // voltage regulator Wo compensation

// Unpublished Variables
//----------------------


// Data Passing
//--------------


// Diagnostics
//------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------
int     LineInResRule(void);
int     LineSrcCtlRule(void);


// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Select the source-control-layer ruleset to run.
//
// EXECUTION LEVEL
//      Initialization or ADL Thread
//
// RETURN VALUE
//      Status to indicate if calculations were successful (SUCCESS/FAIL).
//
///////////////////////////////////////////////////////////////////////////////
int RunLineLCLRules(int Rule_Set)
{
    int     Status=SUCCESS;

    switch(Rule_Set)
    {
        // calculate motor control parameters
        case RULE_SET_MAIN:
            if ((Status = LineInResRule()) == SUCCESS)
                Status = LineSrcCtlRule();
            break;

        // Run again because dependent on main rule set.
        case (RULE_SET_MAIN + RULE_SET_SECOND_PASS):
                Status = LineSrcCtlRule();
            break;

        // set Source control parameters based on final values
        case RULE_SET_DEFAULT_ALL:
            break;


        // Set Default Line Protection
        case RULE_SET_DEFAULT_LINE_PROT:
            break;


        // Set Default Per Unit Parameters
        case RULE_SET_DEFAULT_PER_UNIT:
            break;


    }
    return (Status);
}   // RunLineLCLRules()


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Resolution of proper source-parameter-set for rule calculaion.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
int LineInResRule(void)
{
    return (SUCCESS);
}   // InResRule()


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rules for calculating source control parameters from tuneup results.
//      The source must be stopped or the calculations are not performed.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      Status to indicate if calculations were successful (SUCCESS/FAIL).
//
///////////////////////////////////////////////////////////////////////////////
int LineSrcCtlRule(void)
{

    float Ireg_BW;
    float Ireg_Wn;
    float Spill_Gn_Temp;
    float Vll_Temp;
    float L_VdcRef_Temp;
    float W_Nom;
    float VdcNomCal;

    // Fundamental final values that must be calculated early
    L_PllWNom   = PARM(L_LineFrq);
    GmFrqNomRcp = NoNANLim(1.0F/PARM(L_FrqInput), 1.0e6F, -1.0e6F);
    GmFrqNomWRcp = GmFrqNomRcp * RAD_TO_HZ;  // same as Rcp = 1.0/(L_Frq_Input * HZ_TO_RAD)

    L_VlnIn     = PARM(L_VllIn)*SQRT2*SQRT3_RCP;

    GmVllNomRcp = NoNANLim(1.0F/PARM(L_VllIn), 1.0e6F, -1.0e6F);
    GmVlnNomRcp = GmVllNomRcp*SQRT3;


    Vll_Temp = PARM(L_VllOut);      // Clamp L_VllRms between PARM(L_VllIn
    if ( Vll_Temp >= PARM(L_VllMax) )// and L_VllMax
    {
         Vll_Temp = PARM(L_VllMax);
    }
    else if (Vll_Temp <= PARM(L_VllIn) )
    {
         Vll_Temp = PARM(L_VllIn);
    }
    SET_FV(L_VllRms, Vll_Temp); //Careful! -- This is used later in calc of L_IregSpilGn

    if((PARM(SoftwareIPN) >= DFIG02_2400_5060_A))
    {
        SET_FV(L_CBrg,      0.03F);
        SET_FV(L_RChg,      50.0F);
        SET_FV(L_RBleedCap, 9900.0F);   //15 resistors.  5 parallel circuits, each with 3, 16.5K resistors in series
    }
    else
    {
        SET_FV(L_CBrg,      0.024F);
        SET_FV(L_RChg,      50.0F);
        SET_FV(L_RBleedCap, 12375.0F);  //12 resistors.  4 parallel circuits, each with 3, 16.5K resistors in series
    }

    L_RBldCapRcp = NoNANLim( 1.0F/PARM(L_RBleedCap), 1.0e6F, 0.0F );
    SET_FV(L_CTot, PARM(L_CBrg));


    if ( PARM(L_LineFrq) ==0 || PARM(L_KVABasXfmr) ==0)
    {
        L_LXfmr = 0.0F;
    }
    else
    {
        L_LXfmr = PARM(L_LxfmrSec);
    }

    SET_FV(L_LTot, PARM(CnvLineL));






    if ( PARM(L_KVABasXfmr) ==0.0F)
    {
        L_RFeed = 0.0F;
    }
    else
    {
        L_RFeed = PARM(L_RxfmrSec);
    }
    SET_FV(L_RTot, PARM(CnvLineR));
    SET_FV(LBrgVMtch, PARM(CnvLineL));
    LBrgVMtchDt1 = PARM(LBrgVMtch)*L_DelTm1Rcp;


    // Line reconstruction

    // Generator Settings
    SET_FV(L_LTotSteady, PARM(L_LTot));


    // Final values set to constants

    SET_FV(L_IxMeldReq, 0.5F);
    SET_FV(L_IyMeldReq, 0.5F);

    SET_FV(L_ChgDlyTm, 5.0F*(2.0F*PARM(L_RChg))*PARM(L_CTot));

    // Current Regulator Gains, ect:
    SET_FV(L_IregBwBase, 1500.0F);
    Ireg_BW = PARM(L_IregBwBase)*166.66667e-6F*L_DelTm1Rcp;

    // 0 = "Original"
    // 1 = "Ireg LP filter:
    // 2 = "Feedback LP filter;
    // 3 = "Both"
    if ( PARM(L_IregFbkTyp) == 0) //original
    {
        Ireg_Wn = 0.5*Ireg_BW;
        SET_FV(L_IxregCmdGn, Ireg_BW*PARM(L_LTot));
        SET_FV(L_IxregIntGn, Ireg_Wn*Ireg_Wn*PARM(L_LTot));
        if ( PARM(TestMode) != PUMP_BACK )
        {
            SET_FV(L_IregPGn2,  2.06425F*PARM(L_IxregFbkGn)); //give gn2~1.9 when Gn = .106576 //Not used in this mode.
        }
        else
        {
            SET_FV(L_IregPGn2,  0.0F);
        }
    }
    else if ( PARM(L_IregFbkTyp) == 1) //Ireg LP filter only
    {
        Ireg_Wn = 0.5*0.62635*Ireg_BW; //was 0.5*Ireg_Bw prior to 12/21/07...changed to give int gain of 30 with L = 150uh
        SET_FV(L_IxregCmdGn, Ireg_BW*PARM(L_LTot));
        SET_FV(L_IxregIntGn, Ireg_Wn*Ireg_Wn*PARM(L_LTot));
        if ( PARM(TestMode) != PUMP_BACK )
        {
            SET_FV(L_IregPGn2,  2.06425F*PARM(L_IxregFbkGn)); //give gn2=.442162 when Gn = .2142
        }
        else
        {
            SET_FV(L_IregPGn2,  0.0F);
        }
    }
    else if ( PARM(L_IregFbkTyp) == 2) //Feedback LP filter only
    {
        Ireg_Wn = 0.5F*Ireg_BW; //was 0.5*Ireg_Bw prior to 12/21/07...back to original value
        SET_FV(L_IxregCmdGn, 1.867413632F * Ireg_BW*PARM(L_LTot)); //should give 0.40
        SET_FV(L_IxregIntGn, Ireg_Wn*Ireg_Wn*PARM(L_LTot));
        if ( PARM(TestMode) != PUMP_BACK )
        {
            SET_FV(L_IregPGn2,  1.105405F*PARM(L_IxregFbkGn)); //give gn2~.2142 when gain = 0.4, even though not used
        }
        else
        {
            SET_FV(L_IregPGn2,  0.0F);
        }
    }
    else if ( PARM(L_IregFbkTyp) == 3) //Ireg LP filter AND Feedback LP filter
    {
        Ireg_Wn = 0.5F*0.808613822F*Ireg_BW; //was 0.5*Ireg_Bw prior to 12/21/07...changed to give int gain of 30 with L = 150uh
        SET_FV(L_IxregCmdGn, Ireg_BW*PARM(L_LTot));
        SET_FV(L_IxregIntGn, Ireg_Wn*Ireg_Wn*PARM(L_LTot));
        if ( PARM(TestMode) != PUMP_BACK )
        {
            SET_FV(L_IregPGn2,  3.267973856F*PARM(L_IxregFbkGn)); //give gn2~1.9 when Gn = 0.70, even though not used
        }
        else
        {
            SET_FV(L_IregPGn2,  0.0F);
        }
    }

    SET_FV(L_IxregFbkGn, PARM(L_IxregCmdGn));
    SET_FV(L_IyregCmdGn, PARM(L_IxregCmdGn));
    SET_FV(L_IyregFbkGn, PARM(L_IxregCmdGn));

    SET_FV(L_IyregIntGn, PARM(L_IxregIntGn));

    SET_FV(L_IregErr2W, 2.0F*PI*50.0F);
    L_IregEr2FGn = L_DelTm1*PARM(L_IregErr2W);


    // Ireg Integrator limit
    // Set limit so that it is just able to push L_IyCmd0 into L_IyCmdLim
    //
    // Ixreg_Int_Lim =  .1*L_VllIn*sqrtf(2/3))        +  (L_IyCmdLim*sqrt(2)/L_IregSpilGn)*L_Vdc*1/sqrt(3)
    //                  L_Vx to push into spillover   +  L_Vx to force L_IyCmd to limit
    //
    // L_IyCmdLim = L_IRat * sqrtf(2)
    //
    // L_Vdc = 1.1*L_VllIn*sqrtf(2/3) = L_VdcRef
    //
    // Ixreg_Int_Lim =  .2*L_VllIn*sqrtf(2/3)         + (L_IRat*sqrt(2)/L_IregSpilGn)*1.1*L_VllIn*sqrt(2)
    //                                                * 1/(sqrt(3))
    //



    if ( PARM(TestMode) == PUMP_BACK )
    {
       SET_FV(L_VdcRefMode, VDC_FIXED);
       SET_FV(L_VdcRef,     PARM(L_VdcRefMax));
    }
    else
    {
       SET_FV(L_VdcRefMode, VDC_FLOATING);
       SET_FV(L_VdcRef, PARM(BrgVdcNom));
    }


    SET_FV(L_VdcRefLvrt, 0.90F*PARM(L_VdcRef));
    SET_FV(L_VdcVmgLvrt, L_VlnIn*0.5F);
    SET_FV(L_VdcVMgNThr, 0.20F);

    SET_FV(L_DCInitThr, PARM(L_VdcRef)*0.02F);
    if(SimAcclAct)
    {
        SET_FV(L_DCInitTm, 0.00F);
    }
    else
    {
        SET_FV(L_DCInitTm, 0.50F);
    }
    SET_FV(L_DCInitZVTm, 0.005F); //essentially anything over 0 sec since timer is task 3

    //Settings for adapative voltage regulator reference scheme

    SET_FV(L_VrefBwUp,  1.0F);
    SET_FV(L_VrefBwDn,  0.3F);
    SET_FV(L_VrefLmHyst,  25.0F);
    SET_FV(L_VdcModRef, 0.98F);
    L_VdcMdRfRcp = NoNANLim(1.0F/PARM(L_VdcModRef), 1.0e6F, -1.0e6F);

    SET_FV(R_VdcModRef, 0.97F);
    SET_FV(R_VdcModGn,  0.30F);
    SET_FV(R_VdcMdUpThr,0.0F);
    SET_FV(L_VdcRefWUp,PARM(L_VrefBwUp)*PARM(L_VllRms)*SQRT2);
    SET_FV(L_VdcRefWDn,PARM(L_VrefBwDn)*PARM(L_VllRms)*SQRT2);
    SET_FV(L_VdcRefMin,0.9941F*PARM(L_VllRms)*SQRT2);  // 970 at 690v input
    SET_FV(L_VdcVmagDnRt, -5000.0F);
    SET_FV(L_VdcVmagUpRt,  5000.0F);

    SET_FV(L_UxCmdPrec, -1.0F);

    Spill_Gn_Temp = NoNANLim(0.25F*((PARM(L_VdcRef))/SQRT3)/(PARM(L_LTot)*PARM(L_LineFrq)), 1.0e6F, -1.0e6F);


    SET_FV(L_IxregIntLm, 1.1F*L_VlnIn);


    SET_FV(L_IyregIntLm, PARM(L_IxregIntLm));


    SET_FV(L_UxLim, 1.0F);          // Ux limit where L_Iy reduction begins
    SET_FV(L_UxLimGn, 1.0e5F);      // Ux limiter gain - seems like a big number
                                    // but effective gain is L_IregSpilGn / L_VlnIn
    SET_FV(L_UxLimMin, 0.0F);       // Output clamp on Ux limiter


    SET_FV(L_IxregULim,  1.1F);

    SET_FV(L_IyregULim,  0.5F);

    //Old rules
    //SET_FV(L_ResBwBase, 2.0);
    //SET_FV(L_IregResBW, PARM(L_ResBwBase*(200.0e-6*200.0e-6)/(L_DelTm1*L_DelTm1));
    //SET_FV(L_IregResWI, PARM(L_IregResBW*SQRT3/(1.1*SQRT2*PARM(L_VllRms*PARM(L_LineFrq*PARM(L_LTot));
    /* New settings for negative sequence current regulator */
    SET_FV(L_ResBwBase, 50.0F);  //was 96.0 prior to 12/21/07
    SET_FV(L_IregResBW, PARM(L_ResBwBase));


    SET_FV(L_Res2ndD,  0.2F);     //Nearly the same as old rule calculated value for 60hz (was 0.20178)

    // 0 = "Original"
    // 1 = "Ireg LP filter:
    // 2 = "Feedback LP filter;
    // 3 = "Both"
    if ( PARM(L_IregFbkTyp) < 2) //original
    {
       SET_FV(L_Res2ndQ,  0.03F);     //Was -0.1
    }
    else
    {
       SET_FV(L_Res2ndQ, -0.08F);
    }

    SET_FV(L_IregResWI, NoNANLim(PARM(L_IregResBW)*SQRT3/PARM(L_VdcRef), 1.0e6F, -1.0e6F));

    L_IregResMax      = NoNANLim(PARM(L_VllIn)/PARM(L_VllRms)*0.150F, 1.0e6F, -1.0e6F);
    SET_FV(L_IregResLim, NoNANLim(L_IregResMax/
         (PARM(L_IregResWI)*sqrtf(PARM(L_Res2ndD)*PARM(L_Res2ndD) + PARM(L_Res2ndQ)*PARM(L_Res2ndQ))), 1.0e6F, -1.0e6F));

    SET_FV(L_RsLmZeroOn,0.50F);
    SET_FV(L_RsLmModOn, 0.65F);
    SET_FV(L_RsLmModOff,0.60F);


    if (PARM(L_IyrfIMax) > PARM(L_MaxCur))
    {
       if ( PARM(L_IyrfIMax)*SQRT2 < 2.0F*PARM(L_ShuntRat))
       {
          SET_FV(L_IyCmdLim,   PARM(L_IyrfIMax)*SQRT2);
       }
       else
       {
          SET_FV(L_IyCmdLim,   2.0F*PARM(L_ShuntRat));
       }
    }
    else
    {
       SET_FV(L_IyCmdLim,   PARM(L_MaxCur)*SQRT2);
    }

    SET_FV(L_IyCmdLmHi, 2.5F*PARM(L_ShuntRat)); // for islanding and initial part of HVRT

    if ( PARM(TestMode) == PUMP_BACK )
    {
        switch(PARM(SoftwareIPN))
        {
            case DFIG02_2400_5060_A:
                SET_FV(L_IxLimP,    3000.0F);
                SET_FV(L_IxLimN,   -3000.0F);
                break;
            default:
                SET_FV(L_IxLimP,    SQRT2 * 1200.0 + 100.0 );
                SET_FV(L_IxLimN,  -(SQRT2 * 1200.0 + 100.0));
                break;
        }
    }
    else
    {
        switch(PARM(SoftwareIPN))
        {
            case DFIG02_2400_5060_A:
                SET_FV(L_IxLimP,     1850.0F);
                SET_FV(L_IxLimN,    -1850.0F);
                break;
            case DFIG02_1500_5060_A:
            case DFIG02_1500_5060_B:
                SET_FV(L_IxLimP,     PARM(L_MaxCur)*SQRT2*294.2F*0.01F); // from macc, L_IOVLoad=294.2
                SET_FV(L_IxLimN,    -PARM(L_MaxCur)*SQRT2*294.2F*0.01F);
                break;
            case DFIG02_1850_5060_C:
            case DFIG02_1850_5060_D:
            case DFIG02_1850_5060_E:
            default:
                SET_FV(L_IxLimP,     PARM(L_MaxCur)*SQRT2*237.09F*0.01F); // from macc, L_IOVLoad=237.09
                SET_FV(L_IxLimN,    -PARM(L_MaxCur)*SQRT2*237.09F*0.01F);
                break;
        }
    }

    // Voltage Reference and Regulation:
    // The voltage regulator is compensated for source's capacitance
    // alone(L_CTot).  As load capacitance is added, the damping factor
    // will approach 1 when the total capacitance(source + load) is five
    // times the source's capacitance alone.  This allows for stability
    // when the source is not loaded or when it is loaded to the max.


    SET_FV(L_VregPrpGn,  1.0F);
    SET_FV(L_VregBw, 450.0F);


    SET_FV(L_VregIntGn,  PARM(L_VregBw)*0.0776F);
    SET_FV(L_VregWoGn,   PARM(L_VregBw));

    SET_FV(L_VdcSlw,     NoNANLim(PARM(L_MaxCur)*(SQRT3/SQRT2)/(3.0F*PARM(L_CBrg)), 1.0e6F, -1.0e6F));

    SET_FV(L_VdcTranMrg, 10.0F);


    //1.1*SQRT2*Vll_Temp in next line represents L_VdcRef without fv                                 's
    L_VdcRef_Temp = 1.10F*SQRT2*Vll_Temp;
    SET_FV(L_IregSpilGn, NoNANLim(0.027374581F * ((L_VdcRef_Temp)/SQRT3)/     //0.027374581 to give gn of 300 at 60hz
                        (PARM(L_LTot)*PARM(L_LineFrq)), 1.0e6F, -1.0e6F));           //was 0.25 prior to this

    SET_FV(L_IrgSplGnHi, 10.0F * PARM(L_IregSpilGn));

    SET_FV(L_EnbDlyTm,0.0F);
    SET_FV(L_AcLossTm, 0.020F);
    SET_FV(L_AcLossOn, L_VlnIn*0.10F);
    SET_FV(L_PhImbIpTm, 0.005F);
    SET_FV(L_PhImbIpThr, 0.133F*L_VlnIn );

    //--------------------------------------------------------------------------

    SET_FV(L_LTstType, L_TEST_BOTH);

    //Offset Stuff
    if ( PARM(SimModeReq))
    {
      SET_FV(L_IOffVldTm, 1.0F); //why wait in sim mode?
    }
    else
    {
      SET_FV(L_IOffVldTm, 10.0F);
    }


    SET_FV(L_IOffDsbTm, 0.02F + PARM(Q1OpenTypTm) * 2.0F);
    SET_FV(L_IyCmdD0Gn, 0.75F);
    SET_FV(L_IyCmdD1Gn, -0.75F);

    SET_FV(L_RegLimTm,  1.3F);
    SET_FV(L_IxCmdZ1Gn, 0.5F);
    SET_FV(L_IxCmdZ2Gn, 0.5F);
    SET_FV(L_IyCmdZ1Gn, 0.5F);
    SET_FV(L_IyCmdZ2Gn, 0.5F);
    SET_FV(L_IyCmdA0Gn, 0.5F);
    SET_FV(L_IyCmdA1Gn, 0.5F);
    SET_FV(L_KVdc0,     0.5F);
    SET_FV(L_KVdc1,     0.5F);

    SET_FV(L_RegLimW, 50.0F );

    SET_FV(L_NsXStpAdr,(float *) LADR_FZero);
    SET_FV(L_NsYStpAdr, (float *) LADR_FZero);

    SET_FV(L_DcXStpAdr,(float *) LADR_FZero);
    SET_FV(L_DcYStpAdr, (float *) LADR_FZero);

    SET_FV(L_VrgStpAdr, (float *) LADR_FZero);
    SET_FV(L_IxrgStpAdr, (float *) LADR_FZero);
    SET_FV(L_IyrgStpAdr, (float *) LADR_FZero);



    // Dc current regulator rules
    SET_FV(L_DcIregBW, 30.0F);
    SET_FV(L_Dc1stD,0.2166F);//set to value of old rule output for 60hz.
    // 0 = "Original"
    // 1 = "Ireg LP filter:
    // 2 = "Feedback LP filter;
    // 3 = "Both"
    if ( PARM(L_IregFbkTyp) < 2)
    {
       SET_FV(L_Dc1stQ,-0.20F);
    }
    else
    {
       SET_FV(L_Dc1stQ,-0.3F);
    }
    SET_FV(L_DcRegWI, NoNANLim(PARM(L_DcIregBW)*SQRT3/PARM(L_VdcRef), 1.0e6F, -1.0e6F));
    SET_FV(L_DcIregMax, 0.1F);
    SET_FV(L_IregDcLim, NoNANLim(PARM(L_DcIregMax)/
         (PARM(L_DcRegWI)*sqrtf(PARM(L_Dc1stD)*PARM(L_Dc1stD) + PARM(L_Dc1stQ)*PARM(L_Dc1stQ))), 1.0e6F, -1.0e6F));

    // End Dc current injection rules


    // DC charging level settings and Ok permissive
    if (PARM(AVRPresent))
    {
       SET_FV(L_ChgLvlAc,   0.84F);
       SET_FV(L_VacTolrnce, 0.84F);
    }
    else
    {
       if (!PARM(VoltExtendOpt))
       {
          SET_FV(L_ChgLvlAc,   0.89F);
          SET_FV(L_VacTolrnce, 0.89F);
       }
       else
       {
          SET_FV(L_ChgLvlAc,   0.87F);
          SET_FV(L_VacTolrnce, 0.87F);
       }
    }

    SET_FV(L_ChgLvlDc,   0.81F);
    SET_FV(L_VdcOkRatio, 0.81F);

    if (PARM(AVRPresent))
    {
        SET_FV(L_ChgLvlAcDB,0.76F);
        SET_FV(L_ChgLvlDcDB,0.60F);
    }
    else
    {
        if ( PARM(TestMode) == PUMP_BACK )
        {
           SET_FV(L_ChgLvlAcDB,0.76F);
           SET_FV(L_ChgLvlDcDB,0.64F);
        }
        else
        {
           if (!PARM(VoltExtendOpt))
           {
              SET_FV(L_ChgLvlAcDB,0.81F);
              SET_FV(L_ChgLvlDcDB,0.67F);
           }
           else
           {
              SET_FV(L_ChgLvlAcDB,0.79F);
              SET_FV(L_ChgLvlDcDB,0.67F);
           }
        }
    }
    SET_FV(L_ChgLvlDcCM,0.22F);

    // Line Monitor
    W_Nom = PARM(L_LineFrq);

    SET_FV(L_LTstWtTm,    3.0F);
    SET_FV(L_LTstAvgSmp, 10000);
    SET_FV(L_LTstSlwTm,  20.0F);
    SET_FV(L_LTstSlwThr, 0.10F);
    //SET_FV(L_IyLTest, 0.9 * PARM(VregIyLim);
    SET_FV(L_IyLTest, 0.9F * PARM(L_IRat));//274*0.9

    SET_FV(L_VdcIxIyFlW, 300.0F);
    SET_FV(L_VdcIxIyOff, 1000.0F); //20.0 is the suggested gain if this funciton were used
    SET_FV(L_VdcIyGn, 2.0F);

    ///////////////////////////////////
    // Start BIL DB rules
    ///////////////////////////////////

    VdcNomCal = SQRT2*PARM(L_VllIn);

    SET_FV(DBVdcThrs, PARM(L_VdcRefMax) + 25.0F);
    SET_FV(DBGtOnThrs, PARM(DBVdcThrs)  - 25.0F);
    SET_FV(DBGtOffThr, PARM(DBGtOnThrs) - 50.0F);


    SET_FV(L_VMagT2FilW, 30.0F);

    if ( PARM(L_VMagT2FilW) > 0.0F )
    {
        L_VMagT2Gn = ( PARM(L_VMagT2FilW) * DelTm2 );
    }
    else
    {
        L_VMagT2Gn = 1.0F;
    }

    ///////////////////////////////////
    // End BIL DB rules
    ///////////////////////////////////

    ////////////////////////////////
    // Variable Calculation
    ////////////////////////////////

    SET_FV(L_IyHrmFFilW, 50.0F);
    L_IyHrmFFlGn = PARM(L_IyHrmFFilW) * DelTm2;

    //Previously in DiagVarFilterGain
    if ( PARM(L_WVdcFil) > 0.0F )
    {
        L_VdcFilGn = ( PARM(L_WVdcFil) * DelTm2 );
    }
    else
    {
        L_VdcFilGn = 1.0F;
    }

    if ( PARM(L_WIxFil) > 0.0F )
    {
        L_IxFilGn = ( PARM(L_WIxFil) * DelTm2 );
    }
    else
    {
        L_IxFilGn = 1.0F;
    }

    if ( PARM(L_WIyFil) > 0.0F )
    {
        L_IyFilGn = ( PARM(L_WIyFil) * DelTm2 );
    }
    else
    {
        L_IyFilGn = 1.0F;
    }

    if ( PARM(L_WVxFil) > 0.0F )
    {
        L_VxFilGn = ( PARM(L_WVxFil) * DelTm2 );
    }
    else
    {
        L_VxFilGn = 1.0F;
    }

    if ( PARM(L_WVyFil) > 0.0F )
    {
        L_VyFilGn = ( PARM(L_WVyFil) * DelTm2 );
    }
    else
    {
        L_VyFilGn = 1.0F;
    }

    if ( PARM(L_WVarFil) > 0.0F )
    {
        L_VarFilGn = ( PARM(L_WVarFil) * DelTm2 );
    }
    else
    {
        L_VarFilGn = 1.0F;
    }

    if ( PARM(L_WPwrLinFil) > 0.0F )
    {
        L_PwrLineGn = ( PARM(L_WPwrLinFil) * DelTm2 );
    }
    else
    {
        L_PwrLineGn = 1.0F;
    }

    if ( PARM(L_WVarLinFil) > 0.0F )
    {
        L_VarLineGn = ( PARM(L_WVarLinFil) * DelTm2 );
    }
    else
    {
        L_VarLineGn = 1.0F;
    }

    if ( PARM(L_WIMagFil) > 0.0F )
    {
        L_IMagFilGn = ( PARM(L_WIMagFil) * DelTm2 );
    }
    else
    {
        L_IMagFilGn = 1.0F;
    }

    if ( PARM(L_WDcBusPwr) > 0.0F )
    {
        L_DcPwrGn = ( PARM(L_WDcBusPwr) * DelTm2 );
    }
    else
    {
        L_DcPwrGn = 1.0F;
    }

    if ( PARM(L_WLineFil) > 0.0F )
    {
        L_LineMagGn = ( PARM(L_WLineFil) * DelTm2 );
    }
    else
    {
        L_LineMagGn = 1.0F;
    }

    if ( PARM(L_WIdcFil) > 0.0F )
    {
        L_IdcFilGn = ( PARM(L_WIdcFil) * DelTm2 );
    }
    else
    {
        L_IdcFilGn = 1.0F;
    }

    if ( PARM(L_WFrqFil) > 0.0F )
    {
        L_LineFrqGn = ( PARM(L_WFrqFil) * DelTm2 );
    }
    else
    {
        L_LineFrqGn = 1.0F;
    }

    if ( PARM(L_WCnvPwr) > 0.0F )
    {
        L_CnvPwrGn = ( PARM(L_WCnvPwr) * DelTm2 );
    }
    else
    {
        L_CnvPwrGn = 1.0F;
    }

    L_VlnInRcp   = NoNANLim(1.0F/L_VlnIn, 1.0e6F, -1.0e6F);
    L_VdcNomCal  = SQRT2*PARM(L_VllIn);
    L_VdcNomRcp  = NoNANLim(1.0F/L_VdcNomCal, 1.0e6F, -1.0e6F);
    L_LineFrqRcp = NoNANLim(1.0F/PARM(L_LineFrq), 1.0e6F, -1.0e6F);

    //Previously in Task 3
    L_VregWoComp = PARM(L_VregWoGn)*0.666666667F*PARM(L_CTot);
    L_VregGnRcp  = NoNANLim(1.0F/(L_VregWoComp*PARM(L_VregPrpGn)), 1.0e6F, -1.0e6F);


    //////////////////////////////////////
    //Previously in S0VarCal
    //////////////////////////////////////

    L_LTotCal = PARM(L_LTotSteady);
    L_LTotDt  = PARM(L_LTot)*L_DelTm1Rcp;

    L_IxCmdZ1Sc = PARM(L_IxCmdZ1Gn);
    L_IxCmdZ2Sc = PARM(L_IxCmdZ2Gn);
    L_IyCmdZ1Sc = PARM(L_IyCmdZ1Gn);
    L_IyCmdZ2Sc = PARM(L_IyCmdZ2Gn);
    L_IyCmdD0Sc = PARM(L_IyCmdD0Gn);
    L_IyCmdD1Sc = PARM(L_IyCmdD1Gn);
    L_IyCmdA0Sc = PARM(L_IyCmdA0Gn);
    L_IyCmdA1Sc = PARM(L_IyCmdA1Gn);

    L_VregPrpGnx = PARM(L_VregPrpGn);
    L_VregIntGnx = PARM(L_VregIntGn);
    L_IxLimPx    = PARM(L_IxLimP);
    L_IxLimNx    = PARM(L_IxLimN);
    L_VregWoGnx  = PARM(L_VregWoGn);

    L_VdcSlwx    = PARM(L_VdcSlw);
    L_VdcRefx    = PARM(L_VdcRef);
    L_IxrgCmdGnx = PARM(L_IxregCmdGn);
    L_IxrgIntLmx = PARM(L_IxregIntLm);
    L_IxrgIntGnx = PARM(L_IxregIntGn);
    L_IxrgFbkGnx = PARM(L_IxregFbkGn);
    L_IxrgULimx  = PARM(L_IxregULim);
    L_IrgSpilGnx = PARM(L_IregSpilGn);
    L_IyCmdLimx  = PARM(L_IyCmdLim);
    L_IyrgCmdGnx = PARM(L_IyregCmdGn);
    L_IyrgIntGnx = PARM(L_IyregIntGn);
    L_IyrgIntLmx = PARM(L_IyregIntLm);
    L_IyrgFbkGnx = PARM(L_IyregFbkGn);
    L_IyrgULimx  = PARM(L_IyregULim);
    L_IyRatSqr   = 2.0F*PARM(L_IyrfIMax)*PARM(L_IyrfIMax);  // = [sqrtf(2)* Iyrf_Rat]^2
    L_IyrfIntGnx = PARM(L_IyrfIntGn)*0.001F*DelTm2Rcp;


    /////////////////////////////////////
    //Parameter mirrors
    //////////////////////////////////////

    //Vreg
    L_VdcIxIyFlGn    =  PARM(L_VdcIxIyFlW) * L_DelTm1;



    // Q1
    //------------------
    if(SimAcclAct)
    {
        SET_FV(Q1SprChrgTm, 0.0F);
    }
    else
    {
        SET_FV(Q1SprChrgTm, 6.0F);  // Maximum charging time is 4 seconds, plus a fudge factor
    }                           // 7.0 ok since another apx. 3 seconds required
                                // before rotor conv. gives command to close
    if(SimAcclAct)
    {
        SET_FV(Q1UVPuDly,   0.0F);
    }
    else
    {
        SET_FV(Q1UVPuDly,   0.19F); // Twice maximum undervoltage-trip coil response time
    }
    if ( PARM(SoftwareIPN >= DFIG02_2400_5060_A) )
    {
       SET_FV(Q1OpenTypTm, 0.060F);  // Q1 typical open time
       SET_FV(Q1OpnDlyTm,  0.148F);  // Q1 opening time for checking status, it is the max between
                                // KDF, Q1 and K1 (230 msec) plus a fudge factor of 70 msec
        if(SimAcclAct)
        {
            SET_FV(Q1ClsDlyTm,  0.0F); // Twice Q1 maximum closing time
        }
        else
        {
            SET_FV(Q1ClsDlyTm,  0.14F); // Twice Q1 maximum closing time
        }
    }
    else
    {
       SET_FV(Q1OpenTypTm, 0.025F);// Q1 typical open time
       SET_FV(Q1OpnDlyTm,  0.3F);  // Q1 opening time for checking status, it is the max between
                                // KDF, Q1 and K1 (230 msec) plus a fudge factor of 70 msec
        if(SimAcclAct)
        {
            SET_FV(Q1ClsDlyTm,  0.0F); // Twice Q1 maximum closing time
        }
        else
        {
            SET_FV(Q1ClsDlyTm,  0.14F); // Twice Q1 maximum closing time
        }
    }
    SET_FV(HvacQ1OpnTm, PARM(Q1OpenTypTm)+0.04F);
    
    float temp_Q1VlThrs = (SQRT2/SQRT3) * PARM(L_VllIn);
    if(SimAcclAct)
    {
        SET_FV(Q1VlThrs, 1.0e6);
    }
    else
    {
        SET_FV(Q1VlThrs,  0.1F*temp_Q1VlThrs*0.1F*temp_Q1VlThrs); // Q1 breaker square voltage-difference threshold for sequencing
    }
    // Distortion filter current check
    //------------------

    float temp_L_DfQPwr;
    temp_L_DfQPwr=PARM(CnvLineDfC)*PARM(L_LineFrq)*PARM(L_VllIn)*PARM(L_VllIn)*(1.0F/3.0F);
    SET_FV(L_DfQPwrNomThrs,     0.45 * temp_L_DfQPwr);  // Value of this parameters comes out to be 12922.9 VARS
                                                        // The MACC value had been 15451.0 VARS
    SET_FV(L_DfQPwrLowThrs,     0.1 *  temp_L_DfQPwr);  // Value of this parameters comes out to be 2871.8  VARS
                                                        // The MACC value had been 3949.0 VARS

    // rules for SeqDemodulateLine
    SET_FV(L_Q1DeltVoltW, 30.0);               // Line secondary transf to line bridge difference filt omega
    L_Q1DeltVoltGn = DelTm2*PARM(L_Q1DeltVoltW);

    return (SUCCESS);

}   // LineSrcCtlRule
