/*=============================================================================
* Product:     GE Power Conversion Using MarkStat C
* Host:        TMS320C31 DSP
* File:        $\PwmCtrl\Firmware\Pwm2\FW\A0FLXCMD.C30
* Language:    TI 'C'
* Copyright:   2003... GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file contains those functions which are associated with
*           conditioning of the y component of current.
* -----------------------------------------------------------------------------
*
* Functions in module:
* FlxCurrent                 - Flux Command and Current Generation
* FlxSatCurve                - Flux saturation curve
*
*==============================================================================
* 961104 SAB  Create for y-current conditioning.  Create IyFlxCmd.
* 020927 DS   modify for doubly-fed
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 030225 DS   re-arrange sign (+/-) usage; add diag vars
* 030825 DS   add VAR & volt regs
* 031014 DS   use L_WPllRcp in place of L_WNomRcp
* 031222 SAB  Move slow functions to A0FLXCMS.C30
* 040108 SAB  Remove use of ExcLineConv, ExcRotorConv
* 040119 DS   Add EON style voltage support
* 040218 DS   LvIyRefAct overides modulation index limiting
* 040712 IB   Rename variable and seq. state for new 60hz LVMD.
* 050414 IB   Replace R_IyFbk by its task2 version: R_IyFbkT2
* 051202 DS   Replace R_InnrRegLim by a new task2 var: R_RegLim2
* 051202 DS   Use filtered VregFbk to precondition VregCmd (like on 2.x)
* 051202 DS   anti-windup (+)ve delta on volt-var when mod index limiting
* 060113 SAB  Add filter R_IyCmdFil and use output only during assymentric faults.
* 060119 SAB  Remove code that zeroed reactive current command during short.
* 060126 SAB  Filter VregCmd when R_ZVRtrOff(intead of zeroing it)
*             Turn off EON when S_LineVShort
* 060217 SAB  Prepare for RPNW and rename S_IyPfTmr to IyPfTmp
* 060220 SAB  Modify for RPNW functionality
* 060225 SAB  Use L_LineMagT2, not L_VMagT2 for var calc in pf, var mode.  Precondion
*             VregOut and VregInt based on LineRunOnly when in pf mode.
* 060228 SAB  Use new parm VoltVarEnb instead of PfCtlSel (and change sense).  Also,
*             require line converter to supply max Iy during Rotor CT test to help
*             offset effect on grid.
*
* 060228 SAB  Replace S_IyPfW with IxGridWPf
* 060315 SAB  Use S_NSeqHi to allow volt-var voltage reg integrator to run

* 070108 IB   Rename S_SynSwClsd as K1Status and use R_CTChkEnb
* 070130 SAB  Fix bug that froze volt-var vreg integrator if LineRunOnly with positive
*             vreg error
* 070327 SAB  Change code to support change in ref direction of S_Ix and S_Iy.  Remove
*             Volt-Var regulator
* 070319 SAB  Remove un-used 'power factor control' variables and parameters
* 070321 SAB  Substitute PumpBackAct for R_PumpBack
* 070326 SAB  Use GmQPwrT2 and GmVMagPuT2
* 070326 SAB  Remove FbkScale
* 070326 SAB  Use GmPwrT2 to calculate VarCmd in angle mode
* 070329 SAB  Fix sign error on calc of R_FlxCmd caused by removal of FbkScale.
*             S_IyScl*S_FlxVMag*L_WRcp term not multiplied by Ratio_Rcp
* 070424 SAB  Rename Ratio_Rcp to S_RRatioRcp
* 070508 SAB  Change functionality for current limit structure
* 070517 SAB  Fix voltage reg feedback filter bug that had not effect because of gain settings.
* 070523 SAB  Remove un-used vars
* 070709 SAB  Use iram mirrors for EEs
* 070711 SAB  Make Iy go negative when hi line single phase occurs
* 070712 SAB  S_Rs --> S_R1 so that S_Rs can include cable resistance
* 071108 SAB  Delay switch to LvHvIyRefAct until utillity curve is calling for
*             a greater magnitude of current than the volt-var regulator
* 071114 SAB  During inductance test, multiply L_IyTstCmd times Sqrt(2)
* 071116 SAB  Always add L_IyHrmFCal to VregOut to make IyPFCmd
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 080115 SAB  Allow for dynamic volt-var QRegIntGn
* 080122 SAB  Add dynamic utility reference clamp variables
* 080307 SAB  Modify calc for S_FlxMtr, S_FlxMtrRcp
* 080312 SAB  Add volt reg dynamic clamp variable inputs
* 080529 SAB  Add new field weakening scheme and remove old one
* 080822 SAB  Use CI_ZVZeroIySpd
* 080916 SAB  Add IyAsySpdOn and allow torque error to drive Iy negative during asymmetric
*             faults if speed is high
* 080924 SAB  Further limit reactive current when RPNW and asymmetric ride-through in progress
* 081002 SAB  Remove commented out code
* 081002 SAB  Add vars for field weakening by rotor voltage excess
* 090127 SAB  R_VMagPuT3 --> R_VMagPuT2
* 090810 SAB  Modify low voltage range of Iy insertion with weak grids
* 090810 SAB  Remove filter for VregFbk
* 090810 SAB  Remove some volt-var filter vars.
* 090831 SAB  Add IyPFStepAdr
* 090831 SAB  Add IyPFStepAdr and R_IyStep
* 090902 SAB  Rename CI_VregTstCmd to VregTstCmd
* 111109 SAB  Fix clamping error on VregIyCmd and improper population of IyPFCmd
* 111110 SAB  Remove IyMaxLim, IyMinLim. Populate LvIyReserv and optimize logic.
*______________________________________________________________________________________
* Last Mod: November 10, 2011 0:56:40AM


*/

#include "x0FrameWork.h"
#include "Product.h"
#include "a0FlxCmd.h"
#include "c1BrgCroBarCtl.h"


/*****************************************************************************
*
*       external references
*
*****************************************************************************/
extern cBrgCroBarCtl  R_BrgCroBarCtl;

extern unsigned VPRPActT3;

extern unsigned       R_SeqStat_PackedBits;
extern unsigned       R_MctlCmd_PackedBits;
extern float        DelTm2;
extern float        DelTm3;
extern float        GmPwrT2;
extern float        GmQPwrT2;
extern float        GmVMagPuT2;
extern float        GmVMagT1;
extern float        GmVMagT2;
extern float        GmWPllRcp;
extern float        HvIyMaxDynT2;
extern float        INomGridPk;
extern float        L_IxFbkT3;
extern float        L_IyHrmFCal;
extern float        L_IyHrmFFil;
extern float        L_IyTstCmd;
extern float        LineIyPFCmd;
extern float        IyPFCmd;
extern float        LvIyMaxDynT2;
extern float        PhiAdj;
extern float        QregClmpSF;
extern float        R_IMagOLRcp;
extern float        R_IxregErrT2;
extern float        R_IyFbkT2;
extern float        R_IyLimCal;
extern float        R_KPoles;
extern float        R_Lr;
extern float        R_SpdFbk;
extern float        R_SpdFbkT3;
extern float        R_TocHiSel;
extern float        R_VMagT2;
extern float        RtrLnXfrMld;
extern float        S_FlxYFbkT2;
extern float        S_IMagOLRcp;
extern float        S_IxFbkT2;
extern float        S_IyFbkT2;
extern float        S_IyMagnEstT3;
extern float        S_LmRcp;
extern float        S_Ls;
extern float        S_PllWInt;
extern float        S_R1;
extern float        S_RRatioRcp;
extern float        S_TocHiSel;
extern float        S_VMagT2;
extern float        S_VoltsRcp;
extern float        S_VxFbkT2;
extern float        S_VyFbkT2;
extern float        S_XmNomRcp;
extern float        S_XmRcp;
extern float        TrqPwrTrim;
extern float        VARCmd;
extern float        VregCmdT3;
extern float        VregFbkT3;
extern float        VregHvMaxSlpT2;
extern float        VregHvMinSlpT2;
extern float        VregIyLimDynT2;
extern float        VregLVIyMnDyT2;
extern float        VregLvMaxSlpT2;
extern float        VregLvMinSlpT2;
extern float        VregMaxLimT2;
extern float        VregMxMnDynT2;
extern float        VregOutT3;
extern float        DnTwrTemp;
extern float        LinILimMrg;
extern float        RtrILimMrg;
extern float        StrILimMrg;
extern float        GmIyFbkT2;
extern float        GmIxFbkT3;
extern float        L_IyHrmFCalT3;
extern unsigned     GmAsymRTIp;
extern unsigned     GmLineVLo;
extern unsigned     GmNSeqHi;
extern unsigned     IyXferDone;
extern unsigned     S_K1Status;
extern unsigned     L_LTestIp;
extern unsigned     L_Running;
extern unsigned     R_CTChkEnb;
extern unsigned     R_Running;
extern unsigned     RPNWRunning;
extern unsigned     RtrLnIyXfer;
extern unsigned     S_NSeqHi;
extern unsigned     S_PllProven;
extern unsigned     SysTmSec;
extern unsigned     VARModeCmd;
extern unsigned     WBLimAct;
extern unsigned     ZVRTQuickIp;

extern float        GmVnMagPuMax;

extern unsigned     IslandDet;
extern unsigned     S_VMaxHiEnb;
extern unsigned     S_VMaxIyFast;
extern float        S_VMaxPuPeak;
extern float        S_IxFbk;

extern float        S_LSigma;

extern float        L_IxFbkT2;
extern float        L_VxFbkT2;
extern float        L_VyFbkT2;
extern float        XfmrRatioLnStr;
extern float        GmVyFbkT2;
extern float        L_VlnIn;
extern float        GmIxGrid;
extern float        R_ISpdTmpRcp;
extern float        S_ISpdTmpRcp;
extern float        GmIMaxCrvMin;
extern float        AirTempGenCab;
extern float        AirTempGenIn;

extern float        R_MaxCurOut;



extern unsigned     S_VMaxIyFast;
extern float        ISysCmdSF;
extern float        S_MaxCurOut;
extern float        R_IyCmd1;
extern float        IyMaxLim1;

// RotorFlxCurrentT1
extern unsigned     S_VMaxIyFast;



// Parameters
//-----------
CREATE_PARM(R_ModIndMax2,       float);
CREATE_PARM(R_FwkIntGn2,        float);
CREATE_PARM(R_IyCmdW,           float);     // rotor iy commnad filter omega
CREATE_PARM(S_IyCmdCTChk,       float);     // stator Iy cmd to check CT's
CREATE_PARM(R_IyNotchFilForce,  unsigned);  // Reactive current notch fitler force enable  

CREATE_PARM(R_ImagLimEnb,       unsigned);  // Current mag limit enable
CREATE_PARM(S_FlxMin,           float);     // min clamp on denominator of Trq/Flx = IxCmd
//  new flux current control
CREATE_PARM(S_VregIntGn,        float);
CREATE_PARM(S_VregPrpGn,        float);
CREATE_PARM(S_VregMax,          float);
CREATE_PARM(S_IyregIntGn,       float);
CREATE_PARM(S_IyregPrpGn,       float);
CREATE_PARM(S_IyregMax,         float);
CREATE_PARM(S_LmCalcMeld,       float);
CREATE_PARM(S_LmCalcWI,         float);
CREATE_PARM(S_LmCalcWR,         float);
CREATE_PARM(S_LmCalcMax,        float);
CREATE_PARM(S_LmCalcMin,        float);
CREATE_PARM(S_VrefSclFlW,       float);
CREATE_PARM(S_VrfSyncScl,       float);
CREATE_PARM(IxWIyLim,           float);     // Ix filter omega for Iy limit calc
CREATE_PARM(IyMagnWIyLim,       float);     // Iy magnetizing current filter for Iy limit calc
CREATE_PARM(IyXferRate,         float);     // Rotor-line iy transfer rate

// Published Variables
//--------------------
RRegLimFlgs   R_RegLim2;

CREATE_PUBVAR(RtrLnXfrMld,     float );          // Rotor line transfer meld
CREATE_PUBVAR(LineIyPFCmd,     float );
CREATE_PUBVAR(S_FlxMtr,        float );
CREATE_PUBVAR(S_FlxMtrRcp,     float );

CREATE_PUBVAR(R_FlxCmd,        float );              /* flux command */
CREATE_PUBVAR(R_FlxCmdDdt,     float );              /* flux command derivitive */
CREATE_PUBVAR(R_IyCmd ,        float );              /* y-component current command, prior to flux weakening */
CREATE_PUBVAR(R_IyCmd0,        float );              /* y-component current command */
CREATE_PUBVAR(R_IyCmd2,        float );              /* y-component current command delayed two */
CREATE_PUBVAR(R_IyCmd3,        float );              /* y-component current command delayed three */
CREATE_PUBVAR(R_IyCmdFil,      float );              /* y-component current command, prior to flux weakening, filtered*/
CREATE_PUBVAR(R_IyCmdIn ,      float );              /* y-component current command as filter intput */
CREATE_PUBVAR(R_IyCmdOffLn,    float );              /* rotor reactive current command off-line */
CREATE_PUBVAR(R_IyCmdOnLn,     float );              /* rotor reactive current command on-line */
//CREATE_PUBVAR(VregIyStpAdr,    unsigned*);           /* pointer to reactive current step address */

CREATE_PUBVAR(S_IMagLim,        float );                // Stator current magnitude limit
CREATE_PUBVAR(S_IMagMax ,       float );
CREATE_PUBVAR(S_IMagMaxUc ,     float );
CREATE_PUBVAR(IyMaxLim,         float );
CREATE_PUBVAR(IyMinLim,         float );
CREATE_PUBVAR(R_IMagMax ,       float );

CREATE_PUBVAR(L_IReserveGn,     float );
CREATE_PUBVAR(L_IReserveSS ,    float );
CREATE_PUBVAR(L_IReservSpl,     float );
CREATE_PUBVAR(L_IxFbkSqFil,     float );

CREATE_PUBVAR(S_ILimCab1Pk        , float);    // stator cable current limit peak after altitude de-rating for temperature 1 (Amp)
CREATE_PUBVAR(R_ILimCab1Pk        , float);    // rotor cable current limit peak after altitude de-rating for temperature 1  (Amp)
CREATE_PUBVAR(S_IySpillLim,     float );                // Stator reactive current spill limit
CREATE_PUBVAR(S_IyMagnetiz ,    float );

// RotorFlxCurrentT1()
CREATE_PUBVAR(S_IyCmdDyn,       float);     // Dynamic stator Iy command
CREATE_PUBVAR(S_VrfSyncScl,     float);

CREATE_PUBVAR(QregMaxDyn,      float );
CREATE_PUBVAR(QregMinDyn,      float );



/////////////////////////////////
// Aux bus volt-var regs
/////////////////////////////////

CREATE_PARM  (S_IyCmdCTChkSlwRat  , float);
CREATE_PARM  (SyncTstVsScl        , float);
CREATE_PARM  (VregMinLimFwk       , float);
CREATE_PARM  (VregMinLimFwkW      , float);

// Islanding related                              (need to add rules)
CREATE_PARM  (S_VMagPuIyLm        , float);      // Maximum allowed stator pu votlage by Iy limit
CREATE_PARM  (S_VMaxIyBias        , float);      // Stator voltage clamp Iy bias
CREATE_PARM  (S_VMaxIyGn          , float);      // Stator maximum voltage Iy gain
CREATE_PARM  (S_VMaxIyLm          , float);      // Stator maximum allowed rotor reactive current by voltage limit


CREATE_PUBVAR(S_VoltsRcp,      float );

CREATE_PUBVAR(S_1PlusLsLm,     float );              // constant for (1.0 + S_Ls/S_Lm) from rules


CREATE_PUBVAR(SyncTstVsScl ,    float   );
CREATE_PUBVAR(S_FlxVMag ,       float   );
CREATE_PUBVAR(S_IyBase ,        float   );
CREATE_PUBVAR(S_IyScl  ,        float   );
CREATE_PUBVAR(S_VrefScl,        float   );
CREATE_PUBVAR(S_VregCmd ,       float   );
CREATE_PUBVAR(S_VregErr ,       float   );
CREATE_PUBVAR(S_VregInt ,       float   );
CREATE_PUBVAR(S_VregOut ,       float   );
CREATE_PUBVAR(R_IyLim   ,       float   );
CREATE_PUBVAR(S_IyLim   ,       float   );               /* rotor Iy limit - referred to stator */
CREATE_PUBVAR(S_IyregErr,       float   );
CREATE_PUBVAR(S_IyregInt,       float   );
CREATE_PUBVAR(S_IyregOut,       float   );
CREATE_PUBVAR(S_IyRegTot ,      float   );
CREATE_PUBVAR(S_IyPf    ,       float   );
CREATE_PUBVAR(S_IyCmdCTChkRl,   float   );                // S_IyCmdCTChk after rate limiting
CREATE_PUBVAR(S_IyCmd   ,       float   );                // stator Iy cmd
CREATE_PUBVAR(S_IyFfOnLn  ,     float   );                /* stator Iy feed-forward, on-line */
CREATE_PUBVAR(S_IyFfOffLn ,     float   );                /* stator Iy feed-forward, off-line */
CREATE_PUBVAR(S_VrefSclFil ,    float   );
CREATE_PUBVAR(S_VMaxRegGn ,     float   );
CREATE_PUBVAR(S_IySpilCmd,      float   );                /* Iy sipllover to line control */

CREATE_PUBVAR(S_IyMagnEst ,     float   );                /* magnetizing current calced from feedfwd & reg out */
CREATE_PUBVAR(LvIxRefFilT3,     float   );
CREATE_PUBVAR(R_FwkAct,         unsigned);

//Low and High voltage current reference generation



// Start Islanding-HVRT vars
CREATE_PUBVAR(R_FlxCmdT1 ,      float   );                // flux command T1 for HVRT/Islanding
CREATE_PUBVAR(S_VMaxIyCmd,      float   );                // Stator Iy command for Vmag clamping
// End Islanding-HVRT vars



// Rotor Iy current command notch filter
CREATE_PUBVAR(R_IyNchFilZA0       , float);    // Rotor Iy current command notch filter z domain: A0 gain
CREATE_PUBVAR(R_IyNchFilZA1       , float);    // Rotor Iy current command notch filter z domain: A1 gain
CREATE_PUBVAR(R_IyNchFilZA2       , float);    // Rotor Iy current command notch filter z domain: A2 gain
CREATE_PUBVAR(R_IyNchFilZB1       , float);    // Rotor Iy current command notch filter z domain: B1 gain
CREATE_PUBVAR(R_IyNchFilZB2       , float);    // Rotor Iy current command notch filter z domain: B2 gain
CREATE_PUBVAR(R_IyNchFilu1        , float);    // Rotor Iy current command notch filter: input Z1 delay
CREATE_PUBVAR(R_IyNchFilu2        , float);    // Rotor Iy current command notch filter: input Z2 delay
CREATE_PUBVAR(R_IyNchFily1        , float);    // Rotor Iy current command notch filter: output Z1 delay
CREATE_PUBVAR(R_IyNchFily2        , float);    // Rotor Iy current command notch filter: output Z2 delay
CREATE_PUBVAR(R_IyCmdNchFl        , float);    // Rotor Iy current command notch filter: output
CREATE_PARM(R_IyNchFilSA0         , float);    // Rotor Iy current command notch filter S Domain A0 coefficient
CREATE_PARM(R_IyNchFilSA1         , float);    // Rotor Iy current command notch filter S Domain A1 coefficient
CREATE_PARM(R_IyNchFilSA2         , float);    // Rotor Iy current command notch filter S Domain A2 coefficient
CREATE_PARM(R_IyNchFilSB1         , float);    // Rotor Iy current command notch filter S Domain B1 coefficient
CREATE_PARM(R_IyNchFilSB2         , float);    // Rotor Iy current command notch filter S Domain B2 coefficient
CREATE_PUBVAR(WBLinSpilMrgRcp     , float );
CREATE_PUBVAR(WBLnIMagSpil        , float );
CREATE_PUBVAR(WBLnIyMinSel        , float );
CREATE_PUBVAR(R_IMagMaxUc         , float );




DATA_PASS(LvIxRefFil,     LvIxRefFilT3   , T2_T3,      FILTERED);

/***************************************************************************
*
*                               Private Variables
*
*****************************************************************************/



/*=============================================================================
 * Function: RotorIyControlT2
 * Execution Level: Task 2
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  DelTm2
 *                 AcLineMag
     *
 *
 *
 * Global Outputs: PllW
 *
 * Locals:         PhImbInlim
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: Push2Flt()
 *
 * Abstract:
 *    This function controls power factor
 * loop error.
 *
 * Related Visos:
 *
 *===========================================================================*/
void RotorIyControlT2(void)
{
    float    Del_Lim, Slw_Del;

    if ( R_CTChkEnb)
    {
       RtrLnXfrMld = 0.0;

       Del_Lim = DelTm2*PARM(S_IyCmdCTChkSlwRat);
       Slw_Del = (PARM(S_IyCmdCTChk) - S_IyCmdCTChkRl);

       if (Slw_Del < -Del_Lim) Slw_Del = -Del_Lim;

       S_IyCmdCTChkRl = S_IyCmdCTChkRl + Slw_Del;

       S_IyCmd = S_IyCmdCTChkRl;
       S_IySpilCmd = 0.0;
       LineIyPFCmd = MIN(-S_IyCmdCTChkRl, INomGridPk * VregMaxLimT2) + L_IyHrmFFil;
    }
    else if ( RPNWRunning)
    {
        S_IyCmdCTChkRl = 0.0;
        RtrLnXfrMld    = 0.0;
        S_IyCmd        = 0.0;
        S_IySpilCmd    = 0.0;

        LineIyPFCmd = IyPFCmd;
    }
    else
    {
        S_IyCmdCTChkRl = 0.0;

        if ( R_SeqStat.R_TrqEnbStat || !S_VMaxHiEnb)
        {
            if (RtrLnIyXfer)
            {
               RtrLnXfrMld = RtrLnXfrMld + PARM(IyXferRate);  //Transfer at rate of total amount in 25msec
               if ( RtrLnXfrMld > 1.0)
               {
                   RtrLnXfrMld = 1.0;
                   IyXferDone = true; //Outer sequence will transfer to stop rotor state now
               }

               S_IyCmd  = (1.0 - RtrLnXfrMld)*IyPFCmd;
               S_IySpilCmd = (RtrLnXfrMld*IyPFCmd);
            }
            else //running full rotor command, not transfer
            {
               RtrLnXfrMld = 0.0;
               if ( IyPFCmd > S_IySpillLim)
               {
                    S_IySpilCmd = S_IySpillLim;
               }
               else if (IyPFCmd < -S_IySpillLim)
               {
                    S_IySpilCmd = -S_IySpillLim;
               }
               else
               {
                    S_IySpilCmd = IyPFCmd;
               }
               S_IyCmd = IyPFCmd - S_IySpilCmd;
            }

            LineIyPFCmd = S_IySpilCmd;  //  + LineIyPFComp (add this term to get exactly unity pf on line brg)
        }
        else //Not TrqEnbStat
        {
            RtrLnXfrMld = 0.0;
            S_IyCmd     = 0.0;
            S_IySpilCmd = 0.0;
            LineIyPFCmd = IyPFCmd;
        }

    }



}

/*=============================================================================
 * Function: FlxCurrent
 * Execution Level: Task 2
 * Execution Time: Untimed
 * Formal Inputs: none
 * Formal Outputs: none
 *
 * Global Inputs:
 *                 R2_Rcp
 *
 *                 EE.Flx1
 *                 EE.Im1
 *                 EE.R_L1SclSim
 *                 EE.R2_Sim
 *
 * Global Outputs: Iy_SS
 *                 L1_Cmd
 *
 * Locals:         L1_Sim
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function generates command flux and flux current. Motor Taus
 *    are also calculated.
 *
 *===========================================================================*/

void RotorFlxCurrent()
{
    float   Temp1;
    float   GmWRcp;
    float   S_IyregMaxTmp;


    GmWRcp = GmWPllRcp;

    R_IyLim = R_IyLimCal;
    S_IyLim = R_IyLim * S_RRatioRcp;





    if (R_MctlCmd.R_FlxCmdOn == true) //Turns on to sync, off when stopping rotor
    {
         if (  S_K1Status || R_CTChkEnb  )
         {
            S_VregInt   = 0.0;

            /* Calculate base magnetizing current */
            float Ex_Tmp, Ey_Tmp;
            Ex_Tmp = S_VxFbkT2 + S_IxFbkT2 * S_R1;
            Ey_Tmp = S_VyFbkT2 + S_IyFbkT2 * S_R1;
            S_FlxVMag = sqrt(Ex_Tmp*Ex_Tmp + Ey_Tmp*Ey_Tmp);

            S_IyBase = S_FlxVMag * S_XmRcp * ISysCmdSF;
            S_IyScl  = 1.0;



            /////////////////////////////////////
            // Stator Iy regulator
            /////////////////////////////////////

            S_IyregErr = S_IyFbkT2 - S_IyCmd;

            /* Calculate and clamp integral path */
            S_IyregInt = S_IyregInt + PARM(S_IyregIntGn)*DelTm2*S_IyregErr;

            S_IyregMaxTmp = PARM(S_IyregMax) * S_VMaxRegGn;//sets limits to zero when S_VxMaxIyFast, otherwise 1.0
            if (S_IyregInt >=  S_IyregMaxTmp)
            {
               S_IyregInt =  S_IyregMaxTmp;
            }
            if (S_IyregInt <= -S_IyregMaxTmp)
            {
               S_IyregInt = -S_IyregMaxTmp;
            }

            R_RegLim2.S_IyregLimP = false;
            R_RegLim2.S_IyregLimN = false;

            /* Calculate proportional path, sum with int. path, clamp */
            S_IyregOut = S_IyregInt + S_IyregErr*PARM(S_IyregPrpGn);
            if ( S_IyregOut >= PARM(S_IyregMax))
            {
                 S_IyregOut = PARM(S_IyregMax);
                 R_RegLim2.S_IyregLimP = true;
            }
            if ( S_IyregOut <= -PARM(S_IyregMax))
            {
                 S_IyregOut = -PARM(S_IyregMax);
                 R_RegLim2.S_IyregLimN = true;
            }

            /* Summation of stator Iy command terms */
            S_IyFfOnLn = S_IyBase + S_IyCmd + (S_IyCmd * S_Ls*S_LmRcp);
            S_IyRegTot = S_IyregOut - S_IyFfOnLn;

            /* calculate estimated magnetizing current */
            S_IyMagnEst = S_IyCmd - S_IyRegTot;

            R_IyCmdIn   = -S_IyRegTot  * PARM(S_RRatio);

            R_IyCmdFil  = R_IyCmdFil + PARM(R_IyCmdW)*DelTm2 * (R_IyCmdIn - R_IyCmdFil);


            if ( S_NSeqHi) //Use filtered Iy command if in assymetric line transient
            {
                R_IyCmdOnLn = R_IyCmdFil;
            }
            else
            {
                R_IyCmdOnLn = R_IyCmdIn;
            }
            R_IyCmdOffLn    = 0.0;
            R_IyCmd         = R_IyCmdOnLn;


         }
         else
         {
            // Sync breaker is open

            // Calculate base magnetizing current
            S_FlxVMag  = GmVMagT2;

            S_FlxVMag = S_FlxVMag * SyncTstVsScl;

            S_IyBase = S_FlxVMag * S_XmNomRcp;

            if (SyncTstVsScl < 1.0F)
            {
                S_IyScl  = MIN(ABS(PARM(R_RPMSync)/R_SpdFbk), 1.0);
            }
            // Scale down magnetizing current if Pll not locked or above base freq
            else if (S_PllProven)
            {
                S_IyScl  = MIN(ABS(PARM(L_LineFrq)/S_PllWInt), 1.0);
            }
            else
            {
                S_IyScl  = 0.50;                          // need to make this into F.V?
            }

            // Stator voltage regulation

            // Scale down voltage ref if below base freq
            S_VrefSclFil = S_VrefSclFil + (DelTm2*PARM(S_VrefSclFlW))*( (S_PllWInt*GmWRcp) - S_VrefSclFil );
            S_VrefScl = MIN(ABS(S_VrefSclFil), 1.0);
            S_VregCmd = S_VrfSyncScl * S_FlxVMag * S_VrefScl;
            S_VregErr = S_VregCmd - S_VMagT2;

            if ( R_MctlCmd.S_VregEn )
            {
                // Calculate and clamp integral path
                S_VregInt = S_VregInt + PARM(S_VregIntGn)*DelTm2*S_VregErr;

                if (S_VregInt >=  PARM(S_VregMax))  S_VregInt =  PARM(S_VregMax);
                if (S_VregInt <= -PARM(S_VregMax))  S_VregInt = -PARM(S_VregMax);

                R_RegLim2.S_VregLimP = false;
                R_RegLim2.S_VregLimN = false;

                // Calculate proportional path, sum with int. path, clamp
                S_VregOut = S_VregInt + S_VregErr*PARM(S_VregPrpGn);
                if ( S_VregOut >= PARM(S_VregMax))
                {
                     S_VregOut = PARM(S_VregMax);
                     R_RegLim2.S_VregLimP = true;
                }
                if ( S_VregOut <= -PARM(S_VregMax))
                {
                     S_VregOut = -PARM(S_VregMax);
                     R_RegLim2.S_VregLimN = true;
                }
            }
            else
            {
                // Hold regulator value constant while closing onto grid
                S_VregOut = S_VregInt;
            }


            // Summation of stator Iy command terms
            S_IyFfOffLn = S_IyBase * S_IyScl;
            S_IyRegTot  = -(S_IyFfOffLn + S_VregOut);

            if (S_IyRegTot >  S_IyLim)
            {
                S_IyRegTot =  S_IyLim;
            }
            if (S_IyRegTot < -S_IyLim)
            {
                S_IyRegTot = -S_IyLim;
            }
            R_IyCmdOffLn   = -S_IyRegTot * PARM(S_RRatio);
            R_IyCmdOnLn    = 0.0;
            R_IyCmd        = R_IyCmdOffLn;

            // populate estimated magnetizing current
            S_IyMagnEst = -S_IyRegTot;
         }


    }
    else  // end of "if (R_MctlCmd.R_FlxCmdOn == FALSE)"
    {
        R_FlxCmd    = 0.0;
        R_FlxCmdDdt = 0.0;
        R_IyCmd     = 0.0;
        S_VregInt   = 0.0;
        S_VregOut   = 0.0;
        S_IyregOut  = 0.0;
        S_IyregInt  = 0.0;
        S_IyMagnEst = 0.0;

    }

    // Notch Filter for Rotor Iy Command
    R_IyCmdNchFl = R_IyNchFilZA0 * R_IyCmd
                  - R_IyNchFilZB1 * R_IyNchFily1
                  + R_IyNchFilZA1 * R_IyNchFilu1
                  - R_IyNchFilZB2 * R_IyNchFily2
                  + R_IyNchFilZA2 * R_IyNchFilu2;

    R_IyNchFilu2  = R_IyNchFilu1;
    R_IyNchFilu1  = R_IyCmd;
    R_IyNchFily2  = R_IyNchFily1;
    R_IyNchFily1  = R_IyCmdNchFl;



 /*==========================================================================
                        Copy R_IyCmd to R_IyCmd0
  ==========================================================================*/
    if (PARM(SeriesCmpEnb) || PARM(R_IyNotchFilForce))
    {
        R_IyCmd0          = R_IyCmdNchFl;
    }
    else
    {
        R_IyCmd0          = R_IyCmd;
    }

 /*==========================================================================
                             Calculate "R_FlxCmd"
// ========================================================================== */

    R_FlxCmd = (S_IyScl*S_FlxVMag*GmWRcp + S_Ls*S_IyCmd) * S_RRatioRcp
                                            + R_Lr*(R_IyCmd0+R_IyFbkT2)*0.5;

  //  R_FlxCmdDdt = (R_FlxCmd - R_FlxCmdZ1) * DelTm2Rcp;   need to filter/limit this?
  //  R_FlxCmdZ1 = R_FlxCmd;
    R_FlxCmdDdt = 0;


 /*==========================================================================
                     Calculate Flx_Mtr Flx_Mtr_Rcp
 //========================================================================== */
    Temp1  = S_FlxYFbkT2;

    if ( (Temp1 < PARM(S_FlxMin)) || (PARM(TestMode) == PUMP_BACK) )
    {
        Temp1 = PARM(S_FlxMin);
    }

    S_FlxMtr = Temp1;

    if (R_MctlCmd.R_FlxCmdOn == true)
    {
        S_FlxMtrRcp =  NoNANLim( 1.0/S_FlxMtr, 1.0e6, -1.0e6 );
    }
    else
    {
        S_FlxMtrRcp  = 0.0;
    }


}


/*=============================================================================
 * Function: FlxCurrentT1
 * Execution Level: Task 1
 * Execution Time: Untimed
 * Formal Inputs: none
 * Formal Outputs: none
 *
 * Global Inputs:
 *
 * Global Outputs:
 *
 * Locals:
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function generates command flux and flux current for the case
 *    where an HVRT event or islanding event has been detected. It also
 *    generates the stator reactive current command for those events.
 *
 *===========================================================================*/

void RotorFlxCurrentT1()
{

    float VMaxPeakIn;

// Begin Dynamic reactive current clamp control  -- TRQ_042

    // Begin Dynamic reactive current clamp control
    VMaxPeakIn  = PARM(S_VMaxIyGn) * (PARM(S_VMagPuIyLm) - S_VMaxPuPeak);
    if ( VMaxPeakIn > 0.0F)
    {
        VMaxPeakIn = 0.0F;
    }
    else if ( VMaxPeakIn < PARM(S_VMaxIyLm))
    {
        VMaxPeakIn = PARM(S_VMaxIyLm);
    }
    S_VMaxIyCmd = VMaxPeakIn;

    if(S_VMaxIyFast)
    {
        S_IyCmdDyn = S_VMaxIyCmd;
    }
    else
    {
        S_IyCmdDyn = S_IyCmd;
    }
// End Dynamic reactive current clamp control  -- TRQ_042



//  Start Calculation of  "R_FlxCmdT1"   -- TRQ_025

    R_FlxCmdT1 = (S_IyScl*(GmVMagT1 + S_IxFbk * S_R1)*GmWPllRcp + S_Ls*S_IyCmdDyn) * S_RRatioRcp
                                            + R_Lr*R_IyCmd1;

//  End Calculation of  "R_FlxCmdT1"   -- TRQ_025

}



// from slow file

/*=============================================================================
 * Function: CurrentLimitCalT3
 * Execution Level: Task 3
 * Execution Time: Not tested
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:
 *
 *
 *
  * Global Outputs:
 *
 *
 *
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Diagram(s): ALG_047
 * Related diagram(s): ALG_046
 *
 *
 * Abstract:
 *    This function calculates current lmits for the rotor and stator algorithms and
 * the volt-var fuctions.  Inputs from the toc models are used here, as well as other
 * inputs.
 *
 *
 *===========================================================================*/
void CurrentLimitCalcT3()
{

    //float R_IxFbkSqr, S_IxFbkSqr;
    //float IFilGn;
    float L_IResSq;
    //float GmIResSq;
    float R_IMagOL, S_IMagOL;
    //float WBLnIyMinTmp;
    //float S_IMaxCrvMinT;
    float  L_IReservTemp;


    //Convert rms parameters to peak amps


    //R_ILim1      = SQRT2 * PARM(R_ILim1;   // convert RMS parms to native units (peak A)
    //R_ILim2      = SQRT2 * PARM(R_ILim2;
    //S_ILim1      = SQRT2 * PARM(S_ILim1;
    //S_ILim2      = SQRT2 * PARM(S_ILim2;
    //R_IRatSSLim  = SQRT2 * PARM(R_IRatSSLim;
    R_IMagOL     = SQRT2 * PARM(R_IMagOL);
    S_IMagOL     = SQRT2 * PARM(S_IMagOL);
    R_ILimCab1Pk = SQRT2 * PARM(R_ILimCab1);
    S_ILimCab1Pk = SQRT2 * PARM(S_ILimCab1);



    ////////////////////////////////////////////////////////////////
    // Line Converter Reserve Current -- TRQ_410
    // Qreg limits and torque limit due to heating/LVRT
    ///////////////////////////////////////////////////////////////

    /* calculate line converter reserve for steady-state VARs */
    L_IxFbkSqFil +=  PARM(IxWIyLim*DelTm3)*( (L_IxFbkT3*L_IxFbkT3) - L_IxFbkSqFil);
    L_IResSq = MAX( (2.0F*PARM(L_IRatSSLim)*PARM(L_IRatSSLim) - L_IxFbkSqFil), 0.0F);
    if (L_IResSq < 0.0F )
    {
        L_IResSq  = 0.0F;
    }
    L_IReserveSS = sqrtf(L_IResSq);
    L_IReservTemp = MAX(LinILimMrg, 0.0F);
    L_IReservTemp = MIN(PARM(WBLinSpilMrg), L_IReservTemp);
    L_IReserveGn = L_IReservTemp * WBLinSpilMrgRcp;
    L_IReservSpl =  L_IReserveSS * L_IReserveGn;

    WBLnIyMinSel = MIN(StrILimMrg, (RtrILimMrg*S_RRatioRcp));

    WBLnIMagSpil = MAX(0.0, (SQRT2*PARM(WBStrSpilGn)*(PARM(WBStrSpilMrg) - WBLnIyMinSel) ) );

    S_IySpillLim = MIN(WBLnIMagSpil, L_IReservSpl);




    if (!R_SeqStat.R_TrqEnbStat || RPNWRunning || RtrLnIyXfer)
    {
        // Iy limits for VAR cmd limit calcs
        IyMaxLim  =  L_IReserveSS - L_IyHrmFFil;
        IyMinLim  = -L_IReserveSS - L_IyHrmFFil;
    }
    else
    {
        // Iy limits for VAR cmd limit calcs
        IyMaxLim  =  IyMaxLim1 - L_IyHrmFFil;
        IyMinLim  = -IyMaxLim1 - L_IyHrmFFil;
    }

    QregMaxDyn = IyMaxLim * VregCmdT3 * QregClmpSF;
    QregMinDyn = IyMinLim * VregCmdT3 * QregClmpSF;

    //////////////////////////////////////////
    // Speed Based Current limits -- TRQ_400
    //////////////////////////////////////////


    /* current limits for VAR - volt regulator */







    //////////////////////////////////////////////////
    // Rotor/Stator max current with TOC -- TRQ_400
    /////////////////////////////////////////////////

    //Rotor
    R_IMagMaxUc = PARM(R_IMagOL) + RtrILimMrg;

    if ( R_IMagMaxUc > PARM(R_IMagOL))
    {
        R_IMagMax = PARM(R_IMagOL);
    }
    else if ( R_IMagMaxUc < R_MaxCurOut )
    {
        R_IMagMax = R_MaxCurOut;
    }
    else
    {
        R_IMagMax = R_IMagMaxUc;
    }


    //Stator
    S_IMagMaxUc = PARM(S_IMagOL) + StrILimMrg;

    if ( S_IMagMaxUc > PARM(S_IMagOL))
    {
        S_IMagMax = PARM(S_IMagOL);
    }
    else if ( S_IMagMaxUc < S_MaxCurOut )
    {
        S_IMagMax =  S_MaxCurOut;
    }
    else
    {
        S_IMagMax = S_IMagMaxUc;
    }




    //Composite


    S_IMagLim = MIN( (S_RRatioRcp*R_IMagMax), S_IMagMax) * SQRT2;




    ///////////////////////////////////////////////
    // Filter magnetizing current */   -- TRQ_470
    ///////////////////////////////////////////////

    S_IyMagnetiz += PARM(IyMagnWIyLim )* DelTm3 * (S_IyMagnEstT3  - S_IyMagnetiz);

}



