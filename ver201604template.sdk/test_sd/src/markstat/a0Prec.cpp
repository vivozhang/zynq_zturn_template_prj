/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        A0PREC.C30
* Language:    TI 'C'
* Copyright:   2003... GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file preconditions the regulators.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* Precondition_T1                    - precondition task 1 variables
*
* Precondition_T2                    - precondition task 2 variables
*
*==============================================================================
* 961113 SAB  Create for preconditioning.
* 021014 DS   modify for doubly-fed
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 031202 SAB  Do not pre-condition R_Ix, R_IyFbk unilt after rotor converter is
*             in stopped state
* 040712 IB   Using S_SynSwClsd in place of S_SynBrkClsd for new 60hz LVMD.
* 060125 SAB  Calculate rotor angles here when R_ZVRtrOff
* 070109 IB   Rename S_SynSwClsd as K1Status
* 080311 SAB  Remove S_IxComp
* 080313 SAB  Remove R_IxComp
* 080625 SAB  Pre-condition R_FwkInt to VregRefMax
* 090826 SAB  Remove precondition of R_SpdFstT1
* 110825 SAB  Comment out simulated angle precodition
*______________________________________________________________________________
* Last Mod: August 25, 2011 4:32:57AM
*/

#include   "x0FrameWork.h"
#include   "product.h"
#include   "a0Prec.h"

/*****************************************************************************
*
*       external references
*
*****************************************************************************/
extern CtlBt2        R_MtrCtl1;       /* motor control bit coded word 1 */
extern float         R_FlxDmpInt;
extern float         R_FlxDmpOut;
extern float         R_FlxLkgCmd;
extern float         R_FlxLkgCmdz;
extern float         R_FwkInt;
extern float         R_Ia;
extern float         R_Ib;
extern float         R_Ic;
extern float         R_IregAlmTm;
extern float         R_ISynIOCFil;
extern float         R_ITrqCmd;
extern float         R_IxCmd0;
extern float         R_IxCmd1;
extern float         R_IxCmd2;
extern float         R_IxCmd3;
extern float         R_IxCmdZ;
extern float         R_IxFbk;
extern float         R_IxFbkZ1;
extern float         R_IxFilInt;
extern float         R_IxregInt;
extern float         R_IyCmd;
extern float         R_IyCmd0;
extern float         R_IyCmd1;
extern float         R_IyCmd2;
extern float         R_IyCmd3;
extern float         R_IyCmdZ;
extern float         R_IyFbkFil;
extern float         R_IyregErr;
extern float         R_IyregInt;
extern float         R_IyregPrp;
extern float         R_ModIndFil;
extern float         R_ModIndFil2;
extern float         R_TrqCmdIn;
extern float         R_TrqErr;
extern float         R_UaCmd;
extern float         R_UbCmd;
extern float         R_UcCmd;
extern float         R_USqrFil;
extern float         R_UxCmd;
extern float         R_UyCmd;
extern float         R_VregAlmTm;
extern float         R_VxCmd;

extern float         R_VxFbkZ1;
extern float         R_VxFfCmd;
extern float         R_VxFfZ0;
extern float         R_VxFfZ1;
extern float         R_VxFfZ2;
extern float         R_VxFlx;
extern float         R_VxImpls;
extern float         R_VxReg;
extern float         R_VxregInt;
extern float         R_VyCmd;

extern float         R_VyFfCmd;
extern float         R_VyFfZ0;
extern float         R_VyFfZ1;
extern float         R_VyFfZ2;
extern float         R_VyReg;
extern float         R_VyregInt;
extern float         S_FlxXFbk;
extern float         S_FlxYFbk;
extern float         S_IxCmd;
extern float         S_IxregOut;
extern float         S_IyCmd;
extern float         S_IyregOut;
extern float         S_VregOut;
extern float         R_IxrgLimDyn;
extern float         R_IyrgLimDyn;
extern float         R_ResLimDyn;
extern float         R_DcILimDyn;
extern unsigned      R_DcILimDynZero;

extern float         S_VMaxRegGn;
extern float         R_VrgIntGnDy;
extern float         R_VrgIntLmDy;
extern float         R_DelTm1;

extern int           R_SeqSt;
extern unsigned      R_SeqStBits_PackedBits;
extern unsigned      R_SeqReqT1_PackedBits;
extern unsigned      R_SeqStatT1_PackedBits;
extern RRegLimFlgs   R_InnrRegLim;       // rotor regulator limit hit
extern unsigned      S_K1StatusT1;
extern unsigned      R_BrgPwrEnb;
extern unsigned      SimMode;
extern unsigned      S_VMaxHiEnb;
extern float         S_VMaxPuPeak;
extern unsigned      RPNWRunning;
extern float R_IxErr2Fil;
extern float R_IyErr2Fil;
extern float R_UxCmdUc;
extern float R_UyCmdUc;
extern float R_VyImpls;
extern float R_FlxLkgCmdY;
extern float R_FlxLkCmdYz;

extern float         R_IxCmd0In;
extern float         R_IxNchFilu1;
extern float         R_IxNchFilu2;
extern float         R_IxCmd0NchFl;
extern float         R_IxNchFily1;
extern float         R_IxNchFily2;

extern float         R_IyNchFilu1;
extern float         R_IyNchFilu2;
extern float         R_IyCmdNchFl;
extern float         R_IyNchFily1;
extern float         R_IyNchFily2;



/* *Vars */
CREATE_PUBVAR(R_IyCmdZ,      float);              /* Iy_Cmd delay structure output */
CREATE_PUBVAR(R_IxFbk,       float); // Ix feedback
CREATE_PUBVAR(R_VxFbk         ,       float);             /* vx feedback */
CREATE_PUBVAR(R_VyFbk         ,       float);             /* vy feedback */
CREATE_PUBVAR(R_IyFbk,       float); // iy feedback
CREATE_PUBVAR(R_TrqCal,       float);    // calculated torque

/*  Precondition Task1 function */


/* **************************************************************************
*
*                               Private Variables
*
*****************************************************************************/



/******************************************************************************
* Function:         Precondition_T1
* Execution Level:  Task 1
* Execution Time:   Not measured
*
* Formal Inputs:    None
* Formal Outputs:   None
*
* Global Inputs:    R_DelTm1
*                   R_FlxCmd
*                   R_TauRtrRcp
*                   R_TauRtrSimR
*                   R_MtrCtl1.Use_Tach
*
* Global Outputs:   R_DelTm1
*                   Flx_Adj
*                   R_FlxCmd
*                   Flx_Ref
*                   R_FlxLkgCmd
*                   R_FlxLkgCmdz
*                   Flx_Lkg_Tr
*                   Flx_Meld_Int
*                   R_FlxXSim
*                   Flx_Y_Sim
*                   R_IxCmd0
*                   R_IxCmd1
*                   R_IxCmd2
*                   R_IxCmd3
*                   R_IxCmdZ
*                   Ix_Comp
*                   Ix_Fbk
*                   R_IxFbkZ1
*                   R_IxregInt
*                   R_IxSim
*                   R_IxSim_Z1
*                   R_IxSim_Z2
*                   R_IyCmd0
*                   R_IyCmd1
*                   R_IyCmd2
*                   R_IyCmd3
*                   R_IyCmdZ
*                   R_IyFbk
*                   R_IyFbkFil
*                   R_IyregInt
*                   R_IySim
*                   R_IySim_Z1
*                   R_IySim_Z2
*                   R_ModIndFil
*                   Mod_Y_Gn
*                   Mod_Y_Int
*                   R_MtrCtl1
*                   Slreg_Int
*                   Slreg_Out
*                   Slreg_Prp
*                   (T1_Buf_B_Ptr->R_SeqReq).R_TrqEnbReq
*                   R_SeqStatT1.R_TrqEnbStat
*                   R_TrqCal
*                   R_TrqCmdIn
*                   R_TrqMtrSim
*                   R_UxCmd
*                   R_UyCmd
*                   R_VabSim
*                   R_VbcSim
*                   R_VxFfZ0
*                   R_VxFfZ1
*                   R_VxFfZ2
*                   R_VxReg
*                   R_VxregInt
*                   R_VxFbk
*                   R_VxFbkZ1
*                   R_VxFlx
*                   R_VxImpls
*                   R_VxSim
*                   R_VyReg
*                   R_VyregInt
*                   R_VyCmd
*                   R_VyFbk
*                   R_VyFfZ0
*                   R_VyFfZ1
*                   R_VyFfZ2
*                   R_VySim
*                   R_WElecFil
*                   R_WElecFrq_Ch
*
* Return Value:
* Function Calls:   None
*
* Abstract:         Preconditions storage element vars when R_InnrRegEnb not
*                   set.
*
******************************************************************************/


void RotorPrecondition_T1()

{
    unsigned    *Struct_Prec;     /* temp variable used to precondition structures */

    /************************************************************************/
    /************************************************************************/
    /*   Important: do not insert any precond. from here down to Smp2Encod  */
    /*   Smp2Encod is execution time dependent module                       */
    /************************************************************************/
    /************************************************************************/

    /*  limit words */

    Struct_Prec = (unsigned*) &R_InnrRegLim; /* Set pointer to address of R_InnrRegLim */
    *Struct_Prec = 0;

    /*
     * Precondition the inputs to the Modulator().
     */
    R_UaCmd = R_UbCmd = R_UcCmd = 0;


    // Tach loss dectectio

    R_TrqCal      = 0.0;
    R_TrqCmdIn   = 0.0;
    R_ITrqCmd    = 0.0;
    R_TrqErr      = 0.0;

    // Primary Feedbacks
    if (R_SeqStBits.R_Stopped)
    {
       R_IxFbk       = 0.0;
       R_IyFbk       = 0.0;
    }

    R_IxFbkZ1    = R_IxFbk;


    if(SimMode)
    {
        R_VxFbk = 0.0;
        R_VyFbk = 0.0;
    }
    R_VxFbkZ1    = R_VxFbk;

    //X-Channel Current Command
    R_IxCmd0     = 0.0;
    R_IxCmd1     = 0.0;
    R_IxCmd2     = 0.0;
    R_IxCmd3     = 0.0;
    R_IxCmdZ     = 0.0;

    // Y-Channel Current Command
    R_IyCmd1     = 0.0;
    R_IyCmd2     = 0.0;
    R_IyCmd3     = 0.0;
    R_IyCmdZ     = 0.0;

    // Rotor Ix Current Commnand Notch Filter
    R_IxCmd0In        = 0.0;
    R_IxNchFilu1      = 0.0;
    R_IxNchFilu2      = 0.0;
    R_IxCmd0NchFl     = 0.0;
    R_IxNchFily1      = 0.0;
    R_IxNchFily2      = 0.0;


    // Stator items
    S_IxCmd     = 0.0;
    S_IyCmd     = 0.0;
    S_IxregOut  = 0.0;
    S_IyregOut  = 0.0;
    S_VregOut   = 0.0;

    // Set R_TrqEnbStat to value of Request from sequencer during precond
    // This code is part of the Trq command enable done in IxTrqCmd
    R_SeqStatT1.R_TrqEnbStat = (R_SeqReqT1).R_TrqEnbReq;

    //Filtered Flux Current Command

    R_IyFbkFil   = 0.0;
                                    // X-Y Current Regulator
    R_IxregInt    = 0.0;
    R_IxErr2Fil   = 0.0;
    R_IyErr2Fil   = 0.0;
    R_IyregErr    = 0.0;
    R_IyregInt    = 0.0;
    R_IyregPrp    = 0.0;

                                    // Voltage Regulation and Modulation Index square
    R_UxCmd       = 0.0;
    R_UxCmdUc     = 0.0;
    R_UyCmd       = 0.0;
    R_UyCmdUc     = 0.0;
    R_USqrFil     = 0.0;
    R_VxReg       = 0.0;
    R_VxregInt    = 0.0;
    R_VyReg       = 0.0;
    R_VyregInt    = 0.0;

                                    // Voltage Feed-forward Model
    R_VxCmd       = 0.0;
    R_VxFfCmd    = 0.0;
    R_VxFlx       = 0.0;
    R_VxImpls     = 0.0;
    R_VyImpls     = 0.0;

    R_VyCmd       = 0.0;
    R_VyFfCmd    = 0.0;

    R_FlxLkgCmd  = 0.0;
    R_FlxLkgCmdY = 0.0;
    R_FlxLkgCmdz = 0.0;
    R_FlxLkCmdYz = 0.0;

    if ( !RPNWRunning) 
    {
       S_VMaxPuPeak = 0.0;
       S_VMaxHiEnb  = false; 
    }



    /************************************************************************/
    /************************************************************************/
    /*   Important: do not insert any precondition above this line          */
    /*   Smp2Encod is execution time dependent module                       */
    /*   Any future precondition code must be added after this line         */
    /************************************************************************/
    /************************************************************************/



    R_VxFfZ0     = 0.0;
    R_VxFfZ1     = 0.0;
    R_VxFfZ2     = 0.0;
    R_VyFfZ0     = 0.0;
    R_VyFfZ1     = 0.0;
    R_VyFfZ2     = 0.0;


                           /*  Motor Model */
    // Motor model flux damping
    R_FlxDmpInt  = 0.0;
    R_FlxDmpOut  = 0.0;

    R_IxrgLimDyn = PARM(R_IxregLim);
    R_IyrgLimDyn = PARM(R_IyregLim);
    R_ResLimDyn  = PARM(R_IregResLim);
    R_DcILimDyn  = PARM(R_IregDcLim);
    R_DcILimDynZero  = false;
    S_VMaxRegGn  = 1.0;

    R_VrgIntGnDy = PARM(R_VxregIntGn)*R_DelTm1;
    R_VrgIntLmDy = PARM(R_VxregLim);


     /* ============= Speed =================== */

#if 0
     if ( SimMode == true)
        R_WRtr = (R_SpdSimT1)*PARM(R_PolePairs);
     else
        R_WRtr = R_Spd*PARM(R_PolePairs);

    /* ======== Precondition frequencies and Angles ===== */

        R_WElecFf  =  0;
        R_WElecFil =  0;

        if ( R_ZVRtrOff)
        {
            if ( SimMode)
            {
               /* rotor position feedback */
               R_RtrPosn += R_DelTm1*R_WRtr;
               R_RtrPosn = Modulo2Pi(R_RtrPosn);
            }
            else
            {
               R_RtrPosn = R_TachPosn;
            }

            R_AngTach    = R_RtrPosn - R_TachZero;
            R_AngElecSta = S_AngElecFbk + PARM(S_VllFbkLag);
            R_AngElecFbk = Modulo2Pi(R_AngElecSta - R_AngTach + (4.0*PI) - R_AngSlwCor);
            R_AngElecCmd = R_AngElecFbk + R_DelTm1 * R_WElec;


        }
        else
        {
           R_WElec    =  0;
           R_AngElecFbk = 0;
           R_AngElecCmd = 0;
        }

#endif

     if (!S_K1StatusT1)
     {
        S_FlxXFbk = 0.0;
        S_FlxYFbk = 0.0;
     }

     R_ISynIOCFil = 0.0;
}


/******************************************************************************
* Function:         Precondition_T2
* Execution Level:  Task 2
* Execution Time:   Not measured
*
* Formal Inputs:    None
* Formal Outputs:   None
*
* Global Inputs:    None
*
*
* Global Outputs:   R_SpdSim
*                   R_TauRtrSimR
*                   Iy_Ss
*
* Return Value:
* Function Calls:   None
*
* Abstract:         Preconditions storage element vars when R_InnrRegEnb not
*                   set.
*
******************************************************************************/
void RotorPrecondition_T2()
{


    R_IyCmd      = 0.0;
    R_IyCmd0     = 0.0;

    // Rotor Iy Current Commnand Notch Filter
    R_IyNchFilu1     = 0.0;
    R_IyNchFilu2     = 0.0;
    R_IyCmdNchFl     = 0.0;
    R_IyNchFily1     = 0.0;
    R_IyNchFily2     = 0.0;

                           /* Field-Weakening Controller */
    R_FwkInt     = PARM(VregRefMax);
    R_ModIndFil  = 0.0;
    R_ModIndFil2 = 0.0;

    if ( (R_SeqSt == 0) || ( (R_SeqSt == 13) && (!R_BrgPwrEnb) ) )
    // 0  STOPPED, 13 SPD_REG_TUNE
    {
        R_MtrCtl1.R_FlxUp = false;
    }

                          /* Current mag limit */
    R_IxFilInt = 0.0;

                           /* Inner reg alarm timer */
    R_IregAlmTm = 0.0;
    R_VregAlmTm = 0.0;

}
