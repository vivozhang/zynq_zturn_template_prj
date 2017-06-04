///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       2-level PWM modulator

//
// DESCRIPTION:
// This file defines the 2-level Asynchronous modulator class, which provides
// gating signals to bridge to yield 3-phase 2-level Voltage outputs in response
// to differential-mode & common-mode modulation commands.
// Theoretical modulation carrier is modeled as a triangle wave, synchronized to
// the execution task frame.  Configurability has been deliberately limited
// to streamline nominal functionality & execution time.  While not in saturation,
// nominally schedule one transition per phase per PWM interval ("PWM Single").
// Primary switching events are scheduled within the next frame period.
// Compile-time switch enables scheduling of Contingency switching events,
// which are overwritten by new Primary switching events, if execution is normal.
// Primary inputs are differential mode A,B & C phase modulation commands and
// external Common Mode modulation command, pre-compensated for total DC Link
// Voltage.  External CM command is added to internal injection signals, followed
// by compensation for expected device lockouts.
// Scaling and quantization is performed with integral preservation, hard limiting,
// offsetting and formatting for final output to the hardware modulator logic.
//
//
// Modulator nomenclature:
//   "Frame" = Primary time window in which to schedule bridge events
//   "Quant" = Discrete time packets by which to schedcule bridge events
//   "Count" = Discrete time clocks with which to schedule bridge events
//
// COPYRIGHT:   Copyright (c) 2011
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0PwmMod2Async.vsd
//
///////////////////////////////////////////////////////////////////////////////
#ifndef    B0_PWM_MOD_H
#define    B0_PWM_MOD_H

// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
#include "x0BridgeInterface.h"
// product
#include "Product.h"



// Compile-time Switches
// ---------------------
#define MOD_COMPILE_CONTINGENCY       // Enable contingency tansitions

//#define MOD_COMPILE_PWM_DBL           // Enable(*) PWM Double
                                        //     *ONLY Phase transition array length initialization

// Constants
//----------
#define PWM_CHOP_MIN            (1)                     // minimum transitions per phase per Task1 (not including init state)
#define PWM_CHOP_DEFAULT        (1)                     // default transitions per phase per Task1 (not including init state)
#define PWM_CHOP_MAX            (4)                     // maximum transitions per phase per Task1 (not including init state)

#define MAX_EVTS                (PWM_CHOP_MAX + 1)      // maximum transition per phase per Task1 (including the init state)
#define MAX_BRGS                (2)                     // maximum number of bridges supported

#define FPGA_EVENT_MARGIN_SEC   (1.0e-6)                // FPGA required scheduling margin in seconds

// Function Prototypes
//--------------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <class description>
//
///////////////////////////////////////////////////////////////////////////////
class cPwmMod2Async
{

//-----------------------------------------------------------------------------
public:

// Types and Classes
//------------------
struct PwmMod2AsyncInTyp
{
    const float&        UaCmd;                      //  1 - normalized phase modulation commamds (L-L scaling)
    const float&        UbCmd;                      //  2
    const float&        UcCmd;                      //  3
    const float&        U0Cmd;                      //  4 - normalized common mode mod commamds (L-L scaling)
    const float&        IaCmdLkout;                 //  5 - current commands for lockout compensation
    const float&        IbCmdLkout;                 //  6
    const float&        IcCmdLkout;                 //  7
    const unsigned&     PwmUp;                      //  8 - true = positive-going primary transitions
    const float&        UaAdj;                      //  9 - phase transition adjusts for parallel bridges (L-L modulation scaling)
    const float&        UbAdj;                      // 10
    const float&        UcAdj;                      // 11
    const unsigned&     CroBarAct;                  // 12 - Bridge CroBar Active
    const unsigned&     CroBarUpDn;                 // 13 - Bridge CroBar Up/Down
};

struct PwmMod2AsyncInsTyp
{
    const unsigned&     QuantPerFrame;              //  1 - # of quant per frame   (UFull)
    const unsigned&     CountPerQuant;              //  2 - # of count per quantum (PwmQuant)
    const float&        DelTm1;                     //  3 - Task1 delta time (seconds)
    const float&        LkoutCompVmax;              //  4 - max voltage LL for LkoutComp (VllMax)
    const float&        LkoutCompLeqv;              //  5 - equivalent inductance for LkoutComp   (henrys)
    const float&        LkoutCompTm;                //  6 - minimum time for LkoutComp (seconds)
    const unsigned&     LkoutCompDsb;               //  7 - disable LkoutComp (typically for SimMode)
    const unsigned&     CmModeInjDsb;               //  8 - disable common mode injection
    const unsigned&     SkipFireDsb;                //  9 - disable skip-fire (typically for SimMode)
    const unsigned&     DualBrgEnb;                 // 10 - enable calculations for dual bridge
};

    // Methods
    //--------
     cPwmMod2Async( PwmMod2AsyncInTyp InList, PwmMod2AsyncInsTyp InsList );
    ~cPwmMod2Async( void );

    int  RunRules( void );
    void Task1( void );


    // Output Signals
    //---------------
    CREATE_PUBVAR(ModPwmN,              float);         // Number of PWM pulses per Task1

    PhsTransListTyp PhsTransList;                       // Phase Transition List:
                                                        //     Length of Phase Transition Array,
                                                        //     Pointer to Phase Transition Array,
                                                        //     Pointer to Bridge Test Control Structure.
                                                        //
                                                        // Phase Transition Array, 2-dim: [trans][phase]
                                                        //     Transition Number (0=initial)
                                                        //     Phase Number
    PhsTransTyp     PhsTransArr[MAX_EVTS][NUM_OF_PHASES];

    BrgTstCtlTyp    BrgTstCtl;                          // Pointer to Bridge Test Control Structure
                                                        //      to access individual switches

                                                        // Parallel Bridge Additional Structures
                                                        // -------------------------------------
    PhsTransListTyp PhsTransListBrg2;                   // Phase Transition List Bridge2:
                                                        //     (same format as Phase Transition List)
                                                        //
                                                        // Phase Transition Array Bridge2, 2-dim: [trans][phase]
                                                        //     (same format as Phase Transition Array)
    PhsTransTyp     PhsTransArrBrg2[MAX_EVTS][NUM_OF_PHASES];

                                                        // Legacy Modulation Outputs (temporary?)
                                                        // -------------------------
    CREATE_PUBVAR(ModUaCmdM,            float);         // Modulation Outputs, compatible with Legacy Modulator
    CREATE_PUBVAR(ModUbCmdM,            float);         //
    CREATE_PUBVAR(ModUcCmdM,            float);         //

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;
    friend class cPwmMod2AsyncTest;


    // External Interfaces
    //--------------------
    PwmMod2AsyncInTyp         In;
    PwmMod2AsyncInsTyp        Ins;

    // Types
    //------
    struct ModTyp
    {
        unsigned        Phs;                            // id which phase this structure represents

        float           Ucmd;                           // phase modulation commands (l-l scaling)
        float           Ucmd1;                          // internal l-n commands
        float           Ucmd2;                          // lockout comp'd commands
        float           UcmdLkout;                      // normalized lockout commands
        float           IcmdLkout;                      // current commands for lockout comp

        int             Uskip;                          // skip-fire (slective half-phase off)

        float           Uquant;                         // comamnd quants
        float           UquantRes;                      // command quant residual
        int             UquantInt;                      // integral command quants

        int             Ucnt;                           // command counts

        float           Uadj;                           // phase transition adjusts (l-l modulation scaling)
        int             UcntAdj;                        // dual bridge adjustment
        int             UcntBrg1;                       // bridge 1 command counts adjusted for current sharing
        int             UcntBrg2;                       // bridge 2 command counts adjusted for current sharing
    };


    // Constants
    //----------

    // skip-fire
    #define SKIP_OFF               ( 0)                 // Skip Off
    #define SKIP_UP                ( 1)                 // Skip Upper
    #define SKIP_DN                (-1)                 // Skip Lower

    static const PhsTransTyp        TrInitEnb;
    static const PhsTransTyp        TrInitDsb;
    static const ModTyp             ModInit;


    // Methods
    //--------
    //void      ThirdHarmonicCalc   ( void );
    void        LockoutCalc         ( ModTyp&   mod );
    void        ModCmdToCnt         ( ModTyp&   mod );
    void        ModCntToTrans       ( ModTyp&   mod, unsigned brg );
    void        ModCroBarTrans      ( void );


    // Diagnostics
    //------------


    // Parameters
    //-----------
    CREATE_PARM(ModPwmChop,             int);           // Number of PWM chops per Task1

    CREATE_PARM(ModLkoutGnFactor,       float);         // Lockout compensation Gain Factor
    CREATE_PARM(ModLkoutChopFrq,        float);         // Lockout compensation Chopping freq

    CREATE_PARM(ModCmModeInjDsb,        unsigned);      // DISABLE Internal Common Mode Injection
    CREATE_PARM(ModTransAdjEnb,         unsigned);      // ENABLE  Transition Adjust
    CREATE_PARM(ModQuantDsb,            unsigned);      // DISABLE Pwm Quantization


    // Published Variables
    //--------------------
    CREATE_PUBVAR(ModCountPerQuant,       int);         // # Counts per Quantum (PwmQuant)
    CREATE_PUBVAR(ModQuantPerFrame,       int);         // # Quants per Frame   (UFull)
    CREATE_PUBVAR(ModHalfQPerFrame,     float);         // Half # Q per Frame   (Uhalf)
    CREATE_PUBVAR(ModCountPerFrame,       int);         // # Counts per Frame   (PwmFull)
    CREATE_PUBVAR(ModFramePerCount,     float);         // # Frames per Count   (for UCmdM's)
    CREATE_PUBVAR(ModCountPerAdj,       float);         // # Counts per Adjust  (LL modulation)
    CREATE_PUBVAR(ModCountMargin,         int);         // # Counts Margin from Frame boundaries

    CREATE_PUBVAR(ModPwmChopAct,          int);         // Active PwmChop

    CREATE_PUBVAR(ModCroBarAct,      unsigned);         // Modulator CroBar Active

    CREATE_PUBVAR(ModLkoutIThr,         float);         // lockout compensation current for full comp
    CREATE_PUBVAR(ModLkoutLim,          float);         // lockout compensation limit
    CREATE_PUBVAR(ModLkoutGain,         float);         // lockout compensation gain

    CREATE_PUBVAR(ModU0CmdExtrn,        float);         // common mode command externally-sourced
    CREATE_PUBVAR(ModU0CmdIntrn,        float);         // common mode command internally-sourced

    CREATE_PUBVAR(ModUaCmd1,            float);         // internal L-N commands
    CREATE_PUBVAR(ModUbCmd1,            float);         //
    CREATE_PUBVAR(ModUcCmd1,            float);         //

    CREATE_PUBVAR(ModUaCmd2,            float);         // lockout comp'd commands
    CREATE_PUBVAR(ModUbCmd2,            float);         //
    CREATE_PUBVAR(ModUcCmd2,            float);         //

    CREATE_PUBVAR(ModUaCmdLkout,        float);         // normalized lockout commands
    CREATE_PUBVAR(ModUbCmdLkout,        float);         //
    CREATE_PUBVAR(ModUcCmdLkout,        float);         //

    CREATE_PUBVAR(ModUaCnt,             int);           // Bridge    command counts
    CREATE_PUBVAR(ModUbCnt,             int);           //
    CREATE_PUBVAR(ModUcCnt,             int);           //

    CREATE_PUBVAR(ModUaCntBrg1,         int);           // Bridge(1) command counts
    CREATE_PUBVAR(ModUbCntBrg1,         int);           //
    CREATE_PUBVAR(ModUcCntBrg1,         int);           //

    CREATE_PUBVAR(ModUaCntBrg2,         int);           // Bridge(2) command counts
    CREATE_PUBVAR(ModUbCntBrg2,         int);           //
    CREATE_PUBVAR(ModUcCntBrg2,         int);           //



    // Un-published Variables
    //-----------------------
    ModTyp          ModA;                               // phase A variables
    ModTyp          ModB;                               // phase B variables
    ModTyp          ModC;                               // phase C variables



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Third Harmonic Calculations produce a component signal for
 //     Common Mode Injection to all three phase modulation commands.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
void ThirdHarmonicCalc ( void )
{
    unsigned  UcmdSeg;

    // Compute Segment (UCmdSeg) based upon relationships between pairs of
    // line-to-neutral modulation commands
    UcmdSeg = ( ModA.Ucmd >= ModB.Ucmd ? 4 : 0) |
              ( ModB.Ucmd >= ModC.Ucmd ? 2 : 0) |
              ( ModC.Ucmd >= ModA.Ucmd ? 1 : 0);

    switch ( UcmdSeg )
    {
        case 1:
        case 6:
            ModU0CmdIntrn = ModB.Ucmd;
            break;

        case 2:
        case 5:
            ModU0CmdIntrn = ModA.Ucmd;
            break;

        case 3:
        case 4:
            ModU0CmdIntrn = ModC.Ucmd;
            break;

        default:
            ModU0CmdIntrn = 0;
    }

    ModU0CmdIntrn *= 0.5;

    return;
}
/////////////////////////////////////////////////////////////////////////////

};

#endif  // B0_PWM_MOD_H


