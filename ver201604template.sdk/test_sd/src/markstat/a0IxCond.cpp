///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Conditioning of X-axis rotor current
//
// DESCRIPTION:
//      This file contains those functions which are associated with
//      conditioning of the x component of current.
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
#include "product.h"
#include "a0IxCond.h"


// Externals
//----------
extern float         L_VdcIxIyMod;
extern float         L_VregErr;
extern float         R_DchgScl;
extern float         R_DelTm1;
extern float         R_DelTm1Rcp;
extern float         R_IxCmd0LimPk;
extern float         R_IxPdOutT1;
extern float         R_IxregInt;
extern float         R_KPolesRcp;
extern float         R_PdNegLimT1;
extern float         R_PdPosLimT1;
extern float         R_TrqCmdAct;
extern float         R_TrqCmd2T1;
extern float         R_TrqLimNegT1;
extern float         R_TrqLimPosT1;
extern float         R_TrqRegStepT1;
extern float         R_VdcFbk;
extern float         R_VxFlx;
extern float         S_FlxMtrRcpT1;
extern float         S_IxFbk;
extern unsigned      R_SeqReqT1_PackedBits;
extern unsigned      R_SeqStatT1_PackedBits;
extern RRegLimFlgs   R_InnrRegLim;

extern unsigned      IslandDet;
extern unsigned      DisconWhlRun;

extern float         DTDTrqCmdTot;
extern float         S_LSigma;
extern float         S_FlxXFbk;
extern float         S_IyFbk;
extern float         S_VyFbk;
extern float         S_XmRcp;
extern float         S_R1;

extern float        ISysCmdSF;
extern float         R_IyFbk;
extern   float     SRVRatio;
extern   float     R_IxVdcGnT1;

// Constants
//----------


// Types and Classes
//------------------
struct PosLimFlgs
{
    uint    R_IxCmdFrzp     :1;  // Ix command positive limit hit
    uint    R_TrqCmdLimP    :1;  // Torque command positive limit hit
    uint    Spare_02        :1;  // Spare 02
    uint    Spare_03        :1;  // Spare 03
    uint    Spare_04        :1;  // Spare 04
    uint    Spare_05        :1;  // Spare 05
    uint    Spare_06        :1;  // Spare 06
    uint    Spare_07        :1;  // Spare 07
    uint    Pll_Lim_Max     :1;  // Spare 08
    uint    Spare_09        :1;  // Spare 09
    uint    Spare_10        :1;  // Spare 10
    uint    Spare_11        :1;  // Spare 11
    uint    Spare_12        :1;  // Spare 12
    uint    Spare_13        :1;  // Spare 13 - last element
};

struct NegLimFlgs
{
    uint    R_IxCmdFrzn     :1;  // Ix Command neg limit hit
    uint    R_TrqCmdLimN    :1;  // Torque command negative limit hit
    uint    Spare_02        :1;  // Spare 02
    uint    Spare_03        :1;  // Spare 03
    uint    Spare_04        :1;  // Spare 04
    uint    Spare_05        :1;  // Spare 05
    uint    Spare_06        :1;  // Spare 06
    uint    Spare_07        :1;  // Spare 07
    uint    Pll_Lim_Min     :1;  // Spare 08
    uint    Spare_09        :1;  // Spare 09
    uint    Spare_10        :1;  // Spare 10
    uint    Spare_11        :1;  // Spare 11
    uint    Spare_12        :1;  // Spare 12
    uint    Spare_13        :1;  // Spare 13 - last element
};


// Parameters
//-----------
CREATE_PARM(R_VMgIxDcyW ,       float);     // Stator voltage clamp current reduction rate(rad/sec)
CREATE_PARM(R_VMgIxDcyMn,       float);     // Stator voltage clamp current reduction minimum current
CREATE_PARM(R_VMgIxDcyM2,       float);     // Stator voltage clamp current reduction minimum current 2

CREATE_PARM(R_IxNchFilSA0,      float);     // Rotor Ix current command notch Filter S Domain A0 coefficient
CREATE_PARM(R_IxNchFilSA1,      float);     // Rotor Ix current command notch Filter S Domain A1 coefficient
CREATE_PARM(R_IxNchFilSA2,      float);     // Rotor Ix current command notch filter S Domain A2 coefficient
CREATE_PARM(R_IxNchFilSB1,      float);     // Rotor Ix current command notch filter S Domain B1 coefficient
CREATE_PARM(R_IxNchFilSB2,      float);     // Rotor Ix current command notch filter S Domain B2 coefficient

CREATE_PARM(S_IxCmdTst,         float);     // Test X-axis current command (stator);
CREATE_PARM(R_ICmdTst,          float);     // Test X-axis current command (rotor);
CREATE_PARM(R_IxCompEnb,        unsigned);  // Ix compensation enable
CREATE_PARM(R_IxDdtMin,         float);     // derivitive min. clamp for Ix slew rate
CREATE_PARM(R_IxI1Gn,           float);     // Null
CREATE_PARM(R_IxI2Gn,           float);     // Null
CREATE_PARM(R_IxI3Gn,           float);     // Null
CREATE_PARM(R_IyLimW,           float);     // current limit: Ix filter bandwidth (for Iy limit calc);
CREATE_PARM(R_IxP0Gn,           float);     // Null
CREATE_PARM(R_IxP1Gn,           float);     // Null
CREATE_PARM(R_IxP2Gn,           float);     // Null
CREATE_PARM(R_IxP3Gn,           float);     // Null
CREATE_PARM(R_LsRcpSlew,        float);     // l sigma reciprocal slew rate

CREATE_PARM(R_PdActLvl,         float);     // Power dip action voltage level
CREATE_PARM(R_PdLim,            float);     // Power dip control output limit
CREATE_PARM(R_PdPrpGn,          float);     // Power dip voltage reg gain
CREATE_PARM(R_PdRcyLvl,         float);     // Power dip recovery voltage level
CREATE_PARM(R_PdSpdThr,         float);     // Power dip speed thershold lvl (rpm);
CREATE_PARM(R_PdTm,             float);     // Power dip timer expire time
CREATE_PARM(R_PdTmMode,         unsigned);  // Power dip timer selection mode
CREATE_PARM(R_PwrDipTm,         float);     // Custom power dip time

CREATE_PARM(R_PullOutScl,       float);     // current limit pullout scale factor
CREATE_PARM(R_IxVdcGn,          float);     // Ix gain control for fast vdc management by rotor conv.
CREATE_PARM(R_IxVdcSpdGn,       float);     // Ix-speed gain control for fast vdc management by rotor conv
CREATE_PARM(R_IxVdcOff,         float);     // Ix gain control for fast vdc management by rotor conv.
CREATE_PARM(R_IxVdcRtGn,        float);     // Gain for Ix current change based on rate of change of vdc
CREATE_PARM(R_IyVdcRtGn,        float);     // Gain for Iy current change based on rate of change of vdc
CREATE_PARM(R_IxCmd0Lim,        float);     // Ix command 0 limit
CREATE_PARM(R_VxMargin,         float);     // x-component volt margin for Ix slew rate
CREATE_PARM(R_WIy,              float);     // Omega for Iy lp filter
//stator Ix regulator
CREATE_PARM(S_IxregMax,         float);
CREATE_PARM(S_IxregIntGn,       float);
CREATE_PARM(S_IxregPrpGn,       float);
// torque limit
CREATE_PARM(R_TrqLimGen1,       float);     // Torque command generating limit #1
CREATE_PARM(R_TrqLimGen2,       float);     // Torque command generating limit #2
CREATE_PARM(R_TrqLimMtr1,       float);     // Torque command motoring limit #1
CREATE_PARM(R_TrqLimMtr2,       float);     // Torque command motoring limit #2
CREATE_PARM(R_TrqDTDolMrg,      float);     // Torque over-load drive-train damper margin

// Diagnostics
//------------


// Variables
//----------

// X Torque Command
CREATE_PUBVAR(S_IxCmd,       float); // x-component current command - stator
CREATE_PUBVAR(S_IxregErr,    float); // stator Ix regulator error
CREATE_PUBVAR(S_IxregInt,    float); // stator Ix regulator integrator
CREATE_PUBVAR(S_IxregOut,    float); // stator Ix regulator output
CREATE_PUBVAR(R_IaCmdLkout,  float); // phase A current command for up-down lockout
CREATE_PUBVAR(R_IbCmdLkout,  float); // phase B current command for up-down lockout
CREATE_PUBVAR(R_IcCmdLkout,  float); // phase C current command for up-down lockout
CREATE_PUBVAR(R_IxCmd,       float); // x-component current command
CREATE_PUBVAR(R_IxCmd0,      float); // x-component current command-no delay
CREATE_PUBVAR(R_IxCmd1,      float); // x-component current command-delay 1 pass
CREATE_PUBVAR(R_IxCmd2,      float); // x-component current command-delay 2 passes
CREATE_PUBVAR(R_IxCmd3,      float); // x-component current command-delay 3 passes
CREATE_PUBVAR(R_IxCmdP,      float); // ix command P
CREATE_PUBVAR(R_IxCmdReqDl,  float); // rotor ix command rate limit minimum
CREATE_PUBVAR(R_IxCmdRtLim,  float); // x-component current command rate limited
CREATE_PUBVAR(R_IxCmdRtMax,  float); // rotor ix command rate limit maximum
CREATE_PUBVAR(R_IxCmdRtMin,  float); // rotor ix command rate limit minimum
CREATE_PUBVAR(R_IxCmdZ,      float); // R_IxCmd delay structure output
CREATE_PUBVAR(R_IxMax,       float); // ix command limited by pull-out(pre pwrdip
CREATE_PUBVAR(R_IxVdc,       float); // rotor vdc ix
CREATE_PUBVAR(R_IyFbkFil,    float); // Filtered Iy Feedback
CREATE_PUBVAR(R_IyVdc,       float); // rotor vdc iy
CREATE_PUBVAR(R_PullOutLim,  float); // positive ix limit from pull out protection
CREATE_PUBVAR(R_TrqCmdIn,    float); // final torque command
CREATE_PUBVAR(R_TrqCmdInTr,  float); //
CREATE_PUBVAR(R_TrqCmdInTt,  float); //
CREATE_PUBVAR(R_TrqRegStep,  float); //
CREATE_PUBVAR(SRVRatioRcp,   float); // Stator-rotor voltage ratio recip
CREATE_PUBVAR(R_ITrqCmd,     float); // Torque current command
CREATE_PUBVAR(R_IxCmdRtFil,  float); // x-component current command rate limited filtered
CREATE_PUBVAR(R_VMgIxDcyDy,  float); // rotor dynamic current decay for HVRT
CREATE_PUBVAR(R_VMgIxDcyGn,  float); // P.R_VMgIxDcyW * R_DelTm1
CREATE_PUBVAR(R_TrqLmTtNegT1,float); // T2->T1 version of R_TrqLmTtNeg
CREATE_PUBVAR(TrqPwrTrimT1,  float);
CREATE_PUBVAR(R_TrqCmdInTtT2,   float);     // R_TrqCmdInTt Task 2 version
CREATE_PUBVAR(R_TrqCmdInTtT3,   float);     // R_TrqCmdInTt Task 3 version


// Rotor Ix current command notch filter
CREATE_PUBVAR(R_IxNchFilZA0, float);    // Rotor Ix current command notch filter z domain: A0 gain
CREATE_PUBVAR(R_IxNchFilZA1, float);    // Rotor Ix current command notch filter z domain: A1 gain
CREATE_PUBVAR(R_IxNchFilZA2, float);    // Rotor Ix current command notch filter z domain: A2 gain
CREATE_PUBVAR(R_IxNchFilZB1, float);    // Rotor Ix current command notch filter z domain: B1 gain
CREATE_PUBVAR(R_IxNchFilZB2, float);    // Rotor Ix current command notch filter z domain: B2 gain
CREATE_PUBVAR(R_IxNchFilu1,  float);    // Rotor Ix current command notch filter: input Z1 delay
CREATE_PUBVAR(R_IxNchFilu2,  float);    // Rotor Ix current command notch filter: input Z2 delay
CREATE_PUBVAR(R_IxNchFily1,  float);    // Rotor Ix current command notch filter: output Z1 delay
CREATE_PUBVAR(R_IxNchFily2,  float);    // Rotor Ix current command notch filter: output Z2 delay
CREATE_PUBVAR(R_IxCmd0NchFl, float);    // Rotor Ix current command notch filter: output
CREATE_PUBVAR(R_IxCmd0In,    float);    // Rotor Ix current command notch filter: input


// Unpublished Variables
//----------------------
unsigned*  R_TrqStepAdr;


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Data Passing
//--------------
DATA_PASS(R_TrqLmTtNeg, R_TrqLmTtNegT1, T2_T1, UNFILTERED);
DATA_PASS(TrqPwrTrim,   TrqPwrTrimT1,   T3_T1, UNFILTERED);
DATA_PASS(R_TrqCmdInTt,     R_TrqCmdInTtT2, T1A_T2, FILTERED);
DATA_PASS(R_TrqCmdInTtT2,   R_TrqCmdInTtT3, T2_T3,  FILTERED);

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Calculation of the torque command X component.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorIxTrqCmd(void)
{
     //float   Ey_Tmp;
     float   Temp1, Temp2;
     float   R_IxVdcTemp;


    /////////////////////////////////////////////////////////////////////////
    // Note:  The R_TrqEnbReq from the sequencer is the enable for the torque
    // command.  Any use of the R_TrqEnbReq MUST follow the steps below:
    //  1) It MUST be accompanied by a respective setting or clearing of
    //     R_TrqEnbStat as the required task feedback to the sequencer.
    //  2) Logic must also be added to the Precondition_T1() function because
    //     IxTrqCmd runs only when R_InnrRegEnb is true (see Task1).
    //  3) The values in the data buffer must be updated in the
    //     SequencerTaskInterface function.
    /////////////////////////////////////////////////////////////////////////

     if (R_SeqReqT1.R_TrqEnbReq)
     {
        // Set R_SeqStat.R_TrqEnbStat since request is true
        R_SeqStatT1.R_TrqEnbStat = true;
        S_IxCmd = R_KPolesRcp * R_TrqCmdInTt * S_FlxMtrRcpT1 - PARM(S_IxCmdTst);

        R_ITrqCmd = PARM(R_ICmdTst) + S_IxCmd * SRVRatio - S_IxregOut;

        if( (PARM(R_IxCompEnb)) )
        {
            // Stator Ix regulator
            S_IxregErr = S_IxFbk - S_IxCmd;

            // Calculate and clamp integral path, with anti-windup
            if (    ( !R_InnrRegLim.R_IxCmdFrzn  ||  S_IxregErr > 0)
                 && ( !R_InnrRegLim.R_IxCmdFrzp  ||  S_IxregErr < 0)    )
            {
                S_IxregInt = S_IxregInt + PARM(S_IxregIntGn)*R_DelTm1*S_IxregErr;
            }
            if (S_IxregInt >=  PARM(S_IxregMax))  S_IxregInt =  PARM(S_IxregMax);
            if (S_IxregInt <= -PARM(S_IxregMax))  S_IxregInt = -PARM(S_IxregMax);

            // Calculate proportional path, sum with int. path, clamp
            S_IxregOut = S_IxregInt + S_IxregErr*PARM(S_IxregPrpGn);
            if ( S_IxregOut >= PARM(S_IxregMax))
            {
                 S_IxregOut = PARM(S_IxregMax);
            }

            if ( S_IxregOut <= -PARM(S_IxregMax))
            {
                 S_IxregOut = -PARM(S_IxregMax);
            }
        }
        else
        {
            S_IxregInt = S_IxregInt * R_DchgScl;
        }
     }
     else
     {
        // Clear R_SeqStat.R_TrqEnbStat since request is false
        R_SeqStatT1.R_TrqEnbStat = false;

        S_IxCmd    = 0.0;
        R_ITrqCmd  = 0.0;
        S_IxregInt = 0.0;
     }

    //////////////////////////////////////////////////////////
    //            Form DC Link Suppression Current
    //////////////////////////////////////////////////////////

     //R_IxVdcGnT1 =  T1BufB2Ptr->R_IxVdcGnT2;
     R_IxVdcTemp =  MAX( (-L_VregErr - PARM(R_IxVdcOff)), 0.0);

     R_IxVdc =  R_IxVdcGnT1 * ( R_IxVdcTemp + (L_VdcIxIyMod * PARM(R_IxVdcRtGn)) );
     R_IyVdc = L_VdcIxIyMod * PARM(R_IyVdcRtGn);


    //////////////////////////////////////////////////////////
    //            Form Command current and delays
    //////////////////////////////////////////////////////////

    // R_IxCmd generation
    // Temp1 = -R_ITrqCmd;     // ind motor
    Temp1 = R_ITrqCmd;         // doubly-fed

    R_InnrRegLim.R_IxCmdFrzn = false;
    R_InnrRegLim.R_IxCmdFrzp = false;

    if ( Temp1 > R_PdPosLimT1)
    {
        Temp1 = R_PdPosLimT1;
        S_IxCmd = (SRVRatioRcp*(Temp1 + S_IxregOut) );
        R_InnrRegLim.R_IxCmdFrzn = true;
    }
    else if (  Temp1 < R_PdNegLimT1)
    {
        Temp1 = R_PdNegLimT1;
        S_IxCmd = (SRVRatioRcp*(Temp1 + S_IxregOut) );
        R_InnrRegLim.R_IxCmdFrzp = true;
    }

    R_IxCmd = Temp1 + R_IxPdOutT1;

    // Current Command Delay Model

    R_IxCmd3 = R_IxCmd2;
    R_IxCmd2 = R_IxCmd1;
    R_IxCmd1 = R_IxCmd0;

    R_IxCmdZ = (PARM(R_IxI1Gn)*R_IxCmd1) + (PARM(R_IxI2Gn)*R_IxCmd2) +
            (PARM(R_IxI3Gn)*R_IxCmd3);


    R_IxCmdP = (PARM(R_IxP0Gn)*R_IxCmd0) + (PARM(R_IxP1Gn)*R_IxCmd1) +
            (PARM(R_IxP2Gn)*R_IxCmd2) + (PARM(R_IxP3Gn)*R_IxCmd3);


    //////////////////////////////////////////////////////////
    // Torque Current Slew Rate Limiter output is R_IxCmdRtLim
    //////////////////////////////////////////////////////////

    Temp1 = (R_VdcFbk + PARM(R_VxMargin) )*SQRT3_RCP;
    Temp2 = (R_VxFlx + R_IxregInt);

    // R_IxCmdRtMax is clamped, calculated R_IxCmd upper limit
    R_IxCmdRtMax = PARM(R_LsRcpSlew)*(Temp1 - Temp2);
    if ( R_IxCmdRtMax < PARM(R_IxDdtMin))
    {
        R_IxCmdRtMax = PARM(R_IxDdtMin);
    }


    // R_IxCmdRtMin is clamped,calculated R_IxCmd lower limit
    R_IxCmdRtMin = PARM(R_LsRcpSlew)*((-Temp1) - Temp2);
    if ( R_IxCmdRtMin > (-PARM(R_IxDdtMin)))
    {
        R_IxCmdRtMin = -PARM(R_IxDdtMin);
    }


    //  Calculate clamped intermediate value
    //  R_IxCmdReqDl has output


    R_IxCmdReqDl = (R_IxCmd - R_IxCmdRtLim)*R_DelTm1Rcp;
    if (  R_IxCmdReqDl > R_IxCmdRtMax)
    {
        R_IxCmdReqDl = R_IxCmdRtMax;
    }
    else if (R_IxCmdReqDl < R_IxCmdRtMin)
    {
        R_IxCmdReqDl = R_IxCmdRtMin;
    }

    // Start DC Link Suppreession Control -- TRQ_020

    R_IxCmdRtLim = R_IxCmdRtLim +  R_DelTm1 * R_IxCmdReqDl;

    // Precondition filter with rate limited current command and used filter output going toward zero when S_VMaxHiDet
    if (IslandDet || DisconWhlRun)
    {
       R_IxCmdRtFil = R_IxCmdRtFil + R_VMgIxDcyGn * (R_VMgIxDcyDy - R_IxCmdRtFil);
       R_IxCmd0In   = R_IxCmdRtFil;
    }
    else
    {
       R_IxCmdRtFil = R_IxCmdRtLim;
       R_IxCmd0In   = R_IxCmdRtLim;;
    }

    R_IxCmd0 = MIN( (R_IxCmd0In + R_IxVdc), R_IxCmd0LimPk) * ISysCmdSF;
    

    // End DC Link Suppreession Control -- TRQ_020


    /////////////////////////////////////////////////////////////////////////
    // low pass filter for R_IyFbk:  use for Task 2 to generate Flx_Mtr Only
    /////////////////////////////////////////////////////////////////////////

    R_IyFbkFil = R_IyFbkFil +
                R_DelTm1 * PARM(R_WIy) * (R_IyFbk - R_IyFbkFil);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Calculation of input torque command.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorTorqueInput(void)
{
    float R_TrqCmdInUc;

    ////////////////////////////
    //  Clamp torque command
    ////////////////////////////

    if ( R_TrqCmd2T1 < R_TrqLimNegT1 )
    {
        R_TrqCmdIn   = R_TrqLimNegT1;
        R_InnrRegLim.R_TrqCmdLimN = true;
    }
    else    // Clear limit flags if not in limit
    {
        R_InnrRegLim.R_TrqCmdLimN = false;
        R_TrqCmdIn = R_TrqCmd2T1;
    }


     R_TrqCmdInTr = R_TrqCmdIn + TrqPwrTrimT1 + R_TrqRegStepT1;

     R_TrqCmdInUc = R_TrqCmdInTr - DTDTrqCmdTot;

    if ( R_TrqCmdInUc > R_TrqLimPosT1 )
    {
        R_TrqCmdInUc   = R_TrqLimPosT1;
        R_InnrRegLim.R_TrqCmdLimP = true;
    }
    else
    {
        R_InnrRegLim.R_TrqCmdLimP = false;
    }

    if ( R_TrqCmdInUc < R_TrqLmTtNegT1 )
    {
        R_TrqCmdInUc   = R_TrqLmTtNegT1;
        R_InnrRegLim.R_TrqCmdLimN = true;
    }
    R_TrqCmdInTt = R_TrqCmdInUc;

    return;
}
