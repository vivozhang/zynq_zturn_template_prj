///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Variable interface
//
// DESCRIPTION:
//              provide interface to published variables for toolbox communication
//              and pointer reconciliation
//
// COPYRIGHT:   Copyright (c) 2002
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files:
//---------------
// system
#include <stdlib.h>
#include <string.h>
// core
// framework
#include "x0FrameWork.h"
#include "x0Err.h"
#include "x0Diagnostic.h"
#include "x0Variable.h"



// Constants:
//-----------


// Externals:
//-----------
extern cMarkStatDiag    MarkStatDiag;


// Types and Classes:
//-------------------


// Function Prototypes:
//---------------------


// Variables:
//-----------
int             DummyVar;
static bool     VarErrorLogged;
CREATE_PUBVAR(VarLogAddrErrCnt, unsigned);
CREATE_PUBVAR(VarLogAddrErr, unsigned);


// Parameters:
//------------


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cVariableBase::cVariableBase()
{

    mVarTblSize = 0;
    mVarAddrTbl = NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cVariableBase::~cVariableBase()
{
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      check for NULL pointers in variable address table
//
///////////////////////////////////////////////////////////////////////////////
void cVariableBase::CreateTables()
{
    int     Idx, NullPtrFound;

    // look for NULL pointers
    NullPtrFound = false;
    for (Idx = 0; Idx < (int)(mVarTblSize); ++Idx)
    {
        if (mVarAddrTbl[Idx].Addr == NULL)
        {
            mVarAddrTbl[Idx].Addr     = &DummyVar;
            mVarAddrTbl[Idx].DataType = nsVariable::INT;
            NullPtrFound              = true;
        }
    }

    if (NullPtrFound)
    {
        MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_VAR_STR,
            cParamString("NULL pointer found in Variable address cross reference table").c_str()),
            HERE);
    }

}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void* cVariableBase::GetAddress(unsigned Laddr)
{
    if (Laddr < mVarTblSize)
        return mVarAddrTbl[Laddr].Addr;

    if (!VarErrorLogged)
    {
        VarErrorLogged = true;
        MarkStatDiag.LogErrorMessage(ALARM_MSG, cParamString(MKS_VAR_STR,
            cParamString("GetAddress request for invalid variable logical address %d (0x%X)\n", Laddr, Laddr).c_str()),
            HERE);
    }
    ++VarLogAddrErrCnt;
    VarLogAddrErr = Laddr;
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
unsigned cVariableBase::GetDataType(unsigned Laddr)
{
    if (Laddr < mVarTblSize)
        return mVarAddrTbl[Laddr].DataType;

    // printf("GetDataType(): Bad logical address %d (0x%X)\n", Laddr, Laddr);
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// determine the variable datatype and return the enumerated result
//
// pAddr - variable logical address index
//
// return - integer representing the variable type
//          or LA_INVLD_TYPE (-1) if the variable type cannot be determined
//
///////////////////////////////////////////////////////////////////////////////
int cVariableBase::ReadType(void* Addr)
{
    // check for logical address
    if (GetAddress((unsigned)Addr) == NULL)
        return LA_INVLD_TYPE;

    switch (GetDataType((unsigned)Addr))
    {
      case nsVariable::BLN:
        return (LA_BOOL_TYPE);
      case nsVariable::INT:
      case nsVariable::UINT:
        return (LA_INT_TYPE);
      case nsVariable::FPT:
        return (LA_FLOAT_TYPE);
      case nsVariable::MEM:
        return (LA_MEM_TYPE);
    }

    return LA_INVLD_TYPE;
}


///////////////////////////////////////////////////////////////////////////////
//
// return the least significant bit of the variable
//
// pAddr - variable logical address index
//
// return - value of the input pointer
//          or 0 if the variable type cannot be determined
//
///////////////////////////////////////////////////////////////////////////////
unsigned cVariableBase::ReadAsBool(void* Addr)
{
    unsigned TempUInt;

    // check for logical address
    if (GetAddress((unsigned)Addr) == NULL)
        return 0;

    TempUInt = *((unsigned*)GetAddress((unsigned)Addr));

    switch (GetDataType((unsigned)Addr))
    {
      case nsVariable::BLN:
      case nsVariable::INT:
      case nsVariable::UINT:
      case nsVariable::MEM:
        return(TempUInt&1);
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// return the integer value of the variable, float values are converted
//
// pAddr - variable logical address index
//
// return - value of the input pointer
//          or 0 if the variable type cannot be determined
//
///////////////////////////////////////////////////////////////////////////////
int cVariableBase::ReadAsInt(void* Addr)
{
    float   TempFloat;

    // check for logical address
    if (GetAddress((unsigned)Addr) == NULL)
        return 0;

    switch (GetDataType((unsigned)Addr))
    {
      case nsVariable::INT:
      case nsVariable::UINT:
      case nsVariable::MEM:
        return *((int*)GetAddress((unsigned)Addr));
      case nsVariable::FPT:
        TempFloat = *((float*)GetAddress((unsigned)Addr));
        return (int)TempFloat;
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// return the float value of the variable, integer values are converted
//
// pAddr - variable logical address index
//
// return - value of the input pointer
//          or 0 if the variable type cannot be determined
//
///////////////////////////////////////////////////////////////////////////////
float cVariableBase::ReadAsFloat(void* Addr)
{
    int TempInt;

    // check for logical address
    if (GetAddress((unsigned)Addr) == NULL)
        return 0;

    switch (GetDataType((unsigned)Addr))
    {
      case nsVariable::INT:
      case nsVariable::UINT:
      case nsVariable::MEM:
        TempInt = *((int*)GetAddress((unsigned)Addr));
        return (float)TempInt;
      case nsVariable::FPT:
        return *((float*)GetAddress((unsigned)Addr));
    }

    return 0;
}


