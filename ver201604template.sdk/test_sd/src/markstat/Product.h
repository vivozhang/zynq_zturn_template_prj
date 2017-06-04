///////////////////////////////////////////////////////////////////////////////
//
// TITLE:
//
// DESCRIPTION:
//             Contains product specific information.  This file is
//             used to construct products from various lower level
//             modules.  It is in effect the master ".H" file for
//             the product be linked.
//
// COPYRIGHT:   Copyright (c) 2004
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include <stdint.h>
//typedef unsigned  uint32_t;      // need to eradicate these things

// Include Files
//--------------
// system
// core
// framework
#include "x0Status.h"
#include "x0Rule.h"
// product
#define TODO
#define WIND_DFIG_1X 1

#define PRODUCT_TYPE WIND_DFIG_1X

// Constants
//----------

#define SIMPLE_GRID_MODEL 0

//-----------------------------------------------------------------------------
// rule set case definitions
//   (1-9 are reserved by framework and defined in x0Rule.h)
#define RULE_SET_DEFAULT_CAPT_BUFF      20
#define RULE_SET_DEFAULT_DIAG_FILTER    21
#define RULE_SET_DEFAULT_RELAY_OUT      23
#define RULE_SET_DEFAULT_AIN_FILTER     24
#define RULE_SET_DEFAULT_GROUND_FLT     27
#define RULE_SET_DEFAULT_PER_UNIT       28
#define RULE_SET_DEFAULT_MISC           39
#define RULE_SET_VOLT_VAR_SETUP         40
#define RULE_SET_MTR_CTL_SETUP          52
#define RULE_SET_TORQUE_LIMIT_SET       53
#define RULE_SET_DEFAULT_LINE_PROT      60
#define RULE_SET_DEFAULT_SEQUENCER      70
#define RULE_SET_SECOND_PASS            100

//-----------------------------------------------------------------------------
// high speed serial link assignments (global link number, count from 1)
#define HSSL_NUM_L                      1                       // line bridge
#define HSSL_NUM_R                      2                       // rotor bridge

//-----------------------------------------------------------------------------
// class instance uniqifier text for var/parm/diag descriptions
#define BRG_UNIQ_L                      "Line Bridge"           // line bridge with X030
#define BRG_UNIQ_L_BR                   "Line Bridge"           // line bridge interface
#define BRG_UNIQ_L_IO1                  "Line I/O"               // line IO 1 interface

#define BRG_UNIQ_R                      "Rotor Bridge"          // rotor bridge with X030
#define BRG_UNIQ_R_BR                   "Rotor Bridge"          // rotor bridge interface
#define BRG_UNIQ_R_IO1                  "Rotor I/O"              // rotor IO 1 interface

#define BRG_UNIQ_Lx                     "LineX Bridge"          // line bridge with REBF
#define BRG_UNIQ_Lx_BR1                 "LineX Brg1"            // line bridge 1 interface
#define BRG_UNIQ_Lx_BR2                 "LineX Brg2(unused)"    // line bridge 2 interface
#define BRG_UNIQ_Lx_IO1                 "LineX IO1"             // line IO 1 hardware
#define BRG_UNIQ_Lx_IO2                 "LineX IO2"             // line IO 2 hardware

#define BRG_UNIQ_Rx                     "RotorX Bridge"         // rotor bridge with REBF
#define BRG_UNIQ_Rx_BR1                 "RotorX Brg1"           // rotor bridge 1 interface
#define BRG_UNIQ_Rx_BR2                 "RotorX Brg2"           // rotor bridge 2 interface
#define BRG_UNIQ_Rx_IO1                 "RotorX IO1"            // rotor IO 1 interface
#define BRG_UNIQ_Rx_IO2                 "RotorX IO2"            // rotor IO 2 interface

#define L_CONT_UNIQ                     "Line Contactor"        // line side contactor
#define R_CONT_UNIQ                     "Rotor Contactor"       // rotor side contactor
#define DF_CONT_UNIQ                    "DF Contactor"          // distortion filter contactor
#define XFMR_UNIQ_S                     "Xfmr 6KV Sec"          // transformer 6KV secondary
#define XFMR_UNIQ_L                     "Xfmr 690V Sec"         // transformer 690V secondary

#define CCC_HTR_UNIQ                    "CCC Heater"            // Converter control cabinet heater
#define CFC_HTR_UNIQ                    "CFC Heater"            // Converter distortion filter heater
#define PDC_HTR_UNIQ                    "PDC Heater"            // Converter AC entry heater
#define LQD_HTR_UNIQ                    "LQD Heater"            // Liquid coolant heater

#define BRG_UNIQ_DB                     "Dynamic brake"         // dynamic brake
#define BRG_UNIQ_DB1                    "Dynamic brake"         // dynamic brake circuit 1
#define BRG_UNIQ_DB2                    "Dynamic brake-2"       // dynamic brake circuit 2

#define BRG_UNIQ_DC_CHOP                "DC Chopper"            // Dc chopper

//-----------------------------------------------------------------------------
// class instance bridge #s for diagnostic booleans TrpFlt1, TrpFlt2
#define BRG_NUM_L                       bridge_1                // line side
#define BRG_NUM_R                       bridge_2                // rotor side
#define USE_XLS                         use_xls                 // use bridge entries from .xls file

//-----------------------------------------------------------------------------
// enumerations for parameter SoftwareIPN
#define DFIG02_1500_5060_A               2                       // Converter version A, MACC
#define DFIG02_1500_5060_B               3                       // Converter version B, MACC
#define DFIG02_1850_5060_C               4                       // Converter version C, MACC
#define DFIG02_1850_5060_D               5                       // Converter version D, MACC
#define DFIG02_1850_5060_E               6                       // Converter version E


#define DFIG02_2400_5060_A              10                      // Converter version A, MarkStat
#define DFIG02_2400_5060_B              11                      // Converter version B, MarkStat


//-----------------------------------------------------------------------------
// enumerations for IPN_Version
#define IPN_VERS_A 1
#define IPN_VERS_B 2
#define IPN_VERS_C 3
#define IPN_VERS_D 4
#define IPN_VERS_E 5


//-----------------------------------------------------------------------------
// enumerations for parameter GenTypeCfg
#define GENTYPE_UNSPECIFIED   0
#define GENTYPE_OTHER        100

#define GENTYPE_JFSA560SR06A_WIN60HZ 19
#define GENTYPE_JFEC500SS06A_WIN60HZ 20
#define GENTYPE_TFFOANDQ_HIT60HZ 21

#define GENTYPE_TFFOANDQ_GEHP60HZ 22
#define GENTYPE_DASAA50244UAA_NTC50H 23
#define GENTYPE_JFEC500SR04A_WIN50HZ 24
#define GENTYPE_JFEE500SR04A_WIN50HZ 25
#define GENTYPE_JFEA500SR04A_WIN50HZ 26
#define GENTYPE_JFSA560SR06A_WIN50HZ 27

#define GENTYPE_NAR560G6_IND60HZ     28
#define GENTYPE_NAR560G6_IND50HZ     29


//-----------------------------------------------------------------------------
// enumerations for Cable Type
// 50Hz Cable
#define CABTYP_C50HZ_LEGACY 1
#define CABTYP_C50Hz_BDL_95 2
#define CABTYP_C50Hz_SPC_24 3
#define CABTYP_C50_5X240_2X240  30
#define CABTYP_C50Hz_13522_4_300_2_240 40

// 60Hz Cable
#define CABTYP_C60HZ_LEGACY            51
#define CABTYP_C60Hz_BDL_44            52
#define CABTYP_C60Hz_SPC_44            53
#define CABTYP_C60Hz_13522_5_535_2_535         70
#define CABTYP_C60Hz_13522_4_646_2_444 75
#define CABTYP_C60Hz_13522_4_646_2_535 80

#define CABTYP_UNSP  0
#define CABTYP_OTHER 100
#define CABTYP_UNSPECIFIED 0



//-----------------------------------------------------------------------------
// enumerations for parameter TestMode
#define TEST_DISABLED                   0                       // Test mode disabled
#define PUMP_BACK                       1                       // Pump back test mode
#define HARDWARE_TEST                   3                       // Factory hardware test mode
#define LF_LEG_TEST                     5                       // Low frequency leg test mode


//-----------------------------------------------------------------------------
// enumerations for parameter ConverterLoc
#define FIELD_APPL                      0                       // Field application
#define FACTORY                         1                       // Factory location
#define LABORATORY                      2                       // Lab location

//-----------------------------------------------------------------------------
// enumerations for parameter R_BrgPwrPrm
#define BRIDGE_1_ONLY                   1                       // Rotor bridge 1 only enabled
#define BRIDGE_2_ONLY                   2                       // Rotor bridge 2 only enabled
#define BOTH_BRIDGES                    3                       // Both bridges enabled


//-----------------------------------------------------------------------------
// enumerations for parameter GmVltPrtXfmrWndSel
#define PRIMARY                         1                       // Grid protection uses primary voltage
#define SECONDARY_6KV                   2                       // Grid protection uses 6 kV voltage
#define SECONDARY_690                   3                       // Grid protection uses 690 voltage

//-----------------------------------------------------------------------------
// enumerations for parameter VRTSupptTyp
#define VRTCONV                         0                       // VRT Iy support type Conventional
#define VRTPROP                         1                       // VRT Iy support type Proportional

//-----------------------------------------------------------------------------
// enumerations for parameter GridStrength
#define STRONG                          0                       //
#define WEAK                            1                       //
#define VERY_WEAK                       2                       //

//-----------------------------------------------------------------------------
// enumerations for parameter VARPfModSel
#define PFMODE                         0                       //
#define VARMODE                         1                       //


//-----------------------------------------------------------------------------
// enumerations for parameter TorProtType
#define TACH_BASED                      1
#define TORQUE_BASED                    2

//-----------------------------------------------------------------------------
// enumerations for parameter R_TorSimMode
#define TORSIMSPDMODE                   0
#define TORSIMTRQMODE                   1

//-----------------------------------------------------------------------------
// enumerations for parameter GmCfgSel
#define GM_WIZ_SVRT           0
#define GM_WIZ_LVRT           1
#define GM_WIZ_ZVRT           2
#define GM_WIZ_MAX_CONV       3
#define GM_WIZ_CUST           4
#define GM_WIZ_ZVRT_GEN2_BASE 5
#define GM_WIZ_ZVRT_GEN2_OPT  6
#define GM_WIZ_ZVRT_GEN3_BASE 7

#define FAULTISACTIVE(flt)  (FaultIsActive.flt)

#define DISABLE_DATA_PASS   (1)

#define RAD3_3              0.577350269
#define UP                  1
#define DOWN                0

#define ON                  1
#define OFF                 0

#define POS_SEQ_DSCM        0

#define STOPPED_COMMON      0
#define CONV_RUN_REQ_LINE   1
#define CONV_RUNNING_LINE   2
#define CONV_RUN_REQ_ROTOR  3
#define CONV_RUNNING_ROTOR  4
#define CONV_STOP_REQ_ROTOR 5
#define CONV_STOP_REQ_LINE  6
#define INIT_COMMON         -1

#define UNKNOWNTYPE         (-1)
#define FLOATTYPE           0
#define DOUBLETYPE          1
#define INTTYPE             2
#define UNSIGNEDTYPE        3
#define HEXTYPE             4

#define VPRP_AVG_ARRAY_SIZE 12

#ifndef  WIN_SIM

#define NANSET(a)   *(unsigned*)&a = 0x80800000

//-----------------------------------------------------------------------------
// Calcs offset of structure element (m) from beginning of structure (t)
// #define   offsetof(t, m)  ((int) &(((t *) 0) -> m))  --> This is a standard definition in QNX files

#endif  //WIN_SIM

#define DEF_Fpll_Phs_Off    0.998   // FPLL Phase Offset     , pu
#define DEF_Fpll_Prop_Gn    0.25    // FPLL Proportional Gain, pu
#define DEF_FLLPllIntGn     0.005   // FPLL Integral Gain    , pu
#define DEF_Fpll_Out_Lim    0.0003  // FPLL Output Limit     , pu

#define  PHS_0    0u
#define  PHS_A    1u
#define  PHS_B    2u
#define  PHS_C    4u
#define  PHS_AB   3u
#define  PHS_BC   6u
#define  PHS_CA   5u
#define  PHS_ABC  7u

#define SUCCESS_MSG     30          // completed successfully
#define FAIL_MSG        31          // failed
#define ABORT_MSG       32          // aborted
#define ENDOP_MSG       33          // end of operation

#define MTRCTL_TUNE_ROUTINE     1   // motor control tuneup routine error number
#define SPDREG_TUNE_ROUTINE     2   // speed regulator tuneup routine error number
#define SYNC_MTRCTL_TUNE_ROUTINE 3  // motor control tuneup routine error number
#define COMPLETE        1
#define TUNE_SIM_MSG    60          // autotune simulated measurements
#define STACK_WARN      61          // autotune stack warning
#define MEMORY_ERR      62          // heap memory allocation error
#define RECORD_ERR      63          // data recorder error
#define CNCT_ERR        64          // contactor error
#define CALC_MSG        65          // calculate motor control parameters
#define TRIPFLT_MSG     66          // trip fault occured
#define ERR_CODE        67          // master error code
#define WARN_MSG        68          // warning

//-----------------------------------------------------------------------------
// VCO constants
#define VCO_MASK     0xFFFF

//-----------------------------------------------------------------------------
// Cases for NetType
#define NET_ABSENT  0
#define NET_IPC   5

// Derived Quantities
#define FPLL_FRQCMD_SCL     19.073486   // Resultant Fpll.Freq_Cmd scaling
// =FPLL_CLK_BASE/FPLL_FRQCMD_UNITY

#define FPLL_CLK0_FRQ_NOM   15e6        // Resultant Nominal Clk0 Frequency
// =FPLL_CLK_BASE*FPLL_FRQRAT_NOM

#define FPLL_FRQCMD_NOM     0x0C0000    // Resultant Nominal Fpll.Freq_Cmd
// =FPLL_FRQCMD_UNITY*FPLL_FRQRAT_NOM

// Used by p0FltChk.cpp
# ifdef  BUILD_NVVAR
#   define NVVAR_FLOAT(name)  non-volatile-variable float     name
#   define NVVAR_INT(name)    non-volatile-variable int       name
#   define NVVAR_UINT(name)   non-volatile-variable unsigned  name
# else
#   define NVVAR_FLOAT(name)  /* expands to nothing */
#   define NVVAR_INT(name)    /* expands to nothing */
#   define NVVAR_UINT(name)   /* expands to nothing */
# endif

// Timebase/Modulator Hardware Register Limits
#define BICL_CNT0_MAX    8191       /* Maximum Cnt0Pwm value (13-bits)       */

#define  AMB_TEMP_DATA_PTS         64
#define  THRM_RNG_AUTO             0
#define  THRM_RNG_HIGH             1
#define  THRM_RNG_LOW              2
#define  TEMP_DATA_LOW_ARRAY_SIZE  246
#define  TEMP_DATA_LOW_PTS         (TEMP_DATA_LOW_ARRAY_SIZE-1)
#define  GM_RAF_ARRAY_SIZE         13


#define INIT_FPGA_DLY       60000   // Delay 1ms for T1 load pulse
#define DB_GATE_INIT        0
#define DB_GATE_NORMAL      1
#define DB_GATE_FAULT       2
#define DB_GATE_DISABLE     3

//#define XSIM_BUILD

#define DSIM_MODE

//#define LEG_TEST

// Function Prototypes
//--------------------


// Types and Classes
//------------------

//-----------------------------------------------------------------------------
// Replacement structure for macc's FAULTISACTIVE() macro
// Note that for every fault whose status is needed, the corresponding structure element
// must be set when the fault is pushed, and cleared on a fault reset.
struct FaultIsActiveTyp
{
    unsigned    CoolngSysAlm;
    unsigned    GmFastOverV;
    unsigned    GmRevPwrTrp;
    unsigned    Ps24Nok;
    unsigned    R_CrowBarFlt;
    unsigned    R_CCClsTmout;
    unsigned    S_IOCFlt;
    unsigned    S_K1ClsTmout;
    unsigned    S_K1OpnTmout;
    unsigned    S_K1StuckClose;
    unsigned    S_SyncIOC2Flt;
    unsigned    IslandFlt;
    unsigned    KdfOpnTmOut;
    unsigned    KdfClsTmOut;
    unsigned    R_OverSpeed;
    unsigned    R_UnderSpeed;

};

struct CtlBt1
{
    unsigned    Spare_01        :1;  // Spare 01
    unsigned    Spare_02        :1;  // Spare 02
    unsigned    Spare_03        :1;  // Spare 03
    unsigned    Spare_04        :1;  // Spare 04

    unsigned    Spare_05        :1;  // Spare 05
    unsigned    Spare_06        :1;  // Spare 06
    unsigned    Spare_07        :1;  // Spare 07
    unsigned    Spare_08        :1;  // Spare 08

    unsigned    Spare_09        :1;  // Spare 09
    unsigned    Spare_10        :1;  // Spare 10
    unsigned    Spare_11        :1;  // Spare 11
    unsigned    Spare_12        :1;  // Spare 12

    unsigned    Spare_13        :1;  // Spare 13 - last element
};

struct RegLimFlgs
{
    unsigned    L_PllLimMax     :1;  // Phase lock loop at maximum frequency
    unsigned    L_PllLimMin     :1;  // Phase lock loop at minimum frequency
    unsigned    L_IxrefPLim     :1;  // Voltage regulator output in pos limit
    unsigned    L_IxrefNLim     :1;  // Voltage regulator output in pos limit

    unsigned    L_XAntiWndP     :1;  // Current regulator summary in neg lim.
    unsigned    L_XAntiWndN     :1;  // Current regulator summary in neg lim.
    unsigned    L_UyLimPFlg     :1;  // uy positive limit flag
    unsigned    L_UyLimNFlg     :1;  // uy negative limit flag

    unsigned    L_IyLimPFlg     :1;  // L_Iy positive limit flag
    unsigned    L_IyLimNFlg     :1;  // L_Iy negative limit flag
    unsigned    L_UxlimPFlg     :1;  // Up-down limit positive flag
    unsigned    L_UxlimNFlg     :1;  // Up-down limit negative flag

    unsigned    Spare_12        :1;  // Spare 12
    unsigned    Spare_13        :1;  // Spare 13 - last element
};

struct SeqActBits
{
    unsigned    L_RegEnbAct     :1;  // Regulator enable action
    unsigned    L_ChgEnbAct     :1;  // Charge enable action
    unsigned    L_SyncIniAct    :1;  // Syncronization enable action
    unsigned    L_Test_Act      :1;  // B3  Inductance test action
    unsigned    L_KdfEnbAct     :1;  // B4  Kdf contactor enable action
    unsigned    L_Q1EnbAct      :1;  // B5  Q1 breaker enable action
    unsigned    Spare_06        :1;  // B6  Spare
    unsigned    Spare_07        :1;  // B7  Spare
    unsigned    Spare_08        :1;  // B8  Spare
    unsigned    L_DbPsTstAct    :1;  // B9  DB pulse test active
    unsigned    L_PlsTstAct     :1;  // B10 Pulse test active
    unsigned    L_CelTstAct     :1;  // B11 Cell test active
    unsigned    L_DbClTstAct    :1;  // B13 DB pulse test active
    unsigned    L_DbResMsAct    :1;  // B13 DB resistance measure active
    unsigned    L_DbCapMsAct    :1;  // B14 DB capacitance measure active
};

struct SeqReqBits
{
    unsigned    L_RegEnbReq     :1;  // Regulator enable request
    unsigned    L_ChgEnbReq     :1;  // Charge enable request
    unsigned    L_SyncIniReq    :1;  // Syncronization enable request
    unsigned    L_Test_Req      :1;  // B3  Inductance test request
    unsigned    L_KdfEnbReq     :1;  // B4  Kdf contactor enable request
    unsigned    L_Q1EnbReq      :1;  // B5  Q1 breaker enable request
    unsigned    Spare_06        :1;  // B6  Spare
    unsigned    Spare_07        :1;  // B7  Spare
    unsigned    Spare_08        :1;  // B8  Spare
    unsigned    L_DbPsTstReq    :1;  // B9  DB pulse test request
    unsigned    L_PlsTstReq     :1;  // B10 Pulse test request
    unsigned    L_CelTstReq     :1;  // B11 Cell test request
    unsigned    L_DbClTstReq    :1;  // B12 DB pulse test request
    unsigned    L_DbResMsReq    :1;  // B13 DB resistance measure request
    unsigned    L_DbCapMsReq    :1;  // B14 DB capacitance measure request
};

struct SeqStatBits
{
    unsigned    L_RegEnbStat    :1;  // Regulator enable status
    unsigned    L_ChgEnbStat    :1;  // Charge enable status
    unsigned    L_SyncIniSta    :1;  // Syncronization enable status
    unsigned    L_LTestStat     :1;  // B3  Inductance test status
    unsigned    L_KdfEnbStat    :1;  // B4  Kdf contactor enable status
    unsigned    L_Q1EnbStat     :1;  // B5  Q1 breaker enable status
    unsigned    Spare_06        :1;  // B6  Spare
    unsigned    Spare_07        :1;  // B7  Spare
    unsigned    Spare_08        :1;  // B8  Spare
    unsigned    L_DbPsTstSta    :1;  // B9  DB pulse test status
    unsigned    L_PlsTstStat    :1;  // B10 Pulse test status
    unsigned    L_CelTstStat    :1;  // B11 Cell test status
    unsigned    L_DbClTstSta    :1;  // B12  DB cell test status
    unsigned    L_DbResMsSta    :1;  // B13 DB resistance measure status
    unsigned    L_DbCapMsSta    :1;  // B14 DB capacitance measure status
};

struct SeqBits1
{
    unsigned   Stopped         :1;   // B0 Sequencer is in Stopped state
    unsigned   Parked          :1;   // B1 Sequencer is parked - hard reset to exit
    unsigned   L_Test          :1;   // B2  Inductance test
    unsigned   L_PulseTest     :1;   // B3  Pulse test is active
    unsigned   L_CellTest      :1;   // B4  Cell test is active
    unsigned   L_DbPsTest      :1;   // B5  DB pulse test is active
    unsigned   Spare_06        :1;   // B6  Spare
    unsigned   L_DiagMode      :1;   // B7
    unsigned   L_DbClTest      :1;   // B8  DB cell test is active
};


struct PwmDBCmdRgr
{
    //6h (0x70010) - used in bridge 1 only
    unsigned    spare         :31;   /* first 31 are spare */
    unsigned    Phs_D_St_0    :1;    /* Event 0: state                 */

};

// from a0FlxCmd

struct RRegLimFlgs
{
    unsigned   R_IxCmdFrzp     :1;  /* bit 0: Ix command positive limit hit */
    unsigned   R_IxCmdFrzn     :1;  /* bit 1: Ix Command neg limit hit */
    unsigned   R_TrqCmdLimP    :1;  /* bit 2: Torque command positive limit hit */
    unsigned   R_TrqCmdLimN    :1;  /* bit 3: Torque command negative limit hit */

    unsigned   S_PllLimMax     :1;  /* bit 4: Stator Pll max limit */
    unsigned   S_PllLimMin     :1;  /* bit 5: Stator Pll min limit */
    unsigned   S_IyregLimP     :1;  /* bit 6: Stator Iy reg pos limit */
    unsigned   S_IyregLimN     :1;  /* bit 7: Stator Iy reg neg limit */

    unsigned   S_VregLimP      :1;  /* bit 8: Stator voltage reg pos limit */
    unsigned   S_VregLimN      :1;  /* bit 9: Stator voltage reg neg limit */
    unsigned   S_IyCmdLimP     :1;  /* bit10: Iy cmd pos limit */
    unsigned   S_IyCmdLimN     :1;  /* bit11: Iy cmd neg limit */

    unsigned   S_IxregLimP     :1;  /* bit12: Stator Ix reg pos limit */
    unsigned   S_IxregLimN     :1;  /* bit13: Stator Ix reg neg limit */
    unsigned   VregLimP        :1;  /* bit14: Voltage regulator pos limit */
    unsigned   VregLimN        :1;  /* bit15: Voltage regulator neg limit */
};

//================ Sequencer Task Status Bits ======================
struct R_SeqStatBits
{
    unsigned   R_CCEnbStat    :1;  // B0  Rotor bridge-1 Crowbar Contactor enable status
    unsigned   R2_CCEnbStat    :1;  // B1  Rotor bridge-2 Crowbar Contactor enable status
    unsigned   R_TrqEnbStat    :1;  // B2  Torque enable status
    unsigned   R_SyncStat      :1;  // B3  Sync status
    unsigned   R_SCEnbStat     :1;  // B4  Sync contactor enable status
    unsigned   R_DbTstStat     :1;  // B5  Db sequencer test status
    unsigned   R_Q1EnbStat     :1;  // B6  Q1 Dc-Chopper breaker enable status
    unsigned   Spare_07        :1;  // B7  Spare
    unsigned   Spare_08        :1;  // B8  Spare
    unsigned   R_DBPsTstSta    :1;  // B9  DB pulse test status
    unsigned   R_ChPlsTstStat  :1;  // B10 Dc-Chopper pulse test status
    unsigned   R_PlsTstStat    :1;  // B11 Pulse test status
    unsigned   R_CelTstStat    :1;  // B12 Cell test status
    unsigned   R_MctlTunSta    :1;  // B13 Motor Control tune status
    unsigned   R_DBClTstSta    :1;  // B14  DB cell test status
    unsigned   Spare_14        :1;  // B15 Spare
};

//============== Sequencer bits to Motor control ================*/
struct SeqMCtlBits
{
    unsigned   R_IoffNullEn    :1;  // B0:  I offset null enable
    unsigned   R_VoffNullEn    :1;  // B1:  V offset null enable
    unsigned   R_AngElecFrz    :1;  // B2:  Freeze Rotor angle
    unsigned   S_VregEn        :1;  // B3:  Stator voltage reg enable

    unsigned   S_PllEnab       :1;  // B4:  Stator Pll enable
    unsigned   S_SyncCmd       :1;  // B5:  Stator sync to line command
    unsigned                   :1;  // B6:
    unsigned   R_SpdFbkVld     :1;  // B7:  Speed feedback valid to T1

    unsigned   R_FlxCmdOn      :1;  // B8:  Flux command on
    unsigned   R_IoffSlowSw    :1;  // B9:  Switch to slow I offs null omega
    unsigned   R_VOffsRdy      :1;  // B10: V offset null is ready to perform
    unsigned   R_FlxIntEnb     :1;  // B11:

    unsigned   R_FlxPllEnb     :1;  // B12:
    unsigned                   :1;  // B13:
    unsigned   R_VoffFastSw    :1;  // B14: Use fast voltage offset times
    unsigned                   :1;  // B15:
};

enum    Data_State_Type      // list of data collection states
{
    IDLE,
    APPLIED,                // applied current, waiting for steady state
    SSTATE,                 // steady state current
    REMOVED                 // removed current, decaying to zero
};


struct Rec_Ctl_Type         // 2 channel data recorder input and storage control information
{
    unsigned Brg_Pwr_Enb;       // bridge power state to collect data
    int      Num_Data_Pts;      // number of data points to be collected for each channel
    int      Num_Smpl;          // number of samples to be collected for each data point
    float    *Input_A;          // pointer to input channel A, NULL if not used
    float    *Data_Buf_A;       // pointer to channel A's data buffer
    float    *Input_B;          // pointer to input channel B, NULL if not used
    float    *Data_Buf_B;       // pointer to channel B's data buffer
    unsigned Phs_To_Pls;        // phase to pulse
    float    Pls_Width;         // pulse width (sec)
};

struct R_SeqBits1           // Task Interface State bits
{
    unsigned R_Stopped      :1;  // B0
    unsigned R_Running      :1;  // B1
    unsigned R_DBPsTest     :1;  // B2  DB pulse test is active
    unsigned R_ChPulsesTest :1;  // B3  Dc-Chopper pulse test is active
    unsigned R_PulseTest    :1;  // B4
    unsigned R_CellTest     :1;  // B5
    unsigned R_MtrCtlTune   :1;  // B6
    unsigned R_DBClTest     :1;  // B7  DB pulse test is active
    unsigned R_DiagMode     :1;  // B8
    unsigned R_TuneMode     :1;  // B9
    unsigned R_Parked       :1;  // B10
    unsigned Sharing        :1;  // B11
};

struct R_SeqReqBits         // Sequencer Task Request Bits
{
    unsigned R_CCEnbReq    :1;  // B0  Rotor bridge-1 Crowbar Contactor enable request
    unsigned R2_CCEnbReq    :1;  // B1  Rotor bridge-2 Crowbar Contactor enable request
    unsigned R_TrqEnbReq    :1;  // B2  Torque enable request
    unsigned R_SyncReq      :1;  // B3  Sync request
    unsigned R_SCEnbReq     :1;  // B4  Sync contactor enable request
    unsigned R_DbTstReq     :1;  // B5  DB sequencer test request
    unsigned R_Q1EnbReq     :1;  // B6  Q1 Dc-Chopper breaker enable request
    unsigned Spare_07       :1;  // B7
    unsigned Spare_08       :1;  // B8  Spare
    unsigned R_DBPsTstReq   :1;  // B9  DB pulse test request
    unsigned R_ChPlsTstReq  :1;  // B10 Dc-Chopper pulse test request
    unsigned R_PlsTstReq    :1;  // B11 Pulse test request
    unsigned R_CelTstReq    :1;  // B12 Cell test request
    unsigned R_MctlTunReq   :1;  // B13 Motor Control tune request
    unsigned R_DBClTstReq   :1;  // B14 DB cell test request
    unsigned Spare_14       :1;  // B15 Spare
};


struct R_SeqActBits        // Sequencer Task Active Bits
{
    unsigned R_CCEnbAct     :1;  // B0  Rotor bridge-1 Crowbar Contactor enable active
    unsigned R2_CCEnbAct    :1;  // B1  Rotor bridge-2 Crowbar Contactor enable active
    unsigned R_TrqEnbAct    :1;  // B2  Torque enable active
    unsigned R_SyncAct      :1;  // B3  Sync active
    unsigned R_SCEnbAct     :1;  // B4  Sync contactor enable active
    unsigned R_DbTstAct     :1;  // B5  Db sequencer test active
    unsigned R_Q1EnbAct     :1;  // B6  Q1 Dc-Chopper breaker enable active
    unsigned Spare_07       :1;  // B7  Spare
    unsigned Spare_08       :1;  // B8  Spare
    unsigned R_DBPsTstAct   :1;  // B9  DB pulse test active
    unsigned R_ChPlsTstAct  :1;  // B10 Dc-Chopper pulse test active
    unsigned R_PlsTstAct    :1;  // B11 Pulse test active
    unsigned R_CelTstAct    :1;  // B12 Cell test active
    unsigned R_MctlTunAct   :1;  // B13 Motor Control tune active
    unsigned R_DBClTstAct   :1;  // B14 DB cell test active
    unsigned Spare_14       :1;  // B15 Spare
};

//PWM command register
struct  PwmCmdRgr
{
                                //6h (0x70006) - bridge 2
        // Register : U11 Offset 1h (0x700001), 6h Phase A PWM command - bridge 1

    unsigned  Phs_A_Tmr_1   :13;        /* Event 1: Time from frame start */
    unsigned  Phs_A_St_1    :2;         /* Event 1: state                 */
    unsigned  Phs_A_Tmr_2   :13;        /* Event 2: Time from frame start */
    unsigned  Phs_A_St_2    :2;         /* Event 2: state                 */
    unsigned  Phs_A_St_0    :2;         /* Event 0: state                 */
                               //7h (0x70007) -- bridge 2
        // Register : U11 Offset 2h (0x700002), 7h Phase B PWM command -bridge 1

    unsigned  Phs_B_Tmr_1   :13;        /* Event 1: Time from frame start */
    unsigned  Phs_B_St_1    :2;         /* Event 1: state                 */
    unsigned  Phs_B_Tmr_2   :13;        /* Event 2: Time from frame start */
    unsigned  Phs_B_St_2    :2;         /* Event 2: state                 */
    unsigned  Phs_B_St_0    :2;         /* Event 0: state                 */
                               //8h (0x700008) --bridge 2
        // Register : U11 Offset 3h (0x700002) Phase C PWM command --bridge 1

    unsigned  Phs_C_Tmr_1   :13;        /* Event 1: Time from frame start */
    unsigned  Phs_C_St_1    :2;         /* Event 1: state                 */
    unsigned  Phs_C_Tmr_2   :13;        /* Event 2: Time from frame start */
    unsigned  Phs_C_St_2    :2;         /* Event 2: state                 */
    unsigned  Phs_C_St_0    :2;         /* Event 0: state                 */

};

struct BtmOTFlgs
{   unsigned    UIGBT        :1;  // Upper IGBT fault or alarm flag -- bit0
    unsigned    LIGBT        :1;  // Lower IGBT fault or alarm flag -- bit1
    unsigned    UDiode       :1;  // Upper diode fault or alarm flag-- bit2
    unsigned    LDiode       :1;  // Lower diode fault or alarm flag-- bit3
};

struct BasPltOTFlgs
{   unsigned    BpPhA        :1;  // Base plate phase-A fault or alarm flag -- bit0
    unsigned    BpPhB        :1;  // Base plate phase-B fault or alarm flag -- bit1
    unsigned    BpPhC        :1;  // Base plate phase-C fault or alarm flag -- bit2
};



// Digital I/O status register


struct DigStatRgr {

    // Register 0x700004,9 : Digital status bits (read)

    unsigned  Dsat_Flt_AL   :1;         /* B0:  Desat fault in A lower device  */
    unsigned  Dsat_Flt_BL   :1;         /* B1:  Desat fault in B lower device  */
    unsigned  Dsat_Flt_CL   :1;         /* B2:  Desat fault in C lower device  */
    unsigned  Dsat_Flt_AU   :1;         /* B3:  Desat fault in A upper device  */

    unsigned  Dsat_Flt_BU   :1;         /* B4:  Desat fault in B upper device  */
    unsigned  Dsat_Flt_CU   :1;         /* B5:  Desat fault in C upper device  */
    unsigned  Ioc_Flt_A     :1;         /* B6:  IOC fault in phase A shunt     */
    unsigned  Ioc_Flt_B     :1;         /* B7:  IOC fault in phase B shunt     */

    unsigned  Ioc_Flt_C     :1;         /* B8:  IOC fault in phase C shunt     */
    unsigned  Fast_Shtdwn   :1;         /* B9:  General fault force bit        */
    unsigned  IOC_Timeout   :1;         /* B10: IOC timeout force by CPU       */
    unsigned  Vdc_OV_Flt    :1;         /* B11: Vdc OV forced by CPU           */

    unsigned  Wdog_Flt      :1;         /* B12: Macc Watchdog Toggle Flt       */
    unsigned  Spare13       :1;         /* B13: Spare                          */
    unsigned  Spare14       :1;         /* B14: Spare                          */
    unsigned  Spare15       :1;         /* B15: Spare                          */

    unsigned  Spare16       :1;         /* B16: Spare                          */
    unsigned  Spare17       :1;         /* B17: Spare                          */
    unsigned  Spare18       :1;         /* B18: Spare                          */
    unsigned  Wdog_Toggle   :1;         /* B19: MACC Watchdog Toggle Bit       */

    unsigned  Pwr_Elec_Enb  :1;         /* B20: Power electronics enable       */
    unsigned  Drv_Flt_Stat  :1;         /* B21: Latched drive fault status     */
    unsigned  DB_Pwr_Enb    :1;         /* B22: DB power enable                */
    unsigned  DB_Flt_Stat   :1;         /* B23: Latched DB fault status        */

    unsigned  Spare24       :1;         /* B24: Spare                          */
    unsigned  Spare25       :1;         /* B25: Spare                          */
    unsigned  Spare26       :1;         /* B26: Spare                          */
    unsigned  Spare27       :1;         /* B27: Spare                          */

    unsigned  Spare28       :1;         /* B28: Spare                          */
    unsigned  Spare29       :1;         /* B29: Spare                          */
    unsigned  Spare30       :1;         /* B30: Spare                          */
    unsigned  Spare31       :1;         /* B31: Spare                          */
} ;


struct AEAXDigOut
{        // Register offset - U10 11H
    //Bit no.
    unsigned    Aead_Rly1   :1;  //  0
    unsigned    Aead_Rly2   :1;  //  1
    unsigned    Aead_Rly3   :1;  //  2
    unsigned    Aead_Rly4   :1;  //  3

    unsigned    Aead_Rly5   :1;  //  4
    unsigned    Aead_Rly6   :1;  //  5
    unsigned    Aead_Rly7   :1;  //  6
    unsigned    Aead_Rly8   :1;  //  7

    unsigned    Aead_Rly9   :1;  //  8
    unsigned    Spare09     :1;  //  9
    unsigned    Spare10     :1;  //  10
    unsigned    Spare11     :1;  //  11

    unsigned    Spare12     :1;  //  12
    unsigned    Spare13     :1;  //  13
    unsigned    Spare14     :1;  //  14
    unsigned    Spare15     :1;  //  15

    unsigned    ThrmHiTmpRng:1;  //  16
    unsigned    Sprare17_26 :10; //  17-26
    unsigned    IOWdog_Toggle:1; //  27
    unsigned    TempSelVCO  :3;  //  28-30
    unsigned    IOWdog_Flt  :1;  //  31
};


struct AEAXDigStat
{        // Register offset - U10 11H
    //Bit no.
    unsigned    Aead_Rly1_St   :1;  //  0
    unsigned    Aead_Rly2_St   :1;  //  1
    unsigned    Aead_Rly3_St   :1;  //  2
    unsigned    Aead_Rly4_St   :1;  //  3

    unsigned    Aead_Rly5_St   :1;  //  4
    unsigned    Aead_Rly6_St   :1;  //  5
    unsigned    Aead_Rly7_St   :1;  //  6
    unsigned    Aead_Rly8_St   :1;  //  7

    unsigned    Aead_Rly9_St   :1;  //  8
    unsigned    Spare09_26     :18; //  9-26
    unsigned    IOWdToggle_St   :1; //  27
    unsigned    TempSelVCO_St   :3; //  28-30
    unsigned    IOWdog_St       :1; //  31
};


struct AEAXDigIn
{
    // Register Offset 11h
    //  Bit no.
    unsigned    Spare00      :1; // 0
    unsigned    Spare01      :1; // 1
    unsigned    Aead_Din3    :1; // 2
    unsigned    Aead_Din4    :1; // 3

    unsigned    Aead_Din5    :1; // 4
    unsigned    Aead_Din6    :1; // 5
    unsigned    Spare06      :1; // 6
    unsigned    Spare07      :1; // 7

    unsigned    Aead_Din9    :1; // 8
    unsigned    Aead_Din10   :1; // 9
    unsigned    Spare10      :1; // 10
    unsigned    Spare11      :1; // 11

    unsigned    Spare12      :1; // 12
    unsigned    Spare13      :1; // 13
    unsigned    Spare14      :1; // 14
    unsigned    Spare15      :1; // 15

    unsigned    Spare16      :1; // 16
    unsigned    Spare17      :1; // 17
    unsigned    Spare18      :1; // 18
    unsigned    Spare19      :1; // 19

    unsigned    Aeaa_CtrlUV  :1; // 20
    unsigned    Spare21      :1; // 21
    unsigned    Aead_CtrlUV  :1; // 22
    unsigned    Aead_RlyUV   :1; // 23

    unsigned    Spare22_31   :8; // 24-31
};


struct nvVar_tbl
{

# if  defined(BUILD_NVVAR) || defined(BUILD_PARMS) || defined(BUILD_VARS) || defined(BUILD_FAULTS)
# else
#   include "NvVar.h"
# endif

};


struct EpldCmdRgr
{
    // Register Offset 0h

    unsigned   SOF_Rst_Enb  :1;         // bit 0 - Stack over-flow hard reset enable
    unsigned   H_Reset      :1;         // bit 1 - Issue software generated hard reset
    unsigned   DSTni1_Rst   :1;         // bit 2 - Reset DSTni # 1  Can
    unsigned   DSTni2_Rst   :1;         // bit 3 - Reset DSTni # 2  Enet

    unsigned   BPPB_Rst     :1;         // bit 4 - Reset BBPB
    unsigned   BPPB_TriSt   :1;         // bit 5 - 1 = tristate BBIP Address buffers, 0 = enabled
    unsigned   Bridge_1_Enb :1;         // bit 6 - Enable bridge 1 (hardware enable anded with 30mhz wdog)
    unsigned   Bridge_2_Enb :1;         // bit 7 - Enable bridge 2 (hardware enable anded with 30mhz wdog)

    unsigned   Spare_24     :24;        // spare
};


struct CtlStsDat           // Defines Control Status Data structure
{
    CtrlStatusTyp  CtrlStatus;
};


struct DSPTachCfg
{

    unsigned    Sfb1_Gain   :4;             // Bits 00-03   Not used yet
    unsigned    Spare1      :4;             // Bits 04-07   unused
    unsigned    Sfb1_Clk_Ps :5;             // Bits 08-12   Not used yet
    unsigned    HiFi12_Mode :1;             // Bit  13      0=>VCO up ctrs, 1=> Tach up/dn
    unsigned    Tach1_Cpt   :1;             // Bit  14      0=> Clk T2, 1=> Clk T1
    unsigned    Mrkr_Rset   :1;             // Bit  15      1 = zero value in Tach 1 counter
    unsigned    Sfb4_Gain   :4;             // Bits 16-19   Not used yet
    unsigned    Spare2      :4;             // Bits 20-23   unused
    unsigned    Sfb4_Clk_Ps :5;             // Bits 24-28   Not used yet
    unsigned    HiFi45_Mode :1;             // Bit  29      0=>VCO up ctrs, 1=> Tach up/dn
    unsigned    Tach4_Cpt   :1;             // Bit  30      0=> Clk T2, 1=> Clk T1
    unsigned    Spare3      :1;             // Bit  31      unused
};


struct DiagBits_Typ {
    unsigned    TripFltBrg1   :1;  // Bit 00
    unsigned    L_SeqSt       :4;  // Bit 01
    unsigned    L_RegLimSum   :10; // Bit 05
    unsigned    TripFltBrg2   :1;  // Bit 15
    unsigned    R_SeqSt       :4;  // Bit 16
    unsigned    R_SyncTskSt   :5;  // Bit 20
    unsigned    R_CroBarSt    :2;  // Bit 25
    unsigned    S_K1Status    :1;  // Bit 27
    unsigned    R_CCOpnCmd    :1;  // Bit 28
    unsigned    L_MaStatus    :1;  // Bit 29
    unsigned    Spare_30      :1;  // Bit 30
    unsigned    Spare_31      :1;  // Bit 31
};


struct CtlBt2
{
    unsigned    R_DchgIxInt     :1;  // Discharge ix current reg. integrator
    unsigned    R_DchgIyInt     :1;  // Discharge iy current reg. integrator
    unsigned    R_DchgSlreg     :1;  // Discharge slip regulator
    unsigned    Use_Y_Prp       :1;  // Use Y current reg proportional portion

    unsigned    R_FlxUp         :1;  // Flux up
    unsigned    Tach_Alg        :1;  // Tachometer based algorithm
    unsigned    R_UseModGn      :1;  // Use modulation gain
    unsigned    R_TachErrLt     :1;  // Tach error latched

    unsigned    Use_Y_Int       :1;  // Use Y current reg integrator
    unsigned    R_TachErr       :1;  // Tach error
    unsigned    R_TacLosStat    :1;  // Tach Loss status
    unsigned    Spare_11        :1;  // Spare 12

    unsigned    Spare_12        :1;  // Spare 13
    unsigned    Spare_13        :1;  // Spare 14 - last element
};


//-----------------------------------------------------------------------------
#define  R_SeqStBits    (*((R_SeqBits1*)&R_SeqStBits_PackedBits))
#define  R_SeqStBitsT1  (*((R_SeqBits1*)&R_SeqStBitsT1_PackedBits))
#define  R_SeqStBitsT3  (*((R_SeqBits1*)&R_SeqStBitsT3_PackedBits))

#define  R_SeqStat      (*((R_SeqStatBits*)&R_SeqStat_PackedBits))
#define  R_SeqStatT1    (*((R_SeqStatBits*)&R_SeqStatT1_PackedBits))

#define  R_SeqReq       (*((R_SeqReqBits*)&R_SeqReq_PackedBits))
#define  R_SeqReqT1     (*((R_SeqReqBits*)&R_SeqReqT1_PackedBits))

#define  L_SeqReq       (*((SeqReqBits*)&L_SeqReq_PackedBits))
#define  L_SeqReqT1     (*((SeqReqBits*)&L_SeqReqT1_PackedBits))

#define  R_MctlCmd      (*((SeqMCtlBits*)&R_MctlCmd_PackedBits))
#define  R_MctlCmdT1    (*((SeqMCtlBits*)&R_MctlCmdT1_PackedBits))


#define  R_SeqStBits    (*((R_SeqBits1*)&R_SeqStBits_PackedBits))
#define  R_SeqStBitsT1  (*((R_SeqBits1*)&R_SeqStBitsT1_PackedBits))
#define  R_SeqStBitsT3  (*((R_SeqBits1*)&R_SeqStBitsT3_PackedBits))

#define  R_SeqStat      (*((R_SeqStatBits*)&R_SeqStat_PackedBits))
#define  R_SeqStatT1    (*((R_SeqStatBits*)&R_SeqStatT1_PackedBits))

#define  R_SeqReq       (*((R_SeqReqBits*)&R_SeqReq_PackedBits))
#define  R_SeqReqT1     (*((R_SeqReqBits*)&R_SeqReqT1_PackedBits))

#define  L_SeqReq       (*((SeqReqBits*)&L_SeqReq_PackedBits))
#define  L_SeqReqT1     (*((SeqReqBits*)&L_SeqReqT1_PackedBits))

#define  R_MctlCmd      (*((SeqMCtlBits*)&R_MctlCmd_PackedBits))
#define  R_MctlCmdT1    (*((SeqMCtlBits*)&R_MctlCmdT1_PackedBits))
#endif  // __PRODUCT_H__
