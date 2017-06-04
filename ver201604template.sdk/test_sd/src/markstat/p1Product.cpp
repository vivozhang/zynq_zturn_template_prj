///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rotor Side Product Level Task Drivers
//
// DESCRIPTION:
//      This file provides the top level product code task drivers for
//      the rotor side.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
#include "x0BridgeInterface.h"
#include "x0GpDiagFilter.h"
#include "x0MsgStk.h"
// product
#include "product.h"
#include "b0BridgeRebf.h"
#include "b0BrgThrmstr.h"
#include "b0BrgThrm5p.h"
#include "b0DbThrm.h"
#include "b0DbCtl.h"
#include "b0DbPlsTst.h"
#include "b0DbCelTst.h"
#include "b0CelTst.h"
#include "b0PlsTst.h"
#include "b0PwmMod2Async.h"
#include "b0PwmMod2Async.h"
#include "c1BrgCroBarCtl.h"
#include "c1BrgCurShrReg.h"
#include "c1LegTestLowFrq.h"


// Constants
//----------


// Externals
//----------
extern cBrgCroBarCtl        R_BrgCroBarCtl;

extern float                CpuCps;
extern float                DelTm1;
extern float                DelTm2;
extern float                DelTm2Rcp;
extern float                DelTm3;
extern float                DelTmQuant;
extern float                R_DelTm1;
extern float                R_DelTm1Rcp;
extern volatile int         T1AT2St;
extern int                  R_TempVcoCalcSt;

extern float                R_IaCmdLkout;
extern float                R_IbCmdLkout;
extern float                R_IcCmdLkout;
extern float                R_UaCmd;
extern float                R_UbCmd;
extern float                R_UcCmd;

extern float                GmVMagT1;
extern int                  R_SelCBVdcLvl;
extern unsigned             R_ForceCBOff;
extern unsigned             R_ForceCBFlt;
extern unsigned             R_SeqStopped;
extern unsigned             R_CBFltCond;

extern float                R_CTCurLvl;
extern float                R_VdcFbk;
extern float                R_VdcFbkRcp;
extern float                R_VdcFbkT2;
extern float                R_VdcFbkT3;
extern float                R_IxFbk;
extern float                R_IyFbk;
extern float                R_WElec;
extern float                R_HzRat;
extern float                R_UcmmCmd;
extern SinCosTyp            R_SinCosFbk;

extern float                S_Vab;
extern float                S_Vbc;
extern float                S_Ia;
extern float                S_Ib;
extern float                S_Ic;

extern float                L_VdcBrg;
extern float                DbResCombined;
extern float                BrgDbTauElec;
extern float                L_PwrDisconTm;

extern float                CoolantTemp;
extern float                R_UHalfRcp;
extern float                R_VTemp1;
extern float                R_VTemp2;
extern float                R_VTemp3;
extern float                R_VTemp4;
extern float                R_VTemp5;
extern float                R_VTemp6;
extern float                R_VTemp7;
extern float                R_VTemp8;
extern float                L_VdcFbkT2;
extern float                GmVMagPuT2;
extern float                L_VMagT2;
extern float                L_BusPwrRcp;
extern float                L_VdbTst;
extern float                L_IaBrgT2;
extern float                L_IbBrgT2;
extern float                L_IcBrgT2;
extern float                L_VnoiseLvl;
extern float                GmAngElecFbk;
extern float                GmPllWAvg;
extern float                GmPllW;
extern float                R_LT1TmDelta;
extern float                InletAirTemp;
extern float                R_IMag;

extern unsigned             TempIsValid;
extern volatile int         T1ASt;

extern unsigned             Clk0PerQuant;
extern unsigned             Clk0PerT1A;
extern unsigned             QuantsPerMod1A;
extern int                  MinPulseCnt;

extern unsigned             R_BrgFltRstCmd;
extern unsigned             DiagResetCmdT1;
extern unsigned             DiagResetCmdT3;
extern unsigned             ConvDiscnStatus;

extern uint32_t             FrameFrcT1A;
extern uint32_t             FrameFrcT1AZ;

extern unsigned             LocalMode;
extern unsigned             SimMode;
extern unsigned             XSimMode;
extern unsigned             DsimMode;

extern unsigned             R_BrgPwrEnb;
extern unsigned             R_BrgPwrEnbCmd;
extern unsigned             R_IocMask;
extern unsigned             R_DesatMask;
extern unsigned             R_InnrRegEnb;
extern unsigned             PwrDnFbkDsb;
extern unsigned             R_PwmUp;
extern unsigned             R_CelTstDisab;
extern unsigned             R_AutoCTAct;
extern unsigned             R_SeqCelTstReq;
extern unsigned             L_SeqCelTstReq;

extern unsigned             R_AeadK1Out;
extern unsigned             R_AeadK2Out;
extern unsigned             R_AeadK3Out;
extern unsigned             R_AeadK4Out;
extern unsigned             R_AeadK5Out;
extern unsigned             R_AeadK6Out;
extern unsigned             R_AeadK7Out;
extern unsigned             R_AeadK8Out;
extern unsigned             R_AeadK9Out;
extern unsigned             R_GenAeadK1Out;
extern unsigned             R_GenAeadK2Out;
extern unsigned             R_GenAeadK3Out;
extern unsigned             R_GenAeadK4Out;
extern unsigned             R_GenAeadK5Out;
extern unsigned             R_GenAeadK6Out;
extern unsigned             R_GenAeadK7Out;
extern unsigned             R_GenAeadK8Out;
extern unsigned             R_GenAeadK9Out;
extern unsigned             R_ThrmHiSel;
extern unsigned             R_InnrRegEnbT1;
extern unsigned             L_Running;
extern unsigned             InitDone;
extern unsigned             L_PulseTestReq;

extern unsigned             BrgCardsAct;
extern unsigned             L_StoppedT3;
extern unsigned             L_DiagModeT3;
extern unsigned             R_StoppedT3;
extern unsigned             TempFbkRdy;
extern unsigned             L_IMeasStat;
extern unsigned             L_RunRdy;
extern unsigned             GridFltVcomChkDsb;
extern unsigned             GmPllFrcIni;
extern unsigned             S_PllEnb;
extern unsigned             S_VzeroSeqChkEnb;
extern unsigned             DbAutoTstForce;
extern unsigned             S_K1Status;
extern float                CpuCpsRcpUsec;
extern uint64_t             FrmClkEstT1A;

extern SysTm_Typ            EventTm1;
extern SysTm_Typ            SysTm1;
extern SysTm_Typ            SysTm2;
extern SysTm_Typ            SysTm3;
extern unsigned             R_SeqReq_PackedBits;
extern unsigned             R_SeqStat_PackedBits;
extern R_SeqActBits         R_SeqAct;
extern unsigned             R_MctlCmd_PackedBits;
extern unsigned             R_SeqStBits_PackedBits;
extern cCellTest::AutoCtTyp R_AutoCelTst;
extern cBridgeRebf          L_BridgeRebf;
extern unsigned             R_PulseTestReq;

extern PhsTransListTyp      R_PhsTransListBrg1;
extern PhsTransListTyp      R_PhsDtransListBrg1;

// for R_BrgThrm5pModel
extern float                R_BpTempMaxA;
extern float                R_BpTempMaxB;
extern float                R_BpTempMaxC;
// RotorProductRules()
extern float                S_VoltsPrAmp;
// R_BrgCroBarCtl
extern float                R_ImagSqrT1;
extern unsigned             GmLineVLo;
extern unsigned             GmNSeqHi;
extern float                L_VregRef;

extern unsigned             InitDone1Sec;



// Types and Classes
//------------------
extern cDbCtl               L_DbCtl;


// Diagnostics
//------------
CREATE_DIAG(R_BrgPwrEnbUnhandTrp);                          // rotor-side BrgPwrEnb unhandled Test Mode case
CREATE_DIAG(S_VcomTrp);                                     // stator common mode (zero seq)voltage fault


// Parameters
//-----------
CREATE_PARM(R_BrgSimForce,              unsigned);          // diagnostic bridge state force

CREATE_PARM(R_HslaRebfSite,             unsigned);          // rotor bridge HSLA site on the REBF board

CREATE_PARM(R_BrInfcHdw,               unsigned);          // rotor hardware connected to Bridge_1 on REBF

CREATE_PARM(R_Io1InfcHdw,               unsigned);          // rotor hardware connected to I/O 1 on REBF

CREATE_PARM(R_Io1AttenHdw,              unsigned);          // rotor attenuator hardware(board) connected to Io1 voltage input

CREATE_PARM(S_CTburdenR,                float);             // CT burden resistor value (Ohms)
CREATE_PARM(S_CTratio,                  float);             // CT ratio

CREATE_PARM(R_CshRegEnbInh,             unsigned);          // dual rotor bridge inhibit the current sharing regulator (for analysis or test purpose)

CREATE_PARM(R_CmnRbrg,                  float);             // dual rotor bridge common mode resistance (ohms)
CREATE_PARM(R_CrsRbrg,                  float);             // dual rotor bridge cross normal mode resistance (ohms)
CREATE_PARM(R_CmnLbrg,                  float);             // dual rotor bridge common mode inductance (henries)
CREATE_PARM(R_CrsLbrg,                  float);             // dual rotor bridge cross normal mode inductance (henries)

CREATE_PARM(R_BrgPwrPrm,                unsigned);          // rotor brg power enab permissive (for analysis or test)

CREATE_PARM(DbAutoTstInh,               unsigned);          // inhibit the DB auto ping
CREATE_PARM(DbVfbkTau,                  float);             // dynamic brake voltage feedback circuit time constant
CREATE_PARM(InletToDbRise,              float);             // Air temp rise from inlet to DB resistors
CREATE_PARM(DbGateSkewEnb,              unsigned);          // enable the DB gate skew
CREATE_PARM(DbRTstPbPK,                 float);             // Db resistance test PumpBack power calc fudge factor
CREATE_PARM(DbOtThrsh,                  float);             // Db Over temperature Threshold due to DB overheating (watts)
CREATE_PARM(DbAtoPingVdcMin,            float);             // dynamic brake auot-pulse minimum dc link voltage

CREATE_PARM(S_VcomOkThrsh,              float);             // stator commond mode voltage ok ratio (to the 6K xfmr secondary nominal voltage)
CREATE_PARM(S_PllErNoise,               float);             // Pll error noise

CREATE_PARM(R_SkipFireInh,              unsigned);          // Inhibit skip fire in modulator
CREATE_PARM(R_LkoutInh,                 unsigned);          // Inhibit skip fire in modulator

CREATE_PARM(R_LkoutGnSclK,              float);             // Lockout gain scale normalization factor
CREATE_PARM(R_LkoutGnSclMax,            float);             // Lockout gain scale maximum
CREATE_PARM(R_LkoutGnSclMin,            float);             // Lockout gain scale minimum

CREATE_PARM(VcomFilW,                   float);             // common mode voltage filter W (rad/sec)
CREATE_PARM(VcomTm,                     float);             // time duration that a persistant common mode voltage trips(Sec)
CREATE_PARM(RtrDualBrg,                 unsigned);          // rotor dual bridge

CREATE_PARM(AeaaHv3Scl,                 float);
CREATE_PARM(AeaaHv3FbkT2W,              float);
CREATE_PARM(AeaaHv3DcFbkEnb,            unsigned);


CREATE_PARM(R_Rcable,           float);     // Rotor Cable resistance
CREATE_PARM(S_Rcable,           float);     // Stator Cable resistance
CREATE_PARM(R_VllOut,           float);     // Rated line-line inverter output voltage
CREATE_PARM(R_Winding,          unsigned);  // 1 - delta connected, 0 - Y connected
//Core loss parms
CREATE_PARM(R_Rfe,              float);     //Stator core loss equivalent resistance
CREATE_PARM(S_Rfe,              float);     //Stator core loss equivalent resistance
CREATE_PARM(CableLimTyp,        unsigned);  // Cable limiter type
CREATE_PARM(L_IRatSSLim,        float);     // line converter steady-state rated cur
CREATE_PARM(R_IRatSSLim,        float);     // generator steady-state rated current
CREATE_PARM(S_IRatSSLim,        float);     // stator steady-state rated cur
CREATE_PARM(R_IMaxLim,          float);     // rotor max current limit
CREATE_PARM(S_IMaxLim,          float);     // stator max current limit
CREATE_PARM(R_TocLimGn,         float);     // rotor toc limit function gain
CREATE_PARM(S_TocLimGn,         float);     // stator toc limit function gain
CREATE_PARM(TocPreset,          unsigned);  // Timed over-current preset
CREATE_PARM(R_ILimMinGn,        float);     // Rotor gain from R_MaxAmps to R_ILimMin
CREATE_PARM(S_ILimMinGn,        float);     // Stator gain from S_MaxAmps to S_ILimMin
CREATE_PARM(ILimitBW,           float);     // Current lmit band-width
CREATE_PARM(R_IMagOL,           float);     // Rotor current magnitude over-load
CREATE_PARM(S_IMagOL,           float);     // Stator current magnitude over-load
CREATE_PARM(AeaaAinZero,        float);     // Zero frequency for Aeaa VCOs

// Variables
//----------
CREATE_PUBVAR(R_VabBrg,                float);             // rotor side bridge voltages (abc)
CREATE_PUBVAR(R_VbcBrg,                float);

CREATE_PUBVAR(R_IaBrg,                 float);             // bridge 1 ph-A current (amps)
CREATE_PUBVAR(R_IbBrg,                 float);             // bridge 1 ph-B current (amps)
CREATE_PUBVAR(R_IcBrg,                 float);             // bridge 1 ph-C current (amps)

CREATE_PUBVAR(R_VdcBrg,                float);             // rotor side dc link and Db voltages
CREATE_PUBVAR(R_VdcBrgRcp,             float);
CREATE_PUBVAR(R_VdbBrg,                float);             // voltage across the DB1 resistor (volt)
CREATE_PUBVAR(R_DbTmpEst,              float);             // estimated temperature of DB resistor 1 (deg C)
CREATE_PUBVAR(R_BthtrTemp7,            float);             // Publish thermistor temp inside rotor-1 top DB module (deg C)
CREATE_PUBVAR(R_BthtrTemp8,            float);             // Publish thermistor temp inside rotor-1 bottom DB module (deg C)

CREATE_PUBVAR(S_AngElecLin,             float);

CREATE_PUBVAR(S_Vcom,                   float);             // stator common mode voltage value (volt)
CREATE_PUBVAR(S_VcomAbs,                float);             // stator common mode voltage absolute value (volt)
CREATE_PUBVAR(S_VcomFil,                float);             // stator common mode voltage filtered (volt)
CREATE_PUBVAR(S_VcomAbsThrs,            float);             // threshold for declaring excessive stator common mode voltage (volt)
CREATE_PUBVAR(S_VcomTmr,                float);             // timer to track the common mode voltage abnormal in stator (Sec)


CREATE_PUBVAR(S_Van,                    float);
CREATE_PUBVAR(S_Vbn,                    float);
CREATE_PUBVAR(S_Vcn,                    float);
CREATE_PUBVAR(S_VanT2,                  float);
CREATE_PUBVAR(S_VbnT2,                  float);
CREATE_PUBVAR(S_VcnT2,                  float);


CREATE_PUBVAR(S_VxLkgPri,               float);            // Vx of primary leakage voltage caused by stator current
CREATE_PUBVAR(S_VyLkgPri,               float);            // Vy of primary leakage voltage caused by stator current
CREATE_PUBVAR(S_VxLkgSec,               float);            // Vx of 6kv secondary leakage voltage
CREATE_PUBVAR(S_VyLkgSec,               float);            // Vy of 6kv secondary leakage voltage


CREATE_PUBVAR(S_VxLkgSecT2,             float);
CREATE_PUBVAR(S_VyLkgSecT2,             float);
CREATE_PUBVAR(S_VxLkgPriT2,             float);
CREATE_PUBVAR(S_VyLkgPriT2,             float);
CREATE_PUBVAR(AeaaHv3Fbk,               float);
CREATE_PUBVAR(AeaaHv3FbkT2,             float);
CREATE_PUBVAR(AeaaHv3FbkT2Gn,           float);
CREATE_PUBVAR(AeaaHv3FbkT2Fil,          float);

// DB
extern float DbVregFbk;
extern unsigned DbAutoTstEnb;
extern unsigned DbAutoTstEnbT1;
extern int DbInh;
extern float SclLinkVolts;
extern float VdbVcoFreq;

CREATE_PUBVAR(R_VdbOnAdj,              float) = 1.200F;    // Adjustment factor for calculating Db-1 On threshold

// Other
CREATE_PUBVAR(R_BrgTestL,               float);             // Total rotor phs-to-phs inductance for diag modes (Henrys)
CREATE_PUBVAR(R_BrgTestAltL,            float);             // Alternate inductance for diag modes (Henrys)
CREATE_PUBVAR(R_IsensorScl,             float);             // Bridge current sensor scale in Amp/Hz
CREATE_PUBVAR(R_BrgIocThrs,             float);             // Bridge current Ioc threshold (Amps)

// temporary definition here, need to use extern after DB cell test available

CREATE_PUBVAR(R_Io1VllQuant,            float);
CREATE_PUBVAR(R_VdcQuant,               float);
#if 0
CREATE_PUBVAR(R_VllQuant,               float);
CREATE_PUBVAR(R_ShuntRated,             float);

CREATE_PUBVAR(R_RegEnbStat,             unsigned) = false;

CREATE_PUBVAR(R_ContClsCmdT1,           unsigned);

CREATE_PUBVAR(R_ContEnbStatT1,          unsigned);
CREATE_PUBVAR(R_ContVfbkDetect,         unsigned);

CREATE_PUBVAR(R_BrgPwrEnbReq,           unsigned) = false;
CREATE_PUBVAR(R_BrgPwrEnbReqT1,         unsigned) = false;
CREATE_PUBVAR(R_SeqDiagModeT3,          unsigned);
CREATE_PUBVAR(R_SeqPlsTstReqT1,         unsigned);
CREATE_PUBVAR(R_SeqCelTstReqT1,         unsigned);
CREATE_PUBVAR(R_PlsTstStat,             unsigned);
CREATE_PUBVAR(R_PlsTstAct,              unsigned);
#endif

CREATE_PUBVAR(R_DualBrgEnb,             unsigned);          // enable dual bridge PWM modulation
CREATE_PUBVAR(R_CshRegEnb,              unsigned);          // enable current sharing regulator
CREATE_PUBVAR(R_CshRegEnbT1,            unsigned);          // enable current sharing regulator passed to task1

CREATE_PUBVAR(R_BrPwrPrm,              unsigned);          // Rotor Bridge 1 power enable permissive
CREATE_PUBVAR(R_BrPwrEnbCmd,           unsigned);          // Rotor Bridge 1 power enable command
CREATE_PUBVAR(R_Br2PwrEnbCmd,           unsigned);          // Rotor Bridge 2 power enable command

CREATE_PUBVAR(R_IoffSlowSw,             unsigned);
CREATE_PUBVAR(R_VoffFastSw,             unsigned);
CREATE_PUBVAR(R_IoffNullEn,             unsigned);
CREATE_PUBVAR(R_VoffNullEn,             unsigned);

CREATE_PUBVAR(R_BrgFltHdw,              unsigned);
CREATE_PUBVAR(R_DesatFlt,               unsigned);          // fpga Desat fault indication for R-bridge
CREATE_PUBVAR(R_IocFlt,                 unsigned);          // fpga IOC   fault indication for R-bridge

CREATE_PUBVAR(R_RtrLoopFltEnb,          unsigned);          // Enable assertion of rotor RtrLoop fault

// DFIG generator related variables
CREATE_PUBVAR(R_LdEq,                   float);             // d-axis transient inductance seen by the rotor-side bridge (Henrys)
CREATE_PUBVAR(R_LqEq,                   float);             // q-axis transient inductance seen by the rotor-side bridge (Henrys)

// Low frequency leg test inductance
CREATE_PUBVAR(R_LegTestLeqv,            float) = 450.0e-6F; // Leg test inductance, delta equivalent  (Henrys)


CREATE_PUBVAR(R_LkoutGnScl,             float);             // Scale factor on currents for lockout comp
CREATE_PUBVAR(R_LkoutGnSclT1,           float);

//Temporary variables
CREATE_PUBVAR(BrgMinOnTm,               float) = 0.0F;

CREATE_PUBVAR(R_DesatFlgAL,         unsigned);
CREATE_PUBVAR(R_DesatFlgAU,         unsigned);
CREATE_PUBVAR(R_DesatFlgBL,         unsigned);
CREATE_PUBVAR(R_DesatFlgBU,         unsigned);
CREATE_PUBVAR(R_DesatFlgCL,         unsigned);
CREATE_PUBVAR(R_DesatFlgCU,         unsigned);

CREATE_PUBVAR(R_IocFlgPhsA,         unsigned);
CREATE_PUBVAR(R_IocFlgPhsB,         unsigned);
CREATE_PUBVAR(R_IocFlgPhsC,         unsigned);

CREATE_PUBVAR(R_FpgaHdwFlt,         unsigned);


// Unpublished Variables
//----------------------
float              R_AvgOprVolt;      // average operational rotor voltage
unsigned           R_PlsTstReq;
unsigned           R_ChPlsTstReq;
unsigned           R_BothEnbStatus;
float              VcomFilGn;

BrgHdwOffsTyp      Io1HdwOffset;

#ifdef DSIM_MODE
int                DbInhT1;
#else
cDbCtl::DbInhTyp   DbInhT1;           // inhibit the DB to fire data-passed to task1
#endif

// Data Passing
//--------------
DATA_PASS( R_LkoutGnScl,                  R_LkoutGnSclT1,    T3_T1,  UNFILTERED );
DATA_PASS( R_CshRegEnb,                   R_CshRegEnbT1 ,    T2_T1,  UNFILTERED );
DATA_PASS( S_Van,                         S_VanT2,           T1A_T2, FILTERED );
DATA_PASS( S_Vbn,                         S_VbnT2,           T1A_T2, FILTERED );
DATA_PASS( S_Vcn,                         S_VcnT2,           T1A_T2, FILTERED );
DATA_PASS( S_VxLkgPri,                    S_VxLkgPriT2,      T1A_T2, FILTERED );
DATA_PASS( S_VyLkgPri,                    S_VyLkgPriT2,      T1A_T2, FILTERED );
DATA_PASS( S_VxLkgSec,                    S_VxLkgSecT2,      T1A_T2, FILTERED );
DATA_PASS( S_VyLkgSec,                    S_VyLkgSecT2,      T1A_T2, FILTERED );
DATA_PASS( AeaaHv3Fbk,                    AeaaHv3FbkT2,      T1A_T2, FILTERED );



// Local Prototypes (to resolve forward references)
//-------------------------------------------------
extern cBridgeRebf          R_BridgeRebf;
extern cDbThrmModel         R_DbThrmModel;
extern cBrgThrmstr          R_BrgThrmstr;


// Class Instances
//----------------

//-----------------------------------------------------------------------------
// GP diagnostic filters, a framework class instanced on behalf of product code
cGpDiagFilter::InsListTyp R_GpDiagFiltIns =
{
    cGpDiagFilter::TASK1A,          // 1 - which task 1 to execute filter
    R_DelTm1,                       // 2 - Delta time for task 1 (Sec)
};
cGpDiagFilter R_GpDiagFilter( R_GpDiagFiltIns );
INSTANCE_INFO(R_GpDiagFilter, BRG_UNIQ_R, USE_XLS, TASK1A)
// rotor pulse test
cPulseTest::InListTyp R_PlsTstIn =
{
    //R_PlsTstReq,                    //  1 - Bridge test request from product code - Task2
    R_PulseTestReq,                   //  1 - Bridge test request from product code - Task2
    R_BridgeRebf.Brg1Enabled,       //  2 - Gating enable indication from bridge under test - Task1
    TripFlt,                        //  3 - Trip fault active
};
cBridgeTest::InsListTyp R_BrgTstIns =
{
    R_DelTm1,                       //  1 - Delta time for task 1 (Sec)
    DelTm2,                         //  2 - Delta time for task 2 (Sec)
    R_DelTm1Rcp,                    //  3 - Delta time reciprocal for task 1 (1/Sec)
    Clk0PerQuant,                   //  4 - Number of Pwm Timer counts in a pulse quanta
    QuantsPerMod1A,                 //  5 - Total number of pulse quanta per scheduling
    Clk0PerT1A,                     //  6 - Total number of clock pulses per scheduling
    DelTmQuant,                     //  7 - Delta time in a quant (Sec) - cell test only
    PARM(R_ShuntRat),               //  8 - Shunt rated current (Arms) - cell test only
    R_IsensorScl,                   //  9 - Phase current sensor (lem/shunt) scale (Amp/Hz) - cell test only
    R_BrgTestL,                     // 10 - Total inductance seen between two bridge terminals (Henry)
    FZero,                          // 11 - Total capacitance seen between two bridge terminals (Farad)
    R_CTCurLvl,                     // 12 - Target current for bridge test (Amps)
    PARM(R_VllOut),                 // 13 - Nominal bridge line to line voltage (Volts)
    R_BrgTestAltL,                  // 14 - Alternate inductance seen during cell test (Henry)
    PARM(BrgLockoutTm),             // 15 - Bridge lockout time
};
cRotorPulseTest R_RotorPulseTest( R_PlsTstIn, R_BrgTstIns );
INSTANCE_INFO( R_RotorPulseTest, BRG_UNIQ_R_BR, BRG_NUM_R, TASK1A )
//cRotorPulseTest R2_RotorPulseTest( R_PlsTstIn, R_BrgTstIns );
//INSTANCE_INFO( R2_RotorPulseTest, BRG_UNIQ_R_BR2, BRG_NUM_R, TASK1A )



//-----------------------------------------------------------------------------
// rotor cell test
cCellTest::InListTyp R_CelTstIn =
{
    R_SeqCelTstReq,                  //  1 - Bridge test request from product code - Task2
    TripFlt,                         //  2 - Trip fault active
    R_AutoCelTst,                    //  3 - Type of automatic cell test running - Task2
    cMessageStack::MsgPending,       //  4 - Message pending on stack - Task2
    R_CelTstDisab,                   //  5 - Disable cell test operation - Task2
    SysTm2,                          //  6 - System time for Task2
    SysTm1,                          //  7 - System time for Task1
    R_BridgeRebf.FpgaFltLatch,       //  8 - Fpga fault has latched disabling gating - Task1
    R_BridgeRebf.Brg1DsatIoc,        //  9 - Desaturation and over-current status - Task1
    R_IaBrg,                        // 10 - PhA current feedback (amp) - Task1
    R_IbBrg,                        // 11 - PhB current feedback (amp) - Task1
    R_IcBrg,                        // 12 - PhC current feedback (amp) - Task1
    R_VabBrg,                       // 13 - Phase a-to-b voltage feedback (volt) - Task1
    R_VbcBrg,                       // 14 - Phase b-to-c voltage feedback (volt) - Task1
    FZero,                           // 15 - Dist filter Vll magnitude (volt) - Task2
    R_VdcFbkT3,                      // 16 - DC-link total voltage feedback (volt) - Task3
    SimMode,                         // 17 - Cell test not valid in present operating mode
    PARM(RtrDualBrg)                 // 18 - Rotor dual or singal bridge
};
cCellTest R_CellTest( R_CelTstIn, R_BrgTstIns );
INSTANCE_INFO( R_CellTest, BRG_UNIQ_R_BR, BRG_NUM_R, TASK1A )

//-----------------------------------------------------------------------------
// rotor bridge crowbar control
cBrgCroBarCtl::BrgCroBarCtlInTyp  R_CroBarIn =
{
    R_VdcFbk,                       //  1 - Vdc Feedback
    GmVMagT1,                       //  2 - Vac Feedback
    R_SelCBVdcLvl,                  //  3 - Select Vdc CB Levels
    R_ForceCBOff,                   //  4 - Force CB Off
    R_ForceCBFlt,                   //  5 - Force CB Fault
    R_BrgFltHdw,                    //  6 - Hardware Fault Status
    R_SeqStopped,                   //  7 - Sequencer Stopped Status
    R_IocFlt,                       //  8 - FPGA IOC Status
    R_CBFltCond,                    //  9 - CB Fault Condition
    R_ImagSqrT1,                    // 10 - Current magnitude input
    GmLineVLo,                      // 11 - Gm voltage low flag
    GmNSeqHi,                       // 12 - Gm negative sequence voltage high flag
    L_VregRef,                      // 13 - Dc link regulator reference voltage
};
cBrgCroBarCtl::BrgCroBarCtlInsTyp R_CroBarIns =
{
    R_DelTm1,                       //  1 - Task1 delta time (seconds)
    PARM(TestMode),                 //  2 - Test Mode word (enumerated)
    PARM(DbVmax),                   //  3 - Vdc DB level max
    PARM(L_VllIn),                  //  4 - Vac input nominal
    PARM(L_BrgDbEnb),               //  5 - Enable DB
    PARM(L_VdcRefMax),              //  7 - Maximum dc link voltage reference
    PARM(L_VdcOVLvl),               //  8 - DC link voltage reference
};
cBrgCroBarCtl R_BrgCroBarCtl ( R_CroBarIn, R_CroBarIns );
INSTANCE_INFO( R_BrgCroBarCtl, BRG_UNIQ_R, BRG_NUM_R, TASK1A )


//-----------------------------------------------------------------------------
// rotor current sharing regulator
cBrgCurShrReg::InListTyp R_BrgCurShrRegIn =
{
    R_IaBrg,                       //  1 - bridge 1 ph-A current (amps)
    R_IbBrg,                       //  2 - bridge 1 ph-B current (amps)
    R_IcBrg,                       //  3 - bridge 1 ph-C current (amps)
    FZero,                          //  4 - bridge 2 ph-A current (amps)
    FZero,                          //  5 - bridge 2 ph-B current (amps)
    FZero,                          //  6 - bridge 2 ph-C current (amps)
    R_VdcBrg,                      //  7 - dc bus voltage (volts)
    R_VdcFbkRcp,                    //  8 - reciprocical of dc bus voltage (volts^-1)
    R_WElec,                        //  9 - electrical frequency (rad/s)
    R_SinCosFbk,                    // 10 - sincos electrical angle (rad)
    R_CshRegEnbT1,                  // 11 - enables/disables regulator
};
cBrgCurShrReg::InsListTyp R_BrgCurShrRegIns =
{
    R_DelTm1,                       //  1 - task 1 time step (seconds)
    R_HzRat,                        //  2 - electrical frequency rated (hz)
    PARM(R_CmnRbrg),                //  3 - dual bridge common mode resistance (ohms)
    PARM(R_CrsRbrg),                //  4 - dual bridge cross normal mode resistance (ohms)
    PARM(R_CmnLbrg),                //  5 - dual bridge common mode mode inductance (henries)
    PARM(R_CrsLbrg),                //  6 - dual bridge cross normal mode inductance (henries)
    DelTm3,                         //  7 - task 3 time step (seconds)
};
cBrgCurShrReg R_BrgCurShrReg (R_BrgCurShrRegIn, R_BrgCurShrRegIns);
INSTANCE_INFO( R_BrgCurShrReg, BRG_UNIQ_R, BRG_NUM_R, TASK1A )


//-----------------------------------------------------------------------------
// rotor modulator
cPwmMod2Async::PwmMod2AsyncInTyp  R_PwmMod2AsyncIn =
{
    R_UaCmd,                        //  1 - normalized phase modulation commamds (L-L scaling)
    R_UbCmd,                        //  2
    R_UcCmd,                        //  3
    R_UcmmCmd,                      //  4 - normalized common mode mod commamds (L-L scaling)
    R_IaCmdLkout,                   //  5 - current commands for lockout compensation
    R_IbCmdLkout,                   //  6
    R_IcCmdLkout,                   //  7
    R_PwmUp,                        //  8 - true = positive-going primary transitions
    FZero,     //  9 - phase transition adjusts for parallel bridges (L-L modulation scaling)
    FZero,     // 10
    FZero,     // 11
    R_BrgCroBarCtl.CroBarAct,       // 12 - bridge crow-bar control
    R_BrgCroBarCtl.CroBarUpDn,      // 13
};
cPwmMod2Async::PwmMod2AsyncInsTyp R_PwmMod2AsyncIns =
{
    QuantsPerMod1A,                 //  1 - # of quant per frame   (UFull)
    Clk0PerQuant,                   //  2 - # of count per quantum (PwmQuant)
    R_DelTm1,                       //  3 - Task1 delta time (seconds)
    PARM(SysVoltNom),               //  4 - max voltage LL for LkoutComp (VllMax)
    PARM(R_LSigmaSt),               //  5 - equivalent inductance for LkoutComp   (henrys)
    PARM(BrgLockoutTm),             //  6 - minimum time for LkoutComp (seconds)
    PARM(R_LkoutInh),               //  7 - disable LkoutComp for Isim but enable for Xsim
    False,                          //  8 - disable common mode injection
    PARM(R_SkipFireInh),            //  9 - disable skip-fire (typically for SimMode)
    False,                          // 10 - enable calculations for dual bridge
};
cPwmMod2Async R_PwmMod2Async ( R_PwmMod2AsyncIn, R_PwmMod2AsyncIns );
INSTANCE_INFO( R_PwmMod2Async, BRG_UNIQ_R, BRG_NUM_R, TASK1A )


//-----------------------------------------------------------------------------
// rotor feedback offsets for REBF
BrgHdwOffsInTyp R_BrgHdwFbkOffsIn =
{
    R_IoffNullEn,                   //  1 - I offset null enable
    R_VoffNullEn,                   //  2 - V offset null enable - Only Rotor class
    R_IoffSlowSw,                   //  3 - Slow I offset null omega switch in task2 - Only Rotor class
    R_VoffFastSw,                   //  4 - V offset fast omega and time switch in task2 - Only Rotor class
    True,                           //  5 - Inner regulator enable - Only Line class
    PwrDnFbkDsb,                    //  6 - Power down feedback disable
};
BrgHdwOffsInsTyp R_BrgHdwFbkOffsIns =
{
    R_DelTm1,                       //  1 - Task1 delta time - Only Rotor class
    DelTm2,                         //  2 - Task2 delta time - Only Line class
    DelTm3,                         //  3 - Task3 delta time  - Only Line class
    R_DelTm1Rcp,                    //  4 - One over task1 delta time  - Only Rotor class
    SysTm2,                         //  5 - System time in task2 - Only Rotor class
    "R_",                           //  6 - Class instance uniquifier string
	DsimMode,                       //  7 - Disable voltage offset accumulation
};
cRotorBrgFbkOffs R_RotorBrgFbkOffs ( R_BrgHdwFbkOffsIn, R_BrgHdwFbkOffsIns );
INSTANCE_INFO( R_RotorBrgFbkOffs, BRG_UNIQ_R_BR, BRG_NUM_R, TASK1A )

BrgHdwOffsInTyp RIo1_HdwFbkOffsIn =
{
    R_IoffNullEn,                   //  1 - I offset null enable
    R_VoffNullEn,                   //  2 - V offset null enable - Only Rotor class
    R_IoffSlowSw,                   //  3 - Slow I offset null omega switch in task2 - Only Rotor class
    R_VoffFastSw,                   //  4 - V offset fast omega and time switch in task2 - Only Rotor class
    True,                           //  5 - Inner regulator enable - Only Line class
    PwrDnFbkDsb,                    //  6 - Power down feedback disable
};
BrgHdwOffsInsTyp RIo1_HdwFbkOffsIns =
{
    R_DelTm1,                       //  1 - Task1 delta time - Only Rotor class
    DelTm2,                         //  2 - Task2 delta time - Only Line class
    DelTm3,                         //  3 - Task3 delta time  - Only Line class
    R_DelTm1Rcp,                    //  4 - One over task1 delta time  - Only Rotor class
    SysTm2,                         //  5 - System time in task2 - Only Rotor class
    "S_",                           //  6 - Class instance uniquifier string
    DsimMode,                       //  7 - Disable voltage offset accumulation
};
cRotorBrgFbkOffs RIo1_RotorBrgFbkOffs ( RIo1_HdwFbkOffsIn, RIo1_HdwFbkOffsIns );
INSTANCE_INFO( RIo1_RotorBrgFbkOffs, BRG_UNIQ_R_IO1, BRG_NUM_R, TASK1A )

//-----------------------------------------------------------------------------
// rotor REBF bridge interface
cBridgeRebf::InTyp R_BridgeRebfIn =
{
    R_BrPwrEnbCmd,                  //  1 - bridge 1 enable request
    R_PhsTransListBrg1,             //  2 - bridge 1 phase transition list
    R_PhsDtransListBrg1,            //  3 - bridge 1 phase D transition list
    FrameFrcT1A,                    //  4 - frc time for the upcoming frame boundary
    FrameFrcT1AZ,                   //  5 - frc time for the current frame boundary
    SysTm3,                         //  6 - system time updated at Task3 rate
    EventTm1,                       //  7 - EventTm for Task1
    R_BrgFltRstCmd,                 //  8 - signal to reset latched faults in Task1
    DiagResetCmdT3,                 //  9 - signal to reset latched faults in Task3
    R_AeadK1Out,                    // 10 - signal to drive output relay 1 on IO 1
    R_AeadK2Out,                    // 11 - signal to drive output relay 2 on IO 1
    R_AeadK3Out,                    // 12 - signal to drive output relay 3 on IO 1
    R_AeadK4Out,                    // 13 - signal to drive output relay 4 on IO 1
    R_AeadK5Out,                    // 14 - signal to drive output relay 5 on IO 1
    R_AeadK6Out,                    // 15 - signal to drive output relay 6 on IO 1
    R_AeadK7Out,                    // 16 - signal to drive output relay 7 on IO 1
    R_AeadK8Out,                    // 17 - signal to drive output relay 8 on IO 1
    R_AeadK9Out,                    // 18 - signal to drive output relay 9 on IO 1
    R_RotorBrgFbkOffs,              // 19 - external bridge 1 feedback offset class
    RIo1_RotorBrgFbkOffs,           // 20 - external IO 1 feedback offset class
    False,                          // 21 - fire Db1 request
    R_ThrmHiSel,                    // 22 - 1=select High range for thermistor sensing, 0 = lo
    FZero,                          // 23 - Fan#1 speed reference: <32 off, 32->255 0-100%
    FZero,                          // 24 - Fan#2 speed reference: <32 off, 32->255 0-100%
    R_IocMask,                      // 25 - Mask A-B-C IOC  faults from setting fpga fault latch
    False,                          // 26 - Enable fpga turn off of phase(s) with hi current
    R_DesatMask,                    // 27 - Mask A-B-C DSAT faults from setting fpga fault latch
    L_BridgeRebf.HsslCommFlt,       // 28 - Disable brg ready loop FO Tx
    R_RtrLoopFltEnb,                // 29 - Allow assertion of RtrLoop fault
    False,                          // 30 - signal to drive low-voltage output relay 01 on Rebf board
    False,                          // 31 - signal to drive low-voltage output relay 02 on Rebf board
    False,                          // 32 - signal to drive low-voltage output relay 03 on Rebf board
    FrmClkEstT1A,                   // 33 - time of next Task1 frame (cpu clock counts)
    L_BridgeRebf.DisCmdNotFlg,      // 34 - Disable bridge request not working
    T1AT2St,                        // 35 - T1 state within a T3 frame
    InitDone1Sec,                   // 36 - Flag to indicate 1.0 second done after initialization
};
cBridgeRebf::InsTyp R_BridgeRebfIns =
{
    R_DelTm1,                       //  1 - task1 period (sec)
    R_DelTm1Rcp,                    //  2 - task1 frequency (tasks/sec)
    CpuCps,                         //  3 - cpu cycles per second
    SimMode,                        //  4 - internal simulator mode
    XSimMode,                       //  5 - external simulator mode
        DsimMode,                       //  6  - dll simulator mode
    False,                          //  6 - configure parms for alternate(SEP,etc) operation
    "R_",                           //  7 - class instance uniquifier string
    False,                          //  8 - bridge 1 phase-D DSAT fault detection enable
    PARM(R_BrInfcHdw),              //  9 - hardware connected to Bridge_1 connector
    PARM(R_ShuntRat),               // 10 - bridge 1 shunt current at 50 mV
    PARM(R_PhDShuntRat),            // 11 - phase-D shunt current at 50 mV
    PARM(R_Io1InfcHdw),             // 12 - hardware connected to I/O 1 connector
    PARM(R_Io1AttenHdw),            // 13 - attenuator board connected I/O 1 voltage(low or high voltage input
    PARM(S_CTratio),                // 14 - CT ratio for I/O 1 current feedback
    PARM(BrgLockoutTm),             // 15 - gating lock out time (seconds)
    QuantsPerMod1A,                 // 16 - number of quants per task1 frame
    Clk0PerQuant,                   // 17 - number of clock-tics (or counts) in one quant
    Clk0PerT1A,                     // 18 - FRC counts in task 1 frame
    PARM(R_HslaRebfSite),           // 19 - HSLA site on the REBF board
    PARM(TestMode),                 // 20 - test mode
    False,                          // 21 - Select Line converter fpga behavior
    DelTm3,                         // 22 - task3 period (sec)
    True,                           // 23 - Io1 relay pwr supply check inhibit request
    CpuCpsRcpUsec,                  // 24 - Cpu clock period (usec)
    R_TempVcoCalcSt,                // 25 - Vco temperature calculation state
        PARM(R_TachPPR),                // 27 - Number of tachometer pulses per revolution
};
cBridgeRebf R_BridgeRebf ( R_BridgeRebfIn, R_BridgeRebfIns );
INSTANCE_INFO( R_BridgeRebf, BRG_UNIQ_R, BRG_NUM_R, TASK1A )


//-----------------------------------------------------------------------------
// enhanced rotor bridge thermal models
cBrgThrm5pModel::InListTyp R_BrgThrm5pModelIn =
{
    R_VdcFbkT3,                         //  1 - Dc link voltage fbk (Volts) - Task3
    CoolantTemp,                        //  2 - Coolant temperature (degC) - Task3
    R_IaBrg,                            //  3 - Phase-A current feedback (Amps) - Task1
    R_IbBrg,                            //  4 - Phase-B current feedback (Amps) - Task1
    R_IcBrg,                            //  5 - Phase-C current feedback (Amps) - Task1
    R_PwmMod2Async.ModUaCmdM,           //  6 - Phase A modulation output with one task latency (Pu) - Task1
    R_PwmMod2Async.ModUbCmdM,           //  7 - Phase B modulation output with one task latency (Pu) - Task1
    R_PwmMod2Async.ModUcCmdM,           //  8 - Phase C modulation output with one task latency (Pu) - Task1
    TempIsValid,                        //  9 - Reference temperature fbk is valid (equal to 1) - Task3
    R_BrPwrEnbCmd,                     // 10 - Bridge power enable - Task1
    R_BridgeRebf.Brg1DsatIoc.IocPhsA,   // 11 - Fpga phase-A ioc fault indication - Task1
    R_BridgeRebf.Brg1DsatIoc.IocPhsB,   // 12 - Fpga phase-B ioc fault indication - Task1
    R_BridgeRebf.Brg1DsatIoc.IocPhsC,   // 13 - Fpga phase-C ioc fault indication - Task1
    R_BpTempMaxA,                       // 14 - Baseplate temperature for devices of PhsA (degC) - Task3
    R_BpTempMaxB,                       // 15 - Baseplate temperature for devices of PhsB (degC) - Task3
    R_BpTempMaxC,                       // 16 - Baseplate temperature for devices of PhsC (degC) - Task3
};
cBrgThrm5pModel::InsListTyp R_BrgThrm5pModelIns =
{
    DelTm2,                         //  1 - Task 2 time (Secs) - Rules
    DelTm3,                         //  2 - Delta time for task 3 (Sec) - Rules
    R_DelTm1Rcp,                    //  3 - Delta time reciprocal for task 1 (1/Sec) - Task3
    R_BrgIocThrs,                   //  4 - Ioc detection threshold (Amps)
    R_UHalfRcp,                     //  5 - Reciprocal of half quants per task1 (1/Quant) - Rules
    R_PwmMod2Async.ModPwmN,         //  6 - Number of PWM pulses per task1 - Task3
    PARM(SoftwareIPN),              //  7 - Configure parms for proper bridge type
    PARM(TestMode),                 //  8 - Converter Test mode - Rules
    True,                           //  9 - Parallel IGBTs for 'true'

};
cBrgThrm5pModel R_BrgThrm5pModel ( R_BrgThrm5pModelIn, R_BrgThrm5pModelIns);
INSTANCE_INFO( R_BrgThrm5pModel, BRG_UNIQ_R_BR, BRG_NUM_R, TASK1A )








//-----------------------------------------------------------------------------
// rotor bridge thermistor processing
cBrgThrmstr::InListTyp R_BrgThrmstrIn =
{
    R_VTemp1,                      //  1 - voltage at thermistor input 1
    R_VTemp2,                      //  2 - voltage at thermistor input 2
    R_VTemp3,                      //  3 - voltage at thermistor input 3
    R_VTemp4,                      //  4 - voltage at thermistor input 4
    R_VTemp5,                      //  5 - voltage at thermistor input 5
    R_VTemp6,                      //  6 - voltage at thermistor input 6
    R_VTemp7,                      //  7 - voltage at thermistor input 7
    R_VTemp8,                      //  8 - voltage at thermistor input 8
    TempFbkRdy,                    //  9 - Temperature feedbacks are valid
    True,                          // 10 - Temperature ready permissive
    R_StoppedT3,                   // 11 - Stopped state indication
};
cBrgThrmstr::InsListTyp R_BrgThrmstrIns =
{
    False,                         //  1 - Enable DB thermistor diagnostics
    DelTm3,                        //  2 - Task3 delta time
    R_BthtrTemp7,                  //  3 - Ref to nonconstant data for optional thermistor 7
    R_BthtrTemp8,                  //  4 - Ref to nonconstant data for optional thermistor 8
    True,                          //  5 - Parallel IGBTs for 'true'
    "R_",                          //  6 - Class instance uniquifier string
};
cBrgThrmstr R_BrgThrmstr ( R_BrgThrmstrIn, R_BrgThrmstrIns );
INSTANCE_INFO( R_BrgThrmstr, BRG_UNIQ_R_BR, BRG_NUM_R, TASK1A )

//-----------------------------------------------------------------------------
// leg test low-frq control
cLegTestLowFrq::InListTyp R_LegTstLwfIn =
{
    R_IxFbk,                        // 1 X-axis current feedback
    R_IyFbk,                        // 2 Y-axis current feedback
    R_VdcFbk,                       // 3 Dc link voltage feedback
    R_InnrRegEnbT1,                 // 4 Regulator enable request
};
cLegTestLowFrq::InsListTyp R_LegTstLwfIns =
{
    R_LegTestLeqv,                  // 1 Inductance between phases, open-delta equivalent (henry)
    R_DelTm1,                       // 2 Task1 delta time
    DelTm3,                         // 3 Task3 delta time
    PARM(ConfigForSep),             // 4 Configure parms for Sep operation
};
cLegTestLowFrq Rx_LegTestLowFrq ( R_LegTstLwfIn, R_LegTstLwfIns );
INSTANCE_INFO( Rx_LegTestLowFrq, BRG_UNIQ_R, BRG_NUM_R, TASK1A )


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side task 1 control functions
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorControlT1(void)
{
    R_BrPwrEnbCmd =  R_BrPwrPrm && R_BrgPwrEnbCmd;

    // DB control
    if (R_SeqStBits.R_DiagMode )
    {
    }
    else
    {
    }

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side task 2 control functions
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorControlT2(void)
{
    R_PulseTestReq = R_SeqReq.R_PlsTstReq;

    R_ChPlsTstReq   = R_SeqReq.R_ChPlsTstReq;

    R_IoffSlowSw = R_MctlCmd.R_IoffSlowSw;
    R_VoffFastSw = R_MctlCmd.R_VoffFastSw;
    R_IoffNullEn = R_MctlCmd.R_IoffNullEn;
    R_VoffNullEn = R_MctlCmd.R_VoffNullEn;
    R_CshRegEnb  = R_InnrRegEnb && R_DualBrgEnb && (!PARM(R_CshRegEnbInh));


    // Decide inhibit the DB control, if there are not DB circuits enabled

    //-------------------------------------------------------------------------
    // zero sequence detection on the stator voltage
    S_Vcom = S_VanT2 + S_VbnT2 + S_VcnT2;
    S_VcomAbs = ABS(S_Vcom);
    // Filter
    S_VcomFil += VcomFilGn * (S_VcomAbs - S_VcomFil);

    // Mask the common mode voltage check during grid event and xfmr saturation due to grid event
    if ( S_VzeroSeqChkEnb && !GridFltVcomChkDsb && (S_VcomFil > S_VcomAbsThrs) )
    {
        // prior to K1 close, use a fast check
        if ( !S_K1Status )
        {
            PUSH_DIAG(S_VcomTrp);
        }
        // after K1 closed, use a slow check
        else
        {
            // avoid push the com mode trip upon one glitch. push a trip if the error is persistant after 5 seconds
            S_VcomTmr += DelTm2;
            if (S_VcomTmr >= PARM(VcomTm))
            {
                S_VcomTmr= PARM(VcomTm);
                PUSH_DIAG(S_VcomTrp);
            }
        }
    }
    // decrease the timer
    else
    {
        S_VcomTmr -= DelTm2;
        if (S_VcomTmr <=0.0F)
        {
            S_VcomTmr = 0.0F;
        }
    }






    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side task 3 control functions
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorControlT3(void)
{
    float Tempf;

    //-------------------------------------------------------------------------
    // Lockout gain scale
    Tempf = MAX(1.0F, ABS(R_WElec)*R_IMag);  // avoid divide by zero

    Tempf = MIN(PARM(R_LkoutGnSclMax), PARM(R_LkoutGnSclK) / Tempf);

    R_LkoutGnScl = MAX(PARM(R_LkoutGnSclMin), Tempf);

    // Wait for line-side HSSL to get valid data before allowing rotor RtrLoop fault
    if (  L_BridgeRebf.BrgHdwDataVld )    R_RtrLoopFltEnb = true;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side task 1 instrumentation functions
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorInstrumentationT1(void)
{
    float   LxfmDt;

    if (PwrDnFbkDsb && !R_BrgPwrEnbCmd)
    {
        //-------------------------------------------------------------------------
        // Zero the feedbacks signals if power going down. (2nd condition should be redundant.)
        //  Don't zero Vdc in case used for db or for drving PwrDnFbkDsb.
        R_VabBrg   = 0.0F;
        R_VbcBrg   = 0.0F;
        R_IaBrg    = 0.0F;
        R_IbBrg    = 0.0F;
        R_IcBrg    = 0.0F;

        S_Van       = 0.0F;
        S_Vbn       = 0.0F;
        S_Vcn       = 0.0F;
        S_Vab       = 0.0F;
        S_Vbc       = 0.0F;

        S_Ia        = 0.0F;
        S_Ib        = 0.0F;
        S_Ic        = 0.0F;

    }
    else
    {
        //-------------------------------------------------------------------------
        // get the feedbacks signals from the bridge interface
        R_VabBrg   = R_BridgeRebf.VabBrg1Hdw;
        R_VbcBrg   = R_BridgeRebf.VbcBrg1Hdw;
        R_IaBrg    = R_BridgeRebf.IaBrg1Hdw;
        R_IbBrg    = R_BridgeRebf.IbBrg1Hdw;
        R_IcBrg    = R_BridgeRebf.IcBrg1Hdw;

        if ( PARM(SoftwareIPN) <  DFIG02_1850_5060_E)
        {
            S_Vab   = R_BridgeRebf.VaxIo1;
            S_Vbc   = R_BridgeRebf.VbxIo1;
            S_Van   = ( 2.0F/3.0F) * S_Vab + ( 1.0F/3.0F) * S_Vbc;
            S_Vbn   = (-1.0F/3.0F) * S_Vab + ( 1.0F/3.0F) * S_Vbc;
            S_Vcn   = -S_Van -S_Vbn;
            AeaaHv3Fbk = (R_BridgeRebf.VcxIo1 + (R_BridgeRebf.VcxOffset * PARM(AeaaHv3DcFbkEnb))) * PARM(AeaaHv3Scl);
        }
        else
        {
           S_Van   = R_BridgeRebf.VaxIo1;
           S_Vbn   = R_BridgeRebf.VbxIo1;
           S_Vcn   = R_BridgeRebf.VcxIo1;
           S_Vab   = S_Van - S_Vbn;
           S_Vbc   = S_Vbn - S_Vcn;
        }
        S_Ia    = R_BridgeRebf.IaIo1;
        S_Ib    = R_BridgeRebf.IbIo1;
        S_Ic    = R_BridgeRebf.IcIo1;
    }

    R_VdcBrg   = R_BridgeRebf.VdBrg1Hdw;

    R_VdcBrgRcp= NoNANLim( 1.0F/R_VdcBrg, 1.0e6F, -1.0e6F );

    // Compensate for transformer leakage inductance
    LxfmDt = 0.0F;

    // Combine fpga-enable status indications from parallel bridges
    R_BothEnbStatus = R_BridgeRebf.Brg1Enabled;

    // fpga fault indications for the bridge
    R_BrgFltHdw = R_BridgeRebf.FpgaFltLatch;

    R_DesatFlgAL = R_BridgeRebf.Brg1DsatIoc.DsatSwAL;
    R_DesatFlgBL = R_BridgeRebf.Brg1DsatIoc.DsatSwBL;
    R_DesatFlgCL = R_BridgeRebf.Brg1DsatIoc.DsatSwCL;

    R_DesatFlgAU = R_BridgeRebf.Brg1DsatIoc.DsatSwAU;
    R_DesatFlgBU = R_BridgeRebf.Brg1DsatIoc.DsatSwBU;
    R_DesatFlgCU = R_BridgeRebf.Brg1DsatIoc.DsatSwCU;

    R_IocFlgPhsA = R_BridgeRebf.Brg1DsatIoc.IocPhsA;
    R_IocFlgPhsB = R_BridgeRebf.Brg1DsatIoc.IocPhsB;
    R_IocFlgPhsC = R_BridgeRebf.Brg1DsatIoc.IocPhsC;



    if ( R_DesatFlgAL || R_DesatFlgBL ||
         R_DesatFlgCL || R_DesatFlgAU ||
         R_DesatFlgBU || R_DesatFlgCU )
    {
        R_DesatFlt = true;
    }
    else
    {
        R_DesatFlt = false;
    }

    if (   R_IocFlgPhsA || R_IocFlgPhsB || R_IocFlgPhsC )
    {
        R_IocFlt = true;
    }
    else
    {
        R_IocFlt = false;
    }

    R_FpgaHdwFlt = R_DesatFlt || R_IocFlt;


    /* Compensate (690v) line angle (T1) for difference in elapsed time to T1A */
    S_AngElecLin = GmAngElecFbk + R_LT1TmDelta * GmPllWAvg;



    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side task 1 instrumentation functions
//
// EXECUTION LEVEL
//      Task1       - note: this function should be called after S_GmIx(y)Fbk
//                          have been calculated.
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorInstrumentation2T1(void)
{



    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side task 2 instrumentation functions
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorInstrumentationT2(void)
{
    AeaaHv3FbkT2Fil += AeaaHv3FbkT2Gn * (AeaaHv3FbkT2 - AeaaHv3FbkT2Fil);
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side task 3 instrumentation functions
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorInstrumentationT3(void)
{
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor side rule calculations
//
// EXECUTION LEVEL
//      Init/Rule
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
int RotorProductRules(void)
{
    int Status = SUCCESS;

    //-------------------------------------------------------------------------
    // include the product specific rules
    #include "p1ProductRule.inc"


    //-------------------------------------------------------------------------
    // run the class rules
    if ( R_BridgeRebf.RunRules()        == FAIL )  Status = FAIL;

    R_Io1VllQuant = SQRT3 * R_BridgeRebf.WhatIsIOVoltageQuant(1);
    R_VdcQuant    = R_BridgeRebf.WhatIsVdcQuant(2);
    R_IsensorScl  = R_BridgeRebf.WhatIsSclCurrent(1);  // Bridge 1 current scale in Amp/Hz, a bridge 2
                                                       // version will be needed if both values are different
    R_BrgIocThrs = R_BridgeRebf.WhatIsIocCurrent(1);   // Bridge 1 current Ioc threshold in Amps, a bridge 2
                                                       // version will be needed if both values are different

    if ( R_CellTest.RunRules()         == FAIL )  Status = FAIL;
    if ( R_RotorPulseTest.RunRules()   == FAIL )  Status = FAIL;
    if ( R_BrgCroBarCtl.RunRules()      == FAIL )  Status = FAIL;
    if ( R_PwmMod2Async.RunRules()      == FAIL )  Status = FAIL;
    if ( R_BrgThrm5pModel.RunRules()   == FAIL )  Status = FAIL;
    if ( Rx_LegTestLowFrq.RunRules()    == FAIL )  Status = FAIL;
    if ( R_BrgThrmstr.RunRules()       == FAIL )  Status = FAIL;
    if ( cBrgThrmstr::RunRules(DelTm3, PARM(WTempIoThrm)) == FAIL )  Status = FAIL;

    return (Status);
}

