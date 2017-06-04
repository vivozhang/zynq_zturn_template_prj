/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        A0VARCAL.C30
* Language:    TI 'C'
* Copyright:   2003....GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file contains those functions which are associated with
*           regulator control and parameter conditioning.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* RegControl       - regulator control function
* ScaleCalc        - Parameter and variable scaling
* CurLimCal        - Current limit calculation
*
*==============================================================================
* 961104 0000 SAB  Create for regulator control and parameter conditioning.
* 020821 DS   Modify for doubly-fed
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 030618 DS   Apply regen-inhibit only in super-synchronous operation
* 030925 SAB  Add SpdSimReq
* 040107 SAB  Move slow functions to A0VARCLS.C30
* 040219 SAB  Move in IRAM_FLOATs from A0VARCAL.C30
* 040712 IB   Modified for new 60hz LVMD, adding R_CBDlyTm1x.
* 060113 SAB  Zero Torque command when LineShortIP
* 070109 IB   Remove R_SCDlyTm1x and R_CBDlyTm1x
* 070424 SAB  Rename Ratio_Rcp to S_RRatioRcp
* 070612 SAB  Use Iram copies of parameters for EEs
* 070625 SAB  Rename Dc ireg vars
* 070801 SAB  Add R_PwrCmd
* 070925 SAB  Add S_PllregStep
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 080303 SAB  Add PllTrqRed and S_VMagPu to make input to voltage based torque
*             reduction filter, then clamp the output of the filter.
* 080314 SAB  Add R_KPoles
* 081113 SAB  Use TrqLimVMgScl to normalize voltage based torque limit in PB mode
* 090310 SAB  Remove R_SpdSimReqV
* 090826 SAB  Remove call to RotorScanTach
* 110825 SAB  Move Pll torque reduction scheme back to task2
*             Add Phase lock loop compensation by torque command
*______________________________________________________________________________
* Last Mod: September 15, 2011 5:37:44PM
*/


#include   "x0FrameWork.h"
#include   "Product.h"
#include   "a0VarCal.h"
#include   "c1BrgCroBarCtl.h"


//Parameters
//------------------------------------------------------------------------------------------------------------------
CREATE_PARM(R_TrqLmTtScl,       float);     // Scale factor between torque limit with/without trim
CREATE_PARM(R_WVMaxSpd,         float);     // Rotor max voltage filter omega for speed range reduction
CREATE_PARM(R_VMaxOnLvl,        float);     // Rotor max voltage for speed range reduction on level
CREATE_PARM(R_VMaxOffLvl,       float);     // Rotor max voltage for speed range reduction off level
CREATE_PARM(SpdPllFilW,         float);     // Power pll filter omega
CREATE_PARM(R_DchgRat,          float);     // integrator discharge rate
CREATE_PARM(S_PllrStpAdr,       float*);
CREATE_PARM(R_IyLimAdj,         float);     // Iy current limit if not using magnitude current limit
CREATE_PARM(R_ILimMax,          float);     // Maximum allowable Ix limit
CREATE_PARM(R_ILimAdj1,         float);     // Ix limit bender #1
CREATE_PARM(R_ILimAdj2,         float);     // Ix limit bender #2

// Diagnostics
//------------
CREATE_DIAG(R_PwrDipFlt);       // Power Dip timer expire fault

// Variables
//------------------------------------------------------------------------------------------------------------------
    CREATE_PUBVAR(S_LmRcp         ,       float);
    CREATE_PUBVAR(S_LsLmSumRcp    ,       float);
    CREATE_PUBVAR(S_XmRcp         ,       float);
    CREATE_PUBVAR(S_XmNomRcp      ,       float);

    CREATE_PUBVAR(R_IyLimCal      ,       float);           /* dynamic Iy limit */
    CREATE_PUBVAR(R_IxFilInt      ,       float);           /* Current Mag limit: Ix filter state */
    CREATE_PUBVAR(R_KPolesRcp     ,       float);           /* 2/3/(pole pairs) */
    CREATE_PUBVAR(R_LsRcp         ,       float);           /* reciprocal of R_LSigma */
    CREATE_PUBVAR(R_PolePrRcp     ,       float);           /* pole pairs reciprocal */
    CtlBt2 R_MtrCtl1 = {0};
    CREATE_PUBVAR(R_PdPosLim      ,       float);           /* Power Dip positive current limit */
    CREATE_PUBVAR(R_PdNegLim      ,       float);           /* Power Dip negative current limit */
    CREATE_PUBVAR(R_IxPdOut       ,       float);           /* Power Dip voltage reg output */
    CREATE_PUBVAR(R_PdTimer       ,       float);           /* Power dip timer */
    CREATE_PUBVAR(R_PwrDipEnb     ,       unsigned);        /* Power dip enable bit */
    CREATE_PUBVAR(R_TrqLimPos     ,       float);           /* Positive torque limit */
    CREATE_PUBVAR(R_TrqLimNeg     ,       float);           /* Negative torque limit */
    CREATE_PUBVAR(R_TrqLmTtNeg    ,       float);           /* Negative torque limit on total */

    CREATE_PUBVAR(S_XmAdptEnb     ,       unsigned);        /* XM adaption enable */
    CREATE_PUBVAR(S_LmNom         ,       float);           /* nominal value of S_Lm */
    CREATE_PUBVAR(S_VmCalc        ,       float);           /* calculated magnetizing voltage */
    CREATE_PUBVAR(S_ImCalc        ,       float);           /* calculated magnetizing current */
    CREATE_PUBVAR(S_LmCalc        ,       float);           /* calculated value of S_Lm */
    CREATE_PUBVAR(R_RrMeas        ,       float);           /* calculated values of rotor resistance (includes cable) */
    CREATE_PUBVAR(R_PwrCmd        ,       float);           /* Power command        */
    CREATE_PUBVAR(S_IyMaxSpd      ,       float);
    CREATE_PUBVAR(R_IyMaxSpd      ,       float);
    CREATE_PUBVAR(S_IyMinSpd      ,       float);
    CREATE_PUBVAR(R_IyMinSpd      ,       float);
    CREATE_PUBVAR(R_FreqMaxSpd    ,       float);
    CREATE_PUBVAR(R_VyMaxSpd      ,       float);
    CREATE_PUBVAR(R_VxMaxSpd      ,       float);
    CREATE_PUBVAR(R_VMaxSpd       ,       float);
    CREATE_PUBVAR(R_FreqMinSpd    ,       float);
    CREATE_PUBVAR(R_VyMinSpd      ,       float);
    CREATE_PUBVAR(R_VxMinSpd      ,       float);
    CREATE_PUBVAR(R_VMinSpd       ,       float);
    CREATE_PUBVAR(R_VMaxSpdHi     ,       unsigned);
    CREATE_PUBVAR(R_VMinSpdHi     ,       unsigned);
    CREATE_PUBVAR(R_SpdRngRed     ,       unsigned);
    CREATE_PUBVAR(R_NsXStep       ,       float);
    CREATE_PUBVAR(R_NsYStep       ,       float);
    CREATE_PUBVAR(R_DcXStep       ,       float);
    CREATE_PUBVAR(R_DcYStep       ,       float);
    CREATE_PUBVAR(S_PllregStep    ,       float);           // Pll regulator step
    CREATE_PUBVAR(R_HzRat         ,       float);           /* Motor nameplate Hz */
                                                            // Power calc for angle shift
    CREATE_PUBVAR(R_IyregPSlp     ,       float);           // Slope for y-current regulator proportional dynamic gain funciton
    CREATE_PUBVAR(R_IyregPRtDy    ,       float);           // Dynamic ratio for y-current regulator proportional dynamic gain funciton
    CREATE_PUBVAR(R_IyrgPSpdIn    ,       float);           // Speed intput for rotor current regulator dynamic proportional gain
    CREATE_PUBVAR(Imped_Scl       ,       float);
	CREATE_PUBVAR(SpdPwrPllFil    ,       float);
	CREATE_PUBVAR(TrqLimVMag      ,       float);
	CREATE_PUBVAR(R_IxVdcGnDyn    ,       float); // rotor vdc ix dynamic gain
    CREATE_PUBVAR(SRVRatio,      float); // Stator-rotor voltage ratio
	CREATE_PUBVAR(S_IxFilIyLim ,    float );
	
/*****************************************************************************
*
*       external references
*
*****************************************************************************/
extern cBrgCroBarCtl  R_BrgCroBarCtl;

extern   float  R_USqrFil;                  /* Filtered Ux^2 + Uy^2 */

extern   float     S_VxFbkT3;
extern   float     S_PllWT3;
extern   float     S_Ls;
extern   float     S_Lr;
extern   float     S_Lm;
extern   float     S_IyMagnetiz;
extern   float     S_IyFbkT3;
extern   float     SRVRatioRcp;
extern   float     S_1PlusLsLm;
extern   float     R_WRtr;
extern   float     R_VyStart;
extern   float     R_VdcFbkT2;
extern   float     R_TrqLimMtr;
extern   float     R_TrqLimGenS;
extern   float     R_TrqLimGen;
extern   float     R_TrqCmdInT2;
extern   float     R_SpdT3;
extern   float     R_SpdFbkAvg;
extern   float     R_SpdFbk;
extern   unsigned  R_Running;
extern   float     R_R2;
extern   float     R_PullOutLim;
extern   float     R_Lr;
extern   float     R_IyStart;
extern   float     R_IyLim;
extern   float     R_IyFbkT3;
extern   float     R_IxVdcGnDyn;
extern   float     R_IxMax;
extern   float     R_IxFbkT2;
extern   float     R_IxCmdT3;
extern   unsigned  R_InnrRegEnb;
extern   float     R_IRatSSLimPk;
extern   float     R_ILimAdj;
extern   float     R_FlxCmd;
extern   float     R_DelTm1Rcp;
extern   float     R_DelTm1;
extern   float     R_DchgScl;
extern   unsigned  GmPllHot;
extern   unsigned  GmLineVShort;
extern   float     DelTm3;
extern   float     DelTm2Rcp;
extern   float     DelTm2;
extern   float     DTDGnTrqLmNm;

extern   float     TrqLimVMag;
extern   float     PwrLimVMagPU;
extern   float     LvPwrTrqScl;
extern   float     SpdPwrPllFil;


extern float       R_IyregPGnDy;
extern float            SpdPllFilGn;
extern float R_IxLimCal;
extern float       S_RRatioRcp;

/*=============================================================================
 * Function: RegControl
 * Execution Level: Task 1
 * Execution Time: 0.72usec  12/19/96--Rev:001.01.01.A  (Need new timing)
 * Formal Inputs: None
 * Formal Outputs: None
 *
 *
 * Abstract:
 *    This function calculates logical control variables which are used to
 * determine the sense of switches throughout the control algorithm. This
 * routine should be called first among other reg. control routines in Task 1.
 *
 *===========================================================================*/
void   RotorRegControl(void)
{

    /* ================ Tach Loss detection ================ */


}


 /****************************************************************************
 * Function:         CurLimCal
 * Execution Level:  Task 2
 * Execution Time:   Not measured
 *
 * Formal Inputs:    R_IxFbkT2
 *                   R_IyFbkT2
 *                   R_VdcFbkT2
 *                   R_SpdFbkT2
 *
 * Global Inputs:
 * Global Outputs:  R_PullOutLim
 *                  R_IxMax
 *                  Pd_Ix_Out
 *                  R_TrqLimPos
 *                  R_TrqLimNeg
 *
 *
 * Return Value:     None
 * Function Calls:   None
 *
 * Abstract:        Ix Current limit calculations:
 *                     Limits includes R_IMag limit, Pull-out limit and Power-Dip limit.
 *                     The resultant limits are stored in R_PdLim_Pos and R_PdLim_Neg.
 *                  Power-Dip voltage reg. control calculation (R_IxPdOut).
 *                  Pos and Neg torque limits (R_TrqLimPos and R_TrqLimNeg) calculations.
 *
 ******************************************************************************/
void            RotorCurLimCal(void)
 {
   float Temp1, Temp2;
   float Pd_Out, Pd_PLim, Pd_NLim;

    /* ============ Current Magnitude limit ============== */


    // Ix filter for magnitude current limiter

    if(PARM(R_IyLimW) == 0.0)
    {
        R_IxFilInt = R_IxFbkT2;
    }
    else
    {
        R_IxFilInt = R_IxFilInt + DelTm2 * PARM(R_IyLimW) * (R_IxFbkT2 - R_IxFilInt);
    }

    Temp1 = R_IxFilInt;

    Temp2 = R_IRatSSLimPk;

    Temp2 = Temp2 * Temp2 - Temp1 * Temp1;
    if(Temp2 > 0.0)
    {
       Temp1 = sqrtf(Temp2);
    }
    else
    {
       Temp1 = 0.0;
    }

    if(PARM(R_ImagLimEnb))
    {
        R_IyLimCal = Temp1;                                     // store Iy limit
    }
    else
    {
        R_IyLimCal = PARM(R_IyLimAdj);
    }


    /* =================== Pull Out Limit ================= */

    R_PullOutLim = R_FlxCmd * R_LsRcp * PARM(R_PullOutScl);

    /* apply pull-out limit */
    if ( R_IxLimCal > R_PullOutLim)
    {
        R_IxMax = R_PullOutLim;
    }
    else
    {
        R_IxMax = R_IxLimCal;
    }


    /* ===================== Power dip action ==================== */

    if(R_InnrRegEnb)
    {
        if(R_VdcFbkT2 < PARM(R_PdActLvl))
        {
        R_PwrDipEnb = true;
        }
        else
        {
        if(R_VdcFbkT2 > PARM(R_PdRcyLvl)) R_PwrDipEnb = false;
        }
    }
    else
    {
        R_PwrDipEnb = false;
        R_PdTimer = 0.0;
    }

    if(R_PwrDipEnb)
    {
       // Start power dip fault timer
       R_PdTimer = R_PdTimer + DelTm2;
       if(R_PdTimer >= PARM(R_PdTm))
       {
           PUSH_DIAG(R_PwrDipFlt);
       }

       // dc bus voltage regulation
       Temp1 = PARM(R_PdPrpGn) * (PARM(R_PdActLvl) - R_VdcFbkT2);
       if(Temp1 > PARM(R_PdLim))
       {
       Temp1 = PARM(R_PdLim);
       }
       else
       {
         if(Temp1 < 0.0) Temp1 = 0.0;
       }

       if(R_SpdFbk - PARM(R_RPMSync) < - PARM(R_PdSpdThr))
       {
         Pd_Out = Temp1;  // positive
         Pd_NLim = 0.0;
         Pd_PLim = R_IxMax;
       }
       else if(R_SpdFbk - PARM(R_RPMSync) > PARM(R_PdSpdThr))
       {
         Pd_Out = -Temp1;  // negative
         Pd_NLim = - R_IxMax;
         Pd_PLim = 0.0;
       }
       else
       {
         Pd_Out = 0.0;
         Pd_NLim = 0.0;
         Pd_PLim = 0.0;
       }
    }
    else
    {
       R_PdTimer = R_PdTimer - DelTm2;
       if(R_PdTimer < 0.0) R_PdTimer = 0.0;
       Pd_PLim = R_IxMax;
       Pd_NLim = -R_IxMax;
       Pd_Out = 0.0;
    }



    /* ================Zero limits when short is present ================= */

    if ( GmLineVShort)
    {
       Pd_PLim = 0.0;
       Pd_NLim = 0.0;
    }

    /* ================= store limits ================= */


    R_IxPdOut = Pd_Out;
    R_PdPosLim = Pd_PLim;
    R_PdNegLim = Pd_NLim;

    /* ====== Calculate Positive/Negative Torque limits =========== */

    // Regen limit control
    TrqLimVMag      = PwrLimVMagPU * LvPwrTrqScl * PARM(LvPwrLmPBScl);
    R_TrqLmTtNeg    = -MIN(R_TrqLimGenS * PARM(R_TrqLmTtScl), (TrqLimVMag + DTDGnTrqLmNm));


    // Find the most restrictive case
    R_TrqLimGen = MIN(R_TrqLimGenS, TrqLimVMag);

    // Determination of R_TrqLimPos and R_TrqLimNeg
    /* if speed is positive and not near zero speed */
        /* Limits are +Motoring, -Generating */
    R_TrqLimPos = R_TrqLimMtr;
    R_TrqLimNeg = -R_TrqLimGen;

    R_PwrCmd = -R_TrqCmdInT2*R_WRtr*R_PolePrRcp;


    /* ================================================== */
 }


/*=============================================================================
 * Function: ScaleCalc
 * Execution Level: Task 3
 * Execution Time: Untimed
 * Formal Inputs: None
 * Formal Outputs: None
 * * Global Inputs:
 *                 Adpt_R2
 *                 R_DelTm1Rcp
 *                 Est_R2
 *                 PARM(R_DchgRat)
 *                 PARM(Lsigma)
 *                 PARM(R_PolePairs)
 *                 PARM(S_R1)
 *                 PARM(R_LSigma)
 *                 PARM(R_LSigmaSim)
 *                 PARM(R_ModIndHys)
 *                 PARM(R_ModIndLev1)
 *                 PARM(R_SpdSimReq)
 *                 PARM(R_NsXStpAdr)
 *                 PARM(R_NsYStpAdr)
 *                 PARM(R_DcXStpAdr)
 *                 PARM(R_DcYStpAdr)
 *                 PARM(S_PllrStpAdr)
 *                 PARM(R_RPMSync)
 *                 PARM(R_IxVdcSpdgn)
 *                 V.R_SpdFbkT3
 *
 *
 * Global Outputs: R_DchgScl
 *                 Tau_Sigm_Rcp
 *                 K_Pole_Rcp
 *                 Ls_Sim
 *                 Ls_Sim_Rcp
 *                 Pole_Pair_Rcp
 *
 *                 R_NsXStep
 *                 R_NsYStep
 *                 R_DcXStep
 *                 R_DcYStep
 *                 S_PllregStep
 *
 *
 *
 *
 * Locals:         none
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function calculates logical control variables which are used to
 * determine the sense of switches throughout the control algorithm.
 *
 *===========================================================================*/
void            RotorScaleCalc(void)
{
    float   Temp1, Temp2;
    float   S_LmTmp;
    float   VFilGnTmp;

    /* On-line Xm calculator for changes due to saturation */
    S_VmCalc = S_VxFbkT3 - S_IyFbkT3 * S_Ls * PI2*PARM(S_Hz);
    S_ImCalc = R_IyFbkT3 * S_RRatioRcp - S_IyFbkT3;

    S_LmTmp = ABS(S_VmCalc/S_ImCalc)/(PI2*PARM(S_Hz));
    if (S_LmTmp > PARM(S_LmCalcMax)) S_LmTmp = PARM(S_LmCalcMax);
    if (S_LmTmp < PARM(S_LmCalcMin)) S_LmTmp = PARM(S_LmCalcMin);
    S_LmTmp =  PARM(S_LmCalcMeld) * S_LmTmp + (1.0 - PARM(S_LmCalcMeld)) * S_LmNom;

    /* Xm adaption active if enabled & not crowbar-ing (which saturates CT's) */
    if (!R_InnrRegEnb)
        S_LmCalc = S_LmNom;
    else if (!R_BrgCroBarCtl.CroBarAct)
    {
        if (R_Running)
            S_LmCalc += PARM(S_LmCalcWR) * DelTm3 * (S_LmTmp - S_LmCalc);
        else if (S_XmAdptEnb)
            S_LmCalc += PARM(S_LmCalcWI) * DelTm3 * (S_LmTmp - S_LmCalc);
    }

    /* End of On-line Xm calculator */

    /* Pass value to rest of algorithm.  S_LmCalcW is intended to be slow
     * enough that inter-task data passing is not needed. */
    S_Lm = S_LmCalc;

    S_LmRcp   = 1.0/S_Lm;
    S_LsLmSumRcp = 1.0/(S_Ls + S_Lm);
    S_XmRcp   = 1.0/(PI2 * PARM(S_Hz) * S_Lm);

    S_XmNomRcp   = 1.0/(PI2 * PARM(S_Hz) * S_LmNom);

    SRVRatio     = (S_Lm + S_Ls)*S_LmRcp * PARM(S_RRatio);
    SRVRatioRcp  = S_Lm*S_LsLmSumRcp*S_RRatioRcp;
    S_1PlusLsLm  = (1.0 + (S_Ls * S_LmRcp));

    /* calculate rotor resistance from starting (dc) values */
    R_RrMeas = R_VyStart/R_IyStart;


    /* resistance values for simulator */
    if (PARM(R_Winding))
        Imped_Scl = 0.3333333333;   // Delta Connect
    else
        Imped_Scl = 1.0;            // Wye Connect

    Temp2   = S_RRatioRcp * S_RRatioRcp;

    if ( PARM(R_DchgRat) > R_DelTm1Rcp)
    {
        Temp1 = R_DelTm1Rcp;
    }
    else
    {
        Temp1 = PARM(R_DchgRat);
    }

    R_DchgScl  = (1.0 - Temp1*R_DelTm1);




    if (R_Running)
    {
                                    /* Reduced speed range indication */

#ifdef use_steady_state_cur_lim_for_reduced_spd_range_calc
        /* Calculate stator and rotor Iy at under-excited limit at max speed */
        tmpf =  (PARM(R_RPMMaxOp) - PARM(SpdILim1)) * S_IlimSlope + PARM(S_ILim1);

        if      (tmpf > PARM(S_ILim2)) tmpf = PARM(S_ILim2);
        else if (tmpf < 0.0)           tmpf = 0.0;

        tmpf = (tmpf * tmpf) - (S_IxFilIyLim * S_IxFilIyLim);
        if (tmpf < 0.0 ) tmpf = 0.0;

        S_IyMaxSpd = sqrtf(tmpf);
        R_IyMaxSpd = (S_IyMagnetiz - S_IyMaxSpd) * PARM(S_RRatio);
#else
        R_IyMaxSpd = -R_IyLim;
        S_IyMaxSpd =  S_IyMagnetiz - R_IyMaxSpd * S_RRatioRcp;
#endif
        /* Calculate approximate rotor volts at max speed */
        R_FreqMaxSpd = S_PllWT3 - PARM(R_RPMMaxOp) * RPM_TO_RAD * PARM(R_PolePairs);

        R_VyMaxSpd = R_FreqMaxSpd * (PARM(R_LSigma) * R_IxCmdT3) + R_R2 * R_IyMaxSpd;

        R_VxMaxSpd = -R_FreqMaxSpd * ( (-S_VxFbkT3/S_PllWT3 - S_IyMaxSpd * S_Ls) * S_RRatioRcp + R_IyMaxSpd * R_Lr )
                     + R_R2 * R_IxCmdT3;

        Temp1 = sqrtf(R_VxMaxSpd*R_VxMaxSpd + R_VyMaxSpd*R_VyMaxSpd);

        VFilGnTmp = MIN(1.0, PARM(R_WVMaxSpd) * DelTm3);

        R_VMaxSpd += VFilGnTmp * (Temp1 - R_VMaxSpd);


#ifdef use_steady_state_cur_lim_for_reduced_spd_range_calc
        /* Calculate stator and rotor Iy at under-excited limit at min speed */
        tmpf =  (PARM(R_RPMMinOp) - PARM(SpdILim1)) * S_IlimSlope + PARM(S_ILim1);

        if      (tmpf > PARM(S_ILim2)) tmpf = PARM(S_ILim2);
        else if (tmpf < 0.0)           tmpf = 0.0;

        tmpf = (tmpf * tmpf) - (S_IxFilIyLim * S_IxFilIyLim);
        if (tmpf < 0.0 ) tmpf = 0.0;

        S_IyMinSpd = sqrtf(tmpf);
        R_IyMinSpd = (S_IyMagnetiz - S_IyMinSpd) * PARM(S_RRatio);
#else
        R_IyMinSpd = -R_IyLim;
        S_IyMinSpd = S_IyMagnetiz - R_IyMinSpd * S_RRatioRcp;
#endif

        /* Calculate approximate rotor volts at min speed */
        R_FreqMinSpd = S_PllWT3 - PARM(R_RPMMinOp) * RPM_TO_RAD * PARM(R_PolePairs);

        R_VyMinSpd = R_FreqMinSpd * (PARM(R_LSigma) * R_IxCmdT3) + R_R2 * R_IyMinSpd;

        R_VxMinSpd = -R_FreqMinSpd * ( (-S_VxFbkT3/S_PllWT3 - S_IyMinSpd * S_Ls) * S_RRatioRcp + R_IyMinSpd * R_Lr )
                     + R_R2 * R_IxCmdT3;

        Temp1 = sqrtf(R_VxMinSpd*R_VxMinSpd + R_VyMinSpd*R_VyMinSpd);

        R_VMinSpd += VFilGnTmp * (Temp1 - R_VMinSpd);

        /* Set bit to indicate speed range is reduced due to possible high rotor volts */
        if      (R_VMaxSpd > PARM(R_VMaxOnLvl))  R_VMaxSpdHi = True;
        else if (R_VMaxSpd < PARM(R_VMaxOffLvl)) R_VMaxSpdHi = False;

        if      (R_VMinSpd > PARM(R_VMaxOnLvl))  R_VMinSpdHi = True;
        else if (R_VMinSpd < PARM(R_VMaxOffLvl)) R_VMinSpdHi = False;

        R_SpdRngRed = R_VMaxSpdHi || R_VMinSpdHi;
    }
    else
    {
        R_VMaxSpd   = 0.0;
        R_VMinSpd   = 0.0;
        R_VMaxSpdHi = False;
        R_VMinSpdHi = False;
        R_SpdRngRed = False;
    }


    R_NsXStep   = gVariable.ReadAsFloat(PARM(R_NsXStpAdr));
    R_NsYStep   = gVariable.ReadAsFloat(PARM(R_NsYStpAdr));

    R_DcXStep  = gVariable.ReadAsFloat(PARM(R_DcXStpAdr));
    R_DcYStep  = gVariable.ReadAsFloat(PARM(R_DcYStpAdr));

    S_PllregStep = gVariable.ReadAsFloat(PARM(S_PllrStpAdr));



    ////////////////////////////////////////////////////
    // Calculate DC Link Suppression Control -- TRQ_020
    ///////////////////////////////////////////////////

    Temp1 = MAX( (R_SpdFbkAvg - PARM(R_RPMSync)), 0.0);
    R_IxVdcGnDyn = Temp1 * PARM(R_IxVdcSpdGn);

    ///////////////////////////////////////////////////////
    // Slow filtered speed for pll phase shift -- APP_150
    //////////////////////////////////////////////////////
    SpdPwrPllFil = SpdPwrPllFil + SpdPllFilGn * (R_SpdT3 - SpdPwrPllFil);

    
    /////////////////////////////////////////////////////////////////////
    // Dynamic rotor current regulator Iy proportional gain  -- TRQ_034
    ////////////////////////////////////////////////////////////////////
    
    R_IyrgPSpdIn = R_SpdFbkAvg; 

    if ( PARM(SeriesCmpEnb) )
    {
       if (R_IyrgPSpdIn < PARM(R_IyregPSpd0) )
       {
          R_IyregPRtDy = PARM(R_IyregPRto0);
       }
       else if (R_IyrgPSpdIn > PARM(R_IyregPSpd) )
       {
          R_IyregPRtDy = PARM(R_IyregPRtio);
       }
       else
       {
          R_IyregPRtDy = PARM(R_IyregPRto0) + R_IyregPSlp * ( R_IyrgPSpdIn - PARM(R_IyregPSpd0) );
       }
    }
    else
    {
       R_IyregPRtDy = PARM(R_IyregPRtio);
    }

    R_IyregPGnDy = R_IyregPRtDy * PARM(R_IyregPrpGn);

}


/*=============================================================================
 * Function:        RosettaStone
 * Execution Level: Task 3
 * Execution Time:  Untimed
 * Formal Inputs:   None
 * Formal Outputs:  None
 * Global Inputs:   R_VXover
 *                  PARM(S_Amps)
 *                  PARM(Hz)
 *                  PARM(Power)
 *                  PARM(RPM)
 *                  PARM(R_MotorNT)
 *                  PARM(User_Net)
 *                  PARM(User_Rmp)
 * Global Outputs:  R_VoltsRat
 *                  R_AmpsRat
 *                  R_HzRat
 *                  R_PowerRat
 *                  R_MotorNTRat
 *                  User_Net_Rat
 *                  User_Rmp_Rat
 * Return Value:    None
 * Function Calls:  None
 *
 * Abstract:
 *    This function creates variable copies of the Rosetta Stone parameters.
 *
 *===========================================================================*/
void            RotorRosettaStone(void)
{
   // moved various _Rat settings to A0Rule
}

