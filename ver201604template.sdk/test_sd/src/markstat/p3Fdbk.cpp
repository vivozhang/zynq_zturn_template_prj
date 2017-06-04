///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Initialization and processing of analog hardware feedbacks
//
// DESCRIPTION
//      This file contains functions for initialization, filtering, state
//      mirroring, and mapping of analog hardware feedbacks.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      List Visual SourceSafe locations of pertinent design documents.
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
#include "p3Fdbk.h"


// Constants
//----------

#define  FIL_TAU_N     5                // Feedback offset constant


// Externals
//----------
extern FaultIsActiveTyp FaultIsActive;
extern float            DelTm2;
extern float            GmVanT2;
extern float            GmVbnT2;
extern float            GmVcnT2;
extern float            GmVMgFstDnGn;
extern float            GmVMgFstUpGn;
extern float            S_IaSqrT2;
extern float            S_IaSqrT3;
extern float            S_IbSqrT2;
extern float            S_IbSqrT3;
extern float            S_IcSqrT2;
extern float            S_IcSqrT3;
extern float            S_VabT2;
extern float            S_VbcT2;
extern float            S_Van;
extern float            S_Vbn;
extern float            S_Vcn;

extern unsigned         R_MctlCmd_PackedBits;
extern SysTm_Typ        SysTm2;


extern float            T1T2FbkScale;
extern int              S_VOffsAccNT1;
extern int              S_VOffsFilNT1;
extern float            R_DelTm1;
extern float            R_DelTm1Rcp;
extern float            S_MaxCurOut;
extern unsigned         BrgEnbSim;
extern unsigned         InitDone;
extern unsigned         S_K1ClsCmd;
extern unsigned         L_IOffsEnb;
extern unsigned         L_Running;
extern unsigned         LnBrgOff1Dt;
extern unsigned         OpnPhsAct;
extern unsigned         MvSwGearTrpReq;
extern unsigned         R_CroBarFltT2;
extern unsigned         RtBrgOff1Dt;
extern unsigned         SimMode;
extern unsigned         T2DoneOnceT1;
extern unsigned         R_SyncIOCEnb;
extern unsigned         S_VMaxHiDet;



// Types and Classes
//------------------




// Parameters
//-----------
CREATE_PARM( S_TocMargin,   float );   // Timed over-current margin for stator (Pu)
CREATE_PARM( S_TocTau,      float );
CREATE_PARM( S_TocThrs,     float );
CREATE_PARM( S_TocAlmOn,    float );
CREATE_PARM( S_TocAlmOff,   float );
// stator feedbacks
CREATE_PARM(S_VllFbkLag,        float);     // excess phase shift due to filtering on AEAA
CREATE_PARM(S_VllOffsLim,       float);
CREATE_PARM(S_IOffsPct,         float);
CREATE_PARM(S_WIOffsFst,        float);
CREATE_PARM(S_WIOffsSlw,        float);
CREATE_PARM(S_WVOffsFst,        float);
CREATE_PARM(S_VOffsFst,         float);
CREATE_PARM(S_WVOffsFil,        float);
CREATE_PARM(S_VOffsRnTm,        float);
CREATE_PARM(S_IaOffs,           float);
CREATE_PARM(S_IbOffs,           float);
CREATE_PARM(S_IcOffs,           float);
CREATE_PARM(S_IOffsThr,         float);
CREATE_PARM(S_VabOffs,          float);
CREATE_PARM(S_VbcOffs,          float);
CREATE_PARM(S_VOffsThr,         float);
CREATE_PARM(S_VOffsVlTm,        float);
CREATE_PARM(S_IaVcoScl,         float);
CREATE_PARM(S_IbVcoScl,         float);
CREATE_PARM(S_IcVcoScl,         float);
CREATE_PARM(S_VabVcoScl,        float);
CREATE_PARM(S_VbcVcoScl,        float);
CREATE_PARM(S_WIabcGnd,         float);
CREATE_PARM(S_IOCLvl,           float);     // stator IOC threshold
CREATE_PARM(S_WIOC,             float);     // stator IOC filter omega

// Diagnostics
//------------
CREATE_DIAG(S_TocFlt);          // Stator Time over current trip
CREATE_DIAG(S_TocAlm);          // Stator Time over current alarm
CREATE_DIAG(S_IaOffsFlt );
CREATE_DIAG(S_IbOffsFlt );
CREATE_DIAG(S_IcOffsFlt );
CREATE_DIAG(S_VabOffsFlt);
CREATE_DIAG(S_VbcOffsFlt);
CREATE_DIAG(S_IOCFlt);

// Variables
//----------
SysTm_Typ  S_VOffsTm;

CREATE_PUBVAR(GmIa,         float);     // Current feedback signals
CREATE_PUBVAR(GmIb,         float);     // Units = amps
CREATE_PUBVAR(GmIc,         float);
CREATE_PUBVAR(GmVab,        float);
CREATE_PUBVAR(GmVbc,        float);
CREATE_PUBVAR(GmVca,        float);

CREATE_PUBVAR(GmVan,        float);     // Voltage feedback signals
CREATE_PUBVAR(GmVbn,        float);     // Units = volts
CREATE_PUBVAR(GmVcn,        float);
CREATE_PUBVAR(GmVanSqr,     float);     // Units = volts^2
CREATE_PUBVAR(GmVbnSqr,     float);
CREATE_PUBVAR(GmVcnSqr,     float);

// Hardware scaling variables
CREATE_PUBVAR(GmIaSqr,      float);     //Gm current phase A squared
CREATE_PUBVAR(GmIaT2,       float);
CREATE_PUBVAR(GmIbSqr,      float);     //Gm current phase B squared
CREATE_PUBVAR(GmIbT2,       float);
CREATE_PUBVAR(GmIcSqr,      float);     //Gm current phase C squared
CREATE_PUBVAR(GmIcT2,       float);

// Distortion filter current estimation
CREATE_PUBVAR(DFVaDvDtFil,  float);     //Distortion filter dv/dt, Va filter
CREATE_PUBVAR(DFVbDvDtFil,  float);     //Distortion filter dv/dt, Vb filter
CREATE_PUBVAR(DFVcDvDtFil,  float);     //Distortion filter dv/dt, Vc filter
CREATE_PUBVAR(DFVaDvDtSq,   float);     //Distortion filter dv/dt, Va
CREATE_PUBVAR(DFVbDvDtSq,   float);     //Distortion filter dv/dt, Vb
CREATE_PUBVAR(DFVcDvDtSq,   float);     //Distortion filter dv/dt, Vc
CREATE_PUBVAR(DFVaDvDt,     float);     //Distortion filter dv/dt, Va
CREATE_PUBVAR(DFVbDvDt,     float);     //Distortion filter dv/dt, Vb
CREATE_PUBVAR(DFVcDvDt,     float);     //Distortion filter dv/dt, Vc
CREATE_PUBVAR(DFVllFilGn,   float);     //Distortion filter derivative filter gain

//Stator feedbacks
CREATE_PUBVAR(S_Ia,         float);     // Current feedback signals
CREATE_PUBVAR(S_Ib,         float);     // Units = amps
CREATE_PUBVAR(S_Ic,         float);
CREATE_PUBVAR(S_IaSqr,      float);     // Units = amps^2
CREATE_PUBVAR(S_IbSqr,      float);
CREATE_PUBVAR(S_IcSqr,      float);
CREATE_PUBVAR(S_IaRms,      float);     // RMS stator current values
CREATE_PUBVAR(S_IbRms,      float);     // for Toc protection (Arms)
CREATE_PUBVAR(S_IcRms,      float);


CREATE_PUBVAR(S_Vab,        float);     // Voltage feedback signals
CREATE_PUBVAR(S_Vbc,        float);     // Units = volts
CREATE_PUBVAR(S_VcaT2,      float);
CREATE_PUBVAR(S_VanSqr,     float);
CREATE_PUBVAR(S_VbnSqr,     float);     // Units = volts^2
CREATE_PUBVAR(S_VcnSqr,     float);
CREATE_PUBVAR(S_VabSqr,     float);
CREATE_PUBVAR(S_VbcSqr,     float);


CREATE_PUBVAR(S_IaOffsCal,  float);     // Current and voltage offsets
CREATE_PUBVAR(S_IbOffsCal,  float);     // Units = amps
CREATE_PUBVAR(S_IcOffsCal,  float);
CREATE_PUBVAR(S_VabOffsCal, float);     // Units = volts
CREATE_PUBVAR(S_VbcOffsCal, float);

// Current and voltage offset calculation variables
CREATE_PUBVAR(S_IaOffsIn,   float);
CREATE_PUBVAR(S_IbOffsIn,   float);
CREATE_PUBVAR(S_IcOffsIn,   float);
CREATE_PUBVAR(S_IOffsGn,    float);
CREATE_PUBVAR(S_VabOffsAcc, float);
CREATE_PUBVAR(S_VabOffsFil, float);
CREATE_PUBVAR(S_VabOffsIn,  float);
CREATE_PUBVAR(S_VbcOffsAcc, float);
CREATE_PUBVAR(S_VbcOffsFil, float);
CREATE_PUBVAR(S_VbcOffsIn,  float);
CREATE_PUBVAR(S_VoffRunTm,  float);
CREATE_PUBVAR(S_VOffsGn,    float);
CREATE_PUBVAR(S_VoffAccCnt, int);
CREATE_PUBVAR(S_VoffFilCnt, int);
CREATE_PUBVAR(S_VOffsAccN,  int);
CREATE_PUBVAR(S_VOffsFilN,  int);
CREATE_PUBVAR(S_IoffFltSet, unsigned) = false;
CREATE_PUBVAR(S_VoffFltSet, unsigned) = false;
CREATE_PUBVAR(S_VOffsVld,   unsigned) = false;

CREATE_PUBVAR(S_IabcGnd,    float);
CREATE_PUBVAR(S_IabcGndGn,  float);
CREATE_PUBVAR(S_IabcGndFil, float);
CREATE_PUBVAR(S_ISqrSumFil, float) = 0.0;  // filtered sum of square of stator currents
CREATE_PUBVAR(S_IocLvlFlg,  unsigned);     // Stator current > IOC level flag
CREATE_PUBVAR(S_IocFltInh,  unsigned);     // Stator IOC fault inhibit

// Grid Monitor Fast Vmag vars
CREATE_PUBVAR(GmVMagFstSqr, float);        // Fast grid monitor sum of squared voltages
CREATE_PUBVAR(GmVMagFstPk,  float);        // Fast grid monitor sum of sqr voltages peak detected

// Stator Toc Detection Vars
CREATE_PUBVAR(S_WTocT3,        float);     // Omega for timed over-current (normal) - task 3
CREATE_PUBVAR(S_WTocFstT3,     float);     // Omega for timed over-current (fast) - task 3
CREATE_PUBVAR(S_WTocT3Dyn,     float);     // Omega for timed over-current - task 3
CREATE_PUBVAR(S_IaRmsMag,      float);     // Phase A current rms magnitude
CREATE_PUBVAR(S_IbRmsMag,      float);     // Phase B current rms magnitude
CREATE_PUBVAR(S_IcRmsMag,      float);     // Phase C current rms magnitude
CREATE_PUBVAR(S_TocHiSel,      float);     // High select for stator TOC model
CREATE_PUBVAR(S_TocHiSelSq,    float);     // High select squared for stator TOC model
CREATE_PUBVAR(S_TocAlmFlg,     unsigned);  // Stator TOC alam flag
// Stator Dynamic Toc threshold
CREATE_PUBVAR(S_TocThrDy  ,    float);     // Stator over-current dymanic trip threshold (Amp)
CREATE_PUBVAR(S_TocAlmOnDy,    float);     // Stator over-current dymanic alarm on threshold (Amp)
CREATE_PUBVAR(S_TocAlmOffDy,   float);     // Stator over-current dymanic alarm off threshold (Amp)



CREATE_PUBVAR(TestWord2,    unsigned);
CREATE_PUBVAR(AeadGainFlt,  unsigned);




// Unpublished Variables
//----------------------
float       S_TocAlmOnDl;
float       S_TocAlmOffDl;

// Parameters
//-----------
CREATE_PARM(S_SyncIOC2Lvl,      float);

// Diagnostics
//------------
CREATE_DIAG(S_SyncIOC2Flt);             // Stator sync IOC level 2 fault

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Offline offset calculations for stator current and voltage feedbacks.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  StatorCalcFbkOffsT1( void )
{
    // expect this to be done in brg i/f <==> offset null classes
    //  - need to connect the offset null enables below to the new class

    if  ( R_MctlCmd.S_IoffNullEn )
    {
        S_IaOffsCal += S_IOffsGn * ( S_IaOffsIn - S_IaOffsCal );
        S_IbOffsCal += S_IOffsGn * ( S_IbOffsIn - S_IbOffsCal );
        S_IcOffsCal += S_IOffsGn * ( S_IcOffsIn - S_IcOffsCal );
    }

    if ( R_MctlCmd.S_VoffNullEn )
    {
        S_VabOffsFil += S_VOffsGn * ( S_VabOffsIn - S_VabOffsFil );
        S_VbcOffsFil += S_VOffsGn * ( S_VbcOffsIn - S_VbcOffsFil );

        if ( S_VoffFilCnt == S_VOffsFilNT1 )
        {
            S_VabOffsAcc += S_VabOffsFil;
            S_VbcOffsAcc += S_VbcOffsFil;
            ++S_VoffAccCnt;

            if ( S_VoffAccCnt == S_VOffsAccNT1 )
            {
                S_VabOffsCal = S_VabOffsAcc / S_VOffsAccNT1;
                S_VbcOffsCal = S_VbcOffsAcc / S_VOffsAccNT1;
                S_VabOffsAcc = 0.0;
                S_VbcOffsAcc = 0.0;
                S_VoffAccCnt = 0;
                S_VOffsVld   = true;
                S_VOffsTm    = SysTm2;
            }
        }
        else
        {
            S_VabOffsAcc = 0.0;
            S_VbcOffsAcc = 0.0;
            S_VoffAccCnt = 0;
            ++S_VoffFilCnt;
        }
    }
    else
    {
        S_VabOffsFil = 0.0;
        S_VbcOffsFil = 0.0;
        S_VabOffsAcc = 0.0;
        S_VbcOffsAcc = 0.0;
        S_VoffAccCnt = 0;
        S_VoffFilCnt = 0;
    }


    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculates variables used in stator offline feedback offset algorithm
//      and checks for offset fault conditions.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  StatorProcessFbks2( void )

{
    float S_ISqrSum;

        /* Calculate current offset filter gain. */

    S_VcaT2 = -(S_VabT2 + S_VbcT2);

    // expect this to be done in brg i/f <==> offset null classes
    //  - need to connect the offset null enables below to the new class

    if ( R_MctlCmd.S_IoffSlowSw )
        S_IOffsGn = PARM(S_WIOffsSlw) * R_DelTm1;
    else
        S_IOffsGn = PARM(S_WIOffsFst) * R_DelTm1;

        /* Calculate voltage offset pre-filter gain and integration time. */
    if (R_MctlCmd.S_VoffFastSw)
    {
        S_VOffsGn = PARM(S_WVOffsFst) * R_DelTm1;  /* Use fast filter omega */
        S_VoffRunTm = PARM(S_VOffsFst);             /* Use fast run time */
    }
    else
    {
        S_VOffsGn = PARM(S_WVOffsFil) * R_DelTm1;  /* Use normal filter omega */
        S_VoffRunTm = PARM(S_VOffsRnTm);           /* Use normal run time */
    }
        /* Check for current offset faults. */

    if ( !(R_MctlCmd.S_IoffNullEn) )
    {
        if ( !S_IoffFltSet )
        {
            if ( ABS( S_IaOffsCal + PARM(S_IaOffs) ) > PARM(S_IOffsThr) )
            {
                PUSH_DIAG( S_IaOffsFlt );
                S_IoffFltSet = true;
            }
            if ( ABS( S_IbOffsCal + PARM(S_IbOffs) ) > PARM(S_IOffsThr) )
            {
                PUSH_DIAG( S_IbOffsFlt );
                S_IoffFltSet = true;
            }
            if ( ABS( S_IcOffsCal + PARM(S_IcOffs) ) > PARM(S_IOffsThr) )
            {
                PUSH_DIAG( S_IcOffsFlt );
                S_IoffFltSet = true;
            }
        }
    }
    else
    {
        S_IoffFltSet = false;
    }

    if ( !(R_MctlCmd.S_VoffNullEn) )
    {
            /* Calculate number of Task 1 cycles to run voltage offset */
            /* accumulator.                                            */

        S_VOffsAccN = (int)(S_VoffRunTm * R_DelTm1Rcp ) + 1;
        if ( S_VOffsAccN < 1 )  S_VOffsAccN = 1;

            /* Calculate number of Task 1 cycles to run voltage offset  */
            /* prefilter before activating accumulator - set to 5 times */
            /* filter tau in units of Task 1 cycles.                    */

        S_VOffsFilN = FIL_TAU_N * (int)(1.0/(S_VOffsGn)) + 1;
        if ( S_VOffsFilN < 0 )  S_VOffsFilN = 0;

    }


    if ( !(R_MctlCmd.S_VoffNullEn) || TripFlt )
    {
            /* Check for voltage offset faults. */

        if ( !S_VoffFltSet )
        {
            if ( ABS( S_VabOffsCal + PARM(S_VabOffs) ) > PARM(S_VOffsThr) )
            {
                PUSH_DIAG( S_VabOffsFlt );
                S_VoffFltSet = true;
                S_VOffsVld   = false;
            }
            if ( ABS( S_VbcOffsCal + PARM(S_VbcOffs) ) > PARM(S_VOffsThr) )
            {
                PUSH_DIAG( S_VbcOffsFlt );
                S_VoffFltSet = true;
                S_VOffsVld   = false;
            }
        }
    }
    else
    {
        S_VoffFltSet = false;
    }

        /* Declare voltage offset invalid when specified time expired. */
        /* Declare voltage offset invalid when in simulator mode.      */
    if (( Delta_Tm( SysTm2, S_VOffsTm ) > PARM(S_VOffsVlTm) ) || SimMode )
        S_VOffsVld = false;


     //              Stator IOC check
     // --------------------------------------------------
    S_ISqrSum = S_IaSqrT2 + S_IbSqrT2 + S_IcSqrT2;

    if ( InitDone )
    {
        S_ISqrSumFil += PARM(S_WIOC) * DelTm2  * ( S_ISqrSum - S_ISqrSumFil );
    }

    if (S_ISqrSumFil > PARM(S_IOCLvl))
    {
        S_IocLvlFlg = true;
    }
    else
    {
        S_IocLvlFlg = false;
    }

    S_IocFltInh = R_CroBarFltT2 || OpnPhsAct || S_VMaxHiDet;


    if ( !S_K1ClsCmd )      // Trip if trying to open with current above interrupt capacity
    {
        if ( S_IocLvlFlg && !S_IocFltInh )
        {
            PUSH_DIAG(S_IOCFlt);
            FaultIsActive.S_IOCFlt = true;
        }
    }

    if ( R_SyncIOCEnb )
    {
        if (S_ISqrSumFil > PARM(S_SyncIOC2Lvl))
        {
            if ( !FAULTISACTIVE(S_SyncIOC2Flt) )
            {
                MvSwGearTrpReq = true;
            }
            PUSH_DIAG(S_SyncIOC2Flt);
            FaultIsActive.S_SyncIOC2Flt = true;
        }
    }
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculates primary stator feedbacks used by control, including
//      three shunt currents and two L-N voltages.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
 void StatorProcessFbks( void )
 {

    // Calculate square of line to neutral voltage feedbacks
    S_VanSqr = S_Van * S_Van;
    S_VbnSqr = S_Vbn * S_Vbn;
    S_VcnSqr = S_Vcn * S_Vcn;

    // Calculate square of line to line voltage feedbacks
    S_VabSqr = S_Vab * S_Vab;
    S_VbcSqr = S_Vbc * S_Vbc;
    if ( InitDone )
    {
            // Calculate ground current based on phase currents
        S_IabcGnd = S_Ia + S_Ib + S_Ic;

            /* Filter ground current and test against trip level */
        S_IabcGndFil += S_IabcGndGn * ( ABS( S_IabcGnd ) - S_IabcGndFil );

  // note: ct's saturate during line disturbance so can't do this check:

  //      if ( S_IabcGndFil >= PARM(S_IabcFltThr) )
  //          T1Flt.bit.S_IabcGndFlt = true;
  //      else
  //          T1Flt.bit.S_IabcGndFlt = false;
    }

    // Calculate square of phase currents and voltages feedbacks
    S_IaSqr = S_Ia * S_Ia;
    S_IbSqr = S_Ib * S_Ib;
    S_IcSqr = S_Ic * S_Ic;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculates three line to line grid monitor voltages used by the
//      control.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorProcessFbksBrdg1( void )
{
    float VMagFstDelta;
    //float DervErr;

    //Fast over-voltage detection vars
    GmVab = GmVan - GmVbn;
    GmVbc = GmVbn - GmVcn;
    GmVca = GmVcn - GmVan;

    GmVMagFstSqr = ( GmVab*GmVab)  + ( GmVbc*GmVbc) + ( GmVca*GmVca);

    // Calculate the pll error reference
    VMagFstDelta = GmVMagFstSqr - GmVMagFstPk;

    if ( VMagFstDelta >= 0.0)
    {
        GmVMagFstPk   = GmVMagFstPk + (GmVMgFstUpGn* VMagFstDelta);
    }
    else
    {
        GmVMagFstPk   = GmVMagFstPk + (GmVMgFstDnGn * VMagFstDelta);
    }

    GmIaSqr = GmIa * GmIa;
    GmIbSqr = GmIb * GmIb;
    GmIcSqr = GmIc * GmIc;

    GmVanSqr= GmVan * GmVan;
    GmVbnSqr= GmVbn * GmVbn;
    GmVcnSqr= GmVcn * GmVcn;

    //Calculations for use in determining distortion fliter currents

    //Vab
    DFVaDvDt      =  (GmVab- GmVca) - DFVaDvDtFil;
    DFVaDvDtSq    =  DFVaDvDt*DFVaDvDt;
    DFVaDvDtFil   += DFVllFilGn*DFVaDvDt;

    //Vbc
    DFVbDvDt      =  (GmVbc - GmVab) - DFVbDvDtFil;
    DFVbDvDtSq    =  DFVbDvDt*DFVbDvDt;
    DFVbDvDtFil   += DFVllFilGn*DFVbDvDt;

    //Vca
    DFVcDvDt      = (GmVca - GmVbc) - DFVcDvDtFil;
    DFVcDvDtSq    =  DFVcDvDt*DFVcDvDt;
    DFVcDvDtFil   += DFVllFilGn*DFVcDvDt;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculates calculates task2 variables needed for the line voltage
//      and current feedbacks.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorProcessFbksT2( void )
{

    GmVanT2 = T1T2FbkScale * GmVanT2;
    GmVbnT2 = T1T2FbkScale * GmVbnT2;
    GmVcnT2 = T1T2FbkScale * GmVcnT2;

    GmIaT2 = T1T2FbkScale * GmIaT2;
    GmIbT2 = T1T2FbkScale * GmIbT2;
    GmIcT2 = T1T2FbkScale * GmIcT2;


    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Perform timed over-current check on each of the phase currents.
//      The inputs are squared, then lightly filtered phase currents.
//      This function filters these signals much more by an adjustable amount.
//      Outputs of these filters is then compared to an adjustable threshold.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void StatorTocDetection(void)
{
    S_IaRmsMag = S_IaRmsMag + S_WTocT3Dyn*(S_IaSqrT3 - S_IaRmsMag);
    S_IbRmsMag = S_IbRmsMag + S_WTocT3Dyn*(S_IbSqrT3 - S_IbRmsMag);
    S_IcRmsMag = S_IcRmsMag + S_WTocT3Dyn*(S_IcSqrT3 - S_IcRmsMag);

    S_IaRms    = sqrtf(S_IaRmsMag);
    S_IbRms    = sqrtf(S_IbRmsMag);
    S_IcRms    = sqrtf(S_IcRmsMag);

    // if or not to use the dynamic TOC configuration
    if ( PARM(TocDyThrshEnb) )
    {
        if ( PARM(DelSpdTrqAct) )
        {
            S_TocThrDy    = S_MaxCurOut * PARM(S_TocMargin);
        }
        else
        {
            S_TocThrDy    = S_MaxCurOut;
        }
        S_TocAlmOnDy  = S_TocThrDy - S_TocAlmOnDl;
        S_TocAlmOffDy = S_TocAlmOnDy - S_TocAlmOffDl;
    }
    else
    {
        S_TocThrDy    = PARM(S_TocThrs);
        S_TocAlmOnDy  = PARM(S_TocAlmOn);
        S_TocAlmOffDy = PARM(S_TocAlmOff);
    }

    if ( S_IaRmsMag > S_IbRmsMag )
    {
         S_TocHiSelSq = S_IaRmsMag;
         S_TocHiSel   = S_IaRms;
    }
    else
    {
         S_TocHiSelSq = S_IbRmsMag;
         S_TocHiSel   = S_IbRms;
    }

    if ( S_IcRmsMag > S_TocHiSelSq )
    {
        S_TocHiSelSq = S_IcRmsMag;
        S_TocHiSel   = S_IcRms;
    }


    if ( S_TocHiSel >= S_TocThrDy )
    {
        PUSH_DIAG(S_TocFlt);
    }

    if ( S_TocHiSel >= S_TocAlmOnDy )
    {
        S_TocAlmFlg = true;
    }
    else if ( S_TocHiSel <= S_TocAlmOffDy )
    {
        S_TocAlmFlg = false;
    }

    if ( S_TocAlmFlg )
    {
        PUSH_DIAG(S_TocAlm);
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculate variables used for scaling feedbacks in Task 1.
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  StatorFbksBgnd( void )
{

    S_IabcGndGn = PARM(S_WIabcGnd) * R_DelTm1;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculate task3 variables needed for the line voltage and current
//      feedbacks.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void GridMonitorProcessFbksT3( void )
{
    return;
}

