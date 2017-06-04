///////////////////////////////////////////////////////////////////////////////
//
// TITLE:
//
// DESCRIPTION:
//      Interprocess Communication
//
// COPYRIGHT:   Copyright (c) 2004
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      none
//
///////////////////////////////////////////////////////////////////////////////
// REVISION HISTORY:
// $Log: /Runtime/HSEM/Branches/CurrentProductVersion/Src/hsem/p0ipc.c30 $
//
// 1     6/22/09 3:58p Nowakjm
// First pass of application (blockware) interface
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"

//-----------------------------------------------------------------------------
// IPC interface signals
//-----------------------------------------------------------------------------

// TO Aux Control

// FROM Aux Control

// Parameters
//-----------
CREATE_PARM(Net_Type,       unsigned);     // Communication interface type

//-----------------------------------------------------------------------------
// public vars
//-----------------------------------------------------------------------------
CREATE_PUBVAR(ApplInterfaceOk, unsigned);
CREATE_PUBVAR(ApplFltRstReq, unsigned);


//-----------------------------------------------------------------------------
// external vars
//-----------------------------------------------------------------------------





/******************************************************************************
 * Function:        InputFromApplication
 * Execution Level: Task 2
 *
 * Abstract: Translates signals FROM the Application control
 *
 *******************************************************************************/
void InputFromApplication()
{
}



/******************************************************************************
 * Function:        OutputToApplication
 * Execution Level: Task 2
 *
 * Abstract: Translates signals TO the Application control
 *
 *******************************************************************************/
void OutputToApplication()
{
}
