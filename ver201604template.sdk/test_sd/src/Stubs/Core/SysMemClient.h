#ifndef __SYSMEMCLIENT_H__
#define __SYSMEMCLIENT_H__


#include "Hardware.h"
#include "SysMsgClient.h"


class cSysMemConfigClient
{
public:
    unsigned long      GetFramePeriod()                { return 10000; }
    ControllerIdEnum   GetControllerId()               { return ID_R; }
};

class cSysSwWdogClient
{
public:
    int   Open()                                      {return 0;}
    int   Register(cSysMsgWatchDogRegistration Reg)   {return 0;}
    void  Tickle()                                    {;}

};            
        


#endif  // __SYSMEMCLIENT_H__

