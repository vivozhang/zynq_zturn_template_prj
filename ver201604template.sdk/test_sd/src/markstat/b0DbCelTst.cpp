///////////////////////////////////////////////////////////////////////////////
//
// TITLE        DB Test Class
//
// DESCRIPTION
//      This file contains the definition for the b0DbCelTst class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0DbCelTst.h, b0DbCelTst.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include "x0MsgStk.h"
// product
#include "b0DbCelTst.h"


// Constants
//----------

// Diagnostic message constants
#define  DB_CT_MSG_STK_SZ  800

// Task 1 DB cell test states
#define DB_NORMAL             (0)
#define DB_CT_INIT            (1)
#define DB_CT_PULSE           (2)
#define DB_CT_STOP            (3)

// Task 2 DB cell test states
#define NO_ACTION             (0)
#define WAIT_FOR_REQUEST      (1)
#define PREPARE_FOR_TEST      (2)
#define START_TEST            (3)
#define WAIT_FOR_COMPLETION   (4)
#define TEST_FINISHED         (5)
#define START_DB_SHUTDOWN     (-1)
#define FINISH_DB_SHUTDOWN    (-2)

// DB resistor measuring states
#define  TEST_INIT         (1)
#define  VDC_VERIFY        (2)
#define  DB_TURNON         (3)
#define  CURR_PWR_MEAS     (4)
#define  DB_RES_CHECK      (5)
#define  MA_CLOSE_DIAG     (6)
#define  STOP_TEST         (7)

#define DB_ON_THRS      (0.99F)

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
cDbCelTst::cDbCelTst
(
        InListTyp    InList,
        InsListTyp   InsList
)
        : In(InList), Ins(InsList)
{
    DbCelTstInit();

    DbCtSt            = 0;
    OUT(DbCtIp)         = false;
    DbCtReq             = false;
    OUT(DbCtStat)       = false;
    OUT(DbCtAct)        = false;
    OUT(DbCtImeasReq)   = false;

    //-------------------------------------------------------------------------
    // turn off gate commands
    OUT(DbCtStateCmd1)  = false;
    OUT(DbCtStateCmd2)  = false;

    // DynamicBrakeCapMeas
    OUT(DbCapMsSt) =   0;
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
cDbCelTst::~cDbCelTst ()
{
}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Initialize class data members
//
// EXECUTION LEVEL
//      Class instancing and Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbCelTst::DbCelTstInit()
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
int cDbCelTst::RunRules (void)
{
    int Status = SUCCESS;

    SET_FV(DbCtPwrLm,   0.05*INS(DbOtThrs));
    SET_FV(DbResMsTolP, 0.19F); //19% Max is 2.02 + .202 + (200degc* 433e-6 pu/degc * 2.02) = 2.399 (18.6%) -- allow 19%
    SET_FV(DbResMsTolM, 0.14F); //14% Min is 2.02 - .202 - (55degc* 433e-6 pu/degc * 2.02) = 1.770  (12.4%) -- allow 14%
    SET_FV(DbCtTm,   2.0F);
    SET_FV(DbResMsStlTm, 0.01F);
    SET_FV(DbCtMaClsVdcPu, 0.95F);
    SET_FV(DbCtChgLvlAc, 0.80F);
    SET_FV(DbCtVdbZeroLvl, 0.05F); // DB res voltage low level is 5% of the DC link voltage
    SET_FV(DbResMsDtyReq,  1.0F);

    DbResMsDtyRcp=1.0/PARM(DbResMsDtyReq);

    // For DB cell test
    SET_FV(DbCtDlyTm2,  2.0F);
    SET_FV(DbCtDlyTm3,  5.0F);
    SET_FV(DbCtDlyTm4,  2.0F);
    SET_FV(DbCtDlyTm5,  1.0F);
    SET_FV(DbCtDlyTm6,  4.0F);
    SET_FV(DbCtDlyTm7,  1.0F);
    SET_FV(DbCtDlyTm8,  3.0F);
    SET_FV(DbCtDlyTm9,  3.0F);
    SET_FV(DbCtModPktSize, 1.0F);

    // For DB capacitance test
    SET_FV(DbCapMsVdcFilW, 20.0F);  //no calc required necessary
    DbCapMsVdcFilGn=PARM(DbCapMsVdcFilW)*INS(DelTm2);
    SET_FV(DbCapMsDlyTm1,   3.0F);
    SET_FV(DbCapMsDlyTm2,   2.0F);
    SET_FV(DbCapMsDlyTm3,   5.0F);
    SET_FV(DbCapMsDlyTm4,   1.0F);
    SET_FV(DbCapMsDlyTm5,   1.0F);
    SET_FV(DbCapMsDlyTm6,   3.0F);
    SET_FV(DbCapMsVdcRef,   0.367879F);
    SET_FV(DbCapMsTolP,     0.333F);//Individual cap range: 8,000uf to 12,000uf...noninal at 9,000
    SET_FV(DbCapMsTolM,     0.111F);//So, range it -11.1% to + 33%...temp variation is small
    DbCapMsCmeasMax=(1+PARM(DbCapMsTolP))*INS(DcCap);
    DbCapMsCmeasMin=(1-PARM(DbCapMsTolM))*INS(DcCap);

    if (PARM(ConverterLoc) == FACTORY)
    {
       SET_FV(DbCapMsDtyReq,  1.0F);
    }
    else
    {
       SET_FV(DbCapMsDtyReq,  1.0F);
    }

    DbCapMsDtyRcp=1.0/PARM(DbCapMsDtyReq);

    return (Status);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Task 1 level Dyanmic Breaker Cell Test. The outputs of this
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
void cDbCelTst::Task1 (void)
{
    DbCtBrgEnb = (DbResMsIpT1 || DbCapMsIpT1) && (DbCtDtyReqT1 > DB_ON_THRS);
    if (DbCtBrgEnb)
    {
        if(DbResMsIpT1)
        {
            DbCtDutyCmd = PARM(DbResMsDtyReq);
        }
        else if(DbCapMsIpT1)
        {
            DbCtDutyCmd = PARM(DbCapMsDtyReq);
        }
        else
        {
            DbCtDutyCmd = 0.0F;
        }

        DbCtModOutInt += (float)DbCtModOut;
        if(DbCtModOutInt>=PARM(DbCtModPktSize))
        {
            DbCtModPktInt += DbCtDutyCmd - PARM(DbCtModPktSize);
            DbCtModOutInt -= PARM(DbCtModPktSize);
        }
        else
        {
            DbCtModPktInt += DbCtDutyCmd;
        }
        // limit DbCtModPktInt
        if(DbCtModPktInt>=(PARM(DbCtModPktSize)+1.0F))
        {
            DbCtModPktInt = (PARM(DbCtModPktSize)+1.0F);
        }
        // limit DbCtModPktInt
        if(DbCtModPktInt>=(PARM(DbCtModPktSize)+1.0F))
        {
            DbCtModPktInt = (PARM(DbCtModPktSize)+1.0F);
        }
        // convert the duty to the command
        if ( DbCtModPktInt >= PARM(DbCtModPktSize) )  // || (DbOtLockFlg == true)
        {
            DbCtModOut = 1;
        }
        else
        {
            DbCtModOut = 0;
        }
    }
    else
    {
        DbCtModOutInt = 0.0F;
        DbCtModPktInt = 0.0F;
        DbCtModOut = 0;
        DbCtDutyCmd = 0.0F;
    }
    // if auto test, keep both DB on
    if( DbResMsIpT1 || DbCapMsIpT1 )
    {
        if( !DbCtIpT1 )
        {
            if ( DbCtDtyReqT1 > DB_ON_THRS )
            {
                OUT(DbCtStateCmd1) = DbCtModOut;
                OUT(DbCtStateCmd2) = DbCtModOut;
            }
            else
            {
                OUT(DbCtStateCmd1) = false;
                OUT(DbCtStateCmd2) = false;
            }
        }
        else
        {
            // cell test on each DB upon the selection. Keep the non-selected DB off
            if ( IN(DbCtSel) == CELL_TST_DB1 )
            {
                if ( DbCtDtyReqT1 > DB_ON_THRS )
                {
                    OUT(DbCtStateCmd1) = DbCtModOut;
                }
                else
                {
                    OUT(DbCtStateCmd1) = false;
                }
                DbCtStateCmd2 = false;
            }
            else if ( IN(DbCtSel) == CELL_TST_DB2 )
            {
                if ( DbCtDtyReqT1 > DB_ON_THRS )
                {
                    DbCtStateCmd2 = DbCtModOut;
                }
                else
                {
                    DbCtStateCmd2 = false;
                }
                OUT(DbCtStateCmd1) = false;
            }
            else
            {
                OUT(DbCtStateCmd1) = false;
                OUT(DbCtStateCmd2) = false;
            }
        }
    }
    // turn off the DBs if cell test is not active
    else
    {
        OUT(DbCtStateCmd1) = false;
        OUT(DbCtStateCmd2) = false;
    }
    //DbResMsRmsMeas(); future move rms measure function inside class
    //--------------------------------------------------------------
    
    OUT(DbCtMsgStkSt) = false;
    // done
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Task 2 level state machine for Dyanmic Breaker Pulse Test.
//      This function performs the dynamic brake cell test for the rotor
//      converter. The last message given by cell test must be one of the
//      following: 30,31,32,33,34.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbCelTst::Task2 (void)
{
    DynamicBrakeCellTest();
    DynamicBrakeResMeas();
    DynamicBrakeCapMeas();

    if(DbResMsIp)
    {
        DbCtDtyReq=DbResMsDtyReq;
        DbCtQ1OpnReq=false;
        DbCtKdfOpnReq=false;
    }
    else if(DbCapMsIp)
    {
        DbCtDtyReq=DbCapMsDtyReq;
        DbCtQ1OpnReq=true;
        DbCtKdfOpnReq=DbCapMsKdfOpnReq;
    }
    else
    {
        DbCtDtyReq=0.0F;
        DbCtQ1OpnReq=false;
        DbCtKdfOpnReq=false;
    }

    //--------------------------------------------------------------
    // done

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Task 2 level state machine for Dyanmic Breaker Pulse Test.
//      This function performs the dynamic brake cell test for the rotor
//      converter. The last message given by cell test must be one of the
//      following: 30,31,32,33,34.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbCelTst::DynamicBrakeCellTest (void)
{
      DbCtReq = IN(CelTstReq);

      DbCtTypeSt = DbCtReq | DbCtStat<<1 | DbCtAct <<2;

    switch ( DbCtTypeSt)            // state selection
    {
        case 0:
        case 3:
            DbCtSt = 0;           // do nothing state
            break;
        case 1:
        case 7:
            if ( IN(TripFlt) )
            {
                DbCtSt = 0;       // null state
            }
            else
            {
                DbCtSt = 1;       // 1st enable state
            }
            break;
        case 2:
        case 4:                     // 1st disable state
            if ( DbCtSt >= -1 )
            {
                DbCtSt = -1;
            }

            break;
        case 5:                     // continue with selected state
        case 6:
            break;                  // no change
        default:
            break;
    }

    //-----------------------------------------------------------------------------
    //The following cases are for over-head problems not solely detectable in any one given state
    if ( ( DbCtSt >= 2) && (DbCtSt <= 400) )
    {
        //-----------------------------------------------------------------------------
        //Trip Fault!! -- Abort DB Cell Test
        if ( IN(TripFlt) )
        {
            cMessageStack::PushMsg( 414, xINT, NULL );
            cMessageStack::PushMsg( 35, xINT, NULL ); //Blank line
            DbCtSt  = 999;
            //DbPwrEnb    = false;
        }

        //-----------------------------------------------------------------------------
        //DB expected voltage incorrect
        else if ( IN( Vdb1VdcMisFlg) || IN( Vdb2VdcMisFlg))
        {
            cMessageStack::PushMsg( 410, xINT, NULL );
            cMessageStack::PushMsg( 35, xINT, NULL );
            DbCtSt  = 999;
            //DbPwrEnb    = false;
        }

        //-----------------------------------------------------------------------------
        // DB on when expected off
        else if ( IN(Db1MisOnTstFlg) || IN(Db2MisOnTstFlg) )
        {
            cMessageStack::PushMsg( 411, xINT, NULL );
            cMessageStack::PushMsg( 35, xINT, NULL );
            DbCtSt  = 999;
            //DbPwrEnb    = false;
        }

        //-----------------------------------------------------------------------------
        // DB off when expected on
        else if ( IN(Db1MisOffTstFlg) || IN(Db2MisOffTstFlg) )
        {
            cMessageStack::PushMsg( 412, xINT, NULL );
            cMessageStack::PushMsg( 35, xINT, NULL );
            DbCtSt  = 999;
            //DbPwrEnb    = false;
        }

        //-----------------------------------------------------------------------------
        // Abort -- Link low, current high
        else if ( DbResMsLwFlg )
        {
            cMessageStack::PushMsg( 416, xINT, NULL );
            cMessageStack::PushMsg( 35, xINT, NULL );
            DbCtSt  = 999;
            //DbPwrEnb    = false;
        }

        //-----------------------------------------------------------------------------
        //"The line converter is not ready"
        else if ( !IN(L_RunRdy) )
        {
            cMessageStack::PushMsg( 406, xINT, NULL );
            cMessageStack::PushMsg( 35, xINT, NULL );
            DbCtSt  = 999;
            //DbPwrEnb    = false;
        }
    }


//==============================================================================
//                           DB Cell  Test Enable States
//==============================================================================

        switch ( DbCtSt)
        {

            case 0:              // No action case
                OUT(DbCtIp) = false;
                DbCtResMsReq= false;
                DbCtCapMsReq= false;
                break;

            case 1:
                OUT(DbCtAct) = true;

                OUT(DbCtIp)  = true;
                DbCtResMsReq = false;
                DbCtCapMsReq = false;

                if ( cMessageStack::MsgQuePres == false )   // message queue should have been opened already
                {                                           // thru a external request
                    PUSH_DIAG( DbCelTstMsgStkTrp );
                    DbCtSt = 999;
                }
                else
                {
                    DbCtSt = 10;
                }

                break;

            case 10:

                if ( IN(DbPwrInt) >= PARM(DbCtPwrLm))
                {
                    cMessageStack::PushMsg( 405, xINT, NULL ); //"DB resistor too hot!--allow to cool"
                    cMessageStack::PushMsg( 35, xINT, NULL );
                    DbCtSt    = 999;
                }
                else
                {
                    DbCtSt    = 20;
                }

                break;

            case 20:
                DbCtTmr  = 0.0F;

                cMessageStack::PushMsg( 413, xINT, NULL ); //Begin DB cell test
                cMessageStack::PushMsg( 35, xINT, NULL );
                DbCtSt = 30;

                break;

            case 30:
                DbCtSt = 40;

                break;

            // Call procedure to measure DB resistance
            // Set the request to true and wait for status
            case 40:
                // stop measuring DB resistor if no DB resistor voltage
                if ( DbResMsVfbkErr )
                {
                    cMessageStack::PushMsg( 500, xINT, NULL ); //"DB resistor voltage feedback issue, or DB resistor is disconnected, or DB IGBT does not work properly"
                    cMessageStack::PushMsg( 35, xINT, NULL );
                    DbCtSt = 999;
                }
                else if ( DbResMsSta && !DbResMsAct )
                {
                    if(DbResMsDon)
                    {
                        DbCtResMsReq = false;
                        DbCtTmr  = 0.0F;
                        DbCtSt = 50;
                    }
                    else
                    {
                        cMessageStack::PushMsg( 501, xINT, NULL ); //DB resistance test failed to complete
                        cMessageStack::PushMsg( 35, xINT, NULL );
                        DbCtSt = 999;
                    }
                }
                else
                {
                    DbCtResMsReq = true;
                }

                break;

            //Print out DB measurement value
            case 50:
                DbCtTmr = DbCtTmr + INS(DelTm2);

                if ( DbCtTmr > PARM(DbCtTm) )
                {
                    DbCtTmr   = 0.0;
                    cMessageStack::PushMsg(409, xFLOAT, &OUT(DbCtRmeas), NULL);  // :Measured DB resistance: 'OUT(DbCtRmeas)' ohms"
                    DbCtSt = 60;
                }
                break;

            //Check to make sure measured resistance is within tolerance
            case 60:
                DbCtTmr = DbCtTmr + INS(DelTm2);
                if ( DbCtTmr > PARM(DbCtDlyTm2))
                {
                    DbCtTmr   = 0.0F;
                    if ( (DbCtRmeas > ((1.0F + PARM(DbResMsTolP)) * INS(DbR)) ) )
                    {
                        cMessageStack::PushMsg( 408, xINT, NULL ); //"Measured resistance is too high"
                        cMessageStack::PushMsg( 35, xINT, NULL );
                        DbCtSt = 999;
                    }
                    else if ( (DbCtRmeas < ((1.0 - PARM(DbResMsTolM)) * INS(DbR)) ) )
                    {
                        cMessageStack::PushMsg( 421, xINT, NULL ); //"Measured resistance is too low"
                        cMessageStack::PushMsg( 35, xINT, NULL );
                        DbCtSt = 999;
                    }
                    else
                    {
                        DbCtRmeasRcp     = 1.0F/DbCtRmeas;

                        DbCtTmr=0.0F;
                        cMessageStack::PushMsg( 417, xINT, NULL ); //"Measured resistance is within spec"
                        cMessageStack::PushMsg( 35, xINT, NULL );
                        DbCtSt = 70;   ///will go to 70 when cap check is used later
                    }
                }
                break;


            case 70:  //Allow some recovery time

                DbCtTmr = DbCtTmr + INS(DelTm2);
                if ( DbCtTmr > PARM(DbCtDlyTm3))
                {
                    DbCtTmr = 0.0F;
                    DbCtSt = 75;
                }
                break;

            case 75:  //Begin capacitance test

                DbCtTmr = DbCtTmr + INS(DelTm2);
                if ( DbCtTmr > PARM(DbCtDlyTm4))
                {
                    DbCtTmr = 0.0F;
                    cMessageStack::PushMsg( 400, xINT, NULL ); //"Measuring dc link capacitance"
                    DbCtSt = 80;
                }
                break;

            case 80:  // Calculate the target link voltage
                DbCtSt = 90;
                break;
            case 90:  //Turn DB on at duty = 1.0
                DbCtCapMsReq=true;
                if(DbCapMsSta && !DbCapMsAct)
                {
                    if(DbCapMsDon)
                    {
                        DbCtCapMsReq=false;
                        DbCtSt = 110;
                        DbCtTmr=0.0F;
                    }
                    else
                    {
                        cMessageStack::PushMsg( 502, xINT, NULL ); //DB capacitance test failed to complete
                        cMessageStack::PushMsg( 35, xINT, NULL );
                        DbCtSt=999;
                    }
                }
                break;
            case 110:
                DbCtTmr = DbCtTmr + INS(DelTm2);
                if(DbCtTmr>PARM(DbCtDlyTm5))
                {
                    cMessageStack::PushMsg( 401, xFLOAT, &DbCtCmeasUf,NULL );
                    DbCtTmr=0.0F;
                    DbCtSt = 120;
                }
                break;
            case 120:  //Wait until link voltage goes to threshold
                DbCtTmr = DbCtTmr + INS(DelTm2);
                if (DbCtTmr>PARM(DbCtDlyTm6))
                {
                    DbCtTmr   = 0.0F;
                    if ( DbCtCmeas  > DbCapMsCmeasMax )
                    {
                        cMessageStack::PushMsg( 402, xINT, NULL ); //"Measured capacitance is too high"
                        cMessageStack::PushMsg( 35, xINT, NULL );
                        DbCtSt = 999;
                    }
                    else if (DbCtCmeas  < DbCapMsCmeasMin)
                    {
                        cMessageStack::PushMsg( 403, xINT, NULL ); //"Measured capacitance is too low"
                        cMessageStack::PushMsg( 35, xINT, NULL );
                        DbCtSt = 999;
                    }
                    else
                    {
                        cMessageStack::PushMsg( 404, xINT, NULL ); //"Measured capacitance is within spec"
                        cMessageStack::PushMsg( 35, xINT, NULL );
                        DbCtSt = 400;
                    }
                }
                break;

            case 400:
                DbCtTmr = DbCtTmr + INS(DelTm2);
                if ( DbCtTmr > PARM(DbCtDlyTm7))
                {
                    DbCtTmr   = 0.0F;
                    DbCtSt    = 600;
                    OUT(DbCtIp) = false;
                    cMessageStack::PushMsg( 30, xINT, NULL );    // <<Completed Successfully>>
                }
                break;

            case 600:
                DbCtTmr = DbCtTmr + INS(DelTm2);
                if ( DbCtTmr > PARM(DbCtDlyTm8))
                {
                    DbCtTmr     = 0.0;
                    DbCtSt      = 700;
                }
                break;

            case 700:
                OUT(DbCtStat)  = true;
                OUT(DbCtAct)   = false;
                break;

            // Case -1: First DB Pulse Test Disable State
            case -1:
                OUT(DbCtAct)        = true;

                OUT(DbCtIp)         = false;
                OUT(DbCtPsTstReq)   = false;
                DbCtResMsReq        = false;
                DbCtCapMsReq        = false;
                DbCtSt   = -2;
                break;

            // Case -2: Last DB pulse Test Disable State
            case -2:
                if ( CloseMsgQue() != SUCCESS )
                {
                    PUSH_DIAG( DbCelTstMsgStkTrp );
                }
                //DBMsgStkIni   = false;
                DbCtAct    = false;
                DbCtStat   = false;
                break;

            //Case 999 -- Clear the timer and flags
            case 999:
                OUT(DbCtPsTstReq)   = false;
                OUT(DbCtIp)         = false;
                DbCtResMsReq        = false;
                DbCtCapMsReq        = false;
                DbResMsLwFlg        = false;

                DbCtTmr = 0.0F;
                DbCtSt  = 1000;
                break;

            //Case 1000 -- Give the abort message/
            case 1000:
                DbCtTmr = DbCtTmr + INS(DelTm2);
                if ( DbCtTmr > PARM(DbCtDlyTm9))
                {
                    DbCtTmr = 0.0F;
                    DbCtSt  = 600;
                    cMessageStack::PushMsg( 31, xINT, NULL );    // <<< Failed Completion ! >>>
                }
                break;


            default:
                break;
        }

    //--------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function a test to make sure that the MA contactor is closed.
//      It is normally called after the contactor has been asked to close
//      and a sufficient amount of time has passed to allow for the delay
//      in its closing.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbCelTst::DynamicBrakeResMeas()
{
    if(DbCtIp)
    {
        DbResMsReq=DbCtResMsReq;
    }
    else
    {
        DbResMsReq=IN(ResMsReq);
    }

    if ( !DbResMsReq)
    {
         DbResMsSt = 0;
    }

    //==============================================================================
    //                           MA Contactor Close Test States
    //==============================================================================

    switch ( DbResMsSt)
    {

        case 0:             // DbResMsSt = 0
            DbResMsAct = false;
            DbResMsSta = false;

            DbResMsIp  = false;
            DbResMsDtyReq=0.0F;
            OUT(DbCtImeasReq)=false;

            DbResMsDon = false;
            DbResMsLwFlg=false;

            DbResMsTmr = 0.0F;

            if ( DbResMsReq)
            {
                DbResMsSt = TEST_INIT;
            }
            break;

        case TEST_INIT:         // DbResMsSt = 1
            DbResMsAct      = true;

            DbResMsIp       = true;
            DbResMsDtyReq   = 1.0F;
            DbResMsSt       = VDC_VERIFY;
            break;

        case VDC_VERIFY:       // DbResMsSt = 2
            if ( (IN(GmVMagPuT2) < PARM(DbCtChgLvlAc)) && OUT(DbCtIp) )
            {
                DbResMsSt  = STOP_TEST;
            }
            else if ( IN(VdcFbkT2) >= (PARM(DbCtMaClsVdcPu)*IN(VMagT2)*SQRT3) )
            {
                DbResMsSt       = DB_TURNON;
                DbResMsTmr      = 0.0F;
            }
            else
            {
                DbResMsIp       = false;
                DbResMsDtyReq   = 0.0F;
                DbResMsSt       = STOP_TEST;
            }

            break;

        case DB_TURNON:        // DbResMsSt = 3

            DbResMsTmr = DbResMsTmr + INS(DelTm2);

            if ( IN(VdcFbkT2) < INS(MAClsVdcMn))    // Line contactor MA close minimum allowed vdc check function is new added in 2.75
            {
                DbResMsIp       = false;
                DbResMsDtyReq   = 0.0F;
                DbResMsSt       = STOP_TEST;

                if ( (ABS(IN(IaFbk)) > (0.5*INS(DbResIMin))) || (ABS(IN(IbFbk)) > (0.5*INS(DbResIMin))) || (ABS(IN(IcFbk)) > (0.5*INS(DbResIMin))) )
                {
                    if ( !OUT(DbCtIp))
                    {
                        PUSH_DIAG(DbResLow);
                    }
                    DbResMsLwFlg    = true;
                    DbResMsSt       = STOP_TEST;
                    DbResMsIp       = false;
                    DbResMsDtyReq   = 0.0F;
                }
                else
                {
                    DbResMsIp       = false;
                    DbResMsDtyReq   = 0.0F;
                    DbResMsSt  = STOP_TEST;
                }

            }
            else if (DbResMsTmr >= PARM(DbResMsStlTm))  //Turn on for a short time to allow settling before measurement
            {
                DbResMsSt = CURR_PWR_MEAS;
            }
            break;

        case CURR_PWR_MEAS:           // DbResMsSt = 4
            OUT(DbCtImeasReq) = true;  //Go find the rms current for each phase for MAClsIMsCt task1s
            if ( IN(IMeasStat) )
            {
                OUT(DbCtRmeas)  = IN(VdbTst) * IN(VdbTst) * ABS(IN(BusPwrRcp))*DbResMsDtyRcp;
                DbResMsSt       = DB_RES_CHECK;
                OUT(DbCtImeasReq)   = false;
                DbResMsIp       = false;
                DbResMsDtyReq   = 0.0F;
            }
            else if ( IN(VdcFbkT2) < INS(MAClsVdcMn) )
            {
                DbResMsIp       = false;
                DbResMsDtyReq   = 0.0F;
                DbResMsSt       = STOP_TEST;

                if ( (ABS(IN(IaFbk)) > (0.5*INS(DbResIMin))) || (ABS(IN(IbFbk)) > (0.5*INS(DbResIMin))) || (ABS(IN(IcFbk)) > (0.5*INS(DbResIMin))) )
                {
                    if ( !OUT(DbCtIp)) // Will be added later
                    {
                       PUSH_DIAG(DbResLow);
                    }
                    DbResMsLwFlg    = true;
                    DbResMsSt       = STOP_TEST;
                    DbResMsIp       = false;
                    DbResMsDtyReq   = 0.0F;
                }
                else
                {
                    DbResMsIp       = false;
                    DbResMsDtyReq   = 0.0F;
                    DbResMsSt  = STOP_TEST;
                }

            }
            //20140408, ZJ, consider remove DbResMsVfbkErr check. This function exist in 2.75, not found in 1.6e. It
            //              should be already covered in DbCtl class. Another reason is in CURR_PWR_MEAS, DB is turn
            //              off, Vdb should be zero. Not match with logic.
            //if ( IN(VdbTst) < IN(VdcFbkT2)* PARM(DbCtVdbZeroLvl) )
            //{
            //    // DB resistor voltage feedback issue, or DB resistor is disconnected, or DB IGBT does not work properly
            //    DbResMsVfbkErr    = true;
            //    DbResMsDtyReq   = 0.0F;
            //    DbResMsSt       = STOP_TEST;
            //    DbResMsIp       = false;
            //}
            break;

        case DB_RES_CHECK:    // DbResMsSt = 5
            float tempDulDbScl;
            tempDulDbScl=(INS(DulDbEnb) && !DbCtIp) ? 2.0F : 1.0F;  // in cell test, only 1 bridge is fired
            DbCtRmeas=DbCtRmeas*tempDulDbScl;

            if ( !DbCtIp )
            {
                if ( ( DbCtRmeas > ((1.0 + PARM(DbResMsTolP))*INS(DbR)) ) )
                {
                    if ( !OUT(DbCtIp)) PUSH_DIAG(DbResHigh);
                    DbResMsSt = STOP_TEST;
                    DbResMsDon = true;
                    DbResMsAct = false; //Clearing this here allows time for calling function to remove request
                                        //before cycle has begun again
                }
                else if ( (DbCtRmeas < ((1.0 - PARM(DbResMsTolM))*INS(DbR)) ) )
                {
                    if ( !OUT(DbCtIp)) PUSH_DIAG(DbResLow);
                    DbResMsSt = STOP_TEST;
                    DbResMsDon = true;
                    DbResMsAct = false; //Clearing this here allows time for calling function to remove request
                    //before cycle has begun again
                }
                else
                {
                    DbResMsSt = STOP_TEST;
                    DbResMsDon = true;
                    DbResMsAct = false; //Clearing this here allows time for calling function to remove request
                    //before cycle has begun again
                }
            }
            else
            {
                DbResMsSt = STOP_TEST;
                DbResMsDon = true;
                DbResMsAct = false; //Clearing this here allows time for calling function to remove request
                //before cycle has begun again
            }
            break;

        case STOP_TEST:        // DbResMsSt = 7
            DbResMsDtyReq   = 0.0F;
            if(!DbResMsReq)
            {
                DbResMsSt   = 0;
            }
            DbResMsSta      = true;
            DbResMsAct      = false;
            DbResMsVfbkErr  = false;
            DbResMsIp       = false;
            OUT(DbCtImeasReq) = false;
            DbResMsDtyReq   = 0.0F;
            break;

        default:
            break;
    }

  //--------------------------------------------------------------
  // done
  return;
}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function measure DC link capacitor by measureing the discharge time.
//      It requires Q1 contactor is Open.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbCelTst::DynamicBrakeCapMeas()
{
    if(DbCtIp)
    {
        DbCapMsReq=DbCtCapMsReq;
    }
    else
    {
        DbCapMsReq=IN(CapMsReq);
    }

    switch(DbCapMsSt)
    {
        case 0:
            if(DbCapMsReq)
            {
                DbCapMsSt  = 1;
            }
            DbCapMsAct      = false;
            DbCapMsSta      = false;

            DbCapMsIp       = false;
            DbCapMsDtyReq   = 0.0F;
            DbCapMsQ1OpnReq = false;
            DbCapMsKdfOpnReq= false;

            DbCapMsDon      = false;

            DbCapMsVdcFil   = 0.0F;
            DbCapMsTmr      = 0.0F;
            DbCapMsRcTau    = 0.0F;
            DbCapMsVdcTrgt  = 0.0F;
            break;
        case 1:
            if(!DbCapMsReq)
            {
                DbCapMsSt   = -1;
            }
            DbCapMsAct      = true;

            DbCapMsIp       = true;
            DbCapMsDtyReq   = 0.0F;
            DbCapMsQ1OpnReq = true;

            if(!IN(Q1Stat))
            {
                DbCapMsTmr = 0.0F;
                DbCapMsSt  = 72;
            }
            break;
        case 72: // Wait 3 seconds, need parameter
            if(!DbCapMsReq)
            {
                DbCapMsSt  = -1;
            }
            DbCapMsTmr += INS(DelTm2);
            if(DbCapMsTmr>PARM(DbCapMsDlyTm1))
            {
                DbCapMsTmr = 0.0F;
                DbCapMsSt  = 75;
            }
            break;
        case 75: // Begin capacitance test
            if(!DbCapMsReq)
            {
                DbCapMsSt  = -1;
            }
            DbCapMsTmr += INS(DelTm2);
            DbCapMsVdcFil += DbCapMsVdcFilGn*(IN(VdcFbkT2)-DbCapMsVdcFil);
            if(DbCapMsTmr>PARM(DbCapMsDlyTm2))
            {
                //PushMsg(400,xINT,NULL);
                DbCapMsTmr = 0.0F;
                DbCapMsSt  = 80;
            }
            break;
        case 80: // calculate the target link voltage
            if(!DbCapMsReq)
            {
                DbCapMsSt  = -1;
            }
            //L_DBCapMeas = TRUE; replaced by DbCapMsIp, ZJ, 20140401
            DbCapMsVdcTrgt = PARM(DbCapMsVdcRef)*DbCapMsVdcFil;
            DbCapMsSt  = 90;
            break;
        case 90: // turn DB on at duty =1.0
            if(!DbCapMsReq)
            {
                DbCapMsSt  = -1;
            }
            DbCapMsDtyReq= 1.0F;
            DbCapMsTmr = 0.0F;
            DbCapMsSt  = 100;
            break;
        case 100: // wait until link voltage goes to threshold
            if(!DbCapMsReq)
            {
                DbCapMsSt  = -1;
            }
            if(IN(VdcFbkT2)<=DbCapMsVdcTrgt)
            {
                DbCapMsDtyReq= 0.0F;
                DbCapMsRcTau  = DbCapMsTmr;
                DbCapMsTmr = 0.0F;
                DbCapMsSt  = 110;
            }
            DbCapMsTmr += INS(DelTm2);  // move below target check since the target is from last task
            if(DbCapMsTmr>PARM(DbCapMsDlyTm3))
            {
                if ( !OUT(DbCtIp)) PUSH_DIAG(DbCapMsTmOut);
                DbCapMsDtyReq= 0.0F;
                DbCapMsSt  = -1;
                DbCapMsTmr = 0.0F;
            }
            break;
        case 110: // calculate capacitance and output the value
            if(!DbCapMsReq)
            {
                DbCapMsSt  = -1;
            }
            DbCapMsTmr += INS(DelTm2);
            if(DbCapMsTmr>PARM(DbCapMsDlyTm4))
            {
                float tempDulDbScl;
                tempDulDbScl=(INS(DulDbEnb) && !DbCtIp) ? 0.5F : 1.0F;  // in cell test, only 1 bridge is fired
                OUT(DbCtCmeas)=DbCapMsRcTau/(DbCtRmeas*tempDulDbScl)*PARM(DbCapMsDtyReq);
                DbCtCmeasUf=OUT(DbCtCmeas)*1.0e6F;
                DbCapMsTmr = 0.0F;
                DbCapMsSt  = 120;
            }
            break;
        case 120: //Output appropriate message based on capacitance found and allowed tolerance
            // Since DB capacitance test will not trigged by sequence, so no push diag here
            // For message output, cell test will handle
            DbCapMsSt  = 400;
            DbCapMsKdfOpnReq=true;
            break;
        case 400:
            if(!DbCapMsReq)
            {
                DbCapMsSt  = -1;
            }
            DbCapMsTmr += INS(DelTm2);
            if(DbCapMsTmr>PARM(DbCapMsDlyTm5))
            {
                DbCapMsTmr = 0.0F;
                DbCapMsSt  = 600;
            }
            break;
        case 600:
            if(!DbCapMsReq)
            {
                DbCapMsSt  = -1;
            }
            DbCapMsTmr += INS(DelTm2);
            if(DbCapMsTmr>PARM(DbCapMsDlyTm6))
            {
                DbCapMsTmr = 0.0F;
                DbCapMsSt  = 700;
            }
            break;
        case 700: // completed successfully
            if(!DbCapMsReq)
            {
                DbCapMsSt  = -1;
            }
            DbCapMsAct    = false;
            DbCapMsSta    = true;
            DbCapMsQ1OpnReq  = false;
            DbCapMsIp     = false;
            DbCapMsDon    = true;
            DbCapMsDtyReq= 0.0F;
            break;
        case 701: // fail to perform capacitance test
            if(!DbCapMsReq)
            {
                DbCapMsSt  = -1;
            }
            DbCapMsAct    = false;
            DbCapMsSta    = true;
            DbCapMsQ1OpnReq  = false;
            DbCapMsIp     = false;
            DbCapMsDon    = false;
            DbCapMsDtyReq= 0.0F;
            break;
        case -1:
            DbCapMsSt  = 0;
            DbCapMsAct    = true;
            DbCapMsSta    = false;
            DbCapMsQ1OpnReq  = false;
            DbCapMsKdfOpnReq=false;
            DbCapMsIp     = false;
            DbCapMsDtyReq= 0.0F;
            DbCapMsDon    = false;

            DbCapMsVdcFil = 0.0F;
            DbCapMsTmr = 0.0F;
        default:
            break;
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function asks for the allocation of buffer memory to be used
//      as message queue during the wizard execution.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
int cDbCelTst::OpenMsgQue(void)
{
    int OpnQueStatus;
    OpnQueStatus = cMessageStack::InitMsgStack( DB_CT_MSG_STK_SZ );
    if (OpnQueStatus == SUCCESS)
    {
        OUT(DbCtMsgStkSt) = true;
    }
    return OpnQueStatus;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function asks for the buffer memory closing that was used
//      as message queue during the wizard execution.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      Closing operation status (SUCCESS or FAIL)
//
///////////////////////////////////////////////////////////////////////////////
int cDbCelTst::CloseMsgQue(void)
{
    int    ClsQueStatus;

    ClsQueStatus = cMessageStack::FreeMsgStack();
    if(ClsQueStatus == SUCCESS)
    {
        OUT(DbCtMsgStkSt) = false;
    }

    return   ClsQueStatus;
}
