///////////////////////////////////////////////////////////////////////////////
//
// TITLE:    Product application I/O
//
// DESCRIPTION:
//      This file contains functions associated with assigning application
//      inputs and outputs to application specific I/O.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     z0ParmVar.xls
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "p0Assign.h"
#include "b0BridgeRebf.h"


// Constants
//----------


// Externals
//----------
extern unsigned      L_AeadDin10;
extern unsigned      L_AeadDin3;
extern unsigned      L_AeadDin4;
extern unsigned      L_AeadDin5;
extern unsigned      L_AeadDin6;
extern unsigned      L_AeadDin9;
extern unsigned      R_AeadDin9;
extern unsigned      R_AeadDin3;
extern unsigned      R_AeadDin4;
extern unsigned      KdfStatus;
extern unsigned      KdfClsFbk;
extern unsigned      KdfClsCmd;
extern unsigned      DsimMode;
extern unsigned      SimMode;
extern unsigned      HdwTstMode;
extern unsigned      Q1TripFbk;
extern SeqStatBits   L_SeqStat;      // Sequencer Task Status bits
extern unsigned      R_SeqStat_PackedBits;

extern cBridgeRebf   R_BridgeRebf;


// Types and Classes
//------------------


// Diagnostics
//------------


// Parameters
//-----------
CREATE_PARM(CntrlOnOvr,         unsigned);  // Override real control-on signals in sim mode
CREATE_PARM(PLCRebootAdr,       unsigned*); // Add PLC reboot  pointer

// Variables
//----------
CREATE_PUBVAR(PLCRbtCmdf,     float);
CREATE_PUBVAR(L_SftyChnOpn,   unsigned);
CREATE_PUBVAR(PLCRbtCmd,      unsigned);
CREATE_PUBVAR(R_SftyChnOpn,   unsigned);
CREATE_PUBVAR(R_SyncSwHwSt,   unsigned);
CREATE_PUBVAR(SftyChnOpn,     unsigned);
CREATE_PUBVAR(Ps24vOk,        unsigned);    // PS 24Vdc good indicator
CREATE_PUBVAR(FastBlkReqHw,   unsigned);    // Fast Blocking input



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Assigns generic digital inputs to application specific
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void AssignDigitalInputs()
{
    // Safety chain contact inputs, a.k.a Control-On
    if ( (PARM(CntrlOnOvr) == true) && SimMode )
    {
        L_SftyChnOpn = false;
        R_SftyChnOpn = false;
    }
    else
    {
        L_SftyChnOpn = !L_AeadDin9;  // Bridge interface provides Din10 as a copy of Din9 for
        R_SftyChnOpn = !L_AeadDin10;   // 690-AEAD as well as 6K-AEAD.
    }

    SftyChnOpn    = (L_SftyChnOpn || R_SftyChnOpn);


    if (!SimMode)
    {
        KdfClsFbk    =  L_AeadDin3;
        Q1TripFbk    =  L_AeadDin4;
        FastBlkReqHw = !L_AeadDin5;
        Ps24vOk      =  L_AeadDin6;
    }
	else
	{
	
		if (DsimMode)
			{
				KdfClsFbk=KdfClsCmd;
				Q1TripFbk    = false;
	    		FastBlkReqHw = false;
	    		Ps24vOk      = true;
			}
		
    }


    // Populate PLCRbtCmd from pointer if enabled
    if ((int)PARM(PLCRebootAdr) != LADR_UnusedBool)
    {
       PLCRbtCmd  = gVariable.ReadAsBool(PARM(PLCRebootAdr));

    }
    PLCRbtCmdf    = (float)PLCRbtCmd;

        KdfStatus = KdfClsFbk;
}
