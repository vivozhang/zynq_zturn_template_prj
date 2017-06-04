///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Support for main control sequencer
//
// DESCRIPTION:
//              This file contains the definitions for the  main control
//              sequencer and product support for ADL server.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              p0Seq.vsd, p0Seq.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _P0SSEQ_H_
#define _P0SSEQ_H_

// Include Files:
//---------------
// System
// Core
// Framework
#include "x0Sequencer.h"
// Product


// Constants:
//-----------
#define    Hard_Reset  EpldCmdP->H_Reset
#define STOPPED_COMMON      0
#define CONV_RUN_REQ_LINE   1
#define CONV_RUNNING_LINE   2
#define CONV_RUN_REQ_ROTOR  3
#define CONV_RUNNING_ROTOR  4
#define CONV_STOP_REQ_ROTOR 5
#define CONV_STOP_REQ_LINE  6
#define INIT_COMMON        -1

// enumerations for PtBrgSel
#define PT_ROTOR_BRG       (0)
#define PT_LINE_BRG        (1)
#define PT_DB              (2)
#define PT_DC_CHOP         (3)

// enumerations for CtBrgSel
#define CT_LINE_BRG        (0)
#define CT_ROTOR_BRG1      (1)
#define CT_DB              (2)


// Function Prototypes:
//---------------------


// Types and Classes:
//-------------------

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This class supports product ADL server commands
//
///////////////////////////////////////////////////////////////////////////////
class cCoreCommonAdlSeq : public cSequencerBase
{
//-----------------------------------------------------------------------------
public:

    // Methods
    //--------
    int  AdlCmdServer(int Command, int ParmCount, unsigned* ParmBuf);

};

#endif  // _P0SSEQ_H_
