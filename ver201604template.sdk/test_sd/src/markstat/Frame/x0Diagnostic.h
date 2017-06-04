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

///////////////////////////////////////////////////////////////////////////////
#ifndef _X0DIAGNOSTIC_H_
#define _X0DIAGNOSTIC_H_
///////////////////////////////////////////////////////////////////////////////

// Include Files:
//---------------
#include <stdint.h>

// core
#include "DiagMessenger.h"
#include "MonitorCommand.h"
#include "SoeLogClient.h"

// local


// Constants:
//-----------
#define DIAG_ARRAY_SIZE		(6)

// Function Prototypes:
//---------------------


// Types and Classes:
//-------------------

enum    LogError_Type
{
    TRIP_MSG,
    ALARM_MSG,
    LOG_ONLY
};


///////////////////////////////////////////////////////////////////////////////
class cMarkStatDiag : private cDiagMessenger
{
public:
    cMarkStatDiag();
    ~cMarkStatDiag();

    // Methods
    //--------
    int         Initialize(cMonitorCommand &gMonitor);
    void        PushDiagnostic(unsigned DiagCode);
    void        PopDiagnostic(unsigned DiagCode);
    void        HouseKeeping();
    static int  DiagStatus(cParamString &OutputBuffer, cParamString &CommandLineArgs);
    unsigned    GetBaseDiagNum() {
                    return BaseDiagNum;
                }

    void        ResetDiag() {
                    cDiagMessenger::ResetAlarm();
                }
    void        DiagWasReset() {
                    ResetPending = true;
                }

    void        DiagnosticResetCheck();

    int         LogErrorMessage(enum LogError_Type ErrorType,
                                cParamString Description,
                                int CurrentLine, const char *CurrentFile);

    cDiagMessenger::SetDiagnosticLevel;
    cDiagMessenger::GetDiagnosticLevel;
    cDiagMessenger::LogEventMessage;
    cDiagMessenger::RegisterResetNotify;

private:

    // Methods
    //--------
    int         LogSoe(unsigned SoeNumber, bool State, sTimeType CurrentTime);

    // Unpublished Variables
    //----------------------
    unsigned            DiagTblSize;            // number of entries in diagnostic table
    static void*        DiagAddrTbl[];          // diagnostic address cross reference table
    static uint32_t     DiagAttribTbl[];        // Diagnostic attribute table
    unsigned            BaseDiagNum;            // base diagnostic alarm number
    unsigned            MaxDiagNum;             // maximum diagnostic alarm number
    unsigned            ActCollectCnt;          // number of diagnostic bit structures in collection array to accommodate timer
    unsigned            DiagWrdCnt;             // number of 32 bit words to accommodate diagnostic bit structure
    unsigned            ActCollectIdx;          // current index for active collection array
    uint32_t**          pDiagActCollector;      // pointer to active Diagnostic collection array
    uint32_t*           pDiagActive;            // pointer to active Diagnostic summation
    uint32_t*           pSoeActive;             // pointer to active SOE summation
    uint32_t*           pSoeInActive;           // pointer to inactive SOE summation
    bool                Initialized;            // diagnostic class has been initialized
    bool                ResetPending;           // diagnostic reset happened, waiting to take action
    unsigned            InvalidDiag;            // invalid diagnostic pushed
    bool                LogTripError;           // push diagnostic after initialized
    bool                LogAlarmError;          // push diagnostic after initialized

    pthread_mutex_t     DiagIdxMutex;           // protecting Active Collector array while changing index
    pthread_mutex_t     DiagActiveMutex;        // protecting Active Diagnostic array while changing bits and Logging Diagnostics
    pthread_mutex_t     SoeActiveMutex;         // protecting Active/InActive SOE array while changing bits and Logging SOEs

    ControllerIdEnum    ControllerId;           // system controller ID
    cSoeLogClient       SoeLogClient;           // SOE class instance for communicating to SOE process
    unsigned            SoeIndex;               // unique index to identify SOE entries

};


///////////////////////////////////////////////////////////////////////////////
#endif  // _X0DIAGNOSTIC_H_

