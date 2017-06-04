///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Inter-Task Data Passer Class
//
// DESCRIPTION:
//      This file contains the definition for the Data Passer class.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      x0DataPass.vsd
//
///////////////////////////////////////////////////////////////////////////////

#ifndef X0_DATAPASS_H
#define X0_DATAPASS_H


// Includes
//---------
// system
#include <stdint.h>
#include <stdio.h>


// Externals:
//-----------


// Macros:
//--------


// Types , Enumerations, and Classes:
//----------------------------------
enum   TaskPairEnum
{
    T12   =  0,            // must start at 0 and consecutively increase
    T13   =  1,            // data-flow from Task1 to Task3 unfiltered
    T1A2  =  2,            // data-flow from Task1A to Task2 unfiltered
    T1A3  =  3,            // data-flow from Task1A to Task3 unfiltered
    T21   =  4,            // data-flow from Task2 to Task1 (combined task 1 and 1A signals)
    T23   =  5,            // data-flow from Task2 to Task3 unfiltered
    T31   =  6,            // data-flow from Task3 to Task1 (combined task 1 and 1A signals)
    T32   =  7,            // etc...
    T12F  =  8,            // data-flow form Task1 to Task2 filtered
    T13F  =  9,            // data-flow form Task1 to Task3 filtered
    T1A2F = 10,            // data-flow form Task1A to Task2 filtered
    T1A3F = 11,            // data-flow form Task1A to Task3 filtered
    T23F  = 12,            // data-flow form Task2 to Task3 filtered
    NumOfTaskPairs = 13    // must be last and equal the number of task-pairs
};

struct DataPassItemTyp
{
    const void*             pSrc;           // pointer to constant Source variable
    void*                   pDst;           // pointer to Destination variable
    unsigned                Size;           // size of the variable (bytes)
    float                   Accum;          // accumulator for variable filter
};

struct DataPassInfoTyp
{
    DataPassItemTyp*        pItemArr;       // pointer to the array of items
    unsigned                NumOfItems;     // number of items in the array
    int                     FilterCnt;      // filter count (used only for
};                                          // float-passes from faster to slower tasks)


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      The data passer class provides pointer arrays and data passing
//      methods to accomplish coherent transfer of variables between
//      a pair of tasks executing at different integral rates.The scope
//      of data passing operations is either global or object contained,
//      i.e., between global variables and between class members of the
//      same class instance. Variables being exchanged without filtering
//      can be of any fundamental (int, float, etc.) or compound
//      (array, structure, etc.) data-type. Variable acting as source
//      of data can have the 'Const' qualifier, while variable receiving
//      data cannot. Inter-task filtering is only defined for float
//      variables moving from fast to slow tasks. Data pass entries can
//      be created any where in the code with the following statement:
//
//      DATA_PASS(Src Var-Name, Dst Var-Name, Task-Pair, Filter String)
//
//    Where: Var-Name can be a global name or a class member reference
//           in the form of ClassInstance.mVar .
//           Task-Pair is any combination of source and destination tasks,
//           indicated by these pre-defined names: T1_T2, T2_T3, T1A_T2,
//           T1_T3, T1A_T3, T3_T2, T2_T1, T3_T1, T1A_T2, and T1A_T3.
//           Meaning Source_Destination, and using T1 and T1A to denote two
//           Task1s running at different rates when the product requires it.
//           T1 is faster than T1A by convention.
//           Data pass entries including 'T1A' pre-defined names can only be
//           used outside class definitions, generic 'T1' must always be
//           used inside class definitions.
//           Filter String is 'Filtered' for filtered operations, anything
//           else results on unfiltered operations.
//
//      There are two specific cases where data-passing cannot be carried
//      out without the help of intermediate storage: passing data directly
//      from a class input (In or Ins structures), and passing data directly
//      to a class output (identified by the OUT macro). If needed, these
//      operations can be performed by copying data to a temporary private
//      variable before or after the actual data-passing, depending on the
//      case being handled.
//
//      The DATA_PASS statement does not define variables; it merely
//      allocates memory in the pointer arrays for source and destination
//      pointers. Variables must be already defined in the code.
//
///////////////////////////////////////////////////////////////////////////////
class cDataPasser
{

//-----------------------------------------------------------------------------
public:
    cDataPasser();
    ~cDataPasser();

    // Methods
    //--------
    void Dispatch(TaskPairEnum TaskPair);       // transfer data
    void AccumFil(TaskPairEnum TaskPair);       // accumulate data for filter

    // Output Signals
    //---------------

//-----------------------------------------------------------------------------
private:
    // External Interfaces
    //--------------------

    // Methods
    //--------

    // Diagnostics
    //------------

    // Parameters
    //-----------

    // Published Variables
    //--------------------

    // Unpublished Variables
    //----------------------
    static DataPassItemTyp T12_DataPassItems[];   // Array of unfiltered items flowing from Task1 to Task2
    static DataPassItemTyp T13_DataPassItems[];   // Array of unfiltered items flowing from Task1 to Task3
    static DataPassItemTyp T1A2_DataPassItems[];  // Array of unfiltered items flowing from Task1A to Task2
    static DataPassItemTyp T1A3_DataPassItems[];  // Array of unfiltered items flowing from Task1A to Task3
    static DataPassItemTyp T21_DataPassItems[];   // Array of unfiltered items flowing from Task2 to Task1 and Task1A
    static DataPassItemTyp T23_DataPassItems[];   // Array of unfiltered items flowing from Task2 to Task3
    static DataPassItemTyp T31_DataPassItems[];   // etc...
    static DataPassItemTyp T32_DataPassItems[];
    static DataPassItemTyp T12F_DataPassItems[];  // Array of filtered items flowing from Task1 to Task2
    static DataPassItemTyp T13F_DataPassItems[];  // Array of filtered items flowing from Task1 to Task3
    static DataPassItemTyp T1A2F_DataPassItems[]; // Array of filtered items flowing from Task1A to Task2
    static DataPassItemTyp T1A3F_DataPassItems[]; // Array of filtered items flowing from Task1A to Task3
    static DataPassItemTyp T23F_DataPassItems[];  // Array of filtered items flowing from Task2 to Task3

    static DataPassInfoTyp DataPassInfo[NumOfTaskPairs];

    // Data Passing
    //-------------

};


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   Transference of data between source and destination pointers. All
//   variables requiring filtering are averaged in a sequencial loop, while
//   the non-filtered are copied as blocks of memory.
//
// EXECUTION LEVEL
//      Task1 and task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
inline void cDataPasser::Dispatch(TaskPairEnum TaskPair)
{
    unsigned i;
    float    FilterCntRcp;

    if ( DataPassInfo[TaskPair].FilterCnt > 1 )
    {
        FilterCntRcp = 1.0F / DataPassInfo[TaskPair].FilterCnt;
        for ( i = 0; i < DataPassInfo[TaskPair].NumOfItems; i++ )
        {
            *(float*)DataPassInfo[TaskPair].pItemArr[i].pDst = DataPassInfo[TaskPair].pItemArr[i].Accum * FilterCntRcp;
        }
        DataPassInfo[TaskPair].FilterCnt = 0;
    }
    else
    {
        for ( i = 0; i < DataPassInfo[TaskPair].NumOfItems; i++ )
        {
            *(uint32_t*)DataPassInfo[TaskPair].pItemArr[i].pDst = *(uint32_t*)DataPassInfo[TaskPair].pItemArr[i].pSrc;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   Update accumulators for variables requiring filtering between tasks.
//
// EXECUTION LEVEL
//      Task1 and task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
inline void cDataPasser::AccumFil(TaskPairEnum TaskPair)
{
    unsigned i;

    if ( DataPassInfo[TaskPair].FilterCnt == 0 )
    {
        for ( i = 0; i < DataPassInfo[TaskPair].NumOfItems; i++ )
        {
            DataPassInfo[TaskPair].pItemArr[i].Accum = *(float*)DataPassInfo[TaskPair].pItemArr[i].pSrc;
        }
        ++DataPassInfo[TaskPair].FilterCnt;
    }
    else
    {
        for ( i = 0; i < DataPassInfo[TaskPair].NumOfItems; i++ )
        {
            DataPassInfo[TaskPair].pItemArr[i].Accum += *(float*)DataPassInfo[TaskPair].pItemArr[i].pSrc;
        }
        ++DataPassInfo[TaskPair].FilterCnt;
    }
}

#endif  // X0_DATAPASS_H
