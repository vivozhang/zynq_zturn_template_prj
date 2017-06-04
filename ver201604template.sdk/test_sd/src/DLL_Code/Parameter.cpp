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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// core
// framework
#include "x0FrameWork.h"
#include "x0Err.h"
#include "x0Diagnostic.h"
#include "x0Parameter.h"
#include "DLLInterface.h"


#ifndef PRE_PROC
    #define  PRM_STATIC_INIT
    #include "ParmExtern.h"
    #undef   PRM_STATIC_INIT
#endif



// Constants:
//-----------


// Externals:
//-----------
extern cMarkStatDiag            MarkStatDiag;
extern Code_ID_struct           Code_ID;
extern int                      DummyParm;


// Types and Classes:
//-------------------


// Function Prototypes:
//---------------------
extern int      InitialRules(void);


// Variables:
//-----------
cDLLInterface   gDLLInterface;
const char*     pDfltParmLoadFile = "UserParameter.csv";
int             FirstParm;
int             LastParm;


// Parameters:
//------------
CREATE_PARM(Cfg_Product, unsigned);
CREATE_PARM(Cfg_Version, WORD);


// Diagnostics:
//------------
CREATE_DIAG(VerMisMatch);
CREATE_DIAG(ParmFileReadError);


// Parameter address cross reference table
//----------------------------------------
#undef   PARM_DLL_ENTRY
#define  PARM_DLL_ENTRY(PubName, Cparm, DataType, DefaultVal)  &Cparm,
void* cParameter::DefParmAddrTbl[] =
{
    #ifndef PRE_PROC
        #include "DLLParameter.def"
    #endif
};


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cParameter::cParameter()
{
    ParmTblSize = (sizeof(DefParmAddrTbl)/sizeof(DefParmAddrTbl[0]));
    ParmAddrTbl = &DefParmAddrTbl[0];
    pParmFile   = pDfltParmLoadFile;

    CreateTables();
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      read parameter file into individual parameters using the translation address table
//      verify valid parameter set with; CRC, version, product
//
// EXECUTION LEVEL
//      Initialization
//
///////////////////////////////////////////////////////////////////////////////
void cParameter::Initialize()
{
    FILE*       pFile=NULL;
    char*       pName;
    char*       pValueStr;
    char*       pParmBuf=NULL;
    int         ParmBufSize=200;  // enough bytes for one line of the user parameter file


    //-------------------------------------------------------------------------
    // validate and read the parameter file
    do
    {
        // set parameter defaults
        gDLLInterface.Initialize();
        if (gDLLInterface.SetParmDefaults() != SUCCESS)
        {
            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_PARM_STR,
                cParamString("failed to set parameter defaults").c_str()),
                HERE);
            break;
        }
        ParmInfoType* TmpParmInfo2;

        // run rules to mark final values
        InitialRules();

        TmpParmInfo2 = gDLLInterface.FindParmSymbol("XSimModeReq");
        gDLLInterface.SetParmValue(TmpParmInfo2->Index, "0");
        TmpParmInfo2 = gDLLInterface.FindParmSymbol("DsimModeReq");
        gDLLInterface.SetParmValue(TmpParmInfo2->Index, "0");
        TmpParmInfo2 = gDLLInterface.FindParmSymbol("SimModeReq");
        gDLLInterface.SetParmValue(TmpParmInfo2->Index, "1");

        break;//ignore the reading from csv file

        // read parameters from file
        if ((pFile=fopen(pParmFile, "r")) == NULL)
        {
            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_PARM_STR,
                cParamString("Failed to open Parameter file, %s (%d,%s)", pParmFile, errno, strerror(errno)).c_str()),
                HERE);
            PUSH_DIAG(ParmFileReadError);
            ParmReset();
            break;
        }
        if ((pParmBuf = (char*)malloc(ParmBufSize)) == NULL)
        {
            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_PARM_STR,
                cParamString("Failed to malloc memory for Parameter buffer, (%d,%s)", errno, strerror(errno)).c_str()),
                HERE);
            ParmReset();
            break;
        }

        // copy file parameters into individual parameters
        ParmInfoType* pTmpParmInfo;
        while (fgets(pParmBuf, ParmBufSize, pFile) != NULL)
        {
            pName  = strtok(pParmBuf, ",;\n");
            pValueStr = strtok(NULL, ",;\n");
            if ( (pName != NULL) && (pValueStr != NULL) )
            {
                pTmpParmInfo = gDLLInterface.FindParmSymbol(pName);
                if (pTmpParmInfo == NULL)
                {
                    MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_PARM_STR,
                        cParamString("could not find parameter symbol %s",pName).c_str()),
                        HERE);
                }
                else
                {
                    if (gDLLInterface.SetParmValue(pTmpParmInfo->Index, pValueStr) == FAIL)
                    {
                        MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_PARM_STR,
                            cParamString("failed to set parameter %s to %s", pName, pValueStr).c_str()),
                            HERE);
                    }
                }
            }
        }


        // verify the parameter file product type pedigree
        // can Code_ID.string be used instead
//        if (PARM(Cfg_Product) != _PRODUCT_NUM_)
//        {
//            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_PARM_STR,
//                cParamString("Parameter File type MISMATCH, runtime=%d, file=%d", _PRODUCT_NUM_, PARM(Cfg_Product)).c_str()),
//                HERE);
//            PUSH_DIAG(VerMisMatch);
//            ParmReset();
//            break;
//        }

        // verify the parameter file product version pedigree (major and minor must match)
//        if ( (PARM(Cfg_Version) & 0xFFFFF000) != (Code_ID.version & 0xFFFFF000) )
//        {
//            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_PARM_STR,
//                cParamString("Parameter File version MISMATCH, runtime=%06X, file=%06X", (Code_ID.version & 0xFFFFF000), (PARM(Cfg_Version) & 0xFFFFF000)).c_str()),
//                HERE);
//            PUSH_DIAG(VerMisMatch);
//            ParmReset();
//            break;
//        }

        // success if got to here
        printf("Parameter File read SUCCESS: %s\n", pParmFile);
        //ParmFileUsed = true;

    } while (0);    // do this once

    if (pFile != NULL)
        fclose(pFile);

    if (pParmBuf != NULL)
        free(pParmBuf);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      calculate CRC for entire parameter/final value list
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      CRC
//
///////////////////////////////////////////////////////////////////////////////
uint32_t cParameter::ParmFvCrc()
{
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      calculate CRC for active parameter list
//      normal operation is to calculate CRC in chunks defined by CRC_CHK_SIZE
//
// EXECUTION LEVEL
//      Initialization
//      ADL thread
//      Task Background
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cParameter::CrcCheck (
        int ReCalcCrc                   // calculate CRC from start to finish with no brakes (true/false)
        )
{

    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      save a single parameter value, corresponding patch table entry, updated CRC
//      to the parameter file
//
// EXECUTION LEVEL
//      ADL thread
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cParameter::SaveSingleToFile (
        void* pAddr                     // parameter address or index
        )
{
    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      save all parameters to file
//
// EXECUTION LEVEL
//      Task Background
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cParameter::SaveAllToFile (
        void* pParm                     // pointer to continious memory of parameter values
        )
{
    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      confirm parameter file is valid by checking CRC
//      if test fails rewrite file
//
// EXECUTION LEVEL
//      Task Background
//
///////////////////////////////////////////////////////////////////////////////
void cParameter::FileCrcCheck()
{
;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
int cParameter::Monitor (
        cMonitorCommand &gMonitor
        )
{
    return EOK;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
int cParameter::MonStatus (
        cParamString &OutputBuffer,
        cParamString &CommandLineArgs
        )
{
    return EOK;
}

