///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       2-level Cell Test derived class
//
// DESCRIPTION:
//      This file contains the methods for the 2-level cell test class.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0CelTst.vsd, b0CelTst.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include "x0MsgStk.h"
// product
#include "Product.h"
#include "b0CelTst.h"


// Constants
//----------

// Task 1 state machine states

#define  IDLE_T1        -1
#define  ENB_00_T1       0
#define  ENB_01_T1       1
#define  ENB_02_T1       2
#define  ENB_03_T1       3
#define  ENB_04_T1       4
#define  SCT_00_T1     100
#define  SCT_01_T1     101
#define  SCT_02_T1     102
#define  SCT_03_T1     103
#define  SCT_04_T1     104
#define  SCT_05_T1     105
#define  SCT_06_T1     106
#define  SCT_07_T1     107
#define  SCT_08_T1     108
#define  SCT_09_T1     109
#define  SCT_10_T1     110
#define  SCT_11_T1     111
#define  SCT_12_T1     112
#define  SCT_13_T1     113
#define  SCT_14_T1     114
#define  SCT_15_T1     115
#define  OCT_00_T1     200
#define  OCT_01_T1     201
#define  OCT_02_T1     202
#define  OCT_03_T1     203
#define  OCT_04_T1     204
#define  OCT_05_T1     205
#define  OCT_06_T1     206
#define  OCT_07_T1     207
#define  OCT_08_T1     208
#define  OCT_09_T1     209
#define  OCT_10_T1     210
#define  OCT_11_T1     211
#define  OCT_12_T1     212
#define  VFT_0_T1      250
#define  VFT_1_T1      251
#define  VFT_2_T1      252
#define  VFT_3_T1      253
#define  VFT_4_T1      254
#define  VFT_5_T1      255
#define  VFT_6_T1      256
#define  GATE_00_T1    300
#define  GATE_01_T1    301
#define  GATE_02_T1    302
#define  GATE_03_T1    303
#define  GATE_04_T1    304
#define  GATE_05_T1    305
#define  GATE_06_T1    306
#define  GATE_07_T1    307
#define  GATE_08_T1    308
#define  GATE_09_T1    309
#define  BRG_RST_0_T1  350
#define  BRG_RST_1_T1  351
#define  BRG_RST_2_T1  352
#define  BRG_RST_3_T1  353
#define  BRG_RST_4_T1  354
#define  BRG_RST_5_T1  355
#define  DISAB_0_T1    400
#define  DISAB_1_T1    401
#define  ENB_AB_0_T1   420
#define  DSB_AB_0_T1   440

// Task 2 state machine states

#define  IDLE_T2       -1
#define  ENB_0_T2       0
#define  ENB_1_T2       1
#define  ENB_2_T2       2
#define  ENB_3_T2       3
#define  ENB_4_T2       4
#define  ENB_5_T2       5
#define  ENB_6_T2       6
#define  ENB_7_T2       7
#define  ENB_8_T2       8
#define  DISAB_0_T2    20
#define  DISAB_1_T2    21
#define  DISAB_2_T2    22
#define  ENB_AB_0_T2   40
#define  ENB_AB_1_T2   41
#define  ENB_AB_2_T2   42
#define  ENB_AB_3_T2   43
#define  ENB_AB_4_T2   44
#define  ENB_AB_5_T2   45
#define  DSB_AB_0_T2   60
#define  DSB_AB_1_T2   61
#define  MSG_00_T2    100
#define  MSG_01_T2    101
#define  MSG_02_T2    102
#define  MSG_03_T2    103
#define  MSG_04_T2    104
#define  MSG_05_T2    105
#define  MSG_06_T2    106
#define  MSG_07_T2    107
#define  MSG_08_T2    108
#define  MSG_09_T2    109
#define  MSG_10_T2    110
#define  MSG_11_T2    111
#define  MSG_12_T2    112
#define  MSG_13_T2    113
#define  MSG_14_T2    114
#define  MSG_15_T2    115
#define  MSG_16_T2    116
#define  MSG_17_T2    117
#define  MSG_18_T2    118
#define  MSG_19_T2    119
#define  MSG_20_T2    120
#define  MSG_21_T2    121
#define  MSG_22_T2    122
#define  MSG_23_T2    123
#define  MSG_24_T2    124
#define  MSG_25_T2    125
#define  MSG_26_T2    126
#define  MSG_27_T2    127

// Pulse width calculation constants

#define  PLS_TM_MIN  0.0
#define  PLS_TM_MAX  1.0

// Leakage current threshold constant

#define  CT_I_LKG_PCT  1.0

// Voltage threshold constant

#define  CT_V_THRSH_PCT  75.0

// Current and voltage level output data constants

#define  I_ZERO_LVL   0u
#define  I_POS_LVL    1u
#define  I_NEG_LVL    2u
#define  I_DT_CR_LVL  3u
#define  V_ZERO_LVL   0u
#define  V_POS_LVL    1u
#define  V_NEG_LVL    2u
#define  V_DT_CR_LVL  3u

// Feedback masks for cell test tests

#define  SCT_FBK_MASK  0x000FFF
#define  OCT_FBK_MASK  0x000FFF
#define  VFT_FBK_MASK  0xFFF000

// Feedback levels for no bridge failures

#define  AU_SCT_NORMAL    (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_NORMAL    (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_NORMAL    (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_NORMAL    (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_NORMAL    (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_NORMAL    (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

#define  AUBL_OCT_NORMAL  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_NORMAL  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_NORMAL  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_NORMAL  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_NORMAL  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_NORMAL  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Expected current levels during Crowbar Contactor Testing:
// Brg1: Expect the right current level on the parked phase which is enabled only on Brg1
// Brg2: Current will free-wheel thru all 3 phases as the pulse phase is enabled only on Brg2
#define  AUBL_OCT_NORMAL_CCT  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_NORMAL_CCT  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_NORMAL_CCT  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  ALBU_OCT_NORMAL_CCT  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_NORMAL_CCT  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_NORMAL_CCT  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))


#define  AUBL_VFT_NORMAL  (( V_POS_LVL << 12 ) | ( V_DT_CR_LVL << 16 ) \
                            | ( V_DT_CR_LVL << 20 ))
#define  BUCL_VFT_NORMAL  (( V_DT_CR_LVL << 12 ) | ( V_POS_LVL << 16 ) \
                            | ( V_DT_CR_LVL << 20 ))
#define  CUAL_VFT_NORMAL  (( V_DT_CR_LVL << 12 ) | ( V_DT_CR_LVL << 16 ) \
                            | ( V_POS_LVL << 20 ))
#define  ALBU_VFT_NORMAL  (( V_NEG_LVL << 12 ) | ( V_DT_CR_LVL << 16 ) \
                            | ( V_DT_CR_LVL << 20 ))
#define  BLCU_VFT_NORMAL  (( V_DT_CR_LVL << 12 ) | ( V_NEG_LVL << 16 ) \
                            | ( V_DT_CR_LVL << 20 ))
#define  CLAU_VFT_NORMAL  (( V_DT_CR_LVL << 12 ) | ( V_DT_CR_LVL << 16 ) \
                            | ( V_NEG_LVL << 20 ))

// Short circuit test feedback levels for short in A upper device

#define  AU_SCT_SHORT_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_SHORT_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_SHORT_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_SHORT_AU  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_SHORT_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_SHORT_AU  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Short circuit test feedback levels for short in A lower device

#define  AU_SCT_SHORT_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_SHORT_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_SHORT_AL  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_SHORT_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_SHORT_AL  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CL_SCT_SHORT_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for short in B upper device

#define  AU_SCT_SHORT_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_SHORT_BU  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_SHORT_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_SHORT_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_SHORT_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_SHORT_BU  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Short circuit test feedback levels for short in B lower device

#define  AU_SCT_SHORT_BL  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_SHORT_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_SHORT_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_SHORT_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_SHORT_BL  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CL_SCT_SHORT_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for short in C upper device

#define  AU_SCT_SHORT_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_SHORT_CU  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  BU_SCT_SHORT_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_SHORT_CU  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CU_SCT_SHORT_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_SHORT_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for short in C lower device

#define  AU_SCT_SHORT_CL  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  AL_SCT_SHORT_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_SHORT_CL  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  BL_SCT_SHORT_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_SHORT_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_SHORT_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for A upper device gating problem

#define  AU_SCT_GATE_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_GATE_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_GATE_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_GATE_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_GATE_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_GATE_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for A lower device gating problem

#define  AU_SCT_GATE_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_GATE_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_GATE_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_GATE_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_GATE_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_GATE_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for B upper device gating problem

#define  AU_SCT_GATE_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_GATE_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_GATE_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_GATE_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_GATE_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_GATE_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for B lower device gating problem

#define  AU_SCT_GATE_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_GATE_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_GATE_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_GATE_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_GATE_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_GATE_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for C upper device gating problem

#define  AU_SCT_GATE_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_GATE_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_GATE_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_GATE_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_GATE_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_GATE_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for C lower device gating problem

#define  AU_SCT_GATE_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_GATE_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_GATE_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_GATE_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_GATE_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_GATE_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for grounded motor phase A

#define  AU_SCT_MTR_GND_A  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_MTR_GND_A  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_MTR_GND_A  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_MTR_GND_A  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_MTR_GND_A  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_MTR_GND_A  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for grounded motor phase B

#define  AU_SCT_MTR_GND_B  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_MTR_GND_B  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_MTR_GND_B  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_MTR_GND_B  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_MTR_GND_B  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CL_SCT_MTR_GND_B  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Short circuit test feedback levels for grounded motor phase C

#define  AU_SCT_MTR_GND_C  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_MTR_GND_C  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_MTR_GND_C  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_MTR_GND_C  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_MTR_GND_C  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CL_SCT_MTR_GND_C  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Short circuit test feedback levels for indeterminate motor ground

#define  AU_SCT_MTR_GND  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_MTR_GND  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_MTR_GND  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_MTR_GND  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_MTR_GND  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CL_SCT_MTR_GND  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Short circuit test feedback levels for unexpected current

#define  AU_SCT_UNEXP  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  AL_SCT_UNEXP  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  A_SCT_UNEXP  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BU_SCT_UNEXP  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BL_SCT_UNEXP  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  B_SCT_UNEXP  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CU_SCT_UNEXP  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CL_SCT_UNEXP  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  C_SCT_UNEXP  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Open circuit test feedback levels for open in A upper device

#define  AUBL_OCT_OPEN_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_OPEN_AU  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_OPEN_AU  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_OPEN_AU  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_OPEN_AU  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_OPEN_AU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Open circuit test feedback levels for open in A lower device

#define  AUBL_OCT_OPEN_AL  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_OPEN_AL  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_OPEN_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  ALBU_OCT_OPEN_AL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_OPEN_AL  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_OPEN_AL  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Open circuit test feedback levels for open in B upper device

#define  AUBL_OCT_OPEN_BU  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_OPEN_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CUAL_OCT_OPEN_BU  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_OPEN_BU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_OPEN_BU  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_OPEN_BU  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Open circuit test feedback levels for open in B lower device

#define  AUBL_OCT_OPEN_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_OPEN_BL  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_OPEN_BL  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_OPEN_BL  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_OPEN_BL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CLAU_OCT_OPEN_BL  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Open circuit test feedback levels for open in C upper device

#define  AUBL_OCT_OPEN_CU  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_OPEN_CU  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_OPEN_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  ALBU_OCT_OPEN_CU  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_OPEN_CU  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CLAU_OCT_OPEN_CU  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Open circuit test feedback levels for open in C lower device

#define  AUBL_OCT_OPEN_CL  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_OPEN_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CUAL_OCT_OPEN_CL  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_OPEN_CL  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_OPEN_CL  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_OPEN_CL  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Open circuit test feedback levels for reversed phase A shunt

#define  AUBL_OCT_SHUNT_A  (( I_NEG_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_SHUNT_A  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_SHUNT_A  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_SHUNT_A  (( I_POS_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_SHUNT_A  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_SHUNT_A  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Open circuit test feedback levels for reversed phase B shunt

#define  AUBL_OCT_SHUNT_B  (( I_POS_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_SHUNT_B  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_SHUNT_B  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_SHUNT_B  (( I_NEG_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_SHUNT_B  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_SHUNT_B  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Open circuit test feedback levels for reversed phase C shunt

#define  AUBL_OCT_SHUNT_C  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_SHUNT_C  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CUAL_OCT_SHUNT_C  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  ALBU_OCT_SHUNT_C  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_SHUNT_C  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CLAU_OCT_SHUNT_C  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))

// Open circuit test feedback levels for reversed phase A,B shunt

#define  AUBL_OCT_SHUNT_AB  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_SHUNT_AB  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_SHUNT_AB  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_SHUNT_AB  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_SHUNT_AB  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_SHUNT_AB  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Open circuit test feedback levels for reversed phase B,C shunt

#define  AUBL_OCT_SHUNT_BC  (( I_POS_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_SHUNT_BC  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CUAL_OCT_SHUNT_BC  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  ALBU_OCT_SHUNT_BC  (( I_NEG_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_SHUNT_BC  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CLAU_OCT_SHUNT_BC  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))

// Open circuit test feedback levels for reversed phase A,C shunt

#define  AUBL_OCT_SHUNT_AC  (( I_NEG_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_SHUNT_AC  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CUAL_OCT_SHUNT_AC  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  ALBU_OCT_SHUNT_AC  (( I_POS_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_SHUNT_AC  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CLAU_OCT_SHUNT_AC  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))

// Open circuit test feedback levels for reversed phase A,B,C shunts

#define  AUBL_OCT_SHUNT_ABC  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_SHUNT_ABC  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CUAL_OCT_SHUNT_ABC  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  ALBU_OCT_SHUNT_ABC  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_SHUNT_ABC  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CLAU_OCT_SHUNT_ABC  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))

// Open circuit test feedback levels for bad phase A shunt feedback

#define  AUBL_OCT_IFBK_A  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_IFBK_A  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_IFBK_A  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_IFBK_A  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_IFBK_A  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_IFBK_A  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Open circuit test feedback levels for bad phase B shunt feedback

#define  AUBL_OCT_IFBK_B  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_IFBK_B  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_IFBK_B  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_IFBK_B  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_IFBK_B  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_IFBK_B  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Open circuit test feedback levels for bad phase C shunt feedback

#define  AUBL_OCT_IFBK_C  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_IFBK_C  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CUAL_OCT_IFBK_C  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  ALBU_OCT_IFBK_C  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_IFBK_C  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CLAU_OCT_IFBK_C  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Open circuit test feedback levels for bad phase A,B shunt feedback

#define  AUBL_OCT_IFBK_AB  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_IFBK_AB  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_IFBK_AB  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_IFBK_AB  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_IFBK_AB  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_IFBK_AB  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Open circuit test feedback levels for bad phase B,C shunt feedback

#define  AUBL_OCT_IFBK_BC  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_IFBK_BC  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CUAL_OCT_IFBK_BC  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  ALBU_OCT_IFBK_BC  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_IFBK_BC  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CLAU_OCT_IFBK_BC  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Open circuit test feedback levels for bad phase A,C shunt feedback

#define  AUBL_OCT_IFBK_AC  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_IFBK_AC  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CUAL_OCT_IFBK_AC  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  ALBU_OCT_IFBK_AC  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_IFBK_AC  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CLAU_OCT_IFBK_AC  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Open circuit test feedback levels for unconnected load phase A

#define  AUBL_OCT_LOAD_A  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_LOAD_A  (( I_ZERO_LVL ) | ( I_POS_LVL << 4 ) | ( I_NEG_LVL << 8 ))
#define  CUAL_OCT_LOAD_A  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  ALBU_OCT_LOAD_A  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_LOAD_A  (( I_ZERO_LVL ) | ( I_NEG_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  CLAU_OCT_LOAD_A  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Open circuit test feedback levels for unconnected load phase B

#define  AUBL_OCT_LOAD_B  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_LOAD_B  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CUAL_OCT_LOAD_B  (( I_NEG_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_POS_LVL << 8 ))
#define  ALBU_OCT_LOAD_B  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_LOAD_B  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CLAU_OCT_LOAD_B  (( I_POS_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_NEG_LVL << 8 ))

// Open circuit test feedback levels for unconnected load phase C

#define  AUBL_OCT_LOAD_C  (( I_POS_LVL ) | ( I_NEG_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_LOAD_C  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CUAL_OCT_LOAD_C  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  ALBU_OCT_LOAD_C  (( I_NEG_LVL ) | ( I_POS_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_LOAD_C  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CLAU_OCT_LOAD_C  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Open circuit test feedback levels for unconnected load phase A,B,C

#define  AUBL_OCT_LOAD_ABC  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BUCL_OCT_LOAD_ABC  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CUAL_OCT_LOAD_ABC  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  ALBU_OCT_LOAD_ABC  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  BLCU_OCT_LOAD_ABC  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))
#define  CLAU_OCT_LOAD_ABC  (( I_ZERO_LVL ) | ( I_ZERO_LVL << 4 ) | ( I_ZERO_LVL << 8 ))

// Hardware fault bit locations

#define  NO_GATE_BIT    0
#define  DSAT_AU_BIT    1
#define  DSAT_AL_BIT    2
#define  DSAT_BU_BIT    3
#define  DSAT_BL_BIT    4
#define  DSAT_CU_BIT    5
#define  DSAT_CL_BIT    6
#define  IOC_A_BIT      7
#define  IOC_B_BIT      8
#define  IOC_C_BIT      9
#define  I_DECAY_BIT   10
#define  DSAT_AU1_BIT  11
#define  DSAT_AL1_BIT  12
#define  DSAT_BU1_BIT  13
#define  DSAT_BL1_BIT  14
#define  DSAT_CU1_BIT  15
#define  DSAT_CL1_BIT  16
#define  DSAT_AU2_BIT  17
#define  DSAT_AL2_BIT  18
#define  DSAT_BU2_BIT  19
#define  DSAT_BL2_BIT  20
#define  DSAT_CU2_BIT  21
#define  DSAT_CL2_BIT  22
#define  IMBLNC_A_BIT  23
#define  IMBLNC_B_BIT  24
#define  IMBLNC_C_BIT  25

// Hardware fault codes

#define  NO_FLT_CODE    0x00000000
#define  NO_GATE_CODE   0x00000001
#define  DSAT_AU_CODE   0x00000002
#define  DSAT_AL_CODE   0x00000004
#define  DSAT_BU_CODE   0x00000008
#define  DSAT_BL_CODE   0x00000010
#define  DSAT_CU_CODE   0x00000020
#define  DSAT_CL_CODE   0x00000040
#define  IOC_A_CODE     0x00000080
#define  IOC_B_CODE     0x00000100
#define  IOC_C_CODE     0x00000200
#define  I_DECAY_CODE   0x00000400
#define  DSAT_AU1_CODE  0x00000800
#define  DSAT_AL1_CODE  0x00001000
#define  DSAT_BU1_CODE  0x00002000
#define  DSAT_BL1_CODE  0x00004000
#define  DSAT_CU1_CODE  0x00008000
#define  DSAT_CL1_CODE  0x00010000
#define  DSAT_AU2_CODE  0x00020000
#define  DSAT_AL2_CODE  0x00040000
#define  DSAT_BU2_CODE  0x00080000
#define  DSAT_BL2_CODE  0x00100000
#define  DSAT_CU2_CODE  0x00200000
#define  DSAT_CL2_CODE  0x00400000
#define  IMBLNC_A_CODE  0x00800000
#define  IMBLNC_B_CODE  0x01000000
#define  IMBLNC_C_CODE  0x02000000

// Data array manipulation constants
#define  AU_PTR     0
#define  AL_PTR     1
#define  BU_PTR     2
#define  BL_PTR     3
#define  CU_PTR     4
#define  CL_PTR     5
#define  AUBL_PTR   6
#define  BUCL_PTR   7
#define  CUAL_PTR   8
#define  ALBU_PTR   9
#define  BLCU_PTR  10
#define  CLAU_PTR  11

// Gating sequence repeat / delay constants

//#define  PULSE_RPT        4
#define  PULSE_RPT        1
#define  CT_BRG_RST_DLY   0.004
#define  CT_I_DCY_DLY     1.0
#define  CT_I_ZERO_DLY    0.002
#define  CT_I_ZERO_CNT   10
#define  LINE_PARK_TIME  0.020

// Diagnostic message constants

#define  CT_MSG_STK_SZ  100
#define  CT_MSG_DLY      10.0

#define  MSG_SPC         (( ' ' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_AU_1        (( 'A' ) | ( 'U' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_AU_2        (( ' ' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_AL_1        (( 'A' ) | ( 'L' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_AL_2        (( ' ' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_BU_1        (( 'B' ) | ( 'U' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_BU_2        (( ' ' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_BL_1        (( 'B' ) | ( 'L' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_BL_2        (( ' ' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_CU_1        (( 'C' ) | ( 'U' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_CU_2        (( ' ' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_CL_1        (( 'C' ) | ( 'L' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_CL_2        (( ' ' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_AUBL_1      (( 'A' ) | ( 'U' << 8 ) | ( '/' << 16 ) | ( 'B' << 24 ))
#define  MSG_AUBL_2      (( 'L' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_BUCL_1      (( 'B' ) | ( 'U' << 8 ) | ( '/' << 16 ) | ( 'C' << 24 ))
#define  MSG_BUCL_2      (( 'L' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_CUAL_1      (( 'C' ) | ( 'U' << 8 ) | ( '/' << 16 ) | ( 'A' << 24 ))
#define  MSG_CUAL_2      (( 'L' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_ALBU_1      (( 'A' ) | ( 'L' << 8 ) | ( '/' << 16 ) | ( 'B' << 24 ))
#define  MSG_ALBU_2      (( 'U' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_BLCU_1      (( 'B' ) | ( 'L' << 8 ) | ( '/' << 16 ) | ( 'C' << 24 ))
#define  MSG_BLCU_2      (( 'U' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_CLAU_1      (( 'C' ) | ( 'L' << 8 ) | ( '/' << 16 ) | ( 'A' << 24 ))
#define  MSG_CLAU_2      (( 'U' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_BAD_DEV_1   (( 'B' ) | ( 'A' << 8 ) | ( 'D' << 16 ) | ( ' ' << 24 ))
#define  MSG_BAD_DEV_2   (( ' ' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_I_POS       (( '+' ) | ( 'I' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_I_NEG       (( '-' ) | ( 'I' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_I_ZERO      (( ' ' ) | ( '0' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_I_DT_CR     (( ' ' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_V_POS       (( '+' ) | ( 'V' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_V_NEG       (( '-' ) | ( 'V' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_V_ZERO      (( ' ' ) | ( '0' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_V_DT_CR     (( ' ' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_BAD_FBK     (( 'B' ) | ( 'A' << 8 ) | ( 'D' << 16 ) | ( ' ' << 24 ))
#define  MSG_NO_FLT_1    (( 'N' ) | ( 'o' << 8 ) | ( ' ' << 16 ) | ( 'F' << 24 ))
#define  MSG_NO_FLT_2    (( 'a' ) | ( 'u' << 8 ) | ( 'l' << 16 ) | ( 't' << 24 ))
#define  MSG_NO_GT_1     (( 'N' ) | ( 'o' << 8 ) | ( 't' << 16 ) | ( ' ' << 24 ))
#define  MSG_NO_GT_2     (( 'D' ) | ( 'o' << 8 ) | ( 'n' << 16 ) | ( 'e' << 24 ))
#define  MSG_DSAT_AU_1   (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_AU_2   (( 't' ) | ( ' ' << 8 ) | ( 'A' << 16 ) | ( 'U' << 24 ))
#define  MSG_DSAT_BU_1   (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_BU_2   (( 't' ) | ( ' ' << 8 ) | ( 'B' << 16 ) | ( 'U' << 24 ))
#define  MSG_DSAT_CU_1   (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_CU_2   (( 't' ) | ( ' ' << 8 ) | ( 'C' << 16 ) | ( 'U' << 24 ))
#define  MSG_DSAT_AL_1   (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_AL_2   (( 't' ) | ( ' ' << 8 ) | ( 'A' << 16 ) | ( 'L' << 24 ))
#define  MSG_DSAT_BL_1   (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_BL_2   (( 't' ) | ( ' ' << 8 ) | ( 'B' << 16 ) | ( 'L' << 24 ))
#define  MSG_DSAT_CL_1   (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_CL_2   (( 't' ) | ( ' ' << 8 ) | ( 'C' << 16 ) | ( 'L' << 24 ))
#define  MSG_IOC_A_1     (( 'I' ) | ( 'o' << 8 ) | ( 'c' << 16 ) | ( ' ' << 24 ))
#define  MSG_IOC_A_2     (( 'A' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_IOC_B_1     (( 'I' ) | ( 'o' << 8 ) | ( 'c' << 16 ) | ( ' ' << 24 ))
#define  MSG_IOC_B_2     (( 'B' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_IOC_C_1     (( 'I' ) | ( 'o' << 8 ) | ( 'c' << 16 ) | ( ' ' << 24 ))
#define  MSG_IOC_C_2     (( 'C' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))
#define  MSG_I_DCY_1     (( 'I' ) | ( ' ' << 8 ) | ( 'D' << 16 ) | ( 'e' << 24 ))
#define  MSG_I_DCY_2     (( 'c' ) | ( 'a' << 8 ) | ( 'y' << 16 ) | ( ' ' << 24 ))
#define  MSG_DSAT_AU1_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_AU1_2  (( 't' ) | ( 'A' << 8 ) | ( 'U' << 16 ) | ( '1' << 24 ))
#define  MSG_DSAT_BU1_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_BU1_2  (( 't' ) | ( 'B' << 8 ) | ( 'U' << 16 ) | ( '1' << 24 ))
#define  MSG_DSAT_CU1_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_CU1_2  (( 't' ) | ( 'C' << 8 ) | ( 'U' << 16 ) | ( '1' << 24 ))
#define  MSG_DSAT_AL1_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_AL1_2  (( 't' ) | ( 'A' << 8 ) | ( 'L' << 16 ) | ( '1' << 24 ))
#define  MSG_DSAT_BL1_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_BL1_2  (( 't' ) | ( 'B' << 8 ) | ( 'L' << 16 ) | ( '1' << 24 ))
#define  MSG_DSAT_CL1_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_CL1_2  (( 't' ) | ( 'C' << 8 ) | ( 'L' << 16 ) | ( '1' << 24 ))
#define  MSG_DSAT_AU2_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_AU2_2  (( 't' ) | ( 'A' << 8 ) | ( 'U' << 16 ) | ( '2' << 24 ))
#define  MSG_DSAT_BU2_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_BU2_2  (( 't' ) | ( 'B' << 8 ) | ( 'U' << 16 ) | ( '2' << 24 ))
#define  MSG_DSAT_CU2_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_CU2_2  (( 't' ) | ( 'C' << 8 ) | ( 'U' << 16 ) | ( '2' << 24 ))
#define  MSG_DSAT_AL2_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_AL2_2  (( 't' ) | ( 'A' << 8 ) | ( 'L' << 16 ) | ( '2' << 24 ))
#define  MSG_DSAT_BL2_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_BL2_2  (( 't' ) | ( 'B' << 8 ) | ( 'L' << 16 ) | ( '2' << 24 ))
#define  MSG_DSAT_CL2_1  (( 'D' ) | ( 'e' << 8 ) | ( 's' << 16 ) | ( 'a' << 24 ))
#define  MSG_DSAT_CL2_2  (( 't' ) | ( 'C' << 8 ) | ( 'L' << 16 ) | ( '2' << 24 ))
#define  MSG_IMBLNC_A_1  (( 'I' ) | ( 'm' << 8 ) | ( 'b' << 16 ) | ( 'l' << 24 ))
#define  MSG_IMBLNC_A_2  (( 'n' ) | ( 'c' << 8 ) | ( ' ' << 16 ) | ( 'A' << 24 ))
#define  MSG_IMBLNC_B_1  (( 'I' ) | ( 'm' << 8 ) | ( 'b' << 16 ) | ( 'l' << 24 ))
#define  MSG_IMBLNC_B_2  (( 'n' ) | ( 'c' << 8 ) | ( ' ' << 16 ) | ( 'B' << 24 ))
#define  MSG_IMBLNC_C_1  (( 'I' ) | ( 'm' << 8 ) | ( 'b' << 16 ) | ( 'l' << 24 ))
#define  MSG_IMBLNC_C_2  (( 'n' ) | ( 'c' << 8 ) | ( ' ' << 16 ) | ( 'C' << 24 ))
#define  MSG_BAD_FLT_1   (( 'B' ) | ( 'A' << 8 ) | ( 'D' << 16 ) | ( ' ' << 24 ))
#define  MSG_BAD_FLT_2   (( ' ' ) | ( ' ' << 8 ) | ( ' ' << 16 ) | ( ' ' << 24 ))

// Externals
//----------


// local prototypes to resolve forward references
//-----------------------------------------------


// Variables
//----------


// Data Passing
//-------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Derived class constructor to initialize resources
//
// EXECUTION LEVEL
//      Derived class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cCellTest::cCellTest (
        InListTyp    InList,
        InsListTyp   InsList
)
        : cBridgeTest(InsList), In(InList)     // Pass the Ins list to base class constructor
{
    Hdw_Tst_Req         = false;
    Hdw_Tst_ReqT1       = false;
    Hdw_Tst_Stat        = false;
    Hdw_Tst_StatT2      = false;
    Hdw_Tst_Act         = false;
    Hdw_Tst_ActT2       = false;
    CT_BrgEnbCmd        = false;
    CT_BrgEnbCmdT2      = false;
    CT_Req_T1           = false;
    CT_Stat_T1          = false;
    CT_Act_T1           = false;
    CT_ASR_T1           = 0;
    CT_ASR_T2           = 0;
    CT_Ret_St_T1        = 0;
    CT_St_T1            = IDLE_T1;
    CT_Enb_Ab_T1        = ENB_AB_0_T1;
    CT_Dsb_Ab_T1        = DSB_AB_0_T1;
    CT_St_T2            = IDLE_T2;
    CT_Enb_Ab_T2        = ENB_AB_0_T2;
    CT_Dsb_Ab_T2        = DSB_AB_0_T2;
    CT_Tmr              = 0.0F;
    Park_Tmr            = 0.0F;
    Pulse_Tm            = 0.0F;
    Pulse_Mod           = 0.0F;
    Pulse_Frac          = 0.0F;
    Pulse_Tmr           = 0;
    Pulse_Cnt           = 0;
    Loop_Cnt            = 0;
    Pls_Rpt_Cnt         = 0;
    Pls_Act_Cnt         = 0;
    Brg_Off_Cnt         = 0;
    CT_Data_Ptr         = 0;
    CT_I_Thrsh          = 0.0F;
    CT_I_Vco            = 0.0F;
    CT_V_Thrsh          = 0.0F;
    CT_Vab_Intgr        = 0.0F;
    CT_Vbc_Intgr        = 0.0F;
    CT_Ia_Acc           = 0.0F;
    CT_Ib_Acc           = 0.0F;
    CT_Ic_Acc           = 0.0F;
    CT_I_Acc_Cnt        = 0;
    memset( reinterpret_cast<void*> (&CT_Run)      , 0, sizeof(CT_Run)       );
    memset( reinterpret_cast<void*> (&CT_Data)     , 0, sizeof(CT_Data)      );
    memset( reinterpret_cast<void*> (&CT_Eval_Data), 0, sizeof(CT_Eval_Data) );
    memset( reinterpret_cast<void*> (&CT_Dly_Tm_T1), 0, sizeof(CT_Dly_Tm_T1) );
    memset( reinterpret_cast<void*> (&CT_Dly_Tm_T2), 0, sizeof(CT_Dly_Tm_T2) );
    memset( reinterpret_cast<void*> (&CT_I_Zero_Tm), 0, sizeof(CT_I_Zero_Tm) );
    OUT(CelTstBrgPwrEnb)= false;
    OUT(CelTstStat)     = false;
    OUT(CelTstAct)      = false;
    OUT(CelTstProtEnab) = true;
    OUT(CelTstBrgOff)   = true;
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Derived class destructor to free used resources.
//
// EXECUTION LEVEL
//      Derived instance destruction.
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
cCellTest::~cCellTest ()
{
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by bridge cell test class.
//
// EXECUTION LEVEL
//      Initialization and when required.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cCellTest::RunRules(void)
{
    SET_FV( CelTstCurLvl, INS(CellTestCurLvl) );
    SET_FV( CelTstVloadMax, 0.05F*INS(CellTestVllNom) );
    SET_FV( CelTstVdecayTm, 30.0F );
    SET_FV( CelTstInvModeEnb, false );
    SET_FV( CelTstVthrsPct, 0.75F );
    SET_FV( CelTstIthrsPct, 0.057F ); //Changed from .06 to work down to 90% and 88% voltages
    SET_FV( CelTstMinPls, INS(BrgLockoutTm) + 1.0e-6);

    CelTstCurThrs = PARM(CelTstIthrsPct) * INS(IshuntRat);

    return(SUCCESS);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function implements the Task 1 portion of the cell test
//      state machine.
//      This function must run after BrgThrmModelT1() for absolute values
//      of current feedbacks to be correct.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::Task1( void )
{
    if ( IN(Dual_Brg) )
    {
        AUBL_OctTst = AUBL_OCT_NORMAL_CCT;
        BUCL_OctTst = BUCL_OCT_NORMAL_CCT;
        CUAL_OctTst = CUAL_OCT_NORMAL_CCT;
        ALBU_OctTst = ALBU_OCT_NORMAL_CCT;
        BLCU_OctTst = BLCU_OCT_NORMAL_CCT;
        CLAU_OctTst = CLAU_OCT_NORMAL_CCT;
    }
    else
    {
        AUBL_OctTst = AUBL_OCT_NORMAL;
        BUCL_OctTst = BUCL_OCT_NORMAL;
        CUAL_OctTst = CUAL_OCT_NORMAL;
        ALBU_OctTst = ALBU_OCT_NORMAL;
        BLCU_OctTst = BLCU_OCT_NORMAL;
        CLAU_OctTst = CLAU_OCT_NORMAL;
    }

    // Get request from T2->T1 buffer

    CT_Req_T1 = Hdw_Tst_ReqT1;

    // Form cell test Task 2 state from sequencer variables

    CT_ASR_T1 = ( CT_Act_T1 << 2 ) | ( CT_Stat_T1 << 1 ) | ( CT_Req_T1 );



    //---------------------------------------------------------------
    // Make state machine execution decisions based on Act, Stat, Req
    switch ( CT_ASR_T1 )
    {
      case 0 :
      case 3 :
        CT_St_T1 = IDLE_T1;
        break;

      case 1 :
        if ( IN(TripFlt) )
        {
            CT_St_T1 = CT_Enb_Ab_T1;
        }
        else
        {
            CT_St_T1 = ENB_00_T1;
        }
        break;

      case 2 :
        if ( IN(TripFlt) )
        {
            CT_St_T1 = CT_Dsb_Ab_T1;
        }
        else
        {
            CT_St_T1 = DISAB_0_T1;
        }
        break;

      case 4 :
        CT_St_T1 = CT_Enb_Ab_T1;
        break;

      case 5 :
        if ( IN(TripFlt) )
        {
            CT_St_T1 = CT_Enb_Ab_T1;
        }
        break;

      case 6 :
        if ( IN(TripFlt) )
        {
            CT_St_T1 = CT_Dsb_Ab_T1;
        }
        break;

      case 7 :
        CT_St_T1 = CT_Dsb_Ab_T1;
        break;
    }

    //---------------------------
    // Execute task state machine
    switch ( CT_St_T1 )
    {
      case IDLE_T1 :                // Set active false
        CT_Act_T1    = false;
        OUT(CelTstOctAct) = false;
        break;

            // Task 1 enable states

      case ENB_00_T1 :              // Set active true
                                    // Enable bridge electronics
        CT_Act_T1    = true;
        CT_BrgEnbCmd = true;
        CT_St_T1     = ENB_01_T1;
        break;

      case ENB_01_T1 :              // Reset data collection variables
                                    // Turn power devices off
                                    // Jump to short circuit test
        CellTestResetData();
        BridgeOff( CelTstBrgOff );
        CT_St_T1 = SCT_00_T1;
        break;

      case ENB_02_T1 :              // Skip OC test if SC test failed
                                    // Jump to open circuit test
        if ( CT_Run.SCT_Fail )
        {
            CT_St_T1 = ENB_04_T1;
        }
        else
        {
            CT_St_T1 = OCT_00_T1;
        }
        break;

      case ENB_03_T1 :              // Skip VFB eval if OC test failed
                                    // Jump to voltage feedback evaluation
        if ( CT_Run.OCT_Fail )
        {
            CT_St_T1 = ENB_04_T1;
        }
        else
        {
            CT_St_T1 = VFT_0_T1;
        }
        break;

      case ENB_04_T1 :              // Set status true, active false
                                    // Disable bridge electronics
        CT_BrgEnbCmd = false;
        CT_Stat_T1   = true;
        CT_Act_T1    = false;
        CT_St_T1     = IDLE_T1;
        break;

      //------------------------------------------
      // Short circuit test (Task 1 enable) states
      case SCT_00_T1 :              // Configure for AU short circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[AU_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1 = SCT_02_T1;
        }
        else
        {
            BridgeCfg( PHS_0, PHS_ST_NEG, PHS_A, PHS_ST_POS );
            CT_Data_Ptr              = AU_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = SCT_01_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        break;

      case SCT_01_T1 :              // Skip AL test if desat fault in AU test
        if ( CT_Data[AU_PTR].Flt_Code & ( DSAT_AU_CODE | DSAT_AU1_CODE |
                                          DSAT_AU2_CODE | IMBLNC_A_CODE ))
        {
            CT_Data[AL_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1 = SCT_03_T1;
        }
        else
        {
            CT_St_T1 = SCT_02_T1;
        }
        break;

      case SCT_02_T1 :              // Configure for AL short circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[AL_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1 = SCT_03_T1;
        }
        else
        {
            BridgeCfg( PHS_0, PHS_ST_NEG, PHS_A, PHS_ST_NEG );
            CT_Data_Ptr              = AL_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = SCT_03_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        break;

      case SCT_03_T1 :              // Configure for BU short circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[BU_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1 = SCT_05_T1;
        }
        else
        {
            BridgeCfg( PHS_0, PHS_ST_NEG, PHS_B, PHS_ST_POS );
            CT_Data_Ptr              = BU_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = SCT_04_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        break;

      case SCT_04_T1 :              // Skip BL test if desat fault in BU test
        if ( CT_Data[BU_PTR].Flt_Code & ( DSAT_BU_CODE | DSAT_BU1_CODE |
                                          DSAT_BU2_CODE | IMBLNC_B_CODE ))
        {
            CT_Data[BL_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1 = SCT_06_T1;
        }
        else
        {
            CT_St_T1 = SCT_05_T1;
        }
        break;

      case SCT_05_T1 :              // Configure for BL short circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[BL_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1 = SCT_06_T1;
        }
        else
        {
            BridgeCfg( PHS_0, PHS_ST_NEG, PHS_B, PHS_ST_NEG );
            CT_Data_Ptr              = BL_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = SCT_06_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        break;

      case SCT_06_T1 :              // Configure for CU short circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[CU_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1 = SCT_08_T1;
        }
        else
        {
            BridgeCfg( PHS_0, PHS_ST_NEG, PHS_C, PHS_ST_POS );
            CT_Data_Ptr              = CU_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = SCT_07_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        break;

      case SCT_07_T1 :              // Skip CL test if desat fault in CU test
        if ( CT_Data[CU_PTR].Flt_Code & ( DSAT_CU_CODE | DSAT_CU1_CODE |
                                          DSAT_CU2_CODE | IMBLNC_C_CODE ))
        {
            CT_Data[CL_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1 = SCT_09_T1;
        }
        else
        {
            CT_St_T1 = SCT_08_T1;
        }
        break;

      case SCT_08_T1 :              // Configure for CL short circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[CL_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1 = SCT_09_T1;
        }
        else
        {
            BridgeCfg( PHS_0, PHS_ST_NEG, PHS_C, PHS_ST_NEG );
            CT_Data_Ptr              = CL_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = SCT_09_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        break;

      case SCT_09_T1 :              // Evaluate AU data
        CT_Data_Ptr = AU_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[AU_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK ) != AU_SCT_NORMAL )
                || ( CT_Eval_Data[AU_PTR].Flt_Code != NO_FLT_CODE ))
        {
            CT_Run.SCT_Fail |= 1;
        }
        CT_St_T1 = SCT_10_T1;
        break;

      case SCT_10_T1 :              // Evaluate AL data
        CT_Data_Ptr = AL_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[AL_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK ) != AL_SCT_NORMAL )
                || ( CT_Eval_Data[AL_PTR].Flt_Code != NO_FLT_CODE ))
        {
            CT_Run.SCT_Fail |= 1;
        }
        CT_St_T1 = SCT_11_T1;
        break;

      case SCT_11_T1 :              // Evaluate BU data
        CT_Data_Ptr = BU_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[BU_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK ) != BU_SCT_NORMAL )
                || ( CT_Eval_Data[BU_PTR].Flt_Code != NO_FLT_CODE ))
        {
            CT_Run.SCT_Fail |= 1;
        }
        CT_St_T1 = SCT_12_T1;
        break;

      case SCT_12_T1 :              // Evaluate BL data
        CT_Data_Ptr = BL_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[BL_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK ) != BL_SCT_NORMAL )
                || ( CT_Eval_Data[BL_PTR].Flt_Code != NO_FLT_CODE ))
        {
            CT_Run.SCT_Fail |= 1;
        }
        CT_St_T1 = SCT_13_T1;
        break;

      case SCT_13_T1 :              // Evaluate CU data
        CT_Data_Ptr = CU_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[CU_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK ) != CU_SCT_NORMAL )
                || ( CT_Eval_Data[CU_PTR].Flt_Code != NO_FLT_CODE ))
        {
            CT_Run.SCT_Fail |= 1;
        }
        CT_St_T1 = SCT_14_T1;
        break;

      case SCT_14_T1 :              // Evaluate CL data
        CT_Data_Ptr = CL_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[CL_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK ) != CL_SCT_NORMAL )
                || ( CT_Eval_Data[CL_PTR].Flt_Code != NO_FLT_CODE ))
        {
            CT_Run.SCT_Fail |= 1;
        }
        CT_St_T1 = SCT_15_T1;
        break;

      case SCT_15_T1 :              // Declare short circuit test done
        CT_Run.SCT_Done = 1;
        CT_St_T1 = ENB_02_T1;
        break;

      //-----------------------------------------
      // Open circuit test (Task 1 enable) states
      case OCT_00_T1 :              // Configure for AU/BL open circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[AUBL_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1                   = OCT_01_T1;
        }
        else
        {
            BridgeCfg( PHS_AB, PHS_ST_NEG, PHS_A, PHS_ST_POS );
            CT_Data_Ptr              = AUBL_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = OCT_01_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        OUT(CelTstOctAct) = true;
        OUT(CelTstOctPhs1) = PHASE_A;
        OUT(CelTstOctPhs2) = PHASE_B;
        break;

      case OCT_01_T1 :              // Configure for BU/CL open circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[BUCL_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1                   = OCT_02_T1;
        }
        else
        {
            BridgeCfg( PHS_BC, PHS_ST_NEG, PHS_B, PHS_ST_POS );
            CT_Data_Ptr              = BUCL_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = OCT_02_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        OUT(CelTstOctPhs1) = PHASE_B;
        OUT(CelTstOctPhs2) = PHASE_C;
        break;

      case OCT_02_T1 :              // Configure for CU/AL open circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[CUAL_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1                   = OCT_03_T1;
        }
        else
        {
            BridgeCfg( PHS_CA, PHS_ST_NEG, PHS_C, PHS_ST_POS );
            CT_Data_Ptr              = CUAL_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = OCT_03_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        OUT(CelTstOctPhs1) = PHASE_C;
        OUT(CelTstOctPhs2) = PHASE_A;
        break;

      case OCT_03_T1 :              // Configure for AL/BU open circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[ALBU_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1                   = OCT_04_T1;
        }
        else
        {
            BridgeCfg( PHS_AB, PHS_ST_POS, PHS_A, PHS_ST_NEG );
            CT_Data_Ptr              = ALBU_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = OCT_04_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        OUT(CelTstOctPhs1) = PHASE_A;
        OUT(CelTstOctPhs2) = PHASE_B;
        break;

      case OCT_04_T1 :              // Configure for BL/CU open circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[BLCU_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1                   = OCT_05_T1;
        }
        else
        {
            BridgeCfg( PHS_BC, PHS_ST_POS, PHS_B, PHS_ST_NEG );
            CT_Data_Ptr              = BLCU_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = OCT_05_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        OUT(CelTstOctPhs1) = PHASE_B;
        OUT(CelTstOctPhs2) = PHASE_C;
        break;

      case OCT_05_T1 :              // Configure for CL/AU open circuit test
                                    // Collect Vdc feedback
        if ( !CellTestTimerCfg() )
        {
            CT_Data[CLAU_PTR].Flt_Code = NO_GATE_CODE;
            CT_St_T1                   = OCT_06_T1;
        }
        else
        {
            BridgeCfg( PHS_CA, PHS_ST_POS, PHS_C, PHS_ST_NEG );
            CT_Data_Ptr              = CLAU_PTR;
            CT_Data[CT_Data_Ptr].Vdc = IN(VdcFbkT3);
            CT_Ret_St_T1             = OCT_06_T1;
            CT_St_T1                 = GATE_00_T1;
        }
        OUT(CelTstOctPhs1) = PHASE_C;
        OUT(CelTstOctPhs2) = PHASE_A;
        break;

      case OCT_06_T1 :              // Evaluate AU/BL data
        CT_Data_Ptr = AUBL_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[AUBL_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK ) != AUBL_OctTst )
                || ( CT_Eval_Data[AUBL_PTR].Flt_Code != NO_FLT_CODE ))
            CT_Run.OCT_Fail |= 1;
        CT_St_T1 = OCT_07_T1;
        OUT(CelTstOctAct) = false;
        break;

      case OCT_07_T1 :              // Evaluate BU/CL data
        CT_Data_Ptr = BUCL_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[BUCL_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK ) != BUCL_OctTst )
                || ( CT_Eval_Data[BUCL_PTR].Flt_Code != NO_FLT_CODE ))
            CT_Run.OCT_Fail |= 1;
        CT_St_T1 = OCT_08_T1;
        break;

      case OCT_08_T1 :              // Evaluate CU/AL data
        CT_Data_Ptr = CUAL_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[CUAL_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK ) != CUAL_OctTst )
                || ( CT_Eval_Data[CUAL_PTR].Flt_Code != NO_FLT_CODE ))
            CT_Run.OCT_Fail |= 1;
        CT_St_T1 = OCT_09_T1;
        break;

      case OCT_09_T1 :              // Evaluate AL/BU data
        CT_Data_Ptr = ALBU_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[ALBU_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK ) != ALBU_OctTst )
                || ( CT_Eval_Data[ALBU_PTR].Flt_Code != NO_FLT_CODE ))
            CT_Run.OCT_Fail |= 1;
        CT_St_T1 = OCT_10_T1;
        break;

      case OCT_10_T1 :              // Evaluate BL/CU data
        CT_Data_Ptr = BLCU_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[BLCU_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK ) != BLCU_OctTst )
                || ( CT_Eval_Data[BLCU_PTR].Flt_Code != NO_FLT_CODE ))
            CT_Run.OCT_Fail |= 1;
        CT_St_T1 = OCT_11_T1;
        break;

      case OCT_11_T1 :              // Evaluate CL/AU data
        CT_Data_Ptr = CLAU_PTR;
        CellTestEval();
        if ((( CT_Eval_Data[CLAU_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK ) != CLAU_OctTst )
                || ( CT_Eval_Data[CLAU_PTR].Flt_Code != NO_FLT_CODE ))
            CT_Run.OCT_Fail |= 1;
        CT_St_T1 = OCT_12_T1;
        break;

      case OCT_12_T1 :              // Declare open circuit test done
        CT_Run.OCT_Done = 1;
        CT_St_T1 = ENB_03_T1;
        break;

      //---------------------------------------------
      // Voltage feedback test (Task 1 enable) states
      case VFT_0_T1 :               // Evaluate AU/BL voltage feedback data
        if ((( CT_Eval_Data[AUBL_PTR].Fbk_Lvl.Fbk_Lvl_Int & VFT_FBK_MASK ) != AUBL_VFT_NORMAL ))
            CT_Run.VFT_Fail |= 1;
        CT_St_T1 = VFT_1_T1;
        break;

      case VFT_1_T1 :               // Evaluate BU/CL voltage feedback data
        if ((( CT_Eval_Data[BUCL_PTR].Fbk_Lvl.Fbk_Lvl_Int & VFT_FBK_MASK ) != BUCL_VFT_NORMAL ))
            CT_Run.VFT_Fail |= 1;
        CT_St_T1 = VFT_2_T1;
        break;

      case VFT_2_T1 :               // Evaluate CU/AL voltage feedback data
        if ((( CT_Eval_Data[CUAL_PTR].Fbk_Lvl.Fbk_Lvl_Int & VFT_FBK_MASK ) != CUAL_VFT_NORMAL ))
            CT_Run.VFT_Fail |= 1;
        CT_St_T1 = VFT_3_T1;
        break;

      case VFT_3_T1 :               // Evaluate AL/BU voltage feedback data
        if ((( CT_Eval_Data[ALBU_PTR].Fbk_Lvl.Fbk_Lvl_Int & VFT_FBK_MASK ) != ALBU_VFT_NORMAL ))
            CT_Run.VFT_Fail |= 1;
        CT_St_T1 = VFT_4_T1;
        break;

      case VFT_4_T1 :               // Evaluate BL/CU voltage feedback data
        if ((( CT_Eval_Data[BLCU_PTR].Fbk_Lvl.Fbk_Lvl_Int & VFT_FBK_MASK ) != BLCU_VFT_NORMAL ))
            CT_Run.VFT_Fail |= 1;
        CT_St_T1 = VFT_5_T1;
        break;

      case VFT_5_T1 :               // Evaluate CL/AU voltage feedback data
        if ((( CT_Eval_Data[CLAU_PTR].Fbk_Lvl.Fbk_Lvl_Int & VFT_FBK_MASK ) != CLAU_VFT_NORMAL ))
            CT_Run.VFT_Fail |= 1;
        CT_St_T1 = VFT_6_T1;
        break;

      case VFT_6_T1 :               // Declare voltage feedback test done
        CT_Run.VFT_Done = 1;
        CT_St_T1 = ENB_04_T1;
        break;

      //-------------------------------------
      // Bridge gating (Task 1 enable) states
      case GATE_00_T1 :             // Set up timer for current decay
                                    // Command bridge off
        if ( IN(FpgaFltLatch) == true )
        {
            CellTestGetFlts();
            CT_Data[CT_Data_Ptr].Flt_Code |= NO_GATE_CODE;
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = BRG_RST_0_T1;
        }
        else
        {
            CT_Dly_Tm_T1 = IN(SysTm1);
            CT_I_Zero_Tm = IN(SysTm1);
            CT_Ia_Acc = 0.0;
            CT_Ib_Acc = 0.0;
            CT_Ic_Acc = 0.0;
            CT_I_Acc_Cnt = 0;
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = GATE_01_T1;
        }
        break;

      case GATE_01_T1 :             // Check for current feedbacks zero
                                    // Command bridge to park state when zero
        if ( IN(FpgaFltLatch) == true )
        {
            CellTestGetFlts();
            CT_Data[CT_Data_Ptr].Flt_Code |= NO_GATE_CODE;
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = BRG_RST_0_T1;
        }
        else if ( CellTestZeroCurrent() )
        {
            BridgePark( CelTstBrgOff );
            CT_St_T1 = GATE_02_T1;
        }
        else if ( Delta_Tm( IN(SysTm1), CT_Dly_Tm_T1 ) > CT_I_DCY_DLY )
        {
            CellTestGetFlts();
            CT_Data[CT_Data_Ptr].Flt_Code |= I_DECAY_CODE;
            CT_Data[CT_Data_Ptr].Flt_Code |= NO_GATE_CODE;
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = CT_Ret_St_T1;
        }
        else
        {
            BridgeOff( CelTstBrgOff );
            if ( CT_Ret_St_T1 > OCT_00_T1 )
            {
                Park_Tmr = LINE_PARK_TIME;   // extra park time for brg OCT
            }
            else
            {
                Park_Tmr = 0.0;
            }
            CT_St_T1 = GATE_01_T1;
        }
        break;

      case GATE_02_T1 :             // Reset current accumulators
                                    // Set pulse repeat counters
                                    // Command bridge to park state
        if ( IN(FpgaFltLatch) == true )
        {
            CellTestGetFlts();
            CT_Data[CT_Data_Ptr].Flt_Code |= NO_GATE_CODE;
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = BRG_RST_0_T1;
        }
        else
        {
            CT_Ia_Acc = 0.0;
            CT_Ib_Acc = 0.0;
            CT_Ic_Acc = 0.0;
            CT_I_Acc_Cnt = 0;
            Pls_Act_Cnt = 0;
            Pls_Rpt_Cnt = PULSE_RPT;
            BridgePark( CelTstBrgOff );
            Park_Tmr -= INS(DelTm1);
            if ( Park_Tmr < 0.0 )  // sit in park longer to discharge
            {                      // harmonic filter caps, when present
                CT_St_T1 = GATE_03_T1;
            }
        }
        break;

      case GATE_03_T1 :
        // Reset voltage integrators
        // Command bridge to begin pulse state
        if ( IN(FpgaFltLatch) == true )
        {
            CellTestGetFlts();
            if ( Pls_Rpt_Cnt == PULSE_RPT )
            {
                CT_Data[CT_Data_Ptr].Flt_Code |= NO_GATE_CODE;
            }
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = BRG_RST_0_T1;
        }
        else
        {
            if ( Pls_Rpt_Cnt == PULSE_RPT )
            {
                // Haven't applied a pulse yet - reset current accumulators
                CT_Ia_Acc = 0.0;
                CT_Ib_Acc = 0.0;
                CT_Ic_Acc = 0.0;
                CT_I_Acc_Cnt = 0;
            }
            else
            {
                // Applying additional pulse - continue to accumulate currents
                CT_Ia_Acc += IN(IaFbk);
                CT_Ib_Acc += IN(IbFbk);
                CT_Ic_Acc += IN(IcFbk);
                ++CT_I_Acc_Cnt;
            }
            CT_Vab_Intgr = 0.0;
            CT_Vbc_Intgr = 0.0;
            BridgePulseFrac1( Pulse_Tmr, CelTstBrgOff );
            ++Pls_Act_Cnt;
            Loop_Cnt = Pulse_Cnt;
            if ( Loop_Cnt > 0 )
            {
                CT_St_T1 = GATE_04_T1;
            }
            else
            {
                CT_St_T1 = GATE_05_T1;
            }
        }
        break;

      case GATE_04_T1 :             // Accumulate currents, integrate voltages
                                    // Continue bridge pulse state
        if ( IN(FpgaFltLatch) == true )
        {
            CellTestGetFlts();
            if (( Pls_Rpt_Cnt == PULSE_RPT ) && ( Loop_Cnt == Pulse_Cnt ))
            {
                CT_Data[CT_Data_Ptr].Flt_Code |= NO_GATE_CODE;
            }
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = BRG_RST_0_T1;
        }
        else
        {
            CT_Ia_Acc += IN(IaFbk);
            CT_Ib_Acc += IN(IbFbk);
            CT_Ic_Acc += IN(IcFbk);
            ++CT_I_Acc_Cnt;
            CT_Vab_Intgr += ( IN(VabFbk) * INS(DelTm1) );
            CT_Vbc_Intgr += ( IN(VbcFbk) * INS(DelTm1) );
            BridgePulseFull( CelTstBrgOff );
            ++Pls_Act_Cnt;
            --Loop_Cnt;
            if ( Loop_Cnt > 0 )
            {
                CT_St_T1 = GATE_04_T1;
            }
            else
            {
                CT_St_T1 = GATE_05_T1;
            }
        }
        break;

      case GATE_05_T1 :             // Accumulate currents, integrate voltages
                                    // Command bridge to park state
        if ( IN(FpgaFltLatch) == true )
        {
            CellTestGetFlts();
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = BRG_RST_0_T1;
        }
        else
        {
            CT_Ia_Acc += IN(IaFbk);
            CT_Ib_Acc += IN(IbFbk);
            CT_Ic_Acc += IN(IcFbk);
            ++CT_I_Acc_Cnt;
            CT_Vab_Intgr += ( IN(VabFbk) * INS(DelTm1) );
            CT_Vbc_Intgr += ( IN(VbcFbk) * INS(DelTm1) );
            BridgePark( CelTstBrgOff );
            CT_St_T1 = GATE_06_T1;
        }
        break;

      case GATE_06_T1 :             // Accumulate currents, integrate voltages
                                    // Command bridge to park state
        if ( IN(FpgaFltLatch) == true )
        {
            CellTestGetFlts();
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = BRG_RST_0_T1;
        }
        else
        {
            CT_Ia_Acc += IN(IaFbk);
            CT_Ib_Acc += IN(IbFbk);
            CT_Ic_Acc += IN(IcFbk);
            ++CT_I_Acc_Cnt;
            CT_Vab_Intgr += ( IN(VabFbk) * INS(DelTm1) );
            CT_Vbc_Intgr += ( IN(VbcFbk) * INS(DelTm1) );
            BridgePark( CelTstBrgOff );
            CT_St_T1 = GATE_07_T1;
        }
        break;

      case GATE_07_T1 :             // Accumulate currents, integrate voltages
                                    // Collect voltage feedbacks
                                    // Initiate next pulse or turn off bridge
        if ( IN(FpgaFltLatch) == true )
        {
            CellTestGetFlts();
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = BRG_RST_0_T1;
        }
        else
        {
            CT_Ia_Acc += IN(IaFbk);
            CT_Ib_Acc += IN(IbFbk);
            CT_Ic_Acc += IN(IcFbk);
            ++CT_I_Acc_Cnt;
            CT_Vab_Intgr += ( IN(VabFbk) * INS(DelTm1) );
            CT_Vbc_Intgr += ( IN(VbcFbk) * INS(DelTm1) );
            CellTestVoltFbks();
            --Pls_Rpt_Cnt;
            if ( !CellTestCurrentDetect() && ( Pls_Rpt_Cnt > 0 ))
            {
                BridgePark( CelTstBrgOff );
                CT_St_T1 = GATE_03_T1;
            }
            else
            {
                Brg_Off_Cnt = Pls_Act_Cnt + 1;  // Add extra cfb accumulation state
                                                // because VCO fbk doesn't drop to zero
                                                // until 1 dt after the actual current
                BridgeOff( CelTstBrgOff );
                CT_St_T1 = GATE_08_T1;
            }
        }
        break;

      case GATE_08_T1 :             // Accumulate currents
                                    // Turn off bridge - currents fall to zero
                                    // Collect current feedbacks
        if ( IN(FpgaFltLatch) == true )
        {
            CellTestGetFlts();
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = BRG_RST_0_T1;
        }
        else
        {
            CT_Ia_Acc += IN(IaFbk);
            CT_Ib_Acc += IN(IbFbk);
            CT_Ic_Acc += IN(IcFbk);
            ++CT_I_Acc_Cnt;
            BridgeOff( CelTstBrgOff );
            --Brg_Off_Cnt;
            if ( Brg_Off_Cnt > 0 )
            {
                CT_St_T1 = GATE_08_T1;
            }
            else
            {
                CellTestCurrentFbks();
                Brg_Off_Cnt = 5;
                CT_St_T1 = GATE_09_T1;
            }
        }
        break;

      case GATE_09_T1 :             // Command bridge off
        if ( IN(FpgaFltLatch) == true )
        {
            CellTestGetFlts();
            BridgeOff( CelTstBrgOff );
            CT_St_T1 = BRG_RST_0_T1;
        }
        else
        {
            BridgeOff( CelTstBrgOff );
            --Brg_Off_Cnt;
            if ( Brg_Off_Cnt > 0 )
            {
                CT_St_T1 = GATE_09_T1;
            }
            else
            {
                CT_St_T1 = CT_Ret_St_T1;
            }
        }
        break;

      //--------------------------------------------------
      // Drive hardware fault reset (Task 1 enable) states
      case BRG_RST_0_T1 :           // Disable power electronics
                                    // Set up timer for delay
        CT_BrgEnbCmd = false;
        CT_Dly_Tm_T1 = IN(SysTm1);
        CT_St_T1 = BRG_RST_1_T1;
        break;

      case BRG_RST_1_T1 :           // Delay for fault condition to
                                    // be cleared by bridge interface
        CT_BrgEnbCmd = false;
        if ( (Delta_Tm( IN(SysTm1), CT_Dly_Tm_T1 ) > CT_BRG_RST_DLY) ||
             (IN(FpgaFltLatch) == false) )
        {
            CT_St_T1 = BRG_RST_2_T1;
        }
        else
        {
            CT_St_T1 = BRG_RST_1_T1;
        }
        break;

      case BRG_RST_2_T1 :           // Enable power electronics
        CT_BrgEnbCmd = true;
        CT_St_T1 = BRG_RST_3_T1;
        break;

      case BRG_RST_3_T1 :           // Do nothing
        CT_St_T1 = BRG_RST_4_T1;
        break;

      case BRG_RST_4_T1 :           // Do nothing
        CT_St_T1 = BRG_RST_5_T1;
        break;

      case BRG_RST_5_T1 :           // Return to originating state machine
        CT_St_T1 = CT_Ret_St_T1;
        break;

      //---------------------------
      // Task 1 abort enable states
      case ENB_AB_0_T1 :            // Turn power devices off
                                    // Set active false
        BridgeOff( CelTstBrgOff );
        CT_Act_T1    = false;
        CT_Enb_Ab_T1 = ENB_AB_0_T1;
        break;

       //----------------------
       // Task 1 disable states
      case DISAB_0_T1 :             // Set active true
        CT_Act_T1 = true;
        CT_St_T1  = DISAB_1_T1;
        break;

      case DISAB_1_T1 :             // Turn power devices off
                                    // Set status false, active false
        BridgeOff( CelTstBrgOff );
        CT_Stat_T1 = false;
        CT_Act_T1  = false;
        CT_St_T1   = IDLE_T1;
        break;

      //----------------------------
      // Task 1 abort disable states
      case DSB_AB_0_T1 :            // Turn power devices off
                                    // Set active false
        BridgeOff( CelTstBrgOff );
        CT_Stat_T1 = false;
        CT_Act_T1  = false;
        CT_Dsb_Ab_T1 = DSB_AB_0_T1;
        break;

      //------------------------
      // Unrecognized task state
      default:                      // Set active false
        CT_Act_T1 = false;
        CT_St_T1  = IDLE_T1;
        break;
    }

    //-----------------------------------------------------------------------------
    // Request for bridge test mode so Dsat and Ioc diagnostics are not annunciated
    // Zero discriminated ground currents

    if ( CT_ASR_T1 == 5 )
    {
        OUT(BrgTestControl.BrgTestReq)  = true;
        OUT(CelTstProtEnab)             = false;
    }
    else
    {
        OUT(BrgTestControl.BrgTestReq)  = false;
        OUT(CelTstProtEnab)             = true;
    }

    //----------------------------------------------------
    // Give status and active bits to Task 2 state machine
    Hdw_Tst_Stat = CT_Stat_T1;
    Hdw_Tst_Act   = CT_Act_T1;


    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function implements the Task 2 portion of the cell test
//      state machine.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::Task2( void )
{
    // Form cell test Task 2 state from sequencer variables

    CT_ASR_T2 = ( CelTstAct << 2 ) | ( CelTstStat << 1 ) | ( IN(TstReq) );

    //---------------------------------------------------------------
    // Make state machine execution decisions based on Act, Stat, Req
    switch ( CT_ASR_T2 )
    {
      case 0 :
      case 3 :
        CT_St_T2 = IDLE_T2;
        break;

      case 1 :
        if ( IN(TstDisable) )
        {
            CT_St_T2 = CT_Enb_Ab_T2;
        }
        else
        {
            CT_St_T2 = ENB_0_T2;
        }
        break;

      case 2 :
        if ( IN(TstDisable) )
        {
            CT_St_T2 = CT_Dsb_Ab_T2;
        }
        else
        {
            CT_St_T2 = DISAB_0_T2;
        }
        break;

      case 4 :
        CT_St_T2 = CT_Enb_Ab_T2;
        break;

      case 5 :
        if ( IN(TstDisable) )
        {
            CT_St_T2 = CT_Enb_Ab_T2;
        }
        break;

      case 6 :
        if ( IN(TstDisable) )
        {
            CT_St_T2 = CT_Dsb_Ab_T2;
        }
        break;

      case 7 :
        CT_St_T2 = CT_Dsb_Ab_T2;
        break;
    }

    //---------------------------
    // Execute task state machine
    switch ( CT_St_T2 )
    {
      case IDLE_T2 :                // Do nothing
            CT_Tmr = 0.0;
        break;

      //---------------------
      // Task 2 enable states
      case ENB_0_T2 :               // Set active true
        OUT(CelTstAct) = true;
        if ( IN(AutoCelTst) != AUTOCT_OFF )
        {
            CT_St_T2 = ENB_2_T2;
        }
        else
        {
            CT_St_T2  = ENB_1_T2;
        }
        break;

      case ENB_1_T2 :               // Set up diagnostic message stack
        if ( cMessageStack::InitMsgStack( CT_MSG_STK_SZ ) != SUCCESS )
        {
            PUSH_DIAG( CelTstMsgStkTrp );
            CT_St_T2 = ENB_8_T2;
        }
        else
        {
            CT_St_T2 = ENB_2_T2;
        }
        break;

      case ENB_2_T2 :               // Report start of cell test
        if ( (IN(InvalidMode) && !PARM(CelTstInvModeEnb)) == true )
        {
            if ( IN(AutoCelTst)  != AUTOCT_OFF )
            {
                CT_St_T2 = ENB_8_T2;
            }
            else
            {
                cMessageStack::PushMsg( 101, xINT, NULL ); // Cell test invoked in invalid mode
                cMessageStack::PushMsg( 199, xINT, NULL );
                cMessageStack::PushMsg( 31, xINT, NULL );
                CT_Dly_Tm_T2 = IN(SysTm2);
                CT_St_T2 = ENB_6_T2;
            }
        }
        else
        {
            if ( IN(AutoCelTst) != AUTOCT_OFF )
            {
                CT_St_T2 = ENB_3_T2;
            }
            else
            {
                if ( CT_Tmr >= PARM(CelTstVdecayTm))
                {
                   cMessageStack::PushMsg( 162, xINT, NULL ); // Decay is too long, check filter integrity
                   cMessageStack::PushMsg( 199, xINT, NULL );
                   cMessageStack::PushMsg( 31, xINT, NULL );
                   CT_Dly_Tm_T2 = IN(SysTm2);
                   CT_St_T2 = ENB_6_T2;
                }
                else if ( IN(VdfMagT2) <  PARM(CelTstVloadMax) ) // Begin conv test next time
                {
                   cMessageStack::PushMsg( 100, xINT, NULL );    // Generic invoking message
                   CT_St_T2 = ENB_3_T2;
                }
                else
                {
                   if ( CT_Tmr < INS(DelTm2) )
                   {
                      cMessageStack::PushMsg( 142, xINT, NULL ); // Waiting for up to 30 seconds for distortion filter voltaage to decay
                   }
                   CT_Tmr   = CT_Tmr + INS(DelTm2);
                   CT_St_T2 = ENB_2_T2;     // Repeat this state
                }
            }
        }
        break;

      case ENB_3_T2 :               // Enable bridge electronics
                                    // Run Task 1 state machine
        OUT(CelTstBrgPwrEnb)  = true;
        Hdw_Tst_Req = true;
        CT_St_T2           = ENB_4_T2;
        break;

      case ENB_4_T2 :               // Run Task 1 state machine
        Hdw_Tst_Req = true;
        OUT(CelTstBrgPwrEnb)  = CT_BrgEnbCmdT2;
        if ( Hdw_Tst_StatT2 == true )
        {
            CT_St_T2 = ENB_5_T2;
        }
        else
        {
            CT_St_T2 = ENB_4_T2;
        }
        break;

      case ENB_5_T2 :               // Disable bridge electronics
        OUT(CelTstBrgPwrEnb) = false;
        if ( IN(AutoCelTst) != AUTOCT_OFF )
        {
            CT_St_T2     = ENB_8_T2;
        }
        else
        {
            CT_St_T2     = MSG_00_T2;
        }
        break;

      case ENB_6_T2 :               // Delay for ADL message retrieval
        if ( ( Delta_Tm( IN(SysTm2), CT_Dly_Tm_T2 ) > CT_MSG_DLY )
                || ( cMessageStack::MsgPending == false ) )
        {
            CT_St_T2 = ENB_7_T2;
        }
        else
        {
            CT_St_T2 = ENB_6_T2;
        }
        break;

      case ENB_7_T2 :               // Free diagnostic message stack
        if ( cMessageStack::FreeMsgStack() != SUCCESS )
        {
            PUSH_DIAG( CelTstMsgStkTrp );
        }
        CT_St_T2 = ENB_8_T2;
        break;

      case ENB_8_T2 :               // Set status true, active false
        if ( (!IN(InvalidMode) || PARM(CelTstInvModeEnb)) && (IN(AutoCelTst) != AUTOCT_OFF) )
        {
             if ( IN(AutoCelTst) == AUTOCT_TO_FAIL )
             {
                 int AUBL_Fbk = CT_Eval_Data[AUBL_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;
                 int ALBU_Fbk = CT_Eval_Data[ALBU_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;
                 int BUCL_Fbk = CT_Eval_Data[BUCL_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;
                 int BLCU_Fbk = CT_Eval_Data[BLCU_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;
                 int CUAL_Fbk = CT_Eval_Data[CUAL_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;
                 int CLAU_Fbk = CT_Eval_Data[CLAU_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;

                 // Assert fault for abnormal status with crowbar commanded open
                 if      ( CT_Run.SCT_Done == 0 )
                 {
                     PUSH_DIAG(CelTstScNoDoneTrp);
                 }
                 else if ( CT_Run.SCT_Fail == 1 )
                 {
                     PUSH_DIAG(CelTstScFailTrp);
                 }
                 else if ( CT_Run.OCT_Done == 0 )
                 {
                     PUSH_DIAG(CelTstOcNoDoneTrp);
                 }
                 else if ( CT_Run.OCT_Fail == 0 )
                 {
                     PUSH_DIAG(CelTstOcCcPassTrp);
                 }
                 else if (     ( AUBL_Fbk != AUBL_OCT_LOAD_ABC )
                           ||  ( BUCL_Fbk != BUCL_OCT_LOAD_ABC )
                           ||  ( CUAL_Fbk != CUAL_OCT_LOAD_ABC )
                           ||  ( ALBU_Fbk != ALBU_OCT_LOAD_ABC )
                           ||  ( BLCU_Fbk != BLCU_OCT_LOAD_ABC )
                           ||  ( CLAU_Fbk != CLAU_OCT_LOAD_ABC ) )
                 {
                     PUSH_DIAG(CelTstOcCcCurTrp);
                 }
             }
             else if ( IN(AutoCelTst) == AUTOCT_TO_PASS )
             {
                 // Assert fault for abnormal status with crowbar commanded closed
                 if ( CT_Run.SCT_Done == 0 )
                 {
                     PUSH_DIAG(CelTstScNoDoneTrp);
                 }
                 else if  ( CT_Run.SCT_Fail == 1 )
                 {
                     PUSH_DIAG(CelTstScFailTrp);
                 }
                 else if  ( CT_Run.OCT_Done == 0 )
                 {
                     PUSH_DIAG(CelTstOcNoDoneTrp);
                 }
                 else if  ( CT_Run.OCT_Fail == 1 )
                 {
                     PUSH_DIAG(CelTstOcFailTrp);
                 }
                 else if  ( CT_Run.VFT_Done == 0 )
                 {
                     PUSH_DIAG(CelTstVtNoDoneTrp);
                 }
                 // Not possible to see brg voltage as only one phase per brg is gated at a time
                 // else if  ( CT_Run.VFT_Fail == 1 )
                 // {
                 //     PUSH_DIAG(CelTstVtFailTrp);
                 // }
             }
        }
        OUT(CelTstStat) = true;
        OUT(CelTstAct)  = false;
        CT_St_T2   = IDLE_T2;
        break;

      //------------------------------------------
      // Diagnostic message (Task 2 enable) states
      case MSG_00_T2 :              // Report short circuit test run/pass status
        if ( CT_Run.SCT_Done == 0 )
        {
            cMessageStack::PushMsg( 104, xINT, NULL );
            CT_St_T2 = MSG_09_T2;
        }
        else if ( CT_Run.SCT_Fail == 0 )
        {
            cMessageStack::PushMsg( 105, xINT, NULL );
            CT_St_T2 = MSG_09_T2;
        }
        else
        {
            cMessageStack::PushMsg( 106, xINT, NULL );
            CT_St_T2 = MSG_01_T2;
        }
        break;

      case MSG_01_T2 :              // Header for short circuit test data table
        cMessageStack::PushMsg( 113, xINT, NULL );
        CT_St_T2 = MSG_02_T2;
        break;

      case MSG_02_T2 :              // Report short circuit test AU data
        CT_Data_Ptr = AU_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_03_T2;
        break;

      case MSG_03_T2 :              // Report short circuit test AL data
        CT_Data_Ptr = AL_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_04_T2;
        break;

      case MSG_04_T2 :              // Report short circuit test BU data
        CT_Data_Ptr = BU_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_05_T2;
        break;

      case MSG_05_T2 :              // Report short circuit test BL data
        CT_Data_Ptr = BL_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_06_T2;
        break;

      case MSG_06_T2 :              // Report short circuit test CU data
        CT_Data_Ptr = CU_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_07_T2;
        break;

      case MSG_07_T2 :              // Report short circuit test CL data
        CT_Data_Ptr = CL_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_08_T2;
        break;

      case MSG_08_T2 :              // Diagnose short circuit test failure
        cMessageStack::PushMsg( 116, xINT, NULL );
        ShortCircuitTestDiagnosis();
        CT_St_T2 = MSG_09_T2;
        break;

      case MSG_09_T2 :              // Report open circuit test run/pass status
        if ( CT_Run.OCT_Done == 0 )
        {
            cMessageStack::PushMsg( 107, xINT, NULL );
            CT_St_T2 = MSG_18_T2;
        }
        else if ( CT_Run.OCT_Fail == 0 )
        {
            cMessageStack::PushMsg( 108, xINT, NULL );
            CT_St_T2 = MSG_18_T2;
        }
        else
        {
            cMessageStack::PushMsg( 109, xINT, NULL );
            CT_St_T2 = MSG_10_T2;
        }
        break;

      case MSG_10_T2 :              // Header for open circuit test data table
        cMessageStack::PushMsg( 113, xINT, NULL );
        CT_St_T2 = MSG_11_T2;
        break;

      case MSG_11_T2 :              // Report open circuit test AU/BL data
        CT_Data_Ptr = AUBL_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_12_T2;
        break;

      case MSG_12_T2 :              // Report open circuit test BU/CL data
        CT_Data_Ptr = BUCL_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_13_T2;
        break;

      case MSG_13_T2 :              // Report open circuit test CU/AL data
        CT_Data_Ptr = CUAL_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_14_T2;
        break;

      case MSG_14_T2 :              // Report open circuit test AL/BU data
        CT_Data_Ptr = ALBU_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_15_T2;
        break;

      case MSG_15_T2 :              // Report open circuit test BL/CU data
        CT_Data_Ptr = BLCU_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_16_T2;
        break;

      case MSG_16_T2 :              // Report open circuit test CL/AU data
        CT_Data_Ptr = CLAU_PTR;
        SctOctTableMessage();
        CT_St_T2 = MSG_17_T2;
        break;

      case MSG_17_T2 :              // Diagnose open circuit test failure
        cMessageStack::PushMsg( 116, xINT, NULL );
        OpenCircuitTestDiagnosis();
        CT_St_T2 = MSG_18_T2;
        break;

      case MSG_18_T2 :              // Report voltage feedback run/pass status
        if ( CT_Run.VFT_Done == 0 )
        {
            cMessageStack::PushMsg( 110, xINT, NULL );
            CT_St_T2 = MSG_27_T2;
        }
        else if ( CT_Run.VFT_Fail == 0 )
        {
            cMessageStack::PushMsg( 111, xINT, NULL );
            CT_St_T2 = MSG_27_T2;
        }
        else
        {
            cMessageStack::PushMsg( 112, xINT, NULL );
            CT_St_T2 = MSG_19_T2;
        }
        break;

      case MSG_19_T2 :              // Header for voltage feedback data table
        cMessageStack::PushMsg( 114, xINT, NULL );
        CT_St_T2 = MSG_20_T2;
        break;

      case MSG_20_T2 :              // Report voltage feedback test AU/BL data
        CT_Data_Ptr = AUBL_PTR;
        VftTableMessage();
        CT_St_T2 = MSG_21_T2;
        break;

      case MSG_21_T2 :              // Report voltage feedback test BU/CL data
        CT_Data_Ptr = BUCL_PTR;
        VftTableMessage();
        CT_St_T2 = MSG_22_T2;
        break;

      case MSG_22_T2 :              // Report voltage feedback test CU/AL data
        CT_Data_Ptr = CUAL_PTR;
        VftTableMessage();
        CT_St_T2 = MSG_23_T2;
        break;

      case MSG_23_T2 :              // Report voltage feedback test AL/BU data
        CT_Data_Ptr = ALBU_PTR;
        VftTableMessage();
        CT_St_T2 = MSG_24_T2;
        break;

      case MSG_24_T2 :              // Report voltage feedback test BL/CU data
        CT_Data_Ptr = BLCU_PTR;
        VftTableMessage();
        CT_St_T2 = MSG_25_T2;
        break;

      case MSG_25_T2 :              // Report voltage feedback test CL/AU data
        CT_Data_Ptr = CLAU_PTR;
        VftTableMessage();
        CT_St_T2 = MSG_26_T2;
        break;

      case MSG_26_T2 :              // Diagnose voltage feedback test failure
        cMessageStack::PushMsg( 116, xINT, NULL );
        VoltageFeedbackTestDiagnosis();
        CT_St_T2 = MSG_27_T2;
        break;

      case MSG_27_T2 :              // Report overall cell test pass status
                                    // Set up delay time for ADL
        if ( ( CT_Run.SCT_Done == 1 ) && ( CT_Run.SCT_Fail == 0 ) &&
                ( CT_Run.OCT_Done == 1 ) && ( CT_Run.OCT_Fail == 0 ) &&
                ( CT_Run.VFT_Done == 1 ) && ( CT_Run.VFT_Fail == 0 ) )
        {
            cMessageStack::PushMsg( 199, xINT, NULL );
            cMessageStack::PushMsg( 30, xINT, NULL );
        }
        else
        {
            cMessageStack::PushMsg( 199, xINT, NULL );
            cMessageStack::PushMsg( 31, xINT, NULL );
        }
        CT_Dly_Tm_T2 = IN(SysTm2);
        CT_St_T2 = ENB_6_T2;
        break;

      //---------------------------
      // Task 2 abort enable states
      case ENB_AB_0_T2 :            // Report nature of abort condition
        if ( IN(AutoCelTst) != AUTOCT_OFF )
        {
            CT_Enb_Ab_T2 = ENB_AB_1_T2;
        }
        else
        {
            if ( IN(TstDisable) == true )
            {
                cMessageStack::PushMsg( 103, xINT, NULL );
            }
            else
            {
                cMessageStack::PushMsg( 102, xINT, NULL );
            }
            cMessageStack::PushMsg( 199, xINT, NULL );
            cMessageStack::PushMsg( 32, xINT, NULL );
            CT_Enb_Ab_T2 = ENB_AB_1_T2;
        }
        break;

      case ENB_AB_1_T2 :            // Run Task 1 state machine
        Hdw_Tst_Req = false;
        if ( !Hdw_Tst_ActT2 )
        {
            CT_Enb_Ab_T2 = ENB_AB_2_T2;
        }
        break;

      case ENB_AB_2_T2 :            // Disable bridge electronics
                                    // Set up delay time for ADL
        OUT(CelTstBrgPwrEnb) = false;
        CT_Dly_Tm_T2 = IN(SysTm2);
        CT_Enb_Ab_T2 = ENB_AB_3_T2;
        break;

      case ENB_AB_3_T2 :            // Delay for ADL message retrieval
        if ( IN(AutoCelTst) != AUTOCT_OFF )
        {
            CT_Enb_Ab_T2 = ENB_AB_5_T2;
        }
        else
        {
            if ( ( Delta_Tm( IN(SysTm2), CT_Dly_Tm_T2 ) > CT_MSG_DLY )
                    || ( cMessageStack::MsgPending == false ) )
            {
                CT_Enb_Ab_T2 = ENB_AB_4_T2;
            }
            else
            {
                CT_Enb_Ab_T2 = ENB_AB_3_T2;
            }
        }
        break;

      case ENB_AB_4_T2 :            // Free diagnostic message stack
        if ( cMessageStack::FreeMsgStack() != SUCCESS )
        {
            PUSH_DIAG( CelTstMsgStkTrp );
        }
        CT_Enb_Ab_T2 = ENB_AB_5_T2;
        break;

      case ENB_AB_5_T2 :            // Set active false
        OUT(CelTstAct) = false;
        CT_Enb_Ab_T2   = ENB_AB_0_T2;
        break;

      //----------------------
      // Task 2 disable states
      case DISAB_0_T2 :             // Set active true
        OUT(CelTstAct) = true;
        CT_St_T2       = DISAB_1_T2;
        break;

      case DISAB_1_T2 :             // Run Task 1 state machine
        Hdw_Tst_Req = false;
        if ( !Hdw_Tst_StatT2 )
        {
            CT_St_T2 = DISAB_2_T2;
        }
        break;

      case DISAB_2_T2 :             // Set status false, active false
        OUT(CelTstStat) = false;
        OUT(CelTstAct)  = false;
        CT_St_T2       = IDLE_T2;
        break;

      //----------------------------
      // Task 2 abort disable states
      case DSB_AB_0_T2 :            // Run Task 1 state machine
        Hdw_Tst_Req = false;
        if ( !Hdw_Tst_ActT2 )
        {
            CT_Dsb_Ab_T2 = DSB_AB_1_T2;
        }
        break;

      case DSB_AB_1_T2 :            // Set active false
        OUT(CelTstStat) = false;
        OUT(CelTstAct)  = false;
        CT_St_T2        = IDLE_T2;
        CT_Dsb_Ab_T2    = DSB_AB_0_T2;
      break;

      //------------------------
      // Unrecognized task state
      default:                      // Set active false
        OUT(CelTstAct) = false;
        CT_St_T2       = IDLE_T2;
        break;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function clears the data storage structures used in the
//      cell test.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::CellTestResetData( void )
{

    int Cnt;

        // Reset CT_Run bits

    *(int *)( &CT_Run ) = 0;

        // Reset CT_Data feedback and fault data
        // Reset CT_Eval_Data feedback and fault evaluation data

    for ( Cnt = 0; Cnt < DATA_ARRAY_SIZE; Cnt++ )
    {
        CT_Data[Cnt].Ia_Acc   = 0.0;
        CT_Data[Cnt].Ib_Acc   = 0.0;
        CT_Data[Cnt].Ic_Acc   = 0.0;
        CT_Data[Cnt].Vab      = 0.0;
        CT_Data[Cnt].Vbc      = 0.0;
        CT_Data[Cnt].Vca      = 0.0;
        CT_Data[Cnt].Vdc      = 0.0;
        CT_Data[Cnt].Flt_Code = NO_FLT_CODE;

        CT_Eval_Data[Cnt].Fbk_Lvl.Fbk_Lvl_Int = 0;
        CT_Eval_Data[Cnt].Flt_Code            = NO_FLT_CODE;
    }

        // Calculate VCO quantization threshold
        //   Constants: 2.5 = 2.5 VCO counts
    CT_I_Vco = 2.5 * INS(DelTm1Rcp) * INS(IsensorScl);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function calculates a timer used in setting the PWM command
//      register for the cell test.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      Logical flag indicating vadility of configuration
//
///////////////////////////////////////////////////////////////////////////////
int cCellTest::CellTestTimerCfg( void )
{

    int     Valid_Cfg = true;

    double  I_Part;

    if ( INS(CellTestC) > 0.0 )  // load contains capacitance
    {
        Pulse_Tm  = 2.0F * (float)PI;
        Pulse_Tm *= sqrtf(INS(CellTestC)*INS(CellTestL)) * 0.25F; // estimated time for peak current in a LC network
    }
    else
    {
        if (IN(AutoCelTst) != AUTOCT_OFF)
        {
            Pulse_Tm = INS(CellTestAltL) * INS(CellTestCurLvl) / IN(VdcFbkT3); // estimated time for peak current in a pure L network
        }
        else
        {
            Pulse_Tm = INS(CellTestL) * INS(CellTestCurLvl) / IN(VdcFbkT3); // estimated time for peak current in a pure L network
        }
    }
    Pulse_Tm =  NoNANLim( Pulse_Tm, 1.0e6F, -1.0e6F );

    if (Pulse_Tm < PARM(CelTstMinPls))
    {
        Pulse_Tm = PARM(CelTstMinPls);
    }

    if (( Pulse_Tm > PLS_TM_MIN ) && ( Pulse_Tm < PLS_TM_MAX ))
    {
        Pulse_Mod  = ( Pulse_Tm + INS(DelTimeQuant) ) * INS(DelTm1Rcp);
        Pulse_Frac = modf( Pulse_Mod, &I_Part );
        Pulse_Cnt  = (int)I_Part;
        Pulse_Tmr  = (int)INS(PwmFull) - (int)( Pulse_Frac * (int)(INS(UFull) * INS(PwmQuant)) );
        Pulse_Frac = (float)( (int)INS(PwmFull) - Pulse_Tmr ) / (float)INS(PwmFull);
        Pulse_Tm   = INS(DelTm1) * ( (float)Pulse_Cnt + Pulse_Frac ) - INS(DelTimeQuant);
    }
    else
    {
        Valid_Cfg = false;
    }

    return Valid_Cfg;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function checks whether the current magnitude and accumulated
//      current are below specified zero current thresholds.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      Logical flag indicating if current is zero or not.
//
///////////////////////////////////////////////////////////////////////////////
int cCellTest::CellTestZeroCurrent( void )
{

    int  Zero_Current;

    Zero_Current = true;

        // Hold current magnitude below VCO quantization  level for specified time

    if (( ABS(IN(IaFbk)) < CT_I_Vco ) && ( ABS(IN(IbFbk)) < CT_I_Vco ) && ( ABS(IN(IcFbk)) < CT_I_Vco ))
    {
        if ( Delta_Tm( IN(SysTm1), CT_I_Zero_Tm ) < CT_I_ZERO_DLY )
            Zero_Current = false;
    }
    else
    {
        CT_I_Zero_Tm = IN(SysTm1);
        Zero_Current = false;
    }

        // Check for accumulated current below normalized threshold

    CT_Ia_Acc += IN(IaFbk);
    CT_Ib_Acc += IN(IbFbk);
    CT_Ic_Acc += IN(IcFbk);
    ++CT_I_Acc_Cnt;

    if ( CT_I_Acc_Cnt < CT_I_ZERO_CNT )
    {
        Zero_Current = false;
    }
    else if (( CT_Ia_Acc > CT_I_Vco ) || ( CT_Ib_Acc > CT_I_Vco )
                || ( CT_Ic_Acc > CT_I_Vco ))
    {
        CT_Ia_Acc = 0.0;
        CT_Ib_Acc = 0.0;
        CT_Ic_Acc = 0.0;
        CT_I_Acc_Cnt = 0;
        Zero_Current = false;
    }

    return Zero_Current;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function tests the phase current to determine whether any
//      current was detected during a pulse.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      Logical flag indicating if current was detected or not
//
///////////////////////////////////////////////////////////////////////////////
int cCellTest::CellTestCurrentDetect( void )
{
    int    I_Detect=0;

    float  CT_I_Vco4;

    CT_I_Vco4 = 4.0 * CT_I_Vco;
    if (( ABS(IN(IaFbk)) > CT_I_Vco4 ) || ( ABS(IN(IbFbk)) > CT_I_Vco4 ) || ( ABS(IN(IcFbk)) > CT_I_Vco4 ))
        I_Detect = true;
    else
        I_Detect = false;

    return I_Detect;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function calculates a set of bridge voltage feedbacks and
//      places them in a data storage structure.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::CellTestVoltFbks( void )
{

    CT_Data[CT_Data_Ptr].Vab    = CT_Vab_Intgr / Pulse_Tm;
    CT_Data[CT_Data_Ptr].Vbc    = CT_Vbc_Intgr / Pulse_Tm;
    CT_Data[CT_Data_Ptr].Vca    = -( CT_Data[CT_Data_Ptr].Vab + CT_Data[CT_Data_Ptr].Vbc );

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function calculates a set of bridge current feedbacks and
//      places them in a data storage structure.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::CellTestCurrentFbks( void )
{

    CT_Data[CT_Data_Ptr].Ia_Acc = CT_Ia_Acc;
    CT_Data[CT_Data_Ptr].Ib_Acc = CT_Ib_Acc;
    CT_Data[CT_Data_Ptr].Ic_Acc = CT_Ic_Acc;

    CT_Data[CT_Data_Ptr].I_Acc_Cnt = CT_I_Acc_Cnt;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function reads bridge faults into a data storage structure.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::CellTestGetFlts( void )
{
    int  Flt_Code = 0x0;

    // Check for IOC faults.

    if ( IN(BrgDsatIoc).IocPhsA ) Flt_Code |= IOC_A_CODE;
    if ( IN(BrgDsatIoc).IocPhsB ) Flt_Code |= IOC_B_CODE;
    if ( IN(BrgDsatIoc).IocPhsC ) Flt_Code |= IOC_C_CODE;

    // Check for DSAT faults.

    if ( IN(BrgDsatIoc).DsatSwAU ) Flt_Code |= DSAT_AU_CODE;
    if ( IN(BrgDsatIoc).DsatSwAL ) Flt_Code |= DSAT_AL_CODE;
    if ( IN(BrgDsatIoc).DsatSwBU ) Flt_Code |= DSAT_BU_CODE;
    if ( IN(BrgDsatIoc).DsatSwBL ) Flt_Code |= DSAT_BL_CODE;
    if ( IN(BrgDsatIoc).DsatSwCU ) Flt_Code |= DSAT_CU_CODE;
    if ( IN(BrgDsatIoc).DsatSwCL ) Flt_Code |= DSAT_CL_CODE;

    CT_Data[CT_Data_Ptr].Flt_Code |= Flt_Code;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function compares feedback information in the cell test data
//      storage structure against threshold levels and places the results
//      in an evaluation data structure.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::CellTestEval( void )
{
        // Calculate current detection threshold

    CT_I_Thrsh = CelTstCurThrs * CT_Data[CT_Data_Ptr].I_Acc_Cnt;

    if ( CT_I_Thrsh < CT_I_Vco )
    {
        CT_I_Thrsh = CT_I_Vco;
    }

        // Calculate voltage detection threshold

    CT_V_Thrsh = PARM(CelTstVthrsPct) * CT_Data[CT_Data_Ptr].Vdc;

        // Evaluate Ia feedback

    if ( CT_Data[CT_Data_Ptr].Ia_Acc > CT_I_Thrsh )
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ia = I_POS_LVL;
    }
    else if ( CT_Data[CT_Data_Ptr].Ia_Acc < -CT_I_Thrsh )
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ia = I_NEG_LVL;
    }
    else
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ia = I_ZERO_LVL;
    }

        // Evaluate Ib feedback

    if ( CT_Data[CT_Data_Ptr].Ib_Acc > CT_I_Thrsh )
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ib = I_POS_LVL;
    }
    else if ( CT_Data[CT_Data_Ptr].Ib_Acc < -CT_I_Thrsh )
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ib = I_NEG_LVL;
    }
    else
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ib = I_ZERO_LVL;
    }

        // Evaluate Ic feedback

    if ( CT_Data[CT_Data_Ptr].Ic_Acc > CT_I_Thrsh )
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ic = I_POS_LVL;
    }
    else if ( CT_Data[CT_Data_Ptr].Ic_Acc < -CT_I_Thrsh )
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ic = I_NEG_LVL;
    }
    else
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ic = I_ZERO_LVL;
    }

        // Evaluate Vab feedback

    if (( CT_Data_Ptr == AUBL_PTR ) || ( CT_Data_Ptr == ALBU_PTR ))
    {
        if ( CT_Data[CT_Data_Ptr].Vab > CT_V_Thrsh )
        {
            CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vab = V_POS_LVL;
        }
        else if ( CT_Data[CT_Data_Ptr].Vab < -CT_V_Thrsh )
        {
            CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vab = V_NEG_LVL;
        }
        else
        {
            CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vab = V_ZERO_LVL;
        }
    }
    else
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vab = V_DT_CR_LVL;
    }

        // Evaluate Vbc feedback

    if (( CT_Data_Ptr == BUCL_PTR ) || ( CT_Data_Ptr == BLCU_PTR ))
    {
        if ( CT_Data[CT_Data_Ptr].Vbc > CT_V_Thrsh )
        {
            CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vbc = V_POS_LVL;
        }
        else if ( CT_Data[CT_Data_Ptr].Vbc < -CT_V_Thrsh )
        {
            CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vbc = V_NEG_LVL;
        }
        else
        {
            CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vbc = V_ZERO_LVL;
        }
    }
    else
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vbc = V_DT_CR_LVL;
    }

        // Evaluate Vca feedback

    if (( CT_Data_Ptr == CUAL_PTR ) || ( CT_Data_Ptr == CLAU_PTR ))
    {
        if ( CT_Data[CT_Data_Ptr].Vca > CT_V_Thrsh )
        {
            CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vca = V_POS_LVL;
        }
        else if ( CT_Data[CT_Data_Ptr].Vca < -CT_V_Thrsh )
        {
            CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vca = V_NEG_LVL;
        }
        else
        {
            CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vca = V_ZERO_LVL;
        }
    }
    else
    {
        CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vca = V_DT_CR_LVL;
    }

        // Evaluate fault code

    CT_Eval_Data[CT_Data_Ptr].Flt_Code = CT_Data[CT_Data_Ptr].Flt_Code;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//        This function reports the feedback and fault data collected during
//        the short circuit test or open circuit test for a specified firing
//        pattern.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::SctOctTableMessage( void )
{

    int  Msg_Parm_1, Msg_Parm_2, Msg_Parm_3, Msg_Parm_4;
    int  Msg_Parm_5, Msg_Parm_6, Msg_Parm_7;
    int  CT_Flt_Code;

        // Determine device table entry

    switch ( CT_Data_Ptr )
    {
      case AU_PTR :
        Msg_Parm_1 = MSG_AU_1;
        Msg_Parm_2 = MSG_AU_2;
        break;

      case AL_PTR :
        Msg_Parm_1 = MSG_AL_1;
        Msg_Parm_2 = MSG_AL_2;
        break;

      case BU_PTR :
        Msg_Parm_1 = MSG_BU_1;
        Msg_Parm_2 = MSG_BU_2;
        break;

      case BL_PTR :
        Msg_Parm_1 = MSG_BL_1;
        Msg_Parm_2 = MSG_BL_2;
        break;

      case CU_PTR :
        Msg_Parm_1 = MSG_CU_1;
        Msg_Parm_2 = MSG_CU_2;
        break;

      case CL_PTR :
        Msg_Parm_1 = MSG_CL_1;
        Msg_Parm_2 = MSG_CL_2;
        break;

      case AUBL_PTR :
        Msg_Parm_1 = MSG_AUBL_1;
        Msg_Parm_2 = MSG_AUBL_2;
        break;

      case BUCL_PTR :
        Msg_Parm_1 = MSG_BUCL_1;
        Msg_Parm_2 = MSG_BUCL_2;
        break;

      case CUAL_PTR :
        Msg_Parm_1 = MSG_CUAL_1;
        Msg_Parm_2 = MSG_CUAL_2;
        break;

      case ALBU_PTR :
        Msg_Parm_1 = MSG_ALBU_1;
        Msg_Parm_2 = MSG_ALBU_2;
        break;

      case BLCU_PTR :
        Msg_Parm_1 = MSG_BLCU_1;
        Msg_Parm_2 = MSG_BLCU_2;
        break;

      case CLAU_PTR :
        Msg_Parm_1 = MSG_CLAU_1;
        Msg_Parm_2 = MSG_CLAU_2;
        break;

      default :
        Msg_Parm_1 = MSG_BAD_DEV_1;
        Msg_Parm_2 = MSG_BAD_DEV_2;
        break;
    }               // END switch (CT_Data_Ptr)

    CT_Flt_Code = CT_Eval_Data[CT_Data_Ptr].Flt_Code;

        // Determine feedback and fault table entries

    if ( CT_Flt_Code & NO_GATE_CODE )
    {
            // Indicate that cells were not gated

        Msg_Parm_3 = MSG_SPC;
        Msg_Parm_4 = MSG_SPC;
        Msg_Parm_5 = MSG_SPC;
        Msg_Parm_6 = MSG_NO_GT_1;
        Msg_Parm_7 = MSG_NO_GT_2;

        CT_Flt_Code -= NO_GATE_CODE;
    }
    else
    {
            // Determine Ia feedback table entry

        if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ia == I_POS_LVL )
        {
            Msg_Parm_3 = MSG_I_POS;
        }
        else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ia == I_NEG_LVL )
        {
            Msg_Parm_3 = MSG_I_NEG;
        }
        else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ia == I_ZERO_LVL )
        {
            Msg_Parm_3 = MSG_I_ZERO;
        }
        else
        {
            Msg_Parm_3 = MSG_BAD_FBK;
        }

            // Determine Ib feedback table entry

        if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ib == I_POS_LVL )
        {
            Msg_Parm_4 = MSG_I_POS;
        }
        else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ib == I_NEG_LVL )
        {
            Msg_Parm_4 = MSG_I_NEG;
        }
        else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ib == I_ZERO_LVL )
        {
            Msg_Parm_4 = MSG_I_ZERO;
        }
        else
        {
            Msg_Parm_4 = MSG_BAD_FBK;
        }

            // Determine Ic feedback table entry

        if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ic == I_POS_LVL )
        {
            Msg_Parm_5 = MSG_I_POS;
        }
        else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ic == I_NEG_LVL )
        {
            Msg_Parm_5 = MSG_I_NEG;
        }
        else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Ic == I_ZERO_LVL )
        {
            Msg_Parm_5 = MSG_I_ZERO;
        }
        else
        {
            Msg_Parm_5 = MSG_BAD_FBK;
        }

            // Determine fault detected

        if ( CT_Flt_Code == NO_FLT_CODE )
        {
            Msg_Parm_6 = MSG_NO_FLT_1;
            Msg_Parm_7 = MSG_NO_FLT_2;
        }
        else if ( CT_Flt_Code & DSAT_AU_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AU_1;
            Msg_Parm_7 = MSG_DSAT_AU_2;
            CT_Flt_Code -= DSAT_AU_CODE;
        }
        else if ( CT_Flt_Code & DSAT_AL_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AL_1;
            Msg_Parm_7 = MSG_DSAT_AL_2;
            CT_Flt_Code -= DSAT_AL_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BU_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BU_1;
            Msg_Parm_7 = MSG_DSAT_BU_2;
            CT_Flt_Code -= DSAT_BU_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BL_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BL_1;
            Msg_Parm_7 = MSG_DSAT_BL_2;
            CT_Flt_Code -= DSAT_BL_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CU_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CU_1;
            Msg_Parm_7 = MSG_DSAT_CU_2;
            CT_Flt_Code -= DSAT_CU_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CL_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CL_1;
            Msg_Parm_7 = MSG_DSAT_CL_2;
            CT_Flt_Code -= DSAT_CL_CODE;
        }
        else if ( CT_Flt_Code & IOC_A_CODE )
        {
            Msg_Parm_6 = MSG_IOC_A_1;
            Msg_Parm_7 = MSG_IOC_A_2;
            CT_Flt_Code -= IOC_A_CODE;
        }
        else if ( CT_Flt_Code & IOC_B_CODE )
        {
            Msg_Parm_6 = MSG_IOC_B_1;
            Msg_Parm_7 = MSG_IOC_B_2;
            CT_Flt_Code -= IOC_B_CODE;
        }
        else if ( CT_Flt_Code & IOC_C_CODE )
        {
            Msg_Parm_6 = MSG_IOC_C_1;
            Msg_Parm_7 = MSG_IOC_C_2;
            CT_Flt_Code -= IOC_C_CODE;
        }
        else if ( CT_Flt_Code & I_DECAY_CODE )
        {
            Msg_Parm_6 = MSG_I_DCY_1;
            Msg_Parm_7 = MSG_I_DCY_2;
            CT_Flt_Code -= I_DECAY_CODE;
        }
        else if ( CT_Flt_Code & DSAT_AU1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AU1_1;
            Msg_Parm_7 = MSG_DSAT_AU1_2;
            CT_Flt_Code -= DSAT_AU1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_AL1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AL1_1;
            Msg_Parm_7 = MSG_DSAT_AL1_2;
            CT_Flt_Code -= DSAT_AL1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BU1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BU1_1;
            Msg_Parm_7 = MSG_DSAT_BU1_2;
            CT_Flt_Code -= DSAT_BU1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BL1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BL1_1;
            Msg_Parm_7 = MSG_DSAT_BL1_2;
            CT_Flt_Code -= DSAT_BL1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CU1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CU1_1;
            Msg_Parm_7 = MSG_DSAT_CU1_2;
            CT_Flt_Code -= DSAT_CU1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CL1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CL1_1;
            Msg_Parm_7 = MSG_DSAT_CL1_2;
            CT_Flt_Code -= DSAT_CL1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_AU2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AU2_1;
            Msg_Parm_7 = MSG_DSAT_AU2_2;
            CT_Flt_Code -= DSAT_AU2_CODE;
        }
        else if ( CT_Flt_Code & DSAT_AL2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AL2_1;
            Msg_Parm_7 = MSG_DSAT_AL2_2;
            CT_Flt_Code -= DSAT_AL2_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BU2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BU2_1;
            Msg_Parm_7 = MSG_DSAT_BU2_2;
            CT_Flt_Code -= DSAT_BU2_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BL2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BL2_1;
            Msg_Parm_7 = MSG_DSAT_BL2_2;
            CT_Flt_Code -= DSAT_BL2_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CU2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CU2_1;
            Msg_Parm_7 = MSG_DSAT_CU2_2;
            CT_Flt_Code -= DSAT_CU2_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CL2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CL2_1;
            Msg_Parm_7 = MSG_DSAT_CL2_2;
            CT_Flt_Code -= DSAT_CL2_CODE;
        }
        else if ( CT_Flt_Code & IMBLNC_A_CODE )
        {
            Msg_Parm_6 = MSG_IMBLNC_A_1;
            Msg_Parm_7 = MSG_IMBLNC_A_2;
            CT_Flt_Code -= IMBLNC_A_CODE;
        }
        else if ( CT_Flt_Code & IMBLNC_B_CODE )
        {
            Msg_Parm_6 = MSG_IMBLNC_B_1;
            Msg_Parm_7 = MSG_IMBLNC_B_2;
            CT_Flt_Code -= IMBLNC_B_CODE;
        }
        else if ( CT_Flt_Code & IMBLNC_C_CODE )
        {
            Msg_Parm_6 = MSG_IMBLNC_C_1;
            Msg_Parm_7 = MSG_IMBLNC_C_2;
            CT_Flt_Code -= IMBLNC_C_CODE;
        }
        else
        {
            Msg_Parm_6 = MSG_BAD_FLT_1;
            Msg_Parm_7 = MSG_BAD_FLT_2;
            CT_Flt_Code = NO_FLT_CODE;
        }           // End if (CT_Flt_Code == NO_FLT_CODE)

    }

        // Place table data on message stack

    cMessageStack::PushMsg( 115, xINT, &Msg_Parm_1, &Msg_Parm_2, &Msg_Parm_3, &Msg_Parm_4,
             &Msg_Parm_5, &Msg_Parm_6, &Msg_Parm_7, NULL );

        // Check for additional faults

    while ( CT_Flt_Code != NO_FLT_CODE )
    {
        Msg_Parm_1 = MSG_SPC;
        Msg_Parm_2 = MSG_SPC;
        Msg_Parm_3 = MSG_SPC;
        Msg_Parm_4 = MSG_SPC;
        Msg_Parm_5 = MSG_SPC;

        if ( CT_Flt_Code & DSAT_AU_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AU_1;
            Msg_Parm_7 = MSG_DSAT_AU_2;
            CT_Flt_Code -= DSAT_AU_CODE;
        }
        else if ( CT_Flt_Code & DSAT_AL_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AL_1;
            Msg_Parm_7 = MSG_DSAT_AL_2;
            CT_Flt_Code -= DSAT_AL_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BU_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BU_1;
            Msg_Parm_7 = MSG_DSAT_BU_2;
            CT_Flt_Code -= DSAT_BU_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BL_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BL_1;
            Msg_Parm_7 = MSG_DSAT_BL_2;
            CT_Flt_Code -= DSAT_BL_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CU_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CU_1;
            Msg_Parm_7 = MSG_DSAT_CU_2;
            CT_Flt_Code -= DSAT_CU_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CL_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CL_1;
            Msg_Parm_7 = MSG_DSAT_CL_2;
            CT_Flt_Code -= DSAT_CL_CODE;
        }
        else if ( CT_Flt_Code & IOC_A_CODE )
        {
            Msg_Parm_6 = MSG_IOC_A_1;
            Msg_Parm_7 = MSG_IOC_A_2;
            CT_Flt_Code -= IOC_A_CODE;
        }
        else if ( CT_Flt_Code & IOC_B_CODE )
        {
            Msg_Parm_6 = MSG_IOC_B_1;
            Msg_Parm_7 = MSG_IOC_B_2;
            CT_Flt_Code -= IOC_B_CODE;
        }
        else if ( CT_Flt_Code & IOC_C_CODE )
        {
            Msg_Parm_6 = MSG_IOC_C_1;
            Msg_Parm_7 = MSG_IOC_C_2;
            CT_Flt_Code -= IOC_C_CODE;
        }
        else if ( CT_Flt_Code & I_DECAY_CODE )
        {
            Msg_Parm_6 = MSG_I_DCY_1;
            Msg_Parm_7 = MSG_I_DCY_2;
            CT_Flt_Code -= I_DECAY_CODE;
        }
        else if ( CT_Flt_Code & DSAT_AU1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AU1_1;
            Msg_Parm_7 = MSG_DSAT_AU1_2;
            CT_Flt_Code -= DSAT_AU1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_AL1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AL1_1;
            Msg_Parm_7 = MSG_DSAT_AL1_2;
            CT_Flt_Code -= DSAT_AL1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BU1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BU1_1;
            Msg_Parm_7 = MSG_DSAT_BU1_2;
            CT_Flt_Code -= DSAT_BU1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BL1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BL1_1;
            Msg_Parm_7 = MSG_DSAT_BL1_2;
            CT_Flt_Code -= DSAT_BL1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CU1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CU1_1;
            Msg_Parm_7 = MSG_DSAT_CU1_2;
            CT_Flt_Code -= DSAT_CU1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CL1_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CL1_1;
            Msg_Parm_7 = MSG_DSAT_CL1_2;
            CT_Flt_Code -= DSAT_CL1_CODE;
        }
        else if ( CT_Flt_Code & DSAT_AU2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AU2_1;
            Msg_Parm_7 = MSG_DSAT_AU2_2;
            CT_Flt_Code -= DSAT_AU2_CODE;
        }
        else if ( CT_Flt_Code & DSAT_AL2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_AL2_1;
            Msg_Parm_7 = MSG_DSAT_AL2_2;
            CT_Flt_Code -= DSAT_AL2_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BU2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BU2_1;
            Msg_Parm_7 = MSG_DSAT_BU2_2;
            CT_Flt_Code -= DSAT_BU2_CODE;
        }
        else if ( CT_Flt_Code & DSAT_BL2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_BL2_1;
            Msg_Parm_7 = MSG_DSAT_BL2_2;
            CT_Flt_Code -= DSAT_BL2_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CU2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CU2_1;
            Msg_Parm_7 = MSG_DSAT_CU2_2;
            CT_Flt_Code -= DSAT_CU2_CODE;
        }
        else if ( CT_Flt_Code & DSAT_CL2_CODE )
        {
            Msg_Parm_6 = MSG_DSAT_CL2_1;
            Msg_Parm_7 = MSG_DSAT_CL2_2;
            CT_Flt_Code -= DSAT_CL2_CODE;
        }
        else if ( CT_Flt_Code & IMBLNC_A_CODE )
        {
            Msg_Parm_6 = MSG_IMBLNC_A_1;
            Msg_Parm_7 = MSG_IMBLNC_A_2;
            CT_Flt_Code -= IMBLNC_A_CODE;
        }
        else if ( CT_Flt_Code & IMBLNC_B_CODE )
        {
            Msg_Parm_6 = MSG_IMBLNC_B_1;
            Msg_Parm_7 = MSG_IMBLNC_B_2;
            CT_Flt_Code -= IMBLNC_B_CODE;
        }
        else if ( CT_Flt_Code & IMBLNC_C_CODE )
        {
            Msg_Parm_6 = MSG_IMBLNC_C_1;
            Msg_Parm_7 = MSG_IMBLNC_C_2;
            CT_Flt_Code -= IMBLNC_C_CODE;
        }
        else
        {
            Msg_Parm_6 = MSG_BAD_FLT_1;
            Msg_Parm_7 = MSG_BAD_FLT_2;
            CT_Flt_Code = NO_FLT_CODE;
        }           // End if (CT_Flt_Code & DSAT_AU_CODE)

        cMessageStack::PushMsg( 115, xINT, &Msg_Parm_1, &Msg_Parm_2, &Msg_Parm_3, &Msg_Parm_4,
                 &Msg_Parm_5, &Msg_Parm_6, &Msg_Parm_7, NULL );

    }               // End while (CT_Flt_Code != NO_FLT_CODE)

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//        This function reports the voltage feedbacks collected during the
//        open circuit test for a specified firing pattern.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::VftTableMessage( void )
{

    int  Msg_Parm_1, Msg_Parm_2, Msg_Parm_3, Msg_Parm_4, Msg_Parm_5;

        // Determine device table entry

    switch ( CT_Data_Ptr )
    {
      case AUBL_PTR :
        Msg_Parm_1 = MSG_AUBL_1;
        Msg_Parm_2 = MSG_AUBL_2;
        break;

      case BUCL_PTR :
        Msg_Parm_1 = MSG_BUCL_1;
        Msg_Parm_2 = MSG_BUCL_2;
        break;

      case CUAL_PTR :
        Msg_Parm_1 = MSG_CUAL_1;
        Msg_Parm_2 = MSG_CUAL_2;
        break;

      case ALBU_PTR :
        Msg_Parm_1 = MSG_ALBU_1;
        Msg_Parm_2 = MSG_ALBU_2;
        break;

      case BLCU_PTR :
        Msg_Parm_1 = MSG_BLCU_1;
        Msg_Parm_2 = MSG_BLCU_2;
        break;

      case CLAU_PTR :
        Msg_Parm_1 = MSG_CLAU_1;
        Msg_Parm_2 = MSG_CLAU_2;
        break;

      default :
        Msg_Parm_1 = MSG_BAD_DEV_1;
        Msg_Parm_2 = MSG_BAD_DEV_2;
        break;
    }               // END switch (CT_Data_Ptr)

        // Determine Vab feedback table entry

    if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vab == V_POS_LVL )
        Msg_Parm_3 = MSG_V_POS;
    else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vab == V_NEG_LVL )
        Msg_Parm_3 = MSG_V_NEG;
    else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vab == V_ZERO_LVL )
        Msg_Parm_3 = MSG_V_ZERO;
    else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vab == V_DT_CR_LVL )
        Msg_Parm_3 = MSG_V_DT_CR;
    else
        Msg_Parm_3 = MSG_BAD_FBK;

        // Determine Vbc feedback table entry

    if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vbc == V_POS_LVL )
        Msg_Parm_4 = MSG_V_POS;
    else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vbc == V_NEG_LVL )
        Msg_Parm_4 = MSG_V_NEG;
    else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vbc == V_ZERO_LVL )
        Msg_Parm_4 = MSG_V_ZERO;
    else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vbc == V_DT_CR_LVL )
        Msg_Parm_4 = MSG_V_DT_CR;
    else
        Msg_Parm_4 = MSG_BAD_FBK;

        // Determine Vca feedback table entry

    if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vca == V_POS_LVL )
        Msg_Parm_5 = MSG_V_POS;
    else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vca == V_NEG_LVL )
        Msg_Parm_5 = MSG_V_NEG;
    else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vca == V_ZERO_LVL )
        Msg_Parm_5 = MSG_V_ZERO;
    else if ( CT_Eval_Data[CT_Data_Ptr].Fbk_Lvl.Fbk_Lvl_Bits.Vca == V_DT_CR_LVL )
        Msg_Parm_5 = MSG_V_DT_CR;
    else
        Msg_Parm_5 = MSG_BAD_FBK;

        // Place table data on message stack

    cMessageStack::PushMsg( 115, xINT, &Msg_Parm_1, &Msg_Parm_2, &Msg_Parm_3, &Msg_Parm_4,
             &Msg_Parm_5, NULL );

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//        This function makes a decision about the likely cause of a short
//        circuit test failure based on the feedbacks and fault information
//        and reports the diagnosis.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::ShortCircuitTestDiagnosis( void )
{

    int  AU_Fbk, AL_Fbk, BU_Fbk, BL_Fbk, CU_Fbk, CL_Fbk;
    int  AU_Flt, AL_Flt, BU_Flt, BL_Flt, CU_Flt, CL_Flt;

        // Make copies of feedback and fault evaluation data

    AU_Fbk = CT_Eval_Data[AU_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK;
    AL_Fbk = CT_Eval_Data[AL_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK;
    BU_Fbk = CT_Eval_Data[BU_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK;
    BL_Fbk = CT_Eval_Data[BL_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK;
    CU_Fbk = CT_Eval_Data[CU_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK;
    CL_Fbk = CT_Eval_Data[CL_PTR].Fbk_Lvl.Fbk_Lvl_Int & SCT_FBK_MASK;

    AU_Flt = CT_Eval_Data[AU_PTR].Flt_Code;
    AL_Flt = CT_Eval_Data[AL_PTR].Flt_Code;
    BU_Flt = CT_Eval_Data[BU_PTR].Flt_Code;
    BL_Flt = CT_Eval_Data[BL_PTR].Flt_Code;
    CU_Flt = CT_Eval_Data[CU_PTR].Flt_Code;
    CL_Flt = CT_Eval_Data[CL_PTR].Flt_Code;

        // Common single bridge and parallel bridge failures

            // Ground in motor or cable phase A

    if (( AU_Fbk == AU_SCT_MTR_GND_A ) && ( AU_Flt == NO_FLT_CODE ) &&
        ( AL_Fbk == AL_SCT_MTR_GND_A ) && ( AL_Flt == NO_FLT_CODE ) &&
        ( BU_Fbk == BU_SCT_MTR_GND_A ) && ( BU_Flt == NO_FLT_CODE ) &&
        ( BL_Fbk == BL_SCT_MTR_GND_A ) && ( BL_Flt == NO_FLT_CODE ) &&
        ( CU_Fbk == CU_SCT_MTR_GND_A ) && ( CU_Flt == NO_FLT_CODE ) &&
        ( CL_Fbk == CL_SCT_MTR_GND_A ) && ( CL_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 149, xINT, NULL );
    }

            // Ground in motor or cable phase B

    else if (( AU_Fbk == AU_SCT_MTR_GND_B ) && ( AU_Flt == NO_FLT_CODE ) &&
             ( AL_Fbk == AL_SCT_MTR_GND_B ) && ( AL_Flt == NO_FLT_CODE ) &&
             ( BU_Fbk == BU_SCT_MTR_GND_B ) && ( BU_Flt == NO_FLT_CODE ) &&
             ( BL_Fbk == BL_SCT_MTR_GND_B ) && ( BL_Flt == NO_FLT_CODE ) &&
             ( CU_Fbk == CU_SCT_MTR_GND_B ) && ( CU_Flt == NO_FLT_CODE ) &&
             ( CL_Fbk == CL_SCT_MTR_GND_B ) && ( CL_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 150, xINT, NULL );
    }

            // Ground in motor or cable phase C

    else if (( AU_Fbk == AU_SCT_MTR_GND_C ) && ( AU_Flt == NO_FLT_CODE ) &&
             ( AL_Fbk == AL_SCT_MTR_GND_C ) && ( AL_Flt == NO_FLT_CODE ) &&
             ( BU_Fbk == BU_SCT_MTR_GND_C ) && ( BU_Flt == NO_FLT_CODE ) &&
             ( BL_Fbk == BL_SCT_MTR_GND_C ) && ( BL_Flt == NO_FLT_CODE ) &&
             ( CU_Fbk == CU_SCT_MTR_GND_C ) && ( CU_Flt == NO_FLT_CODE ) &&
             ( CL_Fbk == CL_SCT_MTR_GND_C ) && ( CL_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 151, xINT, NULL );
    }

            // Indeterminate ground in motor or cable

    else if (( AU_Fbk == AU_SCT_MTR_GND ) && ( AU_Flt == NO_FLT_CODE ) &&
             ( AL_Fbk == AL_SCT_MTR_GND ) && ( AL_Flt == NO_FLT_CODE ) &&
             ( BU_Fbk == BU_SCT_MTR_GND ) && ( BU_Flt == NO_FLT_CODE ) &&
             ( BL_Fbk == BL_SCT_MTR_GND ) && ( BL_Flt == NO_FLT_CODE ) &&
             ( CU_Fbk == CU_SCT_MTR_GND ) && ( CU_Flt == NO_FLT_CODE ) &&
             ( CL_Fbk == CL_SCT_MTR_GND ) && ( CL_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 124, xINT, NULL );
    }

            // Unexpected current feedback phase A upper

    else if (((AU_Fbk == AU_SCT_UNEXP) || (AU_Fbk == AL_SCT_UNEXP)) && ( AU_Flt == NO_FLT_CODE ) &&
             ( AL_Fbk == A_SCT_UNEXP ) && ( AL_Flt == NO_FLT_CODE ) &&
             ( BU_Fbk == B_SCT_UNEXP ) && ( BU_Flt == NO_FLT_CODE ) &&
             ( BL_Fbk == B_SCT_UNEXP ) && ( BL_Flt == NO_FLT_CODE ) &&
             ( CU_Fbk == C_SCT_UNEXP ) && ( CU_Flt == NO_FLT_CODE ) &&
             ( CL_Fbk == C_SCT_UNEXP ) && ( CL_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 167, xINT, NULL );
        cMessageStack::PushMsg( 170, xINT, NULL );
    }

            // Unexpected current feedback phase A lower

    else if (( AU_Fbk == A_SCT_UNEXP ) && ( AU_Flt == NO_FLT_CODE ) &&
             ((AL_Fbk == AL_SCT_UNEXP) || (AL_Fbk == AU_SCT_UNEXP)) && ( AL_Flt == NO_FLT_CODE ) &&
             ( BU_Fbk == B_SCT_UNEXP ) && ( BU_Flt == NO_FLT_CODE ) &&
             ( BL_Fbk == B_SCT_UNEXP ) && ( BL_Flt == NO_FLT_CODE ) &&
             ( CU_Fbk == C_SCT_UNEXP ) && ( CU_Flt == NO_FLT_CODE ) &&
             ( CL_Fbk == C_SCT_UNEXP ) && ( CL_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 167, xINT, NULL );
        cMessageStack::PushMsg( 170, xINT, NULL );
    }

            // Unexpected current feedback phase B upper

    else if (( AU_Fbk == A_SCT_UNEXP ) && ( AU_Flt == NO_FLT_CODE ) &&
             ( AL_Fbk == A_SCT_UNEXP ) && ( AL_Flt == NO_FLT_CODE ) &&
             ((BU_Fbk == BU_SCT_UNEXP) || (BU_Fbk == BL_SCT_UNEXP)) && ( BU_Flt == NO_FLT_CODE ) &&
             ( BL_Fbk == B_SCT_UNEXP ) && ( BL_Flt == NO_FLT_CODE ) &&
             ( CU_Fbk == C_SCT_UNEXP ) && ( CU_Flt == NO_FLT_CODE ) &&
             ( CL_Fbk == C_SCT_UNEXP ) && ( CL_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 168, xINT, NULL );
        cMessageStack::PushMsg( 170, xINT, NULL );
    }

            // Unexpected current feedback phase B lower

    else if (( AU_Fbk == A_SCT_UNEXP ) && ( AU_Flt == NO_FLT_CODE ) &&
             ( AL_Fbk == A_SCT_UNEXP ) && ( AL_Flt == NO_FLT_CODE ) &&
             ( BU_Fbk == B_SCT_UNEXP ) && ( BU_Flt == NO_FLT_CODE ) &&
             ((BL_Fbk == BL_SCT_UNEXP) || (BL_Fbk == BU_SCT_UNEXP)) && ( BL_Flt == NO_FLT_CODE ) &&
             ( CU_Fbk == C_SCT_UNEXP ) && ( CU_Flt == NO_FLT_CODE ) &&
             ( CL_Fbk == C_SCT_UNEXP ) && ( CL_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 168, xINT, NULL );
        cMessageStack::PushMsg( 170, xINT, NULL );
    }

            // Unexpected current feedback phase C upper

    else if (( AU_Fbk == A_SCT_UNEXP ) && ( AU_Flt == NO_FLT_CODE ) &&
             ( AL_Fbk == A_SCT_UNEXP ) && ( AL_Flt == NO_FLT_CODE ) &&
             ( BU_Fbk == B_SCT_UNEXP ) && ( BU_Flt == NO_FLT_CODE ) &&
             ( BL_Fbk == B_SCT_UNEXP ) && ( BL_Flt == NO_FLT_CODE ) &&
             ((CU_Fbk == CU_SCT_UNEXP) || (CU_Fbk == CL_SCT_UNEXP)) && ( CU_Flt == NO_FLT_CODE ) &&
             ( CL_Fbk == C_SCT_UNEXP ) && ( CL_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 169, xINT, NULL );
        cMessageStack::PushMsg( 170, xINT, NULL );
    }

    // Unexpected current feedback phase C lower

    else if (( AU_Fbk == A_SCT_UNEXP ) && ( AU_Flt == NO_FLT_CODE ) &&
             ( AL_Fbk == A_SCT_UNEXP ) && ( AL_Flt == NO_FLT_CODE ) &&
             ( BU_Fbk == B_SCT_UNEXP ) && ( BU_Flt == NO_FLT_CODE ) &&
             ( BL_Fbk == B_SCT_UNEXP ) && ( BL_Flt == NO_FLT_CODE ) &&
             ( CU_Fbk == C_SCT_UNEXP ) && ( CU_Flt == NO_FLT_CODE ) &&
             ((CL_Fbk == CL_SCT_UNEXP) || (CL_Fbk == CU_SCT_UNEXP)) && ( CL_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 169, xINT, NULL );
        cMessageStack::PushMsg( 170, xINT, NULL );
    }

        // Single bridge specific failures

    else if ( true )
    {
            // Short in A upper device

        if (( AU_Fbk == AU_SCT_SHORT_AU ) && ( AU_Flt == NO_FLT_CODE )  &&
            ( AL_Fbk == AL_SCT_SHORT_AU ) && ( AL_Flt == DSAT_AL_CODE ) &&
            ( BU_Fbk == BU_SCT_SHORT_AU ) && ( BU_Flt == NO_FLT_CODE )  &&
            ( BL_Fbk == BL_SCT_SHORT_AU ) && ( BL_Flt == NO_FLT_CODE )  &&
            ( CU_Fbk == CU_SCT_SHORT_AU ) && ( CU_Flt == NO_FLT_CODE )  &&
            ( CL_Fbk == CL_SCT_SHORT_AU ) && ( CL_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 118, xINT, NULL );
        }

            // Short in A lower device

        else if (( AU_Fbk == AU_SCT_SHORT_AL ) &&  ( AU_Flt == DSAT_AU_CODE ) &&
                 ( AL_Fbk == AL_SCT_SHORT_AL ) &&  ( AL_Flt == NO_GATE_CODE ) &&
                 ( BU_Fbk == BU_SCT_SHORT_AL ) &&  ( BU_Flt == NO_FLT_CODE )  &&
                 ( BL_Fbk == BL_SCT_SHORT_AL ) &&  ( BL_Flt == NO_FLT_CODE )  &&
                 ( CU_Fbk == CU_SCT_SHORT_AL ) &&  ( CU_Flt == NO_FLT_CODE )  &&
                 ( CL_Fbk == CL_SCT_SHORT_AL ) &&  ( CL_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 119, xINT, NULL );
        }

            // Short in B upper device

        else if (( AU_Fbk == AU_SCT_SHORT_BU ) && ( AU_Flt == NO_FLT_CODE )  &&
                 ( AL_Fbk == AL_SCT_SHORT_BU ) && ( AL_Flt == NO_FLT_CODE )  &&
                 ( BU_Fbk == BU_SCT_SHORT_BU ) && ( BU_Flt == NO_FLT_CODE )  &&
                 ( BL_Fbk == BL_SCT_SHORT_BU ) && ( BL_Flt == DSAT_BL_CODE ) &&
                 ( CU_Fbk == CU_SCT_SHORT_BU ) && ( CU_Flt == NO_FLT_CODE )  &&
                 ( CL_Fbk == CL_SCT_SHORT_BU ) && ( CL_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 120, xINT, NULL );
        }

            // Short in B lower device

        else if (( AU_Fbk == AU_SCT_SHORT_BL ) && ( AU_Flt == NO_FLT_CODE )  &&
                 ( AL_Fbk == AL_SCT_SHORT_BL ) && ( AL_Flt == NO_FLT_CODE )  &&
                 ( BU_Fbk == BU_SCT_SHORT_BL ) && ( BU_Flt == DSAT_BU_CODE ) &&
                 ( BL_Fbk == BL_SCT_SHORT_BL ) && ( BL_Flt == NO_GATE_CODE ) &&
                 ( CU_Fbk == CU_SCT_SHORT_BL ) && ( CU_Flt == NO_FLT_CODE )  &&
                 ( CL_Fbk == CL_SCT_SHORT_BL ) && ( CL_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 121, xINT, NULL );
        }

            // Short in C upper device

        else if (( AU_Fbk == AU_SCT_SHORT_CU ) && ( AU_Flt == NO_FLT_CODE ) &&
                 ( AL_Fbk == AL_SCT_SHORT_CU ) && ( AL_Flt == NO_FLT_CODE ) &&
                 ( BU_Fbk == BU_SCT_SHORT_CU ) && ( BU_Flt == NO_FLT_CODE ) &&
                 ( BL_Fbk == BL_SCT_SHORT_CU ) && ( BL_Flt == NO_FLT_CODE ) &&
                 ( CU_Fbk == CU_SCT_SHORT_CU ) && ( CU_Flt == NO_FLT_CODE ) &&
                 ( CL_Fbk == CL_SCT_SHORT_CU ) && ( CL_Flt == DSAT_CL_CODE ))
        {
            cMessageStack::PushMsg( 122, xINT, NULL );
        }

            // Short in C lower device

        else if (( AU_Fbk == AU_SCT_SHORT_CL ) && ( AU_Flt == NO_FLT_CODE )  &&
                 ( AL_Fbk == AL_SCT_SHORT_CL ) && ( AL_Flt == NO_FLT_CODE )  &&
                 ( BU_Fbk == BU_SCT_SHORT_CL ) && ( BU_Flt == NO_FLT_CODE )  &&
                 ( BL_Fbk == BL_SCT_SHORT_CL ) && ( BL_Flt == NO_FLT_CODE )  &&
                 ( CU_Fbk == CU_SCT_SHORT_CL ) && ( CU_Flt == DSAT_CU_CODE ) &&
                 ( CL_Fbk == CL_SCT_SHORT_CL ) && ( CL_Flt == NO_GATE_CODE ))
        {
            cMessageStack::PushMsg( 123, xINT, NULL );
        }

            // Gating problem in A upper device

        else if (( AU_Fbk == AU_SCT_GATE_AU ) && ( AU_Flt == DSAT_AU_CODE ) &&
                 ( AL_Fbk == AL_SCT_GATE_AU ) && ( AL_Flt == NO_GATE_CODE ) &&
                 ( BU_Fbk == BU_SCT_GATE_AU ) && ( BU_Flt == NO_FLT_CODE )  &&
                 ( BL_Fbk == BL_SCT_GATE_AU ) && ( BL_Flt == NO_FLT_CODE )  &&
                 ( CU_Fbk == CU_SCT_GATE_AU ) && ( CU_Flt == NO_FLT_CODE )  &&
                 ( CL_Fbk == CL_SCT_GATE_AU ) && ( CL_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 143, xINT, NULL );
        }

            // Gating problem in A lower device

        else if (( AU_Fbk == AU_SCT_GATE_AL ) && ( AU_Flt == NO_FLT_CODE )  &&
                 ( AL_Fbk == AL_SCT_GATE_AL ) && ( AL_Flt == DSAT_AL_CODE ) &&
                 ( BU_Fbk == BU_SCT_GATE_AL ) && ( BU_Flt == NO_FLT_CODE )  &&
                 ( BL_Fbk == BL_SCT_GATE_AL ) && ( BL_Flt == NO_FLT_CODE )  &&
                 ( CU_Fbk == CU_SCT_GATE_AL ) && ( CU_Flt == NO_FLT_CODE )  &&
                 ( CL_Fbk == CL_SCT_GATE_AL ) && ( CL_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 144, xINT, NULL );
        }

            // Gating problem in B upper device

        else if (( AU_Fbk == AU_SCT_GATE_BU ) && ( AU_Flt == NO_FLT_CODE )  &&
                 ( AL_Fbk == AL_SCT_GATE_BU ) && ( AL_Flt == NO_FLT_CODE )  &&
                 ( BU_Fbk == BU_SCT_GATE_BU ) && ( BU_Flt == DSAT_BU_CODE ) &&
                 ( BL_Fbk == BL_SCT_GATE_BU ) && ( BL_Flt == NO_GATE_CODE ) &&
                 ( CU_Fbk == CU_SCT_GATE_BU ) && ( CU_Flt == NO_FLT_CODE )  &&
                 ( CL_Fbk == CL_SCT_GATE_BU ) && ( CL_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 145, xINT, NULL );
        }

            // Gating problem in B lower device

        else if (( AU_Fbk == AU_SCT_GATE_BL ) && ( AU_Flt == NO_FLT_CODE )  &&
                 ( AL_Fbk == AL_SCT_GATE_BL ) && ( AL_Flt == NO_FLT_CODE )  &&
                 ( BU_Fbk == BU_SCT_GATE_BL ) && ( BU_Flt == NO_FLT_CODE )  &&
                 ( BL_Fbk == BL_SCT_GATE_BL ) && ( BL_Flt == DSAT_BL_CODE ) &&
                 ( CU_Fbk == CU_SCT_GATE_BL ) && ( CU_Flt == NO_FLT_CODE )  &&
                 ( CL_Fbk == CL_SCT_GATE_BL ) && ( CL_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 146, xINT, NULL );
        }

            // Gating problem in C upper device

        else if (( AU_Fbk == AU_SCT_GATE_CU ) && ( AU_Flt == NO_FLT_CODE )  &&
                 ( AL_Fbk == AL_SCT_GATE_CU ) && ( AL_Flt == NO_FLT_CODE )  &&
                 ( BU_Fbk == BU_SCT_GATE_CU ) && ( BU_Flt == NO_FLT_CODE )  &&
                 ( BL_Fbk == BL_SCT_GATE_CU ) && ( BL_Flt == NO_FLT_CODE )  &&
                 ( CU_Fbk == CU_SCT_GATE_CU ) && ( CU_Flt == DSAT_CU_CODE ) &&
                 ( CL_Fbk == CL_SCT_GATE_CU ) && ( CL_Flt == NO_GATE_CODE ))
        {
            cMessageStack::PushMsg( 147, xINT, NULL );
        }

            // Gating problem in C lower device

        else if (( AU_Fbk == AU_SCT_GATE_AL ) && ( AU_Flt == NO_FLT_CODE ) &&
                 ( AL_Fbk == AL_SCT_GATE_AL ) && ( AL_Flt == NO_FLT_CODE ) &&
                 ( BU_Fbk == BU_SCT_GATE_AL ) && ( BU_Flt == NO_FLT_CODE ) &&
                 ( BL_Fbk == BL_SCT_GATE_AL ) && ( BL_Flt == NO_FLT_CODE ) &&
                 ( CU_Fbk == CU_SCT_GATE_AL ) && ( CU_Flt == NO_FLT_CODE ) &&
                 ( CL_Fbk == CL_SCT_GATE_AL ) && ( CL_Flt == DSAT_CL_CODE ))
        {
            cMessageStack::PushMsg( 148, xINT, NULL );
        }

            // Report message for undiagnosed problem

        else
        {
            cMessageStack::PushMsg( 117, xINT, NULL );
        }
    }

        // Parallel bridge specific failures

    else
    {
            // Report message for undiagnosed problem

        cMessageStack::PushMsg( 152, xINT, NULL );
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//        This function makes a decision about the likely cause of an open
//        circuit test failure based on the feedbacks and fault information
//        and reports the diagnosis.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::OpenCircuitTestDiagnosis( void )
{

    int  AUBL_Fbk, BUCL_Fbk, CUAL_Fbk, ALBU_Fbk, BLCU_Fbk, CLAU_Fbk;
    int  AUBL_Flt, BUCL_Flt, CUAL_Flt, ALBU_Flt, BLCU_Flt, CLAU_Flt;

        // Make copies of feedback and fault evaluation data

    AUBL_Fbk = CT_Eval_Data[AUBL_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;
    ALBU_Fbk = CT_Eval_Data[ALBU_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;
    BUCL_Fbk = CT_Eval_Data[BUCL_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;
    BLCU_Fbk = CT_Eval_Data[BLCU_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;
    CUAL_Fbk = CT_Eval_Data[CUAL_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;
    CLAU_Fbk = CT_Eval_Data[CLAU_PTR].Fbk_Lvl.Fbk_Lvl_Int & OCT_FBK_MASK;

    AUBL_Flt = CT_Eval_Data[AUBL_PTR].Flt_Code;
    ALBU_Flt = CT_Eval_Data[ALBU_PTR].Flt_Code;
    BUCL_Flt = CT_Eval_Data[BUCL_PTR].Flt_Code;
    BLCU_Flt = CT_Eval_Data[BLCU_PTR].Flt_Code;
    CUAL_Flt = CT_Eval_Data[CUAL_PTR].Flt_Code;
    CLAU_Flt = CT_Eval_Data[CLAU_PTR].Flt_Code;

        // Common single bridge and parallel bridge failures

            // Any instantaneous over current

    if (    ( AUBL_Flt & IOC_A_CODE ) | ( AUBL_Flt & IOC_B_CODE ) |
            ( BUCL_Flt & IOC_B_CODE ) | ( BUCL_Flt & IOC_C_CODE ) |
            ( CUAL_Flt & IOC_C_CODE ) | ( CUAL_Flt & IOC_A_CODE ) |
            ( ALBU_Flt & IOC_A_CODE ) | ( ALBU_Flt & IOC_B_CODE ) |
            ( BLCU_Flt & IOC_B_CODE ) | ( BLCU_Flt & IOC_C_CODE ) |
            ( CLAU_Flt & IOC_C_CODE ) | ( CLAU_Flt & IOC_A_CODE )     )
    {
        cMessageStack::PushMsg( 171, xINT, NULL );
    }

            // Load not connected to phase A

    else if (( AUBL_Fbk == AUBL_OCT_LOAD_A ) && ( AUBL_Flt == NO_FLT_CODE ) &&
             ( BUCL_Fbk == BUCL_OCT_LOAD_A ) && ( BUCL_Flt == NO_FLT_CODE ) &&
             ( CUAL_Fbk == CUAL_OCT_LOAD_A ) && ( CUAL_Flt == NO_FLT_CODE ) &&
             ( ALBU_Fbk == ALBU_OCT_LOAD_A ) && ( ALBU_Flt == NO_FLT_CODE ) &&
             ( BLCU_Fbk == BLCU_OCT_LOAD_A ) && ( BLCU_Flt == NO_FLT_CODE ) &&
             ( CLAU_Fbk == CLAU_OCT_LOAD_A ) && ( CLAU_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 138, xINT, NULL );
    }

            // Load not connected to phase B

    else if (( AUBL_Fbk == AUBL_OCT_LOAD_B ) && ( AUBL_Flt == NO_FLT_CODE ) &&
             ( BUCL_Fbk == BUCL_OCT_LOAD_B ) && ( BUCL_Flt == NO_FLT_CODE ) &&
             ( CUAL_Fbk == CUAL_OCT_LOAD_B ) && ( CUAL_Flt == NO_FLT_CODE ) &&
             ( ALBU_Fbk == ALBU_OCT_LOAD_B ) && ( ALBU_Flt == NO_FLT_CODE ) &&
             ( BLCU_Fbk == BLCU_OCT_LOAD_B ) && ( BLCU_Flt == NO_FLT_CODE ) &&
             ( CLAU_Fbk == CLAU_OCT_LOAD_B ) && ( CLAU_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 139, xINT, NULL );
    }

            // Load not connected to phase C

    else if (( AUBL_Fbk == AUBL_OCT_LOAD_C ) && ( AUBL_Flt == NO_FLT_CODE ) &&
             ( BUCL_Fbk == BUCL_OCT_LOAD_C ) && ( BUCL_Flt == NO_FLT_CODE ) &&
             ( CUAL_Fbk == CUAL_OCT_LOAD_C ) && ( CUAL_Flt == NO_FLT_CODE ) &&
             ( ALBU_Fbk == ALBU_OCT_LOAD_C ) && ( ALBU_Flt == NO_FLT_CODE ) &&
             ( BLCU_Fbk == BLCU_OCT_LOAD_C ) && ( BLCU_Flt == NO_FLT_CODE ) &&
             ( CLAU_Fbk == CLAU_OCT_LOAD_C ) && ( CLAU_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 140, xINT, NULL );
    }

            // Load not connected to phases A,B,C

    else if (( AUBL_Fbk == AUBL_OCT_LOAD_ABC ) && ( AUBL_Flt == NO_FLT_CODE ) &&
             ( BUCL_Fbk == BUCL_OCT_LOAD_ABC ) && ( BUCL_Flt == NO_FLT_CODE ) &&
             ( CUAL_Fbk == CUAL_OCT_LOAD_ABC ) && ( CUAL_Flt == NO_FLT_CODE ) &&
             ( ALBU_Fbk == ALBU_OCT_LOAD_ABC ) && ( ALBU_Flt == NO_FLT_CODE ) &&
             ( BLCU_Fbk == BLCU_OCT_LOAD_ABC ) && ( BLCU_Flt == NO_FLT_CODE ) &&
             ( CLAU_Fbk == CLAU_OCT_LOAD_ABC ) && ( CLAU_Flt == NO_FLT_CODE ))
    {
        cMessageStack::PushMsg( 141, xINT, NULL );
    }

        // Single bridge specific failures

//  else if ( PARM(Dual_Brg) == false )
    else if ( true )
    {
            // Open in A upper device

        if (( AUBL_Fbk == AUBL_OCT_OPEN_AU ) && ( AUBL_Flt == NO_FLT_CODE ) &&
            ( BUCL_Fbk == BUCL_OCT_OPEN_AU ) && ( BUCL_Flt == NO_FLT_CODE ) &&
            ( CUAL_Fbk == CUAL_OCT_OPEN_AU ) && ( CUAL_Flt == NO_FLT_CODE ) &&
            ( ALBU_Fbk == ALBU_OCT_OPEN_AU ) && ( ALBU_Flt == NO_FLT_CODE ) &&
            ( BLCU_Fbk == BLCU_OCT_OPEN_AU ) && ( BLCU_Flt == NO_FLT_CODE ) &&
            ( CLAU_Fbk == CLAU_OCT_OPEN_AU ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 125, xINT, NULL );
        }

            // Open in A lower device

        else if (( AUBL_Fbk == AUBL_OCT_OPEN_AL ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_OPEN_AL ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_OPEN_AL ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_OPEN_AL ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_OPEN_AL ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_OPEN_AL ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 126, xINT, NULL );
        }

            // Open in B upper device

        else if (( AUBL_Fbk == AUBL_OCT_OPEN_BU ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_OPEN_BU ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_OPEN_BU ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_OPEN_BU ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_OPEN_BU ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_OPEN_BU ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 127, xINT, NULL );
        }

            // Open in B lower device

        else if (( AUBL_Fbk == AUBL_OCT_OPEN_BL ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_OPEN_BL ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_OPEN_BL ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_OPEN_BL ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_OPEN_BL ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_OPEN_BL ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 128, xINT, NULL );
        }

            // Open in C upper device

        else if (( AUBL_Fbk == AUBL_OCT_OPEN_CU ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_OPEN_CU ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_OPEN_CU ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_OPEN_CU ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_OPEN_CU ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_OPEN_CU ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 129, xINT, NULL );
        }

            // Open in C lower device

        else if (( AUBL_Fbk == AUBL_OCT_OPEN_CL ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_OPEN_CL ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_OPEN_CL ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_OPEN_CL ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_OPEN_CL ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_OPEN_CL ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 130, xINT, NULL );
        }

            // Reversed phase A shunt

        else if (( AUBL_Fbk == AUBL_OCT_SHUNT_A ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_SHUNT_A ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_SHUNT_A ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_SHUNT_A ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_SHUNT_A ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_SHUNT_A ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 131, xINT, NULL );
        }

            // Reversed phase B shunt

        else if (( AUBL_Fbk == AUBL_OCT_SHUNT_B ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_SHUNT_B ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_SHUNT_B ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_SHUNT_B ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_SHUNT_B ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_SHUNT_B ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 132, xINT, NULL );
        }

            // Reversed phase C shunt

        else if (( AUBL_Fbk == AUBL_OCT_SHUNT_C ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_SHUNT_C ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_SHUNT_C ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_SHUNT_C ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_SHUNT_C ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_SHUNT_C ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 133, xINT, NULL );
        }

            // Reversed phase A,B shunt

        else if (( AUBL_Fbk == AUBL_OCT_SHUNT_AB ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_SHUNT_AB ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_SHUNT_AB ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_SHUNT_AB ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_SHUNT_AB ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_SHUNT_AB ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 134, xINT, NULL );
        }

            // Reversed phase B,C shunt

        else if (( AUBL_Fbk == AUBL_OCT_SHUNT_BC ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_SHUNT_BC ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_SHUNT_BC ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_SHUNT_BC ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_SHUNT_BC ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_SHUNT_BC ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 135, xINT, NULL );
        }

            // Reversed phase A,C shunt

        else if (( AUBL_Fbk == AUBL_OCT_SHUNT_AC ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_SHUNT_AC ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_SHUNT_AC ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_SHUNT_AC ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_SHUNT_AC ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_SHUNT_AC ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 136, xINT, NULL );
        }

            // Reversed phase A,B,C shunt

        else if (( AUBL_Fbk == AUBL_OCT_SHUNT_ABC ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_SHUNT_ABC ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_SHUNT_ABC ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_SHUNT_ABC ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_SHUNT_ABC ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_SHUNT_ABC ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 137, xINT, NULL );
        }

            // Bad phase A shunt connection

        else if (( AUBL_Fbk == AUBL_OCT_IFBK_A ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_IFBK_A ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_IFBK_A ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_IFBK_A ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_IFBK_A ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_IFBK_A ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 153, xINT, NULL );
        }

            // Bad phase B shunt connection

        else if (( AUBL_Fbk == AUBL_OCT_IFBK_B ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_IFBK_B ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_IFBK_B ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_IFBK_B ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_IFBK_B ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_IFBK_B ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 154, xINT, NULL );
        }

            // Bad phase C shunt connection

        else if (( AUBL_Fbk == AUBL_OCT_IFBK_C ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_IFBK_C ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_IFBK_C ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_IFBK_C ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_IFBK_C ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_IFBK_C ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 155, xINT, NULL );
        }

            // Bad phase A,B shunt connections

        else if (( AUBL_Fbk == AUBL_OCT_IFBK_AB ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_IFBK_AB ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_IFBK_AB ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_IFBK_AB ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_IFBK_AB ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_IFBK_AB ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 156, xINT, NULL );
        }

            // Bad phase B,C shunt connections

        else if (( AUBL_Fbk == AUBL_OCT_IFBK_BC ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_IFBK_BC ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_IFBK_BC ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_IFBK_BC ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_IFBK_BC ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_IFBK_BC ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 157, xINT, NULL );
        }

            // Bad phase A,C shunt connections

        else if (( AUBL_Fbk == AUBL_OCT_IFBK_AC ) && ( AUBL_Flt == NO_FLT_CODE ) &&
                 ( BUCL_Fbk == BUCL_OCT_IFBK_AC ) && ( BUCL_Flt == NO_FLT_CODE ) &&
                 ( CUAL_Fbk == CUAL_OCT_IFBK_AC ) && ( CUAL_Flt == NO_FLT_CODE ) &&
                 ( ALBU_Fbk == ALBU_OCT_IFBK_AC ) && ( ALBU_Flt == NO_FLT_CODE ) &&
                 ( BLCU_Fbk == BLCU_OCT_IFBK_AC ) && ( BLCU_Flt == NO_FLT_CODE ) &&
                 ( CLAU_Fbk == CLAU_OCT_IFBK_AC ) && ( CLAU_Flt == NO_FLT_CODE ))
        {
            cMessageStack::PushMsg( 158, xINT, NULL );
        }

            // Report message for undiagnosed problem

        else
        {
            cMessageStack::PushMsg( 117, xINT, NULL );
        }
    }

        // Parallel bridge specific failures

    else
    {
            // Report message for undiagnosed problem

        cMessageStack::PushMsg( 152, xINT, NULL );
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//        This function makes a decision about the likely cause of a voltage
//        feedback evaluation failure and reports the diagnosis.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cCellTest::VoltageFeedbackTestDiagnosis( void )
{
        // Problem in voltage feedback circuitry

    cMessageStack::PushMsg( 159, xINT, NULL );

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function communicates causes that prevent cell test from being
//      initiated. Messages are displayed in the Toolbox-Device Response
//      dialog box.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      Status of performed action
//
///////////////////////////////////////////////////////////////////////////////
int cCellTest::CellTestDismissal
(
    // Inputs
    unsigned Action ,   // Two possible actions: (0) initialize message stack and
                        // push messages, and (1) wait until messages are processed
                        // by ADL and release the message stack.
    float CT_DismssTmr  // Delay time for ADL message retrieval.
)
{

    switch ( Action )
    {
      case 0 :
        if ( cMessageStack::InitMsgStack( CT_MSG_STK_SZ ) != SUCCESS )
        {
            PUSH_DIAG( CelTstMsgStkTrp );
            return (FAIL);
        }
        else
        {
            cMessageStack::PushMsg( 165, xINT, NULL );  // Conv is in Not ready state
            cMessageStack::PushMsg( 166, xINT, NULL );  // Possible causes

            cMessageStack::PushMsg( 199, xINT, NULL );
            cMessageStack::PushMsg(  31, xINT, NULL );
        }
        break;

      case 1 :
        if ( (CT_DismssTmr > CT_MSG_DLY ) ||  !cMessageStack::MsgPending )
        {
            if ( cMessageStack::FreeMsgStack() != SUCCESS )
            {
                PUSH_DIAG( CelTstMsgStkTrp );
                return (FAIL);
            }
        }
        else
        {
            return (BUSY);
        }
        break;
    }

    return (SUCCESS);
}
