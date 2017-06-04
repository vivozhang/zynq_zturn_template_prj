/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        TMS320VC33 DSP
* File:        S0FLTCHK.C30
* Language:    TI 'C'
* Copyright:   2003 GE Motors and Industrial Systems, Salem VA
*
* Abstract: This file contains functions which pertain to fault checking in
*           all tasks.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* ProcessFaultFbks           - porcesses fault feedbacks.
*
*==============================================================================
* 970527  SAB Create. Simplify Inline macros. Remove unneeded externs.
* 980930  SAB Add several vars to be cleared at Sim_Mode --> !Sim_Mode trans.
* 981118  SAB Require "Running" to set Reg_Lim_Hist
* 981118  SAB Add "Running" requirement to set Reg_Lim_Hist on trip fault as well.
* 991214  SAB Add function RequiredPrmFlt
* 991223  SAB Remove RequiredPrmFlt function (move to s0rule)
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 021121 SAB  L_LSimMode --> SimMode
* 030929 SAB  Change population of L_RegLimHist so that it retains all
*             history by 'or'ing with L_RegLimSum for briefs and 'copying'
*             for trip faults.
* 070604 SAB  Use Iram copies of parameters for EEs
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
*______________________________________________________________________________
* Last Mod: December 12, 2007 3:26:13PM
*/


#include "x0FrameWork.h"
#include "Product.h"


#define  REG_LIM_MASK     0x03FF

// Parameters
//-----------
CREATE_PARM(L_RegLimTm,         float);     // Regulator limit maximum time

// Diagnostics
//------------
CREATE_DIAG(L_RegLimAlm);           // Regulator clamp in limit(brief)
CREATE_DIAG(L_RegLimFlt);           // Regulator clamp in limit(trip)

// Variables
//----------
CREATE_PUBVAR(L_RegLimTmr   ,float      );           // Inner regulator limit timer
CREATE_PUBVAR(L_RegLimSum   ,unsigned   );           // regulator limit sum (task 2)
CREATE_PUBVAR(L_RegLimHist  ,unsigned   );           // regulator limit history
CREATE_PUBVAR(SimModeZ1     ,unsigned   );           // simulator mode old value
CREATE_PUBVAR(L_RegInLim    ,unsigned   );           // regulator in limit
CREATE_PUBVAR(L_InnrLimFil  ,float      );           // Filter state for L_InnrLimT2
CREATE_PUBVAR(L_InnrLimT2   ,unsigned   );           // Inner regulator in limit



/*****************************************************************************
*
*       external references
*
*****************************************************************************/
extern float        DelTm2                  ;
extern unsigned     L_InnrRegEnb            ;
extern unsigned     L_InnrRegORT2           ;
extern unsigned     SimMode                 ;






/* **************************************************************************
*
*                               Private Variables
*
*****************************************************************************/


/*=============================================================================
 * Function: FaultChkSourceLCL
 * Execution Level: Task 2
 * Execution Time: Untimed
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  L_InnrLimT2
 *                 L_InnrLimFil
 *                 L_RegLimSum
 *
 *                 EE.L_RegLimTm
 *
 * Global Outputs: Flt.Reg_Lim
 *                 L_RegLimHist
 *                 L_PllPrvFstp
 *
 *
 * Locals:         L_RegLimTmr
 *
 *
 *
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function performs fault checking for task2 as needed.
 *
 *===========================================================================*/


void FaultChkSourceLCL(void)
{

    /*=======================================================================*/
    /*                Regulator Saturation Limit Check                       */
    /*=======================================================================*/

    //Check for regulator limit alarm flags
    L_RegLimSum = REG_LIM_MASK & L_InnrRegORT2 ;
    if ( L_RegLimSum != 0 )        /* Inner reg. limit check */
    {
        L_InnrLimT2 = true;
    }
    else
    {
        L_InnrLimT2 = false;
    }


    L_InnrLimFil = L_InnrLimFil + DelTm2*PARM(L_RegLimW)*(L_InnrLimT2 - L_InnrLimFil);


    if (L_InnrLimFil >= .6321)
    {
        L_RegInLim = true;
    }
    else if (L_InnrLimFil <= .3679)
    {
        L_RegInLim = false;
    }

    if ( L_RegInLim)
    {
         L_RegLimTmr = L_RegLimTmr + DelTm2;
         PUSH_DIAG(L_RegLimAlm);
         if ( L_InnrRegEnb)
         {
            L_RegLimHist |= L_RegLimSum;
         }
    }
    else
    {
         L_RegLimTmr = L_RegLimTmr - DelTm2;
         if ( L_RegLimTmr <= 0.0)
         {
              L_RegLimTmr = 0.0;          /* limit timer to 0 on lower end */
         }
    }

    if ( L_RegLimTmr >= PARM(L_RegLimTm) )
    {
         PUSH_DIAG(L_RegLimFlt);
         L_RegLimTmr  = PARM(L_RegLimTm);  /* limit timer to EE.L_RegLimTm*/
         if ( L_InnrRegEnb)
         {
           L_RegLimHist |= L_RegLimSum;
         }
    }






    SimModeZ1 = SimMode;


}
