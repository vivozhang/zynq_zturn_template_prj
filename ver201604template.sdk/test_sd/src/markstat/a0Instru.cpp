///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rotor side instrumentation and diagnostic
//
// DESCRIPTION:
//      Functions that calculate diagnostic variables and other quantities in
//      support of instrumentation.
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
// core
//framework
#include  "x0FrameWork.h"
// product
#include  "Product.h"
#include  "a0Instru.h"


// Externals
//----------
/* Delayed signals for calculating diagnostic variables */
extern float   S_DscmVxPu;   // Negative Sequence Discrimated Vx per unit
extern float   S_DscmVyPu;   // Negative Sequence Discrimated Vy per unit
extern float R_VxFbkT2 ;
extern float R_IxFbkT2 ;
extern float R_VyFbkT2 ;
extern float R_IyFbkT2 ;
extern float R_BrgLossPwr ;
extern float R_VdcFbkT2 ;
extern float DelTm2Rcp ;
extern float R_TrqCalT2 ;
extern float R_WElecT2 ;
extern float R_FlxCmd ;
extern float R_SpdGen ;
extern float R_SpdFbk ;


extern float GmDscmVMagN ;
extern float GmDscmVxNFl2 ;
extern float GmDscmVyNFl2 ;
extern float R_IFbkCompN ;
extern float R_IxFbkCompN ;
extern float R_IxFbkT3;
extern float R_IxyGndFil;
extern float R_IyFbkCompN ;
extern float R_IyFbkT3;
extern float R_TrqRegStep ;
extern float R_VxFbkT3;
extern float R_VyFbkT3;
extern float S_ICompV ;
extern float S_ICompVSpil ;
extern float S_IxFbkT2 ;
extern float S_IxFbkT3;
extern float S_IyFbkT2 ;
extern float S_IyFbkT3;
extern float S_VMagT2 ;
extern float S_VxFbkT2 ;
extern float S_VyFbkT2 ;
extern unsigned S_NSeqHi ;


// Constants
//----------


// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(S_K1VmagErrTol,  float);    // Voltage match tolerance across K1 during CT checking
CREATE_PARM(R_TrqStpAdr,     float*);
CREATE_PARM(S_K1FiltTm,      float);    // Waiting time for filtered signals to settle
// diagnostic filter w's
CREATE_PARM(R_WDcBusPwr,        float);     // DC bus power diagnostic filter omega
CREATE_PARM(R_WVdcFil,          float);     // Vdc diagnostic variable filter omega
CREATE_PARM(R_WIdcFil,          float);     // DC bus current filter omega
CREATE_PARM(R_WFlxFil,          float);     // Flux diagnostic variable filter omega
CREATE_PARM(R_WFrqFil,          float);     // Electrical freq diagnostic filter omega
CREATE_PARM(R_WIMagFil,         float);     // Current magnitude diagnostic filter omega
CREATE_PARM(R_WIxFil,           float);     // Ix diagnostic variable filter omega
CREATE_PARM(R_WIyFil,           float);     // Iy diagnostic variable filter omega
CREATE_PARM(R_WCnvPwr,          float);     // Rotor converter power diagnostic filter omega
CREATE_PARM(R_WSpdGen,          float);     // Generator speed diagnostic filter omega
CREATE_PARM(R_WSpdFbkPLC,       float);     // Generator speed diagnostic filter omega
CREATE_PARM(R_WTrqFil,          float);     // calculated torque diagnostic filter omega
CREATE_PARM(R_WTrqFbkFil,       float);     // Calculated pu torque diagnostic filter omega
CREATE_PARM(R_WVMagFil,         float);     // Voltage magnitude diagnostic filter omega
CREATE_PARM(R_WVxFil,           float);     // Vx diagnostic variable filter omega
CREATE_PARM(R_WVyFil,           float);     // Vy diagnostic variable filter omega
CREATE_PARM(R_WSpdFbk,          float);     // Speed feedback diagnostic filter omega
CREATE_PARM(R_WQPwrFil,         float);     // Rotor reactive power diag var filter omega
CREATE_PARM(S_WIxFil,           float);     // stator Ix diagnostic variable filter omega
CREATE_PARM(S_WIyFil,           float);     // stator Iy diagnostic variable filter omega
CREATE_PARM(S_WIMagFil,         float);     // stator IMag diagnostic variable filter omega
CREATE_PARM(S_WVxFil,           float);     // stator Vx diagnostic variable filter omega
CREATE_PARM(S_WVyFil,           float);     // stator Vy diagnostic variable filter omega
CREATE_PARM(S_WVMagFil,         float);     // stator VMag diagnostic variable filter omega
CREATE_PARM(S_WPwrFil,          float);     // stator power diag var filter omega
CREATE_PARM(S_WQPwrFil,         float);     // stator reactive power diag var filter omega
//Current reg comps based on neg seq magnitude
CREATE_PARM(S_ICompNsThr,       float);     // Current feedback comp negative sequence threshold
CREATE_PARM(S_ICompStRtr,       float);     // Stator to rotor voltage to current transfer ratio

// Diagnostics
//-----------


// Variables
//----------
// Diagnostic variable filter gains
CREATE_PUBVAR(R_BusPwrGn    ,   float);
CREATE_PUBVAR(R_CnvPwrGn    ,   float);
CREATE_PUBVAR(R_FlxFilGn    ,   float);
CREATE_PUBVAR(R_FrqFilGn    ,   float);
CREATE_PUBVAR(R_IdcFilGn    ,   float);
CREATE_PUBVAR(R_IxFilGn     ,   float);
CREATE_PUBVAR(R_IyFilGn     ,   float);
CREATE_PUBVAR(R_QPwrFilGn   ,   float);
CREATE_PUBVAR(R_SpdFbkGn    ,   float);
CREATE_PUBVAR(R_SpdGenGn    ,   float);
CREATE_PUBVAR(R_TrqFbkFGn   ,   float);
CREATE_PUBVAR(R_TrqFilGn    ,   float);
CREATE_PUBVAR(R_VdcFilGn    ,   float);
CREATE_PUBVAR(R_VxFilGn     ,   float);
CREATE_PUBVAR(R_VyFilGn     ,   float);
CREATE_PUBVAR(S_IMagFilGn   ,   float);
CREATE_PUBVAR(S_IxFilGn     ,   float);
CREATE_PUBVAR(S_IyFilGn     ,   float);
CREATE_PUBVAR(S_PwrFilGn    ,   float);
CREATE_PUBVAR(S_QPwrFilGn   ,   float);
CREATE_PUBVAR(S_VMagFilGn   ,   float);
CREATE_PUBVAR(S_VxFilGn     ,   float);
CREATE_PUBVAR(S_VyFilGn     ,   float);

/* Diagnostic variable filter outputs */
CREATE_PUBVAR(S_IxFil       ,   float);
CREATE_PUBVAR(S_IyFil       ,   float);
CREATE_PUBVAR(S_IMag        ,   float);
CREATE_PUBVAR(S_IMagFil     ,   float);
CREATE_PUBVAR(S_VxFil       ,   float);
CREATE_PUBVAR(S_VyFil       ,   float);
CREATE_PUBVAR(S_VMagFil     ,   float);
CREATE_PUBVAR(S_Pwr         ,   float);
CREATE_PUBVAR(S_QPwr        ,   float);
CREATE_PUBVAR(S_PwrFil      ,   float);
CREATE_PUBVAR(S_QPwrFil     ,   float);
CREATE_PUBVAR(S_PwrFactFil  ,   float);
CREATE_PUBVAR(R_CnvPwrFil   ,   float);
CREATE_PUBVAR(R_VdcFil      ,   float);
CREATE_PUBVAR(R_IdcFil      ,   float);
CREATE_PUBVAR(R_IxFil       ,   float);
CREATE_PUBVAR(R_IyFil       ,   float);
CREATE_PUBVAR(R_VxFil       ,   float);
CREATE_PUBVAR(R_VyFil       ,   float);
CREATE_PUBVAR(R_TrqFil      ,   float);
CREATE_PUBVAR(R_TrqFbk      ,   float);
CREATE_PUBVAR(R_TrqFbkFil   ,   float);
CREATE_PUBVAR(R_FlxFil      ,   float);
CREATE_PUBVAR(R_SpdGenFil   ,   float);
CREATE_PUBVAR(R_SpdFbkFil   ,   float);
CREATE_PUBVAR(R_DcBusPwr    ,   float);
CREATE_PUBVAR(R_ElecFrqFil  ,   float);
CREATE_PUBVAR(R_QPwr        ,   float);
CREATE_PUBVAR(R_QPwrFil     ,   float);
CREATE_PUBVAR(R_PwrFactFil  ,   float);


/* Intermediate signals for calculating filtered diagnostic variables */

CREATE_PUBVAR(R_CnvOutPwr   ,   float);
CREATE_PUBVAR(R_MtrLossPwr  ,   float);
CREATE_PUBVAR(R_BusPwrT2    ,   float);
CREATE_PUBVAR(R_IdcT2       ,   float);
CREATE_PUBVAR(R_ElecFrq     ,   float);

/* Delayed signals for calculating diagnostic variables */
CREATE_PUBVAR(R_VdcT2Prv    ,   float);

// Diagnostic variable filter gains
CREATE_PUBVAR(R_IMagFilGn   ,   float);
CREATE_PUBVAR(R_VMagFilGn   ,   float);
CREATE_PUBVAR(R_IMag        ,   float);
CREATE_PUBVAR(R_VMag        ,   float);
CREATE_PUBVAR(R_VMagT2      ,   float);
CREATE_PUBVAR(R_IMagFil     ,   float);
CREATE_PUBVAR(R_VMagFil     ,   float);
CREATE_PUBVAR(R_IZeroSeq    ,   float);
CREATE_PUBVAR(S_K1OpTotDly    ,   float);
CREATE_PUBVAR(R_ImagSqrT1,  float);

// Unpublished Variables
//----------------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Data Passing
//-------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculates rotor related diagnostic values and compensation applied
//      to feedbacks.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorDiagVarCalcT2( void )
{
    float Tmp;
    // Calculate motor shaft output power and DC bus input power

    R_CnvOutPwr  = 1.5 * (( R_VxFbkT2 * R_IxFbkT2 ) + ( R_VyFbkT2 * R_IyFbkT2 ));
    R_MtrLossPwr = 0;


    R_QPwr       = 1.5 * (( R_VyFbkT2 * R_IxFbkT2 ) - ( R_VxFbkT2 * R_IyFbkT2 ));    // GRC (CR&D) wanted this as part
    R_QPwrFil   += R_QPwrFilGn * ( R_QPwr - R_QPwrFil );                             // of VEM capability test data

    // Calculate DC bus current

    if ( R_VdcFbkT2  < 50.0)
    {
         R_IdcT2    = 0.0;
         R_IdcFil   = 0.0;
         R_BusPwrT2 = 0.0;
         R_DcBusPwr = 0.0;
    }
    else
    {
        R_BusPwrT2  = R_CnvOutPwr + R_BrgLossPwr;
        R_IdcT2     = ( R_BusPwrT2 / R_VdcFbkT2 )
                      + PARM(L_CBrg) * ( R_VdcFbkT2 - R_VdcT2Prv ) * DelTm2Rcp;
    }

    R_VdcT2Prv = R_VdcFbkT2;


    // Calculate torque in per-unit.  Flip sign to match reference.

    R_TrqFbk = -R_TrqCalT2;

    // Calculate electrical frequency in hertz

    R_ElecFrq = R_WElecT2 * RAD_TO_HZ;


    // Filter signals to obtain diagnostic variables

    R_VdcFil      += R_VdcFilGn  * ( R_VdcFbkT2 - R_VdcFil );
    R_IdcFil      += R_IdcFilGn  * ( R_IdcT2 - R_IdcFil );
    R_IxFil       += R_IxFilGn   * ( R_IxFbkT2 - R_IxFil );
    R_IyFil       += R_IyFilGn   * ( R_IyFbkT2 - R_IyFil );
    R_VxFil       += R_VxFilGn   * ( R_VxFbkT2 - R_VxFil );
    R_VyFil       += R_VyFilGn   * ( R_VyFbkT2 - R_VyFil );
    R_TrqFil      += R_TrqFilGn  * ( R_TrqCalT2 - R_TrqFil );
    R_TrqFbkFil   += R_TrqFbkFGn * ( R_TrqFbk - R_TrqFbkFil );
    R_FlxFil      += R_FlxFilGn  * ( R_FlxCmd - R_FlxFil );
    R_SpdGenFil   += R_SpdGenGn  * ( R_SpdGen - R_SpdGenFil );
    R_SpdFbkFil   += R_SpdFbkGn  * ( R_SpdFbk - R_SpdFbkFil );
    R_CnvPwrFil   += R_CnvPwrGn  * ( R_CnvOutPwr - R_CnvPwrFil );
    R_DcBusPwr    += R_BusPwrGn  * ( R_BusPwrT2 - R_DcBusPwr );
    R_ElecFrqFil  += R_FrqFilGn  * ( R_ElecFrq - R_ElecFrqFil );

    // stator vars:
    S_IxFil     += S_IxFilGn   * ( S_IxFbkT2 - S_IxFil );
    S_IyFil     += S_IyFilGn   * ( S_IyFbkT2 - S_IyFil );
    S_VxFil     += S_VxFilGn   * ( S_VxFbkT2 - S_VxFil );
    S_VyFil     += S_VyFilGn   * ( S_VyFbkT2 - S_VyFil );
    S_VMagFil   += S_VMagFilGn * ( S_VMagT2  - S_VMagFil );

    // calc stator power factor
    S_Pwr  = (S_VxFbkT2 * S_IxFbkT2 + S_VyFbkT2 * S_IyFbkT2) * 1.5;
    S_QPwr = (S_VyFbkT2 * S_IxFbkT2 - S_VxFbkT2 * S_IyFbkT2) * 1.5;

    S_PwrFil  += S_PwrFilGn  * ( S_Pwr  - S_PwrFil );
    S_QPwrFil += S_QPwrFilGn * ( S_QPwr - S_QPwrFil );

 // R_TrqRegStep = *(float*)R_TrqStepAdr;                     // like this in MACC
    R_TrqRegStep = gVariable.ReadAsFloat(PARM(R_TrqStpAdr));


    Tmp = NoNANLim(S_PwrFil/sqrtf(S_PwrFil*S_PwrFil + S_QPwrFil*S_QPwrFil), 1.0e6F, -1.0e6F);
    if(Tmp >  1.0 )
    {
       Tmp =  1.0;
    }
    else if (Tmp < -1.0 )
    {
       Tmp = -1.0;
    }

    S_PwrFactFil = Tmp;

    Tmp = NoNANLim(R_CnvPwrFil/sqrt(R_CnvPwrFil*R_CnvPwrFil + R_QPwrFil*R_QPwrFil), 1.0e6F, -1.0e6F);
    if(Tmp >  1.0 )
    {
       Tmp =  1.0;
    }
    else if (Tmp < -1.0 )
    {
       Tmp = -1.0;
    }

    R_PwrFactFil = Tmp;


    //////////////////////////////////////
    // Calculate Ix, Iy feedback comps
    //////////////////////////////////////
     S_ICompVSpil = GmDscmVMagN - PARM(S_ICompNsThr);
     if ( S_ICompVSpil < 0.0)
     {
         S_ICompV     = 0.0;
         R_IFbkCompN  = 0.0;
         S_DscmVxPu   = 0.0;
         S_DscmVyPu   = 0.0;
         S_NSeqHi     = false;
     }
     else
     {
         S_ICompV     = S_ICompVSpil;
         R_IFbkCompN  = S_ICompV*PARM(S_ICompStRtr);
         S_DscmVxPu   = GmDscmVxNFl2/GmDscmVMagN;
         S_DscmVyPu   = GmDscmVyNFl2/GmDscmVMagN;
         S_NSeqHi     = true;
     }


     R_IxFbkCompN = S_DscmVyPu*R_IFbkCompN;
     R_IyFbkCompN = -S_DscmVxPu*R_IFbkCompN;

    //////////////////////////////////////
    // END Calculate Ix, Iy feedback comps
    //////////////////////////////////////

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculates rotor related AC magnitudes used in diagnostics and
//      sequence supporting quantities.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorDiagVarCalcT3( void )
{

    // ========== Current Magnitude ===========
    R_IMag = sqrtf(R_IxFbkT3 * R_IxFbkT3 + R_IyFbkT3 * R_IyFbkT3);

    // ========== Voltage Magnitude ===========
    R_VMag = sqrtf(R_VxFbkT3 * R_VxFbkT3 + R_VyFbkT3 * R_VyFbkT3);


    /* Filter signals to obtain diagnostic variables */
    R_IMagFil += R_IMagFilGn * ( R_IMag - R_IMagFil );
    R_VMagFil += R_VMagFilGn * ( R_VMag - R_VMagFil );

    // Copy signals into diagnostic variables

    R_IZeroSeq = R_IxyGndFil;

    // ========== Stator I Magnitude ===========
    S_IMag = sqrtf(S_IxFbkT3 * S_IxFbkT3 + S_IyFbkT3 * S_IyFbkT3);

    S_IMagFil += S_IMagFilGn * ( S_IMag - S_IMagFil );

    // Total time delay while opening Sync Contactor
    // dly on the cmd + dly of the physical device + extra waitign time for task3 variable
    S_K1OpTotDly = PARM(S_K1DropOutDlyTm)+ PARM(S_K1OpnDlyTm) + PARM(S_K1FiltTm);

    return;
}
