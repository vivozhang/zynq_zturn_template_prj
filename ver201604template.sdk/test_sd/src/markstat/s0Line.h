/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        DSPX Running TMS320C31
* File:        S0LINE.H
* Language:    TI 'C'
* Copyright:   2003 GE Motors and Industrial Systems, Salem VA
*
* Abstract:  S1ANGL.H contains constant definitions and function prototypes
*            used in the phase lock loop and angle generation functions
*
* Functions in module:  None
*
*==============================================================================
* Last Mod: January 26, 2007 03:31:08PM
*==============================================================================
* 970321 SAB  Create
* 970415 SAB  Remove Ac_Line_W calculation
* 970416 SAB  Add monitor variables  for line voltage and frequency
* 970513 SAB  Rename Ph_Imb_Err --> Vxy_Imb_Fil, add Ph_Imb
* 970923 SAB  Add Rev_Seq, Pll_Del_Ang
* 971010 SAB  Add L_Ac_Line_Loss
* 971020 SAB  Add Line_Flt_Enb
* 980129 SAB  Add phase loss variables for S3SOURCE and Pll_Prp_Old
* 980130 SAB  Add Ph_Loss
* 980218 SAB  Add Rev_Seq_Lock
* 980305 SAB  Branch from S0ANGL.H and rename to S1ANGL.H. Add Pll_Int_Fil1,
*             Pll_Int_Fil2, Ph_Loss_Tmr
* 980409 SAB  Add test oscillator var Innr_Reg_Z1
* 980519 SAB  Rename to S0LINE.H (old S0ANGL)
* 021022 SAB  Remove Innr_Reg_Z1
* 021031 SAB  Add Pll_Ref, Pll_Vy_Comp
* 021105 SAB  Add new phase imbalance stuff
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 031014 SAB  Add L_PllWRcp
* 031212 SAB  Add prototypes
* 050922 SAB  Add Dscm vars for pos/neg sequence discriminator
* 051006 SAB  Add extern for LinePosNegDiscriminatorT3
* 051010 SAB  Add vars from 2.x
* 060207 SAB  Add L_VMonUV1
* 060313 SAB  Add L_LnVShOfTr2
* 060315 SAB  Add L_NSeqHi
* 070117 SAB  Rename two functions
* 070126 IB   Add support to line bridge voltage loss detection.
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
* 080517 SAB  Remove no longer used phase imbalance vars
*
*==============================================================================
*/

#ifndef __S0LINE_H__
#define __S0LINE_H__



#endif
