///////////////////////////////////////////////////////////////////////////////
//
// TITLE        <class title>
//
// DESCRIPTION
//              This file contains externs and prototypes for X0STATUS.C30
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////

#ifndef X0_STATUS_H
#define X0_STATUS_H

// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product


// Constants
//----------


// Function Prototypes
//--------------------


// Types and Classes
//------------------

struct  CtrlStatusTyp           // word #0 - Control Status Bits
{
    unsigned    Brg1Ready       :1;     //  0 Bridge 1 Ready to run
    unsigned    Brg1Running     :1;     //  1 Bridge 1 Running
    unsigned    Brg1TripFault   :1;     //  2 Bridge 1 Trip fault present
    unsigned    Brg1WarnFault   :1;     //  3 Bridge 1 Warning fault present
    unsigned    Brg2Ready       :1;     //  4 Bridge 2 Ready
    unsigned    OverridePres    :1;     //  5 Override present
    unsigned    LanOK           :1;     //  6 Lan communication ok
    unsigned    DiagMsgPnd      :1;     //  7 Diagnositic msg. pending
    unsigned    ParmNotSav      :1;     //  8 Parameter(s) NOT Saved to NonVolatile memory
    unsigned    DiagMsgQue      :1;     //  9 Diagnostic msg queue present
    unsigned    Brg2Running     :1;     // 10 Bridge 2 Running
    unsigned    Brg1NotStopped  :1;     // 11 Bridge 1 not stopped
    unsigned    CapStatus       :1;     // 12 Capture buffer triggered and completed capturing
    unsigned    Brg2TripFault   :1;     // 13 Bridge 2 Trip fault present
    unsigned    Brg2WarnFault   :1;     // 14 Bridge 2 Warning fault present
    unsigned    Brg2NotStopped  :1;     // 15 Bridge 2 not stopped

                                //Begin Prod Specific

    unsigned    Standby         :1;     // 16 Standby Mode
    unsigned    ZeroSpeed       :1;     // 17 Zero speed
    unsigned    DiagModeBrg2    :1;     // 18 Bridge 2 Diagnostic mode
    unsigned    TuneModeBrg2    :1;     // 19 Bridge 2 Auto-tune mode
    unsigned    DiagModeBrg1    :1;     // 20 Bridge 1 Diagnostci mode
    unsigned    Prd_Spare_21    :1;     // 21 Product Spare
    unsigned    Prd_Spare_22    :1;     // 22 Product spare
    unsigned    Prd_Spare_23    :1;     // 23 Product spare
    unsigned    Prd_Spare_24    :1;     // 24 Product spare
    unsigned    Prd_Spare_25    :1;     // 25 Product spare
    unsigned    Prd_Spare_26    :1;     // 26 Product spare
    unsigned    Prd_Spare_27    :1;     // 27 Product spare
    unsigned    Prd_Spare_28    :1;     // 28 Product spare
    unsigned    Prd_Spare_29    :1;     // 29 Product spare
    unsigned    Prd_Spare_30    :1;     // 30 Product spare
    unsigned    Prd_Spare_31    :1;     // 31 Product spare
} ;


#endif  // X0_STATUS_H
