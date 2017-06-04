//
//   Generated with parser version 1.09
//   WARNING:  Do NOT manually modify; update input file or parser code only.
//
#ifndef INC_MAP_REBX_00_HSLA_FDBK_PKT_H
#define INC_MAP_REBX_00_HSLA_FDBK_PKT_H

// Register offset:
//-------------------
#define REBx_RevCode_offset             0x0000
#define REBx_VcoTemp21_offset           0x0004
#define REBx_VcoBr1_offset              0x0008
#define REBx_StatCom_offset             0x0014
#define REBx_BrStatFlt_offset           0x0018
#define REBx_VcoIo1_offset              0x001c
#define REBx_VcoVdb_offset              0x002c
#define REBx_StatReg21_offset           0x0030
#define REBx_TempRegEngUseOnly_offset   0x0034
#define REBx_Tach01_offset              0x0038
#define REBx_CrLb_offset                0x0040
#define REBx_MiscStat_offset            0x0044
#define REBx_SDL21_ST_offset            0x0048
#define REBx_SDL43_ST_offset            0x004c
#define REBx_VdcSDL5_ST_offset          0x0050
#define REBx_PktCntRb_offset            0x0054
#define subStructure_SDL_ST_offset      0x0058

// Types and Classes:
//-------------------
//-------------------------------------------------------------------------------------------------
struct REBx_RevCodeTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Rfd                     :4 ;  // [0 : 3]    : Revision for the day
    unsigned        Proc                    :4 ;  // [4 : 7]    : Product code: 0=AEGI#A, 1=AEGI#B, 2=REBI#A,4=X030#DwAEAD, 5=REBF_A,6=REBF_B,7=REBH_A
    unsigned        YyMmDd                  :24;  // [8 : 31]   : FPGA HDL revision date: YY=last 2 digits of year, MM =month, DD = day.
};

//-------------------------------------------------------------------------------------------------
struct REBx_VcoTemp21Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        Brg1CellTemp            :16;  // [0 : 15]   : counter for bridge# cell temperatures
    unsigned        Brg2CellTemp            :16;  // [16 : 31]  : counter for bridge# cell temperatures
};

//-------------------------------------------------------------------------------------------------
struct REBx_VcoBr1Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        Ia                      :16;  // [0 : 15]   : VCO - Phase A current
    unsigned        Ib                      :16;  // [16 : 31]  : VCO - Phase B current
    unsigned        Ic                      :16;  // [32 : 47]  : VCO - Phase C current
    unsigned        Vab                     :16;  // [48 : 63]  : VCO - Line-to-line voltage A-B
    unsigned        Vbc                     :16;  // [64 : 79]  : VCO - Line-to-line voltage B-C
    unsigned        Vdc                     :16;  // [80 : 95]  : VCO - DC Link voltage
};

//-------------------------------------------------------------------------------------------------
struct REBx_StatComTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        PeEnb                   :1 ;  // [0]        : Result of BrgEnable register. 1=enable bridge (to accept gate cmds. 0=ignore gate cmds
    unsigned        DriveFlt                :1 ;  // [1]        : 1= drive shutdown due to a fault. 0 = no faults are disabling the drive
    unsigned        FrmErr                  :1 ;  // [2]        : 1 if >2x LdPls2FrcDel since last cntrl packet - clr'ed by FLTRST_REQ
    unsigned        DrPc                    :1 ;  // [3]        : 1= enabled P5 gate logic power to bridge
    unsigned        spare1                  :1 ;  // [4]        : spare
    unsigned        DrvP5                   :1 ;  // [5]        : 1= supply P5 power to gate control logic
    unsigned        spare6                  :1 ;  // [6]        : spare
    unsigned        BrlRxFlt                :1 ;  // [7]        : 1=Bridge-Ready-Loop Rx input inactive.
    unsigned        DlydFrmErr              :1 ;  // [8]        : Latched Timeout for allowing forcing K3&K4 off
    unsigned        LdPls2Evt               :1 ;  // [9]        : 1=LDPLS2 event occurred - clr'ed when send feedback pkt
    unsigned        FrcFlt                  :1 ;  // [10]       : FRC_ERRR (ABS(target-host)>=320nsec
    unsigned        LdPlsAlm                :1 ;  // [11]       : 1= LPPLS_TIME < FRC time - clr'ed when send feedback pkt
    unsigned        TempVcoSel              :3 ;  // [12 : 14]  : indicates which temperature sensors shown in VcoTemp21
    unsigned        reserved1               :1 ;  // [15]
    unsigned        Ambient                 :11;  // [16 : 26]  : HSLA temperature. 11-bit bi-polar. LSb=0.25 deg C
    unsigned        reserved2               :5 ;  // [27 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBx_BrStatFltTyp
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
    unsigned        SDLerr                  :1 ;  // [14]       : 1= 1 or more SDL have error – see RGIB0#_HS_ST for which.
    unsigned        FltBr1                  :1 ;  // [15]       : 1 = Bridge fault. 'OR' of [14:0] below that are enabled.
    unsigned        TempCode                :8 ;  // [16 : 23]  : code representing temperature at humidity sensor
    unsigned        RhCode                  :8 ;  // [24 : 31]  : code representing relative humidity
};

//-------------------------------------------------------------------------------------------------
struct REBx_VcoIo1Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        A02                     :16;  // [0 : 15]   : VCO - IO# analog #2 - BN(AEAD) or BC(AEAA)
    unsigned        A03                     :16;  // [16 : 31]  : VCO - IO# analog #3 - CN(AEAD) or CA(AEAA)
    unsigned        A04                     :16;  // [32 : 47]  : VCO - IO# analog #4 - NotUsed(AEAD) or Netural-to-earth(AEAA)
    unsigned        A05                     :16;  // [48 : 63]  : VCO - IO# analog #5 - Phase A current
    unsigned        A06                     :16;  // [64 : 79]  : VCO - IO# analog #6 - Phase B current
    unsigned        A07                     :16;  // [80 : 95]  : VCO - IO# analog #7 - Phase C current
    unsigned        A01                     :16;  // [96 : 111] : VCO - IO# analog #1 - AN(AEAD) or AB(AEAA)
    unsigned        Vdc                     :16;  // [112 : 127]: VCO - 2nd DC Link voltage
};

//-------------------------------------------------------------------------------------------------
struct REBx_VcoVdbTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Id                      :16;  // [0 : 15]   : VCO - Bridge Phase current D -only active for BatteryController units
    unsigned        Vdcx                    :16;  // [16 : 31]  : VCO - Bridge Phase D Vdc: Link or DB resister voltage or other)
};

//-------------------------------------------------------------------------------------------------
struct REBx_StatReg21Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        REBxpwrOk               :1 ;  // [0]        : REBx P5V power status, 1=ok{L2#}
    unsigned        ContInb                 :10;  // [1 : 10]   : IO#1 - 10 contact inputs: 0= active. Note: software must immediately shut down drive if ContIn 09 or 10 become inactive (go HI).
    unsigned        BrlTx                   :1 ;  // [11]       : BRL transmit status, 1=ok{L2#}
    unsigned        BrlRxb                  :1 ;  // [12]       : BRL recieve status, 0=ok{L2#}
    unsigned        HslaPwrOk               :1 ;  // [13]       : 1 = HSLA power ok{L2#}
    unsigned        CtlPwrOk1               :1 ;  // [14]       : IO#1 - control power, 1=ok
    unsigned        RlyPwrOk                :1 ;  // [15]       : IO#1 - relay power, 1=ok
    unsigned        IamHSLA1                :1 ;  // [16]       : 1=pin identified as HSLA#1, 0=identified as HSLA#2
    unsigned        REPI_IDdn               :1 ;  // [17]       : 1=recovered REPI ID data, 0 = not available
    unsigned        REPI_OKb                :1 ;  // [18]       : 0= all REPI PS ok, 1= 1 or more not ok. Latched – cleared by REPI_EN low.
    unsigned        CIN                     :1 ;  // [19]       : 1=CIN (TB#3) closed, 0 = open
    unsigned        reserved24              :6 ;  // [20 : 25]  : unused
    unsigned        BrlRx                   :1 ;  // [26]       : BRL recieve status, 1=ok}
    unsigned        LBrl                    :1 ;  // [27]       : local BRL recieve status, 1=ok}
    unsigned        ClkBad                  :1 ;  // [28]       : 1=FPGA clk fault. Likely only seen for intermittant detections.
    unsigned        DIn01                   :1 ;  // [29]       : 0=10-24Vdc applied to TB3.1->2. 1= <10V input.{L#1}
    unsigned        P19Ok                   :1 ;  // [30]       : REBx - control power, 1=ok
    unsigned        FO2_OK                  :1 ;  // [31]       : 1= FO#2 active, 0= inactive
};

//-------------------------------------------------------------------------------------------------
struct REBx_TempRegEngUseOnlyTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        GateB1b                 :6 ;  // [0 : 5]    : output to bridge card for BRG# - active low
    unsigned        reserved21              :2 ;  // [6 : 7]    : unused
    unsigned        reserved22              :8 ;  // [8 : 15]   : unused
    unsigned        gState1                 :6 ;  // [16 : 21]  : output of gating scheduler for BRG#
    unsigned        reserved23              :2 ;  // [22 : 23]  : unused
    unsigned        reserved24              :8 ;  // [24 : 31]  : unused
};

//-------------------------------------------------------------------------------------------------
struct REBx_Tach01Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        TachT                   :10;  // [0 : 9]    : Rollover counter of time. 1count = 1usec
    unsigned        TachD                   :6 ;  // [10 : 15]  : Discrete inputs from tach M,/M,B,/B,A,/A = [15:10]
    unsigned        TachV                   :16;  // [16 : 31]  : Rollover up/down counter. Counts up/down for positive (A then B edge)/negative (B then A edge) rotation. Each count = 1 pulse. Can be used to calculate speen when time between samples & # of pulses per revolutions is known.
    unsigned        TachM                   :16;  // [32 : 47]  : Capture of TachV at the RE of marker pulse Combined with TachV to make positional calculations.
    unsigned        TachP                   :16;  // [48 : 63]  : Rollover counter. Counts the time between A & B tach pulses.  1 count = 1usec.  Cleared on each A or B edge.
};

//-------------------------------------------------------------------------------------------------
struct REBx_CrLbTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        VcoT03                  :16;  // [0 : 15]   : VCO counter for AEAD & Local thermister temperatures
    unsigned        LbReg                   :16;  // [16 : 31]  : Contents of register pointed to by CR_CFG.CFG_SEL.
};

//-------------------------------------------------------------------------------------------------
struct REBx_MiscStatTyp
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
struct subStructure_SDL_STTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        LVge                    :2 ;  // [0 : 1]    : [3:2] Lower Vce       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        LVce                    :2 ;  // [2 : 3]    : [1:0] Lower Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        LGateOk                 :1 ;  // [4]        : 1 = lower gate ok, 0=DESAT
    unsigned        UVge                    :2 ;  // [5 : 6]    : [8:7] Upper Vge       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        UVce                    :2 ;  // [7 : 8]    : [6:5] Upper Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        UGateOk                 :1 ;  // [9]        : 1 = upper gate ok, 0=DESAT
    unsigned        RGIB_ok                 :1 ;  // [10]       : 1 = RGIB (power supplies) ok
    unsigned        spare                   :1 ;  // [11]
    unsigned        SBW_ERR                 :1 ;  // [12]       :   1 = 1 or more packet (CRC) error in  SBW stream.
    unsigned        SBW_NEW                 :1 ;  // [13]       :    1= SBW data updated since last frame (captured if error free).
    unsigned        SDL_ID_DN               :1 ;  // [14]       :  1 = RGIB's ID(s) found & stored in local memory.
    unsigned        SDL_Lnk_OK              :1 ;  // [15]       : SDL link (frame) ok (1=ok) (0=fault) (clr by CR1.2). If SDL_LNK_OK=0 then [3:0]= error_code else: see below
};

//-------------------------------------------------------------------------------------------------
struct REBx_SDL21_STTyp
//-------------------------------------------------------------------------------------------------
{
    //subStructure_SDL_STTyp        SDL1_ST                 :16;  // [0 : 15]   : SDL#1 status.
    //subStructure_SDL_STTyp        SDL2_ST                 :16;  // [16 : 31]  : SLD#2 status.

    unsigned        LVge                    :2 ;  // [0 : 1]    : [3:2] Lower Vce       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        LVce                    :2 ;  // [2 : 3]    : [1:0] Lower Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        LGateOk                 :1 ;  // [4]        : 1 = lower gate ok, 0=DESAT
    unsigned        UVge                    :2 ;  // [5 : 6]    : [8:7] Upper Vge       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        UVce                    :2 ;  // [7 : 8]    : [6:5] Upper Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        UGateOk                 :1 ;  // [9]        : 1 = upper gate ok, 0=DESAT
    unsigned        RGIB_ok                 :1 ;  // [10]       : 1 = RGIB (power supplies) ok
    unsigned        spare                   :1 ;  // [11]
    unsigned        SBW_ERR                 :1 ;  // [12]       :   1 = 1 or more packet (CRC) error in  SBW stream.
    unsigned        SBW_NEW                 :1 ;  // [13]       :    1= SBW data updated since last frame (captured if error free).
    unsigned        SDL_ID_DN               :1 ;  // [14]       :  1 = RGIB's ID(s) found & stored in local memory.
    unsigned        SDL_Lnk_OK              :1 ;  // [15]       : SDL link (frame) ok (1=ok) (0=fault) (clr by CR1.2). If SDL_LNK_OK=0 then [3:0]= error_code else: see below

    unsigned        LVge2                    :2 ;  // [0 : 1]    : [3:2] Lower Vce       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        LVce2                    :2 ;  // [2 : 3]    : [1:0] Lower Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        LGateOk2                 :1 ;  // [4]        : 1 = lower gate ok, 0=DESAT
    unsigned        UVge2                    :2 ;  // [5 : 6]    : [8:7] Upper Vge       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        UVce2                    :2 ;  // [7 : 8]    : [6:5] Upper Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        UGateOk2                 :1 ;  // [9]        : 1 = upper gate ok, 0=DESAT
    unsigned        RGIB_ok2                 :1 ;  // [10]       : 1 = RGIB (power supplies) ok
    unsigned        spare2                   :1 ;  // [11]
    unsigned        SBW_ERR2                 :1 ;  // [12]       :   1 = 1 or more packet (CRC) error in  SBW stream.
    unsigned        SBW_NEW2                 :1 ;  // [13]       :    1= SBW data updated since last frame (captured if error free).
    unsigned        SDL_ID_DN2               :1 ;  // [14]       :  1 = RGIB's ID(s) found & stored in local memory.
    unsigned        SDL_Lnk_OK2              :1 ;  // [15]       : SDL link (frame) ok (1=ok) (0=fault) (clr by CR1.2). If SDL_LNK_OK=0 then [3:0]= error_code else: see below
};

//-------------------------------------------------------------------------------------------------
struct REBx_SDL43_STTyp
//-------------------------------------------------------------------------------------------------
{
    //unsigned        SDL3_ST                 :16;  // [0 : 15]   : SLD#1 status.
    //unsigned        SDL4_ST                 :16;  // [16 : 31]  : SLD#4 status.

    unsigned        LVge                    :2 ;  // [0 : 1]    : [3:2] Lower Vce       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        LVce                    :2 ;  // [2 : 3]    : [1:0] Lower Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        LGateOk                 :1 ;  // [4]        : 1 = lower gate ok, 0=DESAT
    unsigned        UVge                    :2 ;  // [5 : 6]    : [8:7] Upper Vge       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        UVce                    :2 ;  // [7 : 8]    : [6:5] Upper Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        UGateOk                 :1 ;  // [9]        : 1 = upper gate ok, 0=DESAT
    unsigned        RGIB_ok                 :1 ;  // [10]       : 1 = RGIB (power supplies) ok
    unsigned        spare                   :1 ;  // [11]
    unsigned        SBW_ERR                 :1 ;  // [12]       :   1 = 1 or more packet (CRC) error in  SBW stream.
    unsigned        SBW_NEW                 :1 ;  // [13]       :    1= SBW data updated since last frame (captured if error free).
    unsigned        SDL_ID_DN               :1 ;  // [14]       :  1 = RGIB's ID(s) found & stored in local memory.
    unsigned        SDL_Lnk_OK              :1 ;  // [15]       : SDL link (frame) ok (1=ok) (0=fault) (clr by CR1.2). If SDL_LNK_OK=0 then [3:0]= error_code else: see below

    unsigned        LVge2                    :2 ;  // [0 : 1]    : [3:2] Lower Vce       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        LVce2                    :2 ;  // [2 : 3]    : [1:0] Lower Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        LGateOk2                 :1 ;  // [4]        : 1 = lower gate ok, 0=DESAT
    unsigned        UVge2                    :2 ;  // [5 : 6]    : [8:7] Upper Vge       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        UVce2                    :2 ;  // [7 : 8]    : [6:5] Upper Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        UGateOk2                 :1 ;  // [9]        : 1 = upper gate ok, 0=DESAT
    unsigned        RGIB_ok2                 :1 ;  // [10]       : 1 = RGIB (power supplies) ok
    unsigned        spare2                   :1 ;  // [11]
    unsigned        SBW_ERR2                 :1 ;  // [12]       :   1 = 1 or more packet (CRC) error in  SBW stream.
    unsigned        SBW_NEW2                 :1 ;  // [13]       :    1= SBW data updated since last frame (captured if error free).
    unsigned        SDL_ID_DN2               :1 ;  // [14]       :  1 = RGIB's ID(s) found & stored in local memory.
    unsigned        SDL_Lnk_OK2              :1 ;  // [15]       : SDL link (frame) ok (1=ok) (0=fault) (clr by CR1.2). If SDL_LNK_OK=0 then [3:0]= error_code else: see below
};

//-------------------------------------------------------------------------------------------------
struct REBx_VdcSDL5_STTyp
//-------------------------------------------------------------------------------------------------
{
    //unsigned        SDL5_ST                 :16;  // [0 : 15]   : SLD#5 status
    unsigned        LVge                    :2 ;  // [0 : 1]    : [3:2] Lower Vce       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        LVce                    :2 ;  // [2 : 3]    : [1:0] Lower Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        LGateOk                 :1 ;  // [4]        : 1 = lower gate ok, 0=DESAT
    unsigned        UVge                    :2 ;  // [5 : 6]    : [8:7] Upper Vge       0: <= 1V;  1: >1V;   2: >2V;  3: >15V.
    unsigned        UVce                    :2 ;  // [7 : 8]    : [6:5] Upper Vge       0: <= 1V;  1: >1V;   2: >4V;  3: >16V.
    unsigned        UGateOk                 :1 ;  // [9]        : 1 = upper gate ok, 0=DESAT
    unsigned        RGIB_ok                 :1 ;  // [10]       : 1 = RGIB (power supplies) ok
    unsigned        spare                   :1 ;  // [11]
    unsigned        SBW_ERR                 :1 ;  // [12]       :   1 = 1 or more packet (CRC) error in  SBW stream.
    unsigned        SBW_NEW                 :1 ;  // [13]       :    1= SBW data updated since last frame (captured if error free).
    unsigned        SDL_ID_DN               :1 ;  // [14]       :  1 = RGIB's ID(s) found & stored in local memory.
    unsigned        SDL_Lnk_OK              :1 ;  // [15]       : SDL link (frame) ok (1=ok) (0=fault) (clr by CR1.2). If SDL_LNK_OK=0 then [3:0]= error_code else: see below

    unsigned        Vc                     :16;  // [16 : 31]  : VCO - Line-to-neutral voltage C.
};

//-------------------------------------------------------------------------------------------------
struct REBx_PktCntRbTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Cnt                     :32;  // [0 : 31]   : Loopback of value from control pkt - always last 32-bits.
};

#endif

