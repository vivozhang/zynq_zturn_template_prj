// MarkStatDLL.cpp : Defines the exported functions for the DLL application.
//


// Include Files:
//---------------
// system
#include <list>
#include <stddef.h>
#include "semaphore.h"
// core
// framework
#include "x0FrameWork.h"
#include "x0Err.h"
#include "MarkStatDLL.h"
#include "DLLInterface.h"

#define  PRODUCTCODE
    #include "Version.inc"
#undef   PRODUCTCODE



// Constants:
//-----------
const char ApiVersion[] = "API  V00.12.03";  //Prev 00.12.00, but WindDFIG01 used 00.12.01


// Externals:
//-----------
extern int   ControllerInit(void);
extern int   xopsInitForDLL(void);
extern void* xopsSched(void* arg);
extern void* xopsBkgnd(void *data);
extern int   RunRuleSet(int Rule_Set);

extern cParameter       gParameter;
extern cDLLInterface    gDLLInterface;

extern unsigned         T1DoneOnce;
extern unsigned         T2DoneOnce;
extern unsigned         T3DoneOnce;
extern unsigned         TBDoneOnce;
extern unsigned         T3CalcDone;
extern unsigned         InitDone;
extern float            DelTm1Rcp;
extern unsigned         XopsSourceIsSysClock;
extern SysTm_Typ        SysTm1;
extern SysTm_Typ        EventTmOffs;


// Variables:
//-----------
std::list <sem_t*> TaskRunList;

//=============================================================================
namespace MarkStatDLL
{
    bool     Initialized = false;
    unsigned BackgroundCount = 0;
    unsigned CountsPerBackground = 0;
    bool     ReportedNullContract = false;

    ///////////////////////////////////////////////////////////////////////////
    //
    //
    ///////////////////////////////////////////////////////////////////////////
    char* MarkStatDLLApi::GetApiRevision()
    {
        return (char*)ApiVersion;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    //
    ///////////////////////////////////////////////////////////////////////////
    char* MarkStatDLLApi::GetProductRevision()
    {
        return (char*)ProductVersion;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      initialize the DLL
    //
    // UserParmFileName - filename of initial parameter values (optional)
    //
    ///////////////////////////////////////////////////////////////////////////
    void MarkStatDLLApi::Initialize(std::string UserParmFileName)
    {
        if (UserParmFileName != "")
            gParameter.SetParmFile(UserParmFileName.c_str());

        ControllerInit();
        xopsInitForDLL();
        CountsPerBackground = (unsigned)(0.5F * DelTm1Rcp);

        Initialized = true;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      read parameter settings from a CSV file
    //
    // ParmFileName - filename of parameter values
    //
    // return SUCCESS(1) or FAIL(0)
    //
    ///////////////////////////////////////////////////////////////////////////
    int MarkStatDLLApi::LoadParameters(std::string ParmFileName)
    {
        int         Status = SUCCESS;

        if (ParmFileName == "")
            return FAIL;

        if (gDLLInterface.LoadParameterFile(ParmFileName) != SUCCESS)
            Status = FAIL;

        if (RunRule() != SUCCESS)
            Status = FAIL;

        return Status;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      write parameter settings to a CSV file
    //
    // ParmFileName - filename for parameter values  (optional)
    //
    // return SUCCESS(1) or FAIL(0)
    //
    ///////////////////////////////////////////////////////////////////////////
    int MarkStatDLLApi::DumpParameters(std::string ParmFileName)
    {
        int         Status = SUCCESS;

        if (gDLLInterface.DumpParameterFile(ParmFileName) != SUCCESS)
            Status = FAIL;

        return Status;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      write parameter settings and variable values to a CSV file
    //
    // SimFileName - filename for parameter and variable values
    //
    // return SUCCESS(1) or FAIL(0)
    //
    ///////////////////////////////////////////////////////////////////////////
    int MarkStatDLLApi::SaveSimulation(std::string SimFileName)
    {
        int         Status = SUCCESS;

        if (SimFileName == "")
            return FAIL;

        if (gDLLInterface.SaveSimulationFile(SimFileName) != SUCCESS)
            Status = FAIL;

        return Status;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      write parameter settings and variable values to a CSV file
    //
    // SimFileName - filename of parameter and variable values
    //
    // return SUCCESS(1) or FAIL(0)
    //
    ///////////////////////////////////////////////////////////////////////////
    int MarkStatDLLApi::RestoreSimulation(std::string SimFileName)
    {
        int         Status = SUCCESS;

        if (SimFileName == "")
            return FAIL;

        if (gDLLInterface.RestoreSimulationFile(SimFileName) != SUCCESS)
            Status = FAIL;

        return Status;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      set parameter back to its default value
    //      or remove override on a final value
    //      then run the MAIN ruleset
    //
    // ParmName - name of parameter
    //
    // return SUCCESS(1) or FAIL(0)
    //
    ///////////////////////////////////////////////////////////////////////////
    int MarkStatDLLApi::ResetParameter(std::string ParmName)
    {
        ParmInfoType*   pTmpParmInfo;
        int             Status = SUCCESS;

        pTmpParmInfo = gDLLInterface.FindParmSymbol(ParmName);
        if (pTmpParmInfo == NULL)
        {
            Status = FAIL;
        }
        else if (gParameter.IsFv((void*)pTmpParmInfo->Index))
        {
            // rules run as part of clear override
            if (gParameter.FvClrOverride((void*)pTmpParmInfo->Index) != SUCCESS)
                Status = FAIL;
        }
        else
        {
            if (gDLLInterface.SetParmValue(pTmpParmInfo->Index, pTmpParmInfo->DefaultVal) != SUCCESS)
                Status = FAIL;
            else if (RunRule() != SUCCESS)
                Status = FAIL;
        }

        return Status;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      execute 1 step in the xops scheduler (ie: 1 task-1)
    //
    ///////////////////////////////////////////////////////////////////////////
    void MarkStatDLLApi::Step()
    {
        if (!Initialized)
            Initialize();

        xopsSched(NULL);

        // run all tasks that were added to the run list this pass
        std::list <sem_t*>::iterator Iter;
        for (Iter=TaskRunList.begin(); Iter != TaskRunList.end(); ++Iter)
        {

            if ((*Iter)->pTask != NULL)
                (*Iter)->pTask((*Iter)->pTaskData);
        }
        TaskRunList.clear();

        // every 500 msec
        if (++BackgroundCount >= CountsPerBackground)
        {
            BackgroundCount = 0;
            xopsBkgnd(NULL);
        }

        if ( T1DoneOnce && T2DoneOnce && T3DoneOnce && TBDoneOnce && T3CalcDone )
            InitDone = true;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      create and initialize a contract to either (Read or Write) a list of (Parameters or Variables)
    //      Read is data from DLL, Write is data to DLL
    //
    ///////////////////////////////////////////////////////////////////////////
    void* MarkStatDLLApi::CreateContract(int ListCount, ContractListType ContractList[], ContractEnum ContractType)
    {
        cDLLContract*   pTmpContract=NULL;
        bool            ErrorFlag = false;

        if (!Initialized)
            Initialize();

        pTmpContract = new (std::nothrow) cDLLContract(ListCount);

        switch (ContractType)
        {
            case READ_PARM:
            case WRITE_PARM:
                pTmpContract->SetType(cDLLContract::PARM);
                break;
            case READ_VAR:
            case WRITE_VAR:
                pTmpContract->SetType(cDLLContract::VAR);
                break;
            default:
                ErrorFlag = true;
                break;
        }

        for (int Idx=0; !ErrorFlag && (pTmpContract != NULL) && (Idx < ListCount); ++Idx)
        {
            switch (ContractType)
            {
                case READ_PARM:
                case READ_VAR:
                    if (pTmpContract->AddXferItem(ContractList[Idx].Symbol, ContractList[Idx].pValue, ContractList[Idx].SizeInBytes) != SUCCESS)
                        ErrorFlag = true;
                    break;
                case WRITE_PARM:
                case WRITE_VAR:
                    if (pTmpContract->AddXferItem(ContractList[Idx].pValue, ContractList[Idx].Symbol, ContractList[Idx].SizeInBytes) != SUCCESS)
                        ErrorFlag = true;
                    break;
                default:
                    ErrorFlag = true;
                    break;
            }
        }

        if (ErrorFlag)
        {
            if (pTmpContract != NULL)
                delete pTmpContract;
            pTmpContract = NULL;
            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                cParamString("CreateContract failed - Type %d  (%s ...)", ContractType, ContractList[0].Symbol.c_str()).c_str()),
                HERE);
        }

        return pTmpContract;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      create and initialize a contract to sample a list of Variables and
    //      write in a CSV file
    //
    ///////////////////////////////////////////////////////////////////////////
    void* MarkStatDLLApi::CreateContract(int ListCount, ContractListType ContractList[], std::string SamplingFileName)
    {
        cDLLContract*   pTmpContract=NULL;
        FILE*           pFile=NULL;
        bool            ErrorFlag = false;

        if (!Initialized)
            Initialize();

        if (SamplingFileName == "")
            ErrorFlag = true;
        else
        {
            pTmpContract = new (std::nothrow) cDLLContract(ListCount);

            pTmpContract->SetType(cDLLContract::VARLOGGER);

            if (pTmpContract->FileOpen(SamplingFileName) != SUCCESS)
                ErrorFlag = true;


            for (int Idx=0; !ErrorFlag && (Idx < ListCount); ++Idx)
            {
                if (pTmpContract->AddSampleItem(ContractList[Idx].Symbol) != SUCCESS)
                    ErrorFlag = true;
            }
        }


        if (ErrorFlag)
        {
            if (pTmpContract != NULL)
                delete pTmpContract;
            pTmpContract = NULL;
            MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                cParamString("CreateContract failed - Sampling  (%s ...)", ContractList[0].Symbol.c_str()).c_str()),
                HERE);
        }

        return pTmpContract;
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      execute the contract as created
    //
    ///////////////////////////////////////////////////////////////////////////
    void MarkStatDLLApi::ExecContract(void* pTmpContract)
    {
        if (pTmpContract == NULL)
        {
            if (!ReportedNullContract)
            {
                MarkStatDiag.LogErrorMessage(TRIP_MSG, cParamString(MKS_DLL_STR,
                    cParamString("Execution of NULL contract is rejected").c_str()),
                    HERE);
            }
            ReportedNullContract = true;

            return;
        }

        ((cDLLContract*)pTmpContract)->ExecContract();
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      run selected rule set
    //
    // RuleSet - rule set identifier
    //
    // return SUCCESS(1) or FAIL(0)
    //
    ///////////////////////////////////////////////////////////////////////////
    int MarkStatDLLApi::RunRule(int RuleSet)
    {
        return RunRuleSet(RuleSet);
    }


    ///////////////////////////////////////////////////////////////////////////
    //
    // DESCRIPTION:
    //      set Event time to new time
    //      this is the time used to stamp alarms and faults
    //
    // NewTime - time to set event time to in sec
    //
    ///////////////////////////////////////////////////////////////////////////
    void  MarkStatDLLApi::SetEventTime(double NewTime)
    {
        XopsSourceIsSysClock = true;

        int NewSec, NewNSec;

        NewSec  = (int)floor(NewTime);
        NewNSec = (int)floor(((NewTime - floor(NewTime)) * 1e9) + 0.5);

        // calculate new offset
        EventTmOffs.Sec  = (int)NewSec  - SysTm1.Sec;
        EventTmOffs.nSec = (int)NewNSec - SysTm1.nSec;

        if (EventTmOffs.nSec < 0)
        {
            EventTmOffs.nSec += 1000000000;
            --EventTmOffs.Sec;
        }
    }

}  // end of namespace

