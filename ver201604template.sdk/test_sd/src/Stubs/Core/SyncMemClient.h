#ifndef __SYNCMEMCLIENT_H__
#define __SYNCMEMCLIENT_H__


#include "x0FrameWork.h"
extern SysTm_Typ SysTm1;


class cSyncMemClient
{
public:
    int  Open()                 {return 0;}
    bool IsOpen()               {return false;}
    bool IsNtpLocked()          {return true;}
    sTimeType PreciseUtcTime()   { sTimeType Ts;
                                  Ts.tv_sec  = SysTm1.Sec;
                                  Ts.tv_nsec = SysTm1.nSec;
                                  return Ts;  }

};



#endif  // __SYNCMEMCLIENT_H__

