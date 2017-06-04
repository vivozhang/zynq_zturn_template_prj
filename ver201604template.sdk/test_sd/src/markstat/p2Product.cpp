///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Line Side Product Level Task Drivers
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
#include "s0SimPlant1x.h"


// Constants
//----------


// Externals
//----------
extern float                CpuCps;
extern float                DelTm2;
extern float                DelTm3;
extern float                DelTmQuant;
extern float                L_DelTm1;
extern float                L_DelTm1Rcp;
extern volatile int         T1T2St;
extern int                  L_TempVcoCalcSt;

extern float                L_IaCmdLkout;
extern float                L_IbCmdLkout;
extern float                L_IcCmdLkout;
extern float                L_UaCmd;
extern float                L_UbCmd;
extern float                L_UcCmd;
extern float                L_VMagT2;
extern float                L_CTCurLvl;
extern float                L_VdcFbkT3;
extern float                L_UHalfRcp;
extern float                L_VTemp1;
extern float                L_VTemp2;
extern float                L_VTemp3;
extern float                L_VTemp4;
extern float                L_VTemp5;
extern float                L_VTemp6;
extern float                L_VTemp7;
extern float                L_VTemp8;
extern float                L_IMagT3;
extern float                BrgDbTauElec;
extern float                CoolantTemp;

extern unsigned             Clk0PerQuant;
extern unsigned             Clk0PerT1;
extern unsigned             QuantsPerMod;

extern unsigned             DiagResetCmdT1;
extern unsigned             DiagResetCmdT3;

extern uint32_t             FrameFrc;
extern uint32_t             FrameFrcZ;

extern unsigned             SimMode;
extern unsigned             XSimMode;
extern unsigned             DsimMode;

extern unsigned             L_BrgPwrEnb;
extern unsigned             L_BrgPwrEnbCmd;
extern unsigned             L_IocMask;
extern unsigned             L_InnrRegEnbT3;
extern unsigned             PwrDnFbkDsb;

extern unsigned             L_PulseTestReq;
extern unsigned             L_Running;
extern unsigned             L_IOffsEnb;
extern unsigned             L_PwmUp;
extern unsigned             L_StoppedT3;
extern unsigned             TempFbkRdy;
extern unsigned             DTAFanEnb;

extern float                GmVan;
extern float                GmVbn;
extern float                GmVcn;
extern float                GmVabT2;
extern float                GmVbcT2;
extern float                GmIa;
extern float                GmIb;
extern float                GmIc;
extern float                GmIxFbk;
extern float                GmIyFbk;
extern float                GmVxFbk;
extern float                GmVyFbk;
extern float                GmPllW;
extern float                GmVMagT2;
extern float                GmLxfmDt;

extern unsigned             L_AeadK1Out;
extern unsigned             L_AeadK2Out;
extern unsigned             L_AeadK3Out;
extern unsigned             L_AeadK4Out;
extern unsigned             L_AeadK5Out;
extern unsigned             L_AeadK6Out;
extern unsigned             L_AeadK7Out;
extern unsigned             L_AeadK8Out;
extern unsigned             L_AeadK9Out;
extern unsigned             L_RebfLvK1Out;
extern unsigned             L_RebfLvK2Out;
extern unsigned             L_RebfLvK3Out;
extern unsigned             L_ThrmHiSel;
extern unsigned             L_CelTstDisab;
extern unsigned             L_SeqCelTstReq;
extern unsigned             HdwTstMode;
extern unsigned             TempIsValid;
extern unsigned             L_Stopped;
extern unsigned             Q1OpnReq;

extern float                BrgMinOnTm;
extern float                VcomFilGn;
extern float                L_VllInRcp;

extern float                CpuCpsRcpUsec;
extern uint64_t             FrmClkEstT1;
extern SysTm_Typ            EventTm1;
extern SysTm_Typ            SysTm1;
extern SysTm_Typ            SysTm2;
extern SysTm_Typ            SysTm3;

extern cSimPlant1x          SimPlant1x;

extern cCellTest::AutoCtTyp L_AutoCelTst;
extern cBridgeRebf          R_BridgeRebf;
extern unsigned             L_SeqReq_PackedBits;
extern PhsTransListTyp      L_PhsTransListBrg1;
extern PhsTransListTyp      L_PhsDtransListBrg1;

// for L_DbPlsTst
extern unsigned             TripFlt;        // Trip Fault flag
extern unsigned             L_DbPlsTstReq;  // DB pulse test request
extern float                L_DelTm1;       // Line task 1 delta time

// for L_DbCelTst
extern unsigned             TripFlt;        // Trip Fault flag
extern unsigned             L_DbCelTstReq;  // DB cell test request
extern float                L_IaBrgT2;      // Task 2 version of line side phase A current
extern float                L_IbBrgT2;      // Task 2 version of line side phase B current
extern float                L_IcBrgT2;      // Task 2 version of line side phase B current
extern float                L_BusPwrRcp;    // Reciprocal of line bridge bus power
extern unsigned             L_RunRdy;       // Source is ready to run
extern unsigned             L_IMeasStat;    // Line phase current measure status
extern float                L_VdcFbkT2;     // Task 2 version of line side DC link voltage
extern float                L_VMagT2;       // Task 2 version of line side voltage magnitude
extern float                GmVMagPuT2;     // Task 2 version of grid monitor voltage magnitude per unit
extern float                L_VdbTst;       // DB voltage feedback during test
extern float                DelTm2;         // Task 2 delta time
extern unsigned             Q1Status;

// for L_DbThrmModel
extern float                L_VdbBrg;       // Line side dc voltage across DB resistor
extern float                L_VdcBrg;       // Line side DC link voltage
extern float                CoolantTemp;    // liquid temperature
extern unsigned             ConvDiscnStatus;// Disconnection status of main converter contactors
extern unsigned             L_StoppedT3;    // Task 3 version of line sequencer stop state
extern unsigned             L_DiagModeT3;   // Task 3 version of line diagnostic mode state
extern int                  L_DbRefTemp;    // Reference temp for DB model (coolant=0 and module sensor =1)
extern float                L_DelTm1;       // Line task 1 time
extern float                L_DelTm1Rcp;    // Reciprocal of line task 1 time
extern float                DelTm2;         // Task 2 time
extern float                DelTm3;         // Task 3 time
extern float                L_VdbOnAdj;     // Adjustment factor for calculating Db On threshold
extern float                L_PwrDisconTm;  // line bridge power disconnection time

// for L_DbCtl
#ifdef DSIM_MODE
extern int                  DbInhT1;
#else
extern cDbCtl::DbInhTyp     DbInhT1;           // inhibit the DB to fire data-passed to task1
#endif

extern unsigned             DbAutoTstEnbT1; // enable the auto DB test task1
extern float                L_VdcFbk;
extern unsigned             DbAutoTstForce;
extern unsigned             BrgCardsAct;
extern unsigned             L_BrgFltHdw;
extern float                SclLinkVolts;
extern float                DbResCombined;

// for LineControlT1
extern unsigned             L_DiagMode;
extern float                GmSVMtchErSq;
extern float                GmSVmtchThr;

// for LineControlT2
extern SeqStatBits          L_SeqStat;
extern SeqActBits           L_SeqAct;
extern unsigned             L_DbGateDsb;
extern unsigned             InitDone;
extern unsigned             L_DbAutoTstDone;

// for LineInstrumentationT1
extern float                VdbSim;

extern unsigned             Ps24vOk;
extern unsigned             SimAcclAct;

extern unsigned             InitDone1Sec;


// Types and Classes
//------------------


// Diagnostics
//------------
CREATE_DIAG(L_BrgPwrEnbUnhandTrp);                  // line-side BrgPwrEnb unhandled Test Mode case
CREATE_DIAG(GmVcomTrp);                             // 690 xfmr common mode (zero seq)voltage fault


// Parameters
//-----------

CREATE_PARM(L_HslaRebfSite,     unsigned);  // line bridge HSLA site on the REBF board
CREATE_PARM(L_BrInfcHdw,        unsigned);  // line hardware connected to Bridge_1 on REBF
CREATE_PARM(L_Io1InfcHdw,       unsigned);  // line hardware connected to I/O 1 on REBF
CREATE_PARM(L_Io1AttenHdw,      unsigned);  // line attenuator hardware(board) connected to Io1 voltage input

CREATE_PARM(GmCTburdenR,           float);  // Gm CT burden resistor value
CREATE_PARM(GmCTratio,             float);  // Gm CT ratio
CREATE_PARM(GmVcomOkThrsh,         float);  // 690 GM common mode voltage ok ratio (to the 690 xfmr secondary nominal voltage)
CREATE_PARM(GmVcomDlyTm,           float);  // time to delay checking of common mode voltage in xfmr 690V winding (Sec)

CREATE_PARM(L_SkipFireInh,      unsigned);  // Inhibit skip fire in modulator
CREATE_PARM(L_LkoutInh,         unsigned);  // Inhibit skip fire in modulator

CREATE_PARM(L_BpTempNom1,          float);  // Nominal baseplate #1 rise temp for sim mode
CREATE_PARM(L_BpTempNom2,          float);  // Nominal baseplate #2 rise temp for sim mode
CREATE_PARM(L_BpTempNom3,          float);  // Nominal baseplate #3 rise temp for sim mode
CREATE_PARM(L_BpTempNom4,          float);  // Nominal baseplate #4 rise temp for sim mode
CREATE_PARM(L_BpTempNom5,          float);  // Nominal baseplate #5 rise temp for sim mode
CREATE_PARM(L_BpTempNom6,          float);  // Nominal baseplate #6 rise temp for sim mode

CREATE_PARM(L_RChg,                float);  // Bridge charge resistor
CREATE_PARM(L_CBrg,                float);  // Bridge capacitance (Farads)
CREATE_PARM(L_RBleedCap,           float);  // Dc link Bleed resistor
CREATE_PARM(L_IRat,                float);  // Rated current
CREATE_PARM(L_ShuntRat,            float);  // Rated shunt, amps at 100 mV

CREATE_PARM(L_BrgDbEnb,         unsigned);  // DB-2 circuit enable for operation
CREATE_PARM(L_ParallelIGBT,     unsigned);  // Parallel IGBTs
CREATE_PARM(GenAeadPresent,     unsigned);  // Generator AEAD present

CREATE_PARM(L_LkoutGnSclK,         float);  // Lockout gain scale normalization factor
CREATE_PARM(L_LkoutGnSclMax,       float);  // Lockout gain scale maximum
CREATE_PARM(L_LkoutGnSclMin,       float);  // Lockout gain scale minimum

CREATE_PARM(GmIfbkShftTm,          float);  // 690v cur fbk shift time (lag, in sec)

CREATE_PARM(GridVoltOkPuLvlLo,     float);  // grid voltage Ok low level to have the common mode voltage check enabled(Pu)
CREATE_PARM(GridVoltOkPuLvlHi,     float);  // grid voltage Ok high level to have the common mode voltage check enabled(Pu)
CREATE_PARM(GridVoltLowTm,         float);  // max time that grid voltage is around the zero crossing with the common mode voltage check enabled (sec)
CREATE_PARM(GridVoltHiTm,         float);   // max time that grid voltage is high with the common mode voltage check enabled (sec)
CREATE_PARM(GridVoltOkTm,          float);  // max time that grid voltage stays above the Ok level with the common mode voltage check disabled (sec)
CREATE_PARM(XfmrSatDampTm,         float);  // time for xfmr saturation to damp after grid event (sec)

CREATE_PARM(L_DbFanOnThr,          float);  // DB fan request on threshold
CREATE_PARM(L_DbFanOffThr,         float);  // DB fan request off threshold
CREATE_PARM(L_Q1OpnTvMin,          float);  // Minimum allowed AC line magnitude for allowing Q1 open test

CREATE_PARM(GmVanVcoSc,         float);     // Gm feedback Van vco scale
CREATE_PARM(GmIaVcoScl,         float);     // Gm feedback Ia vco scale
CREATE_PARM(GmIbVcoScl,         float);     // Gm feedback Ib vco scale
CREATE_PARM(GmIcVcoScl,         float);     // Gm feedback Ic vco scale

CREATE_PARM(L_MAClsVdcMn,       float);     // Line contactor MA close minimum allowed vdc
CREATE_PARM(L_MAClsIMsCt,       unsigned);  //  Line contactor MA close test current measurment time


// Variables
//----------
CREATE_PUBVAR(L_VabBrg,            float);  // line side bridge voltages (abc)
CREATE_PUBVAR(L_VbcBrg,            float);
CREATE_PUBVAR(L_VabBrgT2,          float);
CREATE_PUBVAR(L_VbcBrgT2,          float);

CREATE_PUBVAR(L_IaBrg,             float);  // line side bridge currents (abc)
CREATE_PUBVAR(L_IbBrg,             float);
CREATE_PUBVAR(L_IcBrg,             float);

CREATE_PUBVAR(L_IaBrgT2,           float);  // line side bridge task 2 currents (abc)
CREATE_PUBVAR(L_IbBrgT2,           float);
CREATE_PUBVAR(L_IcBrgT2,           float);

CREATE_PUBVAR(L_VdcBrg,            float);  // line side dc link and Db voltages
CREATE_PUBVAR(L_VdcBrgRcp,         float);
// for L_DbCelTst
CREATE_PUBVAR(L_DbResMeasReq,   unsigned);
CREATE_PUBVAR(L_DbCapMeasReq,   unsigned);
// DB
CREATE_PUBVAR(L_VdbBrg,            float);  // voltage across the DB1 resistor (volt)
CREATE_PUBVAR(L_DbTmpEst,          float);  // estimated temperature of DB resistor 1 (deg C)
CREATE_PUBVAR(L_BthtrTemp7,        float);  // Publish thermistor temp inside rotor-1 top DB module (deg C)
CREATE_PUBVAR(L_BthtrTemp8,        float);  // Publish thermistor temp inside rotor-1 bottom DB module (deg C)
CREATE_PUBVAR(L_DbPavgTotal,       float);  // Total (Db1+Db2) Db resistor power averaged in one Task1 (Watts)
CREATE_PUBVAR(L_DbPavgTotalT3,     float);  // Total (Db1+Db2) Db resistor power averaged in one Task1 and passed to task3 (Watts)
CREATE_PUBVAR(L_DbPdissptTotal,    float);  // Total (Db1+Db2) Db resistor dissipated power (Watts)
CREATE_PUBVAR(L_DbResTempMax,      float);  // Maximum of DB resistor estimated temperatures (deg C)
CREATE_PUBVAR(L_DbRefTemp,           int) = 0; // Reference temp for DB model (coolant=0 and module sensor =1)
CREATE_PUBVAR(DbVregFbk,           float);  // the averaged DB voltage (Volt)
CREATE_PUBVAR(DbAutoTstEnb,     unsigned);  // enable the auto DB test
CREATE_PUBVAR(DbAutoTstEnbT1,   unsigned);  // enable the auto DB test task1
CREATE_PUBVAR(DbInh,                 int);  // enumerated DB inhibit
CREATE_PUBVAR(SclLinkVolts,        float);  // DC link voltage feedback scaling
CREATE_PUBVAR(VdbVcoFreq,          float);  // DB voltage feedback VCO frequency (Hz)
CREATE_PUBVAR(L_DbPlsTstReq,    unsigned);  // DB pulse test request
CREATE_PUBVAR(L_DbCelTstReq,    unsigned);  // DB cell test request
CREATE_PUBVAR(L_VdbOnAdj,          float) = 1.200F; // Adjustment factor for calculating Db-1 On threshold
CREATE_PUBVAR(L_DbOtDsbFlg,     unsigned);  // DB Over temperature disable flag
CREATE_PUBVAR(L_DbDsbOverloadT2,unsigned);  // Disable DB1 because of resistor thermal overload in task2
CREATE_PUBVAR(L_DbcRefLvlSel,        int);
// End of DB
CREATE_PUBVAR(L_BrgFltHdw,      unsigned);
CREATE_PUBVAR(L_DesatFlt,       unsigned);

CREATE_PUBVAR(L_IocPhsA,        unsigned);
CREATE_PUBVAR(L_IocPhsB,        unsigned);
CREATE_PUBVAR(L_IocPhsC,        unsigned);

CREATE_PUBVAR(GmVanComp,           float);  // grid monitor phs-A voltage compensated for transformer leakage
CREATE_PUBVAR(GmVbnComp,           float);  // grid monitor phs-B voltage compensated for transformer leakage
CREATE_PUBVAR(GmVcnComp,           float);  // grid monitor phs-C voltage compensated for transformer leakage
CREATE_PUBVAR(GmVanCompT2,         float);  // grid monitor compensated phase voltages passed to task2
CREATE_PUBVAR(GmVbnCompT2,         float);
CREATE_PUBVAR(GmVcnCompT2,         float);
CREATE_PUBVAR(GmIaZ1,              float);  // one task delayed grid monitor phase currents
CREATE_PUBVAR(GmIbZ1,              float);
CREATE_PUBVAR(GmIcZ1,              float);
CREATE_PUBVAR(GmVanZ1,             float);  // one task delayed grid monitor phase voltages
CREATE_PUBVAR(GmVbnZ1,             float);
CREATE_PUBVAR(GmVcnZ1,             float);
CREATE_PUBVAR(GmVanT2,             float);
CREATE_PUBVAR(GmVbnT2,             float);
CREATE_PUBVAR(GmVcnT2,             float);
CREATE_PUBVAR(GmVcom,              float);  // 690 GM common mode voltage (volt)
CREATE_PUBVAR(GmVcomFil,           float);  // 690 GM common mode voltage filtered (volt)
CREATE_PUBVAR(GmVcomAbs,           float);  // 690 GM common mode voltage absolute value (volt)
CREATE_PUBVAR(GmVcomAbsThrs,       float);  // threshold for declaring excessive 690 GM common mode voltage (volt)
CREATE_PUBVAR(GmVcomDlyTmr,        float) = 0.0; // timer to track the delaying of common mode voltage protection in xfmr 690V winding (Sec)
CREATE_PUBVAR(GmVcomTmr,           float);  // timer to track the common mode voltage abnormal in xfmr 690V winding (Sec)

CREATE_PUBVAR(GmPa,                float);  // Instantaneous xfmr 690V winding phase-A power (Watts)
CREATE_PUBVAR(GmPb,                float);  // Instantaneous xfmr 690V winding phase-B power (Watts)
CREATE_PUBVAR(GmPc,                float);  // Instantaneous xfmr 690V winding phase-C power (Watts)
CREATE_PUBVAR(GmPaT2,              float);  // Instantaneous Pa(t) data-passed to task2 (Watts)
CREATE_PUBVAR(GmPbT2,              float);  // Instantaneous Pb(t) data-passed to task2 (Watts)
CREATE_PUBVAR(GmPcT2,              float);  // Instantaneous Pc(t) data-passed to task2 (Watts)

CREATE_PUBVAR(GmIxFbkZ1 ,          float);  // Gm X-current feedback delay Z1
CREATE_PUBVAR(GmIyFbkZ1 ,          float);  // Gm Y-current feedback delay Z1
CREATE_PUBVAR(GmVxFbkZ1,           float);
CREATE_PUBVAR(GmVyFbkZ1,           float);
CREATE_PUBVAR(GmVxFbkZ,            float);
CREATE_PUBVAR(GmVyFbkZ,            float);
CREATE_PUBVAR(L_VxLkgPri,          float);  // Vx of primary leakage voltage caused by line current
CREATE_PUBVAR(L_VyLkgPri,          float);  // Vy of primary leakage voltage caused by line current
CREATE_PUBVAR(L_VxLkgSec,          float);  // Vx of 690V secondary leakage voltage
CREATE_PUBVAR(L_VyLkgSec,          float);  // Vy of 690V secondary leakage voltage

CREATE_PUBVAR(L_VxLkgPriT2,        float);
CREATE_PUBVAR(L_VyLkgPriT2,        float);
CREATE_PUBVAR(L_VxLkgSecT2,        float);
CREATE_PUBVAR(L_VyLkgSecT2,        float);
CREATE_PUBVAR(GmVxFbkZT2,          float);
CREATE_PUBVAR(GmVyFbkZT2,          float);

CREATE_PUBVAR(GmIxyShftD,          float);
CREATE_PUBVAR(GmIxyShftQ,          float);

// Other
CREATE_PUBVAR(L_BrgTestL,          float);
CREATE_PUBVAR(L_BrgTestC,          float);
CREATE_PUBVAR(L_IsensorScl,        float);
CREATE_PUBVAR(L_VnoiseLvl,         float);
CREATE_PUBVAR(L_RunningT1,      unsigned);
CREATE_PUBVAR(L_BrgIocThrs,        float);  // Bridge current Ioc threshold (Amps)

CREATE_PUBVAR(L_LkoutGnScl,        float);  // Scale factor on currents for lockout comp
CREATE_PUBVAR(L_LkoutGnSclT1,      float);
CREATE_PUBVAR(GridVoltLowTmr1,     float);  // timer that grid voltage Vab stays below low level (sec)
CREATE_PUBVAR(GridVoltLowTmr2,     float);  // timer that grid voltage Vbc stays below low level (sec)
CREATE_PUBVAR(GridVoltHiTmr1,      float);  // timer that grid voltage Vab stays above the high level (sec)
CREATE_PUBVAR(GridVoltHiTmr2,      float);  // timer that grid voltage Vbc stays above the high level (sec)
CREATE_PUBVAR(GridVoltOkTmr1,      float);  // timer that grid voltage Vab is back to Ok level (sec)
CREATE_PUBVAR(GridVoltOkTmr2,      float);  // timer that grid voltage Vbc is back to Ok level (sec)
CREATE_PUBVAR(GridFltVcomChkDsb,unsigned);  // disable the common mode voltage check due to grid event
CREATE_PUBVAR(GridVabLowIp,     unsigned);  // line grid voltage Vab low in progress
CREATE_PUBVAR(GridVbcLowIp,     unsigned);  // line grid voltage Vbc low in progress
CREATE_PUBVAR(GridVabHiIp,      unsigned);  // line grid voltage Vab high in progress
CREATE_PUBVAR(GridVbcHiIp,      unsigned);  // line grid voltage Vbc high in progress
CREATE_PUBVAR(XfmrSatDampTmr,      float);  // timer for xfmr saturation to damp after grid event (sec)
CREATE_PUBVAR(XfmrSatStartFlg,     float);  // xfmr saturation starts when grid events is off

CREATE_PUBVAR(L_RtrLoopFltEnb,  unsigned);  // Enable assertion of line RtrLoop fault

CREATE_PUBVAR(L_VdcFbkAdjMult,     float);  // multiplier to adjust Vdc feedback
CREATE_PUBVAR(L_DbTstCmd,       unsigned);
CREATE_PUBVAR(L_DbCmd,          unsigned);
CREATE_PUBVAR(L_DbTstIp,        unsigned);
CREATE_PUBVAR(L_DbTstIpZ1,      unsigned);
CREATE_PUBVAR(L_Q1StatusVsqr,   unsigned);  // Q1 open check for DB thermal class
CREATE_PUBVAR(L_DSatDx,         unsigned);      // DB desat fault passed from fpga


// Data Passing
//--------------
DATA_PASS(L_LkoutGnScl,                 L_LkoutGnSclT1,    T3_T1, UNFILTERED );
DATA_PASS(L_Running,                    L_RunningT1,       T2_T1, UNFILTERED);
DATA_PASS(L_VabBrg,                     L_VabBrgT2,        T1_T2, FILTERED);
DATA_PASS(L_VbcBrg,                     L_VbcBrgT2,        T1_T2, FILTERED);
DATA_PASS(GmVanComp,                    GmVanCompT2,       T1_T2, FILTERED);
DATA_PASS(GmVbnComp,                    GmVbnCompT2,       T1_T2, FILTERED);
DATA_PASS(GmVcnComp,                    GmVcnCompT2,       T1_T2, FILTERED);
DATA_PASS(L_IaBrg,                      L_IaBrgT2,         T1_T2, FILTERED);
DATA_PASS(L_IbBrg,                      L_IbBrgT2,         T1_T2, FILTERED);
DATA_PASS(L_IcBrg,                      L_IcBrgT2,         T1_T2, FILTERED);
DATA_PASS(GmVan,                        GmVanT2,           T1_T2, FILTERED);
DATA_PASS(GmVbn,                        GmVbnT2,           T1_T2, FILTERED);
DATA_PASS(GmVcn,                        GmVcnT2,           T1_T2, FILTERED);
DATA_PASS(GmVxFbkZ,                     GmVxFbkZT2,        T1_T2, FILTERED );
DATA_PASS(GmVyFbkZ,                     GmVyFbkZT2,        T1_T2, FILTERED );
DATA_PASS(L_VxLkgPri,                   L_VxLkgPriT2,      T1_T2, FILTERED);
DATA_PASS(L_VyLkgPri,                   L_VyLkgPriT2,      T1_T2, FILTERED);
DATA_PASS(L_VxLkgSec,                   L_VxLkgSecT2,      T1_T2, FILTERED);
DATA_PASS(L_VyLkgSec,                   L_VyLkgSecT2,      T1_T2, FILTERED);
DATA_PASS(GmPa,                         GmPaT2,            T1_T2, FILTERED);
DATA_PASS(GmPb,                         GmPbT2,            T1_T2, FILTERED);
DATA_PASS(GmPc,                         GmPcT2,            T1_T2, FILTERED);
DATA_PASS(DbAutoTstEnb,                 DbAutoTstEnbT1,    T2_T1, UNFILTERED);
DATA_PASS(DbInh,                        DbInhT1,           T2_T1, UNFILTERED);
DATA_PASS(L_DbThrmModel.DtmDsbOverload, L_DbDsbOverloadT2, T3_T2, UNFILTERED );


// Unpublished Variables
//----------------------
unsigned GridVabLowIpZ1;
unsigned GridVbcLowIpZ1;
unsigned GridVabHiIpZ1;
unsigned GridVbcHiIpZ1;
float    DummyTemp7;
float    DummyTemp8;


// Local Prototypes (to resolve forward references)
//-------------------------------------------------
extern cBridgeRebf          L_BridgeRebf;
extern cDbCtl               L_DbCtl;
extern cDbThrmModel         L_DbThrmModel;
extern cBrgThrmstr          L_BrgThrmstr;
extern cDbPlsTst            L_DbPlsTst;
extern cDbCelTst            L_DbCelTst;


// Class Instances
//----------------
//-----------------------------------------------------------------------------
// GP diagnostic filters, a framework class instanced on behalf of product code
cGpDiagFilter::InsListTyp L_GpDiagFiltIns =
{
    cGpDiagFilter::TASK1,           // 1 - which task 1 to execute filter
    L_DelTm1,                       // 2 - Delta time for task 1 (Sec)
};
cGpDiagFilter GpDiagFilter( L_GpDiagFiltIns );      // no prefix is used to preserve parm class-names
INSTANCE_INFO(GpDiagFilter, BRG_UNIQ_L, USE_XLS, TASK1)

//-----------------------------------------------------------------------------
// line pulse test
cPulseTest::InListTyp L_PlsTstIn =
{
    L_PulseTestReq,                 //  1 - Bridge test request from product code - Task2
    L_BridgeRebf.Brg1Enabled,       //  2 - Gating enable indication from bridge under test - Task1
    TripFlt,                        //  3 - Trip fault active
};
cBridgeTest::InsListTyp L_BrgTstIns =
{
    L_DelTm1,                       //  1 - Delta time for task 1 (Sec)
    DelTm2,                         //  2 - Delta time for task 2 (Sec)
    L_DelTm1Rcp,                    //  3 - Delta time reciprocal for task 1 (1/Sec)
    Clk0PerQuant,                   //  4 - Number of Pwm Timer counts in a pulse quanta
    QuantsPerMod,                   //  5 - Total number of pulse quanta per scheduling
    Clk0PerT1,                      //  6 - Total number of clock pulses per scheduling
    DelTmQuant,                     //  7 - Delta time in a quant (Sec) - cell test only
    PARM(L_ShuntRat),               //  8 - Shunt rated current (Arms) - cell test only
    L_IsensorScl,                   //  9 - Phase current sensor (lem/shunt) scale (Amp/Hz) - cell test only
    L_BrgTestL,                     // 10 - Total inductance seen between two bridge terminals (Henry)
    L_BrgTestC,                     // 11 - Total capacitance seen between two bridge terminals (Farad)
    L_CTCurLvl,                     // 12 - Target current for bridge test (Amps)
    PARM(L_VllInput),               // 13 - Nominal bridge line to line voltage (Volts)
    FZero,                          // 14 - Alternate inductance seen during cell test (Henry)
    PARM(BrgLockoutTm),             // 15 - Bridge lockout time
};
cLinePulseTest L_LinePulseTest( L_PlsTstIn, L_BrgTstIns );
INSTANCE_INFO( L_LinePulseTest, BRG_UNIQ_L, BRG_NUM_L, TASK1 )


//-----------------------------------------------------------------------------
// Cell test
cCellTest::InListTyp L_CelTstIn =
{
    L_SeqCelTstReq,                  //  1 - Bridge test request from product code - Task2
    TripFlt,                         //  2 - Trip fault active
    L_AutoCelTst,                    //  3 - Type of automatic cell test running - Task2
    cMessageStack::MsgPending,       //  4 - Message pending on stack - Task2
    L_CelTstDisab,                   //  5 - Disable cell test operation - Task2
    SysTm2,                          //  6 - System time for Task2
    SysTm1,                          //  7 - System time for Task1
    L_BridgeRebf.FpgaFltLatch,       //  8 - Fpga fault has latched disabling gating - Task1
    L_BridgeRebf.Brg1DsatIoc,        //  9 - Desaturation and over-current status - Task1
    L_IaBrg,                         // 10 - PhA current feedback (amp) - Task1
    L_IbBrg,                         // 11 - PhB current feedback (amp) - Task1
    L_IcBrg,                         // 12 - PhC current feedback (amp) - Task1
    L_VabBrg,                        // 13 - Phase a-to-b voltage feedback (volt) - Task1
    L_VbcBrg,                        // 14 - Phase b-to-c voltage feedback (volt) - Task1
    L_VMagT2,                        // 15 - Dist filter Vll magnitude (volt) - Task2
    L_VdcFbkT3,                      // 16 - DC-link total voltage feedback (volt) - Task3
    SimMode,                         // 17 - Cell test not valid in present operating mode
    False                            // 18 - Rotor dual or singal bridge -- only for rotor class
};
cCellTest L_CellTest( L_CelTstIn, L_BrgTstIns );
INSTANCE_INFO( L_CellTest, BRG_UNIQ_L, BRG_NUM_L, TASK1 )


//-----------------------------------------------------------------------------
// line modulator
cPwmMod2Async::PwmMod2AsyncInTyp  L_PwmMod2AsyncIn =
{
    L_UaCmd,                        //  1 - normalized phase modulation commamds (L-L scaling)
    L_UbCmd,                        //  2
    L_UcCmd,                        //  3
    FZero,                          //  4 - normalized common mode mod commamds (L-L scaling)
    L_IaCmdLkout,                   //  5 - current commands for lockout compensation
    L_IbCmdLkout,                   //  6
    L_IcCmdLkout,                   //  7
    L_PwmUp,                        //  8 - true = positive-going primary transitions
    FZero,                          //  9 - phase transition adjusts (unused on line-side)
    FZero,                          // 10
    FZero,                          // 11
    False,                          // 12 - bridge crow-bar control (unused on line-side)
    False,                          // 13
};
cPwmMod2Async::PwmMod2AsyncInsTyp L_PwmMod2AsyncIns =
{
    QuantsPerMod,                   //  1 - # of quant per frame   (UFull)
    Clk0PerQuant,                   //  2 - # of count per quantum (PwmQuant)
    L_DelTm1,                       //  3 - Task1 delta time (seconds)
    PARM(SysVoltNom),               //  4 - max voltage LL for LkoutComp (VllMax)
    PARM(CnvLineL),                 //  5 - equivalent inductance for LkoutComp   (henrys)
    PARM(BrgLockoutTm),             //  6 - minimum time for LkoutComp (seconds)
    PARM(L_LkoutInh),                //  7 - disable LkoutComp (typically for SimMode)
    False,                          //  8 - disable common mode injection
    PARM(L_SkipFireInh),            //  9 - disable skip-fire (typically for SimMode)
    False,                          // 10 - enable calculations for dual bridge
};
cPwmMod2Async  L_PwmMod2Async ( L_PwmMod2AsyncIn, L_PwmMod2AsyncIns );
INSTANCE_INFO( L_PwmMod2Async, BRG_UNIQ_L, BRG_NUM_L, TASK1 )


//-----------------------------------------------------------------------------
// line feedback offset for REBF
BrgHdwOffsInTyp L_BrgHdwFbkOffsIn =
{
    L_IOffsEnb,                     //  1 - I offset null enable
    True,                           //  2 - V offset null enable - Only Rotor class
    False,                          //  3 - Slow I offset null omega switch in task2 - Only Rotor class
    False,                          //  4 - V offset fast omega and time switch in task2 - Only Rotor class
    L_InnrRegEnbT3,                 //  5 - Inner regulator enable - Only Line class
    PwrDnFbkDsb,                    //  6 - Power down feedback disable
};
BrgHdwOffsInsTyp L_BrgHdwFbkOffsIns =
{
    L_DelTm1,                       //  1 - Task1 delta time - Only Rotor class
    DelTm2,                         //  2 - Task2 delta time - Only Line class
    DelTm3,                         //  3 - Task3 delta time  - Only Line class
    L_DelTm1Rcp,                    //  4 - One over task1 delta time  - Only Rotor class
    SysTm2,                         //  5 - System time in task2 - Only Rotor class
    "L_",                           //  6 - Class instance uniquifier string
	DsimMode,                       //  7 - Disable voltage offset accumulation
};
cLineBrgFbkOffs L_LineBrgFbkOffs ( L_BrgHdwFbkOffsIn, L_BrgHdwFbkOffsIns );
INSTANCE_INFO( L_LineBrgFbkOffs, BRG_UNIQ_L_BR, BRG_NUM_L, TASK1 )

BrgHdwOffsInTyp LIo1_HdwFbkOffsIn =
{
    L_IOffsEnb,                     //  1 - I offset null enable
    True,                           //  2 - V offset null enable - Only Rotor class
    True,                           //  3 - Slow I offset null omega switch in task2 - Only Rotor class
    False,                          //  4 - V offset fast omega and time switch in task2 - Only Rotor class
    True,                           //  5 - Inner regulator enable - Only Line class
    PwrDnFbkDsb,                    //  6 - Power down feedback disable
};
BrgHdwOffsInsTyp LIo1_HdwFbkOffsIns =
{
    L_DelTm1,                       //  1 - Task1 delta time - Only Rotor class
    DelTm2,                         //  2 - Task2 delta time - Only Line class
    DelTm3,                         //  3 - Task3 delta time  - Only Line class
    L_DelTm1Rcp,                    //  4 - One over task1 delta time  - Only Rotor class
    SysTm2,                         //  5 - System time in task2 - Only Rotor class
    "Gm",                           //  6 - Class instance uniquifier string
	DsimMode,                       //  7 - Disable voltage offset accumulation
};
cLineBrgFbkOffs LIo1_LineBrgFbkOffs ( LIo1_HdwFbkOffsIn, LIo1_HdwFbkOffsIns );
INSTANCE_INFO( LIo1_LineBrgFbkOffs, BRG_UNIQ_L_IO1, BRG_NUM_L, TASK1 )

//-----------------------------------------------------------------------------
// line REBF bridge interface
cBridgeRebf::InTyp L_BridgeRebfIn =
{
    L_BrgPwrEnbCmd,                 //  1 - bridge 1 enable request
    L_PhsTransListBrg1,             //  2 - bridge 1 phase transition list
    L_PhsDtransListBrg1,            //  3 - bridge 1 phase D transition list
    FrameFrc,                       //  4 - frc time for the upcoming frame boundary
    FrameFrcZ,                      //  5 - frc time for the current frame boundary
    SysTm3,                         //  6 - system time updated at Task3 rate
    EventTm1,                       //  7 - EventTm for Task1
    DiagResetCmdT1,                 //  8 - signal to reset latched faults in Task1
    DiagResetCmdT3,                 //  9 - signal to reset latched faults in Task3
    L_AeadK1Out,                    // 10 - signal to drive output relay 1 on IO 1
    L_AeadK2Out,                    // 11 - signal to drive output relay 2 on IO 1
    L_AeadK3Out,                    // 12 - signal to drive output relay 3 on IO 1
    L_AeadK4Out,                    // 13 - signal to drive output relay 4 on IO 1
    L_AeadK5Out,                    // 14 - signal to drive output relay 5 on IO 1
    L_AeadK6Out,                    // 15 - signal to drive output relay 6 on IO 1
    L_AeadK7Out,                    // 16 - signal to drive output relay 7 on IO 1
    L_AeadK8Out,                    // 17 - signal to drive output relay 8 on IO 1
    L_AeadK9Out,                    // 18 - signal to drive output relay 9 on IO 1
    L_LineBrgFbkOffs,               // 19 - external bridge 1 feedback offset class
    LIo1_LineBrgFbkOffs,            // 20 - external IO 1 feedback offset class
    L_DbCmd,                        // 21 - fire Db1 request
    L_ThrmHiSel,                    // 22 - 1=select High range for thermistor sensing, 0 = lo
    FZero,                          // 23 - Fan#1 speed reference: <32 off, 32->255 0-100%
    FZero,                          // 24 - Fan#2 speed reference: <32 off, 32->255 0-100%
    L_IocMask,                      // 25 - Mask A-B-C IOC  faults from setting fpga fault latch
    True,                           // 26 - Enable fpga turn off of phase(s) with hi current
    False,                          // 27 - Mask A-B-C DSAT faults from setting fpga fault latch
    R_BridgeRebf.HsslCommFlt,       // 28 - Disable brg ready loop FO Tx
    L_RtrLoopFltEnb,                // 29 - Allow assertion of RtrLoop fault
    L_RebfLvK1Out,                  // 30 - signal to drive low-voltage output relay 01 on Rebf board
    L_RebfLvK2Out,                  // 31 - signal to drive low-voltage output relay 02 on Rebf board
    L_RebfLvK3Out,                  // 32 - signal to drive low-voltage output relay 03 on Rebf board
    FrmClkEstT1,                    // 33 - time of next Task1 frame (cpu clock counts)
    R_BridgeRebf.DisCmdNotFlg,      // 34 - Disable bridge request not working
    T1T2St,                         // 35 - T1 state within a T3 frame
    InitDone1Sec,                   // 36 - Flag to indicate 1.0 second done after initialization
};
cBridgeRebf::InsTyp L_BridgeRebfIns =
{
    L_DelTm1,                       //  1 - task1 period (sec)
    L_DelTm1Rcp,                    //  2 - task1 frequency (tasks/sec)
    CpuCps,                         //  3 - cpu cycles per second
    SimMode,                        //  4 - internal simulator mode
    XSimMode,                       //  5 - external simulator mode
    DsimMode,                       //  6 - dll simulator mode
	False,                          //  6 - configure parms for alternate(SEP,etc) operation
    "L_",                           //  7 - class instance uniquifier string
    True,                           //  8 - bridge 1 phase-D DSAT fault detection enable
    PARM(L_BrInfcHdw),              //  9 - hardware connected to Bridge_1 connector
    PARM(L_ShuntRat),               // 10 - bridge 1 shunt current at 50 mV
    FZero,                          // 11 - phase-D shunt current at 50 mV
    PARM(L_Io1InfcHdw),             // 12 - hardware connected to I/O 1 connector
    PARM(L_Io1AttenHdw),            // 13 - attenuator board connected I/O 1 voltage(low or high voltage input
    PARM(GmCTratio),                // 14 - CT ratio for I/O 1 current feedback
    PARM(BrgLockoutTm),             // 15 - gating lock out time (seconds)
    QuantsPerMod,                   // 16 - number of quants per task1 frame
    Clk0PerQuant,                   // 17 - number of clock-tics (or counts) in one quant
    Clk0PerT1,                      // 18 - FRC counts in task 1 frame
    PARM(L_HslaRebfSite),           // 19 - HSLA site on the REBF board
    PARM(TestMode),                 // 20 - test mode
    True,                           // 21 - Select Line converter fpga behavior
    DelTm3,                         // 22 - task3 period (sec)
    False,                          // 23 - Io1 relay pwr supply check inhibit request
    CpuCpsRcpUsec,                  // 24 - Cpu clock period (usec)
    L_TempVcoCalcSt,                // 25 - Vco temperature calculation state
    IZero,                         // 27 - Number of tachometer pulses per revolution
};
cBridgeRebf L_BridgeRebf ( L_BridgeRebfIn, L_BridgeRebfIns );
INSTANCE_INFO( L_BridgeRebf, BRG_UNIQ_L, BRG_NUM_L, TASK1 )


//-----------------------------------------------------------------------------
// enhanced line bridge thermal model
cBrgThrm5pModel::InListTyp L_BrgThrm5pModelIn =
{
    L_VdcFbkT3,                         //  1 - Dc link voltage fbk (Volts) - Task3
    CoolantTemp,                        //  2 - Coolant temperature (degC) - Task3
    L_IaBrg,                            //  3 - Phase-A current feedback (Amps) - Task1
    L_IbBrg,                            //  4 - Phase-B current feedback (Amps) - Task1
    L_IcBrg,                            //  5 - Phase-C current feedback (Amps) - Task1
    L_PwmMod2Async.ModUaCmdM,           //  6 - Phase A modulation output with one task latency (Pu) - Task1
    L_PwmMod2Async.ModUbCmdM,           //  7 - Phase B modulation output with one task latency (Pu) - Task1
    L_PwmMod2Async.ModUcCmdM,           //  8 - Phase C modulation output with one task latency (Pu) - Task1
    TempIsValid,                        //  9 - Reference temperature fbk is valid (equal to 1) - Task3
    L_BrgPwrEnbCmd,                     // 10 - Bridge power enable - Task1
    L_BridgeRebf.Brg1DsatIoc.IocPhsA,   // 11 - Fpga phase-A ioc fault indication - Task1
    L_BridgeRebf.Brg1DsatIoc.IocPhsB,   // 12 - Fpga phase-B ioc fault indication - Task1
    L_BridgeRebf.Brg1DsatIoc.IocPhsC,   // 13 - Fpga phase-C ioc fault indication - Task1
    L_BrgThrmstr.BthtrTemp1,            // 14 - Baseplate temperature for devices of PhsA (degC) - Task3
    L_BrgThrmstr.BthtrTemp2,            // 15 - Baseplate temperature for devices of PhsB (degC) - Task3
    L_BrgThrmstr.BthtrTemp3,            // 16 - Baseplate temperature for devices of PhsC (degC) - Task3
};
cBrgThrm5pModel::InsListTyp L_BrgThrm5pModelIns =
{
    DelTm2,                         //  1 - Task 2 time (Secs) - Rules
    DelTm3,                         //  2 - Delta time for task 3 (Sec) - Rules
    L_DelTm1Rcp,                    //  3 - Delta time reciprocal for task 1 (1/Sec) - Task3
    L_BrgIocThrs,                   //  4 - Ioc detection threshold (Amps)
    L_UHalfRcp,                     //  5 - Reciprocal of half quants per task1 (1/Quant) - Rules
    L_PwmMod2Async.ModPwmN,         //  6 - Number of PWM pulses per task1 - Task3
    PARM(SoftwareIPN),              //  7 - Configure parms for proper bridge type
    PARM(TestMode),                 //  8 - Converter Test mode - Rules
    PARM(L_ParallelIGBT),           //  9 - Parallel IGBTs for 'true'

};
cBrgThrm5pModel L_BrgThrm5pModel ( L_BrgThrm5pModelIn, L_BrgThrm5pModelIns);
INSTANCE_INFO( L_BrgThrm5pModel, BRG_UNIQ_L, BRG_NUM_L, TASK1 )


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// dynamic brake pulse test
cDbPlsTst::InListTyp L_DbPlsTstIn =
{
    TripFlt,                        //  1 - Trip fault active
    L_DbPlsTstReq,                  //  2 - DB pulse test request (from sequencer)
};
cDbPlsTst::InsListTyp L_DbPlsTstIns =
{
    L_DelTm1,                       //  1 - task1 delta time (sec)
};
cDbPlsTst L_DbPlsTst ( L_DbPlsTstIn, L_DbPlsTstIns );
INSTANCE_INFO( L_DbPlsTst, BRG_UNIQ_DB, BRG_NUM_L, TASK1)


//-----------------------------------------------------------------------------
// dynamic brake cell test
cDbCelTst::InListTyp L_DbCelTstIn =
{
    TripFlt,                        //  1 - Trip fault active
    L_DbCelTstReq,                  //  2 - DB cell test request (from sequencer)
    L_IaBrgT2,                      //  3 - Line side PhA task 2 current feedback (Amps)
    L_IbBrgT2,                      //  4 - Line side PhB task 2 current feedback (Amps)
    L_IcBrgT2,                      //  5 - Line side PhC task 2 current feedback (Amps)
    L_BusPwrRcp,                    //  6 - Reciprocal of average active power during cell test (1/Watts)
    L_DbCtl.DbcVdb1VdcMisFlg,       //  7 - Flag indicates that the actual DB1 voltage mismatch the expected one
    False,                          //  8 - Flag indicates that the actual DB2 voltage mismatch the expected one
    L_DbCtl.DbcDb1MisOffTstFlg,     //  9 - DB1 mistakenly off flag in DB test mode
    L_DbCtl.DbcDb1MisOnTstFlg,      // 10 - DB1 mistakenly on flag in DB test mode
    False,                          // 11 - DB2 mistakenly off flag in DB test mode
    False,                          // 12 - DB2 mistakenly on flag in DB test mode
    L_RunRdy,                       // 13 - Line converter run ready
    L_DbPlsTst.DbPtStat,            // 14 - DB pulse test status
    L_DbPlsTst.DbPtAct,             // 15 - DB pulse test active
    L_IMeasStat,                    // 16 - Line Rms Phase Current measure status
    L_DbThrmModel.DtmPdisspt,       // 17 - DB resistor rate of energy (Watts)
    L_VdcFbkT2,                     // 18 - DC link voltage feedback task2 version
    L_VMagT2,                       // 19 - Line voltage magnitude
    GmVMagPuT2,                     // 20 - Line Gm voltage magnitude per unit task 2
    L_VdbTst,                       // 21 - Average DB voltage feedback during cell test (Volts)
    PARM(L_DbCtSel),                // 22 - Bridge selector for cell test
    L_DbResMeasReq,                 // 23 - Resistance measurement request
    L_DbCapMeasReq,                 // 24 - Capacitance measurement request
    Q1Status,                       // 25 - Q1 status feedback
};

cDbCelTst::InsListTyp L_DbCelTstIns =
{
    DelTm2,                         //  1 - Task2 delta time (Sec)
    PARM(BrgDbR),                   //  2 - DB resistor value (Ohms)
    PARM(DbResIMin),                //  3 - Line contactor MA close minimum current (Amps)
    PARM(L_MAClsVdcMn),             //  4 - Line contactor MA close minimum allowed vdc (Volts)
    PARM(DbOtThrsh),                //  5 - Db Over temperature Threshold due to DB overheating (Watts)
    PARM(L_CBrg),                   //  6 - DC link capacitance (Farad)
    False,                          //  7 - Dual DB bridge enabled
};
cDbCelTst L_DbCelTst ( L_DbCelTstIn, L_DbCelTstIns );
INSTANCE_INFO( L_DbCelTst, BRG_UNIQ_DB , BRG_NUM_L, TASK1 )
//-----------------------------------------------------------------------------
// dynamic brake thermal models
cDbThrmModel::InListTyp L_DbThrmModelIn =
{
    L_VdbBrg,                       //  1 - Voltage across DB resistor (Volts) - Task1
    L_VdcBrg,                       //  2 - DC bus voltage across DB switch and resistor (Volts) - Task1
    CoolantTemp,                    //  3 - Reference #1 temperature (degC) - Task3
    L_BrgThrmstr.BthtrTempMaxDb,    //  4 - Reference #2 temperature (degC) - Task3
    ConvDiscnStatus,                //  5 - Converter disconnected indication - Task3
    L_StoppedT3,                    //  6 - Stopped state indication - Task3
    L_DiagModeT3,                   //  7 - Diagnostic mode indication - Task3
    L_DbRefTemp,                    //  8 - Reference temperature active in model - Task3
    L_Q1StatusVsqr,                 //  9 - Q1 status determined by squared voltage difference between terminals
};
cDbThrmModel::InsListTyp L_DbThrmModelIns =
{
    L_DelTm1,                       //  1 - Task 1 time (Sec)
    L_DelTm1Rcp,                    //  2 - Reciprocal of task 1 time (Sec)
    DelTm2,                         //  3 - Task 2 time (Sec)
    DelTm3,                         //  4 - Task 3 time (Sec)
    PARM(BrgDbR),                   //  5 - Resistance for one of n DB circuits (Ohm)
    BrgDbTauElec,                   //  6 - Electrical time constant of DB resistor (Sec)
    PARM(DbVfbkTau),                //  7 - DB voltage feedback circuit time constant (Sec)
    PARM(L_VllIn),                  //  8 - Line voltage (volts line-line rms)
    L_VdbOnAdj,                     //  9 - Adjustment factor for calculating Db On threshold
    PARM(BrgDbPwrCont),             // 10 - Continuous power rating of DB resistor (Watt)
    PARM(BrgDbPwrImp),              // 11 - Impulse power rating of DB resistor (Watt)
    PARM(BrgDbImpTm),               // 12 - Impulse time rating of DB resistor (Sec)
    L_PwrDisconTm,                  // 13 - Line bridge power disconnection time (Sec)
    PARM(SoftwareIPN),              // 14 - Configure parms by product type
    PARM(DbOtThrsh),                // 15 - Db Over temperature Threshold due to DB overloading (Watt)
    PARM(BrgDbRthrm),               // 16 - DB resistor thermal resistance (deg C/ Watt)
    PARM(L_BrgDbEnb),               // 17 - DB thermal model enable indication
    PARM(CoolantOtThrsh),           // 18 - Liquid coolant over temperature threshold (degC)
};
cDbThrmModel L_DbThrmModel ( L_DbThrmModelIn, L_DbThrmModelIns );
INSTANCE_INFO( L_DbThrmModel, BRG_UNIQ_DB1, USE_XLS, TASK1 )
// dynamic brake control

cDbCtl::InListTyp L_DbCtlIn =
{
    DbInhT1,                        //  1 - inhibit the DB to fire
    DbAutoTstEnbT1,                 //  2 - enable the auto DB test
    L_DbThrmModel.DtmPdisspt,       //  3 - DB1 thermal energy retention rate (Watts)
    FZero,                          //  4 - DB2 thermal energy retention rate (Watts)
    L_VdcFbk,                       //  5 - DC link voltage feedback (volt)
    L_VdbBrg,                       //  6 - actural measured DB1 voltage from REBF (volt)
    FZero,                          //  7 - actural measured DB2 voltage from REBF (volt)
    FZero,                          //  8 - external duty reference
    L_DbTstIpZ1,                    //  9 - DB is in cell test mode
    BrgCardsAct,                    // 10 - Over-all bridge cards active boolean
    L_BrgFltHdw,                    // 11 - bridge fault indicator
    L_DbThrmModel.DtmSwSt,          // 12 - DB1 switch status feedback
    False,                          // 13 - DB2 switch status feedback
    PARM(DbGateSkewEnb),            // 14 - enable the DB gate skew
    L_DbThrmModel.DtmOtLock,        // 15 - DB1 switch over temp lock-on command
    False,                          // 16 - DB2 switch over temp lock-on command
    DbAutoTstForce,                 // 17 - force the Db auto ping imediately fire
    L_BridgeRebf.CntgncyPktRcvd,    // 18- previous DB cmd was not actuated
    L_DbCmd,                        // 19 - Acutal DB command 1
    False,                          // 20 - Acutal DB command 2
    L_VdcFbkT3,                     // 21 - DC link voltage in task 3 version
    L_DbcRefLvlSel,                 // 22 - DB voltage regulator reference selection
};
cDbCtl::InsListTyp L_DbCtlIns =
{
    PARM(L_CBrg),                   //  1 - DC link capacitance (Farad)
    DbResCombined,                  //  2 - combined DB resistance value in prallel (Ohm)
    PARM(DbVmax),                   //  3 - maximum DC link voltage for DB to kick in (volt)
    PARM(L_VdcRefMax),              //  4 - the maximum DC link reference voltage (volt)
    L_DelTm1,                       //  5 - task 1 execution DelTm (sec)
    SclLinkVolts,                   //  6 - link voltage feedback scaling
    L_VdcFbkAdjMult,                //  7 - DB voltage feedback circuit time constant
    VdbVcoFreq,                     //  8 - DB voltage feedback VCO frequency (Hz)
    PARM(TestMode),                 //  9 - Converter Test mode
    PARM(BrgDbPwrCont),             // 10 - Continuous power rating of DB resistor (Watt)
    PARM(L_VdcOVLvl),               // 11 - DC link over-voltage level

};
cDbCtl L_DbCtl ( L_DbCtlIn, L_DbCtlIns );
INSTANCE_INFO( L_DbCtl, BRG_UNIQ_L, BRG_NUM_L, TASK1 )
// line bridge thermistor processing
cBrgThrmstr::InListTyp L_BrgThrmstrIn =
{
    L_VTemp1,                       //  1 - voltage at thermistor input 1
    L_VTemp2,                       //  2 - voltage at thermistor input 2
    L_VTemp3,                       //  3 - voltage at thermistor input 3
    L_VTemp4,                       //  4 - voltage at thermistor input 4
    L_VTemp5,                       //  5 - voltage at thermistor input 5
    L_VTemp6,                       //  6 - voltage at thermistor input 6
    L_VTemp7,                       //  7 - voltage at thermistor input 7
    L_VTemp8,                       //  8 - voltage at thermistor input 8
    TempFbkRdy,                     //  9 - Temperature feedbacks are valid
    true,                           // 10 - Temperature ready permissive
    L_StoppedT3,                    // 11 - Stopped state indication
};
cBrgThrmstr::InsListTyp L_BrgThrmstrIns =
{
    False,                          //  1 - Enable DB thermistor diagnostics
    DelTm3,                         //  2 - Task3 delta time
    L_BthtrTemp7,                   //  3 - Ref to nonconstant data for optional thermistor 7
    L_BthtrTemp8,                   //  4 - Ref to nonconstant data for optional thermistor 8
    PARM(L_ParallelIGBT),           //  5 - Parallel IGBTs for 'true'
    "L_",                           //  6 - Class instance uniquifier string

};
cBrgThrmstr L_BrgThrmstr ( L_BrgThrmstrIn, L_BrgThrmstrIns );
INSTANCE_INFO( L_BrgThrmstr, BRG_UNIQ_L, BRG_NUM_L, TASK1 )



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side task 1 control functions
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineControlT1(void)
{
    L_DbThrmModel.Task1();
    L_DbCelTst.Task1();
    L_DbPlsTst.Task1();
    L_DbCtl.Task1();

    L_DbTstIpZ1=L_DbTstIp;
    if (L_DbCelTst.DbResMsIpT1 || L_DbCelTst.DbCapMsIpT1)
    {
        L_DbTstIp = true;
        L_DbTstCmd = L_DbCelTst.DbCtStateCmd1;
        L_DbCmd  = L_DbTstCmd;
    }
    else if ( L_DiagMode && L_DbPlsTst.DbPtIp )
    {
        L_DbTstIp = true;
        L_DbTstCmd = L_DbPlsTst.DbPtStateCmd1;
        L_DbCmd  = L_DbTstCmd;
    }
    else
    {
        L_DbTstIp = false;
        L_DbTstCmd=false;
        L_DbCmd = static_cast<unsigned> (L_DbCtl.DbcMod1Out);
    }

    if ( L_DbThrmModel.DtmTr_GridDiscReq || (!Ps24vOk && !PwrDnFbkDsb) || L_DbThrmModel.DtmRes_GridDiscReq)
    {
       Q1OpnReq = true;
    }
    if(GmSVMtchErSq>GmSVmtchThr)
    {
        L_Q1StatusVsqr = false;
    }
    else
    {
        L_Q1StatusVsqr = true;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side task 2 control functions
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineControlT2(void)
{
    //-------------------------------------------------------------------------
    // common mode voltage check disable logic during grid event&recovery
    // check on the l-l voltage to avoid the disturb the grounding issue
    // common mode voltage check is to detect if the xfmr neutral is disconneted or not
    // use the l-l voltage to avoid the l-n votlage disturb
    // the boolean from this detector is only used to dsb the com volt check during
    // grid event/recovery/xfmr saturation

    //-------------------------------------------------------------------------
    // grid l-l Vab
    // set the GridVabLowIp/GridVabHiIp if the ABS(GmVabT2) is low or high
    // store the previous status
    GridVabLowIpZ1 = GridVabLowIp;
    GridVbcLowIpZ1 = GridVbcLowIp;
    GridVabHiIpZ1  = GridVabHiIp;
    GridVbcHiIpZ1  = GridVbcHiIp;

    if ( ( ( ABS(GmVabT2) * L_VllInRcp * SQRT2_RCP ) < PARM(GridVoltOkPuLvlLo) ) && !L_Stopped )
    {
        GridVoltLowTmr1 += DelTm2;
        if ( GridVoltLowTmr1 > PARM(GridVoltLowTm) )
        {
            GridVabLowIp    = true;
            GridVoltLowTmr1 = 0.0F;                  // clear the set timer
            GridVoltOkTmr1  = 0.0F;                  // clear the reset timer
        }
    }

    else if ( ( ( ABS(GmVabT2) * L_VllInRcp * SQRT2_RCP ) > PARM(GridVoltOkPuLvlHi) ) && !L_Stopped )
    {
        GridVoltHiTmr1 += DelTm2;
        if ( GridVoltHiTmr1 >= PARM(GridVoltHiTm) )
        {
            GridVabHiIp     = true;
            GridVoltHiTmr1  = 0.0F;                   // clear the set timer
            GridVoltOkTmr1  = 0.0F;                  // clear the reset timer
        }
    }

    // reset the GridVabLowIp and GridVabHiIp if the ABS(GmVabT2) is between the low and high ok level
    else
    {
        GridVoltLowTmr1 = 0.0F;                  // clear the set timer for voltage low
        GridVoltHiTmr1  = 0.0F;                  // clear the set timer for voltage high
        GridVoltOkTmr1 += DelTm2;
        if ( GridVoltOkTmr1 >= PARM(GridVoltOkTm))
        {
            GridVabLowIp   = false;
            GridVabHiIp    = false;
            GridVoltOkTmr1 = 0.0F;
        }
    }

    //-------------------------------------------------------------------------
    // grid l-l Vbc
    // set the GridVabLowIp if the ABS(GmVbcT2) is below the Ok lvl low
    if ( ( ( ABS(GmVbcT2) * L_VllInRcp * SQRT2_RCP ) < PARM(GridVoltOkPuLvlLo) ) && !L_Stopped )
    {
        GridVoltLowTmr2 += DelTm2;
        if ( GridVoltLowTmr2 > PARM(GridVoltLowTm) )
        {
            GridVbcLowIp    = true;
            GridVoltLowTmr2 = 0.0F;                  // clear the set timer
            GridVoltOkTmr2  = 0.0F;                  // clear the reset timer
        }
    }

    else if ( ( ( ABS(GmVbcT2) * L_VllInRcp * SQRT2_RCP ) > PARM(GridVoltOkPuLvlHi) ) && !L_Stopped )
    {
        GridVoltHiTmr2 += DelTm2;
        if ( GridVoltHiTmr2 >= PARM(GridVoltHiTm) )
        {
            GridVbcHiIp     = true;
            GridVoltHiTmr2  = 0.0F;                  // clear the set timer
            GridVoltOkTmr2  = 0.0F;                  // clear the reset timer
        }
    }
    // reset the GridVbcLowIp/GridVbcHiIp if the ABS(GmVbcT2) is abnormal
    else
    {
        GridVoltLowTmr2 = 0.0F;                  // clear the set timer for voltage low
        GridVoltHiTmr2  = 0.0F;                  // clear the set timer for voltage high
        GridVoltOkTmr2 += DelTm2;
        if ( GridVoltOkTmr2 >= PARM(GridVoltOkTm))
        {
            GridVbcLowIp   = false;
            GridVbcHiIp    = false;
            GridVoltOkTmr2 = 0.0F;
        }
    }

    //-------------------------------------------------------------------------
    // disable the com volt check
    if ( GridVabLowIp || GridVbcLowIp || GridVabHiIp || GridVbcHiIp )
    {
        GridFltVcomChkDsb = true;
    }

    //-------------------------------------------------------------------------
    // re-enable the com volt check XfmrSatDampTm secs after grid event is off to allow the xfmr saturation damp
    // falling edge of the GridVabLowIp or GridVbcLowIp
    else if  ( ( GridVabLowIpZ1 && !GridVabLowIp ) || ( GridVbcLowIpZ1 && !GridVbcLowIp ) ||
                 (GridVabHiIpZ1 && !GridVabHiIp ) || ( GridVbcHiIpZ1 && !GridVbcHiIp ) )
    {
        XfmrSatStartFlg = true;
    }
    if ( XfmrSatStartFlg )
    {
        XfmrSatDampTmr +=  DelTm2;
        if ( XfmrSatDampTmr >= PARM(XfmrSatDampTm) )
        {
            GridFltVcomChkDsb = false;
            XfmrSatDampTmr    = 0.0F;
            XfmrSatStartFlg   = false;
        }
    }

    //-------------------------------------------------------------------------
    // Timer to let voltages settle for some time
    GmVcomDlyTmr = ( GmVMagT2 > GmVcomAbsThrs ) ? (GmVcomDlyTmr + DelTm2) : 0.0F;

    //-------------------------------------------------------------------------
    // zero sequence detection on the 690 xfmr GM voltage

    if ( GmVcomDlyTmr > PARM(GmVcomDlyTm) )       // let voltages settle for some time
    {
        GmVcom = GmVanT2 + GmVbnT2 + GmVcnT2;
        GmVcomAbs = ABS(GmVcom);
        GmVcomFil += VcomFilGn * (GmVcomAbs - GmVcomFil);

        // Mask the common mode voltage check during grid event and xfmr saturation due to grid event
        if ( !GridFltVcomChkDsb && (GmVcomFil > GmVcomAbsThrs) )
        {
            // avoid push the com mode trip upon one glitch
            // push the com mode trip if the fault is persistant for 5 secs
            GmVcomTmr += DelTm2;
            if (GmVcomTmr >= PARM(VcomTm))
            {
                GmVcomTmr = PARM(VcomTm);
                PUSH_DIAG(GmVcomTrp);
            }
        }
        else
        {
            GmVcomTmr -= DelTm2;
            if(GmVcomTmr <= 0.0F)
            {
                GmVcomTmr = 0.0F;
            }
        }

        GmVcomDlyTmr = PARM(GmVcomDlyTm);
    }
    // reset the filter state
    else
    {
        GmVcomFil = 0.0F;
    }

    L_DbPlsTstReq          = L_SeqReq.L_DbPsTstReq;
    L_SeqStat.L_DbPsTstSta = L_DbPlsTst.DbPtStat;
    L_SeqAct.L_DbPsTstAct  = L_DbPlsTst.DbPtAct;

    L_DbOtDsbFlg=L_DbThrmModel.DtmDsbOverload;

    if ( (!L_DbGateDsb || !L_DbAutoTstDone) && InitDone && PARM(L_BrgDbEnb) )
    {
        DbInh = cDbCtl::NO_INHIBIT;     // Both Db circuits are allowed to gate
    }
    else
    {
        DbInh = cDbCtl::DUAL_INHIBIT;       // No Db circuit allowed to gate
    }

    L_PulseTestReq  = L_SeqReq.L_PlsTstReq;
    L_SeqStat.L_PlsTstStat = L_LinePulseTest.PlsTstStat;
    L_SeqAct.L_PlsTstAct  = L_LinePulseTest.PlsTstAct;

    L_DbCelTstReq=L_SeqReq.L_DbClTstReq;
    L_SeqStat.L_DbClTstSta=L_DbCelTst.DbCtStat;
    L_SeqAct.L_DbClTstAct=L_DbCelTst.DbCtAct;

    L_DbResMeasReq=L_SeqReq.L_DbResMsReq;
    L_SeqStat.L_DbResMsSta=L_DbCelTst.DbResMsSta;
    L_SeqAct.L_DbResMsAct=L_DbCelTst.DbResMsAct;

    L_DbCapMeasReq=L_SeqReq.L_DbCapMsReq;
    L_SeqStat.L_DbCapMsSta=L_DbCelTst.DbCapMsSta;
    L_SeqAct.L_DbCapMsAct=L_DbCelTst.DbCapMsAct;

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side task 3 control functions
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineControlT3(void)
{
    float Tempf;

    //-------------------------------------------------------------------------
    // Lockout gain scale
    Tempf = MAX(1.0F, L_IMagT3);    // avoid divide by zero

    Tempf = MIN(PARM(L_LkoutGnSclMax), PARM(L_LkoutGnSclK) / Tempf);

    L_LkoutGnScl =  MAX(PARM(L_LkoutGnSclMin), Tempf);



    // Wait for rotor-side HSSL to get valid data before allowing line RtrLoop fault
    if ( R_BridgeRebf.BrgHdwDataVld )    L_RtrLoopFltEnb = true;

    L_DbCtl.Task3();
    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side task 1 instrumentation functions
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineInstrumentationT1(void)
{
    if (PwrDnFbkDsb && !L_BrgPwrEnbCmd)
    {
        //-------------------------------------------------------------------------
        // Zero the feedbacks signals if power going down. (2nd condition should be redundant.)
        //  Don't zero Vdc in case used for db or for drving PwrDnFbkDsb.
        L_VabBrg = 0.0F;
        L_VbcBrg = 0.0F;

        L_IaBrg  = 0.0F;
        L_IbBrg  = 0.0F;
        L_IcBrg  = 0.0F;

        GmVan    = 0.0F;
        GmVbn    = 0.0F;
        GmVcn    = 0.0F;
        GmIa     = 0.0F;
        GmIb     = 0.0F;
        GmIc     = 0.0F;
    }
    else
    {
        //-------------------------------------------------------------------------
        // get the feedbacks signals from the bridge interface
        L_VabBrg = L_BridgeRebf.VabBrg1Hdw;
        L_VbcBrg = L_BridgeRebf.VbcBrg1Hdw;

        L_IaBrg  = L_BridgeRebf.IaBrg1Hdw;
        L_IbBrg  = L_BridgeRebf.IbBrg1Hdw;
        L_IcBrg  = L_BridgeRebf.IcBrg1Hdw;

        //GmVan  =  L_BridgeRebf.VaxIo1;
        GmVan  =  L_BridgeRebf.VaxIo1;
        GmVbn  =  L_BridgeRebf.VbxIo1;
        GmVcn  =  L_BridgeRebf.VcxIo1;
        GmIa   =  L_BridgeRebf.IaIo1;
        GmIb   =  L_BridgeRebf.IbIo1;
        GmIc   =  L_BridgeRebf.IcIo1;
    }


    if ( (PARM(SoftwareIPN) >= DFIG02_1500_5060_B) && (PARM(SoftwareIPN) != DFIG02_2400_5060_B))//AEDB for line bridge or REBH set
    {
        L_VdcBrg   = L_BridgeRebf.VdcFiberBrgHdw;   // From rotor AEBI, sampled at line task 1 rate.
        L_VdbBrg   = L_BridgeRebf.VdBrg1Hdw;        // From AEDB dc feedback
    }
    else if ( PARM(SoftwareIPN) == DFIG02_2400_5060_B)
    {
        L_VdcBrg   = L_BridgeRebf.VdBrg1Hdw;   // From rotor AEBI, sampled at line task 1 rate.
        L_VdbBrg   = L_VdcBrg - L_BridgeRebf.VdcFiberBrgHdw;
        //L_VdbBrg   = L_BridgeRebf.VdcFiberBrgHdw;        // From AEDB dc feedback
    }
    else //No AEDB; two AEBIs
    {
        L_VdcBrg   = L_BridgeRebf.VdBrg1Hdw;        // From rotor AEBI, sampled at line task 1 rate.
        L_VdbBrg   = 0.0;                           // No DB in this case and no db feedback
    }

    L_VdcBrgRcp = NoNANLim( 1.0F/L_VdcBrg, 1.0e6F, -1.0e6F );

    // Instantaneous per-phase power
    GmPa   =  GmVan * GmIa;
    GmPb   =  GmVbn * GmIb;
    GmPc   =  GmVcn * GmIc;

    // Compensate for transformer leakage inductance
    GmVanComp = 0.5F*(GmVan + GmVanZ1) - GmLxfmDt * (GmIa - GmIaZ1);
    GmVbnComp = 0.5F*(GmVbn + GmVbnZ1) - GmLxfmDt * (GmIb - GmIbZ1);
    GmVcnComp = 0.5F*(GmVcn + GmVcnZ1) - GmLxfmDt * (GmIc - GmIcZ1);

    GmIaZ1  = GmIa;
    GmIbZ1  = GmIb;
    GmIcZ1  = GmIc;
    GmVanZ1 = GmVan;
    GmVbnZ1 = GmVbn;
    GmVcnZ1 = GmVcn;

    // fpga fault indications
    L_BrgFltHdw = L_BridgeRebf.FpgaFltLatch;

    if ( L_BridgeRebf.Brg1DsatIoc.DsatSwAL || L_BridgeRebf.Brg1DsatIoc.DsatSwBL ||
         L_BridgeRebf.Brg1DsatIoc.DsatSwCL || L_BridgeRebf.Brg1DsatIoc.DsatSwAU ||
         L_BridgeRebf.Brg1DsatIoc.DsatSwBU || L_BridgeRebf.Brg1DsatIoc.DsatSwCU )
    {
        L_DesatFlt = true;
    }
    else
    {
         L_DesatFlt = false;
    }

    L_IocPhsA = L_BridgeRebf.Brg1DsatIoc.IocPhsA;
    L_IocPhsB = L_BridgeRebf.Brg1DsatIoc.IocPhsB;
    L_IocPhsC = L_BridgeRebf.Brg1DsatIoc.IocPhsC;

    // fpga fault indications for the DB
    if (L_BridgeRebf.Brg1DsatIoc.DsatSwD && PARM(L_BrgDbEnb))
    {
        L_DSatDx = true;
    }
    else if ( Brg1TripFlt == false )    // clear Desat fault indication when no bridge is tripped
    {
        L_DSatDx = false;
    }
    return;

}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side task 1 instrumentation functions
//
// EXECUTION LEVEL
//      Task1       - note: this function should be called after GmIx(y)Fbk
//                          have been calculated.
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineInstrumentation2T1(void)
{
    float   GmIxFbkZ, GmIyFbkZ;

    // current values centered 1dt in the past, to align I*R and L*di/dt terms
    GmIxFbkZ = 0.5F*(GmIxFbkZ1 + GmIxFbk);
    GmIyFbkZ = 0.5F*(GmIyFbkZ1 + GmIyFbk);
    GmVxFbkZ = 0.5F*(GmVxFbkZ1 + GmVxFbk);
    GmVyFbkZ = 0.5F*(GmVyFbkZ1 + GmVyFbk);

    // 690V secondary leakage voltage calculation
    L_VxLkgSec = (GmIxFbkZ * PARM(L_RxfmrSec)) - (PARM(L_LxfmrSec) * GmIyFbkZ * GmPllW) +
                 (PARM(L_LxfmrSec) * (GmIxFbk - GmIxFbkZ1) * L_DelTm1Rcp);

    L_VyLkgSec = (GmIyFbkZ * PARM(L_RxfmrSec)) + (PARM(L_LxfmrSec) * GmIxFbkZ * GmPllW) +
                 (PARM(L_LxfmrSec) * (GmIyFbk - GmIyFbkZ1) * L_DelTm1Rcp);

    // Primary leakage voltage calculation due to line current, referred to 690 volts
    L_VxLkgPri = (GmIxFbkZ * PARM(L_RxfmrPri)) - (PARM(L_LxfmrPri) * GmIyFbkZ * GmPllW) +
                 (PARM(L_LxfmrPri) * (GmIxFbk - GmIxFbkZ1) * L_DelTm1Rcp);

    L_VyLkgPri = (GmIyFbkZ * PARM(L_RxfmrPri)) + (PARM(L_LxfmrPri) * GmIxFbkZ * GmPllW) +
                 (PARM(L_LxfmrPri) * (GmIyFbk - GmIyFbkZ1) * L_DelTm1Rcp);


    GmIxFbkZ1 = GmIxFbk;
    GmIyFbkZ1 = GmIyFbk;
    GmVxFbkZ1 = GmVxFbk;
    GmVyFbkZ1 = GmVyFbk;


    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side task 2 instrumentation functions
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineInstrumentationT2(void)
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
int LineProductRules(void)
{
    int Status = SUCCESS;

    //-------------------------------------------------------------------------
    // include the product specific rules
    #include "p2ProductRule.inc"

    GmVcomAbsThrs = PARM(GmVcomOkThrsh) * PARM(L_VxfmrSec);

    if ( L_BridgeRebf.RunRules()       == FAIL )  Status = FAIL;

    L_IsensorScl = L_BridgeRebf.WhatIsSclCurrent(1);   // bridge 1 current scale in Amp/Hz
    L_BrgIocThrs = L_BridgeRebf.WhatIsIocCurrent(1);   // bridge 1 current Ioc threshold in Amps
    SclLinkVolts = L_BridgeRebf.WhatIsVdcScale(1);
    VdbVcoFreq   = L_BridgeRebf.WhatIsVcoFmax();
    // Phase Imbalance Thresholds
    // Quantization level line to line voltage is (2*L_VabVcoScl)/L_DelTm1
    // (2 quantas at L_DelTm1 rate
    // 1/sqrtf(3)--> line to neutral voltage
    // sqrtf(2) --> (2 sensors in quadrature)
    // 0.5 compensates for averager in line reconstruction
    L_VnoiseLvl  = L_BridgeRebf.WhatIsSclPhaseVolts(1) * SQRT2 * SQRT3_RCP * 2.0F *
                   L_DelTm1Rcp * 0.5F;

    if ( L_CellTest.RunRules()          == FAIL )  Status = FAIL;
    if ( L_LinePulseTest.RunRules()     == FAIL )  Status = FAIL;
    if ( L_PwmMod2Async.RunRules()      == FAIL )  Status = FAIL;
    if ( L_BrgThrm5pModel.RunRules()    == FAIL )  Status = FAIL;
    if ( L_BrgThrmstr.RunRules()        == FAIL )  Status = FAIL;
    if ( L_DbCelTst.RunRules()          == FAIL )  Status = FAIL;
    if ( L_DbPlsTst.RunRules()          == FAIL )  Status = FAIL;
    if ( L_DbThrmModel.RunRules()          == FAIL )  Status = FAIL;
    if ( L_DbCtl.RunRules()             == FAIL )  Status = FAIL;
    return (Status);
}

