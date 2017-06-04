#ifndef __X0SDIMSGSERVER_H__
#define __X0SDIMSGSERVER_H__


#include "Hardware.h"


class cSdiMsgServer 
{
public:
    int  Open(int ChannelId, ControllerIdEnum ControllerId)     {return 0;}
    int  MessageHandler(int ReceiveId)                          {return 0;}
            
};


#endif  // __X0SDIMSGSERVER_H__

