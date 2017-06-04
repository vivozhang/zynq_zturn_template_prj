/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* File:        S0PROC.C30
* Language:    TI ;C;
* Language:    TI ;C;
* Copyright:   1996..1998 GE Industrial Control Systems, Salem VA
*
* Abstract: This module contains XOPS Task1 entry point
* -----------------------------------------------------------------------------
* Functions in module:
*       ProcessSrcCtrlT1()     -- Task1 Induction Motor Ctl LCL Entry Point
*       ProcessSrcCtrlT2()     -- Task2 Induction Motor Ctl LCL Entry Point
*
*==============================================================================
* 980526 SAB  Created
* 980612 SAB  Remove redundant call to SoftResetCheck funtion.
* 980624 SAB  Exclude I_Fbk_Ivd_OR from ACMVRS6 source
* 980724 SAB  If ACMVRS6_A call XfmrSatSeq (for CAW)
* 980916 SAB  Populate Ld_Run_Cmd, Ld_Offs_Inh
* 990628 SAB  ACMVRS6_A:  Run main rule set in background after inductance test
* 991210 SAB  ACMVRS6-A:  Add checks for non-zero data and faults for the following
*                         parameters: KVA_Bas_Xfmr, Z_Pct_Xfmr, KVA_Bas_Ind, and
*                         Z_Pct_Ind.
* 991214 SAB  ACMVRS6-A:  Remove last change
* 000329 SAB  ACCBR-A:    Allow call to main rule set at end of inductance test
* 020124 SAB  Add task3 call to VoltageReference
* 021107 SAB  Move test oscillator to task2
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 021121 SAB  L_LSimMode --> SimMode
* 030124 SAB  ScaleCalc --> LineScaleCalc
* 030131 SAB  Remove product conditional compliles
* 030205 SAB  ProcessSrcCtrlBgnd --> LineProcessSrcCtrlBgnd
* 030211 SAB  Add call to LineSimulatorT2
* 030226 SCP  add calls to LineVoltSimulator(), LineHrmFCurSim() and
*             LineToLineVoltSim() [commented out for further verification.]
* 030826 DS   uncomment calls to LineVoltSimulator()
* 030903 DS   uncomment calls to LineHrmFCurSim()
* 031212 SAB  Move slow tasks to S0PROCS
* 051006 SAB  Call LinePosNegDiscriminatorT1
* 060228 SAB  Add L_VrgIgrSat
* 070117 SAB  Rename PhaseLockLoopReg to LineConvPhaseLockLoopReg, others
* 070308 SAB  Call LineSimulatorT2 any time in sim mode
* 070803 SAB  Call function, VoltageReferenceT2
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 080909 SAB  Remove un-used var, L_IFbkIvdOR
* 110822 SAB  Add call to DBVReg
* ______________________________________________________________________________
* Last Mod: August 22, 2011 12:13:25PM
*/


#include "x0FrameWork.h"
#include "Product.h"


/*****************************************************************************
*
*       external references
*
*****************************************************************************/

extern float           L_Ix            ;
extern float           L_IxFbk         ;
extern float           L_Iy            ;
extern float           L_IyFbk         ;
extern float           L_Vx            ;
extern float           L_VxFbk         ;
extern float           L_Vy            ;
extern float           L_VyFbk         ;
extern RegLimFlgs      L_InnrRegLim    ;
extern unsigned        L_InnrRegEnbT1  ;
extern unsigned        L_InnrRegEnb    ;
extern unsigned        InitDone        ;
extern unsigned        L_IndTstDone    ;

extern float           L_VabFbk        ;
extern float           L_VbcFbk        ;

extern void DBOpenLoopCheck(void);
extern void DBVReg(void);
extern void Demodulate(void);
extern void Instrument(void);
extern int  LineBrgInfcRule(void);
extern void LineConvLineMonitorT1(void);
extern void LineScaleCalc(void);
extern void PreconditionLCL_T1(void);
extern void PreconditionLCL_T2(void);
extern void RotateUpDn(void);
extern int  RunRuleSet(int Rule_Set);
extern void VoltageReference(void);
extern void VoltageReferenceT2(void);
extern void VoltReg(void);

extern void XYCurrentReg(void);




/******************************************************************************
*
*       global variables
*
******************************************************************************/

unsigned L_InnrRegOR;   /* Bit-wise or of L_InnrRegLimit                        */



/******************************************************************************
 * Function:         ProcessSrcCtrlT1
 * Execution Level:  Task 1
 * Execution Time:   Not measured
 *
 * Formal Inputs:
 * Formal Outputs:
 *
 * Global Inputs:
 * Global Outputs:
 *
 * Return Value:
 * Function Calls:
 *
 * Abstract:
 *
 *******************************************************************************/
void ProcessSrcCtrlT1(void)
{
    unsigned *Lim_Ptr;            // temp variable used to initialize structures

    // Control Algorithm and Line Model
    // Do not alter the call sequence of source control

    if ( InitDone)
    {
        Demodulate();             // Demodulate currents and voltages
    }

    L_IxFbk      =  L_Ix;
    L_IyFbk      =  L_Iy;
    L_VxFbk      =  L_Vx;
    L_VyFbk      =  L_Vy;

    Instrument();                 // Calculate instrumentation
    LineConvLineMonitorT1();      // Line monitor

    // Inner Regulators Enabled
    //----------
    if ( L_InnrRegEnbT1 )
    {
        VoltReg();                // Voltage regulator
        XYCurrentReg();           // X-Y current regulator
    }
    // Inner Regulators Disabled
    //----------
    else
    {
        if ( InitDone)
        {
            PreconditionLCL_T1();          // Precondition control variables as needed
        }
    }
    RotateUpDn();                          // Rotate up/downs

    // Bitwise OR L_InnrRegLim with buffer
    Lim_Ptr = (unsigned*) &L_InnrRegLim;   // Set ptr to address of L_InnrRegLim
    L_InnrRegOR = *Lim_Ptr | L_InnrRegOR;


}



/******************************************************************************
 * Function:         ProcessSrcCtrlT2
 * Execution Level:  Task 2
 * Execution Time:   Not measured
 *
 * Formal Inputs:
 * Formal Outputs:
 *
 * Global Inputs:
 * Global Outputs:
 *
 * Return Value:
 * Function Calls:
 *
 * Abstract:
 *
 *******************************************************************************/
void ProcessSrcCtrlT2(void)
{
    VoltageReferenceT2();

    // Inner Regulators Enabled
    //----------
    if ( L_InnrRegEnb )
    {
        // Nothing now
    }

    // Inner Regulators Disabled
    //----------
    else
    {
        PreconditionLCL_T2();     // precondition task 2 vars 5/3/2012 HL: this function is empty
    }
}




/******************************************************************************
 * Function:         ProcessSrcCtrlT3
 * Execution Level:  Task 3
 * Execution Time:   Not measured
 *
 * Formal Inputs:
 * Formal Outputs:
 *
 * Global Inputs:
 * Global Outputs:
 *
 * Return Value:
 * Function Calls:
 *      LineScaleCalc
 *      VoltageReference
 *
 * Abstract:
 *
 *******************************************************************************/
void ProcessSrcCtrlT3(void)
{

    LineScaleCalc();                /* Calculate control scale factor */
    VoltageReference();

}

/******************************************************************************
 * Function:         LineProcessSrcCtrlBgnd
 * Execution Level:  Background Task
 * Execution Time:   Not measured
 *
 * Formal Inputs:
 *                  L_IndTstDone
 * Formal Outputs:
 *
 * Global Inputs:
 * Global Outputs:
 *
 * Return Value:
 * Function Calls:
 *      RunRleSet
 *
 * Abstract:
 *
 *******************************************************************************/
void LineProcessSrcCtrlBgnd(void)
{


    if ( L_IndTstDone)
    {
        RunRuleSet(RULE_SET_MAIN);
        L_IndTstDone = false;
    }

    LineBrgInfcRule();

}
