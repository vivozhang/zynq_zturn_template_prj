///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Support for line (source) control sequencer task module
//
// DESCRIPTION:
//              This file contains the definitions for the line control
//              sequencer task modules.
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              s0SeqTsk.vsd, s0SeqTsk.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef S0_SEQTSK_H
#define S0_SEQTSK_H

// Include Files:
//---------------
// System
// Core
// Framework
// Product


// Constants:
//-----------

//Distortion-filter-current-status modes
#define DF_STANDBY               0
#define DF_CAPTURE               1
#define DF_READY                 2
#define DF_CALCULATE             3

#define DFMAXTESTNUMBER          3


// Function Prototypes:
//---------------------


// Types and Classes:
//-------------------

// Distortion filter struncture
struct TwoStageTyp
{
    float       Stage1;
    float       Stage2;
};

struct DistFilPhSta
{
    unsigned    IsZero;     // There is not adequate filter reactive pwr variation
    unsigned    IsBad;      // Distortion filter reactive pwr is out of spec
    float       QPwrAux;    // Reactive power measured before closing KDF (termed Auxiliary)
    float       QPwr;       // Reactive power measured after closing KDF
};


#endif      // S0_SEQTSK_H
