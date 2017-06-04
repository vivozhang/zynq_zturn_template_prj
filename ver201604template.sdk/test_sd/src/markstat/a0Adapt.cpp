///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Parameters on-line adaptation
//
// DESCRIPTION:
//              This file contains functions that perform general checks
//              prior to stator synchronization.
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              a0Adapt.vsd, a0Adapt.xls
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// System
// Core
// Framework
#include "x0FrameWork.h"
// Product
#include "Product.h"
#include "a0Adapt.h"


// Constants
//----------
// Tach Loss detection
#define Tac_A_Msk       0x3
#define Tac_B_Msk       0xC

// Resistance Modulation
#define VF_NOM_NOM     1
#define VF_OVR_OVR     2
#define VF_UDR_UDR     3
#define VF_AR_1        11
#define VF_AR_2        12
#define VF_AR_3        13


// Externals
//----------
extern float          DelTm2;
extern float          DelTm3;
extern float          R_DelTm1;
extern float          R_Ia;
extern float          R_Ib;
extern float          R_Ic;
extern float          R_IxFbkT2;
extern float          R_IyFbkT2;
extern float          R_SpdFbkT3;
extern float          S_FlxXFbkT2;
extern float          S_FlxYFbkT2;
extern float          S_IabcGndFil;
extern float          S_IxFbkT2;
extern float          S_IyFbkT2;
extern float          S_Ls;
extern float          S_LsLmSumRcp;
extern float          S_RRatioRcp;
extern unsigned       S_K1VmgErrPrm;
extern unsigned       R_CTChkEnb;
extern unsigned       R_SyncIOCEnb;
extern unsigned       SimMode;
extern unsigned       R_SeqStat_PackedBits;
extern  CtlBt2        R_MtrCtl1;       // motor control bit coded word 1


// Types and Classes
//------------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------


// Parameters
//-----------
CREATE_PARM(R_WSyncIOC,         float);     // Sync IOC filter omega
CREATE_PARM(S_WCTErr2,          float);     // Stator CT error ^2 filter omega
CREATE_PARM(S_CTErr2Lvl,        float);     // stator CT error ^2 flt threshold (amps ^2)
CREATE_PARM(S_CTIabcLvl,        float);     // Iabc check for stator CT's
CREATE_PARM(R_SyncIOCLvl,       float);     // sync IOC fault level

// Published Variables
//--------------------
CREATE_PUBVAR(R_ImxCalc    ,   float);    // calc'ed X-axis magnetizing current
CREATE_PUBVAR(R_ImyCalc    ,   float);    // calc'ed Y-axis magnetizing current
CREATE_PUBVAR(R_ISynIOCFil ,   float);    // filtered sum of squares current
CREATE_PUBVAR(R_SynIOCFilGn,   float);    // filter gain for sync IOC detection
CREATE_PUBVAR(S_CTErr2     ,   float);    // sum of squares of stator CT error
CREATE_PUBVAR(S_CTErr2Fil  ,   float);    // filtered version of S_CTErr2
CREATE_PUBVAR(S_CTErrFilGn ,   float);    // Filter gain for squared CT error
CREATE_PUBVAR(S_IxCalc     ,   float);    // calc'ed X-axis stator current
CREATE_PUBVAR(S_IyCalc     ,   float);    // calc'ed Y-axis stator current
CREATE_PUBVAR(R_SyncIOCAct, unsigned);    // sync ioc detected
CREATE_PUBVAR(R_TacACh    ,    float);    // Encoder A channel (A or A')
CREATE_PUBVAR(R_TacBCh    ,    float);    // Encoder B channel (B or A')
CREATE_PUBVAR(R_TacAChFil ,    float);    // filtered version of R_TacACh
CREATE_PUBVAR(R_TacBChFil ,    float);    // filtered version of R_TacBCh


// Diagnostics
//------------
CREATE_DIAG(R_SyncIOCFlt);                // Synchronization IOC
CREATE_DIAG(S_CTFlt);                     // Stator CT malfunction
CREATE_DIAG(S_SyncGroundFlt);             // Stator synchronization ground fault

// Unpublished Variables
//----------------------


// Data Pass
//----------


// Functions
//----------

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Check for high rotor currents before the enabling of torque.
//      Such currents likely due to closing sync switch with bad voltage
//      magnitude or phase match.
//
// EXECUTION LEVEL
//      Task1 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void   RotorSyncIOCCheck(void)

{
    float I_Sqr_Sum;

    I_Sqr_Sum = R_Ia * R_Ia + R_Ib * R_Ib + R_Ic * R_Ic;

    R_ISynIOCFil += R_SynIOCFilGn  * ( I_Sqr_Sum - R_ISynIOCFil );

    if (R_SyncIOCEnb && (R_ISynIOCFil > PARM(R_SyncIOCLvl)))
    {
        R_SyncIOCAct = True;
        PUSH_DIAG(R_SyncIOCFlt);
    }
    else
    {
        R_SyncIOCAct = False;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Check the stator CT feedbacks against calculated values.
//
// EXECUTION LEVEL
//      Task2 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  StatorCTCheck(void)
{
     float S_IxErr, S_IyErr;

     // Calculate CT error when stator switch is closed
     // Checking SC or CB
     if ( R_SeqStat.R_SCEnbStat || R_CTChkEnb )
     {
         // calculate magnetizing branch currents from stator flux and rotor currents
         R_ImxCalc = (S_FlxXFbkT2 * PARM(S_RRatio) + S_Ls * R_IxFbkT2) * S_LsLmSumRcp;
         R_ImyCalc = (S_FlxYFbkT2 * PARM(S_RRatio) + S_Ls * R_IyFbkT2) * S_LsLmSumRcp;

         // calculate stator currents from rotor and magnetizing branch currents
         S_IxCalc = ( R_IxFbkT2 - R_ImxCalc ) * S_RRatioRcp;
         S_IyCalc = ( R_IyFbkT2 - R_ImyCalc ) * S_RRatioRcp;

         // calculate x & y axis CT errors
         S_IxErr = S_IxCalc - S_IxFbkT2;
         S_IyErr = S_IyCalc - S_IyFbkT2;

         // calculate magnitude squared of error, and filter
         S_CTErr2 = S_IxErr*S_IxErr + S_IyErr*S_IyErr;

         S_CTErr2Fil += S_CTErrFilGn  * ( S_CTErr2 - S_CTErr2Fil );
     }
     else
     {
        S_CTErr2Fil = 0.0;
        S_IxCalc    = 0.0;
        S_IyCalc    = 0.0;

     }

     // Check for CT fault during start only
     if ( R_CTChkEnb )
     {
     // Different line and stator voltage magnitudes will set S_VltSync to False,
     // indicating that K1 is really open and the CT is not the problem
         if ((S_CTErr2Fil > PARM(S_CTErr2Lvl)) && S_K1VmgErrPrm )
         {
             PUSH_DIAG(S_CTFlt);
         }
		 
		 		 
		 if (S_IabcGndFil > PARM(S_CTIabcLvl) )
         {
             PUSH_DIAG(S_SyncGroundFlt);
         }
		
     }

     return;
}

