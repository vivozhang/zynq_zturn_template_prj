///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Product code fault checking
//
// DESCRIPTION
//      Application specific procedures to detect and declare faults.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "b0BridgeRebf.h"
#include "y2FltCk.h"
#include "b0DbCtl.h"

// Constants
//----------


// Externals
//----------
extern FaultIsActiveTyp FaultIsActive;
extern cBridgeRebf      L_BridgeRebf;
extern float            L_IocRtryRto;
extern unsigned         L_DbOtDsbFlg;

extern float            DelTm3;
extern float            ExecLoadPk ;
extern float            ExecLoadT1Pk;
extern float            ExecLoadT2Pk;
extern float            ExecLoadT3Pk;
extern unsigned         FpgaFltRstB1;
extern float            L_DelTm1;
extern float            L_IaRmsMag;
extern float            L_IbRmsMag;
extern float            L_IcRmsMag;
extern float            L_IocRtryLim;
extern float            L_IxGndFil;
extern float            L_IxyGndFil;
extern float            L_IyGndFil;
extern float            L_VdcFbk;
extern float            L_VdcFbkT3;

extern unsigned         BicFltStat;
extern unsigned         BrgCardsAct;
extern unsigned         BrgEnbSim;
extern unsigned         ClrFltT2;
extern unsigned         InitDone;
extern unsigned         L_FpgaFltFb2;
extern unsigned         MvSwGearShuntTrp;
extern unsigned         MvSwGearShuntSep;
extern unsigned         MvSwGearTrpReq;
extern unsigned         IslMaOpnReq;
extern unsigned         IslK1OpnReq;
extern unsigned         SimMode;
extern unsigned         SimModeZ1;
extern unsigned         Stopped;
extern unsigned         Brg1TripFlt;
extern unsigned         DiagResetCmdT3;
extern unsigned         Ps24vOk;
extern unsigned         L_BrgFltHdw;
extern unsigned         L_IocPhsA;
extern unsigned         L_IocPhsB;
extern unsigned         L_IocPhsC;
extern unsigned         L_DbAutoTstDone;

extern unsigned             ISimMode;
extern unsigned             L_DSatDx;

// Types and Classes
//------------------
extern cDbCtl    L_DbCtl;

// Diagnostics
//------------
CREATE_DIAG(PowerDown);                             // Power-down trip
CREATE_DIAG(L_IocPhsA);                             // Recurrent bridge over-current trips
CREATE_DIAG(L_IocPhsB);
CREATE_DIAG(L_IocPhsC);
CREATE_DIAG(Ps24NokTrp);                            // 24v pwr supply not ok


// Parameters
//-----------
CREATE_PARM(PwrDnPs24vTm,       float);     // Controller DC PS-24V down time detection
CREATE_PARM(PwrUpPs24vTm,       float);     // Controller DC PS-24V up time detection
// IOC
CREATE_PARM(L_IocThrsReq,       float);     // Instantaneous over-current threshold req.
CREATE_PARM(L_IocRtryReq,       float);     // Inst. over-current retry limit request (sec);
CREATE_PARM(L_IocRtryMax,       float);     // Inst. over-current retry Max. limit.
CREATE_PARM(L_IocDwnRtio,       float);     // Ioc retry down timer ratio

// Variables
//----------
CREATE_PUBVAR(L_IOCTimeout, unsigned);
CREATE_PUBVAR(L_IocRtryCtr, float);                // Ioc retry counter
CREATE_PUBVAR(PwrDnPs24vTmr,float);                // Controller DC PS-24V down timer
CREATE_PUBVAR(PwrUpPs24vTmr,float);                // Controller DC PS-24V up timer
CREATE_PUBVAR(ClrFpgaCtrB1, unsigned);             // State Counter for line FPGA Flt Reset
CREATE_PUBVAR(ClrFpgaHdwB1, unsigned);             // State Counter for Bic/hardware Reset
CREATE_PUBVAR(FpgaHdwRstB1, unsigned);             // Bic/hardware Reset Command Bool
CREATE_PUBVAR(FpgaRstT3B1,  unsigned);

CREATE_PUBVAR(L_DbFltGate,      unsigned);      // DB bridge fault flag
CREATE_PUBVAR(L_DbGateDsb,      unsigned);      // DB enable boolean
CREATE_PUBVAR(L_DbGtState,      unsigned) = DB_GATE_INIT;

CREATE_PUBVAR(PwrDnFbkDsb,  unsigned) = true;      // Power down feedback disable
CREATE_PUBVAR(Ps24vUpOnce,  unsigned);             // Controller DC PS-24V has been up once
CREATE_PUBVAR(Ps24vOkT3,    unsigned);             // Controller DC PS-24V is Ok, data-passed to task3

// Data Passing
//--------------
DATA_PASS(Ps24vOk,      Ps24vOkT3,      T2_T3, UNFILTERED);


// Local Prototypes (to resolve forward references)
//-------------------------------------------------



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs fault checking for task2 as needed.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void FaultChkSourceBIL(void)
{

    if ( SimMode != SimModeZ1)
    {
        L_IaRmsMag   = 0.0;
        L_IbRmsMag   = 0.0;
        L_IcRmsMag   = 0.0;
        L_IxGndFil   = 0.0;
        L_IyGndFil   = 0.0;
        L_IxyGndFil  = 0.0;
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//     This function checks for shunt faults. If any shunt faults are present,
//     this funtion clears the fault, increments a counter and checks the
//     counter value against a threshold. If the threshold has been reached,
//     the appropriate faults are given. This scheme allows the source to keep
//     on running in the face of recurrent shunt faults, up to a limit.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ShuntFltChk(void)
{
    unsigned Phase_A_Flt, Phase_B_Flt, Phase_C_Flt;

    // Read FPGA if not re-setting and not in sim mode
    if (!ClrFpgaCtrB1 && (!ISimMode || BrgEnbSim) && InitDone && !PwrDnFbkDsb)//Allow Gating in Sim-mode
    {
        Phase_A_Flt = L_IocPhsA;
        Phase_B_Flt = L_IocPhsB;
        Phase_C_Flt = L_IocPhsC;
    }
    else
    {
        Phase_A_Flt = false;
        Phase_B_Flt = false;
        Phase_C_Flt = false;
    }


    // Increment Ioc counter if any shunt trip detected.
    if ( Phase_A_Flt || Phase_B_Flt || Phase_C_Flt)
    {
        L_IocRtryCtr = L_IocRtryCtr + L_DelTm1;
    }
    else//Decrement ioc counter if no shunt trip detected
    {
        L_IocRtryCtr = L_IocRtryCtr - (L_DelTm1*L_IocRtryRto);
    }

    // Clamp between 0.0 and L_IocRtryLim
    if ( L_IocRtryCtr <= 0.0)
    {
        L_IocRtryCtr = 0.0;
        L_IOCTimeout = false;

    } //set appropriate fault, causing trip.
    else if (L_IocRtryCtr >= L_IocRtryLim)
    {
        L_IocRtryCtr = L_IocRtryLim;
        if ( Phase_A_Flt )      PUSH_DIAG(L_IocPhsA);
        if ( Phase_B_Flt )      PUSH_DIAG(L_IocPhsB);
        if ( Phase_C_Flt )      PUSH_DIAG(L_IocPhsC);
        L_IOCTimeout = true;                            // On the MACC platform, L_RamDigCmd.IOC_Timeout was written to the
    }                                                   // fpga here to force an immediate trip.  In the MarkStat
                                                        // Brg1TripFlt will remove L_BrgPwrEnbCmd and disable gating
    return;                                             // when the next command packet is sent over HSSL.
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs fault checking associated with processing of
//      line bridge feedbacks.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineProcessFaultFbks(void)
{
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Sequencing of faults associated with dynamic brake.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void DynamicBrakeFaultSequencer(void)
{
    //L_RamDigCmd.Vdc_OV_Flt  = 0;//clear the fault bits -- the bridge has had plenty of time to be disabled
    //L_RamDigCmd.Fast_Shtdwn = 0;
    //L_RamDigCmd.IOC_Timeout = 0;

    L_FpgaFltFb2  = L_BrgFltHdw;

    switch ( L_DbGtState)
    {
        case DB_GATE_INIT:
            L_DbFltGate = false;


            if ( (!L_FpgaFltFb2 || (!BrgCardsAct && !Brg1TripFlt)) && L_DbCtl.DbcDb1AutoTstDone)
            {
                L_DbGtState = DB_GATE_NORMAL;
                L_DbFltGate = false;
                L_DbGateDsb = false;
            }
            else//stay in this state
            {
                L_DbFltGate  = true;
                L_DbGateDsb = true;
            }
            break;

        case DB_GATE_NORMAL:
            L_DbFltGate = false;
            if ( L_DbOtDsbFlg || L_DSatDx)
            {
                L_DbGtState = DB_GATE_DISABLE;
            }
            else if ( ((L_FpgaFltFb2 && BrgCardsAct  ) || Brg1TripFlt)&& (L_VdcFbk > PARM(DBGtOnThrs)))
            {
                L_DbGtState = DB_GATE_FAULT;
                L_DbFltGate = true;
            }
            else //Stay in this state
            {
                L_DbFltGate = false;
                L_DbGateDsb = false;
            }
            break;

        case DB_GATE_FAULT:

            L_DbFltGate = true;
            if ( L_DbOtDsbFlg || L_DSatDx)
            {
                L_DbGtState = DB_GATE_DISABLE;
                L_DbFltGate = true;
                L_DbGateDsb = true;

            }
            else if (L_VdcFbk < PARM(DBGtOffThr))
            {
                L_DbGtState = DB_GATE_NORMAL;
                L_DbFltGate = false;
                L_DbGateDsb = false;
            }

            break;

        case DB_GATE_DISABLE:
            L_DbGateDsb = true;
            L_DbFltGate = true;
            if (!L_DbOtDsbFlg && !L_DSatDx && (L_VdcFbk < PARM(DBGtOffThr)))
            {
                L_DbGtState = DB_GATE_NORMAL;
                L_DbFltGate = false;
                L_DbGateDsb = false;
            }
            break;

        default:
            break;

    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Clears and resets FPGA faults. Requires a four step process:
//           Dsp_Dig_Cmd.Pwr_Elec_Enb =  0  --> When in stopped state
//           Dsp_Dig_Cmd.Drv_Flt_Rst  =  1
//           Dsp_Dig_Cmd.Drv_Flt_Rst  =  0
//           Dsp_Dig_Cmd.Pwr_Elec_Enb =  1  --> When Starting
//      Where each of these steps occurs in one pass of Task3.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  LineClrFpgaFlt()      //may want to move to a Y0FltCk if not used as line only
{
    unsigned    Clr_Steps = 3;      // Number of steps to clear FPGA fault

    if ((FpgaFltRstB1 || ClrFpgaCtrB1)&&Stopped && !ClrFpgaHdwB1)
    {
        FpgaFltRstB1 = 0;       // After starting, clear.


        if (!ClrFpgaCtrB1)
         ClrFpgaCtrB1 = Clr_Steps;


        switch (ClrFpgaCtrB1)
        {
            case 3:
            FpgaRstT3B1 = 1;
            break;

            case 2:
            FpgaRstT3B1 = 0;
            break;

            case 1:
            ClrFltT2         = 1;   // Clear the fault buffer
            ExecLoadPk       = 0.0;
            ExecLoadT1Pk     = 0.0;
            ExecLoadT2Pk     = 0.0;
            ExecLoadT3Pk     = 0.0;
            MvSwGearShuntTrp = false;
            MvSwGearShuntSep = false;
            MvSwGearTrpReq   = false;
            break;

        }
        --ClrFpgaCtrB1;
    }

    // Add the MarkStat flt reset logic for MV switchgear; keep the above logic (not working properly )
    // for later reference of the Brg interface.
    // MvSwGearTrpReq may be set in task 1, task2 or task 3. Reset it to false only in task 3 here.
    if ( Stopped && DiagResetCmdT3 )
    {
        MvSwGearShuntTrp = false;
        MvSwGearShuntSep = false;
        MvSwGearTrpReq   = false;

        memset( (void*)&FaultIsActive, 0, sizeof(FaultIsActive) );
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Clears and resets FPGA hardware faults. Requires a four step process:
//           Dsp_Dig_Cmd.Pwr_Elec_Enb =  0  --> When in stopped state
//           Dsp_Dig_Cmd.Drv_Flt_Rst  =  1
//           Dsp_Dig_Cmd.Drv_Flt_Rst  =  0
//           Dsp_Dig_Cmd.Pwr_Elec_Enb =  1  --> When Starting
//      Where each of these steps occurs in one pass of Task3.
//      Intended to be used by the CheckFpgaFlt() routine to reset the
//      BIC fault bit and prevent the continuous check of dual bridge faults.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  LineClrFpgaHdw()      //may want to move to a Y0FltCk if not used as line only
{
    unsigned    Clr_Steps = 2;      // Number of steps to clear fpga fault

    //if ((FpgaHdwRstB1 || ClrFpgaHdwB1)&&L_SeqStBits.Stopped && !ClrFpgaCtrB1)
    if ((FpgaHdwRstB1 || ClrFpgaHdwB1)&&Stopped && !ClrFpgaCtrB1)
    {
        FpgaHdwRstB1 = 0;       // After starting, clear.

        if (!ClrFpgaHdwB1)
         ClrFpgaHdwB1 = Clr_Steps;

        switch (ClrFpgaHdwB1)
        {
            case 2:
            FpgaRstT3B1 = 1;
            break;

            case 1:
            FpgaRstT3B1 = 0;
            break;

        }
        --ClrFpgaHdwB1;
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Initialize FPGA faults. Requires a four step process:
//           Dsp_Dig_Cmd.Pwr_Elec_Enb =  0  --> When in stopped state
//           Dsp_Dig_Cmd.Drv_Flt_Rst  =  1
//           Dsp_Dig_Cmd.Drv_Flt_Rst  =  0
//           Dsp_Dig_Cmd.Pwr_Elec_Enb =  1  --> When Starting
//      Where each of these steps occurs in one pass of Task3.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  LineInitFpgaFlt()
{
    int     Ini_Steps = 1;      // Number of steps to clear FPGA fault
    int     Ini_Fpga_Ctr;       // State Counter for FPGA Flt Init

    for (Ini_Fpga_Ctr = Ini_Steps; Ini_Fpga_Ctr >= 0; --Ini_Fpga_Ctr)
    {
        switch (Ini_Fpga_Ctr)
        {

            case 1:
            FpgaRstT3B1 = 1;
            usleep(1000);          // 1msec
            break;

            case 0:
            FpgaRstT3B1 = 0;
            usleep(1000);          // 1msec
            break;
        }
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//       Simulate Adjustments. This is a temporary function to disable
//       assorted faults when appropriate hardware is not available.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  LineSimulateAdjustments()
{
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs fault checking for task3 common converter
//      faults.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void CommonFaultDetectionBIL(void)
{
    // Detetect controller 24V power supply down condition
    // This power supply is independent of Dc-link state
    if ( (Ps24vOkT3 == false) && (Ps24vUpOnce == true) )
    {
        PwrDnPs24vTmr = PwrDnPs24vTmr + DelTm3;
    }
    else
    {
        PwrDnPs24vTmr = 0.0;
    }

    if ( PwrDnPs24vTmr >= PARM(PwrDnPs24vTm) )
    {
        PwrDnPs24vTmr = PARM(PwrDnPs24vTm);
        if ( L_BridgeRebf.BrgHdwDataVld )
        {
            PwrDnFbkDsb   = true;
            PUSH_DIAG(PowerDown);
        }
    }

    // Detect case where controller 24V power supply has been down and it later comes back up
    // This power supply is independent of Dc-link state
    if ( (Ps24vOkT3 == true) && (PwrDnFbkDsb == true) )
    {
        PwrUpPs24vTmr = PwrUpPs24vTmr + DelTm3;
    }
    else
    {
        PwrUpPs24vTmr = 0.0;
    }

    if ( PwrUpPs24vTmr >= PARM(PwrUpPs24vTm) )
    {
       PwrUpPs24vTmr = PARM(PwrUpPs24vTm);
       PwrDnFbkDsb   = false;
       Ps24vUpOnce   = true;
    }



    // check the ok contact from the 24v power supply, only when line rebf link is running
    if ( !Ps24vOkT3 )
    {
        if ( L_BridgeRebf.BrgHdwDataVld )
        {
           PUSH_DIAG(Ps24NokTrp);
           FaultIsActive.Ps24Nok = true;
        }
    }

    return;
}
