///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Support for rotor control sequencer
//
// DESCRIPTION:
//              This file contains the definitions for the  rotor control
//              sequencer and product support for ADL server.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              p1Sseq.vsd, p1Sseq.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _P1SSEQ_H_
#define _P1SSEQ_H_

// Include Files:
//---------------
// System
// Core
// Framework
// Product


// Constants:
//-----------

// Rotor Core drive sequencer states
#define ROTOR_INIT                 -1
#define ROTOR_STOPPED               0
#define ROTOR_AUTO_CELL_TEST        1
#define ROTOR_DB_RES_TEST           2
#define ROTOR_CONTACTOR_PICKUP      3
#define ROTOR_CONTACTOR_VERIFY      4
#define ROTOR_SYNC_STATOR           5
#define ROTOR_CLOSE_SYNC_SW         6
#define ROTOR_ENABLE_TORQUE         7
#define ROTOR_RUNNING               8
#define ROTOR_DISABLE_TORQUE        9
#define ROTOR_OPEN_SYNC_SW          10
#define ROTOR_DISABLE_ALL           11
#define ROTOR_DIAGNOSTIC            12
#define ROTOR_PULSE_TEST            13
#define ROTOR_CELL_TEST             14
#define ROTOR_MTR_CTL_TUNE          15
// For DFIG2.3, keep DB definition in rotor, but not use.
#define ROTOR_DB_PULSE_TEST         16
#define ROTOR_DB_CELL_TEST          17

#define ROTOR_ZVRT_STANDBY          18
#define ROTOR_CH_PULSE_TEST         19


// Function Prototypes:
//---------------------


// Types and Classes:
//-------------------


#endif  // _P1SSEQ_H_
