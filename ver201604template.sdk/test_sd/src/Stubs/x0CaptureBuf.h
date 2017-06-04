#ifndef __X0CAPTUREBUF_H__
#define __X0CAPTUREBUF_H__



#include "MonitorCommand.h"
#include "x0FrameWork.h"


const unsigned  CAP_TASK_1_RATE = 1;
const unsigned  CAP_TASK_2_RATE = 2;
const unsigned  CAP_TASK_3_RATE = 3;


enum   StateEnum
{
    CAP_NOT_ENABLED = 0,
    CAP_WAITING     = 1,
    CAPTURING       = 2,
    CAP_COMPLETE    = 3,
    CAP_LOCKED      = 0x8003,
};

enum   TrigModeEnum        // first 2 digits are for toolbox ordering, last 2 digits are SDI codes
{
    TRIG_BOOL  = 1006,
    TRIG_IBOOL = 1101,
    TRIG_EQ    = 2001,
    TRIG_NE    = 2106,
    TRIG_GT    = 3002,
    TRIG_GE    = 3103,
    TRIG_LT    = 4004,
    TRIG_LE    = 4105,
};

enum   TrigTypeEnum
{
    TRIG_EDGE  = 1,
    TRIG_LEVEL = 2,
};


struct CaptureBufStompParmTyp
{
    unsigned        CapChanCnt;
    unsigned        CapTaskExec;
    unsigned        CapPMult;
    float           CapEnbDelay;

    void*           CapTrigAdr;
    float           CapTrigValue;
    TrigModeEnum    CapTrigMode;
    TrigTypeEnum    CapTrigType;
    unsigned        CapPreSample;

    void*           CapAdr01;
    void*           CapAdr02;
    void*           CapAdr03;
    void*           CapAdr04;
    void*           CapAdr05;
    void*           CapAdr06;
    void*           CapAdr07;
    void*           CapAdr08;
    void*           CapAdr09;
    void*           CapAdr10;
    void*           CapAdr11;
    void*           CapAdr12;
    void*           CapAdr13;
    void*           CapAdr14;
    void*           CapAdr15;
    void*           CapAdr16;
    void*           CapAdr17;
    void*           CapAdr18;
    void*           CapAdr19;
    void*           CapAdr20;
    void*           CapAdr21;
    void*           CapAdr22;
    void*           CapAdr23;
    void*           CapAdr24;
    void*           CapAdr25;
    void*           CapAdr26;
    void*           CapAdr27;
    void*           CapAdr28;
    void*           CapAdr29;
    void*           CapAdr30;
};


class cCaptureBufBase
{
public:
    static void Initialize(cMonitorCommand& gMonitor)   {;}
    void        Enable()                                {;}
    void        Disable()                               {;}
    int         SaveToFlash()                           {return SUCCESS;}
    StateEnum   GetState()                              {return CAP_NOT_ENABLED;}
    
    void        ReConfigure()                           {;}
    void        AutoEnable()                            {;}
    int         RunRules()                              {return SUCCESS;}
    int         StompParms(CaptureBufStompParmTyp &ParmValue)  {return SUCCESS;}
    void        Task1()                                 {;}
    void        Task2()                                 {;}
    void        Task3()                                 {;}
};

class cCaptureBuf : public cCaptureBufBase
{
public:
    cCaptureBuf(const char* Name, unsigned Size, int MaxFlashCnt=0) {;}
};
class cTripBuf : public cCaptureBufBase
{
public:
    cTripBuf(const char* Name, unsigned Size, int MaxFlashCnt=0)    {;}
    int         RunRules(CaptureBufStompParmTyp* pParmValue)        {return SUCCESS;}
};



#endif  // __X0CAPTUREBUF_H__

