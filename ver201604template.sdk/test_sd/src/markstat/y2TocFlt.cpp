/*=============================================================================
* Product:     PWM Source Using DSPX Based Control
* Host:        TMS320C31 DSP
* File:        $\Y1TOCFLT.C30
* Language:    TI 'C'
* Copyright:   1996 GE Motors and Industrial Systems, Salem VA
*
* Abstract: This file contains functions which pertain to checking for TOC
*           faults in any product which wants the function.
*
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* ProcessFaultFbks           - porcesses fault feedbacks.
* TocDetection               - timed overcurrent detection.
*
*==============================================================================
* 970401 SAB  Create for TOC detection.
* 980608 SAB  Move Ia,Ib,Ic_Sqr to Fbk file
* 980608 SAB  Realyy move Ia,Ib,Ic_Sqr to Fbk file
* 020125 SAB  Calculate Ia_Rms, Ib_Rms, and Ic_Rms
* 021120 SAB  Change naming convention: Remove all under-scores and add
*             "L_" on line converter names.
* 021210 SAB  Change name to Y2TOCFlt.c30
* 031119 SAB  Move ProcessFaultFbks to Y2FltCk.c30 and rename to LineProcessFaultFbks
* 070310 SAB  Remove toc cool stuff
*______________________________________________________________________________
* Last Mod: March 10, 2007 8:31:53AM
*/

#include "x0FrameWork.h"
#include "Product.h"

/*****************************************************************************
*
*       external references
*
*****************************************************************************/

extern  float    L_IaSqr;
extern  float    L_IbSqr;
extern  float    L_IcSqr;
extern  float    L_FuseIaSqr;
extern  float    L_FuseIbSqr;
extern  float    L_FuseIcSqr;
extern  float    L_MaxCurOut;
extern  float    L_FuseMaxCurOut;
extern  unsigned DelSpdPAct;

// Parameters
//------------------
CREATE_PARM( L_TocAlmOff,       float);     // Timed over-current alarm "off" threshold
CREATE_PARM( L_TocAlmOn,        float);     // Timed over-current alarm "on" threshold
CREATE_PARM( L_TocTau,          float);     // Timed over-current time constant
CREATE_PARM( L_TocThrs,         float);     // Timed over-current threshold
CREATE_PARM( L_TocMargin,       float);     // Timed over-current margin for line converter

CREATE_PARM( L_FuseTocAlmOff,   float);     // Ac line fuse Timed over-current alarm "off" threshold
CREATE_PARM( L_FuseTocAlmOn,    float);     // Ac line fuse Timed over-current alarm "on" threshold
CREATE_PARM( L_FuseTocTau,      float);     // Ac line fuse Timed over-current time constant
CREATE_PARM( L_FuseTocThrs,     float);     // Ac line fuse Timed over-current threshold
CREATE_PARM( L_FuseTocMargin,   float);     // Timed over-current margin for ACLFU

// Line Converter Toc Detection Vars
CREATE_PUBVAR(L_WTocT3       ,  float );    // Omega for timed over-current (normal) - task 3
CREATE_PUBVAR(L_WTocFstT3    ,  float );    // Omega for timed over-current (fast) - task 3
CREATE_PUBVAR(L_WTocT3Dyn    ,  float );    // Omega for timed over-current - task 3
CREATE_PUBVAR(L_IaRmsMag     ,  float );    // Phase A current rms magnitude
CREATE_PUBVAR(L_IbRmsMag     ,  float );    // Phase B current rms magnitude
CREATE_PUBVAR(L_IcRmsMag     ,  float );    // Phase C current rms magnitude
CREATE_PUBVAR(L_IaRms        ,  float );    // Phase A rms current
CREATE_PUBVAR(L_IbRms        ,  float );    // Phase B rms current
CREATE_PUBVAR(L_IcRms        ,  float );    // Phase C rms current
CREATE_PUBVAR(L_TocHiSel     ,  float );    // Toc high select
CREATE_PUBVAR(L_TocHiSelSq   ,  float );    // Toc high select squared
CREATE_PUBVAR(L_TocAlmFlg    ,  unsigned);  // line side bridge TOC alarm flg
CREATE_PUBVAR(L_IaSqrT3      ,  float);
CREATE_PUBVAR(L_IbSqrT3      ,  float);
CREATE_PUBVAR(L_IcSqrT3      ,  float);
CREATE_PUBVAR(L_IaSqrAvg     ,  float);
CREATE_PUBVAR(L_IbSqrAvg     ,  float);
CREATE_PUBVAR(L_IcSqrAvg     ,  float);

// Line Converter Dynamic Toc threshold
CREATE_PUBVAR(L_TocThrDy  ,     float   ); // Line converter time over-current dymanic trip threshold (Amp)
CREATE_PUBVAR(L_TocAlmOnDy,     float   ); // Line converter time over-current dymanic alarm on threshold (Amp)
CREATE_PUBVAR(L_TocAlmOffDy,    float   ); // Line converter time over-current dymanic alarm off threshold (Amp)

// ACLFU Dynamic Toc threshold
CREATE_PUBVAR(L_FuseTocThrDy  ,     float);     // ACLFU time over-current dymanic trip threshold (Amp)
CREATE_PUBVAR(L_FuseTocAlmOnDy,     float);     // ACLFU time over-current dymanic alarm on threshold (Amp)
CREATE_PUBVAR(L_FuseTocAlmOffDy,    float);     // ACLFU time over-current dymanic alarm off threshold (Amp)

// Aclfu TocDetection Vars
CREATE_PUBVAR(L_FuseWTocT3       ,  float );    // AC line fuse Omega for timed over-current (normal) - task 3
CREATE_PUBVAR(L_FuseWTocFstT3    ,  float );    // AC line fuse Omega for timed over-current (fast) - task 3
CREATE_PUBVAR(L_FuseWTocT3Dyn    ,  float );    // AC line fuse Omega for timed over-current - task 3
CREATE_PUBVAR(L_FuseIaRmsMag     ,  float );    // AC line fuse Phase A current rms magnitude
CREATE_PUBVAR(L_FuseIbRmsMag     ,  float );    // AC line fuse Phase B current rms magnitude
CREATE_PUBVAR(L_FuseIcRmsMag     ,  float );    // AC line fuse Phase C current rms magnitude
CREATE_PUBVAR(L_FuseIaRms        ,  float );    // AC line fuse Phase A rms current
CREATE_PUBVAR(L_FuseIbRms        ,  float );    // AC line fuse Phase B rms current
CREATE_PUBVAR(L_FuseIcRms        ,  float );    // AC line fuse Phase C rms current
CREATE_PUBVAR(L_FuseTocHiSel     ,  float );    // AC line fuse Toc high select
CREATE_PUBVAR(L_FuseTocHiSelSq   ,  float );    // AC line fuse Toc high select squared
CREATE_PUBVAR(L_FuseTocAlmFlg    ,  unsigned);  // Ac line side fuse TOC alarm flg
CREATE_PUBVAR(L_FuseIaSqrT3      ,  float);     // Square of the AC line fuse estimated Ia current task3 version
CREATE_PUBVAR(L_FuseIbSqrT3      ,  float);     // Square of the AC line fuse estimated Ib current task3 version
CREATE_PUBVAR(L_FuseIcSqrT3      ,  float);     // Square of the AC line fuse estimated Ic current task3 version

// Diagnostics
//------------------
/********************************************************************************/
/*                                      + - Trip                                */
/*                                      |  + - Alarm                            */
/*                                      |  |  + - State                         */
/*                                      |  |  |  + - Lock                       */
/*                                      |  |  |  |  + - Noinit                  */
/*                                      |  |  |  |  |  + - Bridge1              */
/*                                      |  |  |  |  |  |  + - Bridge2           */
/*                                      |  |  |  |  |  |  |  + - Current Flt    */
/*                                      |  |  |  |  |  |  |  |  + - Voltage Flt */
/*                                      |  |  |  |  |  |  |  |  |  + - Temp Flt */
/*                                      |  |  |  |  |  |  |  |  |  |  + - Other */
/*                                      |  |  |  |  |  |  |  |  |  |  |         */
/*                                      v  v  v  v  v  v  v  v  v  v  v         */
CREATE_DIAG(L_TocAlm);            //    0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0)      //   Timed over-current alarm
CREATE_DIAG(L_TocFlt);            //    1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0)      //   Timed over-current Fault
CREATE_DIAG(L_FuseTocAlm);        //    0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0)      //   AC line fuse Timed over-current alarm
CREATE_DIAG(L_FuseTocFlt);        //    1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0)      //   AC line fuse Timed over-current Fault


// Unpublished Variables
float L_TocAlmOnDl;
float L_TocAlmOffDl;
float L_FuseTocAlmOnDl;
float L_FuseTocAlmOffDl;


// Data Pass
//------------------
DATA_PASS(L_IaSqr,          L_IaSqrAvg      , T1_T2,   FILTERED);              // L_Ia square
DATA_PASS(L_IbSqr,          L_IbSqrAvg      , T1_T2,   FILTERED);              // L_Ib square
DATA_PASS(L_IcSqr,          L_IcSqrAvg      , T1_T2,   FILTERED);              // L_Ic square
DATA_PASS(L_IaSqrAvg,       L_IaSqrT3       , T2_T3,    FILTERED);             // L_Ia square
DATA_PASS(L_IbSqrAvg,       L_IbSqrT3       , T2_T3,    FILTERED);             // L_Ib square
DATA_PASS(L_IcSqrAvg,       L_IcSqrT3       , T2_T3,    FILTERED);             // L_Ic square

DATA_PASS(L_FuseIaSqr,      L_FuseIaSqrT3   , T2_T3,    FILTERED);             // Ac line fuse Ia square
DATA_PASS(L_FuseIbSqr,      L_FuseIbSqrT3   , T2_T3,    FILTERED);             // Ac line fuse Ib square
DATA_PASS(L_FuseIcSqr,      L_FuseIcSqrT3   , T2_T3,    FILTERED);             // Ac line fuse Ic square

/*=============================================================================
 * Function: TocDetection
 * Execution Level: Task 3
 * Execution Time: Untimed
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  L_DelTm1
 *                 DelTm3
 *                 L_IaSqrT3
 *                 L_IbSqrT3
 *                 L_IcSqrT3
 *                 L_ChgLinkIp
 *
 *                 EE.L_TocTau
 *                 EE.L_TocThrs
 *
 * Global Outputs: L_IaRms
 *                 L_IbRms
 *                 L_IcRms
 *
 * Locals:         L_IaRmsMag
 *                 L_IbRmsMag
 *                 L_IcRmsMag
 *                 L_WTocT3
 *                 L_TocSqrThrs
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function performs a timed over-current check on each of the phase
 * currents.  The inputs are squared, then lightly filtered phase currents.
 * This function filters these signals much more by an adjustable amount.  The
 * outputs of these filters is then compared to an adjustable threshold
 *
 *===========================================================================*/


void TocDetection(void)
{

    L_IaRmsMag = L_IaRmsMag + L_WTocT3Dyn*(L_IaSqrT3 - L_IaRmsMag);
    L_IbRmsMag = L_IbRmsMag + L_WTocT3Dyn*(L_IbSqrT3 - L_IbRmsMag);
    L_IcRmsMag = L_IcRmsMag + L_WTocT3Dyn*(L_IcSqrT3 - L_IcRmsMag);

    L_IaRms = sqrtf(L_IaRmsMag);
    L_IbRms = sqrtf(L_IbRmsMag);
    L_IcRms = sqrtf(L_IcRmsMag);

    // if or not to use the dynamic TOC configuration
    if ( PARM(TocDyThrshEnb) )
    {
        if ( PARM(DelSpdTrqAct) )
        {
            L_TocThrDy    = L_MaxCurOut * PARM(L_TocMargin);
        }
        else
        {
            L_TocThrDy    = L_MaxCurOut;
        }
        L_TocAlmOnDy  = L_TocThrDy - L_TocAlmOnDl;
        L_TocAlmOffDy = L_TocAlmOnDy - L_TocAlmOffDl;
    }
    else
    {
        L_TocThrDy    = PARM(L_TocThrs);
        L_TocAlmOnDy  = PARM(L_TocAlmOn);
        L_TocAlmOffDy = PARM(L_TocAlmOff);
    }

    if ( L_IaRmsMag > L_IbRmsMag)
    {
         L_TocHiSelSq = L_IaRmsMag;
         L_TocHiSel   = L_IaRms;
    }
    else
    {
         L_TocHiSelSq = L_IbRmsMag;
         L_TocHiSel   = L_IbRms;
    }

    if ( L_IcRmsMag > L_TocHiSelSq)
    {
        L_TocHiSelSq = L_IcRmsMag;
        L_TocHiSel   = L_IcRms;
    }


    if ( L_TocHiSel >= L_TocThrDy )
    {
        PUSH_DIAG(L_TocFlt);
    }

    if ( L_TocHiSel >= L_TocAlmOnDy )
    {
        L_TocAlmFlg = true;
    }
    else if ( L_TocHiSel <= L_TocAlmOffDy )
    {
        L_TocAlmFlg = false;
    }

    if ( L_TocAlmFlg )
    {
        PUSH_DIAG(L_TocAlm);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Function: AclfuTocDetection
// Execution Level: Task 3
// Execution Time: Untimed
// Formal Inputs: None
// Formal Outputs: None
//
// Abstract:
//    The purpose of this function is to protect the AC line fuse.
// This function performs a timed over-current check on each of the phase
// currents.  The inputs are squared, then lightly filtered phase currents.
// This function filters these signals much more by an adjustable amount.  The
// outputs of these filters is then compared to an adjustable threshold
////////////////////////////////////////////////////////////////////////////////
void AclfuTocDetection(void)
{

    L_FuseIaRmsMag = L_FuseIaRmsMag + L_FuseWTocT3Dyn*(L_FuseIaSqrT3 - L_FuseIaRmsMag);
    L_FuseIbRmsMag = L_FuseIbRmsMag + L_FuseWTocT3Dyn*(L_FuseIbSqrT3 - L_FuseIbRmsMag);
    L_FuseIcRmsMag = L_FuseIcRmsMag + L_FuseWTocT3Dyn*(L_FuseIcSqrT3 - L_FuseIcRmsMag);

    L_FuseIaRms = sqrtf(L_FuseIaRmsMag);
    L_FuseIbRms = sqrtf(L_FuseIbRmsMag);
    L_FuseIcRms = sqrtf(L_FuseIcRmsMag);

    // if or not to use the dynamic TOC configuration
    if ( PARM(TocDyThrshEnb) )
    {
        if ( PARM(DelSpdTrqAct) )
        {
            L_FuseTocThrDy    = L_FuseMaxCurOut * PARM(L_FuseTocMargin);
        }
        else
        {
            L_FuseTocThrDy    = L_FuseMaxCurOut;
        }
        L_FuseTocAlmOnDy  = L_FuseTocThrDy - L_FuseTocAlmOnDl;
        L_FuseTocAlmOffDy = L_FuseTocAlmOnDy - L_FuseTocAlmOffDl;
    }
    else
    {
        L_FuseTocThrDy    = PARM(L_FuseTocThrs);
        L_FuseTocAlmOnDy  = PARM(L_FuseTocAlmOn);
        L_FuseTocAlmOffDy = PARM(L_FuseTocAlmOff);
    }

    if ( L_FuseIaRmsMag > L_FuseIbRmsMag)
    {
         L_FuseTocHiSelSq = L_FuseIaRmsMag;
         L_FuseTocHiSel   = L_FuseIaRms;
    }
    else
    {
         L_FuseTocHiSelSq = L_FuseIbRmsMag;
         L_FuseTocHiSel   = L_FuseIbRms;
    }

    if ( L_FuseIcRmsMag > L_FuseTocHiSelSq)
    {
        L_FuseTocHiSelSq = L_FuseIcRmsMag;
        L_FuseTocHiSel   = L_FuseIcRms;
    }


    if ( L_FuseTocHiSel >= L_FuseTocThrDy )
    {
        PUSH_DIAG(L_FuseTocFlt);
    }

    if ( L_FuseTocHiSel >= L_FuseTocAlmOnDy )
    {
        L_FuseTocAlmFlg = true;
    }
    else if ( L_FuseTocHiSel <= L_FuseTocAlmOffDy )
    {
        L_FuseTocAlmFlg = false;
    }

    if ( L_FuseTocAlmFlg )
    {
        PUSH_DIAG(L_FuseTocAlm);
    }
}
