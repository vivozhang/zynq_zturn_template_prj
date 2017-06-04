
// Include Files:
//---------------
// system
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string>
#include <vector>
#include <algorithm>
// core
// framework
#include "x0FrameWork.h"
#include "x0Err.h"
#include "DLLInterface.h"



// Constants:
//-----------


// Externals:
//-----------
extern cDLLInterface    gDLLInterface;
extern cParameter       gParameter;
extern SysTm_Typ        EventTm1;


// Types and Classes:
//-------------------


// Function Prototypes:
//---------------------


// Variables:
//-----------
std::vector<ParmInfoType*> ParmXrefTable;
std::vector<VarInfoType*>  VarXrefTable;
const char*     pDfltParmDumpFile = "ParameterDump.csv";


// Parameters:
//------------


// Diagnostics:
//------------


// address cross reference table
//----------------------------------------
#undef  PARM_DLL_ENTRY
#define PARM_DLL_ENTRY(PubName, Cparm, DataType, DefaultVal)  {#PubName, 0, nsDLLInterface::DataType, #DefaultVal},
ParmInfoType ParmInfoTbl[] =
{
    #ifndef PRE_PROC
        #include "DLLParameter.def"
    #endif
};

#undef  VAR_ENTRY
#define VAR_ENTRY(Class, OrigClass, BaseName, PubName, Cparm, Uniquifier, DataType, LadrDataType, ReadOnly)  {#PubName, 0, nsDLLInterface::LadrDataType, (#ReadOnly=="RdOnly"?1:0)},
VarInfoType VarInfoTbl[] =
{
    #ifndef PRE_PROC
        #include "Variable.def"
    #endif
};


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
static bool ParmSortCompare(const void *p1, const void *p2)
{
    if(_stricmp(((ParmInfoType*)p1)->Symbol.c_str(), ((ParmInfoType*)p2)->Symbol.c_str()) < 0)
        return true;

    return false;
}

static bool VarSortCompare(const void *p1, const void *p2)
{
    if(_stricmp(((VarInfoType*)p1)->Symbol.c_str(), ((VarInfoType*)p2)->Symbol.c_str()) < 0)
        return true;

    return false;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      initialize and sort parameter, variable translation tables
//      used to search for info by name
//
///////////////////////////////////////////////////////////////////////////////
void cDLLInterface::Initialize()
{
    int Idx;

    // initialize parameter index
    for (Idx=0; Idx < sizeof(ParmInfoTbl)/sizeof(ParmInfoTbl[0]); ++Idx)
    {
        ParmInfoTbl[Idx].Index = Idx;
    }

    ParmXrefTable.clear();
    for (Idx=0; Idx < sizeof(ParmInfoTbl)/sizeof(ParmInfoTbl[0]); ++Idx)
    {
        ParmXrefTable.push_back(&ParmInfoTbl[Idx]);
    }

    std::sort(ParmXrefTable.begin(), ParmXrefTable.end(), ParmSortCompare);

    // initialize variable index
    for (Idx=0; Idx < sizeof(VarInfoTbl)/sizeof(VarInfoTbl[0]); ++Idx)
    {
        VarInfoTbl[Idx].Index = Idx;
    }

    VarXrefTable.clear();
    for (Idx=0; Idx < sizeof(VarInfoTbl)/sizeof(VarInfoTbl[0]); ++Idx)
    {
        VarXrefTable.push_back(&VarInfoTbl[Idx]);
    }

    std::sort(VarXrefTable.begin(), VarXrefTable.end(), VarSortCompare);

}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      initialize parameter's to their default values
//
///////////////////////////////////////////////////////////////////////////////
int cDLLInterface::SetParmDefaults()
{
    int      Status = SUCCESS;
    int      Idx;

    for (Idx=0; Idx < sizeof(ParmInfoTbl)/sizeof(ParmInfoTbl[0]); ++Idx)
    {
        if (SetParmValue(Idx, ParmInfoTbl[Idx].DefaultVal) == FAIL)
            Status = FAIL;
    }

    return Status;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      set parameter to value
//
///////////////////////////////////////////////////////////////////////////////
int cDLLInterface::SetParmValue(unsigned ParmIdx, std::string ValueStr)
{
    int      Status = SUCCESS;
    uint32_t Value;

    Value = StringToValue(ValueStr, ParmInfoTbl[ParmIdx].DataType);

    if (gParameter.SetValue((void*)ParmIdx, Value) == FAIL)
        Status = FAIL;

    return Status;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      read parameter settings from a CSV file
//
///////////////////////////////////////////////////////////////////////////////
int cDLLInterface::LoadParameterFile(std::string ParmFileName)
{
    FILE*       pFile=NULL;
    char*       pName;
    char*       pValueStr;
    char*       pParmBuf=NULL;
    int         ParmBufSize=200;  // enough bytes for one line of the user parameter file
    int         Status = SUCCESS;


    do
    {
        // read parameters from file
        if ((pFile=fopen(ParmFileName.c_str(), "r")) == NULL)
        {
            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                cParamString("Failed to open Parameter Load file, %s (%d,%s)", ParmFileName.c_str(), errno, strerror(errno)).c_str()),
                HERE);
            Status = FAIL;
            break;
        }
        if ((pParmBuf = (char*)malloc(ParmBufSize)) == NULL)
        {
            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                cParamString("Failed to malloc memory for Parameter Load buffer, (%d,%s)", errno, strerror(errno)).c_str()),
                HERE);
            Status = FAIL;
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
                pTmpParmInfo = FindParmSymbol(pName);
                if (pTmpParmInfo == NULL)
                {
                    MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                        cParamString("could not find parameter symbol %s",pName).c_str()),
                        HERE);
                    Status = FAIL;
                }
                else
                {
                    if (SetParmValue(pTmpParmInfo->Index, pValueStr) == FAIL)
                    {
                        MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                            cParamString("failed to set parameter %s to %s", pName, pValueStr).c_str()),
                            HERE);
                        Status = FAIL;
                    }
                }
            }
        }

    } while (0);    // do this once

    if (pFile != NULL)
        fclose(pFile);

    if (pParmBuf != NULL)
        free(pParmBuf);

    return Status;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      write parameter settings to a CSV file
//
///////////////////////////////////////////////////////////////////////////////
int cDLLInterface::DumpParameterFile(std::string ParmFileName)
{
    std::string ParmDumpFile;
    FILE*       pFile=NULL;
    int         Status = SUCCESS;
    uint32_t    TmpValue;

    if (ParmFileName == "")
        ParmDumpFile = pDfltParmDumpFile;
    else
        ParmDumpFile = ParmFileName;

    do
    {
        if ((pFile=fopen(ParmDumpFile.c_str(), "w")) == NULL)
        {
            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                cParamString("Failed to open Parameter Dump file, %s (%d,%s)", ParmFileName.c_str(), errno, strerror(errno)).c_str()),
                HERE);
            Status = FAIL;
            break;
        }

        fprintf(pFile, "Name,Default Value,Device Value,Override\n");

        std::vector<ParmInfoType*>::const_iterator Iter;
        for (Iter = ParmXrefTable.begin(); Iter != ParmXrefTable.end(); ++Iter)
        {
            fprintf(pFile, "%s", (*Iter)->Symbol.c_str());

            if (gParameter.IsFv((void*)(*Iter)->Index))
                fprintf(pFile, ",%s", "FV");
            else
                fprintf(pFile, ",%s", (*Iter)->DefaultVal.c_str());

            TmpValue = gParameter.GetValue((void*)(*Iter)->Index);
            fprintf(pFile, ",%s", ValueToString(TmpValue, (*Iter)->DataType));

            if (gParameter.IsFvOverridden((void*)(*Iter)->Index))
                fprintf(pFile, ",True");

            fprintf(pFile, "\n");
        }
        printf("%d parameters dumped to file %s\n", ParmXrefTable.size(), ParmDumpFile.c_str());


    } while (0);    // do this once


    if (pFile != NULL)
        fclose(pFile);

    return Status;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      write parameter settings and variable values to a CSV file
//
///////////////////////////////////////////////////////////////////////////////
int cDLLInterface::SaveSimulationFile(std::string SimFileName)
{
    FILE*       pFile=NULL;
    int         Status = SUCCESS;
    uint32_t    TmpValue;


    do
    {
        if ((pFile=fopen(SimFileName.c_str(), "w")) == NULL)
        {
            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                cParamString("Failed to open Simulation file for saving, %s (%d,%s)", SimFileName.c_str(), errno, strerror(errno)).c_str()),
                HERE);
            Status = FAIL;
            break;
        }

        fprintf(pFile, "Type,Name,Device Value\n");

        std::vector<ParmInfoType*>::const_iterator PrmIter;
        for (PrmIter = ParmXrefTable.begin(); PrmIter != ParmXrefTable.end(); ++PrmIter)
        {
            if (gParameter.IsFvOverridden((void*)(*PrmIter)->Index))
                fprintf(pFile, "Ovr");
            else if (gParameter.IsFv((void*)(*PrmIter)->Index))
                fprintf(pFile, "FV ");
            else
                fprintf(pFile, "Prm");

            fprintf(pFile, ",%s", (*PrmIter)->Symbol.c_str());

            TmpValue = gParameter.GetValue((void*)(*PrmIter)->Index);
            fprintf(pFile, ",%s", ValueToString(TmpValue, (*PrmIter)->DataType));

            fprintf(pFile, "\n");
        }
        printf("\n");
        printf("%d parameters saved to file %s\n", ParmXrefTable.size(), SimFileName.c_str());

        std::vector<VarInfoType*>::const_iterator VarIter;
        for (VarIter = VarXrefTable.begin(); VarIter != VarXrefTable.end(); ++VarIter)
        {
            fprintf(pFile, "Var");

            fprintf(pFile, ",%s", (*VarIter)->Symbol.c_str());

            TmpValue = *((uint32_t*)gVariable.GetAddress((*VarIter)->Index));
            fprintf(pFile, ",%s", ValueToString(TmpValue, (*VarIter)->DataType));

            fprintf(pFile, "\n");
        }
        printf("%d variables saved to file %s\n", VarXrefTable.size(), SimFileName.c_str());

    } while (0);    // do this once


    if (pFile != NULL)
        fclose(pFile);

    return Status;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      read parameter settings and variable values from a CSV file
//
///////////////////////////////////////////////////////////////////////////////
int cDLLInterface::RestoreSimulationFile(std::string SimFileName)
{
    FILE*       pFile=NULL;
    char*       pType;
    char*       pName;
    char*       pValueStr;
    char*       pParmBuf=NULL;
    int         ParmBufSize=200;  // enough bytes for one line of the simulation file
    int         Status = SUCCESS;


    do
    {
        if ((pFile=fopen(SimFileName.c_str(), "r")) == NULL)
        {
            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                cParamString("Failed to open Simulation file for restoring, %s (%d,%s)", SimFileName.c_str(), errno, strerror(errno)).c_str()),
                HERE);
            Status = FAIL;
            break;
        }
        if ((pParmBuf = (char*)malloc(ParmBufSize)) == NULL)
        {
            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                cParamString("Failed to malloc memory for Simulation Restore buffer, (%d,%s)", errno, strerror(errno)).c_str()),
                HERE);
            Status = FAIL;
            break;
        }

        // restore file variables
        VarInfoType* pTmpVarInfo;
        while (fgets(pParmBuf, ParmBufSize, pFile) != NULL)
        {
            pType     = strtok(pParmBuf, ",;\n");
            pName     = strtok(NULL, ",;\n");
            pValueStr = strtok(NULL, ",;\n");
            if ( (pType != NULL) && (pName != NULL) && (pValueStr != NULL) )
            {
                if (strcmp(pType, "Var") == 0)
                {
                    pTmpVarInfo = FindVarSymbol(pName);
                    if (pTmpVarInfo == NULL)
                    {
                        MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                            cParamString("could not find variable symbol %s",pName).c_str()),
                            HERE);
                        Status = FAIL;
                    }
                    else if (!pTmpVarInfo->RdOnly)
                    {
                        *((uint32_t*)gVariable.GetAddress(pTmpVarInfo->Index)) = StringToValue(pValueStr, pTmpVarInfo->DataType);
                    }
                }
            }
        }

    } while (0);    // do this once


    if (pFile != NULL)
        fclose(pFile);

    if (pParmBuf != NULL)
        free(pParmBuf);

    return Status;
}


///////////////////////////////////////////////////////////////////////////////
char* cDLLInterface::ValueToString(uint32_t Value, nsDLLInterface::DataTypeEnum DataType)
{
    static char ReturnStr[100];
    ReturnStr[0] = 0;

    switch (DataType)
    {
        case nsDLLInterface::REAL:
            _snprintf(ReturnStr, sizeof(ReturnStr), "%1.6G", *((float*)&Value));
            break;
        case nsDLLInterface::DINT:
            _snprintf(ReturnStr, sizeof(ReturnStr), "%d", *((int32_t*)&Value));
            break;
        case nsDLLInterface::UDINT:
            _snprintf(ReturnStr, sizeof(ReturnStr), "%u", Value);
            break;
        case nsDLLInterface::DWORD:
            _snprintf(ReturnStr, sizeof(ReturnStr), "0x%08X", Value);
            break;
        case nsDLLInterface::pANY:
        case nsDLLInterface::pREAL:
        case nsDLLInterface::pDINT:
        case nsDLLInterface::pUDINT:
            VarInfoType* pTmpVarInfo;
            pTmpVarInfo = FindVarIndex(Value);
            if (pTmpVarInfo != NULL)
                _snprintf(ReturnStr, sizeof(ReturnStr), "%s", pTmpVarInfo->Symbol.c_str());
            else
                _snprintf(ReturnStr, sizeof(ReturnStr), "VAR_%05d", Value);
            break;

        default:
            _snprintf(ReturnStr, sizeof(ReturnStr), "0x%08X", Value);
            break;
    }

    return ReturnStr;
}


///////////////////////////////////////////////////////////////////////////////
uint32_t cDLLInterface::StringToValue(std::string ValueStr, nsDLLInterface::DataTypeEnum DataType)
{
    uint32_t Value;

    switch (DataType)
    {
        case nsDLLInterface::REAL:
            *((float*)&Value) = (float)strtod(ValueStr.c_str(), NULL);
            break;
        case nsDLLInterface::DINT:
            *((int32_t*)&Value) = strtol(ValueStr.c_str(), NULL, 10);
            break;
        case nsDLLInterface::UDINT:
            Value = strtoul(ValueStr.c_str(), NULL, 10);
            break;
        case nsDLLInterface::DWORD:
            Value = strtoul(ValueStr.c_str(), NULL, 16);
            break;
        case nsDLLInterface::pANY:
        case nsDLLInterface::pREAL:
        case nsDLLInterface::pDINT:
        case nsDLLInterface::pUDINT:
            VarInfoType* pTmpVarInfo;
            pTmpVarInfo = FindVarSymbol(ValueStr);
            if (pTmpVarInfo != NULL)
                Value = pTmpVarInfo->Index;
            else
                Value = 0;
            break;

        default:
            Value = 0;
            break;
    }

    return Value;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      find parameter by name and return info structure
//
///////////////////////////////////////////////////////////////////////////////
ParmInfoType* cDLLInterface::FindParmSymbol(std::string Symbol)
{
    int CompareResult;
    int low = 0;
    int high = ParmXrefTable.size() - 1;
    int p = low + ((high - low) / 2);

    while (low <= high)
    {
        CompareResult = _stricmp(ParmXrefTable[p]->Symbol.c_str(), Symbol.c_str());
        if (CompareResult > 0)
            high = p - 1;
        else if (CompareResult < 0) //Wasteful second comparison forced by syntax limitations.
            low = p + 1;
        else
            return ParmXrefTable[p];

        p = low + ((high - low) / 2); //Next probe position.
    }

    return NULL;
}

ParmInfoType* cDLLInterface::FindParmIndex(unsigned Index)
{
    if (Index < sizeof(ParmInfoTbl)/sizeof(ParmInfoTbl[0]))
        return &ParmInfoTbl[Index];
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      find variable by name and return info structure
//
///////////////////////////////////////////////////////////////////////////////
VarInfoType* cDLLInterface::FindVarSymbol(std::string Symbol)
{
    int CompareResult;
    int low = 0;
    int high = VarXrefTable.size() - 1;
    int p = low + ((high - low) / 2);

    while (low <= high)
    {
        CompareResult = _stricmp(VarXrefTable[p]->Symbol.c_str(), Symbol.c_str());
        if (CompareResult > 0)
            high = p - 1;
        else if (CompareResult < 0) //Wasteful second comparison forced by syntax limitations.
            low = p + 1;
        else
            return VarXrefTable[p];

        p = low + ((high - low) / 2); //Next probe position.
    }

    return NULL;
}

VarInfoType* cDLLInterface::FindVarIndex(unsigned Index)
{
    if (Index < sizeof(VarInfoTbl)/sizeof(VarInfoTbl[0]))
        return &VarInfoTbl[Index];
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
cDLLContract::cDLLContract(int ListCount)
{
    mCount           = ListCount;
    mAddIdx          = 0;
    mType            = TYPE_UNKNOWN;
    mDirection       = DIRECTION_UNKNOWN;
    pSamplingFile    = NULL;
    pSourceList      = (uint32_t**)malloc(ListCount * sizeof(uint32_t*));
    pDestinationList = (uint32_t**)malloc(ListCount * sizeof(uint32_t*));
    pSizeInBytes     = (unsigned*) malloc(ListCount * sizeof(unsigned));
    pDataType        = pSizeInBytes;

    if ( (pSourceList == NULL) || (pDestinationList == NULL) || (pSizeInBytes == NULL) )
    {
        MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
            cParamString("CreateContract malloc memory failed").c_str()),
            HERE);
    }
    else
        memset(pSizeInBytes, 0, ListCount * sizeof(unsigned));
}

cDLLContract::~cDLLContract()
{
    if (pSourceList != NULL)
    {
        free(pSourceList);
        pSourceList = NULL;
    }

    if (pDestinationList != NULL)
    {
        free(pDestinationList);
        pDestinationList = NULL;
    }

    if (pSizeInBytes != NULL)
    {
        free(pSizeInBytes);
        pSizeInBytes = NULL;
    }

    if (pSamplingFile != NULL)
    {
        fclose(pSamplingFile);
        pSamplingFile = NULL;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      open sampling file
//
// return SUCCESS or FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cDLLContract::FileOpen(std::string SamplingFileName)
{
    bool    ErrorFlag = false;

    if ((pSamplingFile=fopen(SamplingFileName.c_str(), "w")) == NULL)
    {
        MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
            cParamString("Failed to open Sampling file, %s (%d,%s)", SamplingFileName.c_str(), errno, strerror(errno)).c_str()),
            HERE);
        ErrorFlag = true;
    }

    return (ErrorFlag ? FAIL:SUCCESS);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      add a read transfer entry to the contract
//
// return SUCCESS or FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cDLLContract::AddXferItem(std::string Src, void* pDest, int SizeInBytes)
{
    bool    ErrorFlag = false;

    mDirection = READ;

    if (mAddIdx >= mCount)
        ErrorFlag = true;
    else if (pDest == NULL)
        ErrorFlag = true;
    else if (mType == PARM)
    {
        pDestinationList[mAddIdx]  = (uint32_t*)pDest;

        ParmInfoType* pTmpParmInfo;
        pTmpParmInfo = gDLLInterface.FindParmSymbol(Src);
        if (pTmpParmInfo != NULL)
        {
            pSourceList[mAddIdx] = (uint32_t*)gParameter.GetAddress(pTmpParmInfo->Index);
            ++mAddIdx;
        }
        else
            ErrorFlag = true;
    }
    else if (mType == VAR)
    {
        pDestinationList[mAddIdx]  = (uint32_t*)pDest;

        VarInfoType* pTmpVarInfo;
        pTmpVarInfo = gDLLInterface.FindVarSymbol(Src);
        if (pTmpVarInfo != NULL)
        {
            pSourceList[mAddIdx] = (uint32_t*)gVariable.GetAddress(pTmpVarInfo->Index);

            if ( (pTmpVarInfo->DataType == nsDLLInterface::MEM) && (SizeInBytes > sizeof(uint32_t)) )
                pSizeInBytes[mAddIdx] = SizeInBytes;

            ++mAddIdx;
        }
        else
            ErrorFlag = true;
    }
    else
        ErrorFlag = true;

    return (ErrorFlag ? FAIL:SUCCESS);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      add a write transfer entry to the contract
//
// return SUCCESS or FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cDLLContract::AddXferItem(void* pSrc, std::string Dest, int SizeInBytes)
{
    bool    ErrorFlag = false;

    mDirection = WRITE;

    if (mAddIdx >= mCount)
        ErrorFlag = true;
    else if (pSrc == NULL)
        ErrorFlag = true;
    else if (mType == PARM)
    {
        pSourceList[mAddIdx]  = (uint32_t*)pSrc;

        ParmInfoType* pTmpParmInfo;
        pTmpParmInfo = gDLLInterface.FindParmSymbol(Dest);
        if (pTmpParmInfo != NULL)
        {
            pDestinationList[mAddIdx] = (uint32_t*)pTmpParmInfo->Index;
            ++mAddIdx;
        }
        else
            ErrorFlag = true;
    }
    else if (mType == VAR)
    {
        pSourceList[mAddIdx]  = (uint32_t*)pSrc;

        VarInfoType* pTmpVarInfo;
        pTmpVarInfo = gDLLInterface.FindVarSymbol(Dest);
        if ((pTmpVarInfo != NULL) && !pTmpVarInfo->RdOnly)
        {
            pDestinationList[mAddIdx] = (uint32_t*)gVariable.GetAddress(pTmpVarInfo->Index);

            if ( (pTmpVarInfo->DataType == nsDLLInterface::MEM) && (SizeInBytes > sizeof(uint32_t)) )
                pSizeInBytes[mAddIdx] = SizeInBytes;

            ++mAddIdx;
        }
        else
            ErrorFlag = true;
    }
    else
        ErrorFlag = true;

    return (ErrorFlag ? FAIL:SUCCESS);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      add a variable sampling entry to the contract
//
// return SUCCESS or FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cDLLContract::AddSampleItem(std::string Src)
{
    bool    ErrorFlag = false;

    if (mAddIdx >= mCount)
        ErrorFlag = true;
    else
    {
        VarInfoType* pTmpVarInfo;
        pTmpVarInfo = gDLLInterface.FindVarSymbol(Src);
        if (pTmpVarInfo != NULL)
        {
            pSourceList[mAddIdx] = (uint32_t*)gVariable.GetAddress(pTmpVarInfo->Index);
            pDataType[mAddIdx]   = pTmpVarInfo->DataType;

            if (pSamplingFile != NULL)
            {
                if (mAddIdx == 0)
                    fprintf(pSamplingFile, "Time");

                fprintf(pSamplingFile, ",%s", pTmpVarInfo->Symbol.c_str());

                if (mAddIdx == mCount-1)
                    fprintf(pSamplingFile, "\n");
            }

            ++mAddIdx;
        }
        else
            ErrorFlag = true;
    }

    return (ErrorFlag ? FAIL:SUCCESS);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      create a variable contract to read or write variable values
//
// return SUCCESS or FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cDLLContract::ExecContract()
{
    time_t      TimeSec;
    struct tm*  timeinfo;
    static char TimeStr[30];
    unsigned    mSec;

    // print timestamp
    // ddd hh:mm:ss.sssssssss         needs fixing in trender
    // mm/dd/yyyy hh:mm:ss.sssssssss  works
    if (mType == VARLOGGER)
    {
        TimeSec = EventTm1.Sec;
        timeinfo = gmtime(&TimeSec);
        if (timeinfo->tm_year < 100)
            timeinfo->tm_year = 100;
            mSec = (EventTm1.nSec + 500000) / 1000000;
            if(mSec>=1000)
            {
                mSec = 999;
            }
        if (strftime(TimeStr, sizeof(TimeStr), "%m/%d/%Y %H:%M:%S", timeinfo) != 0)
            fprintf(pSamplingFile, "%s.%03d", TimeStr, mSec);
    }

    for (int Idx=0; Idx < mAddIdx; ++Idx)
    {
        if (mType == VARLOGGER)
        {
            fprintf(pSamplingFile, ",%s", gDLLInterface.ValueToString(*(pSourceList[Idx]), (nsDLLInterface::DataTypeEnum)pDataType[Idx]));
            if (Idx == mAddIdx-1)
                fprintf(pSamplingFile, "\n");
        }
        else if ((mType == PARM) && (mDirection == WRITE))
            gParameter.SetValue(pDestinationList[Idx], *(pSourceList[Idx]));
        else if (pSizeInBytes[Idx] > sizeof(uint32_t))
            memcpy(pDestinationList[Idx], pSourceList[Idx], pSizeInBytes[Idx]);
        else
            *(pDestinationList[Idx]) = *(pSourceList[Idx]);
    }

    return SUCCESS;
}

