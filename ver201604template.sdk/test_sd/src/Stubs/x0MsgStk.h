#ifndef __X0MSGSTK_H__
#define __X0MSGSTK_H__


#include "x0FrameWork.h"


enum    ParmType
{
    xFLOAT,
    xINT
};


class cMessageStack
{
public:
    static  int InitMsgStack(unsigned StackSize)    {return SUCCESS;}
    static  int FreeMsgStack(void)                  {return SUCCESS;}
    static  int PushMsg(unsigned MessageId, enum ParmType ParameterType, ...)  {return SUCCESS;}
    
    static  int MsgQuePres;
    static  int MsgPending;
};




#endif  // __X0MSGSTK_H__
