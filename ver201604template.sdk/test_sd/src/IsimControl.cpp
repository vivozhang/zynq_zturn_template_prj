///////////////////////////////////////////////////////////////////////////////
// TITLE: IsimControl.cpp
//
// DESCRIPTION:
//     ControlDLL Wrapper, for interfacing MarkStatDLL in IsimMode
//     developed from MarkStatDLL_Tester.cpp & Control.cpp
//
/// FLOW:
//     ContractToMarkStat, ContractFrom MarkStat
//
// COPYRIGHT:   Copyright (c) 2015 by GENERAL ELECTRIC COMPANY U.S.A.
///////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include<iostream>

#include "xscugic.h"
#include "xil_exception.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xsdps.h"
#include "xil_printf.h"

//#include "xtmrctr.h"
#include "ff.h"
#include "xil_cache.h"





#include <tchar.h>
#include "IsimControl.h"
#include "MarkStatDLL.h"
#include "data.h"
using namespace MarkStatDLL;





// Parameter Definitions
#define INTC_DEVICE_ID 			XPAR_PS7_SCUGIC_0_DEVICE_ID
#define BTNS_DEVICE_ID			XPAR_AXI_GPIO_0_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID  XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define BTN_INT 				XGPIO_IR_CH1_MASK // This is the interrupt mask for channel one
#define TMR_DEVICE_ID			XPAR_TMRCTR_0_DEVICE_ID
#define INTC_TMR_INTERRUPT_ID 	XPAR_FABRIC_AXI_TIMER_0_INTERRUPT_INTR
/* IMPORTANT NOTE: The AXI timer frequency is set to 50Mhz
 * the timer is set up to be counting UP, these two facts affect the value selected for
 * the load register to generate 1 Hz interrupts
 */
#define TMR_LOAD				0xFD050F7F


XGpio   BTNInst;
XScuGic INTCInst;
//XTmrCtr TMRInst;

//Lab7 addition start
static FATFS FS_instance; // File System instance
static FIL file1;		// File instance
FRESULT result;			// FRESULT variable
static char FileName[32] = "RECORDS2.txt"; // name of the log
static char *Log_File; // pointer to the log
char *Path = "0:/";  //  string pointer to the logical drive number
int counter_logger=0; // Counter for the push button
unsigned int BytesWr; // Bytes written
int len=0;			// length of the string
int accum=0;		//  variable holding the EOF
char Buffer_logger[64] __attribute__ ((aligned(32))); // Buffer should be word aligned (multiple of 4)
u32 Buffer_size = 64;






void*           pContract=NULL;
void*           pContractToMarkStatSetParm=NULL;
extern  void*   pContractTrender;
extern  void*   MakeContractTrender(void);

unsigned        StepCntr    =      0;
unsigned        StepLimit   =      0;
unsigned        IsimEventTmMsec =      0;
unsigned        IsimEventTmSec  =      0;

float           EventTime   =   0.00F;
float           EventLimit  =  750.00F;

float           RTDelta     =   0.05F;
float           RTNext1     =   0.20F;

float           RTDeltaF    =   00.001F;
float           RTMark1     =   1000.0F;
float           RTMark2     =   1000.1F;

//=============================================================================
// Set Parameters ToMarkStat
//-----------------------------------------------------------------------------
float       fZero    = 0.0F;

namespace IsimUserParms
{
    float       BelowOn = 0.0F;// normally < 0.200 means "TRUE"
    float       AboveOff = 0.0F;// normally > 0.144 means "FALSE"
    unsigned Net_Type = 0;
    unsigned SimModeReq = 1;
    unsigned XSimModeReq = 0;
    unsigned DsimModeReq = 0;
    int SoftwareIPN = 10;
    int GenTypeCfg = 19;
    int CableConfig = 80;
    int GmCfgSel = 5;
    float WBAclfuILim1 = 580.052;
    float LclSpdRefSim = 1200.0;
    float L_FrqInput = 60;
    float PwrLimSpdRatLcl = 1800;
    float TurbNpRatLcl = 2330;
}
//------------------------------------
ContractListType ListToMarkStatSetParm[] = {
    {"ThrmOpenOnThrsh" , &IsimUserParms::BelowOn },    // SET TRUE  if below this Threshold
    {"ThrmOpenOffThrsh", &IsimUserParms::AboveOff},    // CLR FALSE if above this Threshold
    { "Net_Type", &IsimUserParms::Net_Type },
    { "SimModeReq", &IsimUserParms::SimModeReq },
    { "XSimModeReq", &IsimUserParms::XSimModeReq },
    { "DsimModeReq", &IsimUserParms::DsimModeReq },
    { "SoftwareIPN", &IsimUserParms::SoftwareIPN },
    { "GenTypeCfg", &IsimUserParms::GenTypeCfg },
    { "CableConfig", &IsimUserParms::CableConfig },
    { "GmCfgSel", &IsimUserParms::GmCfgSel },
    { "WBAclfuILim1", &IsimUserParms::WBAclfuILim1 },
    { "LclSpdRefSim", &IsimUserParms::LclSpdRefSim },
    { "L_FrqInput", &IsimUserParms::L_FrqInput },
    { "PwrLimSpdRatLcl", &IsimUserParms::PwrLimSpdRatLcl },
    { "TurbNpRatLcl", &IsimUserParms::TurbNpRatLcl },
};
//------------------
void* MakeContractToMarkStatSetParm(void){
    void*   pContract = NULL;
    printf("  MakeContract ToMarkStatSetParm...");
    pContract = MarkStatDLLApi::CreateContract(
                sizeof(ListToMarkStatSetParm)/
                sizeof(ListToMarkStatSetParm[0]),
                       ListToMarkStatSetParm, WRITE_PARM );
    return (pContract);
}
//-----------------------------------------------------------------------------

//=============================================================================
// Initial Values FromMarkStat
//-----------------------------------------------------------------------------
void*   pContractFromMarkStatInitVar;
void*   pContractFromMarkStatInitParm;

float       IsimExeDelTm1, IsimExeDelTm1A;
int         IsimSimDevID;
float       IsimTrqRat;
unsigned    IsimT1AExecMask;
unsigned    IsimSimMode  = 2;
unsigned    IsimXSimMode = 2;
unsigned    IsimDsimMode = 2;
//------------------
ContractListType ListFromMarkStatInitVar[] = {
    {"DelTm1",       &IsimExeDelTm1       },
    {"DelTm1A",      &IsimExeDelTm1A      },
    {"SimDevID",     &IsimSimDevID     },
    {"R_TrqRat",     &IsimTrqRat       },
    {"SimMode",      &IsimSimMode      },
    {"XSimMode",     &IsimXSimMode     },
    {"DsimMode",     &IsimDsimMode     },
};
ContractListType ListFromMarkStatInitParm[] = {
    {"T1AExecMask",  &IsimT1AExecMask  },
};
//------------------
void* MakeContractFromMarkStatInitVar(void){
    void*   pContract;
    printf("  MakeContract FromMarkStatInitVar.....");
    pContract  = MarkStatDLLApi::CreateContract(
                 sizeof(ListFromMarkStatInitVar)/
                 sizeof(ListFromMarkStatInitVar[0]),
                        ListFromMarkStatInitVar, READ_VAR );
    return (pContract);
}
void* MakeContractFromMarkStatInitParm(void){
    void*   pContract;
    printf("  MakeContract FromMarkStatInitParm....");
    pContract = MarkStatDLLApi::CreateContract(
                sizeof(ListFromMarkStatInitParm)/
                sizeof(ListFromMarkStatInitParm[0]),
                       ListFromMarkStatInitParm, READ_PARM );
    return (pContract);
}
//-----------------------------------------------------------------------------


//=============================================================================
// Check Parm FromMarkStat
//-----------------------------------------------------------------------------
void*       pContractFromMarkStatCheckParm;

unsigned    MonNetType      = 2;
unsigned    MonSimModeReq   = 2;
unsigned    MonXSimModeReq  = 2;
unsigned    MonDsimModeReq  = 2;
//------------------
ContractListType ListFromMarkStatCheckParm[] = {
    {"Net_Type"     , &MonNetType},
    {"SimModeReq"   , &MonSimModeReq},
    {"XSimModeReq"  , &MonXSimModeReq},
    {"DsimModeReq"  , &MonDsimModeReq},
};
//------------------
void* MakeContractFromMarkStatCheckParm(void){
    void*   pContract;
    printf("  MakeContract FromMarkStatCheckParm...");
    pContract = MarkStatDLLApi::CreateContract(
                sizeof(ListFromMarkStatCheckParm)/
                sizeof(ListFromMarkStatCheckParm[0]),
                       ListFromMarkStatCheckParm, READ_PARM );
    return (pContract);
}
//-----------------------------------------------------------------------------

//=============================================================================
// System Feedbacks FromMarkStat
//-----------------------------------------------------------------------------
void*       pContractFromMarkStatSysFbk;

unsigned    IsimTripFault    = 0;
unsigned    IsimRunReady     = 0;
unsigned    IsimRunning      = 0;

unsigned    SysTrpFlt    = 0;
unsigned    SysRunRdy    = 0;
unsigned    SysRunning   = 0;

float       SysTrpFltTm  = 0.0F;
float       SysRunRdyTm  = 0.0F;
float       SysRunningTm = 0.0F;
//------------------
ContractListType ListFromMarkStatSysFbk[] = {
    {"TripFlt"          , &IsimTripFault},
    {"RunRdy"           , &IsimRunReady},
    {"Running"          , &IsimRunning},
};
//------------------
void* MakeContractFromMarkStatSysFbk(void){
    void*   pContract;
    printf("  MakeContract FromMarkStatSysFbk...");
    pContract = MarkStatDLLApi::CreateContract(
                sizeof(ListFromMarkStatSysFbk)/
                sizeof(ListFromMarkStatSysFbk[0]),
                       ListFromMarkStatSysFbk, READ_VAR );
    return (pContract);
}
//-----------------------------------------------------------------------------

//=============================================================================
// System Commands ToMarkStat
//-----------------------------------------------------------------------------
void*       pContractToMarkStatSysCmd;

unsigned    SysOvdTmp   = 0;
unsigned    SysRunReq   = 0;
unsigned    ParmDump    = 0;

float       SysOvdTmpTm = 0.0F;
float       SysRunReqTm = 0.0F;
//------------------
ContractListType ListToMarkStatSysCmd[] = {
 // {"OvrdTempRunRdy"   , &SysOvdTmp},
    {"RunReq"           , &SysRunReq},
};
//------------------
void* MakeContractToMarkStatSysCmd(void){
    void*   pContract = NULL;
    printf("  MakeContract ToMarkStatSysCmd...");
    pContract = MarkStatDLLApi::CreateContract(
                sizeof(ListToMarkStatSysCmd)/
                sizeof(ListToMarkStatSysCmd[0]),
                       ListToMarkStatSysCmd, WRITE_VAR );
    return (pContract);
}
//------------------
float SysSpdRef;
float SysTrqRef;
float SysVarRef;
void*   pContractToMarkStatSysParmCmd;
ContractListType ListToMarkStatSysParmCmd[] = {
	{"LclSpdRefSim",  	&SysSpdRef},
	{"LclTrqRef",		&SysTrqRef},
	{"LclVarRef",		&SysVarRef},
};
void* MakeContractToMarkStatSysParmCmd(void){
    void*   pContract = NULL;
    printf("  MakeContract ToMarkStatSysParmCmd...");
    pContract = MarkStatDLLApi::CreateContract(
                sizeof(ListToMarkStatSysParmCmd)/
                sizeof(ListToMarkStatSysParmCmd[0]),
                       ListToMarkStatSysParmCmd, WRITE_PARM );
    return (pContract);
}
//-----------------------------------------------------------------------------

//=============================================================================
//  Iteration Values FromMarkStat ToSaber
//-----------------------------------------------------------------------------
void*       pContractFromMarkStatIter;

int         IsimT1St         ;  // T1  state within T2 frame, publ ver of task1 frame cntr
int         IsimT2St         ;  // T2  state within T3 frame, publ ver of task2 frame cntr
int         IsimT3St         ;  // T3  state counter        , publ ver of task3 frame cntr
int         IsimT1T2St       ;  // T1  state within T3 frame
int         IsimT1ASt        ;  // T1A state within T2 frame
int         IsimT1AT2St      ;  // T1A state within T3 frame
unsigned    IsimTask1AExecReq;  // flag to request Task1A execution

//------------------
ContractListType ListFromMarkStatIter[] = {
    {"EventTmSec"       , &IsimEventTmSec},
    {"EventTmMsec"      , &IsimEventTmMsec},
    //
    {"T1St"              , &IsimT1St         },
    {"T2St"              , &IsimT2St         },
    {"T3St"              , &IsimT3St         },
    {"T1T2St"            , &IsimT1T2St       },
    {"T1ASt"             , &IsimT1ASt        },
    {"T1AT2St"           , &IsimT1AT2St      },
    {"Task1AExecReq"     , &IsimTask1AExecReq},
};
//------------------
void* MakeContractFromMarkStatIter(void){
    void*   pContract;
    printf("  MakeContract FromMarkStatIter........");
    pContract = MarkStatDLLApi::CreateContract(
                sizeof(ListFromMarkStatIter)/
                sizeof(ListFromMarkStatIter[0]),
                       ListFromMarkStatIter, READ_VAR );
    return (pContract);
}
//-----------------------------------------------------------------------------

//*****************************************************************************
int main(int argc, char* argv[])
{
    // Report Versions
    // ---------------
    printf("\n");
    printf("WindDFIG02  EXE Isim   \n");
    printf("---------------------- \n");
    printf("ISIM Cntrl  V00.00.06  \n");
    printf("Product% s\n", MarkStatDLLApi::GetApiRevision());
    printf("%s\n", MarkStatDLLApi::GetProductRevision());
    printf("---------------------- \n");

    // Initialize MarkStatDLL
    // ----------------------
    MarkStatDLLApi::Initialize("");
    printf("\n");
/*
    // Create MarkStat Contracts
    // -------------------------
    printf("\n");
    printf("Create MarkStat Contracts \n");
    // -------------------------------------------
        pContractToMarkStatSetParm       //  0
    =MakeContractToMarkStatSetParm();
    if (pContractToMarkStatSetParm == NULL)
        {printf("....FAILED. \n"); return(0);      }
    else{printf("....SUCCESS.\n");                 }
     // -------------------------------------------
        pContractFromMarkStatInitParm       //  1
    =MakeContractFromMarkStatInitParm();
    if (pContractFromMarkStatInitParm == NULL)
        {printf("FAILED. \n"); return(0);      }
    else{printf("SUCCESS.\n");                 }
    // -------------------------------------------
        pContractFromMarkStatInitVar        //  2
    =MakeContractFromMarkStatInitVar();
    if (pContractFromMarkStatInitVar  == NULL)
        {printf("FAILED. \n"); return(0);      }
    else{printf("SUCCESS.\n");                 }
    // -------------------------------------------
        pContractFromMarkStatCheckParm      //  3
    =MakeContractFromMarkStatCheckParm();
    if (pContractFromMarkStatCheckParm== NULL)
        {printf("FAILED. \n"); return(0);      }
    else{printf("SUCCESS.\n");                 }
    // -------------------------------------------
        pContractToMarkStatSysCmd           //  4
    =MakeContractToMarkStatSysCmd();
    if (pContractToMarkStatSysCmd== NULL)
        {printf(".....FAILED. \n"); return(0); }
    else{printf(".....SUCCESS.\n");            }
    // -------------------------------------------
        pContractFromMarkStatSysFbk         //  5
    =MakeContractFromMarkStatSysFbk();
    if (pContractFromMarkStatSysFbk== NULL)
        {printf("...FAILED. \n"); return(0);   }
    else{printf("...SUCCESS.\n");              }
    // -------------------------------------------
        pContractFromMarkStatIter           //  6
    =MakeContractFromMarkStatIter();
    if (pContractFromMarkStatIter== NULL)
        {printf("FAILED. \n"); return(0);      }
    else{printf("SUCCESS.\n");                 }
    // -------------------------------------------
        pContractTrender                    //  7
    =MakeContractTrender();
    if (pContractTrender ==NULL)
        {printf("...FAILED. \n"); return(0);   }
    else{printf("...SUCCESS.\n");              }
	// -------------------------------------------
        pContractToMarkStatSysParmCmd   	//  8
    =MakeContractToMarkStatSysParmCmd();
    if (pContractToMarkStatSysParmCmd ==NULL)
        {printf("...FAILED. \n"); return(0);   }
    else{printf("...SUCCESS.\n");              }
    // -----------------------------------------------------

    // Execute MarkStat Contracts & RunRules
    // -------------------------------------
    MarkStatDLLApi::ExecContract(pContractToMarkStatSetParm);       // [ 0 ]
    MarkStatDLLApi::RunRule();
    MarkStatDLLApi::ExecContract(pContractFromMarkStatInitParm);    // [ 1 ]
    MarkStatDLLApi::ExecContract(pContractFromMarkStatInitVar);     // [ 2 ]
    MarkStatDLLApi::ExecContract(pContractFromMarkStatCheckParm);   // [ 3 ]

    printf("\n");
    printf("SimDevID : %d", IsimSimDevID   );
    printf(", TrqRat= %f" , IsimTrqRat     );
    printf(", NetTyp= %d" , MonNetType );
    printf("\n");
    printf("RulesRun :");
    printf(" DT1 = %f", IsimExeDelTm1     );
    printf(", DT1A= %f", IsimExeDelTm1A    );
    printf(", T1A = %d", IsimT1AExecMask);
    printf("\n");
    printf("          ");
    printf(" SimModeReq =%d", MonSimModeReq );
    printf(", XSimModeReq=%d", MonXSimModeReq);
    printf(", DsimModeReq=%d", MonDsimModeReq);
    printf("\n");
    printf("          ");
    printf(" SimMode    =%d", IsimSimMode       );
    printf(", XSimMode   =%d", IsimXSimMode      );
    printf(", DsimMode   =%d", IsimDsimMode      );
    printf("\n");
    printf("========================================================= \n\n");

    */
    // Iteration Loop
    // --------------
    while ( EventTime < EventLimit )
    {
        // System Events
        // -------------
        if ( (SysOvdTmp==0)&&(EventTime >= 0.3F) ){
            SysOvdTmp = 1;
            printf("\n[[[ SysOverrideTemp @ %f ]]]\n\n", EventTime);
            SysOvdTmpTm=EventTime;
        }
        if ( (SysTrpFlt==0)&&(IsimTripFault==1) ){
            SysTrpFlt = 1;  SysRunRdy = 0;  SysRunReq = 0;
            printf("\n[[[ SysTripFault    @ %f ]]]\n\n", EventTime);
            SysTrpFltTm=EventTime;
        }
        if ( (SysRunRdy==0)&&(IsimRunReady==1)&&(SysTrpFlt==0) ){
            SysRunRdy = 1;
            printf("\n[[[ SysRunReady     @ %f ]]]\n\n", EventTime);
            SysRunRdyTm=EventTime;
        }
        if ( (SysRunReq==0)&&(SysRunRdy==1)&&(EventTime>=(SysRunRdyTm+0.2)) ){
            SysRunReq   = 1;
            printf("\n[[[ SysRequestRun   @ %f ]]]\n\n", EventTime);
            SysRunReqTm=EventTime;
        }
        if ( (SysRunning==0)&&(IsimRunning==1) ){
            SysRunning = 1;
            printf("\n[[[ SysRunning!!!   @ %f ]]]\n\n", EventTime);
            SysRunningTm=EventTime;
        }

        // Execute MarkStat Step
        // ---------------------
        StepCntr += 1;
        MarkStatDLLApi::ExecContract(pContractToMarkStatSysCmd);    // [ 4 ]
		if(SysRunning==1)
		{
			static int CmdTrenderIndx = 0;
			while(CmdTrender[CmdTrenderIndx].time+SysRunningTm+10<EventTime)
			{
				CmdTrenderIndx++;
				if(CmdTrenderIndx>=sizeof(CmdTrender)/sizeof(CmdType))
				{
					EventLimit=EventTime;
				}
				else
				{
					SysSpdRef=CmdTrender[CmdTrenderIndx].rpm/60*6.28;
					SysTrqRef=CmdTrender[CmdTrenderIndx].trq/1000;
					SysVarRef=CmdTrender[CmdTrenderIndx].var;
				}
			}
			MarkStatDLLApi::ExecContract(pContractToMarkStatSysParmCmd);
		}
        MarkStatDLLApi::Step();
        MarkStatDLLApi::ExecContract(pContractFromMarkStatSysFbk);  // [ 5 ]
        MarkStatDLLApi::ExecContract(pContractFromMarkStatIter);    // [ 6 ]

        // EventTime
        // ---------
        EventTime = (float)( (float)IsimEventTmSec + 0.001*(float)IsimEventTmMsec );

        // Debug Monitor
        // -------------
        if ( (EventTime>=RTNext1) )
        {
            if ( (EventTime>=RTMark1)&&(EventTime<=RTMark2))
            {
                  RTNext1  += RTDeltaF;
                  MarkStatDLLApi::ExecContract(pContractTrender);         // [ 7 ]
            }
            else
            {
				static float EventTimeZ=0.0F;
                if ( (IsimT2St==0)&&(IsimT1St==0)&&(IsimT3St==0) )
                {
                  RTNext1 += 0.001;
                  MarkStatDLLApi::ExecContract(pContractTrender);         // [ 7 ]
                }
				if(EventTime-EventTimeZ>1)
				{
					EventTimeZ=EventTime;
					printf("EventTm= %f \n", EventTime );
				}
            }
        }
    }

    // Dump Parameters
    // ---------------
    printf("\n\n[[[ DumpParameters  @ %f ]]]\n\n", EventTime);
    MarkStatDLLApi::DumpParameters();


    return (1);
} // end
//*****************************************************************************
