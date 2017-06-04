///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Initialize and process Line H/W feedbacks
//
// DESCRIPTION:
//      This file provides protection level product code that works on
//      primary feedbacks used by the control.
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "y2Fdbk.h"
#include "b0DbCelTst.h"
#include "b0BrgThrm5p.h"


// Constants
//----------
#define  EX_CNT_2_PCT   100/(0.25*CpuCps)  // Exec count to per cent conversion


// Externals
//----------


extern float        CpuCps         ;
extern float        DelTm2         ;
extern float        DelTm2Rcp      ;
extern float        DelTm3         ;
extern float        DelTm3Rcp      ;
extern float        GmPllWT2       ;
extern float        L_AdjTm1Rcp    ;
extern float        L_DelTm1       ;
extern float        L_IabcGndGn    ;
extern float        L_IaT2         ;
extern float        L_IbT2         ;
extern float        L_IcT2         ;
extern float        L_IocThrs      ;
extern float        LBrgVMtchDt1   ;
extern unsigned     GmPllHot       ;
extern unsigned     InitDone       ;
extern unsigned     L_InnrRegEnb   ;
extern unsigned     L_IocFbkEnb    ;
extern unsigned     L_IOffsEnb     ;
extern float        T1CntT2        ;
extern int          T1ExecCnt      ;
extern unsigned     T2DoneOnceT1   ;
extern int          T2ExecCnt      ;
extern int          T3ExecCnt      ;


extern float        L_VdcBrg ;
extern float        L_VabBrg ;
extern float        L_VbcBrg ;
extern float        L_IaBrg  ;
extern float        L_IbBrg  ;
extern float        L_IcBrg  ;

extern float        R_VdbBrg;
extern float        L_VdbBrg;
extern float        L_DelTm1Rcp;
extern float        L_IxFbk    ;
extern float        L_IyFbk    ;
extern float        L_VxFbk    ;
extern float        L_VyFbk    ;

extern unsigned     PwrDnFbkDsb;

extern cDbCelTst        L_DbCelTst;
extern cBrgThrm5pModel  L_BrgThrm5pModel;


// Types and Classes
//------------------


// Diagnostics
//------------


// Parameters
//-----------
CREATE_PARM(LBrgVMtch,          float);     // L bridge for voltage matching
CREATE_PARM(L_FbkFilGn,         float);     // Line converter feedback filter gain
CREATE_PARM(L_IDiDtAttn,        float);     // Line converter current feedback di/dt attenuation when peaky lp used.
CREATE_PARM(L_IregFbkTyp,       unsigned);  // Feedback type
CREATE_PARM(L_VdcUVLvl,         float);     // DC link undervoltage trip level
CREATE_PARM(L_VdcOVLvl,         float);     // DC link overvoltage trip level
CREATE_PARM(L_IRAFEnbTm,        float);     // dc ireg RAF enable timer

// Variables
//----------
CREATE_PUBVAR(L_VabT2     ,    float);    // T2 version of L_Vab
CREATE_PUBVAR(L_VbcT2     ,    float);    // T2 version of L_Vbc

CREATE_PUBVAR(ExecLoad    ,    float);    // Execution loading in percent
CREATE_PUBVAR(ExecLoadPk  ,    float);    // Execution loading peak in percent
CREATE_PUBVAR(ExecLoadT1  ,    float);    // Execution loading of T1 in percent
CREATE_PUBVAR(ExecLoadT1Pk,    float);    // Execution loading peak of T1 in percent
CREATE_PUBVAR(ExecLoadT2  ,    float);    // Execution loading of T2 in percent
CREATE_PUBVAR(ExecLoadT2Pk,    float);    // Execution loading peak of T2 in percent
CREATE_PUBVAR(ExecLoadT3  ,    float);    // Execution loading of T3 in percent
CREATE_PUBVAR(ExecLoadT3Pk,    float);    // Execution loading peak of T3 in percent
CREATE_PUBVAR(GmPllWRAF   ,    float);    // Pll freq  rolling average filter output
CREATE_PUBVAR(L_BrgLsTstT1,    float);
CREATE_PUBVAR(L_BrgPwrTsT1,    float);
CREATE_PUBVAR(L_BusPwrRcp ,    float);
CREATE_PUBVAR(L_BusPwrTAcc,    float);
CREATE_PUBVAR(L_BusPwrTst ,    float);
CREATE_PUBVAR(L_DcBusPwrTs,    float);
CREATE_PUBVAR(L_DcIxFbk   ,    float);    // Line DC current regulator alpha rolling average filter output
CREATE_PUBVAR(L_DcIyFbk   ,    float);    // Line DC current regulator beta rolling average filter output
CREATE_PUBVAR(L_DPwrTstT1 ,    float);
CREATE_PUBVAR(L_Ia        ,    float);    // Units = amps
CREATE_PUBVAR(L_Iab       ,    float);
CREATE_PUBVAR(L_IabZ1     ,    float);
CREATE_PUBVAR(L_IaFil     ,    float);
CREATE_PUBVAR(L_IAlpha    ,    float);    // Line DC current regulator alpha current
CREATE_PUBVAR(L_IaSqr     ,    float);    // Phase A current squared
CREATE_PUBVAR(L_IaSqrDtAcc,    float);
CREATE_PUBVAR(L_IaTstRms  ,    float);
CREATE_PUBVAR(L_IaTstRmsSq,    float);
CREATE_PUBVAR(L_Ib        ,    float);    // Units = amps
CREATE_PUBVAR(L_Ibc       ,    float);
CREATE_PUBVAR(L_IbcZ1     ,    float);
CREATE_PUBVAR(L_IBeta     ,    float);    // Line DC current regulator beta current
CREATE_PUBVAR(L_IbFil     ,    float);
CREATE_PUBVAR(L_IbSqr     ,    float);    // Phase B current squared
CREATE_PUBVAR(L_IbSqrDtAcc,    float);
CREATE_PUBVAR(L_IbTstRms  ,    float);
CREATE_PUBVAR(L_IbTstRmsSq,    float);
CREATE_PUBVAR(L_Ic        ,    float);    // Units = amps
CREATE_PUBVAR(L_IcFil     ,    float);
CREATE_PUBVAR(L_IcSqr     ,    float);    // Phase C current squared
CREATE_PUBVAR(L_IcSqrDtAcc,    float);
CREATE_PUBVAR(L_IcTstRms  ,    float);
CREATE_PUBVAR(L_IcTstRmsSq,    float);
CREATE_PUBVAR(L_IRAFEnbTmr,    float);    // RAF enable timer...zeroed in precondition
CREATE_PUBVAR(L_IRAFIndx  ,    int);      // RAF index
CREATE_PUBVAR(L_TrPwrTstT1,    float);
CREATE_PUBVAR(L_Vab       ,    float);    // Units = volts
CREATE_PUBVAR(L_VabAvg    ,    float);
CREATE_PUBVAR(L_VabFil    ,    float);
CREATE_PUBVAR(L_VabLineT1 ,    float);
CREATE_PUBVAR(L_VabZ1     ,    float);
CREATE_PUBVAR(L_Vbc       ,    float);    // Units = volts
CREATE_PUBVAR(L_VbcAvg    ,    float);
CREATE_PUBVAR(L_VbcFil    ,    float);
CREATE_PUBVAR(L_VbcLineT1 ,    float);
CREATE_PUBVAR(L_VbcZ1     ,    float);
CREATE_PUBVAR(L_VcaT2     ,    float);    // Units = volts
CREATE_PUBVAR(L_VdbTst    ,    float);
CREATE_PUBVAR(L_VdbTstAcc ,    float);
CREATE_PUBVAR(L_Vdc       ,    float);    // processed dc link voltage in volts
CREATE_PUBVAR(L_VdcComp   ,    float);    //
CREATE_PUBVAR(L_VdcCompRcp,    float);    //
CREATE_PUBVAR(L_VdcFbk    ,    float);    // voltage feedback for regulators
CREATE_PUBVAR(L_VdcFbkRcp ,    float);    // dc link voltage reciprocal
CREATE_PUBVAR(T1CntT2Z1   ,    float);    // Execution loading in percent from T2*/

CREATE_PUBVAR(L_IRAFCnt   ,    int);
CREATE_PUBVAR(L_IMesCntRcp,    float);
CREATE_PUBVAR(L_IRAFCntFlt,    float);
CREATE_PUBVAR(L_IRAFGnDiff,    float);
CREATE_PUBVAR(L_IRAFGnInnr,    float);
CREATE_PUBVAR(L_IRAFGnOutr,    float);
CREATE_PUBVAR(IRAFIndx1st,     int);

CREATE_PUBVAR(L_FpgaFltFb2,    unsigned);
CREATE_PUBVAR(L_IMeasCntr ,    unsigned);
CREATE_PUBVAR(L_IMeasCntRq,    unsigned);
CREATE_PUBVAR(L_IMeasRq   ,    unsigned);
CREATE_PUBVAR(L_IMeasStat ,    unsigned);
CREATE_PUBVAR(L_ITstSqrtRq,    unsigned);
CREATE_PUBVAR(L_ITstSqrtSt,    unsigned);

CREATE_PUBVAR(L_Van       ,    float);
CREATE_PUBVAR(L_Vbn       ,    float);
CREATE_PUBVAR(L_Vcn       ,    float);

// Diagnostics
//--------------------
CREATE_DIAG(L_VdcOverV);            // Line converter bridge over DC link voltage trip
CREATE_DIAG(L_VdcUnderV);           // Line converter bridge under DC link voltage trip
CREATE_DIAG(L_InstOverI);

// Unpublished Variables
//----------------------
float       L_RAFAlphStk[22];           // Line RAF array for Dc Alpha current shold be (1/(50hz)*(5712/6) + 2, add 1 for cushion
float       L_RAFBetaStk[22];           // Line RAF array for Dc Alpha current shold be (1/(50hz)*(5712/6) + 2, add 1 for cushion
float       GmPllWRAFStk[22];           // Pll freq  RAF array for pll freq average


// Data Passing
//--------------
DATA_PASS(L_DbCelTst.DbCtImeasReq, L_IMeasRq, T2_T1, UNFILTERED);

// Local Prototypes (to resolve forward references)
//-------------------------------------------------
extern void  InitAeadInOuts(void);


// Class Instances
//----------------



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculates the primary feedbacks used by the control
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void BILPriFbks1( void )
{
    //   Calculate DC link voltage and perform overvoltage check
    //   The L_Vdc calculation and check must executed before any other
    //   feedback code.  The ASIC DC overvoltage fault mask bit must
    //   be configured during initialization for the bridge to power
    //   off correctly in the event of a DC overvoltage

    L_Vdc        = L_VdcBrg;      // if the brg card is an AEBI
    L_VdcFbk     = L_Vdc;
    L_VdcComp    = L_VdcFbk;

    L_VdcFbkRcp =  NoNANLim( 1.0F/L_VdcFbk, 100.0e6, -100.0e6 );

    L_VdcCompRcp = L_VdcFbkRcp;

    if ( (L_VdcFbk >= PARM(L_VdcOVLvl))  && !PwrDnFbkDsb  )
    {
        PUSH_DIAG(L_VdcOverV);
    }

    //Calculate dynamic brake resistor voltage voltage
    //   L_Vdb       = L_VdcBrg;       if the brg card is an AEDB

    // Calculate line-line voltage feedbacks
    L_Vab       = L_VabBrg;
    L_Vbc       = L_VbcBrg;

    L_Van   = ( 2.0F/3.0F) * L_Vab + ( 1.0F/3.0F) * L_Vbc;
    L_Vbn   = (-1.0F/3.0F) * L_Vab + ( 1.0F/3.0F) * L_Vbc;
    L_Vcn   = -L_Van - L_Vbn;

    // Calculate phase current feedbacks

    L_Ia   = L_IaBrg;
    L_Ib   = L_IbBrg;
    L_Ic   = L_IcBrg;

    // 0 = "Original"
    // 1 = "Ireg LP filter:
    // 2 = "Feedback LP filter;
    // 3 = "Both"
    if (PARM(L_IregFbkTyp) >  1 )
    {
        L_IaFil  = L_IaFil   + PARM(L_FbkFilGn)*(L_Ia - L_IaFil);

        L_IbFil  = L_IbFil   + PARM(L_FbkFilGn)*(L_Ib - L_IbFil);

        L_IcFil  = L_IcFil   + PARM(L_FbkFilGn)*(L_Ic - L_IcFil);

        L_VabFil = L_VabFil  + PARM(L_FbkFilGn)*(L_Vab - L_VabFil);

        L_VbcFil = L_VbcFil  + PARM(L_FbkFilGn)*(L_Vbc - L_VbcFil);
    }

    else
    {
        L_IaFil= L_Ia;
        L_IbFil= L_Ib;
        L_IcFil= L_Ic;

        L_VabFil= L_Vab;
        L_VbcFil= L_Vbc;
    }

    // instantaneous overcurrent fault check
    if ( L_IocFbkEnb )
    {
        if ( (abs(L_Ia) >= L_IocThrs) || (abs(L_Ib) >= L_IocThrs) ||
                (abs(L_Ic) >= L_IocThrs) )
        {
            PUSH_DIAG(L_InstOverI);
        }
    }

    L_Iab = L_Ia - L_Ib;
    L_Ibc = L_Ib - L_Ic;
    L_VabAvg = 0.5*(L_Vab + L_VabZ1);
    L_VbcAvg = 0.5*(L_Vbc + L_VbcZ1);

    L_VabLineT1 = L_VabAvg - ( LBrgVMtchDt1 * PARM(L_IDiDtAttn) * (L_Iab - L_IabZ1)  );
    L_VbcLineT1 = L_VbcAvg - ( LBrgVMtchDt1 * PARM(L_IDiDtAttn) * (L_Ibc - L_IbcZ1)  );

    L_IabZ1 = L_Iab;
    L_IbcZ1 = L_Ibc;
    L_VabZ1 = L_Vab;
    L_VbcZ1 = L_Vbc;

    L_IaSqr = L_Ia*L_Ia;
    L_IbSqr = L_Ib*L_Ib;
    L_IcSqr = L_Ic*L_Ic;

    ExecLoadT1 = EX_CNT_2_PCT*(T1ExecCnt*L_DelTm1Rcp);
    if ( ExecLoadT1 > ExecLoadT1Pk)
    {
        ExecLoadT1Pk = ExecLoadT1;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculates the values of variables used in the offline feedback
//      offset algorithm and checks for offset fault conditions.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  BILPriFbks2( void )
{
     int     i;
     float   L_AlphaNewest, L_BetaNewest;
     float   L_AlphaOldest, L_BetaOldest;
     float   GmPllWNewest,  GmPllWOldest;
     float   L_DcIxFbkTemp;
     float   L_DcIyFbkTemp;
     float   GmPllWRAFTemp;


    ///////////////////////////////////////////////////////////
    // Form L_VcaT2
    ///////////////////////////////////////////////////////////

    L_VcaT2 = - (L_VabT2 + L_VbcT2);


    ///////////////////////////////////////////////////////////////////////////////////
    // Line RAF Line DC current regulator and pll frequency
    ///////////////////////////////////////////////////////////////////////////////////

     /* Form converter current into Alpha-Beta */
     L_IAlpha   = (2./3.) * L_IaT2 - (1./3.) * L_IbT2 - (1./3.) * L_IcT2; //really, Ix here is alpha and Iy is beta
     L_IBeta    = (SQRT3_RCP) * L_IbT2 - (SQRT3_RCP) * L_IcT2;


     ///////////////////////////////////////////////////////////////////////////////
     // RAF for DC current regulator ----6.075usec
     // This filter is able to optain a dc offset over a fundamental line
     // cycle even though a even multiple of task1 times cannot exactly hit
     // the desired frequency.  This is accomoplished by having a gain for
     // each member that is 1/n, called GnInnr here, where n is the float representing
     // L_IntRate/L_LineFrq(hz).  A truncated version of that same number will
     // be referred to here as nTrunc. The total number of member in the array is
     // the truncated n, plus 2, referred to as N here. This RAF has two more
     // members more than the truncated number that would normall be in a full
     // ac line cycle.   The extra two members are attributed to the last member
     // sampled and the oldest member in the array.  A different gain is applied
     // to these two members. The gain applied to those two members is
     // (1.0 - nTrunc*(1/n))/2.0, called GnOuter here.  The sum of all the applied
     // gains is 1.0:   nTrunc*GnInnr + 2*GnOuter = 1.0, where NTrunc = N-2.
     //
     // The first attempt to implement this filter was to have each itteration of
     // the averaging loop to look for the 1st and last member of the array and
     // apply the GnInnr to each of those two elements.  That took 18.0usec.
     //
     // The following equivalent method takes only 6.0usec.  The inner gain is first
     // applied to all members, then a subtraction is made to correct for adding too
     // much to the newest and oldest member.  The error that must be subtracted
     // is I(oldest)*GnInnr + I(newest)*GnInnr - I(oldest)*GnOuter - I(newest)*GnOuter
     //  = I(oldest)*(GnInnr-GnOuter) + I(newest)*(GnInnr-GnOuter)
     //  = (GnInnr-GnOuter)*[I(oldest) + I(newer)]
     //  = L_IRAFGnDiff * I(oldest) + I(newer)], where L_IRAFGnDiff = (GnInnr-GnOouter)
     //
     //////////////////////////////////////////////////////////////////////////////////

     L_IRAFEnbTmr = L_IRAFEnbTmr + DelTm2;//zeroed in precondition


     //Start timing here

     L_IRAFIndx++;
     if      (L_IRAFIndx >= L_IRAFCnt)
     {
        L_IRAFIndx = 0;
     }

     IRAFIndx1st = L_IRAFIndx + 1;  //the oldest member in the array
     if (IRAFIndx1st >= L_IRAFCnt)//if index is at last member in array, oldest memeber is element 0
     {
        IRAFIndx1st = 0;
     }
     L_AlphaOldest  = L_RAFAlphStk[IRAFIndx1st];
     L_BetaOldest   = L_RAFBetaStk[IRAFIndx1st];
     GmPllWOldest   = GmPllWRAFStk[IRAFIndx1st];
     //Fill the arrays and zero the average
     L_AlphaNewest  = L_RAFAlphStk[L_IRAFIndx] = L_IAlpha;
     L_BetaNewest   = L_RAFBetaStk[L_IRAFIndx] = L_IBeta;
     GmPllWNewest   = GmPllWRAFStk[L_IRAFIndx] = GmPllWT2;

     L_DcIxFbkTemp  = 0.0;
     L_DcIyFbkTemp  = 0.0;
     GmPllWRAFTemp  = 0.0;

     if ( !GmPllHot)
     {

        //Add in the rest of the members
        for (i = 0; i < L_IRAFCnt; i++)
        {
            L_DcIxFbkTemp   += L_IRAFGnInnr*L_RAFAlphStk[i];
            L_DcIyFbkTemp   += L_IRAFGnInnr*L_RAFBetaStk[i];
        }
        //total sum of gains is 1.0, so no need to divide.

        //Adjust each componet for 1st and last member gains
        L_DcIxFbkTemp  -= L_IRAFGnDiff*(L_AlphaNewest + L_AlphaOldest);
        L_DcIyFbkTemp  -= L_IRAFGnDiff*(L_BetaNewest  + L_BetaOldest);
        GmPllWRAFTemp   = PARM(GmPllWPrec);

     }
     else //Pll is hot
     {

        //Add in the rest of the members
        for (i = 0; i < L_IRAFCnt; i++)
        {
            L_DcIxFbkTemp   += L_IRAFGnInnr*L_RAFAlphStk[i];
            L_DcIyFbkTemp   += L_IRAFGnInnr*L_RAFBetaStk[i];
            GmPllWRAFTemp   += L_IRAFGnInnr*GmPllWRAFStk[i];
        }
        //total sum of gains is 1.0, so no need to divide.

        //Adjust each componet for 1st and last member gains
        L_DcIxFbkTemp  -= L_IRAFGnDiff*(L_AlphaNewest + L_AlphaOldest);
        L_DcIyFbkTemp  -= L_IRAFGnDiff*(L_BetaNewest  + L_BetaOldest);
        GmPllWRAFTemp  -= L_IRAFGnDiff*(GmPllWNewest  + GmPllWOldest);

     }



     if (L_IRAFEnbTmr < PARM(L_IRAFEnbTm) )
     {
        L_DcIxFbkTemp = 0.0;
        L_DcIyFbkTemp = 0.0;
     }

     L_DcIxFbk = L_DcIxFbkTemp;
     L_DcIyFbk = L_DcIyFbkTemp;
     GmPllWRAF = GmPllWRAFTemp;



    ///////////////////////////////////////////////////////////
    // Execution Loading
    ///////////////////////////////////////////////////////////
    ExecLoad = EX_CNT_2_PCT*(T1CntT2Z1*L_DelTm1Rcp + T2ExecCnt*DelTm2Rcp
                                                       + T3ExecCnt*DelTm3Rcp);
    if ( ExecLoad > ExecLoadPk)
    {
         ExecLoadPk = ExecLoad;
    }
    T1CntT2Z1 = T1CntT2;


    ExecLoadT2 = EX_CNT_2_PCT*(T2ExecCnt*DelTm2Rcp);
    if ( ExecLoadT2 > ExecLoadT2Pk)
    {
         ExecLoadT2Pk = ExecLoadT2;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function does the task 1 calculations for the calculation of
//      the line conveter rmse phase currents.  It uses the 3 squared
//      line currents as inputs and outputs 3 rms currents that are averaged
//      over a specified number of task one's.  This number is an input to
//      this function and is inteded to be a value that will average the
//      currents over a multiple number of line cycles.
//
//      The sqared currents are dt comped, then averaged over the specified
//      period.  Those values are then handed to task3 so that the square
//      root can be taken there.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineRmsPhaseCurrentCalcT1( void )
{
    // Calculate total line bridge power loss per device
    L_DPwrTstT1  = L_BrgThrm5pModel.BtmDpwrLoss;
    L_TrPwrTstT1 = L_BrgThrm5pModel.BtmTrPwrLoss;

    if ( L_IMeasRq )
    {

        //Calculate DB bus power for the period
        if ( PARM(TestMode) == PUMP_BACK )
        {
          // factory tests: weak line (small kva xfmr) causes distortion, makes power calc less than actual
          L_BrgPwrTsT1  = 1.5F*((L_VxFbk*L_IxFbk) + (L_VyFbk*L_IyFbk)) * PARM(DbRTstPbPK);
        }
        else
        {
          L_BrgPwrTsT1  = 1.5F*((L_VxFbk*L_IxFbk) + (L_VyFbk*L_IyFbk));
        }

        L_BrgLsTstT1  = L_DPwrTstT1;// + L_TrPwrTstT1;
        L_DcBusPwrTs  = L_BrgPwrTsT1 - L_BrgLsTstT1;
#ifdef WT_PMGT
        // Add in power from generator. (Gen cur dmod only active if gen conv enabled, so use abc values)
        L_DcBusPwrTs  += 0.333333F * ( G_Vab * (G_Ia-G_Ib) + G_Vbc * (G_Ib-G_Ic) - (G_Vab+G_Vbc) * (G_Ic-G_Ia) );
#endif

        if ( L_IMeasCntr < L_IMeasCntRq)
        {
           L_IaSqrDtAcc  = L_IaSqrDtAcc + (L_DelTm1 * L_IaSqr);
           L_IbSqrDtAcc  = L_IbSqrDtAcc + (L_DelTm1 * L_IbSqr);
           L_IcSqrDtAcc  = L_IcSqrDtAcc + (L_DelTm1 * L_IcSqr);
           L_VdbTstAcc   = L_VdbTstAcc  + (L_DelTm1 * L_VdbBrg);
           L_BusPwrTAcc  = L_BusPwrTAcc + (L_DelTm1 * L_DcBusPwrTs);
           L_IMeasCntr   = L_IMeasCntr  + 1;
        }
        else if ( !L_IMeasStat )
        {
             if ( !L_ITstSqrtRq)
             {
                L_IaTstRmsSq = L_IaSqrDtAcc  * L_IMesCntRcp;
                L_IbTstRmsSq = L_IbSqrDtAcc  * L_IMesCntRcp;
                L_IcTstRmsSq = L_IcSqrDtAcc  * L_IMesCntRcp;
                L_VdbTst     = L_VdbTstAcc   * L_IMesCntRcp;
                L_BusPwrTst  = L_BusPwrTAcc  * L_IMesCntRcp;
             }

             L_ITstSqrtRq = true; //Request LineRmsPhaseCurrentCalcT1 to do square root calculation
             if ( L_ITstSqrtSt )
             {
                 L_ITstSqrtRq = false;
                 L_IMeasStat  = true;
             }
        }

    }
    else
    {
        L_IaSqrDtAcc = 0.0F;
        L_IbSqrDtAcc = 0.0F;
        L_IcSqrDtAcc = 0.0F;
        L_VdbTstAcc  = 0.0F;
        L_BusPwrTAcc = 0.0F;


        L_IMeasCntr  = 0;
        L_ITstSqrtRq = false;
        L_IMeasStat  = false;

    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function calculates the values of variables used in the
//      offline feedback offset algorithm and checks for offset fault
//      conditions.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  BILPriFbks3( void )
{

    ExecLoadT3 = EX_CNT_2_PCT*(T3ExecCnt*DelTm3Rcp);
    if ( ExecLoadT3 > ExecLoadT3Pk)
    {
         ExecLoadT3Pk = ExecLoadT3;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function calculates variables used for scaling in Task 1
//      feedback calculations.
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  BILPriFbksBgnd( void )
{


    // Determine voltage scalings
    L_IabcGndGn  = PARM(L_WIabcGnd )* L_DelTm1;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs initialization operations on behalf of
//      feedbacks.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void InitFbks()
{
        // VCO and shunt scalings

    BILPriFbks3();

    InitAeadInOuts();  // Initialize Relay output buffer

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function does the task 3 calculations for the calculation of
//      the line conveter rms phase current calculations.  These calculations
//      include square root functions and recpriocal calcs that would be very
//      resources-consuming in task1 or task2.
//
//      The sqare root calculations are done only on request of a flag.
//      Another flag is set when this is completed.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  LineRmsPhaseCurrentCalcT3( void )
{
    //L_IMesCntRcp = 1.0/(L_IMeasCntRq*L_DelTm1);  // HL: move to the y2Rule.cpp

    if ( L_ITstSqrtRq)
    {
        L_IaTstRms   = sqrtf(L_IaTstRmsSq);
        L_IbTstRms   = sqrtf(L_IbTstRmsSq);
        L_IcTstRms   = sqrtf(L_IcTstRmsSq);

        L_BusPwrRcp =  NoNANLim( 1.0F/L_BusPwrTst, 1.0e6F, -1.0e6F );

        L_ITstSqrtSt = true;
    }
    else
    {
        L_ITstSqrtSt = false;
    }

    return;
}
