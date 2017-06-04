/*=============================================================================
* Product:     PWM Source Using DSPX Based Control
* Host:        DSPX Running TMS320C31
* File:        Y2GNDFLT.H
* Language:    TI 'C'
* Copyright:   1996..1998 GE Industrial Control Systems, Salem VA
*
* Abstract:  Y1GNDFLT.H contains constant definitions and function prototypes
*            used in checking faults for shared groups of ac drives/sources.
*
* Functions in module:  None
*
*==============================================================================
* Last Mod: January 29, 1999 10:55:23AM
*==============================================================================
* 970401 SAB  Create
* 970421 DGR  Move in inverter ground fault variables from Y0FLTCK2.H.
* 970521 DGR  Add Ix_Iy_Gnd_Gn, Ix_Gnd_Fil, Iy_Gnd_Fil.
* 970610 DGR  Declare (Ia,Ib,Ic)_Sqr_T3 as extern variables.
*             Remove declarations of (Ia,Ib,Ic)_Sqr_Fil.
* 970612 RCB  Add Toc_Cool_Fil
* 970612 DGR  Remove W_Toc_T1.
* 980422 CWM  Remove extern of Ia,b,c_Sqr_T3 (moved to a0datapas.def).
* 980423 CWM  Remove extern of Ix_Gnd_T3, Iy_Gnd_T3 (moved to y1datapa.def).
* 980507 PSB  Toc detection and Iabc ground fault moved to Y0FDBK
* 980514 CWM  Rename to Y1FLTCKA.
* 980520 DGR  Rename to Y1GNDFLT.
* 980601 SAB  Conditionally extern Iabc ground stuff
* 980923 DGF  Add extern Ixy_Gnd
* 990129 SAB  Share with ACCBN-A and include it with cond. compile
* 021120 SAB  Change naming convention: Remove all under-scores and add
*             "L_" on line converter names.
* 021210 SAB  Change name to Y2GndFlt.h
* 030131 SAB  Remove conditional compiles
* 070531 SAB  Add prototype for
*==============================================================================
*/

#ifndef __Y2GNDFLT_H__
#define __Y2GNDFLT_H__




#endif
