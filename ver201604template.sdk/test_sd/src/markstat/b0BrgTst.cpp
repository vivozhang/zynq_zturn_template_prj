///////////////////////////////////////////////////////////////////////////////
//
// TITLE        2-level Bridge Test base class
//
// DESCRIPTION
//      This file contains the methods for the 2-level Bridge Test class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0BrgTst.vsd, b0BrgTst.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
// product
#include "b0BrgTst.h"


// Ver_BrgTst = 0xMMmmpp  --  Class Version Code: MM=Major,mm=Minor,pp=Patch
//--------------------------------------------------------------------------
CREATE_PUBVAR(Ver_BrgTst, WORD) = 0x010100;
// -First 2-level bridge test class release, adapted from 3-level.


// Constants
//----------


// Externals
//----------


// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Variables
//----------


// Member Functions
//-----------------



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Base class constructor to initialize resources
//
// EXECUTION LEVEL
//      Derived class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cBridgeTest::cBridgeTest
(
        InsListTyp  InsList
)
        : Ins(InsList)
{
    OUT(PhsTransList.Len)        = BRGTST_NUM_OF_TRA_PER_PHS;
    OUT(PhsTransList.pTrans)     = &PhsTransArr[0];  // pointer to pointer to 1st row in a 2-dim array
    OUT(PhsTransList.pBrgTstCtl) = &BrgTestControl;

    for ( unsigned phs = 0 ; phs < NUM_OF_PHASES ; phs++ )
    {
        for ( unsigned tra = 0 ; tra < PhsTransList.Len ; tra++ )
        {
            OUT(PhsTransArr[tra][phs].DelTm)  = 0;
            OUT(PhsTransArr[tra][phs].State)  = PHS_ST_OFF;
            OUT(PhsTransArr[tra][phs].Enable) = (tra == 0) ? true : false;
        }
    }

    memset( static_cast<void*> (&OUT(BrgTestControl)), 0 , sizeof(BrgTestControl) );
    memset( static_cast<void*> (&PhsTstCtrl),          0 , sizeof(PhsTstCtrl) );

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Base class destructor to free used resources.
//
// EXECUTION LEVEL
//      Derived instance destruction.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cBridgeTest::~cBridgeTest ()
{
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Configures the park and pulse states for each phase.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      ValidCfg (true/false)
//
///////////////////////////////////////////////////////////////////////////////
unsigned cBridgeTest::BridgeCfg
(
    // Inputs
        unsigned         ParkPhs,     // phase(s) to park
        PhsStateTyp      ParkSt,      // park state
        unsigned         PulsePhs,    // phase(s) to pulse
        PhsStateTyp      PulseSt      // pulse state
    // Outputs - None
)
{
    unsigned  ValidCfg;

    ValidCfg = true;

     // Check for valid bridge configuration parameters

    if (      ( ParkPhs < PHS_ENUM_MIN )  || ( ParkPhs > PHS_ENUM_MAX )
           || ( PulsePhs < PHS_ENUM_MIN ) || ( PulsePhs > PHS_ENUM_MAX )
           || ( ParkSt < ST_ENUM_MIN )    || ( ParkSt > ST_ENUM_MAX )
           || ( PulseSt < ST_ENUM_MIN )   || ( PulseSt > ST_ENUM_MAX )   )
    {
        ValidCfg = false;
    }
    else    // Configure phase A,B,C park and pulse states
    {
        if ( ParkPhs & PHS_A )
        {
            PhsTstCtrl.ParkStA  = ParkSt;
        }
        else
        {
            PhsTstCtrl.ParkStA  = PHS_ST_OFF;
        }

        if ( ParkPhs & PHS_B )
        {
            PhsTstCtrl.ParkStB  = ParkSt;
        }
        else
        {
            PhsTstCtrl.ParkStB  = PHS_ST_OFF;
        }

        if ( ParkPhs & PHS_C )
        {
            PhsTstCtrl.ParkStC  = ParkSt;
        }
        else
        {
            PhsTstCtrl.ParkStC  = PHS_ST_OFF;
        }

        if ( PulsePhs & PHS_A )
        {
            PhsTstCtrl.PulseStA = PulseSt;
        }
        else
        {
            PhsTstCtrl.PulseStA = PhsTstCtrl.ParkStA;
        }

        if ( PulsePhs & PHS_B )
        {
            PhsTstCtrl.PulseStB = PulseSt;
        }
        else
        {
            PhsTstCtrl.PulseStB = PhsTstCtrl.ParkStB;
        }

        if ( PulsePhs & PHS_C )
        {
            PhsTstCtrl.PulseStC = PulseSt;
        }
        else
        {
            PhsTstCtrl.PulseStC = PhsTstCtrl.ParkStC;
        }
    }

    return ValidCfg;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Configures the PWM registers to turn off all devices in the bridge
//      for a full PWM loop time.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeTest::BridgeOff
(
    // Inputs - None
    // Outputs
        unsigned& TstBridgeOff  // entire bridge devices configured to turn off
)
{
    for ( unsigned tra = 0 ; tra < PhsTransList.Len ; tra++ )
    {
        for ( unsigned phs = 0 ; phs < NUM_OF_PHASES ; phs++ )
        {
            OUT(PhsTransArr[tra][phs].DelTm)  = 0;
            OUT(PhsTransArr[tra][phs].State)  = PHS_ST_OFF;
            OUT(PhsTransArr[tra][phs].Enable) = (tra == 0) ? true : false;
        }
    }

    TstBridgeOff = true;

    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Configures the PWM registers to keep the bridge in park state for a
//      full PWM loop time.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeTest::BridgePark
(
    // Inputs - None
    // Outputs
        unsigned&        TstBridgeOff    // entire bridge devices configured to turn off
)
{
    OUT(PhsTransArr[0][PHASE_A].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_A].State)  = PhsTstCtrl.ParkStA;
    OUT(PhsTransArr[0][PHASE_A].Enable) = true;
    OUT(PhsTransArr[1][PHASE_A].DelTm)  = 0;
    OUT(PhsTransArr[1][PHASE_A].State)  = PhsTstCtrl.ParkStA;
    OUT(PhsTransArr[1][PHASE_A].Enable) = false;
    OUT(PhsTransArr[2][PHASE_A].DelTm)  = 0;
    OUT(PhsTransArr[2][PHASE_A].State)  = PhsTstCtrl.ParkStA;
    OUT(PhsTransArr[2][PHASE_A].Enable) = false;

    OUT(PhsTransArr[0][PHASE_B].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_B].State)  = PhsTstCtrl.ParkStB;
    OUT(PhsTransArr[0][PHASE_B].Enable) = true;
    OUT(PhsTransArr[1][PHASE_B].DelTm)  = 0;
    OUT(PhsTransArr[1][PHASE_B].State)  = PhsTstCtrl.ParkStB;
    OUT(PhsTransArr[1][PHASE_B].Enable) = false;
    OUT(PhsTransArr[2][PHASE_B].DelTm)  = 0;
    OUT(PhsTransArr[2][PHASE_B].State)  = PhsTstCtrl.ParkStB;
    OUT(PhsTransArr[2][PHASE_B].Enable) = false;

    OUT(PhsTransArr[0][PHASE_C].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_C].State)  = PhsTstCtrl.ParkStC;
    OUT(PhsTransArr[0][PHASE_C].Enable) = true;
    OUT(PhsTransArr[1][PHASE_C].DelTm)  = 0;
    OUT(PhsTransArr[1][PHASE_C].State)  = PhsTstCtrl.ParkStC;
    OUT(PhsTransArr[1][PHASE_C].Enable) = false;
    OUT(PhsTransArr[2][PHASE_C].DelTm)  = 0;
    OUT(PhsTransArr[2][PHASE_C].State)  = PhsTstCtrl.ParkStC;
    OUT(PhsTransArr[2][PHASE_C].Enable) = false;

    TstBridgeOff = false;

    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Commands the bridge to be turned on in park state for a portion
//      of a PWM loop time (can be no time) and then to be turned on in pulse
//      state for the remainder of the PWM loop time (can also be no time).
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeTest::BridgePulseFrac1
(
    // Inputs
        int              PulseTmr,    // pulse timer
    // Outputs
        unsigned&        TstBridgeOff    // entire bridge devices configured to turn off
)
{
    // If the switch between park state and pulse state is specified at
    // the beginning of the PWM loop time, then enter the PWM loop time
    // in pulse state.

    if ( PulseTmr <= 0 )
    {
        OUT(PhsTransArr[0][PHASE_A].State) = PhsTstCtrl.PulseStA;
        OUT(PhsTransArr[0][PHASE_B].State) = PhsTstCtrl.PulseStB;
        OUT(PhsTransArr[0][PHASE_C].State) = PhsTstCtrl.PulseStC;
    }
    else    // Otherwise enter the PWM loop time in park state.
    {
        OUT(PhsTransArr[0][PHASE_A].State) = PhsTstCtrl.ParkStA;
        OUT(PhsTransArr[0][PHASE_B].State) = PhsTstCtrl.ParkStB;
        OUT(PhsTransArr[0][PHASE_C].State) = PhsTstCtrl.ParkStC;
    }
    OUT(PhsTransArr[0][PHASE_A].Enable) = true;
    OUT(PhsTransArr[0][PHASE_A].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_B].Enable) = true;
    OUT(PhsTransArr[0][PHASE_B].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_C].Enable) = true;
    OUT(PhsTransArr[0][PHASE_C].DelTm)  = 0;

    // If the switch between park state and pulse state is specified at
    // the end of the PWM loop time, then remain in park state for the
    // entire PWM loop time.

    if ( PulseTmr >= (int)INS(PwmFull) )
    {
        OUT(PhsTransArr[1][PHASE_A].DelTm)  = 0;
        OUT(PhsTransArr[1][PHASE_A].State)  = PhsTstCtrl.ParkStA;
        OUT(PhsTransArr[1][PHASE_A].Enable) = false;
        OUT(PhsTransArr[2][PHASE_A].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_A].State)  = PhsTstCtrl.ParkStA;
        OUT(PhsTransArr[2][PHASE_A].Enable) = false;

        OUT(PhsTransArr[1][PHASE_B].DelTm)  = 0;
        OUT(PhsTransArr[1][PHASE_B].State)  = PhsTstCtrl.ParkStB;
        OUT(PhsTransArr[1][PHASE_B].Enable) = false;
        OUT(PhsTransArr[2][PHASE_B].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_B].State)  = PhsTstCtrl.ParkStB;
        OUT(PhsTransArr[2][PHASE_B].Enable) = false;

        OUT(PhsTransArr[1][PHASE_C].DelTm)  = 0;
        OUT(PhsTransArr[1][PHASE_C].State)  = PhsTstCtrl.ParkStC;
        OUT(PhsTransArr[1][PHASE_C].Enable) = false;
        OUT(PhsTransArr[2][PHASE_C].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_C].State)  = PhsTstCtrl.ParkStC;
        OUT(PhsTransArr[2][PHASE_C].Enable) = false;
    }

    // Otherwise switch to pulse state at the specified time and finish
    // the PWM loop time in pulse state.

    else
    {
        OUT(PhsTransArr[1][PHASE_A].DelTm)  = PulseTmr;
        OUT(PhsTransArr[1][PHASE_A].State)  = PhsTstCtrl.PulseStA;
        OUT(PhsTransArr[1][PHASE_A].Enable) = true;
        OUT(PhsTransArr[2][PHASE_A].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_A].State)  = PhsTstCtrl.PulseStA;
        OUT(PhsTransArr[2][PHASE_A].Enable) = false;

        OUT(PhsTransArr[1][PHASE_B].DelTm)  = PulseTmr;
        OUT(PhsTransArr[1][PHASE_B].State)  = PhsTstCtrl.PulseStB;
        OUT(PhsTransArr[1][PHASE_B].Enable) = true;
        OUT(PhsTransArr[2][PHASE_B].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_B].State)  = PhsTstCtrl.PulseStB;
        OUT(PhsTransArr[2][PHASE_B].Enable) = false;

        OUT(PhsTransArr[1][PHASE_C].DelTm)  = PulseTmr;
        OUT(PhsTransArr[1][PHASE_C].State)  = PhsTstCtrl.PulseStC;
        OUT(PhsTransArr[1][PHASE_C].Enable) = true;
        OUT(PhsTransArr[2][PHASE_C].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_C].State)  = PhsTstCtrl.PulseStC;
        OUT(PhsTransArr[2][PHASE_C].Enable) = false;
    }

    TstBridgeOff = false;

    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      commands the bridge to be turned on in park state for a portion of a
//      PWM loop time, then to be turned on in pulse state for another portion,
//      and then to return to park state for the remainder of the PWM loop time.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeTest::BridgePulseFrac2
(
    // Inputs
        int              PulseTmr1,   // timer for 1st pulse
        int              PulseTmr2,   // timer for 2nd pulse
    // Outputs
        unsigned&        TstBridgeOff    // entire bridge devices configured to turn off
)
{
    // Initial state
    OUT(PhsTransArr[0][PHASE_A].Enable) = true;
    OUT(PhsTransArr[0][PHASE_A].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_B].Enable) = true;
    OUT(PhsTransArr[0][PHASE_B].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_C].Enable) = true;
    OUT(PhsTransArr[0][PHASE_C].DelTm)  = 0;

    // Configurations that force the bridge to stay in park state for the
    // entire PWM loop time.
    if ( (PulseTmr1 >= (int)INS(PwmFull)) || (PulseTmr2 <= 0) || (PulseTmr2 <= PulseTmr1) )
    {
        OUT(PhsTransArr[0][PHASE_A].State)  = PhsTstCtrl.ParkStA;
        OUT(PhsTransArr[1][PHASE_A].DelTm)  = 0;
        OUT(PhsTransArr[1][PHASE_A].State)  = PhsTstCtrl.ParkStA;
        OUT(PhsTransArr[1][PHASE_A].Enable) = false;
        OUT(PhsTransArr[2][PHASE_A].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_A].State)  = PhsTstCtrl.ParkStA;
        OUT(PhsTransArr[2][PHASE_A].Enable) = false;

        OUT(PhsTransArr[0][PHASE_B].State)  = PhsTstCtrl.ParkStB;
        OUT(PhsTransArr[1][PHASE_B].DelTm)  = 0;
        OUT(PhsTransArr[1][PHASE_B].State)  = PhsTstCtrl.ParkStB;
        OUT(PhsTransArr[1][PHASE_B].Enable) = false;
        OUT(PhsTransArr[2][PHASE_B].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_B].State)  = PhsTstCtrl.ParkStB;
        OUT(PhsTransArr[2][PHASE_B].Enable) = false;

        OUT(PhsTransArr[0][PHASE_C].State)  = PhsTstCtrl.ParkStC;
        OUT(PhsTransArr[1][PHASE_C].DelTm)  = 0;
        OUT(PhsTransArr[1][PHASE_C].State)  = PhsTstCtrl.ParkStC;
        OUT(PhsTransArr[1][PHASE_C].Enable) = false;
        OUT(PhsTransArr[2][PHASE_C].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_C].State)  = PhsTstCtrl.ParkStC;
        OUT(PhsTransArr[2][PHASE_C].Enable) = false;
    }

    // Configurations that force the bridge to go to pulse state within
    // the PWM loop time.
    else
    {
        // If switching between park state and pulse state is specified
        // at the beginning of the PWM loop time, then enter the PWM loop
        // time in pulse state.
        if ( PulseTmr1 <= 0 )
        {
            OUT(PhsTransArr[0][PHASE_A].State)  = PhsTstCtrl.PulseStA;
            OUT(PhsTransArr[0][PHASE_B].State)  = PhsTstCtrl.PulseStB;
            OUT(PhsTransArr[0][PHASE_C].State)  = PhsTstCtrl.PulseStC;

            // If the switch between pulse state and park state is specified
            // at the end of the PWM loop time, then finish the PWM loop time
            // in pulse state.
            if ( PulseTmr2 >= (int)INS(PwmFull) )
            {
                OUT(PhsTransArr[1][PHASE_A].DelTm)  = 0;
                OUT(PhsTransArr[1][PHASE_A].State)  = PhsTstCtrl.PulseStA;
                OUT(PhsTransArr[1][PHASE_A].Enable) = false;
                OUT(PhsTransArr[2][PHASE_A].DelTm)  = 0;
                OUT(PhsTransArr[2][PHASE_A].State)  = PhsTstCtrl.PulseStA;
                OUT(PhsTransArr[2][PHASE_A].Enable) = false;

                OUT(PhsTransArr[1][PHASE_B].DelTm)  = 0;
                OUT(PhsTransArr[1][PHASE_B].State)  = PhsTstCtrl.PulseStB;
                OUT(PhsTransArr[1][PHASE_B].Enable) = false;
                OUT(PhsTransArr[2][PHASE_B].DelTm)  = 0;
                OUT(PhsTransArr[2][PHASE_B].State)  = PhsTstCtrl.PulseStB;
                OUT(PhsTransArr[2][PHASE_B].Enable) = false;

                OUT(PhsTransArr[1][PHASE_C].DelTm)  = 0;
                OUT(PhsTransArr[1][PHASE_C].State)  = PhsTstCtrl.PulseStC;
                OUT(PhsTransArr[1][PHASE_C].Enable) = false;
                OUT(PhsTransArr[2][PHASE_C].DelTm)  = 0;
                OUT(PhsTransArr[2][PHASE_C].State)  = PhsTstCtrl.PulseStC;
                OUT(PhsTransArr[2][PHASE_C].Enable) = false;
            }

            // Otherwise switch to park state at the specified time and
            // finish the PWM loop time in park state.
            else
            {
                OUT(PhsTransArr[1][PHASE_A].DelTm)  = PulseTmr2;
                OUT(PhsTransArr[1][PHASE_A].State)  = PhsTstCtrl.ParkStA;
                OUT(PhsTransArr[1][PHASE_A].Enable) = true;
                OUT(PhsTransArr[2][PHASE_A].DelTm)  = PulseTmr2;
                OUT(PhsTransArr[2][PHASE_A].State)  = PhsTstCtrl.ParkStA;
                OUT(PhsTransArr[2][PHASE_A].Enable) = false;

                OUT(PhsTransArr[1][PHASE_B].DelTm)  = PulseTmr2;
                OUT(PhsTransArr[1][PHASE_B].State)  = PhsTstCtrl.ParkStB;
                OUT(PhsTransArr[1][PHASE_B].Enable) = true;
                OUT(PhsTransArr[2][PHASE_B].DelTm)  = PulseTmr2;
                OUT(PhsTransArr[2][PHASE_B].State)  = PhsTstCtrl.ParkStB;
                OUT(PhsTransArr[2][PHASE_B].Enable) = false;

                OUT(PhsTransArr[1][PHASE_C].DelTm)  = PulseTmr2;
                OUT(PhsTransArr[1][PHASE_C].State)  = PhsTstCtrl.ParkStC;
                OUT(PhsTransArr[1][PHASE_C].Enable) = true;
                OUT(PhsTransArr[2][PHASE_C].DelTm)  = PulseTmr2;
                OUT(PhsTransArr[2][PHASE_C].State)  = PhsTstCtrl.ParkStC;
                OUT(PhsTransArr[2][PHASE_C].Enable) = false;
            }
        }

        // Otherwise enter the PWM loop time in park state and switch to
        // pulse state at the specified time.
        else
        {
            OUT(PhsTransArr[0][PHASE_A].State)  = PhsTstCtrl.ParkStA;
            OUT(PhsTransArr[1][PHASE_A].DelTm)  = PulseTmr1;
            OUT(PhsTransArr[1][PHASE_A].State)  = PhsTstCtrl.PulseStA;
            OUT(PhsTransArr[1][PHASE_A].Enable) = true;

            OUT(PhsTransArr[0][PHASE_B].State)  = PhsTstCtrl.ParkStB;
            OUT(PhsTransArr[1][PHASE_B].DelTm)  = PulseTmr1;
            OUT(PhsTransArr[1][PHASE_B].State)  = PhsTstCtrl.PulseStB;
            OUT(PhsTransArr[1][PHASE_B].Enable) = true;

            OUT(PhsTransArr[0][PHASE_C].State)  = PhsTstCtrl.ParkStC;
            OUT(PhsTransArr[1][PHASE_C].DelTm)  = PulseTmr1;
            OUT(PhsTransArr[1][PHASE_C].State)  = PhsTstCtrl.PulseStC;
            OUT(PhsTransArr[1][PHASE_C].Enable) = true;

            // If the switch between pulse state and park state is specified
            // at the end of the PWM loop time, then finish the PWM loop time
            // in pulse state.
            if ( PulseTmr2 >= (int)INS(PwmFull) )
            {
                OUT(PhsTransArr[2][PHASE_A].DelTm)  = PulseTmr1;
                OUT(PhsTransArr[2][PHASE_A].State)  = PhsTstCtrl.PulseStA;
                OUT(PhsTransArr[2][PHASE_A].Enable) = false;

                OUT(PhsTransArr[2][PHASE_B].DelTm)  = PulseTmr1;
                OUT(PhsTransArr[2][PHASE_B].State)  = PhsTstCtrl.PulseStB;
                OUT(PhsTransArr[2][PHASE_B].Enable) = false;

                OUT(PhsTransArr[2][PHASE_C].DelTm)  = PulseTmr1;
                OUT(PhsTransArr[2][PHASE_C].State)  = PhsTstCtrl.PulseStC;
                OUT(PhsTransArr[2][PHASE_C].Enable) = false;
            }

            // Otherwise switch to park state at the specified time and
            // finish the PWM loop time in park state.
            else
            {
                OUT(PhsTransArr[2][PHASE_A].DelTm)  = PulseTmr2;
                OUT(PhsTransArr[2][PHASE_A].State)  = PhsTstCtrl.ParkStA;
                OUT(PhsTransArr[2][PHASE_A].Enable) = true;

                OUT(PhsTransArr[2][PHASE_B].DelTm)  = PulseTmr2;
                OUT(PhsTransArr[2][PHASE_B].State)  = PhsTstCtrl.ParkStB;
                OUT(PhsTransArr[2][PHASE_B].Enable) = true;

                OUT(PhsTransArr[2][PHASE_C].DelTm)  = PulseTmr2;
                OUT(PhsTransArr[2][PHASE_C].State)  = PhsTstCtrl.ParkStC;
                OUT(PhsTransArr[2][PHASE_C].Enable) = true;
            }
        }
    }

    TstBridgeOff = false;

    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      BridgePulseFrac3() commands the bridge to be turned on in pulse state
//      for a portion of a PWM loop time (can be no time) and then to be
//      turned on in park state for the remainder of the PWM loop time (can
//      also be no time).
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeTest::BridgePulseFrac3
(
    // Inputs
        int              PulseTmr,    // pulse timer
    // Outputs
        unsigned&        TstBridgeOff    // entire bridge devices configured to turn off
)
{
    // Initial state
    OUT(PhsTransArr[0][PHASE_A].Enable) = true;
    OUT(PhsTransArr[0][PHASE_A].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_B].Enable) = true;
    OUT(PhsTransArr[0][PHASE_B].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_C].Enable) = true;
    OUT(PhsTransArr[0][PHASE_C].DelTm)  = 0;

    // If the switch between pulse state and park state is specified at
    // the beginning of the PWM loop time, then enter the PWM loop time
    // in park state.
    if ( PulseTmr == 0 )
    {
        OUT(PhsTransArr[0][PHASE_A].State) = PhsTstCtrl.ParkStA;

        OUT(PhsTransArr[0][PHASE_B].State) = PhsTstCtrl.ParkStB;

        OUT(PhsTransArr[0][PHASE_C].State) = PhsTstCtrl.ParkStC;
    }

    // Otherwise enter the PWM loop time in pulse state.
    else
    {
        OUT(PhsTransArr[0][PHASE_A].State) = PhsTstCtrl.PulseStA;

        OUT(PhsTransArr[0][PHASE_B].State) = PhsTstCtrl.PulseStB;

        OUT(PhsTransArr[0][PHASE_C].State) = PhsTstCtrl.PulseStC;
    }

    // If the switch between pulse state and park state is specified at
    // the end of the PWM loop time, then remain in pulse state for the
    // entire PWM loop time.
    if ( PulseTmr >= (int)INS(PwmFull) )
    {
        OUT(PhsTransArr[1][PHASE_A].DelTm)  = 0;
        OUT(PhsTransArr[1][PHASE_A].State)  = PhsTstCtrl.PulseStA;
        OUT(PhsTransArr[1][PHASE_A].Enable) = false;
        OUT(PhsTransArr[2][PHASE_A].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_A].State)  = PhsTstCtrl.PulseStA;
        OUT(PhsTransArr[2][PHASE_A].Enable) = false;

        OUT(PhsTransArr[1][PHASE_B].DelTm)  = 0;
        OUT(PhsTransArr[1][PHASE_B].State)  = PhsTstCtrl.PulseStB;
        OUT(PhsTransArr[1][PHASE_B].Enable) = false;
        OUT(PhsTransArr[2][PHASE_B].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_B].State)  = PhsTstCtrl.PulseStB;
        OUT(PhsTransArr[2][PHASE_B].Enable) = false;

        OUT(PhsTransArr[1][PHASE_C].DelTm)  = 0;
        OUT(PhsTransArr[1][PHASE_C].State)  = PhsTstCtrl.PulseStC;
        OUT(PhsTransArr[1][PHASE_C].Enable) = false;
        OUT(PhsTransArr[2][PHASE_C].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_C].State)  = PhsTstCtrl.PulseStC;
        OUT(PhsTransArr[2][PHASE_C].Enable) = false;
    }

    // Otherwise switch to park state at the specified time and finish
    // the PWM loop time in park state.
    else
    {
        OUT(PhsTransArr[1][PHASE_A].DelTm)  = PulseTmr;
        OUT(PhsTransArr[1][PHASE_A].State)  = PhsTstCtrl.ParkStA;
        OUT(PhsTransArr[1][PHASE_A].Enable) = true;
        OUT(PhsTransArr[2][PHASE_A].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_A].State)  = PhsTstCtrl.ParkStA;
        OUT(PhsTransArr[2][PHASE_A].Enable) = false;

        OUT(PhsTransArr[1][PHASE_B].DelTm)  = PulseTmr;
        OUT(PhsTransArr[1][PHASE_B].State)  = PhsTstCtrl.ParkStB;
        OUT(PhsTransArr[1][PHASE_B].Enable) = true;
        OUT(PhsTransArr[2][PHASE_B].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_B].State)  = PhsTstCtrl.ParkStB;
        OUT(PhsTransArr[2][PHASE_B].Enable) = false;

        OUT(PhsTransArr[1][PHASE_C].DelTm)  = PulseTmr;
        OUT(PhsTransArr[1][PHASE_C].State)  = PhsTstCtrl.ParkStC;
        OUT(PhsTransArr[1][PHASE_C].Enable) = true;
        OUT(PhsTransArr[2][PHASE_C].DelTm)  = 0;
        OUT(PhsTransArr[2][PHASE_C].State)  = PhsTstCtrl.ParkStC;
        OUT(PhsTransArr[2][PHASE_C].Enable) = false;
    }

    TstBridgeOff = false;

    return;
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Configures the PWM registers to enable the bridge in pulse state
//      for a full PWM loop time.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBridgeTest::BridgePulseFull
(
    // Inputs - None
    // Outputs
        unsigned&        TstBridgeOff    // entire bridge devices configured to turn off
)
{
    OUT(PhsTransArr[0][PHASE_A].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_A].State)  = PhsTstCtrl.PulseStA;
    OUT(PhsTransArr[0][PHASE_A].Enable) = true;
    OUT(PhsTransArr[1][PHASE_A].DelTm)  = 0;
    OUT(PhsTransArr[1][PHASE_A].State)  = PhsTstCtrl.PulseStA;
    OUT(PhsTransArr[1][PHASE_A].Enable) = false;
    OUT(PhsTransArr[2][PHASE_A].DelTm)  = 0;
    OUT(PhsTransArr[2][PHASE_A].State)  = PhsTstCtrl.PulseStA;
    OUT(PhsTransArr[2][PHASE_A].Enable) = false;

    OUT(PhsTransArr[0][PHASE_B].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_B].State)  = PhsTstCtrl.PulseStB;
    OUT(PhsTransArr[0][PHASE_B].Enable) = true;
    OUT(PhsTransArr[1][PHASE_B].DelTm)  = 0;
    OUT(PhsTransArr[1][PHASE_B].State)  = PhsTstCtrl.PulseStB;
    OUT(PhsTransArr[1][PHASE_B].Enable) = false;
    OUT(PhsTransArr[2][PHASE_B].DelTm)  = 0;
    OUT(PhsTransArr[2][PHASE_B].State)  = PhsTstCtrl.PulseStB;
    OUT(PhsTransArr[2][PHASE_B].Enable) = false;

    OUT(PhsTransArr[0][PHASE_C].DelTm)  = 0;
    OUT(PhsTransArr[0][PHASE_C].State)  = PhsTstCtrl.PulseStC;
    OUT(PhsTransArr[0][PHASE_C].Enable) = true;
    OUT(PhsTransArr[1][PHASE_C].DelTm)  = 0;
    OUT(PhsTransArr[1][PHASE_C].State)  = PhsTstCtrl.PulseStC;
    OUT(PhsTransArr[1][PHASE_C].Enable) = false;
    OUT(PhsTransArr[2][PHASE_C].DelTm)  = 0;
    OUT(PhsTransArr[2][PHASE_C].State)  = PhsTstCtrl.PulseStC;
    OUT(PhsTransArr[2][PHASE_C].Enable) = false;

    TstBridgeOff = false;

    return;
}
