///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rotor side sequencing
//
// DESCRIPTION:
//      Sequencer task interface module.  Contains Sequencer task interface
//      and sequencer tasks.
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
// product
#include "Product.h"
#include "a0SeqTsk.h"
#include "p3Fdbk.h"
#include "b0BridgeRebf.h"
#include "b0BrgFbkOffs.h"
#include "b0DbCelTst.h"


// Externals
//----------
extern float                DelTm2;
extern float                GmPllWT2;
extern float                GmSVMtchErSq;
extern float                GmVMagT2;
extern float                S_K1OpTotDly;
extern float                R_IaT2;
extern float                R_IbT2;
extern float                R_IcT2;
extern float                R_IoffInitTm;
extern float                R_IyFbkT2;
extern float                R_SyncAngErr;
extern float                R_VabT2;
extern float                R_VyFbkT2;
extern float                R_WRtrT2;
extern float                S_FlxVMag;
extern float                S_IMag;
extern float                S_IMagK1Stck;
extern float                S_PllWT2;
extern float                S_VMagT2;
extern float                S_VregErr;
extern float                R_VpeakSqr;
extern float                R2_VpeakSqr;
extern float                R_VdcFbkT2;

extern float                S_VabT2;
extern float                S_VbcT2;
extern float                S_VcaT2;
extern float                XfmrRatioLnStrRcp;
extern float                SyncTstVsScl;
extern float                L_BtmTrPwrLossT2;
extern float                L_BtmDpwrLossT2;
extern float                L_PwrLine;

extern unsigned             Brg2TripFlt;
extern unsigned             InitDone;
extern unsigned             OpnGrdAct;
extern unsigned             OpnPhsAct;
extern unsigned             MvSwGearTrpReq;
extern unsigned             IslK1OpnReq;
extern unsigned             R_AngRotStat;
extern unsigned             R_CroBarFltT2;
extern unsigned             S_LineFltEnb;
extern unsigned             S_PllAllowCl;
extern unsigned             S_PllEnb;
extern unsigned             S_PllProven;
extern unsigned             S_XmAdptEnb;
extern unsigned             SimMode;
extern unsigned             T3CalcDone;
extern unsigned             S_IocLvlFlg;
extern unsigned             S_IocFltInh;
extern unsigned             Q1OpnReq;;
extern unsigned             R_TachLsDoneT2;

extern unsigned             R_SeqStatT1_PackedBits;
extern SysTm_Typ            SysTm2;
extern FaultIsActiveTyp     FaultIsActive;

extern cBridgeRebf          R_BridgeRebf;
extern cRotorBrgFbkOffs     R_RotorBrgFbkOffs;

extern void RotorCoreDriveSequencer(void);
extern void Q1BreakerTask(void);
extern float            S_K1VacrossTol;
extern float            S_K1VerrTime;

// Constants
//----------

// Task Interface states
#define TI_PARK              -20
#define TI_INIT              -10
#define TI_IDLE               0
#define TI_STARTING           10
#define TI_RUNNING            20
#define TI_PULSE_TEST         40
#define TI_CELL_TEST          50
#define TI_MCTL_TUNE          60
// For DFIG2.3, keep DB definition in rotor, but not use.
#define TI_DB_PULSE_TEST      70
#define TI_DB_CELL_TEST       80


// Contactor Task states
#define CC_IDLE             0
#define CC_ENB_1            1
#define CC_ENB_2            2
#define CC_ENB_3            3
#define CC_ENB_4            4
#define CC_ENB_5            5
#define CC_DSB_1            -1
#define CC_DSB_2            -2
#define CC_DSB_3            -3

// Sync Contactor Task states
#define SC_IDLE             0
#define SC_ENB_1            1
#define SC_ENB_2            2
#define SC_ENB_3            3
#define SC_ENB_4            4
#define SC_DSB_1            -1
#define SC_DSB_2            -2
#define SC_DSB_3            -3
// Sync Task states
#define SYNC_IDLE           0
#define SYNC_ENB_1          1
#define SYNC_ENB_2          2
#define SYNC_ENB_3          3
#define SYNC_ENB_4          4
#define SYNC_ENB_5          5
#define SYNC_ENB_6          6
#define SYNC_ENB_7          7
#define SYNC_ENB_8          8
#define SYNC_ENB_9          9
#define SYNC_ENB_10         10
#define SYNC_ENB_11         11
#define SYNC_ENB_12         12
#define SYNC_ENB_13         13
#define SYNC_ENB_14         14
#define SYNC_ENB_15         15
#define SYNC_ENB_16         16
#define SYNC_ENB_17         17
#define SYNC_DSB_1          -1
#define SYNC_DSB_2          -2

// DB Test Task states
#define DBTST_IDLE           0
#define DBTST_ENB_1          1
#define DBTST_ENB_2          2
#define DBTST_ENB_3          3
#define DBTST_ENB_4          4
#define DBTST_ENB_5          5
#define DBTST_DSB_1          -1
#define DBTST_DSB_2          -2



// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(S_K1DropOutDlyTm,   float);     // Time to delay dropping out K1 (sec)
CREATE_PARM(S_K1VmagThrs,       float);     // Voltage threshold for unexpected stator voltage
CREATE_PARM(R_WStrtVI,          float);     // Filter W for rotor starting V & I values
CREATE_PARM(R_RrFltThr,         float);     // Rotor resistance fault threshold
CREATE_PARM(R_CCEcnTm,          float);     // CC economizer delay time (sec);
CREATE_PARM(R_CCDlyTm,          float);     // CC Pick-up/Drop-out delay time (sec);
CREATE_PARM(R_CCvoltThrs,       float);     // Max voltage threshold for dropping crowbar contactor (Volts)
CREATE_PARM(R_CCvoltDecayTm,    float);     // Max time given to rotor voltage to decay below P.R_CCvoltThrs (sec)
CREATE_PARM(R_CCFbkEnb,         unsigned);  // Enable bridge 1 Crowbar Contactor status for use
CREATE_PARM(S_FreqMtchFrzTm,    float);     // Time delay for frequency match during synchronization when angle is frozen
CREATE_PARM(S_VoltMtchFrzTm,    float);     // Time delay for voltage match during synchronization when angle is frozen
CREATE_PARM(S_FreqMtchTm,       float);     // Time delay for frequency match during synchronization
CREATE_PARM(S_K1OpnCmdDlyTm,    float);     // Time delay to execute K1 open command
CREATE_PARM(R_OffNullDly,       float);     // Delay(sec); before running V/I null routines
CREATE_PARM(R_PrefluxTm,        float);     // Preflux delay time (sec);
// new sequencing stuff
CREATE_PARM(S_WFrqDif,          float);
CREATE_PARM(S_WVltErr,          float);
CREATE_PARM(S_WPhsErr,          float);
CREATE_PARM(S_PllSetlTm,        float);
CREATE_PARM(S_PllTacTol,        float);
CREATE_PARM(S_VfbMinLvl,        float);
CREATE_PARM(S_VMagTol,          float);
CREATE_PARM(S_SyncFrqTol,       float);
CREATE_PARM(S_SyncTm,           float);
CREATE_PARM(S_VSyncTol,         float);
CREATE_PARM(S_PhsSyncTol,       float);
CREATE_PARM(S_SyncTmOut,        float);
CREATE_PARM(S_SetlTm1,          float);
CREATE_PARM(S_SetlTm2,          float);
CREATE_PARM(S_SyncDly,          float);
CREATE_PARM(S_PllHotDly,        float);
CREATE_PARM(S_IMagK1StkW,       float);
CREATE_PARM(S_K1ClsDlyTm,       float);     // K1 contactor close delay time (sec);
CREATE_PARM(S_K1OpnDlyTm,       float);     // K1 contactor open delay time (sec);
CREATE_PARM(S_K1VlThrs,         float);     // stator voltage threshold for K1 cont check
CREATE_PARM(S_K1ImagThrs,       float);     // Stator current threshold for K1 cont check
CREATE_PARM(S_K1VmagErrTm,      float);     // Minimum time to declare voltage error across K1 acceptable
CREATE_PARM(R_CTChkTm,          float);     // CT check time
CREATE_PARM(R_TrqEnDlyTm,       float);     // Delay time between sync contactor close & torq enable
CREATE_PARM(R_TrqDlyZVTm,       float);     // Delay time between flux enable and & torq enable in during ZV event
CREATE_PARM(R_CCDisDlyTm,       float);     // Delay between shutdown & drop out crowbar

// Diagnostics
//-----------
CREATE_DIAG(R_BadCCTsk);              // Seq: Bad call to rotor 1 Crowbar Contactor task
CREATE_DIAG(R_CCFbkOpen);             // Seq: Rotor 1 Crowbar Contactor feedback open
CREATE_DIAG(R_CCOpnTmout);            // Seq: Rotor 1 Crowbar contactor did not open on time
CREATE_DIAG(R_CCClsTmout);            // Seq: Rotor 1 Crowbar Contactor did not close on time
CREATE_DIAG(R_CCweldAlm);             // Seq: Rotor 1 Crowbar Contactor is potentially welding
CREATE_DIAG(S_K1StuckClose);             // K1 closed incorrectly based on voltage and current
CREATE_DIAG(R_BadSCTsk);                 // Seq: Bad call to Sync Contactor task
CREATE_DIAG(S_K1ClsTmout);               // Seq: Sync contactor did not close on time
CREATE_DIAG(S_K1OpnTmout);               // Sync contactor did not open on time
CREATE_DIAG(R_BadSyncTsk);               // Seq: Bad call to Sync task
CREATE_DIAG(S_PllRotFlt);                // Stator PLL detected rotation fault
CREATE_DIAG(R_RtrConFlt);                // Rotor is incorrectly opened
CREATE_DIAG(S_VfbAbsent);                // Stator feedback is absent
CREATE_DIAG(S_RevPhsRot);                // Reverse phase sequence detected on stator wiring
CREATE_DIAG(S_PllTacFlt);                // Mismatch between tach & stator PLL frequencies detected
CREATE_DIAG(S_VMagFlt);                  // Stator voltage magnitude does not match command
CREATE_DIAG(S_VltSyncFlt);               // Stator voltage magnitude does not synch
CREATE_DIAG(S_PhsSyncFlt);               // Stator voltage phase does not synch
CREATE_DIAG(S_FrqLinFlt);                // Mismatch between line & stator PLL frequencies detected
CREATE_DIAG(R_RevPhsRot);                // Reverse phase sequence detected on rotor wiring
CREATE_DIAG(GenOnLine);                  // Generator On-Line to grid
CREATE_DIAG(R_RunWhlInit);               // Trying to run while initialization is in progress
CREATE_DIAG(R_VOffsInvld);               // Drive started without valid rotor voltage offset

// Variables
//----------
CREATE_PUBVAR(S_K1ClsCmd,   unsigned);  // Syn Contactor Close Command
CREATE_PUBVAR(S_K1Status,   unsigned);  // Sync contactor status
CREATE_PUBVAR(S_K1VmgErrPrm,unsigned)   = false;
CREATE_PUBVAR(S_K1OpnNowReq,unsigned)   = false;    // Open K1 immediately request
CREATE_PUBVAR(S_K1VmgErrTmr,float);

CREATE_PUBVAR(R_BrgPwrEnb   , unsigned) = false;        // Bridge enable boolean
CREATE_PUBVAR(R_Stopped     , unsigned);                // Rotor sequencer stop state
CREATE_PUBVAR(R_StoppedT3   , unsigned);                // Rotor sequencer stop state in task3
CREATE_PUBVAR(R_CCOpnCmd , unsigned) = false;        // Rotor 1 Crowbar Contactor open command
CREATE_PUBVAR(R_CCOpnEcn , unsigned);                // Rotor 1 economizer picks up for a short time as CC is energized
CREATE_PUBVAR(R_CCOpnFbk , unsigned) = false;        // Rotor 1 Crowbar Contactor open feedback
CREATE_PUBVAR(R_CCEnbStat, unsigned);                // Rotor 1 Crowbar Contactor-task enable status
CREATE_PUBVAR(R_CTChkEnb    , unsigned);                // Enable stator CT checking
CREATE_PUBVAR(R_InitDone    , unsigned) = false;        // Flag that shows that rotor initialization is done
CREATE_PUBVAR(R_InnrRegEnb  , unsigned) = false;        // Inner regulator enable boolean
CREATE_PUBVAR(R_ParkReq     , unsigned) = false;        // Rotor request for Task Interface to go into parked state
CREATE_PUBVAR(R_SpdFbkVld   , unsigned) = false;        // Speed feedback valid signal
CREATE_PUBVAR(R_CelTstDisab , unsigned);                // Disable cell test operation

CREATE_PUBVAR(R_CCTskSt  , int) = CC_IDLE;           // Rotor 1 Crowbar Contactor task state
CREATE_PUBVAR(R_SCTskSt     , int) = SC_IDLE;           // Sync Contactor task state
CREATE_PUBVAR(R_SyncTskSt   , int) = SYNC_IDLE;
CREATE_PUBVAR(R_TskInfcSt   , int) = TI_INIT;           // Task Interface state
CREATE_PUBVAR(R_SCEnbStat   , unsigned);                // Sync Contactor sequencer-task enable status

CREATE_PUBVAR(R_TskInfcRdy  , int) = false;             // Task Interface Ready boolean


CREATE_PUBVAR(R_CCEcnTmr , float);
CREATE_PUBVAR(R_CCvoltThrsSqr,float);                   // Squared volt threshold across crowbar contactor (Volt^2)

CREATE_PUBVAR(R_IaStart     , float);
CREATE_PUBVAR(R_IbStart     , float);
CREATE_PUBVAR(R_IcStart     , float);
CREATE_PUBVAR(R_IyStart     , float);


CREATE_PUBVAR(R_SynAngErFl  , float);                   // filtered version of R_SyncAngErr
CREATE_PUBVAR(R_SyncTskTm   , float);
CREATE_PUBVAR(R_TIInitTmr   , float);                   // Timer for Init state of Task interface
CREATE_PUBVAR(R_VabStart    , float);
CREATE_PUBVAR(R_VoffNulDly  , float);                   // Voltage offset null delay
CREATE_PUBVAR(R_VyStart     , float);

CREATE_PUBVAR(R_SyncStat    , unsigned);                // synchronizing sequencer task status
CREATE_PUBVAR(R_TrqEnbStat  , unsigned);                // torque-enable status in task2
CREATE_PUBVAR(S_PhsSync     , unsigned);
CREATE_PUBVAR(S_SimK1Clsd   , unsigned) = false;        // Simulated Sync Contactor state for generator model
CREATE_PUBVAR(S_SyncCmd     , unsigned) = false;        // Sequencer synchronization control bit
CREATE_PUBVAR(S_SyncCmdT3   , unsigned);                // Sequencer sync control bit in task3

CREATE_PUBVAR(S_VltSync     , unsigned);

CREATE_PUBVAR(S_FrqErrRevR  , float);                   // Stator freq - predicted value if rtr reversed
CREATE_PUBVAR(S_FrqLinDif   , float);                   // Difference between stator freq and line
CREATE_PUBVAR(S_FrqTacDif   , float);                   // Difference between stator freq and tach

CREATE_PUBVAR(S_PhsSyncTmr  , float);
CREATE_PUBVAR(S_VltSyncTmr  , float);

CREATE_PUBVAR(S_VMagErr     , float);                   // Error between xfmr volts and stator volts
CREATE_PUBVAR(S_VMagStart   , float);

CREATE_PUBVAR(S_VRegErrFil  , float);                   // Error between expected voltage and actual

CREATE_PUBVAR(SyncConTypSt  , unsigned);
CREATE_PUBVAR(S_K1StatusZ1  , unsigned);                // previous state Sync breaker is closed


CREATE_PUBVAR(OpnDlyTime,    float);                   // Clamp contactor pick-up/dropout delay time
CREATE_PUBVAR(EcnDlyTime,    float);                   // Clamp economizing contactor pick-up/dropout delay time

CREATE_PUBVAR(S_K1ImagThrs,  float);

// Unpublished Variables
//----------------------
unsigned        Brg2TripFltZ1;

unsigned        R_SeqStBits_PackedBits;                      // Sequencer State bits

R_SeqActBits    R_SeqAct;                               // Sequencer Task Active bits
unsigned        R_SeqReq_PackedBits;                      // Sequencer Task Request bits
unsigned        R_SeqStat_PackedBits;                      // Sequencer Task Status bits

unsigned        R_MctlCmd_PackedBits;                      // Motor control command bits

SysTm_Typ       R_CCTskTmr;                          // Rotor 1 Crowbar Contactor Task timer
SysTm_Typ       R_SCTskTmr;
SysTm_Typ       R_SyncTskTmr;                           // Contactor Task timer
SysTm_Typ       R_TIIdleTmr;                            // Holds msec timestamp for Idle state in Task interface


// Local Prototypes (to resolve forward references)
//-------------------------------------------------
void RotorDbTestTask();
void RotorContactorATask();
void RotorSequencerTaskInterface();
void RotorSyncContactorTask();
void RotorSyncTask();


// Data Passing
//-------------
DATA_PASS(R_Stopped, R_StoppedT3, T2_T3, UNFILTERED);
DATA_PASS(S_SyncCmd, S_SyncCmdT3, T2_T3, UNFILTERED);


///////////////////////////////////////////////////////////////////////////////
//
// TITLE        High level rotor control sequencer
//
// DESCRIPTION
//      Perform main sequencer execution loop.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//
///////////////////////////////////////////////////////////////////////////////
void RotorMotorCtrlSequencer(void)
{

    //RotorCoreDriveSequencerT2();
    RotorCoreDriveSequencer();
    RotorSequencerTaskInterface();   // Must run before any task is executed

    //========= Add tasks here ===========


    RotorContactorATask();

    RotorSyncContactorTask();

    RotorSyncTask();

    // Delayed bridge 2 fault trip to be used on avoiding bad task faults
    Brg2TripFltZ1 = Brg2TripFlt;

    // generator online/offline to grid: rising edge detected
    if (!S_K1StatusZ1 && S_K1Status)
    {
        PUSH_DIAG(GenOnLine);
    }

    S_K1StatusZ1 = S_K1Status;

    return;


}



///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Rotor sequencer task interface
//
// DESCRIPTION
//     Interprets request and status information from the Core drive sequencer
//     and sequencer-tasks to determine an internal sequencer state that is
//     independent of the core drive sequencer. Execution is as follows:
//     1) Perform any overhead tasks.
//     2) Collect all Stat and Act bits from tasks.
//     3) Calculate Task Interface state transition, perform any
//        action that only occurs on a transition.
//     4) Perform actions that occur in present state and calculate R_SeqStBits.
//     5) Present updated Req, Stat, Act, R_SeqStBits to Tasks & Sequencer.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////
void RotorSequencerTaskInterface(void)
{
    int *Temp1, *Temp2;
    int Last_Tsk_St;

    // ======= Collect Stat, Act bits from tasks. ========
    // For Task 2, the collection is based on order of function execution

    // Task1 sequencer task status/active information must be updated here
    if (R_SeqStatT1.R_TrqEnbStat)
    {
        R_SeqStat.R_TrqEnbStat = true;
    }
    else
    {
        R_SeqStat.R_TrqEnbStat = false;
    }

    // ======= Calculate Task Interface state transition ========
    // Save last R_TskInfcSt
    Last_Tsk_St = R_TskInfcSt;

    switch (R_TskInfcSt)
    {
        case TI_PARK:
            // Cannot exit this state except by hard reset
            break;

        case TI_INIT:
            Temp1 = (int*) &R_SeqReq_PackedBits;
            Temp2 = (int*) &R_SeqStat_PackedBits;
            if (!InitDone && ((*Temp1 != 0) || (*Temp2 != 0)))
            {
                PUSH_DIAG(R_RunWhlInit);
            }

            // Perform current offset null during initialization
            R_MctlCmd.R_IoffNullEn = true;
            R_TIInitTmr += DelTm2;

            // Exit TI_Init after current offsets have run for 5 current offset taus
            if ( (R_TIInitTmr > R_IoffInitTm) && InitDone )
            {
                R_MctlCmd.R_IoffNullEn = false;  // Disable I offset null
            // Use slow I offs omega in normal operation
                R_MctlCmd.R_IoffSlowSw = true;
                R_TskInfcRdy    = true;     // Task interface is ready
                R_TskInfcSt     = TI_IDLE;  // Goto TI_IDLE state when INIT done
            }

            break;

        case TI_IDLE:
            // Note: Disable Current/Voltage Offset Null when leaving TI_IDLE
            if ( R_SeqReq.R_CCEnbReq )
            {
                R_MctlCmd.R_IoffNullEn = false;
                R_MctlCmd.R_VoffNullEn = false;

                // If drive starting with not R_VOffsVld, annunciate alarm
                if (!R_RotorBrgFbkOffs.VOffsVld && !SimMode )
                {
                    PUSH_DIAG(R_VOffsInvld);
                }
                R_TskInfcSt = TI_STARTING;
            }
            else if ( !R_BrgPwrEnb )
            {
                if ( R_ParkReq )
                {
                    R_TskInfcSt = TI_PARK;
                    R_MctlCmd.R_IoffNullEn = false;
                    R_MctlCmd.R_VoffNullEn = false;
                }
                else if (R_SeqReq.R_CelTstReq)
                {
                    R_TskInfcSt = TI_CELL_TEST;
                    R_MctlCmd.R_IoffNullEn = false;
                    R_MctlCmd.R_VoffNullEn = false;
                }
                else if (R_SeqReq.R_PlsTstReq)
                {
                    R_TskInfcSt = TI_PULSE_TEST;
                    R_MctlCmd.R_IoffNullEn = false;
                    R_MctlCmd.R_VoffNullEn = false;
                }
                else if (R_SeqReq.R_DBPsTstReq)
                {
                    R_TskInfcSt = TI_DB_PULSE_TEST;
                    R_MctlCmd.R_IoffNullEn = false;
                    R_MctlCmd.R_VoffNullEn = false;
                }
                else if (R_SeqReq.R_DBClTstReq)
                {
                    R_TskInfcSt = TI_DB_CELL_TEST;
                    R_MctlCmd.R_IoffNullEn = false;
                    R_MctlCmd.R_VoffNullEn = false;
                }
                else if (R_SeqReq.R_MctlTunReq)
                {
                    R_TskInfcSt = TI_MCTL_TUNE;
                    R_MctlCmd.R_IoffNullEn = false;
                    R_MctlCmd.R_VoffNullEn = false;
                }
            }

            break;

        case TI_STARTING:
            if ( !R_SeqReq.R_CCEnbReq && !R_SeqStat.R_CCEnbStat )
            {
                R_TskInfcSt = TI_IDLE;
            }
            else if ( R_SeqReq.R_TrqEnbReq && R_SeqStat.R_TrqEnbStat )
            {
                R_TskInfcSt = TI_RUNNING;
            }
            else if ( R_SeqReq.R_CelTstReq )
            {
                R_TskInfcSt = TI_CELL_TEST;
            }
            else if ( R_SeqReq.R_PlsTstReq )
            {
                R_TskInfcSt = TI_PULSE_TEST;
            }
            else if ( R_SeqReq.R_DBClTstReq )
            {
                R_TskInfcSt = TI_DB_CELL_TEST;
            }
            else if ( R_SeqReq.R_DBPsTstReq )
            {
                R_TskInfcSt = TI_DB_PULSE_TEST;
            }
            else if ( R_SeqReq.R_MctlTunReq )
            {
                R_TskInfcSt = TI_MCTL_TUNE;
            }
            break;

        case TI_RUNNING:
            if ( !R_SeqReq.R_CCEnbReq && !R_SeqStat.R_CCEnbStat )
            {
                R_TskInfcSt = TI_IDLE;
            }
            break;

        case TI_PULSE_TEST:
            if (   !R_SeqReq.R_PlsTstReq && !R_SeqStat.R_PlsTstStat
                && !R_SeqAct.R_PlsTstAct )
            {
                R_TskInfcSt = TI_IDLE;
            }
            break;

        case TI_CELL_TEST:
            if (   !R_SeqReq.R_CelTstReq && !R_SeqStat.R_CelTstStat
                && !R_SeqAct.R_CelTstAct )
            {
                if ( R_SeqReq.R_CCEnbReq  )
                {
                    R_TskInfcSt = TI_STARTING;
                }
                else
                {
                    R_TskInfcSt = TI_IDLE;
                }
            }
            break;

        case TI_DB_PULSE_TEST:
             if (!R_SeqReq.R_DBPsTstReq && !R_SeqAct.R_DBPsTstAct && !R_SeqStat.R_DBPsTstSta)
             {
                 R_TskInfcSt = TI_IDLE;
             }
            break;

        case TI_DB_CELL_TEST:
            if (!R_SeqReq.R_DBClTstReq && !R_SeqAct.R_DBClTstAct && !R_SeqStat.R_DBClTstSta)
            {
                R_TskInfcSt = TI_IDLE;
            }
            break;


        case TI_MCTL_TUNE:
            if (   !R_SeqReq.R_MctlTunReq && !R_SeqStat.R_MctlTunSta
                && !R_SeqAct.R_MctlTunAct )
            {
                R_TskInfcSt = TI_IDLE;
            }
            break;

        default:
            break;
    }   // switch (R_TskInfcSt)

    // Publish status of torque-enable indication
    R_TrqEnbStat = R_SeqStat.R_TrqEnbStat;

    // ======= Perform in-State actions, update R_SeqStBits ========

    R_SeqStBits_PackedBits = 0;        // Zero R_SeqStBits

    switch (R_TskInfcSt)
    {
        case TI_PARK:   // Park drive for firmware download- hard reset to exit
            R_TskInfcRdy         = false;    // Clear Tsk interface ready flag
            R_SeqStBits.R_Parked = true;     // Flag that drive is parked
            break;

        case TI_INIT:
            break;

        case TI_IDLE:
            R_SeqStBits.R_Stopped = true;

        // Capture time on entering TI_IDLE state for 1st time
            if ( Last_Tsk_St != TI_IDLE )
            {
                R_TIIdleTmr          = SysTm2;   // Reset R_TIIdleTmr
                R_MctlCmd.R_VOffsRdy = false;    // V offset null is not ready
            }

        // Enable current offset null once delay has expired
            if ( DELTA_TM(SysTm2,R_TIIdleTmr) > PARM(R_OffNullDly) )
            {
                R_MctlCmd.R_IoffNullEn = true;
            }

            R_VoffNulDly = PARM(R_OffNullDly); // Voltage offset delay was 8*rotor tau

        // If R_TauRtr has been calculated & R_VoffNulDly has expired, set R_VOffsRdy
            if ( (DELTA_TM(SysTm2,R_TIIdleTmr) > R_VoffNulDly) )
            {
                R_MctlCmd.R_VOffsRdy = true;
            }

        // Enable voltage offset null if voltage offset can be perfomed (R_VOffsRdy)
            if (R_MctlCmd.R_VOffsRdy)
            {
                R_MctlCmd.R_VoffNullEn = true;
            }
            else
            {
                R_MctlCmd.R_VoffNullEn = false;
            }
            break;

        case TI_STARTING:
            break;

        case TI_RUNNING:
            R_SeqStBits.R_Running = true;
            break;

        case TI_PULSE_TEST:
            R_SeqStBits.R_PulseTest  = true;
            R_SeqStBits.R_DiagMode   = true;
            break;

        case TI_CELL_TEST:
            R_SeqStBits.R_CellTest   = true;
            R_SeqStBits.R_DiagMode   = true;
            break;

        case TI_DB_PULSE_TEST:
             R_SeqStBits.R_DBPsTest   = true;
             R_SeqStBits.R_DiagMode   = true;
            break;

        case TI_DB_CELL_TEST:
             R_SeqStBits.R_DBClTest   = true;
             R_SeqStBits.R_DiagMode   = true;
            break;

        case TI_MCTL_TUNE:
            R_SeqStBits.R_MtrCtlTune  = true;
            R_SeqStBits.R_TuneMode    = true;
            break;
        default:
            break;
    }   // switch (R_TskInfcSt)

    /* Command stator offset nulling as per rotor */
    R_MctlCmd.S_IoffNullEn = R_MctlCmd.R_IoffNullEn;
    R_MctlCmd.S_IoffSlowSw = R_MctlCmd.R_IoffSlowSw;
    R_MctlCmd.S_VoffNullEn = R_MctlCmd.R_VoffNullEn;


    // Publish rotor sequencer stop state
    R_Stopped = R_SeqStBits.R_Stopped;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Rotor Crowbar Contactor sequencer task
//
// DESCRIPTION
//      Interface for contactor hardware to pick-up or drop-out output.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////
void RotorContactorATask()
{
    unsigned    R_SeqBitComb;
    float       SeqStElpsTm;


    R_SeqBitComb = (R_SeqReq.R_CCEnbReq) | (R_SeqStat.R_CCEnbStat << 1)
             | (R_SeqAct.R_CCEnbAct << 2);


    //------------- Rotor 1 Crowbar Contactor Sequencer Task -------------------


    switch (R_SeqBitComb)
    {
        case 0:
            R_CCTskSt = CC_IDLE;       // Goto Idle state
            break;

        case 1:
            if (Brg2TripFltZ1)
            {
                PUSH_DIAG(R_BadCCTsk);
                R_CCTskSt = CC_IDLE;
            }
            else    // Not Brg2TripFltZ1
                R_CCTskSt = CC_ENB_1;       // Goto 1st enable state
            break;

        case 2:
            R_CCTskSt = CC_DSB_1;      // Goto 1st disable state
            break;

        case 3:
            if (!SimMode && (PARM(R_CCFbkEnb) && !R_CCOpnFbk))
            {
                if (!R_SeqStBits.R_Stopped)
                {
                    PUSH_DIAG(R_CCFbkOpen);
                }
            }
            R_CCTskSt = CC_IDLE;       // Goto Idle state
            break;

        case 4:
                // Goto 1st disable state if 1st time abort detected
            if (R_CCTskSt >= CC_IDLE)
            {
                R_CCTskSt = CC_DSB_1;
            }
            break;

        case 5:
            if (Brg2TripFltZ1)
            {
                PUSH_DIAG(R_BadCCTsk);
                R_CCTskSt = CC_IDLE;
            }
            else if (R_CCTskSt <= CC_IDLE)
            {
                R_CCTskSt = CC_ENB_1;
            }
            break;

        case 6:
            if (R_CCTskSt >= CC_IDLE)
            {
                R_CCTskSt = CC_DSB_1;
            }
            break;

        case 7:
            if (Brg2TripFltZ1)
            {
                PUSH_DIAG(R_BadCCTsk);
            }
            else    // Not Brg2TripFltZ1
            {
                // Goto 1st enable state if 1st time abort detected
                if (R_CCTskSt <= CC_IDLE)
                {
                    R_CCTskSt = CC_ENB_1;
                }
            }
            break;
    }   // switch (R_SeqBitComb)

    switch (R_CCTskSt)
    {
        case CC_IDLE:     // Idle State
            break;

        case CC_ENB_1:     // Enable states
            R_SeqAct.R_CCEnbAct = true;
            R_CCOpnCmd = true;             // Command contactors to open
            R_CCOpnEcn = true;             // Stop economizing
            R_CCTskTmr = SysTm2;           // Capture msec timestamp
            R_CCEcnTmr = DelTm2;
            R_CCTskSt  = CC_ENB_2;         // Increment state
            break;

        case CC_ENB_2:
            R_CCEcnTmr += DelTm2;
            SeqStElpsTm = DELTA_TM(SysTm2,R_CCTskTmr);
            if (   (PARM(R_CCFbkEnb) && R_CCOpnFbk)
                || ((!PARM(R_CCFbkEnb) || SimMode) && (SeqStElpsTm > OpnDlyTime)))
            {
                R_CCTskSt = CC_ENB_3;      // Goto next Enable state
            }
            else if (PARM(R_CCFbkEnb) && !R_CCOpnFbk && (SeqStElpsTm > OpnDlyTime))
            {
                PUSH_DIAG(R_CCOpnTmout);
                R_CCOpnCmd            = false;      // Remove command from contactors
                R_CCOpnEcn            = false;      // Release economizing resistor
                R_SeqAct.R_CCEnbAct     = false;
                R_SeqStat.R_CCEnbStat   = false;
                R_CCTskSt             = CC_IDLE;    // Goto Idle state
            }
            break;

        case CC_ENB_3:
            R_CCEcnTmr += DelTm2;
            if ( R_CCEcnTmr > EcnDlyTime )
            {
                R_CCOpnEcn = false;                 // Start economizing
                R_CCTskSt = CC_ENB_4;               // Goto next Enable state
            }
            break;

        case CC_ENB_4:
            if (   R_RotorBrgFbkOffs.VOffsVld || !R_MctlCmd.R_VOffsRdy || SimMode
                || R_SeqStBits.R_TuneMode || R_SeqStBits.R_DiagMode)
            {
                R_CCTskTmr = SysTm2;             /* Capture current time */
                R_MctlCmd.R_VoffNullEn = FALSE;  /* Disable voltage offset */
                R_MctlCmd.R_VoffFastSw = FALSE;  /* Use normal voltage offset times */
                R_CCTskSt           = CC_ENB_5;   /* Goto next state */
            }
            /* Voltage offset not valid */
            else R_MctlCmd.R_VoffNullEn = TRUE;   /* Enable V offset null */
            break;


        case CC_ENB_5:
              R_SeqAct.R_CCEnbAct   = false;
              R_SeqStat.R_CCEnbStat = true;
            break;

        case CC_DSB_1:
            R_SeqAct.R_CCEnbAct    = true;
            R_MctlCmd.R_VoffNullEn = FALSE;  /* Disable V offset null */
            R_MctlCmd.R_VoffFastSw = FALSE;  /* Disable fast voltage offset times */
            R_CCOpnCmd           = false;    // Remove command from contactors
            R_CCOpnEcn           = false;    // Release economizing resistor
            R_CCTskTmr           = SysTm2;   // Reset counter
            R_CCTskSt            = CC_DSB_2; // Goto next disable state
            break;

        case CC_DSB_2:
            if ( R_CCOpnCmd == false )
            {
                R_CCTskSt            = CC_DSB_3; // Goto next disable state
            }
            else if ( R_VpeakSqr < R_CCvoltThrsSqr ) // Wait for voltage across contactor to subside
            {
                R_CCOpnCmd           = false;    // Remove command from contactors
                R_CCOpnEcn           = false;    // Release economizing resistor
                R_CCTskTmr           = SysTm2;   // Reset counter
                R_CCTskSt            = CC_DSB_3; // Goto next disable state
            }
            else if ( DELTA_TM(SysTm2,R_CCTskTmr) > PARM(R_CCvoltDecayTm) )  // Continue after some time
            {
                PUSH_DIAG(R_CCweldAlm);          // Alarm potential welding
                R_CCOpnCmd           = false;    // Remove command from contactors
                R_CCOpnEcn           = false;    // Release economizing resistor
                R_CCTskTmr           = SysTm2;   // Reset counter
                R_CCTskSt            = CC_DSB_3; // Goto next disable state
            }
            break;

        case CC_DSB_3:
            SeqStElpsTm = DELTA_TM(SysTm2,R_CCTskTmr);
            if ( (PARM(R_CCFbkEnb) && !R_CCOpnFbk) ||
                 ((!PARM(R_CCFbkEnb) || SimMode) && (SeqStElpsTm > OpnDlyTime)) )
            {
                R_SeqAct.R_CCEnbAct   = false;
                R_SeqStat.R_CCEnbStat = false;
                R_CCTskSt           = CC_IDLE;  // Goto Idle state
            }
            else if (PARM(R_CCFbkEnb) && R_CCOpnFbk && (SeqStElpsTm > OpnDlyTime))
            {
                PUSH_DIAG(R_CCClsTmout);
                FaultIsActive.R_CCClsTmout = true;
                R_SeqAct.R_CCEnbAct   = false;
                R_SeqStat.R_CCEnbStat = true;     // Stat stays true if fbk true
                R_CCTskSt           = CC_IDLE;  // Goto Idle state
            }
            break;
    }   // switch (R_CCTskSt)

    // Publish crowbar contactor-task enable status
    R_CCEnbStat = R_SeqStat.R_CCEnbStat;

    return;
}   // ContactorTask



///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Rotor synchronization contactor sequencer task
//
// DESCRIPTION
//      Interface for synch contactor hardware to pick-up or drop-out output.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////
void RotorSyncContactorTask()
{
   // The following code is designed to cautiously catch the case where the breaker poles won't open, but feedback
   // indicates they are open -- the control is in a state where the breaker should be open and legitimate voltage
   // is seen at the terminals of the stator, and that voltage is not being caused by crow-bar.
   // Once rotor bridge is shutdown, crowbar action is not active, and there is evidence that CB1 may be closed,
   // then times starts counting. CB1 is considered likely to be picked-up if either of CB1ClsFbk or R_CB1UVHld is
   // asserted; this check does not happen if CB1 is believed to be dropped out.
   // If CB1 feedback has been physically jumpered out (remaining active all the time), the logic still runs but
   // independent of CB1 condition.

    float tempTmr;


    SyncConTypSt = (R_SeqReq.R_SCEnbReq) | (R_SeqStat.R_SCEnbStat << 1)
                   | (R_SeqAct.R_SCEnbAct << 2);


    switch (SyncConTypSt)
    {
        case 3:
        case 0:
            R_SCTskSt = SC_IDLE;       // Goto Idle state
           // Make sure K1 remains open
            if (!S_K1ClsCmd && !S_K1Status && (S_IMagK1Stck > PARM(S_K1ImagThrs)))
            {
                if ( !FAULTISACTIVE(S_K1ClsTmout) )
                {
                    PUSH_DIAG(S_K1StuckClose);
                    FaultIsActive.S_K1StuckClose = true;
                    Q1OpnReq = true;
                }
            }
            break;

        case 1:
            if (Brg2TripFltZ1)
            {
                PUSH_DIAG(R_BadSCTsk);
                R_SCTskSt = SC_IDLE;
            }
            else    // Not Brg2TripFltZ1
                R_SCTskSt = SC_ENB_1;       // Goto 1st enable state
            break;

        case 2:
            if ( FAULTISACTIVE(S_K1OpnTmout) )
                R_SCTskSt = SC_IDLE;
            else
                R_SCTskSt = SC_DSB_1;      // Goto 1st disable state
            break;

        case 4:
            // Goto 1st disable state if 1st time abort detected
            if (R_SCTskSt >= 0) R_SCTskSt = SC_DSB_1;
            break;

        case 5:
            if (Brg2TripFltZ1)
            {
                PUSH_DIAG(R_BadSCTsk);
                R_SCTskSt = SC_IDLE;
            }
            else if (R_SCTskSt <= 0) R_SCTskSt = SC_ENB_1;
            break;

        case 6:
            if (R_SCTskSt >= 0) R_SCTskSt = SC_DSB_1;
            break;

        case 7:
            if (Brg2TripFltZ1)
            {
                PUSH_DIAG(R_BadSCTsk);
            }
            else    // Not Brg2TripFltZ1
            {
                // Goto 1st enable state if 1st time abort detected
                if (R_SCTskSt <= 0) R_SCTskSt = SC_ENB_1;
            }
            break;
    }   // switch (SyncConTypSt)

    switch (R_SCTskSt)
    {
        case SC_IDLE:     // Idle State
            break;

        case SC_ENB_1:     // Enable states
            R_SeqAct.R_SCEnbAct = true;
            S_K1ClsCmd  = true;             // Command contactor to close
            R_SCTskTmr  = SysTm2;           // capture msec timestamp
            R_SCTskSt   = SC_ENB_2;         // Increment state
            break;

        case SC_ENB_2:
            tempTmr = DELTA_TM(SysTm2,R_SCTskTmr);
            if ( tempTmr > PARM(S_K1ClsDlyTm) )
            {
                R_CTChkEnb = true;   //K1 should be closed by now, enable CT checking !
                S_SimK1Clsd = true;  //Simulated Sync Contactor state for generator model
            }
            if ( tempTmr > (PARM(S_K1ClsDlyTm) + PARM(R_CTChkTm)) )  //Enough time to close K1 and test CTs
            {
                if ( !S_K1VmgErrPrm )   // Stator voltage should have dropped if K1 did not close
                {
                    PUSH_DIAG(S_K1ClsTmout);
                    FaultIsActive.S_K1ClsTmout = true;
                    S_K1ClsCmd            = false;    // Remove contactor command
                    R_SeqAct.R_SCEnbAct   = false;
                    R_SeqStat.R_SCEnbStat = false;
                    R_CTChkEnb = false;
                    R_SCTskSt             = SC_IDLE;  // Goto Idle state
                }
                else
                {
                    R_SCTskSt    = SC_ENB_3;     // Goto next Enable state
                    S_K1Status   = true;         // Declare K1 close
                    S_PllEnb = false;        // Use Grid monitor pll from this point on
                }
            }
            break;

        case SC_ENB_3:      // need to do anything with stator offset nulling here?
            R_SCTskTmr      = SysTm2;       // Capture current time
            R_SCTskSt       = SC_ENB_4;     // Goto next state
            break;

        case SC_ENB_4:
            R_SeqAct.R_SCEnbAct   = false;
            R_SeqStat.R_SCEnbStat = true;
            R_CTChkEnb = false;
            S_K1OpnNowReq       = false;
            break;

        case SC_DSB_1:
            R_SeqAct.R_SCEnbAct = true;
            
            
            R_SCTskTmr          = SysTm2;      // Reset counter
            R_SCTskSt           = SC_DSB_2;    // Goto next disable state
            
            break;
        case SC_DSB_2:
            tempTmr = DELTA_TM(SysTm2,R_SCTskTmr);
            if(tempTmr>PARM(S_K1OpnCmdDlyTm) || S_K1OpnNowReq)
            {
                S_PllEnb            = true;
                S_K1ClsCmd          = false;
                R_CTChkEnb          = false;
                R_SCTskSt           = SC_DSB_3;    // Goto next disable state
                R_SCTskTmr          = SysTm2;      // Reset counter
            }
            break;
        case SC_DSB_3:
            tempTmr = DELTA_TM(SysTm2,R_SCTskTmr);

            //Simulated Sync Contactor state for generator model
            if ( tempTmr > PARM(S_K1OpnDlyTm) ) S_SimK1Clsd = false;

            if ( tempTmr > S_K1OpTotDly )
            {

                if ( ( (S_IMag > PARM(S_K1ImagThrs)) || (GmSVMtchErSq < PARM(S_K1VlThrs) ) ) &&
                        !OpnGrdAct && !OpnPhsAct &&
                        !FAULTISACTIVE(IslandFlt) )
                {
                    PUSH_DIAG(S_K1OpnTmout);
                    FaultIsActive.S_K1OpnTmout = true;
                    Q1OpnReq = true;
                }
                else
                {
                    S_K1Status = false;            // Declare K1 open
                }

                R_SeqAct.R_SCEnbAct   = false;
                R_SeqStat.R_SCEnbStat = false;
                R_SCTskSt             = SC_IDLE;   // Goto Idle state
            }
            break;
    }   // switch (R_SCTskSt)

    // Publish synch contactor task enable status
    R_SCEnbStat = R_SeqStat.R_SCEnbStat;

    return;
}   //SyncContactorTask



///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Rotor synchronization sequencer task
//
// DESCRIPTION
//      Enable the rotor bridge and synchronize the stator voltage to the
//      line voltage.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////
void RotorSyncTask ()
{
    unsigned    Temp1, Temp2, Tmp_Flag;
    float   Frq_Err, V_Err, Frq_Err_Rev;


    /* Calculate signals used for decisions in states below */
    if (!S_PllProven)
    {
         S_FrqTacDif  = 0.0;
         S_FrqLinDif  = 0.0;
         S_FrqErrRevR = 0.0;
    }
    else
    {
        if (R_MctlCmd.R_AngElecFrz)
        {
            /* Difference between actual stator freq and expected value
             * when applying dc excitation to the rotor. */
            Frq_Err     = S_PllWT2 - R_WRtrT2;
            S_FrqTacDif += DelTm2 * PARM(S_WFrqDif) * (Frq_Err - S_FrqTacDif);

            S_FrqLinDif = 0.0;
            S_FrqErrRevR = 0.0;
        }
        else
        {
            /* Difference between actual stator freq and expected value when
             * the rotor phase rotation is correct. */
            Frq_Err     = S_PllWT2 - GmPllWT2;
            S_FrqLinDif += DelTm2 * PARM(S_WFrqDif) * (Frq_Err - S_FrqLinDif);

            /* Difference between actual stator freq and predicted value when
             * the rotor phase rotation is reversed. */
            Frq_Err_Rev = S_PllWT2 + GmPllWT2 - 2.0*R_WRtrT2;
            S_FrqErrRevR += DelTm2 * PARM(S_WFrqDif) * (Frq_Err_Rev - S_FrqErrRevR);

            S_FrqTacDif = 0.0;
        }
    }

    /* Error between commanded voltage and actual */
    if (R_MctlCmd.S_VregEn)
        S_VRegErrFil += DelTm2 * PARM(S_WVltErr) * (S_VregErr - S_VRegErrFil);
    else
        S_VRegErrFil = 0.0;

    // Error between stator Vmag and line Vmag
    V_Err   = GmVMagT2 - S_VMagT2;
    S_VMagErr += DelTm2 * PARM(S_WVltErr) * (V_Err - S_VMagErr);


    if (ABS(S_VMagErr) <  PARM(S_VSyncTol))
    {
       S_VltSyncTmr = S_VltSyncTmr + DelTm2;
       if ( S_VltSyncTmr >= PARM(S_SyncTm))
       {
           S_VltSyncTmr = PARM(S_SyncTm);
           S_VltSync = true;
       }
    }
    else
    {
       S_VltSyncTmr = 0.0;
       S_VltSync = false;
    }

    /* Voltage magnitude error across contactor K1 is used
       to set a permissive, only during stator CT checking */
    if ( R_CTChkEnb )
    {

       if ( ABS(S_VMagErr) <  S_K1VacrossTol )
       {
            if ( S_K1VmgErrTmr == 0.0 )
            {
                S_K1VmgErrPrm = false;
            }
            S_K1VmgErrTmr = S_K1VmgErrTmr + DelTm2;
            if ( S_K1VmgErrTmr >= S_K1VerrTime )
            {
                S_K1VmgErrTmr = S_K1VerrTime;
                S_K1VmgErrPrm = true;
            }
       }
       else
       {
            S_K1VmgErrTmr = 0.0;
            S_K1VmgErrPrm = false;
       }
    }
    else
       S_K1VmgErrTmr = 0.0;

    /* Force sync timer to zero prior to enabling of sync reg.  Ensures reg
     * will have time to push angle to center of tolerance window  */
    if (R_SyncTskSt > SYNC_IDLE && R_SyncTskSt < SYNC_ENB_14) Tmp_Flag = false;
    else                                                      Tmp_Flag = true;

    /* Filter sync angle error.  High stator pll gain makes signal noisy for
     * unbalanced line. */
    R_SynAngErFl += DelTm2 * PARM(S_WPhsErr) * (R_SyncAngErr - R_SynAngErFl);

    if ( (ABS(R_SynAngErFl) <  PARM(S_PhsSyncTol)) && Tmp_Flag)
    {
       S_PhsSyncTmr = S_PhsSyncTmr + DelTm2;
       if ( S_PhsSyncTmr >= PARM(S_SyncTm))
       {
           S_PhsSyncTmr = PARM(S_SyncTm);
           S_PhsSync = true;
       }
    }
    else
    {
       S_PhsSyncTmr = 0.0;
       S_PhsSync = false;
    }


    /* Form bit code used to determine which state to execute */
    Temp1 = (R_SeqReq.R_SyncReq) | (R_SeqStat.R_SyncStat << 1)
            | (R_SeqAct.R_SyncAct << 2);

    /* Combine fault and diagnostic/tuneup conditions into 1 boolean */
    Temp2 = Brg2TripFltZ1 || R_SeqStBits.R_DiagMode || R_SeqStBits.R_Parked
            || R_SeqStBits.R_TuneMode;

    switch (Temp1)
    {
        case 0:     /* Idle - do nothing*/
        case 3:
            R_SyncTskSt = SYNC_IDLE;
            break;

        case 1:     /* Goto 1st enable state unless Temp2 */
            if (Temp2)  /* Brg2TripFltZ1 or Diag/Tune mode */
            {
                PUSH_DIAG(R_BadSyncTsk);
                R_SyncTskSt = SYNC_IDLE;
            }
            else R_SyncTskSt = SYNC_ENB_1;
            break;

        case 2:     /* Goto 1st disable state */
            R_SyncTskSt = SYNC_DSB_1;
            break;

        case 4:     /* Abort enable function */
            if (R_SyncTskSt >= 0) R_SyncTskSt = SYNC_DSB_1;
            break;

        case 5:     /* Goto next enable state unless Temp2 */
            if (Temp2)  /* Brg2TripFltZ1 or Diag/Tune mode */
            {
                PUSH_DIAG(R_BadSyncTsk);
                R_SyncTskSt = SYNC_IDLE;
            }
            break;

        case 6:     /* Goto next disable state */
            break;

        case 7:     /* Abort disable function unless Temp2 */
            if (Temp2)  /* Brg2TripFltZ1 or Diag/Tune mode */
            {
                PUSH_DIAG(R_BadSyncTsk);
            }
            else    /* Not Diag/Tune mode, Brg2TripFltZ1 */
            {
                /* Go to first enable state */
                if (R_SyncTskSt <= 0) R_SyncTskSt = SYNC_ENB_1;
            }
            break;
    }   /* switch (Temp1) */

    switch (R_SyncTskSt)
    {
        case SYNC_IDLE:     /* Idle State */
            break;

        case SYNC_ENB_1:
            /* Initial Sync state */
            /* Set task active */
            /* Enable Bridge, Inner regs before turning flux on */
            /* Command dc output on rotor */
            R_SeqAct.R_SyncAct     = true;
            R_BrgPwrEnb            = true;
            R_InnrRegEnb           = true;
            R_MctlCmd.R_AngElecFrz = true;
            R_SyncTskTmr           = SysTm2;
            R_SyncTskSt            = SYNC_ENB_2;
            break;

        case SYNC_ENB_2:
            /* Enable flux */
            R_MctlCmd.R_FlxCmdOn  = true;

            /* wait for voltages and currents to settle (want rotor L * di/dt = 0) */
            if ( (R_SyncTskTm = DELTA_TM(SysTm2,R_SyncTskTmr)) > PARM(R_PrefluxTm))
            {
                R_VabStart  = 0.0;
                R_IaStart   = 0.0;
                R_IbStart   = 0.0;
                R_IcStart   = 0.0;
                S_VMagStart = 0.0;
                R_VyStart   = 0.0;
                R_IyStart   = 0.0;
                R_SyncTskTmr = SysTm2;
                R_SyncTskSt  = SYNC_ENB_3;
            }
            break;

        case SYNC_ENB_3:
            if ( PARM(TestMode) == PUMP_BACK )
            {
                R_MctlCmd.R_AngElecFrz = false;
                R_MctlCmd.S_VregEn     = true;
                if (DELTA_TM(SysTm2,R_SyncTskTmr) > PARM(R_PrefluxTm) )
                {
                    PUSH_DIAG(S_PllRotFlt);
                }
                else if ( S_PllProven )
                {
                    R_SyncTskTmr    = SysTm2;
                    R_SyncTskSt     = SYNC_ENB_12;
                }
            }
            else
            {
                if ( (R_SyncTskTm = (DELTA_TM(SysTm2,R_SyncTskTmr)) ) < PARM(R_PrefluxTm) )
                {
                    /* record V's & I's.  Rotor values should be IR drop only. */
                    R_IaStart   += (DelTm2 * PARM(R_WStrtVI)) * (R_IaT2    - R_IaStart);     // for rotor connection check
                    R_IbStart   += (DelTm2 * PARM(R_WStrtVI)) * (R_IbT2    - R_IbStart);
                    R_IcStart   += (DelTm2 * PARM(R_WStrtVI)) * (R_IcT2    - R_IcStart);

                    R_VabStart  += (DelTm2 * PARM(R_WStrtVI)) * (R_VabT2 - R_VabStart);

                    //--------------------------------------------------------------------
                    // prepare for the stator voltage connection check
                    S_VMagStart += (DelTm2 * PARM(R_WStrtVI)) * (S_VMagT2  - S_VMagStart);   // for stator connection check

                    R_VyStart   += (DelTm2 * PARM(R_WStrtVI)) * (R_VyFbkT2 - R_VyStart);     // For rotor R_RrMeas = Vy/Iy
                    R_IyStart   += (DelTm2 * PARM(R_WStrtVI)) * (R_IyFbkT2 - R_IyStart);     //  Note this wouldn't robustly
                }                                                                            //  detect one open phase.
                else
                {                                                                      // AngElec will target R_Vbc = 0
                    //--------------------------------------------------------------------
                    // check for open rotor connection                                 //  which maximizes cur mag in 3 phases.
                    if (   (R_VabStart > 3.0 * PARM(R_RrFltThr) * ABS(R_IbStart) )     // Expect Ib = 0.5 * Vab/(Rr + Rr||Rr)
                        || (R_VabStart > 3.0 * PARM(R_RrFltThr) * ABS(R_IcStart) )   ) // Expect Ic = 0.5 * Vab/(Rr + Rr||Rr)
                    {                                                                  // If phs A is open, will see in Ib & Ic.
                        PUSH_DIAG(R_RtrConFlt);
                    }

                    //--------------------------------------------------------------------
                    // check for missing stator volts
                    else if (S_VMagStart < PARM(S_VfbMinLvl) * S_FlxVMag)
                    {
                        PUSH_DIAG(S_VfbAbsent);
                    }

                    //--------------------------------------------------------------------
                    // no problem found so continue
                    else
                    {
                        R_SyncTskTmr = SysTm2;
                        R_SyncTskSt = SYNC_ENB_4;
                    }
                }
            }
            break;

        case SYNC_ENB_4:
            /* Enable stator PLL */
            R_SyncTskTmr = SysTm2;
            R_SyncTskSt = SYNC_ENB_5;
            break;

        case SYNC_ENB_5:
            /* Verify stator PLL has locked to rotational frequency. */
            if ( (R_SyncTskTm = DELTA_TM(SysTm2,R_SyncTskTmr)) > PARM(S_PllSetlTm))
            {
                PUSH_DIAG(S_PllRotFlt);
            }
            else if (S_PllProven)
            {
                S_LineFltEnb = true;
                R_SyncTskTmr = SysTm2;
                R_SyncTskSt = SYNC_ENB_6;
            }
            break;

        case SYNC_ENB_6:
            /* Enable voltage regulator. Delay for error signals to settle */
            R_MctlCmd.S_VregEn  = true;
            if ( (R_SyncTskTm = DELTA_TM(SysTm2,R_SyncTskTmr)) > PARM(S_SetlTm1))
            {
                R_SyncTskTmr = SysTm2;
                R_SyncTskSt = SYNC_ENB_7;
            }
            break;

        case SYNC_ENB_7:
            /* Verify stator frequency matches tach speed. */
            if (ABS(S_FrqTacDif) <  PARM(S_PllTacTol))
            {
                R_SyncTskTmr = SysTm2;
                R_SyncTskSt = SYNC_ENB_8;
            }
            else if ( (R_SyncTskTm = DELTA_TM(SysTm2,R_SyncTskTmr)) > PARM(S_FreqMtchFrzTm))
            {
                if (S_PllWT2 < 0.0)
                    PUSH_DIAG(S_RevPhsRot);   // reverse phase sequence detected on stator wiring
                else
                    PUSH_DIAG(S_PllTacFlt);   // mismatch between tach & stator pll - cause uncertain
            }
            break;

        case SYNC_ENB_8:
            /* Verify stator voltage magnitude matches commanded value. */
            if (ABS(S_VRegErrFil) < PARM(S_VMagTol))
                R_SyncTskSt = SYNC_ENB_9;
            else if ( (R_SyncTskTm =  DELTA_TM(SysTm2,R_SyncTskTmr)) > PARM(S_VoltMtchFrzTm))
            {
                PUSH_DIAG(S_VMagFlt);
            }
            break;

        case SYNC_ENB_9:
            S_LineFltEnb = false;
            R_SyncTskTmr = SysTm2;
            R_SyncTskSt = SYNC_ENB_10;
            break;

        case SYNC_ENB_10:
            /* un-freeze electrical rotation on rotor */
            R_MctlCmd.R_AngElecFrz = false;
            R_SyncTskTmr = SysTm2;
            R_SyncTskSt = SYNC_ENB_11;
            break;

        case SYNC_ENB_11:
            /* wait for rotor to catch up to (line - tach) angle */
            if (R_AngRotStat)
            {
                R_SyncTskTmr = SysTm2;
                R_SyncTskSt = SYNC_ENB_12;
            }
            break;

        case SYNC_ENB_12:
            /* Delay for error signals to settle */
            if ( (R_SyncTskTm = DELTA_TM(SysTm2,R_SyncTskTmr)) > PARM(S_SetlTm2))
            {
                S_LineFltEnb = true;
                R_SyncTskTmr = SysTm2;
                R_SyncTskSt = SYNC_ENB_13;
            }
            break;

        case SYNC_ENB_13:
            /* Check stator frequency matches line frequency.
             * Will not be ok if wrong phase rotation on rotor wiring. */
            if ( ABS(S_FrqLinDif) < PARM(S_SyncFrqTol))
            {
                R_SyncTskTmr = SysTm2;
                R_SyncTskSt = SYNC_ENB_14;
            }
            else if ( (R_SyncTskTm = DELTA_TM(SysTm2,R_SyncTskTmr)) > PARM(S_FreqMtchTm))
            {
                if (ABS(S_FrqErrRevR) < (2.0 * PARM(S_SyncFrqTol)))
                    PUSH_DIAG(R_RevPhsRot);  // reverse phase sequence detected on rotor wiring
                else
                    PUSH_DIAG(S_FrqLinFlt);  // mismatch between line & stator pll - cause uncertain
            }
            break;

        case SYNC_ENB_14:
            /* command stator sync to line */
            R_MctlCmd.S_SyncCmd = true;  // initiate synchronization and tach-loose check
            S_XmAdptEnb         = true;  // enable Xm adaptation
            if ( (R_SyncTskTm = DELTA_TM(SysTm2,R_SyncTskTmr)) > PARM(S_PllHotDly))
            {
               S_PllAllowCl = true;
            }
            if ( R_SyncTskTm  > PARM(S_SyncDly) )
            {
                R_SyncTskTmr = SysTm2;
                R_SyncTskSt = SYNC_ENB_15;
            }
            break;

        case SYNC_ENB_15:
            /* verify stator volts mag and phase is synced to line */
            if ( S_VltSync && S_PhsSync)
            {
                R_SyncTskSt = SYNC_ENB_16;
            }
            else if ( (R_SyncTskTm = DELTA_TM(SysTm2,R_SyncTskTmr)) > PARM(S_SyncTmOut))
            {
                if (!S_VltSync) PUSH_DIAG(S_VltSyncFlt);
                if (!S_PhsSync) PUSH_DIAG(S_PhsSyncFlt);
            }
            break;

        case SYNC_ENB_16:
            /* Remove stator sync to line command.  Will hold value of tach zero */
            R_MctlCmd.S_SyncCmd  = false;
            R_MctlCmd.S_VregEn   = false;
            S_XmAdptEnb          = false;
			if ( R_TachLsDoneT2 == true )   // proceed once tach-loose check is done!
            {
            R_SyncTskSt = SYNC_ENB_17;
			}
            break;

        case SYNC_ENB_17:
            /* Update Active, Status bits */
            R_SeqStat.R_SyncStat   = true;
            R_SeqAct.R_SyncAct     = false;
            R_SyncTskSt = SYNC_IDLE;
            break;

        case SYNC_DSB_1:
            R_SeqAct.R_SyncAct       = true;     /* Set active bit */
            R_BrgPwrEnb              = false;    /* Disable bridge, inner regs */
            R_InnrRegEnb             = false;
            S_XmAdptEnb              = false;
            S_LineFltEnb             = false;
            R_MctlCmd.R_FlxCmdOn     = false;
            R_MctlCmd.R_AngElecFrz   = false;
            R_MctlCmd.S_VregEn       = false;
            R_MctlCmd.S_SyncCmd      = false;
            R_SyncTskSt              = SYNC_DSB_2;
            S_PllAllowCl = false;

            break;

        case SYNC_DSB_2:
            /* Update Active, Status bits */
            R_SeqStat.R_SyncStat      = false;
            R_SeqAct.R_SyncAct        = false;
            R_SyncTskSt = SYNC_IDLE;
            break;

        default:
            break;
    }   /* switch (R_SyncTskSt) */

    // Publish synch sequencer task status
    R_SyncStat = R_SeqStat.R_SyncStat;

    // Publish sequencer bit for synchronization control
    S_SyncCmd  = R_MctlCmd.S_SyncCmd;

    return;
}





