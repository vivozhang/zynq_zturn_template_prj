///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Common side product rules
//
// DESCRIPTION:
//      This file contains rule calculations for common control final values
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
// product
#include "Product.h"

// Constants
//----------
#define SINUSOIDAL   1
#define CAP_TASK_1_RATE 1
#define IO_VCO_MAXFREQ   ( 2.0e6 )

// Externals
//----------
extern float            DelTm2;
extern float            DelTm2Rcp;

extern float            L_AeadDin3Gn;
extern float            L_AeadDin4Gn;
extern float            L_AeadDin5Gn;
extern float            L_AeadDin6Gn;
extern float            L_AeadDin9Gn;
extern float            L_AeadDin10Gn;

extern float            R_AeadDin3Gn;
extern float            R_AeadDin4Gn;
extern float            R_AeadDin5Gn;
extern float            R_AeadDin6Gn;
extern float            R_AeadDin9Gn;
extern float            R_AeadDin10Gn;

extern unsigned         Stopped;

extern float            R_TrqRat;

extern float            DbResCombined;
extern float            BrgDbTauElec;
extern float            L_PwrDisconTm;
extern float            ContVmagPuHwTstGn;

extern float            L_VdcFbkT3;
extern float            L_VdcFbkT2;
extern float            GmVMagPuT3;
extern float            VsecLossMgGn;
extern float            HdwDiagVcoGn;
extern float            LqdColingGn;
extern float            BpToJncBadGn;

extern float            DelTm1;
extern float            DelTm1A;
extern float            DelTm3;

extern float            PulseCycTm;
extern float            PulseDelMgDt;
extern float            PulseSineW;
extern unsigned         PulseTypSt;
extern float            PulseHalfDt;
extern unsigned         TriangTypSt;
extern unsigned         PulseGenInit;
extern float            PulseFreqTar;
extern float            PulseGenFreq;
extern float            PulseGenMag;
extern unsigned         PulseGenCyc;
extern unsigned         PlsGenInitZ1;
extern float            PulseGenScl;
extern float            PulseGenSclNtv;
extern float            AeaaHv3FbkT2Gn;
extern float            HdwBadVdcWdt;

// Types and Classes
//------------------

// Published Variables
//--------------------
CREATE_PUBVAR(XRuleCtr,         unsigned);
CREATE_PUBVAR(HdwTstMode,       unsigned);     // I/O test mode

CREATE_PUBVAR(PulseFreqTar,     float);
CREATE_PUBVAR(PulseGenFreq,     float);
CREATE_PUBVAR(PulseGenMag,      float);
CREATE_PUBVAR(PulseGenCyc,      unsigned);
CREATE_PUBVAR(PulseGenScl,      float);

// Unpublished Variables
//----------------------

// Local Prototypes (to resolve forward references)
//-------------------------------------------------
int CommonRule(void);



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Specifies which rule sets where to run common rules.
//
// EXECUTION LEVEL
//      Rule execution
//
// RETURN VALUE
//      Status
//
///////////////////////////////////////////////////////////////////////////////
int RunCommonRules(int Rule_Set)
{
    float    PulseFrqHalfCyc, PulseTotTm;
    float    TimeHalfCyc, PulsesTotFlt;
    unsigned DTsHalfCyc;

    SET_FV(L_RevSeqReq,  false);
    SET_FV(L_IxregWFf,   45.0);   //no calc required necessary
    SET_FV(L_EETest1,     0.0);   //no calc required necessary
    SET_FV(L_EETest2,     0.0);   //no calc required necessary
    SET_FV(L_WDcBusPwr  , 10.0);  //no calc required necessary
    SET_FV(L_WFrqFil    , 10.0);  //no calc required necessary
    SET_FV(L_WIMagFil   , 10.0);  //no calc required necessary
    SET_FV(L_WIdcFil    , 10.0);  //no calc required necessary
    SET_FV(L_WIxFil     , 10.0);  //no calc required necessary
    SET_FV(L_WIyFil     , 10.0);  //no calc required necessary
    SET_FV(L_WLineFil   , 10.0);  //no calc required necessary
    SET_FV(L_WCnvPwr    , 10.0);  //no calc required necessary
    SET_FV(L_WPwrLinFil , 10.0);  //no calc required necessary
    SET_FV(L_WVarFil    , 10.0);  //no calc required necessary
    SET_FV(L_WVarLinFil , 10.0);  //no calc required necessary
    SET_FV(L_WVdcFil    , 10.0);  //no calc required necessary
    SET_FV(L_WVxFil     , 10.0);  //no calc required necessary
    SET_FV(L_WVyFil     , 10.0);  //no calc required necessary







    SET_FV(L_AeadDin3W , 0.0);
    SET_FV(L_AeadDin4W , 0.0);
    SET_FV(L_AeadDin5W , 0.0);
    SET_FV(L_AeadDin6W , 0.0);
    SET_FV(L_AeadDin9W , 0.0);
    SET_FV(L_AeadDin10W, 0.0);

    SET_FV(R_AeadDin3W , 0.0);
    SET_FV(R_AeadDin4W , 0.0);
    SET_FV(R_AeadDin5W , 0.0);
    SET_FV(R_AeadDin6W , 0.0);
    SET_FV(R_AeadDin9W , 0.0);
    SET_FV(R_AeadDin10W, 0.0);

    // -------------
    // Line 690-Aead

    if (PARM(L_AeadDin3W) <= 0.0)
    {
        L_AeadDin3Gn =  1;
    }
    else
    {
        L_AeadDin3Gn =  DelTm2*PARM(L_AeadDin3W);
    }

    if (PARM(L_AeadDin4W) <= 0.0)
    {
        L_AeadDin4Gn =  1;
    }
    else
    {
        L_AeadDin4Gn =  DelTm2*PARM(L_AeadDin4W);
    }

    if (PARM(L_AeadDin5W) <= 0.0)
    {
        L_AeadDin5Gn =  1;
    }
    else
    {
        L_AeadDin5Gn =  DelTm2*PARM(L_AeadDin5W);
    }

    if (PARM(L_AeadDin6W) <= 0.0)
    {
        L_AeadDin6Gn =  1;
    }
    else
    {
        L_AeadDin6Gn =  DelTm2*PARM(L_AeadDin6W);
    }

    if (PARM(L_AeadDin9W) <= 0.0)
    {
        L_AeadDin9Gn =  1;
    }
    else
    {
        L_AeadDin9Gn =  DelTm2*PARM(L_AeadDin9W);
    }

    if (PARM(L_AeadDin10W) <= 0.0)
    {
        L_AeadDin10Gn =  1;
    }
    else
    {
        L_AeadDin10Gn =  DelTm2*PARM(L_AeadDin10W);
    }

    // -------------
    // Rotor 6K-Aead

    if (PARM(R_AeadDin3W) <= 0.0)
    {
        R_AeadDin3Gn =  1;
    }
    else
    {
        R_AeadDin3Gn =  DelTm2*PARM(R_AeadDin3W);
    }

    if (PARM(R_AeadDin4W) <= 0.0)
    {
        R_AeadDin4Gn =  1;
    }
    else
    {
        R_AeadDin4Gn =  DelTm2*PARM(R_AeadDin4W);
    }

    if (PARM(R_AeadDin5W) <= 0.0)
    {
        R_AeadDin5Gn =  1;
    }
    else
    {
        R_AeadDin5Gn =  DelTm2*PARM(R_AeadDin5W);
    }

    if (PARM(R_AeadDin6W) <= 0.0)
    {
        R_AeadDin6Gn =  1;
    }
    else
    {
        R_AeadDin6Gn =  DelTm2*PARM(R_AeadDin6W);
    }

    if (PARM(R_AeadDin9W) <= 0.0)
    {
        R_AeadDin9Gn =  1;
    }
    else
    {
        R_AeadDin9Gn =  DelTm2*PARM(R_AeadDin9W);
    }

    if (PARM(R_AeadDin10W) <= 0.0)
    {
        R_AeadDin10Gn =  1;
    }
    else
    {
        R_AeadDin10Gn =  DelTm2*PARM(R_AeadDin10W);
    }

    SET_FV(Oscill       ,  60.0e6);





    //Start pulse generator rules
    SET_FV(PulseGenPu, true);
    SET_FV(PulseMagLim,  0.20F); //0.20pu torqe
    SET_FV(PulseFreqLim, 25.0F); //Maximum frequency
    SET_FV(PulseCycLim, 20);
    SET_FV(PulseBrstCyc, 1);

    PulseFreqTar = PARM(PulseFreqReq);

    if ( PARM(PulseGenPu))
    {

       if (PARM(R_TrqStpAdr)  == (float*) LADR_PulseGenOut)
       {
         PulseGenScl    = R_TrqRat;
         PulseGenSclNtv = R_TrqRat; //"Ntv" is native value
       }
       else if (PARM(VregIyStpAdr)  == (float*) LADR_PulseGenOut)
       {
         PulseGenScl    =       PARM(SysIratStp);
         PulseGenSclNtv = SQRT2*PARM(SysIratStp);
       }
       else
       {
         PulseGenScl    = 1.0; //then don't scale...keep in per unit
         PulseGenSclNtv = 1.0;
       }

       //Pulse magnitdue restrictions
       if ( PARM(PulseMagReq) > PARM(PulseMagLim) )
       {
          SET_FV (PulseMagMax,  PARM(PulseMagLim)*PulseGenSclNtv);
          SET_FV (PulseMagMin, -PARM(PulseMagLim)*PulseGenSclNtv);
       }
       else
       {
          SET_FV (PulseMagMax,  PARM(PulseMagReq)*PulseGenSclNtv);
          SET_FV (PulseMagMin, -PARM(PulseMagReq)*PulseGenSclNtv);
       }

       //Pulse frequency restrictions
       if ( PulseFreqTar < PARM(PulseFreqLim) )
       {
            PulseFreqTar = PARM(PulseFreqLim);
       }

       if ( PARM(PulseCycReq) > PARM(PulseCycLim))
       {
           SET_FV(PulseCycles, PARM(PulseCycLim));
       }
       else
       {
           SET_FV(PulseCycles, PARM(PulseCycReq));
       }

    }
    else
    {
       SET_FV (PulseMagMax,  PARM(PulseMagReq));
       SET_FV (PulseMagMin, -PARM(PulseMagReq));
       SET_FV(PulseCycles, PARM(PulseCycReq));
       PulseGenScl = 1.0;

    }

    PulseGenMag = PARM(PulseMagMax);
    PulseGenCyc = PARM(PulseCycles);

    PulseTypSt  = PARM(PulseGenType);
    PulseCycTm  = 1.0/PulseFreqTar;
    PulseHalfDt = 0.5*DelTm2;
    PulseSineW  = PI2*PulseFreqTar;

    PulseTotTm  = PulseCycTm * PulseGenCyc; //for more accurate method
    PulsesTotFlt= (int)( (PulseTotTm * DelTm2Rcp) + 0.5);


    PulseFrqHalfCyc = 2.0*PulseFreqTar;
    TimeHalfCyc     = 1.0/PulseFrqHalfCyc;
    DTsHalfCyc      = (TimeHalfCyc * DelTm2Rcp) + 0.50;
    if ( (PARM(PulseGenType) == 0) || PARM(PulseGenType) == 3)//fixed square wave or fixed triangular
    {
       PulseGenFreq    = 1.0/ (2.0 * DTsHalfCyc * DelTm2);
    }
    else
    {
       PulseGenFreq    = 1.0/( (PulsesTotFlt * DelTm2)/PulseGenCyc);
    }

    SET_FV(PulseT2sMax, DTsHalfCyc);
    SET_FV(PulseRptTm,  1.0F);//Time between bursts if multiple bursts
    SET_FV(PulseWaitTm, 4.0F);//Time before returning to idle state
    SET_FV(PulseKillThr,3000.0F);//Magnitude of V.R_TorTrqMag at which pulse generator is killed


    PulseDelMgDt = 2.0*PulseGenMag/PARM(PulseT2sMax);

    //End pulse generator rules


    if ( PARM(PulseGenEnb) && !PlsGenInitZ1 )
    {
        PulseGenInit = true; //Cleared by pulse generator code.
    }
    PlsGenInitZ1 = PARM(PulseGenEnb);

    XRuleCtr += 1;

    if ( Stopped && (PARM(TestMode) == HARDWARE_TEST) )
    {
        HdwTstMode = true;
    }
    else
    {
        HdwTstMode = false;
    }
    SET_FV(WemaEnbSimRq, PARM(SimModeReq));
    SET_FV(WemaPicRevCk, 22);
    SET_FV(WemaFpgaVerOffset, 100);
    // Combination of multiple DB resistors (Ohms), as it might apply
    DbResCombined = PARM(BrgDbR);

    // Electrical time constant of one DB resistor (Sec)
    //TODO, this is different from MACC
    BrgDbTauElec = NoNANLim( PARM(BrgDbL) / PARM(BrgDbR), 1.0e6F, -1.0e6F );

    // Line bridge power disconnection time (Sec)
    L_PwrDisconTm = 1.1F * PARM(Q1OpnDlyTm);

    switch (Rule_Set)
    {
        case RULE_SET_MAIN:
        case RULE_SET_DEFAULT_ALL:
            CommonRule();
            break;

        default:
            break;
    }

    SET_FV(HdwDiagEnabled, true);

    SET_FV(HdwDiagVcoW, 1.0);
    SET_FV(VsecLossMgW  , 3.0F);
    SET_FV(VsecLossThrs, 0.1F);
    SET_FV(VsecLossTm, 10.0F);
    VsecLossMgGn = PARM(VsecLossMgW) * DelTm3;

    SET_FV(HdwBadVdcThr, 490.0F);
    SET_FV(HdwBadVdcW, 25.0F);
    HdwBadVdcWdt = DelTm2 * PARM(HdwBadVdcW);

    HdwDiagVcoGn = DelTm3 * PARM(HdwDiagVcoW);

    SET_FV(HdwVcoHiBadThr, 1.98e6F);
    SET_FV(HdwVcoLoBadThr, 100.00F);

    SET_FV(HdwFanFailTempThr, 7.0F);
    SET_FV(LqdColingW, 1.0F);
    SET_FV(LqdColingBadThr, 30.0F);
    SET_FV(BpToJncW, 1.0F);
    SET_FV(BpToJncBadThr, -2.0F);
    LqdColingGn = PARM(LqdColingW) * DelTm3;
    BpToJncBadGn = PARM(BpToJncW) * DelTm3;

    SET_FV(FchkCcontOpnTm,   1.00F);
    SET_FV(FchkGmLnDelThr,   0.30F);
    SET_FV(FchkDlyTmr,       1.00F);
    SET_FV(FchkGmVMgThr,     0.70F);
    SET_FV(FchkGmLnDelTm,    0.01F);

    SET_FV(HdwSysTmThr,      30);

    SET_FV(K1RunPermMinTemp, -5.0F);

    SET_FV(InitBufSavArmTm, 5.0F);

    SET_FV(AeaaHv3DcFbkEnb, true);
    SET_FV(AeaaHv3Scl, 0.00708F);
    SET_FV(AeaaHv3FbkT2W, 1.0/DelTm3);
    AeaaHv3FbkT2Gn = PARM(AeaaHv3FbkT2W) * DelTm2;

    return (SUCCESS);
}   // RunCommonRules()



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Run rules that are common to all products.
//
// EXECUTION LEVEL
//      Rule execution
//
// RETURN VALUE
//      Status
//
///////////////////////////////////////////////////////////////////////////////
int CommonRule(void)
{
    //--------------------------------------------------------------------------
    //       Rule Calculations

    //  Frame Phaselock Loop (timebase synchronization)



    // Capture buffer trip fault trigger inhibit





    // Line 690-Aead test discrete I/O
    SET_FV(L_AeadK1Tst, false);
    SET_FV(L_AeadK2Tst, false);
    SET_FV(L_AeadK3Tst, false);
    SET_FV(L_AeadK4Tst, false);
    SET_FV(L_AeadK5Tst, false);
    SET_FV(L_AeadK6Tst, false);
    SET_FV(L_AeadK7Tst, false);
    SET_FV(L_AeadK8Tst, false);
    SET_FV(L_AeadK9Tst, false);

    SET_FV(ContVmagPuHwTstW, 10.0);
    ContVmagPuHwTstGn = PARM(ContVmagPuHwTstW) * DelTm2;

    // Rotor Aead test discrete I/O
    SET_FV(R_AeadK1Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_AeadK2Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_AeadK3Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_AeadK4Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_AeadK5Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_AeadK6Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_AeadK7Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_AeadK8Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_AeadK9Adr, (uint * ) LADR_UnusedUInt);


    // Rotor GEN-Aead test discrete I/O
    SET_FV(R_GenAeadK1Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_GenAeadK2Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_GenAeadK3Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_GenAeadK4Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_GenAeadK5Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_GenAeadK6Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_GenAeadK7Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_GenAeadK8Adr, (uint * ) LADR_UnusedUInt);
    SET_FV(R_GenAeadK9Adr, (uint * ) LADR_UnusedUInt);

    // Line Rebf discrete I/O
    SET_FV(L_RebfLvK1Adr, (uint * ) LADR_UnusedBool);
    SET_FV(L_RebfLvK2Adr, (uint * ) LADR_UnusedBool);
    SET_FV(L_RebfLvK3Adr, (uint * ) LADR_UnusedBool);

    return(SUCCESS);

}   // CommonRule()
