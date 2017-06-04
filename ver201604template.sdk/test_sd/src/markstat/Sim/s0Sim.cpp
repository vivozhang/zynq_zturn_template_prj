///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Simulator Mode Primary Interface (ISim and XSim)
//
// DESCRIPTION:
//      This file contains the functions that implement the internal simulator
//      interfaces
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      s0ISim.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// framework

#ifdef DSIM_MODE
#else

//#include <sys/sysmgr.h> // Xsim for reboot

#endif

#include "x0XsimInterface.h"
#include "x0Vector.h"
#include "x0Sequencer.h"
#include "x0Diagnostic.h"
// product
#include "Product.h"
#include "b0BrgThrm5p.h"
#include "b0PlsTst.h"
#include "b0BridgeRebf.h"
#include "s0Sim.h"
#include "s0SimPlant1x.h"
#include "s0SimConvBrg.h"

#include "s0DbSimulator.h"
#include "s0ThrmstrSim.h"
#include "p0Sseq.h"
#include "c1BrgCroBarCtl.h"
#include "b0DbCtl.h"

#include "s0FuseSim.h"
#include "s0SwitchSim.h"
#include "s0XfmrSim.h"
#include "s0RCSnubSim.h"
#include "s0StartUpSim.h"
#include "s0ShortSim.h"
#include "s0SeriesCmpSim.h"
// added for thermal acceleration test, 20150805
#include "b0DbThrm.h"
// Constants
//----------


// Externals
//----------
extern cBrgCroBarCtl        R_BrgCroBarCtl;
extern unsigned             R_SeqStopped;
extern unsigned             R_BrgFltRstCmd;
extern volatile int         T1St;
extern volatile int         T1AT2St;
extern cDbCtl L_DbCtl ;
extern float                DelTm1;
extern float                DelTm1A;
extern float                DelTm2;
extern float                DelTm3;

extern unsigned             L_BrgPwrEnbT1;
extern unsigned             R_BrgPwrEnbT1;

extern unsigned             L_InnrRegEnbT1;
extern unsigned             R_InnrRegEnbT1;

extern unsigned             R_CCOpnCmdT1;
extern unsigned             S_K1ClsCmd;
extern unsigned             Q1ClsPulse;

extern unsigned             SimMode;
extern unsigned             XSimMode;
extern unsigned             DsimMode;

// comment out due to removeal of batt code - JWT
//extern unsigned             R_BatterySimEnb;


extern unsigned             R_SeqStBitsT1_PackedBits;
extern unsigned             R_MctlCmd_PackedBits;
extern SeqStatBits          L_SeqStat;
extern unsigned             KdfClsCmd;
extern unsigned             KdfClsFbk;
extern unsigned             Q1TripFbk;
extern unsigned             Q1StatSim;
extern unsigned             FastBlkReqHw;
extern unsigned             Ps24vOk;
extern float                GmAngElecCmd;
extern float                GmAngElecFbk;
extern float                GmPllW;
extern unsigned             Q1Status;

extern float                Imped_Scl;
extern float                K_RS;
extern float                K_SR;

extern float                L_BrgLssComp;
extern float                R_BusPwrT2;
extern float                L_DelTm1;
extern float                L_DelTm1Rcp;
extern float                R_DelTm1Rcp;
extern float                L_LXfmr;
extern float                L_RBldCapRcp;

extern float                R_BrgLssComp;
extern float                R_AngElecFbk;
extern float                R_AngElecLin;
extern float                R_DelTm1;
extern float                R_LT1TmDelta;
extern float                R_VxCmd;
extern float                R_VyCmd;

extern float                S_AngElecFbk;
extern float                S_LmNom;
extern float                S_Lr;
extern float                S_Ls;
extern float                S_PllW;
extern float                S_R1;
extern float                S_R2;

extern unsigned             AcLineIp;
extern unsigned             L_MaStatus;
extern unsigned             Q1ClsIp;

extern unsigned             GmRevSeq;
extern unsigned             InitDone;
extern unsigned             S_SimK1ClsdT1;
extern float                R_SpdSimRem;        // temporary defined in z0Stub.cpp. will be handled along with IPC singnals

extern unsigned             L_AcLinePres;
extern unsigned             L_CcstrClsCmd;

extern unsigned             R_BrgChopEnb;

extern float                CoolntTempCalc;

extern cBridgeRebf          L_BridgeRebf;
extern cBridgeRebf          R_BridgeRebf;

extern cBrgThrm5pModel      L_BrgThrm5pModel;

extern cXsimInterface       XsimInterface;

extern cMonitorCommand      gMonitor;

// comment out due to removeal of batt code - JWT
// extern cBatteryCtl          R_BatteryCtl;

extern unsigned             L_DbCmd;
extern float                L_DbTmpEst;

//Thermal Model
extern cBrgThrm5pModel      R_BrgThrm5pModel;
extern cBrgThrm5pModel      L_BrgThrm5pModel;
extern cDbThrmModel         L_DbThrmModel;
extern cSequencerBase*      pSequencer;         // base class point
//Xsim diagnostic data
extern int                  GmPllState;
extern unsigned             GmPllProven;
extern float                GmPllErrInt;
extern unsigned             GmPllErrHi;
extern float                GmPllW;
extern float                PwrPllWFFT1;
extern float                GmAngElecFbk;
extern float                GmVMagPuT2;
extern float                GmVMagGridPu  ;
extern unsigned             L_RunRdy;
extern unsigned             R_RunRdy;
extern int                  SeqSt;
extern int                  L_SeqSt;
extern int                  R_SeqSt;
extern unsigned             TripFlt;
extern unsigned             KdfTypeSt;
extern int                  L_KdfTskSt;
extern unsigned             Q1TypeSt;
extern int                  L_Q1TskSt;
extern int                  R_SyncTskSt;
extern float                GmPllWRAFT1;
extern float                GmPllWInt;
extern float                L_VdcSetPt;
extern float                L_VregRef;
extern float                L_VregErr        ;
extern float                L_IxCmd         ;
extern float                L_IxFbk;
extern float                L_IyCmd;
extern float                L_IyFbk;
extern float                L_UxCmd       ;
extern float                L_UyCmd       ;
extern float                L_ModIndex;
// Xsim Feedback Check
extern float                L_VabBrg;
extern float                L_VbcBrg;
extern float                L_IaBrg;
extern float                L_IbBrg;
extern float                L_IcBrg;
extern float                GmVan;
extern float                GmVbn;
extern float                GmVcn;
extern float                GmIa   ;
extern float                GmIb   ;
extern float                GmIc   ;
extern float                L_VdcBrg   ;
extern float                L_VdbBrg   ;

extern float                R_VabBrg;
extern float                R_VbcBrg;
extern float                R_IaBrg;
extern float                R_IbBrg;
extern float                R_IcBrg;
extern float                S_Vab;
extern float                S_Vbc;
extern float                S_Ia   ;
extern float                S_Ib   ;
extern float                S_Ic   ;
extern float                R_VdcBrg   ;
extern unsigned             TcSimEnb;
extern float                TcSpdCmd;
extern float                S_PllWInt;
extern float                S_VrefScl;
extern unsigned             L_IocPhsA;
extern unsigned             L_IocPhsB;
extern unsigned             L_IocPhsC;
extern float                S_PllVyErr;
extern float                S_VregErr;
extern float                S_VregOut;
extern float                L_ModIndFbk;
extern float                R_ModIndFbk;
extern float                L_VdcModErr;
extern float                R_VdcModErr;
extern unsigned             HVACStAct;
extern unsigned             HVACState;
extern float                S_IyCmd;
extern float                S_IyFbkT2;
extern float                R_IyCmd;
extern float                R_IyFbk;
extern float                R_IxCmd;
extern float                R_IxFbk;
extern float                IyPFCmdPu;
extern float                VregOut;
extern volatile float       VregCmd;
extern float                VregFbk;
extern float                QregCmdPu;
extern float                QregFbkPu;
extern float                VregRfMaxDyn;
extern float                GmVMagPkPuT2;
extern float                GmVMagOVFil;
extern float                GmLnVabMtchSqFl;
extern float                GmLnVbcMtchSqFl;
extern float                GmLnVcaMtchSqFl;
extern float                GmLVabMtchEr;
extern float                GmLVbcMtchEr;
extern float                GmLVcaMtchEr;
extern unsigned             IslandDet;
extern unsigned             S_VMaxHVRT;
extern unsigned             S_VMaxHiDet;
extern unsigned             S_VMaxHiEnb;
extern unsigned             S_VMaxIyFast;
extern float                S_VMaxHiTmr;

extern float                L_BrgIocThrs;

extern unsigned             QuantsPerMod;
extern unsigned             QuantsPerMod1A;
extern unsigned             Clk0PerQuant;

extern unsigned             Clk0PerT1;
extern float                FrcFreq;

extern cSimPlant1x SimPlant1x;

extern float L_VdcFbk;


// Types and Classes
//------------------
enum BrgStateSimTyp
{
    BRG_DISC_DSB  = 0,      // bridge disconnected, gating disabled
    BRG_DISC_ENB  = 1,      // bridge disconnected, gating enabled
    BRG_CONN_DSB  = 2,      // bridge connected,    gating disabled
    BRG_CONN_ENB  = 3       // bridge connected,    gating enabled
};


// Parameters
//-----------
CREATE_PARM(XsimDoeEnb          ,unsigned);     // Xsim Doe enable request
CREATE_PARM(SimAcclEnb,         unsigned);
CREATE_PARM(RemSpdReqSim        ,unsigned);     // remote Spd Sim request (from UCSA via IPC);
CREATE_PARM(R_SpdAdrSim         ,float*);       // simulator speed "nip" address
CREATE_PARM(LclSpdRefSim        ,float);        // simulated speed ref
CREATE_PARM(R_SpdAAdrSim        ,float*);       // simulator speed adder address
CREATE_PARM(XsimForceIsim       ,unsigned);     // true = force usage of isim feedbacks even when xsim client is connected
CREATE_PARM(GEvtSimEnb          ,unsigned);     // Enable the grid event simulation
CREATE_PARM(R_SpdRefFilWSim     ,float);        // simulator speed filter W (rad/sec)
CREATE_PARM(CoolEnbSimRq,        unsigned);     // Enable simulation of temperature for coolant thermistor
CREATE_PARM(XsimAcclEndTm,      float);         // Xsim acceleration end time (sec)
CREATE_PARM(BrgEnbSimReq,       unsigned);  // Bridge enable during sim mode

CREATE_PARM(S_VregAcclSim,      float);         // Stator voltage regulator acceleration factor for simulation
CREATE_PARM(R_AngCorAcclSim,    float);         // Rotor angle correction acceleration factor for simulation
CREATE_PARM(S_PllAcclSim,       float);         // Stator PLL acceleration factor for simulation
CREATE_PARM(L_VdcRefAcclSim,    float);         // Line DC link voltage reference acceleration factor for simulation
CREATE_PARM(VmaxRegAcclSim,     float);         // Vmax regulator acceleration factor for simulation
CREATE_PARM(R_FwkMrgAcclSim,    float);         // Weak flux acceleration factor for simulation
CREATE_PARM(QregAcclSim,        float);         // Q regulator acceleration factor for simulation
CREATE_PARM(VregAcclSim,        float);         // V regulator acceleration factor for simulation

CREATE_PARM(XsimFDiag0,         float*);        // Pointer to Xsim diagnostic 0
CREATE_PARM(XsimFDiag1,         float*);        // Pointer to Xsim diagnostic 1
CREATE_PARM(XsimFDiag2,         float*);        // Pointer to Xsim diagnostic 2
CREATE_PARM(XsimFDiag3,         float*);        // Pointer to Xsim diagnostic 3
CREATE_PARM(XsimFDiag4,         float*);        // Pointer to Xsim diagnostic 4
CREATE_PARM(XsimFDiag5,         float*);        // Pointer to Xsim diagnostic 5
CREATE_PARM(XsimFDiag6,         float*);        // Pointer to Xsim diagnostic 6
CREATE_PARM(XsimFDiag7,         float*);        // Pointer to Xsim diagnostic 7
CREATE_PARM(XsimFDiag8,         float*);        // Pointer to Xsim diagnostic 8
CREATE_PARM(XsimFDiag9,         float*);        // Pointer to Xsim diagnostic 9
CREATE_PARM(XsimIDiag0,         int*);     // Pointer to Xsim diagnostic 0
CREATE_PARM(XsimIDiag1,         int*);        // Pointer to Xsim diagnostic 1
CREATE_PARM(XsimIDiag2,         int*);        // Pointer to Xsim diagnostic 2
CREATE_PARM(XsimIDiag3,         int*);        // Pointer to Xsim diagnostic 3
CREATE_PARM(XsimIDiag4,         int*);        // Pointer to Xsim diagnostic 4
CREATE_PARM(XsimIDiag5,         unsigned*);        // Pointer to Xsim diagnostic 5
CREATE_PARM(XsimIDiag6,         unsigned*);        // Pointer to Xsim diagnostic 6
CREATE_PARM(XsimIDiag7,         unsigned*);        // Pointer to Xsim diagnostic 7
CREATE_PARM(XsimIDiag8,         unsigned*);        // Pointer to Xsim diagnostic 8
CREATE_PARM(XsimIDiag9,         unsigned*);        // Pointer to Xsim diagnostic 9
//-------------------------------------------------------------------------
// simulated thermistor resistance and voltages
CREATE_PARM(ThrmstrRSim,      float);           // Simulated thermistor resistance
CREATE_PARM(ThrmstrVltLwSim,  float);           // Simulated voltage at thermistor input, AEAD on low range
CREATE_PARM(ThrmstrVltHiSim,  float);           // Simulated voltage at thermistor input, AEAD on high range

//-------------------------------------------------------------------------
// simulated thermistor temperature
#ifdef TODO

CREATE_PARM(GenAeadTemp2Sim,  float);           // rotor bridge 1 inductortemperatuture for simulation mode (deg C)
CREATE_PARM(GenAeadTemp1Sim,  float);           // line inductor temperatuture for simulation mode (deg C)
CREATE_PARM(AclfuTempSim,     float);           // AC line fuse temperatuture for simulation mode (deg C)
CREATE_PARM(LhslaTempSim,     float);           // line Hsla card temperatuture for simulation mode (deg C)
CREATE_PARM(RhslaTempSim,     float);           // rotor Hsla card temperatuture for simulation mode (deg C)
CREATE_PARM(PdcCabEntryTempSim,     float);           // Contactor K1 bus temperatuture for simulation mode (deg C)

#endif
CREATE_PARM(InletAirTempSim,  float);           // Inlet air temperatuture for simulation mode   (deg C)
CREATE_PARM(PdcTempSim,       float);           // PDC temperatuture for simulation mode (deg C)
CREATE_PARM(CfcTempSim,       float);           // CFC temperatuture for simulation mode (deg C)
CREATE_PARM(CoolantTempSim,   float);           // Liquid coolant heat exchanger temperature (liquid cold side) for simulation mode (deg C)
CREATE_PARM(BasePltTempSim,   float);           // Bridge baseplate temperature for simulation mode (degC)

CREATE_PARM(CbcTempSim,       float);           // CBC temperatuture for simulation mode (deg C)
CREATE_PARM(L_IsimExeN,       unsigned);        // # of times to run the isim plant model within each task1
CREATE_PARM(R_IsimExeN,       unsigned);        // # of times to run the isim plant model within each task1A

CREATE_PARM(ShrtMVA,            float);         // Grid short circuit capacity (MVA)
CREATE_PARM(GridXOR,            float);         // Grid XoR ratio

CREATE_PARM(IsimTmPerCyc,            float);         // Execution time needed for one cycle run of Isim model
CREATE_PARM(NoIsimTmPerT1,            float);         // Execution time needed for code in one task1 (exclude Isim model)
CREATE_PARM(IsimTmRatio,            float);         // Fixed ratio of Isim execution time vs. real time

//Fuse Model param
CREATE_PARM(FuseTau0Sim, float);
CREATE_PARM(FuseTau1Sim, float);
CREATE_PARM(FuseTau2Sim, float);
CREATE_PARM(FuseTau3Sim, float);
CREATE_PARM(FuseGain0Sim, float);
CREATE_PARM(FuseGain1Sim, float);
CREATE_PARM(FuseGain2Sim, float);
CREATE_PARM(FuseGain3Sim, float);
CREATE_PARM(FuseTempLimPSim, float);
CREATE_PARM(FuseIZeroPSim, float);
CREATE_PARM(FuseVArcPSim, float);
CREATE_PARM(FuseFilInitSim, float);
CREATE_PARM(FuseResetSim, unsigned);
//Switch Model Param
CREATE_PARM(KdfOpnDlyTmSim, float);
CREATE_PARM(KdfClsDlyTmSim, float);
CREATE_PARM(K1OpnDlyTmSim, float);
CREATE_PARM(K1ClsDlyTmSim, float);
CREATE_PARM(Q1OpnDlyTmSim, float);
CREATE_PARM(Q1ClsDlyTmSim, float);
CREATE_PARM(KdfSwForceSim, unsigned);
CREATE_PARM(KdfForceCmdSim, unsigned);
CREATE_PARM(K1SwForceSim, unsigned);
CREATE_PARM(K1ForceCmdSim, unsigned);
CREATE_PARM(Q1SwForceSim, unsigned);
CREATE_PARM(Q1ForceCmdSim, unsigned);
//Transformer Model Param
CREATE_PARM(L_XfmrRatioSim, float);
CREATE_PARM(L_RhfXfmrPuSim, float);
CREATE_PARM(L_RmXfmrSim, float);
//Transformer saturation curve param
CREATE_PARM(L_XfmrSatCurvFlxK0, float);
CREATE_PARM(L_XfmrSatCurvFlxK1, float);
CREATE_PARM(L_XfmrSatCurvFlxK2, float);
CREATE_PARM(L_XfmrSatCurvFlxK3, float);
CREATE_PARM(L_XfmrSatCurvFlxK4, float);
CREATE_PARM(L_XfmrSatCurvFlxK5, float);
CREATE_PARM(L_XfmrSatCurvFlxK6, float);
CREATE_PARM(L_XfmrSatCurvFlxK7, float);
CREATE_PARM(L_XfmrSatCurvFlxK8, float);
CREATE_PARM(L_XfmrSatCurvFlxK9, float);
CREATE_PARM(L_XfmrSatCurvImagK0, float);
CREATE_PARM(L_XfmrSatCurvImagK1, float);
CREATE_PARM(L_XfmrSatCurvImagK2, float);
CREATE_PARM(L_XfmrSatCurvImagK3, float);
CREATE_PARM(L_XfmrSatCurvImagK4, float);
CREATE_PARM(L_XfmrSatCurvImagK5, float);
CREATE_PARM(L_XfmrSatCurvImagK6, float);
CREATE_PARM(L_XfmrSatCurvImagK7, float);
CREATE_PARM(L_XfmrSatCurvImagK8, float);
CREATE_PARM(L_XfmrSatCurvImagK9, float);


CREATE_PARM(L_XfmrSatCurvNum, unsigned);
CREATE_PARM(L_LineXfmrSatEnb, unsigned);
CREATE_PARM(L_LineXfmrSatCrvType, unsigned);
CREATE_PARM(L_LineXfmrSatThr, float);
CREATE_PARM(L_LineXfmrSatXm, float);


CREATE_PARM(L_XfmrFlxAInit,      float);
CREATE_PARM(L_XfmrFlxBInit,      float);
CREATE_PARM(L_XfmrFlxCInit,      float);
CREATE_PARM(L_LineRmagXfmrSim, float);

//RC Snubber Model Param
CREATE_PARM(L_CrcLineSim, float);
CREATE_PARM(L_RrcLineSim, float);
CREATE_PARM(L_CrcRShortSim, float);
CREATE_PARM(L_RrcRShortSim, float);

CREATE_PARM(L_CrcLShortSim, float);
CREATE_PARM(L_RrcLShortSim, float);

CREATE_PARM(L_CrcSCSim, float);  
CREATE_PARM(L_RrcSCSim, float);
//shunt cap

CREATE_PARM(L_CShuntSim, float);  
CREATE_PARM(L_RShuntSim, float);
CREATE_PARM(L_ShuntKVA, float); 
//grid model
CREATE_PARM(GridXPctSim, float);
CREATE_PARM(LvrtRPhsTypSim, unsigned);
CREATE_PARM(ShortOpenThr, float);
CREATE_PARM(LStartSim, float);
CREATE_PARM(RStartSim, float);
CREATE_PARM(LvrtRemoteTm, float);
CREATE_PARM(StartTmSim, float);

//CREATE_PARM(RShortEnb, unsigned);
//CREATE_PARM(LShortEnb, unsigned);
//CREATE_PARM(StartEnb, unsigned);
CREATE_PARM(LLShort, unsigned);
CREATE_PARM(LvrtRLvlSim, float);

CREATE_PARM(LvrtLPhsTypSim, unsigned);
CREATE_PARM(LvrtLocalTm, float);
CREATE_PARM(LvrtLLvlSim, float);

//Isim test
CREATE_PARM(AngTest, unsigned);
CREATE_PARM(L_AngMatch, float);
CREATE_PARM(R_AngMatch, float);
CREATE_PARM(AngMatchMargin, float);


//HVRT param
//CREATE_PARM(HVRTEnb, unsigned);
CREATE_PARM(HVRTTime, float);

CREATE_PARM(HvrtPhsTypSim, unsigned);
CREATE_PARM(HvrtLvlSim, float);

//simple Isim model or comprehensive Isim model selection
CREATE_PARM(GridModelTypeSim, unsigned);



//Series compensation MOV curve
CREATE_PARM(SCMovCurVK0Sim, float);
CREATE_PARM(SCMovCurVK1Sim, float);
CREATE_PARM(SCMovCurVK2Sim, float);
CREATE_PARM(SCMovCurVK3Sim, float);
CREATE_PARM(SCMovCurVK4Sim, float);
CREATE_PARM(SCMovCurVK5Sim, float);
CREATE_PARM(SCMovCurVK6Sim, float);
CREATE_PARM(SCMovCurVK7Sim, float);
CREATE_PARM(SCMovCurVK8Sim, float);
CREATE_PARM(SCMovCurVK9Sim, float);
CREATE_PARM(SCMovCurVK10Sim, float);

CREATE_PARM(SCMovCurIK0Sim, float);
CREATE_PARM(SCMovCurIK1Sim, float);
CREATE_PARM(SCMovCurIK2Sim, float);
CREATE_PARM(SCMovCurIK3Sim, float);
CREATE_PARM(SCMovCurIK4Sim, float);
CREATE_PARM(SCMovCurIK5Sim, float);
CREATE_PARM(SCMovCurIK6Sim, float);
CREATE_PARM(SCMovCurIK7Sim, float);
CREATE_PARM(SCMovCurIK8Sim, float);
CREATE_PARM(SCMovCurIK9Sim, float);
CREATE_PARM(SCMovCurIK10Sim, float);

CREATE_PARM(SCMovCurvNumSim, unsigned);
CREATE_PARM(SCMovEnb, unsigned);

    CREATE_PARM(LSCompSim, float);    
    CREATE_PARM(RhfCompSim, float);
    CREATE_PARM(RSwitchSim, float);
    CREATE_PARM(RSCompSim, float);
    CREATE_PARM(CSCompSim, float);
    CREATE_PARM(SCManual, unsigned);
    CREATE_PARM(SCCmd, unsigned);


// added by JL for accelerating temperature calculation, 20150727 
CREATE_PARM(DtmTr0AcclSim,      float);
CREATE_PARM(DtmTr1AcclSim,      float);
CREATE_PARM(DtmTr2AcclSim,      float);
CREATE_PARM(DtmTr3AcclSim,      float);
CREATE_PARM(BtmTrAcclSim,       float);
CREATE_PARM(BtmDAcclSim,        float);
// Variables
//----------
CREATE_PUBVAR(R_SpdSimRem,      float );
CREATE_PUBVAR(DelIsrFrc,        unsigned);           // Delta IsrFrc
CREATE_PUBVAR(XsimDoeAct,       unsigned)=false;  // Xsim Doe Acting flag
CREATE_PUBVAR(SimAcclAct,       unsigned)=false;    // Sim acceleration acting flag
CREATE_PUBVAR(SimRunRuleReq,    unsigned)=false;    // Rule run request by simulation
CREATE_PUBVAR(XsimVoltVarEnb,   unsigned);  // Xsim Volt-Var enable flag
CREATE_PUBVAR(XsimDoeCmd,       int);       // Xsim DOE command
CREATE_PUBVAR(R_TrqRefXsim,     float);     // Torque Reference From Xsim
CREATE_PUBVAR(R_VARRefXsim,     float);     // Var Reference From Xsim
CREATE_PUBVAR(R_PhiAdjustXsim,  float);     // Phi Reference From Xsim
CREATE_PUBVAR(L_VARRefXsim,     float);     // Var Reference From Xsim @ RPNW
CREATE_PUBVAR(R_WRtrXsim,       float);     // Var Reference From Xsim @ RPNW
CREATE_PUBVAR(R_TachPosnXsim,   float);     // Var Reference From Xsim @ RPNW
CREATE_PUBVAR(XsimRealTm,       float);     // Real time from Xsim

CREATE_PUBVAR(R_BrgStateSim     ,int);
CREATE_PUBVAR(L_BrgStateSim     ,int);
CREATE_PUBVAR(LclSpdRfxSim      ,float);        // local speed reference
CREATE_PUBVAR(R_SpdRefSim       ,float);        // simulated speed reference
CREATE_PUBVAR(UseXsimOutput     ,unsigned);     // use output signals from Xsim instead of Isim
CREATE_PUBVAR(Db1GateCmd        ,int);          // gating command for Db1
CREATE_PUBVAR(Db2GateCmd        ,int);          // gating command for Db2
CREATE_PUBVAR(DbGateCmd        ,int);          // gating command for Db2
CREATE_PUBVAR(IdbTot            ,float);        // total DB current flows to the DC link (Amp)
CREATE_PUBVAR(Xfmr6kvIsOpenSim  ,unsigned);     // 6kv fuses are removed for sync test
CREATE_PUBVAR(GEvtReqADLSim      ,unsigned);     // grid event request from ADL
CREATE_PUBVAR(L_DelTm1Sim       ,float);        // simulation line task 1 (sec)
CREATE_PUBVAR(L_DelTm1SimRcp    ,float);        // simulation line task 1 reciprocal(1/sec)
CREATE_PUBVAR(R_DelTm1Sim       ,float);        // simulation rotor task 1 (sec)
CREATE_PUBVAR(CoolntTempSim     ,float);        // simulated coolant temperature (degC)

//----------------------------------------
CREATE_PUBVAR(SimDevID          ,int)=207;      // Simulation Development Identifier
//----------------------------------------
CREATE_PUBVAR(L_Brg1FbkSimVax ,float);
CREATE_PUBVAR(L_Brg1FbkSimVbx ,float);
CREATE_PUBVAR(L_Brg1FbkSimVcx ,float);
CREATE_PUBVAR(L_Ain1FbkSimVax ,float);
CREATE_PUBVAR(L_Ain1FbkSimVbx ,float);
CREATE_PUBVAR(L_Ain1FbkSimVcx ,float);
CREATE_PUBVAR(L_Brg1FbkSimVdc ,float);
CREATE_PUBVAR(L_FibrFbkSimVdc ,float);
//----------------------------------------
CREATE_PUBVAR(R_Brg1FbkSimVax ,float);
CREATE_PUBVAR(R_Brg1FbkSimVbx ,float);
CREATE_PUBVAR(R_Brg1FbkSimVcx ,float);
CREATE_PUBVAR(R_Ain1FbkSimVax ,float);
CREATE_PUBVAR(R_Ain1FbkSimVbx ,float);
CREATE_PUBVAR(R_Ain1FbkSimVcx ,float);
CREATE_PUBVAR(R_Brg1FbkSimVdc ,float);
//----------------------------------------

// Intermediate varialbls for FIR
CREATE_PUBVAR(L_BrgFbkSimBrg1Vdc ,float);

CREATE_PUBVAR(L_BrgFbkSimBrg1Vax ,float);
CREATE_PUBVAR(L_BrgFbkSimBrg1Vbx ,float);
CREATE_PUBVAR(L_BrgFbkSimBrg1Vcx ,float);

CREATE_PUBVAR(L_BrgFbkSimBrg1Ia  ,float);
CREATE_PUBVAR(L_BrgFbkSimBrg1Ib  ,float);
CREATE_PUBVAR(L_BrgFbkSimBrg1Ic  ,float);

CREATE_PUBVAR(L_BrgFbkSimAin1Vax ,float);
CREATE_PUBVAR(L_BrgFbkSimAin1Vbx ,float);
CREATE_PUBVAR(L_BrgFbkSimAin1Vcx ,float);

CREATE_PUBVAR(L_BrgFbkSimAin1Ia  ,float);
CREATE_PUBVAR(L_BrgFbkSimAin1Ib  ,float);
CREATE_PUBVAR(L_BrgFbkSimAin1Ic  ,float);

CREATE_PUBVAR(R_BrgFbkSimVdcFiber,float);
CREATE_PUBVAR(R_BrgFbkSimAin1Vax ,float);
CREATE_PUBVAR(R_BrgFbkSimAin1Vbx ,float);
CREATE_PUBVAR(R_BrgFbkSimAin1Vcx ,float);
CREATE_PUBVAR(R_BrgFbkSimAin1Ia  ,float);
CREATE_PUBVAR(R_BrgFbkSimAin1Ib  ,float);
CREATE_PUBVAR(R_BrgFbkSimAin1Ic  ,float);
CREATE_PUBVAR(R_BrgFbkSimAin2Vax ,float);
CREATE_PUBVAR(R_BrgFbkSimAin2Vbx ,float);
CREATE_PUBVAR(R_BrgFbkSimAin2Vcx ,float);
CREATE_PUBVAR(R_BrgFbkSimAin2Ia  ,float);
CREATE_PUBVAR(R_BrgFbkSimAin2Ib  ,float);
CREATE_PUBVAR(R_BrgFbkSimAin2Ic  ,float);
CREATE_PUBVAR(R_BrgFbkSimBrg1Ia  ,float);
CREATE_PUBVAR(R_BrgFbkSimBrg1Ib  ,float);
CREATE_PUBVAR(R_BrgFbkSimBrg1Ic  ,float);
CREATE_PUBVAR(R_BrgFbkSimBrg1Vax ,float);
CREATE_PUBVAR(R_BrgFbkSimBrg1Vbx ,float);
CREATE_PUBVAR(R_BrgFbkSimBrg1Vcx ,float);
CREATE_PUBVAR(L_IsimExeNRcp      ,float);        // # of times to run the isim plant model within each task1 , reciprocal
CREATE_PUBVAR(R_IsimExeNRcp      ,float);        // # of times to run the isim plant model within each task1A, reciprocal

CREATE_PUBVAR(L_VdbFbkSim,      float);     // Vdb feedback
CREATE_PUBVAR(L_VdcFbkSim,      float);     // L_Vdc feedback
CREATE_PUBVAR(R_VdcFbkSim,      float);     // R_Vdc feedback

//Fuse model variables
CREATE_PUBVAR(FuseOpenIn,      unsigned);   //
CREATE_PUBVAR(FuseArcIn,      unsigned);    //
CREATE_PUBVAR(FuseMeltIn,      unsigned);   //
CREATE_PUBVAR(FuseVaIn,      float);        //
CREATE_PUBVAR(FuseVbIn,      float);        //
CREATE_PUBVAR(FuseVcIn,      float);        //

CREATE_PUBVAR(K1StateSim,      unsigned);   //
CREATE_PUBVAR(Q1StateSim,      unsigned);   //



CREATE_PUBVAR(L_BrgPhACmdVolt,      float);
CREATE_PUBVAR(L_BrgPhBCmdVolt,      float);
CREATE_PUBVAR(L_BrgPhCCmdVolt,      float);
CREATE_PUBVAR(R_BrgPhACmdVolt,      float);
CREATE_PUBVAR(R_BrgPhBCmdVolt,      float);
CREATE_PUBVAR(R_BrgPhCCmdVolt,      float);



CREATE_PUBVAR(IsimTimeClk,      float);
CREATE_PUBVAR(ExeTmPerT1,        float);
CREATE_PUBVAR(SimTmRatio,        float);
CREATE_PUBVAR(RealTimeMode,     unsigned);


CREATE_PUBVAR(ExeTmPerT1Lim,        float);
//Isim test
CREATE_PUBVAR(AngAlign,      unsigned);   //


CREATE_PUBVAR(DelTm3Wdog,        float);

SinCosTyp   L_SinCosFlxSim;

float InterfaceTemp = 0.0;

// Unpublished Variables
//----------------------
float                           L_DfC1Sim;      // Dist-filter DFC1 capacitance (Farads)
BrgStateSimTyp                  L_BrgStateSimUnp = BRG_DISC_DSB;      // state of the line-bridge connection
BrgStateSimTyp                  R_BrgStateSimUnp = BRG_DISC_DSB;      // state of the rotor-bridge connection

BrgCmdVavgSimTyp   L_BrgCmdVavgSim    = { {0.0,0.0,0.0,0.0 } };
BrgCmdVavgSimTyp   L_BrgCmdVavgSimZ1  = { {0.0,0.0,0.0,0.0 } };

BrgCmdVavgSimTyp   R_BrgCmdVavgSim    = { { 0.0,0.0,0.0,0.0 } };
BrgCmdVavgSimTyp   R_BrgCmdVavgSimZ1  = { { 0.0,0.0,0.0,0.0 } };

BrgDbCmdSimTyp     L_BrgDbCmdSim      = { false };
BrgDbCmdSimTyp     L_BrgDbCmdSimZ1    = { false };

Brg_CmdVavgSimTyp  R_BrgVavgApprxSim  = { 0.0, 0.0, 0.0, 0.0 };

// comment out due to removeal of batt code - JWT
//cBatteryCtl::BatCmdSimTyp       R_BatCmdSim        = {0.0,false,false};
//cBatteryCtl::BatCmdSimTyp       R_BatCmdSimZ1      = {0.0,false,false};

CREATE_COMPLEX_PUBVAR(L_BrgCmdTraSim , BrgCmdTraSimTyp);    //     Line-side

CREATE_COMPLEX_PUBVAR(R_BrgCmdTraSim , BrgCmdTraSimTyp);    //     Rotor-side
CREATE_COMPLEX_PUBVAR(DsimDiagFromCtl, DsimDiagFromCtlTyp); //   Diagnostics FromCtl

                                                            // DSIM TO CTL
CREATE_PUBVAR(DsimFaultCode          , unsigned);           //   Faultcode
CREATE_PUBVAR(DsimStatToCtl          , unsigned);           //   Status
CREATE_PUBVAR(DsimT1StToCtl          , unsigned);           //   Task Management
CREATE_PUBVAR(DsimTimeToCtl          , float);              //   Time
CREATE_PUBVAR(DsimTrqCmdToCtl        , float);              //   R_TrqCmd (p1Task.cpp)
CREATE_PUBVAR(DsimVarCmdToCtl        , float);              //   VarCmd
                                                            //   Line-bridge
CREATE_COMPLEX_PUBVAR(L_BrgFbkSim    , BrgFbkSimTyp);       //     Feedbacks in physical units
CREATE_COMPLEX_PUBVAR(L_BrgFltSim    , BrgFltSimTyp);       //     Faults
                                                            //   Rotor-bridge
CREATE_COMPLEX_PUBVAR(R_BrgFbkSim    , BrgFbkSimTyp);       //     Feedbacks in physical units
CREATE_COMPLEX_PUBVAR(R_BrgFltSim    , BrgFltSimTyp);       //     Faults
CREATE_COMPLEX_PUBVAR(DsimDiagToCtl  , DsimDiagToCtlTyp);   //   Diagnostics ToCtl
//-------------------------------------------------------------------------------------------------

XsimDataFromCtlTyp              XsimDataFromCtl;    // data from control to external simulator
XsimDataToCtlTyp                XsimDataToCtl;      // data to control from external simulator

float R_SpdRefSimGn; // filter gain of the speed feedback
float R_SpdRefSimUf; // unfiltered version of the speed feedback

float               IsimDelTm1;
float               IsimDelTm1Rcp;
float               IsimDelTm1A;
float               IsimDelTm1ARcp;
unsigned            R_IsimSmplCntr = 0;
unsigned            L_IsimSmplCntr = 0;
unsigned            R_IsimSmplCntRst = false;

int                 L_IsimTmCnt;
float               L_IsimTmCntRcp;
int                 R_IsimTmCnt;
float               R_IsimTmCntRcp;



int    R_BrgPhAPwmUpTm = 0;
int    R_BrgPhAPwmDnTm = 0;
int    R_BrgPhBPwmUpTm = 0;
int    R_BrgPhBPwmDnTm = 0;
int    R_BrgPhCPwmUpTm = 0;
int    R_BrgPhCPwmDnTm = 0;
int    R_BrgPhAPwmUpTmZ1 = 0;
int    R_BrgPhAPwmDnTmZ1 = 0;
int    R_BrgPhBPwmUpTmZ1 = 0;
int    R_BrgPhBPwmDnTmZ1 = 0;
int    R_BrgPhCPwmUpTmZ1 = 0;
int    R_BrgPhCPwmDnTmZ1 = 0;


int    L_BrgPhAPwmUpTm = 0;
int    L_BrgPhAPwmDnTm = 0;
int    L_BrgPhBPwmUpTm = 0;
int    L_BrgPhBPwmDnTm = 0;
int    L_BrgPhCPwmUpTm = 0;
int    L_BrgPhCPwmDnTm = 0;
int    L_BrgPhAPwmUpTmZ1 = 0;
int    L_BrgPhAPwmDnTmZ1 = 0;
int    L_BrgPhBPwmUpTmZ1 = 0;
int    L_BrgPhBPwmDnTmZ1 = 0;
int    L_BrgPhCPwmUpTmZ1 = 0;
int    L_BrgPhCPwmDnTmZ1 = 0;


int    R_BrgPhAPwmUpTmAct;
int    R_BrgPhAPwmDnTmAct;
int    R_BrgPhBPwmUpTmAct;
int    R_BrgPhBPwmDnTmAct;
int    R_BrgPhCPwmUpTmAct;
int    R_BrgPhCPwmDnTmAct;

// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------

//-----------------------------------------------------------------------------
// Isim line bridge simulator
cSimBrg::SimBrgInTyp L_BrgIn =
{
    L_BrgPhACmdVolt,
    L_BrgPhBCmdVolt,
    L_BrgPhCCmdVolt,

    SimPlant1x.L_VaBrgSrcSim,
    SimPlant1x.L_VbBrgSrcSim,
    SimPlant1x.L_VcBrgSrcSim,

    SimPlant1x.L_IaBrgInSim,
    SimPlant1x.L_IbBrgInSim,
    SimPlant1x.L_IcBrgInSim,

    SimPlant1x.L_VdcSim,

    L_InnrRegEnbT1,

    SimPlant1x.L_IocPhsASim,
    SimPlant1x.L_IocPhsBSim,
    SimPlant1x.L_IocPhsCSim,

    L_BrgLssComp,
};

cSimBrg::SimBrgInsTyp L_BrgIns =
{
    IsimDelTm1,           // 1

};
cSimBrg  L_BrgSim(L_BrgIn, L_BrgIns);    //

//-----------------------------------------------------------------------------
// Isim Rotor bridge simulator
cSimBrg::SimBrgInTyp R_BrgIn =
{

    R_BrgPhACmdVolt,
    R_BrgPhBCmdVolt,
    R_BrgPhCCmdVolt,

    SimPlant1x.R_VaEmfSim,
    SimPlant1x.R_VbEmfSim,
    SimPlant1x.R_VcEmfSim,

    SimPlant1x.R_IaBrgInSim,
    SimPlant1x.R_IbBrgInSim,
    SimPlant1x.R_IcBrgInSim,

    SimPlant1x.L_VdcSim,

    R_InnrRegEnbT1,

    SimPlant1x.R_IocPhsASim,
    SimPlant1x.R_IocPhsBSim,
    SimPlant1x.R_IocPhsCSim,

    R_BrgLssComp,
};

cSimBrg::SimBrgInsTyp R_BrgIns =
{
    IsimDelTm1A,           // 1

};
cSimBrg  R_BrgSim(R_BrgIn, R_BrgIns);    //
//-----------------------------------------------------------------------------
// encapsulated 1.5 monolithic plant simulator
cSimPlant1x::InTyp SimPlant1xIn =
{

    L_BrgPhACmdVolt,                //  1  - line bridge l-n phase A voltage
    L_BrgPhBCmdVolt,                //  2  - line bridge l-n phase B voltage
    L_BrgPhCCmdVolt,                //  3  - line bridge l-n phase C voltage

    L_BrgPwrEnbT1,                  //  4  - line side bridge power enable (from seq)
    L_InnrRegEnbT1,                 //  5  - line side inner regulator enable (from seq)

    R_BrgPhACmdVolt,                //  6  - single rotor bridge l-n phase A voltage (V l-n)
    R_BrgPhBCmdVolt,                //  7  - single rotor bridge l-n phase B voltage (V l-n)
    R_BrgPhCCmdVolt,                //  8  - single rotor bridge l-n phase C voltage (V l-n)


    R_InnrRegEnbT1,                 //  9

    Q1StateSim,                     // 10
    K1StateSim,                     // 11
    KdfClsFbk,                      // 12

    FZero,                          // 13
    AcLineIp,                       // 14
    L_AcLinePres,                   // 15

    IdbTot,                         // 16
    L_BrgThrm5pModel.BtmDpwrLoss,   // 17
    L_BrgLssComp,                   // 18
    R_BusPwrT2,                     // 19
    FZero,                          // 20

    GmRevSeq,                       // 21 - reverse phase sequence boolean
    InitDone,                       // 22
    FZero,                          // 23
    FuseOpenIn,
    FuseArcIn,
    FuseVaIn,
    FuseVbIn,
    FuseVcIn,
};

cSimPlant1x::InsTyp SimPlant1xIns =
{
    PARM(TurbineNpRat),             //  1
    PARM(SysVoltNom),               //  2
    PARM(SysFreqNom),               //  3

    PARM(ShrtMVA),                  //  4
    PARM(GridXOR),                  //  5

    PARM(VxfmrPri),                 //  6
    PARM(LxfmrPri),                 //  7
    PARM(RxfmrPri),                 //  8
    PARM(L_VxfmrSec),               //  9
    PARM(L_LxfmrSec),               // 10
    PARM(L_RxfmrSec),               // 11

    PARM(S_RRatio),                 // 12
    PARM(R_RPMNom),                 // 13
    PARM(R_PolePairs),              // 14
    S_Ls,                           // 15
    S_Lr,                           // 16
    S_R1,                           // 17
    S_R2,                           // 18
    S_LmNom,                        // 19
    Imped_Scl,                      // 20
    R_SpdRefSim,                    // 21

    PARM(CnvLineDfR),               // 22
    PARM(CnvLineDfC),               // 23
    PARM(L_CTot),                   // 24
    PARM(L_RTot),                   // 25
    PARM(L_LTot),                   // 26
    PARM(L_RBleedCap),              // 27
    PARM(L_RChg),                   // 28
    PARM(L_VdcMax),                 // 29

    IsimDelTm1,                       // 30
    IsimDelTm1Rcp,                    // 31
    IsimDelTm1A,                       // 32
    IsimDelTm1ARcp,                    // 33

    PARM(GEvtSimEnb),               // 34
    PARM(L_RevSeqReq),              // 35

    L_BrgIocThrs,
};
cSimPlant1x SimPlant1x ( SimPlant1xIn, SimPlant1xIns );


//-----------------------------------------------------------------------------
// Isim parallel bridge connection only for 3x

cDbSimulator::InTyp L_DbSimulatorIn=
{
    DbGateCmd,                    // 1 - DB1 gating cmd
    False,                        // 2 - DB2 gating cmd
    SimPlant1x.L_VdcSim,           // 3 - DC link voltage (Volt)
};

cDbSimulator::InsTyp L_DbSimulatorIns=
{
    L_DelTm1,                      // 1 - Delta task1 time (sec)
    PARM(BrgDbL),                  // 2 - Dynamic break1 resistor parasitic inductance (Henry)
    PARM(BrgDbL),                  // 3 - Dynamic break2 resistor parasitic inductance (Henry)
    PARM(BrgDbR),                  // 4 - Dynamic break1 resistor (in series with each DB switch) (Ohm)
    PARM(BrgDbR),                  // 5 - Dynamic break2 resistor (in series with each DB switch) (Ohm)
};
cDbSimulator L_DbSimulator ( L_DbSimulatorIn, L_DbSimulatorIns);

// comment out due to removeal of batt code - JWT
//
//cBatSimulator::InTyp R_BatSimulatorIn=
//{
//    R_BatterySimEnb,              // 1-Battery simulator enable
//    R_BatCmdSimZ1.ChgSwEnb,       // 2-Enable charge igbt
//    R_BatCmdSimZ1.DisSwEnb,       // 3-Enable discharge igbt
//    R_BatCmdSimZ1.DutyCmd,        // 4-Battery chopper duty reference
//    SimPlant1x.L_VdcSim,          // 5-DC link voltage (Volt)
//};
//
//cBatSimulator::InsTyp R_BatSimulatorIns=
//{
//    R_DelTm1,                    // 1-task1 cycle time (Sec)
//    R_DelTm1Rcp,                 // 2-task 1 execution reciprocal
//    PARM(BccRbatNom),            // 3-Battery internal resistance (ohms)
//    PARM(BccLfilt),              // 4-Chopper inductor
//    PARM(BccCfilt),              // 5-Chopper capacitor
//};
//cBatSimulator R_BatSimulator ( R_BatSimulatorIn, R_BatSimulatorIns);
//

//-----------------------------------------------------------------------------
// Thermistor simulator
cThrmstrSim::InTyp ThrmstrSimIn =
{
    T1AT2St,                    // 1-T1 state within a T3 frame
    CoolntTempSim,              // 2-Coolant temp (degC)
    PARM(BasePltTempSim),       // 3-Baseplate temp (degC)
};

cThrmstrSim::InsTyp ThrmstrSimIns =
{
    PARM(TempVcoZero),          // 1-Vco frequency corresponding to zero voltage (Hz)
    PARM(TempVcoScl),           // 2-Vco Voltage over frequency scale (V/Hz)
};
cThrmstrSim ThrmstrSim (ThrmstrSimIn, ThrmstrSimIns);

// Isim Fuse model

cSimFuse::InTyp FuseIn=
{
    SimPlant1x.L_IaFuseSim,          // 1 - Fuse phase a current feedback
    SimPlant1x.L_IbFuseSim,          // 2 - Fuse phase b current feedback
    SimPlant1x.L_IcFuseSim,          // 3 - Fuse phase c current feedback
    PARM(FuseResetSim),              // 4 - Fuse Reset Cmd
};

cSimFuse::InsTyp FuseIns=
{
    IsimDelTm1,                         // 1 - Delta task1 time (sec)
    PARM(FuseTau0Sim),                  // 2
    PARM(FuseTau1Sim),                  // 3
    PARM(FuseTau2Sim),                  // 4
    PARM(FuseTau3Sim),                  // 5
    PARM(FuseGain0Sim),                 // 6
    PARM(FuseGain1Sim),                 // 7
    PARM(FuseGain2Sim),                 // 8
    PARM(FuseGain3Sim),                 // 9
    PARM(FuseTempLimPSim),              // 10
    PARM(FuseIZeroPSim),                // 11
    PARM(FuseVArcPSim),                 // 12
    PARM(FuseFilInitSim),               // 13
};
cSimFuse L_FuseSim ( FuseIn, FuseIns);


//-----------------------------------------------------------------------------
// Isim Switch model




cSwitchSim::InTyp KdfSwitchIn=
{
    KdfClsCmd,                    // 1

};

cSwitchSim::InsTyp KdfSwitchIns=
{
    IsimDelTm1,                   // 1 - Delta task1 time (sec)
    PARM(KdfOpnDlyTmSim),         // 2
    PARM(KdfClsDlyTmSim),         // 3
    PARM(KdfSwForceSim),          // 4
    PARM(KdfForceCmdSim),         // 5

};
cSwitchSim KdfSwitchSim ( KdfSwitchIn, KdfSwitchIns);

cSwitchSim::InTyp K1SwitchIn=
{
    S_K1ClsCmd,                   // 1

};

cSwitchSim::InsTyp K1SwitchIns=
{
    IsimDelTm1,                   // 1 - Delta task1 time (sec)
    PARM(K1OpnDlyTmSim),          // 2
    PARM(K1ClsDlyTmSim),          // 3
    PARM(K1SwForceSim),           // 4
    PARM(K1ForceCmdSim),          // 5

};
cSwitchSim K1SwitchSim ( K1SwitchIn, K1SwitchIns);

cSwitchSim::InTyp Q1SwitchIn=
{
    Q1ClsIp,             //  Q1ClsPulse,                    //Q1StatSim,//

};

cSwitchSim::InsTyp Q1SwitchIns=
{
    IsimDelTm1,                   // 1 - Delta task1 time (sec)
    PARM(Q1OpnDlyTmSim),          // 2
    PARM(Q1ClsDlyTmSim),          // 3
    PARM(Q1SwForceSim),           // 4
    PARM(Q1ForceCmdSim),          // 5

};
cSwitchSim Q1SwitchSim ( Q1SwitchIn, Q1SwitchIns);


// Isim Transformer simulator
cXfmrSim::InTyp L_XfmrIn =
{
    SimPlant1x.L_VaXfmrMgSim,
    SimPlant1x.L_VbXfmrMgSim,
    SimPlant1x.L_VcXfmrMgSim,

    SimPlant1x.L_VaXfmrSecSim,
    SimPlant1x.L_VbXfmrSecSim,
    SimPlant1x.L_VcXfmrSecSim,


    SimPlant1x.L_WElecSim,
    SimPlant1x.L_XfmrState,
    L_IsimSmplCntr,
    SimPlant1x.L_BigRchg,



};

cXfmrSim::InsTyp L_XfmrIns =
{
    IsimDelTm1,           // 1
    PARM(L_XfmrRatioSim),
    PARM(VxfmrPri),                 //  6
    PARM(LxfmrPri),                 //  7
    PARM(RxfmrPri),                 //  8
    PARM(L_VxfmrSec),               //  9
    PARM(L_LxfmrSec),               // 10
    PARM(L_RxfmrSec),               // 11
    PARM(L_RhfXfmrPuSim),
    PARM(L_RmXfmrSim),
    PARM(L_LineXfmrSatEnb),
    PARM(L_LineXfmrSatCrvType),
    PARM(L_LineFrq),
    PARM(XfmrKva),
    PARM(L_XfmrSatCurvNum),
    PARM(L_XfmrSatCurvFlxK0),
    PARM(L_XfmrSatCurvFlxK1),
    PARM(L_XfmrSatCurvFlxK2),
    PARM(L_XfmrSatCurvFlxK3),
    PARM(L_XfmrSatCurvFlxK4),
    PARM(L_XfmrSatCurvFlxK5),
    PARM(L_XfmrSatCurvFlxK6),
    PARM(L_XfmrSatCurvFlxK7),
    PARM(L_XfmrSatCurvFlxK8),
    PARM(L_XfmrSatCurvFlxK9),
    PARM(L_XfmrSatCurvImagK0),
    PARM(L_XfmrSatCurvImagK1),
    PARM(L_XfmrSatCurvImagK2),
    PARM(L_XfmrSatCurvImagK3),
    PARM(L_XfmrSatCurvImagK4),
    PARM(L_XfmrSatCurvImagK5),
    PARM(L_XfmrSatCurvImagK6),
    PARM(L_XfmrSatCurvImagK7),
    PARM(L_XfmrSatCurvImagK8),
    PARM(L_XfmrSatCurvImagK9),
    PARM(L_LineXfmrSatThr),
    PARM(L_LineXfmrSatXm),
    PARM(L_XfmrFlxAInit),
    PARM(L_XfmrFlxBInit),
    PARM(L_XfmrFlxCInit),
    PARM(L_LineRmagXfmrSim),



};
cXfmrSim  L_XfmrSim(L_XfmrIn, L_XfmrIns);    //

// Isim RC snubber simulator(Line side)
cRCSnubSim::InTyp LnRCSnubIn =
{
    SimPlant1x.L_IxRCSim,
    SimPlant1x.L_IyRCSim,
    SimPlant1x.L_VxRCSimHold,
    SimPlant1x.L_VyRCSimHold,

    SimPlant1x.L_WElecSim,
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,



};

cRCSnubSim::InsTyp LnRCSnubIns =
{
    IsimDelTm1,           // 1
    PARM(L_CrcLineSim),
    PARM(L_RrcLineSim),


};
cRCSnubSim  LnRCSnubSim(LnRCSnubIn, LnRCSnubIns);    //

// Isim RC snubber simulator(Line side)
cRCSnubSim::InTyp RSRCSnubIn =
{
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    SimPlant1x.IaRCRSSim,
    SimPlant1x.IbRCRSSim,
    SimPlant1x.IcRCRSSim,
    SimPlant1x.VaRCRSSim,
    SimPlant1x.VbRCRSSim,
    SimPlant1x.VcRCRSSim,


    SimPlant1x.RSPhaseState,


};

cRCSnubSim::InsTyp RSRCSnubIns =
{
    IsimDelTm1,           // 1
    PARM(L_CrcRShortSim),
    PARM(L_RrcRShortSim),


};
cRCSnubSim  RSRCSnubSim(RSRCSnubIn, RSRCSnubIns);    //
// Isim RC snubber simulator(shunt cap side)
cRCSnubSim::InTyp LSRCSnubIn =
{
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    SimPlant1x.IaRCLSSim,
    SimPlant1x.IbRCLSSim,
    SimPlant1x.IcRCLSSim,
    SimPlant1x.VaRCLSSim,
    SimPlant1x.VbRCLSSim,
    SimPlant1x.VcRCLSSim,


    SimPlant1x.LSPhaseState,


};

cRCSnubSim::InsTyp LSRCSnubIns =
{
    IsimDelTm1,           // 1
    SimPlant1x.CrcLSSim,
    SimPlant1x.RrcLSSim,
  //  PARM(L_CrcLineSim),
 //   PARM(L_RrcLineSim),
 

};
cRCSnubSim  LSRCSnubSim(LSRCSnubIn, LSRCSnubIns);    // 


// Isim RC snubber simulator(series compensator side)
cRCSnubSim::InTyp SCRCSnubIn =    
{     
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,
    SimPlant1x.IaRCSCSim,
    SimPlant1x.IbRCSCSim,
    SimPlant1x.IcRCSCSim,
    SimPlant1x.VaRCSCSim,
    SimPlant1x.VbRCSCSim,
    SimPlant1x.VcRCSCSim,
 
    
    SimPlant1x.SCPhaseState,  
           
       
};

cRCSnubSim::InsTyp SCRCSnubIns =
{
    IsimDelTm1,           // 1  
    PARM(L_CrcSCSim),
    PARM(L_RrcSCSim),
 
 

};
cRCSnubSim  SCRCSnubSim(SCRCSnubIn, SCRCSnubIns);    // 




//Isim startup circuit



cStartUpSim::InTyp LineStartUpIn =
{

    SimPlant1x.L_VaXfmrMgSim,
    SimPlant1x.L_VbXfmrMgSim,
    SimPlant1x.L_VcXfmrMgSim,
    SimPlant1x.L_VanGridSim,
    SimPlant1x.L_VbnGridSim,
    SimPlant1x.L_VcnGridSim,



};

cStartUpSim::InsTyp LineStartUpIns =
{
    IsimDelTm1,           // 1
    SimPlant1x.StartEnb,
    PARM(ShortOpenThr),
    PARM(LStartSim),
    PARM(RStartSim),


};
cStartUpSim  LineStartUpSim(LineStartUpIn, LineStartUpIns);    //




//Remote short
cShortSim::InTyp RemoteShortIn =
{
    SimPlant1x.RShortEnb,
    SimPlant1x.L_VanGridSim,
    SimPlant1x.L_VbnGridSim,
    SimPlant1x.L_VcnGridSim,
    SimPlant1x.VaRCRSSim,
    SimPlant1x.VbRCRSSim,
    SimPlant1x.VcRCRSSim,
	SimPlant1x.IaGridSptLSim,
    SimPlant1x.IbGridSptLSim,
    SimPlant1x.IcGridSptLSim,
	SimPlant1x.IaGridSptRSim,
    SimPlant1x.IbGridSptRSim,
    SimPlant1x.IcGridSptRSim,
};

cShortSim::InsTyp RemoteShortIns =
{
    IsimDelTm1,           // 1
    PARM(LvrtRPhsTypSim),
    PARM(LvrtRemoteTm),
    PARM(ShortOpenThr),
    SimPlant1x.L_RDirectShort,
    PARM(LLShort),
    SimPlant1x.L_RRshortSim,
    SimPlant1x.L_LRshortSim,
    SimPlant1x.L_RRshortllSim,
    SimPlant1x.L_LRshortllSim,

};
cShortSim  RemoteShortSim(RemoteShortIn, RemoteShortIns);    //

//Local short
cShortSim::InTyp LocalShortIn =
{
    SimPlant1x.LShortEnb,
    SimPlant1x.L_VanGridSim,
    SimPlant1x.L_VbnGridSim,
    SimPlant1x.L_VcnGridSim,
    SimPlant1x.L_VaXfmrMgSim,//VaRCLSSim,
    SimPlant1x.L_VbXfmrMgSim,//VbRCLSSim,
    SimPlant1x.L_VcXfmrMgSim,//VcRCLSSim,
	InterfaceTemp,
	InterfaceTemp,
    InterfaceTemp,       
    InterfaceTemp,
    InterfaceTemp,
    InterfaceTemp,	
};

cShortSim::InsTyp LocalShortIns =
{
    IsimDelTm1,           // 1
    PARM(LvrtLPhsTypSim),
    PARM(LvrtLocalTm),
    PARM(ShortOpenThr),
    SimPlant1x.L_LDirectShort,
    PARM(LLShort),
    SimPlant1x.L_RLshortSim,
    SimPlant1x.L_LLshortSim,
    SimPlant1x.L_RLshortllSim,
    SimPlant1x.L_LLshortllSim, 

};
cShortSim  LocalShortSim(LocalShortIn, LocalShortIns);    // 

//series compensation

cSeriesCmpSim::InTyp SCPhaIn =
{

    SimPlant1x.SCEnb,
    SCRCSnubSim.VaRCSim,//SimPlant1x.VaSCRCSim,//    

    SimPlant1x.L_VaXfmrMgSim,
    SimPlant1x.L_WElecSim,  
    SimPlant1x.IaGridSCSim,


};

cSeriesCmpSim::InsTyp SCPhaIns =
{
    IsimDelTm1,           // 1  
    
    PARM(ShortOpenThr),


};
cSeriesCmpSim  SeriesCmpSimPha(SCPhaIn, SCPhaIns);    // 

cSeriesCmpSim::InTyp SCPhbIn =
{

    SimPlant1x.SCEnb,
    SCRCSnubSim.VbRCSim,//

    SimPlant1x.L_VbXfmrMgSim,
    SimPlant1x.L_WElecSim,  
    SimPlant1x.IbGridSCSim,


};

cSeriesCmpSim::InsTyp SCPhbIns =
{
    IsimDelTm1,           // 1  

    PARM(ShortOpenThr),


};
cSeriesCmpSim  SeriesCmpSimPhb(SCPhbIn, SCPhbIns);    // 

cSeriesCmpSim::InTyp SCPhcIn =
{

    SimPlant1x.SCEnb,
    SCRCSnubSim.VcRCSim,//
 
    SimPlant1x.L_VcXfmrMgSim,
    SimPlant1x.L_WElecSim,  
    SimPlant1x.IcGridSCSim,


};

cSeriesCmpSim::InsTyp SCPhcIns =
{
    IsimDelTm1,           // 1  

    PARM(ShortOpenThr),


};
cSeriesCmpSim  SeriesCmpSimPhc(SCPhcIn, SCPhcIns);    // 


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
//
// EXECUTION LEVEL
//      Initialization
//
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
int SimRules(void)
{
    float temp1;
    float TempFlx1;
    float TempFlx2;
    float TempFlx3;

    int Status = SUCCESS;

    SET_FV(GridModelTypeSim, SIMPLE_GRID_MODEL);
    
    if(PARM(GridModelTypeSim) == SIMPLE_GRID_MODEL)
        {
        SET_FV(L_IsimExeN, 10);
        }
    else 
        {
        SET_FV(L_IsimExeN, 50);
        }

    SET_FV(R_IsimExeN, 3*PARM(L_IsimExeN) / 2);

    SET_FV(NoIsimTmPerT1, 115.0e-6F);
    SET_FV(IsimTmPerCyc, 6.0e-6F);
    SET_FV(IsimTmRatio, 1.0F);


  // ExeTmPerT1 = 115.0e-6F + PARM(L_IsimExeN) * 7.0e-6F;  // Application code average executing time
    ExeTmPerT1Lim = PARM(NoIsimTmPerT1) + PARM(L_IsimExeN) * PARM(IsimTmPerCyc);
    ExeTmPerT1 = PARM(IsimTmRatio) * DelTm1;

    if(ExeTmPerT1 < ExeTmPerT1Lim)
    {
        ExeTmPerT1 = ceil(ExeTmPerT1Lim / DelTm1 ) * DelTm1;
    }

    if((ExeTmPerT1 > DelTm1) && (SimMode))
    {
        RealTimeMode = false;
        DelIsrFrc =  ExeTmPerT1 * FrcFreq ;
        SimTmRatio  =   ExeTmPerT1 / DelTm1;
    }
    else
    {
        if(XSimMode)
        {
            RealTimeMode = false;
        }
        else
        {
            RealTimeMode = true;
        }
        DelIsrFrc = Clk0PerT1;   // 4400
        SimTmRatio  = 1.0;
    }

    DelTm3Wdog = SimTmRatio * DelTm3;       // watchdog timing register for task3


    // number of iterations for simulator within a Task1
    IsimDelTm1     = NoNANLim(DelTm1 / PARM(L_IsimExeN), 1.0e6F,-1.0e6F);
    L_IsimExeNRcp  = NoNANLim(1.0F/PARM(L_IsimExeN), 1.0e6F,-1.0e6F);
    IsimDelTm1Rcp  = NoNANLim(1.0F / IsimDelTm1, 1.0e6F,-1.0e6F);
    IsimDelTm1A    = NoNANLim(DelTm1A / PARM(R_IsimExeN), 1.0e6F,-1.0e6F);
    R_IsimExeNRcp  = NoNANLim(1.0F/PARM(R_IsimExeN), 1.0e6F,-1.0e6F);
    IsimDelTm1ARcp = NoNANLim(1.0F / IsimDelTm1A, 1.0e6F,-1.0e6F);

    temp1 = QuantsPerMod * Clk0PerQuant;
    L_IsimTmCnt   =  temp1  / PARM(L_IsimExeN);
    L_IsimTmCntRcp   =  1.0F / ((float)L_IsimTmCnt);

    temp1 = QuantsPerMod1A * Clk0PerQuant;
    R_IsimTmCnt   =  temp1 / PARM(R_IsimExeN);
    R_IsimTmCntRcp   =  1.0F / ((float)R_IsimTmCnt);
    //-------------------------------------------------------------------------
    // local rules

    // simulated speed reference
    SET_FV(R_SpdAAdrSim, (float * ) LADR_UnusedFloat);          //rpm

    // This piece of code was in z0VarCal for PumpBack test
    if ( PARM(TestMode) == PUMP_BACK )
    {
        LclSpdRfxSim = 0.0F;
        SET_FV(R_SpdAdrSim, (float * ) LADR_LclSpdRfxSim);  // Force speed simulation to zero, in any mode!
    }
    else
    {
        LclSpdRfxSim = PARM(LclSpdRefSim);

        if ( PARM(Net_Type) == NET_ABSENT )                         // Local mode
        {
            if  ( PARM(TcSimEnb) )
            {
                SET_FV(R_SpdAdrSim, (float * ) LADR_TcSpdCmd);
            }
            else
            {
                SET_FV(R_SpdAdrSim, (float * ) LADR_LclSpdRfxSim);
            }
        }
        else                                                        // IPC interface
        {
            if ( PARM(RemSpdReqSim) )
            {
                SET_FV(R_SpdAdrSim, (float * ) LADR_R_SpdSimRem);   //rpm
            }
            else
            {
                SET_FV(R_SpdAdrSim, (float * ) LADR_LclSpdRfxSim);  //rpm
            }
        }
    }

    // force us of isim feedbacks even when xsim client is connected
    SET_FV(XsimForceIsim, false);

    SET_FV(RhslaTempSim,     40.0F);
    SET_FV(LhslaTempSim,     40.0F);

    SET_FV(InletAirTempSim, 30.0F);
    SET_FV(PdcTempSim,      30.0F);
    SET_FV(CfcTempSim,      30.0F);
    SET_FV(CoolantTempSim,  30.0F);
    SET_FV(BasePltTempSim,  35.0F);

    if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
    {
        SET_FV(GenAeadTemp2Sim,  30.0F);
        SET_FV(GenAeadTemp1Sim,  30.0F);
        SET_FV(AclfuTempSim,     30.0F);
        SET_FV(PdcCabEntryTempSim, 30.0F);
    }
    else
    {
        SET_FV(GenAeadTemp2Sim,  0.0F);
        SET_FV(GenAeadTemp1Sim,  0.0F);
        SET_FV(AclfuTempSim,     0.0F);
        SET_FV(PdcCabEntryTempSim, 0.0F);
    }

    // Set simulated thermistor resistance for 20 degrees C.
    // (Refer to resistance vs. temperature table on system elementary.)
    SET_FV(ThrmstrRSim,    12580.0);

    // Calculate voltage on AEAD thermistor input from the thermistor resistance.
    // AEAD has 100 ohms in series with thermistor and 22.1k (low range) or (22.1k||402)
    // = 394.8 ohms (high range) loading resistance.  Circuit is connected to 5.0 volts.
    SET_FV(ThrmstrVltLwSim, 5.0F * 22.1e3F / ((PARM(ThrmstrRSim) + 100.0F) + 22.1e3F) );
    SET_FV(ThrmstrVltHiSim, 5.0F * 394.8F  / ((PARM(ThrmstrRSim) + 100.0F) +  394.8F) );

    Xfmr6kvIsOpenSim = false;

    // Two Distortion filter
    L_DfC1Sim = PARM(CnvLineDfC);

    SET_FV(ShrtMVA,     100.0F);
    SET_FV(GridXOR,     20.0F);

    SET_FV(GridXPctSim, 0.1);
    SET_FV(LvrtRLvlSim, 0.05);
SET_FV(LvrtLLvlSim, 0.05);

    //-------------------------------------------------------------------------
    // run the class rules
    if ( SimPlant1x.RunRules()     == FAIL)     Status = FAIL;
    if ( L_DbSimulator.RunRules()  == FAIL)     Status = FAIL;
    if ( ThrmstrSim.RunRules()     == FAIL)     Status = FAIL;

    // comment out due to removeal of batt code - JWT
    // if ( R_BatSimulator.RunRules() == FAIL)     Status = FAIL;

    SET_FV(R_SpdRefFilWSim,5.0F);
    R_SpdRefSimGn = PARM(R_SpdRefFilWSim) * R_DelTm1;

    // rule for acceleration parameters
    SET_FV(XsimAcclEndTm,   3.5F);
    SET_FV(S_VregAcclSim,   5.0F);
    SET_FV(R_AngCorAcclSim, 10.0F);
    SET_FV(S_PllAcclSim,    4.0F);
    SET_FV(L_VdcRefAcclSim, 6.0F);

    // added by JL, 20150727
    SET_FV(DtmTr0AcclSim, 1.0F);
    SET_FV(DtmTr1AcclSim, 50.0F);
    SET_FV(DtmTr2AcclSim, 1.0F);
    SET_FV(DtmTr3AcclSim, 1.0F);
    SET_FV(BtmTrAcclSim,  1.0F);
    SET_FV(BtmDAcclSim,   1.0F);

// rules for Fuse model
    SET_FV(FuseTau0Sim,   0.0022459F);
    SET_FV(FuseTau1Sim,   0.0128377F);
    SET_FV(FuseTau2Sim,   0.3533196F);
    SET_FV(FuseTau3Sim,   15.049636F);
    SET_FV(FuseGain0Sim,   3.20191e-5F);
    SET_FV(FuseGain1Sim,   1.84375e-5F);
    SET_FV(FuseGain2Sim,   1.96353e-5F);
    SET_FV(FuseGain3Sim,   1.58022e-4F);
    SET_FV(FuseTempLimPSim,   962.0F);
    SET_FV(FuseIZeroPSim,   0.5F);
    SET_FV(FuseVArcPSim,   400.0F);
    SET_FV(FuseFilInitSim,   115600.0F);
    SET_FV(FuseResetSim,   0);
//rules for Transformer model
    SET_FV(L_RmXfmrSim,   10000.0F);
    SET_FV(L_RhfXfmrPuSim,   70.0F);
    SET_FV(L_XfmrRatioSim,   SQRT3);

    //rules for RC Snubber model
    SET_FV(L_CrcLineSim,   20.0e-6F);
    SET_FV(L_RrcLineSim,   0.05F);
	SET_FV(L_ShuntKVA,   0.001F);

    SET_FV(L_CShuntSim,   PARM(L_ShuntKVA) * 1.0e3 /PARM(L_LineFrq) /PARM(SysVoltNom) /PARM(SysVoltNom) );
    SET_FV(L_RShuntSim,   1.0e-6F);  
	if(PARM(L_ShuntKVA)>10.0)
	{
        SimPlant1x.CrcLSSim = PARM(L_CShuntSim);
        SimPlant1x.RrcLSSim = PARM(L_RShuntSim);
	}
	else
   	{
        SimPlant1x.CrcLSSim = PARM(L_CrcLineSim);
        SimPlant1x.RrcLSSim = PARM(L_RrcLineSim);
	}
        

    SET_FV(L_CrcRShortSim,   1000.0e-6F);
    SET_FV(L_RrcRShortSim,   0.00136F);


    SET_FV(L_CrcLShortSim,   1000.0e-6F);
    SET_FV(L_RrcLShortSim,   0.00136F);  
    
    SET_FV(L_CrcSCSim,   20.0e-6F);
    SET_FV(L_RrcSCSim,   0.05F);  
 //   L_FuseSim.RunRules();

//rules for Switch Model
    SET_FV(KdfOpnDlyTmSim,   0.1F);
    SET_FV(KdfClsDlyTmSim,   0.1F);
    SET_FV(K1OpnDlyTmSim,   0.1F);
    SET_FV(K1ClsDlyTmSim,   0.075F);
    SET_FV(Q1OpnDlyTmSim,   0.07F);
    SET_FV(Q1ClsDlyTmSim,   0.1F);
    SET_FV(KdfSwForceSim,   0);
    SET_FV(KdfForceCmdSim,  0);
    SET_FV(K1SwForceSim,   0);
    SET_FV(K1ForceCmdSim,   0);
    SET_FV(Q1SwForceSim,   0);
    SET_FV(Q1ForceCmdSim,   0);
    SET_FV(VmaxRegAcclSim,  4.0F);
    SET_FV(R_FwkMrgAcclSim, 4.0F);
    SET_FV(QregAcclSim,     4.0F);
    SET_FV(VregAcclSim,     1.0F);

    SET_FV(XsimFDiag0,      (float *) LADR_FZero);
    SET_FV(XsimFDiag1,      (float *) LADR_FZero);
    SET_FV(XsimFDiag2,      (float *) LADR_FZero);
    SET_FV(XsimFDiag3,      (float *) LADR_FZero);
    SET_FV(XsimFDiag4,      (float *) LADR_FZero);
    SET_FV(XsimFDiag5,      (float *) LADR_FZero);
    SET_FV(XsimFDiag6,      (float *) LADR_FZero);
    SET_FV(XsimFDiag7,      (float *) LADR_FZero);
    SET_FV(XsimFDiag8,      (float *) LADR_FZero);
    SET_FV(XsimFDiag9,      (float *) LADR_FZero);

    SET_FV(XsimIDiag0,      (int *) LADR_UnusedInt);
    SET_FV(XsimIDiag1,      (int *) LADR_UnusedInt);
    SET_FV(XsimIDiag2,      (int *) LADR_UnusedInt);
    SET_FV(XsimIDiag3,      (int *) LADR_UnusedInt);
    SET_FV(XsimIDiag4,      (int *) LADR_UnusedInt);
    SET_FV(XsimIDiag5,      (unsigned *) LADR_UnusedUInt);
    SET_FV(XsimIDiag6,      (unsigned *) LADR_UnusedUInt);
    SET_FV(XsimIDiag7,      (unsigned *) LADR_UnusedUInt);
    SET_FV(XsimIDiag8,      (unsigned *) LADR_UnusedUInt);
    SET_FV(XsimIDiag9,      (unsigned *) LADR_UnusedUInt);

//transformer saturation curve setting
    SET_FV(L_XfmrSatCurvNum,     10);
    SET_FV(L_XfmrSatCurvFlxK0,  0.0F);
    SET_FV(L_XfmrSatCurvFlxK1, 0.324129F);
    SET_FV(L_XfmrSatCurvFlxK2,     0.61284F);
    SET_FV(L_XfmrSatCurvFlxK3,     0.825118F);
    SET_FV(L_XfmrSatCurvFlxK4,  1.0F);
    SET_FV(L_XfmrSatCurvFlxK5, 1.08024F);
    SET_FV(L_XfmrSatCurvFlxK6,     1.17334F);
    SET_FV(L_XfmrSatCurvFlxK7,     1.26115F);
    SET_FV(L_XfmrSatCurvFlxK8,  1.36094F);
    SET_FV(L_XfmrSatCurvFlxK9, 1.49469F);

    SET_FV(L_XfmrSatCurvImagK0,  0.0F);
    SET_FV(L_XfmrSatCurvImagK1, 0.001774F);
    SET_FV(L_XfmrSatCurvImagK2,     0.00487637F);
    SET_FV(L_XfmrSatCurvImagK3,     0.00980856F);
    SET_FV(L_XfmrSatCurvImagK4,  0.02F);
    SET_FV(L_XfmrSatCurvImagK5, 0.0309543F);
    SET_FV(L_XfmrSatCurvImagK6,     0.0652348F);
    SET_FV(L_XfmrSatCurvImagK7,     0.20357F);
    SET_FV(L_XfmrSatCurvImagK8,  0.60512F);
    SET_FV(L_XfmrSatCurvImagK9, 1.24388F);

    SET_FV(L_LineXfmrSatEnb,  TRUE);
    SET_FV(L_LineXfmrSatCrvType, SAT_CURVE);
    SET_FV(L_LineXfmrSatThr,  1.18);
    SET_FV(L_LineXfmrSatXm, 1000);


    cVectorf::SineCosine(-PI/3.0, &L_SinCosFlxSim);
    cVectorf::RotateLN(-SimPlant1x.L_VlnSSInSim / PARM(L_LineFrq), 0.0F, &L_SinCosFlxSim, &TempFlx1, &TempFlx2);
    TempFlx3 = -(TempFlx1 + TempFlx2);
    SET_FV(L_XfmrFlxAInit, TempFlx1);
    SET_FV(L_XfmrFlxBInit,  TempFlx2);
    SET_FV(L_XfmrFlxCInit, TempFlx3);
SET_FV(L_LineRmagXfmrSim, 0.01);


//SET_FV(LvrtRPhsTypSim, 0);
SET_FV(LvrtRemoteTm, 0.01);
//SET_FV(LvrtLPhsTypSim, 0);
SET_FV(LvrtLocalTm, 0.01);
SET_FV(StartTmSim, 10);

//SET_FV(RShortEnb, 0);
//SET_FV(LShortEnb, 0);
//SET_FV(StartEnb, 0);
if(SimPlant1x.RShortEnb && (PARM(LvrtRPhsTypSim)==3 ||PARM(LvrtRPhsTypSim)==5 ||PARM(LvrtRPhsTypSim)==6))
{
    SET_FV(LLShort, 1);
}
else if(SimPlant1x.LShortEnb && (PARM(LvrtLPhsTypSim)==3 ||PARM(LvrtLPhsTypSim)==5 ||PARM(LvrtLPhsTypSim)==6))
{
    SET_FV(LLShort, 1);
}
else
{
    SET_FV(LLShort, 0);
}



SET_FV(ShortOpenThr, 1.0);
SET_FV(LStartSim, 1e-5);
SET_FV(RStartSim, 850e-6);

SET_FV(AngTest, 0);
SET_FV(L_AngMatch, PI /3.0);
SET_FV(R_AngMatch, 0.0);
SET_FV(AngMatchMargin, 0.005);


//SET_FV(HVRTEnb, 0);
SET_FV(HVRTTime, 0.0);


//SET_FV(HvrtPhsTypSim, 0);
SET_FV(HvrtLvlSim, 0.0);


//Series compensation MOV curve

SET_FV(SCMovCurVK0Sim, 0.0);
SET_FV(SCMovCurVK1Sim, 410.7717);
SET_FV(SCMovCurVK2Sim, 422.7665);
SET_FV(SCMovCurVK3Sim, 439.3061);
SET_FV(SCMovCurVK4Sim, 460.0099);
SET_FV(SCMovCurVK5Sim, 481.6895);
SET_FV(SCMovCurVK6Sim, 496.9380);
SET_FV(SCMovCurVK7Sim, 516.3697);
SET_FV(SCMovCurVK8Sim, 538.8529);
SET_FV(SCMovCurVK9Sim, 563.2744);
SET_FV(SCMovCurVK10Sim, 677.2977);

SET_FV(SCMovCurIK0Sim, 0.0);
SET_FV(SCMovCurIK1Sim, 0.016036);
SET_FV(SCMovCurIK2Sim, 0.160361);
SET_FV(SCMovCurIK3Sim, 1.60361);
SET_FV(SCMovCurIK4Sim, 16.0361);
SET_FV(SCMovCurIK5Sim, 160.361);
SET_FV(SCMovCurIK6Sim, 506.82);
SET_FV(SCMovCurIK7Sim, 1601.8);
SET_FV(SCMovCurIK8Sim, 5062.5);
SET_FV(SCMovCurIK9Sim, 16000.0);
SET_FV(SCMovCurIK10Sim, 400000.0);

SET_FV(SCMovCurvNumSim, 11);
SET_FV(SCMovEnb, 0);
SET_FV(SCManual, 0);
SET_FV(SCCmd, 0);

    SET_FV(LSCompSim, 5.0E-06);
    SET_FV(RhfCompSim, 50.0);
    SET_FV(RSwitchSim, 1.0);
    SET_FV(RSCompSim, PARM(LSCompSim)*PARM(L_LineFrq)/10.0);
    SET_FV(CSCompSim, 4.0E-03);

 
       L_FuseSim.RunRules();
    L_XfmrSim.RunRules();

    SeriesCmpSimPha.RunRules();
    SeriesCmpSimPhb.RunRules();
    SeriesCmpSimPhc.RunRules();
    
    LnRCSnubSim.RunRules();
    RSRCSnubSim.RunRules();
    LSRCSnubSim.RunRules();
    SCRCSnubSim.RunRules();

    RemoteShortSim.RunRules();
    LocalShortSim.RunRules();
    LineStartUpSim.RunRules();


    return (Status);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
//
// EXECUTION LEVEL
//      Initialization
//
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
int SimInit(void)
{
    int Status = SUCCESS;

    memset( (void*)(&L_BrgFbkSim), 0, sizeof(L_BrgFbkSim) );
    memset( (void*)(&R_BrgFbkSim), 0, sizeof(R_BrgFbkSim) );

    if ( XSimMode )
    {
        // pParmBuf = (uint32_t*)malloc(gParameter.TotalEntry()*sizeof(int32_t));

        memset( (void*)(&XsimDataFromCtl), 0, sizeof(XsimDataFromCtl) );
        memset( (void*)(&XsimDataToCtl),   0, sizeof(XsimDataToCtl)   );

        XsimInterface.InitDataPointers( (void*)&XsimDataFromCtl, sizeof(XsimDataFromCtl), (void*)&XsimDataToCtl, sizeof(XsimDataToCtl) );
    }

    UseXsimOutput = false;

    return Status;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Control the simulator states and load models
//
// EXECUTION LEVEL
//      Sim Task
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimStateControlT1(void)
{
    //-----------------------------------------------------------------
    // line side bridge states

    if ( L_BrgPwrEnbT1 == true )
    {
        L_BrgStateSimUnp = BRG_CONN_ENB;
    }
    else
    {
        L_BrgStateSimUnp = BRG_CONN_DSB;
    }

    L_BrgStateSim = (int)L_BrgStateSimUnp;      // update published version of state variables
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Control the simulator states and load models
//
// EXECUTION LEVEL
//      Sim Task
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimStateControlT1A(void)
{
    //-----------------------------------------------------------------
    // rotor side bridge states
    if ( R_CCOpnCmdT1 == true)
    {
        if (R_BrgPwrEnbT1 == true)
        {
            R_BrgStateSimUnp = BRG_CONN_ENB;
        }
        else
        {
            R_BrgStateSimUnp = BRG_CONN_DSB;
        }
    }
    else
    {
        if (R_BrgPwrEnbT1 == true)
        {
            R_BrgStateSimUnp = BRG_DISC_ENB;
        }
        else
        {
            R_BrgStateSimUnp = BRG_DISC_DSB;
        }
    }

    R_BrgStateSim = (int)R_BrgStateSimUnp;      // update published version of state variables

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void FirFbkT1()
{
    //-------------------------------------------------------------------------
    // accumulate the signals needed for FIR filter of bridge feedbacks
    L_BrgFbkSimBrg1Vdc +=  SimPlant1x.L_VdcSim;

    if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
    {
//      PreREBH:
        L_BrgFbkSimBrg1Vax +=  SimPlant1x.L_VabBrgSim;       //L_LineSimulator.VabBrgLnSim;
        L_BrgFbkSimBrg1Vbx +=  SimPlant1x.L_VbcBrgSim;       //L_LineSimulator.VbcBrgLnSim;
        L_BrgFbkSimBrg1Vcx +=  0.0F;
}
else
{
//      REBH:
        L_BrgFbkSimBrg1Vax +=  SimPlant1x.L_VanBrgSim;       //L_LineSimulator.VanBrgLnSim;
        L_BrgFbkSimBrg1Vbx +=  SimPlant1x.L_VbnBrgSim;       //L_LineSimulator.VbnBrgLnSim;
        L_BrgFbkSimBrg1Vcx +=  SimPlant1x.L_VcnBrgSim;       //L_LineSimulator.VcnBrgLnSim;
}

    L_BrgFbkSimBrg1Ia  +=  SimPlant1x.L_IaBrgSim;
    L_BrgFbkSimBrg1Ib  +=  SimPlant1x.L_IbBrgSim;
    L_BrgFbkSimBrg1Ic  +=  SimPlant1x.L_IcBrgSim;

    L_BrgFbkSimAin1Vax +=  SimPlant1x.L_VaXfmrSecSim;
    L_BrgFbkSimAin1Vbx +=  SimPlant1x.L_VbXfmrSecSim;
    L_BrgFbkSimAin1Vcx +=  SimPlant1x.L_VcXfmrSecSim;

    L_BrgFbkSimAin1Ia  += SimPlant1x.L_IaXfmrSecSim;
    L_BrgFbkSimAin1Ib  += SimPlant1x.L_IbXfmrSecSim;
    L_BrgFbkSimAin1Ic  += SimPlant1x.L_IcXfmrSecSim;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void FirFbkT1A()
{

    //-------------------------------------------------------------------------
    // accumulate the signals needed for FIR filter of bridge feedbacks
    // rotor side DC link voltage via the fiber-optic
    R_BrgFbkSimVdcFiber += SimPlant1x.L_VdcSim;

    // XFMR 6Kv winding voltage feedback
    R_BrgFbkSimAin1Vax += SimPlant1x.S_VabSim;
    R_BrgFbkSimAin1Vbx += SimPlant1x.S_VbcSim;
    R_BrgFbkSimAin1Vcx  = 0;
    // 6KCT current feedback
    R_BrgFbkSimAin1Ia  += SimPlant1x.S_IaSim;
    R_BrgFbkSimAin1Ib  += SimPlant1x.S_IbSim;
    R_BrgFbkSimAin1Ic  += SimPlant1x.S_IcSim;

    //R_BrgFbkSimBrg1Ia  += SimPlant1x.R_IaBrgSim;
    //R_BrgFbkSimBrg1Ib  += SimPlant1x.R_IbBrgSim;
    //R_BrgFbkSimBrg1Ic  += SimPlant1x.R_IcBrgSim;

    // Generator stator voltage feedback
    R_BrgFbkSimAin2Vax =   0.0F;
    R_BrgFbkSimAin2Vbx =   0.0F;
    R_BrgFbkSimAin2Vcx =   0.0F;
    // Generator distortion filter currents (not implemented yet)
    R_BrgFbkSimAin2Ia  = 0.0F;
    R_BrgFbkSimAin2Ib  = 0.0F;
    R_BrgFbkSimAin2Ic  = 0.0F;

    R_BrgFbkSimBrg1Ia  += SimPlant1x.R_IaBrgSim;
    R_BrgFbkSimBrg1Ib  += SimPlant1x.R_IbBrgSim;
    R_BrgFbkSimBrg1Ic  += SimPlant1x.R_IcBrgSim;


    R_BrgFbkSimBrg1Vax += SimPlant1x.R_VabBrgSim;
    R_BrgFbkSimBrg1Vbx += SimPlant1x.R_VbcBrgSim;
    R_BrgFbkSimBrg1Vcx += 0.0F;

}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimFbkCopyT1()
{
    //---------------------------------------------------------------------
    if ( DsimMode )
    {
        CoolntTempSim = PARM(CoolEnbSimRq) ? CoolntTempCalc : PARM(CoolantTempSim);
        // run thermistor simulator
        ThrmstrSim.Task();
    }
    else
    {
        //---------------------------------------------------------------------
        // use the simulated signals from Xsim or Isim
        if ( UseXsimOutput )
        {
            //---------------------------------------------------------------------
            // use the simulated signals from Xsim
            L_BrgFbkSim = XsimDataToCtl.L_BrgFbkSim;
            if(PARM(SoftwareIPN) < DFIG02_1500_5060_B)
            {
                L_BrgFbkSim.Brg1.Vdc = L_BrgFbkSim.VdcFiber;
            }
        }
        else

        {
            L_Brg1FbkSimVax = L_BrgFbkSim.Brg1.Vax;
            L_Brg1FbkSimVbx = L_BrgFbkSim.Brg1.Vbx;
            L_Brg1FbkSimVcx = L_BrgFbkSim.Brg1.Vcx;
            L_Ain1FbkSimVax = L_BrgFbkSim.Ain1.Vax;
            L_Ain1FbkSimVbx = L_BrgFbkSim.Ain1.Vbx;
            L_Ain1FbkSimVcx = L_BrgFbkSim.Ain1.Vcx;
            L_Brg1FbkSimVdc = L_BrgFbkSim.Brg1.Vdc;
            L_FibrFbkSimVdc  = L_BrgFbkSim.VdcFiber;
            //---------------------------------------------------------------------
            // use the simulated signals from Isim
            // voltage across the DB1 resistor (not using FIR)
            if(PARM(SoftwareIPN) >= DFIG02_1500_5060_B)
            {
                L_BrgFbkSim.Brg1.Vdc = L_DbSimulator.DbVr1Sim;
                L_VdbFbkSim = L_DbSimulator.DbVr1Sim;
            }

            IdbTot = L_DbSimulator.DbI1Sim;
        }
    }

    L_BrgFbkSim.ThrmstrBrg             = ThrmstrSim.Brg1BpFrqSim;
    L_BrgFbkSim.ThrmstrCoolnt.LwRngFrq = ThrmstrSim.CoolntFrqLwSim;
    L_BrgFbkSim.ThrmstrCoolnt.HiRngFrq = ThrmstrSim.CoolntFrqHiSim;

    //---------------------------------------------------------------------
    // convert bridge feedbacks into Vco counts for the control
    L_BridgeRebf.SimWriteFbk( L_BrgFbkSim );

    // transfer Sim faults to Brg structure
//  L_BridgeRebf.SimWriteFlt( L_BrgFltSim );

    //-------------------------------------------------------------------------
    // done
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void DsimFbkCopyT1()
{
    SimFbkCopyT1();
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimFbkCopyT1A()
{
    //---------------------------------------------------------------------
    if ( DsimMode )
    {
        //all feedbacks transferred at the Task1 interface
    }
    else
    {
        //---------------------------------------------------------------------
        // use the simulated signals from Xsim
        if ( UseXsimOutput )
        {
            R_BrgFbkSim = XsimDataToCtl.R_BrgFbkSim;
        }
        else
        {
            //---------------------------------------------------------------------
            // use the simulated signals from Isim

            R_Brg1FbkSimVax = R_BrgFbkSim.Brg1.Vax;
            R_Brg1FbkSimVbx = R_BrgFbkSim.Brg1.Vbx;
            R_Brg1FbkSimVcx = R_BrgFbkSim.Brg1.Vcx;
            R_Ain1FbkSimVax = R_BrgFbkSim.Ain1.Vax;
            R_Ain1FbkSimVbx = R_BrgFbkSim.Ain1.Vbx;
            R_Ain1FbkSimVcx = R_BrgFbkSim.Ain1.Vcx;
            R_Brg1FbkSimVdc = R_BrgFbkSim.Brg1.Vdc;
            // already copied in SimFbkSampleT1A
            R_BrgFbkSim.TachV  = 0.0F;
            R_BrgFbkSim.TachMv = 0.0F;

            if ( R_BrgStateSimUnp == BRG_DISC_DSB )
            {
                if ( !R_SeqStBitsT1.R_Running && !R_SeqStBitsT1.R_PulseTest )
                {
                    R_BrgFbkSim.Brg1.Vax = 0.0F;
                    R_BrgFbkSim.Brg1.Vbx = 0.0F;
                    R_BrgFbkSim.Brg1.Vcx = 0.0F;
                }

                R_BrgFbkSim.Brg1.Ia = 0.0F;
                R_BrgFbkSim.Brg1.Ib = 0.0F;
                R_BrgFbkSim.Brg1.Ic = 0.0F;
            }
        }
    }

    // added by JL, 20150803
    R_BrgFbkSim.ThrmstrBrg             = ThrmstrSim.Brg2BpFrqSim;

    //---------------------------------------------------------------------
    // convert bridge feedbacks into Vco counts for the control
    R_BridgeRebf.SimWriteFbk( R_BrgFbkSim );

    // transfer Sim faults to Brg structure
//  R_BridgeRebf.SimWriteFlt( R_BrgFltSim );

    //-------------------------------------------------------------------------
    // done
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      Task1A
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void DsimFbkCopyT1A()
{
    SimFbkCopyT1A();
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimFbkSampleT1()
{
    L_VdcFbkSim = L_BrgFbkSimBrg1Vdc * L_IsimExeNRcp;
    if(PARM(SoftwareIPN) <= DFIG02_1500_5060_A)
    {

        L_BrgFbkSim.Brg1.Vdc = L_VdcFbkSim;
    }
    else
    {
        L_BrgFbkSim.VdcFiber = L_VdcFbkSim;
    }

    L_BrgFbkSim.Brg1.Vax =  L_BrgFbkSimBrg1Vax * L_IsimExeNRcp;    //L_LineSimulator.VaBrgLnSim;
    L_BrgFbkSim.Brg1.Vbx =  L_BrgFbkSimBrg1Vbx * L_IsimExeNRcp;    //L_LineSimulator.VaBrgLnSim;
    L_BrgFbkSim.Brg1.Vcx =  L_BrgFbkSimBrg1Vcx * L_IsimExeNRcp;

    L_BrgFbkSim.Brg1.Ia  =  L_BrgFbkSimBrg1Ia  * L_IsimExeNRcp;
    L_BrgFbkSim.Brg1.Ib  =  L_BrgFbkSimBrg1Ib  * L_IsimExeNRcp;
    L_BrgFbkSim.Brg1.Ic  =  L_BrgFbkSimBrg1Ic  * L_IsimExeNRcp;

    L_BrgFbkSim.Ain1.Vax =  L_BrgFbkSimAin1Vax * L_IsimExeNRcp;
    L_BrgFbkSim.Ain1.Vbx =  L_BrgFbkSimAin1Vbx * L_IsimExeNRcp;
    L_BrgFbkSim.Ain1.Vcx =  L_BrgFbkSimAin1Vcx * L_IsimExeNRcp;

    L_BrgFbkSim.Ain1.Ia  =  L_BrgFbkSimAin1Ia  * L_IsimExeNRcp;
    L_BrgFbkSim.Ain1.Ib  =  L_BrgFbkSimAin1Ib  * L_IsimExeNRcp;
    L_BrgFbkSim.Ain1.Ic  =  L_BrgFbkSimAin1Ic  * L_IsimExeNRcp;

    L_BrgFbkSimBrg1Vdc   = 0.0F;
    L_BrgFbkSimBrg1Vax   = 0.0F;
    L_BrgFbkSimBrg1Vbx   = 0.0F;
    L_BrgFbkSimBrg1Vcx   = 0.0F;
    L_BrgFbkSimBrg1Ia    = 0.0F;
    L_BrgFbkSimBrg1Ib    = 0.0F;
    L_BrgFbkSimBrg1Ic    = 0.0F;
    L_BrgFbkSimAin1Vax   = 0.0F;
    L_BrgFbkSimAin1Vbx   = 0.0F;
    L_BrgFbkSimAin1Vcx   = 0.0F;
    L_BrgFbkSimAin1Ia    = 0.0F;
    L_BrgFbkSimAin1Ib    = 0.0F;
    L_BrgFbkSimAin1Ic    = 0.0F;

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimFbkSampleT1A()
{

    // rotor side DC link voltage via the fiber-optic
    R_VdcFbkSim = R_BrgFbkSimVdcFiber * R_IsimExeNRcp;
    R_BrgFbkSim.Brg1.Vdc = R_VdcFbkSim;

    // XFMR 6Kv winding voltage feedback
    if ( PARM(SoftwareIPN) <  DFIG02_1850_5060_E)
    {
        R_BrgFbkSim.Ain1.Vax = R_BrgFbkSimAin1Vax * R_IsimExeNRcp;
        R_BrgFbkSim.Ain1.Vbx = R_BrgFbkSimAin1Vbx * R_IsimExeNRcp;
        R_BrgFbkSim.Ain1.Vcx = R_BrgFbkSimAin1Vcx * R_IsimExeNRcp;
    }
    else
    {
        float tempVab;
        float tempVbc;
        tempVab = R_BrgFbkSimAin1Vax * R_IsimExeNRcp;
        tempVbc = R_BrgFbkSimAin1Vbx * R_IsimExeNRcp;
        R_BrgFbkSim.Ain1.Vax = ( 2.0F/3.0F) * tempVab + ( 1.0F/3.0F) * tempVbc;
        R_BrgFbkSim.Ain1.Vbx = (-1.0F/3.0F) * tempVab + ( 1.0F/3.0F) * tempVbc;
        R_BrgFbkSim.Ain1.Vcx = -R_BrgFbkSim.Ain1.Vax -R_BrgFbkSim.Ain1.Vbx;
    }
    // 6KCT current feedback
    R_BrgFbkSim.Ain1.Ia  = R_BrgFbkSimAin1Ia  * R_IsimExeNRcp;
    R_BrgFbkSim.Ain1.Ib  = R_BrgFbkSimAin1Ib  * R_IsimExeNRcp;
    R_BrgFbkSim.Ain1.Ic  = R_BrgFbkSimAin1Ic  * R_IsimExeNRcp;
    // Generator stator voltage feedback
    R_BrgFbkSim.Ain2.Vax = R_BrgFbkSimAin2Vax * R_IsimExeNRcp;
    R_BrgFbkSim.Ain2.Vbx = R_BrgFbkSimAin2Vbx * R_IsimExeNRcp;
    R_BrgFbkSim.Ain2.Vcx = R_BrgFbkSimAin2Vcx * R_IsimExeNRcp;
    // Generator distortion filter currents (not implemented yet)
    R_BrgFbkSim.Ain2.Ia  = R_BrgFbkSimAin2Ia  * R_IsimExeNRcp;
    R_BrgFbkSim.Ain2.Ib  = R_BrgFbkSimAin2Ib  * R_IsimExeNRcp;
    R_BrgFbkSim.Ain2.Ic  = R_BrgFbkSimAin2Ic  * R_IsimExeNRcp;

    R_BrgFbkSim.Brg1.Ia  = R_BrgFbkSimBrg1Ia  * R_IsimExeNRcp;
    R_BrgFbkSim.Brg1.Ib  = R_BrgFbkSimBrg1Ib  * R_IsimExeNRcp;
    R_BrgFbkSim.Brg1.Ic  = R_BrgFbkSimBrg1Ic  * R_IsimExeNRcp;

    R_BrgFbkSim.Brg1.Vax = R_BrgFbkSimBrg1Vax * R_IsimExeNRcp;
    R_BrgFbkSim.Brg1.Vbx = R_BrgFbkSimBrg1Vbx * R_IsimExeNRcp;
    R_BrgFbkSim.Brg1.Vcx = R_BrgFbkSimBrg1Vcx * R_IsimExeNRcp;

    R_BrgFbkSimVdcFiber  = 0.0F;
    R_BrgFbkSimAin1Vax   = 0.0F;
    R_BrgFbkSimAin1Vbx   = 0.0F;
    R_BrgFbkSimAin1Vcx   = 0.0F;
    R_BrgFbkSimAin1Ia    = 0.0F;
    R_BrgFbkSimAin1Ib    = 0.0F;
    R_BrgFbkSimAin1Ic    = 0.0F;
    R_BrgFbkSimAin2Vax   = 0.0F;
    R_BrgFbkSimAin2Vbx   = 0.0F;
    R_BrgFbkSimAin2Vcx   = 0.0F;
    R_BrgFbkSimAin2Ia    = 0.0F;
    R_BrgFbkSimAin2Ib    = 0.0F;
    R_BrgFbkSimAin2Ic    = 0.0F;
    R_BrgFbkSimBrg1Ia    = 0.0F;
    R_BrgFbkSimBrg1Ib    = 0.0F;
    R_BrgFbkSimBrg1Ic    = 0.0F;
    R_BrgFbkSimBrg1Vax   = 0.0F;
    R_BrgFbkSimBrg1Vbx   = 0.0F;
    R_BrgFbkSimBrg1Vcx   = 0.0F;

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimCmdCopyT1()
{
    //-------------------------------------------------------------------------
    // read bridge commands from the control and synthesize average bridge output
    // votlages for the internal simulator - delay the commands for one task1

    //L_BrgCmdVavgSimZ1.Brg1 = L_BrgCmdVavgSim.Brg1;    // copy one bridge
    // Vdc used here is delayed 1 more task than could be
    //L_BridgeRebf.SimReadCmdVavg( L_BrgCmdVavgSim , L_VdcFbkSim);

    //-------------------------------------------------------------------------
    if (DsimMode)
    {
        // Read Bridge commands from the control
        L_BridgeRebf.SimReadCmdTra( L_BrgCmdTraSim );
    }
    //-------------------------------------------------------------------------

    L_BrgDbCmdSimZ1 = L_BrgDbCmdSim;
    L_BrgDbCmdSim.Db1GateCmd = L_DbCmd;
    DbGateCmd = L_BrgDbCmdSimZ1.Db1GateCmd;


    L_BridgeRebf.SimPwmTm();

    L_BrgPhAPwmUpTmZ1 = L_BrgPhAPwmUpTm;
    L_BrgPhAPwmDnTmZ1 = L_BrgPhAPwmDnTm;
    L_BrgPhBPwmUpTmZ1 = L_BrgPhBPwmUpTm;
    L_BrgPhBPwmDnTmZ1 = L_BrgPhBPwmDnTm;
    L_BrgPhCPwmUpTmZ1 = L_BrgPhCPwmUpTm;
    L_BrgPhCPwmDnTmZ1 = L_BrgPhCPwmDnTm;

    L_BrgPhAPwmUpTm = (int) L_BridgeRebf.SimPhAUpTm;
    L_BrgPhAPwmDnTm = (int) L_BridgeRebf.SimPhADnTm;
    L_BrgPhBPwmUpTm = (int) L_BridgeRebf.SimPhBUpTm;
    L_BrgPhBPwmDnTm = (int) L_BridgeRebf.SimPhBDnTm;
    L_BrgPhCPwmUpTm = (int) L_BridgeRebf.SimPhCUpTm;
    L_BrgPhCPwmDnTm = (int) L_BridgeRebf.SimPhCDnTm;

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimCmdCopyT1A(void)
{

    //R_BrgCmdVavgSimZ1 = R_BrgCmdVavgSim;                // copy both bridges
    //R_BridgeRebf.SimReadCmdVavg( R_BrgCmdVavgSim , R_VdcFbkSim);

    //R_BrgVavgApprxSim.Va   = R_BrgCmdVavgSimZ1.Brg1.Va;
    //R_BrgVavgApprxSim.Vb   = R_BrgCmdVavgSimZ1.Brg1.Vb;
    //R_BrgVavgApprxSim.Vc   = R_BrgCmdVavgSimZ1.Brg1.Vc;

    // Transition Commands
    if (DsimMode)
    {
        R_BridgeRebf.SimReadCmdTra( R_BrgCmdTraSim );
    }

    R_BridgeRebf.SimPwmTm();

    R_BrgPhAPwmUpTmZ1 = R_BrgPhAPwmUpTm;
    R_BrgPhAPwmDnTmZ1 = R_BrgPhAPwmDnTm;
    R_BrgPhBPwmUpTmZ1 = R_BrgPhBPwmUpTm;
    R_BrgPhBPwmDnTmZ1 = R_BrgPhBPwmDnTm;
    R_BrgPhCPwmUpTmZ1 = R_BrgPhCPwmUpTm;
    R_BrgPhCPwmDnTmZ1 = R_BrgPhCPwmDnTm;

    R_BrgPhAPwmUpTm = (int) R_BridgeRebf.SimPhAUpTm;
    R_BrgPhAPwmDnTm = (int) R_BridgeRebf.SimPhADnTm;
    R_BrgPhBPwmUpTm = (int) R_BridgeRebf.SimPhBUpTm;
    R_BrgPhBPwmDnTm = (int) R_BridgeRebf.SimPhBDnTm;
    R_BrgPhCPwmUpTm = (int) R_BridgeRebf.SimPhCUpTm;
    R_BrgPhCPwmDnTm = (int) R_BridgeRebf.SimPhCDnTm;


    //-------------------------------------------------------------------------
    // done
    return;
}





///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//        Calculate phase voltage command per Isim period
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////

float SimPhasePwmVolt(unsigned i, int UpTm, int DnTm, int TmCnt, float TmCntRcp)
{
    int     ThisTm;
    int     NextTm;
    int     OffTm1;
    int     OffTm2;
    int     OffTm;
    int     OnTm;
    float     pwm;

    ThisTm = i * TmCnt;
    NextTm = (i + 1) * TmCnt;

    OffTm1 = UpTm - ThisTm;
    if( OffTm1 < 0)
    {
        OffTm1 = 0;
    }

    OffTm2 = NextTm - DnTm;
    if( OffTm2 < 0)
    {
        OffTm2 = 0;
    }


    OffTm = OffTm1 + OffTm2;
    if(OffTm > TmCnt)
    {
        OffTm = TmCnt;
    }

    OnTm = TmCnt - OffTm;
    pwm =  OnTm * TmCntRcp  - 0.5;
    return pwm * L_VdcFbkSim;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimCmdCopyDbT1A()
{
    // comment out due to removeal of batt code - JWT
    //-------------------------------------------------------------------------
    // Battery chopper commands for the internal simulator - delay the commands for one task1A
    // if( R_BrgChopEnb )
    // {
    //     R_BatCmdSimZ1 = R_BatCmdSim;
    //     R_BatteryCtl.SimReadBatCmd ( R_BatCmdSim );
    // }
    //-------------------------------------------------------------------------
    // done
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimOutRlyControl(void)
{
    XsimDataFromCtl.T1StFromCtl   = T1St;
#ifdef XSIM_BUILD
    XsimDataFromCtl.RxPacketCtr   = XsimInterface.GetXsimRxPacketCtr();//Xsim Package Count
#else
    XsimDataFromCtl.RxPacketCtr   = 0;
#endif


    XsimDataFromCtl.XsimOutRly.L_KDFClsCmd = KdfClsCmd;
    XsimDataFromCtl.XsimOutRly.L_Q1ClsPulse = Q1ClsIp;//Q1ClsPulse;
    XsimDataFromCtl.XsimOutRly.S_K1ClsCmd    = S_K1ClsCmd;
    XsimDataFromCtl.XsimOutRly.R_OVClsCmd    = R_CCOpnCmdT1;

    //Xsim Debug Information
    XsimDataFromCtl.TripFlt = TripFlt;

    for (int i = 0; i < TRIPLOGNUM; i++)
    {
#ifdef XSIM_BUILD
        XsimDataFromCtl.XsimTripLog[i] = MarkStatDiag.TripLogXsim[i].TripCode;
#else
        XsimDataFromCtl.XsimTripLog[i] = 0;
#endif
    }

}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
// Input : cBrgThrm5pModel BrgThrm5pModel
// Output: cBridgeRebf::BrgBtmTempSimTyp& BrgBtmTempSim
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimOutTemp(void)
{
    L_BrgThrm5pModel.SimReadTemp(XsimDataFromCtl.L_BtmTempSim);
    R_BrgThrm5pModel.SimReadTemp(XsimDataFromCtl.R_BtmTempSim);
    L_DbThrmModel.SimReadTemp(XsimDataFromCtl.DtmTempSim);
    XsimDataFromCtl.DbTmpEst     = L_DbTmpEst;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimOutDiagMap(void)
{
    // XSim Debug Variables
    XsimDataFromCtl.Diag[1] = (float)GmPllState;
    XsimDataFromCtl.Diag[2] = (float)GmPllProven;
    XsimDataFromCtl.Diag[3] = GmPllErrInt;
    XsimDataFromCtl.Diag[4] = (float)GmPllErrHi;
    XsimDataFromCtl.Diag[5] = GmVMagPuT2;
    XsimDataFromCtl.Diag[6] = GmVMagGridPu;
    XsimDataFromCtl.Diag[7] = (float)L_RunRdy;
    XsimDataFromCtl.Diag[8] = (float)R_RunRdy;
    XsimDataFromCtl.Diag[9] = (float)SeqSt;
    XsimDataFromCtl.Diag[10] = (float)L_SeqSt;
    XsimDataFromCtl.Diag[12] = (float)R_SeqSt;
    XsimDataFromCtl.Diag[13] = (float)TripFlt;
    XsimDataFromCtl.Diag[14] = (float)KdfTypeSt;
    XsimDataFromCtl.Diag[15] = (float)L_KdfTskSt;
    XsimDataFromCtl.Diag[16] = (float)Q1TypeSt;
    XsimDataFromCtl.Diag[17] = (float)L_Q1TskSt;
    XsimDataFromCtl.Diag[18] = (float)R_SyncTskSt;
    XsimDataFromCtl.Diag[19] = GmPllW;
    XsimDataFromCtl.Diag[20] = PwrPllWFFT1;
    XsimDataFromCtl.Diag[21] = GmAngElecFbk;
    XsimDataFromCtl.Diag[22] = GmPllWRAFT1;
    XsimDataFromCtl.Diag[23] = GmPllWInt;
    XsimDataFromCtl.Diag[24] = L_VdcSetPt;
    XsimDataFromCtl.Diag[25] = L_VregRef;
    XsimDataFromCtl.Diag[26] =  L_VregErr;
    XsimDataFromCtl.Diag[27] =  L_IxCmd;
    XsimDataFromCtl.Diag[28] = L_IxFbk;
    XsimDataFromCtl.Diag[29] = L_IyCmd;
    XsimDataFromCtl.Diag[30] = L_IyFbk;
    XsimDataFromCtl.Diag[31] = L_UxCmd;
    XsimDataFromCtl.Diag[32] = L_UyCmd;
    XsimDataFromCtl.Diag[33] = L_ModIndex;
    XsimDataFromCtl.Diag[34] = S_PllWInt;
    XsimDataFromCtl.Diag[35] = S_VrefScl;
    XsimDataFromCtl.Diag[36] = L_IocPhsA;
    XsimDataFromCtl.Diag[37] = L_IocPhsB;
    XsimDataFromCtl.Diag[38] = L_IocPhsC;
    XsimDataFromCtl.Diag[39] = S_PllW;
    XsimDataFromCtl.Diag[40] = S_PllVyErr;
    XsimDataFromCtl.Diag[41] = S_VregErr;
    XsimDataFromCtl.Diag[42] = S_VregOut;
    XsimDataFromCtl.Diag[43] = (float)L_BridgeRebf.FpgaFltLatch;
    XsimDataFromCtl.Diag[44] = (float)R_BridgeRebf.FpgaFltLatch;
    XsimDataFromCtl.Diag[45] = L_DbCtl.DbcVregRef;
    XsimDataFromCtl.Diag[46] = (float)R_BrgCroBarCtl.CroBarRst;
    XsimDataFromCtl.Diag[47] = (float)R_BrgFltRstCmd;
    XsimDataFromCtl.Diag[48] = (float)R_SeqStopped;
    XsimDataFromCtl.Diag[49] = (float)R_BrgCroBarCtl.CroBarAct;
    XsimDataFromCtl.Diag[50] = (float)HVACStAct;
    XsimDataFromCtl.Diag[51] = (float)HVACState;
    XsimDataFromCtl.Diag[52] = S_IyCmd;
    XsimDataFromCtl.Diag[53] = S_IyFbkT2;
    XsimDataFromCtl.Diag[54] = L_IcBrg;
    XsimDataFromCtl.Diag[55] = R_IyCmd;
    XsimDataFromCtl.Diag[56] = R_IyFbk;
    XsimDataFromCtl.Diag[57] = R_IxCmd;
    XsimDataFromCtl.Diag[58] = R_IxFbk;
    XsimDataFromCtl.Diag[59] = IyPFCmdPu;
    XsimDataFromCtl.Diag[60] = gVariable.ReadAsFloat(PARM(XsimFDiag0));
    XsimDataFromCtl.Diag[61] = gVariable.ReadAsFloat(PARM(XsimFDiag1));
    XsimDataFromCtl.Diag[62] = gVariable.ReadAsFloat(PARM(XsimFDiag2));
    XsimDataFromCtl.Diag[63] = gVariable.ReadAsFloat(PARM(XsimFDiag3));
    XsimDataFromCtl.Diag[64] = gVariable.ReadAsFloat(PARM(XsimFDiag4));
    XsimDataFromCtl.Diag[65] = gVariable.ReadAsFloat(PARM(XsimFDiag5));
    XsimDataFromCtl.Diag[66] = gVariable.ReadAsFloat(PARM(XsimFDiag6));
    XsimDataFromCtl.Diag[67] = gVariable.ReadAsFloat(PARM(XsimFDiag7));
    XsimDataFromCtl.Diag[68] = gVariable.ReadAsFloat(PARM(XsimFDiag8));
    XsimDataFromCtl.Diag[69] = gVariable.ReadAsFloat(PARM(XsimFDiag9));
    XsimDataFromCtl.Diag[70] = (float)gVariable.ReadAsInt(PARM(XsimIDiag0));
    XsimDataFromCtl.Diag[71] = (float)gVariable.ReadAsInt(PARM(XsimIDiag1));
    XsimDataFromCtl.Diag[72] = (float)gVariable.ReadAsInt(PARM(XsimIDiag2));
    XsimDataFromCtl.Diag[73] = (float)gVariable.ReadAsInt(PARM(XsimIDiag3));
    XsimDataFromCtl.Diag[74] = (float)gVariable.ReadAsInt(PARM(XsimIDiag4));
    XsimDataFromCtl.Diag[75] = (float)gVariable.ReadAsBool(PARM(XsimIDiag5));
    XsimDataFromCtl.Diag[76] = (float)gVariable.ReadAsBool(PARM(XsimIDiag6));
    XsimDataFromCtl.Diag[77] = (float)gVariable.ReadAsBool(PARM(XsimIDiag7));
    XsimDataFromCtl.Diag[78] = (float)gVariable.ReadAsBool(PARM(XsimIDiag8));
    XsimDataFromCtl.Diag[79] = (float)gVariable.ReadAsBool(PARM(XsimIDiag9));

}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void XsimDoeAutomation(void)
{
    static int InternalDoeState=0;
    XsimDoeCmd = XsimDataToCtl.XsimDoeCmd;

    if(XsimDoeCmd == -1) //hardware reset
    {
        //NV.PLCResetFlg = true;
        //Hard_Reset = true; // defined in p0Sseq.h
#ifdef DSIM_MODE
#else
        sysmgr_reboot();
#endif
    }
    else if (XsimDoeCmd == -2) //software reset
    {
        MarkStatDiag.ResetDiag();
        InternalDoeState=0;
    }
    else
    {
        if(XsimDoeCmd!=2) InternalDoeState=1;
        static int XsimDoeCmdZ=0;
        //2:Stop 3:Run 4:RPNW(toggle) 5:PulseTest 7 VarModeReq
        //pSequencer = &CoreCommonAdlSeq; already done
        if(XsimDoeCmd!=XsimDoeCmdZ)
        {
#ifdef DSIM_MODE
#else
            pSequencer->AdlCmdServer(XsimDoeCmd,0,NULL);
#endif
            XsimDoeCmdZ=XsimDoeCmd;
        }
    }
    if(InternalDoeState==0 && TripFlt)
    {
        MarkStatDiag.ResetDiag();
    }
    XsimVoltVarEnb = (unsigned)XsimDataToCtl.XsimDoeSp.VoltVarEnb;
    R_TrqRefXsim = XsimDataToCtl.XsimDoeSp.R_TrqRef*(1.0F/1000.F);
    R_VARRefXsim = XsimDataToCtl.XsimDoeSp.R_VARRef*(1.0F/1000.F);
    R_PhiAdjustXsim = XsimDataToCtl.XsimDoeSp.R_PhiAdjust;
    L_VARRefXsim = XsimDataToCtl.XsimDoeSp.L_VARRef*(1.0F/1000.F);

}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
// Line Converter IOC Retry
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void SimLineIocRetry(void)
{

    if( SimPlant1x.L_IocPhsASim == true )
    {
        SimPlant1x.L_IocPhsASim = false;
    }
    if( SimPlant1x.L_IocPhsBSim == true )
    {
        SimPlant1x.L_IocPhsBSim = false;
    }
    if( SimPlant1x.L_IocPhsCSim == true )
    {
        SimPlant1x.L_IocPhsCSim = false;
    }

}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      SimTask
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void Task1S( unsigned Sim1ARunReq, unsigned Sim1AFbkReq )
{

    IsimTimeClk += DelTm1;
    if( IsimTimeClk >= 4095)
    {
        IsimTimeClk = 4095;
    }
    //-------------------------------------------------------------------------
    // control the simulator states and load models (common to both isim and xsim)
    SimStateControlT1();
    if ( Sim1ARunReq )
    {
        SimStateControlT1A();
    }

    //-------------------------------------------------------------------------
    // choose the voltages to apply to the plant model
    SimCmdCopyT1();

    if ( DsimMode )
    {
        if ( Sim1ARunReq )
        {
            SimCmdCopyT1A();
        }
        // Abort for DSIM mode
        return;    // *ABORT procedure
    }

//  else
//  {
        SimCmdCopyT1A();
        if ( Sim1ARunReq )
        {
            SimCmdCopyDbT1A();
        }
    //-------------------------------------------------------------------------
    // call the Isim models
    if ( !R_IsimSmplCntRst && (T1St == 0) )
    {
        R_IsimSmplCntr = 0;
        R_IsimSmplCntRst = true;
    }

    //-------------------------------------------------------------------------
    //Reset IOC, then retry
    SimLineIocRetry();

    //-------------------------------------------------------------------------
    // run the plant models (> once to improve convergence with fast time constants)
    for ( unsigned i = 0; i < PARM(L_IsimExeN); i++ )
    {

        if (R_IsimSmplCntr == 0)
        {
            R_BrgPhAPwmUpTmAct = R_BrgPhAPwmUpTmZ1;
            R_BrgPhAPwmDnTmAct = R_BrgPhAPwmDnTmZ1;
            R_BrgPhBPwmUpTmAct = R_BrgPhBPwmUpTmZ1;
            R_BrgPhBPwmDnTmAct = R_BrgPhBPwmDnTmZ1;
            R_BrgPhCPwmUpTmAct = R_BrgPhCPwmUpTmZ1;
            R_BrgPhCPwmDnTmAct = R_BrgPhCPwmDnTmZ1;
        }

        L_IsimSmplCntr = i;
        L_BrgPhACmdVolt  =  SimPhasePwmVolt (L_IsimSmplCntr, L_BrgPhAPwmUpTmZ1, L_BrgPhAPwmDnTmZ1, L_IsimTmCnt, L_IsimTmCntRcp);
        L_BrgPhBCmdVolt  =  SimPhasePwmVolt (L_IsimSmplCntr, L_BrgPhBPwmUpTmZ1, L_BrgPhBPwmDnTmZ1, L_IsimTmCnt, L_IsimTmCntRcp);
        L_BrgPhCCmdVolt  =  SimPhasePwmVolt (L_IsimSmplCntr, L_BrgPhCPwmUpTmZ1, L_BrgPhCPwmDnTmZ1, L_IsimTmCnt, L_IsimTmCntRcp);

        R_BrgPhACmdVolt  =  SimPhasePwmVolt (R_IsimSmplCntr, R_BrgPhAPwmUpTmAct, R_BrgPhAPwmDnTmAct, R_IsimTmCnt, R_IsimTmCntRcp);
        R_BrgPhBCmdVolt  =  SimPhasePwmVolt (R_IsimSmplCntr, R_BrgPhBPwmUpTmAct, R_BrgPhBPwmDnTmAct, R_IsimTmCnt, R_IsimTmCntRcp);
        R_BrgPhCCmdVolt  =  SimPhasePwmVolt (R_IsimSmplCntr, R_BrgPhCPwmUpTmAct, R_BrgPhCPwmDnTmAct, R_IsimTmCnt, R_IsimTmCntRcp);


        SimPlant1x.Task1();             // run line-side plant simulation
        SimPlant1x.Task1A();            // run rotor-side plant simulation
        FirFbkT1();                     // integrate line-side fdbks
        FirFbkT1A();                    // integrate rotor-side fdbks

        L_FuseSim.FuseModel3P();
        FuseMeltIn = L_FuseSim.FuseMeltSim;
        FuseOpenIn = L_FuseSim.FuseOpenSim;
        FuseArcIn = L_FuseSim.FuseArcSim;
        FuseVaIn = L_FuseSim.VoutASim;
        FuseVbIn = L_FuseSim.VoutBSim;
        FuseVcIn = L_FuseSim.VoutCSim;


        Q1SwitchSim.SwitchModel();
        K1SwitchSim.SwitchModel();
        KdfSwitchSim.SwitchModel();
        KdfClsFbk    = KdfSwitchSim.SwitchState;
        K1StateSim   = K1SwitchSim.SwitchState;
        Q1StateSim   = Q1SwitchSim.SwitchState;
        Q1TripFbk    = false;
        FastBlkReqHw = false;
        Ps24vOk      = true;

        if (++R_IsimSmplCntr == PARM(R_IsimExeN))
        {
            SimFbkSampleT1A();          // sample rotor-side feedbacks when simulation time equals Task1A
            R_IsimSmplCntr = 0;         // reset rotor sample counter

            //SimCmdCopyT1A();            // get the Vcmds for the next T1A frame
        }

    }
    L_DbSimulator.Task1();
    SimFbkSampleT1();                   // sample line-side feedbacks

    if ( Sim1ARunReq )
    {
        // run DB task1

        // comment out due to removeal of batt code - JWT
        // run Battery simulator task1
        //  if( R_BrgChopEnb )
        //  {
        //    R_BatSimulator.Task1();
        //  }

        // update simulated mechanical speed in rad/sec
        R_SpdRefSimUf = ( gVariable.ReadAsFloat(PARM(R_SpdAdrSim)) + (gVariable.ReadAsFloat(PARM(R_SpdAAdrSim))) ) * RPM_TO_RAD;
        R_SpdRefSim += R_SpdRefSimGn * (R_SpdRefSimUf - R_SpdRefSim);

        CoolntTempSim = PARM(CoolEnbSimRq) ? CoolntTempCalc : PARM(CoolantTempSim);

        // run thermistor simulator
        ThrmstrSim.Task();

        // execute plant model
    }

    //-------------------------------------------------------------------------
    // if Xsim is enabled
    if ( XSimMode )
    {
        // read bridge commands from the control and synthesize bridge transition commands for the external simulator
        // delay the commands for one task1 (task1A)
        L_BridgeRebf.SimReadCmdTra( L_BrgCmdTraSim );
        XsimDataFromCtl.L_BrgCmdTraSimZ1 = L_BrgCmdTraSim;
        L_BridgeRebf.SimReadDbCmd( L_BrgDbCmdSim );
        XsimDataFromCtl.L_BrgDbCmdSimZ1 = L_BrgDbCmdSim;
        if ( Sim1ARunReq )
        {
            R_BridgeRebf.SimReadCmdTra( R_BrgCmdTraSim );
            XsimDataFromCtl.R_BrgCmdTraSimZ1 = R_BrgCmdTraSim;
        }
        //Xsim Output Relay Control singnle and syncronous signal T1St
        SimOutRlyControl();
        SimOutTemp();
        SimOutDiagMap();

        // control data flow to external simulator over the enet link
        XsimInterface.LinkControl();

        // set flag to use xsim output signals whenever the xsim client is actually connected
        static unsigned UseXsimOutputZ=false;
        UseXsimOutputZ = UseXsimOutput;// Enabe UseXsimOutput once Xsim be connceted unless XsimForceIsim
        UseXsimOutput = (XsimInterface.IsFbkValid() || UseXsimOutputZ )&& !PARM(XsimForceIsim);
        XsimDoeAct = PARM(XsimDoeEnb) && XsimInterface.IsFbkValid();
        // not acceleration at the beginning of reset, avoid Dependentrule issue.
        SimAcclAct = PARM(SimAcclEnb) && XsimInterface.IsFbkValid() && (XsimRealTm < PARM(XsimAcclEndTm)) && (XsimRealTm > 0.2f);
        static unsigned XsimDoeActZ=false;
        if((XsimDoeAct^XsimDoeActZ))
        {
            SimRunRuleReq=true;
        }
    }
    //-------------------------------------------------------------------------
    // TEMPORARY FOR DEBUGGING THE XSIM ENET CONNECTION - FORCE TO ALWAYS USE ISIM OUTPUT SIGNALS
    else
    {
        XsimDoeAct = false;
        //SimAcclAct = false;
        SimAcclAct = PARM(SimAcclEnb) && InitDone;
        UseXsimOutput = false;
    }
    static unsigned SimAcclActZ=false;
    if( (SimAcclAct^SimAcclActZ) )
    {
        SimRunRuleReq=true;
    }
    SimAcclActZ=SimAcclAct;

    //-------------------------------------------------------------------------.
    // COPY XSIM DOE COMMAND
    if(XsimDoeAct)
    {
        XsimDoeAutomation();
    }

    //-------------------------------------------------------------------------
    // choose the ouput signals to send back to the control
    if(UseXsimOutput)
    {
        XsimRealTm  = XsimDataToCtl.SimRealTm;
        R_WRtrXsim = XsimDataToCtl.XsimDoeSp.R_TachMeterSpd;
        R_TachPosnXsim = XsimDataToCtl.XsimDoeSp.R_TachMeterPosn;

        SimPlant1x.R_WRtrSim=R_WRtrXsim;
        SimPlant1x.R_TachPosnSim=R_TachPosnXsim;
        R_SpdRefSim=R_WRtrXsim/ PARM(R_PolePairs);
    }
    SimFbkCopyT1();
    if ( Sim1AFbkReq )
    {
        SimFbkCopyT1A();
    }
//  Logical end of if-then-else, since may have Aborted
//  }
    //-------------------------------------------------------------------------
    // done
    return;
}
