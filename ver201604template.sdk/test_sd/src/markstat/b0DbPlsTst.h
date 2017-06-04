///////////////////////////////////////////////////////////////////////////////
//
// TITLE        DB Pulse Test Class
//
// DESCRIPTION
//      This file contains the definition for the b0DbPlsTst class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0DbPlsTst.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef B0_DB_PLS_TST_H
#define B0_DB_PLS_TST_H


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
//      This class provides the DB pulse test.
//
///////////////////////////////////////////////////////////////////////////////
class cDbPlsTst
{

//-----------------------------------------------------------------------------
public:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;


    // Types & Enumerations
    //---------------------
    enum DbPtSelTyp                 // DB pulse test selection
    {
        PULSE_DB1          =  0,
        PULSE_DB2          =  1,
        PULSE_BOTH_DB      =  2,
    };

    struct InListTyp
    {
        const unsigned&     TripFlt;                //  1 - Trip fault active
        const unsigned&     PlsTstReq;              //  2 - DB pulse test request (from sequencer)
    };

    struct InsListTyp
    {
        const float&        DelTm1;                 //  1 - task1 delta time (sec)
    };


    // Methods
    //--------
    cDbPlsTst( InListTyp InList, InsListTyp InsList );
    ~cDbPlsTst();

    int  RunRules(void);
    void Task1(void);
    void Task2(void);

    // Output Signals
    //---------------
    CREATE_PUBVAR(DbPtStat,             unsigned);      // pulse test task Stat
    CREATE_PUBVAR(DbPtAct,              unsigned);      // pulse test task Active
    CREATE_PUBVAR(DbPtIp,               unsigned);      // Dynamic brake pulse test in progress flag

    CREATE_PUBVAR(DbPtStateCmd1,        unsigned);      // DB1 gate command
    CREATE_PUBVAR(DbPtStateCmd2,        unsigned);      // DB2 gate command

//-----------------------------------------------------------------------------
private:

    // External Inputs
    //----------------
    InListTyp    In;
    InsListTyp   Ins;

    // Methods
    //--------


    // Diagnostics
    //------------


    // Parameters
    //-----------
    CREATE_PARM(DbPlsTstTm,      float);            // pulse width(sec)


    // Published Variables
    //--------------------
    CREATE_PUBVAR(DbPtReq,        unsigned);        // DB pulse test request
    CREATE_PUBVAR(DbPtActT1,      unsigned);        // pulse test task Active task1 version

    CREATE_PUBVAR(DbPtPktDone,    unsigned);        // DB pulse test packet done
    CREATE_PUBVAR(DbPtPktDoneT2,  unsigned);        // DB pulse test packet done in task2
    CREATE_PUBVAR(DbPtT1St,       unsigned);        // DB pulse test state (task 1)
    CREATE_PUBVAR(DbPtPktsRm,     unsigned);        // Number of db pulse test packets remaining
    CREATE_PUBVAR(DbPtT2St,       int);             // DB pulse test state  (task 2)
    CREATE_PUBVAR(DbPtPktRdy,     unsigned);        // DB pulse test packet ready for task1
    CREATE_PUBVAR(DbPtPktRdyT1,   unsigned);        // DB pulse test packet ready for task1, task1 version
    CREATE_PUBVAR(DbPtPkts,       unsigned);        // Number of DB pulse test packet ready for task1
    CREATE_PUBVAR(DbPtPktsT1,     unsigned);        // Number of DB pulse test packet ready for task1,  task1 version
    CREATE_PUBVAR(DbPtTypeSt,     unsigned);        // DB test type state


    // Unpublished Variables
    //----------------------


    // Data Passing
    //-------------
    DATA_PASS(DbPtPktRdy,       DbPtPktRdyT1,     T2_T1,  UNFILTERED);
    DATA_PASS(DbPtAct,          DbPtActT1,        T2_T1,  UNFILTERED);
    DATA_PASS(DbPtPkts,         DbPtPktsT1,       T2_T1,  UNFILTERED);
    DATA_PASS(DbPtPktDone,      DbPtPktDoneT2,    T1A_T2, UNFILTERED);
};



// Inline Function Definitions
//----------------------------


#endif  // B0_DB_PLS_TST_H
