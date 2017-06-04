///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Speed feedback related functions
//
// DESCRIPTION:
//      Angle feedback processing in application layer.
//
// COPYRIGHT:   Copyright (c) 2014
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     a0Angle.vsd, a0Angle.xls
//
///////////////////////////////////////////////////////////////////////////////


// Includes Files
//---------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
// product
#include "Product.h"
#include "a0Angle.h"
#include "a0Rule.h"
#include "s0SimPlant1x.h"


// External variables

extern float DelTm2;
extern float GmAngEFbkT2;
extern float GmAngElecFbk;
extern float GmPllWAvg;
extern float GmPllWAvg;
extern float R_DelTm1;
extern float R_PosScl;
extern float R_Spd;
extern float S_AngEFbkT2;
extern float S_AngElecFbk;

extern unsigned S_K1StatusT1;
extern unsigned SimMode;
extern unsigned DsimMode;
extern int   R_TachIntgrl;

extern unsigned    R_MctlCmd_PackedBits;
extern cSimPlant1x  SimPlant1x;
extern float R_LT1TmDelta;
extern  float R_WRtrFil;
// RotorAnglesT3()
extern unsigned     S_SyncCmdT3;

// Constants
//----------

// Types and Classes
//------------------

// Parameters
//-----------
CREATE_PARM(R_AngCorRate,       float);     //  Rotor Angle correction rate 	
// RotorAnglesT3()
CREATE_PARM(R_TachZeroDelThrs,  float);     // threshold for detecting excessive tach offset delta (Rad)

CREATE_PARM(R_SyncRegGn,        float);     // sync regulator gain (sync stator to line);
CREATE_PARM(R_WeFfW1,           float);     // Omega feedforward tach case


// Diagnostics
//------------
// RotorAnglesT3()
CREATE_DIAG(R_TachLooseTrp);            // excessive change in tachometer zero value

// Variables
//----------

// CmdFbkAngl procedure
CREATE_PUBVAR(R_AngEstDtT1,   float);        // Angle estimate dt for task1
CREATE_PUBVAR(R_AngEstDtT2,   float);        // Angle estimate dt for task2
CREATE_PUBVAR(R_AngSlwCor  ,  float  );    /* angle corrector for transition from dc to ac */
CREATE_PUBVAR(R_AngElecLin ,  float  );    /* line angle, adjusted for rotor algorithm timing */
CREATE_PUBVAR(R_AngElecSta ,  float  );    /* stator angle, for rotor algorithm  */
CREATE_PUBVAR(R_AngElecFbk ,  float  );    /* electrical feedback angle */
CREATE_PUBVAR(R_AngElecCmd ,  float  );    /* electrical command angle */
CREATE_PUBVAR(R_WElecFil   ,  float  );    /* Filtered electrical freq. */
CREATE_PUBVAR(R_WElec      ,  float  );    /* omega electrical */
CREATE_PUBVAR(R_WElecFf    ,  float  );    /* electrical omega feed-forward*/
CREATE_PUBVAR(R_SpdCalInt  ,  float  );    /* speed feedback calculated integrator*/
CREATE_PUBVAR(R_WRtrFil    ,  float  );   /* omega rotor filter */
CREATE_PUBVAR(R_WRtr       ,  float  );    /* omega rotor */
CREATE_PUBVAR(R_IxFbkZ1    ,  float  );   /* ix feedback old */
CREATE_PUBVAR(R_VxFbkZ1    ,  float  );   /* (z-1)vx feedback */
CREATE_PUBVAR(R_TachPosEst ,  float  );     /* Tach position estimate */
CREATE_PUBVAR(R_RtrPosn    ,  float  );    /* rotor position(rad) */
CREATE_PUBVAR(R_SyncAngErr ,  float  );    /* sync angle error */
CREATE_PUBVAR(R_AngErrInt  ,  float  );    /* sync reg integral */
CREATE_PUBVAR(R_TachZero   ,  float  );    /* tach marker offset */
CREATE_PUBVAR(R_TachZeroT3, float);    // tach marker offset in task3 (Rad)
CREATE_PUBVAR(R_AngTach    ,  float  );    /* tach angle */
CREATE_PUBVAR(R_AngRotStat ,  unsigned );
CREATE_PUBVAR(R_AngEstErr,      float);     //
CREATE_PUBVAR(R_TachLsDone, unsigned); // rotor tach-loose check is done
CREATE_PUBVAR(R_TachLsDoneT2,unsigned);// rotor tach-loose check is done in task2
// RotorAnglesT3()
CREATE_PUBVAR(S_SyncCntr,   unsigned) = 0; // stator synchronization operation counter
CREATE_PUBVAR(R_TachZeroRef,float);    // tach offset reference for detecting excessive delta (Rad)
CREATE_PUBVAR(R_TachZeroDel,float);    // tach marker offset delta between last two sync events (Rad)
CREATE_PUBVAR(R_TachPosn,     float);        // Rotor position seen by the tach
CREATE_PUBVAR(R_AngEstErT2,   float);

// Unpublished Variables
//----------------------
unsigned   S_SyncCmdT3Z1;              // delayed task3 sequencer sync control bit

// Data Passing
//-------------
DATA_PASS(R_AngEstErr,  R_AngEstErT2,   T1A_T2, FILTERED);
DATA_PASS(R_TachZero,   R_TachZeroT3,   T2_T3,  FILTERED);
DATA_PASS(R_TachLsDone, R_TachLsDoneT2, T3_T2,  UNFILTERED);


/*=============================================================================
 * Function: CmdFbkAngl
 * Execution Level: Task 1
 * Execution Time: 6.60usec   12/19/96--Rev:001.01.01.A
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  R_DelTm1
 *                 Est_R1
 *                 Est_R2
 *                 R_IxCmd0
 *                 R_IxFbk
 *                 R_IxFbkZ1
 *                 R_IxregInt
 *                 R_MctlCmd.Lock_Rtr
 *                 R_PolePrRcp
 *                 Sim_Mode
 *                 Slreg_Out
 *                 Spd
 *                 R_SpdSim
 *                 R_MtrCtl1.Tach_Alg
 *                 R_VxFbk
 *                 R_VxFbkZ1
 *                 R_WElec
 *
 *                 CI_Tach_Fil
 *                 CI_Tach_Est_Fil
 *
 * Global Outputs: Ang_Elec_Cmd
 *                 R_IxFbkZ1
 *                 R_WElec
 *                 R_WElecFil
 *                 R_WRtr
 *                 R_WRtrEst
 *                 R_WRtrFrq
 *
 * Locals:         R_SpdCalInt
 *                 R_WElecFf
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function calculates command and feedback angles.
 *
 *===========================================================================*/


void RotorCmdFbkAngl()
{
    float   Temp1;
    float   We_Ff_Tmp;

    /* ============= Speed =================== */
    //Tach position estimation -- Trq_089
    R_TachPosEst = cVectorf::Modulo2Pi(R_TachPosEst+R_AngEstDtT1);
    R_AngEstErr = cVectorf::Modulo2Pi(R_TachPosn-R_TachPosEst+(5*PI))-PI;
    if (( SimMode == true)&&(!DsimMode))
    {
        R_WRtr       = SimPlant1x.R_WRtrSim;
        R_TachPosn   = SimPlant1x.R_TachPosnSim;
    }
    else
    {
        R_WRtr       = R_Spd*PARM(R_PolePairs);
        R_TachPosn   = cVectorf::Modulo2Pi(R_TachIntgrl * R_PosScl + PARM(R_MarkerOffs));
    }

    if ( PARM(R_RtrPosMeth) == OLD_SPD_ANGL_METH )
    {
        R_RtrPosn    = R_TachPosn;
    }
    else
    {
        /* Use estimated, slower position*/
        R_RtrPosn    = R_TachPosEst;
    }

    /* rotor frequency is stator freq - mech freq */
    if (R_MctlCmd.R_AngElecFrz)                            // need to go via data passer (t2 -> t1)?
    {
        R_WElec  = 0;
    }
    else
    {
        R_WElec  = GmPllWAvg - R_WRtr;
    }

    We_Ff_Tmp = PARM(R_WeFfW1); // filter BW
    if ( We_Ff_Tmp == 0.0)
        R_WElecFf = R_WElec;
    else
        R_WElecFf = R_WElecFf +  R_DelTm1 * We_Ff_Tmp * ( R_WElec - R_WElecFf);

    /*  filter R_WElec */
    Temp1 = R_DelTm1 * 400.0;
    R_WElecFil = (1.0 - Temp1) * R_WElecFil +  Temp1* R_WElec;

    /* Compensate line angle for difference in elapsed time */
    R_AngElecLin = GmAngElecFbk + R_LT1TmDelta * GmPllWAvg;


    // R_AngTach = R_RtrPosn - R_TachZero;                          // ESS
    R_AngTach = R_RtrPosn - R_TachZero + R_DelTm1 * R_WRtr;      // W*dt comping for?

    /* ========== Angles Generation ========== */
    if (!R_MctlCmd.R_AngElecFrz)
    {
        if (!R_AngRotStat)
        {
            if (R_AngSlwCor > 0.0)
                R_AngSlwCor -= PARM(R_AngCorRate);    // ramp correction angle to zero at 1 hz rate
            else                                // (softens transiiton from dc to ac)
                R_AngSlwCor += PARM(R_AngCorRate);

            if (ABS(R_AngSlwCor) < PARM(R_AngCorRate))
            {
                R_AngSlwCor  = 0.0;
                R_AngRotStat = True;
            }
        }

        if ( S_K1StatusT1 )
            R_AngElecSta = R_AngElecLin;
        else
            R_AngElecSta = R_AngElecLin;

        R_AngElecFbk = cVectorf::Modulo2Pi(R_AngElecSta - R_AngTach + (4.0*PI) - R_AngSlwCor);
        R_AngElecCmd = R_AngElecFbk + R_DelTm1 * R_WElec;
    }
    else
    {
        R_AngRotStat = False;
        //     R_AngElecFbk = -90.0*DEG_TO_RAD;  // aligns phase A with Y-axis for R_RtrConFlt checking
        R_AngElecFbk = (PI*1.5);             // aligns phase A with Y-axis for R_RtrConFlt checking
        R_AngElecCmd = R_AngElecFbk ;

        R_AngElecSta = R_AngElecLin;

        R_AngSlwCor  = cVectorf::Modulo2Pi((R_AngElecSta - R_AngTach + (4.0*PI)) - R_AngElecFbk);
        if (R_AngSlwCor > PI) R_AngSlwCor -= PI2;      // correction angle when un-frozen (-PI, +PI)
    }


}



/*******************************************************************************
* Function:         RotorAnglesT2()()
* Execution Level:  Task 2
* Execution Time:   Not measured
*
* Formal Inputs:    None
* Formal Outputs:   None
*
* Global Inputs:
* Global Outputs:
*
* Return Value:     None
* Function Calls:   None
*
* Abstract:
*
*******************************************************************************/
void  RotorAnglesT2( void )
{
    float tmpf;
    //float W_Frq_Tmp;

    // note: GmAngEFbkT2 and S_AngEFbkT2 are not exactly time-aligned.  Should calculate
    // ----  angle error using GmAngEFbkZ1T2 and S_AngEFbkZ1T2, which span to the end of
    //       the most recent Task2 frame.
    tmpf = cVectorf::Modulo2Pi(GmAngEFbkT2 - (S_AngEFbkT2 + PARM(S_VllFbkLag)));
    if      (tmpf >  PI) tmpf -= PI2;
    else if (tmpf < -PI) tmpf += PI2;

    R_SyncAngErr = tmpf;

    /* Adjust R_TachZero to null angle error between line and stator */
    if (R_MctlCmd.S_SyncCmd)
    {
        R_AngErrInt += DelTm2 * PARM(R_SyncRegGn) * R_SyncAngErr;

        if      (R_AngErrInt >  PI) R_AngErrInt -= PI2;
        else if (R_AngErrInt < -PI) R_AngErrInt += PI2;

        R_TachZero = R_AngErrInt;
    }


}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Compare tachometer offset variation between very 1st startup and
//      subsequent synchronization operations. Increment of this delta over
//      time is indicative of a loose tachometer.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void  RotorAnglesT3( void )
{
    // keep track of number of sync operations at rising edge
    if ( (S_SyncCmdT3 == true) && (S_SyncCmdT3Z1 == false) )
    {
        ++S_SyncCntr;
		R_TachLsDone = false;
        if ( S_SyncCntr == 0 )
        {
            S_SyncCntr = S_SyncCntr + 2;    // prevent unlikely wrap around zero
        }
    }

    // obtain tach-zero reference at falling edge, during 1st synchronization
    if ( (S_SyncCmdT3 == false) && (S_SyncCmdT3Z1 == true) && (S_SyncCntr == 1) )
    {
        R_TachLsDone  = true;
		R_TachZeroRef = R_TachZeroT3;  // use very 1st reading as reference to
    }                                  // prevent 'walking' with progressive movement

    // only check for excessive Tach offset delta, after at least one sync operation
    // check after tach regulator is done. ZJ: issue here is how to distinguish between regulator done and sync interrupt
    if ( (S_SyncCmdT3 == false) && (S_SyncCmdT3Z1 == true) && (S_SyncCntr > 1) )
    {
        R_TachZeroDel = R_TachZeroT3 - R_TachZeroRef;
        if ( R_TachZeroDel > PI )
        {
            R_TachZeroDel -= PI2;
        }
        else if ( R_TachZeroDel < -PI )
        {
            R_TachZeroDel += PI2;
        }

        if ( ABS(R_TachZeroDel) > PARM(R_TachZeroDelThrs) )
        {
            PUSH_DIAG(R_TachLooseTrp);  // excessive delta between reference and new
        }                               // zero-adjustment, for Tach marker
		R_TachLsDone = true;
    }

    S_SyncCmdT3Z1 = S_SyncCmdT3;    // retain old value for edge detection

    return;
}

