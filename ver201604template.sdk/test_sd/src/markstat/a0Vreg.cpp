/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        A0VREG.C30
* Language:    TI 'C'
* Copyright:   2003..GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file contains those functions which are associated with
*           the voltage regulator.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* InnrRegAlrm               - Inner regulator alarm faults
* VoltReg                   - voltage regulator
*
*==============================================================================
* 961104 0000 SAB  Create for voltage regulation.  Create VoltReg.
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 031218 SAB  Remove slow functions to A0VREGS.C30
* 070625 SAB  Rename dc ireg outputs
* 070706 SAB  Use iram copies for parms
* 070712 SAB  R_Rs --> S_R2 so that R_Rs can include cable resistance
*______________________________________________________________________________
* Last Mod: July 12, 2007 5:49:34PM
*/

#include "product.h"
#include "x0FrameWork.h"
#include "a0Vreg.h"

#define Alm_Lmt_Tm  1.0    // sec.


// Parameters
//-----------
CREATE_PARM(R_VImpFFGnY,        float);     // voltage feed-forward impulse gain for y channel
CREATE_PARM(R_VxregUxLim,       float);     // rotor x-voltage regulator x-channel mod index limit
CREATE_PARM(R_VyregUyLim,       float);     // rotor y-voltage regulator y-channel mod index limit
CREATE_PARM(R_ModIndMax,        float);     // modulated index maximum
CREATE_PARM(R_VImpFFGn,         float);     // voltage feed-forward impulse gain
CREATE_PARM(R_VxregIntGn,       float);     // x-voltage regulator integral gain
CREATE_PARM(R_VxregLim,         float);     // x-voltage regulator integral limit
CREATE_PARM(R_VyregIntGn,       float);     // x-voltage regulator integral gain
CREATE_PARM(R_VyregLim,         float);     // y-voltage regulator integral limit

// Diagnostics
//------------
CREATE_DIAG(R_IregSatAlm);          // Current reg saturation alarm
CREATE_DIAG(R_VregSatAlm);          // Voltage reg saturation alarm

// Variables
//-----------
CREATE_PUBVAR(R_USqr,       float  );   // Ux^2 + Uy^2
CREATE_PUBVAR(R_USqrFil,    float  );   // Filtered Ux^2 + Uy^2
CREATE_PUBVAR(R_FlxLkgCmd,  float  );   // leakage flux command
CREATE_PUBVAR(R_FlxLkgCmdz, float  );   // old value of flux leakage command
CREATE_PUBVAR(R_UxCmd,      float  );   // voltage reg. x component output
CREATE_PUBVAR(R_UyCmd,      float  );   // voltage reg. y component output
CREATE_PUBVAR(R_UxCmdUc,    float  );   // voltage reg. x component output unclamped
CREATE_PUBVAR(R_UyCmdUc,    float  );   // voltage reg. y component output unclamped
CREATE_PUBVAR(R_VxCmd,      float  );   // x-component voltage command
CREATE_PUBVAR(R_VyCmd,      float  );   // y-component voltage command
CREATE_PUBVAR(R_VxFfCmd,    float  );   // x-component feedforward voltage ref
CREATE_PUBVAR(R_VyFfCmd,    float  );   // y-component feedforward voltage ref
CREATE_PUBVAR(R_VxFfZ0,     float  );   // x-voltage feed forward old value
CREATE_PUBVAR(R_VxFfZ1,     float  );   // (z-1)x-voltage feed forward value
CREATE_PUBVAR(R_VxFfZ2,     float  );   // (z-2)x-voltage feed forward value
CREATE_PUBVAR(R_VxFlx,      float  );   // x-component flux
CREATE_PUBVAR(R_VxImpls,    float  );   // x-component voltage impulse
CREATE_PUBVAR(R_VxregInt,   float  );   // x-component voltage reg. integrator
CREATE_PUBVAR(R_VyFfZ0,     float  );   // y-voltage feed forward old value
CREATE_PUBVAR(R_VyFfZ1,     float  );   // (z-1)y-voltage feed forward value
CREATE_PUBVAR(R_VyFfZ2,     float  );   // (z-2)y-voltage feed forward value
CREATE_PUBVAR(R_VyregInt,   float  );   // y-component voltage reg. integrator
CREATE_PUBVAR(R_IregAlmTm,  float  );   // Current reg saturation alarm timer
CREATE_PUBVAR(R_VregAlmTm,  float  );   // Volt reg saturation alarm timer

CREATE_PUBVAR(R_FlxLkgCmdY, float  );   /* leakage flux command - Y */
CREATE_PUBVAR(R_FlxLkCmdYz, float  );   /* old value of flux leakage command - y */
CREATE_PUBVAR(R_VyImpls,    float  );   /* y-component voltage impulse */
CREATE_PUBVAR(R_VyFfCmdT1,  float  );   /* y-component feedforward voltage ref t1 */

CREATE_PUBVAR(R_VrgIntGnDy, float  );   /* dynamic voltage reg integral gain*/
CREATE_PUBVAR(R_VrgIntLmDy, float  );   /* dynamic voltage reg integral limit*/


/*****************************************************************************
*
*       external references
*
*****************************************************************************/

extern float DelTm3;
extern float R_DelTm1;
extern float R_DelTm1Rcp;
extern float R_FlxCmdDdtT1;
extern float R_FlxCmdT1;

extern float R_IxCmd0;
extern float R_IxFbk;
extern float R_IxregDcOut;
extern float R_IxregInt;
extern float R_IxregRsOut;
extern float R_IyCmd0T1;
extern float R_IyregDcOut;
extern float R_IyregInt;
extern float R_IyregRsOut;
extern float R_R2;
extern float R_VdcComp;
extern float R_VdcCompRcp;

extern float R_VxFbk;
extern float R_VxReg;
extern float R_VyFbk;
extern float R_VyReg;
extern float R_WElecFf;

extern float S_VMaxRegGn;
extern float R_IyCmd0;
extern float R_IyCmd1;
extern float R_Rr;
extern float R_FlxCmdT2T1;

extern unsigned S_VMaxHiDet;

/******************************************************************************/
/*  Functions                                                                 */
void RotorInnrRegAlrm(void);

/*=============================================================================
 * Function: VoltReg
 * Execution Level: Task 1
 * Execution Time: 10.40usec     12/19/96--Rev:001.01.01.A
 * Formal Inputs: None
 * Formal Outputs: None
 *
 *
 * Global Inputs: R_DelTm1
 *                R_DelTm1Rcp
 *                Est_R1
 *                R_IxCmd0
 *                R_ModIndFil
 *                Vdc__Fbk
 *                R_VxFbk
 *                R_VxReg
 *                R_VxregInt
 *                R_VyFbk
 *                R_VyReg
 *                R_VyregInt
 *                R_WElecFf
 *                R_VxFlx
 *                Sim_Mode
 *                CI_R_LSigma
 *                CI_R_VdcNom
 *                CI_R_VxregIntGn
 *                CI_R_VxregLim
 *                CI_R_VyregIntGn
 *                CI_R_VyregLim
 *                CI_V_Imp_FF_GN
 *
 *
 * Global Outputs: R_USqrFil
 *                 R_UxCmd
 *                 R_UyCmd
 *                 R_VxCmd
 *                 R_VyCmd
 *
 *
 *
 * Locals:        R_FlxLkgCmd
 *                R_FlxLkgCmdz
 *                R_VxFfZ0
 *                R_VxFfZ1
 *                R_VxFfZ2
 *                R_VxregInt
 *                R_VyregInt
 *                R_VyFfCmd
 *                R_VxFfCmd
 *                R_VxImpls
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function implements the voltage regulator and the voltage feed-
 * forward model.  The voltage feed-forward model is performed first.
 *
 *===========================================================================*/
void RotorVoltReg()
{
    float   Del_Vx_Err;
    float   R_IyCmdX;
    float   R_IxCmdX;
    float   R_FlxCmdX;
    float   R_VyImplsGn;

    ///////////////////////////////////////////////////////////////////////////
    // Start Voltage Feed-Forward Model -- TRQ_050
    ///////////////////////////////////////////////////////////////////////////

        //Model selection based on ac voltage hi detection for islanding and HVRT
    if ( S_VMaxHiDet)
    {
        R_IxCmdX     = R_IxCmd0;
        R_IyCmdX     = R_IyCmd1;
        R_FlxCmdX    = R_FlxCmdT1;                      // calculated in T1

        R_VyImplsGn  = 1.0;
        S_VMaxRegGn  = 0.0;

    }
    else
    {
        R_IxCmdX     = (R_IxFbk+R_IxCmd0)*0.5;
        R_IyCmdX     = R_IyCmd0;
        R_FlxCmdX    = R_FlxCmdT2T1;                    // R_FlxCmd, data-passed from T2

        R_VyImplsGn  = 0.0;
        S_VMaxRegGn  = 1.0;
    }

     /* ---- Leakage flux model ----- */
    R_FlxLkgCmd  = PARM(R_LSigma) * R_IxCmd0;
    R_FlxLkgCmdY = PARM(R_LSigma) * R_IyCmdX;


    /* ----- voltage Feedforward X Channel ---- */
    R_VxImpls    = PARM(R_VImpFFGn) * R_DelTm1Rcp * (R_FlxLkgCmd - R_FlxLkgCmdz);
    R_VxFlx      = R_Rr * R_IxCmd0 - ( R_FlxCmdX * R_WElecFf );
    R_VxFfCmd    = R_VxImpls + R_VxFlx;
    R_FlxLkgCmdz = R_FlxLkgCmd;


    /* ----- voltage Feedforward Y Channel ---- */
    R_VyImpls    = PARM(R_VImpFFGnY) * R_DelTm1Rcp * (R_FlxLkgCmdY - R_FlxLkCmdYz);
    R_VyFfCmdT1  = (R_Rr * R_IyCmdX) + R_FlxCmdDdtT1 + R_VyImpls;
    R_VyFfCmd    = (R_WElecFf * PARM(R_LSigma) * R_IxCmdX ) + R_VyFfCmdT1;
    R_FlxLkCmdYz = R_FlxLkgCmdY;


///////////////////////////////////////////////////////////////////////////
// End Voltage Feed-Forward Model -- TRQ_050
///////////////////////////////////////////////////////////////////////////


    /*=======================================================================*/
    /*                Voltage Regulation and Modulation Index                */
    /*=======================================================================*/

    /* Form feed-forward delays */
    R_VxFfZ2 = R_VxFfZ1;
    R_VxFfZ1 = R_VxFfZ0;
    R_VxFfZ0 = R_VxReg + R_VxFfCmd;

    R_VyFfZ2 = R_VyFfZ1;
    R_VyFfZ1 = R_VyFfZ0;

    R_VyFfZ0 = R_VyReg + R_VyFfCmd;


    Del_Vx_Err = R_VrgIntGnDy * (R_VxFfZ2 - R_VxFbk);


    /*  Calculate and clamp R_VxregInt, calculate R_UxCmd */
    R_VxregInt = R_VxregInt + Del_Vx_Err;

    if ( R_VxregInt > R_VrgIntLmDy)
    {
         R_VxregInt = R_VrgIntLmDy;
    }
    else if ( R_VxregInt < (-R_VrgIntLmDy))
    {
        R_VxregInt  = (-R_VrgIntLmDy);
    }

    // ==============  Form R_VxCmd =================
    R_VxCmd   = (R_VxFfZ0 + R_VxregInt);
    R_UxCmdUc = R_VdcCompRcp * R_VxCmd + R_IxregDcOut + R_IxregRsOut;
    R_UxCmd   = R_UxCmdUc;
    // rotor modulation index X channel limit
    if ( R_UxCmd > PARM(R_VxregUxLim) )
    {
        R_UxCmd = PARM(R_VxregUxLim);
    }
    else if ( R_UxCmd < (-PARM(R_VxregUxLim)))
    {
        R_UxCmd = -PARM(R_VxregUxLim);
    }

    //  Calculate and clamp R_VyregInt, calculate R_UyCmd
    R_VyregInt = R_VyregInt + R_VrgIntGnDy * ( R_VyFfZ2 - R_VyFbk );

    if (R_VyregInt > R_VrgIntLmDy)
    {
        R_VyregInt = R_VrgIntLmDy;
    }
    else if (R_VyregInt < (-R_VrgIntLmDy))
    {
        R_VyregInt  = (-R_VrgIntLmDy);
    }

    R_VyCmd   = (R_VyFfZ0 +R_VyregInt);
    R_UyCmdUc = R_VdcCompRcp * R_VyCmd + R_IyregDcOut + R_IyregRsOut;
    R_UyCmd   = R_UyCmdUc;

    // rotor modulation index Y channel limit
    if ( R_UyCmd > PARM(R_VyregUyLim) )
    {
        R_UyCmd = PARM(R_VyregUyLim);
    }
    else if ( R_UyCmd < (-PARM(R_VyregUyLim)))
    {
        R_UyCmd = -PARM(R_VyregUyLim);
    }

    // Calculate R_USqr
    R_USqr = (R_UxCmdUc*R_UxCmdUc) + (R_UyCmdUc*R_UyCmdUc);

}


/*********************************************************************
**
**    Inner Regulator Saturation alarms
**    ---------------------------------
**    Task 3
**
**    Vreg saturation      (slow alarm)
**    Ireg saturation      (slow alarm)
**
*********************************************************************/
void RotorInnrRegAlrm(void)
{
    // ----------- Current regulators -------------

    if((ABS(R_IxregInt) >= PARM(R_IxregLim)) || (ABS(R_IyregInt) >= PARM(R_IyregLim)))
    {
        R_IregAlmTm = R_IregAlmTm + DelTm3;
        if(R_IregAlmTm > Alm_Lmt_Tm)
        {
            R_IregAlmTm = Alm_Lmt_Tm;
            PUSH_DIAG(R_IregSatAlm);
        }
    }
    else
    {
        R_IregAlmTm = R_IregAlmTm - DelTm3;
        if(R_IregAlmTm < 0.0)
        {
            R_IregAlmTm = 0.0;
            //Pop3Flt(R_IregSatAlm);
        }
    }


    // -------------- Voltage regulators ---------------

    if((ABS(R_VxregInt) > R_VrgIntLmDy) || (ABS(R_VyregInt) > R_VrgIntLmDy) )
    {
        R_VregAlmTm = R_VregAlmTm + DelTm3;
        if(R_VregAlmTm > Alm_Lmt_Tm)
        {
            R_VregAlmTm = Alm_Lmt_Tm;
            PUSH_DIAG(R_VregSatAlm);
        }
    }
    else
    {
        R_VregAlmTm = R_VregAlmTm - DelTm3;
        if(R_VregAlmTm < 0.0)
        {
            R_VregAlmTm = 0.0;
            //Pop3Flt(R_VregSatAlm);
        }
    }


} // end InnrRegAlrm


