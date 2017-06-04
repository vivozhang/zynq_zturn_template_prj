/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        TMS320C31 DSP
* File:        $S0DEMROT.C30
* Language:    TI 'C'
* Copyright:   2003... GE Motors and Industrial Systems, Salem VA
*
* Abstract: This file contains those functions which are associated
*           demodulation and rotation of current, voltage and flux.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* Demodulate                   -  Demodulate input currents and voltages
* LineVoltsSimulator           -  Line voltage simulator
*
*==============================================================================
* 970120 0001 SAB Create for voltage and current demodulation from A0VECT.C30.
* 970120 0002 SAB Include S0EEEMUL.H, not A0EEEMUL.H
* 970124 0003 SAB Add SimulateVolts function.
* 970127 0004 SAB Use L_Ang_Elec_Fbk, not Ang_Sim for line demodulator
* 970130 0005 SAB Rename SimulateLineVolts to LineVoltsSimulator. Combine
*                 DemodVolts and DemodCurrent into Demodulate.
* 970205 0006 SAB Rename LineVoltsSimulator to VoltsSimulator
* 970206 0007 SAB Clean up Vab_Sim, Vbc_Sim generation
* 970214 0008 SAB Rename Ang_Sim to L_Ang_Line_Sim
* 970219 0009 SAB Add RotateCurrentSim
* 970318 SAB  Header file comments and use products.h
* 970324 SAB  Move ram defs internal and use a header file.
* 970325 SAB  Add missed internal ram.
* 970403 SAB  Remove Vdc_Inst
*             Rename Vx_Inst_Sim, Vy_Inst_Sim --> Vx_Brdg_Sim, Vy_Brdg_Sim
* 970729 SAB  Add Transient short/open capability
* 970911 SAB  Exclude RotateUpDn from S3Source_Code
* 971120 SAB  Zero declared vars here, not in s0init.
* 980122 SAB  Fix name in header
* 980217 SAB  Rep rate for shorts is not 2.5sec
* 980519 SAB  Remove SimulateVolts vars and code.
* 980603 SAB  Remove functions and vars for Vab, Vbc_Sim, Vx_,Vy_Brdg_Sim
* 990128 SAB  Share with ACCBN_A
* 990203 SAB  Fix loss of rotate commands via conditional compile for
*             ACMVRS6-A
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 051212 BAG  remove extern from variable definition
* 070127 SAB  Remove In-lining from RotateUpDn
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 091124 SAB  Use filtered version of current and voltage for demod
*______________________________________________________________________________
* Last Mod: November 24, 2009 1:43:26PM
*/

// Includes Files
//---------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
// product
#include "Product.h"



/******************************************************************************/
/*  Definitions                                                               */
/******************************************************************************/
/* *Vars */



    /* Demodulator function */

CREATE_PUBVAR(L_IaCmdLkout    ,float);          // phase A current command for up-down lockout
CREATE_PUBVAR(L_IbCmdLkout    ,float);          // phase B current command for up-down lockout
CREATE_PUBVAR(L_IcCmdLkout    ,float);          // phase C current command for up-down lockout
CREATE_PUBVAR(L_Ix            ,float);          // x-component of demoduled current
CREATE_PUBVAR(L_Iy            ,float);          // y-component of demoduled current
CREATE_PUBVAR(L_UaCmd         ,float);          // Phase A modulation command
CREATE_PUBVAR(L_UbCmd         ,float);          // Phase B modulation command
CREATE_PUBVAR(L_UcCmd         ,float);          // Phase C modulation command
CREATE_PUBVAR(L_Vx            ,float);          // x-component of demodulated voltage
CREATE_PUBVAR(L_Vy            ,float);          // y-component of demodulated voltage


CREATE_PUBVAR(L_IxFbkMet      ,float);          // ix feedback component for Stator RmsMeter
CREATE_PUBVAR(L_IyFbkMet      ,float);          // iy feedback component for Stator RmsMeter
CREATE_PUBVAR(L_VxFbkMet      ,float);          // vx feedback component for Stator RmsMeter
CREATE_PUBVAR(L_VyFbkMet      ,float);          // vy feedback component for Stator RmsMeter




/*****************************************************************************
*
*       external references
*
*****************************************************************************/


//    External vars
extern SinCosTyp    GmSinCosFbk   ;
extern SinCosTyp    GmSinCosCmd   ;
extern SinCosTyp    L_SinCosFbk   ;
extern SinCosTyp    L_SinCosCmd   ;
extern float        GmAngElecCmd  ;
extern float        L_AngElecCmd  ;
extern float        L_IaFil       ;
extern float        L_IbFil       ;
extern float        L_IxCmd0      ;
extern float        L_IyCmd0      ;
extern float        L_UxCmd       ;
extern float        L_UyCmd       ;
extern float        L_VabFil      ;
extern float        L_VbcFil      ;

extern float        L_Ia          ;
extern float        L_Ib          ;
extern float        L_Ic          ;
extern float        L_Vab         ;
extern float        L_Vbc         ;

extern float        L_LkoutGnSclT1;

/* **************************************************************************
*
*                               Private Variables
*
*****************************************************************************/


/*=============================================================================
 * Function: Demodulate
 * Execution Level: Task 1
 * Execution Time:  6.28/6.32usec (sim/non-sim) 12/19/96--Rev:001.01.01.A
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  GmAngElecFbk
 *
 *
 *
 *
 *
 * Global Outputs: L_Ix
 *                 L_Iy
 *
 *
 * Locals:
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function performs the demodulation of source currents and voltages
 * into the x-y coordinate system.
 *
 *===========================================================================*/


void Demodulate()
{
    cVectorf::DemodLN(L_IaFil,L_IbFil,  &L_SinCosFbk,&L_Ix,&L_Iy);
    cVectorf::DemodLL(L_VabFil,L_VbcFil,&L_SinCosFbk,&L_Vx,&L_Vy);

// Strictly, the angle for these transformations should be "L_AngElecFbk",
// such that "L_SinCosFbk" would be used in the instrument below...
    cVectorf::DemodLN3(L_Ia,L_Ib,L_Ic,  &L_SinCosFbk, &L_IxFbkMet,&L_IyFbkMet);
    cVectorf::DemodLL (L_Vab,L_Vbc,     &L_SinCosFbk, &L_VxFbkMet,&L_VyFbkMet);
}



/*=============================================================================
 * Function: RotateUpDn
 * Execution Level: Task 1
 * Execution Time: 6.30usec       12/19/96--Rev:001.01.01.A
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  L_UaCmd
 *                 L_UaCmdM
 *                 L_UbCmd
 *                 L_UbCmdM
 *                 L_UcCmd
 *                 L_UcCmdM
 *                 L_UxCmd
 *                 L_UyCmd
 *                 L_UyCmdz1
 *                 L_VabSim
 *                 L_VbcSim
 *                 L_UxCmd
 *                 L_UyCmd
 *                 L_SinCosCmd
 *
 *
 *
 * Global Outputs: L_VxFbk
 *                 L_VyFbk
 *                 L_IaCmdLkout
 *                 L_IbCmdLkout
 *                 L_IcCmdLkout
 *
 * Locals:         none
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function performs the rotatation of motor voltages into the
 * x-y coordinate system.
 *
 *===========================================================================*/

void RotateUpDn()
{

    cVectorf::SineCosine(GmAngElecCmd,&GmSinCosCmd);
    cVectorf::SineCosine(L_AngElecCmd,&L_SinCosCmd);

    cVectorf::RotateLN(L_UxCmd,L_UyCmd,&L_SinCosCmd,&L_UaCmd,&L_UbCmd);
    L_UcCmd = (-(L_UaCmd + L_UbCmd));

    /*************************************************************************
     * CALCULATE CURRENT COMMANDS FOR LOCKOUT COMPENSATION
     *************************************************************************/
    /*
     * Rotate Current Command to yield Phase Current Commands
     */
    cVectorf::RotateLN(L_IxCmd0*L_LkoutGnSclT1,L_IyCmd0*L_LkoutGnSclT1,&L_SinCosCmd,&L_IaCmdLkout,&L_IbCmdLkout);
    L_IcCmdLkout = (-(L_IaCmdLkout + L_IbCmdLkout));

}

