//
// TITLE:       Generic fault checking in all tasks
//
// DESCRIPTION:
//
// COPYRIGHT:   Copyright (c) 2010
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
#include "x0Vector.h"
// product
#include "Product.h"
#include "b0BridgeRebf.h"
#include "b0BrgThrm5p.h"
#include "b0BrgThrmstr.h"
#include "b0DbCtl.h"
#include "p0FltChk.h"
#include "x0Aead.h"
#include "p0Line.h"
#include "p0Sseq.h"
#include "b0DbThrm.h"


// Externals
//----------
extern FaultIsActiveTyp FaultIsActive;
extern nvVar_tbl        NV;
extern EpldCmdRgr      *EpldCmdP;
extern cBridgeRebf      L_BridgeRebf;
extern cBridgeRebf      R_BridgeRebf;
extern cBrgThrm5pModel  L_BrgThrm5pModel;
extern cBrgThrm5pModel  R_BrgThrm5pModel;
extern cBrgThrmstr      L_BrgThrmstr;
extern cBrgThrmstr      R_BrgThrmstr;
extern float            CoolRHeatRcp;
extern float            DelTm2;
extern float            DelTm3;
extern float            DFConvISqr;
extern float            DFVaDvDtSqT3;
extern float            DFVaDvDtT2;
extern float            DFVbDvDtSqT3;
extern float            DFVbDvDtT2;
extern float            DFVcDvDtSqT3;
extern float            DFVcDvDtT2;
extern float            GmCnvOVMinx;
extern float            GmCnvOVThrx;
extern float            GmIaT2;
extern float            GmIbT2;
extern float            GmIcT2;
extern float            GmLVmtchErGn;
extern float            GmLVmtchThr;
extern float            XfmrSecVmtchGn;
extern float            GmOVPuVSec;
extern float            GmPwrT2;
extern float            GmVMagPuT2;
extern float            GmSLIaErSqT3;
extern float            GmSLIbErSqT3;
extern float            GmSLIcErSqT3;
extern float            GmSVmtchThr;
extern float            GmVabT2;
extern float            GmVbcT2;
extern float            GmVMagT3;
extern float            GmVyFbkT2;
extern float            IOpn1AbsNWGn;
extern float            L_BrgLssGn;
extern float            L_IaT2;
extern float            L_IbT2;
extern float            L_IcT2;
extern float            L_VabLineT2;
extern float            L_VbcLineT2;
extern float            L_VdcFbkT2;
extern float            LSVmtchErGn;
extern float            LSVmtchThr;
extern float            PLCRbtCmdT3;
extern float            PmpShftPwr;
extern float            R_BrgLssGn;
extern float            R_SpdFbkT2;
extern float            R_SpdGen;
extern float            R_TorSpdInT3;
extern float            R_TorSpdSFT3;
extern float            R_TrqCmdInT2;
extern float            S_IaT2;
extern float            S_IbT2;
extern float            S_IcT2;
extern float            S_VabT2;
extern float            S_VbcT2;
extern float            S_VcaT2;
extern float            S_Pwr;
extern float            S_VanT2;
extern float            S_VbnT2;
extern float            S_VcnT2;
extern float            XfmrRatioLnStrRcp;
extern float            XfmrRatioLnStr;
extern float            GmVanCompT2;
extern float            GmVbnCompT2;
extern float            GmVcnCompT2;
extern float            T1T2FbkScale;
extern float            L_IabcDFcomp0;
extern float            L_IabcDFcomp1;
extern float            L_VdcFbkT3;
extern float            GmPllWT2;
extern float            GmPwrGridT2;
extern float            R_VMgIxDcyDy;
extern float            R_IxErr2Fil;
extern float            R_IyErr2Fil;
extern float            R_VrgIntGnDy;
extern float            R_VrgIntLmDy;
extern float            R_IxrgLimDyn;
extern float            R_IyrgLimDyn;
extern float            R_ResLimDyn;
extern unsigned         R_DcILimDynZero;
extern float            L_IrgSplGnDy;
extern float            L_IyCmdLimDy;
extern float            S_VMagPU;
extern float            GmVMagPuT1;
extern float            R_TrqCalT2;
extern float            LvIyMaxDyn;
extern float            FanOnGthRcp;
extern float            InletAirTemp;
extern float            CoolntTempCalcGn;
extern int              T2FramesPerT3;
extern int              SeqSt;
extern unsigned         CoolntHeatOn;
extern unsigned         DTAFanCmd;
extern unsigned         S_K1ClsCmd;
extern unsigned         S_K1Status;
extern unsigned         L_MaClsCmd;
extern unsigned         L_BrgPwrEnb;
extern unsigned         L_CelTstCmd;
extern unsigned         L_Running;
extern unsigned         LineRunOnly;
extern unsigned         R_InnrRegEnb;
extern unsigned         R_Running;
extern unsigned         R_ZeroSpd;
extern unsigned         RPNWRunning;
extern unsigned         SimMode;
extern unsigned         Stopped;
extern unsigned         SysTmSec;
extern unsigned         TransDiagInh;
extern unsigned         TestConfigErrAct;
extern unsigned         XfmrTypeUnspecAct;
extern unsigned         OpnPhsExtEnb;
extern int              DbInh;
extern unsigned         FastBlkReqHw;
extern unsigned         OVProtectTrb;
extern float            GmIGndMag;

extern unsigned         GmUV1TrpAct;
extern unsigned         GmUV2TrpAct;
extern unsigned         GmUV3TrpAct;
extern unsigned         GmUV4TrpAct;
extern unsigned         GmUV5TrpAct;
extern unsigned         GmUV6TrpAct;
extern unsigned         GmUVCvTrpAct;
extern unsigned         GmOV1TrpAct;
extern unsigned         GmOV2TrpAct;
extern unsigned         GmOV3TrpAct;
extern unsigned         GmOV4TrpAct;
extern unsigned         GmUF1TrpAct;
extern unsigned         GmUF2TrpAct;
extern unsigned         GmUF3TrpAct;
extern unsigned         GmOF1TrpAct;
extern unsigned         GmOF2TrpAct;
extern unsigned         GmUV1TrpFlg;
extern unsigned         GmUV2TrpFlg;
extern unsigned         GmUV3TrpFlg;
extern unsigned         GmUV4TrpFlg;
extern unsigned         GmUVCvTrpF;
extern unsigned         GmOV1TrpFlg;
extern unsigned         GmOV2TrpFlg;
extern unsigned         GmOV3TrpFlg;
extern unsigned         GmOV4TrpFlg;
extern unsigned         GmUF1TrpF;
extern unsigned         GmUF2TrpF;
extern unsigned         GmUF3TrpF;
extern unsigned         GmOF1TrpF;
extern unsigned         GmOF2TrpF;

extern unsigned         R_BrgPwrEnbT1;
extern unsigned         L_BrgPwrEnbT1;
extern unsigned         DiagResetCmdT2;
extern unsigned         Q1OpnReq;
extern unsigned         Q1Status;
extern unsigned         KdfStatus;
extern float            R_DelTm1;
extern unsigned         GmOVTrpSum;
extern float            GmIGndProtFilWdt;

extern float            GmVanT2;
extern float            GmVbnT2;
extern float            GmVcnT2;

// FaultChkWindT3()
extern float            GmVMagPuT3;
extern float            GmPwrT3;
extern float            R_TrqCmdInTtT3;
extern float            R_SpdFbkT3;
extern unsigned         GmLineVLo;
extern float            GmIMagT3;

// HVACStateMachine()
extern unsigned         R_SeqReq_PackedBits;
extern unsigned         Q1StatSim;
extern unsigned         ISimMode;
extern unsigned         S_K1OpnNowReq;

extern unsigned         DisconWhlOv;
extern unsigned         OvCnvFltState;

extern cDbThrmModel    L_DbThrmModel;
extern unsigned        GmFrqVltStopAct;
extern unsigned        StpCmdDlyFlg;

extern float            OpnGrdTmDyn;
extern float            OpnPhsTmDyn1;
extern float            OpnPhsTmDyn2;

// Types and Classes
//------------------
struct Flt_Summ_Typ
{
    unsigned Brg1CurrentTrip :1;        // bit 00 - Bridge 1 current trip active
    unsigned Brg1VoltageTrip :1;        // bit 01 - Bridge 1 voltage trip active
    unsigned Brg1TemprTrip   :1;        // bit 02 - Bridge 1 temperature trip active
    unsigned Brg1OtherTrip   :1;        // bit 03 - Bridge 1 other trip active
    unsigned                 :4;        // bit 04..07 - spare
    unsigned Brg2CurrentTrip :1;        // bit 08 - Bridge 2 current trip active
    unsigned Brg2VoltageTrip :1;        // bit 09 - Bridge 2 voltage trip active
    unsigned Brg2TemprTrip   :1;        // bit 0A - Bridge 2 temperature trip active
    unsigned Brg2OtherTrip   :1;        // bit 0B - Bridge 2 other trip active
    unsigned                 :4;        // bit 0C..0F - spare
    unsigned Brg1CurrentAlrm :1;        // bit 10 - Bridge 1 current alarm active
    unsigned Brg1VoltageAlrm :1;        // bit 11 - Bridge 1 voltage alarm active
    unsigned Brg1TemprAlrm   :1;        // bit 12 - Bridge 1 temperature alarm active
    unsigned Brg1OtherAlrm   :1;        // bit 13 - Bridge 1 other alarm active
    unsigned                 :4;        // bit 14..17 - spare
    unsigned Brg2CurrentAlrm :1;        // bit 18 - Bridge 2 current alarm active
    unsigned Brg2VoltageAlrm :1;        // bit 19 - Bridge 2 voltage alarm active
    unsigned Brg2TemprAlrm   :1;        // bit 1A - Bridge 2 temperature alarm active
    unsigned Brg2OtherAlrm   :1;        // bit 1B - Bridge 2 other alarm active
    unsigned                 :4;        // bit 1C..1F - spare
};


// Function Prototypes
//--------------------


// Parameters
//-----------

// OpenPhaseCheckT2()
CREATE_PARM(OpnPhSatCnt,        int);       // Open phase saturation count required declare OpnPhaseSat
CREATE_PARM(OpnPhSatThr2,       float);     // Open phase saturation threshold 2
CREATE_PARM(OpnPhSatThr1,       float);     // Open phase saturation threshold 1
CREATE_PARM(OpnPhsMltMtdEnb,    unsigned);  // Enable the latched open phase detection method
CREATE_PARM(OpnPhsCntThr1,      int);       // Open phase detect counter threshold for secondary side
CREATE_PARM(OpnPhsLTmrUpGn1,    float);     // Open phase detect long timer increase gain for secondary side
CREATE_PARM(OpnPhsLgTmrDecGn1,  float);     // Open phase detect long timer decrease gain for secondary side
CREATE_PARM(OpnPhsRstTm1,       float);     // Open phase detect reset timer threshold for secondary side
CREATE_PARM(OpnPhsLgTm1,        float);     // Open phase detect long timer threshold for secondary side
CREATE_PARM(OpnPhsCntThr2,      int);       // Open phase detect counter threshold for primary side
CREATE_PARM(OpnPhsLTmrUpGn2,    float);     // Open phase detect long timer increase gain for primary side
CREATE_PARM(OpnPhsLTmrDnGn2,    float);     // Open phase detect long timer decrease gain for primary side
CREATE_PARM(OpnPhsRstTm2,       float);     // Open phase detect reset timer threshold for primary side
CREATE_PARM(OpnPhsLgTm2,        float);     // Open phase detect long timer threshold for primary side
CREATE_PARM(OpnPhsIpuThrs,      float);     // Open phase detect open phase current relative threshold in pu
CREATE_PARM(OpnPhsRtrFstStp,    unsigned);  // Open phase fast shutdown rotor converter when there is trip from line converter

CREATE_PARM(GmIGndProtFilW,     float);     // W for Task3 filtered ground fault current for grid monitor ground fault detection
CREATE_PARM(GmGndFltThr,        float);     // Grid monitor ground fault threshold
CREATE_PARM(GmGndAlmOn,         float);     // Grid monitor ground fault alarm on
CREATE_PARM(GmGndAlmOff,        float);     // Grid monitor ground fault alarm off
CREATE_PARM(GmGndFltSatCnt,       int);     // Open phase saturation threshold for gound fault protection
CREATE_PARM(GmGndFltInhTm,      float);     // Open phase saturation hold time for ground fault protection
CREATE_PUBVAR(GmIGndMagT3,     float );     // Gm ground current magnitude


// OpenGridCheckT2()
CREATE_PARM(GmSVMtchIThr,       float);
CREATE_PARM(GmSVMatchTh2,       float);     // Grid monitor to stator voltage match threshold (Volts)
CREATE_PARM(GmLVMatchThr,       float);     // Grid monitor to line voltage match threshold (Volts)
CREATE_PARM(GmSVMatchThr,       float);     // Grid monitor to stator voltage match threshold (Volts)
CREATE_PARM(LSVMatchThr,        float);     // Line to stator voltage match threshold (Volts)
CREATE_PARM(OpnGrdStrVMx,       float);     // Open grid stator voltage max (Pu)
// not used Converter OV
CREATE_PARM(GmCnvOVMin,         float);     // Gm filter converter OV Min
CREATE_PARM(GmCnvOVThr,         float);     // Gm filter converter OV Vsec threshold
CREATE_PARM(GmCnvOVHThr,        float);     // Gm converter over-voltage v-sec high threshold
CREATE_PARM(GmCnvOVLThr,        float);     // Gm converter over-voltage v-sec low threshold
CREATE_PARM(GmCnvOVLTm,         float);     // Gm converter over-voltage v-sec low time
// Islanding detection parmameters  - may "belong" to other files (see where they are used)
CREATE_PARM(S_VMaxHiTm  ,      float);      // Maximum time to detect islanding
CREATE_PARM(R_VrgIntHiTm,      float);      // Maximum time to all rotor vreg integral hi
CREATE_PARM(S_VMaxIyFsTm,      float);      // Maximum time to run fast Iy path during HVRT cycle
CREATE_PARM(R_VrgIntLmHi,      float);      // Maximum rotor volt reg integral limit for islanding
CREATE_PARM(S_VMgILimPkW,      float);      // Stator voltage peak detet response (rad/sec)
CREATE_PARM(S_VMgILimUpW,      float);      // Stator voltage peak detet response up (rad/sec)
CREATE_PARM(S_VMaxHiOfTm,      float);      // Stator voltage S_VMaxHiDet de-activate  time (sec)
CREATE_PARM(S_VMaxHiDtOn,      float);      // Stator voltage high eneough activate S_VMaxHiDet (pu)
CREATE_PARM(S_VMaxHiDtOf,      float);      // Stator voltage low eneough to de-activate S_VMaxHiDet (pu)
CREATE_PARM(S_VMaxShtDV ,      float);      // Stator voltage pu voltage for shutdown permission after islading is detected
CREATE_PARM(S_VMaxShtDTm,      float);      // Required time for Vpu to be below  S_VMaxShtDV for shutdown permission
CREATE_PARM(S_VMaxPllThr,      float);      // Stator voltage control pll filter max allowed error
CREATE_PARM(S_VMaxPllTh2,      float);      // Stator voltage control pll filter max allowed when in "Normal state"
CREATE_PARM(S_VMaxPllMin,      float);      // Stator voltage control pll filter min allowed freq
CREATE_PARM(S_VMaxPllFW ,      float);      // Stator voltage control pll filter respsonse (rad/sec)
CREATE_PARM(S_VMaxPllFRt,      float);      // Stator voltage control pll filter up/down rate limit in pu/sec
CREATE_PARM(S_VMaxPwrW  ,      float);      // Islanding detection power delta filter
CREATE_PARM(S_PwrMaxIs  ,      float);      // Power  maximum for islanding
CREATE_PARM(S_PwrMaxIsTm,      float);      // Power  maximum for islanding time
CREATE_PARM(S_PwrMxIsFst,      float);      // Power maximum islanding inhibit threshold fast

// Islanding related parmameters (a0 in ESS)
CREATE_PARM(R_VrgIntGnHi,      float);  // voltage regulator integral gain for islanding protection
// a0vreg   a0rule - not yet

// Islanding related parmameters (s0 in ESS)
CREATE_PARM(L_IyCmdLmHi ,      float);  //
CREATE_PARM(L_IrgSplGnHi,      float);  // current regulator spill gain for S_VMaxHiDet

CREATE_PARM(GmUV1TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmUV2TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmUV3TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmUV4TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmUV5TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmUV6TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmUVCvTrpTyp,   unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmOV1TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmOV2TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmOV3TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmOV4TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmUF1TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmUF2TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmUF3TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmOF1TrpTyp,    unsigned);  // Turbine or Fast Connect trip type
CREATE_PARM(GmOF2TrpTyp,    unsigned);  // Turbine or Fast Connect trip type

CREATE_PARM(FastDisEnb,     unsigned);  // Fast disconnect enable
CREATE_PARM(FastBlkEnb,     unsigned);  // Fast blocking enable
CREATE_PARM(FastBlkTst,     unsigned);  // Fast blocking test
CREATE_PARM(FastDIRateLim,     float);  // Fast shutdown decrement multiplier

CREATE_PARM(R_TorProtScl,      float);  // Torsional protection scale factor for flt thresholds

// FaultChkWindT3()
CREATE_PARM(S_VMaxFFOnTm,      float);  // On time for islanding frequency freeze logic
CREATE_PARM(S_VMaxFFOfTm,      float);  // Off time for islanding frequency freeze logic
CREATE_PARM(S_VMaxFrqFOn,      float);  // Voltage On level for islanding frequency freeze logic
CREATE_PARM(S_VMaxFrqFOf,      float);  // Voltage Off level for islanding frequency freeze logic
CREATE_PARM(S_PwrLwCmIs,       float);  // Power low command minimum for islanding entry during high voltage events
CREATE_PARM(S_PwrLwFbIs,       float);  // Power low feedback minimu for islanding entry during high voltage events
CREATE_PARM(S_VHiPwrLTm,       float);  // Ac voltage hi and power low timer for islanding

// HVACStateMachine()
CREATE_PARM(S_VMaxPllTh3,      float);  // Per unit frequency threshold when SVHiPwrLo for islanding control
CREATE_PARM(HvacQ1OpnTm,       float);  // HVAC Q1 wait open time

// Cooling and heating system
CREATE_PARM(FanOnGth60,        float);  // Heat exchanger thermal conductance with fan running at 60 hz (Watt/degC)
CREATE_PARM(FanOnGth50,        float);  // Heat exchanger thermal conductance with fan running at 50 hz (Watt/degC)
CREATE_PARM(CoolRHeatr,        float);  // Coolant heater resistance (Ohms)
CREATE_PARM(CFCXfrmRatio,      float);  // CFC transformer (T2); low to high ratio
CREATE_PARM(BtmHiJncTemp,      float);  // Worst-case junction temperature used in thermal model (degC)
CREATE_PARM(BtmDLssComp,       float);  // Diode power loss compensation factor (1/degC)
CREATE_PARM(BtmTrLssComp,      float);  // Transistor power loss compensation factor (1/degC)
CREATE_PARM(L_BrgLssFilW,      float);  // Line bridge losses - filter omega (Rad/Sec)
CREATE_PARM(R_BrgLssFilW,      float);  // Rotor bridge losses - filter omega (Rad/Sec)
CREATE_PARM(PmpShftPwr50,      float);  // Pump shaft power at 50 hz (Watts)
CREATE_PARM(PmpShftPwr60,      float);  // Pump shaft power at 60 hz (Watts)
CREATE_PARM(CoolntTempCalcW,   float);  // Filter omega for coolant temperature calculation (Rad/Sec)
CREATE_PARM(CoolTempCalcFrz,unsigned);  // Coolant temperature calculation freeze
// Moved from z0Parm
CREATE_PARM(WGridAbc1,          float);     // Low pass filter response for grid current 1 filter
CREATE_PARM(WGridAbc2,          float);     // Low pass filter response for grid current 2 filter
CREATE_PARM(OpnPhsThr1,         float);     // Open phase minimum zero current threshold  for currents '1'
CREATE_PARM(OpnPhsUThr1,        float);     // Open phase minimum non-zero current threshold  for currents '1'
CREATE_PARM(OpnPhsThr2,         float);     // Open phase minimum zero current threshold  for currents '2'
CREATE_PARM(OpnPhsUThr2,        float);     // Open phase minimum non-zero current threshold  for currents '2'
CREATE_PARM(OpnPhsInh,          unsigned);  // Open phase protection inhibit
CREATE_PARM(OpnPhsTm1,          float);     // Open phase time 1
CREATE_PARM(OpnPhsTm2,          float);     // Open phase time 2
CREATE_PARM(OpnPhsTm1Mp,        float);     // Open phase torque level below which a longer zero current time is required
CREATE_PARM(OpnPhsTm1Lp,        float);     // Open phase torque level below which a longer zero current time is required
CREATE_PARM(OpnPhsTm1Lp0,       float);     // Open phase torque level below which a longer zero current time is required
CREATE_PARM(OpnPhsTm2Mp,        float);     // Open delta time 2 for medium power level
CREATE_PARM(OpnPhsTm2Lp,        float);     // Open delta time 2 for intermediate power level
CREATE_PARM(OpnPhsTm2Lp0,       float);     // Open delta time 2 for lowest power level
CREATE_PARM(OpnPhsTm1Asy,       float);     // Open delta time 1 when asymmetric ride-through in progress
CREATE_PARM(OpnPhsTm2Asy,       float);     // Open delta time 2 when asymmetric ride-through in progress
CREATE_PARM(IOpn1AbsNWW,        float);     // RPNW  open phase current phase current omega for individual phase currents
CREATE_PARM(IOpn1Thr2NW,        float);     // RPNW  open phase current phase current omega for individual phase currents
CREATE_PARM(IOpnSThr2NW,        float);     // RPNW open phase current phase current omega for summed absoulute currents
CREATE_PARM(OpnPhsNWTm,         float);     // RPNW open phase time
CREATE_PARM(OpnVdcOffThr,       float);     // Open grid action bit off threshold
CREATE_PARM(OpnGrdTm,           float);     // Open grid time             0
CREATE_PARM(OpnGrdTm1,          float);     // Open grid time low power   1
CREATE_PARM(OpnGrdTm2,          float);     // Open grid time low power   2
CREATE_PARM(OpnGrdTm3,          float);     // Open grid time low power   3
CREATE_PARM(OpnGrdTmRPNW,       float);     // RPNW Open grid time
CREATE_PARM(OpnGRPNWIThr,       float);     // RPNW open grid enable treshold
CREATE_PARM(OpnGrdPwrMin,       float);     // Minimum power for open grid
CREATE_PARM(OpnGrdPwrTh2,       float);     // Open grid power threshold 2
CREATE_PARM(OpnGrdPwrLv3,       float);     // Open grid power level 2
CREATE_PARM(OpnGrdPwrLv2,       float);     // Open grid power level 2
CREATE_PARM(OpnGrdPwrLv1,       float);     // Open grid power level 1
CREATE_PARM(PllErrW,            float);     // Phase lock loop omega
CREATE_PARM(PllErrThr,          float);     // Phase lock loop fault fast threshold
CREATE_PARM(PhsJmpTrpEnb,       unsigned);  // Phase jump trip enable
CREATE_PARM(GmLVMtchErrW,       float);     // Grid monitor-line voltage error filter response
CREATE_PARM(LSVMtchErrW,        float);     // Line-stator voltage error filter response
// DistortionFilterTocDetection
CREATE_PARM(DFTocTau,           float);     // Distortion filter toc tau
CREATE_PARM(DFVllFilW,          float);     // Distortion filter line to line voltage filter omega
CREATE_PARM(DFTocAlmOff,        float);     // Distortion filter Timed over-current alarm "off" threshold
CREATE_PARM(DFTocAlmOn,         float);     // Distortion filter Timed over-current alarm "on" threshold
CREATE_PARM(DFTocThrs,          float);     // Distortion filter Timed over-current threshold
// CurrentErrorDetectionT3()
CREATE_PARM(GmSLIErrTau,     float); // Current error tau
CREATE_PARM(GmSLIErrThr,     float); // Current error threshold
// Torsional detection/protection parms
CREATE_PARM(R_TorSpdLpW,        float);     // Torsional detection lo-pass filter omega
CREATE_PARM(R_TorSpdScl,        float);     // Torsional detection magnitude scale
CREATE_PARM(TorTstSlwRat,       float);     // Torsional test slew rate in hz/sec/sec
CREATE_PARM(R_TorTstW1,         float);     // Torsional detection test omega 1
CREATE_PARM(R_TorTstW2,         float);     // Torsional detection test omega 2
CREATE_PARM(R_TorTrqTst,        float);     // Torsional torque for torque mode
CREATE_PARM(R_TorSimMode,       unsigned);  // Torsional test mode for sim mode
CREATE_PARM(R_TorTrqSpGn,       float);     // Torsional speed-torque gain
CREATE_PARM(R_TorSpRstW,        float);     // Torsional speed-reset W
CREATE_PARM(R_TorSpRIntW,       float);     // Torsional speed-reset w for integrator
CREATE_PARM(R_TorTrqScl,        float);     // Torsional speed speed to torque scale
CREATE_PARM(R_TorFrqRef,        float);     // Torsional speed torque frequency refrence...frequency where filter has gain = 1
CREATE_PARM(R_TorTstAmpl,       float);     // Torsional detection test oscillation amplitude
CREATE_PARM(R_TorTrqThrs,       float);     // Torsional torque protection threshold   (rpm);
CREATE_PARM(R_TorTrqFact,       float);     // Torsional torque detection/protection factor
CREATE_PARM(R_TorTrqFilW,       float);     // Torsional torque filter response (rad/sec);
CREATE_PARM(R_TorReset,         float);     // Torsional speed integrator reset rate  (deg/sec);
CREATE_PARM(R_TorSimEnb,        unsigned);  // Torsional speed sim mode test enable
CREATE_PARM(R_TorTrpThrs,       float);     // Torsional torque trip threshold
CREATE_PARM(R_TorAlmOn,         float);     // Torsional speed slip alarm on threshold
CREATE_PARM(R_TorAlmOff,        float);     // Torsional speed slip alarm off threshold
CREATE_PARM(TorSpdHpFilA0,      float);     // Torsional speed high pass filter A0 coefficient
CREATE_PARM(TorSpdHpFilA1,      float);     // Torsional speed high pass filter A1 coefficient
CREATE_PARM(TorSpdHpFilA2,      float);     // Torsional speed high pass filter A2 coefficient
CREATE_PARM(TorSpdHpFilB1,      float);     // Torsional speed high pass filter B1 coefficient
CREATE_PARM(TorSpdHpFilB2,      float);     // Torsional speed high pass filter B2 coefficient
CREATE_PARM(TorSpdLpFilA0,      float);     // Torsional speed low pass filter A0 coefficient
CREATE_PARM(TorSpdLpFilA1,      float);     // Torsional speed low pass filter A1 coefficient
CREATE_PARM(TorSpdLpFilA2,      float);     // Torsional speed low pass filter A2 coefficient
CREATE_PARM(TorSpdLpFilB1,      float);     // Torsional speed low pass filter B1 coefficient
CREATE_PARM(TorSpdLpFilB2,      float);     // Torsional speed low pass filter B2 coefficient
CREATE_PARM(TorPTrpEnb,         unsigned);  // Torsional protection trip enable -- user value
CREATE_PARM(TorProtType,        unsigned);  // Torsional protection type  -- user value
CREATE_PARM(TorPTrpEnbFv,       unsigned);  // Torsional protection trip enable -- final value
CREATE_PARM(TorPAlmEnb,         unsigned);  // Torsional protection alarm enable -- user value
CREATE_PARM(TorPAlmEnbFv,       unsigned);  // Torsional protection alarm enable -- final value


// Diagnostics
//------------
CREATE_DIAG(LnStrVmatchTrp);            // Line to stator grid monitor voltage mismatch trip
CREATE_DIAG(GmLnVanMatchTrp);           // Grid monitor to line phsA voltage mismatch trip
CREATE_DIAG(GmLnVbnMatchTrp);           // Grid monitor to line phsB voltage mismatch trip
CREATE_DIAG(GmLnVcnMatchTrp);           // Grid monitor to line phsC voltage mismatch trip
CREATE_DIAG(GmStrVanMatchTrp);          // Grid monitor to sator phsA voltage mismatch trip
CREATE_DIAG(GmStrVbnMatchTrp);          // Grid monitor to sator phsB voltage mismatch trip
CREATE_DIAG(GmStrVcnMatchTrp);          // Grid monitor to sator phsC voltage mismatch trip
CREATE_DIAG(IslandAlm);                 // Islanding alarm
CREATE_DIAG(IslandFlt);                 // Islanding trip
CREATE_DIAG(FastDistFlt);               // Fast disconnect trip
CREATE_DIAG(FastBlockFlt);              // Fast blocking trip

CREATE_DIAG(GmSLIaErrFlt);              // Current error fault, phase a
CREATE_DIAG(GmSLIbErrFlt);              // Current error fault, phase b
CREATE_DIAG(GmSLIcErrFlt);              // Current error fault, phase c

CREATE_DIAG(OpnPhsADiscAlm);          // Q1 Opn req. while converter running for open phase A, secondary
CREATE_DIAG(OpnPhsBDiscAlm);          // Q1 Opn req. while converter running for open phase B, secondary
CREATE_DIAG(OpnPhsCDiscAlm);          // Q1 Opn req. while converter running for open phase C, secondary
CREATE_DIAG(OpnPhsA2DiscAlm);         // Q1 Opn req. while converter running for open phase A, primary
CREATE_DIAG(OpnPhsB2DiscAlm);         // Q1 Opn req. while converter running for open phase B, primary
CREATE_DIAG(OpnPhsC2DiscAlm);         // Q1 Opn req. while converter running for open phase C, primary
CREATE_DIAG(FastDistDiscAlm);         // Q1 Opn req. while converter running for fast disconnect
CREATE_DIAG(FastBlockDiscAlm);        // Q1 Opn req. while converter running for fast block
CREATE_DIAG(PllPhaseDiscAlm);         // Q1 Opn req. while converter running for pll phase
CREATE_DIAG(OpnGrdDiscAlm);           // Q1 Opn req. while converter running for open grid
CREATE_DIAG(R_TorTrqDiscAlm);         // Q1 Opn req. while converter running for torsional torque error
CREATE_DIAG(IslandDiscAlm);           // Q1 Opn req. while converter running for islanding

CREATE_DIAG(OpnPhsAFlt1);           // Open phase A alarm 1
CREATE_DIAG(OpnPhsBFlt1);           // Open phase B alarm 1
CREATE_DIAG(OpnPhsCFlt1);           // OpenOpen phase C alarm 1
CREATE_DIAG(OpnPhsAFlt2);           // OpenOpen phase A alarm 2
CREATE_DIAG(OpnPhsBFlt2);           // OpenOpen phase B alarm 2
CREATE_DIAG(OpnPhsCFlt2);           // OpenOpen phase C alarm 2
CREATE_DIAG(OpnPhsANFlt2);          // Open phase A alarm 2, RPNW
CREATE_DIAG(OpnPhsBNFlt2);          // Open phase B alarm 2 RPNW
CREATE_DIAG(OpnPhsCNFlt2);          // Open phase C alarm 2 RPNW
CREATE_DIAG(OpnGrdFlt);             // Open grid fault
CREATE_DIAG(PllPhaseFlt);           // Pll phase fault
CREATE_DIAG(DFTocAlm);              // Distortion filter timed over-current alarm
CREATE_DIAG(DFTocFlt);              // Distortion filter timed over-current Fault
CREATE_DIAG(R_TorTrqFlt);           // Torsional torque trip fault
CREATE_DIAG(R_TorTrqAlm);           // Torsional torque alarm

CREATE_DIAG(GmGndFlt);              // Grid monitor ground fault trip
CREATE_DIAG(GmGndAlm);              // Grid monitor ground alarm

// Variables
//----------

// OpenPhaseCheckT2()
CREATE_PUBVAR(OpnPhsSat,        unsigned);  // Grid monitor current saturation detector
CREATE_PUBVAR(OpnPhsSatCtr,     int);       // Open phase saturation counter
CREATE_PUBVAR(IGridAbsMin1,     float);     // Open phase saturation min of abs values

CREATE_PUBVAR(OpnPhsALgTmr1,    float);     // Open phase detection long timer for secondary side phase A
CREATE_PUBVAR(OpnPhsARstTmr1,   float);     // Open phase detection reset timer for secondary side phase A
CREATE_PUBVAR(OpnPhsACnt1,      int);       // Open phase detection counter for secondary side phase A
CREATE_PUBVAR(OpnPhsADet1Z,     unsigned);  // Open phase detection flag Z for secondary side phase A
CREATE_PUBVAR(OpnPhsADet1,      unsigned);  // Open phase detection flat for secondary side phase A
CREATE_PUBVAR(OpnPhsALgTmr2,    float);     // Open phase detection long timer for primary side phase A
CREATE_PUBVAR(OpnPhsARstTmr2,   float);     // Open phase detection reset timer for primary side phase A
CREATE_PUBVAR(OpnPhsACnt2,      int);       // Open phase detection counter for primary side phase A
CREATE_PUBVAR(OpnPhsADet2Z,     unsigned);  // Open phase detection flag Z for primary side phase A
CREATE_PUBVAR(OpnPhsADet2,      unsigned);  // Open phase detection flat for primary side phase A
CREATE_PUBVAR(OpnPhsBLgTmr1,    float);     // Open phase detection long timer for secondary side phase B
CREATE_PUBVAR(OpnPhsBRstTmr1,   float);     // Open phase detection reset timer for secondary side phase B
CREATE_PUBVAR(OpnPhsBCnt1,      int);       // Open phase detection counter for secondary side phase B
CREATE_PUBVAR(OpnPhsBDet1Z,     unsigned);  // Open phase detection flag Z for secondary side phase B
CREATE_PUBVAR(OpnPhsBDet1,      unsigned);  // Open phase detection flat for secondary side phase B
CREATE_PUBVAR(OpnPhsBLgTmr2,    float);     // Open phase detection long timer for primary side phase B
CREATE_PUBVAR(OpnPhsBRstTmr2,   float);     // Open phase detection reset timer for primary side phase B
CREATE_PUBVAR(OpnPhsBCnt2,      int);       // Open phase detection counter for primary side phase B
CREATE_PUBVAR(OpnPhsBDet2Z,     unsigned);  // Open phase detection flag Z for primary side phase B
CREATE_PUBVAR(OpnPhsBDet2,      unsigned);  // Open phase detection flat for primary side phase B
CREATE_PUBVAR(OpnPhsCLgTmr1,    float);     // Open phase detection long timer for secondary side phase C
CREATE_PUBVAR(OpnPhsCRstTmr1,   float);     // Open phase detection reset timer for secondary side phase C
CREATE_PUBVAR(OpnPhsCCnt1,      int);       // Open phase detection counter for secondary side phase C
CREATE_PUBVAR(OpnPhsCDet1Z,     unsigned);  // Open phase detection flag Z for secondary side phase C
CREATE_PUBVAR(OpnPhsCDet1,      unsigned);  // Open phase detection flat for secondary side phase C
CREATE_PUBVAR(OpnPhsCLgTmr2,    float);     // Open phase detection long timer for primary side phase C
CREATE_PUBVAR(OpnPhsCRstTmr2,   float);     // Open phase detection reset timer for primary side phase C
CREATE_PUBVAR(OpnPhsCCnt2,      int);       // Open phase detection counter for primary side phase C
CREATE_PUBVAR(OpnPhsCDet2Z,     unsigned);  // Open phase detection flag Z for primary side phase C
CREATE_PUBVAR(OpnPhsCDet2,      unsigned);  // Open phase detection flat for primary side phase C
CREATE_PUBVAR(IaGrid1,         float);             //Calculated grid ia 1 current
CREATE_PUBVAR(IaGrid2,         float);             //Calculated grid ia 2 current
CREATE_PUBVAR(IbGrid1,         float);             //Calculated grid ib 1 current
CREATE_PUBVAR(IbGrid2,         float);             //Calculated grid ib 2 current
CREATE_PUBVAR(IcGrid1,         float);             //Calculated grid ic 1 current
CREATE_PUBVAR(IcGrid2,         float);             //Calculated grid ic 3 current
CREATE_PUBVAR(IaGridAbs1,      float);             //Grid ia 1 absolute value current
CREATE_PUBVAR(IaGridAbs2,      float);             //Grid ia 2 absolute value current
CREATE_PUBVAR(IbGridAbs1,      float);             //Grid ib 1 absolute value current
CREATE_PUBVAR(IbGridAbs2,      float);             //Grid ib 2 absolute value current
CREATE_PUBVAR(IcGridAbs1,      float);             //Grid ic 1 absolute value current
CREATE_PUBVAR(IcGridAbs2,      float);             //Grid ic 2 absolute value current
CREATE_PUBVAR(IaGridAbs2Fl,    float);             //Filtered value of grid ia 2 absolute value current
CREATE_PUBVAR(IbGridAbs2Fl,    float);             //Filtered value of grid ib 2 absolute value current
CREATE_PUBVAR(IcGridAbs2Fl,    float);             //Filtered value of grid ic 2 absolute value current
CREATE_PUBVAR(IGridAbs22Fl,    float);             //Filtered value of grid ic 2 absolute value current
CREATE_PUBVAR(OpnPhsANWTmr,    float);             //RPNW open phase A timer
CREATE_PUBVAR(OpnPhsBNWTmr,    float);             //RPNW open phase B timer
CREATE_PUBVAR(OpnPhsCNWTmr,    float);             //RPNW open phase C timer
CREATE_PUBVAR(GridIAbcGn1,     float);             //Grid current 1 filter gain
CREATE_PUBVAR(GridIAbcGn2,     float);             //Grid current 2 filter gain
CREATE_PUBVAR(SinEFbkP30,      float);             // Sin of (task 1 electrical feedback + 30deg)
CREATE_PUBVAR(CosEFbkP30,      float);             // Sin of (task 1 electrical feedback + 30deg)
CREATE_PUBVAR(OpnGrdTmr,       float);             // Open grid timer
CREATE_PUBVAR(OpnGrdTmrRPN,    float);             // Open grid timer RPNW
CREATE_PUBVAR(OpnPhsATmr1,     float);             // Open phase A timer 1
CREATE_PUBVAR(OpnPhsATmr2,     float);             // Open phase A timer 2
CREATE_PUBVAR(OpnPhsBTmr1,     float);             // Open phase B timer 1
CREATE_PUBVAR(OpnPhsBTmr2,     float);             // Open phase B timer 2
CREATE_PUBVAR(OpnPhsCTmr1,     float);             // Open phase C timer 1
CREATE_PUBVAR(OpnPhsCTmr2,     float);             // Open phase C timer 2
CREATE_PUBVAR(PllErrFil,       float);             // Filtered pll error

//Grid monitor ground fault protection
CREATE_PUBVAR(GmGndFltInh,     unsigned);   // Open phase fast shutdown rotor converter when there is trip from line converter
CREATE_PUBVAR(GmGndFltInhTmr,     float);   // Grid monitor ground fault inhibit time.
CREATE_PUBVAR(GmIGndProtFil,      float);   // Task3 filtered ground fault current for grid monitor ground fault detection
CREATE_PUBVAR(GmGndAlmAct,     unsigned);   // Grid monitor ground alarm active



// OpenGridCheckT2()
CREATE_PUBVAR(GmSVmtchThr1,     float);
CREATE_PUBVAR(GmSVmtchThr2,     float);
CREATE_PUBVAR(GmSVmtchThr,      float);

// For open phase / open grid signals:
// "Delta" = "2" = xfmr primary"----- convert the 6K and 690 windings current to the primary
//  "Wye"  = "1" = xfmr secondary
//
// Note:  variables Ia(bc)GridWyeT1(2) are xfmr primary currents, which are actually delta!

CREATE_PUBVAR(Opn3PhsDelta, unsigned);             // Open 3 Phase Delta flag
CREATE_PUBVAR(Opn3PhsOR,    unsigned);             // Open 3 Phase Wye or Delta
CREATE_PUBVAR(Opn3PhsWye,   unsigned);             // Open 3 Phase Wye flag
CREATE_PUBVAR(OpnGrd1,      unsigned);             // Open grid 1 detected
CREATE_PUBVAR(OpnGrdAct,    unsigned);             // Open grid action
CREATE_PUBVAR(OpnGrdDet1Ph, unsigned);             // Open grid detect via 1 phase detection
CREATE_PUBVAR(OpnGrdDetect, unsigned);             // Open grid detected
CREATE_PUBVAR(OpnPhsAct,    unsigned);             // Open grid detected
CREATE_PUBVAR(OpnPhsAZrc1,  unsigned);             // Phase A zero current indication -wye method
CREATE_PUBVAR(OpnPhsAZrc2,  unsigned);             // Phase A zero current indication -delta method
CREATE_PUBVAR(OpnPhsBZrc1,  unsigned);             // Phase B zero current indication -wye method
CREATE_PUBVAR(OpnPhsBZrc2,  unsigned);             // Phase B zero current indication -delta method
CREATE_PUBVAR(OpnPhsCZrc1,  unsigned);             // Phase C zero current indication -wye method
CREATE_PUBVAR(OpnPhsCZrc2,  unsigned);             // Phase C zero current indication -delta method
CREATE_PUBVAR(OpnPhsDet1,   unsigned);             // Open grid detected
CREATE_PUBVAR(OpnPhsDet2,   unsigned);             // Open grid detected
CREATE_PUBVAR(OpnPhsEnable, unsigned);             // Open phase enable
CREATE_PUBVAR(GrdFltState,  unsigned)=GRIDFLT_NONE;// Grid fault det state machine
CREATE_PUBVAR(GridPwrDelta,    float);             // Grid power delta: difference between the commanded power and actual power (watts)

//Device voltage mis-match protection
CREATE_PUBVAR(GmLVabMtchEr,    float);             // Grid monitor-line Vab match error
CREATE_PUBVAR(GmLVbcMtchEr,    float);             // Grid monitor-line Vbc match error
CREATE_PUBVAR(GmLVcaMtchEr,    float);             // Grid monitor-line Vca match error
CREATE_PUBVAR(GmStrVanMtchEr,  float);             // Grid monitor-stator Van match error
CREATE_PUBVAR(GmStrVbnMtchEr,  float);             // Grid monitor-stator Vbn match error
CREATE_PUBVAR(GmStrVcnMtchEr,  float);             // Grid monitor-stator Vcn match error
CREATE_PUBVAR(GmStrVanMtchSq,  float);             // Grid monitor-stator squared Van match error
CREATE_PUBVAR(GmStrVbnMtchSq,  float);             // Grid monitor-stator squared Vbn match error
CREATE_PUBVAR(GmStrVcnMtchSq,  float);             // Grid monitor-stator squared Vcn match error
CREATE_PUBVAR(LSVabMtchEr,     float) = 0.0F;      // Line-sator Vab match error
CREATE_PUBVAR(LSVbcMtchEr,     float) = 0.0F;      // Line-sator Vbc match error
CREATE_PUBVAR(LSVcaMtchEr,     float) = 0.0F;      // Line-sator Vca match error
CREATE_PUBVAR(GmLnVabMtchSqFl, float);             // Grid monitor to Line voltage phsA-phsB error squared filtered
CREATE_PUBVAR(GmLnVbcMtchSqFl, float);             // Grid monitor to Line voltage phsB-phsC error squared filtered
CREATE_PUBVAR(GmLnVcaMtchSqFl, float);             // Grid monitor to Line voltage phsC-phsA error squared filtered
CREATE_PUBVAR(GmLnVmtchErFl,   float);             // Grid monitor to Line voltage average voltage error filtered
CREATE_PUBVAR(GmSVMtchErSq,    float);             // Grid monitor - Stator voltage error squared
CREATE_PUBVAR(LSVMtchErSq ,    float);             // Line-Stator voltage error squared
CREATE_PUBVAR(LSVMtchErFl,     float);             // Line-Stator voltage error filtered
CREATE_PUBVAR(DFIaEstT2,       float);             // Distortion estimated Ia current
CREATE_PUBVAR(DFIbEstT2,       float);             // Distortion estimated Ib current
CREATE_PUBVAR(DFIcEstT2,       float);             // Distortion estimated Ic current
CREATE_PUBVAR(FuseIaEstT2,     float);             // AC line fuse estimated Ia current
CREATE_PUBVAR(FuseIbEstT2,     float);             // AC line fuse estimated Ib current
CREATE_PUBVAR(FuseIcEstT2,     float);             // AC line fuse estimated Ic current
CREATE_PUBVAR(L_FuseIaSqr,     float);             // sqared current of line fuse phase A
CREATE_PUBVAR(L_FuseIbSqr,     float);             // sqared current of line fuse phase B
CREATE_PUBVAR(L_FuseIcSqr,     float);             // sqared current of line fuse phase C
CREATE_PUBVAR(GmISumAbsT21,    float);
CREATE_PUBVAR(GmISumAbsT22,    float);
CREATE_PUBVAR(GmSLIaErr,       float);             // Grid-mon, stator, line ia error
CREATE_PUBVAR(GmSLIaErrSq,     float);             // Grid-mon, stator, line ia Err squared
CREATE_PUBVAR(GmSLIbErr,       float);             // Grid-mon, stator, line ib error
CREATE_PUBVAR(GmSLIbErrSq,     float);             // Grid-mon, stator, line ib Err squared
CREATE_PUBVAR(GmSLIcErr,       float);             // Grid-mon, stator, line ic error
CREATE_PUBVAR(GmSLIcErrSq,     float);             // Grid-mon, stator, line ic Err squared
CREATE_PUBVAR(GridPwrExpct,    float);             // Grid power expected
CREATE_PUBVAR(GridPwrExpZ1,    float);             // Grid power expected Z1
CREATE_PUBVAR(GridFltFlg,   unsigned);             // Grid fault flag
CREATE_PUBVAR(GridFltLcFlg, unsigned)=false;
CREATE_PUBVAR(LnBrgOff1Dt,  unsigned)=false;       // Line bridge of 1 dt
CREATE_PUBVAR(RtBrgOff1Dt,  unsigned)=false;       // Rotor bridge of 1 dt
CREATE_PUBVAR(DFCapW,          float);             // Distortion cap omega
CREATE_PUBVAR(R_TorSpHpAvg,    float);             // torsional speed detection high pass average
CREATE_PUBVAR(R_TorSpdFil1,    float);             // torsional detection high pass filter output
CREATE_PUBVAR(R_TorSpdHpFl,    float);             // torsional detection high pass filter out
CREATE_PUBVAR(R_TorSpdHpZ1,    float);             // torsional detection high pass filter out old value
CREATE_PUBVAR(R_TorSpdLpFl,    float);             // torsional detection low pass filter out
CREATE_PUBVAR(R_TorSpdBpFl,    float);             // torsional detection bandpass filter out
CREATE_PUBVAR(R_TorSpdSqr,     float);             // torsional detection bandpass filter out - squared
CREATE_PUBVAR(R_TorTstW,       float);             // torsional detection test omega
CREATE_PUBVAR(R_TorTstAng,     float);             // torsional detection test angle
CREATE_PUBVAR(R_TorTstOut,     float);             // torsional detection test signal output
CREATE_PUBVAR(R_TorSpdLpGn,    float);
CREATE_PUBVAR(R_TorSpdBpIn,    float);
CREATE_PUBVAR(R_TorSpdSqrF,    float);
CREATE_PUBVAR(R_TorSpdAcel,    float);             // torsional speed acceleration rpm/sec
CREATE_PUBVAR(R_TorSpdRaw ,    float);             // torsional speed raw
CREATE_PUBVAR(R_TorTrqSpd ,    float);             // torsional torque speed
CREATE_PUBVAR(R_TorSpRstT2,    float);             // torsional speed reset, task 2
CREATE_PUBVAR(R_TorSpdHpA0,    float);             // High pass portion of torsional speed 2nd order filter 1 A0 gain
CREATE_PUBVAR(R_TorSpdHpA1,    float);             // High pass portion of torsional speed 2nd order filter 1 A1 gain
CREATE_PUBVAR(R_TorSpdHpA2,    float);             // High pass portion of torsional speed 2nd order filter 1 A2 gain
CREATE_PUBVAR(R_TorSpdHpB1,    float);             // High pass portion of torsional speed 2nd order filter 1 B1 gain
CREATE_PUBVAR(R_TorSpdHpB2,    float);             // High pass portion of torsional speed 2nd order filter 1 B2 gain
CREATE_PUBVAR(R_TorFilHpu1,    float);             // High pass portion of torsional speed 2nd order filter 1 input Z1 delay
CREATE_PUBVAR(R_TorFilHpu2,    float);             // High pass portion of torsional speed 2nd order filter 1 input Z2 delay
CREATE_PUBVAR(R_TorFilHpy1,    float);             // High pass portion of torsional speed 2nd order filter 1 output Z1 delay
CREATE_PUBVAR(R_TorFilHpy2,    float);             // High pass portion of torsional speed 2nd order filter 1 output Z2 delay
CREATE_PUBVAR(R_TorSpdLpA0,    float);             // Low pass portion of torsional speed 2nd order filter 1 A0 gain
CREATE_PUBVAR(R_TorSpdLpA1,    float);             // Low pass portion of torsional speed 2nd order filter 1 A1 gain
CREATE_PUBVAR(R_TorSpdLpA2,    float);             // Low pass portion of torsional speed 2nd order filter 1 A2 gain
CREATE_PUBVAR(R_TorSpdLpB1,    float);             // Low pass portion of torsional speed 2nd order filter 1 B1 gain
CREATE_PUBVAR(R_TorSpdLpB2,    float);             // Low pass portion of torsional speed 2nd order filter 1 B2 gain
CREATE_PUBVAR(R_TorFilLpu1,    float);             // Low pass portion of torsional speed 2nd order filter 1 input Z1 delay
CREATE_PUBVAR(R_TorFilLpu2,    float);             // Low pass portion of torsional speed 2nd order filter 1 input Z2 delay
CREATE_PUBVAR(R_TorFilLpy1,    float);             // Low pass portion of torsional speed 2nd order filter 1 output Z1 delay
CREATE_PUBVAR(R_TorFilLpy2,    float);             // Low pass portion of torsional speed 2nd order filter 1 output Z2 delay
CREATE_PUBVAR(RebootPlsTm,     float);             // Reboot pulse time
CREATE_PUBVAR(RebootTmr,       float);             // Reboot timer
CREATE_PUBVAR(FltHistEntry,      int);
CREATE_PUBVAR(FltHistLoops,      int);
CREATE_PUBVAR(FltHistSize,       int);
CREATE_PUBVAR(PLCRbtCmdZ1,  unsigned);             //Old value of PLC reboot cmd
CREATE_PUBVAR(PLCRebootCmd, unsigned);
CREATE_PUBVAR(PLCRebootEnb, unsigned);             // PLC reboot enable

//Distortion Filter Protection
CREATE_PUBVAR(DFWTocT3,       float);              // Distortion filter toc omega
CREATE_PUBVAR(DFCapWSq,       float);              // Distortion cap omega squared
CREATE_PUBVAR(DFIaSqrT3,      float);              // Distortion filter capacitor a-b current squared
CREATE_PUBVAR(DFIbSqrT3,      float);              // Distortion filter capacitor b-c current squared
CREATE_PUBVAR(DFIcSqrT3,      float);              // Distortion filter capacitor c-a current squared
CREATE_PUBVAR(DFIaRmsMag,     float);              // Distortion filter capacitor a-b  current magnitude squared
CREATE_PUBVAR(DFIbRmsMag,     float);              // Distortion filter capacitor b-c  current magnitude squared
CREATE_PUBVAR(DFIcRmsMag,     float);              // Distortion filter capacitor c-a  current magnitude squared
CREATE_PUBVAR(DFIaRms,        float);              // Distortion filter capacitor a-b  current magnitude
CREATE_PUBVAR(DFIbRms,        float);              // Distortion filter capacitor b-c  current magnitude
CREATE_PUBVAR(DFIcRms,        float);              // Distortion filter capacitor c-a  current magnitude
CREATE_PUBVAR(DFTocHiSel,     float);              // Distortion filter current mag high select
CREATE_PUBVAR(DFTocHiSelSq,   float);              // Distortion filter current mag high select squared

//Current Error Protection
CREATE_PUBVAR(GmSLIaErrFil,   float);              // Filtered, rms squarred current error, phase a
CREATE_PUBVAR(GmSLIbErrFil,   float);              // Filtered, rms squarred current error, phase b
CREATE_PUBVAR(GmSLIcErrFil,   float);              // Filtered, rms squarred current error, phase c
CREATE_PUBVAR(AuxLoadIaRms,   float);              // Aux load phase rms phase a
CREATE_PUBVAR(AuxLoadIbRms,   float);              // Aux load phase rms phase b
CREATE_PUBVAR(AuxLoadIcRms,   float);              // Aux load phase rms phase c
CREATE_PUBVAR(GmSLIErrThr,    float);              // Trip threshold for current error protection
CREATE_PUBVAR(GmSLIErrGn,     float);              // Filter gain for current error protection

// Legacy Cooling System Monitor variables and new coolant temp calculation
CREATE_PUBVAR(CoolPwrIn,      float);              // Total power put in the coolant (Watts)
CREATE_PUBVAR(L_BrgLssComp,   float);              // Line bridge total power loss compensated for Tj
CREATE_PUBVAR(L_BrgLssFil,    float);              // Line bridge total power loss filtered
CREATE_PUBVAR(R_BrgLssComp,  float);               // Rotor-1 bridge total power loss compensated for Tj
CREATE_PUBVAR(R_BrgLssFil,    float);              // Total rotor bridge power loss filtered (Watts)
CREATE_PUBVAR(AuxCnvLoss,     float);              // Converter auxiliary losses (Watts)
CREATE_PUBVAR(BpRiseHi,    unsigned)=false;        // Baseplate temperature rise is high
CREATE_PUBVAR(CoolntTempCalc, float);              // Calculated coolant temperature (degC)

// Torsional instablity detection
CREATE_PUBVAR(R_TorSpRstIn,   float);              // torsional speed reset input
CREATE_PUBVAR(R_TorSRstInt,   float);              // torsional speed reset integrator
CREATE_PUBVAR(R_TorSpdRst,    float);              // torsional speed reset, task
CREATE_PUBVAR(R_TorSRIntGn,   float);              // Gain for R_TorSpdIntW * DelTm3
CREATE_PUBVAR(R_TorSpdMag,    float);              // torsional detection speed magnitude
CREATE_PUBVAR(R_TorTrqMag,    float);              // torsional detection torque magnitude
CREATE_PUBVAR(R_TorSpdMagF,   float);              // torsional detection speed magnitude filtered
CREATE_PUBVAR(R_TorTrqErr,    float);              // torsional torque error
CREATE_PUBVAR(R_TorTrqInt,    float);              // torsional slip angle
CREATE_PUBVAR(R_TTrqErrDel,   float);              // torsional torque delta
CREATE_PUBVAR(R_TorTrqMgPk,   float);              // torsional torque peak magnitude peak since started
CREATE_PUBVAR(R_TorTrqFil,    float);              // torsional torque filtered
CREATE_PUBVAR(L_IaT2Z1,       float);              // L_IaT2 * Z^-1
CREATE_PUBVAR(L_IbT2Z1,       float);              // L_IbT2 * Z^-1
CREATE_PUBVAR(L_IcT2Z1,       float);              // L_IcT2 * Z^-1
CREATE_PUBVAR(GmPllVyFbkT2,   float);              // T2 version of GmPllVyFbk

CREATE_PUBVAR(R_TorsAlmAct,unsigned);              // Torsional alarm active
CREATE_PUBVAR(R_TorSlipAct,unsigned)=false;        // torsional slip fault active
CREATE_PUBVAR(DFTocAlmFlg, unsigned)=false;        // distortion filter TOC alarm flag
CREATE_PUBVAR(TorTstSlwRatDt, float);

// Transformer primary currents
CREATE_PUBVAR(IaGridDeltT2,   float);              // xfmr primary phase A current (Amp)
CREATE_PUBVAR(IbGridDeltT2,   float);              // xfmr primary phase B current (Amp)
CREATE_PUBVAR(IcGridDeltT2,   float);              // xfmr primary phase C current (Amp)

CREATE_PUBVAR(GridFltOneSht,unsigned);             // Grid fault one shot flag
CREATE_PUBVAR(GmSecOVFlg,   unsigned);             //High votlage trip flag for xfmr 690V Secdary fault
CREATE_PUBVAR(S_VMagPUT3,      float);

// Add per WindDFIGe v01.06.43E(2012.10.11);  initially all false...
CREATE_PUBVAR(IslandCBDsb,  unsigned)=false;       //Islanding detection crowbar disable
CREATE_PUBVAR(S_VMaxHiDet,  unsigned)=false;       //Vac peak detect voltage raises crowbar threshold
CREATE_PUBVAR(GridFltLatch, unsigned)=false;       //Grid fault latch flat
CREATE_PUBVAR(S_VMaxIyFast, unsigned)=false;       // Use fast reactive current command path
CREATE_PUBVAR(S_VMaxHVRT  , unsigned);             // HVRT scheme has declared that an HVRT event exists
CREATE_PUBVAR(S_VMaxHiEnb , unsigned);             // VMax hi enable detection
CREATE_PUBVAR(IslandDet   , unsigned);             // Islanding detection
CREATE_PUBVAR(S_VMaxFrqHi , unsigned);             // Islanding frequency high
CREATE_PUBVAR(HVACState   , unsigned)=HVAC_NORMAL; // HVAC state machine
CREATE_PUBVAR(HVACStAct   , unsigned);             // HVAC state machine not in "normal" state (active)
CREATE_PUBVAR(HVACHotPll  , unsigned);             // HVAC state machine calls for hot pll
CREATE_PUBVAR(S_VMaxUpDet , unsigned)=false;       // Grid voltage magnitude peak detector indicator for increasing voltage
CREATE_PUBVAR(S_VMxIslInh , unsigned)=false;       // True when islanding should be prohibited
CREATE_PUBVAR(GrdFltClrPrm, unsigned)=false;       // Grid fault clear permissive
CREATE_PUBVAR(IslMaOpnReq , unsigned);             // Request MA to open for islanding
CREATE_PUBVAR(IslK1OpnReq , unsigned);             // Request K1 to open for islanding
CREATE_PUBVAR(DisconWhlRun, unsigned);             // //High voltage AC trip flag

CREATE_PUBVAR(S_VMaxPwrGn ,    float);             // filter gain for grid power
CREATE_PUBVAR(S_VMgILmPkGn,    float);             // Gain for voltage magnitude peak detector
CREATE_PUBVAR(S_VMgILmUpGn,    float);             // Gain for voltage magnitude peak detector -- up direction
CREATE_PUBVAR(S_VMaxPuPeak,    float);             // Grid voltage magnitude peak detector for voltage clamping
CREATE_PUBVAR(S_VMaxPllFil,    float);             // Stator voltage max pll filtered
CREATE_PUBVAR(S_VMxPllFlSs,    float);             // Stator voltage max pll filtered snapshot at the time islanding is detected
CREATE_PUBVAR(S_VMaxHiTmr ,    float);             // Islanding detection
CREATE_PUBVAR(GmFrqNomWRcp,    float);             // Gm nominal frequency reciprocial 1/rad/sec
CREATE_PUBVAR(GmPllWPuT2  ,    float);             // Gm nominal frequency Pu task2
CREATE_PUBVAR(S_VMaxPwrFil,    float);             // Islanding power filter
CREATE_PUBVAR(S_PwrMaxIsTmr,   float);             // Timer that determines when  islanding is ruled out do to too much power

CREATE_PUBVAR(FastDisAct,   unsigned);             // Fast disconnect active
CREATE_PUBVAR(FastBlkAct,   unsigned);             // Fast blocking active
CREATE_PUBVAR(CCUConfig,    unsigned);             // Configuration of converter control
CREATE_PUBVAR(CCUFeature,   unsigned);             // Features in Converter Control
CREATE_PUBVAR(GridCodeCfg,  unsigned);
CREATE_PUBVAR(CCUFwProduct, unsigned);             // Converter Control firmware product
CREATE_PUBVAR(CCUFwVersion, unsigned);             // Converter Control firmware version
CREATE_PUBVAR(CCUHardware,  unsigned);             // Converter Hardware version.  1=A, 2=B, .... (from Software IPN)
CREATE_PUBVAR(PLCPwrNP,        float);             // Maximum turbine power from turbine control P4.1
CREATE_PUBVAR(CCUPwrMax,       float);             // Maximum turbine power from turbine control P4.1
CREATE_PUBVAR(AirDensity,      float);             // Air density from turbine control -- a dymnamic number

CREATE_PUBVAR(FastDState,   unsigned)=FASTD_NORMAL;// Fast Disconnect/Blocking state machine
CREATE_PUBVAR(FastDILimPu,     float);             // Fast Disconnect/Blocking decrement counter
CREATE_PUBVAR(FastDILim,       float);             // Fast Disconnect current command
CREATE_PUBVAR(PhsJmpFlag,   unsigned);             // Phase Jump Fault Flag
CREATE_PUBVAR(FastBlkReq,   unsigned);             // Fast Blocking Request
CREATE_PUBVAR(FastDILimDel,    float);             // PARM(FastDIRateLim) * DelTm2
CREATE_PUBVAR(FastDisReq,   unsigned);             // Fast disconnect request

// FaultChkWindT3()
CREATE_PUBVAR(S_VMaxFrqFrz, unsigned);             // Islanding detection frequency freeze
CREATE_PUBVAR(S_VMxFFOnTmr,    float);             // Islanding detection frquency freeze on timer
CREATE_PUBVAR(S_VMxFFOfTmr,    float);             // Islanding detection frquency freeze off timer
CREATE_PUBVAR(S_PwrCmdT3,      float);             // Power command task 3 for islanding detection
CREATE_PUBVAR(S_VHiPwrLTmr,    float);             // Voltage high power low timer for islanding
CREATE_PUBVAR(S_VHiPwrLow,  unsigned);             // Boolean indicator for ac voltage high and power low for a time

// HVACStateMachine()
CREATE_PUBVAR(HvacQ1OpnTmr,    float);  // HVAC Q1 wait timer
CREATE_PUBVAR(VMtchInh,  unsigned);  // Logic to inhibit Gm to stator, stator to line and Gm to line trips


// Non-Volatile
NVVAR_UINT(PLCResetFlg)               // PLC Watchdog flag
NVVAR_UINT(R_TorTripCtr)              // Torsional trip event counter
NVVAR_FLOAT(R_TorTrqPTrp)             // Torsional slipo max torque before trip


// Unpublished Variables
//----------------------
Flt_Summ_Typ    FltSummary;           // bit coded fault summary word
float           DfTocAlmOnDl;
float           DfTocAlmOffDl;


// Data Passing
//--------------
DATA_PASS(GmPllVyFbk,   GmPllVyFbkT2, T1_T2, FILTERED);
DATA_PASS(IaGridDeltT1, IaGridDeltT2, T1_T2, FILTERED);
DATA_PASS(IbGridDeltT1, IbGridDeltT2, T1_T2, FILTERED);
DATA_PASS(IcGridDeltT1, IcGridDeltT2, T1_T2, FILTERED);
DATA_PASS(S_VMagPU,     S_VMagPUT3,   T2_T3, FILTERED);
DATA_PASS(GmIGndMag,    GmIGndMagT3,  T2_T3, FILTERED);

DATA_PASS(R_TorSpdRst,  R_TorSpRstT2, T3_T2, UNFILTERED);

// Functions
//----------

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Filtering in support of torsional instablity detection.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void TorsionalInstabiltyCheckT2 (void )

{

    // High pass filter for torsioal speed
    R_TorSpdHpFl = R_TorSpdHpA0 * R_TorSpdBpIn
                   - R_TorSpdHpB1 * R_TorFilHpy1
                   + R_TorSpdHpA1 * R_TorFilHpu1
                   - R_TorSpdHpB2 * R_TorFilHpy2
                   + R_TorSpdHpA2 * R_TorFilHpu2;
    R_TorFilHpu2  = R_TorFilHpu1;
    R_TorFilHpu1  = R_TorSpdBpIn;
    R_TorFilHpy2  = R_TorFilHpy1;
    R_TorFilHpy1  = R_TorSpdHpFl;

    //Two stage rolling average filter of high pass (or average of last two samples)
    R_TorSpHpAvg  = 0.5 * (R_TorSpdHpFl + R_TorSpdHpZ1);
    R_TorSpdHpZ1  = R_TorSpdHpFl;

    // Low pass filter for torsioal speed
    R_TorSpdLpFl  = R_TorSpdLpA0 * R_TorSpHpAvg
                    - R_TorSpdLpB1 * R_TorFilLpy1
                    + R_TorSpdLpA1 * R_TorFilLpu1
                    - R_TorSpdLpB2 * R_TorFilLpy2
                    + R_TorSpdLpA2 * R_TorFilLpu2;
    R_TorFilLpu2  = R_TorFilLpu1;
    R_TorFilLpu1  = R_TorSpHpAvg;
    R_TorFilLpy2  = R_TorFilLpy1;
    R_TorFilLpy1  = R_TorSpdLpFl;

    R_TorSpdBpFl  = R_TorSpdLpFl;


    R_TorSpdSqr = R_TorSpdBpFl * R_TorSpdBpFl;

    R_TorSpdSqrF += R_TorSpdLpGn * (R_TorSpdSqr - R_TorSpdSqrF);

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      General Wind application fault checking.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void FaultChkWindT2(void)

{

    ///////////////////////////////////////////
    // Torsional detection / protection
    ///////////////////////////////////////////
    unsigned R_TorCheck;

    R_TorCheck   = true;

    if (SimMode)
    {
        if ( PARM(R_TorSimEnb) )
        {
            R_TorTstW += TorTstSlwRatDt;
            if ( R_TorTstW > PARM(R_TorTstW2) )
            {   // ramp test frequency by 1 rad/sec
                R_TorTstW = PARM(R_TorTstW1);                 // reset to start freq when end freq reached
            }                                                 // set W1=W2 for constant frequency
            R_TorTstAng += R_TorTstW * DelTm2;
            R_TorTstAng  = cVectorf::Modulo2Pi(R_TorTstAng);

            R_TorTstOut = PARM(R_TorTstAmpl) * sinf(R_TorTstAng); // use oscillator to enable or pulse test signal

            if ( PARM(R_TorSimMode) == TORSIMSPDMODE)
            {
                R_TorSpdRaw = R_SpdGen + R_TorTstOut;
            }
            else
            {
                R_TorSpdRaw = R_SpdGen;
            }
        }
        else
        {
            R_TorTstW    = PARM(R_TorTstW1);
            R_TorTstAng  = 0.0F;
            R_TorCheck   = false;
            R_TorTstOut  = 0.0F;
            R_TorSpdRaw  = R_SpdGen;
        }
    }
    else
    {
        R_TorSpdRaw = R_SpdGen;

    }


    if ((SysTmSec > 5) && R_TorCheck)
    {
        // R_TorSpRstT2 = T2BufCPtr->R_TorSpdRst;

        if ( (PARM(R_TorSimMode) == TORSIMSPDMODE) || !SimMode)
        {
            R_TorSpdAcel = R_TrqCalT2 * PARM(R_TorTrqSpGn);
        }
        else
        {
            R_TorSpdAcel = (PARM(R_TorTrqTst) + R_TorTstOut) * PARM(R_TorTrqSpGn);
        }

        R_TorTrqSpd += DelTm2*(-R_TorSpdAcel - R_TorSpRstT2);
        R_TorSpdBpIn = R_TorSpdRaw + R_TorTrqSpd;

        TorsionalInstabiltyCheckT2();
    }
    else //(Sim mode and test mode not enabled) OR (systime < 5seconds)
    {
        R_TorSpdFil1 = R_TorSpdBpIn;
        R_TorSpdHpFl = R_TorSpdBpIn;
        R_TorSpdSqrF = 0.0;
        R_TorSpdBpIn = 0.0;
        R_TorTrqSpd  = 0.0;
    }


    if ( DiagResetCmdT2 )
    {
        Q1OpnReq    = false;
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//   This function uses data-passed grid monitor squared current errors which
//   represent the squared difference of the squared error between grid monitor
//   current for each phase, and the converter calculation of the same current
//   including a model for distortion filter current. The expected difference
//   is the auxillary loading, which can be failry substantial, so this funcion
//   essentially performs a TOC on the three currents allowing them to exceed
//   the trip threshold for a time that is long enough to accomodate the
//   starting currents of auxillary loads.
//   If any threhold are exceeded, the corresponding fault for that phase will
//   be given.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void CurrentErrorDetectionT2(void)
{
    float L_Ia_Dly, L_Ib_Dly, L_Ic_Dly;

    if ( KdfStatus )
    {
        DFIaEstT2    = DFCapW*DFVaDvDtT2;
        DFIbEstT2    = DFCapW*DFVbDvDtT2;
        DFIcEstT2    = DFCapW*DFVcDvDtT2;
    }
    else
    {
        DFIaEstT2    = 0.0F;
        DFIbEstT2    = 0.0F;
        DFIcEstT2    = 0.0F;
    }

    // Delay line currents to time (& phase) align them with DF estimated currents
    L_Ia_Dly = L_IabcDFcomp0 * L_IaT2 + L_IabcDFcomp1 * L_IaT2Z1;
    L_Ib_Dly = L_IabcDFcomp0 * L_IbT2 + L_IabcDFcomp1 * L_IbT2Z1;
    L_Ic_Dly = L_IabcDFcomp0 * L_IcT2 + L_IabcDFcomp1 * L_IcT2Z1;

    L_IaT2Z1 = L_IaT2;
    L_IbT2Z1 = L_IbT2;
    L_IcT2Z1 = L_IcT2;

    FuseIaEstT2  = L_Ia_Dly - DFIaEstT2;
    GmSLIaErr    = GmIaT2 - (S_IaT2 + FuseIaEstT2);
    GmSLIaErrSq  = GmSLIaErr*GmSLIaErr;

    FuseIbEstT2  = L_Ib_Dly - DFIbEstT2;
    GmSLIbErr    = GmIbT2 - (S_IbT2 + FuseIbEstT2);
    GmSLIbErrSq  = GmSLIbErr*GmSLIbErr;

    FuseIcEstT2  = L_Ic_Dly - DFIcEstT2;
    GmSLIcErr    = GmIcT2 - (S_IcT2 + FuseIcEstT2);
    GmSLIcErrSq  = GmSLIcErr*GmSLIcErr;

    // correct for T2 magnitude reduction and calculate the squared current of the line fuse (for TOC function)
    L_FuseIaSqr = (FuseIaEstT2*T1T2FbkScale) * (FuseIaEstT2*T1T2FbkScale);
    L_FuseIbSqr = (FuseIbEstT2*T1T2FbkScale) * (FuseIbEstT2*T1T2FbkScale);
    L_FuseIcSqr = (FuseIcEstT2*T1T2FbkScale) * (FuseIcEstT2*T1T2FbkScale);

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//   This function performs a check for open grid phases. There are two types of
//   checks performed, one for open phase at the secondary of the transformer
//   and another at the primary of the transformer.  In either case the method
//   of detection is similary, but the currents used are different. The
//   secondary or "wye" detection uses grid monitor task2 currentes directly,
//   and the primary or "delta" detection uses grid monitor task1 currents that
//   are phase shifted by 30deg and then data passed to task2.
//   In either case and for each phase, if the absolute value of a task2 current
//   is below the theshold, P.OpnPhsThr1 or P.OpnPhsThr2(for "delta" detection),
//   for a time greater than OpnPhsTmDyn1 or OpnPhsTmDyn2; a check is made for
//   the magnitude of another variable, V.GmISumAbsT21, or V.GmISumAbsT22. These
//   to variables are the sum of the absolute value of the currents for the
//   delta or wye detection. If these variables are higher than P.OpnPhsUThr1 or
//    P.OpnPhsUThr2, then the appropriate fault is given, and the flag,
//   V.GridFltLcFlg is set, which is used by another function to set a global
//   flag which shuts down the gating of both converters in the next task 1.
//   Note that a considerable amount of effort has been expended in an attempt
//   to find a more graceful way to shut down the converters once these faults
//   have been detected. At power ratings below 0.5pu and greater, a crowbar
//   fault will almost always occur once the line conveter shuts down, and a
//   huge amount of current will flow in from the grid until the breaker opens
//   (as a result of the crobar fault). An attempt to shut down only the rotor
//   converter and then shut down the line converter at a delayed time but
//   before the breaker opens, proved beneficial at power levels below 0.4pu,
//   although it was not beneficial at greater power levels because grid fast
//   over-voltage conditions usually occurred subjecting the system to stressful
//   voltage transients.
//   The fault pushing part of this function does not run in simulation mode.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void OpenPhaseCheckT2(void)
{
    ///////////////////////////////////////////////////
    // Note: GridFltLcFlg is cleared here, and then
    // set in any protection that needs it either in
    // this furnciton or the function, OpenGridCheckT2.
    // For this reason, the open phase function must
    // be called first.
    ///////////////////////////////////////////////////
    GridFltLcFlg = false;


    /////////////////////////////////////////////////////////
    //
    //  Check for Phase Loss using 0deg shift (no. 1 fault)
    //
    /////////////////////////////////////////////////////////

    OpnGrdDet1Ph = false;

    IaGrid1      = GmIaT2;
    IbGrid1      = GmIbT2;
    IcGrid1      = GmIcT2;

    //Absolute the currents
    IaGridAbs1 = ABS(IaGrid1);
    IbGridAbs1 = ABS(IbGrid1);
    IcGridAbs1 = ABS(IcGrid1);

    GmISumAbsT21  = 0.5*(IaGridAbs1 + IbGridAbs1 + IcGridAbs1);

    // xmfr primary current
    IaGrid2    = IaGridDeltT2;
    IbGrid2    = IbGridDeltT2;
    IcGrid2    = IcGridDeltT2;

    //Absolute the currents
    IaGridAbs2 = ABS(IaGrid2);
    IbGridAbs2 = ABS(IbGrid2);
    IcGridAbs2 = ABS(IcGrid2);

    GmISumAbsT22  = 0.5F*(IaGridAbs2 + IbGridAbs2 + IcGridAbs2);

    IGridAbsMin1 = MIN( (MIN(IaGridAbs1, IbGridAbs1)), IcGridAbs1);

    OpnPhsSatCtr = 0.0;
    if ( IaGridAbs1 > PARM(OpnPhSatThr1))
    {
        OpnPhsSatCtr = OpnPhsSatCtr + 1;
    }
    if ( IbGridAbs1 > PARM(OpnPhSatThr1))
    {
        OpnPhsSatCtr = OpnPhsSatCtr + 1;
    }
    if ( IcGridAbs1 > PARM(OpnPhSatThr1))
    {
        OpnPhsSatCtr = OpnPhsSatCtr + 1;
    }

    if ( (OpnPhsSatCtr >= PARM(OpnPhSatCnt)) && (IGridAbsMin1 >PARM(OpnPhSatThr2)))
    {
        OpnPhsSat = TRUE;
    }
    else
    {
        OpnPhsSat = FALSE ;
    }

    OpnPhsEnable = OpnPhsExtEnb && !OpnPhsSat;

    if ( OpnPhsSatCtr >= PARM(GmGndFltSatCnt ) )
    {
        GmGndFltInhTmr = 0.0F;
        GmGndFltInh    = true;
    }
    else
    {
        GmGndFltInhTmr = GmGndFltInhTmr + DelTm2;
        if ( GmGndFltInhTmr > PARM(GmGndFltInhTm) )
        {
             GmGndFltInhTmr = PARM(GmGndFltInhTm);
             GmGndFltInh    = false;
        }
    }



    if(TripFlt && PARM(OpnPhsRtrFstStp))
    {
        if(OpnPhsACnt1>0)
        {
            PUSH_DIAG(OpnPhsAFlt1);
            OpnPhsDet1  = true;
            OpnPhsAct   = true;
        }
        if(OpnPhsBCnt1>0)
        {
            PUSH_DIAG(OpnPhsBFlt1);
            OpnPhsDet1  = true;
            OpnPhsAct   = true;
        }
        if(OpnPhsCCnt1>0)
        {
            PUSH_DIAG(OpnPhsCFlt1);
            OpnPhsDet1  = true;
            OpnPhsAct   = true;
        }
        if(OpnPhsACnt2>0)
        {
            PUSH_DIAG(OpnPhsAFlt2);
            OpnPhsDet2  = true;
            OpnPhsAct   = true;
        }
        if(OpnPhsBCnt2>0)
        {
            PUSH_DIAG(OpnPhsBFlt2);
            OpnPhsDet2  = true;
            OpnPhsAct   = true;
        }
        if(OpnPhsCCnt2>0)
        {
            PUSH_DIAG(OpnPhsCFlt2);
            OpnPhsDet2  = true;
            OpnPhsAct   = true;
        }
    }

    if ( OpnPhsEnable )
    {
        {
            //Test for phase A
            OpnPhsADet1Z = OpnPhsADet1;
            if ( IaGridAbs1 < PARM(OpnPhsThr1))
            {
                OpnPhsAZrc1 = true;
                OpnPhsATmr1 = OpnPhsATmr1+DelTm2;
                if ( GmISumAbsT21 > PARM(OpnPhsUThr1) )
                {
                    if ( OpnPhsATmr1 > OpnPhsTmDyn1 )
                    {
                        OpnPhsATmr1 = OpnPhsTmDyn1;
                        OpnPhsARstTmr1 = 0.0F;
                        OpnPhsADet1 = true;
                    }
                }
            }
            else
            {
                OpnPhsATmr1 = 0.0F;
                OpnPhsAZrc1 = false;
                OpnPhsARstTmr1 += DelTm2;
                OpnPhsADet1 = false;
            }
            if(OpnPhsADet1 && !OpnPhsADet1Z)
            {
                OpnPhsACnt1 ++;
            }
            if(OpnPhsACnt1>=PARM(OpnPhsCntThr1))
            {
                GrdFltState  = GRIDFLT_1PHOPN_SEC6K_A;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsADiscAlm);
            }
            // longer timer
            if ( (IaGridAbs1 < PARM(OpnPhsThr1) || IaGridAbs1<PARM(OpnPhsIpuThrs)*GmISumAbsT21) && GmISumAbsT21 > PARM(OpnPhsUThr1) )
            {
                OpnPhsALgTmr1 += DelTm2*PARM(OpnPhsLTmrUpGn1);
                if(OpnPhsALgTmr1>PARM(OpnPhsLgTm1)) OpnPhsALgTmr1=PARM(OpnPhsLgTm1);
            }
            else
            {
                OpnPhsALgTmr1 -= DelTm2*PARM(OpnPhsLgTmrDecGn1);
                if(OpnPhsALgTmr1<0) OpnPhsALgTmr1=0;
            }
            if(OpnPhsALgTmr1>=PARM(OpnPhsLgTm1))
            {
                GrdFltState  = GRIDFLT_1PHOPN_SEC6K_A;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsADiscAlm);
            }
            // Reset logic
            if(OpnPhsARstTmr1>=PARM(OpnPhsRstTm1))
            {
                OpnPhsARstTmr1 = PARM(OpnPhsRstTm1);
                OpnPhsALgTmr1 = 0.0F;
                OpnPhsACnt1 = 0;
            }

            //Test for phase B
            OpnPhsBDet1Z = OpnPhsBDet1;
            if ( IbGridAbs1 < PARM(OpnPhsThr1) )
            {
                OpnPhsBZrc1 = true;
                OpnPhsBTmr1 = OpnPhsBTmr1+DelTm2;
                if ( GmISumAbsT21 > PARM(OpnPhsUThr1) )
                {
                    if ( OpnPhsBTmr1 > OpnPhsTmDyn1 )
                    {
                        OpnPhsBTmr1 = OpnPhsTmDyn1;
                        OpnPhsBRstTmr1 = 0.0F;
                        OpnPhsBDet1 = true;
                    }
                }
            }
            else
            {
                OpnPhsBTmr1 = 0.0F;
                OpnPhsBZrc1 = false;
                OpnPhsBRstTmr1 += DelTm2;
                OpnPhsBDet1 = false;
            }
            if(OpnPhsBDet1 && !OpnPhsBDet1Z)
            {
                OpnPhsBCnt1 ++;
            }
            if(OpnPhsBCnt1>=PARM(OpnPhsCntThr1))
            {
                GrdFltState  = GRIDFLT_1PHOPN_SEC6K_B;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsBDiscAlm);
            }
            // longer timer
            if ( (IbGridAbs1 < PARM(OpnPhsThr1) || IbGridAbs1<PARM(OpnPhsIpuThrs)*GmISumAbsT21) && GmISumAbsT21 > PARM(OpnPhsUThr1) )
            {
                OpnPhsBLgTmr1 += DelTm2*PARM(OpnPhsLTmrUpGn1);
                if(OpnPhsBLgTmr1>PARM(OpnPhsLgTm1)) OpnPhsBLgTmr1=PARM(OpnPhsLgTm1);
            }
            else
            {
                OpnPhsBLgTmr1 -= DelTm2*PARM(OpnPhsLgTmrDecGn1);
                if(OpnPhsBLgTmr1<0) OpnPhsBLgTmr1=0;
            }
            if(OpnPhsBLgTmr1>=PARM(OpnPhsLgTm1))
            {
                GrdFltState  = GRIDFLT_1PHOPN_SEC6K_B;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsBDiscAlm);
            }
            // Reset logic
            if(OpnPhsBRstTmr1>=PARM(OpnPhsRstTm1))
            {
                OpnPhsBRstTmr1 = PARM(OpnPhsRstTm1);
                OpnPhsBLgTmr1 = 0.0F;
                OpnPhsBCnt1 = 0;
            }

            //Test for phase C
            OpnPhsCDet1Z = OpnPhsCDet1;
            if ( IcGridAbs1 < PARM(OpnPhsThr1) )
            {
                OpnPhsCZrc1 = true;
                OpnPhsCTmr1 = OpnPhsCTmr1+DelTm2;
                if ( GmISumAbsT21 > PARM(OpnPhsUThr1) )
                {

                    if ( OpnPhsCTmr1 > OpnPhsTmDyn1 )
                    {
                        OpnPhsCTmr1 = OpnPhsTmDyn1;
                        OpnPhsCRstTmr1 = 0.0F;
                        OpnPhsCDet1 = true;
                    }
                }
            }
            else
            {
                OpnPhsCTmr1 = 0.0F;
                OpnPhsCZrc1 = false;
                OpnPhsCRstTmr1 += DelTm2;
                OpnPhsCDet1 = false;
            }
            if(OpnPhsCDet1 && !OpnPhsCDet1Z)
            {
                OpnPhsCCnt1 ++;
            }
            if(OpnPhsCCnt1>=PARM(OpnPhsCntThr1))
            {
                GrdFltState  = GRIDFLT_1PHOPN_SEC6K_C;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsCDiscAlm);
            }
            // longer timer
            if ( (IcGridAbs1 < PARM(OpnPhsThr1) || IcGridAbs1<PARM(OpnPhsIpuThrs)*GmISumAbsT21) && GmISumAbsT21 > PARM(OpnPhsUThr1) )
            {
                OpnPhsCLgTmr1 += DelTm2*PARM(OpnPhsLTmrUpGn1);
                if(OpnPhsCLgTmr1>PARM(OpnPhsLgTm1)) OpnPhsCLgTmr1=PARM(OpnPhsLgTm1);
            }
            else
            {
                OpnPhsCLgTmr1 -= DelTm2*PARM(OpnPhsLgTmrDecGn1);
                if(OpnPhsCLgTmr1<0) OpnPhsCLgTmr1=0;
            }
            if(OpnPhsCLgTmr1>=PARM(OpnPhsLgTm1))
            {
                GrdFltState  = GRIDFLT_1PHOPN_SEC6K_C;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsCDiscAlm);
            }
            // Reset logic
            if(OpnPhsCRstTmr1>=PARM(OpnPhsRstTm1))
            {
                OpnPhsCRstTmr1 = PARM(OpnPhsRstTm1);
                OpnPhsCLgTmr1 = 0.0F;
                OpnPhsCCnt1 = 0;
            }
        }

        /////////////////////////////////////////////////////////
        //
        //  Check for Phase Loss using +30deg shift (no. 2 fault)
        //
        /////////////////////////////////////////////////////////

        //------------------------------------------------------------------
        // check for the xmfr primary open phase fault
        //Test for phase A
        OpnPhsADet2Z = OpnPhsADet2;
        if ( IaGridAbs2 < PARM(OpnPhsThr2) )
        {
            OpnPhsAZrc2 = true;
            OpnPhsATmr2 = OpnPhsATmr2+DelTm2;
            if ( GmISumAbsT22 > PARM(OpnPhsUThr2) )
            {
                if ( OpnPhsATmr2 > OpnPhsTmDyn2 )
                {
                    OpnPhsATmr2 = OpnPhsTmDyn2;
                    OpnPhsARstTmr2 = 0.0F;
                    OpnPhsADet2 = true;
                }
            }
        }
        else
        {
            OpnPhsATmr2 = 0.0F;
            OpnPhsAZrc2 = false;
            OpnPhsARstTmr2 += DelTm2;
            OpnPhsADet2 = false;
        }
        if(OpnPhsADet2 && !OpnPhsADet2Z)
        {
            OpnPhsACnt2 ++;
        }
        if(OpnPhsACnt2>=PARM(OpnPhsCntThr2))
        {
            if ( !Opn3PhsOR)
            {
                GrdFltState  = GRIDFLT_1PHOPN_PRIM_A;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsA2DiscAlm);
            }
            else
            {
                OpnGrdDet1Ph = true;
            }
        }
        // longer timer
        if ( (IaGridAbs2 < PARM(OpnPhsThr2) || IaGridAbs2<PARM(OpnPhsIpuThrs)*GmISumAbsT22) && GmISumAbsT22 > PARM(OpnPhsUThr2) )
        {
            OpnPhsALgTmr2 += DelTm2*PARM(OpnPhsLTmrUpGn2);
            if(OpnPhsALgTmr2>PARM(OpnPhsLgTm2)) OpnPhsALgTmr2=PARM(OpnPhsLgTm2);
        }
        else
        {
            OpnPhsALgTmr2 -= DelTm2*PARM(OpnPhsLTmrDnGn2);
            if(OpnPhsALgTmr2<0) OpnPhsALgTmr2=0;
        }
        if(OpnPhsALgTmr2>=PARM(OpnPhsLgTm2))
        {
            if ( !Opn3PhsOR)
            {
                GrdFltState  = GRIDFLT_1PHOPN_PRIM_A;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsA2DiscAlm);
            }
            else
            {
                OpnGrdDet1Ph = true;
            }
        }
        // Reset logic
        if(OpnPhsARstTmr2>=PARM(OpnPhsRstTm2))
        {
            OpnPhsARstTmr2 = PARM(OpnPhsRstTm2);
            OpnPhsALgTmr2 = 0.0F;
            OpnPhsACnt2 = 0;
        }
        //Test for phase B
        OpnPhsBDet2Z = OpnPhsBDet2;
        if ( IbGridAbs2 < PARM(OpnPhsThr2) )
        {
            OpnPhsBZrc2 = true;
            OpnPhsBTmr2 = OpnPhsBTmr2+DelTm2;
            if ( GmISumAbsT22 > PARM(OpnPhsUThr2) )
            {
                if ( OpnPhsBTmr2 > OpnPhsTmDyn2 )
                {
                    OpnPhsBTmr2 = OpnPhsTmDyn2;

                    OpnPhsBRstTmr2 = 0.0F;
                    OpnPhsBDet2 = true;

                }
            }
        }
        else
        {
            OpnPhsBTmr2 = 0.0F;
            OpnPhsBZrc2 = false;
            OpnPhsBRstTmr2 += DelTm2;
            OpnPhsBDet2 = false;
        }
        if(OpnPhsBDet2 && !OpnPhsBDet2Z)
        {
            OpnPhsBCnt2 ++;
        }
        if(OpnPhsBCnt2>=PARM(OpnPhsCntThr2))
        {
            if ( !Opn3PhsOR)
            {
                GrdFltState  = GRIDFLT_1PHOPN_PRIM_B;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsB2DiscAlm);
            }
            else
            {
                OpnGrdDet1Ph = true;
            }
        }
        // longer timer
        if ( (IbGridAbs2 < PARM(OpnPhsThr2) || IbGridAbs2<PARM(OpnPhsIpuThrs)*GmISumAbsT22) && GmISumAbsT22 > PARM(OpnPhsUThr2) )
        {
            OpnPhsBLgTmr2 += DelTm2*PARM(OpnPhsLTmrUpGn2);
            if(OpnPhsBLgTmr2>PARM(OpnPhsLgTm2)) OpnPhsBLgTmr2=PARM(OpnPhsLgTm2);
        }
        else
        {
            OpnPhsBLgTmr2 -= DelTm2*PARM(OpnPhsLTmrDnGn2);
            if(OpnPhsBLgTmr2<0) OpnPhsBLgTmr2=0;
        }
        if(OpnPhsBLgTmr2>=PARM(OpnPhsLgTm2))
        {
            if ( !Opn3PhsOR)
            {
                GrdFltState  = GRIDFLT_1PHOPN_PRIM_B;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsB2DiscAlm);
            }
            else
            {
                OpnGrdDet1Ph = true;
            }
        }
        // Reset logic
        if(OpnPhsBRstTmr2>=PARM(OpnPhsRstTm2))
        {
            OpnPhsBRstTmr2 = PARM(OpnPhsRstTm2);
            OpnPhsBLgTmr2 = 0.0F;
            OpnPhsBCnt2 = 0;
        }
        //Test for phase C
        OpnPhsCDet2Z = OpnPhsCDet2;
        if ( IcGridAbs2 < PARM(OpnPhsThr2) )
        {
            OpnPhsCZrc2 = true;
            OpnPhsCTmr2 = OpnPhsCTmr2+DelTm2;
            if ( GmISumAbsT22 > PARM(OpnPhsUThr2) )
            {

                if ( OpnPhsCTmr2 > OpnPhsTmDyn2 )
                {
                    OpnPhsCTmr2 = OpnPhsTmDyn2;

                    OpnPhsCRstTmr2 = 0.0F;
                    OpnPhsCDet2 = true;

                }
            }
        }
        else
        {
            OpnPhsCTmr2 = 0.0F;
            OpnPhsCZrc2 = false;
            OpnPhsCRstTmr2 += DelTm2;
            OpnPhsCDet2 = false;
        }
        if(OpnPhsCDet2 && !OpnPhsCDet2Z)
        {
            OpnPhsCCnt2 ++;
        }
        if(OpnPhsCCnt2>=PARM(OpnPhsCntThr2))
        {
            if ( !Opn3PhsOR)
            {
                GrdFltState  = GRIDFLT_1PHOPN_PRIM_C;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsC2DiscAlm);
            }
            else
            {
                OpnGrdDet1Ph = true;
            }
        }
        // longer timer
        if ( (IcGridAbs2 < PARM(OpnPhsThr2) || IcGridAbs2<PARM(OpnPhsIpuThrs)*GmISumAbsT22) && GmISumAbsT22 > PARM(OpnPhsUThr2) )
        {
            OpnPhsCLgTmr2 += DelTm2*PARM(OpnPhsLTmrUpGn2);
            if(OpnPhsCLgTmr2>PARM(OpnPhsLgTm2)) OpnPhsCLgTmr2=PARM(OpnPhsLgTm2);
        }
        else
        {
            OpnPhsCLgTmr2 -= DelTm2*PARM(OpnPhsLTmrDnGn2);
            if(OpnPhsCLgTmr2<0) OpnPhsCLgTmr2=0;
        }
        if(OpnPhsCLgTmr2>=PARM(OpnPhsLgTm2))
        {
            if ( !Opn3PhsOR)
            {
                GrdFltState  = GRIDFLT_1PHOPN_PRIM_C;
                DisconWhlRun  = true;
                PUSH_DIAG(OpnPhsC2DiscAlm);
            }
            else
            {
                OpnGrdDet1Ph = true;
            }
        }
        // Reset logic
        if(OpnPhsCRstTmr2>=PARM(OpnPhsRstTm2))
        {
            OpnPhsCRstTmr2 = PARM(OpnPhsRstTm2);
            OpnPhsCLgTmr2 = 0.0F;
            OpnPhsCCnt2 = 0;
        }
        ///////////////////////////////////////////////////////////////////////////////
        // Open Phase Detection for PPNW mode
        ///////////////////////////////////////////////////////////////////////////////
        if ( RPNWRunning)
        {
            IaGridAbs2Fl += IOpn1AbsNWGn * ( IaGridAbs2 - IaGridAbs2Fl);
            IbGridAbs2Fl += IOpn1AbsNWGn * ( IbGridAbs2 - IbGridAbs2Fl);
            IcGridAbs2Fl += IOpn1AbsNWGn * ( IcGridAbs2 - IcGridAbs2Fl);
            IGridAbs22Fl += IOpn1AbsNWGn * ( GmISumAbsT22 - IGridAbs22Fl);

            if ( IGridAbs22Fl > PARM(IOpnSThr2NW) )
            {
                //Phase A
                if ( IaGridAbs2Fl < PARM(IOpn1Thr2NW) )
                {
                    OpnPhsANWTmr = OpnPhsANWTmr + DelTm2;
                    if ( OpnPhsANWTmr > PARM(OpnPhsNWTm) )
                    {
                        OpnPhsANWTmr = PARM(OpnPhsNWTm);
                        PUSH_DIAG(OpnPhsANFlt2);
                        OpnPhsDet2 = true;
                    }
                }
                else
                {
                    OpnPhsANWTmr = 0.0F;
                }

                //Phase B
                if ( IbGridAbs2Fl < PARM(IOpn1Thr2NW) )
                {
                    OpnPhsBNWTmr = OpnPhsBNWTmr + DelTm2;
                    if ( OpnPhsBNWTmr > PARM(OpnPhsNWTm) )
                    {
                        OpnPhsBNWTmr = PARM(OpnPhsNWTm);
                        PUSH_DIAG(OpnPhsBNFlt2);
                        OpnPhsDet2 = true;
                    }
                }
                else
                {
                    OpnPhsBNWTmr = 0.0F;
                }

                //Phase C
                if ( IcGridAbs2Fl < PARM(IOpn1Thr2NW) )
                {
                    OpnPhsCNWTmr = OpnPhsCNWTmr + DelTm2;
                    if ( OpnPhsCNWTmr > PARM(OpnPhsNWTm) )
                    {
                        OpnPhsCNWTmr = PARM(OpnPhsNWTm);
                        PUSH_DIAG(OpnPhsCNFlt2);
                        OpnPhsDet2 = true;
                    }
                }
                else
                {
                    OpnPhsCNWTmr = 0.0F;
                }
            }
            else
            {
                OpnPhsANWTmr = 0.0F;
                OpnPhsBNWTmr = 0.0F;
                OpnPhsCNWTmr = 0.0F;
            }

            if (OpnPhsDet2)
            {
                OpnPhsAct    = true;
                GridFltLcFlg = true;
                GridFltLatch = true;
            }

        }
        else
        {
            IaGridAbs2Fl = 0.0F;
            IbGridAbs2Fl = 0.0F;
            IcGridAbs2Fl = 0.0F;
            IGridAbs22Fl = 0.0F;
            OpnPhsANWTmr = 0.0F;
            OpnPhsBNWTmr = 0.0F;
            OpnPhsCNWTmr = 0.0F;
        }

    }
    else
    {
        OpnPhsATmr1  = 0.0F;
        OpnPhsBTmr1  = 0.0F;
        OpnPhsCTmr1  = 0.0F;
        OpnPhsDet1   = false;
        OpnPhsATmr2  = 0.0F;
        OpnPhsBTmr2  = 0.0F;
        OpnPhsCTmr2  = 0.0F;
        OpnPhsDet2   = false;
        OpnPhsAZrc1  = false;
        OpnPhsBZrc1  = false;
        OpnPhsCZrc1  = false;
        OpnPhsAZrc2  = false;
        OpnPhsBZrc2  = false;
        OpnPhsCZrc2  = false;
        OpnPhsANWTmr = 0.0F;
        OpnPhsBNWTmr = 0.0F;
        OpnPhsCNWTmr = 0.0F;
        OpnPhsACnt1  = 0.0F;
        OpnPhsBCnt1  = 0.0F;
        OpnPhsCCnt1  = 0.0F;
        OpnPhsACnt2  = 0.0F;
        OpnPhsBCnt2  = 0.0F;
        OpnPhsCCnt2  = 0.0F;
    }

    Opn3PhsWye   = OpnPhsAZrc1 && OpnPhsBZrc1 && OpnPhsCZrc1;
    Opn3PhsDelta = OpnPhsAZrc2 && OpnPhsBZrc2 && OpnPhsCZrc2;
    Opn3PhsOR = Opn3PhsWye || Opn3PhsDelta;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    This function executes a state machine for fast disconnect and fast
// blocking faults. The state machine ramps down the IxCmd0 and IyCmd currents.
// It sets the appropriate flags for either fast disconnect or blocking, and
// transfers control over to another state machine which commands the Q1 to open
// and pushes the appropriate fault.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void FastDisconnectBlockingT2(void)
{

    if ( PARM(FastBlkEnb) && (FastBlkReqHw || PARM(FastBlkTst)))
    {
        FastBlkReq = true;
    }
    else
    {
        FastBlkReq = false;
    }

    ///////////////////////////////////////////////////
    //                                               //
    //    Fast Disconnect/Blocking State Machine     //
    //                                               //
    ///////////////////////////////////////////////////
    switch (FastDState)
    {
        //==========================================================================
        //                         Normal(0)
        //==========================================================================
    case FASTD_NORMAL:
        FastDILimPu  = LvIyMaxDyn;

        if (!L_Running)
        {
            if (!Q1Status)
            {
                if ( FastDisReq )
                {
                    PUSH_DIAG(FastDistFlt);
                }
                else if (FastBlkReq)
                {
                    PUSH_DIAG(FastBlockFlt);
                }
            }
            else
            {
                if ( FastDisReq)
                {
                    FastDState = FASTD_RAMP_DOWN;
                    FastDisAct = true;
                }
                else if (FastBlkReq || PhsJmpFlag || R_TorSlipAct)
                {
                    FastDState = FASTD_RAMP_DOWN;
                    if (FastBlkReq )
                    {
                        FastBlkAct = true;
                    }
                }
            }
        }
        else if (FastDisReq)
        {
            FastDState = FASTD_RAMP_DOWN;
            FastDisAct = true;
        }
        else if ( FastBlkReq || PhsJmpFlag || R_TorSlipAct)
        {
            FastDState = FASTD_RAMP_DOWN;

            if (FastBlkReq )
            {
                FastBlkAct = true;
            }
        }
        break;

        //==========================================================================
        //             Real and Reactive Current Ramp Down; Q1 Open Req (1)
        //==========================================================================
    case FASTD_RAMP_DOWN:
        DisconWhlRun = true;
        if ( FastDisAct)
        {
            GrdFltState = GRIDFLT_FASTDIS;
            PUSH_DIAG(FastDistDiscAlm);
        }
        else if (FastBlkAct)
        {
            GrdFltState = GRIDFLT_FASTBLK;
            PUSH_DIAG(FastBlockDiscAlm);
        }

        FastDILimPu = FastDILimPu  - FastDILimDel;
        if ( FastDILimPu  < PARM(FastDVrgIyMx))
        {
            FastDILimPu  = PARM(FastDVrgIyMx);
            FastDState   = FASTD_Q1_OPEN_WAIT;
        }
        break;

        //==========================================================================
        //                  Q1 Open Wait (2)
        //==========================================================================
    case FASTD_Q1_OPEN_WAIT:
        if ( GridFltLcFlg)
            FastDState = FASTD_K1_OPEN_WAIT;

        break;

        //==========================================================================
        //                  K1 Open Wait (3)
        //==========================================================================
    case FASTD_K1_OPEN_WAIT:
        if ( !GridFltLatch)
        {
            FastDState = FASTD_NORMAL;
        }
        break;

    default:
        break;

    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//   This purpose of this function is to detect the condition of an open grid.
//   This condtion can be caused either by Mvsg opening or by an open further
//   upstream, such as a breaker at the substation.  Either condition is
//   potentially hazardous for the converter, causing the dc link to go above
//   1400volts. The following is a except from an early note about this
//   condition:
//
//   "Note that the result of this
//    condition is a stator voltage increasing in magnitude
//    to the point where the line converter input voltage
//    allows diode conduction, which drives the dc link
//    upward and eventually blows IGBTs.  This code trys
//    to prevent that from happening by shutting down
//    both conveters quickly, espectially the rotor con-
//    verter, and stopping the flow of excitation current
//    to the rotor, thus preventing the incresing stator
//    voltage."
//
//   There are three different protections here.
//   The newest of these protections does a three way comparison between the ac
//   voltage feedbacks of the grid monitor, line converter and stator. Line
//   conveter ac feedbacks must be L*Di/dt compensated before being compared
//   to stator and grid monitor feedbacks. Also, line conveter voltages and
//   other voltages compared with it must be delayed by 1/2 dt to properly
//   alight with the L*di/dt drop of the reactor. The two comparison involving
//   line converter voltages have low pass filters on the output to provide
//   attenuation of high frequency voltage spikes caused by impure L*Di/dt
//   compensation. These three protections are very fast and robust to catch
//   device opening events. The Grid monitor to stator comparison is unfiltered
//   but very accurate and sets the flag, GmSVMatchFlg, which is passed to the
//   last method, causing a fast shutdown via the FPGA.
//   The second protection is for RPNW mode.  It gives an open grid fault when
//   the line converter dc voltage and current regulators are both saturated in
//   the same direction, the line converter current is low, and the line voltae
//   is high. This protection is important, bus less critical than those
//   provided when the generator is on line.
//   The third protection was developed to solve the original issue with the
//   breaker opening. It is activated when power flow (L_CnvOutPwr-R_CnvOutPwr)
//   is less than some large negative threshold, a dc link voltage predictor
//   shows that the dc voltage will be above a threshold the next task 2, and
//   the grid voltage is above a theshold.   This protetive has proved to be
//   helpful, but also goes off during LVRT/ZVRT when it should not.  It may be
//   replaced soon, especiallyin light of the newest of the three and the first
//   mentioned above.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void OpenGridCheckT2(void)
{

    float    PhsAErrSq;
    float    PhsBErrSq;
    float    PhsCErrSq;
    float    AvgPhsVerrSq;
    unsigned OpenGrid;


    //-------------------------------------------------------------------------
    // AC voltage feedback comparison

    // Grid monitor (690V) to compensated Line bridge
    GmLVabMtchEr  = L_VabLineT2 - GmVabT2;
    PhsAErrSq     = GmLVabMtchEr*GmLVabMtchEr;

    GmLVbcMtchEr  = L_VbcLineT2 - GmVbcT2;
    PhsBErrSq     = GmLVbcMtchEr*GmLVbcMtchEr;

    GmLVcaMtchEr  = -GmLVabMtchEr - GmLVbcMtchEr;
    PhsCErrSq     = GmLVcaMtchEr*GmLVcaMtchEr;

    AvgPhsVerrSq = 0.3333333F * (PhsAErrSq + PhsBErrSq + PhsCErrSq);
    GmLnVmtchErFl = GmLnVmtchErFl + GmLVmtchErGn*(AvgPhsVerrSq - GmLnVmtchErFl);

    GmLnVabMtchSqFl = GmLnVabMtchSqFl + GmLVmtchErGn*(PhsAErrSq - GmLnVabMtchSqFl);
    GmLnVbcMtchSqFl = GmLnVbcMtchSqFl + GmLVmtchErGn*(PhsBErrSq - GmLnVbcMtchSqFl);
    GmLnVcaMtchSqFl = GmLnVcaMtchSqFl + GmLVmtchErGn*(PhsCErrSq - GmLnVcaMtchSqFl);

    VMtchInh = IslandDet || DisconWhlOv || L_DbThrmModel.DtmTr_GridDiscReq || L_DbThrmModel.DtmRes_GridDiscReq || DisconWhlRun;

    // check for the voltage mismatch bewteen the 690 GM and the line
    if ( L_Running && !IslandDet )
    {
        if ( (GmLnVabMtchSqFl > GmLVmtchThr) && (GmLnVcaMtchSqFl > GmLVmtchThr) && !VMtchInh)    // Phase A
        {
            PUSH_DIAG(GmLnVanMatchTrp);
        }

        if ( (GmLnVbcMtchSqFl > GmLVmtchThr) && (GmLnVabMtchSqFl > GmLVmtchThr) && !VMtchInh )    // Phase B
        {
            PUSH_DIAG(GmLnVbnMatchTrp);
        }

        if ( (GmLnVcaMtchSqFl > GmLVmtchThr) && (GmLnVbcMtchSqFl > GmLVmtchThr) && !VMtchInh )    // Phase C
        {
            PUSH_DIAG(GmLnVcnMatchTrp);
        }
    }

    // Compensated Line bridge to Stator
    LSVabMtchEr   = L_VabLineT2 - S_VabT2;
    PhsAErrSq     = LSVabMtchEr*LSVabMtchEr;

    LSVbcMtchEr   = L_VbcLineT2 - S_VbcT2;
    PhsBErrSq     = LSVbcMtchEr*LSVbcMtchEr;

    LSVcaMtchEr  = -LSVabMtchEr - LSVbcMtchEr;
    PhsCErrSq     = LSVcaMtchEr*LSVcaMtchEr;

    LSVMtchErSq  = 0.3333333F * (PhsAErrSq + PhsBErrSq + PhsCErrSq);
    LSVMtchErFl  = LSVMtchErFl + LSVmtchErGn*(LSVMtchErSq - LSVMtchErFl);

    if ( S_K1Status && S_K1ClsCmd )  // from code up above---S_K1Status is not pure,
    {   // so clause is conditioned to K1 command
        if ( LSVMtchErFl > LSVmtchThr && !VMtchInh)
        {
            PUSH_DIAG(LnStrVmatchTrp);
        }
    }
    // Grid monitor (690V or 6KV) to Stator
    GmStrVanMtchEr = S_VanT2 - GmVanT2;
    GmStrVanMtchSq = GmStrVanMtchEr * GmStrVanMtchEr;

    GmStrVbnMtchEr = S_VbnT2 - GmVbnT2;
    GmStrVbnMtchSq = GmStrVbnMtchEr * GmStrVbnMtchEr;

    GmStrVcnMtchEr = S_VcnT2 - GmVcnT2;
    GmStrVcnMtchSq = GmStrVcnMtchEr * GmStrVcnMtchEr;

    GmSVMtchErSq = 0.3333333F * (GmStrVanMtchSq + GmStrVbnMtchSq + GmStrVcnMtchSq);


    // Volt-sec grid monitor Over-Voltage Check (690 Gm)----square root voltage detection
// GmOVPuVSec = GmOVPuVSec + ( DelTm2 *(GmVMagPuT2 - GmCnvOVMinx) );
//
// if ( GmOVPuVSec >= GmCnvOVThrx )
// {
//     GmOVPuVSec   = GmCnvOVThrx;
//     GrdFltState  = GRIDFLT_VSECOV;
//     DisconWhlRun  = true;
// }
// else if (GmOVPuVSec < 0.0F)
// {
//     GmOVPuVSec = 0.0F;
// }



    //-------------------------------------------------------------------------
    //  Line Converter pll phase jump

    // Line converter Phase Error Filter
    PllErrFil = PllErrFil + DelTm2*PARM(PllErrW)*(GmPllVyFbkT2 - PllErrFil);
    if (  (PllErrFil < PARM(PllErrThr)) &&  L_BrgPwrEnb )
    {
        PhsJmpFlag   = true;
        GrdFltState  = GRIDFLT_PLLPHASE;
        DisconWhlRun = true;
        PUSH_DIAG(PllPhaseDiscAlm);
    }


    if ( LineRunOnly && !R_InnrRegEnb)
    {
        // RPNW Open Grid Detection

        if ( Opn3PhsOR )
        {
            OpnGrdTmrRPN  = OpnGrdTmrRPN + DelTm2;
            if ( (OpnGrdTmrRPN >= PARM(OpnGrdTmRPNW)) && !OpnGrdAct)
            {
                OpnGrdTmrRPN = PARM(OpnGrdTmRPNW);
                OpnGrdDetect = true;
                GridFltLcFlg = true;
            }
        }
        else
        {
            OpnGrdTmrRPN  = 0.0F;
            OpnGrdDetect = false; // This flag shuts down crobar for 1 task2
        }

        if ( (OpnGrdDetect || OpnGrdDet1Ph) && !OpnGrdAct )
        {
            OpnGrdDetect   = true; // This flag shuts down crobar for 1msec
            OpnGrdAct      = true; // Latch flag
            GridFltLcFlg   = true;
            // MvSwGearTrpReq = true;   OpnGrdAct => bypass drop-out delay on K1
            PUSH_DIAG(OpnGrdFlt);
        }

        OpnGrd1    =false;
        OpnGrdTmr  = 0.0F;
    }
    else    //open grid detection for both converters or neither running
    {
        OpnGrdTmrRPN  = 0.0F;

        GridPwrExpZ1  = GridPwrExpct;

        if (DisconWhlRun)
            GridPwrExpct = 0.0F;
        else
            GridPwrExpct = -R_TrqCmdInT2*R_SpdFbkT2;

        GridPwrDelta  = GridPwrExpZ1 - GmPwrT2;


        if ( R_Running)
        {
            OpnGrd1 =false;
            OpenGrid=false;

            if ( ((GridPwrExpZ1 > PARM(OpnGrdPwrMin)) && ( ABS(GmPwrT2) < PARM(OpnGrdPwrTh2))) && Opn3PhsOR )
            {
                OpnGrdTmr  = OpnGrdTmr + DelTm2;
                if ( OpnGrdTmr >= OpnGrdTmDyn )
                {
                    OpnGrdTmr = OpnGrdTmDyn;
                    OpenGrid  = true;
                }
            }
            else
            {
                OpnGrdTmr  = 0.0F;
            }

            if ( ( OpenGrid || OpnGrdDet1Ph )  && !(OpnGrdAct || DisconWhlRun) && !IslandDet )
            {
                GrdFltState    = GRIDFLT_3PHOPN;
                DisconWhlRun   = true;
                PUSH_DIAG(OpnGrdDiscAlm);
                // MvSwGearTrpReq = true;   (DisconWhlRun => IslK1OpnReq => bypass drop-out delay on K1)
            }
            else
            {
                OpnGrdDetect = false;
            }

        }

    }

    if ( R_Running && R_TorSlipAct)
    {
        DisconWhlRun = true;
        GrdFltState = GRIDFLT_TORSLIP;
        PUSH_DIAG(R_TorTrqDiscAlm);
    }

    ////////////////////////////////////////////
    //  Grid-monitor-Stator voltage match check
    ///////////////////////////////////////////
    if ( DisconWhlRun )
    {
        Q1OpnReq     = true;
        GridFltLatch = true;
        if ( (GmSVMtchErSq > GmSVmtchThr) || (ISimMode && !Q1StatSim ) || !S_K1Status )
        {
            GridFltLcFlg = true;

            switch (GrdFltState)
            {
                //==========================================================================
                //                           NO FAULT(0)
                //==========================================================================
            case GRIDFLT_NONE:

                break;


                //==========================================================================
                //                    Single phase open 6K secondary A (1)
                //==========================================================================
            case GRIDFLT_1PHOPN_SEC6K_A:
                PUSH_DIAG(OpnPhsAFlt1);
                OpnPhsDet1  = true;
                OpnPhsAct   = true;

                break;

                //==========================================================================
                //                   Single phase open 6K secondary B (2)
                //==========================================================================
            case GRIDFLT_1PHOPN_SEC6K_B:
                PUSH_DIAG(OpnPhsBFlt1);
                OpnPhsDet1  = true;
                OpnPhsAct   = true;

                break;

                //==========================================================================
                //                   Single phase open 6K secondary C (3)
                //==========================================================================
            case GRIDFLT_1PHOPN_SEC6K_C:
                PUSH_DIAG(OpnPhsCFlt1);
                OpnPhsDet1  = true;
                OpnPhsAct   = true;

                break;

                //==========================================================================
                //                    Single phase open primary A (4)
                //==========================================================================
            case GRIDFLT_1PHOPN_PRIM_A:
                PUSH_DIAG(OpnPhsAFlt2);
                OpnPhsDet2  = true;
                OpnPhsAct   = true;

                break;

                //==========================================================================
                //                    Single phase open primary B (5)
                //==========================================================================
            case GRIDFLT_1PHOPN_PRIM_B:
                PUSH_DIAG(OpnPhsBFlt2);
                OpnPhsDet2  = true;
                OpnPhsAct   = true;

                break;

                //==========================================================================
                //                    Single phase open primary A (6)
                //==========================================================================
            case GRIDFLT_1PHOPN_PRIM_C:
                PUSH_DIAG(OpnPhsCFlt2);
                OpnPhsDet2  = true;
                OpnPhsAct   = true;

                break;

                //==========================================================================
                //                    Open Grid 3 phase  (7)
                //==========================================================================
            case GRIDFLT_3PHOPN:
                OpnGrdDetect = true; // This flag shuts down the crobar for one task 2
                OpnGrdAct    = true; // Latch flag
                PUSH_DIAG(OpnGrdFlt);

                break;


                //==========================================================================
                //                  Volt-second over-voltage (9)
                //==========================================================================
            case GRIDFLT_VSECOV:
                // GmSecOVFlg = true;
                // PUSH_DIAG(GmVSecOverV);
                break;

                //==========================================================================
                //                    Fast Disconnect (10)
                //==========================================================================
            case GRIDFLT_FASTDIS:

                PUSH_DIAG( FastDistFlt);     // trip

                break;

                //==========================================================================
                //                     Fast Blocking (11)
                //==========================================================================
            case GRIDFLT_FASTBLK:
                PUSH_DIAG( FastBlockFlt);
                break;

                //==========================================================================
                //                     PLL Phase (12)
                //==========================================================================
            case GRIDFLT_PLLPHASE:
                PUSH_DIAG( PllPhaseFlt);
                break;

                //==========================================================================
                //                     Torsional Slip (13)
                //==========================================================================
            case GRIDFLT_TORSLIP:
                PUSH_DIAG(R_TorTrqFlt);
                break;

                //==========================================================================
                //                   Default
                //==========================================================================
            default:
                break;

            }

            ////////////////////////////////////////////////////////////////
            // End   -- Islanding-HVRT State Machine  -- WTPROT_070
            ////////////////////////////////////////////////////////////////
        }
    }
    else if ( (S_K1Status && S_K1ClsCmd) || IslandDet )  // from code up above---S_K1Status is not pure,
    {   // so clause is conditioned to K1 command
        if ( GmStrVanMtchSq > GmSVmtchThr )
        {
            GridFltLcFlg = true;
            GridFltLatch = true;
            if (!VMtchInh)
            {
            PUSH_DIAG(GmStrVanMatchTrp);    // phase A
            }
        }
        if ( GmStrVbnMtchSq > GmSVmtchThr )
        {
            GridFltLcFlg = true;
            GridFltLatch = true;
            if (!VMtchInh)
            {
            PUSH_DIAG(GmStrVbnMatchTrp);    // phase B
            }
        }
        if ( GmStrVcnMtchSq > GmSVmtchThr )
        {
            GridFltLcFlg = true;
            GridFltLatch = true;
            if (!VMtchInh)
            {
            PUSH_DIAG(GmStrVcnMatchTrp);    // phase C
            }
        }
    }


    //-------------------------------------------------------------------------
    //  General grid fast fault flag management
    //  Allows only 1msec of shutdown total until stopped

    if ( GridFltFlg )
    {
        GridFltOneSht = true; // make sure both T1's have seen this and shut down for apx. 1 Task2
    }
    GridFltFlg = false;
    if ( GridFltLcFlg && !GridFltOneSht ) // This is the local flag...a fault has been seen this pass
    {   // Means fast shut-downs were implemented on both converters
        GridFltFlg   = true;              // Only set this one time per event until complete shut-down
        RtBrgOff1Dt  = true;              // This flag shuts down the rotor converter fast
        LnBrgOff1Dt  = true;              // This flag shuts down the line converter fast
    }
    else
    {
        RtBrgOff1Dt  = false;     // This flag shuts down the rotor converter fast
        LnBrgOff1Dt  = false;     // This flag shuts down the line converter fast
    }

    if ( GrdFltClrPrm )            // do not clear until all permissives met
    {
        GridFltLatch   = false;    // leave this flag latched until stopped
        GridFltOneSht  = false;   // leave this flag latched until stopped
        OpnGrdAct      = false;
        DisconWhlRun   = false;
        GrdFltState    = GRIDFLT_NONE;
        FastDisAct     = false;
        FastBlkAct     = false;
        PhsJmpFlag     = false;
        IslMaOpnReq    = false;
        IslK1OpnReq    = false;
        DisconWhlOv    = false;
        OvCnvFltState  = OVCNVFLT_NONE;


        if ( !OpnPhsDet1 && !OpnPhsDet2 )//Done here to save exec time.
        {
            OpnPhsAct   = false;
        }
    }



    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Filtering in support of torsional instablity detection.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void TorsionalInstabiltyCheckT3 (void )
{
    // Torsional Speed/Torque Detection
    // WTProt_140 -- Torsional speed reset - done in behalf of Task 2 detection
    R_TorSpRstIn = R_TorSpdInT3 * PARM(R_TorSpRstW);
    R_TorSRstInt += R_TorSRIntGn * R_TorSpRstIn;
    R_TorSpdRst  = R_TorSpRstIn + R_TorSRstInt;

    // WTProt_130 -- Torsional torque check
    R_TorSpdMag  = sqrtf(R_TorSpdSFT3) * PARM(R_TorSpdScl);
    R_TorTrqMag  = R_TorSpdMag * PARM(R_TorTrqScl);

    // Torsional Speed/Torque Protection
    // WTProt_130 -- Torsional torque check

    if ( R_TorTrqMag > R_TorTrqMgPk ) //Peak detector
    {
        R_TorTrqMgPk = R_TorTrqMag;
    }
    R_TorTrqErr  = (R_TorTrqMag - PARM(R_TorTrqThrs));        // units = n*meters (peak)
    R_TTrqErrDel = R_TorTrqErr;

    if ( R_TorTrqErr < PARM(R_TorReset) )
    {
        R_TTrqErrDel = PARM(R_TorReset);
    }

    R_TorTrqInt  += DelTm3 * R_TTrqErrDel;
    if ( (R_TorTrqInt  < 0.0) || !PARM(TorPTrpEnbFv) || !R_Running )
    {
        R_TorTrqInt  = 0.0;
    }

    // Check for alarm
    R_TorTrqFil += (PARM(R_TorTrqFilW) * DelTm3) * ( R_TorTrqMag -  R_TorTrqFil);

    if ( (R_TorTrqFil  >= PARM(R_TorAlmOn)) && PARM(TorPAlmEnbFv) )
    {
        if ( R_BridgeRebf.BrgHdwDataVld )
        {
            R_TorsAlmAct = true;
        }
        else
        {
            R_TorTrqFil = 0.0F;
        }
    }
    else if (R_TorTrqFil  < PARM(R_TorAlmOff))
    {
        R_TorsAlmAct = false;
    }

    if (R_TorsAlmAct)
    {
        PUSH_DIAG(R_TorTrqAlm);
    }

    if ( PARM(TorPTrpEnbFv) )
    {
        if ( R_Running)
        {
            //Check for the fault.
            if ( R_TorTrqInt  >= PARM(R_TorTrpThrs) )
            {
                if ( !R_TorSlipAct)
                {
                    R_TorSlipAct = true;
                }
            }
        }
        else
        {
            if ( R_TorSlipAct && !S_K1Status)
            {
                R_TorSlipAct = false;
                NV.R_TorTripCtr = NV.R_TorTripCtr + 1; // Increment counter only once after a trip fault.
                NV.R_TorTrqPTrp = R_TorTrqMgPk;        // If trip, save away peak highest
            }                                          // value of peak torque since running
        }
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      General Wind application fault checking.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void FaultChkWindT3(void)
{
    // PLC reboot command logic

    PLCRbtCmdZ1 = PLCRebootCmd;


    if ( PLCRbtCmdT3 > ((T2FramesPerT3-0.25F)/T2FramesPerT3) )
    {
        PLCRebootCmd = true;
    }
    else
    {
        PLCRebootCmd = false;
    }


    if ( !PARM(PLCRebootInh) )//only run this code if not inhibited
    {
        if ( (PLCRebootCmd && !PLCRbtCmdZ1) && Stopped && R_ZeroSpd ) // a snapshot....all the required conditions were
        {   // present at the time the rising edge was given
            PLCRebootEnb = true;
        }

        if ( PLCRebootEnb )
        {
            RebootTmr = RebootTmr + DelTm3;
        }

        if ( !PLCRebootCmd && PLCRbtCmdZ1 )  // a downward pulse of the reboot command has occurred
        {
            PLCRebootEnb = false;
            RebootPlsTm  = RebootTmr;
            RebootTmr    = 0.0F;
            if ( (RebootPlsTm > PARM(RebootMinTm)) && (RebootPlsTm < PARM(RebootMaxTm)) && Stopped && R_ZeroSpd )
            {
                NV.PLCResetFlg = true; // save fault flag away for initialization check
                // SaveNvVarNow();
                Hard_Reset    = true; // issue the hard reset
            }
        }
        else if ( !Stopped )              // This forces another pulse to be given if converter starts (loses memory of )
        {
            PLCRebootEnb = false;
            RebootTmr    = 0.0F;
        }
    }
    ///////////////////////////////////////////////////////////////////////////
    // Islanding - HVRT State Machine Support -- Start
    ///////////////////////////////////////////////////////////////////////////

    if ( !S_VMaxFrqFrz) //Flag is clear
    {
        S_VMxFFOfTmr = 0.0F;
        //Logic to set frequency freeze flag
        if ( (GmVMagPuT3 <= PARM(S_VMaxFrqFOn) ) && !HVACStAct)
        {
            S_VMxFFOnTmr = S_VMxFFOnTmr + DelTm3;
            if ( S_VMxFFOnTmr > PARM(S_VMaxFFOnTm) )
            {
                S_VMxFFOnTmr = PARM(S_VMaxFFOnTm);
                S_VMaxFrqFrz = true;
            }
        }
        else
        {
            S_VMxFFOnTmr = 0.0F;
        }
    }
    else //Flag is set
    {
        S_VMxFFOnTmr = 0.0F;
        //Logic to clear frequency freeze flag
        if ( (GmVMagPuT3 >= PARM(S_VMaxFrqFOf) ) || HVACStAct)
        {
            S_VMxFFOfTmr = S_VMxFFOfTmr + DelTm3;
            if ( S_VMxFFOfTmr > PARM(S_VMaxFFOfTm) )
            {
                S_VMxFFOfTmr = PARM(S_VMaxFFOfTm);
                S_VMaxFrqFrz = false;
            }
        }
        else
        {
            S_VMxFFOfTmr = 0.0F;
        }
    }

    S_PwrCmdT3 = -R_TrqCmdInTtT3 * R_SpdFbkT3;
    if ( !GmLineVLo && (S_PwrCmdT3 > PARM(S_PwrLwCmIs) ) && (GmPwrT3 < PARM(S_PwrLwFbIs) ) )
    {
        S_VHiPwrLTmr = S_VHiPwrLTmr + DelTm3;
        if ( S_VHiPwrLTmr > PARM(S_VHiPwrLTm) )
        {
            S_VHiPwrLow = true;
        }
    }
    else
    {
        S_VHiPwrLow = false;
        S_VHiPwrLTmr = 0.0F;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Islanding - HVRT State Machine Support -- End
    ///////////////////////////////////////////////////////////////////////////


    //Var calculation for open phase checks

    GridIAbcGn1 = DelTm2*PARM(WGridAbc1);
    GridIAbcGn2 = DelTm2*PARM(WGridAbc2);

    /////////////////////////////////////////
    // Torsional detection / protection T3
    /////////////////////////////////////////

    if ( SimMode || (SysTmSec < 10) )
    {
        if ( PARM(R_TorSimEnb))
        {
            TorsionalInstabiltyCheckT3();
        }
        else
        {
            R_TorTrqInt  = 0.0F;
            R_TorSRstInt = R_TorSpdRaw;
        }
    }
    else
    {
        TorsionalInstabiltyCheckT3();
    }

    //////////////////////////////////////////
    // End Torsional detection / protection
    //////////////////////////////////////////


    ///////////////////////////////////////////////////////////
    // Determine permissive to clear grid fault flag - Start
    ///////////////////////////////////////////////////////////

    if ( SeqSt == STOPPED_COMMON )
    {
        if ((L_VdcFbkT3 < PARM(OpnVdcOffThr) ) &&                           // do not clear until vdc goes below threshold
                !S_K1Status && !Q1Status && (S_VMagPUT3 < PARM(OpnGrdStrVMx)) )  // and in rotor stopped state
        {
            GrdFltClrPrm    = true;
        }
    }
    else
    {
        GrdFltClrPrm    = false;
    }

    ///////////////////////////////////////////////////////
    // Determine permissive to clear grid fault flag - End
    ///////////////////////////////////////////////////////

    // Dynamic set voltage mismatch threshold
    if ( GmIMagT3 > PARM(GmSVMtchIThr))
    {
        GmSVmtchThr = GmSVmtchThr1;
    }
    else
    {
        GmSVmtchThr = GmSVmtchThr2;
    }



    ///////////////////////////////////////////////////////////////////////////
    // Grid monitor Ground Fault Protection
    ///////////////////////////////////////////////////////////////////////////

    GmIGndProtFil  = GmIGndProtFil + GmIGndProtFilWdt * ( GmIGndMagT3  - GmIGndProtFil);

    if ( GmIGndProtFil > PARM(GmGndFltThr))
    {
       PUSH_DIAG (GmGndFlt);
    }

    if ( GmIGndProtFil > PARM(GmGndAlmOn) )
    {
       GmGndAlmAct = true;
    }
    else if (GmIGndProtFil < PARM(GmGndAlmOff) )
    {
       GmGndAlmAct = false;
    }

    if (GmGndAlmAct)
    {
       PUSH_DIAG (GmGndAlm);
    }


    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//   This function uses data-passed grid monitor dv/dt values for each of the
//   grid monitor line to line voltages, then calculates an rms squared
//   distortion filter current for each phase. The rest of the function is like
//   the other TOC protectives in this product. A high select of each calculated
//   rms squared current is performed, then that result is compared to
//   thresholds in order to give, as appropriate, distortion filter TOC alarms
//   or trips.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void DistortionFilterTocDetection(void)
{
    if ( KdfStatus )
    {
        DFIaSqrT3   = (DFVaDvDtSqT3 * DFCapWSq) + DFConvISqr;
        DFIbSqrT3   = (DFVbDvDtSqT3 * DFCapWSq) + DFConvISqr;
        DFIcSqrT3   = (DFVcDvDtSqT3 * DFCapWSq) + DFConvISqr;
    }
    else
    {
        DFIaSqrT3   = 0.0F;
        DFIbSqrT3   = 0.0F;
        DFIcSqrT3   = 0.0F;
    }

    DFIaRmsMag     = DFIaRmsMag + DFWTocT3*( DFIaSqrT3  - DFIaRmsMag);
    DFIbRmsMag     = DFIbRmsMag + DFWTocT3*( DFIbSqrT3  - DFIbRmsMag);
    DFIcRmsMag     = DFIcRmsMag + DFWTocT3*( DFIcSqrT3  - DFIcRmsMag);

    DFIaRms        = sqrtf(DFIaRmsMag);
    DFIbRms        = sqrtf(DFIbRmsMag);
    DFIcRms        = sqrtf(DFIcRmsMag);


    if ( DFIaRmsMag > DFIbRmsMag )
    {
        DFTocHiSelSq = DFIaRmsMag;
    }
    else
    {
        DFTocHiSelSq = DFIbRmsMag;
    }

    if ( DFIcRmsMag > DFTocHiSelSq )
    {
        DFTocHiSelSq = DFIcRmsMag;
    }

    DFTocHiSel   = sqrtf(DFTocHiSelSq);

    if ( DFTocHiSel >= PARM(DFTocThrs ) )
    {
        PUSH_DIAG(DFTocFlt);
    }

    if ( DFTocHiSel >= PARM(DFTocAlmOn) )
    {
        DFTocAlmFlg = true;
    }
    else if ( DFTocHiSel <= PARM(DFTocAlmOff) )
    {
        DFTocAlmFlg = false;
    }

    if ( DFTocAlmFlg )
    {
        PUSH_DIAG(DFTocAlm);
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//   This function does the current error calculations for the the current
//   feedback verification checks which are essential for the verification of
//   grid monitor feedbacks if the grid monitor feedbacks are to be used for
//   more than just the grid monitor protectives.
//   The CurrentErrorDetectionT3 function uses the outputs of this function to
//   detect excess current error.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void CurrentErrorDetectionT3(void)
{
    if ( !TransDiagInh && !L_CelTstCmd && (RPNWRunning || R_Running) )
    {
        GmSLIaErrFil = GmSLIaErrFil + GmSLIErrGn*(GmSLIaErSqT3 - GmSLIaErrFil);
        GmSLIbErrFil = GmSLIbErrFil + GmSLIErrGn*(GmSLIbErSqT3 - GmSLIbErrFil);
        GmSLIcErrFil = GmSLIcErrFil + GmSLIErrGn*(GmSLIcErSqT3 - GmSLIcErrFil);
    }
    else
    {
        GmSLIaErrFil = 0.0F;
        GmSLIbErrFil = 0.0F;
        GmSLIcErrFil = 0.0F;
    }

    AuxLoadIaRms = sqrtf(GmSLIaErrFil);
    AuxLoadIbRms = sqrtf(GmSLIbErrFil);
    AuxLoadIcRms = sqrtf(GmSLIcErrFil);

    if ( AuxLoadIaRms > GmSLIErrThr && !VMtchInh)
    {
        PUSH_DIAG(GmSLIaErrFlt);
    }

    if ( AuxLoadIbRms > GmSLIErrThr && !VMtchInh )
    {
        PUSH_DIAG(GmSLIbErrFlt);
    }

    if ( AuxLoadIcRms > GmSLIErrThr && !VMtchInh )
    {
        PUSH_DIAG(GmSLIcErrFlt);
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//   This function implements the detection  fast disconnect scheme.

// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void FastDisconnectDetection(void)
{

    if ( PARM(FastDisEnb))
    {
        FastDisReq = ( (GmUV1TrpAct  && PARM(GmUV1TrpTyp))  ||
                       (GmUV2TrpAct  && PARM(GmUV2TrpTyp))  ||
                       (GmUV3TrpAct  && PARM(GmUV3TrpTyp))  ||
                       (GmUV4TrpAct  && PARM(GmUV4TrpTyp))  ||
                       (GmUV5TrpAct  && PARM(GmUV5TrpTyp))  ||
                       (GmUV6TrpAct  && PARM(GmUV6TrpTyp))  ||
                       (GmUVCvTrpAct && PARM(GmUVCvTrpTyp)) ||
                       (GmOV1TrpAct  && PARM(GmOV1TrpTyp))  ||
                       (GmOV2TrpAct  && PARM(GmOV2TrpTyp))  ||
                       (GmOV3TrpAct  && PARM(GmOV3TrpTyp))  ||
                       (GmOV4TrpAct  && PARM(GmOV4TrpTyp))  ||
                       (GmUF1TrpAct  && PARM(GmUF1TrpTyp))  ||
                       (GmUF2TrpAct  && PARM(GmUF2TrpTyp))  ||
                       (GmUF3TrpAct  && PARM(GmUF3TrpTyp))  ||
                       (GmOF1TrpAct  && PARM(GmOF1TrpTyp))  ||
                       (GmOF2TrpAct  && PARM(GmOF2TrpTyp))
                     );
    }
    else //trips are disabled
    {
        FastDisReq   = false;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculate coolant temperature based on power losses, thermal
//      characteristics, and operating conditions.
//      Thermostatic bypass valve (TBV) stops bypassing liquid when coolant
//      temperature is between 100 and 120 degF. This model gives a rough
//      steady-state estimation of coolant temperature, only when TBV is
//      fully open (no bypass) and DTA fan is running.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void CoolantTempCalc ( void )
{
    float tempVar;
    float TotalPwrIn;

    if ( PARM(CoolTempCalcFrz) == true ) return;

    if ( DTAFanCmd )
    {
        // Steady-state coolant temperature
        //---------------------------------
        TotalPwrIn = L_BrgLssFil + R_BrgLssFil + AuxCnvLoss;
        tempVar    = InletAirTemp + TotalPwrIn*FanOnGthRcp;
    }
    else
    {
        tempVar = 0.0F;
    }

    CoolntTempCalc += CoolntTempCalcGn * (tempVar - CoolntTempCalc);

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//   This function reduces error on bridge power loss calculation
//   based on junction temperature, and adjusts coolant heater
//   power as a function of grid voltage. Bridge thermal model
//   transfer functions are derived from a junction temperature
//   case of 125 degC, which renders a worst-case scenario for
//   protection purposes only, so correction is necessary to
//   achieve better coolant temperature estimations.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void BrgdTotalPowerLoss(void)
{
    float tempPwr;

    // Bridge power loss compensated for junction temperature

    tempPwr = PARM(BtmHiJncTemp) - L_BrgThrm5pModel.BtmTrJncTemp;
    L_BrgLssComp  = L_BrgThrm5pModel.BtmDpwrLoss  * (1.0F - tempPwr * PARM(BtmDLssComp));
    L_BrgLssComp += L_BrgThrm5pModel.BtmTrPwrLoss * (1.0F - tempPwr * PARM(BtmTrLssComp));
    L_BrgLssFil  += L_BrgLssGn * (L_BrgLssComp - L_BrgLssFil);

    tempPwr = PARM(BtmHiJncTemp) - R_BrgThrm5pModel.BtmTrJncTemp;
    R_BrgLssComp  = R_BrgThrm5pModel.BtmDpwrLoss  * (1.0F - tempPwr * PARM(BtmDLssComp));
    R_BrgLssComp += R_BrgThrm5pModel.BtmTrPwrLoss * (1.0F - tempPwr * PARM(BtmTrLssComp));
    R_BrgLssFil  += R_BrgLssGn * (R_BrgLssComp - R_BrgLssFil);

    AuxCnvLoss  = PmpShftPwr;

    // Total losses going to the coolant
    CoolPwrIn = L_BrgLssComp + R_BrgLssComp + PmpShftPwr;

    if ( CoolntHeatOn )
    {
        tempPwr = GmVMagT3 * SQRT2 * PARM(CFCXfrmRatio);
        tempPwr = tempPwr * tempPwr * CoolRHeatRcp;
        CoolPwrIn  = CoolPwrIn + tempPwr;
        AuxCnvLoss = AuxCnvLoss + tempPwr;
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//   This function implements a state machine for high voltage ride-through
//   events. This funcion used 153 dec Iram words when it was in P0Fltchk.c30
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////

void HVACStateMachine(void)
{
    float VMxFrqIntDel;


///////////////////////////////////////////////////////////////
// Begin -- Islanding-HVRT State Machine -- WTPROT_300
///////////////////////////////////////////////////////////////


    //==========================================================================
    //                     Over-head
    //==========================================================================

    GmPllWPuT2   = GmPllWT2 * GmFrqNomWRcp;                            // s0rule val
    VMxFrqIntDel = PARM(S_VMaxPllFW) * (GmPllWPuT2 - S_VMaxPllFil);
    if ( VMxFrqIntDel > PARM(S_VMaxPllFRt))
    {
        VMxFrqIntDel = PARM(S_VMaxPllFRt);
    }
    else if ( VMxFrqIntDel < -PARM(S_VMaxPllFRt))
    {
        VMxFrqIntDel = -PARM(S_VMaxPllFRt);
    }

    if (S_VMaxFrqFrz)
    {
        S_VMaxPllFil = 1.0F;
    }
    else
    {
        S_VMaxPllFil = S_VMaxPllFil + (VMxFrqIntDel * DelTm2);
    }

    if ( HVACState == HVAC_NORMAL)
    {
        if (S_VMaxPllFil > PARM(S_VMaxPllTh2) || (S_VHiPwrLow && (S_VMaxPllFil > PARM(S_VMaxPllTh3) ) ) )
        {
            S_VMaxFrqHi = true;
        }
        else
        {
            S_VMaxFrqHi = false;
        }
    }
    else if (  (S_VMaxPllFil > PARM(S_VMaxPllThr) ) ||  (S_VMaxPllFil < PARM(S_VMaxPllMin) ) )
    {
        S_VMaxFrqHi  = true;
    }
    else
    {
        S_VMaxFrqHi  = false;
    }


    S_VMaxPwrFil = S_VMaxPwrFil + S_VMaxPwrGn * (GmPwrT2 - S_VMaxPwrFil);                     // need mode here?
    if ( S_VMaxPwrFil > PARM(S_PwrMxIsFst) )
    {
        S_VMxIslInh = true;
    }
    else
    {
        S_VMxIslInh = false;
    }


    switch (HVACState)
    {
        //==========================================================================
        //                         NORMAL (0)
        //==========================================================================
    case HVAC_NORMAL:
        IslandDet     = false;
        S_VMaxHiTmr   = 0.0;
        S_VMaxHVRT    = false;
        R_VMgIxDcyDy  = PARM(R_VMgIxDcyMn);
        S_PwrMaxIsTmr = 0.0;
        HVACStAct     = false;
        HVACHotPll    = false;

        if ( GrdFltClrPrm)
        {
            IslandCBDsb = false;
        }

        //if ( R_InnrRegEnb)
        if ( L_Running)
        {
            S_VMaxHiEnb = true;
            if ( S_VMaxHiDet || S_VMaxFrqHi)
            {
                HVACState   = HVAC_HVDET;
                HVACStAct   = true;
            }
        }

        break;

        //==========================================================================
        //                         High voltage detect (1)
        //==========================================================================
    case HVAC_HVDET:
        S_VMaxHiTmr = S_VMaxHiTmr + DelTm2;
        S_VMaxHiEnb = false;
        R_IxErr2Fil = 0.0;
        R_IyErr2Fil = 0.0;
        HVACState   = HVAC_WAIT;
        HVACHotPll  = true;

        if ( TripFlt)
        {
            HVACState = HVAC_Q1_OPEN_REQ;
        }

        break;

        //==========================================================================
        //                         High Voltage Wait  (2)
        //==========================================================================
    case HVAC_WAIT:
        S_VMaxHiTmr = S_VMaxHiTmr + DelTm2;
        S_VMaxHiEnb = false;
        if ( S_VMaxHiTmr > PARM(R_VrgIntHiTm) )
        {
            R_VrgIntGnDy = PARM(R_VxregIntGn)*R_DelTm1;
            R_VrgIntLmDy = PARM(R_VxregLim);
        }

        if ( S_VMaxHiTmr > PARM(S_VMaxIyFsTm) )
        {
            S_VMaxIyFast = false;
        }


        if ( !S_VMaxIyFast && ((S_VMaxFrqHi && !S_VMxIslInh) || ( (FastDisAct || FastBlkAct || PhsJmpFlag || R_TorSlipAct) ) ))

        {
            HVACState = HVAC_ISLAND;

        }
        else if ( S_VMaxHiTmr >= PARM(S_VMaxHiTm) )
        {
            HVACState = HVAC_FRQCHK;
        }

        if ( S_VMaxPwrFil > PARM(S_PwrMaxIs) )
        {

            S_PwrMaxIsTmr = S_PwrMaxIsTmr + DelTm2;
            if ( S_PwrMaxIsTmr > PARM(S_PwrMaxIsTm) )
            {
                HVACState = HVAC_HVRT_DET;
            }
        }
        else
        {
            S_PwrMaxIsTmr = 0.0;
        }



        if ( TripFlt)
        {
            HVACState = HVAC_Q1_OPEN_REQ;
        }

        break;

        //==========================================================================
        //                         High Voltage Frequency Check (3)
        //==========================================================================
    case HVAC_FRQCHK:
        HVACHotPll  = true;
        S_VMaxHiEnb = false;
        if (S_VMaxFrqHi && !S_VMxIslInh)
        {
            HVACState = HVAC_ISLAND;
        }
        else
        {

            HVACState = HVAC_HVRT_DET;
            S_VMaxHVRT  = true;
            S_VMaxHiTmr = 0.0;

        }

        if ( TripFlt)
        {
            HVACState = HVAC_Q1_OPEN_REQ;
        }

        break;

        //==========================================================================
        //                         High Voltage Ride-Through (4)
        //==========================================================================
    case HVAC_HVRT_DET:
        S_VMaxHiEnb  = false;
        S_VMaxHVRT   = true;
        S_VMaxHiDet  = false;
        HVACHotPll   = false;
        R_IxrgLimDyn = PARM(R_IxregLim);
        R_IyrgLimDyn = PARM(R_IyregLim);
        R_ResLimDyn  = PARM(R_IregResLim);
        R_DcILimDynZero = false;
        R_VrgIntGnDy = PARM(R_VxregIntGn)*R_DelTm1;      // Note: CI_R_VxregIntGn = PARM(R_VxregIntGn)*R_DelTm1
        R_VrgIntLmDy = PARM(R_VxregLim);

        S_VMaxIyFast = false;

        if ( GmVMagPuT2 < PARM(S_VMaxHiDtOf) )
        {
            S_VMaxHiTmr = S_VMaxHiTmr + DelTm2;
            if ( S_VMaxHiTmr >= PARM(S_VMaxHiOfTm) )
            {
                S_VMaxHVRT   = false;
                HVACState    = HVAC_NORMAL;
                S_VMaxPuPeak = GmVMagPuT2;
                L_IrgSplGnDy = PARM(L_IregSpilGn);
                L_IyCmdLimDy = PARM(L_IyCmdLim);

            }
        }
        else
        {
            S_VMaxHiTmr = 0.0;
        }

        if (S_VMaxFrqHi && !S_VMxIslInh)
        {
            HVACState = HVAC_FRQCHK;
            S_VMaxHiDet  = true;
            R_IxrgLimDyn = 0.0;
            R_IyrgLimDyn = 0.0;
            R_ResLimDyn  = 0.0;
            R_DcILimDynZero  = true;
            R_IxErr2Fil = 0.0;
            R_IyErr2Fil = 0.0;
        }

        break;

        //==========================================================================
        //                         High Voltage ISLANDING (5)
        //==========================================================================
    case HVAC_ISLAND:
        S_VMaxHiEnb  = false;
        S_VMaxIyFast = false;
        R_VMgIxDcyDy = PARM(R_VMgIxDcyM2);
        HVACState = HVAC_Q1_OPEN_REQ;
        S_VMaxHiTmr = 0.0;
        PUSH_DIAG(IslandAlm);
        IslandDet   = true;
        S_VMxPllFlSs = S_VMaxPllFil;

        //TODO, in 1.6, L_DBInh is used. There is comments for SAB: this doesn't take into account V.L_DbGateDsb
        if ( DbInh != cDbCtl::DUAL_INHIBIT )   // this does include V.R_DBGateDsb
        {
            IslandCBDsb = true;
        }

        break;

        //==========================================================================
        //                        High Voltage Q1 Open Request  (6)
        //==========================================================================
    case HVAC_Q1_OPEN_REQ:

        Q1OpnReq    = true; // Request breaker Q1 to open
        PUSH_DIAG(IslandDiscAlm);
        HVACState   = HVAC_Q1_OPEN_WAIT;
        HvacQ1OpnTmr = 0.0F;
        S_VMaxIyFast= false;

        break;

        //==========================================================================
        //                         High Voltage Q1 Open Wait (7)
        //==========================================================================
    case HVAC_Q1_OPEN_WAIT:
        if ( TripFlt )
        {
            PUSH_DIAG(IslandFlt);
            FaultIsActive.IslandFlt = true;
            HVACState   = HVAC_NORMAL;
            S_VMaxHiDet = false;
            S_VMaxHiEnb = false;
        }
        else
        {
            if ( GridFltFlg || !Q1Status || (ISimMode && Q1StatSim) )
            {
                PUSH_DIAG(IslandFlt);
                FaultIsActive.IslandFlt = true;
                S_VMaxHiDet = false;
                S_VMaxHiEnb = false;

                if ( !R_InnrRegEnb)
                {
                    HVACState   = HVAC_NORMAL;
                }

            }
            else
            {
                HvacQ1OpnTmr += DelTm2;
                if (HvacQ1OpnTmr>PARM(HvacQ1OpnTm))
                {
                    HVACState   = HVAC_K1_OPEN;
                }
            }
        }

        break;

        //==========================================================================
        //                         High Voltage K1 Open (8)
        //==========================================================================
    case HVAC_K1_OPEN:
        R_SeqReq.R_SCEnbReq = false;
        S_K1OpnNowReq = true;
        if ( TripFlt )
        {
            PUSH_DIAG(IslandFlt);
            HVACState   = HVAC_NORMAL;
            S_VMaxHiDet = false;
            S_VMaxHiEnb = false;
        }
        else
        {
            if ( GridFltFlg || !Q1Status || (ISimMode && Q1StatSim) )
            {
                PUSH_DIAG(IslandFlt);
                S_VMaxHiDet = false;
                S_VMaxHiEnb = false;

                if ( !R_InnrRegEnb)
                {
                    HVACState   = HVAC_NORMAL;
                }

            }
        }

        break;

        //==========================================================================
        //                         Default State
        //==========================================================================


    default:
        PUSH_DIAG(BadSeqSt);
        break;

    }

    return;

////////////////////////////////////////////////////////////////
// End   -- Islanding-HVRT State Machine  -- WTPROT_300
////////////////////////////////////////////////////////////////

}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//   This function performs fault checking for task1 general Wind application
//   faults: Islanding
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void FaultChkWindT1(void)
{

    float   S_PkDetDelta;

///////////////////////////////////////////////////////////////
// Begin -- Islanding-HVRT State Machine Support -- WTPROT_310
///////////////////////////////////////////////////////////////

    S_PkDetDelta = (GmVMagPuT1 - S_VMaxPuPeak);
    if ( S_PkDetDelta > 0.0)
    {
        S_VMaxUpDet  = true;
        S_VMaxPuPeak = S_VMaxPuPeak + (S_VMgILmUpGn * S_PkDetDelta);
    }
    else
    {
        S_VMaxUpDet  = false;
        S_VMaxPuPeak = S_VMaxPuPeak + (S_VMgILmPkGn * S_PkDetDelta);
    }




    if ( ( ( S_VMaxPuPeak > PARM(S_VMaxHiDtOn) ) || S_VMaxFrqHi) && S_VMaxHiEnb)
    {
        S_VMaxHiDet  = true;
        S_VMaxIyFast = true;
        R_IxrgLimDyn = 0.0;
        R_IyrgLimDyn = 0.0;
        R_VrgIntGnDy = PARM(R_VrgIntGnHi)*R_DelTm1;               // parm here
        R_VrgIntLmDy = PARM(R_VrgIntLmHi);
        L_IrgSplGnDy = PARM(L_IrgSplGnHi);
        L_IyCmdLimDy = PARM(L_IyCmdLmHi);
        R_ResLimDyn  = 0.0;
        R_DcILimDynZero  = true;
    }


    return;

////////////////////////////////////////////////////////////////
// End   -- Islanding-HVRT State Machine Support -- WTPROT_310
////////////////////////////////////////////////////////////////

}

