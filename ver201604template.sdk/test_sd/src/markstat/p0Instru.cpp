///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Wind Instrumentation
//
// DESCRIPTION:
//      This module contains Wind Turbine calculation functions
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     p0Instru.xls, p0Instru.vsd
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "p0Instru.h"
#include "b0BridgeRebf.h"
#include "b0BrgThrmstr.h"
#include "b0BrgThrm5p.h"
#include "b0DbThrm.h"

// Constants
//----------


// Externals
//----------


extern float        S_PllWT3;

extern float        GmPllVyErrPu;
extern float        GmPllVyErrT2;
extern float        L_VlnInRcp;
extern float        GmVMagPuT2;
extern float        PwrCmdLine;
extern float        SpdPwrPllFl2;


extern float        R_TrqCmdTrT2;
extern float        OneMnTPllMeld;
extern float        R_TrqCalT2;
extern float        DelTm2Rcp;
extern float        TurbNpRatRcp;


// Existing signals, used in rules for Rms & Signal Meters...
extern float        DelTm1A;
extern float        DelTm1;
extern float        DelTm2;
extern float        DelTm3;

// New & existing XY signals, used as inputs to RmsMeters...
extern float        S_IxFbkMet;
extern float        S_IyFbkMet;
extern float        S_VxFbkMet;
extern float        S_VyFbkMet;
extern float        GmIxFbk;
extern float        GmIyFbk;
extern float        GmVxFbk;
extern float        GmVyFbk;
extern float        L_IxFbkMet;
extern float        L_IyFbkMet;
extern float        L_VxFbkMet;
extern float        L_VyFbkMet;
extern float        R_VxFbk;
extern float        R_VyFbk;
extern float        R_IxFbkMet;
extern float        R_IyFbkMet;
extern float        R_IxFbkMet;
extern float        R_IyFbkMet;
extern float        Const1;




extern unsigned      R_SeqStat_PackedBits;

// Existing signals, used as inputs to SignalMeters...

extern cBridgeRebf     L_BridgeRebf;
extern cBridgeRebf     R_BridgeRebf;




extern cBrgThrm5pModel L_BrgThrm5pModel;
extern cBrgThrm5pModel R_BrgThrm5pModel;
extern cDbThrmModel    L_DbThrmModel;

extern float        R_SpdFbk;
extern float        R_TrqCmdAct;
extern float        R_TrqCmdInTt;
extern float        QregCmd;
extern float        R_TrqFbk;
extern float        GmPwrGridT2;
extern float        GmQPwrGridT2;

extern float        S_Pwr;
extern float        S_QPwr;
extern float        GmPwrT2;
extern float        GmQPwrT2;
extern float        GmPllWT2;

extern float        L_PwrLine;
extern float        L_VarLine;
extern float        R_VdcFil;
extern float        R_CnvOutPwr;
extern float        R_QPwr;
extern float        R_ElecFrq;
extern float        R_TrqCmd;



extern float        L_BrgLossPwr;
extern float        R_BtmTrPwrLossT2;
extern float        R_BtmDpwrLossT2;

// GDPLControl()
extern float    ISysCmdSF;
extern float    GmPllVyErrT2;
extern float    GmDscmVyPT2;
extern float    S_IyCmd;
extern float    R_TrqCmdInTtT2;
extern float    R_TrqRatRcp;
extern float    TurbNpRatWRcp;
extern float    R_SpdFbkRadT3;

//WindInstrumentT3()
extern float    GmIaSqrT3;
extern float    GmIbSqrT3;
extern float    GmIcSqrT3;
extern float    GmVanSqrT3;
extern float    GmVbnSqrT3;
extern float    GmVcnSqrT3;
extern float    L_IaSqrT3;
extern float    L_IbSqrT3;
extern float    L_IcSqrT3;
extern float    R_IaSqrT3;
extern float    R_IbSqrT3;
extern float    R_IcSqrT3;
extern float    R_VabSqrT3;
extern float    R_VbcSqrT3;
extern float    S_IaSqrT3;
extern float    S_IbSqrT3;
extern float    S_IcSqrT3;
extern float    S_VanSqrT3;
extern float    S_VbnSqrT3;
extern float    S_VcnSqrT3;
extern float    S_VabSqrT3;
extern float    S_VbcSqrT3;

// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(S_WxyFilMet,        float);     // Omega for Stator X-Y components for Rms Meters
CREATE_PARM(L_WxyFilMet,        float);     // Omega for Line   X-Y components for Rms Meters
CREATE_PARM(R_WxyFilMet,        float);     // Omega for Rotor  X-Y components for Rms Meters
CREATE_PARM(WrmsFilMet,         float);     // Omega for RMS Averaging Filters for Rms Meters
CREATE_PARM(WsigFilMet,         float);     // Omega for Anti-aliasing Filters for Signal Meters

// Power PLL Vy compensation
CREATE_PARM(PwrPllVyErDb,       float);     // Power PLL Vy error deadband
CREATE_PARM(PwrPllVyErGn,       float);     // Power PLL Vy scaling gain
CREATE_PARM(PllErrPwrOff,       float);     // Pll error for power offset
CREATE_PARM(PllErrPwrGn,        float);     // Pll error gain for power reduction

CREATE_PARM(PwrPllAngGn,        float);     // Torque pll angle gain
CREATE_PARM(TrqPllMeldRq,       float);     // Torque pll meld request
// GDPLControl
//Low Voltage torque reduction
CREATE_PARM(LvPwrLmRcyRt,       float);     // Low voltage Power limit recovery rate
CREATE_PARM(WVmagPwrLm,         float);     // Grid volts filter omega for torq limit down
CREATE_PARM(WVmagPwrUp,         float);     // Grid volts filter omega for torq limit up
CREATE_PARM(LvPwrRcvyOvr,       unsigned);  // Low voltge power recovery over-ride boolean
CREATE_PARM(LvPwrRcvyRat,       float);     // Low voltge power recovery rate when over-ride is set
CREATE_PARM(LvPwrLmRcyP0,       float);     // Low voltage power recovery pu power
CREATE_PARM(LvPwrLmRcyR0,       float);     // Low voltage power recovery rate below P.LvPwrLmRcyP0
CREATE_PARM(LvPwrP1VPu,         float);     // Low voltage power reduction voltage 1
CREATE_PARM(LvPwrP2VPu,         float);     // Low voltage power reduction voltage 2
CREATE_PARM(LvPwrP3VPu,         float);     // Low voltage power reduction voltage 3
CREATE_PARM(LvPwrP1PwrPu,       float);     // Low voltage power reduction point 1
CREATE_PARM(LvPwrP2PwrPu,       float);     // Low voltage power reduction point 2
CREATE_PARM(LvPwrP3PwrPu,       float);     // Low voltage power reduction point 3
CREATE_PARM(LvPwrMxPwrPu,       float);     // Low voltage torque reduction minimum current
CREATE_PARM(LvPwrLmPBScl,       float);     // Compensation for low voltage power scale for pump-back mode
CREATE_PARM(LvPwrCrvOvr,        unsigned);  // Low voltage power curve over-ride
CREATE_PARM(LvPwrP2VOv,         float);     // Low voltage power reduction voltage 2 -- over-ride
CREATE_PARM(LvPwrP3VOv,         float);     // Low voltage power reduction voltage 2 -- over-ride
CREATE_PARM(LvPwrP2PwrOv,       float);     // Low voltage power reduction power 2 -- over-ride
CREATE_PARM(LvPwrP3PwrOv,       float);     // Low voltage power reduction power 3 -- over-ride
CREATE_PARM(MaxRealCurr,        unsigned);  // Select maximum real current during LVRT events.

CREATE_PARM(PllErPLmRcyP0,      float);
CREATE_PARM(PllErrPwrRcyRt,     float);
CREATE_PARM(PllErrPwrRcyR0,     float);
CREATE_PARM(PllErrPwrFlW,       float);
CREATE_PARM(PllErrPwrFlDnW,     float);

CREATE_PARM(IRmsInstTau,        float);     // Rms current instrument tau

// Published Variables
//--------------------

//  Start GDPL Variables
CREATE_PUBVAR(LvPwrTrqScl     ,       float);
CREATE_PUBVAR(PllErrTrqFlUpWdt,       float);
CREATE_PUBVAR(PllErrTrqFlDnWdt,       float);
CREATE_PUBVAR(LvPwrSlope1     ,       float);
CREATE_PUBVAR(LvPwrSlope2     ,       float);
CREATE_PUBVAR(LvPwrSlope3     ,       float);
CREATE_PUBVAR(LvPwrVpuIn      ,       float);   // Lvrt filter voltage input for power reduction
CREATE_PUBVAR(LvPwrVpuOut     ,       float);
CREATE_PUBVAR(LvPwrVpu        ,       float);
CREATE_PUBVAR(PwrLimVMagPU    ,       float);
CREATE_PUBVAR(LvPwrRcyRtDy    ,       float);
//  End GDPL Variables

// Torque reduction
CREATE_PUBVAR(PllErrPwrIn     ,       float);
CREATE_PUBVAR(PllPwrRed0      ,       float);
CREATE_PUBVAR(PllPwrRed1      ,       float);
CREATE_PUBVAR(PllTrqRed       ,       float);

CREATE_PUBVAR(PllErPwrCmdPu   ,       float);
CREATE_PUBVAR(PllPwrRecovery  ,       float);
CREATE_PUBVAR(PllPwrRedErr    ,       float);
CREATE_PUBVAR(PllErrPRcyRtDy  ,       float);
CREATE_PUBVAR(LvPwrTrqSclRcp  ,       float);
CREATE_PUBVAR(PllErrPwrLim    ,       float);
CREATE_PUBVAR(PllPwrIntDel    ,       float);


CREATE_PUBVAR(PwrCmdPuT2      ,       float);
CREATE_PUBVAR(PwrPllFilGn     ,       float);
CREATE_PUBVAR(PwrPllAngFil    ,       float);
CREATE_PUBVAR(PwrPllDelAng    ,       float);
CREATE_PUBVAR(PwrPllWFF       ,       float);
CREATE_PUBVAR(PwrPllAng       ,       float);
CREATE_PUBVAR(PwrPllMod       ,       float);
CREATE_PUBVAR(TrqPllMeld      ,       float);

CREATE_PUBVAR(S_PowerRcp      ,       float);

CREATE_PUBVAR(WIRmsInstT3,      float);     // Omega for rms instrument calculations
CREATE_PUBVAR(L_IaSqrInst,      float);     // Line phase A sqared rms current
CREATE_PUBVAR(L_IaRmsInst,      float);     // Line phase A rms current
CREATE_PUBVAR(L_IbSqrInst,      float);     // Line phase B sqared rms current
CREATE_PUBVAR(L_IbRmsInst,      float);     // Line phase B rms current
CREATE_PUBVAR(L_IcSqrInst,      float);     // Line phase C sqared rms current
CREATE_PUBVAR(L_IcRmsInst,      float);     // Line phase C rms current
CREATE_PUBVAR(R_IaSqrInst,      float);     // Rotor phase A sqared rms current
CREATE_PUBVAR(R_IaRmsInst,      float);     // Rotor phase A rms current
CREATE_PUBVAR(R_IbSqrInst,      float);     // Rotor phase B sqared rms current
CREATE_PUBVAR(R_IbRmsInst,      float);     // Rotor phase B rms current
CREATE_PUBVAR(R_IcSqrInst,      float);     // Rotor phase C sqared rms current
CREATE_PUBVAR(R_IcRmsInst,      float);     // Rotor phase C rms current
CREATE_PUBVAR(R_VabSqrInst,     float);     // Rotor Vab sqared rms voltage
CREATE_PUBVAR(R_VbcSqrInst,     float);     // Rotor Vbc sqared rms voltage
CREATE_PUBVAR(R_VabRmsInst,     float);     // Rotor Vab rms voltage
CREATE_PUBVAR(R_VbcRmsInst,     float);     // Rotor Vbc rms voltage

CREATE_PUBVAR(S_IaSqrInst,      float);     // Stator phase A sqared rms current
CREATE_PUBVAR(S_IaRmsInst,      float);     // Stator phase A rms current
CREATE_PUBVAR(S_IbSqrInst,      float);     // Stator phase B sqared rms current
CREATE_PUBVAR(S_IbRmsInst,      float);     // Stator phase B rms current
CREATE_PUBVAR(S_IcSqrInst,      float);     // Stator phase C sqared rms current
CREATE_PUBVAR(S_IcRmsInst,      float);     // Stator phase C rms current
CREATE_PUBVAR(S_IaRmsVolts,     float);     // Stator phase A vco voltage input
CREATE_PUBVAR(S_IbRmsVolts,     float);     // Stator phase B vco voltage input
CREATE_PUBVAR(S_IcRmsVolts,     float);     // Stator phase C vco voltage input
CREATE_PUBVAR(S_VanSqrInst,     float);     // Stator Van sqared rms voltage
CREATE_PUBVAR(S_VanRmsInst,     float);     // Stator Van rms voltage
CREATE_PUBVAR(S_VbnSqrInst,     float);     // Stator Vbn sqared rms voltage
CREATE_PUBVAR(S_VbnRmsInst,     float);     // Stator Vbn rms voltage
CREATE_PUBVAR(S_VcnSqrInst,     float);     // Stator Vcn sqared rms voltage
CREATE_PUBVAR(S_VcnRmsInst,     float);     // Stator Vcn rms voltage
CREATE_PUBVAR(S_VabSqrInst,     float);     // Stator Van sqared rms voltage
CREATE_PUBVAR(S_VabRmsInst,     float);     // Stator Van rms voltage
CREATE_PUBVAR(S_VbcSqrInst,     float);     // Stator Vbn sqared rms voltage
CREATE_PUBVAR(S_VbcRmsInst,     float);     // Stator Vbn rms voltage
CREATE_PUBVAR(S_VoltsPrAmp,     float);     // Stator current volts per amp ratio
CREATE_PUBVAR(GmVoltsPrAmp,     float);     // Grid monitor current volts per amp ratio

// 690 Grid monitor feedback instrumentation
CREATE_PUBVAR(GmIaSqrIn   ,    float);  // Gm phase A sqared rms current
CREATE_PUBVAR(GmIbSqrIn   ,    float);  // Gm phase B sqared rms current
CREATE_PUBVAR(GmIcSqrIn   ,    float);  // Gm phase C sqared rms current
CREATE_PUBVAR(GmIaRmsVlt  ,    float);  // Gm phase A vco voltage input at burden resistor
CREATE_PUBVAR(GmIbRmsVlt  ,    float);  // Gm phase B vco voltage input at burden resistor
CREATE_PUBVAR(GmIcRmsVlt  ,    float);  // Gm phase C vco voltage input at burden resistor
CREATE_PUBVAR(GmIaRmsIn   ,    float);  // Gm phase A rms current
CREATE_PUBVAR(GmIbRmsIn   ,    float);  // Gm phase B rms current
CREATE_PUBVAR(GmIcRmsIn   ,    float);  // Gm phase C rms current
CREATE_PUBVAR(GmVanSqrIn  ,    float);  // Gm Van squared rms voltage
CREATE_PUBVAR(GmVbnSqrIn  ,    float);  // Gm Vbn squared rms voltage
CREATE_PUBVAR(GmVcnSqrIn  ,    float);  // Gm Vcn squared rms voltage
CREATE_PUBVAR(GmVanRmsIn  ,    float);  // Gm Van rms voltage
CREATE_PUBVAR(GmVbnRmsIn  ,    float);  // Gm Vbn rms voltage
CREATE_PUBVAR(GmVcnRmsIn  ,    float);  // Gm Vcn rms voltage




// RmsMeter & SignalMeter Filter Gains{Published)
CREATE_PUBVAR(S_WxyDtFilMet,   float);  // W*Dt for Stator X-Y components for RmsMeter
CREATE_PUBVAR(L_WxyDtFilMet,   float);  // W*Dt for Line   X-Y components for RmsMeter
CREATE_PUBVAR(R_WxyDtFilMet,   float);  // W*Dt for Rotor  X-Y components for RmsMeter
CREATE_PUBVAR(WrmsDtFilMet ,   float);  // W*Dt for RMS Averaging Filter  for RmsMeter
                                        //
CREATE_PUBVAR(WsigDt2FilMet,   float);  // W*Dt2 for SignalFilters
CREATE_PUBVAR(WsigDt3FilMet,   float);  // W*Dt3 for SignalFilters

// Fundamental RmsMeter signals{Published)
                                        //
CREATE_PUBVAR(S_IxFilMet,      float);  // Stator           Ix-component filtered for RmsMeter
CREATE_PUBVAR(S_IyFilMet,      float);  // Stator           Iy-component filtered for RmsMeter
CREATE_PUBVAR(S_VxFilMet,      float);  // Stator           Vx-component filtered for RmsMeter
CREATE_PUBVAR(S_VyFilMet,      float);  // Stator           Vy-component filtered for RmsMeter
CREATE_PUBVAR(S_IrmsMet,       float);  // Stator           fundamental Irms Meter
CREATE_PUBVAR(S_VrmsMet,       float);  // Stator           fundamental Vrms Meter
CREATE_PUBVAR(S_PllWFilMet,    float);  // Stator           fundamental stator frequency Meter
                                        //
CREATE_PUBVAR(GmIxFilMet,      float);  // Xfmr690V winding Ix-component filtered for RmsMeter
CREATE_PUBVAR(GmIyFilMet,      float);  // Xfmr690V winding Iy-component filtered for RmsMeter
CREATE_PUBVAR(GmVxFilMet,      float);  // Xfmr690V winding Vx-component filtered for RmsMeter
CREATE_PUBVAR(GmVyFilMet,      float);  // Xfmr690V winding Vy-component filtered for RmsMeter
CREATE_PUBVAR(GmIrmsMet,       float);  // Xfmr690V winding fundamental Irms Meter
CREATE_PUBVAR(GmVrmsMet,       float);  // Xfmr690V winding fundamental Vrms Meter
                                        //
CREATE_PUBVAR(L_IxFilMet,      float);  // Line bridge      Ix-component filtered for RmsMeter
CREATE_PUBVAR(L_IyFilMet,      float);  // Line bridge      Iy-component filtered for RmsMeter
CREATE_PUBVAR(L_VxFilMet,      float);  // Line bridge      Vx-component filtered for RmsMeter
CREATE_PUBVAR(L_VyFilMet,      float);  // Line bridge      Vy-component filtered for RmsMeter
CREATE_PUBVAR(L_IrmsMet,       float);  // Line bridge      fundamental Irms Meter
CREATE_PUBVAR(L_VrmsMet,       float);  // Line bridge      fundamental Vrms Meter
                                        //
CREATE_PUBVAR(R_IxFilMet,      float);  // Rotor bridge     Ix-component filtered for RmsMeter
CREATE_PUBVAR(R_IyFilMet,      float);  // Rotor bridge     Iy-component filtered for RmsMeter
CREATE_PUBVAR(R_IrmsMet,       float);  // Rotor bridge     fundamental Irms Meter
CREATE_PUBVAR(R_VxFilMet,      float);  // Rotor bridge     Vx-component filtered for RmsMeter
CREATE_PUBVAR(R_VyFilMet,      float);  // Rotor bridge     Vy-component filtered for RmsMeter
CREATE_PUBVAR(R_VrmsMet,       float);  // Rotor bridge     fundamental Vrms Meter




// Signal Meter signals (Published) (DataPassed signals should be created in signal-owner files)
                                        // Operating Point
CREATE_PUBVAR(R_SpdFbkMet   ,  float);  //     Speed Feedback
CREATE_PUBVAR(R_TrqCmdMet   ,  float);  //     Torque Command
CREATE_PUBVAR(R_TrqCmdTtMet ,  float);  //     Torque Command total
CREATE_PUBVAR(QregCmdMet    ,  float);  //     Var Command
CREATE_PUBVAR(PwrCmdGridMet ,  float);  //     Transformer secondary power command, kw
CREATE_PUBVAR(R_TrqFbkMet   ,  float);  //     Torque Calc
CREATE_PUBVAR(GmPwrGridMet  ,  float);  //     Total Grid Power
CREATE_PUBVAR(GmQPwrGridMet ,  float);  //     Total Grid Vars

CREATE_PUBVAR(S_VrmsMetT2,     float);  //     Vrms  , Task2
CREATE_PUBVAR(S_IrmsMetT2,     float);  //     Irms  , Task2
CREATE_PUBVAR(S_PwrMet,        float);  //     Power  (Task2)
CREATE_PUBVAR(S_QPwrMet,       float);  //     QPower (Task2)
                                        // 690V Grid Monitor
CREATE_PUBVAR(GmVrmsMetT2,     float);  //     Vrms  , Task2
CREATE_PUBVAR(GmIrmsMetT2,     float);  //     Irms  , Task2
CREATE_PUBVAR(GmPwrMet,        float);  //     Power  (Task2)
CREATE_PUBVAR(GmQPwrMet,       float);  //     QPower (Task2)
CREATE_PUBVAR(GmFrqMet,        float);  //     Freq   (Task2)
                                        // Line bridge
CREATE_PUBVAR(L_VrmsMetT2,     float);  //     Vrms  , Task2
CREATE_PUBVAR(L_IrmsMetT2,     float);  //     Irms  , Task2
CREATE_PUBVAR(L_PwrMet,        float);  //     Power  (Task2)
CREATE_PUBVAR(L_QPwrMet,       float);  //     QPower (Task2)
                                        // DC Link
CREATE_PUBVAR(R_VdcMet,        float);  //     Vdc    (Task2)
                                        // Rotor bridge, Net
CREATE_PUBVAR(R_VrmsMetT2,     float);  //     Vrms  , Task2
CREATE_PUBVAR(R_IrmsMetT2,     float);  //     Irms  , Task2

CREATE_PUBVAR(R_PwrMet,        float);  //     Power  (Task2)
CREATE_PUBVAR(R_QPwrMet,       float);  //     QPower (Task2)
CREATE_PUBVAR(R_FrqMet,        float);  //     Freq   (Task2)


CREATE_PUBVAR(CfcTempT2,       float);  // Thermistors
CREATE_PUBVAR(PdcTempT2,       float);  //
CREATE_PUBVAR(LqdTempT2,       float);  //
CREATE_PUBVAR(InletAirTempT2,  float);  //
CREATE_PUBVAR(PdcCabEntryTempT2,   float);  //
CREATE_PUBVAR(AclfuTempT2,     float);  //

CREATE_PUBVAR(L_AmbientTempT2,float);   // Pcb temperatures
CREATE_PUBVAR(R_AmbientTempT2,float);   //

CREATE_PUBVAR(PdcHtrOnCmdT2,unsigned);  // Heater commands
CREATE_PUBVAR(CccHtrOnCmdT2,unsigned);  //
CREATE_PUBVAR(CfcHtrOnCmdT2,unsigned);  //


                                        // Line  MaxTemps & LossPower
CREATE_PUBVAR(L_BrgTjncTmpMxT2,     float);
CREATE_PUBVAR(L_BrgDjncTmpMxT2,     float);
CREATE_PUBVAR(L_BrgPwrLossMet,      float);
CREATE_PUBVAR(L_BrgPwrLossMetT2,    float);
                                        // Rotor MaxTemps & LossPower
CREATE_PUBVAR(R_BrgTjncTmpMxT2,    float);
CREATE_PUBVAR(R_BrgDjncTmpMxT2,    float);
CREATE_PUBVAR(R_BrgPwrLossMet,     float);

                                        // DB Temps & LossPower
CREATE_PUBVAR(L_DbTjncTmpT2,       float);
CREATE_PUBVAR(L_DbTpwrLossT2,      float);
CREATE_PUBVAR(L_DbTpwrLossMet,     float);
CREATE_PUBVAR(L_DbResTmpRiseMet,   float);





// Power PLL Vy compensation
CREATE_PUBVAR(PwrPllMod0,       float);
CREATE_PUBVAR(PwrPllVy,         float);

// Unpublished Variables
//----------------------

float   S_ImagSqr1Met;         // Stator           ImagSqr DataPass for RmsMeter
float   S_VmagSqr1Met;         // Stator           VmagSqr DataPass for RmsMeter
float   S_ImagSqr3Met;         // Stator           ImagSqr DataPass for RmsMeter
float   S_VmagSqr3Met;         // Stator           VmagSqr DataPass for RmsMeter
float   S_ImagSqrFilMet;       // Stator           ImagSqr Filtered for RmsMeter
float   S_VmagSqrFilMet;       // Stator           VmagSqr Filtered for RmsMeter

float   GmImagSqr1Met;         // Xfmr690V winding ImagSqr DataPass for RmsMeter
float   GmVmagSqr1Met;         // Xfmr690V winding VmagSqr DataPass for RmsMeter
float   GmImagSqr3Met;         // Xfmr690V winding ImagSqr DataPass for RmsMeter
float   GmVmagSqr3Met;         // Xfmr690V winding VmagSqr DataPass for RmsMeter
float   GmImagSqrFilMet;       // Xfmr690V winding ImagSqr Filtered for RmsMeter
float   GmVmagSqrFilMet;       // Xfmr690V winding VmagSqr Filtered for RmsMeter

float   L_ImagSqr1Met;         // Line  bridge     ImagSqr DataPass for RmsMeter
float   L_VmagSqr1Met;         // Line  bridge     VmagSqr DataPass for RmsMeter
float   L_ImagSqr3Met;         // Line  bridge     ImagSqr DataPass for RmsMeter
float   L_VmagSqr3Met;         // Line  bridge     VmagSqr DataPass for RmsMeter
float   L_ImagSqrFilMet;       // Line  bridge     ImagSqr Filtered for RmsMeter
float   L_VmagSqrFilMet;       // Line  bridge     VmagSqr Filtered for RmsMeter

float   R_ImagSqr1Met;         // Rotor bridge     ImagSqr DataPass for RmsMeter
float   R_VmagSqr1Met;         // Rotor bridge     VmagSqr DataPass for RmsMeter
float   R_ImagSqr3Met;         // Rotor bridge     ImagSqr DataPass for RmsMeter
float   R_VmagSqr3Met;         // Rotor bridge     VmagSqr DataPass for RmsMeter
float   R_ImagSqrFilMet;       // Rotor bridge     ImagSqr Filtered for RmsMeter
float   R_VmagSqrFilMet;       // Rotor bridge     VmagSqr Filtered for RmsMeter



// Diagnostics
//------------


// Data Passing (DataPassing should reside in signal-owner files)
//-------------

// RMS Meters
DATA_PASS( S_ImagSqr1Met  , S_ImagSqr3Met  , T1A_T3,FILTERED ); // Stator
DATA_PASS( S_VmagSqr1Met  , S_VmagSqr3Met  , T1A_T3,FILTERED ); //
DATA_PASS( GmImagSqr1Met  , GmImagSqr3Met  , T1_T3 ,FILTERED ); // Xfmr690V winding
DATA_PASS( GmVmagSqr1Met  , GmVmagSqr3Met  , T1_T3 ,FILTERED ); //
DATA_PASS( L_ImagSqr1Met  , L_ImagSqr3Met  , T1_T3 ,FILTERED ); // Line  bridge
DATA_PASS( L_VmagSqr1Met  , L_VmagSqr3Met  , T1_T3 ,FILTERED ); //
DATA_PASS( R_ImagSqr1Met  , R_ImagSqr3Met  , T1A_T3,FILTERED ); // Rotor bridge , net
DATA_PASS( R_VmagSqr1Met  , R_VmagSqr3Met  , T1A_T3,FILTERED ); //

// RMS Signal Meters
DATA_PASS( S_VrmsMet      , S_VrmsMetT2    , T3_T2,UNFILTERED); // Stator
DATA_PASS( S_IrmsMet      , S_IrmsMetT2    , T3_T2,UNFILTERED); //
DATA_PASS( GmVrmsMet      , GmVrmsMetT2    , T3_T2,UNFILTERED); // 690V Grid Monitor
DATA_PASS( GmIrmsMet      , GmIrmsMetT2    , T3_T2,UNFILTERED); //
DATA_PASS( L_VrmsMet      , L_VrmsMetT2    , T3_T2,UNFILTERED); // Line  bridge
DATA_PASS( L_IrmsMet      , L_IrmsMetT2    , T3_T2,UNFILTERED); //
DATA_PASS( R_VrmsMet      , R_VrmsMetT2    , T3_T2,UNFILTERED); // Rotor bridge , net
DATA_PASS( R_IrmsMet      , R_IrmsMetT2    , T3_T2,UNFILTERED); //
                                                                //
// Signal Meters

DATA_PASS( CfcTemp        , CfcTempT2         , T3_T2,UNFILTERED); // Thermistors
DATA_PASS( PdcTemp        , PdcTempT2         , T3_T2,UNFILTERED); //
DATA_PASS( CoolantTemp    , LqdTempT2         , T3_T2,UNFILTERED); //
DATA_PASS( InletAirTemp   , InletAirTempT2    , T3_T2,UNFILTERED); //
DATA_PASS( PdcCabEntryTemp, PdcCabEntryTempT2 , T3_T2,UNFILTERED); //
DATA_PASS( AclfuTemp      , AclfuTempT2       , T3_T2,UNFILTERED); //

DATA_PASS( L_BridgeRebf.AmbientTemp, L_AmbientTempT2, T1_T2 , FILTERED);  // Pcb temperatures
DATA_PASS( R_BridgeRebf.AmbientTemp, R_AmbientTempT2, T1A_T2, FILTERED);  //

DATA_PASS( L_BrgThrmstr.BthtrTemp1,  L_BpTemp1T2, T3_T2,UNFILTERED);  // Line   Bp RTDs
DATA_PASS( L_BrgThrmstr.BthtrTemp2,  L_BpTemp2T2, T3_T2,UNFILTERED);  //
DATA_PASS( L_BrgThrmstr.BthtrTemp3,  L_BpTemp3T2, T3_T2,UNFILTERED);  //
DATA_PASS( L_BrgThrmstr.BthtrTemp4,  L_BpTemp4T2, T3_T2,UNFILTERED);  //
DATA_PASS( L_BrgThrmstr.BthtrTemp5,  L_BpTemp5T2, T3_T2,UNFILTERED);  //
DATA_PASS( L_BrgThrmstr.BthtrTemp6,  L_BpTemp6T2, T3_T2,UNFILTERED);  //

DATA_PASS(R_BrgThrmstr.BthtrTemp1, R_BpTemp1T2, T3_T2,UNFILTERED);  // Rotor1 Bp RTDs
DATA_PASS(R_BrgThrmstr.BthtrTemp2, R_BpTemp2T2, T3_T2,UNFILTERED);  //
DATA_PASS(R_BrgThrmstr.BthtrTemp3, R_BpTemp3T2, T3_T2,UNFILTERED);  //
DATA_PASS(R_BrgThrmstr.BthtrTemp4, R_BpTemp4T2, T3_T2,UNFILTERED);  //
DATA_PASS(R_BrgThrmstr.BthtrTemp5, R_BpTemp5T2, T3_T2,UNFILTERED);  //
DATA_PASS(R_BrgThrmstr.BthtrTemp6, R_BpTemp6T2, T3_T2,UNFILTERED);  //
DATA_PASS(R_BthtrTemp7,            R_BpTemp7T2, T3_T2,UNFILTERED);  //
DATA_PASS(R_BthtrTemp8,            R_BpTemp8T2, T3_T2,UNFILTERED);  //
                                                                // Line  MaxTemps & LossPowers
DATA_PASS( L_BrgThrm5pModel.BtmTrJncTempMax,  L_BrgTjncTmpMxT2,     T3_T2, UNFILTERED);
DATA_PASS( L_BrgThrm5pModel.BtmDjncTempMx  ,  L_BrgDjncTmpMxT2,     T3_T2, UNFILTERED);
DATA_PASS( L_BrgPwrLossMet,                   L_BrgPwrLossMetT2,    T3_T2, UNFILTERED);
                                                                // Rotor MaxTemps & LossPowers
DATA_PASS( R_BrgThrm5pModel.BtmTrJncTempMax, R_BrgTjncTmpMxT2,    T3_T2, UNFILTERED);
DATA_PASS( R_BrgThrm5pModel.BtmDjncTempMx,   R_BrgDjncTmpMxT2,    T3_T2, UNFILTERED);
                                                                // DB Temps & LossPowers
DATA_PASS( L_DbThrmModel.DtmTrJncTempT3,     L_DbTjncTmpT2,       T3_T2, UNFILTERED);
DATA_PASS( L_DbThrmModel.DtmTrPwrLoss,       L_DbTpwrLossT2,      T1_T2, FILTERED  );
DATA_PASS( L_DbThrmModel.DtmTmpRise,         L_DbResTmpRiseMet,   T3_T2, UNFILTERED);
                                                                // TOCs
DATA_PASS( S_TocHiSel     , S_TocHiSelT2    , T3_T2, UNFILTERED );
DATA_PASS( S_TocThrDy     , S_TocThrDyT2    , T3_T2, UNFILTERED );
DATA_PASS( GmTocHiSel     , GmTocHiSelT2    , T3_T2, UNFILTERED );
DATA_PASS( GmTocThrDy     , GmTocThrDyT2    , T3_T2, UNFILTERED );
DATA_PASS( DFTocHiSel     , DfTocHiSelT2    , T3_T2, UNFILTERED );  //(note:"DF")
DATA_PASS( L_FuseTocHiSel , L_FuseTocHiSelT2, T3_T2, UNFILTERED );
DATA_PASS( L_FuseTocThrDy , L_FuseTocThrDyT2, T3_T2, UNFILTERED );
DATA_PASS( L_TocHiSel     , L_TocHiSelT2    , T3_T2, UNFILTERED );
DATA_PASS( L_TocThrDy     , L_TocThrDyT2    , T3_T2, UNFILTERED );
DATA_PASS( R_TocHiSel     , R_TocHiSelT2    , T3_T2, UNFILTERED );
DATA_PASS( R_TocThrDy     , R_TocThrDyT2    , T3_T2, UNFILTERED );



// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
// Break in flow of WIND INSTRUMENTATION is left FOR NOW to support merging...
//     WindInstrumentRules(), WindInstrumentT1A(), WindInstrumentT1()
//     <<<< INSERTED: CurrentLimitCalc procedures >>>
//     WindInstrumentT3(), WindInstrumentT2(), WindSignalInstrT3()
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Wind application instrumentation
//
// EXECUTION LEVEL
//      Rules
//
// RETURN VALUE
//      This function expresses the RULES, which support Run-Time Tasks
//      for the fundamental RMS meter(instrument).
//
///////////////////////////////////////////////////////////////////////////////
int WindInstrumentRules(void)
{
    float   Wdt;

    // High-level Omega's
    SET_FV(S_WxyFilMet, 5.0F * (2.0F*PI) );
    SET_FV(L_WxyFilMet, 5.0F * (2.0F*PI) );
    SET_FV(R_WxyFilMet, 2.0F * (2.0F*PI) );
    SET_FV(WrmsFilMet , 0.5F );
    SET_FV(WsigFilMet , 0.5F );

    // W*Dt for Stator X-Y components for RmsMeter
    Wdt  = PARM(S_WxyFilMet)*DelTm1A;
    S_WxyDtFilMet = NoNANLim( Wdt/(1.0F+Wdt), 1.0F, 1.0e-6F );

    // W*Dt for Line   X-Y components for RmsMeter
    Wdt  = PARM(L_WxyFilMet)*DelTm1;
    L_WxyDtFilMet = NoNANLim( Wdt/(1.0F+Wdt), 1.0F, 1.0e-6F );

    // W*Dt for Rotor  X-Y components for RmsMeter
    Wdt = PARM(R_WxyFilMet)*DelTm1A;
    R_WxyDtFilMet = NoNANLim( Wdt/(1.0F+Wdt), 1.0F, 1.0e-6F );

    // W*Dt for RMS Averaging Filter  for RmsMeter
    Wdt = PARM(WrmsFilMet)*DelTm3;
    WrmsDtFilMet  = NoNANLim( Wdt/(1.0F+Wdt), 1.0F, 1.0e-6F );

    // W*Dt for SignalMeter Filters
    Wdt = PARM(WsigFilMet)*DelTm2;
    WsigDt2FilMet  = NoNANLim( Wdt/(1.0F+Wdt), 1.0F, 1.0e-6F );

    Wdt = PARM(WsigFilMet)*DelTm3;
    WsigDt3FilMet  = NoNANLim( Wdt/(1.0F+Wdt), 1.0F, 1.0e-6F );


    return(SUCCESS);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Wind application instrumentation
//
// EXECUTION LEVELS
//      Task1A, Task1
//
// RETURN VALUE
//      This function is used to develop the component-squared terms
//      in the appropriate "Fast Task", to then be integrally-data-passed
//      to the "Slow Task3" for final RMS filtering.
//
///////////////////////////////////////////////////////////////////////////////

float MagSqr(
    //dynamic inputs
    float   Xfbk,
    float   Yfbk,
    //static parameters
    float   WxyDt,
    //states
    float   *Xfil,
    float   *Yfil
)
{
    float   SumSqr;

    // Xfil  += (Xfbk-Xfil)*WxyDt
    // Yfil  += (Yfbk-Yfil)*WxyDt
    // SumSqr = (Xfil*Xfil)*(Yfil*Yfil)
    *Xfil  += (Xfbk - *Xfil)*WxyDt;
    *Yfil  += (Yfbk - *Yfil)*WxyDt;
    SumSqr = (*Xfil)*(*Xfil)+(*Yfil)*(*Yfil);

    return SumSqr;
}

void WindInstrumentT1A()
{

    S_ImagSqr1Met   = MagSqr( S_IxFbkMet  , S_IyFbkMet,   S_WxyDtFilMet,
                             &S_IxFilMet  ,&S_IyFilMet);
    S_VmagSqr1Met   = MagSqr( S_VxFbkMet  , S_VyFbkMet,   S_WxyDtFilMet,
                             &S_VxFilMet  ,&S_VyFilMet);

    R_ImagSqr1Met   = MagSqr( R_IxFbkMet  , R_IyFbkMet,   R_WxyDtFilMet,
                             &R_IxFilMet  ,&R_IyFilMet);

    R_VmagSqr1Met   = MagSqr( R_VxFbk     , R_VyFbk,   R_WxyDtFilMet,
                             &R_VxFilMet  ,&R_VyFilMet);



}

void WindInstrumentT1()
{
    GmImagSqr1Met   = MagSqr( GmIxFbk     , GmIyFbk,      L_WxyDtFilMet,
                              &GmIxFilMet,  &GmIyFilMet);
    GmVmagSqr1Met   = MagSqr( GmVxFbk     , GmVyFbk,      L_WxyDtFilMet,
                              &GmVxFilMet  ,&GmVyFilMet);

    L_ImagSqr1Met   = MagSqr( L_IxFbkMet  , L_IyFbkMet,   L_WxyDtFilMet,
                              &L_IxFilMet  ,&L_IyFilMet);
    L_VmagSqr1Met   = MagSqr( L_VxFbkMet  , L_VyFbkMet,   L_WxyDtFilMet,
                              &L_VxFilMet  ,&L_VyFilMet);
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Wind application instrumentation
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      This function is used to develop the rms vaues for application
//      VCO inputs. Only those VCOs that are used in the application
//      are represented. In addition to the five application VCOs, the
//      six bridge rms currents are calculated here.
//
///////////////////////////////////////////////////////////////////////////////
void WindInstrumentT3()
{
    float S_PllWT3Hz;

    L_IaSqrInst  = L_IaSqrInst + WIRmsInstT3*(L_IaSqrT3 - L_IaSqrInst);
    L_IaRmsInst  = sqrtf(L_IaSqrInst);

    L_IbSqrInst  = L_IbSqrInst + WIRmsInstT3*(L_IbSqrT3 - L_IbSqrInst);
    L_IbRmsInst  = sqrtf(L_IbSqrInst);

    L_IcSqrInst  = L_IcSqrInst + WIRmsInstT3*(L_IcSqrT3 - L_IcSqrInst);
    L_IcRmsInst  = sqrtf(L_IcSqrInst);

    R_IaSqrInst  = R_IaSqrInst + WIRmsInstT3*(R_IaSqrT3 - R_IaSqrInst);
    R_IaRmsInst  = sqrtf(R_IaSqrInst);

    R_IbSqrInst  = R_IbSqrInst + WIRmsInstT3*(R_IbSqrT3 - R_IbSqrInst);
    R_IbRmsInst  = sqrtf(R_IbSqrInst);

    R_IcSqrInst  = R_IcSqrInst + WIRmsInstT3*(R_IcSqrT3 - R_IcSqrInst);
    R_IcRmsInst  = sqrtf(R_IcSqrInst);

    R_VabSqrInst = R_VabSqrInst + WIRmsInstT3*(R_VabSqrT3-R_VabSqrInst);
    R_VabRmsInst = sqrtf(R_VabSqrInst);

    R_VbcSqrInst = R_VbcSqrInst + WIRmsInstT3*(R_VbcSqrT3-R_VbcSqrInst);
    R_VbcRmsInst = sqrtf(R_VbcSqrInst);

    S_IaSqrInst  = S_IaSqrInst + WIRmsInstT3*(S_IaSqrT3 - S_IaSqrInst);
    S_IaRmsInst  = sqrtf(S_IaSqrInst);
    S_IaRmsVolts = S_VoltsPrAmp * S_IaRmsInst;

    S_IbSqrInst  = S_IbSqrInst + WIRmsInstT3*(S_IbSqrT3 - S_IbSqrInst);
    S_IbRmsInst  = sqrtf(S_IbSqrInst);
    S_IbRmsVolts = S_VoltsPrAmp * S_IbRmsInst;

    S_IcSqrInst  = S_IcSqrInst + WIRmsInstT3*(S_IcSqrT3 - S_IcSqrInst);
    S_IcRmsInst  = sqrtf(S_IcSqrInst);
    S_IcRmsVolts = S_VoltsPrAmp * S_IcRmsInst;

    S_VanSqrInst  = S_VanSqrInst + WIRmsInstT3*(S_VanSqrT3 - S_VanSqrInst);
    S_VanRmsInst  = sqrtf(S_VanSqrInst);

    S_VbnSqrInst  = S_VbnSqrInst + WIRmsInstT3*(S_VbnSqrT3 - S_VbnSqrInst);
    S_VbnRmsInst  = sqrtf(S_VbnSqrInst);

    S_VcnSqrInst  = S_VcnSqrInst + WIRmsInstT3*(S_VcnSqrT3 - S_VcnSqrInst);
    S_VcnRmsInst  = sqrtf(S_VcnSqrInst);

    S_VabSqrInst  = S_VabSqrInst + WIRmsInstT3*(S_VabSqrT3 - S_VabSqrInst);
    S_VabRmsInst  = sqrtf(S_VabSqrInst);

    S_VbcSqrInst  = S_VbcSqrInst + WIRmsInstT3*(S_VbcSqrT3 - S_VbcSqrInst);
    S_VbcRmsInst  = sqrtf(S_VbcSqrInst);
    //690V Grid monitor feedbacks
    GmIaSqrIn  = GmIaSqrIn + WIRmsInstT3*(GmIaSqrT3 - GmIaSqrIn);
    GmIaRmsIn  = sqrtf(GmIaSqrIn);
    GmIaRmsVlt = GmVoltsPrAmp * GmIaRmsIn;

    GmIbSqrIn  = GmIbSqrIn + WIRmsInstT3*(GmIbSqrT3 - GmIbSqrIn);
    GmIbRmsIn  = sqrtf(GmIbSqrIn);
    GmIbRmsVlt = GmVoltsPrAmp * GmIbRmsIn;

    GmIcSqrIn  = GmIcSqrIn + WIRmsInstT3*(GmIcSqrT3 - GmIcSqrIn);
    GmIcRmsIn  = sqrtf(GmIcSqrIn);
    GmIcRmsVlt = GmVoltsPrAmp * GmIcRmsIn;

    GmVanSqrIn  = GmVanSqrIn + WIRmsInstT3*(GmVanSqrT3 - GmVanSqrIn);
    GmVanRmsIn  = sqrtf(GmVanSqrIn);

    GmVbnSqrIn  = GmVbnSqrIn + WIRmsInstT3*(GmVbnSqrT3 - GmVbnSqrIn);
    GmVbnRmsIn  = sqrtf(GmVbnSqrIn);

    GmVcnSqrIn  = GmVcnSqrIn + WIRmsInstT3*(GmVcnSqrT3 - GmVcnSqrIn);
    GmVcnRmsIn  = sqrtf(GmVcnSqrIn);

    S_PllWT3Hz  = RAD_TO_HZ * S_PllWT3;

    //RmsMeter, RMS "Mean" Filters: fil+=(fbk-fil)*WrmsDt3

    S_ImagSqrFilMet   += (S_ImagSqr3Met   - S_ImagSqrFilMet)   * WrmsDtFilMet;
    S_VmagSqrFilMet   += (S_VmagSqr3Met   - S_VmagSqrFilMet)   * WrmsDtFilMet;
    S_PllWFilMet      += (S_PllWT3Hz      - S_PllWFilMet)      * WrmsDtFilMet;

    GmImagSqrFilMet   += (  GmImagSqr3Met - GmImagSqrFilMet)   * WrmsDtFilMet;
    GmVmagSqrFilMet   += (  GmVmagSqr3Met - GmVmagSqrFilMet)   * WrmsDtFilMet;

    L_ImagSqrFilMet   += (L_ImagSqr3Met   - L_ImagSqrFilMet)   * WrmsDtFilMet;
    L_VmagSqrFilMet   += (L_VmagSqr3Met   - L_VmagSqrFilMet)   * WrmsDtFilMet;

    R_ImagSqrFilMet   += (R_ImagSqr3Met   - R_ImagSqrFilMet)   * WrmsDtFilMet;
    R_VmagSqrFilMet   += (R_VmagSqr3Met   - R_VmagSqrFilMet)   * WrmsDtFilMet;


    //RmsMeter, Net Scale & Square Root: sqrt(1/2),sqrt(3/2)

    S_IrmsMet   = sqrtf(0.5F*S_ImagSqrFilMet);
    S_VrmsMet   = sqrtf(1.5F*S_VmagSqrFilMet);

    GmIrmsMet   = sqrtf(0.5F*GmImagSqrFilMet);
    GmVrmsMet   = sqrtf(1.5F*GmVmagSqrFilMet);

    L_IrmsMet   = sqrtf(0.5F*L_ImagSqrFilMet);
    L_VrmsMet   = sqrtf(1.5F*L_VmagSqrFilMet);

    R_IrmsMet   = sqrtf(0.5F*R_ImagSqrFilMet);
    R_VrmsMet   = sqrtf(1.5F*R_VmagSqrFilMet);



    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Wind Signal Instrumentation
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      These functions are used to develop the meter signals, scaled in
//      some cases, and filtered prior to the IPC interface.  Signals are
//      designated with "Met" to distinguish them from control signals.
//      Originally created for TestMan thermal testing, but may be used in
//      other cenarios as well.
//      Many signals, similar to those conditioned here, are provided to
//      the IPC interface; consider purging these other signals.
//
///////////////////////////////////////////////////////////////////////////////
void WindSignalInstrT2()
{
    float   temp;

    // Operating PointS
    R_SpdFbkMet   += ( R_SpdFbk               - R_SpdFbkMet  ) * WsigDt2FilMet;
    R_TrqCmdMet   += ((R_TrqCmdAct  * 0.001F) - R_TrqCmdMet  ) * WsigDt2FilMet;
    R_TrqCmdTtMet += ((R_TrqCmdInTt * 0.001F) - R_TrqCmdTtMet) * WsigDt2FilMet;
    QregCmdMet    += ((QregCmd      * 0.001F) - QregCmdMet   ) * WsigDt2FilMet;
    R_TrqFbkMet   += ((-R_TrqFbk    * 0.001F) - R_TrqFbkMet  ) * WsigDt2FilMet;
    PwrCmdGridMet += ((PwrCmdLine   * 0.001F) - PwrCmdGridMet) * WsigDt2FilMet;
    GmPwrGridMet  += ((GmPwrGridT2  * 0.001F) - GmPwrGridMet ) * WsigDt2FilMet;
    GmQPwrGridMet += ((GmQPwrGridT2 * 0.001F) - GmQPwrGridMet) * WsigDt2FilMet;

    // Stator
    S_PwrMet      += ((S_Pwr         * 0.001F) - S_PwrMet     ) * WsigDt2FilMet;
    S_QPwrMet     += ((S_QPwr        * 0.001F) - S_QPwrMet    ) * WsigDt2FilMet;

    // 690V Grid Monitor
    GmPwrMet      += ((GmPwrT2       * 0.001F) - GmPwrMet     ) * WsigDt2FilMet;
    GmQPwrMet     += ((GmQPwrT2      * 0.001F) - GmQPwrMet    ) * WsigDt2FilMet;
    GmFrqMet      += ((GmPllWT2   * RAD_TO_HZ) - GmFrqMet     ) * WsigDt2FilMet;

    // Line Converter
    L_PwrMet      += ((L_PwrLine     * 0.001F) - L_PwrMet     ) * WsigDt2FilMet;
    L_QPwrMet     += ((L_VarLine     * 0.001F) - L_QPwrMet    ) * WsigDt2FilMet;

    // DC Link
    R_VdcMet      += ( R_VdcFil                - R_VdcMet     ) * WsigDt2FilMet;

    // Rotor Converter
    R_PwrMet      += ((R_CnvOutPwr   * 0.001F) - R_PwrMet     ) * WsigDt2FilMet;
    R_QPwrMet     += ((R_QPwr        * 0.001F) - R_QPwrMet    ) * WsigDt2FilMet;
    R_FrqMet      += ( R_ElecFrq               - R_FrqMet     ) * WsigDt2FilMet;

    // Rotor Bridge Thermal Models
    temp              = ( R_BtmTrPwrLossT2 + R_BtmDpwrLossT2 );
    R_BrgPwrLossMet += ( temp - R_BrgPwrLossMet) * WsigDt2FilMet;

    // DB Bridge Thermal Models
    L_DbTpwrLossMet += ( L_DbTpwrLossT2 - L_DbTpwrLossMet) * WsigDt2FilMet;

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Wind Signal Instrumentation
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      These functions are used to develop the meter signals, scaled in
//      some cases, and filtered prior to the IPC interface.  Signals are
//      designated with "Met" to distinguish them from control signals.
//      Originally created for TestMan thermal testing, but may be used in
//      other scenarios as well.
//      Many signals, similar to those conditioned here, are provided to
//      the IPC interface; consider purging these other signals.
//
///////////////////////////////////////////////////////////////////////////////
void WindSignalInstrT3()
{

    // Line Bridge Thermal Models
    L_BrgPwrLossMet  += ( L_BrgLossPwr - L_BrgPwrLossMet ) * WsigDt3FilMet;


    return;
}


/******************************************************************************
* Function:         GDPLControl
* Execution Level:  Task 2
* Execution Time:   Not measured
*
* Formal Inputs:    None
* Formal Outputs:   None
*
* Global Inputs:
*
*
* Global Outputs:
*
* Return Value:
* Function Calls:   None
*
* Abstract:         This function is used to implement the Grid Dependent Power
* Limiter (GDPL).  This limiter reduces real power as a function of grid voltage
* drop.  It has user input parameters
*
*
*
******************************************************************************/
void GDPLControl(void)
{

    float PwrPllAngFilZ1;
    float LvPwrVpuOutTmp;
    float LvPwrVpuErr;
    float PwrLimIntDel;

    GmPllVyErrPu = GmPllVyErrT2 * L_VlnInRcp;
    PllErrPwrIn  = GmPllVyErrPu - PARM(PllErrPwrOff);

    PllPwrRed0   = PARM(PllErrPwrGn) * PllErrPwrIn ;

    if ( PllPwrRed0 < 0.0F)
    {
        PllPwrRed0 = 0.0F;
    }

    /*********************************************/
    //Dynamic recovery rate--slower after inital recovery

    LvPwrTrqSclRcp = R_SpdFbkRadT3 * TurbNpRatWRcp;
    PllErrPwrLim = MAX(R_TrqCmd, 0.0F) * LvPwrTrqSclRcp;

    PllErPwrCmdPu = R_TrqCmd * LvPwrTrqSclRcp;
    PllPwrRecovery = PllErPwrCmdPu - PllPwrRed1;

    if ( PllPwrRecovery >= PARM(PllErPLmRcyP0)) //slow
    {
        PllErrPRcyRtDy = PARM(PllErrPwrRcyRt);
    }
    else //fast
    {
        PllErrPRcyRtDy = PARM(PllErrPwrRcyR0);
    }

    PllPwrRedErr = PllPwrRed0 - PllPwrRed1;
    if ( PllPwrRedErr > 0.0F)
    {
       PllPwrIntDel  = PARM(PllErrPwrFlW) * PllPwrRedErr;
    }
    else
    {
       PllPwrIntDel  = PARM(PllErrPwrFlDnW) * PllPwrRedErr;
    }

    if ( PllPwrIntDel < PllErrPRcyRtDy)
    {
        PllPwrIntDel = PllErrPRcyRtDy;
    }

    if (R_SeqStat.R_TrqEnbStat)
    {
        PllPwrRed1 += DelTm2 * PllPwrIntDel;
    }
    else
    {
        PllPwrRed1 = 0.0F;
    }

    if ( PllPwrRed1 > PllErrPwrLim)
    {
        PllPwrRed1 = PllErrPwrLim;
    }
    else if ( PllPwrRed1 < 0.0F)
    {
        PllPwrRed1 = 0.0F;
    }

    PllTrqRed = PllPwrRed1  * LvPwrTrqScl;
    /********************************************/


    /* Filter grid volts */
    LvPwrVpuIn  = GmVMagPuT2;;

    if (LvPwrVpuIn > PARM(LvPwrP1VPu))
    {
        LvPwrVpuOutTmp = PARM(LvPwrP1PwrPu);
    }
    else if ( LvPwrVpuIn > PARM(LvPwrP2VPu))
    {
        LvPwrVpuOutTmp =  PARM(LvPwrP2PwrPu) + (LvPwrVpuIn - PARM(LvPwrP2VPu)) * LvPwrSlope1;
    }
    else if ( LvPwrVpuIn > PARM(LvPwrP3VPu))
    {
        LvPwrVpuOutTmp =  PARM(LvPwrP3PwrPu) + (LvPwrVpuIn - PARM(LvPwrP3VPu)) * LvPwrSlope2;
    }
    else //else voltage is between 0 and PwrP3VPu
    {
        LvPwrVpuOutTmp =  LvPwrVpuIn * LvPwrSlope3;
    }

    if (LvPwrVpuOutTmp > PARM(LvPwrMxPwrPu))
    {
       LvPwrVpuOutTmp = PARM(LvPwrMxPwrPu);
    }

    LvPwrVpuOut = LvPwrVpuOutTmp;


    //Dynamic recovery rate--slower after inital recovery
    if ( PwrLimVMagPU >= PARM(LvPwrLmRcyP0)) //slow
    {
        LvPwrRcyRtDy = PARM(LvPwrLmRcyRt);
    }
    else //fast
    {
        LvPwrRcyRtDy = PARM(LvPwrLmRcyR0);
    }

    LvPwrVpuErr = LvPwrVpuOut - PwrLimVMagPU;
    if ( LvPwrVpuErr > 0.0)
    {
       PwrLimIntDel  = PARM(WVmagPwrUp) * LvPwrVpuErr;
    }
    else
    {
       PwrLimIntDel  = PARM(WVmagPwrLm) * LvPwrVpuErr;
    }

    if ( PwrLimIntDel > LvPwrRcyRtDy)
    {
        PwrLimIntDel = LvPwrRcyRtDy;
    }

    if (R_SeqStat.R_TrqEnbStat)
    {
        PwrLimVMagPU += DelTm2 * PwrLimIntDel;
    }
    else
    {
        PwrLimVMagPU = 0.0F;
    }

    //////////////////////////////////////////////////////
    /// End Torque reduction as a function of voltages
    //////////////////////////////////////////////// /////

    //////////////////////////////////////////////////////////////////
    /// Start Pll angle compe as a function of power -- APP_150
    //////////////////////////////////////////////// ////////////////

    TrqPllMeld   = (PARM(TrqPllMeldRq) * R_TrqCmdInTtT2) + (OneMnTPllMeld * R_TrqCalT2);
    PwrPllMod0   = -TrqPllMeld * SpdPwrPllFl2 * ISysCmdSF;

    if(ABS(GmDscmVyPT2)>PARM(PwrPllVyErDb))
    {
        PwrPllVy = 1.5F * (GmDscmVyPT2*S_IyCmd) * PARM(PwrPllVyErGn);
    }
    else
    {
        PwrPllVy = 0.0F;
    }
    PwrPllMod = PwrPllMod0 + PwrPllVy;

    PwrCmdPuT2     = PwrPllMod * TurbNpRatRcp;
    PwrPllAngFilZ1 = PwrPllAngFil;
    PwrPllAng      = PwrCmdPuT2 * PARM(PwrPllAngGn);
    PwrPllAngFil   = PwrPllAngFil  + (PwrPllFilGn * (PwrPllAng - PwrPllAngFil));
    PwrPllDelAng   = (PwrPllAngFil - PwrPllAngFilZ1);//Z1 could have been used in above filter, but not needed there
    PwrPllWFF      = PwrPllDelAng * DelTm2Rcp;

    //////////////////////////////////////////////////////////////////
    /// End Pll angle comp as a function of power -- APP_150
    //////////////////////////////////////////////// ////////////////



}
