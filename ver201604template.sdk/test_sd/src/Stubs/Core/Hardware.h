#ifndef __HARDWARE_H__
#define __HARDWARE_H__


#include "GeDString.h"


enum ControllerIdEnum
{
    ID_UNKNOWN = 0,
    ID_LOCAL   = 0,
    ID_R       = 1,
    ID_S       = 2,
    ID_T       = 3,
    MAX_CONTROLLER_ID = 3
};

enum CardTypeEnum
{
    CARD_UNKNOWN       = 0,
    CARD_MAIN          = 1,
    CARD_ACQUISITION   = 2,
    CARD_TERMINAL      = 3
};

enum PlatformTypeEnum
{
    ERROR_PLATFORM = -1,
    NO_PLATFORM    = 0,

    X86_PLATFORM   = 1,         // Generic Hardware, Forced type

    UCVE_PLATFORM  = 2,         // 300MHz VME  x86
    UCVF_PLATFORM  = 3,         // 850MHZ VME  x86
    UCVG_PLATFORM  = 4,         // 650MHZ VME  x86

    BPPB_PLATFORM  = 5,         // 266MHz ---  MIPS

    UCCA_PLATFORM  = 6,         // 650MHz CPCI x86
    UCCC_PLATFORM  = 7,         // 1.6GHz CPCI x86

    ACLE_PLATFORM  = 8,         // 266MHz ACL  x86

    UCSA_PLATFORM  = 9,         // Frescale 8349 (400 MHz PPC)

    UCSB_PLATFORM  = 10,        // Intel Tolopai (1200/600 MHz x86)

    BPPC_PLATFORM = 11,         // Freescale MPC8308 (330MHz PPC)

    BPPD_PLATFORM = 12,			// Freescale MPC8343E (396 MHz PPC)
};

class cHardwareBase  
{
public:
    static PlatformTypeEnum DeterminePlatformType(cDString *pErrStr = NULL,cDString *pName = NULL)  { return UCSB_PLATFORM;}
};



#endif  // __HARDWARE_H__

