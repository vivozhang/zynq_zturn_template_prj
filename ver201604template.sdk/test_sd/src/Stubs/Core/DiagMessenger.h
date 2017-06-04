#ifndef __DIAGMESSENGER_H__
#define __DIAGMESSENGER_H__


#include <stdio.h>
#include <sys/neutrino.h>
#include "Hardware.h"
#include "ParamString.h"


const int  DEVCTL_RESET_ALM=0;

//Diagnostic Levels
const int  DIAG_SYSTEM  = 1;    //System Info ie Process startups, System state changes
const int  DIAG_PROCESS = 2;   //Process Info ie Process startups Process state changes

// Alarm Type
const char DIAG_ALARM_SNGL_SHOT = 0;
const char DIAG_ALARM_RECURRING = 1;
const char DIAG_ALARM_PERMANENT = 2;

// Alarm priority
const char DIAG_ALARM_LOW = 0;
const char DIAG_ALARM_HIGH = 1;


///////////////////////////////////////////////////////////////////////////////
class cDiagMessenger
{
public:
    cDiagMessenger(int EnbAlmLogThread=0);
    ~cDiagMessenger();

    void         SetDiagnosticLevel(unsigned int DiagnosticLevel)   {;}
    unsigned int GetDiagnosticLevel()                               {return 0;}
    int          RegisterResetNotify(int ChannelId, int ProcessId)  {return 0;}
    int          ResetAlarm()                                       {DiagWasReset(); return 0;}

    int  LogEventMessage(int EventNumber,
                         unsigned int DiagLevel,
                         cParamString EventDescription,
                         int CurrentLine, const char *CurrentFile)  {return 0;}

    int  LogErrorMessage(int ErrorNumber,
                         cParamString ErrorDescription,
                         int CurrentLine, const char *CurrentFile,
                         int TimeSec = 0, int TimeNSec = 0);

    int  LogAlarmMessage(unsigned int AlarmNumber, unsigned int MsgNumber,
                         char AlarmType, char Priority,
                         float Param1, float Param2, float Param3, float Param4,
                         char AlmState, sTimeType TimeStamp,
                         unsigned short DropoutFilterMs=0);

protected:
    virtual unsigned GetBaseDiagNum() {return 0;}
    virtual void     DiagWasReset()   {;}


};





#endif  // __DIAGMESSENGER_H__

