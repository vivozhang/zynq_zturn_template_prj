///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rotor feedbacks through bridge-interface-layer
//
// DESCRIPTION:
//      Initialize and process hardware feedbacks.
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     y1Fbk.vsd, y1Fbk.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
//framework
#include  "x0FrameWork.h"
// product
#include  "Product.h"
#include  "b0BridgeRebf.h"
#include  "b0CelTst.h"
#include  "c1BrgCroBarCtl.h"
#include  "y1Fdbk.h"


// Externals
//----------
extern float     R_DelTm1;
extern float     R_DelTm1Rcp;
extern float     R_SpdSlowT2;
extern float     R_SpdT2RAF;
extern float     R_SpdFracT1;
extern float     R_SpdT2;
extern float     R_WRtrSlowT1;
extern float     R_WRtrSlowT2;
extern float     R_AngEstDtT1;
extern float     R_AngEstDtT2;
extern float     PwrPllWFF;
extern float     PwrPllWFFT1;
extern float     R_AdjTm1Rcp;
extern float     R_IaSqrT3;
extern float     R_IbSqrT3;
extern float     R_IcSqrT3;
extern float     R_IbSqrT3;
extern float     R_IcSqrT3;
extern float     R_VdcBrg;
extern float     R_VabBrg;
extern float     R_VbcBrg;
extern float     R_IaBrg;
extern float     R_IbBrg;
extern float     R_IcBrg;
extern float     R_MaxCurOut;
extern unsigned  Q1OpnReq;

extern unsigned  InitDone;
extern unsigned  T2DoneOnceT1;
extern unsigned  PwrDnFbkDsb;
extern unsigned  DelTrqPAct;

extern cCellTest     R_CellTest;
extern SysTm_Typ     SysTm2;            // T2 time passed from First T1
extern cBrgCroBarCtl R_BrgCroBarCtl;


// Constants
//----------


// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(R_TocTau,           float);     // Rotor bridge timed over-current time constant
CREATE_PARM(R_TocThrs,          float);     // Rotor bridge timed over-current threshold
CREATE_PARM(R_TocAlmOn,         float);     // Rotor bridge 1 timed over-current alarm "on" threshold
CREATE_PARM(R_TocAlmOff,        float);     // Rotor bridge timed over-current alarm "off" threshold
CREATE_PARM(R_TocMargin,        float);     // Timed over-current margin for generator (Pu)
CREATE_PARM(R_VdcMeld,          float);     // dc voltage meld coefficient
CREATE_PARM(R_ShuntVolts,       float);     // Shunt voltage at rated current
CREATE_PARM(R_IaShuntScl,       float);     // Phase A shunt scaling, amps/volts
CREATE_PARM(R_IaVcoScl,         float);     // Phase A current scaling, volts/hertz
CREATE_PARM(R_IbShuntScl,       float);     // Phase B shunt scaling, amps/volts
CREATE_PARM(R_IbVcoScl,         float);     // Phase B current scaling, volts/hertz
CREATE_PARM(R_IcShuntScl,       float);     // Phase C shunt scaling, amps/volts
CREATE_PARM(R_IcVcoScl,         float);     // Phase C current scaling, volts/hertz
CREATE_PARM(R_VabVcoScl,        float);     // A-B voltage scaling, volts/hertz
CREATE_PARM(R_VbcVcoScl,        float);     // B-C voltage scaling, volts/hertz
CREATE_PARM(R_VdcVcoScl,        float);     // DC link voltage scaling, volts/hertz
// Fault
CREATE_PARM(R_IabcFltThr,       float);     // Fast ground fault trip threshold
CREATE_PARM(R_IabcMskTm,        float);     // Fast ground fault trip mask out time after crowbar ends
CREATE_PARM(R_VdcUVLvl,         float);     // DC link undervoltage trip level
CREATE_PARM(R_VllMax,           float);     // Maximum line-line inverter output voltage
CREATE_PARM(R_WIabcGnd,         float);     // ABC ground current filter omega

// Diagnostics
//------------
CREATE_DIAG(R_TocAlm);              // Rotor bridge Timed over-current alarm
CREATE_DIAG(R_TocFlt);              // Rotor bridge Timed over-current Fault
CREATE_DIAG(R_IabcGndFlt);

// Variables
//----------
CREATE_PUBVAR(R_WRtrSlowT1, float);     /* Omega rotor slow for t*/
CREATE_PUBVAR(R_SpdFracT1,  float);        // Task1 speed from fractional tach algorithm
CREATE_PUBVAR(PwrPllWFFT1,  float);

// Dc link feedback signals
CREATE_PUBVAR(R_Vdc,        float);     // Units = volts
CREATE_PUBVAR(R_VdcFbk,     float);     // Units = volts
CREATE_PUBVAR(R_VdcFbkRcp,  float);     // dc link voltage reciprocal (volt^-1)

// Current feedback signals
CREATE_PUBVAR(R_Ia,         float);     // Units = amps
CREATE_PUBVAR(R_Ib,         float);     // Units = amps
CREATE_PUBVAR(R_Ic,         float);     // Units = amps
CREATE_PUBVAR(R_IaT2,       float);     // Units = amps
CREATE_PUBVAR(R_IbT2,       float);     // Units = amps
CREATE_PUBVAR(R_IcT2,       float);     // Units = amps

CREATE_PUBVAR(R_IabcGnd,    float);
CREATE_PUBVAR(R_IabcGndGn,  float);
CREATE_PUBVAR(R_IabcGndFil, float);
CREATE_PUBVAR(R_IabcMskTmr, float);
CREATE_PUBVAR(R_IabcMskAct, unsigned);

// Voltage feedback signals
CREATE_PUBVAR(R_Vab,       float);     // Units = volts
CREATE_PUBVAR(R_VabT2,     float);     // Units = volts
CREATE_PUBVAR(R_Vbc,       float);     // Units = volts
CREATE_PUBVAR(R_VbcT2,     float);     // Units = volts
CREATE_PUBVAR(R_VabBrgT2,  float);     // Rotor-1 phsA to phsB voltage in task2 (Volt)
CREATE_PUBVAR(R_VbcBrgT2,  float);     // Rotor-1 phsB to phsC voltage in task2 (Volt)
CREATE_PUBVAR(R_Vca,       float);     // Rotor-1 phsC to phsA voltage (Volt)
CREATE_PUBVAR(R_VbrgSqr,   float);     // Summation of squared rotor-1 bridge voltages (Volt^2)

// Hardware scaling variables
CREATE_PUBVAR(R_IaScl,      float);     // Units = amps/hertz
CREATE_PUBVAR(R_IbScl,      float);     // Units = amps/hertz
CREATE_PUBVAR(R_IcScl,      float);     // Units = amps/hertz
CREATE_PUBVAR(R_VabScl,     float);     // Units = volts/hertz
CREATE_PUBVAR(R_VbcScl,     float);

// Vdc Melding function
CREATE_PUBVAR(R_VdcComp,    float);
CREATE_PUBVAR(R_VdcCompRcp, float);

#ifdef STATOR_DCREG_RAF
//Rotor DC current feedback variables
CREATE_PUBVAR(S_IAlpha,     float);     // Stator DC current regulator alpha current
CREATE_PUBVAR(S_IBeta,      float);     // Stator DC current regulator beta current
CREATE_PUBVAR(S_DcIxFbk,    float);     // Stator DC current regulator alpha rolling average filter output
CREATE_PUBVAR(S_DcIyFbk,    float);     // Stator DC current regulator beta rolling average filter output
float S_RAFAlphStk[22];                 // Stator RAF array for Dc Alpha current shold be (1/(50hz)*(5712/6) + 2, add 1 for cushion
float S_RAFBetaStk[22];                 // Stator RAF array for Dc Alpha current shold be (1/(50hz)*(5712/6) + 2, add 1 for cushion
#endif

// TocDetection Vars
CREATE_PUBVAR(R_TocAlmFlg,  unsigned);  // rotor combined bridge TOC alam flag
CREATE_PUBVAR(R_WTocT3,        float);  // Omega for timed over-current (normal) - task 3
CREATE_PUBVAR(R_WTocFstT3,     float);  // Omega for timed over-current (fast) - task 3
CREATE_PUBVAR(R_WTocT3Dyn,     float);  // Omega for timed over-current - task 3
CREATE_PUBVAR(R_IaRmsMag,      float);  // rotor Phase A current rms magnitude
CREATE_PUBVAR(R_IbRmsMag,      float);  // rotor Phase B current rms magnitude
CREATE_PUBVAR(R_IcRmsMag,      float);  // rotor Phase C current rms magnitude
CREATE_PUBVAR(R_IaRms,         float);  // rotor bridge combined Phase A current rms
CREATE_PUBVAR(R_IbRms,         float);  // rotor bridge combined Phase B current rms
CREATE_PUBVAR(R_IcRms,         float);  // rotor bridge combined Phase C current rms
CREATE_PUBVAR(R_TocHiSel,      float);  // High select for rotor TOC model
CREATE_PUBVAR(R_TocHiSelSq,    float);  // High select squared for rotor TOC model


// Generator dynamic Toc threshold
CREATE_PUBVAR(R_TocThrDy  ,    float);  // Generator over-current dymanic trip threshold (Amp)
CREATE_PUBVAR(R_TocAlmOnDy,    float);  // Generator over-current dymanic alarm on threshold (Amp)
CREATE_PUBVAR(R_TocAlmOffDy,   float);  // Generator over-current dymanic alarm off threshold (Amp)

// Instrument signals
CREATE_PUBVAR(R_IaSqr,      float);     // Rotor Ia sqared current
CREATE_PUBVAR(R_IbSqr,      float);     // Rotor Ib sqared current
CREATE_PUBVAR(R_IcSqr,      float);     // Rotor Ic sqared current
CREATE_PUBVAR(R_IaSqrT2,    float);     // Rotor Ia sqared current Task 2 version
CREATE_PUBVAR(R_IbSqrT2,    float);     // Rotor Ib sqared current Task 2 version
CREATE_PUBVAR(R_IcSqrT2,    float);     // Rotor Ic sqared current Task 2 version
CREATE_PUBVAR(R_IaSqrT3,    float);     // Rotor Ia sqared current Task 3 version
CREATE_PUBVAR(R_IbSqrT3,    float);     // Rotor Ib sqared current Task 3 version
CREATE_PUBVAR(R_IcSqrT3,    float);     // Rotor Ic sqared current Task 3 version

CREATE_PUBVAR(R_VabSqr,     float);     // Rotor Vab sqared voltage
CREATE_PUBVAR(R_VbcSqr,     float);     // Rotor Vbc sqared voltage
CREATE_PUBVAR(R_VabSqrT2,   float);     // Rotor Vab sqared voltage Task 2 version
CREATE_PUBVAR(R_VbcSqrT2,   float);     // Rotor Vbc sqared voltage Task 2 version
CREATE_PUBVAR(R_VabSqrT3,   float);     // Rotor Vab sqared voltage Task 3 version
CREATE_PUBVAR(R_VbcSqrT3,   float);     // Rotor Vbc sqared voltage Task 3 version
CREATE_PUBVAR(R_SpdSlowT1,    float);        // slow speed from T2 rolling average filter  snapshot in T1


// Unpublished Variables
//----------------------
float R_TocAlmOnDl;
float R_TocAlmOffDl;

// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Data Passing
//--------------
DATA_PASS(R_Ia,         R_IaT2,         T1A_T2, FILTERED);
DATA_PASS(R_Ib,         R_IbT2,         T1A_T2, FILTERED);
DATA_PASS(R_Ic,         R_IcT2,         T1A_T2, FILTERED);

DATA_PASS(R_VabBrg,     R_VabBrgT2,     T1A_T2, FILTERED );
DATA_PASS(R_VbcBrg,     R_VbcBrgT2,     T1A_T2, FILTERED );

DATA_PASS(R_VabSqr,     R_VabSqrT2,     T1A_T2, FILTERED);
DATA_PASS(R_VbcSqr,     R_VbcSqrT2,     T1A_T2, FILTERED);
DATA_PASS(R_VabSqrT2,   R_VabSqrT3,     T2_T3,  FILTERED);
DATA_PASS(R_VbcSqrT2,   R_VbcSqrT3,     T2_T3,  FILTERED);

DATA_PASS(R_IaSqr,      R_IaSqrT2,      T1A_T2, FILTERED);
DATA_PASS(R_IbSqr,      R_IbSqrT2,      T1A_T2, FILTERED);
DATA_PASS(R_IcSqr,      R_IcSqrT2,      T1A_T2, FILTERED);
DATA_PASS(R_IaSqrT2,    R_IaSqrT3,      T2_T3,  FILTERED);
DATA_PASS(R_IbSqrT2,    R_IbSqrT3,      T2_T3,  FILTERED);
DATA_PASS(R_IcSqrT2,    R_IcSqrT3,      T2_T3,  FILTERED);

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Processing of rotor bridge-interface-layer primary feedbacks.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  RotorBILPriFbks2( void )
{
    R_Vca = - (R_VabBrgT2 + R_VbcBrgT2);

    // capture summation of squared rotor bridge voltages
    // multiplying constant 2/3=0.66666 cancels out the 3/2 introduced when
    // adding squared balanced sinusoidal signals.
    R_VbrgSqr = 0.66666F * ( (R_VabBrgT2*R_VabBrgT2) + (R_VbcBrgT2*R_VbcBrgT2) + (R_Vca*R_Vca) );

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Calculation of rotor bridge-interface-layer primary feedbacks used
//      in control logic. This includes the three shunts and two L-L voltages.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorBILPriFbks1( void )
{

    //   Calculate DC link voltage and perform overvoltage check
    //   The R_Vdc calculation and check must executed before any other
    //   feedback code.  The ASIC DC overvoltage fault mask bit must
    //   be configured during initialization for the bridge to power
    //   off correctly in the event of a DC overvoltage.

    R_Vdc        = R_VdcBrg;
    R_VdcFbk     = R_Vdc;
    R_VdcFbkRcp  = NoNANLim( 1.0F / R_VdcFbk, 100.0e6, -100.0e6 );
    R_VdcComp    = (PARM(R_VdcMeld)*PARM(R_VdcNom)) + ((1.0F - PARM(R_VdcMeld))*R_VdcFbk);
    R_VdcCompRcp = NoNANLim( (float)SQRT3/R_VdcComp, 100.0e6, -100.0e6 );


    if ( PARM(R_BrInfcHdw) != cBridgeRebf::BRG_INFC_NONE )  // account for brg1 if it has a control-board connected
    {
        R_Vab      = R_VabBrg;
        R_Vbc      = R_VbcBrg;
        R_Ia       = R_IaBrg;
        R_Ib       = R_IbBrg;
        R_Ic       = R_IcBrg;
    }


    if ( InitDone )
    {
        // Calculate ground current based on phase currents
        R_IabcGnd = R_Ia + R_Ib + R_Ic;

        // Filter ground current and test against trip level
        if ( R_BrgCroBarCtl.CroBarAct || R_IabcMskAct ||
             !R_CellTest.CelTstProtEnab )
        {
           R_IabcGndFil = 0.0;

           //Latch it in until time satisfied
           R_IabcMskAct = true;
           if ( !R_BrgCroBarCtl.CroBarAct)
           {
               R_IabcMskTmr += R_DelTm1;
               if ( R_IabcMskTmr >= PARM(R_IabcMskTm) )
               {
                   R_IabcMskTmr = 0.0;
                   R_IabcMskAct = false;
               }
           }
        }
        else
        {
           R_IabcGndFil += R_IabcGndGn * ( ABS( R_IabcGnd ) - R_IabcGndFil );
        }

        if ( R_IabcGndFil >= PARM(R_IabcFltThr) )
        {
            PUSH_DIAG(R_IabcGndFlt);
            Q1OpnReq = true;
        }
    }

    // Calculate square of generator phase current feedbacks
    R_IaSqr = R_Ia * R_Ia;
    R_IbSqr = R_Ib * R_Ib;
    R_IcSqr = R_Ic * R_Ic;

    R_VabSqr = R_Vab * R_Vab;
    R_VbcSqr = R_Vbc * R_Vbc;

    R_SpdFracT1  = R_SpdT2;
    R_SpdSlowT1  = R_SpdSlowT2;
    R_WRtrSlowT1 = R_WRtrSlowT2;
    R_AngEstDtT1 = R_AngEstDtT2;
    PwrPllWFFT1  = PwrPllWFF;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This function performs a timed over-current check on each of the phase
//    currents.  The inputs are squared, then lightly filtered phase currents.
//    This function filters these signals much more by an adjustable amount.
//    The outputs of these filters is then compared to an adjustable
//    threshold.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GenTocDetection(void)
{


    R_IaRmsMag = R_IaRmsMag + R_WTocT3Dyn*(R_IaSqrT3 - R_IaRmsMag);
    R_IbRmsMag = R_IbRmsMag + R_WTocT3Dyn*(R_IbSqrT3 - R_IbRmsMag);
    R_IcRmsMag = R_IcRmsMag + R_WTocT3Dyn*(R_IcSqrT3 - R_IcRmsMag);

    R_IaRms   = sqrtf(R_IaRmsMag);
    R_IbRms   = sqrtf(R_IbRmsMag);
    R_IcRms   = sqrtf(R_IcRmsMag);

    // if or not to use the dynamic TOC configuration
    if ( PARM(TocDyThrshEnb) )
    {
        if ( PARM(DelSpdTrqAct) )
        {
            R_TocThrDy    = R_MaxCurOut * PARM(R_TocMargin);
        }
        else
        {
            R_TocThrDy    = R_MaxCurOut;
        }
        R_TocAlmOnDy  = R_TocThrDy - R_TocAlmOnDl;
        R_TocAlmOffDy = R_TocAlmOnDy - R_TocAlmOffDl;
    }
    else
    {
        R_TocThrDy    = PARM(R_TocThrs);
        R_TocAlmOnDy  = PARM(R_TocAlmOn);
        R_TocAlmOffDy = PARM(R_TocAlmOff);
    }

    if ( R_IaRmsMag > R_IbRmsMag)
    {
         R_TocHiSelSq = R_IaRmsMag;
    }
    else
    {
         R_TocHiSelSq = R_IbRmsMag;
    }

    if ( R_IcRmsMag > R_TocHiSelSq )
    {
        R_TocHiSelSq = R_IcRmsMag;
    }

    R_TocHiSel = sqrtf(R_TocHiSelSq);

    if ( R_TocHiSel >= R_TocThrDy )
    {
        PUSH_DIAG(R_TocFlt);
    }

    if ( R_TocHiSel >= R_TocAlmOnDy )
    {
        R_TocAlmFlg = true;
    }
    else if ( R_TocHiSel <= R_TocAlmOffDy )
    {
        R_TocAlmFlg = false;
    }

    if ( R_TocAlmFlg )
    {
        PUSH_DIAG(R_TocAlm);
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This function calculates variables used for scaling feedback quantities,
//    which occurs in routines with faster execution rates.
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  RotorBILPriFbksBgnd( void )
{
    // Determine phase current scalings

    R_IaScl      = PARM(R_IaVcoScl) * PARM(R_IaShuntScl);
    R_IbScl      = PARM(R_IbVcoScl) * PARM(R_IbShuntScl);
    R_IcScl      = PARM(R_IcVcoScl) * PARM(R_IcShuntScl);
    R_VabScl     = PARM(R_VabVcoScl);
    R_VbcScl     = PARM(R_VbcVcoScl);
    R_IabcGndGn  = PARM(R_WIabcGnd) * R_DelTm1;

    return;
}
