/*=============================================================================
* Product:     GE Power Conversion Using MACC Control
* Host:        TMS320VC33 DSP
* File:        Y1FLTCK.H
* Language:    TI 'C'
* Copyright:   2003... GE Industrial Control Systems, Salem VA
*
* Abstract:    Header File for Y1FLTCK.C30
*
*==============================================================================
* 970318 RCB  Created from X0FAULTS.H Move CheckAsicFlt(), ClrFpgaFlt(),
* 030204 SAB  Branch from 'rotor' and include in 'combo'
* 030205 SAB  Add 'Rotor' prefix to functions
* 030326 SAB  Rename Asic.... -> Fpga....
* 030410 SAB  Resolve resetting of bridge1, bridge 2 faults
* 040108 SAB  Add extern for FpgaHdwRstB2
* 080902 SAB  Add R_IocMask
*______________________________________________________________________________
* Last Mod: May 14, 1998 4:26:31PM
*/


#ifndef  Y1FLTCK_H
#define  Y1FLTCK_H


/*******************************************************************************
* Local constant definitions
*******************************************************************************/

#define INIT_FPGA_DLY    60000                  /* Delay 1ms for T1 load pulse */



#endif
