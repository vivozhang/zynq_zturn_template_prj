/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        TMS320VC33 DSP
* File:        S0LINE.C30
* Language:    TI 'C'
* Copyright:   2003... GE Motors and Industrial Systems, Salem VA
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
* 970120 0001 SAB  Create for angle and speed calculations from A0ANGL.C30
* 970120 0002 SAB Include S0EEEMUL.H, not A0EEEMUL.H
* 970124 0003 SAB Add function AngleSlewSim
* 970130 0004 SAB Remove zero of Frq_Slw_Sign(bug)
* 970130 0005 SAB Add AngleSimulator in place of CmdFbkAngl
* 970205 0006 SAB Fix Omega_Del
* 970210 0007 SAB Remove temp setting of Vx,Vy_Line
* 970214 0008 SAB Pll_Frq--> rad/sec, use abs value of Vx_Line for pll monitor
* 970219 0009 SAB Remove Chg_Opn_Ip
* 970307 0010 SAB Do not set L_PllMin_Lim, L_PllMax_Lim flags unless running.
* 970318 SAB  Header file comments and use products.h
* 970319 SAB  Pop L_Ac_Line_Tran brief fault.
* 970326 SAB  Remove Pll_Vyln_Sim from bit-packed EE, remove extern. ram
* 970415 SAB  Remove Ac_Line_W calculation
* 970416 SAB  Add monitor for line voltage and frequency
* 970422 SAB  Do not check for undervoltage, under-freq. in stopped state
* 970506 SAB  Rename EE.CI.Pll_W_Max to EE.CI.Pll_W_Max_R.
*             Rename EE.CI.Pll_W_Min to EE.CI.Pll_W_Min_R.
* 970508 SAB  Change limits on simultor line angle. Now yields frequency slew
*             to meet spec (.5pu freq to 1.25pu freq).
* 970513 SAB  Rename Ph_Imb_Err --> Vxy_Imb_Fil and re-structure phase imbalance
*             filtering.
* 970527 SAB  Set Line_Mag_Mon = Line_Mag_T2, W_Pll_Mon = Pll_W_T2 when pll first
*             meets requirements to prove pll
* 970624 SAB  Populate Pll_Int_Gn, Pll_Prp_Gn with run-time values when the
*             Pll_Prv_Tmr reaches 60msec (out of 120msec), not when the pll is
*             proven.
* 970812 SAB  Add code to make Rev_Ph_Seq fault effectively a lock fault.
* 970923 SAB  Add conditional compile to allow for optional sync on reverse
*             Make Pll_Del_Ang public
* 970924 SAB  If reverse sequence, subtract W_Slw_Int from W_Sim instead of add
*             Make clamps of slew integrator dynamic and dependent upon seq.
* 971010 SAB  Add logic to drive L_Ac_Line_Loss and use it for L_Ac_Line_Tran fault
* 971020 SAB  Add logic to use Line_Flt_Enb for Ac line transient faults
* 971120 SAB  Zero vars at init
* 980119 SAB  Prove pll faster for S3SOURCE
* 980120 SAB  Use final values Pll_Prv_Tm, Pll_Gn_Sw_Tm
* 980213 SAB  S3SOURCE only -- Use not Stopped for condition to run phase loss
*             detector, not Pll_Proven.  Clear Pll_Proven when Ph_Imb_Avg > 0
*             and do not zero frequency integrator when pll is disproven.
* 980220 SAB  Do not set integrator during proving for regen source -- prevents
*             pll_proven toggle after L_Charging.  Do not use abs value for calc.
*             of Vx_Line_Err anymore.
* 980305 SAB  Branch from S0ANGL.C30 and rename to S1ANGL.C30.  Add double-pole
*             filter for use in initialization of pll integrator.
* 980401 SAB  Use Imb_Trp_Thrs ot drive Ph_Imb_Ref also during cell test.
* 980403 SAB  Zero pll intergrator when pll not proven, ac line missing and stopped
*             Also zero pll clamps at that time.
* 980409 SAB  Update function generator to make the same as the one in motor control
*             with the exception of task1 operation
* 890415 SAB  Do not allow over-frequency fault/alarm unless pll is proven and
*             not stopped.
* 980603 CWM  Simplify Inline macros.
* 980930 SAB  Add reverse phase sequence capability.
* 021022 SAB  Remove Innr_Reg_Z1
* 021031 SAB  Normalize pll ref (vy_line) with vx_Line/Vx line moninal using new
*             var Pll_Ref
* 021104 SAB  Add new phase imbalance scheme.  Add filters to Vx_Vy_Imb
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 021120 SAB  Fix syntax error
* 021121 SAB  L_LSimMode --> SimMode
* 030109 SAB  Remove line populating Ang_Fbk_Z1
* 030212 SAB  Use L_PllVyComp = abs(L_VlnIn/L_VxLine) not -(L_VlnIn/L_VxLine)
* 030320 DS   only modulo 2pi on T2 boundaries so T2 value ok
* 030404 SAB  L_Running --> L_InnrRegEnb
* 030522 SAB  Only detect line monitor faults when Pll_Proven
* 030528 SAB  Go back to use of 0 - L_VyLine as pll error
* 030826 DS   For Phase Imbalance: L_VabLine & L_VbcLine -> L_VabSim & L_VbcSim
* 031006 SAB  Add Step variable for pll regulator. Pop UV/UF, OV/OF briefs when
*             not running
* 031014 SAB  Add L_WPllRcp
* 031212 SAB  Move slow functions to S0LINES.C30
* 040108 SAB  Remove use of ExcLineConv, ExcRotorConv
* 040120 SAB  Mask out UV alarm or fault as appropriate according to settings of
*             P.LvCfgPrtTyp
* 040330 SAB  L_PhLoss --> L_PhShort
* 051006 SAB  Add Dscm vars for pos/neg sequence discriminator
* 050922 SAB  Add functions LinePosNegDiscriminatorT1, LinePosNegDiscriminatorT2
* 051010 SAB  Bring in most of 2.x funcitonality, but modify as needed.
* 051213 SAB  Add separate clamps for pll integrator
* 060113 SAB  Zero neg seq disciminator filters when ac line absent.
* 060120 SAB  Remove redundant code
* 060125 SAB  Allow Pll to prove on ZVRT recovery as long as no line short
* 060207 SAB  Modify UV scheme to clamp voltage and allow trip at 80%. Also,
*             do not trip on uv at 0 volt until about 1.22seconds.
*             Also, move calc of L_LineMagMon to task3 and use min clamp on it
* 060313 SAB  Add new var for line short turn-off logic
* 060313 SAB  Add new logic for line short turn-off logic
* 060315 SAB  Add L_NSeqHi
* 070117 SAB  Add "Conv" in function names to distinguish from similar functions
*             for transformer secondary procedures.
* 070127 SAB  Remove In-lining from LineConvPhaseLockLoopReg, LineConvLineMonitorT1
* 070129 IB   Add support to line bridge voltage loss detection. Rename L_Vab_Fbk
*             and L_Vbc_Fbk as L_VabFbk and L_VbcFbk, also make them globals.
* 070130 SAB  Hold L_WPllMon to L_PllWT2 during early part of COOL_DYN state
* 070214 SAB  Do not give fixed frequency fault when line converter is not running
*             Use Init1Sec
* 070508 SAB  Remove repeat integration in pll
* 070604 SAB  Use Iram copies of parameters for EEs
* 070612 SAB  Remove precondition of line monitor vars and line monitor code.
* 070618 SAB  Change pll logic to closely match Wind-Sync's clamp change.
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
*______________________________________________________________________________
* Last Mod: May 17, 2008 11:49:21AM
*/


#include "x0FrameWork.h"
#include "Product.h"

// Parameters
//-----------
CREATE_PARM(L_AcLossOn,         float);     // Ac loss on threshold
CREATE_PARM(L_AcLossTm,         float);     // Ac line loss time

// Vars
CREATE_PUBVAR(L_VabLossTmr     ,float       );        // Vab line bridge loss timer
CREATE_PUBVAR(L_VbcLossTmr     ,float       );        // Vbc line bridge loss timer
CREATE_PUBVAR(L_AcLineLoss     ,unsigned    );        // Ac line loss boolean
CREATE_PUBVAR(L_VabLossSt      ,unsigned    ) = 1;    // State variable for vab line bridge loss state machine
CREATE_PUBVAR(L_VbcLossSt      ,unsigned    ) = 1;    // State variable for vbc line bridge loss state machine
CREATE_PUBVAR(L_VabFbk         ,float       );        // Line bridge a-b voltage feedback
CREATE_PUBVAR(L_VbcFbk         ,float       );        // Line bridge b-c voltage feedback
CREATE_PUBVAR(L_AngElecCmd     ,float       );        // electrical command angle for line converter
CREATE_PUBVAR(L_AngElecFbk     ,float       );        // electrical feedback angle for line converter

SinCosTyp L_SinCosFbk;                                // sine/cosine structure for angle feedback
SinCosTyp L_SinCosCmd;                                // sine/cosine structure for angle command


#define VAB_POS 1
#define VAB_NEG 0
#define VBC_POS 1
#define VBC_NEG 0

/*****************************************************************************
*
*       external references
*
*****************************************************************************/

extern float    DelTm2             ;
extern float    GmAcLinePeak       ;
extern float    GmAcLnPkT3         ;
extern float    L_Vab              ;
extern float    L_VabFbkT2         ;
extern float    L_Vbc              ;
extern float    L_VbcFbkT2         ;

extern unsigned Init1Sec           ;


/*=============================================================================
 * Function: LineConvLineMonitorT1
 * Execution Level: Task 1
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  L_DelTm1
 *                 L_VxLine
 *
 *                 EE.L_PllPrpGn
 *                 EE.L_PhLossThrs
 *
 *
 *
 *
 *
 * Global Outputs:
 *               V.L_VabFbk
 *               V.L_VbcFbk
 *
 *
 *
 *
 * Locals:
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

void LineConvLineMonitorT1()
{
    //Populate L_VabFbk, L_VbcFbk
    L_VabFbk = L_Vab;
    L_VbcFbk = L_Vbc;
}




/*=============================================================================
 * Function: LineConvLineMonitorT2
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  DelTm2
 *                 L_AcLineMag
 *
 *                 EE.L_AcLossOn
 *                 EE.L_AcLossTm
 *
 *
 * Global Outputs:
 *                 L_AcLineLoss
 *
 * Locals:
 *                 L_VabLossTmr
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

void LineConvLineMonitorT2()
{

    //////////////////////////////////////////////////////////////////////
    // Ac Line loss calcaultion  -- PROT_008
    //////////////////////////////////////////////////////////////////////

    switch ( L_VabLossSt )
    {

    /*==========================================================================
                          Vab is pos
    ==========================================================================*/
       case VAB_POS:

         L_VabLossTmr =  L_VabLossTmr + DelTm2;
         if ( L_VabFbkT2 < -PARM(L_AcLossOn) )
         {
            L_VabLossSt = VAB_NEG;
            L_VabLossTmr = 0.0;
         }
       break;


    /*==========================================================================
                         Vab is neg
    ==========================================================================*/
       case VAB_NEG:

         L_VabLossTmr =  L_VabLossTmr + DelTm2;
         if ( L_VabFbkT2 > PARM(L_AcLossOn) )
         {
            L_VabLossSt = VAB_POS;
            L_VabLossTmr = 0.0;
         }


       break;

    }

    switch ( L_VbcLossSt )
    {

    /*==========================================================================
                          Vbc is pos
    ==========================================================================*/
       case VBC_POS:

         L_VbcLossTmr =  L_VbcLossTmr + DelTm2;
         if ( L_VbcFbkT2 < -PARM(L_AcLossOn) )
         {
            L_VbcLossSt = VBC_NEG;
            L_VbcLossTmr  = 0.0;
         }
       break;


    /*==========================================================================
                         Vbc is neg
    ==========================================================================*/
       case VBC_NEG:

         L_VbcLossTmr =  L_VbcLossTmr + DelTm2;
         if ( L_VbcFbkT2 > PARM(L_AcLossOn) )
         {
            L_VbcLossSt = VBC_POS;
            L_VbcLossTmr  = 0.0;
         }


       break;

    }

   /* Variable L_AcLineLoss is intended to detect absence of oscillating 3-ph
    * line bridge voltage. Assumptions about the quality of the voltage cannot be
    * drawn from the fact that L_AcLineLoss may be false in a given time. */

    if (   ( (L_VabLossTmr > PARM(L_AcLossTm)) && (L_VbcLossTmr > PARM(L_AcLossTm)) ) || !Init1Sec   )
    {
       L_AcLineLoss = true;
    }
    else
    {
       L_AcLineLoss = false;
    }

}


/*=============================================================================
 * Function:            LineConvLineMonitorT3
 * Execution Level:     Task 3
 * Execution Time:      Not tested
 * Formal Inputs:       None
 * Formal Outputs:      None
 * Return Value:        Not applicable
 * Abstract:            This function  monitors various ac line conditions, as
 *                      listed below:
 *                        * Phase imbalance detection
 *
 *===========================================================================*/
void LineConvMonitorT3()
{


    /*=======================================================================*/
    /*                 Phase Imbalance Check  - T3                             */
    /*=======================================================================*/

    GmAcLnPkT3  = GmAcLinePeak;


}
