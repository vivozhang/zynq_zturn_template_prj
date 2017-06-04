#ifndef __IPCDEVICE_H__
#define __IPCDEVICE_H__



class cIpcDevice
{
public:
    void    DisperseDataFrmM6e(void)                            {;}
    void    CollectDataToM6e(void)                              {;}
    void    RegGetAddrFunc(void* (*GetAddrFunc) (unsigned))     {;}
};
    


#endif  // __IPCDEVICE_H__
