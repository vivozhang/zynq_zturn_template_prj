///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Product Definition File
//
// DESCRIPTION:
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      p0Product.xls, p0Product.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
#include "x0BridgeInterface.h"
#include "x0CaptureBuf.h"
#include "x0Vector.h"
// product
#include "Product.h"
#include "b0BridgeRebf.h"
#include "p0EsaBuf.h"
#include "p0Thrmstr.h"

// Constants
//----------
#define NOT_APPLICABLE 0
#define PWR_MARGIN 200

// Externals
//----------
extern cCaptureBuf  CaptureBuf;
extern cTripBuf     TripBuf;
extern unsigned     PwrDnFbkDsb;
extern unsigned     Ps24vOk;
extern unsigned     L_Running;
extern unsigned     CabCfgType;
extern unsigned     GenAeadPresent;
extern unsigned     SimRunRuleReq;


extern void         L_CalculateTimebase();
extern void         R_CalculateTimebase();

extern void         L_ProcessIPN(void);
extern void         R_ProcessIPN(void);
extern void         X_ProcessIPN(void);

extern int          RunCommonRules(int Rule_Set);
extern int          RunLineBILRules(int Rule_Set);
extern int          RunLineLCLRules(int Rule_Set) ;
extern int          RunCommonAILRules(int Rule_Set);
extern int          RunLineAILRules(int Rule_Set);
extern int          RunRotorBILRules(int Rule_Set);
extern int          RunRotorLCLRules(int Rule_Set);
extern int          RunRotorAILRules(int Rule_Set);
extern int          RunRuleSet(int Rule_Set);
extern int          LineProductRules(void);
extern int          RotorProductRules(void);
extern int          SimRules(void);


extern cTripBuf     TripBuf;
extern cBridgeRebf  L_BridgeRebf;
extern cBridgeRebf  R_BridgeRebf;

extern WORD         FrcSchedMask;
extern float        FrcFreq;

extern unsigned     TempFbkRdy;
extern unsigned     TempIsValid;
extern unsigned     CCUParmRdy;
extern unsigned     ThrmOpnFlg;
extern unsigned     HtrFanRunOk;
extern unsigned     CfcOpenLoopReq;
extern unsigned     PdcOpenLoopReq;
extern unsigned     LqdOpenLoopReq;
extern float        DelTm3;
extern float        L_DelTm1;
extern float        L_DelTm1Rcp;
extern float        L_TempVltTh1Lw;
extern float        L_TempVltTh2Lw;
extern float        L_TempVltTh7Lw;
extern float        L_TempVltTh8Lw;
extern float        L_TempVltTh1Hi;
extern float        L_TempVltTh2Hi;
extern float        L_TempVltTh7Hi;
extern float        L_TempVltTh8Hi;
extern float        R_TempVltTh1Lw;
extern float        R_TempVltTh2Lw;
extern float        R_TempVltTh7Lw;
extern float        R_TempVltTh8Lw;
extern float        R_TempVltTh1Hi;
extern float        R_TempVltTh2Hi;
extern float        R_TempVltTh7Hi;
extern float        R_TempVltTh8Hi;
extern float        R2_TempVltTh1Lw;
extern float        R2_TempVltTh2Lw;
extern float        R2_TempVltTh7Lw;
extern float        R2_TempVltTh8Lw;
extern float        R2_TempVltTh1Hi;
extern float        R2_TempVltTh2Hi;
extern float        R2_TempVltTh7Hi;
extern float        R2_TempVltTh8Hi;

extern float        GmIxFbkT2;
extern float        GmIyFbkT2;

extern float        QregCmd;
extern float        GmPwrT2;
extern float        GmQPwrT2;
extern float        GmVxFbkZT2;
extern float        GmVyFbkZT2;
extern float        S_VxLkgSecT2;
extern float        S_VxLkgPriT2;
extern float        L_VxLkgPriT2;
extern float        S_VyLkgSecT2;
extern float        S_VyLkgPriT2;
extern float        L_VyLkgPriT2;
extern float        L_VxLkgSecT2;
extern float        L_VyLkgSecT2;
extern float        GmCosAngFbT2;
extern float        GmSinAngFbT2;
extern float        PLCPwrNP;
extern float        L_VlnInRcp;
extern float        GmVanT2;
extern float        GmVbnT2;
extern float        GmVcnT2;

extern float        PdcTemp;
extern float        CfcTemp;

extern unsigned     SysTmSec;
extern float        R_LegTestLeqv;

// temoprary work around code along with line 588
extern unsigned     PwrDnFbkDsb;
extern unsigned     SimMode;

extern unsigned     CableLimType;
extern float        PwrLimSpdRatRem;
extern unsigned     InitDone;
extern unsigned     IpcInterfaceOk;
extern unsigned     CCUParmRdy;
// Types and Classes
//------------------


// Parameters
//-----------
//------------------------------------------------------------------------------
// The parameters below are high level parameters to be calculated
// very early in the rule calculations so that other rule sets throughout the
// code have them avaliable. "System" here means everything outside of the
// converter that pertain to the turbine electrical system.
//
// System command
CREATE_PARM(LclTrqRef,          float);     // Local torqure reference
CREATE_PARM(LclVarRef,          float);     // Local var reference
CREATE_PARM(LclPfRef,           float);     // Local power factor reference
// System
CREATE_PARM(ConverterLoc,      unsigned);          // Converter physical location
// Grid
CREATE_PARM(GridStrength,       unsigned);  // User parameter
CREATE_PARM(SeriesCmpEnb,       unsigned);  // User parameter
CREATE_PARM(GridHighZ,          unsigned);  // FV parameter true if either of above are set
CREATE_PARM(AVRPresent,         unsigned);  // Auxillary voltage regulator present
CREATE_PARM(VoltExtendOpt,      unsigned);  // Parameter to control voltage levels based of 2.3 IPN
// Converter Rating
CREATE_PARM(R_IRat,        float);   // Rated current
CREATE_PARM(R_ShuntRat,    float);   // Rotor bridge rated shunt, amps at 50 mV
CREATE_PARM(R_PhDShuntRat, float);   // Phase-D rated shunt, amps at 50 mV
CREATE_PARM(R_VdcNom,      float);   // Nominal Vdc
CREATE_PARM(L_VllIn,            float);     // Line input voltage (rms);
CREATE_PARM(L_VllRms,           float);     // Rms voltage of drives using source's out
CREATE_PARM(L_LineFrq,          float);     // Grid line frequency in radians
CREATE_PARM(L_MaxCur,      float);  // Maximum line conveter current
CREATE_PARM(DFIRat,             float);     // Distortion filter rated current
CREATE_PARM(DFConvCurr,         float);     // Distortion filter current provided by the converter.
CREATE_PARM(AuxPwrNom,          float);     // Auxillary nominal power
//------------------------------------------------------------------------------
// Pwr supply buffer check
CREATE_PARM(PwrDnBufSavDsb,    unsigned);          // Buffer Save Disable (Diagnostic FV)
CREATE_PARM(PwrDnBufSavFrc,    unsigned);          // Buffer Save Force   (Diagnostic FV)

//------------------------------------------------------------------------------
// Bridge Hardware
CREATE_PARM(BrgVdcNom,          float);     // bridge nominal full dc link voltage
CREATE_PARM(BrgLockoutTm,       float);     // bridge lockout time  (Sec)
CREATE_PARM(BrgCdclink,         float);     // bridge DC link capacitance
// Line Converter Hardware
CREATE_PARM(CnvLineL,           float);     // line converter inductance, reactor+bridge (Henries)
CREATE_PARM(CnvLineR,           float);     // line converter resistance, reactor+bridge (Ohms)
CREATE_PARM(CnvLineDfC,         float);     // line distortion filter capacitance, wye equivalent (Farads)
CREATE_PARM(CnvLineDfR,         float);     // line distortion filter resistance in series with DFC1 (Ohms)
CREATE_PARM(L_RISqrR,           float);     // Line  circuit resistance
CREATE_PARM(Gm_RISqrR,          float);     // Grid  circuit resistance from transformer secondary to S/L common point
CREATE_PARM(DFRISqrR,           float);     // Distortion filter equivalent resistance for use with TocHiSel rms current
CREATE_PARM(L_RIndCore,         float);     // Line inductor core eq resistance.
CREATE_PARM(L_CTot,             float);     // total source capacitance
CREATE_PARM(L_LTot,             float);     // total input inductance(FV);
CREATE_PARM(L_RTot,             float);     // total input resistance(FV);

//------------------------------------------------------------------------------
// Rotor Converter Hardware
CREATE_PARM(CnvRotorL,          float);     // rotor converter inductance, reactor+bridge (Henries)
CREATE_PARM(CnvRotorAltLpu,     float);     // L modifier for alternate configuration (pu)
CREATE_PARM(CnvRotorR,          float);     // rotor converter resistance, reactor+bridge (Ohms)
CREATE_PARM(R_RIndCore,         float);     // Rotor inductor core eq resistance.
CREATE_PARM(R_RISqrR,           float);     // Rotor converter circuit resistance outside generator and cables

//------------------------------------------------------------------------------
// Transformer data

CREATE_PARM(VxfmrPri  ,           float);          // xmfr primary voltage     (Vrms)
CREATE_PARM(S_XfmrKva,            float);          // xmfr secondary stator apparent power (Kva)
CREATE_PARM(L_VxfmrSec,           float);          // xmfr secondary line converter widing voltage (from product)  (Volts)





CREATE_PARM(XfmrKva,              float);          // xmfr apparent power (kVA)
CREATE_PARM(L_RxfmrPri,           float);          // xfmr primary resistance referring to line side
CREATE_PARM(L_LxfmrPri,           float);          // xfmr primary inductance referring to line side
CREATE_PARM(XfmrZpct,             float);          // xfmr short circuit impedance (%)

CREATE_PARM(RxfmrPri  ,           float);          // xmfr primary resistance  (Ohms)
CREATE_PARM(LxfmrPri  ,           float);          // xmfr primary inductance  (Henrys)
CREATE_PARM(S_VxfmrSec,           float);          // xmfr secondary stator widing voltage     (Vrms)
CREATE_PARM(L_KVABasXfmr,         float);          // xmfr secondary line winding apparent power (Kva)
CREATE_PARM(L_LxfmrSec,           float);          // xmfr secondary line converter winding inductance  (Henrys)
CREATE_PARM(L_RxfmrSec,           float);          // xmfr secondary line winding resistance  (Ohms)

CREATE_PARM(GmVxyGridLd,          float);          // lead comp on GmVx(y)Grid calcs
//------------------------------------------------------------------------------
// Generator data
CREATE_PARM(GenTypeCfg,           int);            // Generator type
CREATE_PARM(GenTypeNewCfg,        int);            // Generator file number
CREATE_PARM(GenTreFile,           unsigned);       // Generator tre file supplied
CREATE_PARM(S_Volts,              float);          // Stator Rated Volts
CREATE_PARM(S_Amps,               float);          // Stator Rated Amps
CREATE_PARM(S_Hz,                 float);          // Stator Rated Frequency Hz
CREATE_PARM(S_Power,              float);          // Generator Rated Power
CREATE_PARM(S_PwrFactor,          float);          // Generator Rated Power Factor
CREATE_PARM(S_RRatio,             float);          // Vs/Vr at zero spd
CREATE_PARM(S_X1,                 float);          // Stator Leakage reactance [Omh]
CREATE_PARM(S_X2,                 float);          // Rotor Leakage reactance referred to stator [Omh]
CREATE_PARM(S_Xm,                 float);          // Magnetizing reactance seen from stator [Omh]
CREATE_PARM(S_XST,                float);          // Saturation leakage reactance (estimated reactance at startup) [Ohm]
CREATE_PARM(S_R1,                 float);          // Stator resistance [Omh]
CREATE_PARM(S_R2,                 float);          // Rotor resistance referred to stator [Omh]
CREATE_PARM(R_RPMMinOp,           float);          // Generator minimun operational speed [RPM]
CREATE_PARM(R_RPMMaxStart,        float);          // Max generator speed for the converter to be ready [RPM]
CREATE_PARM(R_RPMSync,            float);          // Generator synchronous speed [RPM]
CREATE_PARM(R_RPMNom,             float);          // Generator nominal speed (i.e. RPM at which nominal torque yields nominal power);
CREATE_PARM(R_RPMMaxOp,           float);          // Generator maximum operational speed [RPM]
CREATE_PARM(R_TachPPR,            int);            // Tachometer pulses per revolution
CREATE_PARM(R_GenInertia,         float);          // Generator inertia [kg*m^2]
CREATE_PARM(R_LSigma,             float);          // leakage inductance (run);
CREATE_PARM(R_LSigmaSt,           float);          // leakage inductance (start);

//Saturation curve
CREATE_PARM(S_GenSatCurvVld ,unsigned);            // Generator saturation curve is valid
CREATE_PARM(S_SatCurvFlxMax ,float);
CREATE_PARM(S_SatCurvFlxMin ,float);
CREATE_PARM(S_SatCurvXmMax  ,float);
CREATE_PARM(S_SatCurvXmMin  ,float);
CREATE_PARM(S_SatCurvK0     ,float);
CREATE_PARM(S_SatCurvK1     ,float);
CREATE_PARM(S_SatCurvK2     ,float);
CREATE_PARM(S_SatCurvK3     ,float);
//------------------------------------------------------------------------------
// Speed and Temp Current Limit curve
CREATE_PARM(R_ISpdTempNomT,           float);          // Nominal temperature as input for generator current limit calculation, 25.0degc;
//Old polynomial
// I(T,N,A) = (mt*T+Bt)*[(mN*N+Bn)/(mN*Nm+bN)]*(mA*A+bA)
//T = temperature, N = generator speed, A = altitude
CREATE_PARM(R_ISpdTempNm,         float);          // Generator rotor current limit speed/temp dependent nominal speed
CREATE_PARM(R_ISpdTempBn,         float);          // Generator rotor current limit speed/temp dependent speed constant
CREATE_PARM(R_ISpdTempMn,         float);          // Generator rotor current limit speed/temp dependent speed slope
CREATE_PARM(R_ISpdTempBt,         float);          // Generator rotor current limit speed/temp dependent temperature constant
CREATE_PARM(R_ISpdTempMt,         float);          // Generator rotor current limit speed/temp dependent temperature slope
CREATE_PARM(R_ISpdTempBa,         float);          // Generator rotor current limit speed/temp dependent altitude constant
CREATE_PARM(R_ISpdTempMa,         float);          // Generator rotor current limit speed/temp dependent altitude slope
//New polynomial, 2016-1-27, SZ
//I(T,N,A) = (A1*N+A2*T+A3*N^3+A4*N^2+A5*N^3*T+A6*N^2*T+A7*N*T+A8*N^3*T^2+A9*N^2*T^2+A10*N*T^2+A11*N^4*T+A12*N^4*T^2+A13)*(mA*A+bA)
//T = temperature, N = generator speed, A = altitude
CREATE_PARM(R_ISpdTempA1,         float);          // Generator rotor current limit speed/temp dependent coefficient A1
CREATE_PARM(R_ISpdTempA2,         float);          // Generator rotor current limit speed/temp dependent coefficient A2
CREATE_PARM(R_ISpdTempA3,         float);          // Generator rotor current limit speed/temp dependent coefficient A3
CREATE_PARM(R_ISpdTempA4,         float);          // Generator rotor current limit speed/temp dependent coefficient A4
CREATE_PARM(R_ISpdTempA5,         float);          // Generator rotor current limit speed/temp dependent coefficient A5
CREATE_PARM(R_ISpdTempA6,         float);          // Generator rotor current limit speed/temp dependent coefficient A6
CREATE_PARM(R_ISpdTempA7,         float);          // Generator rotor current limit speed/temp dependent coefficient A7
CREATE_PARM(R_ISpdTempA8,         float);          // Generator rotor current limit speed/temp dependent coefficient A8
CREATE_PARM(R_ISpdTempA9,         float);          // Generator rotor current limit speed/temp dependent coefficient A9
CREATE_PARM(R_ISpdTempA10,        float);          // Generator rotor current limit speed/temp dependent coefficient A10
CREATE_PARM(R_ISpdTempA11,        float);          // Generator rotor current limit speed/temp dependent coefficient A11
CREATE_PARM(R_ISpdTempA12,        float);          // Generator rotor current limit speed/temp dependent coefficient A12
CREATE_PARM(R_ISpdTempA13,        float);          // Generator rotor current limit speed/temp dependent coefficient A13
//Old polynomial
// I(T,N,A) = (mt*T+Bt)*[(mN*N+Bn)/(mN*Nm+bN)]*(mA*A+bA)
//T = temperature, N = generator speed, A = altitude
CREATE_PARM(S_ISpdTempNm,         float);          // Generator stator current limit speed/temp dependent nominal speed
CREATE_PARM(S_ISpdTempBn,         float);          // Generator stator current limit speed/temp dependent speed constant
CREATE_PARM(S_ISpdTempMn,         float);          // Generator stator current limit speed/temp dependent speed slope
CREATE_PARM(S_ISpdTempBt,         float);          // Generator stator current limit speed/temp dependent temperature constant
CREATE_PARM(S_ISpdTempMt,         float);          // Generator stator current limit speed/temp dependent temperature slope
//New polynomial, 2016-1-27, SZ
//I(T,N,A) = (A1*N+A2*T+A3*N^3+A4*N^2+A5*N^3*T+A6*N^2*T+A7*N*T+A8*N^3*T^2+A9*N^2*T^2+A10*N*T^2+A11*N^4*T+A12*N^4*T^2+A13)*(mA*A+bA)
//T = temperature, N = generator speed, A = altitude
CREATE_PARM(S_ISpdTempA1,         float);          // Generator stator current limit speed/temp dependent coefficient A1
CREATE_PARM(S_ISpdTempA2,         float);          // Generator stator current limit speed/temp dependent coefficient A2
CREATE_PARM(S_ISpdTempA3,         float);          // Generator stator current limit speed/temp dependent coefficient A3
CREATE_PARM(S_ISpdTempA4,         float);          // Generator stator current limit speed/temp dependent coefficient A4
CREATE_PARM(S_ISpdTempA5,         float);          // Generator stator current limit speed/temp dependent coefficient A5
CREATE_PARM(S_ISpdTempA6,         float);          // Generator stator current limit speed/temp dependent coefficient A6
CREATE_PARM(S_ISpdTempA7,         float);          // Generator stator current limit speed/temp dependent coefficient A7
CREATE_PARM(S_ISpdTempA8,         float);          // Generator stator current limit speed/temp dependent coefficient A8
CREATE_PARM(S_ISpdTempA9,         float);          // Generator stator current limit speed/temp dependent coefficient A9
CREATE_PARM(S_ISpdTempA10,        float);          // Generator stator current limit speed/temp dependent coefficient A10
CREATE_PARM(S_ISpdTempA11,        float);          // Generator stator current limit speed/temp dependent coefficient A11
CREATE_PARM(S_ISpdTempA12,        float);          // Generator stator current limit speed/temp dependent coefficient A12
CREATE_PARM(S_ISpdTempA13,        float);          // Generator stator current limit speed/temp dependent coefficient A13

// User "special" generator parameters from importable file
CREATE_PARM( GenNewCfg,     int     );   // Generator file number
CREATE_PARM( S_VoltsNw,       float   );   // Stator Rated Volts
CREATE_PARM( S_AmpsNw,        float   );   // Stator Rated Amps
CREATE_PARM( S_HzNw,          float   );   // Stator Rated Frequency Hz
CREATE_PARM( S_PowerNw,       float   );   // Generator Rated Power
CREATE_PARM( S_PwrFactorNw,   float   );   // Generator Rated Power Factor
CREATE_PARM( S_RRatioNw,      float   );   // Vs/Vr at zero Nwd
CREATE_PARM( S_X1Nw,          float   );   // Stat Leakage Reactance
CREATE_PARM( S_X2Nw,          float   );   // Rot Leakage Reactance
CREATE_PARM( S_XmNw,          float   );   // MagnetizNwg Reactance
CREATE_PARM( S_XSTNw,         float   );   // Sat leakage reactace
CREATE_PARM( S_R1Nw,          float   );   // r1, stator resistance
CREATE_PARM( S_R2Nw,          float   );   // r2, rotor resistance
CREATE_PARM( R_RPMMinOpNw,    float   );   // Motor mNwimun opera   tional Nweed (RPM);
CREATE_PARM( R_RPMSyncNw,     float   );   // Motor synchronous Nweed
CREATE_PARM( R_RPMNomNw,      float   );   // Motor nomNwal Nweed (i.e. RPM at which nomNwal torque yields nomNwal power);
CREATE_PARM( R_RPMMaxOpNw,    float   );   // Motor maximum operational Nweed (RPM);
CREATE_PARM( R_TachPPRNw,     float   );   // Tachometer pulses per revolution
CREATE_PARM( R_GenInertiaNw,  float   );   // Generator inertia -- kg*m^2/rad
CREATE_PARM( R_ISpdTpBnNw,    float   );   //
CREATE_PARM( R_ISpdTpNmNw,    float   );   //
CREATE_PARM( R_ISpdTpMnNw,    float   );   //
CREATE_PARM( R_ISpdTpBtNw,    float   );   //
CREATE_PARM( R_ISpdTpMtNw,    float   );   //
CREATE_PARM( S_ISpdTpBnNw,    float   );   //
CREATE_PARM( S_ISpdTpNmNw,    float   );   //
CREATE_PARM( S_ISpdTpMnNw,    float   );   //
CREATE_PARM( S_ISpdTpBtNw,    float   );   //
CREATE_PARM( S_ISpdTpMtNw,    float   );   //

CREATE_PARM( S_ISpdTpMaNw,    float   );   //
CREATE_PARM( S_ISpdTpBaNw,    float   );   //
CREATE_PARM( R_ISpdTpMaNw,    float   );   //
CREATE_PARM( R_ISpdTpBaNw,    float   );   //



//------------------------------------------------------------------------------
// Turbine system application data
CREATE_PARM(MaxTrbPwrIPN,           float);        // Turbine max rated power, converter
CREATE_PARM(MinTrbPwrIPN,           float);        // Turbine min rated power, converter
CREATE_PARM(TurbineNpRat,           float);        // Turbine rated power  (kW)
CREATE_PARM(TurbineOvrldPwr,        float);        // Turbine overload power (kW)
CREATE_PARM(TurbineOvrldSpd,        float);        // Turbine overload speed (rpm)
CREATE_PARM(SysPwrFact,             float);        // Rated grid side power factor
CREATE_PARM(SysPower,               float);        // System Rated Power (Watts)
CREATE_PARM(SysVoltNom,             float);        // System voltage nominal(Vrms)
CREATE_PARM(SysVoltMax,             float);        // System voltage maximum (Vrms)
CREATE_PARM(SysVoltMin,             float);        // System voltage minimum (Vrms)
CREATE_PARM(SysVARat,               float);        // System VA rating (VA)
CREATE_PARM(SysInom,                float);        // Nominal current for the converter (Arms)
CREATE_PARM(SysINomGrid,            float);        // Nominal current for the converter for grid events (Arms)
CREATE_PARM(SysIRatLv,              float);        // System current rating for low voltage (Arms)
CREATE_PARM(SysIyRat,               float);        // System reactive current rating (Arms)
CREATE_PARM(SysIOVLoad,             float);        // System current over-load (%)
CREATE_PARM(SysFreqNom,             float);        // System nominal frequency (Hz)
CREATE_PARM(SysIratStp,             float);        // System rated current step (Arms)
CREATE_PARM(SysZBase,               float);        // System impedance base
CREATE_PARM(SysLBase,               float);        // System inductance base
CREATE_PARM(SysCBase,               float);        // System capacitance base
CREATE_PARM(RpnwRatFv,              float);        // Converter RPNW KVAR Rating


//------------------------------------------------------------------------------
// DB and Dc-Chopper Hardware
CREATE_PARM(BrgDbR,               float);          // bridge DB resitance for one circuit (Ohms)
CREATE_PARM(BrgDbL,               float);          // bridge DB parasitic inductance for one circuit (Henrys)
CREATE_PARM(BrgDbPwrCont,         float);          // bridge DB continuous power (watts)
CREATE_PARM(BrgDbPwrImp,          float);          // bridge DB impulse power (watts)
CREATE_PARM(BrgDbImpTm,           float);          // bridge DB impulse time (secs)
CREATE_PARM(BrgDbRthrm,           float);          // bridge DB resistor thermal resistance, with fan (deg C/watt)
CREATE_PARM(DbVmax,               float);          // maximum DC link voltage for DB to kick in (Volts)

//------------------------------------------------------------------------------
// fan1
CREATE_PARM(LqdTempFanMax      ,  float);          // liquid max temperature allow for the fan1(deg C)
CREATE_PARM(LqdTempFanMin      ,  float);          // liquid min temperature allow for the fan1(deg C)
CREATE_PARM(LqdTempFanRiseSetp ,  float);          // liquid temperature rising set point for fan1 operation(deg C)


CREATE_PARM(PdcTempFanMax     ,   float);          // Pdc max temperature allow for the fan2 (deg C)
CREATE_PARM(PdcTempFanMin     ,   float);          // Pdc min temperature allow for the fan2 (deg C)
CREATE_PARM(PdcTempFanRiseSetp,   float);          // Pdc temperature rising set point for fan2 operation(deg C)
CREATE_PARM(PdcTempFanSpdGn   ,   float);          // Pdc fan2 speed control gain(pu/deg C)

CREATE_PARM(LqdTempOkLvl,         float);          // liquid temperature is ok for pump piping the liquid (deg C)
CREATE_PARM(LqdTempLowLvl,        float);          // liquid temperature is too low for pump piping the liquid (deg C)

CREATE_PARM( WTempIoThrm,         float);          // Omega for thermal I/O voltage from VCOs
CREATE_PARM( WHumidData,          float);          // Omega for humidity data

//------------------------------------------------------------------------------
// Test Modes
CREATE_PARM(TestMode,             int);            // Enumerated picks may include pumpback, legtest, ....
CREATE_PARM(ConfigForSep,         unsigned);       // Select configuration for software evaluation parameter (SEP)
CREATE_PARM(Q1UvHldOpnTst,      unsigned);      // Used to open Q1 for Salem Lab tests
CREATE_PARM(GmVltCalXfmrWndSel,     int);          // Transformer winding voltage select for primary voltage estimation

CREATE_PARM(InitBufSavArmTm,    float);
// Wizard
CREATE_PARM(CommWizSel1,        unsigned);  // Commissioning wizard select
CREATE_PARM(Oscill,             float);     // CPU Crystall Oscillator (Hertz)

CREATE_PARM(GenAeadThrm1Enb,   unsigned); //Gen Aead Thermistor 1 Enable
CREATE_PARM(GenAeadThrm2Enb,   unsigned); //Gen Aead Thermistor 2 Enable

// Published Variables
//--------------------
CREATE_PUBVAR(ISimMode,         unsigned);          //  Flag indicate ISimMode
//------------------------------------------------------------------------------
// Power supply
CREATE_PUBVAR(PwrDnBufSavArm,   unsigned)=false;   // Buffer Save Armed
CREATE_PUBVAR(PwrDnBufSavTrig,  unsigned)=false;   // Buffer Save Trigger
CREATE_PUBVAR(PwrDnBufSavDone,  unsigned)=false;   // Buffer Save Done

//------------------------------------------------------------------------------
// Bridge Hardware
CREATE_PUBVAR(MinPulseCnt,          int);          // Minimum Pulse
CREATE_PUBVAR(BrgLockoutTime,       float);        // Bridge lockout time for display purposes (Sec)

//------------------------------------------------------------------------------
// Turbine system application data
CREATE_PUBVAR(TurbNpRatWtt,       float);
CREATE_PUBVAR(TurbNpRatWRcp,      float);
CREATE_PUBVAR(TurbNpRatRcp,       float);
CREATE_PUBVAR(TurbineOvrldPwrPu,  float);
CREATE_PUBVAR(TurbineOvrldTrq,    float);
CREATE_PUBVAR(SysPowerx,          float);
CREATE_PUBVAR(SysPwrFactx,        float);
CREATE_PUBVAR(SysVoltNomx,        float);
CREATE_PUBVAR(INomx,              float);
CREATE_PUBVAR(SysVoltMaxx,        float);
CREATE_PUBVAR(SysVoltMinx,        float);
CREATE_PUBVAR(SysVARatx,          float);
CREATE_PUBVAR(SysIRatx,           float);
CREATE_PUBVAR(SysIRatLvx,         float);
CREATE_PUBVAR(SysIyRatx,          float);
CREATE_PUBVAR(SysIOVLoadx,        float);
CREATE_PUBVAR(SysFreqNomx,        float);
CREATE_PUBVAR(INomGridPk,         float);
CREATE_PUBVAR(SysWnom,            float);   // system frquency in rads/sec
CREATE_PUBVAR(AlttdTurbRem,    float);    // Altitude of turbine from IoNet


CREATE_PUBVAR(InvIPNPwrFlg,       unsigned);
CREATE_PUBVAR(NoCabFileFlg,       unsigned);
CREATE_PUBVAR(CbFrqMsMcFlg,       unsigned);
CREATE_PUBVAR(TbGnPMsMcFlg,       unsigned);
CREATE_PUBVAR(CbRqPrmBdFlg,       unsigned);

//------------------------------------------------------------------------------
// Transformer data
CREATE_PUBVAR(XfmrRatioLnStr,       float);        // xfmr voltage ratio from line to stator windings
CREATE_PUBVAR(XfmrRatioLnStrRcp,    float);        // reciprocal of xfmr voltage ratio from line to stator windings
CREATE_PUBVAR(GmVxGrid ,            float);        // (Estimated) grid voltage, X-axis
CREATE_PUBVAR(GmVyGrid ,            float);        // (Estimated) grid voltage, Y-axis
CREATE_PUBVAR(GmVxGrid0Z,           float);        // (Estimated) grid voltage, X-axis; preliminary calc, Z^-1
CREATE_PUBVAR(GmVyGrid0Z,           float);        // (Estimated) grid voltage, Y-axis; preliminary calc, Z^-1
CREATE_PUBVAR(GmVabGrid ,           float);        // (Estimated) grid voltage, A-B
CREATE_PUBVAR(GmVbcGrid ,           float);        // (Estimated) grid voltage, B-C
CREATE_PUBVAR(GmVcaGrid ,           float);        // (Estimated) grid voltage, C-A
CREATE_PUBVAR(GmVanGrid ,           float);        // (Estimated) grid voltage, A-N
CREATE_PUBVAR(GmVbnGrid ,           float);        // (Estimated) grid voltage, B-N
CREATE_PUBVAR(GmVcnGrid ,           float);        // (Estimated) grid voltage, C-N
CREATE_PUBVAR(GmVMagGrid,           float);        // (Estimated) grid voltage magnitude (volts, ref to 690)
CREATE_PUBVAR(GmVMagGridPu,         float);        // (Estimated) grid voltage magnitude (PU)

CREATE_PUBVAR(GmIxGrid  ,           float);        // net (Gm + S_Gm) Ix, refered to 690 v
CREATE_PUBVAR(GmIyGrid  ,           float);        // net (Gm + S_Gm) Iy, refered to 690 v
CREATE_PUBVAR(GmPwrGridT2 ,         float);        // net (Gm + S_Gm) power
CREATE_PUBVAR(GmQPwrGridT2,         float);        // net (Gm + S_Gm) reactive power
CREATE_PUBVAR(GmPwrGridT3 ,         float);        // net (Gm + S_Gm) power          (T3)
CREATE_PUBVAR(GmQPwrGridT3,         float);        // net (Gm + S_Gm) reactive power (T3)
CREATE_PUBVAR(GmPwrGrid  ,          float);        // net (Gm + S_Gm) power          (filtered for IoNet)
CREATE_PUBVAR(GmQPwrGrid ,          float);        // net (Gm + S_Gm) reactive power (filtered for IoNet)
CREATE_PUBVAR(GmSPwrGrid ,          float);        // net (Gm + S_Gm) apparent power (filtered for IoNet)
CREATE_PUBVAR(GmPwrFactorGrid ,     float);        // net (Gm + S_Gm) power factor   (filtered for IoNet)
CREATE_PUBVAR(GmLxfmDt,             float);        // xmfr 690V secondary winding inductance over one Task1 period (Ohms)

//------------------------------------------------------------------------------
// Generator data
CREATE_PUBVAR(GenType,              int);          // Mirror of P.GenTypeCfg
CREATE_PUBVAR(R_ISpdTmpRcp ,        float);
CREATE_PUBVAR(S_ISpdTmpRcp ,        float);
CREATE_PUBVAR(R_IMaxCrvRule,        float);
CREATE_PUBVAR(S_IMaxCrvRule,        float);
CREATE_PUBVAR(FrqGenMisMtch,     unsigned) = 0;    // Frequency selection mismatch between generator and grid
CREATE_PUBVAR(CabAltDeRtSF,         float);        // Cable altidude dependent cable derate (Amp.meter)

CREATE_PUBVAR(GenNew,    int);      // Mirror of P.GenNewCfg

//------------------------------------------------------------------------------
// Heater and fan related temperatures definitions
// filter action is done on the voltage when it is converted to temperature
CREATE_PUBVAR(InletAirTemp     ,  float) = 25.0F;  // air inlet temperature measured outside the converter control cabinet (deg C)
CREATE_PUBVAR(CfcTemp          ,  float) = 0.0F;  // converter idstortion filter temperature (deg C)
CREATE_PUBVAR(PdcTemp          ,  float) = 0.0F;  // converter AC entry temperature(deg C)

CREATE_PUBVAR(GenAeadTemp1     ,  float) = 0.0F;  // Gen AEAD Thermistor 1 temperature (deg C)
CREATE_PUBVAR(GenAeadTemp2     ,  float) = 0.0F;  // Gen AEAD Thermistor 2 temperature (deg C)
CREATE_PUBVAR(AclfuTemp        ,  float) = 30.0F; // AC line fuse bus temperature (deg C)
CREATE_PUBVAR(PdcCabEntryTemp  ,  float) = 30.0F;


CREATE_PUBVAR(LhslaTemp        ,  float) = 30.0F;  // line Hsla card temperature (deg C) need to verify the init
CREATE_PUBVAR(RhslaTemp        ,  float) = 30.0F;  // rotor Hsla card temperature (deg C) need to verify the init

CREATE_PUBVAR(LhslaTempUfil    ,  float) = 30.0F;  // unfiltered line Hsla card temperature (deg C) need to verify the init
CREATE_PUBVAR(RhslaTempUfil    ,  float) = 30.0F;  // unfiltered rotor Hsla card temperature (deg C) need to verify the init

CREATE_PUBVAR(HumidTempUfil    ,  float);
CREATE_PUBVAR(HumidPctUfil     ,  float);
CREATE_PUBVAR(HumidTemp        ,  float);
CREATE_PUBVAR(HumidPct         ,  float);

CREATE_PUBVAR(DbResCombined    ,  float);          // parallel combination of two DB resistors (Ohms)
CREATE_PUBVAR(BrgDbTauElec     ,  float);          // electrical time constant L/R for one DB circuit (Sec)
CREATE_PUBVAR(L_PwrDisconTm    ,  float);          // line bridge power disconnection time (Sec)
CREATE_PUBVAR(CoolantTemp,  float);     // Cooleant temperature (deg C)
CREATE_PUBVAR(MccTemp,      float);     // MCC temperature (deg C)

CREATE_PUBVAR(RpnwRat,          float);     // Converter RPNW KVAR Rating

CREATE_PUBVAR(InitDoneTmr  ,    unsigned)=0.0F;
CREATE_PUBVAR(InitDone1Sec ,    unsigned)=false;
CREATE_PUBVAR(LocalMode,        unsigned);        // Local  mode

// Unpublished Variables
//----------------------
unsigned          PwrDnBufSavFrcZ=false;
unsigned          PwrDnBufSavEdge=false;

SinCosTyp         GmSinCosFbT2;
FaultIsActiveTyp  FaultIsActive;

// Data Passing
//--------------
DATA_PASS(GmPwrGridT2,            GmPwrGridT3,            T2_T3, FILTERED);
DATA_PASS(GmQPwrGridT2,           GmQPwrGridT3,           T2_T3, FILTERED);

// Diagnostics
//------------
                                                   // Buffer PwrDn Save FailedSOE’s
CREATE_DIAG(CaptureBufSavFail);                    //   Capture Buffer PwrDn Save Failed
CREATE_DIAG(TripBufSavFail);                       //   Trip    Buffer PwrDn Save Failed

CREATE_DIAG(NoGenFileTrp);                     // Required DFIG type parameter unspecified
CREATE_DIAG(FrqSelMisMtchTrp);                     // Transformer, generator, and grid frequencies do not match

CREATE_DIAG(NoCabFile);
CREATE_DIAG(CbFrqMisMtch);
CREATE_DIAG(InvPLCPwrNP);
CREATE_DIAG(TbGnPMisMtch);
CREATE_DIAG(CabReqPrmBad);

// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------
cEsaBuf EsaBuf("DSP_EsaBuffer", 320000, 0);   // ~10 sec @ Task1A


//-----------------------------------------------------------------------------
// Thermistor processing (Line I/O 1)
cThrmstr::InListTyp LIo1_ThrmstrIn =
{
    L_TempVltTh1Lw,                //  1 - voltage at AEAD Din 1 Thermistor input, low range
    L_TempVltTh2Lw,                //  2 - voltage at AEAD Din 2 Thermistor input, low range
    L_TempVltTh7Lw,                //  3 - voltage at AEAD Din 7 Thermistor input, low range
    L_TempVltTh8Lw,                //  4 - voltage at AEAD Din 8 Thermistor input, low range
    L_TempVltTh1Hi,                //  5 - voltage at thermistor input 1, high range
    L_TempVltTh2Hi,                //  6 - voltage at AEAD Din 2 Thermistor input, high range
    L_TempVltTh7Hi,                //  7 - voltage at AEAD Din 7 Thermistor input, high range
    L_TempVltTh8Hi,                //  8 - voltage at AEAD Din 8 Thermistor input, high range
    TempFbkRdy,                    //  9 - Temperature feedbacks are valid
    True,                          // 10 - AEAD Din 1 Thermistor usage, true = used / false = unused
    True,                          // 11 - AEAD Din 2 Thermistor input usage, true = used / false = unused
    True,                          // 12 - AEAD Din 7 Thermistor input usage, true = used / false = unused
    True,                          // 13 - AEAD Din 8 Thermistor input usage, true = used / false = unused
    InletAirTemp,                  // 14 - Inlet air temperature as reference
};
cThrmstr::InsListTyp LIo_ThrmstrIns =
{
    DelTm3,                        //  1 - Task3 delta time
    True,                          //  2 - Feedback enable
};
cThrmstr LIo1_Thrmstr ( LIo1_ThrmstrIn, LIo_ThrmstrIns );
INSTANCE_INFO( LIo1_Thrmstr, BRG_UNIQ_L_IO1, BRG_NUM_L, TASK1 )

//-----------------------------------------------------------------------------
// Thermistor processing (Rotor I/O 1)
cThrmstr::InListTyp RIo1_ThrmstrIn =
{
    R_TempVltTh1Lw,               //  1 - voltage at AEAD Din 1 Thermistor input, low range
    R_TempVltTh2Lw,               //  2 - voltage at AEAD Din 2 Thermistor input, low range
    R_TempVltTh7Lw,               //  3 - voltage at AEAD Din 7 Thermistor input, low range
    R_TempVltTh8Lw,               //  4 - voltage at AEAD Din 8 Thermistor input, low range
    R_TempVltTh1Hi,               //  5 - voltage at thermistor input 1, high range
    R_TempVltTh2Hi,               //  6 - voltage at AEAD Din 2 Thermistor input, high range
    R_TempVltTh7Hi,               //  7 - voltage at AEAD Din 7 Thermistor input, high range
    R_TempVltTh8Hi,               //  8 - voltage at AEAD Din 8 Thermistor input, high range
    TempFbkRdy,                   //  9 - Temperature feedbacks are valid
    PARM(GenAeadThrm1Enb),        // 10 - AEAD Din 1 Thermistor usage, true = used / false = unused
    PARM(GenAeadThrm1Enb),        // 11 - AEAD Din 2 Thermistor input usage, true = used / false = unused
    True,                         // 12 - AEAD Din 7 Thermistor input usage, true = used / false = unused
    True,                         // 13 - AEAD Din 8 Thermistor input usage, true = used / false = unused
    InletAirTemp,                 // 14 - Inlet air temperature as reference
};
cThrmstr::InsListTyp RIo_ThrmstrIns =
{
    DelTm3,                       //  1 - Task3 delta time
    PARM(GenAeadPresent),         //  2 - Feedback enable
};
cThrmstr RIo1_Thrmstr ( RIo1_ThrmstrIn, RIo_ThrmstrIns );
INSTANCE_INFO( RIo1_Thrmstr, BRG_UNIQ_R_IO1, BRG_NUM_R, TASK1A )

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This function executes the top-level product rules via the
//      p0ProductRule.inc file and then calls additional product specific
//      rule functions as needed.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      Status to indicate if calculations were successful (SUCCESS/FAIL).
//
///////////////////////////////////////////////////////////////////////////////
int ProductRules(void)
{
    int Status = SUCCESS;

    #include "p0ProductRule.inc"

    //-------------------------------------------------------------------------
    // legacy timebase assignments
    L_CalculateTimebase();      // line side
    R_CalculateTimebase();      // rotor side

    //-------------------------------------------------------------------------
    // decode intelligent part numbers
    X_ProcessIPN();         // common

    for ( int i=0 ; i < 2 ; ++i )
    {
        // Do not change the order that these rules run!!
        if ( RunCommonRules(RULE_SET_MAIN)    == FAIL )  Status = FAIL;     // p0Rule3

        if ( RunLineBILRules(RULE_SET_MAIN)   == FAIL )  Status = FAIL;     // y2Rule
        if ( RunLineLCLRules(RULE_SET_MAIN)   == FAIL )  Status = FAIL;     // s0Rule

        if ( RunCommonAILRules(RULE_SET_MAIN) == FAIL )  Status = FAIL;     // p0Rule2

        if ( RunLineAILRules(RULE_SET_MAIN)   == FAIL )  Status = FAIL;     // p2Rule

        if ( RunRotorBILRules(RULE_SET_MAIN)  == FAIL )  Status = FAIL;     // y1Rule
        if ( RunRotorLCLRules(RULE_SET_MAIN)  == FAIL )  Status = FAIL;     // a0Rule
        if ( RunRotorAILRules(RULE_SET_MAIN)  == FAIL )  Status = FAIL;     // p1rule

    }

    if ( LineProductRules()  == FAIL )  Status = FAIL;                      // p2ProductRule.inc
    if ( RotorProductRules() == FAIL )  Status = FAIL;                      // p1ProductRule.inc


    // rules for the thermistor signal processing
    if ( cThrmstr::RunRules(DelTm3, PARM(WTempIoThrm)) == FAIL )  Status = FAIL;

    // run the internal simulator rules: Have to be put after the product rules are finished
    if ( SimRules() == FAIL )  Status = FAIL;


    return (Status);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This function is called by the framework timebase calculation.  It
//      allows the product level code to specifiy inputs to the timebase
//      calculation.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      Status to indicate if calculations were successful (SUCCESS/FAIL).
//
///////////////////////////////////////////////////////////////////////////////
int ProductTimebaseRules(void)
{
    int     Status = SUCCESS;

    //-------------------------------------------------------------------------
    // set the parms that are the basis for the framework timebase
    if ( PARM(SoftwareIPN) < DFIG02_2400_5060_A)
    {
       SET_FV(T1FrameRateReq,   5142.0);       // requested T1 frame rate (hz
    }
    else
    {
       SET_FV(T1FrameRateReq,   5712.0);       // requested T1 frame rate (hz)  (=5712 for DFIG    =3110 for MVC)  (jmn)
    }

    SET_FV(T1TaskExeDlyTm,   0.0e-6);       // delay from T1 frame boundary to ISR code execution (sec)

    //-------------------------------------------------------------------------
    // set the Task 2 and 3 execution rates relative to the Task1 execution rate
    //  target is approximately 1msec for T2 and 10msec for T3
    SET_FV(T1FramesPerT2Req,      6);
    SET_FV(T2FramesPerT3Req,     10);

    //-------------------------------------------------------------------------
    // set the Task1A execution rate and pattern relative to Task1
    //  if T1A is not needed
    //      set T1AFramesPerT2Req = T1FramesPerT2Req
    //      set T1AExecMask = 0
    //  else
    //      0 < T1AFramesPerT2 < T1FramesPerT2)
    //      set T1AExecMask to define execution pattern of T1a relative to T1
    //          bit # corresponds to T1 in a T2 frame, set to 1 to have T1A execute
    //          bit #0 and bit #T1FramesPerT2Req must both be 1
    SET_FV(T1AFramesPerT2Req,     4);
    SET_FV(T1AExecMask,        0x6D);

    //-------------------------------------------------------------------------
    // set the relationship of the modulator frame to the Task 1 frame
    SET_FV(T1FramesPerModReq,     1);       // T1 per async modulator (1,2,4)
    SET_FV(T1AFramesPerModReq,    1);       // T1A per async modulator (1,2,4)

    //-------------------------------------------------------------------------
    // Targets for PWM Minimum Pulse Time & target Bridge Lockout Time
    // are expressed explicitly in rules below per requirements from
    // the Bridge HW component Engineer.
    // FrcFreq=25Mhz(Clk0=40ns), though code is independent of assumption.
    //
    // "MinPulseCnt" is for Modulators that enforce Minimum Pulse at Frame
    // boundaries.  It must be an integer number of FRC clock cycles, since
    // events are scheduled according to that time datum.
    // Ceil() would be more conservative, but floor() is used since it was
    // used before to compute quantization, and Minimum Pulse is desired to
    // be consistent with Minimum Quantization.
    if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
    {
       MinPulseCnt = floor(3.55e-6 * FrcFreq);

       // Expect V.MinPlsCnt = floor(88.75) = 88
       //
       // "MinQuantReq" is for Modulators that implement Quantized Modulation
       // across the entire Frame.  It must be an integer number of FRC clock
       // cycles, since events are scheduled according to that time datum.

       SET_FV(MinQuantReq, MinPulseCnt / FrcFreq );

       // Expect PARM(MinQuantReq) = 88/(25MHz) = 88*(40ns) = 3.52us
       //
       // "BrgLockoutTm" is enforced in the FPGA (may be compensated in the Modulator)
       // and must be an integer number of "MinQuantReq".
       // Ceil() is conservative, and is consistent with previous value.

       // Expect PARM(BrgLockoutTm) = ceil(3.977)*3.52us = 4*3.52us = 14.08us

       SET_FV(BrgLockoutTm, ceil(14.0e-6 / PARM(MinQuantReq))*PARM(MinQuantReq) );
    }
    else
    {
       //shoot for 4.4usec...Greg likes this compared to the MACC 4.2666us actual
       MinPulseCnt = floor(4.4e-6 * FrcFreq);

       // Expect PARM(MinQuantReq) = 88/(25MHz) = 110*(40ns) = 4.40us
       SET_FV(MinQuantReq, MinPulseCnt / FrcFreq );

       // Expect PARM(BrgLockoutTm) = ceil(4.39usec)*4.4us = 1*4.4us = 4.4us
       // Entry of 4.40usec exactly causes a 8.8usec answer, so go down a bit
       SET_FV(BrgLockoutTm, ceil(4.39e-6 / PARM(MinQuantReq))*PARM(MinQuantReq) );

    }



    BrgLockoutTime = PARM(BrgLockoutTm);

    //-------------------------------------------------------------------------
    // set the timebase scheduling resolution based on the bridge interface capability
    FrcSchedMask = L_BridgeRebf.WhatIsFrcSchedMask();

    //-------------------------------------------------------------------------
    // done
    return (Status);
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Product task 1 instrumentation functions
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ProductInstrumentationT1(void)
{

}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Product task 2 instrumentation functions
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ProductInstrumentationT2(void)
{

    float GmVxGrid0;
    float GmVyGrid0;

    GmIxGrid = GmIxFbkT2;
    GmIyGrid = GmIyFbkT2;

    GmPwrGridT2  = GmPwrT2;
    GmQPwrGridT2 = GmQPwrT2;

        // Primary voltage calculation from Secondary 690V, referred to 690 V
        GmVxGrid0 = (GmVxFbkZT2 - L_VxLkgSecT2 - L_VxLkgPriT2) - XfmrRatioLnStr * S_VxLkgPriT2;
        GmVyGrid0 = (GmVyFbkZT2 - L_VyLkgSecT2 - L_VyLkgPriT2) - XfmrRatioLnStr * S_VyLkgPriT2;

    // Apply a lead to comp for the 1/2 task1 lag on the voltage components
    GmVxGrid = GmVxGrid0 + (GmVxGrid0 - GmVxGrid0Z) * PARM(GmVxyGridLd);
    GmVyGrid = GmVyGrid0 + (GmVyGrid0 - GmVyGrid0Z) * PARM(GmVxyGridLd);

    GmVxGrid0Z = GmVxGrid0;
    GmVyGrid0Z = GmVyGrid0;

    GmSinCosFbT2.cos = GmCosAngFbT2;
    GmSinCosFbT2.sin = GmSinAngFbT2;

    cVectorf::RotateLN(GmVxGrid, GmVyGrid, & GmSinCosFbT2, &GmVanGrid, &GmVbnGrid);
    GmVcnGrid = -(GmVanGrid + GmVbnGrid);

    // For test: copy the secondary common mode volts to the primary

    GmVabGrid = GmVanGrid - GmVbnGrid;
    GmVbcGrid = GmVbnGrid - GmVcnGrid;
    GmVcaGrid = GmVcnGrid - GmVanGrid;

    GmVMagGrid = sqrtf(GmVxGrid*GmVxGrid + GmVyGrid*GmVyGrid);

    GmVMagGridPu = GmVMagGrid * L_VlnInRcp;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Product task 3 instrumentation functions
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ProductInstrumentationT3(void)
{
    float GmPwrFctTemp;

    // grid totals - replica of GmPwr, GmQPwr, GmSPwr calcs in p0line.cpp:
    GmPwrGrid  = GmPwrGrid  + PARM(GmPwrFilW*DelTm3  )* (GmPwrGridT3  - GmPwrGrid);
    GmQPwrGrid = GmQPwrGrid + PARM(GmQPwrFilW*DelTm3 )* (GmQPwrGridT3 - GmQPwrGrid);

    GmSPwrGrid = sqrtf( (GmPwrGrid*GmPwrGrid) + (GmQPwrGrid*GmQPwrGrid) );
    // prevent devide by zero
    if  (ABS(GmSPwrGrid) < 0.001)
        GmSPwrGrid = 0.001;

    GmPwrFctTemp = ABS(GmPwrGrid)/GmSPwrGrid;
    if ( ABS(GmQPwrGrid) <  PARM(GmPwrFSignTh)) //if QPwr is small, follow sign of command
    {
        if ( QregCmd < -0.1) //slightly negative (-.1VAR is very small)
        {
            GmPwrFctTemp = -GmPwrFctTemp;
        }
    }
    if ( GmQPwrGrid < -PARM(GmPwrFSignTh)) //if QPwr is higher mag negative, also change sign
    {
        GmPwrFctTemp = -GmPwrFctTemp;
    }
    GmPwrFactorGrid = GmPwrFctTemp;

    if (!InitDone)
   {
      InitDoneTmr = InitDoneTmr + DelTm3;
      if (InitDoneTmr > 1.0F)
      {
         InitDone1Sec = true;
         InitDoneTmr  = 1.0F;
      }
   }

    //-------------------------------------------------------------------------
    // Line AEAD thermistor
    InletAirTemp = LIo1_Thrmstr.Thrmstr1Temp;
    PdcTemp      = LIo1_Thrmstr.Thrmstr2Temp;
    CfcTemp      = LIo1_Thrmstr.Thrmstr7Temp;
    CoolantTemp  = LIo1_Thrmstr.Thrmstr8Temp;
    //LIo1_Thrmstr.ThrmstrTemp4 is spare

    //-------------------------------------------------------------------------
    if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
    {
        if ( PARM(GenAeadThrm1Enb))
        {
            GenAeadTemp1    = RIo1_Thrmstr.Thrmstr1Temp;
        }
        else
        {
            GenAeadTemp1    = 0.0F;
        }

        if ( PARM(GenAeadThrm2Enb))
        {
            GenAeadTemp2    = RIo1_Thrmstr.Thrmstr2Temp;
        }
        else
        {
            GenAeadTemp2    = 0.0F;
        }

        AclfuTemp       = RIo1_Thrmstr.Thrmstr7Temp;
        PdcCabEntryTemp = RIo1_Thrmstr.Thrmstr8Temp;
    }
    else
    {
        GenAeadTemp1    = 0.0F;
        GenAeadTemp2    = 0.0F;
        AclfuTemp       = 0.0F;
        PdcCabEntryTemp = 0.0F;
    }

    LhslaTempUfil  = L_BridgeRebf.AmbientTemp;
    RhslaTempUfil  = R_BridgeRebf.AmbientTemp;


    if (SimMode || PwrDnFbkDsb)
    {
        LhslaTempUfil    = PARM(LhslaTempSim);
        RhslaTempUfil    = PARM(RhslaTempSim);
        InletAirTemp     = PARM(InletAirTempSim);
        PdcTemp          = PARM(PdcTempSim);
        CfcTemp          = PARM(CfcTempSim);
        CoolantTemp      = PARM(CoolantTempSim);
        PdcTemp          = PARM(PdcTempSim);
        CfcTemp          = PARM(CfcTempSim);

        if ( PARM(GenAeadThrm1Enb))
        {
            GenAeadTemp1     = PARM(GenAeadTemp1Sim);
        }
        else
        {
            GenAeadTemp1    = 0.0F;
        }

        if ( PARM(GenAeadThrm2Enb))
        {
            GenAeadTemp2     = PARM(GenAeadTemp2Sim);
        }
        else
        {
            GenAeadTemp2    = 0.0F;
        }

        AclfuTemp        = PARM(AclfuTempSim);
        PdcCabEntryTemp  = PARM(PdcCabEntryTempSim);
    }

    // filter the HSLA temperature
    LhslaTemp += PARM(WTempIoThrm) * DelTm3 * ( LhslaTempUfil - LhslaTemp );
    RhslaTemp += PARM(WTempIoThrm) * DelTm3 * ( RhslaTempUfil - RhslaTemp );

    MccTemp        = LhslaTemp; // Map MccTemp as MACCAmbTemp to LhslaTemp

   // Humidity Data
   HumidTempUfil = (175.72 * R_BridgeRebf.HumidThermTemp/256.0F) - 46.85;
   HumidPctUfil  = (125 * R_BridgeRebf.HumidThermPct/256.0F)-6;

   HumidTemp += PARM(WHumidData) * DelTm3 * ( HumidTempUfil - HumidTemp);
   HumidPct  += PARM(WHumidData) * DelTm3 * ( HumidPctUfil  - HumidPct );

   if ( HumidPct > 100.0F)
   {
    HumidPct = 100.0F;
   }
   else if ( HumidPct < 0.0F)
   {
    HumidPct = 0.0F;
   }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Product background functions
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ProductBkgnd(void)
{
    // Power Down Buffer Save to non-volatile memory
    // (locate here to improve probability of faster response)
    //     Trig = Imminent loss of 24V supply detected or "Force Edge"
    //     Arm  = Set while line converter running & not forced
    //            (protects against early & repeated saves)
    //     Done = Set when buffer save was attempted (indication only)

    PwrDnBufSavEdge = PARM(PwrDnBufSavFrc) && !PwrDnBufSavFrcZ;
    PwrDnBufSavFrcZ = PARM(PwrDnBufSavFrc);

    PwrDnBufSavTrig = (PwrDnFbkDsb || !Ps24vOk) || PwrDnBufSavEdge;

    if ( PwrDnBufSavEdge || (float (SysTmSec) > PARM(InitBufSavArmTm)))
    {
        PwrDnBufSavArm  = true;
    }

    if ( PwrDnBufSavArm )
    {
        if ( PwrDnBufSavTrig )
        {
            if ( !PARM(PwrDnBufSavDsb) )    // Disable save   (allow logic function)
            {
                if (CaptureBuf.SaveToFlash()==FAIL)
                {
                    PUSH_DIAG(CaptureBufSavFail);
                }
                if (TripBuf.SaveToFlash()==FAIL)
                {
                    PUSH_DIAG(TripBufSavFail);
                }
            }
            PwrDnBufSavArm  = false;        // Dis-arm save   (require re-arm)
            PwrDnBufSavDone = true;         // Attempted save (indication only)
        }
    }



    // Check for grid consistency and required generator parameters
    if ( (PARM(S_Power) == 0.0F) || (PARM(S_PwrFactor) == 0.0F) )
    {
        PUSH_DIAG(NoGenFileTrp);
    }

    if ( (PARM(ConverterLoc) == FIELD_APPL) &&
         (((PARM(GenTypeCfg)==20) && (PARM(L_FrqInput) != 60.0F)) ||
          ((PARM(GenTypeCfg)==21) && (PARM(L_FrqInput) != 60.0F)) ||
          ((PARM(GenTypeCfg)==22) && (PARM(L_FrqInput) != 60.0F)) ||
          ((PARM(GenTypeCfg)==23) && (PARM(L_FrqInput) != 50.0F)) ||
          ((PARM(GenTypeCfg)==24) && (PARM(L_FrqInput) != 50.0F)) ||
          ((PARM(GenTypeCfg)==25) && (PARM(L_FrqInput) != 50.0F)) ||
          ((PARM(GenTypeCfg)==26) && (PARM(L_FrqInput) != 50.0F)) ||
          ((PARM(GenTypeCfg)==27) && (PARM(L_FrqInput) != 50.0F)) ||
          ((PARM(GenTypeCfg)==28) && (PARM(L_FrqInput) != 60.0F)) ||
          ((PARM(GenTypeCfg)==29) && (PARM(L_FrqInput) != 50.0F))) )
    {
        if ( FrqGenMisMtch < 3 )
        {
            ++FrqGenMisMtch;
        }       // Wait for three consecutive background passes after
        else    // mismatch is seen for 1st time; worst case when 3 parms are
        {       // changing sequentially, one every background pass.
            PUSH_DIAG(FrqSelMisMtchTrp);
        }
    }
    else
    {
        FrqGenMisMtch = 0;
    }

    // Display doubly-fed generator type
    if ( PARM(ConverterLoc) == FIELD_APPL )
    {
        GenType = PARM(GenTypeCfg);   // Mirror for displaying in user diagram
    }
    else
    {
        GenType = PARM(GenTypeCfg);   // Mirror for displaying in user diagram
    }
    if(PARM(GenTypeCfg) == GENTYPE_OTHER)
    {
        GenNew = PARM(GenTypeNewCfg);
    }
    else
    {
        GenNew = NOT_APPLICABLE;
    }

    if ( InvIPNPwrFlg)
    {
       PUSH_DIAG(InvIPNPwr);
    }

    if ( NoCabFileFlg)
    {
       PUSH_DIAG(NoCabFile);
    }

    if ( CbFrqMsMcFlg)
    {
       PUSH_DIAG(CbFrqMisMtch);
    }

    if ( (PARM(Net_Type) != NET_ABSENT) && CCUParmRdy && (PLCPwrNP < 1500 || PLCPwrNP > (PARM(IPN_Frame) + PWR_MARGIN)) )
    {
       PUSH_DIAG(InvPLCPwrNP);
    }

    if ( TbGnPMsMcFlg)
    {
       PUSH_DIAG(TbGnPMisMtch);
    }

    if ( CbRqPrmBdFlg)
    {
       PUSH_DIAG(CabReqPrmBad);
    }

    // PwrLimSpdRatTc valid
    if(PARM(Net_Type)==NET_ABSENT) // Local mode
    {

        if ( ( PARM(PwrLimSpdRatLcl) > PARM(PwrLimSpdRatMax) ) || ( PARM(PwrLimSpdRatLcl) < PARM(PwrLimSpdRatMin)) )
        {
           PUSH_DIAG(TcSpdRatInvalid);
        }
    }
    else if (IpcInterfaceOk && CCUParmRdy) // Remote mode with C to C
    {
        if ( (PwrLimSpdRatRem > PARM(PwrLimSpdRatMax)) || (PwrLimSpdRatRem < PARM(PwrLimSpdRatMin)) )
        {
           PUSH_DIAG(TcSpdRatInvalid);
        }
    }

    if(SimRunRuleReq)
    {
        RunRuleSet(RULE_SET_MAIN);
        SimRunRuleReq=false;
    }


    return;
}
