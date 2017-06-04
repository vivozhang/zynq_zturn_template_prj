///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Source voltage regulator
//
// DESCRIPTION:
//      This file contains rule calculations for source control
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     This file contains functions associated with source voltage regulator.
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "p2Sseq.h"


// Constants
//----------


// Externals
//----------
extern float        DelTm2;
extern float        DelTm3;
extern float        GmDscmVMgNPu;
extern float        L_DcFndXT3;
extern float        L_DcFndYT3;
extern float        L_DelTm1;
extern float        L_IregXUcT3;
extern float        L_IregYUcT3;
extern float        L_ResX2T3;
extern float        L_ResY2T3;
extern float        L_VdcFbkT3;
extern float        L_VdcReg;
extern float        L_VMagT2;
extern float        L_VregGnRcp;
extern float        L_VregStep;
extern float        L_VregWoComp;
extern float        R_UxCmdT3;
extern float        R_UyCmdT3;
extern float        R_IxCmdZ;
extern float        R_FwkDel2;
extern int          L_SeqSt;
extern RegLimFlgs   L_InnrRegLim;
extern unsigned     L_Running;
extern unsigned     R_RunReq;
extern unsigned     RPNWRunning;
extern unsigned     ZVRTIp;
extern unsigned     L_InnrRegEnb;


// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(R_VdcMdUpThr,       float);
CREATE_PARM(L_VdcVmagUpRt,      float);
CREATE_PARM(L_VdcTranMrg,       float);     // L_Vdc transient margin
// VoltReg
CREATE_PARM(L_VregBw,           float);     // voltage regulator bandwidth
CREATE_PARM(L_VregIntGn,        float);     // voltage regulator integral gain
CREATE_PARM(L_VregPrpGn,        float);     // voltage regulator proportional gain
CREATE_PARM(L_VregWoGn,         float);     // Wo for voltage regulator
// VoltageReferenceT2()
CREATE_PARM(L_VdcRefLvrt,       float);     // Low voltage ride through vdc ref supression value
CREATE_PARM(L_VdcVmgLvrt,       float);     // Low voltage ride through Vmag value to supress vref
CREATE_PARM(L_VdcVMgNThr,       float);     // 2nd harmonic per unit required to reduduce link ref to L_VdcRefLvrt
CREATE_PARM(L_VdcVmagDnRt,      float);     // Grid voltage low boost value to increase vref
// VoltageReference()
CREATE_PARM(L_VdcRef,           float);     // dc voltage reference
CREATE_PARM(L_VdcSlw,           float);     // dc voltage reference slew rate
CREATE_PARM(L_IxLimP,           float);     // volt regulator positive current limit
CREATE_PARM(L_IxLimN,           float);     // volt regulator negative current limit
CREATE_PARM(L_VdcModRef,        float);     // Adaptive voltage regulator reference
CREATE_PARM(R_VdcModRef,        float);     // Rotor side dc bus modulation index reference.
CREATE_PARM(R_VdcModGn,         float);     // Rotor side dc bus modulation  reference gain
CREATE_PARM(L_VdcRefWUp,        float);     // Adaptive voltage regulator ref. upward gain
CREATE_PARM(L_VdcRefWDn,        float);     // Adaptive voltage regulator ref. downward gain
CREATE_PARM(L_VdcRefMax,        float);     // Adaptive voltage regulator ref. max. integrator value
CREATE_PARM(L_VdcRefMin,        float);     // Adaptive voltage regulator ref. min. integrator value
CREATE_PARM(L_VdcRefMode,       unsigned);  // Voltage regulator reference mode
CREATE_PARM(L_VrefBwUp,         float);     // Voltage reference band-width up
CREATE_PARM(L_VrefBwDn,         float);     // Voltage reference band-width down
CREATE_PARM(L_VrefLmHyst,       float);     // Voltage ref in max limit hysterisis
CREATE_PARM(L_DCInitThr,        float);     // DC link initialization threshold
CREATE_PARM(L_DCInitTm,         float);     // DC link initialization time
CREATE_PARM(L_DCInitZVTm,       float);     // DC link initialization for zero voltage ride-through time
CREATE_PARM(L_VrgStpAdr,        float*);    // Dc voltaeg regulator step address

// Variables
//----------
CREATE_PUBVAR(L_DCInitTmr,    float);      // Init timer
CREATE_PUBVAR(L_DelVdcRf,     float);      // Reference delta used during slew up/down
CREATE_PUBVAR(L_DelVdcRfT3,   float);      // Task3 delta for vdc ramp-up/down
CREATE_PUBVAR(L_IxCmd,        float);      // x-component current command before ff
CREATE_PUBVAR(L_ModIndDc,     float);      // Line conv. mod. index for dc regulator
CREATE_PUBVAR(L_ModIndFbk,    float);      // Adaptive volt ref. mod. index feedback
CREATE_PUBVAR(L_ModIndPeak,   float);      // Total calc                     'd modulation indx
CREATE_PUBVAR(L_ModIndReg,    float);      // Ireg portion of calc                     'd modulation indx
CREATE_PUBVAR(L_ModIndRes,    float);      // Resonator portion of calc                     'd mod indx
CREATE_PUBVAR(L_SlewMax,      float);      // Max reference slew per dt
CREATE_PUBVAR(L_VdcBstVmag,   float);      // Vdc boost from low or high grid voltage mag
CREATE_PUBVAR(L_VdcMdRfRcp,   float);      // Reciprocal of L_VdcModRef
CREATE_PUBVAR(L_VdcModErr,    float);      // Adaptive volt ref. modulation error
CREATE_PUBVAR(L_VdcSetPt,     float);      // New voltage reference setpoint -
CREATE_PUBVAR(L_VdcStptInt,   float);      // Adaptive volt ref. integrator
CREATE_PUBVAR(L_VdcTranDel,   float);      // L_Vdc transient delta
CREATE_PUBVAR(L_VdcTranErr,   float);      // L_Vdc transient error
CREATE_PUBVAR(L_VregDelInt,   float);      // voltage regulator delta for integrator
CREATE_PUBVAR(L_VregErr,      float);      // voltage regulator error
CREATE_PUBVAR(L_VregErrT3,    float);      // Task 3 voltage regulator error
CREATE_PUBVAR(L_VregIxInt,    float);      // voltage regulator L_Ix integrator
CREATE_PUBVAR(L_VregRef,      float);      // voltage regulator reference
CREATE_PUBVAR(R_ModIndFbk,    float);      // Rotor conv. mod. index feedback
CREATE_PUBVAR(R_ModIndReg,    float);      // Rotor conv. total modulation indx
CREATE_PUBVAR(R_VdcModErr,    float);      // Adaptive volt ref. mod. index error --gen
CREATE_PUBVAR(L_SlwCnt3,      int);
CREATE_PUBVAR(L_DCLinkInit,   unsigned);   // The dc link has completed initial ramp
CREATE_PUBVAR(L_VoltsDec,     unsigned);   // L_Vdc reference decrement command
CREATE_PUBVAR(L_VoltsInc,     unsigned);   // L_Vdc reference increment command
CREATE_PUBVAR(L_VrefInMaxL,   unsigned);   // Voltage reference in max limit.


// Unpublished Variables
//----------------------


// Data Passing
//--------------


// Diagnostics
//------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//     Voltage regulator implmentation and voltage feed-forward model.
//     It also makes compensation to the output limit function for
//     the case of ac line transients.  A two stage FIR filter is applied to
//     the Dc link voltage feedback.  A reference for the dc link voltage is
//     taken from task3.  When the regulator is not running, the reference is
//     pre-conditioned with the present output of the FIR filter.
//     VoltReg   -- Diagram Line/ALG_015.vsd
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void VoltReg(void)
{
    float   Vreg_Ix_Prp, Del_Vdc_Tmp;

    // Begin Voltage reference code

    Del_Vdc_Tmp = (L_VdcSetPt) - L_VregRef;
    L_DelVdcRf = Del_Vdc_Tmp;
    if ( Del_Vdc_Tmp >= L_SlewMax)
    {
       Del_Vdc_Tmp = L_SlewMax;
    }
    else if (Del_Vdc_Tmp <= -L_SlewMax)
    {
       Del_Vdc_Tmp = -L_SlewMax;
    }

    L_VregRef = L_VregRef + Del_Vdc_Tmp;

    L_VdcTranErr = (PARM(L_IxLimP) - L_VregIxInt)*L_VregGnRcp;
    if ( L_VdcTranErr <= 0.0F)
    {
         L_VdcTranErr = 0.0F;
    }
    L_VdcTranDel = L_VdcTranErr + PARM(L_VdcTranMrg);
    if ( L_InnrRegLim.L_IxrefPLim && (L_VdcReg <= (L_VregRef - L_VdcTranDel)))
    {
        L_VregRef = L_VdcReg + L_VdcTranDel;
    }



    // End Voltage reference code




    L_VregErr = (L_VregRef + L_VregStep) - L_VdcReg;

    // Proportion path calculation
    Vreg_Ix_Prp = L_VregErr * PARM(L_VregPrpGn) * L_VregWoComp;

    // Integral path calculation
        // Integrator freeze  if necessary
    L_VregDelInt = L_VregErr * L_DelTm1 * PARM(L_VregIntGn);
    if (( L_VregDelInt > 0.0F) && (L_InnrRegLim.L_IxrefPLim ||
                                  L_InnrRegLim.L_XAntiWndP))
    {
        L_VregDelInt = 0.0F;
    }
    else if ((L_InnrRegLim.L_IxrefNLim || L_InnrRegLim.L_XAntiWndN))
    {
        L_VregDelInt = 0.0F;
    }

        // Integration and clamping
    L_VregIxInt = L_VregIxInt + L_VregDelInt*L_VregWoComp;
    if ( L_VregIxInt > PARM(L_IxLimP) )
    {
        L_VregIxInt = PARM(L_IxLimP);
    }
    else if ( L_VregIxInt < PARM(L_IxLimN) )
    {
        L_VregIxInt = PARM(L_IxLimN);
    }

    // Sum propotional and integral parts and clamp result
    // Set appropriate limit flag if limit hit
         // Clear limit flags before entry
    L_InnrRegLim.L_IxrefPLim = false;
    L_InnrRegLim.L_IxrefNLim = false;
    L_IxCmd = L_VregIxInt + Vreg_Ix_Prp;

    if ( PARM(TestMode) == PUMP_BACK )
    {
        L_IxCmd = L_IxCmd - 0.33F * R_IxCmdZ;   // 3:1 transformer in factory (make scale factor into parm later)
    }
    if ( L_IxCmd > PARM(L_IxLimP) )
    {
        L_IxCmd = PARM(L_IxLimP);
        L_InnrRegLim.L_IxrefPLim = true;
    }
    else if ( L_IxCmd < PARM(L_IxLimN) )
    {
        L_IxCmd = PARM(L_IxLimN);
        L_InnrRegLim.L_IxrefNLim = true;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//     This function implements the task 2 portion of the Dc link
//     voltage reference. It can operate in either Fixed or Adaptive mode.
//     VoltageReferenceT2   -- Diagram Line/ALG_010.vsd
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void VoltageReferenceT2(void)
{
    float Mod_Del_Int;

    //Vdc boost due to  low grid voltage
    if ( RPNWRunning && (GmDscmVMgNPu > PARM(L_VdcVMgNThr)) )
    {
        L_VdcBstVmag = 0.0F;
    }
    else if(   ( (L_VMagT2 < PARM(L_VdcVmgLvrt)) || (GmDscmVMgNPu > PARM(L_VdcVMgNThr)) )  && !ZVRTIp  )
    {
        L_VdcBstVmag = PARM(L_VdcVmagDnRt) * DelTm2;
    }
    else if (R_FwkDel2 < PARM(R_VdcMdUpThr))
    {
        L_VdcBstVmag = PARM(L_VdcVmagUpRt) * DelTm2;
    }
    else
    {
        L_VdcBstVmag = 0.0F;
    }



    //Peak detector
    if ( L_VdcModErr > 0.0F)
    {
        Mod_Del_Int = DelTm2*PARM(L_VdcRefWUp)*L_VdcModErr;
    }
    else//the error is negative
    {
        Mod_Del_Int = DelTm2*PARM(L_VdcRefWDn)*L_VdcModErr;
    }


    //Select reference based on running/not running -- integrate if running
    if ( L_Running)
    {
        L_VdcStptInt = L_VdcStptInt + Mod_Del_Int  + L_VdcBstVmag;
    }
    else
    {
        //L_VdcStptInt = PARM(L_VdcRef);
        L_VdcStptInt = L_VMagT2*SQRT3*L_VdcMdRfRcp;
    }

    //Clamp the integrator
    if ( L_VdcStptInt > PARM(L_VdcRefMax))
    {
        L_VdcStptInt = PARM(L_VdcRefMax);
        L_VrefInMaxL = true;
    }
    else
    {
        if ( L_VdcStptInt < (PARM(L_VdcRefMax) - PARM(L_VrefLmHyst)) )
        {
           L_VrefInMaxL = false;
        }
        if (L_VdcStptInt < PARM(L_VdcRefMin))
        {
           L_VdcStptInt = PARM(L_VdcRefMin);
        }
    }

    //Select fixed or modulated voltage reference
    if ( PARM(L_VdcRefMode) && L_Running && !ZVRTIp)
    {
        L_VdcSetPt = L_VdcStptInt;
    }
    else
    {
        L_VdcSetPt = PARM(L_VdcRef);
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//     This function implements the pre-ramped voltage reference based on
//     the value of L_VdcRefMode. If L_VdcRefMode is 0(Fixed mode), the output
//     of this function, L_VdcSetPt, is simply the Fixed final value, L_VdcRef.
//     If, however, the value of L_VdcRefMode is 1 (Adaptive mode), then this
//     function adjusts the output, L_VdcSetPt, so that the modulation index
//     is equal to the raio of the nominal ac voltage input to the requested
//     line to line voltatge output(L_VllOut). The authority of the regulator
//     is limited so that the reference voltage cannot be greater than
//     L_VdcRefMax, which is rule calculated at 1.1*L_VllRms*sqrt(2), nor
//     less than 0.96*L_VllRms*sqrt(2).
//     A modulation index feedback is calculated from vars passed from Task 1.
//     It takes into consideration the resonators intput.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void VoltageReference(void)
{
    float Slew_Max3, DelVdcRf3;
    float VdcNorm;

    VdcNorm = NoNANLim( L_VdcFbkT3/L_VdcSetPt, 1.0e6F, -1.0e6F );

    //Calculate modulation index feedback
    L_ModIndReg  = sqrtf(L_IregXUcT3*L_IregXUcT3 + L_IregYUcT3*L_IregYUcT3);
    L_ModIndRes  = PARM(L_IregResWI)*sqrtf(L_ResX2T3*L_ResX2T3 + L_ResY2T3*L_ResY2T3);
    L_ModIndPeak = L_ModIndReg + L_ModIndRes;
    L_ModIndFbk  = L_ModIndPeak*VdcNorm;

    L_ModIndDc   = PARM(L_DcRegWI)*sqrtf(L_DcFndXT3*L_DcFndXT3 + L_DcFndYT3*L_DcFndYT3);

     //Calculate modulation index feedback for rotor converter
    R_ModIndReg  = sqrtf(R_UxCmdT3*R_UxCmdT3 + R_UyCmdT3*R_UyCmdT3);

    R_ModIndFbk  = R_ModIndReg*VdcNorm;


    //Find the errors
    L_VdcModErr = -PARM(L_VdcModRef) + L_ModIndFbk;
    R_VdcModErr = PARM(R_VdcModGn)*(-PARM(R_VdcModRef) + R_ModIndFbk);

    //Find the most positive
    if ( (L_VdcModErr < R_VdcModErr) && R_RunReq)
    {
        L_VdcModErr = R_VdcModErr;
    }
    //Remainder of code moved to task2 (VoltageReferenceT2)


#if 0
    if ( L_VdcModErr > 0.0F)
    {
        Mod_Del_Int = DelTm3*PARM(L_VdcRefWUp)*L_VdcModErr;
    }
    else//the error is negative
    {
        Mod_Del_Int = DelTm3*PARM(L_VdcRefWDn)*L_VdcModErr;
    }

    if ( L_InnrRegEnb)
    {
        if ( !L_Running)
        {
           L_VdcStptInt = PARM(L_VdcRef);
        }
        else if (   ( (L_VMagT3 < PARM(L_VdcVmgLvrt)) || (GmDscmVMgNPu > PARM(L_VdcVMgNThr))  ) && !ZVRTIp  )
        {
             L_VdcStptInt = PARM(L_VdcRefLvrt);
        }
        else
        {
             L_VdcStptInt = L_VdcStptInt + Mod_Del_Int;
        }

    }
    else
    {
        L_VdcStptInt = GmVMagT3*SQRT3/PARM(L_VdcModRef);
    }

    //Clamp the integrator
    if ( L_VdcStptInt > PARM(L_VdcRefMax))
    {
        L_VdcStptInt = PARM(L_VdcRefMax);
    }
    else if (L_VdcStptInt < PARM(L_VdcRefMin))
    {
        L_VdcStptInt = PARM(L_VdcRefMin);
    }

    if ( PARM(L_VdcRefMode) )
    {
        L_VdcSetPt = L_VdcStptInt;
    }
    else
    {
        L_VdcSetPt = PARM(L_VdcRef);
    }

#endif

    // Populate DCLinkInit
    L_VregErrT3  = L_VregErr;
    L_DelVdcRfT3 = L_DelVdcRf;

    if ( (L_SeqSt == LINE_RAMPUP) && !L_DCLinkInit)
    {
       if ( ( ABS(L_VregErrT3) < PARM(L_DCInitThr)) && L_InnrRegEnb &&
            (L_DelVdcRfT3 == 0.0F))
       {
            L_DCInitTmr = L_DCInitTmr + DelTm3;
            if ( (L_DCInitTmr > PARM(L_DCInitTm)) || (ZVRTIp && (L_DCInitTmr > (PARM(L_DCInitZVTm))) ) )
            {
               L_DCLinkInit = true;
            }
       }
       else
       {
          L_DCInitTmr  = 0.0F;
       }
    }
    else
    {
       L_DCInitTmr  = 0.0F;
    }

    // ultra slow ramp for pumback capacitor forming
    if ( (PARM(TestMode) == PUMP_BACK) && (PARM(L_VdcSlw) < 10.0F) )
    {
        L_SlwCnt3++;
        if (L_SlwCnt3 == 100)
        {
            L_SlwCnt3 = 0;

            Slew_Max3 = 100.0F*DelTm3*PARM(L_VdcSlw);

            DelVdcRf3  = L_VdcSetPt - L_VregRef;
            if ( DelVdcRf3 >= Slew_Max3)
            {
               DelVdcRf3 = Slew_Max3;
            }
            else if (DelVdcRf3 <= -Slew_Max3)
            {
               DelVdcRf3 = -Slew_Max3;
            }
            L_VregRef = L_VregRef + DelVdcRf3;
        }
        L_SlewMax = 0.0F;   // disable task1 ramp
    }
    else
    {
        L_SlewMax = L_DelTm1*PARM(L_VdcSlw);
    }

    return;
}
