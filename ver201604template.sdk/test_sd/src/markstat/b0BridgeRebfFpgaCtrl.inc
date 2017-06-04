//
//   Generated with parser version 1.09
//   WARNING:  Do NOT manually modify; update input file or parser code only.
//
#ifndef INC_MAP_REBFB_00_HSLA_CTRL_PKT_H
#define INC_MAP_REBFB_00_HSLA_CTRL_PKT_H

// Register offset:
//-------------------
#define REBF_PktCnt_offset              0x0000
#define REBF_LoadPulseReg_offset        0x0004
#define REBF_Brg1PhsTraRegA0_offset     0x0008
#define REBF_Brg1PhsTraRegB0_offset     0x000c
#define REBF_Brg1PhsTraRegC0_offset     0x0010
#define REBF_Brg1PhsTraRegA1_offset     0x0014
#define REBF_Brg1PhsTraRegB1_offset     0x0018
#define REBF_Brg1PhsTraRegC1_offset     0x001c
#define REBF_Brg1PhsTraRegA2_offset     0x0020
#define REBF_Brg1PhsTraRegB2_offset     0x0024
#define REBF_Brg1PhsTraRegC2_offset     0x0028
#define REBF_Brg1PhsTraRegA3_offset     0x002c
#define REBF_Brg1PhsTraRegB3_offset     0x0030
#define REBF_Brg1PhsTraRegC3_offset     0x0034
#define REBF_Brg1PhsTraRegD0_offset     0x0038
#define REBF_Brg1PhsTraRegD1_offset     0x003c
#define REBF_Brg1PhsTraRegD2_offset     0x0040
#define REBF_SpareLocX044_offset        0x0044
#define REBF_CrCfg_offset               0x0048
#define REBF_Cr21_offset                0x004c
#define REBF_FanRef_offset              0x0050

// Types and Classes:
//-------------------
//-------------------------------------------------------------------------------------------------
struct REBF_PktCntTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Cnt                     :32;  // [0 : 31]   : Software increments this value every time it sends down a packet. FPGA loops it back in feedback pkt.
};

//-------------------------------------------------------------------------------------------------
struct REBF_LoadPulseRegTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        LdPls1Frc               :16;  // [0 : 15]   : FRC[15:0] match value to generates a LDPLS1 that transfer Rx Ctrl window to local registers. Bit-0 represents 40ns resolution.
    unsigned        LdPls2FrcDel            :16;  // [16 : 31]  : At LDPLS1, value added to LdPLs1Time, sum is compared to FCR[15:0] to generate LDPLS2
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegA0Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegB0Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegC0Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegA1Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegB1Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegC1Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegA2Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegB2Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegC2Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegA3Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegB3Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegC3Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegD0Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegD1Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_Brg1PhsTraRegD2Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        FrcDel                  :16;  // [0 : 15]   : Time to activate new state, FRC delta clocks from LDPLS1 (40ns/LSB)
    unsigned        State                   :2 ;  // [16 : 17]  : New phase state (b00 = OFF, b10 = VP, b01 = VN, b11 = OFF)
    unsigned        spare01                 :6 ;  // [18 : 23]
    unsigned        Enable                  :1 ;  // [24]       : Enable this FRC comparison (b1 = enabled, b0 = disabled)
    unsigned        spare02                 :7 ;  // [25 : 31]
};

//-------------------------------------------------------------------------------------------------
struct REBF_SpareLocX044Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        spare                   :32;  // [0 : 31]   : Software required to make mapping continous
};

//-------------------------------------------------------------------------------------------------
struct REBF_CrCfgTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        CfgVal                  :16;  // [0 : 15]   : Value used for register selected by  CfgRegSel
    unsigned        CfgRegSel               :8 ;  // [16 : 23]  : Sets which register to put Cf_RegVal when CE set.  Also selects which register to loopback into feedback packet as SR_LB
    unsigned        DiagFromFw1             :1 ;  // [24]       : value to output to DIAG[1] pin when DIAG_MUX_SEL[3:0]=15
    unsigned        DiagFromFw2             :1 ;  // [25]       : value to output to DIAG[2] pin when DIAG_MUX_SEL[11:8]=15
    unsigned        reserved1               :4 ;  // [26 : 29]
    unsigned        CfgEnable               :2 ;  // [30 : 31]  : b10 = Enable write to configuration registers.  Requires sequential pattern b01 to b10, takes effect on next packet
};

//-------------------------------------------------------------------------------------------------
struct REBF_Cr21Typ
//-------------------------------------------------------------------------------------------------
{
    unsigned        BrgEnable               :2 ;  // [0 : 1]    : b10 = Enable bridge#1 gating. Requires sequential pattern b01 to b10 to enable, any other value causes a disable. Drive_Flt also disables.
    unsigned        BrgFltClear             :1 ;  // [2]        : b1 = Clear latched bridge faults/Alarms (MC)
    unsigned        ThermHi                 :1 ;  // [3]        : b1 = Select High range for thermistor sensing, b0=select low range.
    unsigned        BrlTxDis                :1 ;  // [4]        : 1= disable the BRL & LBRL outputs
    unsigned        Relay101State           :1 ;  // [5]        : b1 = Energize IO# relay #01, b0= de-energize.
    unsigned        Relay102State           :1 ;  // [6]        : b1 = Energize IO# relay #02, b0= de-energize.
    unsigned        Relay103State           :1 ;  // [7]        : b1 = Energize IO# relay #03, b0= de-energize.
    unsigned        Relay104State           :1 ;  // [8]        : b1 = Energize IO# relay #04, b0= de-energize.
    unsigned        Relay105State           :1 ;  // [9]        : b1 = Energize IO# relay #05, b0= de-energize.
    unsigned        Relay106State           :1 ;  // [10]       : b1 = Energize IO# relay #06, b0= de-energize.
    unsigned        Relay107State           :1 ;  // [11]       : b1 = Energize IO# relay #07, b0= de-energize.
    unsigned        Relay108State           :1 ;  // [12]       : b1 = Energize IO# relay #08, b0= de-energize.
    unsigned        Relay109State           :1 ;  // [13]       : b1 = Energize IO# relay #09, b0= de-energize.
    unsigned        ForceDb                 :2 ;  // [14 : 15]  : b1 = GATE DB#1[U/L] cells. b0 = disable DB gating. (Note: upper disabled in FPGA).
    unsigned        Spare01                 :2 ;  // [16 : 17]  : spare
    unsigned        LvRelayState            :3 ;  // [18 : 20]  : Energize the local low-voltage relays[1->3]. LvRelay[4] energized for not faults
    unsigned        Spare02                 :11;  // [21 : 31]  : spare
};

//-------------------------------------------------------------------------------------------------
struct REBF_FanRefTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        IlimEnb                 :1 ;  // [0]
    unsigned        AiocEnbBr1              :1 ;  // [1]        : 1= enable Phase A IOC fault
    unsigned        BiocEnbBr1              :1 ;  // [2]        : 1= enable Phase B IOC fault
    unsigned        CiocEnbBr1              :1 ;  // [3]        : 1= enable Phase C IOC fault
    unsigned        DiocEnbBr1              :1 ;  // [4]        : 1= enable DB IOC fault
    unsigned        spare02                 :4 ;  // [5 : 8]    : n/u
    unsigned        DesatEnbBr1             :1 ;  // [9]        : 1= enable Phase A,B,C upper & lower cell desat faults
    unsigned        spare01                 :1 ;  // [10]       : n/u
    unsigned        IlimEnD                 :1 ;  // [11]       : 1= hardware current limit enable. Turns off cells in phase D with overcurrent till no overcurrent (or fault).
    unsigned        spare00                 :4 ;  // [12 : 15]  : n/u
    unsigned        FanRef1                 :8 ;  // [16 : 23]  : Fan#1 speed reference: <32 off, 32->255 0-100%.{L#1}
    unsigned        FanRef2                 :8 ;  // [24 : 31]  : Fan#2 speed reference: <32 off, 32->255 0-100%.{L#1}
};


#endif

