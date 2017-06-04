
// system
#include <sys\syspage.h>

// core
#include "ThreadInfo.h"
#include "SysMemClient.h"
#include "StateMachineClient.h"

// framework
#include "IpcDevice.h"
#include "x0FrameWork.h"
#include "x0CaptureBuf.h"
#include "x0DataPass.h"
#include "x0SdiMsgServer.h"
#include "x0MsgStk.h"


// datapass item arrays can not be empty
float       DummyPassFloat;
DATA_PASS(DummyPassFloat, DummyPassFloat, T1_T2,  UNFILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T1_T3,  UNFILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T1A_T2, UNFILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T1A_T3, UNFILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T2_T1,  UNFILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T2_T3,  UNFILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T3_T1,  UNFILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T3_T2,  UNFILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T1_T2,  FILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T1_T3,  FILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T1A_T2, FILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T1A_T3, FILTERED);
DATA_PASS(DummyPassFloat, DummyPassFloat, T2_T3,  FILTERED);


qtime_entry qtime ={::qtime.cycles_per_sec=1067172800};
cThreadInfoList gThreadInfoList;

int cMessageStack::MsgQuePres  = false;
int cMessageStack::MsgPending  = false;

//=========================================================

// SysMsgClientHandlers
cStateMachineClient     gStateMachineClient;

//IpcMain
cIpcDevice              gIpcDevice;

// IpcTest
unsigned iM6e_DSPOkCnt;
unsigned IpcInterfaceOk = false;
void IpcCommTest()      {;}
bool IpcInit()          {return true;}

//MarkStat
cMarkStatDiag           MarkStatDiag;
cMonitorCommand         gMonitor;
cSysMemConfigClient     gSysMemConfigClient;
cParameter              gParameter;
cVariable               gVariable;
cDataPasser             DataPassManager;
cSdiMsgServer           SdiMsg;
cCaptureBuf             CaptureBuf("DSP_CaptureBuffer", 120000, 2);
cTripBuf                TripBuf("DSP_TripBuffer", 120000, 2);
unsigned                InitDone = false;
bool                    IpcInterfaceEnable = false;
CREATE_PARM(RuleDepCheckInh, unsigned);

// x0AdlSrv
void AdlDataSampler( float DelExeTm ) {;}
int  adlReceiveDirect(int ReceiveId)  {return 0;}
void InitAdlServer(int ChannelId)     {;}

// x0CaptureBuf
unsigned                DSP_CapBufState;
unsigned                DSP_TrpBufState;
unsigned                DSP_EsaBufState;

// x0Monitor
CREATE_PARM(SpinWaitT1,     float);
CREATE_PARM(SpinWaitT1A,    float);
CREATE_PARM(SpinWaitT1S,    float);
CREATE_PARM(SpinWaitT2,     float);
CREATE_PARM(SpinWaitT3,     float);
CREATE_PARM(SpinWaitB,      float);
int         SpinWaitCtrT1  = 0;
int         SpinWaitCtrT1A = 0;
int         SpinWaitCtrT1S = 0;
int         SpinWaitCtrT2  = 0;
int         SpinWaitCtrT3  = 0;
int         SpinWaitCtrB   = 0;

// x0Status
void ToolboxStatus()            {;}

// x0TestUtility
CREATE_PARM(OscDownTm,      float);
CREATE_PUBVAR(OscTmr,       float);
CREATE_PUBVAR(OscOut,       float);
void  ConstantValue()           {;}
void  TestOscillator()          {;}
int BufferDump (cParamString& OutputBuffer, char* pBuf, int DataLen)  {return SUCCESS;}


// Version
Code_ID_struct Code_ID = {
    "MarkStat",
    0x0010302E,
    0x020140807,
    0x00151254 };
