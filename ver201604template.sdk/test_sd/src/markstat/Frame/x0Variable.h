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

///////////////////////////////////////////////////////////////////////////////
#ifndef _VARIABLE_H_
#define _VARIABLE_H_
///////////////////////////////////////////////////////////////////////////////

// Include Files:
//---------------


// Constants:
//-----------

// ReadType function definitions
#define LA_INVLD_TYPE   -1
#define LA_BOOL_TYPE    1
#define LA_INT_TYPE     2
#define LA_FLOAT_TYPE   3
#define LA_MEM_TYPE     4
#define LA_MEMWRD_TYPE  4


// Function Prototypes:
//---------------------


// Types and Classes:
//-------------------

//-----------------------------------------------------------------------------
namespace nsVariable
{
    enum DataTypeEnum
    {
        UNKNOWN     = 0,
        BLN         = 0x0001,
        INT         = 0x0002,
        UINT        = 0x0004,
        FPT         = 0x0008,
        MEM         = 0x0010,
    };
}  // end of namespace
//-----------------------------------------------------------------------------

struct VarAddrTbl_Typ
{
    void*           Addr;
    nsVariable::DataTypeEnum DataType;
};


///////////////////////////////////////////////////////////////////////////////
// methods common to MarkStat product and DLL
class cVariableBase
{
public:
    cVariableBase();
    ~cVariableBase();

    unsigned        TotalEntry()                    { return (mVarTblSize);}
    void*           GetAddress(unsigned Laddr);

    int             ReadType(void* Addr);
    unsigned        ReadAsBool(void* Addr);
    int             ReadAsInt(void* Addr);
    float           ReadAsFloat(void* Addr);

protected:
    void            CreateTables();
    unsigned        GetDataType(unsigned Laddr);

    unsigned                mVarTblSize;        // number of entries in variable table
    VarAddrTbl_Typ*         mVarAddrTbl;        // variable address and data type cross reference table

};


///////////////////////////////////////////////////////////////////////////////
// methods specific to MarkStat product or DLL
class cVariable : public  cVariableBase
{
public:
    cVariable();
    ~cVariable()   {;}

protected:
    static VarAddrTbl_Typ   mDefVarAddrTbl[];      // definition of variable address and data type cross reference table
};


extern cVariable       gVariable;


///////////////////////////////////////////////////////////////////////////////
#endif  // _VARIABLE_H_

