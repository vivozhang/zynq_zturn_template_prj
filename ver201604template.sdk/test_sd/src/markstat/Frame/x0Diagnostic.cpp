///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Diagnostic interface
//
// DESCRIPTION:
//              provide interface and keep track of active diagnostics,
//
// COPYRIGHT:   Copyright (c) 2002
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files:
//---------------
#include <string.h>

#include <stdlib.h>
#include <errno.h>  //vivo: add
// core

// local
#include "x0FrameWork.h"
#include "x0Diagnostic.h"
#include "x0err.h"

#ifndef PRE_PROC
    #define  DIAG_STATIC_INIT
    #include "DiagExtern.h"
    #undef   DIAG_STATIC_INIT
#endif


// Constants:
//-----------


// Externals:
//-----------
extern cSysMemConfigClient  gSysMemConfigClient;
extern unsigned             InitDone;
extern pthread_mutex_t      TimeSyncMutex;
extern SysTm_Typ            EventTm1;
extern unsigned             ApplInterfaceOk;
extern unsigned             ApplFltRstReq;
extern void                 SyncTimeInit(void);
extern unsigned             DsimFaultCode;
extern unsigned             DsimMode;


// Types and Classes:
//-------------------


// Function Prototypes:
//---------------------


// Variables:
//-----------
int     DummyDiag;

CREATE_PUBVAR(NoFlt, unsigned) = true;              // No Fault flag

CREATE_PUBVAR(TripFlt, unsigned);                   // Trip Fault flag
CREATE_PUBVAR(NoTripFlt, unsigned) = true;          // No Trip Fault flag

CREATE_PUBVAR(Brg1NoFlt, unsigned) = true;          // No Fault for bridge 1
CREATE_PUBVAR(Brg1TripFlt, unsigned);               // Bridge1 trip fault

CREATE_PUBVAR(Brg2NoFlt, unsigned) = true;          // No Fault for bridge 2
CREATE_PUBVAR(Brg2TripFlt, unsigned);               // Bridge2 trip fault

CREATE_PUBVAR(DiagResetTaskReq, unsigned);          // reset diagnostic has occured semaphore to Xops

CREATE_PUBVAR(TripTypeWord1,    unsigned) = 0;      // Trip fault type word 1
CREATE_PUBVAR(TripTypeWord2,    unsigned) = 0;      // Trip fault type word 2
CREATE_PUBVAR(TripTypeWord3,    unsigned) = 0;      // Trip fault type word 3
CREATE_PUBVAR(TripTypeWord4,    unsigned) = 0;      // Trip fault type word 4
CREATE_PUBVAR(TripTypeWord5,    unsigned) = 0;      // Trip fault type word 5
CREATE_PUBVAR(TripWordCnt,      unsigned) = 0;      // Trip Word Counter
CREATE_PUBVAR(TripLostWordCnt,  unsigned) = 0;      // Trip Lost Word Counter

cMarkStatDiag* FirstDiagInstance=NULL;              // remember the first instance to be used in LLM commands


// Parameters:
//------------
CREATE_PARM(FltRstAdr, unsigned*);                  // Remote diagnostic reset pointer


// Diagnostics:
//------------
CREATE_DIAG(TripErrorLogged);                       // error message logged and trip fault pushed
CREATE_DIAG(AlarmErrorLogged);                      // error message logged and alarm fault pushed

CREATE_DIAG(TestB1Alm);                             // test diagnostics
CREATE_DIAG(TestB1Trip);
CREATE_DIAG(TestB2Alm);
CREATE_DIAG(TestB2Trip);
CREATE_DIAG(TestAlarm);
CREATE_DIAG(TestAlarmLock);
CREATE_DIAG(TestTrip);
CREATE_DIAG(TestTripLock);
CREATE_DIAG(TestSoe);


// Diagnostic address cross reference table
//----------------------------------------
#undef  DIAG_ENTRY
#define DIAG_ENTRY(Class, OrigClass, BaseName, PubName, Cparm, Uniquifier, Bridge)  &Cparm,
void* cMarkStatDiag::DiagAddrTbl[] = {
    #ifndef PRE_PROC
        #include "Diagnostic.def"
    #endif
};


// Diagnostic attribute table
//----------------------------------------
#ifndef PRE_PROC
    #define  DIAG_DEFINITIONS
    #include "DiagAttrib.def"
    #undef   DIAG_DEFINITIONS
#endif
uint32_t cMarkStatDiag::DiagAttribTbl[] =
{
    #ifndef PRE_PROC
        #define  DIAG_ATTRIBUTES
        #include "DiagAttrib.def"
        #undef   DIAG_ATTRIBUTES
    #endif
};


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Constructor for the diagnostic class
//      This function initalizes all data members and create necessary memory buffers
//
// EXECUTION LEVEL
//      Class instancing
//
// RETURN VALUE
//
///////////////////////////////////////////////////////////////////////////////
cMarkStatDiag::cMarkStatDiag() : cDiagMessenger(1)
{
    unsigned        Idx;
    int             NullPtrFound;

    if (FirstDiagInstance == NULL)
        FirstDiagInstance = this;

    BaseDiagNum   = 2400;
    MaxDiagNum    = (sizeof(DiagAttribTbl) / sizeof(DiagAttribTbl[0])) - 1 + BaseDiagNum;
    DiagWrdCnt    = (((sizeof(DiagAttribTbl) / sizeof(DiagAttribTbl[0])) + 31) / 32);
    ActCollectCnt = 4;  // sets delay for setting diag inactive after stop pushing: MIN = (cnt-1)*(background loop time)
    ActCollectIdx = 0;
    ResetPending  = false;
    InvalidDiag   = 0;
    ControllerId  = ID_UNKNOWN;
    SoeIndex      = 1;

    // look for NULL pointers
    // populate diagnostic numbers
    NullPtrFound = false;
    DiagTblSize  = (sizeof(DiagAddrTbl)/sizeof(DiagAddrTbl[0]));
    for (Idx = 0; Idx < (sizeof(DiagAddrTbl)/sizeof(DiagAddrTbl[0])); ++Idx)
    {
        if (DiagAddrTbl[Idx] == NULL)
        {
            DiagAddrTbl[Idx] = &DummyDiag;
            NullPtrFound     = true;
        }
        else
            *(unsigned*)DiagAddrTbl[Idx] = BaseDiagNum + Idx;
    }

    if (NullPtrFound)
    {
        LogErrorMessage(TRIP_MSG, cParamString(MKS_DIAG_STR,
            cParamString("NULL pointer found in Diagnostic address cross reference table").c_str()),
            HERE);
    }

    // create active diagnostic collection array
    if((pDiagActCollector  = (uint32_t**)malloc(ActCollectCnt*sizeof(uint32_t*))) == NULL)
    {
        LogErrorMessage(TRIP_MSG, cParamString(MKS_DIAG_STR,
            cParamString("Failed to malloc memory for active Diagnostic array (1), (%d,%s)", errno, strerror(errno)).c_str()),
            HERE);
        return;
    }
    for (Idx=0; Idx < ActCollectCnt; ++Idx)
    {
        if((pDiagActCollector[Idx]  = (uint32_t*)malloc(DiagWrdCnt*sizeof(uint32_t))) == NULL)
        {
            LogErrorMessage(TRIP_MSG, cParamString(MKS_DIAG_STR,
                cParamString("Failed to malloc memory for active Diagnostic array (2), (%d,%s)", errno, strerror(errno)).c_str()),
                HERE);
            return;
        }
        memset(pDiagActCollector[Idx], 0, DiagWrdCnt*sizeof(uint32_t));
    }

    // create active diagnostic summation
    if ((pDiagActive  = (uint32_t*)malloc(DiagWrdCnt*sizeof(uint32_t))) == NULL)
    {
        LogErrorMessage(TRIP_MSG, cParamString(MKS_DIAG_STR,
            cParamString("Failed to malloc memory for active Diagnostic summation, (%d,%s)", errno, strerror(errno)).c_str()),
            HERE);
        return;
    }
    memset(pDiagActive, 0, DiagWrdCnt*sizeof(uint32_t));

    // create active SOE summation
    if ((pSoeActive  = (uint32_t*)malloc(DiagWrdCnt*sizeof(uint32_t))) == NULL)
    {
        LogErrorMessage(TRIP_MSG, cParamString(MKS_DIAG_STR,
            cParamString("Failed to malloc memory for active SOE summation, (%d,%s)", errno, strerror(errno)).c_str()),
            HERE);
        return;
    }
    memset(pSoeActive, 0, DiagWrdCnt*sizeof(uint32_t));

    // create inactive SOE summation
    if ((pSoeInActive  = (uint32_t*)malloc(DiagWrdCnt*sizeof(uint32_t))) == NULL)
    {
        LogErrorMessage(TRIP_MSG, cParamString(MKS_DIAG_STR,
            cParamString("Failed to malloc memory for inactive SOE summation, (%d,%s)", errno, strerror(errno)).c_str()),
            HERE);
        return;
    }
    memset(pSoeInActive, 0, DiagWrdCnt*sizeof(uint32_t));

    pthread_mutex_init(&DiagIdxMutex, NULL);
    pthread_mutex_init(&DiagActiveMutex, NULL);
    pthread_mutex_init(&SoeActiveMutex, NULL);

    // initialize SOE
    if (SoeLogClient.Open() != 0)
    {
        LogErrorMessage(TRIP_MSG, cParamString(MKS_DIAG_STR,
            cParamString("Unable to open SoeLogClient").c_str()),
            HERE);
        return;
    }

    SyncTimeInit();

    Initialized = true;

    // push error message diagnostics logged before initialization
    if (LogTripError)
        PUSH_DIAG(TripErrorLogged);
    else if (LogAlarmError)
        PUSH_DIAG(AlarmErrorLogged);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class destructor to free used resources
//
// EXECUTION LEVEL
//      Instance destruction
//
// RETURN VALUE:
//
///////////////////////////////////////////////////////////////////////////////
cMarkStatDiag::~cMarkStatDiag()
{
    unsigned        Idx;

    for (Idx=0; Idx < ActCollectCnt; ++Idx)
        if (pDiagActCollector[Idx] != NULL)
            free(pDiagActCollector[Idx]);
    if (pDiagActCollector != NULL)
    {
        free(pDiagActCollector);
        pDiagActCollector = NULL;
    }

    if (pDiagActive != NULL)
    {
        free(pDiagActive);
        pDiagActive = NULL;
    }

    if (pSoeActive != NULL)
    {
        free(pSoeActive);
        pSoeActive = NULL;
    }

    if (pSoeInActive != NULL)
    {
        free(pSoeInActive);
        pSoeInActive = NULL;
    }

    pthread_mutex_destroy(&DiagIdxMutex);
    pthread_mutex_destroy(&DiagActiveMutex);
    pthread_mutex_destroy(&SoeActiveMutex);

    SoeLogClient.Close();
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      initialize
//      add diagnostic LLM commands
//
// EXECUTION LEVEL
//      initialization
//
// RETURN VALUE:
//      EOK on success, -1 on failure
//
///////////////////////////////////////////////////////////////////////////////
int     cMarkStatDiag::Initialize (cMonitorCommand &gMonitor)
{
    cParamString HelpString;

    ControllerId  = gSysMemConfigClient.GetControllerId();

    HelpString =  "MarkStatDiag\n";
    HelpString += "-r Reset Diagnostic alarms\n";
    HelpString += "-p# Push Diagnostic alarm  (2400..?xx?)\n";
    if (gMonitor.Add("MarkStatDiag", HelpString, DiagStatus, NORMAL_OUTPUT_SIZE, "diag") != EOK)
        return -1;

    return EOK;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    for diagnostic:
//      send a message to the diagnostic queue
//      add to the active diagnostic list
//      clear NoFault and set TripFault booleans
//
//    for SOE
//      add to SOE transition queue in shared memory, to be sent to the list of subscribers
//      add to the active SOE list
//
// EXECUTION LEVEL
//
//
// RETURN VALUE:
//      N/A
//
///////////////////////////////////////////////////////////////////////////////
void    cMarkStatDiag::PushDiagnostic(unsigned DiagCode)
{
    sTimeType    EventTm;
    int         DiagIdx;
    unsigned    WrdIdx, CatNum;
    uint32_t    TestBit, CatNumMask = 0x0000FFFF;

    if (!Initialized)
    {
        cDiagMessenger::LogErrorMessage(ALARM_MSG,
            cParamString("Pushed diagnostic %d before class initialized", DiagCode).c_str(),
            HERE);
        return;
    }

    if ( (DiagCode < BaseDiagNum) || (DiagCode > MaxDiagNum) )
    {
        if (InvalidDiag == 0)  InvalidDiag = DiagCode;
        return;
    }

    DiagIdx = DiagCode - BaseDiagNum;

    // check for diagnostic init attribute
    if ( InitDone || ((DiagAttribTbl[DiagIdx] & ATTRIB_INIT) != 0) )
    {
        WrdIdx  = DiagIdx >> 5;
        TestBit = 1 << (DiagIdx & 0x1F);
        if ((DiagAttribTbl[DiagIdx] & ATTRIB_SOE) != 0)
        {
            // set SOE active
            if ((pSoeActive[WrdIdx] & TestBit) == 0)
            {
                if ( pthread_mutex_lock(&TimeSyncMutex) == EOK )
                {
                    EventTm.tv_sec  = EventTm1.Sec;
                    EventTm.tv_nsec = EventTm1.nSec;
                    pthread_mutex_unlock(&TimeSyncMutex);
                }
                pthread_mutex_lock(&SoeActiveMutex);        // protecting Active/InActive SOE array while changing bits and Logging SOEs
                LogSoe(DiagCode, true, EventTm);
                pSoeActive[WrdIdx]   |= TestBit;            // set Active flag to block subsequent pushes
                pSoeInActive[WrdIdx] &= ~TestBit;           // clear InActive flag to allow next pop
                pthread_mutex_unlock(&SoeActiveMutex);
            }
        }
        else if ((pDiagActive[WrdIdx] & TestBit) == 0)
        {
            // set diagnostic active
            if ( pthread_mutex_lock(&TimeSyncMutex) == EOK )
            {
                EventTm.tv_sec  = EventTm1.Sec;
                EventTm.tv_nsec = EventTm1.nSec;
                pthread_mutex_unlock(&TimeSyncMutex);
            }
            pthread_mutex_lock(&DiagActiveMutex);          // protecting Active Diagnostic array while changing bits and Logging Diagnostics
            LogAlarmMessage(DiagCode, DiagCode,
                            DIAG_ALARM_RECURRING, DIAG_ALARM_HIGH,
                            0, 0, 0, 0,
                            true,
                            EventTm);
            pDiagActive[WrdIdx] |= TestBit;
            pthread_mutex_unlock(&DiagActiveMutex);

            // set fault flags
            if ((DiagAttribTbl[DiagIdx] & ATTRIB_TRIP) != 0)
            {
                TripFlt = true;  //Set True immediately

                // The conditionals below capture the first five unique trip catories
                // NOTE:  The placement of this code was choosen due to it only being run once for the same DiagCode (until reset)
                // The code is optimized for minimum CPU loading when subsequent trips are in the same trip category.
                CatNum = (unsigned)(DiagAttribTbl[DiagIdx] & CatNumMask);
                if (TripTypeWord1 != CatNum)
                {
                    if (TripTypeWord1 == 0)
                    {
                        TripTypeWord1 = CatNum;
                        TripWordCnt++;
                    }
                    else if (TripTypeWord2!=CatNum)
                    {
                        if (TripTypeWord2 == 0)
                        {
                            TripTypeWord2 = CatNum;
                            TripWordCnt++;
                        }
                        else if (TripTypeWord3!=CatNum)
                        {
                            if (TripTypeWord3 == 0)
                            {
                                TripTypeWord3 = CatNum;
                                TripWordCnt++;
                            }
                            else if (TripTypeWord4!=CatNum)
                            {
                                if (TripTypeWord4 == 0)
                                {
                                    TripTypeWord4 = CatNum;
                                    TripWordCnt++;
                                }
                                else if (TripTypeWord5!=CatNum)
                                {
                                    if (TripTypeWord5 == 0)
                                    {
                                        TripTypeWord5 = CatNum;
                                        TripWordCnt++;
                                    }
                                }
                            }
                        }
                    }
                }

                if ((DiagAttribTbl[DiagIdx] & ATTRIB_BRIDGE1) != 0)
                {
                    Brg1TripFlt = true;
                }
                if ((DiagAttribTbl[DiagIdx] & ATTRIB_BRIDGE2) != 0)
                {
                    Brg2TripFlt = true;
                }
                //DLL sim fault code output
                if (DsimMode && (DsimFaultCode<=0))
                {
                    DsimFaultCode=DiagCode;
                }
            }
            NoFlt = false;
            if ((DiagAttribTbl[DiagIdx] & ATTRIB_BRIDGE1) != 0)
            {
                Brg1NoFlt = false;
            }
            if ((DiagAttribTbl[DiagIdx] & ATTRIB_BRIDGE2) != 0)
            {
                Brg2NoFlt = false;
            }
        }

        // set active diagnostic flag
        if ((pDiagActCollector[ActCollectIdx][WrdIdx] & TestBit) == 0)
        {
            pthread_mutex_lock(&DiagIdxMutex);   // protecting Active Collector array while changing index during HouseKeeping()
            pDiagActCollector[ActCollectIdx][WrdIdx] |= TestBit;
            pthread_mutex_unlock(&DiagIdxMutex);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    for diagnostic:
//      send a clear message to the diagnostic queue
//      remove from the active diagnostic list
//
//    for SOE
//      add to SOE transition queue in shared memory, to be sent to the list of subscribers
//      add to the active SOE list
//
// EXECUTION LEVEL
//
//
// RETURN VALUE:
//      N/A
//
///////////////////////////////////////////////////////////////////////////////
void    cMarkStatDiag::PopDiagnostic(unsigned DiagCode)
{
    sTimeType    EventTm;
    int         DiagIdx;
    unsigned    WrdIdx;
    uint32_t    TestBit;

    if (!Initialized)
    {
        cDiagMessenger::LogErrorMessage(ALARM_MSG,
            cParamString("Popped diagnostic %d before class initialized", DiagCode).c_str(),
            HERE);
        return;
    }

    if ( (DiagCode < BaseDiagNum) || (DiagCode > MaxDiagNum) )
    {
        if (InvalidDiag == 0)  InvalidDiag = DiagCode;
        return;
    }

    DiagIdx = DiagCode - BaseDiagNum;
    WrdIdx  = DiagIdx >> 5;
    TestBit = 1 << (DiagIdx & 0x1F);

    if ((DiagAttribTbl[DiagIdx] & ATTRIB_SOE) != 0)
    {
        // check for SOE init attribute
        if ( InitDone || ((DiagAttribTbl[DiagIdx] & ATTRIB_INIT) != 0) )
        {
            // set SOE normal
            if ((pSoeInActive[WrdIdx] & TestBit) == 0)
            {
                if ( pthread_mutex_lock(&TimeSyncMutex) == EOK )
                {
                    EventTm.tv_sec  = EventTm1.Sec;
                    EventTm.tv_nsec = EventTm1.nSec;
                    pthread_mutex_unlock(&TimeSyncMutex);
                }
                pthread_mutex_lock(&SoeActiveMutex);        // protecting Active/InActive SOE array while changing bits and Logging SOEs
                LogSoe(DiagCode, false, EventTm);
                pSoeInActive[WrdIdx] |=  TestBit;           // set InActive flag to block subsequent pops
                pSoeActive[WrdIdx]   &= ~TestBit;           // clear Active flag to allow next push
                pthread_mutex_unlock(&SoeActiveMutex);
            }

            // set active diagnostic flag
            if ((pDiagActCollector[ActCollectIdx][WrdIdx] & TestBit) == 0)
            {
                pthread_mutex_lock(&DiagIdxMutex);   // protecting Active Collector array while changing index during HouseKeeping()
                pDiagActCollector[ActCollectIdx][WrdIdx] |= TestBit;
                pthread_mutex_unlock(&DiagIdxMutex);
            }
        }
    }
    else if ( ((pDiagActive[WrdIdx] & TestBit) != 0) && ((DiagAttribTbl[DiagIdx] & ATTRIB_LOCKED) == 0) )
    {
        // set diagnostic normal
        if ( pthread_mutex_lock(&TimeSyncMutex) == EOK )
        {
            EventTm.tv_sec  = EventTm1.Sec;
            EventTm.tv_nsec = EventTm1.nSec;
            pthread_mutex_unlock(&TimeSyncMutex);
        }
        pthread_mutex_lock(&DiagActiveMutex);            // protecting Active Diagnostic array while changing bits and Logging Diagnostics
        LogAlarmMessage(DiagCode, DiagCode,
                        DIAG_ALARM_RECURRING, DIAG_ALARM_HIGH,
                        0, 0, 0, 0,
                        false,
                        EventTm);
        pDiagActive[WrdIdx] &= ~TestBit;
        pthread_mutex_unlock(&DiagActiveMutex);

        // clear active diagnostic flag
        pthread_mutex_lock(&DiagIdxMutex);   // protecting Active Collector array while changing index during HouseKeeping()
        pDiagActCollector[ActCollectIdx][WrdIdx] &= ~TestBit;
        pthread_mutex_unlock(&DiagIdxMutex);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    for diagnostic:
//      check for active diagnostics not being pushed to send a clear message to the diagnostic queue
//      summarize the active trips into categories
//      handle logic to set NoFault and clear TripFault booleans
//
//    for SOE
//      remove from the active SOE list
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE:
//      N/A
//
///////////////////////////////////////////////////////////////////////////////
void    cMarkStatDiag::HouseKeeping()
{
    static bool ReportedInvalidDiag;
    sTimeType    EventTm;
    unsigned    ActCollectIdx_Z1;
    int         DiagIdx, TmpBrg1Flt, TmpBrg2Flt, StartIdx;
    unsigned    StIdx, WrdIdx, BitIdx, CatNum, TripWordCntTmp;

    //  DIAG_ARRAY_SIZE is defined as "6".  It is the "size" of the arrays- ignoring [0] and starting with [1]
    //  Where [1] through [5] is used for clarification for TripTypeWord1 through TripTypeWord5
    unsigned    TripAltWord[DIAG_ARRAY_SIZE], ActFltWord[DIAG_ARRAY_SIZE], ActFltTmpWord[DIAG_ARRAY_SIZE];
    
    uint32_t    TestWord, TestBit, CatNumMask = 0x0000FFFF;
    bool        TmpFault, TmpTrip, WordRepeatFlg, ActFltFlg[DIAG_ARRAY_SIZE];
    int         Level, WordLoopCnt, ActFltWordCnt;

    // handle changing active diagnostic structure index
    pthread_mutex_lock(&DiagIdxMutex);    // protecting Active Collector array while changing index
    ActCollectIdx_Z1 = ActCollectIdx;
    ActCollectIdx    = (ActCollectIdx + 1) % ActCollectCnt;
    memset(pDiagActCollector[ActCollectIdx], 0, DiagWrdCnt*sizeof(uint32_t));
    pthread_mutex_unlock(&DiagIdxMutex);

    // initialize fault
    TmpFault       = false;
    TmpTrip        = false;
    TmpBrg1Flt     = 0;
    TmpBrg2Flt     = 0;

    // walk the list of active SOEs and diagnostics
    for (WrdIdx=0; WrdIdx < DiagWrdCnt; ++WrdIdx)
    {
        // look for active SOEs
        if ( (pSoeActive[WrdIdx] != 0) || (pSoeInActive[WrdIdx] != 0) )
        {
            // produce current status of diagnostic collector
            TestWord = 0;
            for (StIdx=0; StIdx < ActCollectCnt; ++StIdx)
            {
                if (StIdx != ActCollectIdx)
                    TestWord |= pDiagActCollector[StIdx][WrdIdx];
            }
            pthread_mutex_lock(&SoeActiveMutex);        // protecting Active/InActive SOE array while changing bits and Logging SOEs
            pSoeActive[WrdIdx]   &= TestWord;           // clear Active flag to allow next push
            pSoeInActive[WrdIdx] &= TestWord;           // clear InActive flag to allow next pop
            pthread_mutex_unlock(&SoeActiveMutex);
        }

        // look for active diagnostics
        if (pDiagActive[WrdIdx] != 0)
        {
            // produce current status of diagnostic collector
            TestWord = 0;
            for (StIdx=0; StIdx < ActCollectCnt; ++StIdx)
            {
                if (StIdx != ActCollectIdx)
                    TestWord |= pDiagActCollector[StIdx][WrdIdx];
            }

            for (TestBit=1, BitIdx=0; (TestBit > 0); TestBit<<=1, ++BitIdx)
            {
                DiagIdx = (WrdIdx<<5) + BitIdx;
                // active diagnostic transitioned back to normal
                if ( ((pDiagActive[WrdIdx] & TestBit) != 0) && ((TestWord & TestBit) == 0)
                    && ((DiagAttribTbl[DiagIdx] & ATTRIB_LOCKED) == 0) )
                {
                    if ( pthread_mutex_lock(&TimeSyncMutex) == EOK )
                    {
                        EventTm.tv_sec  = EventTm1.Sec;
                        EventTm.tv_nsec = EventTm1.nSec;
                        pthread_mutex_unlock(&TimeSyncMutex);
                    }
                    pthread_mutex_lock(&DiagActiveMutex);         // protecting Active Diagnostic array while changing bits and Logging Diagnostics
                    LogAlarmMessage((DiagIdx + BaseDiagNum), (DiagIdx + BaseDiagNum),
                                    DIAG_ALARM_RECURRING, DIAG_ALARM_HIGH,
                                    0, 0, 0, 0,
                                    false,
                                    EventTm);
                    pDiagActive[WrdIdx] &= ~TestBit;
                    pthread_mutex_unlock(&DiagActiveMutex);
                }

                // determine fault and assign TripTypeWords
                else if ((pDiagActive[WrdIdx] & TestBit) != 0)
                {
                    TmpFault = true;
                    Level    = 1;
                    if ((DiagAttribTbl[DiagIdx] & ATTRIB_TRIP) != 0)
                    {
                        TmpTrip = true;
                        Level   = 2;
                    }

                    if ( ((DiagAttribTbl[DiagIdx] & ATTRIB_BRIDGE1) != 0) && (TmpBrg1Flt < Level) )
                    {
                        TmpBrg1Flt = Level;
                    }

                    if ( ((DiagAttribTbl[DiagIdx] & ATTRIB_BRIDGE2) != 0) && (TmpBrg2Flt < Level) )
                    {
                        TmpBrg2Flt = Level;
                    }
                }
            }
        }
    }

    // set public fault
    if (ResetPending)
    {
        TripFlt           = TmpTrip;

        if (TmpBrg1Flt == 2)   // "Avoid using Tertiary ? :"
        {
            Brg1TripFlt   = true;
        }
        else
        {
            Brg1TripFlt   = false;
        }

        if (TmpBrg2Flt == 2)      // "Avoid using Tertiary ? :"
        {
            Brg2TripFlt   = true;
        }
        else
        {
            Brg2TripFlt   = false;
        }

        ResetPending      = false;
        DiagResetTaskReq  = true;

        // Update the TripTypeWords
        {
            WordLoopCnt   = 0;
            WordRepeatFlg = false;
            do
            {
                // Only update TripTypeWords if there is at least one category word.
                if (TripTypeWord1 != 0)
                {
                    // Temporarily save the TripTypeWords Counter
                    TripWordCntTmp       = TripWordCnt;

                    // Reset all Active Fault Flags and Words "
                    for (int i=1; i<DIAG_ARRAY_SIZE; i++)
                    {
                        ActFltFlg[i]     = false;
                        ActFltWord[i]    = 0;
                        ActFltTmpWord[i] = 0;
                        TripAltWord[i]   = 0;
                    }

                    // walk the list of active SOEs and diagnostics
                    // Same logic used at the beginning of the Housekeeping ()
                    // to walk through all active SOEs & Diagnostics.
                    // The "walking" through the list has to be repeated because
                    // this section of code allows for the chance that during a 
                    // reset (), that a PUSH_DIAG() may change a TripTypeWordn
                    // as such, the reset of TripTypeWords may need to be repeated.
                    for (WrdIdx=0; WrdIdx < DiagWrdCnt; ++WrdIdx)
                    {
                        // look for active diagnostics
                        if (pDiagActive[WrdIdx] != 0)
                        {
                            for (TestBit=1, BitIdx=0; (TestBit > 0); TestBit<<=1, ++BitIdx)
                            {
                                DiagIdx = (WrdIdx<<5) + BitIdx;
                                if ((pDiagActive[WrdIdx] & TestBit) != 0)
                                {
                                    if ((DiagAttribTbl[DiagIdx] & ATTRIB_TRIP) != 0)
                                    {
                                        // When a fault is found to be active, determine its category number
                                        CatNum = (unsigned)(DiagAttribTbl[DiagIdx] & CatNumMask);

                                        // Find category number in the TripTypeWords
                                        if      (TripTypeWord1 == CatNum)
                                        {
                                            ActFltFlg[1] = true;
                                        }
                                        else if (TripTypeWord2 == CatNum) 
                                        {
                                            ActFltFlg[2] = true;
                                        }
                                        else if (TripTypeWord3 == CatNum) 
                                        {
                                            ActFltFlg[3] = true;
                                        }
                                        else if (TripTypeWord4 == CatNum) 
                                        {
                                            ActFltFlg[4] = true;
                                        }
                                        else if (TripTypeWord5 == CatNum) 
                                        {
                                            ActFltFlg[5] = true;
                                        }

                                        // If unique additional CatNums exist but are not listed in the first five Words,
                                        // then save up to five additional alternate CatNums.
                                        // Use same logic tree for filling TripTypeWords to get "out" of the
                                        // decisions tree as quickly as possible.
                                        else if (TripAltWord[1] != CatNum)
                                        {
                                            if (TripAltWord[1] == 0)
                                            {
                                                TripAltWord[1] = CatNum;
                                            }
                                            else if (TripAltWord[2] != CatNum)
                                            {
                                                if (TripAltWord[2] == 0)
                                                {
                                                    TripAltWord[2] = CatNum;
                                                }
                                                else if (TripAltWord[3] != CatNum)
                                                {
                                                    if (TripAltWord[3] == 0)
                                                    {
                                                        TripAltWord[3] = CatNum;
                                                    }
                                                    else if (TripAltWord[4] != CatNum)
                                                    {
                                                        if (TripAltWord[4] == 0)
                                                        {
                                                            TripAltWord[4] = CatNum;
                                                        }
                                                        else if (TripAltWord[5] != CatNum)
                                                        {
                                                            if (TripAltWord[5] == 0)
                                                            {
                                                                TripAltWord[5] = CatNum;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Create an active fault word list in trip order with inactive words set to zero
                    if (ActFltFlg[1] == true)    // "Avoid using Tertiary ? :"
                    {
                        ActFltTmpWord[1] = TripTypeWord1;
                    }
                    else
                    {
                        ActFltTmpWord[1] = 0;
                    }

                    if (ActFltFlg[2] == true)
                    {
                        ActFltTmpWord[2] = TripTypeWord2;
                    }
                    else
                    {
                        ActFltTmpWord[2] = 0;
                    }

                    if (ActFltFlg[3] == true)
                    {
                        ActFltTmpWord[3] = TripTypeWord3;
                    }
                    else
                    {
                        ActFltTmpWord[3] = 0;
                    }

                    if (ActFltFlg[4] == true)
                    {
                        ActFltTmpWord[4] = TripTypeWord4;
                    }
                    else
                    {
                        ActFltTmpWord[4] = 0;
                    }

                    if (ActFltFlg[5] == true)
                    {
                        ActFltTmpWord[5] = TripTypeWord5;
                    }
                    else
                    {
                        ActFltTmpWord[5] = 0;
                    }

                    // Remove zeroes from Active Fault Word List
                    // Count Active Fault Words
                    ActFltWordCnt = 0;
                    for (int i=1; i<DIAG_ARRAY_SIZE; i++)
                        if (ActFltTmpWord[i] != 0) ActFltWordCnt++;

                    // Remove any zeroes not at the bottom of the Active Fault Word List while retaining trip order
                    // ActFltWord[ ] is reset to all zeroes at start of "Do loop"
                    StartIdx = 1;
                    for (int i=1; i<=ActFltWordCnt; i++)
                    {
                        for (int j=StartIdx; j<DIAG_ARRAY_SIZE; j++)
                        {
                            if (ActFltTmpWord[j] != 0) 
                            {
                                ActFltWord[i] = ActFltTmpWord[j];
                                StartIdx = (j + 1);  // Eliminates looping through unused ActFltTmpWord[ ]
                                break;
                            }
                        }
                    }

                    // If the actual fault word count is less than 5, and at least one Alternate category Word exists
                    // then fill in any zeroes at bottom of Active fault list with alternate category words.
                    if ((ActFltWordCnt < (DIAG_ARRAY_SIZE-1)) && (TripAltWord[1] != 0))
                    {
                        for (int i=1; i<(DIAG_ARRAY_SIZE-ActFltWordCnt); i++)
                        {
                            ActFltWord[(ActFltWordCnt+i)]=TripAltWord[i];
                        }
                    }

                    // Only copy the Active Fault Words if the TripTypeWord counter has not changed
                    // by a Task1, Task2, Task3 PUSH_DIAG
                    if (TripWordCnt == TripWordCntTmp)
                    {
                        TripTypeWord5    = ActFltWord[5];
                        TripTypeWord4    = ActFltWord[4];
                        TripTypeWord3    = ActFltWord[3];
                        TripTypeWord2    = ActFltWord[2];
                        TripTypeWord1    = ActFltWord[1];
 
                        // If a PUSH_DIAG occurs during the copy of the five TripTypeWordn
                        // then a valid category number will be "lost" and not sent over IPC.
                        // "Lost" category numbers are counted in "TripLostWordCnt"
 
                        // The "lost" category number may be recovered if it remains active until
                        // the next RESET.
                         
                        // Any Category Number not the first five TripTypeWords will be lost
                        // if it goes inactive before the RESET goes high. 
                        if (TripWordCnt != TripWordCntTmp) 
                        {    
                            TripLostWordCnt++;
                        }
                        WordRepeatFlg    = false;
                    }
                    else WordRepeatFlg   = true;
                }

            // Repeat the Do-Loop if a PUSH_DIAG occurs while doing a reset.
            // As long as a PUSH_DIAG does not occur while the five TripTypeWordn are being updated
            // then no category numbers are lost, but instead the entire trip category reset section is repeated.
            // Since at most four more PUSH_DIAG may occur, the "DO-LOOP" is limited to four repeats
            } while ( (++WordLoopCnt < (DIAG_ARRAY_SIZE-1)) && (WordRepeatFlg == true) );
        }
    }

    NoFlt            = !TripFlt && !TmpFault;
    NoTripFlt        = !TripFlt;
    Brg1NoFlt        = (!Brg1TripFlt && TmpBrg1Flt == 0) ? true : false;
    Brg2NoFlt        = (!Brg2TripFlt && TmpBrg2Flt == 0) ? true : false;

    // report invalid diagnostic
    if ( (InvalidDiag != 0) && !ReportedInvalidDiag )
    {
        ReportedInvalidDiag = true;
        LogErrorMessage(ALARM_MSG, cParamString(MKS_DIAG_STR,
            cParamString("Invalid Diagnostic number used, %d", InvalidDiag).c_str()),
            HERE);
    }

}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Checks for fault reset request from application control
//
// EXECUTION LEVEL
//
//
// RETURN VALUE:
//      N/A
//
///////////////////////////////////////////////////////////////////////////////
void    cMarkStatDiag::DiagnosticResetCheck()
{
    static unsigned ApplInterfaceOkEdg = FALSE;
    static unsigned ApplFltRstReqEdg = FALSE;
    static unsigned FltRstEdg;
    unsigned        FltRst;

    // Populate FltRst from pointer if enabled
    if ((int)PARM(FltRstAdr) != LADR_UnusedUInt)
        FltRst = gVariable.ReadAsBool(PARM(FltRstAdr));
    else
        FltRst = FALSE;

    // Ignore edges when the application interface status changes
    if ( ApplInterfaceOk ^ ApplInterfaceOkEdg )
    {
        ApplFltRstReqEdg = ApplFltRstReq;
    }
    ApplInterfaceOkEdg = ApplInterfaceOk;

    // Clear faults on rising edge of any FltRst
    // check for requests from the application interface only if it is healthy

    if ( (ApplInterfaceOk && (ApplFltRstReq && (ApplFltRstReq ^ ApplFltRstReqEdg)))
         || (FltRst && (FltRst ^ FltRstEdg)) )
    {
        ResetDiag();
    }

    if ( ApplInterfaceOk )  ApplFltRstReqEdg = ApplFltRstReq;

    // Save fault reset edge for next pass
    FltRstEdg = FltRst;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      log a message into the error log
//
// EXECUTION LEVEL
//
//
// RETURN VALUE:
//      N/A
//
///////////////////////////////////////////////////////////////////////////////
int     cMarkStatDiag::LogErrorMessage(enum LogError_Type ErrorType,
                        cParamString Description,
                        int CurrentLine, const char *CurrentFile)
{
    if (Initialized)
    {
        if (ErrorType == TRIP_MSG)
            PUSH_DIAG(TripErrorLogged);
        else if (ErrorType == ALARM_MSG)
            PUSH_DIAG(AlarmErrorLogged);

        return cDiagMessenger::LogErrorMessage(MKS_ERR_BASE, Description, CurrentLine, CurrentFile, EventTm1.Sec, EventTm1.nSec);
    }
    else
    {
        if (ErrorType == TRIP_MSG)
            LogTripError = true;
        else if (ErrorType == ALARM_MSG)
            LogAlarmError = true;

        return cDiagMessenger::LogErrorMessage(MKS_ERR_BASE, Description, CurrentLine, CurrentFile);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      add to SOE transition queue in shared memory, to be sent to the list of subscribers
//
// EXECUTION LEVEL
//
//
// RETURN VALUE:
//      Status - SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cMarkStatDiag::LogSoe (
        unsigned SoeNumber,         // unique SOE identifier
        bool State,                 // specify wether event transitioned ON or OFF
        sTimeType CurrentTime        // time event occured
)
{
    Soe_Dollar_entry    SoeEntry;

	SoeEntry.index 			= SoeIndex;
    SoeEntry.drop_number    = SoeNumber;
    SoeEntry.state          = (State ? 1:0);
	SoeEntry.time_spec 		= CurrentTime;
	SoeEntry.producer_id 	= 0;
    SoeEntry.channel        = ControllerId;

    if (SoeLogClient.LogSoeTransition(SoeEntry, SoeIndex) != 0)
        return FAIL;

    if (++SoeIndex == 0)
        SoeIndex = 1; // prevent rollover to zero

    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      display diagnostic configuration and status for llm monitor
//          (-r) perform a reset on the diagnostic queue
//          (-p) push a diagnostic into the queue
//
// EXECUTION LEVEL
//      low level monitor thread
//
// RETURN VALUE:
//      EOK
//
///////////////////////////////////////////////////////////////////////////////
int     cMarkStatDiag::DiagStatus(cParamString &OutputBuffer, cParamString &CommandLineArgs)
{
    int             CurrentOffset;
    const char      *pString = NULL;
    int             DiagNumber;

    // diagnostic status
    OutputBuffer.pappend("Diagnostic Status\n");
    OutputBuffer.pappend("   BaseDiagNum   %d\n", FirstDiagInstance->BaseDiagNum);
    OutputBuffer.pappend("   MaxDiagNum    %d\n", FirstDiagInstance->MaxDiagNum);
    OutputBuffer.pappend("   ActCollectCnt %d\n", FirstDiagInstance->ActCollectCnt);
    OutputBuffer.pappend("   DiagWrdCnt    %d\n", FirstDiagInstance->DiagWrdCnt);
    OutputBuffer.pappend("   Controller ID %d\n", FirstDiagInstance->ControllerId);
    OutputBuffer.pappend("\n");

    // reset diagnostic alarms
    CurrentOffset = CommandLineArgs.find("-r");
    if(CurrentOffset != CommandLineArgs.npos)
    {
        OutputBuffer.pappend("Reset Diagnostic\n");
        OutputBuffer.pappend("\n");
        FirstDiagInstance->ResetDiag();
    }

    // push diagnostic alarm
    CurrentOffset = CommandLineArgs.find("-p");
    if(CurrentOffset != CommandLineArgs.npos)
    {
        pString = &CommandLineArgs.at(CurrentOffset+2);
        DiagNumber = (int)atoi(pString);

        OutputBuffer.pappend("Push Diagnostic %d\n", DiagNumber);
        OutputBuffer.pappend("\n");
        FirstDiagInstance->PushDiagnostic(DiagNumber);
    }


    return EOK;
}

