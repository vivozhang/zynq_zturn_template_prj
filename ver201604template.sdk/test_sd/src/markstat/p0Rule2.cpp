///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Grid Product Level Rules
//
// DESCRIPTION
//      This file provides the top level product rules.
//
// COPYRIGHT    Copyright (c) 2010
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
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "p0Rule2.h"


// Constants
//----------
#define VPRP_AVG_ARRAY_SIZE 12

#define TOC_ALM_SCL    1.0776F // = *SQRT( (1.0*1.0) + (1.15*1.15)/2.0 )  -- Heating 1/2 way from heated to trip
#define SQRT2_3   (SQRT2/SQRT3)
#define OLD_VAC_METH 0

#define CAB_LIM_STND  0

#define EXP(a) (1.0 + a + a*a/2.0 + a*a*a/6.0)

#define UF_OF_LVL1_TM         63.0
#define UV_OV_LVL_TM1         1.05


// Externals
//----------
extern void calc_2nd_order(filt2_in_type *Filt_In_Ptr, filt2_out_type *Filt_Out_Ptr);
extern int RunCommonAILRules(int Rule_Set);
extern int WindInstrumentRules(void);

extern float            LvPwrSlope1;
extern float            LvPwrSlope2;
extern float            LvPwrSlope3;
extern float            PwrPllFilGn;
extern float            OneMnTPllMeld;
extern float            TorTstSlwRatDt;
extern float            GmUV1TrpThrA;
extern float            GmUV2TrpThrA;
extern float            GmUV3TrpThrA;
extern float            GmUV4TrpThrA;
extern float            GmUV5TrpThrA;
extern float            GmUV6TrpThrA;
extern float            GmOV1TrpThrA;
extern float            GmOV2TrpThrA;
extern float            GmOV3TrpThrA;
extern float            GmOV4TrpThrA;
extern float            GmUF1TrpThrA;
extern float            GmUF2TrpThrA;
extern float            GmUF3TrpThrA;
extern float            GmOF1TrpThrA;
extern float            GmOF2TrpThrA;

extern float            DelTm2;
extern float            DelTm2Rcp;
extern float            DelTm3;
extern float            DelTm3Rcp;
extern float            DFCapW;
extern float            DFCapWSq;
extern float            DFVllFilGn;
extern float            DFWTocT3;
extern float            FanStateTmr;
extern float            GmOV3Cap;
extern float            GmOV3CapTm;
extern float            GmOV4Cap;
extern float            GmOV4CapTm;
extern float            GmSLIErrGn;
extern float            GmSLIErrThr;
extern float            GmUVCvLCap;
extern float            GmUVCvUCap;
extern float            HvIyRatPk;
extern float            IOpn1AbsNWGn;
extern float            L_DelTm1;
extern float            L_DelTm1Rcp;
extern float            L_DelTm1Rcp;
extern float            L_LXfmr;
extern float            L_VlnIn;
extern float            L_VdcFbkAdjMult;
extern unsigned         SimMode;
extern float            QregClmpSF;
extern float            R_DelTm1Rcp;
extern float            L_DbCelTstVlnGn;
extern float            R_IxRat;
extern float            R_IyMax;
extern float            R_IyRat;
extern float            R_TorSpdHpA0;
extern float            R_TorSpdHpA1;
extern float            R_TorSpdHpA2;
extern float            R_TorSpdHpB1;
extern float            R_TorSpdHpB2;
extern float            R_TorSpdLpA0;
extern float            R_TorSpdLpA1;
extern float            R_TorSpdLpA2;
extern float            R_TorSpdLpB1;
extern float            R_TorSpdLpB2;
extern float            R_TorSpdLpGn;
extern float            R_TorSRIntGn;
extern float            R_TrqRat;
extern float            DelSpdIntDt;
extern float            DelTrqIntDt;
extern float            WBLinSpilMrgRcp;
extern float            WIRmsInstT3;
extern float            GmVoltsPrAmp;
extern float            L_DfPwrgain;
extern float            L_DfQpwrgain;
extern float            L_MaDeltVoltGn;
extern float            SyncTstVsScl;
extern float            XfmrRatioLnStr;
extern float            S_VMgILmPkGn;
extern float            S_VMgILmUpGn;
extern float            S_VMaxPwrGn;
extern float            S_IxRat;
extern int              L_T1FramesPerT2;
extern int              R_T1FramesPerT2;
extern int              T2FramesPerT3;
extern float            GmTocAlmOnDl;
extern float            GmTocAlmOffDl;
extern float            WBGmDynSlp;
extern float            WBLinDynSlp;
extern float            WBAclfuDynSlp;
extern float            DfTocAlmOnDl;
extern float            DfTocAlmOffDl;
extern float            FastDILimDel;
extern float            CCUPwrMax;
extern float            RpnwRat;
extern unsigned         RPNWReqEgd;
extern unsigned         RunReqEgd;
extern unsigned         VarReqEgd;
extern unsigned         CCUConfig;
extern unsigned         CCUFeature;
extern unsigned         GridCodeCfg;
extern unsigned         CCUFwVersion;
extern unsigned         CCUHardware;
extern unsigned         CCUFwProduct;
extern float            DnTwrTempGn;
extern float            R_TrqRat;
extern float            R_IMaxLimSq;
extern float            S_IMaxLimSq;
extern float            AirTempGenGn;
extern float            AirTempDlGn;

extern float            LinImbTempGn;
extern float            RImbTempGn;
extern float            L_SinkTempRseGn;
extern float            RSinkTempRseGn;
extern float            TurbineOvrldPwrPu;

extern float            MaxTimeTempRiseOk;
extern float            NomTimeTempRiseOk;

extern float            R_IlimSlopeC;
extern float            S_IlimSlopeC;
extern float            KdfQPwrTm;
extern float            L_DfQPwrGn;
extern unsigned         LclCnvRunReq;
extern float            GmPllHotDlGn;
extern float            GmPllShiftWdt;
extern Code_ID_struct   Code_ID;

extern float            DelTm3;
extern float            GmFrqNomRcp;
extern float            GmSVmtchThr1;
extern float            GmSVmtchThr2;
extern unsigned         SimAcclAct;

extern double           GmCnvUV1Gn;
extern double           GmCnvOV1GnD;
extern float            GmCnvOV1Gn;
extern float            GmCnvOV2Gn;
extern float            GmCnvOV3Gn;
extern float            GmCnvOV4Gn;
extern float            S_K1ImagThrs;
extern float            VmagStpDlyGn;
extern float            L_VdbOnAdj;
extern float            PllErrTrqFlUpWdt;
extern float            PllErrTrqFlDnWdt;

extern unsigned         L_LTestIp;

extern void VoltVarRules();
extern void PowerLimiterRules();
extern void TurbineControlSimRules();
// Parameters
//----------
CREATE_PARM(DbResIMin,         float);       // MA constactor close test min current
CREATE_PARM(DFIabcEstDtDly,    float);       // Delay to align brg currents to estimated DF currents (dt's)
CREATE_PARM(GmPllIniTm,        float);       // Line converter pll init time after going to stop state
CREATE_PARM(GmProtInitTm,      float);       // Init time for Grid monitor proctections to star executing (sec)
CREATE_PARM(CableConfig,       unsigned);    // Cable configuration type
CREATE_PARM(CableTreFile,      unsigned);

// Variables
//----------
CREATE_PUBVAR(R_IyMax,         float);
CREATE_PUBVAR(GmUVCvUCap,      float);
CREATE_PUBVAR(GmUVCvLCap,      float);
CREATE_PUBVAR(RPNWReqEgd,      unsigned);
CREATE_PUBVAR(RstReqEgd,       unsigned);
CREATE_PUBVAR(RunReqEgd,       unsigned);
CREATE_PUBVAR(GmConfig,        unsigned);
CREATE_PUBVAR(CabCfgType,      unsigned);
CREATE_PUBVAR(P0RuleCntr,      int);
CREATE_PUBVAR(CapWizTypAct,    int);
CREATE_PUBVAR(CapBackAct,      int);
CREATE_PUBVAR(CapResAct,       int);

CREATE_PUBVAR(R_IMagOLRcp,     float);
CREATE_PUBVAR(S_IMagOLRcp,     float);
CREATE_PUBVAR(R_RIndCorRcp,    float);
CREATE_PUBVAR(L_RIndCorRcp,    float);
CREATE_PUBVAR(T1T2FbkScale,    float);

CREATE_PUBVAR(GmRAFCnt,        int);
CREATE_PUBVAR(GmRAFCntFlt,     float);
CREATE_PUBVAR(GmRAFGnInnr,     float);
CREATE_PUBVAR(GmRAFGnOutr,     float);
CREATE_PUBVAR(GmRAFGnDiff,     float);

CREATE_PUBVAR(GmVMgFstUpGn,    float);
CREATE_PUBVAR(GmVMgFstDnGn,    float);
CREATE_PUBVAR(GmVMagFstScl,    float);
CREATE_PUBVAR(GmFstOVFilGn,    float);
CREATE_PUBVAR(GmFstOVThrs,     float);
CREATE_PUBVAR(VlnPeakRcp,      float);
CREATE_PUBVAR(INomPk,          float);
CREATE_PUBVAR(GmCnvOVMinx,     float);
CREATE_PUBVAR(GmCnvOVThrx,     float);

//Device open thresholds based on voltage matches.
CREATE_PUBVAR(GmLVmtchThr,     float);

CREATE_PUBVAR(GmLVmtchErGn,    float);
CREATE_PUBVAR(LSVmtchThr,      float);
CREATE_PUBVAR(LSVmtchErGn,     float);

CREATE_PUBVAR(XfmrSecVmtchGn,  float);

CREATE_PUBVAR(DFConvISqr,      float);
CREATE_PUBVAR(GmUVCvSlope,     float);
CREATE_PUBVAR(GmUVCvConstA,    float);
CREATE_PUBVAR(GmUVCvConstB,    float);
CREATE_PUBVAR(GmUVCvConstC,    float);
CREATE_PUBVAR(GmUVCvConstD,    float);
CREATE_PUBVAR(GmUVCConstK1,    float);
CREATE_PUBVAR(GmUVCVSecThr,    float);
CREATE_PUBVAR(GmUVCvSlopeC,    float);
CREATE_PUBVAR(GmUVCvYInt,      float);
CREATE_PUBVAR(GmUVCvYIntC,     float);
CREATE_PUBVAR(GmUVCvVpuUpr,    float);
CREATE_PUBVAR(GmUVCvVpuLwr,    float);
CREATE_PUBVAR(GmUVCvVKnee,     float);
CREATE_PUBVAR(GmUVCvVLvl4,     float);

CREATE_PUBVAR(GmUVCfgBadT1,    unsigned);
CREATE_PUBVAR(GmUVCfgBadT2,    unsigned);
CREATE_PUBVAR(GmUVCfgBadT3,    unsigned);
CREATE_PUBVAR(GmUVCfgBadT4,    unsigned);
CREATE_PUBVAR(GmUVCfgBadT5,    unsigned);
CREATE_PUBVAR(GmUVCfgBadT6,    unsigned);
CREATE_PUBVAR(GmUVCfgBadT7,    unsigned);
CREATE_PUBVAR(GmUVCfgBadT8,    unsigned);
CREATE_PUBVAR(GmUVCfgBadT9,    unsigned);
CREATE_PUBVAR(GmUVCfgBadT10,   unsigned);
CREATE_PUBVAR(GmUVCfgBadT11,   unsigned);
CREATE_PUBVAR(GmUVCfgBadT12,   unsigned);
CREATE_PUBVAR(GmOVCfgBadT1,    unsigned);
CREATE_PUBVAR(GmOVCfgBadT2,    unsigned);
CREATE_PUBVAR(GmOVCfgBadT3,    unsigned);
CREATE_PUBVAR(GmOVCfgBadT4,    unsigned);
CREATE_PUBVAR(GmOVCfgBadT5,    unsigned);
CREATE_PUBVAR(GmOVCfgBadT6,    unsigned);
CREATE_PUBVAR(GmUFCfgBadT1,    unsigned);
CREATE_PUBVAR(GmUFCfgBadT2,    unsigned);
CREATE_PUBVAR(GmUFCfgBadT3,    unsigned);
CREATE_PUBVAR(GmUFCfgBadT4,    unsigned);
CREATE_PUBVAR(GmUFCfgBadT5,    unsigned);
CREATE_PUBVAR(GmOFCfgBadT1,    unsigned);
CREATE_PUBVAR(GmOFCfgBadT2,    unsigned);
CREATE_PUBVAR(GmOFCfgBadT3,    unsigned);
CREATE_PUBVAR(GmOFCfgBadT4,    unsigned);

CREATE_PUBVAR(GmUVCfgBad,      unsigned) = false;
CREATE_PUBVAR(GmOVCfgBad,      unsigned) = false;
CREATE_PUBVAR(GmUFCfgBad,      unsigned) = false;
CREATE_PUBVAR(GmOFCfgBad,      unsigned) = false;

CREATE_PUBVAR(PmpShftPwr,      float);
CREATE_PUBVAR(FanOnGth,        float);
CREATE_PUBVAR(FanOnGthRcp,     float);
CREATE_PUBVAR(CoolRHeatRcp,    float);
CREATE_PUBVAR(CoolntTempCalcGn,float);
CREATE_PUBVAR(L_BrgLssGn,      float);
CREATE_PUBVAR(R_BrgLssGn,      float);
CREATE_PUBVAR(GmPhImbTmx,      float);
CREATE_PUBVAR(L_FilSignTm1,    float);
CREATE_PUBVAR(L_FilSignTm2,    float);
CREATE_PUBVAR(GmIGndFilWdt,    float);
CREATE_PUBVAR(GmIGndProtFilWdt,float);

CREATE_PUBVAR(GmCVAsmMinFrc,   float);
CREATE_PUBVAR(GmOVMinFrc,      float);
CREATE_PUBVAR(L_IabcDFcomp1,   float);
CREATE_PUBVAR(L_IabcDFcomp0,   float);
CREATE_PUBVAR(GmCVAsmMinInt,   int);
CREATE_PUBVAR(GmUVMaxInt,      int);
CREATE_PUBVAR(GmOVMinInt,      int);
CREATE_PUBVAR(T3sPerSec,       int);
CREATE_PUBVAR(GmVcoSatLvl,   float);
CREATE_PUBVAR(S_K1VacrossTol,   float);
CREATE_PUBVAR(S_K1VerrTime, float);
CREATE_PUBVAR(SpdPllFilGn     ,       float);
CREATE_PUBVAR(GmExPwrFilGn  ,float) ;      // Excessiv power filter gain
CREATE_PUBVAR(GmOF1Cap     ,  float );
CREATE_PUBVAR(GmOF2Cap     ,  float );
CREATE_PUBVAR(GmOF1CapTm   ,  float );
CREATE_PUBVAR(GmOF2CapTm   ,  float );
CREATE_PUBVAR(GmOFMinLvl   ,  float );
CREATE_PUBVAR(GmOV1Cap     ,  float );
CREATE_PUBVAR(GmOV2Cap     ,  float );
CREATE_PUBVAR(GmOV1CapTm   ,  float );
CREATE_PUBVAR(GmOV2CapTm   ,  float );
CREATE_PUBVAR(GmOVMinLvl   ,  float );
CREATE_PUBVAR(GmRvPwrFilGn  ,float );      // Reverse power filter gain
CREATE_PUBVAR(GmUF1Cap     ,  float );
CREATE_PUBVAR(GmUF1CapTm   ,  float );
CREATE_PUBVAR(GmUF2Cap     ,  float );
CREATE_PUBVAR(GmUF2CapTm   ,  float );
CREATE_PUBVAR(GmUF3Cap     ,  float );
CREATE_PUBVAR(GmUF3CapTm   ,  float );
CREATE_PUBVAR(GmUFMaxLvl   ,  float );
CREATE_PUBVAR(GmUV1Cap     ,  float );
CREATE_PUBVAR(GmUV1CapTm   ,  float );
CREATE_PUBVAR(GmUV2Cap     ,  float );
CREATE_PUBVAR(GmUV2CapTm   ,  float );
CREATE_PUBVAR(GmUV3Cap     ,  float );
CREATE_PUBVAR(GmUV3CapTm   ,  float );
CREATE_PUBVAR(GmUV4Cap     ,  float );
CREATE_PUBVAR(GmUV4CapTm   ,  float );
CREATE_PUBVAR(GmUVCvUCapTm ,  float );
CREATE_PUBVAR(GmUVCvLCapTm ,  float );
CREATE_PUBVAR(GmUVCvVSThr1  , float); // Curve under-voltage curve volt-sec threshold, phase 1
CREATE_PUBVAR(GmUVCvVSThr2  , float); // Curve under-voltage curve volt-sec threshold, phase 2
CREATE_PUBVAR(GmUVCvVSThr3  , float); // Curve under-voltage curve volt-sec threshold, phase 3
CREATE_PUBVAR(GmUVMaxLvl   ,  float );

// Unpublished Variables
//----------------------
filt2_in_type  TorSpdFil2S;
filt2_out_type TorSpdFil2Z;
filt2_out_type TorSpdFil2ZTmp;


struct CCU_Feat_type
{
    uint    Svrt            :1;         // B0 SVRT legacy feature
    uint    Lvrt            :1;         // B1 LVRT legacy feature
    uint    Zvrt            :1;         // B2 ZVRT legacy feature
    uint    ZvrtGen2Base    :1;         // B3 ZVRT base feature
    uint    ZvrtGen2Opt     :1;         // B4 ZVRT optional feature
    uint    Cvrt            :1;         // B5 Custom voltage ridethrough
    uint    FastDisc        :1;         // B6 Fast disconnect feature
    uint    FastBlock       :1;         // B7 Fast blocking feature
    uint    PhaseJump       :1;         // B8 Phase Jump Trip
    uint    TorsVibProt     :1;         // B9 Torsional vibration protection feature
    uint    AntiIsland      :1;         // B10 Anit-Islanding feature
    uint    RPNW            :1;         // B11 Reactive power no wind feature
    uint    DelSpdPAct      :1;         // B12 Delta speed/power feature
    uint    DelSpdTrqAct    :1;         // B13 Delta torque and delta speed feature
    uint    DriveTrainDm    :1;         // B14 Drive Train Damping feature
    uint    ZvrtGen3Base    :1;         // B15 ZVRT Gen 3 base feature
    uint    Spares          :17;        // B16-31 spares
};

struct CCU_Cnfg_type
{
    uint    Svrt            :1;         // B0 SVRT legacy feature
    uint    Lvrt            :1;         // B1 LVRT legacy feature
    uint    Zvrt            :1;         // B2 ZVRT legacy feature
    uint    ZvrtGen2Base    :1;         // B3 ZVRT base feature
    uint    ZvrtGen2Opt     :1;         // B4 ZVRT optional feature
    uint    Cvrt            :1;         // B5 Custom voltage ridethrough
    uint    FastDisc        :1;         // B6 Fast disconnect feature
    uint    FastBlock       :1;         // B7 Fast blocking feature
    uint    PhaseJump       :1;         // B8 Phase Jump Trip
    uint    TorsVibProt     :1;         // B9 Torsional vibration protection feature
    uint    AntiIsland      :1;         // B10 Anit-Islanding feature
    uint    RPNW            :1;         // B11 Reactive power no wind feature
    uint    DelSpdPAct      :1;         // B12 Delta speed/power feature
    uint    DelSpdTrqAct    :1;         // B13 Delta torque and delta speed feature
    uint    DriveTrainDm    :1;         // B14 Drive Train Damping feature
    uint    ZvrtGen3Base    :1;         // B15 ZVRT Gen 3 base feature
    uint    Spares          :16;        // B16-31 spares
};

struct Grid_Cnfg_type
{
    uint    GridStrength    :1;         // B0 GridStrength
    uint    VRTSuportTyp    :1;         // B1 VRTSuportType
    uint    QregFFEnb       :1;         // B2 Qreg FF Enable
    uint    AVRPresent      :1;         // B3 Auxiliary Voltage Regulator Present
    uint    LvrtIyOvr       :1;         // B4 Standard LVRT Override
    uint    HvrtIyOvr       :1;         // B5 Standard HVRT Override
    uint    VPRPVrefFix     :1;         // B6 LVRT fixed voltage reference, prop mode
    uint    VPRPIyRstAvg    :1;         // B7 LVRT Iy restore avg, prop mode
    uint    SeriesComp      :1;         // B8 Series compenesation feature
    uint    MaxRealCurr     :1;         // B9 Max Real Current
    uint    GDPLCurveOvr    :1;         // B10 GDPL Curve Override
    uint    GDPLRcrvyOvr    :1;         // B11 GDPL Recovery Override
    uint    GridFeatDef     :1;         // B12 Default grid features
    uint    VregMaxOvr      :1;         // B13 Vreg maximum Override
    uint    Spares          :18;        // B14-31 spares
};


CCU_Feat_type  CCUFeatBits;
CCU_Cnfg_type  CCUCnfgBits;
Grid_Cnfg_type GridCnfgBits;


int     DummyInt;

// Local Prototypes (to resolve forward references)
//-------------------------------------------------
int     CommonAILRule(void);
int     DefaultCaptureBuff(void);
void    RelayOutSetup(void);
int     VoltVarSetup(void);


//******************************************************************************
// Function:            RunCommonAILRules
// Execution Level:
// Execution Time:
// Formal Inputs:
//    Rule_Set          ruleset identifier
// Formal Outputs:
// Global Inputs:
// Global Outputs:
// Return Value:        SUCCESS(1), FAIL(0)
// Function Calls:
// Abstract:            select ruleset to run.
//
//******************************************************************************
int RunCommonAILRules(int Rule_Set)
{
    int     Status=SUCCESS;

    switch(Rule_Set)
    {
        // calculate motor control final values
        case RULE_SET_MAIN:
            Status = CommonAILRule();
            break;

        case (RULE_SET_MAIN + RULE_SET_SECOND_PASS):
            Status = CommonAILRule();
            break;


        // Set Volt-Var reg gains
        case RULE_SET_VOLT_VAR_SETUP:
              VoltVarSetup();
            break;

       // Default case
       default:
            break;
    }


    return (Status);
}   // RunLineRuleSet()



/******************************************************************************
 * Function:            CommonAILRule
 * Execution Level:
 * Execution Time:
 * Formal Inputs:
 * Formal Outputs:
 * Global Inputs:
 *
 * Global Outputs:
 *
 * Return Value:        SUCCESS(1), FAIL(0)
 * Function Calls:
 * Abstract:            Rules for calculating motor control final values from
 *                      tuneup results.  The drive must be stopped or the
 *                      calculations are not performed.
 *
 * Note:  Do not alter the rule calculation sequence.
 *
 *
 ******************************************************************************/
int CommonAILRule(void)
{

    float Slope2xRcp, Temp1;
    float DfInom;
    float DfXnom;
    float V_Noise_Lvl_Ln;
    float S_IxMax;
    float DFIScale;// ILimitBW;
    float VSecThr;
    float IFinal, IInitial, IThr, ITau, IResponseT;

    double  ipart;               // Integer part of floating point number

    float GmUF1TrpLvl;
    float GmUF2TrpLvl;
    float GmOF1TrpLvl;
    float GmOF2TrpLvl;


    float TmpScl;

    unsigned  *StructInit;

   PowerLimiterRules();    // see p0PwrLim.cpp


    if(PARM(TestMode) != PUMP_BACK)
    {
        // Calculate min avg current per phase when DB is full on. Account for dc link at .9*peak and at .9low line
        // Vllrms*IllRms*Sqrt(3) = Vdc*Vdc/DBR, where Vdc = 9.*.9*(Vllrms*sqrt(2))--- solve for IllRms
        SET_FV(DbResIMin, ( ((0.9F*0.9F*PARM(L_VllRms)*SQRT2) *(0.9F*0.9F*PARM(L_VllRms)*SQRT2)) / (PARM(BrgDbR) * PARM(L_VllRms)*SQRT3) ));   // HL: ESS value, needs verify!!!
    }
    else
    {
        SET_FV(DbResIMin,   200.0F);
    }



    SET_FV(LqdAuxOffTm,30.0F);    // Coolant auxillary time to shut-down
    if(SimAcclAct)
    {
        SET_FV(RtrRunDly, 0.0F);
        SET_FV(LqdAuxOnTm, 0.0F);    // bypass lyquid circule delay in acceleration mode
    }
    else
    {
        SET_FV(RtrRunDly, 0.5F);
        SET_FV(LqdAuxOnTm, 10.0F);    // Coolant auxillary time to fill pipes
    }


    SET_FV(PLCRebootInh, false); // PLC reboot inhibit
    SET_FV(RebootMinTm, 0.50F);   // Minimum reboot pulse time
    SET_FV(RebootMaxTm, 1.00F);   // Maximum reboot pulse time
    SET_FV(IRmsInstTau, 1.00F);   // Current rms instrument tau

    WIRmsInstT3 =  NoNANLim( DelTm3 / PARM(IRmsInstTau), 1.0e6F, -1.0e6F );



    SET_FV(GmWTrqFbkFil, DelTm3Rcp);
    SET_FV(R_RISqrR,   243.0e-6F);         //207uohm for inductor--from Greg--255watt@640apms rms  + 36.0uohm for shunts
    SET_FV(L_RISqrR,   2.536e-3F);         //1.74mohm for inductor -- from Greg -- 605watts @340amps rms + 250.0uohms for shunts + 546uohms for fuses
    SET_FV(Gm_RISqrR,  92.3e-6F);          //Q1@13e-6ohm/pole + 79.3uohmm (450watt total buss loss est from Steve @1375apmsrms)
    SET_FV(DFRISqrR,   PARM(CnvLineDfR));  //--From 3.X DFIG schematic

    //TODO, 2.3IPN value
    switch(PARM(SoftwareIPN))
    {
        case DFIG02_1500_5060_A:
        case DFIG02_1500_5060_B:
        case DFIG02_1850_5060_C:
        case DFIG02_1850_5060_D:
        case DFIG02_1850_5060_E:
            SET_FV(R_RIndCore, 4.4e3F);     //--Greg: 72wats at 690vrms
            SET_FV(L_RIndCore, 654.0F);     //Greg: 485watts at 690vrms
            break;
        case DFIG02_2400_5060_A:
        default:
            SET_FV(R_RIndCore, 4.4e3F);     //--Greg: 72wats at 690vrms
            SET_FV(L_RIndCore, 654.0F);     //Greg: 485watts at 690vrms
            break;
    }
    R_RIndCorRcp = 1.0F/PARM(R_RIndCore);
    L_RIndCorRcp = 1.0F/PARM(L_RIndCore);

    SET_FV(IyMagnWIyLim, 2.0F);               // r/s


   TurbineControlSimRules();

   //PowerLimiterRules();    // see p0PwrLim.cpp


    ////////////////////////////////////////////
    // Start Volt-Var Rules -- keep segregated
    ////////////////////////////////////////////

    VoltVarRules();    // see p0VltVar.cpp

    ////////////////////////////////////////////
    // End Volt-Var Rules -- keep segregated
    ////////////////////////////////////////////

    SET_FV(VregIyStpAdr,(float *) LADR_FZero);
    //IyPFStepAdr = (unsigned*) LogAddrTbl[(int)PARM(IyPFStpAdr)];



    SET_FV(BrgEnbSimReq, false);

    T3sPerSec = DelTm3Rcp;


    SET_FV(StopReqAdr,(unsigned * ) LADR_UnusedBool);
    SET_FV(XStpAdr,   (unsigned * ) LADR_UnusedBool);
    if ( PARM(TestMode) == PUMP_BACK )
    {
       SET_FV(FltRstAdr, (unsigned * ) LADR_UnusedBool );
    }
    else
       SET_FV(FltRstAdr, (unsigned * ) LADR_RstReqEgd );
    {
    }
    SET_FV(PLCRebootAdr, (unsigned * ) LADR_UnusedBool );


    if(PARM(Net_Type) == NET_ABSENT)
    {
        if (PARM(TcSimEnb))
        {
           SET_FV(RunReqAdr, (unsigned * ) LADR_TcCnvRunCmd );
           SET_FV(RPNWRunAdr,(unsigned * ) LADR_TcCnvRPNWReq );
        }
        else
        {
           SET_FV(RunReqAdr, (unsigned * ) LADR_LclCnvRunReq);
           SET_FV(RPNWRunAdr,(unsigned * ) LADR_LclRPNWReq );
        }
    }
    else
    {
       SET_FV(RunReqAdr, (unsigned * ) LADR_RunReqEgd );
       SET_FV(RPNWRunAdr,(unsigned * ) LADR_RPNWReqEgd );
    }



    SET_FV(RPNWRunDly, 0.200F);
    SET_FV(IyXferRate, DelTm2/0.200F);//200msec transfer rate

    SET_FV(WGridAbc1,75.0F);
    SET_FV(WGridAbc2,75.0F);


    if ( PARM(TestMode) == PUMP_BACK )
    {
        SET_FV(OpnPhsInh, true);
    }
    else
    {
        SET_FV(OpnPhsInh, false);
    }
    // ZJ 20150317 temporary disable new open phase check logic

    if ( (PARM(SoftwareIPN) == DFIG02_1500_5060_A) )  //disable only this converter that has no DB
    {
            SET_FV(OpnPhsMltMtdEnb,     false);
    }
    else
    {
            SET_FV(OpnPhsMltMtdEnb,     true);
    }

    if(PARM(OpnPhsMltMtdEnb))
    {
        SET_FV(OpnPhsThr1,15.0F);
        SET_FV(OpnPhsThr2,25.0F);
        SET_FV(OpnPhsUThr1,175.0F);
        SET_FV(OpnPhsUThr2,175.0F);

        SET_FV(OpnPhsCntThr1,       3);
        SET_FV(OpnPhsLgTm1,         0.01667F);
        SET_FV(OpnPhsLTmrUpGn1,     1.0F);
        SET_FV(OpnPhsLgTmrDecGn1,   0.25F);
        SET_FV(OpnPhsRstTm1,        0.01667F);

        SET_FV(OpnPhsCntThr2,       3);
        SET_FV(OpnPhsLgTm2,         0.01667F);
        SET_FV(OpnPhsLTmrUpGn2,     1.0F);
        SET_FV(OpnPhsLTmrDnGn2,     0.25F);
        SET_FV(OpnPhsRstTm2,        0.01667F);

        SET_FV(OpnPhsIpuThrs,       0.05F);
        SET_FV(OpnPhsRtrFstStp,     true);
    }
    else
    {
        SET_FV(OpnPhsThr1,15.0F);
        SET_FV(OpnPhsThr2,25.0F);
        SET_FV(OpnPhsUThr1,175.0F);
        SET_FV(OpnPhsUThr2,175.0F);

        SET_FV(OpnPhsCntThr1,       1);
        SET_FV(OpnPhsLgTm1,         1.0F);
        SET_FV(OpnPhsLTmrUpGn1,     0.0F);
        SET_FV(OpnPhsLgTmrDecGn1,   0.0F);
        SET_FV(OpnPhsRstTm1,        1.0F);

        SET_FV(OpnPhsCntThr2,       1);
        SET_FV(OpnPhsLgTm2,         1.0F);
        SET_FV(OpnPhsLTmrUpGn2,     0.0F);
        SET_FV(OpnPhsLTmrDnGn2,     0.0F);
        SET_FV(OpnPhsRstTm2,        1.0F);

        SET_FV(OpnPhsIpuThrs,       0.00F);
        SET_FV(OpnPhsRtrFstStp,     true);
    }
    SET_FV(OpnPhsTm1,0.003F);
    SET_FV(OpnPhsTm1Mp, GmFrqNomRcp * 0.180F);//medium power time:  0.003 at 60hz and proportionally longer at 50hz.
    SET_FV(OpnPhsTm1Lp, 0.005F);   //low power time
    SET_FV(OpnPhsTm1Lp0, 0.010F);  //lowest power time
    SET_FV(OpnPhsTm2,0.003F);
    if(( PARM(L_FrqInput) == 50.0F) && (PARM(GridStrength) == STRONG) )
    {
       SET_FV(OpnPhsTm2Lp, 0.0064F);
       SET_FV(OpnPhsTm2Mp, 0.004F);
    }
    else //50hz
    {
       SET_FV(OpnPhsTm2Lp, 0.005F);
       SET_FV(OpnPhsTm2Mp, 0.003F);
    }
    SET_FV(OpnPhsTm2Lp0,0.010F);
    SET_FV(OpnPhsTm1Asy, 0.010F);
    SET_FV(OpnPhsTm2Asy, 0.010F);
    SET_FV(IOpn1AbsNWW, 4.0F);
    SET_FV(IOpn1Thr2NW, 35.0F);
    SET_FV(IOpnSThr2NW, 125.0F);
    SET_FV(OpnPhsNWTm, 0.25F);



    //Open grid parameters
    SET_FV(OpnVdcOffThr, 1150.0F);   //vdc value at which open phase actions are cleared if stopped
                                                             //for calc of VdcPredict.
                                                             //dv = I*dt/c, where I is power/Vdc, and dt is time for
                                                             //regulators to remove delta power

    //RPNW open grid parms
    SET_FV(OpnGrdTm,  0.010F);
    SET_FV(OpnGrdTm1, 0.005F);
    SET_FV(OpnGrdTm2, 0.003F);
    SET_FV(OpnGrdTm3, 0.003F);
    SET_FV(OpnGrdTmRPNW, 0.010F);

    SET_FV(OpnGrdPwrMin, 200000.0F);
    SET_FV(OpnGrdPwrTh2, 50000.0F);
    SET_FV(OpnGrdPwrLv3, 0.8e6F);  // value of ESS
    SET_FV(OpnGrdPwrLv2, 0.4e6F);  // value of ESS
    SET_FV(OpnGrdPwrLv1, 0.2e6F);  // value of ESS

    SET_FV(OpnGRPNWIThr, 50.0F*SQRT2);//50amps rms


    SET_FV(OpnGrdStrVMx, 1.05F);


    SET_FV(FastDIRateLim, 55.0F);        // units in (pu Iy)/(pu sec)
                                         // Get reactive current to zero in 1/FastDIRateLim time (~18 msec)
                                         // before Q1 opens

    FastDILimDel = PARM(FastDIRateLim) * DelTm2;


    // phase jump:  need to ride thru 52 degrees.  Set err threshold to 55 deg.
    SET_FV(PllErrW, 600.0F);
    // 2.75 org rule: SET_FV(PllErrThr, 0.819F*(PARM(L_VllIn)*SQRT2*SQRT3_RCP)); //sin (55deg)*line to neautral peak
    // new rules copied from 1.6  (need review!):
    if (PARM(PhsJmpTrpEnb))
    {
       SET_FV(PllErrThr, -0.20F*(PARM(L_VllIn)*SQRT2*SQRT3_RCP));
    }
    else
    {
       SET_FV(PllErrThr, -10.0e6F);
    }


    SET_FV(R_ILimMinGn, 0.90F);
    SET_FV(S_ILimMinGn, 0.90F);

    R_IyMax = NoNANLim(((PARM(L_VllRms)*SQRT3_RCP/PARM(S_Xm)) + PARM(VregIyLim)) * PARM(S_RRatio), 1.0e6F, -1.0e6F);
    SET_FV(R_IMagOL, NoNANLim(sqrtf( (R_IyMax*R_IyMax) + (PARM(R_ILimMax)*SQRT2_RCP*PARM(R_ILimMax)*SQRT2_RCP) ), 1.0e6F, -1.0e6F) );
    R_IMagOLRcp = 1.0F/PARM(R_IMagOL);

    S_IxMax = NoNANLim(PARM(R_ILimMax)*SQRT2_RCP/PARM(S_RRatio), 1.0e6F, -1.0e6F);
    SET_FV(S_IMagOL, NoNANLim(sqrtf( (S_IxMax*S_IxMax) + (PARM(VregIyLim)*PARM(VregIyLim)) ), 1.0e6F, -1.0e6F) );
    S_IMagOLRcp = 1.0F/PARM(S_IMagOL);



    SET_FV(ZVGtOfMxTm, 2.0F);
    SET_FV(ZVGtOfMxTmSt, 0.4F);
    SET_FV(ZVStopGtTm, 0.115F);     //Extend by .055msec to match change below
    SET_FV(ZVStopGtTm2,0.230F);     //Extend from .175 to .230 to match wind-sync
    SET_FV(ZVStopGtVdcCBMgn,20.0F); //Vdc margin below CroBar Off threshold for "ZV Stop Gating"
    SET_FV(ZVPllPrmTm, PARM(GmPllGnSwTm)*0.5F);
    SET_FV(ZVPllPrmTmSt, PARM(ZVPllPrmTm));
    SET_FV(ZVQckRcvyTm, .020F);



    SET_FV(SftyRtrLnDly, 0.020F);
    SET_FV(ThrmRangeSel, 0); //0 is automatic..do not force

    // IRAM copies of EEs related to MACC, RTD, and Thermistor temp sensors.

    //  Grid Monitor Stuff (transformer secondary)
    // CT turns ratios for IO currents
    //------------------------------------------------
    SET_FV(GmCTburdenR, 0.1F);  //0.1ohms
    if (PARM(SoftwareIPN) <  DFIG02_2400_5060_A)
    {
       SET_FV(GmCTratio, 1500.0F/5.0F); //1,500:5 CT ratio on3.0 elementary version 0.36
    }
    else
    {
       SET_FV(GmCTratio, 2500.0F/1.0F); //2,500:1 CT ratio
    }

    GmVoltsPrAmp = NoNANLim( PARM(GmCTburdenR)/PARM(GmCTratio), 1.0e6F, -1.0e6F );

    SET_FV(GmVanVcoSc, 856.4367F/1.0e6F); //volts/hz...see AEAD elementary, sheet 8
    SET_FV(GmIaVcoScl, 1.46F/1.0e6F);     //volt/hz....see AEAD elementary, sheet 11
    SET_FV(GmIbVcoScl, 1.46F/1.0e6F);
    SET_FV(GmIcVcoScl, 1.46F/1.0e6F);

    GmVcoSatLvl = 1.0e6F*PARM(GmIaVcoScl)*PARM(GmCTratio)/PARM(GmCTburdenR);
    SET_FV(OpnPhSatThr1, 0.95F * GmVcoSatLvl);//Current calc at sat freq of 1Mhz
    SET_FV(OpnPhSatThr2, 0.80F * GmVcoSatLvl);
    SET_FV(OpnPhSatCnt, 2);

    //Grid monitor ground fault
    SET_FV(GmIGndFilW, 5.0F);
    GmIGndFilWdt = DelTm3 * PARM(GmIGndFilW);
    SET_FV(GmIGndProtFilW, 0.20F);
    GmIGndProtFilWdt = DelTm3 * PARM(GmIGndProtFilW);

    SET_FV(GmGndFltThr, 150.0F);
    SET_FV(GmGndAlmOn,  100.0F);
    SET_FV(GmGndAlmOff,  50.0F);
    SET_FV(GmGndFltSatCnt, 1);
    SET_FV(GmGndFltInhTm, 3.0F * DelTm3);


    SET_FV(GmPllrStpAdr, (float *) LADR_FZero);


///////////////////////////////////
// Start Islanding detection rules
///////////////////////////////////

    SET_FV(S_VMgILimPkW, PARM(L_FrqInput) * 0.1F);
    SET_FV(S_VMgILimUpW,  800.0F);
    SET_FV(S_VMaxHiTm,    0.100F);
    SET_FV(S_VMaxHiOfTm,  0.050F);
    SET_FV(S_VMaxHiDtOn,   1.30F);
    SET_FV(S_VMaxHiDtOf,   1.11F);
    SET_FV(S_VMaxShtDV,    0.95F);
    SET_FV(S_VMaxShtDTm,  0.005F);
    SET_FV(S_VMaxPllFW,    20.0F);
    SET_FV(S_VMaxPllFRt,   10.0F);

    SET_FV(S_VMaxFFOnTm,   1.5F * DelTm3);//Triggers on 2nd pass of task3
    SET_FV(S_VMaxFFOfTm,   0.05F);
    if ( !PARM(AVRPresent))
    {
        SET_FV(S_VMaxFrqFOn, PARM(VregRefMin)-0.08F);
    }
    else
    {
        SET_FV(S_VMaxFrqFOn, PARM(VregRefMin)-0.05F);
    }
    SET_FV(S_VMaxFrqFOf, PARM(VregRefMin)+0.02F);
    SET_FV(S_VMaxPllThr, 1.05F);   //+ 3hz at 60hz
    //SET_FV(S_VMaxPllThr, 1.105F);   //5hz at 50hz

    SET_FV(S_VMaxPllTh2, 1.25F);   //+ 15hz at 60hx
    SET_FV(S_VMaxPllTh3, 1.06F);

    SET_FV(S_VMaxPllMin, 0.95F); //-3hz at 60hz
    //SET_FV(S_VMaxPllMin, 0.895F); //-5hz at 50hz


    SET_FV(S_VMaxPwrW, 200.0F);
    SET_FV(S_PwrMaxIsTm, 0.008F );
    SET_FV(S_PwrMaxIs,  400.0e3F);
    SET_FV(S_PwrMxIsFst, 850.0e3F);
    SET_FV(S_PwrLwCmIs, 0.5F * 1000.0F * PARM(TurbineNpRat));
    SET_FV(S_PwrLwFbIs, 0.07F * 1000.0F * PARM(TurbineNpRat));
    SET_FV(S_VHiPwrLTm, 1.9F * DelTm3); //insure trip in two task 3's

    S_VMgILmPkGn    = L_DelTm1 * PARM(S_VMgILimPkW);
    S_VMgILmUpGn    = L_DelTm1 * PARM(S_VMgILimUpW);
    S_VMaxPwrGn     = DelTm2   * PARM(S_VMaxPwrW);


    //Pos/Neg sequence discriminator

    SET_FV(GmDscmWVxP, 0.5F*PARM(L_LineFrq));
    SET_FV(GmDscmWVyP, 0.5F*PARM(L_LineFrq));
    SET_FV(GmDscmWVxN, 0.5F*PARM(L_LineFrq));
    SET_FV(GmDscmWVyN, 0.5F*PARM(L_LineFrq));
    SET_FV(GmDscmWVxN2,100.0F);
    SET_FV(GmDscmWVyN2,100.0F);
    SET_FV(GmNSeqPuScl,1.6F);
    SET_FV(GmNSeqPuGn,0.0F);//correction adjustment for pu when pu > .05
    SET_FV(GmNSeqPuOffs,0.05F);
    SET_FV(GmNSeqAngThr, 0.05F);
    SET_FV(GmNSeqHiThr, 0.09159F*L_VlnIn);//occurs at 15% imbalance  );
    SET_FV(GmPllVyFbkSw, POS_SEQ_DSCM);
    SET_FV(GmNSqMaxStrt, 4.0F); //4.0%

    //Remote fault rules
    SET_FV(GmPllShftDB, 0.5F);
    SET_FV(GmPllSftVThr, 1.0F);
    if (PARM(SeriesCmpEnb))
    {
       SET_FV(GmPllHiGnTm, 10.0e6F);
    }
    else
    {
       SET_FV(GmPllHiGnTm, 0.0F);
    }
    SET_FV(GmPllShftTm, 0.004F);
    SET_FV(GmPllShftGn, 2.0F);
    SET_FV(GmPllShftGnN, 1.0F);
    SET_FV(GmPllShftGnH, 0.6F);
    SET_FV(GmPllShiftW, 0.5F/DelTm2);

    GmPllShiftWdt = PARM(GmPllShiftW)*DelTm2;

    //System current decrease by pll error
    SET_FV(ISysCmdSFScl, SQRT2);
    SET_FV(ISysCmdUpRt, 300.0F);
    SET_FV(ISysCmdMin, 0.0F);
    SET_FV(ISysCmdVMgMn, 0.20F);
    SET_FV(GmDsVMgPuMin, 0.1F);

    ////////////////////
    // Cool gains
    ////////////////////
    SET_FV(GmPllBwBase,  18.75F);
    SET_FV(GmPllWnSf, 0.725F);
    SET_FV(GmPllWn, PARM(GmPllWnSf) * PARM(GmPllBwBase));
    SET_FV(GmPllPrpGnR, PARM(GmPllBwBase)/L_VlnIn); /* W=Prp_Gn/Vln(peak):*/
    SET_FV(GmPllIntGnR, PARM(GmPllWn)*PARM(GmPllWn)/L_VlnIn);
    SET_FV(GmPrpGnISySF, 6.0F);
    SET_FV(GmPrpGnVMgSF, 0.0F);
    SET_FV(GmPrpGnVMgOf, 1.0F - PARM(GmPrpGnVMgSF));
    SET_FV(GmPllHotGnDy, PARM(SeriesCmpEnb));


    ////////////////////
    // Hot gains
    ////////////////////
    if ( PARM(TestMode) == PUMP_BACK )
    {
      SET_FV(GmPllBaseHot, 151.504F); //gives GmIntGnI =  .450904, GmPrpGnI = 0.268918 at 690volt. (was .4509, 2.704 for succesful china pumpback)
      SET_FV(GmPllWnHotSf, 0.105201F);
    }
    else if (PARM(GridHighZ))
    {
      SET_FV(GmPllBaseHot,  93.897F);
      SET_FV(GmPllWnHotSf,  0.144773F);
    }
    else
    {
      SET_FV(GmPllBaseHot,  451.2480725F);
      SET_FV(GmPllWnHotSf,  0.105201F);
    }

    SET_FV(GmPllWnHot, PARM(GmPllWnHotSf) * PARM(GmPllBaseHot));

    SET_FV(GmPllPrpGnI, PARM(GmPllBaseHot)/L_VlnIn); /*W=Prp_Gn*Vln(peak):W=B*/
    SET_FV(GmPllIntGnI, PARM(GmPllWnHot)*PARM(GmPllWnHot)/L_VlnIn);
    SET_FV(GmPllIntGnI1, 273.366F); //same as old Wind-DFIG at 690v
    SET_FV(GmPllPrpGnI1, 2.056F);   //same as old Wind-DFIG

    SET_FV(GmPllIntGnI2, 4.0F); // High voltage or remote fault gain
    SET_FV(GmPllPrpGnI2, 0.8F); // High voltage or remote fault gain
    SET_FV(GmPllIntGnIs, 273.366F); // integral gain for islanding detection
    SET_FV(GmPllPrpGnIs, 2.056F); // proportional gain for islanding detection
    SET_FV(GmPllFrqPrec, PARM(L_LineFrq));       //HOT_INIT clamps

    SET_FV(GmPllWMaxR,   1.333F*PARM(L_LineFrq)); //COOGmDYN clamps
    SET_FV(GmPllWMinR,   0.250F*PARM(L_LineFrq));

    SET_FV(GmPllWMaxI,   1.70F*PARM(L_LineFrq));   //HOT_DYN  clamps
    SET_FV(GmPllWMinI,   0.30F*PARM(L_LineFrq));

    SET_FV(GmPllWIntMx,  1.1F*PARM(L_LineFrq));
    SET_FV(GmPllWIntMn,  0.9F*PARM(L_LineFrq));
    SET_FV(GmPllWInMxIs,  1.33F*PARM(L_LineFrq));
    SET_FV(GmPllWInMnIs,  0.318F*PARM(L_LineFrq));
    SET_FV(GmPllWMaxIs,   1.62F*PARM(L_LineFrq));
    SET_FV(GmPllWMinIs,   0.30F*PARM(L_LineFrq));

    SET_FV(GmPllWPrec,  PARM(L_LineFrq));        //HOT_FIXFRQ clamps


    SET_FV(GmPllTrackTm, 0.1F);
    SET_FV(GmPllWShift,  0.0F);

    SET_FV(GmAngTrckInh, false);

    if(SimAcclAct)
    {
        SET_FV(GmPllIniTm,    0.1F);
    }
    else
    {
        SET_FV(GmPllIniTm,    1.0F);
    }

    SET_FV(GmProtInitTm, 4.0F + PARM(GmPllIniTm)); // 15 sec is needed in SEP to determine
                                                    // 24V PS buffer is Ok after power-up
    SET_FV(GmWPllWTrack, 1.0F);

    GmPllHotDlGn     = PARM(GmPllPrpGnI) - PARM(GmPllPrpGnR);

    // Phase Imbalance Thresholds*/
    // Quantization level line to line voltage is (2*LineVanVcoSc)/R_DelTm1
    // (2 quantas at LnDelTm1 rate
    // 1/sqrtf(3)--> line to neutral voltage
    // sqrtf(2) --> (2 sensors in quadrature)
    // 0.5 compensates for averager in line reconstruction
    V_Noise_Lvl_Ln = SQRT2*SQRT3_RCP*2.0F*PARM(GmVanVcoSc)*L_DelTm1Rcp*0.5F;
    SET_FV(GmPllIniThrs,  18.0F*4.307e-3F*L_VlnIn);/*18.0% imbalance = ???deg*/
    SET_FV(GmPllRunThrs,  1.73F*18.0F*4.307e-3F*PARM(L_VllIn));//1.73 times  LnPhImbThrs
                       //2.595 in 2.x instead of 1.73
    SET_FV(GmPllRcvThrs,  1.35F*18.0F*4.307e-3F*PARM(L_VllIn));//1.73 times  LnPhImbThrs

    SET_FV(GmPllErNoise, -PARM(GmPllIniThrs) +
          sqrtf(PARM(GmPllIniThrs)*PARM(GmPllIniThrs) + V_Noise_Lvl_Ln*V_Noise_Lvl_Ln));

    SET_FV(GmPllErSpill, 1.773e-5F*L_VlnIn*L_VlnIn);


    SET_FV(GmAcWdogTm,    4.0F);

    GmPhImbTmx       =  PARM(GmAcWdogTm);

    SET_FV(GmSyncInitTm, 1.0F);


    SET_FV(GmPllErrW,    54.0F);
    SET_FV(GmVxVyErrW,  300.0F);
    SET_FV(GmAcLineWUp, 200.0F);
    SET_FV(GmAcLineWDn, 2.0F);

    if( !PARM(SeriesCmpEnb) )
    {
        if(SimAcclAct)
        {
            SET_FV(GmPllGnSwTm, 0.060F);
            SET_FV(GmPllPrvTm, 0.120F);
        }
        else
        {
            SET_FV(GmPllGnSwTm, 0.060F);
            SET_FV(GmPllPrvTm, 0.120F);
        }
    }
    else
    {
        if(SimAcclAct)
        {
            SET_FV(GmPllGnSwTm, 0.090F);
            SET_FV(GmPllPrvTm, 0.180F);
        }
        else
        {
            SET_FV(GmPllGnSwTm, 0.090F); // These two parameters have historically had values of 0.060sec and 0.180sec.  They were changed on the 1.6e program in 11/2013 because
            SET_FV(GmPllPrvTm, 0.180F);  // WindTrap showed the 0.090 and 0.180 values to be of benefit to prevent limit-cycling during SC insertion.   WindTrap had been using
                                         // the 0.090/0.180 value for quite some time, but the reason is not understood

        }
    }


    SET_FV(GmLineVShrTm, 0.005F);
    SET_FV(GmLnVShrOfTm, 0.003F);
    SET_FV(GmLnVShrOfT2, 0.040F);

    SET_FV(GmVLoPulseTm, 0.015F);
    SET_FV(GmVLoNotTm,   0.500F);
    SET_FV(GmLineVShrOn, 0.100F);
    SET_FV(GmLineVShrOf, 0.300F);
    SET_FV(GmLineVShrO2, 0.15F);


    SET_FV(GmLineVHiOn, 1.20F);
    SET_FV(GmLineVHiOff,1.15F);
    SET_FV(GmLineVHiTm, 0.010F);

    SET_FV(GmLineVLowOn,0.65F);
    SET_FV(GmLineVLowOf,0.70F);
    SET_FV(GmLineVLowTm, 0.008F); //50hz, .9pu line (min low line)
    SET_FV(GmFixFrqMxTm, 1.5F);

    SET_FV(GmPhsVoltW,DelTm3Rcp);
    SET_FV(GmPhsCurrW,20.0F);
    SET_FV(GmVxFilW,20.0F);
    SET_FV(GmVyFilW,20.0F);
    SET_FV(GmIxFilW,20.0F);
    SET_FV(GmIyFilW,20.0F);
    SET_FV(GmFreqFilW,20.0F);

    SET_FV(GmPwrFilW,20.0F);
    SET_FV(GmQPwrFilW,20.0F);

    SET_FV(GmPwrFSignTh,40000.0F); //Use 40.0kvar as the threshold to determine sign by sign of command

    SET_FV(GmNegSeqScl, 100.0F); //100 to convert to %

    //Grid monitor fast over-voltage
    SET_FV(GmVMgFstUpW, 1.0F*L_DelTm1Rcp);
    SET_FV(GmVMgFstDnW, 50.0F);
    if ( (PARM(SoftwareIPN) >= DFIG02_2400_5060_A) )
    {
       SET_FV(GmFastOVW,   100.0F);
       SET_FV(GmFastOVThrs,1.65F);
    }
    else
    {
       SET_FV(GmFastOVW,   50.0F);
       SET_FV(GmFastOVThrs,1.42F);
    }

    //SET_FV(GmFastOVW,  DelTm2Rcp);      // temporary settings until
    //SET_FV(GmFastOVThrs, 1.20);         // open grid detection is improved


    // Volt-sec calculation- Set volt seconds for a point around 1.3pu
    // Use excel spreadsheet, "OVVSec" to find best settings
    // Use 1.28 as //See GmProt_050
    // VSecVThr = 1.3  -- Voltage used for Vsec threhosld
    // VLowThr  = 1.28 -- Voltage used for spillover
    //
    SET_FV(GmCnvOVHThr, 1.30F);
    SET_FV(GmCnvOVLThr, 1.29F);
    SET_FV(GmCnvOVLTm, 0.120F);
    VSecThr = (PARM(GmCnvOVHThr) - PARM(GmCnvOVLThr))*PARM(GmCnvOVLTm);

    SET_FV(GmCnvOVMin, PARM(GmCnvOVLThr));
    SET_FV(GmCnvOVThr, VSecThr);
    GmCnvOVThrx  = PARM(GmCnvOVThr);


    SET_FV(GmCVAsmThrs1, 4.90F);
    SET_FV(GmCVAsmMin1, 4.10F);
    SET_FV(GmCVAsmAlmOn, 4.102F);
    SET_FV(GmCVAsmAlmOf, 4.00F);
    SET_FV(GmCVAsmW1, 0.00028F );
    SET_FV(GmCVAsmThrs2, 9.50F);
    SET_FV(GmCVAsmMin2, 8.00F);
    SET_FV(GmCVAsmW2, 0.013F);
    SET_FV(GmVAsmAlmInh, false);
    SET_FV(GmPhShortOn, 45.0F);
    SET_FV(GmPhShortOff, 44.0F);
    SET_FV(GmAsymRTTm, .030F);
    SET_FV(GmAsymRcvTm, .070F);
    SET_FV(GmLvrtIpTm, 0.015F);
    SET_FV(GmLvrtRcvTm, 0.250F);

    GmCVAsmMinFrc= modf(PARM(GmCVAsmMin1), &ipart);
    GmCVAsmMinInt= ipart;

    GmVMgFstUpGn = PARM(GmVMgFstUpW)*L_DelTm1;//should be 1.0
    GmVMgFstDnGn = PARM(GmVMgFstDnW)*L_DelTm1;
    GmVMagFstScl = 1.0F/(3.0F*PARM(L_VllIn)*PARM(L_VllIn));
    GmFstOVFilGn = DelTm2*PARM(GmFastOVW);
    GmFstOVThrs  = PARM(GmFastOVThrs);
    GmCnvOVMinx  = PARM(GmCnvOVMin);

    SET_FV(GmWPllW4RAF,  100.0F);    //   Filter omega for PLLW for RAF
    SET_FV(GmRAFCntDynEnb, true);    //   Enable GmRAFCnt calc in run-time

    if (!PARM(GmRAFCntDynEnb) || (GmRAFCnt == 0))
    {
        //RAF calculations for line feedback processing
        GmRAFCntFlt = 0.5F/(PARM(L_FrqInput))*DelTm2Rcp;        // DelTm2 is lower limited in Y0INIT.h to protect
        GmRAFCnt    = ( ((int) (GmRAFCntFlt))  + 2);          // this calculation
        if ( GmRAFCnt > (GM_RAF_ARRAY_SIZE))
        {
            GmRAFCnt = (GM_RAF_ARRAY_SIZE);
        }
        GmRAFGnInnr = 1.0F/GmRAFCntFlt;
        GmRAFGnOutr = (1.0F - (  GmRAFGnInnr* (float)(GmRAFCnt - 2)  )   )/2.0F;
        GmRAFGnDiff = GmRAFGnInnr - GmRAFGnOutr;
    }

    SET_FV(GmIyInstCalcVmin,    1.0F);      // GmIyInst Calculation min grid voltage

    //Device open thresholds based on voltage matches.
    SET_FV(GmLVMatchThr,    0.31F*L_VlnIn);
    SET_FV(GmSVMatchThr,    0.31F*L_VlnIn); //raised to .31 because of breaker tendancy to loose contact pressure at heavy loads.
    SET_FV(LSVMatchThr,     0.31F*L_VlnIn);
    SET_FV(GmSVMatchTh2,    0.142F*L_VlnIn);

    GmLVmtchThr      = (PARM(GmLVMatchThr)   * PARM(GmLVMatchThr));
    GmSVmtchThr1     = (PARM(GmSVMatchThr)   * PARM(GmSVMatchThr));
    LSVmtchThr       = (PARM(LSVMatchThr)    * PARM(LSVMatchThr));
    GmSVmtchThr2     = (PARM(GmSVMatchTh2)   * PARM(GmSVMatchTh2));

    SET_FV(GmLVMtchErrW, 250.0F);
    SET_FV(LSVMtchErrW,  250.0F);

    GmLVmtchErGn   = PARM(GmLVMtchErrW)* DelTm2;
    LSVmtchErGn    = PARM(LSVMtchErrW) * DelTm2;
    XfmrSecVmtchGn = PARM(LSVMtchErrW) * DelTm2;

    SET_FV(GmSVMtchIThr, 1000.0F*SQRT2);// 1,000 Amps rms

    if ( PARM(SimModeReq))
    {
       SET_FV(GmSVllFbkLag, 0.0F);
    }
    else
    {
        switch(PARM(SoftwareIPN))
        {
        case DFIG02_1500_5060_A:
        case DFIG02_1500_5060_B:
        case DFIG02_1850_5060_C:
        case DFIG02_1850_5060_D:
        case DFIG02_1850_5060_E:
            SET_FV(GmSVllFbkLag,    87.536e-6F * PI2 * PARM(SysFreqNom));
            break;
        case DFIG02_2400_5060_A:
            SET_FV(GmSVllFbkLag,    0.0F);     // all pll fbks are thru the same pwa type
            break;
        default:
            SET_FV(GmSVllFbkLag,    0.0F);
            break;
        }
    }

    SET_FV(GmAngShiftT2, L_DelTm1* PI2 * PARM(SysFreqNom));

    IInitial   = 40.0e3F/(PARM(L_VllInput)*SQRT3*0.9F);//low line
    IThr       = 60.0e3F/(PARM(L_VllInput)*SQRT3*0.9F);//low line
    IFinal     = 250.0e3F/(PARM(L_VllInput)*SQRT3*0.9F);//low line
    IResponseT = 1.0F;
    //Isq = (IInital^2 + (IFinal^2 - IInitial^2)e-(t/tau)
    //Itau = -t/Ln((IFinal^2-Iniital^2)/(IFinal^2 - Initial^2))
    //designed to give a 1 second trip at .9pu low line and stepping from 40kva to 250kva with a
    ITau = -IResponseT/logf((IFinal*IFinal - IThr*IThr)/(IFinal*IFinal - IInitial*IInitial));

    SET_FV(GmSLIErrTau, ITau);
    GmSLIErrGn = DelTm3/PARM(GmSLIErrTau);  //60kva theshold

    SET_FV(GmSLIErrThr,     100.0F);    //over-ridden based on lab LVRT experience
    GmSLIErrThr = PARM(GmSLIErrThr);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                    Grid monitor protection final values
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Capability curve time thresholds have a 5% padded to their values
    // For example, GmUV2CapTm = 10.0 * 1.05 = 10.50 sec.

    // Under-voltage capability curve.
    if( !PARM(AVRPresent) )
    {
        if ( (PARM(SoftwareIPN) >= DFIG02_2400_5060_A) )
        {
           //Converter OF,UF,OV,UV protection based on grid monitor feedbacks
           SET_FV(GmOVTrpThrs1, 1.14F);
           SET_FV(GmOVWV1,      0.0014F);
           SET_FV(GmOVMin1,     1.13F);

           SET_FV(GmOVTrpThrs2, 1.17F);
           SET_FV(GmOVWV2,      0.013F);
           SET_FV(GmOVMin2,     1.15F);

           SET_FV(GmOVTrpThrs3, 1.22F);
           SET_FV(GmOVWV3,      0.30F);
           SET_FV(GmOVMin3,     1.15F);

           SET_FV(GmOVTrpThrs4, 1.62F);
           SET_FV(GmOVWV4,      25.0F);
           SET_FV(GmOVMin4,     1.35F);

           if (!PARM(VoltExtendOpt))
           {
              if ( L_LTestIp)
              {
                 SET_FV(GmUVTrpThrs1, 0.10F);
              }
              else
              {
                 SET_FV(GmUVTrpThrs1, 0.89F);
              }
              SET_FV(GmUVWV1,      0.0001F);
              SET_FV(GmUVMax1,     0.89255F);
           }
           else
           {
              if ( L_LTestIp)
              {
                 SET_FV(GmUVTrpThrs1, 0.10F);
              }
              else
              {
                 SET_FV(GmUVTrpThrs1, 0.87F);
              }
              SET_FV(GmUVWV1,      0.00019F);
              SET_FV(GmUVMax1,     0.87255F);
           }
        }
        else
        {
           //Converter OF,UF,OV,UV protection based on grid monitor feedbacks
           SET_FV(GmOVTrpThrs1, 1.12F);
           SET_FV(GmOVWV1,      0.14F);
           SET_FV(GmOVMin1,     1.11F);

           // Disabled by setting GmOVMin2 and GmOVTrpThrs2 to a large number...not needed for base package
           SET_FV(GmOVTrpThrs2, 100.0F);
           SET_FV(GmOVWV2,      1.22F);
           SET_FV(GmOVMin2,     10.0F);

           SET_FV(GmOVTrpThrs3, 1.175F);
           SET_FV(GmOVWV3,      1.22F);
           SET_FV(GmOVMin3,     1.15F);

           SET_FV(GmOVTrpThrs4, 100.0F);
           SET_FV(GmOVWV4,      25.0F);
           SET_FV(GmOVMin4,     10.0F);

           if ( L_LTestIp)
           {
              SET_FV(GmUVTrpThrs1, 0.10F);
           }
           else
           {
              SET_FV(GmUVTrpThrs1, 0.89F);
           }

           SET_FV(GmUVWV1,      0.0001F);
           SET_FV(GmUVMax1,     0.89255F);
        }

        if ( L_LTestIp)
        {
           SET_FV(GmUVTrpThrs2, 0.200F);
        }
        else
        {
           SET_FV(GmUVTrpThrs2, 0.828F);
        }
        SET_FV(GmUVWV2,      0.01F);
        SET_FV(GmUVMax2,     0.84F);


        SET_FV(GmCfgSelFV, PARM(GmCfgSel));

        SET_FV(GmUVMaxLvl,  0.99F);
        SET_FV(GmUV1Cap,    0.87F);
        SET_FV(GmUV1CapTm,  605.0F);
        SET_FV(GmUV2Cap,    0.85F);
        SET_FV(GmUV2CapTm,  10.50F);
        SET_FV(GmUV3Cap,    0.75F);
        SET_FV(GmUV3CapTm,  3.15F);
        SET_FV(GmUV4Cap,    0.20F);
        SET_FV(GmUV4CapTm,  UV_OV_LVL_TM1);
        SET_FV(GmUV5Cap,    -1.0F);
        SET_FV(GmUV5CapTm,  0.525F);
        SET_FV(GmUV6Cap,     -1.0F);
        SET_FV(GmUV6CapTm, 0.2625F);
        SET_FV(GmUVCvUpCap, 0.70F);
        SET_FV(GmUVCvUCapTm, 3.15F);
        SET_FV(GmUVCvLwrCap,0.20F);   //based on curve of (0%, 200msec; 80%, 3.0sec)
        SET_FV(GmUVCvLCapTm, UV_OV_LVL_TM1);
    }
    else
    {
        //Converter OF,UF,OV,UV protection based on grid monitor feedbacks
        SET_FV(GmOVTrpThrs1, 1.12F);
        SET_FV(GmOVWV1,      0.0055F);
        SET_FV(GmOVMin1,     1.10F);

        SET_FV(GmOVTrpThrs2, 1.2175F);
        SET_FV(GmOVWV2,      0.35F);
        SET_FV(GmOVMin2,     1.14F);

        SET_FV(GmOVTrpThrs3, 1.285F);
        SET_FV(GmOVWV3,      3.6F);
        SET_FV(GmOVMin3,     1.215F);

        if ( L_LTestIp)
        {
           SET_FV(GmUVTrpThrs1, 0.10F);
        }
        else
        {
           SET_FV(GmUVTrpThrs1, 0.85F);
        }
        SET_FV(GmUVWV1,      0.00001F);
        SET_FV(GmUVMax1,     0.85185F);

        if ( L_LTestIp)
        {
           SET_FV(GmUVTrpThrs2, 0.20F);
        }
        else
        {
           SET_FV(GmUVTrpThrs2, 0.78F);
        }
        SET_FV(GmUVWV2,      0.0166F);
        //TODO, TH, Hold off
        SET_FV(GmUVMax2,     0.80F);

        SET_FV(GmCfgSelFV, PARM(GmCfgSelOpt));

        SET_FV(GmUVMaxLvl,  0.99F);
        SET_FV(GmUV1Cap,    0.85F);
        SET_FV(GmUV1CapTm,  3610.0F);
        SET_FV(GmUV2Cap,    0.80F);
        SET_FV(GmUV2CapTm,  10.50F);
        SET_FV(GmUV3Cap,    0.75F);
        SET_FV(GmUV3CapTm,  3.15F);
        SET_FV(GmUV4Cap,    0.20F);
        SET_FV(GmUV4CapTm,  UV_OV_LVL_TM1);
        SET_FV(GmUV5Cap,    -1.0F);
        SET_FV(GmUV5CapTm,  0.525F);
        SET_FV(GmUV6Cap,     -1.0F);
        SET_FV(GmUV6CapTm, 0.2625F);
        SET_FV(GmUVCvUpCap, 0.75F);
        SET_FV(GmUVCvUCapTm, 3.15F);
        SET_FV(GmUVCvLwrCap,0.20F);   //based on curve of (0%, 200msec; 80%, 3.0sec)
        SET_FV(GmUVCvLCapTm, UV_OV_LVL_TM1);
    }

    GmUVMaxLvl    =  PARM(GmUVMaxLvl);
    GmUV1Cap      =  PARM(GmUV1Cap);
    GmUV1CapTm    =  PARM(GmUV1CapTm);
    GmUV2Cap      =  PARM(GmUV2Cap);
    GmUV2CapTm    =  PARM(GmUV2CapTm);
    GmUV3Cap      =  PARM(GmUV3Cap);
    GmUV3CapTm    =  PARM(GmUV3CapTm);
    GmUV4Cap      =  PARM(GmUV4Cap);
    GmUV4CapTm    =  PARM(GmUV4CapTm);
    GmUVCvUCap    =  PARM(GmUVCvUpCap);
    GmUVCvUCapTm  =  PARM(GmUVCvUCapTm);
    GmUVCvLCap    =  PARM(GmUVCvLwrCap);
    GmUVCvLCapTm  =  PARM(GmUVCvLCapTm);

    GmOVMinFrc      = modf( (100.0F*PARM(GmOVMin1)), &ipart);
    GmOVMinInt      = ipart;

    GmCnvUV1Gn  = (double) DelTm3*(double)PARM(GmUVWV1);
    GmCnvOV1GnD = (double) DelTm3*(double)PARM(GmOVWV1);
    GmCnvOV1Gn  = (float) GmCnvOV1GnD;
    GmCnvOV2Gn  = DelTm3 * PARM(GmOVWV2);
    GmCnvOV3Gn  = DelTm3 * PARM(GmOVWV3);
    GmCnvOV4Gn  = DelTm3 * PARM(GmOVWV4);

    SET_FV(VmagStpDlyW, 10.0F);
    VmagStpDlyGn = DelTm2 * PARM(VmagStpDlyW);

    SET_FV(VmagStpDlyThr, 1.15F);


    GmUVCvSlopeC= (PARM(GmUVCvUpCap) - PARM(GmUVCvLwrCap))/ (PARM(GmUVCvUCapTm) - PARM(GmUVCvLCapTm));


    SET_FV(GmUVTrpInh,  false);

    if ( PARM(GmUVTrpInh))
    {
       SET_FV(GmUV1TrpInh, true);
       SET_FV(GmUV2TrpInh, true);
       SET_FV(GmUV3TrpInh, true);
       SET_FV(GmUV4TrpInh, true);
       SET_FV(GmUV5TrpInh, true);
       SET_FV(GmUV6TrpInh, true);
       SET_FV(GmUVCvTrpInh,true);
    }
    else
    {
       SET_FV(GmUV1TrpInh, false);
       SET_FV(GmUV2TrpInh, false);
       SET_FV(GmUV3TrpInh, false);
       SET_FV(GmUV4TrpInh, false);
       SET_FV(GmUV5TrpInh, false);
       SET_FV(GmUV6TrpInh, false);
       SET_FV(GmUVCvTrpInh,false);
    }


    //Grid monitor uv/ov alarm off thresholds and times

    SET_FV(GmUVAlmOff, 0.90F);
    SET_FV(GmUVAlmOffTm, 0.10F);
    SET_FV(GmOVAlmOff, 1.10F);
    SET_FV(GmOVAlmOffTm, 0.10F);


    //Low voltage "curve" final values
    SET_FV(GmUVCvWV    ,30.00F);
    //calculate slop and y intercept for linear and dynamic volt*sec trip threshold
    GmUVCvSlope= (PARM(GmUVCvUpr) - PARM(GmUVCvLwr))/ (PARM(GmUVCvUprTm) - PARM(GmUVCvLwrTm));
    Slope2xRcp = 1.0F/(2.0F*GmUVCvSlope);

    GmUVCConstK1 = 1.0F - 2.0F*PARM(GmUVCvUpr) + 2*GmUVCvSlope*PARM(GmUVCvUprTm);
    GmUVCvConstA =  -Slope2xRcp;
    GmUVCvConstB =  (1.0F - GmUVCConstK1)*Slope2xRcp;
    GmUVCvConstC =  GmUVCConstK1*Slope2xRcp;
    GmUVCvConstD =  PARM(GmUVCvUpr) - GmUVCvSlope*PARM(GmUVCvUprTm);
    GmUVCVSecThr = GmUVCvConstA*(PARM(GmUVCvUpr)*PARM(GmUVCvUpr)) + GmUVCvConstB*PARM(GmUVCvUpr) + GmUVCvConstC;
    GmUVCvVSThr3 = GmUVCVSecThr;
    GmUVCvVSThr2 = GmUVCvVSThr3;
    GmUVCvVSThr1 = GmUVCvVSThr2;




    /////////////////////////////////////////
    // Over-voltage capability curve.
    /////////////////////////////////////////
    if (  !PARM(AVRPresent) )
    {
       if (PARM(SoftwareIPN) <  DFIG02_2400_5060_A)
       {
           SET_FV(GmOVMinLvl,  1.010F);
           SET_FV(GmOV1Cap,    1.110F);
           SET_FV(GmOV1CapTm,  315.0F);
           SET_FV(GmOV2Cap,    1.150F);
           SET_FV(GmOV2CapTm,  0.5250F);
           SET_FV(GmOV3Cap,    1.175F);
           SET_FV(GmOV3CapTm,  0.230F);
           SET_FV(GmOV4Cap,    1.200F);
           SET_FV(GmOV4CapTm,  0.120F);
       }
       else
       {
           SET_FV(GmOVMinLvl,  1.010F);
           SET_FV(GmOV1Cap,    1.130F);
           SET_FV(GmOV1CapTm,  315.0F);
           SET_FV(GmOV2Cap,    1.150F);
           SET_FV(GmOV2CapTm,  31.50F);
           SET_FV(GmOV3Cap,    1.200F);
           SET_FV(GmOV3CapTm,  2.100F);
           SET_FV(GmOV4Cap,    1.250F);
           SET_FV(GmOV4CapTm,  0.510F);
       }
    }
    else
    {
        SET_FV(GmOVMinLvl,  1.01F);
        SET_FV(GmOV1Cap,    1.11F);
        SET_FV(GmOV1CapTm,  31.5F);
        SET_FV(GmOV2Cap,    1.20F);
        SET_FV(GmOV2CapTm,  2.10F);
        SET_FV(GmOV3Cap,    1.25F);
        SET_FV(GmOV3CapTm,  0.2625F);
        SET_FV(GmOV4Cap,    1.30F);
        SET_FV(GmOV4CapTm,  0.12F);
    }

    GmOVMinLvl    =  PARM(GmOVMinLvl);
    GmOV1Cap      =  PARM(GmOV1Cap);
    GmOV1CapTm    =  PARM(GmOV1CapTm);
    GmOV2Cap      =  PARM(GmOV2Cap);
    GmOV2CapTm    =  PARM(GmOV2CapTm);
    GmOV3Cap      =  PARM(GmOV3Cap);
    GmOV3CapTm    =  PARM(GmOV3CapTm);
    GmOV4Cap      =  PARM(GmOV4Cap);
    GmOV4CapTm    =  PARM(GmOV4CapTm);




/////////////////////////////////////////
// Under-frequency capability curve.
/////////////////////////////////////////


    SET_FV(GmUFMaxLvl,  0.99F);
    SET_FV(GmUF1Cap,    0.94F);
    SET_FV(GmUF1CapTm,  UF_OF_LVL1_TM);
    SET_FV(GmUF2Cap,    0.90F);
    SET_FV(GmUF2CapTm,  1.05F);
    SET_FV(GmUF3Cap,    0.85F);
    SET_FV(GmUF3CapTm,  0.2625F);



    GmUFMaxLvl  =  PARM(GmUFMaxLvl);
    GmUF1Cap    =  PARM(GmUF1Cap);
    GmUF1CapTm  =  PARM(GmUF1CapTm);
    GmUF2Cap    =  PARM(GmUF2Cap);
    GmUF2CapTm  =  PARM(GmUF2CapTm);
    GmUF3Cap    =  PARM(GmUF3Cap);
    GmUF3CapTm  =  PARM(GmUF3CapTm);



    SET_FV(GmUFTrpInh,  false);
    if ( PARM(GmUFTrpInh))
    {
      SET_FV(GmUF1TrpInh, true);
      SET_FV(GmUF2TrpInh, true);
      SET_FV(GmUF3TrpInh, true);
    }
    else
    {
      SET_FV(GmUF1TrpInh, false);
      SET_FV(GmUF2TrpInh, false);
      SET_FV(GmUF3TrpInh, false);
    }



/////////////////////////////////////////
// Over-frequency capability curve.
/////////////////////////////////////////

    SET_FV(GmOFMinLvl,  1.01F);
    SET_FV(GmOF1Cap,    1.06F);
    SET_FV(GmOF1CapTm,  UF_OF_LVL1_TM);
    SET_FV(GmOF2Cap,    1.10F);
    SET_FV(GmOF2CapTm,  0.2625F);

    SET_FV(GmOF1TrpInh, false);
    SET_FV(GmOF2TrpInh, false);

    GmOFMinLvl   =  PARM(GmOFMinLvl);
    GmOF1Cap     =  PARM(GmOF1Cap);
    GmOF2Cap     =  PARM(GmOF2Cap);
    GmOF1CapTm   =  PARM(GmOF1CapTm);
    GmOF2CapTm   =  PARM(GmOF2CapTm);

    SET_FV(L_FrqTol,    5.0F);     // 5Hz tolerance


    SET_FV(GmOFTrpThrs1, 1.061F);
    SET_FV(GmUFTrpThrs1, 0.939F);
    SET_FV(GmUFOFW1,     0.011F);
    SET_FV(GmOFTrpThrs2, 1.11F);
    SET_FV(GmUFTrpThrs2, 0.89F);
    SET_FV(GmUFOFW2,     0.022F);
    SET_FV(GmOFMin,      1.105F);
    SET_FV(GmUFMax,      0.895F);

/////////////////////////////////////////
//Excessive power
/////////////////////////////////////////
    SET_FV(GmExPwrFilW, DelTm3Rcp);
    GmExPwrFilGn = DelTm3 * PARM(GmExPwrFilW);              // this gives a filter gain > 1 when GmExPwrFilW = 100 r/s
    GmExPwrFilGn = MIN(1.0F, GmExPwrFilGn);

    SET_FV(GmExPwrOnThr,    1.148F*PARM(SysPower));
    SET_FV(GmExPwrOfThr,    0.95679F*PARM(SysPower));

/////////////////////////////////////////

/////////////////////////////////////////
    //Reverse Power
/////////////////////////////////////////
    SET_FV(GmRevPwrFilW, 10.0F);                // Allow 16kw*sec for 2:1 margin of DC buss step from 850vdc to 1125vdc, .024F
    GmRvPwrFilGn = DelTm3*PARM(GmRevPwrFilW);   // (C*(VdcT2*VdcT2)/2) - (C*(VdcT2*VdcT2)/2)
                                                // 15,188 watt*sec     - 8,670 watt*sec = 6,512 watt*sec
    // This value must be updated if the L_MAClsIMsCt or the filter GmRevPwrFilW is changed!
    SET_FV(GmRevPwrThrDbCel, (-SQRT2*PARM(L_VllRms)*1.10F)* (SQRT2*PARM(L_VllRms)*1.10F)/PARM(BrgDbR) * 0.3935F);  // 1.10 is couting for the high line voltage
                                                                                                                   // 0.3935 = 1-exp(-0.5), count for the filter action,
                                                                                                                   // where 0.5=t/RevPwrtau= PARM(L_MAClsIMsCt)*DelTm1A/0.1

/////////////////////////////////////////       //  1.0/(6,512/150,000) = 23 rad/sec ....set to 10 for near 2:1 margin
// Feedback Verification checks--final values   //  Still near 2:1 margin with cap at 20% high (calculate 19 rad/sec)
/////////////////////////////////////////       //  All this assumes a trip threshold of 150,000 watts

    SET_FV(GmAuxPwrW,   5.0F);
    SET_FV(GmAuxQPwrW,  5.0F);




    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                       Grid monitor protection User Parameter Selection Based on P.GmCfgSel
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////
    // Grid Monitor User configuration selection
    /////////////////////////////////////////////////////

    // No longer using division of frequency levels by L_FrqInput because the result
    // does not round off properly, causing mismatch issues with the default values.
    // For example, 55.0/60.0 is 0.9166666667 and the default is set to 0.9167
    if ( PARM(L_FrqInput) == 50.0F )
    {
         GmUF1TrpLvl = 0.94F;
         GmUF2TrpLvl = 0.90F;
         GmOF1TrpLvl = 1.06F;
         GmOF2TrpLvl = 1.10F;
    }
    else // if ( PARM(L_FrqInput) == 60.0 )
    {
         GmUF1TrpLvl = 0.95F;
         GmUF2TrpLvl = 0.91667F;
         GmOF1TrpLvl = 1.05F;
         GmOF2TrpLvl = 1.08333F;
    }

    if ( PARM(GmCfgSelFV) == GM_WIZ_SVRT) //Legacy
    {
        //Under-Voltage
        SET_PARM(GmUVAlmThr  , 0.890F);
        SET_PARM(GmUVAlmTm   , 0.100F);
        SET_PARM(GmUV1TrpThr , 0.890F);
        SET_PARM(GmUV1TrpTm  , 600.0F);
        SET_PARM(GmUV2TrpThr , 0.850F);
        SET_PARM(GmUV2TrpTm  , 10.00F);
        SET_PARM(GmUV3TrpThr , 0.750F);
        SET_PARM(GmUV3TrpTm  ,  1.00F);
        SET_PARM(GmUV4TrpThr ,  0.70F);
        SET_PARM(GmUV4TrpTm  , 0.030F);
        SET_PARM(GmUV5TrpThr,   -1.0F);
        SET_PARM(GmUV5TrpTm,   0.525F);
        SET_PARM(GmUV6TrpThr,   -1.0F);
        SET_PARM(GmUV6TrpTm,  0.2625F);
        SET_PARM(GmUVCvUpr   ,  -1.0F);
        SET_PARM(GmUVCvUprTm ,   1.0F);
        SET_PARM(GmUVCvLwr   ,  -1.0F);
        SET_PARM(GmUVCvLwrTm ,   1.0F);

        //Over-Voltage
        SET_PARM(GmOVAlmThr  ,  (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOVAlmTm   ,  0.10F);
        SET_PARM(GmOV1TrpThr ,  (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOV1TrpTm  ,  1.00F);
        SET_PARM(GmOV2TrpThr ,  1.15F);
        SET_PARM(GmOV2TrpTm  ,  0.10F);
        SET_PARM(GmOV3TrpThr ,   5.0F);
        SET_PARM(GmOV3TrpTm  ,  -1.0F);
        SET_PARM(GmOV4TrpThr ,   5.0F);
        SET_PARM(GmOV4TrpTm  ,  -1.0F);


        //Under-Frequency
        SET_PARM(GmUFAlmThr  ,  0.9583F); //57.6hz @ 60hz
        SET_PARM(GmUFAlmTm   ,   0.100F);
        SET_PARM(GmUF1TrpThr ,  0.9583F); //57.6hz @ 60hz
        SET_PARM(GmUF1TrpTm  ,   10.00F);
        SET_PARM(GmUF2TrpThr , 0.94167F);//56.5hz @60hz
        SET_PARM(GmUF2TrpTm  ,    1.00F);
        SET_PARM(GmUF3TrpThr ,   -1.00F);
        SET_PARM(GmUF3TrpTm  ,    1.00F);

        //Over-Frequency
        SET_PARM(GmOFAlmThr  ,   1.025F);
        SET_PARM(GmOFAlmTm   ,   0.100F);
        SET_PARM(GmOF1TrpThr ,   1.025F);  //61.5hz @60hz
        SET_PARM(GmOF1TrpTm  ,    30.0F);
        SET_PARM(GmOF2TrpThr , 1.04167F);//62.5hz @ 60hz
        SET_PARM(GmOF2TrpTm  ,    1.00F);

        //Voltage Asymmetry
        SET_PARM(GmVAsyTrpThr,    8.10F);
        SET_PARM(GmVAsyTrpTm ,   10.01F);
        SET_PARM(GmVAsy2TrpThr,   8.10F);
        SET_PARM(GmVAsy2TrpTm,    10.0F);
        SET_PARM(GmVAsyAlmTm ,    0.10F);

        SET_PARM(GmExPwrTrpTh,   2.0F*PARM(SysPower));


        GmConfig = GM_WIZ_SVRT;
        CCUCnfgBits.Svrt          = true;
        CCUCnfgBits.Lvrt          = false;
        CCUCnfgBits.Zvrt          = false;
        CCUCnfgBits.Cvrt          = false;
        CCUCnfgBits.ZvrtGen2Base  = false;
        CCUCnfgBits.ZvrtGen3Base  = false;
        CCUCnfgBits.ZvrtGen2Opt   = false;
        //CCUCnfgBits.MaxCapVals    = false;


    }
    else if ( PARM(GmCfgSelFV) == GM_WIZ_LVRT) //Legacy
    {
        //Under-Voltage
        SET_PARM(GmUVAlmThr  ,   0.890F);
        SET_PARM(GmUVAlmTm   ,   0.100F);
        SET_PARM(GmUV1TrpThr ,   0.890F);
        SET_PARM(GmUV1TrpTm  ,   600.0F);
        SET_PARM(GmUV2TrpThr ,   0.850F);
        SET_PARM(GmUV2TrpTm  ,   10.00F);
        SET_PARM(GmUV3TrpThr ,   0.750F);
        SET_PARM(GmUV3TrpTm  ,    2.50F);
        SET_PARM(GmUV4TrpThr ,    0.15F);
        SET_PARM(GmUV4TrpTm  ,   0.030F);
        SET_PARM(GmUV5TrpThr,     -1.0F);
        SET_PARM(GmUV5TrpTm,     0.525F);
        SET_PARM(GmUV6TrpThr,     -1.0F);
        SET_PARM(GmUV6TrpTm,    0.2625F);

        SET_PARM(GmUVCvUpr   ,    0.75F);
        SET_PARM(GmUVCvUprTm ,    2.50F);
        SET_PARM(GmUVCvLwr   ,    0.15F);
        SET_PARM(GmUVCvLwrTm ,   0.625F);

        //Over-Voltage
        SET_PARM(GmOVAlmThr  ,    (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOVAlmTm   ,    1.00F);
        SET_PARM(GmOV1TrpThr ,    (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOV1TrpTm  ,    1.00F);
        SET_PARM(GmOV2TrpThr ,    1.15F);
        SET_PARM(GmOV2TrpTm  ,    0.10F);
        SET_PARM(GmOV3TrpThr ,     5.0F);
        SET_PARM(GmOV3TrpTm  ,    -1.0F);
        SET_PARM(GmOV4TrpThr ,     5.0F);
        SET_PARM(GmOV4TrpTm  ,    -1.0F);


        //Under-Frequency
        SET_PARM(GmUFAlmThr  ,    0.96F); //57.6hz @ 60hz
        SET_PARM(GmUFAlmTm   ,   0.100F);
        SET_PARM(GmUF1TrpThr ,    0.94F); //57.6hz @ 60hz
        SET_PARM(GmUF1TrpTm  ,   10.01F);
        SET_PARM(GmUF2TrpThr ,    0.90F);//56.5hz @60hz
        SET_PARM(GmUF2TrpTm  ,    1.01F);
        SET_PARM(GmUF3TrpThr ,    0.85F);
        SET_PARM(GmUF3TrpTm  ,    0.26F);

        //Over-Frequency
        SET_PARM(GmOFAlmThr  ,   1.025F);
        SET_PARM(GmOFAlmTm   ,   0.100F);
        SET_PARM(GmOF1TrpThr ,    1.06F);  //61.5hz @60hz
        SET_PARM(GmOF1TrpTm  ,   10.01F);
        SET_PARM(GmOF2TrpThr ,    1.10F);  //62.5hz @ 60hz
        SET_PARM(GmOF2TrpTm  ,    0.26F);

        //Voltage Asymmetry
        SET_PARM(GmVAsyTrpThr,    8.10F);
        SET_PARM(GmVAsyTrpTm,    10.01F);
        SET_PARM(GmVAsy2TrpThr,   8.10F);
        SET_PARM(GmVAsy2TrpTm,    10.0F);
        SET_PARM(GmVAsyAlmTm,     0.10F);

        SET_PARM(GmExPwrTrpTh,   2.0F*PARM(SysPower));
        GmConfig = GM_WIZ_LVRT;
        CCUCnfgBits.Svrt          = false;
        CCUCnfgBits.Lvrt          = true;
        CCUCnfgBits.Zvrt          = false;
        CCUCnfgBits.Cvrt          = false;
        CCUCnfgBits.ZvrtGen2Base  = false;
        CCUCnfgBits.ZvrtGen3Base  = false;
        CCUCnfgBits.ZvrtGen2Opt   = false;
        //CCUCnfgBits.MaxCapVals    = false;

    }
    else if ( PARM(GmCfgSelFV) == GM_WIZ_ZVRT) //Legacy
    {
        //Under-Voltage
        SET_PARM(GmUVAlmThr    , 0.890F);
        SET_PARM(GmUVAlmTm     , 0.100F);
        SET_PARM(GmUV1TrpThr   , 0.890F);
        SET_PARM(GmUV1TrpTm    , 600.0F);
        SET_PARM(GmUV2TrpThr   ,  0.85F);
        SET_PARM(GmUV2TrpTm    ,  10.0F);
        SET_PARM(GmUV3TrpThr   , 0.750F);
        SET_PARM(GmUV3TrpTm    , 2.825F);
        SET_PARM(GmUV4TrpThr   ,  -1.0F);
        SET_PARM(GmUV4TrpTm    , 1.000F);
        SET_PARM(GmUV5TrpThr,     -1.0F);
        SET_PARM(GmUV5TrpTm,     0.525F);
        SET_PARM(GmUV6TrpThr,     -1.0F);
        SET_PARM(GmUV6TrpTm,    0.2625F);
        SET_PARM(GmUVCvUpr     , 0.750F);
        SET_PARM(GmUVCvUprTm   , 2.825F);
        SET_PARM(GmUVCvLwr     , 0.000F);
        SET_PARM(GmUVCvLwrTm   , 0.200F);

        //Over-Voltage
        SET_PARM(GmOVAlmThr    , (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOVAlmTm     , 1.00F);
        SET_PARM(GmOV1TrpThr   , (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOV1TrpTm    , 1.00F);
        SET_PARM(GmOV2TrpThr   , 1.15F);
        SET_PARM(GmOV2TrpTm    , 0.10F);
        SET_PARM(GmOV3TrpThr   ,  5.0F);
        SET_PARM(GmOV3TrpTm    , -1.0F);
        SET_PARM(GmOV4TrpThr   ,  5.0F);
        SET_PARM(GmOV4TrpTm    , -1.0F);


        //Under-Frequency
        SET_PARM(GmUFAlmThr   ,  0.96F);  //57.6hz @ 60hz
        SET_PARM(GmUFAlmTm    , 0.100F);
        SET_PARM(GmUF1TrpThr  ,  0.94F);  //57.6hz @ 60hz
        SET_PARM(GmUF1TrpTm   , 10.01F);
        SET_PARM(GmUF2TrpThr  ,  0.90F);  //56.5hz @60hz
        SET_PARM(GmUF2TrpTm   ,  1.01F);
        SET_PARM(GmUF3TrpThr  ,  0.85F);
        SET_PARM(GmUF3TrpTm   ,  0.26F);

        //Over-Frequency
        SET_PARM(GmOFAlmThr   , 1.025F);
        SET_PARM(GmOFAlmTm    , 0.100F);
        SET_PARM(GmOF1TrpThr  ,  1.06F);  //61.5hz @60hz
        SET_PARM(GmOF1TrpTm   , 10.01F);
        SET_PARM(GmOF2TrpThr  ,  1.10F);  //62.5hz @ 60hz
        SET_PARM(GmOF2TrpTm   ,  0.26F);

        //Voltage Asymmetry
        SET_PARM(GmVAsyTrpThr  ,  8.10F);
        SET_PARM(GmVAsyTrpTm   , 10.01F);
        SET_PARM(GmVAsy2TrpThr,   8.10F);
        SET_PARM(GmVAsy2TrpTm,    10.0F);
        SET_PARM(GmVAsyAlmTm   ,  0.10F);

        SET_PARM(GmExPwrTrpTh,   2.0F*PARM(SysPower));

        GmConfig = GM_WIZ_ZVRT;
        CCUCnfgBits.Svrt          = false;
        CCUCnfgBits.Lvrt          = false;
        CCUCnfgBits.Zvrt          = true;
        CCUCnfgBits.Cvrt          = false;
        CCUCnfgBits.ZvrtGen2Base  = false;
        CCUCnfgBits.ZvrtGen3Base  = false;
        CCUCnfgBits.ZvrtGen2Opt   = false;
        //CCUCnfgBits.MaxCapVals    = false;
    }
    else if ( PARM(GmCfgSelFV) == GM_WIZ_MAX_CONV)
    {
        //Under-Voltage
        SET_PARM(GmUVAlmThr    , 0.890F);
        SET_PARM(GmUVAlmTm     , 0.100F);
        SET_PARM(GmUV1TrpThr   , PARM(GmUV1Cap));
        SET_PARM(GmUV1TrpTm    , PARM(GmUV1CapTm));
        SET_PARM(GmUV2TrpThr   , PARM(GmUV2Cap));
        SET_PARM(GmUV2TrpTm    , PARM(GmUV2CapTm));
        SET_PARM(GmUV3TrpThr   , PARM(GmUV3Cap));
        SET_PARM(GmUV3TrpTm    , PARM(GmUV3CapTm));
        SET_PARM(GmUV4TrpThr   , PARM(GmUV4Cap));
        SET_PARM(GmUV4TrpTm    , PARM(GmUV4CapTm));
        SET_PARM(GmUV5TrpThr,    PARM(GmUV5Cap));
        SET_PARM(GmUV5TrpTm,     PARM(GmUV5CapTm));
        SET_PARM(GmUV6TrpThr,    PARM(GmUV6Cap));
        SET_PARM(GmUV6TrpTm,     PARM(GmUV6CapTm));
        SET_PARM(GmUVCvUpr     , PARM(GmUVCvUpCap));
        SET_PARM(GmUVCvUprTm   , PARM(GmUVCvUCapTm));
        SET_PARM(GmUVCvLwr     , PARM(GmUVCvLwrCap));
        SET_PARM(GmUVCvLwrTm   , PARM(GmUVCvLCapTm));

        //Over-Voltage
        SET_PARM(GmOVAlmThr    , (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOVAlmTm     ,  0.1F);
        SET_PARM(GmOV1TrpThr   , PARM(GmOV1Cap));
        SET_PARM(GmOV1TrpTm    , PARM(GmOV1CapTm));
        SET_PARM(GmOV2TrpThr   , PARM(GmOV2Cap));
        SET_PARM(GmOV2TrpTm    , PARM(GmOV2CapTm));
        SET_PARM(GmOV3TrpThr   , PARM(GmOV3Cap));
        SET_PARM(GmOV3TrpTm    , PARM(GmOV3CapTm));
        SET_PARM(GmOV4TrpThr   , PARM(GmOV4Cap));
        SET_PARM(GmOV4TrpTm    , PARM(GmOV4CapTm));

        //Under-Frequency
        SET_PARM(GmUFAlmThr    , 0.949F);
        SET_PARM(GmUFAlmTm     , 0.100F);
        SET_PARM(GmUF1TrpThr   , PARM(GmUF1Cap));
        SET_PARM(GmUF1TrpTm    , PARM(GmUF1CapTm));
        SET_PARM(GmUF2TrpThr   , PARM(GmUF2Cap));
        SET_PARM(GmUF2TrpTm    , PARM(GmUF2CapTm));
        SET_PARM(GmUF3TrpThr   , PARM(GmUF3Cap));
        SET_PARM(GmUF3TrpTm    , PARM(GmUF3CapTm));

        //Over-Frequency
        SET_PARM(GmOFAlmThr    , 1.051F);
        SET_PARM(GmOFAlmTm     , 0.100F);
        SET_PARM(GmOF1TrpThr   , PARM(GmOF1Cap));
        SET_PARM(GmOF1TrpTm    , PARM(GmOF1CapTm));
        SET_PARM(GmOF2TrpThr   , PARM(GmOF2Cap));
        SET_PARM(GmOF2TrpTm    , PARM(GmOF2CapTm));

        //Voltage Asymmetry
        SET_PARM(GmVAsyTrpThr  , 8.10F);
        SET_PARM(GmVAsyTrpTm  , 10.01F);
        SET_PARM(GmVAsy2TrpThr,  8.10F);
        SET_PARM(GmVAsy2TrpTm,   10.0F);
        SET_PARM(GmVAsyAlmTm   , 0.10F);

        SET_PARM(GmExPwrTrpTh,   2.0F*PARM(SysPower));

        GmConfig = GM_WIZ_MAX_CONV;
        CCUCnfgBits.Svrt          = false;
        CCUCnfgBits.Lvrt          = false;
        CCUCnfgBits.Zvrt          = false;
        CCUCnfgBits.Cvrt          = false;
        CCUCnfgBits.ZvrtGen2Base  = false;
        CCUCnfgBits.ZvrtGen3Base  = false;
        CCUCnfgBits.ZvrtGen2Opt   = false;
        //CCUCnfgBits.MaxCapVals    = true;


    }
    else if ( PARM(GmCfgSelFV) == GM_WIZ_ZVRT_GEN2_BASE)
    {
        //Under-Voltage
        SET_PARM(GmUVAlmThr    , 0.890F);
        SET_PARM(GmUVAlmTm     , 0.100F);
        SET_PARM(GmUV1TrpThr   , 0.890F);
        SET_PARM(GmUV1TrpTm    , 605.0F);
        SET_PARM(GmUV2TrpThr   ,  0.85F);
        SET_PARM(GmUV2TrpTm    , 10.50F);
        SET_PARM(GmUV3TrpThr   , 0.750F);
        SET_PARM(GmUV3TrpTm    ,  3.15F);
        SET_PARM(GmUV4TrpThr   ,  0.20F);
        SET_PARM(GmUV4TrpTm    , UV_OV_LVL_TM1);
        SET_PARM(GmUV5TrpThr,     -1.0F);
        SET_PARM(GmUV5TrpTm,     0.525F);
        SET_PARM(GmUV6TrpThr,     -1.0F);
        SET_PARM(GmUV6TrpTm,    0.2625F);
        SET_PARM(GmUVCvUpr     , 0.750F);
        SET_PARM(GmUVCvUprTm   ,  3.15F);
        SET_PARM(GmUVCvLwr     ,  0.20F);
        SET_PARM(GmUVCvLwrTm   , UV_OV_LVL_TM1);

        //Over-Voltage
        SET_PARM(GmOVAlmThr    , (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOVAlmTm     , 1.00F);
        SET_PARM(GmOV1TrpThr   , (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOV1TrpTm    , 315.0F);
        SET_PARM(GmOV2TrpThr   , 1.150F);
        SET_PARM(GmOV2TrpTm    , 0.525F);
        SET_PARM(GmOV3TrpThr   , 1.175F);
        SET_PARM(GmOV3TrpTm    , 0.230F);
        SET_PARM(GmOV4TrpThr   ,  1.20F);
   //   SET_PARM(GmOV4TrpTm    , 0.105F);
        SET_PARM(GmOV4TrpTm    , 0.120F);   // 0.1s flt when sampled at T3 can appear to be one T3 longer


        //Under-Frequency
        SET_PARM(GmUFAlmThr    , 0.960F);
        SET_PARM(GmUFAlmTm     , 0.100F);
        SET_PARM(GmUF1TrpThr   , GmUF1TrpLvl);
        SET_PARM(GmUF1TrpTm    , UF_OF_LVL1_TM);
        SET_PARM(GmUF2TrpThr   , GmUF2TrpLvl);
        SET_PARM(GmUF2TrpTm    , 1.05F);
        SET_PARM(GmUF3TrpThr   , 0.85F);
        SET_PARM(GmUF3TrpTm    , 0.2625F);

        //Over-Frequency
        SET_PARM(GmOFAlmThr    , 1.025F);
        SET_PARM(GmOFAlmTm     , 0.100F);
        SET_PARM(GmOF1TrpThr   , GmOF1TrpLvl);
        SET_PARM(GmOF1TrpTm    , UF_OF_LVL1_TM);
        SET_PARM(GmOF2TrpThr   , GmOF2TrpLvl);
        SET_PARM(GmOF2TrpTm    , 0.2625F);

        //Voltage Asymmetry
        SET_PARM(GmVAsyTrpThr  , 8.10F);
        SET_PARM(GmVAsyTrpTm   , 10.01F);
        SET_PARM(GmVAsy2TrpThr,  100.0F);
        SET_PARM(GmVAsy2TrpTm,    10.0F);
        SET_PARM(GmVAsyAlmTm   , 0.10F);

        SET_PARM(GmExPwrTrpTh,   2.0F*PARM(SysPower));

        GmConfig = GM_WIZ_ZVRT_GEN2_BASE;
        CCUCnfgBits.Svrt          = false;
        CCUCnfgBits.Lvrt          = false;
        CCUCnfgBits.Zvrt          = false;
        CCUCnfgBits.Cvrt          = false;
        CCUCnfgBits.ZvrtGen2Base  = true;
        CCUCnfgBits.ZvrtGen3Base  = false;
        CCUCnfgBits.ZvrtGen2Opt   = false;
        //CCUCnfgBits.MaxCapVals    = false;
    }
    else if ( PARM(GmCfgSelFV) == GM_WIZ_ZVRT_GEN2_OPT)
    {
        //Under-Voltage
        SET_PARM(GmUVAlmThr    , 0.850F);
        SET_PARM(GmUVAlmTm     , 0.100F);
        SET_PARM(GmUV1TrpThr   , 0.850F);
        SET_PARM(GmUV1TrpTm    , 3610.F);
        SET_PARM(GmUV2TrpThr   ,  0.80F);
        SET_PARM(GmUV2TrpTm    , 10.50F);
        SET_PARM(GmUV3TrpThr   , 0.750F);
        SET_PARM(GmUV3TrpTm    ,  3.15F);
        SET_PARM(GmUV4TrpThr   ,  0.20F);
        SET_PARM(GmUV4TrpTm    , UV_OV_LVL_TM1);
        SET_PARM(GmUV5TrpThr,     -1.0F);
        SET_PARM(GmUV5TrpTm,     0.525F);
        SET_PARM(GmUV6TrpThr,     -1.0F);
        SET_PARM(GmUV6TrpTm,    0.2625F);
        SET_PARM(GmUVCvUpr     , 0.750F);
        SET_PARM(GmUVCvUprTm   ,  3.15F);
        SET_PARM(GmUVCvLwr     ,  0.20F);
        SET_PARM(GmUVCvLwrTm   , UV_OV_LVL_TM1);

        //Over-Voltage
        SET_PARM(GmOVAlmThr    , (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOVAlmTm     , 1.00F);
        SET_PARM(GmOV1TrpThr   , (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOV1TrpTm    , 31.5F);
        SET_PARM(GmOV2TrpThr   , 1.20F);
        SET_PARM(GmOV2TrpTm    , 2.10F);
        SET_PARM(GmOV3TrpThr   , 1.25F);
        SET_PARM(GmOV3TrpTm    , 0.2625F);
        SET_PARM(GmOV4TrpThr   , 1.30F);
        SET_PARM(GmOV4TrpTm    , 0.12F);


        //Under-Frequency
        SET_PARM(GmUFAlmThr    , 0.960F);
        SET_PARM(GmUFAlmTm     , 0.100F);
        SET_PARM(GmUF1TrpThr   , GmUF1TrpLvl);
        SET_PARM(GmUF1TrpTm    , UF_OF_LVL1_TM);
        SET_PARM(GmUF2TrpThr   , GmUF2TrpLvl);
        SET_PARM(GmUF2TrpTm    , 1.05F);
        SET_PARM(GmUF3TrpThr   , 0.85F);
        SET_PARM(GmUF3TrpTm    , 0.2625F);

        //Over-Frequency
        SET_PARM(GmOFAlmThr    , 1.025F);
        SET_PARM(GmOFAlmTm     , 0.100F);
        SET_PARM(GmOF1TrpThr   , GmOF1TrpLvl);
        SET_PARM(GmOF1TrpTm    , UF_OF_LVL1_TM);
        SET_PARM(GmOF2TrpThr   , GmOF2TrpLvl);
        SET_PARM(GmOF2TrpTm    , 0.2625F);

        //Voltage Asymmetry
        SET_PARM(GmVAsyTrpThr  , 8.10F);
        SET_PARM(GmVAsyTrpTm   , 10.0F);
        SET_PARM(GmVAsy2TrpThr,  100.0F);
        SET_PARM(GmVAsy2TrpTm,    10.0F);
        SET_PARM(GmVAsyAlmTm   , 0.10F);

        SET_PARM(GmExPwrTrpTh,   2.0F*PARM(SysPower));

        GmConfig = GM_WIZ_ZVRT_GEN2_OPT;
        CCUCnfgBits.Svrt          = false;
        CCUCnfgBits.Lvrt          = false;
        CCUCnfgBits.Zvrt          = false;
        CCUCnfgBits.Cvrt          = false;
        CCUCnfgBits.ZvrtGen2Base  = false;
        CCUCnfgBits.ZvrtGen3Base  = false;
        CCUCnfgBits.ZvrtGen2Opt   = true;
    }
    else if ( PARM(GmCfgSelFV) == GM_WIZ_ZVRT_GEN3_BASE)
    {
        //Under-Voltage
        if (!PARM(VoltExtendOpt))
        {
           SET_PARM(GmUVAlmThr    , 0.890F);
           SET_PARM(GmUV1TrpThr   , 0.890F);
        }
        else
        {
           SET_PARM(GmUVAlmThr    , 0.87F);
           SET_PARM(GmUV1TrpThr   , 0.87F);
        }
        SET_PARM(GmUVAlmTm     , 0.100F);
        SET_PARM(GmUV1TrpTm    , 605.0F);
        SET_PARM(GmUV2TrpThr   ,  0.85F);
        SET_PARM(GmUV2TrpTm    , 10.50F);
        SET_PARM(GmUV3TrpThr   , 0.750F);
        SET_PARM(GmUV3TrpTm    ,  3.15F);
        SET_PARM(GmUV4TrpThr   ,  0.20F);
        SET_PARM(GmUV4TrpTm    , UV_OV_LVL_TM1);
        SET_PARM(GmUV5TrpThr,     -1.0F);
        SET_PARM(GmUV5TrpTm,     0.525F);
        SET_PARM(GmUV6TrpThr,     -1.0F);
        SET_PARM(GmUV6TrpTm,    0.2625F);
        SET_PARM(GmUVCvUpr     , 0.700F);
        SET_PARM(GmUVCvUprTm   ,  3.15F);
        SET_PARM(GmUVCvLwr     ,  0.20F);
        SET_PARM(GmUVCvLwrTm   , UV_OV_LVL_TM1);

        //Over-Voltage
        //SET_PARM(GmOVAlmThr    , 1.130F);
        SET_PARM(GmOVAlmThr    , (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOVAlmTm     , 300.0F);
        SET_PARM(GmOV1TrpThr   , (PARM(VregRefMax) + 0.01));
        SET_PARM(GmOV1TrpTm    , 315.0F);
        SET_PARM(GmOV2TrpThr   , 1.150F);
        SET_PARM(GmOV2TrpTm    , 31.50F);
        SET_PARM(GmOV3TrpThr   , 1.200F);
        SET_PARM(GmOV3TrpTm    , 2.100F);
        SET_PARM(GmOV4TrpThr   ,  1.25F);
   //   SET_PARM(GmOV4TrpTm    , 0.105F);
        SET_PARM(GmOV4TrpTm    , 0.510F);


        //Under-Frequency
        SET_PARM(GmUFAlmThr    , 0.960F);
        SET_PARM(GmUFAlmTm     , 0.100F);
        SET_PARM(GmUF1TrpThr   , GmUF1TrpLvl);
        SET_PARM(GmUF1TrpTm    , UF_OF_LVL1_TM);
        SET_PARM(GmUF2TrpThr   , GmUF2TrpLvl);
        SET_PARM(GmUF2TrpTm    , 1.05F);
        SET_PARM(GmUF3TrpThr   , 0.85F);
        SET_PARM(GmUF3TrpTm    , 0.2625F);

        //Over-Frequency
        SET_PARM(GmOFAlmThr    , 1.025F);
        SET_PARM(GmOFAlmTm     , 0.100F);
        SET_PARM(GmOF1TrpThr   , GmOF1TrpLvl);
        SET_PARM(GmOF1TrpTm    , UF_OF_LVL1_TM);
        SET_PARM(GmOF2TrpThr   , GmOF2TrpLvl);
        SET_PARM(GmOF2TrpTm    , 0.2625F);

        //Voltage Asymmetry
        SET_PARM(GmVAsyTrpThr  , 8.10F);
        SET_PARM(GmVAsyTrpTm   , 10.01F);
        SET_PARM(GmVAsy2TrpThr,  100.0F);
        SET_PARM(GmVAsy2TrpTm,    10.0F);
        SET_PARM(GmVAsyAlmTm   , 0.10F);

        SET_PARM(GmExPwrTrpTh,   2.0F*PARM(SysPower));

        GmConfig = GM_WIZ_ZVRT_GEN3_BASE;
        CCUCnfgBits.Svrt          = false;
        CCUCnfgBits.Lvrt          = false;
        CCUCnfgBits.Zvrt          = false;
        CCUCnfgBits.Cvrt          = false;
        CCUCnfgBits.ZvrtGen2Base  = false;
        CCUCnfgBits.ZvrtGen3Base  = true;
        CCUCnfgBits.ZvrtGen2Opt   = false;
        //CCUCnfgBits.MaxCapVals    = false;
    }
    else //Custom
    {
        //Custom -- allow use to change from default

        GmConfig = GM_WIZ_CUST;
        CCUCnfgBits.Svrt          = false;
        CCUCnfgBits.Lvrt          = false;
        CCUCnfgBits.Zvrt          = false;
        CCUCnfgBits.Cvrt          = true;
        CCUCnfgBits.ZvrtGen2Base  = false;
        CCUCnfgBits.ZvrtGen3Base  = false;
        CCUCnfgBits.ZvrtGen2Opt   = false;
        //CCUCnfgBits.MaxCapVals    = false;
    }

    // Use these two parms to apply a margin to configured voltage and frequency thresholds:
    SET_FV(GmVltThrMrg, 0.0F)  // pu volts
    SET_FV(GmFrqThrMrg, 0.0F)  // pu freq

    GmUV1TrpThrA = PARM(GmUV1TrpThr) - PARM(GmVltThrMrg);  // Actual UV1 threshold
    GmUV2TrpThrA = PARM(GmUV2TrpThr) - PARM(GmVltThrMrg);  // Actual UV2 threshold
    GmUV3TrpThrA = PARM(GmUV3TrpThr) - PARM(GmVltThrMrg);  // Actual UV3 threshold
    GmUV4TrpThrA = PARM(GmUV4TrpThr) - PARM(GmVltThrMrg);  // Actual UV4 threshold
    GmUV5TrpThrA = PARM(GmUV5TrpThr) - PARM(GmVltThrMrg);  // Actual UV5 threshold
    GmUV6TrpThrA = PARM(GmUV6TrpThr) - PARM(GmVltThrMrg);  // Actual UV6 threshold

    GmOV1TrpThrA = PARM(GmOV1TrpThr) + PARM(GmVltThrMrg);  // Actual OV1 threshold
    GmOV2TrpThrA = PARM(GmOV2TrpThr) + PARM(GmVltThrMrg);  // Actual OV2 threshold
    GmOV3TrpThrA = PARM(GmOV3TrpThr) + PARM(GmVltThrMrg);  // Actual OV3 threshold
    GmOV4TrpThrA = PARM(GmOV4TrpThr) + PARM(GmVltThrMrg);  // Actual OV4 threshold

    GmUF1TrpThrA = PARM(GmUF1TrpThr) - PARM(GmFrqThrMrg);  // Actual UF1 threshold
    GmUF2TrpThrA = PARM(GmUF2TrpThr) - PARM(GmFrqThrMrg);  // Actual UF2 threshold
    GmUF3TrpThrA = PARM(GmUF3TrpThr) - PARM(GmFrqThrMrg);  // Actual UV3 threshold

    GmOF1TrpThrA = PARM(GmOF1TrpThr) + PARM(GmFrqThrMrg);  // Actual OF1 threshold
    GmOF2TrpThrA = PARM(GmOF2TrpThr) + PARM(GmFrqThrMrg);  // Actual OF2 threshold


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                       Grid Monitor Configuration Faults
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////
    // Under-voltage
    ///////////////////


    // Find equation for proposed curve .
    // Use this equation to plug in two capability cureve times.
    // If either resulting point is below the respective capability curve point, trip..
    // Straight line equation is Y = M1(X-X1) + Y1--Line #1
    // M1 = V.GmUVCvSlope  (calced above)
    // M2 = V.GmUVCvSlopeC (calced above)
    // X1 = P.GmUVCvUprTm
    // Y1 = P.GmUVCvUpr
    //
    // (slope calcs from above for refrerence
    // GmUVCvSlope= ( PARM(GmUVCvUpr   - PARM(GmUVCvLwr)/    (PARM(GmUVCvUprTm  - PARM(GmUVCvLwrTm);  1
    //                        Y1                                       X1
    //
    // GmUVCvSlopeC= (PARM(GmUVCvUpCap - PARM(GmUVCvLwrCap)/ (PARM(GmUVCvUCapTm - PARM(GmUVCvLCapTm); 2
    //




    GmUVCfgBadT1 = GmUVCfgBadT2 = GmUVCfgBadT3 = GmUVCfgBadT4  = GmUVCfgBadT5  = GmUVCfgBadT6  = GmUVCfgBadT7
                 = GmUVCfgBadT8 = GmUVCfgBadT9 = GmUVCfgBadT10 = GmUVCfgBadT11 = GmUVCfgBadT12 = false;


    GmUVCvYInt  = PARM(GmUVCvUpr)   - (GmUVCvSlope*PARM(GmUVCvUprTm));
    GmUVCvYIntC = PARM(GmUVCvUpCap) - (GmUVCvSlopeC*PARM(GmUVCvUCapTm));
    // Vpu =  GmUVCvSlope*time + YInt;  (These are corresponding capability curve points
    // These are capability curve points at attempted curve lower and upper times
    GmUVCvVpuUpr = (GmUVCvSlopeC*PARM(GmUVCvUprTm)) + GmUVCvYIntC;
    GmUVCvVpuLwr = (GmUVCvSlopeC*PARM(GmUVCvLwrTm)) + GmUVCvYIntC;
    GmUVCvVLvl4   = (GmUVCvSlopeC*PARM(GmUV1TrpTm)) + GmUVCvYIntC;

    //Find the voltage that the proposed curve at the lower curve capabilitiy time.
    GmUVCvVKnee = (GmUVCvSlope*PARM(GmUVCvLCapTm)) + GmUVCvYInt;

    //Proposed thesholds must be above capablity curve

    if ( (PARM(GmUVCvLwr) < 0.0F)  &&  (PARM(GmUVCvUpr) < 0.0F) )
    {
        //Skip the check...the curve is disabled
    }
    else
    {
       //Case where lower curve threshold is below lower capability point and upper curve threshold is above  are below lower capability point
       if ( ( (PARM(GmUVCvLwr) < 0.0F) || ( PARM(GmUVCvLwr) < PARM(GmUVCvLwrCap)) ) && ( ( PARM(GmUVCvUpr) < 0.0F) || ( PARM(GmUVCvUpr) > PARM(GmUVCvLwrCap)) )  )
       {
          if ( !( ( (PARM(GmUVCvLwr) < 0.0F) || ( (PARM(GmUVCvUpr) < 0.0F) ) )  ||
                  ( (PARM(GmUVCvUpr) >= (GmUVCvVpuUpr - 0.01))   && (GmUVCvVKnee >= (PARM(GmUVCvLwrCap) - 0.01F) )  )  )  )
          {
              GmUVCfgBadT1  = true;
          }
       }
       else //all other cases
       {
          if ( !( ((PARM(GmUVCvUpr) < 0.0F) || (PARM(GmUVCvUpr) >= (GmUVCvVpuUpr - 0.01F)) )   &&  ((PARM(GmUVCvLwr) < 0.0F) || (PARM(GmUVCvLwr) >= (GmUVCvVpuLwr - 0.01F)) )      )   )
          {
              GmUVCfgBadT1  = true;
          }
       }
    }



    //Make sure UV levels are in correct order
    if ( ! ( ((PARM(GmUV4TrpThr) < PARM(GmUV3TrpThr)) || (PARM(GmUV4TrpThr) < 0.0F)) &&  ( (PARM(GmUV3TrpThr) < PARM(GmUV2TrpThr)) || (PARM(GmUV3TrpThr) < 0.0F) ) &&
            (PARM(GmUV2TrpThr) < PARM(GmUV1TrpThr)) &&  (PARM(GmUV1TrpThr) < (PARM(GmUVMaxLvl) + 0.01F) )  &&
            (PARM(GmUVAlmThr)  <= (PARM(GmUVMaxLvl) + 0.01F))  )   )
    {
       GmUVCfgBadT2  = true;
    }

    //Make sure UV times are in correct order
    if ( ! ( ( (PARM(GmUV4TrpTm) < PARM(GmUV3TrpTm)) || (PARM(GmUV4TrpThr) < 0.0F) )  &&  (  (PARM(GmUV3TrpTm) < PARM(GmUV2TrpTm))  ||  (PARM(GmUV3TrpThr) < 0.0F) )&&
            (PARM(GmUV2TrpTm) < PARM(GmUV1TrpTm)) &&  (PARM(GmUV1TrpTm) > 0.0F)  &&
            (PARM(GmUVAlmTm)  >= 0.0F ) )  )
    {
       GmUVCfgBadT3  = true;
    }

    //Make sure UV curve levels and times are in correct order...but don't care if theshold are negative
    if( !( ( ( PARM(GmUVCvLwr) <= PARM(GmUVCvUpr)) && (PARM(GmUVCvLwrTm) <= PARM(GmUVCvUprTm)))  || ( (PARM(GmUVCvLwr) < 0.0F) && (PARM(GmUVCvUpr) < 0.0F) ) )  )
    {
       GmUVCfgBadT4  = true;
    }

      //Level 1
    if ( !((PARM(GmUV1TrpThr) >= (PARM(GmUV1Cap) - 0.01F) ) &&
           (  (PARM(GmUV1TrpTm) <= (PARM(GmUV1CapTm) + 0.01F) ) || (PARM(GmUV1TrpThr) >= 0.90F)  )  )  )
    {
       GmUVCfgBadT5  = true;
    }

      //Level 2
    if ( !( (PARM(GmUV2TrpThr) >= (PARM(GmUV2Cap) - 0.01F) ) &&
           (  (PARM(GmUV2TrpTm) <= (PARM(GmUV2CapTm) + 0.01F) ) || (PARM(GmUV2TrpThr) >= 0.90F)  )  )  )
    {
       GmUVCfgBadT6  = true;
    }


    //Check the rest based on position of 'curve' portion
    //Curve is above level 4
    if ( PARM(GmUVCvLwr) >= PARM(GmUV4TrpThr))
    {   //Level 3
        if ( ! ( ( PARM(GmUV3TrpThr) < 0.0F) ||
               ( (PARM(GmUV3TrpThr) >= (PARM(GmUV3Cap) - 0.01F)) && (PARM(GmUV3TrpThr) >= PARM(GmUVCvUpr)) &&
               ( (  PARM(GmUV3TrpTm) <= (PARM(GmUV3CapTm) + 0.01F)) || (PARM(GmUV3TrpThr) >= 0.90) )  )   )  )
       {
          GmUVCfgBadT7  = true;
       }
       //Level 4
       if (  ! ( ( PARM(GmUV3TrpThr) < 0.0F)  ||
               ( (PARM(GmUV4TrpThr) <= PARM(GmUVCvUpr)) && (PARM(GmUV4TrpThr) <= PARM(GmUVCvLwr) ) &&
               ( (  PARM(GmUV4TrpTm) <= PARM(GmUVCvLwrTm)) || (PARM(GmUV4TrpThr) < 0.0F) ) )    )  )
       {
          GmUVCfgBadT8  = true;
       }

       //Curve
       if ( !( ( (PARM(GmUVCvUprTm) <= PARM(GmUV3TrpTm)) || ( PARM(GmUV3TrpThr) < 0.0F) || ( PARM(GmUVCvUpr) < 0.0F)) && ( (PARM(GmUVCvLwrTm) <= (PARM(GmUVCvLCapTm) + 0.01) || ( PARM(GmUVCvLwr) < 0.0F))   ) ) )
       {
          GmUVCfgBadT9  = true;
       }
    }
    else //Curve is below level 4
    {
       if ( ! ( ( PARM(GmUV3TrpThr) < 0.0F) ||
          ( (PARM(GmUV3TrpThr) >= (PARM(GmUV3Cap) - 0.01F) &&
          ( ( PARM(GmUV3TrpTm) <= (PARM(GmUV3CapTm)  + 0.01F ) || (PARM(GmUV3TrpThr) >= 0.90F)) )  ) ) )    )
       {
          GmUVCfgBadT10 = true;
       }
       //Level  4
       if ( ! ( ( ( PARM(GmUV4TrpThr) < 0.0F) || ( PARM(GmUVCvUpr) < 0.0F)  || ( PARM(GmUVCvLwr) < 0.0F)  )        ||
              ( (PARM(GmUV4TrpThr)  >=   (PARM(GmUVCvUpr) - 0.01F) ) &&
                ( GmUVCvVLvl4       >=   (PARM(GmUVCvUpCap) - 0.01F)  )  && //ensures level 4 above cap curve extended above and below one point
                ( ( PARM(GmUV4TrpTm)  >=   (PARM(GmUVCvUprTm) - 0.01f) || (PARM(GmUV4TrpThr) < 0.0F) ))    )  )   )
       {
          GmUVCfgBadT11 = true;
       }
       //Curve
       if ( ! ( ( PARM(GmUVCvLwr) < 0.0F )  || (PARM(GmUVCvLwrTm) <= (PARM(GmUVCvLCapTm) + 0.01F) ) ) )
       {
          GmUVCfgBadT12 = true;
       }
    }

    GmUVCfgBad = (GmUVCfgBadT1 || GmUVCfgBadT2 || GmUVCfgBadT3 || GmUVCfgBadT4  || GmUVCfgBadT5  || GmUVCfgBadT6 ||
                  GmUVCfgBadT7 || GmUVCfgBadT8 || GmUVCfgBadT9 || GmUVCfgBadT10 || GmUVCfgBadT11 ||  GmUVCfgBadT12);








    GmOVCfgBadT1 = GmOVCfgBadT2 = GmOVCfgBadT3 = GmOVCfgBadT4 = GmOVCfgBadT5 = GmOVCfgBadT6 = false;

    //Make sure OV levels are in correct order
    if ( !(((PARM(GmOV4TrpThr) > PARM(GmOV3TrpThr)) || (PARM(GmOV4TrpThr) > 4.0F)) && ((PARM(GmOV3TrpThr) > PARM(GmOV2TrpThr)) || (PARM(GmOV3TrpThr) > 4.0F))
            && (PARM(GmOV2TrpThr) > PARM(GmOV1TrpThr)) &&  (PARM(GmOV1TrpThr) > (PARM(GmOVMinLvl) - 0.01F) )
            && (PARM(GmOVAlmThr)  > (PARM(GmOVMinLvl) - 0.01F) )    )    )
       {
          GmOVCfgBadT1 = true;
       }

         //Make sure OV times are in correct order
   if ( !(((PARM(GmOV4TrpTm) < PARM(GmOV3TrpTm)) || (PARM(GmOV4TrpTm) < 0.0F)) && ((PARM(GmOV3TrpTm) < PARM(GmOV2TrpTm)) || (PARM(GmOV3TrpTm) < 0.0F))
           && (PARM(GmOV2TrpTm) < PARM(GmOV1TrpTm)) &&  (PARM(GmOV1TrpTm) > 0.0F) && (PARM(GmOVAlmTm) > 0.0F  ) ) )
       {
          GmOVCfgBadT2 = true;
       }

         //Level 1
         //Make sure OV level 1 is ok
    if ( !((PARM(GmOV1TrpThr) <= (PARM(GmOV1Cap)+0.01F)) &&  (PARM(GmOV1TrpThr) >= (PARM(GmOVMinLvl) - 0.01F) ) &&
          ( (PARM(GmOV1TrpTm) <= (PARM(GmOV1CapTm)+0.01F)) || (PARM(GmOV1TrpThr) < 1.10F))  ) )
       {
          GmOVCfgBadT3 = true;
       }

          //Level 2
         //Make sure OV level 2 is ok
    if (!( (PARM(GmOV2TrpThr) <= (PARM(GmOV2Cap)+0.01F)) &&
         ( (PARM(GmOV2TrpTm) <= (PARM(GmOV2CapTm)+0.01F)) || (PARM(GmOV2TrpThr) < 1.10F)) )   )
       {
          GmOVCfgBadT4 = true;
       }

          //Level 3
         //Make sure OV level 3 is ok
    if (!( (PARM(GmOV3TrpThr) > 4.0F) || ((PARM(GmOV3TrpThr) <= (PARM(GmOV3Cap)+0.01F)) &&
         ( (PARM(GmOV3TrpTm) <= (PARM(GmOV3CapTm)+0.01F)) || (PARM(GmOV3TrpThr) < 1.10F))) )   )
       {
          GmOVCfgBadT5 = true;
       }

          //Level 4
         //Make sure OV level 3 is ok
    if (!( (PARM(GmOV4TrpThr) > 4.0F) || ((PARM(GmOV4TrpThr) <= (PARM(GmOV4Cap)+0.01F)) &&
         ( (PARM(GmOV4TrpTm) <= (PARM(GmOV4CapTm)+0.01F)) || (PARM(GmOV4TrpThr) < 1.10F))) )   )
       {
          GmOVCfgBadT6 = true;
       }




    GmOVCfgBad = (GmOVCfgBadT1 || GmOVCfgBadT2 || GmOVCfgBadT3 || GmOVCfgBadT4 || GmOVCfgBadT5 || GmOVCfgBadT6);



    GmUFCfgBadT1 = false;
    GmUFCfgBadT2 = false;
    GmUFCfgBadT1 = false;
    GmUFCfgBadT3 = false;
    GmUFCfgBadT4 = false;
    GmUFCfgBadT5 = false;

    //Make sure UF levels are in correct order
    if (  ! (  (PARM(GmUF3TrpThr) < PARM(GmUF2TrpThr)) && (PARM(GmUF2TrpThr) < PARM(GmUF1TrpThr))  &&
            (PARM(GmUF1TrpThr) < (PARM(GmUFMaxLvl) + 0.01F) ) && (PARM(GmUFAlmThr) < (PARM(GmUFMaxLvl) + 0.01F) )   ) )
    {
        GmUFCfgBadT1 =true;
    }

         //Make sure UF times are in correct order
    if ( ! ( ( (PARM(GmUF3TrpTm) < PARM(GmUF2TrpTm)) || (PARM(GmUF3TrpThr) < 0.0F))  && (PARM(GmUF2TrpTm) < PARM(GmUF1TrpTm)) &&
           (PARM(GmUF1TrpTm) > 0.0F) && (PARM(GmUFAlmTm) > 0.0F  ) ) )
    {
        GmUFCfgBadT2 =true;
    }

         //Level 1
         //Make sure UF level 1 is ok
    if ( !((PARM(GmUF1TrpThr) >= (PARM(GmUF1Cap) - 0.01)) &&  (PARM(GmUF1TrpThr) <= (PARM(GmUFMaxLvl)+0.01)) &&
          ((PARM(GmUF1TrpTm) <= (PARM(GmUF1CapTm) + 0.01)) ||(PARM(GmUF1TrpThr) > 0.95) )   )  )
    {
        GmUFCfgBadT3 =true;
    }

         //Make sure UF level 2 is ok
    if ( !( (PARM(GmUF2TrpThr) >= (PARM(GmUF2Cap) - 0.01F)) &&
          ( (PARM(GmUF2TrpTm) <= (PARM(GmUF2CapTm) + 0.01F)) ||(PARM(GmUF2TrpThr) > 0.95F) ) )  )
    {
        GmUFCfgBadT4 =true;
    }

         //Make sure UF level 3 is ok
    if ( !(  (PARM(GmUF3TrpThr) < 0.0) ||
          ((PARM(GmUF3TrpThr) >= (PARM(GmUF3Cap) - 0.01F)) &&
          ( (PARM(GmUF3TrpTm) <= (PARM(GmUF3CapTm) + 0.01F)) ||(PARM(GmUF3TrpThr) > 0.95F) ) ) )     )
    {
        GmUFCfgBadT5 =true;
    }

       GmUFCfgBad = GmUFCfgBadT1 || GmUFCfgBadT2 || GmUFCfgBadT3 || GmUFCfgBadT4 || GmUFCfgBadT5;





    GmOFCfgBadT1 = false;
    GmOFCfgBadT2 = false;
    GmOFCfgBadT1 = false;
    GmOFCfgBadT3 = false;
    GmOFCfgBadT4 = false;

    //Make sure OF levels are in correct order
    if ( !((PARM(GmOF2TrpThr) > PARM(GmOF1TrpThr)) &&  (PARM(GmOF1TrpThr) >= (PARM(GmOFMinLvl) - 0.01F)) &&
           (PARM(GmOFAlmThr)  >= (PARM(GmOFMinLvl) - 0.01F) )    )  )
    {
        GmOFCfgBadT1 =true;
    }


         //Make sure OF times are in correct order
    if ( ! ((PARM(GmOF2TrpTm) < PARM(GmOF1TrpTm)) && (PARM(GmOF1TrpTm) > 0.0F )  &&
            (PARM(GmOFAlmTm) > 0.0F ))  )
    {
        GmOFCfgBadT2 =true;
    }

         //Level 1
         //Make sure OF level 1 is ok
    if ( !((PARM(GmOF1TrpThr) <= (PARM(GmOF1Cap) + 0.01F)) &&  (PARM(GmOF1TrpThr) >= (PARM(GmOFMinLvl) - 0.01F)  ) &&
          (  (PARM(GmOF1TrpTm) <= (PARM(GmOF1CapTm) + 0.01F)  ) || (PARM(GmOF1TrpThr) < 1.05F))  ) )
    {
        GmOFCfgBadT3 =true;
    }


         //Make sure OF level 2 is ok
    if (!( (PARM(GmOF2TrpThr) <= (PARM(GmOF2Cap) + 0.01F)) &&
         ( (PARM(GmOF2TrpTm) <= (PARM(GmOF2CapTm) + 0.01F)) || (PARM(GmOF1TrpThr) < 1.05F)) )  )
    {
        GmOFCfgBadT4 =true;
    }

    GmOFCfgBad = GmOFCfgBadT1 || GmOFCfgBadT2 || GmOFCfgBadT3 || GmOFCfgBadT4;

    SET_FV(GmUVIndTstAlmThr,   0.10F);
    SET_FV(GmUV2IndTstTrpThr,  0.20F);
    SET_FV(GmUV3IndTstTrpThr,  0.10F);
    SET_FV(GmUVIndTstTrpThrs1, 0.20F);
    SET_FV(GmUVIndTstTrpThrs2, 0.10F);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                            End of grid monitor rules
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    VlnPeakRcp= 1.0F/(PARM(L_VllInput)*SQRT2*SQRT3_RCP);


    //Thermal I/O feedback and protection
    SET_FV(WTempIoThrm,  10.0F);

    //Humidity feedback
    SET_FV(WHumidData,  10.0F);

    //-------------------------------------------------------------------------
    // under temperature trip threshold

    //-------------------------------------------------------------------------
    // Thermistor based alarm thresholds
    SET_FV(InletAirOtThrsh    , 55.0F);          // (deg C)
    SET_FV(InletAirOtOnThrsh  , 51.0F);          // (deg C)
    SET_FV(InletAirOtOffThrsh , 46.0F);          // (deg C)

    SET_FV(InletAirUtOnThrsh  , -30.0F);         // (deg C)
    SET_FV(InletAirUtOffThrsh , -27.0F);         // (deg C)


    // Coolant
    SET_FV(CoolantOtDlyThrsh  , 70.0F);       // (deg C)
    SET_FV(CoolantOtThrsh     , PARM(CoolantOtDlyThrsh) +2.0F);
    SET_FV(CoolantOtOnThrsh   , 67.0F);          // (deg C)
    SET_FV(CoolantOtOffThrsh  , 63.0F);          // (deg C)

    SET_FV(CoolantUtThrsh     , 0.0F);
    SET_FV(CoolantUtOnThrsh   , 1.0F);           // (deg C)
    SET_FV(CoolantUtOffThrsh  , 3.0F);           // (deg C)
    SET_FV(CoolntUtTm         , 0.5F);


    SET_FV(PdcCabEnOtThrsh       , 80.0F);         // (deg C)
    SET_FV(PdcCabEnOtOnThrsh     , 77.0F);          // (deg C)
    SET_FV(PdcCabEnOtOffThrsh    , 73.0F);          // (deg C)

    SET_FV(PdcCabEnUtOnThrsh     , 0.0F);           // (deg C)
    SET_FV(PdcCabEnUtOffThrsh    , 3.0F);           // (deg C)


    // CFC
    SET_FV(CfcOtThrsh         , 70.0F);          // (deg C)
    SET_FV(CfcOtOnThrsh       , 67.0F);          // (deg C)
    SET_FV(CfcOtOffThrsh      , 63.0F);          // (deg C)

    SET_FV(CfcUtOnThrsh       , -20.0F);         // (deg C)
    SET_FV(CfcUtOffThrsh      , -15.0F);         // (deg C)

    // PDC

    if (PARM(SoftwareIPN) >=  DFIG02_2400_5060_A)
    {
        SET_FV(PdcOtThrsh         , 115.0F);         // (deg C)
        SET_FV(PdcOtOnThrsh       , 112.0F);          // (deg C)
        SET_FV(PdcOtOffThrsh      , 108.0F);          // (deg C)
    }
    else
    {
        SET_FV(PdcOtThrsh         , 115.0F);         // (deg C)
        SET_FV(PdcOtOnThrsh       , 112.0F);          // (deg C)
        SET_FV(PdcOtOffThrsh      , 108.0F);          // (deg C)
    }


    SET_FV(PdcUtThrsh         , -35.0F);         // (deg C)
    SET_FV(PdcUtOnThrsh       , -30.0F);         // (deg C)
    SET_FV(PdcUtOffThrsh      , -25.0F);         // (deg C)


    // Fuses
    SET_FV(AclfuOtThrsh       , 90.0F);          // (deg C)
    SET_FV(AclfuOtOnThrsh     , 87.0F);          // (deg C)
    SET_FV(AclfuOtOffThrsh    , 83.0F);          // (deg C)

    // HSLA
    SET_FV(HslaOtThrsh        , 85.0F);          // (deg C)
    SET_FV(HslaOtOnThrsh      , 70.0F);          // (deg C)
    SET_FV(HslaOtOffThrsh     , 65.0F);          // (deg C)

    SET_FV(HslaUtThrsh        , -30.0F);         // (deg C)
    SET_FV(HslaUtOnThrsh      , -25.0F);         // (deg C)
    SET_FV(HslaUtOffThrsh     , -20.0F);         // (deg C)


    // thresholds for the run permissive from thermistor
    SET_FV(MaContTempLowLvl   , -25.0F);         // (deg C) Spec rev 13
    SET_FV(CcstarTempLowLvl   , -24.0F);         // (deg C) Spec rev 13
    SET_FV(InletAirTempLimMax , 32.0F);
    SET_FV(InletAirTempLimMin , 0.0F);
    SET_FV(TempRiseOkTmMax    , 70.0F * 60.0F * DelTm3Rcp);          // approx 420,000 Counts = 70 min at task 3 rate
    SET_FV(TempRiseOkTmNom    , 15.0F * 60.0F * DelTm3Rcp);          // aprox 90,000 Counts   = 15 min at task 3 rate
    // these two are for display only
    MaxTimeTempRiseOk = PARM(TempRiseOkTmMax)* DelTm3 * 0.0166667F;
    NomTimeTempRiseOk = PARM(TempRiseOkTmNom)* DelTm3 * 0.0166667F;
    SET_FV(TempFbkRdyTm       , 2.0F);                             // waiting time for the temperature feedbacks to be ready
    SET_FV(TempFilterSettleTm , 10.0F* (1.0F/ PARM(WTempIoThrm)) );  // 10 times the filter time constant
    SET_FV(HtrFanSupresTm,      5.0F);
    SET_FV(TempRiseOkLvl,       4.0F);
    SET_FV(TempRiseNotOkLvl,    0.5F);  // revised based on spec rev21
    SET_FV(HtrFanEnb,           true);
    SET_FV(DcLinkFrostThrsh,    0.0F);
    SET_FV(DcLinkNotFrostThrsh, 5.0F);


    //DTA Cooling Fan Control
    SET_FV(FanMinOffTm, 120.0F);  //seconds
    SET_FV(FanMinOnTm,   60.0F);  //seconds
    SET_FV(InAirMaxTemp,  0.0F);
    SET_FV(InAirMinTemp,  0.0F);
    SET_FV(CfcDeltaOn,   55.0F);
    SET_FV(CfcDeltaOff,  50.0F);
    SET_FV(Q1FanPdcOnThr, 40.0F);
    SET_FV(Q1FanPdcOfThr, 36.0F);
    SET_FV(Q1FanCabEntryOnThr, 60.0F);
    SET_FV(Q1FanCabEntryOfThr, 55.0F);
    FanStateTmr = PARM(FanMinOffTm);


    // Heating Control
    if( PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
    {
       SET_FV(AirHeatrMax, 30.0F); // Maximun limit to clamp inlet air temp for heating control
    }
    else
    {
       SET_FV(AirHeatrMax, 25.0F);
    }
    SET_FV(AirHeatrMin,  5.0F); // Minimum limit to clamp inlet air temp for heating control
    SET_FV(MCCHtrOn,     1.0F); // MCC heat On threshold in degC
    SET_FV(MCCHtrOff,    2.0F); // MCC heat Off threshold in degC
    SET_FV(CoolntHtrOn,  1.0F); // Coolant heat On threshold in degC
    SET_FV(CoolntHtrOff, 5.0F); // Coolant heat Off threshold in degC
    SET_FV(PdcCfcHtrOn,  1.0F); // PDC & CFC heat On threshold in degC
    SET_FV(PdcCfcHtrOff, 5.0F); // PDC & CFC heat Off threshold in degC
    SET_FV(WEMAComDlyTm, 100);  // Delay time for testing communications with WEMA - sec


    // Cooling System
    SET_FV(PmpShftPwr50, 340.0F); //Pump shaft power at 50 hz
    SET_FV(PmpShftPwr60, 570.0F); //Pump shaft power at 60 hz
    SET_FV(FanOnGth50,  1180.0F); //Heat exchanger thermal conductance with fan running at 50 hz
    SET_FV(FanOnGth60,  1300.0F); //Heat exchanger thermal conductance with fan running at 60 hz
    SET_FV(CoolntTempCalcW, 1.0F/DelTm3);   // This results in unity filter gain for now!
    SET_FV(CoolTempCalcFrz,false);
    SET_FV(CoolDeltaOn,  20.0);
    SET_FV(CoolDeltaOff, 10.0);

    SET_FV(IpcIntTrpUpDt, DelTm2);
    SET_FV(IpcIntTrpDnDt, 0.3333F * DelTm2);
    SET_FV(IpcIntTrpThr,  0.05F);


    if ( PARM(L_FrqInput) == 50.0F )     // 50 hz case
    {
        PmpShftPwr = PARM(PmpShftPwr50);
        FanOnGth   = PARM(FanOnGth50);
    }
    else                                // 60 hz case
    {
        PmpShftPwr = PARM(PmpShftPwr60);
        FanOnGth   = PARM(FanOnGth60);
    }
    FanOnGthRcp = (FanOnGth > 0.0F) ? (1.0F/FanOnGth) : 0.0F;
    CoolntTempCalcGn = PARM(CoolntTempCalcW) * DelTm3;

    SET_FV(CoolRHeatr,   38.4F);        //Coolant heater resistance in ohms (240)^2/1500
    CoolRHeatRcp = 1.0F / PARM(CoolRHeatr);
    SET_FV(CFCXfrmRatio, 400.0F/690.0F); //CFC transformer (T2) low to high ratio

    SET_FV(BtmHiJncTemp, 140.0F);       // Worst-case junction temperature used in thermal model
    SET_FV(BtmDLssComp,  0.00348F);     // Diode power loss compensation factor
    SET_FV(BtmTrLssComp, 0.00199F);     // Transistor power loss compensation factor

    SET_FV(L_BrgLssFilW, 10.0F);        // Line bridge losses - filter omega rad/s
    L_BrgLssGn = DelTm3*PARM(L_BrgLssFilW);
    SET_FV(R_BrgLssFilW, 10.0F);        // Rotor bridge losses - filter omega rad/s
    R_BrgLssGn = DelTm3*PARM(R_BrgLssFilW);


    SET_FV(L_Q1FiltTm, (5.0F/PARM(GmPhsCurrW)));// Waiting time for filtered signals to settle

    if(SimAcclAct)
    {
        L_FilSignTm1=0.0F;
    }
    else
    {
        L_FilSignTm1 = PARM(Q1ClsDlyTm) + PARM(L_Q1FiltTm);
    }
    L_FilSignTm2 = PARM(Q1OpnDlyTm) + PARM(L_Q1FiltTm);

    Temp1 = SQRT2_3 * PARM(L_VllInput);
    SET_FV(L_Q1VlThrs,  0.1F*Temp1*0.1F*Temp1);  // Q1 contactor square voltage-difference threshold for sequencing

    // Percentage of nominal dist-filter power to tolerate variations in
    // capacitance and resistance
    DfXnom = NoNANLim( (1.0F/(PARM(L_LineFrq) * PARM(CnvLineDfC))), 1.0e6F, -1.0e6F );
    DfInom = PARM(L_LineFrq) * PARM(CnvLineDfC) * SQRT3_RCP * PARM(L_VxfmrSec);     // Very small resistor is neglected

    SET_FV(DFtestDlyTm, 1.5F);                  // Time delay between Q1 closing attempts for dist-filt verification
    SET_FV(L_DfPfilW,  5.0F);                   // Active power low pass filter response for Dist-Filter verification
    SET_FV(L_DfQfilW, 20.0F);                   // Reactive power low pass filter response for Dist-Filter verification
    L_DfPwrgain = PARM(L_DfPfilW)*DelTm2;
    L_DfQpwrgain = PARM(L_DfQfilW)*DelTm2;
    L_DfQPwrGn=PARM(L_DfQfilW)*DelTm2;

    if(SimAcclAct)
    {
        SET_FV(KdfFiltTm,  0.0F); // Waiting time for filtered signals to settle
        SET_FV(KdfClOpTm,  0.0F);                // Based on manufacturer test data, about twice the six-sigma upper level
        SET_FV(L_DfQPwrDlyTm, 0.03F);
    }
    else
    {
        SET_FV(KdfFiltTm,  (5.0F/PARM(L_DfQfilW))); // Waiting time for filtered signals to settle
        SET_FV(KdfClOpTm,  0.150F);                // Based on manufacturer test data, about twice the six-sigma upper level
        SET_FV(L_DfQPwrDlyTm, 1.5F);                // Time delay to check that power reaches minimun-nominal level
    }


    if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_A )
    {
       SET_FV(CContOpnTm, 0.040F);
    }
    else
    {
       SET_FV(CContOpnTm, 0.000F);
    }

    KdfQPwrTm=PARM(KdfFiltTm);
    SET_FV(KdfClOpTm,  0.150F);                // Based on manufacturer test data, about twice the six-sigma upper level

    SET_FV(MvSwGearTrpTm, 1.0F);               // Duration of time that MVSG coil is energized

    SET_FV(S_K1DropOutDlyTm, 0.0F);             // No delay needed for 690 volt contactor

    ////////////////////////////////////////////////////////////////////////////
    // S_K1 related parameters
    ////////////////////////////////////////////////////////////////////////////
    //TODO, 2.3IPN value may not be correct
    switch(PARM(SoftwareIPN))
    {
        case DFIG02_1500_5060_A:
        case DFIG02_1500_5060_B:
        case DFIG02_1850_5060_C:
        case DFIG02_1850_5060_D:
        case DFIG02_1850_5060_E:
            // sync contactor pickup time
            if(SimAcclAct)
            {
                SET_FV(S_K1ClsDlyTm,    0.0F);
            }
            else
            {
                SET_FV(S_K1ClsDlyTm,    0.25F);
            }

            SET_FV(S_K1FiltTm,      0.16F);     // Waiting time for filtered signals to settle
            // Maximum sync-switch dropping time from spec, plus a fudge factor of 60 msec.
            // Exercise CAUTION when increasing this parm because it will eventually interfere
            // with the safety-cain shutdown sequence.
            SET_FV(S_K1OpnDlyTm,0.29F);
            SET_FV(S_K1OpnCmdDlyTm, 0.0F);  // do not delay for 1.x
            if (PARM(TestMode)==PUMP_BACK )
            {
                //This is the value set for non-pumpback with a nomral R_IyRat value
                SET_FV(S_K1ImagThrs, 30.067F);
            }
            else
            {
                //K1 contactor current threshold for sequencing
                SET_FV(S_K1ImagThrs, 0.05F*R_IyRat/PARM(S_RRatio));
            }

            S_K1ImagThrs = PARM(S_K1ImagThrs);
            break;
        case DFIG02_2400_5060_A:
        default:
            // sync contactor pickup time
            if(SimAcclAct)
            {
                SET_FV(S_K1ClsDlyTm,    0.0F);
            }
            else
            {
                SET_FV(S_K1ClsDlyTm,    0.13F);
            }
                                                // sync contactor pickup time (K1 should close in 100 ms max but no harm in
                                                // waiting a little longer before starting CT test
            SET_FV(S_K1FiltTm,      0.02F);      // Waiting time for filtered signals to settle
            // Maximum sync-switch dropping time from spec, plus a fudge factor of 60 msec.
            // Exercise CAUTION when increasing this parm because it will eventually interfere
            // with the safety-cain shutdown sequence.
            SET_FV(S_K1OpnDlyTm,0.230F);
            SET_FV(S_K1OpnCmdDlyTm, 0.07);  // delay 50ms for 2.x
            if (PARM(TestMode)==PUMP_BACK )
            {
                //This is the value set for non-pumpback with a nomral R_IyRat value
                SET_FV(S_K1ImagThrs, 30.067F);
            }
            else
            {
                //K1 contactor current threshold for sequencing
                SET_FV(S_K1ImagThrs, 0.15F*R_IyRat/PARM(S_RRatio));
            }
            break;
    }
    SET_FV(S_K1VmagErrTol, 0.025F * SQRT2_3 * PARM(S_Volts));       // voltage match tolerance across K1 during CT checking
    if(SimAcclAct)
    {
        SET_FV(S_K1VmagErrTm, 0.01F);
    }
    else
    {
        SET_FV(S_K1VmagErrTm, 0.25F);                // Minimum time to declare voltage error across K1 acceptable
    }
    SET_FV(S_K1VmagThrs, 0.05F * SQRT2_3 * PARM(S_VxfmrSec));   // For verifying K1 state based only on voltage
    // Square voltage mismatch threshold across K1 for sequencing
    SET_FV(S_K1VlThrs, PARM(S_K1VmagThrs) * PARM(S_K1VmagThrs));
    S_K1VacrossTol = PARM(S_K1VmagErrTol);
    S_K1VerrTime   = PARM(S_K1VmagErrTm);

    SET_FV(SyncTstVsScl,      1.0F);     // scale factor to apply to commanded stator volts during sync
                                         // should be 1.0 except for lab testing
    SyncTstVsScl = MIN(1.0F, PARM(SyncTstVsScl));  // don't use setting > 1.0

    SET_FV(SyncK1TstSgmVmax,  0.05F);    // max pu volts on 6 kV winding to allow RunRdy in Sync_K1_Test mode

    Temp1 = PARM(L_LineFrq)*(0.5F*DelTm2);
    T1T2FbkScale=Temp1/(sinf(Temp1));


    SET_FV(GmIxWIyLim,   1.0F);

    SET_FV(TocPreset, false);

    if(PARM(TestMode) == PUMP_BACK)
    {
        SET_FV(TocDyThrshEnb, false);
    }
    else
    {
        SET_FV(TocDyThrshEnb, true);
    }
    GmTocAlmOnDl  = PARM(GmTocThrs)  - PARM(GmTocAlmOn);
    GmTocAlmOffDl = PARM(GmTocAlmOn) - PARM(GmTocAlmOff);

    SET_FV(DFIRat,      112.6F);
    SET_FV(DFConvCurr, 70.0F * (150.0e-6F / PARM(CnvLineL))); // Scale using the line inductance of ESS(150uH) and 3.0DFIG (120UH)
    DFConvISqr = PARM(DFConvCurr)*PARM(DFConvCurr);

    SET_FV(DFTocTau, 267.2); //Class 10 protection
    /////////////////////////////////////////////
    // Dffu current limits
    /////////////////////////////////////////////

    SET_FV(DFVllFilW,   1.339F*L_DelTm1Rcp);          // note: see DFIabcEstDtDly below
    SET_FV(DFTocThrs,   1.15F*PARM(DFIRat));
    SET_FV(DFTocAlmOn,  TOC_ALM_SCL*PARM(DFIRat));
    SET_FV(DFTocAlmOff, PARM(DFIRat));

    DFWTocT3   = DelTm3/PARM(DFTocTau);
    DFVllFilGn = L_DelTm1*PARM(DFVllFilW);


//  DFIScale = .985; //scaled to comp for over-comp of 1.339 filter gain in task 1 at IGCT converter type frequencies
                         //Filter is a bit 'peaky' at these frequencies (average of about 1.5% high)

    DFIScale = 1.0F;   // can't justify the .985 value (from ESS)

    DFCapW     = (0.3333333F*DFIScale*PARM(CnvLineDfC)*PARM(DFVllFilW)); //  .33333 is for for the DvDt calculation from line-line voltage to the line to neutral voltage:

                                                                        //  Eg: Vbc -Vab = Vb-Vc-Va+Vb
                                                                        //  Eg: Vbc -Vab = Vb-Vc-Va+Vb
                                                                        //                = 2Vb-(Vc+Va)
                                                                        //                = 3Vb
    DFCapWSq   =  DFCapW*DFCapW;

    SET_FV(DFIabcEstDtDly,   0.25F);   // Delay the brg currents by this amount to phase-align them with the
                                       // estimated DF currents (= C * dv/dt) which have a 1/2 dt delay but
                                       // the lead produced by DFVllFilW (resulting in DFVllFilGn > 1) cancels
                                       // about half of this; thus set DFIabcEstDtDly to 1/4 (of a dt.)

    L_IabcDFcomp1 = PARM(DFIabcEstDtDly) / L_T1FramesPerT2;   // FIR filter coeffiecents to apply to Task2 brg currents
    L_IabcDFcomp0 = 1.0F - L_IabcDFcomp1;                     //   "


    //PowerLimiterRules();    // see p0PwrLim.cpp


    SET_FV(IxWIyLim, (1.0F/PARM(L_TocTau)));


    SET_FV(IxWIyLim, 0.0167F);

    SET_FV(ILimitBW, 0.5F);
    SET_FV(R_TocLimGn, PARM(ILimitBW)*PARM(R_TocTau));  // rotor toc based volt reg current limit gain
    SET_FV(S_TocLimGn, PARM(ILimitBW)*PARM(S_TocTau));            // stator toc based volt reg current limit gain

    ////////////////////////////////
    // Run Wind Instrument Rules
    DummyInt = WindInstrumentRules();
    ////////////////////////////////

    SET_FV(WBLinSpilMrg, 0.5F);//1.0amps rms
    WBLinSpilMrgRcp = 1.0F/PARM(WBLinSpilMrg);


    ////////////////////////////////
    // Start GDPL  Rules
    ////////////////////////////////

    SET_FV(PllErrPwrOff, 0.05F);
    SET_FV(PllErrPwrGn,  2.0F);
    SET_FV(PllErrPwrFlW, 100.0F);
    SET_FV(PllErrPwrFlDnW,20.0F);
    PllErrTrqFlUpWdt = DelTm2 * PARM(PllErrPwrFlW);
    PllErrTrqFlDnWdt = DelTm2 * PARM(PllErrPwrFlDnW);


    if ( PARM(LvPwrCrvOvr) )
    {
        SET_FV(LvPwrP2VPu,   PARM(LvPwrP2VOv));
        SET_FV(LvPwrP2PwrPu, PARM(LvPwrP2PwrOv));
        SET_FV(LvPwrP3VPu,   PARM(LvPwrP3VOv));
        SET_FV(LvPwrP3PwrPu, PARM(LvPwrP3PwrOv));
    }
    else if ( PARM(MaxRealCurr) )
    {
        SET_FV(LvPwrP2VPu,   PARM(VregRefMin)-0.15F);
        SET_FV(LvPwrP2PwrPu, 0.75F);
        SET_FV(LvPwrP3VPu,   0.00F);
        SET_FV(LvPwrP3PwrPu, 0.00F);
    }
    else //Maximum real current
    {
        SET_FV(LvPwrP2VPu,   PARM(VregRefMin)-0.15F);
        SET_FV(LvPwrP2PwrPu, 0.75F);
        SET_FV(LvPwrP3VPu,   0.50F);
        SET_FV(LvPwrP3PwrPu, 0.00F);
    }

    //SET_FV(LvPwrMxPwrPu, TurbineOvrldPwrPu); from 2.75
    SET_FV(LvPwrMxPwrPu, PARM(R_TrqLimGen1)); //from macc

    SET_FV(LvPwrP1VPu,      PARM(VregRefMin));
    SET_FV(LvPwrP1PwrPu,    1.23F);

    SET_FV(WVmagPwrLm, 100.0F);   // filter omega for grid volts for torq limit down
    SET_FV(WVmagPwrUp,  20.0F);   // filter omega for grid volts for torq limit up



    if (PARM(LvPwrRcvyOvr))
    {
       SET_FV(LvPwrLmRcyRt,  PARM(LvPwrRcvyRat));
    }
    else
    {
        if ( PARM(GridHighZ))
        {
           SET_FV(LvPwrLmRcyRt,  3.0F);     // torq lim recovery rate (pu /sec) -- weak grid
        }
        else
        {
           SET_FV(LvPwrLmRcyRt,  5.0F);     // torq lim recovery rate (pu /sec) -- strong grid
        }
    }

    SET_FV(LvPwrLmRcyR0, 20.0F); //20 pu/sec up rate

    //No protection is provided by the code for incorrect parameter relationships, however, the user diagrams show the requirements.

    if ( (PARM(LvPwrP1VPu) - PARM(LvPwrP2VPu)) < 1.0e-6F)//essentially zero
    {
       LvPwrSlope1 = 0.0F;
    }
    else
    {
       LvPwrSlope1 = (PARM(LvPwrP1PwrPu) - PARM(LvPwrP2PwrPu))/(PARM(LvPwrP1VPu) - PARM(LvPwrP2VPu));
    }

    if ( (PARM(LvPwrP2VPu) - PARM(LvPwrP3VPu)) < 1.0e-6F)//essentially zero
    {
       LvPwrSlope2 = 0.0F;
    }
    else
    {
       LvPwrSlope2 = (PARM(LvPwrP2PwrPu) - PARM(LvPwrP3PwrPu))/(PARM(LvPwrP2VPu) - PARM(LvPwrP3VPu));
    }

    if ( PARM(LvPwrP3VPu) < 1.0e-6F)//essentially zero
    {
       LvPwrSlope3 = 0.0F;
    }
    else
    {
       LvPwrSlope3 = PARM(LvPwrP3PwrPu) /PARM(LvPwrP3VPu);
    }

    // Parameters for Pll angle compensation method as a function of power
    SET_FV(PwrPllVyErDb, 0.05F * L_VlnIn);
    SET_FV(PwrPllVyErGn, 0.0F);

    SET_FV(PwrPllAngGn, 0.40F);


    PwrPllFilGn = DelTm2*PARM(PllErrPwrFlW);

    SET_FV(SpdPllFilW, 0.5F);
    SpdPllFilGn = DelTm3 * PARM(SpdPllFilW);
    SET_FV(TrqPllMeldRq, 1.0F); //1.0 is all command
    OneMnTPllMeld = 1.0F - PARM(TrqPllMeldRq);

    SET_FV(PllErPLmRcyP0,  0.70F);

    if ( PARM(GridHighZ))
    {
        SET_FV(PllErrPwrRcyRt,  -3.0F);     // torq lim recovery rate (pu /sec) -- weak grid
    }
    else
    {
        SET_FV(PllErrPwrRcyRt,  -5.0F);     // torq lim recovery rate (pu /sec) -- strong grid
    }

    SET_FV(PllErrPwrRcyR0, -20.0F);
    SET_FV(PllErrPwrFlW,   100.0F);
    SET_FV(PllErrPwrFlDnW, 20.0F);

    ////////////////////////////////
    // End GDPL  Rules
    ////////////////////////////////

    SET_FV(FastBlkTst, false); // override to 'true' to test Fast Blocking function

    ////////////////////////////////////////////////
    // Torsional Detection / Protection Rules-Start
    ////////////////////////////////////////////////

    //  desired bandpass filter response is +/- 5% between 35 & 50 hz
    //  adjust next 3 parms to achieve this:


    SET_FV(R_TorSpdScl,  2.895F);             // Torsional detection filter output scale

    SET_FV(R_TorSpdLpW,      50.0F);          // Torsional detection lo-pass filter omega
    SET_FV(TorTstSlwRat,     0.30F);          // Torsional test slew rate in hz/sec
    SET_FV(R_TorTstW1,        0.0F);          // Torsional detection test omega 1
    SET_FV(R_TorTstW2,  200.0F*PI2);          // Torsional detection test omega 2
    SET_FV(R_TorTstAmpl,  0.0F);              // Torsional detection test oscillation amplitude
    SET_FV(R_TorTrqTst, -10000.0F);           // Torsional torque for sim mode


    R_TorSpdLpGn = DelTm2*PARM(R_TorSpdLpW); // filtering rectified sigal - don't need pre-warping

    SET_FV(R_TorSpRstW, 2.0F);
    SET_FV(R_TorSpRIntW, PARM(R_TorSpRstW)*0.333333F);
    R_TorSRIntGn = PARM(R_TorSpRIntW) * DelTm3;
    SET_FV(R_TorFrqRef, 51.0F);//hz

    SET_FV(R_TorProtScl,  R_TrqRat/10000.0F);       // Base values below are based on rated torque of 1.5, or 1520kw at 1440rpm
    TmpScl = PARM(R_TorProtScl);                    // or apx 10.0 kn*m

    if ( PARM(TorProtType) == TACH_BASED)
    {
       SET_FV(R_TorTrqThrs,  4900.0F * TmpScl);    // Torsional torque protection threshold  (torque peak)
       SET_FV(R_TorTrpThrs,  6000.0F * TmpScl);    // Torsional torque trip threshold -- nm*sec
       SET_FV(R_TorAlmOn,    2500.0F * TmpScl);    // Torsional slip alarm on threshold-- nm
       SET_FV(R_TorAlmOff,   1000.0F * TmpScl);    // Torsional slip alarm off treshold-- nm
       SET_FV(R_TorTrqFact, PARM(R_GenInertia));              // Torsional factor used in rules below
    }
    else //TORQUE_BASED
    {
       SET_FV(R_TorTrqThrs,  2000.0F * TmpScl);    // Torsional torque protection threshold  (torque peak)
       SET_FV(R_TorTrpThrs,  1500.0F * TmpScl);    // Torsional torque trip threshold -- nm*sec
       SET_FV(R_TorAlmOn,    1500.0F * TmpScl);    // Torsional slip alarm on threshold-- nm
       SET_FV(R_TorAlmOff,    500.0F * TmpScl);    // Torsional slip alarm off treshold-- nm
       SET_FV(R_TorTrqFact,    0.10F);             // Torsional factor used in rules below
    }

    SET_FV(R_TorTrqFilW,    0.050F);               // Torsional torque filter response
    SET_FV(R_TorReset, -1500.0F * TmpScl);         // Torsional speed integrator reset rate  (deg/sec)


    SET_FV(R_TorTrqSpGn, (60.0F/PI2)/PARM(R_TorTrqFact));
    SET_FV(R_TorTrqScl,  PARM(R_TorTrqFact) * (PI2/60.0F) * (PI2 * PARM(R_TorFrqRef)));


    SET_FV(R_TorSimEnb, false);
    SET_FV(R_TorSimMode, TORSIMSPDMODE);      // Mode type for torsional tests in sim mode. Speed mode or torque mode

    SET_FV(TorPTrpEnb, true);
    SET_FV(TorPAlmEnb, true);

    if ( PARM(TorPTrpEnb))
    {
       SET_FV(TorPTrpEnbFv, true);
       SET_FV(TorPAlmEnbFv, true);
    }
    else if ( PARM(TorPAlmEnb))
    {
       SET_FV(TorPTrpEnbFv, false);
       SET_FV(TorPAlmEnbFv, true);
    }
    else
    {
       SET_FV(TorPTrpEnbFv, false);
       SET_FV(TorPAlmEnbFv, false);
    }

    ///////////////////////////////////////////////
    // Torsional Detection / Protection Rules-End
    ///////////////////////////////////////////////

    if ( (PARM(SoftwareIPN) >= DFIG02_2400_5060_A) )
    {
       SET_FV(L_VdcRefMax,1.204131113F*PARM(L_VllRms)*SQRT2);  // 1175 at 690 input
    }
    else
    {
       SET_FV(L_VdcRefMax,1.152891491F*PARM(L_VllRms)*SQRT2);  // 1125 at 690 input
    }

    SET_FV(DbVmax, PARM(L_VdcRefMax + 25.0F) ); //1150/1200
    SET_FV(L_DbCelTstVlnW,     5.0F );
    L_DbCelTstVlnGn = PARM(L_DbCelTstVlnW) * DelTm2;
    SET_FV(DbAutoTstInh, false);       // normally not inhibit the DB auto ping
    if (!PARM(SimModeReq))
    {
        L_VdcFbkAdjMult = PARM(DbVfbkTau);
        SET_FV(DbGateSkewEnb, true);
    }
    else
    {
        L_VdcFbkAdjMult = 0.0F;          // no adjustment applied to Vdc feedback when calculating expected Vdb
        SET_FV(DbGateSkewEnb, false);   // not enable the DB gate skew
        L_VdbOnAdj = 0.0F;  // do not compensate Vdb in simmode
    }
    SET_FV(DbRTstPbPK, 1.06F);
    SET_FV(DbAtoPingVdcMin, 0.5F*SQRT2*PARM(L_VllIn));

    IOpn1AbsNWGn     =  PARM(IOpn1AbsNWW) * DelTm2;
    TorTstSlwRatDt   =  PARM(TorTstSlwRat) * PI2 * DelTm2;


    ///////////////////////////////////////////////////////////////////////////
    // Set S-domain gains and calculate z domain gains for 2nd order filters
    // High Pass Filter
    ///////////////////////////////////////////////////////////////////////////

    // Set final values for high pass filter S-domain gains
    SET_FV(TorSpdHpFilA0, 0.0F);
    SET_FV(TorSpdHpFilA1, 0.0F);
    SET_FV(TorSpdHpFilA2, 7.0361616e-6F);
    SET_FV(TorSpdHpFilB1, 3.7136069e-3F);
    SET_FV(TorSpdHpFilB2, 7.0361616e-6F);

    // Populate the s-domain structure from final values
    TorSpdFil2S.A0   = PARM(TorSpdHpFilA0);
    TorSpdFil2S.A1   = PARM(TorSpdHpFilA1);
    TorSpdFil2S.A2   = PARM(TorSpdHpFilA2);
    TorSpdFil2S.B1   = PARM(TorSpdHpFilB1);
    TorSpdFil2S.B2   = PARM(TorSpdHpFilB2);
    TorSpdFil2S.dt   = DelTm2;

    // Call the function
    calc_2nd_order(&TorSpdFil2S, &TorSpdFil2ZTmp);
    TorSpdFil2Z = TorSpdFil2ZTmp;

    // Set the Z-domain variables
    R_TorSpdHpA0 = TorSpdFil2Z.a0;
    R_TorSpdHpA1 = TorSpdFil2Z.a1;
    R_TorSpdHpA2 = TorSpdFil2Z.a2;
    R_TorSpdHpB1 = TorSpdFil2Z.b1;
    R_TorSpdHpB2 = TorSpdFil2Z.b2;

    ///////////////////////////////////////////////////////////////////////////
    // Set S-domain gains and calculate z domain gains for 2nd order filters
    // Low Pass Filter
    ///////////////////////////////////////////////////////////////////////////

    // Set final values for low pass filter S-domain gains
    SET_FV(TorSpdLpFilA0, 1.0F);
    SET_FV(TorSpdLpFilA1, 0.0F);
    SET_FV(TorSpdLpFilA2, 0.0F);
    SET_FV(TorSpdLpFilB1, 3.7136069e-3F);
    SET_FV(TorSpdLpFilB2, 7.0361616e-6F);

    // Populate the s-domain structure from final values
    TorSpdFil2S.A0   = PARM(TorSpdLpFilA0);
    TorSpdFil2S.A1   = PARM(TorSpdLpFilA1);
    TorSpdFil2S.A2   = PARM(TorSpdLpFilA2);
    TorSpdFil2S.B1   = PARM(TorSpdLpFilB1);
    TorSpdFil2S.B2   = PARM(TorSpdLpFilB2);
    TorSpdFil2S.dt   = DelTm2;

    // Call the function
    calc_2nd_order(&TorSpdFil2S, &TorSpdFil2ZTmp);
    TorSpdFil2Z = TorSpdFil2ZTmp;

    // Set the Z-domain variables
    R_TorSpdLpA0 = TorSpdFil2Z.a0;
    R_TorSpdLpA1 = TorSpdFil2Z.a1;
    R_TorSpdLpA2 = TorSpdFil2Z.a2;
    R_TorSpdLpB1 = TorSpdFil2Z.b1;
    R_TorSpdLpB2 = TorSpdFil2Z.b2;


    //CCU features
    CCUFeatBits.Svrt          = true;
    CCUFeatBits.Lvrt          = true;
    CCUFeatBits.Zvrt          = true;
    CCUFeatBits.Cvrt          = true;
    CCUFeatBits.ZvrtGen2Base  = true;
    CCUFeatBits.ZvrtGen3Base  = true;
    CCUFeatBits.ZvrtGen2Opt   = false;
    CCUFeatBits.FastDisc      = true;
    CCUFeatBits.FastBlock     = true;
    CCUFeatBits.PhaseJump     = true;
    CCUFeatBits.TorsVibProt   = true;
    CCUFeatBits.AntiIsland    = true;
    CCUFeatBits.RPNW          = true;
    CCUFeatBits.DelSpdPAct    = false;
    CCUFeatBits.DelSpdTrqAct  = true;
    CCUFeatBits.DriveTrainDm  = true;
    StructInit = (unsigned*) &CCUFeatBits;
    CCUFeature = *StructInit;

    //CCU configuration
    //Commented out bits are configured above.
    //CCUCnfgBits.Svrt        = false;
    //CCUCnfgBits.Lvrt        = false;
    //CCUCnfgBits.ZvrtGen2Base= false;
    //CCUCnfgBits.ZvrtGen2Opt = false;
    //CCUCnfgBits.Cvrt          = false;
    CCUCnfgBits.FastDisc      = PARM(FastDisEnb);
    CCUCnfgBits.FastBlock     = PARM(FastBlkEnb);
    CCUCnfgBits.PhaseJump     = PARM(PhsJmpTrpEnb);
    CCUCnfgBits.TorsVibProt   = PARM(TorPTrpEnb);
    CCUCnfgBits.AntiIsland    = true;
    CCUCnfgBits.RPNW          = true;
    CCUCnfgBits.DelSpdPAct    = false; // old for ESS, not uses here
    CCUCnfgBits.DelSpdTrqAct  = PARM(DelSpdTrqAct);
    CCUCnfgBits.DriveTrainDm  = true;
    StructInit = (unsigned*) &CCUCnfgBits;
    CCUConfig  = *StructInit;

    //Grid configuration
    if ((PARM(GridStrength) == STRONG) )
    {
       GridCnfgBits.GridStrength = 0;
    }
    else
    {
       GridCnfgBits.GridStrength = 1; //weak or very weak
    }
    GridCnfgBits.VRTSuportTyp = PARM(VRTSupptTyp);
    GridCnfgBits.QregFFEnb    = PARM(QregFFEnb);
    GridCnfgBits.AVRPresent   = PARM(AVRPresent);
    GridCnfgBits.LvrtIyOvr    = PARM(LvrtIyOvr);
    GridCnfgBits.HvrtIyOvr    = PARM(HvrtIyOvr);
    GridCnfgBits.VPRPVrefFix  = PARM(VPRPVrefFix);
    GridCnfgBits.VPRPIyRstAvg = PARM(VPRPIyRstAvg);
    GridCnfgBits.SeriesComp   = PARM(SeriesCmpEnb);
    GridCnfgBits.MaxRealCurr  = PARM(MaxRealCurr);
    GridCnfgBits.GDPLCurveOvr = PARM(LvPwrCrvOvr);
    GridCnfgBits.GDPLRcrvyOvr = PARM(LvPwrRcvyOvr);
    GridCnfgBits.GridFeatDef  = PARM(GridFeatDef);
    GridCnfgBits.VregMaxOvr   = PARM(VregMaxOvr);
    StructInit = (unsigned*) &GridCnfgBits;
    GridCodeCfg = *StructInit;

    CCUFwProduct = 3; //1 is WindDFIGe MACC based, 2 is DFIG01, and 3 is DFIG02
    CCUFwVersion = Code_ID.version;
    CCUHardware  = PARM(IPN_Version);
    if (PARM(IPN_Frame) == 1500)
    {
       CCUPwrMax    = 1700.0F;
    }
    else
    {
       CCUPwrMax    = (float) PARM(IPN_Frame);
    }

    if ( (PARM(SoftwareIPN) >= DFIG02_2400_5060_A) )
    {
       SET_FV(RpnwRatFv, 400.0F);
    }
    else
    {
       SET_FV(RpnwRatFv, 200.0F);
    }
    RpnwRat = PARM(RpnwRatFv); //for IoNet



    SET_FV(PrmPssTmrThr, 60.0F);
    SET_FV(GmFrqVltDlyTm, 0.025F);


    P0RuleCntr = P0RuleCntr + 1;

    // for CoreCommonSequencer
    if(PARM(TestMode) == PUMP_BACK)
    {
        SET_FV(LinkChgOKTm, 10.0e6);   //  hold off forever in pumpback mode
    }
    else
    {
        SET_FV(LinkChgOKTm, 10.0F);    //Time to indicate if DC Link stays low too long
    }

    return (SUCCESS); // 1 = success, 0 = fail;
}   // CommonAILRule()






//******************************************************************************
// Function:            DefaultCaptureBuff
// Execution Level:
// Execution Time:
// Formal Inputs:
// Formal Outputs:
// Global Inputs:
// Global Outputs:
// Return Value:        SUCCESS(1), FAIL(0)
// Function Calls:
// Abstract:
//
//******************************************************************************

//******************************************************************************
// Function:            DefaultGridMonitor
// Execution Level:
// Execution Time:
// Formal Inputs:
// Formal Outputs:
// Global Inputs:
// Global Outputs:
// Return Value:        SUCCESS(1), FAIL(0)
// Function Calls:
// Abstract:   This function sets grid monitor voltage and frequency user parameters
//          to the maximum capability of the converter. The AIL rule function is
//          called at the end of the function so that the rule constraint checks
//          can run to check the condition of the newly set paramters.  This should
//          really only be necessary when a code change is made to either this
//          functinon or that function, but is called each time this one runs
//          to prevent a 'hanging' coding error that might not otherwise get called
//          until the rule function runs.
//
//******************************************************************************
int     DefaultGridMonitor(void)
{


    CommonAILRule();


    return (SUCCESS);
}   // DefaultGridMonitor()


//******************************************************************************
//              VoltVarSetup
// Function:
// Execution Level:
// Execution Time:
// Formal Inputs:
// Formal Outputs:
// Global Inputs:
// Global Outputs:
// Return Value:        SUCCESS(1), FAIL(0)
// Function Calls:
// Abstract:
//
//******************************************************************************
float ZFeed;

int   VoltVarSetup(void)
{
    //  The code that was in this funciton was moved to P1Rule.c30 and allowed
    //  to run each time the rules run.  The 'CalcRequired" field was given to
    //  all of the input parameters.  The outputs were formerly user inputs and
    //  are now final values.  They could not be left here because they must be
    //  run at power-up.  There was no need to leave them in both places, but
    //  this funciton was left as a 'placeholder' for now.  The rules will run
    //  insead becaues all of the input parameters have 'calcrequired' fields.

    return (SUCCESS);
}





/******************************************************************************
*******************************************************************************
**
**  Function:
**        This function sets up relay output default values
**
******************************************************************************
******************************************************************************/
void RelayOutSetup(void)
{

}


/*****************************************************************
*FUNCTION:    calc_2nd_order
*EXECUTION:
*
*ABSTRACT:    calculates the co-efficients of a difference
*             equation which implements a second order filter
*             with transfer function.
*
*                         A2*s^2 + A1*s + A0
*                 H(s) =  ------------------
*                         B2*s^2 + B1*s + 1
*
*
*PARAMETERS:  Filt_In_Ptr  - pointer to structure containing
*                       A2, A1, A0, B2, B1, dt (s-domain values)
*             Filt_Out_Ptr - pointer to structure to contain
*                       a0, a1, a2, b1, b2 (z-domain filter coefficients)
*
*RETURN VALUE:  void
*
* Note: This funciton is now called from a rule file only when rules run
==================================================================*/
void calc_2nd_order(filt2_in_type *Filt_In_Ptr, filt2_out_type *Filt_Out_Ptr)
{
    float A0, A1, A2, B0, B1, B2, dt;
    float Cn, Cn_Sqr, Cd, Cd_Sqr, D, D_Rcp, W;

        A0 = Filt_In_Ptr->A0;
        A1 = Filt_In_Ptr->A1;
        A2 = Filt_In_Ptr->A2;
        B0 = 1.0F;
        B1 = Filt_In_Ptr->B1;
        B2 = Filt_In_Ptr->B2;
        dt = Filt_In_Ptr->dt;

        if ( (A2 == 0.0F) && (A1 == 0.0F) && (A0 == 0.0F) )
        {
            /* All 0's in numerator.  */
            Filt_Out_Ptr->a0 = 0.0F;
            Filt_Out_Ptr->a1 = 0.0F;
            Filt_Out_Ptr->a2 = 0.0F;
            Filt_Out_Ptr->b1 = 0.0F;
            Filt_Out_Ptr->b2 = 0.0F;
        }
        else if ( (A2 == 0.0F) && (B2 == 0.0F) )
        {
            /* 1st order transfer function in s-domain.
             * Use s = (1 - z^-1)/dt to get 1st order t.f in z-domain */
            if (A1*A0 != 0.0F)
                A1 = A1 * A1 * (1-expf(-A0*dt/A1)) / dt / A0; // prewarp numerator W
            if (B1*B0 != 0.0F)
                B1 = B1 * B1 * (1-expf(-B0*dt/B1)) / dt / B0; // prewarp denominator W

            D = B1+B0*dt;
            D_Rcp = 1.0F/D;
            Filt_Out_Ptr->a0 = (A1+A0*dt)* D_Rcp;
            Filt_Out_Ptr->a1 = (-A1)     * D_Rcp;
            Filt_Out_Ptr->b1 = (-B1)     * D_Rcp;
            Filt_Out_Ptr->a2 = 0.0F;
            Filt_Out_Ptr->b2 = 0.0F;
        }
        else
        {
            if (A0*A2 != 0.0F)
            {
                /* numerator is 2nd order - prewarp for bilinear transform */
                W  = sqrtf(A0/A2);
                Cn = W/tanf(0.5F * W * dt);
            }
            else
            {
                Cn = 2/dt;
            }
            Cn_Sqr = Cn*Cn;

            if (B0*B2 != 0.0F)
            {
                /* denominator is 2nd order - prewarp for bilinear transform  */
                W  = sqrtf(B0/B2);
                Cd = W/tanf(0.5F * W * dt);
            }
            else
            {
                Cd = 2.0F/dt;
            }
            Cd_Sqr = Cd*Cd;

            D = B0 + B1*Cd + B2*Cd_Sqr;
            D_Rcp = 1.0F/D;

        /* Bilinear transformation coefficients */
            Filt_Out_Ptr->a0 = (A0 + A1*Cn + A2*Cn_Sqr) * D_Rcp;
            Filt_Out_Ptr->a1 = (2.0F*A0 -2.0F*A2*Cn_Sqr)      * D_Rcp;
            Filt_Out_Ptr->a2 = (A0 - A1*Cn + A2*Cn_Sqr) * D_Rcp;
            Filt_Out_Ptr->b1 = (2.0F*B0 -2.0F*B2*Cd_Sqr)      * D_Rcp;
            Filt_Out_Ptr->b2 = (B0 - B1*Cd + B2*Cd_Sqr) * D_Rcp;
        }
}
