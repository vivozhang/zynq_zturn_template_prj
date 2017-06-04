///////////////////////////////////////////////////////////////////////////////
//
// TITLE:    Dynamic brake thermal model (Dtm) class for single
//           semiconductor module.
//
// DESCRIPTION:
//      This file contains the definition for the cDbThrmModel class.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0DbThrm.vsd and b0DbThrm.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef    B0_DBTHRM_H
#define    B0_DBTHRM_H

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
// DESCRIPTION
//     Thermal model implementation for a dynamic brake consisting of semi-
//     conductor/heatsink assembly connected in series with a power resistors.
//     Dynamic brake module packages power transistors, power diodes, and
//     temperature sensors; only transistor model is developed since thermal
//     stress on diodes is not expected to be significant. One or two semi-
//     conductor modules can be attached to heatsink, in which case parameters
//     would properly reflect the configuration.
//
///////////////////////////////////////////////////////////////////////////////
class cDbThrmModel
{

//-----------------------------------------------------------------------------
public:

    // Enumerations
    //-------------
    enum RefTempeTyp
    {
        DB_REF_TEMP_1      = 0,
        DB_REF_TEMP_2      = 1
    };

    // Types and Classes
    //------------------
    struct InListTyp
    {
        const float&             Vdb;               //  1 - Voltage across DB resistor (Volts) - Task1
        const float&             Vdc;               //  2 - DC bus voltage across DB switch and resistor (Volts) - Task1
        const float&             RefTemp1;          //  3 - Reference #1 temperature (degC) - Task3
        const float&             RefTemp2;          //  4 - Reference #2 temperature (degC) - Task3
        const unsigned&          ConvDiscnnted;     //  5 - Converter disconnected indication - Task3
        const unsigned&          Stopped;           //  6 - Stopped state indication - Task3
        const unsigned&          DiagMode;          //  7 - Diagnostic mode indication - Task3
        const int&               RefTempAct;        //  8 - Reference temperature active in model - Task3
        const unsigned&          Q1StatusVsqr;      //  9 - Q1 status determined by squared voltage difference between terminals
    };

    struct InsListTyp
    {
        const float&        DelTm1;                 //  1 - Task 1 time (Secs)
        const float&        DelTm1Rcp;              //  2 - Reciprocal of task 1 time (Secs)
        const float&        DelTm2;                 //  3 - Task 2 time (Secs)
        const float&        DelTm3;                 //  4 - Task 3 time (Secs)
        const float&        DbR;                    //  5 - Resistance for one of n DB circuits (Ohms)
        const float&        DbTauElec;              //  6 - Electrical time constant of DB resistor (Secs)
        const float&        DbTauFbkCkt;            //  7 - Time constant of DB voltage feedback circuit (Secs)
        const float&        VllIn;                  //  8 - Line voltage (volts line-line rms)
        const float&        VdbAdjFact;             //  9 - Adjustment factor for calculating Db On threshold
        const float&        DbContPwr;              // 10 - Continuous power rating of DB resistor (Watts)
        const float&        DbImpPwr;               // 11 - Impulse power rating of DB resistor (Watts)
        const float&        DbImpTm;                // 12 - Impulse time rating of DB resistor (Secs)
        const float&        PwrDisconTm;            // 13 - Line Line bridge power disconnection time (Sec)
        const int&          ProductType;            // 14 - Configure parms by product type
        const float&        DbOvldTrpThrs;          // 15 - DB Over temperature Threshold due to DB overloading (Watts)
        const float&        BrgDbRthrm;             // 16 - DB resistor thermal resistance (deg C/ Watts)
        const unsigned&     DbEnb;                  // 17 - DB thermal model enable indication
        const float&        LiquidOtThrs;           // 18 - Liquid coolant over temperature threshold (degC)
    };
#include "b0DbThrmSim.inc"
    // Methods
    //--------
    cDbThrmModel( InListTyp InList, InsListTyp InsList );
    ~cDbThrmModel( void );
    void Task1( void );
    void Task2( void );
    void Task3( void );
    int  RunRules( void );
    void SimReadTemp(DtmTempSimTyp & target);
    
    //
    // Output Signals
    //---------------
    CREATE_PUBVAR(DtmTmpRise,     float);        // DB resistor temperature rise (degC)
    CREATE_PUBVAR(DtmPavg,        float);        // Avg DB thermal energy change over one task1 or average power (Watts)
    CREATE_PUBVAR(DtmPavgT3,      float);        // Average DB power data passed to task3 (Watts)
    CREATE_PUBVAR(DtmPdisspt,     float);        // DB resistor rate of energy dissipation (Watts)
    CREATE_PUBVAR(DtmTrPwrLoss,    float);       // Transistor total power loss in task1 (Watts)
    CREATE_PUBVAR(DtmTrJncTempT3, float);        // Transistor junction temperature passed to task3 (degC)
    CREATE_PUBVAR(DtmDsbOverload, unsigned);     // DB resistor overload disable
    CREATE_PUBVAR(DtmSwSt,        unsigned);     // state of DB switch (on=true, off=false)
    CREATE_PUBVAR(DtmOtLock,      unsigned);     // DB transistor to lock-on due to junction over temperature
    CREATE_PUBVAR(DtmTrTmpRdy,    unsigned);     // DB transistor thermaly settled
    CREATE_PUBVAR(DtmOverload,    unsigned);     // DB resistor overload alarm
    CREATE_PUBVAR(DtmTrpOverload, unsigned);     // DB resistor overload trip
    CREATE_PUBVAR(DtmCoolingReq,  unsigned);     // DB transistor request for cooling
    CREATE_PUBVAR(DtmTr_GridDiscReq,    unsigned);     // Grid disconnect request
    CREATE_PUBVAR(DtmJncHot,      unsigned);     // DB transistor is hot flag
    CREATE_PUBVAR(DtmRes_GridDiscReq,    unsigned);   // Grid disconnect request for Task 3 function


//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cMarkStatDiag;
    friend class cDataPasser;

    // External Inputs
    //----------------
    InListTyp       In;
    InsListTyp      Ins;

    // Methods
    //--------
    void ThrmModelInit( void );

    // Diagnostics
    //------------
    CREATE_DIAG(DtmOvldAlm);         // DB resistor overload alarm
    CREATE_DIAG(DtmOvldTrp);         // DB resistor overload trip
    CREATE_DIAG(DtmOvldDiscAlm);     // DB resistor overload disconnect alarm
    CREATE_DIAG(DtmOvldDsbTrp);      // DB resistor overload disable
    CREATE_DIAG(DtmJncOtTrp);        // DB transistor junction over-temp trip
    CREATE_DIAG(DtmJncOtDiscAlm);    // DB transistor junction over-temp disconnect alarm
    CREATE_DIAG(DtmJncOtLockTrp);    // DB junction over-temp require transistor to lock-on

    // Parameters
    //-----------
    CREATE_PARM(DtmTrVo,           float);       // Transistor constant voltage during conduction (Volts)
    CREATE_PARM(DtmTrR,            float);       // Transistor resistance during conduction (Ohms)
    CREATE_PARM(DtmTrSw0,          float);       // Transistor constant charge term for switching losses (Coulombs)
    CREATE_PARM(DtmTrSw1,          float);       // Transistor linear charge term for switching losses (Coulombs/Amp)
    CREATE_PARM(DtmTrSw2,          float);       // Transistor quadratic charge term for switching losses (Coulombs/Amp^2)
    CREATE_PARM(DtmTrCJnt,         float);       // Transistor junction thermal capacitance (Jouls/degC)
    CREATE_PARM(DtmTrCnd3,         float);       // Transistor 3rd node thermal capacitance (Jouls/degC)
    CREATE_PARM(DtmTrCnd2,         float);       // Transistor 2nd node thermal capacitance (Jouls/degC)
    CREATE_PARM(DtmTrCnd1,         float);       // Transistor 1st node thermal capacitance (Jouls/degC)
    CREATE_PARM(DtmTrCnd2Ref2,     float);       // Transistor 2nd node thermal capacitance for ref-temp #2 (Jouls/degC)
    CREATE_PARM(DtmTrCnd1Ref2,     float);       // Transistor 1st node thermal capacitance for ref-temp #2 (Jouls/degC)
    CREATE_PARM(DtmTrRthJntNd3,    float);       // Transistor junction to 3rd node thermal resistance (degC/Watts)
    CREATE_PARM(DtmTrRthNd3Nd2,    float);       // Transistor 3rd to 2nd node thermal resistance (degC/Watts)
    CREATE_PARM(DtmTrRthNd2Nd1,    float);       // Transistor 2nd to 1st node thermal resistance (degC/Watts)
    CREATE_PARM(DtmTrRthNd1Clnt,   float);       // Transistor 1st node to coolant thermal resistance (degC/Watts)
    CREATE_PARM(DtmTrRth21Ref2,    float);       // Transistor 2nd to 1st node thermal resistance for ref-temp #2 (degC/Watts)
    CREATE_PARM(DtmTrRth1CRef2,    float);       // Transistor 1st node to coolant thermal resistance for ref-temp #2 (degC/Watts)
    CREATE_PARM(DtmJncOtThr,       float);       // Transistor junction over-temperature trip threshold (degC)
    CREATE_PARM(DtmJncOtLockThr,   float);       // Tunction temperature threshold for locking transistor (degC)
    CREATE_PARM(DtmOtVdcUnlckThr,  float);       // Transistor over-temp Dc voltage un-lock threshold (Volts)
    CREATE_PARM(DtmTrLqdOtThr,     float);       // Transitor thermal readiness based on coolant liquid temp threshold (degC)
    CREATE_PARM(DtmCoolOnThr,      float);       // Transistor junct to ref-temp max delta for requesting cooling (degC)
    CREATE_PARM(DtmJncHotOnThr,    float);       // Transitor junction hot flag on threshold (degC)
    CREATE_PARM(DtmJncHotOffThr,   float);       // Transitor junction hot flag off threshold (degC)

    CREATE_PARM(DtmEnb,         unsigned);       // Enable db thermal model
    CREATE_PARM(DtmOnThrs,         float);       // Dc voltage threshold for DB switch conducting (Volts)
    CREATE_PARM(DtmOvldOnThrs,     float);       // Rate of energy retention threshold to set overload alarm (Watts)
    CREATE_PARM(DtmOvldOffThrs,    float);       // Rate of energy retention threshold to clear overload alarm (Watts)
    CREATE_PARM(DtmOvldTrpThrs,    float);       // Rate of energy retention threshold to trip on DB oveload (Watts)
    CREATE_PARM(DtmOvldDsbThrs,    float);       // Rate of energy retention threshold to disable use of DB circuit (Watts)
    CREATE_PARM(DtmW,              float);       // DB thermal omega as reciprocal of DB time constant (Rad/Sec)
    CREATE_PARM(DtmVdbAdj,         float);       // Average value of [1-e(t/tau)] over 1 DelTm1 (dimensionless units)
    CREATE_PARM(DtmVdcThrs,        float);       // Maximum dc bus voltage for DB switch state determination (Volts)
    CREATE_PARM(DtmVdcOvLvl,       float);       // Dc bus overvoltage level for overload determination (Volts)

    // Published Variables
    //--------------------
    CREATE_PUBVAR(DtmSwLoss,       float);       // Transistor switching losses (Watts)
    CREATE_PUBVAR(DtmTrSw0Gn,      float);       // Transistor current-independent gain for switching losses (Amps)
    CREATE_PUBVAR(DtmTrSw1Gn,      float);       // Transistor current linear-dependency gain for switching losses (dimensionless units)
    CREATE_PUBVAR(DtmTrSw2Gn,      float);       // Transistor current quadratic-dependency gain for switching losses (1/Amp)
    CREATE_PUBVAR(DtmTrCondPwr,    float);       // Transistor power loss due to conduction (Watts)
    CREATE_PUBVAR(DtmTrPwrJnNd3,   float);       // Transistor junction to node-3 power flow (Watts)
    CREATE_PUBVAR(DtmTrPwrJnNd3T2, float);       // Transistor junction to node-3 power passed to task2 (Watts)
    CREATE_PUBVAR(DtmTrCjnGn,      float);       // Transistor junction integral thermal gain (degC/Watts)
    CREATE_PUBVAR(DtmTrJncTemp,    float);       // Transistor junction temperature (degC)
    CREATE_PUBVAR(DtmTrNd3Temp,    float);       // Transistor node-3 temperature (degC)
    CREATE_PUBVAR(DtmTrNd3TempT1,  float);       // Transistor node-3 temperature passed to task1 (degC)
    CREATE_PUBVAR(DtmTrCnd3Gn,     float);       // Transistor node-3 integral thermal gain (degC/Watts)
    CREATE_PUBVAR(DtmTrPwrNd3Nd2,  float);       // Transistor node-3 to node-2 power flow (Watts)
    CREATE_PUBVAR(DtmTrPwrNd3Nd2T3,float);       // Transistor node-3 to node-2 power passed to task3 (Watts)
    CREATE_PUBVAR(DtmTrPwrNd2Nd1,  float);       // Transistor node-2 to node-1 power flow (Watts)
    CREATE_PUBVAR(DtmTrPwrNd1Clnt, float);       // Transistor node-1 to coolant power flow (Watts)
    CREATE_PUBVAR(DtmRthJnNd3Rcp,  float);       // Reciprocal transtr junction to node-3 thermal resistance (Watt/degC)
    CREATE_PUBVAR(DtmRthNd3Nd2Rcp, float);       // Reciprocal transtr node-3 to node-2 thermal resistance (Watt/degC)
    CREATE_PUBVAR(DtmTrNd2Temp,    float);       // Transistor node-2 temperature (degC)
    CREATE_PUBVAR(DtmTrNd2TempT2,  float);       // Transistor node-2 temperature passed to task2 (degC)
    CREATE_PUBVAR(DtmTrNd1Temp,    float);       // Transistor node-1 temperature (degC)
    CREATE_PUBVAR(DtmRefTemp,      float);       // Transistor thermal model reference temp (degC)

    CREATE_PUBVAR(DtmIo,           float);       // Previous value of estimated DB current (Amps)
    CREATE_PUBVAR(DtmIdel,         float);       // Change in induced current from previous estimate (Amps)
    CREATE_PUBVAR(DtmIest,         float);       // Estimated DB current based on DB electrical time constant (Amps)
    CREATE_PUBVAR(DtmEdtTau,       float);       // Exponential per unit change in current for 1 task1 dt (dimensionless units)
    CREATE_PUBVAR(DtmE2DtTau,      float);       // Exponential squared per unit change in current for 1 task1 dt (dimensionless units)
    CREATE_PUBVAR(DtmIsqrInt,      float);       // Integral of estimated current squared for 1 task 1 dt (Amp^2.Sec)
    CREATE_PUBVAR(DtmIint,         float);       // Avg DB current from integral of est current for 1 task1 dt (Amp.Sec)
    CREATE_PUBVAR(DtmAvgIsqr,      float);       // Avg DB current^2 from integral^2 of est current for 1 task1 dt (Amp^2)
    CREATE_PUBVAR(DtmIavg,         float);       // Average DB current over one task 1 time interval (Amps)
    CREATE_PUBVAR(DtmTrpTau,       float);       // DB thermal time constant (Sec)
    CREATE_PUBVAR(DtmOvldTrpTm,    float);       // DB trip time for worst case fault (Sec)
    CREATE_PUBVAR(DtmDsbTm,        float);       // DB disable time for worst case fault (Sec)
    CREATE_PUBVAR(DtmIr,           float);       // DB voltage divided by DB resistance (Amps)
    CREATE_PUBVAR(DtmLinkVdcT3,    float);       // Dc-link voltage data passed to task3 (Volts)
    CREATE_PUBVAR(DtmVdbComp,      float);       // Compensated DB volt feedback for DB switch state determination (Volts)
    CREATE_PUBVAR(DtmSwStZ1,       unsigned);    // Previous state of DtmSwSt
    CREATE_PUBVAR(DtmOtLockT3,     unsigned);    // DB transistor to lock-on data passed to task3
    CREATE_PUBVAR(DtmClearOtLock,  unsigned);    // DB lock-on condition to be cleared in task3
    CREATE_PUBVAR(DtmClearOtLockT1,unsigned);    // DB lock-on clearing data passed to task1


    // Unpublished Variables
    //----------------------
    float       DtmVr;                           // Estimated DB voltage using DC link voltage (Volts)
    float       DtmRrcp;                         // Reciprocal of DB resistance (1/Ohms)
    float       DtmLinkVdc;                      // Copy of dynamic input for dc-link voltage (Volts)
    float       DtmVdbZ1;                        // One task delayed DB voltage feedback (Volts)
    float       DtmTrJncRefDelta;                // Delta between junction and reference temperatures (degC)
    float       DtmTrRefModel[2][4];             // Multi-temp reference table of gains and reciprocal resistances
    double      DtmTrNd2TempDbl;                 // Temperature for 2nd node in double precision slow filter (degC)
    double      DtmTrNd1TempDbl;                 // Temperature for 1st node in double precision slow filter (degC)


    // Data Passing
    //-------------
    DATA_PASS( DtmPavg,        DtmPavgT3,        T1_T3, FILTERED   );
    DATA_PASS( DtmTrJncTemp,   DtmTrJncTempT3,   T1_T3, FILTERED   );
    DATA_PASS( DtmTrNd3Temp,   DtmTrNd3TempT1,   T2_T1, UNFILTERED );
    DATA_PASS( DtmTrNd2Temp,   DtmTrNd2TempT2,   T3_T2, UNFILTERED );
    DATA_PASS( DtmTrPwrJnNd3,  DtmTrPwrJnNd3T2,  T1_T2, FILTERED   );
    DATA_PASS( DtmTrPwrNd3Nd2, DtmTrPwrNd3Nd2T3, T2_T3, FILTERED   );
    DATA_PASS( DtmOtLock,      DtmOtLockT3,      T1_T3, UNFILTERED );
    DATA_PASS( DtmLinkVdc,     DtmLinkVdcT3,     T1_T3, FILTERED   );
    DATA_PASS( DtmClearOtLock, DtmClearOtLockT1, T3_T1, UNFILTERED );

};



// Inline Function Definitions
//----------------------------


#endif  // B0_DBTHRM_H
