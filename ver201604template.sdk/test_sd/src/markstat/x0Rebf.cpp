///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Initialization and processing of REBF discrete I/O
//
// DESCRIPTION
//      This file contains functions for initialization, filtering, state
//      mirroring, and mapping of discrete signals pertaining the Renewable
//      Energy Bridge Interface card (REBF).
//
// COPYRIGHT    Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"


// Constants
//----------


// Externals
//----------


// Types and Classes
//------------------


// Diagnostics
//------------


// Parameters
//-----------

//-----------------------------------------------------------------------
// Line Rebf discrete configurable I/O
CREATE_PARM(L_RebfLvK1Adr,    unsigned *);       // Line REBF low-voltage K1 test-mode pointer
CREATE_PARM(L_RebfLvK2Adr,    unsigned *);       // Line REBF low-voltage K2 test-mode pointer
CREATE_PARM(L_RebfLvK3Adr,    unsigned *);       // Line REBF low-voltage K3 test-mode pointer


// Variables
//----------

//-----------------------------------------------------------------------
// Line Rebf discrete I/O
CREATE_PUBVAR(L_RebfLvK1Out,  unsigned) = false; // Line REBF low-voltage K1 final output
CREATE_PUBVAR(L_RebfLvK2Out,  unsigned) = false; // Line REBF low-voltage K2 final output
CREATE_PUBVAR(L_RebfLvK3Out,  unsigned) = false; // Line REBF low-voltage K3 final output

CREATE_PUBVAR(L_RebfLvK1OutT3, unsigned) =false; // Task3 version of Rebf K1 final output
CREATE_PUBVAR(L_RebfLvK2OutT3, unsigned) =false; // Task3 version of Rebf K2 final output
CREATE_PUBVAR(L_RebfLvK3OutT3, unsigned) =false; // Task3 version of Rebf K3 final output

CREATE_PUBVAR(L_RebfLvK1St,   unsigned) = false;  // Line Rebf mirrored K1 status
CREATE_PUBVAR(L_RebfLvK2St,   unsigned) = false;  // Line Rebf mirrored K2 status
CREATE_PUBVAR(L_RebfLvK3St,   unsigned) = false;  // Line Rebf mirrored K3 status



// Data Passing
//--------------
DATA_PASS(L_RebfLvK1Out,   L_RebfLvK1OutT3,  T2_T3, UNFILTERED);
DATA_PASS(L_RebfLvK2Out,   L_RebfLvK2OutT3,  T2_T3, UNFILTERED);
DATA_PASS(L_RebfLvK3Out,   L_RebfLvK3OutT3,  T2_T3, UNFILTERED);




// Local Prototypes (to resolve forward references)
//-------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Process Rebf relay mapping.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  ProcessRelaysRebf()
{
    //-----------------------------------------------------------------------
    // Line Rebf

    if ((int)PARM(L_RebfLvK1Adr) != LADR_UnusedUInt)
    {
        L_RebfLvK1Out = gVariable.ReadAsBool(PARM(L_RebfLvK1Adr));
    }
    else
    {
        L_RebfLvK1Out = false;
    }

    if ((int)PARM(L_RebfLvK2Adr) != LADR_UnusedUInt)
    {
        L_RebfLvK2Out = gVariable.ReadAsBool(PARM(L_RebfLvK2Adr));
    }
    else
    {
        L_RebfLvK2Out = false;
    }

    if ((int)PARM(L_RebfLvK3Adr) != LADR_UnusedUInt)
    {
        L_RebfLvK3Out = gVariable.ReadAsBool(PARM(L_RebfLvK3Adr));
    }
    else
    {
        L_RebfLvK3Out = false;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Mirror the expected states of Rebf relays
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  MirrorRebfStatus()
{
    // Line Rebf
    L_RebfLvK1St = L_RebfLvK1OutT3;
    L_RebfLvK2St = L_RebfLvK2OutT3;
    L_RebfLvK3St = L_RebfLvK3OutT3;


    return;
}
