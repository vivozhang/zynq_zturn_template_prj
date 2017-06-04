///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rotor Side Product Level Task Drivers
//
// DESCRIPTION:
//      This file contain common application I/O functions pertaining to
//      Alternative Energy Application D card (AEAD).
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __X0AEAD_H__
#define __X0AEAD_H__

// Include Files
//--------------
// system
// core
// framework
// product

// Constants
//----------
#define  HIFI_MASK     0x0ffff      // Hifi Vco scaling
#define  HIFI_VCO_SCL  1.0e6F
#define  HIFI_VCO_ZER  1.0e6F

#define  DIN_LOW_LVL   0.36788F     // Digital In low hysteresis turn off level
#define  DIN_HI_LVL    0.63212F     // Digital In hi hysteresis turn on level
#define  RLY_LOW_LVL   0.36788F     // Relay Stat In low hysteresis turn off level
#define  RLY_HI_LVL    0.63212F     // Relay Stat In hi hysteresis turn on level
#define  LOC_LOW_LVL   0.36788F     // Local Flt String low hysteresis turn off level
#define  LOC_HI_LVL    0.63212F     // Local Flt String hi hysteresis turn on level
#define  SYS_LOW_LVL   0.36788F     // System Flt String low hysteresis turn off level
#define  SYS_HI_LVL    0.63212F     // System Flt String hi hysteresis turn on level
#define  MA_LOW_LVL    0.36788F     // System Flt String low hysteresis turn off level
#define  MA_HI_LVL     0.63212F     // System Flt String hi hysteresis turn on level

// Function Prototypes
//--------------------

// Types and Classes
//------------------

#endif   // __X0AEAD_H__
