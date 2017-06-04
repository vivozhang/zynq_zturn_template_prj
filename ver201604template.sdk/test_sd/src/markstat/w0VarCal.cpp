/*=============================================================================
* Product:     GE Power Conversion Using MACC Based Control
* Host:        TMS320C31 DSP
* File:        $\W0VARCAL.C30
* Language:    TI 'C'
* Copyright:   2003 GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file contains those functions which are associated with
*           variable conditioning.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
*
*==============================================================================
* 030124 SAB Create for regulator control and parameter conditioning.
* 031004 JMN Modify LoVRdThru logic to set false if line converter not running
*            Modify TrqReduct logic to set false if generator not on-line (!Running)
* 031018 SAB Add calculation of calculated grid currents
* 040108 SAB Move slow functions to W0VARCLS.C30
* 051201 SAB Add local ref vars
* 060221 SAB Move calc of LineIyPfCmd to A0FlxCmd.c30
* 070109 IB  Add simulated y-grid current.
* 070202 SAB Remove sim mode conditional from calc of IxGrid, IyGrid
* 070326 SAB Remove FbkScale
* 070705 SAB Add S_PwrCalc, GridPwrCalc, WSlip
* 070712 SAB Rename GridPwrCalc to GridPwrEst.  Add new calculations for grid
*            power error
* 070801 SAB Remove vars no longer used and stator/rotor power calcs.
* 070824 SAB Remove S_RFeRcp
* 071101 SAB Add TrqPwrTrimT1, T2
* 081002 SAB Declare
* 090313 SAB Add LclSpdSimRfx
*______________________________________________________________________________
* Last Mod: March 13, 2009 11:14:39AM
*/

# include "x0FrameWork.h"
# include "Product.h"
# include "w0VarCal.h"

#define UTC_THRESHOLD  630720000       // 6.3e8 = 20 years

NVVAR_UINT(PowerUpTmAcc)       // total powered-up time (min)
NVVAR_UINT(RunningTmAcc)       // total running time (min)
NVVAR_UINT(PowerGenAcc)        // total power generated (megawatt-min)

// extern
extern float            GmTrqErrT3;
extern float            GmVMagT3;
extern float            HvIyRatNWPk;
extern float            HvIyRatPk;
extern float            HvSlopeMin;
extern float            HvSlopeMinNW;
extern float            L_BrgLossPwr;
extern float            L_IMagT3;
extern float            L_RIndCorRcp;
extern float            LvIyRatNWPk;
extern float            LvIyRatPk;
extern float            LvPwrTrqScl;
extern float            LvSlopeMax;
extern float            LvPwrTrqScl;
extern float            TurbNpRatRcp;
extern float            LvSlopeMaxNW;
extern float            R_R2;
extern float            R_RfeRcp;
extern float            R_RIndCorRcp;
extern float            R_Rr;
extern float            R_SpdFbkT2;
extern float            R_SpdFbkT3;
extern float            R_TrqCmdInT2;
extern float            R_TrqCmdInT3;
extern float            R_VMag;
extern float            S_IMag;
extern float            S_R1;
extern float            S_RfeRcp;
extern float            S_Rs;
extern float            S_VMagT3;
extern float            VregHvMaxSlp;
extern float            VregHvMinSlp;
extern float            VregHvSlopePk;
extern float            VregHvSlpNWPk;
extern float            VregIyLimDyn;
extern float            VregIyLimNWPk;
extern float            VryLimPk;
extern float            VregLVIyMnDy;
extern float            VregLVIyMnDyNWPk;
extern float            VregLVIyMnDyPk;
extern float            VregLvMaxSlp;
extern float            VregLvMinSlp;
extern float            VregLvSlopePk;
extern float            VregLvSlpNWPk;
extern float            VregMxMnDfNWPk;
extern float            VregMxMnDiffPk;
extern float            VregMxMnDyn;
extern float            VregIyLimPk;
extern float            DelTm3;
extern float            GmIMagT3;
extern float            GmPwr;
extern float            GmPwrFactor;
extern float            GmPwrT3;
extern float            GmQPwr;
extern float            L_VlnIn;
extern float            R_BrgLossT3;
extern float            R_TrqLimFT3;
extern float            CoolPwrIn;
extern float            DFTocHiSel;
extern float            L_VMagT3;
extern float            PhiCmd;
extern float            R_IMag;
extern float            R_SpdFbk;
extern float            VregInt;
extern float            XfmrRatioLnStrRcp;
extern float            TurbNpRatWtt;
extern int              SeqSt;
extern unsigned         GmLineVLo;
extern unsigned         L_LTestIp;
extern unsigned         L_Running;
extern unsigned         L_ZVGtOff;
extern unsigned         LineRunOnly;
extern unsigned         OpnPhsEnable;
extern unsigned         PrecIySpdPrm;
extern unsigned         R_CroBarFlag;
extern unsigned         R_Running;
extern unsigned         R_SpdRngRed;
extern unsigned         RPNWRunning;
extern unsigned         RtrLnIyXfer;
extern unsigned         Running;
extern unsigned         SimMode;
extern unsigned         T3CalcDone;
extern unsigned         VregNSeqSpdA;
extern unsigned         ZVRTIp;
extern FaultIsActiveTyp FaultIsActive;
extern nvVar_tbl        NV;
extern unsigned         R_SeqReq_PackedBits;
extern SysTm_Typ        SysTm3;
extern RRegLimFlgs      R_InnrRegLim;     // rotor regulator limit hit

// VarCalcT2()
extern unsigned     IslandDet;
/* *Vars */

// Parameters
//-----------
CREATE_PARM(GmLvrtIpTm,         float);     // Gm LVRT right-through in progress timer threshold
CREATE_PARM(GmLvrtRcvTm,        float);     // Gm LVRT recovery timer threshold
CREATE_PARM(WPwrFactFil,        float);     // power factor diag var filter omega
CREATE_PARM(WPhiCalcFil,        float);     // power angle diag var filter omega
CREATE_PARM(WIMagGridFil,       float);     // grid current diag var filter omega
CREATE_PARM(WPwrGridFil,        float);     // grid power diag var filter omega
CREATE_PARM(WQPwrGridFil,       float);     // grid reactive power diag var filter omega
CREATE_PARM(LvrtUvThr,          float);     // low voltage threshold (%); for LVRT bit
CREATE_PARM(LvrtUvHyst,         float);     // low voltage hysteresis band(%)
// Torque trimmer
CREATE_PARM(TrqTrimIntGn,    float); // Torque trimmer integral gain
CREATE_PARM(TrqTrimLim,      float); // Torque trimmer limit (watts);
CREATE_PARM(GmWTrqFbkFil,    float); // Grid monitor torque filter omega
CREATE_PARM(PwrTrimFfDyn,    unsigned); // Dynamic feed-fowrward for power trim

// Variables
//----------
CREATE_PUBVAR(VregHvMaxSlp      ,float);
CREATE_PUBVAR(VregHvMinSlp      ,float);
CREATE_PUBVAR(VregIyLimDyn      ,float);
CREATE_PUBVAR(VregLVIyMnDy      ,float);
CREATE_PUBVAR(VregLvMaxSlp      ,float);
CREATE_PUBVAR(VregLvMinSlp      ,float);
CREATE_PUBVAR(VregMxMnDyn       ,float);
CREATE_PUBVAR(GmTrqErrT3        ,float);
CREATE_PUBVAR( IMagGrid         ,float);
CREATE_PUBVAR( IMagGridFil      ,float);
CREATE_PUBVAR( IxGrid           ,float);
CREATE_PUBVAR( IxGridCalc       ,float);
CREATE_PUBVAR( IyGrid           ,float);
CREATE_PUBVAR( IyGridCalc       ,float);
CREATE_PUBVAR( PwrGrid          ,float);
CREATE_PUBVAR( PwrGridFil       ,float);
CREATE_PUBVAR( QPwrGrid         ,float);
CREATE_PUBVAR( QPwrGridFil      ,float);
CREATE_PUBVAR( R_LSigmaDtT2     ,float);            // Rotor L_Sigma/deltm2
CREATE_PUBVAR( WSlip    ,float);
CREATE_PUBVAR(GmAsymRcvIp , unsigned) =false;  //  Gm asymmetry recovery in progress
CREATE_PUBVAR(GmAsymRTIp  , unsigned) =false;  //  Gm asymmetry ride-through
CREATE_PUBVAR(LclTrqRefx    ,   float );
CREATE_PUBVAR(LclVarRefx    ,   float);

CREATE_PUBVAR(  GmAsymRcvTmr, float);           //  Gm asymmetry recovery timer
CREATE_PUBVAR(  GmAsymRTTmr, float);            //  Gm asymmetry ride-through timer
CREATE_PUBVAR( CoreLossTot, float);
CREATE_PUBVAR( CroBarFltSav, unsigned);
CREATE_PUBVAR( DFIRLoss, float);
CREATE_PUBVAR( Gm_IRLoss, float);
CREATE_PUBVAR( GmTrqFbk , float);
CREATE_PUBVAR( GmTrqFbkFil , float);
CREATE_PUBVAR(L_VMagT3,      float);
CREATE_PUBVAR( L_IRLoss, float);
CREATE_PUBVAR( LvrtCrowBar, unsigned);
CREATE_PUBVAR( LvrtLowVolt, unsigned);
CREATE_PUBVAR( LvrtOutCntrl, unsigned);
CREATE_PUBVAR( LvrtSpdRgRed, unsigned);
CREATE_PUBVAR( LvrtTrqLim, unsigned);
CREATE_PUBVAR( PhiCalc    ,float);
CREATE_PUBVAR( PhiCalcFil    ,float);
CREATE_PUBVAR( PowerGenAcc, float);      // total power generated (megawatt-hours)
CREATE_PUBVAR( PowerUpTmAcc, float);     // total powered-up time (hours)
CREATE_PUBVAR( PowerUpTmr, float);
CREATE_PUBVAR( PwrCmdLine , float);
CREATE_PUBVAR( PwrFactFil    ,float);        // filtered system power factor
CREATE_PUBVAR( PwrRegErr, float);
CREATE_PUBVAR( PwrRegInt , float);
CREATE_PUBVAR( PwrRegTot, float);
CREATE_PUBVAR( PwrTrimFF, float);
CREATE_PUBVAR( QregClmpSF, float);
CREATE_PUBVAR( R_I2RLossPwr    ,float);   // I^2 * R on rotor
CREATE_PUBVAR( R_IRLoss, float);
CREATE_PUBVAR( R_SpdFbkT3Rc, float);
CREATE_PUBVAR( RunningTmAcc, float);     // total running time (hours)
CREATE_PUBVAR( RunningTmr  , unsigned);
CREATE_PUBVAR( S_I2RLossPwr    ,float);   // I^2 * R on stator
CREATE_PUBVAR( S_IRLoss, float);
CREATE_PUBVAR( TotalLossPwr    ,float);   // I^2 * R plus BrdgLossPwrs
CREATE_PUBVAR( TransDiagInh, unsigned);
CREATE_PUBVAR( TrqPwrTrim , float);
CREATE_PUBVAR( WasRunning, unsigned);
CREATE_PUBVAR( WattSecAccFrc, float);
CREATE_PUBVAR( WattSecAccInt, int);
CREATE_PUBVAR(VregNSeqHi, unsigned) = false;
CREATE_PUBVAR(PwrCmdLineT3 ,   float);
CREATE_PUBVAR(OpnPhsExtEnb,   unsigned); // Open phase external enable

CREATE_PUBVAR(GmLvrtIpTmr,      float);     // Gm LVRT in progress timer
CREATE_PUBVAR(GmLvrtRcvTmr,     float);     // Gm LVRT recovery timer
CREATE_PUBVAR(GmLvrtIp,         unsigned);  // Gm LVRT in progress flag
CREATE_PUBVAR(GmLvrtRcvIp,      unsigned);  // Gm LVRT recovery in progress flag
CREATE_PUBVAR(GmLvOpnPhsDs,     unsigned);  // Gm open phase desentize flag under low voltage

CREATE_PUBVAR(R_SpdFbkRadT3,     float);
CREATE_PUBVAR(OpnGrdTmDyn,     float);             // Open grid time, dynamic
CREATE_PUBVAR(OpnPhsTmDyn1,    float);             //Open phase dynamic time #1
CREATE_PUBVAR(OpnPhsTmDyn2,    float);             //Open phase dynamic time #2
CREATE_PUBVAR(HvIyMaxDyn,      float );
CREATE_PUBVAR(LvIyMaxDyn,      float );
/*****************************************************************************
*
*       external references
*
*****************************************************************************/

extern float DelTm2;
extern float GmDscmVMgNT3;
extern unsigned GmLVRTRcyIpT3;
extern float L_IxFbkT2;
extern float L_IyFbkT2;
extern float L_IyHrmFCal;
extern float L_PwrLine;
extern float L_VarHrmF;
extern float L_VarLine;
extern float L_VMagT2;
extern float R_WRtr;
extern float S_IxCalc;
extern float S_IxFbkT2;
extern float S_IyCalc;
extern float S_IyFbkT2;
extern float S_Pwr;
extern float S_QPwr;
extern unsigned R_CroBarActT3;
extern unsigned         R_SeqReq_PackedBits;
SysTm_Typ  LvrtCrowBarTm;

// Data Pass
//----------
DATA_PASS(PwrCmdLine, PwrCmdLineT3 ,  T2_T3,   FILTERED);
DATA_PASS(L_VMagT2,   L_VMagT3     ,  T2_T3,   FILTERED);


/* **************************************************************************
*
*                               Private Variables
*
*****************************************************************************/


/*=============================================================================
 * Function: VarCalcT2
 * Execution Level: Task 2
 * Execution Time: Untimed
 * Formal Inputs: None
 * Formal Outputs: None
 *  Global Inputs:
 *
 *
 * Global Outputs:
 *
 * Locals:         none
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function calculates system variables
 *
 *===========================================================================*/
void VarCalcT2()
{

    IxGrid = L_IxFbkT2 + S_IxFbkT2;
    IyGrid = L_IyFbkT2 + S_IyFbkT2 - L_IyHrmFCal;


    IMagGrid = sqrtf(IxGrid*IxGrid + IyGrid*IyGrid);

    // calc total real & reactive power
    PwrGrid      =  L_PwrLine + S_Pwr ;
    QPwrGrid     =  L_VarLine + S_QPwr - L_VarHrmF;


    //Estimate WSlip
    WSlip        =  PARM(L_LineFrq) - R_WRtr;

    // filtered grid variables
    IMagGridFil += PARM(WIMagGridFil) * DelTm2  * ( IMagGrid - IMagGridFil );
    PwrGridFil  += PARM(WPwrGridFil)  * DelTm2  * ( PwrGrid  - PwrGridFil );
    QPwrGridFil += PARM(WQPwrGridFil) * DelTm2  * ( QPwrGrid - QPwrGridFil );

    //Calculated grid currents

    IxGridCalc = L_IxFbkT2 + S_IxCalc;
    IyGridCalc = L_IyFbkT2 + S_IyCalc - L_IyHrmFCal;


    if (R_SeqReq.R_TrqEnbReq)
        PwrCmdLine = -R_TrqCmdInT2 * R_SpdFbkT2;
    else
        PwrCmdLine = 0.0F;

    ///////////////////////////////////////////////////////////
    // Set level of open grid and open phase detection
    // detection based on power command
    ///////////////////////////////////////////////////////////

    if ( GmAsymRTIp || GmAsymRcvIp || GmLvrtIp || GmLvrtRcvIp)
    {
        GmLvOpnPhsDs = true;
    }
    else
    {
        GmLvOpnPhsDs = false;
    }
    if ( PwrCmdLine > PARM(OpnGrdPwrLv3))//800KW
    {
        OpnGrdTmDyn  = PARM(OpnGrdTm3);
        if ( !GmLvOpnPhsDs)
        {
            OpnPhsTmDyn1 = PARM(OpnPhsTm1);
            OpnPhsTmDyn2 = PARM(OpnPhsTm2); //0.003
        }
        else
        {
            OpnPhsTmDyn1 = PARM(OpnPhsTm1Asy);
            OpnPhsTmDyn2 = PARM(OpnPhsTm2Asy); //0.01
        }
    }
    else if ( PwrCmdLine > PARM(OpnGrdPwrLv2))//400KW
    {
        OpnGrdTmDyn  = PARM(OpnGrdTm2);
        if ( !GmLvOpnPhsDs)
        {
            OpnPhsTmDyn1 = PARM(OpnPhsTm1Mp);
            OpnPhsTmDyn2 = PARM(OpnPhsTm2Mp);//0.003
        }
        else
        {
            OpnPhsTmDyn1 = PARM(OpnPhsTm1Asy);
            OpnPhsTmDyn2 = PARM(OpnPhsTm2Asy); //0.010
        }
    }
    else if ( PwrCmdLine > PARM(OpnGrdPwrLv1))//200kw
    {
        OpnGrdTmDyn  = PARM(OpnGrdTm1);
        if ( !GmLvOpnPhsDs)
        {
            OpnPhsTmDyn1 = PARM(OpnPhsTm1Lp);
            OpnPhsTmDyn2 = PARM(OpnPhsTm2Lp); //0.005
        }
        else
        {
            OpnPhsTmDyn1 = PARM(OpnPhsTm1Asy);
            OpnPhsTmDyn2 = PARM(OpnPhsTm2Asy); //0.010
        }
    }
    else
    {
        OpnGrdTmDyn  = PARM(OpnGrdTm);
        if ( !GmLvOpnPhsDs)
        {
            OpnPhsTmDyn1 = PARM(OpnPhsTm1Lp0);
            OpnPhsTmDyn2 = PARM(OpnPhsTm2Lp0); //0.010
        }
        else
        {
            OpnPhsTmDyn1 = PARM(OpnPhsTm1Asy);
            OpnPhsTmDyn2 = PARM(OpnPhsTm2Asy);
        }
    }


    if ( !PARM(OpnPhsInh)   && !ZVRTIp && !L_LTestIp &&  !GmLineVLo && !IslandDet && ( (LineRunOnly && ( ABS(VregInt) > PARM(OpnGRPNWIThr )) )
            || (SeqSt == CONV_RUNNING_ROTOR) )  )
    {
        OpnPhsExtEnb =  true;
    }
    else
    {
        OpnPhsExtEnb =  false;
    }
}
/*=============================================================================
 * Function: ScaleCalc
 * Execution Level: Task 3
 * Execution Time: Untimed
 * Formal Inputs: None
 * Formal Outputs: None
 *  Global Inputs:
 *
 *
 * Global Outputs: TransDiagInh
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


void ScaleCalc()
{



    /////////////////////////////////////
    // Neg seq hi for Vreg
    /////////////////////////////////////

    if ( GmDscmVMgNT3 >= PARM(VregNSqHiThr))
    {
        VregNSeqHi     = true;
    }
    else
    {
        VregNSeqHi     = false;
    }


    TransDiagInh = GmLVRTRcyIpT3  || VregNSeqHi || R_CroBarActT3 ;


    /////////////////////////////////////
    // Asymmetry ride-through in progress
    /////////////////////////////////////
    if (VregNSeqHi)
    {
        GmAsymRTTmr = GmAsymRTTmr + DelTm3;
        if ( GmAsymRTTmr >= PARM(GmAsymRTTm))
        {
            GmAsymRTIp  = true;
        }
    }
    else
    {
        GmAsymRTTmr = 0.0;
        if ( GmAsymRTIp && !VregNSeqHi)
        {
            GmAsymRTIp  = false;
            GmAsymRcvIp = true;
        }
    }


    if ( GmAsymRcvIp)
    {
        GmAsymRcvTmr = GmAsymRcvTmr + DelTm3;
        if ( GmAsymRcvTmr >= PARM(GmAsymRcvTm))
        {
            GmAsymRcvIp  = false;
            GmAsymRcvTmr = 0.0;
        }
    }

    /////////////////////////////////////
    // LVRT in progress
    /////////////////////////////////////

    if (GmLineVLo)
    {
        GmLvrtIpTmr = GmLvrtIpTmr + DelTm3;
        if ( GmLvrtIpTmr >= PARM(GmLvrtIpTm))
        {
            GmLvrtIp  = true;
        }
    }
    else
    {
        GmLvrtIpTmr = 0.0F;
        if ( GmLvrtIp && !GmLineVLo)
        {
            GmLvrtIp    = false;
            GmLvrtRcvIp = true;
            GmLvrtRcvTmr= 0.0F;
        }
    }


    if ( GmLvrtRcvIp)
    {
        GmLvrtRcvTmr = GmLvrtRcvTmr + DelTm3;
        if ( GmLvrtRcvTmr >= PARM(GmLvrtRcvTm))
        {
            GmLvrtRcvIp  = false;
            GmLvrtRcvTmr = 0.0F;
        }
    }

    ///////////////////////////////////////////////////////
    // Asymmetry Iy Current reduction speed requirements
    ///////////////////////////////////////////////////////

    if ( (R_SpdFbk > PARM(IyAsySpdOn)) && R_Running)
    {
        VregNSeqSpdA = true;
    }
    else if (R_SpdFbk < PARM(IyAsySpdOff))
    {
        VregNSeqSpdA = false;
    }

}

/*=============================================================================
 * Function: VarCalcT3
 * Execution Level: Task 3
 * Execution Time: Untimed
 * Formal Inputs: None
 * Formal Outputs: None
 *  Global Inputs:
 *
 *
 * Global Outputs:
 *
 * Locals:         none
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function calculates system variables
 *
 *===========================================================================*/
void VarCalcT3()
{

    float P,Q;
    double tmpf;


    if ( PARM(TestMode) == PUMP_BACK )
    {
        R_SpdFbkRadT3 = (PARM(R_RPMSync) * RPM_TO_RAD);
    }
    else
    {
        R_SpdFbkRadT3 = MAX(R_SpdFbkT3, (PARM(R_RPMMinOp) * RPM_TO_RAD));
    }

    R_SpdFbkT3Rc    =  1.0F /R_SpdFbkRadT3;
    LvPwrTrqScl     = TurbNpRatWtt * R_SpdFbkT3Rc;


    P = GmPwr;
    Q = GmQPwr;

    // prevent devide by zero
    if ( (ABS(P) < 0.001F) && (ABS(Q) < 0.001F) )
        P = 0.001F;


    PwrFactFil += MIN(PARM(WPwrFactFil)*DelTm3, 1.0F)  * ( GmPwrFactor - PwrFactFil );

    PhiCalc     = atanf(Q/P);

    PhiCalcFil += MIN(PARM(WPhiCalcFil)*DelTm3, 1.0F)  * ( PhiCalc - PhiCalcFil );



    R_I2RLossPwr = 1.5F * R_IMag * R_IMag * R_R2;      // I^2 * R on rotor
    S_I2RLossPwr = 1.5F * S_IMag * S_IMag * S_R1;      // I^2 * R on stator
    TotalLossPwr = R_I2RLossPwr + S_I2RLossPwr + L_BrgLossPwr + R_BrgLossT3;   // I^2 * R plus bridge losses



    LclTrqRefx   = PARM(LclTrqRef);
    LclVarRefx   = PARM(LclVarRef);



    if ( !RPNWRunning && !RtrLnIyXfer )
    {

        LvIyMaxDyn    = LvIyRatPk;
        VregIyLimDyn  = VregIyLimPk;
        HvIyMaxDyn    = HvIyRatPk;
        VregLvMinSlp  = VregLvSlopePk;  // Direct parm driven
        VregLvMaxSlp  = LvSlopeMax;     // Calculated from parms
        VregHvMinSlp  = HvSlopeMin;     // Calculated from parms
        VregHvMaxSlp  = VregHvSlopePk;  // Direct parm driven
        VregMxMnDyn   = VregMxMnDiffPk;
        VregLVIyMnDy  = VregLVIyMnDyPk;


    }
    else //RPNWRunning or stopped
    {

        LvIyMaxDyn    =  LvIyRatNWPk;
        VregIyLimDyn  =  VregIyLimNWPk;
        HvIyMaxDyn    =  HvIyRatNWPk;
        VregLvMinSlp  =  VregLvSlpNWPk; // Direct parm driven
        VregLvMaxSlp  =  LvSlopeMaxNW;  // Calculated from parms
        VregHvMinSlp  =  HvSlopeMinNW;  // Calculated from parms
        VregHvMaxSlp  =  VregHvSlpNWPk; // Direct parm driven
        VregMxMnDyn   =  VregMxMnDfNWPk;
        VregLVIyMnDy  =  VregLVIyMnDyNWPk;
    }




    /* Calculate the LVRT bits for GEWE to be transmitted over the CAN bus */

    // LVRT - low voltage
    if ( L_Running && (GmVMagT3 < (L_VlnIn * PARM(LvrtUvThr) * 0.01F)) )
        LvrtLowVolt = True;
    else if ( !L_Running || (GmVMagT3 > (L_VlnIn * (PARM(LvrtUvThr)+PARM(LvrtUvHyst)) * .01)) )
        LvrtLowVolt = False;

    // LVRT - crowbar active
    if ( R_CroBarFlag )
    {
        LvrtCrowBar = True;
        LvrtCrowBarTm = SysTm3;
        R_CroBarFlag = False;
    }
    else if ( Delta_Tm(SysTm3, LvrtCrowBarTm) > 0.050F )      // 50ms is canopen pdo #2 tx period
        LvrtCrowBar = False;

    // LVRT - torque limit
    if ( Running && ((R_TrqLimFT3) >= 0.5F) )
        LvrtTrqLim = True;
    else
        LvrtTrqLim = False;

    // LVRT - speed range reduction
    LvrtSpdRgRed = R_SpdRngRed;

    // LVRT - out of control
    LvrtOutCntrl = False;

    // count crowbar faults
    if (FAULTISACTIVE(R_CrowBarFlt) && !CroBarFltSav)  ++NV.R_CroBarCnt;
    CroBarFltSav = FAULTISACTIVE(R_CrowBarFlt);

    // keep track of total time powered up
    if ((SysTm3.Sec - PowerUpTmr) >= 60)
    {
        PowerUpTmr += 60.0F;
        ++NV.PowerUpTmAcc;
        PowerUpTmAcc = (NV.PowerUpTmAcc / 6.0F) / 10.0F;
    }
    else if (!T3CalcDone)
        PowerUpTmAcc = (NV.PowerUpTmAcc / 6.0F) / 10.0F;

    // keep track of total time running
    if (!Running && WasRunning)
        RunningTmr = SysTm3.Sec - RunningTmr;
    else if (Running && !WasRunning)
        RunningTmr = SysTm3.Sec - RunningTmr;
    else if (!SimMode && Running && ((SysTm3.Sec - RunningTmr) >= 60) )
    {
        RunningTmr += 60;
        ++NV.RunningTmAcc;
        RunningTmAcc = (NV.RunningTmAcc / 6.0F) / 10.0F;
    }
    else if (!T3CalcDone)
        RunningTmAcc = (NV.RunningTmAcc / 6.0F) / 10.0F;
    WasRunning = Running;

    // keep track of total power generated
    if (!T3CalcDone)
        PowerGenAcc = (NV.PowerGenAcc / 6.0F) / 10.0F;
    else if (!SimMode)
    {
        WattSecAccFrc = modf((WattSecAccFrc + (GmPwr * DelTm3)), (double*)&tmpf);
        WattSecAccInt += tmpf;
        if (WattSecAccInt <= -60000000)
        {
            WattSecAccInt += 60000000;
            if (PowerGenAcc > 0) --PowerGenAcc;
            PowerGenAcc = (PowerGenAcc / 6.0F) / 10.0F;
        }
        if (WattSecAccInt >= 60000000)
        {
            WattSecAccInt -= 60000000;
            ++PowerGenAcc;
            PowerGenAcc = (PowerGenAcc / 6.0F) / 10.0F;
        }
    }


    ///////////////////////////////////////////////////////////
    // Torque trimmer for grid power
    ///////////////////////////////////////////////////////////

    ///Loss calculations
    //Change to S_IRLoss and R_IRLoss
    S_IRLoss     = 1.5F*(S_IMag*S_IMag*S_Rs);//S_Rs is actually S_Rs + S_RCable
    R_IRLoss     = 1.5F*(R_IMag*R_IMag*(R_Rr + PARM(R_RISqrR)) ) + (R_VMag*R_VMag*R_RIndCorRcp);//R_RR included cable resistance
    L_IRLoss     = 1.5F*(L_IMagT3*L_IMagT3*PARM(L_RISqrR)) + (L_VMagT3*L_VMagT3*L_RIndCorRcp);//
    Gm_IRLoss    = 1.5F*GmIMagT3*GmIMagT3*PARM(Gm_RISqrR);
    DFIRLoss     = DFTocHiSel*DFTocHiSel*PARM(DFRISqrR);
    CoreLossTot  = (R_VMag*R_VMag*R_RfeRcp) + (S_VMagT3*S_VMagT3*S_RfeRcp);
    if ( PARM(PwrTrimFfDyn))
    {
        PwrTrimFF    = PARM(AuxPwrNom) + S_IRLoss + R_IRLoss + L_IRLoss + Gm_IRLoss + DFIRLoss + CoreLossTot + CoolPwrIn ;
    }
    else
    {
        PwrTrimFF    = PARM(AuxPwrNom);
    }


    //Regulator

    PwrRegErr  = PwrCmdLineT3 - GmPwrT3;

    if ( (!(R_SeqReq.R_TrqEnbReq)) || L_ZVGtOff )
    {
        PwrRegInt = 0;
    }
    else if (    ( (PwrRegErr > 0 ) && !R_InnrRegLim.R_IxCmdFrzn )       // note: using Task1 Lim bits means reg will
                 || ( (PwrRegErr < 0 ) && !R_InnrRegLim.R_IxCmdFrzp )    )  // integrate until solidly in current limit
    {
        PwrRegInt += PwrRegErr * DelTm3 * PARM(TrqTrimIntGn);
    }
    if      (PwrRegInt >  PARM(TrqTrimLim))
    {
        PwrRegInt =  PARM(TrqTrimLim);
    }
    else if (PwrRegInt < -PARM(TrqTrimLim))
    {
        PwrRegInt = -PARM(TrqTrimLim);
    }

    if ( (PARM(TestMode) != PUMP_BACK) && R_Running )
    {
        PwrRegTot = PwrRegInt + PwrTrimFF;

        TrqPwrTrim = -PwrRegTot  * R_SpdFbkT3Rc;

        // torque feedback in terms of line power

        GmTrqFbk = GmPwrT3 * R_SpdFbkT3Rc;

        GmTrqFbkFil   += PARM(GmWTrqFbkFil)  * DelTm3  * ( GmTrqFbk - GmTrqFbkFil );
    }
    else
    {
        PwrRegTot = 0.0F;
        TrqPwrTrim = 0.0F;
        GmTrqFbk = 0.0F;
        GmTrqFbkFil = 0.0F;
    }

    GmTrqErrT3 = R_TrqCmdInT3  - GmTrqFbk;


    ///////////////////////////////////////////////////////////
    // End Torque trimmer for grid power
    ///////////////////////////////////////////////////////////
}
