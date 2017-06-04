/*=============================================================================
* Product:     GE Power Conversion Control Using MACC
* Host:        DSPX Running TMS320C31
* File:        Y1FDBK.H
* Language:    TI 'C'
* Copyright:   2003.. GE Drives and Controls, Inc., Salem VA
*
* Abstract:  Y1FDBK.H contains constant definitions and function prototypes
*            used in the calculation of voltage and current feedbacks.
*
* Functions in module:  None
*
*==============================================================================
* 961107 DGR  Initial create.  Import constants from A0FDBK.C30.
* 030204 SAB  Effective branch from 'Rotor' and change name from Y2Fdkb.c30
*             to P3Fdkb.c30
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 030205 SAB  Add 'Rotor' prefix to functions
* 030304 SAB  Chanage 'raw' vars back to single word.  Bring in Vco register
*             from Y1Task.h
* 030305 SAB  Remove Brg2VcoRgr refs.y
* 031222 SAB  Add proto-types as needed.
* 061206 SAB  Add frequency variables
* 070303 SAB  Add rotor TOC high select
* 070310 SAB  Remove toc cool stuff
* 070626 SAB  Add new RAF feedback variables
* 070627 SAB  Add RAF vars for dc current reg
* 080218 SAB  Add IabcMskTmr, IabcMskAct
* ==============================================================================
* Last Mod: February 06, 192003 4:23:34PM
*==============================================================================
*/

#ifndef __Y1FDBK_H__
#define __Y1FDBK_H__

/* Feedback offset constant */
#define  FIL_TAU_N  5



#endif
