/*=============================================================================
 * Product:     GE Power Conversion Using MACC Control
 * Host:        MACC Running TMS320VC33
 * File:        Y1FLTCK.C30
 * Language:    TI 'C'
 * Copyright:   2003... GE Drives and Controls, Inc. Systems, Salem VA
 *
 * Abstract:    Drive Specific Fault declaration procedures
 *
 * Functions in module:  ClrFpgaFlt
 *                       InvGndFltDetectT1
 *                       InvGndFltDetectT2
 *                       InvGndFltDetectT3
 *
 *=============================================================================
 * Last Mod: January 12, 2009 4:52:47PM
 *=============================================================================
 * 970318 RCB  Created from X0FAULTS.C30 Move CheckAsicFlt(), ClrASICFlt(),
 * 011213 SAB  Clear Exec_Load_Pk when reset is given
 * 030204 SAB  Branch from 'rotor' and include in 'combo'
 * 030306 SAB  Bic_Dig_Stat --> R_MACCDigStat
 * 030307 SAB  Compile out Brg_1_Init, Brg_2_Init stuff for MACC
 * 030326 SAB  Rename Asic --> Fpga...
 * 030410 SAB  Resolve reseting of bridge1, bridge 2 faults
 * 030522 SAB  Change value of 1msec wait time to agree with new H1 clock
 * 030620 SAB  Add BrgEnbSim logic
 * 031120 SAB  Clear new Fast_Shtdwn flag for rotor converter
 * 031204 SAB  OR in S_VacFltFlg to Fast_Shtdwn flag
 * 040108 SAB  Move slow functions to Y1FLTCKS.C30
 * 040224 SAB  Remove MACC MODE condtionals
 * 040305 DS   remove setting R_IFbkInvld (no dual bridge)
 * 060120 SAB  Allow crowbar if not R_BrgPwrEnb and R_ZVRtrOff
 * 070301 IB   Add var Q1OpnReq right after declaring a crowbar fault.
 * 070321 SAB  Substitute PumpBackAct for R_PumpBack
 * 070508 SAB  Disable desats when PwrDnFbkDsb
 * 070614 SAB  Rename S_VacFlgFlg to GridFltFlg
 * 070709 SAB  Use IRam copies of EEs
 * 080827 SAB  Move fast shutdown of rotor bridge to line converter task1
 * 080902 SAB  Create R_IocMask and block IOC on low grid mon voltage.
 * 090112 SAB  Change rotor IOC masking to be the original intent.  Don't allos
 *             masking all the time volt mag is below 50%...only when that is
 *             true along with dc link voltage aproaching crobar level.
 *=============================================================================
 */

/*****************************************************************************
*
*       Inline expansion control
*
*****************************************************************************/

#include  "x0FrameWork.h"
#include  "Product.h"
#include  "p1Sseq.h"
#include  "y1FltCk.h"


// Constants
//----------



// Parameters
//----------
//TODO, whether to add L_ for these parameters.
CREATE_PARM(DBGtOnThrs,   float);                // Dynamic brake gate on threshold
CREATE_PARM(DBGtOffThr,   float);                // Dynamic brake gate off threshold


/*****************************************************************************
*
*       Global declarations
*
*****************************************************************************/


CREATE_PUBVAR(FpgaHdwRstB2,             unsigned);                 // Bic/hardware Reset Command Bool */
CREATE_PUBVAR(R_IFbkInvld,              unsigned);                 // Current feedbacks invalid status */
CREATE_PUBVAR(FpgaRstT3B2,              unsigned);
CREATE_PUBVAR(ClrFpgaHdwB2,             unsigned);                 // State Counter for Bic/hardware Reset */
CREATE_PUBVAR(BpibRstT3B2,              unsigned);




/*****************************************************************************
*
*       Local declarations
*
*****************************************************************************/
/*
 The Bic EPLD loads commands based on the load pulse it sees on the backplane.
 That load pulse is the same as the Task 1 rate which may be as long as 1 ms.
 */



/*****************************************************************************
*
*       external references
*
*****************************************************************************/

extern unsigned   R_SeqStBits_PackedBits;

extern float ExecLoadPk;
extern float ExecLoadT1Pk;
extern float ExecLoadT2Pk;
extern float ExecLoadT3Pk;
extern float R_VdcFbk       ;
extern unsigned BicFltStat     ;
extern unsigned BrgEnbSim;
extern unsigned ClrFltT2;
extern unsigned ClrFpgaCtr;
extern unsigned ClrFpgaCtr;
extern unsigned FpgaFltRstB2;
extern unsigned FpgaHdwRstB2;
extern unsigned PwrDnFbkDsb    ;
extern unsigned R_BrgFltHdw;
extern unsigned DiagResetCmdT3;
extern unsigned S_K1OpnNowReq;
extern unsigned BrgCardsAct;



/*****************************************************************************
*
*       Task3 Clear FPGA Faults
*
*       Clears faults in and resets the FPGA.  Requires a four step process:
*           Dsp_Dig_Cmd.Pwr_Elec_Enb =  0; When in stopped state.
*           Dsp_Dig_Cmd.Drv_Flt_Rst  =  1;
*           Dsp_Dig_Cmd.Drv_Flt_Rst  =  0;
*           Dsp_Dig_Cmd.Pwr_Elec_Enb =  1; When Starting...
*       Where each of these steps occurs in one pass of Task3.
*
*
*
******************************************************************************/
void  RotorClrFpgaFlt(void)                 // need only 1 of these functions in combo!
{
unsigned    Clr_Steps = 3;      /* Number of steps to clear FPGA fault */

    if ((FpgaFltRstB2 || ClrFpgaCtr)&&R_SeqStBits.R_Stopped && !ClrFpgaHdwB2)
    {
        FpgaFltRstB2 = 0;       /* After starting, clear. */

        if (!ClrFpgaCtr)
         ClrFpgaCtr = Clr_Steps;

        switch (ClrFpgaCtr)
        {

            case 3:
            FpgaRstT3B2 = 1;
            break;

            case 2:
            FpgaRstT3B2 = 0;
            break;

            case 1:
            ClrFltT2 = 1;   /* Clear the fault buffer. */
            ExecLoadPk   = 0.0;
            ExecLoadT1Pk = 0.0;
            ExecLoadT2Pk = 0.0;
            ExecLoadT3Pk = 0.0;
            break;

        }
        --ClrFpgaCtr;
    }
    if(DiagResetCmdT3 && R_SeqStBits.R_Stopped)
    {
        S_K1OpnNowReq = false;
    }
}

/*****************************************************************************
*
*       Task3 Clear FPGA Hardware
*
*       Clears faults in and resets the FPGA.  Requires a four step process:
*           Dsp_Dig_Cmd.Pwr_Elec_Enb =  0; When in stopped state.
*           Dsp_Dig_Cmd.Drv_Flt_Rst  =  1;
*           Dsp_Dig_Cmd.Drv_Flt_Rst  =  0;
*           Dsp_Dig_Cmd.Pwr_Elec_Enb =  1; When Starting...
*       Where each of these steps occurs in one pass of Task3.
*
*       Intended to be used by the CheckFPGAFlt() routine to reset the
*       BIC fault bit and prevent the continuous check of dual bridge faults.
*
*
******************************************************************************/
void  RotorClrFPGAHdw(void)
{
unsigned    Clr_Steps = 2;      /* Number of steps to clear FPGA fault */

    if ((FpgaHdwRstB2 || ClrFpgaHdwB2)&&R_SeqStBits.R_Stopped && !ClrFpgaCtr)
    {
        FpgaHdwRstB2 = 0;       /* After starting, clear. */

        if (!ClrFpgaHdwB2)
         ClrFpgaHdwB2 = Clr_Steps;

        switch (ClrFpgaHdwB2)
        {
            case 2:
            FpgaRstT3B2 = 1;
            break;

            case 1:
            FpgaRstT3B2 = 0;
            break;

        }
        --ClrFpgaHdwB2;
    }
}


/*****************************************************************************
*
*       Task3 Init FPGA Faults
*
*       Clears faults in and resets the FPGA.  Requires a four step process:
*           Dsp_Dig_Cmd.Pwr_Elec_Enb =  0; When in stopped state.
*           Dsp_Dig_Cmd.Drv_Flt_Rst  =  1;
*           Dsp_Dig_Cmd.Drv_Flt_Rst  =  0;
*           Dsp_Dig_Cmd.Pwr_Elec_Enb =  1; When Starting...
*       Where each of these steps occurs in one pass of Task3.
*
*
*
******************************************************************************/
void  RotorInitFpgaFlt(void)
{
int     Ini_Steps = 1;      /* Number of steps to clear FPGA fault */
int     Ini_Fpga_Ctr;       /* State Counter for Fpga Flt Init */

    for (Ini_Fpga_Ctr = Ini_Steps; Ini_Fpga_Ctr >= 0; --Ini_Fpga_Ctr)
    {
        switch (Ini_Fpga_Ctr)
        {

            case 1:
            FpgaRstT3B2 = 1;
            usleep(1000);          // 1msec
            break;

            case 0:
            FpgaRstT3B2 = 0;
            usleep(1000);          // 1msec
            break;
        }
    }

}


/*****************************************************************************
*
*       Simulate Adjustments
*
*       This is a temporary function to disable assorted faults when
*       appropriate hardware is not available.
*
*
******************************************************************************/
void  RotorSimulateAdjustments(void)
{

}
