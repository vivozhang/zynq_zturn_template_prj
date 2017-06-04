#ifndef __DLLINTERFACE_H__
#define __DLLINTERFACE_H__
///////////////////////////////////////////////////////////////////////////////



// Include Files:
//---------------
// system
#include <stdint.h>
#include <string>
// core
// framework



// Constants:
//-----------


// Function Prototypes:
//---------------------


// Types and Classes:
//-------------------
class cDLLContract;

//-----------------------------------------------------------------------------
namespace nsDLLInterface
{
    enum DataTypeEnum
    {
        UNKNOWN     = 0,
        REAL        = 1,
        DINT        = 2,
        UDINT       = 3,
        DWORD       = 4,
        pANY        = 5,
        pREAL       = 6,
        pDINT       = 7,
        pUDINT      = 8,
        FPT         = 1,
        INT         = 2,
        UINT        = 3,
        MEM         = 4,
    };
}  // end of namespace
//-----------------------------------------------------------------------------

struct ParmInfoType
{
    std::string  Symbol;
    unsigned     Index;
    nsDLLInterface::DataTypeEnum DataType;
    std::string  DefaultVal;
} ;

struct VarInfoType
{
    std::string  Symbol;
    unsigned     Index;
    nsDLLInterface::DataTypeEnum DataType;
    int          RdOnly;
} ;


///////////////////////////////////////////////////////////////////////////////
class cDLLInterface
{
public:
    cDLLInterface()   {;}
    ~cDLLInterface()  {;}

    void            Initialize();
    int             SetParmDefaults();
    int             SetParmValue(unsigned ParmIdx, std::string ValueStr);
    int             LoadParameterFile(std::string ParmFileName);
    int             DumpParameterFile(std::string ParmFileName);
    int             SaveSimulationFile(std::string SimFileName);
    int             RestoreSimulationFile(std::string SimFileName);
    ParmInfoType*   FindParmSymbol(std::string Symbol);
    ParmInfoType*   FindParmIndex(unsigned Index);
    VarInfoType*    FindVarSymbol(std::string Symbol);
    VarInfoType*    FindVarIndex(unsigned Index);

private:
    friend class    cDLLContract;

    char*           ValueToString(uint32_t Value, nsDLLInterface::DataTypeEnum DataType);
    uint32_t        StringToValue(std::string ValueStr, nsDLLInterface::DataTypeEnum DataType);

};

///////////////////////////////////////////////////////////////////////////////
class cDLLContract
{
public:
    enum            TypeEnum        { TYPE_UNKNOWN, PARM, VAR, VARLOGGER };
    enum            DirectionEnum   { DIRECTION_UNKNOWN, READ, WRITE };

    cDLLContract(int ListCount);
    ~cDLLContract();

    int             FileOpen(std::string SamplingFileName);
    void            SetType(TypeEnum Type)  {mType = Type;}
    int             AddXferItem(std::string Src, void* Dest,       int SizeInBytes=0);
    int             AddXferItem(void* Src,       std::string Dest, int SizeInBytes=0);
    int             AddSampleItem(std::string Src);
    int             ExecContract();

private:
    int             mCount;
    TypeEnum        mType;
    DirectionEnum   mDirection;
    int             mAddIdx;
    FILE*           pSamplingFile;
    uint32_t**      pSourceList;
    uint32_t**      pDestinationList;
    unsigned*       pSizeInBytes;
    unsigned*       pDataType;
};



///////////////////////////////////////////////////////////////////////////////
#endif  // __DLLINTERFACE_H__
