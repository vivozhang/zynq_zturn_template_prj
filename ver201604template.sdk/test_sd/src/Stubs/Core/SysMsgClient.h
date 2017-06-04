#ifndef __SYSMSGCLIENT_H__
#define __SYSMSGCLIENT_H__


#include <sys/neutrino.h>              // Messaging
#include "GeDString.h"


enum SysActionEnum
{
    DIAGNOSTIC_ONLY  = 1,
    SEND_SERVER_FAIL = 2,
    SYS_FAIL         = 3,
    RESTART          = 4,
    REBOOT           = 5
};

class cSysMsgWatchDogRegistration
{
public:
    cDString           mName;       // Name to display on error
    SysActionEnum      mAction;     // what sys should do on a watchdog timeout
    sTimeType           mPeriod;     // rate of the watch dog (rate of tickle)
};
    


#endif  // __SYSMSGCLIENT_H__

