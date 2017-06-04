/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        A0Stator.C30
* Language:    TI 'C'
* Copyright:   2003.... GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file contains those functions which are associated with
*           angle calculations.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* PhaseLockLoopReg           - phase lock loop regulator
* LineMonitorT1              - Task 1 line monitor
* LineMonitorT2              - Task 2 line monitor
*
*==============================================================================
* yymmdd
* 030108 DS   create from line\s0line
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 030528 SAB  Use on 0.0 - S_VyLine for pll error
* 031203 SAB  Uncomment sator over-voltage fault
* 031205 SAB  Add detection of fast phase lock loop mis-lock
* 031219 SAB  Move slow functions to A0Statrs.c30
* 040129 SAB  Check abs value of pll error, not just positive side.
* 040303 SAB  Remove stator phase imbalance/phase loss detection
* 040712 IB   Modified for new 60hz LVMD,using S_SynSwClsd instead of S_SynBrkClsd.
* 051007 SAB  Add Dscm vars for pos/neg sequence discriminator
* 051007 SAB  Add functions StatorPosNegDiscriminatorT1, StatorPosNegDiscriminatorT2
* 051213 SAB  Fix divide by zero opportunity for S_DscmVyPu and S_DscmVxPu
* 051213 SAB  Add separate clamps for pll integrator
* 060106 SAB  Populate S_NSeqHi
* 060113 SAB  Add LineShortIp
* 060119 SAB  Remove S_PhImbInt
* 060125 SAB  Allow Pll to prove on ZVRT recovery as long as no line short
* 060210 SAB  Use L_VyLineT2 to form PllErrFil
* 060313 SAB  Add new logic for turn-off of LineVShort and associated vars
* 060313 SAB  Add new logic for line short turn-off logic
* 070109 IB   Rename S_SynSwClsd as K1Status and remove LVDP dependency.
* 070127 SAB  Remove in-lining from PhaseLockLoopReg, RotorS_LineMonitorT1,
* 070612 SAB  Remove stator frequency var
* 070613 SAB  Move grid open protection to P0FltChk with other of the same.
* 070618 SAB  Change pll logic to closely match Wind-Sync's clamp change.
* 070925 SAB  Add step capbility to pll regulator
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 080115 SAB  Allow S_PllProven to follow GmPllProven when !S_PllEnb
 * 080211 SAB  Remove S_VMagMon
*______________________________________________________________________________
* Last Mod: March 11, 2008 6:02:16AM  */


// Includes Files
//---------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
// product
#include "Product.h"
#include "a0Stator.h"
#include "p0Line.h"

// Parameters
//-----------
// stator PLL CI's
CREATE_PARM(S_PllBwBase,        float);     // Phase lock loop bandwidth base
CREATE_PARM(S_PllBaseHot,       float);     // Phase lock loop bandwidth hot
CREATE_PARM(S_VxVyErrW,         float);   
CREATE_PARM(S_VMagWUp,          float);   
CREATE_PARM(S_VMagWDn,          float);   
CREATE_PARM(S_PllIntGnI,        float);   
CREATE_PARM(S_PllPrpGnI,        float);   
CREATE_PARM(S_PllIntGnR,        float);   
CREATE_PARM(S_PllPrpGnR,        float);   
CREATE_PARM(S_PllPrvTm,         float);   
CREATE_PARM(S_PllIntFilW,       float);   
CREATE_PARM(S_PllIniThrs,       float);     // Phase lock loop init threshold
CREATE_PARM(S_PllRunThrs,       float);     // Phase imbalance trip threshold
CREATE_PARM(S_PllErSpill,       float);     // Phase lock loop spill-over limit
CREATE_PARM(S_LineVLowOn,       float);     // Line voltage low on threshold
CREATE_PARM(S_LineVLowOf,       float);     // Line voltage low off threshold
CREATE_PARM(S_LineVLowTm,       float);     //  voltage low time
CREATE_PARM(S_AcWdogTm,         float);     // Time a phase lockl loop error in limit allowed to exist
CREATE_PARM(S_PllErrW,          float);     // Phase lock loop error omega
CREATE_PARM(S_PllWPrec,         float);     // Pll initial state pre-condition
CREATE_PARM(S_PllWMinI,         float);     // Pll minimum initialization frequency
CREATE_PARM(S_PllWMaxI,         float);     // Pll maximum initialization frequency
CREATE_PARM(S_PllWMaxOfL,       float);     // Pll max freq off line
CREATE_PARM(S_PllWMinOfL,       float);     // Pll min freq off line

// Diagnostics
//------------
CREATE_DIAG(S_AcLineTran);          // Stator volts transient
CREATE_DIAG(S_AcLineWdog);          // Stator volts  watch dog
    
    
   /*Phase lock loop regulator */
    CREATE_PUBVAR(S_AngElecFbk ,          float);     // electrical feedback angle
    CREATE_PUBVAR(S_PllDelAng  ,          float);     // phase lock loop delta angle
    CREATE_PUBVAR(S_PllW       ,          float);     // phase lock loop frequency
    CREATE_PUBVAR(S_PllWInt    ,          float);     // phase lock loop frequency intergrator
    CREATE_PUBVAR(S_PllVyErr   ,          float);     // phase lock loop S_Vy error
    CREATE_PUBVAR(S_PllWIntFil ,          float);     // phase lock loop frequency intergrator (filtered)
    CREATE_PUBVAR(S_CosAngFbk  ,          float);     // Cosine of angle command
    CREATE_PUBVAR(S_SinAngFbk  ,          float);     // Sin of angle command
    CREATE_PUBVAR(S_Cos2Ang    ,          float);     // Cosine (2*Angle)
    CREATE_PUBVAR(S_Sin2Ang    ,          float);     // Sin (2*Angle)

                                                      /* Task 1 Line monitor vars */
    CREATE_PUBVAR(S_VxErr      ,          float);     // error for S_VxLine
    CREATE_PUBVAR(S_VxVyImb    ,          float);     // S_Vx,S_Vy phase imbalance
    CREATE_PUBVAR(S_PllErrRef  ,          float);     // pll error reference
    CREATE_PUBVAR(S_VxyImbFil  ,          float);     // phase imbalance volts

                                          // DO NOT ZERO next 4 pll gains
    CREATE_PUBVAR(S_PllIntGn   ,          float);     // phase lock loop dynamic integral gain
    CREATE_PUBVAR(S_PllPrpGn   ,          float);     // phase lock loop dynamic proportional gn
    CREATE_PUBVAR(S_PllWMax    ,          float);     // maximum allowed pll freqency
    CREATE_PUBVAR(S_PllWMin    ,          float);     // minimum allowed pll freqency
    CREATE_PUBVAR(S_PllWMaxInt ,          float);     // maximum allowed pll freqency
    CREATE_PUBVAR(S_PllWMinInt ,          float);     // minimum allowed pll freqency
    CREATE_PUBVAR(S_NSeqHi     ,          unsigned);  // Negative sequence is high
    CREATE_PUBVAR(S_PllAllowCl ,          unsigned) = false;   // Allow stator pll cool gains
                                          // DO NOT ZERO previous 4 pll gains

    CREATE_PUBVAR(S_VMagPeak   ,          float);     // stator peak magnitude
    CREATE_PUBVAR(S_VMagUp     ,          unsigned);  // stator peak detection error positive
    CREATE_PUBVAR(S_RevSeq     ,          unsigned);  // stator reverse phase sequence boolean
    CREATE_PUBVAR(S_PllErSetpt ,          float);     // stator pll error setpoint
    CREATE_PUBVAR(S_VMag       ,          float);

                                                      // Task 2 Line monitor vars
    CREATE_PUBVAR(S_PllProven  ,          unsigned);  // Pll proven
    CREATE_PUBVAR(S_PllPrvFstp ,          unsigned);  // Pll prove first pass flag
    CREATE_PUBVAR(S_PllPrvTmr  ,          float);     // Pll prove timer
    CREATE_PUBVAR(S_LineFltEnb ,          unsigned);  // Ac line transient fault enable flag
    CREATE_PUBVAR(S_VMagPkT3   ,          float);     // Ac line peak T3
    CREATE_PUBVAR(S_VxErrFil   ,          float);     // X-component filtered error
    CREATE_PUBVAR(S_VyErrFil   ,          float);     // Y-component filtered error
    CREATE_PUBVAR(S_PllPrvPrm0 ,          unsigned) = false;   // Pll prove permissive (initial)
    CREATE_PUBVAR(S_PllPrvPrm  ,          unsigned) = false;   // Pll prove permissive (final)
    CREATE_PUBVAR(S_PllErrHi   ,          unsigned) = false;   // Pll error is high (above zero)
    CREATE_PUBVAR(S_PllState   ,          int) =  HOT_DYN;     // Pll gain/clamp state variable
    CREATE_PUBVAR(S_PllStateZ1 ,          unsigned);  // Transition to hot fix freqency state one old
    CREATE_PUBVAR(S_PllStateZ2 ,          unsigned);  // Transition to hot fix freqency state one old
    CREATE_PUBVAR(S_AcWdogTmr  ,          float);     // pll error timer
    CREATE_PUBVAR(S_PllErrInt  ,          float);     // pll error integrator
    CREATE_PUBVAR(S_PllErInLim ,          unsigned);  // pll error in limit
    CREATE_PUBVAR(S_PllErr     ,          float);     // phase imbalance
    CREATE_PUBVAR(S_PllEnb     ,          unsigned) = true;    // Stator pll enable
    CREATE_PUBVAR(S_LineVLo    ,          unsigned) = false;   //  Line voltage low
    CREATE_PUBVAR(S_LineVLoTmr ,          float);     // Line voltage low timer

                                                      //Current regulator Feedback Compensation
    CREATE_PUBVAR(S_ICompVSpil,           float);     // Current feedback compensation voltage spill-over
    CREATE_PUBVAR(S_ICompV,               float);     // Current feedback compensation voltage spill-over clamped
    CREATE_PUBVAR(R_IFbkCompN,            float);     // Current feedback compensation negative sequence authority
    CREATE_PUBVAR(R_IxFbkCompN,           float);     // Current feedback compensation negative sequence x authority
    CREATE_PUBVAR(R_IxFbkCompNT1,         float);     // Current feedback comp neg-seq x authority in task1
    CREATE_PUBVAR(R_IyFbkCompN,           float);     // Current feedback compensation negative sequence y authority
    CREATE_PUBVAR(R_IyFbkCompNT1,         float);     // Current feedback comp neg-seq y authority in task1
    CREATE_PUBVAR(R_IxFbkComp,            float);     // Current regulator feedback x channel compensation
    CREATE_PUBVAR(R_IyFbkComp,            float);     // Current regulator feedback y channel compensation
    CREATE_PUBVAR(S_DscmVxPu,             float);     // Negative Sequence Discrimated Vx per unit
    CREATE_PUBVAR(S_DscmVyPu,             float);     // Negative Sequence Discrimated Vy per unit

    CREATE_PUBVAR(S_IMagK1Stck,           float);
	CREATE_PUBVAR(S_VMagPU,               float);



/*****************************************************************************
*
*       external references
*
*****************************************************************************/

extern float    S_Vy;
extern float    S_Vx;

extern int      S_PllStateZ1T1;
extern float    S_PllIntGnT1;
extern float    S_PllWMaxIntT1;
extern float    S_PllWMinIntT1;
extern float    S_PllPrpGnT1;
extern float    S_PllWMaxT1;
extern float    S_PllWMinT1;
extern unsigned R_InnrRegEnbT1;
extern float    S_VMagPeakT3;

extern volatile int      T1ASt;
extern float    S_VyFbk;
extern float    S_VxFbk;
extern float    S_VlnInRcp;
extern float    S_VMagPU;
extern float    S_PllregStep;
extern float    S_PllWIntT2;
extern float    S_PllErrT2;
extern float    S_IMgK1StkGn;
extern float    S_IMag;
extern float    R_LT1TmDelta;
extern float    R_DelTm1;
extern float    GmPllWAvg;
extern float    GmAngElecCmd;
extern float    DelTm2;
extern float    CI_S_PllPrpGnI;
extern float    CI_S_PllIntGnI;
extern float    GmAngElecFbk;
extern float    S_VoltsRcp;
extern float    S_VMagT2;
extern unsigned R_InnrRegEnb;
extern unsigned S_K1Status;
extern unsigned GmPllProven;

extern RRegLimFlgs R_InnrRegLim;



/*=============================================================================
 * Function: PhaseLockLoopReg
 * Execution Level: Task 1
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  R_DelTm1
 *                 S_PllProven
 *                 S_Vy
 *
 *                 EE.S_LineFrq
 *                 EE.S_PllPrpGn
 *                 EE.S_PllPrpGnI
 *                 EE.S_PllIntGn
 *                 EE.S_PllIntGnI
 *
 *
 *
 *
 * Global Outputs: S_AngElecCmd
 *                 S_AngElecFbk
 *                 S_PllW
 *                 S_PllLimMax
 *                 S_PllLimMin
 *                 S_PllVyErr
 *                 S_PllRef
 *
 * Locals:         S_PllWInt
 *                 S_PllIntGn
 *                 S_PllPrpGn
 *
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function tracks the line ac voltage angle, both in real and
 * simulator mode.  The most significant outputs are electrical command and
 * feedback angles, as well and line frequency.
 *
 *===========================================================================*/

void RotorS_PhaseLockLoopReg()
{


     /* Code copied from S0LINCAL, modified for stator */
     S_VMag = sqrtf(S_VxFbk*S_VxFbk + S_VyFbk*S_VyFbk);


     /* Clear limit flags */
     R_InnrRegLim.S_PllLimMax = false;
     R_InnrRegLim.S_PllLimMin = false;

     if ( S_PllEnb)
     {


        /* Form error signal */
        S_PllVyErr  = (0.0 + S_PllregStep)- S_VyFbk;


        /* Calculate and clamp integral path */
        if ((S_PllStateZ1T1 == COOL_DYN) && (S_PllStateZ2 == HOT_DYN))
        {
             S_PllWInt = S_PllWIntFil;
        }
        else
        {
            S_PllWInt = S_PllWInt + S_PllIntGnT1*R_DelTm1*S_PllVyErr;
        }

        if ( S_PllWInt >= S_PllWMaxIntT1)
        {
             S_PllWInt = S_PllWMaxIntT1;
        }
        if (S_PllWInt <= S_PllWMinIntT1)
        {
             S_PllWInt = S_PllWMinIntT1;
        }


        /* Calculate proportional path, sum with int. path, clamp */
        S_PllW = S_PllWInt + S_PllVyErr * S_PllPrpGnT1;
        if ( S_PllW >= S_PllWMaxT1)
        {
             S_PllW = S_PllWMaxT1;
             if ( R_InnrRegEnbT1 )
             {
                 R_InnrRegLim.S_PllLimMax = true;
             }
        }
        if ( S_PllW <= S_PllWMinT1)
        {
             S_PllW = S_PllWMinT1;
             if ( R_InnrRegEnbT1 )
             {
                 R_InnrRegLim.S_PllLimMin = true;
             }
        }



        // Calculate electrical angles
        S_PllDelAng  = R_DelTm1*S_PllW;
        S_AngElecFbk = S_AngElecFbk + S_PllDelAng;

     }
     else
     {
        S_PllWInt = PARM(S_PllWPrec);
        S_PllW  = GmPllWAvg;
        //HL: ESS code use GmAngElecCmd
        S_AngElecFbk = GmAngElecFbk + R_LT1TmDelta*GmPllWAvg - PARM(GmSVllFbkLag);
     }


         if (T1ASt == 1)
         {
             S_AngElecFbk = cVectorf::Modulo2Pi(S_AngElecFbk);
         }

     S_PllStateZ2 = S_PllStateZ1T1;


}


/*=============================================================================
 * Function: LineMonitorT1
 * Execution Level: Task 1
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  S_DelTm1
 *                 S_PllVyErr
 *                 S_VxFbk
 *
 *                 EE.S_AcLineWDn
 *                 EE.S_AcLineWUp
 *                 EE.S_PllPrpGn
 *
 *
 *
 *
 *
 * Global Outputs: S_AcLinePeak
 *                 S_PhImb
 *
 *
 *
 *
 *
 * Locals:         S_PhImbRef
 *                 S_AcLineUp
 *                 S_VxErr
 *                 S_VxVyImb
 *                 S_VxyImbFil
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function tracks monitors the ac line by observing the phase lock
 * loop error.
 *
 *===========================================================================*/

void RotorS_LineMonitorT1()
{
     float   S_Vy_Err_Gn,Pre_Ph_Imb_Ref;

     if ( S_PllEnb)
     {

        /*Calculate and sum S_Vx and S_Vy errors */

        S_VxErr  = S_VMag + S_VxFbk;

        //Add low pass filters to each component
        S_VxErrFil  = S_VxErrFil + PARM(S_VxVyErrW)*R_DelTm1 * (S_VxErr - S_VxErrFil);
        S_VyErrFil  = S_VyErrFil + PARM(S_VxVyErrW)*R_DelTm1 * (S_PllVyErr - S_VyErrFil);
        //

        S_VxVyImb    = S_VxErrFil * S_VxErrFil + S_VyErrFil * S_VyErrFil;
        S_VxyImbFil  = S_VxyImbFil + PARM(S_PllErrW)*R_DelTm1 * (S_VxVyImb - S_VxyImbFil);


        /*Calculate phase imbalance reference */
        if ( (S_VMag - S_VMagPeak) >= 0.0)
        {
           S_VMagUp = true;
           S_Vy_Err_Gn = PARM(S_VMagWUp);
        }
        else
        {
           S_VMagUp = false;
           S_Vy_Err_Gn = PARM(S_VMagWDn);
        }

        S_VMagPeak   = S_VMagPeak + R_DelTm1*S_Vy_Err_Gn*(S_VMag - S_VMagPeak);



        if ( S_PllProven)
        {
           S_PllErSetpt = PARM(S_PllRunThrs);
        }
        else
        {
            S_PllErSetpt = PARM(S_PllIniThrs);
        }


        Pre_Ph_Imb_Ref = S_VMagPeak * S_VlnInRcp * S_PllErSetpt + PARM(S_PllErNoise);

        S_PllErrRef     = Pre_Ph_Imb_Ref*Pre_Ph_Imb_Ref;


        /* Calculate phase lock loop error */
        S_PllErr        = S_VxyImbFil - S_PllErrRef;

     }
     else
     {
        S_PllErr    = 0.0;
        S_VxErrFil  = 0.0;
        S_VyErrFil  = 0.0;
        S_VMagPeak  = 0.0;

     }




}


/*=============================================================================
 * Function: LineMonitorT2
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  DelTm2
 *                 S_VMag
 *                 AcLossOn
 *                 S_AcLossOff
 *
 *                 EE.S_PllPrpGn
 *                 EE.S_PllPrpGnI
 *                 EE.S_PllIntGn
 *                 EE.S_PllIntGnI
 *                 EE.S_PllWMax
 *                 EE.S_PllWMin
 *
 *
 *
 *
 * Global Outputs: S_PllW
 *                 S_PllWMax
 *                 S_PllWMin
 *                 S_PllProven
 *                 S_PllPrpGn
 *                 S_PllIntGn
 *                 S_VLoss
 *
 * Locals:         S_PhImbInlim
 *                 S_PllPrvTmr
 *                 S_PhImb_Trm
 *
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function tracks monitors the ac line by observing the phase lock
 * loop error.
 *
 *===========================================================================*/

void RotorS_LineMonitorT2()
{

     S_VMagPU  = S_VMagT2 * S_VoltsRcp;

     if (S_PllEnb)
     {



        //////////////////////////////////////////////////////////////////////
        //Pll error limit check
        //////////////////////////////////////////////////////////////////////
        S_PllErrInt = S_PllErrInt + DelTm2*S_PllErrT2;
        if ( S_PllErrInt < 0.0)
        {
            S_PllErrHi   = false;
            S_PllErrInt  = 0;
            S_PllErInLim = false;
        }
        else
        {
            S_PllErrHi   = true;
            if (S_PllErrInt >= PARM(S_PllErSpill))
            {
               S_PllErrInt  = PARM(S_PllErSpill);
               S_PllErInLim = true;
            }
        }


        //////////////////////////////////////////////////////////////////////
        //Symetric Low Voltage Detection -- PROT_308
        //////////////////////////////////////////////////////////////////////
        if ( S_VMagPU <= PARM(S_LineVLowOn))
        {
             S_LineVLoTmr = S_LineVLoTmr + DelTm2;
             if ( S_LineVLoTmr >= PARM(S_LineVLowTm))
             {
                  S_LineVLoTmr  = PARM(S_LineVLowTm);
                  S_LineVLo     = true;
             }
        }
        else if ( S_VMagPU >= PARM(S_LineVLowOf))
        {
             S_LineVLo     = false;
             S_LineVLoTmr  = 0.0;
        }

        //////////////////////////////////////////////////////////////////////
        // S_PllProven logic -- PROT_308
        //////////////////////////////////////////////////////////////////////
        if (  ( S_K1Status && !S_LineVLo) || (!S_K1Status && R_InnrRegEnb)    )
        {
            S_PllPrvPrm0 = true;
        }
        else
        {
            S_PllPrvPrm0 = false;
        }

        if ( !S_PllPrvPrm0 || S_PllErrHi)
        {
            S_PllPrvPrm = false;
        }
        else if ( S_PllPrvPrm0 && !S_PllErInLim)
        {
            S_PllPrvPrm = true;
        }

        if ( S_PllPrvPrm && !S_PllProven)
        {
            S_PllPrvTmr = S_PllPrvTmr + DelTm2;
            if ( S_PllPrvTmr >= PARM(S_PllPrvTm))
            {


               S_PllPrvTmr = PARM(S_PllPrvTm);
               if ( !S_PllPrvFstp)
               {
                   S_PllPrvFstp = true;



                   if (S_PllW < 0.0)
                   {
                      S_RevSeq    = true;
                      S_PllProven = false;
                      S_PllPrvTmr = 0.0;


                      //Push2Flt(S_RevPhSeq);

                   }
                   else
                   {
                      S_RevSeq    = false;
                      S_PllProven = true;
                   }
                }
                else
                {
                   S_PllProven  = true;
                }
            }
        }
        else if (!S_PllPrvPrm)
        {
            S_PllPrvTmr = 0.0;
            S_PllProven = false;
            S_LineVLo   = false;
            S_LineVLoTmr= 0.0;

            //S_PllIntGn   =  CI_S_PllIntGnI;
            //S_PllPrpGn   =  CI_S_PllPrpGnI;
            //S_PllWMax    =  CI_S_PllWPrec;
            //S_PllWMin    =  CI_S_PllWPrec;
            //S_PllWMaxInt =  CI_S_PllWPrec;
            //S_PllWMinInt =  CI_S_PllWPrec;

        }



        //////////////////////////////////////////////////////////////////////
        // Ac line transient / watchdog checks -- PROT_308
        //////////////////////////////////////////////////////////////////////
        if (R_InnrRegEnb && S_PllErInLim && S_LineFltEnb)
        {
           PUSH_DIAG(S_AcLineTran);
           S_AcWdogTmr = S_AcWdogTmr + DelTm2;
           if ( S_AcWdogTmr >= PARM(S_AcWdogTm))
           {
                S_AcWdogTmr = PARM(S_AcWdogTm);
                PUSH_DIAG(S_AcLineWdog);
           }
        }
        else
        {
//           Pop2Flt(S_AcLineTran);
           S_AcWdogTmr = 0.0;
        }

        //////////////////////////////////////////////////////////////////////
        // Pll Gain/Limit State Machine
        //////////////////////////////////////////////////////////////////////
        /* get average value of pll integrator for preset  */

        S_PllWIntFil += PARM(S_PllIntFilW) * DelTm2   * ( S_PllWIntT2 - S_PllWIntFil );
        S_PllStateZ1 = S_PllState;
         switch (S_PllState)
         {

             case HOT_DYN:    // S_PllState = 1
                 S_PllIntGn  =  PARM(S_PllIntGnI);
                 S_PllPrpGn  =  PARM(S_PllPrpGnI);
                 S_PllWMax    =  PARM(S_PllWMaxOfL);
                 S_PllWMaxInt =  PARM(S_PllWMaxOfL);
                 if ( S_PllPrvFstp && !S_RevSeq)
                 {
                     S_PllWMin    =  PARM(S_PllWMinOfL); //allow plenty room while off line
                     S_PllWMinInt =  PARM(S_PllWMinOfL);
                 }
                 else
                 {
                     S_PllWMin    = -PARM(S_PllWMaxOfL);
                     S_PllWMinInt = -PARM(S_PllWMaxOfL);
                 }

                 if ( S_PllProven && S_PllAllowCl)
                 {
                    S_PllIntGn  = PARM(S_PllIntGnR);
                    S_PllPrpGn  = PARM(S_PllPrpGnR);
                    S_PllState = COOL_DYN;
                 }

                 break;



             case COOL_DYN:    // S_PllState = 2
                 S_PllIntGn   =  PARM(S_PllIntGnR);
                 S_PllPrpGn   =  PARM(S_PllPrpGnR);
                 S_PllWMax    =  PARM(S_PllWMaxOfL);
                 S_PllWMaxInt =  PARM(S_PllWMaxOfL);
                 if ( S_PllProven && !S_RevSeq && S_PllPrvFstp)
                 {
                    S_PllWMin    =  PARM(S_PllWMinOfL);
                    S_PllWMinInt =  PARM(S_PllWMinOfL);
                 }
                 else //Re-proving or first pass
                 {
                    S_PllWMin    = -PARM(S_PllWMaxOfL);
                    S_PllWMinInt = -PARM(S_PllWMaxOfL);
                 }

                 if ( !S_PllPrvPrm || !S_PllAllowCl)
                 {
                     S_PllState = HOT_DYN;
                 }


                 break;
            default:
                break;
         }

     }
     else//Pll is not enabled
     {

        S_AcWdogTmr = 0.0;

        //This takes the place of the old HOT_INIT state
        //Gains and clamps
        S_PllIntGn   =  PARM(S_PllIntGnI);
        S_PllPrpGn   =  PARM(S_PllPrpGnI);
        S_PllWMax    =  PARM(S_PllWPrec);
        S_PllWMin    =  PARM(S_PllWPrec);
        S_PllWMaxInt =  PARM(S_PllWPrec);
        S_PllWMinInt =  PARM(S_PllWPrec);
        S_PllState   =  HOT_DYN;  //Precondition to hot state
        S_LineVLo    =  false;
        S_LineVLoTmr =  0.0;
        S_PllProven  =  GmPllProven;


     }


}


/*=============================================================================
 * Function:            LineMonitorT3
 * Execution Level:     Task 3
 * Execution Time:      Not tested
 * Formal Inputs:       None
 * Formal Outputs:      None
 * Return Value:        Not applicable
 * Abstract:            This function  monitors various ac line conditions, as
 *                      listed below:
 *
 *                      * Stator voltage feedback detection --
 *                        Diagram Rotor/BIL/Protection/Prot_025
 *
 *===========================================================================*/

void RotorLineMonitorT3()
{

    S_VMagPkT3  = S_VMagPeakT3;

    if ( S_K1Status )
    {
        S_IMagK1Stck = 0.0;
    }
    else
    {
        S_IMagK1Stck += S_IMgK1StkGn * ( S_IMag - S_IMagK1Stck );
    }

}

