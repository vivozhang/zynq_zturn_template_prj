///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Control/Simulation Memory Map
//
// DESCRIPTION:
//      Provides the data structure definitions that are used to communicate
//      between simulation (isim and xsim) and the control
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __S0_SIM_INC__
#define __S0_SIM_INC__

// Constants
//----------
#define DiagNumFrmCtrl 80
#define DiagNumToCtrl  30
#define TRIPLOGNUM     5

// Types and Classes
//------------------


//-----------------------------------------------------------------------------
// set alignment to 1 byte boundary to get same memory layout on simulator
//-----------------------------------------------------------------------------
#pragma pack(push)      // push current alignment to stack
#pragma pack(1)


//Out Relay control interface
struct XsimOutRlyType
{
    unsigned    L_Q1ClsPulse;   // line side Q1 breaker close pulse
    unsigned    L_KDFClsCmd;    // Line side DF Contactor String close command
    unsigned    R_OVClsCmd;     // Rotor over voltage close command
    unsigned    S_K1ClsCmd;     // Syn Contactor Close Command
};

struct XsimDoeSpType
{
    float   R_TrqRef;           // Torque Reference From Xsim
    float   R_VARRef;           // Var Reference From Xsim
    float   R_PhiAdjust;        // Phi Reference From Xsim
    float   L_VARRef;           // Var Reference From Xsim @ RPNW
    float   R_TachMeterSpd;     // Shaft Speed
    float   R_TachMeterPosn;    // Shaft Position
    float   VoltVarEnb;         // Var/Phi Mode selection
};

// data transmitted from control to simulator
struct XsimDataFromCtlTyp
{
    unsigned                        Status;                  // status signals
    float                           Diag[DiagNumFrmCtrl];    // diagnostic signals
    unsigned                        T1StFromCtl;             //Synchronous Flag from Controller
    unsigned                        RxPacketCtr;             //Receive package count
    unsigned                        TripFlt;                 //Trip Flt
    unsigned                        XsimTripLog[TRIPLOGNUM]; //Trip Log for Debugging Purpose

    XsimOutRlyType                  XsimOutRly;              //Xsim Out Reley control interface

    BrgCmdTraSimTyp                 L_BrgCmdTraSimZ1;   // line bridge tranistion commands (1 task1 delayed)
    BrgCmdTraSimTyp                 R_BrgCmdTraSimZ1;   // rotor bridge transition commands (1 task1 delayed)
    BrgDbCmdSimTyp                  L_BrgDbCmdSimZ1;

    // Brg and DB temperature mapping to simulator
    cBrgThrm5pModel::BtmTempSimTyp      L_BtmTempSim;
    cBrgThrm5pModel::BtmTempSimTyp      R_BtmTempSim;
    cDbThrmModel::DtmTempSimTyp         DtmTempSim;
    float                           DbTmpEst;
};

struct DsimDiagFromCtlTyp
{
    float              Diag[80];           // diagnostic signals
};

// data transmitted from simulator to control
struct XsimDataToCtlTyp
{
    unsigned                        Status;             // status signals
    float                           Diag[DiagNumToCtrl];// diagnostic signals
    float                           SimRealTm;          //Real time from Xsim Simulation
    float                           XsimDoeCmd;         //Xsim Doe Commmand
    XsimDoeSpType                   XsimDoeSp;          //Xsim Doe Setpoints
    BrgFbkSimTyp                    L_BrgFbkSim;        // line bridge feedbacks in physical units
    BrgFbkSimTyp                    R_BrgFbkSim;        // rotor bridge feedbacks in physical units
};

struct DsimDiagToCtlTyp
{
    float              Diag[30];           // diagnostic signals
};


#pragma pack(pop)       // restore original alignment from stack
//-----------------------------------------------------------------------------


#endif  // __S0_SIM_INC__
