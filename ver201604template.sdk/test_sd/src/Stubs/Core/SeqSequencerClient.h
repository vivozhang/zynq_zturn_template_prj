#ifndef __SEQSEQUENCERCLIENT_H__
#define __SEQSEQUENCERCLIENT_H__


class cSeqSequencerClient   
{
public:
    int Register(char const *pStartFrameStateName,
                 char const *pStopFrameStateName,
                 char const *pClientName,
                 struct sigevent const *pOverrunEvent,
                 bool CriticalClient = false,
                 bool DisableOverrunAlarm = false,
                 bool OnlyOnePendingBid = false,
                 bool WaitForPriorStateTimeout = false)       {return EOK;}
    int Arm(unsigned int *pFrameUsecOffset = NULL)            {return EOK;}
    
};            



#endif  // __SEQSEQUENCERCLIENT_H__

