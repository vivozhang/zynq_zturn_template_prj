#ifndef __X0LINKIDINFO_H__
#define __X0LINKIDINFO_H__


#include "Hardware.h"
#include "x0FrameWork.h"


class cLinkIdInfo
{
public:
    struct IdRecordTyp
    {
        int         BusNo;                  // dallas id bus number, 0 reserved for link id (hssl babble), others defined by specific product
        uint32_t    SerialNumberLow;        // serial number low word
        uint16_t    SerialNumberHigh;       // serial number high word
        int         CardIndex;              // used to order the Id records that are submitted to HW
        CardTypeEnum CardType;              // card type
        char        pBarCode[8];            // bar code                   (6-7 chars + null term)
        char        pCatNo[21];             // catalog number             (8-20 chars + null term)
        char        pCardName[8];           // card name                  (7 chars + null term)
        char        pCardBaseName[5];       // card name                  (4 chars + null term)
        char        pConnName[4];           // connector name             (3 chars + null term)
        char        pDsr[12][6];            // 12 design specific records (1-5 chars + null term)
    };
    
    int   DecodeBab( int& RetErrCode )                                      {return SUCCESS;}
    int   DecodeAux( char* pBuf, int DataLen, int BusNo, int& RetErrCode )  {return SUCCESS;}
    int   SetConnName( const char* pConnName, int PwaIdx )                  {return SUCCESS;}
    int   SetCardIndx( int CardIndex, int PwaIdx )                          {return SUCCESS;}
    int   SetCardType( CardTypeEnum CardType, int PwaIdx )                  {return SUCCESS;}
    int   SetHwInventory( void )                                            {return SUCCESS;}
    int   ClearHwInventory( void )                                          {return SUCCESS;}
    int   GetNumIds( void )                                                 {return 0;}
    int   GetPwaIds( IdRecordTyp* pIds, int NumIds )                        {return SUCCESS;}
    int   GetAppId( cDString& AppId )                                       {return SUCCESS;}
};    







#endif //  __X0LINKIDINFO_H__
