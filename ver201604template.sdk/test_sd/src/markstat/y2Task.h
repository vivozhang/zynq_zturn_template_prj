/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        MACC Running TMS320VC33
* File:        Y2TASK.H
* Language:    TI 'C'
* Copyright:   2003.... GE Drives and Controls, Inc., Salem VA
*
* Abstract:  Contains constant definitions and function prototypes
*            used to abstract the PWM3 hardware.
*
* Functions in module:  None
*
*==============================================================================
* 980526 SAB  Created.  Add bridge power loss vars.
* 980603 SAB  Remove L_Brg_Loss_Pwr
* 021023 SAB  Remove R_Thrm_R_Rcp
* 021108 SAB  Add prefix "L_" in front of vars
* 021120 SAB  Change naming convention: Remove all under-scores and add
*             "L_" on line converter names.
* 021223 SAB  Move in L_T0,L_T1 frame stuff
* 030304 SAB  Remove extern for DspVcoRgr
* 030306 SAB  BigDigxxx ---> MACCDig
* 030516 SAB  Add Brg2DsbOvr
* 030611 SAB  Add L_IocInstEnb
* 030623 SAB  Remove Brg2DsbOvr
* 030919 SAB  Remove L_RChgRcp
* 031222 SAB  Add prototypes
* 040302 SAB  Add L_IocRtryRto
* 110819 SAB  Add DB Stuff
*==============================================================================
* Last Mod: June 10, 1998 10:30:15AM
*==============================================================================
*/


#ifndef __Y2TASK_H__
#define __Y2TASK_H__





extern void ProcessBrgT3(void);
extern void LineProcessBrgBgnd(void);

//#include "y2task.c30"

#endif
