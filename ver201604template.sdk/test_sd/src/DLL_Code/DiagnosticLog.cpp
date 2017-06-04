///////////////////////////////////////////////////////////////////////////////
//
// Diagnostic Logger
//
///////////////////////////////////////////////////////////////////////////////

// Include Files:
//---------------
// system
#include <stdio.h>
#include <string>
// core
#include "ParamString.h"
#include "DiagMessenger.h"
#include "SoeLogClient.h"
// framework
#include "x0FrameWork.h"
#include "x0Diagnostic.h"


// Constants:
//-----------


// Externals:
//-----------
extern cMarkStatDiag* FirstDiagInstance;


// Types and Classes:
//-------------------
struct DiagInfoType
{
    std::string  Symbol;
    std::string  Description;
    std::string  Type;
} ;


// Function Prototypes:
//---------------------


// Variables:
//-----------
const char*     pDfltDiagLogFile = "DiagnosticLog.csv";
const char*     pDiagLogFile;
FILE*           pDiagLog;
unsigned        MaxDiagIdx;


// Parameters:
//------------


// Diagnostics:
//------------


// address cross reference table
//----------------------------------------
#undef  DIAG_DLL_ENTRY
#define DIAG_DLL_ENTRY(PubName, Description, Type)  {PubName, Description, Type},
DiagInfoType DiagInfoTbl[] =
{
    #ifndef PRE_PROC
        #include "DLLDiagnostic.def"
    #endif
};


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cDiagMessenger::cDiagMessenger(int EnbAlmLogThread)
{
    pDiagLogFile  = pDfltDiagLogFile;

    if ((pDiagLog=fopen(pDiagLogFile, "w")) == NULL)
    {
        printf("Failed to open Diagnostic Log file, %s (%d,%s)", pDiagLogFile, errno, strerror(errno));
    }
    else
        fprintf(pDiagLog, "Time (sec),Type,Diag Name,State,Description\n");

    MaxDiagIdx = (sizeof(DiagInfoTbl)/sizeof(DiagInfoTbl[0])) - 1;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cDiagMessenger::~cDiagMessenger()
{
    if (pDiagLog != NULL)
        fclose(pDiagLog);
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
int  cDiagMessenger::LogAlarmMessage(unsigned int AlarmNumber, unsigned int MsgNumber,
                         char AlarmType, char Priority,
                         float Param1, float Param2, float Param3, float Param4,
                         char AlmState, sTimeType TimeStamp,
                         unsigned short DropoutFilterMs)
{
    unsigned    DiagIdx;
    cDString    Message;

    if ( (AlarmNumber == DIAG_TripErrorLogged) || (AlarmNumber == DIAG_AlarmErrorLogged) )
        return 0;

    DiagIdx = AlarmNumber - GetBaseDiagNum();
    if (DiagIdx <= MaxDiagIdx)
    {
        Message = cParamString("%d.%09d,%s,%s,%s,\"%s\"", TimeStamp.tv_sec, TimeStamp.tv_nsec, DiagInfoTbl[DiagIdx].Type.c_str(),
                    DiagInfoTbl[DiagIdx].Symbol.c_str(), (AlmState?"true":"false"), DiagInfoTbl[DiagIdx].Description.c_str());
    }
    else
    {
        Message = cParamString("%d.%09d,,Alarm %d,%s,\"\"", TimeStamp.tv_sec, TimeStamp.tv_nsec,
                    AlarmNumber, (AlmState?"true":"false"));
    }

    if (pDiagLog != NULL)
        fprintf(pDiagLog, "%s\n", Message.c_str());
    else
        printf("ALARM: %s\n", Message.c_str());

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
int  cDiagMessenger::LogErrorMessage(int ErrorNumber,
                         cParamString ErrorDescription,
                         int CurrentLine, const char *CurrentFile,
                         int TimeSec, int TimeNSec)
{
    if (pDiagLog != NULL)
        fprintf(pDiagLog, "%d.%09d,ERRORlog,,,\"%s  (%s:%d)\"\n", TimeSec, TimeNSec,
                ErrorDescription.c_str(), (strrchr(CurrentFile,'\\')?strrchr(CurrentFile,'\\')+1:CurrentFile), CurrentLine );

    // allways printf ErrorLog to screen
    printf("ERROR: %d.%09d  %s  (%s:%d)\n", TimeSec, TimeNSec,
            ErrorDescription.c_str(), (strrchr(CurrentFile,'\\')?strrchr(CurrentFile,'\\')+1:CurrentFile), CurrentLine );

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
int  cSoeLogClient::LogSoeTransition(Soe_Dollar_entry SoeEntry,unsigned int SoeIndex)
{
    unsigned    DiagIdx;
    cDString    Message;

    DiagIdx = SoeEntry.drop_number - FirstDiagInstance->GetBaseDiagNum();
    if (DiagIdx <= MaxDiagIdx)
    {
        Message = cParamString("%d.%09d,%s,%s,%s,\"%s\"", SoeEntry.time_spec.tv_sec, SoeEntry.time_spec.tv_nsec, DiagInfoTbl[DiagIdx].Type.c_str(),
                    DiagInfoTbl[DiagIdx].Symbol.c_str(), (SoeEntry.state?"true":"false"), DiagInfoTbl[DiagIdx].Description.c_str());
    }
    else
    {
        Message = cParamString("%d.%09d,%s,SOE %d,%s,\"\"", SoeEntry.time_spec.tv_sec, SoeEntry.time_spec.tv_nsec, "SOE",
                    SoeEntry.drop_number, (SoeEntry.state?"true":"false"));
    }

    if (pDiagLog != NULL)
        fprintf(pDiagLog, "%s\n", Message.c_str());
    else
        printf("SOE: %s\n", Message.c_str());

    return 0;
}

