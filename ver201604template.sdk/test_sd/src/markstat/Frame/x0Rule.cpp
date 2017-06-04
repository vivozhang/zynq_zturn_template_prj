///////////////////////////////////////////////////////////////////////////////
//
// TITLE:      Framework Rules
//
// DESCRIPTION:
//             This file contains common rule calculations
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
#include "x0rule.h"
#include "x0CaptureBuf.h"
// product


// Constants
//----------


// Externals
//----------
extern unsigned     IsrSvcErrThresh;
extern unsigned     SimMode;
extern unsigned     XSimMode;
extern unsigned     DsimMode;
extern unsigned     SimNotDsim;

// comment out due to removeal of batt code - JWT
//extern unsigned     R_BatterySimEnb;

extern float        CpuCps;

extern cCaptureBuf  CaptureBuf;
extern cTripBuf     TripBuf;

extern int  RunProductRules(int Rule_Set);
extern int  TimebaseInit(void);
extern void ConstantValue(void);


// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Variables
//----------
CREATE_PUBVAR(SimMode,              unsigned);      // system simulation mode

// Parameters:
//------------


// Diagnostics:
//------------


// Member Functions
//-----------------




//******************************************************************************
// Function:            FrameWorkRulesInit
// Execution Level:
// Execution Time:
// Formal Inputs:
// Formal Outputs:
// Global Inputs:
//
// Global Outputs:
//
// Return Value:        SUCCESS, FAIL
// Function Calls:
// Abstract:            Framework rules that are hard reset required and only
//                      run at init time.
//
//******************************************************************************
int FrameWorkRulesInit(void)
{
    int     Status = SUCCESS;

    //FVSET(HdwTstCmd,    HWT_NONE);

    SET_FV(XSimModeReq,   false);
    XSimMode = PARM(XSimModeReq);

    SET_FV(DsimModeReq,   false);
    DsimMode = PARM(DsimModeReq);

    SET_FV(SimModeReq,    false);
    SimMode  = PARM(SimModeReq);

    SET_FV(RuleDepCheckInh, false);

    // comment out due to removeal of batt code - JWT
    // R_BatterySimEnb = SimMode && !XSimMode;

    // done
    return (Status);
}


//******************************************************************************
// Function:            FrameWorkRules
// Execution Level:
// Execution Time:
// Formal Inputs:
// Formal Outputs:
// Global Inputs:
//
// Global Outputs:
//
// Return Value:        SUCCESS, FAIL
// Function Calls:
// Abstract:            Framework rules that are not hard reset required run when
//                      all the rules run.
//
//******************************************************************************
int FrameWorkRules(void)
{
    int     Status = SUCCESS;

    SET_FV(XmsProbeSite, 0);                // probe site set to "disabled"


    SET_FV(IsrSvcErrThresh, 60.0e-6F);       // ISR must be serviced within 60 usec of the scheduled time
    IsrSvcErrThresh = (uint32_t)(PARM(IsrSvcErrThresh) * CpuCps);

    // debug spin waits default to 0 seconds
    SET_FV(SpinWaitT1,      0.0F);
    SET_FV(SpinWaitT1A,     0.0F);
    SET_FV(SpinWaitT1S,     0.0F);
    SET_FV(SpinWaitT2,      0.0F);
    SET_FV(SpinWaitT3,      0.0F);
    SET_FV(SpinWaitB,       0.0F);

    // done
    return (Status);
}


//******************************************************************************
// Function:            RunRuleSet
// Execution Level:
// Execution Time:
// Formal Inputs:
//    Rule_Set          ruleset identifier
// Formal Outputs:
// Global Inputs:
// Global Outputs:
// Return Value:        SUCCESS, FAIL
// Function Calls:
// Abstract:            Select ruleset to run.  This is a separate function
//                      because it may be called by the Toolbox via an ADL cmd.
//
//******************************************************************************
int RunRuleSet(int Rule_Set)
{
    int         Status = SUCCESS;

    if ( Rule_Set == RULE_SET_MAIN )
    {
        // framework rules
        if ( FrameWorkRules() == FAIL )
            Status = FAIL;

        // capture buffer rules
        if ( CaptureBuf.RunRules() == FAIL )
            Status = FAIL;
        if ( TripBuf.RunRules(NULL) == FAIL )
            Status = FAIL;

        // process user defined constant values
        ConstantValue();
    }

    if ( RunProductRules(Rule_Set) == FAIL )
        Status = FAIL;

    // done
    return (Status);
}


/******************************************************************************
* Function:         InitialRules
*
* Execution Level:  Initialization
* Execution Time:   Not measured
* Formal Inputs:    None
* Formal Outputs:   None
*
* Function Calls:
*
* Abstract:
*   This function orchestrates the execution of Rules which must run during
*   the platform initialization process.  It is expected to be invoked once only
*   by the FvInitialize function.
*
 *****************************************************************************/
int InitialRules()
{
    int     Status = SUCCESS;

    // run the timebase calculations
    if ( TimebaseInit() == FAIL )
        Status = FAIL;

    // run the framework rules (must run first becuase product rules may have dependencies on these rules)
    if ( FrameWorkRulesInit() == FAIL )
        Status = FAIL;

    // first exec of the Main Rule Set (Auto-Rules)
    if ( RunRuleSet(RULE_SET_MAIN) == FAIL )
        Status = FAIL;

    // annunciate rule faitlure ???
    if ( Status == FAIL )
    {
        // TBD
    }

    // done
    return (Status);
}

