/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        A0PROC.C30
* Language:    TI 'C'
* Copyright:   2003...GE Drives and Controls, Inc., Salem VA
*
* Abstract: This module contains XOPS Task1 entry point
* -----------------------------------------------------------------------------
* Functions in module:
*       ProcessMtrCtrlT1()     -- Task1 Induction Motor Ctl LCL Entry Point
*       ProcessMtrCtrlT2()     -- Task2 Induction Motor Ctl LCL Entry Point
*       ProcessMtrCtrlT3()     -- Task3 Induction Motor Ctl LCL Entry Point
*       RotorProcessMtrCtrlBgnd()   -- TaskBgnd Induction Motor Ctl LCL Entry Point
*       MtrCtlTuneT1()         -- Task1 Induction Motor Ctl Tuneup
*
*==============================================================================
* 980403 PSB  Created
*
* 020822 DS   Modify for doubly-fed
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 030205 SAB  ProcessMtrCtrlBgnd --> RotorProcessMtrCtrlBgnd
* 030218 SD   add call to StatorTocDetection()
* 030226 SCP  add calls to LineVoltSimulator(), LineHrmFCurSim() and
*             LineToLineVoltSim() [commented out for further verification.]
* 031203 SAB  Call RotorDemodCurrent until rotor converter is stopped
* 040107 SAB  Move slow functions to A0PROCS.C30
* 051007 SAB  Add call to StatorPosNegDiscriminatorT1();
* 070604 SAB  Remove call to RotorFlxPll
* 070625 SAB  Add call to RotorDCCurrentRegT2
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 071214 SAB  Remove call to RotorDemodulateStatorSim and put RotorDemodulateStator
*             outside simmode/non simmode case
* 080307 SAB  Add call to RotorIxTrqCompT2
* 080313 SAB  Remove call to RotorIxTrqCompT2
* 110825 SAB  Always run RotorCmdFbkAngl
* ______________________________________________________________________________
* Last Mod: August 25, 2011 4:18:20AM
*/

#include   "x0FrameWork.h"
#include   "product.h"
#include   "a0Proc.h"


/*****************************************************************************
*
*       external references
*
*****************************************************************************/

extern void RotorAnglesT2(void);
extern void RotorAnglesT3(void);
extern void RotorBrgTestT1(void);
extern void RotorCmdFbkAngl(void);
extern void RotorCurLimCal(void);
extern void RotorDemodCurrent(void);
extern void RotorDemodulateStator(void);
extern void RotorDemodVoltage(void);
extern void RotorDiagVarCalcT2(void);
extern void RotorDiagVarCalcT3(void);
extern void RotorFlxCurrent(void);
extern void RotorInnrRegAlrm(void);
extern void RotorIxTrqCmd(void);
extern void RotorModulator(void);
extern void RotorMtrCtlTuneFbk(void);
extern void RotorMtrCtlTuneReg(void);
extern void RotorMtrModelElec(void);
extern void RotorPrecondition_T1(void);
extern void RotorPrecondition_T2(void);
extern void RotorRegControl(void);
extern void RotorRosettaStone(void);
extern void RotorRotateUpDn(void);
extern void RotorS_LineMonitorT1(void);
extern void RotorS_PhaseLockLoopReg(void);
extern void RotorScaleCalc(void);
extern void RotorSyncIOCCheck(void);
extern void GenTocDetection(void);
extern void RotorTorqueInput(void);
extern int  RotorTuneupRecorder(void);
extern void RotorVoltReg(void);
extern void RotorXYCurrentReg(void);
extern void RotorXYCurrentRegT3(void);
extern void RotorFlxCurrentT1(void);
extern void StatorCTCheck(void);
extern void StatorTocDetection(void);

extern float       S_Ix;
extern float       S_IxFbk;
extern float       S_Iy;
extern float       S_IyFbk;
extern float       S_Vx;
extern float       S_VxFbk;
extern float       S_Vy;
extern float       S_VyFbk;
extern unsigned    R_SeqStBits_PackedBits;
extern RRegLimFlgs R_InnrRegLim;
extern unsigned    R_InnrRegEnb;
extern unsigned    R_InnrRegEnbT1;
extern unsigned    R_TrqLimAct;
extern float       R_VxFbk;
extern float       R_VyFbk;
extern float       R_IxFbk;
extern float       R_IyFbk;
extern float       R_Ia;
extern float       R_Ib;
extern float       R_Ic;

/******************************************************************************
*
*       global variables
*
******************************************************************************/
CREATE_PUBVAR(R_TrqLimF,    float);
CREATE_PUBVAR(R_TrqLimFT2,  float);
CREATE_PUBVAR(R_TrqLimAct,  unsigned);   // In current or torque limit

CREATE_PUBVAR(S_VxFbk,      float);             /* vx feedback */
CREATE_PUBVAR(S_VyFbk,      float);             /* vy feedback */
CREATE_PUBVAR(S_IxFbk,      float);             /* ix feedback */
CREATE_PUBVAR(S_IyFbk,      float);             /* iy feedback */


/******************************************************************************
 * Function:         ProcessMtrCtrlT1
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

void RotorProcessMtrCtrlT1(void)
{
    /* Control Algorithm and Motor Model */
    // Do not alter the call sequence of motor control
    RotorTorqueInput();                  // 1.3uS
    RotorRegControl();                   // 4.1uS

    RotorDemodulateStator();

    S_IxFbk  =  S_Ix;
    S_IyFbk  =  S_Iy;
    S_VxFbk  =  S_Vx;
    S_VyFbk  =  S_Vy;

    //Inner Regulators Enabled
    //----------
    if ( R_InnrRegEnbT1 )
    {
        RotorSyncIOCCheck();
        RotorS_PhaseLockLoopReg();       /* Stator PLL */
        RotorS_LineMonitorT1();
        RotorDemodVoltage();             /* was 5.7uS  EYYH re-time 05/04/00 4.8uS                   */
        RotorDemodCurrent();             /* was 1.3uS  EYYH retime 05/10/00 1.333usec                */
        RotorMtrModelElec();             /* was 4.8uS  EYYH retime 05/10/00 3.667usec (non Sim mode) */
        /* If exec time between Smp1Encod and Smp2Encod changes     */
        /* Tac_Read_Tm (A0Rule) has to be updated.                  */
        /* Smp2Encod is also called inside Precondition_T1 for      */
        /* covering R_InnrRegEnb = False case. The exec time        */
        /* between Smp1Encod and Smp2Encod is identical for both    */
        /* R_InnrRegEnb = True and False case.                      */
        /************************************************************/


        RotorIxTrqCmd();                 // 1.4uS
        RotorXYCurrentReg();             // 3.9uS
        RotorFlxCurrentT1();
        RotorVoltReg();                  // 11uS
        RotorCmdFbkAngl();               // 11uS
        RotorRotateUpDn();               // 4.7uS
    }

    // Inner Regulators Disabled
    //----------
    else
    {
        //  if ( InitDone)
        //  {
        //  }
        RotorS_PhaseLockLoopReg();       /* Stator PLL */
        RotorS_LineMonitorT1();
        RotorCmdFbkAngl();               // 11uS
        RotorDemodVoltage();
        if (!R_SeqStBits.R_Stopped)
        {
            RotorDemodCurrent();
        }
        RotorPrecondition_T1();
    }

    if (( *(unsigned*) &R_InnrRegLim) & 0xF)
        R_TrqLimF = 1.0;
    else
        R_TrqLimF = 0.0;
}


/******************************************************************************
 * Function:         ProcessMtrCtrlT2
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
void RotorProcessMtrCtrlT2(void)
{

    if (R_TrqLimFT2 >= 0.5)
        R_TrqLimAct = true;
    else
        R_TrqLimAct = false;

    RotorAnglesT2();
    RotorFlxCurrent();              /* Flux current calculation  */
    RotorCurLimCal();               /* Current limit calculation */

    StatorCTCheck();                /* Check stator CT's         */

    if (!R_InnrRegEnb)              /* Precondition if not running */
        RotorPrecondition_T2();

    RotorDiagVarCalcT2();           /* Calculate diagnostic variables */
}

/******************************************************************************
 * Function:         ProcessMtrCtrlT3
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
 *      ScaleCalc
 *      TocDetection
 *      AdaptR2
 *      AdaptR1T3
 *      MotorProtection
 *      MtrThermal
 *      InnrRegAlrm
 *      SpinTune
 *      DiagVarCalcT3
 *
 * Abstract:
 *
 *******************************************************************************/
void RotorProcessMtrCtrlT3(void)
{
    RotorAnglesT3();
    RotorScaleCalc();                // Calculate control scale factor
    GenTocDetection();               // Generator timed overcurrent detection
    StatorTocDetection();            // Stator timed overcurrent detection
    RotorInnrRegAlrm();              // Inner regulator alarms
    RotorDiagVarCalcT3();            // Diagnostic variable calculation routine
    RotorXYCurrentRegT3();           // Rotor current regulator, task 3

}


/******************************************************************************
 * Function:         RotorProcessMtrCtrlBgnd
 * Execution Level:  Background Task
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
 *      DiagVarFilterGain
 *      RosettaStone
 *
 * Abstract:
 *
 *******************************************************************************/
void RotorProcessMtrCtrlBgnd(void)
{

    RotorRosettaStone();         /* Make copies of fundamental motor parameters */

}

/******************************************************************************
 * Function:         MtrCtlTuneT1
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
 * Abstract: Note: left in Sram becuase runs off-line
 *
 *******************************************************************************/
void RotorMtrCtlTuneT1(void)
{

#if 0

    RotorMtrCtlTuneFbk();
    if (I_Info.Using_Pls_Tst)
    {
        RotorPrecondition_T1();
        RotorBrgTestT1();
    }
    else
    {
        RotorMtrCtlTuneReg();         // motor control tuneup regulator
        RotorModulator();
    }
    RotorTuneupRecorder();     // data sample collector for tuneup

#endif
}
