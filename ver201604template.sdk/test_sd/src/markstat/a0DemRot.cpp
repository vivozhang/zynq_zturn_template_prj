/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        A0DEMROT.C30
* Language:    TI 'C'
* Copyright:   1996 GE Motors and Industrial Systems, Salem VA
*
* Abstract: This file contains those functions which are associated
*           demodulation and rotation of current, voltage and flux.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* DemodVoltage                 -  Demodulate input voltages
* DemodCurrent                 -  Demodulate input currents
*
*==============================================================================
* 961114 0000 SAB  Create for voltage and current demodulation and rotation.
* 020822 DS   Modify for doubly-fed
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 060121 SAB  Only demode R_VxSim R_VySim when InnrRegEnb since RotorVoltageFbkSim
*             now called any time sync switch is closed.
* 070604 SAB  Use Iram copies of parameters for EEs
* 071212 SAB  Make adjustments for implementation of "One pll" scheme -- comments only
*______________________________________________________________________________
* Last Mod: December 14, 2007 1:47:19PM
*/


// Includes Files
//---------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
// product
#include "Product.h"
#include "a0DemRot.h"

/*******************
* Global variables *
********************/

/*
 * Re-initialized variables for shared motion.  All initialized variables
 * between REINIT_VARS_START and REINIT_VARS_STOP are re-initialized when
 * switching motors in shared motion applications.
 */


CREATE_PUBVAR(S_Ix            ,       float);             /* x-component of demoduled current           */
CREATE_PUBVAR(S_Iy            ,       float);             /* y-component of demoduled current           */
CREATE_PUBVAR(S_Vx            ,       float);             /* x-component of demodulated voltage         */
CREATE_PUBVAR(S_Vy            ,       float);             /* y-component of demodulated voltage         */

CREATE_PUBVAR(R_UaCmd         ,       float);             /* Phase A modulation command */
CREATE_PUBVAR(R_UbCmd         ,       float);             /* Phase B modulation command */
CREATE_PUBVAR(R_UcCmd         ,       float);             /* Phase C modulation command */

CREATE_PUBVAR(S_IxFbkMet      ,       float);             /* ix feedback component for Stator RmsMeter */
CREATE_PUBVAR(S_IyFbkMet      ,       float);             /* iy feedback component for Stator RmsMeter */
CREATE_PUBVAR(S_VxFbkMet      ,       float);             /* vx feedback component for Stator RmsMeter */
CREATE_PUBVAR(S_VyFbkMet      ,       float);             /* vy feedback component for Stator RmsMeter */

CREATE_PUBVAR(R_IxFbkMet      ,       float);             /* ix feedback component for Rotor  RmsMeter */
CREATE_PUBVAR(R_IyFbkMet      ,       float);             /* iy feedback component for Rotor  RmsMeter */


/***************************************************************************
*
*                               Private Variables
*
*****************************************************************************/
/* DemodVoltage procedure */

SinCosTyp R_SinCosCmd;                                    /* sine/cosine structure for angle command */
SinCosTyp R_SinCosFbk;                                    /* sine/cosine structure for angle feedback */
SinCosTyp S_SinCosFbk;                                    /* sine/cosine structure for stator angle feedback */

extern float  S_AngElecFbk;
extern float  R_IaCmdLkout;           /* phase A current command for up-down lockout*/
extern float  R_IbCmdLkout;           /* phase B current command for up-down lockout*/
extern float  R_IcCmdLkout;           /* phase C current command for up-down lockout*/

extern float   S_CosAngFbk;           // Cosine of angle command
extern float   S_SinAngFbk;           // Sin of angle command
extern float   S_Cos2Ang;             // Cosine (2*Angle)
extern float   S_Sin2Ang;             // Sin (2*Angle)

extern float   R_LkoutGnSclT1;

/*****************************************************************************
*
*       external references
*
*****************************************************************************/


extern  float    R_Ia;
extern  float    R_Ib;
extern  float    R_Ic;
extern  float    R_IxCmd0;
extern  float    R_IxFbk;
extern  float    R_IxFbkComp;
extern  float    R_IxFbkCompNT1;
extern  float    R_IyCmd0T1;
extern  float    R_IyFbk;
extern  float    R_IyFbkComp;
extern  float    R_IyFbkCompNT1;
extern  float    R_UxCmd;
extern  float    R_UyCmd;
extern  float    R_Vab;
extern  float    R_Vbc;
extern  float    R_VxCmd;
extern  float    R_VyCmd;
extern  float    S_Ia;
extern  float    S_Ib;
extern  float    S_Ic;
extern  float    S_Vab;
extern  float    S_Vbc;
extern  float    R_AngElecCmd;           /* electrical command angle */
extern  float    R_AngElecFbk;           /* electrical feedback angle */
extern  unsigned R_InnrRegEnbT1;

extern  float    S_Van;
extern  float    S_Vbn;
extern  float    S_Vcn;

extern  float    R_IaBrg;
extern  float    R_IbBrg;
extern  float    R_IcBrg;
extern  float    R_IxFbkMet;
extern  float    R_IyFbkMet;
extern float         R_VxFbk;
extern float         R_VyFbk;

/*=============================================================================
 * Function: DemodVoltage
 * Execution Level: Task 1
 * Execution Time: 3.04usec  12/19/96--Rev:001.01.01.A
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  R_UaCmd
 *                 R_UaCmdM
 *                 R_UbCmd
 *                 R_UbCmdM
 *                 R_UcCmd
 *                 R_UcCmdM
 *                 R_UxCmd
 *                 R_UyCmd
 *                 R_UyCmdZ1
 *                 R_Vab
 *                 R_Vbc
 *                 R_VdcComp
 *                 Sim_Mode
 *
 *
 * Global Outputs: R_VxFbk
 *                 R_VxSim
 *                 R_VyFbk
 *                 R_VySim
 *
 * Locals:         R_VabSim
 *                 R_VbcSim
 *                 R_VbcSimz
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function performs the demodulation of motor voltages into the
 * x-y coordinate system.
 *
 *===========================================================================*/

void RotorDemodVoltage()
{
    cVectorf::SineCosine(R_AngElecFbk,&R_SinCosFbk);

    cVectorf::DemodLL(R_Vab,R_Vbc, &R_SinCosFbk, &R_VxFbk,&R_VyFbk);
}

/*=============================================================================
 * Function: DemodCurrent
 * Execution Level: Task 1
 * Execution Time:  6.28/6.32usec (sim/non-sim) 12/19/96--Rev:001.01.01.A
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  Ang_Elec_Cmd
 *                 R_IxSim
 *                 R_IySim
 *                 Sim_Mode
 *
 *
 *
 *
 *
 * Global Outputs: R_IxFbk
 *                 R_IyFbk
 *
 *
 * Locals:         R_IxSim_Z1
 *                 R_IxSim_Z2
 *                 R_IySim_Z1
 *                 R_IySim_Z2
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function performs the demodulation of motor currents into the
 * x-y coordinate system.
 *
 *===========================================================================*/


void RotorDemodCurrent()
{
    cVectorf::DemodLN( R_Ia,R_Ib,     &R_SinCosFbk, &R_IxFbk,&R_IyFbk);

    cVectorf::DemodLN3(R_Ia ,R_Ib ,R_Ic ,&R_SinCosFbk, &R_IxFbkMet ,&R_IyFbkMet );
    cVectorf::DemodLN3(R_IaBrg,R_IbBrg,R_IcBrg,&R_SinCosFbk, &R_IxFbkMet,&R_IyFbkMet);
}

/*=============================================================================
 * Function: DemodulateStator
 * Execution Level: Task 1
 * Execution Time:  6.28/6.32usec (sim/non-sim) 12/19/96--Rev:001.01.01.A
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  Ang_Elec_Cmd
 *                  *
 * Global Outputs: Ix
 *                 Iy
 * Locals:
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function performs the demodulation of source currents and voltages
 * into the x-y coordinate system.
 *
 *===========================================================================*/
void RotorDemodulateStator()
{
        cVectorf::SineCosine(S_AngElecFbk,&S_SinCosFbk);

        cVectorf::DemodLN( S_Ia ,S_Ib ,      &S_SinCosFbk, &S_Ix,&S_Iy);
        cVectorf::DemodLL( S_Vab,S_Vbc,      &S_SinCosFbk, &S_Vx,&S_Vy);

        cVectorf::DemodLN3(S_Ia,S_Ib,S_Ic,   &S_SinCosFbk, &S_IxFbkMet,&S_IyFbkMet);
        cVectorf::DemodLN3(S_Van,S_Vbn,S_Vcn,&S_SinCosFbk, &S_VxFbkMet,&S_VyFbkMet);

    //////////////////////////////////////////
    //  Caluculate L_Cos2Ang and L_Sin2Ang  //
    //////////////////////////////////////////

     //Caluculate S_Cos2Ang and S_Sin2Ang
    S_CosAngFbk = S_SinCosFbk.cos;
    S_SinAngFbk = S_SinCosFbk.sin;
    S_Cos2Ang = 2.0*(S_CosAngFbk*S_CosAngFbk) - 1.0;
    S_Sin2Ang = 2.0*S_SinAngFbk*S_CosAngFbk;

    //////////////////////////////////////
    //  Current feedback comp rotation  //
    //////////////////////////////////////
    R_IxFbkComp  = (  R_IxFbkCompNT1*S_Cos2Ang) + (R_IyFbkCompNT1*S_Sin2Ang);
    R_IyFbkComp  = ( -R_IxFbkCompNT1*S_Sin2Ang) + (R_IyFbkCompNT1*S_Cos2Ang);
}


/*=============================================================================
 * Function: RotateUpDn
 * Execution Level: Task 1
 * Execution Time: 6.30usec       12/19/96--Rev:001.01.01.A
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  R_VxCmd
 *                 R_VyCmd
 *                 Ang_Elec_Cmd
 *                 R_IxCmd0
 *                 R_IyCmd0 (From T2)
 *
 * Global Outputs: R_UaCmd
 *                 R_UbCmd
 *                 R_UcCmd
 (                 R_IaCmdLkout
 (                 R_IbCmdLkout
 (                 R_IcCmdLkout
 *
 * Locals:
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function performs the rotatation of motor voltages & current from the
 * x-y coordinate system into the ABC system.
 *
 *===========================================================================*/

void RotorRotateUpDn()
{

    cVectorf::SineCosine(R_AngElecCmd,&R_SinCosCmd);

    cVectorf::RotateLN(R_UxCmd,R_UyCmd,&R_SinCosCmd,&R_UaCmd,&R_UbCmd);
    R_UcCmd = (-(R_UaCmd + R_UbCmd));


    /*************************************************************************
     * CALCULATE CURRENT COMMANDS FOR MODULATOR LOCKOUT COMPENSATION
     *************************************************************************/
    /*
     * Rotate Current Command to yield Phase Current Commands
     */

    cVectorf::RotateLN(R_IxCmd0*R_LkoutGnSclT1, R_IyCmd0T1*R_LkoutGnSclT1,&R_SinCosCmd,&R_IaCmdLkout,&R_IbCmdLkout);

    R_IcCmdLkout = (-(R_IaCmdLkout + R_IbCmdLkout));

}

