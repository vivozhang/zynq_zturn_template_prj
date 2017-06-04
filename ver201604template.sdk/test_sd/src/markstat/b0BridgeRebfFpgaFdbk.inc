//
//   Generated with parser version 1.09
//   WARNING:  Do NOT manually modify; update input file or parser code only.
//
#ifndef INC_MAP_REBFB_00_HSLA_FDBK_PKT_H
#define INC_MAP_REBFB_00_HSLA_FDBK_PKT_H

// Register offset:
//-------------------
#define REBF_RevCode_offset             0x0000
#define REBF_VcoTemp21_offset           0x0004
#define REBF_VcoBr1_offset              0x0008
#define REBF_StatCom_offset             0x0014
#define REBF_BrStatFlt_offset           0x0018
#define REBF_VcoIo1_offset              0x001c
#define REBF_VcoVdc_offset              0x002c
#define REBF_StatReg21_offset           0x0030
#define REBF_TempRegEngUseOnly_offset   0x0034
#define REBF_Tach01_offset              0x0038
#define REBF_CrLb_offset                0x0040
#define REBF_MiscStat_offset            0x0044
#define REBF_PktCntRb_offset            0x0048

// Types and Classes:
//-------------------
//-------------------------------------------------------------------------------------------------
struct REBF_RevCodeTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Rfd                     :4 ;  // [0 : 3]    : Revision for the day
    unsigned        Proc                    :4 ;  // [4 : 7]    : Product code: 0=AEGI#A, 1=AEGI#B, 2=REBI#A,4=X030#DwAEAD, 5=REBFA,6=REBFB
    unsigned        YyMmDd                  :24;  // [8 : 31]   : FPGA HDL revision date: YY=last 2 digits of year, MM =month, DD = day.
};

//-------------------------------------------------------------------------------------------------
struct REBF_VcoTemp21Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        BrgCellTemp             :16;  // [0 : 15]   : VCO counter for bridge# cell temperatures
    unsigned        spare                   :16;  // [16 : 31]  : spares
};

//-------------------------------------------------------------------------------------------------
struct REBF_VcoBr1Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        Ia                      :16;  // [0 : 15]   : VCO - Phase A current
    unsigned        Ib                      :16;  // [16 : 31]  : VCO - Phase B current
    unsigned        Ic                      :16;  // [32 : 47]  : VCO - Phase C current
    unsigned        Vab                     :16;  // [48 : 63]  : VCO - Line-to-line voltage A-B
    unsigned        Vbc                     :16;  // [64 : 79]  : VCO - Line-to-line voltage B-C
    unsigned        Vdc                     :16;  // [80 : 95]  : VCO - DC Link voltage if AEBI, DB voltage if AEAD
};

//-------------------------------------------------------------------------------------------------
struct REBF_StatComTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        PeEnb                   :1 ;  // [0]        : Result of BrgEnable register. 1=enable bridge (to accept gate cmds. 0=ignore gate cmds
    unsigned        DriveFlt                :1 ;  // [1]        : 1= drive shutdown due to a fault. 0 = no faults are disabling the drive
    unsigned        FrmErr                  :1 ;  // [2]        : 1 if >2x LdPls2FrcDel since last cntrl packet - clr'ed by FLTRST_REQ
    unsigned        DrPc                    :1 ;  // [3]        : 1= enabled P5 gate logic power to bridge
    unsigned        spare1                  :1 ;  // [4]        : spare
    unsigned        DrvP5                   :1 ;  // [5]        : 1= supply P5 power to gate control logic
    unsigned        spare2                  :1 ;  // [6]        : spare
    unsigned        BrlRxFlt                :1 ;  // [7]        : 1=Bridge-Ready-Loop Rx input inactive.
    unsigned        spare3                  :1 ;  // [8]        : spare
    unsigned        LdPls2Evt               :1 ;  // [9]        : 1=LDPLS2 event occurred - clr'ed when send feedback pkt
    unsigned        FrcFlt                  :1 ;  // [10]       : FRC_ERRR (ABS(target-host)>=320nsec
    unsigned        LdPlsAlm                :1 ;  // [11]       : 1= LPPLS_TIME < FRC time - clr'ed when send feedback pkt
    unsigned        TempVcoSel              :3 ;  // [12 : 14]  : indicates which temperature sensors shown in VcoTemp21
    unsigned        reserved1               :1 ;  // [15]
    unsigned        Ambient                 :11;  // [16 : 26]  : HSLA temperature. 11-bit bi-polar. LSb=0.25 deg C
    unsigned        reserved2               :5 ;  // [27 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_BrStatFltTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        AnegB1                  :1 ;  // [0]        : 1= phase A lower leg cell Desat fault
    unsigned        BnegB1                  :1 ;  // [1]        : 1= phase B lower leg cell Desat fault
    unsigned        CnegB1                  :1 ;  // [2]        : 1= phase C lower leg cell Desat fault
    unsigned        AposB1                  :1 ;  // [3]        : 1= phase A upper leg cell Desat fault
    unsigned        BposB1                  :1 ;  // [4]        : 1= phase B upper leg cell Desat fault
    unsigned        CposB1                  :1 ;  // [5]        : 1= phase C upper leg cell Desat fault
    unsigned        AiocB1                  :1 ;  // [6]        : 1= phase A overcurrent
    unsigned        BiocB1                  :1 ;  // [7]        : 1= phase B overcurrent
    unsigned        CiocB1                  :1 ;  // [8]        : 1= phase C overcurrent
    unsigned        DdesatB1                :1 ;  // [9]        : 1= phase D upper or lower leg cell Desat fault
    unsigned        DiocB1                  :1 ;  // [10]       : 1= phase D overcurrent
    unsigned        FwFltB1                 :1 ;  // [11]       : 1= Software Forced a bridge fault
    unsigned        Cin109RtrFlt            :1 ;  // [12]       : 1 = ContInb109 inactive >350msec.
    unsigned        Rly101RtrFlt            :1 ;  // [13]       : always 0 - disabled.
    unsigned        spare00                 :1 ;  // [14]       : unused.
    unsigned        FltBr1                  :1 ;  // [15]       : 1 = Bridge fault. 'OR' of [14:0] below that are enabled.
    unsigned        spare01                 :16;  // [16 : 31]  : unused.
};

//-------------------------------------------------------------------------------------------------
struct REBF_VcoIo1Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        A02                     :16;  // [0 : 15]   : VCO - IO# analog #2 - BN(AEAD) or BC(AEAA)
    unsigned        A03                     :16;  // [16 : 31]  : VCO - IO# analog #3 - CN(AEAD) or CA(AEAA)
    unsigned        A04                     :16;  // [32 : 47]  : VCO - IO# analog #4 - NotUsed(AEAD) or Netural-to-earth(AEAA)
    unsigned        A05                     :16;  // [48 : 63]  : VCO - IO# analog #5 - Phase A current
    unsigned        A06                     :16;  // [64 : 79]  : VCO - IO# analog #6 - Phase B current
    unsigned        A07                     :16;  // [80 : 95]  : VCO - IO# analog #7 - Phase C current
    unsigned        A01                     :16;  // [96 : 111] : VCO - IO# analog #1 - AN(AEAD) or AB(AEAA)
    unsigned        spare01                 :16;  // [112 : 127]: unused.
};

//-------------------------------------------------------------------------------------------------
struct REBF_VcoVdcTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        VCO_107                 :16;  // [0 : 15]   : VCO - Bridge 1 Phase current D -only active for BatteryController units
    unsigned        Vdcx                    :16;  // [16 : 31]  : VCO - DC Link voltage from other bridge (AEDB/AEBI)
};

//-------------------------------------------------------------------------------------------------
struct REBF_StatReg21Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        REBFpwrOk               :1 ;  // [0]        : REBF P5V power status, 1=ok{L2#}
    unsigned        ContInb                 :10;  // [1 : 10]   : IO#1 - 10 contact inputs: 0= active. Note: software must immediately shut down drive if ContIn 09 or 10 become inactive (go HI).
    unsigned        BrlTx                   :1 ;  // [11]       : BRL transmit status, 1=ok{L2#}
    unsigned        BrlRxb                  :1 ;  // [12]       : BRL recieve status, 0=ok{L2#}
    unsigned        HslaPwrOk               :1 ;  // [13]       : 1 = HSLA power ok{L2#}
    unsigned        CtlPwrOk1               :1 ;  // [14]       : IO#1 - control power, 1=ok
    unsigned        RlyPwrOk                :1 ;  // [15]       : IO#1 - relay power, 1=ok
    unsigned        IamHSLA1                :1 ;  // [16]       : 1=pin identified as HSLA#1, 0=identified as HSLA#2
    unsigned        reserved24              :9 ;  // [17 : 25]  : unused
    unsigned        BrlRx                   :1 ;  // [26]       : BRL recieve status, 1=ok}
    unsigned        LBrl                    :1 ;  // [27]       : local BRL recieve status, 1=ok}
    unsigned        ClkBad                  :1 ;  // [28]       : 1=FPGA clk fault. Likely only seen for intermittant detections.
    unsigned        DIn01                   :1 ;  // [29]       : 0=10-24Vdc applied to TB3.1->2. 1= <10V input.{L#1}
    unsigned        P19OK                   :1 ;  // [30]       : 1= P19 supply >12V = OK
    unsigned        PFO                     :1 ;  // [31]       : not used
};

//-------------------------------------------------------------------------------------------------
struct REBF_TempRegEngUseOnlyTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        GateB1b                 :6 ;  // [0 : 5]    : output to bridge card for BRG#1 - active low
    unsigned        reserved21              :2 ;  // [6 : 7]    : unused
    unsigned        reserved22              :8 ;  // [8 : 15]   : unused
    unsigned        gState1                 :6 ;  // [16 : 21]  : output of gating scheduler for BRG#1
    unsigned        reserved23              :2 ;  // [22 : 23]  : unused
    unsigned        reserved24              :8 ;  // [24 : 31]  : unused
};

//-------------------------------------------------------------------------------------------------
struct REBF_Tach01Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        TachT                   :10;  // [0 : 9]    : Rollover counter of time. 1count = 1usec
    unsigned        TachD                   :6 ;  // [10 : 15]  : Discrete inputs from tach M,/M,B,/B,A,/A = [15:10]
    unsigned        TachV                   :16;  // [16 : 31]  : Rollover up/down counter. Counts up/down for positive (A then B edge)/negative (B then A edge) rotation. Each count = 1 pulse. Can be used to calculate speen when time between samples & # of pulses per revolutions is known.
    unsigned        TachM                   :16;  // [32 : 47]  : Capture of TachV at the RE of marker pulse Combined with TachV to make positional calculations.
    unsigned        TachP                   :16;  // [48 : 63]  : Rollover counter. Counts the time between A & B tach pulses.  1 count = 1usec.  Cleared on each A or B edge.
};

//-------------------------------------------------------------------------------------------------
struct REBF_CrLbTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        VcoT03                  :16;  // [0 : 15]   : VCO counter for AEAD thermister temperatures
    unsigned        LbReg                   :16;  // [16 : 31]  : Contents of register pointed to by CR_CFG.CFG_SEL.
};

//-------------------------------------------------------------------------------------------------
struct REBF_MiscStatTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        RxErr                   :8 ;  // [0 : 7]    : Count # of receive packets that had bad CRC
    unsigned        TxErr                   :8 ;  // [8 : 15]   : Count # of transmit packets that receive NAK for
    unsigned        PktIgnErr               :8 ;  // [16 : 23]  : Count # of receive packets that were ignored
    unsigned        reserved1               :4 ;  // [24 : 27]  : unused
    unsigned        InPwrOk                 :1 ;  // [28]       : Input source to HSLA >21.8Vdc
    unsigned        CfgEnabled              :1 ;  // [29]       : 1=CFG_ENABLE bits sequenced correctly to allow changes to configuration registers
    unsigned        reserved2               :2 ;  // [30 : 31]  : unused
};

//-------------------------------------------------------------------------------------------------
struct REBF_PktCntRbTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Cnt                     :32;  // [0 : 31]   : Loopback of value from control pkt.
};


#endif

