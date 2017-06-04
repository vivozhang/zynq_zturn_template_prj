/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        DSPX Running TMS320C31
* File:        P0LINE.H
* Language:    TI 'C'
* Copyright:   2003... GE Drives and Controls, Inc., Salem VA
*
* Abstract:  P0LINE.H contains constant definitions and function prototypes
*            used in the phase lock loop and angle generation functions
*
* Functions in module:  None
*
*==============================================================================
* Last Mod: June 2, 1998 10:00:39AM
*==============================================================================
* 070116 SAB  Create
* 070120 SAB  Add
* 070131 SAB  Add LnVMagT2, LnIMagT2, LnVMgLnPuT2
* 070207 SAB  Rename  LnVMagT2, LnIMagT2, LnVMgLnPuT2 to ....T3
* 070212 SAB  Rename all "Line" and "Ln" stuff to "Gm"
* 070217 SAB  Remove GmNegSeqPc2 and add GmNSeqVMag, GmPSeqVMag
* 070310 SAB  Add GmTocHiSelSq. Rename and remove others
* 070324 SAB  Add volt mag, real and reactive power for task 2
* 070326 SAB  GmVMgLnPuT2 --> GmVMagPuT2
* 070328 SAB  Add GmNegPosVPct
* 070522 SAB  Add Grid monitor protection variables
* 070524 SAB  Add GmVlnNomRcp, GmVllNomRcp, GmFreqNomRcp, GmFreqNom
* 070608 SAB  Add extern for GridMonitorConfigCheck
* 070719 SAB  Add GmIyInst
* 070801 SAB  Add timers uv/ov alarm off
* 070829 SAB  Add GmUVAlmFlg...summary of all
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 080211 SAB  Add new open phase detection variables
* 080303 SAB  Add new GmPllVyErrT3
* 080419 SAB  Add new grid monitor converter protection asym vars.
* 080624 SAB  Add GridMonConverterFilterPreset function
* 110825 SAB  Rename GmPllErrPuT3 to GmPllErrPuT2
* 111110 SAB  Add define for GM_RAF_ARRAY_SIZE
*==============================================================================
*/

#ifndef __P0LINE_H__
#define __P0LINE_H__

#define HOT_INIT   0
#define HOT_DYN    1
#define COOL_DYN   2
#define HOT_FIXFRQ 3

#define OVCNVFLT_NONE   0
#define OVCNVFLT_LVL1   1
#define OVCNVFLT_LVL2   2
#define OVCNVFLT_LVL3   3
#define OVCNVFLT_LVL4   4
#define OVCNVFLT_FASTOV 5

#endif
