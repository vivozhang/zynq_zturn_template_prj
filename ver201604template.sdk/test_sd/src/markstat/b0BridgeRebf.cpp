///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rebf Based Bridge Interface
//
// DESCRIPTION:
//      This file defines the Rebf based bridge interface class
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0BridgeRebf.vsd, b0BridgeRebf.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
#include <sys/neutrino.h>
// core
#include "Hardware.h"
#include "GeSList.h"
// framework
#include "x0Err.h"
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "b0BridgeRebf.h"
#include "b0BrgFbkOffs.h"


// Constants
//----------

// event scheduling constants
#define FRC_CLK                         (25.0e6)                    // fpga timebase clock frequency
#define FRC_CLK_RCP                     (1.0/FRC_CLK)               // fpga timebase clock period
#define FRC_SCHED_QUANT                 (0x4)                       // least significant 2 bits of frc times not used by fpga
#define FRC_SCHED_MASK                  (~(FRC_SCHED_QUANT-1))      // evenly divisible by FRC_SCHED_QUANT
#define FRC_SCHED_MASK16                (FRC_SCHED_MASK & 0xffff)   // 16 bits, evenly divisible by FRC_SCHED_QUANT
#define FRC_SCHED_MAX                   (0x7FFF & FRC_SCHED_MASK16) // furthest away an event can be scheduled
#define MIN_CNT_AFTER_LDPLS             (7*FRC_SCHED_QUANT)         // soonest we can schedule a gate event after the load pulse-nust be at least 1 usec after load pulse (TT)

// bridge interface layer (bil) constants
#define BIL_CNT_MIN                     (0)
#define BIL_CNT_MAX_4                   ((1<< 4)-1)
#define BIL_CNT_MAX_8                   ((1<< 8)-1)
#define BIL_CNT_MAX_9                   ((1<< 9)-1)
#define BIL_CNT_MAX_12                  ((1<<12)-1)
#define BIL_CNT_MAX_16                  ((1<<16)-1)
#define BIL_SCL_TM_40                   (1.0/   40.0e-9)            // FRC_CLK /    1.0
#define BIL_SCL_TM_160                  (1.0/  160.0e-9)            // FRC_CLK /    4.0
#define BIL_SCL_TM_5120                 (1.0/ 5120.0e-9)            // FRC_CLK /  128.0
#define BIL_SCL_TM_40960                (1.0/40960.0e-9)            // FRC_CLK / 1024.0
#define BIL_SCL_V                       (255.0)
#define BIL_SCL_I                       (255.0)
#define BIL_SCL_C                       (1.0)
#define BIL_OFS_V                       (0)
#define BIL_OFS_I                       (0)
#define BIL_OFS_T                       (0)
#define BIL_OFS_C                       (0)

// gate monitor bit definitions
#define GATE_MON_REGATE                 (0x0001)        // ignore (normally occurrs)
#define GATE_MON_IOERR                  (0x0002)
#define GATE_MON_SHTHRU                 (0x0004)
#define GATE_MON_PPULERR                (0x0008)
#define GATE_MON_PULERR                 (0x0010)
#define GATE_MON_TSKWERR                (0x0020)
#define GATE_MON_TOFFERR                (0x0040)        // ignore (always get on line conv when re-strike enabled)
#define GATE_MON_TONERR                 (0x0080)
#define GATE_MON_ERROR                  (0x4000)
#define GATE_MON_RUNNING                (0x8000)
#define GATE_MON_CARE_ABOUT             (0x00BE)        // depricated, replaced by EE

// defines for hw faults
#define BRSTATFLT_GD_FLT_ANY_MASK   (0x0023F023F)
#define STATCOM_GD_PWR_FLT_MASK     (0x00000060)
//#define BRSTATFLT_IOC_FLT_MASK      (0x01C001C0)


#if 0   // THIS IS NOW AN ALARM, ALSO BELIEVE THERE IS A BUG IN THE FPGA CODE THAT MAKES THIS SIGNAL BEHAVE BADLY (JMN)
#define STATCOM_TARG_FRC_FLT_MASK   (0x00000C04)
#endif
#define STATCOM_TARG_FRC_FLT_MASK   (0x00000404)

// task1 passes to periodically issue flt reset to fpga
#define BRIDGE_FAULT_RESET_DLY          (20)

// hssl link error (trip) flags
#define HOST_RX_FBK_NOT_ON_TIME         (0x00000010)    // trip, monitored in task1
#define HOST_LINK_LOST                  (0x10000000)    // trip, monitored in task3
#define HOST_NOT_RUNNING_STATE          (0x20000000)    // trip, monitored in task3
#define TARG_NOT_RUNNING_STATE          (0x40000000)    // trip, monitored in task3

// hssl link alarm flags
#define TARG_RX_BAD_CRC                 (0x00000001)    // alarm
#define TARG_NAKS_ON_TX                 (0x00000002)    // alarm
#define TARG_RX_BAD_HDR                 (0x00000004)    // alarm

// hssl discover error flags
#define TARG_DSC_INITIAL_STATE_FAIL     (0x00000001)    // trip
#define TARG_DSC_TARG_ID_READ_FAIL      (0x00000002)    // trip
#define TARG_DSC_TARG_HW_VALIDATE_FAIL  (0x00000004)    // trip
#define TARG_DSC_TARG_FPGA_PROG_FAIL    (0x00000008)    // trip
#define TARG_DSC_TARG_FPGA_WRONG_VER    (0x00000010)    // trip
#define TARG_DSC_TARG_ADV_TO_CFG_FAIL   (0x00000020)    // trip
#define TARG_DSC_TARG_FRC_SYNC_FAIL     (0x00000040)    // trip
#define TARG_DSC_LINK_ADV_TO_RUN_FAIL   (0x00000100)    // trip
#define TARG_DSC_HOST_WIN_INI_FAIL      (0x00000200)    // trip
#define TARG_DSC_INV_STATE              (0x10000000)    // trip

// target initialization error flags
#define TARG_INI_ERR_FPGA_REV           (0x00000001)
#define TARG_INI_CFG_ENB_FAIL           (0x00000002)
#define TARG_INI_CFG_DIS_FAIL           (0x00000004)
#define TARG_INI_READBACK_FAIL          (0x00000008)
#define TARG_INI_READBACK_VAL_BAD       (0x00000010)
#define TARG_INI_ERR_WIN_ENB            (0x00000100)
#define TARG_INI_AUX1_ID_INV_SIZE       (0x00001000)
#define TARG_INI_AUX2_ID_INV_SIZE       (0x00002000)
#define TARG_INI_AUX1_ID_DECODE_FAIL    (0x00003000)
#define TARG_INI_AUX2_ID_DECODE_FAIL    (0x00004000)
#define TARG_INI_REPI_ID_DECODE_FAIL    (0x00005000)
#define TARG_INI_RIB1_ID_DECODE_FAIL    (0x00006000)
#define TARG_INI_RIB2_ID_DECODE_FAIL    (0x00007000)
#define TARG_INI_RIB3_ID_DECODE_FAIL    (0x00008000)
#define TARG_INI_RIB4_ID_DECODE_FAIL    (0x00009000)
#define TARG_INI_AUX_ID_VALIDATE_FAIL   (0x00010000)
#define TARG_INI_INV_STATE              (0x10000000)

// max number of pwas recorded per hssl link
#define PWA_PER_LINK                    (4)

// Rebf low voltage relay state mapping
#define REBF_LV_RELAY02                 (1)
#define REBF_LV_RELAY03                 (2)

#define VFB_OFFSET                      ((1000000.0-976800.977)*(2375+6.9)/6.9/2000000)         // voltage feedback offset voltage


// Externals
//----------
extern int BufferDump ( cParamString& OutputBuffer, char* pBuf, int DataLen );

extern cMonitorCommand      gMonitor;

extern unsigned DsimMode;

// Types and Classes
//------------------


// Internal Function Prototypes
//-----------------------------


// Published Variables
//--------------------


// Unpublished Variables
//----------------------
cSLinkedList <cBridgeRebf*> BridgeRebfList;         // linked list of cBridgeRebf class instance pointers


// Static Class Members (init'd to zero by default)
//-------------------------------------------------
const float     cBridgeRebf::FrcClk             = (float)FRC_CLK;
const unsigned  cBridgeRebf::FrcSchedMax        = (unsigned)FRC_SCHED_MAX;
const unsigned  cBridgeRebf::FrcSchedMask       = (unsigned)FRC_SCHED_MASK;
const float     cBridgeRebf::VfbOffset          = (float)VFB_OFFSET;

const double    cBridgeRebf::VcoFzro            = BRG_VCO_FZRO;
const double    cBridgeRebf::VcoFmax            = BRG_VCO_FMAX;
//const double    cBridgeRebf::VcoFmaxREBH        = BRG_VCO_REBH_FMAX;
const double    cBridgeRebf::VcoFmin            = BRG_VCO_FMIN;
const unsigned  cBridgeRebf::VcoMask            = (unsigned)BRG_VCO_MASK;
const unsigned  cBridgeRebf::FrcSchedMask16     = (unsigned)FRC_SCHED_MASK16;

unsigned        cBridgeRebf::TotalInstances     = 0;
unsigned        cBridgeRebf::IoThrmstrCnts1Sim  = 4600;
unsigned        cBridgeRebf::IoThrmstrCnts2Sim  = 826;


// offsets into the fpga cmd structure
#if 0  // later read back configuration registers
const size_t    cBridgeRebf::CmdReadBackOffs[REBx_NUM_OF_CONFIG_REGS] =
{
    offsetof(cBridgeRebf::CmdWinTyp, Config1Reg),
    offsetof(cBridgeRebf::CmdWinTyp, Config2Reg),
    offsetof(cBridgeRebf::CmdWinTyp, Config3Reg),
    offsetof(cBridgeRebf::CmdWinTyp, Config4Reg),
    offsetof(cBridgeRebf::CmdWinTyp, Config5Reg),
    offsetof(cBridgeRebf::CmdWinTyp, Config6Reg),
    offsetof(cBridgeRebf::CmdWinTyp, Config7Reg),
    offsetof(cBridgeRebf::CmdWinTyp, Config8Reg),
};
#endif

// constants for the BIL register calculations                      { mincnt,       maxcnt,          offcnt,     sclcnt           }
const cBridgeRebf::BilCalcTyp   cBridgeRebf::BilCalc_LockOutTm    = { BIL_CNT_MIN,  BIL_CNT_MAX_9,   BIL_OFS_T,  BIL_SCL_TM_40    };


//-----------------------------------------------------------------------------
// target fpga application image file and reported versions for HSLA H1
const char*     cBridgeRebf::HslaH1ApplFile       = "hslaH1_Rebf.bin";

// target fpga application image file and reported versions for REBH
const char*     cBridgeRebf::HslApplFile          = "rebha_00.bit";


//  - 16012960_06 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF06";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x16012960;

//  - 16050270_000 new FPGA code for 2.3x program for REBH
const char*     cBridgeRebf::HslApplVer           = "REBH000";
const unsigned  cBridgeRebf::HslApplRevCode       = 0x16050270;

#if 0
//  - 16041970_020 new FPGA code for 2.3x program for REBH
const char*     cBridgeRebf::HslApplVer           = "REBH020";
const unsigned  cBridgeRebf::HslApplRevCode       = 0x16041970;

//  - 16041570_010 new FPGA code for 2.3x program for REBH
const char*     cBridgeRebf::HslApplVer           = "REBH010";
const unsigned  cBridgeRebf::HslApplRevCode       = 0x16041570;

//  - 16022370_000 new FPGA code for 2.3x program for REBH
const char*     cBridgeRebf::HslApplVer           = "REBH000";
const unsigned  cBridgeRebf::HslApplRevCode       = 0x16022370;

//  - 16021770_010 new FPGA code for 2.3x program for REBH
const char*     cBridgeRebf::HslApplVer           = "REBH010";
const unsigned  cBridgeRebf::HslApplRevCode       = 0x16021770;

//  - 16012670_000 new FPGA code for 2.3x program for REBH
const char*     cBridgeRebf::HslApplVer           = "REBH000";
const unsigned  cBridgeRebf::HslApplRevCode       = 0x16012670;

//  - 14061761_04 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF05";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x14073060;

//  - 16012570_010 new FPGA code for 2.3x program for REBH
const char*     cBridgeRebf::HslApplVer           = "REBH010";
const unsigned  cBridgeRebf::HslApplRevCode       = 0x16012570;

//  - 16012170_020 new FPGA code for 2.3x program for REBH
const char*     cBridgeRebf::HslApplVer           = "REBH020";
const unsigned  cBridgeRebf::HslApplRevCode       = 0x16012170;

//  - 15102170_01 new FPGA code for 2.3x program for REBH
const char*     cBridgeRebf::HslApplVer           = "REBH000";
const unsigned  cBridgeRebf::HslApplRevCode       = 0x15102170;

//  - 14061761_04 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF04";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x14061761;

//  - 14061760_04 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF04";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x14061760;

//  - 14061660_04 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF04";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x1406166

//  - 14061360_04 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF04";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x14061360;

//  - 14061160_04 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF04";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x14061160;

//  - 14060660_04 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF04";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x14060660;

//  - 14052960_04 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF04";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x14052960;

//  - 14050760_03 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF03";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x14050760;

//  - 14041160_02 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF02";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x14041160;

//  - 14040360_01 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF01";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x14040360;

//  - 14030460_00 new FPGA code for 2.3x program
const char*     cBridgeRebf::HslaH1ApplVer        = "REBF00";
const unsigned  cBridgeRebf::HslaH1ApplRevCode    = 0x14030460;

#endif


//-------------------------------------------------------------------------
// rebf connector names
const char* cBridgeRebf::NULL_Str = "";

const char* cBridgeRebf::J1_Str = "J1";
const char* cBridgeRebf::J2_Str = "J2";

const char* cBridgeRebf::P1_Str   = "P1";
const char* cBridgeRebf::P2_Str   = "P2";
const char* cBridgeRebf::REPI_Str = "R01";
const char* cBridgeRebf::J01_Str  = "J01";
const char* cBridgeRebf::J02_Str  = "J02";
const char* cBridgeRebf::J03_Str  = "J03";
const char* cBridgeRebf::J04_Str  = "J04";

const char* cBridgeRebf::J06_Str  = "J06";
const char* cBridgeRebf::J07_Str  = "J07";
const char* cBridgeRebf::J08_Str  = "J08";


const char* cBridgeRebf::JA1_Str = "JA1";
const char* cBridgeRebf::JA2_Str = "JA2";

//-------------------------------------------------------------------------
// required revisions for cards supported by this class
const char* cBridgeRebf::Reqd_HSLA_CatNum_Str[]    = { "IS210HSLAH1A", "IS200HSLAH1A"};

const char* cBridgeRebf::Reqd_REBx_CatNum_Str[]    = { "IS200REBFH1B", "IS400REBHH1A"};       // Required, if not prototype
const char* cBridgeRebf::Reqd_REBx_CatNum_Str_Pr[] = { "IS200REBFH1BPR1", "IS400REBHH1APR1"};   // Required, if prototype
const char* cBridgeRebf::REBx_CatNum_Str_Pr        = { "IS400REBHH1APR"};    // Prototype (generic)

const char* cBridgeRebf::Reqd_REPI_H1_CatNum_Str[]    = { "IS200REPIH1"};       // Required, if not prototype
const char* cBridgeRebf::Reqd_REPI_H1_CatNum_Str_Pr[] = { "IS200REPIH1APR1", "IS400REPIH1APR2" };   // Required, if prototype
const char* cBridgeRebf::REPI_H1_CatNum_Str_Pr   = { "IS200REPIH1APR"};    // Prototype (generic)

const char* cBridgeRebf::Reqd_AEDB_H1_CatNum_Str[] = { ""};
const char* cBridgeRebf::Reqd_AEDB_H2_CatNum_Str[] = { ""};
const char* cBridgeRebf::Reqd_AEDB_H3_CatNum_Str[] = { ""};
const char* cBridgeRebf::Reqd_AEDB_H4_CatNum_Str[] = { "IS210AEDBH4A" };

const char* cBridgeRebf::Reqd_AEBI_H1_CatNum_Str[] = { ""};
const char* cBridgeRebf::Reqd_AEBI_H2_CatNum_Str[] = { ""};
const char* cBridgeRebf::Reqd_AEBI_H3_CatNum_Str[] = { "IS210AEBIH3B" };

const char* cBridgeRebf::Reqd_AEAA_H1_CatNum_Str[] = { ""};
const char* cBridgeRebf::Reqd_AEAA_H2_CatNum_Str[] = { ""};
const char* cBridgeRebf::Reqd_AEAA_H3_CatNum_Str[] = { "IS210AEAAH3B"};
const char* cBridgeRebf::Reqd_AEAA_H4_CatNum_Str[] = { ""};

const char* cBridgeRebf::Reqd_AEAD_H1_CatNum_Str[] = { ""};
const char* cBridgeRebf::Reqd_AEAD_H2_CatNum_Str[] = { ""};
const char* cBridgeRebf::Reqd_AEAD_H3_CatNum_Str[] = { "IS200AEADH3A" };
const char* cBridgeRebf::Reqd_AEAD_H4_CatNum_Str[] = { "IS200AEADH4A" };

const char* cBridgeRebf::Reqd_RGIB_H1_CatNum_Str[] = { "IS400RGIBH1A" };



const char* cBridgeRebf::FbkWinRegNames[] =
{
    "RevCode",
    "VcoTemp21",
    "VcoBr1IaIb",
    "VcoBr1IcVab",
    "VcoBr1VbcVdc",
    "StatCom",
    "BrStatFlt",
    "VcoIo1A01A02",
    "VcoIo1A03A04",
    "VcoIo1A05A06",
    "VcoIo1A07spr",
    "VcoVdb",
    "StatReg21",
    "TempRegEngUseOnly",
    "Tach01TV",
    "Tach01MP",
    "CrLb",
    "MiscStat",
    "SDL21",
    "SDL43",
    "SDL5Vdc",
    "PktCntRb"
    //"SdlST"
};

const char* cBridgeRebf::CmdWinRegNames[] =
{
    "PktCnt",
    "LoadPulseTimeReg",
    "Brg1PhsTraA0",
    "Brg1PhsTraB0",
    "Brg1PhsTraC0",
    "Brg1PhsTraA1",
    "Brg1PhsTraB1",
    "Brg1PhsTraC1",
    "Brg1PhsTraA2",
    "Brg1PhsTraB2",
    "Brg1PhsTraC2",
    "Brg1PhsTraA3",
    "Brg1PhsTraB3",
    "Brg1PhsTraC3",
    "Brg1PhsTraA4",
    "Brg1PhsTraB4",
    "Brg1PhsTraC4",
    "SpareLocX044",
    "CrConfig",
    "Cr21",
    "FanRef"
};


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Constructor for the class.  This function initalizes all of
//      member variables.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
cBridgeRebf::cBridgeRebf
(
    InTyp    In,
    InsTyp   Ins
)
    : In(In), Ins(Ins)
{

    //-------------------------------------------------------------------------
    // output signals
    OUT(IaBrg1Hdw)       = 0.0;
    OUT(IbBrg1Hdw)       = 0.0;
    OUT(IcBrg1Hdw)       = 0.0;
    OUT(IdBrg1Hdw)       = 0.0F;

    OUT(VabBrg1Hdw)      = 0.0;
    OUT(VbcBrg1Hdw)      = 0.0;

    OUT(VdBrg1Hdw)       = 0.0;
    OUT(VdcFiberBrgHdw)  = 0.0;


    OUT(AmbientTemp)    = 0.0;

    OUT(ContactIn1Io1)     = false;
    OUT(ContactIn2Io1)     = false;
    OUT(ContactIn3Io1)     = false;
    OUT(ContactIn4Io1)     = false;
    OUT(ContactIn5Io1)     = false;
    OUT(ContactIn6Io1)     = false;
    OUT(ContactIn7Io1)     = false;
    OUT(ContactIn8Io1)     = false;
    OUT(ContactIn9Io1)     = false;
    OUT(ContactIn10Io1)    = false;

    OUT(HsslCommFlt)       = false;
    OUT(BrgHdwDataVld)     = false;

    OUT(Brg1PhDenabled)    = false;

    OUT(VaxIo1)         = 0.0;
    OUT(VbxIo1)         = 0.0;
    OUT(VcxIo1)         = 0.0;
    OUT(IaIo1)          = 0.0;
    OUT(IbIo1)          = 0.0;
    OUT(IcIo1)          = 0.0;

    OUT(ThermistorVcoIo1) = 0;
    OUT(BrgTempVcoSel) = 0;
    OUT(CellTempVcoBrg1) = 0;

    OUT(TachValue)      = 0;
    OUT(TachMrkrValue)  = 0;

    OUT(HsslCmdMrg)     = 0.0;
    OUT(HsslCmdMrgMin)  = 1000.0;
    OUT(FpgaFltLatch)   = false;
    memset( (void*)&Brg1DsatIoc, 0, sizeof(Brg1DsatIoc) );

    DisCmdNotFlg = false;
    BrlDisDlyTmr = 0.0F;

        OUT(SimSpdEncLatchCnt)  = 0;
    OUT(SimSpdEncDelCntRes) = 0.0F;
    OUT(SimBrgFbkTachV)  = 0.0F;

    //-------------------------------------------------------------------------
    // private signals

    AeadGnCalcEnb   = false;
    AeadGnCalcDone  = false;
    AeaaGnCalcEnb   = false;
    AeaaGnCalcDone  = false;

    HostIsOpen      = false;

    HsslIsEnabled   = false;
    FbkCalcFstpDone = false;

    TargDscCase     = -1;
    TargDscPass     = 0;
    TargDscDone     = false;

    TargInitCase    = -1;
    TargInitPass    = 0;
    TargInitCnt     = 0;
    TargSdlNum      = 0;
    TargInitDone    = false;

    TargDscReq      = false;

    HsslErrChksEnb  = false;

    HsslErrFlags    = 0;
    HsslErrFlagsT3  = 0;
    HsslErrFlagsAcc = 0;
    HsslErrCnt      = 0;

    HsslErrFlagsDsc = 0;
    HsslErrFlagsIni = 0;

    HsslAlmFlagsAcc = 0;

    HsslFbkWaitMin  = 1000.0F;
    HsslFbkWaitMax  = 0.0F;

    HsslFbkWaitLdPls2 = 0.0F;
    LdPls2EvtFlg      = false;
    LdPls2EvtCtr      = 0;
    TargReprogCtr   = 0;

    CmdPktCnt = 0;

    HsslFbkLateCtr = 0;
    HslaApplRevCode = HslaH1ApplRevCode;
    HsslApplRevCode = HslApplRevCode;


    HsslHostState   = Hssl_HOST_NO_LINK;
    HsslTargState   = Hssl_TARGET_NO_LINK;

    ContactIn6Z = false;

    Brg1EnabCtr = 0;

    BrgFaultActZ     = false;
    BrgFaultResolved = false;

    SimModeBrgEnabled = false;
    SimModeSdlEnabled = false;
    TargetHdwExpected = false;

    Brg1TransList.Len    = ARRAY_LEN( Brg1TransArr );
    Brg1TransList.pTrans = (BrgTransTyp(*)[])Brg1TransArr;

    TargRxErrCnt     = 0;
    TargTxErrCnt     = 0;
    TargPktIgnoreCnt = 0;

    BrgTstModeState  = 0;
    Brg1EnbMisMatch  = 0;
    Brg1DsbMisMatch  = 0;

    HsslCommFltRstTmr = 0.0F;

    Io1ContInp03Latch = false;

    //-------------------------------------------------------------------------
    // get the physical addresses for the hssl window buffers
    if ( Hssl.GetPhysicalAddress( (uint32_t*)&BrgCmd, BrgCmdAddr, ErrCode ) != SUCCESS )
    {
        // INIT FAIL DIAGNOSTIC
    }
    if ( Hssl.GetPhysicalAddress( (uint32_t*)&BrgFbk, BrgFbkAddr, ErrCode ) != SUCCESS )
    {
        // INIT FAIL DIAGNOSTIC
    }
    if ( Hssl.GetPhysicalAddress( (uint32_t*)&BrgFbkAlt, BrgFbkAltAddr, ErrCode ) != SUCCESS )
    {
        // INIT FAIL DIAGNOSTIC
    }
    for ( unsigned buf = 0 ; buf < REBx_DIAG_BUF_NUM ; ++buf )
    {
        for ( unsigned frm = 0 ; frm < REBx_DIAG_FRM_NUM ; ++frm )
        {
            if ( Hssl.GetPhysicalAddress( (uint32_t*)&BrgDiagArr[buf][frm], BrgDiagArrAddr[buf][frm], ErrCode ) != SUCCESS )
            {
                // INIT FAIL DIAGNOSTIC
            }
        }
    }

    memset( (void*)&BrgCmd,           0, sizeof(BrgCmd)     );
    memset( (void*)&BrgFbk,           0, sizeof(BrgFbk)     );
    memset( (void*)&BrgFbkAlt,        0, sizeof(BrgFbkAlt)  );
    memset( (void*)&BrgDiagArr[0][0], 0, sizeof(BrgDiagArr) );

    //-------------------------------------------------------------------------
    // link info
    LinkInfo.LinkId    = 0;                     // will be filled out later by the Open method
    LinkInfo.DeviceId  = 0;                     // will be filled out later by the Open method
    LinkInfo.ChannelId = 0;                     // will be filled out later by the Open method
    LinkInfo.NumOfWin  = ARRAY_LEN(WinInfo);

    memset( IdBufAux1, 0, sizeof(IdBufAux1) );
    memset( IdBufAux2, 0, sizeof(IdBufAux2) );

    IdBufAux1DataLen = 0;
    IdBufAux2DataLen = 0;

    //-------------------------------------------------------------------------
    // command window info
    WinInfo[REBF_WIN_CMD].Idx         = REBF_WIN_CMD;
    WinInfo[REBF_WIN_CMD].Type        = Hssl_WIN_TYPE_OUT;
    WinInfo[REBF_WIN_CMD].NumOfPkts   = 1;
    WinInfo[REBF_WIN_CMD].PktSize     = sizeof(REBx_CmdWinTyp);
    WinInfo[REBF_WIN_CMD].DmaBufPtr   = (uint32_t*)&BrgCmd;
    WinInfo[REBF_WIN_CMD].DmaBufAddr  = BrgCmdAddr;
    WinInfo[REBF_WIN_CMD].DescBufSize = 0;      // descriptors not used since output window
    WinInfo[REBF_WIN_CMD].DescBufPtr  = NULL;
    WinInfo[REBF_WIN_CMD].DescBufAddr = NULL;

    //-------------------------------------------------------------------------
    // feedback window info
    WinInfo[REBF_WIN_FBK].Idx         = REBF_WIN_FBK;
    WinInfo[REBF_WIN_FBK].Type        = Hssl_WIN_TYPE_IN;
    WinInfo[REBF_WIN_FBK].NumOfPkts   = 1;
    WinInfo[REBF_WIN_FBK].PktSize     = sizeof(REBx_FbkWinTyp);
    WinInfo[REBF_WIN_FBK].DmaBufPtr   = (uint32_t*)&BrgFbk;
    WinInfo[REBF_WIN_FBK].DmaBufAddr  = BrgFbkAddr;
    WinInfo[REBF_WIN_FBK].DescBufSize = 0;      // descriptors not used since only 1 packet
    WinInfo[REBF_WIN_FBK].DescBufPtr  = NULL;
    WinInfo[REBF_WIN_FBK].DescBufAddr = NULL;

    //-------------------------------------------------------------------------
    // diagnostic window info
    WinInfo[REBF_WIN_DIAG].Idx         = REBF_WIN_DIAG;
    WinInfo[REBF_WIN_DIAG].Type        = Hssl_WIN_TYPE_IN;
    WinInfo[REBF_WIN_DIAG].NumOfPkts   = 1;
    WinInfo[REBF_WIN_DIAG].PktSize     = sizeof(REBx_DiagWinTyp);
    WinInfo[REBF_WIN_DIAG].DmaBufPtr   = (uint32_t*)&BrgDiagArr[0][0];
    WinInfo[REBF_WIN_DIAG].DmaBufAddr  = BrgDiagArrAddr[0][0];
    WinInfo[REBF_WIN_DIAG].DescBufSize = 0;     // descriptors not used since only 1 packet
    WinInfo[REBF_WIN_DIAG].DescBufPtr  = NULL;
    WinInfo[REBF_WIN_DIAG].DescBufAddr = NULL;
    HwFdbkTxDlyFrc  = 0;

    LlmCmdShdwCapReq = false;
    memset( (void*)&LlmCmdShdw, 0, sizeof(LlmCmdShdw) );

    LlmFbkShdwCapReq = false;
    memset( (void*)&LlmFbkShdw, 0, sizeof(LlmFbkShdw) );

    FbkSampFrcTime = 0;

    // residualization variables for float-to-vco conversion
    IaVcoBr1LatchCnt = 0;
    IbVcoBr1LatchCnt = 0;
    IcVcoBr1LatchCnt = 0;
    IdVcoBr1LatchCnt = 0;

    VaxVcoBr1LatchCnt = 0;
    VbxVcoBr1LatchCnt = 0;
    VcxVcoBr1LatchCnt = 0;
    VdcVcoBr1LatchCnt = 0;

    VdcFiberVcoLatchCnt = 0;

    IaVcoAin1LatchCnt = 0;
    IbVcoAin1LatchCnt = 0;
    IcVcoAin1LatchCnt = 0;

    VaxVcoAin1LatchCnt = 0;
    VbxVcoAin1LatchCnt = 0;
    VcxVcoAin1LatchCnt = 0;

    IaVcoAin2LatchCnt = 0;
    IbVcoAin2LatchCnt = 0;
    IcVcoAin2LatchCnt = 0;

    VaxVcoAin2LatchCnt = 0;
    VbxVcoAin2LatchCnt = 0;
    VcxVcoAin2LatchCnt = 0;

    IaVcoBr1DelCntRes = 0.0;
    IbVcoBr1DelCntRes = 0.0;
    IcVcoBr1DelCntRes = 0.0;
    IdVcoBr1DelCntRes = 0.0;

    VaxVcoBr1DelCntRes = 0.0;
    VbxVcoBr1DelCntRes = 0.0;
    VcxVcoBr1DelCntRes = 0.0;
    VdcVcoBr1DelCntRes = 0.0;

    VdcFiberVcoDelCntRes = 0.0;

    IaVcoAin1DelCntRes = 0.0;
    IbVcoAin1DelCntRes = 0.0;
    IcVcoAin1DelCntRes = 0.0;

    VaxVcoAin1DelCntRes = 0.0;
    VbxVcoAin1DelCntRes = 0.0;
    VcxVcoAin1DelCntRes = 0.0;

    IaVcoAin2DelCntRes = 0.0;
    IbVcoAin2DelCntRes = 0.0;
    IcVcoAin2DelCntRes = 0.0;

    VaxVcoAin2DelCntRes = 0.0;
    VbxVcoAin2DelCntRes = 0.0;
    VcxVcoAin2DelCntRes = 0.0;

    Reqd_HslaConn_Str = NULL_Str;

    Reqd_Brg1Infc_CatNum_Str = &NULL_Str;
    Reqd_Brg1Infc_CatNum_ArrLen = 0;

    Reqd_Io1Infc_CatNum_Str = &NULL_Str;
    Reqd_Io1Infc_CatNum_ArrLen = 0;

    ClearHwInventoryReq = false;
    TaskBkgndRst        = false;
    PhsDdsatFltLatch    = false;

    // instance identity
    ++TotalInstances;                   // this is shared by all class-instances
    InstNumber  = TotalInstances-1;     // instance number starting at zero
    Uniq[0] = 0;

    // Thermistor simulator
    OUT(TempVco1stSmpl)     = false;
    OUT(TempVco2ndSmpl)     = false;
    ThrmHiSelSim       = (strncmp(INS(Uniquifier), "L", 1) == 0) ? true:false;  // Init as 'true' for Line
    TempVco1stCnt      = 0;
    TempVco2ndCnt      = 0;
    ThrmstrBrgCnts     = 0;
    ThrmCoolntLwCntsZ1 = 0;
    ThrmHxinLwCntsZ1   = 0;
    ThrmCoolntHiCntsZ1 = 0;
    ThrmHxinHiCntsZ1   = 0;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Destructor for the class.  This function removes any allocated
//      memory held by this class
//
// EXECUTION LEVEL
//      Shutdown
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
cBridgeRebf::~cBridgeRebf(void)
{
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Open the HSSL requested.
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      1 = success, 0 = failure
//
///////////////////////////////////////////////////////////////////////////////
int cBridgeRebf::Open (
    unsigned            LinkIdReq,                  // requested link number (count from 1)
    unsigned            DevIdReq,                   // requested device id (count from 1)
    unsigned            ChanIdReq                   // requested channel id (count from 1)
)
{
    int                 ErrCode = 0;                // error code local to this function

    //-------------------------------------------------------------------------
    // check to make sure the host has not already been opened
    if ( HostIsOpen )
    {
        printf("Hssl device %d channel %d already open\n", DevIdReq, ChanIdReq);
        return FAIL;
    }

    //-------------------------------------------------------------------------
    // check to make sure the link has been enabled
    if ( !HsslIsEnabled )
    {
        HostIsOpen = false;
        printf("Hssl device %d channel %d is disabled\n", DevIdReq, ChanIdReq);
        return FAIL;
    }

    //-------------------------------------------------------------------------
    // use an alternate buffer for the feedback dma if sim mode and sim mode bridge is enabled
    //  (so that the dma from target does not overwrite the simulated feedback)
    if ( INS(XSimMode) || INS(SimMode) )
    {
        if ( SimModeBrgEnabled )
        {
            WinInfo[REBF_WIN_FBK].DmaBufPtr  = (uint32_t*)&BrgFbkAlt;
            WinInfo[REBF_WIN_FBK].DmaBufAddr = BrgFbkAltAddr;
        }
    }

    //-------------------------------------------------------------------------
    // open the host side of the channel
    LinkInfo.LinkId    = LinkIdReq;
    LinkInfo.DeviceId  = DevIdReq;
    LinkInfo.ChannelId = ChanIdReq;
    if ( Hssl.Open( LinkInfo, INS(Uniquifier), ErrCode ) != SUCCESS )
    {
        HostIsOpen = false;
        printf("Hssl device %d channel %d open failure (error code = %d)\n", DevIdReq, ChanIdReq, ErrCode);
        PUSH_DIAG(HsslOpnFail);
        return FAIL;
    }

    //------------------------------------------------------------------------
    // init the private class data
    Uniq = INS(Uniquifier);

    //-------------------------------------------------------------------------
    // add this instance to the linked list of class instance pointers
    BridgeRebfList.Insert(this);

    //-------------------------------------------------------------------------
    // add low level monitor support if this is the first channel to be opened
    if ( BridgeRebfList.NumElements() == 1 )
    {
        if ( LlmAdd( gMonitor ) != SUCCESS )
        {
            HostIsOpen = false;
            printf("Hssl device %d channel %d failure to add LLM support\n", DevIdReq, ChanIdReq);
            PUSH_DIAG(HsslOpnFail);
            return FAIL;
        }
    }

    //-------------------------------------------------------------------------
    // host hssl hardware has been successfully opened
    //  HostIsOpen is used to guard all hssl hardware register access from within this class
    //  it is set true here since the hssl hardware has been successfully allocated
    HostIsOpen = true;
    printf("Hssl device %d channel %d opened successfully\n", LinkInfo.DeviceId, LinkInfo.ChannelId);
    return SUCCESS;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Close the HSSL requested.
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      1 = success, 0 = failure
//
///////////////////////////////////////////////////////////////////////////////
int cBridgeRebf::Close(void)
{
    int     ErrCode;

    Hssl.Close( ErrCode );
    HostIsOpen = false;

    return SUCCESS;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Write fpga command data into external command-window reference, and
//      calculate task-averaged bridge voltages based on same fpga commands
//      and Dc-link voltage.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::SimReadCmdVavg (
    BrgCmdVavgSimTyp&   BrgCmdVavgSim       // output:  commanded bridge output average voltages
)
{
    PhsTransListTyp PhsTransList;

    //-------------------------------------------------------------------------
    // bridge 1
    if ( INS(Brg1InfcHdw != BRG_INFC_NONE) )
    {
        PhsTransList = IN(Brg1PhsTransList);

        if ( strncmp(INS(Uniquifier), "L", 1) == 0 )
        {
            BrgCmdVavgSim.Brg1.Va  = BrgSim.SimPhaseVoltAvg( PhsTransList, VdcFiberBrgHdw, PHASE_A, PwmFull ,PwmFullRcp );
            BrgCmdVavgSim.Brg1.Vb  = BrgSim.SimPhaseVoltAvg( PhsTransList, VdcFiberBrgHdw, PHASE_B, PwmFull ,PwmFullRcp );
            BrgCmdVavgSim.Brg1.Vc  = BrgSim.SimPhaseVoltAvg( PhsTransList, VdcFiberBrgHdw, PHASE_C, PwmFull ,PwmFullRcp );
        }
        else
        {
            BrgCmdVavgSim.Brg1.Va  = BrgSim.SimPhaseVoltAvg( PhsTransList, VdBrg1Hdw, PHASE_A, PwmFull ,PwmFullRcp );
            BrgCmdVavgSim.Brg1.Vb  = BrgSim.SimPhaseVoltAvg( PhsTransList, VdBrg1Hdw, PHASE_B, PwmFull ,PwmFullRcp );
            BrgCmdVavgSim.Brg1.Vc  = BrgSim.SimPhaseVoltAvg( PhsTransList, VdBrg1Hdw, PHASE_C, PwmFull ,PwmFullRcp );
        }
    }

    //-------------------------------------------------------------------------
    // done
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Write fpga command data into external command-window reference, and
//      calculate task-averaged bridge voltages based on same fpga commands
//      and Dc-link voltage.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::SimReadCmdVavg (
    BrgCmdVavgSimTyp&   BrgCmdVavgSim,       // output:  commanded bridge output average voltages
    float Vdc
)
{
    PhsTransListTyp PhsTransList;

    //-------------------------------------------------------------------------
    // bridge 1
    if ( INS(Brg1InfcHdw != BRG_INFC_NONE) )
    {
        PhsTransList = IN(Brg1PhsTransList);

        BrgCmdVavgSim.Brg1.Va  = BrgSim.SimPhaseVoltAvg( PhsTransList, Vdc, PHASE_A, PwmFull ,PwmFullRcp );
        BrgCmdVavgSim.Brg1.Vb  = BrgSim.SimPhaseVoltAvg( PhsTransList, Vdc, PHASE_B, PwmFull ,PwmFullRcp );
        BrgCmdVavgSim.Brg1.Vc  = BrgSim.SimPhaseVoltAvg( PhsTransList, Vdc, PHASE_C, PwmFull ,PwmFullRcp );
    }

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Write fpga command data into external command-window reference, and
//      calculate task-averaged bridge voltages based on same fpga commands
//      and Dc-link voltage.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::SimPwmTm(void)
{
    PhsTransListTyp PhsTransList;

    //-------------------------------------------------------------------------
    // bridge 1
    if ( INS(Brg1InfcHdw != BRG_INFC_NONE) )
    {
        PhsTransList = IN(Brg1PhsTransList);

        BrgSim.PhasePwmUpDnTm( PhsTransList, PHASE_A, PwmFull );
        SimPhAUpTm  = BrgSim.PwmUpTm;
        SimPhADnTm  = BrgSim.PwmDnTm;

        BrgSim.PhasePwmUpDnTm( PhsTransList, PHASE_B, PwmFull );
        SimPhBUpTm  = BrgSim.PwmUpTm;
        SimPhBDnTm  = BrgSim.PwmDnTm;

        BrgSim.PhasePwmUpDnTm( PhsTransList, PHASE_C, PwmFull );
        SimPhCUpTm  = BrgSim.PwmUpTm;
        SimPhCDnTm  = BrgSim.PwmDnTm;


    }

    return;
}




///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Write fpga DB command data into external command-window reference to
//      dive the DB simulator
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::SimReadDbCmd (
    BrgDbCmdSimTyp&   BrgDbCmdSim         // output:  commanded DB gating
)
{
    if ( strncmp(INS(Uniquifier), "L", 1) == 0 )
    {
        BrgDbCmdSim.Db1GateCmd = BrgCmd.Cr21.ForceDb;
        //BrgDbCmdSim.Db2GateCmd = BrgCmd.Cr21.ForceDb2;
    }

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Write fpga command data into external command-window reference, and
//      calculate task-averaged bridge voltages based on same fpga commands
//      and Dc-link voltage.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::SimReadCmdTra (
    BrgCmdTraSimTyp&    BrgCmdTraSim        // output:  commanded bridge phase tranistions
)
{
    PhsTransListTyp PhsTransList;

    //-------------------------------------------------------------------------
    // bridge 1
    BrgCmdTraSim.PeEnableBrg1 = false;
    if ( INS(Brg1InfcHdw != BRG_INFC_NONE) )
    {
        PhsTransList = IN(Brg1PhsTransList);
        for ( unsigned tra = 0 ; tra < PhsTransList.Len ; ++tra )
        {
            for ( unsigned phs = 0 ; phs < NUM_OF_PHASES ; ++phs )
            {
                BrgCmdTraSim.Brg1[tra][phs].DelTm  = (int)((PhsTransList.pTrans)[tra][phs].DelTm) * (float)FRC_CLK_RCP;
                BrgCmdTraSim.Brg1[tra][phs].State  = (PhsTransList.pTrans)[tra][phs].State;
                BrgCmdTraSim.Brg1[tra][phs].Enable = (PhsTransList.pTrans)[tra][phs].Enable;;
            }
        }
        if ( (BrgCmd.Cr21.BrgEnable == 0x2) && !BrgFbk.StatCom.DriveFlt )
        {
            BrgCmdTraSim.PeEnableBrg1 = true;
        }

        {
            BrgCmdTraSim.BrgCfg.AiocEnbBr1 = !IN(MaskIocFlts);
            BrgCmdTraSim.BrgCfg.BiocEnbBr1 = !IN(MaskIocFlts);
            BrgCmdTraSim.BrgCfg.CiocEnbBr1 = !IN(MaskIocFlts);
            BrgCmdTraSim.BrgCfg.DiocEnbBr1 = false;
            BrgCmdTraSim.BrgCfg.IlimEnb    = IN(IlimEnb);
            BrgCmdTraSim.BrgCfg.IlimEnD    = false;
            BrgCmdTraSim.BrgCfg.FltRst     = BrgCmd.Cr21.BrgFltClear;
        }

    }

    BrgCmdTraSim.DelTm1 = INS(DelTm1);

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Convert simulated task-averaged feedbacks into Vco counts. Simulate
//      some bridge faults and diagnostic signals. Pass all this information
//      to fga feedback window for further processing.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::SimWriteFbk (
    const BrgFbkSimTyp&    BrgFbkSim      // input:   structure holding average feedbacks to be converted to vco
)
{

    //=========================================================================================================
    // Bridge 1 feedabacks
    {
        BrgSim.SetVcoMask( BRG_VCO_MASK );              // Constant for all configurations
        BrgSim.SetVcoFmin( (float)BRG_VCO_FMIN );       // Constant for all configurations

        // bridge io #1
        if ( INS(Brg1InfcHdw != BRG_INFC_NONE) )
        {
            //-------------------------------------------------------------------------------------------------
            // dc voltage (Bridge 1 DC link voltage if AEBI, VdbR1 if AEDB)
            if ( Brg1InfcBoardAedb )
            {
//              PreREBH:
                BrgSim.SetVcoFmax( (float)BRG_VCO_FMAX );
                BrgSim.SetVcoFzro( (float)AEDB_VDC_FBK_OFFSET_FREQ );
                BrgSim.SetVcoGain( SclLinkVoltsBrg1Rcp );
            }
            else
            {
                if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
                {
//                  PreREBH:
                    BrgSim.SetVcoFmax( (float)BRG_VCO_FMAX      );
                    BrgSim.SetVcoFzro( 0.0F );
                    BrgSim.SetVcoGain( SclLinkVoltsBrg1Rcp       );
                }
                else
                {
//                  REBH:
                    BrgSim.SetVcoFmax( (float)BRG_VCO_REBH_FMAX );
                    BrgSim.SetVcoFzro(  0.0F );
                    BrgSim.SetVcoGain( SclPhsVoltsBrg1Rcp*2.0F );
                }
            }
            BrgFbk.VcoBr1.Vdc = BrgSim.Avg2Vco( BrgFbkSim.Brg1.Vdc, VdcVcoBr1LatchCnt, VdcVcoBr1DelCntRes );

            //-------------------------------------------------------------------------------------------------
            // ac voltage
            if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
            {
//              PreREBH:
                BrgSim.SetVcoFmax( (float)BRG_VCO_FMAX     );
                BrgSim.SetVcoFzro( (float)VcoBiasFreqBrg1  );
                BrgSim.SetVcoGain( SclPhsVoltsBrg1Rcp      );
                BrgFbk.VcoBr1.Vab    = BrgSim.Avg2Vco( BrgFbkSim.Brg1.Vax, VaxVcoBr1LatchCnt, VaxVcoBr1DelCntRes );
                BrgFbk.VcoBr1.Vbc    = BrgSim.Avg2Vco( BrgFbkSim.Brg1.Vbx, VbxVcoBr1LatchCnt, VbxVcoBr1DelCntRes );
            }
            else
            {
//              REBH:
                BrgSim.SetVcoFmax( (float)BRG_VCO_REBH_FMAX);
                BrgSim.SetVcoFzro(  0.0F );
                BrgSim.SetVcoGain( SclPhsVoltsBrg1Rcp *2.0F);
                BrgFbk.VcoBr1.Vab    = BrgSim.Avg2Vco( BrgFbkSim.Brg1.Vax, VaxVcoBr1LatchCnt, VaxVcoBr1DelCntRes );
                BrgFbk.VcoBr1.Vbc    = BrgSim.Avg2Vco( BrgFbkSim.Brg1.Vbx, VbxVcoBr1LatchCnt, VbxVcoBr1DelCntRes );
                BrgFbk.VdcSDL5_ST.Vc = BrgSim.Avg2Vco( BrgFbkSim.Brg1.Vcx, VcxVcoBr1LatchCnt, VcxVcoBr1DelCntRes );
             }

            //-------------------------------------------------------------------------------------------------
            // ac current
            if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
            {
//              PreREBH:
                BrgSim.SetVcoFmax( (float)BRG_VCO_FMAX     );
                BrgSim.SetVcoFzro( BRG_VCO_FZRO );
                BrgSim.SetVcoGain( SclPhsCurBrg1Rcp );
            }
            else
            {
//              REBH:
                BrgSim.SetVcoFmax( (float)BRG_VCO_REBH_FMAX);
                BrgSim.SetVcoFzro( 0.0F );
                BrgSim.SetVcoGain( SclPhsCurBrg1Rcp );
            }
            BrgFbk.VcoBr1.Ia = BrgSim.Avg2Vco( BrgFbkSim.Brg1.Ia, IaVcoBr1LatchCnt, IaVcoBr1DelCntRes );
            BrgFbk.VcoBr1.Ib = BrgSim.Avg2Vco( BrgFbkSim.Brg1.Ib, IbVcoBr1LatchCnt, IbVcoBr1DelCntRes );
            BrgFbk.VcoBr1.Ic = BrgSim.Avg2Vco( BrgFbkSim.Brg1.Ic, IcVcoBr1LatchCnt, IcVcoBr1DelCntRes );
        }
        //-----------------------------------------------------------------------------------------------------
//      REBx: dc link voltage, dc link voltage via fiber-optic, db resistor voltage
//      REBH: rename "BrgFbk.VcoVdc.Vdcx" --> "...VcoVdb.Vdcx"
        if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
        {
//          PreREBH:
            BrgSim.SetVcoFmax( (float)BRG_VCO_FMAX);
            BrgSim.SetVcoFzro( 0.0F );
            BrgSim.SetVcoGain( SclLinkVoltsBrg1Rcp );
        }
        else
        {
//          REBH:
            BrgSim.SetVcoFmax( (float)BRG_VCO_REBH_FMAX);
            BrgSim.SetVcoFzro( 0.0F );
            BrgSim.SetVcoGain( SclLinkVoltsBrg1Rcp*2.0F);
        }
        BrgFbk.VcoVdb.Vdcx = BrgSim.Avg2Vco( BrgFbkSim.VdcFiber, VdcFiberVcoLatchCnt, VdcFiberVcoDelCntRes );
    }
    //=========================================================================================================

    //=========================================================================================================
    // IO 1 feedbacks
    {
            BrgSim.SetVcoMask( IO_VCO_MASK );
            BrgSim.SetVcoFmin( (float)IO_VCO_FMIN );
            BrgSim.SetVcoFmax( (float)IO_VCO_FMAX );
            BrgSim.SetVcoFzro( (float)IO_VCO_FZRO );

            // analog io #1
            if ( INS(Io1InfcHdw) != IO_INFC_NONE )
            {
                    // ac voltage
                    BrgSim.SetVcoFzro( (float)IO_VCO_FZRO );
                    BrgSim.SetVcoGain( SclPhsVoltsIo1Rcp );
                    BrgFbk.VcoIo1.A01 = BrgSim.Avg2Vco( BrgFbkSim.Ain1.Vax, VaxVcoAin1LatchCnt, VaxVcoAin1DelCntRes );
                    BrgFbk.VcoIo1.A02 = BrgSim.Avg2Vco( BrgFbkSim.Ain1.Vbx, VbxVcoAin1LatchCnt, VbxVcoAin1DelCntRes );
                    BrgFbk.VcoIo1.A03 = BrgSim.Avg2Vco( BrgFbkSim.Ain1.Vcx, VcxVcoAin1LatchCnt, VcxVcoAin1DelCntRes );
                    // ac current
                    BrgSim.SetVcoGain( SclPhsCurIo1Rcp );
                    BrgFbk.VcoIo1.A05 = BrgSim.Avg2Vco( BrgFbkSim.Ain1.Ia, IaVcoAin1LatchCnt, IaVcoAin1DelCntRes );
                    BrgFbk.VcoIo1.A06 = BrgSim.Avg2Vco( BrgFbkSim.Ain1.Ib, IbVcoAin1LatchCnt, IbVcoAin1DelCntRes );
                    BrgFbk.VcoIo1.A07 = BrgSim.Avg2Vco( BrgFbkSim.Ain1.Ic, IcVcoAin1LatchCnt, IcVcoAin1DelCntRes );
            }

    }
    //=========================================================================================================


    //-------------------------------------------------------------------------
    // 1st brg faults
    if ((INS(XSimMode))||(INS(DSimMode)))
    {
        BrgFbk.BrStatFlt.AiocB1 = BrgFbkSim.BrgFlt1.IocPhsA;
        BrgFbk.BrStatFlt.BiocB1 = BrgFbkSim.BrgFlt1.IocPhsB;
        BrgFbk.BrStatFlt.CiocB1 = BrgFbkSim.BrgFlt1.IocPhsC;
        BrgFbk.BrStatFlt.DiocB1 = BrgFbkSim.BrgFlt1.IocPhsDB;
        BrgFbk.BrStatFlt.AnegB1 = BrgFbkSim.BrgFlt1.DsatSwAL;
        BrgFbk.BrStatFlt.BnegB1 = BrgFbkSim.BrgFlt1.DsatSwBL;
        BrgFbk.BrStatFlt.CnegB1 = BrgFbkSim.BrgFlt1.DsatSwCL;
        BrgFbk.BrStatFlt.AposB1 = BrgFbkSim.BrgFlt1.DsatSwAU;
        BrgFbk.BrStatFlt.BposB1 = BrgFbkSim.BrgFlt1.DsatSwBU;
        BrgFbk.BrStatFlt.CposB1 = BrgFbkSim.BrgFlt1.DsatSwCU;
        BrgFbk.BrStatFlt.DdesatB1 = BrgFbkSim.BrgFlt1.DsatSwDB;
        BrgFbk.BrStatFlt.Cin109RtrFlt = cBrgSim::LlmForceTable[InstNumber][1];
        BrgFbk.BrStatFlt.Rly101RtrFlt = BrgFbk.BrStatFlt.Cin109RtrFlt;
        BrgFbk.BrStatFlt.FltBr1 =  BrgFbk.BrStatFlt.AiocB1 || BrgFbk.BrStatFlt.BiocB1 || BrgFbk.BrStatFlt.CiocB1 ||
                                   BrgFbk.BrStatFlt.AnegB1 || BrgFbk.BrStatFlt.BnegB1 || BrgFbk.BrStatFlt.CnegB1 ||
                                   BrgFbk.BrStatFlt.AposB1 || BrgFbk.BrStatFlt.BposB1 || BrgFbk.BrStatFlt.CposB1 ||
                                   BrgFbk.BrStatFlt.DdesatB1 || BrgFbk.BrStatFlt.Cin109RtrFlt || BrgFbk.BrStatFlt.Rly101RtrFlt;
    }
    else if ( InstNumber < cBrgSim::NUM_INST_SUPPORT )  // llm forcing table only supports a limited number of instances
    {
        BrgFbk.BrStatFlt.AiocB1 = cBrgSim::LlmForceTable[InstNumber][3];
        BrgFbk.BrStatFlt.AposB1 = cBrgSim::LlmForceTable[InstNumber][4];
        BrgFbk.BrStatFlt.AnegB1 = cBrgSim::LlmForceTable[InstNumber][5];
        BrgFbk.BrStatFlt.Cin109RtrFlt = cBrgSim::LlmForceTable[InstNumber][1];
        BrgFbk.BrStatFlt.Rly101RtrFlt = BrgFbk.BrStatFlt.Cin109RtrFlt;
        BrgFbk.BrStatFlt.FltBr1 = BrgFbk.BrStatFlt.AiocB1 || BrgFbk.BrStatFlt.AposB1 || BrgFbk.BrStatFlt.AnegB1 ||
                              BrgFbk.BrStatFlt.Cin109RtrFlt || BrgFbk.BrStatFlt.Rly101RtrFlt;
    }

    //-------------------------------------------------------------------------
    // Diagnostic support
    if ( InstNumber < cBrgSim::NUM_INST_SUPPORT )  // llm forcing table only supports a limited number of instances
    {
        BrgFbk.MiscStat.InPwrOk = cBrgSim::LlmForceTable[InstNumber][0];          // HSLA power supply voltage is above 21.8 Vdc
        if((INS(XSimMode))||(INS(DSimMode)))
        {
            BrgFbk.StatCom.DriveFlt=BrgFbkSim.BrgFlt1.BrgFlt;
        }
        else
        {
        BrgFbk.StatCom.DriveFlt = ( BrgFbk.BrStatFlt.AiocB1 ||
                                    BrgFbk.BrStatFlt.AposB1 ||
                                    BrgFbk.BrStatFlt.AnegB1 ||
                                    cBrgSim::LlmForceTable[InstNumber][1] || cBrgSim::LlmForceTable[InstNumber][2] ) ? true : false;
        }
        // ready-to-run enable status, implemented through contact-inputs 10 and 09
        // also force the sim mode to have valid pwr status for Hsla, Relay, and control
        *( reinterpret_cast<unsigned*> (&BrgFbk.StatReg21) ) = 0xCC06E7FF;     // Cont-Inputs 09 & 10 are inactive - shutdown must be commanded

        if ( cBrgSim::LlmForceTable[InstNumber][1] == false )
        {
            *( reinterpret_cast<unsigned*> (&BrgFbk.StatReg21) ) = 0xCC06E3FF; // only Cont-Input 09 is inactive - shutdown must be commanded
        }

        if ( cBrgSim::LlmForceTable[InstNumber][2] == false )
        {
            *( reinterpret_cast<unsigned*> (&BrgFbk.StatReg21) ) = 0xCC06E1FF; // Cont-Inputs 09 & 10 are active - bridge ready-to-run
        }
    }

    //-------------------------------------------------------------------------
    // pe enable status
    if ( (BrgCmd.Cr21.BrgEnable == 0x2) && !BrgFbk.StatCom.DriveFlt )
    {
        BrgFbk.StatCom.PeEnb = true;
        BrgFbk.StatCom.DrPc  |= 0x1;     // enable P5 gate logic power to bridges 1 ; remove from code after verification
    }
    else
    {
        BrgFbk.StatCom.PeEnb = false;
        BrgFbk.StatCom.DrPc  &= 0x2;     // disable P5 gate logic power to bridges 1

    }

    //-------------------------------------------------------------------------
    // prevent 'P5 power to gate control' problems, and FRC and load-Pulse faults
    *( reinterpret_cast<unsigned*> (&BrgFbk.StatCom) ) |= 0x8F5060;

    //-------------------------------------------------------------------------
    // cfg enable status
    if ( BrgCmd.CrCfg.CfgEnable == 0x2 )
    {
        BrgFbk.MiscStat.CfgEnabled = true;
    }
    else
    {
        BrgFbk.MiscStat.CfgEnabled = false;
    }

    //-------------------------------------------------------------------------
    // speed and angle
#if 0
    Sim.DataToCtl.Diag[0] = IN(M_SpdSim);
    Sim.DataToCtl.Diag[1] = static_cast<float> (IN(M_TachPosnSim));
#endif

    //-------------------------------------------------------------------------
    // Thermistors - Bridge and IO
    if ( INS(DSimMode) )
    {
        if (IN(T1wthnT3) == 0)
        {                                   // code starting its count conversion
            OUT(TempVco1stSmpl) = true;
        }
    }
    else
    {
        if (IN(T1wthnT3) == (T1perT3-1))    // Provide fbks one-task prior to product
        {                                   // code starting its count conversion
            OUT(TempVco1stSmpl) = true;
        }
    }

    OUT(ThrmstrBrgCnts);

    if (TempVco1stSmpl)
    {
        BrgFbk.StatCom.TempVcoSel    = TempVco1stCnt;

//      REBH: rename "BrgFbk.VcoTemp21.BrgCellTemp" --> "...Brg1CellTemp"
        BrgFbk.VcoTemp21.Brg1CellTemp = ThrmstrBrgCnts;

        if ( (TempVco1stCnt == 3) && (InstNumber == 0) )
        {
            if ( ThrmHiSelSim )
            {
                BrgFbk.CrLb.VcoT03 = ThrmCoolntLwCntsZ1;
            }
            else
            {
                BrgFbk.CrLb.VcoT03 = ThrmCoolntHiCntsZ1;
            }
        }
        else
        {
            BrgFbk.CrLb.VcoT03 = 0;
        }
        TempVco1stCnt++;

        if (TempVco1stCnt > 7)
            {
                OUT(TempVco1stSmpl) = false;
                TempVco1stCnt = 0;
            }
    }

    if ( INS(DSimMode) )
    {
        if (IN(T1wthnT3) == (INS(TempVcoCalcSt)))    // Dsim
        {                                              // code starting its count conversion
            OUT(TempVco2ndSmpl) = true;
            ThrmstrBrgCnts = ThrmstrBrgCnts +
                             static_cast<unsigned> (BrgFbkSim.ThrmstrBrg * TempVcoCalTm);
        }
    }
    else
    {
        if (IN(T1wthnT3) == (INS(TempVcoCalcSt)-1))    // Provide fbks one-task prior to product
        {                                              // code starting its count conversion
            OUT(TempVco2ndSmpl) = true;
            ThrmstrBrgCnts = ThrmstrBrgCnts +
                             static_cast<unsigned> (BrgFbkSim.ThrmstrBrg * TempVcoCalTm);
        }
    }

    if (TempVco2ndSmpl)
    {
        BrgFbk.StatCom.TempVcoSel    = TempVco2ndCnt;

//      REBH: rename "BrgFbk.VcoTemp21.BrgCellTemp" --> "...Brg1CellTemp"
        BrgFbk.VcoTemp21.Brg1CellTemp = ThrmstrBrgCnts;

        if ( (TempVco2ndCnt == 3) && (InstNumber == 0) )
        {
            if ( ThrmHiSelSim )
            {
                BrgFbk.CrLb.VcoT03 = ThrmCoolntLwCntsZ1 +
                                     static_cast<unsigned> (BrgFbkSim.ThrmstrCoolnt.LwRngFrq * TempVcoCalTm);
                ThrmCoolntLwCntsZ1 = BrgFbk.CrLb.VcoT03;
            }
            else
            {
                BrgFbk.CrLb.VcoT03 = ThrmCoolntHiCntsZ1 +
                                     static_cast<unsigned> (BrgFbkSim.ThrmstrCoolnt.HiRngFrq * TempVcoCalTm);
                ThrmCoolntHiCntsZ1 = BrgFbk.CrLb.VcoT03;
            }
        }
        else
        {
            BrgFbk.CrLb.VcoT03 = ThrmHiSelSim ? IoThrmstrCnts1Sim:IoThrmstrCnts2Sim;
        }
        TempVco2ndCnt++;

        if (TempVco2ndCnt > 7)
        {
            OUT(TempVco2ndSmpl) = false;
            TempVco2ndCnt = 0;
            ThrmHiSelSim = !ThrmHiSelSim;
        }
    }

    //-------------------------------------------------------------------------
    // Encoder data

//  CHANGE "strncmp(INS(Uniquifier),"R",1)==0" --> "INS(DSimMode)":
    if ( InstNumber == 1)
    {
                if ( INS(DSimMode) )
        {
            BrgFbk.Tach01.TachV  = BrgSim.Avg2Enc( SimBrgFbkTachV/PARM(R_PolePairs), SimSpdEncLatchCnt, SimSpdEncDelCntRes );
            BrgFbk.Tach01.TachT  = INS(DelTm1);
        }
        else
        {
            BrgFbk.Tach01.TachV  = BrgFbkSim.TachV;
        }
                BrgFbk.Tach01.TachM  = BrgFbkSim.TachMv;
        }


    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Pass simulated bridge fault status into the "BrgFbk" structure,
//      private to this "BridgeRebf" class.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::SimWriteFlt (
    const BrgFltSimTyp&    BrgFltSim      // input:   structure holding faults
)
{
    // bridge io #1
    if ( INS(Brg1InfcHdw != BRG_INFC_NONE) )
    {
        BrgFbk.BrStatFlt.AiocB1 = BrgFltSim.Brg1.IocPhsA;
        BrgFbk.BrStatFlt.BiocB1 = BrgFltSim.Brg1.IocPhsB;
        BrgFbk.BrStatFlt.CiocB1 = BrgFltSim.Brg1.IocPhsC;

        BrgFbk.BrStatFlt.AposB1 = BrgFltSim.Brg1.DsatSwAU;
        BrgFbk.BrStatFlt.AnegB1 = BrgFltSim.Brg1.DsatSwAL;
        BrgFbk.BrStatFlt.BposB1 = BrgFltSim.Brg1.DsatSwBU;
        BrgFbk.BrStatFlt.BnegB1 = BrgFltSim.Brg1.DsatSwBL;
        BrgFbk.BrStatFlt.CposB1 = BrgFltSim.Brg1.DsatSwCU;
        BrgFbk.BrStatFlt.CnegB1 = BrgFltSim.Brg1.DsatSwCL;
    }

//-------------------------------------------------------------------------
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Initialize the bridge interface fpga.
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::InitializeTarget(void)
{
    int     ErrCode;

    //-------------------------------------------------------------------------
    // force a return to the abort case if discovery request and not already in an abort or armed state
    //  this makes sure all housekeeping is done regardless of the state of this state machine
    if ( TargDscReq && (TargInitCase != 99) && (TargInitCase != -1) )
    {
        TargInitCase = 99;
        TargInitPass = 0;
    }

    //-------------------------------------------------------------------------
    // state machine to achieve initialization sequence for the target
    switch ( TargInitCase )
    {

        //---------------------------------------------------------------------
        // initialization is armed, waiting for discover sequence to complete
        case -1:

            if ( TargInitPass == 0 )
            {
                // disable error detection and disable the window transfers
                HsslErrChksEnb = false;
                for ( unsigned WinIdx = 0 ; WinIdx < ARRAY_LEN(WinInfo) ; ++WinIdx )
                {
                    Hssl.DisableWinTransfers( WinInfo[WinIdx].Idx, ErrCode );
                }
                ++TargInitPass;
                break;
            }

            // hold in this state until background initialization completes
            if ( TargDscReq || !TargDscDone )
            {
                break;
            }

            // enable window transfers
            for ( unsigned WinIdx = 0 ; WinIdx < ARRAY_LEN(WinInfo) ; ++WinIdx )
            {
                if ( Hssl.EnableWinTransfers( WinInfo[WinIdx].Idx, ErrCode ) != SUCCESS )
                {
                    HsslErrFlagsIni |= TARG_INI_ERR_WIN_ENB;
                }
            }
            if ( (HsslErrFlagsIni & TARG_INI_ERR_WIN_ENB) != 0 )
            {
                PUSH_DIAG(HsslIniFail);
                TargInitCase = 99;
                TargInitPass = 0;
                break;
            }

            // advance to the next state
            TargInitCase++;
            TargInitPass = 0;
            break;

        //---------------------------------------------------------------------
        // waiting to assure stable link operation
        case 0:
             if ( OUT(DisCmdNotFlg))
             {
                OUT(DisCmdNotFlg) = false;
             }
            // toggle the bridge fault reset
            if ( BrgCmd.Cr21.BrgFltClear == false )
            {
                BrgCmd.Cr21.BrgFltClear = true;
                break;
            }
            else
            {
                BrgCmd.Cr21.BrgFltClear = false;
                if ( ++TargInitPass < 20 )      // wait for 20 toggles before advancing state to stabilize link operation
                {                           // inc here so that always exit this state with BrgFltClear == false
                    break;
                }
            }

            // things are stable so enable error detection (only if expect there to be target hardware)
            if ( TargetHdwExpected )
            {
                HsslErrChksEnb = true;
            }

            // advance to the next state
            TargInitCase++;
            TargInitPass = 0;
            break;

        //---------------------------------------------------------------------
        // check hsla fpga revision code
        case 1:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            TargFpgaRev = *(unsigned*)&BrgFbk.RevCode;

            if ( PARM(TargFpgaRvCk) != 0 )
            {
                // if revision error, then return to initialization state 0
                if ( TargFpgaRev != PARM(TargFpgaRvCk) )
                {
                    HsslErrFlagsIni |= TARG_INI_ERR_FPGA_REV;
                    PUSH_DIAG(HsslIniFail);
                    TargInitCase = 99;
                    TargInitPass = 0;
                }
                else
                {
                    // advance to the next state
                    TargInitCase++;
                    TargInitPass = 0;

                }
            }
            else
            {
                // inhibit target fpga rev fault (useful when targets with different revs exist in the system)
                TargInitCase++;
                TargInitPass = 0;
            }
            break;

        //---------------------------------------------------------------------
        // enable the ability to write to configuration bits
        case 2:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            // advance the config enable request 00>01>10
            if ( TargInitPass < 3 )
            {
                BrgCmd.CrCfg.CfgEnable = TargInitPass;
                ++TargInitPass;
            }
            else if ( !BrgCfgEnabled )
            {

                BrgCmd.CrCfg.CfgEnable = 0x0;
                HsslErrFlagsIni |= TARG_INI_CFG_ENB_FAIL;
                PUSH_DIAG(HsslIniFail);
                TargInitCase = 99;
                TargInitPass = 0;
            }
            else
            {
                // advance to the next state
                TargInitCase++;
                TargInitPass = 0;
            }
            break;

        //---------------------------------------------------------------------
        // read the auxiliary board list from ID bus 1 (REBF J1 and P1)
        case 3:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            // command aux board id data to be sent in the loopback register
            BrgCmd.CrCfg.CfgRegSel = REBx_AuxBrdId1_offset >> 1;
            BrgCmd.CrCfg.CfgVal = TargInitPass;

            // on 1st pass - nothing to read
            if ( TargInitPass == 0 )
            {
                memset( IdBufAux1, 0, sizeof(IdBufAux1) );    // clear the buffer
                ++TargInitPass;
                break;
            }

            // on 2nd pass - read the size (bytes) of the auxiliary board id data from word #0 (byte 0,1)
            else if ( TargInitPass == 1 )
            {
                IdBufAux1DataLen = BrgFbk.CrLb.LbReg;
                if ( IdBufAux1DataLen > (int)sizeof(IdBufAux1) )
                {
                    HsslErrFlagsIni |= TARG_INI_AUX1_ID_INV_SIZE;
                    PUSH_DIAG(HsslIniFail);
                    TargInitCase = 99;
                    TargInitPass = 0;
                    break;
                }
            }

            // on 3rd+ pass - read the data into the buffer (exclude size in word #0)
            else
            {
                IdBufAux1[TargInitPass-2] = (uint16_t)BrgFbk.CrLb.LbReg;
            }

            // check to see if we have read all the data
            if ( (2*(int)TargInitPass) < IdBufAux1DataLen )     // convert word index to byte index for comparision
            {
                ++TargInitPass;
            }
            else
            {
                // adjust data length to account for buffer not including size in first 2 bytes
                IdBufAux1DataLen -= 2;

                // advance to the next state
                if ( RebhSet)
                {
                    TargInitCase++;
                    StatReg21    = *(unsigned*)&BrgFbk.StatReg21;
                }
                else
                {
                    TargInitCase = 11;
                }

                TargInitPass = 0;
            }

            break;

        //---------------------------------------------------------------------
        // read the auxiliary board list from ID bus 2 (REBF J2 and P2)
        case 4:

            if ( BrgFbk.StatReg21.REPI_IDdn == true)
            {

                // skip this step if target hardware is not expected
                if ( !TargetHdwExpected )
                {
                    TargInitCase++;
                    TargInitPass = 0;
                    break;
                }

                // command aux board id data to be sent in the loopback register
                BrgCmd.CrCfg.CfgRegSel = REPI_BrdId_offset >> 1;
                BrgCmd.CrCfg.CfgVal = TargInitPass;

                // on 1st pass - nothing to read
                if ( TargInitPass == 0 )
                {
                    memset( IdBufRepi, 0, sizeof(IdBufRepi) );    // clear the buffer
                    ++TargInitPass;
                    break;
                }

                // on 2nd pass - read the size (bytes) of the auxiliary board id data from word #0 (byte 0,1)
                else if ( TargInitPass == 1 )
                {
                    IdBufRepiDataLen = BrgFbk.CrLb.LbReg;
                    if ( IdBufRepiDataLen > (int)sizeof(IdBufRepi) )
                    {
                        HsslErrFlagsIni |= TARG_INI_AUX1_ID_INV_SIZE;
                        PUSH_DIAG(HsslIniFail);
                        TargInitCase = 99;
                        TargInitPass = 0;
                        break;
                    }
                }

                // on 3rd+ pass - read the data into the buffer (exclude size in word #0)
                else
                {
                    IdBufRepi[TargInitPass-2] = (uint16_t)BrgFbk.CrLb.LbReg;
                }

                // check to see if we have read all the data
                if ( (2*(int)TargInitPass) < IdBufRepiDataLen )     // convert word index to byte index for comparision
                {
                    ++TargInitPass;
                }
                else
                {
                    BrgCmd.Cr21.REPI_EN = true;

                    // adjust data length to account for buffer not including size in first 2 bytes
                    IdBufRepiDataLen -= 2;
                    TargInitCase++;
                    TargInitPass = 0;
                    TargInitDly = 0.0F;
                }
            }
            break;

        //---------------------------------------------------------------------
        // Added Delay
        case 5:

            TargInitDly = TargInitDly + INS(DelTm1);

            if ( (TargInitDly > 90.0F) || INS(SimMode) )
            {
                // advance to the next state
                Sdl21Reg  = *(unsigned *)&BrgFbk.SDL21_ST;
                Sdl43Reg  = *(unsigned *)&BrgFbk.SDL43_ST;

                TargInitCase++;
                TargInitPass = 0;
                TargInitDly = 0.0F;
            }

            break;

        //---------------------------------------------------------------------
        // read the auxiliary board list from ID bus 2 (REBF J2 and P2)
        case 6:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            // command aux board id data to be sent in the loopback register
            BrgCmd.CrCfg.CfgRegSel = RGIB_BrdId_offset >> 1;

            if ( TargInitCnt == 0)
            {
                TargInitPass = (2 * TargSdlNum + 1) * 512;
                ++TargInitCnt;
            }

            BrgCmd.CrCfg.CfgVal = TargInitPass;

            // on 1st pass - nothing to read
            if ( (TargInitCnt == 1))
            {
                memset( IdBufRgib, 0, sizeof(IdBufRgib) );    // clear the buffer
                TargInitCnt = TargInitPass;
                break;
            }


            //if ( BrgFbk.SDL21_ST.SDL_ID_DN == true)
            //{
                // on 2nd pass - read the size (bytes) of the auxiliary board id data from word #0 (byte 0,1)
                if ( TargInitPass == TargInitCnt )
                {
                    IdBufRgibDataLen = BrgFbk.CrLb.LbReg;
                    if ( IdBufRgibDataLen > (int)sizeof(IdBufRgib) )
                    {
                        HsslErrFlagsIni |= TARG_INI_AUX1_ID_INV_SIZE;
                        PUSH_DIAG(HsslIniFail);
                        TargInitCase = 99;
                        TargInitPass = 0;
                        break;
                    }
                }

                // on 3rd+ pass - read the data into the buffer (exclude size in word #0)
                else
                {
                    IdBufRgib[(TargInitPass - TargInitCnt)- 2] = (uint16_t)BrgFbk.CrLb.LbReg;
                }

                // check to see if we have read all the data

                if ( ((2*((int)TargInitPass - (int)TargInitCnt))-0) < IdBufRgibDataLen )     // convert word index to byte index for comparision
                {
                    ++TargInitPass;
                }
                else
                {
                    // adjust data length to account for buffer not including size in first 2 bytes
                    IdBufRgibDataLen -= 2;

                    // advance to the next state
                    TargInitCase++;
                    TargInitPass = 0;
                    ++TargSdlNum;
                    TargInitCnt  = 0;
                }
            //}
            break;

        //---------------------------------------------------------------------
        // read the auxiliary board list from ID bus 2 (REBF J2 and P2)
        case 7:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            // command aux board id data to be sent in the loopback register
            BrgCmd.CrCfg.CfgRegSel = RGIB_BrdId_offset >> 1;

            if ( TargInitCnt == 0)
            {
                TargInitPass = (2 * TargSdlNum + 1) * 512;
                ++TargInitCnt;
            }

            BrgCmd.CrCfg.CfgVal = TargInitPass;

            // on 1st pass - nothing to read
            if ( (TargInitCnt == 1))
            {
                memset( IdBufRgib2, 0, sizeof(IdBufRgib2) );    // clear the buffer
                TargInitCnt = TargInitPass;
                break;
            }

            //if ( BrgFbk.SDL21_ST.SDL_ID_DN == true)
            //{
                // on 2nd pass - read the size (bytes) of the auxiliary board id data from word #0 (byte 0,1)
                if ( TargInitPass == TargInitCnt )
                {
                    IdBufRgib2DataLen = BrgFbk.CrLb.LbReg;
                    if ( IdBufRgib2DataLen > (int)sizeof(IdBufRgib2) )
                    {
                        HsslErrFlagsIni |= TARG_INI_AUX1_ID_INV_SIZE;
                        PUSH_DIAG(HsslIniFail);
                        TargInitCase = 99;
                        TargInitPass = 0;
                        break;
                    }
                }

                // on 3rd+ pass - read the data into the buffer (exclude size in word #0)
                else
                {
                    IdBufRgib2[(TargInitPass - TargInitCnt)- 2] = (uint16_t)BrgFbk.CrLb.LbReg;
                }

                // check to see if we have read all the data

                if ( ((2*((int)TargInitPass - (int)TargInitCnt))-0) < IdBufRgib2DataLen )     // convert word index to byte index for comparision
                {
                    ++TargInitPass;
                }
                else
                {
                    // adjust data length to account for buffer not including size in first 2 bytes
                    IdBufRgib2DataLen -= 2;

                    // advance to the next state
                    TargInitCase++;
                    TargInitPass = 0;
                    ++TargSdlNum;
                    TargInitCnt  = 0;
                }
            //}
            break;

        //---------------------------------------------------------------------
        // read the auxiliary board list from ID bus 2 (REBF J2 and P2)
        case 8:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            // command aux board id data to be sent in the loopback register
            BrgCmd.CrCfg.CfgRegSel = RGIB_BrdId_offset >> 1;

            if ( TargInitCnt == 0)
            {
                TargInitPass = (2 * TargSdlNum + 1) * 512;
                ++TargInitCnt;
            }

            BrgCmd.CrCfg.CfgVal = TargInitPass;

            // on 1st pass - nothing to read
            if ( (TargInitCnt == 1))
            {
                memset( IdBufRgib3, 0, sizeof(IdBufRgib3) );    // clear the buffer
                TargInitCnt = TargInitPass;
                break;
            }


            //if ( BrgFbk.SDL43_ST.SDL_ID_DN == true)
            //{
                // on 2nd pass - read the size (bytes) of the auxiliary board id data from word #0 (byte 0,1)
                if ( TargInitPass == TargInitCnt )
                {
                    IdBufRgib3DataLen = BrgFbk.CrLb.LbReg;
                    if ( IdBufRgib3DataLen > (int)sizeof(IdBufRgib3) )
                    {
                        HsslErrFlagsIni |= TARG_INI_AUX1_ID_INV_SIZE;
                        PUSH_DIAG(HsslIniFail);
                        TargInitCase = 99;
                        TargInitPass = 0;
                        break;
                    }
                }

                // on 3rd+ pass - read the data into the buffer (exclude size in word #0)
                else
                {
                    IdBufRgib3[(TargInitPass - TargInitCnt)- 2] = (uint16_t)BrgFbk.CrLb.LbReg;
                }

                // check to see if we have read all the data

                if ( ((2*((int)TargInitPass - (int)TargInitCnt))-0) < IdBufRgib3DataLen )     // convert word index to byte index for comparision
                {
                    ++TargInitPass;
                }
                else
                {
                    // adjust data length to account for buffer not including size in first 2 bytes
                    IdBufRgib3DataLen -= 2;

                    // advance to the next state
                    TargInitCase++;
                    TargInitPass = 0;
                    ++TargSdlNum;
                    TargInitCnt  = 0;
                }
            //}
            break;

        //---------------------------------------------------------------------
        // read the auxiliary board list from ID bus 2 (REBF J2 and P2)
        case 9:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            // command aux board id data to be sent in the loopback register
            BrgCmd.CrCfg.CfgRegSel = RGIB_BrdId_offset >> 1;

            if ( TargInitCnt == 0)
            {
                TargInitPass = (2 * TargSdlNum + 1) * 512;
                ++TargInitCnt;
            }

            BrgCmd.CrCfg.CfgVal = TargInitPass;

            // on 1st pass - nothing to read
            if ( (TargInitCnt == 1))
            {
                memset( IdBufRgib4, 0, sizeof(IdBufRgib4) );    // clear the buffer
                TargInitCnt = TargInitPass;
                break;
            }


            //if ( BrgFbk.SDL43_ST.SDL_ID_DN == true)
            //{
                // on 2nd pass - read the size (bytes) of the auxiliary board id data from word #0 (byte 0,1)
                if ( TargInitPass == TargInitCnt )
                {
                    IdBufRgib4DataLen = BrgFbk.CrLb.LbReg;
                    if ( IdBufRgib4DataLen > (int)sizeof(IdBufRgib4) )
                    {
                        HsslErrFlagsIni |= TARG_INI_AUX1_ID_INV_SIZE;
                        PUSH_DIAG(HsslIniFail);
                        TargInitCase = 99;
                        TargInitPass = 0;
                        break;
                    }
                }

                // on 3rd+ pass - read the data into the buffer (exclude size in word #0)
                else
                {
                    IdBufRgib4[(TargInitPass - TargInitCnt)- 2] = (uint16_t)BrgFbk.CrLb.LbReg;
                }

                // check to see if we have read all the data

                if ( ((2*((int)TargInitPass - (int)TargInitCnt))-0) < IdBufRgib4DataLen )     // convert word index to byte index for comparision
                {
                    ++TargInitPass;
                }
                else
                {
                    // adjust data length to account for buffer not including size in first 2 bytes
                    IdBufRgib4DataLen -= 2;

                    // advance to the next state
                    TargInitCase++;
                    TargInitPass = 0;
                    ++TargSdlNum;
                    TargInitCnt  = 0;
                }
            //}
            break;



        //---------------------------------------------------------------------
        // read the RGIB plug information (upper CPLD)
        case 10:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            // command aux board id data to be sent in the loopback register
            BrgCmd.CrCfg.CfgRegSel = RGIB_BrdId_offset >> 1;
            BrgCmd.CrCfg.CfgVal = TargInitPass + 16;


            if ( (TargInitCnt % 2) != 0)
            {
                 IdBufPlug = (uint16_t)BrgFbk.CrLb.LbReg;
                 if ( strncmp(INS(Uniquifier), "L", 1) == 0 )
                 {
                    PlugInfoArr1[TargInitCnt2] = IdBufPlug & 0x00F0;
                    PlugInfoArr3[TargInitCnt2] = IdBufPlug & 0x0F00;

                    if ( PlugInfoArr1[TargInitCnt2] == 0x00B0)
                    {
                        PlugInfo = IdBufPlug;
                    }
                    else if ( PlugInfoArr1[TargInitCnt2] == 0x00A0)
                    {
                        PlugInfo2 = IdBufPlug;
                    }
                    else if ( PlugInfoArr1[TargInitCnt2] == 0x0090)
                    {
                        PlugInfo3 = IdBufPlug;
                    }
                    else if ( PlugInfoArr1[TargInitCnt2] == 0x0080)
                    {
                        PlugInfo4 = IdBufPlug;
                    }

                    TargInitCnt2++;
                 }
                 else
                 {
                    PlugInfoArr2[TargInitCnt2] = IdBufPlug & 0x00F0;
                    PlugInfoArr4[TargInitCnt2] = IdBufPlug & 0x0F00;
                    if ( PlugInfoArr2[TargInitCnt2] == 0x00E0)
                    {
                        PlugInfo = IdBufPlug;
                    }
                    else if ( PlugInfoArr2[TargInitCnt2] == 0x00D0)
                    {
                        PlugInfo2 = IdBufPlug;
                    }
                    else if ( PlugInfoArr2[TargInitCnt2] == 0x00C0)
                    {
                        PlugInfo3 = IdBufPlug;
                    }

                    TargInitCnt2++;
                 }
                 TargInitPass = (64 * 16) + TargInitPass;
                 TargInitCnt++;
            }
            else
            {
                TargInitCnt++;
            }

            if ( TargInitCnt == 8)
            {
                 TargInitCase++;
                 TargInitPass = 0;
                 TargInitCnt = 0;
                 TargInitCnt2 = 0;
            }

            break;

        //---------------------------------------------------------------------
        // read the RGIB plug information (upper CPLD)
        case 11:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            // command aux board id data to be sent in the loopback register
            BrgCmd.CrCfg.CfgRegSel = RGIB_BrdId_offset >> 1;
            BrgCmd.CrCfg.CfgVal = TargInitPass + 32;


            if ( (TargInitCnt % 2) != 0)
            {
                 IdBufPlug = (uint16_t)BrgFbk.CrLb.LbReg;
                 if ( strncmp(INS(Uniquifier), "L", 1) == 0 )
                 {
                    //PlugInfoArr1[TargInitCnt2] = IdBufPlug & 0x00F0;
                    //PlugInfoArr3[TargInitCnt2] = IdBufPlug & 0x0F00;

                    //PlugInfo = IdBufPlug;

                    if ( TargInitCnt2 == 0)
                    {
                        PlugInfoL = IdBufPlug;
                    }
                    else if ( TargInitCnt2 == 1)
                    {
                        PlugInfo2L = IdBufPlug;
                    }
                    else if ( TargInitCnt2 == 2)
                    {
                        PlugInfo3L = IdBufPlug;
                    }
                    else if ( TargInitCnt2 == 3)
                    {
                        PlugInfo4L = IdBufPlug;
                    }

                    TargInitCnt2++;
                 }
                 else
                 {
                    //PlugInfoArr2[TargInitCnt2] = IdBufPlug & 0x00F0;
                    //PlugInfoArr4[TargInitCnt2] = IdBufPlug & 0x0F00;

                    //PlugInfo = IdBufPlug;

                    if ( TargInitCnt2 == 0)
                    {
                        PlugInfoL = IdBufPlug;
                    }
                    else if ( TargInitCnt2 == 1)
                    {
                        PlugInfo2L = IdBufPlug;
                    }
                    else if ( TargInitCnt2 == 2)
                    {
                        PlugInfo3L = IdBufPlug;
                    }

                    TargInitCnt2++;
                 }
                 TargInitPass = (64 * 32) + TargInitPass;
                 TargInitCnt++;
            }
            else
            {
                TargInitCnt++;
            }

            if ( TargInitCnt == 8)
            {
                 TargInitCase++;
                 TargInitPass = 0;
                 TargInitCnt = 0;
            }

            break;



        //---------------------------------------------------------------------
        // decode the board list
        case 12:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            // decode the data from ID bus 1
            if ( Hssl.LinkIdInfo.DecodeAux((char*)IdBufAux1, IdBufAux1DataLen, 1, ErrCode) != SUCCESS )
            {
                HsslErrFlagsIni |= TARG_INI_AUX1_ID_DECODE_FAIL;
            }

            if ( RebhSet)
            {
               // decode the data from ID bus 1
               if ( Hssl.LinkIdInfo.DecodeAux((char*)IdBufRepi, IdBufRepiDataLen, 1, ErrCode) != SUCCESS )
               {
                   HsslErrFlagsIni |= TARG_INI_REPI_ID_DECODE_FAIL;
               }

               // decode the data from ID bus 1
               if ( Hssl.LinkIdInfo.DecodeAux((char*)IdBufRgib, IdBufRgibDataLen, 1, ErrCode) != SUCCESS )
               {
                   HsslErrFlagsIni |= TARG_INI_RIB1_ID_DECODE_FAIL;
               }

               if ( Hssl.LinkIdInfo.DecodeAux((char*)IdBufRgib2, IdBufRgib2DataLen, 1, ErrCode) != SUCCESS )
               {
                   HsslErrFlagsIni |= TARG_INI_RIB2_ID_DECODE_FAIL;
               }

               if ( Hssl.LinkIdInfo.DecodeAux((char*)IdBufRgib3, IdBufRgib3DataLen, 1, ErrCode) != SUCCESS )
               {
                   HsslErrFlagsIni |= TARG_INI_RIB3_ID_DECODE_FAIL;
               }

               if ( Hssl.LinkIdInfo.DecodeAux((char*)IdBufRgib4, IdBufRgib4DataLen, 1, ErrCode) != SUCCESS )
               {
                   HsslErrFlagsIni |= TARG_INI_RIB4_ID_DECODE_FAIL;
               }


            }

            // advance to the next state
            if ( HsslErrFlagsIni != 0 )
            {
                PUSH_DIAG(HsslIniFail);
                TargInitCase = 99;
                TargInitPass = 0;
            }
            else
            {
                TargInitCase++;
                TargInitPass = 0;
            }
            break;

        //---------------------------------------------------------------------
        // validate the board list
        case 13:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            // validate the pwas on ID bus 1 and 2
            if ( !RebhSet)
            {
               if ( ValidateAuxPwaList() != SUCCESS )  // Check Auxilary boards for REBF board-set
               {
                  HsslErrFlagsIni |= TARG_INI_AUX_ID_VALIDATE_FAIL;
               }
            }
            else
            {
               if ( ValdteRebhAuxPwaLst() != SUCCESS ) // Check Auxilary boards for REBH board-set
               {
                  HsslErrFlagsIni |= TARG_INI_AUX_ID_VALIDATE_FAIL;
               }
            }

            // set pwa ID information into the HW inventory
            Hssl.LinkIdInfo.SetHwInventory();

            // advance to the next state
            if ( HsslErrFlagsIni != 0 )
            {
                // expect that specific diagnostic is pushed by the ValidateAuxPwaList() function
                TargInitCase = 99;
                TargInitPass = 0;
                break;
            }
            else
            {
                // advance to the next state
                TargInitCase++;
                TargInitPass = 0;
            }
            break;

        //---------------------------------------------------------------------
        // AEAD calibration
        case 14:

            // enable slower task AEAD gain calcs and
            // advance to the next state when gain calc's done
            if (!AeadGnCalcDone)
            {
                AeadGnCalcEnb = true;
            }
            else
            {
                TargInitCase++;
                TargInitPass = 0;
            }
            break;

        //---------------------------------------------------------------------
        // AEAA calibration
        case 15:

            // enable slower task AEAA gain calcs and
            // advance to the next state when gain calc's done
            if (!AeaaGnCalcDone)
            {
                AeaaGnCalcEnb = true;
            }
            else
            {
                TargInitCase++;
                TargInitPass = 0;
            }
            break;

        //---------------------------------------------------------------------
        // write the configuration registers
        case 16:
            if ( TargInitPass < REBx_NUM_OF_CONFIG_REGS )
            {
                // write next configuration register
                BrgCmd.CrCfg.CfgRegSel = TargInitPass;
                BrgCmd.CrCfg.CfgVal = CfgVal.Val[TargInitPass];
                ++TargInitPass;
            }
            else
            {
                // advance to the next state
                TargInitCase++;
                TargInitPass = 0;
            }
            break;

        //---------------------------------------------------------------------
        // disable the ability to write to configuration bits
        case 17:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            if ( TargInitPass == 0 )
            {
                BrgCmd.CrCfg.CfgEnable = 0x0;
                ++TargInitPass;
            }
            else if ( BrgCfgEnabled )
            {
                HsslErrFlagsIni |= TARG_INI_CFG_DIS_FAIL;
                PUSH_DIAG(HsslIniFail);
                TargInitCase = 99;
                TargInitPass = 0;
            }
            else
            {
                // advance to the next state
                TargInitCase++;
                TargInitPass = 0;
            }
            break;

        //---------------------------------------------------------------------
        // configuration verification via readback
        case 18:

            // skip this step if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargInitCase++;
                TargInitPass = 0;
                break;
            }

            // save the readback values (delayed by 1 task from the request)
            if ( (TargInitPass > 0) && (TargInitPass <= REBx_NUM_OF_CONFIG_REGS) )
            {
                CfgReadBack[TargInitPass-1] = BrgFbk.CrLb.LbReg;
            }

            // control the register read back request
            if ( (TargInitPass >= 0) && (TargInitPass < REBx_NUM_OF_CONFIG_REGS) )
            {
                BrgCmd.CrCfg.CfgRegSel = TargInitPass;
                // re-write configuration registers since IOC enables are always writeable
                BrgCmd.CrCfg.CfgVal = CfgVal.Val[TargInitPass];
            }
// enable the following code to check the read back signals
#if 0
            ReadBack0 = CfgReadBack[0];
            ReadBack1 = CfgReadBack[1];
            ReadBack2 = CfgReadBack[2];
            ReadBack3 = CfgReadBack[3];
            ReadBack4 = CfgReadBack[4];
            ReadBack5 = CfgReadBack[5];
            ReadBack6 = CfgReadBack[6];
            ReadBack7 = CfgReadBack[7];

            CfgVa0    = CfgVal.Val[0];
            CfgVa1    = CfgVal.Val[1];
            CfgVa2    = CfgVal.Val[2];
            CfgVa3    = CfgVal.Val[3];
            CfgVa4    = CfgVal.Val[4];
            CfgVa5    = CfgVal.Val[5];
            CfgVa6    = CfgVal.Val[6];
            CfgVa7    = CfgVal.Val[7];
#endif
            // compare the readback values
            if ( TargInitPass == REBx_NUM_OF_CONFIG_REGS )
            {
                for ( unsigned i = 0 ; i < REBx_NUM_OF_CONFIG_REGS ; ++i )
                {

                    if ( CfgReadBack[i] != CfgVal.Val[i] )
                    {
                        HsslErrFlagsIni |= TARG_INI_READBACK_VAL_BAD;
                    }
                }

                // initialize  phase-D PE_EN configuration
                OUT(Brg1PhDenabled) = false;

                if ( (HsslErrFlagsIni & TARG_INI_READBACK_VAL_BAD) != 0 )
                {
                    PUSH_DIAG(HsslIniFail);
                    TargInitCase = 99;
                    TargInitPass = 0;
                    break;
                }
                else
                {
                    // readback phase-D PE_EN configuration: DDisPeEn (1= not affected by PE_EN, 0=affected by PE_EN)
                    if ( (CfgReadBack[2] & 0x4000) != 0 )  OUT(Brg1PhDenabled) = true;  // bit[14] of CrBrEnb1

                    // advance to the next state
                    TargInitCase++;
                    TargInitPass = 0;
                    break;
                }
            }

            // stay in this case but advance the pass counter which is used as an index for the read back
            ++TargInitPass;
            break;

        //---------------------------------------------------------------------
        // advance to the running state
        case 19:
            // toggle the bridge fault reset
            if ( BrgCmd.Cr21.BrgFltClear == false )
            {
                BrgCmd.Cr21.BrgFltClear = true;
                break;
            }
            else
            {
                BrgCmd.Cr21.BrgFltClear = false;
                if ( ++TargInitPass < 2 )       // wait for 2 toggles before advancing state to clear brg status loop fault
                {                           // inc here so that always exit this state with BrgFltClear == false
                    break;
                }
            }

            // advance to the init done state
            TargInitCase = 100;
            TargInitPass = 0;
            break;

        //---------------------------------------------------------------------
        // failure case
        case 99:

            // perform any needed housekeeping
            if ( TargInitPass == 0 )
            {
                // clear the config enable request
                BrgCmd.CrCfg.CfgEnable = 0x0;
                ++TargInitPass;
            }
            else if ( TargInitPass == 1 )
            {
                // disable error checks and disable the window transfers
                HsslErrChksEnb = false;
                for ( unsigned WinIdx = 0 ; WinIdx < ARRAY_LEN(WinInfo) ; ++WinIdx )
                {
                    Hssl.DisableWinTransfers( WinInfo[WinIdx].Idx, ErrCode );
                }
                ++TargInitPass;
            }
            else
            {
                // re-arm the target initialization sequence
                TargDscReq = true;          // request re-discovery of the link
                TargInitCase = -1;
                TargInitPass = 0;
            }
            break;

        //-----------------------------------------------------------------
        // init done
        case 100:

            break;

        //-----------------------------------------------------------------
        // default case ... should never get here
        default:

            HsslErrFlagsIni |= TARG_INI_INV_STATE;
            PUSH_DIAG(HsslIniFail);
            TargInitCase = 99;
            TargInitPass = 0;
            break;

    //-------------------------------------------------------------------------
    // end of switch (HsslInitCase) state machine to achieve initialization sequence
    }

    //-------------------------------------------------------------------------
    // assign done status based on state machine case
    (TargInitCase == 100) ? TargInitDone = true : TargInitDone = false;

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Populate the command gating registers to be sent to the bridge
//      interface by the HSSL.
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::XlateGateCmds (void)
{

    //-------------------------------------------------------------------------
    // populate the command registers for bridge 1 with phase transition list
    // from modulator/bridge test
    if ( INS(Brg1InfcHdw != BRG_INFC_NONE) )
    {
        if (IN(Brg1PhsTransList.Len) > REBx_NUM_OF_TRA_PER_PHS)
        {   // translation list size corrupted
            PUSH_DIAG(BrgFirXlateFlt);
        }
        else
        {
            //-------------------------------------------------------------------------
            // initialize the gate event array in the bridge command dma shadow
            //  (all commanded states are off, all enables are false)
            memset( (void*)BrgCmd.PhsGateEvtRegArr1, 0, sizeof(BrgCmd.PhsGateEvtRegArr1) );

            for ( unsigned tra = 0 ; tra < IN(Brg1PhsTransList.Len) ; ++tra )
            {
                for ( unsigned phs = 0 ; phs < NUM_OF_PHASES ; ++phs )
                {
                    BrgCmd.PhsGateEvtRegArr1[tra][phs].FrcDel = IN(Brg1PhsTransList).pTrans[tra][phs].DelTm;
                    BrgCmd.PhsGateEvtRegArr1[tra][phs].State  = IN(Brg1PhsTransList).pTrans[tra][phs].State;
                    BrgCmd.PhsGateEvtRegArr1[tra][phs].Enable = IN(Brg1PhsTransList).pTrans[tra][phs].Enable;

 #if REBx_DIAG_BRIDGE_EVENTS
                    switch (phs)
                    {
                        case 0:
                            switch (tra)
                            {
                                case 0:
                                    Gate1AEvtAr1Enb   = BrgCmd.PhsGateEvtRegArr1[tra][phs].Enable;
                                    Gate1AEvtAr1RelTm = BrgCmd.PhsGateEvtRegArr1[tra][phs].FrcDel;
                                    Gate1AEvtAr1State = BrgCmd.PhsGateEvtRegArr1[tra][phs].State;
                                    break;
                                case 1:
                                    Gate2AEvtAr1Enb   = BrgCmd.PhsGateEvtRegArr1[tra][phs].Enable;
                                    Gate2AEvtAr1RelTm = BrgCmd.PhsGateEvtRegArr1[tra][phs].FrcDel;
                                    Gate2AEvtAr1State = BrgCmd.PhsGateEvtRegArr1[tra][phs].State;
                                    break;
                                case 2:
                                    Gate3AEvtAr1Enb   = BrgCmd.PhsGateEvtRegArr1[tra][phs].Enable;
                                    Gate3AEvtAr1RelTm = BrgCmd.PhsGateEvtRegArr1[tra][phs].FrcDel;
                                    Gate3AEvtAr1State = BrgCmd.PhsGateEvtRegArr1[tra][phs].State;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        case 1:
                            switch (tra)
                            {
                                case 0:
                                    Gate1BEvtAr1Enb   = BrgCmd.PhsGateEvtRegArr1[tra][phs].Enable;
                                    Gate1BEvtAr1RelTm = BrgCmd.PhsGateEvtRegArr1[tra][phs].FrcDel;
                                    Gate1BEvtAr1State = BrgCmd.PhsGateEvtRegArr1[tra][phs].State;
                                    break;
                                case 1:
                                    Gate2BEvtAr1Enb   = BrgCmd.PhsGateEvtRegArr1[tra][phs].Enable;
                                    Gate2BEvtAr1RelTm = BrgCmd.PhsGateEvtRegArr1[tra][phs].FrcDel;
                                    Gate2BEvtAr1State = BrgCmd.PhsGateEvtRegArr1[tra][phs].State;
                                    break;
                                case 2:
                                    Gate3BEvtAr1Enb   = BrgCmd.PhsGateEvtRegArr1[tra][phs].Enable;
                                    Gate3BEvtAr1RelTm = BrgCmd.PhsGateEvtRegArr1[tra][phs].FrcDel;
                                    Gate3BEvtAr1State = BrgCmd.PhsGateEvtRegArr1[tra][phs].State;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        case 2:
                            switch (tra)
                            {
                                case 0:
                                    Gate1CEvtAr1Enb   = BrgCmd.PhsGateEvtRegArr1[tra][phs].Enable;
                                    Gate1CEvtAr1RelTm = BrgCmd.PhsGateEvtRegArr1[tra][phs].FrcDel;
                                    Gate1CEvtAr1State = BrgCmd.PhsGateEvtRegArr1[tra][phs].State;
                                    break;
                                case 1:
                                    Gate2CEvtAr1Enb   = BrgCmd.PhsGateEvtRegArr1[tra][phs].Enable;
                                    Gate2CEvtAr1RelTm = BrgCmd.PhsGateEvtRegArr1[tra][phs].FrcDel;
                                    Gate2CEvtAr1State = BrgCmd.PhsGateEvtRegArr1[tra][phs].State;
                                    break;
                                case 2:
                                    Gate3CEvtAr1Enb   = BrgCmd.PhsGateEvtRegArr1[tra][phs].Enable;
                                    Gate3CEvtAr1RelTm = BrgCmd.PhsGateEvtRegArr1[tra][phs].FrcDel;
                                    Gate3CEvtAr1State = BrgCmd.PhsGateEvtRegArr1[tra][phs].State;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        default:
                            break;
                    }
#endif
                }
            }
        }

    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Populate command registers to be sent to bridge interface fpga
//      via the HSSL.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::CommandCalc(void)
{
    //-------------------------------------------------------------------------
    // drive Diag1 pin
    BrgCmd.CrCfg.DiagFromFw1   = TripFlt;      // trigger for ChipScope

    //-------------------------------------------------------------------------
    // drive the relay outputs
    BrgCmd.Cr21.LvRelayState  = (IN(OutLvRly03) << REBF_LV_RELAY03) | (IN(OutLvRly02) << REBF_LV_RELAY02) | IN(OutLvRly01);
    BrgCmd.Cr21.Relay101State = IN(OutRly101);
    BrgCmd.Cr21.Relay102State = IN(OutRly102);
    BrgCmd.Cr21.Relay103State = IN(OutRly103);
    BrgCmd.Cr21.Relay104State = IN(OutRly104);
    BrgCmd.Cr21.Relay105State = IN(OutRly105);
    BrgCmd.Cr21.Relay106State = IN(OutRly106);
    BrgCmd.Cr21.Relay107State = IN(OutRly107);
    BrgCmd.Cr21.Relay108State = IN(OutRly108);
    BrgCmd.Cr21.Relay109State = IN(OutRly109);
    //-------------------------------------------------------------------------
    // drive the DB command
     BrgCmd.Cr21.ForceDb  = IN(BrgCmdDb);

    //-------------------------------------------------------------------------
    // translate gate commands from delta-times to FRC times
    XlateGateCmds();

    //-------------------------------------------------------------------------
    // manage the bridge 1 power electronics enable state
    if ( !IN(Brg1EnableReq) || !TargInitDone )
    {
        Brg1EnabCtr = 0;
    }
    if ( Brg1EnabCtr < 3 )
    {
        BrgCmd.Cr21.BrgEnable = Brg1EnabCtr;
        ++Brg1EnabCtr;
    }

    //-------------------------------------------------------------------------
    // update the event time information
    BrgCmd.LoadPulseTimeReg.LdPls1Frc = IN(FrameFrc);
    BrgCmd.LoadPulseTimeReg.LdPls2FrcDel = INS(Clk0PerFrm);

    //-------------------------------------------------------------------------
    // select temperature range for thermistor sensing
    BrgCmd.Cr21.ThermHi = IN(ThermHi);

    //-------------------------------------------------------------------------
    // Bridge ready loop Tx (1= Disable local BRL TX which creates BRlRxFlt on connected REBF.)
    BrgCmd.Cr21.BrlTxDis = IN(BrgRdyLpTxDis);

    if ( IN(DisCmdNotFunc)  )
    {
        BrlDisDlyTmr += INS(DelTm1);
        if ( BrlDisDlyTmr > INS(DelTm1))
        {
           BrgCmd.Cr21.BrlTxDis = true;
        }
    }

    //-------------------------------------------------------------------------
    // set the h/w current limit
    BrgCmd.FanRef.IlimEnb = IN(IlimEnb);

    //-------------------------------------------------------------------------
    // set the IOC fault enables
    if ( IN(MaskIocFlts) || ( INS(Brg1InfcHdw)==BRG_INFC_NONE ) )
    {
        *(unsigned*)&BrgCmd.FanRef &= REBx_Disab_Abc_Ioc_Mask_Br1;
    }
    else
    {
        *(unsigned*)&BrgCmd.FanRef |= REBx_Enab_Abc_Ioc_Mask_Br1;
    }

    // always disable fpga IOC for phase-D
    BrgCmd.FanRef.DiocEnbBr1 = false;

    //-------------------------------------------------------------------------
    // set the DESAT fault enables for Bridge 1 A-B-C
    if ( INS(Brg1InfcHdw)==BRG_INFC_NONE )
    {
        BrgCmd.FanRef.DesatEnbBr1 = false;
    }
    else
    {
        if ( INS(SimMode) )
        {
            if ( PARM(HwGdStatMask)==0 )
            {
                BrgCmd.FanRef.DesatEnbBr1 = false;
            }
            else
            {
                BrgCmd.FanRef.DesatEnbBr1 = true;
            }
        }
        else
        {
            if ( IN(MaskDsatFlts) )
            {
                BrgCmd.FanRef.DesatEnbBr1 = false;
            }
            else
            {
                BrgCmd.FanRef.DesatEnbBr1 = true;
            }
        }
    }

    //-------------------------------------------------------------------------
    // control fan speed
    BrgCmd.FanRef.FanRef1 = static_cast<unsigned> ( IN(FanRef1) * 255.0F);
    BrgCmd.FanRef.FanRef2 = static_cast<unsigned> ( IN(FanRef2) * 255.0F );

    //-------------------------------------------------------------------------
    // increment the packet counter that will be returned in feedback packet
    ++CmdPktCnt;
    BrgCmd.PktCnt.Cnt = CmdPktCnt;

#if REBx_DIAG_FPGA_REGISTERS
    //-------------------------------------------------------------------------
    // update the published registers for diagnostics
    FrameFrc16 = IN(FrameFrc) & REBx_FRC_MASK;
    LoadPulseTimeReg = *(unsigned*)&BrgCmd.LoadPulseTimeReg;
#endif

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculate analog feedbacks from fpga vco counts.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::FeedbackCalc(void)
{
    unsigned temp;

    //-------------------------------------------------------------------------
    // dont start caclculating feedbacks until after data is valid
    if ( !TargInitDone ||  (HsslErrFlagsAcc != 0) )
    {
        OUT(BrgHdwDataVld) = false; // feedbacks and other data provided by bridge-interface are not valid
        FeedbackZero();
        return;
    }

    OUT(BrgHdwDataVld) = true;      // feedbacks and other data provided by bridge-interface are valid

    //-----------------------------------------------------------------------------
    // Bridge 1 feedbacks
    //-----------------------------------------------------------------------------
    if ( INS(Brg1InfcHdw != BRG_INFC_NONE) )
    {
        if ( Brg1InfcBoardAedb )
        {
            //--------------------------------------------------------------------------
            // calc VCO voltage feedback rotor phase D; this could be voltage across DB1
            // resistor or Dc-chopper phase voltage
            OUT(VdBrg1Hdw)        =  ((int)((BRG_VCO_MASK & (BrgFbk.VcoBr1.Vdc   - VcoBr1Z1.Vdc   ))) * INS(DelTm1Rcp) - AEDB_VDC_FBK_OFFSET_FREQ  ) * SclLinkVoltsBrg1;
        }
        else
        {
            //---------------------------------------------------------------------
            // calc VCO link voltage feedback for line side
            //if ( !RebhSet)
            if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
            {
//              PreREBH: now conditioned here
                OUT(VdBrg1Hdw)    =  (((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Vdc   - VcoBr1Z1.Vdc   ))  * INS(DelTm1Rcp)                      ) * SclLinkVoltsBrg1);
            }
            else
            {
//              REBH: new code
                OUT(VdBrg1Hdw)    = ((((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Vdc   - VcoBr1Z1.Vdc   ))  * INS(DelTm1Rcp)                      ) * SclPhsVoltsBrg1)- SclVoltBrgOffset)/2;
            }
        }

        //-------------------------------------------------------------------------
        // calc bridge voltage from  VCO feedbacks
        //if ( !RebhSet)
        if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
        {
//          PreREBH: now conditioned here
            VcoSampBrg1HdwScl.Vab =  (((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Vab   - VcoBr1Z1.Vab   ))  * INS(DelTm1Rcp) - VcoBiasFreqBrg1    ) * SclPhsVoltsBrg1);
            VcoSampBrg1HdwScl.Vbc =  (((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Vbc   - VcoBr1Z1.Vbc   ))  * INS(DelTm1Rcp) - VcoBiasFreqBrg1    ) * SclPhsVoltsBrg1);

            OUT(VabFreq)       = (int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Vab - VcoBr1Z1.Vab)) * INS(DelTm1Rcp);
            OUT(VbcFreq)       = (int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Vbc - VcoBr1Z1.Vbc)) * INS(DelTm1Rcp);
            OUT(VabBrgVcoCnt) = BrgFbk.VcoBr1.Vab;
            OUT(VbcBrgVcoCnt) = BrgFbk.VcoBr1.Vbc;
        }
        else
        {
//          REBH: new code///
            VcoSampBrg1HdwScl.Va  = (((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Vab    - VcoBr1Z1.Vab   ))  * INS(DelTm1Rcp)                      ) * SclPhsVoltsBrg1)- SclVoltBrgOffset;
            VcoSampBrg1HdwScl.Vb  = (((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Vbc    - VcoBr1Z1.Vbc   ))  * INS(DelTm1Rcp)                      ) * SclPhsVoltsBrg1)- SclVoltBrgOffset;
            VcoSampBrg1HdwScl.Vc  = (((int)(BRG_VCO_MASK & (BrgFbk.VdcSDL5_ST.Vc - VdcSDL5_STZ1.Vc))  * INS(DelTm1Rcp)                      ) * SclPhsVoltsBrg1)- SclVoltBrgOffset;
//          Note: "*0.5"
            VcoSampBrg1HdwScl.Vab = (VcoSampBrg1HdwScl.Va - VcoSampBrg1HdwScl.Vb) * 0.5F;
            VcoSampBrg1HdwScl.Vbc = (VcoSampBrg1HdwScl.Vb - VcoSampBrg1HdwScl.Vc) * 0.5F;

            OUT(VabFreq) = ((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Vdc - VcoBr1Z1.Vdc)) * INS(DelTm1Rcp));
            OUT(VbcFreq) =  (int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Vbc - VcoBr1Z1.Vbc)) * INS(DelTm1Rcp) ;
            OUT(VabBrgVcoCnt) = BrgFbk.VcoBr1.Vdc;
            OUT(VbcBrgVcoCnt) = BrgFbk.VcoBr1.Vbc;
        }

        Brg1HdwInOffs.Vax = VcoSampBrg1HdwScl.Vab;
        Brg1HdwInOffs.Vbx = VcoSampBrg1HdwScl.Vbc;

        //-------------------------------------------------------------------------
        // calc bridge phase currents from VCO feedbacks
        //if ( !RebhSet)
        if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
        {
//          PreREBH, now conditioned here
            VcoSampBrg1HdwScl.Ia  = (((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Ia     - VcoBr1Z1.Ia    ))  * INS(DelTm1Rcp) - (float)BRG_VCO_FZRO) * SclPhsCurBrg1);
            VcoSampBrg1HdwScl.Ib  = (((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Ib     - VcoBr1Z1.Ib    ))  * INS(DelTm1Rcp) - (float)BRG_VCO_FZRO) * SclPhsCurBrg1);
            VcoSampBrg1HdwScl.Ic  = (((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Ic     - VcoBr1Z1.Ic    ))  * INS(DelTm1Rcp) - (float)BRG_VCO_FZRO) * SclPhsCurBrg1);
        }
        else
        {
//          REBH: new code
            VcoSampBrg1HdwScl.Ia  = (((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Ia     - VcoBr1Z1.Ia    ))  * INS(DelTm1Rcp)                      ) * SclPhsCurBrg1  ) - BRG_I_CNT_OFFSET;
            VcoSampBrg1HdwScl.Ib  = (((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Ib     - VcoBr1Z1.Ib    ))  * INS(DelTm1Rcp)                      ) * SclPhsCurBrg1  ) - BRG_I_CNT_OFFSET;
            VcoSampBrg1HdwScl.Ic  = (((int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Ic     - VcoBr1Z1.Ic    ))  * INS(DelTm1Rcp)                      ) * SclPhsCurBrg1  ) - BRG_I_CNT_OFFSET;
        }

        OUT(IaFreq) = (int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Ia - VcoBr1Z1.Ia)) * INS(DelTm1Rcp);
        OUT(IbFreq) = (int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Ib - VcoBr1Z1.Ib)) * INS(DelTm1Rcp);
        OUT(IcFreq) = (int)(BRG_VCO_MASK & (BrgFbk.VcoBr1.Ic - VcoBr1Z1.Ic)) * INS(DelTm1Rcp);
        OUT(IaBrgVcoCnt) = BrgFbk.VcoBr1.Ia;
        OUT(IbBrgVcoCnt) = BrgFbk.VcoBr1.Ib;
        OUT(IcBrgVcoCnt) = BrgFbk.VcoBr1.Ic;

        Brg1HdwInOffs.Ia = VcoSampBrg1HdwScl.Ia;
        Brg1HdwInOffs.Ib = VcoSampBrg1HdwScl.Ib;
        Brg1HdwInOffs.Ic = VcoSampBrg1HdwScl.Ic;

        //-------------------------------------------------------------------------
        // perform the feedback offset calculations
        IN(Brg1FbkOffs.Task1(Brg1HdwInOffs, Brg1HdwOffs));

        //-------------------------------------------------------------------------
        // output bridge feedbacks
        // =======================
        OUT(VabBrg1Hdw) = VcoSampBrg1HdwScl.Vab - Brg1HdwOffs.Vax;
        OUT(VbcBrg1Hdw) = VcoSampBrg1HdwScl.Vbc - Brg1HdwOffs.Vbx;
        OUT(IaBrg1Hdw)  = VcoSampBrg1HdwScl.Ia  - Brg1HdwOffs.Ia;
        OUT(IbBrg1Hdw)  = VcoSampBrg1HdwScl.Ib  - Brg1HdwOffs.Ib;
        OUT(IcBrg1Hdw)  = VcoSampBrg1HdwScl.Ic  - Brg1HdwOffs.Ic;

        //-------------------------------------------------------------------------
        // save vco values for next delta calc
        VcoBr1Z1 = BrgFbk.VcoBr1;

//      REBF: new code
        VdcSDL5_STZ1 = BrgFbk.VdcSDL5_ST;

        //-------------------------------------------------------------------------
        // output temperature VCO counts

//      REBH: rename ".BrgCellTemp" to ".Brg1CellTemp":
        OUT(CellTempVcoBrg1) = BrgFbk.VcoTemp21.Brg1CellTemp;

    }

    //-----------------------------------------------------------------------------
    // calc VCO DC link voltage feedback via the fiber-optic from other REBx
    //if ( !RebhSet)
    if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
    {
//      PreREBH+RENAME(VcoVd-->VcoVdb):
        OUT(VdcFiberBrgHdw)       =  ((    (BRG_VCO_MASK & (BrgFbk.VcoVdb.Vdcx- VcoVdbZ1.Vdcx     ))  * INS(DelTm1Rcp)                      ) * SclLinkVoltsBrg1);
    }
    else
    {
        OUT(VdcFiberBrgHdw)       = (((    (BRG_VCO_MASK & (BrgFbk.VcoVdb.Vdcx- VcoVdbZ1.Vdcx     ))  * INS(DelTm1Rcp)                      ) * SclLinkVoltsBrg1) - SclVoltBrgOffset)/2;
    }

    VcoVdbZ1             = BrgFbk.VcoVdb;

    //-------------------------------------------------------------------------
    // IO 1 feedbacks
    //-------------------------------------------------------------------------
    if ( INS(Io1InfcHdw != IO_INFC_NONE) )
    {
        //-------------------------------------------------------------------------
        // calc IO card  voltages from  VCO feedbacks
        VcoSampIo1HdwScl.Vax = (((int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A01 - VcoIo1Z1.A01)) * INS(DelTm1Rcp) - Io1AeaxAin2Zero) * Io1VanScl);
        VcoSampIo1HdwScl.Vbx = (((int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A02 - VcoIo1Z1.A02)) * INS(DelTm1Rcp) - Io1AeaxAin3Zero) * Io1VbnScl);
        VcoSampIo1HdwScl.Vcx = (((int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A03 - VcoIo1Z1.A03)) * INS(DelTm1Rcp) - Io1AeaxAin4Zero) * Io1VcnScl);

        OUT(VaxIoVcoCnt) = BrgFbk.VcoIo1.A01;
        OUT(VbxIoVcoCnt) = BrgFbk.VcoIo1.A02;
        OUT(VcxIoVcoCnt) = BrgFbk.VcoIo1.A03;

        Io1HdwInOffs.Vax = VcoSampIo1HdwScl.Vax;
        Io1HdwInOffs.Vbx = VcoSampIo1HdwScl.Vbx;
        Io1HdwInOffs.Vcx = VcoSampIo1HdwScl.Vcx;
        //-------------------------------------------------------------------------
        // calc IO card  phase currents from VCO feedbacks
        VcoSampIo1HdwScl.Ia = (((int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A05 - VcoIo1Z1.A05)) * INS(DelTm1Rcp) - Io1AeaxAin5Zero) * Io1IaScl);
        VcoSampIo1HdwScl.Ib = (((int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A06 - VcoIo1Z1.A06)) * INS(DelTm1Rcp) - Io1AeaxAin6Zero) * Io1IbScl);
        VcoSampIo1HdwScl.Ic = (((int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A07 - VcoIo1Z1.A07)) * INS(DelTm1Rcp) - Io1AeaxAin7Zero) * Io1IcScl);

        OUT(AeaxAin1Freq) = (int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A01 - VcoIo1Z1.A01)) * INS(DelTm1Rcp);
        OUT(AeaxAin2Freq) = (int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A02 - VcoIo1Z1.A02)) * INS(DelTm1Rcp);
        OUT(AeaxAin3Freq) = (int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A03 - VcoIo1Z1.A03)) * INS(DelTm1Rcp);

        OUT(AeaxAin5Freq) = (int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A05 - VcoIo1Z1.A05)) * INS(DelTm1Rcp);
        OUT(AeaxAin6Freq) = (int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A06 - VcoIo1Z1.A06)) * INS(DelTm1Rcp);
        OUT(AeaxAin7Freq) = (int)(IO_VCO_MASK & (BrgFbk.VcoIo1.A07 - VcoIo1Z1.A07)) * INS(DelTm1Rcp);

        OUT(IaIoVcoCnt) = BrgFbk.VcoIo1.A05;
        OUT(IbIoVcoCnt) = BrgFbk.VcoIo1.A06;
        OUT(IcIoVcoCnt) = BrgFbk.VcoIo1.A07;

        Io1HdwInOffs.Ia = VcoSampIo1HdwScl.Ia;
        Io1HdwInOffs.Ib = VcoSampIo1HdwScl.Ib;
        Io1HdwInOffs.Ic = VcoSampIo1HdwScl.Ic;

        //-------------------------------------------------------------------------
        // perform the feedback offset calculations
        IN(Io1FbkOffs.Task1(Io1HdwInOffs, Io1HdwOffs));

        //-------------------------------------------------------------------------
        // output the feedbacks
        OUT(VaxIo1) = VcoSampIo1HdwScl.Vax - Io1HdwOffs.Vax;
        OUT(VbxIo1) = VcoSampIo1HdwScl.Vbx - Io1HdwOffs.Vbx;
        OUT(VcxIo1) = VcoSampIo1HdwScl.Vcx - Io1HdwOffs.Vcx;
        OUT(IaIo1) = VcoSampIo1HdwScl.Ia - Io1HdwOffs.Ia;
        OUT(IbIo1) = VcoSampIo1HdwScl.Ib - Io1HdwOffs.Ib;
        OUT(IcIo1) = VcoSampIo1HdwScl.Ic - Io1HdwOffs.Ic;

        OUT(VcxOffset) = Io1HdwOffs.Vcx;  // Needed for AeaaHv3Fbk signal when the input is DC

        //-------------------------------------------------------------------------
        // save vco values for next delta calc
        VcoIo1Z1 = BrgFbk.VcoIo1;

        //-------------------------------------------------------------------------
        // output temperature VCO counts
        OUT(ThermistorVcoIo1) = BrgFbk.CrLb.VcoT03;

        //-------------------------------------------------------------------------
        // Output humidity sensor data
//      REBH: new code
        OUT(HumidThermTemp) = BrgFbk.BrStatFlt.TempCode;
        OUT(HumidThermPct)  = BrgFbk.BrStatFlt.RhCode;

        //----------------------------------------------
        // Io 1 digital inputs   (invert the sense so that closed = true)
        OUT(ContactIn1Io1) =  !(BrgFbk.StatReg21.ContInb & 0x01);
        OUT(ContactIn2Io1) =  !(BrgFbk.StatReg21.ContInb & 0x02);;
        OUT(ContactIn3Io1) =  !(BrgFbk.StatReg21.ContInb & 0x04);
        OUT(ContactIn4Io1) =  !(BrgFbk.StatReg21.ContInb & 0x08);
        OUT(ContactIn5Io1) =  !(BrgFbk.StatReg21.ContInb & 0x10);
        OUT(ContactIn6Io1) =  !(BrgFbk.StatReg21.ContInb & 0x20);
        OUT(ContactIn7Io1) =  !(BrgFbk.StatReg21.ContInb & 0x40);
        OUT(ContactIn8Io1) =  !(BrgFbk.StatReg21.ContInb & 0x80);
        OUT(ContactIn9Io1) =  !(BrgFbk.StatReg21.ContInb & 0x100);
        OUT(ContactIn10Io1) = !(BrgFbk.StatReg21.ContInb & 0x200);
    }

    //-------------------------------------------------------------------------
    // Rebf active digital input
    OUT(ActiveDigIn1) = !(BrgFbk.StatReg21.DIn01);

    //-------------------------------------------------------------------------
    // tach inputs
    OUT(TachValue)     = BrgFbk.Tach01.TachV;
    OUT(TachMrkrValue) = BrgFbk.Tach01.TachM;
    OUT(TachTm)        = BrgFbk.Tach01.TachT;
    OUT(TachPm)        = BrgFbk.Tach01.TachP;
    OUT(TachPhases)    = BrgFbk.Tach01.TachD;

    //-------------------------------------------------------------------------
    // bridge temperature mux selection
    OUT(BrgTempVcoSel) = BrgFbk.StatCom.TempVcoSel;

    //-------------------------------------------------------------------------
    // board temperature
    // 11 bit with two's-complement representation for negative numbers
    temp = BrgFbk.StatCom.Ambient;
    if ( (temp & 0x400) == 0 )
    {
        OUT(AmbientTemp) = 0.25F * (int)temp;   // positive numbser
    }
    else
    {
        OUT(AmbientTemp) = -0.25F * (int)(0x800 - temp);    // negative number
    }


    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Zero all class outputs.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::FeedbackZero(void)
{
    OUT(VdBrg1Hdw)      = 0.0F;
    OUT(VabBrg1Hdw)     = 0.0F;
    OUT(VbcBrg1Hdw)     = 0.0F;
    OUT(IaBrg1Hdw)      = 0.0F;
    OUT(IbBrg1Hdw)      = 0.0F;
    OUT(IcBrg1Hdw)      = 0.0F;
    OUT(IdBrg1Hdw)      = 0.0F;
    VcoBr1Z1 = BrgFbk.VcoBr1;
    VdcSDL5_STZ1 = BrgFbk.VdcSDL5_ST;

    OUT(VdcFiberBrgHdw) = 0.0F;
    VcoVdbZ1 = BrgFbk.VcoVdb;

    OUT(VaxIo1)         = 0.0F;
    OUT(VbxIo1)         = 0.0F;
    OUT(VcxIo1)         = 0.0F;
    OUT(IaIo1)          = 0.0F;
    OUT(IbIo1)          = 0.0F;
    OUT(IcIo1)          = 0.0F;
    VcoIo1Z1 = BrgFbk.VcoIo1;

    OUT(AmbientTemp)      = 0.0F;
    OUT(CellTempVcoBrg1)  = 0;
    OUT(ThermistorVcoIo1) = 0;
    OUT(BrgTempVcoSel)    = 0;

    Brg1HdwInOffs.Vax    = 0.0F;
    Brg1HdwInOffs.Vbx    = 0.0F;
    Brg1HdwInOffs.Vcx    = 0.0F;
    Brg1HdwInOffs.Ia     = 0.0F;
    Brg1HdwInOffs.Ib     = 0.0F;
    Brg1HdwInOffs.Ic     = 0.0F;
    IN(Brg1FbkOffs.Task1(Brg1HdwInOffs, Brg1HdwOffs));

    Io1HdwInOffs.Vax    = 0.0F;
    Io1HdwInOffs.Vbx    = 0.0F;
    Io1HdwInOffs.Vcx    = 0.0F;
    Io1HdwInOffs.Ia     = 0.0F;
    Io1HdwInOffs.Ib     = 0.0F;
    Io1HdwInOffs.Ic     = 0.0F;
    IN(Io1FbkOffs.Task1(Io1HdwInOffs, Io1HdwOffs));

    OUT(ContactIn1Io1)     = false;
    OUT(ContactIn2Io1)     = false;
    OUT(ContactIn3Io1)     = false;
    OUT(ContactIn4Io1)     = false;
    OUT(ContactIn5Io1)     = false;
    OUT(ContactIn6Io1)     = false;
    OUT(ContactIn7Io1)     = false;
    OUT(ContactIn8Io1)     = false;
    OUT(ContactIn9Io1)     = false;
    OUT(ContactIn10Io1)    = false;

    OUT(TachValue)      = 0;
    OUT(TachMrkrValue)  = 0;

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Check for bridge faults detected by bridge interface fpga logic.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::CheckBridgeStatus(void)
{
    REBx_BrStatFltTyp BrgStatFlts;

    //-------------------------------------------------------------------------
    // dont start checking for faults
    if ( !TargInitDone )
    {
        return;
    }

#if REBx_DIAG_FAULT_TEST  // test code for simulating faults
    if ( PARM(BrgFltTst) )
    {
        *(unsigned*)&BrgFbk.BrStatFlt = PARM(BrStatFltTst);
        *(unsigned*)&BrgFbk.StatCom   = PARM(StatComTst);
        *(unsigned*)&BrgFbk.StatReg21 = PARM(StatReg21Tst);
        *(unsigned*)&BrgFbk.MiscStat  = PARM(MiscStatTst);
    }
#endif

    // Copy to published var in case desired to see on capture buffer
    BrgStatFlg = *(unsigned *)&BrgFbk.BrStatFlt;
    StatCom    = *(unsigned *)&BrgFbk.StatCom;
    StatReg21  = *(unsigned *)&BrgFbk.StatReg21;

    //-----------------------------------------------------------------------------------------------------------------------------
    // Output bridge 1 IOC indications (dynamically configured to set the fault latch in some conditions)
    if ( BrgFbk.BrStatFlt.AiocB1 )
    {
        OUT(Brg1DsatIoc).IocPhsA = true;
    }
    else
    {
        OUT(Brg1DsatIoc).IocPhsA = false;
    }

    if ( BrgFbk.BrStatFlt.BiocB1 )
    {
        OUT(Brg1DsatIoc).IocPhsB = true;
    }
    else
    {
        OUT(Brg1DsatIoc).IocPhsB = false;
    }

    if ( BrgFbk.BrStatFlt.CiocB1 )
    {
        OUT(Brg1DsatIoc).IocPhsC = true;
    }
    else
    {
        OUT(Brg1DsatIoc).IocPhsC = false;
    }


    if ( BrgStatFlts.AnegB1 )
    {
       OUT(Brg1DsatIoc).DsatSwAL = true;
    }
    else
    {
       OUT(Brg1DsatIoc).DsatSwAL = false;
    }
    if ( BrgStatFlts.BnegB1 )
    {
       OUT(Brg1DsatIoc).DsatSwBL = true;
    }
    else
    {
       OUT(Brg1DsatIoc).DsatSwBL = false;
    }
    if ( BrgStatFlts.CnegB1 )
    {
        OUT(Brg1DsatIoc).DsatSwCL = true;
    }
    else
    {
        OUT(Brg1DsatIoc).DsatSwCL = false;
    }
    if ( BrgStatFlts.AposB1 )
    {
        OUT(Brg1DsatIoc).DsatSwAU = true;
    }
    else
    {
        OUT(Brg1DsatIoc).DsatSwAU = false;
    }
    if ( BrgStatFlts.BposB1 )
    {
        OUT(Brg1DsatIoc).DsatSwBU = true;
    }
    else
    {
        OUT(Brg1DsatIoc).DsatSwBU = false;
    }
    if ( BrgStatFlts.CposB1 )
    {
        OUT(Brg1DsatIoc).DsatSwCU = true;
    }
    else
    {
        OUT(Brg1DsatIoc).DsatSwCU = false;
    }

    //-----------------------------------------------------------------------------------------------------------------------------
    // check for phase-D desats (these are not configured to set the fpga bridge-fault latch)
    PhsDdsatFltLatch = false;

    if ( BrgFbk.BrStatFlt.DdesatB1 && DesatFltMskBr1.DdesatB1 && !( ((BrgInfcHdwTyp)(INS(Brg1InfcHdw))) == BRG_INFC_AEBIH3 ))
    {
        OUT(Brg1DsatIoc).DsatSwD = true;
        PhsDdsatFltLatch         = true;
        PUSH_DIAG(DesatFltD);
    }
    else
    {
        OUT(Brg1DsatIoc).DsatSwD = false;
    }

    //-----------------------------------------------------------------------------------------------------------------------------
    // check for phase-D ioc (this is not configured to set the fpga fault latch)
    if ( BrgFbk.BrStatFlt.DiocB1 && StatIocFltMskBr1.DiocB1 && !( ((BrgInfcHdwTyp)(INS(Brg1InfcHdw))) == BRG_INFC_AEBIH3 ) )
    {
        OUT(Brg1DsatIoc).IocPhsD = true;
        PUSH_DIAG(IocPhsD);
    }
    else
    {
        OUT(Brg1DsatIoc).IocPhsD = false;
    }

    //-----------------------------------------------------------------------------------------------------------------------------
    // check for bridge faults that are configured to set the fpga fault latch
    if (!BrgFbk.StatCom.DriveFlt)
    {
        OUT(FpgaFltLatch)         = false;    // reset fpga fault latch
        OUT(Brg1DsatIoc).DsatSwAL = false;
        OUT(Brg1DsatIoc).DsatSwBL = false;
        OUT(Brg1DsatIoc).DsatSwCL = false;
        OUT(Brg1DsatIoc).DsatSwAU = false;
        OUT(Brg1DsatIoc).DsatSwBU = false;
        OUT(Brg1DsatIoc).DsatSwCU = false;

        BrgFaultActZ = false;           // reset the flag to detect transition from no fault to fault
        BrgFaultResolved = false;       // reset the bridge fault "resovled" flag

        // bridge 1 fault checks
        if (INS(Brg1InfcHdw != BRG_INFC_NONE))
        {
            // check gate drive power supply problems if no faults and bridge power enable
            if ( !(BrgFbk.StatCom.DrPc & 0x1) && Brg1Enabled && !RebhSet)
            {
                PUSH_DIAG(GdPwrSup1Fail);
            }
        }
    }
    else
    {
        // an fpga bridge fault has been latched, resolve into a specific fault
        OUT(FpgaFltLatch) = true;

        // bridge 1 fault checks
        if (INS(Brg1InfcHdw != BRG_INFC_NONE))
        {

            if ( (BrgFbk.BrStatFlt.FltBr1))
            {
                // check for bridge 1 IOC faults
                *(unsigned*)&BrgStatFlts = *(unsigned*)&BrgFbk.BrStatFlt & (*(unsigned*)&StatIocFltMskBr1);
                if (*(unsigned*)&BrgStatFlts != 0x0)
                {
                    BrgFaultResolved = true;

                    if ( IN(Brg1PhsTransList).pBrgTstCtl->BrgTestReq == false )
                    {
                        if ( BrgStatFlts.AiocB1 )
                        {
                            PUSH_DIAG(IocPhsA1);
                        }
                        if ( BrgStatFlts.BiocB1 )
                        {
                            PUSH_DIAG(IocPhsB1);
                        }
                        if ( BrgStatFlts.CiocB1 )
                        {
                            PUSH_DIAG(IocPhsC1);
                        }
                    }
                }

                *(unsigned*)&BrgStatFlts = *(unsigned*)&BrgFbk.BrStatFlt & (*(unsigned*)&DesatFltMskBr1);
                if (*(unsigned*)&BrgStatFlts != 0x0)
                {
                    BrgFaultResolved = true;
                    if ( BrgStatFlts.AnegB1 && (strncmp(INS(Uniquifier), "L", 1) != 0 ))
                    {
                        if ( IN(Brg1PhsTransList).pBrgTstCtl->BrgTestReq == false ) { PUSH_DIAG(DesatFltAL); };
                    }
                    if ( BrgStatFlts.BnegB1 && (strncmp(INS(Uniquifier), "L", 1) != 0 ))
                    {
                        if ( IN(Brg1PhsTransList).pBrgTstCtl->BrgTestReq == false ) { PUSH_DIAG(DesatFltBL); };
                    }
                    if ( BrgStatFlts.CnegB1 && (strncmp(INS(Uniquifier), "L", 1) != 0 ))
                    {
                        if ( IN(Brg1PhsTransList).pBrgTstCtl->BrgTestReq == false ) { PUSH_DIAG(DesatFltCL); };
                    }
                    if ( BrgStatFlts.AposB1 && (strncmp(INS(Uniquifier), "L", 1) != 0 ))
                    {
                        if ( IN(Brg1PhsTransList).pBrgTstCtl->BrgTestReq == false ) { PUSH_DIAG(DesatFltAU); };
                    }
                    if ( BrgStatFlts.BposB1 && (strncmp(INS(Uniquifier), "L", 1) != 0 ))
                    {
                        if ( IN(Brg1PhsTransList).pBrgTstCtl->BrgTestReq == false ) { PUSH_DIAG(DesatFltBU); }
                    };
                    if ( BrgStatFlts.CposB1 && (strncmp(INS(Uniquifier), "L", 1) != 0 ))
                    {
                        if ( IN(Brg1PhsTransList).pBrgTstCtl->BrgTestReq == false ) { PUSH_DIAG(DesatFltCU); };
                    }
                }

                //-------------------------------------------------------------------------
                // Check bridge status loop shutdown status (Cin109RtrFlt = !Din9 delayed, needs reset to clear)
                // The product code should react to an open Din9 by initiating a Safety chain stop sequence and remove
                // Brg_EnableReq before the hsla fpga sets Cin109RtrFlt.  Therefore only push a fault if the fpga shuts
                // down gating before the product code does.
                // The fpga will force aead K1 to drop out when Cin109RtrFlt is set.
                // The fpga will not allow Cin109RtrFlt to reset if K1 command is picked up.
                if ( BrgFbk.BrStatFlt.Cin109RtrFlt )
                {
                    BrgFaultResolved  = true;
                    if (IN(Brg1EnableReq)) PUSH_DIAG(RdytrLoopShtdwnBr);
                }
            }

            //-------------------------------------------------------------------------
            // check bridge ready loop fiber optic input
            if (BrgFbk.StatCom.BrlRxFlt)
            {
                BrgFaultResolved = true;
                if (IN(RtrLoopFltEnb)) PUSH_DIAG(RdytrLoopShtdwn);
            }
        }


        //-------------------------------------------------------------------------
        // check for target synchronization problems
        if ( (*(unsigned*)&BrgFbk.StatCom & STATCOM_TARG_FRC_FLT_MASK) != 0x0 )
        {
            BrgFaultResolved = true;
            if ( BrgFbk.StatCom.FrcFlt)     {   PUSH_DIAG(TargFrcErrFlt);   }
            if ( BrgFbk.StatCom.FrmErr)     {   PUSH_DIAG(TargFrameErrFlt); }
#if 0   // THIS IS NOW AN ALARM, ALSO BELIEVE THERE IS A BUG IN THE FPGA CODE THAT MAKES THIS SIGNAL BEHAVE BADLY (JMN)
            if ( BrgFbk.StatCom.LdPlsAlm)   {   PUSH_DIAG(TargLdPlsAlm);    }
#endif
        }

        //-------------------------------------------------------------------------
        // check to see if unable to resolve the general bridge fault indicaton from the FPGA into a specific fault
        if ( !BrgFaultActZ && !BrgFaultResolved )
        {
            PUSH_DIAG(UnresolvedBrgFlt);
        }
        BrgFaultActZ = true;
    }

    //-------------------------------------------------------------------------
    // check gate drive power supply problems
    if ( (BrgFbk.StatCom.DrvP5 != 0x3 ) && (!(BrgFbk.BrStatFlt.Rly101RtrFlt)) && !BrgFbk.MiscStat.CfgEnabled && !RebhSet)
    {
        if ( !(BrgFbk.StatCom.DrvP5 & 0x1) )   {   PUSH_DIAG(GdPwrSup3Fail);   }
    }

    //-------------------------------------------------------------------------
    // check for Rebf faults
    if ( !(BrgFbk.StatReg21.REBxpwrOk) )
    {
        PUSH_DIAG(RebfCtlPwrFlt);
    }

    //-------------------------------------------------------------------------
    // check IO 1 faults

    if ( INS(Io1InfcHdw) != IO_INFC_NONE )
    {
        CtlPwrOk =  (int)BrgFbk.StatReg21.CtlPwrOk1; //Check is in task 2 for both of these
        RlyPwrOk =  (int)BrgFbk.StatReg21.RlyPwrOk;
    }


    //-------------------------------------------------------------------------
    // check for Hsla supply problem
    if (((!BrgFbk.StatReg21.HslaPwrOk) || (!BrgFbk.MiscStat.InPwrOk)) && !INS(SimMode))
    {
        PUSH_DIAG(HslaPwrFlt);
    }

    //-------------------------------------------------------------------------
    // check for Hsla clock problem
    if (BrgFbk.StatReg21.ClkBad)
    {
        PUSH_DIAG(HslaClkFlt);
    }

    //-------------------------------------------------------------------------
    // check for Hsla supply problem
    if ((!BrgFbk.StatReg21.P19Ok) && !INS(SimMode) && RebhSet)
    {
        PUSH_DIAG(P5OkFlt);
    }

    //-------------------------------------------------------------------------
    // Missing SDL
    if ((BrgFbk.BrStatFlt.SDLerr) && !INS(SimMode) && RebhSet)
    {
        if ( !BrgFbk.SDL21_ST.SDL_Lnk_OK)
        {
            PUSH_DIAG(Sdl1BrgErr);
        }
        else if ( !BrgFbk.SDL21_ST.SDL_Lnk_OK2)
        {
            PUSH_DIAG(Sdl2BrgErr);

        }
        else if ( !BrgFbk.SDL43_ST.SDL_Lnk_OK)
        {
            PUSH_DIAG(Sdl3BrgErr);
        }
        else if ( !BrgFbk.SDL43_ST.SDL_Lnk_OK2)
        {
            PUSH_DIAG(Sdl4BrgErr);

        }
    }

    //-------------------------------------------------------------------------
    // periodically issue an fpga fault latch reset to allow the diagnostic to go inactive after the fault condition subsides
    // there are 2 independent fault latches that are all reset by BrgFltClear
    //      BrgFlt, BrgStatusLoopShtdwn
    //  only issue the fpga bridge fault reset if one of these fault latches is set
    //  and the bridge is not enabled and bridge is not being reqeusted to enable
    //-------------------------------------------------------------------------
    // Write out the Reset command at task 1
    BrgCmd.Cr21.BrgFltClear = IN(ClearFltsT1);
    //BrgCmd.Cr21.BrgFltClear = false;
    if ( (FpgaFltLatch || PhsDdsatFltLatch) && !Brg1Enabled && !IN(Brg1EnableReq))
    {
        if ( ++BrgFltResetDly > BRIDGE_FAULT_RESET_DLY )
        {
            BrgCmd.Cr21.BrgFltClear = true;  // fpga latches reset on rising edge of BrgFltClear
            BrgFltResetDly = 0;              // re-start the fpga bridge fault reset delay
        }
    }
    else
    {
        BrgFltResetDly = 0;                             // hold the fpga bridge fault reset delay at zero
    }

#if REBx_DIAG_FPGA_REGISTERS
    //-------------------------------------------------------------------------
    // update the published registers for diagnostics
    StatComReg      = *(unsigned*)&BrgFbk.StatCom;
    BrStatFltReg    = *(unsigned*)&BrgFbk .BrStatFlt;
    StatReg21Reg    = *(unsigned*)&BrgFbk.StatReg21;
    FbkReserveReg   = *(unsigned*)&BrgFbk.FbkReserve;
    MiscStatReg     = *(unsigned*)&BrgFbk.MiscStat;
    PktCntRbReg     = *(unsigned*)&BrgFbk.PktCntRb;
    RevCodeReg      = *(unsigned*)&BrgFbk.RevCode;
    Cr21Reg         = *(unsigned*)&BrgCmd.Cr21;
    CrCfgReg        = *(unsigned*)&BrgCmd.CrCfg;
#endif

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Wait for HSSL fedback packet and then process the data.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::Task1Fbk(void)
{

    //-------------------------------------------------------------------------
    // no work to do if the link is disabled
    if ( !HsslIsEnabled )
    {
        return;
    }

    //-------------------------------------------------------------------------
    // wait for fresh hssl feedback packet to arrive
    HsslFbkWait = 0.0F;
    LdPls2EvtFlg = false;
    OUT(VcoBr1VabRaw) = BrgFbk.VcoBr1.Vab;
    if ( HsslErrChksEnb )
    {
        LdPls2EvtFlg0 = (volatile unsigned)BrgFbk.StatCom.LdPls2Evt;
        // check to see if the new feedback has arrived
        if ( BrgFbk.PktCntRb.Cnt != CmdPktCnt )
        {
            unsigned HsslFbkRetryCtr = 0;
            HsslFbkWaitStrt = ClockCycles();
            while ( true )
            {
                // if simulation mode and bridge is enabled then use feedbacks from real hssl instead of isim
                if ( INS(SimMode) && SimModeBrgEnabled )
                {
                    BrgFbk.PktCntRb.Cnt = BrgFbkAlt.PktCntRb.Cnt;
                }

                // check to see if the new feedback has arrived
                if ( (volatile unsigned)BrgFbk.PktCntRb.Cnt == CmdPktCnt )  // cast BrgFbk as volatile to force re-read
                {
                    break;  // while (true)
                }

                // keep track of how long we have waited for new feedback to arrive
                HsslFbkWait = (float)((int)((unsigned)(ClockCycles() - HsslFbkWaitStrt))) * 1.0e6F / INS(CpuCps);
                if ( (volatile unsigned)BrgFbk.StatCom.LdPls2Evt )
                {
                    if (!LdPls2EvtFlg)
                    {
                        LdPls2EvtFlg = true;
                        HsslFbkWaitLdPls2 = HsslFbkWait;
                    }
                    // delay to allow time for dma of whole feedback packet
                    if (HsslFbkWait - HsslFbkWaitLdPls2 > 1.0F)
                    {
                        break;  // while (true)
                    }
                }
                if ( HsslFbkWait > 25.0F )  // 25us (dont wait too long or will cause task over-run problems)
                {
                    // try again just in case exceeded limit due to higher priority interruption
                    if ( HsslFbkRetryCtr < 2 )
                    {
                        ++HsslFbkRetryCtr;
                        continue;   // while (true)
                    }
                    // waited long enough
                    HsslErrFlags |= HOST_RX_FBK_NOT_ON_TIME;
                    OUT(HsslCommFlt) = true;
                    break;  // while (true)
                }
            }
            // keep track of min/max wait times
            if ( HsslFbkWaitMin > HsslFbkWait )  HsslFbkWaitMin = HsslFbkWait;
            if ( HsslFbkWaitMax < HsslFbkWait )  HsslFbkWaitMax = HsslFbkWait;
        }
        ( (HsslErrFlags & HOST_RX_FBK_NOT_ON_TIME) != 0 ) ? ++HsslFbkLateCtr : HsslFbkLateCtr = 0;
        if (LdPls2EvtFlg0 || LdPls2EvtFlg)
        {
            ++LdPls2EvtCtrTot;               // track total # of events

            if (PARM(HsslLdPls2AlmEnb))      // if enabled,
            {
                PUSH_DIAG(HsslLdPls2Alm);    //   alarm if fpga sent contingency fdbk packet
            }

            ++LdPls2EvtCtr;                  // track consecutive events
            if (LdPls2EvtCtr > 1)
            {
                PUSH_DIAG(HsslLdPls2Trp);    // trip if more than one consecutive events
            }

            OUT(CntgncyPktRcvd) = true;      // indicate contingency packet was received
        }
        else
        {
            LdPls2EvtCtr = 0;
            OUT(CntgncyPktRcvd) = false;
        }
    }

    //-------------------------------------------------------------------------
    // if simulation mode and bridge is enabled then use feedbacks from real hssl instead of isim
    //  (in sim mode, BrgFbkAlt holds the real feedback signals from the target hardware)
    if ( INS(SimMode) && SimModeBrgEnabled )
    {
#if 1   // compile time switch to force using simulated feedback instead of target feedback when in sim mode with active hssl
        //  this is useful if you have hsla/rebf but no brg or i/o interface cards
        //  use #if 1 when you have hsla/rebf with bridge and io cards (forces signals to come from real hssl target hardware) (NORMAL)
        //  use #if 0 when you have hsla/rebf with NO bridge or io cards (forces signals to come from the simulator)
        BrgFbk.StatCom   = BrgFbkAlt.StatCom;
        if ( !INS(DSimMode) )
        {
            BrgFbk.BrStatFlt = BrgFbkAlt.BrStatFlt;
        }
        BrgFbk.StatReg21 = BrgFbkAlt.StatReg21;
        BrgFbk.MiscStat  = BrgFbkAlt.MiscStat;
#endif
        BrgFbk.RevCode   = BrgFbkAlt.RevCode;
        BrgFbk.CrLb      = BrgFbkAlt.CrLb;
        BrgFbk.VcoTemp21 = BrgFbkAlt.VcoTemp21;
    }
    //-------------------------------------------------------------------------
    // always update the status info
    OUT(Brg1Enabled) = BrgFbk.StatCom.PeEnb;
    BrgCfgEnabled    = BrgFbk.MiscStat.CfgEnabled;

    //-------------------------------------------------------------------------
    // check for problems that should cause trip after link has advanced from the discover state
    if ( HsslErrChksEnb )
    {
        // target did not enable gating for bridge 1 as commanded
        //  only check if there is no fault latched in the fpga
        if ( !BrgFbk.StatCom.DriveFlt )
        {
            if ( (Brg1EnabCtr == 3) && !Brg1Enabled )
            {
                PUSH_DIAG(HsslBrgEnbFailTrp);
            }
        }
    }

    //-------------------------------------------------------------------------
    // check for problems that should cause trip after link has advanced from the init state
    if ( TargInitDone )
    {
        // target should never enable configuration changes once target initializtion is complete
        if ( BrgCfgEnabled )
        {
            PUSH_DIAG(HsslInvCfgEnbTrp);
        }
        if ( !IN(Brg1EnableReq) && Brg1Enabled  )
        {
           OUT(DisCmdNotFlg) = true;
           PUSH_DIAG(DisCmdNotFuncTrp);
        }
        else
        {
           OUT(DisCmdNotFlg) = false;
        }
    }

    //-------------------------------------------------------------------------
    // check for hssl errors
    if ( HsslErrFlags != 0 )
    {
        HsslErrFlagsAcc |= HsslErrFlags;    // update the error flag accumulator
        HsslErrCnt++;
        PUSH_DIAG(HsslCommErr);
    }

    //-------------------------------------------------------------------------
    // always call state machine that initializes the target
    InitializeTarget();

    //-------------------------------------------------------------------------
    // always call feedback calculation
    CheckBridgeStatus();
    FeedbackCalc();

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::Task1Cmd(void)
{

    //-------------------------------------------------------------------------
    // no work to do if the link is disabled
    if ( !HsslIsEnabled )
    {
        return;
    }

    //-------------------------------------------------------------------------
    // calculate and populate the command window
    CommandCalc();

    //-------------------------------------------------------------------------
    // if target discovery is done
    if ( TargDscDone )
    {
        unsigned HsslTstIntDsb = false;

        if ( PARM(HsslTstIntvl) == 0 )                             // rule value of 0 disables test
        {
            HsslTstSgn = 0.0F;
        }
        else if (HsslTstSgn == 0.0F)                               // initialize test
        {
            HsslTstMrg  = PARM(HsslTstMrgMax);
            HsslTstSgn  = 1.0F;
            HsslTstCntr = 0;
        }
        else
        {
            HsslTstCntr++;                                         // HsslTstCntr = 0...N, 0...N
            if (HsslTstCntr >= PARM(HsslTstIntvl))
            {
                HsslTstCntr = 0;

                if (    PARM(HsslTstMrgMin) > INS(DelTm1) * (-0.5e6F)
                     && PARM(HsslTstMrgMax) < INS(DelTm1) * ( 1.0e6F)
                     && PARM(HsslTstMrgMax) >= PARM(HsslTstMrgMin)
                   )
                {
                    HsslTstMrg += .01F * HsslTstSgn;               // HsslTstMrg = margin in usec (10 nsec inc/dec)
                                                                   // HsslTstSgn = sign of increment
                    if (HsslTstMrg > PARM(HsslTstMrgMax))
                    {
                        HsslTstMrg = PARM(HsslTstMrgMax);          // max margin reached
                        HsslTstSgn = -1.0F;                        // start decrementing margin
                    }
                    if (HsslTstMrg < PARM(HsslTstMrgMin))
                    {
                        HsslTstMrg = PARM(HsslTstMrgMin);          // min margin reached
                        HsslTstSgn = 1.0F;                         // start incrementing margin
                    }

                    while ( ((int)((unsigned)(IN(T1FrameTm) - ClockCycles()))) * INS(CpuCpsRcpUsec) > (HsslTstMrg + 20.0F) )
                    {
                        // spin here until margin gets to within 20 usec of specified value
                    }

                    InterruptDisable();                            // prevent interrupts from disrupting timing
                    HsslTstIntDsb = true;                          // flag to indicate interrupts disabled for test

                    while ( ((int)((unsigned)(IN(T1FrameTm) - ClockCycles()))) * INS(CpuCpsRcpUsec) > HsslTstMrg )
                    {
                        // spin here until margin reaches specified value
                    }
                }
            }
        }
        // demand the CMD window to be transmitted immediately
        Hssl.DemandWinTransfer(REBF_WIN_CMD, ErrCode);
        // margin between now and next frame boundary (usec)
        OUT(HsslCmdMrg) = (float)(((int)((unsigned)(IN(T1FrameTm) - ClockCycles())))) * INS(CpuCpsRcpUsec);
        // Re-enable interrupts if disabled above for hssl testing
        if (HsslTstIntDsb)
        {
            InterruptEnable();
        }

        // worst case margin (may be negative if overrun) since pwr-up
        if (HsslCmdMrg < HsslCmdMrgMin)
        {
            OUT(HsslCmdMrgMin) = HsslCmdMrg;
        }
    }

    //-------------------------------------------------------------------------
    // capture data for the low level monitor
    if ( LlmCmdShdwCapReq )
    {
        LlmCmdShdw = BrgCmd;
        LlmCmdShdwCapReq = false;
    }
    if ( LlmFbkShdwCapReq )
    {
        LlmFbkShdw = BrgFbk;
        LlmFbkShdwCapReq = false;
    }

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculate feedback offsets.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::Task2(void)
{
    //-------------------------------------------------------------------------
    // no work to do if the link is disabled
    if ( !HsslIsEnabled )
    {
        return;
    }

    // calculation and monitoring of variables in support of
    // feedback offset calculation
    IN(Brg1FbkOffs).Task2();
    IN(Io1FbkOffs).Task2();

    if ( (INS(Io1InfcHdw) != IO_INFC_NONE) && IN(InitDone))
    {
        CtlPwrOkFil  += CtlPwrOkFilWdt * ( CtlPwrOkT2 - CtlPwrOkFil);
        RlyPwrOkFil  += RlyPwrOkFilWdt * ( RlyPwrOkT2 - RlyPwrOkFil);

        if ( CtlPwrOkFil < PARM(CtlRlyPwrOkThrs))
        {
            PUSH_DIAG(IoCtlPwrFlt);
        }
        if ( (RlyPwrOkFil < PARM(CtlRlyPwrOkThrs))  && !PARM(Io1RlyPwrChkInh))
        {
            PUSH_DIAG(IoRlyPwrFlt);
        }
    }
    else
    {

       CtlPwrOkFil = 1.0F;
       RlyPwrOkFil = 1.0F;
    }




    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Fault resets logic.
//      Check for link problems.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::Task3(void)
{
   const int     ErrCode = 0;
    bool    HsslLinkOk;

    //-------------------------------------------------------------------------
    // populate rebf analog-out for display purposes
    BrgFan1SpdRef = BrgCmd.FanRef.FanRef1;
    BrgFan2SpdRef = BrgCmd.FanRef.FanRef2;

    //-------------------------------------------------------------------------
    // no work to do if the link is disabled
    if ( !HsslIsEnabled )
    {
        return;
    }

    //-------------------------------------------------------------------------
    // calculation and monitoring of variables in support of
    // feedback offset calculation
    IN(Brg1FbkOffs).Task3();
    IN(Io1FbkOffs).Task3();

    //-------------------------------------------------------------------------
    // re-discover the link for certain serious problems
    if ( ((HsslErrFlagsAcc & (HOST_LINK_LOST | HOST_NOT_RUNNING_STATE | TARG_NOT_RUNNING_STATE) ) != 0) )
    {
        TargDscReq = true;
        ClearHwInventoryReq = true;     // link is lost so clear the inventory
        AeadGnCalcDone = false;
        AeaaGnCalcDone = false;
    }

    //-------------------------------------------------------------------------
    // one-time calculation of pwa gains when Dallas-Id data received
    if (AeadGnCalcEnb)
    {
        AeadGnCalc();
        AeadGnDefault();
        AeadGnCalcDone = true;
        AeadGnCalcEnb = false;
    }

    if (AeaaGnCalcEnb)
    {
        AeaaGnCalc();
        AeaaGnCalcDone = true;
        AeaaGnCalcEnb = false;
    }

    //-------------------------------------------------------------------------
    // Clear comm fault indicator if set and comm is now ok
    if (HsslCommFlt)
    {
        if ( ((HsslErrFlags & (HOST_LINK_LOST | HOST_RX_FBK_NOT_ON_TIME) ) == 0) )
        {
            HsslCommFltRstTmr += INS(DelTm3);
            if (HsslCommFltRstTmr > 1.0F)
            {
                OUT(HsslCommFlt) = false;
                HsslCommFltRstTmr = 0.0F;
            }
        }
    }



    //-------------------------------------------------------------------------
    // re-discover the link if feedback persistently late
    if ( ((HsslErrFlagsAcc & HOST_RX_FBK_NOT_ON_TIME) != 0) && (HsslFbkLateCtr > 100) )
    {
        TargDscReq = true;
    }

    //-------------------------------------------------------------------------
    // reset task3 error flags, alarm flags, diagnostic accumulators, etc.
    if ( IN(ClearFltsT3) )
    {
        HsslAlmFlags    = 0;
        HsslAlmFlagsAcc = 0;

        HsslErrFlagsDsc = 0;
        HsslErrFlagsIni = 0;

        HsslErrFlagsAcc = 0;
        HsslErrCnt      = 0;

        TaskBkgndRst = true;

        HsslFbkWaitMin  = 1000.0F;
        HsslFbkWaitMax  = 0.0F;
    }

    //-------------------------------------------------------------------------
    // reset the Task3 error flags and pick up the init alarm flags generated in Background
    HsslErrFlagsT3 = 0;

    //-------------------------------------------------------------------------
    // only perform link monitoring checks if the link host side is open
    if ( HostIsOpen )
    {
        //---------------------------------------------------------------------
        // determine the sate of the host and target hardware
        if ( TargetHdwExpected )
        {
            HsslLinkOk = Hssl.IsLinkOk();
            Hssl.GetCurrentHostState( HsslHostState, ErrCode );
            Hssl.GetCurrentTargetState( HsslTargState, ErrCode );
        }
        else
        {
            HsslLinkOk    = true;
            HsslHostState = Hssl_HOST_STATE_RUNNING;
            HsslTargState = Hssl_TARGET_STATE_RUNNING;
        }

        //---------------------------------------------------------------------
        // check to see if the physical link has been lost
        //  skip when link is being reset
        if ( !HsslLinkOk && !Hssl.ResetLinkIsActive() )
        {
            HsslErrFlagsT3 |= HOST_LINK_LOST;
            OUT(HsslCommFlt) = true;
        }

        //---------------------------------------------------------------------
        // check for problems that should cause trip after link has advance from the discover state
        if ( TargDscDone )
        {
            // check operational state of the host
            if ( HsslHostState != Hssl_HOST_STATE_RUNNING )
            {
                HsslErrFlagsT3 |= HOST_NOT_RUNNING_STATE;
            }

            // check operational state of the target
            if ( HsslTargState != Hssl_TARGET_STATE_RUNNING )
            {
                HsslErrFlagsT3 |= TARG_NOT_RUNNING_STATE;
            }

            // check if target has been receiving packets with bad CRCs
            if ( TargRxErrCnt != BrgFbk.MiscStat.RxErr )
            {
                HsslAlmFlags |= TARG_RX_BAD_CRC;
            }

            // check if target has been receiving NAKs on the packets it transmits (feedbacks)
            if ( TargTxErrCnt != BrgFbk.MiscStat.TxErr )
            {
                HsslAlmFlags |= TARG_NAKS_ON_TX;
            }

            // check if target has been receiving packets with bad headers
            if ( TargPktIgnoreCnt != BrgFbk.MiscStat.PktIgnErr )
            {
                HsslAlmFlags |= TARG_RX_BAD_HDR;
            }
        }

        //---------------------------------------------------------------------
        // save the error counts for use on the next pass
        TargRxErrCnt     = BrgFbk.MiscStat.RxErr;
        TargTxErrCnt     = BrgFbk.MiscStat.TxErr;
        TargPktIgnoreCnt = BrgFbk.MiscStat.PktIgnErr;

        //---------------------------------------------------------------------
        // push HSSL communication alarms
        //  update the alarm flag accumulator
        if ( HsslAlmFlags != 0 )
        {
            HsslAlmFlagsAcc |= HsslAlmFlags;
            PUSH_DIAG(HsslCommAlm);
        }
    }

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Parse diagnostic data.
//      Target discovery state machine.
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::TaskBkgnd(void)
{
    unsigned    TargFpgaVerMatch = false;
    int         ErrCode = 0;                // error code local to this function

    //-------------------------------------------------------------------------
    // pick up the reset command from Task3
    bool TaskBkgndRstLcl = TaskBkgndRst;
    TaskBkgndRst = false;

    //-------------------------------------------------------------------------
    // no work to do if the link is disabled or host resource is not open
    if ( !HsslIsEnabled || !HostIsOpen )
    {
        return;
    }

    //-------------------------------------------------------------------------
    // force a return to the abort case if discovery request and not already in an abort or armed state
    //  this makes sure all housekeeping is done regardless of the state of this state machine
    if ( TargDscReq && (TargDscCase != 99) && (TargDscCase != -1) )
    {
        TargDscCase = 99;
        TargDscPass = 0;
    }

    //-------------------------------------------------------------------------
    // increment the target discovery state machine pass flag
    //  if first pass in this state record the time for use in implementing delays
    if ( ++TargDscPass == 1 )
    {
        HsslTimer = IN(SysTm3);
    }

    int IdCounts = Hssl.LinkIdInfo.GetNumIds();
    cLinkIdInfo::IdRecordTyp* IdRecord;
	IdRecord = new (std::nothrow) cLinkIdInfo::IdRecordTyp[IdCounts];

    if ( Hssl.LinkIdInfo.GetPwaIds( IdRecord, (int)ARRAY_LEN(IdRecord) ) != SUCCESS )
    {
        return;
    }


    //-------------------------------------------------------------------------
    // target discovery state machine
    switch ( TargDscCase )
    {

        //---------------------------------------------------------------------
        // discovery is armed
        case -1:

            // if this is a re-discovery request
            if ( TargDscReq == true )
            {
                if ( ClearHwInventoryReq == true )
                {
                    // clear the pwa ID information into the HW inventory
                    Hssl.LinkIdInfo.ClearHwInventory();
                    ClearHwInventoryReq = false;
                }
                // wait for a reset to trigger the re-discovery
                if ( !TaskBkgndRstLcl )
                {
                    break;
                }
                // clear the re-discovery request
                TargDscReq = false;
            }

            // advance to the next state
            TargDscCase++;
            TargDscPass = 0;
            break;

        //-----------------------------------------------------------------
        // reset the link
        case 0:

            // skip discovery if target hardware is not expected
            if ( !TargetHdwExpected )
            {
                TargDscCase = 100;
                TargDscPass = 0;
                break;
            }

            // reset the link



            Hssl.ResetLink( ErrCode );

            // wait for the correct state
            if ( (HsslHostState != Hssl_HOST_STATE_DISCOVER) ||
                    ((HsslTargState != Hssl_TARGET_STATE_IDLE) && (HsslTargState != Hssl_TARGET_STATE_SYNCWAIT)) )
            {
                // delay to allow reception of a babble message
                if ( DELTA_TM(IN(SysTm3), HsslTimer) < 1.000 )    // seconds
                {
                    break;
                }

                // abort discovery if required state not entered within expected time
                HsslErrFlagsDsc |= TARG_DSC_INITIAL_STATE_FAIL;
                PUSH_DIAG(HsslDscFail);
                TargDscCase = 99;
                TargDscPass = 0;
                break;
            }

            // correct state found so advance to the next state
            TargDscCase++;
            TargDscPass = 0;
            break;

        //-----------------------------------------------------------------
        // read link target id information
        case 1:

            // on first pass, enable the transmission of the id information
            if ( TargDscPass == 1 )
            {
                Hssl.EnableLinkIdTx( ErrCode );
                break;
            }

            // allow some time for a babble message to be sent
            if ( DELTA_TM(IN(SysTm3), HsslTimer) < 1.000 )    // seconds
            {
                break;
            }

            // disable the transmission of the id information
            Hssl.DisableLinkIdTx( ErrCode );

            // read and decode the babble data
            if ( Hssl.LinkIdInfo.DecodeBab( ErrCode ) != SUCCESS )
            {
                // abort discovery if unable to decode the babble
                HsslErrFlagsDsc |= TARG_DSC_TARG_ID_READ_FAIL;
                PUSH_DIAG(HsslDscFail);
                TargDscCase = 99;
                TargDscPass = 0;
                break;
            }

            // advance to the next state
            TargDscCase++;
            TargDscPass = 0;
            break;

        //-----------------------------------------------------------------
        // check the link target hardware id information
        case 2:

            // scan the id records for required bus 0 cards (REBH)
            for ( int idx = 0 ; idx < IdCounts ; ++idx )
            {
                // only look at catalog numbers on bus #0 (babble)
                if ( IdRecord[idx].BusNo != 0 )
                {
                    continue;   // go to the next record in the LinkIdInfo list
                }

                // look for REBH card
                if ( !strcmp(IdRecord[idx].pCardBaseName, "REBH") )
                {
                    RebhSet = true;
                }
            }

            if ( RebhSet)
            {

                strcpy(HslaApplFile, HslApplFile);
                strcpy(HslaApplVer,  HslApplVer);
            }
            else
            {
                strcpy(HslaApplFile, HslaH1ApplFile);
                strcpy(HslaApplVer,  HslaH1ApplVer);
            }

            if ( !RebhSet)
            {
                if ( ValidatePriPwaList() != SUCCESS)
                {
                    // abort discovery if required cards are not found
                    HsslErrFlagsDsc |= TARG_DSC_TARG_HW_VALIDATE_FAIL;
                }
            }
            else
            {
                if ( ValdteRebhPrPwaLst() != SUCCESS)
                {
                    // abort discovery if required cards are not found
                    HsslErrFlagsDsc |= TARG_DSC_TARG_HW_VALIDATE_FAIL;
                }
            }

            // set pwa ID information into the HW inventory
            Hssl.LinkIdInfo.SetHwInventory();

            // advance to the next state
            if ( HsslErrFlagsDsc != 0 )
            {
                // expect that specific diagnostic has been pushed by the ValidatePriPwaList() or ValdteRebhPrPwaLst functions
                TargDscCase = 99;
                TargDscPass = 0;
            }
            else
            {
                TargDscCase++;
                TargDscPass = 0;
            }
            break;

        //-----------------------------------------------------------------
        // check the link target application fpga version, reprogram as necessary
        case 3:

            // extract the application version from mpAppId
            Hssl.LinkIdInfo.GetAppId( TargFpgaVer );
            if ( !RebhSet)
            {
               TargFpgaVer = TargFpgaVer.substr( TargFpgaVer.length() - 6, TargFpgaVer.length() );     // last 6 chars for REBF
            }
            else
            {
               TargFpgaVer = TargFpgaVer.substr( TargFpgaVer.length() - 7, TargFpgaVer.length() );     // last 7 chars for REBH
            }

            // check to see if the version of the target application fpga matches the expected value
            if ( strcmp(TargFpgaVer.c_str(), HslaApplVer) == 0 )
            {
                TargFpgaVerMatch = true;

            }
            // printf("checking for fpga version match, match= %d\n",TargFpgaVerMatch);
            // printf("target version = %s  file version= %s\n",TargFpgaVer.c_str(), HslaApplVer);

            // reprogram if version mismatch or parameter is set to force reprogramming
            if ( (TargFpgaVerMatch && !PARM(TargFpgaReprog)) ||
                 (TargFpgaVerMatch &&  PARM(TargFpgaReprog) && (TargReprogCtr > 0)) )
            {
                // version is correct so no reprogramming required
                if ( TargReprogCtr > 0 )
                {
                    printf("Hssl device %d channel %d target fpga reprogram successful: version %s\n", LinkInfo.DeviceId, LinkInfo.ChannelId, HslaApplVer);
                }
                TargReprogCtr = 0;
            }
            else
            {
                // check to see if reprogramming has already been attempted
                if ( TargReprogCtr > 0 )
                {
                    // already attempted reprogramming and version is still wrong
                    printf("Hssl device %d channel %d target fpga version wrong after %d attempts to reprogram\n", LinkInfo.DeviceId, LinkInfo.ChannelId, TargReprogCtr);
                    printf("target version = %s, requried version = %s\n", TargFpgaVer.c_str(), HslaApplVer);
                    TargReprogCtr = 0;       // reset the reprogram counter
                    HsslErrFlagsDsc |= TARG_DSC_TARG_FPGA_WRONG_VER;
                    PUSH_DIAG(HsslDscFail);
                    TargDscCase = 99;
                    TargDscPass = 0;
                    break;
                }

                // reprogram the target fpga
                printf("Hssl device %d channel %d target fpga reprogramming ...\n", LinkInfo.DeviceId, LinkInfo.ChannelId);
                printf("target version = %s, requried version = %s\n", TargFpgaVer.c_str(), HslaApplVer);
                if ( Hssl.LoadTargetFpga( (char*)HslaApplFile, ErrCode ) != SUCCESS )
                {
                    // error
                    printf("Hssl device %d channel %d target fpga programming error (%d)\n", LinkInfo.DeviceId, LinkInfo.ChannelId, ErrCode);
                    HsslErrFlagsDsc |= TARG_DSC_TARG_FPGA_PROG_FAIL;
                    PUSH_DIAG(HsslDscFail);
                    TargDscCase = 99;
                    TargDscPass = 0;
                    break;
                }

                // no error so rediscover the link
                if (gVerbosityLevel > 1)
                {
                    printf("Hssl device %d channel %d target fpga programming complete, re-discover the link\n", LinkInfo.DeviceId, LinkInfo.ChannelId);
                }
                ++TargReprogCtr;        // keep track of reprogramming attempts
                TargDscCase = 0;
                TargDscPass = 0;
                break;
            }

            // advance to the next state
            TargDscCase++;
            TargDscPass = 0;
            break;


        //-----------------------------------------------------------------
        // configure the target
        case 4:

            // on first pass, advance the host state to config
            if ( TargDscPass == 1 )
            {
                Hssl.SetCurrentHostState( Hssl_HOST_STATE_CONFIG, ErrCode );
                break;
            }

            // check the host and target states
            // (check state before time to cover the case where exec of this test was delayed by re-programming on another link)
            if ( (HsslHostState != Hssl_HOST_STATE_CONFIG) || (HsslTargState != Hssl_TARGET_STATE_SYNCWAIT) )
            {
                // allow some time for the states to advance
                if ( DELTA_TM(IN(SysTm3), HsslTimer) < 3.000 )    // seconds
                {
                    break;
                }

                // abort discovery if required state not entered within expected time
                printf("Hssl device %d channel %d did not advance to config state (host=%d, targ=%d)\n", LinkInfo.DeviceId, LinkInfo.ChannelId, HsslHostState, HsslTargState);
                HsslErrFlagsDsc |= TARG_DSC_TARG_ADV_TO_CFG_FAIL;
                PUSH_DIAG(HsslDscFail);
                TargDscCase = 99;
                TargDscPass = 0;
                break;
            }

            // configure the windows
            for ( unsigned WinIdx = 0 ; WinIdx < ARRAY_LEN(WinInfo) ; ++WinIdx )
            {
                if ( Hssl.ConfigureWindow( WinInfo[WinIdx], ErrCode ) != SUCCESS )
                {
                    printf("Hssl device %d channel %d window %d configuration failure (%d)\n", LinkInfo.DeviceId, LinkInfo.ChannelId, WinIdx, ErrCode );
                    HsslErrFlagsDsc |= TARG_DSC_HOST_WIN_INI_FAIL;
                }
            }

            // abort discovery if one of the window configurations failed
            if ( (HsslErrFlagsDsc & TARG_DSC_HOST_WIN_INI_FAIL) != 0 )
            {
                PUSH_DIAG(HsslDscFail);
                TargDscCase = 99;
                TargDscPass = 0;
                break;
            }

            // advance to the next state
            TargDscCase++;
            TargDscPass = 0;
            break;

        //-----------------------------------------------------------------
        // synchronize the target to the host
        case 5:

            // on first pass, advance the host state to sync
            if ( TargDscPass == 1 )
            {
                Hssl.SetCurrentHostState( Hssl_HOST_STATE_SYNC, ErrCode );
                break;
            }

            // check the host and target states
            // (check state before time to cover the case where exec of this test was delayed by re-programming on another link)
            if ( (HsslHostState != Hssl_HOST_STATE_SYNC) || (HsslTargState != Hssl_TARGET_STATE_SYNCD) )
            {
                // allow some time for the states to advance
                if ( DELTA_TM(IN(SysTm3), HsslTimer) < 1.000 )    // seconds
                {
                    break;
                }

                // abort discovery if required state not entered within expected time
                printf("Hssl device %d channel %d did not advance to sync state (host=0x%02x, targ=0x%02x)\n", LinkInfo.DeviceId, LinkInfo.ChannelId, HsslHostState, HsslTargState);
                HsslErrFlagsDsc |= TARG_DSC_TARG_FRC_SYNC_FAIL;
                PUSH_DIAG(HsslDscFail);
                TargDscCase = 99;
                TargDscPass = 0;
                break;
            }

            // advance to the next state
            TargDscCase++;
            TargDscPass = 0;
            break;

        //-----------------------------------------------------------------
        // advance the link to the running state
        case 6:

            // on first pass, advance the host state to running
            if ( TargDscPass == 1 )
            {
                Hssl.SetCurrentHostState( Hssl_HOST_STATE_RUNNING, ErrCode );
                break;
            }

            // check the host and target states
            // (check state before time to cover the case where exec of this test was delayed by re-programming on another link)
            if ( (HsslHostState != Hssl_HOST_STATE_RUNNING) || (HsslTargState != Hssl_TARGET_STATE_RUNNING) )
            {
                // allow some time for the states to advance
                if ( DELTA_TM(IN(SysTm3), HsslTimer) < 1.000 )    // seconds
                {
                    break;
                }

                // abort discovery if required state not entered within expected time
                printf("Hssl device %d channel %d did not advance to running state\n", LinkInfo.DeviceId, LinkInfo.ChannelId);
                HsslErrFlagsDsc |= TARG_DSC_LINK_ADV_TO_RUN_FAIL;
                PUSH_DIAG(HsslDscFail);
                TargDscCase = 99;
                TargDscPass = 0;
                break;
            }

            // advance to the discovery done state
            TargDscCase = 100;
            TargDscPass = 0;
            break;

        //-----------------------------------------------------------------
        // failure case
        case 99:

            // perform any needed housekeeping
            if ( TargDscPass == 1 )
            {
                // request re-discovery of the link
                TargDscReq = true;
            }
            else
            {
                // re-arm the target discovery sequence
                TargDscCase = -1;
                TargDscPass = 0;
            }
            break;

        //-----------------------------------------------------------------
        // discovery done
        case 100:

            TargDscPass = 0;
            break;

        //-----------------------------------------------------------------
        // default case ... should never get here
        default:

            HsslErrFlagsDsc |= TARG_DSC_INV_STATE;
            PUSH_DIAG(HsslDscFail);
            TargDscCase = 99;
            TargDscPass = 0;
            break;

    //-------------------------------------------------------------------------
    // end of switch (HsslDscCase) state machine to achieve discovery sequence
    }

    //-------------------------------------------------------------------------
    // assign done status based on state machine case
    (TargDscCase == 100) ? TargDscDone = true : TargDscDone = false;

    //-------------------------------------------------------------------------
    // done
    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Validate primary pwa list
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      SUCCESS / FAIL
//
/////////////////////////////////////////////////////////////////////////////
int cBridgeRebf::ValidatePriPwaList(void)
{
    int         Status = SUCCESS;

    int         RebfIdx = -1;          // pwa id record array index for REBF (-1 = not found)
    int         HslaIdx = -1;          // pwa id record array index for HSLA (-1 = not found)

    bool        HslaCardValid = false;
    bool        RebfCardValid = false;

    const char* HslaConnStr = NULL_Str;

    //-------------------------------------------------------------------------
    // retrieve the id records for local use
    int IdCnt = Hssl.LinkIdInfo.GetNumIds();
    cLinkIdInfo::IdRecordTyp* IdRecord;
    IdRecord = new (std::nothrow) cLinkIdInfo::IdRecordTyp[IdCnt];
    if (IdRecord == NULL)
        return FAIL;

    if ( Hssl.LinkIdInfo.GetPwaIds( IdRecord, IdCnt ) != SUCCESS )
    {
        delete [] IdRecord;
        return FAIL;
    }

    if      ( (HslaSiteTyp)(INS(HslaSite)) == HSLA_SITE_JA1 )  { HslaSite_JA1 = true; }
    else if ( (HslaSiteTyp)(INS(HslaSite)) == HSLA_SITE_JA2 )  { HslaSite_JA1 = false; }

    //-------------------------------------------------------------------------
    // scan the id records for required bus 0 cards (HSLA and REBF)
    for ( int idx = 0 ; idx < IdCnt ; ++idx )
    {
        // only look at catalog numbers on bus #0 (babble)
        if ( IdRecord[idx].BusNo != 0 )
        {
            continue;   // go to the next record in the LinkIdInfo list
        }

        // look for REBF card
        if ( !strcmp(IdRecord[idx].pCardBaseName, "REBF") )
        {
            Hssl.LinkIdInfo.SetCardType( CARD_ACQUISITION, idx );
            Hssl.LinkIdInfo.SetConnName( NULL_Str, idx );    // connector for REBF is n/a (not applicable)
            Hssl.LinkIdInfo.SetCardIndx( 0, idx );
            RebfIdx = idx;
        }

        // look for HSLA card
        else if ( !strcmp(IdRecord[idx].pCardBaseName, "HSLA") )
        {
            Hssl.LinkIdInfo.SetCardType( CARD_ACQUISITION, idx );
            Hssl.LinkIdInfo.SetConnName( NULL_Str, idx );    // connector for HSLA not known yet (comes from REBF dsr0)
            Hssl.LinkIdInfo.SetCardIndx( 1, idx );
            HslaIdx = idx;
        }
    }

    //-------------------------------------------------------------------------
    // check to see if the REBF card is in the required REBF card list
    //  and push a diagnostic if not

    if ( RebfIdx != -1 )
    {
        // Determine whether REBF is a prototype
        if ( !strncmp(IdRecord[RebfIdx].pCatNo, REBx_CatNum_Str_Pr, strlen( REBx_CatNum_Str_Pr )) )
        {
            // REBF is a prototype - check it's an acceptable version
            for ( int rdx = 0 ; rdx < (int)ARRAY_LEN(Reqd_REBx_CatNum_Str_Pr) ; ++rdx )
            {
                if ( !strncmp(IdRecord[RebfIdx].pCatNo, Reqd_REBx_CatNum_Str_Pr[rdx], strlen( Reqd_REBx_CatNum_Str_Pr[rdx] )) )
                {
                    RebfCardValid = true;
                    break;      // stop walking the Reqd_REBF_CatNum_Str_PrReq list
                }
            }
        }
        else
        {
            // REBF is not a prototype - check it's an acceptable version
            for ( int rdx = 0 ; rdx < (int)ARRAY_LEN(Reqd_REBx_CatNum_Str) ; ++rdx )
            {
                if ( !strncmp(IdRecord[RebfIdx].pCatNo, Reqd_REBx_CatNum_Str[rdx], strlen( Reqd_REBx_CatNum_Str[rdx] )) )
                {
                    RebfCardValid = true;
                    break;      // stop walking the Reqd_REBF_CatNum_Str list
                }
            }
        }
    }

    if ( !RebfCardValid && !PARM(PwaInvRebfDsb) )
    {
        PUSH_DIAG(PwaInvRebfTrp);
        Status = FAIL;
    }

    //-------------------------------------------------------------------------
    // check to see if the HSLA card is in the required HSLA card list
    //  and push a diagnostic if not
    if ( HslaIdx != -1 )
    {
        for ( int rdx = 0 ; rdx < (int)ARRAY_LEN(Reqd_HSLA_CatNum_Str) ; ++rdx )
        {
            if ( !strncmp(IdRecord[HslaIdx].pCatNo, Reqd_HSLA_CatNum_Str[rdx], strlen( Reqd_HSLA_CatNum_Str[rdx] )) )
            {
                HslaCardValid = true;
                break;      // stop walking the Reqd_HSLA_CatNum_Str list
            }
        }
    }
    if ( !HslaCardValid && !PARM(PwaInvHslaDsb) )
    {
        PUSH_DIAG(PwaInvHslaTrp);
        Status = FAIL;
    }

    //-------------------------------------------------------------------------
    // push a diagnostic if HSLA card not on the correct REBF site
    if ( (RebfIdx != -1) && (HslaIdx != -1) )
    {
        HslaConnStr = IdRecord[RebfIdx].pDsr[0];            // connector for HSLA is in REBF design specific record 0 (dsr0)
        //HslaConnStr = IdRecord[RebfIdx].pDsr00;             // connector for HSLA is in REBF design specific record 0 (dsr0)
        Hssl.LinkIdInfo.SetConnName( HslaConnStr, HslaIdx );
    }
    if ( strcmp(HslaConnStr, Reqd_HslaConn_Str) )
    {
        PUSH_DIAG(PwaInvHslaSiteTrp);
        Status = FAIL;
    }

    //-------------------------------------------------------------------------
    // done
    delete [] IdRecord;
    return Status;
}
/*****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Validate primary pwa list for REBH
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      SUCCESS / FAIL
//
/////////////////////////////////////////////////////////////////////////////
int cBridgeRebf::ValdteRebhPrPwaLst(void)
{
    int         Status = SUCCESS;

    int         RebhIdx = -1;          // pwa id record array index for REBH (-1 = not found)

    bool        RebhCardValid = false;

    //-------------------------------------------------------------------------
    // retrieve the id records for local use
    int IdCnt = Hssl.LinkIdInfo.GetNumIds();
    cLinkIdInfo::IdRecordTyp* IdRecord;
    IdRecord = new (std::nothrow) cLinkIdInfo::IdRecordTyp[IdCnt];
    if (IdRecord == NULL)
        return FAIL;

    if ( Hssl.LinkIdInfo.GetPwaIds( IdRecord, IdCnt ) != SUCCESS )
    {
        delete [] IdRecord;
        return FAIL;
    }

    //-------------------------------------------------------------------------
    // scan the id records for required bus 0 cards (REBH)
    for ( int idx = 0 ; idx < IdCnt ; ++idx )
    {
        // only look at catalog numbers on bus #0 (babble)
        if ( IdRecord[idx].BusNo != 0 )
        {
            continue;   // go to the next record in the LinkIdInfo list
        }

        // look for REBH card
        if ( !strcmp(IdRecord[idx].pCardBaseName, "REBH") )
        {
            Hssl.LinkIdInfo.SetCardType( CARD_ACQUISITION, idx );
            Hssl.LinkIdInfo.SetConnName( NULL_Str, idx );    // connector for REBF is n/a (not applicable)
            Hssl.LinkIdInfo.SetCardIndx( 0, idx );
            RebhIdx = idx;
        }
    }

    //-------------------------------------------------------------------------
    // check to see if the REBH card is in the required REBH card list
    //  and push a diagnostic if not

    if ( RebhIdx != -1 )
    {
        // Determine whether REBH is a prototype
        if ( !strncmp(IdRecord[RebhIdx].pCatNo, REBx_CatNum_Str_Pr, strlen( REBx_CatNum_Str_Pr )) )
        {
            // REBx is a prototype - check it's an acceptable version
            for ( int rdx = 0 ; rdx < (int)ARRAY_LEN(Reqd_REBx_CatNum_Str_Pr) ; ++rdx )
            {
                if ( !strncmp(IdRecord[RebhIdx].pCatNo, Reqd_REBx_CatNum_Str_Pr[rdx], strlen( Reqd_REBx_CatNum_Str_Pr[rdx] )) )
                {
                    RebhCardValid = true;
                    break;      // stop walking the Reqd_REBx_CatNum_Str_PrReq list
                }
            }
        }
        else
        {
            // REBx is not a prototype - check it's an acceptable version
            for ( int rdx = 0 ; rdx < (int)ARRAY_LEN(Reqd_REBx_CatNum_Str) ; ++rdx )
            {
                if ( !strncmp(IdRecord[RebhIdx].pCatNo, Reqd_REBx_CatNum_Str[rdx], strlen( Reqd_REBx_CatNum_Str[rdx] )) )
                {
                    RebhCardValid = true;
                    break;      // stop walking the Reqd_REBx_CatNum_Str list
                }
            }
        }
    }

    if ( !RebhCardValid && !PARM(PwaInvRebhDsb) )
    {
        PUSH_DIAG(PwaInvRebhTrp);
        Status = FAIL;
    }

    //-------------------------------------------------------------------------
    // done
    delete [] IdRecord;
    return Status;
}
/**************************************************************************************/


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Validate Aux1 pwa list (J1/P1)
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      SUCCESS / FAIL
//
/////////////////////////////////////////////////////////////////////////////
int cBridgeRebf::ValidateAuxPwaList(void)
{
    int         Status = SUCCESS;

    int         PwaIdxJ1 = -1;          // pwa id record array index for card connected to REBF J1 (-1 = not found)
    int         PwaIdxJ2 = -1;          // pwa id record array index for card connected to REBF J2 (-1 = not found)
    int         PwaIdxP1 = -1;          // pwa id record array index for card connected to REBF P1 (-1 = not found)
    int         PwaIdxP2 = -1;          // pwa id record array index for card connected to REBF P2 (-1 = not found)

    bool        Brg1InfcCardValid = false;

    //-------------------------------------------------------------------------
    // retrieve the id records for local use
    int IdCnt = Hssl.LinkIdInfo.GetNumIds();
    cLinkIdInfo::IdRecordTyp* IdRecord;
    IdRecord = new (std::nothrow) cLinkIdInfo::IdRecordTyp[IdCnt];
    if (IdRecord == NULL)
        return FAIL;

    if ( Hssl.LinkIdInfo.GetPwaIds( IdRecord, IdCnt ) != SUCCESS )
    {
        delete [] IdRecord;
        return FAIL;
    }

    //-------------------------------------------------------------------------
    // determine the connector and record number for each card
    for ( int idx = 0 ; idx < IdCnt ; ++idx )
    {
        // look for J1 or J2 card (assumes AEBI,AEDB can only be on J1 or J2, J1 on bus 1, J2 on bus 2)
        if ( !strcmp(IdRecord[idx].pCardBaseName, "AEBI") ||
             !strcmp(IdRecord[idx].pCardBaseName, "AEDB") )
        {
            // HSLA site JA1 wires to bridge interface connector J1
            if ( HslaSite_JA1 )
            {
                Hssl.LinkIdInfo.SetCardType( CARD_TERMINAL, idx );
                Hssl.LinkIdInfo.SetConnName( J1_Str, idx );
                Hssl.LinkIdInfo.SetCardIndx( 2, idx );
                PwaIdxJ1 = idx;
            }
            // HSLA site JA2 wires to bridge interface connector J2
            else if ( !HslaSite_JA1 )
            {
                Hssl.LinkIdInfo.SetCardType( CARD_TERMINAL, idx );
                Hssl.LinkIdInfo.SetConnName( J2_Str, idx );
                Hssl.LinkIdInfo.SetCardIndx( 4, idx );
                PwaIdxJ2 = idx;
            }
        }
        // look for P1 or P2 card (assumes AEAA,AEAB,AEAC,AEAD can only be on P1 or P2, P1 on bus 1, P2 on bus 2)
        else if ( !strcmp(IdRecord[idx].pCardBaseName, "AEAA") ||
                  !strcmp(IdRecord[idx].pCardBaseName, "AEAB") ||
                  !strcmp(IdRecord[idx].pCardBaseName, "AEAC") ||
                  !strcmp(IdRecord[idx].pCardBaseName, "AEAD") )
        {
            // HSLA site JA1 wires to i/o interface connector P1
            if ( HslaSite_JA1 )
            {
                Hssl.LinkIdInfo.SetCardType( CARD_TERMINAL, idx );
                Hssl.LinkIdInfo.SetConnName( P1_Str, idx );
                Hssl.LinkIdInfo.SetCardIndx( 3, idx );
                PwaIdxP1 = idx;
            }
            // HSLA site JA2 wires to i/o interface connector P2
            else if ( !HslaSite_JA1 )
            {
                Hssl.LinkIdInfo.SetCardType( CARD_TERMINAL, idx );
                Hssl.LinkIdInfo.SetConnName( P2_Str, idx );
                Hssl.LinkIdInfo.SetCardIndx( 5, idx );
                PwaIdxP2 = idx;
            }
        }
    }

    //-------------------------------------------------------------------------
    // if HSLA site JA1 then check J1 and P1
    if ( HslaSite_JA1 )
    {
        //---------------------------------------------------------------------
        // check to see if the J1 card is in the required bridge 1 interface card list
        //  required cat number array length == 0 indicates nothing should be connected
        if ( Reqd_Brg1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxJ1 != -1) && !PARM(PwaInvRebfJ1Dsb) )
            {
                PUSH_DIAG(PwaInvRebfJ1Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Brg1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxJ1].pCatNo, Reqd_Brg1Infc_CatNum_Str[rdx], strlen( Reqd_Brg1Infc_CatNum_Str[rdx] )) )
                {
                    Brg1InfcCardValid = true;
                    break;      // stop walking the Reqd_Brg1Infc_CatNum_Str list
                }
            }
            if ( !Brg1InfcCardValid && !PARM(PwaInvRebfJ1Dsb) )
            {
                PUSH_DIAG(PwaInvRebfJ1Trp);
                Status = FAIL;
            }
        }

        //---------------------------------------------------------------------
        // check to see if the P1 card is in the required i/o 1 interface card list
        //  required cat number array length == 0 indicates nothing should be connected
        if ( Reqd_Io1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxP1 != -1) && !PARM(PwaInvRebfP1Dsb) )
            {
                PUSH_DIAG(PwaInvRebfP1Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Io1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxP1].pCatNo, Reqd_Io1Infc_CatNum_Str[rdx], strlen( Reqd_Io1Infc_CatNum_Str[rdx] )) )
                {
                    Io1InfcCardValid = true;
                    break;      // stop walking the Reqd_Io1Infc_CatNum_Str list
                }
            }
            if ( !Io1InfcCardValid && !PARM(PwaInvRebfP1Dsb) )
            {
                PUSH_DIAG(PwaInvRebfP1Trp);
                Status = FAIL;
            }
        }
    }

    //-------------------------------------------------------------------------
    // if HSLA site JA2 then check J2 and P2
    if ( !HslaSite_JA1 )
    {
        //---------------------------------------------------------------------
        // check to see if the J2 card is in the required bridge 1 interface card list
        //  required cat number array length == 0 indicates nothing should be connected
        if ( Reqd_Brg1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxJ2 != -1) && !PARM(PwaInvRebfJ2Dsb) )
            {
                PUSH_DIAG(PwaInvRebfJ2Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Brg1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxJ2].pCatNo, Reqd_Brg1Infc_CatNum_Str[rdx], strlen( Reqd_Brg1Infc_CatNum_Str[rdx] )) )
                {
                    Brg1InfcCardValid = true;
                    break;      // stop walking the Reqd_Brg1Infc_CatNum_Str list
                }
            }
            if ( !Brg1InfcCardValid && !PARM(PwaInvRebfJ2Dsb) )
            {
                PUSH_DIAG(PwaInvRebfJ2Trp);
                Status = FAIL;
            }
        }

        //---------------------------------------------------------------------
        // check to see if the P2 card is in the required i/o 1 interface card list
        //  required cat number array length == 0 indicates nothing should be connected
        if ( Reqd_Io1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxP2 != -1) && !PARM(PwaInvRebfP2Dsb) )
            {
                PUSH_DIAG(PwaInvRebfP2Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Io1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxP2].pCatNo, Reqd_Io1Infc_CatNum_Str[rdx], strlen( Reqd_Io1Infc_CatNum_Str[rdx] )) )
                {
                    Io1InfcCardValid = true;
                    break;      // stop walking the Reqd_Io1Infc_CatNum_Str list
                }
            }
            if ( !Io1InfcCardValid && !PARM(PwaInvRebfP2Dsb) )
            {
                PUSH_DIAG(PwaInvRebfP2Trp);
                Status = FAIL;
            }
        }
    }

    //-------------------------------------------------------------------------
    // done
    delete [] IdRecord;
    return Status;
}

/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Validate Aux PWA list
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      SUCCESS / FAIL
//
/////////////////////////////////////////////////////////////////////////////
int cBridgeRebf::ValdteRebhAuxPwaLst(void)
{
    int         Status = SUCCESS;

    int         PwaIdxP1 = -1;          // pwa id record array index for card connected to REBH P1 (-1 = not found)
    int         PwaIdxP2 = -1;          // pwa id record array index for card connected to REBH P2 (-1 = not found)
    int         PwaIdxR1 = -1;          // pwa id record array index for card connected to REBH R1 (-1 = not found)
    int         PwaIdxJ01 = -1;         // pwa id record array index for card connected to REBH J01 (-1 = not found)
    int         PwaIdxJ02 = -1;         // pwa id record array index for card connected to REBH J02 (-1 = not found)
    int         PwaIdxJ03 = -1;         // pwa id record array index for card connected to REBH J03 (-1 = not found)
    int         PwaIdxJ04 = -1;         // pwa id record array index for card connected to REBH J04 (-1 = not found)
    int         PwaIdxJ06 = -1;         // pwa id record array index for card connected to REBH J02 (-1 = not found)
    int         PwaIdxJ07 = -1;         // pwa id record array index for card connected to REBH J03 (-1 = not found)
    int         PwaIdxJ08 = -1;         // pwa id record array index for card connected to REBH J04 (-1 = not found)


    int         BrgCardCnt = 3;

    bool        Brg1InfcCardValid = false;
    bool        PwrInfcCardValid  = false;

    bool        LnSdl1LnkBrgErr = false;
    bool        LnSdl2LnkBrgErr = false;
    bool        LnSdl3LnkBrgErr = false;
    bool        LnSdl4LnkBrgErr = false;
    bool        LnSdl6LnkBrgErr = false;
    bool        LnSdl7LnkBrgErr = false;
    bool        LnSdl8LnkBrgErr = false;


    //-------------------------------------------------------------------------
    // retrieve the id records for local use
    int IdCnt = Hssl.LinkIdInfo.GetNumIds();
    cLinkIdInfo::IdRecordTyp* IdRecord;
    IdRecord = new (std::nothrow) cLinkIdInfo::IdRecordTyp[IdCnt];
    if (IdRecord == NULL)
        return FAIL;

    if ( Hssl.LinkIdInfo.GetPwaIds( IdRecord, IdCnt ) != SUCCESS )
    {
        delete [] IdRecord;
        return FAIL;
    }


    //-------------------------------------------------------------------------
    // determine the connector and record number for each card
    for ( int idx = 0 ; idx < IdCnt ; ++idx )
    {
        // look for P1 or P2 card (assumes AEAA,AEAB,AEAC,AEAD can only be on P1 or P2, P1 on bus 1, P2 on bus 2)
        if (      !strcmp(IdRecord[idx].pCardBaseName, "AEAA") ||
                  !strcmp(IdRecord[idx].pCardBaseName, "AEAB") ||
                  !strcmp(IdRecord[idx].pCardBaseName, "AEAC") ||
                  !strcmp(IdRecord[idx].pCardBaseName, "AEAD") )
        {
            // HSLA site JA1 wires to i/o interface connector P1
            if ( strncmp(INS(Uniquifier), "L", 1) == 0 )
            {
                Hssl.LinkIdInfo.SetCardType( CARD_TERMINAL, idx );
                Hssl.LinkIdInfo.SetConnName( P1_Str, idx );
                Hssl.LinkIdInfo.SetCardIndx( 2, idx );
                PwaIdxP1 = idx;
            }
            // HSLA site JA2 wires to i/o interface connector P2
            else
            {
                Hssl.LinkIdInfo.SetCardType( CARD_TERMINAL, idx );
                Hssl.LinkIdInfo.SetConnName( P2_Str, idx );
                Hssl.LinkIdInfo.SetCardIndx( 7, idx );
                PwaIdxP2 = idx;
            }
        }
        else if ( !strcmp(IdRecord[idx].pCardBaseName, "REPI") )
        {
                Hssl.LinkIdInfo.SetCardType( CARD_TERMINAL, idx );
                Hssl.LinkIdInfo.SetConnName( REPI_Str, idx );
                Hssl.LinkIdInfo.SetCardIndx( 1, idx );
                PwaIdxR1 = idx;
        }

        else if ( !strcmp(IdRecord[idx].pCardBaseName, "RGIB") )
        {
            // HSLA site JA1 wires to i/o interface connector P1
            if ( strncmp(INS(Uniquifier), "L", 1) == 0 )
            {
                Hssl.LinkIdInfo.SetCardType( CARD_TERMINAL, idx );
                Hssl.LinkIdInfo.SetCardIndx( BrgCardCnt, idx );

                if ((PlugInfoArr1[BrgCardCnt - 3]) == 0x00B0)
                {
                   //strcpy(BrgCon_Str[0], "J01");
                   if ( BrgCardCnt - 3 != 0)
                   {
                    LnSdl1LnkBrgErr = true;
                   }

                   Hssl.LinkIdInfo.SetConnName( J01_Str, idx );
                   PwaIdxJ01 = idx;
                }
                else if ((PlugInfoArr1[BrgCardCnt - 3]) == 0x00A0)
                {
                   //strcpy(BrgCon_Str[1], "J02");
                   if ( BrgCardCnt - 3 != 1)
                   {
                    LnSdl2LnkBrgErr = true;
                   }

                   Hssl.LinkIdInfo.SetConnName( J02_Str, idx );
                   PwaIdxJ02 = idx;
                }
                else if ((PlugInfoArr1[BrgCardCnt - 3]) == 0x0090)
                {
                   //strcpy(BrgCon_Str[2], "J03");
                   if ( BrgCardCnt - 3 != 2)
                   {
                    LnSdl3LnkBrgErr = true;
                   }

                   Hssl.LinkIdInfo.SetConnName( J03_Str, idx );
                   PwaIdxJ03 = idx;
                }
                else if ((PlugInfoArr1[BrgCardCnt - 3]) == 0x0080)
                {
                   //strcpy(BrgCon_Str[3], "J04");
                   if ( BrgCardCnt - 3 != 3)
                   {
                    LnSdl4LnkBrgErr = true;
                   }

                   Hssl.LinkIdInfo.SetConnName( J04_Str, idx );
                   PwaIdxJ04 = idx;
                }
            }
            // HSLA site JA2 wires to i/o interface connector P2
            else
            {
                Hssl.LinkIdInfo.SetCardType( CARD_TERMINAL, idx );
                Hssl.LinkIdInfo.SetCardIndx( BrgCardCnt+5, idx );

                if ((PlugInfoArr2[BrgCardCnt - 3]) == 0x00E0)
                {
                   if ( BrgCardCnt - 3 != 0)
                   {
                    LnSdl6LnkBrgErr = true;
                   }

                   Hssl.LinkIdInfo.SetConnName( J06_Str, idx );
                   PwaIdxJ06 = idx;
                }
                else if ((PlugInfoArr2[BrgCardCnt - 3]) == 0x00D0)
                {
                   if ( BrgCardCnt - 3 != 1)
                   {
                    LnSdl7LnkBrgErr = true;
                   }

                   Hssl.LinkIdInfo.SetConnName( J07_Str, idx );
                   PwaIdxJ07 = idx;
                }
                else if ((PlugInfoArr2[BrgCardCnt - 3]) == 0x00C0)
                {
                   if ( BrgCardCnt - 3 != 2)
                   {
                    LnSdl8LnkBrgErr = true;
                   }

                   Hssl.LinkIdInfo.SetConnName( J08_Str, idx );
                   PwaIdxJ08 = idx;
                }

                //PwaIdxP2 = idx;
            }
            BrgCardCnt++;
        }
    }

    if ( PwaIdxR1 != -1 )
    {
        // Determine whether REBH is a prototype
        if ( !strncmp(IdRecord[PwaIdxR1].pCatNo, REPI_H1_CatNum_Str_Pr, strlen( REPI_H1_CatNum_Str_Pr )) )
        {
            // REBx is a prototype - check it's an acceptable version
            for ( int rdx = 0 ; rdx < (int)ARRAY_LEN(Reqd_REPI_H1_CatNum_Str_Pr) ; ++rdx )
            {
                if ( !strncmp(IdRecord[PwaIdxR1].pCatNo, Reqd_REPI_H1_CatNum_Str_Pr[rdx], strlen( Reqd_REPI_H1_CatNum_Str_Pr[rdx] )) )
                {
                    PwrInfcCardValid = true;
                    break;      // stop walking the Reqd_REBx_CatNum_Str_PrReq list
                }
            }
        }
        else
        {
            // REBx is not a prototype - check it's an acceptable version
            for ( int rdx = 0 ; rdx < (int)ARRAY_LEN(Reqd_REPI_H1_CatNum_Str) ; ++rdx )
            {
                if ( !strncmp(IdRecord[PwaIdxR1].pCatNo, Reqd_REPI_H1_CatNum_Str[rdx], strlen( Reqd_REPI_H1_CatNum_Str[rdx] )) )
                {
                    PwrInfcCardValid = true;
                    break;      // stop walking the Reqd_REBx_CatNum_Str list
                }
            }
        }
    }

    if ( !PwrInfcCardValid && !PARM(PwaInvRebhP1Dsb) )
    {
        PUSH_DIAG(PwaInvRebhP1Trp);
        Status = FAIL;
    }

    //-------------------------------------------------------------------------
    // if HSLA site JA1 then check J1 and P1
    if ( strncmp(INS(Uniquifier), "L", 1) == 0 )
    {
        //---------------------------------------------------------------------
        // check to see if the P1 card is in the required i/o 1 interface card list
        //  required cat number array length == 0 indicates nothing should be connected
        if ( Reqd_Io1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxP1 != -1) && !PARM(PwaInvRebhP1Dsb) )
            {
                PUSH_DIAG(PwaInvRebhP1Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Io1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxP1].pCatNo, Reqd_Io1Infc_CatNum_Str[rdx], strlen( Reqd_Io1Infc_CatNum_Str[rdx] )) )
                {
                    Io1InfcCardValid = true;
                    break;      // stop walking the Reqd_Io1Infc_CatNum_Str list
                }
            }

            if ( !Io1InfcCardValid && !PARM(PwaInvRebhP1Dsb) )
            {
                PUSH_DIAG(PwaInvRebhP1Trp);
                Status = FAIL;
            }
        }

        //---------------------------------------------------------------------
        // check to see if the J01-J04 card is in the required bridge 1 interface card list
        //  required cat number array length == 0 indicates nothing should be connected
        if ( Reqd_Brg1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxJ01 != -1) && !PARM(PwaInvRebhJ01Dsb) )
            {
                PUSH_DIAG(PwaInvRebhJ01Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Brg1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxJ01].pCatNo, Reqd_Brg1Infc_CatNum_Str[rdx], strlen( Reqd_Brg1Infc_CatNum_Str[rdx] )) )
                {
                    Brg1InfcCardValid = true;
                    break;      // stop walking the Reqd_Io1Infc_CatNum_Str list
                }
            }
            if ( (!Brg1InfcCardValid || LnSdl1LnkBrgErr) && !PARM(PwaInvRebhJ01Dsb) )
            {
                PUSH_DIAG(PwaInvRebhJ01Trp);
                Status = FAIL;
            }

            Brg1InfcCardValid = false;
        }

        if ( Reqd_Brg1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxJ02 != -1) && !PARM(PwaInvRebhJ02Dsb) )
            {
                PUSH_DIAG(PwaInvRebhJ02Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Brg1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxJ02].pCatNo, Reqd_Brg1Infc_CatNum_Str[rdx], strlen( Reqd_Brg1Infc_CatNum_Str[rdx] )) )
                {
                    Brg1InfcCardValid = true;
                    break;      // stop walking the Reqd_Io1Infc_CatNum_Str list
                }
            }
            if ( (!Brg1InfcCardValid || LnSdl2LnkBrgErr) && !PARM(PwaInvRebhJ02Dsb))
            {
                PUSH_DIAG(PwaInvRebhJ02Trp);
                Status = FAIL;
            }

            Brg1InfcCardValid = false;
        }

        if ( Reqd_Brg1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxJ03 != -1) && !PARM(PwaInvRebhJ03Dsb))
            {
                PUSH_DIAG(PwaInvRebhJ03Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Brg1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxJ03].pCatNo, Reqd_Brg1Infc_CatNum_Str[rdx], strlen( Reqd_Brg1Infc_CatNum_Str[rdx] )) )
                {
                    Brg1InfcCardValid = true;
                    break;      // stop walking the Reqd_Io1Infc_CatNum_Str list
                }
            }
            if ( (!Brg1InfcCardValid || LnSdl3LnkBrgErr) && !PARM(PwaInvRebhJ03Dsb))
            {
                PUSH_DIAG(PwaInvRebhJ03Trp);
                Status = FAIL;
            }

            Brg1InfcCardValid = false;
        }

        if ( Reqd_Brg1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxJ04 != -1) && !PARM(PwaInvRebhJ04Dsb) )
            {
                PUSH_DIAG(PwaInvRebhJ04Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Brg1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxJ04].pCatNo, Reqd_Brg1Infc_CatNum_Str[rdx], strlen( Reqd_Brg1Infc_CatNum_Str[rdx] )) )
                {
                    Brg1InfcCardValid = true;
                    break;      // stop walking the Reqd_Io1Infc_CatNum_Str list
                }
            }
            if ( (!Brg1InfcCardValid || LnSdl4LnkBrgErr) && !PARM(PwaInvRebhJ04Dsb))
            {
                PUSH_DIAG(PwaInvRebhJ04Trp);
                Status = FAIL;
            }

            Brg1InfcCardValid = false;
        }
    }

    //-------------------------------------------------------------------------
    // if HSLA site JA2 then check J2 and P2
    if ( strncmp(INS(Uniquifier), "L", 1) != 0 )
    {
        //---------------------------------------------------------------------
        // check to see if the P2 card is in the required i/o 1 interface card list
        //  required cat number array length == 0 indicates nothing should be connected
        if ( Reqd_Io1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxP2 != -1) && !PARM(PwaInvRebhP2Dsb) )
            {
                PUSH_DIAG(PwaInvRebhP2Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Io1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxP2].pCatNo, Reqd_Io1Infc_CatNum_Str[rdx], strlen( Reqd_Io1Infc_CatNum_Str[rdx] )) )
                {
                    Io1InfcCardValid = true;
                    break;      // stop walking the Reqd_Io1Infc_CatNum_Str list
                }
            }
            if ( !Io1InfcCardValid && !PARM(PwaInvRebhP2Dsb) )
            {
                PUSH_DIAG(PwaInvRebhP2Trp);
                Status = FAIL;
            }
        }

        //---------------------------------------------------------------------
        // check to see if the J06-J08 card is in the required bridge 1 interface card list
        //  required cat number array length == 0 indicates nothing should be connected
        if ( Reqd_Brg1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxJ06 != -1) && !PARM(PwaInvRebhJ06Dsb) )
            {
                PUSH_DIAG(PwaInvRebhJ06Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Brg1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxJ06].pCatNo, Reqd_Brg1Infc_CatNum_Str[rdx], strlen( Reqd_Brg1Infc_CatNum_Str[rdx] )) )
                {
                    Brg1InfcCardValid = true;
                    break;      // stop walking the Reqd_Io1Infc_CatNum_Str list
                }
            }
            if ( (!Brg1InfcCardValid || LnSdl6LnkBrgErr) && !PARM(PwaInvRebhJ06Dsb) )
            {
                PUSH_DIAG(PwaInvRebhJ06Trp);
                Status = FAIL;
            }

            Brg1InfcCardValid = false;
        }

        if ( Reqd_Brg1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxJ07 != -1) && !PARM(PwaInvRebhJ07Dsb) )
            {
                PUSH_DIAG(PwaInvRebhJ07Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Brg1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxJ07].pCatNo, Reqd_Brg1Infc_CatNum_Str[rdx], strlen( Reqd_Brg1Infc_CatNum_Str[rdx] )) )
                {
                    Brg1InfcCardValid = true;
                    break;      // stop walking the Reqd_Io1Infc_CatNum_Str list
                }
            }
            if ( (!Brg1InfcCardValid || LnSdl7LnkBrgErr) && !PARM(PwaInvRebhJ07Dsb))
            {
                PUSH_DIAG(PwaInvRebhJ07Trp);
                Status = FAIL;
            }

            Brg1InfcCardValid = false;
        }

        if ( Reqd_Brg1Infc_CatNum_ArrLen == 0 )
        {
            // check to see if something plugged in when there should not be
            if ( (PwaIdxJ08 != -1) && !PARM(PwaInvRebhJ08Dsb))
            {
                PUSH_DIAG(PwaInvRebhJ08Trp);
                Status = FAIL;
            }
        }
        else
        {
            for ( int rdx = 0 ; rdx < Reqd_Brg1Infc_CatNum_ArrLen ; ++rdx )
            {
                // compare for a match
                if ( !strncmp(IdRecord[PwaIdxJ08].pCatNo, Reqd_Brg1Infc_CatNum_Str[rdx], strlen( Reqd_Brg1Infc_CatNum_Str[rdx] )) )
                {
                    Brg1InfcCardValid = true;
                    break;      // stop walking the Reqd_Io1Infc_CatNum_Str list
                }
            }
            if ( (!Brg1InfcCardValid || LnSdl8LnkBrgErr) && !PARM(PwaInvRebhJ08Dsb))
            {
                PUSH_DIAG(PwaInvRebhJ08Trp);
                Status = FAIL;
            }

            Brg1InfcCardValid = false;
        }

    }

    //-------------------------------------------------------------------------
    // done
    delete [] IdRecord;
    return Status;
}

/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculate the AEAD gain for feedback calibration
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      SUCCESS / FAIL
//
/////////////////////////////////////////////////////////////////////////////
int cBridgeRebf::AeadGnCalc(void)
{

    float PwaFrqIo1[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    Io1AeadGainFlt = false;
    Io1AeadOffsFlt = false;
    Io1AeadDsrFlt  = false;
    //-------------------------------------------------------------------------
    // retrieve the id records for local use
    int IdCnt = Hssl.LinkIdInfo.GetNumIds();
    cLinkIdInfo::IdRecordTyp* IdRecord;
    IdRecord = new (std::nothrow) cLinkIdInfo::IdRecordTyp[IdCnt];
    if (IdRecord == NULL)
        return FAIL;

    if ( Hssl.LinkIdInfo.GetPwaIds( IdRecord, IdCnt ) != SUCCESS )
    {
        delete [] IdRecord;
        return FAIL;
    }

        for (int id = 0; id < IdCnt; id++)
        {
            if ( !strcmp(IdRecord[id].pCardBaseName, "AEAD"))
            {
                //-------------------------------------------------------------------------
                // Io1 card on Hsla Site JA1 connected to P1 or Io1 card on Hsla Site JA2 connected to P2
                if ( (!strcmp(IdRecord[id].pConnName, "P1") && HslaSite_JA1  )
                   ||(!strcmp(IdRecord[id].pConnName, "P2") && !HslaSite_JA1 )
                   || RebhSet)
                {
                    if ( !Io1InfcCardValid )
                    {
                        PUSH_DIAG(IoAeadDsrBadTrp);

                        Io1VanPFrq  = 0.0;
                        Io1VbnPFrq  = 0.0;
                        Io1VcnPFrq  = 0.0;
                        Io1VanNFrq  = 0.0;
                        Io1VbnNFrq  = 0.0;
                        Io1VcnNFrq  = 0.0;
                        Io1IaPFrq   = 0.0;
                        Io1IbPFrq   = 0.0;
                        Io1IcPFrq   = 0.0;
                        Io1IaNFrq   = 0.0;
                        Io1IbNFrq   = 0.0;
                        Io1IcNFrq   = 0.0;

                        Io1AeadDsrFlt   = true;
                    }
                    else
                    {

                        for ( int idx = 0; idx < (int)ARRAY_LEN(IdRecord[id].pDsr); idx++)
                         {

                                PwaFrqIo1[idx]  = atof(IdRecord[id].pDsr[idx] );
                         }

                        //-------------------------------------------------------------------------
                        //Scale back to test data frequency for each of 12 records
                        Io1VanPFrq  = 20.0*PwaFrqIo1[0];
                        Io1VanNFrq  = 20.0*PwaFrqIo1[1];
                        Io1VbnPFrq  = 20.0*PwaFrqIo1[2];
                        Io1VbnNFrq  = 20.0*PwaFrqIo1[3];
                        Io1VcnPFrq  = 20.0*PwaFrqIo1[4];
                        Io1VcnNFrq  = 20.0*PwaFrqIo1[5];
                        Io1IaPFrq   = 20.0*PwaFrqIo1[6];
                        Io1IaNFrq   = 20.0*PwaFrqIo1[7];
                        Io1IbPFrq   = 20.0*PwaFrqIo1[8];
                        Io1IbNFrq   = 20.0*PwaFrqIo1[9];
                        Io1IcPFrq   = 20.0*PwaFrqIo1[10];
                        Io1IcNFrq   = 20.0*PwaFrqIo1[11];

                        //-------------------------------------------------------------------------
                        // Io1 Voltage and current scaling calculations
                        Io1PwaVanSc = PARM(Io1VlnHvTestScl)/(Io1VanPFrq - Io1VanNFrq);
                        Io1PwaVbnSc = PARM(Io1VlnHvTestScl)/(Io1VbnPFrq - Io1VbnNFrq);
                        Io1PwaVcnSc = PARM(Io1VlnHvTestScl)/(Io1VcnPFrq - Io1VcnNFrq);

                        Io1PwaIaScl = PARM(Io1CTITestScl) / (Io1IaPFrq - Io1IaNFrq);
                        Io1PwaIbScl = PARM(Io1CTITestScl) / (Io1IbPFrq - Io1IbNFrq);
                        Io1PwaIcScl = PARM(Io1CTITestScl) / (Io1IcPFrq - Io1IcNFrq);

                        if (    ( ABS(Io1PwaVanSc - SclPhsVoltsIo1) > PARM(VGnErMax) * SclPhsVoltsIo1 ) ||
                                ( ABS(Io1PwaVbnSc - SclPhsVoltsIo1) > PARM(VGnErMax) * SclPhsVoltsIo1 ) ||
                                ( ABS(Io1PwaVcnSc - SclPhsVoltsIo1) > PARM(VGnErMax) * SclPhsVoltsIo1 ) ||

                                ( ABS(Io1PwaIaScl - SclPhsCurIo1) > PARM(IGnErMax) * SclPhsCurIo1 ) ||
                                ( ABS(Io1PwaIbScl - SclPhsCurIo1) > PARM(IGnErMax) * SclPhsCurIo1 ) ||
                                ( ABS(Io1PwaIcScl - SclPhsCurIo1) > PARM(IGnErMax) * SclPhsCurIo1 )
                           )
                        {
                            if ( INS(TestMode) == PUMP_BACK )
                            {
                                PUSH_DIAG(IoPwaGainTrp);
                            }
                            else
                            {
                                PUSH_DIAG(IoPwaGainAlm);
                            }
                            Io1AeadGainFlt = true;
                        }
                        else
                        {

                            Io1VanScl   = Io1PwaVanSc;
                            Io1VbnScl   = Io1PwaVbnSc;
                            Io1VcnScl   = Io1PwaVcnSc;
                            Io1IaScl    = -Io1PwaIaScl;
                            Io1IbScl    = -Io1PwaIbScl;
                            Io1IcScl    = -Io1PwaIcScl;
                        }

                        //-------------------------------------------------------------------------
                        //Io1 Voltage and current offset calculations
                        Io1PwaOffs2 =  ( Io1VanNFrq + 0.5*(Io1VanPFrq - Io1VanNFrq)- IO_VCO_FZRO);
                        Io1PwaOffs3 =  ( Io1VbnNFrq + 0.5*(Io1VbnPFrq - Io1VbnNFrq)- IO_VCO_FZRO);
                        Io1PwaOffs4 =  ( Io1VcnNFrq + 0.5*(Io1VcnPFrq - Io1VcnNFrq)- IO_VCO_FZRO);

                        Io1PwaOffs5 =  ( Io1IaNFrq + 0.5*(Io1IaPFrq - Io1IaNFrq)   - IO_VCO_FZRO);
                        Io1PwaOffs6 =  ( Io1IbNFrq + 0.5*(Io1IbPFrq - Io1IbNFrq)   - IO_VCO_FZRO);
                        Io1PwaOffs7 =  ( Io1IcNFrq + 0.5*(Io1IcPFrq - Io1IcNFrq)   - IO_VCO_FZRO);

                        if (    ( ABS(Io1PwaOffs2) > PARM(OffErMax) * IO_VCO_FZRO )  ||
                                ( ABS(Io1PwaOffs3) > PARM(OffErMax) * IO_VCO_FZRO )  ||
                                ( ABS(Io1PwaOffs4) > PARM(OffErMax) * IO_VCO_FZRO )  ||
                                ( ABS(Io1PwaOffs5) > PARM(OffErMax) * IO_VCO_FZRO )  ||
                                ( ABS(Io1PwaOffs6) > PARM(OffErMax) * IO_VCO_FZRO )  ||
                                ( ABS(Io1PwaOffs7) > PARM(OffErMax) * IO_VCO_FZRO )
                           )
                        {
                            if ( INS(TestMode) == PUMP_BACK )
                            {
                                PUSH_DIAG(IoPwaOffsTrp);
                            }
                            else
                            {
                                PUSH_DIAG(IoPwaOffsAlm);
                            }
                            Io1AeadOffsFlt = true;
                        }
                        else
                        {
                            Io1AeaxAin2Zero = (float)IO_VCO_FZRO + Io1PwaOffs2;
                            Io1AeaxAin3Zero = (float)IO_VCO_FZRO + Io1PwaOffs3;
                            Io1AeaxAin4Zero = (float)IO_VCO_FZRO + Io1PwaOffs4;
                            Io1AeaxAin5Zero = (float)IO_VCO_FZRO + Io1PwaOffs5;
                            Io1AeaxAin6Zero = (float)IO_VCO_FZRO + Io1PwaOffs6;
                            Io1AeaxAin7Zero = (float)IO_VCO_FZRO + Io1PwaOffs7;
                        }
                    }
                }
            }
        }

        //-------------------------------------------------------------------------
        // done
    delete [] IdRecord;
        return SUCCESS;
    }

/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set the default AEAD gain for each card
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      NONE
//
/////////////////////////////////////////////////////////////////////////////
void cBridgeRebf::AeadGnDefault(void)
{

    //-------------------------------------------------------------------------
    // Set all Io1 Gains to defaults if sim mode without bridge enable
    // or AeadIDAdjInh is override to manually inhibit the calibration
    // or any gain or offset test failed

    if ( (INS(SimMode)  && !PARM(SimModeBrgEnb)) || PARM(AeadIDAdjInh) ||
          Io1AeadDsrFlt || Io1AeadGainFlt        || Io1AeadOffsFlt )
    {
        Io1AeaxAin2Zero = IO_VCO_FZRO;
        Io1AeaxAin3Zero = IO_VCO_FZRO;
        Io1AeaxAin4Zero = IO_VCO_FZRO;
        Io1AeaxAin5Zero = IO_VCO_FZRO;
        Io1AeaxAin6Zero = IO_VCO_FZRO;
        Io1AeaxAin7Zero = IO_VCO_FZRO;

        Io1VanScl       = SclPhsVoltsIo1;
        Io1VbnScl       = SclPhsVoltsIo1;
        Io1VcnScl       = SclPhsVoltsIo1;
        Io1IaScl        = SclPhsCurIo1;
        Io1IbScl        = SclPhsCurIo1;
        Io1IcScl        = SclPhsCurIo1;
    }

    //-------------------------------------------------------------------------
    // done
    return;
}

/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculate the AEAA gain for feedback calibration
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      SUCCESS / FAIL
//
/////////////////////////////////////////////////////////////////////////////

int cBridgeRebf::AeaaGnCalc(void)
{
    //-------------------------------------------------------------------------
    // retrieve the id records for local use
    int IdCnt = Hssl.LinkIdInfo.GetNumIds();
    cLinkIdInfo::IdRecordTyp* IdRecord;
    IdRecord = new (std::nothrow) cLinkIdInfo::IdRecordTyp[IdCnt];
    if (IdRecord == NULL)
        return FAIL;

    if ( Hssl.LinkIdInfo.GetPwaIds( IdRecord, IdCnt ) != SUCCESS )
    {
        delete [] IdRecord;
        return FAIL;
    }
        for (int id = 0; id < IdCnt; id++)
        {
            if ( !strcmp(IdRecord[id].pCardBaseName, "AEAA"))
            {
                //-------------------------------------------------------------------------
                // Io1 card
                if (( !strcmp(IdRecord[id].pConnName, "P1") && HslaSite_JA1)
                  ||( !strcmp(IdRecord[id].pConnName, "P2") && !HslaSite_JA1))
                {
                    Io1AeaxAin2Zero = IO_VCO_FZRO;
                    Io1AeaxAin3Zero = IO_VCO_FZRO;
                    Io1AeaxAin4Zero = IO_VCO_FZRO;
                    Io1AeaxAin5Zero = IO_VCO_FZRO;
                    Io1AeaxAin6Zero = IO_VCO_FZRO;
                    Io1AeaxAin7Zero = IO_VCO_FZRO;

                    Io1VanScl   = PARM(S_VabVcoScl);
                    Io1VbnScl   = PARM(S_VbcVcoScl);
                    Io1VcnScl   = PARM(S_VbcVcoScl);  // There is no Vca signal; this is set here for AeaaHv3Fbk signal
                    Io1IaScl    = -(PARM(S_IaVcoScl) * INS(CtRatioIo1)) / PARM(S_CTburdenR);
                    Io1IbScl    = -(PARM(S_IbVcoScl) * INS(CtRatioIo1)) / PARM(S_CTburdenR);
                    Io1IcScl    = -(PARM(S_IcVcoScl) * INS(CtRatioIo1)) / PARM(S_CTburdenR);
                }
            }
        }
        delete [] IdRecord;
        return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Initialize lock out time in fpga configurations.
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      1 = success, 0 = failure
//
///////////////////////////////////////////////////////////////////////////////
int cBridgeRebf::CalcBilRegister( float input, float scale, const BilCalcTyp& reg , int& status)
{
    int count = reg.offcnt + (int)( input * reg.sclcnt / scale );
    if (count < reg.mincnt)
    {
        count = reg.mincnt;
        status = FAIL;
    }
    else if (count > reg.maxcnt)
    {
        count = reg.maxcnt;
        status = FAIL;
    }
    else
    {
        status = SUCCESS;
    }
    return (count);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rules for bridge interface fpga confirguation.
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      1 = success, 0 = failure
//
///////////////////////////////////////////////////////////////////////////////
unsigned cBridgeRebf::InitializeBIL(void)
{
    int     CalcBilStatus;
    int     status = SUCCESS;

    CfgVal.Ccfg.DisableBrFlts  = false;
    CfgVal.Ccfg.DsbFrmErr      = false;             // enable frame error detection for late command packets
    CfgVal.Ccfg.TriGates       = true;              // disable tri-state of bridge state gate outputs
    CfgVal.Ccfg.BRLdis         = true;              // disable tri-state of IO#1&2
    CfgVal.Ccfg.LineSide       = INS(LineSide);     // select Line side specific behavior
    CfgVal.Ccfg.BatCntl        = false;             // phase D is used for battery chopper controller
    if      ( (((IoInfcHdwTyp)(INS(Io1InfcHdw))) == IO_INFC_AEAAH3) || (((IoInfcHdwTyp)(INS(Io1InfcHdw))) == IO_INFC_AEAAH2))
    {
        CfgVal.Ccfg.SelAEAA    = 1;
    }
    else
    {
        CfgVal.Ccfg.SelAEAA    = 0;
    }



    //-------------------------------------------------------------------------
    // dac output control
    CfgVal.DiagMuxSel.Diag1             = PARM(TargTstPt1Sel);
    CfgVal.DiagMuxSel.Diag2             = PARM(TargTstPt2Sel);

    //-------------------------------------------------------------------------
    // gate drive status (desat) masks and fpga configuration
    *(unsigned*)&DesatFltMskBr1 = 0;
    *(unsigned*)&DesatFltMskBr2 = 0;


    if (INS(Brg1InfcHdw != BRG_INFC_NONE))
    {   // bridge 1
        BrgCmd.FanRef.DesatEnbBr1   = true;
#if 0
        CfgVal.CrBrEnb1.AnegEnb     = (PARM(HwGdStatMask) & 0x0001) == 0 ? false : true;
        CfgVal.CrBrEnb1.BnegEnb     = (PARM(HwGdStatMask) & 0x0002) == 0 ? false : true;
        CfgVal.CrBrEnb1.CnegEnb     = (PARM(HwGdStatMask) & 0x0004) == 0 ? false : true;
        CfgVal.CrBrEnb1.AposEnb     = (PARM(HwGdStatMask) & 0x0008) == 0 ? false : true;
        CfgVal.CrBrEnb1.BposEnb     = (PARM(HwGdStatMask) & 0x0010) == 0 ? false : true;
        CfgVal.CrBrEnb1.CposEnb     = (PARM(HwGdStatMask) & 0x0020) == 0 ? false : true;
#endif
        // keep these for now to allow desat faults to be asserted (future: eliminate DesatFltMskBr1):
        DesatFltMskBr1.AnegB1       = (PARM(HwGdStatMask) & 0x0001) == 0 ? false : true;
        DesatFltMskBr1.BnegB1       = (PARM(HwGdStatMask) & 0x0002) == 0 ? false : true;
        DesatFltMskBr1.CnegB1       = (PARM(HwGdStatMask) & 0x0004) == 0 ? false : true;
        DesatFltMskBr1.AposB1       = (PARM(HwGdStatMask) & 0x0008) == 0 ? false : true;
        DesatFltMskBr1.BposB1       = (PARM(HwGdStatMask) & 0x0010) == 0 ? false : true;
        DesatFltMskBr1.CposB1       = (PARM(HwGdStatMask) & 0x0020) == 0 ? false : true;
        if (INS(PhsD1FltEnb))
        {
            CfgVal.CrBrEnb1.DdesatEnb = false;  // Dsat masked so it does not latch fpga bridge-fault
            CfgVal.CrBrEnb1.DdesatSDb = false;  // When Dsat occurs, phase-D switches are opened immmediately
            CfgVal.CrBrEnb1.DDisPeEn  = true;   // Phase-D is not affected when other phases latch the fpga bridge-fault
            DesatFltMskBr1.DdesatB1   = true;   // Firmware pushes Dsat faults if reported by fpga
        }
        else
        {
            CfgVal.CrBrEnb1.DdesatEnb = false;  // Dsat masked so it does not latch fpga bridge-fault
            CfgVal.CrBrEnb1.DdesatSDb = true;   // When Dsat occurs, phase-D switches are not opened immmediately
            CfgVal.CrBrEnb1.DDisPeEn  = false;  // Phase-D is affected when other phases latch the fpga bridge-fault
            DesatFltMskBr1.DdesatB1   = false;   // Firmware does not push Dsat faults if reported by fpga
        }

    }
    else
    {   // bridge 1 unused - disable all
        BrgCmd.FanRef.DesatEnbBr1   = false;
#if 0
        CfgVal.CrBrEnb1.AnegEnb     = false;
        CfgVal.CrBrEnb1.BnegEnb     = false;
        CfgVal.CrBrEnb1.CnegEnb     = false;
        CfgVal.CrBrEnb1.AposEnb     = false;
        CfgVal.CrBrEnb1.BposEnb     = false;
        CfgVal.CrBrEnb1.CposEnb     = false;
#endif

        DesatFltMskBr1.AnegB1       = false;
        DesatFltMskBr1.BnegB1       = false;
        DesatFltMskBr1.CnegB1       = false;
        DesatFltMskBr1.AposB1       = false;
        DesatFltMskBr1.BposB1       = false;
        DesatFltMskBr1.CposB1       = false;
        CfgVal.CrBrEnb1.DdesatEnb   = false;
        CfgVal.CrBrEnb1.DdesatSDb   = false;
        CfgVal.CrBrEnb1.DDisPeEn    = false;
        DesatFltMskBr1.DdesatB1     = false;

    }

    //-------------------------------------------------------------------------
    // set ioc enable for all phases (A thru D) and both bridges

    *(unsigned*)&StatIocFltMskBr1 = 0;
    *(unsigned*)&StatIocFltMskBr2 = 0;
    if (INS(Brg1InfcHdw != BRG_INFC_NONE))
    {   // bridge 1
        StatIocFltMskBr1.AiocB1  = true;
        StatIocFltMskBr1.BiocB1  = true;
        StatIocFltMskBr1.CiocB1  = true;
        StatIocFltMskBr1.DiocB1  = false;   // for Dc-chopper phase
    }
    else
    {   // bridge 1 unused - disable all
        StatIocFltMskBr1.AiocB1  = false;
        StatIocFltMskBr1.BiocB1  = false;
        StatIocFltMskBr1.CiocB1  = false;
        StatIocFltMskBr1.DiocB1  = false;
    }

    if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_B)
    {
        IocThreshold = (unsigned)(((PARM(PhsIocThrs) * (0.0625))/312.5F) * 255.0F ) + 256.0F; //0.0625 = 50mV/800A (shunt ratio)
        CfgVal.IO_Threshold.Threshold = IocThreshold;
        //CfgVal.IO_Threshold.Threshold = 0x01CC;  // 4000 A
        //CfgVal.IO_Threshold.Threshold = 0x0133; //1000 A
        //CfgVal.IO_Threshold.Threshold = 0x014C; //1500 A
    }

    //-------------------------------------------------------------------------
    // override normal setting if sim mode and bridge is enabled
    if ( INS(SimMode) && SimModeBrgEnabled )
    {
        if ( !SimModeSdlEnabled )
        {
            CfgVal.Ccfg.DisableBrFlts     = true;  // disable bridge faults for missing hardware
        }
    }

    //-------------------------------------------------------------------------
    // set inner igct lockout time
    CfgVal.DeadTime.Cnt = CalcBilRegister( INS(LockOutTime), 1.0, BilCalc_LockOutTm, CalcBilStatus );
    if ( CalcBilStatus == FAIL )
    {
        status = FAIL;
        MarkStatDiag.LogErrorMessage( LOG_ONLY, cParamString("Init BIL:  %sLockOutTm outside of range supported by hardware", INS(Uniquifier)).c_str(), HERE );
    }


    //-------------------------------------------------------------------------
    // disable firmware faults
    CfgVal.CrBrEnb1.FwFltEnb = false;


    //-------------------------------------------------------------------------
    // Don't disable forced dropout of relays when communication fault occurs
    CfgVal.Ccfg.DisRlyFDO = false;


    //-------------------------------------------------------------------------
    // disable(temporarily) diagnostic packet
    CfgVal.Ccfg.AutoDL = false;

    //-------------------------------------------------------------------------
    // disable/enable SDL faults
    if ( strncmp(INS(Uniquifier), "L", 1) == 0 )
    {
        CfgVal.CrBrEnb1.SdlEnB = 0;
    }
    else
    {
        CfgVal.CrBrEnb1.SdlEnB = 8;
    }

    //-------------------------------------------------------------------------
    // done
    return (status);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Invoke rule calculations.
// EXECUTION LEVEL
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
int cBridgeRebf::RunRules(void)
{
    int status = SUCCESS;

    //-------------------------------------------------------------------------
    // determine the required HSLA site on the REBF
    if      ( (HslaSiteTyp)(INS(HslaSite)) == HSLA_SITE_JA1 )  { Reqd_HslaConn_Str = JA1_Str; }
    else if ( (HslaSiteTyp)(INS(HslaSite)) == HSLA_SITE_JA2 )  { Reqd_HslaConn_Str = JA2_Str; }
    else
    {
        Reqd_HslaConn_Str = NULL_Str;
        MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString("%sRebf Rule: Invalid INS(HslaSite)", INS(Uniquifier)).c_str(), HERE );
    }

    //-------------------------------------------------------------------------
    // determine the required bridge 1 interface card
    if      ( ((BrgInfcHdwTyp)(INS(Brg1InfcHdw))) == BRG_INFC_AEDBH1 )  { Reqd_Brg1Infc_CatNum_Str = Reqd_AEDB_H1_CatNum_Str;  Reqd_Brg1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEDB_H1_CatNum_Str); }
    else if ( ((BrgInfcHdwTyp)(INS(Brg1InfcHdw))) == BRG_INFC_AEDBH2 )  { Reqd_Brg1Infc_CatNum_Str = Reqd_AEDB_H2_CatNum_Str;  Reqd_Brg1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEDB_H2_CatNum_Str); }
    else if ( ((BrgInfcHdwTyp)(INS(Brg1InfcHdw))) == BRG_INFC_AEDBH3 )  { Reqd_Brg1Infc_CatNum_Str = Reqd_AEDB_H3_CatNum_Str;  Reqd_Brg1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEDB_H3_CatNum_Str); }
    else if ( ((BrgInfcHdwTyp)(INS(Brg1InfcHdw))) == BRG_INFC_AEDBH4 )  { Reqd_Brg1Infc_CatNum_Str = Reqd_AEDB_H4_CatNum_Str;  Reqd_Brg1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEDB_H4_CatNum_Str); }
    else if ( ((BrgInfcHdwTyp)(INS(Brg1InfcHdw))) == BRG_INFC_AEBIH1 )  { Reqd_Brg1Infc_CatNum_Str = Reqd_AEBI_H1_CatNum_Str;  Reqd_Brg1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEBI_H1_CatNum_Str); }
    else if ( ((BrgInfcHdwTyp)(INS(Brg1InfcHdw))) == BRG_INFC_AEBIH2 )  { Reqd_Brg1Infc_CatNum_Str = Reqd_AEBI_H2_CatNum_Str;  Reqd_Brg1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEBI_H2_CatNum_Str); }
    else if ( ((BrgInfcHdwTyp)(INS(Brg1InfcHdw))) == BRG_INFC_AEBIH3 )  { Reqd_Brg1Infc_CatNum_Str = Reqd_AEBI_H3_CatNum_Str;  Reqd_Brg1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEBI_H3_CatNum_Str); }
    else if ( ((BrgInfcHdwTyp)(INS(Brg1InfcHdw))) == BRG_INFC_RGIBH1 )  { Reqd_Brg1Infc_CatNum_Str = Reqd_RGIB_H1_CatNum_Str;  Reqd_Brg1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_RGIB_H1_CatNum_Str); }
    else
    {
        Reqd_Brg1Infc_CatNum_Str = &NULL_Str;
        Reqd_Brg1Infc_CatNum_ArrLen = 0;
        if ( ((BrgInfcHdwTyp)(INS(Brg1InfcHdw))) != BRG_INFC_NONE )
        {
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString("%sRebf Rule: Invalid INS(Brg1InfcHdw)", INS(Uniquifier)).c_str(), HERE );
        }
    }

    //-------------------------------------------------------------------------
    // determine the required i/o 1 interface card
    if      ( ((IoInfcHdwTyp)(INS(Io1InfcHdw))) == IO_INFC_AEAAH1 )  { Reqd_Io1Infc_CatNum_Str = Reqd_AEAA_H1_CatNum_Str;  Reqd_Io1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEAA_H1_CatNum_Str); }
    else if ( ((IoInfcHdwTyp)(INS(Io1InfcHdw))) == IO_INFC_AEAAH2 )  { Reqd_Io1Infc_CatNum_Str = Reqd_AEAA_H2_CatNum_Str;  Reqd_Io1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEAA_H2_CatNum_Str); }
    else if ( ((IoInfcHdwTyp)(INS(Io1InfcHdw))) == IO_INFC_AEAAH3 )  { Reqd_Io1Infc_CatNum_Str = Reqd_AEAA_H3_CatNum_Str;  Reqd_Io1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEAA_H3_CatNum_Str); }
    else if ( ((IoInfcHdwTyp)(INS(Io1InfcHdw))) == IO_INFC_AEAAH4 )  { Reqd_Io1Infc_CatNum_Str = Reqd_AEAA_H4_CatNum_Str;  Reqd_Io1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEAA_H4_CatNum_Str); }
    else if ( ((IoInfcHdwTyp)(INS(Io1InfcHdw))) == IO_INFC_AEADH1 )  { Reqd_Io1Infc_CatNum_Str = Reqd_AEAD_H1_CatNum_Str;  Reqd_Io1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEAD_H1_CatNum_Str); }
    else if ( ((IoInfcHdwTyp)(INS(Io1InfcHdw))) == IO_INFC_AEADH2 )  { Reqd_Io1Infc_CatNum_Str = Reqd_AEAD_H2_CatNum_Str;  Reqd_Io1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEAD_H2_CatNum_Str); }
    else if ( ((IoInfcHdwTyp)(INS(Io1InfcHdw))) == IO_INFC_AEADH3 )  { Reqd_Io1Infc_CatNum_Str = Reqd_AEAD_H3_CatNum_Str;  Reqd_Io1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEAD_H3_CatNum_Str); }
    else if ( ((IoInfcHdwTyp)(INS(Io1InfcHdw))) == IO_INFC_AEADH4 )  { Reqd_Io1Infc_CatNum_Str = Reqd_AEAD_H4_CatNum_Str;  Reqd_Io1Infc_CatNum_ArrLen = ARRAY_LEN(Reqd_AEAD_H4_CatNum_Str); }
    else
    {
        Reqd_Io1Infc_CatNum_Str = &NULL_Str;
        Reqd_Io1Infc_CatNum_ArrLen = 0;
        if ( ((IoInfcHdwTyp)(INS(Io1InfcHdw))) != IO_INFC_NONE )
        {
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString("%sRebf Rule: Invalid INS(Io1InfcHdw)", INS(Uniquifier)).c_str(), HERE );
        }
    }

    //-------------------------------------------------------------------------
    // final values to override required hardware
    SET_FV(PwaInvHslaDsb ,  false);
    SET_FV(PwaInvRebfDsb ,  false);

    SET_FV(PwaInvRebfJ1Dsb, false);
    SET_FV(PwaInvRebfJ2Dsb, false);

    SET_FV(PwaInvRebfP1Dsb, false);
    SET_FV(PwaInvRebfP2Dsb, false);

    SET_FV(PwaInvRebhDsb ,   false);
    SET_FV(PwaInvRebhP1Dsb,  false);
    SET_FV(PwaInvRebhP2Dsb,  false);
    SET_FV(PwaInvRebhJ01Dsb, false);
    SET_FV(PwaInvRebhJ02Dsb, false);
    SET_FV(PwaInvRebhJ03Dsb, false);
    SET_FV(PwaInvRebhJ04Dsb, false);
    SET_FV(PwaInvRebhJ06Dsb, false);
    SET_FV(PwaInvRebhJ07Dsb, false);
    SET_FV(PwaInvRebhJ08Dsb, false);

    SET_FV(RgabPresent,      true);
    SET_FV(PhsIocThrs,       4000.0F);

    //-------------------------------------------------------------------------
    // product defined rules
    #include "b0BridgeRebfRule.inc"

    //-------------------------------------------------------------------------
    // feedback offset rules
    IN(Brg1FbkOffs).RunRules(BRG_VCO_FZRO * SclPhsVoltsBrg1, PARM(IfbkFs));

    IN(Io1FbkOffs).RunRules(IO_VCO_FZRO * SclPhsVoltsIo1, IO_VCO_FZRO * SclPhsCurIo1);

    //-------------------------------------------------------------------------
    // calculate and intialize the bridge fpga protection settings
    if ( InitializeBIL() != SUCCESS )
    {
        status = FAIL;
    }

    //-------------------------------------------------------------------------
    // initialize settings for simulation of Vco counters
    BrgSim.SetVcoFzro  ( 0.0F );
    BrgSim.SetVcoFmax  ( (float)VcoFmax );
    BrgSim.SetVcoFmin  ( (float)VcoFmin );
    BrgSim.SetVcoMask  ( VcoMask );
    BrgSim.SetVcoGain  ( 0.0F );
    BrgSim.SetVcoDelTm ( INS(DelTm1) );

        //-------------------------------------------------------------------------
    // initialize settings for simulation of Spd encoder counters
    BrgSim.SetEncMask( 0xFFFF );
    BrgSim.SetEncGain( SimSclSpdEnc );
    BrgSim.SetEncDelTm(INS(DelTm1));

    //-------------------------------------------------------------------------
    //
    PwmFull    = INS(QntPerFrm) * INS(Clk0PerQnt);
    PwmFullRcp = NoNANLim( 1.0F / (float)(INS(QntPerFrm) * INS(Clk0PerQnt)), 1.0e6F, -1.0e6F );
    if( (INS(DelTm3)/INS(DelTm1) + 0.5F) >= (static_cast<int> (INS(DelTm3)/INS(DelTm1)) + 1) )
    {
        T1perT3 = static_cast<int> (INS(DelTm3)/INS(DelTm1)) + 1;
    }
    else
    {
        T1perT3 = static_cast<int> (INS(DelTm3)/INS(DelTm1));
    }

    //-------------------------------------------------------------------------
    // final values to enable LdPls2 alarms
    SET_FV(HsslLdPls2AlmEnb ,  false);

    //-------------------------------------------------------------------------
    // final values to configure hssl late packet test
    SET_FV(HsslTstIntvl  ,      0);   // 0 disables test; set to ~10-100 to test
    SET_FV(HsslTstMrgMax ,  40.0F);
    SET_FV(HsslTstMrgMin , -10.0F);   // -ve margin means sending cmd pkt after
                                      //  the end of the current frame

    //-------------------------------------------------------------------------
    TempVcoCalTm = INS(DelTm1) * (static_cast<float> (INS(TempVcoCalcSt)));


    SET_FV(CtlRlyPwrOkThrs, 0.368);

    SET_FV(RlyPwrOkFilW, 200.0);
    RlyPwrOkFilWdt = 0.00130944 * PARM(RlyPwrOkFilW);
    SET_FV(CtlPwrOkFilW, 200.0);
    CtlPwrOkFilWdt = 0.00130944 * PARM(CtlPwrOkFilW);

    //CtlPwrOkFil = 1.0F;
    //RlyPwrOkFil = 1.0F;


    //-------------------------------------------------------------------------
    // done
    return (status);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Add low level monitor command to support instances of this class
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      Status (success=EOK, fail=-1)
//
///////////////////////////////////////////////////////////////////////////////
int cBridgeRebf::LlmAdd (
    cMonitorCommand&    gMonitor        // reference to global llm class instance
)
{
    cParamString        HelpString;     // temp storage for llm help string

    //-------------------------------------------------------------------------
    // add the "sdl" command to llm
    HelpString  = "\nREBF Bridge Interface information\n";
    HelpString += "  valid command qualifiers:\n";
    HelpString += "    -rid  = dump raw aux board id data\n";
    HelpString += "    -dfw  = dump feedback window\n";
    HelpString += "    -dcw  = dump command window\n";
    HelpString += "    -dfcv = dump fpga config verification data\n";
    HelpString += "    -rst  = reset capture logic\n";
    HelpString += "    -do   = dump cmd/fbk register offset\n";
    HelpString += "  multiple command qualifiers can be entered on single command line\n";
    HelpString += "  all commands qualifiers require a single instance qualifier somewhere on the command line\n";
    HelpString += "     example 1:  rebf -rid L_\n";
    HelpString += "     example 2:  rebf L_ -rid\n";
    gMonitor.Add( "rebf", HelpString, Llm, NORMAL_OUTPUT_SIZE, "rebf");

    return SUCCESS;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Low level monitor command proccessing
//
// EXECUTION LEVEL
//      low level monitor
//
// RETURN VALUE
//      Status (success=EOK, fail=-1)
//
///////////////////////////////////////////////////////////////////////////////
int cBridgeRebf::Llm (
    cParamString&   OutputBuffer,
    cParamString&   CommandLineArgs
)
{
    cSLinkedList <cBridgeRebf*>::cIterator Iter = NULL;
    int             Status = SUCCESS;
    int             CurrentOffset;

    //-------------------------------------------------------------------------
    // search for a valid instance qualifier
    for ( cSLinkedList <cBridgeRebf*>::cIterator IterTmp = BridgeRebfList.Begin() ; IterTmp != BridgeRebfList.End() ; ++IterTmp )
    {
        CurrentOffset = CommandLineArgs.find( (*IterTmp)->Uniq.c_str() );
        if ( CurrentOffset != CommandLineArgs.npos )
        {
            Iter = IterTmp;
            break;
        }
    }
    if ( Iter == NULL )
    {
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("A valid instance qualifier was not found\n");
        OutputBuffer.pappend("  There are %d valid instance qualifiers:", BridgeRebfList.NumElements());
        for ( cSLinkedList <cBridgeRebf*>::cIterator IterTmp = BridgeRebfList.Begin() ; IterTmp != BridgeRebfList.End() ; ++IterTmp )
        {
            OutputBuffer.pappend("  %s", (*IterTmp)->Uniq.c_str());
        }
        OutputBuffer.pappend("\n");
        return FAIL;
    }

    //-------------------------------------------------------------------------
    // list host fpga registers
    OutputBuffer.pappend("\n");
    OutputBuffer.pappend("REBF Bridge Interface %s on Hssl device #%d channel #%d\n", (*Iter)->Uniq.c_str(), (*Iter)->LinkInfo.DeviceId, (*Iter)->LinkInfo.ChannelId);

    //-------------------------------------------------------------------------
    // dump raw aux board id data
    CurrentOffset = CommandLineArgs.find("-rid");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("\n");

        OutputBuffer.pappend("Auxiliary Bus #1 ID Data\n");
        BufferDump( OutputBuffer, (char*)(*Iter)->IdBufAux1, (*Iter)->IdBufAux1DataLen );

        OutputBuffer.pappend("Auxiliary Bus #2 ID Data\n");
        BufferDump( OutputBuffer, (char*)(*Iter)->IdBufAux2, (*Iter)->IdBufAux2DataLen );

        OutputBuffer.pappend("REPI ID Data\n");
        BufferDump( OutputBuffer, (char*)(*Iter)->IdBufRepi, (*Iter)->IdBufRepiDataLen );

        OutputBuffer.pappend("RGIB-A ID Data\n");
        BufferDump( OutputBuffer, (char*)(*Iter)->IdBufRgib, (*Iter)->IdBufRgibDataLen );

        OutputBuffer.pappend("RGIB-B ID Data\n");
        BufferDump( OutputBuffer, (char*)(*Iter)->IdBufRgib2, (*Iter)->IdBufRgib2DataLen );

        OutputBuffer.pappend("RGIB-C ID Data\n");
        BufferDump( OutputBuffer, (char*)(*Iter)->IdBufRgib3, (*Iter)->IdBufRgib3DataLen );

        OutputBuffer.pappend("RGIB-DB ID Data\n");
        BufferDump( OutputBuffer, (char*)(*Iter)->IdBufRgib4, (*Iter)->IdBufRgib4DataLen );

        return (Status);
    }

    //-------------------------------------------------------------------------
    // dump hssl feedback window
    CurrentOffset = CommandLineArgs.find("-dfw");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        (*Iter)->LlmFbkShdwCapReq = true;
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("  dump feedback window\n");
        while ( (*Iter)->LlmFbkShdwCapReq ) { }
        for ( unsigned i = 0 ; i < sizeof((*Iter)->LlmFbkShdw)/4 ; ++i )
        {
            OutputBuffer.pappend("    0x%08x    %s\n", *(((unsigned*)&(*Iter)->LlmFbkShdw)+i), FbkWinRegNames[i] );
        }
        OutputBuffer.pappend("\n");
        return (Status);
    }

    //-------------------------------------------------------------------------
    // dump hssl command window
    CurrentOffset = CommandLineArgs.find("-dcw");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        (*Iter)->LlmCmdShdwCapReq = true;
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("  dump command window\n");
        while ( (*Iter)->LlmCmdShdwCapReq ) { }
        for ( unsigned i = 0 ; i < sizeof((*Iter)->LlmCmdShdw)/4 ; ++i )
        {
            OutputBuffer.pappend("    0x%08x    %s\n", *(((unsigned*)&(*Iter)->LlmCmdShdw)+i), CmdWinRegNames[i] );
        }
        OutputBuffer.pappend("\n");
        return (Status);
    }

    //-------------------------------------------------------------------------
    // dump fpga config verification data
    CurrentOffset = CommandLineArgs.find("-dfcv");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        (*Iter)->LlmCmdShdwCapReq = true;
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("  dump fpga config verification data\n");
        OutputBuffer.pappend("    Index  Config  Rdback\n" );
        for ( unsigned i = 0 ; i < REBx_NUM_OF_CONFIG_REGS ; ++i )
        {
            OutputBuffer.pappend("        %d    %4d    %4d\n", i, (*Iter)->CfgVal.Val[i], (*Iter)->CfgReadBack[i] );
        }
        OutputBuffer.pappend("\n");
        return (Status);
    }
    //-------------------------------------------------------------------------
    // reset capture logic
    CurrentOffset = CommandLineArgs.find("-rst");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        (*Iter)->HsslFbkWaitMin = 1.0;
        (*Iter)->HsslFbkWaitMax = 0.0;
        OutputBuffer.pappend("\n");
        OutputBuffer.pappend("  capture logic has been reset\n");
        return (Status);
    }

    //-------------------------------------------------------------------------
    // dump cmd and fbk window register offsets
    CurrentOffset = CommandLineArgs.find("-do");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("sizeof FbkWinTyp = %d\n",  sizeof(REBx_FbkWinTyp));
        OutputBuffer.pappend("    offsetof           MiscStat = 0x%08x\n", offsetof(REBx_FbkWinTyp, MiscStat));
        OutputBuffer.pappend("    offsetof           PktCntRb = 0x%08x\n", offsetof(REBx_FbkWinTyp, PktCntRb));
        OutputBuffer.pappend("    offsetof           SDL1     = 0x%08x\n", offsetof(REBx_FbkWinTyp, SDL21_ST));
        OutputBuffer.pappend("    offsetof               CrLb = 0x%08x\n", offsetof(REBx_FbkWinTyp, CrLb));
        OutputBuffer.pappend("sizeof CmdWinTyp = %d\n",  sizeof(REBx_CmdWinTyp));
        OutputBuffer.pappend("    offsetof  PhsGateEvtRegArr1 = 0x%08x\n", offsetof(REBx_CmdWinTyp, PhsGateEvtRegArr1));
        OutputBuffer.pappend("    offsetof  PhsGateEvtRegArr3 = 0x%08x\n", offsetof(REBx_CmdWinTyp, PhsGateEvtRegArr3));
        OutputBuffer.pappend("    offsetof             FanRef = 0x%08x\n", offsetof(REBx_CmdWinTyp, FanRef));
        OutputBuffer.pappend("\n");
        return (Status);
    }

    //-------------------------------------------------------------------------
    // hssl info
    OutputBuffer.pappend("\n");
    OutputBuffer.pappend("HSSL INFO\n");
    OutputBuffer.pappend("\n");

    OutputBuffer.pappend("  Hssl Cmd packet size     = %d bytes\n", sizeof(REBx_CmdWinTyp));
    OutputBuffer.pappend("  Hssl Cmd packet tx time  = %f us\n", 1.84 + 0.4 + (163.84 * sizeof(REBx_CmdWinTyp) / 2048));
    OutputBuffer.pappend("\n");

    OutputBuffer.pappend("  Hssl Fbk packet size     = %d bytes\n", sizeof(REBx_FbkWinTyp));
    OutputBuffer.pappend("  Hssl Fbk packet tx time  = %f us\n", 1.84 + 0.4 + (163.84 * sizeof(REBx_FbkWinTyp) / 2048));
    OutputBuffer.pappend("\n");

    OutputBuffer.pappend("  Hssl Diag packet size    = %d bytes\n", sizeof(REBx_DiagWinTyp));
    OutputBuffer.pappend("  Hssl Diag packet tx time = %f us\n", 1.84 + 0.4 + (163.84 * sizeof(REBx_DiagWinTyp) / 2048));
    OutputBuffer.pappend("\n");

    //-------------------------------------------------------------------------
    // feedback scaling
    OutputBuffer.pappend("\n");
    OutputBuffer.pappend("FEEDBACK SCALING\n");
    OutputBuffer.pappend("\n");

    if ( (*Iter)->Ins.Brg1InfcHdw != BRG_INFC_NONE )
    {
        OutputBuffer.pappend("  Brg 1            Current   AC Voltage   DC Voltage\n");
        OutputBuffer.pappend("    Full Scale = %9.3f    %9.3f    %9.3f\n", BRG_VCO_FZRO * (*Iter)->SclPhsCurBrg1  , 2.0F * BRG_VCO_FZRO * (*Iter)->SclPhsVoltsBrg1  , BRG_VCO_FZRO * (*Iter)->SclLinkVoltsBrg1   );
        OutputBuffer.pappend("    Resolution = %9.3f    %9.3f    %9.3f\n", (*Iter)->SclPhsCurBrg1 * (*Iter)->Ins.DelTm1Rcp, (*Iter)->SclPhsVoltsBrg1 * (*Iter)->Ins.DelTm1Rcp, (*Iter)->SclLinkVoltsBrg1 * (*Iter)->Ins.DelTm1Rcp );
        OutputBuffer.pappend("\n");
    }

    if ( (*Iter)->Ins.Io1InfcHdw != IO_INFC_NONE )
    {
        OutputBuffer.pappend("  Io 1             Current   AC Voltage\n");
        OutputBuffer.pappend("    Full Scale = %9.3f    %9.3f\n", IO_VCO_FZRO * (*Iter)->SclPhsCurIo1   , IO_VCO_FZRO * (*Iter)->SclPhsVoltsIo1    );
        OutputBuffer.pappend("    Resolution = %9.3f    %9.3f\n", (*Iter)->SclPhsCurIo1 * (*Iter)->Ins.DelTm1Rcp, (*Iter)->SclPhsVoltsIo1 * (*Iter)->Ins.DelTm1Rcp );
        OutputBuffer.pappend("\n");
    }

    //-------------------------------------------------------------------------
    // feedback offset compensation
    OutputBuffer.pappend("\n");
    OutputBuffer.pappend("FEEDBACK OFFSET COMPENSATION\n");
    OutputBuffer.pappend("\n");

    if ( (*Iter)->Ins.Brg1InfcHdw != BRG_INFC_NONE )
    {
        OutputBuffer.pappend("  Brg 1                Ia          Ib          Ic         Vax         Vbx\n" );
        OutputBuffer.pappend("    HdwInOffs = %9.3f   %9.3f   %9.3f   %9.3f   %9.3f\n", (*Iter)->Brg1HdwInOffs.Ia, (*Iter)->Brg1HdwInOffs.Ib, (*Iter)->Brg1HdwInOffs.Ic, (*Iter)->Brg1HdwInOffs.Vax, (*Iter)->Brg1HdwInOffs.Vbx );
        OutputBuffer.pappend("    HdwOffs   = %9.6f   %9.6f   %9.6f   %9.6f   %9.6f\n", (*Iter)->Brg1HdwOffs.Ia  , (*Iter)->Brg1HdwOffs.Ib  , (*Iter)->Brg1HdwOffs.Ic  , (*Iter)->Brg1HdwOffs.Vax  , (*Iter)->Brg1HdwOffs.Vbx   );
        OutputBuffer.pappend("\n");
    }

    if ( (*Iter)->Ins.Io1InfcHdw != IO_INFC_NONE )
    {
        OutputBuffer.pappend("  Io 1                 Ia          Ib          Ic         Vax         Vbx\n" );
        OutputBuffer.pappend("    HdwInOffs = %9.3f   %9.3f   %9.3f   %9.3f   %9.3f\n", (*Iter)->Io1HdwInOffs.Ia, (*Iter)->Io1HdwInOffs.Ib, (*Iter)->Io1HdwInOffs.Ic, (*Iter)->Io1HdwInOffs.Vax, (*Iter)->Io1HdwInOffs.Vbx );
        OutputBuffer.pappend("    HdwOffs   = %9.6f   %9.6f   %9.6f   %9.6f   %9.6f\n", (*Iter)->Io1HdwOffs.Ia  , (*Iter)->Io1HdwOffs.Ib  , (*Iter)->Io1HdwOffs.Ic  , (*Iter)->Io1HdwOffs.Vax  , (*Iter)->Io1HdwOffs.Vbx   );
        OutputBuffer.pappend("\n");
    }

    //-------------------------------------------------------------------------
    // control & protection status
    OutputBuffer.pappend("\n");
    OutputBuffer.pappend("CONTROL & PROTECTION STATUS\n");
    OutputBuffer.pappend("\n");

    OutputBuffer.pappend("  PktCnt            = 0x%08x\n", (*Iter)->BrgCmd.PktCnt );
    OutputBuffer.pappend("  CrCfg             = 0x%08x\n", (*Iter)->BrgCmd.CrCfg );
    OutputBuffer.pappend("  LoadPulseTimeReg  = 0x%08x\n", (*Iter)->BrgCmd.LoadPulseTimeReg );
    OutputBuffer.pappend("  Cr21              = 0x%08x\n", (*Iter)->BrgCmd.Cr21 );
    OutputBuffer.pappend("\n");

    OutputBuffer.pappend("  Brg1TransList.Len     = %d\n", (*Iter)->Brg1TransList.Len );
    OutputBuffer.pappend("  BrgCmd.GateEvtArr1Len = %d\n", REBx_NUM_OF_GATE_CMD_EVTS );
    OutputBuffer.pappend("\n");
#if 0
    OutputBuffer.pappend("    GateEvt101              \n");
    for ( unsigned i = 0 ; i < REBx_NUM_OF_GATE_CMD_EVTS ; i++ )
    {
        OutputBuffer.pappend("    Event %d\n",i+1);
        OutputBuffer.pappend("      Time       %d\n", (*Iter)->BrgCmd.GateEvtArr1[i].Time);
        OutputBuffer.pappend("      State A    %d\n", (*Iter)->BrgCmd.GateEvtArr1[i].StateA);
        OutputBuffer.pappend("      State B    %d\n", (*Iter)->BrgCmd.GateEvtArr1[i].StateB);
        OutputBuffer.pappend("      State C    %d\n", (*Iter)->BrgCmd.GateEvtArr1[i].StateC);
        OutputBuffer.pappend("      Enable     %d\n", (*Iter)->BrgCmd.GateEvtArr1[i].Enable);
    }
#endif

#if 0
    OutputBuffer.pappend("    GateEvt201              \n");
    for ( unsigned i = 0 ; i < REBx_NUM_OF_GATE_CMD_EVTS ; i++ )
    {
        OutputBuffer.pappend("    Event %d\n",i+1);
        OutputBuffer.pappend("      Time       %d\n", (*Iter)->BrgCmd.GateEvtArr2[i].Time);
        OutputBuffer.pappend("      State A    %d\n", (*Iter)->BrgCmd.GateEvtArr2[i].StateA);
        OutputBuffer.pappend("      State B    %d\n", (*Iter)->BrgCmd.GateEvtArr2[i].StateB);
        OutputBuffer.pappend("      State C    %d\n", (*Iter)->BrgCmd.GateEvtArr2[i].StateC);
        OutputBuffer.pappend("      Enable     %d\n", (*Iter)->BrgCmd.GateEvtArr2[i].Enable);
    }
#endif

    //-------------------------------------------------------------------------
    // done
    return SUCCESS;
}
