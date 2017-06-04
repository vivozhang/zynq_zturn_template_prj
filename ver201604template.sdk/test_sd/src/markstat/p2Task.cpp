///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Line Product Level Task Drivers
//
// DESCRIPTION:
//      This file provides the top level product code task drivers for
//      that are called by Xops.
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "b0CelTst.h"
#include "b0PlsTst.h"
#include "b0PwmMod2Async.h"
#include "b0DbThrm.h"

// Constants
//----------
#define NUM_OF_FIR_EVT_PWM (1+1)    // init + 1 trans


// Externals
//----------
extern volatile int T1St;

extern float        L_AdjTm1Rcp;
extern float        L_DelTm1Rcp;
extern float        L_Ix;
extern float        L_IxFbk;
extern float        L_Iy;
extern float        L_IyFbk;
extern float        L_Vx;
extern float        L_VxFbk;
extern float        L_Vy;
extern float        L_VyFbk;
extern unsigned     InitDone;
extern unsigned     L_InnrRegEnb;
extern unsigned     L_BrgPwrEnb;
extern unsigned     L_InnrRegOR;

extern SeqBits1     L_SeqStBits;
extern unsigned     L_SeqReq_PackedBits;
extern SeqStatBits  L_SeqStat;
extern SeqActBits   L_SeqAct;

extern cLinePulseTest L_LinePulseTest;
extern cCellTest      L_CellTest;
extern cPwmMod2Async  L_PwmMod2Async;
extern cDbThrmModel L_DbThrmModel;

// Local Prototypes (to resolve forward references)
//-------------------------------------------------
// task1
extern void Demodulate();
extern void ModulatorLineDB();
extern void ModulatorLineDB();
extern void PreconditionLCL_T1();
extern void ProcessBrgT1();
extern void ProcessFbks();
extern void ProcessSrcCtrlT1();
extern void LineControlT1();
// task2
extern void DiagVarCalcT2();
extern void FaultChkSourceLCL();
extern void InstrumentT2();
extern void LineConvLineMonitorT2();
extern void LineCurrentRegCalcsT2();
extern void PreconditionIyref_T2();
extern void PreconditionIyref_T2();
extern void ProcessSrcCtrlT2();
extern void SourceCtrlSequencer();
extern void YCurrentRegReference();
extern void YCurrentRegReference();

// task3
extern void ProcessBrgT3();
extern void LineConvMonitorT3();
extern void GDIyLProportionalT3();
extern void ProcessSrcCtrlT3();

// taskB
extern void LineProcessBrgBgnd();
extern void LineProcessSrcCtrlBgnd();
extern void LineVarCalcBgnd();
extern void ProcessSrcCtrlT1(void);


// Types and Classes
//------------------


// Parameters
//-----------


// Variables
//-----------
CREATE_PUBVAR(L_IxFbk,          float);          // L_Ix feedback
CREATE_PUBVAR(L_IyFbk,          float);          // L_Iy feedback
CREATE_PUBVAR(L_VxFbk,          float);          // L_Vx feedback
CREATE_PUBVAR(L_VyFbk,          float);          // L_Vy feedback
CREATE_PUBVAR(L_AdjTm1Rcp,      float);     // Adjusted Task1 Time Recip  - Task 1
CREATE_PUBVAR(L_IyrfISetpt,     float);     // Reactive current referenct current setpt
CREATE_PUBVAR(L_IyrfVSetpt,     float);     // Reactive current referenct voltage setpt
CREATE_PUBVAR(L_Test1Float,     float);     // global line converter float test variable 1
CREATE_PUBVAR(L_Test2Float,     float);     // global line converter float test variable 2
CREATE_PUBVAR(L_Task1Pass,      int);       // # times task 1 has run
CREATE_PUBVAR(L_Test1Int,       int);       // global line converter integer test variable 1
CREATE_PUBVAR(L_Test2Int,       int);       // global line converter integer test variable 2
CREATE_PUBVAR(L_SeqStBitsWd,    WORD);
CREATE_PUBVAR(L_SeqReqWd,       WORD);

CREATE_PUBVAR(L_PwmUp,          unsigned);
CREATE_PUBVAR(L_PulseTestReq,   unsigned);



// Unpublished Variables
//----------------------
PhsTransListTyp  L_PhsTransListBrg1  = { 0, NULL, NULL };    // bridge 1 transition list
PhsTransListTyp  L_PhsDtransListBrg1 = { 0, NULL, NULL };    // bridge 1 phase D transition list (not used)

RegLimFlgs       L_InnrRegLim;           // inner regulator maximum limit hit
unsigned         L_SeqReqT1_PackedBits;


// Data Passing
//--------------
DATA_PASS( L_SeqReq_PackedBits, L_SeqReqT1_PackedBits, T2_T1, UNFILTERED);


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side task 1 execution functions
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineTask1(void)
{
    // Task 1 counter
    ++L_Task1Pass;

    //-------------------------------------------------------------------------
    // Feedback sampling and calculations
    //   Feedback processing includes DC link overvoltage protection.  For
    //   this reason it must occur as soon as possible in Task1.
    ProcessFbks();

    //-------------------------------------------------------------------------
    // control the phase of the pwm modulator
    if ( (L_PwmMod2Async.ModPwmN == 0.5) && (L_Task1Pass & 1) )
    {
        // if (single chop AND odd task1)
        L_PwmUp  = false;
    }
    else
    {
        // else (multiple chops OR even task1)
        L_PwmUp  = true;
    }

    //-------------------------------------------------------------------------
    // Diagnostic Mode

    if ( L_SeqStBits.L_DiagMode)
    {
        PreconditionLCL_T1();
        Demodulate();             // Demodulate currents and voltages
        L_IxFbk      =  L_Ix;
        L_IyFbk      =  L_Iy;
        L_VxFbk      =  L_Vx;
        L_VyFbk      =  L_Vy;

        L_LinePulseTest.Task1();
        if ( L_SeqReqT1.L_PlsTstReq )
        {
            L_PhsTransListBrg1 = L_LinePulseTest.PhsTransList;
        }

        L_CellTest.Task1();
        if ( L_SeqReq.L_CelTstReq )
        {
            L_PhsTransListBrg1 = L_CellTest.PhsTransList;
        }
    }

    //-------------------------------------------------------------------------
    // Control Algorithm
    else
    {
        ProcessSrcCtrlT1();

        // Modulator & Phase Transition Lists
        L_PwmMod2Async.Task1();
        L_PhsTransListBrg1 = L_PwmMod2Async.PhsTransList;
    }
    LineControlT1();
    /* Gather Task1 data for buffer passing to Task2 */
    //T1DataPassT1_T2(); Need two of these
    if (T1St == 1)   //Special clear of or'ed var.
    {
        L_InnrRegOR  = 0;
    }

    //-------------------------------------------------------------------------
    // Output data to bridge
    ProcessBrgT1();

    L_SeqStBitsWd = *(unsigned*)&L_SeqStBits;
    L_SeqReqWd = L_SeqReq_PackedBits;

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side task 2 execution functions
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineTask2(void)
{

    SourceCtrlSequencer();          //Source control sequencer

    if ( InitDone)
    {
        LineConvLineMonitorT2();    // Task 2 line monitor


        InstrumentT2();             // Instrument function for T2
        L_DbThrmModel.Task2();
        DiagVarCalcT2();            // filtered or calculated feedback vars

        FaultChkSourceLCL();        // Source fault check for task2

        LineCurrentRegCalcsT2();
    }


    ProcessSrcCtrlT2();

    //-------------------------------------------------------------------------
    // AIL stuff
    if ( L_InnrRegEnb )
    {
        YCurrentRegReference();    // Inner Regulators Enabled
    }
    else
    {
        PreconditionIyref_T2();    // Inner Regulators Disabled
    }

    //-------------------------------------------------------------------------
    // bridge power enable logic
    if ( L_SeqStBits.L_DiagMode == true )
    {
        if ( L_SeqReq.L_PlsTstReq )
        {
            L_BrgPwrEnb = L_LinePulseTest.PlsTstBrgPwrEnb;
        }
        else if ( L_SeqReq.L_CelTstReq  )
        {
            L_BrgPwrEnb = L_CellTest.CelTstBrgPwrEnb;
        }
        else
        {
            L_BrgPwrEnb = false;
        }
    }
    else if ( PARM(TestMode) == PUMP_BACK )
    {

    }
    #ifdef LEG_TEST
    else if ( PARM(TestMode) == LF_LEG_TEST )
    {

    }
    #endif
    else if ( PARM(TestMode) == HARDWARE_TEST )
    {
        L_BrgPwrEnb = false;
    }
    else if ( PARM(TestMode) == TEST_DISABLED )
    {
        //
    }
    else
    {
        L_BrgPwrEnb = false;
        PUSH_DIAG( L_BrgPwrEnbUnhandTrp );
    }

    L_PulseTestReq  = L_SeqReq.L_PlsTstReq;
    L_SeqStat.L_PlsTstStat = L_LinePulseTest.PlsTstStat;
    L_SeqAct.L_PlsTstAct  = L_LinePulseTest.PlsTstAct;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side task 3 execution functions
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineTask3()

{
    //LineCoreDriveSequencer();
    L_DbThrmModel.Task3();
    ProcessBrgT3();             // Update VCO & shunt scalings, OV trip level
    LineConvMonitorT3();
    GDIyLProportionalT3();
    ProcessSrcCtrlT3();         // Process Source control functions

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side background general variable calculations
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineVarCalcBgnd(void)
{
    L_IyrfISetpt = PARM(L_IyrfIStpt);
    L_IyrfVSetpt = PARM(L_IyrfVStpt);

    return;

}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side background execution functions
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineTaskBgnd(void)
{
    LineVarCalcBgnd();           // Background variable calculator
    LineProcessBrgBgnd();        // Process Line Bridge background functions
    LineProcessSrcCtrlBgnd();    // Process Line control background

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line side initialization functions
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void Task1_Init_A(void)
{
    L_AdjTm1Rcp = L_DelTm1Rcp;

    return;
}
