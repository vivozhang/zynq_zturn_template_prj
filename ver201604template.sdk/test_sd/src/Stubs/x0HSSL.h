#ifndef __X0HSSL_H__
#define __X0HSSL_H__


#include <sys\types.h>
#include <stdint.h>
#include "x0FrameWork.h"
#include "x0LinkIdInfo.h"



#define Hssl_WIN_TYPE_IN                (0)         // target -> host
#define Hssl_WIN_TYPE_OUT               (1)         // host -> target

#define Hssl_MAX_PACKET_SIZE            (1024)

// host state machine definitions
#define Hssl_HOST_NO_LINK               (0x00)
#define Hssl_HOST_STATE_DISCOVER        (0x01)
#define Hssl_HOST_STATE_LD_TRGT         (0x02)
#define Hssl_HOST_STATE_LDING           (0x04)
#define Hssl_HOST_STATE_PROG_ERR        (0x08)
#define Hssl_HOST_STATE_CONFIG          (0x10)
#define Hssl_HOST_STATE_SYNC            (0x20)
#define Hssl_HOST_STATE_RUNNING         (0x40)
#define Hssl_HOST_STATE_FAULT           (0x80)

// target state machine definitions
#define Hssl_TARGET_NO_LINK             (0x00)
#define Hssl_TARGET_STATE_PROG_ERR      (0x01)
#define Hssl_TARGET_STATE_IDLE          (0x02)
#define Hssl_TARGET_STATE_LOADING       (0x04)
#define Hssl_TARGET_STATE_SYNCWAIT      (0x08)
#define Hssl_TARGET_STATE_SYNCD         (0x10)
#define Hssl_TARGET_STATE_RUNNING       (0x20)
#define Hssl_TARGET_STATE_FAULT         (0x40)
#define Hssl_TARGET_STATE_RESETPND      (0x80)



struct WinInfoTyp
{
    unsigned        Idx;                // window index (count from 0)
    unsigned        Type;               // window type (0=input, 1=output)
    unsigned        NumOfPkts;          // number of packets in the window

    unsigned        PktSize;            // size of each packet (bytes)
    unsigned        DescBufSize;        // size of the descriptor ring buffer (bytes)

    uint32_t*       DmaBufPtr;          // pointer to the dma buffer
    uint32_t*       DescBufPtr;         // pointer to the descriptor ring buffer

    off_t           DmaBufAddr;         // physical address of the dma buffer
    off_t           DescBufAddr;        // physical address of the descriptor ring buffer
};

struct LinkInfoTyp
{
    unsigned        LinkId;             // global link id (count from 1)
    unsigned        DeviceId;           // device id (count from 1)
    unsigned        ChannelId;          // channel id (count from 1)
    unsigned        NumOfWin;           // number of windows
};


class cHssl
{
public:
    cLinkIdInfo     LinkIdInfo;
    
    int   Open( LinkInfoTyp& LinkInfo, const char* Uniq, int& RetErrCode )  {return SUCCESS;}
    int   Close( int& RetErrCode )                                          {return SUCCESS;}
    int   ConfigureWindow( WinInfoTyp& WinInfo, int& RetErrCode )           {return SUCCESS;}
    int   GetPhysicalAddress( uint32_t* Ptr, off_t& Addr, int& RetErrCode ) {Addr=NULL; return SUCCESS;}
    int   EnableLinkIdTx( int& RetErrCode )                                 {return SUCCESS;}
    int   DisableLinkIdTx( int& RetErrCode )                                {return SUCCESS;}
    int   LoadTargetFpga( char* filename, int& RetErrCode )                 {return SUCCESS;}
    int   SetCurrentHostState( unsigned NewState, int& RetErrCode )         {return SUCCESS;}
    int   GetCurrentHostState( unsigned& HostState, int& RetErrCode )       {return SUCCESS;}
    int   GetCurrentTargetState( unsigned& TargState, int& RetErrCode )     {return SUCCESS;}
    int   EnableWinTransfers( unsigned WinIdx, int& RetErrCode )            {return SUCCESS;}
    int   DisableWinTransfers( unsigned WinIdx, int& RetErrCode )           {return SUCCESS;}
    int   DemandWinTransfer( unsigned WinIdx, int& RetErrCode )             {return SUCCESS;}
    int   ResetLink( int& RetErrCode )                                      {return SUCCESS;}
    bool  ResetLinkIsActive( void )                                         {return false;}
    bool  IsLinkOk( void )                                                  {return true;}
};



#endif   //  __X0HSSL_H__
