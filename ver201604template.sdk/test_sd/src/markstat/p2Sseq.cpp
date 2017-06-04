///////////////////////////////////////////////////////////////////////////////
//
// TITLE:        Source-side level state machine sequencer
//
// DESCRIPTION:
//      Coordination of line sequencer components.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     p2Sseq.vsd, p2Sseq.xls
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
#include "b0CelTst.h"
#include "p2Sseq.h"
#include "s0SeqTsk.h"
#include "b0BridgeRebf.h"
#include "b0DbCelTst.h"
#include "b0DbCtl.h"
#include "b0DbPlsTst.h"

// Externals
//----------
extern FaultIsActiveTyp FaultIsActive;
extern SeqActBits       L_SeqAct;                   // Sequencer Task Action bits
extern SeqStatBits      L_SeqStat;                  // Sequencer Task Status bits
extern unsigned         R_SeqStat_PackedBits;
extern R_SeqActBits     R_SeqAct;
extern DistFilPhSta     DFphA[DFMAXTESTNUMBER];     // Distortion filetr phase-A reactive pwr status
extern DistFilPhSta     DFphB[DFMAXTESTNUMBER];     // Distortion filetr phase-B reactive pwr status
extern DistFilPhSta     DFphC[DFMAXTESTNUMBER];     // Distortion filetr phase-C reactive pwr status
extern CtlStsDat        CSDat;
extern cCellTest        L_CellTest;
extern cBridgeRebf      L_BridgeRebf;

extern float            DelTm2;
extern float            DelTm3;
extern float            GmI1Alpha;
extern float            GmI1Beta;
extern float            GmI2Alpha;
extern float            GmI2Beta;
extern float            GmI3Alpha;
extern float            GmI3Beta;
extern float            GmQPwr;
extern float            GmV1nAlpha;
extern float            GmV1nBeta;
extern float            GmV2nAlpha;
extern float            GmV2nBeta;
extern float            GmV3nAlpha;
extern float            GmV3nBeta;
extern float            GmVMagPuT2;
extern float            L_DBRRcp;
extern float            L_VdcFbkT2;
extern float            L_VMagT2;
extern float            L_VMagT2Fil;
extern float            L_VxFbkT2;
extern float            L_VyFbkT2;
extern float            L_MaDeltVoltSqFil;
extern float            LineLnSeqFil;
extern int              L_DfTestIndx;
extern unsigned         IslMaOpnReq;
extern unsigned         Brg1TripFlt;
extern unsigned         GmLineVShort;
extern unsigned         HdwTstMode;
extern unsigned         S_K1Status;
extern unsigned         L_BrgPwrEnb;
extern unsigned         L_DBOTDsbFlg;
extern unsigned         L_DbPwrEnb;
extern unsigned         L_DbGateDsb;
extern unsigned         L_DCLinkInit;
extern unsigned         L_DCLinkInitT2;
extern unsigned         L_DSatDx;
extern unsigned         L_IocInstEnb;
extern unsigned         L_IOffsVld;
extern unsigned         L_ParkReq;
extern unsigned         L_RunRdy;
extern unsigned         L_SftyChnOpn;
extern unsigned         L_ZVGtOff;
extern unsigned         PwrDnFbkDsb;
extern unsigned         SafetyStopIp;
extern unsigned         SftyChnOpn;
extern unsigned         SftyChnQ1Opn;
extern unsigned         InitDone;

// for LineCoreSequencer
// for LineCoreDriveSequencer
extern unsigned Q1TripFbk;      // Trip feedback signle from Q1
extern unsigned L_AcLinePres;   // Ac line present signal
extern int L_DfTestIndx;
extern unsigned DbAutoTstEnb;
extern unsigned L_PulseTestReq;
extern unsigned R_PlsTstReq;
extern unsigned L_SeqCelTstReq;
extern unsigned R_SeqCelTstReq;
extern unsigned L_DbDsbOverloadT2;
extern unsigned DbAutoTstForce;
// for LineKeypadProductStatusWrite
// for InitCoreLineSequencer

extern unsigned FchkTstDone;

// Constants
//----------


// Types and Classes
//------------------
extern cDbCelTst L_DbCelTst;
extern cDbCtl    L_DbCtl;
// Parameters
//-----------
CREATE_PARM(DFtestDlyTm,        float);     // Time delay between KDF contactor transitions for dist-filt verification
CREATE_PARM(MvSwGearTrpTm,      float);     // Time to energize the MVSG trip coil
CREATE_PARM(L_DbAutoTstInitTm,  float);     // DB auto test init time (time for 1st pulse)
CREATE_PARM(L_Q1FiltTm,         float);     // Q1 contactor waiting time for filtered signals to settle
CREATE_PARM(L_Q1VlThrs,         float);     // Q1 contactor voltage threshold for status verification

// for LineCoreSequencer
// for LineCoreDriveSequencer
CREATE_PARM(L_PlsTstVmagMax,    float);    // Max line terminal voltage
CREATE_PARM(L_StartupDbMsEnb,   unsigned);
// for LineKeypadProductStatusWrite
// for InitCoreLineSequencer

// Diagnostics
//------------
CREATE_DIAG(MsgStkFail);        // ADL message stack failure
CREATE_DIAG(Q1Tripped);         //
CREATE_DIAG(L_RunWhlInit);      // Run generated during initialization

// for LineKeypadProductStatusWrite
// for InitCoreLineSequencer
// Variables
//----------
CREATE_PUBVAR(L_DbPwrEnb,       unsigned);      // DB enable boolean
CREATE_PUBVAR(DfTestTmr,        float);            // dist-filter test timer
CREATE_PUBVAR(L_CTDismTmr,      float);            // Line cell test dismissal timer
CREATE_PUBVAR(L_StopStTmr,      float);            // Stop state timer
CREATE_PUBVAR(MvSwGearTrpTmr,   float);            // MVSG trip timer

CREATE_PUBVAR(L_SeqSt,          int);              // Sequencer state variable
CREATE_PUBVAR(L_AutoCTAct,      unsigned) = false; // Line automatic cell test is active
CREATE_PUBVAR(L_CelTstCmd,      unsigned);         // Cell Test command



CREATE_PUBVAR(L_InitDone,       unsigned);         // Flag that shows that rotor initialization is done
CREATE_PUBVAR(L_LTestCmd,       unsigned);         // Inductor Test command */
CREATE_PUBVAR(L_PlsTstCmd,      unsigned);         // Pulse Test command
CREATE_PUBVAR(L_RunCmd,         unsigned);         // Line run command
CREATE_PUBVAR(L_Running,        unsigned);         // Lince converter is Running, or in ZVRT standby
//CREATE_PUBVAR(L_RunAct,         unsigned);         // Lince converter is actually Running
CREATE_PUBVAR(L_RunReq ,        unsigned);         // Run command request
CREATE_PUBVAR(MvSwGearShuntTrp, unsigned);         // Medium voltage switchgear shunt trip
CREATE_PUBVAR(MvSwGearTrpReq,   unsigned) = false; // Collection of faults that cause medium voltage switchgear to trip
CREATE_PUBVAR(MvSwGearShuntSep, unsigned);         // Equivalent MV switchgear shunt trip affecting converter in SEP lab
CREATE_PUBVAR(Q1OpnReq,         unsigned) = false;  // Q1OpnReq


// for LineCoreSequencer
// for LineCoreDriveSequencer
CREATE_PUBVAR(L_DbCelTstCmd,    unsigned);  // DB Cell Pulse test command
CREATE_PUBVAR(L_DbPlsTstCmd,    unsigned);  // DB Pulse test command
CREATE_PUBVAR(L_DiagCmd,        unsigned);  // Line combined diagnostic command
CREATE_PUBVAR(Q1EnbStatZ1,      unsigned);  // Q1 enable state Z1
CREATE_PUBVAR(L_ChgLvlDcDy,     float);     // Dc charge level dynamic
CREATE_PUBVAR(L_ChgLvlAcDy,     float);     // Ac charge level dynamic
CREATE_PUBVAR(L_DbAutoTstDone,  unsigned)=false;    // flag indicate whether DB auto test is done
CREATE_PUBVAR(L_DbcAtDnT2,      unsigned);  // mapping to Dbc class auto done flag in task 2 version
CREATE_PUBVAR(L_DbAutoTstTmr,   float);     // DB auto test timer for first pulse
// for LineKeypadProductStatusWrite
// for InitCoreLineSequencer


// Unpublished Variables
//----------------------
SeqBits1                L_SeqStBits;               // Sequencer State bits
unsigned                L_SeqReq_PackedBits;                  // Sequencer Task Request bits
unsigned                L_SeqCelTstReq;                // Auxiliary var to hold conversion of single bit into boolean
cCellTest::AutoCtTyp    L_AutoCelTst = cCellTest::AUTOCT_OFF;    // Line auto-cell-test type
// Data Pass
//----------
DATA_PASS(L_DbCtl.DbcDb1AutoTstDone,    L_DbcAtDnT2,        T1_T2,  UNFILTERED);

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Line core sequencer
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineCoreSequencer(void)
{
    if (IslMaOpnReq)
    {
        L_SeqReq.L_Q1EnbReq = false;
    }

    if ( MvSwGearTrpReq || (L_PlsTstCmd || L_CelTstCmd))  // Include trip-faults and other conditions that
        L_SeqReq.L_Q1EnbReq = false;                      // open medium voltage switchgear

    if ( MvSwGearTrpReq && !FAULTISACTIVE(Ps24Nok))  // Only include trip-faults that open medium voltage switchgear
    {                                                      // and if control power is ok. (Getting spurious faults as pwr lost.)
        MvSwGearShuntTrp = true;
        MvSwGearShuntSep = true;
    }

    if (MvSwGearShuntTrp)
    {
        MvSwGearTrpTmr += DelTm2;
        if (MvSwGearTrpTmr > PARM(MvSwGearTrpTm))
        {
            MvSwGearTrpReq   = false;
            MvSwGearShuntTrp = false;
            MvSwGearShuntSep = false;
        }
    }
    else
    {
        MvSwGearTrpTmr = 0.0F;
    }


    if (Brg1TripFlt || !L_RunCmd)
    {
        L_SeqReq.L_RegEnbReq = false;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Line core drive-sequencer. Core drive sequencer state machine that
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
void LineCoreDriveSequencer(void)
{
    int *tmp;
    unsigned L_DiagCmd;


    // Note: On May 9, 2000, !L_RunRdy was added to this logic.  The existing
    // L_RunRdy logic had been added for dislay only.  Since that time serveral
    // additions had been made to that logic with the intent of those changes to
    // effect the L_RunReq signal (as shown in the user diagram). Addition of
    // L_RunRdy to this string now makes those changes effective.  NOTE THAT
    // LOGIC INSERTED DIRECTLY HERE WILL TAKE EFFECT 1 task 2 faster than
    // the logic that drives L_RunRdy.  DO NOT REMOVE TRIPFLT FROM THIS STRING
    // FOR THAT REASON!!!   TRIPFLT IS IN THE L_RUNRDY STRING ONLY FOR DISPLAY.
    // IT ACTS ONE msec FASTER HERE.
    //

    //DO NOT REMOVE TRIPFLT (OR ANY ITEMS) FROM THIS STRING.
    if (Brg1TripFlt || !L_IOffsVld || L_SeqStBits.Parked || (!L_RunRdy && !SafetyStopIp) )
    {
        L_RunReq = false;
    }

    if ( L_RunReq )
    {
        L_RunCmd    = true;
        L_PlsTstCmd  = false;
        L_CelTstCmd  = false;
        L_DbPlsTstCmd = false;
        L_DbCelTstCmd = false;
        L_DiagCmd     = false;
    }
    else
    {
        L_RunCmd  = false;
    }
    // Clear Run cmd if TripFlt
    if (Brg1TripFlt)
    {
        L_RunCmd = false;
    }
    if ( !L_RunCmd)
    {
        // Form L_DiagCmd - composite of all diagnostic/tune-up test commands */
        L_DiagCmd = L_PlsTstCmd || L_DbPlsTstCmd || L_CelTstCmd || L_DbCelTstCmd;
        if (L_PlsTstCmd || L_CelTstCmd || L_DbPlsTstCmd)
        {
            if(!L_RunRdy)
            {
                L_PlsTstCmd  = false;
                L_CelTstCmd  = false;
                L_DbPlsTstCmd = false;
            }
        }

        // Close message stack used for cell-test wizard if test was never initiated
        if ( L_CTDismTmr > 0.0F )
        {
            if ( L_CellTest.CellTestDismissal( 1 , L_CTDismTmr ) == BUSY )
            {
                L_CTDismTmr = L_CTDismTmr + DelTm2;
            }
            else
            {
                L_CTDismTmr = 0.0F;
            }
        }

        if ( L_DiagCmd && !L_DbCelTstCmd)
        {
            //if (L_SftyChnOpn) PUSH_DIAG(L_SftyChnOpn);

            if ( Brg1TripFlt  || L_SftyChnOpn)
            {
                if ( L_CelTstCmd && !L_SeqAct.L_CelTstAct )      // Dismiss cell test if it was never initiated
                {
                    if ( L_CellTest.CellTestDismissal( 0 , 0.0 ) == SUCCESS )
                    {
                        L_CTDismTmr = DelTm2;
                    }
                }
                if ( L_DbCelTst.DbCtMsgStkSt )
                {
                    if ( L_DbCelTst.CloseMsgQue() != SUCCESS )
                    {
                        PUSH_DIAG( MsgStkFail );
                    }
                    //DBMsgStkIni = FALSE; already done inside DbCelTst class
                }
                L_PlsTstCmd = false;
                L_CelTstCmd = false;
                L_DbPlsTstCmd = false;
            }
        }
    }

    if ( Q1OpnReq || L_PlsTstCmd || L_CelTstCmd || Q1TripFbk )   // Include trip-faults and other conditions that
    {
        L_SeqReq.L_Q1EnbReq = false;
    }   // open main breaker Q1

    // Set the DB auto test enable signal
    if( (!PARM(DbAutoTstInh)) && (!L_Running) && InitDone && !( L_PulseTestReq || R_PlsTstReq ) &&
         !( L_SeqCelTstReq || R_SeqCelTstReq ) &&
         !L_DbDsbOverloadT2 && L_VdcFbkT2 > PARM(DbAtoPingVdcMin) && PARM(L_BrgDbEnb) && InitDone)
    {
        DbAutoTstEnb = true;
    }
    else
    {
        DbAutoTstEnb = false;
    }
    /////////////////////////////////////////////////////////////////////////////////
    // DB power enable logic for stopped state
    /////////////////////////////////////////////////////////////////////////////////
    /*if ( DBApPwrEnb || ( ( (L_VdcFbkT2 > EE.CI.L_DBGtOnThrs) || L_MAOpnTsReq)  && !L_DBGateDsb) )*/
    if ( ( L_VdcFbkT2 > PARM(DbVmax) )  && !L_DbGateDsb )
    {
        L_DbPwrEnb             = true;
    }
    else
    {
        L_DbPwrEnb             = false;
    }
    /////////////////////////////////////////////////////////////////////////////////
    // End of DB power enable logic for stopped state
    /////////////////////////////////////////////////////////////////////////////////

//--------------------------
// State Machine begins here

    switch ( L_SeqSt )
    {
        //----------------------------- LINE_STOPPED ---------------------------
        case LINE_STOPPED:
            L_StopStTmr = L_StopStTmr + DelTm2;

            tmp = (int*) &L_SeqStat;     // No task active except the one regarding charger
            if (((*tmp & 0xFFDF) == 0) && !L_BrgPwrEnb)
            {
                if ( L_RunCmd )
                {
                    L_SeqSt = LINE_CHARGECHECK;
                    L_StopStTmr = 0.0F;
                    FchkTstDone = false;
                }
                else if ( L_DiagCmd )
                {
                    if (L_DbPlsTstCmd || L_DbCelTstCmd)
                    {
                        L_SeqSt = LINE_CHARGECHECK;
                    }
                    else if ( !L_SeqStat.L_Q1EnbStat )  // wait for Q1 to open
                    {
                        L_SeqSt = LINE_CHARGECHECK;
                    }
                    L_StopStTmr = 0.0F;
                    FchkTstDone = false;
                }
                L_AcLinePres   = true;      //Line bridge always has voltage present in its stop state
            }

            // Disable all requests while stopped

            L_SeqReq.L_ChgEnbReq  = false;
            L_SeqReq.L_RegEnbReq  = false;
            L_SeqReq.L_SyncIniReq = false;
            if ( SftyChnQ1Opn )   L_SeqReq.L_Q1EnbReq = false;

            // Change the value of L_ChgLvlDcDy and L_ChgLvlAcDy to the original paramter.
            L_ChgLvlDcDy          = PARM(L_ChgLvlDc);
            L_ChgLvlAcDy          = PARM(L_ChgLvlAc);

            L_DfTestIndx =  -1;      // Dist-filter is not being tested

            // Do not delay opening of KDF if Q1 doesn't need to open and K1 successfully opened
            if ( L_SeqReq.L_Q1EnbReq && !SafetyStopIp && !S_K1Status )
            {
                L_SeqReq.L_KdfEnbReq = false;
            }
            // Wait until Q1 and K1 finish their sequence tasks to remove KDF request
            else if ( !L_SeqStat.L_Q1EnbStat && !R_SeqStat.R_SCEnbStat )
            {
                L_SeqReq.L_KdfEnbReq = false;
            }

            L_Running             = false;

            if ( Q1TripFbk)
            {
                PUSH_DIAG(Q1Tripped);
            }

            // code for DB first auto ping
            if (    !L_DiagCmd  &&  !L_RunReq
                &&  !L_SeqStBits.Parked && !L_ParkReq
                &&  !HdwTstMode
                &&  !SftyChnOpn
                &&  !Brg1TripFlt
                &&  !GmLineVShort    &&  (L_VMagT2Fil > PARM(L_Q1OpnTvMin))
                &&  !PwrDnFbkDsb
                &&  DbAutoTstEnb
                &&  !L_DbAutoTstDone)
            {
                L_DbAutoTstTmr += DelTm2;
                if (L_DbAutoTstTmr>PARM(L_DbAutoTstInitTm))
                {
                    L_DbAutoTstTmr  = PARM(L_DbAutoTstInitTm);
                    DbAutoTstForce  = true;
                    if(L_DbcAtDnT2)
                    {
                        L_DbAutoTstDone  = true;
                    }
                }
            }
            else
            {
                L_DbAutoTstTmr  = 0.0F;
                DbAutoTstForce  = false;
            }

            break;

        //-------------------------- LINE_CHARGECHECK --------------------------
        case LINE_CHARGECHECK:

            //  Go to Stopped state if Brg1TripFlt or removal of Run or removal of diagnostic
            if ( Brg1TripFlt || !(L_RunCmd || L_DiagCmd) )
            {
                L_SeqSt = LINE_STOPPED;
                L_SeqReq.L_ChgEnbReq = false;
            }
            else
            {
                L_SeqReq.L_ChgEnbReq = true;

                //  Go to next state when Charging is complete
                if ( L_SeqStat.L_ChgEnbStat && !L_SeqAct.L_ChgEnbAct )
                {
                    if (!L_DbCelTstCmd)
                    {
                        L_DfTestIndx = 0;
                        L_SeqSt = LINE_CLOSE_KDF;
                    }
                    else
                    {
                        if(L_DbCelTst.OpenMsgQue()== SUCCESS)
                        {
                            L_DfTestIndx = 0;
                            L_SeqSt = LINE_CLOSE_KDF;
                        }
                        else
                        {
                            PUSH_DIAG( MsgStkFail );
                            L_DbCelTstCmd = false;
                        }
                    }
                }
            }

            break;
        //--------------------------- LINE_CLOSE_KDF ---------------------------
        case LINE_CLOSE_KDF:
            // Go to Stopped state if Brg1TripFlt or removal of Run or removal of diagnostic
            if (Brg1TripFlt || !(L_RunCmd ||L_DiagCmd))
            {
                L_SeqSt=LINE_STOPPED;
                L_SeqReq.L_KdfEnbReq = FALSE;
            }
            else
            {
                L_SeqReq.L_KdfEnbReq=true;
                /*  Go to next state when distortion filter is connected */
                if(L_SeqStat.L_KdfEnbStat)
                {
                    if(L_PlsTstCmd)
                    {
                        // Shorted phases are expected once KDF closes for pulse test
                        if(L_VMagT2>PARM(L_PlsTstVmagMax))
                        {
                            L_PlsTstCmd = false;
                            L_SeqSt = LINE_STOPPED;
                            L_SeqReq.L_KdfEnbReq = FALSE;
                        }
                        else
                        {
                            L_SeqSt = LINE_DIAGNOSTIC;
                        }
                    }
                    else if(L_CelTstCmd)
                    {
                        L_SeqSt = LINE_DIAGNOSTIC;
                    }
                    else if ( L_DbPlsTstCmd)
                    {
                        DfTestTmr=0.0F;
                        L_SeqSt = LINE_CLOSE_Q1;
                    }
                    else if (L_DbCelTstCmd) // DB cell test
                    {
                        DfTestTmr = 0.0F;
                        L_SeqSt = LINE_CLOSE_Q1;
                        cMessageStack::PushMsg( 418, xINT, NULL ); //"KDF closed"
                    }
                    else
                    {
                        DfTestTmr = 0.0F;
                        L_SeqSt = LINE_CLOSE_Q1;
                    }
                }
            }
            break;
        //--------------------------- LINE_CLOSE_Q1 ----------------------------

        case LINE_CLOSE_Q1:
            if (Brg1TripFlt || !(L_RunCmd || L_DiagCmd) )
            {
                if ( LineLnSeqFil > PARM(L_Q1VlThrs) )
                {
                    L_SeqReq.L_Q1EnbReq = false;
                }
                L_SeqSt = LINE_STOPPED;
                if ( L_SeqStat.L_Q1EnbStat )   Q1EnbStatZ1 = TRUE;
            }
            else
            {
                if ( L_SeqStat.L_KdfEnbStat && L_SeqReq.L_KdfEnbReq )
                {
                    DfTestTmr =  DfTestTmr + DelTm2;  // Wait while Dist-Filter stays closed
                }
                else if ( !L_SeqStat.L_KdfEnbStat && !L_SeqReq.L_KdfEnbReq )
                {
                    DfTestTmr =  DfTestTmr - DelTm2;  // Wait while Dist-Filter stays opened
                }

                if ( Q1EnbStatZ1 )      // Request the closing of MA independently of its status
                {
                    L_SeqStat.L_Q1EnbStat  = false;
                    L_SeqReq.L_Q1EnbReq   = true;
                    Q1EnbStatZ1           = false;
                }
                else if ( L_SeqStat.L_Q1EnbStat )   // Go to next state when Q1 has a closed state
                {

                    if ( L_DfTestIndx > 0 )   // A negative index indicates that dist-filter verification is over
                    {
                        if ( L_SeqStat.L_KdfEnbStat && (DfTestTmr >= PARM(DFtestDlyTm)) )
                        {
                            L_SeqReq.L_KdfEnbReq = false;  // Remove Kdf request
                        }
                        else if ( !L_SeqStat.L_KdfEnbStat && (DfTestTmr <= 0.0F) )
                        {
                            DfTestTmr     = 0.0;
                            L_SeqSt     = LINE_CLOSE_KDF;  // Go back to close KDF
                            Q1EnbStatZ1 = true;   // Request closing of MA again during next pass
                        }
                    }
                    else if (L_DbPlsTstCmd)
                    {
                        L_SeqSt     = LINE_DIAGNOSTIC;
                        Q1EnbStatZ1 = true;
                    }
                    else if (L_DbCelTstCmd)
                    {
                        L_SeqSt     = LINE_DIAGNOSTIC;
                        cMessageStack::PushMsg( 420, xINT, NULL );  //"Q1 closed"
                        Q1EnbStatZ1 = TRUE;
                    }
                    #ifdef LEG_TEST
                    else if ( PARM(TestMode) == LF_LEG_TEST)
                    {
                        L_SeqSt     = LINE_IDLE_RTR_LEG_TEST;
                        Q1EnbStatZ1 = TRUE;
                    }
                    #endif
                    else if (PARM(L_StartupDbMsEnb))
                    {
                        L_SeqSt     = LINE_DB_RES_MEAS;
                        Q1EnbStatZ1 = TRUE;
                    }
                    else
                    {
                        L_SeqSt       = LINE_PLLSYNC;
                        Q1EnbStatZ1 = false;
                    }

                }
                else
                {
                    L_SeqReq.L_Q1EnbReq  = true;  // Request to close Q1
                }
            }

            L_PlsTstCmd = false;        // Always remove pulse test command in this state

            break;
        //----------------------------DB_RES_MEAS-------------------------------
        case LINE_DB_RES_MEAS:
            // Disable Db resistance test if fault occurs or loss of run command
            if (Brg1TripFlt || !L_RunCmd || !PARM(L_BrgDbEnb) || (L_DbGateDsb && L_DbAutoTstDone))
            {
                L_ChgLvlDcDy = PARM(L_ChgLvlDc);
                L_SeqSt      = LINE_STOPPED;
                L_SeqReq.L_DbResMsReq = false;
            }
            else
            {
                L_ChgLvlDcDy = PARM(L_ChgLvlDcDB);

                L_SeqReq.L_DbResMsReq = true;
                if(L_SeqStat.L_DbResMsSta && !L_SeqAct.L_DbResMsAct)
                {
                    L_ChgLvlDcDy = PARM(L_ChgLvlDc);
                    L_SeqSt      = LINE_PLLSYNC;
                    L_SeqReq.L_DbResMsReq = false;
                }
            }
             break;
        #ifdef LEG_TEST
        //----------------------------IDLE_RTR_LEG_TEST-------------------------------
        case LINE_IDLE_RTR_LEG_TEST:

            if (Brg1TripFlt || !L_RunCmd)
            {
                L_SeqSt      = LINE_STOPPED;
            }

            break;
        #endif
        //--------------------------- LINE_PLLSYNC -----------------------------
        case LINE_PLLSYNC:
            if (Brg1TripFlt || !L_RunCmd)
            {
                L_SeqSt = LINE_STOPPED;
                L_SeqReq.L_SyncIniReq = false;
            }
            else
            {
                L_SeqReq.L_SyncIniReq = true;
                if ( L_SeqStat.L_SyncIniSta && !L_IocInstEnb)
                {
                    L_SeqSt = LINE_RAMPUP;
                    L_SeqReq.L_RegEnbReq = true;
                }
            }

            L_PlsTstCmd = false;        // Always remove pulse test command is this state

            break;

        //--------------------------- LINE_RAMPUP ------------------------------
        case LINE_RAMPUP:
            if (Brg1TripFlt || !L_RunCmd )
            {
                L_SeqSt = LINE_STOPPED;
            }
            else
            {
                if ((L_SeqStat.L_RegEnbStat) && ( L_DCLinkInitT2)) L_SeqSt = LINE_RUNNING;
            }

            L_PlsTstCmd = false;        // Always remove pulse test command is this state

            break;

        //-------------------------- LINE_RUNNING ------------------------------
        case LINE_RUNNING:
            L_Running = true;

            if (Brg1TripFlt || !L_RunCmd)
            {
                L_SeqSt = LINE_STOPPED;
                L_SeqReq.L_RegEnbReq = false;
                L_Running            = false;
                L_DCLinkInit         = false;
            }
            else if (L_ZVGtOff)
            {
                L_SeqReq.L_RegEnbReq = false;
                if ( !L_SeqStat.L_RegEnbStat)
                {
                    L_SeqSt      = LINE_ZVRT_STANDBY;
                    L_DCLinkInit = false;
                }
            }

            L_PlsTstCmd = false;        // Always remove pulse test command is this state

            break;

        //----------------------  LINE_ZVRT_STANDBY ----------------------------
        case LINE_ZVRT_STANDBY:
            L_Running = true;

            if (Brg1TripFlt || !L_RunCmd)
            {
                L_SeqSt = LINE_STOPPED;
                L_SeqReq.L_RegEnbReq = false;
                L_Running            = false;
                L_DCLinkInit         = false;
            }
            else if (!L_ZVGtOff)
            {
                L_SeqReq.L_RegEnbReq = true;
                if (L_SeqStat.L_RegEnbStat)
                {
                    L_SeqSt = LINE_RAMPUP;
                }
            }

            L_PlsTstCmd = false;        // Always remove pulse test command is this state

            break;

        //-----------------------  LINE_DIAGNOSTIC -----------------------------
        case LINE_DIAGNOSTIC:    // Execution of Diagnostic commands
            //  Go to Stopped state if Trip_Flt or removal of Pulse/Cell test command
            if ( Brg1TripFlt || (!L_DiagCmd))
            {
                L_SeqSt = LINE_STOPPED;
            }
            else if (L_PlsTstCmd)
            {
                L_SeqSt = LINE_PULSE_TEST;
            }
            else if (L_CelTstCmd)
            {
                L_SeqSt = LINE_CELL_TEST;
            }
            else if (L_DbPlsTstCmd)
            {
                L_SeqSt = LINE_DB_PULSE_TEST;
            }
            else if (L_DbCelTstCmd)
            {
                L_SeqSt = LINE_DB_CELL_TEST;
            }
            break;

        //-------------------------  PULSE_TEST --------------------------------
        case LINE_PULSE_TEST:
            // Disable pulse test if complete, aborted or fault occurs
            if (Brg1TripFlt || L_SeqStat.L_PlsTstStat || !L_PlsTstCmd)
            {
                L_SeqReq.L_PlsTstReq = false;
                L_PlsTstCmd          = false;
                if (!L_SeqStat.L_PlsTstStat && !L_SeqAct.L_PlsTstAct)
                {
                    L_SeqSt = LINE_DIAGNOSTIC;
                }
            }
            else
            {
                L_SeqReq.L_PlsTstReq = true;  // Set pulse test request
            }
            break;

        //-------------------------  CELL_TEST ---------------------------------
        case LINE_CELL_TEST:
            // Disable cell test if complete, aborted or fault occurs
            if (Brg1TripFlt || L_SeqStat.L_CelTstStat || !L_CelTstCmd)
            {
                L_SeqReq.L_CelTstReq = false;
                L_CelTstCmd          = false;
                if (!L_SeqStat.L_CelTstStat && !L_SeqAct.L_CelTstAct)
                {
                    L_SeqSt = LINE_DIAGNOSTIC;
                }
            }
            else
            {
                L_SeqReq.L_CelTstReq = true;  // Set cell test request
            }

            L_PlsTstCmd = false;        // Always remove pulse test command is this state

            break;

        //-----------------------  DB_PULSE_TEST -------------------------------
        case LINE_DB_PULSE_TEST:
            // Disable pulse test if complete, aborted or fault occurs
            if (Brg1TripFlt || L_SeqStat.L_DbPsTstSta || !L_DbPlsTstCmd || !PARM(L_BrgDbEnb) || (L_DbGateDsb && L_DbAutoTstDone))
            {
                L_ChgLvlDcDy          = PARM(L_ChgLvlDc);
                L_ChgLvlAcDy          = PARM(L_ChgLvlAc);
                L_SeqReq.L_DbPsTstReq = false;
                L_DbPlsTstCmd          = false;
                L_DbPwrEnb            = false;
                if (!L_SeqStat.L_DbPsTstSta && !L_SeqAct.L_DbPsTstAct)
                {
                    L_SeqSt = LINE_DIAGNOSTIC;
                }
            }
            else
            {
                L_ChgLvlDcDy          = PARM(L_ChgLvlDcDB);
                L_ChgLvlAcDy          = PARM(L_ChgLvlAcDB);
                L_DbPwrEnb            = true;  // Enable the DB
                L_SeqReq.L_DbPsTstReq = true;  // Set pulse test request
            }
            break;

        //-----------------------  DB_CELL_TEST --------------------------------
        case LINE_DB_CELL_TEST:
            // Disable cell test if complete, aborted or fault occurs
            if (L_SeqStat.L_DbClTstSta || !L_DbCelTstCmd || !PARM(L_BrgDbEnb) || (L_DbGateDsb && L_DbAutoTstDone))
            {
                L_ChgLvlDcDy          = PARM(L_ChgLvlDc);
                L_ChgLvlAcDy          = PARM(L_ChgLvlAc);
                L_SeqReq.L_DbClTstReq = false;
                L_DbCelTstCmd          = false;
                L_DbPwrEnb            = false;
                L_SeqReq.L_KdfEnbReq     =false;

                if (!L_SeqStat.L_DbClTstSta && !L_SeqAct.L_DbClTstAct)
                    L_SeqSt = LINE_DIAGNOSTIC;
            }
            else
            {
                if(L_DbCelTst.DbCapMsIp)
                {
                   L_ChgLvlDcDy       = PARM(L_ChgLvlDcCM);
                }
                else
                {
                   L_ChgLvlDcDy       = PARM(L_ChgLvlDcDB);
                }
                L_ChgLvlAcDy          = PARM(L_ChgLvlAcDB);
                L_DbPwrEnb            = true;  // Enable the bridge
                L_SeqReq.L_DbClTstReq = true;  // Set pulse test request
                if(L_DbCelTst.DbCtQ1OpnReq)
                {
                    L_SeqReq.L_Q1EnbReq=false;
                }
                if(L_DbCelTst.DbCtKdfOpnReq)
                {
                    L_SeqReq.L_KdfEnbReq=false;
                }

            }
            break;



        //------------------------  LINE_INIT ----------------------------------
        case LINE_INIT:  // Drive Initialization - wait for Task3 to complete
            if (L_RunCmd )
            {
                L_RunCmd = false;
                PUSH_DIAG(L_RunWhlInit); // Call warning fault
            }
            if (InitDone)
            {
                L_SeqSt    = LINE_STOPPED;
                L_InitDone = true;
            }

            L_PlsTstCmd = false;        // Always remove pulse test command is this state

            break;

        default:    // Invalid Sequencer State
            PUSH_DIAG(BadSeqSt);
            break;
    }   // End of switch(L_SeqSt)

    L_SeqCelTstReq =  L_SeqReq.L_CelTstReq ? true : false;  // Move request single bit to a boolean

    return;
}               // SourceCtrlSequencer


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This function populates that part of the keypad status structure
//      that is product specific.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineKeypadProductStatusWrite()

{

    CSDat.CtrlStatus.DiagModeBrg1  = L_SeqStBits.L_DiagMode;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Initializes core line sequencer
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void InitCoreLineSequencer(void)
{
    // Sequencer global variable initializations
    L_RunCmd      = false;
    L_SeqSt       = LINE_INIT;     // Set L_SeqState to LINE_INIT state

//  Initialize user functions

    return;
}


