/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        DSPX Running TMS320C31
* File:        Y2FDBK.H
* Language:    TI 'C'
* Copyright:   2003... GE Drives and Controls, Inc., Salem VA
*
* Abstract:  Y2FDBK.H contains constant definitions and function prototypes
*            used in the calculation of voltage and current feedbacks.
*
* Functions in module:  None
*
*==============================================================================
* Last Mod: February 4, 1999 4:41:36PM
*==============================================================================
* 970117 0001 SAB  Initial create from A0FDBK.H
* 970318 SAB  Rename file from S0FDBK.H TO S2FDBK.H and modify file header
*             comment changes.
* 970320 SAB  Insert RACK_MODE conditionals.  Declare fdbk externals. Add
*             include for inline code.
* 970325 SAB  Remove def of Vdc_Fbk, Vdc_Inst
* 970325 SAB  Rename from S2FDBK.H to S0FDBK.H
* 970403 SAB  Add Vdc_Fbk, Vdc_Fbk_Rcp
* 970404 SAB  Make Rack_Mode defs like those in pwm2 motor control.
* 970409 SAB  Remove BAIA stuff.  It now exists in  x0baia.h
* 970411 SAB  Add dynamic voltage offset variables
* 970414 SAB  Add dynamic current offset variables
* 970415 SAB  Remove Ioff_Null_En
* 970416 SAB  Add Ia,Ib,Ic_Offs_Int
* 970722 SAB  Remove RACK_MODE conditionals
* 971016 SAB  Add Vco registers for DB
* 971104 SAB  Add DB_I
* 980407 SAB  Change VCE_Raw to type int
* 980421 SAB  Add DB_V_Old.  Remove DB_I
* 980526 SAB  Rename to Y2FDBK.H. Remove S3SOURCE code.
* 980604 SAB  Remove bridge interface declarations redundant to y2task
* 980608 SAB  Add Vdc_Comp, Vdc_Comp_Rcp.  Add Ia,Ib,Ic_Sqr
* 990128 SAB  Incorporate ACCBN-A code
* 990128 SAB  Incorporate ACCBN-A code
* 990204 SAB  Fix ifdef for ACCBN_A
* 020125 SAB  Add Exec_Load, Exec_Load_Pk
* 021120 SAB  Change naming convention: Remove all under-scores and add
*             "L_" on line converter names.
* 030131 SAB  Remove product conditional compiles
* 030304 SAB  Move in extern for BrgVcoRgr
* 030305 SAB  Remove Brg1VcoRgr refs.y
* 030308 SAB  BAIA --> AEAA
* 030728 SAB  Add ExecLoadT1, ExecLoadT2, PkExecLoadT2, ExecLoadT1
*             PkExecLoadT3, ExecLoadT3Pk
* 031222 SAB  Add proto-types as needed.
* 061206 SAB  Aeaa structures --> Aeax. Add frequency variables
* 070611 SAB  Add L_VcaT2
* 070626 SAB  Add line and rotor RAF variables for task2 currents
* 070801 SAB  Add L_Ia, Ib, IcFbk
* 080103 SAB  Add additional vars to accomodate current feedback low pass filters.
* 080305 SAB  Add RAF for PllWT2
* 091123 SAB  Add new variables for unfiltered current.
* 110818 SAB  Add dynamic brake feedback, LineRmsPhaseCurrentCalcT2, T3
*==============================================================================
*/

#ifndef __Y2FDBK_H__
#define __Y2FDBK_H__


#endif
