///////////////////////////////////////////////////////////////////////////////
//
// TITLE        DB Test Class
//
// DESCRIPTION
//      This file contains the definition for the b0DbPlsTst class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0DbPlsTst.h, b0DbPlsTst.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
// product
#include "b0DbPlsTst.h"


// Constants
//----------

// Task 1 DB pulse test states
#define DB_NORMAL             (0)
#define DB_PT_INIT            (1)
#define DB_PT_PULSE           (2)
#define DB_PT_STOP            (3)

// Task 2 DB pulse test states
#define NO_ACTION             (0)
#define WAIT_FOR_REQUEST      (1)
#define PREPARE_FOR_TEST      (2)
#define START_TEST            (3)
#define WAIT_FOR_COMPLETION   (4)
#define TEST_FINISHED         (5)
#define START_DB_SHUTDOWN     (-1)
#define FINISH_DB_SHUTDOWN    (-2)


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
cDbPlsTst::cDbPlsTst
(
        InListTyp    InList,
        InsListTyp   InsList
)
        : In(InList), Ins(InsList)
{
    DbPtT2St            = WAIT_FOR_REQUEST;
    DbPtT1St            = DB_NORMAL;
    DbPtActT1           = false;
    DbPtPktRdy          = false;
    DbPtPktRdyT1        = false;
    DbPtPkts            = 0;
    DbPtPktsT1          = 0;
    DbPtPktsRm          = 0;
    DbPtPktDone         = false;
    DbPtPktDoneT2       = false;
    OUT(DbPtIp)         = false;
    DbPtReq             = false;
    OUT(DbPtStat)       = false;
    OUT(DbPtAct)        = false;

    //-------------------------------------------------------------------------
    // turn off gate commands
    OUT(DbPtStateCmd1)  = false;
    OUT(DbPtStateCmd2)  = false;
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
cDbPlsTst::~cDbPlsTst ()
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
int cDbPlsTst::RunRules (void)
{
    int Status = SUCCESS;


    return (Status);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Task 1 level Dyanmic Breaker Pulse Test State Machine
//      This function modulates the DB duty request so that the outputs are
//      in terms of a full task 1 'on' time, or packets.  The outputs of this
//      function are modulation packets, or full task1 'on' commands to the
//      FPGA, which specify either ON or OFF for the DB on a pass by
//      pass basis.  This function will produce a DB switching frequency which is
//      equal to the Task1 interrupt frequency.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbPlsTst::Task1 (void)
{
    //--------------------------------------------------------------
    // DB Pulse test is active
    if (DbPtActT1) //pulse test only
    {
        switch ( DbPtT1St)
        {
            //---------------------- NO_PULSE_TEST  -----------------------
            case DB_NORMAL:

                DbPtPktDone = false;
                if ( DbPtPktRdyT1)
                {
                    DbPtT1St   = DB_PT_INIT;
                    DbPtPktsRm = 0;
                    OUT(DbPtIp)= true;
                }
                else
                {
                    OUT(DbPtIp) = false;
                }

                // REBF turn off DB switches
                OUT(DbPtStateCmd1) = OFF;
                OUT(DbPtStateCmd2) = OFF;

                break;

            //---------------------- PACKETS_READY ------------------------
            //Pulse test has packets ready--copy the value
            case DB_PT_INIT:

                DbPtPktsRm = DbPtPktsT1;   //copy (becomes remaining packets to output)
                DbPtT1St   = DB_PT_PULSE;
                break;

            //---------------------- PULSE_OUTPUT -------------------------
            case DB_PT_PULSE:
                if ( DbPtPktsRm > 0)
                {
                    // pulse Db1
                    if ( PARM(L_DbPtSel) == PULSE_DB1 )
                    {
                        OUT(DbPtStateCmd1) = ON;
                        OUT(DbPtStateCmd2) = OFF;
                    }
                    // pulse Db2
                    if ( PARM(L_DbPtSel) == PULSE_DB2 )
                    {
                        OUT(DbPtStateCmd1) = OFF;
                        OUT(DbPtStateCmd2) = ON;
                    }
                    // pulse Db1 and Db2. Db2 is skewed by fpga (10 usec)
                    if ( PARM(L_DbPtSel) == PULSE_BOTH_DB )
                    {
                        OUT(DbPtStateCmd1) = ON;
                        OUT(DbPtStateCmd2) = ON;
                    }

                    DbPtPktsRm         = DbPtPktsRm - 1;
                }
                else
                {
                    //  pulse DB low when no packets remaining
                    DbPtStateCmd1       = OFF;
                    DbPtStateCmd2       = OFF;
                    DbPtPktDone         = true;
                    DbPtT1St            = DB_PT_STOP;
                }
                break;

            //---------------------- PULSE_STOP----------------------------
            case DB_PT_STOP:
                if ( !DbPtPktRdyT1)
                {
                    DbPtT1St  = DB_NORMAL;
                }

                // REBF turn off DB switches
                OUT(DbPtStateCmd1) = OFF;
                OUT(DbPtStateCmd2) = OFF;

                break;

            //--------------------------------------------------------------
            //  should not get to this state- somethings wrong so shutdown DB
            default:

                // REBF turn off DB switches
                OUT(DbPtStateCmd1) = OFF;
                OUT(DbPtStateCmd2) = OFF;
                DbPtPktDone        = true;
                DbPtT1St           = DB_PT_STOP;

                break;
        }
    }

    //--------------------------------------------------------------
    // DB pulse test not active
    else
    {
        DbPtPktsRm = 0;
        DbPtT1St   = DB_NORMAL;
        OUT(DbPtIp)= false;

        // REBF turn off DB switches
        OUT(DbPtStateCmd1) = OFF;
        OUT(DbPtStateCmd2) = OFF;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Task 2 level state machine for Dyanmic Breaker Pulse Test.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbPlsTst::Task2 (void)
{
    DbPtReq = IN(PlsTstReq);

    DbPtTypeSt = DbPtReq | DbPtStat<<1 | DbPtAct <<2;

    switch ( DbPtTypeSt )                        // state selection
    {
        case 0:

        case 3:
            DbPtT2St = NO_ACTION;                // do nothing state
            break;

        case 1:

        case 7:
            if ( IN(TripFlt) )
            {

                DbPtT2St = NO_ACTION;            // null state
            }
            else
            {
                DbPtT2St = WAIT_FOR_REQUEST;     // 1st enable state
            }
            break;

        case 2:

        case 4:                                  // 1st disable state
            if ( DbPtT2St >= START_DB_SHUTDOWN)
            {
                DbPtT2St = START_DB_SHUTDOWN;
            }
            break;

        case 5:                                  // continue with selected state
        {
            // no change
        }

        case 6:
            break;                               // no change

        default:
            break;
    }


//==============================================================================
//                           DB Pulse Test Enable States
//==============================================================================

    switch ( DbPtT2St)
    {
        //---------------------- No action case -------------------------------
        case NO_ACTION:

            break;

        //---------------------- WAIT_FOR_REQUEST  ----------------------------
        case WAIT_FOR_REQUEST:
            OUT(DbPtAct) = true;
            DbPtT2St     = PREPARE_FOR_TEST;

            break;

        //----------------------  PREPARE_FOR_TEST ----------------------------
        //Calculate the number of packets to send
        case PREPARE_FOR_TEST:
            DbPtPkts = (int)( 0.5 +  ( PARM(DbPlsTstTm)/INS(DelTm1) ) );
            DbPtT2St = START_TEST;

            break;

        //----------------------  START_TEST ----------------------------------
        //Signal that the packets are ready to be used
        case START_TEST:
            DbPtPktRdy = true;
            DbPtT2St   = WAIT_FOR_COMPLETION;

            break;

            //----------------------  WAIT_FOR_COMPLETION -------------------------
        case WAIT_FOR_COMPLETION:
            if ( IN(TripFlt) || !DbPtReq )
            {
                //test aborted
                DbPtT2St = START_DB_SHUTDOWN ;
            }
            else if ( DbPtPktDoneT2 )
            {
                DbPtT2St = TEST_FINISHED;
            }

            break;

        //----------------------  TEST_FINISHED -------------------------------
        case TEST_FINISHED:
            OUT(DbPtStat) = true;
            OUT(DbPtAct)  = false;
            DbPtT2St      = START_DB_SHUTDOWN ;

            break;

        //---------------------- START_DB_SHUTDOWN ----------------------------
        // First DB Pulse Test Disable State
        case START_DB_SHUTDOWN :
            DbPtPkts   = 0;
            DbPtPktRdy = false;
            DbPtT2St   = FINISH_DB_SHUTDOWN;

            break;

        //----------------------  FINISH_DB_SHUTDOWN --------------------------
        // Last DB pulse Test Disable State
        case FINISH_DB_SHUTDOWN:
            OUT(DbPtAct)    = false;
            OUT(DbPtStat)   = false;
            DbPtT2St        = WAIT_FOR_REQUEST;

            break;

        //--------------------------- default ---------------------------------
        //  should not get to this state- somethings wrong so shutdown DB
        default:
            DbPtT2St = START_DB_SHUTDOWN ;

            break;

    }
    return;
}
