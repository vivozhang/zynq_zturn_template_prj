///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rotor Product Level Task Drivers
//
// DESCRIPTION:
//      This file provides the top level product code task drivers for
//      that are called by Xops.
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
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "b0CelTst.h"
#include "b0PlsTst.h"
#include "b0PwmMod2Async.h"
#include "c1BrgCurShrReg.h"
#include "c1BrgCroBarCtl.h"
#include "c1LegTestLowFrq.h"



// Constants
//----------
#define NUM_OF_FIR_EVT_PWM (1+1)    // init + 1 trans


// Externals
//----------
extern float                R_AdjTm1Rcp;
extern float                R_DelTm1Rcp;
extern float                S_Ix;
extern float                S_IxFbk;
extern float                S_Iy;
extern float                S_IyFbk;
extern float                S_Vx;
extern float                S_VxFbk;
extern float                S_Vy;
extern float                S_VyFbk;
extern float                R_UxCmd;
extern float                R_UyCmd ;
extern float                R_AngElecFbk;
extern float                R_AngElecCmd;
extern unsigned             InitDone;
extern unsigned             R_CtlAlgAct;
extern unsigned             Task1Pass;
extern unsigned             R_BrgPwrEnb;
extern unsigned             R_DualBrgEnb;
extern unsigned             R_AutoCTAct;
extern int                  T1AFramesPerT2;

extern R_SeqActBits         R_SeqAct;
extern unsigned             R_SeqStat_PackedBits;
extern unsigned             R_SeqReq_PackedBits;
extern unsigned             R_SeqStBitsT3_PackedBits;
extern unsigned             R_SeqStBits_PackedBits;
extern unsigned             R_SeqReqT1_PackedBits;

extern cRotorPulseTest      R_RotorPulseTest;
extern cCellTest            R_CellTest;
extern cPwmMod2Async        R_PwmMod2Async;
#ifdef LEG_TEST
extern cLegTestLowFrq       Rx_LegTestLowFrq;
#endif
extern cBrgCurShrReg        R_BrgCurShrReg;

extern unsigned             CroBarVdcEnbFlg;
extern cBrgCroBarCtl        R_BrgCroBarCtl;

// Local Prototypes (to resolve forward references)
//-------------------------------------------------
// task1
extern void CloseSfbVcoCapture();
extern void RotorDemodulateStator();
extern void RotorMtrCtlTuneT1();
extern void RotorPrecondition_T1();
extern void RotorProcessBrgT1();
extern void RotorProcessFbks();
extern void RotorProcessMtrCtrlT1();
extern void RotorProcessSfb1();
extern void StatorProcessFbks();
extern void RotorRotateUpDn();
extern void RotorDemodVoltage();
extern void RotorDemodCurrent();

extern PhsTransListTyp  PhsTransDisabList;
// task2
extern void DriveTrainDamper();
extern void RotorFluxAdjustSelect();
extern void RotorMotorCtrlSequencer();
extern void RotorProcessFbks2();
extern void RotorProcessMtrCtrlT2();
extern void RotorProcessSfb2();
extern void RotorS_LineMonitorT2();
extern void RotorTorqueLimitSelect();
extern void StatorProcessFbks2();

// task3
extern void RotorLineMonitorT3();
extern void RotorProcessBrgT3();
extern void RotorProcessMtrCtrlT3();
extern void RotorProcessSfb3();
extern void RotorSpdCtrlFltChk();

// taskB
extern void ParamPassHandShake();
extern void RotorProcessBrgBgnd();
extern void RotorProcessMtrCtrlBgnd();
extern void RotorProcessSfbInit();
extern void RotorSpeedLevel();
extern void StatorFbksBgnd();


// Types and Classes
//------------------


// Parameters
//-----------


// Variables
//----------
CREATE_PUBVAR(R_AdjTm1Rcp,        float);      // Adjusted Task1 Time Recip  - Task 1
CREATE_PUBVAR(R_Test1Float,       float);      // global rotor converter float test variable 1
CREATE_PUBVAR(R_Test2Float,       float);      // global rotor converter float test variable 2
CREATE_PUBVAR(R_UcmmCmd,          float) = 0.0;// Normalized rotor common mode command for modulator (L-L scaling)
CREATE_PUBVAR(R_Task1Pass,        int);        //
CREATE_PUBVAR(R_Test1Int,         int);        // global rotor converter integer test variable 1
CREATE_PUBVAR(R_Test2Int,         int);        // global rotor converter integer test variable 2
CREATE_PUBVAR(TaskBPrio,          int);        //
CREATE_PUBVAR(TB_Mtr_Sw_Req,      int);        //
CREATE_PUBVAR(R_PwmUp,            unsigned);
CREATE_PUBVAR(R_BrgEnbLegTstLfT2, unsigned);
CREATE_PUBVAR(R_AutoCTActT1,      unsigned);
CREATE_PUBVAR(R_PulseTestReq,     unsigned);

// Unpublished Variables
//----------------------
PhsTransListTyp  R_PhsTransListBrg1  = { 0, NULL, NULL };    // bridge 1 transition list
PhsTransListTyp  R_PhsTransListChop  = { 0, NULL, NULL };    // bridge 3 (Dc-chopper) transition list
PhsTransListTyp  R_PhsDtransListBrg1 = { 0, NULL, NULL };    // bridge 1 phase D transition list (not used)

RRegLimFlgs             R_InnrRegLim;       // inner regulator max/min limit hit

PhsTransTyp      R_AutoCtPhsTransArr[BRGTST_NUM_OF_TRA_PER_PHS][NUM_OF_PHASES];
PhsTransTyp      R2_AutoCtPhsTransArr[BRGTST_NUM_OF_TRA_PER_PHS][NUM_OF_PHASES];

PhsTransListTyp  R_AutoCtPhsTransList;
PhsTransListTyp  R2_AutoCtPhsTransList;


// Data Passing
//--------------
DATA_PASS( Rx_LegTestLowFrq.BrgEnbLegTstLf, R_BrgEnbLegTstLfT2, T1A_T2, UNFILTERED );
DATA_PASS( R_AutoCTAct,                     R_AutoCTActT1,      T2_T1,  UNFILTERED );


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side task 1 execution functions
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorTask1(void)
{

    // Task 1 counter
    ++R_Task1Pass;

    //-------------------------------------------------------------------------
    // Feedback sampling and calculations
    //  Feedback processing includes DC link overvoltage protection.
    //  For this reason it must occur as soon as possible in Task1.
    RotorProcessFbks();
    StatorProcessFbks();

    //-------------------------------------------------------------------------
    // Read hardware register from AEAA */
    // NOTE:  This line must be called BEFORE ProcessAeaaVcos() or ProcessSfb1()
    RotorProcessSfb1();


    //-------------------------------------------------------------------------
    // control the phase of the pwm modulator
    if ( (R_PwmMod2Async.ModPwmN == 0.5) && (R_Task1Pass & 1) )
    {
        // if (single chop AND odd task1)
        R_PwmUp  = false;
    }
    else
    {
        // else (multiple chops OR even task1)
        R_PwmUp  = true;
    }

    //-------------------------------------------------------------------------
    // Diagnostic Mode
    if ( R_SeqStBits.R_DiagMode )
    {
        RotorPrecondition_T1();
        RotorDemodulateStator();
        S_IxFbk  =  S_Ix;
        S_IyFbk  =  S_Iy;
        S_VxFbk  =  S_Vx;
        S_VyFbk  =  S_Vy;

        R_RotorPulseTest.Task1();
        if ( R_SeqReqT1.R_PlsTstReq )
        {
            R_PhsTransListBrg1 = R_RotorPulseTest.PhsTransList;
        }

        R_CellTest.Task1();

        if ( R_SeqReqT1.R_CelTstReq )
        {
           R_PhsTransListBrg1 = R_CellTest.PhsTransList;
        }

        R_CtlAlgAct = false;
    }

    //-------------------------------------------------------------------------
    // Motor Control Tune Mode
    else if ( R_SeqStBits.R_MtrCtlTune )
    {
        RotorMtrCtlTuneT1();
        R_CtlAlgAct = false;
    }

    //-------------------------------------------------------------------------
    // Control Algorithm
    else
    {
        #ifdef LEG_TEST
        // Low frequency leg test
        if ( PARM(TestMode) == LF_LEG_TEST )
        {
            R_AngElecFbk = Rx_LegTestLowFrq.AngElecLegTstLf;
            RotorDemodVoltage();
            RotorDemodCurrent();
            Rx_LegTestLowFrq.Task1();
            R_UxCmd      = Rx_LegTestLowFrq.UxOutLegTstLf;
            R_UyCmd      = Rx_LegTestLowFrq.UyOutLegTstLf;
            R_UcmmCmd    = Rx_LegTestLowFrq.UcmOutLegTstLf;
            R_AngElecCmd = Rx_LegTestLowFrq.AngElecLegTstLf;
            RotorRotateUpDn();
        }
        // Motor Model
        else
        {
        #endif

            R_CtlAlgAct = true;
            RotorProcessMtrCtrlT1();
            R_UcmmCmd   = 0.0F;
        #ifdef LEG_TEST
        }
        #endif

        // Current sharing regulator
        //R_BrgCurShrReg.Task1();

        // Modulator & Phase Transition Lists
        R_PwmMod2Async.Task1();

        if ( CroBarVdcEnbFlg && !R_BrgCroBarCtl.CroBarAct )
        {
            R_PhsTransListBrg1 = PhsTransDisabList;
        }
        else
        {
            R_PhsTransListBrg1 = R_PwmMod2Async.PhsTransList;
        }

    }

    //-------------------------------------------------------------------------
    // Output data to bridge
    RotorProcessBrgT1();

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side task 2 execution functions
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorTask2(void)
{
    RotorProcessFbks2();             // Feedback calculations for bridge

    StatorProcessFbks2();            // Stator feedbacks for DFIG control

    RotorProcessSfb2();              // Speed feedback for speed regulator

    RotorMotorCtrlSequencer();       // Rotor control sequencer

    if ( InitDone)
    {
        RotorS_LineMonitorT2();
    }

    // SpeedRegulator();             // Speed Regulator function

    //ZJ: removed since it is empty function
    //RotorTorqueLimitSelect();        // Select torque command limits used in Task 1

    RotorFluxAdjustSelect();         // Select flux   adjust  signal used in Task 1

    DriveTrainDamper();              // Drive Train Damper

    RotorProcessMtrCtrlT2();         // Process Motor control Task 2 functions


    //-------------------------------------------------------------------------
    // bridge power enable logic

    if ( R_SeqStBits.R_DiagMode == true )
    {
        if ( R_SeqReq.R_PlsTstReq )
        {
            //R_BrgPwrEnb = R_RotorPulseTest.PlsTstBrgPwrEnb || R2_RotorPulseTest.PlsTstBrgPwrEnb;
            R_BrgPwrEnb = R_RotorPulseTest.PlsTstBrgPwrEnb;

        }
        else if ( R_SeqReq.R_CelTstReq  )
        {
            R_BrgPwrEnb = R_CellTest.CelTstBrgPwrEnb;
        }
        else
        {
            R_BrgPwrEnb = false;
        }
    }
    else if ( PARM(TestMode) == PUMP_BACK )
    {

    }
    #ifdef LEG_TEST
    else if ( (PARM(TestMode) == LF_LEG_TEST) && (PARM(ConfigForSep) == false) )  // Leg test only supported for full-rated setup
    {
        R_BrgPwrEnb = R_BrgEnbLegTstLfT2;
    }
    #endif
    else if ( PARM(TestMode) == HARDWARE_TEST )
    {
        R_BrgPwrEnb = false;
    }
    else if ( PARM(TestMode) == TEST_DISABLED )
    {

    }
    else
    {
        R_BrgPwrEnb = false;
        PUSH_DIAG( R_BrgPwrEnbUnhandTrp );
    }

    R_PulseTestReq  = R_SeqReq.R_PlsTstReq;
    R_SeqStat.R_PlsTstStat = R_RotorPulseTest.PlsTstStat;
    R_SeqAct.R_PlsTstAct  = R_RotorPulseTest.PlsTstAct;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side task 3 execution functions
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorTask3()

{

    // Application Layer Functions

    //RotorCoreDriveSequencerT3();

    RotorProcessSfb3();              // Updates R_SpdFbkAvg

    RotorSpdCtrlFltChk();            // Check for speed control faults

   // GenericCircuitFlt();        // Normally closed and open circuit check

    RotorProcessBrgT3();             // Process Bridge functions

    RotorLineMonitorT3();            // Stator line monitor


    RotorProcessMtrCtrlT3();         // Process Motor control functions


   // SoftResetCheck();           // Checks for remote soft reset command

   // ConfigCaptureBuffer();      // Configure capture buffer from EE's

    if (R_SeqStBitsT3.R_DiagMode == false )
    {
        //R_RotorPulseTest.CalcDelayPulse();  // Update information for Pulse-Test diagram
        //R2_RotorPulseTest.CalcDelayPulse();
    }

    #ifdef LEG_TEST
    if ( PARM(TestMode) == LF_LEG_TEST )
    {
        Rx_LegTestLowFrq.Task3();
    }
    #endif


    //R_BrgCurShrReg.Task3();       // Current sharing regulator alarm

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side background execution functions
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorTaskBgnd(void)
{

    RotorSpeedLevel();           // Set zero speed and over speed thresholds
    RotorProcessMtrCtrlBgnd();   // Process Motor control background functions
    RotorProcessBrgBgnd();       // Process Bridge background functions
    StatorFbksBgnd();            // Stator fbk background processing
    ParamPassHandShake();        // Drive Train Param Handshake

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side initialization functions
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void Task1_Init_B(void)
{

    R_AdjTm1Rcp = R_DelTm1Rcp;

    RotorProcessSfbInit();


    // Configure auto cell test phase transition list as cell test
    R_AutoCtPhsTransList.Len        = R_CellTest.PhsTransList.Len;
    R_AutoCtPhsTransList.pBrgTstCtl = R_CellTest.PhsTransList.pBrgTstCtl;

    // Set pointer to gate commands to special purpose array
    R_AutoCtPhsTransList.pTrans     = &R_AutoCtPhsTransArr[0];

    // Initialize all gate commands to disabled / off
    for ( unsigned phs = 0 ; phs < NUM_OF_PHASES ; phs++ )
    {
        for ( unsigned tra = 0 ; tra < BRGTST_NUM_OF_TRA_PER_PHS ; tra++ )
        {
            R_AutoCtPhsTransArr[tra][phs].DelTm  = 0;
            R_AutoCtPhsTransArr[tra][phs].State  = PHS_ST_OFF;
            R_AutoCtPhsTransArr[tra][phs].Enable = false;
        }
    }

 }
