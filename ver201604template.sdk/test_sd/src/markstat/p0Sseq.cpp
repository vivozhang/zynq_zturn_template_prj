///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Main control sequencer
//
// DESCRIPTION:
//              This file contains functions for the  main control sequencer
//              and product support for ADL server.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              p0Seq.vsd, p0Seq.xls
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// System
// Core
// Framework
#include "x0FrameWork.h"
#include "x0AdlSvr.h"
// Product
#include  "Product.h"
#include  "b0BridgeRebf.h"
#include  "b0DbThrm.h"
#include  "b0DbCtl.h"
#include  "b0DbPlsTst.h"
#include  "b0DbCelTst.h"
#include  "p0EsaBuf.h"
#include  "p0Sseq.h"
#include  "p1Sseq.h"
#include  "p2Sseq.h"
#include  "s0SeqTsk.h"
#include  "p0FltChk.h"


// Constants
//----------
#define ZVRT_GATE_NORM    0
#define ZVRT_RTR_OFFREQ   1
#define ZVRT_LINE_OFFREQ  2
#define ZVRT_WAIT         3
#define ZVRT_LINE_RUNREQ  4
#define ZVRT_LINE_RUNNING 5
#define ZVRT_RTR_RUNREQ   6
#define ZVRT_RTR_RUNNING  7

// Externals
//----------
extern EpldCmdRgr * EpldCmdP;
extern nvVar_tbl    NV;
extern unsigned     R_SeqStBits_PackedBits;
extern SeqActBits   L_SeqAct;       // Sequencer Task Action bits
extern SeqBits1     L_SeqStBits;    // Sequencer State bits
extern unsigned     L_SeqReq_PackedBits;
extern SeqStatBits  L_SeqStat;      // Sequencer Task Status bits
extern SysTm_Typ    SysTm3;
extern cBridgeRebf  L_BridgeRebf;
extern cBridgeRebf  R_BridgeRebf;
extern cDbThrmModel L_DbThrmModel;
extern cEsaBuf      EsaBuf;

extern float        CfcTemp;
extern float        CoolantTemp;
extern float        DelTm2;
extern float        DelTm3;
extern float        GmNegSeqVPct;
extern float        GmVllMagAvg;
extern float        InletAirTemp;
extern float        L_VdcFbkT2;
extern float        L_VdcNomCal;
extern float        LqdAuxTmr1;
extern float        PdcTemp;
extern float        CoolantTemp;
extern float        R_CroBarVdcLvlOffT2;
extern float        GmVMagPuT2;
extern float        CoolantTemp;

extern int          DbInh;
extern int          L_SeqSt;
extern int          R_SeqSt;
extern int          L_DbRefTemp;
extern int          WEMADisabSim;
extern unsigned     CoolantOtDlyAlmFlg;
extern unsigned     GmLineVShort;
extern unsigned     GmPllProven;
extern unsigned     GmPllPrvPrm;
extern unsigned     Init1Sec;
extern unsigned     InitDone;
extern unsigned     LqdThrmDlFlg;
extern unsigned     S_K1Status;
extern unsigned     L_BrgPwrEnb;
extern unsigned     L_InitDone;
extern unsigned     L_InnrRegEnb;
extern unsigned     L_OTAlSum;
extern unsigned     L_ParkReq;
extern unsigned     L_PlsTstCmd;
extern unsigned     L_CelTstCmd;
extern unsigned     L_RunCmd;
extern unsigned     L_Running;
extern unsigned     L_RunRdy;
extern unsigned     L_RunReq;
extern unsigned     LqdPmpMtrCmd;
extern unsigned     LqdPmpMtrCmdT3;
extern unsigned     LhslaOtDlyTrpFlg;
extern unsigned     RhslaOtDlyTrpFlg;
extern unsigned     R_BrgPwrEnb;
extern unsigned     R_InitDone;
extern unsigned     R_InnrRegEnb;
extern unsigned     R_MctlTunCmd;
extern unsigned     R_OTAlSum;
extern unsigned     R_ParkReq;
extern unsigned     R_PlsTstCmd;
extern unsigned     R_CelTstCmd;
extern unsigned     R_Running;
extern unsigned     R_RunRdy;
extern unsigned     L_SpinRdy;
extern unsigned     R_SpinRdy;
extern unsigned     R_RunReq;
extern unsigned     S_PllPrvPrm;
extern unsigned     S_SimK1Clsd;
extern unsigned     SftyChnOpn;
extern unsigned     SimMode;
extern unsigned     T3CalcDone;
extern unsigned     TempTrpLatch;
extern unsigned     ThermisAlmSm;
extern unsigned     LqdThrmOpnFlg;

extern unsigned     L_DbCelTstCmd;
extern unsigned     IpcInterfaceOk;
extern unsigned     DTAFanEnb;
extern unsigned     HeaterEnb;
extern unsigned     LqdPmpMtrEnb;
extern unsigned     GEvtReqADLSim;

extern unsigned     L_LTestCmd;
extern unsigned     Brg2TripFlt;
extern unsigned     GmFrqVltStopAct;
extern unsigned     GmOVTrpSum;
extern unsigned     Q1OpnReq;
extern float   		GmSVMtchErSq;
extern float	    GmSVmtchThr;
extern unsigned     ISimMode;
extern unsigned     Q1StatSim;
extern unsigned     HVACState;
extern unsigned     IslandDet;
extern unsigned     DisconWhlRun;
extern unsigned     DisconWhlOv;
extern float        GmVMagT2;
extern float        L_VlnInRcp;
extern cDbThrmModel L_DbThrmModel;

extern void DisplayFaults();
extern void GridMonConverterFilterPreset(void);
extern unsigned     Q1Status;
extern unsigned     Q1UVHld;
extern unsigned     SysTmSec;
extern unsigned     Q1RunPrm;
// for CoreCommonSequencer()
extern float    L_ChgLvlDcDy;
extern float    L_ChgLvlAcDy;
// for HeatControl
extern unsigned PdcThrmOpnFlg;
extern unsigned PdcThrmDlFlg;
extern unsigned CfcThrmOpnFlg;
extern unsigned CfcThrmDlFlg;
extern float    MccTemp;
extern float    PdcCabEntryTemp;

// for CmdPulseTest
extern cDbPlsTst    L_DbPlsTst;
extern unsigned     L_DbPlsTstCmd;

// for CmdCellTest
extern cDbCelTst    L_DbCelTst;
extern unsigned     L_DbCelTstCmd;

// for AbortCellTest
extern unsigned     L_DbCelTstCmd;

extern unsigned      TcRunCmd;
extern unsigned      TcCnvRunCmd;
extern unsigned      TcSimEnb;
extern unsigned      TcRPNWReq;
extern unsigned      LclRPNWReq;

extern FaultIsActiveTyp     FaultIsActive;

extern unsigned      HVACStAct;
extern unsigned     LocalMode;


// Types and Classes
//------------------
struct EdgBits
{
    unsigned    RunReqEdg     :1;     // Edge detect for run request
    unsigned    RPNWReqEdg    :1;     // Edge detect for RPNW run request
    unsigned    KypadRPNWEdg  :1;     // Edge detect for Keypad RPNW run request
    unsigned    KypadVarEdg   :1;     // Edge detect for Keypad Var Mode request
} ;


// Local Prototypes (to resolve forward references)
//-------------------------------------------------
int ReqStop(void);
int ReqRun(void);
int ToggleRpnwRunRequest(void);
int ToggleVARModeRequest(void);
int CmdPulseTest(void);
int CmdCellTest(void);
int AbortCellTest(void);
int CmdIndTest(void);
void StopCmdDelay(void);


// Class Instances
//----------------
cCoreCommonAdlSeq   CoreCommonAdlSeq;


// Parameters
//-----------
CREATE_PARM(PtBrgSel,           unsigned);  // 0 = rotor bridge, 1 = line bridge, 2 = DB, 3 = Dc-Chopper
CREATE_PARM(CtBrgSel,           unsigned);  // 0 = line bridge, 1 = rotor bridge1, 2 = rotor bridge2, 3 = DB
CREATE_PARM(L_DbPtSel,          int);       // DB pulse-test selection DB#1 = 0, DB#2 = 1, and Both DBs = 2
CREATE_PARM(L_DbCtSel,          int);       // DB cell-test selection DB#1 = 0 and DB#2 = 1
CREATE_PARM(L_VacTolrnce,       float);     // Ac line-to-line magnitude tolerance (Pu)
CREATE_PARM(L_VdcOkRatio,       float);     // Ratio between minimum allowed Vdc and a calculated nominal level (Pu)
CREATE_PARM(L_DbCelTstVlnW,     float);     // Omega to filter line voltage mag in Pu during Db cell test (Rad/Sec)
CREATE_PARM(WemaEnbSimRq,       unsigned);
CREATE_PARM(WemaPicRevCk,       int);
CREATE_PARM(WemaFpgaVerOffset,  int);
CREATE_PARM(VmagStpDlyW,        float);     // Omega to filter for ac voltage
CREATE_PARM(VmagStpDlyThr,      float);     // ac filter threshold to ignore stop command

CREATE_PARM(LinkChgOKTm,        float);
CREATE_PARM(RtrRunDly,          float);     // Rotor converter run delay
CREATE_PARM(XStpAdr,            unsigned*); // XStop request pointer
CREATE_PARM(RunReqAdr,          unsigned*); // Run request pointer
CREATE_PARM(RPNWRunAdr,         unsigned*); // RPNW run request pointer
CREATE_PARM(RPNWRunDly,         float);     // RPNW run delay
CREATE_PARM(StopReqAdr,         unsigned*); // Stop request pointer (for Momentary start/stop);
CREATE_PARM(LqdAuxOffTm,        float);     // Liquid auxillary time to shut-down
CREATE_PARM(LqdAuxOnTm,         float);     // Liquid auxillary time to fill pipes
CREATE_PARM(PLCRebootInh,       unsigned);  // PLC reboot inhibit bit
CREATE_PARM(RebootMaxTm,        float);     // PLC reboot max time threshold
CREATE_PARM(RebootMinTm,        float);     // PLC reboot min time threshold
CREATE_PARM(SftyRtrLnDly,       float);     // Safety stop delay time between rotor bridge power off and command to turn of line conv.
// ZVRT Sequence
CREATE_PARM(ZVGtOfMxTm,         float);     // Zero voltage max gate off time
CREATE_PARM(ZVGtOfMxTmSt,       float);     // Zero voltage max gate off time stator
CREATE_PARM(ZVStopGtTm,         float);     // Zero voltage stop gating time
CREATE_PARM(ZVStopGtTm2,        float);     // Zero voltage stop gating time # 2
CREATE_PARM(ZVStopGtVdcCBMgn,   float);     // Zero voltage stop gate Vdc margin below CroBar Off threshold
CREATE_PARM(ZVPllPrmTm,         float);     // ZVRT pll prove time  for line
CREATE_PARM(ZVPllPrmTmSt,       float);     // ZVRT pll prove time for stator
CREATE_PARM(ZVQckRcvyTm,        float);     // ZVRT time for which Iy iz zeroed if speed is lower than ZVZEroIySpd at recovery
// FanDTAControl
CREATE_PARM(FanMinOffTm,        float);     // Fan minimum time in the off-state
CREATE_PARM(FanMinOnTm,         float);     // Fan minimum time in the on-state
CREATE_PARM(InAirMaxTemp,       float);     // Upper limit for trimming inlet air temperature
CREATE_PARM(InAirMinTemp,       float);     // Lower limit for trimming inlet air temperature
CREATE_PARM(CoolDeltaOn,        float);     // Upper differential limit for coolant temp reference
CREATE_PARM(CoolDeltaOff,       float);     // Lower differential limit for coolant temp reference
CREATE_PARM(CfcDeltaOn,         float);     // Upper hysteresis limit for CFC temp
CREATE_PARM(CfcDeltaOff,        float);     // Lower hysteresis limit for CFC temp
CREATE_PARM(Q1FanPdcOnThr,      float);     // Fan 5 On threshold in degC for PdcTemp
CREATE_PARM(Q1FanPdcOfThr,      float);     // Fan 5 Off threshold in degC for PdcTemp
CREATE_PARM(Q1FanCabEntryOnThr, float);     // Fan 5 On threshold in degC for PdcTemp
CREATE_PARM(Q1FanCabEntryOfThr, float);     // Fan 5 Off threshold in degC for PdcTemp
// HeatControl
CREATE_PARM(AirHeatrMax,        float);     // Maximun limit to clamp inlet air temp for heating control
CREATE_PARM(AirHeatrMin,        float);     // Minimum limit to clamp inlet air temp for heating control
CREATE_PARM(MCCHtrOn,           float);     // MCC heat On threshold in degC
CREATE_PARM(MCCHtrOff,          float);     // MCC heat Off threshold in degC
CREATE_PARM(CoolntHtrOn,        float);     // Coolant heat On threshold in degC
CREATE_PARM(CoolntHtrOff,       float);     // Coolant heat Off threshold in degC
CREATE_PARM(PdcCfcHtrOn,        float);     // PDC & CFC heat On threshold in degC
CREATE_PARM(PdcCfcHtrOff,       float);     // PDC & CFC heat Off threshold in degC
CREATE_PARM(WEMAComDlyTm,       int);       // Delay time for tesing communications with WEMA - sec

CREATE_PARM(IpcIntTrpUpDt,      float);     // IPC Not Ok Trip Up Counter
CREATE_PARM(IpcIntTrpDnDt,      float);     // IPC Not Ok Trip Down Counter
CREATE_PARM(IpcIntTrpThr,       float);     // IPC Not Ok Trip Threshold

// Published Variables
//--------------------
CREATE_PUBVAR(SeqSt,            int) = -1;        // Sequencer state variable
CREATE_PUBVAR(LineRunOnly,      unsigned);        // Run line converter only
CREATE_PUBVAR(LnRunningSt,      unsigned);        // Line converter running state
CREATE_PUBVAR(LnRunReqSt,       unsigned);        // Line converter run request state
CREATE_PUBVAR(LnStpReqSt,       unsigned);        // Line converter stop request state
CREATE_PUBVAR(LocFlt,           unsigned);        // Global local fault
CREATE_PUBVAR(Parked,           unsigned);        // Global parked flag
CREATE_PUBVAR(ParkReq,          unsigned);        // Global Park request flag
CREATE_PUBVAR(RtRunReqSt,       unsigned);        // Rotor converter run request state
CREATE_PUBVAR(RtStpReqSt,       unsigned);        // Rotor converter stop request state
CREATE_PUBVAR(Running,          unsigned);        // Global running flag
CREATE_PUBVAR(RunRdy,           unsigned) =false; // Global ready to run
CREATE_PUBVAR(SpinRdy,          unsigned) =false; // Global ready to spin
CREATE_PUBVAR(RunReq,           unsigned);        // Global run request flag
CREATE_PUBVAR(RunCmd,           unsigned);        // Global run request flag
CREATE_PUBVAR(RunCmdZ1,         unsigned);        // Previous sample of global run request
CREATE_PUBVAR(StpCmdDlyFlg,     unsigned);        // Flag for conditions met
CREATE_PUBVAR(VmagStpDly,       float);           // Filter for ac voltage
CREATE_PUBVAR(VmagStpDlyPu,     float);           // Pu value for filter for ac voltage
CREATE_PUBVAR(VmagStpDlyGn,     float);           // Gain for filter for ac voltage
CREATE_PUBVAR(Stopped,          unsigned) = true; // Global stopped flag
CREATE_PUBVAR(SysFlt,           unsigned);        // Global system fault
CREATE_PUBVAR(StoppedB1,        unsigned);        // Bridge 1 stopped
CREATE_PUBVAR(StoppedB2,        unsigned);        // Bridge 2 stopped
CREATE_PUBVAR(LqdAuxTmr,        float);           // Liquid auxillary timer for turn-off delay
CREATE_PUBVAR(LqdAuxTmr1,       float);           // Liquid auxillary timer for turn-on delay
CREATE_PUBVAR(LqdPmpMtrCmd,     unsigned);        // Liquid pump motor command
CREATE_PUBVAR(LqdPmpMtrCmdT3,   unsigned);        // pump on command task 3 version
CREATE_PUBVAR(LqdPmpCnvCmd,     unsigned);        // Liquid pump motor command from converter operation status
CREATE_PUBVAR(LqdPmpHtrCmd,     unsigned);        // Liquid pump motor command from liquid heater operation status
CREATE_PUBVAR(LqdTempOkFlg,     unsigned);        // flag indicating liquid temperature is ok to run the pump
CREATE_PUBVAR(BrgEnbSim,        unsigned);        // Bridge enbaled when in simulation mode
CREATE_PUBVAR(BrgCardsAct,      unsigned);        // Over-all bridge cards active boolean
CREATE_PUBVAR(SafetyStopIp,     unsigned);        // Safety chain stop
CREATE_PUBVAR(SafetyChnStop,    unsigned);        // Safety chain stop diag
CREATE_PUBVAR(SftyChnQ1Opn,     unsigned);        // Safety chain requests MA to open via line sequencer
CREATE_PUBVAR(TempDlyFlag,      unsigned);        // Temperature delay trip flag
CREATE_PUBVAR(VgridIsOk,        unsigned);        // Grid voltage is within tolerances while in the stop state
CREATE_PUBVAR(VdcIsOk,          unsigned);        // DC link voltage is high enough while stopped so fuses won't
                                                  // fatige during closing of Q1
CREATE_PUBVAR(VdcOkLvl,         float);           // Vdc Ok level (volt)
CREATE_PUBVAR(L_DbCelTstVlnPu,  float);           // Line voltage magnitude in Pu for checking Dc-level during Db cell test
CREATE_PUBVAR(L_DbCelTstVlnGn,  float);           // Gain for filtering line voltage mag in Pu during Db cell test
CREATE_PUBVAR(PsInrshLimClsCmd, unsigned) =false; // relay command to short 24Vdc power supply inrush limiter resistor
CREATE_PUBVAR(DtaFanRunning,    unsigned) =true;  // DTA main fan running indication - logic needs to be attached to this!

//ZVRT vars
CREATE_PUBVAR(L_ZVGtOff       , unsigned);       // Line converter zero voltage gate off request.
CREATE_PUBVAR(L_ZVLineOff     , unsigned);       // Line zero voltage rotor off
CREATE_PUBVAR(R_ZVGtOff       , unsigned);       // Rotor converter zero voltage gate off request.
CREATE_PUBVAR(R_ZVRtrOff      , unsigned);       // Rotor zero voltage rotor off
CREATE_PUBVAR(ZVRTGtSt        , unsigned);       // ZVRT gate state
CREATE_PUBVAR(ZVRTIp          , unsigned);       // ZVRT in progress
CREATE_PUBVAR(ZVRTQuickIp     , unsigned);       // ZVRT active
CREATE_PUBVAR(ZVStopGtTmr     , float);          // ZVRT stop gate timer
CREATE_PUBVAR(ZVStopGtTmr2    , float);          // ZVRT stop gate timer
CREATE_PUBVAR(ZVRTTmr         , float);          // General ZVRT timer
CREATE_PUBVAR(ZVRcvryTmr      , float);          // ZVRT recovery timer
CREATE_PUBVAR(ZVQckRcvyTmr    , float);          // ZVRT quick recovery timer

//RPNW vars
CREATE_PUBVAR(RPNWRunCmd      , unsigned);       // RPNW run command
CREATE_PUBVAR(RPNWRunReq      , unsigned);       // Global RPNW run request flag
CREATE_PUBVAR(RPNWRunReqZ1    , unsigned);       // Previous sample of global RPNW run request
CREATE_PUBVAR(RPNWRunning     , unsigned);       // Global RPNW running flag
CREATE_PUBVAR(RPNWRunRdy      , unsigned);       // Converter ready to run RPNW
CREATE_PUBVAR(RtrLnIyXfer     , unsigned);       // Rotor-Line iy transfer rquest
CREATE_PUBVAR(IyXferDone      , unsigned);       // Rotor-Line iy transfer done

CREATE_PUBVAR(SeqTimer        , float );         // Main sequencer timer
CREATE_PUBVAR(SeqStTmr        , float );         // Sequence state timer

// Main DTA cooling fan control
CREATE_PUBVAR(DTAFanCmd       , unsigned);       // Main DTA cooling fan control
CREATE_PUBVAR(CoolntIsHot     , unsigned);       // Liquid coolant temperature is above threshold
CREATE_PUBVAR(CoolntIsCool    , unsigned);       // Liquid coolant temperature is below threshold
CREATE_PUBVAR(CfcIsHot        , unsigned);       // CFC temperature is above threshold
CREATE_PUBVAR(CfcIsCool       , unsigned);       // CFC temperature is below threshold
CREATE_PUBVAR(DTAFanRunReq    , unsigned);       // DTA fan run request
CREATE_PUBVAR(FanPassOff      , unsigned);       // Fan stayed off more than minimum time
CREATE_PUBVAR(FanPassOn       , unsigned);       // Fan ran more than minimum time
CREATE_PUBVAR(FanStateTmr     , float);          // Fan state timer
CREATE_PUBVAR(AirTempRefLv    , float);          // Inlet air temperature reference level
CREATE_PUBVAR(L_DbFanReq,       unsigned);
CREATE_PUBVAR(Q1FanCmd        , unsigned);       // Fan 5 control command


// Heating control
CREATE_PUBVAR(MCCHeatOff      , unsigned);       // When asserted, allows MCC heater zero duty cycle.
CREATE_PUBVAR(CoolntHeatOn    , unsigned);       // When asserted, connects coolant heater and pump.
CREATE_PUBVAR(PdcCfcHeatOf    , unsigned);       // When asserted, disconnects PDC and CFC heaters.
CREATE_PUBVAR(HeatCtrlOn      , unsigned);       // When asserted, WECA controls WEMA heaters.
CREATE_PUBVAR(InlAirForHtr    , float);          // Inlet air temperature subjected to upper and lower limits
CREATE_PUBVAR(PdcCfcForHtr    , float);          // Minimun of trustworthy PDC and CFC temperature readings

CREATE_PUBVAR(LclModeEdg      , unsigned);       // Edge detect for Local mode
CREATE_PUBVAR(StopReq         , unsigned);       // Stop request
CREATE_PUBVAR(WEMAOnline,       unsigned);      // p0Bppb.c30 - Converter <--> WEMA onliness status
CREATE_PUBVAR(PicChksum,       unsigned);
CREATE_PUBVAR(LnkOkWEMA,       unsigned);
CREATE_PUBVAR(WEMAPicVersion, int);

// BPPB vars
CREATE_PUBVAR(BppbEnable      , unsigned) = 2;   // p0Bppb.c30     force enable since detection commented out in p0task.cpp  0-Disable,  1-ADL Only,  2-Enable
CREATE_PUBVAR(BppbHtCtrlDisabled, unsigned);     // p0Bppb.c30

CREATE_PUBVAR(TestFaultNum    , int);

CREATE_PUBVAR(LqdHtrOnCmdT2,    unsigned);       // liquid heater on command task2 version
CREATE_PUBVAR(DbAutoTstForce,   unsigned);       // external signal to force auto Db ping fire

CREATE_PUBVAR(L_DbCelTstCmdZ1,  unsigned);
CREATE_PUBVAR(LclRPNWReq,       unsigned);
CREATE_PUBVAR(LclCnvRunReq,     unsigned);

CREATE_PUBVAR(WemaPicVer,       int);
CREATE_PUBVAR(WemaFpgaVer,      int);
CREATE_PUBVAR(VARModeCmd    ,unsigned  );        /* selection of VAR cmd (vs. pwr-fact control) */

CREATE_PUBVAR(IpcIntTrpTmr,     float);     // IPC Not Ok Trip Timer
CREATE_PUBVAR(IpcInterfaceDown, unsigned);     // IPC Interface down flag

// Diagnostics
//--------------------
CREATE_DIAG(IpcInfcNotOkTrp);                    // IPC interface not ok trip
CREATE_DIAG(WemaPicVerBad);                     // Wema PIC version check
CREATE_DIAG(GmOvTrpDiscAlm);
CREATE_DIAG(GmOvStpDiscAlm);
CREATE_DIAG(WEMACommAlm);           //   WEMA went off-line after power-up
CREATE_DIAG(Starting);              //   converter starting
CREATE_DIAG(BadSeqSt);              // Seq: Illegal sequencer state
CREATE_DIAG(ZVGtOfTOut);            // Zero Voltage gate off time out line
CREATE_DIAG(ZVGtOfTOutSt);          // Zero Voltage gate off time out stator
CREATE_DIAG(RPNWInvalid);           // Reactive Power No Wind is not supported in phi mode
CREATE_DIAG(RpnwOnLine);            // RPNW OnLine
CREATE_DIAG(OffLine);               // OffLine
CREATE_DIAG(SftyChnStop);           // Safety chain stop state
CREATE_DIAG(SafetyChnOpn);          // Safety chain open fault
CREATE_DIAG(TempShtDwn);            // Temperature shut down state
CREATE_DIAG(RunWhlNotRdy);          // Run command while not ready
CREATE_DIAG(GmSyncIniTm);           // Grid monitor PLL synchronize initialize time out

// Unpublished Variables
//----------------------
EdgBits Req_Z = {0};                             // Collection of request edges

// Data Pass
//----------

DATA_PASS(LqdPmpMtrCmd, LqdPmpMtrCmdT3, T2_T3, UNFILTERED);

// Functions
//----------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Main sequencer execution loop
//
// EXECUTION LEVEL


//      Task2 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void CoreCommonSequencer(void)
{
    StopCmdDelay();

    if ( LqdTempOkFlg && LqdHtrOnCmdT2 )
    {
        LqdPmpHtrCmd = true;
    }
    else
    {
        LqdPmpHtrCmd = false;
    }

    Running  = (L_Running  && R_Running);

    StoppedB1 = L_SeqStBits.Stopped;
    StoppedB2 = R_SeqStBits.R_Stopped;


    if ( (SeqSt == STOPPED_COMMON) || (SeqSt == INIT_COMMON))
    {
        if (!Stopped)  PUSH_DIAG(OffLine);  //Push2State(OffLine, MASKABLE);
        Stopped = true;
    }
    else
    {
        if (Stopped)  PUSH_DIAG(Starting);  //Push2State(Starting, MASKABLE);
        Stopped = false;
    }


    if ( L_Running && RPNWRunning )
    {
        LineRunOnly = true;
    }
    else
    {
        LineRunOnly = false;
    }

    if (RPNWRunReq && VARModeCmd && !TripFlt && RPNWRunRdy )
    {
        RPNWRunCmd = true;
    }
    else
    {
        RPNWRunCmd = false;
    }


    if ( L_Running && SftyChnOpn && !TripFlt)
    {
        if ( !SafetyStopIp)
        {
            SafetyStopIp = true;
            RunCmd       = false;
            LclCnvRunReq       = false;
            RPNWRunCmd   = false;
            PUSH_DIAG(SftyChnStop);
            SafetyChnStop = true;

        }
    }


    // If safety chain loss , then remove Run and call fault
    if ( SftyChnOpn  && ((!L_Running && (RunCmd || RPNWRunCmd))  || L_PlsTstCmd ||
                         R_PlsTstCmd || R_MctlTunCmd) )
    {
        RunCmd     = false;
        LclCnvRunReq = false;
        RPNWRunCmd = false;
        if ( R_BridgeRebf.BrgHdwDataVld && L_BridgeRebf.BrgHdwDataVld )
        {
            PUSH_DIAG(SafetyChnOpn);
        }
    }


    if ( ParkReq && Stopped)
    {
        if ( L_SeqStat.L_Q1EnbStat )
        {
            L_SeqReq.L_Q1EnbReq = false;
        }
        else
        {
            L_ParkReq = true;
            R_ParkReq = true;
        }
    }
    Parked = (L_SeqStBits.Parked && R_SeqStBits.R_Parked);


    // Keep track of the time needed to circulate coolant thru pipes once pump is running
    // 10 seconds
    if ( LqdPmpMtrCmd )
    {
        if ( LqdAuxTmr1 < PARM(LqdAuxOnTm) )
        {
            LqdAuxTmr1  = LqdAuxTmr1 + DelTm2;
        }
        else
        {
            LqdAuxTmr1  = PARM(LqdAuxOnTm);
            L_DbRefTemp = static_cast<int> (cDbThrmModel::DB_REF_TEMP_1);  // use coolant temp as reference
        }
    }
    else
    {
        L_DbRefTemp = static_cast<int> (cDbThrmModel::DB_REF_TEMP_2);      // use Igbt plate temp as reference
        if ( LqdAuxTmr1 > 0.0F )
        {
            LqdAuxTmr1 = LqdAuxTmr1 - DelTm2;
        }
        else
        {
            LqdAuxTmr1 = 0.0F;
        }
    }


    // Push delayed shutdown state if a temperature trip has been latched
    if ( TempTrpLatch && !TempDlyFlag )
    {
       // Push2State(TempShtDwn, NONMASKABLE);
        PUSH_DIAG(TempShtDwn);
        TempDlyFlag = true;
    }
    else if ( !TempTrpLatch )
    {
        TempDlyFlag = false;
    }

    if ( !LocalMode && (Running||L_Running) )
    {
       if (!IpcInterfaceOk )
       {
          IpcIntTrpTmr = IpcIntTrpTmr + PARM(IpcIntTrpUpDt); //Set to DelTm2
          if ( IpcIntTrpTmr > PARM(IpcIntTrpThr) )
          {
             IpcIntTrpTmr =  PARM(IpcIntTrpThr);
             IpcInterfaceDown = true;
             PUSH_DIAG( IpcInfcNotOkTrp );
          }
       }
        else
       {
          IpcIntTrpTmr = IpcIntTrpTmr - PARM(IpcIntTrpDnDt); //Set to 0.3333 * DelTm2
          if ( IpcIntTrpTmr < 0.0F)
          {
             IpcIntTrpTmr =  0.0F;
          }
       }
    }
    else
    {
        IpcIntTrpTmr =  0.0F;
        IpcInterfaceDown = !IpcInterfaceOk;
    }


//------------------------- Common Sequencer State Machine  --------------------

    switch ( SeqSt )
    {

    //------------------------ STOPPED COMMON ----------------------------------
        case STOPPED_COMMON:
            LnStpReqSt   = false;

            BrgEnbSim    = PARM(BrgEnbSimReq) && SimMode;
            BrgCardsAct = !SimMode || BrgEnbSim;
            IyXferDone   = false;
            RtrLnIyXfer  = false;

            StpCmdDlyFlg = false;

            if ((GmVllMagAvg >= (PARM(L_VllIn)*L_ChgLvlAcDy)) && (GmVllMagAvg <= ((2.0-PARM(L_VacTolrnce))*PARM(L_VllIn)))
                && (GmNegSeqVPct < PARM(GmNSqMaxStrt))  )
            {
                VgridIsOk = true;
            }
            else
            {
                VgridIsOk = false;
            }


            if ( L_VdcFbkT2 >= L_ChgLvlDcDy*L_VdcNomCal )
            {
                VdcIsOk = true;
            }
            else
            {
                VdcIsOk             = false;
                L_SeqReq.L_Q1EnbReq = false;
                if(SysTmSec>PARM(LinkChgOKTm))

            {
                    //TODO, how to handler from DB capacitance test
                    //PUSH_DIAG(L_LinkChgFlt);
            }
            }

            //=================================================================
            // Common SpinRdy, RunRdy & RPNWRunRdy Logic
            SpinRdy    = L_SpinRdy &&  R_SpinRdy;
            RunRdy     = L_RunRdy  &&  R_RunRdy ;
            RPNWRunRdy = L_RunRdy  && !Brg2TripFlt;
            //=================================================================

            if (RunCmd && !RunCmdZ1 && !RunRdy)
            {
                RunCmd     = false;
                LclCnvRunReq = false;
                PUSH_DIAG(RunWhlNotRdy);
            }

            if ( !RunRdy )
            {
                RunCmd=false;
                LclCnvRunReq = false;
            }

            // Save requests for next pass
            RunCmdZ1     = RunCmd;

            // decide if turn on the pump based ont he liquid temperature
            // not turn on the pump if liquid temperature is below LqdTempOkLvl
            // liquid temperature is ok for pump to pipe
            if( CoolantTemp >= PARM(LqdTempOkLvl) )
            {
                // set the lqd temperature ok
                LqdTempOkFlg = 1;
            }

            else if ( CoolantTemp < PARM(LqdTempLowLvl) )
            {
                // reset the lqd temperature ok
                LqdTempOkFlg = 0;
            }
            if ( !TripFlt && ((RunCmd && RunRdy) || (RPNWRunCmd && RPNWRunRdy)) )
            {
                if ( GmPllProven )      // Grid pll is required to be proven before running
                {
                    if ( LqdTempOkFlg )
                    {
                        LqdPmpCnvCmd = true;
                    }
                    LqdAuxTmr    = 0.0F;
                    RunCmdZ1     = false;

                    //-------------------------------------------------------------------------
                    // immediately fire the DB auto test if:
                    // every time a run cmd is issued
                    SeqSt = CONV_RUN_REQ_LINE;
                    GridMonConverterFilterPreset();
                }
                else
                {
                    PUSH_DIAG(GmSyncIniTm);
                }
            }
            else
            {
                R_RunReq = false;
                L_RunReq = false;

                // Let coolant flow 30 seconds after converter stops
                if ( LqdPmpCnvCmd )
                {
                    LqdAuxTmr = LqdAuxTmr + DelTm2;
                    if (LqdAuxTmr > PARM(LqdAuxOffTm))
                    {
                        LqdPmpCnvCmd = false;
                        LqdAuxTmr    = PARM(LqdAuxOffTm);
                    }
                }
            }

            if ( L_DbThrmModel.DtmCoolingReq && LqdTempOkFlg )
            {
                LqdPmpCnvCmd = true;
                LqdAuxTmr = 0.0F;
            }

            if ( SafetyStopIp || SftyChnOpn )
            {
                SafetyStopIp  = false;
                SftyChnQ1Opn  = true;       // Open MA via line upper sequencer
                SafetyChnStop = false;
            }
            else
            {
                SftyChnQ1Opn = false;
            }

            // Initialize MA and K1 status
            if ( !TripFlt )
            {
                if( !Q1UVHld && !L_SeqStat.L_Q1EnbStat && !L_SeqAct.L_Q1EnbAct )
                {
                    Q1Status = false;
                }
                S_K1Status  = false;
                S_SimK1Clsd = false;
            }

            VmagStpDly   = VmagStpDly + VmagStpDlyGn *(GmVMagT2 - VmagStpDly);
            VmagStpDlyPu = VmagStpDly * L_VlnInRcp;

            if ( (VmagStpDlyPu > PARM(VmagStpDlyThr)) && Q1UVHld)
            {
               Q1OpnReq = true;
               PUSH_DIAG(GmOvStpDiscAlm);
            }

            break;

    //------------------------ LINE CONVERTER RUN REQUEST ----------------------
        case CONV_RUN_REQ_LINE:
            LnRunReqSt = true;


            if ( TripFlt || !(RunCmd || RPNWRunCmd)  )
            {
                RunCmdZ1 = RunCmd;
                SeqSt      = CONV_STOP_REQ_LINE;
            }
            #ifdef LEG_TEST
            else if ( PARM(TestMode) == LF_LEG_TEST)
            {
                L_RunReq = true;
                SeqStTmr = 0.0F;

                if ( L_SeqSt == LINE_IDLE_RTR_LEG_TEST)
                {
                    SeqSt = CONV_RUN_REQ_ROTOR;
                }
            }
            #endif
            else
            {
                if ( L_SeqSt == LINE_RUNNING)
                {
                        SeqSt         = CONV_RUNNING_LINE;
                        SeqTimer      = 0.0F;
                        SeqStTmr      = 0.0F;
                }
                else if ( LqdAuxTmr1 >= PARM(LqdAuxOnTm) )   // Wait until pump circulates coolant
                {
                    L_RunReq = true;
                    SeqStTmr = 0.0F;
                }
            }

            SeqTimer = 0.0;

            break;

    //------------------------ LINE CONVERTER RUNNING --------------------------
        case CONV_RUNNING_LINE:

            LnRunReqSt   = false;
            LnRunningSt  = true;

            if ( TripFlt || !(RunCmd || RPNWRunCmd) )
            {
                RunCmdZ1 = RunCmd;
                SeqSt = CONV_STOP_REQ_LINE;
            }
            else if ( RunCmd && !RunCmdZ1 && !R_RunRdy )
            {
                PUSH_DIAG(RunWhlNotRdy);         //Trip if trying to go from RPNW to running the rotor
                R_RunReq = false;               // and rotor is not ready
            }
            else if ( RunCmd )
            {
                SeqTimer = SeqTimer + DelTm2;
                if ( (SeqTimer >= PARM(RtrRunDly)) && Q1RunPrm )
                {
                    SeqSt    =  CONV_RUN_REQ_ROTOR;
                    SeqTimer = 0.0F;
                    RunCmdZ1 = false;
                }
            }
            else if ( RPNWRunCmd && !RunCmd )
            {
                if (!RPNWRunning)
                {
                    SeqTimer = SeqTimer + DelTm2;
                    if ( SeqTimer > PARM(RPNWRunDly)) //This delay gives time for line volt feedback filters to stabalize
                    {                                 //when running RPNW and before enabling var regulator
                        SeqTimer = 0.0F;
                        //Push2State(RpnwOnLine,  MASKABLE);
                        PUSH_DIAG(RpnwOnLine);
                        RPNWRunning = true;  // Truly running RPNW
                    }
                }
                RunRdy = R_RunRdy;  //Set global run-ready if rotor becomes ready while in RPNW
            }

            break;

    //------------------------ ROTOR CONVERTER RUN REQUEST ---------------------
        case CONV_RUN_REQ_ROTOR:
            LnRunningSt  = false;
            RtRunReqSt   = true;

            if ( TripFlt || !RunCmd)
            {
                SeqSt = CONV_STOP_REQ_ROTOR;
            }
            else
            {
                if ( R_SeqSt == ROTOR_RUNNING)
                {
                    SeqSt    = CONV_RUNNING_ROTOR;
                }
                else
                {
                    R_RunReq = true;
                }
            }

            break;

    //------------------------ ROTOR CONVERTER RUNNING -------------------------
        case CONV_RUNNING_ROTOR:

            RtRunReqSt   = false;
            RPNWRunning  = false;

            if ( TripFlt || !RunCmd)
            {
                if ( !TripFlt && VARModeCmd && RPNWRunCmd)
                {
                    RtrLnIyXfer = true;
                    if ( IyXferDone)
                    {
                        SeqSt = CONV_STOP_REQ_ROTOR;
                    }
                }
                else
                {
                    SeqSt = CONV_STOP_REQ_ROTOR;
                }
            }

            break;

    //------------------------ ROTOR CONVERTER STOP REQUEST --------------------
        case CONV_STOP_REQ_ROTOR:

            RtStpReqSt   = true;
            RtRunReqSt   = false;
            R_RunReq     = false;

            if (R_InnrRegEnb)
            {
                SeqStTmr = 0.0F;
            }
            else
            {
                SeqStTmr = SeqStTmr + DelTm2;
            }

            if (SafetyStopIp && (SeqStTmr > PARM(SftyRtrLnDly)))
            {
                L_RunReq = false;
            }

            if ( L_InnrRegEnb)
            {
                if (RPNWRunCmd)
                {
                    RPNWRunning = true;
                }
            }
            else
            {
                RPNWRunning = false;
            }


            if ( R_SeqSt == ROTOR_STOPPED)
            {
                SeqSt       = CONV_RUNNING_LINE;
                RtrLnIyXfer = false;
                IyXferDone  = false;

            }

            break;

    //----------------------- LINE CONVERTER STOP REQUEST ----------------------
        case CONV_STOP_REQ_LINE:

            LnRunReqSt   = false;
            LnRunningSt  = false;
            RtStpReqSt   = false;
            LnStpReqSt   = true;
            RPNWRunning  = false;

            L_RunReq = false;

            if ( L_SeqSt == LINE_STOPPED)
            {
                SeqSt = STOPPED_COMMON;
            }

            break;

    //----------------------------- INIT_COMMON --------------------------------
        case INIT_COMMON:  // Drive Initialization - wait for Task3 to complete
            if (RunCmd )
            {
                L_RunCmd = false;
                PUSH_DIAG(L_RunWhlInit); // Call warning fault
            }
            if (InitDone)
            {
                PsInrshLimClsCmd = true;
                SeqSt = STOPPED_COMMON;
            }
            break;

        default:    // Invalid Sequencer State
            PUSH_DIAG(BadSeqSt);
            break;
    }   // End of switch(SeqSt)


    if ( LqdPmpMtrEnb == true )  // allows the pump state to change
    {
        LqdPmpMtrCmd = LqdTempOkFlg && (LqdPmpHtrCmd || LqdPmpCnvCmd);
    }
    else
    {
        ;   // Liquid pump motor stay the same
    }

    return;

}   // CoreMainSequencer


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This Zero Voltage Ridethrough function coordinates the shutting down
//      of rotor and line converters during a ZVRT event.
//
// EXECUTION LEVEL
//      Task2 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ZeroVoltageRideThroughSequencer(void)
{
    unsigned  TmrClamp, TmrClamp2;
    unsigned  RunningZVRT;


    RunningZVRT = L_Running && (R_Running || LineRunOnly);
    if ( GmLineVShort &&  RunningZVRT)
    {
        ZVRTQuickIp = true;
        if ( L_VdcFbkT2 < (R_CroBarVdcLvlOffT2-PARM(ZVStopGtVdcCBMgn)) )
        {
            ZVStopGtTmr = ZVStopGtTmr + DelTm2;
            if ( ZVStopGtTmr >= PARM(ZVStopGtTm))
            {
                ZVStopGtTmr = PARM(ZVStopGtTm);
                TmrClamp = true;
            }
            else
            {
                TmrClamp = false;
            }
        }
        else
        {
            ZVStopGtTmr = 0.0;
            TmrClamp    = false;
        }

        ZVStopGtTmr2 = ZVStopGtTmr2 + DelTm2;
        if ( ZVStopGtTmr2 >= PARM(ZVStopGtTm2))
        {
            ZVStopGtTmr2 = PARM(ZVStopGtTm2);
            TmrClamp2 = true;
        }
        else
        {
            TmrClamp2 = false;
        }
    }
    else
    {
        ZVStopGtTmr = 0.0;
        TmrClamp    = false;
        ZVStopGtTmr2 = 0.0;
        TmrClamp2    = false;
        if ( RunningZVRT )
        {
            if ( ZVRTQuickIp && !GmLineVShort )
            {
                ZVQckRcvyTmr = ZVQckRcvyTmr + DelTm2;
                if ( ZVQckRcvyTmr >= PARM(ZVQckRcvyTm) )
                {
                    ZVQckRcvyTmr = 0.0;
                    ZVRTQuickIp  = false;
                }
            }
        }
        else
        {
            ZVRTQuickIp =false;
        }
    }

    if ( TripFlt || !L_Running || (!R_Running && !LineRunOnly) )
    {
        ZVRTGtSt = ZVRT_GATE_NORM;
    }


    switch (ZVRTGtSt)
    {
        case ZVRT_GATE_NORM:// ZVRTGtSt = 0
            ZVRTTmr    = 0.0;
            ZVRcvryTmr = 0.0;
            L_ZVGtOff  = false;
            R_ZVGtOff  = false;
            R_ZVRtrOff = false;
            L_ZVLineOff= false;
            ZVRTIp     = false;

            if (   TmrClamp
                ||(TmrClamp2 &&
                     (L_VdcFbkT2 < (R_CroBarVdcLvlOffT2-PARM(ZVStopGtVdcCBMgn)))
                  )
               )
            {
                NV.ZVRTEvntCtr = NV.ZVRTEvntCtr + 1;
                ZVRTIp         = true;
                ZVRTQuickIp    = false;
                if ( LineRunOnly)
                {
                    L_ZVGtOff  = true;
                    L_ZVLineOff= true;
                    ZVRTGtSt   = ZVRT_LINE_OFFREQ;
                }
                else
                {
                    R_ZVGtOff      = true;
                    ZVRTGtSt       = ZVRT_RTR_OFFREQ;
                }
            }
            break;

        case ZVRT_RTR_OFFREQ:// ZVRTGtSt = 1
            ZVRTTmr    = 0.0;
            ZVRcvryTmr = 0.0;
            L_ZVGtOff  = false;
            R_ZVGtOff  = true;
            R_ZVRtrOff = true; //flag to show rotor power enable may be off

            if ( R_SeqSt == ROTOR_ZVRT_STANDBY )
            {
                L_ZVGtOff  = true;
                L_ZVLineOff= true;
                ZVRTGtSt   = ZVRT_LINE_OFFREQ;
            }
            break;

        case ZVRT_LINE_OFFREQ:// ZVRTGtSt = 2
            ZVRTTmr    = 0.0;
            ZVRcvryTmr = 0.0;
            L_ZVGtOff  = true;
            R_ZVGtOff  = true;
            if ( L_SeqSt == LINE_ZVRT_STANDBY)
            {
                ZVRTGtSt   = ZVRT_WAIT;
            }
            break;

        case ZVRT_WAIT:// ZVRTGtSt = 3
            R_ZVGtOff   = true;
            L_ZVGtOff   = true;

            if ( !GmLineVShort )
            {
                ZVRcvryTmr = ZVRcvryTmr + DelTm2;
            }
            else
            {
                ZVRcvryTmr = 0.0;
            }
            if ( ZVRcvryTmr > PARM(ZVGtOfMxTm) )
            {
                PUSH_DIAG(ZVGtOfTOut);
            }
            else if ( GmPllPrvPrm )
            {
                ZVRTTmr = ZVRTTmr + DelTm2;
                if ( ZVRTTmr > PARM(ZVPllPrmTm) )
                {
                    ZVRTGtSt    = ZVRT_LINE_RUNREQ;
                    ZVRTTmr     = 0.0;
                    ZVRcvryTmr  = 0.0;
                }
            }
            else
            {
                ZVRTTmr = 0.0;
            }
            break;

        case ZVRT_LINE_RUNREQ:// ZVRTGtSt = 4
            ZVRTTmr     = 0.0;
            ZVRcvryTmr  = 0.0;
            R_ZVGtOff   = true;
            L_ZVGtOff   = false;
            if ( L_BrgPwrEnb )
            {
                L_ZVLineOff= false;
            }

            if ( L_SeqSt == LINE_RUNNING )
            {
                if ( LineRunOnly )
                {
                    ZVRTGtSt = ZVRT_GATE_NORM;
                }
                else
                {
                    ZVRTGtSt = ZVRT_LINE_RUNNING;
                }
            }
            break;

        case ZVRT_LINE_RUNNING:// ZVRTGtSt = 5
            L_ZVGtOff  = false;
            R_ZVGtOff  = true;

            ZVRcvryTmr = ZVRcvryTmr + DelTm2;
            if ( ZVRcvryTmr > PARM(ZVGtOfMxTmSt) )
            {
                PUSH_DIAG(ZVGtOfTOutSt);
            }
            else if ( S_PllPrvPrm )
            {
                ZVRTTmr = ZVRTTmr + DelTm2;
                if ( ZVRTTmr > PARM(ZVPllPrmTmSt) )
                {
                    ZVRTGtSt    = ZVRT_RTR_RUNREQ;
                    ZVRTTmr     = 0.0;
                    ZVRcvryTmr  = 0.0;
                }
            }
            else
            {
                ZVRTTmr = 0.0;
            }
            break;

        case ZVRT_RTR_RUNREQ:// ZVRTGtSt = 6
            ZVRTTmr    = 0.0;
            ZVRcvryTmr = 0.0;
            L_ZVGtOff  = false;
            R_ZVGtOff  = false;
            if ( R_BrgPwrEnb )
            {
                R_ZVRtrOff = false;
            }

            if ( R_SeqSt == ROTOR_ENABLE_TORQUE )
            {
                ZVRTGtSt   = ZVRT_RTR_RUNNING;
            }
            break;

        case ZVRT_RTR_RUNNING:// ZVRTGtSt = 7
            ZVRTTmr    = 0.0;
            ZVRcvryTmr = 0.0;
            L_ZVGtOff  = false;
            R_ZVGtOff  = false;
            if ( R_SeqSt == ROTOR_RUNNING )
            {
                ZVRTGtSt   = ZVRT_GATE_NORM;
            }
            break;

        default:
            ZVRTGtSt = ZVRT_GATE_NORM;
            break;
  }

  return;

}   // ZeroVoltageRideThroughSequencer


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Modulation of main DTA fan to regulate coolant temperature and provide
//      air flow to entire assembly.
//
// EXECUTION LEVEL
//      Task3 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void FanDTAControl(void)
{
    // Determine if fan needs to be running
    if ( (CoolntIsHot && LqdPmpMtrCmdT3 && !CoolntHeatOn) || L_OTAlSum
        || R_OTAlSum || ThermisAlmSm || CfcIsHot )
    {
        DTAFanRunReq = true;
    }
    else
    {
        DTAFanRunReq = false;
    }

    // move from LineDBThermalModelT3
    if (L_DbThrmModel.DtmPdisspt > PARM(L_DbFanOnThr))
    {
        L_DbFanReq = true;
    }
    else if (L_DbThrmModel.DtmPdisspt < PARM(L_DbFanOffThr))
    {
        L_DbFanReq = false;
    }

    // Set fan run command accordingly to the request
    // Do not allow state transitions if fan is not enable
    if ( (DTAFanRunReq && (FanPassOff || LqdThrmOpnFlg || LqdThrmDlFlg) && DTAFanEnb) || L_DbFanReq)
    {
        if ((DTAFanRunReq || L_DbFanReq) && !DTAFanCmd)  FanStateTmr = 0.0;
        DTAFanCmd   = true;
        FanPassOff  = false;
    }
    if ( (CoolntIsCool || !LqdPmpMtrCmdT3 || CoolntHeatOn) && !DTAFanRunReq && FanPassOn
        && DTAFanEnb && CfcIsCool && !L_DbFanReq)
    {
        DTAFanCmd = false;
        FanPassOn = false;
    }

    // Keep track of minimum times for each fan state
    if ( !(DTAFanCmd || FanPassOff) )
    {
        FanStateTmr = FanStateTmr + DelTm3;
        if ( FanStateTmr > PARM(FanMinOffTm ))
        {
            FanPassOff   = true;
            FanStateTmr  = 0.0F;
        }
    }
    else if( DTAFanCmd && !FanPassOn )
    {
        FanStateTmr = FanStateTmr + DelTm3;
        if ( FanStateTmr > PARM(FanMinOnTm ))
        {
            FanPassOn   = true;
            FanStateTmr  = 0.0F;
        }
    }

    // Determine if coolant temperature is within acceptable range

    if ( InletAirTemp > PARM(InAirMaxTemp ))
    {
        AirTempRefLv = PARM(InAirMaxTemp);
    }
    else if ( InletAirTemp < PARM(InAirMinTemp ))
    {
        AirTempRefLv = PARM(InAirMinTemp);
    }
    else
    {
        AirTempRefLv = InletAirTemp;
    }


    if ( CoolantTemp > (AirTempRefLv + PARM(CoolDeltaOn) ))
    {
        CoolntIsHot  = true;       //Coolant is hot
        CoolntIsCool = false;
    }
    else if ( CoolantTemp < (AirTempRefLv + PARM(CoolDeltaOff) ))
    {
        CoolntIsCool = true;       //Coolant is cold
        CoolntIsHot  = false;
    }
    else
    {
        CoolntIsCool = CoolntIsHot  = false;  //Coolant is fine no state transition is required
    }

    if ( CfcTemp > PARM(CfcDeltaOn ))
    {
        CfcIsHot  = true;       //CFC is hot
        CfcIsCool = false;
    }
    else if ( CfcTemp < PARM(CfcDeltaOff ))
    {
        CfcIsCool = true;       //CFC is cold
        CfcIsHot  = false;
    }
    else
    {
        CfcIsCool = CfcIsHot  = false;  //CFC is fine no state transition is required
    }

    //Determine operation of Fan 5 based Pdc temperature and cable entry temperature
    if ((PdcTemp > PARM(Q1FanPdcOnThr)) || (PdcCabEntryTemp > PARM(Q1FanCabEntryOnThr)))
    {
        Q1FanCmd = true;
    }
    else if ((PdcTemp < PARM(Q1FanPdcOfThr)) && (PdcCabEntryTemp < PARM(Q1FanCabEntryOfThr)))
    {
        Q1FanCmd = false;
    }

    return;

}       // FanDTAControl


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Setting of control bits to remotely control WEMA heaters. This
//      is effectively the modulation of heating power to provide humidity
//      control under changing ambient and operational conditions.
//
// EXECUTION LEVEL
//      Task3 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void HeatControl(void)
{
    unsigned    AuxVar1, AuxVar2;

    WEMAOnline = LnkOkWEMA && PicChksum; //need equivalent of this
    if ( WEMAOnline || PARM(WemaEnbSimRq) )    // WECA is communicating with PIC in the WEMA
    {
        WemaPicVer  = WEMAPicVersion % PARM(WemaFpgaVerOffset);

        if ( (WemaPicVer != PARM(WemaPicRevCk)) && (PARM(SoftwareIPN) >= DFIG02_1850_5060_E ) && !SimMode)
        {
            PUSH_DIAG(WemaPicVerBad);
        }
        // Determine if inlet air temperature is within acceptable range

        if ( InletAirTemp > PARM(AirHeatrMax ) )
        {
            InlAirForHtr = PARM(AirHeatrMax);
        }
        else if ( InletAirTemp < PARM(AirHeatrMin ) )
        {
            InlAirForHtr = PARM(AirHeatrMin);
        }
        else
        {
            InlAirForHtr = InletAirTemp;
        }

        HeatCtrlOn = true;              // WECA always controlling remotely

        if ( HeaterEnb )               // Heating control is allowed to change states
        {
            // MCC heat off control bit
            if ( (MccTemp - InlAirForHtr) < PARM(MCCHtrOn ) )
            {
                MCCHeatOff = false;
            }
            else if ( (MccTemp - InlAirForHtr) >= PARM(MCCHtrOff ) )
            {
                MCCHeatOff = true;
            }

            // Coolant heat on control bit
            if ( (CoolantTemp - InlAirForHtr) < PARM(CoolntHtrOn ) )
            {
                CoolntHeatOn = true;
            }
            else if ( (CoolantTemp - InlAirForHtr) >= PARM(CoolntHtrOff ) )
            {
                CoolntHeatOn = false;
            }
            CoolntHeatOn = CoolntHeatOn && !CoolantOtDlyAlmFlg;

            // PDC & CFC heat off control bit
            AuxVar1 = PdcThrmOpnFlg || PdcThrmDlFlg;  // PDC thermister reading is not trustworthy
            AuxVar2 = CfcThrmOpnFlg || CfcThrmDlFlg;  // CFC thermister reading is not trustworthy

            if ( AuxVar1 && !AuxVar2 )
            {
                PdcCfcForHtr = CfcTemp;
            }
            else if ( AuxVar2 && !AuxVar1 )
            {
                PdcCfcForHtr = PdcTemp;
            }
            else
            {
                if ( PdcTemp < CfcTemp )        // Choose minimum of PDC and CFC
                {
                    PdcCfcForHtr = PdcTemp;
                }
                else
                {
                    PdcCfcForHtr = CfcTemp;
                }
            }

            if ( (PdcCfcForHtr - InlAirForHtr) < PARM(PdcCfcHtrOn ) )
            {
                PdcCfcHeatOf = false;
            }
            else if ( (PdcCfcForHtr - InlAirForHtr) >= PARM(PdcCfcHtrOff ) )
            {
                PdcCfcHeatOf = true;
            }
        }
    }
    else                       // No communication via IONet
    {
        if (BppbEnable == 0)
        {
            BppbHtCtrlDisabled = true;
        }
        MCCHeatOff = CoolntHeatOn = PdcCfcHeatOf = HeatCtrlOn = false;

        // push alarm if WEMA is not online while running and after power-up
        if ( L_Running && (SysTm3.Sec > PARM(WEMAComDlyTm)) )
        {
            PUSH_DIAG( WEMACommAlm );
        }
    }

    return;

}       // HeatControl


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Forms the sequencer request, RunReq, from
//      pointable inputs or Lan inputs if either are enabled.
//
// EXECUTION LEVEL
//      Task3 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ApplicReqInputs()
{
    unsigned RunReqInput;     //*Temp_Ptr;
    unsigned Stop_Tst = false;
    //unsigned Lan_Stop_Tst = false;

    if ( (int)PARM(StopReqAdr) != LADR_UnusedBool )
    {
        Stop_Tst = true;
    }

    if ( (int)PARM(RunReqAdr) != LADR_UnusedBool )
    {
        RunReqInput = ( gVariable.ReadAsBool(PARM(RunReqAdr)) && !GmFrqVltStopAct);

        if (RunReqInput ^ Req_Z.RunReqEdg)  // If +/- edge detected
        {
            if ( RunReqInput && Init1Sec )
            {
               RunReq = true;     // + edge detected
            }
            // - edge detected & not in momentary start/stop mode
            else if (!Stop_Tst)
            {
                RunReq = false;
            }
        }
        Req_Z.RunReqEdg = RunReqInput;    // Save for next pass
     }
     else if ( Req_Z.RunReqEdg )
     {
         RunReq = false;
         Req_Z.RunReqEdg = false;
     }

     //PRNW mode
     if (!L_LTestCmd )
     {
        RPNWRunReq = gVariable.ReadAsBool(PARM(RPNWRunAdr) );
     }

     if ( RPNWRunReq && !VARModeCmd)
     {
        PUSH_DIAG( RPNWInvalid);
     }


     // If just switched from local mode, throw away 1st detected edges
     if ( LocalMode ^ LclModeEdg )
     {
         RunCmd = false;
     }


    LclModeEdg = LocalMode;  // Update local mode edge detect

    if ( (Stop_Tst && !gVariable.ReadAsBool(PARM(StopReqAdr))) )
    {
        RunCmd  = false;
        StopReq = true;
    }
    else
    {
        StopReq = false;
    }

    return;

}   // ApplicReqInputs

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Forms the scheme to delay stop run command for a certain
//      number of events.
//
// EXECUTION LEVEL
//      Task2 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void StopCmdDelay()
{

     if (GmOVTrpSum
        && !IslandDet
        && R_Running && !RunReq
        )
     {
         Q1OpnReq     = true;
         StpCmdDlyFlg = true;
         PUSH_DIAG(GmOvTrpDiscAlm);
     }
     else
     {
         RunCmd = RunReq;
     }

     if (((((HVACState >= HVAC_HVDET) && (HVACState != HVAC_HVRT_DET))
        || L_DbThrmModel.DtmTr_GridDiscReq || L_DbThrmModel.DtmRes_GridDiscReq
        || DisconWhlRun
        || DisconWhlOv
        || StpCmdDlyFlg)
        && (!((GmSVMtchErSq > GmSVmtchThr) || (ISimMode && !Q1StatSim ) || !S_K1Status )
           )
         )
        && R_Running && !RunReq
        )
     {
        RunCmd = true;
     }
     else if ( !TripFlt)
     {
        RunCmd = RunReq;
     }
     else
     {
        RunCmd = false;
     }

    return;

}   // StopCmdDelay


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Services product specific ADL commands.
//
// EXECUTION LEVEL
//      Called out of x0Adlsvr.
//
// RETURN VALUE
//      Status (valid status codes defined in x0AdlSvr.h)
//
///////////////////////////////////////////////////////////////////////////////
int cCoreCommonAdlSeq::AdlCmdServer
(
    int         Command,        // command code
    int         ParmCount,      // number of parameters in the parmeter buffer
    unsigned*   ParmBuf         // pointer to the parameter buffer
)
{

    switch ( Command )
    {

        //---------------------------------------------------------------------
        // LVRT simulation enable
        case 1:
            if ( PARM(GEvtSimEnb) )
            {
                GEvtReqADLSim = true;
            }
            return 0;

        //---------------------------------------------------------------------
        // stop request
        case 2:
            // always allow local stop
            ReqStop();
            return  0;                  // success


        //---------------------------------------------------------------------
        // run request
        case 3:
            if ( !LocalMode )
            {
                return ADLCMD_RF;       // Request Fail error
            }
            ReqRun();
            return  0;                  // success


        //---------------------------------------------------------------------
        // toggle RPNW run request
        case 4:
            if ( !LocalMode )
            {
                return ADLCMD_RF;       // Request Fail error
            }
            if ( ToggleRpnwRunRequest() != SUCCESS )
            {
                return ADLCMD_RF;
            }
            return  0;                  // success


        //---------------------------------------------------------------------
        // pulse test request
        case 5:
            if (!Stopped)               // Drive Not Stopped error
            {
                return ADLCMD_NS;
            }
            if ( TripFlt )              // Trip Fault present
            {
                return ADLCMD_TF;
            }

            if ( CmdPulseTest() != SUCCESS )
            {

                 return ADLCMD_RF;       // Request Fail error

            }
            return 0;                    // success


        //---------------------------------------------------------------------
        // cell test request
        case 6:
            if ( !Stopped )
            {
                return ADLCMD_NS;       // converter Not Stopped
            }
            if ( TripFlt )
            {
                return ADLCMD_TF;       // Trip Fault present
            }
            if ( CmdCellTest() != SUCCESS )
            {
                return ADLCMD_RF;       // Request Fail error
            }
            return  0;                  // success


        //---------------------------------------------------------------------
        // toggle var mode
        case 7:
            if ( !LocalMode )
            {
                return ADLCMD_RF;       // Request Fail error
            }
            if ( ToggleVARModeRequest() != SUCCESS )
            {
                return ADLCMD_RF;
            }
            return  0;                  // success

        //---------------------------------------------------------------------
        // perform Inductance Test
        case 8:
            if (!Stopped) {             // line converter must be stopped
                return ADLCMD_NS;
            }
            if (TripFlt) {              // Trip fault present
                return ADLCMD_TF;
            }
            if ( CmdIndTest() != SUCCESS )
            {
                return ADLCMD_RF;       // Request Fail error
            };
            return  0;                  // success

        //---------------------------------------------------------------------
        // commands not implemented
        case 9:
            return ADLCMD_NI;           // Not Implemented command


        //---------------------------------------------------------------------
        // abort diagnostic or tune-up tests
        case 29:
            if ( AbortCellTest() != SUCCESS )
            {
                return ADLCMD_RF;       // Request Fail error
            }
            return  0;                  // success

        //---------------------------------------------------------------------
        // Electrical Signal Analysis buffer control
        case 32:
            EsaBuf.Disable();           // Disable ESA buffer
            return 0;

        case 33:
            EsaBuf.Enable();            // Enable ESA buffer
            return 0;



        //---------------------------------------------------------------------
        // all other cmds reserved for framework adl server and should be handled there
        default:
            return ADLCMD_RF;           // request fail error

    }

    //-------------------------------------------------------------------------
    // should never get to here!
    return ADLCMD_RF;                   // request fail error
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Clears RunCmd
//
// EXECUTION LEVEL
//      Called from product Adl command server.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int ReqStop(void)
{
    if ( LocalMode )
    {
        if(PARM(TcSimEnb))
        {
            TcRunCmd = false;
        }
        else
        {
            LclCnvRunReq = false;
        }
    }
        return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Sets RunCmd
//
// EXECUTION LEVEL
//      Called from product Adl command server.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int ReqRun(void)
{
    if ( LocalMode )
    {
        if(PARM(TcSimEnb))
        {
           TcRunCmd = true;
        }
        else
        {
           LclCnvRunReq = true;
        }
    }
    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Toggles RPNWRunReq
//
// EXECUTION LEVEL
//      Called from product Adl command server.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int ToggleRpnwRunRequest(void)
{
    if ( LocalMode )
    {
        if ( !VARModeCmd )
        {
            return FAIL;
        }
        else
        {
            if (PARM(TcSimEnb))
            {
                if (TcRPNWReq == false)
                {
                    TcRPNWReq = true;
                }
                else
                {
                    TcRPNWReq = false;
                }
            }
            else
            {
                if (LclRPNWReq == false)
                {
                    LclRPNWReq = true;
                }
                else
                {
                    LclRPNWReq = false;
                }

            }
        }
    }
    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Toggles Phi/Var mode request
//
// EXECUTION LEVEL
//      Called from product Adl command server.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int ToggleVARModeRequest(void)
{
    if (RPNWRunReq)
    {
        return FAIL;
    }
    else
    {
        VARModeCmd ^= 1;
    }
    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Sets appropriate PlsTstCmd output based on P.PtBrgSel
//
// EXECUTION LEVEL
//      Called from product Adl command server.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int CmdPulseTest(void)
{
    if (SftyChnOpn)
    {
       PUSH_DIAG(SafetyChnOpn);
       return FAIL;
    }
    else if (!Stopped || TripFlt)
    {
        return FAIL;
    }
    else if ( PARM(PtBrgSel) == PT_ROTOR_BRG )
    {
        R_PlsTstCmd = true;       // Request rotor pulse test
        return SUCCESS;
    }
    else if ( PARM(PtBrgSel) == PT_LINE_BRG )
    {
        L_PlsTstCmd = true;       // Request line pulse test
        return SUCCESS;
    }
    else if ( (PARM(PtBrgSel)==PT_DB) &&
              (DbInh==cDbCtl::NO_INHIBIT)
            )
    {
        L_DbPlsTstCmd = true;      // Request DB pulse test
        return SUCCESS;
    }
    else
    {
        return FAIL;
    }

}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Sets appropriate CelTstCmd output based on P.CtBrgSel
//
// EXECUTION LEVEL
//      Called out of x0Adlsvr.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int CmdCellTest(void)
{
      if (SftyChnOpn)
      {
         PUSH_DIAG(SafetyChnOpn);
         return FAIL;
      }
      else if ( (PARM(CtBrgSel) == CT_ROTOR_BRG1))
      {
          R_CelTstCmd = true;       // Request rotor cell test
          return SUCCESS;
      }
      else if (PARM(CtBrgSel) == CT_LINE_BRG)
      {
          L_CelTstCmd = true;       // Request line cell test
          return SUCCESS;
      }
      else if ( (PARM(CtBrgSel) == CT_DB) &&
                (DbInh == cDbCtl::NO_INHIBIT)
              )
      {
          L_DbCelTstCmd = true;     // Request DB cell test
          return SUCCESS;
      }
      else
      {
          return FAIL;
      }
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Clears CelTstCmd's
//
// EXECUTION LEVEL
//      Called out of x0Adlsvr.
//
// RETURN VALUE
//      Status  SUCCESS
//
///////////////////////////////////////////////////////////////////////////////
int AbortCellTest(void)
{
      R_CelTstCmd   = false;
      L_CelTstCmd   = false;
      L_DbCelTstCmd = false;

      return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Set L_LTextCmd
//
// EXECUTION LEVEL
//      Called out of x0Adlsvr.
//
// RETURN VALUE
//      Status  SUCCESS
//
///////////////////////////////////////////////////////////////////////////////
int CmdIndTest(void)
{
    L_LTestCmd = true;           /* request inductor test */
    return SUCCESS;
}
