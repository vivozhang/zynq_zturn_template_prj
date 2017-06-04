///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Parameter interface
//
// DESCRIPTION:
//              provide interface to published parameters for toolbox communication,
//              CRC checking, parameter file updating, final value interface
//
// COPYRIGHT:   Copyright (c) 2002
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#ifndef _PARAMETER_H_
#define _PARAMETER_H_
///////////////////////////////////////////////////////////////////////////////

// Include Files:
//---------------
// system
#include <stdint.h>
// core
#include "MonitorCommand.h"
// framework
#include "x0FrameWork.h"
#include "x0Variable.h"


// Constants:
//-----------


// Function Prototypes:
//---------------------


// Types and Classes:
//-------------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//
// methods common to MarkStat product and DLL
//
///////////////////////////////////////////////////////////////////////////////
class cParameterBase
{

//-----------------------------------------------------------------------------
public:
    cParameterBase();
    ~cParameterBase();

    // Methods
    //--------
    virtual void    Initialize();

    //-------------------------------------------------------------------------
    // Parameter
    int             SetValue(void* pAddr, uint32_t Value);
    uint32_t        GetValue(void* pAddr);
    void*           GetAddress(unsigned Laddr);
    unsigned        LastIndex()                     { return (ParmTblSize-1);}
    unsigned        TotalEntry()                    { return (ParmTblSize+OverrideTblSize);}

    bool            ReadPtrValue(bool* Name)        { return (gVariable.ReadAsBool(Name) != 0);}
    int             ReadPtrValue(int* Name)         { return gVariable.ReadAsInt(Name);}
    long int        ReadPtrValue(long int* Name)    { return gVariable.ReadAsInt(Name);}
    unsigned        ReadPtrValue(unsigned* Name)    { return gVariable.ReadAsInt(Name);}
    unsigned long   ReadPtrValue(unsigned long* Name){ return gVariable.ReadAsInt(Name);}
    float           ReadPtrValue(float* Name)       { return gVariable.ReadAsFloat(Name);}

    //-------------------------------------------------------------------------
    // support for SET_PARM automation
    int             SetParm(float* pAddr, double Value)         {float TempF=Value;
                                                                 return SetValue((void*)pAddr, *((uint32_t*)&TempF));}
    int             SetParm(float* pAddr, float Value)          {return SetValue((void*)pAddr, *((uint32_t*)&Value));}
    int             SetParm(int32_t* pAddr, int32_t Value)      {return SetValue((void*)pAddr, *((uint32_t*)&Value));}
    int             SetParm(int32_t* pAddr, uint32_t Value)     {return SetValue((void*)pAddr, *((uint32_t*)&Value));}
    int             SetParm(uint32_t* pAddr, uint32_t Value)    {return SetValue((void*)pAddr, *((uint32_t*)&Value));}
    int             SetParm(uint32_t* pAddr, int32_t Value)     {return SetValue((void*)pAddr, *((uint32_t*)&Value));}
    int             SetParm(void* pAddr, void* Value)           {return SetValue((void*)pAddr, *((uint32_t*)&Value));}
    int             SetParm(void* pAddr, int32_t Value)         {return SetValue((void*)pAddr, *((uint32_t*)&Value));}    // pointer
                    //****************************************************************************************************
                    // Note:
                    //
                    // with 2 choices for the previous data types the ambiguous error will catch the following problems
                    //
                    // using the following definitions will specify directly the unwanted combinations
                    // however the error will be less specific
                    //
                    //****************************************************************************************************
    int             SetParm(float* pAddr, int Value);           // undesired combination
    int             SetParm(int* pAddr, float Value);           // undesired combination
    int             SetParm(int* pAddr, double Value);          // undesired combination
    int             SetParm(unsigned* pAddr, float Value);      // undesired combination
    int             SetParm(unsigned* pAddr, double Value);     // undesired combination

    //-------------------------------------------------------------------------
    // Final Value
    int             FvInitialize();
    bool            IsFv(void* pAddr);
    bool            IsFvOverridden(void* pAddr);
    bool            IsOverridePresent()             { return OverridePresent;}
    int             FvRegister(void* pAddr);
    int             FvSetOverride(void* pAddr);
    int             FvClrOverride(void* pAddr);


//-----------------------------------------------------------------------------
protected:

    // Methods
    //--------
    void            CreateTables();
    virtual int     SaveSingleToFile(void* pAddr);
    void            ParmReset();

    // Unpublished Variables
    //----------------------
    unsigned        ParmTblSize;            // number of entries in parameter table
    void**          ParmAddrTbl;            // parameter address cross reference table
    unsigned        OverrideTblSize;        // number of entries in final value override table
    uint32_t*       pFvOverrideTbl;         // pointer to final value override table
    uint32_t*       pFvTbl;                 // pointer to final value table
    bool            OverridePresent;        // override present in final value table

};


///////////////////////////////////////////////////////////////////////////////
// methods specific to MarkStat product or DLL
class cParameter : public  cParameterBase
{

//-----------------------------------------------------------------------------
public:
    cParameter();
    ~cParameter()   {;}

    // Methods
    //--------
    void            Initialize();
    uint32_t        ParmFvCrc();
    int             CrcCheck(int ReCalcCrc);
    void            FileCrcCheck();
    void            SetParmFile(const char* pFileName)     {pParmFile = pFileName;}

    int             Monitor(cMonitorCommand &gMonitor);

//-----------------------------------------------------------------------------
protected:

    // Methods
    //--------
    int             SaveSingleToFile(void* pAddr);
    int             SaveAllToFile(void* pParm);
    static int      MonStatus(cParamString &OutputBuffer, cParamString &CommandLineArgs);

    // Unpublished Variables
    //----------------------
    static void*    DefParmAddrTbl[];          // definition of parameter address cross reference table
    const  char*    pParmFile;

};


///////////////////////////////////////////////////////////////////////////////
#endif  // _PARAMETER_H_

