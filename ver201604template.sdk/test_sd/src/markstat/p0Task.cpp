///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Product Level Task Drivers
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
// core
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "b0BrgThrm5p.h"
#include "b0BrgThrmstr.h"
#include "b0CelTst.h"
#include "b0PlsTst.h"
#include "b0BridgeRebf.h"
#include "p0Thrmstr.h"
#include "p0Sseq.h"
#include "b0DbPlsTst.h"
#include "b0DbCelTst.h"
#include "c1BrgCroBarCtl.h"
#include "p0EsaBuf.h"


// Constants
//----------


// Externals
//----------
extern cBrgCroBarCtl    R_BrgCroBarCtl;

extern float            L_DelTm1;
extern float            R_DelTm1;
extern float            R_TrqCalT3;
extern float            R_TrqCmdInT3;
extern float            TrqCmdInTrT3;
extern float            R_TrqRatRcp;
extern float            S_IMag;
extern float            S_AmpsRatRcp;
extern unsigned         EventTmSec;
extern unsigned         EventTmSecNv;

extern int              BkGdPassCntr;
extern int              L_SeqSt;
extern int              R_SeqSt;
extern int              R_SyncTskSt;

extern unsigned         Brg1TripFlt;
extern unsigned         Brg2TripFlt;
extern unsigned         InitDone;
extern unsigned         L_MaStatus;
extern unsigned         S_K1Status;
extern unsigned         L_AutoCTAct;
extern unsigned         L_OTTrpDlySm;
extern unsigned         L_RegLimSum;
extern unsigned         LhslaOtDlyTrpFlg;
extern unsigned         RhslaOtDlyTrpFlg;
extern unsigned         R_AutoCTAct;
extern unsigned         R_CCOpnCmd;
extern unsigned         R_OTTrpDlySm;
extern unsigned         SimMode;
extern unsigned 		DsimMode;
extern unsigned         TBDoneOnce;
extern unsigned         TherBadDlySm;
extern unsigned         ThermisDlySm;
extern unsigned         TempIsValid;
extern unsigned         HtrFanRunOk;

extern unsigned         AirInThrmOpnFlg;
extern unsigned         LqdThrmOpnFlg;
extern unsigned         CfcThrmOpnFlg;
extern unsigned         PdcThrmOpnFlg;
extern unsigned         AirInThrmShrtFlg;
extern unsigned         LqdThrmShrtFlg;
extern unsigned         PdcThrmShrtFlg;
extern unsigned         CfcThrmShrtFlg;
extern unsigned         L_Running;

extern volatile int     T1St;
extern volatile int     T1ASt;
extern volatile int     T2St;
extern int              NvUpdateSt;

extern SysTm_Typ        SysTm3;
extern DiagBits_Typ     DiagBits;
extern unsigned         R_SeqStBits_PackedBits;
extern SeqStatBits      L_SeqStat;
extern SeqActBits       L_SeqAct;
extern R_SeqActBits     R_SeqAct;
extern unsigned         R_SeqStat_PackedBits;

extern cLinePulseTest   L_LinePulseTest;
extern cCellTest        L_CellTest;

extern cRotorPulseTest  R_RotorPulseTest;
extern cCellTest        R_CellTest;

extern cBridgeRebf      L_BridgeRebf;
extern cBridgeRebf      R_BridgeRebf;


extern cThrmstr         LIo1_Thrmstr;
extern cThrmstr         RIo1_Thrmstr;

extern cBrgThrm5pModel  L_BrgThrm5pModel;
extern cBrgThrm5pModel  R_BrgThrm5pModel;
extern cBrgThrmstr      L_BrgThrmstr;
extern cBrgThrmstr      R_BrgThrmstr;

extern cDbCelTst        L_DbCelTst;
extern cDbPlsTst        L_DbPlsTst;

extern cEsaBuf          EsaBuf;
extern unsigned         DSP_EsaBufState;


// task1
extern void GridMonitorDemodulate();
extern void GridMonitorLineMonitorT1();
extern void GridMonitorPhaseLockLoopReg();
extern void FaultChkWindT1();
extern void GridMonitorPosNegDiscrimT1();
extern void GridMonitorProcessFbksBrdg1();
extern void LineTask1();
extern void ProductInstrumentationT1(void);
extern void LineInstrumentationT1(void);
extern void LineInstrumentation2T1(void);
extern void WindInstrumentT1(void);
extern void TestSupportT1(void);
extern void IoNetDiag(void);
extern void PeakDetectorT1(void);
extern void DsimFbkCopyT1();

// task1A
extern void RotorInstrumentationT1(void);
extern void RotorInstrumentation2T1(void);
extern void RotorTask1();
extern void RotorControlT1(void);
extern void WindInstrumentT1A(void);
extern void DsimFbkCopyT1A();

// task2
extern void AssignDigitalInputs();
extern void BppbIoNetIO();
extern void CoreCommonSequencer();
extern void CurrentErrorDetectionT2();
extern void FastDisconnectBlockingT2();
extern void FaultChkWindT2();
extern void GDPLControl();
extern void HVACStateMachine();
extern void GridMonitorInstrumentT2();
extern void GridMonitorLineMonitorT2();
extern void GridMonitorPosNegDiscriminatorT2();
extern void GridMonitorProcessFbksT2();
extern void PulseGeneratorT2();
extern void GridTransientIySupport();
extern void InputFromApplication(void);
extern void LineProcessBrgT2();
extern void LineTask2();
extern void OpenGridCheckT2();
extern void OpenPhaseCheckT2();
extern void OutputToApplication(void);
extern void ProcessFbks2();
extern void ProcessRelaysRebf();
extern void ProcessRelaysAead();
extern void ReadDigitalInsAead();
extern void ReactiveCurrentControlT2();
extern void RotorIyControlT2();
extern void RotorProcessBrgT2();
extern void RotorTask2();
extern void RotorTorqueInputT2();
extern void TimePLL();
extern void VarCalcT2();
extern void ZeroVoltageRideThroughSequencer();
extern void RotorInstrumentationT2(void);
extern void ProductInstrumentationT2(void);
extern void LineControlT2(void);
extern void RotorControlT2(void);
extern void GridMonitorConverterProtectionT2();

// task3
extern void AmbientTemperatureProtection();
extern void ThermistRunPermissive();
extern void ThermalOpenFlg();
extern void ThermalShrtFlg();
extern void SealInSuicide();
extern void ThermistTempOkCheck(void);
extern void ApplicReqInputs();
extern void BrgdTotalPowerLoss();
extern void CoolantTempCalc();
extern void CurrentErrorDetectionT3();
extern void CurrentLimitCalcT3();
extern void DistFiltPhCurr();
extern void DistortionFilterTocDetection();
extern void FanDTAControl();
extern void FastDisconnectDetection();
extern void FaultChkWindT3();
extern void GridMonitorFundamentalCalcsT3();
extern void GridMonitorLineMonitorT3();
extern void GridMonitorPosNegDiscrimT3();
extern void GridMonitorProcessFbksT3();
extern void GridMonitorProtection();
extern void GridMonitorTocDetection();
extern void HeatControl();
extern void IOThermalConversion();
extern void LineTask3();
extern void MirrorRebfStatus();
extern void MirrorAeadStatus();
extern void RotorTask3();
extern void RotorControlT3();
extern void LineControlT3(void);
extern void ScaleCalc();
extern void TemperatureShutdownT3();
extern void ThermistBasedProt();
extern void VarCalcT3();
extern void VARVoltRegT3();
extern void TurbPwrLimSpdT3();
extern void TurbPwrLimTrqT3();
extern void TurbPwrLimDerateIdCheck();
extern void WindInstrumentT3();
extern void WindSignalInstrT3();
extern void WindSignalInstrT2();
extern void TempValidCheck();
extern void ProductInstrumentationT3(void);
extern void RotorInstrumentationT3(void);
extern void NonVolatileVarUpdateT3(void);
extern void TurbineControlSimulator(void);
extern void ReactiveCurrentControlT3(void);

// taskB
extern void ConstantValue();
extern void DigInCalcConstAead();
extern void GridMonitorConfigCheck();
extern void LineTaskBgnd();
extern void RotorTaskBgnd();
extern void ProductBkgnd();
extern void HdwDiag();
extern void HdwDiagT1();
extern void HdwDiagT2();
extern void HdwDiagT3();



// Types and Classes
//------------------



// Parameters
//-----------


// Variables
//----------
CREATE_PUBVAR(R_OTTrpDlySm, unsigned);
CREATE_PUBVAR(L_OTTrpDlySm,	unsigned);
CREATE_PUBVAR(Task1Pass,    unsigned) = 0;      // Task 1 counter
CREATE_PUBVAR(Task1APass,   unsigned) = 0;      // Task 1A counter
CREATE_PUBVAR(Task2Pass,    unsigned) = 0;      // Task 2 counter
CREATE_PUBVAR(Task3Pass,    unsigned) = 0;      // Task 3 counter
CREATE_PUBVAR(TaskBPass,    unsigned) = 0;      // Task B counter

CREATE_PUBVAR(T2PassCntr,   int);
CREATE_PUBVAR(BkGdPassCntr, int);               // Task counter for background

CREATE_PUBVAR(Init1Sec,     unsigned);          // system time is larger or equal to 1 sec
CREATE_PUBVAR(Init2Sec,     unsigned);          // system time is larger or equal to 2 sec
CREATE_PUBVAR(Init3Sec,     unsigned);          // system time is larger or equal to 3 sec
CREATE_PUBVAR(Init5Sec,     unsigned);          // system time is larger or equal to 3 sec

CREATE_PUBVAR(CelTstBrgSel,   unsigned);        // Bridge selection for Cell tests (task2 variable)

// thermistor fault-based heater open loop operation request
CREATE_PUBVAR(CfcOpenLoopReq     , unsigned) =false;   // open loop request for converter distortion filter heaters
CREATE_PUBVAR(PdcOpenLoopReq     , unsigned) =false;   // open loop request for converter AC entry cabinet heaters
CREATE_PUBVAR(LqdOpenLoopReq     , unsigned) =false;   // open loop request for liquid heaters

CREATE_PUBVAR(CnvShutDownTm      , unsigned);          // open loop request for liquid heaters
CREATE_PUBVAR(NvUpdateStZ1       , int);               //  deltayed NvUpdateSt

CREATE_PUBVAR(TrqCalT3Pu,   float);             // calculated torque (pu) available for ESA buffer trigger logic
CREATE_PUBVAR(S_IMagT3Pu,   float);             // stator current (pu) available for ESA buffer trigger logic
CREATE_PUBVAR(TmpTrpDlySum , unsigned) ;            // Temperature delay trip summary

// Data Passing
//--------------


// Unpublished Variables
//----------------------
float           R_LT1TmDelta;



// Local Prototypes (to resolve forward references)
//-------------------------------------------------



// Class Instances
//----------------




///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Product level Task 1 driver
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void Task1(void)
{
    ++Task1Pass;

    /* Calculate difference in elapsed rotor-line time at center of Task1.
     * Used to compensate T1 variables in one algorithm for use by the other.
     * e.g. R_Angle = L_Angle + R_LT1TmDelta * `L_Omega.
     * or   L_Angle = R_Angle - R_LT1TmDelta * R_Omega.    */

    // R_LT1TmDelta = (T1ASt + 0.5) * R_DelTm1 - (T1St + 0.5) * L_DelTm1;      - ESS formula

    if (T1ASt & 0x1) R_LT1TmDelta = -0.25F * L_DelTm1;    // this assumes T1A exec sequence  = 0 - 1 2 - 3
    else             R_LT1TmDelta =  0.25F * L_DelTm1;    //                        for T1St = 0 1 2 3 4 5


    //These line read in analog inputs --KEEP THEM IN ORDER: Open,Process,Close--were formally after feedbacks

    //-------------------------------------------------------------------------
    // bridge feedbacks
	if(SimMode && DsimMode)
    {
        DsimFbkCopyT1();
    }

    L_BridgeRebf.Task1Fbk();

    //-------------------------------------------------------------------------
    // instrumentation
    ProductInstrumentationT1();
    LineInstrumentationT1();

    GridMonitorProcessFbksBrdg1();   // rotate
    GridMonitorDemodulate();         // calculate the sin cos of feedback
    GridMonitorLineMonitorT1();
    GridMonitorPosNegDiscrimT1();
    GridMonitorPhaseLockLoopReg();   // pll update the GmAngElecFbk and GmAngElecCmd

    FaultChkWindT1();                // islanding / hvrt support

    LineTask1();                     // Line converter task 1 runs if selected, calculate the sin cos of GmAngEleccmd

    LineInstrumentation2T1();        // Line instrumentation to run after LineTask1()

    WindInstrumentT1();              // Wind instrumentation (X-Y Filters for RmsMeter)

    /* Read AEAA Digital inputs and Relay states   */
    /* They will be passed to Task2 for processing */

    /* Gather Task1 data for buffer passing to Task2 */

    /* Output Asic commands */
    /* WARNING: Must not be interrupted */             /* (Timing = 1.90usec) */


    /* Output data to bridge */
    //ProcessBrgT1(); move for each bridge

    PeakDetectorT1();

    //-------------------------------------------------------------------------
    // bridge commands
    L_BridgeRebf.Task1Cmd();
    TestSupportT1();
    HdwDiagT1();

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Product level Task 1A driver
//
// EXECUTION LEVEL
//      Task1A
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void Task1A(void)
{
    ++Task1APass;

    //-------------------------------------------------------------------------
    // bridge feedbacks
	if(SimMode && DsimMode)
    {
        DsimFbkCopyT1A();
    }
    R_BridgeRebf.Task1Fbk();
    //-------------------------------------------------------------------------
    // instrumentation
    RotorInstrumentationT1();

    //-------------------------------------------------------------------------
    // control functions
    RotorTask1();

    RotorControlT1();

    RotorInstrumentation2T1();   // Rotor instrumentation to run after RotorTask1()

    //-------------------------------------------------------------------------
    // Wind instrumentation(X-Y Filters for RmsMeters)
    WindInstrumentT1A();

    //-------------------------------------------------------------------------
    // bridge commands
    R_BridgeRebf.Task1Cmd();

    //-------------------------------------------------------------------------
    // Electrical Signal Analysis buffer (normal exec is T1A, but configurable)
    EsaBuf.Task1A();


    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Product level Task 2 driver
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void Task2(void)
{
    ++Task2Pass;

    //-------------------------------------------------------------------------
    // hardware input

    //-------------------------------------------------------------------------
    // needs to be first in line
    if ( !L_Running )
    {
        L_DbCelTst.Task2();
    }

    //-------------------------------------------------------------------------
    // bridge interfaces
    L_BridgeRebf.Task2();
    R_BridgeRebf.Task2();

    //-------------------------------------------------------------------------
    // instrumentation
    RotorInstrumentationT2();

    //-------------------------------------------------------------------------
    // translate inputs from application control
    InputFromApplication();


    TimePLL();
    /*
     * FramePLL() must run before InSyncIo() to fix ASIC DMA bug
     */




    /* Read Digital Inputs from AEAA */
    ReadDigitalInsAead();
    //Assign digital inputs to application specific vars
    AssignDigitalInputs();

    GridMonitorProcessFbksT2();
    GridMonitorInstrumentT2();
    GridMonitorPosNegDiscriminatorT2();
    GridMonitorLineMonitorT2();

    //-------------------------------------------------------------------------
    // product instrumentation - combines Gm & S_Gm T2 values, therefore call
    // after RotorInstrumentationT2() and GridMonitorInstrumentT2()
    ProductInstrumentationT2();


    ProcessFbks2();             // Feedback calculations for bridge
    CoreCommonSequencer();
    ZeroVoltageRideThroughSequencer();

    PulseGeneratorT2();

    GridTransientIySupport();

    ReactiveCurrentControlT2();

    RotorIyControlT2();

    GDPLControl();

    RotorTask2();
    LineTask2();

    //  Wizard selection of celltest bridge
    if (!L_AutoCTAct && !R_AutoCTAct)
    {
        CelTstBrgSel = PARM(CtBrgSel);
    }

    //-----------
    // test modes
    R_RotorPulseTest.Task2();
    L_LinePulseTest.Task2();
    L_DbPlsTst.Task2();

    // run cell test task2 state machine, only if a valid bridge is chosen
    // selection of bridge comes from Wizard or sequencer Auto-Cell-Test
    if ( CelTstBrgSel == CT_LINE_BRG )
    {
        L_CellTest.Task2();
        L_SeqAct.L_CelTstAct   = L_CellTest.CelTstAct;
        L_SeqStat.L_CelTstStat = L_CellTest.CelTstStat;
    }
    else if ( CelTstBrgSel == CT_ROTOR_BRG1 )
    {
        R_CellTest.Task2();
        R_SeqAct.R_CelTstAct   = R_CellTest.CelTstAct;
        R_SeqStat.R_CelTstStat = R_CellTest.CelTstStat;
    }

    LineControlT2();
    RotorControlT2();

    ProcessRelaysRebf();          // Rebf low-voltage relay outputs
    ProcessRelaysAead();          // AEAD Relay outputs

    if ( InitDone)
    {
        VarCalcT2();     // Calculate diagnostic variables
        FaultChkWindT2();
        HVACStateMachine();
        //**These three function calls should be in this order***
        OpenPhaseCheckT2();
        GridMonitorConverterProtectionT2();
        OpenGridCheckT2();
        FastDisconnectBlockingT2();
        //*******************************************************
        CurrentErrorDetectionT2();
    }

    RotorProcessBrgT2();     // Process rotor bridge functions
    LineProcessBrgT2();      // Process line bridge functions

    /* Diagnostic bit-packed word */
    DiagBits.TripFltBrg1   = Brg1TripFlt;
    DiagBits.L_SeqSt       = L_SeqSt;
    DiagBits.L_RegLimSum   = L_RegLimSum;
    DiagBits.TripFltBrg2   = Brg2TripFlt;
    DiagBits.R_SeqSt       = R_SeqSt;
    DiagBits.R_SyncTskSt   = R_SyncTskSt;
    DiagBits.R_CroBarSt    = R_BrgCroBarCtl.CroBarSt;
    DiagBits.S_K1Status    = S_K1Status;
    DiagBits.R_CCOpnCmd = R_CCOpnCmd;
    DiagBits.L_MaStatus    = L_MaStatus;


    // service IoNet interface to BPPB dual port memory
    BppbIoNetIO();
    RotorTorqueInputT2();            // Condition torque input--Put here just after IONet call to save exec time and allow T2

    // Signal Meter Instrumentation
    WindSignalInstrT2();


    // translate ouptuts to application control
    OutputToApplication();

    // Electrical Signal Analysis buffer
    EsaBuf.Task2();
    HdwDiagT2();

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Product level Task 3 driver
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void Task3()
{
    ++Task3Pass;

    if ( (EventTmSec > 100000000U) && (NvUpdateStZ1 ==2) && (NvUpdateSt == 3) ) // total secs since 1/1/1970, calcualte this only when boot up
    {
        // calculate the converter shutdown time
        CnvShutDownTm = EventTmSec - EventTmSecNv;
    }

    NvUpdateStZ1 = NvUpdateSt;

    //-------------------------------------------------------------------------
    // bridge interfaces
    L_BridgeRebf.Task3();
    R_BridgeRebf.Task3();

    TempValidCheck();

    //-------------------------------------------------------------------------
    // instrumentation
    ProductInstrumentationT3();
    RotorInstrumentationT3();
    ReactiveCurrentControlT3();
    //-------------------------------------------------------------------------
    // run heater and fan control when temperature is valid
    // logic for the heater is embeded in the class

    //-------------------------------------------------------------------------
    // run the heater control

    // check the open loop request indicator for heaters
    CfcOpenLoopReq = AirInThrmOpnFlg || CfcThrmOpnFlg || AirInThrmShrtFlg || CfcThrmShrtFlg;
    PdcOpenLoopReq = AirInThrmOpnFlg || PdcThrmOpnFlg || AirInThrmShrtFlg || PdcThrmShrtFlg;
    LqdOpenLoopReq = AirInThrmOpnFlg || LqdThrmOpnFlg || AirInThrmShrtFlg || LqdThrmShrtFlg;


    // suppression heater andf fans for 5 seconds after booting up (got the TempValid)
    if ( HtrFanRunOk )
    {
        //-------------------------------------------------------------------------
        // run fan control

    }


    /* Following 2 flags must be set early in task 3 before any fault-checks can reference them */
    L_OTTrpDlySm = false;
    R_OTTrpDlySm = false;

    ApplicReqInputs();         /* Formation of RunReq, RPNWRunReq, and VARModeCmd */

    //CoreCommonSequencer();

    FaultChkWindT3();

    ScaleCalc();

    //GenericCircuitFlt();        /* Normally closed and open circuit check */

    IOThermalConversion();

    LIo1_Thrmstr.Task3();         // convert volts to degrees
    RIo1_Thrmstr.Task3();

    ThermistRunPermissive();
    ThermistTempOkCheck();

    if ( TempIsValid )
    {
        ThermalOpenFlg ();
        ThermalShrtFlg ();
        SealInSuicide();

        AmbientTemperatureProtection();
        ThermistBasedProt();
    }
    FanDTAControl();

    HeatControl();

    // generator and rotor bridges current limit calculation
    CurrentLimitCalcT3();

    VARVoltRegT3();

    CoolantTempCalc();

    BrgdTotalPowerLoss();        /* Bridge total power loss calculation */

    RotorTask3();

    RotorControlT3();

    LineTask3();

    LineControlT3();


    VarCalcT3();

    GridMonitorProcessFbksT3();

    GridMonitorPosNegDiscrimT3();

    GridMonitorLineMonitorT3();

    GridMonitorFundamentalCalcsT3();

    GridMonitorTocDetection();

    GridMonitorProtection();

    FastDisconnectDetection();

    CurrentErrorDetectionT3();

    DistortionFilterTocDetection();

    // turbine power limiter
    TurbPwrLimSpdT3();
    TurbPwrLimTrqT3();
    TurbPwrLimDerateIdCheck();

    TurbineControlSimulator();

    // Rms & Signal Meter Instrumentation
    WindInstrumentT3();
    WindSignalInstrT3();


    MirrorRebfStatus();
    MirrorAeadStatus();

    // Following summary must be done after all functions that populate
    // the flags run, so leave it near end of T3
    TmpTrpDlySum = ( L_OTTrpDlySm || L_BrgThrmstr.BthtrOtFltDlySum ||
                     R_BrgThrmstr.BthtrOtFltDlySum ||
                     LhslaOtDlyTrpFlg || RhslaOtDlyTrpFlg || ThermisDlySm || TherBadDlySm ||
                     L_BrgThrm5pModel.BtmOtFaultDlySum || R_BrgThrm5pModel.BtmOtFaultDlySum );

    TemperatureShutdownT3();

    //FaultSummaryCheck();

    NonVolatileVarUpdateT3();


    if ( SysTm3.Sec > 0)
    {
        Init1Sec = true;
    }

    if ( SysTm3.Sec > 1)
    {
        Init2Sec = true;
    }

    if ( SysTm3.Sec > 2)
    {
        Init3Sec = true;
    }


    if ( SysTm3.Sec > 4)
    {
        Init5Sec = true;
    }

    T2PassCntr = 0;

	//                 Electrical Signal Analysis buffer trigger:
    //                 -----------------------------------------
    // PU torque
	// (Factor TrqCmdIn/TrqCmdInTr makes TrqCalT3Pu equal 1.0 at rated.)
    TrqCalT3Pu = -R_TrqCalT3 * R_TrqRatRcp * ( NoNANLim( R_TrqCmdInT3 / TrqCmdInTrT3, 1.0e6F, -1.0e6F ) );
    
	// PU stator current
    S_IMagT3Pu = S_IMag * S_AmpsRatRcp;

#ifndef DSIM_MODE	
    // Electrical Signal Analysis buffer
    EsaBuf.EsaTrigger();
    EsaBuf.Task3();
#endif
	
    IoNetDiag();
    HdwDiagT3();


    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Product level Background driver
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void TaskB()
{
    ++TaskBPass;

    //-------------------------------------------------------------------------
    // bridge interfaces
    L_BridgeRebf.TaskBkgnd();
    R_BridgeRebf.TaskBkgnd();

    //-------------------------------------------------------------------------
    // Common background calls
    {

        DigInCalcConstAead();           /* Calc. fwd. gain of Dig. in debounce filt. for AEAD */
        ConstantValue();                /* Process user defined constant values      */
        GridMonitorConfigCheck();       /* call grid monitor config check */


        ProductBkgnd();
        HdwDiag();
        RotorTaskBgnd();
        LineTaskBgnd();


        BkGdPassCntr = 0;
    }

    // Electrical Signal Analysis buffer
    EsaBuf.ReConfigure();                   // Configure ESA buffer from EE's
    EsaBuf.AutoEnable();                    // reenable ESA buffer
    DSP_EsaBufState = EsaBuf.GetState();    // this is the link to IPC and trender



    //-------------------------------------------------------------------------
    // done
    return;
}

