///////////////////////////////////////////////////////////////////////////////
//
// TITLE:        Core drive sequencer state machine
//
// DESCRIPTION:
//      High level rotor sequencer state machine and initialization routines.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// core
//framework
#include "x0FrameWork.h"
#include "x0MsgStk.h"
// product
#include "Product.h"
#include "b0BridgeRebf.h"
#include "b0BrgFbkOffs.h"
#include "b0CelTst.h"
#include "b0PlsTst.h"
#include "b0DbCelTst.h"
#include "b0DbCtl.h"
#include "p0Sseq.h"
#include "p1Sseq.h"


// Externals
//----------
extern unsigned 		SimAcclAct;
extern FaultIsActiveTyp FaultIsActive;
extern CtlStsDat        CSDat;
extern R_SeqActBits     R_SeqAct;      // Sequencer Task Active bits
extern unsigned         R_SeqStBits_PackedBits;
extern unsigned         R_SeqReq_PackedBits;
extern unsigned         R_SeqStat_PackedBits;
extern unsigned         R_SeqStatT1_PackedBits;
extern unsigned         R_MctlCmd_PackedBits;
extern SysTm_Typ        SysTm3;

extern float            DelTm2;
extern float            R_SpdFbk;
extern float            R_TorTrqMgPk;
extern float            R_ZeroSpdLvl;
extern float            R_ZeroSpdVal;
extern float            R_VdcFbkT2;
extern float            R_VMagT2;
extern float            R_VbrgSqr;


extern unsigned         IslK1OpnReq;
extern unsigned         Brg2TripFlt;
extern unsigned         CelTstBrgSel;
extern unsigned         L_RunRdy;
extern unsigned         LocalMode;
extern unsigned         HdwTstMode;
extern unsigned         R_BrgPwrEnb;
extern unsigned         InitDone;
extern unsigned         R_InitDone;
extern unsigned         R_InnrRegEnb;
extern unsigned         R_SftyChnOpn;
extern int              R_TskInfcRdy;
extern unsigned         R_VdcRdy;
extern unsigned         R_ZVGtOff;
extern unsigned         RPNWRunRdy;
extern unsigned         VdcIsOk;
extern unsigned         VgridIsOk;
extern unsigned         ZVRTIp;
extern int              DbInh;
extern unsigned         L_MaStatus;     // Line main contactor status
extern unsigned         R_CroBarActT2;
extern unsigned         S_VMaxHVRT;

extern unsigned         R_SeqStopped;

extern cBridgeRebf      R_BridgeRebf;
extern cRotorBrgFbkOffs R_RotorBrgFbkOffs;
extern cCellTest        R_CellTest;

extern float            PdcTemp;
extern unsigned         RunCmd;



// Constants
//----------
// X Stop configuration definitions
#define XSTP_NRM_STP        0       // X stop is ramp stop - removal of run
//#define XSTP_QCK_STP        1       // X stop is quick stop
//#define XSTP_CST_STP        2       // X stop is coast stop
#define XSTP_TRP_FLT        3       // X stop is trip fault
#define XSTP_EMER_STP       4       // X stop is emergency ramp stop -removal of run

// Normal stop configuration definitions
#define NRMSTP_QCK_STP      1       // Normal stop is quick stop
#define NRMSTP_CST_STP      2       // Normal stop is coast stop

// Flying restart definitions
#define FLY_DISABLE         0       // Disable Flying restart
#define FLY_ENABLE          1       // Enable Flying restart
#define FLY_LOCK_SHAFT      2       // Locked-shaft restart


// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(SyncK1TstSgmVmax,   float);     // max pu volts on 6 kV winding to allow RunRdy in Sync_K1_Test mode
CREATE_PARM(R_VbrgUpW,          float);     // omegas for capturing and retaining
CREATE_PARM(R_VbrgDnW,          float);     // rotor voltage peaks (Rad/Sec)
CREATE_PARM(R_AutoCTEnb,        unsigned);  // enable rotor auto cell test during start
CREATE_PARM(R_CCChkCTEnb,       unsigned);  // rotor crobar contactor check cell test enable
CREATE_PARM(R_SeqQ1TmDly,       float);     // Q1 breaker sequencing time delay (Sec)
CREATE_PARM(R_XStpCfg,          int);       // X stop behavior configuration
CREATE_PARM(R_RunPrmAdr,        unsigned*); // Run permissive pointer
CREATE_PARM(R_StrtPrmAdr,       unsigned*); // Start permissive pointer
CREATE_PARM(K1RunPermMinTemp,   float);
// RotorCoreDriveSequencer()
CREATE_PARM(R_ZeroSpdDly,       float);     // Zero speed delay (sec);

// Diagnostics
//------------
CREATE_DIAG(R_SpdHighAlm);          // speed is too high to start
CREATE_DIAG(R_SpdLowAlm);           // speed is too low to start
CREATE_DIAG(R_StrtPrmOpn);          // Sequence: Start permissive open
CREATE_DIAG(R_SpdFbkRev);           // Sequence: Speed feedback reversed Trip Fault
CREATE_DIAG(R_RunPrmOpen);          // Sequence: Run permissive open

// Variables
//----------

// Command variables
CREATE_PUBVAR(R_CTDismTmr  ,  float   ) ;        // Rotor cell test dismissal timer
CREATE_PUBVAR(R_CelTstCmd  ,  unsigned) = false; // Cell Test command
CREATE_PUBVAR(R_MctlTunCmd ,  unsigned) = false; // Motor Control tune command
CREATE_PUBVAR(R_PlsTstCmd  ,  unsigned) = false; // Pulse Test command



CREATE_PUBVAR(R_RunCmd     ,  unsigned) = false; // Run command
CREATE_PUBVAR(R_RunReq     ,  unsigned) = false; // Run request

//Core drive sequencer variables
CREATE_PUBVAR(R_StopStTmr   , float);            // Stop state timer
CREATE_PUBVAR(R_SeqSt       , int) = ROTOR_INIT; // Sequencer state variable
CREATE_PUBVAR(R_RunAct      , unsigned) = false; // Run request is active
CREATE_PUBVAR(R_Running     , unsigned) = false; // Drive is running
CREATE_PUBVAR(R_RunPrm      , unsigned) = false; // Run permissive (local or remote)
CREATE_PUBVAR(R_RunRdy      , unsigned) = false; // Drive is ready to run
CREATE_PUBVAR(R_SpinRdy     , unsigned) = false; // Rotor bridge Ready for SpinUp
CREATE_PUBVAR(R_StrtPrm     , unsigned) = false; // Start permissive (local or remote)
CREATE_PUBVAR(R_ZeroSpd     , unsigned) = true;  // Drive is at zero speed
CREATE_PUBVAR(R_TrqEnbReq   , unsigned);         // Rotor sequencer torque-enable request

CREATE_PUBVAR(R_IoffInitTm  , float);            // Delay time for initial I offset null
CREATE_PUBVAR(R_AutoCTAct   , unsigned);         // Rotor automatic cell test is active
CREATE_PUBVAR(R_SpdLow      , unsigned);         // Speed is below minimum operating value
CREATE_PUBVAR(R_SpdHigh     , unsigned);         // Speed is above the sync speed
CREATE_PUBVAR(R_SyncIOCEnb  , unsigned);
CREATE_PUBVAR(R_AutoCTDone  , unsigned) = false; // Auto cell test has completed
CREATE_PUBVAR(R_CCChkDone   , unsigned) = false; // Crowbar contactor check has completed
CREATE_PUBVAR(R_CCDlyTmr    , float);
CREATE_PUBVAR(R_SyncReqTmr  , float);
CREATE_PUBVAR(R_TrqDlyTmr   , float);
CREATE_PUBVAR(R_VpeakSqr   , float);            // Peak of rotor-1 voltage summation squared (Volt^2)
CREATE_PUBVAR(R2_VpeakSqr   , float);            // Peak of rotor-2 voltage summation squared (Volt^2)
CREATE_PUBVAR(R_VbrgUpGn    , float);            // Filter gain for raising rotor voltage (dimensionless units)
CREATE_PUBVAR(R_VbrgDnGn    , float);            // Filter gain for falling rotor voltage (dimensionless units)

                                                 // Logical factors in Local Run & Spin Readiness
CREATE_PUBVAR(DbDualInh     , unsigned) = false; //
CREATE_PUBVAR(DbRdy         , unsigned) = false; //
CREATE_PUBVAR(SCEnbReq      , unsigned) = false; //

CREATE_PUBVAR(R_DiagCmd      , unsigned) = false; //
CREATE_PUBVAR(R_RunRdyVar1  , unsigned) = false; //

CREATE_PUBVAR(R_K1TempRunPerm,  unsigned);

CREATE_PUBVAR(R_StatLegTstLfT2, unsigned);       // Status of leg test control in task2

// DB varialbles




CREATE_PUBVAR(S_VzeroSeqChkEnb, unsigned);       // enable the stator zero sequence voltage check

// Unpublished Variables
//----------------------
SysTm_Typ             R_ZeroSpdTmr;              //Timer for zero speed


unsigned              R_SeqCelTstReq;            // Auxiliary var to hold conversion of single bit into boolean
cCellTest::AutoCtTyp  R_AutoCelTst = cCellTest::AUTOCT_OFF;    // Rotor auto-cell-test type


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Data Passing
//--------------
DATA_PASS( Rx_LegTestLowFrq.RegStatLegTstLf, R_StatLegTstLfT2, T1A_T2, UNFILTERED );




///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rotor core drive-sequencer. Core drive sequencer state machine that
//      brings bridge from a stopped condition to a running condition.
//      Coordinates bridge normal and fault shutdowns.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorCoreDriveSequencer(void)
{
    unsigned  Stat_Bits;
    unsigned  Act_Bits;
    int       R_CTVar;
    float     VdeltaSqr;

    // Insert non-state tasks here

    // if Run permissive pointer is active, populate R_RunPrm with ptr value
    // otherwise R_RunPrm is true
    if ((int)PARM(R_RunPrmAdr) != LADR_UnusedBool)
    {
        R_RunPrm = gVariable.ReadAsBool(PARM(R_RunPrmAdr));
    }
    else
    {
        R_RunPrm = true;
    }
    // If Run permissive is open, generate brief warning
    if (!R_RunPrm)
    {
        PUSH_DIAG(R_RunPrmOpen);
    }

    if (PdcTemp >= PARM(K1RunPermMinTemp))  // Allow rotor to run only when Cfc temperature is above a threshold
    {
        R_K1TempRunPerm = true;
    }
    else
    {
        R_K1TempRunPerm = false;
    }


    // if Start permissive pointer is active, populate Strt_Prm with ptr value
    // otherwise Strt_Prm is true
    if ((int)PARM(R_StrtPrmAdr) != LADR_UnusedBool)
    {
        R_StrtPrm = gVariable.ReadAsBool(PARM(R_StrtPrmAdr)) || !R_SeqStBits.R_Stopped;
    }
    else
    {
        R_StrtPrm = true;
    }
    // If Start permissive is open, generate brief warning
    if (!R_StrtPrm)
    {
        PUSH_DIAG(R_StrtPrmOpn);
    }

    // Check if speed too low to start
    if (R_SpdFbk > PARM(R_RPMMinOp))
    {
        R_SpdLow = false;
    }
    // Use ZeroSpdThr for hysterisis (based on tach quantization)
    else if ( (R_SpdFbk < (PARM(R_RPMMinOp) - R_ZeroSpdVal)) && (PARM(TestMode) == TEST_DISABLED) )
    {
        R_SpdLow = true;
    }

    // Check if speed too high to start
    // only give run permissive if the speed is sub-synchronous speed.
    // can override R_RPMMaxStart to chage it. Default: R_RPMSync
    if (R_SpdFbk > PARM(R_RPMMaxStart))
    {
        R_SpdHigh = true;
    }
    else
    {
        R_SpdHigh = false;
    }


    if (R_SeqStBits.R_Stopped)
    {
        // If speed too low to start, generate brief warning
        //if (R_SpdLow && !LocalMode && gVariable.ReadAsBool(PARM(RunReqAdr)) )
        if (R_SpdLow && !LocalMode && RunCmd )
        {
            PUSH_DIAG(R_SpdLowAlm);
        }

        // If speed too high to start, generate brief warning
        if (R_SpdHigh && !LocalMode && RunCmd )
        {
            PUSH_DIAG(R_SpdHighAlm);
        }

        // Check for reversed tach
        if ( R_SpdFbk < -PARM(R_RPMMinOp ))
        {
            PUSH_DIAG(R_SpdFbkRev);
        }
    }

    // Form R_DiagCmd - composite of all diagnostic/tune-up test commands
    R_DiagCmd = R_PlsTstCmd || R_CelTstCmd || R_MctlTunCmd;

    /* If any run command or not stopped and Local permissive fault or System
       permissive fault, call appropriate fault */


    //---------------------------------------------------------
    // Intermediate signals on behalf of Line Seqeuence
    DbDualInh   = (DbInh==cDbCtl::DUAL_INHIBIT);

    /* Drive is ready to run */
    R_RunRdyVar1 = R_RunPrm && R_StrtPrm && !Brg2TripFlt && !R_SftyChnOpn && R_K1TempRunPerm
              && R_TskInfcRdy
              && (R_SeqSt > ROTOR_STOPPED || (R_SeqSt == ROTOR_STOPPED));


    R_SpinRdy   = R_RunRdyVar1 && (R_VdcRdy  || R_PlsTstCmd);

    RPNWRunRdy =  R_RunRdyVar1 &&
                  L_RunRdy &&
                  VgridIsOk &&
                  VdcIsOk;

    R_RunRdy   = R_RunRdyVar1 &&
              (((!R_SpdLow ||
                 !R_SeqStBits.R_Stopped ||
                  R_CelTstCmd ||
                  R_MctlTunCmd) &&
                  R_VdcRdy ) ||
                  R_PlsTstCmd );



    // Close message stack used for cell-test wizard if test was never initiated
    if ( R_CTDismTmr > 0.0 )
    {
        R_CTVar = FAIL;
        if ( PARM(CtBrgSel) == CT_ROTOR_BRG1 )
        {
            R_CTVar = R_CellTest.CellTestDismissal( 1 , R_CTDismTmr );
        }
        if ( R_CTVar == BUSY )
        {
            R_CTDismTmr = R_CTDismTmr + DelTm2;
        }
        else
        {
            R_CTDismTmr = 0.0;
        }
    }

    // If drive is not ready to run, remove all run requests
    if (!R_RunRdy)
    {
        R_RunReq        = false;
        if ( R_CelTstCmd && !R_SeqAct.R_CelTstAct )   // Dismiss cell test if it was never initiated
        {
            R_CTVar = FAIL;
            if ( PARM(CtBrgSel) == CT_ROTOR_BRG1 )
            {
                R_CTVar = R_CellTest.CellTestDismissal( 0 , 0.0 );
            }

            if ( R_CTVar == SUCCESS )
            {
                R_CTDismTmr = DelTm2;
            }
        }
        R_PlsTstCmd     = false;
        R_CelTstCmd     = false;
        R_MctlTunCmd    = false;
    }

    // Form Drive commands
    R_RunCmd         = R_RunRdy && R_RunReq;

    // Form Run Active signal
    R_RunAct = R_Running && R_RunReq;

    // Zero Speed calculation - If threshold is 0 or negative, or then
    // R_ZeroSpd is always set to true

    if ((ABS(R_SpdFbk) < R_ZeroSpdLvl) || (R_ZeroSpdLvl <= 0))
    {   // Delay setting R_ZeroSpd true for specified time delay
        if (DELTA_TM(SysTm3, R_ZeroSpdTmr) >= PARM(R_ZeroSpdDly))
        {
            R_ZeroSpd = true;
        }
    }
    else
    {
        R_ZeroSpd = false;
        R_ZeroSpdTmr = SysTm3;  // Update timer when not @ zero speed
    }

    //--------------------------------------------------------------------------
    // Retain peak of summation squared rotor bridge voltages
    // to protect against crowbar contactor welding

    if ( (R_SeqSt == ROTOR_RUNNING) ||     // precondition peak summation
         (R_SeqSt == ROTOR_STOPPED) )
    {
        VdeltaSqr   = 0.0F;
        R_VpeakSqr = 3.0F * R_VMagT2 * R_VMagT2;
        R2_VpeakSqr = R_VpeakSqr;
    }
    else
    {
        VdeltaSqr = R_VbrgSqr - R_VpeakSqr;

        if ( VdeltaSqr > 0.0F )
        {
            R_VpeakSqr += R_VbrgUpGn* VdeltaSqr;
        }
        else
        {
            R_VpeakSqr += R_VbrgDnGn * VdeltaSqr;
        }
    }



//--------------------------
// State Machine begins here

    switch ( R_SeqSt )
    {
        //-----------------------------  ROTOR_STOPPED  ------------------------
        case ROTOR_STOPPED:

            // Clear Coast stop, Quick stop, Running booleans on entering R_Stopped state
            R_Running    = false;
            // Disable synchronization IOC fault
            R_SyncIOCEnb = false;

            // Clear automatic cell and Db sequencer test done flags
            R_AutoCTDone   = false;
            R_CCChkDone    = false;

            R_StopStTmr = R_StopStTmr + DelTm2;


            // Boolean OR of sequencer task Status bits
            Stat_Bits =  R_SeqStat.R_CCEnbStat || R_SeqStat.R_SyncStat || R_SeqStat.R_SCEnbStat || R_SeqStat.R_TrqEnbStat ||
                         R_SeqStat.R_DbTstStat;

            // Boolean OR of sequencer task Active bits
            Act_Bits  =  R_SeqAct.R_CCEnbAct || R_SeqAct.R_SyncAct || R_SeqAct.R_SCEnbAct || R_SeqAct.R_TrqEnbAct ||
                         R_SeqAct.R_DbTstAct;

            // Only transition state if not Trip fault & no active tasks
            if (!Brg2TripFlt && !Stat_Bits && !Act_Bits)
            {
                if (R_RunCmd)
                {
                    if ( PARM(R_AutoCTEnb) )
                    {
                        R_SeqSt = ROTOR_AUTO_CELL_TEST;
                    }
                    else
                    {
                        R_SeqSt = ROTOR_CONTACTOR_PICKUP;
                    }
                    R_StopStTmr = 0.0;
                }
                if ( R_DiagCmd )
                {
                    R_SeqSt = ROTOR_DIAGNOSTIC;
                    R_StopStTmr = 0.0;
                }
            }
            else // Brg2TripFlt or Stat_Bits or Act_Bits
            {
                // Check for crowbar contactor enabled
                if (R_SeqStat.R_CCEnbStat || R_SeqAct.R_CCEnbAct)
                {
                    R_SeqReq.R_CCEnbReq = false;
                }
                // Check for Torque enabled
                if (R_SeqStat.R_TrqEnbStat || R_SeqAct.R_TrqEnbAct)
                {
                    R_SeqReq.R_TrqEnbReq = false;
                }
                // Check for sync enabled
                if (R_SeqStat.R_SyncStat || R_SeqAct.R_SyncAct)
                {
                    R_SeqReq.R_SyncReq = false;
                }
                // Check for sync contactor enabled
                if (R_SeqStat.R_SCEnbStat || R_SeqAct.R_SCEnbAct )
                {
                    R_SeqReq.R_SCEnbReq = false;
                }
            }

            break;

        //---------------------- ROTOR_AUTO_CELL_TEST --------------------------
        case ROTOR_AUTO_CELL_TEST:
            // Disable cell test if fault occurs or loss of run command
            if (Brg2TripFlt || !R_RunCmd)
            {
                R_SeqReq.R_CelTstReq = false;
                if (!R_SeqStat.R_CelTstStat && !R_SeqAct.R_CelTstAct)
                {
                    R_AutoCTAct = false;
                    R_SeqSt = ROTOR_DISABLE_ALL;
                }
            }
            else if (R_SeqStat.R_CelTstStat || R_AutoCTDone)
            {
                //  Go to next state when auto-cell test is complete
                R_AutoCTDone = true;
                R_SeqReq.R_CelTstReq = false;
                if (!R_SeqStat.R_CelTstStat && !R_SeqAct.R_CelTstAct)
                {
                    R_AutoCTAct = false;
                    R_SeqSt = ROTOR_CONTACTOR_PICKUP;
                }
            }
            else    // Initiate cell test
            {
                R_AutoCTAct = true;           // indicate non-adl cmd
                CelTstBrgSel  = CT_ROTOR_BRG1;  // select rotor bridge
                R_SeqReq.R_CelTstReq = true;  // Set cell test request
            }
            break;



        //------------------------  CONTACTOR_PICKUP  --------------------------
        case ROTOR_CONTACTOR_PICKUP:
            if (Brg2TripFlt)
            {
                R_SeqReq.R_CCEnbReq = false; // Remove contactor request
                R_SeqSt = ROTOR_DISABLE_ALL;
            }
            else if (!R_RunCmd)
            {
                R_SeqSt = ROTOR_DISABLE_ALL;
            }
            else // not Brg2TripFlt and R_RunCmd
            {
                R_SeqReq.R_CCEnbReq = true;     // Request 1st contactor open

                if ( R_SeqStat.R_CCEnbStat && !R_SeqAct.R_CCEnbAct)
                {
                    if (PARM(R_CCChkCTEnb))
                    {
                       R_SeqSt = ROTOR_CONTACTOR_VERIFY;
                    }
                    else
                    {
                       R_SeqSt = ROTOR_SYNC_STATOR;
                    }
                }
            }
            break;

        //------------------------ ROTOR_CONTACTOR_VERIFY ----------------------
        case ROTOR_CONTACTOR_VERIFY:
            // Disable cell test if fault occurs or loss of run command
            if (Brg2TripFlt || !R_RunCmd)
            {
                R_SeqReq.R_CelTstReq = false;
                if (!R_SeqStat.R_CelTstStat && !R_SeqAct.R_CelTstAct)
                {
                    R_AutoCTAct = false;
                    R_SeqSt = ROTOR_DISABLE_ALL;
                }
            }
            else if (R_SeqStat.R_CelTstStat || R_CCChkDone)
            {
                //  Go to next state when crowbar check is complete
                R_CCChkDone = true;
                R_SeqReq.R_CelTstReq = false;
                if (!R_SeqStat.R_CelTstStat && !R_SeqAct.R_CelTstAct)
                {
                    R_AutoCTAct = false;
                    R_SeqSt = ROTOR_SYNC_STATOR;
                }
            }
            else    // Initiate cell test
            {
                R_AutoCTAct = true;           // indicate non-adl cmd
                CelTstBrgSel  = CT_ROTOR_BRG1;  // select rotor bridge
                R_SeqReq.R_CelTstReq = true;  // Set cell test request
            }
            break;

        //-------------------------- ROTOR_SYNC_STATOR -------------------------
        case ROTOR_SYNC_STATOR:   // to Line
            if (Brg2TripFlt)
            {
                R_SeqReq.R_SyncReq = false;    // Remove sync request
                R_SeqSt = ROTOR_DISABLE_ALL;
            }
            else if (!R_RunCmd)
            {
                R_SeqSt = ROTOR_DISABLE_ALL;
            }
            #ifdef LEG_TEST
            else if ( PARM(TestMode) == LF_LEG_TEST )
            {
                R_InnrRegEnb = true;
                if ( R_StatLegTstLfT2 )
                {
                    R_SeqSt = ROTOR_RUNNING;
                }
            }
            #endif
            else // not Brg2TripFlt and R_RunCmd
            {
                // Request to synchronize
                R_SeqReq.R_SyncReq = true;

                //  Transistion state once synchronized
                if (R_SeqStat.R_SyncStat)
                {
                    R_TrqDlyTmr = 0.0;
                    R_SeqSt = ROTOR_CLOSE_SYNC_SW;
                    R_TorTrqMgPk = 0.0;//Zero on starting
                }
            }
            break;

        //-----------------------  ROTOR_CLOSE_SYNC_SW -------------------------
        case ROTOR_CLOSE_SYNC_SW:


            if (Brg2TripFlt)
            {
                R_SeqReq.R_SCEnbReq = false;
                R_SeqSt = ROTOR_DISABLE_ALL;
            }
            else if (!R_RunCmd)
            {
               R_SeqSt = ROTOR_OPEN_SYNC_SW;
               R_SyncReqTmr = 0.0;
            }
            else // not Brg2TripFlt and R_RunCmd
            {
                // Request to close synchronizing contactor
                R_SeqReq.R_SCEnbReq = true;

                // Enable synchronization IOC fault
                R_SyncIOCEnb = true;

                //  Transistion state after delay - S_CTFlt checking is also performed during this delay
                if ( R_SeqStat.R_SCEnbStat )
                {
                    R_SeqSt = ROTOR_ENABLE_TORQUE;
                    R_TrqDlyTmr = 0.0;
                }
            }
            break;

        //--------------------------  ROTOR_ENABLE_TORQUE  ---------------------
        case ROTOR_ENABLE_TORQUE:
            if (Brg2TripFlt)
            {
                R_SeqSt = ROTOR_DISABLE_ALL;
            }
            else if (!R_RunCmd) R_SeqSt = ROTOR_DISABLE_TORQUE;
            else // not Brg2TripFlt and R_RunCmd

            {   // Request to enable torque.  Allow V & I to return to normal during delay.
                R_TrqDlyTmr += DelTm2;
                if ((R_TrqDlyTmr > PARM(R_TrqEnDlyTm)) || ( ZVRTIp && (R_TrqDlyTmr > PARM(R_TrqDlyZVTm)) )
                    || S_VMaxHVRT  ) R_SeqReq.R_TrqEnbReq = TRUE;

                // Disable synchronization IOC fault
                R_SyncIOCEnb = false;

                //  Transition state once Torque is enabled
                if (R_SeqStat.R_TrqEnbStat) R_SeqSt = ROTOR_RUNNING;
            }
            break;

        //-----------------------------  ROTOR_RUNNING  ------------------------
        case ROTOR_RUNNING:
            R_Running = true;
            if (Brg2TripFlt)
            {
                R_SeqSt    = ROTOR_DISABLE_ALL;
            }
            else if (!R_RunCmd || R_ZVGtOff)
            {
                R_SeqSt = ROTOR_DISABLE_TORQUE;
            }

            #ifdef LEG_TEST
            if ( (R_SeqSt != ROTOR_RUNNING) && (PARM(TestMode) == LF_LEG_TEST) )
            {
                R_InnrRegEnb = false;
            }
            #endif

            break;

        //-------------------------  ROTOR_DISABLE_TORQUE  ---------------------
        case ROTOR_DISABLE_TORQUE:
            // If Brg2TripFlt, then shutdown bridge, inner regulators before
            // clamping torque reference to zero.
            if (Brg2TripFlt)
            {
                R_SeqSt = ROTOR_DISABLE_ALL;
            }
            else    // No Brg2TripFlt
            {
                // First disable torque
                R_SeqReq.R_TrqEnbReq = false;

                // Goto open sync brkr state once flux and torque are disabled
                if (!R_SeqStat.R_TrqEnbStat)
                {
                    if (!R_RunCmd || !R_ZVGtOff)
                    {
                        R_SeqSt = ROTOR_OPEN_SYNC_SW;
                        R_SyncReqTmr = 0.0;
                    }
                    else
                    {
                        R_SeqSt = ROTOR_ZVRT_STANDBY;
                    }
                }
            }
            break;

        //-------------------------  ROTOR_ZVRT_STANDBY ------------------------
        case ROTOR_ZVRT_STANDBY:
            // If Brg2TripFlt, then shutdown bridge, inner regulators before
            // clamping torque reference to zero.
            if (Brg2TripFlt || !R_RunCmd)
            {
                R_SeqSt = ROTOR_DISABLE_ALL;
            }
            else    // No Brg2TripFlt
            {
                if ( R_ZVGtOff)
                {
                    R_BrgPwrEnb              = false;    // Disable bridge, inner regs
                    R_InnrRegEnb             = false;
                    R_MctlCmd.R_FlxCmdOn     = false;
                }
                else
                {
                    R_BrgPwrEnb              = true;     // Enable bridge, inner regs
                    R_InnrRegEnb             = true;
                    R_MctlCmd.R_FlxCmdOn     = true;
                    R_SeqSt                  = ROTOR_ENABLE_TORQUE;
                    R_TrqDlyTmr              = 0.0;
                }
            }
            break;

        //-------------------------  ROTOR_OPEN_SYNC_SW  -----------------------
        case ROTOR_OPEN_SYNC_SW:

            if (Brg2TripFlt)
            {
                R_SeqSt = ROTOR_DISABLE_ALL;
            }
            else    // No Brg2TripFlt
            {
                R_SeqReq.R_SCEnbReq  = false;   // Command sync contactor (SC) to open

                R_SyncReqTmr += DelTm2;
                if ( R_SyncReqTmr > PARM(S_K1OpnDlyTm ))
                {
                    R_SeqReq.R_SyncReq   = false;       // disable gating
                }

                // Goto Disable All  state once sync contactor is de-energized
                if ( !R_SeqStat.R_SCEnbStat && !R_SeqAct.R_SCEnbAct )
                {
                    R_SyncReqTmr = 0.0;     // Reset delay timer for next time
                    R_SeqSt = ROTOR_DISABLE_ALL;
                }
            }
            break;

        //-------------------------  ROTOR_DISABLE_ALL  ------------------------
        case ROTOR_DISABLE_ALL:

            R_SeqReq.R_TrqEnbReq = false;       // set Torque cmd to zero
            R_SeqReq.R_SCEnbReq  = false;       // open synch contactor
            R_SeqReq.R_SyncReq   = false;       // disable gating
            R_SeqReq.R_CelTstReq = false;       // cancel auto-cell test

            if (FAULTISACTIVE(R_CrowBarFlt) && R_CroBarActT2)
            {
                R_SeqReq.R_CCEnbReq  = false;  // de-energize crowbar contactor without delay
            }
            else
            {
                if ( !R_SeqStat.R_SCEnbStat && !R_SeqAct.R_SCEnbAct )
                {
                    R_CCDlyTmr += DelTm2;
                    if (FAULTISACTIVE(S_K1ClsTmout))   // R_SCEnbStat never changed from False state
                    {
                        if (R_CCDlyTmr > (PARM(R_CCDisDlyTm) + PARM(S_K1OpnDlyTm)))  // extra wait to allow SC to open
                        {
                            R_SeqReq.R_CCEnbReq  = false;     // de-energize crowbar contactor
                        }
                    }
                    else
                    {
                        if (R_CCDlyTmr > PARM(R_CCDisDlyTm))
                        {
                            R_SeqReq.R_CCEnbReq  = false;     // de-energize crowbar contactor
                        }
                    }
                }
            }

            // Goto R_Stopped state once crowbar contactor de-energized
            if ( !R_SeqStat.R_CCEnbStat || FAULTISACTIVE(R_CCClsTmout))
            {
                // Reset delay timers for next time
                R_CCDlyTmr = 0.0;
                R_SeqSt = ROTOR_STOPPED;
            }
            break;

        //---------------------------- ROTOR_DIAGNOSTIC ------------------------
        case ROTOR_DIAGNOSTIC:    // Execution of Diagnostic commands
            //  Go to R_Stopped state if TripFlt or removal of Pulse/Cell test command
            if ( TripFlt || (!R_DiagCmd) )
            {
                R_SeqSt = ROTOR_DISABLE_ALL;
            }
            else if ( R_PlsTstCmd )
            {
                //if ( PARM(TestMode) == PUMP_BACK )
                //{
                    R_SeqReq.R_CCEnbReq = true;
                    if ( R_SeqStat.R_CCEnbStat && !R_SeqAct.R_CCEnbAct )
                    {
                        R_SeqSt = ROTOR_PULSE_TEST;
                    }
                //}
            }

            else if ( R_CelTstCmd )
            {
                 R_SeqSt = ROTOR_CELL_TEST;
             }
            else if ( R_MctlTunCmd )
            {
                R_SeqReq.R_CCEnbReq = true;  // Request contactor open

                //  Go to next state when contactor is picked up and
                //  contactor task has completed
                if ( R_SeqStat.R_CCEnbStat && !R_SeqAct.R_CCEnbAct)
                {
                    R_SeqSt = ROTOR_MTR_CTL_TUNE;
                }
            }
            break;

        //--------------------------- ROTOR_PULSE_TEST -------------------------
        case ROTOR_PULSE_TEST:
            // Disable pulse test if complete, aborted or fault occurs
            if (Brg2TripFlt || R_SeqStat.R_PlsTstStat || !R_PlsTstCmd)
            {
                R_SeqReq.R_PlsTstReq = false;
                R_PlsTstCmd          = false;
                if (!R_SeqStat.R_PlsTstStat && !R_SeqAct.R_PlsTstAct)
                {
                    R_SeqSt = ROTOR_DIAGNOSTIC;
                }
            }
            else
            {
                R_SeqReq.R_PlsTstReq = true;  // Set pulse test request
            }
            break;

        //---------------------------- ROTOR_CELL_TEST -------------------------
        case ROTOR_CELL_TEST:
            // Disable cell test if complete, aborted or fault occurs
            if (Brg2TripFlt || R_SeqStat.R_CelTstStat || !R_CelTstCmd)
            {
                R_SeqReq.R_CelTstReq = false;
                R_CelTstCmd         = false;
                if (!R_SeqStat.R_CelTstStat && !R_SeqAct.R_CelTstAct)
                {
                    R_SeqSt = ROTOR_DIAGNOSTIC;
                }
            }
            else    // not Trip fault and Cell test command
            {
                R_SeqReq.R_CelTstReq = true;  // Set cell test request
            }
            break;


        //------------------------ ROTOR_MTR_CTL_TUNE --------------------------
        case ROTOR_MTR_CTL_TUNE:
            // Disable motor control tune if complete, aborted or fault occurs
            if (Brg2TripFlt || R_SeqStat.R_MctlTunSta || !R_MctlTunCmd)
            {
                R_SeqReq.R_MctlTunReq = false;
                R_MctlTunCmd         = false;
                if (!R_SeqStat.R_MctlTunSta && !R_SeqAct.R_MctlTunAct)
                {
                    R_SeqSt = ROTOR_DIAGNOSTIC;
                }
            }
            else    // not Trip fault and Motor Control tune command
            {
                R_SeqReq.R_MctlTunReq = true;  // Set motor control tune request
            }
            break;

        //------------------------------  ROTOR_INIT  --------------------------
        case ROTOR_INIT:  // Drive Initialization - wait for Task interface ready

            if (R_RunReq || R_DiagCmd)
            {
                PUSH_DIAG(R_RunWhlInit); // Call warning fault
            }

            // Goto ROTOR_STOPPED state if Task Interface is ready
            if (InitDone)
            {
                R_SeqSt = ROTOR_STOPPED;
                R_InitDone = true;
            }
            break;

        default:    // Invalid Sequencer State
            PUSH_DIAG(BadSeqSt);              // need to make this R_BadSeqSt?
            break;

    }   // End of switch(R_SeqSt)

    // Publish rotor sequencer torque-enable request
    R_TrqEnbReq = R_SeqReq.R_TrqEnbReq;

    // Form auto-cell-test type
    if (  R_AutoCTAct )
    {
        if ( R_SeqReq.R_CCEnbReq
                )
        {
            R_AutoCelTst = cCellTest::AUTOCT_TO_FAIL;
        }
        else
        {
            R_AutoCelTst = cCellTest::AUTOCT_TO_PASS;
        }
    }
    else
    {
        R_AutoCelTst = cCellTest::AUTOCT_OFF;
    }

    R_SeqCelTstReq = R_SeqReq.R_CelTstReq ? true : false;   // Move request single bit to a boolean

    return;

}   // CoreDriveSequencer

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rotor core drive-sequencer initialization.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorInitCoreDriveSequencer(void)
{
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rotor control sequencer initialization.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorInitSequencer()
{
    float Temp2;

    RotorInitCoreDriveSequencer();

    R_InitDone      = false;    // Set once Task 3 has completed.

    R_SeqStatT1_PackedBits = 0; // Init Task 1 R_SeqStat

    // Calc time to delay in TI_INIT state for I offset null

    //Temp2 = NoNANLim( 5.0F/PARM(R_WIOffsFst), 1.0e6F, -1.0e6F );
    //Temp2 = NoNANLim( 5.0F/R_RotorBrgFbkOffs.PARM(WIOffsFst), 1.0e6F, -1.0e6F );
    Temp2 = NoNANLim( 5.0F/R_RotorBrgFbkOffs.WIOffsFast, 1.0e6F, -1.0e6F );


    // If R_WIOffsFst is too small, clamp value of R_IoffInitTm
    if ( Temp2 > 2.0F )
    {
        R_IoffInitTm = 2.0F;
    }
    else
    {
        R_IoffInitTm = Temp2;
    }

    return;
}   // InitSequencer
