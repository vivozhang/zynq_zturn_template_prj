///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Speed feedback and regulation support
//
// DESCRIPTION:
//      Speed feedback functions related to diagnostic detection.
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     p1SpdFbk.vsd, p1SpdFbk.xls
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
#include "b0BridgeRebf.h"
#include "p1Sseq.h"
#include "p1SpdFbk.h"


// Externals
//----------
extern  int           R_SeqSt;         // Sequencer state variable
extern  float         R_SpdFbkT3;
extern  cBridgeRebf   R_BridgeRebf;
extern  FaultIsActiveTyp     FaultIsActive;



// Constants
//----------


// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(R_OvrSpdThr,    float);    // Overspeed threshold (Rpm);
CREATE_PARM(R_UndrSpdThr,   float);    // Under speed threshold (Rpm);
CREATE_PARM(R_RevRotFlt, unsigned);    // Reverse rotation fault enabled
CREATE_PARM(R_ZeroSpdThr,   float);    // Zero Speed Threshold (Rpm);


// Diagnostics
//------------
CREATE_DIAG(R_OverSpeed);               // Sreg: Over Speed trip fault
CREATE_DIAG(R_UnderSpeed);              // Sreg: Under Speed trip fault
CREATE_DIAG(R_RevRotate);               // Reverse rotation alarm


// Variables
//----------
CREATE_PUBVAR(R_ZeroSpdLvl,  float);    // Zero speed threshold (Rpm)
CREATE_PUBVAR(R_OvrSpdLvl,   float);    // Overspeed threshold, (Rpm)
CREATE_PUBVAR(R_SpdFbkT3,    float);    // Speed feedback passed to task3 (Rad/sec)
CREATE_PUBVAR(OverSpeedTrip, unsigned); // Overspeed trip diag is active
CREATE_PUBVAR(UnderSpeedTrip,unsigned); // Underspeed trip diag is active

// Unpublished Variables
//----------------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Data Passing
//--------------
DATA_PASS(R_SpdFbkT2, R_SpdFbkT3 , T2_T3, FILTERED);


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Examines the speed feedback and the speed regulator error to make
//      sure speed control is occuring.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorSpdCtrlFltChk( void )
{
    float SpdFbkT3;

    SpdFbkT3 = R_SpdFbkT3 * RAD_TO_RPM;   // convert Task3 variable to RPM

    // Over speed fault

    if ( SpdFbkT3 > PARM(R_OvrSpdThr) )
    {
        if ( R_BridgeRebf.BrgHdwDataVld )
        {
            PUSH_DIAG( R_OverSpeed );
            FaultIsActive.R_OverSpeed = true;
        }
    }

    OverSpeedTrip =   FAULTISACTIVE(R_OverSpeed);

    // Under speed fault

    if ( (R_SeqSt > ROTOR_CONTACTOR_PICKUP) && (R_SeqSt <= ROTOR_RUNNING)
        && ( SpdFbkT3  < PARM(R_UndrSpdThr) ) )
    {
        PUSH_DIAG( R_UnderSpeed );
        FaultIsActive.R_UnderSpeed = true;
    }

    UnderSpeedTrip =   FAULTISACTIVE(R_UnderSpeed);

    // Reverse rotation fault

    if ( PARM(R_RevRotFlt) == true )
    {
        if ( SpdFbkT3 <= -PARM(R_ZeroSpdThr)*10.0F )
        {
            PUSH_DIAG(R_RevRotate);
        }
        else if ( SpdFbkT3 >= 0.0F )
        {
            POP_DIAG(R_RevRotate);
        }
    }
    else
    {
        POP_DIAG(R_RevRotate);
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Sets the overspeed and zero speed RPM thresholds.
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorSpeedLevel( void )
{
    R_OvrSpdLvl  = PARM(R_OvrSpdThr);
    R_ZeroSpdLvl = PARM(R_ZeroSpdThr);

    return;
}
