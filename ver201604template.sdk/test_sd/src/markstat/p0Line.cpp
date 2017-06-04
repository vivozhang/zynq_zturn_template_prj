///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Line Grid Monitor functionality
//
// DESCRIPTION:
//      Functions which associated with angle calculations.
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     p0Line.vsd, p0Line.xls
//
///////////////////////////////////////////////////////////////////////////////

// Includes Files
//---------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
// product
#include "Product.h"
#include "b0BridgeRebf.h"
#include "b0DbCelTst.h"
#include "p0Line.h"
#include "p2Sseq.h"


// Externals
//----------

extern unsigned         GmGndFltInh; 
extern FaultIsActiveTyp FaultIsActive ;
extern cDbCelTst        L_DbCelTst    ;
extern cBridgeRebf      L_BridgeRebf  ;
extern float            CosEFbkP30    ;
extern float            DelTm2        ;
extern float            DelTm3        ;
extern float            GmAngEFbkT2   ;
extern float            GmCVAsmMinFrc ;
extern int              GmCVAsmMinInt ;
extern float            GmDscmVMgNT3  ;
extern float            GmDscmVMgPT3  ;
extern float            GmDscmVxNT2   ;
extern float            GmDscmVxNT3   ;
extern float            GmDscmVxPT2;
extern float            GmDscmVyNT2   ;
extern float            GmDscmVyNT3   ;
extern float            GmDscmVyPT2   ;
extern float            GmIa          ;
extern float            GmIaAl2T3     ;
extern float            GmIaBe2T3     ;
extern float            GmIaSqrT3     ;
extern float            GmIaT2        ;
extern float            GmIb          ;
extern float            GmIbAl2T3     ;
extern float            GmIbBe2T3     ;
extern float            GmIbSqrT3     ;
extern float            GmIbT2        ;
extern float            GmIc          ;
extern float            GmIcAl2T3     ;
extern float            GmIcBe2T3     ;
extern float            GmIcSqrT3     ;
extern float            GmIcT2        ;
extern float            GmPllErrT2    ;
extern float            GmPllIntGnT1  ;
extern float            GmPllPrpGnT1;
extern float            GmPllWMaxIntT1;
extern float            GmPllWMaxT1   ;
extern float            GmPllWMinIntT1;
extern float            GmPllWMinT1   ;
extern float            GmPllWRAFT1   ;
extern float            GmPllWTrackT1 ;
extern float            GmPwrT3       ;
extern float            GmQPwrT3      ;
extern float            GmRAFGnDiff   ;
extern float            GmRAFGnInnr   ;
extern float            GmUV2TrpTmr1  ;
extern float            GmUV2TrpTmr2  ;
extern float            GmUV2TrpTmr3  ;
extern float            GmUV3TrpTmr1  ;
extern float            GmUVCvConstA  ;
extern float            GmUVCvConstB  ;
extern float            GmUVCvConstC  ;
extern float            GmUVCvConstD  ;
extern int              GmUVMaxInt    ;
extern float            GmVan         ;
extern float            GmVanAl2T3    ;
extern float            GmVanBe2T3    ;
extern float            GmVanT2       ;
extern float            GmVbn         ;
extern float            GmVbnAl2T3    ;
extern float            GmVbnBe2T3    ;
extern float            GmVbnT2       ;
extern float            GmVcn         ;
extern float            GmVcnAl2T3    ;
extern float            GmVcnBe2T3    ;
extern float            GmVcnT2       ;
extern float            GmVanGrid     ;
extern float            GmVbnGrid     ;
extern float            GmVcnGrid     ;
extern float            GmIGndFilWdt  ;
extern float            L_DelTm1      ;
extern float            L_IaRmsMag    ;
extern float            L_IbRmsMag    ;
extern float            L_IcRmsMag    ;
extern float            L_IMagFil     ;
extern float            L_StopStTmr   ;
extern float            L_VlnInRcp    ;
extern float            PwrGridT3;
extern float            PwrPllWFFT1   ;
extern float            QPwrGridT3    ;
extern float            QregCmd       ;
extern float            R_IaRmsMag    ;
extern float            R_IbRmsMag    ;
extern float            R_IcRmsMag    ;
extern float            R_IMagFil     ;
extern float            S_IaRmsMag    ;
extern float            S_IbRmsMag    ;
extern float            S_IcRmsMag    ;
extern float            S_IMagFil     ;
extern float            SinEFbkP30    ;
extern float            VlnPeakRcp    ;
extern float            XfmrRatioLnStrRcp;
extern float            XfmrRatioLnStr;
extern float            L_VyLkgSecT2  ;
extern float            L_VyLkgPriT2  ;
extern float            S_VyLkgPriT2  ;
extern float            GmIxyShftD    ;
extern float            GmIxyShftQ    ;
extern float            L_PwrLineT3   ;
extern float            L_VarHrmFT3   ;
extern float            L_VarLineT3   ;
extern float            L_AngElecCmd  ;
extern float            L_AngElecFbk  ;
extern float            GmVMagGridPu  ;
extern float            GmRAFCntFlt   ;
extern float            DelTm2Rcp     ;
extern float            GmRAFGnOutr   ;

extern int              GmPllStateT1  ;
extern int              GmRAFCnt      ;
extern int              L_SeqSt       ;
extern int              GmOVMinInt    ;
extern float            GmOVMinFrc    ;
extern unsigned         HVACStAct     ;
extern unsigned         DiagResetCmdT3;
extern unsigned         GmOFCfgBad    ;
extern unsigned         GmOVCfgBad    ;
extern unsigned         GmPllStateZ1T1;
extern unsigned         GmRevPwrTrpFT2;
extern unsigned         GmUFCfgBad    ;
extern unsigned         GmUVCfgBad    ;
extern unsigned         Init1Sec      ;
extern unsigned         Init3Sec      ;
extern unsigned         InitDone      ;
extern unsigned         L_InnrRegEnb  ;
extern unsigned         L_InnrRegEnbT1;
extern unsigned         L_Running     ;
extern unsigned         L_ZVLineOff   ;
extern unsigned         PwrDnFbkDsb   ;
extern unsigned         HdwTstMode    ;
extern unsigned         MvSwGearTrpReq;
extern unsigned         Stopped       ;
extern unsigned         SysTmSec      ;
extern unsigned         HVACHotPll    ;
extern volatile int     T1St          ;
extern RegLimFlgs       L_InnrRegLim  ;
extern float            S_IxFbkT2     ;
extern float            PwrToGrid     ;
extern float            GmIxGrid      ;
extern float            GmIyGrid      ;
extern float            GmMaxCurOut   ;

extern SinCosTyp        L_SinCosFbk;

extern unsigned         IslandDet;
extern float            GmPllVyErrT2;
extern unsigned         Q1OpnReq;
extern float            GmSVMtchErSq;
extern float            GmSVmtchThr;
extern unsigned         ISimMode;
extern unsigned         Q1StatSim;
extern unsigned         S_K1Status;
extern float            GmVMagFstScl;
extern float            GmVMgFstPkT2;
extern float            GmFstOVFilGn;
extern float            GmFstOVThrs;

extern unsigned         L_LTestIp;
extern float            GmExPwrFilGn;
extern float            GmOF1Cap;
extern float            GmOF1CapTm;
extern float            GmOF2Cap;
extern float            GmOF2CapTm;
extern float            GmOFMinLvl;
extern float            GmOV1Cap;
extern float            GmOV2Cap;
extern float            GmOV1CapTm;
extern float            GmOV2CapTm;
extern float            GmOVMinLvl;
extern float            GmRvPwrFilGn;
extern float            GmUF1Cap;
extern float            GmUF1CapTm;
extern float            GmUF2Cap;
extern float            GmUF2CapTm;
extern float            GmUF3Cap;
extern float            GmUF3CapTm;
extern float            GmUFMaxLvl;
extern float            GmUV1Cap;
extern float            GmUV1CapTm;
extern float            GmUV2Cap;
extern float            GmUV2CapTm;
extern float            GmUV3Cap;
extern float            GmUV3CapTm;
extern float            GmUV4Cap;
extern float            GmUV4CapTm;
extern float            GmUVCvUCapTm;
extern float            GmUVCvLCapTm;
extern float            GmUVCvVSThr1;
extern float            GmUVCvVSThr2;
extern float            GmUVCvVSThr3;
extern float            GmUVMaxLvl;
extern unsigned     GmLineFltEnb;   //Ac Gm transient fault enable flag

// Constants
//----------
#define SQRT3_DIV_2  0.5*SQRT3
#define LINE_TO_LINE 0
#define LINE_TO_NEUT 1
#define ALL 0
#define ANY 1
#define RCP_3   (1.0/3.0)
#define TRP_TMR_RST_MRG   (5.0F)

// Types and Classes
//------------------


// Parameters
//-----------

// Wizard
CREATE_PARM(GmCfgSel,       unsigned );    // Grid Monitor volt and frequency selection
CREATE_PARM(GmCfgSelOpt,    unsigned );    // Grid Monitor volt and frequency selection
CREATE_PARM(GmCfgSelFV,     unsigned );    // Grid Monitor volt and frequency selection

CREATE_PARM(GmIyInstCalcVmin ,    float);          // GmIyInst Calculation min grid voltage 

CREATE_PARM(GmRAFCntDynEnb ,   unsigned);          // Enable GmRAFCnt calc in run-time
CREATE_PARM(GmWPllW4RAF    ,      float);          // Filter omega for PLLW for RAF
CREATE_PARM(GmOV3RlyEnb    ,   unsigned);          // Enables/disables OV level 3 faults for grid monitor relay
CREATE_PARM(GmOV4RlyEnb    ,   unsigned);          // Enables/disables OV level 4 faults for grid monitor relay

CREATE_PARM(GmUV5RlyEnb    ,   unsigned);          // Enables/disables OV level 3 faults for grid monitor relay
CREATE_PARM(GmUV6RlyEnb    ,   unsigned);          // Enables/disables UV level 5 faults for grid monitor relay
CREATE_PARM(GmVAsy2RlyEnb  ,   unsigned);          // Enables/disables Vasym level 2 faults for grid monitor relay

CREATE_PARM(GmTocMargin    ,      float);          // Timed over-current margin for grid monitor
CREATE_PARM(TocDyThrshEnb  ,      unsigned);       // Enable the dynamic TOC threshold

//Remote fault parameters
CREATE_PARM(GmPllShftDB    ,      float);          // Pll shift dead-band for remote faults
CREATE_PARM(GmPllSftVThr   ,      float);          // Pll shift voltage threshold for remote faults
CREATE_PARM(GmPllShftGn    ,      float);          // Pll shift gain for remote faults
CREATE_PARM(GmPllShiftW    ,      float);          // Pll shift omega for remote faults
CREATE_PARM(GmPllShftTm    ,      float);          // Pll shift omega for remote faults

CREATE_PARM(GmUV5TrpInh    ,   unsigned);
CREATE_PARM(GmUV5TrpThr    ,      float);
CREATE_PARM(GmUV5TrpTm     ,      float);

CREATE_PARM(GmUV6TrpInh    ,   unsigned);
CREATE_PARM(GmUV6TrpThr    ,      float);
CREATE_PARM(GmUV6TrpTm     ,      float);

CREATE_PARM(GmOV3Cap       ,      float);          // Over-voltage capability voltage 3
CREATE_PARM(GmOV3CapTm     ,      float);          // Over-voltage capability time 3
CREATE_PARM(GmOV3TrpThr    ,      float);
CREATE_PARM(GmOV3TrpTm     ,      float);

CREATE_PARM(GmOV4Cap       ,      float);          // Over-voltage capability voltage 4
CREATE_PARM(GmOV4CapTm     ,      float);          // Over-voltage capability time 4
CREATE_PARM(GmOV4TrpThr    ,      float);
CREATE_PARM(GmOV4TrpTm     ,      float);

CREATE_PARM(GmUV5Cap       ,      float);          // Under-voltage capability voltage 5
CREATE_PARM(GmUV6Cap       ,      float);          // Under-voltage capability voltage 6
CREATE_PARM(GmUV5CapTm     ,      float);          // Under-voltage capability time for level 5
CREATE_PARM(GmUV6CapTm     ,      float);          // Under-voltage capability time for level 6

CREATE_PARM(GmVAsy2TrpThr  ,      float);
CREATE_PARM(GmVAsy2AlmTm   ,      float);
CREATE_PARM(GmVAsy2TrpTm   ,      float);

// low voltage event edge detection
CREATE_PARM(GmVLoPulseTm,       float);     // Line voltage low latch time
CREATE_PARM(GmVLoNotTm,         float);     // Line voltage not low time

// Grid monitor Toc threshold parameters
CREATE_PARM(GmTocTau,           float);     // Timed over-current time constant
CREATE_PARM(GmTocThrs,          float);     // Timed over-current threshold
CREATE_PARM(GmTocAlmOff,        float);     // Timed over-current alarm "off" threshold
CREATE_PARM(GmTocAlmOn,         float);     // Timed over-current alarm "on" threshold

CREATE_PARM(GmFrqVltDlyTm,      float);     // Grid Monitor Freq Voltage Timer Threshold

CREATE_PARM(GmVltThrMrg,        float);     // Grid Monitor Voltage Threshold Margin
CREATE_PARM(GmFrqThrMrg,        float);     // Grid Monitor Freq Threshold Margin

CREATE_PARM(GmPllPrpGnIs,       float);     // PLL proportional gain for islanding detection
CREATE_PARM(GmPllIntGnIs,       float);     // PLL integral gain for islanding detection
CREATE_PARM(GmPllWInMxIs,       float);     // PLL integrator max frequency for islanding detection
CREATE_PARM(GmPllWInMnIs,       float);     // PLL integrator min frequency for islanding detection
CREATE_PARM(GmPllWMaxIs,        float);     // PLL max frequency for islanding detection
CREATE_PARM(GmPllWMinIs,        float);     // PLL min frequency for islanding detection

CREATE_PARM(GmPllPrpGnI2,       float);
CREATE_PARM(GmPllIntGnI2,       float);
CREATE_PARM(L_RevSeqReq,        unsigned);  // Reverse sequence request
CREATE_PARM(GmPrpGnISySF,       float);
CREATE_PARM(GmPrpGnVMgSF,       float);
CREATE_PARM(GmPrpGnVMgOf,       float);
CREATE_PARM(GmPllHotGnDy,       unsigned);

CREATE_PARM(GmPllShftGnN,       float);
CREATE_PARM(GmPllShftGnH,       float);
CREATE_PARM(GmPllHiGnTm,        float);
// System current decrease by PLL error parameters
CREATE_PARM(ISysCmdSFScl,       float);     // Adjustment factor: Sqrt(2) multiplier
CREATE_PARM(ISysCmdUpRt,        float);     // System current up rate
CREATE_PARM(ISysCmdMin,         float);     // System current minimum
CREATE_PARM(ISysCmdVMgMn,       float);     // System current command minimum voltage value
CREATE_PARM(GmDsVMgPuMin,       float);     // Min voltage clamp for GmDscmVMgPPu

CREATE_PARM(GmUVIndTstAlmThr,      float);
CREATE_PARM(GmUV2IndTstTrpThr,     float);
CREATE_PARM(GmUV3IndTstTrpThr,     float);
CREATE_PARM(GmUVIndTstTrpThrs1,    float);
CREATE_PARM(GmUVIndTstTrpThrs2,    float);
//Pll and line parameters
CREATE_PARM(GmPllBwBase,        float);     // Phase lock loop bandwidth base
CREATE_PARM(GmPllWnSf,          float);     // Phase lock loop Wn scale factor for cool gains
CREATE_PARM(GmPllWn,            float);     // Phase lock loop Wn scale factor for cool gains
CREATE_PARM(GmPllBaseHot,       float);     // Phase lock loop bandwidth hot
CREATE_PARM(GmPllWnHotSf,       float);     // Phase lock loop Wn scale factor for hot gains
CREATE_PARM(GmPllWnHot,         float);     // Phase lock loop Wn scale factor for hot gains
CREATE_PARM(GmPllPrpGnR,        float);     // Pll proportional gain for pll proven
CREATE_PARM(GmPllPrpGnI,        float);     // Pll proportional gain for pll not proven
CREATE_PARM(GmPllPrpGnI1,       float);     // Pll proportional gain for pll not proven and after init state exit
CREATE_PARM(GmPllIntGnR,        float);     // Pll integral gain for pll proven
CREATE_PARM(GmPllIntGnI,        float);     // Pll integral gain for pll not proven
CREATE_PARM(GmPllIntGnI1,       float);     // Pll integral gain for pll not proven and after init state exit
CREATE_PARM(GmPllWMinR,         float);     // Pll minimum frequency
CREATE_PARM(GmPllWMaxR,         float);     // Pll maximum frequency
CREATE_PARM(GmPllWMinI,         float);     // Pll minimum initialization frequency
CREATE_PARM(GmPllWMaxI,         float);     // Pll maximum initialization frequency
CREATE_PARM(GmPllWIntMx,        float);     // Pll integrator maximum frequency
CREATE_PARM(GmPllWIntMn,        float);     // Pll integrator minimum frequency
CREATE_PARM(GmPllWPrec,         float);     // Pll initial state pre-condition
CREATE_PARM(GmPllFrqPrec,       float);     // Pll frequency for pre-condition
CREATE_PARM(GmFixFrqMxTm,       float);     // Pll state machine fixed frequency maximum time.
CREATE_PARM(GmAcLineWUp,        float);     // Ac line peak detection omega up
CREATE_PARM(GmAcLineWDn,        float);     // Ac line peak detection omega down
CREATE_PARM(GmPllErNoise,       float);     // Pll error noise
CREATE_PARM(GmPllIniThrs,       float);     // Phase lock loop init threshold
CREATE_PARM(GmPllRcvThrs,       float);     // Phase lock loop recovery threshold
CREATE_PARM(GmPllRunThrs,       float);     // Phase imbalance trip threshold
CREATE_PARM(GmPllErSpill,       float);     // Phase lock loop spill-over limit
CREATE_PARM(GmWPllWTrack,       float);     // Phase lock loop frequency track omega
CREATE_PARM(GmPllWShift,        float);     // Phase lock loop frequency shift for track omega
CREATE_PARM(GmPllTrackTm,       float);     // Phase lock loop track syncronization time.
CREATE_PARM(GmAngTrckInh,       unsigned);  // Inhibt pll angle tracking during symetric short
CREATE_PARM(GmLineVHiOn,        float);     // Line voltage hi on threshold
CREATE_PARM(GmLineVHiOff,       float);     // Line voltage hi off threshold
CREATE_PARM(GmLineVHiTm,        float);     // Line voltage hi time
CREATE_PARM(GmLineVLowOn,       float);     // Line voltage low on threshold
CREATE_PARM(GmLineVLowOf,       float);     // Line voltage low off threshold
CREATE_PARM(GmLineVLowTm,       float);     // Line voltage low time
CREATE_PARM(GmLineVShrOn,       float);     // Line loss on threshold
CREATE_PARM(GmLineVShrOf,       float);     // Line loss off threshold
CREATE_PARM(GmLineVShrO2,       float);     // Line loss off threshold 2
CREATE_PARM(GmLineVShrTm,       float);     // Line line loss time
CREATE_PARM(GmLnVShrOfTm,       float);     // Line line loss off time
CREATE_PARM(GmLnVShrOfT2,       float);     // Line line loss off time 2
CREATE_PARM(GmAcWdogTm,         float);     // Time a phase lockl loop error in limit allowed to exist
CREATE_PARM(GmPllErrW,          float);     // Phase lock loop error omega
CREATE_PARM(GmVxVyErrW,         float);     // LineVx,    LineVy Error omega for pll lock/mislock detector
CREATE_PARM(GmPllGnSwTm,        float);     // Pll gain switch time
CREATE_PARM(GmPllPrvTm,         float);     // Pll prove time
CREATE_PARM(GmPllrStpAdr,       float *);   // Pll regulator step address
CREATE_PARM(GmSyncInitTm,       float);     // PLl regulator sync. init max time.
CREATE_PARM(GmSVllFbkLag,       float);     // Grid monitor-stator feedback lag
CREATE_PARM(GmAngShiftT2,       float);     // Grid monitor feedback lag for task 2
//Positive/Negative Sequence Discriminator
CREATE_PARM(GmDscmWVxP,         float);     // Positive/Negative Sequence Discrimator omega for Vx positive
CREATE_PARM(GmDscmWVyP,         float);     // Positive/Negative Sequence Discrimator omega for Vy positive
CREATE_PARM(GmDscmWVxN,         float);     // Positive/Negative Sequence Discrimator omega for Vx positive
CREATE_PARM(GmDscmWVyN,         float);     // Positive/Negative Sequence Discrimator omega for Vy negative
CREATE_PARM(GmDscmWVxN2,        float);     // Positive/Negative Sequence Discrimator omega for Vx positive for task 2 filter
CREATE_PARM(GmDscmWVyN2,        float);     // Positive/Negative Sequence Discrimator omega for Vy negative for task 2 filter
CREATE_PARM(GmNSeqPuScl,        float);     // Negative sequence voltage adjustment factor (to pu);     
CREATE_PARM(GmNSeqPuGn,         float);     // Negative sequence voltage per unit gain
CREATE_PARM(GmNSeqPuOffs,       float);     // Negative sequence voltage per unit offset
CREATE_PARM(GmNSeqAngThr,       float);     // Negative sequence angle calc permissive threshold
CREATE_PARM(GmNSeqHiThr,        float);     // Negative sequence hi threshold
CREATE_PARM(GmNSqMaxStrt,       float);     // Negative sequence maximum start threshold
CREATE_PARM(GmPhsVoltW,         float);     // Grid moniotr phase voltage response omega
CREATE_PARM(GmPhsCurrW,         float);     // Grid monitor phase current response omega
CREATE_PARM(GmPllVyFbkSw,       unsigned);  // Phase lock loop regulator Vy feedback switch
CREATE_PARM(GmVxFilW,           float);     // Grid monitor Vx low pass filter response
CREATE_PARM(GmVyFilW,           float);     // Grid monitor Vy low pass filter response
CREATE_PARM(GmIxFilW,           float);     // Grid monitor Ix low pass filter response
CREATE_PARM(GmIyFilW,           float);     // Grid monitor Iy low pass filter response
CREATE_PARM(GmFreqFilW,         float);     // Grid monitor line frequency low pass filter response
CREATE_PARM(GmIGndFilW,         float);     // Grid monitor ground current low pass filter response
CREATE_PARM(GmPwrFilW,          float);     // Grid monitor power filter omega
CREATE_PARM(GmQPwrFilW,         float);     // Grid monitor reactive power filter omega
CREATE_PARM(GmPwrFSignTh,       float);     // Grid monitor power factor var sign threshold
// Converter Protection--Voltage
CREATE_PARM(GmOVWV1,            float);     //Grid monitor converter OV omega 1
CREATE_PARM(GmOVWV2,            float);     //Grid monitor converter OV omega 2
CREATE_PARM(GmOVWV3,            float);     //Grid monitor converter OV omega 3
CREATE_PARM(GmOVWV4,            float);     //Grid monitor converter OV omega 4
CREATE_PARM(GmOVMin1,           float);     //Grid monitor converter OV min 1
CREATE_PARM(GmOVMin2,           float);     //Grid monitor converter OV min 2
CREATE_PARM(GmOVMin3,           float);     //Grid monitor converter OV min 3
CREATE_PARM(GmOVMin4,           float);     //Grid monitor converter OV min 4
CREATE_PARM(GmOVTrpThrs1,       float);     //Grid monitor converter OV trip threshold 1
CREATE_PARM(GmOVTrpThrs2,       float);     //Grid monitor converter OV trip threshold 2
CREATE_PARM(GmOVTrpThrs3,       float);     //Grid monitor converter OV trip threshold 3
CREATE_PARM(GmOVTrpThrs4,       float);     //Grid monitor converter OV trip threshold 4
CREATE_PARM(GmUVWV1,            float);     //Grid monitor converter UV omega 1
CREATE_PARM(GmUVWV2,            float);     //Grid monitor converter UV omega 2
CREATE_PARM(GmUVMax1,           float);     //Grid monitor converter UV max 1
CREATE_PARM(GmUVMax2,           float);     //Grid monitor converter UV max 2
CREATE_PARM(GmUVTrpThrs1,       float);     //Grid monitor converter UV trip threshold 1
CREATE_PARM(GmUVTrpThrs2,       float);     //Grid monitor converter UV trip threshold 2
// Converter Protection--Frequency
CREATE_PARM(GmUFOFW1,           float);     //Grid monitor converter under-frequency/over-frequency omega
CREATE_PARM(GmUFTrpThrs1,       float);     //Grid monitor under-frequency trip threshold
CREATE_PARM(GmOFTrpThrs1,       float);     //Grid monitor over-frequency trip threshold
CREATE_PARM(GmUFOFW2,           float);     //Grid monitor converter under-frequency/over-frequency omega
CREATE_PARM(GmUFTrpThrs2,       float);     //Grid monitor under-frequency trip threshold
CREATE_PARM(GmOFTrpThrs2,       float);     //Grid monitor over-frequency trip threshold
CREATE_PARM(GmOFMin,            float);     //Grid monitor converter OV min
CREATE_PARM(GmUFMax,            float);     //Grid monitor converter UV max
//Converter Asymmetry
CREATE_PARM(GmCVAsmThrs1,       float);     // Gm converter asymmetry 1(slow);  threshold
CREATE_PARM(GmCVAsmMin1,        float);     // Gm converter asymmetry minimum 1(slow); 
CREATE_PARM(GmCVAsmW1,          float);     // Gm converter asymmetry omega 1(slow); 
CREATE_PARM(GmCVAsmThrs2,       float);     // Gm converter asymmetry 2(fast);  threshold
CREATE_PARM(GmCVAsmMin2,        float);     // Gm converter asymmetry minimum 2(fast); 
CREATE_PARM(GmCVAsmW2,          float);     // Gm converter asymmetry omeaga 2 (slow); 
CREATE_PARM(GmCVAsmAlmOn,       float);     // Gm converter asymmetry alarm on threshold
CREATE_PARM(GmCVAsmAlmOf,       float);     // Gm converter asymmetry alarm off threshold
CREATE_PARM(GmPhShortOn,        float);     // Gm phase short on level
CREATE_PARM(GmPhShortOff,       float);     // Gm phase short off level
CREATE_PARM(GmAsymRTTm,         float);     // Gm asymmetry ride-through timer
CREATE_PARM(GmAsymRcvTm,        float);     // Gm asymmetry recovery timer
CREATE_PARM(GmVAsmAlmInh,       unsigned);  // Gm converter voltage asymmetry alamm inhibit
// Trip/Alarm types
CREATE_PARM(GmVltPrtSrc,        unsigned);  // Select line to neutral or line to line voltages for UV/OV protection
CREATE_PARM(GmVltPrtType,       unsigned);  // Selects all three phases or one phase for voltage based trips
//Under-voltage
CREATE_PARM(GmUVRlyEnb,         unsigned);  // Enables/disables all enabled UV faults for grid monitor relay
CREATE_PARM(GmUV1RlyEnb,        unsigned);  // Enables/disables UV level 1 faults for grid monitor relay
CREATE_PARM(GmUV2RlyEnb,        unsigned);  // Enables/disables UV level 2 faults for grid monitor relay
CREATE_PARM(GmUV3RlyEnb,        unsigned);  // Enables/disables UV level 3 faults for grid monitor relay
CREATE_PARM(GmUV4RlyEnb,        unsigned);  // Enables/disables UV level 4 faults for grid monitor relay
CREATE_PARM(GmUVCvRlyEnb,       unsigned);  // Enables/disables UV level curve faults for grid monitor relay
CREATE_PARM(GmUVTrpInh,         unsigned);  // Inhibts all grid monitor UV faults
CREATE_PARM(GmUV1TrpInh,        unsigned);  // Inhibts grid monitor UV level 1 faults
CREATE_PARM(GmUV2TrpInh,        unsigned);  // Inhibts grid monitor UV level 2 faults
CREATE_PARM(GmUV3TrpInh,        unsigned);  // Inhibts grid monitor UV level 3 faults
CREATE_PARM(GmUV4TrpInh,        unsigned);  // Inhibts grid monitor UV level 4 faults
CREATE_PARM(GmUVCvTrpInh,       unsigned);  // Inhibts grid monitor UV level curve faults
CREATE_PARM(GmUVAlmThr,         float);     // Under-Voltage alarm threshold
CREATE_PARM(GmUVAlmOff,         float);     // Under-Voltage alarm off threshold
CREATE_PARM(GmUV1TrpThr,        float);     // Under-Voltage level 1 trip threshold
CREATE_PARM(GmUV2TrpThr,        float);     // Under-Voltage level 2 trip threshold
CREATE_PARM(GmUV3TrpThr,        float);     // Under-Voltage level 3 trip threshold
CREATE_PARM(GmUV4TrpThr,        float);     // Under-Voltage level 4 trip threshold
CREATE_PARM(GmUVAlmTm,          float);     // Under-Voltage alarm time
CREATE_PARM(GmUVAlmOffTm,       float);     // Under-Voltage alarm off time
CREATE_PARM(GmUVMaxLvl,         float);     // Under-voltage maximum level
CREATE_PARM(GmUV1TrpTm,         float);     // Under-Voltage level 1 trip threshold
CREATE_PARM(GmUV2TrpTm,         float);     // Under-Voltage level 2 trip threshold
CREATE_PARM(GmUV3TrpTm,         float);     // Under-Voltage level 3 trip threshold
CREATE_PARM(GmUV4TrpTm,         float);     // Under-Voltage level 4 trip threshold
CREATE_PARM(GmUV1Cap,           float);     // Under-voltage capability voltage 1
CREATE_PARM(GmUV2Cap,           float);     // Under-voltage capability voltage 2
CREATE_PARM(GmUV3Cap,           float);     // Under-voltage capability voltage 3
CREATE_PARM(GmUV4Cap,           float);     // Under-voltage capability voltage 4
CREATE_PARM(GmUVCvUpCap,        float);     // Under-voltage capability voltage upper curve
CREATE_PARM(GmUVCvLwrCap,       float);     // Under-voltage capability voltage lower curve
CREATE_PARM(GmUV1CapTm,         float);     // Under-voltage capability time for level 1
CREATE_PARM(GmUV2CapTm,         float);     // Under-voltage capability time for level 2
CREATE_PARM(GmUV3CapTm,         float);     // Under-voltage capability time for level 3
CREATE_PARM(GmUV4CapTm,         float);     // Under-voltage capability time for level 4
CREATE_PARM(GmUVCvUCapTm,       float);     // Under-voltage capability time upper curve
CREATE_PARM(GmUVCvLCapTm,       float);     // Under-voltage capability time lower curve
CREATE_PARM(GmUVCvUpr,          float);     // Under-voltage  curve per unit voltage upper level
CREATE_PARM(GmUVCvLwr,          float);     // Under-voltage  curve per unit voltage lower level
CREATE_PARM(GmUVCvUprTm,        float);     // Under-voltage  curve upper time 1
CREATE_PARM(GmUVCvLwrTm,        float);     // Under-voltage  curve lower time 2
CREATE_PARM(GmUVCvWV,           float);     // Under-voltage  curve lp filter voltage response
//Over-voltage
CREATE_PARM(GmOVRlyEnb,         unsigned);  // Enables/disables all enabled OV faults for grid monitor relay
CREATE_PARM(GmOV1RlyEnb,        unsigned);  // Enables/disables OV level 1 faults for grid monitor relay
CREATE_PARM(GmOV2RlyEnb,        unsigned);  // Enables/disables OV level 2 faults for grid monitor relay
CREATE_PARM(GmOVFsRlyEnb,       unsigned);  // Enables/disables fast OV fault for grid monitor relay
CREATE_PARM(GmOVAlmThr,         float);     // Over-Voltage alarm threshold
CREATE_PARM(GmOVAlmOff,         float);     // Over-Voltage alarm off threshold
CREATE_PARM(GmOVMinLvl,         float);     // Under-voltage maximum level
CREATE_PARM(GmOV1TrpThr,        float);     // Over-Voltage level 1 trip threshold
CREATE_PARM(GmOV2TrpThr,        float);     // Over-Voltage level 2 trip threshold
CREATE_PARM(GmOVAlmTm,          float);     // Over-Voltage alarm time
CREATE_PARM(GmOVAlmOffTm,       float);     // Over-Voltage alarm off time
CREATE_PARM(GmOV1TrpTm,         float);     // Over-Voltage level 1 trip threshold
CREATE_PARM(GmOV2TrpTm,         float);     // Over-Voltage level 2 trip threshold
CREATE_PARM(GmOV1Cap,           float);     // Over-voltage capability voltage 1
CREATE_PARM(GmOV2Cap,           float);     // Over-voltage capability voltage 2
CREATE_PARM(GmOV1CapTm,         float);     // Over-voltage capability time 1
CREATE_PARM(GmOV2CapTm,         float);     // Over-voltage capability time 2
//Under-frequency
CREATE_PARM(GmUFRlyEnb,         unsigned);  // Enables/disables all enabled UF faults for grid monitor relay
CREATE_PARM(GmUF1RlyEnb,        unsigned);  // Enables/disables UF level 1 faults for grid monitor relay
CREATE_PARM(GmUF2RlyEnb,        unsigned);  // Enables/disables UF level 2 faults for grid monitor relay
CREATE_PARM(GmUF3RlyEnb,        unsigned);  // Enables/disables UF level 3 faults for grid monitor relay
CREATE_PARM(GmUFTrpInh,         unsigned);  // Inhibts all grid monitor UF faults
CREATE_PARM(GmUF1TrpInh,        unsigned);  // Inhibts grid monitor UF level 1 faults
CREATE_PARM(GmUF2TrpInh,        unsigned);  // Inhibts grid monitor UF level 2 faults
CREATE_PARM(GmUF3TrpInh,        unsigned);  // Inhibts grid monitor UF level 3 faults
CREATE_PARM(GmUFAlmThr,         float);     // Under-Frequency alarm threshold
CREATE_PARM(GmUFMaxLvl,         float);     // Under-frequency maximul level
CREATE_PARM(GmUF1TrpThr,        float);     // Under-Frequency level 1 trip threshold
CREATE_PARM(GmUF2TrpThr,        float);     // Under-Frequency level 2 trip threshold
CREATE_PARM(GmUF3TrpThr,        float);     // Under-Frequency level 3 trip threshold
CREATE_PARM(GmUFAlmTm,          float);     // Over-Frequency alarm time
CREATE_PARM(GmUF1TrpTm,         float);     // Over-Frequency level 1 trip threshold
CREATE_PARM(GmUF2TrpTm,         float);     // Over-Frequency level 2 trip threshold
CREATE_PARM(GmUF3TrpTm,         float);     // Over-Frequency level 3 trip threshold
CREATE_PARM(GmUF1Cap,           float);     // Under-frequency capability frequency 1
CREATE_PARM(GmUF2Cap,           float);     // Under-frequency capability frequency 2
CREATE_PARM(GmUF3Cap,           float);     // Under-frequency capability frequency 3
CREATE_PARM(GmUF1CapTm,         float);     // Under-frequency capability time 1
CREATE_PARM(GmUF2CapTm,         float);     // Under-frequency capability time 2
CREATE_PARM(GmUF3CapTm,         float);     // Under-frequency capability time 3
//Over-frequency
CREATE_PARM(GmOFRlyEnb,         unsigned);  // Enables/disables all enabled OF faults for grid monitor relay
CREATE_PARM(GmOF1RlyEnb,        unsigned);  // Enables/disables OF level 1 faults for grid monitor relay
CREATE_PARM(GmOF2RlyEnb,        unsigned);  // Enables/disables OF level 2 faults for grid monitor relay
CREATE_PARM(GmOF1TrpInh,        unsigned);  // Inhibts grid monitor OF level 1 faults
CREATE_PARM(GmOF2TrpInh,        unsigned);  // Inhibts grid monitor OF level 2 faults
CREATE_PARM(GmOFAlmThr,         float);     // Over-Frequency alarm threshold
CREATE_PARM(GmOFMinLvl,         float);     // Under-frequency maximul level
CREATE_PARM(GmOF1TrpThr,        float);     // Over-Frequency level 1 trip threshold
CREATE_PARM(GmOF2TrpThr,        float);     // Over-Frequency level 2 trip threshold
CREATE_PARM(GmOFAlmTm,          float);     // Over-Frequency alarm time
CREATE_PARM(GmOF1TrpTm,         float);     // Over-Frequency level 1 trip threshold
CREATE_PARM(GmOF2TrpTm,         float);     // Over-Frequency level 2 trip threshold

// Voltage Asymmetry
CREATE_PARM(GmVAsyRlyEnb,       unsigned);  // Enables/disables Negative sequence fault for grid monitor relay
CREATE_PARM(GmVAsyTrpThr,       float);     // Negative sequence trip threshold
CREATE_PARM(GmVAsyTrpTm,        float);     // Negative sequence trip time
CREATE_PARM(GmVAsyAlmTm,        float);     // Negative sequence alarm time
// GridMonitorFundamentalCalcsT3
CREATE_PARM(GmNegSeqScl,        float);     // Gm negative sequence scale factor
// GridMonitorConverterProtectionT2
// Fast over-voltage
CREATE_PARM(GmVMgFstUpW,        float);     // Gm vmag fast pk det up omega
CREATE_PARM(GmVMgFstDnW,        float);     // Gm filter vmag fast pk det dn omega
CREATE_PARM(GmFastOVW,          float);     // Gm filter vmag fast ov omega
CREATE_PARM(GmFastOVThrs,       float);     // Gm filter vmag fast over-voltage level
// GridMonitorExcessivePower
CREATE_PARM(GmEPwrRlyEnb,       unsigned);  // Enables/disables excessive power fault for grid monitor relay
CREATE_PARM(GmExPwrTrpTh,       float);     // Excess power high trip threshold
CREATE_PARM(GmExPwrOnThr,       float);     // Excess power alarm on threshold
CREATE_PARM(GmExPwrOfThr,       float);     // Excess power alarm off threshold
CREATE_PARM(GmExPwrFilW,        float);     // Excess power filter omega
// GridMonitorReversePower
CREATE_PARM(GmRPwrRlyEnb,       unsigned);  // Enables/disables reverse power fault for grid monitor relay
CREATE_PARM(GmRevPwrThr,        float);     // Reverse power high trip threshold
CREATE_PARM(GmRevPwrThrDbCel,   float);     // Reverse power high trip threshold when DB cell test is in progress
CREATE_PARM(GmRevPwrFilW,       float);     // Reverse power filter omega
// GridMonitorFeedbackVerfication
CREATE_PARM(GmAuxPwrW,          float);     // Grid monitor aux real power  omega
CREATE_PARM(GmAuxQPwrW,         float);     // Grid monitor aux reactive power omega
CREATE_PARM(GmOF1Cap,           float);     // Over-frequency capability frequency 1
CREATE_PARM(GmOF1CapTm,         float);     // Over-frequency capability time 1
CREATE_PARM(GmOF2Cap,           float);     // Over-frequency capability frequency 2
CREATE_PARM(GmOF2CapTm,         float);     // Over-frequency capability time 2

// Diagnostics
//------------

// Under voltage protection
CREATE_DIAG(GmUV1TrpLN);          // Grid monitor UV L-N Trip, level 1
CREATE_DIAG(GmUV1TrpLL);          // Grid monitor UV L-L Trip, level 1
CREATE_DIAG(GmUV1Trp1N);          // Grid monitor UV 1-N Trip, level 1
CREATE_DIAG(GmUV1Trp2N);          // Grid monitor UV 2-N Trip, level 1
CREATE_DIAG(GmUV1Trp3N);          // Grid monitor UV 3-N Trip, level 1
CREATE_DIAG(GmUV1Trp12);          // Grid monitor UV 1-2 Trip, level 1
CREATE_DIAG(GmUV1Trp23);          // Grid monitor UV 2-3 Trip, level 1
CREATE_DIAG(GmUV1Trp31);          // Grid monitor UV 3-1 Trip, level 1

CREATE_DIAG(GmUV2TrpLN);          // Grid monitor UV L-N Trip, level 2
CREATE_DIAG(GmUV2TrpLL);          // Grid monitor UV L-L Trip, level 2
CREATE_DIAG(GmUV2Trp1N);          // Grid monitor UV 1-N Trip, level 2
CREATE_DIAG(GmUV2Trp2N);          // Grid monitor UV 2-N Trip, level 2
CREATE_DIAG(GmUV2Trp3N);          // Grid monitor UV 3-N Trip, level 2
CREATE_DIAG(GmUV2Trp12);          // Grid monitor UV 1-2 Trip, level 2
CREATE_DIAG(GmUV2Trp23);          // Grid monitor UV 2-3 Trip, level 2
CREATE_DIAG(GmUV2Trp31);          // Grid monitor UV 3-1 Trip, level 2

CREATE_DIAG(GmUV3TrpLN);          // Grid monitor UV L-N Trip, level 3
CREATE_DIAG(GmUV3TrpLL);          // Grid monitor UV L-L Trip, level 3
CREATE_DIAG(GmUV3Trp1N);          // Grid monitor UV 1-N Trip, level 3
CREATE_DIAG(GmUV3Trp2N);          // Grid monitor UV 2-N Trip, level 3
CREATE_DIAG(GmUV3Trp3N);          // Grid monitor UV 3-N Trip, level 3
CREATE_DIAG(GmUV3Trp12);          // Grid monitor UV 1-2 Trip, level 3
CREATE_DIAG(GmUV3Trp23);          // Grid monitor UV 2-3 Trip, level 3
CREATE_DIAG(GmUV3Trp31);          // Grid monitor UV 3-1 Trip, level 3

CREATE_DIAG(GmUV4TrpLN);          // Grid monitor UV L-N Trip, level 4
CREATE_DIAG(GmUV4TrpLL);          // Grid monitor UV L-L Trip, level 4
CREATE_DIAG(GmUV4Trp1N);          // Grid monitor UV 1-N Trip, level 4
CREATE_DIAG(GmUV4Trp2N);          // Grid monitor UV 2-N Trip, level 4
CREATE_DIAG(GmUV4Trp3N);          // Grid monitor UV 3-N Trip, level 4
CREATE_DIAG(GmUV4Trp12);          // Grid monitor UV 1-2 Trip, level 4
CREATE_DIAG(GmUV4Trp23);          // Grid monitor UV 2-3 Trip, level 4
CREATE_DIAG(GmUV4Trp31);          // Grid monitor UV 3-1 Trip, level 4

CREATE_DIAG(GmUV5Trp1N);          // Grid monitor UV L-N Trip, level 5
CREATE_DIAG(GmUV5Trp12);          // Grid monitor UV L-L Trip, level 5
CREATE_DIAG(GmUV5Trp2N);          // Grid monitor UV 1-N Trip, level 5
CREATE_DIAG(GmUV5Trp23);          // Grid monitor UV 2-N Trip, level 5
CREATE_DIAG(GmUV5Trp3N);          // Grid monitor UV 3-N Trip, level 5
CREATE_DIAG(GmUV5Trp31);          // Grid monitor UV 1-2 Trip, level 5
CREATE_DIAG(GmUV5TrpLN);          // Grid monitor UV 2-3 Trip, level 5
CREATE_DIAG(GmUV5TrpLL);          // Grid monitor UV 3-1 Trip, level 5

CREATE_DIAG(GmUV6Trp1N);          // Grid monitor UV L-N Trip, level 6
CREATE_DIAG(GmUV6Trp12);          // Grid monitor UV L-L Trip, level 6
CREATE_DIAG(GmUV6Trp2N);          // Grid monitor UV 1-N Trip, level 6
CREATE_DIAG(GmUV6Trp23);          // Grid monitor UV 2-N Trip, level 6
CREATE_DIAG(GmUV6Trp3N);          // Grid monitor UV 3-N Trip, level 6
CREATE_DIAG(GmUV6Trp31);          // Grid monitor UV 1-2 Trip, level 6
CREATE_DIAG(GmUV6TrpLN);          // Grid monitor UV 2-3 Trip, level 6
CREATE_DIAG(GmUV6TrpLL);          // Grid monitor UV 3-1 Trip, level 6

CREATE_DIAG(GmUVAlmLN);           // Grid monitor UV L-N alarm
CREATE_DIAG(GmUVAlmLL);           // Grid monitor UV L-L alarm
CREATE_DIAG(GmUVAlm1N);           // Grid monitor UV 1-N alarm
CREATE_DIAG(GmUVAlm2N);           // Grid monitor UV 2-N alarm
CREATE_DIAG(GmUVAlm3N);           // Grid monitor UV 3-N alarm
CREATE_DIAG(GmUVAlm12);           // Grid monitor UV 1-2 alarm
CREATE_DIAG(GmUVAlm23);           // Grid monitor UV 2-3 alarm
CREATE_DIAG(GmUVAlm31);           // Grid monitor UV 3-1 alarm

CREATE_DIAG(GmUVFstTrp1N);
CREATE_DIAG(GmUVFstTrp2N);
CREATE_DIAG(GmUVFstTrp3N);
CREATE_DIAG(GmUVFstTrp12);
CREATE_DIAG(GmUVFstTrp23);
CREATE_DIAG(GmUVFstTrp31);
CREATE_DIAG(GmUVFstTrpLN);
CREATE_DIAG(GmUVFstTrpLL);

CREATE_DIAG(GmUVCvTrpLN);         // Grid monitor UV L-N Trip, time curve
CREATE_DIAG(GmUVCvTrpLL);         // Grid monitor UV L-L Trip, time curve
CREATE_DIAG(GmUVCvTrp1N);         // Grid monitor UV 1-N Trip, time curve
CREATE_DIAG(GmUVCvTrp2N);         // Grid monitor UV 2-N Trip, time curve
CREATE_DIAG(GmUVCvTrp3N);         // Grid monitor UV 3-N Trip, time curve
CREATE_DIAG(GmUVCvTrp12);         // Grid monitor UV 1-2 Trip, time curve
CREATE_DIAG(GmUVCvTrp23);         // Grid monitor UV 2-3 Trip, time curve
CREATE_DIAG(GmUVCvTrp31);         // Grid monitor UV 3-1 Trip, time curve

CREATE_DIAG(GmUVCfgTrp);          // Grid monitor under-voltage protection configuration trip
CREATE_DIAG(GmCnvUVFlt);          // Grid monitor converter under-voltage fault

// Over voltage protection
CREATE_DIAG(GmOV1TrpLN);          // Grid monitor OV L-N Trip, level 1
CREATE_DIAG(GmOV1TrpLL);          // Grid monitor OV L-L Trip, level 1
CREATE_DIAG(GmOV1Trp1N);          // Grid monitor OV 1-N Trip, level 1
CREATE_DIAG(GmOV1Trp2N);          // Grid monitor OV 2-N Trip, level 1
CREATE_DIAG(GmOV1Trp3N);          // Grid monitor OV 3-N Trip, level 1
CREATE_DIAG(GmOV1Trp12);          // Grid monitor OV 1-2 Trip, level 1
CREATE_DIAG(GmOV1Trp23);          // Grid monitor OV 2-3 Trip, level 1
CREATE_DIAG(GmOV1Trp31);          // Grid monitor OV 3-1 Trip, level 1

CREATE_DIAG(GmOV2TrpLN);          // Grid monitor OV L-N Trip, level 2
CREATE_DIAG(GmOV2TrpLL);          // Grid monitor OV L-L Trip, level 2
CREATE_DIAG(GmOV2Trp1N);          // Grid monitor OV 1-N Trip, level 2
CREATE_DIAG(GmOV2Trp2N);          // Grid monitor OV 2-N Trip, level 2
CREATE_DIAG(GmOV2Trp3N);          // Grid monitor OV 3-N Trip, level 2
CREATE_DIAG(GmOV2Trp12);          // Grid monitor OV 1-2 Trip, level 2
CREATE_DIAG(GmOV2Trp23);          // Grid monitor OV 2-3 Trip, level 2
CREATE_DIAG(GmOV2Trp31);          // Grid monitor OV 3-1 Trip, level 2

CREATE_DIAG(GmOV3Trp1N);          // Grid monitor OV L-N Trip, level 3
CREATE_DIAG(GmOV3Trp12);          // Grid monitor OV L-L Trip, level 3
CREATE_DIAG(GmOV3Trp2N);          // Grid monitor OV 1-N Trip, level 3
CREATE_DIAG(GmOV3Trp23);          // Grid monitor OV 2-N Trip, level 3
CREATE_DIAG(GmOV3Trp3N);          // Grid monitor OV 3-N Trip, level 3
CREATE_DIAG(GmOV3Trp31);          // Grid monitor OV 1-2 Trip, level 3
CREATE_DIAG(GmOV3TrpLN);          // Grid monitor OV 2-3 Trip, level 3
CREATE_DIAG(GmOV3TrpLL);          // Grid monitor OV 3-1 Trip, level 3

CREATE_DIAG(GmOV4Trp1N);          // Grid monitor OV L-N Trip, level 4
CREATE_DIAG(GmOV4Trp12);          // Grid monitor OV L-L Trip, level 4
CREATE_DIAG(GmOV4Trp2N);          // Grid monitor OV 1-N Trip, level 4
CREATE_DIAG(GmOV4Trp23);          // Grid monitor OV 2-N Trip, level 4
CREATE_DIAG(GmOV4Trp3N);          // Grid monitor OV 3-N Trip, level 4
CREATE_DIAG(GmOV4Trp31);          // Grid monitor OV 1-2 Trip, level 4
CREATE_DIAG(GmOV4TrpLN);          // Grid monitor OV 2-3 Trip, level 4
CREATE_DIAG(GmOV4TrpLL);          // Grid monitor OV 3-1 Trip, level 4

CREATE_DIAG(GmOVAlmLN);           // Grid monitor OV L-N alarm
CREATE_DIAG(GmOVAlmLL);           // Grid monitor OV L-L alarm
CREATE_DIAG(GmOVAlm1N);           // Grid monitor OV 1-N alarm
CREATE_DIAG(GmOVAlm2N);           // Grid monitor OV 2-N alarm
CREATE_DIAG(GmOVAlm3N);           // Grid monitor OV 3-N alarm
CREATE_DIAG(GmOVAlm12);           // Grid monitor OV 1-2 alarm
CREATE_DIAG(GmOVAlm23);           // Grid monitor OV 2-3 alarm
CREATE_DIAG(GmOVAlm31);           // Grid monitor OV 3-1 alarm

CREATE_DIAG(GmOVCfgTrp);          // Grid monitor over-voltage protection configuration trip
CREATE_DIAG(GmCnvOv1DiscAlm);     // Grid monitor converter over-voltage alarm 1
CREATE_DIAG(GmCnvOv2DiscAlm);     // Grid monitor converter over-voltage alarm 2
CREATE_DIAG(GmCnvOv3DiscAlm);     // Grid monitor converter over-voltage alarm 3
CREATE_DIAG(GmCnvOv4DiscAlm);     // Grid monitor converter over-voltage alarm 4
CREATE_DIAG(GmCnvOv1Flt);         // Grid monitor converter over-voltage fault 1
CREATE_DIAG(GmCnvOv2Flt);         // Grid monitor converter over-voltage fault 2
CREATE_DIAG(GmCnvOv3Flt);         // Grid monitor converter over-voltage fault 3
CREATE_DIAG(GmCnvOv4Flt);         // Grid monitor converter over-voltage fault 4
CREATE_DIAG(GmFastOvDiscAlm);     // Grid monitor fast over voltage Q1 disconnect alarm

// Voltage asymetry protection
CREATE_DIAG(GmVAsyAlm);           // Grid monitor voltage asymmetry alarm
CREATE_DIAG(GmVAsyTrp);           // Grid monitor voltage asymmetry trip
CREATE_DIAG(GmVAsy2Alm);          // Grid monitor voltage asymmetry alarm, level 2
CREATE_DIAG(GmVAsy2Trp);          // Grid monitor voltage asymmetry trip, level 2

// Power based protection
CREATE_DIAG(GmExPwrAlm);          // Grid monitor excessive power alarm
CREATE_DIAG(GmExPwrTrp);          // Grid monitor excessive power trip
CREATE_DIAG(GmRevPwrTrp);         // Grid monitor reverse power trip

// Under-frequency protection
CREATE_DIAG(GmUFAlm);             // Grid monitor UF alarm
CREATE_DIAG(GmUF1Trp);            // Grid monitor UF  Trip, level 1
CREATE_DIAG(GmUF2Trp);            // Grid monitor UF  Trip, level 2
CREATE_DIAG(GmUF3Trp);            // Grid monitor UF  Trip, level 3
CREATE_DIAG(GmUFCfgTrp);          // Grid monitor under-frequency protection configuration trip
CREATE_DIAG(GmCnvUFFlt);          // Grid monitor converter under-frequency fault

// Over-frequency protection
CREATE_DIAG(GmOFAlm);             // Grid monitor OF alarm
CREATE_DIAG(GmOF1Trp);            // Grid monitor OF  Trip, level 1
CREATE_DIAG(GmOF2Trp);            // Grid monitor OF  Trip, level 2
CREATE_DIAG(GmOFCfgTrp);          // Grid monitor over-frequency protection configuration trip
CREATE_DIAG(GmCnvOFFlt);          // Grid monitor converter over-frequncy fault

// Protection summary
CREATE_DIAG(GmFrqVltStop);         // Grid monitor freq/volt stop summary bits

CREATE_DIAG(L_AcLineTran);          // Ac line transient
CREATE_DIAG(L_AcLineWdog);          // Ac line watch dog
CREATE_DIAG(L_FixFrqTOut);          // Fix frequency timeout
CREATE_DIAG(GmCnvVAsmAlm);          // Converter voltage asym alarm
CREATE_DIAG(GmCnvVAsmFlt);          // Converter voltage asym fault
CREATE_DIAG(GmTocAlm);              // Grid monitor timed over-current alarm
CREATE_DIAG(GmTocFlt);              // Grid monitor timed over-current Fault
CREATE_DIAG(GmRevPhSeq);            // Grid reverse phase sequence

CREATE_DIAG(GmFastOverV);               // Grid monitor fast over-voltage

// Variables
//----------

// Phase lock loop regulator
CREATE_PUBVAR(GmAngElecCmd  , float    ) ;           // electrical command angle
CREATE_PUBVAR(GmAngElecFbk  , float    ) ;           // electrical feedback angle
CREATE_PUBVAR(GmLineVHiTmr  , float    ) ;           // Gm voltage high timer
CREATE_PUBVAR(GmLineVLoTmr  , float    ) ;           // Gm voltage low timer
CREATE_PUBVAR(GmLineVShTmr  , float    ) ;           // Gm voltage short timer
CREATE_PUBVAR(GmLnVShOfTmr  , float    ) ;           // Gm voltage short off timer
CREATE_PUBVAR(GmLnVShOfTr2  , float    ) ;           // Gm voltage short off timer 2
CREATE_PUBVAR(GmPllDelAng   , float    ) ;           // phase lock loop delta angle
CREATE_PUBVAR(GmPllErrInt   , float    ) ;           // pll error integrator
CREATE_PUBVAR(GmPllVyErrPu  , float    ) ;           // Pll err per unit
CREATE_PUBVAR(GmPllPrvTmr   , float    ) ;           // Pll prove timer
CREATE_PUBVAR(GmPllregStep  , float    ) ;           // Pll regulator step
CREATE_PUBVAR(GmPllVyErr    , float    ) ;           // phase lock loop GmVy error
CREATE_PUBVAR(GmPllVyFbk    , float    ) ;           // Pll Vy feedback --steered by P.gmPllVyFbkSw
CREATE_PUBVAR(GmPllW        , float    ) ;           // phase lock loop frequency
CREATE_PUBVAR(GmPllWAvg     , float    ) ;           // phase lock loop frequency --average of last two passes
CREATE_PUBVAR(GmPllWInt     , float    ) ;           // phase lock loop frequency intergrator
CREATE_PUBVAR(GmPllWIntT2   , float    ) ;           // phase lock loop frequency intergrator
CREATE_PUBVAR(GmPllWZ1      , float    ) ;           // phase lock loop frequency -- last pass
CREATE_PUBVAR(GmPllWT2      , float    ) ;           // phase lock loop frequency in task2
CREATE_PUBVAR(GmPllWT3      , float    ) ;           // phase lock loop frequency in task3
CREATE_PUBVAR(GmPllState    , int      ) ;           // Pll gain/clamp state variable
CREATE_PUBVAR(GmLineVHi     , unsigned ) = false;    // Gm voltage high
CREATE_PUBVAR(GmLineVLo     , unsigned ) = false;    // Gm voltage low
CREATE_PUBVAR(GmLineVShort  , unsigned ) = false;    // Gm voltage short
CREATE_PUBVAR(GmNSeqHi      , unsigned ) ;           // Negative sequence is high
CREATE_PUBVAR(GmPllErInLim  , unsigned ) ;           // pll error in limit
CREATE_PUBVAR(GmPllErrHi    , unsigned ) = false;    // Pll error is high (above zero)
CREATE_PUBVAR(GmPllFrcIni   , unsigned ) ;           // Force pll to init
CREATE_PUBVAR(GmPllHot      , unsigned ) = false;    // Using hot pll gains
CREATE_PUBVAR(GmPllProven   , unsigned ) ;           // Pll proven
CREATE_PUBVAR(GmPllPrvFstp  , unsigned ) ;           // Pll prove first pass flag
CREATE_PUBVAR(GmPllPrvPrm   , unsigned ) = false;    // Pll prove permissive (final)
CREATE_PUBVAR(GmPllPrvPrm0  , unsigned ) = false;    // Pll prove permissive (initial)
CREATE_PUBVAR(GmFrqVltStopAct,unsigned ) = false;    // Grid monitor stop active to simulate turbine control stop in local mode



// Remote fault Vars
CREATE_PUBVAR(GmPllShftEnb  , unsigned );            // Gm pll shift enable
CREATE_PUBVAR(GmPllShftV1   ,    float );            // Gm pll shift per unit voltage min
CREATE_PUBVAR(GmPllShftV2   ,    float );            // Gm pll shift per unit voltage min 2
CREATE_PUBVAR(GmPllAngShft  ,    float );            // Gm pll shift for pll angle (filtered)
CREATE_PUBVAR(GmPllShftTmr  ,    float );            // Gm pll shift timer for pll angle adjustment
// State Machine Vars
CREATE_PUBVAR(GmPllIntGn    ,    float);             // phase lock loop dynamic integral gain
CREATE_PUBVAR(GmPllPrpGn    ,    float);             // phase lock loop dynamic proportional gn
CREATE_PUBVAR(GmPllWMax     ,    float);             // maximum allowed pll freqency
CREATE_PUBVAR(GmPllWMin     ,    float);             // minimum allowed pll freqency
CREATE_PUBVAR(GmPllWMaxInt  ,    float);             // maximum allowed pll integrator frquency
CREATE_PUBVAR(GmPllWMinInt  ,    float);             // minimum allowed pll integrator freqency
CREATE_PUBVAR(GmPllWTrack   ,    float);             // Pll frequency tracker
CREATE_PUBVAR(GmPllW4RAF    ,    float);             // Pll frequency tracker for RAF
CREATE_PUBVAR(GmFixFrqTmr   ,    float);             // Fixed frequency timer
CREATE_PUBVAR(GmHotDynTmr   ,    float);             // Hot dynamic timer
CREATE_PUBVAR(GmPllStateZ1  ,    unsigned );         // Transition to hot fix freqency state 1 old

// Task 2 Gm monitor vars
CREATE_PUBVAR(GmVxLineErr  , float   );              // error for GmVxLine
CREATE_PUBVAR(GmVxVyImb    , float   );              // GmVx,GmVy phase imbalance
CREATE_PUBVAR(GmPllErrRef  , float   );              // phase imbalance reference
CREATE_PUBVAR(GmPllErr     , float   );              // phase imbalance
CREATE_PUBVAR(GmVxyImbFil  , float   );              // phase imbalance volts
// DO NOT ZERO next 4 pll gains
// DO NOT ZERO previous 4 pll gains
CREATE_PUBVAR(GmAcLinePeak  , float   );             // Ac Gm peak magnitude
CREATE_PUBVAR(GmPllErSetpt  ,  float  );             // pll error setpoint
CREATE_PUBVAR(GmAcLineUp    ,  unsigned  );          // ac Gm peak detection error positive
CREATE_PUBVAR(GmAngFbkInit  ,  unsigned )= false;    // Pll angle init signal
CREATE_PUBVAR(GmRevSeq      ,  unsigned  );          // reverse phase sequence boolean

CREATE_PUBVAR(GmPllVyComp   ,float );                // Phase lock loop comp for GmVy
CREATE_PUBVAR(GmVxErrFil    ,float );                // X-component filtered error
CREATE_PUBVAR(GmVyErrFil    ,float );                // Y-component filtered error
CREATE_PUBVAR(GmAcWdogTmr   ,float );                // pll error timer
CREATE_PUBVAR(GmPllTrckTmr  ,float );                // Pll frequency track timer
CREATE_PUBVAR(GmAngFbTrack  ,float );                // Angle elec feedback Tracker

// Zero voltage ridethrough vars
CREATE_PUBVAR(GmPllStateZ2  ,unsigned );             // Transition to hot fix freqency state 2 old
CREATE_PUBVAR(GmLVRTRcyIp   ,unsigned );             // Low voltage ridegthrough recovery in progress

// Positive/Negative Sequence Discriminator Vars
CREATE_PUBVAR(GmDscmVxPos   , float );               // Positive Sequence Discrimator x-component output
CREATE_PUBVAR(GmDscmVyPos   , float );               // Positive Sequence Discrimator y-component output
CREATE_PUBVAR(GmDscmVxPxy   , float );               // Positive Sequence Discrimator x-component of a/b to x-y output
CREATE_PUBVAR(GmDscmVyPxy   , float );               // Positive Sequence Discrimator y-component of a/b to x-y output
CREATE_PUBVAR(GmDscmVxPFil  , float );               // Positive Sequence Discrimated x-component output, filtered
CREATE_PUBVAR(GmDscmVyPFil  , float );               // Positive Sequence Discrimated y-component output, filtered
CREATE_PUBVAR(GmDscmVxPGn   , float );               // Positive Sequence Discrimator gain for lp filter of GmDscmVxPos
CREATE_PUBVAR(GmDscmVyPGn   , float );               // Positive Sequence Discrimator gain for lp filter of GmDscmVyPos
CREATE_PUBVAR(GmDscmVxNeg   , float );               // Negative Sequence Discrimator x-component output
CREATE_PUBVAR(GmDscmVyNeg   , float );               // Negative Sequence Discrimator y-component output
CREATE_PUBVAR(GmDscmVxNab   , float );               // Negative Sequence Discrimator x-component of a/b to x-y input
CREATE_PUBVAR(GmDscmVyNab   , float );               // Negative Sequence Discrimator y-component of a/b to x-y input
CREATE_PUBVAR(GmDscmVxNFil  , float );               // Negative Sequence Discrimated x-component output, filtered
CREATE_PUBVAR(GmDscmVyNFil  , float );               // Negative Sequence Discrimated y-component output, filtered
CREATE_PUBVAR(GmDscmVxNGn   , float );               // Negative Sequence Discrimator gain for lp filter of GmDscmVxPos
CREATE_PUBVAR(GmDscmVyNGn   , float );               // Negative Sequence Discrimator gain for lp filter of GmDscmVyPos

// Demodulator
SinCosTyp GmSinCosFbk;                               // sine/cosine structure for angle feedback
SinCosTyp GmSinCosCmd;                               // sine/cosine structure for angle command
CREATE_PUBVAR(GmCos2Ang     , float );               // Cosine (2*Angle)
CREATE_PUBVAR(GmSin2Ang     , float );               // Sin (2*Angle)
CREATE_PUBVAR(GmCosAngFbT2  , float );               //Cos (AngleT2)
CREATE_PUBVAR(GmSinAngFbT2  , float );               //Sin (AngleT2)

// Gm demodulated feedbacks temporally defined here until they become fully implemented
CREATE_PUBVAR(GmVxFbk      ,    float );       // Gm X-voltage feedback
CREATE_PUBVAR(GmVyFbk      ,    float );       // Gm Y-voltage feedback
CREATE_PUBVAR(GmVxFbkT2    ,    float );       // Gm X-voltage fedback passed to task2
CREATE_PUBVAR(GmVyFbkT2    ,    float );       // Gm Y-voltage fedback passed to task2
CREATE_PUBVAR(GmVxFbkT3    ,    float );       // Gm X-voltage fedback passed to task3
CREATE_PUBVAR(GmVyFbkT3    ,    float );       // Gm Y-voltage fedback passed to task3
CREATE_PUBVAR(GmIxFbk      ,    float );       // Gm X-current feedback
CREATE_PUBVAR(GmIyFbk      ,    float );       // Gm Y-current feedback
CREATE_PUBVAR(GmIxFbkT2    ,    float );       // Gm X-current feedback passed to task2
CREATE_PUBVAR(GmIyFbkT2    ,    float );       // Gm Y-current feedback passed to task2
CREATE_PUBVAR(GmIxFbkT3    ,    float );       // Gm X-current feedback passed to task3
CREATE_PUBVAR(GmIyFbkT3    ,    float );       // Gm Y-current feedback passed to task3
CREATE_PUBVAR(GmPwrT2      ,    float );       // Gm real power task 2
CREATE_PUBVAR(GmQPwrT2     ,    float );       // Gm reactive power task 2
CREATE_PUBVAR(GmVMagT1     ,    float );       // Gm voltage magnitude task 1
CREATE_PUBVAR(GmVMagT2     ,    float );       // Gm voltage magnitude task 2
CREATE_PUBVAR(GmVMagT3     ,    float );       // Gm voltage magnitude task 3
CREATE_PUBVAR(GmVMagPuT1   ,    float );       // Gm voltage magnitude per unit task 1
CREATE_PUBVAR(GmVMagPuT2   ,    float );       // Gm voltage magnitude per unit task 2
CREATE_PUBVAR(GmVMagPuT3   ,    float );       // Gm voltage magnitude per unit task 3
CREATE_PUBVAR(GmIyInst     ,    float );       // Gm Iy calculated independent of pll
CREATE_PUBVAR(GmCosAngFbk  ,    float );       // Gm cosine of angle feedback
CREATE_PUBVAR(GmSinAngFbk  ,    float );       // Gm sine of angle feedback
CREATE_PUBVAR(GmVanAl1     ,    float );       // Gm Van alpha unscaled and unfiltered
CREATE_PUBVAR(GmVbnAl1     ,    float );       // Gm Van alpha unscaled and unfiltered
CREATE_PUBVAR(GmVcnAl1     ,    float );       // Gm Van alpha unscaled and unfiltered
CREATE_PUBVAR(GmVanBe1     ,    float );       // Gm Van beta unscaled and unfiltered
CREATE_PUBVAR(GmVbnBe1     ,    float );       // Gm Van beta unscaled and unfiltered
CREATE_PUBVAR(GmVcnBe1     ,    float );       // Gm Van beta unscaled and unfiltered
CREATE_PUBVAR(GmIaAl1      ,    float );       // Gm Ia alpha unscaled and unfiltered
CREATE_PUBVAR(GmIbAl1      ,    float );       // Gm Ia alpha unscaled and unfiltered
CREATE_PUBVAR(GmIcAl1      ,    float );       // Gm Ia alpha unscaled and unfiltered
CREATE_PUBVAR(GmIaBe1      ,    float );       // Gm Ia beta unscaled and unfiltered
CREATE_PUBVAR(GmIbBe1      ,    float );       // Gm Ia beta unscaled and unfiltered
CREATE_PUBVAR(GmIcBe1      ,    float );       // Gm Ia beta unscaled and unfiltered
CREATE_PUBVAR(GmVanAl2     ,    float );       // Gm Van alpha RAF and scaled output
CREATE_PUBVAR(GmVbnAl2     ,    float );       // Gm Van alpha RAF and scaled output
CREATE_PUBVAR(GmVcnAl2     ,    float );       // Gm Van alpha RAF and scaled output
CREATE_PUBVAR(GmVanBe2     ,    float );       // Gm Van beta  RAF and scaled output
CREATE_PUBVAR(GmVbnBe2     ,    float );       // Gm Van beta  RAF and scaled output
CREATE_PUBVAR(GmVcnBe2     ,    float );       // Gm Van beta  RAF and scaled output
CREATE_PUBVAR(GmIaAl2      ,    float );       // Gm Ia alpha  RAF and scaled output
CREATE_PUBVAR(GmIbAl2      ,    float );       // Gm Ia alpha  RAF and scaled output
CREATE_PUBVAR(GmIcAl2      ,    float );       // Gm Ia alpha  RAF and scaled output
CREATE_PUBVAR(GmIaBe2      ,    float );       // Gm Ia beta   RAF and scaled output
CREATE_PUBVAR(GmIbBe2      ,    float );       // Gm Ia beta   RAF and scaled output
CREATE_PUBVAR(GmIcBe2      ,    float );       // Gm Ia beta   RAF and scaled output


CREATE_PUBVAR(IaGridDeltT1 ,  float );
CREATE_PUBVAR(IbGridDeltT1 ,  float );
CREATE_PUBVAR(IcGridDeltT1 ,  float );
CREATE_PUBVAR(GmRAFIndx    ,  int   );     // RAF index
CREATE_PUBVAR(RAFIndx1st   ,  int   );

//Pll Error stuff
CREATE_PUBVAR(GmAcLnPkT3    ,  float  );   // Ac Gm peak T3
CREATE_PUBVAR(GmPhShort     ,  unsigned ); // Ac Gm phase short active

// Phase Unused
CREATE_PUBVAR(GmWPllRcp     , float );     //Reciprocal of GmPllWT3

// Positive/Negative Sequence Discriminator Vars
CREATE_PUBVAR(GmDscmVMgNPu  , float );     // Negative Sequence Discrimator voltage magnitude per unit
CREATE_PUBVAR(GmDscmAngN    , float );     // Negative Sequence Discrimator voltage angle
CREATE_PUBVAR(GmDscmVMagP   , float );     // Positive Sequence Discrimator voltage magnitude
CREATE_PUBVAR(GmDscmVMagN   , float );     // Negative Sequence Discrimator voltage magnitude
CREATE_PUBVAR(GmIMagT3      , float );     // Gm current mag

CREATE_PUBVAR(GmDscmVxNFl2  ,  float );    // Negative Sequence Discrimated x-component output, filtered, task 2
CREATE_PUBVAR(GmDscmVxNGn2  ,  float );    // Negative Sequence Discrimator gain for lp filter of GmDscmVxPos task 2
CREATE_PUBVAR(GmDscmVyNFl2  ,  float );    // Negative Sequence Discrimated y-component output, filtered, task 2
CREATE_PUBVAR(GmDscmVyNGn2  ,  float );    // Negative Sequence Discrimator gain for lp filter of GmDscmVyPos task 2
CREATE_PUBVAR(GmFreqPu      ,  float );    // Gm per unit frequency
CREATE_PUBVAR(GmFrqNomRcp   ,  float );    // Gm nominal frequency reciprocial 1/rad/sec
CREATE_PUBVAR(GmVllNomRcp   ,  float );    // Gm line-line rms reciprocal
CREATE_PUBVAR(GmVlnNomRcp   ,  float );    // Gm line-neut rms reciprocal

// Grid Monitor stuff
CREATE_PUBVAR(GmGridMonOK, unsigned) =false;// Gm monitor ok
CREATE_PUBVAR(GmV1nAlpha , float  );        // Gm V1n alpha
CREATE_PUBVAR(GmV1nBeta  , float  );        // Gm V1n beta
CREATE_PUBVAR(GmV2nAlpha , float  );        // Gm V2n alpha
CREATE_PUBVAR(GmV2nBeta  , float  );        // Gm V2n beta
CREATE_PUBVAR(GmV3nAlpha , float  );        // Gm V3n alpha
CREATE_PUBVAR(GmV3nBeta  , float  );        // Gm V3n beta
CREATE_PUBVAR(GmI1Alpha  , float  );        // Gm Ia alpha
CREATE_PUBVAR(GmI1Beta   , float  );        // Gm Ia beta
CREATE_PUBVAR(GmI2Alpha  , float  );        // Gm Ib alpha
CREATE_PUBVAR(GmI2Beta   , float  );        // Gm Ib beta
CREATE_PUBVAR(GmI3Alpha  , float  );        // Gm Ic alpha
CREATE_PUBVAR(GmI3Beta   , float  );        // Gm Ic beta
CREATE_PUBVAR(GmVxT3Fil  , float  );        // Gm filtered Vx
CREATE_PUBVAR(GmVyT3Fil  , float  );        // Gm filtered Vy
CREATE_PUBVAR(GmIxT3Fil  , float  );        // Gm filtered Ix
CREATE_PUBVAR(GmIyT3Fil  , float  );        // Gm filtered Iy
CREATE_PUBVAR(GmFreqT3Fil, float  );        // Gm filtered line frequency

CREATE_PUBVAR(GmV1nMag    , float );        // Gm V1n magnitude
CREATE_PUBVAR(GmV2nMag    , float );        // Gm V2n magnitude
CREATE_PUBVAR(GmV3nMag    , float );        // Gm V3n magnitude
CREATE_PUBVAR(GmV12Mag    , float );        // Gm V12 magnitude
CREATE_PUBVAR(GmV23Mag    , float );        // Gm V23 magnitude
CREATE_PUBVAR(GmV31Mag    , float );        // Gm V31 magnitude
CREATE_PUBVAR(GmI1Mag     , float );        // Gm Ia magnitude
CREATE_PUBVAR(GmI2Mag     , float );        // Gm Ib magnitude
CREATE_PUBVAR(GmI3Mag     , float );        // Gm Ic magnitude
CREATE_PUBVAR(GmV1nMagPu  , float );        // Gm 1-n voltage magnitude per unit
CREATE_PUBVAR(GmV2nMagPu  , float );        // Gm 2-n voltage magnitude per unit
CREATE_PUBVAR(GmV3nMagPu  , float );        // Gm 3-n voltage magnitude per unit
CREATE_PUBVAR(GmV12MagPu  , float );        // Gm 1-2 voltage magnitude per unit
CREATE_PUBVAR(GmV23MagPu  , float );        // Gm 2-3 voltage magnitude per unit
CREATE_PUBVAR(GmV31MagPu  , float );        // Gm 3-1 voltage magnitude per unit
CREATE_PUBVAR(GmPwr       , float );        // Gm calculated real power
CREATE_PUBVAR(GmQPwr      , float );        // Gm calculated reactive power
CREATE_PUBVAR(GmSPwr      , float );        // Gm calculated apparant power
CREATE_PUBVAR(GmPwrFactor , float );        // Gm calculated power factor
CREATE_PUBVAR(GmFreq      , float );        // Gm frequency
CREATE_PUBVAR(GmIGndAlSum , float );        // Gm gound current alpha sum
CREATE_PUBVAR(GmIGndBeSum , float );        // Gm gound current beta sum
CREATE_PUBVAR(GmIGndSqr   , float );        // Gm gound current summed and squared
CREATE_PUBVAR(GmIGndAlFil , float );        // Gm gound alpha component filtered
CREATE_PUBVAR(GmIGndBeFil , float );        // Gm gound beta component filtered
CREATE_PUBVAR(GmIGndMag   , float );        // Gm gound current
CREATE_PUBVAR(GmNSeqVMag  , float );        // Gm negative sequence voltage magnitude
CREATE_PUBVAR(GmPSeqVMag  , float );        // Gm positive sequence voltage magnitude
CREATE_PUBVAR(GmNegSeqVPct, float );        // Gm negative sequence per cent voltage
CREATE_PUBVAR(GmPosSeqVPct, float );        // Gm positive sequence per cent voltage
CREATE_PUBVAR(GmVlnMagAvg , float );        // Gm line to nuetral average votlage
CREATE_PUBVAR(GmVllMagAvg , float );        // Gm line to line average votlage
CREATE_PUBVAR(GmIMagAvg   , float );        // Gm average current


// Toc function
CREATE_PUBVAR(GmWTocT3     , float );          // Omega for timed over-current (normal) - task 3
CREATE_PUBVAR(GmWTocFstT3  , float );          // Omega for timed over-current (fast) - task 3
CREATE_PUBVAR(GmWTocT3Dyn  , float );          // Omega for timed over-current - task 3
CREATE_PUBVAR(GmIaRmsMag   , float );          // Phase A current rms magnitude
CREATE_PUBVAR(GmIbRmsMag   , float );          // Phase B current rms magnitude
CREATE_PUBVAR(GmIcRmsMag   , float );          // Phase C current rms magnitude
CREATE_PUBVAR(GmIaRms      , float );          // Phase A rms current
CREATE_PUBVAR(GmIbRms      , float );          // Phase B rms current
CREATE_PUBVAR(GmIcRms      , float );          // Phase C rms current
CREATE_PUBVAR(GmTocHiSel   , float );          // Toc high select
CREATE_PUBVAR(GmTocHiSelSq , float );          // Toc high select squared
CREATE_PUBVAR(TocPresetZ1  , unsigned )=false; // Toc preset z-1
CREATE_PUBVAR(GmTocAlmFlg  , unsigned);        // Grid monitor TOC alam flag

// General grid monitor protection
CREATE_PUBVAR(GmV1MagPu   ,  float ) ;         // Phase 1 voltage, from either line to line or line to neutral
CREATE_PUBVAR(GmV2MagPu   ,  float ) ;         // Phase 2 voltage, from either line to line or line to neutral
CREATE_PUBVAR(GmV3MagPu   ,  float ) ;         // Phase 3 voltage, from either line to line or line to neutral
CREATE_PUBVAR(GmVnMagPuMax,  float ) ;         // Maximum of GmV[1,2,3]MagPu

// Steering
CREATE_PUBVAR(GmVltType     , unsigned );
CREATE_PUBVAR(GmVltSource   , unsigned );

// Grid monitor under-voltage alarms
CREATE_PUBVAR(GmUVAlmTmr1   , float); // Phase 1 undervoltage alarm timer
CREATE_PUBVAR(GmUVAlmTmr2   , float); // Phase 2 undervoltage alarm timer
CREATE_PUBVAR(GmUVAlmTmr3   , float); // Phase 3 undervoltage alarm timer
CREATE_PUBVAR(GmUVAlmTmrO1  , float); // Phase 1 uv alarm off timer
CREATE_PUBVAR(GmUVAlmTmrO2  , float); // Phase 2 uv alarm off timer
CREATE_PUBVAR(GmUVAlmTmrO3  , float); // Phase 3 uv alarm off timer

// Grid monitor under-voltage trips, level 1
CREATE_PUBVAR(GmUV1TrpTmr1  , float); // Level 1, Phase 1 undervoltage trip timer
CREATE_PUBVAR(GmUV1TrpTmr2  , float); // Level 1, Phase 2 undervoltage trip timer
CREATE_PUBVAR(GmUV1TrpTmr3  , float); // Level 1, Phase 3 undervoltage trip timer

// Grid monitor under-voltage trips,  level 2
CREATE_PUBVAR(GmUV2TrpTmr1  , float); // Level 2, Phase 1 undervoltage trip timer
CREATE_PUBVAR(GmUV2TrpTmr2  , float); // Level 2, Phase 2 undervoltage trip timer
CREATE_PUBVAR(GmUV2TrpTmr3  , float); // Level 2, Phase 3 undervoltage trip timer

// Grid monitor under-voltage trips,  level 3
CREATE_PUBVAR(GmUV3TrpTmr1  , float); // Level 3, Phase 1 undervoltage trip timer
CREATE_PUBVAR(GmUV3TrpTmr2  , float); // Level 3, Phase 2 undervoltage trip timer
CREATE_PUBVAR(GmUV3TrpTmr3  , float); // Level 3, Phase 3 undervoltage trip timer
// Grid monitor under-voltage trips,  level 4
CREATE_PUBVAR(GmUV4TrpTmr1  , float); // Level 4, Phase 1 undervoltage trip timer
CREATE_PUBVAR(GmUV4TrpTmr2  , float); // Level 4, Phase 2 undervoltage trip timer
CREATE_PUBVAR(GmUV4TrpTmr3  , float); // Level 4, Phase 3 undervoltage trip timer

// Grid monitor curve under-voltage  variables
CREATE_PUBVAR(GmUVCvVMgFl1  , float); // Curve under-voltage curve volt-sec, phase 1
CREATE_PUBVAR(GmUVCvVMag1   , float); // Curve under-voltage curve under-volt mag, phase 1
CREATE_PUBVAR(GmUVCvVS1     , float); // Curve under-voltage curve volt-sec, phase 1
CREATE_PUBVAR(GmUVCVSDyn1   , float); // Curve under-voltage curve volt-sec threshol, phase 1
CREATE_PUBVAR(GmUVcVSDel1   , float); // Curve under-voltage curve volt-sec delta, phase 1
CREATE_PUBVAR(GmUVCvVMgFl2  , float); // Curve under-voltage curve volt-sec, phase 2
CREATE_PUBVAR(GmUVCvVMag2   , float); // Curve under-voltage curve under-volt mag, phase 2
CREATE_PUBVAR(GmUVCvVS2     , float); // Curve under-voltage curve volt-sec, phase 2
CREATE_PUBVAR(GmUVCVSDyn2   , float); // Curve under-voltage curve volt-sec threshol, phase 2
CREATE_PUBVAR(GmUVcVSDel2   , float); // Curve under-voltage curve volt-sec delta, phase 2
CREATE_PUBVAR(GmUVCvVMgFl3  , float); // Curve under-voltage curve volt-sec, phase 3
CREATE_PUBVAR(GmUVCvVMag3   , float); // Curve under-voltage curve under-volt mag, phase 3
CREATE_PUBVAR(GmUVCvVS3     , float); // Curve under-voltage curve volt-sec, phase 3
CREATE_PUBVAR(GmUVCVSDyn3   , float); // Curve under-voltage curve volt-sec threshol, phase 3
CREATE_PUBVAR(GmUVcVSDel3   , float); // Curve under-voltage curve volt-sec delta, phase 3
CREATE_PUBVAR(GmCvPh1Low    , unsigned) =false; // Curve under-voltage phase 1 low
CREATE_PUBVAR(GmCvPh2Low    , unsigned) =false; // Curve under-voltage phase 2 low
CREATE_PUBVAR(GmCvPh3Low    , unsigned) =false; // Curve under-voltage phase 3 low

CREATE_PUBVAR(GmUVAlmFlg    , unsigned) =false;// Under-voltage alarm flag, any
CREATE_PUBVAR(GmUVAlmFlg1   , unsigned) =false;// Under-voltage alarm flag, phase 1
CREATE_PUBVAR(GmUVAlmFlg2   , unsigned) =false;// Under-voltage alarm flag, phase 2
CREATE_PUBVAR(GmUVAlmFlg3   , unsigned) =false;// Under-voltage alarm flag, phase 3
CREATE_PUBVAR(GmUV1TrpFlg   , unsigned) =false;// Under-voltage level 1 trip flag
CREATE_PUBVAR(GmUV2TrpFlg   , unsigned) =false;// Under-voltage level 2 trip flag
CREATE_PUBVAR(GmUV3TrpFlg   , unsigned) =false;// Under-voltage level 3 trip flag
CREATE_PUBVAR(GmUV4TrpFlg   , unsigned) =false;// Under-voltage level 4 trip flag

CREATE_PUBVAR(GmUVAlmDynThr    , float);
CREATE_PUBVAR(GmUVTrpDynThr1   , float);
CREATE_PUBVAR(GmUVTrpDynThr2   , float);

// Grid monitor curve over-voltage  variables
CREATE_PUBVAR(GmOVAlmFlg    ,unsigned)=false;// Over-voltage alarm flag, any
CREATE_PUBVAR(GmOV1TrpFlg   ,unsigned)=false;// Over-voltage level 1 trip flag
CREATE_PUBVAR(GmOV2TrpFlg   ,unsigned)=false;// Over-voltage level 2 trip flag
CREATE_PUBVAR(GmOVAlmFlg1   ,unsigned)=false;// Over-voltage alarm flag, phase 1
CREATE_PUBVAR(GmOVAlmFlg2   ,unsigned)=false;// Over-voltage alarm flag, phase 2
CREATE_PUBVAR(GmOVAlmFlg3   ,unsigned)=false;// Over-voltage alarm flag, phase 3

// Grid monitor over-voltage alarms
CREATE_PUBVAR(GmOVAlmTmr1   ,float);  // Phase 1 over-voltage alarm timer
CREATE_PUBVAR(GmOVAlmTmr2   ,float);  // Phase 2 over-voltage alarm timer
CREATE_PUBVAR(GmOVAlmTmr3   ,float);  // Phase 3 over-voltage alarm timer
CREATE_PUBVAR(GmOVAlmTmrO1  ,float); // Phase 1 ov alarm off timer
CREATE_PUBVAR(GmOVAlmTmrO2  ,float); // Phase 2 ov alarm off timer
CREATE_PUBVAR(GmOVAlmTmrO3  ,float); // Phase 3 ov alarm off timer

// Grid monitor over-voltage trips, level 1
CREATE_PUBVAR(GmOV1TrpTmr1  ,float); // Level 1, Phase 1 over-voltage trip timer
CREATE_PUBVAR(GmOV1TrpTmr2  ,float); // Level 1, Phase 2 over-voltage trip timer
CREATE_PUBVAR(GmOV1TrpTmr3  ,float); // Level 1, Phase 3 over-voltage trip timer

// Grid monitor over-voltage trips, level 2
CREATE_PUBVAR(GmOV2TrpTmr1  ,float); // Level 2, Phase 1 over-voltage trip timer
CREATE_PUBVAR(GmOV2TrpTmr2  ,float); // Level 2, Phase 2 over-voltage trip timer
CREATE_PUBVAR(GmOV2TrpTmr3  ,float); // Level 2, Phase 3 over-voltage trip timer

// Grid monitor under-frequency  variables
CREATE_PUBVAR(GmUFAlmTmr  , float );  // Under-frequency alarm timer
CREATE_PUBVAR(GmUF1TrpTmr , float );  // Level 1, under-frequency trip timer
CREATE_PUBVAR(GmUF2TrpTmr , float );  // Level 2, under-frequency trip timer
CREATE_PUBVAR(GmUF3TrpTmr , float );  // Level 3, under-frequency trip timer
CREATE_PUBVAR(GmOFAlmTmr  , float );  // Over-frequency alarm timer
CREATE_PUBVAR(GmOF1TrpTmr , float );  // Level 1, over-frequency trip timer
CREATE_PUBVAR(GmOF2TrpTmr , float );  // Level 2, over-frequency trip timer

// Voltage Asymmentry
CREATE_PUBVAR(GmVAsyTrpTmr  , float  )=0.0;// Voltage asymmetry trip timer

// Actual UV, OV, UF, OF thresholds (Parms, with margin)
CREATE_PUBVAR(GmUV1TrpThrA ,float );  //
CREATE_PUBVAR(GmUV2TrpThrA ,float );  //
CREATE_PUBVAR(GmUV3TrpThrA ,float );  //
CREATE_PUBVAR(GmUV4TrpThrA ,float );  //
CREATE_PUBVAR(GmUV5TrpThrA ,float );  //
CREATE_PUBVAR(GmUV6TrpThrA ,float );  //
CREATE_PUBVAR(GmOV1TrpThrA ,float );  //
CREATE_PUBVAR(GmOV2TrpThrA ,float );  //
CREATE_PUBVAR(GmOV3TrpThrA ,float );  //
CREATE_PUBVAR(GmOV4TrpThrA ,float );  //

CREATE_PUBVAR(GmUF1TrpThrA ,float );  //
CREATE_PUBVAR(GmUF2TrpThrA ,float );  //
CREATE_PUBVAR(GmUF3TrpThrA ,float );  //
CREATE_PUBVAR(GmOF1TrpThrA ,float );  //
CREATE_PUBVAR(GmOF2TrpThrA ,float );  //


//Excessive Power
CREATE_PUBVAR(GmExPwrFil    ,float) ;      // Excessiv power filtered

// Reverse Power
CREATE_PUBVAR(GmRevPwrFil   ,float );      // Reverse power filtered

// Feedback Verification
CREATE_PUBVAR(GmAuxPwr     , float );     // Real aux power error
CREATE_PUBVAR(GmAuxPwrFil  , float );     // Real aux power error filtered
CREATE_PUBVAR(GmAuxQPwr    , float );     // Reactive aux power error
CREATE_PUBVAR(GmAuxQPwrFil , float );     // Reactive aux power error filtered

// Fault flags --Duplicate of fault name with "F" as suffix
CREATE_PUBVAR(GmUVAlmLNF    , unsigned) =false;
CREATE_PUBVAR(GmUVAlmLLF    , unsigned) =false;
CREATE_PUBVAR(GmUVAlm1NF    , unsigned) =false;
CREATE_PUBVAR(GmUVAlm2NF    , unsigned) =false;
CREATE_PUBVAR(GmUVAlm3NF    , unsigned) =false;
CREATE_PUBVAR(GmUVAlm12F    , unsigned) =false;
CREATE_PUBVAR(GmUVAlm23F    , unsigned) =false;
CREATE_PUBVAR(GmUVAlm31F    , unsigned) =false;
CREATE_PUBVAR(GmUV1TrpLNF   , unsigned) =false;
CREATE_PUBVAR(GmUV1TrpLLF   , unsigned) =false;
CREATE_PUBVAR(GmUV1Trp1NF   , unsigned) =false;
CREATE_PUBVAR(GmUV1Trp2NF   , unsigned) =false;
CREATE_PUBVAR(GmUV1Trp3NF   , unsigned) =false;
CREATE_PUBVAR(GmUV1Trp12F   , unsigned) =false;
CREATE_PUBVAR(GmUV1Trp23F   , unsigned) =false;
CREATE_PUBVAR(GmUV1Trp31F   , unsigned) =false;
CREATE_PUBVAR(GmUV2TrpLNF   , unsigned) =false;
CREATE_PUBVAR(GmUV2TrpLLF   , unsigned) =false;
CREATE_PUBVAR(GmUV2Trp1NF   , unsigned) =false;
CREATE_PUBVAR(GmUV2Trp2NF   , unsigned) =false;
CREATE_PUBVAR(GmUV2Trp3NF   , unsigned) =false;
CREATE_PUBVAR(GmUV2Trp12F   , unsigned) =false;
CREATE_PUBVAR(GmUV2Trp23F   , unsigned) =false;
CREATE_PUBVAR(GmUV2Trp31F   , unsigned) =false;
CREATE_PUBVAR(GmUV3TrpLNF   , unsigned) =false;
CREATE_PUBVAR(GmUV3TrpLLF   , unsigned) =false;
CREATE_PUBVAR(GmUV3Trp1NF   , unsigned) =false;
CREATE_PUBVAR(GmUV3Trp2NF   , unsigned) =false;
CREATE_PUBVAR(GmUV3Trp3NF   , unsigned) =false;
CREATE_PUBVAR(GmUV3Trp12F   , unsigned) =false;
CREATE_PUBVAR(GmUV3Trp23F   , unsigned) =false;
CREATE_PUBVAR(GmUV3Trp31F   , unsigned) =false;
CREATE_PUBVAR(GmUV4TrpLNF   , unsigned) =false;
CREATE_PUBVAR(GmUV4TrpLLF   , unsigned) =false;
CREATE_PUBVAR(GmUV4Trp1NF   , unsigned) =false;
CREATE_PUBVAR(GmUV4Trp2NF   , unsigned) =false;
CREATE_PUBVAR(GmUV4Trp3NF   , unsigned) =false;
CREATE_PUBVAR(GmUV4Trp12F   , unsigned) =false;
CREATE_PUBVAR(GmUV4Trp23F   , unsigned) =false;
CREATE_PUBVAR(GmUV4Trp31F   , unsigned) =false;
CREATE_PUBVAR(GmUVCvTrpLNF  , unsigned) =false;
CREATE_PUBVAR(GmUVCvTrpLLF  , unsigned) =false;
CREATE_PUBVAR(GmUVCvTrp1NF  , unsigned) =false;
CREATE_PUBVAR(GmUVCvTrp2NF  , unsigned) =false;
CREATE_PUBVAR(GmUVCvTrp3NF  , unsigned) =false;
CREATE_PUBVAR(GmUVCvTrp12F  , unsigned) =false;
CREATE_PUBVAR(GmUVCvTrp23F  , unsigned) =false;
CREATE_PUBVAR(GmUVCvTrp31F  , unsigned) =false;
CREATE_PUBVAR(GmUVCvTrpF    , unsigned) = false;
CREATE_PUBVAR(GmOVAlmLNF    , unsigned) =false;
CREATE_PUBVAR(GmOVAlmLLF    , unsigned) =false;
CREATE_PUBVAR(GmOVAlm1NF    , unsigned) =false;
CREATE_PUBVAR(GmOVAlm2NF    , unsigned) =false;
CREATE_PUBVAR(GmOVAlm3NF    , unsigned) =false;
CREATE_PUBVAR(GmOVAlm12F    , unsigned) =false;
CREATE_PUBVAR(GmOVAlm23F    , unsigned) =false;
CREATE_PUBVAR(GmOVAlm31F    , unsigned) =false;
CREATE_PUBVAR(GmOV1TrpLNF   , unsigned) =false;
CREATE_PUBVAR(GmOV1TrpLLF   , unsigned) =false;
CREATE_PUBVAR(GmOV1Trp1NF   , unsigned) =false;
CREATE_PUBVAR(GmOV1Trp2NF   , unsigned) =false;
CREATE_PUBVAR(GmOV1Trp3NF   , unsigned) =false;
CREATE_PUBVAR(GmOV1Trp12F   , unsigned) =false;
CREATE_PUBVAR(GmOV1Trp23F   , unsigned) =false;
CREATE_PUBVAR(GmOV1Trp31F   , unsigned) =false;
CREATE_PUBVAR(GmOV2TrpLNF   , unsigned) =false;
CREATE_PUBVAR(GmOV2TrpLLF   , unsigned) =false;
CREATE_PUBVAR(GmOV2Trp1NF   , unsigned) =false;
CREATE_PUBVAR(GmOV2Trp2NF   , unsigned) =false;
CREATE_PUBVAR(GmOV2Trp3NF   , unsigned) =false;
CREATE_PUBVAR(GmOV2Trp12F   , unsigned) =false;
CREATE_PUBVAR(GmOV2Trp23F   , unsigned) =false;
CREATE_PUBVAR(GmOV2Trp31F   , unsigned) =false;
CREATE_PUBVAR(GmFastOverVF  , unsigned) =false; //High votlage trip flag for fast ac over-voltage fault
CREATE_PUBVAR(GmUFAlmF      , unsigned) =false;
CREATE_PUBVAR(GmUF1TrpF     , unsigned) =false;
CREATE_PUBVAR(GmUF2TrpF     , unsigned) =false;
CREATE_PUBVAR(GmUF3TrpF     , unsigned) =false;
CREATE_PUBVAR(GmOFAlmF      , unsigned) =false;
CREATE_PUBVAR(GmOF1TrpF     , unsigned) =false;
CREATE_PUBVAR(GmOF2TrpF     , unsigned) =false;
CREATE_PUBVAR(GmVAsyAlmF    , unsigned) =false;
CREATE_PUBVAR(GmVAsyTrpF    , unsigned) =false;
CREATE_PUBVAR(GmExPwrAlmF   , unsigned) =false;
CREATE_PUBVAR(GmExPwrTrpF   , unsigned) =false;
CREATE_PUBVAR(GmRevPwrTrpF  , unsigned) =false;
CREATE_PUBVAR(GmLVRTMon     , unsigned) =false;
CREATE_PUBVAR(GmVoltMon     , unsigned) =false;
CREATE_PUBVAR(GmFreqMon     , unsigned) =false;
CREATE_PUBVAR(GmOV3Cap     ,  float );
CREATE_PUBVAR(GmOV4Cap     ,  float );
CREATE_PUBVAR(GmOV3CapTm   ,  float );
CREATE_PUBVAR(GmOV4CapTm   ,  float );

// Converter Protection
CREATE_PUBVAR(GmCnvFrqFil   , float );
CREATE_PUBVAR(GmCnvOFFil    , float );
CREATE_PUBVAR(GmCnvUFFil    , float );
CREATE_PUBVAR(GmCnvOV1Fil   , float );
CREATE_PUBVAR(GmCnvOV2Fil   , float );
CREATE_PUBVAR(GmCnvOV3Fil   , float );
CREATE_PUBVAR(GmCnvOV4Fil   , float );
CREATE_PUBVAR(GmCnvOVTmr    , float );
CREATE_PUBVAR(GmCnvOV1Pct   , float );
CREATE_PUBVAR(GmCnvOV1Frc   , float );
CREATE_PUBVAR(GmCnvOV1Int   , int   );
CREATE_PUBVAR(GmCnvUV1Fil   , float );
CREATE_PUBVAR(GmCnvUV2Fil   , float );
CREATE_PUBVAR(GmCVAsmFil1   , float );
CREATE_PUBVAR(GmCVAsmFil2   , float );
CREATE_PUBVAR(GmCVAsmFrc    , float );
CREATE_PUBVAR(GmCVAsmInt    , int   );
CREATE_PUBVAR(GmCnvVAsmAlmFlg,  unsigned);  // Converter asymetic voltage alarm flag

// Grid monitor dynamic Toc threshold
CREATE_PUBVAR(GmTocThrDy  ,  float   ); // Gird monitor time  dymanic trip threshold (Amp)
CREATE_PUBVAR(GmTocAlmOnDy,  float   ); // Gird monitor time overover-current-current dymanic alarm on threshold (Amp)
CREATE_PUBVAR(GmTocAlmOffDy, float   ); // Gird monitor time over-current dymanic alarm off threshold (Amp)

CREATE_PUBVAR(GmUV5TrpTmr1 , float   );
CREATE_PUBVAR(GmUV5TrpTmr2 , float   );
CREATE_PUBVAR(GmUV5TrpTmr3 , float   );

CREATE_PUBVAR(GmUV6TrpTmr1 , float   );
CREATE_PUBVAR(GmUV6TrpTmr2 , float   );
CREATE_PUBVAR(GmUV6TrpTmr3 , float   );

CREATE_PUBVAR(GmOV3TrpTmr1 , float   );
CREATE_PUBVAR(GmOV3TrpTmr2 , float   );
CREATE_PUBVAR(GmOV3TrpTmr3 , float   );

CREATE_PUBVAR(GmOV4TrpTmr1 , float   );
CREATE_PUBVAR(GmOV4TrpTmr2 , float   );
CREATE_PUBVAR(GmOV4TrpTmr3 , float   );

CREATE_PUBVAR(GmVAsy2TrpTmr, float   );
CREATE_PUBVAR(GmFrqVltTmr,   float   );

CREATE_PUBVAR(GmUV5TrpFlg  , unsigned) =false;
CREATE_PUBVAR(GmUV5Trp1NF  , unsigned) =false;
CREATE_PUBVAR(GmUV5Trp12F  , unsigned) =false;
CREATE_PUBVAR(GmUV5Trp2NF  , unsigned) =false;
CREATE_PUBVAR(GmUV5Trp23F  , unsigned) =false;
CREATE_PUBVAR(GmUV5Trp3NF  , unsigned) =false;
CREATE_PUBVAR(GmUV5Trp31F  , unsigned) =false;
CREATE_PUBVAR(GmUV5TrpLNF  , unsigned) =false;
CREATE_PUBVAR(GmUV5TrpLLF  , unsigned) =false;

CREATE_PUBVAR(GmUV6TrpFlg  , unsigned) =false;
CREATE_PUBVAR(GmUV6Trp1NF  , unsigned) =false;
CREATE_PUBVAR(GmUV6Trp12F  , unsigned) =false;
CREATE_PUBVAR(GmUV6Trp2NF  , unsigned) =false;
CREATE_PUBVAR(GmUV6Trp23F  , unsigned) =false;
CREATE_PUBVAR(GmUV6Trp3NF  , unsigned) =false;
CREATE_PUBVAR(GmUV6Trp31F  , unsigned) =false;
CREATE_PUBVAR(GmUV6TrpLNF  , unsigned) =false;
CREATE_PUBVAR(GmUV6TrpLLF  , unsigned) =false;


CREATE_PUBVAR(GmOV3TrpFlg  , unsigned) =false;
CREATE_PUBVAR(GmOV3Trp1NF  , unsigned) =false;
CREATE_PUBVAR(GmOV3Trp12F  , unsigned) =false;
CREATE_PUBVAR(GmOV3Trp2NF  , unsigned) =false;
CREATE_PUBVAR(GmOV3Trp23F  , unsigned) =false;
CREATE_PUBVAR(GmOV3Trp3NF  , unsigned) =false;
CREATE_PUBVAR(GmOV3Trp31F  , unsigned) =false;
CREATE_PUBVAR(GmOV3TrpLNF  , unsigned) =false;
CREATE_PUBVAR(GmOV3TrpLLF  , unsigned) =false;

CREATE_PUBVAR(GmOV4TrpFlg  , unsigned) =false;
CREATE_PUBVAR(GmOV4Trp1NF  , unsigned) =false;
CREATE_PUBVAR(GmOV4Trp12F  , unsigned) =false;
CREATE_PUBVAR(GmOV4Trp2NF  , unsigned) =false;
CREATE_PUBVAR(GmOV4Trp23F  , unsigned) =false;
CREATE_PUBVAR(GmOV4Trp3NF  , unsigned) =false;
CREATE_PUBVAR(GmOV4Trp31F  , unsigned) =false;
CREATE_PUBVAR(GmOV4TrpLNF  , unsigned) =false;
CREATE_PUBVAR(GmOV4TrpLLF  , unsigned) =false;

CREATE_PUBVAR(GmVAsy2AlmF  , unsigned) =false;
CREATE_PUBVAR(GmVAsy2TrpF  , unsigned) =false;

CREATE_PUBVAR(GmUVTrpSum   , unsigned) =false;
CREATE_PUBVAR(GmOVTrpSum   , unsigned) =false;
CREATE_PUBVAR(GmVAsyTrpSum , unsigned) =false;
CREATE_PUBVAR(GmUFTrpSum   , unsigned) =false;
CREATE_PUBVAR(GmOFTrpSum   , unsigned) =false;

CREATE_PUBVAR(GmRevPwrThr  , float    );    // Reverse power high trip threshold when DB cell test is in progress (high threshold)

CREATE_PUBVAR(GmUV1TrpAct ,  unsigned);     // Under-voltage 1 trip condition is active
CREATE_PUBVAR(GmUV2TrpAct ,  unsigned);     // Under-voltage 2 trip condition is active
CREATE_PUBVAR(GmUV3TrpAct ,  unsigned);     // Under-voltage 3 trip condition is active
CREATE_PUBVAR(GmUV4TrpAct ,  unsigned);     // Under-voltage 4 trip condition is active
CREATE_PUBVAR(GmUV5TrpAct ,  unsigned);     // Under-voltage 5 trip condition is active
CREATE_PUBVAR(GmUV6TrpAct ,  unsigned);     // Under-voltage 6 trip condition is active
CREATE_PUBVAR(GmUVCvTrpAct,  unsigned);     // Under-voltage curve trip condition is active
CREATE_PUBVAR(GmOV1TrpAct ,  unsigned);     // Over-voltage 1 trip condition is active
CREATE_PUBVAR(GmOV2TrpAct ,  unsigned);     // Over-voltage 2 trip condition is active
CREATE_PUBVAR(GmOV3TrpAct ,  unsigned);     // Over-voltage 3 trip condition is active
CREATE_PUBVAR(GmOV4TrpAct ,  unsigned);     // Over-voltage 4 trip condition is active
CREATE_PUBVAR(GmUF1TrpAct ,  unsigned);     // Under-frequency 1 trip condition is active
CREATE_PUBVAR(GmUF2TrpAct ,  unsigned);     // Under-frequency 2 trip condition is active
CREATE_PUBVAR(GmUF3TrpAct ,  unsigned);     // Under-frequency 3 trip condition is active
CREATE_PUBVAR(GmOF1TrpAct ,  unsigned);     // Over-frequency 1 trip condition is active
CREATE_PUBVAR(GmOF2TrpAct ,  unsigned);     // Over-frequency 2 trip condition is active

//////////////////////////
// Converter Protection
//////////////////////////
CREATE_PUBVAR(OVProtectTrb   , unsigned);   // Converter-Turbine over-voltage trip
CREATE_PUBVAR(UVProtectTrb   , unsigned);   // Converter-Turbine under-voltage trip
CREATE_PUBVAR(VAProtectTrb   , unsigned);   // Converter-Turbine voltage asymmetry trip
CREATE_PUBVAR(OFProtectTrb   , unsigned);   // Converter-Turbine over-frequency trip
CREATE_PUBVAR(UFProtectTrb   , unsigned);   // Converter-Turbine under-frequecy trip

// Grid monitor- System current decrease by PLL error
CREATE_PUBVAR(GmDscmVxPPu,      float);     // Positive sequence discrimator x-component in per unit
CREATE_PUBVAR(GmDscmVMgPPu,     float);     // Positive sequence discrimator magnitude in per unit
CREATE_PUBVAR(GmDsVMgPPuIn,     float);     // Positive sequence discrimator magnitude in per unit
CREATE_PUBVAR(GmDsVMagRcp,      float);     // Reciprocal of GmDsVMgPPuIn
CREATE_PUBVAR(GmVxVmgRtio,      float);     // Result of GmDscmVxPT2/GmDscmVMagP
CREATE_PUBVAR(ISysCmdSFUc,      float);     // Input to the uprate limited LPF
CREATE_PUBVAR(ISysCmdSF,        float);     // Command to reduce system current
CREATE_PUBVAR(GmPllPrpHtSF,     float);     // Scale factor for hot dynamic gains when series comp is enabled

// low voltage event edge detection
CREATE_PUBVAR(GmVLoNotTmr,      float);     // Gm voltage not low timer
CREATE_PUBVAR(GmVLoPlsTmr,      float);     // Gm low voltage pulse timer
CREATE_PUBVAR(GmVLoPlsRdy,      unsigned);  // Gm low voltage pulse ready
CREATE_PUBVAR(GmVLoPulse,       unsigned);  // Gm low voltage pulse

CREATE_PUBVAR(GmPllHotDlGn,     float);
CREATE_PUBVAR(GmPllHiGnEnb,     unsigned);
CREATE_PUBVAR(GmPllShftFil,     float);
CREATE_PUBVAR(GmPllShiftWdt,    float);
CREATE_PUBVAR(GmPllShftGnx,     float);
CREATE_PUBVAR(GmPllHiGnTmr,     float);

// Grid Monitor Fast Vmag vars
CREATE_PUBVAR(GmVMagPkPuT2,     float);             // Fast grid monitor peak detected voltage magnitude per unit T2
CREATE_PUBVAR(GmVMagOVFil,      float);             // Fast grid monitor voltage squared and filtered

CREATE_PUBVAR(DisconWhlOv,      unsigned);
CREATE_PUBVAR(GmCnvOv1DiscFlg,  unsigned);
CREATE_PUBVAR(GmCnvOv2DiscFlg,  unsigned);
CREATE_PUBVAR(GmCnvOv3DiscFlg,  unsigned);
CREATE_PUBVAR(OvCnvFltState,    unsigned);
CREATE_PUBVAR(GmCnvOV1Gn,       float);
CREATE_PUBVAR(GmCnvOV2Gn,       float);
CREATE_PUBVAR(GmCnvOV3Gn,       float);
CREATE_PUBVAR(GmCnvOV4Gn,       float);

// Unpublished Variables
//----------------------
float   GmTocAlmOnDl;
float   GmTocAlmOffDl;
double  GmCnvUV1Filter;
double  GmCnvUV1Gn;
double  GmCnvOV1Filter;
double  GmCnvOV1GnD;


// use only 1/2 cycyle
float   GmRAFVanAlAr[GM_RAF_ARRAY_SIZE]; // RAF array for Van alpha average calc... max shold be .5*(1/(50hz)*(5712/6) + 2, add 1 for cushion?
float   GmRAFVanBeAr[GM_RAF_ARRAY_SIZE]; // RAF array vor Van beta average calc.
float   GmRAFVbnAlAr[GM_RAF_ARRAY_SIZE]; // RAF array for Vbn alpha average calc
float   GmRAFVbnBeAr[GM_RAF_ARRAY_SIZE]; // RAF array for Vbn beta average calc.
float   GmRAFVcnAlAr[GM_RAF_ARRAY_SIZE]; // RAF array for Vcn alpha average calc
float   GmRAFVcnBeAr[GM_RAF_ARRAY_SIZE]; // RAF array for Vcn beta average calc.
float   GmRAFIaAlAr[GM_RAF_ARRAY_SIZE];  // RAF array for Ia alpha average calc
float   GmRAFIaBeAr[GM_RAF_ARRAY_SIZE];  // RAF array for Ia beta average calc.
float   GmRAFIbAlAr[GM_RAF_ARRAY_SIZE];  // RAF array for Ib alpha average calc
float   GmRAFIbBeAr[GM_RAF_ARRAY_SIZE];  // RAF array for Ib beta average calc.
float   GmRAFIcAlAr[GM_RAF_ARRAY_SIZE];  // RAF array for Ic alpha average calc
float   GmRAFIcBeAr[GM_RAF_ARRAY_SIZE];  // RAF array for Ic beta average calc.


// Data Passing
//--------------
DATA_PASS(GmPllW            ,GmPllWT2     , T1_T2, FILTERED);
DATA_PASS(GmPllW            ,GmPllWT3     , T2_T3, FILTERED);
DATA_PASS(GmPllWInt         ,GmPllWIntT2  , T1_T2, FILTERED);
DATA_PASS(GmVMagT1          ,GmVMagT2     , T1_T2, FILTERED);
DATA_PASS(GmVMagT2          ,GmVMagT3     , T2_T3, FILTERED);
DATA_PASS(GmVMagPuT2        ,GmVMagPuT3   , T2_T3, FILTERED);
DATA_PASS(GmVxFbk           ,GmVxFbkT2    , T1_T2, FILTERED);
DATA_PASS(GmVyFbk           ,GmVyFbkT2    , T1_T2, FILTERED);
DATA_PASS(GmVxFbkT2         ,GmVxFbkT3    , T2_T3, FILTERED);
DATA_PASS(GmVyFbkT2         ,GmVyFbkT3    , T2_T3, FILTERED);
DATA_PASS(GmIxFbk           ,GmIxFbkT2    , T1_T2, FILTERED);
DATA_PASS(GmIyFbk           ,GmIyFbkT2    , T1_T2, FILTERED);
DATA_PASS(GmIxFbkT2         ,GmIxFbkT3    , T2_T3, FILTERED);
DATA_PASS(GmIyFbkT2         ,GmIyFbkT3    , T2_T3, FILTERED);


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   This function tracks the line ac voltage angle, both in real and
//   simulator mode.  The most significant outputs are electrical command and
//   feedback angles, as well and line frequency.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorPhaseLockLoopReg()
{
    float DelAngTrack;

    // Clear limit flags
    L_InnrRegLim.L_PllLimMax = false;
    L_InnrRegLim.L_PllLimMin = false;
    GmPllWZ1 = GmPllW;
    if ( PARM(GmPllVyFbkSw) == POS_SEQ_DSCM)
    {
        GmPllVyFbk = GmDscmVyPos;
    }
    else
    {
        GmPllVyFbk = GmVyFbk;
    }
    GmPllVyErr  = (0.0 + GmPllregStep) - GmPllVyFbk;


    // Calculate and clamp integral path
    if ((GmPllStateT1 == COOL_DYN) && (GmPllStateZ1 == HOT_DYN))
    {
        GmPllWInt = GmPllWRAFT1;
    }
    else
    {
        GmPllWInt = GmPllWInt + (GmPllIntGnT1)*L_DelTm1*GmPllVyErr;

    }

    if ( GmPllWInt >= ( GmPllWMaxIntT1))
    {
        GmPllWInt = (GmPllWMaxIntT1);
    }
    if (GmPllWInt <= (GmPllWMinIntT1))
    {
        GmPllWInt = (GmPllWMinIntT1);
    }


    // Calculate proportional path, sum with int. path, clamp
    GmPllW = GmPllWInt + GmPllVyErr*(GmPllPrpGnT1);
    if ( GmPllW >= (GmPllWMaxT1))
    {
        GmPllW = ( GmPllWMaxT1);
        if ( L_InnrRegEnbT1 )
        {
            L_InnrRegLim.L_PllLimMax = true;
        }
    }
    if ( GmPllW <= (GmPllWMinT1))
    {
        GmPllW = (GmPllWMinT1);
        if ( L_InnrRegEnbT1 )
        {
            L_InnrRegLim.L_PllLimMin = true;
        }
    }


    // Calculate electrical angles

    // if GmPllState indicates HOT_FIXFRQ mode, force GmAngElecFbk to GmAngFbTrack for one pass
    if ( (GmPllStateZ1T1 == HOT_FIXFRQ) && !PARM(GmAngTrckInh) )
    {

        GmPllW            = GmPllW + PARM(GmPllWShift);
        GmPllDelAng       = L_DelTm1*GmPllW ;
        if (GmPllStateZ2 != HOT_FIXFRQ )
        {
            GmAngElecFbk = GmAngFbTrack + GmPllDelAng;//take the snapshot
            GmPllVyComp  = GmAngFbTrack;

        }
        else
        {
            GmAngElecFbk = GmAngElecFbk + GmPllDelAng;
        }

    }
    else //normal mode
    {
        GmPllW       = GmPllW + PwrPllWFFT1;
        GmPllDelAng  = L_DelTm1*GmPllW;
        GmAngElecFbk = GmAngElecFbk + GmPllDelAng;
    }


    GmAngElecCmd = GmAngElecFbk + GmPllDelAng;


    // Develop slow tracking angle
    DelAngTrack  = L_DelTm1*(GmPllWTrackT1);
    GmAngFbTrack = GmAngFbTrack + DelAngTrack;

    GmAngFbkInit = false;


    if ((GmPllStateZ1T1 == COOL_DYN))
    {
        GmPllTrckTmr = GmPllTrckTmr + L_DelTm1;
        if ( GmPllTrckTmr >= PARM(GmPllTrackTm)  )
        {
            GmPllTrckTmr = 0.0;
            GmAngFbkInit = true;//Diag only
            GmAngFbTrack = GmAngElecFbk;
        }
    }
    else
    {
        // Preset timer while in HOT_FIXFRQ state
        GmPllTrckTmr = 0.5F*PARM(GmPllTrackTm);//do not allow track angle update unless in cool gain mode
    }


    // only modulo 2pi on T2 boundaries so T2 value ok
    // Modulo the angles derived from fbk only when fbk is moduloed and at the boundaries of T2
    if ((T1St == 1) && (ABS(GmAngElecFbk) > PI2) )
    {
        GmAngElecFbk  = cVectorf::Modulo2Pi(GmAngElecFbk);
        GmAngElecCmd  = cVectorf::Modulo2Pi(GmAngElecCmd);
        GmAngFbTrack  = cVectorf::Modulo2Pi(GmAngFbTrack);
    }

    L_AngElecCmd = cVectorf::Modulo2Pi(GmAngElecCmd + GmPllAngShft);
    L_AngElecFbk = cVectorf::Modulo2Pi(GmAngElecFbk + GmPllAngShft);


    GmPllStateZ2 = GmPllStateZ1T1;

    GmPllWAvg = 0.5*(GmPllW + GmPllWZ1);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   This function tracks monitors the ac Gm by observing the phase lock
//   loop error.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorLineMonitorT1()
{
    float   GmVy_Err_Gn,Pre_Ph_Imb_Ref; // GmPhImb_Pre_Fil;

    // Calculate and sum GmVx and GmVy errors

    GmVxLineErr  = GmVMagT1 + GmVxFbk;

    // Add low pass filters to each component
    GmVxErrFil  = GmVxErrFil + PARM(GmVxVyErrW)*L_DelTm1 * (GmVxLineErr - GmVxErrFil);
    GmVyErrFil  = GmVyErrFil + PARM(GmVxVyErrW)*L_DelTm1 * (GmPllVyErr - GmVyErrFil);
    //

    GmVxVyImb    = GmVxErrFil * GmVxErrFil + GmVyErrFil * GmVyErrFil;
    GmVxyImbFil  = GmVxyImbFil + PARM(GmPllErrW)*L_DelTm1 * (GmVxVyImb - GmVxyImbFil);


    // Calculate phase imbalance reference
    if ( (GmVMagT2 - GmAcLinePeak) >= 0.0)
    {
        GmAcLineUp  = true;
        GmVy_Err_Gn = PARM(GmAcLineWUp);
    }
    else
    {
        GmAcLineUp  = false;
        GmVy_Err_Gn = PARM(GmAcLineWDn);
    }

    GmAcLinePeak   = GmAcLinePeak + L_DelTm1*GmVy_Err_Gn*(GmVMagT2 - GmAcLinePeak);


    if ( L_InnrRegEnb && GmPllProven)
    {
        GmPllErSetpt = PARM(GmPllRunThrs);
    }
    else if (L_InnrRegEnb && GmLVRTRcyIp && (GmPllPrvTmr >= PARM(GmPllGnSwTm)))
    {
        GmPllErSetpt = PARM(GmPllRcvThrs);
    }
    else
    {
        GmPllErSetpt = PARM(GmPllIniThrs);
    }



    Pre_Ph_Imb_Ref = GmAcLinePeak*L_VlnInRcp*GmPllErSetpt + PARM(GmPllErNoise);

    GmPllErrRef     = Pre_Ph_Imb_Ref*Pre_Ph_Imb_Ref;


    // Calculate phase lock loop error
    GmPllErr        = GmVxyImbFil - GmPllErrRef;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//     This function uses grid monitor feedbacks to implement the same
//     functionality as line converter  discrimianator. It was developed
//     pimarily so that the positive y-axis componentcould be used as the
//     y-component pll feedback, providing a signal that is free of the
//     negative sequence component. This is especially helpful for the pll
//     for the caseof line to line shorts and
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorPosNegDiscrimT1()
{

    //Positive Sequence Calculations
    GmDscmVxPxy  = ( GmDscmVxNFil*GmCos2Ang) + (GmDscmVyNFil*GmSin2Ang);
    GmDscmVyPxy  = (-GmDscmVxNFil*GmSin2Ang) + (GmDscmVyNFil*GmCos2Ang);

    GmDscmVxPos  = GmVxFbk - GmDscmVxPxy;
    GmDscmVyPos  = GmVyFbk - GmDscmVyPxy;

    //Negative Sequence Calculations
    GmDscmVxNab  = GmVxFbk - GmDscmVxPFil;
    GmDscmVyNab  = GmVyFbk - GmDscmVyPFil;

    GmDscmVxNeg  = ( GmDscmVxNab*GmCos2Ang) - (GmDscmVyNab*GmSin2Ang);
    GmDscmVyNeg  = ( GmDscmVxNab*GmSin2Ang) + (GmDscmVyNab*GmCos2Ang);

    //Filters
    if ( Init1Sec)
    {
        GmDscmVxPFil = GmDscmVxPFil + GmDscmVxPGn*(GmDscmVxPos - GmDscmVxPFil);
        GmDscmVyPFil = GmDscmVyPFil + GmDscmVyPGn*(GmDscmVyPos - GmDscmVyPFil);

        GmDscmVxNFil = GmDscmVxNFil + GmDscmVxNGn*(GmDscmVxNeg - GmDscmVxNFil);
        GmDscmVyNFil = GmDscmVyNFil + GmDscmVyNGn*(GmDscmVyNeg - GmDscmVyNFil);
    }
    else
    {
        GmDscmVxPFil = 0.0;
        GmDscmVyPFil = 0.0;

        GmDscmVxNFil = 0.0;
        GmDscmVyNFil = 0.0;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This function uses the discriminated positive and negative sequence
//    components of the AC Line that are calculated by the task1 counter-part
//    of this function. Both a positive and negative sequence component of
//    the Line voltage magnitude are developed here.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorPosNegDiscriminatorT2()
{

    GmDscmVxNFl2 = GmDscmVxNFl2 + GmDscmVxNGn2*(GmDscmVxNT2 - GmDscmVxNFl2);
    GmDscmVyNFl2 = GmDscmVyNFl2 + GmDscmVyNGn2*(GmDscmVyNT2 - GmDscmVyNFl2);

    //Calculate the pos,neg seq magnitudes
    GmDscmVxPPu = GmDscmVxPT2 * L_VlnInRcp;
    GmDscmVMagP = sqrtf( (GmDscmVxPT2*GmDscmVxPT2) + (GmDscmVyPT2*GmDscmVyPT2) );
    GmDscmVMagN = sqrtf( (GmDscmVxNFl2*GmDscmVxNFl2) + (GmDscmVyNFl2*GmDscmVyNFl2) );//use filtered values here
    GmDscmVMgPPu = GmDscmVMagP * L_VlnInRcp;

    if ( GmDscmVMagN >= PARM(GmNSeqHiThr) )
    {
        GmNSeqHi     = true;
    }
    else
    {
        GmNSeqHi     = false;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This code presets the converter voltage proteciton low pass filters
//    HL: move this from bottom to here to make it usable by
//        GridMonitorLineMonitorT2
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonConverterFilterPreset(void)
{
    GmCnvOV1Filter = (double) PARM(GmOVMin1);
    GmCnvOV2Fil    = PARM(GmOVMin2);
    GmCnvOV3Fil    = PARM(GmOVMin3);
    GmCnvOV4Fil    = PARM(GmOVMin4);
    GmCnvUV2Fil    = PARM(GmUVMax2);
    GmCnvUV1Filter = (double) PARM(GmUVMax1);

    GmCnvFrqFil    = 1.0F;
    GmCnvOFFil     = PARM(GmOFMin);
    GmCnvUFFil     = PARM(GmUFMax);
    GmCVAsmFil1    = PARM(GmCVAsmMin1);
    GmCVAsmInt     = GmCVAsmMinInt;
    GmCVAsmFrc     = GmCVAsmMinFrc;
    GmCVAsmFil2    = PARM(GmCVAsmMin2);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This function tracks monitors the ac line by observing the phase lock
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorLineMonitorT2()
{
    //float   Temp1, W_Pll_Temp;
    //unsigned    Pll_Prv_Cond;

    ///////////////////////////////////////////////////////////////////////////
    // System Current Decrease by Pll Error Logic
    ///////////////////////////////////////////////////////////////////////////
    GmDsVMgPPuIn = MAX(GmDscmVMgPPu, PARM(GmDsVMgPuMin));
    GmDsVMagRcp  = NoNANLim( ( 1.0F / GmDsVMgPPuIn), 1.0e6F, -1.0e6F );
    GmVxVmgRtio  = -GmDscmVxPPu * GmDsVMagRcp;

    if (GmLineVShort || (GmDscmVMgPPu <= PARM(ISysCmdVMgMn)))
    {
        ISysCmdSFUc = 1.0F;
    }
    else
    {
        ISysCmdSFUc = GmVxVmgRtio * PARM(ISysCmdSFScl);
    }

    float ISysCmdDel;
    ISysCmdDel = (ISysCmdSFUc - ISysCmdSF) * DelTm2Rcp;
    if ( ISysCmdDel > PARM(ISysCmdUpRt))
    {
        ISysCmdDel = PARM(ISysCmdUpRt);
    }

    ISysCmdSF = ISysCmdSF + (ISysCmdDel * DelTm2);

    if (ISysCmdSF <= PARM(ISysCmdMin))
    {
        ISysCmdSF = PARM(ISysCmdMin);
    }
    else if (ISysCmdSF >= 1.0F)
    {
        ISysCmdSF = 1.0F;
    }

    ///////////////////////////////////////////////////////////////////////////
    // End System Current Decrease by Pll Error Logic
    ///////////////////////////////////////////////////////////////////////////

    GmPllVyErrPu = GmPllVyErrT2 * L_VlnInRcp;

    if ( (L_StopStTmr < PARM(GmPllIniTm)) && (L_SeqSt == LINE_STOPPED))
    {
        GmPllFrcIni = true;
    }
    else
    {
        GmPllFrcIni = false;
    }


    // Pll error limit check
    //----------------------
    GmPllErrInt = GmPllErrInt + DelTm2*GmPllErrT2;
    if ( GmPllErrInt < 0.0)
    {
        GmPllErrHi   = false;
        GmPllErrInt  = 0;
        GmPllErInLim = false;
    }
    else
    {
        GmPllErrHi   = true;
        if (GmPllErrInt >= PARM(GmPllErSpill))
        {
            GmPllErrInt  = PARM(GmPllErSpill);
            GmPllErInLim = true;
        }
    }


    // Symetric High Voltage Detection -- PROT_008
    //--------------------------------------------
    if ( GmVMagPuT2 >= PARM(GmLineVHiOn) )
    {
        GmLineVHiTmr = GmLineVHiTmr + DelTm2;
        if ( GmLineVHiTmr >= PARM(GmLineVHiTm) )
        {
            GmLineVHiTmr = PARM(GmLineVHiTm);
            GmLineVHi    = true;
        }
    }
    else if ( GmVMagPuT2 <= PARM(GmLineVHiOff) )
    {
        GmLineVHi     = false;
        GmLineVHiTmr  = 0.0;
    }


    // Symetric Low Voltage Detection -- PROT_008
    //-------------------------------------------
    if ( GmVMagPuT2 <= PARM(GmLineVLowOn) )
    {
        if ( GmVLoPlsRdy)//This is the first time in P.GmVLoNotTm this has happened
        {
            GmVLoPulse = true;
        }

        GmLineVLoTmr = GmLineVLoTmr + DelTm2;
        GmVLoPlsRdy  = false;
        GmVLoNotTmr  = 0.0F;

        if ( GmLineVLoTmr >= PARM(GmLineVLowTm) )
        {
            GmLineVLoTmr  = PARM(GmLineVLowTm);
            GmLineVLo     = true;
        }
    }
    else if ( GmVMagPuT2 >= PARM(GmLineVLowOf) )
    {
        GmLineVLo     = false;
        GmLineVLoTmr  = 0.0F;
        GmVLoNotTmr   = GmVLoNotTmr + DelTm2;
        if ( GmVLoNotTmr > PARM(GmVLoNotTm) )
        {
            GmVLoNotTmr = PARM(GmVLoNotTm);
            GmVLoPlsRdy = true;
        }
    }

    if ( GmVLoPulse)
    {
        GmVLoPlsTmr = GmVLoPlsTmr + DelTm2;
        if ( GmVLoPlsTmr > PARM(GmVLoPulseTm))
        {
            GmVLoPulse  = false;
            GmVLoPlsTmr = 0.0F;
        }
    }

    // Ac Line short calculation -- PROT_008
    //--------------------------------------
    if ( GmVMagGridPu <= PARM(GmLineVShrOn) )
    {
        GmLineVShTmr = GmLineVShTmr + DelTm2;
        if ( GmLineVShTmr >= PARM(GmLineVShrTm) )
        {
            GmLineVShTmr = PARM(GmLineVShrTm);
            GmLnVShOfTmr = 0.0F;
            GmLnVShOfTr2 = 0.0F;
            GmLineVShort = true;
        }
    }
    else if ( GmVMagGridPu >= PARM(GmLineVShrOf) ) //30%
    {
        GmLnVShOfTmr = GmLnVShOfTmr + DelTm2;
        if ( GmLnVShOfTmr > PARM(GmLnVShrOfTm) ) //3msec
        {
            GmLineVShort   = false;
            GmLnVShOfTmr   = PARM(GmLnVShrOfTm);
            GmLineVShTmr   = 0.0F;
        }
    }
    else if ( GmVMagGridPu >= PARM(GmLineVShrO2) ) //15%
    {
        GmLnVShOfTr2 = GmLnVShOfTr2 + DelTm2;
        if ( GmLnVShOfTr2 > PARM(GmLnVShrOfT2) )
        {
            GmLineVShort   = false;
            GmLnVShOfTr2   = PARM(GmLnVShrOfT2); //20msec
            GmLineVShTmr   = 0.0F;
        }
    }

    // GmPllProven logic -- PROT_008
    //------------------------------
    if ( !GmLineVShort && !GmLineVHi && (!GmLineVLo || L_ZVLineOff) && !GmNSeqHi && !HVACHotPll)
    {
        GmPllPrvPrm0 = true;
    }
    else
    {
        GmPllPrvPrm0 = false;
    }

    if ( !GmPllPrvPrm0 || GmPllErrHi)
    {
        GmPllPrvPrm = false;
    }
    else if ( GmPllPrvPrm0 && !GmPllErInLim)
    {
        GmPllPrvPrm = true;
    }

    if ( GmPllPrvPrm && !GmPllProven)
    {
        GmPllPrvTmr = GmPllPrvTmr + DelTm2;
        if ( GmPllPrvTmr >= PARM(GmPllPrvTm))
        {

            GmPllPrvTmr = PARM(GmPllPrvTm);
            if ( !GmPllPrvFstp)
            {
                GridMonConverterFilterPreset();
                GmPllPrvFstp = true;


                if (GmPllW < 0.0)
                {
                    GmRevSeq    = true;
                    GmPllProven = false;
                    GmPllPrvTmr = 0.0;

                    PUSH_DIAG(GmRevPhSeq);

                }
                else
                {
                    GmRevSeq    = false;
                    GmPllProven = true;
                }
            }
            else
            {
                GmPllProven  = true;
            }
        }
    }
    else if (!GmPllPrvPrm)
    {
        GmPllPrvTmr = 0.0;
        GmPllProven = false;
    }


    // Ac line transient / watchdog checks -- PROT_008
    //------------------------------------------------
    if (L_InnrRegEnb && GmPllErInLim && GmLineFltEnb)
    {
        PUSH_DIAG(L_AcLineTran);
        GmAcWdogTmr = GmAcWdogTmr + DelTm2;
        if ( GmAcWdogTmr >= PARM(GmAcWdogTm) )
        {
            GmAcWdogTmr = PARM(GmAcWdogTm);
            PUSH_DIAG(L_AcLineWdog);
        }
    }
    else
    {
        // Pop2Flt(L_AcLineTran);
        GmAcWdogTmr = 0.0;
    }

    //get average value of pll integrator for preset  */
    GmPllStateZ1 = GmPllState;


    // Pll Gain/Limit State Machine
    //-----------------------------

    switch (GmPllState)
    {
    case HOT_INIT:// GmPllState = 0
        GmPllIntGn  =  PARM(GmPllIntGnI1);
        GmPllPrpGn  =  PARM(GmPllPrpGnI);
        GmPllWMax   =  PARM(GmPllWPrec);
        GmPllWMin   =  PARM(GmPllWPrec);
        GmPllWMaxInt=  PARM(GmPllWPrec);
        GmPllWMinInt=  PARM(GmPllWPrec);
        GmPllProven =  false;
        GmPllPrvFstp=  false;

        GmFixFrqTmr =  0.0F;
        GmHotDynTmr =  0.0F;

        if ( !GmPllFrcIni)
        {
            GmPllState = HOT_DYN;
        }

        GmPllWTrack = PARM(L_LineFrq);//preset the filter output

        GmLVRTRcyIp = false;
        GmPllHot    = true;

        break;



    case HOT_DYN:    // GmPllState = 1
        if ( GmPllPrvFstp && !GmRevSeq)
        {
            if (IslandDet)
            {
                GmPllPrpGn   =  PARM(GmPllPrpGnIs);
                GmPllIntGn   =  PARM(GmPllIntGnIs);
                GmPllWMax    =  PARM(GmPllWMaxIs);
                GmPllWMaxInt =  PARM(GmPllWInMxIs);
                GmPllWMin    =  PARM(GmPllWMinIs);
                GmPllWMinInt =  PARM(GmPllWInMnIs);
            }
             else if ( HVACHotPll || GmPllHiGnEnb)
             {
                GmPllPrpGn   =  PARM(GmPllPrpGnI2);
                GmPllIntGn   =  PARM(GmPllIntGnI2);

                GmPllWMax    =  PARM(GmPllWMaxI);
                GmPllWMaxInt =  PARM(GmPllWIntMx);
                GmPllWMin    =  PARM(GmPllWMinI);
                GmPllWMinInt =  PARM(GmPllWIntMn);
             }
             else
             {
                if (!PARM(GmPllHotGnDy))
                {
                   GmPllPrpHtSF = MAX(   1.0F, (PARM(GmPrpGnISySF) * (1.0F - ISysCmdSF) ) );
                }
                else  //normally for Series Comp only
                {
                   GmPllPrpHtSF = MAX(  ( (ABS(GmPllVyErrPu) * (PARM(GmPrpGnVMgOf) + (PARM(GmPrpGnVMgSF) * GmDsVMagRcp)))), (PARM(GmPrpGnISySF) * (1.0F - ISysCmdSF) ) );
                }

                GmPllPrpGn   = PARM(GmPllPrpGnR) +  GmPllHotDlGn * GmPllPrpHtSF;
                GmPllIntGn   = PARM(GmPllIntGnI);

                GmPllWMax    =  PARM(GmPllWMaxI);
                GmPllWMaxInt =  PARM(GmPllWIntMx);
                GmPllWMin    =  PARM(GmPllWMinI);
                GmPllWMinInt =  PARM(GmPllWIntMn);
             }
          }

        else
        {
             GmPllPrpGn     =  PARM(GmPllPrpGnI1); //use very hot integral gain to capture negative sequence
             GmPllIntGn     =  PARM(GmPllIntGnI1); //use very hot integral gain to capture negative sequence

             GmPllWMax      =  PARM(GmPllWMaxI);
             GmPllWMaxInt   =  PARM(GmPllWIntMx);
            GmPllWMin   =  -PARM(GmPllWMaxI);
            GmPllWMinInt = -PARM(GmPllWIntMx);
        }


        GmFixFrqTmr =  0.0F;
        GmHotDynTmr = GmHotDynTmr + DelTm2;




        if ( GmPllFrcIni)
        {
            GmPllState = HOT_INIT;
        }
          else if ( GmLineVShort && !IslandDet)
        {
            GmPllState = HOT_FIXFRQ;
        }
        else if (GmPllPrvTmr > PARM(GmPllGnSwTm))
        {
            GmPllState = COOL_DYN;

        }                     

        GmPllHot    = true;

        break;



    case COOL_DYN:    // GmPllState = 2
        GmPllIntGn   =  PARM(GmPllIntGnR);
        GmPllPrpGn   =  PARM(GmPllPrpGnR);
        GmPllWMax    =  PARM(GmPllWMaxR);
        GmPllWMaxInt =  PARM(GmPllWIntMx);

        if ( !GmPllPrvFstp)//not sure of roation direction yet
        {
            GmPllWMin   =  -PARM(GmPllWMaxR);
            GmPllWMinInt = -PARM(GmPllWIntMx);
        }
        else if ( GmPllProven && !GmRevSeq)
        {
            GmPllWMin    =  PARM(GmPllWMinR);
            GmPllWMinInt =  PARM(GmPllWIntMn);
            GmLVRTRcyIp  =  false;
        }
        else //Proven before but re-proving now
        {
            GmPllWMin    = -PARM(GmPllWMaxR);
            GmPllWMinInt =  PARM(GmPllWIntMn);
        }

        GmFixFrqTmr =  0.0F;
        GmHotDynTmr =  0.0F;


        if ( GmPllFrcIni)
        {
            GmPllState = HOT_INIT;
        }
        else if ( !GmPllPrvPrm)
        {
             if (GmLineVShort && !(L_SeqSt == LINE_DIAGNOSTIC) && !IslandDet)
            {
                GmPllState = HOT_FIXFRQ;
                GmLVRTRcyIp= true;
            }
            else
            {
                GmPllState = HOT_DYN;
                if ( GmLineVShort || GmLineVLo || GmLineVHi)
                {
                    GmLVRTRcyIp = true;
                }
            }
        }

        //Only allow the filter to update here...prevents populating it with bad frequency
        GmPllWTrack = GmPllWTrack + (PARM(GmWPllWTrack)*DelTm2*(GmPllWT2 - GmPllWTrack));

        GmPllHot    = false;

        break;



    case HOT_FIXFRQ: // GmPllState = 3
        GmPllIntGn     =  PARM(GmPllIntGnI);
        GmPllPrpGn     =  PARM(GmPllPrpGnI);
        if (L_Running)
        {
            GmPllWMax      =  GmPllWTrack;
            GmPllWMin      =  GmPllWTrack;
            GmPllWMaxInt   =  GmPllWTrack;
            GmPllWMinInt   =  GmPllWTrack;
        }
        else
        {
            GmPllWMax      =  PARM(GmPllFrqPrec);
            GmPllWMin      =  PARM(GmPllFrqPrec);
            GmPllWMaxInt   =  PARM(GmPllFrqPrec);
            GmPllWMinInt   =  PARM(GmPllFrqPrec);
        }

        GmHotDynTmr =  0.0;
        GmFixFrqTmr = GmFixFrqTmr + DelTm2;
        if ( (GmFixFrqTmr > PARM(GmFixFrqMxTm)) && L_Running )
        {
            PUSH_DIAG(L_FixFrqTOut);
        }

        if ( GmPllFrcIni)
        {
            GmPllState = HOT_INIT;
        }
        else if ( !GmLineVShort)
        {
            GmPllState = HOT_DYN;
        }

        GmPllHot    = true;


        break;
    default:
        break;
    }


    // Pll frequency track filter
    //---------------------------

    if ( GmPllState == COOL_DYN)//freeze filter if pll not in cool gain state
    {
        GmPllWTrack = GmPllWTrack + (PARM(GmWPllWTrack)*DelTm2*(GmPllWT2 - GmPllWTrack));
    }


     //////////////////////////////////////////////////////////////////////
     // Remote fault angle shift logic
     //////////////////////////////////////////////////////////////////////

     if ( GmLineVLo)
     {
        GmPllShftGnx = PARM(GmPllShftGn);
     }
     else if (GmLineVHi )
     {
        GmPllShftGnx = PARM(GmPllShftGnH);
     }
     else
     {
        GmPllShftGnx = PARM(GmPllShftGnN);
     }


     GmPllShftV1  = PARM(GmPllShftDB) * MIN(GmVMagPuT2, PARM(GmPllSftVThr));
     GmPllShftFil = GmPllShftFil + (GmPllShiftWdt * ( GmPllVyErrPu - GmPllShftFil));


     if ( ABS(GmPllShftFil) > GmPllShftV1 )
     {
         GmPllShftV2 = GmPllShftFil;
     }
     else
     {
         GmPllShftV2 = 0.0F;
     }

     GmPllAngShft  = GmPllShftGnx * GmPllShftV2;

     //////////////////////////////////////////////////////////////////////
     // End Remote fault angle shift logic
     //////////////////////////////////////////////////////////////////////


     //////////////////////////////////////////////////////////////////////
     // Pll Hotter Hot gain logic
     //////////////////////////////////////////////////////////////////////

     if ( GmLineVLo || GmNSeqHi)
     {
        GmPllHiGnTmr = GmPllHiGnTmr + DelTm2;
        if ( (GmPllHiGnTmr > PARM(GmPllHiGnTm)) && !HVACStAct)
        {
           GmPllHiGnEnb = true;
        }
     }
     else
     {
        GmPllHiGnEnb = false;
        GmPllHiGnTmr = 0.0F;
     }

     //////////////////////////////////////////////////////////////////////
     // End Pll Hotter Hot gain logic
     //////////////////////////////////////////////////////////////////////

    // End Remote fault angle shift logic
    //-----------------------------------


    if ( L_DbCelTst.DbCtIp || L_DbCelTst.DbResMsIp)
    {
        GmRevPwrThr = PARM(GmRevPwrThrDbCel);  // final value parameter
    }
    else
    {
        GmRevPwrThr = PARM(GmRevPwrThr);       // user parameter
    }



    return;

}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//     Calculates Vx, Vy Gm and magnitude
//
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorInstrumentT2(void)
{
    int     i, GmRAFCntNew;
    float   CosSF, SinSF;

    float   VanAlNewest, VanBeNewest;
    float   VanAlOldest, VanBeOldest;

    float   VbnAlNewest, VbnBeNewest;
    float   VbnAlOldest, VbnBeOldest;

    float   VcnAlNewest, VcnBeNewest;
    float   VcnAlOldest, VcnBeOldest;

    float   IaAlNewest, IaBeNewest;
    float   IaAlOldest, IaBeOldest;

    float   IbAlNewest, IbBeNewest;
    float   IbAlOldest, IbBeOldest;

    float   IcAlNewest, IcBeNewest;
    float   IcAlOldest, IcBeOldest;



    ///////////////////////////////////////////////////////////////////////////////
    // RAF for current and voltage signals ----6.075usec each, but 15.0usc for 12
    // These filters are able to optain a dc offset over a fundamental Gm
    // cycle even though a even multiple of task1 times cannot exactly hit
    // the desired frequency.  This is accomoplished by having a gain for
    // each member that is 1/n, called GnInnr here, where n is the float representing
    // L_IntRate/L_LineFrq(hz).  A truncated version of that same number will
    // be referred to here as nTrunc. The total number of member in the array is
    // the truncated n, plus 2, referred to as N here. This RAF has two more
    // members more than the truncated number that would normall be in a full
    // ac Gm cycle.   The extra two members are attributed to the last member
    // sampled and the oldest member in the array.  A different gain is applied
    // to these two members. The gain applied to those two members is
    // (1.0 - nTrunc*(1/n))/2.0, called GnOuter here.  The sum of all the applied
    // gains is 1.0:   nTrunc*GnInnr + 2*GnOuter = 1.0, where NTrunc = N-2.
    //
    // The first attempt to implement this filter was to have each itteration of
    // the averaging loop to look for the 1st and last member of the array and
    // apply the GnInnr to each of those two elements.  That took 18.0usec.
    //
    // The following equivalent method takes only 6.0usec.  The inner gain is first
    // applied to all members, then a subtraction is made to correct for adding too
    // much to the newest and oldest member.  The error that must be subtracted
    // is I(oldest)*GnInnr + I(newest)*GnInnr - I(oldest)*GnOuter - I(newest)*GnOuter
    //  = I(oldest)*(GnInnr-GnOuter) + I(newest)*(GnInnr-GnOuter)
    //  = (GnInnr-GnOuter)*[I(oldest) + I(newer)]
    //  = L_IRAFGnDiff * I(oldest) + I(newer)], where L_IRAFGnDiff = (GnInnr-GnOouter)
    //
    //////////////////////////////////////////////////////////////////////////////////


    GmCosAngFbT2 = cosf(GmAngEFbkT2 - PARM(GmAngShiftT2));
    GmSinAngFbT2 = sinf(GmAngEFbkT2 - PARM(GmAngShiftT2));

    CosSF      = 2.0*GmCosAngFbT2;
    SinSF      = 2.0*GmSinAngFbT2; //compensate for 1/2 loss due to sin2x, cos2x



    GmVanAl1 = CosSF*GmVanT2;
    GmVanBe1 = SinSF*GmVanT2;
    GmVbnAl1 = CosSF*GmVbnT2;
    GmVbnBe1 = SinSF*GmVbnT2;
    GmVcnAl1 = CosSF*GmVcnT2;
    GmVcnBe1 = SinSF*GmVcnT2;


    GmIaAl1  = CosSF*GmIaT2;
    GmIaBe1  = SinSF*GmIaT2;
    GmIbAl1  = CosSF*GmIbT2;
    GmIbBe1  = SinSF*GmIbT2;
    GmIcAl1  = CosSF*GmIcT2;
    GmIcBe1  = SinSF*GmIcT2;


    GmRAFIndx++;


    // Dynamic re-sizing of RAF filters for operation away from nominal frequency
    GmPllW4RAF += PARM(GmWPllW4RAF)*DelTm2*(GmPllWIntT2 - GmPllW4RAF);

    if (PARM(GmRAFCntDynEnb))
    {
        if (GmRAFIndx >= GmRAFCnt)                         // check if GmRAFCntCnt needs to increase
        {   // or the stay the same
            GmRAFCntFlt = PI/GmPllW4RAF * DelTm2Rcp;
            GmRAFCntNew = ( ((int) (GmRAFCntFlt))  + 2);
            if (GmRAFCntNew >= GmRAFCnt)
            {
                GmRAFCnt    = GmRAFCntNew;
                if ( GmRAFCnt > (GM_RAF_ARRAY_SIZE))
                {
                    GmRAFCnt = (GM_RAF_ARRAY_SIZE);
                }
                GmRAFGnInnr = 1.0F/GmRAFCntFlt;
                GmRAFGnOutr = (1.0F - (  GmRAFGnInnr*(GmRAFCnt - 2)  ) ) * 0.5F;
                GmRAFGnDiff = GmRAFGnInnr - GmRAFGnOutr;
            }
        }
        else if (GmRAFIndx >= GmRAFCnt - 1)                // check if GmRAFCntCnt needs to decrease
        {
            GmRAFCntFlt = PI/GmPllW4RAF * DelTm2Rcp;
            GmRAFCntNew = ( ((int) (GmRAFCntFlt))  + 2);
            if (GmRAFCntNew < GmRAFCnt)
            {
                GmRAFCnt    = GmRAFCntNew;
                GmRAFGnInnr = 1.0F/GmRAFCntFlt;
                GmRAFGnOutr = (1.0F - (  GmRAFGnInnr*(GmRAFCnt - 2)  ) ) * 0.5F;
                GmRAFGnDiff = GmRAFGnInnr - GmRAFGnOutr;
            }
        }
    }


    if (GmRAFIndx >= GmRAFCnt) //Clamped value of GmRAFIndx is  GM_RAF_ARRAY_SIZE-1 ...see rule file
    {
        GmRAFIndx = 0;
    }

    RAFIndx1st = GmRAFIndx + 1;  //the oldest member in the array
    if (RAFIndx1st >= GmRAFCnt)  //if index is at last member in array, oldest memeber is element 0
    {
        RAFIndx1st = 0;
    }

    VanAlOldest  = GmRAFVanAlAr[RAFIndx1st];
    VanBeOldest  = GmRAFVanBeAr[RAFIndx1st];
    VbnAlOldest  = GmRAFVbnAlAr[RAFIndx1st];
    VbnBeOldest  = GmRAFVbnBeAr[RAFIndx1st];
    VcnAlOldest  = GmRAFVcnAlAr[RAFIndx1st];
    VcnBeOldest  = GmRAFVcnBeAr[RAFIndx1st];

    IaAlOldest   = GmRAFIaAlAr[RAFIndx1st];
    IaBeOldest   = GmRAFIaBeAr[RAFIndx1st];
    IbAlOldest   = GmRAFIbAlAr[RAFIndx1st];
    IbBeOldest   = GmRAFIbBeAr[RAFIndx1st];
    IcAlOldest   = GmRAFIcAlAr[RAFIndx1st];
    IcBeOldest   = GmRAFIcBeAr[RAFIndx1st];

    // Fill the arrays and zero the average
    VanAlNewest  = GmRAFVanAlAr[GmRAFIndx] = GmVanAl1;
    VanBeNewest  = GmRAFVanBeAr[GmRAFIndx] = GmVanBe1;
    VbnAlNewest  = GmRAFVbnAlAr[GmRAFIndx] = GmVbnAl1;
    VbnBeNewest  = GmRAFVbnBeAr[GmRAFIndx] = GmVbnBe1;
    VcnAlNewest  = GmRAFVcnAlAr[GmRAFIndx] = GmVcnAl1;
    VcnBeNewest  = GmRAFVcnBeAr[GmRAFIndx] = GmVcnBe1;

    IaAlNewest   = GmRAFIaAlAr[GmRAFIndx] = GmIaAl1;
    IaBeNewest   = GmRAFIaBeAr[GmRAFIndx] = GmIaBe1;
    IbAlNewest   = GmRAFIbAlAr[GmRAFIndx] = GmIbAl1;
    IbBeNewest   = GmRAFIbBeAr[GmRAFIndx] = GmIbBe1;
    IcAlNewest   = GmRAFIcAlAr[GmRAFIndx] = GmIcAl1;
    IcBeNewest   = GmRAFIcBeAr[GmRAFIndx] = GmIcBe1;


    GmVanAl2  = 0.0;
    GmVanBe2  = 0.0;
    GmVbnAl2  = 0.0;
    GmVbnBe2  = 0.0;
    GmVcnAl2  = 0.0;
    GmVcnBe2  = 0.0;

    GmIaAl2   = 0.0;
    GmIaBe2   = 0.0;
    GmIbAl2   = 0.0;
    GmIbBe2   = 0.0;
    GmIcAl2   = 0.0;
    GmIcBe2   = 0.0;


    // Add in the rest of the members
    for (i = 0; i < GmRAFCnt; i++)
    {
        GmVanAl2   += GmRAFGnInnr*GmRAFVanAlAr[i];
        GmVanBe2   += GmRAFGnInnr*GmRAFVanBeAr[i];
        GmVbnAl2   += GmRAFGnInnr*GmRAFVbnAlAr[i];
        GmVbnBe2   += GmRAFGnInnr*GmRAFVbnBeAr[i];
        GmVcnAl2   += GmRAFGnInnr*GmRAFVcnAlAr[i];
        GmVcnBe2   += GmRAFGnInnr*GmRAFVcnBeAr[i];

        GmIaAl2   += GmRAFGnInnr*GmRAFIaAlAr[i];
        GmIaBe2   += GmRAFGnInnr*GmRAFIaBeAr[i];
        GmIbAl2   += GmRAFGnInnr*GmRAFIbAlAr[i];
        GmIbBe2   += GmRAFGnInnr*GmRAFIbBeAr[i];
        GmIcAl2   += GmRAFGnInnr*GmRAFIcAlAr[i];
        GmIcBe2   += GmRAFGnInnr*GmRAFIcBeAr[i];
    }
    // total sum of gains is 1.0, so no need to divide.

    if ( Init1Sec)
    {

        // Adjust each componet for 1st and last member gains
        GmVanAl2  -= GmRAFGnDiff*(VanAlNewest  + VanAlOldest);
        GmVanBe2  -= GmRAFGnDiff*(VanBeNewest  + VanBeOldest);
        GmVbnAl2  -= GmRAFGnDiff*(VbnAlNewest  + VbnAlOldest);
        GmVbnBe2  -= GmRAFGnDiff*(VbnBeNewest  + VbnBeOldest);
        GmVcnAl2  -= GmRAFGnDiff*(VcnAlNewest  + VcnAlOldest);
        GmVcnBe2  -= GmRAFGnDiff*(VcnBeNewest  + VcnBeOldest);

        GmIaAl2   -= GmRAFGnDiff*(IaAlNewest  + IaAlOldest);
        GmIaBe2   -= GmRAFGnDiff*(IaBeNewest  + IaBeOldest);
        GmIbAl2   -= GmRAFGnDiff*(IbAlNewest  + IbAlOldest);
        GmIbBe2   -= GmRAFGnDiff*(IbBeNewest  + IbBeOldest);
        GmIcAl2   -= GmRAFGnDiff*(IcAlNewest  + IcAlOldest);
        GmIcBe2   -= GmRAFGnDiff*(IcBeNewest  + IcBeOldest);

    }
    else
    {
        GmVanAl2  = 0.0;
        GmVanBe2  = 0.0;
        GmVbnAl2  = 0.0;
        GmVbnBe2  = 0.0;
        GmVcnAl2  = 0.0;
        GmVcnBe2  = 0.0;

        GmIaAl2   = 0.0;
        GmIaBe2   = 0.0;
        GmIbAl2   = 0.0;
        GmIbBe2   = 0.0;
        GmIcAl2   = 0.0;
        GmIcBe2   = 0.0;

    }


    GmPwrT2     = 1.5 * ( ( GmVxFbkT2*GmIxFbkT2)  + (GmVyFbkT2*GmIyFbkT2) );
    GmQPwrT2    = 1.5 * ( ( GmVyFbkT2*GmIxFbkT2)  - (GmVxFbkT2*GmIyFbkT2) );
	if (GmVMagT2 < PARM(GmIyInstCalcVmin))
	{
        GmIyInst=0; //prevent divide by zero
    }
	else
	{
		GmIyInst    = GmQPwrT2*(0.6666666666666/GmVMagT2);
	}
    GmVMagPuT2  = GmVMagT2*L_VlnInRcp;

    //Q1OpnReq populated here from T3 so it is set before sequencer runs.
    if (GmRevPwrTrpFT2)
    {
        Q1OpnReq = true;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This function performs the demodulation of Master Gm voltages
//      into the x-y coordinate system.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorDemodulate()
{
    // sin cos of GmAngElecCmd is moved to RotateUpDn function.
    // This is a bug inherited from ESS code.
    cVectorf::SineCosine(GmAngElecFbk,&GmSinCosFbk);
    cVectorf::SineCosine(L_AngElecFbk,&L_SinCosFbk);

    GmCosAngFbk = GmSinCosFbk.cos;
    GmSinAngFbk = GmSinCosFbk.sin;
    GmCos2Ang   = 2.0F*(GmCosAngFbk*GmCosAngFbk) - 1.0F;
    GmSin2Ang   = 2.0F*GmSinAngFbk*GmCosAngFbk;

    cVectorf::DemodLN3(GmVan, GmVbn, GmVcn, &GmSinCosFbk, &GmVxFbk, &GmVyFbk);
    GmVMagT1    = sqrtf( (GmVxFbk*GmVxFbk) + (GmVyFbk*GmVyFbk) );
    GmVMagPuT1  = GmVMagT1*L_VlnInRcp;

    cVectorf::DemodLN3(GmIa,  GmIb,  GmIc,  &GmSinCosFbk, &GmIxFbk, &GmIyFbk);

    // Phase shift the current relative to the voltage to account for differences in signal processing
    GmIxFbk = GmIxyShftD * GmIxFbk + GmIxyShftQ * GmIyFbk;
    GmIyFbk = GmIxyShftD * GmIyFbk - GmIxyShftQ * GmIxFbk;

    /////////////////////////////////////////////////////////
    //
    //  Form "2" currrents for wye detection: +30deg shift
    //
    /////////////////////////////////////////////////////////

    //sin(alpha + B) = sin(alpha)*cos(B) + cos(alpha)*sin(B)
    //cos(alpha + B) = cos(alpha)*cos(B) - sin(alpha)*sin(B)
    //B=30deg
    //Cos(30deg) = sqrtf(3)/2
    //Sin(30deg) = 0.5
    //sin(alpah + 30deg) = (sqrtf(3)/2)*sin(alpha) + 0.5*cos(alpha)
    //cos(alpah + 30deg) = (sqrtf(3)/2)*cos(alpha) - 0.5*sin(alpha)


    SinEFbkP30   = (SQRT3/2.0F)*GmSinAngFbk + 0.5F*GmCosAngFbk;
    CosEFbkP30   = (SQRT3/2.0F)*GmCosAngFbk - 0.5F*GmSinAngFbk;


    //Rotate the currents
    IaGridDeltT1    = GmIxFbk*CosEFbkP30 - GmIyFbk*SinEFbkP30;
    IbGridDeltT1    = GmIxFbk*( -(1.0/2.0) *CosEFbkP30  + (SQRT3/2.0)*SinEFbkP30) +
                      GmIyFbk*((SQRT3/2.0) *CosEFbkP30  + (1.0/2.0)*SinEFbkP30);
    IcGridDeltT1    = GmIxFbk*(-(1.0/2.0)  *CosEFbkP30  - (SQRT3/2.0)*SinEFbkP30) +
                      GmIyFbk*(-(SQRT3/2.0)*CosEFbkP30  + (1.0/2.0)*SinEFbkP30);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This function uses the discriminated positive and negative sequence
//    components of the AC grid that are calculated by the task1 and task 2
//    counter-parts of this function. Several gains are calculated here, as
//    well as a per unit version of the negative sequence voltage component.
//    An angle representing the x-y relationship of the negative sequence
//    component is also calculated here.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorPosNegDiscrimT3()
{

    float VMgNPuTemp;


    //Calculate the per unit magnitude
    //--------------------------------
    VMgNPuTemp   =  GmDscmVMagN/GmAcLnPkT3 * PARM(GmNSeqPuScl);


    if ( VMgNPuTemp < PARM(GmNSeqPuOffs))
    {
        GmDscmVMgNPu = VMgNPuTemp;
    }
    else
    {
        GmDscmVMgNPu = VMgNPuTemp + PARM(GmNSeqPuGn)*(GmDscmVMgNPu - PARM(GmNSeqPuOffs));
    }


    if ( GmDscmVMgNPu > PARM(GmNSeqAngThr))
    {
        GmDscmAngN   = atan2(GmDscmVyNT3,   GmDscmVxNT3);
    }
    else
    {
        GmDscmAngN   = 0.0;
    }


    //Gain Calculations
    //-----------------
    GmDscmVxPGn  = PARM(GmDscmWVxP)*L_DelTm1;
    GmDscmVyPGn  = PARM(GmDscmWVyP)*L_DelTm1;

    GmDscmVxNGn  = PARM(GmDscmWVxN)*L_DelTm1;
    GmDscmVyNGn  = PARM(GmDscmWVyN)*L_DelTm1;

    GmDscmVxNGn2 = PARM(GmDscmWVxN2)*DelTm2;
    GmDscmVyNGn2 = PARM(GmDscmWVyN2)*DelTm2;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This function  monitors various Ac line conditions, as listed below:
//    - Calculation of Current magnitude
//    - Population of pll reg step variable
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorLineMonitorT3()
{

    GmIMagT3    = sqrtf( (GmIxFbkT3*GmIxFbkT3) + (GmIyFbkT3*GmIyFbkT3) );


    GmPllregStep = gVariable.ReadAsFloat(PARM(GmPllrStpAdr));

    GmWPllRcp = 1.0/GmPllWT3;


    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This function tracks monitors the ac line by observing the phase lock
//    loop error.
//
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorFundamentalCalcsT3()
{
    float VoltGain, CurrGain;
    float Sqrt2Rcp, Rcp3;
    float PSeqVTemp, NSeqTempPct;
    float GmPwrFctTemp;

    VoltGain = MIN(DelTm3*PARM(GmPhsVoltW), 1.0F);
    CurrGain = MIN(DelTm3*PARM(GmPhsCurrW), 1.0F);
    Sqrt2Rcp = SQRT2_RCP;
    Rcp3 = RCP_3;



    ////////////////////////////////////////////////////////////////////////////////////
    //   Grid Monidotr Signal Conditioning  -- INST_200
    ////////////////////////////////////////////////////////////////////////////////////

    //Low pass filters and scale for RMS
    GmV1nAlpha = GmV1nAlpha  + VoltGain * (Sqrt2Rcp*(GmVanAl2T3) - GmV1nAlpha);
    GmV1nBeta  = GmV1nBeta   + VoltGain * (Sqrt2Rcp*(GmVanBe2T3) - GmV1nBeta);
    GmV2nAlpha = GmV2nAlpha  + VoltGain * (Sqrt2Rcp*(GmVbnAl2T3) - GmV2nAlpha);
    GmV2nBeta  = GmV2nBeta   + VoltGain * (Sqrt2Rcp*(GmVbnBe2T3) - GmV2nBeta);
    GmV3nAlpha = GmV3nAlpha  + VoltGain * (Sqrt2Rcp*(GmVcnAl2T3) - GmV3nAlpha);
    GmV3nBeta  = GmV3nBeta   + VoltGain * (Sqrt2Rcp*(GmVcnBe2T3) - GmV3nBeta);

    GmI1Alpha  = GmI1Alpha   + CurrGain * (Sqrt2Rcp*(GmIaAl2T3)  - GmI1Alpha);
    GmI1Beta   = GmI1Beta    + CurrGain * (Sqrt2Rcp*(GmIaBe2T3)  - GmI1Beta);
    GmI2Alpha  = GmI2Alpha   + CurrGain * (Sqrt2Rcp*(GmIbAl2T3)  - GmI2Alpha);
    GmI2Beta   = GmI2Beta    + CurrGain * (Sqrt2Rcp*(GmIbBe2T3)  - GmI2Beta);
    GmI3Alpha  = GmI3Alpha   + CurrGain * (Sqrt2Rcp*(GmIcAl2T3)  - GmI3Alpha);
    GmI3Beta   = GmI3Beta    + CurrGain * (Sqrt2Rcp*(GmIcBe2T3)  - GmI3Beta);

    GmVxT3Fil  = GmVxT3Fil   + PARM(GmVxFilW)  * DelTm3 * (GmVxFbkT3 - GmVxT3Fil);
    GmVyT3Fil  = GmVyT3Fil   + PARM(GmVyFilW)  * DelTm3 * (GmVyFbkT3 - GmVyT3Fil);
    GmIxT3Fil  = GmIxT3Fil   + PARM(GmIxFilW)  * DelTm3 * (GmIxFbkT3 - GmIxT3Fil);
    GmIyT3Fil  = GmIyT3Fil   + PARM(GmIyFilW)  * DelTm3 * (GmIyFbkT3 - GmIyT3Fil);
    GmFreqT3Fil= GmFreqT3Fil + PARM(GmFreqFilW)* DelTm3 * (GmPllWT3  - GmFreqT3Fil);



    ////////////////////////////////////////////////////////////////////////////////////
    //   Grid Monitor Fundamental Signa Calculations (1 of 2)  -- INST_210
    ////////////////////////////////////////////////////////////////////////////////////

    //Line to nuetral rms magnitudes
    GmV1nMag   = sqrtf( (GmV1nAlpha*GmV1nAlpha) + (GmV1nBeta*GmV1nBeta) );
    GmV2nMag   = sqrtf( (GmV2nAlpha*GmV2nAlpha) + (GmV2nBeta*GmV2nBeta) );
    GmV3nMag   = sqrtf( (GmV3nAlpha*GmV3nAlpha) + (GmV3nBeta*GmV3nBeta) );

    //Line to line rms magnitudes
    GmV12Mag   = sqrtf( ( (GmV1nAlpha-GmV2nAlpha)*(GmV1nAlpha-GmV2nAlpha) ) + ( (GmV1nBeta-GmV2nBeta)*(GmV1nBeta-GmV2nBeta) ) );
    GmV23Mag   = sqrtf( ( (GmV2nAlpha-GmV3nAlpha)*(GmV2nAlpha-GmV3nAlpha) ) + ( (GmV2nBeta-GmV3nBeta)*(GmV2nBeta-GmV3nBeta) ) );
    GmV31Mag   = sqrtf( ( (GmV3nAlpha-GmV1nAlpha)*(GmV3nAlpha-GmV1nAlpha) ) + ( (GmV3nBeta-GmV1nBeta)*(GmV3nBeta-GmV1nBeta) ) );

    GmV1nMagPu = GmVlnNomRcp*GmV1nMag;
    GmV2nMagPu = GmVlnNomRcp*GmV2nMag;
    GmV3nMagPu = GmVlnNomRcp*GmV3nMag;

    GmV12MagPu = GmVllNomRcp*GmV12Mag;
    GmV23MagPu = GmVllNomRcp*GmV23Mag;
    GmV31MagPu = GmVllNomRcp*GmV31Mag;

    //Current magnitudes for each phase
    GmI1Mag    = sqrtf( (GmI1Alpha*GmI1Alpha) + (GmI1Beta*GmI1Beta) );
    GmI2Mag    = sqrtf( (GmI2Alpha*GmI2Alpha) + (GmI2Beta*GmI2Beta) );
    GmI3Mag    = sqrtf( (GmI3Alpha*GmI3Alpha) + (GmI3Beta*GmI3Beta) );

    //Take the average of each
    GmVlnMagAvg= 0.3333333333F * (GmV1nMag + GmV2nMag + GmV3nMag);
    GmVllMagAvg= 0.3333333333F * (GmV12Mag + GmV23Mag + GmV31Mag);
    GmIMagAvg  = 0.3333333333F * (GmI1Mag + GmI2Mag + GmI3Mag);





    ////////////////////////////////////////////////////////////////////////////////////
    //   Grid Monitotor Fundamental Signa Calculations(2 of 2)  -- INST_220
    ////////////////////////////////////////////////////////////////////////////////////

    GmPwr        = GmPwr     + PARM(GmPwrFilW)*DelTm3  * (GmPwrT3   - GmPwr);
    GmQPwr       = GmQPwr    + PARM(GmQPwrFilW)*DelTm3 * (GmQPwrT3  - GmQPwr);



    GmSPwr       = sqrtf( (GmPwr*GmPwr) + (GmQPwr*GmQPwr) );
    // prevent devide by zero
    if  (ABS(GmSPwr) < 0.001F)
        GmSPwr = 0.001F;

    GmPwrFctTemp = ABS(GmPwr)/GmSPwr;
    if ( ABS(GmQPwr) <  PARM(GmPwrFSignTh)) //if QPwr is small, follow sign of command
    {
        if ( QregCmd < -0.1F) //slightly negative (-.1VAR is very small)
        {
            GmPwrFctTemp = -GmPwrFctTemp;
        }
    }
    if ( GmQPwr < -PARM(GmPwrFSignTh)) //if QPwr is higher mag negative, also change sign
    {
        GmPwrFctTemp = -GmPwrFctTemp;
    }
    GmPwrFactor = GmPwrFctTemp;

    GmFreq       = GmFreqT3Fil * RAD_TO_HZ;
    GmFreqPu     = GmFreq*GmFrqNomRcp;

    //Ground current calculation

    if ( GmGndFltInh) 
    {
       GmIGndAlSum  = 0.0F;
       GmIGndBeSum  = 0.0F;
        
    }
    else
    {
       GmIGndAlSum  = Sqrt2Rcp * (GmIaAl2T3 + GmIbAl2T3 + GmIcAl2T3);
       GmIGndBeSum  = Sqrt2Rcp * (GmIaBe2T3 + GmIbBe2T3 + GmIcBe2T3);
    }
       

    GmIGndAlFil  = GmIGndAlFil + GmIGndFilWdt * ( GmIGndAlSum  - GmIGndAlFil);
    GmIGndBeFil  = GmIGndBeFil + GmIGndFilWdt * ( GmIGndBeSum  - GmIGndBeFil);

    GmIGndSqr   = (GmIGndAlFil * GmIGndAlFil) + (GmIGndBeFil * GmIGndBeFil);
    GmIGndMag   = sqrtf(GmIGndSqr);

    ////////////////////////////////////////////////////////////////////////////////////
    //   Grid Monitor Negative Sequence Magnitude Calculation (4 of 4)  -- INST_240
    ////////////////////////////////////////////////////////////////////////////////////

    GmNSeqVMag    = GmDscmVMgNT3;
    GmPSeqVMag    = GmDscmVMgPT3;
    GmPosSeqVPct  = 100.0*GmPSeqVMag*VlnPeakRcp;


    PSeqVTemp   = GmPSeqVMag;
    if ( PSeqVTemp < 10.0F)
    {
        PSeqVTemp = 10.0F;   //avoid divide by zero during ZVRT
    }

    NSeqTempPct  = PARM(GmNegSeqScl) * (GmNSeqVMag/PSeqVTemp);
    if ( NSeqTempPct > 100.0F)
    {
        NSeqTempPct = 100.0F;
    }
    GmNegSeqVPct = NSeqTempPct;

    if ( L_LTestIp)
     {
        GmUVAlmDynThr   = PARM(GmUVIndTstAlmThr);
        GmUV2TrpThrA    = PARM(GmUV2IndTstTrpThr);
        GmUV3TrpThrA    = PARM(GmUV3IndTstTrpThr);
        GmUVTrpDynThr1  = PARM(GmUVIndTstTrpThrs1);
        GmUVTrpDynThr2  = PARM(GmUVIndTstTrpThrs2);
     }
     else
     {
        GmUVAlmDynThr   = PARM(GmUVAlmThr);
        GmUV2TrpThrA    = PARM(GmUV2TrpThr);
        GmUV3TrpThrA    = PARM(GmUV3TrpThr);
        GmUVTrpDynThr1  = PARM(GmUVTrpThrs1);
        GmUVTrpDynThr2  = PARM(GmUVTrpThrs2);
     }



    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This function performs a timed over-current check on each of the phase
//    currents. The inputs are squared, then lightly filtered phase currents.
//    This function filters these signals much more by an adjustable amount.
//    The outputs of these filters is then compared to an adjustable
//    threshold.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorTocDetection(void)
{

    //float Sqrt2Rcp;
    float L_IMagSqr;
    float R_IMagSqr;
    float S_IMagSqr;
    float GmIMagSqr;



    GmIaRmsMag   = GmIaRmsMag + GmWTocT3Dyn*(GmIaSqrT3 - GmIaRmsMag);
    GmIbRmsMag   = GmIbRmsMag + GmWTocT3Dyn*(GmIbSqrT3 - GmIbRmsMag);
    GmIcRmsMag   = GmIcRmsMag + GmWTocT3Dyn*(GmIcSqrT3 - GmIcRmsMag);

    GmIaRms   = sqrtf(GmIaRmsMag);
    GmIbRms   = sqrtf(GmIbRmsMag);
    GmIcRms   = sqrtf(GmIcRmsMag);

    // if or not to use the dynamic TOC configuration
    if ( PARM(TocDyThrshEnb) )
    {
        if ( PARM(DelSpdTrqAct) )
        {
            GmTocThrDy    = GmMaxCurOut * PARM(GmTocMargin);
        }
        else
        {
            GmTocThrDy    = GmMaxCurOut;
        }
        GmTocAlmOnDy  = GmTocThrDy - GmTocAlmOnDl;
        GmTocAlmOffDy = GmTocAlmOnDy - GmTocAlmOffDl;
    }
    else
    {
        GmTocThrDy    = PARM(GmTocThrs);
        GmTocAlmOnDy  = PARM(GmTocAlmOn);
        GmTocAlmOffDy = PARM(GmTocAlmOff);
    }

    if ( GmIaRmsMag > GmIbRmsMag)
    {
        GmTocHiSelSq = GmIaRmsMag;
        GmTocHiSel   = GmIaRms;
    }
    else
    {
        GmTocHiSelSq = GmIbRmsMag;
        GmTocHiSel   = GmIbRms;
    }

    if ( GmIcRmsMag > GmTocHiSelSq)
    {
        GmTocHiSelSq = GmIcRmsMag;
        GmTocHiSel   = GmIcRms;
    }


    if ( GmTocHiSel >= GmTocThrDy )
    {
        PUSH_DIAG(GmTocFlt);
    }

    if ( GmTocHiSel >= GmTocAlmOnDy )
    {
        GmTocAlmFlg = true;
    }
    else if ( GmTocHiSel <= GmTocAlmOffDy )
    {
        GmTocAlmFlg = false;
    }

    if ( GmTocAlmFlg )
    {
        PUSH_DIAG(GmTocAlm);
    }

    if ( PARM(TocPreset) && !TocPresetZ1)
    {
        L_IMagSqr = 0.5*L_IMagFil*L_IMagFil;
        R_IMagSqr = 0.5*R_IMagFil*R_IMagFil;
        S_IMagSqr = 0.5*S_IMagFil*S_IMagFil;
        GmIMagSqr = 0.5*GmIMagT3 *GmIMagT3;

        L_IaRmsMag = L_IMagSqr;
        L_IbRmsMag = L_IMagSqr;
        L_IcRmsMag = L_IMagSqr;

        R_IaRmsMag = R_IMagSqr;
        R_IbRmsMag = R_IMagSqr;
        R_IcRmsMag = R_IMagSqr;

        S_IaRmsMag = S_IMagSqr;
        S_IbRmsMag = S_IMagSqr;
        S_IcRmsMag = S_IMagSqr;


        GmIaRmsMag = GmIMagSqr;
        GmIbRmsMag = GmIMagSqr;
        GmIcRmsMag = GmIMagSqr;

    }
    TocPresetZ1 = PARM(TocPreset);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This function is repsonsible for selection of voltages used for the grid
//    monitor under-votlage and over-voltage protections.  Either line-neutral
//    or line-line voltages are chosen.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorVoltageInputSteering(void)
{

    if ( PARM(GmVltPrtSrc) == LINE_TO_NEUT)
    {
        GmV1MagPu   = GmV1nMagPu;
        GmV2MagPu   = GmV2nMagPu;
        GmV3MagPu   = GmV3nMagPu;
        GmVltSource = LINE_TO_NEUT;
    }
    else // Line to Line
    {
        GmV1MagPu = GmV12MagPu;
        GmV2MagPu = GmV23MagPu;
        GmV3MagPu = GmV31MagPu;
        GmVltSource = LINE_TO_LINE;
    }

    GmVnMagPuMax = MAX(MAX(GmV1MagPu, GmV2MagPu),  GmV3MagPu);


    if ( PARM(GmVltPrtType) == ANY)
    {
        GmVltType = ANY;
    }
    else //ALL
    {
        GmVltType = ALL;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This is function repsonsible for implementing grid monitor under
//    voltgage protection for alarm only. There are eight separate alarms.
//    At most, only three of these can occur for a given configuration as
//    shown below.
//
//      Fault vs. Configuration:
//
//               All and Line to Neutral:
//                   Flt.GmUVAlmLN
//
//               All and Line to Line:
//                   Flt.GmUVAlmLL
//
//               Any and Line to Neutral:
//                   Flt.GmUVAlm1N
//                   Flt.GmUVAlm2N
//                   Flt.GmUVAlm3N
//
//               Any and Line to Line:
//                   Flt.GmUVAlm12
//                   Flt.GmUVAlm23
//                   Flt.GmUVAlm31
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorUnderVoltageAlarm(void)
{
    unsigned GmUVAlmFlgTp;


    GmUVAlmFlgTp = false;

    //Phase 1
    if ( GmV1MagPu < GmUVAlmDynThr)
    {
        GmUVAlmTmr1  = GmUVAlmTmr1 + DelTm3;
        GmUVAlmTmrO1 = 0.0;

        if ( GmUVAlmTmr1 >= PARM(GmUVAlmTm))
        {
            GmUVAlmTmr1 = PARM(GmUVAlmTm);
            GmUVAlmFlg1 = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    GmUVAlm1NF   = true;
                    GmUVAlmFlgTp = true;
                }
                else
                {
                    GmUVAlm12F   = true;
                    GmUVAlmFlgTp = true;
                }
            }
        }
    }
    else
    {
        GmUVAlmTmr1 = GmUVAlmTmr1 - DelTm3;
        if ( GmUVAlmTmr1 <= 0.0)
        {
            GmUVAlmTmr1 = 0.0;
        }
        if ( GmV1MagPu > PARM(GmUVAlmOff))
        {
            GmUVAlmTmrO1 = GmUVAlmTmrO1 + DelTm3;
            if ( GmUVAlmTmrO1 > PARM(GmUVAlmOffTm))
            {
                GmUVAlmTmrO1 = PARM(GmUVAlmOffTm);

                GmUVAlmFlg1 = false;
                GmUVAlm1NF  = false;
                GmUVAlm12F  = false;
            }
        }
    }



    //Phase 2
    if ( GmV2MagPu < GmUVAlmDynThr)
    {
        GmUVAlmTmr2  = GmUVAlmTmr2 + DelTm3;
        GmUVAlmTmrO2 = 0.0;
        if ( GmUVAlmTmr2 >= PARM(GmUVAlmTm))
        {
            GmUVAlmTmr2 = PARM(GmUVAlmTm);
            GmUVAlmFlg2 = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    GmUVAlm2NF   = true;
                    GmUVAlmFlgTp = true;
                }
                else
                {
                    GmUVAlm23F   = true;
                    GmUVAlmFlgTp = true;
                }
            }
        }
    }
    else
    {
        GmUVAlmTmr2 = GmUVAlmTmr2 - DelTm3;
        if ( GmUVAlmTmr2 <= 0.0)
        {
            GmUVAlmTmr2 = 0.0;
        }
        if ( GmV2MagPu > PARM(GmUVAlmOff))
        {
            GmUVAlmTmrO2 = GmUVAlmTmrO2 + DelTm3;
            if ( GmUVAlmTmrO2 > PARM(GmUVAlmOffTm))
            {
                GmUVAlmTmrO2 = PARM(GmUVAlmOffTm);

                GmUVAlmFlg2 = false;
                GmUVAlm2NF  = false;
                GmUVAlm23F  = false;
            }
        }
    }


    //Phase 3
    if ( GmV3MagPu < GmUVAlmDynThr)
    {
        GmUVAlmTmr3  = GmUVAlmTmr3 + DelTm3;
        GmUVAlmTmrO3 = 0.0;
        if ( GmUVAlmTmr3 >= PARM(GmUVAlmTm))
        {
            GmUVAlmTmr3 = PARM(GmUVAlmTm);
            GmUVAlmFlg3 = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    GmUVAlm3NF   = true;
                    GmUVAlmFlgTp = true;
                }
                else
                {
                    GmUVAlm31F   = true;
                    GmUVAlmFlgTp = true;
                }
            }
        }
    }
    else
    {
        GmUVAlmTmr3 = GmUVAlmTmr3 - DelTm3;
        if ( GmUVAlmTmr3 <= 0.0)
        {
            GmUVAlmTmr3 = 0.0;
        }
        if ( GmV3MagPu > PARM(GmUVAlmOff))
        {
            GmUVAlmTmrO3 = GmUVAlmTmrO3 + DelTm3;
            if ( GmUVAlmTmrO3 > PARM(GmUVAlmOffTm))
            {
                GmUVAlmTmrO3 = PARM(GmUVAlmOffTm);

                GmUVAlmFlg3 = false;
                GmUVAlm3NF  = false;
                GmUVAlm31F  = false;
            }
        }
    }


    //Check the "All" case
    if ( GmVltType == ALL)
    {
        if ( GmUVAlmFlg1 && GmUVAlmFlg2 && GmUVAlmFlg3 )
        {
            if ( GmVltSource == LINE_TO_NEUT)
            {
                PUSH_DIAG(GmUVAlmLN);
                GmUVAlmLNF    = true;
                GmUVAlmFlgTp  = true;
            }
            else
            {
                PUSH_DIAG(GmUVAlmLL);
                GmUVAlmLLF    = true;
                GmUVAlmFlgTp  = true;
            }
        }
        else
        {
            GmUVAlmLNF  = false;
            GmUVAlmLLF  = false;
        }
        GmUVAlm1NF  = false;
        GmUVAlm12F  = false;
        GmUVAlm2NF  = false;
        GmUVAlm23F  = false;
        GmUVAlm3NF  = false;
        GmUVAlm31F  = false;
    }
    else
    {
        GmUVAlmLNF  = false;
        GmUVAlmLLF  = false;
    }

    //Clear alarms and flags based on inactive type
    if ( GmVltSource == LINE_TO_NEUT)
    {
        GmUVAlm12F  = false;
        GmUVAlm23F  = false;
        GmUVAlm31F  = false;
        GmUVAlmLLF  = false;
    }
    else
    {
        GmUVAlm1NF  = false;
        GmUVAlm2NF  = false;
        GmUVAlm3NF  = false;
        GmUVAlmLNF  = false;

        // GmUVAlmTmr1 = 0.0;    // this looks like bug in ESS
        // GmUVAlmTmr2 = 0.0;    // comment out - need to confirm
        // GmUVAlmTmr3 = 0.0;
    }

    GmUVAlmFlg = GmUVAlmFlgTp;

    if (GmUVAlm1NF)  PUSH_DIAG(GmUVAlm1N);
    if (GmUVAlm12F)  PUSH_DIAG(GmUVAlm12);
    if (GmUVAlm2NF)  PUSH_DIAG(GmUVAlm2N);
    if (GmUVAlm23F)  PUSH_DIAG(GmUVAlm23);
    if (GmUVAlm3NF)  PUSH_DIAG(GmUVAlm3N);
    if (GmUVAlm31F)  PUSH_DIAG(GmUVAlm31);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This is function repsonsible for implementing grid monitor under-voltgage
//    protection for level #1 trip faults.  There are eight separate trip
//    faults for this level. At most, only three of these can occur for a
//    given configuration as shown below.
//
//    Fault vs. Configuration:
//
//             All and Line to Neutral:
//                Flt.GmUV1TrpLN
//
//             All and Line to Line:
//                Flt.GmUV1TrpLL
//
//             Any and Line to Neutral:
//                Flt.GmUV1Trp1N
//                Flt.GmUV1Trp2N
//                Flt.GmUV1Trp3N
//
//             Any and Line to Line:
//                Flt.GmUV1Trp12
//                Flt.GmUV1Trp23
//                Flt.GmUV1Trp31
//
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorUnderVoltageLevel1(void)
{
    unsigned Ph1Trip, Ph2Trip, Ph3Trip;
    float RstTrpTm;

    //Init internal tirp flags to 'false'
    Ph1Trip = Ph2Trip = Ph3Trip = false;


    if ( !PARM(GmUV1TrpInh))
    {
        //Phase 1
        if ( GmV1MagPu < GmUV1TrpThrA )
        {
            GmUV1TrpTmr1 = GmUV1TrpTmr1 + DelTm3;
            if ( GmUV1TrpTmr1 >= PARM(GmUV1TrpTm) )
            {
                GmUV1TrpTmr1 = PARM(GmUV1TrpTm);
                Ph1Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {

                        PUSH_DIAG(GmUV1Trp1N);
                        GmUV1Trp1NF = true;
                        GmUV1TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV1Trp12);
                        GmUV1Trp12F = true;
                        GmUV1TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV1TrpTmr1 = GmUV1TrpTmr1 - DelTm3;
            if ( GmUV1TrpTmr1 <= 0.0)
            {
                GmUV1TrpTmr1 = 0.0;
                GmUV1Trp1NF = false;
                GmUV1Trp12F = false;
            }
        }



        //Phase 2
        if ( GmV2MagPu < GmUV1TrpThrA )
        {
            GmUV1TrpTmr2 = GmUV1TrpTmr2 + DelTm3;
            if ( GmUV1TrpTmr2 >= PARM(GmUV1TrpTm) )
            {
                GmUV1TrpTmr2 = PARM(GmUV1TrpTm);
                Ph2Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV1Trp2N);
                        GmUV1Trp2NF = true;
                        GmUV1TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV1Trp23);
                        GmUV1Trp23F = true;
                        GmUV1TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV1TrpTmr2 = GmUV1TrpTmr2 - DelTm3;
            if ( GmUV1TrpTmr2 <= 0.0)
            {
                GmUV1TrpTmr2 = 0.0;
                GmUV1Trp2NF = false;
                GmUV1Trp23F = false;
            }
        }



        //Phase 3
        if ( GmV3MagPu < GmUV1TrpThrA )
        {
            GmUV1TrpTmr3 = GmUV1TrpTmr3 + DelTm3;
            if ( GmUV1TrpTmr3 >= PARM(GmUV1TrpTm) )
            {
                GmUV1TrpTmr3 = PARM(GmUV1TrpTm);
                Ph3Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV1Trp3N);
                        GmUV1Trp3NF = true;
                        GmUV1TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV1Trp31);
                        GmUV1Trp31F = true;
                        GmUV1TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV1TrpTmr3 = GmUV1TrpTmr3 - DelTm3;
            if ( GmUV1TrpTmr3 <= 0.0)
            {
                GmUV1TrpTmr3 = 0.0;
                GmUV1Trp3NF = false;
                GmUV1Trp31F = false;
            }
        }


        //Check the "Any" case
        if ( GmVltType == ALL)
        {
            if (Ph1Trip && Ph2Trip && Ph3Trip)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmUV1TrpLN);
                    GmUV1TrpLNF  = true;
                    GmUV1TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmUV1TrpLL);
                    GmUV1TrpLLF  = true;
                    GmUV1TrpFlg = true;
                }
            }

        }

    }
    else//trips are inhibited
    {
        GmUV1TrpTmr1 = 0.0;
        GmUV1TrpTmr2 = 0.0;
        GmUV1TrpTmr3 = 0.0;
    }


    if (GmUV1TrpTmr1 + GmUV1TrpTmr2 + GmUV1TrpTmr3 == 0.0F)
    {
        GmUV1TrpLNF = false;
        GmUV1TrpLLF = false;
        GmUV1TrpFlg = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmUV1TrpInh))
    {
        GmUV1TrpLNF =false;
        GmUV1TrpLLF =false;

        GmUV1Trp1NF =false;
        GmUV1Trp2NF =false;
        GmUV1Trp3NF =false;

        GmUV1Trp12F =false;
        GmUV1Trp23F =false;
        GmUV1Trp31F =false;

        GmUV1TrpFlg = false;

        GmUV1TrpTmr1 = 0.0;
        GmUV1TrpTmr2 = 0.0;
        GmUV1TrpTmr3 = 0.0;
    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmUV1TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmUV1TrpTmr1 < RstTrpTm)
            {
                GmUV1Trp1NF = false;
                GmUV1Trp12F = false;
            }
            if (GmUV1TrpTmr2 < RstTrpTm)
            {
                GmUV1Trp2NF = false;
                GmUV1Trp23F = false;
            }
            if (GmUV1TrpTmr3 < RstTrpTm)
            {
                GmUV1Trp3NF = false;
                GmUV1Trp31F = false;
            }
            if ( GmVltType == ANY)
            {
                if ((GmUV1TrpTmr1 < RstTrpTm) && (GmUV1TrpTmr2 < RstTrpTm) && (GmUV1TrpTmr3 < RstTrpTm))
                {
                    GmUV1TrpFlg = false;
                }
            }
            else
            {
                if ((GmUV1TrpTmr1 < RstTrpTm) || (GmUV1TrpTmr2 < RstTrpTm) || (GmUV1TrpTmr3 < RstTrpTm))
                {
                    GmUV1TrpFlg = false;
                }
            }
        }
    }

    GmUV1TrpAct = Ph1Trip || Ph2Trip || Ph3Trip;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   This is function repsonsible for implementing grid monitor under-voltgage
//   protection for level #2 trip faults.  There are eight separate trip
//   faults for this level. At most, only three of these can occur for a
//   given configuration as shown below.
//
//   Fault vs. Configuration:
//
//           All and Line to Neutral:
//               Flt.GmUV2TrpLN
//
//           All and Line to Line:
//               Flt.GmUV2TrpLL
//
//           Any and Line to Neutral:
//               Flt.GmUV2Trp1N
//               Flt.GmUV2Trp2N
//               Flt.GmUV2Trp3N
//
//           Any and Line to Line:
//               Flt.GmUV2Trp12
//               Flt.GmUV2Trp23
//               Flt.GmUV2Trp31
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorUnderVoltageLevel2(void)
{
    unsigned Ph1Trip, Ph2Trip, Ph3Trip;
    float RstTrpTm;

    //Init internal tirp flags to 'false'
    Ph1Trip = Ph2Trip = Ph3Trip = false;

    if ( !PARM(GmUV2TrpInh))
    {
        //Phase 1
        if ( GmV1MagPu < GmUV2TrpThrA )
        {
            GmUV2TrpTmr1 = GmUV2TrpTmr1 + DelTm3;
            if ( GmUV2TrpTmr1 >= PARM(GmUV2TrpTm) )
            {
                GmUV2TrpTmr1 = PARM(GmUV2TrpTm);
                Ph1Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV2Trp1N);
                        GmUV2Trp1NF = true;
                        GmUV2TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV2Trp12);
                        GmUV2Trp12F = true;
                        GmUV2TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV2TrpTmr1 = GmUV2TrpTmr1 - DelTm3;
            if ( GmUV2TrpTmr1 <= 0.0)
            {
                GmUV2TrpTmr1 = 0.0;
                GmUV2Trp1NF = false;
                GmUV2Trp12F = false;
            }
        }



        //Phase 2
        if ( GmV2MagPu < GmUV2TrpThrA )
        {
            GmUV2TrpTmr2 = GmUV2TrpTmr2 + DelTm3;
            if ( GmUV2TrpTmr2 >= PARM(GmUV2TrpTm))
            {
                GmUV2TrpTmr2 = PARM(GmUV2TrpTm);
                Ph2Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV2Trp2N);
                        GmUV2Trp2NF = true;
                        GmUV2TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV2Trp23);
                        GmUV2Trp23F = true;
                        GmUV2TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV2TrpTmr2 = GmUV2TrpTmr2 - DelTm3;
            if ( GmUV2TrpTmr2 <= 0.0)
            {
                GmUV2TrpTmr2 = 0.0;
                GmUV2Trp2NF = false;
                GmUV2Trp23F = false;
            }
        }



        //Phase 3
        if ( GmV3MagPu < GmUV2TrpThrA )
        {
            GmUV2TrpTmr3 = GmUV2TrpTmr3 + DelTm3;
            if ( GmUV2TrpTmr3 >= PARM(GmUV2TrpTm))
            {
                GmUV2TrpTmr3 = PARM(GmUV2TrpTm);
                Ph3Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV2Trp3N);
                        GmUV2Trp3NF = true;
                        GmUV2TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV2Trp31);
                        GmUV2Trp31F = true;
                        GmUV2TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV2TrpTmr3 = GmUV2TrpTmr3 - DelTm3;
            if ( GmUV2TrpTmr3 <= 0.0)
            {
                GmUV2TrpTmr3 = 0.0;
                GmUV2Trp3NF = false;
                GmUV2Trp31F = false;
            }
        }



        //Check the "Any" case
        if ( GmVltType == ALL)
        {
            if (Ph1Trip && Ph2Trip && Ph3Trip)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmUV2TrpLN);
                    GmUV2TrpLNF  = true;
                    GmUV2TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmUV2TrpLL);
                    GmUV2TrpLLF  = true;
                    GmUV2TrpFlg = true;
                }
            }

        }

    }
    else//trips are inhibited
    {
        GmUV2TrpTmr1 = 0.0;
        GmUV2TrpTmr2 = 0.0;
        GmUV2TrpTmr3 = 0.0;
    }


    if (GmUV2TrpTmr1 + GmUV2TrpTmr2 + GmUV2TrpTmr3 == 0.0F)
    {
        GmUV2TrpLNF = false;
        GmUV2TrpLLF = false;
        GmUV2TrpFlg = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmUV2TrpInh))
    {
        GmUV2TrpLNF =false;
        GmUV2TrpLLF =false;

        GmUV2Trp1NF =false;
        GmUV2Trp2NF =false;
        GmUV2Trp3NF =false;

        GmUV2Trp12F =false;
        GmUV2Trp23F =false;
        GmUV2Trp31F =false;

        GmUV2TrpFlg = false;

        GmUV2TrpTmr1 = 0.0;
        GmUV2TrpTmr2 = 0.0;
        GmUV2TrpTmr3 = 0.0;

    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmUV2TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmUV2TrpTmr1 < RstTrpTm)
            {
                GmUV2Trp1NF = false;
                GmUV2Trp12F = false;
            }
            if (GmUV2TrpTmr2 < RstTrpTm)
            {
                GmUV2Trp2NF = false;
                GmUV2Trp23F = false;
            }
            if (GmUV2TrpTmr3 < RstTrpTm)
            {
                GmUV2Trp3NF = false;
                GmUV2Trp31F = false;
            }
            if ( GmVltType == ANY)
            {
                if ((GmUV2TrpTmr1 < RstTrpTm) && (GmUV2TrpTmr2 < RstTrpTm) && (GmUV2TrpTmr3 < RstTrpTm))
                {
                    GmUV2TrpFlg = false;
                }
            }
            else
            {
                if ((GmUV2TrpTmr1 < RstTrpTm) || (GmUV2TrpTmr2 < RstTrpTm) || (GmUV2TrpTmr3 < RstTrpTm))
                {
                    GmUV2TrpFlg = false;
                }
            }
        }
    }

    GmUV2TrpAct = Ph1Trip || Ph2Trip || Ph3Trip;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   This is function repsonsible for implementing grid monitor under-voltgage
//   protection for level #3 trip faults.  There are eight separate trip
//   faults for this level. At most, only three of these can occur for a
//   given configuration as shown below.
//
//   Fault vs. Configuration:
//
//             All and Line to Neutral:
//                Flt.GmUV3TrpLN
//
//             All and Line to Line:
//                Flt.GmUV3TrpLL
//
//             Any and Line to Neutral:
//                Flt.GmUV3Trp1N
//                Flt.GmUV3Trp2N
//                Flt.GmUV3Trp3N
//
//             Any and Line to Line:
//                Flt.GmUV3Trp12
//                Flt.GmUV3Trp23
//                Flt.GmUV3Trp31
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorUnderVoltageLevel3(void)
{

    unsigned Ph1Trip, Ph2Trip, Ph3Trip;
    float RstTrpTm;

    //Init internal tirp flags to 'false'
    Ph1Trip = Ph2Trip = Ph3Trip = false;

    if ( !PARM(GmUV3TrpInh))
    {
        //Phase 1
        if ( GmV1MagPu < GmUV3TrpThrA )
        {
            GmUV3TrpTmr1 = GmUV3TrpTmr1 + DelTm3;
            if ( GmUV3TrpTmr1 >= PARM(GmUV3TrpTm) )
            {
                GmUV3TrpTmr1 = PARM(GmUV3TrpTm);
                Ph1Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV3Trp1N);
                        GmUV3Trp1NF = true;
                        GmUV3TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV3Trp12);
                        GmUV3Trp12F = true;
                        GmUV3TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV3TrpTmr1 = GmUV3TrpTmr1 - DelTm3;
            if ( GmUV3TrpTmr1 <= 0.0 )
            {
                GmUV3TrpTmr1 = 0.0;
                GmUV3Trp1NF = false;
                GmUV3Trp12F = false;
            }
        }



        //Phase 2
        if ( GmV2MagPu < GmUV3TrpThrA )
        {
            GmUV3TrpTmr2 = GmUV3TrpTmr2 + DelTm3;
            if ( GmUV3TrpTmr2 >= PARM(GmUV3TrpTm) )
            {
                GmUV3TrpTmr2 = PARM(GmUV3TrpTm);
                Ph2Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV3Trp2N);
                        GmUV3Trp2NF = true;
                        GmUV3TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV3Trp23);
                        GmUV3Trp23F = true;
                        GmUV3TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV3TrpTmr2 = GmUV3TrpTmr2 - DelTm3;
            if ( GmUV3TrpTmr2 <= 0.0 )
            {
                GmUV3TrpTmr2 = 0.0;
                GmUV3Trp2NF = false;
                GmUV3Trp23F = false;
            }
        }



        //Phase 3
        if ( GmV3MagPu < GmUV3TrpThrA )
        {
            GmUV3TrpTmr3 = GmUV3TrpTmr3 + DelTm3;
            if ( GmUV3TrpTmr3 >= PARM(GmUV3TrpTm))
            {
                GmUV3TrpTmr3 = PARM(GmUV3TrpTm);
                Ph3Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV3Trp3N);
                        GmUV3Trp3NF = true;
                        GmUV3TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV3Trp31);
                        GmUV3Trp31F = true;
                        GmUV3TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV3TrpTmr3 = GmUV3TrpTmr3 - DelTm3;
            if ( GmUV3TrpTmr3 <= 0.0)
            {
                GmUV3TrpTmr3 = 0.0;
                GmUV3Trp3NF = false;
                GmUV3Trp31F = false;
            }
        }



        //Check the "Any" case
        if ( GmVltType == ALL)
        {
            if (Ph1Trip && Ph2Trip && Ph3Trip)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmUV3TrpLN);
                    GmUV3TrpLNF  = true;
                    GmUV3TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmUV3TrpLL);
                    GmUV3TrpLLF  = true;
                    GmUV3TrpFlg = true;
                }
            }

        }

    }
    else//trips are inhibited
    {
        GmUV3TrpTmr1 = 0.0;
        GmUV3TrpTmr2 = 0.0;
        GmUV3TrpTmr3 = 0.0;
    }

    if (GmUV3TrpTmr1 + GmUV3TrpTmr2 + GmUV3TrpTmr3 == 0.0F)
    {
        GmUV3TrpLNF = false;
        GmUV3TrpLLF = false;
        GmUV3TrpFlg = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmUV3TrpInh))
    {
        GmUV3TrpLNF =false;
        GmUV3TrpLLF =false;

        GmUV3Trp1NF =false;
        GmUV3Trp2NF =false;
        GmUV3Trp3NF =false;

        GmUV3Trp12F =false;
        GmUV3Trp23F =false;
        GmUV3Trp31F =false;

        GmUV3TrpFlg = false;

        GmUV3TrpTmr1 = 0.0;
        GmUV3TrpTmr2 = 0.0;
        GmUV3TrpTmr3 = 0.0;


    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmUV3TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmUV3TrpTmr1 < RstTrpTm)
            {
                GmUV3Trp1NF = false;
                GmUV3Trp12F = false;
            }
            if (GmUV3TrpTmr2 < RstTrpTm)
            {
                GmUV3Trp2NF = false;
                GmUV3Trp23F = false;
            }
            if (GmUV3TrpTmr3 < RstTrpTm)
            {
                GmUV3Trp3NF = false;
                GmUV3Trp31F = false;
            }
            if ( GmVltType == ANY)
            {
                if ((GmUV3TrpTmr1 < RstTrpTm) && (GmUV3TrpTmr2 < RstTrpTm) && (GmUV3TrpTmr3 < RstTrpTm))
                {
                    GmUV3TrpFlg = false;
                }
            }
            else
            {
                if ((GmUV3TrpTmr1 < RstTrpTm) || (GmUV3TrpTmr2 < RstTrpTm) || (GmUV3TrpTmr3 < RstTrpTm))
                {
                    GmUV3TrpFlg = false;
                }
            }
        }
    }

    GmUV3TrpAct = Ph1Trip || Ph2Trip || Ph3Trip;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   This is function repsonsible for implementing grid monitor under-voltgage
//   protection for level #3 trip faults.  There are eight separate trip
//   faults for this level. At most, only three of these can occur for a
//   given configuration as shown below.
//
//   Fault vs. Configuration:
//
//             All and Line to Neutral:
//                Flt.GmUV4TrpLN
//
//             All and Line to Line:
//                Flt.GmUV4TrpLL
//
//             Any and Line to Neutral:
//                Flt.GmUV4Trp1N
//                Flt.GmUV4Trp2N
//                Flt.GmUV4Trp3N
//
//             Any and Line to Line:
//                Flt.GmUV4Trp12
//                Flt.GmUV4Trp23
//                Flt.GmUV4Trp31
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorUnderVoltageLevel4(void)
{

    unsigned Ph1Trip, Ph2Trip, Ph3Trip;
    float RstTrpTm;

    //Init internal tirp flags to 'false'
    Ph1Trip = Ph2Trip = Ph3Trip = false;


    if ( !PARM(GmUV4TrpInh))
    {
        //Phase 1
        if ( GmV1MagPu < GmUV4TrpThrA )
        {
            GmUV4TrpTmr1 = GmUV4TrpTmr1 + DelTm3;
            if ( GmUV4TrpTmr1 >= PARM(GmUV4TrpTm))
            {
                GmUV4TrpTmr1 = PARM(GmUV4TrpTm);
                Ph1Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV4Trp1N);
                        GmUV4Trp1NF = true;
                        GmUV4TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV4Trp12);
                        GmUV4Trp12F = true;
                        GmUV4TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV4TrpTmr1 = GmUV4TrpTmr1 - DelTm3;
            if ( GmUV4TrpTmr1 <= 0.0)
            {
                GmUV4TrpTmr1 = 0.0;
                GmUV4Trp1NF = false;
                GmUV4Trp12F = false;
            }
        }



        //Phase 2
        if ( GmV2MagPu < GmUV4TrpThrA )
        {
            GmUV4TrpTmr2 = GmUV4TrpTmr2 + DelTm3;
            if ( GmUV4TrpTmr2 >= PARM(GmUV4TrpTm))
            {
                GmUV4TrpTmr2 = PARM(GmUV4TrpTm);
                Ph2Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV4Trp2N);
                        GmUV4Trp2NF = true;
                        GmUV4TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV4Trp23);
                        GmUV4Trp23F = true;
                        GmUV4TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV4TrpTmr2 = GmUV4TrpTmr2 - DelTm3;
            if ( GmUV4TrpTmr2 <= 0.0)
            {
                GmUV4TrpTmr2 = 0.0;
                GmUV4Trp2NF = false;
                GmUV4Trp23F = false;
            }
        }



        //Phase 3
        if ( GmV3MagPu < GmUV4TrpThrA )
        {
            GmUV4TrpTmr3 = GmUV4TrpTmr3 + DelTm3;
            if ( GmUV4TrpTmr3 >= PARM(GmUV4TrpTm))
            {
                GmUV4TrpTmr3 = PARM(GmUV4TrpTm);
                Ph3Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV4Trp3N);
                        GmUV4Trp3NF = true;
                        GmUV4TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV4Trp31);
                        GmUV4Trp31F = true;
                        GmUV4TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV4TrpTmr3 = GmUV4TrpTmr3 - DelTm3;
            if ( GmUV4TrpTmr3 <= 0.0)
            {
                GmUV4TrpTmr3 = 0.0;
                GmUV4Trp3NF = false;
                GmUV4Trp31F = false;
            }
        }



        //Check the "Any" case
        if ( GmVltType == ALL)
        {
            if (Ph1Trip && Ph2Trip && Ph3Trip)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmUV4TrpLN);
                    GmUV4TrpLNF  = true;
                    GmUV4TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmUV4TrpLL);
                    GmUV4TrpLLF  = true;
                    GmUV4TrpFlg = true;
                }
            }

        }

    }
    else//trips are inhibited
    {
        GmUV4TrpTmr1 = 0.0;
        GmUV4TrpTmr2 = 0.0;
        GmUV4TrpTmr3 = 0.0;
    }


    if (GmUV4TrpTmr1 + GmUV4TrpTmr2 + GmUV4TrpTmr3 == 0.0F)
    {
        GmUV4TrpLNF = false;
        GmUV4TrpLLF = false;
        GmUV4TrpFlg = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmUV4TrpInh))
    {
        GmUV4TrpLNF =false;
        GmUV4TrpLLF =false;

        GmUV4Trp1NF =false;
        GmUV4Trp2NF =false;
        GmUV4Trp3NF =false;

        GmUV4Trp12F =false;
        GmUV4Trp23F =false;
        GmUV4Trp31F =false;

        GmUV4TrpFlg = false;

        GmUV4TrpTmr1 = 0.0;
        GmUV4TrpTmr2 = 0.0;
        GmUV4TrpTmr3 = 0.0;


    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmUV4TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmUV4TrpTmr1 < RstTrpTm)
            {
                GmUV4Trp1NF = false;
                GmUV4Trp12F = false;
            }
            if (GmUV4TrpTmr2 < RstTrpTm)
            {
                GmUV4Trp2NF = false;
                GmUV4Trp23F = false;
            }
            if (GmUV4TrpTmr3 < RstTrpTm)
            {
                GmUV4Trp3NF = false;
                GmUV4Trp31F = false;
            }
            if ( GmVltType == ANY)
            {
                if ((GmUV4TrpTmr1 < RstTrpTm) && (GmUV4TrpTmr2 < RstTrpTm) && (GmUV4TrpTmr3 < RstTrpTm))
                {
                    GmUV4TrpFlg = false;
                }
            }
            else
            {
                if ((GmUV4TrpTmr1 < RstTrpTm) || (GmUV4TrpTmr2 < RstTrpTm) || (GmUV4TrpTmr3 < RstTrpTm))
                {
                    GmUV4TrpFlg = false;
                }
            }
        }
    }

    GmUV4TrpAct = Ph1Trip || Ph2Trip || Ph3Trip;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   This is function repsonsible for implementing grid monitor under-voltgage
//   protection for level #3 trip faults.  There are eight separate trip
//   faults for this level. At most, only three of these can occur for a
//   given configuration as shown below.
//
//   Fault vs. Configuration:
//
//            All and Line to Neutral:
//                Flt.GmUV5TrpLN
//
//            All and Line to Line:
//                Flt.GmUV5TrpLL
//
//            Any and Line to Neutral:
//                Flt.GmUV5Trp1N
//                Flt.GmUV5Trp2N
//                Flt.GmUV5Trp3N
//
//            Any and Line to Line:
//                Flt.GmUV5Trp12
//                Flt.GmUV5Trp23
//                Flt.GmUV5Trp31
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorUnderVoltageLevel5(void)
{

    unsigned Ph1Trip, Ph2Trip, Ph3Trip;
    float RstTrpTm;

    //Init internal tirp flags to 'false'
    Ph1Trip = Ph2Trip = Ph3Trip = false;


    if ( !PARM(GmUV5TrpInh))
    {
        //Phase 1
        if ( GmV1MagPu < GmUV5TrpThrA )
        {
            GmUV5TrpTmr1 = GmUV5TrpTmr1 + DelTm3;
            if ( GmUV5TrpTmr1 >= PARM(GmUV5TrpTm))
            {
                GmUV5TrpTmr1 = PARM(GmUV5TrpTm);
                Ph1Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV5Trp1N);
                        GmUV5Trp1NF = true;
                        GmUV5TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV5Trp12);
                        GmUV5Trp12F = true;
                        GmUV5TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV5TrpTmr1 = GmUV5TrpTmr1 - DelTm3;
            if ( GmUV5TrpTmr1 <= 0.0)
            {
                GmUV5TrpTmr1 = 0.0;
                GmUV5Trp1NF = false;
                GmUV5Trp12F = false;
            }
        }


        //Phase 2
        if ( GmV2MagPu < GmUV5TrpThrA )
        {
            GmUV5TrpTmr2 = GmUV5TrpTmr2 + DelTm3;
            if ( GmUV5TrpTmr2 >= PARM(GmUV5TrpTm))
            {
                GmUV5TrpTmr2 = PARM(GmUV5TrpTm);
                Ph2Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV5Trp2N);
                        GmUV5Trp2NF = true;
                        GmUV5TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV5Trp23);
                        GmUV5Trp23F = true;
                        GmUV5TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV5TrpTmr2 = GmUV5TrpTmr2 - DelTm3;
            if ( GmUV5TrpTmr2 <= 0.0)
            {
                GmUV5TrpTmr2 = 0.0;
                GmUV5Trp2NF = false;
                GmUV5Trp23F = false;
            }
        }


        //Phase 3
        if ( GmV3MagPu < GmUV5TrpThrA )
        {
            GmUV5TrpTmr3 = GmUV5TrpTmr3 + DelTm3;
            if ( GmUV5TrpTmr3 >= PARM(GmUV5TrpTm))
            {
                GmUV5TrpTmr3 = PARM(GmUV5TrpTm);
                Ph3Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV5Trp3N);
                        GmUV5Trp3NF = true;
                        GmUV5TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV5Trp31);
                        GmUV5Trp31F = true;
                        GmUV5TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV5TrpTmr3 = GmUV5TrpTmr3 - DelTm3;
            if ( GmUV5TrpTmr3 <= 0.0)
            {
                GmUV5TrpTmr3 = 0.0;
                GmUV5Trp3NF = false;
                GmUV5Trp31F = false;
            }
        }


        //Check the "Any" case
        if ( GmVltType == ALL)
        {
            if (Ph1Trip && Ph2Trip && Ph3Trip)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmUV5TrpLN);
                    GmUV5TrpLNF  = true;
                    GmUV5TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmUV5TrpLL);
                    GmUV5TrpLLF  = true;
                    GmUV5TrpFlg = true;
                }
            }

        }

    }
    else//trips are inhibited
    {
        GmUV5TrpTmr1 = 0.0;
        GmUV5TrpTmr2 = 0.0;
        GmUV5TrpTmr3 = 0.0;
    }


    if (GmUV5TrpTmr1 + GmUV5TrpTmr2 + GmUV5TrpTmr3 == 0.0F)
    {
        GmUV5TrpLNF = false;
        GmUV5TrpLLF = false;
        GmUV5TrpFlg = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmUV5TrpInh))
    {
        GmUV5TrpLNF =false;
        GmUV5TrpLLF =false;

        GmUV5Trp1NF =false;
        GmUV5Trp2NF =false;
        GmUV5Trp3NF =false;

        GmUV5Trp12F =false;
        GmUV5Trp23F =false;
        GmUV5Trp31F =false;

        GmUV5TrpFlg = false;

        GmUV5TrpTmr1 = 0.0;
        GmUV5TrpTmr2 = 0.0;
        GmUV5TrpTmr3 = 0.0;
    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmUV5TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmUV5TrpTmr1 < RstTrpTm)
            {
                GmUV5Trp1NF = false;
                GmUV5Trp12F = false;
            }
            if (GmUV5TrpTmr2 < RstTrpTm)
            {
                GmUV5Trp2NF = false;
                GmUV5Trp23F = false;
            }
            if (GmUV5TrpTmr3 < RstTrpTm)
            {
                GmUV5Trp3NF = false;
                GmUV5Trp31F = false;
            }
            if ( GmVltType == ANY)
            {
                if ((GmUV5TrpTmr1 < RstTrpTm) && (GmUV5TrpTmr2 < RstTrpTm) && (GmUV5TrpTmr3 < RstTrpTm))
                {
                    GmUV5TrpFlg = false;
                }
            }
            else
            {
                if ((GmUV5TrpTmr1 < RstTrpTm) || (GmUV5TrpTmr2 < RstTrpTm) || (GmUV5TrpTmr3 < RstTrpTm))
                {
                    GmUV5TrpFlg = false;
                }
            }
        }
    }

    GmUV5TrpAct = Ph1Trip || Ph2Trip || Ph3Trip;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   This is function repsonsible for implementing grid monitor under-voltgage
//   protection for level #3 trip faults.  There are eight separate trip
//   faults for this level. At most, only three of these can occur for a
//   given configuration as shown below.
//
//   Fault vs. Configuration:
//
//             All and Line to Neutral:
//                Flt.GmUV6TrpLN
//
//             All and Line to Line:
//                Flt.GmUV6TrpLL
//
//             Any and Line to Neutral:
//                Flt.GmUV6Trp1N
//                Flt.GmUV6Trp2N
//                Flt.GmUV6Trp3N
//
//             Any and Line to Line:
//                Flt.GmUV6Trp12
//                Flt.GmUV6Trp23
//                Flt.GmUV6Trp31
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorUnderVoltageLevel6(void)
{

    unsigned Ph1Trip, Ph2Trip, Ph3Trip;
    float RstTrpTm;

    //Init internal tirp flags to 'false'
    Ph1Trip = Ph2Trip = Ph3Trip = false;


    if ( !PARM(GmUV6TrpInh))
    {
        //Phase 1
        if ( GmV1MagPu < GmUV6TrpThrA )
        {
            GmUV6TrpTmr1 = GmUV6TrpTmr1 + DelTm3;
            if ( GmUV6TrpTmr1 >= PARM(GmUV6TrpTm))
            {
                GmUV6TrpTmr1 = PARM(GmUV6TrpTm);
                Ph1Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV6Trp1N);
                        GmUV6Trp1NF = true;
                        GmUV6TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV6Trp12);
                        GmUV6Trp12F = true;
                        GmUV6TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV6TrpTmr1 = GmUV6TrpTmr1 - DelTm3;
            if ( GmUV6TrpTmr1 <= 0.0)
            {
                GmUV6TrpTmr1 = 0.0;
                GmUV6Trp1NF = false;
                GmUV6Trp12F = false;
            }
        }


        //Phase 2
        if ( GmV2MagPu < GmUV6TrpThrA )
        {
            GmUV6TrpTmr2 = GmUV6TrpTmr2 + DelTm3;
            if ( GmUV6TrpTmr2 >= PARM(GmUV6TrpTm))
            {
                GmUV6TrpTmr2 = PARM(GmUV6TrpTm);
                Ph2Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV6Trp2N);
                        GmUV6Trp2NF = true;
                        GmUV6TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV6Trp23);
                        GmUV6Trp23F = true;
                        GmUV6TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV6TrpTmr2 = GmUV6TrpTmr2 - DelTm3;
            if ( GmUV6TrpTmr2 <= 0.0)
            {
                GmUV6TrpTmr2 = 0.0;
                GmUV6Trp2NF = false;
                GmUV6Trp23F = false;
            }
        }


        //Phase 3
        if ( GmV3MagPu < GmUV6TrpThrA )
        {
            GmUV6TrpTmr3 = GmUV6TrpTmr3 + DelTm3;
            if ( GmUV6TrpTmr3 >= PARM(GmUV6TrpTm))
            {
                GmUV6TrpTmr3 = PARM(GmUV6TrpTm);
                Ph3Trip     = true;
                if ( GmVltType == ANY)
                {
                    if ( GmVltSource == LINE_TO_NEUT)
                    {
                        PUSH_DIAG(GmUV6Trp3N);
                        GmUV6Trp3NF = true;
                        GmUV6TrpFlg = true;
                    }
                    else
                    {
                        PUSH_DIAG(GmUV6Trp31);
                        GmUV6Trp31F = true;
                        GmUV6TrpFlg = true;
                    }
                }
            }
        }
        else
        {
            GmUV6TrpTmr3 = GmUV6TrpTmr3 - DelTm3;
            if ( GmUV6TrpTmr3 <= 0.0)
            {
                GmUV6TrpTmr3 = 0.0;
                GmUV6Trp3NF = false;
                GmUV6Trp31F = false;
            }
        }


        //Check the "Any" case
        if ( GmVltType == ALL)
        {
            if (Ph1Trip && Ph2Trip && Ph3Trip)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmUV6TrpLN);
                    GmUV6TrpLNF  = true;
                    GmUV6TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmUV6TrpLL);
                    GmUV6TrpLLF  = true;
                    GmUV6TrpFlg = true;
                }
            }

        }

    }
    else//trips are inhibited
    {
        GmUV6TrpTmr1 = 0.0;
        GmUV6TrpTmr2 = 0.0;
        GmUV6TrpTmr3 = 0.0;
    }


    if (GmUV6TrpTmr1 + GmUV6TrpTmr2 + GmUV6TrpTmr3 == 0.0F)
    {
        GmUV6TrpLNF = false;
        GmUV6TrpLLF = false;
        GmUV6TrpFlg = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmUV6TrpInh))
    {
        GmUV6TrpLNF =false;
        GmUV6TrpLLF =false;

        GmUV6Trp1NF =false;
        GmUV6Trp2NF =false;
        GmUV6Trp3NF =false;

        GmUV6Trp12F =false;
        GmUV6Trp23F =false;
        GmUV6Trp31F =false;

        GmUV6TrpFlg = false;

        GmUV6TrpTmr1 = 0.0;
        GmUV6TrpTmr2 = 0.0;
        GmUV6TrpTmr3 = 0.0;
    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmUV6TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmUV6TrpTmr1 < RstTrpTm)
            {
                GmUV6Trp1NF = false;
                GmUV6Trp12F = false;
            }
            if (GmUV6TrpTmr2 < RstTrpTm)
            {
                GmUV6Trp2NF = false;
                GmUV6Trp23F = false;
            }
            if (GmUV6TrpTmr3 < RstTrpTm)
            {
                GmUV6Trp3NF = false;
                GmUV6Trp31F = false;
            }
            if ( GmVltType == ANY)
            {
                if ((GmUV6TrpTmr1 < RstTrpTm) && (GmUV6TrpTmr2 < RstTrpTm) && (GmUV6TrpTmr3 < RstTrpTm))
                {
                    GmUV6TrpFlg = false;
                }
            }
            else
            {
                if ((GmUV6TrpTmr1 < RstTrpTm) || (GmUV6TrpTmr2 < RstTrpTm) || (GmUV6TrpTmr3 < RstTrpTm))
                {
                    GmUV6TrpFlg = false;
                }
            }
        }
    }

    GmUV6TrpAct = Ph1Trip || Ph2Trip || Ph3Trip;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This is function repsonsible for implementing grid monitor under
//    voltgage protection for the 'sloped' curve portion of the capablility
//    curve, allowing the configuration to get the most out of the converter
//    capability. The calculated, dynamic volt*sec theshold for this function
//    is made assuming that the voltage drop is a step and stays at the voltage
//    level until a fault occurs. This will give a fault at the desired time.
//    The function was implemented using volt seconds in an attempt to cover
//    the case where the voltage may change again before a trip occurs once it
//    is below the activation level. This does not give perfect results, but
//    yields better results than the simple function that causes a trip after
//    a certain time based on the voltage level.
//    Note that a common functionality is essentially duplicated for each phase,
//    by enabling this function to act as the other grid monitor uv/ov functions
//    giving faults for each phase on a line-line or line-neutral basis, or
//    giving either a line to line or line to neutral fault only when all
//    phases are low.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorUnderVoltageCurve(void)
{
    float Volt2;


    //Init internal tirp flags to 'false'
    GmCvPh1Low = GmCvPh2Low = GmCvPh3Low = false;

    if ( !PARM(GmUVCvTrpInh) )
    {

        ////////////////////////////////////////////////
        // Phase 1
        ////////////////////////////////////////////////
        //filtered phase 1 voltage
        GmUVCvVMgFl1 = GmUVCvVMgFl1 + DelTm3*PARM(GmUVCvWV)*(GmV1MagPu - GmUVCvVMgFl1);

        if ( GmUVCvVMgFl1 < PARM(GmUVCvUpr))
        {
            GmUVCVSDyn1 = GmUVCvConstA*(GmUVCvVMgFl1*GmUVCvVMgFl1) + GmUVCvConstB*GmUVCvVMgFl1 + GmUVCvConstC;
        }
        else
        {
            GmUVCVSDyn1 = GmUVCvVSThr1;
        }

        // Form the per unit voltage drop signal and volt*sec
        GmUVCvVMag1 = 1.0 - GmUVCvVMgFl1;

        Volt2 = GmUVCvVMag1*GmUVCvVMag1/(2.0*(GmUVCvVMgFl1-GmUVCvConstD));

        // Do positive integration of UV mag unless voltage is above threshold, then subtract from integrator
        if ( GmUVCvVMgFl1 <= PARM(GmUVCvUpr ))
        {
            if ((GmUVCvVMgFl1 > PARM(GmUVCvLwr )) && !L_LTestIp )
            {
                GmUVcVSDel1 = DelTm3*(GmUVCvVMag1+Volt2)*(GmUVCvVSThr1/GmUVCVSDyn1);
            }
            else
            {
                GmUVcVSDel1 = 0.0; //Freeze integrator when voltage below lower curve point.
            }

        }
        else
        {
            GmUVcVSDel1 = -DelTm3*(GmUVCvVMgFl1-PARM(GmUVCvUpr))*(GmUVCvVSThr1/GmUVCVSDyn1);
        }

        GmUVCvVS1= GmUVCvVS1 + GmUVcVSDel1;

        if ( GmUVCvVS1 < 0.0)
        {
            GmUVCvVS1 = 0.0;
            GmUVCvTrp1NF = false;
            GmUVCvTrp12F = false;
        }

        if ( (GmUVCvVS1 >= GmUVCvVSThr1) )
        {
            GmCvPh1Low     = true;
            GmUVCvVS1 = GmUVCvVSThr1;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmUVCvTrp1N);
                    GmUVCvTrp1NF = true;
                    GmUVCvTrpF   = true;
                }
                else
                {
                    PUSH_DIAG(GmUVCvTrp12);
                    GmUVCvTrp12F = true;
                    GmUVCvTrpF   = true;
                }
            }
        }


        ////////////////////////////////////////////////
        // Phase 2
        ////////////////////////////////////////////////
        //filtered phase 2 voltage
        GmUVCvVMgFl2 = GmUVCvVMgFl2 + DelTm3*PARM(GmUVCvWV)*(GmV2MagPu - GmUVCvVMgFl2);

        if ( GmUVCvVMgFl2 < PARM(GmUVCvUpr))
        {
            GmUVCVSDyn2 = GmUVCvConstA*(GmUVCvVMgFl2*GmUVCvVMgFl2) + GmUVCvConstB*GmUVCvVMgFl2 + GmUVCvConstC;
        }
        else
        {
            GmUVCVSDyn2 = GmUVCvVSThr2;
        }

        // Form the per unit voltage drop signal and volt*sec
        GmUVCvVMag2 = 1.0 - GmUVCvVMgFl2;

        Volt2 = GmUVCvVMag2*GmUVCvVMag2/(2.0*(GmUVCvVMgFl2-GmUVCvConstD));

        // Do positive integration of UV mag unless voltage is above threshold, then subtract from integrator
        if ( GmUVCvVMgFl2 <= PARM(GmUVCvUpr))
        {
            if ((GmUVCvVMgFl2 > PARM(GmUVCvLwr )) && !L_LTestIp )
            {
                GmUVcVSDel2 = DelTm3*(GmUVCvVMag2+Volt2)*(GmUVCvVSThr2/GmUVCVSDyn2);
            }
            else
            {
                GmUVcVSDel2 =0.0; //Freeze integrator when voltage below lower curve point.
            }
        }
        else
        {
            GmUVcVSDel2 = -DelTm3*(GmUVCvVMgFl2-PARM(GmUVCvUpr))*(GmUVCvVSThr2/GmUVCVSDyn2);
        }

        GmUVCvVS2= GmUVCvVS2 + GmUVcVSDel2;

        if ( GmUVCvVS2 < 0.0)
        {
            GmUVCvVS2 = 0.0;
            GmUVCvTrp2NF = false;
            GmUVCvTrp23F = false;
        }


        if ( (GmUVCvVS2 >= GmUVCvVSThr2)  )
        {
            GmCvPh2Low     = true;
            GmUVCvVS2 = GmUVCvVSThr2;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmUVCvTrp2N);
                    GmUVCvTrp2NF = true;
                    GmUVCvTrpF   = true;
                }
                else
                {
                    PUSH_DIAG(GmUVCvTrp23);
                    GmUVCvTrp23F = true;
                    GmUVCvTrpF   = true;
                }
            }
        }



        ////////////////////////////////////////////////
        // Phase 3
        ////////////////////////////////////////////////
        //filtered phase 3 voltage
        GmUVCvVMgFl3 = GmUVCvVMgFl3 + DelTm3*PARM(GmUVCvWV)*(GmV3MagPu - GmUVCvVMgFl3);

        if ( GmUVCvVMgFl3 < PARM(GmUVCvUpr))
        {
            GmUVCVSDyn3 = GmUVCvConstA*(GmUVCvVMgFl3*GmUVCvVMgFl3) + GmUVCvConstB*GmUVCvVMgFl3 + GmUVCvConstC;
        }
        else
        {
            GmUVCVSDyn3 = GmUVCvVSThr3;
        }

        // Form the per unit voltage drop signal and volt*sec
        GmUVCvVMag3 = 1.0 - GmUVCvVMgFl3;

        Volt2 = GmUVCvVMag3*GmUVCvVMag3/(2.0*(GmUVCvVMgFl3-GmUVCvConstD));

        // Do positive integration of UV mag unless voltage is above threshold, then subtract from integrator
        if ( GmUVCvVMgFl3 <= PARM(GmUVCvUpr))
        {
            if ((GmUVCvVMgFl3 > PARM(GmUVCvLwr )) && !L_LTestIp )
            {
                GmUVcVSDel3 = DelTm3*(GmUVCvVMag3+Volt2)*(GmUVCvVSThr3/GmUVCVSDyn3);
            }
            else
            {
                GmUVcVSDel3 =0.0; //Freeze integrator when voltage below lower curve point.
            }

        }
        else
        {
            GmUVcVSDel3 = -DelTm3*(GmUVCvVMgFl3-PARM(GmUVCvUpr))*(GmUVCvVSThr3/GmUVCVSDyn3);
        }

        GmUVCvVS3= GmUVCvVS3 + GmUVcVSDel3;

        if ( GmUVCvVS3 < 0.0)
        {
            GmUVCvVS3 = 0.0;
            GmUVCvTrp3NF = false;
            GmUVCvTrp31F = false;
        }

        if ( (GmUVCvVS3 >= GmUVCvVSThr3)  )
        {
            GmCvPh3Low     = true;
            GmUVCvVS3 = GmUVCvVSThr3;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmUVCvTrp3N);
                    GmUVCvTrp3NF = true;
                    GmUVCvTrpF   = true;
                }
                else
                {
                    PUSH_DIAG(GmUVCvTrp31);
                    GmUVCvTrp31F = true;
                    GmUVCvTrpF   = true;
                }
            }
        }


        //Check the "Any" case
        if ( GmVltType == ALL)
        {
            if (GmCvPh1Low && GmCvPh2Low && GmCvPh3Low)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmUVCvTrpLN);
                    GmUVCvTrpLNF  = true;
                    GmUVCvTrpF    = true;
                }
                else
                {
                    PUSH_DIAG(GmUVCvTrpLL);
                    GmUVCvTrpLLF  = true;
                    GmUVCvTrpF    = true;
                }
            }

        }
    }
    else //function disabled
    {
        GmUVCvVMgFl1 = GmV1MagPu;
        GmUVCVSDyn1  = 0.0;
        GmUVCvVS1  = 0.0;
        GmUVCvVMgFl2 = GmV2MagPu;
        GmUVCVSDyn2  = 0.0;
        GmUVCvVS2  = 0.0;
        GmUVCvVMgFl3 = GmV3MagPu;
        GmUVCVSDyn3  = 0.0;
        GmUVCvVS3  = 0.0;
    }

    if (GmUVCvVS1 + GmUVCvVS2 + GmUVCvVS3 == 0.0F)
    {
        GmUVCvTrpF = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmUVCvTrpInh))
    {

        GmUVCvTrpLNF  = false;
        GmUVCvTrpLLF  = false;

        GmUVCvTrp1NF = false;
        GmUVCvTrp2NF = false;
        GmUVCvTrp3NF = false;

        GmUVCvTrp12F = false;
        GmUVCvTrp23F = false;
        GmUVCvTrp31F = false;

        GmUVCvTrpF   = false;

        GmUVCvVS1  = 0.0;
        GmUVCvVS2  = 0.0;
        GmUVCvVS3  = 0.0;
    }

    GmUVCvTrpAct = GmCvPh1Low || GmCvPh2Low || GmCvPh3Low;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   This is function repsonsible for implementing grid monitor Over-voltgage
//   protection for alarm only. There are eight separate alarms.  At most,
//   only three of these can occur for a given configuration as shown below.
//
//   Fault vs. Configuration:
//
//            All and Line to Neutral:
//                Flt.GmOVAlmLN
//
//            All and Line to Line:
//                Flt.GmOVAlmLL
//
//            Any and Line to Neutral:
//                Flt.GmOVAlm1N
//                Flt.GmOVAlm2N
//                Flt.GmOVAlm3N
//
//            Any and Line to Line:
//                Flt.GmOVAlm12
//                Flt.GmOVAlm23
//                Flt.GmOVAlm31
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorOverVoltageAlarm(void)
{

    unsigned GmOVAlmFlgTp;

    GmOVAlmFlgTp = false;

    //Phase 1
    if ( GmV1MagPu > PARM(GmOVAlmThr))
    {
        GmOVAlmTmr1  = GmOVAlmTmr1 + DelTm3;
        GmOVAlmTmrO1 = 0.0;
        if ( GmOVAlmTmr1 >= PARM(GmOVAlmTm))
        {
            GmOVAlmTmr1 = PARM(GmOVAlmTm);
            GmOVAlmFlg1 = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    GmOVAlm1NF   = true;
                    GmOVAlmFlgTp = true;
                }
                else
                {
                    GmOVAlm12F   = true;
                    GmOVAlmFlgTp = true;
                }
            }
        }
    }
    else
    {
        GmOVAlmTmr1 = GmOVAlmTmr1 - DelTm3;
        if ( GmOVAlmTmr1 <= 0.0)
        {
            GmOVAlmTmr1 = 0.0;
        }
        if ( GmV1MagPu < PARM(GmOVAlmOff))
        {
            GmOVAlmTmrO1 = GmOVAlmTmrO1 + DelTm3;
            if ( GmOVAlmTmrO1 > PARM(GmOVAlmOffTm))
            {
                GmOVAlmTmrO1 = PARM(GmOVAlmOffTm);

                GmOVAlmFlg1 = false;
                GmOVAlm1NF  = false;
                GmOVAlm12F  = false;
            }
        }
    }


    //Phase 2
    if ( GmV2MagPu > PARM(GmOVAlmThr))
    {
        GmOVAlmTmr2  = GmOVAlmTmr2 + DelTm3;
        GmOVAlmTmrO2 = 0.0;
        if ( GmOVAlmTmr2 >= PARM(GmOVAlmTm))
        {
            GmOVAlmTmr2 = PARM(GmOVAlmTm);
            GmOVAlmFlg2 = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    GmOVAlm2NF   = true;
                    GmOVAlmFlgTp = true;
                }
                else
                {
                    GmOVAlm23F   = true;
                    GmOVAlmFlgTp = true;
                }
            }
        }
    }
    else
    {
        GmOVAlmTmr2 = GmOVAlmTmr2 - DelTm3;
        if ( GmOVAlmTmr2 <= 0.0)
        {
            GmOVAlmTmr2 = 0.0;
        }
        if ( GmV2MagPu < PARM(GmOVAlmOff))
        {
            GmOVAlmTmrO2 = GmOVAlmTmrO2 + DelTm3;
            if ( GmOVAlmTmrO2 > PARM(GmOVAlmOffTm))
            {
                GmOVAlmTmrO2 = PARM(GmOVAlmOffTm);

                GmOVAlmFlg2 = false;
                GmOVAlm2NF  = false;
                GmOVAlm23F  = false;
            }
        }
    }


    //Phase 3
    if ( GmV3MagPu > PARM(GmOVAlmThr))
    {
        GmOVAlmTmr3 = GmOVAlmTmr3 + DelTm3;
        GmOVAlmTmrO3 = 0.0;
        if ( GmOVAlmTmr3 >= PARM(GmOVAlmTm))
        {
            GmOVAlmTmr3 = PARM(GmOVAlmTm);
            GmOVAlmFlg3 = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    GmOVAlm3NF   = true;
                    GmOVAlmFlgTp = true;
                }
                else
                {
                    GmOVAlm31F   = true;
                    GmOVAlmFlgTp = true;
                }
            }
        }
    }
    else
    {
        GmOVAlmTmr3 = GmOVAlmTmr3 - DelTm3;
        if ( GmOVAlmTmr3 <= 0.0)
        {
            GmOVAlmTmr3 = 0.0;
        }
        if ( GmV3MagPu < PARM(GmOVAlmOff))
        {
            GmOVAlmTmrO3 = GmOVAlmTmrO3 + DelTm3;
            if ( GmOVAlmTmrO3 > PARM(GmOVAlmOffTm))
            {
                GmOVAlmTmrO3 = PARM(GmOVAlmOffTm);

                GmOVAlmFlg3 = false;
                GmOVAlm3NF  = false;
                GmOVAlm31F  = false;
            }
        }
    }


    //Check the "Any" case
    if ( GmVltType == ALL)
    {
        if (GmOVAlmFlg1 && GmOVAlmFlg2 && GmOVAlmFlg3)
        {
            if ( GmVltSource == LINE_TO_NEUT)
            {
                PUSH_DIAG(GmOVAlmLN);
                GmOVAlmLNF   = true;
                GmOVAlmFlgTp = true;
            }
            else
            {
                PUSH_DIAG(GmOVAlmLL);
                GmOVAlmLLF   = true;
                GmOVAlmFlgTp = true;
            }
        }
        else
        {
            GmOVAlmLNF  = false;
            GmOVAlmLLF  = false;
        }

        GmOVAlm1NF  = false;
        GmOVAlm12F  = false;
        GmOVAlm2NF  = false;
        GmOVAlm23F  = false;
        GmOVAlm3NF  = false;
        GmOVAlm31F  = false;

    }
    else
    {
        GmOVAlmLNF  = false;
        GmOVAlmLLF  = false;
    }


    //Clear alarms and flags based on inactive type
    if ( GmVltSource == LINE_TO_NEUT)
    {
        GmOVAlm12F  = false;
        GmOVAlm23F  = false;
        GmOVAlm31F  = false;
        GmOVAlmLLF  = false;
    }
    else
    {
        GmOVAlm1NF  = false;
        GmOVAlm2NF  = false;
        GmOVAlm3NF  = false;
        GmOVAlmLNF  = false;

        // GmOVAlmTmr1 = 0.0;    // looks like bug in ESS code
        // GmOVAlmTmr2 = 0.0;    // comment out - need to confirm
        // GmOVAlmTmr3 = 0.0;
    }


    GmOVAlmFlg = GmOVAlmFlgTp;

    if (GmOVAlm1NF) PUSH_DIAG(GmOVAlm1N);
    if (GmOVAlm12F) PUSH_DIAG(GmOVAlm12);
    if (GmOVAlm2NF) PUSH_DIAG(GmOVAlm2N);
    if (GmOVAlm23F) PUSH_DIAG(GmOVAlm23);
    if (GmOVAlm3NF) PUSH_DIAG(GmOVAlm3N);
    if (GmOVAlm31F) PUSH_DIAG(GmOVAlm31);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   This function is repsonsible for implementing grid monitor Over-voltgage
//   protection for level #1 trip faults.  There are eight separate trip
//   faults for this level. At most, only three of these can occur for a
//   given configuration as shown below.
//
//   Fault vs. Configuration:
//
//            All and Line to Neutral:
//                Flt.GmOV1TrpLN
//
//            All and Line to Line:
//                Flt.GmOV1TrpLL
//
//            Any and Line to Neutral:
//                Flt.GmOV1Trp1N
//                Flt.GmOV1Trp2N
//                Flt.GmOV1Trp3N
//
//            Any and Line to Line:
//                Flt.GmOV1Trp12
//                Flt.GmOV1Trp23
//                Flt.GmOV1Trp31
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorOverVoltageLevel1(void)
{

    unsigned Ph1Trip, Ph2Trip, Ph3Trip;
    float RstTrpTm;

    //Init internal tirp flags to 'false'
    Ph1Trip = Ph2Trip = Ph3Trip = false;


    //Phase 1
    if ( GmV1MagPu > GmOV1TrpThrA)
    {
        GmOV1TrpTmr1 = GmOV1TrpTmr1 + DelTm3;
        if ( GmOV1TrpTmr1 >= PARM(GmOV1TrpTm))
        {
            GmOV1TrpTmr1 = PARM(GmOV1TrpTm);
            Ph1Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV1Trp1N);
                    GmOV1Trp1NF = true;
                    GmOV1TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV1Trp12);
                    GmOV1Trp12F = true;
                    GmOV1TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV1TrpTmr1 = GmOV1TrpTmr1 - DelTm3;
        if ( GmOV1TrpTmr1 <= 0.0)
        {
            GmOV1TrpTmr1 = 0.0;
            GmOV1Trp1NF = false;
            GmOV1Trp12F = false;
        }
    }



    //Phase 2
    if ( GmV2MagPu > GmOV1TrpThrA)
    {
        GmOV1TrpTmr2 = GmOV1TrpTmr2 + DelTm3;
        if ( GmOV1TrpTmr2 >= PARM(GmOV1TrpTm))
        {
            GmOV1TrpTmr2 = PARM(GmOV1TrpTm);
            Ph2Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV1Trp2N);
                    GmOV1Trp2NF = true;
                    GmOV1TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV1Trp23);
                    GmOV1Trp23F = true;
                    GmOV1TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV1TrpTmr2 = GmOV1TrpTmr2 - DelTm3;
        if ( GmOV1TrpTmr2 <= 0.0)
        {
            GmOV1TrpTmr2 = 0.0;
            GmOV1Trp2NF = false;
            GmOV1Trp23F = false;
        }
    }



    //Phase 3
    if ( GmV3MagPu > GmOV1TrpThrA)
    {
        GmOV1TrpTmr3 = GmOV1TrpTmr3 + DelTm3;
        if ( GmOV1TrpTmr3 >= PARM(GmOV1TrpTm))
        {
            GmOV1TrpTmr3 = PARM(GmOV1TrpTm);
            Ph3Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV1Trp3N);
                    GmOV1Trp3NF = true;
                    GmOV1TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV1Trp31);
                    GmOV1Trp31F = true;
                    GmOV1TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV1TrpTmr3 = GmOV1TrpTmr3 - DelTm3;
        if ( GmOV1TrpTmr3 <= 0.0)
        {
            GmOV1TrpTmr3 = 0.0;
            GmOV1Trp3NF = false;
            GmOV1Trp31F = false;
        }
    }




    //Check the "Any" case
    if ( GmVltType == ALL)
    {
        if (Ph1Trip && Ph2Trip && Ph3Trip)
        {
            if ( GmVltSource == LINE_TO_NEUT)
            {
                PUSH_DIAG(GmOV1TrpLN);
                GmOV1TrpLNF  = true;
                GmOV1TrpFlg = true;
            }
            else
            {
                PUSH_DIAG(GmOV1TrpLL);
                GmOV1TrpLLF  = true;
                GmOV1TrpFlg = true;
            }
        }

    }


    if (GmOV1TrpTmr1 + GmOV1TrpTmr2 + GmOV1TrpTmr3 == 0.0F)
    {
        GmOV1TrpLNF = false;
        GmOV1TrpLLF = false;
        GmOV1TrpFlg = false;
    }


    //if faults are being reset
    if (Stopped && DiagResetCmdT3)
    {
        GmOV1TrpLNF =false;
        GmOV1TrpLLF =false;

        GmOV1Trp1NF =false;
        GmOV1Trp2NF =false;
        GmOV1Trp3NF =false;

        GmOV1Trp12F =false;
        GmOV1Trp23F =false;
        GmOV1Trp31F =false;

        GmOV1TrpFlg = false;

        GmOV1TrpTmr1 = 0.0;
        GmOV1TrpTmr2 = 0.0;
        GmOV1TrpTmr3 = 0.0;

    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmOV1TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmOV1TrpTmr1 < RstTrpTm)
            {
                GmOV1Trp1NF = false;
                GmOV1Trp12F = false;
            }
            if (GmOV1TrpTmr2 < RstTrpTm)
            {
                GmOV1Trp2NF = false;
                GmOV1Trp23F = false;
            }
            if (GmOV1TrpTmr3 < RstTrpTm)
            {
                GmOV1Trp3NF = false;
                GmOV1Trp31F = false;
            }
            if ( GmVltType == ANY)
            {
                if ((GmOV1TrpTmr1 < RstTrpTm) && (GmOV1TrpTmr2 < RstTrpTm) && (GmOV1TrpTmr3 < RstTrpTm))
                {
                    GmOV1TrpFlg = false;
                }
            }
            else
            {
                if ((GmOV1TrpTmr1 < RstTrpTm) || (GmOV1TrpTmr2 < RstTrpTm) || (GmOV1TrpTmr3 < RstTrpTm))
                {
                    GmOV1TrpFlg = false;
                }
            }
        }
    }

    GmOV1TrpAct = Ph1Trip || Ph2Trip || Ph3Trip;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   This is function repsonsible for implementing grid monitor Over-voltgage
//   protection for level #2 trip faults.  There are eight separate trip
//   faults for this level. At most, only three of these can occur for a
//   given configuration as shown below.
//
//   Fault vs. Configuration:
//
//            All and Line to Neutral:
//                Flt.GmOV2TrpLN
//
//            All and Line to Line:
//                Flt.GmOV2TrpLL
//
//            Any and Line to Neutral:
//                Flt.GmOV2Trp1N
//                Flt.GmOV2Trp2N
//                Flt.GmOV2Trp3N
//
//            Any and Line to Line:
//                Flt.GmOV2Trp12
//                Flt.GmOV2Trp23
//                Flt.GmOV2Trp31
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorOverVoltageLevel2(void)
{

    unsigned Ph1Trip, Ph2Trip, Ph3Trip;
    float RstTrpTm;

    //Init internal tirp flags to 'false'
    Ph1Trip = Ph2Trip = Ph3Trip = false;


    //Phase 1
    if ( GmV1MagPu > GmOV2TrpThrA)
    {
        GmOV2TrpTmr1 = GmOV2TrpTmr1 + DelTm3;
        if ( GmOV2TrpTmr1 >= PARM(GmOV2TrpTm))
        {
            GmOV2TrpTmr1 = PARM(GmOV2TrpTm);
            Ph1Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV2Trp1N);
                    GmOV2Trp1NF = true;
                    GmOV2TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV2Trp12);
                    GmOV2Trp12F = true;
                    GmOV2TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV2TrpTmr1 = GmOV2TrpTmr1 - DelTm3;
        if ( GmOV2TrpTmr1 <= 0.0)
        {
            GmOV2TrpTmr1 = 0.0;
            GmOV2Trp1NF = false;
            GmOV2Trp12F = false;
        }
    }



    //Phase 2
    if ( GmV2MagPu > GmOV2TrpThrA)
    {
        GmOV2TrpTmr2 = GmOV2TrpTmr2 + DelTm3;
        if ( GmOV2TrpTmr2 >= PARM(GmOV2TrpTm))
        {
            GmOV2TrpTmr2 = PARM(GmOV2TrpTm);
            Ph2Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV2Trp2N);
                    GmOV2Trp2NF = true;
                    GmOV2TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV2Trp23);
                    GmOV2Trp23F = true;
                    GmOV2TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV2TrpTmr2 = GmOV2TrpTmr2 - DelTm3;
        if ( GmOV2TrpTmr2 <= 0.0)
        {
            GmOV2TrpTmr2 = 0.0;
            GmOV2Trp2NF = false;
            GmOV2Trp23F = false;
        }
    }



    //Phase 3
    if ( GmV3MagPu > GmOV2TrpThrA)
    {
        GmOV2TrpTmr3 = GmOV2TrpTmr3 + DelTm3;
        if ( GmOV2TrpTmr3 >= PARM(GmOV2TrpTm))
        {
            GmOV2TrpTmr3 = PARM(GmOV2TrpTm);
            Ph3Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV2Trp3N);
                    GmOV2Trp3NF = true;
                    GmOV2TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV2Trp31);
                    GmOV2Trp31F = true;
                    GmOV2TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV2TrpTmr3 = GmOV2TrpTmr3 - DelTm3;
        if ( GmOV2TrpTmr3 <= 0.0)
        {
            GmOV2TrpTmr3 = 0.0;
            GmOV2Trp3NF = false;
            GmOV2Trp31F = false;
        }
    }




    //Check the "Any" case
    if ( GmVltType == ALL)
    {
        if (Ph1Trip && Ph2Trip && Ph3Trip)
        {
            if ( GmVltSource == LINE_TO_NEUT)
            {
                PUSH_DIAG(GmOV2TrpLN);
                GmOV2TrpLNF  = true;
                GmOV2TrpFlg = true;
            }
            else
            {
                PUSH_DIAG(GmOV2TrpLL);
                GmOV2TrpLLF  = true;
                GmOV2TrpFlg = true;
            }
        }

    }


    if (GmOV2TrpTmr1 + GmOV2TrpTmr2 + GmOV2TrpTmr3 == 0.0F)
    {
        GmOV2TrpLNF = false;
        GmOV2TrpLLF = false;
        GmOV2TrpFlg = false;
    }

    //if faults are being reset
    if (Stopped && DiagResetCmdT3)
    {
        GmOV2TrpLNF =false;
        GmOV2TrpLLF =false;

        GmOV2Trp1NF =false;
        GmOV2Trp2NF =false;
        GmOV2Trp3NF =false;

        GmOV2Trp12F =false;
        GmOV2Trp23F =false;
        GmOV2Trp31F =false;

        GmOV2TrpFlg = false;

        GmOV2TrpTmr1 = 0.0;
        GmOV2TrpTmr2 = 0.0;
        GmOV2TrpTmr3 = 0.0;

    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmOV2TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmOV2TrpTmr1 < RstTrpTm)
            {
                GmOV2Trp1NF = false;
                GmOV2Trp12F = false;
            }
            if (GmOV2TrpTmr2 < RstTrpTm)
            {
                GmOV2Trp2NF = false;
                GmOV2Trp23F = false;
            }
            if (GmOV2TrpTmr3 < RstTrpTm)
            {
                GmOV2Trp3NF = false;
                GmOV2Trp31F = false;
            }
            if ( GmVltType == ANY)
            {
                if ((GmOV2TrpTmr1 < RstTrpTm) && (GmOV2TrpTmr2 < RstTrpTm) && (GmOV2TrpTmr3 < RstTrpTm))
                {
                    GmOV2TrpFlg = false;
                }
            }
            else
            {
                if ((GmOV2TrpTmr1 < RstTrpTm) || (GmOV2TrpTmr2 < RstTrpTm) || (GmOV2TrpTmr3 < RstTrpTm))
                {
                    GmOV2TrpFlg = false;
                }
            }
        }
    }

    GmOV2TrpAct = Ph1Trip || Ph2Trip || Ph3Trip;

    return;
}


/*=============================================================================
 * Function: GridMonitorOvervoltageLevel3
 * Execution Level: Task 3
 * Execution Time: Untimed
 *
 * Global Inputs:
 *                 P.GmVltPrtType -- All or Any
 *                 P.GmVltPrtSrc  -- Line to Neutral or Line to line
 *
 *                 P.GmOV2TrpThr
 *                 P.GmOV2TrpTm
 *
 *                 V.DelTm3
 *
 *
 * Global Outputs:
 *                Flt.GmOV3TrpLN
 *                Flt.GmOV3TrpLL
 *                Flt.GmOV3Trp1N
 *                Flt.GmOV3Trp2N
 *                Flt.GmOV3Trp3N
 *                Flt.GmOV3Trp12
 *                Flt.GmOV3Trp23
 *                Flt.GmOV3Trp31
 *
 *                V.GmOV3TrpLNF
 *                V.GmOV3TrpLLF
 *                V.GmOV3Trp1NF
 *                V.GmOV3Trp2NF
 *                V.GmOV3Trp3NF
 *                V.GmOV3Trp12F
 *                V.GmOV3Trp23F
 *                V.GmOV3Trp31F
 *
 *
 * Locals:
 *                V.GmOV3TrpTmr1
 *                V.GmOV3TrpTmr2
 *                V.GmOV3TrpTmr3
 *
 *                V.GmOV3TrpFlg1
 *                V.GmOV3TrpFlg2
 *                V.GmOV3TrpFlg3
 *
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Related Visos:  GmProt_033
 *
 * Abstract:
 *    This is function repsonsible for implementing grid monitor Over-voltgage protection
 * for level #3 trip faults.  There are eight separate trip faults for this level.
 * At most, only three of these can occur for a given configuration as shown below.
 *
 *   Fault vs. Configuration:
 *
 *            All and Line to Neutral:
 *                Flt.GmOV3TrpLN
 *
 *            All and Line to Line:
 *                Flt.GmOV3TrpLL
 *
 *            Any and Line to Neutral:
 *                Flt.GmOV3Trp1N
 *                Flt.GmOV3Trp2N
 *                Flt.GmOV3Trp3N
 *
 *            Any and Line to Line:
 *                Flt.GmOV3Trp12
 *                Flt.GmOV3Trp23
 *                Flt.GmOV3Trp31
 *
 *
 *
 *===========================================================================*/
void GridMonitorOverVoltageLevel3(void)
{


    unsigned Ph1Trip, Ph2Trip, Ph3Trip;
    float RstTrpTm;

    //Init internal tirp flags to 'false'
    Ph1Trip = Ph2Trip = Ph3Trip = false;


    //Phase 1
    if ( GmV1MagPu > GmOV3TrpThrA)
    {
        GmOV3TrpTmr1 = GmOV3TrpTmr1 + DelTm3;
        if ( GmOV3TrpTmr1 >= PARM(GmOV3TrpTm))
        {
            GmOV3TrpTmr1 = PARM(GmOV3TrpTm);
            Ph1Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV3Trp1N);
                    GmOV3Trp1NF = true;
                    GmOV3TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV3Trp12);
                    GmOV3Trp12F = true;
                    GmOV3TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV3TrpTmr1 = GmOV3TrpTmr1 - DelTm3;
        if ( GmOV3TrpTmr1 <= 0.0)
        {
            GmOV3TrpTmr1 = 0.0;
            GmOV3Trp1NF = false;
            GmOV3Trp12F = false;
        }
    }



    //Phase 2
    if ( GmV2MagPu > GmOV3TrpThrA)
    {
        GmOV3TrpTmr2 = GmOV3TrpTmr2 + DelTm3;
        if ( GmOV3TrpTmr2 >= PARM(GmOV3TrpTm))
        {
            GmOV3TrpTmr2 = PARM(GmOV3TrpTm);
            Ph2Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV3Trp2N);
                    GmOV3Trp2NF = true;
                    GmOV3TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV3Trp23);
                    GmOV3Trp23F = true;
                    GmOV3TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV3TrpTmr2 = GmOV3TrpTmr2 - DelTm3;
        if ( GmOV3TrpTmr2 <= 0.0)
        {
            GmOV3TrpTmr2 = 0.0;
            GmOV3Trp2NF = false;
            GmOV3Trp23F = false;
        }
    }



    //Phase 3
    if ( GmV3MagPu > GmOV3TrpThrA)
    {
        GmOV3TrpTmr3 = GmOV3TrpTmr3 + DelTm3;
        if ( GmOV3TrpTmr3 >= PARM(GmOV3TrpTm))
        {
            GmOV3TrpTmr3 = PARM(GmOV3TrpTm);
            Ph3Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV3Trp3N);
                    GmOV3Trp3NF = true;
                    GmOV3TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV3Trp31);
                    GmOV3Trp31F = true;
                    GmOV3TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV3TrpTmr3 = GmOV3TrpTmr3 - DelTm3;
        if ( GmOV3TrpTmr3 <= 0.0)
        {
            GmOV3TrpTmr3 = 0.0;
            GmOV3Trp3NF = false;
            GmOV3Trp31F = false;
        }
    }




    //Check the "Any" case
    if ( GmVltType == ALL)
    {
        if (Ph1Trip && Ph2Trip && Ph3Trip)
        {
            if ( GmVltSource == LINE_TO_NEUT)
            {
                PUSH_DIAG(GmOV3TrpLN);
                GmOV3TrpLNF  = true;
                GmOV3TrpFlg = true;
            }
            else
            {
                PUSH_DIAG(GmOV3TrpLL);
                GmOV3TrpLLF  = true;
                GmOV3TrpFlg = true;
            }
        }

    }


    if (GmOV3TrpTmr1 + GmOV3TrpTmr2 + GmOV3TrpTmr3 == 0.0F)
    {
        GmOV3TrpLNF = false;
        GmOV3TrpLLF = false;
        GmOV3TrpFlg = false;
    }


    //if faults are being reset
    if (Stopped && DiagResetCmdT3)
    {
        GmOV3TrpLNF =false;
        GmOV3TrpLLF =false;

        GmOV3Trp1NF =false;
        GmOV3Trp2NF =false;
        GmOV3Trp3NF =false;

        GmOV3Trp12F =false;
        GmOV3Trp23F =false;
        GmOV3Trp31F =false;

        GmOV3TrpFlg = false;

        GmOV3TrpTmr1 = 0.0;
        GmOV3TrpTmr2 = 0.0;
        GmOV3TrpTmr3 = 0.0;

    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmOV3TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmOV3TrpTmr1 < RstTrpTm)
            {
                GmOV3Trp1NF = false;
                GmOV3Trp12F = false;
            }
            if (GmOV3TrpTmr2 < RstTrpTm)
            {
                GmOV3Trp2NF = false;
                GmOV3Trp23F = false;
            }
            if (GmOV3TrpTmr3 < RstTrpTm)
            {
                GmOV3Trp3NF = false;
                GmOV3Trp31F = false;
            }
            if ( GmVltType == ANY)
            {
                if ((GmOV3TrpTmr1 < RstTrpTm) && (GmOV3TrpTmr2 < RstTrpTm) && (GmOV3TrpTmr3 < RstTrpTm))
                {
                    GmOV3TrpFlg = false;
                }
            }
            else
            {
                if ((GmOV3TrpTmr1 < RstTrpTm) || (GmOV3TrpTmr2 < RstTrpTm) || (GmOV3TrpTmr3 < RstTrpTm))
                {
                    GmOV3TrpFlg = false;
                }
            }
        }
    }

    GmOV3TrpAct = Ph1Trip || Ph2Trip || Ph3Trip;

    return;
}


/*=============================================================================
 * Function: GridMonitorOvervoltageLevel4
 * Execution Level: Task 3
 * Execution Time: Untimed
 *
 * Global Inputs:
 *                 P.GmVltPrtType -- All or Any
 *                 P.GmVltPrtSrc  -- Line to Neutral or Line to line
 *
 *                 P.GmOV2TrpThr
 *                 P.GmOV2TrpTm
 *
 *                 V.DelTm3
 *
 *
 * Global Outputs:
 *                Flt.GmOV4TrpLN
 *                Flt.GmOV4TrpLL
 *                Flt.GmOV4Trp1N
 *                Flt.GmOV4Trp2N
 *                Flt.GmOV4Trp3N
 *                Flt.GmOV4Trp12
 *                Flt.GmOV4Trp23
 *                Flt.GmOV4Trp31
 *
 *                V.GmOV4TrpLNF
 *                V.GmOV4TrpLLF
 *                V.GmOV4Trp1NF
 *                V.GmOV4Trp2NF
 *                V.GmOV4Trp3NF
 *                V.GmOV4Trp12F
 *                V.GmOV4Trp23F
 *                V.GmOV4Trp31F
 *
 *
 * Locals:
 *                V.GmOV4TrpTmr1
 *                V.GmOV4TrpTmr2
 *                V.GmOV4TrpTmr3
 *
 *                V.GmOV4TrpFlg1
 *                V.GmOV4TrpFlg2
 *                V.GmOV4TrpFlg3
 *
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Related Visos:  GmProt_033
 *
 * Abstract:
 *    This is function repsonsible for implementing grid monitor Over-voltgage protection
 * for level #3 trip faults.  There are eight separate trip faults for this level.
 * At most, only three of these can occur for a given configuration as shown below.
 *
 *   Fault vs. Configuration:
 *
 *            All and Line to Neutral:
 *                Flt.GmOV4TrpLN
 *
 *            All and Line to Line:
 *                Flt.GmOV4TrpLL
 *
 *            Any and Line to Neutral:
 *                Flt.GmOV4Trp1N
 *                Flt.GmOV4Trp2N
 *                Flt.GmOV4Trp3N
 *
 *            Any and Line to Line:
 *                Flt.GmOV4Trp12
 *                Flt.GmOV4Trp23
 *                Flt.GmOV4Trp31
 *
 *
 *
 *===========================================================================*/
void GridMonitorOverVoltageLevel4(void)
{

    unsigned Ph1Trip, Ph2Trip, Ph3Trip;
    float RstTrpTm;

    //Init internal tirp flags to 'false'
    Ph1Trip = Ph2Trip = Ph3Trip = false;


    //Phase 1
    if ( GmV1MagPu > GmOV4TrpThrA)
    {
        GmOV4TrpTmr1 = GmOV4TrpTmr1 + DelTm3;
        if ( GmOV4TrpTmr1 >= PARM(GmOV4TrpTm))
        {
            GmOV4TrpTmr1 = PARM(GmOV4TrpTm);
            Ph1Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV4Trp1N);
                    GmOV4Trp1NF = true;
                    GmOV4TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV4Trp12);
                    GmOV4Trp12F = true;
                    GmOV4TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV4TrpTmr1 = GmOV4TrpTmr1 - DelTm3;
        if ( GmOV4TrpTmr1 <= 0.0)
        {
            GmOV4TrpTmr1 = 0.0;
            GmOV4Trp1NF = false;
            GmOV4Trp12F = false;
        }
    }



    //Phase 2
    if ( GmV2MagPu > GmOV4TrpThrA)
    {
        GmOV4TrpTmr2 = GmOV4TrpTmr2 + DelTm3;
        if ( GmOV4TrpTmr2 >= PARM(GmOV4TrpTm))
        {
            GmOV4TrpTmr2 = PARM(GmOV4TrpTm);
            Ph2Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV4Trp2N);
                    GmOV4Trp2NF = true;
                    GmOV4TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV4Trp23);
                    GmOV4Trp23F = true;
                    GmOV4TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV4TrpTmr2 = GmOV4TrpTmr2 - DelTm3;
        if ( GmOV4TrpTmr2 <= 0.0)
        {
            GmOV4TrpTmr2 = 0.0;
            GmOV4Trp2NF = false;
            GmOV4Trp23F = false;
        }
    }



    //Phase 3
    if ( GmV3MagPu > GmOV4TrpThrA)
    {
        GmOV4TrpTmr3 = GmOV4TrpTmr3 + DelTm3;
        if ( GmOV4TrpTmr3 >= PARM(GmOV4TrpTm))
        {
            GmOV4TrpTmr3 = PARM(GmOV4TrpTm);
            Ph3Trip     = true;
            if ( GmVltType == ANY)
            {
                if ( GmVltSource == LINE_TO_NEUT)
                {
                    PUSH_DIAG(GmOV4Trp3N);
                    GmOV4Trp3NF = true;
                    GmOV4TrpFlg = true;
                }
                else
                {
                    PUSH_DIAG(GmOV4Trp31);
                    GmOV4Trp31F = true;
                    GmOV4TrpFlg = true;
                }
            }
        }
    }
    else
    {
        GmOV4TrpTmr3 = GmOV4TrpTmr3 - DelTm3;
        if ( GmOV4TrpTmr3 <= 0.0)
        {
            GmOV4TrpTmr3 = 0.0;
            GmOV4Trp3NF = false;
            GmOV4Trp31F = false;
        }
    }




    //Check the "Any" case
    if ( GmVltType == ALL)
    {
        if (Ph1Trip && Ph2Trip && Ph3Trip)
        {
            if ( GmVltSource == LINE_TO_NEUT)
            {
                PUSH_DIAG(GmOV4TrpLN);
                GmOV4TrpLNF  = true;
                GmOV4TrpFlg = true;
            }
            else
            {
                PUSH_DIAG(GmOV4TrpLL);
                GmOV4TrpLLF  = true;
                GmOV4TrpFlg = true;
            }
        }

    }


    if (GmOV4TrpTmr1 + GmOV4TrpTmr2 + GmOV4TrpTmr3 == 0.0F)
    {
        GmOV4TrpLNF = false;
        GmOV4TrpLLF = false;
        GmOV4TrpFlg = false;
    }


    //if faults are being reset
    if (Stopped && DiagResetCmdT3)
    {
        GmOV4TrpLNF =false;
        GmOV4TrpLLF =false;

        GmOV4Trp1NF =false;
        GmOV4Trp2NF =false;
        GmOV4Trp3NF =false;

        GmOV4Trp12F =false;
        GmOV4Trp23F =false;
        GmOV4Trp31F =false;

        GmOV4TrpFlg = false;

        GmOV4TrpTmr1 = 0.0;
        GmOV4TrpTmr2 = 0.0;
        GmOV4TrpTmr3 = 0.0;

    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmOV4TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmOV4TrpTmr1 < RstTrpTm)
            {
                GmOV4Trp1NF = false;
                GmOV4Trp12F = false;
            }
            if (GmOV4TrpTmr2 < RstTrpTm)
            {
                GmOV4Trp2NF = false;
                GmOV4Trp23F = false;
            }
            if (GmOV4TrpTmr3 < RstTrpTm)
            {
                GmOV4Trp3NF = false;
                GmOV4Trp31F = false;
            }
            if ( GmVltType == ANY)
            {
                if ((GmOV4TrpTmr1 < RstTrpTm) && (GmOV4TrpTmr2 < RstTrpTm) && (GmOV4TrpTmr3 < RstTrpTm))
                {
                    GmOV4TrpFlg = false;
                }
            }
            else
            {
                if ((GmOV4TrpTmr1 < RstTrpTm) || (GmOV4TrpTmr2 < RstTrpTm) || (GmOV4TrpTmr3 < RstTrpTm))
                {
                    GmOV4TrpFlg = false;
                }
            }
        }
    }

    GmOV4TrpAct = Ph1Trip || Ph2Trip || Ph3Trip;

    return;
}


/*=============================================================================
 * Function: GridMonitorUnderFrequency
 * Execution Level: Task 3
 * Execution Time: Untimed
 *
 * Global Inputs:
 *                 P.GmUFAlmThr
 *                 P.GmUFAlmTm
 *                 P.GmUF1TrpThr
 *                 P.GmUF1TrpTm
 *
 *                 P.GmUF2TrpThr
 *                 P.GmUF2TrpTm
 *
 *                 P.GmUF3TrpThr
 *                 P.GmUF3TrpTm
 *
 *                 V.GmFreqPu
 *                 V.DelTm3
 *
 *
 * Global Outputs:
 *                Flt.GmUFAlm
 *                Flt.GmUF1Trp
 *                Flt.GmUF2Trp
 *                Flt.GmUF3Trp
 *
 *
 *                V.GmUFAlmFlg
 *                V.GmUF1TrpF
 *                V.GmUF2TrpF
 *                V.GmUF3TrpF

 *
 *
 * Locals:
 *                V.GmUFAlmTmr
 *                V.GmUF1TrpTmr
 *                V.GmUF2TrpTmr
 *                V.GmUF3TrpTmr
 *
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Related Visos:  GmProt_033
 *
 * Abstract:
 *    This function is repsonsible for implementing grid monitor under-frequency protection
 * for all alarms and trips.  There is one alarm and there are three separate trip
 * fault levels.
 *
 *
 *===========================================================================*/
void GridMonitorUnderFrequency(void)
{
    unsigned Uf1Trip, Uf2Trip, Uf3Trip;
    float RstTrpTm;

    //Init internal trip flags to 'false'
    Uf1Trip = Uf2Trip = Uf3Trip = false;

    /////////////////////////////////
    // Alarm
    /////////////////////////////////

    if ( GmFreqPu < PARM(GmUFAlmThr))
    {
        GmUFAlmTmr = GmUFAlmTmr + DelTm3;

        if ( GmUFAlmTmr >= PARM(GmUFAlmTm))
        {
            GmUFAlmTmr = PARM(GmUFAlmTm);
            PUSH_DIAG(GmUFAlm);
            GmUFAlmF = true;
        }
    }
    else
    {
        GmUFAlmTmr = GmUFAlmTmr - DelTm3;
        if ( GmUFAlmTmr <= 0.0)
        {
            GmUFAlmTmr = 0.0;
            // Pop3Flt(GmUFAlm);
            GmUFAlmF = false;

        }
    }

    //if faults are being reset
    if (Stopped && DiagResetCmdT3)
    {
        GmUFAlmF    = false;
        GmUFAlmTmr  = 0.0;
    }



    /////////////////////////////////
    //Level 1 Trip
    /////////////////////////////////

    if ( !PARM(GmUF1TrpInh))
    {
        if ( GmFreqPu < GmUF1TrpThrA)
        {
            GmUF1TrpTmr = GmUF1TrpTmr + DelTm3;

            if ( GmUF1TrpTmr >= PARM(GmUF1TrpTm))
            {
                GmUF1TrpTmr = PARM(GmUF1TrpTm);
                PUSH_DIAG(GmUF1Trp);
                GmUF1TrpF = true;
                Uf1Trip   = true;
            }
        }
        else
        {
            GmUF1TrpTmr = GmUF1TrpTmr - DelTm3;
            if ( GmUF1TrpTmr <= 0.0)
            {
                GmUF1TrpTmr = 0.0;
                GmUF1TrpF = false;
            }
        }

    }
    else//trip is inhibited
    {
        GmUF1TrpTmr = 0.0;
        GmUF1TrpF   = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmUF1TrpInh))
    {
        GmUF1TrpF   = false;
        GmUF1TrpTmr = 0.0;
    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmUF1TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmUF1TrpTmr < RstTrpTm)
            {
                GmUF1TrpF = false;
            }
        }
    }



    /////////////////////////////////
    //Level 2 Trip
    /////////////////////////////////

    if ( !PARM(GmUF2TrpInh))
    {
        if ( GmFreqPu < GmUF2TrpThrA)
        {
            GmUF2TrpTmr = GmUF2TrpTmr + DelTm3;

            if ( GmUF2TrpTmr >= PARM(GmUF2TrpTm))
            {
                GmUF2TrpTmr = PARM(GmUF2TrpTm);
                PUSH_DIAG(GmUF2Trp);
                GmUF2TrpF = true;
                Uf2Trip   = true;
            }
        }
        else
        {
            GmUF2TrpTmr = GmUF2TrpTmr - DelTm3;
            if ( GmUF2TrpTmr <= 0.0)
            {
                GmUF2TrpTmr = 0.0;
                GmUF2TrpF = false;
            }
        }

    }
    else//trip is inhibited
    {
        GmUF2TrpTmr = 0.0;
        GmUF2TrpF   = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmUF2TrpInh))
    {
        GmUF2TrpF   = false;
        GmUF2TrpTmr = 0.0;
    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmUF2TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmUF2TrpTmr < RstTrpTm)
            {
                GmUF2TrpF = false;
            }
        }
    }


    /////////////////////////////////
    //Level 3 Trip
    /////////////////////////////////

    if ( !PARM(GmUF3TrpInh))
    {
        if ( GmFreqPu < GmUF3TrpThrA)
        {
            GmUF3TrpTmr = GmUF3TrpTmr + DelTm3;

            if ( GmUF3TrpTmr >= PARM(GmUF3TrpTm))
            {
                GmUF3TrpTmr = PARM(GmUF3TrpTm);
                PUSH_DIAG(GmUF3Trp);
                GmUF3TrpF = true;
                Uf3Trip   = true;
            }
        }
        else
        {
            GmUF3TrpTmr = GmUF3TrpTmr - DelTm3;
            if ( GmUF3TrpTmr <= 0.0)
            {
                GmUF3TrpTmr = 0.0;
                GmUF3TrpF = false;
            }
        }

    }
    else//trip is inhibited
    {
        GmUF3TrpTmr = 0.0;
        GmUF3TrpF   = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmUF3TrpInh))
    {
        GmUF3TrpF   = false;
        GmUF3TrpTmr = 0.0;
    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmUF3TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmUF3TrpTmr < RstTrpTm)
            {
                GmUF3TrpF = false;
            }
        }
    }

    GmUF1TrpAct = Uf1Trip;
    GmUF2TrpAct = Uf2Trip;
    GmUF3TrpAct = Uf3Trip;

    return;
}


/*=============================================================================
 * Function: GridMonitorOverFrequency
 * Execution Level: Task 3
 * Execution Time: Untimed
 *
 * Global Inputs:
 *                 P.GmOF1TrpThr
 *                 P.GmOFAlmTm
 *                 P.GmOF1TrpTm
 *
 *                 P.GmOF2TrpThr
 *                 P.GmOF2TrpTm
 *
 *                 P.GmOF3TrpThr
 *                 P.GmOF3TrpTm
 *
 *                 V.GmFreqPu
 *                 V.DelTm3
 *
 *
 * Global Outputs:
 *                Flt.GmOFAlm
 *                Flt.GmOF1Trp
 *                Flt.GmOF2Trp
 *
 *
 *                V.GmOFAlmFlg
 *                V.GmOF1TrpFlg
 *                V.GmOF2TrpFlg

 *
 *
 * Locals:
 *                V.GmOF1TrpTmr
 *                V.GmOF2TrpTmr
 *
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Related Visos:  GmProt_033
 *
 * Abstract:
 *    This function is repsonsible for implementing grid monitor Over-frequency protection
 * for all alarms and trips.  There is one alarm and there are two separate trip
 * fault levels.
 *
 *
 *===========================================================================*/
void GridMonitorOverFrequency(void)
{
    unsigned Of1Trip, Of2Trip;
    float RstTrpTm;

    //Init internal trip flags to 'false'
    Of1Trip = Of2Trip = false;

    /////////////////////////////////
    // Alarm
    /////////////////////////////////

    if ( GmFreqPu > PARM(GmOFAlmThr))
    {
        GmOFAlmTmr = GmOFAlmTmr + DelTm3;

        if ( GmOFAlmTmr >= PARM(GmOFAlmTm))
        {
            GmOFAlmTmr = PARM(GmOFAlmTm);
            PUSH_DIAG(GmOFAlm);
            GmOFAlmF = true;
        }
    }
    else
    {
        GmOFAlmTmr = GmOFAlmTmr - DelTm3;
        if ( GmOFAlmTmr <= 0.0)
        {
            GmOFAlmTmr = 0.0;
            // Pop3Flt(GmOFAlm);
            GmOFAlmF = false;

        }
    }

    //if faults are being reset
    if (Stopped && DiagResetCmdT3)
    {
        GmOFAlmF    = false;
        GmOFAlmTmr  = 0.0;
    }



    /////////////////////////////////
    //Level 1 Trip
    /////////////////////////////////

    if ( !PARM(GmOF1TrpInh))
    {
        if ( GmFreqPu > GmOF1TrpThrA)
        {
            GmOF1TrpTmr = GmOF1TrpTmr + DelTm3;

            if ( GmOF1TrpTmr >= PARM(GmOF1TrpTm))
            {
                GmOF1TrpTmr = PARM(GmOF1TrpTm);
                PUSH_DIAG(GmOF1Trp);
                GmOF1TrpF = true;
                Of1Trip   = true;
            }
        }
        else
        {
            GmOF1TrpTmr = GmOF1TrpTmr - DelTm3;
            if ( GmOF1TrpTmr <= 0.0)
            {
                GmOF1TrpTmr = 0.0;
                GmOF1TrpF = false;
            }
        }

    }
    else//trip is inhibited
    {
        GmOF1TrpTmr = 0.0;
        GmOF1TrpF   = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmOF1TrpInh))
    {
        GmOF1TrpF   = false;
        GmOF1TrpTmr = 0.0;
    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmOF1TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmOF1TrpTmr < RstTrpTm)
            {
                GmOF1TrpF = false;
            }
        }
    }


    /////////////////////////////////
    //Level 2 Trip
    /////////////////////////////////

    if ( !PARM(GmOF2TrpInh))
    {
        if ( GmFreqPu > GmOF2TrpThrA)
        {
            GmOF2TrpTmr = GmOF2TrpTmr + DelTm3;

            if ( GmOF2TrpTmr >= PARM(GmOF2TrpTm))
            {
                GmOF2TrpTmr = PARM(GmOF2TrpTm);
                PUSH_DIAG(GmOF2Trp);
                GmOF2TrpF = true;
                Of2Trip   = true;
            }
        }
        else
        {
            GmOF2TrpTmr = GmOF2TrpTmr - DelTm3;
            if ( GmOF2TrpTmr <= 0.0)
            {
                GmOF2TrpTmr = 0.0;
                GmOF2TrpF = false;
            }
        }

    }
    else//trip is inhibited
    {
        GmOF2TrpTmr = 0.0;
        GmOF2TrpF   = false;
    }

    //if faults are being reset
    if ( (Stopped && DiagResetCmdT3) || PARM(GmOF2TrpInh))
    {
        GmOF2TrpF   = false;
        GmOF2TrpTmr = 0.0;
    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmOF2TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmOF2TrpTmr < RstTrpTm)
            {
                GmOF2TrpF = false;
            }
        }
    }

    GmOF1TrpAct = Of1Trip;
    GmOF2TrpAct = Of2Trip;

    return;
}


/*=============================================================================
 * Function: GridMonitorVoltageAsymmetry
 * Execution Level: Task 3
 * Execution Time: Untimed
 *
 * Global Inputs:
 *                 P.GmVAsyTrpThr
 *                 P.GmVAsyTrpTm
 *                 P.GmVAsyAlmTm
 *
 *
 *                 V.GmNegSeqVPct
 *                 V.DelTm3
 *
 *
 * Global Outputs:
 *                Flt.GmVAsyAlm
 *                Flt.GmVAsyTrp
 *
 *
 *                V.GmVAsyAlmF
 *                V.GmVAsyTrpF
 *
 *
 * Locals:
 *                V.GmVAsyTrpTmr
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Related Visos:  GmProt_033
 *
 * Abstract:
 *    This function is repsonsible for implementing grid monitor voltage asymmetry
 * protection.  There is one alarm and and one trip fault.
 *
 *
 *===========================================================================*/
void GridMonitorVoltageAsymmetry(void)
{
    float RstTrpTm;

    /////////////////////////////////
    //Trip and alarm
    /////////////////////////////////

    if ( GmNegSeqVPct > PARM(GmVAsyTrpThr))
    {
        GmVAsyTrpTmr = GmVAsyTrpTmr + DelTm3;

        if ( GmVAsyTrpTmr >= PARM(GmVAsyAlmTm))
        {
            PUSH_DIAG(GmVAsyAlm);
            GmVAsyAlmF = true;
        }

        if ( GmVAsyTrpTmr >= PARM(GmVAsyTrpTm))
        {
            GmVAsyTrpTmr = PARM(GmVAsyTrpTm);
            PUSH_DIAG(GmVAsyTrp);
            GmVAsyTrpF = true;
        }
    }
    else
    {
        GmVAsyTrpTmr = GmVAsyTrpTmr - DelTm3;
        if ( GmVAsyTrpTmr <= 0.0)
        {
            GmVAsyTrpTmr = 0.0;
            GmVAsyAlmF = false;
            GmVAsyTrpF = false;
        }
    }


    //if faults are being reset
    if (Stopped && DiagResetCmdT3)
    {
        GmVAsyAlmF   = false;
        GmVAsyTrpF   = false;
    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmVAsyTrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmVAsyTrpTmr < RstTrpTm)
            {
                GmVAsyTrpF = false;
            }
        }
    }

    return;
}


/*=============================================================================
 * Function: GridMonitorVoltageAsymmetryLevel2
 * Execution Level: Task 3
 * Execution Time: Untimed
 *
 * Global Inputs:
 *                 P.GmVAsyTrpThr
 *                 P.GmVAsyTrpTm
 *                 P.GmVAsyAlmTm
 *
 *
 *                 V.GmNegSeqVPct
 *                 V.DelTm3
 *
 *
 * Global Outputs:
 *                Flt.GmVAsyAlm
 *                Flt.GmVAsyTrp
 *
 *
 *                V.GmVAsyAlmF
 *                V.GmVAsyTrpF
 *
 *
 * Locals:
 *                V.GmVAsyTrpTmr
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Related Visos:  GmProt_033
 *
 * Abstract:
 *    This function is repsonsible for implementing grid monitor voltage asymmetry
 * protection.  There is one alarm and and one trip fault.
 *
 *
 *===========================================================================*/
void GridMonitorVoltageAsymmetryLevel2(void)
{
    float RstTrpTm;

    /////////////////////////////////
    //Trip and alarm
    /////////////////////////////////

    if ( GmNegSeqVPct > PARM(GmVAsy2TrpThr))
    {
        GmVAsy2TrpTmr = GmVAsy2TrpTmr + DelTm3;

        if ( GmVAsy2TrpTmr >= PARM(GmVAsy2AlmTm))
        {
            PUSH_DIAG(GmVAsy2Alm);
            GmVAsy2AlmF = true;
        }

        if ( GmVAsy2TrpTmr >= PARM(GmVAsy2TrpTm))
        {
            GmVAsy2TrpTmr = PARM(GmVAsy2TrpTm);
            PUSH_DIAG(GmVAsy2Trp);
            GmVAsy2TrpF = true;
        }
    }
    else
    {
        GmVAsy2TrpTmr = GmVAsy2TrpTmr - DelTm3;
        if ( GmVAsy2TrpTmr <= 0.0)
        {
            GmVAsy2TrpTmr = 0.0;
            GmVAsy2AlmF = false;
            GmVAsy2TrpF = false;
        }
    }


    //if faults are being reset
    if (Stopped && DiagResetCmdT3)
    {
        GmVAsy2AlmF   = false;
        GmVAsy2TrpF   = false;
    }

    // Reset for long trip times
    if ( Stopped)
    {
        RstTrpTm = PARM(GmVAsy2TrpTm) - TRP_TMR_RST_MRG;
        if (RstTrpTm > 0.0F)
        {
            if (GmVAsy2TrpTmr < RstTrpTm)
            {
                GmVAsy2TrpF = false;
            }
        }
    }

    return;
}


/*=============================================================================
 * Function: GridMonitorExcessivePower
 * Execution Level: Task 3
 * Execution Time: Untimed
 *
 * Global Inputs:
 *                 P.GmEx
PwrTrpTh
 *                 P.GmExPwrOnThr
 *                 P.GmExPwrOfTrh
 *
 *
 *                 V.GmPwr
 *
 * Global Outputs:
 *                Flt.GmExPwrTrp
 *                Flt.GmExPwrAlm
 *
 *                V.GmExPwrTrpF
 *                V.GmExPwrAlmF
 *
 *
 * Locals:
 *                V.GmExPwrFil
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Related Visos:  GmProt_033
 *
 * Abstract:
 *    This function is repsonsible for implementing grid monitor excessive power
 * protection.  There is one alarm and and one trip fault.
 *
 *
 *===========================================================================*/
void GridMonitorExcessivePower(void)
{

    GmExPwrFil = GmExPwrFil + GmExPwrFilGn*(GmPwr - GmExPwrFil);

    /////////////////////////////
    // Alarm
    /////////////////////////////
    if ( GmExPwrFil >= PARM(GmExPwrOnThr))
    {
        PUSH_DIAG(GmExPwrAlm);
        GmExPwrAlmF = true;
    }
    else if ( GmExPwrFil <= PARM(GmExPwrOfThr))
    {
        // Pop3Flt(GmExPwrAlm);
        GmExPwrAlmF = false;
    }

    /////////////////////////////
    // Trip
    /////////////////////////////
    if ( GmExPwrFil >= PARM(GmExPwrTrpTh))
    {
        PUSH_DIAG(GmExPwrTrp);
        GmExPwrTrpF = true;
    }

    //if faults are being reset
    if (Stopped && DiagResetCmdT3)
    {
        GmExPwrAlmF = false;
        GmExPwrTrpF = false;
    }


    return;
}


/*=============================================================================
 * Function: GridMonitorReversePower
 * Execution Level: Task 3
 * Execution Time: Untimed
 *
 * Global Inputs:
 *                 P.GmRevPwrTh
 *                 P.GmRevPwrFilW
 *
 *
 *                 V.GmPwr
 *                 V.GmRvPwrFilGn
 *
 * Global Outputs:
 *                Flt.GmRevPwrTrp
 *
 *                V.GmRevPwrTrpF
 *
 *
 * Locals:
 *                V.GmRevPwrFil
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Related Visos:  GmProt_033
 *
 * Abstract:
 *    This function is repsonsible for implementing grid monitor reverse power
 * protection.  There is one trip fault for this protective.
 *
 *
 *===========================================================================*/
void GridMonitorReversePower(void)
{
    GmRevPwrFil = GmRevPwrFil + GmRvPwrFilGn*(GmPwr - GmRevPwrFil);
    if ( GmRevPwrFil > 0.0)
    {
        GmRevPwrFil = 0.0;
    }


    /////////////////////////////
    // Trip
    /////////////////////////////
    // use different thresholds for GmRevPwrTrp if Db cell test is in progress
    if ( GmRevPwrFil <= GmRevPwrThr )
    {
        PUSH_DIAG(GmRevPwrTrp);
        GmRevPwrTrpF = true;
    }

    //if faults are being reset
    if (Stopped && DiagResetCmdT3)
    {
        GmRevPwrTrpF = false;
    }


    return;
}


/*=============================================================================
 * Function: GridMonitorFeedbackVerfication
 * Execution Level: Task 3
 * Execution Time: Untimed
 *
 * Global Inputs:
 *                 P.GmAuxPwrW
 *                 V.GmAuxPwr
 *
 *                 P.GmAuxQPwrW
 *                 V.GmAuxQPwr
 *
 *
 * Global Outputs:
 *                V.GmAuxPwrFil
 *                V.GmAuxQPwrFil
 *
 *
 * Locals:
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Related Visos:  GmProt_033
 *
 * Abstract:
 *    This function is repsonsible for calculating the auxillary real and reactive
 * power by taking the difference of the grid monitor calculated power and the
 * power calculated by the converter feedbacks.
 *
 *
 *===========================================================================*/
void GridMonitorFeedbackVerfication(void)
{

    //Auxillary Real Power Check
    GmAuxPwr    = GmPwrT3 - PwrGridT3;
    GmAuxPwrFil = GmAuxPwrFil + PARM(GmAuxPwrW)*DelTm3*(GmAuxPwr - GmAuxPwrFil);

    //Auxillary Reative Power Check
    GmAuxQPwr    = GmQPwrT3 - QPwrGridT3;
    GmAuxQPwrFil = GmAuxQPwrFil + PARM(GmAuxQPwrW)*DelTm3*(GmAuxQPwr - GmAuxQPwrFil);

    return;
}


/*=============================================================================
 * Function: GridMonitorRelayManagement
 * Execution Level: Task 3
 * Execution Time: Untimed
 *
 * Global Inputs:
 *
 *                 P.GmUVRlyEnb
 *                 P.GmUV1RlyEnb
 *                 P.GmUV2RlyEnb
 *                 P.GmUV3RlyEnb
 *                 P.GmUV4RlyEnb
 *                 P.GmUVCvRlyEnb
 *                 V.GmUV1TrpFlg
 *                 V.GmUV2TrpFlg
 *                 V.GmUV3TrpFlg
 *                 V.GmUV4TrpFlg
 *                 V.GmUVCvTrpF
 *
 *                 P.GmOVRlyEnb
 *                 P.GmOV1RlyEnb
 *                 P.GmOV2RlyEnb
 *                 V.GmOV1TrpFlg
 *                 V.GmOV2TrpFlg
 *                 V.GmFastOverVF
 *
 *                 P.GmUFRlyEnb
 *                 P.GmUF1RlyEnb
 *                 P.GmUF2RlyEnb
 *                 P.GmUF3RlyEnb
 *
 *                 P.GmOFRlyEnb
 *                 P.GmOF1RlyEnb
 *                 P.GmOF2RlyEnb
 *
 *                 P.GmVAsyRlyEnb
 *
 *                 P.GmEPwrRlyEnb
 *
 *                 P.GmRPwrRlyEnb

 *
 *
 * Global Outputs:
 *
 *
 *
 * Locals:
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Related Visos:  GmProt_033
 *
 * Abstract:
 *    This function is repsonsible for selecting the combination of grid monitor
 * faults that will open the safety string.
 *
 *
 *===========================================================================*/
void GridMonitorRelayManagement(void)
{
    unsigned UVTripFlg;
    unsigned OVTripFlg;
    unsigned UFTripFlg;
    unsigned OFTripFlg;
    unsigned MiscTripFlg;

    UVTripFlg   =  PARM(GmUVRlyEnb) && (  (PARM(GmUV1RlyEnb) && GmUV1TrpFlg) || (PARM(GmUV2RlyEnb) && GmUV2TrpFlg) ||
                                          (PARM(GmUV3RlyEnb) && GmUV3TrpFlg) || (PARM(GmUV4RlyEnb) && GmUV4TrpFlg) ||
                                          (PARM(GmUV5RlyEnb) && GmUV5TrpFlg) || (PARM(GmUV6RlyEnb) && GmUV6TrpFlg) ||
                                          (PARM(GmUVCvRlyEnb)&& GmUVCvTrpF)  );


    OVTripFlg   =  PARM(GmOVRlyEnb) && (  (PARM(GmOV1RlyEnb) && GmOV1TrpFlg) || (PARM(GmOV2RlyEnb) && GmOV2TrpFlg) ||
                                          (PARM(GmOV3RlyEnb) && GmOV3TrpFlg) || (PARM(GmOV4RlyEnb) && GmOV4TrpFlg) ||
                                          (PARM(GmOVFsRlyEnb)&& GmFastOverVF)  );


    UFTripFlg   =  PARM(GmUFRlyEnb) && (  (PARM(GmUF1RlyEnb) && GmUF1TrpF)   || (PARM(GmUF2RlyEnb) && GmUF2TrpF)   ||
                                          (PARM(GmUF3RlyEnb) && GmUF3TrpF)  );


    OFTripFlg   =  PARM(GmOFRlyEnb) && (  (PARM(GmOF1RlyEnb) && GmOF1TrpF)   || (PARM(GmOF2RlyEnb) && GmOF2TrpF)  );


    MiscTripFlg =                      (  (PARM(GmVAsyRlyEnb) && GmVAsyTrpF) || (PARM(GmVAsy2RlyEnb) && GmVAsy2TrpF) ||
                                          (PARM(GmEPwrRlyEnb) && GmExPwrTrpF)|| (PARM(GmRPwrRlyEnb)  && GmRevPwrTrpF)  );


    GmGridMonOK =  !(UVTripFlg || OVTripFlg || UFTripFlg || OFTripFlg || MiscTripFlg);

    return;
}

//===========================================================================
// Function: GridMonitorConverterProtectionT2
// Execution Level: Task 2
// Execution Time: Untimed
//
//
// Related Visos:  P0Line.vsd
//
// Abstract:
//    This function is repsonsible for pushing converter over-frequency,
// trips.
//
//
//===========================================================================
void GridMonitorConverterProtectionT2(void)
{
    //-------------------------------------------------------------------------
    //            Fast Action Open checks

    // Fast grid monitor filter Voltage Mag calc----peak voltage detection

    GmVMagPkPuT2 =  sqrtf(GmVMagFstScl * GmVMgFstPkT2);
    GmVMagOVFil += GmFstOVFilGn * ( GmVMagPkPuT2 - GmVMagOVFil );
    if ( GmVMagOVFil > GmFstOVThrs )
    {
        DisconWhlOv    = true;
        OvCnvFltState  = OVCNVFLT_FASTOV;
        PUSH_DIAG(GmFastOvDiscAlm);
    }

    //Filter 4
    GmCnvOV4Fil = GmCnvOV4Fil + DelTm2*PARM(GmOVWV4)*(GmVMagPuT2 - GmCnvOV4Fil);
    if (GmCnvOV4Fil < PARM(GmOVMin4))
    {
        GmCnvOV4Fil = PARM(GmOVMin4);
    }
    else if ( GmCnvOV4Fil >= PARM(GmOVTrpThrs4))
    {
        GmCnvOV4Fil = PARM(GmOVTrpThrs4);
        if (InitDone && GmLineFltEnb)
        {
           DisconWhlOv    = true;
           OvCnvFltState  = OVCNVFLT_LVL4;
           PUSH_DIAG(GmCnvOv4DiscAlm);
        }
    }

    if (InitDone && GmLineFltEnb)
    {
        if ( GmCnvOv3DiscFlg)
        {
            DisconWhlOv    = true;
            OvCnvFltState  = OVCNVFLT_LVL3;
            PUSH_DIAG(GmCnvOv3DiscAlm);
        }
        else if ( GmCnvOv2DiscFlg)
        {
            DisconWhlOv    = true;
            OvCnvFltState  = OVCNVFLT_LVL2;
            PUSH_DIAG(GmCnvOv2DiscAlm);
        }
        else if ( GmCnvOv1DiscFlg)
        {
            DisconWhlOv    = true;
            OvCnvFltState  = OVCNVFLT_LVL1;
            PUSH_DIAG(GmCnvOv1DiscAlm);
        }
    }

    if ( DisconWhlOv )
    {
        Q1OpnReq     = true;
        if ( (GmSVMtchErSq > GmSVmtchThr) || (ISimMode && !Q1StatSim ) || !S_K1Status )
        {
            switch (OvCnvFltState)
            {
                //==========================================================================
                //                           NO FAULT(0)
                //==========================================================================
            case OVCNVFLT_NONE:

                break;


                //==========================================================================
                //                    Over-voltage Converter Protection Level 1
                //==========================================================================
            case OVCNVFLT_LVL1:
                OVProtectTrb = true;
                PUSH_DIAG(GmCnvOv1Flt);

                break;

                //==========================================================================
                //                   Over-voltage Converter Protection Level 2
                //==========================================================================
            case OVCNVFLT_LVL2:
                OVProtectTrb = true;
                PUSH_DIAG(GmCnvOv2Flt);

                break;

                //==========================================================================
                //                   Over-voltage Converter Protection Level 3
                //==========================================================================
            case OVCNVFLT_LVL3:
                OVProtectTrb = true;
                PUSH_DIAG(GmCnvOv3Flt);

                break;

                //==========================================================================
                //                    Over-voltage Converter Protection Level 4
                //==========================================================================
            case OVCNVFLT_LVL4:
                OVProtectTrb = true;
                PUSH_DIAG(GmCnvOv4Flt);

                break;

                //==========================================================================
                //                   Fast over-voltage (5)
                //==========================================================================
            case OVCNVFLT_FASTOV:
                GmFastOverVF = true;
                OVProtectTrb = true;
                PUSH_DIAG( GmFastOverV);
                break;
            }
        }
    }

    return;
}

//=============================================================================
// Function: GridMonitorConverterProtection
// Execution Level: Task 3
// Execution Time: Untimed
//
// Global Inputs:
//                 P.
//
//
//                 V.Gm
//
// Global Outputs:
//                Flt.
//                Flt.
//
//
//
// Locals:
//                V.GmExPwrFil
//
//
// Return Value: Not applicable
// Function Calls: None
//
// Related Visos:  P0Line.vsd
//
// Abstract:
//    This function is repsonsible for converter over-frequency, under-frequency,
// over-voltage, under-voltage faults.
//
//
//===========================================================================
void GridMonitorConverterProtection(void)
{
    float   DelInt;
    double  ipart;              // Integer part of floating point number


    /////////////////////////////////////////////////
    // Converter Over-Voltage Protection
    /////////////////////////////////////////////////

    //Filter 1
    // The modf function is used to allow the slower filter to be accurate
    // Intermediate calculations are done in per cent to allow for use of modf

    GmCnvOV1Filter =  GmCnvOV1Filter + GmCnvOV1GnD *((double)GmVMagPuT3 - GmCnvOV1Filter);

    if (GmCnvOV1Filter < (double)PARM(GmOVMin1))
    {
        GmCnvOV1Filter = (double)PARM(GmOVMin1);
        GmCnvOv1DiscFlg = false;
    }
    else if ( GmCnvOV1Filter >= (double)PARM(GmOVTrpThrs1))
    {
        GmCnvOV1Filter  = (double)PARM(GmOVTrpThrs1);
        GmCnvOv1DiscFlg = true;
    }
    else
    {
        GmCnvOv1DiscFlg = false;
    }

    GmCnvOV1Fil = (float) GmCnvOV1Filter;

    //Filter 2
    GmCnvOV2Fil = GmCnvOV2Fil + DelTm3*PARM(GmOVWV2)*(GmVMagPuT3 - GmCnvOV2Fil);
    if (GmCnvOV2Fil < PARM(GmOVMin2))
    {
        GmCnvOV2Fil     = PARM(GmOVMin2);
        GmCnvOv2DiscFlg = false;
    }
    else if ( GmCnvOV2Fil >= PARM(GmOVTrpThrs2))
    {
        GmCnvOV2Fil     = PARM(GmOVTrpThrs2);
        GmCnvOv2DiscFlg = true;
    }
    else
    {
        GmCnvOv2DiscFlg = false;
    }

    //Filter 3
    GmCnvOV3Fil = GmCnvOV3Fil + DelTm3*PARM(GmOVWV3)*(GmVMagPuT3 - GmCnvOV3Fil);
    if (GmCnvOV3Fil < PARM(GmOVMin3))
    {
        GmCnvOV3Fil     = PARM(GmOVMin3);
        GmCnvOv3DiscFlg = false;
    }
    else if ( GmCnvOV3Fil >= PARM(GmOVTrpThrs3))
    {
        GmCnvOV3Fil     = PARM(GmOVTrpThrs3);
        GmCnvOv3DiscFlg = true;
    }
    else
    {
        GmCnvOv3DiscFlg = false;
    }


    /////////////////////////////////////////////////
    // Converter Under-Voltage Protection
    /////////////////////////////////////////////////

    GmCnvUV1Filter =  GmCnvUV1Filter + GmCnvUV1Gn *((double)GmVMagPuT3 - GmCnvUV1Filter);
    if (GmCnvUV1Filter > PARM(GmUVMax1))
    {
        GmCnvUV1Filter = (double)PARM(GmUVMax1);
    }


    GmCnvUV1Fil = (float) GmCnvUV1Filter;

    //Fast filter
    GmCnvUV2Fil =  GmCnvUV2Fil + DelTm3*PARM(GmUVWV2)*(GmVMagPuT3 - GmCnvUV2Fil);
    if (GmCnvUV2Fil > PARM(GmUVMax2))
    {
        GmCnvUV2Fil = PARM(GmUVMax2);
    }

    if (InitDone && L_Running && GmPllPrvFstp)
    {
        if ( (GmCnvUV2Fil <= GmUVTrpDynThr2) || (GmCnvUV1Fil <= GmUVTrpDynThr1) )
        {
            UVProtectTrb = true;
            PUSH_DIAG(GmCnvUVFlt);
        }
    }

    GmCnvFrqFil =  GmCnvFrqFil + DelTm3*PARM(GmUFOFW1)*(GmFreqPu - GmCnvFrqFil);
    GmCnvOFFil =  GmCnvOFFil + DelTm3*PARM(GmUFOFW2)*(GmFreqPu - GmCnvOFFil);
    if ( GmCnvOFFil < PARM(GmOFMin))
    {
        GmCnvOFFil = PARM(GmOFMin);
    }

    GmCnvUFFil =  GmCnvUFFil + DelTm3*PARM(GmUFOFW2)*(GmFreqPu - GmCnvUFFil);
    if ( GmCnvUFFil > PARM(GmUFMax))
    {
        GmCnvUFFil = PARM(GmUFMax);
    }

    /////////////////////////////////////////////////
    // Converter Frequency Protection
    /////////////////////////////////////////////////
    if (InitDone && !Stopped)
    {
        /* Over-frequency  trip*/
        if ( (GmCnvOFFil >= PARM(GmOFTrpThrs2)) || (GmCnvFrqFil >= PARM(GmOFTrpThrs1)))
        {
            OFProtectTrb = true;
            PUSH_DIAG(GmCnvOFFlt);
        }

        /* Under-frequency  trip*/
        if ( (GmCnvUFFil <= PARM(GmUFTrpThrs2)) || (GmCnvFrqFil <= PARM(GmUFTrpThrs1)))
        {
            UFProtectTrb = true;
            PUSH_DIAG(GmCnvUFFlt);
        }
    }


    /////////////////////////////////////////////////
    // Converter Asymmetry  Protection
    /////////////////////////////////////////////////


    //Slow filter
    // The modf function is used to allow the slower filter to be accurate
    DelInt      = DelTm3*PARM(GmCVAsmW1) * (GmNegSeqVPct - GmCVAsmFil1);
    GmCVAsmFrc  = modf( (GmCVAsmFrc + DelInt), &ipart);
    GmCVAsmInt  = GmCVAsmInt + ipart;
    GmCVAsmFil1 = GmCVAsmInt + GmCVAsmFrc;

    if (GmCVAsmFil1 < PARM(GmCVAsmMin1))
    {
        GmCVAsmFil1 = PARM(GmCVAsmMin1);
        GmCVAsmInt  = GmCVAsmMinInt;
        GmCVAsmFrc  = GmCVAsmMinFrc;
    }

    //Fast filter
    GmCVAsmFil2 = GmCVAsmFil2 + DelTm3*PARM(GmCVAsmW2)*(GmNegSeqVPct - GmCVAsmFil2);
    if (GmCVAsmFil2 < PARM(GmCVAsmMin2))
    {
        GmCVAsmFil2  = PARM(GmCVAsmMin2);
    }

    if (InitDone && GmLineFltEnb)
    {
        if ( (GmCVAsmFil1 >= PARM(GmCVAsmThrs1)) || (GmCVAsmFil2 >= PARM(GmCVAsmThrs2) ))
        {
            VAProtectTrb = true;
            PUSH_DIAG(GmCnvVAsmFlt);
        }

        if ( GmCVAsmFil1 > PARM(GmCVAsmAlmOn))
        {
            if ( !PARM(GmVAsmAlmInh))
            {
                GmCnvVAsmAlmFlg = true;
            }
        }
        else if (GmCVAsmFil1 <= PARM(GmCVAsmAlmOf))
        {
            GmCnvVAsmAlmFlg = false;
        }

        if ( GmNegSeqVPct > PARM(GmPhShortOn))
        {
            GmPhShort = true;
        }
        else if (GmNegSeqVPct < PARM(GmPhShortOff))
        {
            GmPhShort = false;
        }

    }
    else
    {
        GmPhShort = false;
        GmCnvVAsmAlmFlg = false;
    }
    if(GmCnvVAsmAlmFlg) PUSH_DIAG(GmCnvVAsmAlm);



    //if faults are being reset
    if (Stopped && DiagResetCmdT3)
    {
        OVProtectTrb = false;
        UVProtectTrb = false;
        VAProtectTrb = false;
        OFProtectTrb = false;
        UFProtectTrb = false;
		GmFastOverVF = false;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This function is repsonsible for populating three summary bits for use
//    by the controller.  At present, this bit population is fixed
//    combinational logic with no parameter control over poplation of the
//    old bits.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorSummaryBitManagement(void)
{

    GmLVRTMon = !GmUVAlmFlg;


    GmUVTrpSum = GmUV1TrpFlg || GmUV2TrpFlg || GmUV3TrpFlg || GmUV4TrpFlg || GmUV5TrpFlg || GmUV6TrpFlg || GmUVCvTrpF;

    GmOVTrpSum = GmOV1TrpFlg || GmOV2TrpFlg || GmOV3TrpFlg || GmOV4TrpFlg;

    GmVAsyTrpSum = GmVAsyTrpF|| GmVAsy2TrpF;

    GmVoltMon = !(GmUVTrpSum || GmOVTrpSum  || GmVAsyTrpSum);


    GmUFTrpSum = GmUF1TrpF   || GmUF2TrpF   || GmUF3TrpF;

    GmOFTrpSum = GmOF1TrpF   || GmOF2TrpF;

    GmFreqMon = !(GmUF1TrpF  || GmOF1TrpF);

    if ( PARM(Net_Type) == NET_ABSENT )
    {
        if ( (!GmVoltMon || !GmFreqMon) && !Stopped)
        {
            GmFrqVltTmr = GmFrqVltTmr + DelTm3;
            if ( GmFrqVltTmr > PARM(GmFrqVltDlyTm) )
            {
                GmFrqVltTmr = PARM(GmFrqVltDlyTm);
                PUSH_DIAG(GmFrqVltStop);
                GmFrqVltStopAct = true;

            }
        }
        else
        {
            GmFrqVltTmr = 0.0;
        }
    }

    if ( Stopped)
    {
        GmFrqVltStopAct = false;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This is repsonsible for pushing the grid monitor configuration faults.
//    These faults originate in other functions that make up the total of the
//    grid monitor protections.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorConfigCheck(void)
{

    if ( Init3Sec)
    {
        if (GmUVCfgBad)
        {
            PUSH_DIAG(GmUVCfgTrp);

        }

        if (GmOVCfgBad)
        {
            PUSH_DIAG(GmOVCfgTrp);

        }

        if (GmUFCfgBad)
        {
            PUSH_DIAG(GmUFCfgTrp);

        }

        if (GmOFCfgBad)
        {
            PUSH_DIAG(GmOFCfgTrp);

        }
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This is repsonsible for implementing the grid monitor protectives.  It
//    calls a number of other functions that make up the total of the grid
//    monitor protections.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorProtection(void)
{

    GridMonitorVoltageInputSteering();
    if ( (SysTmSec > PARM(GmProtInitTm)) && !PwrDnFbkDsb && !HdwTstMode &&
            L_BridgeRebf.BrgHdwDataVld )
    {
        GridMonitorUnderVoltageAlarm();
        GridMonitorUnderVoltageLevel1();
        GridMonitorUnderVoltageLevel2();
        GridMonitorUnderVoltageLevel3();
        GridMonitorUnderVoltageLevel4();
        GridMonitorUnderVoltageLevel5();
        GridMonitorUnderVoltageLevel6();
        GridMonitorUnderVoltageCurve();
        GridMonitorOverVoltageAlarm();
        GridMonitorOverVoltageLevel1();
        GridMonitorOverVoltageLevel2();
        GridMonitorOverVoltageLevel3();
        GridMonitorOverVoltageLevel4();
        GridMonitorUnderFrequency();
        GridMonitorOverFrequency();
        GridMonitorVoltageAsymmetry();
        GridMonitorVoltageAsymmetryLevel2();
        GridMonitorExcessivePower();
        GridMonitorReversePower();
        GridMonitorFeedbackVerfication();
        GridMonitorRelayManagement();
        GridMonitorSummaryBitManagement();
        GridMonitorConverterProtection();
    }

    return;
}





