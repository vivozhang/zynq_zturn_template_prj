///////////////////////////////////////////////////////////////////////////////
//
// TITLE     Bridge thermistor supporting functions
//
// DESCRIPTION
//          Functions contained in this file support processing of Negative
//          Temperature Coefficient (NTC) thermistor signals, implementation
//          of sensor/thermal protections, and verification of thermal
//          permissives.
//
// COPYRIGHT    Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// core
// framework
#include  "x0FrameWork.h"
// product
#include  "Product.h"
#include  "b0BridgeRebf.h"
#include  "y0Brthm.h"
#include  "p0Thrmstr.h"
#include  "b0BrgThrmstr.h"


// Constants
//----------


// Externals
//----------
extern float        DelTm3;
extern unsigned     Init1Sec;
extern unsigned     L_Running;
extern unsigned     PwrDnFbkDsb;
extern unsigned     SimMode;
extern unsigned     Stopped;
extern unsigned     HeaterEnb;

extern unsigned     L_ThrmTempCntsT3[];
extern float        L_TempVcoDelTmRcp;
extern float        L_DelTm1Rcp;
extern unsigned     L_ThrmHiSelT3;

extern unsigned     R_ThrmTempCntsT3[];
extern float        R_TempVcoDelTmRcp;
extern unsigned     R_ThrmHiSelT3;

extern float        PdcTemp;
extern float        CfcTemp;
extern float        CoolantTemp;
extern float        GenAeadTemp1;
extern float        GenAeadTemp2;
extern float        AclfuTemp;
extern float        LhslaTemp;
extern float        RhslaTemp;
extern float        InletAirTemp;

extern cBridgeRebf  L_BridgeRebf;
extern cBridgeRebf  R_BridgeRebf;

extern cThrmstr     LIo1_Thrmstr;
extern cThrmstr     RIo1_Thrmstr;

extern cBrgThrmstr  R_BrgThrmstr;
extern cBrgThrmstr  R2_BrgThrmstr;
extern cBrgThrmstr  L_BrgThrmstr;

extern unsigned     R_OTAlSum;
extern unsigned     L_OTAlSum;
extern float        DelTm3Rcp;
extern unsigned     CnvShutDownTm;
// for SealInSuicide()
extern float    MccTemp;

extern float    PdcCabEntryTemp;
extern unsigned         TmpTrpDlySum;

extern unsigned L_ThrmTempCnts1;
extern unsigned R_ThrmTempCnts1;

// Types and Classes
//------------------
struct  ThermisDlyThrmTyp
{
    // inputs
    const float&       ThrmTemp;           // 1 - thermistor temperature (deg C)
    const float&       ThrmOtTrpThrsh;     // 2 - thermistor over temperature trip threshold(deg C)
    const float&       ThrmOtAlmOnThrsh;   // 3 - thermistor over temperature alarm on threshold(deg C)
    const float&       ThrmOtAlmOffThrsh;  // 4 - thermistor over temperature alarm off threshold(deg C)
    const float&       ThrmUtTrpThrsh;     // 5 - thermistor under temperature trip threshold(deg C)
    const float&       ThrmUtAlmOnThrsh;   // 6 - thermistor under temperature alarm on threshold(deg C)
    const float&       ThrmUtAlmOffThrsh;  // 7 - thermistor under temperature alarm off threshold(deg C)
    // outputs
       unsigned&    ThrmOtTrpFlg;          // 8 - delayed over temperature trip flag
       unsigned&    ThrmOtAlmFlg;          // 9 - delayed over temperature alarm flag
       unsigned&    ThrmUtTrpFlg;          //10 - delayed under temperature trip flag
       unsigned&    ThrmUtAlmFlg;          //11 - delayed under temperature alarm flag
};


// Parameters
//-----------
CREATE_PARM(CoolantOtThrsh     , float);      // liquid coolant over temperature threshold (deg C)

// thermistor delayed thermal trips thresholds, actually they are for delayed OT
CREATE_PARM(PdcOtThrsh         , float);      // converter AC entry over temperature threshold (deg C)
CREATE_PARM(CfcOtThrsh         , float);      // converter distortion filter liquid coolant over temperature threshold (deg C)
CREATE_PARM(InletAirOtThrsh    , float);      // air inlet over temperature threshold (deg C)
CREATE_PARM(PdcCabEnOtThrsh    , float);      // liquid coolant heat exchanger temperature (liquid hot side) over temperature threshold (deg C)
CREATE_PARM(CoolantOtDlyThrsh  , float);      // liquid coolant delayed over temperature threshold (deg C)
CREATE_PARM(AclfuOtThrsh       , float);      // AC line fuse over temperature threshold (deg C)
CREATE_PARM(HslaOtThrsh        , float);      // Hsla over temperature threshold (deg C)

CREATE_PARM(HslaUtThrsh        , float);      // Hsla under temperature threshold (deg C)
CREATE_PARM(PdcUtThrsh         , float);      // converter AC entry under temperature threshold (deg C)
CREATE_PARM(CoolantUtThrsh     , float);      // Coolant under temperature delay trip threshold (deg C)
// thermistor alarms on/off thresholds
CREATE_PARM(HslaOtOnThrsh      , float);      // HSLA over temperature alarm on threshold (deg C)
CREATE_PARM(HslaOtOffThrsh     , float);      // HSLA over temperature alarm off threshold (deg C)
CREATE_PARM(HslaUtOnThrsh      , float);      // HSLA under temperature alarm on threshold (deg C)
CREATE_PARM(HslaUtOffThrsh     , float);      // HSLA under temperature alarm off threshold (deg C)

CREATE_PARM(InletAirOtOnThrsh  , float);      // air inlet over temperature alarm on threshold (deg C)
CREATE_PARM(InletAirOtOffThrsh , float);      // air inlet over temperature alarm off threshold (deg C)
CREATE_PARM(InletAirUtOnThrsh  , float);      // air inlet under temperature alarm on threshold (deg C)
CREATE_PARM(InletAirUtOffThrsh , float);      // air inlet under temperature alarm off threshold (deg C)

CREATE_PARM(CoolantOtOnThrsh   , float);      // liquid coolant over temperature alarm on threshold (deg C)
CREATE_PARM(CoolantOtOffThrsh  , float);      // liquid coolant over temperature alarm off threshold (deg C)
CREATE_PARM(CoolantUtOnThrsh   , float);      // liquid coolant under temperature alarm on threshold (deg C)
CREATE_PARM(CoolantUtOffThrsh  , float);      // liquid coolant under temperature alarm off threshold (deg C)
CREATE_PARM(CoolntUtTm,         float);     // Coolant under temperature time (sec);
CREATE_PARM(PdcCabEnOtOnThrsh     , float);   // PDC Cable entry over temperature alarm on threshold (deg C)
CREATE_PARM(PdcCabEnOtOffThrsh    , float);   // PDC Cable entryover temperature alarm off threshold (deg C)
CREATE_PARM(PdcCabEnUtOnThrsh     , float);   // PDC Cable entry under temperature alarm on threshold (deg C)
CREATE_PARM(PdcCabEnUtOffThrsh    , float);   // PDC Cable entry under temperature alarm off threshold (deg C)

CREATE_PARM(CfcOtOnThrsh       , float);      // converter distortion filter over temperature alarm on threshold (deg C)
CREATE_PARM(CfcOtOffThrsh      , float);      // converter distortion filter over temperature alarm off threshold (deg C)
CREATE_PARM(CfcUtOnThrsh       , float);      // converter distortion filter under temperature alarm on threshold (deg C)
CREATE_PARM(CfcUtOffThrsh      , float);      // converter distortion filter under temperature alarm off threshold (deg C)

CREATE_PARM(PdcOtOnThrsh       , float);      // converter AC entry over temperature alarm on threshold (deg C)
CREATE_PARM(PdcOtOffThrsh      , float);      // converter AC entry over temperature alarm off threshold (deg C)
CREATE_PARM(PdcUtOnThrsh       , float);      // converter AC entry under temperature alarm on threshold (deg C)
CREATE_PARM(PdcUtOffThrsh      , float);      // converter AC entry under temperature alarm off threshold (deg C)

CREATE_PARM(AclfuOtOnThrsh      , float);      // AC line fuse over temperature alarm on threshold (deg C)
CREATE_PARM(AclfuOtOffThrsh     , float);      // AC line fuse over temperature alarm off threshold (deg C)

// thermistor-based run permissive
CREATE_PARM(MaContTempLowLvl,   float);       // MA contactor temperature ok level (to start the converter)(deg C)
CREATE_PARM(CcstarTempLowLvl,   float);       // capacitor charger starter temperature ok level (to start the converter)(deg C)
CREATE_PARM(InletAirTempLimMax, float);       // max air inlet temperature(to start the converter)(deg C)
CREATE_PARM(InletAirTempLimMin, float);       // min air inlet temperature (to start the converter) (deg C)
CREATE_PARM(TempRiseOkTmMax ,   int);         // temperature rise ok max level(cnt): the longest heat soak time
CREATE_PARM(TempRiseOkTmNom ,   int);         // temperature rise ok nom level(cnt): normal heat soak time
CREATE_PARM(TempFbkRdyTm,       float);       // waiting time for the temperature feedbacks to be ready (sec)
CREATE_PARM(TempFilterSettleTm, float);       // waiting time for the temperature filter settle down (sec)
CREATE_PARM(HtrFanSupresTm,     float);       // heater and fan suppresion time (sec)
CREATE_PARM(TempRiseOkLvl,      float);       // temperature rise ok level (deg C)
CREATE_PARM(TempRiseNotOkLvl,   float);       // temperature rise not ok level (deg C)
CREATE_PARM(HtrFanEnb,          unsigned);    // parameter used to enable/disable the heater and fan control
CREATE_PARM(DcLinkFrostThrsh,    float);      // DC link has frost threshold (deg C)
CREATE_PARM(DcLinkNotFrostThrsh, float);      // DC link not have frost threshold (deg C)
// IOThermalConversion()
CREATE_PARM(TempVcoScl,         float);     // Temperature Vco scale
CREATE_PARM(TempVcoZero,        float);     // Temperature Vco scale
CREATE_PARM(ThrmRangeSel,       unsigned);  // IO thermistor sensing range select
// TemperatureShutdownT3()
CREATE_PARM(TempDlyTm,          float);     // Delayed shutdown/trip time

// Variables
//----------

// Thermistors
CREATE_PUBVAR(L_TempTh1RawHi  , int);     // Temp Th1 VCO counts
CREATE_PUBVAR(L_TempTh1RawLw  , int);     // Temp Th1 VCO counts
CREATE_PUBVAR(L_TempTh2RawHi  , int);     // Temp Th2 VCO counts
CREATE_PUBVAR(L_TempTh2RawLw  , int);     // Temp Th2 VCO counts
CREATE_PUBVAR(L_TempTh7RawHi  , int);     // Temp Th7 VCO counts
CREATE_PUBVAR(L_TempTh7RawLw  , int);     // Temp Th7 VCO counts
CREATE_PUBVAR(L_TempTh8RawHi  , int);     // Temp Th8 VCO counts
CREATE_PUBVAR(L_TempTh8RawLw  , int);     // Temp Th8 VCO counts

CREATE_PUBVAR(L_TempTh1FrqHi  , float) ;  // Temp Th1 VCO frequency--high range
CREATE_PUBVAR(L_TempTh1FrqLw  , float) ;  // Temp Th1 VCO frequency--low range
CREATE_PUBVAR(L_TempTh2FrqHi  , float) ;  // Temp Th2 VCO frequency--high range
CREATE_PUBVAR(L_TempTh2FrqLw  , float) ;  // Temp Th2 VCO frequency--low range
CREATE_PUBVAR(L_TempTh7FrqHi  , float) ;  // Temp Th7 VCO frequency--high range
CREATE_PUBVAR(L_TempTh7FrqLw  , float) ;  // Temp Th7 VCO frequency--low range
CREATE_PUBVAR(L_TempTh8FrqHi  , float) ;  // Temp Th8 VCO frequency--high range
CREATE_PUBVAR(L_TempTh8FrqLw  , float) ;  // Temp Th8 VCO frequency--low range

CREATE_PUBVAR(L_TempVltTh1Hi  , float) ;  // Temp volts for Th1
CREATE_PUBVAR(L_TempVltTh1Lw  , float) ;  // Temp volts for Th1
CREATE_PUBVAR(L_TempVltTh2Hi  , float) ;  // Temp volts for Th2
CREATE_PUBVAR(L_TempVltTh2Lw  , float) ;  // Temp volts for Th2
CREATE_PUBVAR(L_TempVltTh7Hi  , float) ;  // Temp volts for Th7
CREATE_PUBVAR(L_TempVltTh7Lw  , float) ;  // Temp volts for Th7
CREATE_PUBVAR(L_TempVltTh8Hi  , float) ;  // Temp volts for Th8
CREATE_PUBVAR(L_TempVltTh8Lw  , float) ;  // Temp volts for Th8

CREATE_PUBVAR(R_TempTh1RawHi  , int);     // Temp Th1 VCO counts
CREATE_PUBVAR(R_TempTh1RawLw  , int);     // Temp Th1 VCO counts
CREATE_PUBVAR(R_TempTh2RawHi  , int);     // Temp Th2 VCO counts
CREATE_PUBVAR(R_TempTh2RawLw  , int);     // Temp Th2 VCO counts
CREATE_PUBVAR(R_TempTh7RawHi  , int);     // Temp Th7 VCO counts
CREATE_PUBVAR(R_TempTh7RawLw  , int);     // Temp Th7 VCO counts
CREATE_PUBVAR(R_TempTh8RawHi  , int);     // Temp Th8 VCO counts
CREATE_PUBVAR(R_TempTh8RawLw  , int);     // Temp Th8 VCO counts

CREATE_PUBVAR(R_TempTh1FrqHi  , float) ;  // Temp Th1 VCO frequency--high range
CREATE_PUBVAR(R_TempTh1FrqLw  , float) ;  // Temp Th1 VCO frequency--low range
CREATE_PUBVAR(R_TempTh2FrqHi  , float) ;  // Temp Th2 VCO frequency--high range
CREATE_PUBVAR(R_TempTh2FrqLw  , float) ;  // Temp Th2 VCO frequency--low range
CREATE_PUBVAR(R_TempTh7FrqHi  , float) ;  // Temp Th7 VCO frequency--high range
CREATE_PUBVAR(R_TempTh7FrqLw  , float) ;  // Temp Th7 VCO frequency--low range
CREATE_PUBVAR(R_TempTh8FrqHi  , float) ;  // Temp Th8 VCO frequency--high range
CREATE_PUBVAR(R_TempTh8FrqLw  , float) ;  // Temp Th8 VCO frequency--low range

CREATE_PUBVAR(R_TempVltTh1Hi  , float) ;  // Temp volts for Th1
CREATE_PUBVAR(R_TempVltTh1Lw  , float) ;  // Temp volts for Th1
CREATE_PUBVAR(R_TempVltTh2Hi  , float) ;  // Temp volts for Th2
CREATE_PUBVAR(R_TempVltTh2Lw  , float) ;  // Temp volts for Th2
CREATE_PUBVAR(R_TempVltTh7Hi  , float) ;  // Temp volts for Th7
CREATE_PUBVAR(R_TempVltTh7Lw  , float) ;  // Temp volts for Th7
CREATE_PUBVAR(R_TempVltTh8Hi  , float) ;  // Temp volts for Th8
CREATE_PUBVAR(R_TempVltTh8Lw  , float) ;  // Temp volts for Th8

CREATE_PUBVAR(R2_TempTh1RawHi  , int);     // Temp Th1 VCO counts
CREATE_PUBVAR(R2_TempTh1RawLw  , int);     // Temp Th1 VCO counts
CREATE_PUBVAR(R2_TempTh2RawHi  , int);     // Temp Th2 VCO counts
CREATE_PUBVAR(R2_TempTh2RawLw  , int);     // Temp Th2 VCO counts
CREATE_PUBVAR(R2_TempTh7RawHi  , int);     // Temp Th7 VCO counts
CREATE_PUBVAR(R2_TempTh7RawLw  , int);     // Temp Th7 VCO counts
CREATE_PUBVAR(R2_TempTh8RawHi  , int);     // Temp Th8 VCO counts
CREATE_PUBVAR(R2_TempTh8RawLw  , int);     // Temp Th8 VCO counts

CREATE_PUBVAR(R2_TempTh1FrqHi  , float) ;  // Temp Th1 VCO frequency--high range
CREATE_PUBVAR(R2_TempTh1FrqLw  , float) ;  // Temp Th1 VCO frequency--low range
CREATE_PUBVAR(R2_TempTh2FrqHi  , float) ;  // Temp Th2 VCO frequency--high range
CREATE_PUBVAR(R2_TempTh2FrqLw  , float) ;  // Temp Th2 VCO frequency--low range
CREATE_PUBVAR(R2_TempTh7FrqHi  , float) ;  // Temp Th7 VCO frequency--high range
CREATE_PUBVAR(R2_TempTh7FrqLw  , float) ;  // Temp Th7 VCO frequency--low range
CREATE_PUBVAR(R2_TempTh8FrqHi  , float) ;  // Temp Th8 VCO frequency--high range
CREATE_PUBVAR(R2_TempTh8FrqLw  , float) ;  // Temp Th8 VCO frequency--low range

CREATE_PUBVAR(R2_TempVltTh1Hi  , float) ;  // Temp volts for Th1
CREATE_PUBVAR(R2_TempVltTh1Lw  , float) ;  // Temp volts for Th1
CREATE_PUBVAR(R2_TempVltTh2Hi  , float) ;  // Temp volts for Th2
CREATE_PUBVAR(R2_TempVltTh2Lw  , float) ;  // Temp volts for Th2
CREATE_PUBVAR(R2_TempVltTh7Hi  , float) ;  // Temp volts for Th7
CREATE_PUBVAR(R2_TempVltTh7Lw  , float) ;  // Temp volts for Th7
CREATE_PUBVAR(R2_TempVltTh8Hi  , float) ;  // Temp volts for Th8
CREATE_PUBVAR(R2_TempVltTh8Lw  , float) ;  // Temp volts for Th8

CREATE_PUBVAR(TempRngSel     , unsigned);   // Temperature range selection

CREATE_PUBVAR(HtrFanRunOk,       unsigned);   // heater and fan is ok to run
CREATE_PUBVAR(HtrSoakFastTmrSel, unsigned);   // heater soak timer uses the fast time step
CREATE_PUBVAR(InletAirTempClamp, float);      // the clamped air inlet temperature to calculate the desired temperature rise (deg C)
CREATE_PUBVAR(HtrSoakTimerStep,  int);        // the selected timer step used by heater soak timer
CREATE_PUBVAR(HtrFanSupresTmr,   float);      // the heater and fan suppresion timer (sec)
CREATE_PUBVAR(MinBrgLqdTemp,     float);      // minimal bridge baseplate and liquid temperature (deg C)
CREATE_PUBVAR(BrgLqdTempRise,    float);      // bridge baseplate and liquid temperature rise (deg C)

CREATE_PUBVAR(ThrmOpnFlg    ,  unsigned);           // global thermal open flag
CREATE_PUBVAR(BrgOvrTempFlg ,  unsigned);           // global brg IGBT over temperature flag

CREATE_PUBVAR(ThermisAlmSm  , unsigned);            // Thermister brief alarm summary
CREATE_PUBVAR(ThermisDlySm  , unsigned);            // Thermister pre-trip alarm summary
CREATE_PUBVAR(TempDlyTmr   , float);                // Temperature delay trip timer
CREATE_PUBVAR(TempTrpLatch , unsigned);             // Temperature delayed trip latch
CREATE_PUBVAR(TherBadDlySm , unsigned) =false;      // Bad thermistor delayed trip summary
CREATE_PUBVAR(TTripPend    , unsigned) ;            // Temperature delayed trip indication for IONET
CREATE_PUBVAR(TempIOFbkVld , unsigned) = false;

// thermistor delta resistance alarm flag
CREATE_PUBVAR(AirInThrmDlFlg,   unsigned) = false;  // Flag for L AEAD Din 1 delta resistance alarm
CREATE_PUBVAR(PdcThrmDlFlg,     unsigned) = false;  // Flag for L AEAD Din 2 delta resistance alarm
CREATE_PUBVAR(CfcThrmDlFlg,     unsigned) = false;  // Flag for L AEAD Din 7 delta resistance alarm
CREATE_PUBVAR(LqdThrmDlFlg,     unsigned) = false;  // Flag for L AEAD Din 8 delta resistance alarm

CREATE_PUBVAR(GenAeadThrm1DlFlg,unsigned) = false;  // Flag for R AEAD Din 1 delta resistance alarm
CREATE_PUBVAR(GenAeadThrm2DlFlg,unsigned) = false;  // Flag for R AEAD Din 2 delta resistance alarm
CREATE_PUBVAR(PdcCabEnThrmDlFlg,unsigned) = false;  // Flag for R AEAD Din 8 delta resistance alarm
CREATE_PUBVAR(AclfuThrmDlFlg,   unsigned) = false;  // Flag for R AEAD Din 7 delta resistance alarm

// open thermistor flag
CREATE_PUBVAR(AirInThrmOpnFlg,  unsigned) = false;  // Flag for L AEAD Din 1 Thermistor open (InletAirTemp)
CREATE_PUBVAR(PdcThrmOpnFlg,    unsigned) = false;  // Flag for L AEAD Din 2 Thermistor open
CREATE_PUBVAR(CfcThrmOpnFlg,    unsigned) = false;  // Flag for L AEAD Din 7 Thermistor open
CREATE_PUBVAR(LqdThrmOpnFlg,    unsigned) = false;  // Flag for L AEAD Din 8 Thermistor open (CoolantTemp)

CREATE_PUBVAR(GenAeadThrm1OpnFlg,unsigned) = false;  // Flag for R AEAD Din 1 Thermistor open
CREATE_PUBVAR(GenAeadThrm2OpnFlg,unsigned) = false;  // Flag for R AEAD Din 2 Thermistor open
CREATE_PUBVAR(PdcCabEnThrmOpnFlg,unsigned)= false;  // Flag for R AEAD Din 8 Thermistor open (PdcCabEntryTemp)
CREATE_PUBVAR(AclfuThrmOpnFlg,   unsigned) = false;  // Flag for R AEAD Din 7 Thermistor open(AclfuTemp)

// short thermistor flag
CREATE_PUBVAR(AirInThrmShrtFlg, unsigned) = false;  // Flag for L AEAD Din 1 Thermistor short (InletAirTemp)
CREATE_PUBVAR(PdcThrmShrtFlg,   unsigned) = false;  // Flag for L AEAD Din 2 Thermistor short (PDCTemp)
CREATE_PUBVAR(CfcThrmShrtFlg,   unsigned) = false;  // Flag for L AEAD Din 7 Thermistor short (CFCTemp)
CREATE_PUBVAR(LqdThrmShrtFlg,   unsigned) = false;  // Flag for L AEAD Din 8 Thermistor short (CoolantTemp)

CREATE_PUBVAR(GenAeadThrm1ShrtFlg, unsigned) = false;  // Flag for R AEAD Din 1 Thermistor short
CREATE_PUBVAR(GenAeadThrm2ShrtFlg, unsigned) = false;  // Flag for R AEAD Din 2 Thermistor short
CREATE_PUBVAR(PdcCabEnThrmShrtFlg, unsigned)= false; // Flag for R AEAD Din 8 Thermistor short (PdcCabEntryTemp)
CREATE_PUBVAR(AclfuThrmShrtFlg, unsigned) = false;  // Flag for R AEAD Din 7 Thermistor short (AclfuTemp)

// thermistor delayed thermal diagnostics flag
CREATE_PUBVAR(InletAirOtDlyTrpFlg   , unsigned) = false;  // air inlet delayed over temperature trip flag
CREATE_PUBVAR(CoolantOtDlyTrpFlg    , unsigned) = false;  // Coolant delayed over temperature trip flag
CREATE_PUBVAR(CoolantUtDlyTrpFlg    , unsigned) = false;  // Coolant delayed under temperature trip flag
CREATE_PUBVAR(CoolantUtDlyTrpFlg2   , unsigned) = false;  // Set when CoolantUtDlyTrpFlg is true for certain time
CREATE_PUBVAR(PdcCabEnOtDlyTrpFlg   , unsigned) = false;  // liquid exhanged-in delayed over temperature trip flag
CREATE_PUBVAR(CfcOtDlyTrpFlg        , unsigned) = false;  // converter distortion filter delayed over temperature trip flag
CREATE_PUBVAR(PdcOtDlyTrpFlg        , unsigned) = false;  // converter distortion filter delayed over temperature trip flag
CREATE_PUBVAR(AclfuOtDlyTrpFlg      , unsigned) = false;  // AC line fuse delayed over temperature trip flag

CREATE_PUBVAR(InletAirOtDlyAlmFlg   , unsigned) = false;  // air inlet delayed over temperature alarm flag
CREATE_PUBVAR(InletAirUtDlyAlmFlg   , unsigned) = false;  // air inlet delayed under temperature alarm flag
CREATE_PUBVAR(CoolantOtDlyAlmFlg    , unsigned) = false;  // liquid delayed over temperature alarm flag
CREATE_PUBVAR(CoolantUtDlyAlmFlg    , unsigned) = false;  // liquid delayed under temperature alarm flag
CREATE_PUBVAR(PdcCabEnOtDlyAlmFlg   , unsigned) = false;  // liquid exhanged-in delayed over temperature alarm flag
CREATE_PUBVAR(PdcCabEnUtDlyAlmFlg   , unsigned) = false;  // liquid exhanged-in delayed under temperature alarm flag
CREATE_PUBVAR(CfcOtDlyAlmFlg        , unsigned) = false;  // converter distortion filter delayed over temperature alarm flag
CREATE_PUBVAR(PdcOtDlyAlmFlg        , unsigned) = false;  // converter AC entry delayed over temperature alarm flag
CREATE_PUBVAR(AclfuOtDlyAlmFlg      , unsigned) = false;  // AC line fuse delayed over temperature alarm flag

CREATE_PUBVAR(LhslaOtDlyAlmFlg      , unsigned) = false;  // line HSLA over temperature delayed alarm falg
CREATE_PUBVAR(LhslaUtDlyAlmFlg      , unsigned) = false;  // line HSLA under temperature delayed alarm falg
CREATE_PUBVAR(RhslaOtDlyAlmFlg      , unsigned) = false;  // rotor HSLA over temperature delayed alarm falg
CREATE_PUBVAR(RhslaUtDlyAlmFlg      , unsigned) = false;  // rotor HSLA under temperature delayed alarm falg

CREATE_PUBVAR(LhslaOtDlyTrpFlg      , unsigned) = false;  // line HSLA over temperature delayed trip falg
CREATE_PUBVAR(RhslaOtDlyTrpFlg      , unsigned) = false;  // rotor HSLA over temperature delayed trip falg
CREATE_PUBVAR(LhslaUtDlyTrpFlg      , unsigned) = false;  // rotor HSLA under temperature delayed trip falg
CREATE_PUBVAR(RhslaUtDlyTrpFlg      , unsigned) = false;  // rotor HSLA under temperature delayed trip falg

CREATE_PUBVAR(PdcUtDlyAlmFlg,       unsigned) = false;    // PDC under temperature delayed alarm flag
CREATE_PUBVAR(CfcUtDlyAlmFlg,       unsigned) = false;    // CFC under temperature delayed alarm flag

CREATE_PUBVAR(CoolantUtDlyTrpTmr,   float);
// set EnvOkSealCmd latch state to 1 at controller boot/power up.
CREATE_PUBVAR(EnvOkSealCmd,     unsigned) =1;           // seal-in relay command for environmental Ok indication

// thermistor-based temperature ok indicator
CREATE_PUBVAR(CcstarNotCold      , unsigned);           // CCSTR is warmed up (to start the converter)
CREATE_PUBVAR(MaContNotCold      , unsigned);           // MA contactor is warmed up (to start the converter)
CREATE_PUBVAR(BridgeNotCold      , unsigned) = false;   // all bridges are not cold: all IGBT and liquid temperature rises are OK
CREATE_PUBVAR(TempRiseOk2Run     , unsigned) = false;   // all IGBT and liquid temperature rises are OK (to start the converter)
CREATE_PUBVAR(TempRiseOkTmr      , int);                // temperature rise OK (to start the converter) timer (cnt)
CREATE_PUBVAR(TempRiseOkTmrDsp   , float);              // temperature rise OK (to start the converter) timer (sec)
CREATE_PUBVAR(TempFilterSettleTmr, float);              // timer for the temperature filter settle down (sec)
CREATE_PUBVAR(TempFbkRdy         , unsigned) = false;   // temperature data from AEAD's via HSSL are ready
CREATE_PUBVAR(TempIsValid        , unsigned) = false;   // thermistor temperature is valid: filter is settled down
CREATE_PUBVAR(TempIsValidZ       , unsigned) = false;   // delayed thermistor temperature is valid

CREATE_PUBVAR(MinRotor1BpTemp    , float);              // minimum rotor bridge1 base plate temperature (deg C)
CREATE_PUBVAR(MinRotor2BpTemp    , float);              // minimum rotor bridge2 base plate temperature (deg C)
CREATE_PUBVAR(MinLineBpTemp      , float);              // minimum line bridge base plate temperature (deg C)
CREATE_PUBVAR(DcLinkTempOk       , unsigned);           // DC link does not have frost
CREATE_PUBVAR(DcLinkHtrSoakTm    , int);                // the heat soak time in integer (cnt)
CREATE_PUBVAR(DcLinkHtrSoakTmDsp , float);              // the heat soak time (min), display only
CREATE_PUBVAR(MaxTimeTempRiseOk  , float);              // the longest heat soak time for temperature rise ok (sec)
CREATE_PUBVAR(NomTimeTempRiseOk  , float);              // normal heat soak time for temperature rise ok (Sec)

CREATE_PUBVAR(CnvShutDownFlg,      unsigned);

// Diagnostics
//-------------

// thermistor delayed trips
CREATE_DIAG(CoolantOtTrp    );		 // liquid coolant over temperature trip
CREATE_DIAG(InletAirOtTrpDly);       // air inlet over temperature delayed trip
CREATE_DIAG(CoolantOtDly    );       // liquid coolant over temperature delayed trip
CREATE_DIAG(PdcCabEnOtTrpDly);         // liquid coolant heater exchanger over temperature delayed trip
CREATE_DIAG(CfcOtTrpDly     );       // converter distortion filter over temperature delayed trip
CREATE_DIAG(PdcOtTrpDly     );       // converter AC entry over temperature delayed trip
CREATE_DIAG(AclfuOtTrpDly   );       // AC line fuse over temperature delayed trip
CREATE_DIAG(PdcUtTrpDly     );       // liquid coolant under temperature delayed trip

CREATE_DIAG(LhslaOtTrpDly   );       // line side HSLA over temperature delayed trip
CREATE_DIAG(RhslaOtTrpDly   );       // rotor side HSLA over temperature delayed trip
CREATE_DIAG(LhslaUtTrpDly   );       // line side HSLA under temperature delayed trip
CREATE_DIAG(RhslaUtTrpDly   );       // rotor side HSLA under temperature delayed trip
CREATE_DIAG(CoolantUtDly    );       // Coolant under temperature delayed trip
// thermistor alarms
CREATE_DIAG(InletAirOtAlmDly);       // air inlet over temperature alarm
CREATE_DIAG(InletAirUtAlmDly);       // air inlet under temperature alarm
CREATE_DIAG(CfcOtAlmDly     );       // converter distortion filter over temperature alarm
CREATE_DIAG(CfcUtAlmDly     );       // converter distortion filter under temperature alarm
CREATE_DIAG(PdcUtAlmDly     );       // Power distribution cabinet under temperature alarm
CREATE_DIAG(PdcOtAlmDly     );       // Power distribution cabinet over temperature alarm
CREATE_DIAG(AclfuOtAlmDly   );       // AC line fuse over temperature alarm
CREATE_DIAG(CoolantOtAlm    );       // liquid coolant over temperature alarm
CREATE_DIAG(CoolantUtAlm    );       // liquid coolant under temperature alarm
CREATE_DIAG(PdcCabEnOtAlmDly );      // liquid coolant heat exchanger over temperature alarm
CREATE_DIAG(PdcCabEnUtAlmDly );      // liquid coolant heat exchanger under temperature alarm
CREATE_DIAG(LhslaOtAlmDly   );       // line side HSLA over temperature alarm
CREATE_DIAG(RhslaOtAlmDly   );       // rotor side HSLA over temperature alarm
CREATE_DIAG(LhslaUtAlmDly   );       // line side HSLA under temperature alarm
CREATE_DIAG(RhslaUtAlmDly   );       // rotor side HSLA under temperature alarm
CREATE_DIAG(PdcOtDly);      // PDC over temperature trip
CREATE_DIAG(PdcOtAlm);      // PDC over temperature alarm
CREATE_DIAG(CfcOtDly);      // CFC over temperature trip
CREATE_DIAG(CfcOtAlm);      // CFC over temperature alarm
// Others
CREATE_DIAG(TempDlyFlt);             // General temperature delayed trip


// Data Passing
//--------------


// Unpublished Variables
//----------------------
unsigned FlagNa = false;

ThermisDlyThrmTyp L_HslaThermis =
{
    LhslaTemp,                // 1 - thermistor temperature (deg C)
    PARM(HslaOtThrsh),        // 2 - thermistor over temperature trip threshold(deg C)
    PARM(HslaOtOnThrsh),      // 3 - thermistor over temperature alarm on threshold(deg C)
    PARM(HslaOtOffThrsh),     // 4 - thermistor over temperature alarm off threshold(deg C)
    PARM(HslaUtThrsh),        // 5 - thermistor under temperature trip threshold(deg C)
    PARM(HslaUtOnThrsh),      // 6 - thermistor under temperature alarm on threshold(deg C)
    PARM(HslaUtOffThrsh),     // 7 - thermistor under temperature alarm off threshold(deg C)
    LhslaOtDlyTrpFlg,         // 8 - delayed over temperature trip flag
    LhslaOtDlyAlmFlg,         // 9 - delayed over temperature alarm flag
    LhslaUtDlyTrpFlg,         //10 - delayed under temperature trip flag
    LhslaUtDlyAlmFlg,         //11 - delayed under temperature alarm flag
};

ThermisDlyThrmTyp R_HslaThermis =
{
    RhslaTemp,                // 1 - thermistor temperature (deg C)
    PARM(HslaOtThrsh),        // 2 - thermistor over temperature trip threshold(deg C)
    PARM(HslaOtOnThrsh),      // 3 - thermistor over temperature alarm on threshold(deg C)
    PARM(HslaOtOffThrsh),     // 4 - thermistor over temperature alarm off threshold(deg C)
    PARM(HslaUtThrsh),        // 5 - thermistor under temperature trip threshold(deg C)
    PARM(HslaUtOnThrsh),      // 6 - thermistor under temperature alarm on threshold(deg C)
    PARM(HslaUtOffThrsh),     // 7 - thermistor under temperature alarm off threshold(deg C)
    RhslaOtDlyTrpFlg,         // 8 - delayed over temperature trip flag
    RhslaOtDlyAlmFlg,         // 9 - delayed over temperature alarm flag
    RhslaUtDlyTrpFlg,         //10 - delayed under temperature trip flag
    RhslaUtDlyAlmFlg,         //11 - delayed under temperature alarm flag
};

ThermisDlyThrmTyp L_InletAirThermis =
{
    InletAirTemp,             // 1 - thermistor temperature (deg C)
    PARM(InletAirOtThrsh),    // 2 - thermistor over temperature trip threshold(deg C)
    PARM(InletAirOtOnThrsh),  // 3 - thermistor over temperature alarm on threshold(deg C)
    PARM(InletAirOtOffThrsh), // 4 - thermistor over temperature alarm off threshold(deg C)
    FZero,                    // 5 - thermistor under temperature trip threshold(deg C)
    PARM(InletAirUtOnThrsh),  // 6 - thermistor under temperature alarm on threshold(deg C)
    PARM(InletAirUtOffThrsh), // 7 - thermistor under temperature alarm off threshold(deg C)
    InletAirOtDlyTrpFlg,      // 8 - delayed over temperature trip flag
    InletAirOtDlyAlmFlg,      // 9 - delayed over temperature alarm flag
    FlagNa,                   //10 - delayed under temperature trip flag
    InletAirUtDlyAlmFlg,      //11 - delayed under temperature alarm flag
};

ThermisDlyThrmTyp PdcThermis =
{
    PdcTemp,                // 1 - thermistor temperature (deg C)
    PARM(PdcOtThrsh),       // 2 - thermistor over temperature trip threshold(deg C)
    PARM(PdcOtOnThrsh),     // 3 - thermistor over temperature alarm on threshold(deg C)
    PARM(PdcOtOffThrsh),    // 4 - thermistor over temperature alarm off threshold(deg C)
    FZero,                  // 5 - thermistor under temperature trip threshold(deg C)
    PARM(PdcUtOnThrsh),     // 6 - thermistor under temperature alarm on threshold(deg C)
    PARM(PdcUtOffThrsh),    // 7 - thermistor under temperature alarm off threshold(deg C)
    PdcOtDlyTrpFlg,         // 8 - delayed over temperature trip flag
    PdcOtDlyAlmFlg,         // 9 - delayed over temperature alarm flag
    FlagNa,                 //10 - delayed under temperature trip flag
    PdcUtDlyAlmFlg,         //11 - delayed under temperature alarm flag
};

ThermisDlyThrmTyp CfcThermis =
{
    CfcTemp,                // 1 - thermistor temperature (deg C)
    PARM(CfcOtThrsh),       // 2 - thermistor over temperature trip threshold(deg C)
    PARM(CfcOtOnThrsh),     // 3 - thermistor over temperature alarm on threshold(deg C)
    PARM(CfcOtOffThrsh),    // 4 - thermistor over temperature alarm off threshold(deg C)
    FZero,                  // 5 - thermistor under temperature trip threshold(deg C)
    PARM(CfcUtOnThrsh),     // 6 - thermistor under temperature alarm on threshold(deg C)
    PARM(CfcUtOffThrsh),    // 7 - thermistor under temperature alarm off threshold(deg C)
    CfcOtDlyTrpFlg,         // 8 - delayed over temperature trip flag
    CfcOtDlyAlmFlg,         // 9 - delayed over temperature alarm flag
    FlagNa,                 //10 - delayed under temperature trip flag
    CfcUtDlyAlmFlg,         //11 - delayed under temperature alarm flag
};

ThermisDlyThrmTyp L_CoolantThermis =
{
    CoolantTemp,            // 1 - thermistor temperature (deg C)
    PARM(CoolantOtDlyThrsh),// 2 - thermistor over temperature trip threshold(deg C)
    PARM(CoolantOtOnThrsh), // 3 - thermistor over temperature alarm on threshold(deg C)
    PARM(CoolantOtOffThrsh),// 4 - thermistor over temperature alarm off threshold(deg C)
    PARM(CoolantUtThrsh),   // 5 - thermistor under temperature trip threshold(deg C)
    PARM(CoolantUtOnThrsh), // 6 - thermistor under temperature alarm on threshold(deg C)
    PARM(CoolantUtOffThrsh),// 7 - thermistor under temperature alarm off threshold(deg C)
    CoolantOtDlyTrpFlg,     // 8 - delayed over temperature trip flag
    CoolantOtDlyAlmFlg,     // 9 - delayed over temperature alarm flag
    CoolantUtDlyTrpFlg,     //10 - delayed under temperature trip flag
    CoolantUtDlyAlmFlg,     //11 - delayed under temperature alarm flag
};

ThermisDlyThrmTyp R_AclfuThermis =
{
    AclfuTemp,                // 1 - thermistor temperature (deg C)
    PARM(AclfuOtThrsh),       // 2 - thermistor over temperature trip threshold(deg C)
    PARM(AclfuOtOnThrsh),     // 3 - thermistor over temperature alarm on threshold(deg C)
    PARM(AclfuOtOffThrsh),    // 4 - thermistor over temperature alarm off threshold(deg C)
    FZero,                    // 5 - thermistor under temperature trip threshold(deg C)
    FZero,                    // 6 - thermistor under temperature alarm on threshold(deg C)
    FZero,                    // 7 - thermistor under temperature alarm off threshold(deg C)
    AclfuOtDlyTrpFlg,         // 8 - delayed over temperature trip flag
    AclfuOtDlyAlmFlg,         // 9 - delayed over temperature alarm flag
    FlagNa,                   //10 - delayed under temperature trip flag
    FlagNa,                   //11 - delayed under temperature alarm flag
};

ThermisDlyThrmTyp R_PdcCabEnThermis =
{
    PdcCabEntryTemp,           // 1 - thermistor temperature (deg C)
    PARM(PdcCabEnOtThrsh),     // 2 - thermistor over temperature trip threshold(deg C)
    PARM(PdcCabEnOtOnThrsh),   // 3 - thermistor over temperature alarm on threshold(deg C)
    PARM(PdcCabEnOtOffThrsh),  // 4 - thermistor over temperature alarm off threshold(deg C)
    FZero,                     // 5 - thermistor under temperature trip threshold(deg C)
    PARM(PdcCabEnUtOnThrsh),   // 6 - thermistor under temperature alarm on threshold(deg C)
    PARM(PdcCabEnUtOffThrsh),  // 7 - thermistor under temperature alarm off threshold(deg C)
    PdcCabEnOtDlyTrpFlg,       // 8 - delayed over temperature trip flag
    PdcCabEnOtDlyAlmFlg,       // 9 - delayed over temperature alarm flag
    FlagNa,                    //10 - delayed under temperature trip flag
    PdcCabEnUtDlyAlmFlg,       //11 - delayed under temperature alarm flag
};

// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      The purpose of this function is to convert four vco voltages
//      into four temperatures.  These voltages are read by a circuit on the
//      AEAD card and the four cirucits one that are identical.  For each
//      thermistor, there are two voltage readings, one for the high temperature
//      range, and another for the low temperature range.  All of these voltages
//      are converted to resistance, based on the card circuit.  Each of the
//      eight readings is done within a task3 period by the task 2 functions
//      that converts vco frequency to voltage. Each voltage is also conveted
//      to a temperature based on look-up tables. This function then determines
//      which converted temperature to use for each thermistor based on the
//      calculated resistance.  There is a transition range of resistance (and
//      therefore temperature) for each thermistor type. When the feedback shows
//      that the resistance is this transition range, the calculated high and
//      low range temperatures are melded together based on the resistance
//      calculated from the high range reading.
//      This function also checks the delta between the temperature calculated
//      at the high range and low range, alarms are generated if delta is too
//      high. These alarms are intended to check for AEAD circuit problems,
//      either in resistance or a problem with the high/low selection switch.
//      There are two different types of thermisters accomodated by this
//      function. Thermisters 1,3 and 4 use one type and thermistor 2 uses
//      anohter. Thermister 2 has the same profile as the thermisters used on
//      the IGBTs, but must use a different table because the PWA hardware
//      cirucit is different. There are, therefore, four different voltage to
//      temperature look-up tables used here, two for each thermistor type
//      with two types of thermisters.
//      The temperature look-up is by voltage instead of resistance because
//      the voltage to temperature look-up is much more linear than the
//      resistance to temperature look-up. Using voltage allows use of a much
//      more even distribution of voltage inputs.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void IOThermalConversion( void )
{
    //int Temp;
    ///////////////////////////////////////////////////////////////////////
    //  I/O Thermal Conversion
    ///////////////////////////////////////////////////////////////////////


   /* In simulator mode set temperatures to nominal values
      except coolant temperature */

    L_ThrmTempCnts1 = L_ThrmTempCntsT3[0];
    R_ThrmTempCnts1 = R_ThrmTempCntsT3[0];

    if ( PwrDnFbkDsb || SimMode )

    {
       L_TempVltTh1Lw = PARM(ThrmstrVltLwSim);
       L_TempVltTh2Lw = PARM(ThrmstrVltLwSim);
       L_TempVltTh7Lw = PARM(ThrmstrVltLwSim);
       L_TempVltTh8Lw = PARM(ThrmstrVltLwSim);
       L_TempVltTh1Hi = PARM(ThrmstrVltHiSim);
       L_TempVltTh2Hi = PARM(ThrmstrVltHiSim);
       L_TempVltTh7Hi = PARM(ThrmstrVltHiSim);
       L_TempVltTh8Hi = PARM(ThrmstrVltHiSim);

       R_TempVltTh1Lw = PARM(ThrmstrVltLwSim);
       R_TempVltTh2Lw = PARM(ThrmstrVltLwSim);
       R_TempVltTh7Lw = PARM(ThrmstrVltLwSim);
       R_TempVltTh8Lw = PARM(ThrmstrVltLwSim);
       R_TempVltTh1Hi = PARM(ThrmstrVltHiSim);
       R_TempVltTh2Hi = PARM(ThrmstrVltHiSim);
       R_TempVltTh7Hi = PARM(ThrmstrVltHiSim);
       R_TempVltTh8Hi = PARM(ThrmstrVltHiSim);

       R2_TempVltTh1Lw = PARM(ThrmstrVltLwSim);
       R2_TempVltTh2Lw = PARM(ThrmstrVltLwSim);
       R2_TempVltTh7Lw = PARM(ThrmstrVltLwSim);
       R2_TempVltTh8Lw = PARM(ThrmstrVltLwSim);
       R2_TempVltTh1Hi = PARM(ThrmstrVltHiSim);
       R2_TempVltTh2Hi = PARM(ThrmstrVltHiSim);
       R2_TempVltTh7Hi = PARM(ThrmstrVltHiSim);
       R2_TempVltTh8Hi = PARM(ThrmstrVltHiSim);
    }
    else //non-sim mode or sim mode and bridge enabled.
    {

       if ( Init1Sec && !PwrDnFbkDsb)
       {
          TempIOFbkVld  = true;
       }
       else
       {
          TempIOFbkVld  = false;
       }


        ///////////////////////////////////////////////////////////////////////
        //  Convert thermistor delta-vco counts to volts
        ///////////////////////////////////////////////////////////////////////

        // Line bridge
        if (  ((TempRngSel == THRM_RNG_AUTO) &&  L_ThrmHiSelT3)        // toggling L_ThrmHiSel: use complement (= Z^-1 value)
            ||((TempRngSel != THRM_RNG_AUTO) && !L_ThrmHiSelT3)  )     // static L_ThrmHiSel:  use directly
        {

            // Convert Th1 vco to volts -- Low Range
            L_TempTh1RawLw  = VCO_MASK & L_ThrmTempCntsT3[0];
            L_TempTh1FrqLw  = L_TempTh1RawLw * L_TempVcoDelTmRcp;
            L_TempVltTh1Lw  = (L_TempTh1FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th2 vco to volts -- Low Range
            L_TempTh2RawLw  = VCO_MASK & L_ThrmTempCntsT3[1];
            L_TempTh2FrqLw  = L_TempTh2RawLw * L_TempVcoDelTmRcp;
            L_TempVltTh2Lw  = (L_TempTh2FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th7 vco to volts -- Low Range
            L_TempTh7RawLw  = VCO_MASK & L_ThrmTempCntsT3[2];
            L_TempTh7FrqLw  = L_TempTh7RawLw * L_TempVcoDelTmRcp;
            L_TempVltTh7Lw  = (L_TempTh7FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th8 vco to volts -- Low Range
            L_TempTh8RawLw  = VCO_MASK & L_ThrmTempCntsT3[3];
            L_TempTh8FrqLw  = L_TempTh8RawLw * L_TempVcoDelTmRcp;
            L_TempVltTh8Lw  = (L_TempTh8FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);
        }
        else
        {

            // Convert Th1 vco to volts -- High Range
            L_TempTh1RawHi  = VCO_MASK & L_ThrmTempCntsT3[0];
            L_TempTh1FrqHi  = L_TempTh1RawHi * L_TempVcoDelTmRcp;
            L_TempVltTh1Hi  = (L_TempTh1FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th2 vco to volts -- High Range
            L_TempTh2RawHi  = VCO_MASK & L_ThrmTempCntsT3[1];
            L_TempTh2FrqHi  = L_TempTh2RawHi * L_TempVcoDelTmRcp;
            L_TempVltTh2Hi  = (L_TempTh2FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th7 vco to volts -- High Range
            L_TempTh7RawHi  = VCO_MASK & L_ThrmTempCntsT3[2];
            L_TempTh7FrqHi  = L_TempTh7RawHi * L_TempVcoDelTmRcp;
            L_TempVltTh7Hi  = (L_TempTh7FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th8 vco to volts -- High Range
            L_TempTh8RawHi  = VCO_MASK & L_ThrmTempCntsT3[3];
            L_TempTh8FrqHi  = L_TempTh8RawHi * L_TempVcoDelTmRcp;
            L_TempVltTh8Hi  = (L_TempTh8FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);
        }

        // Rotor bridge 1
        if (  ((TempRngSel == THRM_RNG_AUTO) && R_ThrmHiSelT3)        // toggling R_ThrmHiSel: use complement (= Z^-1 value)
            ||((TempRngSel != THRM_RNG_AUTO) && !R_ThrmHiSelT3)  )     // static R_ThrmHiSel:  use directly
        {
            // Bridge 1
            // Convert Th1 vco to volts -- Low Range
            R_TempTh1RawLw  = VCO_MASK & R_ThrmTempCntsT3[0];
            R_TempTh1FrqLw  = R_TempTh1RawLw * R_TempVcoDelTmRcp;
            R_TempVltTh1Lw  = (R_TempTh1FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th2 vco to volts -- Low Range
            R_TempTh2RawLw  = VCO_MASK & R_ThrmTempCntsT3[1];
            R_TempTh2FrqLw  = R_TempTh2RawLw * R_TempVcoDelTmRcp;
            R_TempVltTh2Lw  = (R_TempTh2FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th7 vco to volts -- Low Range
            R_TempTh7RawLw  = VCO_MASK & R_ThrmTempCntsT3[2];
            R_TempTh7FrqLw  = R_TempTh7RawLw * R_TempVcoDelTmRcp;
            R_TempVltTh7Lw  = (R_TempTh7FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th8 vco to volts -- Low Range
            R_TempTh8RawLw  = VCO_MASK & R_ThrmTempCntsT3[3];
            R_TempTh8FrqLw  = R_TempTh8RawLw * R_TempVcoDelTmRcp;
            R_TempVltTh8Lw  = (R_TempTh8FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);
        }
        else
        {
            // Convert Th1 vco to volts -- High Range
            R_TempTh1RawHi  = VCO_MASK & R_ThrmTempCntsT3[0];
            R_TempTh1FrqHi  = R_TempTh1RawHi * R_TempVcoDelTmRcp;
            R_TempVltTh1Hi  = (R_TempTh1FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th2 vco to volts -- High Range
            R_TempTh2RawHi  = VCO_MASK & R_ThrmTempCntsT3[1];
            R_TempTh2FrqHi  = R_TempTh2RawHi * R_TempVcoDelTmRcp;
            R_TempVltTh2Hi  = (R_TempTh2FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th7 vco to volts -- High Range
            R_TempTh7RawHi  = VCO_MASK & R_ThrmTempCntsT3[2];
            R_TempTh7FrqHi  = R_TempTh7RawHi * R_TempVcoDelTmRcp;
            R_TempVltTh7Hi  = (R_TempTh7FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th8 vco to volts -- High Range
            R_TempTh8RawHi  = VCO_MASK & R_ThrmTempCntsT3[3];
            R_TempTh8FrqHi  = R_TempTh8RawHi * R_TempVcoDelTmRcp;
            R_TempVltTh8Hi  = (R_TempTh8FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);
        }
         /*
        // Rotor bridge 2
        if (  ((TempRngSel == THRM_RNG_AUTO) &&  R_ThrmHiSelT3)        // toggling R_ThrmHiSel: use complement (= Z^-1 value)
            ||((TempRngSel != THRM_RNG_AUTO) && !R_ThrmHiSelT3)  )     // static R_ThrmHiSel:  use directly
        {
            // Convert Th1 vco to volts -- Low Range
            R2_TempTh1RawLw  = VCO_MASK & R_ThrmTempCntsT3[4];
            R2_TempTh1FrqLw  = R2_TempTh1RawLw * R_TempVcoDelTmRcp;
            R2_TempVltTh1Lw  = (R2_TempTh1FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th2 vco to volts -- Low Range
            R2_TempTh2RawLw  = VCO_MASK & R_ThrmTempCntsT3[5];
            R2_TempTh2FrqLw  = R2_TempTh2RawLw * R_TempVcoDelTmRcp;
            R2_TempVltTh2Lw  = (R2_TempTh2FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th7 vco to volts -- Low Range
            R2_TempTh7RawLw  = VCO_MASK & R_ThrmTempCntsT3[6];
            R2_TempTh7FrqLw  = R2_TempTh7RawLw * R_TempVcoDelTmRcp;
            R2_TempVltTh7Lw  = (R2_TempTh7FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th8 vco to volts -- Low Range
            R2_TempTh8RawLw  = VCO_MASK & R_ThrmTempCntsT3[7];
            R2_TempTh8FrqLw  = R2_TempTh8RawLw * R_TempVcoDelTmRcp;
            R2_TempVltTh8Lw  = (R2_TempTh8FrqLw - PARM(TempVcoZero)) * PARM(TempVcoScl);
        }
        else
        {
            // Convert Th1 vco to volts -- High Range
            R2_TempTh1RawHi  = VCO_MASK & R_ThrmTempCntsT3[4];
            R2_TempTh1FrqHi  = R2_TempTh1RawHi * R_TempVcoDelTmRcp;
            R2_TempVltTh1Hi  = (R2_TempTh1FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th2 vco to volts -- High Range
            R2_TempTh2RawHi  = VCO_MASK & R_ThrmTempCntsT3[5];
            R2_TempTh2FrqHi  = R2_TempTh2RawHi * R_TempVcoDelTmRcp;
            R2_TempVltTh2Hi  = (R2_TempTh2FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th7 vco to volts -- High Range
            R2_TempTh7RawHi  = VCO_MASK & R_ThrmTempCntsT3[6];
            R2_TempTh7FrqHi  = R2_TempTh7RawHi * R_TempVcoDelTmRcp;
            R2_TempVltTh7Hi  = (R2_TempTh7FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);

            // Convert Th8 vco to volts -- High Range
            R2_TempTh8RawHi  = VCO_MASK & R_ThrmTempCntsT3[7];
            R2_TempTh8FrqHi  = R2_TempTh8RawHi * R_TempVcoDelTmRcp;
            R2_TempVltTh8Hi  = (R2_TempTh8FrqHi - PARM(TempVcoZero)) * PARM(TempVcoScl);
        }*/
    }

    //Copy range selection parm
    TempRngSel  = PARM(ThrmRangeSel);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Coordination of thermistor open detection and differential failure.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ThermalOpenFlg( void )
{
    TherBadDlySm = false;

    //-------------------------------------------------------------------------
    // Delta thermistor fault coordination (BTM_031)
    AirInThrmDlFlg    = LIo1_Thrmstr.IOThrm1DlFlg;    // InletAirTemp
    PdcThrmDlFlg      = LIo1_Thrmstr.IOThrm2DlFlg;    // PdcTemp
    CfcThrmDlFlg      = LIo1_Thrmstr.IOThrm7DlFlg;    // CfcTemp
    LqdThrmDlFlg      = LIo1_Thrmstr.IOThrm8DlFlg;    // CoolantTemp

    GenAeadThrm1DlFlg = RIo1_Thrmstr.IOThrm1DlFlg;
    GenAeadThrm2DlFlg = RIo1_Thrmstr.IOThrm2DlFlg;
    AclfuThrmDlFlg    = RIo1_Thrmstr.IOThrm7DlFlg;    // AclfuTemp
    PdcCabEnThrmDlFlg = RIo1_Thrmstr.IOThrm8DlFlg;    // PdcCabEntryTemp


    //-------------------------------------------------------------------------
    // Open thermistor fault coordination
    AirInThrmOpnFlg   = LIo1_Thrmstr.Thrm1OpenFlg;    // InletAirTemp
    PdcThrmOpnFlg     = LIo1_Thrmstr.Thrm2OpenFlg;    // PdcTemp
    CfcThrmOpnFlg     = LIo1_Thrmstr.Thrm7OpenFlg;    // CfcTemp
    LqdThrmOpnFlg     = LIo1_Thrmstr.Thrm7OpenFlg;    // CoolantTemp

    GenAeadThrm1OpnFlg = RIo1_Thrmstr.Thrm1OpenFlg;
    GenAeadThrm2OpnFlg = RIo1_Thrmstr.Thrm2OpenFlg;
    AclfuThrmOpnFlg    = RIo1_Thrmstr.Thrm7OpenFlg;    // AclfuTemp
    PdcCabEnThrmOpnFlg = RIo1_Thrmstr.Thrm8OpenFlg;    // PdcCabEntryTemp


    ThrmOpnFlg = AirInThrmOpnFlg    || PdcThrmOpnFlg      || CfcThrmOpnFlg   || LqdThrmOpnFlg ||
                 GenAeadThrm1OpnFlg || GenAeadThrm2OpnFlg || AclfuThrmOpnFlg || PdcCabEnThrmOpnFlg;

    if ( PARM(TestMode) == PUMP_BACK )
    {
        if ( ThrmOpnFlg     ||
            AirInThrmDlFlg    ||  PdcThrmDlFlg      || CfcThrmDlFlg   || LqdThrmDlFlg ||
            GenAeadThrm1DlFlg ||  GenAeadThrm2DlFlg || AclfuThrmDlFlg || PdcCabEnThrmDlFlg )
        {
            TherBadDlySm = true;
        }
    }
    else
    {
        // HL: the logic matches the design spec rev 16
        if ( (AirInThrmOpnFlg    || AirInThrmDlFlg)   || (PdcThrmOpnFlg      || PdcThrmDlFlg)      ||
             (CfcThrmOpnFlg      || CfcThrmDlFlg)     || (LqdThrmOpnFlg      || LqdThrmDlFlg)      ||
             (GenAeadThrm1OpnFlg || GenAeadThrm1DlFlg)|| (GenAeadThrm2OpnFlg    || GenAeadThrm2DlFlg) ||
             (AclfuThrmOpnFlg ||  AclfuThrmDlFlg)   || (PdcCabEnThrmOpnFlg || PdcCabEnThrmDlFlg)
           )
        {
            TherBadDlySm = true;
        }
    }

    //-------------------------------------------------------------------------
    // done
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Coordination of thermistor short detection in support of heater and
//      fan control.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ThermalShrtFlg( void )
{
    //-------------------------------------------------------------------------
    //Short thermistor fault coordination-- required by heater & fan control
    AirInThrmShrtFlg   = LIo1_Thrmstr.Thrm1ShrtFlg;  // InletAirTemp
    PdcThrmShrtFlg     = LIo1_Thrmstr.Thrm2ShrtFlg;  // PdcTemp
    CfcThrmShrtFlg     = LIo1_Thrmstr.Thrm7ShrtFlg;  // CfcTemp
    LqdThrmShrtFlg     = LIo1_Thrmstr.Thrm8ShrtFlg;  // CoolantTemp


    GenAeadThrm1ShrtFlg  = RIo1_Thrmstr.Thrm1ShrtFlg;
    GenAeadThrm2ShrtFlg  = RIo1_Thrmstr.Thrm2ShrtFlg;
    AclfuThrmShrtFlg     = RIo1_Thrmstr.Thrm7ShrtFlg;  // AclfuTemp
    PdcCabEnThrmShrtFlg  = RIo1_Thrmstr.Thrm8ShrtFlg;  // PdcCabEntryTemp


    //-------------------------------------------------------------------------
    // done
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Thermistor delayed thermal check. Outputs of this method is 4 flags:
//      delayed over temperature trip flag, delayed over temperature alarm
//      flag, Delayed under temperature trip flag, delayed under temperature
//      alarm flag.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ThermisDlyThrm (ThermisDlyThrmTyp& Plist)
{

    //-------------------------------------------------------------------------
    // over-temperature delayed-trip and alarm
    if ( Plist.ThrmTemp > Plist.ThrmOtTrpThrsh )
    {
        Plist.ThrmOtTrpFlg  = true;
    }
    else
    {
        Plist.ThrmOtTrpFlg  = false;
    }

    if ( Plist.ThrmTemp > Plist.ThrmOtAlmOnThrsh )
    {
        Plist.ThrmOtAlmFlg = true;
    }
    else if ( Plist.ThrmTemp < Plist.ThrmOtAlmOffThrsh )
    {
        Plist.ThrmOtAlmFlg = false;
    }

    //-------------------------------------------------------------------------
    // under-temperature delayed-trip and alarm
    if ( Plist.ThrmTemp < Plist.ThrmUtTrpThrsh )
    {
        Plist.ThrmUtTrpFlg  = true;
    }
    else
    {
        Plist.ThrmUtTrpFlg  = false;
    }

    if ( Plist.ThrmTemp < Plist.ThrmUtAlmOnThrsh )
    {
        Plist.ThrmUtAlmFlg = true;
    }
    else if ( Plist.ThrmTemp > Plist.ThrmUtAlmOffThrsh )
    {
        Plist.ThrmUtAlmFlg = false;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Calculate ambient temperature and check for over and under temperature
//      conditions.
//
// EXECUTION LEVEL
//      Task3 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void AmbientTemperatureProtection( void )
{
    //int Temp;
    ///////////////////////////////////////////////////////////////////////
    //  Ambient Temperature Protection (BTM_035--Common)
    ///////////////////////////////////////////////////////////////////////

    //-------------------------------------------------------------------------
    // line side Hsla abmient over-temperature fault, delayed-trip, and alarm
    ThermisDlyThrm(L_HslaThermis);

     // push the line side HSLA delayed trips and alarms
    if ( LhslaOtDlyTrpFlg )
    {
        PUSH_DIAG( LhslaOtTrpDly );
    }
    if ( LhslaOtDlyAlmFlg )
    {
        PUSH_DIAG( LhslaOtAlmDly );
    }
    if ( LhslaUtDlyTrpFlg )
    {
        PUSH_DIAG( LhslaUtTrpDly );
    }
    if ( LhslaUtDlyAlmFlg )
    {
        PUSH_DIAG( LhslaUtAlmDly );
    }

    //-------------------------------------------------------------------------
    // rotor side Hsla abmient over-temperature fault, delayed-trip, and alarm
    ThermisDlyThrm(R_HslaThermis);

    // push the rotor side HSLA delayed trips and alarms
    if ( RhslaOtDlyTrpFlg )
    {
        PUSH_DIAG( RhslaOtTrpDly );
    }
    if ( RhslaOtDlyAlmFlg )
    {
        PUSH_DIAG( RhslaOtAlmDly );
    }
    if ( RhslaUtDlyTrpFlg )
    {
        PUSH_DIAG( RhslaUtTrpDly );
    }
    if ( RhslaUtDlyAlmFlg )
    {
        PUSH_DIAG( RhslaUtAlmDly );
    }

    //-------------------------------------------------------------------------
    // done
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Compare thermistor I/O readings against alarm thresholds and push
//      corresponding alarms.
//
// EXECUTION LEVEL
//      Task3 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ThermistBasedProt( void )
{
    //-------------------------------------------------------------------------
    //Clear the flags containing delayed trips every pass
    InletAirOtDlyTrpFlg  = false;
    PdcOtDlyTrpFlg       = false;
    CfcOtDlyTrpFlg       = false;
    CoolantOtDlyTrpFlg       = false;

    AclfuOtDlyTrpFlg     = false;
    PdcCabEnOtDlyTrpFlg  = false;


    //-------------------------------------------------------------------------
    // Inlet Air dleayed thermal diagnostics
    ThermisDlyThrm(L_InletAirThermis);

    // push the delayed temperature trips and alarms
    if ( InletAirOtDlyTrpFlg )
    {
        PUSH_DIAG( InletAirOtTrpDly );
    }
    if ( InletAirOtDlyAlmFlg )
    {
        PUSH_DIAG( InletAirOtAlmDly );
    }
    if ( InletAirUtDlyAlmFlg )
    {
        PUSH_DIAG( InletAirUtAlmDly );
    }

    //-------------------------------------------------------------------------
    // PDC temperature delayed thermal diagnostics
    ThermisDlyThrm(PdcThermis);
    if (PdcOtDlyTrpFlg)
    {
        PUSH_DIAG(PdcOtDly);
    }
    if (PdcOtDlyAlmFlg)
    {
        PUSH_DIAG(PdcOtAlm);
    }
    if (PdcUtDlyAlmFlg)
    {
        PUSH_DIAG(PdcUtAlmDly);
    }

    //-------------------------------------------------------------------------
    // CFC temperature delayed thermal diagnostics
    ThermisDlyThrm(CfcThermis);
    if (CfcOtDlyTrpFlg)
    {
        PUSH_DIAG(CfcOtDly);
    }
    if (CfcOtDlyAlmFlg)
    {
        PUSH_DIAG(CfcOtAlm);
    }
    if (CfcUtDlyAlmFlg)
    {
        PUSH_DIAG(CfcUtAlmDly);
    }

    //-------------------------------------------------------------------------
    // liquid coolant dleayed thermal diagnostics
    ThermisDlyThrm(L_CoolantThermis);

    // push the delayed temperature trips and alarms
    if ( CoolantTemp>PARM(CoolantOtThrsh))
    {
        PUSH_DIAG( CoolantOtTrp );
    }
    if ( CoolantOtDlyTrpFlg )
    {
        PUSH_DIAG( CoolantOtDly );
    }
    if ( CoolantOtDlyAlmFlg )
    {
        PUSH_DIAG( CoolantOtAlm );
    }
    if ( CoolantUtDlyAlmFlg )
    {
        PUSH_DIAG( CoolantUtAlm );
    }
    if ( CoolantUtDlyTrpFlg )
    {
        CoolantUtDlyTrpTmr += DelTm3;
        if(CoolantUtDlyTrpTmr>PARM(CoolntUtTm))
        {
            CoolantUtDlyTrpTmr = PARM(CoolntUtTm);
            CoolantUtDlyTrpFlg2 = true;
            PUSH_DIAG( CoolantUtDly );
        }
        else
        {
            CoolantUtDlyTrpFlg2 = false;
        }
    }
    else
    {
        CoolantUtDlyTrpTmr = 0.0F;
        CoolantUtDlyTrpFlg2 = false;
    }


    //-------------------------------------------------------------------------
    // AC line fuse temperature dleayed thermal diagnostics
    ThermisDlyThrm(R_AclfuThermis);

    // push the delayed temperature trips and alarms
    if ( AclfuOtDlyTrpFlg )
    {
        PUSH_DIAG( AclfuOtTrpDly );
    }
    if ( AclfuOtDlyAlmFlg )
    {
        PUSH_DIAG( AclfuOtAlmDly );
    }


    //-------------------------------------------------------------------------
    // Exchanged-in liquid coolant dleayed thermal diaganostics
    ThermisDlyThrm(R_PdcCabEnThermis);

    // push the  delayed temperature trips and alarms
    if ( PdcCabEnOtDlyTrpFlg )
    {
        PUSH_DIAG( PdcCabEnOtTrpDly );
    }
    if ( PdcCabEnOtDlyAlmFlg )
    {
        PUSH_DIAG( PdcCabEnOtAlmDly );
    }
    if ( PdcCabEnUtDlyAlmFlg )
    {
        PUSH_DIAG( PdcCabEnUtAlmDly );
    }


    //-------------------------------------------------------------------------
    // generate the overall fault bits (all the thermistors OtDlyFlg)
    if (InletAirOtDlyTrpFlg ||
        PdcOtDlyTrpFlg      ||
        CfcOtDlyTrpFlg      ||
        CoolantOtDlyTrpFlg  || CoolantUtDlyTrpFlg2  ||
        PdcCabEnOtDlyTrpFlg || AclfuOtDlyTrpFlg ||
        LhslaOtDlyTrpFlg    || LhslaUtDlyTrpFlg ||
        RhslaOtDlyTrpFlg    || RhslaUtDlyTrpFlg
        )
    {
        ThermisDlySm = true;
    }
    else
    {
        ThermisDlySm = false;
    }

    //-------------------------------------------------------------------------
    // generate the overall alarm bits (all the thermistors OtDlyAlmFlg)
    if ( InletAirOtDlyAlmFlg|| InletAirUtDlyAlmFlg ||
         CfcOtDlyAlmFlg     || CfcUtDlyAlmFlg ||
         PdcOtDlyAlmFlg     || PdcUtDlyAlmFlg ||
         CoolantOtDlyAlmFlg     || CoolantUtDlyAlmFlg   ||
         AclfuOtDlyAlmFlg   ||
         PdcCabEnOtDlyAlmFlg|| PdcCabEnUtDlyAlmFlg ||
         LhslaOtDlyAlmFlg   || RhslaOtDlyAlmFlg    ||
         LhslaUtDlyAlmFlg   || RhslaUtDlyAlmFlg
        )
    {
        ThermisAlmSm = true;
    }
    else
    {
        ThermisAlmSm = false;
    }

    //-------------------------------------------------------------------------
    //done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Generate the command to shut down the controller power if the
//      temperature inside the converter control cabinet is <-25 deg C
//
// EXECUTION LEVEL
//      Task3 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void SealInSuicide( void )
{

    if ( MccTemp < PARM(HslaUtOnThrsh) )
    {
        // Trip the converter first before shut down the controller power
        PUSH_DIAG(LhslaOtTrpDly);
        // send the request to seal in
        EnvOkSealCmd = 0;
    }
    else
    {
        EnvOkSealCmd = 1;
    }

    //-------------------------------------------------------------------------
    //done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Check if temperatures are delivered via HSSL and they have had
//      time to settled down
//
// EXECUTION LEVEL
//      Task3 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void TempValidCheck(void)
{
    // get the previous value of the temperature valid
    TempIsValidZ = TempIsValid;

    //-------------------------------------------------------------------------
    // check if the temperature is valid
    if ( L_BridgeRebf.BrgHdwDataVld && R_BridgeRebf.BrgHdwDataVld )
    {
        if ( !TempFbkRdy || !TempIsValid )
        {
            // wait some time to let the temperature filter settle down
            TempFilterSettleTmr += DelTm3;

            if ( TempFilterSettleTmr > PARM(TempFbkRdyTm) )
            {
                TempFbkRdy = true;                 // getting valid data from AEAD's
            }

            if ( TempFilterSettleTmr > (PARM(TempFbkRdyTm) + PARM(TempFilterSettleTm)) )
            {
                TempIsValid = true;
            }
        }
    }
    else
    {
        TempFilterSettleTmr = 0.0F;
        TempFbkRdy          = false;
        TempIsValid         = false;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Check if the MA contactor, CCSTR are warmed up. Check if the liquid
//      and all IGBT baseplates have the desired temperature rises to start
//      the converter. Run the heat soak if the IGBT baseplate or liquie
//      temperature is low.
//
// EXECUTION LEVEL
//      Task3 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ThermistTempOkCheck(void)
{
    if ( TempIsValid )
    {
        //-------------------------------------------------------------------------
        // check if MA contactor is warmed up (to start the converter)
        if ( AclfuTemp >= PARM(MaContTempLowLvl) )
        {
            MaContNotCold = true;
        }
        else
        {
            MaContNotCold = false;
        }

        //-------------------------------------------------------------------------
        // check if capacitor charger starter is warmed up (to start the converter)
        if ( CfcTemp >= PARM(CcstarTempLowLvl) )
        {
            CcstarNotCold = true;
        }
        else
        {
            CcstarNotCold = false;
        }

        //-------------------------------------------------------------------------
        // check if the liquid and the IGBT baseplate are warm enough to let the converter run

        //-------------------------------------------------------------------------
        // The desired air ambient temperature
        if ( InletAirTemp >= PARM(InletAirTempLimMax) )
        {
            InletAirTempClamp = PARM(InletAirTempLimMax) ;
        }
        else if ( InletAirTemp < PARM(InletAirTempLimMin) )
        {
            InletAirTempClamp = PARM(InletAirTempLimMin);
        }
        else
        {
            InletAirTempClamp = InletAirTemp;
        }

        //-------------------------------------------------------------------------
        // calculate the min temperature (22 inlcuding all IGBT baseplates and liquid)
        MinRotor1BpTemp = MIN(MIN(MIN(R_BrgThrmstr.BthtrTempMinA,  R_BrgThrmstr.BthtrTempMinB),
                                      R_BrgThrmstr.BthtrTempMinC), R_BrgThrmstr.BthtrTempMinD);

        //MinRotor2BpTemp = MIN(MIN(MIN(R2_BrgThrmstr.BthtrTempMinA,  R2_BrgThrmstr.BthtrTempMinB),
                                      //fR2_BrgThrmstr.BthtrTempMinC), R2_BrgThrmstr.BthtrTempMinD);

        MinLineBpTemp   = MIN(MIN(L_BrgThrmstr.BthtrTempMinA, L_BrgThrmstr.BthtrTempMinB), L_BrgThrmstr.BthtrTempMinC);

        MinBrgLqdTemp  = MIN(MIN(MIN(CoolantTemp, PdcCabEntryTemp), MinRotor1BpTemp),MinLineBpTemp);

        BrgLqdTempRise = MinBrgLqdTemp - InletAirTempClamp;

        //-------------------------------------------------------------------------
        // check if the rise over the min temperature is ok
        if ( BrgLqdTempRise >= PARM(TempRiseOkLvl) )
        {
            BridgeNotCold = true;
        }
        else if ( BrgLqdTempRise < PARM(TempRiseNotOkLvl) )
        {
            BridgeNotCold = false;
        }
        // new logic in sepc rev21

        // logic from spec rev21
        HtrFanRunOk = PARM(HtrFanEnb);
        HtrFanRunOk = PARM(HtrFanEnb);

        // -------------------------------------------------------------------------
        // Heater soak the timer
        if ( BridgeNotCold )
        {
            TempRiseOkTmr += 1;
        }

        //-------------------------------------------------------------------------
        // time down if the liquid temperature is not within the allowed range
        else
        {
            TempRiseOkTmr += -1;
        }
        // consider the converter down time in the heat soak time, execute this only once
        if ( (CnvShutDownTm > 0) && (!CnvShutDownFlg) )
        {
            TempRiseOkTmr -= int(float(CnvShutDownTm) * DelTm3Rcp);
            CnvShutDownFlg = true;

            if (TempRiseOkTmr <=0)
            {
                TempRiseOkTmr = 0;
            }
        }
        // convert the TempRiseOkTmr to min for display only
        TempRiseOkTmrDsp = TempRiseOkTmr * DelTm3 * 0.016667F;
        // the end of added new logic

    //-------------------------------------------------------------------------
    // calculate the overrall brg OT alarm
    BrgOvrTempFlg = R_OTAlSum || L_OTAlSum;      // Bridge trasistor and diode OT alarm, and Bridge thermistor OT alarm
    }

    //-------------------------------------------------------------------------
    // if temperature is not valid, do not make any run permissive decision
    else
    {
        HtrFanSupresTmr = 0.0F;
    }

    // by pass the heater soak If the controller is "rebooting" && the liquid temperature is
    // within the desired run permissive range
    // "rebooting" actually need to be after the HSSL links are running and the temperatures are
    // processed by the REBF, and the temperature filter is settled

    if ( InletAirTemp >= PARM(DcLinkNotFrostThrsh) )
    {
        DcLinkTempOk = true;
    }
    else if ( InletAirTemp < PARM(DcLinkFrostThrsh) )
    {
        DcLinkTempOk = false;
    }
    if ( DcLinkTempOk )
    {
        DcLinkHtrSoakTm = PARM(TempRiseOkTmNom);
    }
    else
    {
        DcLinkHtrSoakTm = PARM(TempRiseOkTmMax);
    }
    // convert the DcLinkHtrSoakTm to sec
    DcLinkHtrSoakTmDsp = DcLinkHtrSoakTm * DelTm3 * 0.016667F;

    // comment out the old logic
#if 0
    //-------------------------------------------------------------------------
    // limit the timer
    if ( TempRiseOkTmr >= PARM(TempRiseOkTmMax) )
    {
        TempRiseOkTmr = PARM(TempRiseOkTmMax);
    }
    else if ( TempRiseOkTmr <= 0 )
    {
        TempRiseOkTmr = 0;
    }

    //-------------------------------------------------------------------------
    // determine if the liquid is too cold (to start the converter)
    if ( TempRiseOkTmr >= PARM(TempRiseOkTmMax) )
    {
        TempRiseOk2Run = true;
    }
    else if ( TempRiseOkTmr <= 0 )
    {
        TempRiseOk2Run = false;
    }
#endif
    // the new logic in spec rev21
    //-------------------------------------------------------------------------
    // limit the timer
    if ( TempRiseOkTmr >= DcLinkHtrSoakTm )
    {
        TempRiseOkTmr = DcLinkHtrSoakTm;
    }
    else if ( TempRiseOkTmr <= 0 )
    {
        TempRiseOkTmr = 0;
    }

    //-------------------------------------------------------------------------
    // determine if the liquid is too cold (to start the converter)
    if ( TempRiseOkTmr >= DcLinkHtrSoakTm )
    {
        TempRiseOk2Run = true;
    }
    else if ( TempRiseOkTmr <= 0 )
    {
        TempRiseOk2Run = false;
    }

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Global run permissive from the thermistor temperatures
//
// EXECUTION LEVEL
//      Task3 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ThermistRunPermissive(void)
{
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//     Keep track of time and operating conditions  for a delayed thermal trip.
//
// EXECUTION LEVEL
//      Task3 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void TemperatureShutdownT3( void )
{

    if ( TmpTrpDlySum && !TempTrpLatch && L_Running)
    {
    // Following var assertion causes the TempShtDwn state to be pushed in task2
       TempTrpLatch = true;
       TTripPend    = true;
    }


    if ( TempTrpLatch )
    {
         TempDlyTmr = TempDlyTmr + DelTm3;
         if ( TempDlyTmr >= PARM(TempDlyTm))
         {
             PUSH_DIAG(TempDlyFlt);
             TempDlyTmr = PARM(TempDlyTm);
             if ( Stopped)
             {
                TempTrpLatch = false;
                TTripPend    = false;
             }
         }
         else if ( Stopped )
         {
             PUSH_DIAG(TempDlyFlt);
             TempTrpLatch = false;
             TTripPend    = false;
         }
    }
    else
    {
        TempDlyTmr = 0.0;
        if ( Stopped && TmpTrpDlySum)
        {
           PUSH_DIAG(TempDlyFlt);
        }
    }

}
