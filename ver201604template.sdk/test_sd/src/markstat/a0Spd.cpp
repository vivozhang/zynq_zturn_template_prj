///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Speed feedback related functions
//
// DESCRIPTION:
//      Speed feedback processing in application layer.
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     a0Spd.vsd, a0Spd.xls
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
#include "a0Spd.h"
#include "a0Rule.h"
#include "b0BridgeRebf.h"
#include "s0SimPlant1x.h"


// Externals
//----------
extern float        CI_R_WRtrW;
extern float        DelTm2;
extern float        DelTm2Rcp2x;
extern float        DelTm2RcpSqr;
extern float        FpllFrqAdj;
extern float        R_AngCorPIGn;
extern float        R_AngEstFF;
extern float        R_AngEstFFGn;
extern float        R_DelTm1;
extern float        R_PolePrRcp;
extern float        R_SpdCalInt;
extern float        R_SpdFbkT3;
extern float        R_TachPosInt;
extern float        R_TrqCal;
extern float        R_TrqCmdIn;
extern float        R_TrqRatRcp;
extern float        R_WRtrFil;
extern float        R_WRtrT2;
extern float        R_MrkLosFlagT2;
extern float        R_TacBadFlagT2;
extern float        DelTm3;
extern int          R_MrkrPosT2;
extern int          R_T1FramesPerT2;
extern unsigned     SimMode;
extern unsigned     DsimMode;
extern unsigned     T2DoneOnce;
extern unsigned     R_SpdT2RAFN;
extern unsigned     R_SpdTcRAFN;
extern unsigned *   DSP_TTMR1_ADDR;
extern unsigned *   DSP_HCF_ADDR;
extern unsigned *   DSP_HIF_ADDR;
extern unsigned     TachValueZ1;
extern cBridgeRebf  R_BridgeRebf;
extern cSimPlant1x  SimPlant1x;
extern unsigned      R_SeqStat_PackedBits;
extern float        R_TrqCmdInTtT3;
extern float        R_AngEstDtT2;
extern float     R_SpdSlowT1;
extern float R_TachPosn;
extern float R_AngEstErT2;

// Constants
//----------
// Tach pulse scaling - number of signal pulses per tach pulse
#define PI5                 (5.0F * PI)
#define NEW_CANSPD_METHOD   1
#define OLD_CANSPD_METHOD   0
#define INTRG_MODE          0
#define FRACT_MODE          1


// Types and Classes
//------------------


// Parameters
//-----------

CREATE_PARM(R_AngCorPIW,        float);     // Angle feed-forward band-width for PI corrector
CREATE_PARM(R_MrkrTplThr,       unsigned);  // Marker loss alarm tach pulse threshold
CREATE_PARM(R_MrkrTplFlt,       unsigned);  // Marker fault tach pulse threshold
CREATE_PARM(R_MrkrMisFlt,       unsigned);  // Marker missing fault threshold
CREATE_PARM(R_PolePairs,        float);     // number of pole pairs
CREATE_PARM(R_TrqErrW,          float);     // Torque error filter crossover
CREATE_PARM(R_TrqErAlmLv,       float);     // Torque error alarm level
CREATE_PARM(R_TrqErFltLv,       float);     // Torque error trip level
CREATE_PARM(R_WTrqTrk,          float);     // Torque tracking filer crossover
CREATE_PARM(R_TachPlsThr,       unsigned);  // pulse error at marker for tach alarm
CREATE_PARM(R_TachPlsFlt,       int);       // pulse count error at marker for tach fault
CREATE_PARM(R_MarkerOffs,       float);     // tach marker offset(rads); from pk of Va
CREATE_PARM(R_MrkrErrLim,       int);       // marker error correction value
CREATE_PARM(R_TrqTrkThr,        float);     // torque tracking threshold
CREATE_PARM(R_TachLtScl,        float);     // Tach latch scale
CREATE_PARM(R_TachFrcScl,       float);     // Tach fractional scale
CREATE_PARM(R_SpdScl2,          float);     // Speed scale 2
CREATE_PARM(R_RtrPosMeth,       unsigned);  // Rotor angle position method
CREATE_PARM(R_SpdT2RAFN,        unsigned);  // Depth of speed RAF task2 filter
CREATE_PARM(R_AngCorDt,         float);     // Angle correction delta time
CREATE_PARM(R_AngFFGn,          float);     // Angle feed-forward gain
CREATE_PARM(R_SpdRAFCrGn,       float);     // RAF Correction gain for RAF
CREATE_PARM(R_AngEstFFW,        float);     // Estimated angle estimate feed-forward omega
CREATE_PARM(R_SpdFrcFilW,       float);     // Response for T1 filter of fractional speed rad/sec
CREATE_PARM(R_AngCorW,          float);     // Angle correction filter omega
CREATE_PARM(R_SpdTcRAFN,        unsigned);  // Depth of speed RAF for turbine control
CREATE_PARM(R_SpdTcFFRto,       float);     // Turbine control speed feed-forward ratio
CREATE_PARM(R_SpdTcFFW,         float);     // Turbine control speed feed-forward lag break
CREATE_PARM(R_SimQntReq,        unsigned ); // Use simulated speed quantization emulation (crunchies);
CREATE_PARM(R_WRtrW,            float);     // Inner loop speed filter
// RotorProcessSfb2()
CREATE_PARM(R_WSpdT2,           float);     // Omega for task 2 speed feedback

CREATE_PARM(R_TachPosScl,       float);     // Units = degrees/tach pulse
CREATE_PARM(R_TachPosRatio,     float);     // Tach position ratio
CREATE_PARM(R_TachPosMax,       int);       // Tach pulse


// Diagnostics
//------------
CREATE_DIAG(R_TachBadAlm);          // lost/gained tach pulses alarm
CREATE_DIAG(R_TachBadFlt);          // lost/gained tach pulses trip
CREATE_DIAG(R_MrkLossAlm);          // marker not seen alarm
CREATE_DIAG(R_MrkLossFlt);          // marker not seen trip
CREATE_DIAG(R_TrqErrFlt);           // torque error trip fault
CREATE_DIAG(R_TrqErrAlm);           // torque error alarm

// Variables
//----------
CREATE_PUBVAR(R_AngEstFF,     float);     /* Angle estimate feed-forward to comp for speed feedback delay*/
CREATE_PUBVAR(R_TachPosInt,   float);     /* Tach position integrator for estimation */
CREATE_PUBVAR(R_AngCorr,      float);        // Angle correction

CREATE_PUBVAR(R_AngEstInt,    float);        // Angle estimate error integrator
CREATE_PUBVAR(R_MrkLosFlag,   float);        // marker loss alarm for task 2
CREATE_PUBVAR(R_PosScl,       float);        // position cnts to radians scale factor
CREATE_PUBVAR(R_Spd,          float);        // Units = rad/sec
CREATE_PUBVAR(R_SpdFbk,       float);        // Units = Rev/min in T2
CREATE_PUBVAR(R_SpdFbkAvg,    float);
CREATE_PUBVAR(R_SpdFbkPLC,    float);
CREATE_PUBVAR(R_SpdFbkT2,     float);        // speed feedback task 1*/
CREATE_PUBVAR(R_SpdGen,       float);        // Tach based Mech Speed RPM T2
CREATE_PUBVAR(R_SpdQnt,       float);        // Units = hertz/pulse
CREATE_PUBVAR(R_SpdQntInv,    float);        // Units = pulse/hertz
CREATE_PUBVAR(R_SpdRAFErr,    float);        // Error for RAF1
CREATE_PUBVAR(R_SpdRAFErr2,   float);        // Error for RAF2
CREATE_PUBVAR(R_SpdRAFNRcp,   float);        // Reciprocal of R_SpdT2FAFN;
CREATE_PUBVAR(R_SpdRAFNTRcp,  float);        // Reciprocal of R_SpdTcFAFN;
CREATE_PUBVAR(R_SpdRAFT2N,    float);        // Oldest member value
CREATE_PUBVAR(R_SpdRAFTcN,    float);        // Oldest member value for Tc array
CREATE_PUBVAR(R_SpdRawT2,     float);        // Units = accumulator counts
CREATE_PUBVAR(R_SpdScl,       float);        // Units = hertz/pulse
CREATE_PUBVAR(R_SpdSlowT2,    float);        // slow speed from T2 rolling average filter
CREATE_PUBVAR(R_SpdT1,        float);        // Units = rpm
CREATE_PUBVAR(R_SpdT2,        float);        // Units =rad/sec
CREATE_PUBVAR(R_SpdT2RAF,     float);        // Rolling average filter output for Task2 speed
CREATE_PUBVAR(R_SpdTach,      float);        // Tach based Mech Speed RPM T2
CREATE_PUBVAR(R_SpdTcFFFil,   float);        // Speed for TC feed-fwd filter
CREATE_PUBVAR(R_SpdTcFFGn,    float);        // Speed for TC speed ff gain
CREATE_PUBVAR(R_SpdTcRAF,     float);        // RAF For TC speed
CREATE_PUBVAR(R_SpdTcRAF2,    float);        // RAF For TC speed rpm
CREATE_PUBVAR(R_SpdTCtlT2,    float);        // Speed for turbine control
CREATE_PUBVAR(R_TacBadFlag,   float);        // tach loss alarm for task 2
CREATE_PUBVAR(R_TachDt,       float);        // accumulated time sinc last speed calculation
CREATE_PUBVAR(R_TachDtRcp,    float);        // reciprocal of R_TachDt
CREATE_PUBVAR(R_TachE0,       float);        // fractional tach estimate last task 2 interrupt
CREATE_PUBVAR(R_TachE1,       float);        // fractional tach estimate this task 2 interrupt
CREATE_PUBVAR(R_TachNs0,      float);        // delta tach pulses last task 2 interrupt
CREATE_PUBVAR(R_TachNs1,      float);        // delta tach pulses this task 2 interrupt
CREATE_PUBVAR(R_TachPulses,   float);        // delta tach pulses this task 1 interrupt
CREATE_PUBVAR(R_TachT0,       float);        // time since last tach pulse last task 2 interrupt(secs)
CREATE_PUBVAR(R_TachT1,       float);        // time since last tach pulse last task 2 interrupt(secs)
CREATE_PUBVAR(R_TachTs1,      float);        // time for Ns1 integer tach pulses(secs)
CREATE_PUBVAR(R_TachTs1Rcp,   float);        // reciprocal of R_TachTs1
CREATE_PUBVAR(R_TrqErrFil,    float);        // Torque error filtered
CREATE_PUBVAR(R_TrqTFilGn,    float);        // Torque tracking filter gain
CREATE_PUBVAR(R_TrqTrkErr,    float);        // Torque tracking error              // maybe these belongs elsewhere
CREATE_PUBVAR(R_TrqTrkFil,    float);        // Torque tracking error filtered     // but so far only used in this file
CREATE_PUBVAR(R_TrqTrkFil1,   float);
CREATE_PUBVAR(R_TrqTrkFil2T1, float);
CREATE_PUBVAR(R_WRtrRAFT2,    float);        // Rotor elec speed from rolling average filter
CREATE_PUBVAR(R_WRtrSlowT2,   float);        // Rotor electircal speed in rad/sec, task 3 for slow use in task1
CREATE_PUBVAR(R_WRtrT2,       float);        // Units = rad/sec
CREATE_PUBVAR(R_AngRaw,       int);          // delta tach count from marker count
CREATE_PUBVAR(R_DelMrkrPls,   int);          // tach pulses since last marker
CREATE_PUBVAR(R_MrkPosInit,   int);          // Tach quadrature counts at marker at initialization
CREATE_PUBVAR(R_MrkrErr,      int);          // tach pulses lost/gained each marker pulse
CREATE_PUBVAR(R_MrkrPlsAlm,   int) = 32000;  // pulse threshold for marker loss alarm
CREATE_PUBVAR(R_MrkrPlsFlt,   int) = 32000;  // pulse threshold for marker loss fault
CREATE_PUBVAR(R_MrkrPos,      int);          // DSPX Tach quadrature counts at marker task 1
CREATE_PUBVAR(R_MrkrPosOld,   int);          // DSPX Tach quadrature counts at marker task 1-for simulated marker loss
CREATE_PUBVAR(R_MrkrTst,      int);          // simulated missing marker pulse count
CREATE_PUBVAR(R_PosRaw,       int);          // DSPX Tach quadrature counts task 1
CREATE_PUBVAR(R_SpdRaw,       int);          // Units = accumulator counts
CREATE_PUBVAR(R_RtrPosPulses, int);          // Units = tach pulses integrated and modulo 1,000 * R_TachPulses
CREATE_PUBVAR(R_RtrHssPos,    float);        // Units = degrees of high speed shaft .....free running, wraps at 360,000 deg or 1,000 revolutions
CREATE_PUBVAR(R_TachBadCnt,   int);          // tach error out of fault limit counter
CREATE_PUBVAR(R_TachFlt,      int);          // a tach fault has occurred
CREATE_PUBVAR(R_TachIntgrl,   int);          // tach position integral (tach edges) 0=forward marker pulse position
CREATE_PUBVAR(R_TachMax,      int);          // tach ppr * 4
CREATE_PUBVAR(R_TachTst,      int);          // simulated extra tach pulse count
CREATE_PUBVAR(R_TacMaxHalf,   int);          // tach ppr * 2
CREATE_PUBVAR(R_MrkrKnown,    unsigned);     // Marker has been seen
CREATE_PUBVAR(R_SuperSync,    unsigned);     // Super-synchronous operation
CREATE_PUBVAR(R_TachFrac,     unsigned);     // tics since last tach pulse
CREATE_PUBVAR(R_TachLtchTm,   unsigned);     // previous value for free running cntr at tach latch
CREATE_PUBVAR(R_TachLtTmDt,   unsigned);     // delta of tach R_TachLtchTm
CREATE_PUBVAR(R_TachMode,     unsigned);     // tach calc mode, 0=integer calc,1=fractional calc
CREATE_PUBVAR(R_SRAFIndxT2,   unsigned);     // RAF index
CREATE_PUBVAR(R_SRAFIndxTc,   unsigned);     // RAF index
CREATE_PUBVAR(R_TachPulsesT2, float);
CREATE_PUBVAR(R_TachLtTmDtT2, unsigned);
CREATE_PUBVAR(R_TachFracT2,   unsigned);
CREATE_PUBVAR(R_SpdSimT2 ,    float);        // the simulated speed in task 2 from the packed sim class
CREATE_PUBVAR(R_TrqErrAlmFlg, unsigned);     // Torque error alarm flag
CREATE_PUBVAR(TrqCmdInTrT3,   float);
CREATE_PUBVAR(R_TrqCalT3,     float);
// RotorProcessSfb2()                                                                    r
CREATE_PUBVAR(R_AngCorPIGn      , float);


// Unpublished Variables
//----------------------
DSPTachCfg      R_DspTachCfg;                // AEAA Tach configuration parameters
float           R_RAFSpdT2Stk[SPD_RAF_MAX_N];//  RAF array for task2 speed (rad/s)
float           R_RAFSpdTcStk[SPD_RAF_MAX_N];//  RAF array for task2 speed (rad/s)


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Data Passing
//-------------
DATA_PASS(SimPlant1x.R_SpdSim  , R_SpdSimT2, T1A_T2, FILTERED);
DATA_PASS(R_TrqCmdTrT2      , TrqCmdInTrT3 , T2_T3 , FILTERED);
DATA_PASS(R_TrqCalT2        , R_TrqCalT3   , T2_T3 , FILTERED);



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Processing of raw speed feedback.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorProcessSfb1()
{
    float temp;
    // Calculate speed feedback
    R_SpdRaw     = (((int)R_BridgeRebf.TachValue - (int)TachValueZ1)<<16)>>16; // force modulo math
            
    if ( PARM(TestMode) == PUMP_BACK )
    {
        R_Spd    = 0.0F;
        R_SpdRaw = 0;
        return;
    }

    if ( PARM(R_RtrPosMeth) == OLD_SPD_ANGL_METH )
    {
       R_Spd        = R_SpdRaw * (R_SpdScl * FpllFrqAdj) ;
    }
    else
    {
       R_Spd        = R_SpdSlowT1;
    }

    R_SpdT1      = R_Spd * RAD_TO_RPM;
    R_TachLtTmDt = ((R_BridgeRebf.TachTm - R_TachLtchTm)) & 0x03ff;  // only 10 bits in HSLA fpga
    R_TachFrac   = R_BridgeRebf.TachPm;

               // Process position information ( algorithm from B0spd in acmvdc6-u )

    // Calculate raw speed feedback i.e. delta tach counts
    R_TachPulses = R_SpdRaw;

    // Position indicator for turbine control 
    R_RtrPosPulses = R_RtrPosPulses + R_SpdRaw;
    if ( R_SpdRaw >= 0) //forward rotation 
    {
       if (R_RtrPosPulses >= PARM(R_TachPosMax)) //use of "=" insures that R_TachPosMax will not be output
       {
            R_RtrPosPulses   = R_RtrPosPulses - PARM(R_TachPosMax);
       }
    } 
    else  //reverse rotation
    {
       if (R_RtrPosPulses < 0)    // if reult is zero, leave it alone.  No need to integrate and also gets rid of special case that wold land at R_TachPosMax
       {
              R_RtrPosPulses = PARM(R_TachPosMax) + R_RtrPosPulses;
       }
    }                                                        
    R_RtrHssPos = R_RtrPosPulses * PARM(R_TachPosScl); 

    // Process position information
    R_AngRaw    = (((int)R_BridgeRebf.TachValue - (int)R_BridgeRebf.TachMrkrValue)<<16)>>16; /* force modulo math */

    // Integrate position change
    R_TachIntgrl = R_TachIntgrl + R_TachPulses;
    if (R_TachIntgrl >= R_TachMax)
        R_TachIntgrl = R_TachIntgrl - R_TachMax;
    else if (R_TachIntgrl < 0)
        R_TachIntgrl = R_TachIntgrl + R_TachMax;

    if ((((int)R_BridgeRebf.TachMrkrValue) != R_MrkrPos)) // && (R_MrkrTst == 0) ) for test
    {   // new marker
        if (R_MrkrKnown)
        {   // marker pulse has been seen before-check for errors in tach pulse count

            // compute deviation from expected tach count at marker pulse
            R_MrkrErr = R_TachIntgrl - R_AngRaw;

            if (R_MrkrErr >= R_TacMaxHalf)
            {
                R_MrkrErr = R_MrkrErr - R_TachMax;
            }
            else if (R_MrkrErr <= -R_TacMaxHalf)
            {
                R_MrkrErr = R_MrkrErr + R_TachMax;
            }

            // check for tach pulse error above alarm threshold
            if (ABS(R_MrkrErr) >= ((int)PARM(R_TachPlsThr)))
            {
                if ( R_BridgeRebf.BrgHdwDataVld )
                {
                    R_TacBadFlag = 1.0F;
                }
                else
                {
                    R_TacBadFlag = 0.0F;
                    R_MrkrKnown  = false;
                    R_MrkPosInit = (int)R_BridgeRebf.TachMrkrValue;
                }
            }
            else
            {
                R_TacBadFlag = 0.0F;
            }

            // check for tach pulse error above fault threshold
            if ( (ABS(R_MrkrErr) >= PARM(R_TachPlsFlt)) )
            {   // error above threshold-check for 3 consecutive bad errors and adjust tach position integral
                R_TachBadCnt +=1;
                if (R_TachBadCnt >= 3)
                {   // 3 consecutive bad errors
                    PUSH_DIAG(R_TachBadFlt);
                    R_TachFlt = true;
                }
            }
            else
            {   // error within fault threshold-clear bad error counter and adjust tach position integral
                R_TachBadCnt = 0;
            }

            // limit error correction to marker error threshold
            if (R_MrkrErr >= PARM(R_MrkrErrLim))
            {
                R_TachIntgrl -= PARM(R_MrkrErrLim);
            }
            else if (R_MrkrErr <= -PARM(R_MrkrErrLim))
            {
                R_TachIntgrl += PARM(R_MrkrErrLim);
            }
            else
            {
                R_TachIntgrl -= R_MrkrErr;
            }
        }
        else
        {
            // first new marker
            temp = cVectorf::Modulo2Pi(cVectorf::Modulo2Pi(R_AngRaw * R_PosScl) -  cVectorf::Modulo2Pi(R_TachIntgrl * R_PosScl));
            if (temp > PI) temp = temp - PI2;

            // reset tach integrator to match marker
            if (R_AngRaw >= 0)
            {
                R_TachIntgrl = R_AngRaw;
            }
            else
            {
                R_TachIntgrl = R_TachMax + R_AngRaw;
            }
        }
        R_DelMrkrPls = R_AngRaw; // reset tach pulse edges since last marker
    }
    else
    {   // no new marker-check for marker loss

        // accumulate pulses since marker for diagnostics
        R_DelMrkrPls += R_TachPulses;

        // check for marker loss alarm
        if (ABS(R_DelMrkrPls) >= R_MrkrPlsAlm)
        {
            R_MrkLosFlag = 1.0F;   // Flag MrkrLoss to task 2
        }
        else
        {
            R_MrkLosFlag = 0.0F;
        }

        // check for marker loss fault
        if ( ABS(R_DelMrkrPls) >= R_MrkrPlsFlt )
        {
                PUSH_DIAG(R_MrkLossFlt);
                R_TachFlt = true;
                R_DelMrkrPls = 0;
        }
    }

    R_TachLtchTm = R_BridgeRebf.TachTm;


    // test code to simulate tach pulse error
    // R_TachIntgrl = R_TachIntgrl + R_TachTst;

    R_TrqTrkFil1 = R_TrqTrkFil2T1;
    // check for torque out of control if tach error in progress
    if ( (R_TachBadCnt !=0) && ( (ABS(R_TrqTrkFil1)*R_TrqRatRcp) > PARM(R_TrqTrkThr)) )
    {   //torque out of control-don't wait for three revolutions, fault immediattely
        PUSH_DIAG(R_TachBadFlt);
        R_TachFlt = true;
    }


    // update images of counters
    R_PosRaw  = (int)R_BridgeRebf.TachValue;

    //-------------------------------------------------------------//
    //            tach problem simulator                           //
    // simulates lost or delayed marker pulses                     //
    // 1)to simulate lost marker pulses                            //
    //   set R_MrkrTst to number of marker pulses to miss           //
    // 2)to simulate delayed marker pulse                          //
    //   set R_MrkrLate=1                                           //
    //   set R_MrkrTst=1                                            //
    //-------------------------------------------------------------//
    //if (R_MrkrTst != 0)
    //{   /* simulate lost and late marker(s) */
    //    if ( (((int)HifiHdwFbk.Mrkr_3) != R_MrkrPosOld) ||
    //        (R_MrkrLate && (R_MrkLosFlag !=0.0)) )
    //
    //   {   /* new marker */
    //           R_MrkrTst -= 1;  /* decrement number of markers to skip */
    //   }
    //   R_MrkrPosOld = (int)HifiHdwFbk.Mrkr_3;
    //}
    //else
    //{
    R_MrkrPos = (int)R_BridgeRebf.TachMrkrValue;
    R_MrkrPosOld = R_MrkrPos;
    //}

    // Makes a copy of Tach counts be used in calculating deltas. It must be
    // run only once at Task1 rate after processing bridge and speed feedbacks.
    TachValueZ1 = R_BridgeRebf.TachValue;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Processes raw speed feedback counts passed from Task 1. Also, populate
//      speed signal associated with tachometer.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  RotorProcessSfb2()
{
    float R_AngEstIntZ1;

    if ( !SimMode || DsimMode )
    {
       //////////////////////////
       // Start Fractional Tach Alg
       //////////////////////////

       // pick up latched counters from Task 1 and unaverage delta counts
       R_TachNs1 = R_TachPulsesT2 * R_T1FramesPerT2;
	   
	   if ( DsimMode )
       {
            R_TachDt = DelTm2;
       }
       else
       {
       R_TachDt  = R_TachLtTmDtT2 * PARM(R_TachFrcScl);
	   }
	   
       R_TachT1  = R_TachFracT2   * PARM(R_TachLtScl);


       // select mode-fractional calc or integer calc
       if (ABS(R_TachNs1 + R_TachNs0) < 1.99F)
       {
          R_TachMode = INTRG_MODE;      // calc speed using integer # pulses/dt
       }
       else if (ABS(R_TachNs1 + R_TachNs0) > 3.99F)
       {
          R_TachMode = FRACT_MODE;      // calc speed using fractional # pulses/dt
       }
       R_TachNs0 = R_TachNs1;

       // Ts1 = actual time for Ns1 tach pulses
       R_TachTs1 = R_TachDt + R_TachT0 - R_TachT1;

       R_TachDtRcp = NoNANLim( (1.0F/R_TachDt), 1.0e6F, -1.0e6F );     // is this equal to R_DelTm1Rcp on Markstat?

       R_TachTs1Rcp = NoNANLim( (1.0F/R_TachTs1), 1.0e6F, -1.0e6F );


       // E1 = estimated fractional value of tach pulse at end of sample
       R_TachE1 = R_TachT1 * R_TachNs1 * R_TachTs1Rcp;

       if (ABS(R_TachE1) > 1.5F)
       {
          R_TachE1 = R_TachE0;  // bad data so force E0 and E1 to cancel
       }


       if (R_TachMode == INTRG_MODE)
       {   // integer tach pulse mode selected
           R_SpdRawT2 = R_TachNs1;
           if ( R_TachNs1 != 0.0F )
           {   // save old values for fractional tach mode
               R_TachE0 = R_TachE1;
               R_TachT0 = R_TachT1;
           }
           else
           {   // no tach pulses so clear old values
               R_TachE0 = 0.0F;
               R_TachT0 = 0.0F;
           }
       }
       else
       {   // fractional tach pulse mode selected
           if ( R_TachNs1 == 0.0F )
           {   // no tach pulses
               R_SpdRawT2 = 0.0F;
               // don't update E0 or T0
           }
           else
           {
               R_SpdRawT2 = R_TachNs1 + R_TachE1 - R_TachE0;
               R_TachE0   = R_TachE1;
               R_TachT0   = R_TachT1;
           }

       }
       // Calculate basic speed variables for task2
       R_SpdT2   = R_SpdRawT2 * PARM(R_SpdScl2) * R_TachDtRcp;
       R_SpdTach = R_SpdT2 * RAD_TO_RPM;
       R_WRtrT2  = R_SpdT2 * PARM(R_PolePairs);
       R_SpdGen  = R_SpdTach;

       //////////////////////////
       // End Fractional Tach Alg
       //////////////////////////

    }
    else
    {
        // Calculate basic speed variables for task2
        R_SpdT2   = R_SpdSimT2;
        R_SpdTach = 0.0F;

        R_WRtrT2  = R_SpdT2 * PARM(R_PolePairs);
        R_SpdGen  = R_SpdT2 * RAD_TO_RPM;
    }

    /////////////////////////////////////////////////////////////////
    //  speed filter (speed to outer-loop) --was in task 1
    /////////////////////////////////////////////////////////////////
    R_SpdCalInt = R_SpdCalInt + DelTm2*PARM(R_WSpdT2) * (R_WRtrT2 - R_SpdCalInt);
    R_SpdFbkT2  = R_SpdCalInt * R_PolePrRcp;

    // Start Rolling average filter for filter 1
    R_SRAFIndxT2++;
    if (R_SRAFIndxT2 >= R_SpdT2RAFN)  //0,1,2,3....RAFN
    {
       R_SRAFIndxT2 = 0;
    }

    R_SpdRAFT2N     = R_RAFSpdT2Stk[R_SRAFIndxT2];   //get oldest member before writing over that member..oldest is one coming up
    R_SpdT2RAF      = R_SpdT2RAF + (R_SpdT2 - R_SpdRAFT2N + (R_SpdRAFErr * PARM(R_SpdRAFCrGn)) ) * R_SpdRAFNRcp;
    R_SpdRAFErr     = R_SpdT2 + R_SpdRAFT2N - (2.0F * R_SpdT2RAF);
    R_RAFSpdT2Stk[R_SRAFIndxT2] = R_SpdT2;

    // Start Rolling average filter for filter 1
    R_SRAFIndxTc++;
    if (R_SRAFIndxTc >= R_SpdTcRAFN)  //0,1,2,3....RAFN
    {
       R_SRAFIndxTc = 0;
    }

    R_SpdRAFTcN     = R_RAFSpdTcStk[R_SRAFIndxTc];   //get oldest member before writing over that member..oldest is one coming up
    R_SpdTcRAF      = R_SpdTcRAF + (R_SpdT2 - R_SpdRAFTcN + (R_SpdRAFErr2 * PARM(R_SpdRAFCrGn)) ) * R_SpdRAFNTRcp;
    R_SpdRAFErr2    = R_SpdT2 + R_SpdRAFTcN - (2.0F * R_SpdTcRAF);
    R_RAFSpdTcStk[R_SRAFIndxTc] = R_SpdT2;

    /////////////////////////////////////////////////////////////////
    //  Task2 portion of angle correction path
    /////////////////////////////////////////////////////////////////

    R_WRtrRAFT2 = R_SpdT2RAF * PARM(R_PolePairs);

    //Angle correction path
    R_TachPosInt = R_TachPosInt + R_AngEstErT2;
    R_AngCorr    = PARM(R_AngCorW) * (R_AngEstErT2 + (R_AngCorPIGn * R_TachPosInt) );

    //Feed-forward compensation
    R_AngEstIntZ1 = R_AngEstInt;
    R_AngEstInt   = R_AngEstInt  + (R_AngEstFFGn * (R_WRtrRAFT2 - R_AngEstInt));
    R_AngEstFF    = PARM(R_AngFFGn) * (R_AngEstInt - R_AngEstIntZ1);//Z1 could have been used in above filter, but not needed there

    //Final output
    R_WRtrSlowT2 = R_WRtrRAFT2 + R_AngEstFF + R_AngCorr;
    R_SpdSlowT2  = R_WRtrSlowT2 * R_PolePrRcp;
    R_AngEstDtT2 = R_DelTm1 * R_WRtrSlowT2;  //rotor dt1 comp used here because integration is done in T1


    /////////////////////////////////////////////////////////////////
    //  Turbine control filtered speed
    /////////////////////////////////////////////////////////////////

    //Old method for turbine control speed feedback, but used for other purposes, as well


    R_SpdTcRAF2  = R_SpdTcRAF * RAD_TO_RPM;
    R_SpdTcFFFil = R_SpdTcFFFil + R_SpdTcFFGn * (R_SpdTcRAF2 - R_SpdTcFFFil);
    R_SpdTCtlT2  = R_SpdTcRAF2 + PARM(R_SpdTcFFRto) * (R_SpdTcRAF2 - R_SpdTcFFFil);


    /////////////////////////////////////////////////////////////////
    //  Diagnostic only
    /////////////////////////////////////////////////////////////////
    R_WRtrFil = R_WRtrFil + DelTm2*PARM(R_WRtrW) * (R_WRtrT2 - R_WRtrFil);


    // Speed feedback in RPM from R_SpdFbkT2->Best value of spd fbk
    R_SpdFbk = R_SpdFbkT2 * RAD_TO_RPM;

    if (R_SpdFbk > PARM(R_RPMSync))
    {
      R_SuperSync = true;
    }
    else
    {
      R_SuperSync = false;
    }


    R_SpdFbkPLC = R_SpdFbkPLC + DelTm2*PARM(R_WSpdFbkPLC)*( R_SpdGen - R_SpdFbkPLC);


        // Algorithm from B0spd

    // new marker-reset marker unknown status
    if (((R_MrkrPosT2) != R_MrkPosInit ) && T2DoneOnce)
    {
        R_MrkrKnown = true;
    }

    // check task 1 alarm flags
    if ( (R_TacBadFlagT2 != 0.0F) && (PARM(TestMode) == TEST_DISABLED) )
    {
        PUSH_DIAG(R_TachBadAlm);
    }

    if ( (R_MrkLosFlagT2 != 0.0F) && (PARM(TestMode) == TEST_DISABLED) )
    {
        PUSH_DIAG(R_MrkLossAlm);
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Calculate average speed in rev per min.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  RotorProcessSfb3()
{
    float tmpf;

    // Calculate R_SpdFbkAvg
    R_SpdFbkAvg = R_SpdFbkT3 * RAD_TO_RPM;  // R_SpdFbkAvg in Rpm


    R_TrqTrkErr  = R_TrqCmdInTtT3 - R_TrqCalT3;
    R_TrqTrkFil += R_TrqTFilGn * (R_TrqTrkErr - R_TrqTrkFil);

    /* Torque error checking */
    if (R_SeqStat.R_TrqEnbStat)
    {
        R_TrqErrFil += (PARM(R_TrqErrW)*DelTm3) * ( (R_TrqTrkErr * R_TrqRatRcp) - R_TrqErrFil);
    }
    else
    {
        R_TrqErrFil  = 0.0;
    }

    /* Trip or alarm on sustained torque error - most likely cause is tach mechanical problem */
    tmpf = fabs(R_TrqErrFil);

    if (tmpf > PARM(R_TrqErFltLv))
    {
        PUSH_DIAG(R_TrqErrFlt);
    }
    else if (tmpf > PARM(R_TrqErAlmLv))
    {
        R_TrqErrAlmFlg = true;
    }
    else if (tmpf < PARM(R_TrqErAlmLv) * 0.5F)
    {
        R_TrqErrAlmFlg = false;
    }

    if (R_TrqErrAlmFlg)
    {
        PUSH_DIAG(R_TrqErrAlm);
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Initializes the tach interface.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  RotorProcessSfbInit(void)
{
    R_DspTachCfg.Sfb1_Gain   = 0;   // Not used yet
    R_DspTachCfg.Spare1      = 0;
    R_DspTachCfg.Sfb1_Clk_Ps = 0;   // Not used yet
    R_DspTachCfg.HiFi12_Mode = 1;   // 0=>VCO up ctrs, 1=> Tach up/dn
    R_DspTachCfg.Tach1_Cpt   = 1;   // 0=> Clk T2, 1=> Clk T1
    R_DspTachCfg.Mrkr_Rset   = 0;   // 1 = zero value in Tach 1 counter
    R_DspTachCfg.Sfb4_Gain   = 0;   // Not used yet
    R_DspTachCfg.Spare2      = 0;
    R_DspTachCfg.Sfb4_Clk_Ps = 0;   // Not used yet
    R_DspTachCfg.HiFi45_Mode = 0;   // 0=>VCO up ctrs, 1=> Tach up/dn
    R_DspTachCfg.Tach4_Cpt   = 1;   // 0=> Clk T2, 1=> Clk T1
    R_DspTachCfg.Spare3      = 0;
    *(DSPTachCfg *)DSP_HCF_ADDR = R_DspTachCfg;   // write Tach config to hardware


                         // from B0spd

    // Read initial tach counts at marker
    //HifiHdwFbk   = *(DSPHifiIn  *)DSP_HIF_ADDR;   // 600010  AEAA hifi ins for Tach and VCOs
    R_MrkPosInit = (int)R_BridgeRebf.TachMrkrValue;
    R_MrkrPos    = R_MrkPosInit;

    // Determine speed and position feedback scaling
    R_TachMax    = (PARM(R_TachPPR) * PULSE_SCL);
    R_TacMaxHalf = R_TachMax/2;
    R_PosScl     = PI2 * PARM(R_PolePairs) / R_TachMax;

    // Calculate tach diagnostic thresholds
    R_MrkrPlsAlm = PARM(R_MrkrTplThr) + R_TachMax;                          // pulse edges
    R_MrkrPlsFlt = PARM(R_MrkrTplFlt) + (R_TachMax * PARM(R_MrkrMisFlt));   // pulse edges

    return;
}
