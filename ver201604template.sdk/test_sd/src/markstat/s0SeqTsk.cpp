///////////////////////////////////////////////////////////////////////////////
//
// TITLE:        Source-side control sequencer for core drive
//
// DESCRIPTION:
//      Line sequencer interface and task routines.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     s0SeqTsk.vsd, s0SeqTsk.xls
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
#include "b0DbThrm.h"
#include "s0SeqTsk.h"
#include "b0DbCelTst.h"


// Externals
//----------
extern unsigned     SimAcclAct;
extern SeqBits1     L_SeqStBits;
extern unsigned     L_SeqReq_PackedBits;

extern float        DelTm2;
extern float        GmLnVmtchErFl;
extern float        GmLVmtchThr;
extern float        GmVllMagAvg;
extern float        L_FilSignTm1;
extern float        L_FilSignTm2;
extern float        L_VdcFbkT2;
extern float        L_VdcNomCal;
extern float        GmQPwrT2;
extern float        GmPwrT2;
extern float        L_PwrLine;
extern float        L_GmVanAl2;
extern float        GmIaAl2;
extern float        GmIaBe2;
extern float        L_GmVbnAl2;
extern float        L_GmVbnBe2;
extern float        GmIbAl2;
extern float        GmIbBe2;
extern float        L_GmVcnAl2;
extern float        L_GmVcnBe2;
extern float        GmIcAl2;
extern float        GmIcBe2;

extern float        GmIGndMag;
extern float        GmPllWT2;
extern float        GmPaT2;
extern float        GmPbT2;
extern float        GmPcT2;

extern unsigned     Brg1TripFlt;
extern unsigned     DTAFanEnb;
extern unsigned     LqdPmpMtrEnb;
extern unsigned     GmPllProven;
extern unsigned     HdwTstMode;
extern unsigned     HeaterEnb;
extern unsigned     L_AcLineLoss;
extern unsigned     L_CelTstCmd;
extern unsigned     L_InitDone;
extern unsigned     L_LTestIp;
extern unsigned     L_PlsTstCmd;

extern unsigned     L_RunCmd;
extern unsigned     L_Running;
extern unsigned     L_SftyChnOpn;
extern unsigned     L_ZVLineOff;
extern unsigned     ParkReq;
extern unsigned     MvSwGearTrpReq;
extern unsigned     SftyChnMaOpn;
extern unsigned     SimMode;
extern unsigned     XSimMode;
extern unsigned     VdcIsOk;
extern unsigned     VgridIsOk;
extern unsigned     IpcInterfaceOk ;
extern unsigned     LocalMode      ;
extern unsigned     L_AcLinePres;
extern unsigned     CCUParmRdy;
extern unsigned     DbRdy;

extern void         LineCoreSequencer              (void);
extern void         LineCoreDriveSequencer         (void);
extern void         InductanceMeasurementSequencer (void);
extern void         InitCoreLineSequencer          (void);

extern FaultIsActiveTyp     FaultIsActive;

// for SequencerTaskInterface
extern unsigned     L_SeqReq_PackedBits;
extern SeqStatBits  L_SeqStat;  // Sequencer Task Status bits
extern SeqActBits   L_SeqAct;   // Sequencer Task Action bits
extern unsigned     L_ParkReq;  // Request for Task Interface to park source
extern unsigned     Brg1TripFlt;    // Bridge1 trip fault
extern unsigned     CCUParmRdy;     // Converter control unit parameters status have been filled
extern unsigned     DbRdy;          // DB is ready to run
extern unsigned     HdwTstMode;     // I/O test mode
extern unsigned     IpcInterfaceOk; // IPC interface is OK
extern unsigned     L_ParkReq;      // Request for Task Interface to park source
extern unsigned     L_Running;      // Lince converter is Running, or in ZVRT standby
extern SeqActBits   L_SeqAct;       // Sequencer Task Action bits
extern unsigned     L_SeqReq_PackedBits;
extern SeqStatBits  L_SeqStat;      // Sequencer Task Status bits
extern unsigned     LocalMode;      // Local  mode
extern unsigned     Q1TripFbk;      // Trip feedback signle from Q1
extern unsigned     VdcIsOk;        // DC link voltage is high enough while stopped so fuses won't fatige during closing of Q1
extern unsigned     VgridIsOk;      // Grid voltage is within tolerances while in the stop state
extern unsigned     L_DbPwrEnb;
extern unsigned     L_DbCelTstCmd;
extern unsigned     L_DbPlsTstCmd;
extern unsigned     L_DbGateDsb;
// for InitSequencer
extern unsigned L_BrgPwrEnb;
extern unsigned L_InnrRegEnb;
extern unsigned L_SftyChnOpn;
extern unsigned L_InitDone;
extern int L_ChgTypeSt;
extern int L_ChgSt;
extern int L_SyncSt;
extern int L_SyncTypeSt;
extern int L_RegEnbSt;
extern int L_EnbTypeSt;
extern unsigned     L_SeqReq_PackedBits;
extern SeqStatBits  L_SeqStat;  // Sequencer Task Status bits
extern SeqActBits   L_SeqAct;   // Sequencer Task Action bits
extern SeqBits1     L_SeqStBits;
// for LinkChargeCheck
extern unsigned     L_SeqReq_PackedBits;
extern SeqStatBits  L_SeqStat;      // Sequencer Task Status bits
extern SeqActBits   L_SeqAct;       // Sequencer Task Action bits
extern unsigned     Brg1TripFltZ1;  // Bridge-1 trip fault indication, delayed one execution task
extern unsigned     L_RegLimHist;   // regulator limit history
extern float        L_VdcFbkT2;     // DC link voltage feedback task2 version
extern float        GmVllMagAvg;    // Gm line to line average votlage
extern unsigned     Q1Status;       // Q1 breaker status
extern unsigned     L_CelTstCmd;    //Cell Test command
extern unsigned     L_PlsTstCmd;    // Pulse Test command
extern unsigned     L_IOffsDsb;     // Current offset disable boolean
extern float        L_IOffsTmr;     // Current offset timer (Sec)
extern float        GmLVmtchErFl;   // Grid monitor - Line voltage error filtered
// for KdfContactorTask
extern unsigned L_DfMode;
extern int L_DfTestIndx;
extern unsigned L_DfPhAIsZero;
extern unsigned L_DfPhBIsZero;
extern unsigned L_DfPhCIsZero;
extern unsigned L_AcLinePres;
extern unsigned Q1Status;
extern unsigned Q1UVHld;
// for SyncInit
extern unsigned     Brg1TripFltZ1;  // Bridge-1 trip fault indication, delayed one execution task
extern SeqActBits   L_SeqAct;       // Sequencer Task Action bits
extern unsigned     L_SeqReq_PackedBits;
extern SeqStatBits  L_SeqStat;      // Sequencer Task Status bits
extern unsigned     L_IOffsDsb;     // Current offset disable boolean
extern float        L_IOffsTmr;     // Current offset timer (Sec)
extern unsigned     GmLineFltEnb;   // Ac Gm transient fault enable flag
extern unsigned     GmPllProven;    // Pll proven
// for RegEnable
extern SeqActBits   L_SeqAct;   // Sequencer Task Action bits
extern unsigned     L_SeqReq_PackedBits;
extern SeqStatBits  L_SeqStat;  // Sequencer Task Status bits
extern unsigned     Brg1TripFltZ1;  // Bridge-1 trip fault indication, delayed one execution task
extern unsigned     L_IOffsDsb; // Current offset disable boolean
extern unsigned     L_IOffsVld; // Current offset valid boolean
extern unsigned     L_LTestIp;  // Inductance test in progress
extern unsigned     L_ZVLineOff;// Line zero voltage rotor off
extern float        L_IOffsTmr; // Current offset timer (Sec)
// for L_Q1BreakerTask
extern unsigned Q1OpnReq;
extern unsigned SftyChnQ1Opn;
extern unsigned L_PlsTstCmd;
extern unsigned L_CelTstCmd;
extern unsigned Brg1TripFltZ1;
extern unsigned L_DfMode;
extern int L_DfTestIndx;
extern unsigned L_DfPhAIsZero;
extern unsigned L_DfPhBIsZero;
extern unsigned L_DfPhCIsZero;
extern unsigned L_DfPhA0IsZero;
extern unsigned L_DfPhB0IsZero;
extern unsigned L_DfPhC0IsZero;
extern unsigned L_DfPhA1IsZero;
extern unsigned L_DfPhB1IsZero;
extern unsigned L_DfPhC1IsZero;
extern unsigned L_DfPhA2IsZero;
extern unsigned L_DfPhB2IsZero;
extern unsigned L_DfPhC2IsZero;
extern unsigned L_DfPhAIsBad;
extern unsigned L_DfPhBIsBad;
extern unsigned L_DfPhCIsBad;
extern unsigned L_DfPhA0IsBad;
extern unsigned L_DfPhB0IsBad;
extern unsigned L_DfPhC0IsBad;
extern unsigned L_DfPhA1IsBad;
extern unsigned L_DfPhB1IsBad;
extern unsigned L_DfPhC1IsBad;
extern unsigned L_DfPhA2IsBad;
extern unsigned L_DfPhB2IsBad;
extern unsigned L_DfPhC2IsBad;
extern float LineLnSeqFil;
extern unsigned S_K1OpnNowReq;
// for SourceCtrlSequencer
extern unsigned Brg1TripFltZ1;// Bridge-1 trip fault indication, delayed one execution task
extern unsigned L_Running;// Line converter is Running, or in ZVRT standby
extern unsigned Brg1TripFlt;// Bridge1 trip fault
// for DistFiltPhCurr
extern float GmQPwr;
extern float GmVanAl2;
extern float GmVanBe2;
extern float GmVbnAl2;
extern float GmVbnBe2;
extern float GmVcnAl2;
extern float GmVcnBe2;
extern float GmIaAl2;
extern float GmIaBe2;
extern float GmIbAl2;
extern float GmIbBe2;
extern float GmIcAl2;
extern float GmIcBe2;
extern unsigned L_PlsTstCmd;
extern unsigned L_CelTstCmd;
extern unsigned L_AcLineLoss;
// for SeqDemodulateLine
extern float        GmVxFbkT2;
extern float        GmVyFbkT2;
extern float        L_VxFbkT2;
extern float        L_VyFbkT2;

extern unsigned     FchkCcontOpn;
extern unsigned     IpcInterfaceDown;

// Constants
//----------
// Q1 contactor states
#define Q1_IDLE      ( 0)
#define Q1_ENB_1     ( 1)
#define Q1_ENB_2     ( 2)
#define Q1_ENB_3     ( 3)
#define Q1_ENB_4     ( 4)
#define Q1_DSB_1     (-1)
#define Q1_DSB_2     (-2)
#define Q1_DSB_3     (-3)

// for SequencerTaskInterface
// for InitSequencer
// for LinkChargeCheck
// for KdfContactorTask
// for SyncInit
// for RegEnable
// for L_Q1BreakerTask
// for SourceCtrlSequencer
// for DistFiltPhCurr
// for SeqDemodulateLine

// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(L_DfPfilW,          float);  // Active power low pass filter response for Dist-Filter verification (Rad/Sec)

// for SequencerTaskInterface
CREATE_PARM( L_IOffVldTm,   float     );   // Current offset valid time
CREATE_PARM( L_IOffDsbTm,   float     );   // Current offset disable time
// for InitSequencer
// for LinkChargeCheck
CREATE_PARM(L_ChgLvlAc,     float); // DC link charge level based on ac line (Pu)
CREATE_PARM(L_ChgLvlDc,     float); // DC link charge level based on nominal dc (Pu)
CREATE_PARM(L_ChgDlyTm,     float); // Dc bus charging delay
CREATE_PARM(L_ChgLvlDcDB,   float); // Parameter for dynamic value change
CREATE_PARM(L_ChgLvlAcDB,   float); // Parameter for dynamic value change
CREATE_PARM(L_ChgLvlDcCM,   float); // Parameter for dynmic value change during capacitance test
// for KdfContactorTask
CREATE_PARM(KdfClOpTm,          float); // KDF open and close delay time
CREATE_PARM(KdfFiltTm,          float); // Delay to allow low-pass fiters to settle
CREATE_PARM(CContOpnTm,         float);  // Charge contactor open and close delay time
// for SyncInit
// for RegEnable
CREATE_PARM( L_EnbDlyTm,    float     );   // Enable delay time
// for L_Q1BreakerTask
CREATE_PARM(Q1SprChrgTm,    float);
CREATE_PARM(Q1UVPuDly,      float);
CREATE_PARM(Q1OpenTypTm,    float);
CREATE_PARM(Q1OpnDlyTm,     float);
CREATE_PARM(Q1ClsDlyTm,     float);
CREATE_PARM(Q1VlThrs,       float);
// for SourceCtrlSequencer
// for DistFiltPhCurr
CREATE_PARM(L_DfQfilW,          float);  // Reactive power low pass filter response for Dist-Filter verification (Rad/Sec)
CREATE_PARM(L_DfQPwrNomThrs,    float);  // Reactive power tolerance around nominal for detecting distortion filter is close
CREATE_PARM(L_DfQPwrDlyTm,      float);  // Time delay to check that reactive power reaches minimun-nominal level (Sec)
CREATE_PARM(L_DfQPwrLowThrs,    float);  // Reactive power tolerance for detecting distortion filter is open
// for SeqDemodulateLine
CREATE_PARM(L_Q1DeltVoltW,      float);  // Filter omega for square voltage difference across MA (Rad/Sec)

// Diagnostics
//------------



// for SequencerTaskInterface
// for InitSequencer
// for LinkChargeCheck
CREATE_DIAG(L_LinkChgFlt);      // Dc link charger fault, used in 1.6e
CREATE_DIAG(L_BadChgTsk);       // Bad sequencer dc link charger task call
CREATE_DIAG(L_ChgSyncFlt);      // Line monitor through charger
// for KdfContactorTask
CREATE_DIAG(L_BadKdfTsk);               // Bad sequencer KDF contactor task call
CREATE_DIAG(KdfOpnTmOut);               // KDF did not open on time
CREATE_DIAG(KdfClsTmOut);               // KDF did not close on time
CREATE_DIAG(KdfFbkBad);                 // KDF contactor feedback is incorrect
CREATE_DIAG(LineConnFlt);               // Line failed to disconnect from grid
// for SyncInit
CREATE_DIAG(L_SyncIniTm);         // 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0   Pll synce init. timeout
CREATE_DIAG(L_BadSyncTsk);               // Bad sequencer sync task call
// for RegEnable
CREATE_DIAG(L_BadEnbTsk);                // Bad sequencer reg enable task call
// for L_Q1BreakerTask
CREATE_DIAG(L_BadQ1Tsk);
CREATE_DIAG(GLVxyImbFlt);
CREATE_DIAG(L_BrdgOpen);
CREATE_DIAG(DFiltPhAOpen);
CREATE_DIAG(DFiltPhAFlt);
CREATE_DIAG(DFiltPhBOpen);
CREATE_DIAG(DFiltPhBFlt);
CREATE_DIAG(DFiltPhCOpen);
CREATE_DIAG(DFiltPhCFlt);
CREATE_DIAG(Q1OpnTmOut);
CREATE_DIAG(LinePhAOpen);
CREATE_DIAG(LinePhBOpen);
CREATE_DIAG(LinePhCOpen);
// for SourceCtrlSequencer
// for DistFiltPhCurr
// for SeqDemodulateLine

// Variables
//----------
CREATE_PUBVAR(DFiltAuxTmr,      float);  // Distortion filter auxiliary timer (Sec)
CREATE_PUBVAR(CContOpnTmr,      float);  // CCont auxiliary timer (Sec)




//CREATE_PUBVAR(AcLineIp,           int);  // Ac line voltage is developing



CREATE_PUBVAR(L_DfPwrgain,      float);  // Gain for filtering dist-filter active power
CREATE_PUBVAR(L_DfQpwrgain,     float);  // Gain for filtering dist-filter reactive power







CREATE_PUBVAR(Brg1TripFltZ1,  unsigned); // Bridge-1 trip fault indication, delayed one execution task

//CREATE_PUBVAR(L_AcLinePres,   unsigned); // Ac line present signal

CREATE_PUBVAR(L_ParkReq,      unsigned); // Request for Task Interface to park source
CREATE_PUBVAR(L_RunRdy,       unsigned); // Source is ready to run
CREATE_PUBVAR(L_SpinRdy,      unsigned); // Source is ready to spin
CREATE_PUBVAR(DTAFanEnb,      unsigned) = true; // DTA fan control enable to change state
CREATE_PUBVAR(HeaterEnb,      unsigned) = true; // Heating control enable to change state
CREATE_PUBVAR(LqdPmpMtrEnb,   unsigned) = true; // Pump motor enable to change state

CREATE_PUBVAR(L_MaClsCmd,     unsigned); // Main Contactor close command
CREATE_PUBVAR(L_MaStatus,     unsigned); // Main Contactor status
CREATE_PUBVAR(L_MaStatusT3,   unsigned); // Main Contactor status in task3
CREATE_PUBVAR(L_CcstrClsCmd,  unsigned); // Dc-link Charger Contactor String close command

CREATE_PUBVAR(L_CelTstDisab,  unsigned); // Disable cell test operation




CREATE_PUBVAR(TempOk,         unsigned); // Temperature OK - intermediate signal for Line Sequence

// for SequencerTaskInterface
CREATE_PUBVAR(L_Stopped,      unsigned); // Line sequencer stop state
CREATE_PUBVAR(L_StoppedT3,    unsigned); // Line sequencer stop state in task3
CREATE_PUBVAR(L_DiagMode,     unsigned); // Line sequencer diagnostic-mode state
CREATE_PUBVAR(L_DiagModeT3,   unsigned); // Line sequencer diagnostic-mode state in task3
CREATE_PUBVAR(L_IOffsDsb,     unsigned); // Current offset disable boolean
CREATE_PUBVAR(L_IOffsEnb,     unsigned); // Current offset enable boolean
CREATE_PUBVAR(L_IOffsVld,     unsigned); // Current offset valid boolean
CREATE_PUBVAR(L_IOffsTmr,       float);  // Current offset timer (Sec)
CREATE_PUBVAR(L_DbTrTmpRdyT2,   unsigned);  // DB transistor temperature ready in task2 version
// for InitSequencer
// for LinkChargeCheck
CREATE_PUBVAR(L_ChgSt,      int);   // Dc link charger task state variable
CREATE_PUBVAR(L_ChgTypeSt,  int);   // Dc link charger task-type state variable
CREATE_PUBVAR(L_ChgDlyTmr,  float); // Syncronization delay timer (Sec)
// for KdfContactorTask
CREATE_PUBVAR(KdfTypeSt,     unsigned); // Distortion filter type state variable
CREATE_PUBVAR(L_KdfTskSt,         int); // Distortion filter state variable
CREATE_PUBVAR(KdfClsCmd,     unsigned); // KDF Contactor close command
CREATE_PUBVAR(KdfClsFbk,     unsigned); // KDF Contactor feedback
CREATE_PUBVAR(KdfQPwrTm,        float); // Mapping to P.KDFFiltTm
CREATE_PUBVAR(KdfStatus,     unsigned); // Mapping to L_SeqStat.L_KDFEnbStat
CREATE_PUBVAR(CContOpnCmd,   unsigned); // Charge Contactor Open Command
// for SyncInit
CREATE_PUBVAR(L_SyncSt,           int);  // PLL synchronization task state
CREATE_PUBVAR(L_SyncTypeSt,       int);  // PLL synchronization type state
CREATE_PUBVAR(L_SyncDlyTmr,     float);  // Syncronization delay timer (Sec)
// for RegEnable
// for L_Q1BreakerTask
CREATE_PUBVAR(Q1RunPrm,     unsigned);
CREATE_PUBVAR(Q1ChargTmr,   float);
CREATE_PUBVAR(Q1Status,     unsigned);  // Q1 breaker status
CREATE_PUBVAR(Q1ClsPulse,   unsigned);
CREATE_PUBVAR(Q1ClsIp,      unsigned);
CREATE_PUBVAR(L_AcLinePres, unsigned);
CREATE_PUBVAR(AcLineIp,     unsigned);
CREATE_PUBVAR(Q1TypeSt,     unsigned);
CREATE_PUBVAR(L_Q1TskSt,    int);
CREATE_PUBVAR(Q1TripFbk,    unsigned);
CREATE_PUBVAR(L_Q1TskTmr,   float);
CREATE_PUBVAR(Q1UVHld,      unsigned);
CREATE_PUBVAR(Q1ShuntTrip,  unsigned);
CREATE_PUBVAR(Q1StatSim,    unsigned);
// for SourceCtrlSequencer
// for DistFiltPhCurr
CREATE_PUBVAR(L_DfMode,       unsigned); // Dist-filter test mode handshaking
CREATE_PUBVAR(L_DfModeZ1,     unsigned); // One task3 delayed dist-filter test mode handshaking
CREATE_PUBVAR(L_DfTestIndx,        int); // Dist-filter test index (from -1 to 2)
CREATE_PUBVAR(L_DfTestIndxZ1,      int); // One task3 delayed dist-filter index
DistFilPhSta  L_DfPhA[DFMAXTESTNUMBER];  // Distortion filetr phase-A reactive pwr status
DistFilPhSta  L_DfPhB[DFMAXTESTNUMBER];  // Distortion filetr phase-B reactive pwr status
DistFilPhSta  L_DfPhC[DFMAXTESTNUMBER];  // Distortion filetr phase-C reactive pwr status
CREATE_PUBVAR(L_DfQPwrTmr,       float); // Distortion filter power timer (Sec)
CREATE_PUBVAR(L_DfQPwrGn,        float); // Gain for filtering dist-filter reactive power
CREATE_PUBVAR(L_DfPhAIsZero, unsigned); // Dist filter test #  phs-A current is zero
CREATE_PUBVAR(L_DfPhAIsBad,  unsigned); // Dist filter test #  phs-A failed
CREATE_PUBVAR(L_DfPhAQPwrAux,   float); // Dist filter test #  phs-A auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhAQPwr,      float); // Dist filter test #  phs-A reactive power (Vars)
CREATE_PUBVAR(L_DfPhBIsZero, unsigned); // Dist filter test #  phs-B current is zero
CREATE_PUBVAR(L_DfPhBIsBad,  unsigned); // Dist filter test #  phs-B failed
CREATE_PUBVAR(L_DfPhBQPwrAux,   float); // Dist filter test #  phs-B auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhBQPwr,      float); // Dist filter test #  phs-B reactive power (Vars)
CREATE_PUBVAR(L_DfPhCIsZero, unsigned); // Dist filter test #  phs-C current is zero
CREATE_PUBVAR(L_DfPhCIsBad,  unsigned); // Dist filter test #  phs-C failed
CREATE_PUBVAR(L_DfPhCQPwrAux,   float); // Dist filter test #  phs-C auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhCQPwr,      float); // Dist filter test #  phs-C reactive power (Vars)
CREATE_PUBVAR(L_DfPhA0IsZero, unsigned); // Dist filter test # 0 phs-A current is zero
CREATE_PUBVAR(L_DfPhA0IsBad,  unsigned); // Dist filter test # 0 phs-A failed
CREATE_PUBVAR(L_DfPhA0QPwrAux,   float); // Dist filter test # 0 phs-A auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhA0QPwr,      float); // Dist filter test # 0 phs-A reactive power (Vars)
CREATE_PUBVAR(L_DfPhA1IsZero, unsigned); // Dist filter test # 1 phs-A current is zero
CREATE_PUBVAR(L_DfPhA1IsBad,  unsigned); // Dist filter test # 1 phs-A failed
CREATE_PUBVAR(L_DfPhA1QPwrAux,   float); // Dist filter test # 1 phs-A auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhA1QPwr,      float); // Dist filter test # 1 phs-A reactive power (Vars)
CREATE_PUBVAR(L_DfPhA2IsZero, unsigned); // Dist filter test # 2 phs-A current is zero
CREATE_PUBVAR(L_DfPhA2IsBad,  unsigned); // Dist filter test # 2 phs-A failed
CREATE_PUBVAR(L_DfPhA2QPwrAux,   float); // Dist filter test # 2 phs-A auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhA2QPwr,      float); // Dist filter test # 2 phs-A reactive power (Vars)
CREATE_PUBVAR(L_DfPhB0IsZero, unsigned); // Dist filter test # 0 phs-B current is zero
CREATE_PUBVAR(L_DfPhB0IsBad,  unsigned); // Dist filter test # 0 phs-B failed
CREATE_PUBVAR(L_DfPhB0QPwrAux,   float); // Dist filter test # 0 phs-B auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhB0QPwr,      float); // Dist filter test # 0 phs-B reactive power (Vars)
CREATE_PUBVAR(L_DfPhB1IsZero, unsigned); // Dist filter test # 1 phs-B current is zero
CREATE_PUBVAR(L_DfPhB1IsBad,  unsigned); // Dist filter test # 1 phs-B failed
CREATE_PUBVAR(L_DfPhB1QPwrAux,   float); // Dist filter test # 1 phs-B auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhB1QPwr,      float); // Dist filter test # 1 phs-B reactive power (Vars)
CREATE_PUBVAR(L_DfPhB2IsZero, unsigned); // Dist filter test # 2 phs-B current is zero
CREATE_PUBVAR(L_DfPhB2IsBad,  unsigned); // Dist filter test # 2 phs-B failed
CREATE_PUBVAR(L_DfPhB2QPwrAux,   float); // Dist filter test # 2 phs-B auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhB2QPwr,      float); // Dist filter test # 2 phs-B reactive power (Vars)
CREATE_PUBVAR(L_DfPhC0IsZero, unsigned); // Dist filter test # 0 phs-C current is zero
CREATE_PUBVAR(L_DfPhC0IsBad,  unsigned); // Dist filter test # 0 phs-C failed
CREATE_PUBVAR(L_DfPhC0QPwrAux,   float); // Dist filter test # 0 phs-C auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhC0QPwr,      float); // Dist filter test # 0 phs-C reactive power (Vars)
CREATE_PUBVAR(L_DfPhC1IsZero, unsigned); // Dist filter test # 1 phs-C current is zero
CREATE_PUBVAR(L_DfPhC1IsBad,  unsigned); // Dist filter test # 1 phs-C failed
CREATE_PUBVAR(L_DfPhC1QPwrAux,   float); // Dist filter test # 1 phs-C auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhC1QPwr,      float); // Dist filter test # 1 phs-C reactive power (Vars)
CREATE_PUBVAR(L_DfPhC2IsZero, unsigned); // Dist filter test # 2 phs-C current is zero
CREATE_PUBVAR(L_DfPhC2IsBad,  unsigned); // Dist filter test # 2 phs-C failed
CREATE_PUBVAR(L_DfPhC2QPwrAux,   float); // Dist filter test # 2 phs-C auxiliary reactive power (Vars)
CREATE_PUBVAR(L_DfPhC2QPwr,      float); // Dist filter test # 2 phs-C reactive power (Vars)
// for SeqDemodulateLine

// for RegEnable
CREATE_PUBVAR(L_RegEnbSt,   int);           // Regulator enable state variable
CREATE_PUBVAR(L_EnbTypeSt,  int);           // Regulator enb type state variable
CREATE_PUBVAR(L_EnbDlyTmr,  float);         // Enable delay timer (Sec)
CREATE_PUBVAR(L_InnrRegEnb, unsigned);      // Inner regulator enable boolean
CREATE_PUBVAR(L_BrgPwrEnb,  unsigned);      // Bridge enable boolean
CREATE_PUBVAR(L_BrgPwrEnbT1,    unsigned);  // Task 1 version of line bridge enable boolean
CREATE_PUBVAR(L_InnrRegEnbT1,   unsigned);  // Task 1 version of line inner regulator enable boolean
CREATE_PUBVAR(L_InnrRegEnbT3,   unsigned);  // Task 3 version of line inner regulator enable boolean
CREATE_PUBVAR(GmLineFltEnb  , unsigned ) ;           // Ac Gm transient fault enable flag

// for SeqDemodulateLine
CREATE_PUBVAR(LineLnSeqFil,     float); // Filtered squared voltage difference across Q1 (Volt^2)
CREATE_PUBVAR(LineLnVdifSq,     float); // Squared voltage difference across Q1 (Volt^2)
CREATE_PUBVAR(L_Q1DeltVoltGn,   float);  // Gain for filtering squared voltage difference across Q1

// Unpublished Variables
//----------------------
SeqActBits    L_SeqAct;                   // Sequencer Task Action bits
SeqStatBits   L_SeqStat;                  // Sequencer Task Status bits

// Local Prototypes (to resolve forward references)
//-------------------------------------------------
void   SeqDemodulateLine  (void);
void   DistFiltPhCurr     (void);


// Data Passing
//--------------
DATA_PASS(L_MaStatus,                 L_MaStatusT3,   T2_T3, UNFILTERED);
// for SequencerTaskInterface
DATA_PASS(L_Stopped,                  L_StoppedT3,    T2_T3, UNFILTERED);
DATA_PASS(L_DiagMode,                 L_DiagModeT3,   T2_T3, UNFILTERED);
DATA_PASS(L_DbThrmModel.DtmTrTmpRdy,    L_DbTrTmpRdyT2, T3_T2,  UNFILTERED);
// for InitSequencer
// for LinkChargeCheck
// for KdfContactorTask
// for SyncInit
// for RegEnable
DATA_PASS(L_BrgPwrEnb,      L_BrgPwrEnbT1,         T2_T1, UNFILTERED);
DATA_PASS(L_InnrRegEnb      , L_InnrRegEnbT1        , T2_T1, UNFILTERED);
DATA_PASS(L_InnrRegEnb      , L_InnrRegEnbT3        , T2_T3, UNFILTERED);
// for L_Q1BreakerTask
// for SourceCtrlSequencer
// for DistFiltPhCurr
// for SeqDemodulateLine

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Interface from local sequencer or ACL to sequencer tasks.
//      For the source control, the interface  handles the current
//      offset enable/disable scheme as well as population of variables
//      needed for keypad status information.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void SequencerTaskInterface()
{
    int     *L_SeqReq_Ptr;
    int     *L_SeqStat_Ptr;
    int     *L_SeqAct_Ptr;
    int      tempReq;
    int      tempStat;


    L_SeqReq_Ptr  = (int*) &L_SeqReq_PackedBits;
    L_SeqStat_Ptr = (int*) &L_SeqStat;
    L_SeqAct_Ptr  = (int*) &L_SeqAct;

    // Exclude Dc link charger bits
    tempReq  = *L_SeqReq_Ptr  & 0xFFDF;
    tempStat = *L_SeqStat_Ptr & 0xFFDF;

      // Park source
    if ( L_ParkReq)
    {
        L_SeqStBits.Stopped = false;
        L_SeqStBits.Parked  = true;     // Source is parked
    }
    else
    {
        // Current offset sequencing
        if (L_IOffsDsb)
        {
            L_IOffsTmr = L_IOffsTmr + DelTm2;
            if (L_IOffsTmr >= PARM(L_IOffDsbTm))
            {
                L_IOffsDsb = false;
                L_IOffsTmr = 0.0;
            }
        }

        if ((tempReq == 0) && (tempStat ==0) && (*L_SeqAct_Ptr == 0) && !L_IOffsDsb)
        {
            L_IOffsEnb = true;
            L_IOffsTmr = L_IOffsTmr + DelTm2;
            if (L_IOffsTmr >= PARM(L_IOffVldTm))
            {
                L_IOffsTmr = PARM(L_IOffVldTm);
                L_IOffsVld = true;
            }
        }
        else
        {
            L_IOffsEnb = false;
        }

        // Set diagnostic mode status bits
        if (L_SeqReq.L_PlsTstReq)
        {
            L_SeqStBits.L_PulseTest = true;
            L_SeqStBits.L_DiagMode  = true;
        }
        else if (L_SeqReq.L_DbPsTstReq)
        {
            L_SeqStBits.L_DbPsTest = true;
            L_SeqStBits.L_DiagMode = true;
        }
        else if (L_SeqReq.L_CelTstReq)
        {
            L_SeqStBits.L_CellTest = true;
            L_SeqStBits.L_DiagMode = true;
        }
        else if (L_SeqReq.L_DbClTstReq)
        {
            L_SeqStBits.L_DbClTest   = true;
            L_SeqStBits.L_DiagMode   = true;
        }

        if (L_SeqStBits.L_PulseTest)
        {
           if (!L_SeqReq.L_PlsTstReq && !L_SeqAct.L_PlsTstAct && !L_SeqStat.L_PlsTstStat)
           {
                L_SeqStBits.L_PulseTest = false;
                L_SeqStBits.L_DiagMode  = false;
           }
        }
        else if (L_SeqStBits.L_DbPsTest)
        {
           if (!L_SeqReq.L_DbPsTstReq && !L_SeqAct.L_DbPsTstAct && !L_SeqStat.L_DbPsTstSta)
           {
                L_SeqStBits.L_DbPsTest   = false;
                L_SeqStBits.L_DiagMode   = false;
           }
        }
        else if (L_SeqStBits.L_CellTest)
        {
           if (!L_SeqReq.L_CelTstReq && !L_SeqAct.L_CelTstAct && !L_SeqStat.L_CelTstStat)
           {
                L_SeqStBits.L_CellTest = false;
                L_SeqStBits.L_DiagMode = false;
           }
        }
        else if (L_SeqStBits.L_DbClTest)
        {
           if (!L_SeqReq.L_DbClTstReq && !L_SeqAct.L_DbClTstAct && !L_SeqStat.L_DbClTstSta)
           {
                L_SeqStBits.L_DbClTest   = false;
                L_SeqStBits.L_DiagMode   = false;
           }
        }

        if ( (!L_RunCmd && !L_DbPwrEnb) || L_SeqStBits.L_PulseTest ||
              L_SeqStBits.L_DbPsTest || L_SeqStBits.L_CellTest)
        {
            L_SeqStBits.Stopped = true;
            GmLineFltEnb        = false;
        }
        else
        {
            L_SeqStBits.Stopped = false;
        }

        //=================================================================
        // Ready Logic
        // Note:  This L_RunRdy string was originally created for display purposes only.
        // On May 9, 2000, it was incorporated into the actual L_RunReq logic in the
        // P3SSEQ.C30.  From that time on, a change made to the L_RunRdy logic here will
        // directly effect the L_RunReq logic.  The effect of the L_RunRdy Logic below is
        // not seen by the sequencer until the next pass of task2, so TripFltBrg1 is also
        // incorporated in the logic there (redundant, but faster).
        //---------------------------------------------------------
        // Intermediate signal on behalf of Line Sequence
        TempOk = true;

        //---------------------------------------------------------
        // Line SpinRdy & RunRdy Logic
        DbRdy=  ( !L_DbGateDsb  || !PARM(L_BrgDbEnb) ) &&
                ( ( L_DbTrTmpRdyT2|| !PARM(L_BrgDbEnb) ) || L_DbCelTstCmd || L_DbPlsTstCmd );

        L_SpinRdy =    !Brg1TripFlt
                       &&  VdcIsOk
                       &&  DbRdy
                       &&  TempOk
                       &&  VgridIsOk
                       && !Q1TripFbk
                       && !HdwTstMode
                       && ( (!IpcInterfaceDown && CCUParmRdy) || LocalMode );

        L_RunRdy =      L_SpinRdy
                        && !L_SftyChnOpn
                        &&  L_IOffsVld
                        && !Q1TripFbk;
        //=================================================================

    }   // if (!L_SeqStBits.Parked)

    // Publish line sequencer states
    L_Stopped  = L_SeqStBits.Stopped;
    L_DiagMode = L_SeqStBits.L_DiagMode;

    return;

}   // SequencerTaskInterface



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Initializes source control sequencer.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void InitSequencer()
{
    static float  Seq_Dly_Tm;  // Time stamp used by sequencer tasks
    int *tmp;                  // temp variable used to initialize structures

    InitCoreLineSequencer();

    L_BrgPwrEnb  = false;

    L_DbPwrEnb   = false;
    L_InnrRegEnb = false;
    L_SftyChnOpn = true;
    L_InitDone   = false;    // Set once Task 3 has completed
    Q1UVHld      = false;
    Q1ShuntTrip  = true;

    // Sequencer private variable initializations
    Seq_Dly_Tm   = 0.0F;
    L_ChgTypeSt  = 0;
    L_ChgSt      = 0;
    L_SyncSt     = 0;
    L_SyncTypeSt = 0;
    L_RegEnbSt   = 0;
    L_EnbTypeSt  = 0;

    L_SeqReq_PackedBits= 0;             // Set pointer to address of L_SeqReq
    tmp = (int*) &L_SeqStat;            // Set pointer to addr of L_SeqStat
    *tmp = 0;                           // Zero L_SeqStat
    tmp = (int*) &L_SeqAct;             // Set pointer to addr of L_SeqAct
    *tmp = 0;                           // Zero L_SeqStat
    tmp = (int*) &L_SeqStBits;          // Set ptr to address of L_SeqStBits
    *tmp = 0;                           // Zero L_SeqStBits

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This function verifies Dc-bus voltage level provided by the charger.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LinkChargeCheck()
{

    L_ChgTypeSt = (L_SeqReq.L_ChgEnbReq)   |
                  (L_SeqStat.L_ChgEnbStat<<1) |
                  (L_SeqAct.L_ChgEnbAct<<2);

    switch ( L_ChgTypeSt)       // state selection
    {
      case 0:
      case 3:
        L_ChgSt = 0;         // do nothing state
        break;
      case 1:
      case 7:
        if (Brg1TripFltZ1)
        {
            PUSH_DIAG(L_BadChgTsk);
            L_ChgSt = 0;  // null state
        }
        else
        {
            L_ChgSt = 1;   // 1st enable state
        }
        break;
      case 2:
      case 4:                 // 1st disable state
        if ( L_ChgSt >= -1)
        {
           L_ChgSt = -1;
        }
        break;
      case 5:             // continue with selected state
        if (Brg1TripFltZ1)
        {
            PUSH_DIAG(L_BadChgTsk);
            L_ChgSt = -1;     // 1st disable state
        }
      case 6:
        break;              // no change
      default:
        break;
    }


    switch ( L_ChgSt)
    {
      case 0:       // No action case
        break;
      case 1:              // Case 1: First Charger Enable State
        L_SeqAct.L_ChgEnbAct = true;
        L_RegLimHist         = 0;
        L_ChgSt              = 2;
        //ZJ, 20140320, init L_ChgDlyTmr as 2.75
        L_ChgDlyTmr          = 0.0F;

        if (PARM(L_BrgDbEnb))
        {
            L_DbPwrEnb   = true;
        }

        break;
      case 2:              // Case 2: Verify Dc-bus voltage level
        //Wait L_ChgDlyTm seconds before pushing fault if Dc voltage is low
        //Test Vdc against Grid voltage to verify charger
        //Test Vdc against nominal Vdc to ensure low capacitor surge current when closing Q1
        L_ChgDlyTmr     = L_ChgDlyTmr + DelTm2;

        if( (L_VdcFbkT2 >= (PARM(L_ChgLvlAc)*SQRT2*GmVllMagAvg)) && (L_VdcFbkT2 >= PARM(L_ChgLvlDc)*L_VdcNomCal) )
        {
            L_ChgSt       = 3;
            L_ChgDlyTmr   = 0.0F;
        }
        else if ( L_ChgDlyTmr > PARM(L_ChgDlyTm) )
        {
            PUSH_DIAG(L_LinkChgFlt);
        }
        break;

      case 3:              // Case 3: Verify Pll through charger

        //Variable L_AcLinePres turns On simulated line voltages, simulated Dc bus, and Task 2 Line-Monitor protections.

        if ( !Q1Status && !(L_CelTstCmd || L_PlsTstCmd) )
        {
            if (GmLnVmtchErFl < GmLVmtchThr)
            {
                L_ChgSt = 4;
                L_ChgDlyTmr = 0.0F;
            }
            else
            {
                PUSH_DIAG(L_ChgSyncFlt);
            }
        }
        else L_ChgSt = 4;
        break;
      case 4:              // Case 4: Last Charger Enable State
        L_SeqStat.L_ChgEnbStat = true;
        L_SeqAct.L_ChgEnbAct   = false;
        break;

      case -1:             // Case -1: First Charge Disable State
        L_SeqAct.L_ChgEnbAct    = true;
        L_ChgDlyTmr             = 0.0F;
        L_IOffsDsb              = true;
        L_IOffsTmr              = 0.0F;
        L_ChgSt                 = -2;
        break;
      case -2:             // Case -2: Wait until main contactor dropped out
        L_ChgSt              = -3;
        break;
      case -3:             // Case -3: Final Charge Disable State
        L_SeqAct.L_ChgEnbAct    = false;
        L_SeqStat.L_ChgEnbStat  = false;
        L_ChgDlyTmr             = 0.0F;
        break;

     default:
        break;
    }

    return;
}


/******************************************************************************
 * Function:  KDFContactorTask
 * Execution Level: Task 2
 * Execution Time:  Not Measured
 * Formal Inputs:  None
 * Formal Outputs:  None
 * Global Inputs:   L_SeqReq
 *                  L_SeqAct
 *                  L_SeqStat
 * Global Outputs:  DFiltCClsCmd
 *
 * Locals:          Temp1
 *
 * Return Value:
 * Function Calls:  none
 *
 * Abstract: Manage the opening and closing of the distortion filter contactor
 *
 * Related Viso:  Doc/Diagrams/Common/AIL/Line/
 *
 *******************************************************************************/
void KdfContactorTask(void)
{

    KdfTypeSt = (L_SeqReq.L_KdfEnbReq) | (L_SeqStat.L_KdfEnbStat << 1)
                | (L_SeqAct.L_KdfEnbAct << 2);

    switch (KdfTypeSt)
    {
        case 0:
             if( KdfClsFbk && !FAULTISACTIVE(KdfOpnTmOut) && !FAULTISACTIVE(KdfClsTmOut) )
            {
                PUSH_DIAG(KdfFbkBad);
            }
            L_KdfTskSt = 0;         /* No action substate */
            break;
        case 3:
              if (!KdfClsFbk )    PUSH_DIAG(KdfFbkBad);
            L_KdfTskSt = 0;         /* No action substate */
            break;
        case 1:
        case 7:
            if (Brg1TripFltZ1)
            {
                PUSH_DIAG(L_BadKdfTsk);
                L_KdfTskSt = 0;         /* No action substate */
            }
            else
            {
                DFiltAuxTmr = 0.0F;
                L_KdfTskSt = 1;         /* 1st enable substate */
            }
            break;
        case 2:
        case 4:
            if ( L_KdfTskSt >= -1)
            {
                L_KdfTskSt = -1;        /* 1st disable substate */
            }
            break;
        case 5:                     /* continue with selected state */
            if (Brg1TripFltZ1)
            {
                PUSH_DIAG(L_BadKdfTsk);
                L_KdfTskSt = -1;        /* 1st disable substate */
            }
        case 6:                     /* No change */
            break;
    }  /* End switch (KdfTypeSt) */

    switch (L_KdfTskSt)
    {
        case 0:              /* Case 0: No action case */
            break;
        case 1:                              /* Case 1: First Enable Substate  */
            L_SeqAct.L_KdfEnbAct = true;
            HeaterEnb = DTAFanEnb = LqdPmpMtrEnb = false;  //Prevent auxiliary loads from changing states
            DFiltAuxTmr  = DFiltAuxTmr + DelTm2;
            if ( DFiltAuxTmr >= KdfQPwrTm )
            {
                L_DfMode = DF_CAPTURE;        //Let task3 capture existing grid currents
                DFiltAuxTmr = 0.0F;
                L_KdfTskSt  = 2;
                CContOpnTmr  = 0.0F;
            }
            break;
        case 2:            /* Case 2: Start timer and close Kdf relay */
            if ( L_DfMode == DF_READY )    //Wait for task3 to capture existing grid currents
            {
                KdfClsCmd   = true;
                CContOpnCmd = true;
                DFiltAuxTmr = DFiltAuxTmr + DelTm2;
                if ( KdfClsFbk )
                {
                    L_DfMode = DF_CALCULATE;    // Let task3 calculate expected filter currents
                    DFiltAuxTmr = 0.0F;
                    L_KdfTskSt = 3;
                }
                else if ( DFiltAuxTmr > PARM(KdfClOpTm) )
                {
                    // Kdf did not close on time
                    HeaterEnb = DTAFanEnb = LqdPmpMtrEnb = true;
                    PUSH_DIAG(KdfClsTmOut);
                    FaultIsActive.KdfClsTmOut=true;
                    DFiltAuxTmr = 0.0F;
                }
            }
            break;
        case 3:            /* Case 3: Verify connection of Distortion Filter or disconnection of charger */
            if ( L_AcLineLoss )  L_AcLinePres = false;
            if ( L_DfMode == DF_STANDBY )
            {
                if ( L_AcLineLoss && L_DfPhAIsZero && L_DfPhBIsZero && L_DfPhCIsZero )
                {
                    L_DfMode = DF_READY;
                    Q1Status = FALSE;             // Update the status of breaker
                }
                #ifdef LEG_TEST
                else if ( !Q1UVHld && (PARM(TestMode) != LF_LEG_TEST))
                #else
                else if ( !Q1UVHld )
                #endif
                {
                    HeaterEnb = DTAFanEnb = TRUE;
                    PUSH_DIAG(LineConnFlt);        // line is incorrectly connected
                }
                L_KdfTskSt   = 4;            /* Jump to next Enable State if no faults were pushed */
            }
            break;
        case 4:              /* Case 4: Last Enable Substate         */
            HeaterEnb = DTAFanEnb  = LqdPmpMtrEnb= true;
            L_SeqAct.L_KdfEnbAct   = false;
            L_SeqStat.L_KdfEnbStat = true;
            break;
        case -1:             /* Case -1: First  Disable Substate */
            L_SeqAct.L_KdfEnbAct = true;
            DFiltAuxTmr = 0.0F;
            CContOpnTmr = 0.0F;
            L_KdfTskSt = -2;
            break;
        case -2:             /* Case -2: Disconnect line distortion filter */
            KdfClsCmd  = false;
            DFiltAuxTmr = DFiltAuxTmr + DelTm2;
            if( !KdfClsFbk )
            {
                CContOpnTmr  = CContOpnTmr + DelTm2;
                L_AcLinePres = true;
                if( CContOpnTmr > PARM(CContOpnTm) )
                {
                    CContOpnCmd = false;
                    CContOpnTmr  = 0.0F;
                    DFiltAuxTmr  = 0.0F;
                    L_KdfTskSt   = -3;
                }
            }
            else if( DFiltAuxTmr > PARM(KdfClOpTm) )
            {
                CContOpnCmd = false;
                CContOpnTmr  = 0.0F;
                DFiltAuxTmr = 0.0F;
                L_KdfTskSt = -3;
                PUSH_DIAG(KdfOpnTmOut); // Distortion filter contactor failed to open
                FaultIsActive.KdfOpnTmOut=true;
            }
            break;
        case -3:             /* Case -3: Final Disable Substate */
            L_SeqAct.L_KdfEnbAct   = false;
            L_SeqStat.L_KdfEnbStat = false;
            break;

    }/* End switch (L_KdfTskSt) */

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This function verifies that PLL is locked to line voltage and
//      enables line transient fault detection.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void SyncInit()
{

    L_SyncTypeSt =(L_SeqReq.L_SyncIniReq)   |
                  (L_SeqStat.L_SyncIniSta<<1) |
                  (L_SeqAct.L_SyncIniAct<<2);

  switch ( L_SyncTypeSt)      // state selection
  {
      case 0:
      case 3:
          L_SyncSt = 0;        // do nothing state
          break;
      case 1:
      case 7:
            if (Brg1TripFlt)
            {
                PUSH_DIAG(L_BadSyncTsk);
                L_SyncSt = 0;  // null state
            }
            else
            {
                L_SyncSt = 1;  // 1st enable state
            }
          break;
      case 2:
      case 4:                 // 1st disable state
          if ( L_SyncSt >= -1)
          {
             L_SyncSt = -1;
          }
          break;
      case 5:                 // continue with selected state
            if (Brg1TripFlt)
            {
                PUSH_DIAG(L_BadSyncTsk);
                L_SyncSt = -1; // 1st disable state
            }
      case 6:
          break;              // no change
      default:
          break;
  }


  switch ( L_SyncSt)
  {
      case 0:              // No action case
          break;
      case 1:              // Case 1: First Sync Init State
                           // Init timer
          L_SeqAct.L_SyncIniAct  = true;
          L_SyncDlyTmr           = 0.0F;
          L_SyncSt               = 2;     // --> pll sync wait state
          break;
      case 2:
          L_SyncDlyTmr           = L_SyncDlyTmr + DelTm2;
          if ( L_SyncDlyTmr > PARM(GmSyncInitTm))
          {
              PUSH_DIAG(L_SyncIniTm);
          }
          else if ( GmPllProven)
          {
              GmLineFltEnb = true;
              L_SeqStat.L_SyncIniSta = true;
              L_SeqAct.L_SyncIniAct  = false;
          }
          break;

      case -1:             // Case -1: First Pll Sync  Disable State
         L_SeqAct.L_SyncIniAct  = true;
         L_SyncSt               = -2;
         L_IOffsDsb             = true;
         L_IOffsTmr             = 0.0F;
         break;
      case -2:             // Case -2: Last Pll Sync Disable State
         L_SeqAct.L_SyncIniAct  = false;
         L_SeqStat.L_SyncIniSta = false;
         break;
     default:
         break;
  }

  return;

}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This function enables line bridge gating and inner regulator.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RegEnable()
{

    L_EnbTypeSt = (L_SeqReq.L_RegEnbReq)   |
                  (L_SeqStat.L_RegEnbStat<<1) |
                  (L_SeqAct.L_RegEnbAct<<2);

  switch ( L_EnbTypeSt)      // state selection
  {
      case 0:
      case 3:
          L_RegEnbSt = 0;        // do nothing state
          break;
      case 1:
      case 7:
            if (Brg1TripFlt)
            {
                PUSH_DIAG(L_BadEnbTsk);
                L_RegEnbSt = 0;  // null state
            }
            else
            {
                L_RegEnbSt = 1;  // 1st enable state
            }
          break;
      case 2:
      case 4:                 // 1st disable state
          if ( L_RegEnbSt >= -1)
          {
             L_RegEnbSt = -1;
          }
          break;
      case 5:                 // continue with selected state
            if (Brg1TripFlt)
            {
                PUSH_DIAG(L_BadEnbTsk);
                L_RegEnbSt = -1; // 1st disable state
            }
      case 6:
          break;              // no change
      default:
          break;
  }


  switch ( L_RegEnbSt)
  {
      case 0:              // No action case
          break;
      case 1:              // Case 1: First Reg Enable State
          L_SeqAct.L_RegEnbAct   = true;
          L_EnbDlyTmr = L_EnbDlyTmr + DelTm2;
          if ( L_EnbDlyTmr >= PARM(L_EnbDlyTm))
          {
             L_BrgPwrEnb           = true;
             L_InnrRegEnb          = true;
             L_RegEnbSt            = 2;
          }
          break;
      case 2:
          L_SeqStat.L_RegEnbStat = true;
          L_SeqAct.L_RegEnbAct   = false;
          break;


      case -1:             // Case -1: First Regulator Disable State
         L_SeqAct.L_RegEnbAct    = true;
         L_BrgPwrEnb             = false;
         L_InnrRegEnb            = false;
         L_EnbDlyTmr             = 0.0F;
         L_RegEnbSt              = -2;      // --> delay one loop time
         break;

      case -2:             // Case -2: Last Regulator Disable State
         L_SeqStat.L_RegEnbStat  = false;
         L_SeqAct.L_RegEnbAct    = false;
         L_IOffsDsb              = true;
         if ( !L_LTestIp && !L_ZVLineOff)
         {
            L_IOffsVld          = false;
         }
         L_IOffsTmr             = 0.0F;
         break;
     default:
         break;
  }

  return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Interface to hardware to pick-up or drop-out line main contactor Q1.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void L_Q1BreakerTask()
{
    unsigned Test1All, Test1Open, Test2All, Test3All;
    unsigned PhAGood, PhBGood, PhCGood;
    unsigned PhAOpen, PhBOpen, PhCOpen;

    // ZJ, 20140327: these vars are not always write before read, so init here
    Test1All = Test1Open = Test2All = Test3All = false;

    if ( !Q1RunPrm )
    {
        Q1ChargTmr = Q1ChargTmr + DelTm2;
        if ( Q1ChargTmr >= PARM(Q1SprChrgTm))   //Wait 6 sec for closing spring to charge before enabling Q1 permissive
        {
            Q1RunPrm    = true;
            Q1ChargTmr  = 0.0F;
        }
    }
    // Logic for Q1 status
    // UV ClsPls Q1(z)  Q1
    // 0  0      0      0
    // 0  0      1      0
    // 0  1      0      0
    // 0  1      1      0
    // 1  0      0      0
    // 1  0      1      1
    // 1  1      0      1
    // 1  1      1      1
    Q1ClsIp = (Q1UVHld && Q1ClsPulse) || (Q1UVHld && Q1ClsIp);
    //if (Q1Status || Q1ClsPulse)
    //{
    //    Q1ClsIp = true;
    //}
    //else
    //{
    //    Q1ClsIp = false;
    //}

    if (L_AcLinePres || Q1ClsPulse)  AcLineIp = true;
    else AcLineIp = false;

    Q1TypeSt = (L_SeqReq.L_Q1EnbReq) | (L_SeqStat.L_Q1EnbStat << 1)
               | (L_SeqAct.L_Q1EnbAct << 2);


    switch (Q1TypeSt)
    {
        case 3:
        case 0:
            L_Q1TskSt = Q1_IDLE;       /* Goto Idle state */
            break;

        case 1:
            // L_BadQ1Tsk is pushed only for specific fault conditions
            if ( Q1OpnReq || SftyChnQ1Opn ||  L_PlsTstCmd || L_CelTstCmd || Q1TripFbk )
            {
                PUSH_DIAG(L_BadQ1Tsk);
                L_Q1TskSt = Q1_IDLE;
            }
            else if ( Brg1TripFltZ1 )
            {
                L_Q1TskSt = Q1_IDLE;
            }
            else    /* Not TrpFltBg1z1 */
                L_Q1TskSt = Q1_ENB_1;       /* Goto 1st enable state */
            break;

        case 2:
        case 4:
                /* Goto 1st disable state if 1st time abort detected */
            if (L_Q1TskSt >= Q1_DSB_1) L_Q1TskSt = Q1_DSB_1;
            break;

        case 5:
            // L_BadQ1Tsk is pushed only for specific fault conditions
            if ( Q1OpnReq || SftyChnQ1Opn ||  L_PlsTstCmd || L_CelTstCmd || Q1TripFbk )
            {
                PUSH_DIAG(L_BadQ1Tsk);
                L_Q1TskSt = Q1_IDLE;
            }
            else if ( Brg1TripFltZ1 )
            {
                L_Q1TskSt = Q1_IDLE;
            }
            else if (L_Q1TskSt <= 0) L_Q1TskSt = Q1_ENB_1;
            break;

        case 6:
            if (L_Q1TskSt >= 0) L_Q1TskSt = Q1_DSB_1;
            break;

        case 7:
            // L_BadQ1Tsk is pushed only for specific fault conditions
            if ( Q1OpnReq || SftyChnQ1Opn ||  L_PlsTstCmd || L_CelTstCmd || Q1TripFbk )
            {
                PUSH_DIAG(L_BadQ1Tsk);
                L_Q1TskSt = Q1_IDLE;
            }
            else if ( Brg1TripFltZ1 )
            {
                L_Q1TskSt = Q1_IDLE;
            }
            else    /* Not Brg1TripFltZ1 */
            {
                /* Goto 1st enable state if 1st time abort detected */
                if (L_Q1TskSt <= 0) L_Q1TskSt = Q1_ENB_1;
            }
            break;
    }   /* switch (Q1TypeSt) */

    switch (L_Q1TskSt)
    {
        case Q1_IDLE:     /* Idle State */
            break;

        case Q1_ENB_1:      /* Enable states */
             L_SeqAct.L_Q1EnbAct   = true;
             HeaterEnb = DTAFanEnb = false;     /* Prevent auxiliary loads from changing states */
             L_Q1TskTmr            = 0.0F;
             Q1UVHld               = true;
             if ( SimMode)
             {
                Q1StatSim = true;
             }
             Q1ShuntTrip           = false;
             L_Q1TskSt             = Q1_ENB_2;  /* Increment state */
             if (L_DfMode == DF_READY)
             {
                L_DfMode = DF_CAPTURE;          // Let task3 capture existing grid currents
             }
             break;

        case Q1_ENB_2:     /* Enable states - Wait for UV to pickup */
            L_Q1TskTmr = L_Q1TskTmr + DelTm2;
            if ( (L_Q1TskTmr >= PARM(Q1UVPuDly)) && ((L_DfMode == DF_READY) || (L_DfMode == DF_STANDBY)) )
            {
                 Q1ClsPulse = true;             /* Q1 close pulse     */
                 L_Q1TskTmr   = 0.0F;
                 L_Q1TskSt    = Q1_ENB_3;    /* Increment state */
            }
            break;

        case Q1_ENB_3:     /* Enable states - Wait for Q1 to close */
            L_Q1TskTmr = L_Q1TskTmr + DelTm2;
            SeqDemodulateLine ();                  // Calculate volt vector square error across Q1
            if ( L_Q1TskTmr > PARM(Q1ClsDlyTm) )
            {
                if ( L_Q1TskTmr > L_FilSignTm1 )   // Wait for filtered voltage to settle
                {
                    if ( L_DfMode == DF_READY )
                    {
                        L_DfMode = DF_CALCULATE;         // Let task3 calculate expected filter currents
                    }
                    L_Q1TskTmr   = 0.0F;
                    Q1ClsPulse   = false;              /* Clear Breaker close pulse     */
                    L_Q1TskSt    = Q1_ENB_4;     /* Goto next Enable state */
                }
            }
            break;

        case Q1_ENB_4:
            if ( L_DfMode == DF_STANDBY )    // Distortion Filter currents were evaluated
            {
                HeaterEnb = DTAFanEnb = true;

                Test1Open  = L_DfPhAIsZero || L_DfPhBIsZero || L_DfPhCIsZero;
                Test1All  = Test1Open || L_DfPhAIsBad || L_DfPhBIsBad || L_DfPhCIsBad;
                if ( L_DfTestIndx == 2 )
                {
                    Test2All  = L_DfPhA1IsZero || L_DfPhB1IsZero || L_DfPhC1IsZero;
                    Test2All  = Test2All || L_DfPhA1IsBad || L_DfPhB1IsBad || L_DfPhC1IsBad;

                    Test3All  = L_DfPhA0IsZero || L_DfPhB0IsZero || L_DfPhC0IsZero;
                    Test3All  = Test3All || L_DfPhA0IsBad || L_DfPhB0IsBad || L_DfPhC0IsBad;
                }

                /* Voltage mismatch conditions are only analyzed for latest test result */
                if ( LineLnSeqFil > PARM(Q1VlThrs) )
                {
                    if ( !Test1Open )
                    {
                        if ( !L_AcLineLoss )
                        {
                            PUSH_DIAG(GLVxyImbFlt);
                         }
                        else
                        {
                            PUSH_DIAG(L_BrdgOpen);
                        }
                    }
                    else
                    {
                        if ( L_DfPhAIsZero )    PUSH_DIAG(LinePhAOpen);
                        if ( L_DfPhBIsZero )    PUSH_DIAG(LinePhBOpen);
                        if ( L_DfPhCIsZero )    PUSH_DIAG(LinePhCOpen);
                    }

                    Q1UVHld               = false;      /* Remove Q1 undervoltage */
                    Q1ShuntTrip           = true;
                    Q1Status              = false;
                    L_SeqAct.L_Q1EnbAct   = false;
                    L_SeqStat.L_Q1EnbStat = false;
                    L_Q1TskSt             = Q1_IDLE;    /* Goto Idle state */
                }
                else if ( Test1All && Test2All && Test3All )  /* All tests reported irregular results - Analyze phases individually */
                {
                   /* Phase-A */
                    PhAGood = (!L_DfPhA2IsZero && !L_DfPhA2IsBad) && (!L_DfPhA1IsZero && !L_DfPhA1IsBad);
                    PhAGood = PhAGood || ((!L_DfPhA2IsZero && !L_DfPhA2IsBad) && (!L_DfPhA0IsZero && !L_DfPhA0IsBad));
                    PhAGood = PhAGood || ((!L_DfPhA1IsZero && !L_DfPhA1IsBad) && (!L_DfPhA0IsZero && !L_DfPhA0IsBad));

                    PhAOpen = L_DfPhA2IsZero && L_DfPhA1IsZero;
                    PhAOpen = PhAOpen || (L_DfPhA2IsZero && L_DfPhA0IsZero);
                    PhAOpen = PhAOpen || (L_DfPhA1IsZero && L_DfPhA0IsZero);

                   /* Phase-B */
                    PhBGood = (!L_DfPhB2IsZero && !L_DfPhB2IsBad) && (!L_DfPhB1IsZero && !L_DfPhB1IsBad);
                    PhBGood = PhBGood || ((!L_DfPhB2IsZero && !L_DfPhB2IsBad) && (!L_DfPhB0IsZero && !L_DfPhB0IsBad));
                    PhBGood = PhBGood || ((!L_DfPhB1IsZero && !L_DfPhB1IsBad) && (!L_DfPhB0IsZero && !L_DfPhB0IsBad));

                    PhBOpen = L_DfPhB2IsZero && L_DfPhB1IsZero;
                    PhBOpen = PhBOpen || (L_DfPhB2IsZero && L_DfPhB0IsZero);
                    PhBOpen = PhBOpen || (L_DfPhB1IsZero && L_DfPhB0IsZero);

                   /* Phase-C */
                    PhCGood = (!L_DfPhC2IsZero && !L_DfPhC2IsBad) && (!L_DfPhC1IsZero && !L_DfPhC1IsBad);
                    PhCGood = PhCGood || ((!L_DfPhC2IsZero && !L_DfPhC2IsBad) && (!L_DfPhC0IsZero && !L_DfPhC0IsBad));
                    PhCGood = PhCGood || ((!L_DfPhC1IsZero && !L_DfPhC1IsBad) && (!L_DfPhC0IsZero && !L_DfPhC0IsBad));

                    PhCOpen = L_DfPhC2IsZero && L_DfPhC1IsZero;
                    PhCOpen = PhCOpen || (L_DfPhC2IsZero && L_DfPhC0IsZero);
                    PhCOpen = PhCOpen || (L_DfPhC1IsZero && L_DfPhC0IsZero);

                    if ( PhAGood && PhBGood && PhCGood )
                    {
                        L_DfTestIndx =  -1;   /* Dist-filter was verified successfully */
                    }
                    else                    /* Not all tests agreed on a good result */
                    {
                        if ( !PhAGood )
                        {
                            if ( PhAOpen )
                            {
                                PUSH_DIAG(DFiltPhAOpen);     /* At least two tests agreed on Ph-A been opened */
                            }
                            else if ( !PhBOpen && !PhCOpen )
                            {
                                PUSH_DIAG(DFiltPhAFlt);      /* Ph-A is considered out of specs if it isn't opened */
                            }
                        }
                        if ( !PhBGood )
                        {
                            if ( PhBOpen )
                            {
                                PUSH_DIAG(DFiltPhBOpen);     /* At least two tests agreed on Ph-B been opened */
                            }
                            else if ( !PhAOpen && !PhCOpen )
                            {
                                PUSH_DIAG(DFiltPhBFlt);      /* Ph-B is considered out of specs if it isn't opened */
                            }
                        }
                        if ( !PhCGood )
                        {
                            if ( PhCOpen )
                            {
                                PUSH_DIAG(DFiltPhCOpen);     /* At least two tests agreed on Ph-C been opened */
                            }
                            else if ( !PhAOpen && !PhBOpen )
                            {
                                PUSH_DIAG(DFiltPhCFlt);      /* Ph-C is considered out of specs if it isn't opened */
                            }

                        }
                    }
                }
                else
                {
                    if ( (L_DfTestIndx < 2) && Test1All && !SimAcclAct)  /* Perform another test only if last results were doubful */
                    {
                        L_DfTestIndx =  L_DfTestIndx + 1;
                    }
                    else
                    {
                        L_DfTestIndx =  -1;           /* Dist-filter was verified successfully or 3 tests were performed */
                    }
                }

                if ( Q1UVHld )
                {
                    Q1Status       = true;         /* Declare Q1 close  */
                    Q1RunPrm       = false;        /* Closing spring starts charging */
                    L_AcLinePres   = true;
                    L_SeqStat.L_Q1EnbStat = true;
                    L_SeqAct.L_Q1EnbAct   = false;
                }
            }
            break;

        case Q1_DSB_1:
            L_SeqAct.L_Q1EnbAct = true;
            L_Q1TskTmr          = 0.0F;
            Q1UVHld             = false;
            Q1ShuntTrip         = true;
            Q1ClsPulse          = false;            /* Clear Breaker close pulse(should already be done */
            L_Q1TskSt           = Q1_DSB_2;         /* Goto next disable state */
            break;

        case Q1_DSB_2:
            L_Q1TskTmr          = L_Q1TskTmr + DelTm2;
            SeqDemodulateLine ();                  //Calculate volt vector square error across Q1
            if ( L_Q1TskTmr > PARM(Q1OpenTypTm))
            {
                Q1StatSim = false;
            }
            if ( L_Q1TskTmr > PARM(Q1OpnDlyTm) )
            {
                if ( L_Q1TskTmr > L_FilSignTm2 )   //Wait for filtered voltage to settle
                {
                    // Make sure that KDF remains closed until Q1 finishes its sequence task
                    // and grid monitor is locked to its voltage !
                    if ( (KdfTypeSt == 3 && !L_BrgPwrEnb) && (((LineLnSeqFil < PARM(Q1VlThrs)) && GmPllProven) || !L_AcLineLoss) )
                    {
                        PUSH_DIAG(Q1OpnTmOut);
                        S_K1OpnNowReq = true;
                    }
                    else if ( ParkReq )
                    {
                        Q1Status = false;   /* Declare Q1 open even though there is uncertainty*/
                    }
                    else
                    {
                        Q1Status        = false;            /* Declare Q1 open  */
                        if ( KdfTypeSt == 3 )    L_AcLinePres    = false;
                    }
                    L_Q1TskTmr            = 0.0F;
                    L_Q1TskSt             = -3;    /* Goto next disable state */
                }
            }
            break;

        case -3:        /* Case -3: Final Disable Substate */
            L_SeqAct.L_Q1EnbAct   = false;
            L_SeqStat.L_Q1EnbStat = false;
            break;
    }   /* switch (L_Q1TskSt) */
    //TODO, ZJ, bug fix for L_MaStatus not popup. Later should check if Ipc can
    // change from Ma to Q1, so that the 2 variables can be combined.
    L_MaStatus=Q1Status;
}




///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Main line sequencer execution loop.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void SourceCtrlSequencer(void)
{
    LineCoreSequencer();
    LineCoreDriveSequencer();
    SequencerTaskInterface();       // Must run before any task is executed

    LinkChargeCheck();
    KdfContactorTask();
    L_Q1BreakerTask();
    DistFiltPhCurr();

    SyncInit();
    RegEnable();

    InductanceMeasurementSequencer();

    //TODO, ZJ 20140324, in 2.75, not in 1.6e. Later to see whether needed for 2.3
    //L_CelTstDisab = TripFlt || !L_SeqStBits.L_CellTest || !L_SeqStBits.L_DiagMode;

    // Delayed bridge 1 fault trip to be used on avoiding bad task faults
    Brg1TripFltZ1 = Brg1TripFlt;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Calculate distortion filter active and reactive power error on a
//      per-phase basis, and evaluate if error magnitudes are within their
//      tolerances.
//
// EXECUTION LEVEL
//      Task2 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void DistFiltPhCurr (void)
{
    float tempVar;


    if ( (L_DfTestIndx > -1) && (L_DfTestIndx < DFMAXTESTNUMBER) )  // Valid range
    {
        // Initialize low-pass filter versions of Qpwr and Pwr when test index is incremented
        if ( L_DfTestIndx > L_DfTestIndxZ1 )
        {
            L_DfPhA[L_DfTestIndx].QPwr = GmQPwr * 0.333333;
            L_DfPhB[L_DfTestIndx].QPwr = L_DfPhA[L_DfTestIndx].QPwr;
            L_DfPhC[L_DfTestIndx].QPwr = L_DfPhB[L_DfTestIndx].QPwr;

        }

        // Calculate and filter power quantities
        L_DfPhA[L_DfTestIndx].QPwr += L_DfQPwrGn * (0.5F*((GmVanAl2 * GmIaBe2) - (GmVanBe2 * GmIaAl2)) - L_DfPhA[L_DfTestIndx].QPwr);
        L_DfPhB[L_DfTestIndx].QPwr += L_DfQPwrGn * (0.5F*((GmVbnAl2 * GmIbBe2) - (GmVbnBe2 * GmIbAl2)) - L_DfPhB[L_DfTestIndx].QPwr);
        L_DfPhC[L_DfTestIndx].QPwr += L_DfQPwrGn * (0.5F*((GmVcnAl2 * GmIcBe2) - (GmVcnBe2 * GmIcAl2)) - L_DfPhC[L_DfTestIndx].QPwr);
    }
    L_DfTestIndxZ1 = L_DfTestIndx;

    switch (L_DfMode)
    {
        case DF_CAPTURE:
            if ( L_DfTestIndx < 0 )
            {
                L_DfTestIndx = 0;
            }
            if ( L_DfTestIndx > (DFMAXTESTNUMBER-1) )
            {
                L_DfTestIndx = DFMAXTESTNUMBER-1;
            }


           // Clear content of status structures and preserve filtered reactive power
            tempVar = L_DfPhA[L_DfTestIndx].QPwr;
            memset(L_DfPhA + L_DfTestIndx, 0, (sizeof L_DfPhA[L_DfTestIndx])*(DFMAXTESTNUMBER-L_DfTestIndx));
            L_DfPhA[L_DfTestIndx].QPwr = tempVar;
            L_DfPhA[L_DfTestIndx].IsBad = true;

            tempVar = L_DfPhB[L_DfTestIndx].QPwr;
            memset(L_DfPhB + L_DfTestIndx, 0, (sizeof L_DfPhB[L_DfTestIndx])*(DFMAXTESTNUMBER-L_DfTestIndx));
            L_DfPhB[L_DfTestIndx].QPwr = tempVar;
            L_DfPhB[L_DfTestIndx].IsBad = true;

            tempVar = L_DfPhC[L_DfTestIndx].QPwr;
            memset(L_DfPhC + L_DfTestIndx, 0, (sizeof L_DfPhC[L_DfTestIndx])*(DFMAXTESTNUMBER-L_DfTestIndx));
            L_DfPhC[L_DfTestIndx].QPwr = tempVar;
            L_DfPhC[L_DfTestIndx].IsBad = true;

            // Capture present reactive power
            L_DfPhA[L_DfTestIndx].QPwrAux  = L_DfPhA[L_DfTestIndx].QPwr;
            L_DfPhB[L_DfTestIndx].QPwrAux  = L_DfPhB[L_DfTestIndx].QPwr;
            L_DfPhC[L_DfTestIndx].QPwrAux  = L_DfPhC[L_DfTestIndx].QPwr;

           // Let task2  continue with the sequence
            L_DfMode = DF_READY;
            L_DfQPwrTmr = 0.0F; // add here to fix bug of the possibility of jumping to calculate state directly, without reset timer
            break;


        case DF_READY:       // Ready for following calculation
            L_DfQPwrTmr = 0.0F;
            break;

        case DF_CALCULATE:
            if ( L_DfTestIndx < 0 )                     L_DfTestIndx = 0;
            if ( L_DfTestIndx > (DFMAXTESTNUMBER-1) )   L_DfTestIndx = DFMAXTESTNUMBER-1;
            L_DfQPwrTmr  = L_DfQPwrTmr + DelTm2;
            if ( (L_DfPhA[L_DfTestIndx].QPwr - L_DfPhA[L_DfTestIndx].QPwrAux) >= PARM(L_DfQPwrNomThrs))  L_DfPhA[L_DfTestIndx].IsBad = false;
            if ( (L_DfPhB[L_DfTestIndx].QPwr - L_DfPhB[L_DfTestIndx].QPwrAux) >= PARM(L_DfQPwrNomThrs))  L_DfPhB[L_DfTestIndx].IsBad = false;
            if ( (L_DfPhC[L_DfTestIndx].QPwr - L_DfPhC[L_DfTestIndx].QPwrAux) >= PARM(L_DfQPwrNomThrs))  L_DfPhC[L_DfTestIndx].IsBad = false;

            if ( (L_PlsTstCmd || L_CelTstCmd) & L_AcLineLoss )  // A diagnostic command is being executed
            {
                L_DfPhA[L_DfTestIndx].IsZero = true;
                L_DfPhB[L_DfTestIndx].IsZero = true;
                L_DfPhC[L_DfTestIndx].IsZero = true;

                // Let task2  continue with the sequence, dist-filter is not verified while running diagnostics
                L_DfMode = DF_STANDBY;
            }
            else if ( !(L_DfPhA[L_DfTestIndx].IsBad || L_DfPhB[L_DfTestIndx].IsBad || L_DfPhC[L_DfTestIndx].IsBad) )
            {
                // Let task2  continue with the sequence, dist-filter is Ok.
                L_DfMode = DF_STANDBY;
            }
            else if ( L_DfQPwrTmr > PARM(L_DfQPwrDlyTm) )
            {
               /* Phase-A open phase checking, based on before and after reactive power */
                if ( (L_DfPhA[L_DfTestIndx].QPwr - L_DfPhA[L_DfTestIndx].QPwrAux) <= PARM(L_DfQPwrLowThrs) )
                {
                    #ifdef LEG_TEST
                    if ( PARM(TestMode) != LF_LEG_TEST)
                    {
                    #endif
                        L_DfPhA[L_DfTestIndx].IsZero = true;
                    #ifdef LEG_TEST
                    }
                    #endif
                }

               /* Phase-B open phase checking, based on before and after reactive power */
                if ( (L_DfPhB[L_DfTestIndx].QPwr - L_DfPhB[L_DfTestIndx].QPwrAux) <= PARM(L_DfQPwrLowThrs) )
                {
                    #ifdef LEG_TEST
                    if ( PARM(TestMode) != LF_LEG_TEST)
                    {
                    #endif
                        L_DfPhB[L_DfTestIndx].IsZero = true;
                    #ifdef LEG_TEST
                    }
                    #endif
                }

               /* Phase-C open phase checking, based on before and after reactive power */
                if ( (L_DfPhC[L_DfTestIndx].QPwr - L_DfPhC[L_DfTestIndx].QPwrAux) <= PARM(L_DfQPwrLowThrs) )
                {
                    #ifdef LEG_TEST
                    if ( PARM(TestMode) != LF_LEG_TEST)
                    {
                    #endif
                        L_DfPhC[L_DfTestIndx].IsZero = true;
                    #ifdef LEG_TEST
                    }
                    #endif
                }

               // Let task2  continue with the sequence
                L_DfMode = DF_STANDBY;
            }
            break;

        case DF_STANDBY:      // Do nothing
            break;

    }      /* switch (DFMode) */

    L_DfModeZ1 = L_DfMode;

    // Publish array elements
    if ( (L_DfTestIndx > -1) && (L_DfTestIndx < DFMAXTESTNUMBER) )  // Valid range
    {
        L_DfPhAIsZero = L_DfPhA[L_DfTestIndx].IsZero;
        L_DfPhAIsBad  = L_DfPhA[L_DfTestIndx].IsBad;
        L_DfPhAQPwrAux = L_DfPhA[L_DfTestIndx].QPwrAux;
        L_DfPhAQPwr   = L_DfPhA[L_DfTestIndx].QPwr;
        L_DfPhA0IsZero = L_DfPhA[0].IsZero;
        L_DfPhA0IsBad  = L_DfPhA[0].IsBad;
        L_DfPhA0QPwrAux = L_DfPhA[0].QPwrAux;
        L_DfPhA0QPwr   = L_DfPhA[0].QPwr;
        L_DfPhA1IsZero = L_DfPhA[1].IsZero;
        L_DfPhA1IsBad  = L_DfPhA[1].IsBad;
        L_DfPhA1QPwrAux = L_DfPhA[1].QPwrAux;
        L_DfPhA1QPwr   = L_DfPhA[1].QPwr;
        L_DfPhA2IsZero = L_DfPhA[2].IsZero;
        L_DfPhA2IsBad  = L_DfPhA[2].IsBad;
        L_DfPhA2QPwrAux = L_DfPhA[2].QPwrAux;
        L_DfPhA2QPwr   = L_DfPhA[2].QPwr;

        L_DfPhBIsZero = L_DfPhB[L_DfTestIndx].IsZero;
        L_DfPhBIsBad  = L_DfPhB[L_DfTestIndx].IsBad;
        L_DfPhBQPwrAux = L_DfPhB[L_DfTestIndx].QPwrAux;
        L_DfPhBQPwr   = L_DfPhB[L_DfTestIndx].QPwr;
        L_DfPhB0IsZero = L_DfPhB[0].IsZero;
        L_DfPhB0IsBad  = L_DfPhB[0].IsBad;
        L_DfPhB0QPwrAux = L_DfPhB[0].QPwrAux;
        L_DfPhB0QPwr   = L_DfPhB[0].QPwr;
        L_DfPhB1IsZero = L_DfPhB[1].IsZero;
        L_DfPhB1IsBad  = L_DfPhB[1].IsBad;
        L_DfPhB1QPwrAux = L_DfPhB[1].QPwrAux;
        L_DfPhB1QPwr   = L_DfPhB[1].QPwr;
        L_DfPhB2IsZero = L_DfPhB[2].IsZero;
        L_DfPhB2IsBad  = L_DfPhB[2].IsBad;
        L_DfPhB2QPwrAux = L_DfPhB[2].QPwrAux;
        L_DfPhB2QPwr   = L_DfPhB[2].QPwr;

        L_DfPhCIsZero = L_DfPhC[L_DfTestIndx].IsZero;
        L_DfPhCIsBad  = L_DfPhC[L_DfTestIndx].IsBad;
        L_DfPhCQPwrAux = L_DfPhC[L_DfTestIndx].QPwrAux;
        L_DfPhCQPwr   = L_DfPhC[L_DfTestIndx].QPwr;
        L_DfPhC0IsZero = L_DfPhC[0].IsZero;
        L_DfPhC0IsBad  = L_DfPhC[0].IsBad;
        L_DfPhC0QPwrAux = L_DfPhC[0].QPwrAux;
        L_DfPhC0QPwr   = L_DfPhC[0].QPwr;
        L_DfPhC1IsZero = L_DfPhC[1].IsZero;
        L_DfPhC1IsBad  = L_DfPhC[1].IsBad;
        L_DfPhC1QPwrAux = L_DfPhC[1].QPwrAux;
        L_DfPhC1QPwr   = L_DfPhC[1].QPwr;
        L_DfPhC2IsZero = L_DfPhC[2].IsZero;
        L_DfPhC2IsBad  = L_DfPhC[2].IsBad;
        L_DfPhC2QPwrAux = L_DfPhC[2].QPwrAux;
        L_DfPhC2QPwr   = L_DfPhC[2].QPwr;

    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Demodulate line bridge voltage feedacks using grid 690V
//      transformer secondary angle, calculate square voltage
//      difference across breaker MA, and filter calculation.
//
// EXECUTION LEVEL
//      Task2 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void SeqDemodulateLine (void)
{
    float   LineLnVxDiff;
    float   LineLnVyDiff;

    LineLnVxDiff  = GmVxFbkT2 - L_VxFbkT2;
    LineLnVyDiff  = GmVyFbkT2 - L_VyFbkT2;

    LineLnVdifSq     = LineLnVxDiff*LineLnVxDiff + LineLnVyDiff*LineLnVyDiff;
    LineLnSeqFil += L_Q1DeltVoltGn * (LineLnVdifSq - LineLnSeqFil);

    return;
}

