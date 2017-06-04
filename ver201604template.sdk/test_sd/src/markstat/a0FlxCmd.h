/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        A0FLXCMD.H
* Language:    TI 'C'
* Copyright:   1996..1998 GE Motors and Industrial Systems, Salem VA
*
* Abstract:
* -----------------------------------------------------------------------------
*
*==============================================================================
* 970321 PSB  Create file following new module guidelines.
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 060113 SAB  Add R_IyCmdFil
* 060217 SAB  S_IyPfTmp --> IyPfTmp
* 060220 SAB  Add prototype for PowerFactorControlT2 and add VGridVCFil
* 070303 SAB  Add R_RegLim2
* 070305 SAB  Add S_VrefSclFil
* 070319 SAB  Remove un-used 'power factor control' variables and parameters
* 070326 SAB  Remove FbkScale
* 070424 SAB  Add new current limit vars and new current limit function,
* 070424 SAB  Move in current limit vars from W0VarCal.h
* 070508 SAB  Make adjustments to support current limit changes, add call to
* 070523 SAB  Clean-up
* 070823 SAB  Add VarRegRespTm VltRegResptm
* 071025 SAB  Add filter for L_IxFbkSqT3 for calulation of L_IReserveSS
* 080115 SAB  Add QRegIntGnDyn
* 080122 SAB  Add new dynamic iy limit variables
* 080312 SAB  Add volt reg dynamic clamp variable inputs
* 080529 SAB  Add new field weakening variables
* 080917 SAB  Add excess torque/asymmetric fault vars
* 081002 SAB  Add vars for field weakening by rotor voltage excess
* 090127 SAB  R_VMagPuT3 --> R_VMagPuT2
* 090428 SAB  Add new WindBOOST vars.
* 090810 SAB  Add VregLVIyMnDy
* 090810 SAB  Remove VregFilIn, others
* 090831 SAB  Add IyPFStepAdr and R_IyStep
* 090915 SAB  Import vars from Wind-DFIG WindBOOST
* 090929 SAB  Remove rotor and stator speed dependent curve slope and add
*             reciprocal for new constant
* 110825 SAB  Remove volt filter coefficient prototype.  Add SDL2 logic and
*             and new form of volt-var regulator
* 111110 SAB  Remove IyMaxLim, IyMinLim
* ______________________________________________________________________________
* Last Mod: January 21, 192004 2:32:26PM
*/

# ifndef    __A0FLXCMD_H__
# define    __A0FLXCMD_H__


# endif /* __A0FLXCMD_H__ */
