///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Bridge thermal model (Btm) class for 2-level PWM inverter
//
//
// DESCRIPTION
//              Constants, function prototypes for bridge thermal model.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0BrgThrm.vsd, b0BrgThrm.xls
//
///////////////////////////////////////////////////////////////////////////////

#if 0       // This code is only for reference purposes. Do not use it !

#ifndef B0_BRG_THRM_H
#define B0_BRG_THRM_H


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product


// Constants
//----------


// Function Prototypes
//--------------------


// Types and Classes
//------------------


///////////////////////////////////////////////////////////////////////////////
//
// CLASS DESCRIPTION:
//      Thermal model implementation for a 2-level PWM voltage-source
//      inverter, three-phase topology. Each phase comprises two leg-modules,
//      each one packaging power switches, power diodes, and a temperature
//      sensor.
//
///////////////////////////////////////////////////////////////////////////////
class cBrgThrmModel
{
//-----------------------------------------------------------------------------
public:

    const float    BTM_GN_CONST;               // dimensionless units

    // Types and Classes
    //------------------
    struct InListTyp
    {
        const float&       BpTempAU;           //  1 - Baseplate temperature for upper devices of PhsA (degC) - Task3
        const float&       BpTempAL;           //  2 - Baseplate temperature for lower devices of PhsA (degC) - Task3
        const float&       BpTempBU;           //  3 - Baseplate temperature for upper devices of PhsB (degC) - Task3
        const float&       BpTempBL;           //  4 - Baseplate temperature for lower devices of PhsB (degC) - Task3
        const float&       BpTempCU;           //  5 - Baseplate temperature for upper devices of PhsC (degC) - Task3
        const float&       BpTempCL;           //  6 - Baseplate temperature for lower devices of PhsC (degC) - Task3
        const float&       VdcFbkT3;           //  7 - Dc link voltage fbk (Volts) - Task3
        const float&       IaFbk;              //  8 - Phase-A current feedback (Amps) - Task1
        const float&       IbFbk;              //  9 - Phase-B current feedback (Amps) - Task1
        const float&       IcFbk;              // 10 - Phase-C current feedback (Amps) - Task1
        const float&       ModUaCmdZ1;         // 11 - Phase A modulation output with one task latency (Pu) - Task1
        const float&       ModUbCmdZ1;         // 12 - Phase B modulation output with one task latency (Pu) - Task1
        const float&       ModUcCmdZ1;         // 13 - Phase C modulation output with one task latency (Pu) - Task1
        const int&         BpTempValid;        // 14 - Baseplate temperature fbks are valid (equal to 1) - Task3
        const unsigned&    BrgPwrEnb;          // 15 - Bridge power enable - Task1
    };

    struct InsListTyp
    {
        const float&        DelTm3;            //  1 - Delta time for task 3 (Sec)
        const float&        DelTm1Rcp;         //  2 - Delta time reciprocal for task 1 (1/Sec)
        const float&        HalfQuantPerT1Rcp; //  3 - Reciprocal of half quants per task1 (1/Quant)
        const float&        PwmPulseNumb;      //  4 - Number of PWM pulses per Task1
        const unsigned&     CfgForSep;         //  5 - Configure parms for Sep operation
    };

    // Methods
    //--------
    cBrgThrmModel( InListTyp InList, InsListTyp InsList );
    ~cBrgThrmModel();
    void Task1(void);
    void Task3(void);
    int  RunRules(void);

    // Output Signals
    //---------------
    CREATE_PUBVAR(BtmDPwrLoss     , float);         // Total diode power loss (Watts)
    CREATE_PUBVAR(BtmTrPwrLoss    , float);         // Total transistor power loss (Watts)
    CREATE_PUBVAR(BtmJncTemp      , float);         // Filtered max transistor junct temp (degC)
    CREATE_PUBVAR(BtmDJncTemp     , float);         // Filtered max diode junct temperature (degC)
    CREATE_PUBVAR(BtmOtFaultSum   , unsigned);      // Overtemperature fault summary
    CREATE_PUBVAR(BtmOtFaultDlySum, unsigned);      // Overtemperature delayed fault summary
    CREATE_PUBVAR(BtmOtAlarmSum   , unsigned);      // Overtemperature alarm summary

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;

    // Types and Classes
    //------------------

    // External Inputs
    //----------------
    InListTyp    In;
    InsListTyp   Ins;

    // Methods
    //--------
    //void PhaseLossCalcT1 (  float Iph, unsigned PwmCmdTZ1, int SwXnNdx[2],
    //                        PwrLossTyp& S1PwrT1, PwrLossTyp& S2PwrT1, PwrLossTyp& S3PwrT1, PwrLossTyp& S4PwrT1,
    //                        PwrLossTyp& D1PwrT1, PwrLossTyp& D2PwrT1, PwrLossTyp& D3PwrT1, PwrLossTyp& D4PwrT1,
    //                        PwrLossTyp& D5PwrT1, PwrLossTyp& D6PwrT1 );

    //void SemicondJuncTemp ( float SwPwr, float DiodPwr, float CoolntTemp, double& PackHsnkTemp,
    //                        double& SwBsPltTemp, float& SwSubstTemp, float& SwJncDelTemp, float& SwJncTemp,
    //                        double& DiBsPltTemp, float& DiSubstTemp, float& DiJncDelTemp, float& DiJncTemp);

    // Diagnostics
    //------------
    CREATE_DIAG(BtmAjncOtAlm);     // Bridge PhsA junction over-temp alarm
    CREATE_DIAG(BtmAjncOtDlyTrp);  // Bridge PhsA junction over-temp delayed trip
    CREATE_DIAG(BtmAjncOtTrp);     // Bridge PhsA junction over-temp trip
    CREATE_DIAG(BtmBjncOtAlm);     // Bridge PhsB junction over-temp alarm
    CREATE_DIAG(BtmBjncOtDlyTrp);  // Bridge PhsB junction over-temp delayed trip
    CREATE_DIAG(BtmBjncOtTrp);     // Bridge PhsB junction over-temp trip
    CREATE_DIAG(BtmCjncOtAlm);     // Bridge PhsC junction over-temp alarm
    CREATE_DIAG(BtmCjncOtDlyTrp);  // Bridge PhsC junction over-temp delayed trip
    CREATE_DIAG(BtmCjncOtTrp);     // Bridge PhsC junction over-temp trip

    // Parameters
    //-----------
    CREATE_PARM(BtmTrVo,            float); // Transistor constant voltage during conduction (Volts)
    CREATE_PARM(BtmTrR,             float); // Transistor resistance during conduction (Ohms)
    CREATE_PARM(BtmDiodeVo,         float); // Diode constant voltage during conduction (Volts)
    CREATE_PARM(BtmDiodeR,          float); // Diode resistance during conduction (Ohms)
    CREATE_PARM(BtmTrSw0,           float); // Transistor constant charge term for switching losses (Coulombs)
    CREATE_PARM(BtmTrSw1,           float); // Transistor linear charge term for switching losses (Coulombs/Amp)
    CREATE_PARM(BtmTrSw2,           float); // Transistor quadratic charge term for switching losses (Coulombs/Amp^2)
    CREATE_PARM(BtmDiodSw0,         float); // Diode constant charge term for switching losses (Coulombs)
    CREATE_PARM(BtmDiodSw1,         float); // Diode linear charge term for switching losses (Coulombs/Amp)
    CREATE_PARM(BtmDiodSw2,         float); // Diode quadratic charge term for switching losses (Coulombs/Amp^2)
    CREATE_PARM(BtmRaTh,            float); // Transistor junction to die thermal resistance (degC/Watt)
    CREATE_PARM(BtmDRaTh,           float); // Diode junction to die thermal resistance (degC/Watt)
    CREATE_PARM(BtmRbTh,            float); // Transistor die (point-A) to baseplate thermal resistance (degC/Watt)
    CREATE_PARM(BtmDRbTh,           float); // Diode die (point-A) to baseplate thermal resistance (degC/Watt)
    CREATE_PARM(BtmCaTh,            float); // Transistor die to reference thermal capacitance (Joul/degC)
    CREATE_PARM(BtmDCaTh,           float); // Diode die to reference thermal capacitance (Joul/degC)
    CREATE_PARM(BtmJncOTThr,        float); // Transistor junction over-temperature trip threshold (degC)
    CREATE_PARM(BtmJncOTThDly,      float); // Transistor junction over-temperature delayed trip threshold (degC)
    CREATE_PARM(BtmJncOTOn,         float); // Transistor junction activate over-temperature alarm threshold (degC)
    CREATE_PARM(BtmJncOTOff,        float); // Transistor junction deactivate over-temperature alarm threshold (degC)
    CREATE_PARM(BtmDJncOTThr,       float); // Diode junction over-temperature trip threshold (degC)
    CREATE_PARM(BtmDJncOTThDly,     float); // Diode junction over-temperature delayed trip threshold (degC)
    CREATE_PARM(BtmDJncOTOn,        float); // Diode junction activate over-temperature alarm threshold (degC)
    CREATE_PARM(BtmDJncOTOff,       float); // Diode junction deactivate over-temperature alarm threshold (degC)
    CREATE_PARM(BtmWjncUp,          float); // Filter omega for rising max junction temperatures (Rad/Sec)
    CREATE_PARM(BtmWjncDn,          float); // Filter omega for declining max junction temperatures (Rad/Sec)


    // Published Variables
    //--------------------
    CREATE_PUBVAR(BtmAlarmStPhsA,   unsigned);// Transistor alarm diagnostic state for phase A
    CREATE_PUBVAR(BtmAlarmStPhsB,   unsigned);// Transistor alarm diagnostic state for phase B
    CREATE_PUBVAR(BtmAlarmStPhsC,   unsigned);// Transistor alarm diagnostic state for phase C
    CREATE_PUBVAR(BtmAlarmStDphsA,  unsigned);// Diode alarm diagnostic state for phase A
    CREATE_PUBVAR(BtmAlarmStDphsB,  unsigned);// Diode alarm diagnostic state for phase B
    CREATE_PUBVAR(BtmAlarmStDphsC,  unsigned);// Diode alarm diagnostic state for phase C
    CREATE_PUBVAR(BtmIaAbs      ,   float);   // Absolute current feedbacks (Amps)
    CREATE_PUBVAR(BtmIbAbs      ,   float);
    CREATE_PUBVAR(BtmIcAbs      ,   float);
    CREATE_PUBVAR(BtmIaSqr      ,   float);   // Square current feedbacks (Amps)
    CREATE_PUBVAR(BtmIbSqr      ,   float);
    CREATE_PUBVAR(BtmIcSqr      ,   float);
    CREATE_PUBVAR(BtmUaOld      ,   float);   // Scaled modulator transition commands (Pu)
    CREATE_PUBVAR(BtmUbOld      ,   float);
    CREATE_PUBVAR(BtmUcOld      ,   float);
    CREATE_PUBVAR(BtmUOldThrHi  ,   float);   // Tresholds for valid modulator commands (Pu)
    CREATE_PUBVAR(BtmUOldThrLo  ,   float);
    CREATE_PUBVAR(BtmTrSwPwr    ,   float);   // Transistor power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrCondPwr  ,   float);   // Transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDSwPwr     ,   float);   // Diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmDCondPwr   ,   float);   // Diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrSwGn     ,   float);   // Approx rate of change of voltage across transistor (Volt/Sec)
    CREATE_PUBVAR(BtmTrSwGnT1   ,   float);   // Task1 version of BtmTrSwGn
    CREATE_PUBVAR(BtmDSwGn      ,   float);   // Approx rate of change of voltage across diode (Volt/Sec)
    CREATE_PUBVAR(BtmDSwGnT1    ,   float);   // Task1 version of BtmTrDSwGn
    CREATE_PUBVAR(BtmTrPwrAL    ,   float);   // Lower transistor power loss in PhsA (Watts)
    CREATE_PUBVAR(BtmTrPwrAU    ,   float);   // Upper transistor power loss in PhsA (Watts)
    CREATE_PUBVAR(BtmTrPwrBL    ,   float);   // Lower transistor power loss in PhsB (Watts)
    CREATE_PUBVAR(BtmTrPwrBU    ,   float);   // Upper transistor power loss in PhsB (Watts)
    CREATE_PUBVAR(BtmTrPwrCL    ,   float);   // Lower transistor power loss in PhsC (Watts)
    CREATE_PUBVAR(BtmTrPwrCU    ,   float);   // Upper transistor power loss in PhsC (Watts)
    CREATE_PUBVAR(BtmDPwrAL     ,   float);   // Lower diode power loss in PhsA (Watts)
    CREATE_PUBVAR(BtmDPwrAU     ,   float);   // Upper diode power loss in PhsA (Watts)
    CREATE_PUBVAR(BtmDPwrBL     ,   float);   // Lower diode power loss in PhsB (Watts)
    CREATE_PUBVAR(BtmDPwrBU     ,   float);   // Upper diode power loss in PhsB (Watts)
    CREATE_PUBVAR(BtmDPwrCL     ,   float);   // Lower diode power loss in PhsC (Watts)
    CREATE_PUBVAR(BtmDPwrCU     ,   float);   // Upper diode power loss in PhsC (Watts)
    CREATE_PUBVAR(BtmTrPwrALT3  ,   float);   // Task3 version of transistor power loss in PhsA (Watts)
    CREATE_PUBVAR(BtmTrPwrAUT3  ,   float);
    CREATE_PUBVAR(BtmTrPwrBLT3  ,   float);   // Task3 version of transistor power loss in PhsB (Watts)
    CREATE_PUBVAR(BtmTrPwrBUT3  ,   float);
    CREATE_PUBVAR(BtmTrPwrCLT3  ,   float);   // Task3 version of transistor power loss in PhsC (Watts)
    CREATE_PUBVAR(BtmTrPwrCUT3  ,   float);
    CREATE_PUBVAR(BtmDPwrALT3   ,   float);   // Task3 version of diode power loss in PhsA (Watts)
    CREATE_PUBVAR(BtmDPwrAUT3   ,   float);
    CREATE_PUBVAR(BtmDPwrBLT3   ,   float);   // Task3 version of diode power loss in PhsB (Watts)
    CREATE_PUBVAR(BtmDPwrBUT3   ,   float);
    CREATE_PUBVAR(BtmDPwrCLT3   ,   float);   // Task3 version of diode power loss in PhsC (Watts)
    CREATE_PUBVAR(BtmDPwrCUT3   ,   float);
    CREATE_PUBVAR(BtmRbRcp      ,   float);   // Reciprocal transtr die to baseplate thermal resistance (Watt/degC)
    CREATE_PUBVAR(BtmDRbRcp     ,   float);   // Reciprocal diode die to baseplate thermal resistance (Watt/degC)
    CREATE_PUBVAR(BtmTaGn       ,   float);   // Transistor die (point-A) integral thermal gain (degC/Watts)
    CREATE_PUBVAR(BtmDTaGn      ,   float);   // Diode die (point-A) integral thermal gain (degC/Watts)
    CREATE_PUBVAR(BtmTempAAL    ,   float);   // Lower transistor PhsA die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmTempAAU    ,   float);   // Upper transistor PhsA die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmTempABL    ,   float);   // Lower transistor PhsB die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmTempABU    ,   float);   // Upper transistor PhsB die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmTempACL    ,   float);   // Lower transistor PhsC die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmTempACU    ,   float);   // Upper transistor PhsC die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempAAL   ,   float);   // Lower diode PhsA die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempAAU   ,   float);   // Upper diode PhsA die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempABL   ,   float);   // Lower diode PhsB die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempABU   ,   float);   // Upper diode PhsB die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempACL   ,   float);   // Lower diode PhsC die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempACU   ,   float);   // Upper diode PhsC die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmABPwrA     ,   float);   // Total transistor pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmABPwrAL    ,   float);   // Lower transistor pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmABPwrAU    ,   float);   // Upper transistor pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmABPwrB     ,   float);   // Total transistor pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmABPwrBL    ,   float);   // Lower transistor pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmABPwrBU    ,   float);   // Upper transistor pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmABPwrC     ,   float);   // Total transistor pwr loss put in baseplate PhsC (Watts)
    CREATE_PUBVAR(BtmABPwrCL    ,   float);   // Lower transistor pwr loss put in baseplate PhsC (Watts)
    CREATE_PUBVAR(BtmABPwrCU    ,   float);   // Upper transistor pwr loss put in baseplate PhsC (Watts)
    CREATE_PUBVAR(BtmDABPwrA    ,   float);   // Total diode pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmDABPwrAL   ,   float);   // Lower diode pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmDABPwrAU   ,   float);   // Upper diode pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmDABPwrB    ,   float);   // Total diode pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmDABPwrBL   ,   float);   // Lower diode pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmDABPwrBU   ,   float);   // Upper diode pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmDABPwrC    ,   float);   // Total diode pwr loss put in baseplate PhsC (Watts)
    CREATE_PUBVAR(BtmDABPwrCL   ,   float);   // Lower diode pwr loss put in baseplate PhsC (Watts)
    CREATE_PUBVAR(BtmDABPwrCU   ,   float);   // Upper diode pwr loss put in baseplate PhsC (Watts)
    CREATE_PUBVAR(BtmTempJncAL  ,   float);   // Lower transistor junction temperature PhsA (degC)
    CREATE_PUBVAR(BtmTempJncAU  ,   float);   // Upper transistor junction temperature PhsA (degC)
    CREATE_PUBVAR(BtmTempJncBL  ,   float);   // Lower transistor junction temperature PhsB (degC)
    CREATE_PUBVAR(BtmTempJncBU  ,   float);   // Upper transistor junction temperature PhsB (degC)
    CREATE_PUBVAR(BtmTempJncCL  ,   float);   // Lower transistor junction temperature PhsC (degC)
    CREATE_PUBVAR(BtmTempJncCU  ,   float);   // Upper transistor junction temperature PhsC (degC)
    CREATE_PUBVAR(BtmDTempJncAL ,   float);   // Lower diode junction temperature PhsA (degC)
    CREATE_PUBVAR(BtmDTempJncAU ,   float);   // Upper diode junction temperature PhsA (degC)
    CREATE_PUBVAR(BtmDTempJncBL ,   float);   // Lower diode junction temperature PhsB (degC)
    CREATE_PUBVAR(BtmDTempJncBU ,   float);   // Upper diode junction temperature PhsB (degC)
    CREATE_PUBVAR(BtmDTempJncCL ,   float);   // Lower diode junction temperature PhsC (degC)
    CREATE_PUBVAR(BtmDTempJncCU ,   float);   // Upper diode junction temperature PhsC (degC)
    CREATE_PUBVAR(BtmJncTempMax ,   float);   // Maximum of all transistor junction temperatures (degC)
    CREATE_PUBVAR(BtmJncTempErr ,   float);   // Error between max and filtered transistor junct temp (degC)
    CREATE_PUBVAR(BtmDJncTempMx ,   float);   // Maximum of all diode junction temperatures (degC)
    CREATE_PUBVAR(BtmDJncTempErr,   float);   // Error between max and filtered diode junct temp (degC)
    CREATE_PUBVAR(BtmJncTempUpGn,   float);   // Filter gain for positive junct temp errors (dimensionless units)
    CREATE_PUBVAR(BtmJncTempDnGn,   float);   // Filter gain for negative junct temp errors (dimensionless units)
    CREATE_PUBVAR(BtmJncOvrTempThr, float);   // Transistor junction diagnostic over-temp trip threshold (degC)
    CREATE_PUBVAR(BtmJncOvrTempOn,  float);   // Transistor junction diagnostic over-temp alarm On level (degC)
    CREATE_PUBVAR(BtmJncOvrTempOff, float);   // Transistor junction diagnostic over-temp alarm Off level (degC)
    CREATE_PUBVAR(BtmDjncOvrTempThr,float);   // Diode junction diagnostic over-temp trip threshold (degC)
    CREATE_PUBVAR(BtmDjncOvrTempOn, float);   // Diode junction diagnostic over-temp alarm On level (degC)
    CREATE_PUBVAR(BtmDjncOvrTempOff,float);   // Diode junction diagnostic over-temp alarm Off level (degC)
    CREATE_PUBVAR(BtmTrSwchPwrAU,   float);   // Upper PhsA transistor power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrAL,   float);   // Lower PhsA transistor power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrBU,   float);   // Upper PhsB transistor power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrBL,   float);   // Lower PhsB transistor power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrCU,   float);   // Upper PhsC transistor power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrCL,   float);   // Lower PhsC transistor power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrCondPwrAU,   float);   // Upper PhsA transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrCondPwrAL,   float);   // Lower PhsA transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrCondPwrBU,   float);   // Upper PhsB transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrCondPwrBL,   float);   // Lower PhsB transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrCondPwrCU,   float);   // Upper PhsC transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrCondPwrCL,   float);   // Lower PhsC transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDswchPwrAU,    float);   // Upper PhsA diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmDswchPwrAL,    float);   // Lower PhsA diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmDswchPwrBU,    float);   // Upper PhsB diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmDswchPwrBL,    float);   // Lower PhsB diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmDswchPwrCU,    float);   // Upper PhsC diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmDswchPwrCL,    float);   // Lower PhsC diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmDcondPwrAU,    float);   // Upper PhsA diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDcondPwrAL,    float);   // Lower PhsA diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDcondPwrBU,    float);   // Upper PhsB diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDcondPwrBL,    float);   // Lower PhsB diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDcondPwrCU,    float);   // Upper PhsC diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDcondPwrCL,    float);   // Lower PhsC diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrCondPwrLoss, float);   // Total transistor conduction power loss (watts)
    CREATE_PUBVAR(BtmTrSwchPwrLoss, float);   // Total transistor switching power loss (watts)
    CREATE_PUBVAR(BtmDcondPwrLoss,  float);   // Total diode conduction power loss (watts)
    CREATE_PUBVAR(BtmDswchPwrLoss,  float);   // Total diode switching power loss (watts)


    // Unpublished Variables
    //----------------------

    // Data Passing
    //-------------
    DATA_PASS( BtmTrSwGn      , BtmTrSwGnT1      , T3_T1, UNFILTERED );     // Approx rate of change of voltage
    DATA_PASS( BtmDSwGn       , BtmDSwGnT1       , T3_T1, UNFILTERED );     // across transistor and diode passing to task1
    DATA_PASS( BtmTrPwrAL     , BtmTrPwrALT3     , T1_T3, FILTERED   );     // Bridge transistor power loss for all
    DATA_PASS( BtmTrPwrAU     , BtmTrPwrAUT3     , T1_T3, FILTERED   );     // phases passing to task3
    DATA_PASS( BtmTrPwrBL     , BtmTrPwrBLT3     , T1_T3, FILTERED   );
    DATA_PASS( BtmTrPwrBU     , BtmTrPwrBUT3     , T1_T3, FILTERED   );
    DATA_PASS( BtmTrPwrCL     , BtmTrPwrCLT3     , T1_T3, FILTERED   );
    DATA_PASS( BtmTrPwrCU     , BtmTrPwrCUT3     , T1_T3, FILTERED   );
    DATA_PASS( BtmDPwrAL      , BtmDPwrALT3      , T1_T3, FILTERED   );     // Bridge diode power loss for all
    DATA_PASS( BtmDPwrAU      , BtmDPwrAUT3      , T1_T3, FILTERED   );     // phases passing to task3
    DATA_PASS( BtmDPwrBL      , BtmDPwrBLT3      , T1_T3, FILTERED   );
    DATA_PASS( BtmDPwrBU      , BtmDPwrBUT3      , T1_T3, FILTERED   );
    DATA_PASS( BtmDPwrCL      , BtmDPwrCLT3      , T1_T3, FILTERED   );
    DATA_PASS( BtmDPwrCU      , BtmDPwrCUT3      , T1_T3, FILTERED   );

};

#endif  // B0_BRG_THRM_H

#endif
