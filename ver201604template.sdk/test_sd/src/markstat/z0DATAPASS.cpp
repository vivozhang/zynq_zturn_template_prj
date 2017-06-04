///////////////////////////////////////////////////////////////////////////////
//
// TITLE        <Data pass definition>
//
// DESCRIPTION
//      This file contains the methods for the class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product
#include "product.h"



// Constants
//----------


// Externals
//----------


// Published Variables
//--------------------

//-------------------------------------------------------------------------
// Task1 Vars
CREATE_PUBVAR(GmPllIntGnT1          , float);
CREATE_PUBVAR(GmPllPrpGnT1          , float);
CREATE_PUBVAR(GmPllStateT1          , int);
CREATE_PUBVAR(GmPllStateZ1T1        , unsigned);
CREATE_PUBVAR(GmPllWMaxIntT1        , float);
CREATE_PUBVAR(GmPllWMaxT1           , float);
CREATE_PUBVAR(GmPllWMinIntT1        , float);
CREATE_PUBVAR(GmPllWMinT1           , float);
CREATE_PUBVAR(GmPllWRAFT1           , float);
CREATE_PUBVAR(GmPllWTrackT1         , float);


CREATE_PUBVAR(R_BrgPwrEnbT1         , unsigned);
CREATE_PUBVAR(R_FlxCmdDdtT1         , float);
CREATE_PUBVAR(R_FlxCmdT2T1          , float);
CREATE_PUBVAR(R_InnrRegEnbT1        , unsigned);
CREATE_PUBVAR(R_IxPdOutT1           , float);
CREATE_PUBVAR(R_IyCmd0T1            , float);
CREATE_PUBVAR(R_PdNegLimT1          , float);
CREATE_PUBVAR(R_PdPosLimT1          , float);
CREATE_PUBVAR(R_TrqLimNegT1         , float);
CREATE_PUBVAR(R_TrqLimPosT1         , float);
CREATE_PUBVAR(R_TrqRegStepT1        , float);
CREATE_PUBVAR(R_CCOpnCmdT1          , unsigned);
CREATE_PUBVAR(S_FlxMtrRcpT1         , float);
CREATE_PUBVAR(S_K1StatusT1          , unsigned);
CREATE_PUBVAR(S_PllIntGnT1          , float);
CREATE_PUBVAR(S_PllPrpGnT1          , float);
CREATE_PUBVAR(S_PllStateZ1T1        , int);
CREATE_PUBVAR(S_PllWMaxIntT1        , float);
CREATE_PUBVAR(S_PllWMaxT1           , float);
CREATE_PUBVAR(S_PllWMinIntT1        , float);
CREATE_PUBVAR(S_PllWMinT1           , float);
CREATE_PUBVAR(S_SimK1ClsdT1         , unsigned);
CREATE_PUBVAR(S_VOffsAccNT1         , int);
CREATE_PUBVAR(S_VOffsFilNT1         , int);
CREATE_PUBVAR(T2DoneOnceT1          , unsigned);

unsigned        R_SeqReqT1_PackedBits;
unsigned        R_SeqStBitsT1_PackedBits;
unsigned        R_MctlCmdT1_PackedBits;                  // Motor control command bits
unsigned        R_SeqStatT1_PackedBits;                  // this is used as a bit-coded word of type R_SeqStatBits


//-------------------------------------------------------------------------
// Task2 Vars
CREATE_PUBVAR(DFVaDvDtT2            , float);
CREATE_PUBVAR(DFVbDvDtT2            , float);
CREATE_PUBVAR(DFVcDvDtT2            , float);
CREATE_PUBVAR(GmAngEFbkT2           , float);
CREATE_PUBVAR(GmDscmVxNT2           , float);
CREATE_PUBVAR(GmDscmVxPT2           , float);
CREATE_PUBVAR(GmDscmVyNT2           , float);
CREATE_PUBVAR(GmDscmVyPT2           , float);
CREATE_PUBVAR(GmGridMonOKT2         , unsigned);
CREATE_PUBVAR(GmPllErrT2            , float);
CREATE_PUBVAR(GmPllVyErrT2          , float);
CREATE_PUBVAR(GmRevPwrTrpFT2        , unsigned);
CREATE_PUBVAR(GmVabT2               , float);
CREATE_PUBVAR(GmVbcT2               , float);
CREATE_PUBVAR(GmVMgFstPkT2          , float);
CREATE_PUBVAR(HvIyMaxDynT2          , float);
CREATE_PUBVAR(L_DCLinkInitT2        , unsigned);
CREATE_PUBVAR(L_IaT2                , float);
CREATE_PUBVAR(L_IbT2                , float);
CREATE_PUBVAR(L_IcT2                , float);
CREATE_PUBVAR(L_IMagT2              , float);
CREATE_PUBVAR(L_InnrRegORT2         , unsigned);
CREATE_PUBVAR(L_IxCmdT2             , float);
CREATE_PUBVAR(L_IxFbkT2             , float);
CREATE_PUBVAR(L_IyFbkT2             , float);
CREATE_PUBVAR(L_ModIndexT2          , float);
CREATE_PUBVAR(L_VabFbkT2            , float);
CREATE_PUBVAR(L_VabLineT2           , float);
CREATE_PUBVAR(L_VbcFbkT2            , float);
CREATE_PUBVAR(L_VbcLineT2           , float);
CREATE_PUBVAR(L_VdcFbkT2            , float);
CREATE_PUBVAR(L_VxFbkT2             , float);
CREATE_PUBVAR(L_VyFbkT2             , float);
CREATE_PUBVAR(LvIyMaxDynT2          , float);
CREATE_PUBVAR(R_CroBarActT2         , unsigned);
CREATE_PUBVAR(R_CroBarFltT2         , unsigned);
CREATE_PUBVAR(R_CroBarVdcLvlOffT2   , float);
CREATE_PUBVAR(R_CroBarVdcLvlOnT2    , float);

CREATE_PUBVAR(R_IxFbkT2             , float);
CREATE_PUBVAR(R_IxGndT2             , float);
CREATE_PUBVAR(R_IxVdcGnT2           , float);
CREATE_PUBVAR(R_IyFbkT2             , float);
CREATE_PUBVAR(R_IyGndT2             , float);
CREATE_PUBVAR(R_MrkLosFlagT2        , float);
CREATE_PUBVAR(R_MrkrPosT2           , int);
CREATE_PUBVAR(R_TacBadFlagT2        , float);
CREATE_PUBVAR(R_TrqCalT2            , float);
CREATE_PUBVAR(R_TrqCmdInT2          , float);
CREATE_PUBVAR(R_TrqCmdTrT2          , float);
CREATE_PUBVAR(R_UyCmdT2             , float);
CREATE_PUBVAR(R_VdcFbkT2            , float);
CREATE_PUBVAR(R_VxFbkT2             , float);
CREATE_PUBVAR(R_VyFbkT2             , float);
CREATE_PUBVAR(R_WElecT2             , float);
CREATE_PUBVAR(S_AngEFbkT2           , float);
CREATE_PUBVAR(S_FlxXFbkT2           , float);
CREATE_PUBVAR(S_FlxYFbkT2           , float);
CREATE_PUBVAR(S_IaSqrT2             , float);
CREATE_PUBVAR(S_IaT2                , float);
CREATE_PUBVAR(S_IbSqrT2             , float);
CREATE_PUBVAR(S_IbT2                , float);
CREATE_PUBVAR(S_IcSqrT2             , float);
CREATE_PUBVAR(S_IcT2                , float);
CREATE_PUBVAR(S_IxFbkT2             , float);
CREATE_PUBVAR(S_IyFbkT2             , float);
CREATE_PUBVAR(S_PllErrT2            , float);
CREATE_PUBVAR(S_PllWIntT2           , float);
CREATE_PUBVAR(S_PllWT2              , float);
CREATE_PUBVAR(S_VabT2               , float);
CREATE_PUBVAR(S_VbcT2               , float);
CREATE_PUBVAR(S_VMagT2              , float);
CREATE_PUBVAR(S_VxFbkT2             , float);
CREATE_PUBVAR(S_VyFbkT2             , float);
CREATE_PUBVAR(T1CntT2               , float);
CREATE_PUBVAR(VregHvMaxSlpT2        , float);
CREATE_PUBVAR(VregHvMinSlpT2        , float);
CREATE_PUBVAR(VregIyLimDynT2        , float);
CREATE_PUBVAR(VregLVIyMnDyT2        , float);
CREATE_PUBVAR(VregLvMaxSlpT2        , float);
CREATE_PUBVAR(VregLvMinSlpT2        , float);
CREATE_PUBVAR(VregMaxLimT2          , float);
CREATE_PUBVAR(VregMxMnDynT2         , float);

CREATE_PUBVAR(S_VanSqrAvg           , float);
CREATE_PUBVAR(S_VbnSqrAvg           , float);
CREATE_PUBVAR(S_VcnSqrAvg           , float);
CREATE_PUBVAR(S_VabSqrAvg           , float);
CREATE_PUBVAR(S_VbcSqrAvg           , float);
CREATE_PUBVAR(R_TrqTrkFil2          , float);
CREATE_PUBVAR(SpdPwrPllFl2          , float);


//-------------------------------------------------------------------------
// Task3 Vars
CREATE_PUBVAR(DFVaDvDtSqT3          , float);
CREATE_PUBVAR(DFVbDvDtSqT3          , float);
CREATE_PUBVAR(DFVcDvDtSqT3          , float);
CREATE_PUBVAR(GmDscmVMgNT3          , float);
CREATE_PUBVAR(GmDscmVMgPT3          , float);
CREATE_PUBVAR(GmDscmVxNT3           , float);
CREATE_PUBVAR(GmDscmVyNT3           , float);
CREATE_PUBVAR(GmDscmVyPT3           , float);
CREATE_PUBVAR(GmIaAl2T3             , float);
CREATE_PUBVAR(GmIaBe2T3             , float);
CREATE_PUBVAR(GmIaSqrT3             , float);
CREATE_PUBVAR(GmIbAl2T3             , float);
CREATE_PUBVAR(GmIbBe2T3             , float);
CREATE_PUBVAR(GmIbSqrT3             , float);
CREATE_PUBVAR(GmIcAl2T3             , float);
CREATE_PUBVAR(GmIcBe2T3             , float);
CREATE_PUBVAR(GmIcSqrT3             , float);
CREATE_PUBVAR(GmLVRTRcyIpT3         , unsigned);
CREATE_PUBVAR(GmPwrT3               , float);
CREATE_PUBVAR(GmQPwrT3              , float);
CREATE_PUBVAR(GmSLIaErSqT3          , float);
CREATE_PUBVAR(GmSLIbErSqT3          , float);
CREATE_PUBVAR(GmSLIcErSqT3          , float);
CREATE_PUBVAR(GmVanAl2T3            , float);
CREATE_PUBVAR(GmVanBe2T3            , float);
CREATE_PUBVAR(GmVanSqrT3            , float);
CREATE_PUBVAR(GmVbnAl2T3            , float);
CREATE_PUBVAR(GmVbnBe2T3            , float);
CREATE_PUBVAR(GmVbnSqrT3            , float);
CREATE_PUBVAR(GmVcnAl2T3            , float);
CREATE_PUBVAR(GmVcnBe2T3            , float);
CREATE_PUBVAR(GmVcnSqrT3            , float);
CREATE_PUBVAR(L_DcFndXT3            , float);
CREATE_PUBVAR(L_DcFndYT3            , float);
CREATE_PUBVAR(L_IMagT3              , float);

CREATE_PUBVAR(L_IregXUcT3           , float);
CREATE_PUBVAR(L_IregYUcT3           , float);
CREATE_PUBVAR(L_IxFbkT3             , float);
CREATE_PUBVAR(L_IxGndT3             , float);
CREATE_PUBVAR(L_IyGndT3             , float);
CREATE_PUBVAR(L_IyHrmFCalT3         , float);
CREATE_PUBVAR(L_ResX2T3             , float);
CREATE_PUBVAR(L_ResY2T3             , float);
CREATE_PUBVAR(L_VdcFbkT3            , float);
CREATE_PUBVAR(PLCRbtCmdT3           , float);
CREATE_PUBVAR(PwrGridT3             , float);
CREATE_PUBVAR(QPwrGridT3            , float);
CREATE_PUBVAR(R_BrgLossT3           , float);
CREATE_PUBVAR(R_CroBarActT3         , unsigned);

CREATE_PUBVAR(R_IxCmdT3             , float);
CREATE_PUBVAR(R_IxFbkT3             , float);
CREATE_PUBVAR(R_IxGndT3             , float);
CREATE_PUBVAR(R_IyFbkT3             , float);
CREATE_PUBVAR(R_IyGndT3             , float);
CREATE_PUBVAR(R_SpdT3               , float);
CREATE_PUBVAR(R_TorSpdInT3          , float);
CREATE_PUBVAR(R_TorSpdSFT3          , float);
CREATE_PUBVAR(R_TrqCmdInT3          , float);
CREATE_PUBVAR(R_TrqLimFT3           , float);
CREATE_PUBVAR(R_UxCmdT3             , float);
CREATE_PUBVAR(R_UyCmdT3             , float);
CREATE_PUBVAR(R_VdcFbkT3            , float);
CREATE_PUBVAR(R_VxFbkT3             , float);
CREATE_PUBVAR(R_VyFbkT3             , float);
CREATE_PUBVAR(S_IaSqrT3             , float);
CREATE_PUBVAR(S_IbSqrT3             , float);
CREATE_PUBVAR(S_IcSqrT3             , float);
CREATE_PUBVAR(S_IxFbkT3             , float);
CREATE_PUBVAR(S_IyFbkT3             , float);
CREATE_PUBVAR(S_IyMagnEstT3         , float);
CREATE_PUBVAR(S_K1StatusT3          , unsigned);
CREATE_PUBVAR(S_PllWT3              , float);
CREATE_PUBVAR(S_VanSqrT3            , float);
CREATE_PUBVAR(S_VbnSqrT3            , float);
CREATE_PUBVAR(S_VcnSqrT3            , float);
CREATE_PUBVAR(S_VabSqrT3            , float);
CREATE_PUBVAR(S_VbcSqrT3            , float);
CREATE_PUBVAR(S_VMagPeakT3          , float);
CREATE_PUBVAR(S_VMagT3              , float);
CREATE_PUBVAR(S_VxFbkT3             , float);
CREATE_PUBVAR(VPRPActT3             , unsigned);
CREATE_PUBVAR(VregCmdT3             , float);
CREATE_PUBVAR(VregFbkT3             , float);
CREATE_PUBVAR(VregOutT3             , float);
CREATE_PUBVAR(R_IxVdcGnT1,   float); // rotor vdc ix dynamic gain for task1

unsigned      R_SeqStBitsT3_PackedBits;



// DataPass
//------------
#if DISABLE_DATA_PASS

//=============================================================================
DATA_PASS(DFVaDvDt          , DFVaDvDtT2            , T1_T2, FILTERED);
DATA_PASS(DFVbDvDt          , DFVbDvDtT2            , T1_T2, FILTERED);
DATA_PASS(DFVcDvDt          , DFVcDvDtT2            , T1_T2, FILTERED);
DATA_PASS(GmAngElecFbk      , GmAngEFbkT2           , T1_T2, FILTERED);
DATA_PASS(GmDscmVxNFil      , GmDscmVxNT2           , T1_T2, FILTERED);
DATA_PASS(GmDscmVxPFil      , GmDscmVxPT2           , T1_T2, FILTERED);
DATA_PASS(GmDscmVyNFil      , GmDscmVyNT2           , T1_T2, FILTERED);
DATA_PASS(GmDscmVyPFil      , GmDscmVyPT2           , T1_T2, FILTERED);
DATA_PASS(GmIa              , GmIaT2                , T1_T2, FILTERED);
DATA_PASS(GmIb              , GmIbT2                , T1_T2, FILTERED);
DATA_PASS(GmIc              , GmIcT2                , T1_T2, FILTERED);
DATA_PASS(GmPllErr          , GmPllErrT2            , T1_T2, FILTERED);
DATA_PASS(GmPllVyErr        , GmPllVyErrT2          , T1_T2, FILTERED);
DATA_PASS(GmVab             , GmVabT2               , T1_T2, FILTERED);
DATA_PASS(GmVbc             , GmVbcT2               , T1_T2, FILTERED);
DATA_PASS(GmVMagFstPk       , GmVMgFstPkT2          , T1_T2, FILTERED);
DATA_PASS(L_Ia              , L_IaT2                , T1_T2, FILTERED);
DATA_PASS(L_Ib              , L_IbT2                , T1_T2, FILTERED);
DATA_PASS(L_Ic              , L_IcT2                , T1_T2, FILTERED);
DATA_PASS(L_IMagT1          , L_IMagT2              , T1_T2, FILTERED);
DATA_PASS(L_InnrRegOR       , L_InnrRegORT2         , T1_T2, UNFILTERED);
DATA_PASS(L_IxCmd0          , L_IxCmdT2             , T1_T2, FILTERED);
DATA_PASS(L_IxFbk           , L_IxFbkT2             , T1_T2, FILTERED);
DATA_PASS(L_IyFbk           , L_IyFbkT2             , T1_T2, FILTERED);
DATA_PASS(L_ModIndex        , L_ModIndexT2          , T1_T2, FILTERED);
DATA_PASS(L_Vab             , L_VabT2               , T1_T2, FILTERED);
DATA_PASS(L_VabFbk          , L_VabFbkT2            , T1_T2, FILTERED);
DATA_PASS(L_VabLineT1       , L_VabLineT2           , T1_T2, FILTERED);
DATA_PASS(L_Vbc             , L_VbcT2               , T1_T2, FILTERED);
DATA_PASS(L_VbcFbk          , L_VbcFbkT2            , T1_T2, FILTERED);
DATA_PASS(L_VbcLineT1       , L_VbcLineT2           , T1_T2, FILTERED);
DATA_PASS(L_VdcFbk          , L_VdcFbkT2            , T1_T2, FILTERED);
DATA_PASS(L_VxFbk           , L_VxFbkT2             , T1_T2, FILTERED);
DATA_PASS(L_VyFbk           , L_VyFbkT2             , T1_T2, FILTERED);
DATA_PASS(T1ExecCnt         , T1CntT2               , T1_T2, UNFILTERED);

//=============================================================================

DATA_PASS(R_IxFbk           , R_IxFbkT2             , T1A_T2, FILTERED);
DATA_PASS(R_IxGndT1         , R_IxGndT2             , T1A_T2, FILTERED);
DATA_PASS(R_IxregErr        , R_IxregErrT2          , T1A_T2, FILTERED);
DATA_PASS(R_IyFbk           , R_IyFbkT2             , T1A_T2, FILTERED);
DATA_PASS(R_IyGndT1         , R_IyGndT2             , T1A_T2, FILTERED);
DATA_PASS(R_MrkLosFlag      , R_MrkLosFlagT2        , T1A_T2, FILTERED);
DATA_PASS(R_MrkrPos         , R_MrkrPosT2           , T1A_T2, UNFILTERED);
DATA_PASS(R_TacBadFlag      , R_TacBadFlagT2        , T1A_T2, FILTERED);
DATA_PASS(R_TachFrac        , R_TachFracT2          , T1A_T2, UNFILTERED);
DATA_PASS(R_TachLtTmDt      , R_TachLtTmDtT2        , T1A_T2, UNFILTERED);
DATA_PASS(R_TachPulses      , R_TachPulsesT2        , T1A_T2, FILTERED);
DATA_PASS(R_TrqCal          , R_TrqCalT2            , T1A_T2, FILTERED);
DATA_PASS(R_TrqCmdIn        , R_TrqCmdInT2          , T1A_T2, FILTERED);
DATA_PASS(R_TrqCmdInTr      , R_TrqCmdTrT2          , T1A_T2, FILTERED);
DATA_PASS(R_TrqLimF         , R_TrqLimFT2           , T1A_T2, FILTERED);
DATA_PASS(R_UyCmd           , R_UyCmdT2             , T1A_T2, FILTERED);
DATA_PASS(R_Vab             , R_VabT2               , T1A_T2, FILTERED);
DATA_PASS(R_Vbc             , R_VbcT2               , T1A_T2, FILTERED);
DATA_PASS(R_VdcFbk          , R_VdcFbkT2            , T1A_T2, FILTERED);
DATA_PASS(R_VxFbk           , R_VxFbkT2             , T1A_T2, FILTERED);
DATA_PASS(R_VyFbk           , R_VyFbkT2             , T1A_T2, FILTERED);
DATA_PASS(R_WElec           , R_WElecT2             , T1A_T2, FILTERED);
DATA_PASS(S_AngElecFbk      , S_AngEFbkT2           , T1A_T2, FILTERED);
DATA_PASS(S_FlxXFbk         , S_FlxXFbkT2           , T1A_T2, FILTERED);
DATA_PASS(S_FlxYFbk         , S_FlxYFbkT2           , T1A_T2, FILTERED);
DATA_PASS(S_Ia              , S_IaT2                , T1A_T2, FILTERED);
DATA_PASS(S_IaSqr           , S_IaSqrT2             , T1A_T2, FILTERED);
DATA_PASS(S_Ib              , S_IbT2                , T1A_T2, FILTERED);
DATA_PASS(S_IbSqr           , S_IbSqrT2             , T1A_T2, FILTERED);
DATA_PASS(S_Ic              , S_IcT2                , T1A_T2, FILTERED);
DATA_PASS(S_IcSqr           , S_IcSqrT2             , T1A_T2, FILTERED);
DATA_PASS(S_IxFbk           , S_IxFbkT2             , T1A_T2, FILTERED);
DATA_PASS(S_IyFbk           , S_IyFbkT2             , T1A_T2, FILTERED);
DATA_PASS(S_PllErr          , S_PllErrT2            , T1A_T2, FILTERED);
DATA_PASS(S_PllW            , S_PllWT2              , T1A_T2, FILTERED);
DATA_PASS(S_PllWInt         , S_PllWIntT2           , T1A_T2, FILTERED);
DATA_PASS(S_Vab             , S_VabT2               , T1A_T2, FILTERED);
DATA_PASS(S_VanSqr          , S_VanSqrAvg           , T1A_T2, FILTERED);
DATA_PASS(S_Vbc             , S_VbcT2               , T1A_T2, FILTERED);
DATA_PASS(S_VbnSqr          , S_VbnSqrAvg           , T1A_T2, FILTERED);
DATA_PASS(S_VcnSqr          , S_VcnSqrAvg           , T1A_T2, FILTERED);
DATA_PASS(S_VabSqr          , S_VabSqrAvg           , T1A_T2, FILTERED);
DATA_PASS(S_VbcSqr          , S_VbcSqrAvg           , T1A_T2, FILTERED);
DATA_PASS(S_VMag            , S_VMagT2              , T1A_T2, FILTERED);
DATA_PASS(S_VxFbk           , S_VxFbkT2             , T1A_T2, FILTERED);
DATA_PASS(S_VyFbk           , S_VyFbkT2             , T1A_T2, FILTERED);

DATA_PASS(R_BrgCroBarCtl.CroBarAct,       R_CroBarActT2,       T1A_T2, UNFILTERED);
DATA_PASS(R_BrgCroBarCtl.CroBarFlt,       R_CroBarFltT2,       T1A_T2, UNFILTERED);
DATA_PASS(R_BrgCroBarCtl.CroBarVdcLvlOff, R_CroBarVdcLvlOffT2, T1A_T2, FILTERED);
DATA_PASS(R_BrgCroBarCtl.CroBarVdcLvlOn,  R_CroBarVdcLvlOnT2,  T1A_T2, UNFILTERED);

//=============================================================================
DATA_PASS(L_VregErr         , L_VregErrT3           , T1_T3,  FILTERED);
DATA_PASS(L_DelVdcRf        , L_DelVdcRfT3          , T1_T3,  FILTERED);
DATA_PASS(GmVanSqr          , GmVanSqrT3            , T1_T3, FILTERED);
DATA_PASS(GmVbnSqr          , GmVbnSqrT3            , T1_T3, FILTERED);
DATA_PASS(GmVcnSqr          , GmVcnSqrT3            , T1_T3, FILTERED);
DATA_PASS(GmIaSqr           , GmIaSqrT3             , T1_T3, FILTERED);
DATA_PASS(GmIbSqr           , GmIbSqrT3             , T1_T3, FILTERED);
DATA_PASS(GmIcSqr           , GmIcSqrT3             , T1_T3, FILTERED);
DATA_PASS(L_IxGndT1         , L_IxGndT3             , T1_T3, FILTERED);
DATA_PASS(L_IyGndT1         , L_IyGndT3             , T1_T3, FILTERED);

//=============================================================================
DATA_PASS(R_IxCmd           , R_IxCmdT3             , T1A_T3, FILTERED);
DATA_PASS(R_TrqLimF         , R_TrqLimFT3           , T1A_T3, FILTERED);
DATA_PASS(S_VMagPeak        , S_VMagPeakT3          , T1A_T3, FILTERED);

//=============================================================================
DATA_PASS(GmPllIntGn        , GmPllIntGnT1          , T2_T1, UNFILTERED);
DATA_PASS(GmPllPrpGn        , GmPllPrpGnT1          , T2_T1, UNFILTERED);
DATA_PASS(GmPllState        , GmPllStateT1          , T2_T1, UNFILTERED);
DATA_PASS(GmPllStateZ1      , GmPllStateZ1T1        , T2_T1, UNFILTERED);
DATA_PASS(GmPllWMax         , GmPllWMaxT1           , T2_T1, UNFILTERED);
DATA_PASS(GmPllWMaxInt      , GmPllWMaxIntT1        , T2_T1, UNFILTERED);
DATA_PASS(GmPllWMin         , GmPllWMinT1           , T2_T1, UNFILTERED);
DATA_PASS(GmPllWMinInt      , GmPllWMinIntT1        , T2_T1, UNFILTERED);
DATA_PASS(GmPllWRAF         , GmPllWRAFT1           , T2_T1, UNFILTERED);
DATA_PASS(GmPllWTrack       , GmPllWTrackT1         , T2_T1, UNFILTERED);
DATA_PASS(L_BrgPwrEnb       , L_BrgPwrEnbT1         , T2_T1, UNFILTERED);

DATA_PASS(R_BrgPwrEnb       , R_BrgPwrEnbT1         , T2_T1, UNFILTERED);
DATA_PASS(R_DcFndX          , R_DcFndXT1            , T2_T1, UNFILTERED);
DATA_PASS(R_DcFndY          , R_DcFndYT1            , T2_T1, UNFILTERED);
DATA_PASS(R_FlxCmd          , R_FlxCmdT2T1          , T2_T1, UNFILTERED);
DATA_PASS(R_FlxCmdDdt       , R_FlxCmdDdtT1         , T2_T1, UNFILTERED);
DATA_PASS(R_InnrRegEnb      , R_InnrRegEnbT1        , T2_T1, UNFILTERED);
DATA_PASS(R_IxFbkCompN      , R_IxFbkCompNT1        , T2_T1, UNFILTERED);
DATA_PASS(R_IxPdOut         , R_IxPdOutT1           , T2_T1, UNFILTERED);
DATA_PASS(R_IxVdcGnT2       , R_IxVdcGnT1           , T2_T1, UNFILTERED);
DATA_PASS(R_IyCmd0          , R_IyCmd0T1            , T2_T1, UNFILTERED);
DATA_PASS(R_IyFbkCompN      , R_IyFbkCompNT1        , T2_T1, UNFILTERED);
DATA_PASS(R_MctlCmd_PackedBits, R_MctlCmdT1_PackedBits, T2_T1, UNFILTERED);
DATA_PASS(R_PdNegLim        , R_PdNegLimT1          , T2_T1, UNFILTERED);
DATA_PASS(R_PdPosLim        , R_PdPosLimT1          , T2_T1, UNFILTERED);
DATA_PASS(R_SeqReq_PackedBits, R_SeqReqT1_PackedBits, T2_T1, UNFILTERED);
DATA_PASS(R_SeqStat_PackedBits, R_SeqStatT1_PackedBits, T2_T1, UNFILTERED);
DATA_PASS(R_SeqStBits_PackedBits, R_SeqStBitsT1_PackedBits, T2_T1, UNFILTERED);
DATA_PASS(R_TrqLimNeg       , R_TrqLimNegT1         , T2_T1, UNFILTERED);
DATA_PASS(R_TrqLimPos       , R_TrqLimPosT1         , T2_T1, UNFILTERED);
DATA_PASS(R_TrqRegStep      , R_TrqRegStepT1        , T2_T1, UNFILTERED);
DATA_PASS(R_TrqTrkFil2      , R_TrqTrkFil2T1        , T2_T1, UNFILTERED);
DATA_PASS(R_CCOpnCmd        , R_CCOpnCmdT1          , T2_T1, UNFILTERED);
DATA_PASS(S_FlxMtrRcp       , S_FlxMtrRcpT1         , T2_T1, UNFILTERED);
DATA_PASS(S_K1Status        , S_K1StatusT1          , T2_T1, UNFILTERED);
DATA_PASS(S_PllIntGn        , S_PllIntGnT1          , T2_T1, UNFILTERED);
DATA_PASS(S_PllPrpGn        , S_PllPrpGnT1          , T2_T1, UNFILTERED);
DATA_PASS(S_PllStateZ1      , S_PllStateZ1T1        , T2_T1, UNFILTERED);
DATA_PASS(S_PllWMax         , S_PllWMaxT1           , T2_T1, UNFILTERED);
DATA_PASS(S_PllWMaxInt      , S_PllWMaxIntT1        , T2_T1, UNFILTERED);
DATA_PASS(S_PllWMin         , S_PllWMinT1           , T2_T1, UNFILTERED);
DATA_PASS(S_PllWMinInt      , S_PllWMinIntT1        , T2_T1, UNFILTERED);
DATA_PASS(S_SimK1Clsd       , S_SimK1ClsdT1         , T2_T1, UNFILTERED);
DATA_PASS(S_VOffsAccN       , S_VOffsAccNT1         , T2_T1, UNFILTERED);
DATA_PASS(S_VOffsFilN       , S_VOffsFilNT1         , T2_T1, UNFILTERED);
DATA_PASS(T2DoneOnce        , T2DoneOnceT1          , T2_T1, UNFILTERED);

//=============================================================================
DATA_PASS(DFVaDvDtSq        , DFVaDvDtSqT3          , T2_T3, FILTERED);
DATA_PASS(DFVbDvDtSq        , DFVbDvDtSqT3          , T2_T3, FILTERED);
DATA_PASS(DFVcDvDtSq        , DFVcDvDtSqT3          , T2_T3, FILTERED);
DATA_PASS(GmDscmVMagN       , GmDscmVMgNT3          , T2_T3, FILTERED);
DATA_PASS(GmDscmVMagP       , GmDscmVMgPT3          , T2_T3, FILTERED);
DATA_PASS(GmDscmVxNT2       , GmDscmVxNT3           , T2_T3, FILTERED);
DATA_PASS(GmDscmVyNT2       , GmDscmVyNT3           , T2_T3, FILTERED);
DATA_PASS(GmDscmVyPT2       , GmDscmVyPT3           , T2_T3, FILTERED);
DATA_PASS(GmIaAl2           , GmIaAl2T3             , T2_T3, FILTERED);
DATA_PASS(GmIaBe2           , GmIaBe2T3             , T2_T3, FILTERED);
DATA_PASS(GmIbAl2           , GmIbAl2T3             , T2_T3, FILTERED);
DATA_PASS(GmIbBe2           , GmIbBe2T3             , T2_T3, FILTERED);
DATA_PASS(GmIcAl2           , GmIcAl2T3             , T2_T3, FILTERED);
DATA_PASS(GmIcBe2           , GmIcBe2T3             , T2_T3, FILTERED);
DATA_PASS(GmLVRTRcyIp       , GmLVRTRcyIpT3         , T2_T3, UNFILTERED);
DATA_PASS(GmPwrT2           , GmPwrT3               , T2_T3, FILTERED);
DATA_PASS(GmQPwrT2          , GmQPwrT3              , T2_T3, FILTERED);
DATA_PASS(GmSLIaErrSq       , GmSLIaErSqT3          , T2_T3, FILTERED);
DATA_PASS(GmSLIbErrSq       , GmSLIbErSqT3          , T2_T3, FILTERED);
DATA_PASS(GmSLIcErrSq       , GmSLIcErSqT3          , T2_T3, FILTERED);
DATA_PASS(GmVanAl2          , GmVanAl2T3            , T2_T3, FILTERED);
DATA_PASS(GmVanBe2          , GmVanBe2T3            , T2_T3, FILTERED);
DATA_PASS(GmVbnAl2          , GmVbnAl2T3            , T2_T3, FILTERED);
DATA_PASS(GmVbnBe2          , GmVbnBe2T3            , T2_T3, FILTERED);
DATA_PASS(GmVcnAl2          , GmVcnAl2T3            , T2_T3, FILTERED);
DATA_PASS(GmVcnBe2          , GmVcnBe2T3            , T2_T3, FILTERED);
DATA_PASS(L_DcFndX          , L_DcFndXT3            , T2_T3, FILTERED);
DATA_PASS(L_DcFndY          , L_DcFndYT3            , T2_T3, FILTERED);
DATA_PASS(L_IMagT2          , L_IMagT3              , T2_T3, FILTERED);

DATA_PASS(L_IregXUc         , L_IregXUcT3           , T1_T3, FILTERED);
DATA_PASS(L_IregYUc         , L_IregYUcT3           , T1_T3, FILTERED);
DATA_PASS(L_IxFbkT2         , L_IxFbkT3             , T2_T3, FILTERED);
DATA_PASS(L_IyHrmFCal       , L_IyHrmFCalT3         , T2_T3, FILTERED);
DATA_PASS(L_ResX2           , L_ResX2T3             , T2_T3, FILTERED);
DATA_PASS(L_ResY2           , L_ResY2T3             , T2_T3, FILTERED);
DATA_PASS(L_VdcFbkT2        , L_VdcFbkT3            , T2_T3, FILTERED);
DATA_PASS(PLCRbtCmdf        , PLCRbtCmdT3           , T2_T3, FILTERED);
DATA_PASS(R_BrgLossPwr      , R_BrgLossT3           , T2_T3, FILTERED);
DATA_PASS(R_CroBarActT2     , R_CroBarActT3         , T2_T3, UNFILTERED);

DATA_PASS(R_IxFbkT2         , R_IxFbkT3             , T2_T3, FILTERED);
DATA_PASS(R_IxGndT2         , R_IxGndT3             , T2_T3, FILTERED);
DATA_PASS(R_IyFbkT2         , R_IyFbkT3             , T2_T3, FILTERED);
DATA_PASS(R_IyGndT2         , R_IyGndT3             , T2_T3, FILTERED);
DATA_PASS(R_SeqStBits_PackedBits, R_SeqStBitsT3_PackedBits, T2_T3, UNFILTERED);
DATA_PASS(R_SpdT2           , R_SpdT3               , T2_T3, FILTERED);
DATA_PASS(R_TorSpdBpIn      , R_TorSpdInT3          , T2_T3, FILTERED);
DATA_PASS(R_TorSpdSqrF      , R_TorSpdSFT3          , T2_T3, FILTERED);
DATA_PASS(R_TrqCmdInT2      , R_TrqCmdInT3          , T2_T3, FILTERED);
DATA_PASS(R_UxCmd           , R_UxCmdT3             , T2_T3, FILTERED);
DATA_PASS(R_UyCmd           , R_UyCmdT3             , T2_T3, FILTERED);
DATA_PASS(R_VdcFbkT2        , R_VdcFbkT3            , T2_T3, FILTERED);
DATA_PASS(R_VxFbkT2         , R_VxFbkT3             , T2_T3, FILTERED);
DATA_PASS(R_VyFbkT2         , R_VyFbkT3             , T2_T3, FILTERED);
DATA_PASS(S_IaSqrT2         , S_IaSqrT3             , T2_T3, FILTERED);
DATA_PASS(S_IbSqrT2         , S_IbSqrT3             , T2_T3, FILTERED);
DATA_PASS(S_IcSqrT2         , S_IcSqrT3             , T2_T3, FILTERED);
DATA_PASS(S_IxFbkT2         , S_IxFbkT3             , T2_T3, FILTERED);
DATA_PASS(S_IyFbkT2         , S_IyFbkT3             , T2_T3, FILTERED);
DATA_PASS(S_IyMagnEst       , S_IyMagnEstT3         , T2_T3, FILTERED);
DATA_PASS(S_K1Status        , S_K1StatusT3          , T2_T3, UNFILTERED);
DATA_PASS(S_PllWT2          , S_PllWT3              , T2_T3, FILTERED);
DATA_PASS(S_VanSqrAvg       , S_VanSqrT3            , T2_T3, FILTERED);
DATA_PASS(S_VbnSqrAvg       , S_VbnSqrT3            , T2_T3, FILTERED);
DATA_PASS(S_VcnSqrAvg       , S_VcnSqrT3            , T2_T3, FILTERED);
DATA_PASS(S_VabSqrAvg       , S_VabSqrT3            , T2_T3, FILTERED);
DATA_PASS(S_VbcSqrAvg       , S_VbcSqrT3            , T2_T3, FILTERED);
DATA_PASS(S_VMagT2          , S_VMagT3              , T2_T3, FILTERED);
DATA_PASS(S_VxFbkT2         , S_VxFbkT3             , T2_T3, FILTERED);
DATA_PASS(VPRPAct           , VPRPActT3             , T2_T3, UNFILTERED);
DATA_PASS(VregCmd           , VregCmdT3             , T2_T3, FILTERED);
DATA_PASS(VregFbk           , VregFbkT3             , T2_T3, FILTERED);
DATA_PASS(VregOut           , VregOutT3             , T2_T3, FILTERED);

//=============================================================================
DATA_PASS(GmGridMonOK       , GmGridMonOKT2          ,T3_T2, UNFILTERED);
DATA_PASS(GmRevPwrTrpF      , GmRevPwrTrpFT2         ,T3_T2, UNFILTERED);
DATA_PASS(HvIyMaxDyn        , HvIyMaxDynT2           ,T3_T2, UNFILTERED);
DATA_PASS(L_DCLinkInit      , L_DCLinkInitT2         ,T3_T2, UNFILTERED);
DATA_PASS(LvIyMaxDyn        , LvIyMaxDynT2           ,T3_T2, UNFILTERED);
DATA_PASS(R_IxVdcGnDyn      , R_IxVdcGnT2            ,T3_T2, UNFILTERED);
DATA_PASS(R_TrqTrkFil       , R_TrqTrkFil2           ,T3_T2, UNFILTERED);
DATA_PASS(R_VMag            , R_VMagT2               ,T3_T2, UNFILTERED);
DATA_PASS(SpdPwrPllFil      , SpdPwrPllFl2           ,T3_T2, UNFILTERED);
DATA_PASS(VregHvMaxSlp      , VregHvMaxSlpT2         ,T3_T2, UNFILTERED);
DATA_PASS(VregHvMinSlp      , VregHvMinSlpT2         ,T3_T2, UNFILTERED);
DATA_PASS(VregIyLimDyn      , VregIyLimDynT2         ,T3_T2, UNFILTERED);
DATA_PASS(VregLVIyMnDy      , VregLVIyMnDyT2         ,T3_T2, UNFILTERED);
DATA_PASS(VregLvMaxSlp      , VregLvMaxSlpT2         ,T3_T2, UNFILTERED);
DATA_PASS(VregLvMinSlp      , VregLvMinSlpT2         ,T3_T2, UNFILTERED);
DATA_PASS(VregMaxLim        , VregMaxLimT2           ,T3_T2, UNFILTERED);
DATA_PASS(VregMxMnDyn       , VregMxMnDynT2          ,T3_T2, UNFILTERED);

#endif  // DISABLE_DATA_PASS
