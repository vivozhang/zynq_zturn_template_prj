///////////////////////////////////////////////////////////////////////////////
//
// TITLE        <class title>
//
// DESCRIPTION
//      This file contains the definition for the class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////

#ifndef S0_DB_SIMULATOR_H
#define S0_DB_SIMULATOR_H


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


// Function Prototypes
//--------------------


// Types and Classes
//------------------
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <class description>
//
///////////////////////////////////////////////////////////////////////////////
class cDbSimulator
{
//-----------------------------------------------------------------------------
public:
    // Types
    //------
    struct InTyp
    {
        const int&           Db1Cmd;             // 1-DB1 gating cmd
        const int&           Db2Cmd;             // 2-DB2 gating cmd
        const float&         Vdc;                // 3-DC link voltage (Volt)
    };

    struct InsTyp
    {
        const float&         DelTm1;             // 1-task1 cycle time (Sec)
        const float&         Ldb1;               // 2-Dynamic break1 resistor parasitic inductance (Henrys)
        const float&         Ldb2;               // 3-Dynamic break2 resistor parasitic inductance (Henrys)
        const float&         Rdb1;               // 4-Dynamic break1 resistor (in series with each DB switch) (Ohm)
        const float&         Rdb2;               // 5-Dynamic break2 resistor (in series with each DB switch) (Ohm)
    };



    cDbSimulator( InTyp InList, InsTyp InsList );
    ~cDbSimulator();

    // Methods
    //--------
    int  RunRules            ( void );
    void Task1(void);

    // Output Signals
    //---------------

    CREATE_PUBVAR(DbI1Sim,           float);      // DB1 to DC-link Total Current    (AMP)
    CREATE_PUBVAR(DbI2Sim,           float);      // DB2 to DC-link Total Current    (AMP)
    CREATE_PUBVAR(DbVr1Sim,          float);      // DB1 resistor cross voltage      (Volt)
    CREATE_PUBVAR(DbVr2Sim,          float);      // DB2 resistor cross voltage      (Volt)

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;

    // External Inputs
    //----------------
    InTyp    In;
    InsTyp   Ins;

    // Methods
    //--------
    void    DbVoltAvgSim ( float& Vdc, int& DbSwState, float& Vdb, float& Udb );
    void    DbCurrentSim ( void );

    // Diagnostics
    //------------

    // Parameters
    //-----------

    CREATE_PARM(DbL1Sim,              float);     // L DB1 (parasitic with R DB1, counting for the cable inductance) (Henrys)
    CREATE_PARM(DbL2Sim,              float);     // L DB2 (parasitic with R DB1, counting for the cable inductance)  (Henrys)

    // Published Variables
    //--------------------

    CREATE_PUBVAR(DbSwState1Sim,      int) ;
    CREATE_PUBVAR(DbSwState2Sim,      int) ;

    CREATE_PUBVAR(DbU1Sim,            float);     // DB1 duty cycle
    CREATE_PUBVAR(DbU2Sim,            float);     // DB2 duty cycle

    CREATE_PUBVAR(DbVbrg1Sim,         float);     // DB1 leg cross voltage      (Volt)
    CREATE_PUBVAR(DbVbrg2Sim,         float);     // DB2 leg cross voltage      (Volt)

    // define these two as var to prepare for later using the dynamic inputs
    CREATE_PUBVAR(DbR1Sim,            float);     // R DB1 (Ohm)
    CREATE_PUBVAR(DbR2Sim,            float);     // R DB2 (Ohm)

    CREATE_PUBVAR(Db1WSim,            float);     // W for Db1 current filter   (rad)
    CREATE_PUBVAR(Db2WSim,            float);     // W for Db2 current filter   (rad)
    CREATE_PUBVAR(Db1WdtSim,          float);     // Wdt for Db1 current filter (rad * sec)
    CREATE_PUBVAR(Db2WdtSim,          float);     // Wdt for Db2 current filter (rad * sec)

    // Unpublished Variables
    //----------------------

    float   DbR1RcpSim;                         // reciprocal of R DB1
    float   DbR2RcpSim;                         // reciprocal of R DB2
    float   DbVdcSim;

    // Data Passing
    //-------------

};



// Inline Function Definitions
//----------------------------


#endif  // S0_DB_SIMULATOR_H
