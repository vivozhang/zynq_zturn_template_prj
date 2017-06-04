#ifndef __X0SIMINTERFACE_H__
#define __X0SIMINTERFACE_H__



class cXsimInterface 
{
public:
    void LinkControl (void)          {;}
    int  InitDataPointers (void* pDataToXSim, unsigned BytesToXSim, void* pDataFromXSim, unsigned BytesFromXSim) {return 0;}
    void TerminateConnection (void)  {;}
    bool IsClientConnected (void)    {return false;}
    bool IsFbkValid (void)           {return false;}
            
};


#endif  // __X0SIMINTERFACE_H__

