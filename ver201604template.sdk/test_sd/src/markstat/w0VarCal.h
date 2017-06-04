/*=============================================================================
* Product:     GE Power Conversion Using MACC Based Control
* Host:        DSPX Running TMS320C31
* File:        W0VARCAL.H
* Language:    TI 'C'
* Copyright:   2003....GE Drives and Controls, Inc., Salem VA
*
* Abstract:  W0VARCAL.H contains constant definitions and function prototypes
*            used in the pwm source variable calculation functions.
*
* Functions in module:  None
*
*==============================================================================
* Last Mod: 10/6/03 9:34AM
*==============================================================================
* 030124 SAB  Create
* 031006 JMN  Add LVRT signals (to be sent via Can bus to GEWE plc)
* 031018 SAB  Add calculated grid current vars
* 040108 SAB  Remove ExecRotorConv, ExecLineConv
* 040308 BAG  add total running time accumulator
* 040317 BAG  add total powered up time accumulator, total power generated accumulator
* 051201 SAB  Add local ref vars
* 060216 IB   Update file with following changes: 1)UTC sync and master sync to both.
*             2)Add LastUtcUpdat. 3)Publish UTCTime.Sec, SyncUtcErr, and SyncMstrErr.
*             3)Create LastUtcErr to replace SyncUtcErr.
* 060310 SAB  Add IyGrid
* 061207 BAG  move time syncronization to P0TmSync.c30 for ESS
* 070109 IB   Add x-grid current.
* 070305 SAB  Add R_IMagMxSpil, S_IMagMxSpil
* 070310 SAB  Add S_IyMaxUc
* 070424 SAB  Move some vars to A0FlxCmd.h
* 070424 SAB  Move more vars to A0FlxCmd.h
* 070508 SAB  Add FctTestSelx
* 070709 SAB  Add S_PwrCalc, GridPwrCalc, WSlip
* 070712 SAB  Rename GridPwrCalc to GridPwrEst
* 070801 SAB  Remove vars no longer used.
* 071101 SAB  Add torque trimmer algorithm
* 080312 SAB  Add QregClmpSF
* 080816 SAB  Create new boolean, TransDiagInh
* 080815 SAB  Add grid monitor asymmetry ride-through variables
* 080930 SAB  Add VregNSeqHi
* 090313 SAB  Add LclSpdSimRfx

*==============================================================================
* Last Mod: January 09, 2007 03:57:06PM
*/

#ifndef __W0VARCAL_H__
#define __W0VARCAL_H__


#endif
