/*=============================================================================
* Product:     GE Power Conversion Control Using MACC
* Host:        DSPX Running TMS320C31
* File:        P3FDBK.H
* Language:    TI 'C'
* Copyright:   2003.... GE Drives and Controls, Inc., Salem VA
*
* Abstract:  P3FDBK.H contains constant definitions and function prototypes
*            used in the calculation of voltage and current feedbacks.
*
* Functions in module:  None
*
*==============================================================================
* Last Mod: April 11, 192003 3:24:51PM
*==============================================================================
* 970117 SAB  Initial create from A0FDBK.H
* 020827 DS   Modify for doubly-fed
* 030204 SAB  Effective branch from 'Rotor' and change name from Y2Fdkb.c30
*             to P3Fdkb.c30
* 030304 SAB  Chanage 'raw' vars back to single word
* 030306 SAB  Fix MACC_MODE conditional
* 030317 SAB  Move analog input capture vars to X0AEAA.H
* 030415 SAB  Add S_VabSqr, S_VbcSqr
* 031212 SAB  Add prototypes
* 061202 SAB  Add prototype for LineFbksInit
* 061211 SAB  Add prototype for LineProcessFbks
* 070109 IB   Add line demodulated feedbacks
* 070113 SAB  Add LineMonOK, LineVx,Vy,Ix,Iy Fbk.....will be moved to P0Line later.
* 070120 SAB  Move some "line" code to P0Line.c30
* 070208 SAB  Add LineIaT2, LineIbT2, LineIcT2, LineVabT2, LineVbcT2
* 070210 SAB  Rename all "Line" and "Ln" stuff to "Gm"
* 070220 SAB  Add fast grid monitor over-voltage vars
* 070303 SAB  Add Stator TOC high select
* 070310 SAB Remove Toc cool stuff. Add S_TocHiSelSq
* 070326 SAB  Create GmVab, GmVbc, GmVca
* 070417 SAB  Add new vars for calculation derivatives for use in distortion
*             filter current.
* 070508 SAB  Rename of distortion filter dv/dt signals
* 070604 SAB  Add GmOVPuVSec
* 070611 SAB  Add GmVcaT2
* 070629 SAB  Add AuxIySimx
* 070801 SAB  New vars for delayed stator and gm voltages
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 080109 SAB  Add variables for meld delay of line ac voltages
*______________________________________________________________________________
*==============================================================================
* Last Mod: January 09, 2007 11:40:42AM
*/

// need dedicated S_ vars for these
#define     S_IoffNullEn R_IoffNullEn
#define     S_VoffNullEn R_VoffNullEn
#define     S_IoffSlowSw R_IoffSlowSw
#define     S_VoffFastSw R_VoffFastSw

#ifndef __P3FDBK_H__
#define __P3FDBK_H__

#endif
