///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Parameter interface
//
// DESCRIPTION:
//      Provide interface to published parameters for toolbox communication,
//      CRC checking, parameter file updating, final value interface
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      List locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files:
//---------------
// system
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
// core
// framework
#include "x0FrameWork.h"
#include "x0Err.h"
#include "x0Diagnostic.h"
#include "x0Parameter.h"
#include "x0Rule.h"



// Constants:
//-----------


// Externals:
//-----------
extern cMarkStatDiag            MarkStatDiag;


// Types and Classes:
//-------------------


// Function Prototypes:
//---------------------
extern int      InitialRules(void);
extern int      RunRuleSet(int Rule_Set);


// Variables:
//-----------
int             DummyParm;
static bool     ParmErrorLogged;
CREATE_PUBVAR(ParmLogAddrErrCnt, unsigned);
CREATE_PUBVAR(ParmLogAddrErr, unsigned);
CREATE_PUBVAR(FirmwareRev, unsigned);   // product firmware revision for display purposes (enumerated var)


// Parameters:
//------------


// Diagnostics:
//------------


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cParameterBase::cParameterBase()
{
    ParmTblSize     = 0;
    ParmAddrTbl     = NULL;
    OverrideTblSize = 0;
    pFvOverrideTbl  = NULL;
    pFvTbl          = NULL;
    OverridePresent = false;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cParameterBase::~cParameterBase()
{
    if (pFvOverrideTbl != NULL)
    {
        free(pFvOverrideTbl);
        pFvOverrideTbl = NULL;
    }

    if (pFvTbl != NULL)
    {
        free(pFvTbl);
        pFvTbl = NULL;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void cParameterBase::Initialize()
{
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      check for NULL pointers in parameter address table
//      create override and final value tables
//
///////////////////////////////////////////////////////////////////////////////
void cParameterBase::CreateTables()
{
    int     Idx, NullPtrFound;

    // look for NULL pointers
    // find table index for final value override table
    NullPtrFound = false;
    for (Idx = 0; Idx < (int)(ParmTblSize); ++Idx)
    {
        if (ParmAddrTbl[Idx] == NULL)
        {
            ParmAddrTbl[Idx] = &DummyParm;
            NullPtrFound     = true;
        }
    }

    if (NullPtrFound)
    {
        MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_PARM_STR,
            cParamString("NULL pointer found in Parameter address cross reference table").c_str()),
            HERE);
    }

    OverridePresent = false;
    OverrideTblSize = ((ParmTblSize-1) >> 5) + 1;
    pFvOverrideTbl  = (uint32_t*)malloc(OverrideTblSize*sizeof(int32_t));
    memset(pFvOverrideTbl, 0, OverrideTblSize*sizeof(uint32_t));
    pFvTbl          = (uint32_t*)malloc(OverrideTblSize*sizeof(int32_t));
    memset(pFvTbl, 0, OverrideTblSize*sizeof(uint32_t));
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      change a single parameter value, then save to file
//
// EXECUTION LEVEL
//      ADL thread
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cParameterBase::SetValue (
        void*    pAddr,                 // parameter address or index
        uint32_t Value                  // value to set parameter
        )
{
    unsigned    ParmIdx;        // index into parameter table
    int         Status=SUCCESS;

    // check for logical address
    if ((unsigned)pAddr < (ParmTblSize+OverrideTblSize))
        ParmIdx = (unsigned)pAddr;
    // absolute address must be in parameter table
    else
    {
        for (ParmIdx=0; ParmIdx < ParmTblSize; ++ParmIdx)
        {
            if (ParmAddrTbl[ParmIdx] == pAddr)
                break;
        }
        if (ParmIdx >= ParmTblSize)
            return FAIL;
    }

    // set parameter or final value to passed value
    if (ParmIdx >= ParmTblSize)
        pFvOverrideTbl[ParmIdx-ParmTblSize] = Value;
    else
    {
        *(uint32_t*)ParmAddrTbl[ParmIdx] = Value;
        if (IsFv((void*)ParmIdx))
            Status = FvSetOverride((void*)ParmIdx);
    }

    // save a single parameter to file
    if ((Status == SUCCESS) && (ParmIdx < ParmTblSize))
        Status = SaveSingleToFile((void*)ParmIdx);

    return Status;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      get a single parameter value
//
// EXECUTION LEVEL
//      ADL thread
//
// RETURN VALUE
//      value of parameter
//
///////////////////////////////////////////////////////////////////////////////
uint32_t cParameterBase::GetValue (
        void* pAddr                     // parameter address or index
        )
{
    unsigned    ParmIdx;        // index into parameter table

    // check for logical address
    if ((unsigned)pAddr < (ParmTblSize+OverrideTblSize))
        ParmIdx = (unsigned)pAddr;
    // absolute address must be in parameter table
    else
    {
        for (ParmIdx=0; ParmIdx < ParmTblSize; ++ParmIdx)
        {
            if (ParmAddrTbl[ParmIdx] == pAddr)
                break;
        }
        if (ParmIdx >= ParmTblSize)
            return 0;
    }

    DummyParm = 0;
    if (ParmIdx >= ParmTblSize)
        return pFvOverrideTbl[ParmIdx-ParmTblSize];
    else
        return *(uint32_t*)ParmAddrTbl[ParmIdx];
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      get the physical address of a single parameter
//
// RETURN VALUE
//      address of the parameter
//
///////////////////////////////////////////////////////////////////////////////
void* cParameterBase::GetAddress(unsigned Laddr)
{
    // check for logical address
    if (Laddr >= (ParmTblSize+OverrideTblSize))
    {
        if (!ParmErrorLogged)
        {
            ParmErrorLogged = true;
            MarkStatDiag.LogErrorMessage(ALARM_MSG, cParamString(MKS_VAR_STR,
                cParamString("GetAddress request for invalid parameter logical address %d (0x%X)\n", Laddr, Laddr).c_str()),
                HERE);
        }
        ++ParmLogAddrErrCnt;
        ParmLogAddrErr = Laddr;
        return NULL;
    }

    if (Laddr >= ParmTblSize)
        return &pFvOverrideTbl[Laddr-ParmTblSize];
    else
        return ParmAddrTbl[Laddr];
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      reset all parameters to zero
//
// EXECUTION LEVEL
//      Initialization
//
///////////////////////////////////////////////////////////////////////////////
void cParameterBase::ParmReset()
{
    int             Idx;

    for (Idx=0; Idx < (int)(ParmTblSize+OverrideTblSize); ++Idx)
    {
        if (Idx >= (int)ParmTblSize)
            pFvOverrideTbl[Idx-ParmTblSize] = 0;
        else
            *(uint32_t*)ParmAddrTbl[Idx] = 0;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      stub function to be supplied by
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cParameterBase::SaveSingleToFile (
        void* pAddr                     // parameter address or index
        )
{
    return FAIL;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Final Value initialization
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cParameterBase::FvInitialize(void)
{
    int     Status = SUCCESS;
    int     Idx;

    // run initial ruleset
    if ( InitialRules() == FAIL )
        Status = FAIL;

    // scan or overrides
    OverridePresent = false;
    for (Idx=0; Idx < (int)OverrideTblSize; ++Idx)
    {
        if ((pFvTbl[Idx] & pFvOverrideTbl[Idx]) != 0)
            OverridePresent = true;
    }

    return (Status);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      test if parameter is a final value
//
// EXECUTION LEVEL
//      Any
//
// RETURN VALUE
//      Status  true/false
//
///////////////////////////////////////////////////////////////////////////////
bool cParameterBase::IsFv (
        void* pAddr                     // parameter address or index
        )
{
    unsigned    ParmIdx;        // index into parameter table

    // replacing IsFnlVal()

    // check for logical address
    if ((unsigned)pAddr < ParmTblSize)
        ParmIdx = (unsigned)pAddr;
    // absolute address must be in parameter table
    else
    {
        for (ParmIdx=0; ParmIdx < ParmTblSize; ++ParmIdx)
        {
            if (ParmAddrTbl[ParmIdx] == pAddr)
                break;
        }
        if (ParmIdx >= ParmTblSize)
            return false;
    }

    // test for final value
    if ( (pFvTbl[ParmIdx >> 5] & (1 << (ParmIdx & 0x1F))) != 0 )
        return true;

    return false;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      test if parameter is an overridden final value
//
// EXECUTION LEVEL
//      Any
//
// RETURN VALUE
//      Status  true/false
//
///////////////////////////////////////////////////////////////////////////////
bool cParameterBase::IsFvOverridden (
        void* pAddr                     // parameter address or index
        )
{
    unsigned    ParmIdx;        // index into parameter table

    // replacing ISPATCHED()

    // check for logical address
    if ((unsigned)pAddr < ParmTblSize)
        ParmIdx = (unsigned)pAddr;
    // absolute address must be in parameter table
    else
    {
        for (ParmIdx=0; ParmIdx < ParmTblSize; ++ParmIdx)
        {
            if (ParmAddrTbl[ParmIdx] == pAddr)
                break;
        }
        if (ParmIdx >= ParmTblSize)
            return false;
    }

    // test for final value and override
    if ( ( (pFvTbl[ParmIdx >> 5] & (1 << (ParmIdx & 0x1F))) != 0 )
         && ( (pFvOverrideTbl[ParmIdx >> 5] & (1 << (ParmIdx & 0x1F))) != 0 ) )
        return true;

    return false;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      register parameter as final value
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cParameterBase::FvRegister (
        void* pAddr                     // parameter address or index
        )
{
    unsigned    ParmIdx;        // index into parameter table

    // replacing FnlValPatchMgr()

    // check for logical address
    if ((unsigned)pAddr < ParmTblSize)
        ParmIdx = (unsigned)pAddr;
    // absolute address must be in parameter table
    else
    {
        for (ParmIdx=0; ParmIdx < ParmTblSize; ++ParmIdx)
        {
            if (ParmAddrTbl[ParmIdx] == pAddr)
                break;
        }
        if (ParmIdx >= ParmTblSize)
            return FAIL;
    }

    // mark parameter as final value
    pFvTbl[ParmIdx >> 5] |= (1 << (ParmIdx & 0x1F));

    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      set final value override flag
//
// EXECUTION LEVEL
//      ADL thread
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cParameterBase::FvSetOverride (
        void* pAddr                     // parameter address or index
        )
{
    unsigned    ParmIdx;        // index into parameter table

    // check for logical address
    if ((unsigned)pAddr < ParmTblSize)
        ParmIdx = (unsigned)pAddr;
    // absolute address must be in parameter table
    else
    {
        for (ParmIdx=0; ParmIdx < ParmTblSize; ++ParmIdx)
        {
            if (ParmAddrTbl[ParmIdx] == pAddr)
                break;
        }
        if (ParmIdx >= ParmTblSize)
            return FAIL;
    }

    // test for final value
    if ( (pFvTbl[ParmIdx >> 5] & (1 << (ParmIdx & 0x1F))) == 0 )
        return FAIL;

    // mark final value as overridden
    pFvOverrideTbl[ParmIdx >> 5] |= (1 << (ParmIdx & 0x1F));
    OverridePresent = true;

    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      clear final value override flag
//
// EXECUTION LEVEL
//      ADL thread
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cParameterBase::FvClrOverride (
        void* pAddr                     // parameter address or index
        )
{
    unsigned    ParmIdx;        // index into parameter table
    int         Status=SUCCESS;
    int         Idx;

    // check for logical address
    if ((unsigned)pAddr < ParmTblSize)
        ParmIdx = (unsigned)pAddr;
    // absolute address must be in parameter table
    else
    {
        for (ParmIdx=0; ParmIdx < ParmTblSize; ++ParmIdx)
        {
            if (ParmAddrTbl[ParmIdx] == pAddr)
                break;
        }
        if (ParmIdx >= ParmTblSize)
            return FAIL;
    }

    // test for final value
    if ( (pFvTbl[ParmIdx >> 5] & (1 << (ParmIdx & 0x1F))) == 0 )
        return FAIL;

    // clear final value override
    pFvOverrideTbl[ParmIdx >> 5] &= ~(1 << (ParmIdx & 0x1F));

    // scan or overrides
    OverridePresent = false;
    for (Idx=0; Idx < (int)OverrideTblSize; ++Idx)
    {
        if ((pFvTbl[Idx] & pFvOverrideTbl[Idx]) != 0)
            OverridePresent = true;
    }

    // run rules to restore final value
    if ( RunRuleSet(RULE_SET_MAIN) == FAIL )
        Status = FAIL;

    // save a single parameter to file
    if (SaveSingleToFile((void*)ParmIdx) == FAIL)
        Status = FAIL;

    return Status;
}

