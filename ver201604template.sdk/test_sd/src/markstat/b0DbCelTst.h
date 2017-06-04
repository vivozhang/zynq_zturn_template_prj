///////////////////////////////////////////////////////////////////////////////
//
// TITLE        DB Cell Test Class
//
// DESCRIPTION
//      This file contains the definition for the b0DbCelTst class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0DbCelTst.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef B0_DB_CETST_H
#define B0_DB_CETST_H


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"


// Constants
//----------


// Enumerations
//-------------


// Function Prototypes
//--------------------


// Types and Classes
//------------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This class provides the DB cell test.
//
///////////////////////////////////////////////////////////////////////////////
class cDbCelTst
{

//-----------------------------------------------------------------------------
public:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;


    // Types & Enumerations
    //---------------------
    enum DbCtSelTyp                 // DB cell test selection
    {
        CELL_TST_DB1    =  0,
        CELL_TST_DB2    =  1,
    };

    struct InListTyp
    {
        const unsigned&     TripFlt;                //  1 - Trip fault active
        const unsigned&     CelTstReq;              //  2 - DB cell test request (from sequencer)
        const float&        IaFbk;                  //  3 - Line side PhA task 2 current feedback (Amps)
        const float&        IbFbk;                  //  4 - Line side PhB task 2 current feedback (Amps)
        const float&        IcFbk;                  //  5 - Line side PhC task 2 current feedback (Amps)
        const float&        BusPwrRcp;              //  6 - Reciprocal of average active power during cell test (1/Watts)
        const unsigned&     Vdb1VdcMisFlg;          //  7 - Flag indicates that the actual DB1 voltage mismatch the expected one
        const unsigned&     Vdb2VdcMisFlg;          //  8 - Flag indicates that the actual DB2 voltage mismatch the expected one
        const unsigned&     Db1MisOffTstFlg;        //  9 - DB1 mistakenly off flag in DB test mode
        const unsigned&     Db1MisOnTstFlg;         // 10 - DB1 mistakenly on flag in DB test mode
        const unsigned&     Db2MisOffTstFlg;        // 11 - DB2 mistakenly off flag in DB test mode
        const unsigned&     Db2MisOnTstFlg;         // 12 - DB2 mistakenly on flag in DB test mode
        const unsigned&     L_RunRdy;               // 13 - Line converter run ready
        const unsigned&     DbPsTstStat;            // 14 - DB pulse test status
        const unsigned&     DbPsTstAct;             // 15 - DB pulse test active
        const unsigned&     IMeasStat;              // 16 - Line Rms Phase Current measure status
        const float&        DbPwrInt;               // 17 - DB resistor rate of energy (Watts)
        const float&        VdcFbkT2;               // 18 - DC link voltage feedback task2 version
        const float&        VMagT2;                 // 19 - Line voltage magnitude
        const float&        GmVMagPuT2;             // 20 - Line Gm voltage magnitude per unit task 2
        const float&        VdbTst;                 // 21 - Average DB voltage feedback during cell test (Volts)
        const int&          DbCtSel;                // 22 - Bridge selector for cell test
        const unsigned&     ResMsReq;               // 23 - Resistance measurement request
        const unsigned&     CapMsReq;               // 24 - Capacitance measurement request
        const unsigned&     Q1Stat;                 // 25 - Q1 status feedback
    };

    struct InsListTyp
    {
        const float&        DelTm2;                 //  1 - Task2 delta time (Sec)
        const float&        DbR;                    //  2 - DB resistor value (Ohms)
        const float&        DbResIMin;              //  3 - Line contactor MA close minimum current (Amps)
        const float&        MAClsVdcMn;             //  4 - Line contactor MA close minimum allowed vdc (Volts)
        const float&        DbOtThrs;               //  5 - Db Over temperature Threshold due to DB overheating (Watts)
        const float&        DcCap;                  //  6 - DC link capacitance (Farad)
        const unsigned&     DulDbEnb;               //  7 - Dual DB bridge enabled
    };


    // Methods
    //--------
    cDbCelTst( InListTyp InList, InsListTyp InsList );
    ~cDbCelTst();

    int   RunRules(void);
    void  Task1(void);
    void  Task2(void);

    int  OpenMsgQue(void);
    int   CloseMsgQue(void);
    float WhatIsDbResTol(unsigned Level) const;


    // Output Signals
    //---------------
    CREATE_PUBVAR(DbCtStat,     unsigned);  // cell test task Stat
    CREATE_PUBVAR(DbCtAct,      unsigned);  // cell test task Active
    CREATE_PUBVAR(DbCtIp,       unsigned);  // Dynamic brake cell test in progress flag
    CREATE_PUBVAR(DbCtPsTstReq, unsigned);  // DB cell test set the DB pulse test request
    CREATE_PUBVAR(DbCtStateCmd1,unsigned);  // DB1 gate command
    CREATE_PUBVAR(DbCtStateCmd2,unsigned);  // DB2 gate command
    CREATE_PUBVAR(DbCtRmeas,    float);     // the measured DB resistance (Ohm)
    CREATE_PUBVAR(DbCtCmeas,    float);     // the measured DB capacitance (Farad)
    CREATE_PUBVAR(DbCtQ1OpnReq, unsigned);  // the measured DB resistance (Ohm)
    CREATE_PUBVAR(DbCtKdfOpnReq,unsigned);  // the measured DB resistance (Ohm)

    CREATE_PUBVAR(DbCtImeasReq, unsigned);  // Request to external measurement for DB test

    // DynamicBrakeResMeas
    CREATE_PUBVAR(DbResMsSt,    int);       // DB resistance test task state
    CREATE_PUBVAR(DbResMsAct,   unsigned);  // DB resistance test task active
    CREATE_PUBVAR(DbResMsSta,   unsigned);  // DB resistance test task status
    CREATE_PUBVAR(DbResMsIp,    unsigned);  // DB resistance test in progress
    CREATE_PUBVAR(DbResMsDon,   unsigned);  // DB resistance test done
    CREATE_PUBVAR(DbResMsIpT1,  unsigned);  // DB resistance test in progress task 1 version

    // DynamicBrakeCapMeas
    CREATE_PUBVAR(DbCapMsSt,    int);       // DB capacitance test task state
    CREATE_PUBVAR(DbCapMsAct,   unsigned);  // DB capacitance test task active
    CREATE_PUBVAR(DbCapMsSta,   unsigned);  // DB capacitance test task status
    CREATE_PUBVAR(DbCapMsIp,    unsigned);  // DB capacitance test in progress
    CREATE_PUBVAR(DbCapMsDon,   unsigned);  // DB capacitance test done
    CREATE_PUBVAR(DbCapMsIpT1,  unsigned);  // DB capacitance test in progress task 1 version
    
    CREATE_PUBVAR(DbCtMsgStkSt, unsigned);  // DB cell test message stack status
//-----------------------------------------------------------------------------
private:

    // External Inputs
    //----------------
    InListTyp    In;
    InsListTyp   Ins;


    // Methods
    //--------
    //void DbResMsRmsMeas(void);        // Measure Power and Vdb during resistance test in Task 1
    void DynamicBrakeCellTest(void);    // DB cell test
    void DynamicBrakeResMeas(void);     // DB resistance test
    void DynamicBrakeCapMeas(void);     // DB capacitance test
    void DbCelTstInit(void);            // Init for DB cell test, resistance test, capacitance test

    // Diagnostics
    //------------
    CREATE_DIAG(DbResHigh);                 // The DB resistance measured too high trip
    CREATE_DIAG(DbResLow);                  // The DB resistance measured too low trip
    CREATE_DIAG(DbCelTstMsgStkTrp);         // DB ADL message stack failed
    CREATE_DIAG(DbCapMsTmOut);              // Time out in capacitance measurement

    // Parameters
    //-----------
    CREATE_PARM(DbCtPwrLm,      float);     // Power limit above which the DB Cell Test will not
                                            // execute in order to allow DB Res to cool off
    CREATE_PARM(DbCtTm,         float);     // Inductance test general time (Sec)
    CREATE_PARM(DbCtMaClsVdcPu, float);     // Line contactor MA close vdc per as a per unit of line magnitude
    CREATE_PARM(DbCtChgLvlAc,   float);     // DC link charge level 2 based on ac line
    CREATE_PARM(DbResMsStlTm,   float);     // DB resistance test settle time before measurement
    CREATE_PARM(DbCtVdbZeroLvl, float);     // Level to determine a zero DB resistor voltage
    CREATE_PARM(DbCtDlyTm2,     float);     // DB cell test delay time 2 (Sec)
    CREATE_PARM(DbCtDlyTm3,     float);     // DB cell test delay time 3 (Sec)
    CREATE_PARM(DbCtDlyTm4,     float);     // DB cell test delay time 4 (Sec)
    CREATE_PARM(DbCtDlyTm5,     float);     // DB cell test delay time 5 (Sec)
    CREATE_PARM(DbCtDlyTm6,     float);     // DB cell test delay time 6 (Sec)
    CREATE_PARM(DbCtDlyTm7,     float);     // DB cell test delay time 7 (Sec)
    CREATE_PARM(DbCtDlyTm8,     float);     // DB cell test delay time 8 (Sec)
    CREATE_PARM(DbCtDlyTm9,     float);     // DB cell test delay time 9 (Sec)
    CREATE_PARM(DbCtModPktSize, float);     // DB resistance test package size

    CREATE_PARM(DbResMsTolP,       float);  // DB resistance test tolerance (plus side)
    CREATE_PARM(DbResMsTolM,       float);  // DB resistance test tolerance (minus side)
    CREATE_PARM(DbResMsDtyReq,  float);     // DB resistance test gate duty command request

    CREATE_PARM(DbCapMsVdcFilW, float);     // DB capacitance test Vdc filter W
    CREATE_PARM(DbCapMsDlyTm1,  float);     // DB capacitance test Delay time 1
    CREATE_PARM(DbCapMsDlyTm2,  float);     // DB capacitance test Delay time 2
    CREATE_PARM(DbCapMsDlyTm3,  float);     // DB capacitance test Delay time 3
    CREATE_PARM(DbCapMsDlyTm4,  float);     // DB capacitance test Delay time 4
    CREATE_PARM(DbCapMsDlyTm5,  float);     // DB capacitance test Delay time 5
    CREATE_PARM(DbCapMsDlyTm6,  float);     // DB capacitance test Delay time 6
    CREATE_PARM(DbCapMsVdcRef,  float);     // DB capacitance test Vdc drop reference
    CREATE_PARM(DbCapMsTolP,    float);     // DB capacitance test tolerance (puls side)
    CREATE_PARM(DbCapMsTolM,    float);     // DB capacitance test tolerance (minus side)
    CREATE_PARM(DbCapMsDtyReq,  float);     // DB capacitance test gate duty command request

    // Published Variables
    //--------------------

    // DB Cell Test variables
    CREATE_PUBVAR(DbCtIpT1,         unsigned);  // DB cell test in process flag task 1 version
    CREATE_PUBVAR(DbCtReq,          unsigned);  // DB cell test request
    CREATE_PUBVAR(DbCtActT1,        unsigned);  // DB cell test active flag task1 version
    CREATE_PUBVAR(DbCtSt,           int);       // DB cell test state
    CREATE_PUBVAR(DbCtDtyReq,       float);     // DB cell test gate duty command request
    CREATE_PUBVAR(DbCtDtyReqT1,     float);     // DB cell test gate duty command request task1 version
    CREATE_PUBVAR(DbCtTypeSt,       int);       // DB cell test type state
    CREATE_PUBVAR(DbCtCapMsReq,     unsigned);  // DB internal capacitance measurement request from cell test
    CREATE_PUBVAR(DbCtResMsReq,     unsigned);  // DB internal resistance measurement request from cell test
    CREATE_PUBVAR(DbCtTmr,          float);     // Inductance test general timer (Sec)
    CREATE_PUBVAR(DbCtRmeasRcp,     float);     // reciprocal of the measured DB resistance (1/Ohm)
    CREATE_PUBVAR(DbCtBrgEnb,       unsigned);
    CREATE_PUBVAR(DbCtDutyCmd,      float);
    CREATE_PUBVAR(DbCtModOutInt,    float);
    CREATE_PUBVAR(DbCtModPktInt,    float);
    CREATE_PUBVAR(DbCtModOut,       unsigned);

    CREATE_PUBVAR(DbResMsReq,       unsigned);  // DB resistance test request
    CREATE_PUBVAR(DbResMsDtyReq,    unsigned);  // DB resistance test gate duty command request
    CREATE_PUBVAR(DbResMsTmr,       float);     // DB resistance test timer (Sec)
    CREATE_PUBVAR(DbResMsLwFlg,     unsigned);  // DB resistance test ressitance low flag
    CREATE_PUBVAR(DbResMsHiFlg,     unsigned);  // DB resistance test ressitance high flag
    CREATE_PUBVAR(DbResMsVfbkErr,   unsigned);  // Missing Db resistor voltage feedback
    CREATE_PUBVAR(DbResMsDtyRcp,    float);

    CREATE_PUBVAR(DbCapMsReq,       unsigned);  // DB capacitance test request
    CREATE_PUBVAR(DbCapMsQ1OpnReq,  unsigned);  // DB capacitance test Q1 open request
    CREATE_PUBVAR(DbCapMsKdfOpnReq, unsigned);  // DB capacitance test Kdf open request
    CREATE_PUBVAR(DbCapMsDtyReq,    float);     // DB capacitance test gate duty command request
    CREATE_PUBVAR(DbCapMsTmr,       float);     // DB capacitance test timer (Sec)
    CREATE_PUBVAR(DbCapMsVdcFil,    float);     // DB capacitance test filtered Vdc (V)
    CREATE_PUBVAR(DbCapMsCmeasMax,  float);     // DB capacitance test tolerance (plus side)
    CREATE_PUBVAR(DbCapMsCmeasMin,  float);     // DB capacitance test tolerance (minus side)
    CREATE_PUBVAR(DbCapMsVdcFilGn,  float);     // DB capacitance test Vdc filter Gn
    CREATE_PUBVAR(DbCapMsVdcTrgt,   float);     // DB capacitance test Vdc drop target
    CREATE_PUBVAR(DbCapMsRcTau,     float);     // DB capacitance test RC time constant
    CREATE_PUBVAR(DbCtCmeasUf,      float);     // Measured DC link capacitance (uF)
    CREATE_PUBVAR(DbCapMsDtyRcp,    float);

    CREATE_PUBVAR(DbRmsMsReq,       unsigned);  // DB measurement task request
    CREATE_PUBVAR(DbRmsMsReqT1,     unsigned);  // DB measurement task request in task 1 version
    CREATE_PUBVAR(DbRmsMsStaT1,     unsigned);  // DB measurement task status in task 1 version
    CREATE_PUBVAR(DbRmsMsSta,       unsigned);  // DB measurement task status
    CREATE_PUBVAR(DbRmsMsPwrAcc,    float);     // DB measurement power integrator
    CREATE_PUBVAR(DbRmsMsVdbAcc,    float);     // DB measurement Vdc integrator
    // Unpublished Variables
    //----------------------


    // Data Passing
    //-------------
    DATA_PASS(DbCtDtyReq,        DbCtDtyReqT1,      T2_T1,  UNFILTERED);
    DATA_PASS(DbCtAct,          DbCtActT1,        T2_T1,  UNFILTERED);
    DATA_PASS(DbCtIp,           DbCtIpT1,        T2_T1,  UNFILTERED);
    DATA_PASS(DbResMsIp,        DbResMsIpT1,        T2_T1,  UNFILTERED);
    DATA_PASS(DbCapMsIp,        DbCapMsIpT1,        T2_T1,  UNFILTERED);

    // For DB RMS measure
    DATA_PASS(DbRmsMsReq,   DbRmsMsReqT1,   T2_T1,  UNFILTERED);
    DATA_PASS(DbRmsMsStaT1, DbRmsMsSta,     T1_T2,  UNFILTERED);
};



// Inline Function Definitions
//----------------------------

//-----------------------------------------------------------------------------
// Retrieve dynamic-brake resistance plus and minus tolerance levels
inline float    cDbCelTst::WhatIsDbResTol(unsigned Level) const
{
    if (Level == 1)     // + level
    {
        return  PARM(DbResMsTolP);
    }
    else                // - level
    {
        return  PARM(DbResMsTolM);
    }
}


#endif  // B0_DB_CETST_H
