///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       XOPS Task Scheduler
//
// DESCRIPTION:
//      Provides task scheduling and data passing for the MarkStat control process
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      List locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////


// Includes
//---------
// system
#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <sys/iomsg.h>
#include <sys/trace.h>          // for TraceEvent()
#include <semaphore.h>
#include <unistd.h>             // sleep(), usleep()
#include <fcntl.h>
#include <fpstatus.h>
#include <x86/inline.h>         // pswget()
#include <x86/priv.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>  //vivo: add
// core
#include "FrcSched.h"
#include "Hssl.h"
#include "InterfaceBase.h"
#include "ParamString.h"
#include "Priority.h"
#include "SdiComm.h"
#include "StateMachineClient.h"
#include "SyncMemClient.h"
#include "SysMsgClient.h"
#include "ThreadInfo.h"
#include "MonitorCommand.h"
#include "SeqSequencerClient.h"
#include "GeSList.h"
// ipc
#include "IpcDevice.h"
// framework
#include "x0FrameWork.h"        // must be after IpcDevice.h so as to not redefine TRUE/FALSE
#include "x0DataPass.h"
#include "x0XsimInterface.h"
#include "x0CaptureBuf.h"
#include "x0BridgeInterface.h"
#include "x0Diagnostic.h"
#include "x0SdiMsgServer.h"
#include "x0err.h"
#include "x0PerfMon.h"
#include "x0Xops.h"
#include "x0GpDiagFilter.h"
// product
#include "s0Sim.h"

#define __max(a,b) (((a) > (b)) ? (a) : (b))
#define __min(a,b) (((a) < (b)) ? (a) : (b))

// Externals
//----------
extern cSysMemConfigClient  gSysMemConfigClient;
extern cStateMachineClient  gStateMachineClient;
extern cMonitorCommand      gMonitor;               // global low level monitor class instance
extern cIpcDevice           gIpcDevice;
extern cMarkStatDiag        MarkStatDiag;
extern cDataPasser          DataPassManager;
extern cSdiMsgServer        SdiMsg;
extern cCaptureBuf          CaptureBuf;
extern cTripBuf             TripBuf;

extern cFrcSchedBase*       pFrcSched;

extern cGpDiagFilter*       pGpDiagFilterT1;
extern cGpDiagFilter*       pGpDiagFilterT1A;

extern unsigned             DSP_CapBufState;
extern unsigned             DSP_TrpBufState;

extern unsigned             SimMode;
extern unsigned             DelIsrFrc;

extern bool                 IpcInterfaceEnable;

extern int                  T1FramesPerT2;
extern int                  T1FramesPerT3;

extern int                  T1AFramesPerT2;
extern int                  T1AFramesPerT3;

extern int                  T2FramesPerT3;
extern int                  T3FramesPerTn;

extern float                DelTm1;
extern float                DelTm1A;
extern float                DelTm2;
extern float                DelTm3;

extern unsigned             InitDone;

extern unsigned             LlmAccumRst;

extern float                CpuCps;
extern float                CpuCpsRcp;
extern float                CpuCpsRcpUsec;

extern float                FrcClkToCpuClk;
extern float                CpuClkToFrcClk;

extern unsigned             Clk0PerT1;
extern unsigned             Clk0PerT1A;

extern float                FrcFreq;
extern WORD                 FrcSchedMask;
extern unsigned             XopsIsrFrmOffFrc;
extern unsigned             XopsIsrFrmOffCpu;

extern unsigned             DiagResetTaskReq;

extern unsigned             XSimMode;
extern float                SimTmRatio;
extern float                DelTm3Wdog;

extern int                  SpinWaitCtrT1;
extern int                  SpinWaitCtrT1A;
extern int                  SpinWaitCtrT1S;
extern int                  SpinWaitCtrT2;
extern int                  SpinWaitCtrT3;
extern int                  SpinWaitCtrB;
extern unsigned SimRunRuleReq;
extern bool     IpcInit( void );

extern void*    FlashFileSvr( void* arg );
extern void     VmetroTrigger( void );

extern void     Task1( void );
extern void     Task1A( void );
extern void     Task1S( unsigned Sim1ARunReq, unsigned Sim1AFbkReq );
extern void     Task2( void );
extern void     Task3( void );
extern void     TaskB( void );
int RunRuleSet(int Rule_Set);

extern void     InitAdlServer( int ChannelId );
extern int      adlReceiveDirect( int ReceiveId );
extern void     AdlDataSampler( float DelExeTm );

extern void     ToolboxStatus( void );

extern void     ConstantValue( void );
extern void     TestOscillator( void );

extern void     IpcCommTest( void );


// Constants
//----------

// index for TaskData[]
#define TDP_SCHED               (0)
#define TDP_TASK1               (1)
#define TDP_TASK2               (2)
#define TDP_TASK3               (3)
#define TDP_N_TASKS             (4)     // number of tasks managed by xops (= 1 + largest task index)

// execution measurement system
#define XMS_W_FIL               (0.1F)  // filter omega (r/s)

#define XMS_XOPS                (0)     // measurement site index definions (must be ordered from high to low execution priority)
#define XMS_TSK1                (1)     //  (also must match XmsSiteEnum definitions in x0Xops.xls)
#define XMS_TSK1A               (2)
#define XMS_TSK1S               (3)
#define XMS_TSK2                (4)
#define XMS_TSK3                (5)
#define XMS_N_MEAS              (6)     // number of measurement system sites (= 1 + largest measurement site index)


#define XMS_XOPS_PER_T1         (1)     // maximum XopsSched per Task1 (for measurement system memory allocation)
#define XMS_T1_PER_T2           (10)    // maximum Task1's   per Task2 (for measurement system memory allocation)
#define XMS_T2_PER_T3           (15)    // maximum Task2's   per Task3 (for measurement system memory allocation)
#define XMS_N_SAMPLES           (XMS_T2_PER_T3 * XMS_T1_PER_T2 * XMS_XOPS_PER_T1)

// trace event base numbers
#define TRACE_TASK1A            (15)
#define TRACE_TASK1S            (16)
#define TRACE_ISR_LATENCY       (20)
#define TRACE_TASK_OVERRUN      (30)
#define TRACE_LLM_TRIG          (40)
#define TRACE_PERFMON_MEAS      (50)
#define TRACE_PERFMON_TRIG      (60)
#define TRACE_EVENT_TRIG        (70)


// Types and Classes
//------------------

// task execution measurement structures
struct XmsProbeTyp
{
    uint64_t        ClkStart;
    uint64_t        ClkEnd;
    uint32_t        ClkDelta;
    uint32_t        ClkDeltaMax;
    uint32_t        ClkDeltaMin;
    float           ClkDeltaAvg;
    float*          pClkProbe;

    uint64_t        Pm0Start;
    uint64_t        Pm0End;
    uint32_t        Pm0Delta;
    uint32_t        Pm0DeltaMax;
    uint32_t        Pm0DeltaMin;
    float           Pm0DeltaAvg;
    uint32_t*       pPm0Probe;

    uint64_t        Pm1Start;
    uint64_t        Pm1End;
    uint32_t        Pm1Delta;
    uint32_t        Pm1DeltaMax;
    uint32_t        Pm1DeltaMin;
    float           Pm1DeltaAvg;
    uint32_t*       pPm1Probe;

    uint32_t        IsrEnbCtr;
};

struct SofDataTyp
{
    uint64_t        IsrClkRd;
    uint32_t        FrmFrc;
    uint32_t        IsrFrc;
};

struct XmsStatsTyp
{
    uint32_t        Max;
    uint32_t        Min;
    float           Avg;
};

struct XmsDataTyp
{
    uint32_t        StartClk;
    uint32_t        EndClk;
    uint32_t        DeltaClk;
    unsigned        SofDataIdx;
    unsigned        DeltaClkAdjCnt;
};

struct XmsInfoTyp
{
    XmsStatsTyp      StartDelClk;
    XmsStatsTyp      EndDelClk;
    XmsStatsTyp      DeltaClk;
};

struct XmsBufTyp
{
    SofDataTyp      SofData[XMS_N_SAMPLES];             // start of frame data array [sample#]
    XmsDataTyp      Data[XMS_N_MEAS][XMS_N_SAMPLES];    // sample data array [meas#] [sample#]
    unsigned        Sidx[XMS_N_MEAS];                   // sample index [meas#]
    XmsInfoTyp      Info[XMS_N_MEAS];                   // information array [meas#]
};

struct ClkFrcValTyp
{
    uint64_t        ClkVal;
    uint32_t        FrcVal;
};

// task execution control structure
struct TaskDataTyp
{
    int             TaskNo;
    int             Priority;
    int*            pFrames;
    SysTm_Typ*      pSysTm;
    SysTm_Typ*      pEvtTm;
    int             State;
    unsigned        BusyCtr;
    unsigned        DoneOnce;
    unsigned        OverRunCtr;
    unsigned        OverRunLstpCtr;
    unsigned        OverRunLstpFlg;
    sem_t           Sem;
};



#ifdef XOPS_PERF_MON_TASK
struct PM_CounterDataTyp
{
    uint64_t            Start;
    uint64_t            SmplAcc;
    int                 SmplCnt;
};
struct PM_CaptureDataTyp
{
    unsigned            Cycles;         // clock cycles
    unsigned            Instructions;   // instruction
    float               CPI;            // cycles per instruction
    unsigned            L2CacheMisses;  // L2 cache misses
};
struct  PM_DataType
{
    PM_CaptureDataTyp   Min;
    PM_CaptureDataTyp   Max;
};
struct  PM_TaskTyp
{
    PM_DataType         ExecTime;
    PM_DataType         CPI;
    PM_CaptureDataTyp   Average;
    PM_CounterDataTyp   Ctr[2];
    PM_CounterDataTyp   Cycle;      // time stamp counter
};
struct PM_MeasureTyp
{
    uint64_t            Start;
    uint64_t            SmplAcc;
    int                 SmplCnt;
    unsigned            Min;
    unsigned            Max;
    unsigned            Avg;
};
struct PM_ProbeTyp
{
    PM_MeasureTyp       Cycle;
    PM_MeasureTyp       Cntr[2];
    unsigned            FrcStartOfFrame;
    unsigned            FrcBegin;
    unsigned            FrcEnd;
};
#endif


// Parameters
//-----------
CREATE_PARM(SimModeReq,             unsigned);      // user request for internal simulation mode
CREATE_PARM(XSimModeReq,            unsigned);      // user request for external simulation mode
CREATE_PARM(DsimModeReq,            unsigned);      // user request for DLL simulation mode

CREATE_PARM(IsrSvcErrThresh,        float);         // interrupt service error threshold (sec)

CREATE_PARM(XmsProbeSite,           unsigned);      // execution measurement system probe site


// Published Variables
//--------------------
CREATE_PUBVAR(XSimMode,             unsigned);      // system external sim mode
CREATE_PUBVAR(DsimMode,             unsigned);      // system DLL       sim  mode
CREATE_PUBVAR(SimNotDsim,           unsigned);      // system (Sim && !Dsim) mode

CREATE_PUBVAR(T1St,                 volatile int);  // T1 state within a T2 frame, published version of the task1 frame counter (counts from 0)
CREATE_PUBVAR(T2St,                 volatile int);  // T2 state within a T3 frame, published version of the task2 frame counter (counts from  0)
CREATE_PUBVAR(T3St,                 volatile int);  // T3 state counter, published version of the task3 frame counter (counts from  0)

CREATE_PUBVAR(T1T2St,               volatile int);  // T1 state within a T3 frame

CREATE_PUBVAR(T1ASt,                volatile int);  // T1A state within a T2 frame
CREATE_PUBVAR(T1AT2St,              volatile int);  // T1A state within a T3 frame

CREATE_PUBVAR(Task1AExecReq,        unsigned) = false;  // flag to request Task1A execution

CREATE_PUBVAR(EventOffSec,          unsigned);      // displayed event time offset (sec)
CREATE_PUBVAR(EventOffNsec,         unsigned);      // displayed event time offset (nsec)
CREATE_PUBVAR(SyncToUtc,            unsigned);
CREATE_PUBVAR(LastUtcErr,           float);

CREATE_PUBVAR(SysTmSec,             unsigned);      // Displayed system time (sec)
CREATE_PUBVAR(SysTmMsec,            unsigned);      // Displayed system time (msec)

CREATE_PUBVAR(EventTmSec,           unsigned);      // Displayed event time (sec)
CREATE_PUBVAR(EventTmMsec,          unsigned);      // Displayed event time (msec)

CREATE_PUBVAR(DelSysTm,             int);           // used to incrment system time in xops ISR (nsec)

// diagnostic reset to the tasks (used to reset error counters, fpga's,e etc)
//  guaranteed to be true for one execution of the associated task
CREATE_PUBVAR(DiagResetCmdT1,       unsigned);      // for Task1
CREATE_PUBVAR(DiagResetCmdT2,       unsigned);      // for Task2
CREATE_PUBVAR(DiagResetCmdT3,       unsigned);      // for Task3

CREATE_PUBVAR(FrameFrc,             WORD);          // frc of upcoming Task1 frame
CREATE_PUBVAR(FrameFrcZ,            WORD);          // frc of current Task1 frame

CREATE_PUBVAR(FrameFrcT1A,          WORD);          // frc of upcoming Task1A frame
CREATE_PUBVAR(FrameFrcT1AZ,         WORD);          // frc of current Task1A frame

CREATE_PUBVAR(IsrFrc,               WORD);          // scheduled frc of the interrupt

CREATE_PUBVAR(IsrSvcLatency,        unsigned);      // interrupt service latency (cpu clock counts)
CREATE_PUBVAR(IsrSvcErrThresh,      unsigned);      // interrupt service error threshold (cpu clock counts)

CREATE_PUBVAR(PM_ProbeFrameBegin,   float);         // time from the beginning of frame to the start probe  (usec)
CREATE_PUBVAR(PM_ProbeFrameEnd,     float);         // time from the beginning of frame to the end probe  (usec)
CREATE_PUBVAR(PM_ProbeExecTime,     float);         // time between start and end probe  (usec)
CREATE_PUBVAR(PM_ProbeClkCycle,     unsigned);      // clock cycles between start and end probe
CREATE_PUBVAR(PM_ProbeInstruction,  unsigned);      // instruction count between start and end probe
CREATE_PUBVAR(PM_ProbeL2CacheMiss,  unsigned);      // level 2 cache misses between start and end probe

CREATE_PUBVAR(TraceLoggerArm,       unsigned);      // flag to enable tracelogger monitoring of trigger condition

#if 0
CREATE_PUBVAR(XmsTask1StartTm,      float);
CREATE_PUBVAR(XmsTask1EndTm,        float);
#endif

CREATE_PUBVAR(XmsXopsSched,         float);
CREATE_PUBVAR(XmsXopsTask1,         float);
CREATE_PUBVAR(XmsXopsTask1A,        float);
CREATE_PUBVAR(XmsXopsTask1S,        float);

CREATE_PUBVAR(XmsClkProbe0,         float);
CREATE_PUBVAR(XmsClkProbe1,         float);
CREATE_PUBVAR(XmsClkProbe2,         float);
CREATE_PUBVAR(XmsClkProbe3,         float);
CREATE_PUBVAR(XmsClkProbe4,         float);
CREATE_PUBVAR(XmsClkProbe5,         float);
CREATE_PUBVAR(XmsClkProbe6,         float);
CREATE_PUBVAR(XmsClkProbe7,         float);
CREATE_PUBVAR(XmsClkProbe8,         float);
CREATE_PUBVAR(XmsClkProbe9,         float);

CREATE_PUBVAR(XmsPm0Probe0,         uint32_t);
CREATE_PUBVAR(XmsPm0Probe1,         uint32_t);
CREATE_PUBVAR(XmsPm0Probe2,         uint32_t);
CREATE_PUBVAR(XmsPm0Probe3,         uint32_t);
CREATE_PUBVAR(XmsPm0Probe4,         uint32_t);
CREATE_PUBVAR(XmsPm0Probe5,         uint32_t);
CREATE_PUBVAR(XmsPm0Probe6,         uint32_t);
CREATE_PUBVAR(XmsPm0Probe7,         uint32_t);
CREATE_PUBVAR(XmsPm0Probe8,         uint32_t);
CREATE_PUBVAR(XmsPm0Probe9,         uint32_t);

CREATE_PUBVAR(XmsPm1Probe0,         uint32_t);
CREATE_PUBVAR(XmsPm1Probe1,         uint32_t);
CREATE_PUBVAR(XmsPm1Probe2,         uint32_t);
CREATE_PUBVAR(XmsPm1Probe3,         uint32_t);
CREATE_PUBVAR(XmsPm1Probe4,         uint32_t);
CREATE_PUBVAR(XmsPm1Probe5,         uint32_t);
CREATE_PUBVAR(XmsPm1Probe6,         uint32_t);
CREATE_PUBVAR(XmsPm1Probe7,         uint32_t);
CREATE_PUBVAR(XmsPm1Probe8,         uint32_t);
CREATE_PUBVAR(XmsPm1Probe9,         uint32_t);

CREATE_PUBVAR(IsrErrCtr,            unsigned) = 0;  // interrupt service error counter (early and late)
CREATE_PUBVAR(OverRunCtrT1,         unsigned) = 0;  // Task 1 overrun counter
CREATE_PUBVAR(OverRunCtrT2,         unsigned) = 0;  // Task 2 overrun counter
CREATE_PUBVAR(OverRunCtrT3,         unsigned) = 0;  // Task 3 overrun counter

CREATE_PUBVAR(OverRunLstpCtrT1,     unsigned) = 0;  // Task 1 overrun counter (counts if >1 in a row)
CREATE_PUBVAR(OverRunLstpCtrT2,     unsigned) = 0;  // Task 2 overrun counter (counts if >1 in a row)
CREATE_PUBVAR(OverRunLstpCtrT3,     unsigned) = 0;  // Task 3 overrun counter (counts if >1 in a row)


// Data Passing
//--------------
DATA_PASS(DiagResetReqT3,       DiagResetCmdT1,     T3_T1, UNFILTERED);
DATA_PASS(DiagResetReqT3,       DiagResetCmdT2,     T3_T2, UNFILTERED);


// Diagnostics
//------------
CREATE_DIAG(XopsIsrErrTrp);         // Xops interrupt service error
CREATE_DIAG(Tsk1OverRunTrp);        // Task 1 overrun
CREATE_DIAG(Tsk2OverRunTrp);        // Task 2 overrun
CREATE_DIAG(Tsk3OverRunTrp);        // Task 3 overrun
CREATE_DIAG(UtcUnLock);             // UTC time not locked


// Unpublished Variables
//----------------------
pthread_t           TidT3;                          // task3 thread id

SofDataTyp          IsrSofData;

sigevent            XopsEvent;                      // event for xopsScheduler
int                 XopsIrqNum = -1;
int                 XopsIrqId  = -1;
int                 XopsEvtId  = -1;

pthread_mutex_t     TimeSyncMutex;                  // guard EventTmOffs adjustment

sTimeType            UTCTime;

SysTm_Typ           EventTmOffs = { 0, 0 };         // EventTm offset from Task3

SysTm_Typ           SysTm1 = { 0, 0 };              // SysTm for Task1
SysTm_Typ           SysTm2 = { 0, 0 };              // SysTm for Task2
SysTm_Typ           SysTm3 = { 0, 0 };              // SysTm for Task3

SysTm_Typ           EventTm1 = { 0, 0 };            // EventTm for Task1
SysTm_Typ           EventTm2 = { 0, 0 };            // EventTm for Task2
SysTm_Typ           EventTm3 = { 0, 0 };            // EventTm for Task3

unsigned            IsrErrCtrZ = 0;                 // interrupt service error counter (early and late) previous value
unsigned            IsrErrCtrT3Z = 0;               // interrupt service error counter (early and late) previous value for Task3

unsigned            T1DoneOnce = false;
unsigned            T2DoneOnce = false;
unsigned            T3DoneOnce = false;
unsigned            TBDoneOnce = false;
unsigned            T3CalcDone = false;             // Task3 calcualtion done flag

unsigned            XopsSourceIsSysClock = false;

unsigned            DiagResetReqT3 = false;

uint32_t            FrmClkEst;                      // estimated cpu clock of the frame (Task3, for use by XMS)
uint64_t            FrmClkEstT1;                    // estimated cpu clock of the upcoming frame (Task1)
uint64_t            FrmClkEstT1A;                   // estimated cpu clock of the upcoming frame (Task1A)
uint64_t            IsrClkEst;                      // estimated cpu clock of the interrupt (for use by ISR)

intrspin_t          SpinLock;                       // spin lock variable used by InterruptLock/InterruptUnlock

ClkFrcValTyp        ClkFrcVal;
ClkFrcValTyp        ClkFrcValT3;

struct sigaction    seg_handler_oact;               // holds the context of the "original" qnx segment exception handler
struct sigaction    fpe_handler_oact;               // holds the context of the "original" qnx fpu exception handler

int                 Fpe_Enb   = 0;                  // floating point exception enable flag
int                 Fpe_Msk   = 0;                  // floating point exception mask
uint32_t            Fpe_Req   = false;              // request for task 1 to modify fp exeception mask
int                 Fpe_Signo = 0;
siginfo_t           Fpe_Info  = {0};
uint32_t            Fpe_Ctr   = 0;

XmsProbeTyp         XmsProbe[XMS_PID_LEN];          // data for execution probe measurments

unsigned            XmsCalcEnb = 0;                 // enable Xms calculation from LLM
XmsBufTyp           XmsBuf0;                        // execution measurement buffer
XmsBufTyp           XmsBuf1;                        // execution measurement buffer
XmsBufTyp*          pXmsSampBuf = &XmsBuf0;         // execution measurement sample buffer pointer (start with 1st buffer)
XmsBufTyp*          pXmsCalcBuf = &XmsBuf1;         // execution measurement calulation buffer pointer (start with 2nd buffer)
XmsBufTyp*          pXmsBufSwap = NULL;             // intermediate pointer for buffer swapping
float               XmsFilSettleGuage;              // execution measurment low pass filter settling guage
double              XmsMinMaxAccumTm;               // execution measurement time since min/max reset

XmsInfoTyp          XmsGlobInfo[XMS_N_MEAS];        // global execution measurement information
unsigned            XmsMeasIdx[TDP_N_TASKS] = { XMS_XOPS, XMS_TSK1, XMS_TSK2, XMS_TSK3 };

#ifdef XMS_DEBUG_BUF
XmsBufTyp           XmsDebugBuf;                    // buffer for debugging the execution measurement system
XmsBufTyp*          pXmsDebugBuf = &XmsDebugBuf;    // pointer to execution measurement debugging buffer
#endif

#ifdef XOPS_PERF_MON_TASK
PM_TaskTyp          PM_Task[4];
int                 PM_TrigTask = -1;
bool                PM_TrigMin;
bool                PM_TrigMax;
PM_ProbeTyp         PM_Probe;

bool                TaskRunning[4];
int                 TaskInterrupted[4];
int                 TaskBadData[4];
const char*         TaskStartStr[4] = {"Task 0 Starting", "Task 1 Starting", "Task 2 Starting", "Task 3 Starting"};
const char*         TaskEndStr[4]   = {"Task 0 Ending",   "Task 1 Ending",   "Task 2 Ending",   "Task 3 Ending"  };
#endif


// task execution control structure
TaskDataTyp TaskData[TDP_N_TASKS] =
{
    { TDP_SCHED, PRIORITY_PWRCNV_XOPS,  NULL,           NULL,    NULL,      0, 0, 0, 0, 0, 0, {NULL} },
    { TDP_TASK1, PRIORITY_PWRCNV_TASK1, &T1FramesPerT2, &SysTm1, &EventTm1, 0, 0, 0, 0, 0, 0, {NULL} },
    { TDP_TASK2, PRIORITY_PWRCNV_TASK2, &T2FramesPerT3, &SysTm2, &EventTm2, 0, 0, 0, 0, 0, 0, {NULL} },
    { TDP_TASK3, PRIORITY_PWRCNV_TASK3, &T3FramesPerTn, &SysTm3, &EventTm3, 0, 0, 0, 0, 0, 0, {NULL} }
};


// Static Class Members (init'd to zero by default)
//-------------------------------------------------
float   cMarkStatAlphaWdog::Period=0;


// Local Prototypes (to resolve forward references)
//-------------------------------------------------
void    xopsTaskOverRun( TaskDataTyp* tdp );
void    xopsTaskExecFltDetect( void );
void    SyncTimeT3( void );
void    XmsCalc( void );
void    XmsProbeCalc ( float DelTm );

#ifdef XOPS_PERF_MON_TASK
void    PerfMonTaskStart(int TaskNum);
void    PerfMonTaskEnd(int TaskNum);
void    PerfMonCalc(void);
void    PerfMonProbeStart(unsigned StartOfFrame);
void    PerfMonProbeEnd(void);
void    PerfMonProbeCalc(void);
#endif


// Class Instances
//----------------

//-----------------------------------------------------------------------------
// alpha watch dog to check pups
cMarkStatAlphaWdog  MarkStatAlphaWdog;

//-----------------------------------------------------------------------------
// list of registered dogs to be watched
cSLinkedList <cMarkStatWdog*> RegDogList;

//-----------------------------------------------------------------------------
// scheduler watchdog sign-up
cMarkStatWdog       MarkStatWdog_Sched;
INSTANCE_INFO(MarkStatWdog_Sched, "Scheduler", USE_XLS, TASK1)

//-----------------------------------------------------------------------------
// task1 watchdog sign-up
cMarkStatWdog       MarkStatWdog_T1;
INSTANCE_INFO(MarkStatWdog_T1, "Task 1", USE_XLS, TASK1)

//-----------------------------------------------------------------------------
// task2 watchdog sign-up
cMarkStatWdog       MarkStatWdog_T2;
INSTANCE_INFO(MarkStatWdog_T2, "Task 2", USE_XLS, TASK1)

//-----------------------------------------------------------------------------
// task3 watchdog sign-up
cMarkStatWdog       MarkStatWdog_T3;
INSTANCE_INFO(MarkStatWdog_T3, "Task 3", USE_XLS, TASK1)

//-----------------------------------------------------------------------------
// background watchdog sign-up
cMarkStatWdog       MarkStatWdog_TB;
INSTANCE_INFO(MarkStatWdog_TB, "Task Bkgnd", USE_XLS, TASK1)

//-----------------------------------------------------------------------------
// syncd memory client
cSyncMemClient      Syncd;

//-----------------------------------------------------------------------------
// external simulator interface
cXsimInterface      XsimInterface;


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      xopsISR is the interrupt service routine (ISR) which serves as the
//      master timebase for Task 1,2,3 execution.  This ISR is generated when
//      a system free running counter (FRC) matches the value programmed into
//      an FRC match register.  This ISR is responsible for checking to make
//      sure the source of the interrupt was the match register that it is
//      using, and if so, then it re-schedules the next match time and
//      triggers the execution of xopsScheduler interrupt service thread (IST).
//
// EXECUTION LEVEL
//      interrpt service routine, typically int 0x09 shared with SYNCD
//
// RETURN VALUE
//      sigevent* to trigger execution of the IST
//
///////////////////////////////////////////////////////////////////////////////
const sigevent* xopsISR( void* pX, int id )
{

    //-------------------------------------------------------------------------
    // check to see if this is my interrupt
    if ( pFrcSched->IsMyInterrupt() )
    {
        //---------------------------------------------------------------------
        // clear the current interrupt request
        //  it appears that clearing the interrupt first instead of last in the ISR
        //  and specifiying in InterruptAttach() that this ISR is serviced last
        //  reduces the max start time for the IST from ~65us to ~45us on a
        //  1066 mhz ucsb.
        pFrcSched->ClearInterrupt();

        //---------------------------------------------------------------------
        // capture fame and isr timing data for the current frame
        IsrSofData.IsrClkRd = ClockCycles();
        IsrSofData.FrmFrc = FrameFrc;
        IsrSofData.IsrFrc = IsrFrc;

        //---------------------------------------------------------------------
        // advance frame FRC and interrupt FRC
        if ( !SimMode )    // instead of XSimMode by SimMode, CL, 2015-02-02
        {
            //---------------------------------------------------------------------
            // advance the frame FRC for the next frame boundary
            FrameFrcZ = FrameFrc;
            FrameFrc = (FrameFrc + Clk0PerT1) & FrcSchedMask;

            //---------------------------------------------------------------------
            // set the interrupt FRC relative to the frame FRC
            IsrFrc = FrameFrc + XopsIsrFrmOffFrc;
        }
        else
        {
            //---------------------------------------------------------------------
            // in Xsim advance the frame FRC only if xopsTask1 has completed
            if ( !TaskData[TDP_TASK1].BusyCtr )
            {
                FrameFrcZ = FrameFrc;
                FrameFrc = (FrameFrc + Clk0PerT1) & FrcSchedMask;
            }

            //---------------------------------------------------------------------
            // always advance the interrupt FRC independent of the FrameFrc
            IsrFrc += DelIsrFrc;
        }

        //---------------------------------------------------------------------
        // detect if interrupt is serviced early or late
        //  (always expect it to be a little late)
        IsrSvcLatency = (unsigned)(IsrSofData.IsrClkRd - IsrClkEst);
        if ( IsrSvcLatency < IsrSvcErrThresh )
        {
            // schedule the next interrupt
            pFrcSched->SetCompare(IsrFrc, false);
        }
        else
        {
            // set error flag for recovery action to be taken by xopsSched
            ++IsrErrCtr;
        }

        //---------------------------------------------------------------------
        // send event to run XopsScheduler thread
        return ( &XopsEvent );
    }

    //-------------------------------------------------------------------------
    // if we have gotten here, its not my interrupt
    return ( NULL );
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      xopsSched() is the interrupt service thread (IST) that coordinates
//      execution of Task 1,2,3.  Normally this IST is triggered from an
//      interrupt sevice routine (ISR) that is attached to a hardware interrupt
//      source that is triggered by a free-running counter (FRC) match condition.
//      This resouce is allocated by xopsInit using the the core cFrcSched
//      class.  This class provides a common set of methods to manaage the FRC
//      interrupt source independent of the underlying hardware.  These common
//      methods are accessed throught the pFrcSched pointer.  In the case that
//      allocation of the hardware interrupt resource failed (pFrcSched==NULL),
//      then this IST is attached to the system time-tic interrupt (1 msec).
//
// EXECUTION LEVEL
//      xopsScheduler
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void* xopsSched(void* arg)
{
    uint32_t        StartClk;
    uint32_t        EndClk;
    unsigned        XmsSidx;


#ifdef XOPS_DISTRIBUTED_TASK_CONTROL
#else
    TaskDataTyp*        tdp;
#endif

#ifdef XOPS_EX2100_SCHED_METHOD
    _pulse  pulse;
    int     rc;
    int     chid;
#endif

    //-------------------------------------------------------------------------
    // request I/O privileges for this thread
    ThreadCtl( _NTO_TCTL_IO, 0 );

    //-------------------------------------------------------------------------
    // setup the performance monitoring control registers
    {
        uint32_t oCR4, nCR4;

        // allow read (ReadPmc) at all privilege levels
        ReadCr  (4, &oCR4);
        nCR4 = oCR4 | 0x00000100;
        WriteCr (4, nCR4);
        ReadCr  (4, &nCR4);

        // assign the event counters
        WriteMsr (MSR_PERF_EVT_SEL1, MSR_DEMAND_DATA_READ_L2_HIT);  // demand data read requests that hit L2 cache
        WriteMsr (MSR_PERF_EVT_SEL0, MSR_INSTRUCT_RETIRED);         // number of instructions at retirement
    }

#ifdef XOPS_EX2100_SCHED_METHOD
    chid = ChannelCreate(0);
    if ( chid == -1 )
    {
        fprintf(stderr, "ChannelCreate() error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    rc = sync_attach_sched(_PULSE_CODE_MINAVAIL, 0, 250000, getprio(0), chid);
    if ( rc == -1 )
    {
        fprintf(stderr, "sync_attach_sched() error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
#else
    //-------------------------------------------------------------------------
    // initialize the sigevent structure used to send an interrupt notification to this thread
    SIGEV_INTR_INIT(&XopsEvent);

    //-------------------------------------------------------------------------
    // if the hardware interrupt souce has been successfully allocated
    // then attach the xopsISR to that source and schedule the first interrupt
    if ( pFrcSched != NULL )
    {
        // get the number of the FRC interrupt source
        XopsIrqNum = pFrcSched->GetIrq();

        // attach the interrupt service routine (ISR) to the FRC interrupt source
        XopsIrqId  = InterruptAttach( XopsIrqNum, xopsISR, NULL, 0, _NTO_INTR_FLAGS_TRK_MSK | _NTO_INTR_FLAGS_END );

        if (XopsIrqId < 0)
        {
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                cParamString("xopsISR failed InterruptAttach  (%d,%s)", errno, strerror(errno)).c_str()),
                HERE );
        }
        else
        {
            MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                cParamString("xopsSched ISR attached to FRC interrupt source").c_str()),
                HERE );

            // get a synchronous snapshot of the cpu clock and the frc clock
            InterruptLock(&SpinLock);
            ClkFrcVal.ClkVal = ClockCycles();
            ClkFrcVal.FrcVal = (uint32_t)pFrcSched->GetCounter();
            InterruptUnlock(&SpinLock);

            // calculate FRC time for the first frame 200 msec seconds from now
            FrameFrc = ClkFrcVal.FrcVal;
            FrameFrc = (FrameFrc + (unsigned)(0.2 * FrcFreq)) & FrcSchedMask;
            FrameFrcT1A = FrameFrc;

            // calculate and schedule the first interrupt
            IsrFrc = FrameFrc + XopsIsrFrmOffFrc;
            IsrClkEst = ClkFrcVal.ClkVal + (uint64_t)(FrcClkToCpuClk * (IsrFrc - ClkFrcVal.FrcVal));
            pFrcSched->EnableInterrupt(IsrFrc);
        }
    }
    else
    {
        MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
            cParamString("xopsSched ISR attached to System Timer Event (1ms)").c_str()), HERE );

        // no hardware FRC so attach xopsScheduler to a system timer event (1ms) instead
        XopsSourceIsSysClock = true;
        XopsEvtId = InterruptAttachEvent(CLOCK_INTR, &XopsEvent, _NTO_INTR_FLAGS_TRK_MSK);
    }
#endif


    //-------------------------------------------------------------------------
    // setup watchdog
    MarkStatWdogRegistration Reg;
    Reg.Name      = "Scheduler";
    Reg.Period    = DelTm1 * SimTmRatio;
    Reg.AvgMin    = DelTm1 * SimTmRatio * 0.5;
    Reg.AvgMax    = DelTm1 * SimTmRatio * 2.0;
    Reg.AvgCount  = 20;
    Reg.InitTimer = 2.0;
    MarkStatWdog_Sched.Register(Reg);


    //-------------------------------------------------------------------------
    do
    {

#ifdef XOPS_EX2100_SCHED_METHOD
        rc = MsgReceivePulse(chid, &pulse, sizeof pulse, NULL);
        if ( rc == -1 )
        {
            fprintf(stderr, "MsgReceivePulse() error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        //---------------------------------------------------------------------
        // capture fame and isr timing data for the current frame
        InterruptDisable();
        IsrSofData.IsrClkRd = (uint32_t)ClockCycles();
        IsrSofData.IsrFrcRd = (uint32_t)pFrcSched->GetCounter();
        InterruptEnable();
        IsrSofData.FrmFrc = FrameFrc;
        IsrSofData.IsrFrc = IsrFrc;
        IsrSvcLatencyFrc = IsrSofData.IsrFrcRd - IsrFrc;

        //---------------------------------------------------------------------
        // caclulate the FRC for the upcoming interrupt and frame boundary
        IsrFrc = (uint32_t)pFrcSched->GetCompare();
        FrameFrcZ = FrameFrc;
        FrameFrc = IsrFrc - XopsIsrFrmOffFrc;

#else
        //---------------------------------------------------------------------
        // block here waiting for the signal from the ISR via XopsEvent
        if ( InterruptWait(0, NULL) < 0 )
        {
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_XOPS_STR,
                cParamString("InterruptWait error  (%d,%s)", errno, strerror(errno)).c_str()),
                HERE );
        }
#endif

        //---------------------------------------------------------------------
        // record XMS data for this task, save to buffer later
        StartClk = (uint32_t)ClockCycles();

        //---------------------------------------------------------------------
        // interrupt did not get serviced in a timely fashion
        //  (cannot do this recovery work in isr because multiplies cannot be done there)
        if ( IsrErrCtr != IsrErrCtrZ )
        {
            // estimate current frc based on cpu clock cycles
            FrameFrc = (ClkFrcVal.FrcVal + (uint32_t)(CpuClkToFrcClk * (int)(ClockCycles() - ClkFrcVal.ClkVal))) & FrcSchedMask;

            // advance the frame FRC for the next frame boundary
            FrameFrcZ = FrameFrc;
            FrameFrc = (FrameFrc + Clk0PerT1) & FrcSchedMask;

            // set the interrupt FRC relative to the frame FRC
            IsrFrc = FrameFrc + XopsIsrFrmOffFrc;

            // schedule the next interrupt
            pFrcSched->SetCompare(IsrFrc, false);
#if 0
            if ( TraceLoggerArm )
            {
                TraceLoggerArm = false;
                // ***DEBUG*** generate a profiler event
                TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_ISR_LATENCY, "XOPS ISR SVC LATENCY TRIGGER" );
                // ***DEBUG*** generate an event to trigger the the PCI bus analyzer
                TriggerVmetro(0x1234);
            }
#endif
            IsrErrCtrZ = IsrErrCtr;
        }

        //---------------------------------------------------------------------
        // estimate cpu clock value at upcoming ISR and frame
        IsrClkEst = ClkFrcVal.ClkVal + (uint64_t)(FrcClkToCpuClk * (int)((IsrFrc - ClkFrcVal.FrcVal) & FrcSchedMask));
        FrmClkEstT1 = IsrClkEst - (uint64_t)XopsIsrFrmOffCpu;

        //---------------------------------------------------------------------
        // go back and wait for the next interrupt if the Xsim calc still busy
        if ( SimMode && TaskData[TDP_TASK1].BusyCtr )        // instead of XSimMode by SimMode, CL, 2015-02-02
        {
            // update the synchronous snapshot of the cpu clock and the frc clock
            InterruptLock(&SpinLock);
            ClkFrcVal.ClkVal = ClockCycles();
            ClkFrcVal.FrcVal = (uint32_t)pFrcSched->GetCounter();
            InterruptUnlock(&SpinLock);

            continue;   // return to waiting at the the while(1)
        }

        //---------------------------------------------------------------------
        // update local copy of XMS sample index
        XmsSidx = pXmsSampBuf->Sidx[XMS_XOPS];
        // record XMS start-of-frame data
        pXmsSampBuf->SofData[ XmsSidx ] = IsrSofData;

        //---------------------------------------------------------------------
        // increment System Time and update Event Time
        if ( (SysTm1.nSec += DelSysTm) > 1000000000 )
        {
            SysTm1.nSec -= 1000000000;
            ++SysTm1.Sec;
        }
        if ( pthread_mutex_lock(&TimeSyncMutex) == EOK )
        {
            //  mutex ensures coherency with consumers of EventTm1 and producer of EventTmOffs
            EventTm1.Sec  = SysTm1.Sec  + EventTmOffs.Sec;
            EventTm1.nSec = SysTm1.nSec + EventTmOffs.nSec;
            if ( EventTm1.nSec > 1000000000 )
            {
                EventTm1.nSec -= 1000000000;
                ++EventTm1.Sec;
            }
            pthread_mutex_unlock(&TimeSyncMutex);
        }

#ifdef XOPS_DISTRIBUTED_TASK_CONTROL
#else
        //---------------------------------------------------------------------
        // task state control
        for ( tdp = &TaskData[TDP_TASK1] ; tdp <= &TaskData[TDP_TASK3] ; ++tdp )
        {
            if ( (tdp-1)->State == 0 )
            {
                // record the task XMS start-of-frame data index
                pXmsSampBuf->Data[ XmsMeasIdx[tdp->TaskNo] ][ pXmsSampBuf->Sidx[XmsMeasIdx[tdp->TaskNo]] ].SofDataIdx = XmsSidx;

                // update the task system and event time (Task1 times already done)
                if ( tdp->TaskNo != TDP_TASK1 )
                {
                    *tdp->pSysTm = SysTm1;
                    *tdp->pEvtTm = EventTm1;
                }

                // advance the task state counter
                if ( ++tdp->State >= *tdp->pFrames )
                {
                    tdp->State = 0;
                }

                // check if the task finished work from last time
                if ( !tdp->BusyCtr )
                {
                    tdp->OverRunLstpFlg = false;
                }
                else
                {
                    xopsTaskOverRun( tdp );
                    tdp->OverRunLstpFlg = true;
                }
                ++(tdp->BusyCtr);

                // make the task ready to run
                sem_post( &tdp->Sem );
            }
            else
            {
                break;  // exit the "for" loop
            }
        }
#endif

        #ifdef XOPS_PERF_MON_TASK
        if ( TaskFrameSum == T3FrameSum )
        {
            PerfMonCalc();
            PerfMonProbeCalc();
        }
        #endif

        //---------------------------------------------------------------------
        // record XMS data for this task
        if ( XmsCalcEnb )
        {
            EndClk = (uint32_t)ClockCycles();
            pXmsSampBuf->Data[ XMS_XOPS ][ XmsSidx ].StartClk   = StartClk;
            pXmsSampBuf->Data[ XMS_XOPS ][ XmsSidx ].SofDataIdx = XmsSidx;
            pXmsSampBuf->Data[ XMS_XOPS ][ XmsSidx ].EndClk     = EndClk;
            XmsXopsSched = CpuCpsRcpUsec * (int)(EndClk - StartClk);
            if ( ++XmsSidx >= XMS_N_SAMPLES )
            {
                XmsSidx = 0;
            }
            pXmsSampBuf->Sidx[XMS_XOPS] = XmsSidx;
        }

        //---------------------------------------------------------------------
        // tickle watchdog
        // occurs at the end of the loop to ensure all code has an opportunity to run
        MarkStatWdog_Sched.Tickle();

        //---------------------------------------------------------------------
        // re-enable the tic interrupt only if triggered from the system clock
        if ( XopsEvtId != -1 )
        {
            InterruptUnmask(0, XopsEvtId);
        }
#ifdef MARKSTAT_DLL
    } while (0);    // do this once
#else
    } while (1);    // do forever
#endif

    //-------------------------------------------------------------------------
    // should never get here
    return NULL;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Driver for Task 1A
//
// EXECUTION LEVEL
//      Task1A
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void xopsTask1A ( void )
{
    unsigned        XmsSidx;
    uint32_t        StartClk;
    uint32_t        EndClk;

    //---------------------------------------------------------------------
    // record XMS data for this task, save to buffer later
    StartClk = (uint32_t)ClockCycles();

#ifdef XOPS_TRACE_TASK
    TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_TASK1A, "TASK1A THREAD STARTING" );
#endif

    //-------------------------------------------------------------------------
    // debug spin wait for some # of usec
    if ( SpinWaitCtrT1A != 0 )
    {
        uint64_t SpinWaitStrt = ClockCycles();
        while ( ((ClockCycles() - SpinWaitStrt) * CpuCpsRcpUsec) < PARM(SpinWaitT1A) )
        {
            // spin here for specified time
        }
        // if <0 spin every pass, otherwise only for specified number of passes
        if ( SpinWaitCtrT1A > 0 )
        {
            --SpinWaitCtrT1A;
        }
    }

    //---------------------------------------------------------------------
    // product specific task
    Task1A();

    //---------------------------------------------------------------------
    // data pass
    DataPassManager.AccumFil(T1A2F);
    DataPassManager.AccumFil(T1A3F);
    if ( T1ASt == 0 )
    {
        // task2 frame boundary
        DataPassManager.Dispatch(T1A2);  // dispatch items going to Task2
        DataPassManager.Dispatch(T1A2F);
        if ( T2St == 0 )
        {
            // task3 frame boundary
            DataPassManager.Dispatch(T1A3);  // dispatch items going to Task3
            DataPassManager.Dispatch(T1A3F);
        }
    }

    //---------------------------------------------------------------------
    // record XMS data for this task
    //  for SOF data, must backup one Task1 sample index because it has alreaded been incremented
    if ( XmsCalcEnb )
    {
        EndClk = (uint32_t)ClockCycles();
        XmsSidx = pXmsSampBuf->Sidx[XMS_TSK1A];
        pXmsSampBuf->Data[ XMS_TSK1A ][ XmsSidx ].SofDataIdx = pXmsSampBuf->Data[ XMS_TSK1 ][ pXmsSampBuf->Sidx[XMS_TSK1]-1 ].SofDataIdx;
        pXmsSampBuf->Data[ XMS_TSK1A ][ XmsSidx ].StartClk   = StartClk;
        pXmsSampBuf->Data[ XMS_TSK1A ][ XmsSidx ].EndClk     = EndClk;
        XmsXopsTask1A = CpuCpsRcpUsec * (int)(EndClk - StartClk);
        if ( ++XmsSidx >= XMS_N_SAMPLES )
        {
            XmsSidx = 0;
        }
        pXmsSampBuf->Sidx[XMS_TSK1A] = XmsSidx;
    }

    //---------------------------------------------------------------------
    // run task1A diagnostic filters
    if (pGpDiagFilterT1A != NULL)
    {
        pGpDiagFilterT1A->Task1();
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Driver for Simulator
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void xopsTask1S ( void )
{
    unsigned        XmsSidx;
    uint32_t        StartClk;
    uint32_t        EndClk;
    bool            Task1ASimFbkReq = false;        // flag to request Task1A simulator feedback copy

    //---------------------------------------------------------------------
    // record XMS data for this task, save to buffer later
    StartClk = (uint32_t)ClockCycles();

#ifdef XOPS_TRACE_TASK
    TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_TASK1S, "TASK1_SIM THREAD STARTING" );
#endif

    //---------------------------------------------------------------------
    // request a Task1A simulator feedback copy if Task1A regulators will execute in the next Task1
    if ( ((PARM(T1AExecMask) >> T1St) & 0x2) != 0 )
    {
        Task1ASimFbkReq = true;
    }

    //-------------------------------------------------------------------------
    // debug spin wait for some # of usec
    if ( SpinWaitCtrT1S != 0 )
    {
        uint64_t SpinWaitStrt = ClockCycles();
        while ( ((ClockCycles() - SpinWaitStrt) * CpuCpsRcpUsec) < PARM(SpinWaitT1S) )
        {
            // spin here for specified time
        }
        // if <0 spin every pass, otherwise only for specified number of passes
        if ( SpinWaitCtrT1S > 0 )
        {
            --SpinWaitCtrT1S;
        }
    }

    //---------------------------------------------------------------------
    // call the simulator interface task
    Task1S( Task1AExecReq, Task1ASimFbkReq );

    //---------------------------------------------------------------------
    // record XMS data for this task
    //  for SOF data, must backup one Task1 sample index because it has alreaded been incremented
    if ( XmsCalcEnb )
    {
        EndClk = (uint32_t)ClockCycles();
        XmsSidx = pXmsSampBuf->Sidx[XMS_TSK1S];
        pXmsSampBuf->Data[ XMS_TSK1S ][ XmsSidx ].SofDataIdx = pXmsSampBuf->Data[ XMS_TSK1 ][ pXmsSampBuf->Sidx[XMS_TSK1]-1 ].SofDataIdx;
        pXmsSampBuf->Data[ XMS_TSK1S ][ XmsSidx ].StartClk   = StartClk;
        pXmsSampBuf->Data[ XMS_TSK1S ][ XmsSidx ].EndClk     = EndClk;
        XmsXopsTask1S = CpuCpsRcpUsec * (int)(EndClk - StartClk);
        if ( ++XmsSidx  >= XMS_N_SAMPLES )
        {
            XmsSidx  = 0;
        }
        pXmsSampBuf->Sidx[XMS_TSK1S]= XmsSidx;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Task 1 execution thread
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void* xopsTask1(void *vp)
{
    TaskDataTyp*    tdp = (TaskDataTyp*)vp;
    uint32_t        StartClk;
    uint32_t        EndClk;
    unsigned        XmsSidx;


    //-------------------------------------------------------------------------
    // setup thread execution environment
    ThreadCtl( _NTO_TCTL_IO, 0 );   // request I/O privileges for this thread
    DENORM_OFF                      // enable hardware suppression of floating point denormals

    //-------------------------------------------------------------------------
    // setup watchdog
    MarkStatWdogRegistration Reg;
    Reg.Name      = "Task 1";
    Reg.Period    = DelTm1 * SimTmRatio;
    Reg.AvgMin    = DelTm1 * SimTmRatio * 0.5;
    Reg.AvgMax    = DelTm1 * SimTmRatio * 2.0;
    Reg.AvgCount  = 20;
    Reg.InitTimer = 2.0;
    MarkStatWdog_T1.Register(Reg);

    //-------------------------------------------------------------------------
#ifndef MARKSTAT_DLL
    // loop forever
    while (1)
#endif
    {
        //---------------------------------------------------------------------
        // task synchronization
        sem_wait( &tdp->Sem );

        //---------------------------------------------------------------------
        // record XMS data for this task, save to buffer later
        StartClk = (uint32_t)ClockCycles();

#ifdef XOPS_PERF_MON_TASK
        PerfMonTaskStart(1);
#endif

        //---------------------------------------------------------------------
        // update the task information used by the application code
        //  snapshot task information that is modified in xopsScheduler() so that this execution
        //  of Task1 thread will have consistent usage even if interrupted by xopsScheduler()
        //  NOTE - T1ASt is used in Task1 so must be updated here
        T1St = tdp->State;
        T2St = (tdp+1)->State;
        T3St = (tdp+2)->State;
        T1T2St = T1St + ( T2St * T1FramesPerT2 );
        if ( ((PARM(T1AExecMask) >> T1St) & 0x1) != 0 )
        {
            Task1AExecReq = true;
            FrameFrcT1AZ = FrameFrcT1A;
            if ( T1St == 0 )
            {
                // re-sync to the Task1 frame at every Task2 boundary
                T1ASt = 0;
                FrameFrcT1A = (FrameFrcZ + Clk0PerT1A) & FrcSchedMask;
            }
            else
            {
                // incremental from previous Task2 boundary
                ++T1ASt;
                FrameFrcT1A = (FrameFrcT1A + Clk0PerT1A) & FrcSchedMask;
            }
            // estimate cpu clock value at upcoming T1A frame
            FrmClkEstT1A = FrmClkEstT1 + (uint64_t)(FrcClkToCpuClk * (int)(FrameFrcT1A - FrameFrc));

            T1AT2St = T1ASt + ( T2St * T1AFramesPerT2 );
        }

        //---------------------------------------------------------------------
        // reset the diagnost reseet command datapassed from T3 on the previous T1 exec
        DiagResetCmdT1 = false;

        //---------------------------------------------------------------------
        // data pass
        if ( T1St == 0 )
        {
            // task2 frame boundary
            DataPassManager.Dispatch(T21);  // dispatch items coming from Task2
            if ( T2St == 0 )
            {
                // task3 frame boundary
                DataPassManager.Dispatch(T31);  // dispatch items coming from Task3
            }
        }

        //-------------------------------------------------------------------------
        // debug spin wait for some # of usec
        if ( SpinWaitCtrT1 != 0 )
        {
            uint64_t SpinWaitStrt = ClockCycles();
            while ( ((ClockCycles() - SpinWaitStrt) * CpuCpsRcpUsec) < PARM(SpinWaitT1) )
            {
                // spin here for specified time
            }
            // if <0 spin every pass, otherwise only for specified number of passes
            if ( SpinWaitCtrT1 > 0 )
            {
                --SpinWaitCtrT1;
            }
        }

        //---------------------------------------------------------------------
        // product specific task
        Task1();

        //---------------------------------------------------------------------
        // data pass
        DataPassManager.AccumFil(T12F);
        DataPassManager.AccumFil(T13F);
        if ( T1St == 0 )
        {
            // task2 frame boundary
            DataPassManager.Dispatch(T12);  // dispatch items going to Task2
            DataPassManager.Dispatch(T12F);
            if ( T2St == 0 )
            {
                // task3 frame boundary
                DataPassManager.Dispatch(T13);  // dispatch items going to Task3
                DataPassManager.Dispatch(T13F);
            }
        }

#ifdef XOPS_DISTRIBUTED_TASK_CONTROL
            // xms start of frame data for next task
            pXmsSampBuf->Data[XMS_TSK2][pXmsSampBuf->Sidx[XMS_TSK2]].SofDataIdx = pXmsSampBuf->Data[XMS_TSK1][pXmsSampBuf->Sidx[XMS_TSK1]].SofDataIdx;

            // pointer to next task data
            TaskDataTyp* tdpnxt = tdp + 1;

            // system and event time for next task
            *tdpnxt->pSysTm = *tdp->pSysTm;
            *tdpnxt->pEvtTm = *tdp->pEvtTm;

            // thread control for next task
            if ( tdpnxt->Busy )
            {
                xopsTaskOverRun( tdpnxt );
            }
            tdpnxt->Busy = true;
            sem_post( &(tdpnxt->Sem) );
        }

        // thread control for this task
        if ( ++tdp->State >= *tdp->pFrames )
        {
            tdp->State = 0;
#endif

#ifdef XOPS_FPE
        //---------------------------------------------------------------------
        // enable/disable float point exceptions (special for debugging, from llm command)
        if ( Fpe_Req )
        {
            fp_exception_mask( Fpe_Msk, Fpe_Enb );
            Fpe_Req = false;
        }
#endif

#ifdef XOPS_PERF_MON_TASK
        PerfMonTaskEnd(1);
#endif

        //---------------------------------------------------------------------
        // record XMS data for this task
        if ( XmsCalcEnb )
        {
            EndClk = (uint32_t)ClockCycles();
            XmsSidx = pXmsSampBuf->Sidx[XMS_TSK1];
            pXmsSampBuf->Data[ XMS_TSK1 ][ XmsSidx ].StartClk = StartClk;
            pXmsSampBuf->Data[ XMS_TSK1 ][ XmsSidx ].EndClk   = EndClk;
            XmsXopsTask1 = CpuCpsRcpUsec * (int)(EndClk - StartClk);
            if ( ++XmsSidx >= XMS_N_SAMPLES )
            {
                XmsSidx = 0;
            }
            pXmsSampBuf->Sidx[XMS_TSK1]= XmsSidx;
        }

        //---------------------------------------------------------------------
        // Task1A exeuction control
        if ( Task1AExecReq )
        {
            xopsTask1A();
        }

        //---------------------------------------------------------------------
        // simulator exectuion control
        if ( SimMode )
        {
            xopsTask1S();
        }

        //---------------------------------------------------------------------
        // run task1 diagnostic filters
        if (pGpDiagFilterT1 != NULL)
        {
            pGpDiagFilterT1->Task1();
        }

        //---------------------------------------------------------------------
        // collect data for capture buffer
        CaptureBuf.Task1();
        TripBuf.Task1();

        //---------------------------------------------------------------------
        // clear the Task1A execution request (after capture buffer so it can be sampled)
        Task1AExecReq = false;

        //---------------------------------------------------------------------
        // xms probe calculations
        if ( PARM(XmsProbeSite) == XMS_TSK1 )
        {
            XmsProbeCalc( DelTm1 );
        }

        //---------------------------------------------------------------------
        // if this is the last Task1 in a Task3 frame then swap XMS buffers
        //  (assumes Task3 will always be completed before the last Task1 execution in a Task3 frame)
        if ( T1T2St+1 >= T1FramesPerT3 )
        {
            pXmsBufSwap = pXmsCalcBuf;
            pXmsCalcBuf = pXmsSampBuf;
            pXmsSampBuf = pXmsBufSwap;
        }

        //---------------------------------------------------------------------
        // set the done flags
        tdp->DoneOnce = true;
        T1DoneOnce    = true;

        //---------------------------------------------------------------------
        // tickle watchdog
        // occurs at the end of the loop to ensure all code has an opportunity to run
        MarkStatWdog_T1.Tickle();

        //---------------------------------------------------------------------
        // decrement the task busy flag
        --(tdp->BusyCtr);

#if 0
        //---------------------------------------------------------------------
        // execution measurements for the capture buffer from the previous frame
        uint32_t XmsTask1FrmClk;
        XmsTask1FrmClk  = pXmsSampBuf->Data[XMS_TSK1][pXmsSampBuf->Sidx[XMS_TSK1]].IsrSofData.IsrClkRd - (uint32_t)(FrcClkToCpuClk * (pXmsSampBuf->Data[XMS_TSK1][XmsSampIdx].IsrSofData.IsrFrcRd - pXmsSampBuf->Data[XMS_TSK1][XmsSampIdx].IsrSofData.IsrFrc));
        XmsTask1StartTm = (float)(pXmsSampBuf->Data[XMS_TSK1][pXmsSampBuf->Sidx[XMS_TSK1]].StartClk - XmsTask1FrmClk) * CpuCpsRcpUsec;
        XmsTask1EndTm   = (float)(pXmsSampBuf->Data[XMS_TSK1][pXmsSampBuf->Sidx[XMS_TSK1]].EndClk - XmsTask1FrmClk) * CpuCpsRcpUsec;

        //---------------------------------------------------------------------
        // allow a window for procnto isr
        InterruptUnmask(0, -1);
        InterruptMask(0, -1);

        //---------------------------------------------------------------------
        // allow a window for devc-ser8250 isr
        InterruptUnmask(4, -1);
        InterruptMask(4, -1);
#endif
    }

    //---------------------------------------------------------------------
    // should never get to here
    return NULL;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Task 2 execution thread
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void* xopsTask2(void *vp)
{
    TaskDataTyp*    tdp = (TaskDataTyp*)vp;
    uint32_t        StartClk;
    unsigned        XmsSidx;

    //-------------------------------------------------------------------------
    // setup thread execution environment
    ThreadCtl( _NTO_TCTL_IO, 0 );   // request I/O privileges for this thread
    DENORM_OFF                      // enable hardware suppression of floating point denormals

    //-------------------------------------------------------------------------
    // setup watchdog
    MarkStatWdogRegistration Reg;
    Reg.Name      = "Task 2";
    Reg.Period    = DelTm2 * SimTmRatio;
    Reg.AvgMin    = DelTm2 * SimTmRatio * 0.5;
    Reg.AvgMax    = DelTm2 * SimTmRatio * 2.0;
    Reg.AvgCount  = 20;
    Reg.InitTimer = 2.0;
    MarkStatWdog_T2.Register(Reg);

    //-------------------------------------------------------------------------
#ifndef MARKSTAT_DLL
    // loop forever
    while (1)
#endif
    {
        //---------------------------------------------------------------------
        // task synchronization
        sem_wait( &tdp->Sem );

        //---------------------------------------------------------------------
        // record XMS data for this task, save to buffer later
        StartClk = (uint32_t)ClockCycles();

#ifdef XOPS_PERF_MON_TASK
        PerfMonTaskStart(2);
#endif

        //---------------------------------------------------------------------
        // record the system cycle counter (CLK) and fpga free running counter (FRC)
        //  this is used to estimate the CLK value for a given FRC value
        //  do this first thing in Task2 so that the FRC read does not conflict with ISR/IST/Task1
        InterruptLock(&SpinLock);
        ClkFrcVal.ClkVal = ClockCycles();
        ClkFrcVal.FrcVal = (uint32_t)pFrcSched->GetCounter();
        InterruptUnlock(&SpinLock);

        //---------------------------------------------------------------------
        // reset the diagnost reseet command datapassed from T3 on the previous T2 exec
        DiagResetCmdT2 = false;

        //---------------------------------------------------------------------
        // data pass
        if ( tdp->State == 0 )
        {
            // task3 frame boundary
            DataPassManager.Dispatch(T32);  // dispatch items coming from Task3
        }

        //---------------------------------------------------------------------
        // inter-process data pass to MarkVie
        gIpcDevice.DisperseDataFrmM6e();

        //-------------------------------------------------------------------------
        // debug spin wait for some # of usec
        if ( SpinWaitCtrT2 != 0 )
        {
            uint64_t SpinWaitStrt = ClockCycles();
            while ( ((ClockCycles() - SpinWaitStrt) * CpuCpsRcpUsec) < PARM(SpinWaitT2) )
            {
                // spin here for specified time
            }
            // if <0 spin every pass, otherwise only for specified number of passes
            if ( SpinWaitCtrT2 > 0 )
            {
                --SpinWaitCtrT2;
            }
        }

        //---------------------------------------------------------------------
        // product specific task
        Task2();

        //---------------------------------------------------------------------
        // data pass
        DataPassManager.AccumFil(T23F);
        if ( tdp->State == 0 )
        {
            // task3 frame boundary
            DataPassManager.Dispatch(T23);  // dispatch items going to Task3
            DataPassManager.Dispatch(T23F);
            ClkFrcValT3 = ClkFrcVal;        // structure not handled by normal data passer

#ifdef XOPS_DISTRIBUTED_TASK_CONTROL
            // xms start of frame data for next task
            pXmsSampBuf->Data[XMS_TSK3][pXmsSampBuf->Sidx[XMS_TSK3]].SofDataIdx = pXmsSampBuf->Data[XMS_TSK2][pXmsSampBuf->Sidx[XMS_TSK2]].SofDataIdx;

            // pointer to next task data
            TaskDataTyp* tdpnxt = tdp + 1;

            // system and event time for next task
            *tdpnxt->pSysTm = *tdp->pSysTm;
            *tdpnxt->pEvtTm = *tdp->pEvtTm;

            // thread control for next task
            if ( tdpnxt->Busy )
            {
                xopsTaskOverRun( tdpnxt );
            }
            tdpnxt->Busy = true;
            sem_post( &tdpnxt->Sem );
        }

        // thread control for this task
        if ( ++tdp->State >= *tdp->pFrames )
        {
            tdp->State = 0;
#endif
        }

        //---------------------------------------------------------------------
        // run the diagnostic signal generator
        TestOscillator();

        //---------------------------------------------------------------------
        // inter-process communication test
        IpcCommTest();

        //---------------------------------------------------------------------
        // inter-process data pass to MarkVie
        gIpcDevice.CollectDataToM6e();

        //---------------------------------------------------------------------
        // collect data for capture buffer
        CaptureBuf.Task2();
        TripBuf.Task2();

#ifdef XOPS_PERF_MON_TASK
        PerfMonTaskEnd(2);
#endif

        //---------------------------------------------------------------------
        // record XMS data for this task
        if ( XmsCalcEnb )
        {
            XmsSidx = pXmsSampBuf->Sidx[XMS_TSK2];
            pXmsSampBuf->Data[ XMS_TSK2 ][ XmsSidx ].StartClk = StartClk;
            pXmsSampBuf->Data[ XMS_TSK2 ][ XmsSidx ].EndClk   = (uint32_t)ClockCycles();
            if ( ++XmsSidx >= XMS_N_SAMPLES )
            {
                XmsSidx = 0;
            }
            pXmsSampBuf->Sidx[XMS_TSK2]= XmsSidx;
        }

        //---------------------------------------------------------------------
        // xms probe calculations
        if ( PARM(XmsProbeSite) == XMS_TSK2 )
        {
            XmsProbeCalc( DelTm2 );
        }

        //---------------------------------------------------------------------
        // set the done flags
        tdp->DoneOnce = true;
        T2DoneOnce    = true;

        //---------------------------------------------------------------------
        // tickle watchdog
        // occurs at the end of the loop to ensure all code has an opportunity to run
        MarkStatWdog_T2.Tickle();

        //---------------------------------------------------------------------
        // decrement the task busy flag
        --(tdp->BusyCtr);
    }

    //---------------------------------------------------------------------
    // should never get to here
    return NULL;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Task 3 execution thread
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void* xopsTask3(void *vp)
{
    TaskDataTyp*    tdp = (TaskDataTyp*)vp;
    uint32_t        StartClk;
    unsigned        XmsSidx;

    //-------------------------------------------------------------------------
    // setup thread execution environment
    ThreadCtl( _NTO_TCTL_IO, 0 );   // request I/O privileges for this thread
    DENORM_OFF                      // enable hardware suppression of floating point denormals

    //-------------------------------------------------------------------------
    // setup watchdog
    MarkStatWdogRegistration Reg;
    Reg.Name      = "Task 3";
    Reg.Period    = DelTm3 * SimTmRatio;
    Reg.AvgMin    = DelTm3 * SimTmRatio * 0.5;
    Reg.AvgMax    = DelTm3 * SimTmRatio * 2.0;
    Reg.AvgCount  = 20;
    Reg.InitTimer = 2.0;
    MarkStatWdog_T3.Register(Reg);

    //-------------------------------------------------------------------------
#ifndef MARKSTAT_DLL
    // loop forever
    while (1)
#endif
    {
        //---------------------------------------------------------------------
        // task synchronization
        sem_wait( &tdp->Sem );

        //---------------------------------------------------------------------
        // record XMS data for this task, save to buffer later
        StartClk = (uint32_t)ClockCycles();

#ifdef XOPS_PERF_MON_TASK
        PerfMonTaskStart(3);
#endif

        //---------------------------------------------------------------------
        // set the flag to indicate an entire first Task3 frame has completed
        //  including the data passing that occurs at the beginning of the 2nd Task3 frame
        T3CalcDone = tdp->DoneOnce;

        //---------------------------------------------------------------------
        // manage the diagnostic resets (reset is true for the first T1, T2, T3 in a T3 frame)
        DiagResetCmdT3 = false;                     // clear the task3 diag reset command set on previour T3 exec
        MarkStatDiag.DiagnosticResetCheck();        // check for fault reset request from application control
        if ( DiagResetTaskReq )                     // new diagnostic reset request
        {
            DiagResetTaskReq = false;               // clear the request
            DiagResetReqT3   = true;                // set the T3 request for reset command in the next T3 and data passed to T1,T2
        }
        else if ( DiagResetReqT3 )
        {
            DiagResetReqT3 = false;                 // clear the T3 request for reset (set on previous T3 exec)
            DiagResetCmdT3 = true;                  // set the task3 diag reset command
        }

        //---------------------------------------------------------------------
        // for toolbox display
        SysTmSec     = SysTm3.Sec;
        SysTmMsec    = (unsigned)(SysTm3.nSec * 1.0e-6F);
        EventTmSec   = EventTm3.Sec;
        EventTmMsec  = (unsigned)(EventTm3.nSec * 1.0e-6F);

        //---------------------------------------------------------------------
        // check for any task execution problems
        xopsTaskExecFltDetect();

        //-------------------------------------------------------------------------
        // debug spin wait for some # of usec
        if ( SpinWaitCtrT3 != 0 )
        {
            uint64_t SpinWaitStrt = ClockCycles();
            while ( ((ClockCycles() - SpinWaitStrt) * CpuCpsRcpUsec) < PARM(SpinWaitT3) )
            {
                // spin here for specified time
            }
            // if <0 spin every pass, otherwise only for specified number of passes
            if ( SpinWaitCtrT3 > 0 )
            {
                --SpinWaitCtrT3;
            }
        }

        //---------------------------------------------------------------------
        // product specific task
        Task3();

#ifdef XOPS_DISTRIBUTED_TASK_CONTROL
        //---------------------------------------------------------------------
        // thread control
        if ( ++tdp->State >= *tdp->pFrames )
        {
            tdp->State = 0;
        }
#endif

        //---------------------------------------------------------------------
        // synchronize event time to UTC time
        SyncTimeT3();

        //---------------------------------------------------------------------
        // Keypad status writes -Leave these two together
        ToolboxStatus();

        //---------------------------------------------------------------------
        // execution measurment system calculation of statistics
        XmsCalc();

        //---------------------------------------------------------------------
        // check registered tasks, confirmed they are still alive
        if ( !XSimMode )
        {
            MarkStatAlphaWdog.CheckRegDogs();
        }

        //---------------------------------------------------------------------
        // sample data/timestamp to service ADL requests (from toolbox)
        if ( !XsimInterface.IsClientConnected() )
        {
            AdlDataSampler( DelTm3 );
        }

        //---------------------------------------------------------------------
        // collect data for capture buffer
        CaptureBuf.Task3();
        TripBuf.Task3();

#ifdef XOPS_PERF_MON_TASK
        PerfMonTaskEnd(3);
#endif

        //---------------------------------------------------------------------
        // record XMS data for this task
        if ( XmsCalcEnb )
        {
            XmsSidx = pXmsSampBuf->Sidx[XMS_TSK3];
            pXmsSampBuf->Data[ XMS_TSK3 ][ XmsSidx ].StartClk = StartClk;
            pXmsSampBuf->Data[ XMS_TSK3 ][ XmsSidx ].EndClk   = (uint32_t)ClockCycles();
            if ( ++XmsSidx >= XMS_N_SAMPLES )
            {
                XmsSidx = 0;
            }
            pXmsSampBuf->Sidx[XMS_TSK3]= XmsSidx;
        }

        //---------------------------------------------------------------------
        // xms probe calculations
        if ( PARM(XmsProbeSite) == XMS_TSK3 )
        {
            XmsProbeCalc( DelTm3 );
        }

        //---------------------------------------------------------------------
        // set the done flags
        tdp->DoneOnce = tdp->DoneOnce || TBDoneOnce;
        T3DoneOnce    = true;

        //---------------------------------------------------------------------
        // tickle watchdog
        // occurs at the end of the loop to ensure all code has an opportunity to run
        MarkStatWdog_T3.Tickle();

        //---------------------------------------------------------------------
        // decrement the task busy flag
        --(tdp->BusyCtr);
    }

    //---------------------------------------------------------------------
    // should never get to here
    return NULL;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Untimed background execution thread
//
// EXECUTION LEVEL
//      Background (untimed)
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void* xopsBkgnd(void *data)
{
    static unsigned TraceLoggerArmZ = false;

    DENORM_OFF

    //-------------------------------------------------------------------------
    // request I/O privileges for this thread
    //  needed for OS_ENTER/EXIT_CRITICAL()
    ThreadCtl( _NTO_TCTL_IO, 0 );

    //-------------------------------------------------------------------------
    // setup watchdog
    MarkStatWdogRegistration Reg;
    Reg.Name      = "Task Bkgnd";
    Reg.Period    = 0.5;
    Reg.AvgMin    = 0.500 * 0.5;
    Reg.AvgMax    = 0.500 * 4.0;
    Reg.AvgCount  = 20;
    Reg.InitTimer = 10.0;
    MarkStatWdog_TB.Register(Reg);

    //-------------------------------------------------------------------------
#ifndef MARKSTAT_DLL
    // loop forever
    while (1)
#endif
    {
        //---------------------------------------------------------------------
        // update the global diagnostic verbosity level
        gVerbosityLevel = MarkStatDiag.GetDiagnosticLevel();

        //---------------------------------------------------------------------
        // Update diag filter information
        if (pGpDiagFilterT1 != NULL)
        {
            pGpDiagFilterT1->SetupDiagFilters();
        }
        if (pGpDiagFilterT1A != NULL)
        {
            pGpDiagFilterT1A->SetupDiagFilters();
        }

        //---------------------------------------------------------------------
        // summerize diagnostic alarms, change index
        MarkStatDiag.HouseKeeping();

        //---------------------------------------------------------------------
        // Configure capture buffer from EE's
        CaptureBuf.ReConfigure();
        TripBuf.ReConfigure();
        // reenable capture buffer
        CaptureBuf.AutoEnable();
        TripBuf.AutoEnable();
        // this is the link to IPC and trender
        DSP_CapBufState = CaptureBuf.GetState();
        DSP_TrpBufState = TripBuf.GetState();

        //---------------------------------------------------------------------
        // run CRC check on the parms
        gParameter.CrcCheck(false);
        gParameter.FileCrcCheck();

        //---------------------------------------------------------------------
        // stop the tracelogger if trigger condition occurred (arm 1->0)
        //  (doing this in background allows some post trigger samples)
        if ( TraceLoggerArmZ && !TraceLoggerArm )
        {
            printf("\n");
            printf("Trace Logger trigger occurred\n");
            TraceEvent(_NTO_TRACE_STOP);
        }
        TraceLoggerArmZ = TraceLoggerArm;

        //-------------------------------------------------------------------------
        // debug spin wait for some # of usec
        if ( SpinWaitCtrB != 0 )
        {
            uint64_t SpinWaitStrt = ClockCycles();
            while ( ((ClockCycles() - SpinWaitStrt) * CpuCpsRcpUsec) < PARM(SpinWaitB) )
            {
                // spin here for specified time
            }
            // if <0 spin every pass, otherwise only for specified number of passes
            if ( SpinWaitCtrB > 0 )
            {
                --SpinWaitCtrB;
            }
        }

        //---------------------------------------------------------------------
        // product specific task
        TaskB();

        //---------------------------------------------------------------------
        TBDoneOnce = true;

        //---------------------------------------------------------------------
        //  sleep for a while
        if ( XsimInterface.IsClientConnected() )
        {
            OS_ENTER_CRITICAL();        // inhibit task interruption
            AdlDataSampler(50e-3F);
            OS_EXIT_CRITICAL();
            usleep(50000);              // 50 msec
        }
        else
        {
            usleep(500000);             // 500 msec
        }

        //---------------------------------------------------------------------
        // tickle watchdog
        // occurs at the end of the loop to ensure all code has an opportunity to run
        MarkStatWdog_TB .Tickle();
    }

    //---------------------------------------------------------------------
    // should never get to here
    return NULL;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Sequencer Client thread - "App" subframe
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void* T3SeqClientApp(void *vp)
{
    cSeqSequencerClient SeqClient;
    sigevent            OverrunSigEvent;
    uint32_t            FramePeriod = gSysMemConfigClient.GetFramePeriod();
    int                 Status;
    bool                SeqClientErr = false;

    //-------------------------------------------------------------------------
    // register for wakeup call at the start of the MarkVIe APP sub-frame
    //  define as a non-critical client because not necessary for it to finish within the App sub-frame
    SIGEV_SIGNAL_INIT(&OverrunSigEvent, SIGUSR1);
    Status = SeqClient.Register("App", "App", "T3SeqClientApp", &OverrunSigEvent, false, true, true, false);
    if ( Status == -1 )
    {
        SeqClientErr = true;
        MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_XOPS_STR,
                                     cParamString("T3SeqClient Register Fail (%d,%s)",errno,strerror(errno)).c_str()),
                                     HERE);
    }

    //-------------------------------------------------------------------------
    // loop forever
    while (1)
    {
        // take a nap using sequencer to wake-up
        if ( !SeqClientErr && !gStateMachineClient.IsShutdown() )
        {
            // wakeup from sequencer
            Status = SeqClient.Arm();
            if ( (Status == -1) && !gStateMachineClient.IsShutdown() )
            {
                SeqClientErr = true;
                MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_XOPS_STR,
                                             cParamString("T3SeqClient Arm Fail (%d,%s)",errno,strerror(errno)).c_str()),
                                             HERE);
            }
            else
            {
                // adjust task3 priority above MarkVIe APP threads only if frame period > 10 msec
                //  for 10 msec frame period MarkVIe APP threads have less time to complete than task3
                //  so leave task3 set to low prioirty
                if ( FramePeriod > 10000 )
                {
                    pthread_setschedprio(TidT3, PRIORITY_PWRCNV_T3_HI);
                }
                else
                {
                    pthread_setschedprio(TidT3, PRIORITY_PWRCNV_T3_LO);
                }
            }
        }
    }

    //---------------------------------------------------------------------
    // should never get to here
    return NULL;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Sequencer Client thread - "OutputXfer" subframe
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void* T3SeqClientOut(void *vp)
{
    cSeqSequencerClient SeqClient;
    sigevent            OverrunSigEvent;
    int                 Status;
    bool                SeqClientErr = false;

    //-------------------------------------------------------------------------
    // register for wakeup call at the start of the MarkVIe OUTPUT_XFER sub-frame
    //  define as a non-critical client because not necessary for it to finish within the App sub-frame
    SIGEV_SIGNAL_INIT(&OverrunSigEvent, SIGUSR1);
    Status = SeqClient.Register("OutputXfer", "OutputXfer", "T3SeqClientApp", &OverrunSigEvent, false, true, true, true);
    if ( Status == -1 )
    {
        SeqClientErr = true;
        MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_XOPS_STR,
                                     cParamString("T3SeqClient Register Fail (%d,%s)",errno,strerror(errno)).c_str()),
                                     HERE);
    }

    //-------------------------------------------------------------------------
    // loop forever
    while (1)
    {
        // take a nap using sequencer to wake-up
        if ( !SeqClientErr && !gStateMachineClient.IsShutdown() )
        {
            // wakeup from sequencer
            Status = SeqClient.Arm();
            if ( (Status == -1) && !gStateMachineClient.IsShutdown() )
            {
                SeqClientErr = true;
                MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_XOPS_STR,
                                             cParamString("T3SeqClient Arm Fail (%d,%s)",errno,strerror(errno)).c_str()),
                                             HERE);
            }
            else
            {
                // adjust task3 priority below MarkVIe InputXfer and OutputXfer threads
                pthread_setschedprio(TidT3, PRIORITY_PWRCNV_T3_LO);
            }
        }
    }

    //---------------------------------------------------------------------
    // should never get to here
    return NULL;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Surrogate funcion for conversion of logical address to absolute address
//
// EXECUTION LEVEL
//      n/a
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void* GetVarAddress(unsigned Laddr)
{
    return gVariable.GetAddress(Laddr);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This method is the input thread which services incoming messages to MarkStat.
//      This thread handles commands and not unsolicited data transfers, from other processes.
//      ADL, SDI, Diag Reset
//
// EXECUTION LEVEL
//      Background (untimed)
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void*  MessageServer (void* arg)
{
    char                pBuffer[__max(__max(__max(sizeof(cCommandMsgHeader),sizeof(_pulse)),sizeof(cSdiQnxHdr)),sizeof(_io_msg))];
    cCommandMsgHeader   *pMsgHeader = (cCommandMsgHeader *) pBuffer;
    _pulse              *pPulse = (_pulse *) pBuffer;
    _io_msg             *pIoMsgHdr = (_io_msg*) pBuffer;
    int                 ChannelId;
    int                 ReceiveId;

    if ((ChannelId = ChannelCreate(0)) == -1)
    {
        MarkStatDiag.LogErrorMessage(ALARM_MSG, cParamString(MKS_MSG_SVR_STR,
            cParamString("Unable to open channel for Diagnostic Reset notification  (%d,%s)", errno, strerror(errno)).c_str()),
            HERE);
        return NULL;
    }

    // register for diagnostic notification
    MarkStatDiag.RegisterResetNotify(ChannelId, getpid());

    // register for ADL messages
    InitAdlServer(ChannelId);

    // register for SDI messages
    if (SdiMsg.Open(ChannelId, gSysMemConfigClient.GetControllerId()) == -1)
    {
        MarkStatDiag.LogErrorMessage(ALARM_MSG, cParamString(MKS_MSG_SVR_STR,
            cParamString("Unable to attach to SDI server, continuing anyway").c_str()),
            HERE);
    }

    while (!gStateMachineClient.IsShutdown())
    {
        memset(pBuffer, 0, sizeof(pBuffer));
        ReceiveId = MsgReceive(ChannelId, pBuffer, sizeof(pBuffer), NULL);

        // receive error
        if (ReceiveId == -1)
        {
            delay(1);
            continue;
        }
        // pulse message received
        else if (ReceiveId == 0)
        {
            switch (pPulse->value.sival_int)
            {
                // diagnostic alarm queue has been reset
                case DEVCTL_RESET_ALM:
                    MarkStatDiag.DiagWasReset();
                    break;
                default:
                    MarkStatDiag.LogErrorMessage(ALARM_MSG, cParamString(MKS_MSG_SVR_STR,
                        cParamString("pulse code %d,  value %d  NOT handled", pPulse->code, pPulse->value.sival_int).c_str()),
                        HERE);
                    break;
            }
        }
        // message received
        else
        {
            switch(pMsgHeader->mType)
            {
                case _IO_MSG:
                    adlReceiveDirect(ReceiveId);
                    break;
                case SDI_MESSAGE:
                    if (SdiMsg.MessageHandler(ReceiveId) == -1)
                    {
                        MarkStatDiag.LogEventMessage( 1, DIAG_SYSTEM, cParamString(MKS_MSG_SVR_STR,
                            cParamString("SDI Message Handler returned -1\n").c_str()), HERE );
                    }
                    break;
                default:
                    MarkStatDiag.LogErrorMessage(ALARM_MSG, cParamString(MKS_MSG_SVR_STR,
                        cParamString("message %d,  command %d,  subtype %d  NOT handled", pMsgHeader->mType, pMsgHeader->mCmdMsgType, pIoMsgHdr->subtype).c_str()), HERE);
                    break;
            }
        }
    }

    return EOK;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
int xopsInit(void)
{

    int                 Status = SUCCESS;

    pthread_attr_t      attr;
    pthread_t           tid;
    sched_param         param;
    TaskDataTyp*        tdp;


#if 0
    _clockperiod        ClockPeriodOld;
    _clockperiod        ClockPeriodNew;
    ClockPeriodNew.nsec  = 10000000;    // 10 msec
    ClockPeriodNew.fract = 0;
    ClockPeriod(CLOCK_REALTIME, &ClockPeriodNew, &ClockPeriodOld, 0);
    printf("***********************************/n");
    printf("Old clock period = %f (msec)\n", ClockPeriodOld.nsec * 1e-6);
    printf("New clock period = %f (msec)\n", ClockPeriodNew.nsec * 1e-6);
    printf("***********************************/n");
#endif

    //--------------------------------------------------------------------------------------------------------------------------
    // reset global min/max for execution statitistics
    for ( unsigned meas = 0 ; meas < ARRAY_LEN(XmsGlobInfo) ; ++meas )
    {
        XmsGlobInfo[meas].StartDelClk.Max = 0;
        XmsGlobInfo[meas].StartDelClk.Min = 0x7FFFFFFF;
        XmsGlobInfo[meas].StartDelClk.Avg = 0.0F;

        XmsGlobInfo[meas].EndDelClk.Max = 0;
        XmsGlobInfo[meas].EndDelClk.Min = 0x7FFFFFFF;
        XmsGlobInfo[meas].EndDelClk.Avg = 0.0F;

        XmsGlobInfo[meas].DeltaClk.Max = 0;
        XmsGlobInfo[meas].DeltaClk.Min = 0x7FFFFFFF;
        XmsGlobInfo[meas].DeltaClk.Avg = 0.0F;
    }

    // zero the minimums for Task1S if not configured to execute
    if ( !SimMode )
    {
        XmsGlobInfo[ XMS_TSK1S ].StartDelClk.Min = 0;
        XmsGlobInfo[ XMS_TSK1S ].EndDelClk.Min = 0;
        XmsGlobInfo[ XMS_TSK1S ].DeltaClk.Min = 0;
    }

    // zero the minimums for Task1A if not configured to execute
    if ( PARM(T1AExecMask) == 0x0 )
    {
        XmsGlobInfo[ XMS_TSK1A ].StartDelClk.Min = 0;
        XmsGlobInfo[ XMS_TSK1A ].EndDelClk.Min = 0;
        XmsGlobInfo[ XMS_TSK1A ].DeltaClk.Min = 0;
    }

    for ( unsigned pid = 0 ; pid < ARRAY_LEN(XmsProbe) ; ++pid )
    {
        XmsProbe[pid].ClkDeltaMin = (unsigned)(999.99e-6F * CpuCps);
        XmsProbe[pid].ClkDeltaMax = 0;
        XmsProbe[pid].ClkDeltaAvg = 0.0F;
        XmsProbe[pid].pClkProbe   = NULL;

        XmsProbe[pid].Pm0DeltaMin = 99999;
        XmsProbe[pid].Pm0DeltaMax = 0;
        XmsProbe[pid].Pm0DeltaAvg = 0.0F;
        XmsProbe[pid].pPm0Probe   = NULL;

        XmsProbe[pid].Pm1DeltaMin = 99999;
        XmsProbe[pid].Pm1DeltaMax = 0;
        XmsProbe[pid].Pm1DeltaAvg = 0.0F;
        XmsProbe[pid].pPm1Probe   = NULL;

        XmsProbe[pid].IsrEnbCtr   = 0;
    }

    XmsProbe[XMS_PID_0].pClkProbe = &XmsClkProbe0;
    XmsProbe[XMS_PID_1].pClkProbe = &XmsClkProbe1;
    XmsProbe[XMS_PID_2].pClkProbe = &XmsClkProbe2;
    XmsProbe[XMS_PID_3].pClkProbe = &XmsClkProbe3;
    XmsProbe[XMS_PID_4].pClkProbe = &XmsClkProbe4;
    XmsProbe[XMS_PID_5].pClkProbe = &XmsClkProbe5;
    XmsProbe[XMS_PID_6].pClkProbe = &XmsClkProbe6;
    XmsProbe[XMS_PID_7].pClkProbe = &XmsClkProbe7;
    XmsProbe[XMS_PID_8].pClkProbe = &XmsClkProbe8;
    XmsProbe[XMS_PID_9].pClkProbe = &XmsClkProbe9;

    XmsProbe[XMS_PID_0].pPm0Probe = &XmsPm0Probe0;
    XmsProbe[XMS_PID_1].pPm0Probe = &XmsPm0Probe1;
    XmsProbe[XMS_PID_2].pPm0Probe = &XmsPm0Probe2;
    XmsProbe[XMS_PID_3].pPm0Probe = &XmsPm0Probe3;
    XmsProbe[XMS_PID_4].pPm0Probe = &XmsPm0Probe4;
    XmsProbe[XMS_PID_5].pPm0Probe = &XmsPm0Probe5;
    XmsProbe[XMS_PID_6].pPm0Probe = &XmsPm0Probe6;
    XmsProbe[XMS_PID_7].pPm0Probe = &XmsPm0Probe7;
    XmsProbe[XMS_PID_8].pPm0Probe = &XmsPm0Probe8;
    XmsProbe[XMS_PID_9].pPm0Probe = &XmsPm0Probe9;

    XmsProbe[XMS_PID_0].pPm1Probe = &XmsPm1Probe0;
    XmsProbe[XMS_PID_1].pPm1Probe = &XmsPm1Probe1;
    XmsProbe[XMS_PID_2].pPm1Probe = &XmsPm1Probe2;
    XmsProbe[XMS_PID_3].pPm1Probe = &XmsPm1Probe3;
    XmsProbe[XMS_PID_4].pPm1Probe = &XmsPm1Probe4;
    XmsProbe[XMS_PID_5].pPm1Probe = &XmsPm1Probe5;
    XmsProbe[XMS_PID_6].pPm1Probe = &XmsPm1Probe6;
    XmsProbe[XMS_PID_7].pPm1Probe = &XmsPm1Probe7;
    XmsProbe[XMS_PID_8].pPm1Probe = &XmsPm1Probe8;
    XmsProbe[XMS_PID_9].pPm1Probe = &XmsPm1Probe9;

    XmsFilSettleGuage = 0.0F;
    XmsMinMaxAccumTm  = 0.0F;

    //--------------------------------------------------------------------------------------------------------------------------
    // finish initialization of control structure for each task
    // note: TaskNo, Priority, (*task)(), (*taskOverrun)(), pFrames, pState were initialized when memory for the struture was allocated
    for ( tdp = TaskData ; tdp < TaskData+TDP_N_TASKS ; ++tdp )
    {
        sem_init( &tdp->Sem, 0, 0 );                            // initialize the Task semaphores
    }

    //##########################################################################################################################
    // IPC likes to create it's threads early
    // followed be the Message Server thread

    //--------------------------------------------------------------------------------------------------------------------------
    // initialize Local Process to Controller communication
    if (IpcInterfaceEnable)
    {
        gIpcDevice.RegGetAddrFunc(GetVarAddress);  // register GetAddress function
        IpcInit();
    }

    //--------------------------------------------------------------------------------------------------------------------------
    // create the Message Server thread
    pthread_create( &tid, NULL, MessageServer, NULL );
    gThreadInfoList.Insert(cThreadInfo(tid, MARKSTAT_MSGSVR_THREAD));
    MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
        cParamString("MessageServer thread created (thread id = %d)", tid ).c_str()),
        HERE );

    //##########################################################################################################################


    //--------------------------------------------------------------------------------------------------------------------------
    // create the flash file writer thread
    pthread_create( &tid, NULL, FlashFileSvr, NULL );
    gThreadInfoList.Insert(cThreadInfo(tid, MARKSTAT_FLASH_THREAD));
    MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
        cParamString("FlashFileSrvr thread created (thread id = %d)", tid ).c_str()),
        HERE );


    //--------------------------------------------------------------------------------------------------------------------------
    // create the Background thread
    pthread_create( &tid, NULL, xopsBkgnd,   NULL );
    gThreadInfoList.Insert(cThreadInfo(tid, MARKSTAT_XOPS_BKGND_THREAD));
    MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
        cParamString("xopsBkgnd thread created (thread id = %d)", tid ).c_str()),
        HERE );


    //--------------------------------------------------------------------------------------------------------------------------
    // create the Xops Task threads
    pthread_attr_init            (&attr                          );
    pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED  );
    pthread_attr_setdetachstate  (&attr, PTHREAD_CREATE_DETACHED );
    pthread_attr_setschedpolicy  (&attr, SCHED_FIFO              );

    for ( tdp = TaskData ; tdp < TaskData+TDP_N_TASKS ; ++tdp )
    {
        pthread_attr_getschedparam(&attr, &param);
        param.sched_priority = tdp->Priority;
        pthread_attr_setschedparam(&attr, &param);

        if ( tdp == &TaskData[TDP_SCHED] )
        {
            pthread_create(&tid, &attr, xopsSched, NULL);
            gThreadInfoList.Insert(cThreadInfo(tid, MARKSTAT_XOPS_SCHED_THREAD));
            MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                cParamString("xopsSched thread created (thread id = %d)", tid ).c_str()),
                HERE );
        }

        if ( tdp == &TaskData[TDP_TASK1] )
        {
            pthread_create(&tid, &attr, xopsTask1, tdp);
            gThreadInfoList.Insert(cThreadInfo(tid, MARKSTAT_XOPS_TASK1_THREAD));
            MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                cParamString("xopsTask1 thread created (thread id = %d)", tid ).c_str()),
                HERE );
        }

        if ( tdp == &TaskData[TDP_TASK2] )
        {
            pthread_create(&tid, &attr, xopsTask2, tdp);
            gThreadInfoList.Insert(cThreadInfo(tid, MARKSTAT_XOPS_TASK2_THREAD));
            MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                cParamString("xopsTask2 thread created (thread id = %d)", tid ).c_str()),
                HERE );
        }

        if ( tdp == &TaskData[TDP_TASK3] )
        {
            pthread_create(&tid, &attr, xopsTask3, tdp);
            gThreadInfoList.Insert(cThreadInfo(tid, MARKSTAT_XOPS_TASK3_THREAD));
            MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                cParamString("xopsTask3 thread created (thread id = %d)", tid ).c_str()),
                HERE );
            TidT3 = tid;

            // set task3 priority based on MarkVIe frame period
            {
                // get the MarkVIe frame period (usec)
                uint32_t FramePeriod = gSysMemConfigClient.GetFramePeriod();
                printf("xopsInit:  MarkVIe FramePeriod = %d\n", FramePeriod);

                // 10ms MarkVIe frame
                if ( FramePeriod == 10000 )
                {
                    // set task3 priority below MarkVIe InputXfer, App and OutputXfer threads
                    pthread_setschedprio(TidT3, PRIORITY_PWRCNV_T3_LO);
                }

                // 20ms, 40ms MarkVIe frame period
                else if ( (FramePeriod == 20000) || (FramePeriod == 40000) )
                {
                    // create thread to dynamically adjust task3 priority for MarkVIe InputXfer and OutputXfer subframes
                    //  (typically set task3 prioiry below MarkVIe input/output threads)
                    pthread_create( &tid, &attr, T3SeqClientApp,   NULL );
                    gThreadInfoList.Insert(cThreadInfo(tid, MARKSTAT_SEQ_CLIENT_APP_THREAD));
                    MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                                                  cParamString("T3SeqClientApp thread created (thread id = %d)", tid ).c_str()),
                                                  HERE );

                    // create thread to dynamically adjust task3 priority for MarkVIe App subframes
                    //  (typically set task3 priority above MarkVIe high prioirty blockware threads)
                    pthread_create( &tid, &attr, T3SeqClientOut,   NULL );
                    gThreadInfoList.Insert(cThreadInfo(tid, MARKSTAT_SEQ_CLIENT_OUT_THREAD));
                    MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                                                  cParamString("T3SeqClientOut thread created (thread id = %d)", tid ).c_str()),
                                                  HERE );
                }

                // 80ms, 160ms MarkVIe frame period
                else if ( (FramePeriod == 80000) || (FramePeriod == 160000) )
                {
                    // set task3 priority above MarkVIe InputXfer, App and OutputXfer threads
                    pthread_setschedprio(TidT3, PRIORITY_PWRCNV_T3_HI);
                }

                // unsupported MarkVIe frame period
                else
                {
                    MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_XOPS_STR,
                                                 cParamString("Unsupported MarkVIe frame period").c_str()),
                                                 HERE);
                }
            }
        }
    }

    //--------------------------------------------------------------------------------------------------------------------------
    // initialize watchdog
    if ( !XSimMode )
    {
        MarkStatAlphaWdog.Initialize(gMonitor, DelTm3Wdog);
    }

    //--------------------------------------------------------------------------------------------------------------------------
    // initialize capture buffers in general
    cCaptureBufBase::Initialize(gMonitor);

    //--------------------------------------------------------------------------------------------------------------------------
    // start capture buffers
    CaptureBuf.Enable();
    TripBuf.Enable() ;


    //--------------------------------------------------------------------------------------------------------------------------
    // our work here is done
    return Status;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      initialize the task execution control structure for DLL operation
//
// EXECUTION LEVEL
//      DLL Initialization
//
// RETURN VALUE
//      Status - SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
#ifdef MARKSTAT_DLL
int xopsInitForDLL(void)
{
    int                 Status = SUCCESS;
    TaskDataTyp*        tdp;

    for ( tdp = TaskData ; tdp < TaskData+TDP_N_TASKS ; ++tdp )
    {
        if ( tdp == &TaskData[TDP_SCHED] )
        {
            tdp->Sem.pTask     = xopsSched;
            tdp->Sem.pTaskData = tdp;
        }

        if ( tdp == &TaskData[TDP_TASK1] )
        {
            tdp->Sem.pTask     = xopsTask1;
            tdp->Sem.pTaskData = tdp;
        }

        if ( tdp == &TaskData[TDP_TASK2] )
        {
            tdp->Sem.pTask     = xopsTask2;
            tdp->Sem.pTaskData = tdp;
        }

        if ( tdp == &TaskData[TDP_TASK3] )
        {
            tdp->Sem.pTask     = xopsTask3;
            tdp->Sem.pTaskData = tdp;
        }
    }

    return Status;
}
#endif


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Setup a exception handlers
//
// EXECUTION LEVEL
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// handler for segmentation violation
void seg_handler( int signo, siginfo_t* info, void* other )
{
    // print context information about the offending instruction
    printf("\n");
    printf("seg_handler: signo = %d, code = %d, errno = %d\n", signo,          info->si_code, info->si_errno);
    printf("             fltno = %d, addr = %p, fltip = %p\n", info->si_fltno, info->si_addr, info->si_fltip);

    // restore context to the original qnx handler, the return from this handler will
    // be back to the offending instruction which will execute again, this time calling
    // the original qnx handler which will call "dumper" to generate a core dump and reboot
    sigaction( SIGSEGV, &seg_handler_oact, NULL );
}

//-----------------------------------------------------------------------------
// handler for floating point exception
void fpe_handler( int signo, siginfo_t* info, void* other )
{
    // capture the information for llm display
    ++Fpe_Ctr;
    Fpe_Signo = signo;
    Fpe_Info  = *info;

    // it appears that returning from an fp exception does not return to offending
    // instruction but the one after, so it seems best to not restore the context
    // to the original qnx handler (so leave the following line commented out)
    //sigaction( SIGFPE, &fpe_handler_oact, NULL );
}

//-----------------------------------------------------------------------------
// setup the exception handlers
void handler_setup(void)
{
    struct sigaction    act;
    sigset_t            set;

    // segment violation
    sigemptyset( &set );
    sigaddset( &set, SIGSEGV );
    act.sa_flags = 0;
    act.sa_mask = set;
    act.sa_sigaction = &seg_handler;
    sigaction( SIGSEGV, &act, &seg_handler_oact );  // save original handler context to oact

    // floating point exception
    sigemptyset( &set );
    sigaddset( &set, SIGFPE );
    act.sa_flags = 0;
    act.sa_mask = set;
    act.sa_sigaction = &fpe_handler;
    sigaction( SIGFPE, &act, &fpe_handler_oact );

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This is the Xops Task overrun exception handler.
//
// EXECUTION LEVEL
//      exception handler
//
// RETURN VALUE
//      one of XOPS_CONT, XOPS_KILL, XOPS_SYNC
//
///////////////////////////////////////////////////////////////////////////////
void xopsTaskOverRun(TaskDataTyp* tdp)
{
    ++(tdp->OverRunCtr);
    if ( tdp->OverRunLstpFlg )
    {
        ++(tdp->OverRunLstpCtr);
    }

#if 0
    // TriggerVmetro(1+tdp->TaskNo);    // intend a function to read/write specific pci address & setup vmetro to trigger on that
    TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_TASK_OVERRUN+(tdp->TaskNo), "TASK OVERRUN TRIGGER" );
#endif

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calls appropriate warning fault when the appropriate task overrun
//      has been detected.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void xopsTaskExecFltDetect(void)
{
    if (InitDone)
    {
        if ( IsrErrCtr != IsrErrCtrT3Z )
            PUSH_DIAG(XopsIsrErrTrp);

        if ( TaskData[TDP_TASK1].OverRunLstpCtr != OverRunLstpCtrT1 )
            PUSH_DIAG(Tsk1OverRunTrp);

        if ( TaskData[TDP_TASK2].OverRunLstpCtr != OverRunLstpCtrT2 )
            PUSH_DIAG(Tsk2OverRunTrp);

        if ( TaskData[TDP_TASK3].OverRunLstpCtr != OverRunLstpCtrT3 )
            PUSH_DIAG(Tsk3OverRunTrp);
    }

    OverRunLstpCtrT1 = TaskData[TDP_TASK1].OverRunLstpCtr;
    OverRunLstpCtrT2 = TaskData[TDP_TASK2].OverRunLstpCtr;
    OverRunLstpCtrT3 = TaskData[TDP_TASK3].OverRunLstpCtr;

    IsrErrCtrT3Z   = IsrErrCtr;
    OverRunCtrT1 = TaskData[TDP_TASK1].OverRunCtr;
    OverRunCtrT2 = TaskData[TDP_TASK2].OverRunCtr;
    OverRunCtrT3 = TaskData[TDP_TASK3].OverRunCtr;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Low level monitor support the XOPS function.
//
// EXECUTION LEVEL
//      low level monitor thread
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
int xopsLlm(cParamString &OutputBuffer, cParamString &CommandLineArgs)
{
    int     CurrentOffset;

    //-------------------------------------------------------------------------
    // reset task timing and fpe information
    CurrentOffset = CommandLineArgs.find("-r");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("Reset Execution Monitoring System Statistics\n");
        //--------------------------------------------------------------------------------------------------------------------------
        // reset global min/max for execution statitistics
        for ( unsigned meas = 0 ; meas < ARRAY_LEN(XmsGlobInfo) ; ++meas )
        {
            XmsGlobInfo[meas].StartDelClk.Max = 0;
            XmsGlobInfo[meas].StartDelClk.Min = 0x7FFFFFFF;
            XmsGlobInfo[meas].StartDelClk.Avg = 0.0;

            XmsGlobInfo[meas].EndDelClk.Max = 0;
            XmsGlobInfo[meas].EndDelClk.Min = 0x7FFFFFFF;
            XmsGlobInfo[meas].EndDelClk.Avg = 0.0;

            XmsGlobInfo[meas].DeltaClk.Max = 0;
            XmsGlobInfo[meas].DeltaClk.Min = 0x7FFFFFFF;
            XmsGlobInfo[meas].DeltaClk.Avg = 0.0;
        }

        // zero the minimums for Task1S if not configured to execute
        if ( !SimMode )
        {
            XmsGlobInfo[ XMS_TSK1S ].StartDelClk.Min = 0;
            XmsGlobInfo[ XMS_TSK1S ].EndDelClk.Min = 0;
            XmsGlobInfo[ XMS_TSK1S ].DeltaClk.Min = 0;
        }

        // zero the minimums for Task1A if not configured to execute
        if ( PARM(T1AExecMask) == 0x0 )
        {
            XmsGlobInfo[ XMS_TSK1A ].StartDelClk.Min = 0;
            XmsGlobInfo[ XMS_TSK1A ].EndDelClk.Min = 0;
            XmsGlobInfo[ XMS_TSK1A ].DeltaClk.Min = 0;
        }

        for ( unsigned pid = 0 ; pid < ARRAY_LEN(XmsProbe) ; ++pid )
        {
            XmsProbe[pid].ClkStart    = 0;
            XmsProbe[pid].ClkEnd      = 0;
            XmsProbe[pid].ClkDelta    = 0;
            XmsProbe[pid].ClkDeltaMax = 0;
            XmsProbe[pid].ClkDeltaMin = (unsigned)(999.99e-6F * CpuCps);
            XmsProbe[pid].ClkDeltaAvg = 0.0F;

            XmsProbe[pid].Pm0Start    = 0;
            XmsProbe[pid].Pm0End      = 0;
            XmsProbe[pid].Pm0Delta    = 0;
            XmsProbe[pid].Pm0DeltaMax = 0;
            XmsProbe[pid].Pm0DeltaMin = 99999;
            XmsProbe[pid].Pm0DeltaAvg = 0.0F;

            XmsProbe[pid].Pm1Start    = 0;
            XmsProbe[pid].Pm1End      = 0;
            XmsProbe[pid].Pm1Delta    = 0;
            XmsProbe[pid].Pm1DeltaMax = 0;
            XmsProbe[pid].Pm1DeltaMin = 99999;
            XmsProbe[pid].Pm1DeltaAvg = 0.0F;

            XmsProbe[pid].IsrEnbCtr   = 0;
        }
        XmsFilSettleGuage = 0.0F;
        XmsMinMaxAccumTm = 0.0F;

        Fpe_Ctr   = 0;
        Fpe_Signo = 0;
        memset( (void*)&Fpe_Info, 0, sizeof(Fpe_Info) );

#ifdef XOPS_PERF_MON_TASK
        for ( int Sidx = 0; Sidx < (int)(sizeof(PM_Task)/sizeof(PM_Task[0])); ++Sidx)
        {
            PM_Task[Sidx].ExecTime.Min.Cycles = PM_Task[Sidx].ExecTime.Max.Cycles;
            PM_Task[Sidx].ExecTime.Max.Cycles = 0;
            PM_Task[Sidx].CPI.Min.CPI         = PM_Task[Sidx].CPI.Max.CPI;
            PM_Task[Sidx].CPI.Max.CPI         = 0;
        }
        PM_TrigTask = -1;

        PM_Probe.Cycle.Min  = PM_Probe.Cycle.Max;
        PM_Probe.Cycle.Max  = 0;
        PM_Probe.Cntr[0].Min = PM_Probe.Cntr[0].Max;
        PM_Probe.Cntr[0].Max = 0;
        PM_Probe.Cntr[1].Min = PM_Probe.Cntr[1].Max;
        PM_Probe.Cntr[1].Max = 0;
#endif

        OutputBuffer.pappend("  timing and fp exception information reset complete\n" );
        OutputBuffer.pappend("\n");

        return 1;
    }

    //-------------------------------------------------------------------------
    // display task data structure layoutfloating point information
    CurrentOffset = CommandLineArgs.find("-lay");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        TaskDataTyp     TaskDataLlm={0};
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("Layout (offsetof/sizeof) of TaskDataTyp structure:\n" );
        OutputBuffer.pappend("          TaskNo = 0x%04x / %d\n", offsetof(TaskDataTyp, TaskNo)         ,sizeof(TaskDataLlm.TaskNo)          );
        OutputBuffer.pappend("        Priority = 0x%04x / %d\n", offsetof(TaskDataTyp, Priority)       ,sizeof(TaskDataLlm.Priority)        );
        OutputBuffer.pappend("         pFrames = 0x%04x / %d\n", offsetof(TaskDataTyp, pFrames)        ,sizeof(TaskDataLlm.pFrames)         );
        OutputBuffer.pappend("          pSysTm = 0x%04x / %d\n", offsetof(TaskDataTyp, pSysTm)         ,sizeof(TaskDataLlm.pSysTm)          );
        OutputBuffer.pappend("          pEvtTm = 0x%04x / %d\n", offsetof(TaskDataTyp, pEvtTm)         ,sizeof(TaskDataLlm.pEvtTm)          );
        OutputBuffer.pappend("           State = 0x%04x / %d\n", offsetof(TaskDataTyp, State)          ,sizeof(TaskDataLlm.State)           );
        OutputBuffer.pappend("         BusyCtr = 0x%04x / %d\n", offsetof(TaskDataTyp, BusyCtr)        ,sizeof(TaskDataLlm.BusyCtr)         );
        OutputBuffer.pappend("        DoneOnce = 0x%04x / %d\n", offsetof(TaskDataTyp, DoneOnce)       ,sizeof(TaskDataLlm.DoneOnce)        );
        OutputBuffer.pappend("      OverRunCtr = 0x%04x / %d\n", offsetof(TaskDataTyp, OverRunCtr)     ,sizeof(TaskDataLlm.OverRunCtr)      );
        OutputBuffer.pappend("  OverRunLstpCtr = 0x%04x / %d\n", offsetof(TaskDataTyp, OverRunLstpCtr) ,sizeof(TaskDataLlm.OverRunLstpCtr)  );
        OutputBuffer.pappend("  OverRunLstpFlg = 0x%04x / %d\n", offsetof(TaskDataTyp, OverRunLstpFlg) ,sizeof(TaskDataLlm.OverRunLstpFlg)  );
        OutputBuffer.pappend("             Sem = 0x%04x / %d\n", offsetof(TaskDataTyp, Sem)            ,sizeof(TaskDataLlm.Sem)             );
        OutputBuffer.pappend("\n");

        return 1;
    }

#ifdef XOPS_FPE
    //-------------------------------------------------------------------------
    // display floating point information
    CurrentOffset = CommandLineArgs.find("-fpc");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("Floating point configuration:\n" );
        OutputBuffer.pappend("  fp precision = %d\n", fp_precision(-1) );
        OutputBuffer.pappend("  fp rounding  = %d\n", fp_rounding(-1) );
        OutputBuffer.pappend("\n");

        return 1;
    }

    //-------------------------------------------------------------------------
    // enable floating point exceptions
    CurrentOffset = CommandLineArgs.find("-fpe");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("Enable floating point exceptions\n" );
        // enable fpe exception definitions from <fpstatus.h>
        Fpe_Msk = 0;
      //Fpe_Msk |= _FP_EXC_INEXACT;     // = 0x01
      //Fpe_Msk |= _FP_EXC_DIVZERO;     // = 0x02
      //Fpe_Msk |= _FP_EXC_UNDERFLOW;   // = 0x04
      //Fpe_Msk |= _FP_EXC_OVERFLOW;    // = 0x08
      //Fpe_Msk |= _FP_EXC_INVALID;     // = 0x10
        Fpe_Msk |= _FP_EXC_DENORMAL;    // = 0x20
        Fpe_Enb = 1;
        Fpe_Req = true;
        OutputBuffer.pappend("  fp exception mask = 0x%08x\n\t", Fpe_Msk );
        if (Fpe_Msk & _FP_EXC_INEXACT)
            OutputBuffer.pappend("Inexact ");
        if (Fpe_Msk & _FP_EXC_DIVZERO)
            OutputBuffer.pappend("DivZero ");
        if (Fpe_Msk & _FP_EXC_UNDERFLOW)
            OutputBuffer.pappend("Underflow ");
        if (Fpe_Msk & _FP_EXC_OVERFLOW)
            OutputBuffer.pappend("Overflow ");
        if (Fpe_Msk & _FP_EXC_INVALID)
            OutputBuffer.pappend("Invalid ");
        if (Fpe_Msk & _FP_EXC_DENORMAL)
            OutputBuffer.pappend("DeNormal ");
        OutputBuffer.pappend("\n");

        return 1;
    }

    //-------------------------------------------------------------------------
    // disable floating point exceptions
    CurrentOffset = CommandLineArgs.find("-fpd");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("Disable floating point exceptions\n" );
        Fpe_Enb = 0;
        Fpe_Req = true;
        OutputBuffer.pappend("  fp exception mask = 0x%08x\n", Fpe_Msk );
        if (Fpe_Msk & _FP_EXC_INEXACT)
            OutputBuffer.pappend("Inexact ");
        if (Fpe_Msk & _FP_EXC_DIVZERO)
            OutputBuffer.pappend("DivZero ");
        if (Fpe_Msk & _FP_EXC_UNDERFLOW)
            OutputBuffer.pappend("Underflow ");
        if (Fpe_Msk & _FP_EXC_OVERFLOW)
            OutputBuffer.pappend("Overflow ");
        if (Fpe_Msk & _FP_EXC_INVALID)
            OutputBuffer.pappend("Invalid ");
        if (Fpe_Msk & _FP_EXC_DENORMAL)
            OutputBuffer.pappend("DeNormal ");
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("\n");

        return 1;
    }

    //-------------------------------------------------------------------------
    // display floating point exception occurance information
    CurrentOffset = CommandLineArgs.find("-fpi");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("Floating point exception information\n" );
        OutputBuffer.pappend("  Ctr = %d, Msk = 0x%08x, %s\n", Fpe_Ctr, Fpe_Msk, Fpe_Enb?"Enabled":"Disabled" );
        switch ( Fpe_Info.si_code )
        {
            case FPE_INTDIV:    // 1 Integer divide by zero
                OutputBuffer.pappend("  Integer divide by zero\n" );
                break;
            case FPE_INTOVF:    // 2 Integer overflow
                OutputBuffer.pappend("  Integer overflow\n" );
                break;
            case FPE_FLTDIV:    // 3 Floating point divide by zero
                OutputBuffer.pappend("  Floating point divide by zero\n" );
                break;
            case FPE_FLTOVF:    // 4 Floating point overflow
                OutputBuffer.pappend("  Floating point overflow\n" );
                break;
            case FPE_FLTUND:    // 5 Floating point underflow
                OutputBuffer.pappend("  Floating point underflow\n" );
                break;
            case FPE_FLTRES:    // 6 Floating point inexact result
                OutputBuffer.pappend("  Floating point inexact result (de-normal)\n" );
                break;
            case FPE_FLTINV:    // 7 Invalid floating point operation
                OutputBuffer.pappend("  Invalid floating point operation\n" );
                break;
            case FPE_FLTSUB:    // 8 Subscript out of range
                OutputBuffer.pappend("  Subscript out of range\n" );
                break;
            case FPE_NOFPU:     // 9 No FPU or emulator
                OutputBuffer.pappend("  No FPU or emulator\n" );
                break;
            case FPE_NOMEM:     // 10 No kernel space for FPU save area
                OutputBuffer.pappend("  No kernel space for FPU save area\n" );
                break;
            case SI_NOINFO:     // 127 no signal info
                OutputBuffer.pappend("  No SIGFPE signal information available\n" );
                break;
            default:
                OutputBuffer.pappend("  Unknown SIGFPE signal code\n" );
                break;
        }
        OutputBuffer.pappend("    signo = %d, code = %d, errno = %d\n", Fpe_Signo,         Fpe_Info.si_code, Fpe_Info.si_errno );
        OutputBuffer.pappend("    fltno = %d, addr = %p, fltip = %p\n", Fpe_Info.si_fltno, Fpe_Info.si_addr, Fpe_Info.si_fltip );
        OutputBuffer.pappend("\n");

        return 1;
    }
#endif  // XOPS_FPE


    //-------------------------------------------------------------------------
    // display task execution timing information
    CurrentOffset = CommandLineArgs.find("-ctr");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("Early/Late/Frc Counters\n");
        OutputBuffer.pappend("  Xops isr error counter = %d\n", IsrErrCtr );
        OutputBuffer.pappend("  Task1 overrun counter  = %d\n", OverRunCtrT1 );
        OutputBuffer.pappend("  Task2 overrun counter  = %d\n", OverRunCtrT2 );
        OutputBuffer.pappend("  Task3 overrun counter  = %d\n", OverRunCtrT3 );
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("  Task1 overrun lstp counter  = %d\n", OverRunLstpCtrT1 );
        OutputBuffer.pappend("  Task2 overrun lstp counter  = %d\n", OverRunLstpCtrT2 );
        OutputBuffer.pappend("  Task3 overrun lstp counter  = %d\n", OverRunLstpCtrT3 );
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("     IsrFrc = 0x%08x\n", IsrFrc );
        OutputBuffer.pappend("   FrameFrc = 0x%08x\n", FrameFrc );
        OutputBuffer.pappend("\n");
        return 1;
    }

#if XMS_DEBUG_BUF
    //-------------------------------------------------------------------------
    // display execution timing debug buffer
    CurrentOffset = CommandLineArgs.find("-tmg");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        unsigned PrintSampMax = 10;
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("Task Execution Timing Buffer (sampled at MAX Task3)\n", sizeof(XmsBuf) );
        OutputBuffer.pappend("  Sample Buffer Size = %d bytes\n", sizeof(XmsBuf)/2 );
        for ( unsigned meas = 0 ; meas < XMS_N_MEAS ; ++meas )
        {
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("  Measurement %d (%d samples)\n", meas, pXmsDebugBuf->Sidx[meas] );
            OutputBuffer.pappend("    Start      :");
            for ( unsigned i = 0 ; i < pXmsDebugBuf->Sidx[meas] ; ++i )
            {
                if ( i < PrintSampMax )   OutputBuffer.pappend("    0x%08x", pXmsDebugBuf->Data[meas][i].StartClk );
            }
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("    End        :");
            for ( unsigned i = 0 ; i < pXmsDebugBuf->Sidx[meas] ; ++i )
            {
                if ( i < PrintSampMax )   OutputBuffer.pappend("    0x%08x", pXmsDebugBuf->Data[meas][i].EndClk );
            }
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("    Delta Raw  :");
            for ( unsigned i = 0 ; i < pXmsDebugBuf->Sidx[meas] ; ++i )
            {
                if ( i < PrintSampMax )   OutputBuffer.pappend("    0x%08x", pXmsDebugBuf->Data[meas][i].EndClk - pXmsDebugBuf->Data[meas][i].StartClk );
            }
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("    Delta Adj  :");
            for ( unsigned i = 0 ; i < pXmsDebugBuf->Sidx[meas] ; ++i )
            {
                if ( i < PrintSampMax )   OutputBuffer.pappend("    0x%08x", pXmsDebugBuf->Data[meas][i].DeltaClk );
            }
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("    Del Tm Raw :");
            for ( unsigned i = 0 ; i < pXmsDebugBuf->Sidx[meas] ; ++i )
            {
                if ( i < PrintSampMax )   OutputBuffer.pappend("         %5.1f", (pXmsDebugBuf->Data[meas][i].EndClk - pXmsDebugBuf->Data[meas][i].StartClk) * CpuCpsRcpUsec);
            }
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("    Del Tm Adj :");
            for ( unsigned i = 0 ; i < pXmsDebugBuf->Sidx[meas] ; ++i )
            {
                if ( i < PrintSampMax )   OutputBuffer.pappend("         %5.1f", pXmsDebugBuf->Data[meas][i].DeltaClk * CpuCpsRcpUsec);
            }
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("    Num Adj    :");
            for ( unsigned i = 0 ; i < pXmsDebugBuf->Sidx[meas] ; ++i )
            {
                if ( i < PrintSampMax )   OutputBuffer.pappend("             %d", pXmsDebugBuf->Data[meas][i].ClkNumAdj );
            }
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("    Max        :  %5.1f  usec\n", XmsGlobStat[meas].ClkMax * CpuCpsRcpUsec );
            OutputBuffer.pappend("    Avg        :  %5.1f  usec\n", XmsGlobStat[meas].ClkAvg * CpuCpsRcpUsec );
            OutputBuffer.pappend("    Min        :  %5.1f  usec\n", XmsGlobStat[meas].ClkMin * CpuCpsRcpUsec );

            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("    Frame Start:");
            for ( unsigned i = 0 ; i < pXmsDebugBuf->Sidx[meas] ; ++i )
            {
                if ( i < PrintSampMax )   OutputBuffer.pappend("    0x%08x", pXmsDebugBuf->Data[meas][i].FrcStart );
            }
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("    End        :");
            for ( unsigned i = 0 ; i < pXmsDebugBuf->Sidx[meas] ; ++i )
            {
                if ( i < PrintSampMax )   OutputBuffer.pappend("    0x%08x", pXmsDebugBuf->Data[meas][i].FrcEnd );
            }
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("    Delta Raw  :");
            for ( unsigned i = 0 ; i < pXmsDebugBuf->Sidx[meas] ; ++i )
            {
                if ( i < PrintSampMax )   OutputBuffer.pappend("    0x%08x", pXmsDebugBuf->Data[meas][i].FrcEnd - pXmsDebugBuf->Data[meas][i].FrcStart );
            }
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("    Max        :  %5.1f  usec\n", XmsGlobStat[meas].FrameMax / FrcFreq * 1e6 );
            OutputBuffer.pappend("    Avg        :  %5.1f  usec\n", XmsGlobStat[meas].FrameAvg / FrcFreq * 1e6 );
            OutputBuffer.pappend("    Min        :  %5.1f  usec\n", XmsGlobStat[meas].FrameMin / FrcFreq * 1e6 );
        }
        OutputBuffer.pappend("\n");
        return 1;
    }
#endif

    //-------------------------------------------------------------------------
    // execution measurement system timing information
    CurrentOffset = CommandLineArgs.find("-tme");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("Enable Task Execution Timing\n" );
        if ( !XmsCalcEnb )
        {
            ++XmsCalcEnb;
        }
        return 1;
    }

    CurrentOffset = CommandLineArgs.find("-tmd");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("Disable Task Execution Timing\n" );
        XmsCalcEnb = 0;
        return 1;
    }

    CurrentOffset = CommandLineArgs.find("-tm");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        if ( !XmsCalcEnb )
        {
            OutputBuffer.pappend("Task Execution Timing Calculation has not been enabled\n" );
            OutputBuffer.pappend("  use 'proc -tme' to enable\n");
            return 1;
        }

        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("Task Execution Timing (usec)\n" );

        // gauge timings
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("  Filter Settle Gauge        %4.2f\n", XmsFilSettleGuage);
        OutputBuffer.pappend("  Min/Max Accum Time(sec) %7.0f\n", XmsMinMaxAccumTm);

        // task timings
        for ( unsigned meas = 0 ; meas < ARRAY_LEN(XmsGlobInfo) ; ++meas )
        {
            OutputBuffer.pappend("\n");
            if      ( meas == XMS_XOPS  )    OutputBuffer.pappend("  SCHED " );
            else if ( meas == XMS_TSK1  )    OutputBuffer.pappend("  TASK1 " );
            else if ( meas == XMS_TSK1A )    OutputBuffer.pappend("  TASK1A" );
            else if ( meas == XMS_TSK1S )    OutputBuffer.pappend("  TASK1S" );
            else if ( meas == XMS_TSK2  )    OutputBuffer.pappend("  TASK2 " );
            else if ( meas == XMS_TSK3  )    OutputBuffer.pappend("  TASK3 " );
            else
            {
                OutputBuffer.pappend("   unknown XMS measurement encountered\n");
                OutputBuffer.pappend("\n");
                break;
            }
            OutputBuffer.pappend("  TimStart   TimEnd    TimDelta\n");
            OutputBuffer.pappend("    Max     %6.1f   %6.1f      %6.1f\n",
                                 (int)XmsGlobInfo[meas].StartDelClk.Max * CpuCpsRcpUsec,
                                 (int)XmsGlobInfo[meas].EndDelClk.Max   * CpuCpsRcpUsec,
                                 (int)XmsGlobInfo[meas].DeltaClk.Max    * CpuCpsRcpUsec
                                 );

            OutputBuffer.pappend("    Avg     %6.1f   %6.1f      %6.1f\n",
                                 XmsGlobInfo[meas].StartDelClk.Avg * CpuCpsRcpUsec,
                                 XmsGlobInfo[meas].EndDelClk.Avg   * CpuCpsRcpUsec,
                                 XmsGlobInfo[meas].DeltaClk.Avg    * CpuCpsRcpUsec
                                 );

            OutputBuffer.pappend("    Min     %6.1f   %6.1f      %6.1f\n",
                                 (int)XmsGlobInfo[meas].StartDelClk.Min * CpuCpsRcpUsec,
                                 (int)XmsGlobInfo[meas].EndDelClk.Min   * CpuCpsRcpUsec,
                                 (int)XmsGlobInfo[meas].DeltaClk.Min    * CpuCpsRcpUsec
                                );
        }

        // probe timings
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("  XmsProbeSite = ");
        if      ( PARM(XmsProbeSite) == 0         )     OutputBuffer.pappend("Disabled" );
        else if ( PARM(XmsProbeSite) == XMS_TSK1  )     OutputBuffer.pappend("Task 1"   );
        else if ( PARM(XmsProbeSite) == XMS_TSK1A )     OutputBuffer.pappend("Task 1A"  );
        else if ( PARM(XmsProbeSite) == XMS_TSK1S )     OutputBuffer.pappend("Task 1S"  );
        else if ( PARM(XmsProbeSite) == XMS_TSK2  )     OutputBuffer.pappend("Task 2"   );
        else if ( PARM(XmsProbeSite) == XMS_TSK3  )     OutputBuffer.pappend("Task 3"   );
        else
        {
            OutputBuffer.pappend("   unknown XMS probe site encountered\n");
            OutputBuffer.pappend("\n");
            return 1;
        }
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("  Probe#  TimMin  TimMax  TimAvg     Pm0Min  Pm0Max  Pm0Avg     Pm1Min  Pm1Max  Pm1Avg    IsrEnbCtr\n");
        for ( unsigned meas = 0 ; meas < ARRAY_LEN(XmsProbe) ; ++meas )
        {
            OutputBuffer.pappend("     %d    %6.2f  %6.2f  %6.2f      %5d   %5d   %5.0f      %5d   %5d   %5.0f   %10d\n",
                                 meas,

                                 (float)XmsProbe[meas].ClkDeltaMin * CpuCpsRcpUsec,
                                 (float)XmsProbe[meas].ClkDeltaMax * CpuCpsRcpUsec,
                                        XmsProbe[meas].ClkDeltaAvg * CpuCpsRcpUsec,

                                 XmsProbe[meas].Pm0DeltaMin,
                                 XmsProbe[meas].Pm0DeltaMax,
                                 XmsProbe[meas].Pm0DeltaAvg,

                                 XmsProbe[meas].Pm1DeltaMin,
                                 XmsProbe[meas].Pm1DeltaMax,
                                 XmsProbe[meas].Pm1DeltaAvg,

                                 XmsProbe[meas].IsrEnbCtr
                                );
        }

        OutputBuffer.pappend("\n");
        return 1;
    }

    //-------------------------------------------------------------------------
    // arm tracelogger trigger
    CurrentOffset = CommandLineArgs.find("-tla");
    if(CurrentOffset != CommandLineArgs.npos)
    {
        if ( TraceLoggerArm == (unsigned)true )
        {
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("Trace Logger trigger already armed\n");
        }
        else
        {
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("Arm Trace Logger trigger\n");
            TraceLoggerArm = true;
        }

        return 1;
    }

    //-------------------------------------------------------------------------
    // trigger the tracelogger
    CurrentOffset = CommandLineArgs.find("-tlt");
    if(CurrentOffset != CommandLineArgs.npos)
    {
        if ( TraceLoggerArm == (unsigned)false )
        {
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("Trace Logger trigger is not armed\n");
        }
        else
        {
            OutputBuffer.pappend("\n");
            OutputBuffer.pappend("Manual Trigger Trace Logger\n");
            TraceLoggerArm = false;
            TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_LLM_TRIG, "XOPS LLM TRIGGER" );
        }

        return 1;
    }

#ifdef XOPS_PERF_MON_TASK
    //-------------------------------------------------------------------------
    // display performance monitor statistics
    CurrentOffset = CommandLineArgs.find("-pmdata");
    if(CurrentOffset != CommandLineArgs.npos)
    {
        int Sidx;

        OutputBuffer.pappend("Performance Monitor Statistics\n");
        OutputBuffer.pappend("   %s\n", (PIN(Brg1Running) && PIN(Brg2Running) ? "Running":"Stopped"));
        OutputBuffer.pappend("\n");

        for (Sidx = 0; Sidx < (int)(sizeof(PM_Task)/sizeof(PM_Task[0])); ++Sidx)
        {
            OutputBuffer.pappend("Task %d           Time Min  Time Max   CPI Min   CPI Max   Average\n", Sidx);
            OutputBuffer.pappend("ExecTime         %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  usec\n",
                                (float)PM_Task[Sidx].ExecTime.Min.Cycles * CpuCpsRcpUsec,
                                (float)PM_Task[Sidx].ExecTime.Max.Cycles * CpuCpsRcpUsec,
                                (float)PM_Task[Sidx].CPI.Min.Cycles * CpuCpsRcpUsec,
                                (float)PM_Task[Sidx].CPI.Max.Cycles * CpuCpsRcpUsec,
                                (float)PM_Task[Sidx].Average.Cycles * CpuCpsRcpUsec );
            OutputBuffer.pappend("Cycles           %8u  %8u  %8u  %8u  %8u\n",
                                PM_Task[Sidx].ExecTime.Min.Cycles, PM_Task[Sidx].ExecTime.Max.Cycles,
                                PM_Task[Sidx].CPI.Min.Cycles, PM_Task[Sidx].CPI.Max.Cycles, PM_Task[Sidx].Average.Cycles );
            OutputBuffer.pappend("Instructions     %8u  %8u  %8u  %8u  %8u\n",
                                PM_Task[Sidx].ExecTime.Min.Instructions, PM_Task[Sidx].ExecTime.Max.Instructions,
                                PM_Task[Sidx].CPI.Min.Instructions, PM_Task[Sidx].CPI.Max.Instructions, PM_Task[Sidx].Average.Instructions );
            OutputBuffer.pappend("CPI              %8.2f  %8.2f  %8.2f  %8.2f  %8.2f\n",
                                PM_Task[Sidx].ExecTime.Min.CPI, PM_Task[Sidx].ExecTime.Max.CPI,
                                PM_Task[Sidx].CPI.Min.CPI, PM_Task[Sidx].CPI.Max.CPI, PM_Task[Sidx].Average.CPI );
            OutputBuffer.pappend("L2 Cache Misses  %8u  %8u  %8u  %8u  %8u\n",
                                PM_Task[Sidx].ExecTime.Min.L2CacheMisses, PM_Task[Sidx].ExecTime.Max.L2CacheMisses,
                                PM_Task[Sidx].CPI.Min.L2CacheMisses, PM_Task[Sidx].CPI.Max.L2CacheMisses, PM_Task[Sidx].Average.L2CacheMisses );
            OutputBuffer.pappend("\n");
        }

        return 1;
    }

    //-------------------------------------------------------------------------
    // test performance monitor trigger profiler
    CurrentOffset = CommandLineArgs.find("-pmtrig");
    if(CurrentOffset != CommandLineArgs.npos)
    {
        const char      *pString = NULL;
        uint64_t        TmpTaskNum;

        PM_TrigTask = -1;
        PM_TrigMin  = false;
        PM_TrigMax  = false;

        pString = &CommandLineArgs.at(CurrentOffset+7);
        TmpTaskNum = (int)atoi(pString);

        if ((TmpTaskNum < 0) || (TmpTaskNum > 3))
        {
            OutputBuffer.pappend("Performance Monitor trigger Disabled\n");
        }
        else
        {
            pString = pString + strspn(pString, "0123456789 ");
            if (strstr(pString, "min") != NULL)
            {
                PM_TrigMin = true;
            }
            if (strstr(pString, "max") != NULL)
            {
                PM_TrigMax = true;
            }
            PM_TrigTask = TmpTaskNum;
            OutputBuffer.pappend("Performance Monitor trigger on Task %d%s%s\n", PM_TrigTask, (PM_TrigMin ? "  min":""), (PM_TrigMax ? "  max":""));
        }

        return 1;
    }

    //-------------------------------------------------------------------------
    // display performance monitor statistics for probe
    CurrentOffset = CommandLineArgs.find("-pmprobe");
    if(CurrentOffset != CommandLineArgs.npos)
    {
        OutputBuffer.pappend("Performance Monitor Statistics\n");
        OutputBuffer.pappend("   %s\n", (PIN(Brg1Running) && PIN(Brg2Running) ? "Running":"Stopped"));
        OutputBuffer.pappend("\n");

        OutputBuffer.pappend("Probe data        Minimum   Average   Maximum\n");
        OutputBuffer.pappend("ExecTime         %8.2f  %8.2f  %8.2f  usec\n",
                            (float)PM_Probe.Cycle.Min * CpuCpsRcpUsec,
                            (float)PM_Probe.Cycle.Avg * CpuCpsRcpUsec,
                            (float)PM_Probe.Cycle.Max * CpuCpsRcpUsec );
        OutputBuffer.pappend("Cycles           %8u  %8u  %8u\n",
                            PM_Probe.Cycle.Min, PM_Probe.Cycle.Avg, PM_Probe.Cycle.Max );
        OutputBuffer.pappend("Instructions     %8u  %8u  %8u\n",
                            PM_Probe.Cntr[0].Min, PM_Probe.Cntr[0].Avg, PM_Probe.Cntr[0].Max );
        OutputBuffer.pappend("L2 Cache Misses  %8u  %8u  %8u\n",
                            PM_Probe.Cntr[1].Min, PM_Probe.Cntr[1].Avg, PM_Probe.Cntr[1].Max );
        OutputBuffer.pappend("\n");

        return 1;
    }
#endif

    return 0;   // did not find a valid command line option
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      initialize event time to UTC time
//
// EXECUTION LEVEL
//      initilization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void    SyncTimeInit(void)
{

    // get utc time
    if (!Syncd.IsOpen())
    {
        Syncd.Open();
    }
    if (Syncd.IsOpen())
    {
        UTCTime = Syncd.PreciseUtcTime();
    }
    else
    {
        clock_gettime(CLOCK_REALTIME, &UTCTime);
    }

    // calculate new offset
    if (UTCTime.tv_sec > (unsigned)SysTm1.Sec)
    {
        EventTmOffs.Sec  = (int)UTCTime.tv_sec  - SysTm1.Sec;
        EventTmOffs.nSec = 0;

        EventTm1.Sec  = (int)UTCTime.tv_sec;
        EventTm1.nSec = 0;
        EventTm2     = EventTm1;
        EventTm3     = EventTm1;
    }

    pthread_mutex_init(&TimeSyncMutex, NULL);

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Synchronize event time to UTC time
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void    SyncTimeT3(void)
{
    if (EventTmOffs.Sec == 0)
        SyncTimeInit();

    // keep time synchronized when not using system clock
    if ( !XopsSourceIsSysClock )
    {

        // get utc time
        if (!Syncd.IsOpen())
        {
            Syncd.Open();
        }
        else
        {
            UTCTime = Syncd.PreciseUtcTime();

            if (pthread_mutex_lock(&TimeSyncMutex) == EOK)
            {
                // UTC time error = UTC time - Event time
                LastUtcErr = UTCTime.tv_sec - SysTm1.Sec - EventTmOffs.Sec
                        + (UTCTime.tv_nsec - SysTm1.nSec - EventTmOffs.nSec) * 1.0e-9F ;

                // calculate new offset
                EventTmOffs.Sec  = (int)UTCTime.tv_sec  - SysTm1.Sec;
                EventTmOffs.nSec = (int)UTCTime.tv_nsec - SysTm1.nSec;

                if (EventTmOffs.nSec < 0)
                {
                    EventTmOffs.nSec += 1000000000;
                    --EventTmOffs.Sec;
                }
                pthread_mutex_unlock(&TimeSyncMutex);
            }

            // check for NTP lock
            if (Syncd.IsNtpLocked())
                SyncToUtc = TRUE;
            else
                SyncToUtc = FALSE;
        }

        // alarm for loss of UTC time lock  (disable alarm for now, must revist and align with Mk6 behaviors  JMN)
        //if (!SyncToUtc)
        //    PUSH_DIAG(UtcUnLock);
    }
    else
    {
        SyncToUtc = FALSE;
    }

    EventOffSec  = EventTmOffs.Sec;
    EventOffNsec = EventTmOffs.nSec;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Execution measurement localized measurment probes
//
// EXECUTION LEVEL
//      where ever the measurment is made
//
// RETURN VALUE
//      none
//
//      test for interrupts disabled
//      volatile unsigned psw_var;
//      psw_var=pswget();
//      if ((psw_var & 0x00000200) != 0) ++IntEnableErr2;
//
///////////////////////////////////////////////////////////////////////////////
void XmsProbeStart (
    XmsPidTyp    pid     // probe id # (must be less than the length of the XmsProbe array)
)
{
    InterruptDisable();
    ReadPmc( 1, &XmsProbe[(unsigned)pid].Pm1Start );
    ReadPmc( 0, &XmsProbe[(unsigned)pid].Pm0Start );
    XmsProbe[(unsigned)pid].ClkStart = ClockCycles();
    return;
}

void XmsProbeStartNested (
    XmsPidTyp    pid     // probe id # (must be less than the length of the XmsProbe array)
)
{
    ReadPmc( 1, &XmsProbe[(unsigned)pid].Pm1Start );
    ReadPmc( 0, &XmsProbe[(unsigned)pid].Pm0Start );
    XmsProbe[(unsigned)pid].ClkStart = ClockCycles();
    return;
}

void XmsProbeEnd (
    XmsPidTyp    pid     // probe id # (must be less than the length of the XmsProbe array)
)
{
    XmsProbe[(unsigned)pid].ClkEnd = ClockCycles();
    ReadPmc( 0, &XmsProbe[(unsigned)pid].Pm0End );
    ReadPmc( 1, &XmsProbe[(unsigned)pid].Pm1End );
    if ( (pswget() & 0x00000200) != 0 ) ++XmsProbe[pid].IsrEnbCtr;
    InterruptEnable();
    return;
}

void XmsProbeEndNested (
    XmsPidTyp    pid     // probe id # (must be less than the length of the XmsProbe array)
)
{
    XmsProbe[(unsigned)pid].ClkEnd = ClockCycles();
    ReadPmc( 0, &XmsProbe[(unsigned)pid].Pm0End );
    ReadPmc( 1, &XmsProbe[(unsigned)pid].Pm1End );
    if ( (pswget() & 0x00000200) != 0 ) ++XmsProbe[pid].IsrEnbCtr;
    return;
}

void XmsProbeCalc (
    float DelTm         // calculation period (sec) needed for filter
)
{
    //-------------------------------------------------------------------------
    // clockcycles
    for ( unsigned pid = 0 ; pid < ARRAY_LEN(XmsProbe) ; ++pid )
    {
        // skip the calculations if no new data since the reset
        if ( (XmsProbe[pid].ClkEnd != 0) || (XmsProbe[pid].ClkStart != 0) )
        {
            // calculate delta time and average delta time
            XmsProbe[pid].ClkDelta = (uint32_t)(XmsProbe[pid].ClkEnd - XmsProbe[pid].ClkStart);

            // calculate average delta time
            XmsProbe[pid].ClkDeltaAvg += (XMS_W_FIL * DelTm) * ( (float)XmsProbe[pid].ClkDelta - XmsProbe[pid].ClkDeltaAvg );

            // detect new max
            if ( XmsProbe[pid].ClkDeltaMax < XmsProbe[pid].ClkDelta )
            {
                XmsProbe[pid].ClkDeltaMax = XmsProbe[pid].ClkDelta;
            }
            // detect new min
            if ( XmsProbe[pid].ClkDeltaMin > XmsProbe[pid].ClkDelta )
            {
                XmsProbe[pid].ClkDeltaMin = XmsProbe[pid].ClkDelta;
            }
        }

        // write to the published var
        if ( XmsProbe[pid].pClkProbe != NULL )
        {
            *XmsProbe[pid].pClkProbe = (int)XmsProbe[pid].ClkDelta * CpuCpsRcpUsec;
        }
    }

    //-------------------------------------------------------------------------
    // pm0
    for ( unsigned pid = 0 ; pid < ARRAY_LEN(XmsProbe) ; ++pid )
    {
        // skip the calculations if no new data since the reset
        if ( (XmsProbe[pid].Pm0End != 0) || (XmsProbe[pid].Pm0Start != 0) )
        {
            // calculate delta time and average delta time
            XmsProbe[pid].Pm0Delta = (uint32_t)(XmsProbe[pid].Pm0End - XmsProbe[pid].Pm0Start);

            // calculate average delta time
            XmsProbe[pid].Pm0DeltaAvg += (XMS_W_FIL * DelTm) * ( (float)XmsProbe[pid].Pm0Delta - XmsProbe[pid].Pm0DeltaAvg );

            // detect new max
            if ( XmsProbe[pid].Pm0DeltaMax < XmsProbe[pid].Pm0Delta )
            {
                XmsProbe[pid].Pm0DeltaMax = XmsProbe[pid].Pm0Delta;
            }
            // detect new min
            if ( XmsProbe[pid].Pm0DeltaMin > XmsProbe[pid].Pm0Delta )
            {
                XmsProbe[pid].Pm0DeltaMin = XmsProbe[pid].Pm0Delta;
            }
        }

        // write to the published var
        if ( XmsProbe[pid].pPm0Probe != NULL )
        {
            *XmsProbe[pid].pPm0Probe = XmsProbe[pid].Pm0Delta;
        }
    }

    //-------------------------------------------------------------------------
    // pm1
    for ( unsigned pid = 0 ; pid < ARRAY_LEN(XmsProbe) ; ++pid )
    {
        // skip the calculations if no new data since the reset
        if ( (XmsProbe[pid].Pm1End != 0) || (XmsProbe[pid].Pm1Start != 0) )
        {
            // calculate delta time and average delta time
            XmsProbe[pid].Pm1Delta = (uint32_t)(XmsProbe[pid].Pm1End - XmsProbe[pid].Pm1Start);

            // calculate average delta time
            XmsProbe[pid].Pm1DeltaAvg += (XMS_W_FIL * DelTm) * ( (float)XmsProbe[pid].Pm1Delta - XmsProbe[pid].Pm1DeltaAvg );

            // detect new max
            if ( XmsProbe[pid].Pm1DeltaMax < XmsProbe[pid].Pm1Delta )
            {
                XmsProbe[pid].Pm1DeltaMax = XmsProbe[pid].Pm1Delta;
            }
            // detect new min
            if ( XmsProbe[pid].Pm1DeltaMin > XmsProbe[pid].Pm1Delta )
            {
                XmsProbe[pid].Pm1DeltaMin = XmsProbe[pid].Pm1Delta;
            }
        }

        // write to the published var
        if ( XmsProbe[pid].pPm1Probe != NULL )
        {
            *XmsProbe[pid].pPm1Probe = XmsProbe[pid].Pm1Delta;
        }
    }

    //-------------------------------------------------------------------------
    // work is complete
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Clear specific data in an XmsBufTyp buffer to prepare it for reuse.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void XmsBufClear ( XmsBufTyp* pXmsBuf )
{
    //-------------------------------------------------------------------------
    // zero the calc buffer to make ready for use by the samplers
    for ( unsigned meas = 0 ; meas < ARRAY_LEN(pXmsBuf->Data) ; ++meas )
    {
        for ( unsigned i = 0 ; i < pXmsBuf->Sidx[meas] ; ++i )
        {
            pXmsBuf->Data[meas][i].DeltaClkAdjCnt = 0;
        }
        pXmsBuf->Sidx[meas] = 0;
    }

    //-------------------------------------------------------------------------
    // brute force method, expected to take more time than scheme above
    //memset( (void*)pXmsBuf, 0, sizeof(XmsBufTyp) );

    //-------------------------------------------------------------------------
    // work is complete
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Execution measurment statistics calculations
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void XmsCalc (void)
{
    #define         CLK_ROLL    (0x80000000)    // for uint32_t comparisions
    unsigned        XmsSofIdx;
    uint32_t        Temp;
    float           TempF;

#ifdef XMS_DEBUG_BUF
    unsigned        DbgSample = false;
#endif

    //---------------------------------------------------------------------
    // calculation enable sequencing
    {
        if ( !T3CalcDone || !XmsCalcEnb )
        {
            return;
        }
        // force two rounds of buffer clearing (to clear both buffers) before starting the calculations
        else if ( XmsCalcEnb < 3 )
        {
            XmsFilSettleGuage = 0.0F;
            XmsMinMaxAccumTm = 0.0F;
            XmsBufClear(pXmsCalcBuf);
            ++XmsCalcEnb;
            return;
        }
    }

    //---------------------------------------------------------------------
    // for each measurement ...
    for ( unsigned meas = 0 ; meas < ARRAY_LEN(pXmsCalcBuf->Info) ; ++meas )
    {
        if ( !SimMode )
        {
            if ( meas == XMS_TSK1S )
            {
                continue;
            }
        }

        // for each sample ...
        //  calculate the delta clocks for each sample and
        //  remove clocks that can be attributed to higher priority preempting measurements
        for ( unsigned samp = 0 ; samp < pXmsCalcBuf->Sidx[meas] ; ++samp )
        {
            // total delta clocks in the sample (including higher priority preempting measurement)
            pXmsCalcBuf->Data[meas][samp].DeltaClk = pXmsCalcBuf->Data[meas][samp].EndClk - pXmsCalcBuf->Data[meas][samp].StartClk;

            // for each higher priority measurement ...
            for ( unsigned rmvmeas = 0 ; rmvmeas < meas ; ++rmvmeas )
            {
                if ( !SimMode )
                {
                    if ( rmvmeas == XMS_TSK1S )
                    {
                        continue;
                    }
                }
                // for each sample of the higher priority measurement ...
                for ( unsigned rmvsamp = 0 ; rmvsamp < pXmsCalcBuf->Sidx[rmvmeas] ; ++rmvsamp )
                {
                    // if higher priority measurement sample start time is after lower priority measurement end time
                    if ( (pXmsCalcBuf->Data[rmvmeas][rmvsamp].StartClk - pXmsCalcBuf->Data[meas][samp].EndClk) < CLK_ROLL )
                    {
                        // stop looking
                        break;
                    }
                    // if higher priority measurement start time is after lower priority measurement start time
                    if ( (pXmsCalcBuf->Data[rmvmeas][rmvsamp].StartClk - pXmsCalcBuf->Data[meas][samp].StartClk) < CLK_ROLL )
                    {
                        // remove the delta clocks of the higher priority measurement
                        pXmsCalcBuf->Data[meas][samp].DeltaClk -= pXmsCalcBuf->Data[rmvmeas][rmvsamp].DeltaClk;
                        pXmsCalcBuf->Data[meas][samp].DeltaClkAdjCnt++;
                    }
                }
            }
        }

        //-----------------------------------------------------------------
        // reset the measurement statistics for the sample set
        pXmsCalcBuf->Info[meas].StartDelClk.Max = 0;
        pXmsCalcBuf->Info[meas].StartDelClk.Min = 0x7FFFFFFF;
        pXmsCalcBuf->Info[meas].StartDelClk.Avg = 0.0F;

        pXmsCalcBuf->Info[meas].EndDelClk.Max = 0;
        pXmsCalcBuf->Info[meas].EndDelClk.Min = 0x7FFFFFFF;
        pXmsCalcBuf->Info[meas].EndDelClk.Avg = 0.0F;

        pXmsCalcBuf->Info[meas].DeltaClk.Max = 0;
        pXmsCalcBuf->Info[meas].DeltaClk.Min = 0x7FFFFFFF;
        pXmsCalcBuf->Info[meas].DeltaClk.Avg = 0.0F;

        //-----------------------------------------------------------------
        // min/max/avg for the measurment set
        //  for each sample ...
        for ( unsigned samp = 0 ; samp < pXmsCalcBuf->Sidx[meas] ; ++samp )
        {
            // estimate the cpu clock at frame boundary
            XmsSofIdx = pXmsCalcBuf->Data[meas][samp].SofDataIdx;
            FrmClkEst = (uint32_t)(ClkFrcValT3.ClkVal - (uint64_t)(FrcClkToCpuClk * (ClkFrcValT3.FrcVal - pXmsCalcBuf->SofData[XmsSofIdx].FrmFrc)));

            // start clock delta from frame boundary
            Temp = pXmsCalcBuf->Data[meas][samp].StartClk - FrmClkEst;
            if ( pXmsCalcBuf->Info[meas].StartDelClk.Max < Temp )
            {
                pXmsCalcBuf->Info[meas].StartDelClk.Max = Temp;
            }
            if ( pXmsCalcBuf->Info[meas].StartDelClk.Min > Temp )
            {
                pXmsCalcBuf->Info[meas].StartDelClk.Min = Temp;
            }
            pXmsCalcBuf->Info[meas].StartDelClk.Avg += (float)Temp;

            // end clock delta from frame boundary
            Temp = pXmsCalcBuf->Data[meas][samp].EndClk - FrmClkEst;
            if ( pXmsCalcBuf->Info[meas].EndDelClk.Max < Temp )
            {
                pXmsCalcBuf->Info[meas].EndDelClk.Max = Temp;
            }
            if ( pXmsCalcBuf->Info[meas].EndDelClk.Min > Temp )
            {
                pXmsCalcBuf->Info[meas].EndDelClk.Min = Temp;
            }
            pXmsCalcBuf->Info[meas].EndDelClk.Avg += (float)Temp;

            // end clock delta from start (clocks from higher priority preempting measurement samples already removed)
            Temp = pXmsCalcBuf->Data[meas][samp].DeltaClk;
            if ( pXmsCalcBuf->Info[meas].DeltaClk.Max < Temp )
            {
                pXmsCalcBuf->Info[meas].DeltaClk.Max = Temp;
            }
            if ( pXmsCalcBuf->Info[meas].DeltaClk.Min > Temp )
            {
                pXmsCalcBuf->Info[meas].DeltaClk.Min = Temp;
            }
            pXmsCalcBuf->Info[meas].DeltaClk.Avg += (float)Temp;

        }
        TempF = 1.0F / (float)((int)pXmsCalcBuf->Sidx[meas]);
        pXmsCalcBuf->Info[meas].StartDelClk.Avg *= TempF;
        pXmsCalcBuf->Info[meas].EndDelClk.Avg   *= TempF;
        pXmsCalcBuf->Info[meas].DeltaClk.Avg    *= TempF;

        //-----------------------------------------------------------------
        // global min/max

        // start clock delta from frame boundary
        if ( XmsGlobInfo[meas].StartDelClk.Max < pXmsCalcBuf->Info[meas].StartDelClk.Max )
        {
            XmsGlobInfo[meas].StartDelClk.Max = pXmsCalcBuf->Info[meas].StartDelClk.Max;
        }
        if ( XmsGlobInfo[meas].StartDelClk.Min > pXmsCalcBuf->Info[meas].StartDelClk.Min )
        {
            XmsGlobInfo[meas].StartDelClk.Min = pXmsCalcBuf->Info[meas].StartDelClk.Min;
        }

        // end clock delta from frame boundary
        if ( XmsGlobInfo[meas].EndDelClk.Max < pXmsCalcBuf->Info[meas].EndDelClk.Max )
        {
            XmsGlobInfo[meas].EndDelClk.Max = pXmsCalcBuf->Info[meas].EndDelClk.Max;
        }
        if ( XmsGlobInfo[meas].EndDelClk.Min > pXmsCalcBuf->Info[meas].EndDelClk.Min )
        {
            XmsGlobInfo[meas].EndDelClk.Min = pXmsCalcBuf->Info[meas].EndDelClk.Min;
        }

        // end clock delta from start (clocks from higher priority preempting measurement samples already removed)
        if ( XmsGlobInfo[meas].DeltaClk.Max < pXmsCalcBuf->Info[meas].DeltaClk.Max )
        {
            XmsGlobInfo[meas].DeltaClk.Max = pXmsCalcBuf->Info[meas].DeltaClk.Max;
        }
        if ( XmsGlobInfo[meas].DeltaClk.Min > pXmsCalcBuf->Info[meas].DeltaClk.Min )
        {
            XmsGlobInfo[meas].DeltaClk.Min = pXmsCalcBuf->Info[meas].DeltaClk.Min;
        }
    }

#ifdef XMS_DEBUG_BUF
    //-----------------------------------------------------------------
    // debug capture of the sample buffer for llm
    //  (must be done after all sample calcultions are completed and before prep for buffer swap)
    if ( DbgSample )
    {
        *pXmsDebugBuf = *pXmsCalcBuf;
    }
#endif

    //-----------------------------------------------------------------
    // global avg low pass filters
    for ( unsigned meas = 0 ; meas < ARRAY_LEN(XmsGlobInfo) ; ++meas )
    {
        XmsGlobInfo[meas].StartDelClk.Avg += (XMS_W_FIL * DelTm3) * (pXmsCalcBuf->Info[meas].StartDelClk.Avg - XmsGlobInfo[meas].StartDelClk.Avg);
        XmsGlobInfo[meas].EndDelClk.Avg   += (XMS_W_FIL * DelTm3) * (pXmsCalcBuf->Info[meas].EndDelClk.Avg - XmsGlobInfo[meas].EndDelClk.Avg);
        XmsGlobInfo[meas].DeltaClk.Avg    += (XMS_W_FIL * DelTm3) * (pXmsCalcBuf->Info[meas].DeltaClk.Avg - XmsGlobInfo[meas].DeltaClk.Avg);
    }

    //-----------------------------------------------------------------
    // guages for filter settling time and min/max accumlation time
    XmsFilSettleGuage += (XMS_W_FIL * DelTm3) * (1.0F - XmsFilSettleGuage);
    XmsMinMaxAccumTm  += DelTm3;

    //-------------------------------------------------------------------------
    // zero the calc buffer to make ready for use by the samplers
    XmsBufClear(pXmsCalcBuf);

    //-------------------------------------------------------------------------
    // work is complete
    return;
}

#ifdef XOPS_PERF_MON_TASK
///////////////////////////////////////////////////////////////////////////////
void PerfMonTaskStart(int TaskNum)
{
    for (int Sidx=0; Sidx<4; ++Sidx)
        if (TaskRunning[Sidx])
            ++TaskInterrupted[Sidx];
    TaskRunning[TaskNum] = true;

    TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_PERFMON_MEAS+TaskNum, TaskStartStr[TaskNum] );

    InterruptDisable();
    ReadTsc(&PM_Task[TaskNum].Cycle.Start);
    ReadPmc(0, &PM_Task[TaskNum].Ctr[0].Start);
    ReadPmc(1, &PM_Task[TaskNum].Ctr[1].Start);
    InterruptEnable();
}

///////////////////////////////////////////////////////////////////////////////
void PerfMonTaskEnd(int TaskNum )
{
    uint64_t    PM_Cnt[2], PM_TmCnt;
    uint32_t    L2CacheMisses, ClkCycles, Instructions;
    float       CPI;
    int         Sidx;

    InterruptDisable();
    ReadPmc(1, &PM_Cnt[1]);      // clock cycles
    ReadPmc(0, &PM_Cnt[0]);      // instructions
    ReadTsc(&PM_TmCnt);          // time counts

    ClkCycles     = PM_TmCnt - PM_Task[TaskNum].Cycle.Start;
    Instructions  = PM_Cnt[0] - PM_Task[TaskNum].Ctr[0].Start;
    L2CacheMisses = PM_Cnt[1] - PM_Task[TaskNum].Ctr[1].Start;
    InterruptEnable();

    TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_PERFMON_MEAS+TaskNum, TaskEndStr[TaskNum] );

    if ( ((ClkCycles & 0xF0000000) != 0) || ((Instructions & 0xF0000000) != 0) || ((L2CacheMisses & 0xF0000000) != 0) )
    {
        ++TaskBadData[TaskNum];
        return;
    }

    ++PM_Task[TaskNum].Cycle.SmplCnt;
    PM_Task[TaskNum].Cycle.SmplAcc += ClkCycles;
    for (Sidx = TaskNum+1; Sidx < (int)(sizeof(PM_Task)/sizeof(PM_Task[0])); ++Sidx)
        PM_Task[Sidx].Cycle.Start   += ClkCycles;

    ++PM_Task[TaskNum].Ctr[0].SmplCnt;
    PM_Task[TaskNum].Ctr[0].SmplAcc += Instructions;
    for (Sidx = TaskNum+1; Sidx < (int)(sizeof(PM_Task)/sizeof(PM_Task[0])); ++Sidx)
        PM_Task[Sidx].Ctr[0].Start   += Instructions;

    ++PM_Task[TaskNum].Ctr[1].SmplCnt;
    PM_Task[TaskNum].Ctr[1].SmplAcc += L2CacheMisses;
    for (Sidx = TaskNum+1; Sidx < (int)(sizeof(PM_Task)/sizeof(PM_Task[0])); ++Sidx)
        PM_Task[Sidx].Ctr[1].Start   += L2CacheMisses;

    TaskRunning[TaskNum] = false;
    CPI      = (double)ClkCycles / (double)Instructions;

    if ( !InitDone )
    {
        PM_Task[TaskNum].ExecTime.Min.Cycles   = ClkCycles + ClkCycles;
        PM_Task[TaskNum].ExecTime.Max.Cycles   = 0;
        PM_Task[TaskNum].CPI.Min.CPI           = CPI + CPI;
        PM_Task[TaskNum].CPI.Max.CPI           = 0;
    }
    else
    {
        if ( PM_Task[TaskNum].ExecTime.Min.Cycles > ClkCycles )
        {
            PM_Task[TaskNum].ExecTime.Min.Cycles        = ClkCycles;
            PM_Task[TaskNum].ExecTime.Min.Instructions  = Instructions;
            PM_Task[TaskNum].ExecTime.Min.CPI           = CPI;
            PM_Task[TaskNum].ExecTime.Min.L2CacheMisses = L2CacheMisses;

            if ((PM_TrigTask == TaskNum) && PM_TrigMin)
            {
                TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_PERFMON_TRIG+TaskNum, "TRIGGER ON TASK MIN" );
                TraceEvent(_NTO_TRACE_STOP);
            }
        }
        else if ( PM_Task[TaskNum].ExecTime.Max.Cycles < ClkCycles )
        {
            PM_Task[TaskNum].ExecTime.Max.Cycles        = ClkCycles;
            PM_Task[TaskNum].ExecTime.Max.Instructions  = Instructions;
            PM_Task[TaskNum].ExecTime.Max.CPI           = CPI;
            PM_Task[TaskNum].ExecTime.Max.L2CacheMisses = L2CacheMisses;

            if ((PM_TrigTask == TaskNum) && PM_TrigMax)
            {
                TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_PERFMON_TRIG+TaskNum, "TRIGGER ON TASK MAX" );
                TraceEvent(_NTO_TRACE_STOP);
            }
        }

        if ( PM_Task[TaskNum].CPI.Min.CPI > CPI )
        {
            PM_Task[TaskNum].CPI.Min.Cycles             = ClkCycles;
            PM_Task[TaskNum].CPI.Min.Instructions       = Instructions;
            PM_Task[TaskNum].CPI.Min.CPI                = CPI;
            PM_Task[TaskNum].CPI.Min.L2CacheMisses      = L2CacheMisses;
        }
        else if ( PM_Task[TaskNum].CPI.Max.CPI < CPI )
        {
            PM_Task[TaskNum].CPI.Max.Cycles             = ClkCycles;
            PM_Task[TaskNum].CPI.Max.Instructions       = Instructions;
            PM_Task[TaskNum].CPI.Max.CPI                = CPI;
            PM_Task[TaskNum].CPI.Max.L2CacheMisses      = L2CacheMisses;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void PerfMonCalc(void)
{
    static uint64_t PM_AvgStrtTick=0;
    int             Sidx;

    if ( InitDone )
    {
        if ((ClockCycles() - PM_AvgStrtTick) > (uint64_t)(10.0 * CpuCps))
        {
            for (Sidx = 0; (PM_AvgStrtTick != 0) && (Sidx < (int)(sizeof(PM_Task)/sizeof(PM_Task[0]))); ++Sidx)
            {
                PM_Task[Sidx].Average.Cycles         = PM_Task[Sidx].Cycle.SmplAcc / PM_Task[Sidx].Cycle.SmplCnt ;

                PM_Task[Sidx].Average.Instructions   = PM_Task[Sidx].Ctr[0].SmplAcc / PM_Task[Sidx].Ctr[0].SmplCnt ;

                PM_Task[Sidx].Average.CPI            = (double)PM_Task[Sidx].Cycle.SmplAcc / (double)PM_Task[Sidx].Ctr[0].SmplAcc ;

                PM_Task[Sidx].Average.L2CacheMisses  = PM_Task[Sidx].Ctr[1].SmplAcc / PM_Task[Sidx].Ctr[1].SmplCnt ;
            }

            for (Sidx = 0; Sidx < (int)(sizeof(PM_Task)/sizeof(PM_Task[0])); ++Sidx)
            {
                PM_Task[Sidx].Cycle.SmplAcc     = 0;
                PM_Task[Sidx].Cycle.SmplCnt     = 0;
                PM_Task[Sidx].Ctr[0].SmplAcc    = 0;
                PM_Task[Sidx].Ctr[0].SmplCnt    = 0;
                PM_Task[Sidx].Ctr[1].SmplAcc    = 0;
                PM_Task[Sidx].Ctr[1].SmplCnt    = 0;
            }

            PM_AvgStrtTick = ClockCycles();
        }
    }

}


///////////////////////////////////////////////////////////////////////////////
void PerfMonProbeStart(unsigned StartOfFrame)
{
//    InterruptDisable();
    ReadTsc(&PM_Probe.Cycle.Start);
    ReadPmc(0, &PM_Probe.Cntr[0].Start);
    ReadPmc(1, &PM_Probe.Cntr[1].Start);
    PM_Probe.FrcStartOfFrame = StartOfFrame;
    PM_Probe.FrcBegin = pFrcSched->GetCounter();
//    InterruptEnable();
}

///////////////////////////////////////////////////////////////////////////////
void PerfMonProbeEnd(void)
{
    uint64_t    CycleEnd,  CntrEnd[2];
    uint32_t    ClkCycles, CntrDiff[2];

//    InterruptDisable();
    ReadTsc(&CycleEnd);         // time counts
    ReadPmc(0, &CntrEnd[0]);
    ReadPmc(1, &CntrEnd[1]);
    PM_Probe.FrcEnd = pFrcSched->GetCounter();
//    InterruptEnable();

    ClkCycles   = CycleEnd - PM_Probe.Cycle.Start;
    CntrDiff[0] = CntrEnd[0] - PM_Probe.Cntr[0].Start;
    CntrDiff[1] = CntrEnd[1] - PM_Probe.Cntr[1].Start;


    if ( ((ClkCycles & 0xF0000000) != 0) || ((CntrDiff[0] & 0xF0000000) != 0) || ((CntrDiff[1] & 0xF0000000) != 0) )
    {
        return;     // bad data
    }

    ++PM_Probe.Cycle.SmplCnt;
    PM_Probe.Cycle.SmplAcc += ClkCycles;

    ++PM_Probe.Cntr[0].SmplCnt;
    PM_Probe.Cntr[0].SmplAcc += CntrDiff[0];

    ++PM_Probe.Cntr[1].SmplCnt;
    PM_Probe.Cntr[1].SmplAcc += CntrDiff[1];

    if ( !InitDone )
    {
        PM_Probe.Cycle.Min  = ClkCycles + ClkCycles;
        PM_Probe.Cycle.Max  = 0;

        PM_Probe.Cntr[0].Min = CntrDiff[0] + CntrDiff[0];
        PM_Probe.Cntr[0].Max = 0;

        PM_Probe.Cntr[1].Min = CntrDiff[1] + CntrDiff[1];
        PM_Probe.Cntr[1].Max = 0;
    }
    else
    {
        if (PM_Probe.Cycle.Min > ClkCycles)
            PM_Probe.Cycle.Min = ClkCycles;
        if (PM_Probe.Cycle.Max < ClkCycles)
            PM_Probe.Cycle.Max = ClkCycles;

        if (PM_Probe.Cntr[0].Min > CntrDiff[0])
            PM_Probe.Cntr[0].Min = CntrDiff[0];
        if (PM_Probe.Cntr[0].Max < CntrDiff[0])
            PM_Probe.Cntr[0].Max = CntrDiff[0];

        if (PM_Probe.Cntr[1].Min > CntrDiff[1])
            PM_Probe.Cntr[1].Min = CntrDiff[1];
        if (PM_Probe.Cntr[1].Max < CntrDiff[1])
            PM_Probe.Cntr[1].Max = CntrDiff[1];
    }

    // published variables
    PM_ProbeFrameBegin  = (float)(PM_Probe.FrcBegin - PM_Probe.FrcStartOfFrame) / FrcFreq * 1e6;
    PM_ProbeFrameEnd    = (float)(PM_Probe.FrcEnd - PM_Probe.FrcStartOfFrame) / FrcFreq * 1e6;
    PM_ProbeExecTime    = (float)ClkCycles * CpuCpsRcpUsec;
    PM_ProbeClkCycle    = ClkCycles;
    PM_ProbeInstruction = CntrDiff[0];
    PM_ProbeL2CacheMiss = CntrDiff[1];
}

///////////////////////////////////////////////////////////////////////////////
void PerfMonProbeCalc(void)
{
    static uint64_t PM_AvgStrtTick=0;

    if ( InitDone )
    {
        if ((ClockCycles() - PM_AvgStrtTick) > (uint64_t)(60.0 * CpuCps))
        {
            if (PM_AvgStrtTick != 0)
            {
                PM_Probe.Cycle.Avg    = PM_Probe.Cycle.SmplAcc / PM_Probe.Cycle.SmplCnt ;
                PM_Probe.Cntr[0].Avg  = PM_Probe.Cntr[0].SmplAcc / PM_Probe.Cntr[0].SmplCnt ;
                PM_Probe.Cntr[1].Avg  = PM_Probe.Cntr[1].SmplAcc / PM_Probe.Cntr[1].SmplCnt ;
            }

            PM_Probe.Cycle.SmplAcc    = 0;
            PM_Probe.Cycle.SmplCnt    = 0;
            PM_Probe.Cntr[0].SmplAcc  = 0;
            PM_Probe.Cntr[0].SmplCnt  = 0;
            PM_Probe.Cntr[1].SmplAcc  = 0;
            PM_Probe.Cntr[1].SmplCnt  = 0;

            PM_AvgStrtTick = ClockCycles();
        }
    }

}
#endif


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cMarkStatAlphaWdog::cMarkStatAlphaWdog()
{
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cMarkStatAlphaWdog::~cMarkStatAlphaWdog()
{
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void cMarkStatAlphaWdog::Initialize (
    cMonitorCommand &Monitor,       // global low level monitor
    float CheckPeriod               // normal execution rate to check registered dogs (sec)
)
{
    float TmpSec, TmpNsec;

    Period = CheckPeriod;

    // Send WatchDog Registration to Sys
    {
        cSysMsgWatchDogRegistration      WatchDogReg;

        // Setup WatchDog
        TmpNsec = modff(CheckPeriod, &TmpSec) * 1e9F;
        WatchDogReg.mName           = "MarkStat";
        WatchDogReg.mAction         = REBOOT;
        WatchDogReg.mPeriod.tv_sec  = (int)TmpSec;
        WatchDogReg.mPeriod.tv_nsec = (int)TmpNsec;

        if (SysWdog.Open() == -1)
        {
            MarkStatDiag.LogErrorMessage(ALARM_MSG, cParamString(MKS_INIT_STR,
                cParamString("Error opening WatchDog Client'%s'", strerror(errno)).c_str()),
                HERE);
            return ;
        }
        if (SysWdog.Register(WatchDogReg) == -1)
        {
            MarkStatDiag.LogErrorMessage(ALARM_MSG, cParamString(MKS_INIT_STR,
                cParamString("Error sending/receiving Watchdog Registration with Sys '%s'", strerror(errno)).c_str()),
                HERE);
            return ;
        }
    }

    // add monitor command
    cParamString HelpString;
    HelpString =  "Watchdog";
    HelpString += "-r Reset Stats\n";
    Monitor.Add( "Watchdog", HelpString, Llm, NORMAL_OUTPUT_SIZE, "wdog" );
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void cMarkStatAlphaWdog::CheckRegDogs()
{
    cSLinkedList <cMarkStatWdog*>::cIterator Iter;

    // scan tasks registered with me
    for (Iter = RegDogList.Begin(); Iter != RegDogList.End(); ++Iter)
    {
        (*Iter)->Check();
    }

    // tickle watchdog registered with sys
    // occurs at the end of the loop to ensure all code has an opportunity to run
    SysWdog.Tickle();
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
int cMarkStatAlphaWdog::Llm(cParamString &OutputBuffer, cParamString &CommandLineArgs)
{
    int     CurrentOffset;

    OutputBuffer.pappend("MarkStat Watchdog\n" );

    CurrentOffset = CommandLineArgs.find("-r");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        cSLinkedList <cMarkStatWdog*>::cIterator Iter;

        OutputBuffer.pappend("  Reset\n" );
        // scan tasks registered with me
        for (Iter = RegDogList.Begin(); Iter != RegDogList.End(); ++Iter)
        {
            (*Iter)->ResetStats();
        }
    }

    else
    {
        cSLinkedList <cMarkStatWdog*>::cIterator Iter;

        OutputBuffer.pappend("  ----- %s -----\n", "Alpha" );
        OutputBuffer.pappend("  Period:    %f\n", Period );
        OutputBuffer.pappend("\n");

        // scan tasks registered with me
        for (Iter = RegDogList.Begin(); Iter != RegDogList.End(); ++Iter)
        {
            (*Iter)->LlmSummary(OutputBuffer, CommandLineArgs);
        }
    }

    OutputBuffer.pappend("\n");
    return 1;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cMarkStatWdog::cMarkStatWdog()
{
    // add me to the list of dogs to watch
    // do this in the constructor in case the task does not start and the resister method never runs
    RegDogList.Insert(this);

    Period          = 0;
    AvgMin          = 0;
    AvgMax          = 0;
    AvgCount        = 0;
    InitTimer       = 10;

    AvgPeriod       = 0;
    TickleCnt       = 0;
    TickleCnt_Z1    = 0;;
    ClkCyc_Z1       = 0;
    ClkCycInc       = 0;
    TickleGood      = false;
    LogDelay        = 0;

    MaxPeriod = 0;
    MinPeriod = 1000;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cMarkStatWdog::~cMarkStatWdog()
{
    // remove me from the list of dogs to watch
    RegDogList.Remove(this);
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void cMarkStatWdog::Register(MarkStatWdogRegistration Reg)
{
    Name         = Reg.Name;
    Period       = Reg.Period;
    AvgMin       = Reg.AvgMin;
    AvgMax       = Reg.AvgMax;
    AvgCount     = Reg.AvgCount;
    InitTimer    = Reg.InitTimer;

    TickleCnt    = 0;
    TickleCnt_Z1 = 0;
    TickleGood   = false;
    ClkCyc_Z1    = ClockCycles();
    ClkCycInc    = (uint64_t)(Period * (float)AvgCount * CpuCps);
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void cMarkStatWdog::Tickle()
{
    ++TickleCnt;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void cMarkStatWdog::Check()
{
    if ( (ClockCycles() - ClkCyc_Z1) > ClkCycInc )
    {
        // calculate average period
        if (TickleCnt != TickleCnt_Z1)
        {
            if (TickleGood)
            {
                AvgPeriod = (float)(ClockCycles() - ClkCyc_Z1) * CpuCpsRcp / (float)(TickleCnt - TickleCnt_Z1);

                if (AvgPeriod < AvgMin)
                {
                    if (LogDelay < 2)
                        ++LogDelay;
                    else
                        PUSH_DIAG(WdogPeriodMinTrp);
                }
                else if (AvgPeriod > AvgMax)
                {
                    if (LogDelay < 2)
                        ++LogDelay;
                    else
                        PUSH_DIAG(WdogPeriodMaxTrp);
                }

                if (MaxPeriod < AvgPeriod)  MaxPeriod = AvgPeriod;
                if (MinPeriod > AvgPeriod)  MinPeriod = AvgPeriod;
            }
            else
                LogDelay = 0;

            TickleGood    = true;
        }
        // task has died
        else if (TickleCnt != 0)
        {
            TickleGood = false;
            PUSH_DIAG(WdogStoppedTrp);
        }
        // wait for tickling to begin
        else
        {
            TickleGood = false;
            InitTimer -= (float)(ClockCycles() - ClkCyc_Z1) * CpuCpsRcp;
            if (InitTimer < 0)
            {
                InitTimer = -1;
                PUSH_DIAG(WdogFailStartTrp);
            }
        }

        // prepare for next
        TickleCnt_Z1   = TickleCnt;
        ClkCyc_Z1      =  ClockCycles();
    }
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void cMarkStatWdog::ResetStats()
{
    MaxPeriod = 0;
    MinPeriod = 1000;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void cMarkStatWdog::LlmSummary(cParamString &OutputBuffer, cParamString &CommandLineArgs)
{
    OutputBuffer.pappend("  ----- %s -----\n", Name.c_str() );
    OutputBuffer.pappend("  Period:    %f\n", Period );
    OutputBuffer.pappend("  Min:       %f\n", AvgMin );
    OutputBuffer.pappend("  Max:       %f\n", AvgMax );
    OutputBuffer.pappend("  Avg Count: %d\n", AvgCount );
    OutputBuffer.pappend("  Clk Inc:   %llu\n", ClkCycInc );
    OutputBuffer.pappend("  Avg Period:%f\n", AvgPeriod );
    OutputBuffer.pappend("  Max Period:%f\n", MaxPeriod );
    OutputBuffer.pappend("  Min Period:%f\n", MinPeriod );
    OutputBuffer.pappend("  %s\n", TickleGood ? "Tickling Good":"No Tickling" );
    OutputBuffer.pappend("\n");
}

