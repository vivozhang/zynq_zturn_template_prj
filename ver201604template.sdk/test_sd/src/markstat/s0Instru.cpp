

/* INLINE EXPANSION CONTROL */
///////////////////////////////////////////////////////////////////////////////
//
// TITLE:        Source-side instrumentation
//
// DESCRIPTION:
//      Line calculation functions.
//
// COPYRIGHT:   Copyright (c) 2014
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     s0Instru.vsd, s0Instru.xls
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// core
//framework
#include   "x0FrameWork.h"

// product
#include   "Product.h"
#include   "s0Instru.h"


// Externals

extern float    L_IxFbk;              // L_Ix feedback
extern float    L_IyFbk;              // L_Iy feedback
extern float    L_IMagT1;             // fast current magnitude calculation
extern float    DelTm2;
extern float    GmPllWT2;
extern float    GmVMagT2;
extern float    GmVxFbkT2;
extern float    GmVyFbkT2;
extern float    L_BrgLossPwr;
extern float    L_IxFbkT2;
extern float    L_IxyGndFil;
extern float    L_IyFbkT2;
extern float    L_PllWNom;
extern float    L_VabT2;
extern float    L_VbcT2;
extern float    L_VdcFbk;
extern float    L_VdcFbkT2;
extern float    L_VdcIxIyFlGn;
extern float    L_VxFbk;
extern float    L_VxFbkT2;
extern float    L_VyFbkT2;
extern unsigned GmRevSeq;
extern unsigned Q1ClsIp;
extern unsigned L_AcLinePres;

// DiagVarCalcT2()
extern unsigned     KdfClsCmd;
extern unsigned     Q1ClsIp;
// Constants
//----------


// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(L_VdcIxIyOff,       float);     // vdc current reduction offset
CREATE_PARM(L_KVdc0,            float);     // L_Vdc FIR filter gain 0
CREATE_PARM(L_KVdc1,            float);     // L_Vdc FIR filter gain 1
CREATE_PARM(L_VdcIyGn,          float);     // vdc current reduction gain for line Iy
CREATE_PARM(L_VdcIxIyFlW,       float);     // vdc current reduction filter omega
CREATE_PARM(L_WVdcFil,          float);     // L_Vdc diagnostic variable filter omega
CREATE_PARM(L_WIxFil,           float);     // L_Ix diagnostic variable filter omega
CREATE_PARM(L_WIyFil,           float);     // L_Iy diagnostic variable filter omega
CREATE_PARM(L_WVxFil,           float);     // L_Vx diagnostic variable filter omega
CREATE_PARM(L_WVyFil,           float);     // L_Vy diagnostic variable filter omega
CREATE_PARM(L_WIMagFil,         float);     // Current magnitude diagnostic filter omega
CREATE_PARM(L_WLineFil,         float);     // Omega for line magnitude filter
CREATE_PARM(L_WIdcFil,          float);     // Omega for dc current diagnositic filter
CREATE_PARM(L_WCnvPwr,          float);     // Omega for converter output power diagnositic filter
CREATE_PARM(L_WDcBusPwr,        float);     // Omega for Dc output power diagnositic fil.
CREATE_PARM(L_WFrqFil,          float);     // Electrical freq diagnostic filter omega
CREATE_PARM(L_WVarFil,          float);     // Omega for filtered volt-amp-reactive
CREATE_PARM(L_WPwrLinFil,       float);     // Omega for filtered power into line
CREATE_PARM(L_WVarLinFil,       float);     // Omega for filtered VARs into line
CREATE_PARM(L_VMagT2FilW,       float);     // Response for L_VMagT2Fil

// Diagnostics
//------------


// Variables

CREATE_PUBVAR(L_IMagT1      , float);        // fast current magnitude calculation
CREATE_PUBVAR(L_IyHrmFCal   , float);        // calculated harmonic filter y-current
CREATE_PUBVAR(L_VarHrmF     , float);        // harmonic filter VARs

// Instrument
CREATE_PUBVAR(L_VdcReg      , float);        // FIR filter output
CREATE_PUBVAR(L_VdcFbkZ0    , float);        // L_Vdc feedback FIR filter state 0
CREATE_PUBVAR(L_VdcFbkZ1    , float);        // L_Vdc feedback FIR filter state 1

// InstrumentT2 */
CREATE_PUBVAR(L_BrdgLoss    , float);
CREATE_PUBVAR(L_VdcRcpT2    , float);
CREATE_PUBVAR(L_IdcT2       , float);
CREATE_PUBVAR(L_VMagT2      , float);


    // Diagnostic variable filter outputs
CREATE_PUBVAR(L_VdcFil        , float);
CREATE_PUBVAR(L_IxFil         , float);
CREATE_PUBVAR(L_IyFil         , float);
CREATE_PUBVAR(L_VxFil         , float);
CREATE_PUBVAR(L_VyFil         , float);
CREATE_PUBVAR(L_IMagFil       , float);
CREATE_PUBVAR(L_IGndFil       , float);
CREATE_PUBVAR(L_LineMagFil    , float);
CREATE_PUBVAR(L_DcBusPwr      , float);
CREATE_PUBVAR(L_IdcFil        , float);
CREATE_PUBVAR(L_LineFrqFil    , float);
CREATE_PUBVAR(L_CnvPwrFil     , float);
CREATE_PUBVAR(L_IZeroSeq      , float);
CREATE_PUBVAR(L_VarFil        , float);
CREATE_PUBVAR(L_VarCalc       , float);
CREATE_PUBVAR(L_VarLine       , float);
CREATE_PUBVAR(L_PwrLine       , float);
CREATE_PUBVAR(L_VarLineFil    , float);
CREATE_PUBVAR(L_PwrLineFil    , float);
CREATE_PUBVAR(L_IyHrmFFil     , float);
CREATE_PUBVAR(L_IyHrmFFlGn    , float);

    // Diagnostic variable Calc T2
CREATE_PUBVAR(L_IxFbkFil2     , float);
CREATE_PUBVAR(L_IyFbkFil2     , float);
CREATE_PUBVAR(L_VMagT2Fil     , float);
CREATE_PUBVAR(L_VMagT2Gn      , float);


    // Intermediate signals for calculating filtered diagnostic variables
CREATE_PUBVAR(L_CnvOutPwr     , float);
CREATE_PUBVAR(L_CnvPwrT2      , float);
CREATE_PUBVAR(L_BusPwr        , float);

    // Diagnostic variable filter gains -- used in T2
CREATE_PUBVAR(L_VdcFilGn    , float);
CREATE_PUBVAR(L_IxFilGn     , float);
CREATE_PUBVAR(L_IyFilGn     , float);
CREATE_PUBVAR(L_VxFilGn     , float);
CREATE_PUBVAR(L_VyFilGn     , float);
CREATE_PUBVAR(L_VarFilGn    , float);
CREATE_PUBVAR(L_PwrLineGn   , float);
CREATE_PUBVAR(L_VarLineGn   , float);
CREATE_PUBVAR(L_IMagFilGn   , float);
CREATE_PUBVAR(L_LineMagGn   , float);
CREATE_PUBVAR(L_LineFrqGn   , float);
CREATE_PUBVAR(L_BusPwrGn    , float);
CREATE_PUBVAR(L_DcPwrGn     , float);
CREATE_PUBVAR(L_IdcFilGn    , float);
CREATE_PUBVAR(L_FrqFilGn    , float);
CREATE_PUBVAR(L_CnvPwrGn    , float);


//Vdc High Ix, Iy reduction
CREATE_PUBVAR(L_VdcIxIyFil  , float);
CREATE_PUBVAR(L_VdcIxIyDel  , float);
CREATE_PUBVAR(L_VdcIxIyMod  , float);
CREATE_PUBVAR(L_IyVdc       , float);


CREATE_PUBVAR(L_PwrLineT3   , float );
CREATE_PUBVAR(L_VarLineT3   , float );
CREATE_PUBVAR(L_VarHrmFT3   , float );

// Unpublished Variables
//----------------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Data Passing
//--------------


DATA_PASS(L_PwrLine  ,L_PwrLineT3     , T2_T3, FILTERED);
DATA_PASS(L_VarLine  ,L_VarLineT3     , T2_T3, FILTERED);
DATA_PASS(L_VarHrmF  ,L_VarHrmFT3     , T2_T3, FILTERED);

/*****************************************************************************
*
*       private variables
*
*****************************************************************************/


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Instrumentation of Dc link voltage feedback for regulation purposes,
//      and line current magnitude.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
 void Instrument(void)
{
    /* FIR filter for L_VdcReg*/
    L_VdcFbkZ1  = L_VdcFbkZ0;
    L_VdcFbkZ0  = L_VdcFbk;

    L_VdcReg     = L_VdcFbkZ1*PARM(L_KVdc1) +
                  L_VdcFbkZ0*PARM(L_KVdc0);

    /*************************************************************************/
    /*                          Form L_IMagT1 (task1)                         */
    /**************************************************************************/
    L_IMagT1  = sqrtf(L_IxFbk*L_IxFbk + L_IyFbk*L_IyFbk);


    /*************************************************************************/
    /*              Calculate L_VdcIxIyMod and L_IyVdc                        */
    /**************************************************************************/
    L_VdcIxIyFil = L_VdcIxIyFil + L_VdcIxIyFlGn*(L_VdcReg - L_VdcIxIyFil);
    L_VdcIxIyDel = L_VdcReg - L_VdcIxIyFil;
    if ( (L_VdcIxIyDel - PARM(L_VdcIxIyOff)) < 0.0F)
    {
         L_VdcIxIyMod = 0.0F;
    }
    else
    {
         L_VdcIxIyMod = L_VdcIxIyDel - PARM(L_VdcIxIyOff);
    }
    L_IyVdc = PARM(L_VdcIyGn) * L_VdcIxIyMod;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Calculates instrumentation outputs for task 2.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
void InstrumentT2(void)
{
  // replaced by functionality in bridge i/f:
  //  L_VabOffsFil   += CI_L_WVOffsT2*DelTm2*(L_VabT2 - L_VabOffsFil );
  //  L_VbcOffsFil   += CI_L_WVOffsT2*DelTm2*(L_VbcT2 - L_VbcOffsFil );

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This function calculates the diagnositc variables which are primarily
//      for the purpose of user diagnositcs. They may, however, be used for
//      control puposes if the control can tolerated the varioius filter
//      gains that a user might want to apply. The filter gains used here
//      are caculated by another functions.
//
// EXECUTION LEVEL

void DiagVarCalcT2( void )
{

    float L_VxLineT2;
    // Calculate source inverter output power and DC bus input power

    L_CnvOutPwr  = 1.5F * (( L_VxFbkT2 * L_IxFbkT2 ) + ( L_VyFbkT2 * L_IyFbkT2 ));
    L_CnvPwrT2   = L_CnvOutPwr - 0.0F;

    // Real power into line converter
    if ( Q1ClsIp )
    {
        L_VxLineT2 = GmVxFbkT2;
    }
    else
    {
        L_VxLineT2 = L_VxFbkT2;
    }

    // Calculate harmonic filter current
    if ( KdfClsCmd && L_AcLinePres && Q1ClsIp )
    {
        L_IyHrmFCal =  L_VxLineT2 * L_PllWNom * PARM(CnvLineDfC);
    }
    else
    {
        L_IyHrmFCal = 0.0F;
    }

    //Filtered harmonic filter current
    L_IyHrmFFil  += L_IyHrmFFlGn * ( L_IyHrmFCal - L_IyHrmFFil);


    // Reactive power into harmonic filter
    L_VarHrmF     =  -1.5F*(GmVxFbkT2*L_IyHrmFCal);

    // Real & reactive power out of line reactor
    L_PwrLine  = 1.5F * (GmVxFbkT2 * L_IxFbkT2 + GmVyFbkT2 * L_IyFbkT2);
    L_VarLine  = 1.5F * (GmVyFbkT2 * L_IxFbkT2 - GmVxFbkT2 * L_IyFbkT2);

    L_VMagT2 = sqrtf(L_VxFbkT2*L_VxFbkT2 + L_VyFbkT2*L_VyFbkT2);

    // Reactive power into line converter
    if ( !GmRevSeq)
    {
      L_VarCalc     = -1.5F*(L_VyFbkT2*L_IxFbkT2 - L_VxFbkT2*L_IyFbkT2);
    }
    else
    {
      L_VarCalc     = 1.5F*(L_VyFbkT2*L_IxFbkT2 - L_VxFbkT2*L_IyFbkT2);
    }


    // Filter signals to obtain diagnostic variables
    L_IxFbkFil2  += L_IMagFilGn * ( L_IxFbkT2 - L_IxFbkFil2 );
    L_IyFbkFil2  += L_IMagFilGn * ( L_IyFbkT2 - L_IyFbkFil2 );
    L_IMagFil     = sqrtf(L_IxFbkFil2*L_IxFbkFil2 + L_IyFbkFil2*L_IyFbkFil2);

    L_VdcFil     += L_VdcFilGn * ( L_VdcFbkT2 - L_VdcFil );
    L_IxFil      += L_IxFilGn * ( L_IxFbkT2 - L_IxFil );
    L_IyFil      += L_IyFilGn * ( L_IyFbkT2 - L_IyFil );
    L_VxFil      += L_VxFilGn * ( L_VxFbkT2 - L_VxFil );
    L_VyFil      += L_VyFilGn * ( L_VyFbkT2 - L_VyFil );
    L_VarFil     += L_VarFilGn * ( L_VarCalc - L_VarFil );
    L_PwrLineFil += L_PwrLineGn * ( L_PwrLine - L_PwrLineFil );
    L_VarLineFil += L_VarLineGn * ( L_VarLine - L_VarLineFil );
    L_LineMagFil += L_LineMagGn * (GmVMagT2 - L_LineMagFil);
    L_LineFrqFil += L_LineFrqGn * (GmPllWT2 - L_LineFrqFil);
    L_IZeroSeq    = L_IxyGndFil;

    L_CnvPwrFil += L_CnvPwrGn * (L_CnvOutPwr - L_CnvPwrFil);


    L_VdcRcpT2 =  NoNANLim( 1.0F/L_VdcFbkT2, 1.0e6F, -1.0e6F );

    if ( L_VdcFbkT2  < 50.0F)
    {
         L_IdcT2     = 0.0F;
         L_IdcFil    = 0.0F;
         L_BusPwr    = 0.0F;
         L_DcBusPwr  = 0.0F;
    }
    else
    {
         L_BusPwr    = -L_CnvOutPwr - L_BrgLossPwr;
         L_IdcT2     = L_BusPwr * L_VdcRcpT2;
    }
    L_DcBusPwr   += L_DcPwrGn * ( L_BusPwr - L_DcBusPwr );
    L_IdcFil      += L_IdcFilGn * (L_IdcT2 - L_IdcFil);

    L_VMagT2Fil += L_VMagT2Gn * (L_VMagT2 - L_VMagT2Fil);


    return;
}



