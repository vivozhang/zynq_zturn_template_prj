#include "x0FrameWork.h"
#include "Product.h"




///////////////////////////////////////////////////////////////////////////////
// STUBBED VARIABLE DEFINITIONS
//
///////////////////////////////////////////////////////////////////////////////

CREATE_PUBVAR(VarReqEgd   ,  unsigned );

CREATE_PUBVAR(T1ExecCnt    , int );                                                  // x0Xops.a30
CREATE_PUBVAR(T2ExecCnt    , int );                                                  // x0Xops.a30
CREATE_PUBVAR(T3ExecCnt    , int );                                                  // x0Xops.a30
CREATE_PUBVAR(WEMADisabSim , int )= true;                                           // p0Bppb.c30 - Status of WEMA hardware for simulator purposes
void RotorReportDataBuffers(char *Title)        {;}                 // p1Atune.c30
int  RotorStartRecorder(Rec_Ctl_Type Rec_Ctl)   {return SUCCESS;}   // p1AtTsk1.c30
int  RotorTuneupRecorder(void)                  {return SUCCESS;}   // p1AtTsk1.c30
void RotorWait(float Delay_Tm)                  {;}                 // p1Atune.c30
void Task0(void)                                {;}                 // x0Xops.cpp why is xops using a Task0 ???
void TimePLL(void)                              {;}                 // p0TmSync.c30

// x0fault.c30
CREATE_PUBVAR(ClrFltT2      ,   unsigned ) ;   // set to start t2 flt buffer clear
CREATE_PUBVAR(ClrFpgaCtr    ,   unsigned ) ;   // State Counter for Asic Flt Reset
CREATE_PUBVAR(FpgaFltRstB1  ,   unsigned ) ;   // Bridge0 Fault Reset Command Bool
CREATE_PUBVAR(FpgaFltRstB2  ,   unsigned ) ;   // Line Fault Reset Command Bool


// x0Status.c30
 CtlStsDat CSDat;         // Control Status Data structure allocation


// p0Task2.c30
DiagBits_Typ DiagBits;

//p1Atune.c30

enum        Data_State_Type Data_State;     // state of data collection

// HW address in x0Asic.h
unsigned Dummy2;
unsigned * DSP_HCF_ADDR   = & Dummy2   ;
unsigned * DSP_HIF_ADDR   = & Dummy2   ;
unsigned * DSP_TTMR1_ADDR = & Dummy2   ;
unsigned * DSP_CSR_ADDR   = & Dummy2   ;
unsigned * DSP_DIO_ADDR   = & Dummy2   ;

// x0init.c30

// p0Task1.c30
CREATE_PUBVAR(FpllFrqAdj,  float) =1.0;      // Fpll Task Frequency Adjust - Task 1

// x0EPLD.h
unsigned * EPLD_CMD_ADDR  = & Dummy2   ;
//x0EPLD.c30
EpldCmdRgr  *EpldCmdP  = (EpldCmdRgr*)EPLD_CMD_ADDR;

// x0fault.c30         Used by y1fltck.cpp and y2FltCk.cpp set if bic faulted. cleared upon fault reset.
CREATE_PUBVAR(BicFltStat,           unsigned);


// xNvVar.h
nvVar_tbl  NV;
