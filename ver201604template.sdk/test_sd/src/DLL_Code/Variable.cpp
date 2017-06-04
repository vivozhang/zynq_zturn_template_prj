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
// core
// framework
#include "x0Variable.h"


#ifndef PRE_PROC
    #define  VAR_STATIC_INIT
    #include "VarExtern.h"
    #undef   VAR_STATIC_INIT
#endif



// Constants:
//-----------


// Externals:
//-----------
extern int             DummyVar;


// Types and Classes:
//-------------------


// Function Prototypes:
//---------------------


// Variables:
//-----------


// Parameters:
//------------


// Diagnostics:
//------------


// Variable address cross reference table
//---------------------------------------
#undef  VAR_ENTRY                                                                                           // temporary solution
#define VAR_ENTRY(Class, OrigClass, BaseName, PubName, Cparm, Uniquifier, DataType, LadrDataType, RdOnly)   // temporary solution
#include "ProductVar.def"                                                                                   // temporary solution

#undef  VAR_ENTRY
#define VAR_ENTRY(Class, OrigClass, BaseName, PubName, Cparm, Uniquifier, DataType, LadrDataType, RdOnly)  {(void*)&Cparm, nsVariable::LadrDataType},
VarAddrTbl_Typ cVariable::mDefVarAddrTbl[] = {
    #ifndef PRE_PROC
        #include "Variable.def"
    #endif
};


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cVariable::cVariable()
{

    mVarTblSize = sizeof(mDefVarAddrTbl) / sizeof(mDefVarAddrTbl[0]);
    mVarAddrTbl = &mDefVarAddrTbl[0];

    CreateTables();
}



