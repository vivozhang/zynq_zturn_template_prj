///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rotor x-y current regulator.
//
// DESCRIPTION:
//      Functions associated with current regulation.
//
// COPYRIGHT:   Copyright (c) 2014
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     a0Ireg.vsd, a0Ireg.xls
//
///////////////////////////////////////////////////////////////////////////////

// Includes Files
//---------------

// system
// framework
#include   "x0FrameWork.h"
// product
#include   "Product.h"
#include   "a0Ireg.h"

/*****************************************************************************
*
*       external references
*
*****************************************************************************/
extern float     R_DcXStep;
extern float     R_DcYStep;
extern float     R_DelTm1;
extern float     R_IxCmdP;
extern float     R_IxCmdZ;
extern float     R_IxFbk;
extern float     R_IxFbkComp;
extern float     R_IyCmd0T1;
extern float     R_IyCmd2;
extern float     R_IyCmd3;
extern float     R_IyCmdZ;
extern float     R_IyFbk;
extern float     R_IyFbkComp;
extern float     R_IyVdc;
extern float     R_NsXStep;
extern float     R_NsYStep;
extern float     R_VxFbk;
extern float     R_VyFbk;
extern float     R_WElec;
extern float     S_Cos2Ang;
extern float     S_CosAngFbk;
extern float     S_Sin2Ang;
extern float     S_SinAngFbk;
extern float     S_VMaxIyCmd;
extern float     S_1PlusLsLm;
extern float     S_IyBase;
extern float     XfmrRatioLnStr;
extern unsigned  S_K1Status;
extern unsigned  S_K1StatusT1;
extern unsigned  R_InnrRegEnb;
extern unsigned  SimMode;
extern unsigned  S_VMaxIyFast;
extern float     ISysCmdSF;

extern float     TurbNpRatRcp;
extern float     PwrCmdLineT3;

// Constants
//----------


// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Parameters
//-----------
CREATE_PARM(R_IregPGn2      ,float);    // Current regulator proportional gain 2
CREATE_PARM(R_IregErr2W     ,float);    // Current regulator current error2 2 filter response
CREATE_PARM(R_IyregPrpGn    ,float);    // y-current regulator proportional gain
CREATE_PARM(R_IyregIntGn    ,float);    // y-current regulator integral gain
CREATE_PARM(R_IxregPrpGn    ,float);    // x-current regulator proportional gain
CREATE_PARM(R_IxregIntGn    ,float);    // x-current regulator integral gain
CREATE_PARM(R_Res2ndD       ,float);
CREATE_PARM(R_Res2ndQ       ,float);
CREATE_PARM(R_IregResWI     ,float);
CREATE_PARM(R_Dc1stD        ,float);
CREATE_PARM(R_Dc1stQ        ,float);
CREATE_PARM(R_DcRegWI       ,float);
CREATE_PARM(R_DcRegIntW     ,float);

CREATE_PARM(R_IyregIRtio    ,float);    // rotor y-current regulator ratio to x-current regulator integral gain
CREATE_PARM(R_IyregPRtio    ,float);    // rotor y-current regulator ratio to x-current regulator proportional gain
CREATE_PARM(R_IyregPRto0    ,float);    // rotor y-current regulator ratio to x-current regulator proportional gain at min speed
CREATE_PARM(R_IyregPSpd0    ,float);    // rotor y-current regulator speed at which the dynamic ratio is equal to R_IyregPRto0
CREATE_PARM(R_IyregPSpd     ,float);    // rotor y-current regulator speed at which the dynamic ratio is equal to R_IyregPRto
CREATE_PARM(R_CPmpBk        ,float);  /* Rotor pumpback cap (2*65 kVARs) */

CREATE_PARM(R_IregResBW,        float);     // 2nd harmonic ireg items
CREATE_PARM(R_IregResMax,       float);
CREATE_PARM(R_IregResLim,       float);
CREATE_PARM(R_NsXStpAdr,        float*);
CREATE_PARM(R_NsYStpAdr,        float*);
CREATE_PARM(R_DcIregBW,         float);     // dc ireg items
CREATE_PARM(R_DcIregMaxDynEnb,  unsigned);
CREATE_PARM(R_DcIregMax,        float);
CREATE_PARM(R_DcIregMax2,       float);
CREATE_PARM(R_IregDcLim,        float);
CREATE_PARM(R_IregDcLim2,       float);
CREATE_PARM(R_DcIregMax1Pwr,    float);
CREATE_PARM(R_DcIregMax2Pwr,    float);
CREATE_PARM(R_DcXStpAdr,        float*);
CREATE_PARM(R_DcYStpAdr,        float*);
CREATE_PARM(R_IxregLim,         float);         // x-current regulator integral limit
CREATE_PARM(R_IyregLim,         float);          // y-current regulator integral limit

// Variables
//----------
CREATE_PUBVAR(PwrCmdLinePu  , float);   
CREATE_PUBVAR(R_DcIregMaxSlope, float);
CREATE_PUBVAR(R_DcFndX      , float);
CREATE_PUBVAR(R_DcFndXT1    , float);    /* DC current regulator  y axis roator input */
CREATE_PUBVAR(R_DcFndY      , float);
CREATE_PUBVAR(R_DcFndYT1    , float);    /* DC current regulator  y axis roator input */
CREATE_PUBVAR(R_DcXOut      , float);
CREATE_PUBVAR(R_DcYOut      , float);
CREATE_PUBVAR(R_IxErr2Fil   , float);    /* x-component lead-lag filter output*/
CREATE_PUBVAR(R_IxregDcOut  , float);
CREATE_PUBVAR(R_IxregDcX    , float);
CREATE_PUBVAR(R_IxregErr    , float);    /* x-current reg error */
CREATE_PUBVAR(R_IxregErrT2  , float);    /* x-current reg error in task2 */
CREATE_PUBVAR(R_IxregInt    , float);    /* x-current regulator integrator */
CREATE_PUBVAR(R_IxregPErr   , float);    /* x-component proportional error*/
CREATE_PUBVAR(R_IxregPrp2   , float);    /* x-component lead-lag proportional output*/
CREATE_PUBVAR(R_IxregResX   , float);
CREATE_PUBVAR(R_IxregRsOut  , float);
CREATE_PUBVAR(R_IyErr2Fil   , float);    /* y-component lead-lag filter output*/
CREATE_PUBVAR(R_IyregDcOut  , float);
CREATE_PUBVAR(R_IyregDcY    , float);
CREATE_PUBVAR(R_IyregErr    , float);    /* y-current reg error */
CREATE_PUBVAR(R_IyregInt    , float);    /* y-current regulator integrator */
CREATE_PUBVAR(R_IyregPErr   , float);    /* y-component proportional error*/
CREATE_PUBVAR(R_IyregPrp    , float);    /* y-current regulator Proportional */
CREATE_PUBVAR(R_IyregPGnDy  ,float);    // y-current regulator proportional dynamic gain
CREATE_PUBVAR(R_IyregPrp2   , float);    /* y-component lead-lag proportional output*/
CREATE_PUBVAR(R_IyregResY   , float);
CREATE_PUBVAR(R_IyregRsOut  , float);
CREATE_PUBVAR(R_ResIx2      , float);    /* x-channel current err, demod by 2*Wsta */
CREATE_PUBVAR(R_ResIy2      , float);    /* y-channel current err, demod by 2*Wsta */
CREATE_PUBVAR(R_VxReg       , float);    /* x-component reg */
CREATE_PUBVAR(R_VyReg       , float);    /* y-component reg */
CREATE_PUBVAR(S_DcIx        , float);    /* x-channel current err, demod by Wsta */
CREATE_PUBVAR(S_DcIy        , float);    /* y-channel current err, demod by Wsta */
CREATE_PUBVAR(R_DcRegIntWdt , float);    /* dc current regulator integrator W*dt */
CREATE_PUBVAR(R_DcRegLimK    , float);   /* dc current regulator output constant for clamping */
CREATE_PUBVAR(R_IregDcMag    , float);   /* dc current regulator maghitude of inegrators */



CREATE_PUBVAR(R_IxrgLimDyn  , float);    /* x-component regulator dynamic limit*/
CREATE_PUBVAR(R_IyrgLimDyn  , float);    /* y-component regulator dynamic limit*/
CREATE_PUBVAR(R_ResLimDyn   , float);
CREATE_PUBVAR(R_DcILimDyn   , float);
CREATE_PUBVAR(R_DcILimDynT3 , float);
CREATE_PUBVAR(R_DcILimDynZero, unsigned);
CREATE_PUBVAR(R_IregErr2Gn  ,float);

CREATE_PUBVAR(R_IyCmd1,        float );              /* y-component current command delayed one */

/* XYCurrentReg procedure */

/*=============================================================================
 * Function: XYCurrentReg
 * Execution Level: Task 1
 * Execution Time: 5.60usec     12/19/96--Rev:001.01.01.A
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  R_DelTm1
 *                 R_IxCmdP
 *                 R_IxCmdZ
 *                 R_IxFbk
 *                 R_IyFbk
 *                 R_WElecFil
 *                 R_MtrCtl1.R_DchgIxInt
 *                 R_MtrCtl1.R_DchgIyInt
 *                 R_MtrCtl1.R_UseModGn
 *                 R_MtrCtl1.Use_Y_Int
 *
 *                 CI_R_IxregIntGn
 *                 CI_R_IyregIntGn
 *                 CI_R_IxregPrpGn
 *                 CI_R_IyregPrpGn
 *                 CI_Mod_Brk_Rcp
 *                 CI_R_IxregLim
 *                 CI_R_IyregLim
 *                 CI_Slreg_Lim
 *                 CI_Slreg_Int_Gn
 *                 CI_Slreg_Prp_Gn
 *
 *
 *
 *
 * Global Outputs: Slreg_Out
 *                 R_VxReg
 *                 R_VyReg
 *
 * Locals:         R_IxregInt
 *                 R_IyregInt
 *                 Mod_Y_Gn
 *                 Mod_Y_Int
 *                 Slreg_Int
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function performs the regulation of x-y current.
 *
 *===========================================================================*/

void RotorXYCurrentReg()
{
     float Res_X2, Res_Y2;
     float Res_X_Out, Res_Y_Out;

     float R_IxFbkPrp, R_IyFbkPrp;
     float R_IxFbkInt, R_IyFbkInt;
     float R_DcRegLimKTemp;


        R_IxFbkInt = R_IxFbkPrp = R_IxFbk-R_IxFbkComp;
        R_IyFbkInt = R_IyFbkPrp = R_IyFbk-R_IyFbkComp;
        if ( ( PARM(TestMode) == PUMP_BACK)  && !S_K1Status )
        {
             /* Compensate current feedbacks for filter currents */
            R_IxFbkPrp = R_IxFbk;
            R_IyFbkPrp = R_IyFbk;
            R_IxFbkPrp += (PARM(R_CPmpBk) * R_WElec) * R_VyFbk;
            R_IyFbkPrp -= (PARM(R_CPmpBk) * R_WElec) * R_VxFbk;
            R_IxFbkInt = R_IxCmdZ;
            R_IyFbkInt = R_IyCmdZ;
        }

        /*  ================= Ix regulator - TRQ_030 ====== */

        R_IxregErr = R_IxCmdZ - R_IxFbkInt;

        R_IxregInt = R_IxregInt + R_DelTm1*PARM(R_IxregIntGn)*R_IxregErr;
        /*  clamp R_IxregInt */
        if ( R_IxregInt > R_IxrgLimDyn)
        {
            R_IxregInt = R_IxrgLimDyn;
        }
        else if (R_IxregInt < -R_IxrgLimDyn)
        {
            R_IxregInt = -R_IxrgLimDyn;
        }

        R_IxregPErr = R_IxCmdP - R_IxFbkPrp;
        R_IxErr2Fil = R_IxErr2Fil + R_IregErr2Gn *(R_IxregPErr - R_IxErr2Fil);
        R_IxregPrp2 = R_IxErr2Fil * PARM(R_IregPGn2);

        R_VxReg = R_IxregInt + (PARM(R_IxregPrpGn)*R_IxregPErr) + R_IxregPrp2;

        /*  clamp R_VxReg */
        if ( R_VxReg > R_IxrgLimDyn)
        {
            R_VxReg = R_IxrgLimDyn;
        }
        else if (R_VxReg < -R_IxrgLimDyn)
        {
            R_VxReg = -R_IxrgLimDyn;
        }

        /*  End Ix regulator */



 /*==========================================================================
                          Iy command delay structure -- TRQ_025
   ========================================================================== */

     R_IyCmd3 = R_IyCmd2;
     R_IyCmd2 = R_IyCmd1;

     if ( S_VMaxIyFast)
     {
        R_IyCmd1 = PARM(S_RRatio) * ISysCmdSF * (S_VMaxIyCmd * S_1PlusLsLm  + S_IyBase );
     }
     else
     {
        R_IyCmd1 = R_IyCmd0T1;
     }

     R_IyCmdZ = 0.5*(R_IyCmd3 + R_IyCmd2);


    /* =============== Iy regulator -- TRQ_030 ==== */

    R_IyregErr = R_IyCmdZ - R_IyFbkInt;

    R_IyregInt = R_IyregInt + R_DelTm1 * PARM(R_IyregIntGn) * R_IyregErr;
    /*  Clamp R_IyregInt */
    if ( R_IyregInt > R_IyrgLimDyn)
    {
        R_IyregInt = R_IyrgLimDyn;
    }
    else if ( R_IyregInt < -R_IyrgLimDyn)
    {
        R_IyregInt = -R_IyrgLimDyn;
    }

    R_IyregPErr = R_IyCmd1 - R_IyFbkPrp;
    R_IyregPrp  = R_IyregPGnDy * R_IyregPErr;
    R_IyErr2Fil = R_IyErr2Fil + R_IregErr2Gn *(R_IyregPErr - R_IyErr2Fil);
    R_IyregPrp2 = R_IyErr2Fil * PARM(R_IregPGn2);

    R_VyReg = R_IyregPrp + R_IyregInt + R_IyregPrp2;
    /*  clamp R_VyReg */
    if ( R_VyReg > R_IyrgLimDyn)
    {
        R_VyReg = R_IyrgLimDyn;
    }
    else if (R_VyReg < -R_IyrgLimDyn)
    {
        R_VyReg = -R_IyrgLimDyn;
    }

    /*  End Iy regulator */

     /*************************************************************************/
     /*                              Resonator                                 */
     /*************************************************************************/

     if (S_K1Status && R_InnrRegEnb )
     {

         /****************************************/
         /*            Demodulation              */
         /****************************************/
         R_ResIx2 = S_Cos2Ang*R_IxregErr - S_Sin2Ang*R_IyregErr;
         R_ResIy2 = S_Sin2Ang*R_IxregErr + S_Cos2Ang*R_IyregErr;

         /****************************************/
         /*    Current Integration and Clamp     */
         /****************************************/
         R_IxregResX = R_IxregResX + R_DelTm1*R_ResIx2;
         if ( R_IxregResX > R_ResLimDyn)
         {
              R_IxregResX = R_ResLimDyn;
         }
         if ( R_IxregResX < -R_ResLimDyn)
         {
              R_IxregResX = -R_ResLimDyn;
         }

         R_IyregResY = R_IyregResY + R_DelTm1*R_ResIy2;
         if ( R_IyregResY > R_ResLimDyn)
         {
              R_IyregResY = R_ResLimDyn;
         }
         if ( R_IyregResY < -R_ResLimDyn)
         {
              R_IyregResY = -R_ResLimDyn;
         }

         /****************************************/
         /*          Rotator Input Calculation   */
         /****************************************/
         Res_X2 = PARM(R_Res2ndD)*(R_IxregResX + R_NsXStep) + PARM(R_Res2ndQ)*(R_IyregResY + R_NsYStep);
         Res_Y2 = PARM(R_Res2ndD)*(R_IyregResY + R_NsYStep) - PARM(R_Res2ndQ)*(R_IxregResX + R_NsXStep);

         /****************************************/
         /*         Rotation to Output           */
         /****************************************/
         Res_X_Out = S_Cos2Ang*(Res_X2)  + S_Sin2Ang*(Res_Y2);
         Res_Y_Out = -S_Sin2Ang*(Res_X2) + S_Cos2Ang*(Res_Y2);

         /****************************************/
         /*        Final Resonator Output        */
         /****************************************/
         R_IxregRsOut = PARM(R_IregResWI)*Res_X_Out;
         R_IyregRsOut = PARM(R_IregResWI)*Res_Y_Out;



         /**********************************************************************/
         /*       reverse Fundamental, for DC (in ac line) Demodulation        */
         /**********************************************************************/
         S_DcIx      = S_CosAngFbk*R_IxregErr - S_SinAngFbk*R_IyregErr;
         S_DcIy      = S_SinAngFbk*R_IxregErr + S_CosAngFbk*R_IyregErr;

         /****************************************/
         /*    Current Integration and Clamp     */
         /****************************************/         
                                                   
         R_IxregDcX = R_IxregDcX + ( (R_DelTm1*S_DcIx) - (R_DcRegIntWdt*R_IxregDcX) );
         R_IyregDcY = R_IyregDcY + ( (R_DelTm1*S_DcIy) - (R_DcRegIntWdt*R_IyregDcY) );

         /*********************************************/
         /* Clamp Calculation by output scale factor  */
         /*********************************************/
         if ( R_DcILimDynZero) 
         {
             R_DcILimDyn = 0.0;
         }
         else
         {
             R_DcILimDyn = R_DcILimDynT3;
         }

         R_IregDcMag     = MAX( sqrtf(R_IxregDcX*R_IxregDcX +  R_IyregDcY*R_IyregDcY), 0.0001);
         R_DcRegLimKTemp = R_DcILimDyn/R_IregDcMag; 
         if ( R_DcRegLimKTemp > 1.0F) 
         {
             R_DcRegLimKTemp = 1.0F;    
         }
         else if ( R_DcRegLimKTemp < 0.0F) 
         {
             R_DcRegLimKTemp = 0.0F;    
         }
         R_DcRegLimK = R_DcRegLimKTemp; 


         /****************************************/
         /*          Rotator Input Calculation   */
         /****************************************/
         R_DcFndX = PARM(R_Dc1stD)*(R_IxregDcX + R_DcXStep) + PARM(R_Dc1stQ)*(R_IyregDcY + R_DcYStep);
         R_DcFndY = PARM(R_Dc1stD)*(R_IyregDcY + R_DcYStep) - PARM(R_Dc1stQ)*(R_IxregDcX + R_DcXStep);


         R_DcFndXT1 = R_DcFndX;
         R_DcFndYT1 = R_DcFndY;


         /****************************************/
         /*         Rotation to Output           */
         /****************************************/
         R_DcXOut =  S_CosAngFbk*R_DcFndXT1 + S_SinAngFbk*R_DcFndYT1;
         R_DcYOut = -S_SinAngFbk*R_DcFndXT1 + S_CosAngFbk*R_DcFndYT1;

         /****************************************/
         /*        Final Fundamental Output        */
         /****************************************/
         R_IxregDcOut = PARM(R_DcRegWI) * R_DcRegLimK * R_DcXOut;
         R_IyregDcOut = PARM(R_DcRegWI) * R_DcRegLimK * R_DcYOut;

     }
     else
     {
         R_ResIx2     =  0.0;
         R_ResIy2     =  0.0;
         R_IxregResX  =  0.0;
         R_IyregResY  =  0.0;
         Res_X2       =  0.0;
         Res_Y2       =  0.0;
         Res_X_Out    =  0.0;
         Res_Y_Out    =  0.0;
         R_IxregRsOut =  0.0;
         R_IyregRsOut =  0.0;

         //Remove the following six lines if T2 scheme below is used.
#ifndef STATOR_DCREG_RAF
         R_IxregDcX   =  0.0;
         R_IyregDcY   =  0.0;
         R_DcFndXT1   =  0.0;
         R_DcFndYT1   =  0.0;
         R_IxregDcOut =  0.0;
         R_IyregDcOut =  0.0;
#endif

     }
}


/*=============================================================================
 * Function: XYCurrentRegT3
 * Execution Level: Task 3
 * Execution Time: N/A
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  PwrCmdLineT3 
 *                 TurbNpRatRcp
 *
 *
 * Global Outputs: PwrCmdLinePu
 *
 * Locals:       
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function performs task 3 work in behalf of the rotor current regulator 
 *
 *===========================================================================*/

void RotorXYCurrentRegT3()
{
    PwrCmdLinePu = PwrCmdLineT3 * TurbNpRatRcp; 

    if ( PwrCmdLinePu <= PARM(R_DcIregMax1Pwr) ) 
    {
        R_DcILimDynT3 = PARM(R_IregDcLim);
    }
    else if ( PwrCmdLinePu >= PARM(R_DcIregMax2Pwr) )
    {
        R_DcILimDynT3 = PARM(R_IregDcLim2);
    }
    else 
    {
        R_DcILimDynT3 = PARM(R_IregDcLim) + R_DcIregMaxSlope * (PwrCmdLinePu - PARM(R_DcIregMax1Pwr) ); 
    }


}
