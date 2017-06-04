///////////////////////////////////////////////////////////////////////////////
//
// TITLE        <class title>
//
// DESCRIPTION
//      This file contains the definition for the class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __S0_SIM_H__
#define __S0_SIM_H__


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "b0BridgeRebf.h"
#include "b0DbThrm.h"
#include "b0BrgThrm5p.h"

// Constants
//----------


// Function Prototypes
//--------------------


// Types and Classes
//------------------
enum LineSimStateTyp
{
    LSIM_IDLE           = 0,
    LSIM_PRE_CHARGE     = 1,
    LSIM_CONNECTED      = 2,
    LSIM_CONNECT_DF     = 3,
    LSIM_RUNNING        = 4
};

enum RotorSimStateTyp
{
    RSIM_IDLE           = 0,
    RSIM_CONNECTED      = 1,
    RSIM_RUNNING        = 2
};

#include "s0Sim.inc"                    // file shared with external simulator



#endif  // __S0_SIM_H__



