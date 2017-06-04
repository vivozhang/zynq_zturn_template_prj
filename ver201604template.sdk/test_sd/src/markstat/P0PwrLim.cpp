

/* INLINE EXPANSION CONTROL */
///////////////////////////////////////////////////////////////////////////////
//
// TITLE:        Source-side instrumentation
//
// DESCRIPTION:
//      Line calculation functions.
//
// COPYRIGHT:   Copyright (c) 2014
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     s0Instru.vsd, s0Instru.xls
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// core
//framework

#include "x0FrameWork.h"
#include "Product.h"
#include "P0PwrLim.h"

#include "b0BridgeRebf.h"
#include "b0BrgThrmstr.h"
#include "b0BrgThrm5p.h"
#include "b0DbThrm.h"


// Constants
//----------

#define TOC_ALM_SCL    1.0776F// = *SQRT( (1.0*1.0) + (1.15*1.15)/2.0 )  -- Heating 1/2 way from heated to trip
#define PWRLIM_STRCUR_GEN (1)
#define PWRLIM_STRCUR_CNV (2)
#define PWRLIM_STRCUR_CAB (3)
#define PWRLIM_RTRCUR_GEN (1)
#define PWRLIM_RTRCUR_CNV (2)
#define PWRLIM_RTRCUR_CAB (3)

/*****************************************************************************
*
*       external references
*
*****************************************************************************/

extern float        R_ISpdTmpRcp;
extern float        S_ISpdTmpRcp;
extern float        GmIMaxCrvMin;
extern float        AirTempGenCab;
extern float        AirTempGenIn;

extern unsigned     SysTmSec;
extern float        R_SpdFbkT3;

extern float        GmTocHiSel;
extern float        GmTocHiSelSq;

extern float        InletAirTemp;

extern float        L_TocHiSel;
extern float        L_TocHiSelSq;
extern float        R_DJncTemp;

extern float        R_JncTemp;
extern float        R_MaxCurOut;
extern float        R_TocHiSel;
extern float        R_TocHiSelSq;
extern float        R_VMag;

extern float        S_TocHiSel;
extern float        S_TocHiSelSq;

extern float        LJncTempGn;
extern float        RJncTempGn;
extern float        RtrVLimGn;
extern float        RtrVLimSqr;

extern float        DFTocHiSelSq;
extern float        DFTocHiSel;
extern float        L_FuseTocHiSel;
extern float        L_FuseTocHiSelSq;

extern float        DelTm3;
extern unsigned     R_SuperSync;


extern float        CfcTemp;
extern float        PdcTemp;
extern float        CoolantTemp;
extern float        InletAirTemp;
extern float        PdcCabEntryTemp;
extern float        AclfuTemp;

extern unsigned     IpcInterfaceOk;


extern float        L_BpTemp1;
extern float        L_BpTemp2;
extern float        L_BpTemp3;
extern float        L_BpTemp4;
extern float        L_BpTemp5;
extern float        L_BpTemp6;
extern float        R_BpTemp1;
extern float        R_BpTemp2;
extern float        R_BpTemp3;
extern float        R_BpTemp4;
extern float        R_BpTemp5;
extern float        R_BpTemp6;
extern float        R_BpTemp7;
extern float        R_BpTemp8;

extern cBrgThrmstr  L_BrgThrmstr;
extern cBrgThrmstr  R_BrgThrmstr;
extern float        CabAltDeRtSF;
extern float        R_TrqRat;
extern unsigned     CableLimType;
extern float        L_TocAlmOnDl;
extern float        L_TocAlmOffDl;
extern float        R_TocAlmOnDl;
extern float        R_TocAlmOffDl;
extern float        R_WTocT3;
extern float        R_WTocFstT3;
extern float        R_WTocT3Dyn;
extern float        GmWTocT3;
extern float        GmWTocFstT3;
extern float        GmWTocT3Dyn;



extern float        L_FuseTocAlmOnDl;
extern float        L_FuseTocAlmOffDl;

extern float        S_WTocT3;
extern float        S_WTocFstT3;
extern float        S_WTocT3Dyn;
extern float        S_TocAlmOnDl;
extern float        S_TocAlmOffDl;
extern float        L_FuseWTocT3;
extern float        L_FuseWTocFstT3;
extern float        L_FuseWTocT3Dyn;
extern float        L_WTocT3;
extern float        L_WTocFstT3;
extern float        L_WTocT3Dyn;

extern unsigned     PLCDiag2;
extern unsigned     IpcInterfaceDown;

// Types and Classes
//------------------
enum PwrLimDerateIdTpy
{
                                        // Bit  Bit Name
    NONE_DERATE=0,
    LNCNVCUR_DERATE       = 0x00000001,    //  0  Line converter curent, speed de-rate
                                           //  1  Had been used for LindTemp
    LNFUSECUR_DERATE      = 0x00000004,    //  2  Line fuse current (ACLFU), speed de-rate
    LNFUSETEMP_DERATE     = 0x00000008,    //  3  Line fuse temperature  (ACLFU), speed de-rate
    RTRVLT_DERATE         = 0x00000010,    //  4  Rotor voltage, speed de-rate
    LNBPTEMPIMB_DERATE    = 0x00000020,    //  5  Line converter baseplate temperature imbalance, speed de-rate
    LNBPTEMPRISE_DERATE   = 0x00000040,    //  6  Line converter baseplate temperature rise, speed de-rate
    GMCUR_DERATE          = 0x00010000,    // 16  Grid monitor current, torque de-rate
    RTRCNVCUR_DERATE      = 0x00020000,    // 17  Rotor converter current, torque de-rate
    RTRCABCUR_DERATE      = 0x00040000,    // 18  Rotor cable current, torque de-rate
    RTRGENCUR_DERATE      = 0x00080000,    // 19  Generator rotor current, torque de-rate
    STRCNVCUR_DERATE      = 0x00100000,    // 20  Stator DTA current, torque de-rate
    STRCABCUR_DERATE      = 0x00200000,    // 21  Stator cable current, torque de-rate
    STRGENCUR_DERATE      = 0x00400000,    // 22  Generator stator current, torque de-rate
    RTRBPTEMPRISE_DERATE  = 0x00800000,    // 23  Rotor converter baseplate temperature rise, torque de-rate
    RTRBPTEMPIMB_DERATE   = 0x01000000,    // 24  Rotor converter base-plate temerature imbalance, torque de-rate
    DNTWRTEMP_DERATE      = 0x02000000,    // 25  Down-tower temperature, torque de-rate
    CLNTTEMP_DERATE       = 0x04000000,    // 26  Coolant temperature, torque de-rate
                                           // 27  Had been used for R1indTemp
    PDCCABENTEMP_DERATE   = 0x10000000,    // 28  PDC cable entry temperature, torque de-rate
    CFCTEMP_DERATE        = 0x20000000,    // 29  CFC temperature, torque de-rate
    PDCTEMP_DERATE        = 0x40000000,    // 30  PDC temperature, torque de-rate
    DFCUR_DERATE          = 0x80000000,    // 31  Distortion Filter current, torque de-rate
};

// Diagnostics
//------------
// Speed Limiters
CREATE_DIAG(PwrLimLnCnvCurAlm);     // Power limiter, line converter current
CREATE_DIAG(PwrLimFuseCurAlm);      // Power limiter, ACLFU current
CREATE_DIAG(PwrLimFuseTempAlm);     // Power limiter, ACLFU temperature
CREATE_DIAG(PwrLimRtrVltAlm);       // Power limiter, rotor voltage
CREATE_DIAG(PwrLimLnBpImbAlm);      // Power limiter, line converter baseplate temperature imbalance
CREATE_DIAG(PwrLimLnBpRiseAlm);     // Power limiter, line converter baseplate temperature rise
// Torque Limiters
CREATE_DIAG(PwrLimGmCurAlm);        // Power limiter, grid monitor current
CREATE_DIAG(PwrLimRtrCnvCurAlm);    // Power limiter, rotor converter current
CREATE_DIAG(PwrLimRtrCabCurAlm);    // Power limiter, rotor cable current
CREATE_DIAG(PwrLimRtrGenCurAlm);    // Power limiter, generator rotor current
CREATE_DIAG(PwrLimStrDtaCurAlm);    // Power limiter, stator DTA current
CREATE_DIAG(PwrLimStrCabCurAlm);    // Power limiter, stator cable current
CREATE_DIAG(PwrLimStrGenCurAlm);    // Power limiter, generator stator current
CREATE_DIAG(PwrLimRtrBpRiseAlm);    // Power limiter, rotor converter baseplate temperature rise
CREATE_DIAG(PwrLimRtrBpImbAlm);     // Power limiter, rotor converter baseplate temperature imbalance
CREATE_DIAG(PwrLimDnTwrTempAlm);    // Power limiter, down-tower temperature
CREATE_DIAG(PwrLimClntTempAlm);     // Power limiter, coolant temperature
CREATE_DIAG(PwrLimPdcCabEnTmpAlm);  // Power limiter, PDC cable entry temperature
CREATE_DIAG(PwrLimCfcTempAlm);      // Power limiter, CFC temperature
CREATE_DIAG(PwrLimPdcTempAlm);      // Power limiter, PDC temperature

CREATE_DIAG(R_VacOVFlt);            // Rotor voltage over-volt flt

// Parameters
//-----------
// Diagnostic threshold
CREATE_PARM(PwrLimDiagThr,  float); // Diagnostic threshold for power limiter
CREATE_PARM(PwrLimSpdDiagThr,   float); // Diagnostic threshold for speed power limiter
CREATE_PARM(PwrLimTrqDiagThr,   float); // Diagnostic threshold for torque power limiter
CREATE_PARM(PwrLimHysLwThr,     float); // Hysteresis low threshold in power for power limit active detection
CREATE_PARM(PwrLimHysHiThr,     float); // Hysteresis high threshold in power for power limit active detection
CREATE_PARM(PwrLimHysHiTm,      float); // Time delay for hysteresis high threshold
CREATE_PARM(PwrLimTrqHysLwThr,  float); // Hysteresis low threshold in torque for power limit active detection
CREATE_PARM(PwrLimTrqHysHiThr,  float); // Hysteresis high threshold in torque for power limit active detection
CREATE_PARM(PwrLimSpdHysLwThr,  float); // Hysteresis low threshold in speed for power limit active detection
CREATE_PARM(PwrLimSpdHysHiThr,  float); // Hysteresis high threshold in speed for power limit active detection
//------------------------------------------------------------------------------
// Altitude parameters
CREATE_PARM(AltitudeTurb,         float);          // Altitude of turbine (meters)
CREATE_PARM(AltitudeFin,          float);          // Altitude of turbine used for calculation (meters)
CREATE_PARM(AltitudeRat,          float);          // Maximum operating altitude for full rating ..dreated above this (meters)
CREATE_PARM(AltitudeRatCab,       float);          // Maximum operating altitude for full rating for cables (meters)
CREATE_PARM(Altitude2,            float);          // Altitude 2 for rating 2 (meter)
CREATE_PARM(Altitude3,            float);          // Altitude 2 for rating 3 (meter)
CREATE_PARM(AltitudeLpse,         float);          // Altitude lapse (deg C/meter)
CREATE_PARM(WBIRatTempLo,         float);          // Low temp used for current rating  (deg C)
CREATE_PARM(WBIRatTempHi,         float);          // High temp used for current rating (deg C)
CREATE_PARM(WBTemp,               float);          // Turbine Power Limiter Control temperature (deg C)
CREATE_PARM(WBIRefTemp,           float);          // Reference temp used for current rating (deg C)
CREATE_PARM(WBAmbRefTemp,         float);          // Reference temp used for down tower temp derating (deg C)

//------------------------------------------------------------------------------
// general cable derating parameters
CREATE_PARM(CabAltDeRtK1,         float);          // Constant for cable der-rate 2nd order curve fit1
CREATE_PARM(CabAltDeRtX1,         float);          // 1st order term for cable der-rate 2nd order curve fit
CREATE_PARM(CabAltDeRtX2,         float);          // 2nd order term for cable der-rate 2nd order curve fit
// current limit
CREATE_PARM(LinILimDeRtK1,      float);     // Line current limit de-rating curve K1
CREATE_PARM(LinILimDeRtX1,      float);     // Line current limit de-rating curve X1
CREATE_PARM(LinILimDeRtX2,      float);     // Line current limit de-rating curve X2
CREATE_PARM(LinILimDeRtX3,      float);     // Line current limit de-rating curve X3
CREATE_PARM(GmILimDeRtK1,       float);     // Grid monitor current limit de-rating curve K1
CREATE_PARM(GmILimDeRtX1,       float);     // Grid monitor current limit de-rating curve X1
CREATE_PARM(GmILimDeRtX2,       float);     // Grid monitor current limit de-rating curve X2
CREATE_PARM(GmILimDeRtX3,       float);     // Grid monitor current limit de-rating curve X3
CREATE_PARM(RtrILimDeRtK1,      float);     // Rotor current limit de-rating curve K1
CREATE_PARM(RtrILimDeRtX1,      float);     // Rotor current limit de-rating curve X1
CREATE_PARM(RtrILimDeRtX2,      float);     // Rotor current limit de-rating curve X2
CREATE_PARM(RtrILimDeRtX3,      float);     // Rotor current limit de-rating curve X3
CREATE_PARM(StrILimDeRtK1,      float);     // Stator current limit de-rating curve K1
CREATE_PARM(StrILimDeRtX1,      float);     // Stator current limit de-rating curve X1
CREATE_PARM(StrILimDeRtX2,      float);     // Stator current limit de-rating curve X2
CREATE_PARM(StrILimDeRtX3,      float);     // Stator current limit de-rating curve X3
CREATE_PARM(GmILimAltSf,        float);     // Grid monitor current limit altitude scale factor
CREATE_PARM(LinILimAltSf,       float);     // Line current limit altitude scale factor
CREATE_PARM(RtrILimAltSf,       float);     // Rotor current limit altitude scale factor
CREATE_PARM(StrILimAltSf,       float);     // Stator current limit altitude scale factor
CREATE_PARM(GmILimAltLpse,      float);     // Grid monitor current limit altitude factor
CREATE_PARM(LinILimAltLpse,     float);     // Line current limit altitude factor
CREATE_PARM(RtrILimAltLpse,     float);     // Rotor current limit altitude factor
CREATE_PARM(StrILimAltLpse,     float);     // Stator current limit altitude factor
CREATE_PARM(GmILimTempMax,      float);     // Grid monitor current limit temperature maximum
CREATE_PARM(GmILimTempMin,      float);     // Grid monitor current limit temperature minimum
CREATE_PARM(LinILimTempMax,     float);     // Line current limit temperature maximum
CREATE_PARM(LinILimTempMin,     float);     // Line current limit temperature minimum
CREATE_PARM(RtrILimTempMax,     float);     // Rotor current limit temperature maximum
CREATE_PARM(RtrILimTempMin,     float);     // Rotor current limit temperature minimum
CREATE_PARM(StrILimTempMax,     float);     // Stator current limit temperature maximum
CREATE_PARM(StrILimTempMin,     float);     // Stator current limit temperature minimum
CREATE_PARM(GmILimDstCmp,       float);     // Grid monitor current limit distortion compensation
CREATE_PARM(LinILimDstCmp,      float);     // Line current limit distortion compensation
CREATE_PARM(RtrILimDstCmp,      float);     // Rotor current limit distortion compensation
CREATE_PARM(StrILimDstCmp,      float);     // Stator current limit distortion compensation
CREATE_PARM(GmILimAltDeRtK1,    float);     // Grid monitor current limit altitude de-rating curve K1
CREATE_PARM(GmILimAltDeRtX1,    float);     // Grid monitor current limit altitude de-rating curve X1
CREATE_PARM(GmILimAltDeRtX2,    float);     // Grid monitor current limit altitude de-rating curve X2
CREATE_PARM(AclfuILimAltDeRtK1, float);     // ACLFU current limit altitude de-rating curve K1
CREATE_PARM(AclfuILimAltDeRtX1, float);     // ACLFU current limit altitude de-rating curve X1
CREATE_PARM(AclfuILimAltDeRtX2, float);     // ACLFU current limit altitude de-rating curve X2
CREATE_PARM(LinILimAltDeRtK1,   float);     // Line current limit altitude de-rating curve K1
CREATE_PARM(LinILimAltDeRtX1,   float);     // Line current limit altitude de-rating curve X1
CREATE_PARM(LinILimAltDeRtX2,   float);     // Line current limit altitude de-rating curve X2
CREATE_PARM(RtrILimAltDeRtK1,   float);     // Rotor current limit altitude de-rating curve K1
CREATE_PARM(RtrILimAltDeRtX1,   float);     // Rotor current limit altitude de-rating curve X1
CREATE_PARM(RtrILimAltDeRtX2,   float);     // Rotor current limit altitude de-rating curve X2
CREATE_PARM(StrILimAltDeRtK1,   float);     // Stator current limit altitude de-rating curve K1
CREATE_PARM(StrILimAltDeRtX1,   float);     // Stator current limit altitude de-rating curve X1
CREATE_PARM(StrILimAltDeRtX2,   float);     // Stator current limit altitude de-rating curve X2
// ACLFU current limit
CREATE_PARM(AclfuILimDeRtK1,    float);     // ACLFU current limit de-rating curve K1
CREATE_PARM(AclfuILimDeRtX1,    float);     // ACLFU current limit de-rating curve X1
CREATE_PARM(AclfuILimDeRtX2,    float);     // ACLFU current limit de-rating curve X2
CREATE_PARM(AclfuILimDeRtX3,    float);     // ACLFU current limit de-rating curve X3
CREATE_PARM(AclfuILimAltSf,     float);     // ACLFU current limit altitude scale factor
CREATE_PARM(AclfuILimAltLpse,   float);     // ACLFU current limit altitude factor
CREATE_PARM(AclfuILimTempMax,   float);     // ACLFU current limit temperature maximum
CREATE_PARM(AclfuILimTempMin,   float);     // ACLFU current limit temperature minimum
CREATE_PARM(AclfuILimDstCmp,    float);     // ACLFU current limit distortion compensation

CREATE_PARM(GenILimTempMax,     float);     // Generator current limit temperature maximum
CREATE_PARM(GenILimTempMin,     float);     // Generator current limit temperature minimum

CREATE_PARM(R_ImaxCrvGenMax,     float);     // Generator rotor current max limit
CREATE_PARM(S_ImaxCrvGenMax,     float);     // Generator stator current max limit

// line converter (bridge, shut, line inductor)
CREATE_PARM(WBLineIFnd1,          float);          // Line converter fundamental current limit associated with WBRtrITemp1   (Amp)
CREATE_PARM(WBLineIFnd2,          float);          // Line converter fundamental current limit associated with WBRtrITemp2   (Amp)

// Aclfu
CREATE_PARM(WBAclfuIFnd1,         float);          // ACLFU fundamental current limit associated with WBRtrITemp1   (Amp)
CREATE_PARM(WBAclfuIFnd2,         float);          // ACLFU fundamental current limit associated with WBRtrITemp2   (Amp)

CREATE_PARM(WBTocTauNorm,         float);          // Normal TOC time constant
CREATE_PARM(R_ISpdAlttdK,         float);          // generator rotor cable current limit for temperature 1 at 1,000 meters  (Amp)

// Generator rotor cable
CREATE_PARM(R_ILimA1Cab1,         float);          // generator rotor cable current limit for temperature 1 at 1,000 meters  (Amp)
CREATE_PARM(R_ILimA1Cab2,         float);          // generator rotor cable current limit for temperature 2 at 1,000 meters  (Amp)
CREATE_PARM(R_ILimCab0,           float);          // generator rotor cable current limit for temperature 0 after derating   (Amp)
CREATE_PARM(R_ILimCab1,           float);          // generator rotor cable current limit for temperature 1 after derating   (Amp)
CREATE_PARM(R_ILimCab2,           float);          // generator rotor cable current limit for temperature 2 after derating   (Amp)
CREATE_PARM(R_TempCabDel,         float);          // generator rotor cable current temperature delta from down-tower ambient (deg C)
CREATE_PARM(R_TempCable1,         float);          // rotor and cable temperature limit 1
CREATE_PARM(R_TempCable2,         float);          // rotor and cable temperature limit 2

CREATE_PARM(R_ILimCab1Ov,         float);
CREATE_PARM(R_ILimCab2Ov,         float);
CREATE_PARM(R_TempCab1Ov,         float);
CREATE_PARM(R_TempCab2Ov,         float);
CREATE_PARM(S_ILimCab1Ov,         float);
CREATE_PARM(S_ILimCab2Ov,         float);
CREATE_PARM(S_TempCab1Ov,         float);
CREATE_PARM(S_TempCab2Ov,         float);




// Generator stator cable
CREATE_PARM(S_ILimA1Cab1,         float);          // generator stator cable current limit for temperature 1 at 1,000 meters  (Amp)
CREATE_PARM(S_ILimA1Cab2,         float);          // generator stator cable current limit for temperature 2 at 1,000 meters  (Amp)
CREATE_PARM(S_ILimCab0,           float);          // generator stator cable current limit for temperature 0 after derating   (Amp)
CREATE_PARM(S_ILimCab1,           float);          // generator stator cable current limit for temperature 1 after derating   (Amp)
CREATE_PARM(S_ILimCab2,           float);          // generator stator cable current limit for temperature 2 after derating   (Amp)
CREATE_PARM(S_TempCabDel,         float);          // generator stator cable current temperature delta from down-tower ambient (deg C)
CREATE_PARM(S_TempCable1,         float);          // stator and cable temperature limit 1
CREATE_PARM(S_TempCable2,         float);          // stator and cable temperature limit 2

//------------------------------------------------------------------------------
// current limit @ altitude
// 690 GM
CREATE_PARM(WBGmIFnd1,            float);          // 690 GM fundamental current limit associated with WBRtrITemp1   (Amp)
CREATE_PARM(WBGmIFnd2,            float);          // 690 GM fundamental current limit associated with WBRtrITemp2   (Amp)
// generator rotor                                                                                                                       (Amp)
CREATE_PARM(WBRtrIFnd1,           float);          // Turbine power limiter Gen rotor fundamental current limit associated iwth WBRtrITemp1      (Amp)
CREATE_PARM(WBRtrIFnd2,           float);          // Turbine power limiter Gen rotor fundamental current limit associated iwth WBRtrITemp2      (Amp)
CREATE_PARM(WBRtrILim1,           float);          // Turbine power limiter Gen rotor current point 1 maximum current limit altitude dependent  (Amp)
CREATE_PARM(WBRtrILim2,           float);          // Turbine power limiter Gen rotor current point 2 maximum current limitaltitude dependent                   (Amp)
CREATE_PARM(WBRtrITemp1,          float);          // Turbine power limiter Gen rotor temperature for dynamic current calculation corresponding to Ilim1  (degC)
CREATE_PARM(WBRtrITemp2,          float);          // Turbine power limiter Gen rotor temperature for dynamic current calculation corresponding to Ilim2  (degC)
// generator rotor                                                                                                                       (Amp)
CREATE_PARM(WBStrIFnd1,           float);          // Turbine power limiter Gen stator fundamental current limit associated iwth WBRtrITemp1      (Amp)
CREATE_PARM(WBStrIFnd2,           float);          // Turbine power limiter Gen stator fundamental current limit associated iwth WBRtrITemp2      (Amp)
CREATE_PARM(WBStrILim1,           float);          // Turbine power limiter Gen stator current point 1 maximum current limit altitude dependent  (Amp)
CREATE_PARM(WBStrILim2,           float);          // Turbine power limiter Gen stator current point 2 maximum current limitaltitude dependent                   (Amp)
CREATE_PARM(WBStrITemp1,          float);          // Turbine power limiter Gen stator temperature for dynamic current calculation corresponding to Ilim1  (degC)
CREATE_PARM(WBStrITemp2,          float);          // Turbine power limiter Gen stator temperature for dynamic current calculation corresponding to Ilim2  (degC)


CREATE_PARM(WBGmILim1      ,   float);    // Grid monitor point 1 and minimum current limit for dynamic calculation
CREATE_PARM(WBGmILim2      ,   float);    // Grid monitor point 2 and maximum current limit for dynamic calculation
CREATE_PARM(WBGmITemp1     ,   float);    // Grid monitor temperature for dynamic current calculation corresponding to Ilim1
CREATE_PARM(WBGmITemp2     ,   float);    // Grid monitor temperature for dynamic current calculation corresponding to Ilim2
CREATE_PARM(GmIMaxTau      ,   float);    // Tau for grid monitor i max
CREATE_PARM(WBGmIMaxLim    ,   float);    // Grid monitor max current limit (Amp)
CREATE_PARM(WBLineILim1    ,   float);    // Line converter point 1 and minimum current limit for dynamic calculation
CREATE_PARM(WBLineILim2    ,   float);    // Line converter point 2 and maximum current limit for dynamic calculation
CREATE_PARM(WBLineITemp1   ,   float);    // Line converter temperature for dynamic current calculation corresponding to Ilim1
CREATE_PARM(WBLineITemp2   ,   float);    // Line converter temperature for dynamic current calculation corresponding to Ilim2
CREATE_PARM(L_IMaxTau      ,   float);    // Tau for Line converter i max
CREATE_PARM(WBLnIMaxLim    ,   float);    // Line converter max current limit (Amp)

CREATE_PARM(WBAclfuILim1   ,   float);    // ACLFU point 1 and minimum current limit for dynamic calculation
CREATE_PARM(WBAclfuILim2   ,   float);    // ACLFU point 2 and maximum current limit for dynamic calculation
CREATE_PARM(WBAclfuITemp1  ,   float);    // ACLFU temperature for dynamic current calculation corresponding to Ilim1
CREATE_PARM(WBAclfuITemp2  ,   float);    // ACLFU temperature for dynamic current calculation corresponding to Ilim2
CREATE_PARM(L_FuseIMaxTau  ,   float);    // Tau for ACLFU i max
CREATE_PARM(WBAclfuIMaxLim ,   float);    // ACLFU max current limit (Amp)



//Turbine Power Limiter parms
CREATE_PARM( WBSpeed,        float);      // Turbine power limiter speed
CREATE_PARM( DelSpdPrpGn,    float);      // Turbine power speed control limiter proportional gain
CREATE_PARM( DelSpdIntGn,    float);      // Turbine power speed control limiter  integral gain
CREATE_PARM( DelTrqPrpGn,    float);      // Turbine power torque control limiter proportional gain
CREATE_PARM( DelTrqIntGn,    float);      // Turbine power torque control limiter  integral gain
CREATE_PARM( DelSpdIntLm,    float);      // Turbine power limiter speed control integral lower limit
CREATE_PARM( DelTrqIntLm,    float);      // Turbine power limiter speed control integral lower limit
CREATE_PARM( RtrVLimW,       float);      // Turbine power limiter rotor voltage limit omega
CREATE_PARM( RtrVLimGn,      float);      // Turbine power limiter rotor voltage limit gain
CREATE_PARM( RtrVSubSPwr,    float);      // Turbine power limiter rotor voltage sub-sync power output
CREATE_PARM( LnILimGn,       float);      // Turbine power limiter line current limit gain.
CREATE_PARM( RtrILimGn,      float);      // Turbine power limiter rotor current limit gain.
CREATE_PARM( StrILimGn,      float);      // Turbine power limiter stator current limit gain.
CREATE_PARM( GmILimGn,       float);      // Turbine power limiter grid monitor current limit gain.
CREATE_PARM( RtrVLim,        float);
CREATE_PARM( DelSpdTrqAct,   unsigned);   // delspd deltrq power limiter active
CREATE_PARM( DelSpdClamp,    float);      // Delta Speed clamp (RPM)
CREATE_PARM( DelTrqClamp,    float);      // Delta Torque clamp (KNm)

CREATE_PARM( TempDerate,        float);   // Turbine power limiter down-tower temp derate gain
CREATE_PARM( DnTwrTempLimGn,    float);   // Turbine power limiter down-tower temp limit gain
CREATE_PARM( DnTwrTempW,        float);   // Turbine power limiter down-tower ambient temp filter omega
CREATE_PARM( DnTwrTmpRt,        float);   // Turbine power limiter down-tower Temp derate slow threshold
CREATE_PARM( DnTwrTmpMx,        float);   // Turbine power limiter down-tower Temp derate fast threshold
CREATE_PARM( DnTwrTmpZr,        float);   // Turbine power limiter down-tower Temp derate zero threshold
CREATE_PARM( DnTPwrPuMx,        float);   // Turbine power limiter down-tower Temp derate power level for P.DnTwrTmpMx point
CREATE_PARM( WBStrSpilMrg,      float);   // Turbine power limiter stator spill margin
CREATE_PARM( WBStrSpilGn,       float);   // Turbine power limiter stator spill gain
CREATE_PARM( WBLinSpilMrg,      float);   // Turbine power limiter line spill margin

CREATE_PARM(RSinkTempRseLim  ,  float);   // Rotor heater sink temperature rise limit (degC)
CREATE_PARM(LinSinkTempRseLim,  float);   // Line heater sink temperature rise limit (degC)

CREATE_PARM(CfcTempLimGn,         float);
CREATE_PARM(PdcTempLimGn,         float);
CREATE_PARM(LqdTempLimGn,         float);
CREATE_PARM(PdcCabEntryTempLimGn, float);
CREATE_PARM(AclfuTempLimGn,       float);
CREATE_PARM(CfcTempLim,           float);   // Cfc temperature limit for turbine power limiter path
CREATE_PARM(PdcTempLim,           float);   // Pdc temperature limit for turbine power limiter path
CREATE_PARM(LqdTempLim,           float);   // LQD temperature limit for turbine power limiter path
CREATE_PARM(PdcCabEntryTempLim,   float);     // K1 Bus temperature limit for turbine power limiter path
CREATE_PARM(AclfuTempLim,         float);   // ACLFU temperature limit for turbine power limiter path
CREATE_PARM(RSinkTempRseLimGn  ,  float);
CREATE_PARM(L_FuseILimGn       ,  float);
CREATE_PARM(LinSinkTempRseLimGn,  float);

CREATE_PARM(RImbTempLim,          float);
CREATE_PARM(RImbTempLimGn,        float);
CREATE_PARM(LinImbTempLim,        float);
CREATE_PARM(LinImbTempLimGn,      float);
CREATE_PARM(SinkTempDeRtW,        float);

CREATE_PARM(AirTempGenW,          float);     // Air temp at generator inlet omega for lp filter
CREATE_PARM(AirTempDlW,           float);     // Air temp at drip loop omega for lp filter
CREATE_PARM(DelSpdTrqScl,         float);     // Delta-speed to delta-torque scale factor


// TOC
CREATE_PARM  (GmIxWIyLim,           float);
CREATE_PARM  (GmIRatSSLim,          float);
CREATE_PARM  (Q1Irat,               float);
CREATE_PARM  (GmCsaIrat,            float);

CREATE_PARM  (GmICnvNameP,          float);
CREATE_PARM  (L_ICnvNameP,          float);
CREATE_PARM  (R_ICnvNameP,          float);
CREATE_PARM  (S_ICnvNameP,          float);


CREATE_PARM  (L_IDistTOC,           float);    // Line converter current detectable by toc model  (Amps)
CREATE_PARM  (L_AclfuIDistTOC,      float);    // AClFU current detectable by toc model  (Amps)
CREATE_PARM  (R_IDistTOC,           float);    // Rotor converter current detectable by toc model  (Amps)
CREATE_PARM  (S_IDistTOC,           float);    // Stator current detectable by toc model  (Amps)
CREATE_PARM  (GmIDistTOC,           float);    // Grid monitor current detectable by toc model  (Amps)

CREATE_PARM  (TocTauTstScl,         float);    // Speed up factor for TOC tau's, primarily for lab use

CREATE_PARM(R_OVTrpThrs,        float);     // rotor over-voltage trip threshold
CREATE_PARM(R_ProtClass,        float);     // Motor Protection Class
CREATE_PARM(R_MaxCurAdj,        float);     // Adjusted rotor max current
CREATE_PARM(S_MaxCurAdj,        float);     // Adjusted stator max current
CREATE_PARM(R_IMaxTau,          float);     // Tau for rotor i max
CREATE_PARM(S_IMaxTau,          float);     // Tau for stator i max



// Public variables
//-----------------
CREATE_PUBVAR(GmILimCnv1km,     float);     // Grid monitor current limit at 1km altitude
CREATE_PUBVAR(GmILimTemp,       float);     // Grid monitor current limit temperature reference
CREATE_PUBVAR(LinILimCnv1km,    float);     // Line current limit at 1km altitude
CREATE_PUBVAR(LinILimTemp,      float);     // Line current limit temperature reference
CREATE_PUBVAR(RtrILimCnv1km,    float);     // Rotor current limit at 1km altitude
CREATE_PUBVAR(RtrILimTemp,      float);     // Rotor current limit temperature reference
CREATE_PUBVAR(StrILimCnv1km,    float);     // Stator current limit at 1km altitude
CREATE_PUBVAR(StrILimTemp,      float);     // Stator current limit temperature reference
// ACLFU current limit
CREATE_PUBVAR(AclfuILim1km,     float);     // ACLFU current limit at 1km altitude
CREATE_PUBVAR(AclfuILimTemp,    float);     // ACLFU current limit temperature reference

CREATE_PUBVAR(WBTocTauRcp,          float);

CREATE_PUBVAR(GmTocHiSelT2,         float);
CREATE_PUBVAR(GmTocThrDyT2,         float);
CREATE_PUBVAR(DfTocHiSelT2,         float);
CREATE_PUBVAR(L_FuseTocHiSelT2,     float);
CREATE_PUBVAR(L_FuseTocThrDyT2,     float);
CREATE_PUBVAR(L_TocHiSelT2,         float);
CREATE_PUBVAR(L_TocThrDyT2,         float);
CREATE_PUBVAR(R_TocHiSelT2,         float);
CREATE_PUBVAR(R_TocThrDyT2,         float);
CREATE_PUBVAR(S_TocHiSelT2,         float);
CREATE_PUBVAR(S_TocThrDyT2,         float);


CREATE_PUBVAR(R_IMaxCrvCab,     float );
CREATE_PUBVAR(R_IMaxCrvGen,     float );
CREATE_PUBVAR(R_IMaxCrvMin,     float );
CREATE_PUBVAR(R_IMaxCrvSq,      float );
CREATE_PUBVAR(R_IMaxGn,         float );
CREATE_PUBVAR(R_IMaxFstGn,      float );
CREATE_PUBVAR(R_IMaxGnDyn,      float );
CREATE_PUBVAR(R_MaxAmps,        float );                // Rotor maximum amps
CREATE_PUBVAR(R_MaxCurOut,      float );
CREATE_PUBVAR(R_MxCurOutSq,     float );

CREATE_PUBVAR(S_IMaxCrvCab,     float );
CREATE_PUBVAR(S_IMaxCrvGen,     float );
CREATE_PUBVAR(S_IMaxCrvMin,     float );
CREATE_PUBVAR(S_IMaxCrvSq,      float );
CREATE_PUBVAR(S_IMaxGn,         float );
CREATE_PUBVAR(S_IMaxFstGn,      float );
CREATE_PUBVAR(S_IMaxGnDyn,      float );
CREATE_PUBVAR(S_MaxCurOut,      float );
CREATE_PUBVAR(S_MxCurOutSq,     float );

CREATE_PUBVAR(R_IlimSlopeC ,    float );
CREATE_PUBVAR(R_MaxCurSqFrc,    float );
CREATE_PUBVAR(S_IlimSlopeC ,    float );

CREATE_PUBVAR(S_MaxCurSqFrc,    float );
CREATE_PUBVAR(SpdILim,          float );
CREATE_PUBVAR(R_MaxCurSqInt,    int   );
CREATE_PUBVAR(S_MaxCurSqInt,    int   );
// Generator rotor converter current limiter
CREATE_PUBVAR(WBRtrILimCnv,     float );            // Generator rotor current limit for converter as a function of termperature
CREATE_PUBVAR(WBRtrDynSlp,      float );            // Generator rotor current limit curve slope
// Generator stator converter current limiter
CREATE_PUBVAR(WBStrILimCnv,     float );            // Generator stator current limit for converter as a function of termperature
CREATE_PUBVAR(WBStrDynSlp,      float );            // Generator stator current limit curve slope
// Sync Sw current limiter

CREATE_PUBVAR(R_IMaxLimSq,     float );
CREATE_PUBVAR(S_IMaxLimSq,     float );

CREATE_PUBVAR(L_BpTemp1T2,     float);  // Line   Bp RTDs
CREATE_PUBVAR(L_BpTemp2T2,     float);  //
CREATE_PUBVAR(L_BpTemp3T2,     float);  //
CREATE_PUBVAR(L_BpTemp4T2,     float);  //
CREATE_PUBVAR(L_BpTemp5T2,     float);  //
CREATE_PUBVAR(L_BpTemp6T2,     float);  //

CREATE_PUBVAR(R_BpTemp1T2,    float);  // Rotor1 Bp RTDs
CREATE_PUBVAR(R_BpTemp2T2,    float);  //
CREATE_PUBVAR(R_BpTemp3T2,    float);  //
CREATE_PUBVAR(R_BpTemp4T2,    float);  //
CREATE_PUBVAR(R_BpTemp5T2,    float);  //
CREATE_PUBVAR(R_BpTemp6T2,    float);  //
CREATE_PUBVAR(R_BpTemp7T2,    float);  //
CREATE_PUBVAR(R_BpTemp8T2,    float);  //



CREATE_PUBVAR(S_TempCabVar        , float);    // generator rotor cable temperate set point (deg C)
CREATE_PUBVAR(R_TempCabVar        , float);    // generator rotor cable temperate set point (deg C)

// vars for the dynamic Gm TOC thresholds calcualtion
CREATE_PUBVAR(GmMaxCurOut ,    float);  // 690 Gm max current (Amp)
CREATE_PUBVAR(GmILimCnv   ,    float);
CREATE_PUBVAR(GmIMaxCrvSq ,    float);
CREATE_PUBVAR(GmIMaxGn    ,    float);
CREATE_PUBVAR(GmIMaxFstGn ,    float);
CREATE_PUBVAR(GmIMaxGnDyn ,    float);
CREATE_PUBVAR(GmMxCurOutSq,    float);
CREATE_PUBVAR(GmMaxCurSqFrc,   float);
CREATE_PUBVAR(WBGmDynSlp,      float);
CREATE_PUBVAR(GmMaxCurSqInt,   int);
CREATE_PUBVAR(GmIMaxCrvMin,    float );

// vars for the dynamic Line Converter TOC thresholds calcualtion
CREATE_PUBVAR(L_MaxCurOut ,    float);  // Line converter max current (Amp)
CREATE_PUBVAR(L_ILimCnv   ,    float);
CREATE_PUBVAR(L_IMaxCrvSq ,    float);
CREATE_PUBVAR(L_IMaxGn    ,    float);
CREATE_PUBVAR(L_IMaxFstGn ,    float);
CREATE_PUBVAR(L_IMaxGnDyn ,    float);
CREATE_PUBVAR(L_MxCurOutSq,    float);
CREATE_PUBVAR(L_MaxCurSqFrc,   float);
CREATE_PUBVAR(WBLinDynSlp,     float);
CREATE_PUBVAR(L_MaxCurSqInt,   int);

// vars for the dynamic ACLFU TOC thresholds calcualtion
CREATE_PUBVAR(L_FuseMaxCurOut ,float);  // ACLFU max current (Amp)
CREATE_PUBVAR(L_FuseILimCnv   ,float);
CREATE_PUBVAR(L_FuseIMaxCrvSq ,    float);
CREATE_PUBVAR(L_FuseIMaxGn    ,    float);
CREATE_PUBVAR(L_FuseIMaxFstGn ,    float);
CREATE_PUBVAR(L_FuseIMaxGnDyn ,    float);
CREATE_PUBVAR(L_FuseMxCurOutSq,    float);
CREATE_PUBVAR(L_FuseMaxCurSqFrc,   float);
CREATE_PUBVAR(WBAclfuDynSlp,       float);
CREATE_PUBVAR(L_FuseMaxCurSqInt,   int);


CREATE_PUBVAR(RotorVMgFil,     float);
CREATE_PUBVAR(DelSpdErr ,      float);
CREATE_PUBVAR(DelSpdErrTrq,    float);      // DelSpdErr, scaled to torque for sub-sync operation
CREATE_PUBVAR(DelSpdInt ,      float);
CREATE_PUBVAR(DelSpdPrp ,      float);
CREATE_PUBVAR(DelSpdIntDt,     float);

CREATE_PUBVAR(DelTrqErr ,      float);
CREATE_PUBVAR(DelTrqInt ,      float);
CREATE_PUBVAR(DelTrqPrp ,      float);
CREATE_PUBVAR(DelTrqIntDt,     float);

CREATE_PUBVAR(DnTwrPLimF,      float);
CREATE_PUBVAR(DnTwrPLimS,      float);
CREATE_PUBVAR(DnTwrTemp,       float);    //Down tower temperature (degC)
CREATE_PUBVAR(DnTwrTempGn,     float);
CREATE_PUBVAR(RImbTempGn,      float);
CREATE_PUBVAR(DnTwrTMrg,       float);    //Down tower temperature limit margin (Amp)
CREATE_PUBVAR(GmILimMrg,       float);    //690 GM current limit margin  (Amp)
CREATE_PUBVAR(GmILimOut,       float);    //690 GM current limit margin (Amp)
CREATE_PUBVAR(GmIMrgSqr,       float);    //690 GM current limit squared (Amp)
CREATE_PUBVAR(LinILimMrg,      float);    //Line converter current limit margin squared (Amp)
CREATE_PUBVAR(LinILimOut,      float);    //Line converter current limit margin (Amp)
CREATE_PUBVAR(LinIMrgSqr,      float);    //Line converter current limit  (Amp)
CREATE_PUBVAR(DelTrqTempILimOut,float);   //Delta torque overall minimum temperature for turbine power limiter (degC)
CREATE_PUBVAR(RtrILim,         float);    //Generator rotor current limit peak(Amp)
CREATE_PUBVAR(RtrILimMrg,      float);    //Generator rotor current limit margin(Amp)
CREATE_PUBVAR(RtrILimOut,      float);    //Generator rotor current limit (Amp)
CREATE_PUBVAR(RtrIMrgSqr,      float);    //Rotor voltage limit margin squared (Amp)
CREATE_PUBVAR(RtrVLimMrg,      float);    //Rotor voltage limit margin (Amp)
CREATE_PUBVAR(RtrVLimOut,      float);    //Rotor voltage limit  (Amp)
CREATE_PUBVAR(RtrVMrgSqr,      float);    //Rotor voltage limit squared (Amp)
CREATE_PUBVAR(RtrVSqrFil,      float);
CREATE_PUBVAR(StrILim,         float);    //Stator current limit peak (Amp)
CREATE_PUBVAR(StrILimMrg,      float);    //Stator current limit margin (Amp)

CREATE_PUBVAR(StrILimOut,            float);     //Stator current limit  (Amp)
CREATE_PUBVAR(StrIMrgSqr,            float);
CREATE_PUBVAR(LinImbTempLimOut,      float);     //Rotor 2 inductor temperature limit (DegC)
CREATE_PUBVAR(R1ImbTempLimOut,       float);     //Rotor 1 heat sink temperature rise limit (DegC)
CREATE_PUBVAR(LinImbTempLimMrg,      float);     //line heat sink temperature rise limit (DegC)
CREATE_PUBVAR(R1ImbTempLimMrg,       float);     //Rotor 1 heat sink imblance temperature limit margin (DegC)
CREATE_PUBVAR(R1TempRseLimOut,       float);     //Rotor 1 heat sink temperature rise limit (DegC)
CREATE_PUBVAR(R1TempRseLimMrg,       float);     //Rotor 1 heat sink temperature rise limit margin(DegC)
CREATE_PUBVAR(L_FuseILimMrg,         float);     //ACLFU current limit margin (Amp)
CREATE_PUBVAR(L_FuseILimOut,         float);     //ACLFU current limit  (Amp)
CREATE_PUBVAR(CfcTempLimMrg ,        float);     //Cfc temperature limit margin (DegC)
CREATE_PUBVAR(CfcTempLimOut ,        float);     //Cfc temperature limit (DegC)
CREATE_PUBVAR(LqdTempLimMrg,         float);     //Lqd temperature limit margin (DegC)
CREATE_PUBVAR(LqdTempLimOut ,        float);     //Lqd temperature limit (DegC)
CREATE_PUBVAR(PdcCabEnTempLimMrg,    float);     //Pdc cable entry temperature limit margin (DegC)
CREATE_PUBVAR(PdcCabEnTempLimOut,    float);     //Pdc cable entry temperature limit  (DegC)

CREATE_PUBVAR(AclfuTempLimMrg,  float);     //ACLFU temperature limit margin (DegC)
CREATE_PUBVAR(AclfuTempLimOut,  float);     //ACLFU temperature limit  (DegC)

CREATE_PUBVAR(PdcTempLimMrg,    float);     //Pdc temperature limit margin (DegC)
CREATE_PUBVAR(PdcTempLimOut,    float);     //Pdc temperature limit  (DegC)

CREATE_PUBVAR(R1TempRseFil    , float);     // Rotor 1 heat sink temperature sink temp rise filted (degC)
CREATE_PUBVAR(LinTempRseLimMrg, float);     //Line heat sink temperature rise limit  (DegC)
CREATE_PUBVAR(LinTempRseLimOut, float);     //Line heat sink temperature rise limit margin (DegC)

CREATE_PUBVAR(L_HtSinkTempILimOut, float);  // Line current limit from heat sink temperature (Amp)

CREATE_PUBVAR(WBLimAct,        unsigned) = false;

CREATE_PUBVAR(GmTempCabVar   , float);            // 690 Gm cable temperate set point (deg C)

CREATE_PUBVAR(PwrLimDelSpeed,         float);  // Delta speed for turbine power limiter (Rpm)
CREATE_PUBVAR(PwrLimDelTorque,        float);  // Delta torque for turbine power limiter (Nm)
CREATE_PUBVAR(PwrLimDelTorque_KNm ,    float);  // Delta torque for turbine power limiter (KNm)


CREATE_PUBVAR(L_SinkTempRseFil,   float);  // line heat sink temperature rise filtered (degC)
CREATE_PUBVAR(L_SinkTempRse,      float);  // line heat sink temperature rise  (degC)
CREATE_PUBVAR(L_SinkTempRseGn,    float);
CREATE_PUBVAR(R1SinkTempRse,      float);  // rotor 1 heat sink temperature rise  (degC)
CREATE_PUBVAR(R1SinkTempRseFil,   float);  // rotor 1 heat sink temperature rise filtered (degC)
CREATE_PUBVAR(LinImbTempFil,      float);  // line heat sink imbalance temperature filted (degC)
CREATE_PUBVAR(R1ImbTempFil ,      float);  // rotor 1 heat sink imbalance temperature filted (degC)
CREATE_PUBVAR(LinBthtrImbMax,     float);
CREATE_PUBVAR(R1BthtrImbMax,      float);  // rotor 1 heat sink max imbalance temperature (degC)
CREATE_PUBVAR(LinImbTempGn,       float);  // line heat sink max imbalance temperature (degC)
CREATE_PUBVAR(RSinkTempRseGn     ,float);
CREATE_PUBVAR(DnTwrTempLmOut,     float);  // Down tower temperature limit (degC)
CREATE_PUBVAR(DnTwrTempLmOutPu,   float);  // Down tower temperature limit in pu (degC)
// Turbine Power Limiter Vars

CREATE_PUBVAR(AirTempGenInlet, float);    // Air inlet temp at generator inlet
CREATE_PUBVAR(AirTempGenInFil, float);    // Air inlet temp at generator inlet, filtered
CREATE_PUBVAR(AirTempGenIn,    float);    // Air inlet temp at generator inlet used for power limiter
CREATE_PUBVAR(AirTempGenClmp,  float);    // Air inlet temp at generator inlet used for power limiter--clamped
CREATE_PUBVAR(AirTempGnSensAct,unsigned); // Air temp for generator Up tower being used
CREATE_PUBVAR(AirTempGnSensOk, unsigned); // Air inlet temp sensor at geneator inlet is ok
CREATE_PUBVAR(AirTempDripLoop, float);    // Air inlet temp at drip loop
CREATE_PUBVAR(AirTempDlFil,    float);    // Air inlet temp at drip loop of cable, filtered
CREATE_PUBVAR(AirTempGenCab,   float);    // Air temperature used for generator cable limiters.
CREATE_PUBVAR(AirTempDlSensAct,unsigned); // Air temp for generator drip loop Up tower drip loop temperature being used
CREATE_PUBVAR(AirTempDlSensOk, unsigned); // Air inlet temp sensor at drip loop is ok
CREATE_PUBVAR(AirTempGenGn,    float);    // Air temp at generator inlet gain for lp filter
CREATE_PUBVAR(AirTempDlGn,     float);    // Air temp at drip loop gain for lp filter

CREATE_PUBVAR(AltitudeClmp,    float);          // Clamped turbine altitude (meters)
CREATE_PUBVAR(AltitudeClmpCab, float);          // Clamped turbine altitude for cables (meters)
CREATE_PUBVAR(RtrVLimGn,       float);
CREATE_PUBVAR(RtrVLimSqr,      float);
CREATE_PUBVAR(CableLimType,    unsigned);
CREATE_PUBVAR(R_RpmSlipNom,    float);

CREATE_PUBVAR(PwrLimStrCurSel,  int);   // Indicate which channel is most negative in stator current power limiter
CREATE_PUBVAR(PwrLimRtrCurSel,  int);   // Indicate which channel is most negative in rotor current power limiter

CREATE_PUBVAR(FastTauSel,       unsigned);       // flag to indicate fast TOC tau
// delta-speed to delta-torque transition
CREATE_PARM(PwrLimSlpMrg,       float);     // Slip margin for power limiter speed control
CREATE_PARM(PwrLimSyncSpd,      float);     // Synchronized speed for power limiter speed control
CREATE_PARM(PwrLimSpdRatMax,    float);     // Max rated speed for power limiter speed control
CREATE_PARM(PwrLimSpdRatMin,    float);     // Min rated speed for power limiter speed control
CREATE_PARM(PwrLimSpdRatLcl,    float);     // Rated speed local set value for power limiter speed control
CREATE_PUBVAR(PwrLimSpdRatRem,  float);     // Rated speed remote set value for power limiter speed control
CREATE_PUBVAR(PwrLimTrqRatRem,  float);     // Rated torque remote set value for power limiter speed control
CREATE_PARM(PwrLimSpdRatTc,     float);     // Rated speed for power limiter speed control
CREATE_PARM(PwrLimDelSpdMin,    float);     // Min de-rating speed for power limiter speed control
CREATE_PUBVAR(PwrLimDelSpdUc,   float);     // Un-clamped de-rating speed for power limiter speed control
CREATE_DIAG(TcSpdRatInvalid);               // Trip indicates invalid rated speed for power limiter speed control

CREATE_PUBVAR(PwrLimDerateId,   unsigned);  // Power limit ID
CREATE_PUBVAR(PwrLimDelTrqMin,  float);     // Power limit delta torque request from speed channel
CREATE_PUBVAR(PwrLimDelSpdMin,  float);     //  Power limit min delta speed request

// power limit active detection
CREATE_PUBVAR(PwrLimLnCnvCurFlg,    unsigned);  // Power limit line converter current active flag
CREATE_PUBVAR(PwmLimFuseCurFlg,     unsigned);  // Power limit line fuse current active flag
CREATE_PUBVAR(PwmLimFuseTempFlg,    unsigned);  // Power limit line fuse temperature active flag
CREATE_PUBVAR(PwmLimRtrVltFlg,      unsigned);  // Power limit rotor converter voltage active flag
CREATE_PUBVAR(PwmLimLnBpImbFlg,     unsigned);  // Power limit line baseplate temperature imbalance active flag
CREATE_PUBVAR(PwmLimLnBpRiseFlg,    unsigned);  // Power limit line baseplate temperature rise active flag

CREATE_PUBVAR(PwrLimLnCnvCurHiTmr,  float);     // Power limit line converter current indicator timer
CREATE_PUBVAR(PwrLimFuseCurHiTmr,   float);     // Power limit line fuse current indicator timer
CREATE_PUBVAR(PwrLimFuseTempHiTmr,  float);     // Power limit line fuse temperature aindicator timer
CREATE_PUBVAR(PwrLimRtrVltHiTmr,    float);     // Power limit rotor converter voltage indicator timer
CREATE_PUBVAR(PwrLimLnBpImbHiTmr,   float);     // Power limit line baseplate temperature imbalance indicator timer
CREATE_PUBVAR(PwrLimLnBpRiseHiTmr,  float);     // Power limit line baseplate temperature rise indicator timer

CREATE_PUBVAR(PwmLimGmCurFlg,       unsigned);  // Power limit grid monitor current active flag
CREATE_PUBVAR(PwmLimRtrCurFlg,      unsigned);  // Power limit rotor current active flag
CREATE_PUBVAR(PwmLimStrCurFlg,      unsigned);  // Power limit stator current active flag
CREATE_PUBVAR(PwmLimRtrBpRiseFlg,   unsigned);  // Power limit rotor baseplate temperautre rise active flag
CREATE_PUBVAR(PwmLimRtrBpImbFlg,    unsigned);  // Power limit rotor baseplate temperautre imbalance active flag
CREATE_PUBVAR(PwmLimDnTwrTempFlg,   unsigned);  // Power limit down tower temperature active flag
CREATE_PUBVAR(PwmLimClntTempFlg,    unsigned);  // Power limit coolant temperature active flag
CREATE_PUBVAR(PwmLimPdcCabEnTmpFlg, unsigned);  // Power limit PDC cable entry cabinet temperature active flag
CREATE_PUBVAR(PwmLimCfcTempFlg,     unsigned);  // Power limit CFC temperature active flag
CREATE_PUBVAR(PwmLimPdcTempFlg,     unsigned);  // Power limit PDC temperature active flag

CREATE_PUBVAR(PwrLimGmCurHiTmr,     float);     // Power limit grid monitor current indicator timer
CREATE_PUBVAR(PwrLimRtrCurHiTmr,    float);     // Power limit rotor current indicator timer
CREATE_PUBVAR(PwrLimStrCurHiTmr,    float);     // Power limit stator current indicator timer
CREATE_PUBVAR(PwrLimRtrBpRiseHiTmr, float);     // Power limit rotor baseplate temperautre rise indicator timer
CREATE_PUBVAR(PwrLimRtrBpImbHiTmr,  float);     // Power limit rotor baseplate temperautre imbalance indicator timer
CREATE_PUBVAR(PwrLimDnTwrTempHiTmr, float);     // Power limit down tower temperature indicator timer
CREATE_PUBVAR(PwrLimClntTempHiTmr,  float);     // Power limit coolant temperature indicator timer
CREATE_PUBVAR(PwrLimRtrIndTmpHiTmr, float);     // Power limit rotor inductor temperature indicator timer
CREATE_PUBVAR(PwrLimPdcCbEnTmpHTmr, float);     // Power limit PDC cable entry cabinet temperature indicator timer
CREATE_PUBVAR(PwrLimCfcTempHiTmr,   float);     // Power limit CFC temperature indicator timer
CREATE_PUBVAR(PwrLimPdcTempHiTmr,   float);     // Power limit PDC temperature indicator timer


// Function Prototypes
//--------------------
void PowerLimiterDiagnosticRule(void);

/***************************************************************************
*
*                               Private Variables
*
*****************************************************************************/

// Unpublished variables for generator current limit calculation
float           SpdILimSqr;             //SpdILim Square
float           SpdILimCub;             //SpdILim Cubic
float           SpdILimBqu;             //SpdILim Biquadrate
float           AirTempGenInSqr;        //AirTempGenIn Square



///////////////////////////////////////////////////////////////////////////////
//
//       Begin functions used in speed limitation
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Function:         DownTowerTempCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
// Abstract:         This function calculated down-tower temperature
//
// Engineering
//
///////////////////////////////////////////////////////////////////////////////
void  DownTowerTempCalcT3(void)
{

    if (SysTmSec < 5)
    {
        DnTwrTemp  = InletAirTemp;
    }
    else
    {
        DnTwrTemp  = DnTwrTemp + DnTwrTempGn*( InletAirTemp - DnTwrTemp);
    }

}

///////////////////////////////////////////////////////////////////////////////
// Function:         DownTowerTempRules
// Execution Level:  Task 3
// Execution Time:   Not measured
// Abstract:         This function calculated down-tower temperature rules
//
// Engineering
//
///////////////////////////////////////////////////////////////////////////////
void  DownTowerTempRules()
{

   SET_FV(DnTwrTempW, WBTocTauRcp);
   DnTwrTempGn  = PARM(DnTwrTempW)*DelTm3;

}


///////////////////////////////////////////////////////////////////////////////
// Function:         LineConverterCurrentLimitCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
// Abstract:         This function is used to find the minimum line current
//
// Engineering Diagram:  \Doc\Diagrams\Rotor\LCL\Torque Control\TRQ_480.vsd
//
///////////////////////////////////////////////////////////////////////////////
void  LineConverterCurrentLimitCalcT3(void)
{
    float L_IMaxGnx;
    float DelInt;
    double ipart;              // Integer part of floating point number

    //Line converter currrent magnitude limiter path
    //Dynamic current limit as a function of temperature
    if (SysTmSec > 5)
    {
        // clamp input temperature
        if(DnTwrTemp>PARM(LinILimTempMax))
        {
            LinILimTemp = PARM(LinILimTempMax);
        }
        else if(DnTwrTemp<PARM(LinILimTempMin))
        {
            LinILimTemp = PARM(LinILimTempMin);
        }
        else
        {
            LinILimTemp = DnTwrTemp;
        }
        LinILimCnv1km =   PARM(LinILimDeRtK1)
                        + PARM(LinILimDeRtX1)*LinILimTemp
                        + PARM(LinILimDeRtX2)*LinILimTemp*LinILimTemp
                        + PARM(LinILimDeRtX3)*LinILimTemp*LinILimTemp*LinILimTemp;

        L_ILimCnv = LinILimCnv1km*PARM(LinILimAltSf) + PARM(LinILimDstCmp);
       L_IMaxGnx     =  L_IMaxGnDyn;

    }
    else
    {
       L_ILimCnv   =  PARM(WBLineILim1);
       L_IMaxGnx  =  1.0F;
    }

    L_IMaxCrvSq   = L_ILimCnv * L_ILimCnv;

    //Filter
    //Need extra precision here
    DelInt        = L_IMaxGnx * (L_IMaxCrvSq - L_MxCurOutSq);
    L_MaxCurSqFrc = modf( (L_MaxCurSqFrc + DelInt), &ipart);
    L_MaxCurSqInt = L_MaxCurSqInt + ipart;
    L_MxCurOutSq  = L_MaxCurSqInt + L_MaxCurSqFrc;
    /* clamp to max allowed current */
    if ( L_MxCurOutSq > (PARM(WBLnIMaxLim) * PARM(WBLnIMaxLim)) )
    {
        L_MxCurOutSq  = (PARM(WBLnIMaxLim) * PARM(WBLnIMaxLim));
        L_MaxCurSqInt = (int)L_MxCurOutSq;
        L_MaxCurSqFrc = 0.0;

    }
    L_MaxCurOut  = sqrtf(L_MxCurOutSq);

    LinIMrgSqr = L_MxCurOutSq - L_TocHiSelSq;
    LinILimOut = PARM(LnILimGn) * LinIMrgSqr;
    LinILimMrg = L_MaxCurOut- L_TocHiSel;

    WBLinDynSlp          = NoNANLim(( PARM(WBLineILim1)       -  PARM(WBLineILim2))         / (PARM(WBLineITemp1)     - PARM(WBLineITemp2)),       1.0e6F, -1.0e6F);


}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      LineConverterCurrentLimitRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void LineConverterCurrentLimitRules()
{

    float TocThrsVar;
    float TmpTocThrs;
    float TmpMaxCur;
    float TmpILimGn;


    SET_FV(L_IDistTOC, 58.61F);
    SET_FV(L_TocTau, PARM(WBTocTauNorm));
    L_WTocT3      = DelTm3/PARM(L_TocTau);
    L_WTocFstT3   = (DelTm3 * PARM(TocTauTstScl)) / PARM(L_TocTau);


    if ( (PARM(SoftwareIPN) == DFIG02_1500_5060_A) || (PARM(SoftwareIPN) == DFIG02_1500_5060_B) )
    {
        SET_FV(L_IRat,          274.0F);
        SET_FV(L_TocMargin,     1.15F);

    }
    else if ( (PARM(SoftwareIPN) == DFIG02_1850_5060_C) || (PARM(SoftwareIPN) == DFIG02_1850_5060_D) || (PARM(SoftwareIPN) == DFIG02_1850_5060_E) )
    {
        SET_FV(L_IRat,      340.0F);
        SET_FV(L_TocMargin, 1.15F);
    }
    else //( (PARM(SoftwareIPN) == DFIG02_2400_5060_A)
    {
        SET_FV(L_IRat,      480.0F);
        SET_FV(L_TocMargin, 1.06F);
    }
    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
        SET_FV(LinILimDeRtK1,   280.00F);// = 340 - m*35.0
        SET_FV(LinILimDeRtX1,   0.0F);   //(274.0 - 340.0)/(45.0 - 35.0) = m
        SET_FV(LinILimDeRtX2,   0.0F);
        SET_FV(LinILimDeRtX3,   0.0F);
        SET_FV(LinILimAltLpse,  1.00F);
        break;
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
        SET_FV(LinILimDeRtK1,   571.00F);// = 340 - m*35.0
        SET_FV(LinILimDeRtX1,   -6.60F); //(274.0 - 340.0)/(45.0 - 35.0) = m
        SET_FV(LinILimDeRtX2,   0.0F);
        SET_FV(LinILimDeRtX3,   0.0F);
        SET_FV(LinILimAltLpse,  1.0F);
        break;
    case DFIG02_1850_5060_E:
        SET_FV(LinILimDeRtK1,   571.00F);// = 340 - m*35.0
        SET_FV(LinILimDeRtX1,   -6.60F); //(274.0 - 340.0)/(45.0 - 35.0) = m
        SET_FV(LinILimDeRtX2,   0.0F);
        SET_FV(LinILimDeRtX3,   0.0F);
        SET_FV(LinILimAltLpse,  0.97F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(LinILimDeRtK1,   577.5F);
        SET_FV(LinILimDeRtX1,   -6.21667F);
        SET_FV(LinILimDeRtX2,   0.42F);
        SET_FV(LinILimDeRtX3,   -0.00733F);
        SET_FV(LinILimAltLpse,  0.98F);
        break;
    case DFIG02_2400_5060_B:
        SET_FV(LinILimDeRtK1,   577.5F);
        SET_FV(LinILimDeRtX1,   -6.21667F);
        SET_FV(LinILimDeRtX2,   0.42F);
        SET_FV(LinILimDeRtX3,   -0.00733F);
        SET_FV(LinILimAltLpse,  0.98F);
        break;
    }
                                    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
    case DFIG02_1850_5060_E:
        SET_FV(LinILimTempMin,  35.0F);
        SET_FV(LinILimTempMax,  55.0F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(LinILimTempMin,  25.0F);
        SET_FV(LinILimTempMax,  55.0F);
        break;
    case DFIG02_2400_5060_B:
        SET_FV(LinILimTempMin,  25.0F);
        SET_FV(LinILimTempMax,  55.0F);
        break;
    }

    SET_FV(LinILimAltDeRtX1,    -(1.0F-PARM(LinILimAltLpse))*0.001F);
    SET_FV(LinILimAltDeRtX2,    0.0F);
    SET_FV(LinILimAltDeRtK1,    1.0F-PARM(LinILimAltDeRtX1)*PARM(AltitudeRat)-PARM(LinILimAltDeRtX2)*PARM(AltitudeRat)*PARM(AltitudeRat));
    float tempLinILimAltSf;
    tempLinILimAltSf = PARM(LinILimAltDeRtK1)+PARM(LinILimAltDeRtX1)*AltitudeClmp+PARM(LinILimAltDeRtX2)*AltitudeClmp*AltitudeClmp;
    if(tempLinILimAltSf>1.0F)
    {
        SET_FV(LinILimAltSf, 1.0F);
    }
    else
    {
        SET_FV(LinILimAltSf, tempLinILimAltSf);
    }

    // use the minimum temperature to calculate the IFnd1
    SET_FV(WBLineIFnd1, PARM(LinILimAltSf)*
                    (PARM(LinILimDeRtK1) + PARM(LinILimDeRtX1)*PARM(LinILimTempMin)
                    + PARM(LinILimDeRtX2)*PARM(LinILimTempMin)*PARM(LinILimTempMin)
                    + PARM(LinILimDeRtX3)*PARM(LinILimTempMin)*PARM(LinILimTempMin)*PARM(LinILimTempMin)));
    // use the maximum temperature to calculate the IFnd2
    SET_FV(WBLineIFnd2, PARM(LinILimAltSf)*
                    (PARM(LinILimDeRtK1) + PARM(LinILimDeRtX1)*PARM(LinILimTempMax)
                    + PARM(LinILimDeRtX2)*PARM(LinILimTempMax)*PARM(LinILimTempMax)
                    + PARM(LinILimDeRtX3)*PARM(LinILimTempMax)*PARM(LinILimTempMax)*PARM(LinILimTempMax)));
    SET_FV( WBLineITemp1, PARM(WBTemp) );        //this one varies
    SET_FV( WBLineITemp2, PARM(WBIRatTempHi) );


    SET_FV(L_ICnvNameP, PARM(L_IRat) );

    if ( PARM(TestMode) != PUMP_BACK )
    {
       SET_FV(WBLineILim1, sqrtf( (PARM(WBLineIFnd1) * PARM(WBLineIFnd1)) + (PARM(L_IDistTOC) * PARM(L_IDistTOC) ) ) );
       SET_FV(WBLineILim2, sqrtf( (PARM(WBLineIFnd2) * PARM(WBLineIFnd2)) + (PARM(L_IDistTOC) * PARM(L_IDistTOC) ) ) );

       TocThrsVar = 1.15 * sqrtf( (PARM(L_IDistTOC) * PARM(L_IDistTOC) ) + (PARM(L_ICnvNameP) * PARM(L_ICnvNameP)) );
       TocThrsVar = MIN(TocThrsVar, ( PARM(L_TocMargin) * PARM(WBLineILim1) ) );
       SET_FV(L_TocThrs,   TocThrsVar);
       SET_FV(L_TocAlmOn,  PARM(L_IRat) * TOC_ALM_SCL );
       SET_FV(L_TocAlmOff, PARM(L_IRat));
       SET_FV(WBLnIMaxLim, TocThrsVar/PARM(L_TocMargin) )
    }
    else
    {
        SET_FV(WBLineILim1, PARM(WBLnIMaxLim));
        SET_FV(WBLineILim2, PARM(WBLnIMaxLim));

        if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
        {

           TocThrsVar        = 690.0F;
           SET_FV(L_TocThrs,   TocThrsVar);
           SET_FV(L_TocAlmOn,  650.0F);
           SET_FV(L_TocAlmOff, 630.0F);
        }
        else
        {
           TocThrsVar        = 500.0F;
           SET_FV(L_TocThrs,   TocThrsVar);
           SET_FV(L_TocAlmOn,  475.0F);
           SET_FV(L_TocAlmOff, 450.0F);
        }
        SET_FV(WBLnIMaxLim, TocThrsVar/PARM(L_TocMargin) );
    }
    SET_FV(LinILimDstCmp,   PARM(WBLineILim1)-PARM(WBLineIFnd1));
    SET_FV(L_MaxCur, PARM(WBLnIMaxLim) );
    SET_FV(L_IRatSSLim, PARM(WBLnIMaxLim));


    L_TocAlmOnDl  = PARM(L_TocThrs)  - PARM(L_TocAlmOn);
    L_TocAlmOffDl = PARM(L_TocAlmOn) - PARM(L_TocAlmOff);


    SET_FV( L_IMaxTau, PARM(L_TocTau) );
    L_IMaxGn    = DelTm3 / PARM(L_IMaxTau);
    L_IMaxFstGn = (DelTm3 * PARM(TocTauTstScl)) / PARM(L_IMaxTau);


    // LnILimGn
    TmpMaxCur  = PARM(WBLnIMaxLim);
    TmpTocThrs = PARM(L_TocMargin)* TmpMaxCur;

    TmpILimGn  = R_RpmSlipNom / (TmpTocThrs * TmpTocThrs - TmpMaxCur * TmpMaxCur);
    SET_FV(LnILimGn, TmpILimGn);

}


///////////////////////////////////////////////////////////////////////////////
// Function:         AclfuCurrentLimitCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
// Abstract:         This function is used to find the minimum ACLFU current
//
//
///////////////////////////////////////////////////////////////////////////////
void  AclfuCurrentLimitCalcT3(void)
{
    float L_FuseIMaxGnx;
    float DelInt;
    double ipart;              // Integer part of floating point number
    float L_FuseIMrgSqr;


    //Line converter currrent magnitude limiter path
    //Dynamic current limit as a function of temperature
    if (SysTmSec > 5)
    {
        // clamp input temperature
        if(DnTwrTemp>PARM(AclfuILimTempMax))
        {
            AclfuILimTemp = PARM(AclfuILimTempMax);
        }
        else if(DnTwrTemp<PARM(AclfuILimTempMin))
        {
            AclfuILimTemp = PARM(AclfuILimTempMin);
        }
        else
        {
            AclfuILimTemp = DnTwrTemp;
        }
        AclfuILim1km =   PARM(AclfuILimDeRtK1)
                        + PARM(AclfuILimDeRtX1)*AclfuILimTemp
                        + PARM(AclfuILimDeRtX2)*AclfuILimTemp*AclfuILimTemp
                        + PARM(AclfuILimDeRtX3)*AclfuILimTemp*AclfuILimTemp*AclfuILimTemp;

        L_FuseILimCnv = AclfuILim1km*PARM(AclfuILimAltSf) + PARM(AclfuILimDstCmp);
        L_FuseIMaxGnx     =  L_FuseIMaxGnDyn;
    }
    else
    {
       L_FuseILimCnv   =  PARM(WBAclfuILim1);
       L_FuseIMaxGnx  =  1.0F;
    }

    L_FuseIMaxCrvSq   = L_FuseILimCnv * L_FuseILimCnv;

    //Filter
    //Need extra precision here
    DelInt        = L_FuseIMaxGnx * (L_FuseIMaxCrvSq - L_FuseMxCurOutSq);
    L_FuseMaxCurSqFrc = modf( (L_FuseMaxCurSqFrc + DelInt), &ipart);
    L_FuseMaxCurSqInt = L_FuseMaxCurSqInt + ipart;
    L_FuseMxCurOutSq  = L_FuseMaxCurSqInt + L_FuseMaxCurSqFrc;
    /* clamp to max allowed current */
    if ( L_FuseMxCurOutSq > (PARM(WBAclfuIMaxLim) * PARM(WBAclfuIMaxLim)) )
    {
        L_FuseMxCurOutSq  = (PARM(WBAclfuIMaxLim) * PARM(WBAclfuIMaxLim));
        L_FuseMaxCurSqInt = (int)L_FuseMxCurOutSq;
        L_FuseMaxCurSqFrc = 0.0;

    }
    L_FuseMaxCurOut  = sqrtf(L_FuseMxCurOutSq);


    L_FuseIMrgSqr = L_FuseMxCurOutSq - L_FuseTocHiSelSq;
    L_FuseILimOut = PARM(L_FuseILimGn) * L_FuseIMrgSqr;
    L_FuseILimMrg = L_FuseMaxCurOut- L_FuseTocHiSel;


}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      AclfuCurrentLimitRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void AclfuCurrentLimitRules()
{
    float FuseGnTemp;
    float FuseIMaxTemp;

    SET_FV(L_AclfuIDistTOC, 0.0F);

    SET_FV(L_FuseTocTau, PARM(WBTocTauNorm));
    L_FuseWTocT3    = DelTm3/PARM(L_FuseTocTau);
    L_FuseWTocFstT3 = (DelTm3 * PARM(TocTauTstScl)) / PARM(L_FuseTocTau);

    SET_FV( L_FuseIMaxTau, PARM(L_FuseTocTau) );
    L_FuseIMaxGn    = DelTm3/PARM(L_FuseIMaxTau);
    L_FuseIMaxFstGn = (DelTm3 * PARM(TocTauTstScl))/PARM(L_FuseIMaxTau);


    if ( (PARM(SoftwareIPN) == DFIG02_1500_5060_A) || (PARM(SoftwareIPN) == DFIG02_1500_5060_B) )
    {
        SET_FV(L_FuseTocMargin, 1.06F);
    }
    else if ( (PARM(SoftwareIPN) == DFIG02_1850_5060_C) || (PARM(SoftwareIPN) == DFIG02_1850_5060_D) || (PARM(SoftwareIPN) == DFIG02_1850_5060_E))
    {
        SET_FV(L_FuseTocMargin, 1.06F);
    }

    else //( (PARM(SoftwareIPN) == DFIG02_2400_5060_A)
    {
        SET_FV(L_FuseTocMargin, 1.06F);
    }
    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
        SET_FV(AclfuILimDeRtK1, 637.0F);
        SET_FV(AclfuILimDeRtX1, -2.6F);
        SET_FV(AclfuILimDeRtX2, 0.0F);
        SET_FV(AclfuILimDeRtX3, 0.0F);
        SET_FV(AclfuILimAltLpse,1.00F);
        break;
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
        SET_FV(AclfuILimDeRtK1, 637.0F);
        SET_FV(AclfuILimDeRtX1, -2.6F);
        SET_FV(AclfuILimDeRtX2, 0.0F);
        SET_FV(AclfuILimDeRtX3, 0.0F);
        SET_FV(AclfuILimAltLpse,1.0F);
        break;
    case DFIG02_1850_5060_E:
        SET_FV(AclfuILimDeRtK1, 637.0F);
        SET_FV(AclfuILimDeRtX1, -2.6F);
        SET_FV(AclfuILimDeRtX2, 0.0F);
        SET_FV(AclfuILimDeRtX3, 0.0F);
        SET_FV(AclfuILimAltLpse,0.96F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(AclfuILimDeRtK1, 623.75F);
        SET_FV(AclfuILimDeRtX1, -8.54167F);
        SET_FV(AclfuILimDeRtX2, 0.43F);
        SET_FV(AclfuILimDeRtX3, -0.00633F);
        SET_FV(AclfuILimAltLpse,0.98F);
        break;
    case DFIG02_2400_5060_B:
        SET_FV(AclfuILimDeRtK1, 623.75F);
        SET_FV(AclfuILimDeRtX1, -8.54167F);
        SET_FV(AclfuILimDeRtX2, 0.43F);
        SET_FV(AclfuILimDeRtX3, -0.00633F);
        SET_FV(AclfuILimAltLpse,0.98F);
        break;
    }
    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
    case DFIG02_1850_5060_E:
        SET_FV(AclfuILimTempMin,  35.0F);
        SET_FV(AclfuILimTempMax,  55.0F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(AclfuILimTempMin,  25.0F);
        SET_FV(AclfuILimTempMax,  55.0F);
        break;
    case DFIG02_2400_5060_B:
        SET_FV(AclfuILimTempMin,  25.0F);
        SET_FV(AclfuILimTempMax,  55.0F);
        break;
    }
    SET_FV(AclfuILimAltDeRtX1,    -(1.0F-PARM(AclfuILimAltLpse))*0.001F);
    SET_FV(AclfuILimAltDeRtX2,    0.0F);
    SET_FV(AclfuILimAltDeRtK1,    1.0F-PARM(AclfuILimAltDeRtX1)*PARM(AltitudeRat)-PARM(AclfuILimAltDeRtX2)*PARM(AltitudeRat)*PARM(AltitudeRat));
    float tempAclfuILimAltSf;
    tempAclfuILimAltSf = PARM(AclfuILimAltDeRtK1)+PARM(AclfuILimAltDeRtX1)*AltitudeClmp+PARM(AclfuILimAltDeRtX2)*AltitudeClmp*AltitudeClmp;
    if(tempAclfuILimAltSf>1.0F)
    {
        SET_FV(AclfuILimAltSf, 1.0F);
    }
    else
    {
        SET_FV(AclfuILimAltSf, tempAclfuILimAltSf);
    }

    // use the minimum temperature to calculate the IFnd1
    SET_FV(WBAclfuIFnd1, PARM(AclfuILimAltSf)*
                     (PARM(AclfuILimDeRtK1)
                    + PARM(AclfuILimDeRtX1)*PARM(AclfuILimTempMin)
                    + PARM(AclfuILimDeRtX2)*PARM(AclfuILimTempMin)*PARM(AclfuILimTempMin)
                    + PARM(AclfuILimDeRtX3)*PARM(AclfuILimTempMin)*PARM(AclfuILimTempMin)*PARM(AclfuILimTempMin)));
    // use the maximum temperature to calculate the IFnd2
    SET_FV(WBAclfuIFnd2, PARM(AclfuILimAltSf)*
                     (PARM(AclfuILimDeRtK1)
                    + PARM(AclfuILimDeRtX1)*PARM(AclfuILimTempMax)
                    + PARM(AclfuILimDeRtX2)*PARM(AclfuILimTempMax)*PARM(AclfuILimTempMax)
                    + PARM(AclfuILimDeRtX3)*PARM(AclfuILimTempMax)*PARM(AclfuILimTempMax)*PARM(AclfuILimTempMax)));


    SET_FV( WBAclfuITemp1, PARM(WBTemp) );        //this one varies
    SET_FV( WBAclfuITemp2, PARM(WBIRatTempHi) );   //normally 50 deg, Tony: 55 DegC

    if ( PARM(TestMode) != PUMP_BACK )
    {

        SET_FV(L_FuseTocThrs,   700.0F * 0.80F);
        SET_FV(L_FuseTocAlmOn,  700.0F * 0.78F);
        SET_FV(L_FuseTocAlmOff, 700.0F * 0.75F);

        SET_FV(WBAclfuIMaxLim,  sqrtf( (PARM(L_FuseTocThrs)/PARM(L_FuseTocMargin)) * (PARM(L_FuseTocThrs)/PARM(L_FuseTocMargin)) ) );
        SET_FV(WBAclfuILim1,    sqrtf( (PARM(WBAclfuIFnd1) * PARM(WBAclfuIFnd1)) + (PARM(L_AclfuIDistTOC) * PARM(L_AclfuIDistTOC) ) ) );
        SET_FV(WBAclfuILim2,    sqrtf( (PARM(WBAclfuIFnd2) * PARM(WBAclfuIFnd2)) + (PARM(L_AclfuIDistTOC) * PARM(L_AclfuIDistTOC) ) ) );

    }
    else
    {

        // Hl 20121022 need revisit!!
        SET_FV(L_FuseTocThrs,   630.0F);
        SET_FV(L_FuseTocAlmOn,  620.0F);
        SET_FV(L_FuseTocAlmOff, 610.0F);

        // rules below need to be updated when ACLFU TOC dyn curve is available !!!
        SET_FV(WBAclfuIMaxLim, 632.5F/1.15F);   // ESS value, need updated??
        SET_FV(WBAclfuILim1, PARM(WBLnIMaxLim));
        SET_FV(WBAclfuILim2, PARM(WBLnIMaxLim));

    }
    SET_FV(AclfuILimDstCmp,   PARM(WBAclfuILim1)-PARM(WBAclfuIFnd1));
    FuseIMaxTemp =  PARM(L_FuseTocThrs) * 0.97;

    FuseGnTemp  = R_RpmSlipNom / (PARM(L_FuseTocThrs) * PARM(L_FuseTocThrs) - FuseIMaxTemp * FuseIMaxTemp);
    SET_FV(L_FuseILimGn, FuseGnTemp);

    L_FuseTocAlmOnDl  = PARM(L_FuseTocThrs)  - PARM(L_FuseTocAlmOn);
    L_FuseTocAlmOffDl = PARM(L_FuseTocAlmOn) - PARM(L_FuseTocAlmOff);


    WBAclfuDynSlp        = NoNANLim(( PARM(WBAclfuILim1)      -  PARM(WBAclfuILim2))        / (PARM(WBAclfuITemp1)    - PARM(WBAclfuITemp2)),      1.0e6F, -1.0e6F);


}





///////////////////////////////////////////////////////////////////////////////
// Function:         ThermisterBasedSpeedLimiterCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
//
//
// Abstract:         This function is used to find the maximum allowed line current
//                   as a function of various temperature sensors
//
// Engineering Diagram:
//
///////////////////////////////////////////////////////////////////////////////
void  ThermisterBasedSpeedLimiterCalcT3(void)
{


    AclfuTempLimMrg = PARM(AclfuTempLim) - AclfuTemp;
    AclfuTempLimOut = PARM(AclfuTempLimGn) * AclfuTempLimMrg;




}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      ThermisterBasedSpeedLimiterRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void ThermisterBasedSpeedLimiterRules()
{
    SET_FV(AclfuTempLim,   PARM(AclfuOtThrsh) - 5.0F) ;
    SET_FV(AclfuTempLimGn, 0.2F * R_RpmSlipNom);    // derate from 75deg C and torque is 0 @ 80deg C



}



///////////////////////////////////////////////////////////////////////////////
// Function:         LineBasePlateTempCurrentLimitCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
//
//
// Abstract:         This function is used to find the maximum allowed line
//                   current allowed as functions of the heat sink imbalance
//                   and heat sink to coolant rise issues.
//
// Engineering Diagram:
//
///////////////////////////////////////////////////////////////////////////////
void  LineBasePlateTempCurrentLimitCalcT3(void)
{

    //-------------------------------------------------
    //Line heat sink temperature rise and imbalance, initialization and filtering

    if (SysTmSec < 10.0F)
    {
       L_SinkTempRseFil = 5.0F;
       LinImbTempFil    = LinBthtrImbMax;
    }
    else
    {
       //Line heat sink temperature imblance limiter path
       L_SinkTempRse    = L_BrgThrmstr.BthtrTempMax - CoolantTemp;
       L_SinkTempRseFil = L_SinkTempRseFil + L_SinkTempRseGn * ( L_SinkTempRse- L_SinkTempRseFil);

       LinBthtrImbMax   = L_BrgThrmstr.BthtrImbMax;
       LinImbTempFil    = LinImbTempFil + LinImbTempGn * ( LinBthtrImbMax- LinImbTempFil);

    }

    //-------------------------------------------------
    //Line heat sink temperature rise limiter path
    LinTempRseLimMrg = PARM(LinSinkTempRseLim) - L_SinkTempRseFil;
    LinTempRseLimOut = PARM(LinSinkTempRseLimGn) * LinTempRseLimMrg;

    //-------------------------------------------------
    //Line heat sink temperature imbalance limiter path
    LinImbTempLimMrg = PARM(LinImbTempLim) - LinImbTempFil;
    LinImbTempLimOut = PARM(LinImbTempLimGn) * LinImbTempLimMrg;

    //-------------------------------------------------
    //Total Out
    L_HtSinkTempILimOut = MIN(LinImbTempLimOut, LinTempRseLimOut);


}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      LineBasePlateTempCurrentLimitRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void LineBasePlateTempCurrentLimitRules()
{
    SET_FV(LinImbTempLim,      12.0F);

    SET_FV(PdcTempLimGn,           0.2F * R_RpmSlipNom);

    SET_FV(LinSinkTempRseLim, 100.0F);    // set hi to defea
    SET_FV(LinSinkTempRseLimGn,    0.2F * R_RpmSlipNom);    //
    SET_FV(LinImbTempLimGn,        0.2F * R_RpmSlipNom);    //


    SET_FV(SinkTempDeRtW, 10.0F);
    LinImbTempGn    = PARM(SinkTempDeRtW) * DelTm3;
    L_SinkTempRseGn = PARM(SinkTempDeRtW) * DelTm3;


}


///////////////////////////////////////////////////////////////////////////////
// Function:         RotorVoltageLimiterCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
//
//
// Abstract:         This function is used to help limit the rotor voltage
//                   by limiting speed when necessary.
//
// Engineering Diagram:
//
///////////////////////////////////////////////////////////////////////////////
void  RotorVoltageLimiterCalcT3(void)
{

    //-------------------------------------------------
    //Rotor voltage magnitude limiter path
    RtrVSqrFil = RtrVSqrFil + RtrVLimGn*( (1.5*R_VMag*R_VMag)- RtrVSqrFil);
    RotorVMgFil  = sqrtf(RtrVSqrFil);
    RtrVMrgSqr = RtrVLimSqr - RtrVSqrFil;
    if (R_SuperSync)
    {
        RtrVLimOut = PARM(RtrVLimGn)*RtrVMrgSqr;
    }
    else
    {
        RtrVLimOut = PARM(RtrVSubSPwr);
    }
    RtrVLimMrg = PARM(RtrVLim) - RotorVMgFil;


    if (RotorVMgFil > PARM(R_OVTrpThrs))
    {
        PUSH_DIAG(R_VacOVFlt);
    }

}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      RotorVoltageLimiterRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void RotorVoltageLimiterRules()
{

    SET_FV(RtrVSubSPwr,3.276e6 ); //Watts
    SET_FV(RtrVLimGn, 0.01);
    SET_FV(RtrVLim, 1.10* PARM(L_VllIn));      //volts rms
    SET_FV(RtrVLimW, WBTocTauRcp);


    RtrVLimSqr   = PARM(RtrVLim) * PARM(RtrVLim);
    RtrVLimGn    = PARM(RtrVLimW)*DelTm3;

}

















///////////////////////////////////////////////////////////////////////////////
//
//       Begin functions used in torque limitation
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:         GridMonitorCurrentLimitCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
//
//
// Abstract:         This function is used to find the minimum line current
//
// Engineering Diagram:  \Doc\Diagrams\Rotor\LCL\Torque Control\TRQ_480.vsd
//
///////////////////////////////////////////////////////////////////////////////
void  GridMonitorCurrentLimitCalcT3(void)
{

    float GmIMaxGnx;
    float DelInt;
    double ipart;              // Integer part of floating point number

    //Line converter currrent magnitude limiter path
    //Dynamic current limit as a function of temperature
    if (SysTmSec > 5)
    {
        // clamp input temperature
        if(DnTwrTemp>PARM(GmILimTempMax))
        {
            GmILimTemp = PARM(GmILimTempMax);
        }
        else if(DnTwrTemp<PARM(GmILimTempMin))
        {
            GmILimTemp = PARM(GmILimTempMin);
        }
        else
        {
            GmILimTemp = DnTwrTemp;
        }

        // new current limit structure. ZJ, 20150310
        GmILimCnv1km = PARM(GmILimDeRtK1) + PARM(GmILimDeRtX1)*GmILimTemp
                    + PARM(GmILimDeRtX2)*GmILimTemp*GmILimTemp
                    + PARM(GmILimDeRtX3)*GmILimTemp*GmILimTemp*GmILimTemp;

        GmILimCnv = GmILimCnv1km*PARM(GmILimAltSf) + PARM(GmILimDstCmp);
        GmIMaxGnx     =  GmIMaxGnDyn;

    }
    else
    {
        GmILimCnv   =  PARM(WBGmILim1);
        GmIMaxGnx   =  1.0F;
    }


    GmIMaxCrvMin = GmILimCnv;

    GmIMaxCrvSq   = GmIMaxCrvMin * GmIMaxCrvMin;

    //Filter
    //Need extra precision here
    DelInt        = GmIMaxGnx * (GmIMaxCrvSq - GmMxCurOutSq);
    GmMaxCurSqFrc = modf( (GmMaxCurSqFrc + DelInt), &ipart);
    GmMaxCurSqInt = GmMaxCurSqInt + ipart;
    GmMxCurOutSq  = GmMaxCurSqInt + GmMaxCurSqFrc;

    // clamp to max allowed current
    if (GmMxCurOutSq > (PARM(WBGmIMaxLim) * PARM(WBGmIMaxLim)) )
    {
        GmMxCurOutSq  = ( PARM(WBGmIMaxLim) * PARM(WBGmIMaxLim) );
        GmMaxCurSqInt = (int)GmMxCurOutSq;
        GmMaxCurSqFrc = 0.0;

    }
    GmMaxCurOut  = sqrtf(GmMxCurOutSq);

    GmILimMrg = GmMaxCurOut - GmTocHiSel; //Diag only
    GmIMrgSqr = GmMxCurOutSq - GmTocHiSelSq;
    GmILimOut = PARM(GmILimGn) * GmIMrgSqr;


}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      GridMonitorCurrentLimitRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorCurrentLimitRules()
{
    float TocThrsVar;
    float TmpTocThrs;
    float TmpMaxCur;
    float TmpILimGn;


    SET_FV(GmIDistTOC, 62.61F);

    // GmILimGn                      // SHOULD USE TocThrs parms but will get "dependent rule"
    TmpMaxCur   = PARM(WBGmIMaxLim); //  - need to move these rules so TocThrs parms can be used
    TmpTocThrs  = PARM(GmTocMargin)* TmpMaxCur;
    TmpILimGn  = R_RpmSlipNom / (TmpTocThrs * TmpTocThrs - TmpMaxCur * TmpMaxCur);
    SET_FV(GmILimGn, TmpILimGn);

    SET_FV(GmTocTau, PARM(WBTocTauNorm));
    GmWTocT3     = DelTm3/PARM(GmTocTau);
    GmWTocFstT3  = (DelTm3 * PARM(TocTauTstScl))/PARM(GmTocTau);
    SET_FV(GmIMaxTau, PARM(GmTocTau));
    GmIMaxGn    = DelTm3/PARM(GmIMaxTau);
    GmIMaxFstGn = (DelTm3 * PARM(TocTauTstScl))/PARM(GmIMaxTau);




    if ( (PARM(SoftwareIPN) == DFIG02_1500_5060_A) || (PARM(SoftwareIPN) == DFIG02_1500_5060_B) )
    {
        SET_FV(GmTocMargin, 1.05F);

        SET_FV(Q1Irat,    1570.435F);
        SET_FV(GmCsaIrat, 1459.65F);

    }
    else if ( (PARM(SoftwareIPN) == DFIG02_1850_5060_C) || (PARM(SoftwareIPN) == DFIG02_1850_5060_D) || (PARM(SoftwareIPN) == DFIG02_1850_5060_E))
    {
        SET_FV(GmTocMargin, 1.05F);

        SET_FV(Q1Irat,    1570.435F);
        SET_FV(GmCsaIrat, 1570.435F);

    }
    else //( (PARM(SoftwareIPN) == DFIG02_2400_5060_A)
    {
        SET_FV(GmTocMargin, 1.06F);


        SET_FV(Q1Irat,    2200.0F);
        SET_FV(GmCsaIrat, 2200.0F);
    }
    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
        SET_FV(GmILimDeRtK1,    1598.66F);  // =  1598.66 - m*35.0
        SET_FV(GmILimDeRtX1,    0.0F);      //(1598.66- 1598.66)/(45.0 - 35.0) = m
        SET_FV(GmILimDeRtX2,    0.0F);
        SET_FV(GmILimDeRtX3,    0.0F);
        SET_FV(GmILimAltLpse,   1.00F);
        break;
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
        SET_FV(GmILimDeRtK1,    1598.66F);  // =  1598.66 - m*35.0
        SET_FV(GmILimDeRtX1,    0.0F);      //(1598.66- 1598.66)/(45.0 - 35.0) = m
        SET_FV(GmILimDeRtX2,    0.0F);
        SET_FV(GmILimDeRtX3,    0.0F);
        SET_FV(GmILimAltLpse,   1.00F);
        break;
    case DFIG02_1850_5060_E:
        SET_FV(GmILimDeRtK1,    2141.54F); // = 1719.3 - m*35.0
        SET_FV(GmILimDeRtX1,    -12.064F); // (1598.66- 1719.3)/(45.0 - 35.0) = m
        SET_FV(GmILimDeRtX2,    0.0F);
        SET_FV(GmILimDeRtX3,    0.0F);
        SET_FV(GmILimAltLpse,   0.98F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(GmILimDeRtK1,    3037.5F);
        SET_FV(GmILimDeRtX1,    -43.4167F);
        SET_FV(GmILimDeRtX2,    1.7F);
        SET_FV(GmILimDeRtX3,    -0.02333F);
        SET_FV(GmILimAltLpse,   0.945F);
        break;
    case DFIG02_2400_5060_B:
        SET_FV(GmILimDeRtK1,    3037.5F);
        SET_FV(GmILimDeRtX1,    -43.4167F);
        SET_FV(GmILimDeRtX2,    1.7F);
        SET_FV(GmILimDeRtX3,    -0.02333F);
        SET_FV(GmILimAltLpse,   0.945F);
        break;
    }
    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
    case DFIG02_1850_5060_E:
        SET_FV(GmILimTempMin,   35.0F);
        SET_FV(GmILimTempMax,   55.0F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(GmILimTempMin,   25.0F);
        SET_FV(GmILimTempMax,   55.0F);
        break;
    case DFIG02_2400_5060_B:
        SET_FV(GmILimTempMin,   25.0F);
        SET_FV(GmILimTempMax,   55.0F);
        break;
    }
    SET_FV(GmILimAltDeRtX1,    -(1.0F-PARM(GmILimAltLpse))*0.001F);
    SET_FV(GmILimAltDeRtX2,    0.0F);
    SET_FV(GmILimAltDeRtK1,    1.0F-PARM(GmILimAltDeRtX1)*PARM(AltitudeRat)-PARM(GmILimAltDeRtX2)*PARM(AltitudeRat)*PARM(AltitudeRat));
    float tempGmILimAltSf;
    tempGmILimAltSf = PARM(GmILimAltDeRtK1)+PARM(GmILimAltDeRtX1)*AltitudeClmp+PARM(GmILimAltDeRtX2)*AltitudeClmp*AltitudeClmp;
    if(tempGmILimAltSf>1.0F)
    {
        SET_FV(GmILimAltSf, 1.0F);
    }
    else
    {
        SET_FV(GmILimAltSf, tempGmILimAltSf);
    }
    // use the minimum temperature to calculate the IFnd1
    SET_FV(WBGmIFnd1, PARM(GmILimAltSf)*
                    (PARM(GmILimDeRtK1) + PARM(GmILimDeRtX1)*PARM(GmILimTempMin)
                    + PARM(GmILimDeRtX2)*PARM(GmILimTempMin)*PARM(GmILimTempMin)
                    + PARM(GmILimDeRtX3)*PARM(GmILimTempMin)*PARM(GmILimTempMin)*PARM(GmILimTempMin)));
    // use the maximum temperature to calculate the IFnd2
    SET_FV(WBGmIFnd2, PARM(GmILimAltSf)*
                    (PARM(GmILimDeRtK1) + PARM(GmILimDeRtX1)*PARM(GmILimTempMax)
                    + PARM(GmILimDeRtX2)*PARM(GmILimTempMax)*PARM(GmILimTempMax)
                    + PARM(GmILimDeRtX3)*PARM(GmILimTempMax)*PARM(GmILimTempMax)*PARM(GmILimTempMax)));
    SET_FV( WBGmITemp1, PARM(WBTemp) );         //this one varies
    SET_FV( WBGmITemp2, PARM(WBIRatTempHi) );   //normall 45 deg


    SET_FV(GmICnvNameP,  MIN( PARM(GmCsaIrat), PARM(Q1Irat) ) );


    if ( PARM(TestMode) != PUMP_BACK )
    {

        SET_FV(GmIRatSSLim, PARM(GmICnvNameP) );
        SET_FV(WBGmILim1, sqrtf( (PARM(WBGmIFnd1) * PARM(WBGmIFnd1)) + (PARM(GmIDistTOC) * PARM(GmIDistTOC) ) ) );
        SET_FV(WBGmILim2, sqrtf( (PARM(WBGmIFnd2) * PARM(WBGmIFnd2)) + (PARM(GmIDistTOC) * PARM(GmIDistTOC) ) ) );

        TocThrsVar = 1.15 * sqrtf( (PARM(GmIDistTOC) * PARM(GmIDistTOC) ) + (PARM(GmICnvNameP) * PARM(GmICnvNameP)) );
        TocThrsVar = MIN(TocThrsVar, (PARM(GmTocMargin) * PARM(WBGmILim1)) );
        SET_FV(GmTocThrs,   TocThrsVar );
        SET_FV(GmTocAlmOn,  PARM(GmIRatSSLim) * TOC_ALM_SCL );
        SET_FV(GmTocAlmOff, PARM(GmIRatSSLim));

        SET_FV(WBGmIMaxLim, PARM(GmTocThrs)/PARM(GmTocMargin) );


    }
    else //Pumpback
    {
        SET_FV(GmIRatSSLim, 1000.0F);
        SET_FV(GmTocThrs,   600.0F);
        SET_FV(GmTocAlmOn,  575.0F);
        SET_FV(GmTocAlmOff, 550.0F);

        SET_FV(WBGmIMaxLim, PARM(GmTocThrs)/PARM(GmTocMargin) );
        SET_FV(WBGmILim1, PARM(WBGmIMaxLim));
        SET_FV(WBGmILim2, PARM(WBGmIMaxLim));


    }
    SET_FV(GmILimDstCmp,     PARM(WBGmILim1)-PARM(WBGmIFnd1));


    WBGmDynSlp           = NoNANLim(( PARM(WBGmILim1)         -  PARM(WBGmILim2) )          / (PARM(WBGmITemp1)       - PARM(WBGmITemp2)),         1.0e6F, -1.0e6F);


}



///////////////////////////////////////////////////////////////////////////////
// Function:         GenAndCableTempCalcT3();
// Execution Level:  Task 3
// Execution Time:   Not measured
//
//
// Abstract:         This function is used to find the maximum allowed rotor current
//
// Engineering Diagram:
//
///////////////////////////////////////////////////////////////////////////////
void  GenAndCableTempCalcT3()
{

    float SpdTemp;


    if ( (AirTempGnSensOk && !IpcInterfaceDown) && (SysTmSec > 5) )
    {
       AirTempGnSensAct = true;  //display
       AirTempGenInFil  = AirTempGenInFil + AirTempGenGn*( AirTempGenInlet - AirTempGenInFil);
       AirTempGenIn     = AirTempGenInFil;
    }
    else
    {
       AirTempGnSensAct = false;
       AirTempGenInFil  = DnTwrTemp; //precondition for use when interface is ok
       AirTempGenIn     = DnTwrTemp;
    }

    AirTempGenClmp = AirTempGenIn;

    if ( AirTempGenClmp >= PARM(GenILimTempMax))
    {
        AirTempGenClmp = PARM(GenILimTempMax);
    }
    else if ( AirTempGenClmp <= PARM(GenILimTempMin))
    {
        AirTempGenClmp = PARM(GenILimTempMin);
    }

    AirTempGenInSqr = AirTempGenClmp * AirTempGenClmp;


    if ( (AirTempDlSensOk && !IpcInterfaceDown) && (SysTmSec > 5))
    {
       AirTempDlSensAct = true;  //display
       AirTempDlFil     = AirTempDlFil + AirTempDlGn*( AirTempDripLoop - AirTempDlFil);
       AirTempGenCab    = AirTempDlFil;
    }
    else
    {
       AirTempDlSensAct = false;
       AirTempDlFil     = DnTwrTemp;
       AirTempGenCab    = DnTwrTemp + PARM(S_TempCabDel);
    }


    SpdTemp = R_SpdFbkT3 * RAD_TO_RPM;
    if ( SpdTemp < PARM(R_RPMMinOp))
    {
        SpdILim = PARM(R_RPMMinOp);

    }
    else if (SpdTemp > PARM(R_OvrSpdThr))
    {
        SpdILim = PARM(R_OvrSpdThr);
    }
    else
    {
        SpdILim = SpdTemp;
    }

    SpdILimSqr  =  SpdILim    * SpdILim;
    SpdILimCub  =  SpdILimSqr * SpdILim;
    SpdILimBqu  =  SpdILimCub * SpdILim;


}

///////////////////////////////////////////////////////////////////////////////
// Function:         GenAndCableTempRules);
// Execution Level:  Task 3
// Execution Time:   Not measured
//
//
// Abstract:         This function is used to find the maximum allowed rotor current
//
// Engineering Diagram:
//
///////////////////////////////////////////////////////////////////////////////
void  GenAndCableTempRules()
{

    SET_FV(S_TempCabDel, 0.0F);
    SET_FV(R_TempCabDel, 0.0F);

    SET_FV(AirTempGenW,WBTocTauRcp);
    AirTempGenGn = PARM(AirTempGenW)*DelTm3;

    SET_FV(AirTempDlW, WBTocTauRcp);
    AirTempDlGn = PARM(AirTempDlW)*DelTm3;

}


///////////////////////////////////////////////////////////////////////////////
// Function:         RotorCurrentLimitCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
//
//
// Abstract:         This function is used to find the maximum allowed rotor current
//
// Engineering Diagram:
//
///////////////////////////////////////////////////////////////////////////////
void  RotorCurrentLimitCalcT3(void)
{
    float  DelInt;
    double ipart;              /* Integer part of floating point number */
    float R_IMaxGnx;

    //////////////////////////////////////////
    // Rotor speed - current profile
    //////////////////////////////////////////

    if (SysTmSec > 5)
    {

        //New genRotor Speed,Temperature and Altitude dependent Current Limit, SZ, 2016-01-27

        //Old polynomial
        //I(T,N,A) = (mt*T+Bt)*[(mN*N+Bn)/(mN*Nm+bN)]*(mA*A+bA)
        //T = temperature, N = generator speed, A = altitude
        /*R_IMaxCrvGen    = ( (PARM(R_ISpdTempMt) * AirTempGenIn) + PARM(R_ISpdTempBt))  *
                          ( ( (PARM(R_ISpdTempMn) * SpdILim) + PARM(R_ISpdTempBn)) * R_ISpdTmpRcp)  *
                          PARM(R_ISpdAlttdK);
        */

        //New polynomial, 2016-1-27, SZ
        //I(T,N,A) = (A1*N+A2*T+A3*N^3+A4*N^2+A5*N^3*T+A6*N^2*T+A7*N*T+A8*N^3*T^2+A9*N^2*T^2+A10*N*T^2+A11*N^4*T+A12*N^4*T^2+A13)*(mA*A+bA)
        //T = temperature, N = generator speed, A = altitude
        R_IMaxCrvGen    = ( PARM(R_ISpdTempA1)   * SpdILim          +
                            PARM(R_ISpdTempA2)   * AirTempGenClmp   +
                            PARM(R_ISpdTempA3)   * SpdILimCub       +
                            PARM(R_ISpdTempA4)   * SpdILimSqr       +
                            PARM(R_ISpdTempA5)   * SpdILimCub   * AirTempGenClmp  +
                            PARM(R_ISpdTempA6)   * SpdILimSqr   * AirTempGenClmp  +
                            PARM(R_ISpdTempA7)   * SpdILim      * AirTempGenClmp  +
                            PARM(R_ISpdTempA8)   * SpdILimCub   * AirTempGenInSqr +
                            PARM(R_ISpdTempA9)   * SpdILimSqr   * AirTempGenInSqr +
                            PARM(R_ISpdTempA10)  * SpdILim      * AirTempGenInSqr +
                            PARM(R_ISpdTempA11)  * SpdILimBqu   * AirTempGenClmp  +
                            PARM(R_ISpdTempA12)  * SpdILimBqu   * AirTempGenInSqr +
                            PARM(R_ISpdTempA13)) *
                            PARM(R_ISpdAlttdK);

        if ( R_IMaxCrvGen >= PARM(R_ImaxCrvGenMax))
        {
            R_IMaxCrvGen = PARM(R_ImaxCrvGenMax);
        }

        //Generator  converter rotor Altitude Temperature Dependent Current Limit
        if(DnTwrTemp>PARM(RtrILimTempMax))
        {
            RtrILimTemp = PARM(RtrILimTempMax);
        }
        else if(DnTwrTemp<PARM(RtrILimTempMin))
        {
            RtrILimTemp = PARM(RtrILimTempMin);
        }
        else
        {
            RtrILimTemp = DnTwrTemp;
        }
        RtrILimCnv1km =   PARM(RtrILimDeRtK1)
                        + PARM(RtrILimDeRtX1)*RtrILimTemp
                        + PARM(RtrILimDeRtX2)*RtrILimTemp*RtrILimTemp
                        + PARM(RtrILimDeRtX3)*RtrILimTemp*RtrILimTemp*RtrILimTemp;

        WBRtrILimCnv = RtrILimCnv1km*PARM(RtrILimAltSf) + PARM(RtrILimDstCmp);

        //Rotor cable Temperature Dependent Current Limit
        // cable temperature is 3 deg C over the ambient
        if ( AirTempGenCab <= PARM(R_TempCable1))
        {
            R_IMaxCrvCab = PARM(R_ILimCab1);
        }
        else
        {
            R_IMaxCrvCab =  (AirTempGenCab - PARM(R_TempCable1)) * R_IlimSlopeC + PARM(R_ILimCab1);
        }


        R_IMaxGnx      =  R_IMaxGnDyn;

    }
    else
    {
        R_IMaxGnx      =  1.0F;
        WBRtrILimCnv   = PARM(WBRtrILim1);

        //Rotor

        //Old polynomial
        //R_IMaxCrvGen    = ( (PARM(R_ISpdTempMt) * 25.0) + PARM(R_ISpdTempBt) ) *
        //                  ( ( (PARM(R_ISpdTempMn) * PARM(R_RPMMinOp)) + PARM(R_ISpdTempBn)) * R_ISpdTmpRcp) ;

        //New polynomial, 2016-1-27, SZ
        R_IMaxCrvGen    = ( PARM(R_ISpdTempA1)  * PARM(R_RPMMinOp) +
                            PARM(R_ISpdTempA2)  * PARM(R_ISpdTempNomT) +
                            PARM(R_ISpdTempA3)  * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) +
                            PARM(R_ISpdTempA4)  * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) +
                            PARM(R_ISpdTempA5)  * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) +
                            PARM(R_ISpdTempA6)  * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) +
                            PARM(R_ISpdTempA7)  * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) +
                            PARM(R_ISpdTempA8)  * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) * PARM(R_ISpdTempNomT) +
                            PARM(R_ISpdTempA9)  * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) * PARM(R_ISpdTempNomT) +
                            PARM(R_ISpdTempA10) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) * PARM(R_ISpdTempNomT) +
                            PARM(R_ISpdTempA11) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) +
                            PARM(R_ISpdTempA12) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) * PARM(R_ISpdTempNomT) +
                            PARM(R_ISpdTempA13));

        if ( R_IMaxCrvGen >= PARM(R_ImaxCrvGenMax))
        {
            R_IMaxCrvGen = PARM(R_ImaxCrvGenMax);
        }

        R_IMaxCrvCab   =  PARM(R_ILimCab1);

    }


    //Final Curve based minimum select for stator DTA, generator stator and stator cable
    if (R_IMaxCrvGen < WBRtrILimCnv)
    {
        R_IMaxCrvMin = R_IMaxCrvGen;
        PwrLimRtrCurSel = PWRLIM_RTRCUR_GEN;
    }
    else
    {
        R_IMaxCrvMin = WBRtrILimCnv;
        PwrLimRtrCurSel = PWRLIM_RTRCUR_CNV;
    }
    if (R_IMaxCrvCab < R_IMaxCrvMin)
    {
        R_IMaxCrvMin = R_IMaxCrvCab;
        PwrLimRtrCurSel = PWRLIM_RTRCUR_CAB;
    }

    //-------------------------------------------------
    // Generator rotor currrent magnitude limiter path


    // Need extra precision here
    R_IMaxCrvSq   = R_IMaxCrvMin*R_IMaxCrvMin;
    DelInt        = R_IMaxGnx * (R_IMaxCrvSq - R_MxCurOutSq);
    R_MaxCurSqFrc = modf( (R_MaxCurSqFrc + DelInt), &ipart);
    R_MaxCurSqInt = R_MaxCurSqInt + ipart;
    R_MxCurOutSq  = R_MaxCurSqInt + R_MaxCurSqFrc;
    //R_MxCurOutSq = R_MxCurOutSq + R_IMaxGnx*(R_IMaxCrvSq - R_MxCurOutSq);

    // clamp to max allowed current
    if (R_MxCurOutSq > R_IMaxLimSq )
    {
        R_MxCurOutSq  = R_IMaxLimSq;
        R_MaxCurSqInt = (int)R_MxCurOutSq;
        R_MaxCurSqFrc = 0.0;
    }

    R_MaxCurOut  = sqrtf(R_MxCurOutSq);

    //Rotor currrent magnitude limiter path
    RtrILim    = R_MaxCurOut;
    RtrIMrgSqr = (RtrILim * RtrILim) - R_TocHiSelSq;
    RtrILimOut = PARM(RtrILimGn) * RtrIMrgSqr;
    RtrILimMrg = RtrILim - R_TocHiSel;


}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      RotorCurrentLimitRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void RotorCurrentLimitRules()
{

    float Temp1;
    float TocThrsVar;
    float R_ISpdKTemp;
    float TmpTocThrs;
    float TmpMaxCur;
    float TmpILimGn;


    SET_FV(R_IDistTOC, 48.61F);
    SET_FV(R_ProtClass,  10.0F);

    //RtrILimGn
    TmpMaxCur   = PARM(R_IMaxLim);
    TmpTocThrs  = PARM(R_TocMargin)* TmpMaxCur;
    TmpILimGn  = R_TrqRat / (TmpTocThrs * TmpTocThrs - TmpMaxCur * TmpMaxCur);
    SET_FV(RtrILimGn, TmpILimGn);

    SET_FV(R_TocTau, PARM(WBTocTauNorm));
    R_WTocT3    = DelTm3/PARM(R_TocTau);
    R_WTocFstT3 = (DelTm3 * PARM(TocTauTstScl))/PARM(R_TocTau);

    SET_FV(R_IMaxTau, PARM(R_TocTau));
    R_IMaxGn    = DelTm3/PARM(R_IMaxTau);
    R_IMaxFstGn = (DelTm3 * PARM(TocTauTstScl))/PARM(R_IMaxTau);

    if ( (PARM(SoftwareIPN) == DFIG02_1500_5060_A) || (PARM(SoftwareIPN) == DFIG02_1500_5060_B) )
    {
        if(PARM(TestMode) != PUMP_BACK)
        {
            SET_FV(R_MaxCurAdj, 650.0F);
            SET_FV(R_IRatSSLim, 640.0F);
            SET_FV(R_IRat,      640.0F);
        }
        else
        {
            SET_FV(R_MaxCurAdj, 2400.0F);
            SET_FV(R_IRatSSLim, 2400.0F);
            SET_FV(R_IRat,      2400.0F);
        }

        SET_FV(R_TocMargin, 1.05F);
    }
    else if ( (PARM(SoftwareIPN) == DFIG02_1850_5060_C) || (PARM(SoftwareIPN) == DFIG02_1850_5060_D) || (PARM(SoftwareIPN) == DFIG02_1850_5060_E))
    {
        if(PARM(TestMode) != PUMP_BACK)
        {
            SET_FV(R_MaxCurAdj, 650.0F);
            SET_FV(R_IRatSSLim, 640.0F);
            SET_FV(R_IRat,      640.0F);
        }
        else
        {
            SET_FV(R_MaxCurAdj, 2400.0F);
            SET_FV(R_IRatSSLim, 2400.0F);
            SET_FV(R_IRat,      2400.0F);
        }
        SET_FV(R_TocMargin, 1.05F);

    }

    else //( (PARM(SoftwareIPN) == DFIG02_2400_5060_A)
    {
        if(PARM(TestMode) != PUMP_BACK)
        {
            SET_FV(R_MaxCurAdj, 750.0F);
            SET_FV(R_IRatSSLim, 750.0F);
            SET_FV(R_IRat,      750.0F);
        }
        else
        {
            SET_FV(R_MaxCurAdj, 2400.0F);
            SET_FV(R_IRatSSLim, 2400.0F);
            SET_FV(R_IRat,      2400.0F);
        }
        SET_FV(R_TocMargin, 1.07F);

    }
    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
        SET_FV(RtrILimDeRtK1,   640.0F);
        SET_FV(RtrILimDeRtX1,   0.0F);
        SET_FV(RtrILimDeRtX2,   0.0F);
        SET_FV(RtrILimDeRtX3,   0.0F);
        SET_FV(RtrILimAltLpse,  1.00F);
        break;
    case DFIG02_1850_5060_E:
        SET_FV(RtrILimDeRtK1,   640.0F);
        SET_FV(RtrILimDeRtX1,   0.0F);
        SET_FV(RtrILimDeRtX2,   0.0F);
        SET_FV(RtrILimDeRtX3,   0.0F);
        SET_FV(RtrILimAltLpse,  0.93F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(RtrILimDeRtK1,   1997.5F);
        SET_FV(RtrILimDeRtX1,   -101.75F);
        SET_FV(RtrILimDeRtX2,   3.0F);
        SET_FV(RtrILimDeRtX3,   -0.03F);
        SET_FV(RtrILimAltLpse,  0.94F);
        break;
    case DFIG02_2400_5060_B:
        SET_FV(RtrILimDeRtK1,   1997.5F);
        SET_FV(RtrILimDeRtX1,   -101.75F);
        SET_FV(RtrILimDeRtX2,   3.0F);
        SET_FV(RtrILimDeRtX3,   -0.03F);
        SET_FV(RtrILimAltLpse,  0.94F);
        break;
    }

    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
    case DFIG02_1850_5060_E:
        SET_FV(RtrILimTempMin,  35.0F);
        SET_FV(RtrILimTempMax,  55.0F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(RtrILimTempMin,  25.0F);
        SET_FV(RtrILimTempMax,  55.0F);
        break;
    case DFIG02_2400_5060_B:
        SET_FV(RtrILimTempMin,  25.0F);
        SET_FV(RtrILimTempMax,  55.0F);
        break;
    }
    SET_FV(RtrILimAltDeRtX1,    -(1.0F-PARM(RtrILimAltLpse))*0.001F);
    SET_FV(RtrILimAltDeRtX2,    0.0F);
    SET_FV(RtrILimAltDeRtK1,    1.0F-PARM(RtrILimAltDeRtX1)*PARM(AltitudeRat)-PARM(RtrILimAltDeRtX2)*PARM(AltitudeRat)*PARM(AltitudeRat));
    float tempRtrILimAltSf;
    tempRtrILimAltSf = PARM(RtrILimAltDeRtK1)+PARM(RtrILimAltDeRtX1)*AltitudeClmp+PARM(RtrILimAltDeRtX2)*AltitudeClmp*AltitudeClmp;
    if(tempRtrILimAltSf>1.0F)
    {
        SET_FV(RtrILimAltSf, 1.0F);
    }
    else
    {
        SET_FV(RtrILimAltSf, tempRtrILimAltSf);
    }

    //Generator speed altitude dependent current limit constant
    R_ISpdKTemp = PARM(R_ISpdTempBa) + (PARM(R_ISpdTempMa) * MAX(0.0, PARM(AltitudeTurb)) );
    if ( ( R_ISpdKTemp > 1.0) )
    {
        R_ISpdKTemp = 1.0F;
    }
    SET_FV(R_ISpdAlttdK, R_ISpdKTemp);

    // use the minimum temperature to calculate the IFnd1
    SET_FV(WBRtrIFnd1, PARM(RtrILimAltSf)*
                     (PARM(RtrILimDeRtK1)
                    + PARM(RtrILimDeRtX1)*PARM(RtrILimTempMin)
                    + PARM(RtrILimDeRtX2)*PARM(RtrILimTempMin)*PARM(RtrILimTempMin)
                    + PARM(RtrILimDeRtX3)*PARM(RtrILimTempMin)*PARM(RtrILimTempMin)*PARM(RtrILimTempMin)));
    // use the maximum temperature to calculate the IFnd2
    SET_FV(WBRtrIFnd2, PARM(RtrILimAltSf)*
                     (PARM(RtrILimDeRtK1)
                    + PARM(RtrILimDeRtX1)*PARM(RtrILimTempMax)
                    + PARM(RtrILimDeRtX2)*PARM(RtrILimTempMax)*PARM(RtrILimTempMax)
                    + PARM(RtrILimDeRtX3)*PARM(RtrILimTempMax)*PARM(RtrILimTempMax)*PARM(RtrILimTempMax)));

    // Generator rotor temp and current limits with altitude de-rating
    SET_FV(WBRtrITemp1, PARM(WBTemp));                 //this one varies
    SET_FV(WBRtrITemp2, PARM(WBIRatTempHi));

    SET_FV(R_ICnvNameP, MIN(PARM(R_IRatSSLim), PARM(R_MaxCurAdj)) );

    SET_FV(WBRtrILim1, sqrtf( (PARM(WBRtrIFnd1) * PARM(WBRtrIFnd1)) + (PARM(R_IDistTOC) * PARM(R_IDistTOC) ) ) );
    SET_FV(WBRtrILim2, sqrtf( (PARM(WBRtrIFnd2) * PARM(WBRtrIFnd2)) + (PARM(R_IDistTOC) * PARM(R_IDistTOC) ) ) );
    SET_FV(RtrILimDstCmp,   PARM(WBRtrILim1)-PARM(WBRtrIFnd1));



    SET_FV(R_ILimCab1, PARM(R_ILimA1Cab1) * CabAltDeRtSF);
    SET_FV(R_ILimCab2, PARM(R_ILimA1Cab2) * CabAltDeRtSF);
    R_IlimSlopeC   =  ( PARM(R_ILimCab2) -   PARM(R_ILimCab1))   / (PARM(R_TempCable2) -   PARM(R_TempCable1));
    // project lines defined by (TempCable2, ILimCab2) and (TempCable1, ILimCab1) to (WBTemp, ILimCab0)
    // rotor cables
    if ( PARM(WBTemp) >= PARM(R_TempCable1))
    {
        Temp1 = PARM(R_ILimCab1);
    }
    else
    {
        Temp1 =  (PARM(WBTemp) - PARM(R_TempCable1)) * R_IlimSlopeC + PARM(R_ILimCab1);
    }
    SET_FV(R_ILimCab0, Temp1);

    TocThrsVar = 1.05 * sqrtf( (PARM(R_IDistTOC) * PARM(R_IDistTOC) ) + (PARM(R_ICnvNameP) * PARM(R_ICnvNameP)) );
    TocThrsVar = MIN(TocThrsVar, ( PARM(R_TocMargin) * PARM(WBRtrILim1) ) );
    TocThrsVar = MIN(TocThrsVar, PARM(R_ILimCab1) );
    SET_FV(R_TocThrs,   TocThrsVar);
    SET_FV(R_TocAlmOn,  PARM(R_IRatSSLim) * TOC_ALM_SCL );
    SET_FV(R_TocAlmOff, PARM(R_IRatSSLim));
    SET_FV(R_IMaxLim,   TocThrsVar/PARM(R_TocMargin) );
    R_IMaxLimSq = PARM(R_IMaxLim)*PARM(R_IMaxLim);


    R_TocAlmOnDl  = PARM(R_TocThrs)  - PARM(R_TocAlmOn);
    R_TocAlmOffDl = PARM(R_TocAlmOn) - PARM(R_TocAlmOff);

    WBRtrDynSlp          = NoNANLim(( PARM(WBRtrILim1)        -  PARM(WBRtrILim2) )         / (PARM(WBRtrITemp1)      - PARM(WBRtrITemp2)),        1.0e6F, -1.0e6F);



}



///////////////////////////////////////////////////////////////////////////////
// Function:         StatorCurrentLimitCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
//
//
// Abstract:         This function is used to find the maximum allowed stator current
//
// Engineering Diagram:
//
///////////////////////////////////////////////////////////////////////////////
void  StatorCurrentLimitCalcT3(void)
{

    float  DelInt;
    double ipart;              /* Integer part of floating point number */
    float S_IMaxGnx;

    //////////////////////////////////////////
    // Rotor/Stator speed - current profile
    //////////////////////////////////////////

    if (SysTmSec > 5)
    {


        //Gen Stator Speed,Temperature dependent Current Limit
        //Old polynomial
        // I(T,N,A) = (mt*T+Bt)*[(mN*N+Bn)/(mN*Nm+bN)]*(mA*A+bA)
        //T = temperature, N = generator speed, A = altitude
        /*S_IMaxCrvGen    = ( (PARM(S_ISpdTempMt) * AirTempGenIn) + PARM(S_ISpdTempBt))  *
                          ( ( (PARM(S_ISpdTempMn) * SpdILim) + PARM(S_ISpdTempBn)) * S_ISpdTmpRcp) *
                          PARM(R_ISpdAlttdK);
        */

        //New polynomial, 2016-1-27, SZ
        //I(T,N,A) = (A1*N+A2*T+A3*N^3+A4*N^2+A5*N^3*T+A6*N^2*T+A7*N*T+A8*N^3*T^2+A9*N^2*T^2+A10*N*T^2+A11*N^4*T+A12*N^4*T^2+A13)*(mA*A+bA)
        //T = temperature, N = generator speed, A = altitude
        S_IMaxCrvGen    = ( PARM(S_ISpdTempA1)   * SpdILim              +
                            PARM(S_ISpdTempA2)   * AirTempGenClmp +
                            PARM(S_ISpdTempA3)   * SpdILimCub   +
                            PARM(S_ISpdTempA4)   * SpdILimSqr   +
                            PARM(S_ISpdTempA5)   * SpdILimCub   * AirTempGenClmp    +
                            PARM(S_ISpdTempA6)   * SpdILimSqr   * AirTempGenClmp    +
                            PARM(S_ISpdTempA7)   * SpdILim      * AirTempGenClmp    +
                            PARM(S_ISpdTempA8)   * SpdILimCub   * AirTempGenInSqr +
                            PARM(S_ISpdTempA9)   * SpdILimSqr   * AirTempGenInSqr +
                            PARM(S_ISpdTempA10)  * SpdILim      * AirTempGenInSqr +
                            PARM(S_ISpdTempA11)  * SpdILimBqu   * AirTempGenClmp    +
                            PARM(S_ISpdTempA12)  * SpdILimBqu   * AirTempGenInSqr +
                            PARM(S_ISpdTempA13)) *
                            PARM(R_ISpdAlttdK);

        if ( S_IMaxCrvGen >= PARM(S_ImaxCrvGenMax))
        {
            S_IMaxCrvGen = PARM(S_ImaxCrvGenMax);
        }

        //Generator converter stator Altitude Temperature Dependent Current Limit
        if(DnTwrTemp>PARM(StrILimTempMax))
        {
            StrILimTemp = PARM(StrILimTempMax);
        }
        else if(DnTwrTemp<PARM(StrILimTempMin))
        {
            StrILimTemp = PARM(StrILimTempMin);
        }
        else
        {
            StrILimTemp = DnTwrTemp;
        }
        StrILimCnv1km =   PARM(StrILimDeRtK1)
                        + PARM(StrILimDeRtX1)*StrILimTemp
                        + PARM(StrILimDeRtX2)*StrILimTemp*StrILimTemp
                        + PARM(StrILimDeRtX3)*StrILimTemp*StrILimTemp*StrILimTemp;

        WBStrILimCnv = StrILimCnv1km*PARM(StrILimAltSf) + PARM(StrILimDstCmp);

        //Stator cable Temperature Dependent Current Limit
        if ( AirTempGenCab <= PARM(S_TempCable1))
        {
            S_IMaxCrvCab = PARM(S_ILimCab1);
        }
        else
        {
            S_IMaxCrvCab = (AirTempGenCab - PARM(S_TempCable1)) * S_IlimSlopeC + PARM(S_ILimCab1);
        }


        S_IMaxGnx      =  S_IMaxGnDyn;

    }
    else
    {
        S_IMaxGnx      =  1.0F;
        WBStrILimCnv   = PARM(WBStrILim1);

        //Old polynomial
        //S_IMaxCrvGen    = ( (PARM(S_ISpdTempMt) * 25.0) + PARM(S_ISpdTempBt) ) *
        //                  ( ( (PARM(S_ISpdTempMn) * PARM(R_RPMMinOp)) + PARM(S_ISpdTempBn)) * S_ISpdTmpRcp) ;

        //New polynomial, 2016-1-27, SZ
        S_IMaxCrvGen    = ( PARM(S_ISpdTempA1)  * PARM(R_RPMMinOp) +
                            PARM(S_ISpdTempA2)      * PARM(R_ISpdTempNomT) +
                            PARM(S_ISpdTempA3)      * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) +
                            PARM(S_ISpdTempA4)      * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) +
                            PARM(S_ISpdTempA5)      * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) +
                            PARM(S_ISpdTempA6)      * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) +
                            PARM(S_ISpdTempA7)      * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) +
                            PARM(S_ISpdTempA8)      * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) * PARM(R_ISpdTempNomT) +
                            PARM(S_ISpdTempA9)      * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) * PARM(R_ISpdTempNomT) +
                            PARM(S_ISpdTempA10) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) * PARM(R_ISpdTempNomT) +
                            PARM(S_ISpdTempA11) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) +
                            PARM(S_ISpdTempA12) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_RPMMinOp) * PARM(R_ISpdTempNomT) * PARM(R_ISpdTempNomT) +
                            PARM(S_ISpdTempA13));

        if ( S_IMaxCrvGen >= PARM(S_ImaxCrvGenMax))
        {
            S_IMaxCrvGen = PARM(S_ImaxCrvGenMax);
        }

        S_IMaxCrvCab   = PARM(S_ILimCab1);
    }

    //Final Curve based minimum select for stator DTA, generator stator and stator cable
    if (S_IMaxCrvGen < WBStrILimCnv)
    {
        S_IMaxCrvMin = S_IMaxCrvGen;
        PwrLimStrCurSel = PWRLIM_STRCUR_GEN;
    }
    else
    {
        S_IMaxCrvMin = WBStrILimCnv;
        PwrLimStrCurSel = PWRLIM_STRCUR_CNV;
    }
    if (S_IMaxCrvCab < S_IMaxCrvMin)
    {
        S_IMaxCrvMin = S_IMaxCrvCab;
        PwrLimStrCurSel = PWRLIM_STRCUR_CAB;
    }

    // Generator stator currrent magnitude limiter path

    // clamp to max allowed current
    S_IMaxCrvSq  = S_IMaxCrvMin*S_IMaxCrvMin;
    // Need extra precision here
    DelInt        = S_IMaxGnx * (S_IMaxCrvSq - S_MxCurOutSq);
    S_MaxCurSqFrc = modf( (S_MaxCurSqFrc + DelInt), &ipart);
    S_MaxCurSqInt = S_MaxCurSqInt + ipart;
    S_MxCurOutSq  = S_MaxCurSqInt + S_MaxCurSqFrc;

    if (S_MxCurOutSq > S_IMaxLimSq)
    {
        S_MxCurOutSq = S_IMaxLimSq;
        S_MaxCurSqInt = (int)S_MxCurOutSq;
        S_MaxCurSqFrc = 0.0;
    }


    S_MaxCurOut  = sqrtf(S_MxCurOutSq);

    StrILim    = S_MaxCurOut;
    StrIMrgSqr = (StrILim * StrILim) - S_TocHiSelSq;
    StrILimOut = PARM(StrILimGn) * StrIMrgSqr;
    StrILimMrg = StrILim - S_TocHiSel;



}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      StatorCurrentLimitRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void StatorCurrentLimitRules()
{
    float TocThrsVar;
    float Temp1;
    float TmpTocThrs;
    float TmpMaxCur;
    float TmpILimGn;


    SET_FV(S_IDistTOC, 62.61F);

    //StrILimGn
    TmpMaxCur   = PARM(S_IMaxLim);
    TmpTocThrs  = PARM(S_TocMargin)* TmpMaxCur;
    TmpILimGn  = R_TrqRat / (TmpTocThrs * TmpTocThrs - TmpMaxCur * TmpMaxCur);
    SET_FV(StrILimGn, TmpILimGn);


    SET_FV(S_TocTau, PARM(WBTocTauNorm));
    S_WTocT3    = DelTm3/PARM(S_TocTau);
    S_WTocFstT3 = (DelTm3 * PARM(TocTauTstScl))/PARM(S_TocTau);

    SET_FV(S_IMaxTau, PARM(S_TocTau));
    S_IMaxGn    = DelTm3/PARM(S_IMaxTau);
    S_IMaxFstGn = (DelTm3 * PARM(TocTauTstScl))/PARM(S_IMaxTau);


    SET_FV(WBStrSpilMrg, 15.0);//These two used in A0FlxCmd
    SET_FV(WBStrSpilGn, 13.333333); //the product of these should be 200amps of Iy on spillover...eneough



    if ( (PARM(SoftwareIPN) == DFIG02_1500_5060_A) || (PARM(SoftwareIPN) == DFIG02_1500_5060_B) )
    {
        if(PARM(TestMode) != PUMP_BACK)
        {
           SET_FV(S_IRatSSLim, 1400.0F);
           SET_FV(S_MaxCurAdj, 1410.0F);
        }
        else
        {
           SET_FV(S_IRatSSLim, 3400.0F);
           SET_FV(S_MaxCurAdj, 3410.0F);
        }
        SET_FV(S_TocMargin, 1.05F);

    }
    else if ( (PARM(SoftwareIPN) == DFIG02_1850_5060_C) || (PARM(SoftwareIPN) == DFIG02_1850_5060_D) || (PARM(SoftwareIPN) == DFIG02_1850_5060_E))
    {
        if(PARM(TestMode) != PUMP_BACK)
        {
           SET_FV(S_IRatSSLim, 1425.0F);
           SET_FV(S_MaxCurAdj, 1435.0F);
        }
        else
        {
           SET_FV(S_IRatSSLim, 3400.0F);
           SET_FV(S_MaxCurAdj, 3435.0F);
        }

        SET_FV(S_TocMargin, 1.05F);
    }

    else //( (PARM(SoftwareIPN) == DFIG02_2400_5060_A)
    {
        if(PARM(TestMode) != PUMP_BACK)
        {
            SET_FV(S_MaxCurAdj, 1900.0F);
            SET_FV(S_IRatSSLim, 1900.0F);
        }
        else
        {
            SET_FV(S_MaxCurAdj, 2400.0F);
            SET_FV(S_IRatSSLim, 2400.0F);
        }
        SET_FV(S_TocMargin, 1.05F);

    }
    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
        SET_FV(StrILimDeRtK1,   1400.0F);
        SET_FV(StrILimDeRtX1,   0.0F);
        SET_FV(StrILimDeRtX2,   0.0F);
        SET_FV(StrILimDeRtX3,   0.0F);
        SET_FV(StrILimAltLpse,  1.00F);
        break;
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
        SET_FV(StrILimDeRtK1,   1425.0F);
        SET_FV(StrILimDeRtX1,   0.0F);
        SET_FV(StrILimDeRtX2,   0.0F);
        SET_FV(StrILimDeRtX3,   0.0F);
        SET_FV(StrILimAltLpse,  1.00F);
        break;
    case DFIG02_1850_5060_E:
        SET_FV(StrILimDeRtK1,   1425.0F);
        SET_FV(StrILimDeRtX1,   0.0F);
        SET_FV(StrILimDeRtX2,   0.0F);
        SET_FV(StrILimDeRtX3,   0.0F);
        SET_FV(StrILimAltLpse,  0.94F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(StrILimDeRtK1,   2200.0F);
        SET_FV(StrILimDeRtX1,   -15.6667F);
        SET_FV(StrILimDeRtX2,   0.8F);
        SET_FV(StrILimDeRtX3,   -0.01333F);
        SET_FV(StrILimAltLpse,  0.945F);
        break;
    case DFIG02_2400_5060_B:
        SET_FV(StrILimDeRtK1,   2200.0F);
        SET_FV(StrILimDeRtX1,   -15.6667F);
        SET_FV(StrILimDeRtX2,   0.8F);
        SET_FV(StrILimDeRtX3,   -0.01333F);
        SET_FV(StrILimAltLpse,  0.945F);
        break;
    }
    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
    case DFIG02_1850_5060_E:
        SET_FV(StrILimTempMin,  35.0F);
        SET_FV(StrILimTempMax,  55.0F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(StrILimTempMin,  25.0F);
        SET_FV(StrILimTempMax,  55.0F);
        break;
    case DFIG02_2400_5060_B:
        SET_FV(StrILimTempMin,  25.0F);
        SET_FV(StrILimTempMax,  55.0F);
        break;
    }
    SET_FV(StrILimAltDeRtX1,    -(1-PARM(StrILimAltLpse))*0.001);
    SET_FV(StrILimAltDeRtX2,    0.0F);
    SET_FV(StrILimAltDeRtK1,    1.0F-PARM(StrILimAltDeRtX1)*PARM(AltitudeRat)-PARM(StrILimAltDeRtX2)*PARM(AltitudeRat)*PARM(AltitudeRat));
    float tempStrILimAltSf;
    tempStrILimAltSf = PARM(StrILimAltDeRtK1)+PARM(StrILimAltDeRtX1)*AltitudeClmp+PARM(StrILimAltDeRtX2)*AltitudeClmp*AltitudeClmp;
    if(tempStrILimAltSf>1.0F)
    {
        SET_FV(StrILimAltSf, 1.0F);
    }
    else
    {
        SET_FV(StrILimAltSf, tempStrILimAltSf);
    }

    // use the minimum temperature to calculate the IFnd1
    SET_FV(WBStrIFnd1, PARM(StrILimAltSf)*
                     (PARM(StrILimDeRtK1)
                    + PARM(StrILimDeRtX1)*PARM(StrILimTempMin)
                    + PARM(StrILimDeRtX2)*PARM(StrILimTempMin)*PARM(StrILimTempMin)
                    + PARM(StrILimDeRtX3)*PARM(StrILimTempMin)*PARM(StrILimTempMin)*PARM(StrILimTempMin)));
    // use the maximum temperature to calculate the IFnd2
    SET_FV(WBStrIFnd2, PARM(StrILimAltSf)*
                     (PARM(StrILimDeRtK1)
                    + PARM(StrILimDeRtX1)*PARM(StrILimTempMax)
                    + PARM(StrILimDeRtX2)*PARM(StrILimTempMax)*PARM(StrILimTempMax)
                    + PARM(StrILimDeRtX3)*PARM(StrILimTempMax)*PARM(StrILimTempMax)*PARM(StrILimTempMax)));
    // Generator stator temp and current limits with altitude de-rating
    SET_FV(WBStrITemp1, PARM(WBTemp));                 //this one varies
    SET_FV(WBStrITemp2, PARM(WBIRatTempHi));           //normally 50 deg

    SET_FV(S_ICnvNameP, MIN(PARM(S_IRatSSLim), PARM(S_MaxCurAdj)) );

    SET_FV(WBStrILim1, sqrtf( (PARM(WBStrIFnd1) * PARM(WBStrIFnd1)) + (PARM(S_IDistTOC) * PARM(S_IDistTOC) ) ) );
    SET_FV(WBStrILim2, sqrtf( (PARM(WBStrIFnd2) * PARM(WBStrIFnd2)) + (PARM(S_IDistTOC) * PARM(S_IDistTOC) ) ) );
    SET_FV(StrILimDstCmp,   PARM(WBStrILim1)-PARM(WBStrIFnd1));
    SET_FV(S_ILimCab1, PARM(S_ILimA1Cab1) * CabAltDeRtSF);
    SET_FV(S_ILimCab2, PARM(S_ILimA1Cab2) * CabAltDeRtSF);
    S_IlimSlopeC   =  ( PARM(S_ILimCab2) -   PARM(S_ILimCab1))   / (PARM(S_TempCable2) -   PARM(S_TempCable1));

    if ( PARM(WBTemp) >= PARM(S_TempCable1))
    {
        Temp1 = PARM(S_ILimCab1);
    }
    else
    {
        Temp1 =  (PARM(WBTemp) - PARM(S_TempCable1)) * S_IlimSlopeC + PARM(S_ILimCab1);
    }
    SET_FV(S_ILimCab0, Temp1);

    TocThrsVar = 1.05 * sqrtf( (PARM(S_IDistTOC) * PARM(S_IDistTOC) ) + (PARM(S_ICnvNameP) * PARM(S_ICnvNameP)) );
    TocThrsVar = MIN(TocThrsVar, ( PARM(S_TocMargin) * PARM(WBStrILim1) ) );
    TocThrsVar = MIN(TocThrsVar, PARM(S_ILimCab1) );
    SET_FV(S_TocThrs,   TocThrsVar);
    SET_FV(S_TocAlmOn,  PARM(S_IRatSSLim) * TOC_ALM_SCL );
    SET_FV(S_TocAlmOff, PARM(S_IRatSSLim));
    SET_FV(S_IMaxLim,   TocThrsVar/PARM(S_TocMargin) );
    S_IMaxLimSq = PARM(S_IMaxLim)*PARM(S_IMaxLim);

    S_TocAlmOnDl  = PARM(S_TocThrs)  - PARM(S_TocAlmOn);
    S_TocAlmOffDl = PARM(S_TocAlmOn) - PARM(S_TocAlmOff);


    WBStrDynSlp          = NoNANLim(( PARM(WBStrILim1)        -  PARM(WBStrILim2) )         / (PARM(WBStrITemp1)      - PARM(WBStrITemp2)),        1.0e6F, -1.0e6F);






}



///////////////////////////////////////////////////////////////////////////////
// Function:         RotorBasePlateTempCurrentLimitCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
//
//
// Abstract:         This function is used to find the maximum allowed rotor
//                   current allowed as functions of the heat sink imbalance
//                   and heat sink to coolant rise issues.
//
// Engineering Diagram:
//
///////////////////////////////////////////////////////////////////////////////
void  RotorBasePlateTempCurrentLimitCalcT3(void)
{

    if (SysTmSec < 10.0F)
    {
       R1SinkTempRseFil = 5.0F;
       R1ImbTempFil    = R1BthtrImbMax;
    }
    else
    {
       //Rotor heat sink temperature imblance limiter path
       R1SinkTempRse    = R_BrgThrmstr.BthtrTempMax - CoolantTemp;
       R1SinkTempRseFil = R1SinkTempRseFil + RSinkTempRseGn * ( R1SinkTempRse- R1SinkTempRseFil);

       R1TempRseLimMrg  = PARM(RSinkTempRseLim) - R1SinkTempRseFil;
       R1TempRseLimOut  = PARM(RSinkTempRseLimGn) * R1TempRseLimMrg;


       R1BthtrImbMax  = R_BrgThrmstr.BthtrImbMax;
       R1ImbTempFil    = R1ImbTempFil + RImbTempGn * ( R1BthtrImbMax- R1ImbTempFil);


    }

    //-------------------------------------------------
    //R1e heat sink temperature rise limiter path
    R1TempRseLimMrg = PARM(RSinkTempRseLim) - R1SinkTempRseFil;
    R1TempRseLimOut = PARM(RSinkTempRseLimGn) * R1TempRseLimMrg;

    //-------------------------------------------------
    //Rotor heat sink temperature imbalance limiter path
    R1ImbTempLimMrg = PARM(RImbTempLim) - R1ImbTempFil;
    R1ImbTempLimOut = PARM(RImbTempLimGn) * R1ImbTempLimMrg;





}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      RotorBasePlateTempCurrentLimitRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void RotorBasePlateTempCurrentLimitRules()
{

   SET_FV(RImbTempLimGn,      0.2F * R_TrqRat);        //
   SET_FV(RImbTempLim,        12.0F);    // 7/xx/2013 Steve S
   SET_FV(RSinkTempRseLimGn,  0.2F * R_TrqRat);        //
   SET_FV(RSinkTempRseLim,   100.0F);    // set hi to defeat per SS

   RImbTempGn      = PARM(SinkTempDeRtW) * DelTm3;
   RSinkTempRseGn  = PARM(SinkTempDeRtW) * DelTm3;

}

///////////////////////////////////////////////////////////////////////////////
// Function:         RotorCurrentLimitThermisterBasedCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
//
//
// Abstract:         This function is used to find the maximum allowed rotor
//
// Engineering Diagram:
//
///////////////////////////////////////////////////////////////////////////////
void  ThermisterBasedTorqueLimiterCalcT3(void)
{

    //-------------------------------------------------
    //Cfc temperature limiter path
    CfcTempLimMrg = PARM(CfcTempLim) - CfcTemp;
    CfcTempLimOut = PARM(CfcTempLimGn) * CfcTempLimMrg;


    //-------------------------------------------------
    //LQD temperature limiter path
    LqdTempLimMrg = PARM(LqdTempLim) - CoolantTemp;
    LqdTempLimOut = PARM(LqdTempLimGn) * LqdTempLimMrg;

    //-------------------------------------------------
    //K1 bus temperature limiter path
    PdcCabEnTempLimMrg = PARM(PdcCabEntryTempLim) - PdcCabEntryTemp;
    PdcCabEnTempLimOut = PARM(PdcCabEntryTempLimGn) * PdcCabEnTempLimMrg;

    //-------------------------------------------------
    //PDC temperature limiter path
    PdcTempLimMrg = PARM(PdcTempLim) - PdcTemp;
    PdcTempLimOut = PARM(PdcTempLimGn) * PdcTempLimMrg;



}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      ThermisterBasedTorqueLimiterRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void ThermisterBasedTorqueLimiterRules()
{

   SET_FV(CfcTempLim,     PARM(CfcOtThrsh)  - 5.0F);
   SET_FV(CfcTempLimGn,   0.2F * R_TrqRat); // derate from 60deg C and torque is 0 @ 65deg C

   SET_FV(LqdTempLim,     PARM(CoolantOtDlyThrsh) - 5.0F);
   SET_FV(LqdTempLimGn,   8.0e5F);

   SET_FV(PdcCabEntryTempLim,   PARM(PdcCabEnOtThrsh) - 5.0F );

   SET_FV(PdcCabEntryTempLimGn, 0.2F * R_TrqRat);


}

///////////////////////////////////////////////////////////////////////////////
// Function:         DownTowerTemperatureLimiterCalcT3
// Execution Level:  Task 3
// Execution Time:   Not measured
//
//
// Abstract:         This function is used to make an output used in the power
//                   limiter to help reduce down-tower temperature.
//
// Engineering Diagram:
//
///////////////////////////////////////////////////////////////////////////////
void  DownTowerTemperatureLimiterCalcT3(void)
{

    //Down-tower temperature limiter path
   DnTwrTMrg  = PARM(DnTwrTmpRt) - DnTwrTemp;
   DnTwrPLimS = PARM(TempDerate) * DnTwrTMrg;
   DnTwrPLimF = MIN(0.0F, (PARM(DnTwrTempLimGn)*(PARM(DnTwrTmpMx) - DnTwrTemp)) );
   DnTwrTempLmOutPu=DnTwrPLimS + DnTwrPLimF;
   DnTwrTempLmOut  = DnTwrTempLmOutPu*R_TrqRat;

}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      DownTowerTemperatureLimiterRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void DownTowerTemperatureLimiterRules()
{

    if ( PARM(SoftwareIPN) < DFIG02_2400_5060_A )
    {
        SET_FV(DnTwrTmpRt, 45.0F);
        SET_FV(DnTwrTmpMx, 50.0F);
        SET_FV(DnTPwrPuMx, 0.85F);
        SET_FV(DnTwrTmpZr, PARM(InletAirOtThrsh));
    }
    else // for DFIG02_2400_5060_A
    {
        SET_FV(DnTwrTmpRt, 45.0F);
        SET_FV(DnTwrTmpMx, 50.0F);
        SET_FV(DnTPwrPuMx, 1.0F);
        SET_FV(DnTwrTmpZr, PARM(InletAirOtThrsh));
    }
    SET_FV(TempDerate,      NoNANLim((1.0F-PARM(DnTPwrPuMx))/(PARM(DnTwrTmpMx)-PARM(DnTwrTmpRt)), 1.0e6F, -1.0e6F));
    SET_FV(DnTwrTempLimGn,  NoNANLim((1.0F-PARM(TempDerate)*(PARM(DnTwrTmpZr)-PARM(DnTwrTmpRt)))/(PARM(DnTwrTmpZr)-PARM(DnTwrTmpMx)), 1.0e6F, -1.0e6F));




    SET_FV(WBAmbRefTemp, 45.0F);
    //SET_FV(DnTwrTmpRt, ( PARM(WBAmbRefTemp) - ((AltitudeClmp - PARM(AltitudeRat)) * PARM(AltitudeLpse)) )); // 2.75PM way


    SET_FV(PdcTempLim,     PARM(PdcOtThrsh) - 5.0F);



}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Turbine Power Limiter Speed Control Instrumentation
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      This function is used to implement the 'power boost' function. Its inputs
//      are a rotor voltage, timed over-current model squared currents, and
//      the maximum line and rotor junction temperatures.  Each of the inputs is
//      subtracted from a threshold and multiplied by a gain, and then the
//      minimum of these output 'power' signals is selected to pass to the PLC.
//      This minimum output is used by the PLC to limit the power of the turbine
//      until the converter no longer exceeds its limits.
//
///////////////////////////////////////////////////////////////////////////////
void TurbPwrLimSpdT3()
{

    float TempMin;

    FastTauSel = PLCDiag2;

    if ( FastTauSel)
    {
        L_WTocT3Dyn     = L_WTocFstT3;
        L_FuseWTocT3Dyn = L_FuseWTocFstT3;

        L_IMaxGnDyn     = L_IMaxFstGn;
        L_FuseIMaxGnDyn = L_FuseIMaxFstGn;
    }
    else
    {
        L_WTocT3Dyn     = L_WTocT3;
        L_FuseWTocT3Dyn = L_FuseWTocT3;

        L_IMaxGnDyn     = L_IMaxGn;
        L_FuseIMaxGnDyn = L_FuseIMaxGn;
    }

    // Call the current limit calculation functions that reduce speed
    DownTowerTempCalcT3();
    LineConverterCurrentLimitCalcT3();
    AclfuCurrentLimitCalcT3();
    ThermisterBasedSpeedLimiterCalcT3();
    LineBasePlateTempCurrentLimitCalcT3();
    RotorVoltageLimiterCalcT3();

    //-------------------------------------------------
    //Select the minimum of all the outputs
    TempMin     = LinILimOut;
    TempMin     = MIN(TempMin, L_FuseILimOut);
    TempMin     = MIN(TempMin, AclfuTempLimOut);
    TempMin     = MIN(TempMin, L_HtSinkTempILimOut);
    TempMin     = MIN(TempMin, RtrVLimOut);
    DelSpdErr = TempMin;

    // Proportional path
    DelSpdPrp  = DelSpdErr * PARM(DelSpdPrpGn);

    //Integral path
    if ( SysTmSec < 10.0F)
    {
       DelSpdInt = 0.0;
    }
    else
    {
       DelSpdInt = DelSpdInt + (DelSpdIntDt * DelSpdErr);
    }


    if ( DelSpdInt > 0.0F)
    {
        DelSpdInt = 0.0F;
    }
    else if ( DelSpdInt < PARM(DelSpdIntLm))
    {
        DelSpdInt = PARM(DelSpdIntLm);
    }

    //Total Path and clamp
    PwrLimDelSpdUc  = DelSpdPrp + DelSpdInt;

    // Clamp the PwrLimDelSpeed in RPM, do not derating if running sub-sync
    if ( PwrLimDelSpdUc > 0.0F )
    {
         PwrLimDelSpdUc = 0.0F;
    }
    if ( PwrLimDelSpdUc < -PARM(DelSpdClamp) )
    {
         PwrLimDelSpdUc = -PARM(DelSpdClamp);
    }

    if(PwrLimDelSpdUc<PARM(PwrLimDelSpdMin))
    {
        PwrLimDelSpeed = PARM(PwrLimDelSpdMin);
    }
    else
    {
        PwrLimDelSpeed = PwrLimDelSpdUc;
    }
    DelSpdErrTrq = (PwrLimDelSpdUc-PwrLimDelSpeed) * PARM(DelSpdTrqScl); // Scale the delta-speed error signal to torque

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Turbine Power Limiter Speed Control Rules
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//
///////////////////////////////////////////////////////////////////////////////
void TurbPwrLimSpdRules()
{
    R_RpmSlipNom = PARM(R_RPMNom) - PARM(R_RPMSync);

    SET_FV(DelSpdPrpGn, 1.0F);
    SET_FV(DelSpdIntGn, 0.01F);
    SET_FV(DelSpdIntLm, -0.1*(PARM(R_RPMNom)-PARM(R_RPMSync)));
    SET_FV(DelSpdClamp, (PARM(R_RPMNom)-PARM(R_RPMSync)));
    DelSpdIntDt = DelTm3 * PARM(DelSpdIntGn);
    SET_FV(DelSpdTrqScl,       R_TrqRat / R_RpmSlipNom);    // Delta-speed to delta-torque scale factor

    SET_FV(PwrLimSlpMrg,    50.0F);
    SET_FV(PwrLimSyncSpd,   PARM(R_RPMSync)+PARM(PwrLimSlpMrg));
    SET_FV(PwrLimSpdRatMax, 1.3F * PARM(R_RPMNom)); // Set the max speed rate

    if (PARM(ConverterLoc) == FACTORY)
    {
       SET_FV(PwrLimSpdRatMin, -5.0); // defeat for factory tests
    }
    else
    {
       SET_FV(PwrLimSpdRatMin, 0.7F * PARM(R_RPMNom)); // set the min speed rate
    }
    if(PARM(Net_Type)==NET_ABSENT) // Local mode
    {
        if ( ( PARM(PwrLimSpdRatLcl) > PARM(PwrLimSpdRatMax) ) || ( PARM(PwrLimSpdRatLcl) < PARM(PwrLimSpdRatMin)) )
        {
           SET_FV(PwrLimSpdRatTc,    PARM(R_RPMNom) ); //covers case where no one has populated the user input
        }
        else
        {
           SET_FV(PwrLimSpdRatTc,    PARM(PwrLimSpdRatLcl) );
        }
    }
    else // Remote mode with C to C
    {
        if ( (PwrLimSpdRatRem > PARM(PwrLimSpdRatMax)) || (PwrLimSpdRatRem < PARM(PwrLimSpdRatMin)) )
        {
           SET_FV(PwrLimSpdRatTc,    PARM(R_RPMNom) ); //covers case where TC has not populated this pararmeter and most likely zero.
        }
        else
        {
           SET_FV(PwrLimSpdRatTc,    PwrLimSpdRatRem);
        }
    }

    float tempPwrLimDelSpdMin;
    tempPwrLimDelSpdMin = PARM(PwrLimSyncSpd)-PARM(PwrLimSpdRatTc);
    if (tempPwrLimDelSpdMin>0.0F) tempPwrLimDelSpdMin=0.0F;
    SET_FV(PwrLimDelSpdMin, tempPwrLimDelSpdMin);

}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Turbine Power Limiter Torque Control
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      This function is used to implement the 'power boost' function. Its inputs
//      are a rotor voltage, timed over-current model squared currents, and
//      the maximum line and rotor junction temperatures.  Each of the inputs is
//      subtracted from a threshold and multiplied by a gain, and then the
//      minimum of these output 'power' signals is selected to pass to the PLC.
//      This minimum output is used by the PLC to limit the power of the turbine
//      until the converter no longer exceeds its limits.
//
///////////////////////////////////////////////////////////////////////////////
void TurbPwrLimTrqT3()
{


    float DelTrqKTemp;
    float TempMin;



    // call the current limit calculation functions

    // request lower speed and torque separately:
    PwrLimDelTorque = 0.0F;

    if ( FastTauSel)
    {
        GmWTocT3Dyn = GmWTocFstT3;
        S_WTocT3Dyn = S_WTocFstT3;
        R_WTocT3Dyn = R_WTocFstT3;

        GmIMaxGnDyn = GmIMaxFstGn;
        S_IMaxGnDyn = S_IMaxFstGn;
        R_IMaxGnDyn = R_IMaxFstGn;
    }
    else
    {
        GmWTocT3Dyn = GmWTocT3;
        S_WTocT3Dyn = S_WTocT3;
        R_WTocT3Dyn = R_WTocT3;

        GmIMaxGnDyn = GmIMaxGn;
        S_IMaxGnDyn = S_IMaxGn;
        R_IMaxGnDyn = R_IMaxGn;
    }

    GenAndCableTempCalcT3();
    GridMonitorCurrentLimitCalcT3();
    RotorCurrentLimitCalcT3();
    StatorCurrentLimitCalcT3();
    RotorBasePlateTempCurrentLimitCalcT3();
    ThermisterBasedTorqueLimiterCalcT3();
    DownTowerTemperatureLimiterCalcT3();


    TempMin     = CfcTempLimOut;
    TempMin     = MIN(TempMin, LqdTempLimOut);
    TempMin     = MIN(TempMin, PdcCabEnTempLimOut);
    TempMin     = MIN(TempMin, PdcTempLimOut);
    DelTrqTempILimOut = TempMin;//separated from the rest to allow all elements to fit on user diagram

    //Select the minimum of all the outputs
    TempMin     = GmILimOut;
    TempMin     = MIN(TempMin, RtrILimOut);
    TempMin     = MIN(TempMin, StrILimOut);
    TempMin     = MIN(TempMin, R1ImbTempLimOut);
    TempMin     = MIN(TempMin, R1TempRseLimOut);
    TempMin     = MIN(TempMin, DelTrqTempILimOut);
    DelTrqErr   = TempMin;


    DelTrqPrp  = DelTrqErr * PARM(DelTrqPrpGn);

    if (SysTmSec < 10.0F)
    {
        DelTrqInt = 0.0F;
    }
    else
    {
       DelTrqInt  = DelTrqInt + (DelTrqIntDt * DelTrqErr);
    }

    if ( DelTrqInt > 0.0F)
    {
        DelTrqInt = 0.0F;
    }
    else if ( DelTrqInt < PARM(DelTrqIntLm))
    {
        DelTrqInt = PARM(DelTrqIntLm);
    }

    PwrLimDelTorque = MIN ( (DelTrqPrp + DelTrqInt), DnTwrTempLmOut);
    PwrLimDelTorque = MIN (PwrLimDelTorque, DelSpdErrTrq); // delta-speed scaled to trq when sub-sync

    DelTrqKTemp  =  PwrLimDelTorque;
    DelTrqKTemp  =  DelTrqKTemp * 0.001F;
    // Clamp the PwrLimDelTorque in KNm
    if ( DelTrqKTemp > 0.0F )
    {
        DelTrqKTemp = 0.0F;
    }
    if ( DelTrqKTemp < -PARM(DelTrqClamp))
    {
        DelTrqKTemp = -PARM(DelTrqClamp);
    }
    PwrLimDelTorque_KNm =  DelTrqKTemp;

    return;
}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Turbine Power Limiter Torque Control Rules
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//
///////////////////////////////////////////////////////////////////////////////
void TurbPwrLimTrqRules()
{

    SET_FV(DelTrqPrpGn, 1.0F);
    SET_FV(DelTrqIntGn, 0.01F);
    SET_FV(DelTrqIntLm, -0.1F * R_TrqRat); //10% of the rated torque
    SET_FV(DelTrqClamp, R_TrqRat * 0.001F);
    DelTrqIntDt = DelTm3 * PARM(DelTrqIntGn);

}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      AltitudeAndCableRules
//
// EXECUTION LEVEL
//      Rule
//
// RETURN VALUE
//      This function is used to calculate rules for the assocated function.
//
///////////////////////////////////////////////////////////////////////////////
void AltitudeAndCableDerateRules()
{
    float CabAltDeRtSFVar;

    CableLimType = PARM(CableLimTyp);

    //------------------------------------------------------------------------------

    SET_FV(AltitudeRat, 1000.0F);
    SET_FV(Altitude2,   2000.0F);
    SET_FV(Altitude3,   3000.0F);
    SET_FV(AltitudeLpse,6.49e-3F); // temperature decreases about 6 degrees per 1000 meters altitude increasing

    SET_FV(AltitudeFin, PARM(AltitudeTurb));
    AltitudeClmp = MAX( PARM(AltitudeFin), PARM(AltitudeRat) );


    //------------------------------------------------------------------------------
    // General cable limit paramters

    SET_FV(AltitudeRatCab, 2000.0F);
    AltitudeClmpCab = MAX( PARM(AltitudeTurb), PARM(AltitudeRatCab) );

    SET_FV(CabAltDeRtX1, -2.0e-5F);
    SET_FV(CabAltDeRtX2,     0.0F);

    // set CabAltDeRtK1 so CabAltDeRtSF equals 1.0 when AltitudeClmpCab equals AltitudeRatCab
    SET_FV(CabAltDeRtK1,    1.0F - PARM(CabAltDeRtX1) * PARM(AltitudeRatCab) - PARM(CabAltDeRtX2) * PARM(AltitudeRatCab) * PARM(AltitudeRatCab));

    CabAltDeRtSFVar = PARM(CabAltDeRtK1) + (AltitudeClmpCab * PARM(CabAltDeRtX1)) + (AltitudeClmpCab * AltitudeClmpCab * PARM(CabAltDeRtX2));
    if (CabAltDeRtSFVar > 1.0F)
    {
        CabAltDeRtSFVar = 1.0F;
    }
    CabAltDeRtSF = CabAltDeRtSFVar;









    SET_FV(WBIRefTemp,   35.0F);
    SET_FV(WBIRatTempLo, 35.0F);
    SET_FV(WBIRatTempHi, 45.0F);
    SET_FV( WBTemp, PARM(WBIRefTemp) - (AltitudeClmp - PARM(AltitudeRat) ) * PARM(AltitudeLpse) );  //deg C


    SET_FV(WBTocTauNorm, 267.2);
    WBTocTauRcp = 1.0/PARM(WBTocTauNorm);

}

/*=============================================================================
 * Function: PowerLimiterRules
 * Execution Level: Rule
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  DelTm3
 *
 *
 *
 * Global Outputs:
 *
 * Locals:
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls
 *
 * Abstract:
 *    This function suupports rules for the power limiter
 *
 * Related
 *
 *===========================================================================*/
void PowerLimiterRules(void)
{

    AltitudeAndCableDerateRules();
    DownTowerTempRules();

    //Rules for speed-reducing functions
    LineConverterCurrentLimitRules();
    AclfuCurrentLimitRules();
    ThermisterBasedSpeedLimiterRules();
    LineBasePlateTempCurrentLimitRules();
    RotorVoltageLimiterRules();
    TurbPwrLimTrqRules();


    //Rules for torque-reducing functions
    GenAndCableTempRules();
    GridMonitorCurrentLimitRules();
    RotorCurrentLimitRules();
    StatorCurrentLimitRules();
    RotorBasePlateTempCurrentLimitRules();
    ThermisterBasedTorqueLimiterRules();
    DownTowerTemperatureLimiterRules();
    TurbPwrLimSpdRules();

    // Rules for diagnostics
    PowerLimiterDiagnosticRule();
}

void PowerLimiterDiagnosticRule(void)
{
    SET_FV(PwrLimDiagThr, 50.0F);
    SET_FV(PwrLimTrqDiagThr, R_TrqRat*NoNANLim(PARM(PwrLimDiagThr)/PARM(TurbineNpRat), 1.0e6F, -1.0e6F));
    SET_FV(PwrLimSpdDiagThr, NoNANLim(PARM(PwrLimTrqDiagThr)/PARM(DelSpdTrqScl), 1.0e6F, -1.0e6F));
    SET_FV(PwrLimHysLwThr, -25.0F);
    SET_FV(PwrLimHysHiThr, 0.0F);
    SET_FV(PwrLimHysHiTm,  10.0F);
    SET_FV(PwrLimTrqHysLwThr,R_TrqRat*NoNANLim(PARM(PwrLimHysLwThr)/PARM(TurbineNpRat), 1.0e6F, -1.0e6F));
    SET_FV(PwrLimTrqHysHiThr,R_TrqRat*NoNANLim(PARM(PwrLimHysHiThr)/PARM(TurbineNpRat), 1.0e6F, -1.0e6F));
    SET_FV(PwrLimSpdHysLwThr, NoNANLim(PARM(PwrLimTrqHysLwThr)/PARM(DelSpdTrqScl), 1.0e6F, -1.0e6F));
    SET_FV(PwrLimSpdHysHiThr, NoNANLim(PARM(PwrLimTrqHysHiThr)/PARM(DelSpdTrqScl), 1.0e6F, -1.0e6F));
}

void TurbPwrLimDerateIdCheck(void)
{
    unsigned tempId;

    // set flag when PwrLimDelSpdUc is negative
    if (PwrLimDelSpdUc<PARM(PwrLimSpdHysLwThr))
    {
        // line current
        if(LinILimOut<0)
        {
            PwrLimLnCnvCurFlg = true;
            PwrLimLnCnvCurHiTmr = 0.0F;
        }
        // line fuse
        if(L_FuseILimOut<0)
        {
            PwmLimFuseCurFlg = true;
            PwrLimFuseCurHiTmr = 0.0F;
        }
        // Aclfu temp
        if(AclfuTempLimOut<0)
        {
            PwmLimFuseTempFlg = true;
            PwrLimFuseTempHiTmr = 0.0F;
        }
        // rotor voltage
        if(RtrVLimOut<0)
        {
            PwmLimRtrVltFlg = true;
            PwrLimRtrVltHiTmr = 0.0F;
        }
        // line imbalance temperature
        if(LinImbTempLimOut<0)
        {
            PwmLimLnBpImbFlg = true;
            PwrLimLnBpImbHiTmr = 0.0F;
        }
        // line bp temperature rise
        if(LinTempRseLimOut<0)
        {
            PwmLimLnBpRiseFlg = true;
            PwrLimLnBpRiseHiTmr = 0.0F;
        }
    }
    else if (PwrLimDelSpdUc>=PARM(PwrLimSpdHysHiThr))// speed limit output is positive
    {
        PwrLimLnCnvCurFlg = false;
        PwmLimFuseCurFlg = false;
        PwmLimFuseTempFlg = false;
        PwmLimRtrVltFlg = false;
        PwmLimLnBpImbFlg = false;
        PwmLimLnBpRiseFlg = false;
    }
    // line converter current
    if (LinILimOut>0)
    {
        PwrLimLnCnvCurHiTmr += DelTm3;
        if(PwrLimLnCnvCurHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimLnCnvCurHiTmr=PARM(PwrLimHysHiTm);
            PwrLimLnCnvCurFlg = false;
        }
    }
    // line fuse
    if (L_FuseILimOut>0)
    {
        PwrLimFuseCurHiTmr += DelTm3;
        if(PwrLimFuseCurHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimFuseCurHiTmr=PARM(PwrLimHysHiTm);
            PwmLimFuseCurFlg = false;
        }
    }
    // Aclfu temp
    if (AclfuTempLimOut>0)
    {
        PwrLimFuseTempHiTmr += DelTm3;
        if(PwrLimFuseTempHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimFuseTempHiTmr=PARM(PwrLimHysHiTm);
            PwmLimFuseTempFlg = false;
        }
    }
    // rotor voltage
    if (RtrVLimOut>0)
    {
        PwrLimRtrVltHiTmr += DelTm3;
        if(PwrLimRtrVltHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimRtrVltHiTmr=PARM(PwrLimHysHiTm);
            PwmLimRtrVltFlg = false;
        }
    }
    // line imbalance temperature
    if (LinImbTempLimOut>0)
    {
        PwrLimLnBpImbHiTmr += DelTm3;
        if(PwrLimLnBpImbHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimLnBpImbHiTmr=PARM(PwrLimHysHiTm);
            PwmLimLnBpImbFlg = false;
        }
    }
    // line bp temperature rise
    if (LinTempRseLimOut>0)
    {
        PwrLimLnBpRiseHiTmr += DelTm3;
        if(PwrLimLnBpRiseHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimLnBpRiseHiTmr=PARM(PwrLimHysHiTm);
            PwmLimLnBpRiseFlg = false;
        }
    }

    // torque limit
    if(PwrLimDelTorque_KNm<PARM(PwrLimTrqHysLwThr)*0.001F)
    {
        // GM current
        if(GmILimOut<0)
        {
            PwmLimGmCurFlg = true;
            PwrLimGmCurHiTmr = 0.0F;
        }
        // Rotor current
        if(RtrILimOut<0)
        {
            PwmLimRtrCurFlg = true;
            PwrLimRtrCurHiTmr = 0.0F;
        }
        // Stator current
        if(StrILimOut<0)
        {
            PwmLimStrCurFlg = true;
            PwrLimStrCurHiTmr = 0.0F;
        }
        // rotor bp temperature rise
        if(R1TempRseLimOut<0)
        {
            PwmLimRtrBpRiseFlg = true;
            PwrLimRtrBpRiseHiTmr = 0.0F;
        }
        // rotor bp imbalance
        if(R1ImbTempLimOut<0)
        {
            PwmLimRtrBpImbFlg = true;
            PwrLimRtrBpImbHiTmr = 0.0F;
        }
        // Down tower temperature
        if(DnTwrTempLmOut<0)
        {
            PwmLimDnTwrTempFlg = true;
            PwrLimDnTwrTempHiTmr = 0.0F;
        }
        // coolant temperature
        if(LqdTempLimOut<0)
        {
            PwmLimClntTempFlg = true;
            PwrLimClntTempHiTmr = 0.0F;
        }
        // PCA cable entry temperature
        if(PdcCabEnTempLimOut<0)
        {
            PwmLimPdcCabEnTmpFlg = true;
            PwrLimPdcCbEnTmpHTmr = 0.0F;
        }
        // Cfc temp
        if(CfcTempLimOut<0)
        {
            PwmLimCfcTempFlg = true;
            PwrLimCfcTempHiTmr = 0.0F;
        }
        // pdc temp
        if(PdcTempLimOut<0)
        {
            PwmLimPdcTempFlg = true;
            PwrLimPdcTempHiTmr = 0.0F;
        }
    }
    else if(PwrLimDelTorque_KNm>=PARM(PwrLimTrqHysHiThr)*0.001F)
    {
        PwmLimGmCurFlg = false;
        PwmLimRtrCurFlg = false;
        PwmLimStrCurFlg = false;
        PwmLimRtrBpRiseFlg = false;
        PwmLimRtrBpImbFlg = false;
        PwmLimDnTwrTempFlg = false;
        PwmLimClntTempFlg = false;
        PwmLimPdcCabEnTmpFlg = false;
        PwmLimCfcTempFlg = false;
        PwmLimPdcTempFlg = false;
    }
    // GM current
    if (GmILimOut>0)
    {
        PwrLimGmCurHiTmr += DelTm3;
        if(PwrLimGmCurHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimGmCurHiTmr=PARM(PwrLimHysHiTm);
            PwmLimGmCurFlg = false;
        }
    }
    // Rotor current
    if (RtrILimOut>0)
    {
        PwrLimRtrCurHiTmr += DelTm3;
        if(PwrLimRtrCurHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimRtrCurHiTmr=PARM(PwrLimHysHiTm);
            PwmLimRtrCurFlg = false;
        }
    }
    // Stator current
    if (StrILimOut>0)
    {
        PwrLimStrCurHiTmr += DelTm3;
        if(PwrLimStrCurHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimStrCurHiTmr=PARM(PwrLimHysHiTm);
            PwmLimStrCurFlg = false;
        }
    }
    // rotor bp temperature rise
    if (R1TempRseLimOut>0)
    {
        PwrLimRtrBpRiseHiTmr += DelTm3;
        if(PwrLimRtrBpRiseHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimRtrBpRiseHiTmr=PARM(PwrLimHysHiTm);
            PwmLimRtrBpRiseFlg = false;
        }
    }
    // rotor bp imbalance
    if (R1ImbTempLimOut>0)
    {
        PwrLimRtrBpImbHiTmr += DelTm3;
        if(PwrLimRtrBpImbHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimRtrBpImbHiTmr=PARM(PwrLimHysHiTm);
            PwmLimRtrBpImbFlg = false;
        }
    }
    // Down tower temperature
    if (DnTwrTempLmOut>0)
    {
        PwrLimDnTwrTempHiTmr += DelTm3;
        if(PwrLimDnTwrTempHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimDnTwrTempHiTmr=PARM(PwrLimHysHiTm);
            PwmLimDnTwrTempFlg = false;
        }
    }
    // coolant temperature
    if (LqdTempLimOut>0)
    {
        PwrLimClntTempHiTmr += DelTm3;
        if(PwrLimClntTempHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimClntTempHiTmr=PARM(PwrLimHysHiTm);
            PwmLimClntTempFlg = false;
        }
    }
    // PCA cable entry temperature
    if (PdcCabEnTempLimOut>0)
    {
        PwrLimPdcCbEnTmpHTmr += DelTm3;
        if(PwrLimPdcCbEnTmpHTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimPdcCbEnTmpHTmr=PARM(PwrLimHysHiTm);
            PwmLimPdcCabEnTmpFlg = false;
        }
    }
    // Cfc temp
    if (CfcTempLimOut>0)
    {
        PwrLimCfcTempHiTmr += DelTm3;
        if(PwrLimCfcTempHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimCfcTempHiTmr=PARM(PwrLimHysHiTm);
            PwmLimCfcTempFlg = false;
        }
    }
    // pdc temp
    if (PdcTempLimOut>0)
    {
        PwrLimPdcTempHiTmr += DelTm3;
        if(PwrLimPdcTempHiTmr>PARM(PwrLimHysHiTm))
        {
            PwrLimPdcTempHiTmr=PARM(PwrLimHysHiTm);
            PwmLimPdcTempFlg = false;
        }
    }

    // down tower temperature limiter
    // if(DnTwrTempLmOut<PARM(PwrLimTrqHysLwThr))
    // {
        // PwmLimDnTwrTempFlg = true;
    // }
    // else if(DnTwrTempLmOut>PARM(PwrLimTrqHysHiThr))
    // {
        // PwmLimDnTwrTempFlg = false;
    // }

    tempId = NONE_DERATE;
    // push alarm and set bits
    if (PwrLimLnCnvCurFlg)
    {
        tempId |= LNCNVCUR_DERATE;
        PUSH_DIAG(PwrLimLnCnvCurAlm);
    }
    if(PwmLimFuseCurFlg)
    {
        tempId |= LNFUSECUR_DERATE;
        PUSH_DIAG(PwrLimFuseCurAlm);
    }
    if(PwmLimFuseTempFlg)
    {
        tempId |= LNFUSETEMP_DERATE;
        PUSH_DIAG(PwrLimFuseTempAlm);
    }
    if(PwmLimRtrVltFlg)
    {
        tempId |= RTRVLT_DERATE;
        PUSH_DIAG(PwrLimRtrVltAlm);
    }
    if(PwmLimLnBpImbFlg)
    {
        tempId |= LNBPTEMPIMB_DERATE;
        PUSH_DIAG(PwrLimLnBpImbAlm);
    }
    if(PwmLimLnBpRiseFlg)
    {
        tempId |= LNBPTEMPRISE_DERATE;
        PUSH_DIAG(PwrLimLnBpRiseAlm);
    }

    if(PwmLimGmCurFlg)
    {
        PUSH_DIAG(PwrLimGmCurAlm);
        tempId |= GMCUR_DERATE;
    }
    if(PwmLimRtrCurFlg)
    {
        if (PwrLimRtrCurSel==PWRLIM_RTRCUR_CNV)
        {
            tempId |= RTRCNVCUR_DERATE;
            PUSH_DIAG(PwrLimRtrCnvCurAlm);
        }
        if (PwrLimRtrCurSel==PWRLIM_RTRCUR_GEN)
        {
            tempId |= RTRGENCUR_DERATE;
            PUSH_DIAG(PwrLimRtrGenCurAlm);
        }
        if (PwrLimRtrCurSel==PWRLIM_RTRCUR_CAB)
        {
            tempId |= RTRCABCUR_DERATE;
            PUSH_DIAG(PwrLimRtrCabCurAlm);
        }
    }
    if(PwmLimStrCurFlg)
    {
        if (PwrLimStrCurSel==PWRLIM_STRCUR_CNV)
        {
            tempId |= STRCNVCUR_DERATE;
            PUSH_DIAG(PwrLimStrDtaCurAlm);
        }
        if (PwrLimStrCurSel==PWRLIM_STRCUR_GEN)
        {
            tempId |= STRGENCUR_DERATE;
            PUSH_DIAG(PwrLimStrGenCurAlm);
        }
        if (PwrLimStrCurSel==PWRLIM_STRCUR_CAB)
        {
            tempId |= STRCABCUR_DERATE;
            PUSH_DIAG(PwrLimStrCabCurAlm);
        }
    }
    if(PwmLimRtrBpRiseFlg)
    {
        tempId |= RTRBPTEMPRISE_DERATE;
        PUSH_DIAG(PwrLimRtrBpRiseAlm);
    }
    if(PwmLimRtrBpImbFlg)
    {
        tempId |= RTRBPTEMPIMB_DERATE;
        PUSH_DIAG(PwrLimRtrBpImbAlm);
    }
    if(PwmLimDnTwrTempFlg)
    {
        tempId |= DNTWRTEMP_DERATE;
        PUSH_DIAG(PwrLimDnTwrTempAlm);
    }
    if(PwmLimClntTempFlg)
    {
        tempId |= CLNTTEMP_DERATE;
        PUSH_DIAG(PwrLimClntTempAlm);
    }
    if(PwmLimPdcCabEnTmpFlg)
    {
        tempId |= PDCCABENTEMP_DERATE;
        PUSH_DIAG(PwrLimPdcCabEnTmpAlm);
    }
    if(PwmLimCfcTempFlg)
    {
        tempId |= CFCTEMP_DERATE;
        PUSH_DIAG(PwrLimCfcTempAlm);
    }
    if(PwmLimPdcTempFlg)
    {
        tempId |= PDCTEMP_DERATE;
        PUSH_DIAG(PwrLimPdcTempAlm);
    }

    PwrLimDerateId = tempId;

}
