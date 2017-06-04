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
//      b0BrgThrm5p.vsd, b0BrgThrm5p.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef B0_BRG_THRM5P_H
#define B0_BRG_THRM5P_H


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
//#include "x0BridgeInterface.h"
// product


// Constants
//----------

///////////////////////////////////////////////////////////////////////////////
//
// CONSTANT/MACRO DESCRIPTION
//    Calculation of temperature for a series-R and shunt-C network.
//    Connection node is denoted as 'X' and node following resistance is
//    identified as 'Y'. Also, calculation of power flow thru series
//    thermal resistance. Network is treated as a low-pass filter where
//    input given by (PwrFlowIn*Rth + YndTemp) is impressed accross thermal
//    capacitance, and output is the resulting temperature after filtering
//    the input stimulus.
//
// EXECUTION LEVEL
//      Task2 & Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
#define RC_NETWORK(Plist) \
    Plist.XndTemp += Plist.ThermGn * ((Plist.PwrFlowIn * Plist.XYrth + Plist.YndTemp) - Plist.XndTemp); \
    Plist.XYpwr    = Plist.XYrthRcp * (Plist.XndTemp - Plist.YndTemp);


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
//      sensor. If product type is less than DFIG02_2400_5060_A, will use Gen 3
//      IGBT thermal model, that equivalent thermal circuit is referenced to
//      baseplate temperature, which acts as a constant temperature source
//      connected to two transistor nodes (including junction), and two other
//      diode nodes. Otherwirse it is referenced to coolant temperature,
//      which acts as a constant temperature source connected to two common
//      nodes, three transistor nodes (including junction), and three other
//      diode nodes.
//
///////////////////////////////////////////////////////////////////////////////
class cBrgThrm5pModel
{
//-----------------------------------------------------------------------------
public:
    // Types and Classes
    //------------------
    struct InListTyp
    {
        const float&        VdcFbkT3;       //  1 - Dc link voltage fbk (Volts) - Task3
        const float&        CoolantTemp;    //  2 - Coolant temperature (degC) - Task3
        const float&        IaFbk;          //  3 - Phase-A current feedback (Amps) - Task1
        const float&        IbFbk;          //  4 - Phase-B current feedback (Amps) - Task1
        const float&        IcFbk;          //  5 - Phase-C current feedback (Amps) - Task1
        const float&        ModUaCmdZ1;     //  6 - Phase A modulation output with one task latency (Pu) - Task1
        const float&        ModUbCmdZ1;     //  7 - Phase B modulation output with one task latency (Pu) - Task1
        const float&        ModUcCmdZ1;     //  8 - Phase C modulation output with one task latency (Pu) - Task1
        const unsigned&     RefTempValid;   //  9 - Reference temperature fbk is valid (equal to 1) - Task3
        const unsigned&     BrgPwrEnb;      // 10 - Bridge power enable - Task1
        const unsigned&     IocPhsA;        // 11 - Fpga phase-A ioc fault indication - Task1
        const unsigned&     IocPhsB;        // 12 - Fpga phase-B ioc fault indication - Task1
        const unsigned&     IocPhsC;        // 13 - Fpga phase-C ioc fault indication - Task1
        const float&        BpTempA;        // 14 - Baseplate temperature for devices of PhsA (degC) - Task3
        const float&        BpTempB;        // 15 - Baseplate temperature for devices of PhsB (degC) - Task3
        const float&        BpTempC;        // 16 - Baseplate temperature for devices of PhsC (degC) - Task3
    };

    struct InsListTyp
    {
        const float&        DelTm2;            //  1 - Task 2 time (Secs)
        const float&        DelTm3;            //  2 - Delta time for task 3 (Sec)
        const float&        DelTm1Rcp;         //  3 - Delta time reciprocal for task 1 (1/Sec)
        const float&        IocThrs;           //  4 - Ioc detection threshold (Amps)
        const float&        HalfQuantPerT1Rcp; //  5 - Reciprocal of half quants per task1 (1/Quant)
        const float&        PwmPulseNumb;      //  6 - Number of PWM pulses per Task1
        const int&          ProductType;       //  7 - Configure parms for proper product type
        const int&          TestMode;          //  8 - Converter Test mode
        const unsigned&     IgbtParallel;      //  9 - Parallel IGBTs for 'true'
    };

#include "b0BrgThrm5pSim.inc"
    // Methods
    //--------
    cBrgThrm5pModel( InListTyp InList, InsListTyp InsList );
    ~cBrgThrm5pModel();
    void Task1(void);
    void Task2(void);
    void Task3(void);
    int  RunRules(void);
    void SimReadTemp(BtmTempSimTyp & target);

    // Output Signals
    //---------------
    CREATE_PUBVAR(BtmDpwrLoss     , float);         // Total diode power loss (Watts)
    CREATE_PUBVAR(BtmTrPwrLoss    , float);         // Total transistor power loss (Watts)
    CREATE_PUBVAR(BtmTrJncTemp    , float);         // Filtered max transistor junct temp (degC)
    CREATE_PUBVAR(BtmDjncTemp     , float);         // Filtered max diode junct temperature (degC)
    CREATE_PUBVAR(BtmOtFaultSum   , unsigned);      // Overtemperature fault summary
    CREATE_PUBVAR(BtmOtFaultDlySum, unsigned);      // Overtemperature delayed fault summary
    CREATE_PUBVAR(BtmOtAlarmSum   , unsigned);      // Overtemperature alarm summary
    CREATE_PUBVAR(BtmTrJncTempALT3, float);     // Task3 version of transistor junction temp in PhsA (degC)
    CREATE_PUBVAR(BtmTrJncTempAUT3, float);
    CREATE_PUBVAR(BtmTrJncTempBLT3, float);     // Task3 version of transistor junction temp in PhsB (degC)
    CREATE_PUBVAR(BtmTrJncTempBUT3, float);
    CREATE_PUBVAR(BtmTrJncTempCLT3, float);     // Task3 version of transistor junction temp in PhsC (degC)
    CREATE_PUBVAR(BtmTrJncTempCUT3, float);

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;

    // Types and Classes
    //------------------

    struct RcNetworkPlistTyp
    {
        // Structure's constructor to provide an initializer list
        RcNetworkPlistTyp (const float& PwrFlowIn, const float& YndTemp, const float& ThermGn,
                           const float& XYrthRcp, const float& XYrth,
                           float& XndTemp, float& XYpwr)
                         : PwrFlowIn(PwrFlowIn), YndTemp(YndTemp), ThermGn(ThermGn),
                           XYrthRcp(XYrthRcp), XYrth(XYrth),
                           XndTemp(XndTemp), XYpwr(XYpwr)
        {
        // Empty structure's constructor body, because curly braces cannot be omitted
        }

        // Inputs
        const float&  PwrFlowIn;    // power flowing into X node (Watts)
        const float&  YndTemp;      // temperature in Y node (degC)
        const float&  ThermGn;      // Thermal network gain between X and Y nodes
        const float&  XYrthRcp;     // reciprocal thermal resistance between X and Y nodes (Watt/degC)
        const float&  XYrth;        // thermal resistance between X and Y nodes (degC/Watt)
        // Outputs
              float&  XndTemp;      // temperature in X node (degC)
              float&  XYpwr;        // power flowing between X and Y nodes (Watts)
    };

    //struct BtmUcmdZ1Typ
    //{
    //    float         Duty;
    //    float         SwEventCnt;
    //    PhsStateTyp   LastState;
    //};

    // External Inputs
    //----------------
    InListTyp    In;
    InsListTyp   Ins;

    // Methods
    //--------
    void PowerLossModel_5P_T1(void);
    void PowerLossModel_2P_T1(void);
    void ThermalModel_5P_T2 (void);
    void ThermalModel_5P_T3 (void);
    void ThermalModel_2P_T3 (void);
    void Diagnostic_5P_T2 (void);
    void Diagnostic_2P_T3 (void);
    void MaxJncTempChk_5P_T2(void);
    void MaxJncTempChk_2P_T3(void);
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
    CREATE_PARM(BtmType,            int);       // Bridge thermal model type

    CREATE_PARM(BtmTrC1,            float);     // Transistor constant voltage during conduction (Volts)
    CREATE_PARM(BtmTrC2,            float);     // Transistor constant resistance during conduction (Ohms)
    CREATE_PARM(BtmTrC3,            float);     // Transistor constant rate of resistance change with current during conduction (Ohm/Amp)
    CREATE_PARM(BtmDiodC1,          float);     // Diode constant voltage during conduction (Volts)
    CREATE_PARM(BtmDiodC2,          float);     // Diode constant resistance during conduction (Ohms)
    CREATE_PARM(BtmDiodC3,          float);     // Diode constant rate of resistance change with current during conduction (Ohm/Amp)
    CREATE_PARM(BtmTrBondR,         float);     // Transistor bond-wire resistance (Ohms)
    CREATE_PARM(BtmDbondR,          float);     // Diode bond-wire resistance (Ohms)
    CREATE_PARM(BtmTrSw0,           float);     // Transistor constant charge term for switching losses (Coulombs)
    CREATE_PARM(BtmTrSw1,           float);     // Transistor linear charge term for switching losses (Coulombs/Amp)
    CREATE_PARM(BtmTrSw2,           float);     // Transistor quadratic charge term for switching losses (Coulombs/Amp^2)
    CREATE_PARM(BtmTrVdcSw0,        float);     // Transistor constant term of Vdc adjust factor for switching losses
    CREATE_PARM(BtmTrVdcSw1,        float);     // Transistor linear term of Vdc adjust factor for switching losses
    CREATE_PARM(BtmTrVdcSw2,        float);     // Transistor quadratic term of Vdc adjust factor for switching losses
    CREATE_PARM(BtmDiodSw0,         float);     // Diode constant charge term for switching losses (Coulombs)
    CREATE_PARM(BtmDiodSw1,         float);     // Diode linear charge term for switching losses (Coulombs/Amp)
    CREATE_PARM(BtmDiodSw2,         float);     // Diode quadratic charge term for switching losses (Coulombs/Amp^2)
    CREATE_PARM(BtmDiVdcSw0,        float);     // Diode constant term of Vdc adjust factor for switching losses
    CREATE_PARM(BtmDiVdcSw1,        float);     // Diode linear term of Vdc adjust factor for switching losses
    CREATE_PARM(BtmDiVdcSw2,        float);     // Diode quadratic term of Vdc adjust factor for switching losses
    CREATE_PARM(BtmIocThrs,         float);     // Current threshold at which hardware Ioc is declared (Amps)

    CREATE_PARM(BtmTrCjnc,          float);     // Transistor junction to reference thermal capacitance (Joul/degC)
    CREATE_PARM(BtmTrCnd4,          float);     // Transistor node 4 to reference thermal capacitance (Joul/degC)
    CREATE_PARM(BtmTrRthJncNd4,     float);     // Transistor junction to node 4 thermal resistance (degC/Watt)
    CREATE_PARM(BtmTrRthNd4Nd3,     float);     // Transistor node 4 to node 3 thermal resistance (degC/Watt)
    CREATE_PARM(BtmTrRthNd3Nd2,     float);     // Transistor node 3 to node 2 thermal resistance (degC/Watt)

    CREATE_PARM(BtmDcjnc,           float);     // Diode junction to reference thermal capacitance (Joul/degC)
    CREATE_PARM(BtmDcnd4,           float);     // Diode node 4 to reference thermal capacitance (Joul/degC)
    CREATE_PARM(BtmDrthJncNd4,      float);     // Diode junction to node 4 thermal resistance (degC/Watt)
    CREATE_PARM(BtmDrthNd4Nd3,      float);     // Diode node 4 to node 3 thermal resistance (degC/Watt)
    CREATE_PARM(BtmDrthNd3Nd2,      float);     // Diode node 3 to node 2 thermal resistance (degC/Watt)

    CREATE_PARM(BtmCnd1,            float);     // Common node 1 to reference thermal capacitance (Joul/degC)
    CREATE_PARM(BtmRthNd2Nd1,       float);     // Common node 2 to node 1 thermal resistance (degC/Watt)
    CREATE_PARM(BtmRthNd1Clnt,      float);     // Common node 1 to coolant thermal resistance (degC/Watt)

    CREATE_PARM(BtmTrJncOTThr,      float);     // Transistor junction over-temperature trip threshold (degC)
    CREATE_PARM(BtmTrJncOTThDly,    float);     // Transistor junction over-temperature delayed trip threshold (degC)
    CREATE_PARM(BtmTrJncOTOn,       float);     // Transistor junction activate over-temperature alarm threshold (degC)
    CREATE_PARM(BtmTrJncOTOff,      float);     // Transistor junction deactivate over-temperature alarm threshold (degC)
    CREATE_PARM(BtmDjncOTThr,       float);     // Diode junction over-temperature trip threshold (degC)
    CREATE_PARM(BtmDjncOTThDly,     float);     // Diode junction over-temperature delayed trip threshold (degC)
    CREATE_PARM(BtmDjncOTOn,        float);     // Diode junction activate over-temperature alarm threshold (degC)
    CREATE_PARM(BtmDjncOTOff,       float);     // Diode junction deactivate over-temperature alarm threshold (degC)
    CREATE_PARM(BtmWjncUp,          float);     // Filter omega for rising max junction temperatures (Rad/Sec)
    CREATE_PARM(BtmWjncDn,          float);     // Filter omega for declining max junction temperatures (Rad/Sec)

    // for 2P
    CREATE_PARM(BtmRaTh,            float);     // Transistor junction to die thermal resistance (degC/Watt)
    CREATE_PARM(BtmDRaTh,           float);     // Diode junction to die thermal resistance (degC/Watt)
    CREATE_PARM(BtmRbTh,            float);     // Transistor die (point-A) to baseplate thermal resistance (degC/Watt)
    CREATE_PARM(BtmDRbTh,           float);     // Diode die (point-A) to baseplate thermal resistance (degC/Watt)
    CREATE_PARM(BtmCaTh,            float);     // Transistor die to reference thermal capacitance (Joul/degC)
    CREATE_PARM(BtmDCaTh,           float);     // Diode die to reference thermal capacitance (Joul/degC)

    // Published Variables
    //--------------------
    CREATE_PUBVAR(Btm2PmdlSel,      unsigned);  // Select 2P model
    // -------- task1 --------
    CREATE_PUBVAR(BtmDcondPwr     , float);     // Diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrCondPwr    , float);     // Transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDswPwr       , float);     // Diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrSwPwr      , float);     // Transistor power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmDbondPwr     , float);     // Diode bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmTrBondPwr    , float);     // Transistor bond-wire power loss (Watts)

                                                // phase A
    CREATE_PUBVAR(BtmDbondPwrAL   , float);     // Lower PhsA diode bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmDbondPwrAU   , float);     // Upper PhsA diode bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmDcondPwrAL   , float);     // Lower PhsA diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDcondPwrAU   , float);     // Upper PhsA diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDswchPwrAL   , float);     // Lower PhsA diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmDswchPwrAU   , float);     // Upper PhsA diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrBondPwrAL  , float);     // Lower PhsA transistor bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmTrBondPwrAU  , float);     // Upper PhsA transistor bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmTrCondPwrAL  , float);     // Lower PhsA transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrCondPwrAU  , float);     // Upper PhsA transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrAL  , float);     // Lower PhsA transistor power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrAU  , float);     // Upper PhsA transistor power loss due to a switching event (Watts)

    CREATE_PUBVAR(BtmIaAbs        , float);     // Absolute current feedbacks (Amps)
    CREATE_PUBVAR(BtmIaSqr        , float);     // Square current feedbacks (Amps^2)
    CREATE_PUBVAR(BtmDpwrAL       , float);     // Lower diode power loss in PhsA (Watts)
    CREATE_PUBVAR(BtmDpwrAU       , float);     // Upper diode power loss in PhsA (Watts)
    CREATE_PUBVAR(BtmTrPwrAL      , float);     // Lower transistor power loss in PhsA (Watts)
    CREATE_PUBVAR(BtmTrPwrAU      , float);     // Upper transistor power loss in PhsA (Watts)
    CREATE_PUBVAR(BtmUaOld        , float);     // Scaled modulator transition commands (Pu)

                                                // phase B
    CREATE_PUBVAR(BtmDbondPwrBL   , float);     // Lower PhsB diode bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmDbondPwrBU   , float);     // Upper PhsB diode bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmDcondPwrBL   , float);     // Lower PhsB diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDcondPwrBU   , float);     // Upper PhsB diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDswchPwrBL   , float);     // Lower PhsB diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmDswchPwrBU   , float);     // Upper PhsB diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrBondPwrBL  , float);     // Lower PhsB transistor bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmTrBondPwrBU  , float);     // Upper PhsB transistor bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmTrCondPwrBL  , float);     // Lower PhsB transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrCondPwrBU  , float);     // Upper PhsB transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrBL  , float);     // Lower PhsB transistor power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrBU  , float);     // Upper PhsB transistor power loss due to a switching event (Watts)

    CREATE_PUBVAR(BtmIbAbs      ,   float);     // PhsB absolute current feedbacks (Amps)
    CREATE_PUBVAR(BtmIbSqr      ,   float);     // PhsB square current feedbacks (Amps^2)
    CREATE_PUBVAR(BtmDpwrBL     ,   float);     // Lower diode power loss in PhsB (Watts)
    CREATE_PUBVAR(BtmDpwrBU     ,   float);     // Upper diode power loss in PhsB (Watts)
    CREATE_PUBVAR(BtmTrPwrBL    ,   float);     // Lower transistor power loss in PhsB (Watts)
    CREATE_PUBVAR(BtmTrPwrBU    ,   float);     // Upper transistor power loss in PhsB (Watts)
    CREATE_PUBVAR(BtmUbOld        , float);     // PhsB scaled modulator transition commands (Pu)

                                                // phase C
    CREATE_PUBVAR(BtmDbondPwrCL   , float);     // Lower PhsC diode bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmDbondPwrCU   , float);     // Upper PhsC diode bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmDcondPwrCL   , float);     // Lower PhsC diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDcondPwrCU   , float);     // Upper PhsC diode power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmDswchPwrCL   , float);     // Lower PhsC diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmDswchPwrCU   , float);     // Upper PhsC diode power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrBondPwrCL  , float);     // Lower PhsC transistor bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmTrBondPwrCU  , float);     // Upper PhsC transistor bond-wire power loss (Watts)
    CREATE_PUBVAR(BtmTrCondPwrCL  , float);     // Lower PhsC transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrCondPwrCU  , float);     // Upper PhsC transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrCL  , float);     // Lower PhsC transistor power loss due to a switching event (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrCU  , float);     // Upper PhsC transistor power loss due to a switching event (Watts)

    CREATE_PUBVAR(BtmIcAbs      ,   float);     // PhsC absolute current feedbacks (Amps)
    CREATE_PUBVAR(BtmIcSqr      ,   float);     // PhsC square current feedbacks (Amps^2)
    CREATE_PUBVAR(BtmDpwrCL     ,   float);     // Lower diode power loss in PhsC (Watts)
    CREATE_PUBVAR(BtmDpwrCU     ,   float);     // Upper diode power loss in PhsC (Watts)
    CREATE_PUBVAR(BtmTrPwrCL    ,   float);     // Lower transistor power loss in PhsC (Watts)
    CREATE_PUBVAR(BtmTrPwrCU    ,   float);     // Upper transistor power loss in PhsC (Watts)
    CREATE_PUBVAR(BtmUcOld      ,   float);     // PhsC scaled modulator transition commands (Pu)

    CREATE_PUBVAR(BtmUOldThrHi  ,   float);     // Tresholds for valid modulator commands (Pu)
    CREATE_PUBVAR(BtmUOldThrLo  ,   float);
    CREATE_PUBVAR(BtmTrSwGnT1   ,   float);     // Task1 version of BtmTrSwGn
    CREATE_PUBVAR(BtmDswGnT1    ,   float);     // Task1 version of BtmTrDswGn

    CREATE_PUBVAR(BtmTrCondPwrLoss, float);     // Total transistor conduction power loss (Watts)
    CREATE_PUBVAR(BtmTrSwchPwrLoss, float);     // Total transistor switching power loss (Watts)
    CREATE_PUBVAR(BtmDcondPwrLoss , float);     // Total diode conduction power loss (Watts)
    CREATE_PUBVAR(BtmDswchPwrLoss , float);     // Total diode switching power loss (Watts)
    CREATE_PUBVAR(BtmBondPwrLoss  , float);     // Total bond-wire power loss (Watts)

    // -------- task2 --------
    CREATE_PUBVAR(BtmRefTempValidT2,unsigned);  // Task2 version of reference temperature validation
    CREATE_PUBVAR(BtmTrCnd4Gn     , float);     // Transistor node 4 integral thermal gain (degC/Watts)
    CREATE_PUBVAR(BtmTrRthNd4Nd3Rcp,float);     // Reciprocal transistor node 4 to node 3 thermal resistance (Watt/degC)
    CREATE_PUBVAR(BtmTrRthNd4Nd2Rcp,float);     // Reciprocal transistor node 4 to node 2 thermal resistance (Watt/degC)
    CREATE_PUBVAR(BtmDcnd4Gn      , float);     // Diode node 4 integral thermal gain (degC/Watts)
    CREATE_PUBVAR(BtmDrthNd4Nd3Rcp, float);     // Reciprocal diode node 4 to node 3 thermal resistance (Watt/degC)
    CREATE_PUBVAR(BtmDrthNd4Nd2Rcp, float);     // Reciprocal diode node 4 to node 2 thermal resistance (Watt/degC)
    CREATE_PUBVAR(BtmTrJnGn       , float);     // Thermal RC network gain for transistor junctions
    CREATE_PUBVAR(BtmTrRthJnNd4Rcp, float);     // Reciprocal transistor junction to node 4 thermal resistance (Watt/degC)
    CREATE_PUBVAR(BtmDjnGn        , float);     // Thermal RC network gain for diode junctions
    CREATE_PUBVAR(BtmDrthJnNd4Rcp , float);     // Reciprocal diode junction to node 4 thermal resistance (Watt/degC)
    CREATE_PUBVAR(BtmRthNd4Nd2Prll, float);     // Parallel of trans and diode thermal resistances between node 4 and node 2 (degC/Watts)
    CREATE_PUBVAR(BtmNd2Divider,    float);     // Thermal resistance divider ratio onto node 2
    CREATE_PUBVAR(BtmRthNd2Nd1Rcp,  float);     // Reciprocal node 2 to node 1 thermal resistance (Watt/degC)

                                                // phase A
    CREATE_PUBVAR(BtmAlarmStDphsA , unsigned);  // Diode alarm diagnostic state for phase A
    CREATE_PUBVAR(BtmAlarmStPhsA  , unsigned);  // Transistor alarm diagnostic state for phase A
    CREATE_PUBVAR(BtmDbondPwrALT2 , float);     // Task2 version of lower PhsA diode bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmDbondPwrAUT2 , float);     // Task2 version of upper PhsA diode bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmDjncTempAL   , float);     // Lower diode junction temperature PhsA (degC)
    CREATE_PUBVAR(BtmDjncTempAU   , float);     // Upper diode junction temperature PhsA (degC)
    CREATE_PUBVAR(BtmDpwrNd3Nd2AL , float);     // Lower diode pwr loss flow between nodes 3 and 2 PhsA (Watts)
    CREATE_PUBVAR(BtmDpwrNd3Nd2AU , float);     // Upper diode pwr loss flow between nodes 3 and 2 PhsA (Watts)
    CREATE_PUBVAR(BtmDnd3TempAL   , float);     // Lower diode node 3 temperature PhsA (degC)
    CREATE_PUBVAR(BtmDnd3TempAU   , float);     // Upper diode node 3 temperature PhsA (degC)
    CREATE_PUBVAR(BtmDnd4TempAL   , float);     // Lower diode PhsA node 4 temperature (degC)
    CREATE_PUBVAR(BtmDnd4TempAU   , float);     // Upper diode PhsA node 4 temperature (degC)
    CREATE_PUBVAR(BtmDpwrALT2     , float);     // Task2 version of low diode pwr loss flow into junct PhsA (Watts)
    CREATE_PUBVAR(BtmDpwrAUT2     , float);     // Task2 version of up diode pwr loss flow into junct PhsA (Watts)
    CREATE_PUBVAR(BtmDpwrJnNd4AL  , float);     // Lower diode pwr loss flow between junct and node 4 PhsA (Watts)
    CREATE_PUBVAR(BtmDpwrJnNd4AU  , float);     // Upper diode pwr loss flow between junct and node 4 PhsA (Watts)
    CREATE_PUBVAR(BtmNd1TempALT2  , float);     // Task2 version of lower node 1 temperature PhsA (degC)
    CREATE_PUBVAR(BtmNd1TempAUT2  , float);     // Task2 version of upper node 1 temperature PhsA (degC)
    CREATE_PUBVAR(BtmNd2TempAL    , float);     // Lower node 2 temperature PhsA (degC)
    CREATE_PUBVAR(BtmNd2TempAU    , float);     // Upper node 2 temperature PhsA (degC)
    CREATE_PUBVAR(BtmPwrNd2Nd1AL  , float);     // Lower pwr loss flow between nodes 2 and 1 PhsA (Watts)
    CREATE_PUBVAR(BtmPwrNd2Nd1AU  , float);     // Upper pwr loss flow between nodes 2 and 1 PhsA (Watts)
    CREATE_PUBVAR(BtmTrBondPwrALT2, float);     // Task2 version of lower PhsA trans bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmTrBondPwrAUT2, float);     // Task2 version of upper PhsA trans bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmTrJncTempAL  , float);     // Lower transistor junction temperature PhsA (degC)
    CREATE_PUBVAR(BtmTrJncTempAU  , float);     // Upper transistor junction temperature PhsA (degC)
    CREATE_PUBVAR(BtmTrPwrNd3Nd2AL, float);     // Lower transistor pwr loss flow between nodes 3 and 2 PhsA (Watts)
    CREATE_PUBVAR(BtmTrPwrNd3Nd2AU, float);     // Upper transistor pwr loss flow between nodes 3 and 2 PhsA (Watts)
    CREATE_PUBVAR(BtmTrNd3TempAL  , float);     // Lower transistor node 3 temperature PhsA (degC)
    CREATE_PUBVAR(BtmTrNd3TempAU  , float);     // Upper transistor node 3 temperature PhsA (degC)
    CREATE_PUBVAR(BtmTrNd4TempAL  , float);     // Lower transistor PhsA node 4 temperature (degC)
    CREATE_PUBVAR(BtmTrNd4TempAU  , float);     // Upper transistor PhsA node 4 temperature (degC)
    CREATE_PUBVAR(BtmTrPwrALT2    , float);     // Task2 version of low trans pwr loss flow into junct PhsA (Watts)
    CREATE_PUBVAR(BtmTrPwrAUT2    , float);     // Task2 version of up trans pwr loss flow into junct PhsA (Watts)
    CREATE_PUBVAR(BtmTrPwrJnNd4AL , float);     // Lower transistor pwr loss flow between junct and node 4 PhsA (Watts)
    CREATE_PUBVAR(BtmTrPwrJnNd4AU , float);     // Upper transistor pwr loss flow between junct and node 4 PhsA (Watts)

                                                // phase B
    CREATE_PUBVAR(BtmAlarmStDphsB , unsigned);  // Diode alarm diagnostic state for phase B
    CREATE_PUBVAR(BtmAlarmStPhsB  , unsigned);  // Transistor alarm diagnostic state for phase B
    CREATE_PUBVAR(BtmDbondPwrBLT2 , float);     // Task2 version of lower PhsB diode bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmDbondPwrBUT2 , float);     // Task2 version of upper PhsB diode bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmDjncTempBL ,   float);     // Lower diode junction temperature PhsB (degC)
    CREATE_PUBVAR(BtmDjncTempBU ,   float);     // Upper diode junction temperature PhsB (degC)
    CREATE_PUBVAR(BtmDpwrNd3Nd2BL , float);     // Lower diode pwr loss flow between nodes 3 and 2 PhsB (Watts)
    CREATE_PUBVAR(BtmDpwrNd3Nd2BU , float);     // Upper diode pwr loss flow between nodes 3 and 2 PhsB (Watts)
    CREATE_PUBVAR(BtmDnd3TempBL   , float);     // Lower diode node 3 temperature PhsB (degC)
    CREATE_PUBVAR(BtmDnd3TempBU   , float);     // Upper diode node 3 temperature PhsB (degC)
    CREATE_PUBVAR(BtmDnd4TempBL   , float);     // Lower diode PhsB node 4 temperature (degC)
    CREATE_PUBVAR(BtmDnd4TempBU   , float);     // Upper diode PhsB node 4 temperature (degC)
    CREATE_PUBVAR(BtmDpwrBLT2,      float);     // Task2 version of low diode pwr loss flow into junct PhsB (Watts)
    CREATE_PUBVAR(BtmDpwrBUT2,      float);     // Task2 version of up diode pwr loss flow into junct PhsB (Watts)
    CREATE_PUBVAR(BtmDpwrJnNd4BL,   float);     // Lower diode pwr loss flow between junct and node 4 PhsB (Watts)
    CREATE_PUBVAR(BtmDpwrJnNd4BU,   float);     // Upper diode pwr loss flow between junct and node 4 PhsB (Watts)
    CREATE_PUBVAR(BtmNd1TempBLT2  , float);     // Task2 version of lower node 1 temperature PhsB (degC)
    CREATE_PUBVAR(BtmNd1TempBUT2  , float);     // Task2 version of upper node 1 temperature PhsB (degC)
    CREATE_PUBVAR(BtmNd2TempBL    , float);     // Lower node 2 temperature PhsB (degC)
    CREATE_PUBVAR(BtmNd2TempBU    , float);     // Upper node 2 temperature PhsB (degC)
    CREATE_PUBVAR(BtmPwrNd2Nd1BL  , float);     // Lower pwr loss flow between nodes 2 and 1 PhsB (Watts)
    CREATE_PUBVAR(BtmPwrNd2Nd1BU  , float);     // Upper pwr loss flow between nodes 2 and 1 PhsB (Watts)
    CREATE_PUBVAR(BtmTrBondPwrBLT2, float);     // Task2 version of lower PhsB trans bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmTrBondPwrBUT2, float);     // Task2 version of upper PhsB trans bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmTrJncTempBL,   float);     // Lower transistor junction temperature PhsB (degC)
    CREATE_PUBVAR(BtmTrJncTempBU,   float);     // Upper transistor junction temperature PhsB (degC)
    CREATE_PUBVAR(BtmTrPwrNd3Nd2BL, float);     // Lower transistor pwr loss flow between nodes 3 and 2 PhsB (Watts)
    CREATE_PUBVAR(BtmTrPwrNd3Nd2BU, float);     // Upper transistor pwr loss flow between nodes 3 and 2 PhsB (Watts)
    CREATE_PUBVAR(BtmTrNd3TempBL  , float);     // Lower transistor node 3 temperature PhsB (degC)
    CREATE_PUBVAR(BtmTrNd3TempBU  , float);     // Upper transistor node 3 temperature PhsB (degC)
    CREATE_PUBVAR(BtmTrNd4TempBL  , float);     // Lower transistor PhsB node 4 temperature (degC)
    CREATE_PUBVAR(BtmTrNd4TempBU  , float);     // Upper transistor PhsB node 4 temperature (degC)
    CREATE_PUBVAR(BtmTrPwrBLT2,     float);     // Task2 version of low trans pwr loss flow into junct PhsB (Watts)
    CREATE_PUBVAR(BtmTrPwrBUT2,     float);     // Task2 version of up trans pwr loss flow into junct PhsB (Watts)
    CREATE_PUBVAR(BtmTrPwrJnNd4BL,  float);     // Lower transistor pwr loss flow between junct and node 4 PhsB (Watts)
    CREATE_PUBVAR(BtmTrPwrJnNd4BU,  float);     // Upper transistor pwr loss flow between junct and node 4 PhsB (Watts)

                                                // phase C
    CREATE_PUBVAR(BtmAlarmStDphsC,  unsigned);  // Diode alarm diagnostic state for phase C
    CREATE_PUBVAR(BtmAlarmStPhsC,   unsigned);  // Transistor alarm diagnostic state for phase C
    CREATE_PUBVAR(BtmDbondPwrCLT2 , float);     // Task2 version of lower PhsC diode bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmDbondPwrCUT2 , float);     // Task2 version of upper PhsC diode bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmDjncTempCL ,   float);     // Lower diode junction temperature PhsC (degC)
    CREATE_PUBVAR(BtmDjncTempCU ,   float);     // Upper diode junction temperature PhsC (degC)
    CREATE_PUBVAR(BtmDpwrNd3Nd2CL , float);     // Lower diode pwr loss flow between nodes 3 and 2 PhsC (Watts)
    CREATE_PUBVAR(BtmDpwrNd3Nd2CU , float);     // Upper diode pwr loss flow between nodes 3 and 2 PhsC (Watts)
    CREATE_PUBVAR(BtmDnd3TempCL   , float);     // Lower diode node 3 temperature PhsC (degC)
    CREATE_PUBVAR(BtmDnd3TempCU   , float);     // Upper diode node 3 temperature PhsC (degC)
    CREATE_PUBVAR(BtmDnd4TempCL   , float);     // Lower diode PhsC node 4 temperature (degC)
    CREATE_PUBVAR(BtmDnd4TempCU   , float);     // Upper diode PhsC node 4 temperature (degC)
    CREATE_PUBVAR(BtmDpwrCLT2,      float);     // Task2 version of low diode pwr loss flow into junct PhsC (Watts)
    CREATE_PUBVAR(BtmDpwrCUT2,      float);     // Task2 version of up diode pwr loss flow into junct PhsC (Watts)
    CREATE_PUBVAR(BtmDpwrJnNd4CL,   float);     // Lower diode pwr loss flow between junct and node 4 PhsC (Watts)
    CREATE_PUBVAR(BtmDpwrJnNd4CU,   float);     // Upper diode pwr loss flow between junct and node 4 PhsC (Watts)
    CREATE_PUBVAR(BtmNd1TempCLT2  , float);     // Task2 version of lower node 1 temperature PhsC (degC)
    CREATE_PUBVAR(BtmNd1TempCUT2  , float);     // Task2 version of upper node 1 temperature PhsC (degC)
    CREATE_PUBVAR(BtmNd2TempCL    , float);     // Lower node 2 temperature PhsC (degC)
    CREATE_PUBVAR(BtmNd2TempCU    , float);     // Upper node 2 temperature PhsC (degC)
    CREATE_PUBVAR(BtmPwrNd2Nd1CL  , float);     // Lower pwr loss flow between nodes 2 and 1 PhsC (Watts)
    CREATE_PUBVAR(BtmPwrNd2Nd1CU  , float);     // Upper pwr loss flow between nodes 2 and 1 PhsC (Watts)
    CREATE_PUBVAR(BtmTrBondPwrCLT2, float);     // Task2 version of lower PhsC trans bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmTrBondPwrCUT2, float);     // Task2 version of upper PhsC trans bond-wire powr loss (Watts)
    CREATE_PUBVAR(BtmTrJncTempCL,   float);     // Lower transistor junction temperature PhsC (degC)
    CREATE_PUBVAR(BtmTrJncTempCU,   float);     // Upper transistor junction temperature PhsC (degC)
    CREATE_PUBVAR(BtmTrPwrNd3Nd2CL, float);     // Lower transistor pwr loss flow between nodes 3 and 2 PhsC (Watts)
    CREATE_PUBVAR(BtmTrPwrNd3Nd2CU, float);     // Upper transistor pwr loss flow between nodes 3 and 2 PhsC (Watts)
    CREATE_PUBVAR(BtmTrNd3TempCL  , float);     // Lower transistor node 3 temperature PhsC (degC)
    CREATE_PUBVAR(BtmTrNd3TempCU  , float);     // Upper transistor node 3 temperature PhsC (degC)
    CREATE_PUBVAR(BtmTrNd4TempCL  , float);     // Lower transistor PhsC node 4 temperature (degC)
    CREATE_PUBVAR(BtmTrNd4TempCU  , float);     // Upper transistor PhsC node 4 temperature (degC)
    CREATE_PUBVAR(BtmTrPwrCLT2,     float);     // Task2 version of low trans pwr loss flow into junct PhsC (Watts)
    CREATE_PUBVAR(BtmTrPwrCUT2,     float);     // Task2 version of up trans pwr loss flow into junct PhsC (Watts)
    CREATE_PUBVAR(BtmTrPwrJnNd4CL,  float);     // Lower transistor pwr loss flow between junct and node 4 PhsC (Watts)
    CREATE_PUBVAR(BtmTrPwrJnNd4CU,  float);     // Upper transistor pwr loss flow between junct and node 4 PhsC (Watts)

    // -------- task3 --------
    CREATE_PUBVAR(BtmDjncTempALT3 , float);     // Task3 version of diode junction temp in PhsA (degC)
    CREATE_PUBVAR(BtmDjncTempAUT3 , float);
    CREATE_PUBVAR(BtmNd1TempAL    , float);     // Lower node 1 temperature PhsA (degC)
    CREATE_PUBVAR(BtmNd1TempAU    , float);     // Upper node 1 temperature PhsA (degC)
    CREATE_PUBVAR(BtmPwrNd1ClnAL  , float);     // Lower pwr loss flow between node 1 and coolant PhsA (Watts)
    CREATE_PUBVAR(BtmPwrNd1ClnAU  , float);     // Upper pwr loss flow between node 1 and coolant PhsA (Watts)
    CREATE_PUBVAR(BtmPwrNd2Nd1ALT3, float);     // Task3 version of lower pwr loss flow between nodes 2 and 1 PhsA (Watts)
    CREATE_PUBVAR(BtmPwrNd2Nd1AUT3, float);     // Task3 version of upper  pwr loss flow between nodes 2 and 1 PhsA (Watts)

    CREATE_PUBVAR(BtmDjncTempBLT3 , float);     // Task3 version of diode junction temp in PhsB (degC)
    CREATE_PUBVAR(BtmDjncTempBUT3 , float);
    CREATE_PUBVAR(BtmNd1TempBL    , float);     // Lower node 1 temperature PhsB (degC)
    CREATE_PUBVAR(BtmNd1TempBU    , float);     // Upper node 1 temperature PhsB (degC)
    CREATE_PUBVAR(BtmPwrNd1ClnBL  , float);     // Lower pwr loss flow between node 1 and coolant PhsB (Watts)
    CREATE_PUBVAR(BtmPwrNd1ClnBU  , float);     // Upper pwr loss flow between node 1 and coolant PhsB (Watts)
    CREATE_PUBVAR(BtmPwrNd2Nd1BLT3, float);     // Task3 version of lower pwr loss flow between nodes 2 and 1 PhsB (Watts)
    CREATE_PUBVAR(BtmPwrNd2Nd1BUT3, float);     // Task3 version of upper  pwr loss flow between nodes 2 and 1 PhsB (Watts)

    CREATE_PUBVAR(BtmDjncTempCLT3 , float);     // Task3 version of diode junction temp in PhsC (degC)
    CREATE_PUBVAR(BtmDjncTempCUT3 , float);
    CREATE_PUBVAR(BtmNd1TempCU    , float);     // Upper node 1 temperature PhsC (degC)
    CREATE_PUBVAR(BtmNd1TempCL    , float);     // Lower node 1 temperature PhsC (degC)
    CREATE_PUBVAR(BtmPwrNd1ClnCL  , float);     // Lower pwr loss flow between node 1 and coolant PhsC (Watts)
    CREATE_PUBVAR(BtmPwrNd1ClnCU  , float);     // Upper pwr loss flow between node 1 and coolant PhsC (Watts)
    CREATE_PUBVAR(BtmPwrNd2Nd1CLT3, float);     // Task3 version of lower pwr loss flow between nodes 2 and 1 PhsC (Watts)
    CREATE_PUBVAR(BtmPwrNd2Nd1CUT3, float);     // Task3 version of upper  pwr loss flow between nodes 2 and 1 PhsC (Watts)

    CREATE_PUBVAR(BtmCnd1Gn       , float);     // Node 1 integral thermal gain (degC/Watts)
    CREATE_PUBVAR(BtmDjncTempErr,   float);     // Error between max and filtered diode junct temp (degC)
    CREATE_PUBVAR(BtmDjncTempMx ,   float);     // Maximum of all diode junction temperatures (degC)
    CREATE_PUBVAR(BtmDswGn        , float);     // Approx rate of change of voltage across diode (Volt/Sec)
    CREATE_PUBVAR(BtmJncTempDnGn,   float);     // Filter gain for negative junct temp errors (dimensionless units)
    CREATE_PUBVAR(BtmJncTempUpGn,   float);     // Filter gain for positive junct temp errors (dimensionless units)
    CREATE_PUBVAR(BtmRefTempValid , unsigned);  // Reference temperature validation
    CREATE_PUBVAR(BtmRthNd1ClnRcp , float);     // Reciprocal node 1 to coolant thermal resistance (Watt/degC)
    CREATE_PUBVAR(BtmTrJncTempErr,  float);     // Error between max and filtered transistor junct temp (degC)
    CREATE_PUBVAR(BtmTrJncTempMax,  float);     // Maximum of all transistor junction temperatures (degC)
    CREATE_PUBVAR(BtmTrSwGn       , float);     // Approx rate of change of voltage across transistor (Volt/Sec)

    CREATE_PUBVAR(BtmTrJncOvrTmpThr,float);     // Transistor junction diagnostic over-temp trip threshold (degC)
    CREATE_PUBVAR(BtmTrJncOvrTmpOn, float);     // Transistor junction diagnostic over-temp alarm On level (degC)
    CREATE_PUBVAR(BtmTrJncOvrTmpOff,float);     // Transistor junction diagnostic over-temp alarm Off level (degC)
    CREATE_PUBVAR(BtmDjncOvrTempThr,float);     // Diode junction diagnostic over-temp trip threshold (degC)
    CREATE_PUBVAR(BtmDjncOvrTempOn, float);     // Diode junction diagnostic over-temp alarm On level (degC)
    CREATE_PUBVAR(BtmDjncOvrTempOff,float);     // Diode junction diagnostic over-temp alarm Off level (degC)

    // 2p model
    CREATE_PUBVAR(BtmTrPwrAUT3,     float);     // Task3 version of upper transistor pwr loss flow into junct PhsA (Watts)
    CREATE_PUBVAR(BtmTrPwrALT3,     float);     // Task3 version of lower transistor pwr loss flow into junct PhsA (Watts)
    CREATE_PUBVAR(BtmTrPwrBUT3,     float);     // Task3 version of upper transistor pwr loss flow into junct PhsB (Watts)
    CREATE_PUBVAR(BtmTrPwrBLT3,     float);     // Task3 version of lower transistor pwr loss flow into junct PhsB (Watts)
    CREATE_PUBVAR(BtmTrPwrCUT3,     float);     // Task3 version of upper transistor pwr loss flow into junct PhsC (Watts)
    CREATE_PUBVAR(BtmTrPwrCLT3,     float);     // Task3 version of lower transistor pwr loss flow into junct PhsC (Watts)

    CREATE_PUBVAR(BtmDpwrAUT3,      float);     // Task3 version of upper diode pwr loss flow into junct PhsA (Watts)
    CREATE_PUBVAR(BtmDpwrALT3,      float);     // Task3 version of lower diode pwr loss flow into junct PhsA (Watts)
    CREATE_PUBVAR(BtmDpwrBUT3,      float);     // Task3 version of upper diode pwr loss flow into junct PhsB (Watts)
    CREATE_PUBVAR(BtmDpwrBLT3,      float);     // Task3 version of lower diode pwr loss flow into junct PhsB (Watts)
    CREATE_PUBVAR(BtmDpwrCUT3,      float);     // Task3 version of upper diode pwr loss flow into junct PhsC (Watts)
    CREATE_PUBVAR(BtmDpwrCLT3,      float);     // Task3 version of lower diode pwr loss flow into junct PhsC (Watts)

    CREATE_PUBVAR(BtmRbRcp      ,   float);     // Reciprocal transtr die to baseplate thermal resistance (Watt/degC)
    CREATE_PUBVAR(BtmDRbRcp     ,   float);     // Reciprocal diode die to baseplate thermal resistance (Watt/degC)
    CREATE_PUBVAR(BtmTaGn       ,   float);     // Transistor die (point-A) integral thermal gain (degC/Watts)
    CREATE_PUBVAR(BtmDTaGn      ,   float);     // Diode die (point-A) integral thermal gain (degC/Watts)

    CREATE_PUBVAR(BtmTempAAL    ,   float);     // Lower transistor PhsA die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmTempAAU    ,   float);     // Upper transistor PhsA die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmTempABL    ,   float);     // Lower transistor PhsB die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmTempABU    ,   float);     // Upper transistor PhsB die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmTempACL    ,   float);     // Lower transistor PhsC die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmTempACU    ,   float);     // Upper transistor PhsC die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempAAL   ,   float);     // Lower diode PhsA die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempAAU   ,   float);     // Upper diode PhsA die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempABL   ,   float);     // Lower diode PhsB die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempABU   ,   float);     // Upper diode PhsB die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempACL   ,   float);     // Lower diode PhsC die (point-A) temperature (degC)
    CREATE_PUBVAR(BtmDTempACU   ,   float);     // Upper diode PhsC die (point-A) temperature (degC)

    CREATE_PUBVAR(BtmABPwrA     ,   float);     // Total transistor pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmABPwrAL    ,   float);     // Lower transistor pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmABPwrAU    ,   float);     // Upper transistor pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmABPwrB     ,   float);     // Total transistor pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmABPwrBL    ,   float);     // Lower transistor pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmABPwrBU    ,   float);     // Upper transistor pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmABPwrC     ,   float);     // Total transistor pwr loss put in baseplate PhsC (Watts)
    CREATE_PUBVAR(BtmABPwrCL    ,   float);     // Lower transistor pwr loss put in baseplate PhsC (Watts)
    CREATE_PUBVAR(BtmABPwrCU    ,   float);     // Upper transistor pwr loss put in baseplate PhsC (Watts)
    CREATE_PUBVAR(BtmDABPwrA    ,   float);     // Total diode pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmDABPwrAL   ,   float);     // Lower diode pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmDABPwrAU   ,   float);     // Upper diode pwr loss put in baseplate PhsA (Watts)
    CREATE_PUBVAR(BtmDABPwrB    ,   float);     // Total diode pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmDABPwrBL   ,   float);     // Lower diode pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmDABPwrBU   ,   float);     // Upper diode pwr loss put in baseplate PhsB (Watts)
    CREATE_PUBVAR(BtmDABPwrC    ,   float);     // Total diode pwr loss put in baseplate PhsC (Watts)
    CREATE_PUBVAR(BtmDABPwrCL   ,   float);     // Lower diode pwr loss put in baseplate PhsC (Watts)
    CREATE_PUBVAR(BtmDABPwrCU   ,   float);     // Upper diode pwr loss put in baseplate PhsC (Watts)
    // Unpublished Variables
    //----------------------

    // -------- task1 --------

    // -------- task2 --------
    float               BtmPowerHolder;       // Dummy variable to hold power values (Watts)
    RcNetworkPlistTyp   RcNetworkPlistJncTAU;
    RcNetworkPlistTyp   RcNetworkPlistJncTAL;
    RcNetworkPlistTyp   RcNetworkPlistJncTBU;
    RcNetworkPlistTyp   RcNetworkPlistJncTBL;
    RcNetworkPlistTyp   RcNetworkPlistJncTCU;
    RcNetworkPlistTyp   RcNetworkPlistJncTCL;

    RcNetworkPlistTyp   RcNetworkPlistJncDAU;
    RcNetworkPlistTyp   RcNetworkPlistJncDAL;
    RcNetworkPlistTyp   RcNetworkPlistJncDBU;
    RcNetworkPlistTyp   RcNetworkPlistJncDBL;
    RcNetworkPlistTyp   RcNetworkPlistJncDCU;
    RcNetworkPlistTyp   RcNetworkPlistJncDCL;

    RcNetworkPlistTyp   RcNetworkPlistNd4TAU;
    RcNetworkPlistTyp   RcNetworkPlistNd4TAL;
    RcNetworkPlistTyp   RcNetworkPlistNd4TBU;
    RcNetworkPlistTyp   RcNetworkPlistNd4TBL;
    RcNetworkPlistTyp   RcNetworkPlistNd4TCU;
    RcNetworkPlistTyp   RcNetworkPlistNd4TCL;

    RcNetworkPlistTyp   RcNetworkPlistNd4DAU;
    RcNetworkPlistTyp   RcNetworkPlistNd4DAL;
    RcNetworkPlistTyp   RcNetworkPlistNd4DBU;
    RcNetworkPlistTyp   RcNetworkPlistNd4DBL;
    RcNetworkPlistTyp   RcNetworkPlistNd4DCU;
    RcNetworkPlistTyp   RcNetworkPlistNd4DCL;

    // -------- task3 --------
    RcNetworkPlistTyp   RcNetworkPlistNd1AU;
    RcNetworkPlistTyp   RcNetworkPlistNd1AL;
    RcNetworkPlistTyp   RcNetworkPlistNd1BU;
    RcNetworkPlistTyp   RcNetworkPlistNd1BL;
    RcNetworkPlistTyp   RcNetworkPlistNd1CU;
    RcNetworkPlistTyp   RcNetworkPlistNd1CL;

    // -------- rules --------
    //float               FullTmRcp;

    // Data Passing
    //-------------
    DATA_PASS( BtmTrJncTempAU,  BtmTrJncTempAUT3 , T2_T3, FILTERED   );     // Junction temperatures
    DATA_PASS( BtmTrJncTempAL,  BtmTrJncTempALT3 , T2_T3, FILTERED   );     // passed to task3
    DATA_PASS( BtmTrJncTempBU,  BtmTrJncTempBUT3 , T2_T3, FILTERED   );
    DATA_PASS( BtmTrJncTempBL,  BtmTrJncTempBLT3 , T2_T3, FILTERED   );
    DATA_PASS( BtmTrJncTempCU,  BtmTrJncTempCUT3 , T2_T3, FILTERED   );
    DATA_PASS( BtmTrJncTempCL,  BtmTrJncTempCLT3 , T2_T3, FILTERED   );
    DATA_PASS( BtmDjncTempAU,   BtmDjncTempAUT3  , T2_T3, FILTERED   );
    DATA_PASS( BtmDjncTempAL,   BtmDjncTempALT3  , T2_T3, FILTERED   );
    DATA_PASS( BtmDjncTempBU,   BtmDjncTempBUT3  , T2_T3, FILTERED   );
    DATA_PASS( BtmDjncTempBL,   BtmDjncTempBLT3  , T2_T3, FILTERED   );
    DATA_PASS( BtmDjncTempCU,   BtmDjncTempCUT3  , T2_T3, FILTERED   );
    DATA_PASS( BtmDjncTempCL,   BtmDjncTempCLT3  , T2_T3, FILTERED   );

    DATA_PASS( BtmTrPwrAU,      BtmTrPwrAUT2     , T1_T2, FILTERED   );     // Power loss injected in
    DATA_PASS( BtmTrPwrAL,      BtmTrPwrALT2     , T1_T2, FILTERED   );     // juction nodes passed to
    DATA_PASS( BtmTrPwrBU,      BtmTrPwrBUT2     , T1_T2, FILTERED   );     // task2
    DATA_PASS( BtmTrPwrBL,      BtmTrPwrBLT2     , T1_T2, FILTERED   );
    DATA_PASS( BtmTrPwrCU,      BtmTrPwrCUT2     , T1_T2, FILTERED   );
    DATA_PASS( BtmTrPwrCL,      BtmTrPwrCLT2     , T1_T2, FILTERED   );
    DATA_PASS( BtmDpwrAL,       BtmDpwrALT2      , T1_T2, FILTERED   );
    DATA_PASS( BtmDpwrAU,       BtmDpwrAUT2      , T1_T2, FILTERED   );
    DATA_PASS( BtmDpwrBU,       BtmDpwrBUT2      , T1_T2, FILTERED   );
    DATA_PASS( BtmDpwrBL,       BtmDpwrBLT2      , T1_T2, FILTERED   );
    DATA_PASS( BtmDpwrCU,       BtmDpwrCUT2      , T1_T2, FILTERED   );
    DATA_PASS( BtmDpwrCL,       BtmDpwrCLT2      , T1_T2, FILTERED   );

    DATA_PASS( BtmTrBondPwrAU , BtmTrBondPwrAUT2 , T1_T2, FILTERED   );     // Bond wire power loss
    DATA_PASS( BtmTrBondPwrAL , BtmTrBondPwrALT2 , T1_T2, FILTERED   );     // passed to task2
    DATA_PASS( BtmDbondPwrAU  , BtmDbondPwrAUT2  , T1_T2, FILTERED   );
    DATA_PASS( BtmDbondPwrAL  , BtmDbondPwrALT2  , T1_T2, FILTERED   );
    DATA_PASS( BtmTrBondPwrBU , BtmTrBondPwrBUT2 , T1_T2, FILTERED   );
    DATA_PASS( BtmTrBondPwrBL , BtmTrBondPwrBLT2 , T1_T2, FILTERED   );
    DATA_PASS( BtmDbondPwrBU  , BtmDbondPwrBUT2  , T1_T2, FILTERED   );
    DATA_PASS( BtmDbondPwrBL  , BtmDbondPwrBLT2  , T1_T2, FILTERED   );
    DATA_PASS( BtmTrBondPwrCU , BtmTrBondPwrCUT2 , T1_T2, FILTERED   );
    DATA_PASS( BtmTrBondPwrCL , BtmTrBondPwrCLT2 , T1_T2, FILTERED   );
    DATA_PASS( BtmDbondPwrCU  , BtmDbondPwrCUT2  , T1_T2, FILTERED   );
    DATA_PASS( BtmDbondPwrCL  , BtmDbondPwrCLT2  , T1_T2, FILTERED   );

    DATA_PASS( BtmNd1TempAU   , BtmNd1TempAUT2   , T3_T2, UNFILTERED );     // Temperatures in 1st nodes
    DATA_PASS( BtmNd1TempAL   , BtmNd1TempALT2   , T3_T2, UNFILTERED );     // passed to task2
    DATA_PASS( BtmNd1TempBU   , BtmNd1TempBUT2   , T3_T2, UNFILTERED );
    DATA_PASS( BtmNd1TempBL   , BtmNd1TempBLT2   , T3_T2, UNFILTERED );
    DATA_PASS( BtmNd1TempCU   , BtmNd1TempCUT2   , T3_T2, UNFILTERED );
    DATA_PASS( BtmNd1TempCL   , BtmNd1TempCLT2   , T3_T2, UNFILTERED );

    DATA_PASS( BtmPwrNd2Nd1AU , BtmPwrNd2Nd1AUT3 , T2_T3, FILTERED   );     // Total power flowing onto
    DATA_PASS( BtmPwrNd2Nd1AL , BtmPwrNd2Nd1ALT3 , T2_T3, FILTERED   );     // heatsink passed to task3
    DATA_PASS( BtmPwrNd2Nd1BU , BtmPwrNd2Nd1BUT3 , T2_T3, FILTERED   );
    DATA_PASS( BtmPwrNd2Nd1BL , BtmPwrNd2Nd1BLT3 , T2_T3, FILTERED   );
    DATA_PASS( BtmPwrNd2Nd1CU , BtmPwrNd2Nd1CUT3 , T2_T3, FILTERED   );
    DATA_PASS( BtmPwrNd2Nd1CL , BtmPwrNd2Nd1CLT3 , T2_T3, FILTERED   );

    DATA_PASS( BtmRefTempValid, BtmRefTempValidT2, T3_T2, UNFILTERED );     // passed to tasks 1 and 2

    DATA_PASS( BtmTrSwGn      , BtmTrSwGnT1      , T3_T1, UNFILTERED );     // Approx rate of change of voltage
    DATA_PASS( BtmDswGn       , BtmDswGnT1       , T3_T1, UNFILTERED );     // across transistor and diode passing to task1
    
    // For 2P
    DATA_PASS( BtmTrPwrAU,      BtmTrPwrAUT3     , T1_T3, FILTERED   );     // Power loss injected in
    DATA_PASS( BtmTrPwrAL,      BtmTrPwrALT3     , T1_T3, FILTERED   );     // juction nodes passed to
    DATA_PASS( BtmTrPwrBU,      BtmTrPwrBUT3     , T1_T3, FILTERED   );     // task2
    DATA_PASS( BtmTrPwrBL,      BtmTrPwrBLT3     , T1_T3, FILTERED   );
    DATA_PASS( BtmTrPwrCU,      BtmTrPwrCUT3     , T1_T3, FILTERED   );
    DATA_PASS( BtmTrPwrCL,      BtmTrPwrCLT3     , T1_T3, FILTERED   );
    DATA_PASS( BtmDpwrAL,       BtmDpwrALT3      , T1_T3, FILTERED   );
    DATA_PASS( BtmDpwrAU,       BtmDpwrAUT3      , T1_T3, FILTERED   );
    DATA_PASS( BtmDpwrBU,       BtmDpwrBUT3      , T1_T3, FILTERED   );
    DATA_PASS( BtmDpwrBL,       BtmDpwrBLT3      , T1_T3, FILTERED   );
    DATA_PASS( BtmDpwrCU,       BtmDpwrCUT3      , T1_T3, FILTERED   );
    DATA_PASS( BtmDpwrCL,       BtmDpwrCLT3      , T1_T3, FILTERED   );
};

#endif  // B0_BRG_THRM5P_H
