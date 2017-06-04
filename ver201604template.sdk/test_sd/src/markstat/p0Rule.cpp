///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Product Level Rules
//
// DESCRIPTION:
//      This file provides the top level product rules.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Rule.h"
#include "x0CaptureBuf.h"
#include "x0GpDiagFilter.h"
// product
#include "Product.h"


// Constants
//----------


// Externals
//----------
extern cCaptureBuf      CaptureBuf;
extern cGpDiagFilter   GpDiagFilter;
extern cGpDiagFilter   R_GpDiagFilter;

extern int  ProductRules(void);

// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(CapBufWizardTyp     ,unsigned);     // Pre-configured capture buffer setups  (definition in z0ParmVar.xls)


// Variables
//----------
CREATE_PUBVAR(R_RtrPosMeth, unsigned);

// Function Prototypes (to resolve forward references)
//----------------------------------------------------
int  DefaultCaptureBuff(void);
int  DefaultDiagFilter(void);
int  DefaultRelayOuts(void);




///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Select the product ruleset to run.
//
// EXECUTION LEVEL
//      Initialization or ADL Thread
//
// RETURN VALUE
//      Status to indicate if calculations were successful (SUCCESS/FAIL).
//
///////////////////////////////////////////////////////////////////////////////
int RunProductRules(int Rule_Set)
{
    int     Status = SUCCESS;

    R_RtrPosMeth = PARM(R_RtrPosMeth);

    //-------------------------------------------------------------------------
    // main rules
    switch(Rule_Set)
    {
        // calculate motor control final values
        case RULE_SET_MAIN:
            if ( ProductRules() == FAIL ) Status = FAIL;
            break;

        // set motor control parameters based on final values
        case RULE_SET_DEFAULT_ALL:
            if ( DefaultCaptureBuff() == FAIL ) Status = FAIL;
            if ( DefaultDiagFilter()  == FAIL ) Status = FAIL;
            if ( DefaultRelayOuts()   == FAIL ) Status = FAIL;
            break;

        // Set Default Capture Buffer Settings
        case RULE_SET_DEFAULT_CAPT_BUFF:
            if ( DefaultCaptureBuff() == FAIL ) Status = FAIL;
            break;

        // Set Default Diagnostic Filter Settings
        case RULE_SET_DEFAULT_DIAG_FILTER:
            if ( DefaultDiagFilter()  == FAIL ) Status = FAIL;
            break;

        // Set Default Analog Input Filter Settings
        case RULE_SET_DEFAULT_AIN_FILTER:
            break;

        // Set Default Relay Out Parameters
        case RULE_SET_DEFAULT_RELAY_OUT:
            if ( DefaultRelayOuts()   == FAIL ) Status = FAIL;
            break;

       // Default case
       default:
            break;
    }

    return (Status);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Set pre-configured capture buffer setting
//
// EXECUTION LEVEL
//      ADL thread
//
// RETURN VALUE
//      Status to indicate if calculations were successful (SUCCESS/FAIL).
//
///////////////////////////////////////////////////////////////////////////////
int DefaultCaptureBuff(void)
{
    int                     Status = SUCCESS;
    CaptureBufStompParmTyp  ParmValue;

    memset(&ParmValue, 0, sizeof(ParmValue));

       switch (PARM(CapBufWizardTyp))
       {
           case 0:  //Default for creation of new systems are found in P0Rule.xls
               ParmValue.CapChanCnt   = 30;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_TripFlt;
               ParmValue.CapTrigValue = 0;
               ParmValue.CapTrigMode  = TRIG_BOOL;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 50;

               ParmValue.CapAdr01     = (void*)LADR_S_VabT2;
               ParmValue.CapAdr02     = (void*)LADR_S_VbcT2;
               ParmValue.CapAdr03     = (void*)LADR_R_IaT2;
               ParmValue.CapAdr04     = (void*)LADR_R_IbT2;
               ParmValue.CapAdr05     = (void*)LADR_R_IcT2;
               ParmValue.CapAdr06     = (void*)LADR_L_IaT2;
               ParmValue.CapAdr07     = (void*)LADR_L_IbT2;
               ParmValue.CapAdr08     = (void*)LADR_L_IcT2;
               ParmValue.CapAdr09     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr10     = (void*)LADR_R_SpdFbk;
               ParmValue.CapAdr11     = (void*)LADR_S_IaT2;
               ParmValue.CapAdr12     = (void*)LADR_S_IbT2;
               ParmValue.CapAdr13     = (void*)LADR_S_IcT2;
               ParmValue.CapAdr14     = (void*)LADR_R_VabT2;
               ParmValue.CapAdr15     = (void*)LADR_R_VbcT2;
               ParmValue.CapAdr16     = (void*)LADR_L_VabT2;
               ParmValue.CapAdr17     = (void*)LADR_L_VbcT2;
               ParmValue.CapAdr18     = (void*)LADR_GmVMagPuT2;
               ParmValue.CapAdr19     = (void*)LADR_GmIMagT3;
               ParmValue.CapAdr20     = (void*)LADR_GmPwrT2;
               ParmValue.CapAdr21     = (void*)LADR_GmQPwrT2;
               ParmValue.CapAdr22     = (void*)LADR_L_DbCmd;
               ParmValue.CapAdr23     = (void*)LADR_R_CroBarSt;
               ParmValue.CapAdr24     = (void*)LADR_CoolantTemp;
               ParmValue.CapAdr25     = (void*)LADR_InletAirTemp;
               ParmValue.CapAdr26     = (void*)LADR_Q1UVHld;
               ParmValue.CapAdr27     = (void*)LADR_KdfClsFbk;
               ParmValue.CapAdr28     = (void*)LADR_L_SeqSt;
               ParmValue.CapAdr29     = (void*)LADR_R_SeqSt;
               ParmValue.CapAdr30     = (void*)LADR_HVACState;
               break;

           case 1:                         //Line feedbacks
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_1_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 1
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_TripFlt;
               ParmValue.CapTrigValue = 0;
               ParmValue.CapTrigMode  = TRIG_BOOL;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 80;

               ParmValue.CapAdr01     = (void*)LADR_L_Vab;
               ParmValue.CapAdr02     = (void*)LADR_L_Vbc;
               ParmValue.CapAdr03     = (void*)LADR_L_Ia;
               ParmValue.CapAdr04     = (void*)LADR_L_Ib;
               ParmValue.CapAdr05     = (void*)LADR_L_Ic;
               ParmValue.CapAdr06     = (void*)LADR_L_Vdc;
               ParmValue.CapAdr07     = (void*)LADR_GmAngElecFbk;
               ParmValue.CapAdr08     = (void*)LADR_GmPllState;

               break;

           case 2:                         //Rotor feedbacks
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_1_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 1
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_TripFlt;
               ParmValue.CapTrigValue = 0;
               ParmValue.CapTrigMode  = TRIG_BOOL;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 80;

               ParmValue.CapAdr01     = (void*)LADR_R_Vab;
               ParmValue.CapAdr02     = (void*)LADR_R_Vbc;
               ParmValue.CapAdr03     = (void*)LADR_R_Ia;
               ParmValue.CapAdr04     = (void*)LADR_R_Ib;
               ParmValue.CapAdr05     = (void*)LADR_R_Ic;
               ParmValue.CapAdr06     = (void*)LADR_R_IxCmd;
               ParmValue.CapAdr07     = (void*)LADR_R_IyFbk;
               ParmValue.CapAdr08     = (void*)LADR_R_IxFbk;

               break;

           case 3:                         //Stator feedbacks
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_1_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 1
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_TripFlt;
               ParmValue.CapTrigValue = 0;
               ParmValue.CapTrigMode  = TRIG_BOOL;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 80;

               ParmValue.CapAdr01     = (void*)LADR_S_Vab;
               ParmValue.CapAdr02     = (void*)LADR_S_Vbc;
               ParmValue.CapAdr03     = (void*)LADR_S_Ia;
               ParmValue.CapAdr04     = (void*)LADR_S_Ib;
               ParmValue.CapAdr05     = (void*)LADR_S_Ic;
               ParmValue.CapAdr06     = (void*)LADR_S_K1Status;
               ParmValue.CapAdr07     = (void*)LADR_S_AngElecFbk;
               ParmValue.CapAdr08     = (void*)LADR_GmPllState;

               break;

           case 4:                         // Grid Monitor feedbacks
               ParmValue.CapChanCnt   = 10;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_1_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 1
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_TripFlt;
               ParmValue.CapTrigValue = 0;
               ParmValue.CapTrigMode  = TRIG_BOOL;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 80;

               ParmValue.CapAdr01     = (void*)LADR_GmVan;
               ParmValue.CapAdr02     = (void*)LADR_GmVbn;
               ParmValue.CapAdr03     = (void*)LADR_GmVcn;
               ParmValue.CapAdr04     = (void*)LADR_GmIa;
               ParmValue.CapAdr05     = (void*)LADR_GmIb;
               ParmValue.CapAdr06     = (void*)LADR_GmIc;
               ParmValue.CapAdr07     = (void*)LADR_GmAngElecFbk;
               ParmValue.CapAdr08     = (void*)LADR_GmPllW;
               ParmValue.CapAdr08     = (void*)LADR_GmVMagPuT2;
               ParmValue.CapAdr09     = (void*)LADR_GmPwrT2;
               ParmValue.CapAdr10     = (void*)LADR_GmQPwrT2;

               break;

           case 5:                         // Gm calculations
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_3_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 3
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_TripFlt;
               ParmValue.CapTrigValue = 0;
               ParmValue.CapTrigMode  = TRIG_BOOL;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 80;

               ParmValue.CapAdr01     = (void*)LADR_GmV1nMag;
               ParmValue.CapAdr02     = (void*)LADR_GmV2nMag;
               ParmValue.CapAdr03     = (void*)LADR_GmV3nMag;
               ParmValue.CapAdr04     = (void*)LADR_GmI1Mag;
               ParmValue.CapAdr05     = (void*)LADR_GmI2Mag;
               ParmValue.CapAdr06     = (void*)LADR_GmI3Mag;
               ParmValue.CapAdr07     = (void*)LADR_GmPwr;
               ParmValue.CapAdr08     = (void*)LADR_GmQPwr;

               break;

           case 6:                         // Sequencing
               ParmValue.CapChanCnt   = 10;         // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_TripFlt;
               ParmValue.CapTrigValue = 0;
               ParmValue.CapTrigMode  = TRIG_BOOL;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 80;

               ParmValue.CapAdr01     = (void*)LADR_L_RunCmd;
               ParmValue.CapAdr02     = (void*)LADR_Q1UVHld;
               ParmValue.CapAdr03     = (void*)LADR_Q1Status;
               ParmValue.CapAdr04     = (void*)LADR_KdfClsCmd;
               ParmValue.CapAdr05     = (void*)LADR_KdfClsFbk;
               ParmValue.CapAdr06     = (void*)LADR_S_K1ClsCmd;
               ParmValue.CapAdr07     = (void*)LADR_S_K1Status;
               ParmValue.CapAdr08     = (void*)LADR_R_CCOpnCmd;
               ParmValue.CapAdr09     = (void*)LADR_KdfClsCmd;
               ParmValue.CapAdr10     = (void*)LADR_KdfClsFbk;

               break;

           case 7:                         // LVRT
               ParmValue.CapChanCnt   = 27;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_GmVMagPuT2;
               ParmValue.CapTrigValue = 0.85;
               ParmValue.CapTrigMode  = TRIG_LT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 5;

               ParmValue.CapAdr01     = (void*)LADR_GmVMagPuT2;
               ParmValue.CapAdr02     = (void*)LADR_VregOutAPk;
               ParmValue.CapAdr03     = (void*)LADR_GmIyInst;
               ParmValue.CapAdr04     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr05     = (void*)LADR_GmPllState;
               ParmValue.CapAdr06     = (void*)LADR_GmNSeqHi;
               ParmValue.CapAdr07     = (void*)LADR_R_CroBarAct;
               ParmValue.CapAdr08     = (void*)LADR_R_TrqCalT2;
               ParmValue.CapAdr09     = (void*)LADR_L_DbCmd;
               ParmValue.CapAdr10     = (void*)LADR_R_SpdFbk;
               ParmValue.CapAdr11     = (void*)LADR_GmPwrT2;
               ParmValue.CapAdr12     = (void*)LADR_R_TrqCmd;
               ParmValue.CapAdr13     = (void*)LADR_PllTrqRed;
               ParmValue.CapAdr14     = (void*)LADR_DTDTrqCmdTot;
               ParmValue.CapAdr15     = (void*)LADR_R_TrqCmdInTt;
               ParmValue.CapAdr16     = (void*)LADR_GmPllErrT2;
               ParmValue.CapAdr17     = (void*)LADR_VarRefRem;
               ParmValue.CapAdr18     = (void*)LADR_GmQPwrT2;
               ParmValue.CapAdr19     = (void*)LADR_L_CnvOutPwr;
               ParmValue.CapAdr20     = (void*)LADR_R_CnvOutPwr;
               ParmValue.CapAdr21     = (void*)LADR_HVACState;
               ParmValue.CapAdr22     = (void*)LADR_R_IaT2;
               ParmValue.CapAdr23     = (void*)LADR_R_IbT2;
               ParmValue.CapAdr24     = (void*)LADR_R_IcT2;
               ParmValue.CapAdr25     = (void*)LADR_VarRefRem;
               ParmValue.CapAdr26     = (void*)LADR_PllTrqRed;
               ParmValue.CapAdr27     = (void*)LADR_R_TrqCmd;

               break;

           case 8:                         // Exec load test
               ParmValue.CapChanCnt   = 30;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_1_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 1
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_ExecLoad;
               ParmValue.CapTrigValue = 10e14;
               ParmValue.CapTrigMode  = TRIG_GT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 5;

               ParmValue.CapAdr01     = (void*)LADR_S_VabT2;
               ParmValue.CapAdr02     = (void*)LADR_S_VbcT2;
               ParmValue.CapAdr03     = (void*)LADR_R_IaT2;
               ParmValue.CapAdr04     = (void*)LADR_R_IbT2;
               ParmValue.CapAdr05     = (void*)LADR_R_IcT2;
               ParmValue.CapAdr06     = (void*)LADR_L_IaT2;
               ParmValue.CapAdr07     = (void*)LADR_L_IbT2;
               ParmValue.CapAdr08     = (void*)LADR_L_IcT2;
               ParmValue.CapAdr09     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr10     = (void*)LADR_R_SpdFbk;
               ParmValue.CapAdr11     = (void*)LADR_S_IaT2;
               ParmValue.CapAdr12     = (void*)LADR_S_IbT2;
               ParmValue.CapAdr13     = (void*)LADR_S_IcT2;
               ParmValue.CapAdr14     = (void*)LADR_R_VabT2;
               ParmValue.CapAdr15     = (void*)LADR_R_VbcT2;
               ParmValue.CapAdr16     = (void*)LADR_L_VabT2;
               ParmValue.CapAdr17     = (void*)LADR_L_VbcT2;
               ParmValue.CapAdr18     = (void*)LADR_GmVMagPuT2;
               ParmValue.CapAdr19     = (void*)LADR_GmIMagT3;
               ParmValue.CapAdr20     = (void*)LADR_GmPwrT2;
               ParmValue.CapAdr21     = (void*)LADR_GmQPwrT2;
               ParmValue.CapAdr22     = (void*)LADR_L_DbCmd;
               ParmValue.CapAdr23     = (void*)LADR_ExecLoad;
               ParmValue.CapAdr24     = (void*)LADR_ExecLoadPk;
               ParmValue.CapAdr22     = (void*)LADR_ExecLoadT1;
               ParmValue.CapAdr26     = (void*)LADR_ExecLoadT1Pk;
               ParmValue.CapAdr27     = (void*)LADR_ExecLoadT2;
               ParmValue.CapAdr28     = (void*)LADR_ExecLoadT2Pk;
               ParmValue.CapAdr29     = (void*)LADR_ExecLoadT3;
               ParmValue.CapAdr30     = (void*)LADR_ExecLoadT3Pk;

               break;

           case 9:                         // Torsional Mech Test
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_TripFlt;
               ParmValue.CapTrigValue = 0;
               ParmValue.CapTrigMode  = TRIG_BOOL;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 95;

               ParmValue.CapAdr01     = (void*)LADR_R_TrqCalT2;
               ParmValue.CapAdr02     = (void*)LADR_R_SpdGen;
               ParmValue.CapAdr03     = (void*)LADR_R_TorTrqMag;
               ParmValue.CapAdr04     = (void*)LADR_R_TorTrqInt;
               ParmValue.CapAdr05     = (void*)LADR_PulseGenFreq;
               ParmValue.CapAdr06     = (void*)LADR_R_TorTrqErr;
               ParmValue.CapAdr07     = (void*)LADR_R_TorTrqInt;
               ParmValue.CapAdr08     = (void*)LADR_R_TrqCmdInTr;

               break;

           case 10:                         // Ac voltage regulator step
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_VregTstCmd;
               ParmValue.CapTrigValue = -0.001F;
               ParmValue.CapTrigMode  = TRIG_LT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 5;

               ParmValue.CapAdr01     = (void*)LADR_VregCmd;
               ParmValue.CapAdr02     = (void*)LADR_VregOut;
               ParmValue.CapAdr03     = (void*)LADR_VregFbk;
               ParmValue.CapAdr04     = (void*)LADR_L_VregErr;
               ParmValue.CapAdr05     = (void*)LADR_IyGrid;
               ParmValue.CapAdr06     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr07     = (void*)LADR_R_TrqCalT2;
               ParmValue.CapAdr08     = (void*)LADR_VregTstCmd;

               break;

           case 11:                         // Dc voltage regulator step
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_OscOut;
               ParmValue.CapTrigValue = -0.01F;
               ParmValue.CapTrigMode  = TRIG_LT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 5;

               ParmValue.CapAdr01     = (void*)LADR_L_VregStep;
               ParmValue.CapAdr02     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr03     = (void*)LADR_L_VregErr;
               ParmValue.CapAdr04     = (void*)LADR_L_IxCmdT2;
               ParmValue.CapAdr05     = (void*)LADR_L_IxFbkT2;
               ParmValue.CapAdr06     = (void*)LADR_R_TrqCalT2;
               ParmValue.CapAdr07     = (void*)LADR_VregOut;
               ParmValue.CapAdr08     = (void*)LADR_GmIxFbk;

               break;

           case 12:                         // Rotor dc ireg step
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_R_DcXStep;
               ParmValue.CapTrigValue = 0.01F;
               ParmValue.CapTrigMode  = TRIG_GT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 10;

               ParmValue.CapAdr01     = (void*)LADR_R_DcXStep;
               ParmValue.CapAdr02     = (void*)LADR_R_IxregDcX;
               ParmValue.CapAdr03     = (void*)LADR_R_IyregDcY;
               ParmValue.CapAdr04     = (void*)LADR_GmPllVyErrT2;
               ParmValue.CapAdr05     = (void*)LADR_GmPllState;
               ParmValue.CapAdr06     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr07     = (void*)LADR_S_IxFbkT2;
               ParmValue.CapAdr08     = (void*)LADR_S_IyFbkT2;

               break;

           case 13:                         // Generator Characteristic test by Iy
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_PulseGenOut;
               ParmValue.CapTrigValue = 0.001F;
               ParmValue.CapTrigMode  = TRIG_GT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 5;

               ParmValue.CapAdr01     = (void*)LADR_S_IyFbkT2;
               ParmValue.CapAdr02     = (void*)LADR_S_IxFbkT2;
               ParmValue.CapAdr03     = (void*)LADR_S_VxFbkT2;
               ParmValue.CapAdr04     = (void*)LADR_S_VyFbkT2;
               ParmValue.CapAdr05     = (void*)LADR_R_IyFbkT2;
               ParmValue.CapAdr06     = (void*)LADR_R_IxFbkT2;
               ParmValue.CapAdr07     = (void*)LADR_R_VyFbkT2;
               ParmValue.CapAdr08     = (void*)LADR_R_VxFbkT2;

               break;

           case 14:                         // Torque Step
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_PulseGenOut;
               ParmValue.CapTrigValue = 0.001F;
               ParmValue.CapTrigMode  = TRIG_GT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 5;

               ParmValue.CapAdr01     = (void*)LADR_R_TrqCalT2;
               ParmValue.CapAdr02     = (void*)LADR_R_TorSpdBpFl;
               ParmValue.CapAdr03     = (void*)LADR_R_SpdGen;
               ParmValue.CapAdr04     = (void*)LADR_R_TorSpdMag;
               ParmValue.CapAdr05     = (void*)LADR_PulseGenFreq;
               ParmValue.CapAdr06     = (void*)LADR_R_TorTrqErr;
               ParmValue.CapAdr07     = (void*)LADR_R_TorTrqInt;
               ParmValue.CapAdr08     = (void*)LADR_R_TrqCmdInTr;

               break;

           case 15:                         // Line dc ireg step
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_OscOut;
               ParmValue.CapTrigValue = -0.01F;
               ParmValue.CapTrigMode  = TRIG_LT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 5;

               ParmValue.CapAdr01     = (void*)LADR_L_DcXStep;
               ParmValue.CapAdr02     = (void*)LADR_L_DcYStep;
               ParmValue.CapAdr03     = (void*)LADR_L_IxregDcX;
               ParmValue.CapAdr04     = (void*)LADR_L_IyregDcY;
               ParmValue.CapAdr05     = (void*)LADR_GmPllState;
               ParmValue.CapAdr06     = (void*)LADR_S_PllState;
               ParmValue.CapAdr07     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr08     = (void*)LADR_PwrGrid;

               break;

           case 16:                         // Line neg seq step
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_OscOut;
               ParmValue.CapTrigValue = -0.001F;
               ParmValue.CapTrigMode  = TRIG_LT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 5;

               ParmValue.CapAdr01     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr02     = (void*)LADR_L_IxregResX;
               ParmValue.CapAdr03     = (void*)LADR_L_IyregResY;
               ParmValue.CapAdr04     = (void*)LADR_L_NsXStep;
               ParmValue.CapAdr05     = (void*)LADR_L_NsYStep;
               ParmValue.CapAdr06     = (void*)LADR_L_IxregRsOut;
               ParmValue.CapAdr07     = (void*)LADR_L_IyregRsOut;
               ParmValue.CapAdr08     = (void*)LADR_R_TrqCal;

               break;

           case 17:                         // Rotor neg seq step
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_OscOut;
               ParmValue.CapTrigValue = -0.001F;
               ParmValue.CapTrigMode  = TRIG_LT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 5;

               ParmValue.CapAdr01     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr02     = (void*)LADR_R_IxregResX;
               ParmValue.CapAdr03     = (void*)LADR_R_IyregResY;
               ParmValue.CapAdr04     = (void*)LADR_R_NsXStep;
               ParmValue.CapAdr05     = (void*)LADR_R_NsYStep;
               ParmValue.CapAdr06     = (void*)LADR_R_IxregRsOut;
               ParmValue.CapAdr07     = (void*)LADR_R_IyregRsOut;
               ParmValue.CapAdr08     = (void*)LADR_R_TrqCal;

               break;

           case 18:                         // Line pll step
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_1_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 1
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_OscOut;
               ParmValue.CapTrigValue = 0.001F;
               ParmValue.CapTrigMode  = TRIG_LT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 5;

               ParmValue.CapAdr01     = (void*)LADR_GmPllVyErr;
               ParmValue.CapAdr02     = (void*)LADR_GmPllW;
               ParmValue.CapAdr03     = (void*)LADR_OscOut;
               ParmValue.CapAdr04     = (void*)LADR_GmAngElecFbk;
               ParmValue.CapAdr05     = (void*)LADR_GmAngElecCmd;
               ParmValue.CapAdr06     = (void*)LADR_L_VdcFbk;
               ParmValue.CapAdr07     = (void*)LADR_GmVMagPuT1;
               ParmValue.CapAdr08     = (void*)LADR_R_TrqCal;

               break;

           case 19:                         // pll sequencing
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_3_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 3
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_GmPllWT2;
               ParmValue.CapTrigValue = 1.0F;
               ParmValue.CapTrigMode  = TRIG_GT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 10;

               ParmValue.CapAdr01     = (void*)LADR_GmPllState;
               ParmValue.CapAdr02     = (void*)LADR_GmPllWT2;
               ParmValue.CapAdr03     = (void*)LADR_S_PllState;
               ParmValue.CapAdr04     = (void*)LADR_S_K1ClsCmd;
               ParmValue.CapAdr05     = (void*)LADR_L_VdcFbk;
               ParmValue.CapAdr06     = (void*)LADR_PwrGrid;
               ParmValue.CapAdr07     = (void*)LADR_QPwrGrid;
               ParmValue.CapAdr08     = (void*)LADR_R_TrqCalT2;

               break;

           case 20:                         // speed method tests
               ParmValue.CapChanCnt   = 8;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_R_RtrPosMeth;
               ParmValue.CapTrigValue = 0;
               ParmValue.CapTrigMode  = TRIG_BOOL;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 10;

               ParmValue.CapAdr01     = (void*)LADR_R_SpdT2;
               ParmValue.CapAdr02     = (void*)LADR_R_SpdT2RAF;
               ParmValue.CapAdr03     = (void*)LADR_R_AngEstErr;
               ParmValue.CapAdr04     = (void*)LADR_R_Spd;
               ParmValue.CapAdr05     = (void*)LADR_R_SpdSlowT1;
               ParmValue.CapAdr06     = (void*)LADR_R_RtrPosn;
               ParmValue.CapAdr07     = (void*)LADR_R_TrqCmdInTr;
               ParmValue.CapAdr08     = (void*)LADR_R_TrqCalT2;

               break;

           case 21:                         // Capacitive switching test
               ParmValue.CapChanCnt   = 30;             // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_R_IxregDcX;
               ParmValue.CapTrigValue = 0.05F;
               ParmValue.CapTrigMode  = TRIG_GT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 10;

               ParmValue.CapAdr01     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr02     = (void*)LADR_GmVMagPuT2;
               ParmValue.CapAdr03     = (void*)LADR_R_IxCmd0;
               ParmValue.CapAdr04     = (void*)LADR_R_IyCmd0;
               ParmValue.CapAdr05     = (void*)LADR_R_IxFbk;
               ParmValue.CapAdr06     = (void*)LADR_R_IyFbk;
               ParmValue.CapAdr07     = (void*)LADR_S_IxCmd;
               ParmValue.CapAdr08     = (void*)LADR_S_IyCmd;
               ParmValue.CapAdr09     = (void*)LADR_S_IxFbkT2;
               ParmValue.CapAdr10     = (void*)LADR_S_IyFbkT2;
               ParmValue.CapAdr11     = (void*)LADR_S_VxFbkT2;
               ParmValue.CapAdr12     = (void*)LADR_S_VyFbkT2;
               ParmValue.CapAdr13     = (void*)LADR_L_IxFbkT2;
               ParmValue.CapAdr14     = (void*)LADR_R_IyFbkT2;
               ParmValue.CapAdr15     = (void*)LADR_R_IyCmd0; 
               ParmValue.CapAdr16     = (void*)LADR_R_IxFbkT2; 
               ParmValue.CapAdr17     = (void*)LADR_GmPllState;
               ParmValue.CapAdr18     = (void*)LADR_HVACState;
               ParmValue.CapAdr19     = (void*)LADR_GmPllVyErr;
               ParmValue.CapAdr20     = (void*)LADR_R_SpdT1;
               ParmValue.CapAdr21     = (void*)LADR_S_PllW;
               ParmValue.CapAdr22     = (void*)LADR_R_IxregDcX;
               ParmValue.CapAdr23     = (void*)LADR_R_IyregDcY;
               ParmValue.CapAdr24     = (void*)LADR_R_IxregDcOut;
               ParmValue.CapAdr25     = (void*)LADR_R_IyregDcOut;
               ParmValue.CapAdr26     = (void*)LADR_R_VxCmd;
               ParmValue.CapAdr27     = (void*)LADR_GmPwrT2;
               ParmValue.CapAdr28     = (void*)LADR_IyPFCmd;
               ParmValue.CapAdr29     = (void*)LADR_GmIyInst;
               ParmValue.CapAdr30     = (void*)LADR_R_TorTrqInt;
               break;

           case 22:                         // Open Grid Detection
               ParmValue.CapChanCnt   = 30;              // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_OpnGrdDetect;
               ParmValue.CapTrigValue = 0;
               ParmValue.CapTrigMode  = TRIG_BOOL;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 10;

               ParmValue.CapAdr01     = (void*)LADR_GmVMagPuT2;
               ParmValue.CapAdr02     = (void*)LADR_GridFltLatch;
               ParmValue.CapAdr03     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr04     = (void*)LADR_R_CroBarSt;
               ParmValue.CapAdr05     = (void*)LADR_GmIaT2;
               ParmValue.CapAdr06     = (void*)LADR_GmIbT2;
               ParmValue.CapAdr07     = (void*)LADR_GmIcT2;
               ParmValue.CapAdr08     = (void*)LADR_GmPwrT2;
               ParmValue.CapAdr09     = (void*)LADR_HVACState;
               ParmValue.CapAdr10     = (void*)LADR_OpnPhsSat;
               ParmValue.CapAdr11     = (void*)LADR_OpnPhsEnable;
               ParmValue.CapAdr12     = (void*)LADR_IaGridAbs1;
               ParmValue.CapAdr13     = (void*)LADR_IbGridAbs1;
               ParmValue.CapAdr14     = (void*)LADR_IcGridAbs1;
               ParmValue.CapAdr15     = (void*)LADR_OpnPhsATmr1;
               ParmValue.CapAdr16     = (void*)LADR_OpnPhsBTmr1;
               ParmValue.CapAdr17     = (void*)LADR_OpnPhsCTmr1;
               ParmValue.CapAdr18     = (void*)LADR_DisconWhlRun;
               ParmValue.CapAdr19     = (void*)LADR_OpnPhsARstTmr1;
               ParmValue.CapAdr20     = (void*)LADR_OpnPhsBRstTmr1;
               ParmValue.CapAdr21     = (void*)LADR_IaGridAbs2;
               ParmValue.CapAdr23     = (void*)LADR_IbGridAbs2;
               ParmValue.CapAdr24     = (void*)LADR_IcGridAbs2;
               ParmValue.CapAdr25     = (void*)LADR_OpnPhsATmr2;
               ParmValue.CapAdr26     = (void*)LADR_OpnPhsBTmr2;
               ParmValue.CapAdr27     = (void*)LADR_OpnPhsCTmr2;
               ParmValue.CapAdr28     = (void*)LADR_OpnPhsARstTmr2;
               ParmValue.CapAdr29     = (void*)LADR_OpnPhsBRstTmr2;
               ParmValue.CapAdr30     = (void*)LADR_OpnPhsCRstTmr2;

               break;

           case 23:                         // LVRT Fast Currents
               ParmValue.CapChanCnt   = 17;             // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_1_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 1
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_TripFlt;
               ParmValue.CapTrigValue = 0.85F;
               ParmValue.CapTrigMode  = TRIG_BOOL;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 10;

               ParmValue.CapAdr01     = (void*)LADR_GmVMagPuT1;
               ParmValue.CapAdr02     = (void*)LADR_L_VdcFbk;
               ParmValue.CapAdr03     = (void*)LADR_GmIa;
               ParmValue.CapAdr04     = (void*)LADR_GmIb;
               ParmValue.CapAdr05     = (void*)LADR_GmIc;
               ParmValue.CapAdr06     = (void*)LADR_IaGridDeltT1;
               ParmValue.CapAdr07     = (void*)LADR_IbGridDeltT1;
               ParmValue.CapAdr08     = (void*)LADR_IcGridDeltT1;
               ParmValue.CapAdr09     = (void*)LADR_S_Ia;
               ParmValue.CapAdr10     = (void*)LADR_S_Ib;
               ParmValue.CapAdr11     = (void*)LADR_S_Ic;
               ParmValue.CapAdr12     = (void*)LADR_R_Ia;
               ParmValue.CapAdr13     = (void*)LADR_R_Ib;
               ParmValue.CapAdr14     = (void*)LADR_R_Ic;
               ParmValue.CapAdr15     = (void*)LADR_L_Ia;
               ParmValue.CapAdr16     = (void*)LADR_L_Ib;
               ParmValue.CapAdr17     = (void*)LADR_L_Ic;

               break;

           case 24:                         // Islanding Diagnostics
               ParmValue.CapChanCnt   = 23;            // valid values are 1 to 30
               ParmValue.CapTaskExec  = CAP_TASK_2_RATE;
               ParmValue.CapPMult     = 1;              //Once per task 2
               ParmValue.CapEnbDelay  = 2880;           // 2880 Minutes (48 hours)

               ParmValue.CapTrigAdr   = (void*)LADR_HVACState;
               ParmValue.CapTrigValue = 4;
               ParmValue.CapTrigMode  = TRIG_GT;
               ParmValue.CapTrigType  = TRIG_EDGE;
               ParmValue.CapPreSample = 35;

               ParmValue.CapAdr01     = (void*)LADR_GmVMagPuT2;
               ParmValue.CapAdr02     = (void*)LADR_HVACState;
               ParmValue.CapAdr03     = (void*)LADR_L_VdcFbkT2;
               ParmValue.CapAdr04     = (void*)LADR_GmIyInst;
               ParmValue.CapAdr05     = (void*)LADR_S_VMaxPllFil;
               ParmValue.CapAdr06     = (void*)LADR_Q1OpnReq;
               ParmValue.CapAdr07     = (void*)LADR_R_IxCmd0;
               ParmValue.CapAdr08     = (void*)LADR_L_DtmTrJncTemp;
               ParmValue.CapAdr09     = (void*)LADR_S_VMxIslInh;
               ParmValue.CapAdr10     = (void*)LADR_IslandDet;
               ParmValue.CapAdr11     = (void*)LADR_S_VMaxHiTmr;
               ParmValue.CapAdr12     = (void*)LADR_R_VMgIxDcyDy;
               ParmValue.CapAdr13     = (void*)LADR_HVACHotPll;
               ParmValue.CapAdr14     = (void*)LADR_S_VMaxHiDet;
               ParmValue.CapAdr16     = (void*)LADR_S_VMaxHiEnb;
               ParmValue.CapAdr17     = (void*)LADR_HVACHotPll;
               ParmValue.CapAdr18     = (void*)LADR_R_VrgIntGnDy;
               ParmValue.CapAdr19     = (void*)LADR_R_VrgIntLmDy;
               ParmValue.CapAdr20     = (void*)LADR_IslandDet;
               ParmValue.CapAdr21     = (void*)LADR_R_VrgIntLmDy;
               ParmValue.CapAdr22     = (void*)LADR_R_VrgIntLmDy;
               ParmValue.CapAdr23     = (void*)LADR_Q1OpnReq;

               break;

          }


    if (CaptureBuf.StompParms(ParmValue) == FAIL)
        Status = FAIL;

    return (Status);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Set diagnostic filter configuration to a default setting
//
// EXECUTION LEVEL
//      ADL thread
//
// RETURN VALUE
//      Status to indicate if calculations were successful (SUCCESS/FAIL).
//
///////////////////////////////////////////////////////////////////////////////
int DefaultDiagFilter(void)
{
    int Status = SUCCESS;

    if ( GpDiagFilter.RunRules()  == FAIL )  Status = FAIL;
    if ( R_GpDiagFilter.RunRules() == FAIL )  Status = FAIL;

    return (Status);
}



///////////////////////////////////////////////////////////////////////////////
// Function:            DefaultRelayOuts
// Execution Level:
// Formal Inputs:
// Formal Outputs:
// Abstract:
//        This function sets up relay output default values
///////////////////////////////////////////////////////////////////////////////
int DefaultRelayOuts(void)
{
    return (SUCCESS);
}
