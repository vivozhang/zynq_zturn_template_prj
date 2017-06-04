///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       2-level PWM modulator
//
// DESCRIPTION:
// This file contains member functions for the 2-level Asynchronous modulator class
//
// COPYRIGHT:   Copyright (c) 2011
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0PwmMod2Async.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
#include <math.h>
// framework
// product
#include "b0PwmMod2Async.h"


// Constants
//----------


// Externals
//----------


// Types and Classes
//------------------


// Parameters
//-----------


// Variables
//----------

//CREATE_PUBVAR(Ver_PwmMod2Async, WORD) = 0xMMmmpp;
//Class Version Code: MM=Major,mm=Minor,pp=Patch
//==================================================
CREATE_PUBVAR(Ver_PwmMod2Async, WORD) = 0x011300;
// v01.13.00: Simplify ModU(abc)CmdM values, coordinated with BrgTherm
// v01.12.01: (ModCntToTrans)Avoid scheduling at next load pulse
// v01.12.00: (Task1,ModCroBarTrans) Respond to bridge CroBar commands
// v01.11.00: (RunRules,CmdToCnts) Limit fine adjust to avoid frame boundaries
// v01.10.01: Remove (x0.5) in applying Uadj input to UcntAdj
// v01.10.00: Legacy modulation outputs: ModU(abc)CmdM
// v01.09.05: Replace "New" Modulator with this re-cast Markstat Modulator
// v01.09.04: PwmMod2Async1-->PwmMod2Async
// v01.09.03: QuantPerFrame & CountPerQuant type change (int)-->(unsigned)
// v01.09.02: (ModTest) Unit Test via connections only; All Observers to PostMod
// v01.09.01: Final mods regarding re-cast; conversions singularized
// v01.09.00: Re-cast for improved execution: RAS incorporates JMN work + fixes
// v01.08.01: Last Mod prior to JMN re-cast: Ua[bc]Adj; Simple saturate Adj Limits
//===============================================


// Unpublished Variables
//----------------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Static Class Members (init'd to zero by default)
//-------------------------------------------------
const PhsTransTyp               cPwmMod2Async::TrInitEnb  = { 0, PHS_ST_OFF,  true };
const PhsTransTyp               cPwmMod2Async::TrInitDsb  = { 0, PHS_ST_OFF,  false };
const cPwmMod2Async::ModTyp     cPwmMod2Async::ModInit    = { PHASE_A, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0, 0, 0, 0.0, 0, 0 };



// Member Functions
//-----------------

/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Constructor for 2-level PWM modulator class.
//      This function initalizes all data members.
//      Class interface aliases are initialized with structures of
//      references that are passed to this constructur.
//
// EXECUTION LEVEL
//      Class instancing
//
/////////////////////////////////////////////////////////////////////////////
cPwmMod2Async::cPwmMod2Async
(
    PwmMod2AsyncInTyp   InList,
    PwmMod2AsyncInsTyp  InsList
)
    : In(InList), Ins(InsList)
{

    //-------------------------------------------------------------------------
    // output signals
    OUT(ModPwmN) = 0.0;

    //-------------------------------------------------------------------------
    // init the phase transition array
    OUT( PhsTransList.pTrans ) = &PhsTransArr[0];          // pointer to the Phase Transition event array
    OUT( PhsTransList.pBrgTstCtl ) = &BrgTstCtl;           // pointer to the Bridge Test Control structure
    for ( unsigned tra = 0 ; tra < MAX_EVTS ; ++tra )
    {
        for ( unsigned phs = 0 ; phs < NUM_OF_PHASES ; ++phs )
        {
            OUT( PhsTransArr[tra][phs] ) = TrInitDsb;
        }
    }

    //-------------------------------------------------------------------------
    // init the phase transition array for parallel bridge
    OUT( PhsTransListBrg2.pTrans ) = &PhsTransArrBrg2[0];  // pointer to the Phase Transition event array
    OUT( PhsTransListBrg2.pBrgTstCtl ) = &BrgTstCtl;       // pointer to the Bridge Test Control structure
    for ( unsigned tra = 0 ; tra < MAX_EVTS ; ++tra )
    {
        for ( unsigned phs = 0 ; phs < NUM_OF_PHASES ; ++phs )
        {
            OUT( PhsTransArrBrg2[tra][phs] ) = TrInitDsb;
        }
    }

    //-------------------------------------------------------------------------
    // init the Bridge Test Control structure
    memset( (void*)&OUT(BrgTstCtl), 0, sizeof(BrgTstCtl) );

    //-------------------------------------------------------------------------
    // common mode internal signal
    ModU0CmdIntrn = 0.0;

    //-------------------------------------------------------------------------
    // lockout compensation
    ModLkoutLim = 0.0;
    ModLkoutGain = 0.0;
    ModLkoutIThr = 0.0;

    //-------------------------------------------------------------------------
    // init the per-phase structures
    ModA = ModInit;
    ModA.Phs = PHASE_A;

    ModB = ModInit;
    ModB.Phs = PHASE_B;

    ModC = ModInit;
    ModC.Phs = PHASE_C;

}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction
//
/////////////////////////////////////////////////////////////////////////////
cPwmMod2Async::~cPwmMod2Async(void)
{
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by the class.
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
int cPwmMod2Async::RunRules(void)
{
    int     Status = SUCCESS;

    //-------------------------------------------------------------------------
    // PWM Chops/Task1
    ModPwmChopAct = PARM(ModPwmChop);
    if (ModPwmChopAct < PWM_CHOP_MIN)
    {
        ModPwmChopAct = PWM_CHOP_MIN;
    }
    if (ModPwmChopAct > PWM_CHOP_MAX)
    {
        ModPwmChopAct = PWM_CHOP_MAX;
    }
    SET_FV(ModPwmChop,  ModPwmChopAct);

    //-------------------------------------------------------------------------
    // PWM Pulses/Task1
    OUT(ModPwmN) = ((float)ModPwmChopAct) / 2.0f;


    //-------------------------------------------------------------------------
    // Lockout compensation

    //     LkoutComp Chopping Frequency
    SET_FV(ModLkoutChopFrq, (float)ModPwmChopAct / 2.0 / INS(DelTm1));

    //     LkoutComp full-compensation phase current level
    //         VllMax       = maximum line-to-line output voltage
    //         Leqv         = net load inductance
    //         LkoutChopFrq = full cycle chopping frequency (Hz)
    //         0.77         = distortion factor
    ModLkoutIThr = 0.77 * ( SQRT2 * SQRT3_RCP * INS(LkoutCompVmax) )
                        / ( PI2 * PARM(ModLkoutChopFrq) * INS(LkoutCompLeqv) ) ;

    //     LkoutComp Gain Factor
    SET_FV(ModLkoutGnFactor, PARM(ModLkoutChopFrq) / ModLkoutIThr);

        //     LkoutComp Gain & Max limit
    //         2.0 = adjust for [-1,+1] to [0,1] U scaling shift
    ModLkoutGain = INS(LkoutCompTm) * PARM(ModLkoutGnFactor) * 2.0;
    ModLkoutLim  = INS(LkoutCompTm) * PARM(ModLkoutChopFrq) * 2.0;



    //-------------------------------------------------------------------------
    // Pwm Mod general parameters
    SET_FV(ModCmModeInjDsb,    INS(CmModeInjDsb));

    //-------------------------------------------------------------------------
    // Dual-Bridge & Transition Time Adjust
    SET_FV(ModTransAdjEnb, (INS(DualBrgEnb) ? true : false));

    //-------------------------------------------------------------------------
    // Modulation, Quantization & Time values
    SET_FV(ModQuantDsb, false);
    if ( PARM(ModQuantDsb) )
    {
        ModCountPerQuant = 1;
        ModQuantPerFrame = static_cast<int> (INS(CountPerQuant) * INS(QuantPerFrame));
    }
    else
    {
        ModCountPerQuant = static_cast<int> (INS(CountPerQuant));
        ModQuantPerFrame = static_cast<int> (INS(QuantPerFrame));
    }
    ModHalfQPerFrame = 0.5 * (float)ModQuantPerFrame;
    ModCountPerFrame = ModCountPerQuant * ModQuantPerFrame;
    ModFramePerCount = 1.0 / (float)ModCountPerFrame;
    ModCountPerAdj   = ModCountPerFrame * SQRT3_RCP;

    // Counts of Margin from Frame boundaries to avoid for Fine Adjustments
    ModCountMargin   = int( FPGA_EVENT_MARGIN_SEC * (float)ModCountPerFrame / INS(DelTm1) );

    //-------------------------------------------------------------------------
    // Initialize LENGTH of Phase Transition Lists, based on Configuration
    //     Bridge 1
#ifdef MOD_COMPILE_PWM_DBL
 #ifdef MOD_COMPILE_CONTINGENCY
    OUT( PhsTransList.Len ) = 5;
 #else
    OUT( PhsTransList.Len ) = 3;
 #endif
#else
 #ifdef MOD_COMPILE_CONTINGENCY
    OUT( PhsTransList.Len ) = 3;
 #else
    OUT( PhsTransList.Len ) = 2;
 #endif
#endif
    //     Bridge 2
    if ( INS(DualBrgEnb) )
    {
#ifdef MOD_COMPILE_PWM_DBL
 #ifdef MOD_COMPILE_CONTINGENCY
        OUT( PhsTransListBrg2.Len ) = 5;
 #else
        OUT( PhsTransListBrg2.Len ) = 3;
 #endif
#else
 #ifdef MOD_COMPILE_CONTINGENCY
        OUT( PhsTransListBrg2.Len ) = 3;
 #else
        OUT( PhsTransListBrg2.Len ) = 2;
 #endif
#endif
    }
    else
    {
        OUT( PhsTransListBrg2.Len ) = 0;
    }

    return (Status);
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//   Lockout compensation is based upon ABC current commands, derived from
//   rotated XY current commands.  These current commands are scaled
//   (LkOutGain), limited (per LkoutMaxLim), and added to phase modulation
//   commands, where [-1,+1] is the full modulation range.
//   Hence LkoutMaxLim defines the level of modulation required to fully-
//   compensate for voltage errors due to the assymmetry introduced by
//   lockout effects, and LkoutGain defines the characteristics of the
//   current-dependent transistion between positive to negative comps.
//
//   Skip-Fire involves the selective disabling of one-half phase,
//   based upon determination of the condition that conduction is expected
//   in the anti-parallel diode rather than the active switching device.
//   The Skip-Fire behavior is to inhibit the gating of the associated
//   half-phase, thus avoiding commanding the FPGA to transition between
//   Up & Down states (transitioning instead between Off & (Up or Down).
//   In this case, the FPGA will not enforce a Lockout Time, and therefore
//   the Voltage disturbance is avoided, and Lockout Comp should be zeroed.
//
// EXECUTION LEVEL
//      Task1
//
// INVOCATION
//      Per Phase
//
/////////////////////////////////////////////////////////////////////////////
void  cPwmMod2Async::LockoutCalc (
    ModTyp&         mod
)

{

    //-------------------------------------------------------------------------
    // add common mode and third harmonic
    mod.Ucmd1 = ( mod.Ucmd + ModU0CmdExtrn + ModU0CmdIntrn ) * 2.0 * SQRT3_RCP;

    //----------------------------------------------------------
    // handle skip-fire & calculate lockout compensation
    mod.UcmdLkout = mod.IcmdLkout * ModLkoutGain;
    mod.Uskip = SKIP_OFF;

    if ( mod.UcmdLkout >= ModLkoutLim )
    {
        mod.UcmdLkout = ModLkoutLim;
        if ( mod.Ucmd1 >  0.0 )
        {
            mod.Uskip = SKIP_DN;
        }
    }
    else if ( mod.UcmdLkout <= -ModLkoutLim )
    {
        mod.UcmdLkout = -ModLkoutLim;
        if ( mod.Ucmd1 <  0.0 )
        {
            mod.Uskip = SKIP_UP;
        }
    }

    if ( INS(SkipFireDsb) )
    {
        mod.Uskip = SKIP_OFF;
    }

    if ( INS(LkoutCompDsb) || (mod.Uskip != SKIP_OFF) )
    {
        mod.UcmdLkout = 0.0;
    }

    // apply lockout comp
    mod.Ucmd2 = mod.Ucmd1 + mod.UcmdLkout;

    return;
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Transform Modulation Command to Counts
//         Compensate for DC Link imbalance
//         Introduce deliberate Quantization
//         Provide Residuals & Reconstructions
//
// EXECUTION LEVEL
//      Task1
//
// INVOCATION
//      Per Phase
//
/////////////////////////////////////////////////////////////////////////////
void  cPwmMod2Async::ModCmdToCnt(
    ModTyp&         mod
)
{
    int UcntAdjLim;
    int UcntAdjLim1;
    int UcntAdjLim2;

    //-------------------------------------------------------------------------
    // convert modulation command to quants
    mod.Uquant = (mod.Ucmd2 + 1.0) * ModHalfQPerFrame;

    //-------------------------------------------------------------------------
    // Residuals are maintained in A-B-C frame;
    // A,B,C residuals from previous pass are added here.
     mod.Uquant += mod.UquantRes;

     //-------------------------------------------------------------------------
    // truncate to integral quants & residual
    //      The behavior of "floor" to truncate always towards negative infinity
    //      is preferred for consistency, independent of the signal value & sign.
    //      The residual calculation is robust against extreme signal values,
    //      which may exceed the range of representable integer counts.
    mod.UquantInt = (int)floorf(mod.Uquant);
    mod.UquantRes = mod.Uquant - (float)mod.UquantInt;

    //-------------------------------------------------------------------------
    //limit to [min,max]
    if ( mod.UquantInt < 0 )
    {
        mod.UquantInt = 0;
        mod.UquantRes = 0;
    }
    else if ( mod.UquantInt > ModQuantPerFrame )
    {
        mod.UquantInt = ModQuantPerFrame;
        mod.UquantRes = 0;
    }

    //-------------------------------------------------------------------------
    // convert quants to counts & initially populate both bridge commands
    mod.Ucnt     = mod.UquantInt * ModCountPerQuant;
    mod.UcntBrg1 = mod.Ucnt;
    mod.UcntBrg2 = mod.Ucnt;

    //-------------------------------------------------------------------------
    // Dual Bridge && Not on either frame boundary
    if ( PARM(ModTransAdjEnb) && (mod.Ucnt!=ModCountPerFrame) && (mod.Ucnt!= 0) )
    {
        //---------------------------------------------------------------------
        // adjustment limit to avoid getting too close to this frame’s load pulse
        UcntAdjLim1 = MAX(0,  mod.Ucnt - ModCountMargin);

        // adjustment limit to avoid getting too close to next frame’s load pulse
        UcntAdjLim2 = MAX(0,ModCountPerFrame - mod.Ucnt - ModCountMargin);

        // net adjust limit is the minimum of the limits
        UcntAdjLim = MIN(UcntAdjLim1, UcntAdjLim2);

        //---------------------------------------------------------------------
        // convert adjust to counts
        mod.UcntAdj = (int)( mod.Uadj * ModCountPerAdj );

        // limit count adjust
        if (mod.UcntAdj >  UcntAdjLim) mod.UcntAdj =  UcntAdjLim;
        if (mod.UcntAdj < -UcntAdjLim) mod.UcntAdj = -UcntAdjLim;

        // apply limited count adjust
        mod.UcntBrg1 += mod.UcntAdj;
        mod.UcntBrg2 -= mod.UcntAdj;
    }

    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Transform Modulation Counts to Transition Structures
//
// EXECUTION LEVEL
//      Task1
//
// INVOCATION
//      Per Phase Per Bridge
//
/////////////////////////////////////////////////////////////////////////////
void  cPwmMod2Async::ModCntToTrans(
    ModTyp&         mod,
    unsigned        brg
)
{
    unsigned        Phs = mod.Phs;

    int             Ucnt = ( (brg == 2) ? mod.UcntBrg2 : mod.Ucnt );

    PhsStateTyp     StateUp = ( (mod.Uskip == SKIP_UP) ? PHS_ST_OFF : PHS_ST_POS );
    PhsStateTyp     StateDn = ( (mod.Uskip == SKIP_DN) ? PHS_ST_OFF : PHS_ST_NEG );

    PhsTransTyp     Tr0 = TrInitEnb;
    PhsTransTyp     Tr1 = TrInitEnb;
#ifdef MOD_COMPILE_CONTINGENCY
    PhsTransTyp     Tr2 = TrInitEnb;
#endif


    //-------------------------------------------------------------------------
    // pwm up
    if ( IN(PwmUp) == 1 )
    {
        Tr1.DelTm = ModCountPerFrame - Ucnt;
        if (      Tr1.DelTm == 0 )
        {
            Tr0.State  = StateUp;
            Tr1.Enable = false;
#ifdef MOD_COMPILE_CONTINGENCY
            Tr2.Enable = false;
#endif
        }
        else if ( Tr1.DelTm == static_cast<unsigned>(ModCountPerFrame) )
        {
            Tr0.State  = StateDn;
            Tr1.Enable = false;
#ifdef MOD_COMPILE_CONTINGENCY
            Tr2.Enable = false;
#endif
        }
        else
        {
            Tr0.State = StateDn;
            Tr1.State = StateUp;
#ifdef MOD_COMPILE_CONTINGENCY
            Tr2.State = StateDn;
            Tr2.DelTm = ModCountPerFrame + Ucnt;
#endif
        }
    }

    //-------------------------------------------------------------------------
    // pwm down
    else
    {
        Tr1.DelTm = Ucnt;
        if (      Tr1.DelTm == 0 )
        {
            Tr0.State  = StateDn;
            Tr1.Enable = false;
#ifdef MOD_COMPILE_CONTINGENCY
            Tr2.Enable = false;
#endif
        }
        else if ( Tr1.DelTm == static_cast<unsigned>(ModCountPerFrame) )
        {
            Tr0.State  = StateUp;
            Tr1.Enable = false;
#ifdef MOD_COMPILE_CONTINGENCY
            Tr2.Enable = false;
#endif
        }
        else
        {
            Tr0.State = StateUp;
            Tr1.State = StateDn;
#ifdef MOD_COMPILE_CONTINGENCY
            Tr2.State = StateUp;
            Tr2.DelTm = ModCountPerFrame + ModCountPerFrame - Ucnt;
#endif
        }
    }

    // ------------------------------------------------------------------------
    // assign phase states & timers
    if ( brg == 2 )
    {
        OUT(PhsTransArrBrg2)[0][Phs] = Tr0;
        OUT(PhsTransArrBrg2)[1][Phs] = Tr1;
#ifdef MOD_COMPILE_CONTINGENCY
        OUT(PhsTransArrBrg2)[2][Phs] = Tr2;
#endif
    }
    else
    {
        OUT( PhsTransArr )[0][Phs] = Tr0;
        OUT( PhsTransArr )[1][Phs] = Tr1;
#ifdef MOD_COMPILE_CONTINGENCY
        OUT( PhsTransArr )[2][Phs] = Tr2;
#endif
    }

    return;
}

////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Bridge CroBar commands to Transition Structure
//      All UP or DOWN for entire Frame interval
//
// EXECUTION LEVEL
//      Task1
//
// INVOCATION
//      Once for all phases of entire single/dual-bridge
//
/////////////////////////////////////////////////////////////////////////////
void  cPwmMod2Async::ModCroBarTrans()
{
    PhsTransTyp     Tr0 = TrInitEnb;    // { 0, PHS_ST_OFF, true }
    PhsTransTyp     TrN = TrInitDsb;    // { 0, PHS_ST_OFF, false}

    if (IN(CroBarUpDn))
    {
        Tr0.State  = PHS_ST_POS;
    }
    else
    {
        Tr0.State  = PHS_ST_NEG;
    }

    OUT( PhsTransArr )[0][ModA.Phs] = Tr0;
    OUT( PhsTransArr )[0][ModB.Phs] = Tr0;
    OUT( PhsTransArr )[0][ModC.Phs] = Tr0;

    OUT( PhsTransArr )[1][ModA.Phs] = TrN;
    OUT( PhsTransArr )[1][ModB.Phs] = TrN;
    OUT( PhsTransArr )[1][ModC.Phs] = TrN;

    OUT( PhsTransArr )[2][ModA.Phs] = TrN;
    OUT( PhsTransArr )[2][ModB.Phs] = TrN;
    OUT( PhsTransArr )[2][ModC.Phs] = TrN;

    if ( INS(DualBrgEnb) )
    {
        OUT( PhsTransArrBrg2 )[0][ModA.Phs] = Tr0;
        OUT( PhsTransArrBrg2 )[0][ModB.Phs] = Tr0;
        OUT( PhsTransArrBrg2 )[0][ModC.Phs] = Tr0;

        OUT( PhsTransArrBrg2 )[1][ModA.Phs] = TrN;
        OUT( PhsTransArrBrg2 )[1][ModB.Phs] = TrN;
        OUT( PhsTransArrBrg2 )[1][ModC.Phs] = TrN;

        OUT( PhsTransArrBrg2 )[2][ModA.Phs] = TrN;
        OUT( PhsTransArrBrg2 )[2][ModB.Phs] = TrN;
        OUT( PhsTransArrBrg2 )[2][ModC.Phs] = TrN;
    }

}

/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This is the PRIMARY RUN-TIME Method for this Class, which is the
//      main executive of the tranasformation from Input Modulation Commands
//      (assumed to have been pre-compensated for total DC Link Voltages)
//      and Upper & Lower DC Link Voltage Feedbacks
//      into Bridge FPGA Event Commands (in the form of PWM states and timers).
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
void cPwmMod2Async::Task1(void)
{

    //-------------------------------------------------------------------------
    // inputs
    ModA.Ucmd      = IN(UaCmd);
    ModB.Ucmd      = IN(UbCmd);
    ModC.Ucmd      = IN(UcCmd);
    ModU0CmdExtrn  = IN(U0Cmd);

    ModA.IcmdLkout = IN(IaCmdLkout);
    ModB.IcmdLkout = IN(IbCmdLkout);
    ModC.IcmdLkout = IN(IcCmdLkout);

    if ( PARM(ModTransAdjEnb) )
    {
        ModA.Uadj = IN(UaAdj);
        ModB.Uadj = IN(UbAdj);
        ModC.Uadj = IN(UcAdj);
    }

    //-------------------------------------------------------------------------
    // internal common mode injection
    if ( !PARM(ModCmModeInjDsb) )
    {
        ThirdHarmonicCalc();
    }

    //-------------------------------------------------------------------------
    // lockout calculations
    LockoutCalc( ModA );
    LockoutCalc( ModB );
    LockoutCalc( ModC );

    //-------------------------------------------------------------------------
    // transform modulation command to FRC counts
    ModCmdToCnt( ModA );
    ModCmdToCnt( ModB );
    ModCmdToCnt( ModC );

    //-------------------------------------------------------------------------
    // populate the phase transition lists for output to the bridge interface
    if (IN(CroBarAct))
    {
        ModCroBarAct = true;
        ModCroBarTrans();
    }
    else
    {
        ModCroBarAct = false;
        ModCntToTrans( ModA, 1 );
        ModCntToTrans( ModB, 1 );
        ModCntToTrans( ModC, 1 );
        if ( INS(DualBrgEnb) )
        {
            ModCntToTrans( ModA, 2 );
            ModCntToTrans( ModB, 2 );
            ModCntToTrans( ModC, 2 );
        }
    }

    //-------------------------------------------------------------------------
    // populate the published variables
    ModUaCmd1     = ModA.Ucmd1;
    ModUbCmd1     = ModB.Ucmd1;
    ModUcCmd1     = ModC.Ucmd1;

    ModUaCmd2     = ModA.Ucmd2;
    ModUbCmd2     = ModB.Ucmd2;
    ModUcCmd2     = ModC.Ucmd2;

    ModUaCmdLkout = ModA.UcmdLkout;
    ModUbCmdLkout = ModB.UcmdLkout;
    ModUcCmdLkout = ModC.UcmdLkout;

    ModUaCnt      = ModA.Ucnt;
    ModUbCnt      = ModB.Ucnt;
    ModUcCnt      = ModC.Ucnt;
    if ( INS(DualBrgEnb) )
    {
        ModUaCntBrg1  = ModA.UcntBrg1;
        ModUbCntBrg1  = ModB.UcntBrg1;
        ModUcCntBrg1  = ModC.UcntBrg1;
        ModUaCntBrg2  = ModA.UcntBrg2;
        ModUbCntBrg2  = ModB.UcntBrg2;
        ModUcCntBrg2  = ModC.UcntBrg2;
    }

    //-------------------------------------------------------------------------
    // Legacy Modulation Outputs
    OUT(ModUaCmdM) = (float)ModUaCnt*ModFramePerCount;
    OUT(ModUbCmdM) = (float)ModUbCnt*ModFramePerCount;
    OUT(ModUcCmdM) = (float)ModUcCnt*ModFramePerCount;

    return;
}

