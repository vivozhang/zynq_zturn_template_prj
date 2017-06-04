///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Turbine Control Simulator
//
// DESCRIPTION:
//      This module contains Wind Turbine calculation functions
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     p0Instru.xls, p0Instru.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "p0Instru.h"
#include "b0BridgeRebf.h"
#include "b0BrgThrmstr.h"
#include "b0BrgThrm5p.h"
#include "b0DbThrm.h"




// Sequence states definition

#define TC_STOPPED          0
#define TC_SPD_RAMP_UP      1
#define TC_STABILIZE        2
#define TC_CONV_RUN_WAIT    3
#define TC_ON_LINE          4
#define TC_POWER_RAMP_DOWN  5
#define TC_CONV_STOP_WAIT   6
#define TC_WIND_FREE        7



// Constants
//----------


// Externals
//----------
  extern unsigned     TripFlt;
  extern unsigned     R_Running;
  extern float        WBDelSpdPwr_kW;
  extern float        R_TrqRatRcp;
  extern unsigned     VARModeCmd;
  extern unsigned     RunRdy;
  extern float        DelTm3;
  extern int          SeqSt;
  extern unsigned     TempTrpLatch;
  extern float        PwrLimDelTorque_KNm;
  extern float        PwrLimDelSpeed;
  extern float        LclPfRefx;
  extern float        PfLim;
  extern float        PfAdj;
  extern float        PfCmd;
  extern float        VarPfScale;
  extern float        LclTrqRef;
  extern float        R_SpdRefSim;
  extern float        TurbineNpRat;
  extern float        LclVarRefx;


// Types and Classes
//------------------


// Parameters
//-----------

 //Turbine Control Simulator
 CREATE_PARM(TcPwrMaxCmd,    float);     //  Simulated max turbine power output
 CREATE_PARM(TcPwrMinCmd,    float);     //  Simulated min turbine power output
 CREATE_PARM(TcPwrRestCmd,   float);     //  Simulated resting turbine power output
 CREATE_PARM(TcPwrRampTm,    float);     //  Simulated time for turbine power to reach targetted power setpoint
 CREATE_PARM(TcPwrRmpRat,    float);     //  Simulated turbine power ramp rate
 CREATE_PARM(TcSimEnb,       unsigned);  //  Enable turbine control simulation mode
 CREATE_PARM(TcRPNWAlways,   unsigned);  //  Enable turbine control simulation mode
 CREATE_PARM(TcSpd1,         float);     //  First speed point
 CREATE_PARM(TcSpd2,         float);     //  Second speed point
 CREATE_PARM(TcSpd3,         float);     //  Third speed point
 CREATE_PARM(TcSpd4,         float);     //  Fourth speed point
 CREATE_PARM(TcPwr1,         float);     //  First power point
 CREATE_PARM(TcPwr2,         float);     //  Second power point
 CREATE_PARM(TcPwr3,         float);     //  Third power point
 CREATE_PARM(TcPwr4,         float);     //  Fourth power point
 CREATE_PARM(TcStabTm,       float);     //  Simulated min turbine power output
 CREATE_PARM(TcPwrReq,       float);     //  Steady state power
 CREATE_PARM(TcSpdMin,       float);     //  Min turbine speed
 CREATE_PARM(TcStallSpd1,    float);     //  First speed point
 CREATE_PARM(TcStallSpd2,    float);     //  Second speed point
 CREATE_PARM(TcStallSpd3,    float);     //  Third speed point
 CREATE_PARM(TcStallSpd4,    float);     //  Fourth speed point


  // End Turbine Control Simulator

// Published Variables
//--------------------
// Turbien Control Simulator
CREATE_PUBVAR(LclPfRefx,      	   float);          // Local power factor command

CREATE_PUBVAR(TcPwrCmd,            float);            // Turbine power output command
CREATE_PUBVAR(TcPwrSetPt,          float);            // Turbine power setpoint
CREATE_PUBVAR(TcPwrSetPtC,         float);            // Clamped turbine power setpoint
CREATE_PUBVAR(TcPwrDelRef,         float);            // Delta between turbine power output and setpoint
CREATE_PUBVAR(TcPwrRmpMax,         float);            // Max ramp rate of turbine power output
CREATE_PUBVAR(TcPwrDstb,           float);            // Disturbance into turbine power
CREATE_PUBVAR(TcTrqCmd,            float);            // Turbine torque command
CREATE_PUBVAR(TcTrqCmdZ1,          float);            // Turbine torque command
CREATE_PUBVAR(TcSpdCmd,            float);            // Turbine speed command
CREATE_PUBVAR(TcCnvPwrCmd,         float);            // Converter power command
CREATE_PUBVAR(TcSpdDstb,           float);            // Disturbance into turbine speed
CREATE_PUBVAR(TcVARCmd,            float);            // Turbine reactive power command
CREATE_PUBVAR(TcVARCmdZ1,          float);            // Turbine reactive power command
CREATE_PUBVAR(TcSpdOut1,           float);            // Turbine speed output
CREATE_PUBVAR(TcPwrSlp1,           float);            // Turbine speed ramp slope calc for phase 1
CREATE_PUBVAR(TcPwrSlp2,           float);            // Turbine speed ramp slope calc for phase 2
CREATE_PUBVAR(TcPwrSlp3,           float);            // Turbine speed ramp slope calc for phase 3
CREATE_PUBVAR(TcPwrSlp4,           float);            // Turbine speed ramp slope calc for phase 4
CREATE_PUBVAR(TcCnvRunCmd,      unsigned);            // Turbine converter run command
CREATE_PUBVAR(TcRPNWReq,        unsigned);            // Turbine rpnw run request
CREATE_PUBVAR(TcCnvRPNWReq,     unsigned);            // Turbine control to converter rpnw run request
CREATE_PUBVAR(TcSeqSt,               int);            // State of the TurbineControlSimulator Sequence
CREATE_PUBVAR(TcStabTmr,           float);            // Turbine stablization timer
CREATE_PUBVAR(TcRunCmd,         unsigned);            // Turbine control run command
CREATE_PUBVAR(TcSimMode,        unsigned);            // Turbine control sim mode

CREATE_PUBVAR(TcSpdMinVsTrq,       float);            // Turbine min apeed vs torque
CREATE_PUBVAR(TcTrqCmd0,           float);            // Turbine torque command 0
CREATE_PUBVAR(TcSpdCmd0,            float);           // Turbine speed command 0
CREATE_PUBVAR(TcDelTrq,             float);           // Turbine delta torque
CREATE_PUBVAR(TcDelSpd,             float);           // Turbine delta speed




CREATE_PUBVAR(TcPwrReq,            float);            // Turbine power output request
CREATE_PUBVAR(TcPwrReqTot,         float);            // Total turbine power request
CREATE_PUBVAR(TcTrqReq,            float);            // Turbine torque request
CREATE_PUBVAR(TcSpdReq,            float);            // Turbine speed request
CREATE_PUBVAR(TcPwrSlp21,           float);            // Turbine speed ramp slope 2 calc for phase 1
CREATE_PUBVAR(TcPwrSlp22,           float);            // Turbine speed ramp slope 2  calc for phase 2
CREATE_PUBVAR(TcPwrSlp23,           float);            // Turbine speed ramp slope 2 calc for phase 3
CREATE_PUBVAR(TcPwrSlp24,           float);            // Turbine speed ramp slope 2 calc for phase 4


CREATE_PARM(TcDelTrqHyst,       float);     // Hysteresis for delta torque request
CREATE_PARM(TcDelSpdHyst,       float);     // Hysteresis for delta speed request
CREATE_PUBVAR(TcDelTrqAct,      float);     // Delta torque request after hysteresis
CREATE_PUBVAR(TcDelSpdAct,      float);     // Delta speed request after hysteresis
CREATE_PUBVAR(TcDelTrqMax,      float);     // Max torque with de-rating
CREATE_PUBVAR(TcDelSpdMax,      float);     // Max speed with de-rating
CREATE_PARM(TcTrqRat,         float);
CREATE_PARM(TcSpdRat,         float);
// Unpublished Variables


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      TurbineControlSimRunRules
//
// EXECUTION LEVEL
//
// RETURN VALUE
//
//
///////////////////////////////////////////////////////////////////////////////


void TurbineControlSimRules(void)
{
    float sign;
    float Pf_Tmp;

    // Set final values for TcPwrMaxCmd and TcPwrMinCmd
    SET_FV(TcPwrMinCmd, 0.0F);
    SET_FV(TcPwrMaxCmd, PARM(TurbineNpRat));

    // Calculate Simulated Turbine Control Ramp Rate
    SET_FV(TcPwrRmpRat, (PARM(TcPwrMaxCmd) - PARM(TcPwrMinCmd)) / PARM(TcPwrRampTm));



    // Set final values for TcPwr
    // 60Hz
    if ( PARM(L_FrqInput) == 60.0F)
    {
       if ( PARM(TurbineNpRat) <= 1.67e3F)
       {
           SET_FV(TcPwr1, (246.0F/1620.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr2, (391.0F/1620.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr3, (686.0F/1620.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr4, (1620.0F/1620.0F)*PARM(TurbineNpRat));
       }
       else
       {
           SET_FV(TcPwr1, (151.0F/1850.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr2, (296.0F/1850.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr3, (529.0F/1850.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr4, (1850.0F/1850.0F)*PARM(TurbineNpRat));
       }
    }
    else  // 50hz
    {
       if ( PARM(TurbineNpRat) <= 1.67e3F)
       {
           SET_FV(TcPwr1, (246.0F/1620.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr2, (391.0F/1620.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr3, (686.0F/1620.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr4, (1620.0F/1620.0F)*PARM(TurbineNpRat));
       }
       else
       {
           SET_FV(TcPwr1, (151.0F/1850.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr2, (296.0F/1850.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr3, (529.0F/1850.0F)*PARM(TurbineNpRat));
           SET_FV(TcPwr4, (1850.0F/1850.0F)*PARM(TurbineNpRat));
       }
    }

    SET_FV(TcSpdMin, (PARM(R_RPMMinOp) + 30.0F));
    // 60Hz
    if ( PARM(L_FrqInput) == 60.0F)
    {
        // Set final values for TcSpd and TcSpdMin
        SET_FV(TcSpd1, MIN(PARM(TcSpdMin), (1000.0F/1520.0F)*PARM(R_RPMNom)));
        SET_FV(TcSpd2, (1000.0F/1520.0F)*PARM(R_RPMNom));
        SET_FV(TcSpd3, (1200.0F/1520.0F)*PARM(R_RPMNom));
        SET_FV(TcSpd4, (1520.0F/1520.0F)*PARM(R_RPMNom));

        SET_FV(TcStallSpd1, PARM(TcSpd1));
        SET_FV(TcStallSpd2, MAX(PARM(TcStallSpd1), PARM(TcSpd2)-200.0F));
        SET_FV(TcStallSpd3, MAX(PARM(TcStallSpd2), PARM(TcSpd3)-200.0F));
        SET_FV(TcStallSpd4, MAX(PARM(TcStallSpd3), PARM(TcSpd4)-200.0F));
    }
    else
    {
        // Set final values for TcSpd and TcSpdMin
        SET_FV(TcSpd1, MIN(PARM(TcSpdMin), (1260.0F/1915.0F)*PARM(R_RPMNom)));
        SET_FV(TcSpd2, (1260.0F/1915.0F)*PARM(R_RPMNom)); //calculated from 60Hz ratio -- 1000/1520*1915
        SET_FV(TcSpd3, (1511.0F/1915.0F)*PARM(R_RPMNom)); //calculated from 60Hz ratio -- 1200/1520*1915
        SET_FV(TcSpd4, (1915.0F/1915.0F)*PARM(R_RPMNom));

        SET_FV(TcStallSpd1, PARM(TcSpd1));
        SET_FV(TcStallSpd2, MAX(PARM(TcStallSpd1), PARM(TcSpd2)-200.0F));
        SET_FV(TcStallSpd3, MAX(PARM(TcStallSpd2), PARM(TcSpd3)-200.0F));
        SET_FV(TcStallSpd4, MAX(PARM(TcStallSpd3), PARM(TcSpd4)-200.0F));
    }


    TcPwrRmpMax   =  PARM(TcPwrRmpRat) * DelTm3;
    TcPwrSlp1     =  PARM(TcSpd1) / PARM(TcPwr1);
    TcPwrSlp2     = (PARM(TcSpd2) - PARM(TcSpd1)) / (PARM(TcPwr2) - PARM(TcPwr1));
    TcPwrSlp3     = (PARM(TcSpd3) - PARM(TcSpd2)) / (PARM(TcPwr3) - PARM(TcPwr2));
    TcPwrSlp4     = (PARM(TcSpd4) - PARM(TcSpd3)) / (PARM(TcPwr4) - PARM(TcPwr3));


    TcPwrSlp21    =  PARM(TcStallSpd1) / PARM(TcPwr1);
    TcPwrSlp22    = (PARM(TcStallSpd2) - PARM(TcStallSpd1)) / (PARM(TcPwr2) - PARM(TcPwr1));
    TcPwrSlp23    = (PARM(TcStallSpd3) - PARM(TcStallSpd2)) / (PARM(TcPwr3) - PARM(TcPwr2));
    TcPwrSlp24    = (PARM(TcStallSpd4) - PARM(TcStallSpd3)) / (PARM(TcPwr4) - PARM(TcPwr3));

	LclPfRefx    = PARM(LclPfRef);
    if ( PARM(LclPfRef) < 0.0F )
    {
        sign = -1.0F;
    }
    else
    {
        sign = 1.0F;
    }

    Pf_Tmp = LclPfRefx;

    //tre files clamp from -1.0 to 1.0 -- Clamp abs value to min of 0.9 here
    if (fabs(Pf_Tmp) <  PARM(PfLim)) Pf_Tmp =  sign * PARM(PfLim);

    PfAdj = Pf_Tmp;
    VarPfScale = sign * sqrt(1.0F - PfAdj*PfAdj)/(fabs(PfAdj));

    /* Calculate Iy factor needed to balance Ix for desired pwr factor from real power  */
    PfCmd        = PfAdj;

    SET_FV(TcDelTrqHyst,     0.0F);
    SET_FV(TcDelSpdHyst,     0.0F);

    SET_FV(TcSpdRat,    PARM(PwrLimSpdRatTc));
    SET_FV(TcTrqRat,    PARM(TurbineNpRat)/(PARM(PwrLimSpdRatTc)*RPM_TO_RAD));
}




///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      TurbineControlSimulator
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//
//
///////////////////////////////////////////////////////////////////////////////

void TurbineControlSequencer(void)
{

    if (TripFlt)
    {
        TcRunCmd = FALSE;
    }

    switch ( TcSeqSt )
    {
        case TC_STOPPED:
            TcPwrSetPt = PARM(TcPwrRestCmd);
            TcStabTmr = 0;
            TcCnvRunCmd  = FALSE;
            TcCnvRPNWReq = FALSE;
            if (TcRunCmd && !TripFlt)
            {
                TcSeqSt = TC_SPD_RAMP_UP;
            }
            if ( (TcRPNWReq || PARM(TcRPNWAlways)) && !TripFlt )
            {
                TcSeqSt = TC_WIND_FREE;
            }
            break;

        case TC_SPD_RAMP_UP:
            if (TripFlt || !TcRunCmd)
            {
                TcSeqSt = TC_STOPPED;
            }
            else
            {
                TcPwrSetPt = PARM(TcPwrMinCmd);
                if ( TcPwrCmd == PARM(TcPwrMinCmd) )
                {
                    TcSeqSt = TC_STABILIZE;
                }
            }
            break;

        case TC_STABILIZE:
            if (!RunRdy || !TcRunCmd)
            {
                TcSeqSt = TC_STOPPED;
            }
            else
            {
                TcPwrSetPt = PARM(TcPwrMinCmd);
                TcStabTmr = TcStabTmr + DelTm3;
                if ( TcStabTmr >= PARM(TcStabTm))
                {
                    TcSeqSt = TC_CONV_RUN_WAIT;
                }
            }
            break;

        case TC_CONV_RUN_WAIT:
            if (!RunRdy || !TcRunCmd)
            {
                TcSeqSt = TC_STOPPED;
                TcCnvRunCmd = FALSE;
            }
            else
            {
                TcPwrSetPt = PARM(TcPwrMinCmd);
                TcCnvRunCmd = TRUE;
                if (R_Running)
                {
                    TcSeqSt = TC_ON_LINE;
                }
            }
            break;

        case TC_ON_LINE:
            if (TripFlt || !RunRdy ||  !R_Running)
            {
                TcSeqSt = TC_STOPPED;
            }
            else
            {
                TcPwrSetPt = PARM(TcPwrMaxCmd);
                if (!TcRunCmd || TempTrpLatch)
                {
                    TcSeqSt = TC_POWER_RAMP_DOWN;
                }
            }
            break;


        case TC_POWER_RAMP_DOWN:
            TcPwrSetPt = PARM(TcPwrMinCmd);
            TcRunCmd = FALSE;
            if (TcPwrCmd == PARM(TcPwrMinCmd))
            {
                TcSeqSt = TC_CONV_STOP_WAIT;
            }
            break;

        case TC_CONV_STOP_WAIT:
            TcPwrSetPt = PARM(TcPwrMinCmd);
            TcCnvRunCmd = FALSE;
            if (SeqSt == STOPPED_COMMON)
            {
                TcSeqSt = TC_STOPPED;
            }
            if (  ( (SeqSt == CONV_RUNNING_LINE) && TcRPNWReq )  || PARM(TcRPNWAlways) )
            {
                TcSeqSt = TC_WIND_FREE;
            }

            break;

        case TC_WIND_FREE:
            TcPwrSetPt = PARM(TcPwrRestCmd);
            TcStabTmr = 0;
            if ( (!TcRPNWReq && !PARM(TcRPNWAlways)) || TripFlt)
            {                                
                TcSeqSt = TC_STOPPED;
            }
            if (TcRunCmd && !TripFlt)
            {
                TcSeqSt = TC_SPD_RAMP_UP;
            }
            TcCnvRPNWReq = TRUE;
            TcCnvRunCmd  = FALSE;
            break;


        default:    /* Invalid Sequencer State */
            TcSeqSt = TC_STOPPED;
            break;
     }   /* End of switch(TcSeqSt) */

}




///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      TurbineControlSimulator
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      Calculate simulated turbine output for torque, speed, VAR, Phi
//
///////////////////////////////////////////////////////////////////////////////

void TurbineControlSimulator(void)
{
    float TcPwrReqTotTmp;
    
    TcTrqCmdZ1 = TcTrqCmd;
    TcVARCmdZ1 = TcVARCmd;

    TcSimMode = PARM(TcSimEnb);

    TurbineControlSequencer();

    TcPwrSetPtC = TcPwrSetPt;

    ////////////////////////////////////////////////////////////////////////////
    //  Clamp Power Set Point
    ////////////////////////////////////////////////////////////////////////////

    if(TcPwrSetPtC > PARM(TcPwrReq))
    {
        TcPwrSetPtC = PARM(TcPwrReq);
    }

    ////////////////////////////////////////////////////////////////////////////
    //  Calculate Turbine Power
    ////////////////////////////////////////////////////////////////////////////
    TcPwrDelRef = TcPwrSetPtC - TcPwrReq;

    // clamping TcPwrDelRef
    if (TcPwrDelRef >= TcPwrRmpMax)
    {
        TcPwrDelRef = TcPwrRmpMax;
    }
    else if (TcPwrDelRef <= -TcPwrRmpMax)
    {
        TcPwrDelRef = -TcPwrRmpMax;
    }

    TcPwrReq = TcPwrReq + TcPwrDelRef;




    ////////////////////////////////////////////////////////////////////////////
    //   Use TcPwrReqTotTmp to ensure TcPwrReqTot always get the clamp value
    ////////////////////////////////////////////////////////////////////////////
    TcPwrReqTotTmp = TcPwrReq + TcPwrDstb;

    if ( TcPwrReqTotTmp < 0.0 )
    {
        TcPwrReqTotTmp = 0.0;
    }

    TcPwrReqTot = TcPwrReqTotTmp;

    ////////////////////////////////////////////////////////////////////////////
    //   Calculate speed and torque output based on turbine power input
    ////////////////////////////////////////////////////////////////////////////
    if ( TcPwrReqTot <= PARM(TcPwr1) )
    {
        TcSpdOut1 = TcPwrSlp1 * TcPwrReqTot;
    }
    else if ( (TcPwrReqTot > PARM(TcPwr1)) && (TcPwrReqTot <= PARM(TcPwr2)) )
    {
        TcSpdOut1 = TcPwrSlp2 * (TcPwrReqTot - PARM(TcPwr1)) + PARM(TcSpd1);
    }
    else if ( (TcPwrReqTot > PARM(TcPwr2)) && (TcPwrReqTot <= PARM(TcPwr3)) )
    {
        TcSpdOut1 = TcPwrSlp3 * (TcPwrReqTot - PARM(TcPwr2)) + PARM(TcSpd2);
    }
    else
    {
        TcSpdOut1 = TcPwrSlp4 * (TcPwrReqTot - PARM(TcPwr3)) + PARM(TcSpd3);
    }

    TcSpdReq = TcSpdOut1;                      


    if (TcSpdReq<= PARM(TcSpdMin))
    {
        TcSpdReq = PARM(TcSpdMin);
    }




    TcTrqReq = TcPwrReqTot / ( TcSpdReq * RPM_TO_RAD );


    ////////////////////////////////////////////////////////////////////////////
    //   Calculate TcPwrCmd for second curve
    ////////////////////////////////////////////////////////////////////////////

    TcDelTrq = PwrLimDelTorque_KNm;
    if (TcDelTrq >= 0.0F)
    {
        TcDelTrqAct = 0.0F;
    }
    else if(TcDelTrq < PARM(TcDelTrqHyst) || TcDelTrqAct < 0.0F)
    {
        TcDelTrqAct = TcDelTrq;
    }

    TcDelTrqMax = PARM(TcTrqRat) + TcDelTrqAct + MAX( (TcTrqReq - PARM(TcTrqRat) ),  0.0F);

    if (TcTrqReq > TcDelTrqMax)
    {
        TcTrqCmd0 = TcDelTrqMax;
    }
    else
    {
        TcTrqCmd0 = TcTrqReq;
    }

    if (TcTrqCmd0 < 0.0)
    {
        TcTrqCmd0 = 0.0;
    }                        
    
    
    TcDelSpd = PwrLimDelSpeed;
             
    if (TcDelSpd >= 0.0F)
    {
        TcDelSpdAct = 0.0F;
    }
    else if(TcDelSpd < PARM(TcDelSpdHyst) || TcDelSpdAct < 0.0F)
    {
        TcDelSpdAct = TcDelSpd;
    }
    TcDelSpdMax = PARM(TcSpdRat) + TcDelSpdAct + MAX( (TcSpdReq - PARM(TcSpdRat) ),  0.0F);
    
    if (TcSpdReq > TcDelSpdMax)
    {
        TcSpdCmd0 = TcDelSpdMax;
    }
    else
    {
        TcSpdCmd0 = TcSpdReq;
    }

    TcPwrCmd = TcTrqCmd0 * (TcSpdCmd0 * RPM_TO_RAD) ;


    ////////////////////////////////////////////////////////////////////////////
    //   Calculate speed and torque cmd output based on curve 2
    ////////////////////////////////////////////////////////////////////////////
    if ( TcPwrCmd <= PARM(TcPwr1) )
    {
        TcSpdMinVsTrq = TcPwrSlp21 * TcPwrCmd;
    }
    else if ( (TcPwrCmd > PARM(TcPwr1)) && (TcPwrCmd <= PARM(TcPwr2)) )
    {
        TcSpdMinVsTrq = TcPwrSlp22 * (TcPwrCmd - PARM(TcPwr1)) + PARM(TcStallSpd1);
    }
    else if ( (TcPwrCmd > PARM(TcPwr2)) && (TcPwrCmd <= PARM(TcPwr3)) )
    {
        TcSpdMinVsTrq = TcPwrSlp23 * (TcPwrCmd - PARM(TcPwr2)) + PARM(TcStallSpd2);
    }
    else
    {
        TcSpdMinVsTrq = TcPwrSlp24 * (TcPwrCmd - PARM(TcPwr3)) + PARM(TcStallSpd3);
    }


    TcSpdCmd = MAX(TcSpdCmd0, TcSpdMinVsTrq) + TcSpdDstb;


    ////////////////////////////////////////////////////////////////////////////
    //   Clamp TcSpdCmd to TcSpdMin
    ////////////////////////////////////////////////////////////////////////////

    if (TcSpdCmd <= PARM(TcSpdMin))
    {
        TcSpdCmd = PARM(TcSpdMin);
    }


    TcTrqCmd = TcPwrCmd / ( TcSpdCmd * RPM_TO_RAD );


    ////////////////////////////////////////////////////////////////////////////
    //   Calculate torque, converter power, VARS, Phi
    ////////////////////////////////////////////////////////////////////////////

    if ( PARM(TcSimEnb))
    {
       TcCnvPwrCmd = TcTrqCmd * ( TcSpdCmd * RPM_TO_RAD );
    }
    else
    {
       TcCnvPwrCmd = PARM(LclTrqRef) * R_SpdRefSim;
    }


    if (VARModeCmd)
    {
        TcVARCmd =  LclVarRefx;
    }
    else
    {
        TcVARCmd = TcCnvPwrCmd  * VarPfScale;
    }





}



