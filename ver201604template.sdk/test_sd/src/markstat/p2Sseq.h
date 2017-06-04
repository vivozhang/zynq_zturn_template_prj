/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        TMS320CVC33 DSP
* File:        P2SSEQ.H
* Language:    TI 'C'
* Copyright:   1996 GE Motors and Industrial Systems, Salem VA
*
* Abstract: Header file for A0SSEQ.C30, the motor control sequencer module.
*
* -----------------------------------------------------------------------------
*
* Functions in module:  None
*
*==============================================================================
* 970120 0001 SAB  Create from A0SSEQ.H
* 970125 0002 SAB Modify states and tasks to meet source needs
* 970325 0003 SAB Remove state definitions
* 970326 0004 SAB Add L_Auto_Start_V and Stop_Req_V
* 970331 SAB  Remove Stop_Req_V
* 970407 SAB  Add hard reset definition
* 970409 SAB  Fix hard reset non-rack mode definition.
* 970423 SAB  Add Hdw_Run_Req extern
* 970424 SAB  Add No_Trip_Flt
* 970425 SAB  Define Local_Mode
*        SAB  Define ready, running, L_Charging.
* 970426 SAB  Define prototype for KeypadProductStatusWrite
* 970428 SAB  Remove ready, running, L_Charging.
* 970505 SAB  Add Run_Req as an external
* 970513 SAB  Rename Sync_Ini_Stat --> Sync_Ini_Sta
* 970619 CWM  Add Parked (Bit 1) to SeqBits1 struct.
* 970722 SAB  Remove RACK_MODE conditionals
* 971105 SAB  Add prototypes for LocalRunFunction and ProcessIPN
* 980311 CWM  Remove No_Trip_Flt, now declared in X0FAULTS.C30.
* 980519 SAB  Rename file.
* 980604 SAB  Remove prototype for ProcessIPN
* 980915 SAB  Add  Ld_Run_Cmd for ACMVRS6_A only
* 981030 DS   for ACMVRS6_A only: Add  Pulse Test & Cell Test bits to SeqBits1,
*             SeqReqBits, SeqStatBits, SeqActBits. Add Pls_Tst_Cmd extern
* 981201 DS   for ACMVRS6_A only: Add Cel_Tst_Cmd extern
* 990107 SAB  Add L_Test_Req, Stat, Act, L_Test_Cmd
* 000328 SAB  Make Incuctance test stuff visible to ACCBR-A
* 021022 SAB  Remove Local_Mode definition...now located in X0MISC
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 021206 SAB  Add function ToggleLinkCharge
* 021210 SAB  Rename to P2SSeq.h
* 030124 SAB  Remove prototype for LocalRunFunction and Hard Reset definition
* 030131 SAB  Remove product conditional compiles
* 030207 SAB  Move in Line Seq. state defs.
* 030212 SAB  Declare L_InitDone
* 030317 DS   add celltest/pulsetest definitions
* 030401 SAB  Add "Line" to KeypadProductSatuswrite funciton. Remove un-used vars
* 030404 SAB  Add LINE_RAMPUP, move in L_Running
* 030922 SAB  Remove  ToggleLinkCharge function
* 030930 SAB  Add CoolPurgeCmd, definition for COOL_PURGE_TEST, Purg
* 051129 SAB  Add L_StopStTmr
* 070109 IB   Update structures with KDF and Q1 bits.
* 070130 IB   Add Q1EpldTrip
* 070214 IB   Remove prototype SeqDemodulateLine(). Add LineLnVdifSq , LineLnSeqFil,
*             SeqDemVxfbk, and SeqDemVyfbk.
* 070226 IB   Remove cooling purge state definition and associated variable.
* 070301 IB   Add var Q1OpnReq.
* 070321 IB   Add L_CTDismTmr.
* 070424 IB   Add declarations to support dist-fiter monitoring during line sequencing.
* 070508 IB   Update DistFilPhSta data structure.
* 071029 SAB  Remove inductor test state
* 080424 IB   Remove DistFilPhSta structure and add some variables to support
*             new dist-filter verification with multiple KDF operations.
* 080520 IB   Delete DFtestDrctT3. Add DfTestIndxZ1 and DFQPwrTmr.
* 110819 SAB  Add DB stuff
*______________________________________________________________________________
* Last Mod: May 20, 2008 02:18:11PM
*/

/* Define Statements: */
#ifndef P2SSEQ_H
#define P2SSEQ_H

//Line Converter Sequencer States
#define LINE_STOPPED        0
#define LINE_CHARGECHECK    1
#define LINE_PLLSYNC        2
#define LINE_RAMPUP         3
#define LINE_RUNNING        4
#define LINE_DIAGNOSTIC     5
#define LINE_PULSE_TEST     6
#define LINE_CELL_TEST      7
#define LINE_DB_PULSE_TEST  8
#define LINE_DB_CELL_TEST   9
#define LINE_ZVRT_STANDBY       10
#define LINE_CLOSE_KDF          11
#define LINE_CLOSE_Q1           12
#define LINE_DB_RES_MEAS        13
#define LINE_IDLE_RTR_LEG_TEST  14
#define LINE_INIT               -1

#endif
