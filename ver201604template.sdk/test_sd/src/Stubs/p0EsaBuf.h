#ifndef __P0ESABUF_H__
#define __P0ESABUF_H__



#include "x0CaptureBuf.h"


class cEsaBuf : public cCaptureBufBase
{
public:
    cEsaBuf(const char* Name, unsigned Size, int MaxFlashCnt=0)     {;}
    void        Task1A()                                            {;}
    void        EsaTrigger(float)                                   {;}
};





#endif  // __P0ESABUF_H__
