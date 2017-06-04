///////////////////////////////////////////////////////////////////////////////
//
// TITLE        DbSimulator
//
// DESCRIPTION
//      This file contains the methods for the class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      s0DbSimulator.h, s0DbSimulator.vsd, s0DbSimulator.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
// product
#include "Product.h"
#include "s0DbSimulator.h"


// Constants
//----------


// Externals
//----------


// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Diagnostics
//------------


// Parameters
//-----------


// Published Variables
//--------------------


// Unpublished Variables
//----------------------


// Data Passing
//-------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class constructor to initialize resources
//
// EXECUTION LEVEL
//      Class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cDbSimulator::cDbSimulator
(
        InTyp    InList,
        InsTyp   InsList
)
        : In(InList), Ins(InsList)
{
    // initilize
    DbSwState1Sim   = 0;
    DbSwState2Sim   = 0;

    DbI1Sim         = 0.0;
    DbI2Sim         = 0.0;

    DbVr1Sim        = 0.0;
    DbVr2Sim        = 0.0;
    DbU1Sim         = 0.0;
    DbU2Sim         = 0.0;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cDbSimulator::~cDbSimulator ()
{
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by the class.
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cDbSimulator::RunRules (void)
{
    int Status = SUCCESS;

    SET_FV( DbL1Sim, INS(Ldb1) );
    SET_FV( DbL2Sim, INS(Ldb2) );

    return (Status);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Task 1 functions
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbSimulator::Task1 (void)
{
    // Get DB1 and DB2 SW cmd
    DbSwState1Sim = IN(Db1Cmd);
    DbSwState2Sim = IN(Db2Cmd);
    DbVdcSim      = IN(Vdc);

    // DB1 resistor voltage
    DbVoltAvgSim( DbVdcSim,DbSwState1Sim, DbVr1Sim, DbU1Sim );
    DbVbrg1Sim  = DbVr1Sim * DbU1Sim;

    // DB2 resistor voltage
    DbVoltAvgSim( DbVdcSim,DbSwState2Sim, DbVr2Sim, DbU2Sim );
    DbVbrg2Sim  = DbVr2Sim * DbU2Sim;

    // Calculate the DB total current to DC link
    DbCurrentSim();

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Simulate average DB Voltages over the Pwm Interval
//      from Commanded Bridge Events & DC Link Voltages.  This function
//      assumes full frame gating of the DB.

//      This function assumes gating and feedback through an AEDB:
//          The DB1 sensed voltage = resistor voltage cross the Rdb1
//          The DB2 sensed voltage = resistor voltage cross the Rdb2
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbSimulator::DbVoltAvgSim
(
    // Inputs
    float& DbVdcSim,              // DC link voltage
    int  & DbSwStateSim,        // DB gating command
    // Outputs
    float& DbVSim,              // DB resistor average voltage
    float& DbUSim               // DB switch duty cycle
)
{
    if ( DbSwStateSim )
    {
        DbUSim   = 1.0;
        DbVSim   = DbVdcSim;      // resistor avg voltage (including the parasitic inductance voltage drop)
    }
    else
    {
        DbUSim   = 0.0;
        DbVSim   = 0.0;         // resistor avg voltage (including the parasitic inductance voltage drop)
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Simulate dynamic break current
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbSimulator::DbCurrentSim (void)
{
    // Wdt calculation
    //------------
    DbR1Sim    = INS( Rdb1 );
    DbR2Sim    = INS( Rdb2 );
    DbR1RcpSim = NoNANLim( 1.0 / DbR1Sim, 1.0e6, -1.0e6);
    DbR2RcpSim = NoNANLim( 1.0 / DbR2Sim, 1.0e6, -1.0e6);
    Db1WSim    = NoNANLim( DbR1Sim / INS(Ldb1), 1.0e6, -1.0e6);
    Db1WdtSim  = Db1WSim * INS(DelTm1);
    // clamp the Wdt to [0, 1]
    if ( Db1WdtSim > 1.0 )
    {
        Db1WdtSim = 1.0;
    }
    else if ( Db1WdtSim < 0.0 )
    {
        Db1WdtSim = 0.0;
    }
    Db2WSim    = NoNANLim( DbR2Sim / INS(Ldb1), 1.0e6, -1.0e6);
    Db2WdtSim  = Db2WSim * INS(DelTm1);

    // clamp the Wdt to [0, 1]
    if ( Db2WdtSim > 1.0 )
    {
        Db2WdtSim = 1.0;
    }
    else if ( Db1WdtSim < 0.0 )
    {
        Db2WdtSim = 0.0;
    }

    //-------------------------------------------------------------------------
    // DB1 to DC link
    OUT(DbI1Sim)  += Db1WdtSim * ( DbVbrg1Sim * DbR1RcpSim - DbI1Sim );

    //-------------------------------------------------------------------------
    // DB2 to DC link
    OUT(DbI2Sim)  += Db2WdtSim * ( DbVbrg2Sim * DbR2RcpSim - DbI2Sim );

    return;
}
