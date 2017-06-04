//
//   Generated with parser version 1.09
//   WARNING:  Do NOT manually modify; update input file or parser code only.
//
#ifndef INC_MAP_REBX_00_HSLA_CFG_SEL_REGS_H
#define INC_MAP_REBX_00_HSLA_CFG_SEL_REGS_H

// Register offset:
//-------------------
#define REBx_DiagMuxSel_offset          0x0000
#define REBx_Ccfg_offset                0x0001
#define REBx_CrBrEnb1_offset            0x0002
#define REBxB_spareLocX006_offset       0x0003
#define REBx_DeadTime_offset            0x0004
#define REBx_DbOn_offset                0x0005
#define REBx_CrBrF1_offset              0x0006
#define IO_Threshold_offset             0x0007
#define REPI_BrdId_offset               0x01e0
#define RGIB_BrdId_offset               0x01e2
#define REBx_AuxBrdId1_offset           0x01fe

// Types and Classes:
//-------------------
//-------------------------------------------------------------------------------------------------
struct REBx_DiagMuxSelTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Diag1                   :4 ;  // [0 : 3]    : selects FPGA signal to output to Diag1 testpoint.
    unsigned        spare01                 :4 ;  // [4 : 7]
    unsigned        Diag2                   :4 ;  // [8 : 11]   : selects FPGA signal to output to Diag2 testpoint.
    unsigned        spare02                 :4 ;  // [12 : 15]
};

//-------------------------------------------------------------------------------------------------
struct REBx_CcfgTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        FltToWsEnab             :1 ;  // [0]        : Set to enable FRM_ERR to forces target to SyncWait State
    unsigned        DisableBrFlts           :1 ;  // [1]        : 1= disable bridge faults (TBD) from turning off drive.
    unsigned        DsbFrmErr               :1 ;  // [2]        : 1= disable FRM_ERR fault (&FRC_FLT & LDPLS_FLT). For engineering use only
    unsigned        SelAEAA                 :1 ;  // [3]        : 0=IO connection is to an AEAD, 1= to an AEAA. (relay control differs.)
    unsigned        BR1                     :1 ;  // [4]        : 1= using a REBx_BBR1 revision.
    unsigned        DisRlyFDO               :1 ;  // [5]        : 1=disable forced dropout of relays when SR_C[2] set.
    unsigned        spare02                 :2 ;  // [6 : 7]
    unsigned        TriGates                :1 ;  // [8]        : 0= tri-state the Bridge side gate outputs
    unsigned        BRLdis                  :1 ;  // [9]        : 1=disable the BRL RX input.  Used when Link#2 not used.
    unsigned        LineSide                :1 ;  // [10]       : 1= select behavior unique to line-side REBx.---not used.
    unsigned        BatCntl                 :1 ;  // [11]       : 0=BRG#1(AEDB) Phase D is normal DB control (same as BRG#2). 1=Phase D used for Battery Controller.
    unsigned        spare03                 :3 ;  // [12 : 14]
    unsigned        AutoDL                  :1 ;  // [15]       : Set to automatically send DL after feedback pkt
};

//-------------------------------------------------------------------------------------------------
struct REBx_CrBrEnb1Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        SdlEnB                  :4 ;  // [0 : 3]    : 0=enable SDL[4:1] fault, 1=disable. Only use when link not used!
    unsigned        spare01                 :5 ;  // [4 : 8]
    unsigned        DdesatEnb               :1 ;  // [9]        : 1= enable DB cell desat fault
    unsigned        spare02                 :1 ;  // [10]
    unsigned        FwFltEnb                :1 ;  // [11]       : 1= enable software fault
    unsigned        spare03                 :1 ;  // [12]
    unsigned        DdesatSDb               :1 ;  // [13]       : 0= enable PH-D desat fault disables PH-D gating immediately
    unsigned        DDisPeEn                :1 ;  // [14]       : 1= disable PE_EN as enable for Phase D (chopper mode).
    unsigned        reserved1               :1 ;  // [15]
};

//-------------------------------------------------------------------------------------------------
struct REBxB_spareLocX006Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        spare                   :16;  // [0 : 15]   : Required for Software which needs continous mapping
};

//-------------------------------------------------------------------------------------------------
struct REBx_DeadTimeTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Cnt                     :9 ;  // [0 : 8]    : Sets deadtime between when 1 cell is turned off & other turned on in same phase. 1cnt = 40ns
    unsigned        reserved1               :7 ;  // [9 : 15]
};

//-------------------------------------------------------------------------------------------------
struct REBx_DbOnTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Threshold               :9 ;  // [0 : 8]    : DC LINK voltage at which DB turns on. 0x1ff = max positive
    unsigned        spare01                 :6 ;  // [9 : 14]
    unsigned        Enb                     :1 ;  // [15]       : 1=enable FPGA controlled DB. software control still active.
};

//-------------------------------------------------------------------------------------------------
struct REBx_CrBrF1Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        AnegF                   :1 ;  // [0]        : 1= force Phase A lower cell desat fault
    unsigned        BnegF                   :1 ;  // [1]        : 1= force Phase B lower cell desat fault
    unsigned        CnegF                   :1 ;  // [2]        : 1= force Phase C lower cell desat fault
    unsigned        AposF                   :1 ;  // [3]        : 1= force Phase A upper cell desat fault
    unsigned        BposF                   :1 ;  // [4]        : 1= force Phase B upper cell desat fault
    unsigned        CposF                   :1 ;  // [5]        : 1= force Phase C upper cell desat fault
    unsigned        AiocF                   :1 ;  // [6]        : 1= force Phase A IOC fault
    unsigned        BiocF                   :1 ;  // [7]        : 1= force Phase B IOC fault
    unsigned        CiocF                   :1 ;  // [8]        : 1= force Phase C IOC fault
    unsigned        DdesatF                 :1 ;  // [9]        : 1= force DB cell desat fault
    unsigned        DiocF                   :1 ;  // [10]       : 1= force DB IOC fault
    unsigned        FwFltF                  :1 ;  // [11]       : 1= force software fault
    unsigned        spare                   :4 ;  // [12 : 15]
};

//-------------------------------------------------------------------------------------------------
struct IO_ThresholdTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Threshold               :9 ;  // [0 : 8]    : Phase current level at which IOC turns on. (0x0->0x1ff = 0to max psoitve = mid to max SD count.
    unsigned        spare                   :7 ;  // [9 : 15]   : not used
};

//-------------------------------------------------------------------------------------------------
struct REPI_BrdIdTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Addr                    :16;  // [0 : 15]   : Rd Addr of the REPI BRD ID chip buffer: 0 ->1023 words. Value returned in CrLb. Addr 0 holds buffer size.
};

//-------------------------------------------------------------------------------------------------
struct RGIB_BrdIdTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Addr                    :16;  // [0 : 15]   : Rd Addr of the REPI BRD ID chip buffer: 0 ->1023 words. Value returned in CrLb. Addr 0 holds buffer size.
};

//-------------------------------------------------------------------------------------------------
struct REBx_AuxBrdId1Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        Addr                    :16;  // [0 : 15]   : Rd Addr of the Aux BRD ID chip buffer: 0 ->1023 words. Value returned in CrLb. Addr 0 holds buffer size.
};


#endif

