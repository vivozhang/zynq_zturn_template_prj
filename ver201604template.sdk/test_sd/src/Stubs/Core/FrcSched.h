#ifndef __FRCSCHED_H__
#define __FRCSCHED_H__

#include <stdint.h>



enum FrcSyncSrcTyp   { FrcSyncExtern, FrcSyncIntern, FrcSyncSrcUnknown };

class cFrcSchedBase 
{
public:
    virtual uint64_t    GetCountsPerSec()                       {return 25000000;}
    virtual int         Init(bool ConfigureHardware = true)     {return 0;}
    virtual int         SelectSyncSrc(FrcSyncSrcTyp src)        {return 0;}
    virtual int         SelectCmpNum(unsigned long num)         {return 0;}
    virtual bool        IsMyInterrupt()                         {return false;}
    virtual void        ClearInterrupt()                        {;}
    virtual int         SetCompare(long value, bool frameFlag = false) {return 0;}
    virtual int         GetIrq()                                {return 0;}
    virtual long        GetCounter()                            {return 0;}
    virtual int         EnableInterrupt(uint32_t NextCmp)       {return 0;}
};

class cFrcSchedCsla : public cFrcSchedBase
{
};    

class cFrcSchedEpmc : public cFrcSchedBase
{
};    

class cFrcSchedUcsb : public cFrcSchedBase
{
};    


#endif  // __FRCSCHED_H__

