///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Inter-Task Data Passer Class
//
// DESCRIPTION:
//      This file contains the methods for data passer class
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      x0DataPass.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
#include <string.h>
// framework
#include "x0DataPass.h"
#include "x0Err.h"
#include "x0Diagnostic.h"


// Constants
//----------


// Externals
//----------
extern cMarkStatDiag    MarkStatDiag;


// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Variables
//----------
unsigned    DummyPass;


// Static Class Members Data (initialized by the macro):
//------------------------------------------------------

// Following macro initializes the various data passing entries.
// C++ explicit casting is used to prevent compiling code that data-passes to a
// constant Var, even when the constant location is a class Output, which is
// being accessed directly within the class instance.
#define PASS_ENTRY(a, b) { (void*) (&a), (void*) (&b), sizeof(a), 0.0 },

// Following file is automatically created during code build, has externs so the addresses can be resovled at link time
#ifndef PRE_PROC
    #include "DataPassExtern.h"
#endif

DataPassItemTyp cDataPasser::T12_DataPassItems[] =
    {
    #ifndef PRE_PROC
        #define T1_to_T2
        #include "DataPass.def"
        #undef  T1_to_T2
    #endif
    };
DataPassItemTyp cDataPasser::T13_DataPassItems[] =
    {
    #ifndef PRE_PROC
        #define T1_to_T3
        #include "DataPass.def"
        #undef  T1_to_T3
    #endif
    };
DataPassItemTyp cDataPasser::T1A2_DataPassItems[] =
    {
    #ifndef PRE_PROC
    #define T1A_to_T2
    #include "DataPass.def"
    #undef  T1A_to_T2
    #endif
    };
DataPassItemTyp cDataPasser::T1A3_DataPassItems[] =
    {
    #ifndef PRE_PROC
    #define T1A_to_T3
    #include "DataPass.def"
    #undef  T1A_to_T3
    #endif
    };
DataPassItemTyp cDataPasser::T21_DataPassItems[] =
    {
    #ifndef PRE_PROC
        #define T2_to_T1
        #include "DataPass.def"
        #undef  T2_to_T1
    #endif
    };
DataPassItemTyp cDataPasser::T23_DataPassItems[] =
    {
    #ifndef PRE_PROC
        #define T2_to_T3
        #include "DataPass.def"
        #undef  T2_to_T3
    #endif
    };
DataPassItemTyp cDataPasser::T31_DataPassItems[] =
    {
    #ifndef PRE_PROC
        #define T3_to_T1
        #include "DataPass.def"
        #undef  T3_to_T1
    #endif
    };
DataPassItemTyp cDataPasser::T32_DataPassItems[] =
    {
    #ifndef PRE_PROC
        #define T3_to_T2
        #include "DataPass.def"
        #undef  T3_to_T2
    #endif
    };
DataPassItemTyp cDataPasser::T12F_DataPassItems[] =
    {
    #ifndef PRE_PROC
        #define T1_to_T2_FILT
        #include "DataPass.def"
        #undef  T1_to_T2_FILT
    #endif
    };
DataPassItemTyp cDataPasser::T13F_DataPassItems[] =
    {
    #ifndef PRE_PROC
        #define T1_to_T3_FILT
        #include "DataPass.def"
        #undef  T1_to_T3_FILT
    #endif
    };
DataPassItemTyp cDataPasser::T1A2F_DataPassItems[] =
    {
    #ifndef PRE_PROC
        #define T1A_to_T2_FILT
        #include "DataPass.def"
        #undef  T1A_to_T2_FILT
    #endif
    };
    DataPassItemTyp cDataPasser::T1A3F_DataPassItems[] =
    {
    #ifndef PRE_PROC
        #define T1A_to_T3_FILT
        #include "DataPass.def"
        #undef  T1A_to_T3_FILT
    #endif
    };

DataPassItemTyp cDataPasser::T23F_DataPassItems[] =
    {
    #ifndef PRE_PROC
        #define T2_to_T3_FILT
        #include "DataPass.def"
        #undef  T2_to_T3_FILT
    #endif
    };

#undef PASS_ENTRY


// !!! this array order MUST correspond to TaskPairEnum definitions !!!
//---------------------------------------------------------------------
DataPassInfoTyp cDataPasser::DataPassInfo[NumOfTaskPairs] =
{
    { cDataPasser::T12_DataPassItems,   sizeof(cDataPasser::T12_DataPassItems)   / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T13_DataPassItems,   sizeof(cDataPasser::T13_DataPassItems)   / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T1A2_DataPassItems,  sizeof(cDataPasser::T1A2_DataPassItems)  / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T1A3_DataPassItems,  sizeof(cDataPasser::T1A3_DataPassItems)  / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T21_DataPassItems,   sizeof(cDataPasser::T21_DataPassItems)   / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T23_DataPassItems,   sizeof(cDataPasser::T23_DataPassItems)   / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T31_DataPassItems,   sizeof(cDataPasser::T31_DataPassItems)   / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T32_DataPassItems,   sizeof(cDataPasser::T32_DataPassItems)   / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T12F_DataPassItems,  sizeof(cDataPasser::T12F_DataPassItems)  / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T13F_DataPassItems,  sizeof(cDataPasser::T13F_DataPassItems)  / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T1A2F_DataPassItems, sizeof(cDataPasser::T1A2F_DataPassItems) / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T1A3F_DataPassItems, sizeof(cDataPasser::T1A3F_DataPassItems) / sizeof(DataPassItemTyp) ,0 },
    { cDataPasser::T23F_DataPassItems,  sizeof(cDataPasser::T23F_DataPassItems)  / sizeof(DataPassItemTyp) ,0 }
};


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Constructor for the data passer class.
//      This function verifies that all data passer's pointers are valid.
//
// EXECUTION LEVEL
//      Clas instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cDataPasser::cDataPasser()
{
    int     TaskPair;
    int     Idx;
    int     NullPtrFound;

    // look for NULL pointers
    NullPtrFound = false;
    for (TaskPair=0; TaskPair < NumOfTaskPairs; ++TaskPair)
    {
        for (Idx=0; Idx < (int)DataPassInfo[TaskPair].NumOfItems; Idx++ )
        {
            if ( (DataPassInfo[TaskPair].pItemArr[Idx].pSrc == NULL) || (DataPassInfo[TaskPair].pItemArr[Idx].pDst == NULL) )
            {
                DataPassInfo[TaskPair].pItemArr[Idx].pSrc = &DummyPass;
                DataPassInfo[TaskPair].pItemArr[Idx].pDst = &DummyPass;
                DataPassInfo[TaskPair].pItemArr[Idx].Size = sizeof(DummyPass);
                NullPtrFound = true;
            }
        }
    }

    if (NullPtrFound)
    {
        MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DATAP_STR,
            cParamString("NULL pointer found in DataPass address table").c_str()),
            HERE);
    }
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction.
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
cDataPasser::~cDataPasser()
{
}
