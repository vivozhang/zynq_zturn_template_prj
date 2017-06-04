/*=============================================================================
 * Product:     AccuWave Power Conversion Using MACC
 * Host:        TMS320C31 DSP
 * File:        Y2Rule.h
 * Language:    TI 'C'
 * Copyright:   1998 GE Industrial Control Systems, Salem VA
 *==============================================================================
 * 980401 CWM  Create from A0Rule.h
 * 980401 CWM  Add externs to support vars moved from Y0GRID.C30.
 * 980403 CWM  Remove Y0GRID externs.
 * 980514 CWM  Rename to Y1RULE.
 * 021120 SAB  Change naming convention: Remove all under-scores and add
 *             "L_" on line converter names.
 * 021210 SAB  Change name to Y2Rule.h
 * 030203 SAB  Add 'Line' to all functions
 * 070604 SAB  Create Iram copies of  y2 parameters
 * 110823 SAB  Add extern for LineBrgInfcRule
 *______________________________________________________________________________
 * Last Mod: May 14, 1998 4:34:54PM
 */
#ifndef Y2RULE_H
#define Y2RULE_H


/******************************************************************************/
/*  Functions                                                                 */
extern int  RunLineBILRules(int Rule_Set);
extern int  LineBrgInfcRule(void);

/******************************************************************************/
#endif
