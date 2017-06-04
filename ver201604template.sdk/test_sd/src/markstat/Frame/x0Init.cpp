///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       MarkStat Controller Common Initialization
//
// DESCRIPTION:
//      This file contains the initialization and shutdown functions that
//      are common to all MarkStat controllers.  These functions make calls
//      to product specific initialization and shutdown functions which are
//      assumed to exist:
//          ProductInit
//          ProductShutdown
//          ProductTimebaseRules
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      List locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
#include <fpstatus.h>
#include <math.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>
// core
#include "FrcSched.h"
#include "Hardware.h"
#include "ParamString.h"
// framework
#include "x0Err.h"
#include "x0BridgeInterface.h"
#include "x0FrameWork.h"
#include "x0Sequencer.h"
#include "x0XsimInterface.h"
// product
#include "s0Sim.h"


// Constants
//----------
#define T1_FRAME_RATE_MIN           (700.0)     // min allowed Task1 rate (hz)
#define T1_FRAME_RATE_MAX           (7000.0)    // max allowed Task1 rate (hz)

#define T1_FRAMES_PER_T2_MIN        (2)         // min allowed Task1/Task2 ratio
#define T1_FRAMES_PER_T2_MAX        (10)        // max allowed Task1/Task2 ratio

#define T1A_FRAMES_PER_T2_MIN       (1)         // min allowed Task1A/Task2 ratio

#define T2_FRAMES_PER_T3_MIN        (5)         // min allowed Task2/Task3 ratio
#define T2_FRAMES_PER_T3_MAX        (20)        // max allowed Task2/Task3 ratio

#define T1_FRAMES_PER_MOD_1         (1)         // 1 Task1 frames/modulator ratio
#define T1_FRAMES_PER_MOD_2         (2)         // 2 Task1 frames/modulator ratio
#define T1_FRAMES_PER_MOD_4         (4)         // 4 Task1 frames/modulator ratio

#define T1A_FRAMES_PER_MOD_1        (1)         // 1 Task1A frames/modulator ratio
#define T1A_FRAMES_PER_MOD_2        (2)         // 2 Task1A frames/modulator ratio
#define T1A_FRAMES_PER_MOD_4        (4)         // 4 Task1A frames/modulator ratio

#define MOD_QUANT_MIN               ( 0.0e-6)   // min allowed modulator quant size (sec)
#define MOD_QUANT_MAX               (60.0e-6)   // max allowed modulator quant size (sec)

#define T1_FRAME_RATE_DEFAULT       (4000.0)    // 1/4000     = 250us Task1
#define T1_FRAMES_PER_T2_DEFAULT    (4)         // 4 * 250us  = 1ms   Task2
#define T2_FRAMES_PER_T3_DEFAULT    (10)        // 10 * 1ms   = 10ms  Task3
#define T3_FRAMES_PER_TN_DEFAULT    (10)        // 10 * 10ms  = 100s    arbitrary wrap point since no tasks below T3

#define T1_FRAMES_PER_MOD_DEFAULT   (1)         // 1 Task1 frames per modulator

#define QUANT_MIN_SEED              (1.0e-6)    // seed time for the quant search algorithm (sec)


// Externals
//----------
extern cMarkStatDiag        MarkStatDiag;
extern cXsimInterface       XsimInterface;

extern unsigned             XSimMode;

extern int                  DelSysTm;
extern SysTm_Typ            SysTm1;
extern SysTm_Typ            SysTm2;
extern SysTm_Typ            SysTm3;

extern float                OscTmr;
extern Code_ID_struct       Code_ID;

extern void handler_setup(void);
extern int  xopsInit(void);

// product defined startup/shutdown functions
extern void ProductInit(void);
extern void ProductShutdown(void);
extern int  ProductTimebaseRules(void);


// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(T1FrameRateReq,     float);         // requested Task1 frame rate (hz)
CREATE_PARM(T1FramesPerT2Req,   unsigned);      // requested Task1 frames per Task2
CREATE_PARM(T1AFramesPerT2Req,  unsigned);      // requested Task1A frames per Task2
CREATE_PARM(T2FramesPerT3Req,   unsigned);      // requested Task2 frames per Task3
CREATE_PARM(T1FramesPerModReq,  unsigned);      // requested Task1 frames per Async Modulator
CREATE_PARM(T1AFramesPerModReq, unsigned);      // requested Task1A frames per Async Modulator
CREATE_PARM(MinQuantReq,        float);         // requested minimum quant size (sec)

CREATE_PARM(T1TaskExeDlyTm,     float);         // Task1 sw execution delay from the Task1 frame boundary (sec)
CREATE_PARM(T1AExecMask,        WORD);          // execution mask for Task1A

CREATE_PARM(UnitType,           unsigned);      // General Units Selection

CREATE_PARM(TrndCurScl,         float);         // Toolbox trend current scale
CREATE_PARM(TrndVoltScl,        float);         // Toolbox trend voltage scale
CREATE_PARM(TrndFrqScl,         float);         // Toolbox trend frequency scale
CREATE_PARM(TrndSpdScl,         float);         // Toolbox trend speed scale
CREATE_PARM(TrndTempScl,        float);         // Toolbox trend temperature scale


// Published Variables
//--------------------
extern const float              FZero, FPosOne, FNegOne, UnusedFloat;    // constant must be given global scope
CREATE_PUBVAR(FPosOne,          const float)    =  1.0;
CREATE_PUBVAR(FNegOne,          const float)    = -1.0;
CREATE_PUBVAR(FZero,            const float)    =  0.0;
CREATE_PUBVAR(UnusedFloat,      const float)    =  0.0;

extern const int                IPosOne, INegOne, IZero, UnusedInt;     // constant must be given global scope
CREATE_PUBVAR(IPosOne,          const int)      =  1;
CREATE_PUBVAR(INegOne,          const int)      = -1;
CREATE_PUBVAR(IZero,            const int)      =  0;
CREATE_PUBVAR(UnusedInt,        const int)      =  0;

extern const unsigned           UnusedUInt;                             // constant must be given global scope
CREATE_PUBVAR(UnusedUInt,       const unsigned) =  0;

extern const WORD               UnusedWord;                             // constant must be given global scope
CREATE_PUBVAR(UnusedWord,       const WORD)     =  0;

extern const unsigned           UnusedBool;                             // constant must be given global scope
CREATE_PUBVAR(UnusedBool,       const unsigned) =  false;

extern const unsigned           True, False;                            // constant must be given global scope
CREATE_PUBVAR(True,             const unsigned) =  true;
CREATE_PUBVAR(False,            const unsigned) =  false;

CREATE_PUBVAR(FScratch1,        float);
CREATE_PUBVAR(FScratch2,        float);
CREATE_PUBVAR(FScratch3,        float);
CREATE_PUBVAR(FScratch4,        float);
CREATE_PUBVAR(IScratch1,        int);
CREATE_PUBVAR(IScratch2,        int);
CREATE_PUBVAR(IScratch3,        int);
CREATE_PUBVAR(IScratch4,        int);

CREATE_PUBVAR(ProdVer,          WORD);

CREATE_PUBVAR(DelTmQuant,       float);         // delta time - quant

CREATE_PUBVAR(DelTm1,           float);         // delta time - task 1
CREATE_PUBVAR(DelTm1A,          float);         // delta time - task 1A
CREATE_PUBVAR(DelTm2,           float);         // delta time - task 2
CREATE_PUBVAR(DelTm3,           float);         // Delta time - task 3

CREATE_PUBVAR(DelTm1Rcp,        float);         // delta time reciprocal - task 1
CREATE_PUBVAR(DelTm1ARcp,       float);         // delta time reciprocal - task 1A
CREATE_PUBVAR(DelTm2Rcp,        float);         // delta time reciprocal - task 2
CREATE_PUBVAR(DelTm3Rcp,        float);         // Delta time reciprocal - task 3

CREATE_PUBVAR(FrcClkToCpuClk,   float);
CREATE_PUBVAR(CpuClkToFrcClk,   float);
CREATE_PUBVAR(XopsIsrFrmOffFrc, unsigned);      // isr delay after frame boundary (frc clocks)
CREATE_PUBVAR(XopsIsrFrmOffCpu, unsigned);      // isr delay after frame boundary (cpu clocks)

CREATE_PUBVAR(FrcSchedMask,     WORD);          // the resolution to schedule frame boundaries

CREATE_PUBVAR(T1FramesPerT2,    int);           // number of T1 per T2
CREATE_PUBVAR(T1FramesPerT3,    int);           // number of T1 per T3
CREATE_PUBVAR(T1AFramesPerT2,   int);           // number of T1A per T2
CREATE_PUBVAR(T1AFramesPerT3,   int);           // number of T1A per T3
CREATE_PUBVAR(T2FramesPerT3,    int);           // number of T2 per T3
CREATE_PUBVAR(T3FramesPerTn,    int);           // no tasks below T3, but needed for consistency in the task state counter algorithm
CREATE_PUBVAR(T1FramesPerMod,   int);           // number of T1 per async modulator
CREATE_PUBVAR(T1AFramesPerMod,  int);           // number of T1A per async modulator

CREATE_PUBVAR(Clk0PerQuant,     unsigned);      // cpu clocks per modulator quant
CREATE_PUBVAR(Clk0PerT1,        unsigned);      // cpu clocks per T1 frame
CREATE_PUBVAR(Clk0PerT1A,       unsigned);      // cpu clocks per T1A frame
CREATE_PUBVAR(Clk0PerMod,       unsigned);      // cpu clocks per modulator frame
CREATE_PUBVAR(Clk0PerMod1A,     unsigned);      // cpu clocks per modulator 1A frame
CREATE_PUBVAR(QuantsPerMod,     unsigned);      // modulator quants per modulator frame
CREATE_PUBVAR(QuantsPerMod1A,   unsigned);      // modulator quants per modulator 1A frame
CREATE_PUBVAR(QuantsPerT1,      unsigned);      // modulator quants per T1 frame
CREATE_PUBVAR(QuantsPerT1A,     unsigned);      // modulator quants per T1A frame


// Unpublished Variables
//----------------------
cSequencerBase*         pSequencer = NULL;
cFrcSchedBase*          pFrcSched = NULL;

float                   CpuCps = 1.0;           // cpu cycles per second
float                   CpuCpsRcp = 1.0;        // cpu second per cycle
float                   CpuCpsRcpUsec = 1.0;    // cpu usec per cycle

float                   FrcFreq;                // free running counter (FRC) frequency

unsigned                TimebaseCalcErr = 0;

unsigned                gVerbosityLevel = 0;

ProductStatusAddrTyp    ProdStatIf = { &False, &False, &False, &False, &False, &False, &False, &False, &False, };

PlatformTypeEnum        PlatformType = ERROR_PLATFORM;






///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function initializes pointers to product status indicator
//      variables.  These allow the framework to be aware of product
//      status without knowing the product variable names.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void ProductFrameworkInit(ProductStatusAddrTyp ProductStatusAddr)
{
    ProdStatIf = ProductStatusAddr;

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function is for miscelleneous initialization functions common
//      to all members of the product family.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void CommonInit(void)
{

    OscTmr   = PARM(OscDownTm);

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      Status - SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
void FrcHardwareInit(void)
{

    //--------------------------------------------------------------------------------------------------------------------------
    // get CPU clock cycles per second for this system
    CpuCps = (float)SYSPAGE_ENTRY(qtime)->cycles_per_sec;
    CpuCpsRcp = 1.0F / CpuCps;
    CpuCpsRcpUsec = 1.0e6F / CpuCps;

    //--------------------------------------------------------------------------------------------------------------------------
    // determine the hardware exectuion platform
    PlatformType = cHardwareBase::DeterminePlatformType(NULL,NULL);

    switch ( PlatformType )
    {

        //---------------------------------------------------------------------
        case UCCC_PLATFORM:
        //  - use the frc resource on the CSLA fpga, associated with hssl link #1
        //  - the CSLA frc must be set to 'internal' sync since there is no way for it to sync to the EPMC frc
        //  - if unable to allocate the CSLA frc resource, attempt to allocate the EPMC frc resource
        //  - on the EPMC we must use function #1 frc compare register #2 to avoid conflict with SYNCD (which uses 0 and 1)
        //---------------------------------------------------------------------

            MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                cParamString("UCCC platform detected").c_str()),
                HERE );
            MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                cParamString("CSLA frc hssl#1 will be allocated for xopsScheduler").c_str()),
                HERE );

            // instance the CSLA frc scheduling class
            pFrcSched = new (std::nothrow) cFrcSchedCsla();
            if ( pFrcSched == NULL )
            {
                MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                    cParamString("Unable to allocate CSLA frc resource").c_str()),
                    HERE );
            }
            else if ( pFrcSched->Init(true) == -1 )    // OK TO INIT HARDWARE SINCE ONLY THIS PROCESS IS USING IT
            {
                MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                    cParamString("Unable to initialize CSLA frc resource").c_str()),
                    HERE );
                delete pFrcSched;
                pFrcSched = NULL;
            }
            else
            {
                pFrcSched->SelectSyncSrc(FrcSyncIntern);   // set FRC clock for internal operation since no external FRC to sync to
                MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                    cParamString("CSLA frc hssl#1 successfully allocated for xopsScheduler").c_str()),
                    HERE );
                break;
            }

            // if we have gotten here, attempt to allocate CSLA frc resource hase failed
            // instance the EPMC frc scheduling class instead
            MarkStatDiag.LogEventMessage( 1, DIAG_SYSTEM, cParamString(MKS_INIT_STR,
                cParamString("EPMC frc will be allocated for xopsScheduler").c_str()),
                HERE );
            pFrcSched = new (std::nothrow) cFrcSchedEpmc();
            if ( pFrcSched == NULL )
            {
                MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                    cParamString("Unable to allocate EPMC frc resource").c_str()),
                    HERE );
            }
            else if ( pFrcSched->Init(false) == -1 )   // DONT INIT HARDWARE (disable ints) OR ELSE SYNCD WILL NOT WORK !!!!!
            {
                MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                    cParamString("Unable to initialize EPMC frc resource").c_str()),
                    HERE );
                delete pFrcSched;
                pFrcSched = NULL;
            }
            else if ( pFrcSched->SelectCmpNum(2) == -1 )
            {

                MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                    cParamString("Unable to initialize EPMC frc counter #2 resource").c_str()),
                    HERE );
                delete pFrcSched;
                pFrcSched = NULL;
            }
            else
            {
                MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                    cParamString("EPMC frc successfully allocated for xopsScheduler").c_str()),
                    HERE );
                break;
            }

            // if we have gotten here all attempts to allocate an frc resource have failed
            MarkStatDiag.LogEventMessage( 1, DIAG_SYSTEM, cParamString(MKS_INIT_STR,
                cParamString("xopsScheduler will be attached to the system clock").c_str()),
                HERE );
            break;

        //---------------------------------------------------------------------
        case UCSB_PLATFORM:
        //  - use the frc resource on the UCSB fpga (function #1)
        //  - on the UCSB we must use function #1 frc compare register #2 to avoid conflict with SYNCD (which uses 0 and 1)
        //  - the CSLA frc defaults to 'external' sync, allowing it to sync to the UCSB frc
        //---------------------------------------------------------------------

            MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                cParamString("UCSB platform detected").c_str()),
                HERE );
            MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                cParamString("UCSB frc funct#1 will be allocated for xopsScheduler").c_str()),
                HERE );

            // instance the UCSB frc scheduling class
            pFrcSched = new (std::nothrow) cFrcSchedUcsb();
            if ( pFrcSched == NULL )
            {
                MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                    cParamString("Unable to allocate UCSB frc resource").c_str()),
                    HERE );
            }
            else if ( pFrcSched->Init(false) == -1 )   // DONT INIT HARDWARE (disable ints) OR ELSE SYNCD WILL NOT WORK !!!!!
            {
                MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                    cParamString("Unable to initialize UCSB frc resource").c_str()),
                    HERE );
                delete pFrcSched;
                pFrcSched = NULL;
            }
#ifdef XOPS_EX2100_SCHED_METHOD
            else if ( pFrcSched->SelectCmpNum(0) == -1 )
#else
            else if ( pFrcSched->SelectCmpNum(2) == -1 )
#endif
            {
                MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                    cParamString("Unable to initialize UCSB frc counter #2 resource").c_str()),
                    HERE );
                delete pFrcSched;
                pFrcSched = NULL;
            }
            else
            {
                MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
                    cParamString("UCSB frc funct#1 successfully allocated for xopsScheduler").c_str()),
                    HERE );
                break;
            }

            // if we have gotten here all attempts to allocate an frc resource have failed
            MarkStatDiag.LogEventMessage( 1, DIAG_SYSTEM, cParamString(MKS_INIT_STR,
                cParamString("xopsScheduler will be attached to the system clock").c_str()),
                HERE );
            break;

        //---------------------------------------------------------------------
        default:
        //  - a supported hardware platform was not detected
        //---------------------------------------------------------------------
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                cParamString("Unsupported hardware platform (UCSB or UCCC required)").c_str()),
                HERE );
            MarkStatDiag.LogEventMessage( 1, DIAG_SYSTEM, cParamString(MKS_INIT_STR,
                cParamString("xopsScheduler will be attached to the system clock").c_str()),
                HERE );
            pFrcSched = NULL;
            break;
    }

    //-------------------------------------------------------------------------
    // assign FrcFreq from the hardware
    if ( pFrcSched != NULL )
    {
        FrcFreq = (float)pFrcSched->GetCountsPerSec();
    }
    else
    {
        FrcFreq = 25.0e6;
    }

    //-------------------------------------------------------------------------
    // convert between frc counts to cpu clock counts
    FrcClkToCpuClk = CpuCps / FrcFreq;
    CpuClkToFrcClk = FrcFreq / CpuCps;

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function calculates some basic quantities used to program the Timebase.
//      These quantities primarily define the lockout times for the modulator and the
//      number of 25Mhz clock tics in a quanta.  It is intended that both converters
//      will use this caculation.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      Status - SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int TimebaseInit()
{
    float       IsrRate;
    unsigned    Clk0PerQuantMax;
    float       QuantSeed;
    int         Status = SUCCESS;


    //-------------------------------------------------------------------------
    // run the product timebase rules
    if ( ProductTimebaseRules() == FAIL )
    {
        Status = FAIL;
        MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
            cParamString("Product timebase rule failure").c_str()),
            HERE );
    }
    else
    {
        // check validity of product timebase settings
        if ( (PARM(T1FrameRateReq) < (float)T1_FRAME_RATE_MIN) || (PARM(T1FrameRateReq) > (float)T1_FRAME_RATE_MAX) )
        {
            Status = FAIL;
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                cParamString("Invalid timebase T1FrameRateReq (%f)", PARM(T1FrameRateReq)).c_str()),
                HERE );
        }

        if ( (PARM(T1FramesPerT2Req) < T1_FRAMES_PER_T2_MIN) || (PARM(T1FramesPerT2Req) > T1_FRAMES_PER_T2_MAX) )
        {
            Status = FAIL;
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                cParamString("Invalid timebase T1FramesPerT2Req (%d)", PARM(T1FramesPerT2Req)).c_str()),
                HERE );
        }

        if ( (PARM(T2FramesPerT3Req) < T2_FRAMES_PER_T3_MIN) || (PARM(T2FramesPerT3Req) > T2_FRAMES_PER_T3_MAX) )
        {
            Status = FAIL;
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                cParamString("Invalid timebase T2FramesPerT3Req (%d)", PARM(T2FramesPerT3Req)).c_str()),
                HERE );
        }

        if ( PARM(T1AFramesPerT2Req) < T1A_FRAMES_PER_T2_MIN || PARM(T1AFramesPerT2Req) > PARM(T1FramesPerT2Req) )
        {
            Status = FAIL;
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                cParamString("Invalid timebase T1AFramesPerT2Req (%d)", PARM(T1AFramesPerT2Req)).c_str()),
                HERE );
        }

        if ( (PARM(T1FramesPerModReq) != T1_FRAMES_PER_MOD_1) && (PARM(T1FramesPerModReq) != T1_FRAMES_PER_MOD_2) && (PARM(T1FramesPerModReq) != T1_FRAMES_PER_MOD_4) )
        {
            Status = FAIL;
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                cParamString("Invalid timebase T1FramesPerModReq (%d)", PARM(T1FramesPerModReq)).c_str()),
                HERE );
        }

        if ( (PARM(T1AFramesPerModReq) != T1A_FRAMES_PER_MOD_1) && (PARM(T1AFramesPerModReq) != T1A_FRAMES_PER_MOD_2) && (PARM(T1AFramesPerModReq) != T1A_FRAMES_PER_MOD_4) )
        {
            Status = FAIL;
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                cParamString("Invalid timebase T1AFramesPerModReq (%d)", PARM(T1AFramesPerModReq)).c_str()),
                HERE );
        }

        if ( (PARM(MinQuantReq) < (float)MOD_QUANT_MIN)|| (PARM(MinQuantReq) > (float)MOD_QUANT_MAX) )
        {
            Status = FAIL;
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                cParamString("Invalid timebase MinQuantReq (%f)", PARM(MinQuantReq)).c_str()),
                HERE );
        }
    }

    //-------------------------------------------------------------------------
    // default the timebase settings if any problems with product timebase
    if ( Status == FAIL )
    {
        // use the default timebase settings
        IsrRate         = T1_FRAME_RATE_DEFAULT;
        T1FramesPerT2   = T1_FRAMES_PER_T2_DEFAULT;
        T1FramesPerT3   = T1_FRAMES_PER_T2_DEFAULT * T2_FRAMES_PER_T3_DEFAULT;
        T1AFramesPerT2  = T1_FRAMES_PER_T2_DEFAULT;
        T1AFramesPerT3  = T1_FRAMES_PER_T2_DEFAULT * T2_FRAMES_PER_T3_DEFAULT;
        T2FramesPerT3   = T2_FRAMES_PER_T3_DEFAULT;
        T1FramesPerMod  = T1_FRAMES_PER_MOD_DEFAULT;
        T1AFramesPerMod = T1_FRAMES_PER_MOD_DEFAULT;
        T3FramesPerTn   = T3_FRAMES_PER_TN_DEFAULT;
        QuantSeed       = QUANT_MIN_SEED;
        MarkStatDiag.LogEventMessage( 1, DIAG_SYSTEM, cParamString(MKS_INIT_STR,
            cParamString("Timebase assigned to default values").c_str()),
            HERE );
    }
    else
    {
        // use the product timebase settings
        IsrRate         = PARM(T1FrameRateReq);
        T1FramesPerT2   = PARM(T1FramesPerT2Req);
        T1FramesPerT3   = PARM(T1FramesPerT2Req) * PARM(T2FramesPerT3Req);
        T1AFramesPerT2  = PARM(T1AFramesPerT2Req);
        T1AFramesPerT3  = PARM(T1AFramesPerT2Req) * PARM(T2FramesPerT3Req);
        T2FramesPerT3   = PARM(T2FramesPerT3Req);
        T1FramesPerMod  = PARM(T1FramesPerModReq);
        T1AFramesPerMod = PARM(T1AFramesPerModReq);
        T3FramesPerTn   = T3_FRAMES_PER_TN_DEFAULT;
        QuantSeed       = MAX( (float)QUANT_MIN_SEED, PARM(MinQuantReq) );
    }

    //-------------------------------------------------------------------------
    // seed the initial quant size and assign max value for iterative search
#ifdef XOPS_EX2100_SCHED_METHOD
    Clk0PerQuant = 0;
#else
    Clk0PerQuant = (unsigned)ceil( QuantSeed * FrcFreq ) - 1;           // -1 because first pass iteration will ++
#endif
    Clk0PerQuantMax = Clk0PerQuant + 100;

    // iterativly increase the quant size increments of FRC_CLK in order to find the smallest
    //  quant that satisfies the conditions described below
    do
    {
        // check to see if we have tested the last value in the search range
        if ( Clk0PerQuant >= Clk0PerQuantMax )
        {
            TimebaseCalcErr = TRUE;
            Status = FAIL;
            MarkStatDiag.LogErrorMessage( TRIP_MSG, cParamString(MKS_INIT_STR,
                cParamString("No solution found for timbase quant calculation").c_str()), HERE );
            break;
        }

        // try the next larger quant size
        ++Clk0PerQuant;

        // force integral number of (quants / modulator frame) based on proposed quant size and target interrupt rate
        QuantsPerMod   = (int)floorf( ( FrcFreq / (IsrRate * Clk0PerQuant / T1FramesPerMod) ) + 0.5F);
        Clk0PerMod     = Clk0PerQuant * QuantsPerMod;
        Clk0PerT1      = Clk0PerMod / T1FramesPerMod;

        Clk0PerT1A     = Clk0PerT1 * T1FramesPerT2 / T1AFramesPerT2;
        Clk0PerMod1A   = Clk0PerT1A * T1AFramesPerMod;
        QuantsPerMod1A = Clk0PerMod1A / Clk0PerQuant;

        #if 0
        continue;   // this line FOR TEST ONLY ... uncomment to force 'TimbaseErr' fault
        #endif

        // test if Modulator1A Frame is an integral number of modulator quants
        if ( (Clk0PerQuant * QuantsPerMod1A) != Clk0PerMod1A )
            continue;

        // test if task1 is an even number of clk0s
        if ( (Clk0PerT1 * T1FramesPerMod) != Clk0PerMod )
            continue;

        // test if task1A is an even number of clk0s
        if ( (Clk0PerT1A * T1AFramesPerMod) != Clk0PerMod1A )
            continue;

        // test if Modulator Frame is an integral number of scheduling quant
        if ( (Clk0PerMod & FrcSchedMask) != Clk0PerMod )
            continue;

        // test if Modulator1A Frame is an integral number of scheduling quant
        if ( (Clk0PerMod1A & FrcSchedMask) != Clk0PerMod1A )
            continue;

        // test if quant is an integral number of scheduling quant
        if ( (Clk0PerQuant & FrcSchedMask) != Clk0PerQuant )
            continue;

        // test if Task1 is an integral number of scheduling quant
        if ( (Clk0PerT1 & FrcSchedMask) != Clk0PerT1 )
            continue;

        // test if Task1A is an integral number of scheduling quant
        if ( (Clk0PerT1A & FrcSchedMask) != Clk0PerT1A )
            continue;

        // we have a winner (if we got to here then all criteria are satified)
        break;

    } while (1);

    // calculate the del times
    DelTmQuant = (double)(Clk0PerQuant                             ) / FrcFreq;
    DelTm1     = (double)(Clk0PerT1                                ) / FrcFreq;
    DelTm2     = (double)(Clk0PerT1 * T1FramesPerT2                ) / FrcFreq;
    DelTm3     = (double)(Clk0PerT1 * T1FramesPerT2 * T2FramesPerT3) / FrcFreq;

    DelTm1A    = DelTm1 * (double)T1FramesPerT2 / (double)T1AFramesPerT2;

    // calculate value used to increment system time in the xops ISR
    DelSysTm     = (int)floorf( (DelTm1 * 1e9F) + 0.5F);    // DelTm1 in nsec

    // calculate frequencies from the delta times
    DelTm1Rcp  = 1.0 / DelTm1;
    DelTm1ARcp = 1.0 / DelTm1A;
    DelTm2Rcp  = 1.0 / DelTm2;
    DelTm3Rcp  = 1.0 / DelTm3;

    QuantsPerT1  = QuantsPerMod   / T1FramesPerMod;
    QuantsPerT1A = QuantsPerMod1A / T1AFramesPerMod;

    XopsIsrFrmOffFrc = PARM(T1TaskExeDlyTm) * FrcFreq;   // isr delay from frame boundary (frc counts)
    XopsIsrFrmOffCpu = PARM(T1TaskExeDlyTm) * CpuCps;    // isr delay from frame boundary (cpu counts)
    
    //-------------------------------------------------------------------------
    // done
    return (Status);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the necessary function to shutdown the
//      MarkStat process in an orderly fashion.  A product supplied shutodwn
//      procedure is called to allow product specific shutdown cutomization.
//
// EXECUTION LEVEL
//      Main @ shutdown
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ControllerShutdown( void )
{

    ProductShutdown();
    XsimInterface.TerminateConnection();

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function is called by MarkStat.Main to perform initialization
//      functions for the MarkStat process.
//
// EXECUTION LEVEL
//      Main @ shutdown
//
// RETURN VALUE
//      Status - SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int ControllerInit( void )
{
    int     Status = SUCCESS;

    ProdVer = Code_ID.version;

    //-------------------------------------------------------------------------
    // setup a handler for qnx segment violations (SIGSEGV)
    handler_setup();

    //-------------------------------------------------------------------------
    // allocate FRC hardware for xops interrupt generation
    MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
        cParamString("Start frc Hardware init").c_str()), HERE );
    FrcHardwareInit();

    //-------------------------------------------------------------------------
    //#warning Check for reboot due to previous watchdog TBD

    //-------------------------------------------------------------------------
    // Parameter initialization
    MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
        cParamString("Start parameter init").c_str()), HERE );
    gParameter.Initialize();

    //-------------------------------------------------------------------------
    // Final Value initialization (includes a call to InitialRules()
    MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
        cParamString("Start final value init").c_str()), HERE );
    if ( gParameter.FvInitialize() == FAIL )    Status = FAIL;

    //-------------------------------------------------------------------------
    // Miscellaneous Functions Initialzation Sequence
    MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
        cParamString("Start common init").c_str()), HERE );
    CommonInit();

    //-------------------------------------------------------------------------
    // Foreground Task Initialization Sequence
    MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
        cParamString("Start product init").c_str()), HERE );
    ProductInit();           // Task1-specific Functions

    //-------------------------------------------------------------------------
    // Xops Initiailization which creates all other markstat process threads
    MarkStatDiag.LogEventMessage( 1, DIAG_PROCESS, cParamString(MKS_INIT_STR,
        cParamString("Start xops init").c_str()), HERE );
    if ( xopsInit() == FAIL )   Status = FAIL;

    //-------------------------------------------------------------------------
    // done
    return (Status);
}
