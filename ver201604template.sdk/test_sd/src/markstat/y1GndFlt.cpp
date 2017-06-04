/*=============================================================================
* Product:     AccuWave Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        Y1GNDFLT.C30
* Language:    TI 'C'
* Copyright:   1996..1998 GE Industrial Control Systems, Salem VA
*
* Abstract: This file contains functions which pertain to Ix, Iy fault checking.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* InvGndFltDetectT1          - ground current fault detection
* InvGndFltDetectT3
*
*==============================================================================
* 970401 SAB  Create for general fault checking from Y0FLTCHK
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 030610 DS   Disable discriminated ground faults when rotor not enabled
*______________________________________________________________________________
* Last Mod: June 10, 192003 5:09:46PM
*/


// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
// product
#include "Product.h"
#include "b0CelTst.h"
#include "y1GndFlt.h"

// Externals
//----------
extern unsigned  Q1OpnReq;

// Parameters
//-----------
CREATE_PARM(R_WIxIyGnd,         float);     // Ix & Iy ground current filter omega
CREATE_PARM(R_WIxyGnd,          float);     // XY ground current filter omega
CREATE_PARM(R_GndAlmOff,        float);     // Discriminated ground alarm off level
CREATE_PARM(R_GndAlmOn,         float);     // Discriminated ground alarm on level
CREATE_PARM(R_GndFltThrs,       float);     // Discriminated ground fault trip threshold


// Variables
//----------
/* Inverter ground fault detection variables */
CREATE_PUBVAR(R_IxGndT1, float);
CREATE_PUBVAR(R_IyGndT1, float);
CREATE_PUBVAR(R_IxyGnd, float);
CREATE_PUBVAR(R_IxIyGndGn, float);
CREATE_PUBVAR(R_IxyGndGn, float);
CREATE_PUBVAR(R_IxGndFil, float);
CREATE_PUBVAR(R_IyGndFil, float);
CREATE_PUBVAR(R_IxyGndFil, float);

// Diagnostics
//------------
CREATE_DIAG(R_InvGndAlm);           // Discriminated inverter ground warning
CREATE_DIAG(R_InvGndFlt);           // Discriminated inverter ground fault

/*****************************************************************************
*
*       external references
*
*****************************************************************************/
extern float    R_IabcGnd;
extern float    DelTm3;
extern unsigned R_BrgPwrEnb;
extern float    R_IxGndT3;
extern float    R_IyGndT3;

extern SinCosTyp   R_SinCosFbk;
extern cCellTest   R_CellTest;


/* **************************************************************************
*
*                               Private Variables
*
*****************************************************************************/



/*******************************************************************************
* DESCRIPTION:
*     Excessive ground current detection basedon A-B-C quantities.
*     Discriminates the summed phas currents into X and Y components.
*
* EXECUTION LEVEL
*      Task 1
*
* RETURN VALUE
*      none
*******************************************************************************/
void RotorInvGndFltDetectT1( void )
{
    cVectorf::Discrim( R_IabcGnd, &R_SinCosFbk, &R_IxGndT1, &R_IyGndT1 );

    if ( R_CellTest.CelTstProtEnab == false )
    {
        R_IxGndT1 = 0.0F;
        R_IyGndT1 = 0.0F;
    }

    return;
}


/*******************************************************************************
* Function:        RotorInvGndFltDetectT3
* Execution Level: Task 3
*
* DESCRIPTION:
*      Calculates the magnitude of the ground current, filters it, and
*      checks the result against fault levels.
*
* RETURN VALUE
*       none
*
*******************************************************************************/

void RotorInvGndFltDetectT3( void )
{
        /* Calculate filter gains */

    R_IxyGndGn   = PARM(R_WIxyGnd) * DelTm3;
    R_IxIyGndGn = PARM(R_WIxIyGnd) * DelTm3;

    if (!R_BrgPwrEnb)
    {
        R_IxGndFil  = 0.0F;
        R_IyGndFil  = 0.0F;
        R_IxyGnd    = 0.0F;
        R_IxyGndFil = 0.0F;
        return;
    }

        /* Filter X and Y components of ground current */

    R_IxGndFil += R_IxIyGndGn * ( R_IxGndT3 - R_IxGndFil );
    R_IyGndFil += R_IxIyGndGn * ( R_IyGndT3 - R_IyGndFil );

        /* Calculate magnitude of ground current */

    R_IxyGnd = sqrtf(( R_IxGndFil * R_IxGndFil ) + ( R_IyGndFil * R_IyGndFil ));

        /* Filter ground current magnitude */

    R_IxyGndFil += R_IxyGndGn * ( R_IxyGnd - R_IxyGndFil );

        /* Check for fault conditions */

    if ( R_IxyGndFil >= PARM(R_GndFltThrs) )
    {
        PUSH_DIAG( R_InvGndFlt );
        Q1OpnReq = true;
    }
    if ( R_IxyGndFil >= PARM(R_GndAlmOn) )
    {
        PUSH_DIAG( R_InvGndAlm );
    }

    return;
}
