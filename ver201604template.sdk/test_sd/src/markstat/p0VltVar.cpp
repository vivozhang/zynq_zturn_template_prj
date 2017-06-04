/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        $\PwmCtrl\Firmware\Pwm2\FW\A0FLXCMD.C30
* Language:    TI 'C'
* Copyright:   2003... GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file contains those functions which are associated with
*           conditioning of the y component of current.
* -----------------------------------------------------------------------------
*
* Functions in module:
* FlxCurrent                 - Flux Command and Current Generation
* FlxSatCurve                - Flux saturation curve
*
*==============================================================================
* 130504 DS  Create from a0flxcmd
*______________________________________________________________________________________
* Last Mod: November 10, 2011 0:56:40AM */


///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Generic fault checking in all tasks
//
// DESCRIPTION:
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      p0FltChk.xls, p0FltChk.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Includes Files
//---------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "a0FlxCmd.h"
#include "c1BrgCroBarCtl.h"


// Constants
//----------
#define IYBOOST_OFF 0
#define IYBOOST_ON  1

// Externals
//----------
extern cBrgCroBarCtl  R_BrgCroBarCtl;

extern unsigned VPRPActT3;

extern unsigned      R_SeqStat_PackedBits;

extern float        CI_L_LineFrq;


extern float        DelTm2;
extern float        DelTm3;
extern float        DelTm2Rcp;
extern float        DelTm3Rcp;
extern float        GmPwrT2;
extern float        GmPwrGridT3;
extern float        GmQPwrT2;
extern float        GmVMagPuT2;
extern float        GmVMagPuT3;
extern float        GmVMagT2;
extern float        GmWPllRcp;
extern float        L_VdcFbkT2;
extern float        HvIyMaxDynT2;
extern float        INomGridPk;
extern float        PwrCmdLine;
extern float        PwrCmdLineT3;
extern float        L_IxFbkT3;
extern float        L_IyHrmFCal;
extern float        L_IyHrmFFil;
extern float        L_IyTstCmd;
extern float        LineIyPFCmd;
extern float        LvIyMaxDynT2;
extern float        PhiAdj;
extern float        QregClmpSF;
extern float        R_IMagOLRcp;
extern float        R_IxregErrT2;
extern float        R_IyFbkT2;
extern float        R_IyLimCal;
extern float        R_KPoles;
extern float        R_Lr;
extern float        R_SpdFbk;
extern float        R_SpdFbkT3;
extern float        R_TocHiSel;
extern float        R_USqrFil;
extern float        R_VMagT2;
extern float        RtrLnXfrMld;
extern float        S_FlxYFbkT2;
extern float        S_IMagOLRcp;
extern float        S_IxFbkT2;
extern float        S_IyFbkT2;
extern float        S_IyMagnEstT3;
extern float        S_LmRcp;
extern float        S_Ls;
extern float        S_PllWInt;
extern float        S_R1;
extern float        S_RRatioRcp;
extern float        S_TocHiSel;
extern float        S_VMagT2;
extern float        S_VoltsRcp;
extern float        S_VxFbkT2;
extern float        S_VyFbkT2;
extern float        S_XmNomRcp;
extern float        S_XmRcp;
extern float        VregCmdT3;
extern float        VregFbkT3;
extern float        VregHvMaxSlpT2;
extern float        VregHvMinSlpT2;
extern float        VregIyLimDynT2;
extern float        VregLVIyMnDyT2;
extern float        VregLvMaxSlpT2;
extern float        VregLvMinSlpT2;
extern float        VregMaxLimT2;
extern float        VregMxMnDynT2;
extern float        VregOutT3;
extern float        WBDnTwrTemp;
extern float        WBLinILimMrg;
extern float        WBRtrILimMrg;
extern float        WBStrILimMrg;
extern float        GmIyFbkT2;
extern float        L_IyHrmFCalT3;
extern float        R_IxRat;
extern float        S_IxRat;
extern float        R_WElecT2;
extern unsigned     GmAsymRTIp;
extern unsigned     GmLineVLo;
extern unsigned     GmNSeqHi;
extern unsigned     IyXferDone;
extern unsigned     S_K1Status;
extern unsigned     L_LTestIp;
extern unsigned     L_Running;
extern unsigned     R_CTChkEnb;
extern unsigned     R_Running;
extern unsigned     RPNWRunning;
extern unsigned     RtrLnIyXfer;
extern unsigned     S_NSeqHi;
extern unsigned     S_PllProven;
extern unsigned     SysTmSec;
extern unsigned     VARModeCmd;
extern unsigned     WBLimAct;
extern unsigned     ZVRTQuickIp;
extern float        R_RPMSyncRcp;

extern float        GmVnMagPuMax;

extern unsigned     IslandDet;
extern unsigned     S_VMaxHiEnb;
extern unsigned     S_VMaxIyFast;
extern float        S_VMaxPuPeak;
extern float        S_IxFbk;

extern float        S_LSigma;

extern float        L_IxFbkT2;
extern float        L_VxFbkT2;
extern float        L_VyFbkT2;
extern float        XfmrRatioLnStr;
extern float        XfmrRatioLnStrRcp;
extern float        GmVyFbkT2;
extern float        L_VlnIn;
extern float        GmVxFbkT2;
extern float        GmIxGrid;
extern float        GmIxFbkT2;
extern float        R_ISpdTmpRcp;
extern float        S_ISpdTmpRcp;
extern float        TurbNpRatRcp;
extern float        PwrCmdPuT2;
extern float        S_VMaxIyCmd;
extern float        S_IMagLim;
extern float        LvIxRefFilT3;
extern float        GmIReserveSS;
extern float        L_IReserveSS;
extern float        FastDILimPu;
extern float        R_USqr;
extern unsigned     S_VMaxIyFast;
extern unsigned     R_FwkAct;

extern unsigned     R_SeqStBits_PackedBits;

extern float        ISysCmdSF;
extern unsigned     SimAcclAct;
extern   float      R_SpdFbkAvg;
extern float        VarPfScale;
extern float        QregMaxDyn;
extern float        QregMinDyn;
extern float        HvIyMaxDyn;
extern float        LvIyMaxDyn;
extern float        S_X1;
extern float        S_X2;
extern float        S_Xm;


// Parms
// ReactiveCurrentControlT2()
CREATE_PARM(WPwrGVARCmd,        float);     // grid power filter omega for VAR command
CREATE_PARM(QregGnPFMod,        float);     // gain modifier for var reg in power factor mode
CREATE_PARM(VARPfModSel,        unsigned);  // toggle between VAR and Pf mode

CREATE_PUBVAR(PfAdj,          float);          // Power factor adjustment
CREATE_PUBVAR(PfCmd,          float);          // Power factor command
CREATE_PUBVAR(VarPfScale,     float);          // VAR scaling
CREATE_PUBVAR(PwrGVARCmdGn,   float);          // Gain from WPwrGVARCmd

//Reactive Current Selection
CREATE_PARM(VRTSupptTyp,      unsigned);       // Voltage ride-through voltage support type
CREATE_PARM(VPRPEnb,          unsigned);       // VPRP enable -- user parameter


//VPRP specific
CREATE_PARM(VPRPDBandLv,      float);          // VPRP Low voltage Deadband
CREATE_PARM(VPRPDBandHv,      float);          // VPRP High voltage Deadband
CREATE_PARM(VPRPNoise,        float);          // VPRP noise (volt(pu)/sec
CREATE_PARM(VPRPNoise2,       float);          // VPRP noise (volt(pu)/sec (for disable at high voltage)
CREATE_PARM(VPRPHvDsbThr,     float);          // VPRP high voltage disable threshold
CREATE_PARM(VPRPPrpGnLv,      float);          // VPRP proportional gain for low voltage
CREATE_PARM(VPRPPrpGnHv,      float);          // VPRP proportional gain for high voltage
CREATE_PARM(VPRPFilW,         float);          // VPRP lp filter response (rad/sec)
CREATE_PARM(VPRPFilWFast,     float);          // VPRP lp filter response(fast) (rad/sec)
CREATE_PARM(VPRPWSlowTm,      float);          // VPRP lp filter time to switch to slow response time
CREATE_PARM(VPRPVrefFix,      unsigned);       // VPRP voltage fixed reference
CREATE_PARM(VPRPIyRstAvg,     unsigned);       // VPRP reactive current restore by 60sec avg.
CREATE_PARM(VPRPIyTolMax,     float);          // VPRP reactive current tolerance maximum
CREATE_PARM(VPRPIyTolMin,     float);          // VPRP reactive current tolerance manimum
CREATE_PARM(VPRPSmpCntTh,     unsigned);       // VPRP average count threshold for short sample time
CREATE_PARM(VPRPAvgTm,        float);          // VPRP average time
CREATE_PARM(VPRPSmpTm,        float);          // VPRP short sample time


CREATE_PARM(VregIyStpAdr,     float *);        // Reactive current step address

CREATE_PARM(VARRefAdr,        float *);        // VAR reference pointer
CREATE_PARM(QregIntGn0,       float);          // VAR regulator gain for RPNW and Sluggish pll gains
CREATE_PARM(QregIntGn,        float);          // VAR regulator gain for generator on line
CREATE_PARM(VregRefMax,       float);          // Voltage reg max ref (VAR reg max lim)
CREATE_PARM(VregRefMin,       float);          // Voltage reg min ref (VAR reg min lim)
CREATE_PARM(VregMaxExtOpt,    float);          // Voltage reg maximum -- User adjustable when AVR present
CREATE_PARM(VregMaxOvr,       unsigned);       // Voltage reg maximum reference rule over-ride
CREATE_PARM(VregLimHyst,      float);          // Hysterisis on vreg limit status for var reg anti-windup
CREATE_PARM(VregTstCmd,       float);          // volt reg test command
CREATE_PARM(VregIntGn,        float);          // Voltage regulator integral gain
CREATE_PARM(VregPrpGn,        float);          // Voltage regulator proportional gain
CREATE_PARM(VregIntGn0,       float);          // Voltage regulator integral gain for line only
CREATE_PARM(VregPrpGn0,       float);          // Voltage regulator proportional gain for line only
CREATE_PARM(VregIntGnIs,      float);          // Voltage regulator integral gain for islanding
CREATE_PARM(VregPrpGnIs,      float);          // Voltage regulator proportional gain for islanding
CREATE_PARM(VregPrpFilW,      float);          // Voltage regulator proportional low pass filter response (rad/sec)
CREATE_PARM(VregPrcFilW,      float);          // Voltage reg precondition filter omega
CREATE_PARM(VregNSqHiThr,     float);          // Voltage reg neg seq high threshold
CREATE_PARM(VregRefIsld,      float);          // Voltage reg reference for islanding
CREATE_PARM(VregIyIsldMx,     float);          // Voltage reg current clamp max for islanding
CREATE_PARM(VregIyIsldMn,     float);          // Voltage reg current clamp min for islanding
CREATE_PARM(FastDVrgIyMx,     float);          // Voltage reg current clamp max for Fast Disconnect

CREATE_PARM(VregFrzHThrs,     float);          // Voltage reg integrator feezed high threshold based on vline pu
CREATE_PARM(VregFrzLThrs,     float);          // Voltage reg integrator feezed low threshold based on vline pu
CREATE_PARM(VregHvKickGn,     float);          // Voltage regulator high voltage kick gain
CREATE_PARM(VregOutBias,      float);          // Voltage regulator output limit bias allows headroom over VregInt
CREATE_PARM(VregPrpMax,       float);          // Voltage regulator proportional maximum output
CREATE_PARM(VregPrpMin,       float);          // Voltage regulator proportional minimum output
CREATE_PARM(VregFrzFilW,      float);          // Voltage reg integrator feeze voltage filter response
CREATE_PARM(VnVregRedGn,      float);          // Voltage regulator ac voltage reduction gain
CREATE_PARM(VnVregRedThr,     float);          // Voltage regulator ac voltage reduction theshold


CREATE_PARM(PfLim,            float);          // power factor limit
CREATE_PARM(IyCmdForce,       float);          // Reactive current manual force amps.
CREATE_PARM(IyCmdOvr,         unsigned);       // Reactive current control over-ride forces us of L_IyCmdForce


CREATE_PARM(VregCorrW,        float);          // Voltage regulator correction resonse


//Volt-Var dynamic clamp parameters
CREATE_PARM(VregIyLim,        float);          // Volt-var SS positive output limit
CREATE_PARM(VregIyLimPu,      float);          // Volt-var SS positive output limit in per unit
CREATE_PARM(LvIyRat,          float);          // Defines max pu lvrt reactive current  (amps rms)
CREATE_PARM(HvIyRat,          float);          // Defines min hvrt reactive current  (amps rms)
CREATE_PARM(VregVpuFilW,      float);          // Voltage regulator dynamic clamp filter response (rad/sec)
CREATE_PARM(VregDVFilOff,     float);          // Voltage regulator dynamic clamp dv/dt offset
CREATE_PARM(VregDVGn,         float);          // Voltage regulator dynamic clamp dv/dt gain
CREATE_PARM(LvrtIyMaxOvr,     float);          // User over-ride for max allowed reactive lvrt current
CREATE_PARM(HvrtIyMaxOvr,     float);          // User over-ride for in allowed reactive hvrt current
CREATE_PARM(LvrtIyOvr,        unsigned);       // User parameter to turn on use of P.LvrtIyMaxOvr
CREATE_PARM(HvrtIyOvr,        unsigned);       // User parameter to turn on use of P.HvrtIyMaxOvr
CREATE_PARM(VregMxMnDiff,     float);          // Voltage regulator dynamic cmamp max/min delta
CREATE_PARM(VregLvSlope,      float);          // Voltage regulator dynamic cmamp low voltage slope
CREATE_PARM(VregHvSlope,      float);          // Voltage regulator dynamic cmamp high voltage slope
CREATE_PARM(VregMnLvSlp,      float);          // Voltage regulator dynamic cmamp Vpu where low voltage min slope begins.
CREATE_PARM(VregLVMnAb,       float);          // Voltage regulator dynamic dlamp low voltage absolute minimum current
CREATE_PARM(VregMxHvSlp,      float);          // Voltage regulator dynamic cmamp Vpu where high voltage max slope begins.
CREATE_PARM(VregMaxLv1,       float);          // Voltage regulator dynamic cmamp Vpu for lv range at which max limit starts increasing
CREATE_PARM(VregMaxLv2,       float);          // Voltage regulator dynamic cmamp Vpu for lv range at which max limit stops increasing
CREATE_PARM(VregMinHv1,       float);          // Voltage regulator dynamic cmamp Vpu for hv range at which min limit starts decreasing
CREATE_PARM(VregMinHv2,       float);          // Voltage regulator dynamic cmamp Vpu for hv range at which min limit stops decreasing
CREATE_PARM(IyMaxLimSS,       float);          // Voltage regulator Iy current command dynamic clamp limit;
CREATE_PARM(LvIyRatNW,        float);          // Defines max pu lvrt reactive current  (amps rms) for RPNW
CREATE_PARM(VregIyLimNW,      float);          // Volt-var output negative limit for RPNW
CREATE_PARM(VrgIyLmNWAsm,     float);          // Volt-var output limit for RPNW asymmetric faults
CREATE_PARM(HvIyRatNW,        float);          // Defines min hvrt reactive current for RPNW (amps rms)
CREATE_PARM(VregLvSlpNW,      float);          // Voltage regulator dynamic cmamp low voltage slope for RPNW
CREATE_PARM(VregHvSlpNW,      float);          // Voltage regulator dynamic cmamp high voltage slope for RPNW
CREATE_PARM(VregMxMnDfNW,     float);          // Voltage regulator dynamic cmamp max/min delta for RPNW
CREATE_PARM(VregLVMnAbNW,     float);          // Voltage regulator dynamic dlamp low voltage absolute minimum current for RPNW
CREATE_PARM(VregNSeqIyLm,     float);          // Votlage reg current limit when high phase imbalance
CREATE_PARM(VregNSeqIyNw,     float);          // Votlage reg current limit when high phase imbalance in now wind mode
CREATE_PARM(LvIxW,            float);          // Ix filter response for reserve Iy calc
CREATE_PARM(LvIxRLUpPu,       float);          // Ix filter max upward rate change
CREATE_PARM(LvIxRLDnPu,       float);          // Ix filter max downward rate change
//End Low Voltage torque reduction


// Volt-Var Feed-Forward
CREATE_PARM(QregFFGn,         float);           // Var regulator feed-forward gain
CREATE_PARM(QregTstCmd,       float);           // Var regulator Step Test Command
CREATE_PARM(QregFFW,          float);           // Var regulator omega for low pass part of hp filter
CREATE_PARM(QregFFEnb,        unsigned);        // VAR regulator feed-forward enable

// Volt boost vs. Power Rise
CREATE_PARM(VBstPVMaxPu,      float);           // Voltage boost voltage threshold for lowering voltage boost clamp -- Vpu
CREATE_PARM(VBstPLimGn,       float);           // Voltage boost gain for lowering voltage boost clamp -- pu/pu
CREATE_PARM(VBstPLmUpW,       float);           // Voltage boost up omega for lowering voltage boost clamp - rad/sec
CREATE_PARM(VBstPLmDnW,       float);           // Voltage boost down omega for lowering voltage boost clamp - rad/sec
CREATE_PARM(VBstPGn,          float);           // Voltage boost gain power gain -- Vpu/(Ppu/sec)
CREATE_PARM(VBstPOffs,        float);           // Voltage boost gain power gain offset

CREATE_PARM(LvIyKSpdThr,      float);           // Iy boost function speed threshold
CREATE_PARM(LvIyKPulseTm,     float);           // Iy boost function pulse time
CREATE_PARM(LvIyKickMax,      float);           // Iy boost function maximum kick
CREATE_PARM(LvIyKPrmTm,       float);           // Iy boost function time
CREATE_PARM(LvIyKickRstK,     float);           // Iy boost function reset
CREATE_PARM(LvIyKickDcy,      float);           // Iy boost function kick decay
CREATE_PARM(LvIyKickGn,       float);           // Iy boost function kick gain
CREATE_PARM(LvIyKickVThr,     float);           // Iy boost function kick threshold
CREATE_PARM(LvIyKickDvGn,     float);           // Iy boost function kick

CREATE_PARM( LvIyKTmOvr,    unsigned );         // Iy boost kick time over
CREATE_PARM( LvIyKTmMxOvr,  float    );         // Iy boost kick time over max
CREATE_PARM( LvIyKIOvr,     unsigned );         // Iy boost kick current over
CREATE_PARM( LvIyKIMxOvr,   float    );         // Iy boost kick current over max

// Grid feature default
CREATE_PARM(GridFeatDef,      unsigned);        // Grid feature default boolean


// Field weakening
CREATE_PARM(R_FwkMargin,       float);           // field weakening margin to ac voltage reference
CREATE_PARM(R_ModLimFbWD,      float);           // field weakening feedback omega down
CREATE_PARM(R_ModLimFbW2,      float);           // field weakening feedback omega 2
CREATE_PARM(R_ModLmFbW2D,      float);           // field weakening feedback omega 2 down

CREATE_PARM(R_WeSlipRat,       float);           // hi-slip: rated electrical slip freq
CREATE_PARM(R_WeSlipFilW,      float);           // hi-slip: rated electrical slip freq
CREATE_PARM(VmagHiSlipW,       float);           // hi-slip: filter omega for GmVMag
CREATE_PARM(VmagHiSlipLdOfs,   float);           // hi-slip: lead offset for GmVMag
CREATE_PARM(VmagHiSlipLdK,     float);           // hi-slip: lead gain for GmVMag
CREATE_PARM(VmagHiSlipLdLim,   float);           // hi-slip: lead limit for GmVMag
CREATE_PARM(VmagHiSlipMin,     float);           // hi-slip: min clamp for GmVMag
CREATE_PARM(VregMaxHiSlipMx,   float);           // hi-slip: VregMaxLim maximum
CREATE_PARM(VregMaxHiSlipGn,   float);           // hi-slip: VregMaxLim (reduction) gain

CREATE_PARM(R_ModLimFbkW,       float);     // field weakening feedback omega
CREATE_PARM(R_FwkIntGn,         float);     // field weakening integral gain
CREATE_PARM(R_VMaxIntGn,        float);     // rotor voltage max integral gain
CREATE_PARM(R_VMaxPu,           float);     // rotor voltage max integral gain
CREATE_PARM(R_FwkRtLim,         float);     // field weakening downward rate limit
CREATE_PARM(R_FwkMinLim,        float);     // field weakening minimum limit
// Asymmetry Iy Current reduction speed requirements
CREATE_PARM(IyAsySpdOn,         float);     // Speed above which reactive current is driven negative if
CREATE_PARM(IyAsySpdOff,        float);     // Speed below which function that drives reactive current neg during asym faults is turned off
CREATE_PARM(IyAsyIxRef,         float);     // Reactive current rotor ix regulator error reference for asymmetric faults
CREATE_PARM(IyAsyIxW,           float);     // Response for reactive current reduction on asymmetric voltage based on excess rotor ix error
CREATE_PARM(IyAsyIxMin,         float);     // Minimum reactive current allowed for reactive current reduction based on excess ix error during asymmetric faults
CREATE_PARM(VrgIyLmAsm,         float);     // Timed over-current Iy reduction threshold for asymmetric faults

// Modulation index limiting based on Iy needed to hold mod index.
CREATE_PARM(VrgIyMxA1,          float);     // coefficient 1 for final Iy output
CREATE_PARM(VrgIyMxA2,          float);     // coefficient 2 for final Iy output
CREATE_PARM(VrgIyMxA3,          float);     // coefficient 3 for final Iy output
CREATE_PARM(VrgIyMxModThr,      float);     // Modulation index reference
CREATE_PARM(VrgIyMxModFilW,     float);     // Modulation index fileter W
CREATE_PARM(VrgIyMxRModLimUp,   float);     // Up rate for up/down integrator of modulation index error
CREATE_PARM(VrgIyMxRModLimDn,   float);     // Down rate rate for up/down integrator of modulation index error
CREATE_PARM(VrgIyMxRModLimMax,  float);     // Upper limit for up/down filter of modulation index error
CREATE_PARM(VrgIyMxRModLimMin,  float);     // Lower limit for up/down filter of modulation index error
CREATE_PARM(VrgIyMxRModEnb,     unsigned);  // Lower limit for up/down filter of modulation index error
CREATE_PARM(VregMaxHiSlipEnb,   unsigned);  // Enable high slip algorithm
CREATE_PARM(VrgIyMxSlipMin,     float);     // Minimum absolute slip frequency value to prevent divide by zero
CREATE_PARM(VrgIyMxSlipFilW,    float);     // filter response for per unit slip filter

CREATE_PARM(CTChkEnbDoDlyTm,   float);      // D.O. delay for CTChkEnb for enabling volt-var reg
CREATE_PARM(TrqEnbStPuDlyTm,   float);      // P.U. delay for TrqEnbStat for enabling volt-var reg



// Variables
//----------

/////////////////////////////////////////////////////////////////////
// Volt-Var regulaton
/////////////////////////////////////////////////////////////////////
CREATE_PUBVAR(VregIyStep,      float );              /* Reactive current regulator step */

CREATE_PUBVAR(QregCmd ,        float );              /* VAR regulator cmd */
CREATE_PUBVAR(QregCmdPu ,      float );              /* VAR regulator cmd, pu */
CREATE_PUBVAR(QregErr ,        float );              /* VAR regulator error */
CREATE_PUBVAR(QregDelInt ,     float );
CREATE_PUBVAR(VregErr ,        float );              /* Volt regulator error */
CREATE_PUBVAR(VregFbk ,        float );              /* Volt regulator fdbk */
CREATE_PUBVAR(VregInt ,        float );              /* Volt regulator integrator */
CREATE_PUBVAR(VregOut ,        float );              /* Volt regulator output (pu) */
CREATE_PUBVAR(VregOutAPk ,     float );              /* Volt regulator output (amps) */
CREATE_PUBVAR(QregIntGnDyn,    float );
CREATE_PUBVAR(VregIntGnDyn,    float );
CREATE_PUBVAR(VregPrcFilGn,    float );
CREATE_PUBVAR(VnVregMagDel,    float ) = 0.0;        /* Vneg delta for integrator*/

CREATE_PUBVAR(QregFbk ,        float );              // VAR regulator feedback */
CREATE_PUBVAR(QregFbkPu ,      float );              // VAR regulator feedback, pu */
CREATE_PUBVAR(VregFrzVFil,     float );              // Voltage integrator frz detection filter
CREATE_PUBVAR(VregFrzInt,      unsigned ) =false;    // Voltage integrator frz signal
CREATE_PUBVAR(VregLimN,        unsigned );
CREATE_PUBVAR(VregLimP,        unsigned );
CREATE_PUBVAR(QregFrozen,      unsigned );
CREATE_PUBVAR(VregPrpFil,      float );              // Volt regulator proportional path filter*/
CREATE_PUBVAR(VregPrp1,        float );              // Volt regulator prop, pre-filter*/
CREATE_PUBVAR(VregPrpDel,      float );              // Volt regulator prop path delta from previous path*/
CREATE_PUBVAR(VregPrpFilGn,    float );              // Volt regulator prop path low pass filter gain*/
CREATE_PUBVAR(VregPrpGnDyn ,   float );
CREATE_PUBVAR(VregVpuFlErr,    float );              // Kicker error
CREATE_PUBVAR(VregHvKick,      float );              // Voltage reg high voltage kicker
CREATE_PUBVAR(VregCorrGn   ,   float );
CREATE_PUBVAR(VregCorrInt   ,  float );
CREATE_PUBVAR(VregCorrErr   ,  float );
CREATE_PUBVAR(VregOutMax   ,   float );
CREATE_PUBVAR(VregOutMin   ,   float );
CREATE_PUBVAR(VregIyCmd    ,   float );              // Total reactive current command for converter
CREATE_PUBVAR(VregIyCmd0   ,   float );              // Reactive current command transformer

/////////////////////////////////////
// VPRPSupportT2
/////////////////////////////////////
CREATE_PUBVAR(VPRPAct   ,      unsigned )=false;     // VPRP logic is active
CREATE_PUBVAR(VPRPActLv ,      unsigned )=false;     // VPRP logic is active for low voltage
CREATE_PUBVAR(VPRPActHv ,      unsigned )=false;     // VPRP logic is active for high voltage
CREATE_PUBVAR(VPRPVFbkFil,     float );              // VPRP filtered voltage feedback
CREATE_PUBVAR(VPRPIyCmdFil,    float );              // VPRP filtered voltage command
CREATE_PUBVAR(VPRPVErr,        float );              // VPRP voltage error
CREATE_PUBVAR(VPRPVErrLv,      float );              // VPRP voltage error for low voltage
CREATE_PUBVAR(VPRPVErrHv,      float );              // VPRP voltage error for high voltage
CREATE_PUBVAR(VPRPLVsecErr,    float );              // VPRP volt-sec error for low voltage
CREATE_PUBVAR(VPRPHVsecErr,    float );              // VPRP volt-sec error for high voltage
CREATE_PUBVAR(VPRPHVsecEr2,    float );              // VPRP volt-sec error for high voltage for hv disable
CREATE_PUBVAR(VPRPIyDelPu,     float );              // VPRP voltage delta
CREATE_PUBVAR(VPRPIyRef,       float );              // VPRP pre-adjusted output
CREATE_PUBVAR(VPRPIyMax,       float );              // VPRP output maximum
CREATE_PUBVAR(VPRPIyMin,       float );              // VPRP output minimum

//////////////////////////////////
// Var Reg Feed-Forward - Start
//////////////////////////////////
CREATE_PUBVAR(QregFFWDt,       float );              // Var regulator feed-forward P.QregFFW*dt
CREATE_PUBVAR(QregFFLpFil,     float );              // Var regulator feed-forward low-pass filter output
CREATE_PUBVAR(QregFFHpFil,     float );              // Var regulator feed-forward high-pass filter output
CREATE_PUBVAR(QregFF,          float );              // Var regulator feed-forward output

/////////////////////////////////////////////////
// Var Reg Voltage Boost vs. Power Rise - Start
/////////////////////////////////////////////////
CREATE_PUBVAR(R_ModLimFbkGn,   float );
CREATE_PUBVAR(R_ModLimFbGnD,   float );
CREATE_PUBVAR(R_ModLimFbGn2,   float );
CREATE_PUBVAR(R_ModLmFbGn2D,   float );
CREATE_PUBVAR(VBstPGnDtRcp,    float );              // Voltage boost Gain*det reciprocal (DelTm2*Ppu/sec)
CREATE_PUBVAR(VBstPLmUpWdt,    float );              // Voltage boost slow up/fast down filter "Up" omega
CREATE_PUBVAR(VBstPLmDnWdt,    float );              // Voltage boost slow up/fast down filter "Down" omega
CREATE_PUBVAR(QregFFGnDt,      float );
CREATE_PUBVAR(PwrCmdPuZ1,      float );              // Real power command z-1 value
CREATE_PUBVAR(VBstVpuUc,       float );              // Voltage boost per unit voltage boost pre-offset
CREATE_PUBVAR(VBstVpuRaw,      float );              // Voltage boost per unit voltage boost raw (pre filter)
CREATE_PUBVAR(VBstPVLimFil,    float );              // Voltage boost low pass filter upper clamp
CREATE_PUBVAR(VBstPVpu,        float );              // Output of function -- Vpu
CREATE_PUBVAR(VBstPVpuLim,     float );              // Input to slow up/fast down filter -- Vpu(Ppu/sec)

CREATE_PUBVAR(VregRfMaxDyn,    float );              /* voltage regulator max dynamic*/
CREATE_PUBVAR(IyPFCmd,         float );
CREATE_PUBVAR(IyPFCmdPu,       float );
CREATE_PUBVAR(IyPFCmdPuT3,     float );
CREATE_PUBVAR(PwrGridVCFil,    float );              /* grid power filtered for var caommand */
CREATE_PUBVAR(VregVpuFil,      float );
CREATE_PUBVAR(VregVpuLimIn,    float );
CREATE_PUBVAR(VregFilIn,       float );

CREATE_PUBVAR(LvIxRLUp,        float );
CREATE_PUBVAR(LvIxRLDn,        float );
CREATE_PUBVAR(LvIxRefFil,      float );

CREATE_PUBVAR(IyAsyIxErr,      float );
CREATE_PUBVAR(IyAsyIxLim,      float );
CREATE_PUBVAR(VregMaxLim0,     float );              // Maximum limit output for stage 0
CREATE_PUBVAR(VregMinLim0,     float );              // Minimum limit output for stage 0

CREATE_PUBVAR(IyCnvLimMx,      float );              // Maximum converter reactive current limit
CREATE_PUBVAR(IyCnvLimMn,      float );              // Minimum converter reactive current limit
CREATE_PUBVAR(LvIyReserv,      float );              // Low voltage iy reserve
CREATE_PUBVAR(LvIyReservP,     float );              // Low voltage iy reserve +ve
CREATE_PUBVAR(LvIyReservN,     float );              // Low voltage iy reserve -ve
CREATE_PUBVAR(S_LvIyReserv,    float );              // Stator Low voltage iy reserve
CREATE_PUBVAR(INomGrdPkRcp,    float );              // Reciprocal of nomina grid peak current


//Low and High voltage current reference generation
CREATE_PUBVAR(VregMaxLim,      float );
CREATE_PUBVAR(VregMinLim,      float );
CREATE_PUBVAR(VregMinLimNom,   float );              // nominal VregMinLim (when not Fwk)
CREATE_PUBVAR(VregMaxSel0,     float );
CREATE_PUBVAR(VregMaxSel1,     float );
CREATE_PUBVAR(VregMinSel0,     float );
CREATE_PUBVAR(VregMinSel1,     float );
CREATE_PUBVAR(VregIyNSeq,      float );
CREATE_PUBVAR(VregNSeqSpdA,    unsigned )=false;
CREATE_PUBVAR(VregNSeqNwA,     unsigned )=false;
CREATE_PUBVAR(VregNSeqIyIx,    float );
CREATE_PUBVAR(VregNSqIyxPu,    float );

CREATE_PUBVAR(QregInt,         float );

CREATE_PUBVAR(VregCmd ,        volatile float );       /* Volt regulator cmd (VAR reg output) */
CREATE_PUBVAR(QregIntZ1,       volatile float );
CREATE_PUBVAR(DelQregInt,      volatile float );
CREATE_PUBVAR(QregCarry,       volatile float );

CREATE_PUBVAR(IyHrmFFilPu,      float );             // Reactive filter current per unit

CREATE_PUBVAR(CTChkEnbDoDly, unsigned );
CREATE_PUBVAR(CTChkEnbDoDlyTmr, float );
CREATE_PUBVAR(TrqEnbStPuDly, unsigned );
CREATE_PUBVAR(TrqEnbStPuDlyTmr, float );

/////////////////////////////////
// Start VPRP Vars
/////////////////////////////////
CREATE_PUBVAR(VPRPAryCtRcp,     float );             // VPRP average array size reciprocal
CREATE_PUBVAR(VPRPFilGnDyn,     float );             // VPRP lp filter response filter gain
CREATE_PUBVAR(VPRPFilGnFst,     float );             // VPRP lp filter response filter gain fast
CREATE_PUBVAR(VPRPFilGnSlw,     float );             // VPRP lp filter response filter gain slow
CREATE_PUBVAR(VPRPIyCmAvg,      float );             // 60 second average for VPRP iy command, used in VPRP logic
CREATE_PUBVAR(VPRPIyCmAvg0,     float );             // Last sample average or VPRP iy command
CREATE_PUBVAR(VPRPIyCmAvg1,     float );             // 60 second array output for Iy command
CREATE_PUBVAR(VPRPIyCmdInt,     float );             // Integrator for VPRP Iy command sample averager
CREATE_PUBVAR(VPRPIyCmdZst,     float );             // VPRP Iy command array oldest member
CREATE_PUBVAR(VPRPIyCmSum,      float );             // VPRP Voltage feedback array sum
CREATE_PUBVAR(VPRPSmpThRcp,     float );             // Reciprocal for VPRP sample time
CREATE_PUBVAR(VPRPT3sPrSec,     float );             // VPRP T3's per second
CREATE_PUBVAR(VPRPVFbkAvg,      float );             // 60 second average for VPRP voltage feedback, used in VPRP logic
CREATE_PUBVAR(VPRPVFbkAvg0,     float );             // Last sample average or VPRP voltage feedback
CREATE_PUBVAR(VPRPVFbkAvg1,     float );             // 60 second array output for voltage feedback
CREATE_PUBVAR(VPRPVFbkInt,      float );             // Integrator for VPRP voltage feedback sample averager
CREATE_PUBVAR(VPRPVFbkSum,      float );             // VPRP reactive current command array sum
CREATE_PUBVAR(VPRPVFbkZst,      float );             // VPRP Voltage feedback array oldest member
CREATE_PUBVAR(VPRPWSlowTmr,     float );             // VPRP time for tracking time to switch to fast response
//CREATE_PUBVAR(VPRPActT3,        unsigned );          // VPRP is active--task3
CREATE_PUBVAR(VPRPArrayIdx,     unsigned );          // VPRP array index for 60 second averager
CREATE_PUBVAR(VPRPAryIdZs,      unsigned );          // VPRP oldest array member
CREATE_PUBVAR(VPRPSmpCtr,       unsigned );          // VPRP sample counter
CREATE_PUBVAR(VPRPWSlowAct,     unsigned ) =true;    //VPRP slow response time active
CREATE_PUBVAR(VPRPAryIdZst,     unsigned );          //VPRP oldest array member

float VPRPIyCmdAry[VPRP_AVG_ARRAY_SIZE];             //VPRP average iy command array
float VPRPVFbkAry[VPRP_AVG_ARRAY_SIZE] ;             //VPRP average voltage array


// Modindex limiting
CREATE_PUBVAR(R_ModIndDel,     float );              /* Rotor mod index delta for inegrator*/
CREATE_PUBVAR(R_ModIndErr,     float );              /* Modulation index error*/
CREATE_PUBVAR(R_ModIndex,      float );              /* modulaltion index */
CREATE_PUBVAR(R_ModIndFil,     float );              /* modulaltion index filtered */
CREATE_PUBVAR(R_ModIndFil2,    float );              /* modulaltion index filtered 2*/
CREATE_PUBVAR(R_ModIndLim,     float );              /* modulaltion index limit */
CREATE_PUBVAR(R_VmagPuT2,      float );              /* Rotor voltage magnitude pu*/
CREATE_PUBVAR(R_VMaxErr,       float );              /* Rotor vmax delta for integrator*/
CREATE_PUBVAR(R_VMaxIntDel,    float );              /* Rotor vmax delta for integrator*/
CREATE_PUBVAR(R_VRatRcp,       float );              /* Reciprocal of rotor rated voltage */
CREATE_PUBVAR(R_FwkRtLimDn,    float );              /* flux weakening downward rate limit */
CREATE_PUBVAR(R_FwkInt,        float );              /* flux weakening noise integrator */
CREATE_PUBVAR(R_FwkIntMax ,    float );              /* field weakening dynamice integrator maximum value*/
CREATE_PUBVAR(R_ModLimAct,     unsigned);            /* modulation index limiting active */
CREATE_PUBVAR(R_FwkDel,        float );              /* Rotor mod pre rate limited delta */
CREATE_PUBVAR(R_FwkDel2,       float );
CREATE_PUBVAR(R_ModIndDel2,    float );
CREATE_PUBVAR(FwkInh,          unsigned ) =false;

CREATE_PUBVAR(VregIyLimPk,     float);
CREATE_PUBVAR(LvIyRatPk,       float);
CREATE_PUBVAR(HvIyRatPk,       float);
CREATE_PUBVAR(VregMxMnDiffPk,  float);
CREATE_PUBVAR(VregLvSlopePk,   float);
CREATE_PUBVAR(VregHvSlopePk,   float);
CREATE_PUBVAR(IyMaxLimSSPk,    float);
CREATE_PUBVAR(VregLVIyMnDyPk,  float);
CREATE_PUBVAR(LvIyRatNWPk,     float);
CREATE_PUBVAR(VregIyLimNWPk,   float);
CREATE_PUBVAR(VrgIyLmNWAsmPk,  float);

CREATE_PUBVAR(VregTstCmd,       float);
CREATE_PUBVAR(HvSlopeMin,       float);
CREATE_PUBVAR(LvSlopeMax,       float);
CREATE_PUBVAR(HvSlopeMinNW,     float);
CREATE_PUBVAR(LvSlopeMaxNW,     float);
CREATE_PUBVAR(HvIyRatNWPk,      float);
CREATE_PUBVAR(VregLvSlpNWPk,    float);
CREATE_PUBVAR(VregHvSlpNWPk,    float);
CREATE_PUBVAR(VregMxMnDfNWPk,   float);
CREATE_PUBVAR(VregLVIyMnDyNWPk, float);
CREATE_PUBVAR(VrgIyLmAsmPk,     float);
CREATE_PUBVAR(VregFrzFilGn,     float);

CREATE_PUBVAR(R_USqrT2,        float);
CREATE_PUBVAR(PwrCmdPuT3,      float);

CREATE_PUBVAR(R_WeSlipRatRcp,  float);
CREATE_PUBVAR(R_WeSlipPu,      float);
CREATE_PUBVAR(R_WeSlipFilGn,   float);
CREATE_PUBVAR(VmagHiSlipFil,   float);
CREATE_PUBVAR(VmagHiSlip,      float);
CREATE_PUBVAR(VregMaxHiSlip,   float);
CREATE_PUBVAR(VmagHiSlipFilGn, float);

CREATE_PUBVAR(IyMaxLim1,        float );
//Low voltage kick variables
CREATE_PUBVAR(LvIyKickSt,   int) = IYBOOST_OFF;
CREATE_PUBVAR(LvIyKickPerm, unsigned) = false;
CREATE_PUBVAR(LvIyKickAct,  unsigned) = false;
CREATE_PUBVAR(LvIyKickInLm, unsigned) = false;
CREATE_PUBVAR(LvIyKick,     float) = 0.0;
CREATE_PUBVAR(LvIyKickVErr, float) = 0.0;
CREATE_PUBVAR(LvIyKickTmr,  float) = 0.0;
CREATE_PUBVAR(LvIyKPrmTmr,  float) = 0.0;

// Variables for rotor modulation index current limiting
CREATE_PUBVAR(S_ZBase,               float);
CREATE_PUBVAR(R_VlnBase,             float);
CREATE_PUBVAR(R_VlnBaseRcp,          float);
CREATE_PUBVAR(VrgIyMxRModFil,        float);
CREATE_PUBVAR(VrgIyMxSlipClmp,       float);
CREATE_PUBVAR(VrgIyMxRModErr,        float);
CREATE_PUBVAR(VrgIyMxRModLim,        float);
CREATE_PUBVAR(VrgIyMxRMod,           float);
CREATE_PUBVAR(VrgIyMxModFilGn,       float);
CREATE_PUBVAR(VrgIyMxRModLimUpGn,    float);
CREATE_PUBVAR(VrgIyMxSlipFilGn,      float);
CREATE_PUBVAR(VrgIyMxRModLimDnGn,    float);
CREATE_PUBVAR(VrgIyMxVrS,            float);
CREATE_PUBVAR(VrgIyMxSlipPu,         float);
CREATE_PUBVAR(VrgIyMxSlipFil,        float);


CREATE_PUBVAR(VregIyCompFl, float) = 0.0F;
CREATE_PUBVAR(VarCmdPf,     float) = 0.0F;
CREATE_PUBVAR(VARCmd        ,float  )  ;         /* VAR command                              */

// Data Passing
//--------------
DATA_PASS(R_USqr,        R_USqrT2,    T1A_T2, FILTERED );
DATA_PASS(PwrCmdPuT2,    PwrCmdPuT3,  T2_T3,  FILTERED );
DATA_PASS(IyPFCmdPu,     IyPFCmdPuT3, T2_T3,  FILTERED );

void VoltVarRegulator(void);
void VoltVarPrecondition(void);
void LowVoltageIyBoost(void);

/***************************************************************************
*
*                               Private Variables
*
*****************************************************************************/

/*=============================================================================
 * Function: ModulationIndexLimiting
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
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
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function controls modulation index limiting
 * loop error.
 *
 * Related Visos:
 *
 *===========================================================================*/

void ModulationIndexLimiting(void)
{
    float R_FwkDelInt;
    unsigned  FwkInh;
    float R_ModIndFlEr;
    float VmagHiSlipLd;
    float tempVregMaxHiSlip;


    R_ModIndex = sqrtf(R_USqrT2);
    R_ModIndFlEr = R_ModIndex - R_ModIndFil;
    if (R_ModIndFlEr > 0.0)
    {
       R_ModIndFil  = R_ModIndFil  + (R_ModLimFbkGn * R_ModIndFlEr);
    }
    else
    {
       R_ModIndFil  = R_ModIndFil  + (R_ModLimFbGnD * R_ModIndFlEr);
    }
    R_ModIndFlEr = R_ModIndex - R_ModIndFil2;
    if (R_ModIndFlEr > 0.0)
    {
       R_ModIndFil2  = R_ModIndFil2  + (R_ModLimFbGn2 * R_ModIndFlEr);
    }
    else
    {
       R_ModIndFil2  = R_ModIndFil2  + (R_ModLmFbGn2D * R_ModIndFlEr);
    }


    if ( R_BrgCroBarCtl.CroBarAct || GmNSeqHi || GmLineVLo)
    {
        R_FwkRtLimDn = 0.0;
        FwkInh       = true;
    }
    else
    {
        R_FwkRtLimDn = PARM(R_FwkRtLim);
        FwkInh       = false;
    }

    R_ModIndLim = PARM(R_ModIndMax);
    R_ModIndErr = R_ModIndLim - R_ModIndFil;
    R_ModIndDel = PARM(R_FwkIntGn)*R_ModIndErr;

    R_VmagPuT2 = R_VRatRcp * R_VMagT2;
    R_VMaxErr   = PARM(R_VMaxPu) - R_VmagPuT2;
    if ( R_SpdFbk > PARM(R_RPMSync))
    {
       R_VMaxIntDel= R_VMaxErr*PARM(R_VMaxIntGn);
    }
    else
    {
       R_VMaxIntDel= 1.0e6F;     // disable when sub-sync
    }                            // (set to large +ve value to avoid affecting R_ModIndDel below)

    VnVregMagDel = PARM(VnVregRedGn) * (PARM(VnVregRedThr) - GmVnMagPuMax);

    R_FwkDel    = MIN(MIN(R_ModIndDel, R_VMaxIntDel), VnVregMagDel);


    if ( R_FwkDel < R_FwkRtLimDn)
    {
         R_FwkDel = R_FwkRtLimDn;
    }

    R_FwkDelInt = R_FwkDel * DelTm2;

    R_FwkInt = R_FwkInt + R_FwkDelInt;
    R_FwkIntMax = MIN((PARM(R_FwkMargin) + VregCmd), PARM(VregRefMax));

    if ( R_FwkInt > R_FwkIntMax)
    {
        R_FwkInt    = R_FwkIntMax;
        R_ModLimAct = false;
    }
    if ( R_FwkInt < PARM(R_FwkMinLim))
    {
        R_FwkInt    = PARM(R_FwkMinLim);
        R_ModLimAct = true;
    }


    VregRfMaxDyn = R_FwkInt;

    //Faster mod index scheme

    R_ModIndDel2 = PARM(R_FwkIntGn2) * (PARM(R_ModIndMax2) - R_ModIndFil2);
    if ( FwkInh)
    {
        R_FwkDel2 = 0.0;
    }
    else//calculate and clamp to max of zero
    {
       R_FwkDel2 = MIN(0.0, R_ModIndDel2 );
    }


    ////////////////////////////////////////////////
    // VregMaxLim reduction for high slip conditions
    ////////////////////////////////////////////////

    // Pu slip
    R_WeSlipPu += R_WeSlipFilGn * (R_WElecT2 * R_WeSlipRatRcp - R_WeSlipPu);

    // Clamped grid V mag with (+)going lead
    VmagHiSlipFil += VmagHiSlipFilGn  * ( GmVMagPuT2 - VmagHiSlipFil );
    VmagHiSlipLd   = PARM(VmagHiSlipLdK) * MAX( (GmVMagPuT2 - VmagHiSlipFil - PARM(VmagHiSlipLdOfs)), 0.0);
    VmagHiSlip     = MAX(PARM(VmagHiSlipMin), GmVMagPuT2) + MIN(VmagHiSlipLd, PARM(VmagHiSlipLdLim));

    // Limit on Vreg is scaled delta between max pu rotor volts and product of pu slip * pu volts
    tempVregMaxHiSlip = PARM(VregMaxHiSlipGn) * (PARM(VregMaxHiSlipMx) - VmagHiSlip*fabs(R_WeSlipPu));

    if (R_Running && (VmagHiSlipLd > 0.0F) && !GmAsymRTIp && PARM(VregMaxHiSlipEnb) )
    {
       VregMaxHiSlip = MAX(tempVregMaxHiSlip, -PARM(HvIyRat));
    }
    else
    {
       VregMaxHiSlip = 5.0; //  disable by setting output high
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // VrgIyMxRMod calculations conditions for modulation index control through a calulcated
    // reactive current command
    //////////////////////////////////////////////////////////////////////////////////////////

    VrgIyMxRModFil += VrgIyMxModFilGn * (R_ModIndex - VrgIyMxRModFil);
    VrgIyMxRModErr = PARM(VrgIyMxModThr) -  VrgIyMxRModFil;


    //Calculate VrgIyMxRModLim
    if (VrgIyMxRModErr >= 0.0F)
    {
        VrgIyMxRModLim += (VrgIyMxRModLimUpGn * VrgIyMxRModErr);
    }
    else
    {
        VrgIyMxRModLim += (VrgIyMxRModLimDnGn * VrgIyMxRModErr);
    }
    if ( VrgIyMxRModLim > PARM(VrgIyMxRModLimMax) )
    {
        VrgIyMxRModLim = PARM(VrgIyMxRModLimMax);
    }
    else if ( VrgIyMxRModLim < PARM(VrgIyMxRModLimMin))
    {
        VrgIyMxRModLim = PARM(VrgIyMxRModLimMin);
    }


    //Find VrgIyMxVrS
    VrgIyMxSlipPu   = (R_SpdFbk - PARM(R_RPMSync) ) * R_RPMSyncRcp;
    VrgIyMxSlipFil  += VrgIyMxSlipFilGn * ( VrgIyMxSlipPu - VrgIyMxSlipFil);
    VrgIyMxSlipClmp = MAX(fabs(VrgIyMxSlipFil), PARM(VrgIyMxSlipMin));   //prevent divide by zero
    VrgIyMxVrS = (L_VdcFbkT2 * 0.5F * R_VlnBaseRcp) / VrgIyMxSlipClmp;

    //Give the final output
    if ( PARM(VrgIyMxRModEnb) )
    {
        VrgIyMxRMod = ( VrgIyMxRModLim * VrgIyMxVrS * PARM(VrgIyMxA1) * PARM(VrgIyMxA2) ) - ( GmVMagPuT2 *  PARM(VrgIyMxA3) );
    }
    else
    {
        VrgIyMxRMod = 5.0F;
    }


}


/*=============================================================================
 * Function: VPRPSupportT2
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
 *
 *
 *
 * Global Outputs: VPRPIyRef
 *
 * Locals:
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function implements the requirements to meet the German
 * grid code, VPRP
 *
 *
 * Related Visos: ALG_425
 *
 *===========================================================================*/
void GDIyLProportional(void)
{
    unsigned  VPRPVsecLim1=false;
    unsigned  VPRPVsecLim2=false;
    unsigned  VPRPVsecZero1=false;
    unsigned  VPRPVsecZero2=false;
    float VPRPIyMaxAdj = 0.0;
    float VPRPIyMinAdj = 0.0;


    if ( VPRPWSlowAct)
    {
       VPRPVErr    =  VPRPVFbkAvg - VregVpuLimIn;
    }
    else
    {
       VPRPVErr    =  0.0;
    }
    VPRPVErrLv  =  VPRPVErr - PARM(VPRPDBandLv);
    VPRPVErrHv  =  VPRPVErr + PARM(VPRPDBandHv);
    VPRPIyDelPu =  (PARM(VPRPPrpGnLv) * MAX(VPRPVErrLv, 0.0)) + (PARM(VPRPPrpGnHv) * MIN(VPRPVErrHv, 0.0));

    //Low voltage integrator
    VPRPLVsecErr += VPRPVErrLv * DelTm2;
    if ( PARM(VPRPEnb) && (VPRPLVsecErr > PARM(VPRPNoise) ) )
    {
        VPRPLVsecErr = PARM(VPRPNoise);
        VPRPActLv    = true;
    }
    else if ( VPRPLVsecErr < 0.0)
    {
        VPRPLVsecErr = 0.0;
        VPRPActLv    = false;
    }


    //High voltage integrator
    VPRPHVsecErr += VPRPVErrHv * DelTm2;
    if ( VPRPHVsecErr < -PARM(VPRPNoise))
    {
        VPRPHVsecErr = -PARM(VPRPNoise);
        VPRPVsecLim1 = true;
    }
    else if ( VPRPHVsecErr > 0.0)
    {
        VPRPHVsecErr = 0.0;
        VPRPVsecZero1 = true;
    }

    //High voltage integrator for defeat of VPRP
    VPRPHVsecEr2 += (PARM(VPRPHvDsbThr) - VregVpuLimIn) * DelTm2;
    if ( VPRPHVsecEr2 < -PARM(VPRPNoise2))
    {
        VPRPHVsecEr2 = -PARM(VPRPNoise2);
        VPRPVsecLim2 = true;
       // VPRPHVsecErr = 0.0;
    }
    else if ( VPRPHVsecEr2 > 0.0)
    {
        VPRPHVsecEr2 = 0.0;
        VPRPVsecZero2 = true;
    }

    if ( PARM(VPRPEnb) && VPRPVsecLim1 && VPRPVsecZero2)
    {
        VPRPActHv    = true;
    }
    else if (VPRPVsecZero1 || VPRPVsecLim2)
    {
        VPRPActHv    = false;
    }

    if ( VPRPActLv || VPRPActHv)
    {
        VPRPAct   = true;
        VPRPIyRef = VPRPIyCmAvg + VPRPIyDelPu;
    }
    else
    {
        VPRPAct   = false;
        VPRPIyRef = 0.0;
    }

    if ( VPRPActHv )
    {
       VPRPIyMaxAdj = PARM(VPRPIyTolMin);
       VPRPIyMinAdj = PARM(VPRPIyTolMax);
    }
    else
    {
       VPRPIyMaxAdj = PARM(VPRPIyTolMax);
       VPRPIyMinAdj = PARM(VPRPIyTolMin);
    }


    VPRPIyMax =  MIN( (VPRPIyRef + VPRPIyMaxAdj),  LvIyMaxDyn);
    VPRPIyMin =  MAX( (VPRPIyRef - VPRPIyMinAdj), -HvIyMaxDyn);



}


/*=============================================================================
 * Function: GridDependentReactiveCurrentLimiter (GDIyL)
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
 *
 *
 *
 * Global Outputs: VPRPIyRef
 *
 * Locals:
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function implements the conventional GDIyL
 * grid code, VPRP
 *
 *
 * Related Visos:
 *
 *===========================================================================*/
void GDIyLConventional(void)
{

    //float  Temp2;
    float  VregMaxLimTemp;
    float  VregMinLimTemp;


       /////////////////////////////////////////////////////////////
       //  Low and High Voltage Ride-Through Detection -- TRQ_420
       /////////////////////////////////////////////////////////////

    VregFilIn    = GmVMagPuT2;

    if ( L_Running)
    {

       ///////////////////////////////////////////////////////////////
       // Dynamic reactive current clamp for voltage regulator
       ///////////////////////////////////////////////////////////////

       VregVpuFil += PARM(VregVpuFilW) * DelTm2  * ( VregFilIn - VregVpuFil );
       VregVpuFlErr = MAX( (VregFilIn - VregVpuFil - PARM(VregDVFilOff)), 0.0);
       VregVpuLimIn = VregVpuFil + (PARM(VregDVGn) * VregVpuFlErr);

       VregMaxLimTemp =  VregIyLimDynT2;
       VregMinLimTemp = -VregIyLimDynT2;

       if ( VregVpuLimIn < PARM(VregMxHvSlp) )
       {
           //LVRT upper curve (max)
           if ( VregVpuLimIn < PARM(VregMaxLv1))
           {
               VregMaxLimTemp = VregIyLimDynT2 + VregLvMaxSlpT2 *(VregVpuLimIn - PARM(VregMaxLv1));
               VregMaxLimTemp = MIN(VregMaxLimTemp, LvIyMaxDynT2);
           }

           //LVRT lower curve (min)
           if ( VregVpuLimIn < PARM(VregMnLvSlp))
           {
               VregMinLimTemp = -VregIyLimDynT2 + VregLvMinSlpT2 *(VregVpuLimIn - PARM(VregMnLvSlp));
               VregMinLimTemp = MIN(VregMinLimTemp, (VregMaxLimTemp - VregMxMnDynT2));
               VregMinLimTemp = MIN(VregMinLimTemp, VregLVIyMnDyT2);
           }
           else if( VregVpuLimIn > PARM(VregMinHv1))
           {
               VregMinLimTemp = -VregIyLimDynT2 + VregHvMinSlpT2 *(VregVpuLimIn - PARM(VregMinHv1));
               VregMinLimTemp = MAX(VregMinLimTemp, -HvIyMaxDynT2);
           }

     }
     else //VregVpuLimIn >= PARM(VregMxHvSlp)
     {
         //HVRT lower curve (min)
         if ( VregVpuLimIn > PARM(VregMinHv1))
         {
             VregMinLimTemp = -VregIyLimDynT2 + VregHvMinSlpT2 *(VregVpuLimIn - PARM(VregMinHv1));
             VregMinLimTemp = MAX(VregMinLimTemp, -HvIyMaxDynT2);
         }

         //HVRT upper curve (max)
         if ( VregVpuLimIn > PARM(VregMxHvSlp))
         {
             VregMaxLimTemp = VregIyLimDynT2 + VregHvMaxSlpT2 *(VregVpuLimIn - PARM(VregMxHvSlp));
             VregMaxLimTemp = MAX(VregMaxLimTemp, (VregMinLimTemp + VregMxMnDynT2));
         }


     }

     VregMaxLim0 = VregMaxLimTemp;
     VregMinLim0 = VregMinLimTemp;

    ///////////////////////////////////////////////////////////////
    // End Dynamic reactive current clamp for voltage regulator
    ///////////////////////////////////////////////////////////////


   }
   else
   {
     ;

   }


}


/*=============================================================================
 * Function: RotorIxCompensationAsymmetricT2
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
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
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function manipulates reactive current to help reduce the rotor real
 * current regulator error during asymmetric faults
 *
 *
 * Related Visos: ALG_425
 *
 *===========================================================================*/
void RotorModIndexCompensationAsymmetricT2(void)
{
    float  IyAsyIxLimDel;

    IyAsyIxErr     =  PARM(IyAsyIxRef) - R_IxregErrT2;

    if ( GmAsymRTIp && VregNSeqSpdA)
    {
       //Freeze integrator if crowbar active
       if ( R_BrgCroBarCtl.CroBarAct)
       {
          IyAsyIxLimDel = 0.0;
       }
       else
       {
          IyAsyIxLimDel = IyAsyIxErr*PARM(IyAsyIxW)*DelTm2;
       }

       //Run integrator
       VregNSeqIyIx = VregNSeqIyIx + IyAsyIxLimDel;


       //Clamp integrator
       if ( VregNSeqIyIx > VrgIyLmAsmPk)
       {
           VregNSeqIyIx = VrgIyLmAsmPk;
       }
       if ( VregNSeqIyIx < PARM(IyAsyIxMin))
       {
           VregNSeqIyIx = PARM(IyAsyIxMin);
       }
    }
    else
    {
       VregNSeqIyIx = MIN(VregOutAPk, VrgIyLmAsmPk);
    }

    VregNSqIyxPu = VregNSeqIyIx * INomGrdPkRcp;
}


/*=============================================================================
 * Function: GridTransientIySupport
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
 *
 *
 *
 * Global Outputs: PllW
 *
 * Locals:
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function controls power factor
 * loop error.
 *
 * Related Visos:
 *
 *===========================================================================*/
void GridTransientIySupport(void)
{

    float IxRatLimTemp;

    ModulationIndexLimiting();
    GDIyLConventional();
    GDIyLProportional();

    ////////////////////////////////////////////
    //VPRP Selection
    ////////////////////////////////////////////
    if ( VPRPAct)
    {
       VregMaxSel0 = VPRPIyMax;
       VregMinSel0 = VPRPIyMin;
    }
    else
    {
       VregMaxSel0 = VregMaxLim0;
       VregMinSel0 = VregMinLim0;
    }

    ////////////////////////////////////////////
    //Select the negative sequence value
    ////////////////////////////////////////////
    RotorModIndexCompensationAsymmetricT2();

    if(VregNSeqSpdA)                          // Doubly-fed iy reduction to control modulation
    {
       VregIyNSeq = VregNSqIyxPu;
    }
    else if (VregNSeqNwA)                     // No wind mode  during neg seq
    {
       VregIyNSeq = PARM(VregNSeqIyNw);
    }
    else                                     // gen connected during neg seq
    {
       VregIyNSeq = PARM(VregNSeqIyLm);
    }
    ////////////////////////////////////////////
    // Select the final output for upper limit
    ////////////////////////////////////////////
    if (IslandDet)
    {
       VregMaxSel1 = PARM(VregIyIsldMx);
    }
    else if (GmAsymRTIp)
    {
       VregMaxSel1 = MIN(VregMaxSel0, VregIyNSeq);
    }
    else
    {
       VregMaxSel1 = VregMaxSel0;
    }
    VregMaxLim  = MIN(MAX(MIN(VregMaxSel1, IyCnvLimMx),IyCnvLimMn),FastDILimPu);
    // Apply high slip limit if rotor running and +ve going grid disturbance
    VregMaxLim = MIN(VregMaxHiSlip, VregMaxLim);
    VregMaxLim = MIN(VrgIyMxRMod, VregMaxLim);

    ////////////////////////////////////////////
    // Select the final output for lower limit
    ////////////////////////////////////////////

    if (IslandDet)
    {
       VregMinSel1 = PARM(VregIyIsldMn);
       VregMinLimNom = MAX(MIN(VregMinSel1, IyCnvLimMx),IyCnvLimMn);
       VregMinLim    = VregMinLimNom;
       R_FwkAct      = false;
    }
    else
    {
       VregMinSel1 = MIN(VregMinSel0, (VregMaxLim - VregMxMnDynT2) );
       VregMinLimNom = MAX(MIN(VregMinSel1, IyCnvLimMx),IyCnvLimMn);
       // If low grid volts, always use nominal Iy limit (as it may be used to force positive Iy)
       if (VregVpuLimIn < PARM(VregMaxLv1))
       {
           VregMinLim = VregMinLimNom;
           R_FwkAct = false;
       }
       else if (R_ModIndFil > PARM(R_ModIndMax))
       {
           // If rotor overmodulating, quickly allow larger negative Iy
           VregMinLim = PARM(VregMinLimFwk);
           R_FwkAct = true;
       }
       else
       {
           if (R_FwkAct)
           {
               // If rotor not overmodulating, slowly move back towards nominal Iy limit
               VregMinLim += PARM(VregMinLimFwkW) * DelTm2 * (VregMinLimNom - VregMinLim);

               if (VregMinLim > 1.01 * VregMinLimNom)  R_FwkAct = false;
           }
           else
           {
               VregMinLim = VregMinLimNom;
           }
       }
    }

    /////////////////////////////////////////////////////////
    // Filter and rate limit pu for calculation of Iy Limit
    /////////////////////////////////////////////////////////
    IxRatLimTemp = PARM(LvIxW) * ( fabs(S_IxFbkT2) - LvIxRefFil );
    if (IxRatLimTemp >  LvIxRLUp)
    {
       IxRatLimTemp =  LvIxRLUp;
    }
    else if (IxRatLimTemp < -LvIxRLDn)
    {
       IxRatLimTemp = -LvIxRLDn;
    }

    LvIxRefFil +=  DelTm2 * IxRatLimTemp;


}


/*=============================================================================
 * Function: ReactiveCurrentControlT2
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  DelTm2
 *                 AcLineMag
     *
 *
 *
 * Global Outputs: PllW
 *
 * Locals:         PhImbInlim
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function controls power factor
 * loop error.
 *
 * Related Visos:
 *
 *===========================================================================*/

void ReactiveCurrentControlT2(void)
{
    float    QregCmdTemp;
    float    IyCmdTemp;

    VARCmd = 1000.0 * gVariable.ReadAsFloat(PARM(VARRefAdr));

    // Calculate VAR command needed to balance Ix for desired pwr factor

    //Diagnostic only


    VregFrzVFil = VregFrzVFil +  (VregFrzFilGn*(GmVMagPuT2 - VregFrzVFil)) ;


    /* select / calc QregCmd */
    if (VARModeCmd)
    {
        QregCmdTemp = VARCmd;

    }
    else
    {
        QregCmdTemp = VarCmdPf;
    }



    if ( QregCmdTemp > QregMaxDyn)
    {
         QregCmdTemp = QregMaxDyn;
    }
    else if ( QregCmdTemp <  QregMinDyn)
    {
         QregCmdTemp = QregMinDyn;
    }

    QregCmd = QregCmdTemp + PARM(QregTstCmd);
    QregCmdPu = QregCmd * TurbNpRatRcp;


    if ( R_Running)
    {
        QregIntGnDyn = PARM(QregIntGn)*DelTm2;
        if(IslandDet)
        {
            VregIntGnDyn = PARM(VregIntGnIs)*DelTm2;
            VregPrpGnDyn = PARM(VregPrpGnIs);
        }
        else
        {
            VregIntGnDyn = PARM(VregIntGn)*DelTm2;
            VregPrpGnDyn = PARM(VregPrpGn);
        }
        VregNSeqNwA  = false;
    }
    else
    {
        QregIntGnDyn = PARM(QregIntGn0)*DelTm2;
        VregIntGnDyn = PARM(VregIntGn0)*DelTm2;
        VregPrpGnDyn = PARM(VregPrpGn0);
        VregNSeqNwA  = true;
    }
    // Delay enabling Volt-Var to allow precondition filter to settle after start-up transients
    if (R_SeqStat.R_TrqEnbStat)
    {
        TrqEnbStPuDlyTmr += DelTm2;
        if (TrqEnbStPuDlyTmr > PARM(TrqEnbStPuDlyTm))
        {
            TrqEnbStPuDly = true;
        }
    }
    else
    {
        TrqEnbStPuDlyTmr = 0.0F;
        TrqEnbStPuDly = false;
    }


    if (R_CTChkEnb)
    {
        CTChkEnbDoDlyTmr = 0.0F;
        CTChkEnbDoDly = true;
    }
    else
    {
        CTChkEnbDoDlyTmr += DelTm2;
        if (CTChkEnbDoDlyTmr > PARM(CTChkEnbDoDlyTm))
        {
            CTChkEnbDoDly = false;
        }
    }
    if ( ((TrqEnbStPuDly && !S_VMaxIyFast) || RPNWRunning) && !CTChkEnbDoDly && !PARM(IyCmdOvr) && !L_LTestIp)
    {
        VoltVarRegulator();
    }
    else
    {
        VoltVarPrecondition();
    }

    LowVoltageIyBoost();



    //////////////////////////////////////////
    //  Manage the output
    //////////////////////////////////////////

    VregIyCmd0    =  VregOut - VregIyCompFl;

    //// Code from here up to implement normal power factor control
    if (PARM(IyCmdOvr))
    {
          if (PARM(TestMode) == PUMP_BACK)
          {
              VregIyCmd = PARM(IyCmdForce);
          }
          else
          {
              VregIyCmd = PARM(IyCmdForce) + IyHrmFFilPu;
          }
    }
    else
    {
          IyCmdTemp   = VregIyCmd0 + IyHrmFFilPu;

          if (R_FwkAct)
          {
              // bypass the LvIyReservN limit if mod index limiting the rotor
              VregIyCmd   = MIN(IyCmdTemp, LvIyReservP);
          }
          else
          {
              VregIyCmd   = MAX(MIN(IyCmdTemp, LvIyReservP), LvIyReservN);
          }
    }


    VregIyStep = READ_PARMPTR(VregIyStpAdr);
    IyPFCmdPu  = ISysCmdSF * (VregIyCmd + LvIyKick + VregIyStep);
    IyPFCmd    = IyPFCmdPu * INomGridPk;



}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function calculates VARCmdPf, which is used in the local power
// factor test mode only
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ReactiveCurrentControlT3 (void)
{
    // filter grid power for power-factor control thru volt-var regulator
    PwrGridVCFil += PwrGVARCmdGn * ( GmPwrGridT3 - PwrGridVCFil );
    VarCmdPf      = PwrCmdLineT3 * VarPfScale;
}

/*=============================================================================
 * Function: LowVoltageIyBoost
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
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
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function gives a transient boost the the reactive current command when
 * the a fast rate of rise is needed and GridStrength is 'strong'
 *
 * Related Visos:VVAR_500, VVAR_505
 *
 *===========================================================================*/

void LowVoltageIyBoost(void)
{
    unsigned LvIyKickInLmTemp;
    unsigned LvIyKickPulseDone;

    // Find the error
    LvIyKickVErr = PARM(LvIyKickVThr) - GmVMagPuT2 - (PARM(LvIyKickDvGn) * VregVpuFlErr);
    LvIyKickPulseDone = false;

    switch (LvIyKickSt)
    {
        case IYBOOST_OFF:    // LvIyKickSt = 0

           LvIyKickAct = false;
           LvIyKickTmr = 0.0;

           LvIyKPrmTmr = LvIyKPrmTmr + DelTm2;
           if ( LvIyKPrmTmr >= PARM(LvIyKPrmTm) )
           {
              LvIyKPrmTmr  = PARM(LvIyKPrmTm);
              if ( !RPNWRunning)
              {
                 LvIyKickPerm = true;
                 if ( (R_SpdFbkAvg < PARM(LvIyKSpdThr)) && (LvIyKickVErr > 0.0) )
                 {
                    LvIyKickSt  = IYBOOST_ON;
                    LvIyKickAct = true;
                 }
              }
           }



        break;



        case IYBOOST_ON:    // LvIyKickSt = 1

           LvIyKickAct  = true;

           LvIyKickTmr = LvIyKickTmr + DelTm2;
           if ( LvIyKickTmr > PARM(LvIyKPulseTm) )
           {
              LvIyKickPulseDone = TRUE;
              LvIyKPrmTmr = 0.0;

           }

           if ( LvIyKickInLm || LvIyKickPulseDone)
           {
              LvIyKickSt  = IYBOOST_OFF;
              LvIyKickAct = false;
              LvIyKickTmr = 0.0;
           }

           LvIyKPrmTmr = LvIyKPrmTmr - (PARM(LvIyKickRstK) * DelTm2); //Count down to allow for another pulse if time not expired
           if (LvIyKPrmTmr <= 0.0)
           {
            LvIyKPrmTmr  = 0.0;
           }


           LvIyKickPerm = false;



        break;

       default:
        break;
    }//End state machine


    LvIyKickInLmTemp = false;
    if (LvIyKickAct && !GmNSeqHi)
    {
       LvIyKick = LvIyKick + (DelTm2 * ( LvIyKickVErr * PARM(LvIyKickGn)) );
       if (LvIyKick >= PARM(LvIyKickMax))
       {
          LvIyKick     = PARM(LvIyKickMax);
       }
    }
    else
    {
       LvIyKick = LvIyKick + (DelTm2 * PARM(LvIyKickDcy));
    }

    if (LvIyKick <= 0.0F)
    {
       LvIyKick = 0.0F;
       LvIyKickInLmTemp = true;
    }


    LvIyKickInLm = LvIyKickInLmTemp;


}


/*=============================================================================
 * Function: VARRegulator
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
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
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function regulator VARs
 * loop error.
 *
 * Related Visos:
 *
 *===========================================================================*/

void VARRegulator(void)
{

    /* Feed-Forward Model */
    QregFFLpFil = QregFFLpFil + QregFFWDt * (QregCmdPu - QregFFLpFil);
    QregFFHpFil = QregCmdPu - QregFFLpFil;
    QregFF      = QregFFGnDt * QregFFHpFil;
    /* Output reactive current command */
    QregFbk = GmQPwrT2;
    QregFbkPu = QregFbk * TurbNpRatRcp;
    QregErr   = QregCmdPu - QregFbkPu;

    // anti-windup on outer loop when inner loop is in limit
    if ( (VregFrzVFil > PARM(VregFrzHThrs)) || (VregFrzVFil < PARM(VregFrzLThrs)))
    {
        VregFrzInt = true;
    }
    else
    {
        VregFrzInt = false;
    }

    if ( (VregLimP && (QregErr > 0.0F) ) || (VregLimN && (QregErr < 0.0F) )  || VregFrzInt   || VPRPAct  )
    {
        QregDelInt = 0.0;
        QregFrozen = true;
    }
    else
    {
        QregDelInt = (QregIntGnDyn * QregErr + QregFF) + QregCarry;
        QregFrozen = false;
    }

    QregIntZ1  = QregInt;                  // save old integrator value
    QregInt    = QregInt + QregDelInt;     // calculate new integrator value
    DelQregInt = QregInt - QregIntZ1;      // calculate how much integrator acutally changed
    QregCarry  = QregDelInt - DelQregInt;  // carry over amount that didn't get added in
                                           // due to floating point limitations

    if ( QregInt > VregRfMaxDyn)
    {
       QregInt = VregRfMaxDyn;
    }
    else if (QregInt < PARM(VregRefMin))
    {

       QregInt = PARM(VregRefMin);
    }


    if ( IslandDet)
    {
        VregCmd = PARM(VregRefIsld);
    }
    else
    {
        VregCmd = QregInt + VBstPVpu;
    }


    if ( (VregMaxLim - VregOut) > PARM(VregLimHyst))
    {
        VregLimP = false;
    }
    if ( (VregOut - VregMinLim) >  PARM(VregLimHyst))
    {
        VregLimN = false;
    }

}

/*=============================================================================
 * Function: VoltageRegulator
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
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
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function regulates AC voltage
 *
 * Related Visos:
 *
 *===========================================================================*/

void VoltageRegulator(void)
{

    float VregPrp1Z1;

    VregFbk     = GmVMagPuT2;
    VregCorrErr = VregInt - VregOut;
    VregErr     = VregCmd - VregFbk + PARM(VregTstCmd);

    if (S_K1Status)
    {
        VregErr = VregErr + R_FwkDel2;
    }

    VregPrp1Z1  = VregPrp1; //save for later

    VregPrpFil  += VregPrpFilGn * ( VregErr - VregPrpFil );
    if ( VregPrpFil > PARM(VregPrpMax))
    {
        VregPrpFil = PARM(VregPrpMax);
    }
    else if ( VregPrpFil < PARM(VregPrpMin))
    {
        VregPrpFil = PARM(VregPrpMin);
    }

    VregPrp1     = VregPrpFil * VregPrpGnDyn;
    VregPrpDel   = VregPrp1 - VregPrp1Z1;

    VregInt     += (VregIntGnDyn * VregErr) + VregPrpDel;
    if (VregInt >=  VregMaxLim)
    {
        VregInt  =  VregMaxLim;
        VregLimP = true;
    }
    else if (VregInt <= VregMinLim)
    {
        VregInt  = VregMinLim;
        VregLimN = true;
    }

    VregHvKick  = PARM(VregHvKickGn) * VregVpuFlErr;

    //VregOut Correction
    VregCorrInt = VregCorrInt + (VregCorrGn * VregCorrErr);

    VregOut = VregInt - VregHvKick + VregCorrInt;

    //Clamp total output
    VregOutMax = VregMaxLim + PARM(VregOutBias);
    VregOutMin = -HvIyMaxDyn;
    if ( VregOut > VregOutMax)
    {
       VregOut = VregOutMax;
    }
    else if (VregOut < VregOutMin)
    {
       VregOut = VregOutMin;
    }
    VregOutAPk = VregOut * INomGridPk;

    // If overmodulating, set the limit to wherever the regulator needed to go
    // (so that the return to nominal doesn't begin with VregMinLim at P.VregMinLimFwk)
    if (R_FwkAct)
    {
        VregMinLim = MIN(VregOut, VregMinLimNom);
    }

}

/*=============================================================================
 * Function: VoltageBoostVsPowerRise
 * Execution Level: Task 3
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
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
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function boosts the ac voltage reference upon certain conditions
 * of rising power.
 *
 * Related Visos:  VVAR_140
 *
 *===========================================================================*/
void VoltageBoostVsPowerRise(void)
{
    float VBstPVUpDnGn;
    float VBstPVUpDnErr;
    float VBstPVpuTemp;

    //Slow up/Fast down low pass filter

    //////////////////////////////////////////////////////////////////////////////
    // Filter and rate limit pu for calculation of Iy Limit -- T3 portion -End
    //////////////////////////////////////////////////////////////////////////////

    VBstVpuRaw    = VBstPGnDtRcp * (PwrCmdPuT3 - PwrCmdPuZ1);
    VBstVpuUc     = VBstVpuRaw - PARM(VBstPOffs);
    PwrCmdPuZ1    = PwrCmdPuT3;


    VBstPVpuLim   = PARM(VBstPLimGn) * (PARM(VBstPVMaxPu) - GmVMagPuT3);
    VBstPVUpDnErr = VBstPVpuLim - VBstPVLimFil;
    if ( VBstPVUpDnErr >= 0.0)
    {
        VBstPVUpDnGn = VBstPLmUpWdt;
    }
    else
    {
        VBstPVUpDnGn = VBstPLmDnWdt;
    }
    VBstPVLimFil = VBstPVLimFil + VBstPVUpDnGn * VBstPVUpDnErr;
    if ( VBstPVLimFil < 0.0)
    {
         VBstPVLimFil = 0.0;
    }

    //Main path out with lower clamp at zero
    VBstPVpuTemp  = VBstVpuUc;
    if ( VBstPVpuTemp > VBstPVLimFil)
    {
       VBstPVpuTemp = VBstPVLimFil;
    }
    else if ( VBstPVpuTemp < 0.0)
    {
       VBstPVpuTemp = 0.0;
    }

    VBstPVpu = VBstPVpuTemp;
}

/*=============================================================================
 * Function: VoltVarRegulator
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
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
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function controls power factor
 * loop error.
 *
 * Related Visos:
 *
 *===========================================================================*/

void VoltVarRegulator(void)
{

    VARRegulator();
    VoltageRegulator();

}



/*=============================================================================
 * Function: VoltVarPrecondition
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
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
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function pre-conditions the volt-var regulator.  It is located in a
 * 'slow' file to save code-space IRAM.
 *
 *
 * Related Visos:
 *
 *===========================================================================*/

void VoltVarPrecondition(void)
{
     float VltVarBypass;

     /* Precondition Volt-VAR variables when line not running */
     if ( PARM(IyCmdOvr) || L_LTestIp)
     {

         if (PARM(IyCmdOvr))
         {
             VltVarBypass = SQRT2*PARM(IyCmdForce);
         }
         else// (L_LTestIp)
         {
             VltVarBypass = SQRT2*L_IyTstCmd;
         }

         if ( VltVarBypass > VregMaxLim)
         {
             VregInt = VregOut = VregMaxLim;
         }
         else if (VltVarBypass < VregMinLim)
         {
             VregInt = VregOut = VregMinLim;

         }
         else
         {
             VregInt = VregOut = VltVarBypass;
         }
     }
     else
     {
         if (!S_VMaxHiEnb)
         {
            VregOut = VregInt = (S_VMaxIyCmd * INomGrdPkRcp);
         }
         else
         {
            VregOut     = 0.0;
            VregInt     = 0.0;
         }
     }

     VregFbk  = GmVMagPuT2;
     QregInt += VregPrcFilGn * ( VregFbk - QregInt );   // put min filtering on V fbk for precondition
     VregCmd  = QregInt;
     QregCarry   = 0.0;
     VregRfMaxDyn  = R_FwkInt = PARM(VregRefMax);
     R_ModLimAct = false;


}




void VARVoltRegT3(void)
{
    float  S_LvIyResSqr;


    //////////////////////////////////////////////////////////////////////////////
    // Filter and rate limit pu for calculation of Iy Limit -- T3 portion -Start
    //////////////////////////////////////////////////////////////////////////////

    S_LvIyResSqr =  (S_IMagLim * S_IMagLim) - (LvIxRefFilT3 * LvIxRefFilT3);
    if ( (S_LvIyResSqr < 0.0 ) || !R_SeqStat.R_TrqEnbStat)
    {
       S_LvIyResSqr = 0.0;
    }
    S_LvIyReserv = sqrt(S_LvIyResSqr);

    IyHrmFFilPu  =  INomGrdPkRcp * L_IyHrmFFil;

    LvIyReserv   =  INomGrdPkRcp * (S_LvIyReserv + L_IReserveSS);
    LvIyReservP  =  LvIyReserv;
    LvIyReservN  = -LvIyReserv;
    IyCnvLimMx   =  LvIyReserv - IyHrmFFilPu + VregIyCompFl;
    IyCnvLimMn   = -LvIyReserv - IyHrmFFilPu + VregIyCompFl;


    VoltageBoostVsPowerRise();

}


/*=============================================================================
 * Function: VPRPSupportT3
 * Execution Level: Task 3
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
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function suupports the German Grid code, VPRP
 *
 * Related Visos: ALG_425 (task3 portion)
 *
 *===========================================================================*/
void GDIyLProportionalT3(void)
{
    int     i;
    //float   VPRPMultInt;
    //float   VPRPArySumDv;
    //float   VPRPArySumNm;
    //float   VPRPSmpCtrRcp;

    ///////////////////////////////
    // Filters for precondition
    ///////////////////////////////
    VPRPVFbkFil  += VPRPFilGnDyn * (VregFbkT3 - VPRPVFbkFil);
    VPRPIyCmdFil += VPRPFilGnDyn * (VregOutT3 - VPRPIyCmdFil);


    ///////////////////////////////
    // Decide mode of operation
    ///////////////////////////////

    if ( L_Running )
    {
        VPRPWSlowTmr += DelTm3;
        if ( VPRPWSlowTmr >= PARM(VPRPWSlowTm))
        {
            VPRPWSlowTmr = PARM(VPRPWSlowTm);
            VPRPWSlowAct = true;
        }
    }
    else //converter not ready to accept vars
    {
        VPRPWSlowTmr = 0.0F;
        VPRPWSlowAct = false;
    }

    //////////////////////////////
    // Take averages from one pass
    // to passes at 5 second
    //////////////////////////////

    VPRPVFbkInt  += VregFbkT3;

    VPRPIyCmdInt += VregOutT3;                   // current in quadrature to secondary voltage

    VPRPSmpCtr ++;

    if ( VPRPSmpCtr >= PARM(VPRPSmpCntTh))
    {

        VPRPVFbkAvg0 = VPRPVFbkInt  * VPRPSmpThRcp;
        VPRPIyCmAvg0 = VPRPIyCmdInt * VPRPSmpThRcp;

        VPRPSmpCtr   = 0;
        VPRPVFbkInt  = 0.0F;
        VPRPIyCmdInt = 0.0F;
    }




    if ( !VPRPWSlowAct)
        //Initalize array members to filter values until converter running and vars values settled
    {
        // Precondition the array with filtered values and set output equal to it
        for (i = 0; i < VPRP_AVG_ARRAY_SIZE; i++)
        {
            VPRPVFbkAry [i] = VPRPVFbkFil;
            VPRPIyCmdAry[i] = VPRPIyCmdFil;
        }

        VPRPVFbkAvg1 = VPRPVFbkFil;
        VPRPIyCmAvg1 = VPRPIyCmdFil;
        VPRPVFbkSum  = VPRPVFbkFil  * VPRP_AVG_ARRAY_SIZE;
        VPRPIyCmSum  = VPRPIyCmdFil * VPRP_AVG_ARRAY_SIZE;
        VPRPArrayIdx= 0;

    }
    else
    {
        ///////////////////////////////
        //Run VPRP 60 second averager
        ///////////////////////////////
        if ( VPRPSmpCtr == 0)
        {
            VPRPArrayIdx ++;
            if (VPRPArrayIdx >= VPRP_AVG_ARRAY_SIZE )
            {
                VPRPArrayIdx = 0;
            }

            VPRPAryIdZst =  VPRPArrayIdx + 1;
            if (VPRPAryIdZst >= VPRP_AVG_ARRAY_SIZE )
            {
                VPRPAryIdZst = 0;
            }

            VPRPVFbkSum = 0.0;
            VPRPIyCmSum = 0.0;
            VPRPVFbkAry [VPRPArrayIdx] = VPRPVFbkAvg0;
            VPRPIyCmdAry[VPRPArrayIdx] = VPRPIyCmAvg0;
            for (i = 0; i < VPRP_AVG_ARRAY_SIZE; i++)
            {
                VPRPVFbkSum  += VPRPVFbkAry[i];
                VPRPIyCmSum  += VPRPIyCmdAry[i];
            }

            VPRPVFbkZst  = VPRPVFbkAry[VPRPAryIdZst];   // Grab value of oldest voltage array member
            VPRPIyCmdZst = VPRPIyCmdAry[VPRPAryIdZst];  // Grab value of oldest iy array member
        }



        VPRPVFbkAvg1  = ((PARM(VPRPSmpCntTh) * VPRPVFbkSum) + VPRPVFbkInt -
                         (VPRPVFbkZst * VPRPSmpCtr) ) * VPRPAryCtRcp;

        VPRPIyCmAvg1  = ((PARM(VPRPSmpCntTh) * VPRPIyCmSum) + VPRPIyCmdInt -
                         (VPRPIyCmdZst * VPRPSmpCtr) ) * VPRPAryCtRcp;

    }

    ///////////////////////////////
    // Pick the output
    ///////////////////////////////
    if ( PARM(VPRPVrefFix))
    {
        VPRPVFbkAvg = 1.0F;
    }
    else
    {
        VPRPVFbkAvg = VPRPVFbkAvg1;
    }
     ///////////////////////////////
     // Pick the output for Iy
     ///////////////////////////////
     if ( PARM(VPRPIyRstAvg) )
     {
        VPRPIyCmAvg = VPRPIyCmAvg1;
     }
     else
     {
        VPRPIyCmAvg = 0.0;
     }


}


/*=============================================================================
 * Function: VoltVarRules
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
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function suupports rules for the Volt-Var regulator path
 *
 * Related
 *
 *===========================================================================*/
void VoltVarRules(void)
{
    float L_IRatPu;
    float VPRPT3sPrSecTmp;
    float QregIntGnTemp;
    float ParallelTemp;

    INomGrdPkRcp = NoNANLim(1.0F / INomGridPk, 1.0e6F, -1.0e6F);

    L_IRatPu = NoNANLim(PARM(L_IRat)/PARM(SysINomGrid), 1.0e6F, -1.0e6F);



    //////////////////////////////////////////////////
    // Mode selection
    //////////////////////////////////////////////////


    //////////////////////////////////////////////////
    // Grid code configuration --Start
    //////////////////////////////////////////////////

    if ( PARM(VRTSupptTyp) == VRTCONV)
    {
         SET_FV(VPRPEnb, false);
    }
    else
    {
         SET_FV(VPRPEnb, true);
    }

    if ( (PARM(GridStrength) != STRONG) || (PARM(SeriesCmpEnb)) )//weak, very weak, or series comp, then high Z
    {
       SET_FV(GridHighZ, true);
    }
    else
    {
       SET_FV(GridHighZ, false);
    }


    ////////////////////////////////////////////
    // Start Volt-Var Rules -- keep segregated
    ////////////////////////////////////////////

    SET_FV(WPwrGVARCmd, 1.0/DelTm3);
    if (PARM(VoltExtendOpt))
    {
       if (PARM(VregMaxOvr))
       {
          SET_FV(VregRefMax,  PARM(VregMaxExtOpt));
       }
       else
       {
          SET_FV(VregRefMax,  1.12F);
       }
    }
    else if (PARM(AVRPresent))
    {
       SET_FV(VregRefMin, 0.80F);
    }
    else
    {
       SET_FV(VregRefMax,  1.10F);
       SET_FV(VregRefMin,  0.90F);
    }

    ////////////////////////////////////
    //Var regulator parameters
    ////////////////////////////////////

    // VAR reg input & output is pu, therefore gains depend on net pu impedance and desired BW,
    // therefore should be approx the same on the 1.6 and 2.75 turbines

    SET_FV(QregTstCmd, 0.0F);

    SET_FV(QregGnPFMod, 1.0F);

    if ( PARM(QregFFEnb))
    {
        if ( PARM(GridStrength == VERY_WEAK))
        {
           SET_FV(QregIntGn,  0.01F);
           SET_FV(QregFFGn,  13.50F);
           SET_FV(QregFFW,   30.00F);
           SET_FV(QregIntGn0,PARM(QregGnPFMod) *0.6956522F*PARM(QregIntGn));
        }
        else //Weak
        {
           SET_FV(QregIntGn, 0.120F);
           SET_FV(QregFFGn,  1.10F);
           SET_FV(QregFFW,   0.77F);
           SET_FV(QregIntGn0,PARM(QregGnPFMod) *0.6956522F*PARM(QregIntGn));
        }

    }
    else //no feed-forward
    {
        SET_FV(QregIntGn, 0.370F);
        SET_FV(QregFFGn,  0.0F);
        SET_FV(QregFFW,   0.77F);
        SET_FV(QregIntGn0,0.6956522F*PARM(QregIntGn));
    }

    QregIntGnTemp = PARM(QregIntGn);
    if(SimAcclAct)
    {
        SET_FV(QregIntGn, QregIntGnTemp*PARM(QregAcclSim));
    }



    ////////////////////////////////////
    //Voltage regulator parameters
    ////////////////////////////////////

    // VAR reg input & output is pu, therefore gains depend on net pu impedance and desired BW,
    // therefore should be approx the same on the 1.6 and 2.75 turbines

    if ( PARM(GridHighZ) )
    {
        //Formerly 125,000, now normalized on basis of a 1.6MW turbine with INomGridPk of 1893.33 amps, or 125,000/1893.33
        if(SimAcclAct)
        {
            SET_FV(VregIntGn,  66.0F*PARM(VregAcclSim));
            SET_FV(VregPrpGn,   1.2F*PARM(VregAcclSim));
        }
        else
        {
            SET_FV(VregIntGn,  66.0F);
            SET_FV(VregPrpGn,   1.2F);
        }
        //Formerly 75,000, now normalized on basis of a 1.6MW turbine with INomGridPk of 1893.33 amps, or 150,000/1893.33
        SET_FV(VregIntGn0, 22.0F);
        SET_FV(VregPrpGn0,  0.4F);
    }
    else
    {
        if(SimAcclAct)
        {
            SET_FV(VregIntGn,  160.0F*PARM(VregAcclSim));
            SET_FV(VregPrpGn,   0.0F*PARM(VregAcclSim));
        }
        else
        {
            //Formerly 300,000, now normalized on basis of a 1.6MW turbine with INomGridPk of 1893.33 amps, or 300,000/1893.33
            SET_FV(VregIntGn,  160.0F);
            SET_FV(VregPrpGn,   0.0F);
        }
        //Formerly 150,000, now normalized on basis of a 1.6MW turbine with INomGridPk of 1893.33 amps, or 150,000/1893.33
        SET_FV(VregIntGn0, 79.23F);
        SET_FV(VregPrpGn0,  0.0F);

    }
    SET_FV(VregIntGnIs, 1120.0F);//7.0 * non islanding case
    SET_FV(VregPrpGnIs, 0.0F);

    SET_FV(VregPrpMax,  0.2F);
    SET_FV(VregPrpMin, -0.2F);
    SET_FV(VregPrpFilW, 170.0F);

    ////////////////////////////////////
    // Volt boost vs. Power Rise
    ////////////////////////////////////
    SET_FV(VBstPVMaxPu,   1.10F);
    SET_FV(VBstPLimGn,    1.00F);
    SET_FV(VBstPOffs,      0.2);
    SET_FV(VBstPGn,       0.10F);
    SET_FV(VBstPLmDnW,   DelTm3Rcp);
    SET_FV(VBstPLmUpW,   20.0F);

    QregFFWDt      = PARM(QregFFW)    * DelTm2;
    VBstPGnDtRcp   = PARM(VBstPGn)    * DelTm3Rcp;
    VBstPLmUpWdt   = PARM(VBstPLmUpW) * DelTm3;
    VBstPLmDnWdt   = PARM(VBstPLmDnW) * DelTm3;
    QregFFGnDt     = PARM(QregFFGn)   * DelTm2;

    SET_FV(FastDVrgIyMx, -0.1F);

    SET_FV(VregRefIsld, 0.75F);
    SET_FV(VregIyIsldMx,    0.79F);
    SET_FV(VregIyIsldMn,   -1.25F);   // (P.INom in ESS)
    SET_FV(VregTstCmd, 0.0F);                 // pu
    SET_FV(VregLimHyst, 10.0F*INomGrdPkRcp);  // amps (pu)

    SET_FV(VregPrcFilW, 300.0F);
    VregPrcFilGn = PARM(VregPrcFilW)*DelTm2;

    SET_FV(TrqEnbStPuDlyTm, (13.5F/300.0F));      // delay to allow precondition to settle
    SET_FV(CTChkEnbDoDlyTm, (13.5F/300.0F));

    /* Power angle command limit */
    SET_FV(PfLim, 0.866025); //eqivalent to 30deg phi (previous limit)
    /* final value ram images for user Visio's */

    SET_FV(IyCmdForce, 0.0F);

    //Iy boost function//

    if (PARM(GridStrength) != STRONG)   // weak or very weak
    {
       SET_FV(LvIyKSpdThr,  -800.0);
    }
    else //Strong
    {
       SET_FV(LvIyKSpdThr,  PARM(R_RPMSync));
    }

    if (!PARM(LvIyKTmOvr))
    {
       SET_FV(LvIyKPulseTm, 0.075);
    }
    else
    {
       SET_FV(LvIyKPulseTm, PARM(LvIyKTmMxOvr));
    }

    if (!PARM(LvIyKIOvr))
    {
       SET_FV(LvIyKickMax,  0.25);
    }
    else
    {
       SET_FV(LvIyKickMax,  PARM(LvIyKIMxOvr));
    }


    SET_FV(LvIyKPrmTm,   5.0); //At 75% voltage, requrement is 3.15 seconds, so prevent re-sart in a single dip
    SET_FV(LvIyKickRstK, PARM(LvIyKPrmTm)/PARM(LvIyKPulseTm));
    SET_FV(LvIyKickDcy,  -50.0);
    SET_FV(LvIyKickGn,   100.0);
    SET_FV(LvIyKickVThr, 0.75);
    SET_FV(LvIyKickDvGn, 3.20);

    S_VoltsRcp = 1.0F/(PARM(S_Volts)*SQRT2/SQRT3);

    ////////////////////////////////////////////
    // End Volt-Var Rules -- keep segregated
    ////////////////////////////////////////////


    SET_FV(LvrtUvHyst, 7.0F); //7% hysterisis -- On at 80%, Off at 87%

    //AC Voltage regulator dynamic clamp rules
    SET_FV(VregVpuFilW, 100.0F);

    SET_FV(VregDVFilOff, 0.15F);
    if ( PARM(VPRPEnb) )
    {
       SET_FV(VregDVGn, 0.0F);
    }
    else
    {
       SET_FV(VregDVGn, 1.6F);
    }

    SET_FV(VregMxMnDiff, 0.25F);
    SET_FV(VregLvSlope, -10.0F);
    SET_FV(VregHvSlope, -10.0F);
    SET_FV(VregMnLvSlp,  PARM(VregRefMin));
    SET_FV(VregMxHvSlp,  1.10F);
    SET_FV(VregMaxLv1,   PARM(VregRefMin));
    SET_FV(VregMaxLv2,   0.50F);
    SET_FV(VregMinHv1,   1.00F);
    SET_FV(VregMinHv2,   1.20F);
    if ( PARM(TestMode) == PUMP_BACK )
    {
       SET_FV(IyMaxLimSS,    0.0F);
    }
    else
    {
        SET_FV(IyMaxLimSS,   0.01F*PARM(SysIOVLoad)*PARM(SysIyRat));
    }
    IyMaxLim1    = SQRT2 * PARM(IyMaxLimSS);
    SET_FV(VregIyLim,    1.05F*PARM(IyMaxLimSS));
    SET_FV(VregIyLimPu,  NoNANLim(PARM(VregIyLim)/PARM(SysINomGrid), 1.0e6F, -1.0e6F));
    SET_FV(VrgIyLmAsm,   0.9F* PARM(IyMaxLimSS));
    SET_FV(VregNSqHiThr, 0.053249743F * L_VlnIn);//for suicide of volt-var regulator ...derived from GmNSeqHiThr, but lower
                                               //to accomodate 600 sec tests, at 9%...suicides volt-var reg.
                                               //it amounts to two line to neutrals at .869pu and on at 1.0pu,
                                               //and GmNegSeqVPct = 4.8%.  Steady state requirement is 2.5%

    SET_FV(VregFrzHThrs,1.13F);
    SET_FV(VregFrzLThrs,PARM(VregRefMin)-0.03F);
    SET_FV(VregFrzFilW, 80.0F);
    SET_FV(VregHvKickGn, 5.0F);
    SET_FV(VregOutBias, 0.05F);

    if ( PARM(VPRPEnb) )
    {
        SET_FV(VregCorrW, 75.0F);
    }
    else
    {
        SET_FV(VregCorrW, 0.0F);
    }

    if(SimAcclAct)
    {
        SET_FV(VnVregRedGn, 1.00F*PARM(VmaxRegAcclSim));    //  increase vamx control in acceleration mode
    }
    else
    {
        SET_FV(VnVregRedGn, 1.00F);
    }

    SET_FV(VnVregRedThr,PARM(VregRefMax) + 0.005F);

    VregPrpFilGn   = PARM(VregPrpFilW) * DelTm2;
    VregCorrGn     = PARM(VregCorrW) * DelTm2;



    if ( PARM(LvrtIyOvr) )
    {
        SET_FV(LvIyRat, PARM(LvrtIyMaxOvr));
    }
    else
    {
       SET_FV(LvIyRat, 1.066053F);
    }

    if ( PARM(HvrtIyOvr) )
    {
        SET_FV(HvIyRat, PARM(HvrtIyMaxOvr));
    }
    else
    {
       SET_FV(HvIyRat, 1.4537F);
    }



    SET_FV(LvIyRatNW,   1.613F * L_IRatPu); //gives 442arms, same as 1.5converter
    SET_FV(VregIyLimNW, NoNANLim((1.05F * PARM(L_IRatSSLim) + SQRT2_RCP*(L_VlnIn * PARM(L_LineFrq) * PARM(CnvLineDfC)))/PARM(SysINomGrid), 1.0e6F, -1.0e6F) );
    SET_FV(VrgIyLmNWAsm, 0.5F * PARM(VregIyLimNW));
    SET_FV(HvIyRatNW,   (1.613F * L_IRatPu));
    SET_FV(VregLvSlpNW, -10.0F* L_IRatPu * 1.241F); //1.241 factor is here to make same as 1.5, since L_IRat is higer for it
    SET_FV(VregHvSlpNW, -10.0F* L_IRatPu * 1.241F);
    SET_FV(VregMxMnDfNW,0.25F * L_IRatPu * 1.241F);

    /* Iy filter omega for Ix limit for lvrt */
    SET_FV(LvIxW, 200.0F );
    SET_FV(LvIxRLUpPu, 5.0F );
    SET_FV(LvIxRLDnPu, 300.0F );

    LvIxRLUp  = PARM(LvIxRLUpPu) * S_IxRat;
    LvIxRLDn  = PARM(LvIxRLDnPu) * S_IxRat;


    if ( PARM(GridHighZ) )
    {
        SET_FV(VregLVMnAb,   -PARM(VregIyLimPu));
        SET_FV(VregLVMnAbNW, -PARM(VregIyLimNW));
    }
    else//Set out of way, positive,  for strong grids.
    {
        SET_FV( VregLVMnAb,   3.0F * PARM(VregIyLimPu));
        SET_FV( VregLVMnAbNW, 3.0F * PARM(VregIyLimNW));
    }
    //
    VregIyLimPk    = PARM(VregIyLimPu);
    LvIyRatPk      = PARM(LvIyRat);
    HvIyRatPk      = PARM(HvIyRat);
    VregMxMnDiffPk = PARM(VregMxMnDiff);
    VregLvSlopePk  = PARM(VregLvSlope);
    VregHvSlopePk  = PARM(VregHvSlope);
    IyMaxLimSSPk   = PARM(IyMaxLimSS);
    VregLVIyMnDyPk = PARM(VregLVMnAb);

    LvIyRatNWPk    = PARM(LvIyRatNW);
    VregIyLimNWPk  = PARM(VregIyLimNW);
    VrgIyLmNWAsmPk = PARM(VrgIyLmNWAsm);
    HvIyRatNWPk    = PARM(HvIyRatNW);
    VregLvSlpNWPk  = PARM(VregLvSlpNW);
    VregHvSlpNWPk  = PARM(VregHvSlpNW);
    VregMxMnDfNWPk = PARM(VregMxMnDfNW);
    VregLVIyMnDyNWPk= PARM(VregLVMnAbNW);

    VregFrzFilGn    = PARM(VregFrzFilW) * DelTm2;


    LvSlopeMax     =  NoNANLim((PARM(LvIyRat)- PARM(VregIyLimPu))/(PARM(VregMaxLv2) - PARM(VregMaxLv1)), 1.0e6F, -1.0e6F);

    HvSlopeMin     =  NoNANLim((-PARM(VregIyLimPu) - (-PARM(HvIyRat)) )/(PARM(VregMinHv1)-PARM(VregMinHv2)), 1.0e6F, -1.0e6F);

    LvSlopeMaxNW   =  NoNANLim((PARM(LvIyRatNW)- PARM(VregIyLimNW))/(PARM(VregMaxLv2) - PARM(VregMaxLv1)), 1.0e6F, -1.0e6F);

    HvSlopeMinNW   =  NoNANLim((-PARM(VregIyLimNW) - (-PARM(HvIyRatNW)) )/(PARM(VregMinHv1)-PARM(VregMinHv2)), 1.0e6F, -1.0e6F);


    QregClmpSF     =  SQRT3*SQRT2_RCP*PARM(SysVoltNom);


    //Start vreg clamp proportional path Rules (formerly SDL2)
    SET_FV(VPRPFilW, 1.0F);       //  1.0 sec tau
    SET_FV(VPRPFilWFast,1.0F);    //  1 rad/sec
    SET_FV(VPRPWSlowTm, NoNANLim(6.0F/PARM(VPRPFilW), 1.0e6F, -1.0e6F)); //6 tau's
    SET_FV(VPRPNoise,200.0e-6F);  //  0.1pu voltage in 2msec
    SET_FV(VPRPNoise2,200.0e-6F); //  0.1pu voltage in 2msec
    SET_FV(VPRPHvDsbThr, 1.12F);  //  1.12pu voltage
    SET_FV(VPRPIyTolMax, 0.05F);
    SET_FV(VPRPIyTolMin, 0.05F);
    SET_FV(VPRPAvgTm, 60.0F);
    SET_FV(VPRPSmpTm, (PARM(VPRPAvgTm)/VPRP_AVG_ARRAY_SIZE));

    VPRPT3sPrSecTmp = DelTm3Rcp;
    SET_FV(VPRPSmpCntTh,(int)(VPRPT3sPrSecTmp*PARM(VPRPSmpTm)) );

    VPRPFilGnDyn   = PARM(VPRPFilW) * DelTm3;
    VPRPFilGnSlw   = PARM(VPRPFilW) * DelTm3;//not used
    VPRPFilGnFst   = PARM(VPRPFilWFast) * DelTm3;//not used
    VPRPAryCtRcp   = NoNANLim(1.0F/(VPRP_AVG_ARRAY_SIZE * PARM(VPRPSmpCntTh)), 1.0e6F, -1.0e6F);
    VPRPT3sPrSec   = VPRPT3sPrSecTmp;
    VPRPSmpThRcp   = NoNANLim(1.0F/PARM(VPRPSmpCntTh), 1.0e6F, -1.0e6F);

    //End VPRP Rules

    SET_FV(VregNSeqIyLm, 0.45F);
    SET_FV(VregNSeqIyNw, NoNANLim(0.50F*(PARM(L_IRatSSLim)/PARM(SysINomGrid)), 1.0e6F, -1.0e6F));//scale down line converter current to per unit of INom

    if(PARM(TestMode) == PUMP_BACK)
    {
        SET_FV(IyCmdOvr, true);
    }
    else
    {
        SET_FV(IyCmdOvr, false);
    }

    SET_FV(IyAsySpdOn, 1.05F*PARM(R_RPMNom));
    SET_FV(IyAsySpdOff,0.95F*PARM(R_RPMNom));
    SET_FV(IyAsyIxRef, -0.01F*R_IxRat);
    SET_FV(IyAsyIxW, 50.0F ); //
    SET_FV(IyAsyIxMin, -PARM(VregIyLim)*SQRT2);

    if (PARM(L_FrqInput) == 60.0F)
    {
        SET_FV(R_WeSlipRat, NoNANLim(PI2 * (60.0F * (1600.F - 1200.F)/1200.F), 1.0e6F, -1.0e6F)); // R_WElec at Wgrid=nom & top speed
    }
    else
    {
        SET_FV(R_WeSlipRat, NoNANLim(PI2 * (50.0F * (2000.F - 1500.F)/1500.F), 1.0e6F, -1.0e6F));
    }
    R_WeSlipRatRcp = NoNANLim(1.0F / PARM(R_WeSlipRat), 1.0e6F, -1.0e6F);   // rated electrical slip freq - reciprocal

    SET_FV(R_WeSlipFilW,    25.0F);
    SET_FV(VmagHiSlipW,     50.0F);
    SET_FV(VmagHiSlipLdOfs,  0.1F);
    SET_FV(VmagHiSlipLdK,    2.0F);
    SET_FV(VmagHiSlipLdLim,  0.3F);
    SET_FV(VmagHiSlipMin,    0.0F);
    SET_FV(VregMaxHiSlipMx,  1.1F);
    SET_FV(VregMaxHiSlipGn,  5.0F);



// Modulation index limiting based on Iy needed to hold mod index.
    S_ZBase      = PARM(S_Volts) * PARM(S_Volts)/(1000.0F * PARM(TurbineNpRat));
    R_VlnBase    = PARM(S_Volts) / PARM(S_RRatio) * SQRT2_3;
    R_VlnBaseRcp = 1.0F/R_VlnBase;

    //A1 = S_ZBase/ (S_X2 + (S_X1 || S_Xm) );
    ParallelTemp = (S_X1 * S_Xm)/(S_X1 + S_Xm);
    SET_FV(VrgIyMxA1, S_ZBase / (S_X2 + ParallelTemp) ); // coefficient 1 for final Iy output

    //A2 = Xm/ (S_X1 + S_Xm);
    SET_FV(VrgIyMxA2, S_Xm / (S_X1 + S_Xm) ); // coefficient 2 for final Iy output

    //A3 = S_ZBase/ (S_X1 + (S_X2 || S_Xm) );
    ParallelTemp =    (S_X2 * S_Xm)/(S_X2 + S_Xm);
    SET_FV(VrgIyMxA3, S_ZBase / (S_X1 + ParallelTemp) ); // coefficient 3 for final Iy output


    SET_FV(VrgIyMxModThr,      1.15F);  // Modulation index reference

    SET_FV(VrgIyMxModFilW,     400.0F); // Modulation index fileter W
    VrgIyMxModFilGn = DelTm2 * PARM(VrgIyMxModFilW);

    SET_FV(VrgIyMxRModLimUp,  10.0F);  // Up filter "W" for up/down filter of modulation index error
    VrgIyMxRModLimUpGn  = DelTm2 * PARM(VrgIyMxRModLimUp);

    SET_FV(VrgIyMxRModLimDn,100.0F);  // Down filter "W" for up/down filter of modulation index error
    VrgIyMxRModLimDnGn = DelTm2 * PARM(VrgIyMxRModLimDn);

    SET_FV(VrgIyMxRModLimMax, 1.20F);  // Upper limit for up/down filter of modulation index error
    SET_FV(VrgIyMxRModLimMin, 1.05F);  // Lower limit for up/down filter of modulation index error
    SET_FV(VrgIyMxSlipMin,    0.01F);  // Minimum slip frequency to preven divide by zero
    SET_FV(VrgIyMxSlipFilW,   25.0F);  // filter response for per unit slip filter
    VrgIyMxSlipFilGn = DelTm2 * PARM(VrgIyMxSlipFilW);


    if ( PARM(SeriesCmpEnb) )
    {
       if( PARM(SoftwareIPN) < DFIG02_2400_5060_A)
       {
          SET_FV(VrgIyMxRModEnb,  false); // turn both of these schemes off...to make identical to 1.6e performance.
          SET_FV(VregMaxHiSlipEnb,false);
       }
       else
       {
          SET_FV(VrgIyMxRModEnb,  true); // Lower limit for up/down filter of modulation index error
          SET_FV(VregMaxHiSlipEnb,false);  // Enable high slip algorithm
       }

    }
    else //series comp not enabled
    {
       SET_FV(VrgIyMxRModEnb,  false);  // only enable high slip logic since most of 2.3/1.6e Markstat DOEs used this
       SET_FV(VregMaxHiSlipEnb,true);
    }

    R_WeSlipFilGn   = PARM(R_WeSlipFilW) * DelTm2;
    VmagHiSlipFilGn = PARM(VmagHiSlipW)  * DelTm2;

     ////////////////////////////////
     // IRAM copy population
     ////////////////////////////////
    VrgIyLmAsmPk     =  PARM(VrgIyLmAsm) * SQRT2;

    VregTstCmd       =  PARM(VregTstCmd);


    /* Calculate Iy factor needed to balance Ix for desired pwr factor from real power  */



    PwrGVARCmdGn  = MIN(1.0F, (PARM(WPwrGVARCmd) * DelTm3));



}
