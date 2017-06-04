///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Flash File Server Class
//
// DESCRIPTION:
//      This file contains the functions for the Flash File Server class
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
#include <unistd.h>
#include <stdio.h>
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"


// Constants
//----------


// Externals
//----------
#if 0
extern cBridgeCircList  CLA;
extern cBridgeCircList  CLB;
#endif

#if 0
extern cNatcSerialLink  NatcLnkA;
extern cNatcSerialLink  NatcLnkB;
#endif


// Types and Classes
//------------------


// Internal Function Prototypes
//-----------------------------


// Variables
//----------


// Member Functions
//-----------------




///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Product specific initialization
//
// EXECUTION LEVEL
//      Main, Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void* FlashFileSvr( void* arg )
{

    //-------------------------------------------------------------------------
    // loop forever waiting for a new file to write
    while (1)
    {
        sleep(1);

#if 0
        CLA.Reconstruct();
        CLA.WriteDataFile(err);
        CLB.Reconstruct();
        CLB.WriteDataFile(err);
#endif

#if 0
        NatcLnkA.WriteDataFile(err);
        NatcLnkB.WriteDataFile(err);
#endif
    }

    // should never get here ...
    printf("Flash File Server thread terminated\n");
    return 0;
}



