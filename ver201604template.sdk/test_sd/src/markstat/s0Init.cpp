/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        TMS320VC33 DSP
* File:        $S0INIT.C30
* Language:    TI 'C'
* Copyright:   1996 GE Motors and Industrial Systems, Salem VA
*
* Abstract: Product specific initilizations.
*
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* InitTasks
*
*==============================================================================
* 970120 0001 SAB  TEMPORARY branch from IMD\A0TKINIT.C30.
*                  Sreg init removed. motor contorl init changed to source init
* 970120 0002 SAB  Remove trqinit call
* 970120 0003 SAB Include S0EEEMUL.H, not A0EEEMUL.H
* 970131 0004 SAB Rename EE.CI.Mtr_Ctl_Bits to EE.CI.Src_Ctl_Bits
* 970304 0005 SAB Add init for BAIA_Rly_1, Rly_2, Rly_3
* 970310 0006 SAB Add BAIA_Stat_Rgr
* 970318 SAB  Header file comments and use products.h
* 970320 SAB  Add RACK_MODE conditionals
* 970321 SAB  Remove function ProductInit to S2INIT.C30 and create PwmSourceCtlInit
*             Leave previous ProductInit functinality in this file,
*             ImdMotorCtlInit.
* 970325 SAB  Import all motor control init functionality
* 970325 SAB  Add declaration for Lp_Gnd_Vco
* 970327 SAB  Init L_Auto_Start_V
* 970403 SAB  Remove Lp_Gnd_Vco
* 970403 SAB  Remove Vdc_Inst, Rename Vx,Vy,Ix,Iy_Inst to Vx,Vy,Ix,Iy
*             Rename Vx_Inst_Sim, Vy_Inst_Sim --> Vx_Brdg_Sim, Vy_Brdg_Sim
* 970404 SAB  Remove Rack_Mode conditinal for baia defs, remove Addr_Peripherals
* 970407 SAB  Init all sincos structures
* 970415 SAB  Remove EE.Pwm_Dbl_Req logic
* 970415 SAB  Remove Ac_Line_W init.  Rename Vx,Vy_Line_Calc --> Vx,Vy_Line_Cal
* 970421 SAB  Add call to BrgThrmInit.
* 970422 SAB  Remove init of Vln_Scl
* 970424 SAB  Remove init of Run_Cmd_Req,  Remove Ref_4_Avg
* 970501 SAB  Remove I_Mag_Sqr
* 970507 SAB  EE.CI.Pll_W_Max --> EE.CI.Pll_W_Max_R, Vdc_Nom --> Vdc_Nom_Cal
* 970513 SAB  Remove Ph_Imb_Err, other phase imbalance vars
* 970520 SAB  Remove L_Line_Cal, L_Line_Dt
* 970527 SAB  Remove Vln_In init.
* 970610 DGR  Remove (Ia,Ib,Ic)_Sqr_Fil.
* 970612 DGR  Remove W_Toc_T1.
* 970625 SAB  Remove Ext_Iy_Ref
* 970722 SAB  Remove RACK_MODE conditionals
* 970729 SAB  Rename Fuse_Ok_Fil to Lp_Fuse_Ok
* 970911 SAB  Remove Vreg_Wo_Comp.
* 970917 SAB  Remove init of Vdc_Nom_Cal
* 971118 SAB  Remove L_Chg_Opn_Tmr init, others
* 971120 SAB  Remove many variables and init at place of declaration
* 971120 SAB  Remove externs to pwm stuff
* 980521 SAB  Rename PwmSourceCtlInit to InitLCL, Remove call to
*                    BrgThrmInit  -- now in Y1Init
* 980713 SAB  Init Vdc_Sim_T2 with EE.CI.Vdc_Nom for ACMVRS6_A
* 980717 SAB  Modify init of Vdc_Sim to be Vll_In*sqrt2
* 980724 SAB  If ACMVRS6-A call XfmrRafInit in InitLCL
*             (for CAW)
* 990212 SAB  ACCBN_A: Remove declaration of src_ctl_1
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 021121 SAB  L_LSimMode --> SimMode
* 030131 SAB  Remove product conditional compliles
* 030402 SAB  Remove L_AutoStartV
* 051213 SAB  Init L_PllWMaxInt, L_PllWMinInt
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
*______________________________________________________________________________
* Last Mod: December 12, 2007 3:26:37PM
*/

#include "x0FrameWork.h"
#include "Product.h"



extern CtlBt1       L_SrcCtl1          ;
extern RegLimFlgs   L_InnrRegLim       ;

extern void InitSequencer();
extern void InitFbks();

  /* InitSourceControl variables */




void InitSourceControl(void)
{
    unsigned *Struct_Init;                   // temp variable used to initialize structures

    Struct_Init = (unsigned*) &L_SrcCtl1;    // Set pointer to address of L_SrcCtl1
    *Struct_Init = 0;

    Struct_Init = (unsigned*) &L_InnrRegLim; // Set pointer to address of L_InnrRegLim
    *Struct_Init = 0;
}


/*****************************************************************************
*
*       private variables
*
*****************************************************************************/


/******************************************************************************
* Function:         InitLCL
* Execution Level:  Initialization
* Execution Time:   Not measured
*
* Formal Inputs:
* Formal Outputs:
*
* Global Inputs:
* Global Outputs:
*
* Return Value:
*
* Function Calls:   InitDacs
*                   pwm2_init
*                   InitSequencer
*                   InitFbks
*
* Abstract:         Initialize controller functions
*
******************************************************************************/

void InitLCL (void)

{



  InitSequencer();      // Initialize sequencer variables


  InitFbks();



  InitSourceControl();  // Initialize motor source parameters

}
