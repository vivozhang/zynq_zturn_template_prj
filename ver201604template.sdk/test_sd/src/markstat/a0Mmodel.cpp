/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        A0MMODEL.C30
* Language:    TI 'C'
* Copyright:   1996..1998 GE Industrial Control Systems, Salem VA
*
* Abstract: This file contains those functions which are used to model the
*           motor.  This functionality allows the user to simulate a motor
*           without using a real motor.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* MtrModelElec                          - Motor electrical model.
*
*==============================================================================
* 961029 0000 SAB  Create for motor model
* 020822 DS   Modify for doubly-fed
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 040712 IB   Using S_SynSwClsd in place of S_SynBrkClsd for new 60hz LVMD.
* 060121 SAB  Also run logic normally run with sync switch open when R_
* 070109 IB   Rename S_SynSwClsd as S_SimK1Clsd
* 070127 SAB  Remove In-lining from RotorMtrElecSim
* 070321 SAB  Substitute PumpBackAct for R_PumpBack
* 070317 SAB  Remove un-used code.  Remove FbkScale
* 070612 SAB  Use Iram copies of parameters for EEs
* 070712 SAB  S_Rs --> S_R1 so that S_Rs can include cable resistance
*             S_RsSim  --> S_R1Sim
* 071102 SAB  Use S_Rs in flux model to calc flux all the way back to air gap
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
*______________________________________________________________________________
* Last Mod: December 12, 2007 2:14:10PM
*/


// Includes Files
//---------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
// product
#include "Product.h"
#include "a0Mmodel.h"

// Parameters
//-----------
CREATE_PARM(R_FlxDmpKd,         float);     // flux damping constant
CREATE_PARM(R_FlxDmpWd,         float);     // flux damping response

// Variables
//----------
// electrical model
CREATE_PUBVAR(S_FlxXFbk,      float);    // calculated x component of stator flux
CREATE_PUBVAR(S_FlxYFbk,      float);    // calculated y component of stator flux
CREATE_PUBVAR(R_FlxDmpInt,    float);    // flux damping integrator
CREATE_PUBVAR(R_FlxDmpOut,    float);    // flux dampening output
CREATE_PUBVAR(S_PwrCal,       float);    // power calculated
CREATE_PUBVAR(R_TrqErr,       float);    // torque err

/*****************************************************************************
*
*       external references
*
*****************************************************************************/
extern float GmAngElecCmd;
extern float GmPllWAvg;
extern float L_LineFrqRcp;
extern float R_AngElecFbk;
extern float R_AngElecLin;
extern float R_DelTm1;
extern float R_LT1TmDelta;
extern float R_TrqCmdIn;
extern float R_WElec;
extern float R_WRtr;
extern float S_AngElecFbk;
extern float S_IxFbk;
extern float S_IyFbk;
extern float S_PllW;
extern float S_Rs;
extern float S_VMag;
extern float S_VxFbk;
extern float S_VyFbk;
extern SinCosTyp S_SinCosFbk;
extern unsigned R_InnrRegEnbT1;
extern unsigned S_PllProven;
extern unsigned S_SimK1ClsdT1;
extern float         R_TrqCal;

/*=============================================================================
 * Function: MtrModelElec
 * Execution Level: Task 1
 * Execution Time: 6.00/3.50usec (sim/non-sim) 12/19/96--Rev:001.01.01.A
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  R_DelTm1
 *                 Est_R1
 *                 R_IxFbk
 *                 Ix_Sim
 *                 R_IyFbk
 *                 Iy_Sim
 *                 R_FlxLkgCmd
 *                 Ls_Sim_Rcp
 *                 R_TauRtrSimR
 *                 R_TauRtrRcp
 *                 R_TrqCmdIn
 *                 R_VxFbk
 *                 R_VxSim
 *                 R_VyFbk
 *                 R_VySim
 *                 R_WElec
 *                 R_WElecFil
 *                 Sim_Mode
 *
 *                 EE.CI.R_FlxDmpKd
 *                 EE.CI.R_PolePairs
 *                 EE.CI.S_R1Sim
 *                 EE.CI.S_R2Sim
 *                 EE.CI.Flx_Meld_Gn
 *                 EE.CI.W_Flx_Meld
 *
 * Global Outputs: Flx_Meld
 *                 S_PwrCal
 *                 R_TrqCal
 *                 R_TrqErr
 *                 R_FlxXSim
 *                 R_FlxYSim
 *                 Flx_X_Fbk
 *                 Flx_Y_Fbk
 *                 Ix_Sim
 *                 Iy_Sim
 *
 * Locals:         R_FlxDmpInt
 *                 R_FlxDmpOut
 *                 Flx_Meld_Int
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function performs the electircal motor model.  It depends upon
 * the control regulator functions to provide electrical frequency and slip
 * frequency.  Its primary outputs are the x and y components of current
 * and calculated torque and power.  Some parts of this function run only in
 * simulator mode.
 *     There are three basic parts to this function, as shown on the associated
 * three block diagrams: Motor Flux Model, Motor Current Model, and Motor
 * Electrical Torque and Power.
 *     The Motor Flux Model has a sub-part called Motor Flux Damping, which has
 * an associated block diagram.
 *
 *===========================================================================*/

void RotorMtrModelElec(void)
{
    float   Temp1;


    // Flux Meld generation has moved to task 2 04/04/00 to allow more cpu time

    /* ------ Flux Cal ----- */

    if (S_PllProven)
    {
        Temp1 = S_VxFbk + S_IxFbk*S_Rs + S_PllW*S_FlxYFbk;
        S_FlxXFbk  = S_FlxXFbk + R_DelTm1 * Temp1;

        Temp1 = S_VyFbk + S_IyFbk*S_Rs - S_PllW*S_FlxXFbk;
        S_FlxYFbk  = S_FlxYFbk - R_FlxDmpOut + R_DelTm1 * Temp1;
    }
    else
    {
        S_FlxXFbk  = 0;
        if ( PARM(TestMode) == PUMP_BACK )
        {
            S_FlxYFbk  = S_VMag*L_LineFrqRcp;
        }
        else
        {
            S_FlxYFbk  = S_VMag/(R_WElec + R_WRtr);
        }
    }


    /* ------ Flux Damping ----- */
    R_FlxDmpInt = R_FlxDmpInt + PARM(R_FlxDmpWd) * R_DelTm1 *
                  (S_FlxYFbk - R_FlxDmpInt);
    R_FlxDmpOut = PARM(L_LineFrq) * PARM(R_FlxDmpKd) * R_DelTm1 *
                  (S_FlxYFbk - R_FlxDmpInt);


    /* =========== Motor Electrical Torque, Power, and Torque Error ======== */

    Temp1 = 1.5*( S_IxFbk*S_FlxYFbk - S_IyFbk*S_FlxXFbk );
    R_TrqCal = PARM(R_PolePairs)* Temp1;
    S_PwrCal = Temp1 * S_PllW;
    R_TrqErr = R_TrqCmdIn - R_TrqCal;

}
