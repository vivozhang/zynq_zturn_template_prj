#ifndef __MARKSTATDLL_H__
#define __MARKSTATDLL_H__
///////////////////////////////////////////////////////////////////////////////



// Defines the exported functions for the DLL application.

#ifdef MARKSTATDLL_EXPORTS
#define MARKSTATDLL_API __declspec(dllexport)
#else
#define MARKSTATDLL_API __declspec(dllimport)
#endif


// Include Files:
//---------------
#include <string>



namespace MarkStatDLL
{
    struct ContractListType
    {
        std::string Symbol;
        void*       pValue;
        int         SizeInBytes;
    };

    enum ContractEnum
    {
        UNKNOWN = 0,
        READ_PARM,
        WRITE_PARM,
        READ_VAR,
        WRITE_VAR,
    };

    class MarkStatDLLApi
    {
    public:
        static MARKSTATDLL_API char* GetApiRevision();
        static MARKSTATDLL_API char* GetProductRevision();
        static MARKSTATDLL_API void  Initialize(std::string UserParmFileName="");
        static MARKSTATDLL_API int   LoadParameters(std::string UserParmFileName);
        static MARKSTATDLL_API int   DumpParameters(std::string UserParmFileName="");
        static MARKSTATDLL_API int   SaveSimulation(std::string SimFileName);
        static MARKSTATDLL_API int   RestoreSimulation(std::string SimFileName);
        static MARKSTATDLL_API int   ResetParameter(std::string ParmName);
        static MARKSTATDLL_API void  Step();
        static MARKSTATDLL_API void* CreateContract(int ListCount, ContractListType ContractList[], ContractEnum ContractType);
        static MARKSTATDLL_API void* CreateContract(int ListCount, ContractListType ContractList[], std::string SamplingFileName);
        static MARKSTATDLL_API void  ExecContract(void*);
        static MARKSTATDLL_API int   RunRule(int RuleSet=1);
        static MARKSTATDLL_API void  SetEventTime(double NewTime);
    };
}  // end of namespace





///////////////////////////////////////////////////////////////////////////////
#endif  // __MARKSTATDLL_H__
