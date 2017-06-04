///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Bridge Interface Virtual Class
//
// DESCRIPTION:
//
//  proposal ...
//  {
//
//      WindMV interface is PhsCmds
//      {
//          async mod natively outputs PhsCmds
//          bridge test natively outputs PhsCmds
//
//          thermal model inputs PhsCmds
//          REBI bridge interface inputs PhsCmds and translates to BrgCmds (TRANS***)
//
//          simulator translates BrgCmds to PhsCmds (TRANS***)
//      }
//
//      ECD01 interface is BrgCmds
//      {
//          async mod natively generates PhsCmds and translates to BrgCmds (TRANS***)
//          sync mod outputs PhsCmds
//          bridge test outputs PhsCmds (TRANS***)
//
//          thermal model inputs PhsCmds (???***)
//          ECGI bridge interface inputs BrgCmds
//
//          simulator translates BrgCmds to PhsCmds (TRANS***)
//      }
//
//  }
//
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////
#ifndef X0_BRIDGE_INTERFACE_H
#define X0_BRIDGE_INTERFACE_H


// Include Files
//--------------
// system
// core
#include "ParamString.h"
// framework
#include "x0FrameWork.h"


// Constants
//----------
#define MAX_NUM_PHSTRANS    (5)                 // Number of supported phase transitions per frame,
                                                // some software components could accept 2 and/or 3.

// 3 phase system definitions
#define NUM_OF_PHASES       (3)                 // number of Ac bridge phases
#define PHASE_A             (0)                 // select phase A
#define PHASE_B             (1)                 // select phase B
#define PHASE_C             (2)                 // select phase C
#define NUM_OF_CHOP_PHS     (1)                 // number of Dc chopper phases


// Types and Classes
//------------------

//-----------------------------------------------------------------------------
// bridge phase-terminal states
enum PhsStateTyp
{
    PHS_ST_OFF = 0,
    PHS_ST_NEG = 1,
    PHS_ST_POS = 2,
    PHS_ST_MID = 3
};


//-----------------------------------------------------------------------------
// bridge test control structures
//  (used for cell test, pulse test)
enum BrgTstSrsCtlTyp
{
    BRG_TST_SERIES_FORCE_NONE,                  // force nothing
    BRG_TST_SERIES_FORCE_ABC_S1,                // force series cell position 1 of 3-phase bridge (phase ABC)
    BRG_TST_SERIES_FORCE_ABC_S2,                // force series cell position 2 of 3-phase bridge (phase ABC)
    BRG_TST_SERIES_FORCE_DE_S1,                 // force series cell position 1 of voltage and current crowbar (phase DE)
    BRG_TST_SERIES_FORCE_DE_S2,                 // force series cell position 2 of voltage and current crowbar (phase DE)
};
enum BrgTstCntrlTyp
{
    BRG_TST_ENB_ALL,                            // enable all (1,2,3,4)
    BRG_TST_ENB_UPR,                            // enable upper (1,2)
    BRG_TST_ENB_LWR,                            // enable lower (3,4)
    BRG_TST_ENB_OTR,                            // enable outer (1,4)
    BRG_TST_ENB_INR,                            // enable inner (2,3)
};
struct BrgTstCtlTyp
{
    unsigned char   BrgTestReq;                 // request for bridge test mode
    unsigned char   EnabPhCells;                // Enable phase-cells to be tested
    unsigned char   DisabMask;                  // disable applying a Mask to normal gating
    unsigned char   DisabFltGateDrv;            // prevent any gate driver fault from changing bridge status

    BrgTstSrsCtlTyp SeriesForceCtl;             // series switch forcing control

    unsigned char   GateEnabA1;                 // individual switch enables for phase A
    unsigned char   GateEnabA2;
    unsigned char   GateEnabA3;
    unsigned char   GateEnabA4;

    unsigned char   GateEnabB1;                 // individual switch enables for phase B
    unsigned char   GateEnabB2;
    unsigned char   GateEnabB3;
    unsigned char   GateEnabB4;

    unsigned char   GateEnabC1;                 // individual switch enables for phase C
    unsigned char   GateEnabC2;
    unsigned char   GateEnabC3;
    unsigned char   GateEnabC4;

    unsigned char   GateEnabDb1;                // individual switch enables for db / crowbar
    unsigned char   GateEnabDb2;

    BrgTstCntrlTyp  CntrlPhsA;
    BrgTstCntrlTyp  CntrlPhsB;
    BrgTstCntrlTyp  CntrlPhsC;
};


//-----------------------------------------------------------------------------
// phase transition structures
//  used as modulator outputs, inputs to the bridge interface class
//  transition [0] is assumed to define the initial state for the frame
//  although Enable and DelTm fields are not used, the modulator should set Enable=true and DelTm=0
struct PhsTransTyp
{
    unsigned        DelTm;                      // transition delta time from the load pulse (frc counts)
    PhsStateTyp     State;                      // new state of the phase
    unsigned        Enable;                     // true to enable this transition
};
struct PhsTransListTyp
{
    unsigned        Len;                        // length of the phase transition array
    PhsTransTyp     (*pTrans)[NUM_OF_PHASES];   // pointer to a 2-dim array of phase transitions [trans][phase]
    BrgTstCtlTyp*   pBrgTstCtl;                 // pointer to bridge test control
};


//-----------------------------------------------------------------------------
// bridge transition structures
//  (used to hold transition commands for output to the bridge gating FPGA)
struct BrgTransTyp
{
    unsigned        DelTm;                      // transition delta time from the load pulse (frc counts)
    PhsStateTyp     State[NUM_OF_PHASES];       // new state of the bridge (1-dim array of phase states [phase])
    unsigned        Enable;                     // = true to enable this transition
};
struct BrgTransListTyp
{
    unsigned        Len;                        // length of the bridge transition array
    BrgTransTyp     (*pTrans)[];                // pointer to a 1-dim array of bridge transitions [trans]
};


//-----------------------------------------------------------------------------
// sinc feedback structures
struct SncSampTyp
{
    float           Ia;
    float           Ib;
    float           Ic;
    float           Va;
    float           Vb;
    float           Vc;
};
struct SncSampLstTyp
{
    unsigned        Len;                        // length of the sinc sample array
    unsigned        Frc;                        // frc time of the first sample
    unsigned        FrcDel;                     // frc delta time between samples
    SncSampTyp      (*pSamp)[];                 // pointer to an array of sinc samples
};

//-----------------------------------------------------------------------------
// bridge desaturation and over-current reporting structure
struct BrgDsatIocTyp
{
    unsigned  DsatSwAL;        // Desat fault in A lower switch
    unsigned  DsatSwBL;        // Desat fault in B lower switch
    unsigned  DsatSwCL;        // Desat fault in C lower switch
    unsigned  DsatSwAU;        // Desat fault in A upper switch
    unsigned  DsatSwBU;        // Desat fault in B upper switch
    unsigned  DsatSwCU;        // Desat fault in C upper switch
    unsigned  DsatSwD;         // Desat fault in D upper or lower switch
    unsigned  IocPhsA;         // IOC fault in phase A shunt
    unsigned  IocPhsB;         // IOC fault in phase B shunt
    unsigned  IocPhsC;         // IOC fault in phase C shunt
    unsigned  IocPhsD;         // IOC fault in phase D shunt
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
//      The class provides methods for translating between two different ways of representing bridge commands:
//          - phase transition lists
//          - bridge transition lists
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cBridgeInterface
{
public:

    int XlatePhs2BrgTrans ( PhsTransListTyp PhsTransList, BrgTransListTyp BrgTransList );
    int XlateBrg2PhsTrans ( BrgTransListTyp PhsTransLst,  PhsTransListTyp BrgTransList );

private:

    // phase transition sort structure
    //  (used to sort all the phase transitions by time)
    struct PhsTransSortTyp
    {
        unsigned        DelTm;                  // transition delta time from the load pulse (frc counts)
        PhsStateTyp     State;                  // new state of the phase
        unsigned        Phase;                  // phase (PHASE_A,B,C)
    };

    struct CurrPhsInfoTyp
    {
        unsigned        Trans;                  // transition number for the phase
        PhsStateTyp     State;                  // new state of the phase
    };

    PhsTransSortTyp     PhsTransSortArr[24];    // arbitrary length > typical bridge fpga that handles 12
    CurrPhsInfoTyp      CurrPhsInfo[NUM_OF_PHASES];

};



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
//      A quicksort algorithm is used to order the the bridge gate command
//      list in ascending time order.  Time is assumed to be a delta time
//      from a datum such as start-of-frame so as to avoid complexities
//      required to deal with fixed length counter wrap-around.

//      The basic quicksort algorithm and following descriptions
//      were copied from the following:
//          http://www.algolist.net/Algorithms/Sorting/Quicksort
//
//      A recursive divide-and-conquer strategy is used in quicksort:
//
//        - Choose a pivot value
//          We take the value of the middle element as pivot value, but it can
//          be any value, which is in range of sorted values, even if it doesn't
//          present in the array.
//
//        - Partition
//          Rearrange elements in such a way, that all elements
//          which are lesser than the pivot go to the left part of the array
//          and all elements greater than the pivot, go to the right part
//          of the array. Values equal to the pivot can stay in any part of
//          the array. Notice, that array may be divided in non-equal parts.
//
//        - Sort both parts
//          Apply quicksort algorithm recursively to the left and the right
//          parts.
//
//      Algorithm in detail:
//          There are two indices i and j and at the very beginning of the
//          partition algorithm i points to the first element in the array
//          and j points to the last one.  Then algorithm moves i forward,
//          until an element with value greater or equal to the pivot is
//          found. Index j is moved backward, until an element with value
//          lesser or equal to the pivot is found. If i <= j then they are
//          swapped and i steps to the next position (i + 1), j steps to
//          the previous one (j - 1). Algorithm stops, when i becomes
//          greater than j.  After partition, all values before i-th element
//          are less or equal than the pivot and all values after j-th
//          element are greater or equal to the pivot.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
/////////////////////////////////////////////////////////////////////////////
template <class X> void SortArray
(
    X   arr[],
    int left,
    int right
)
{
    X               tmp;
    int             i = left;
    int             j = right;
    unsigned        pivot = arr[(left+right)/2].DelTm;

    //-------------------------------------------------------------------------
    // partition
    while ( i <= j )
    {
        while ( arr[i].DelTm < pivot )
        {
            i++;
        }
        while ( arr[j].DelTm > pivot )
        {
            j--;
        }
        if ( i <= j )
        {
            // swap the array elements
            tmp    = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    };

    //-------------------------------------------------------------------------
    // recursion
    if ( left < j )
    {
        SortArray( arr, left, j );
    }
    if ( i < right )
    {
        SortArray( arr, i, right );
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
//    This class provides methods and data members for monitoring phase transition commands via low-level-monitor command 'tran'.
//    The 'TranCapture()' method will capture all phase transitions when a gating edge is detected on any bridge phase;
//    a gating edge is defined as a change of output terminal voltage between positive and negative rails or vice versa.
//    Up to four sets of phase transitions can be permanently captured; storage buffers can be reset with a -r switch.
//    Capture action can occur anywhere in the code, as long as a phase-transition-list structure is within scope.
//    Following line of code must be added in the code location where data capture is desired:
//                  cTransitionMonitor::TranCapture( <PhsTransListTyp>.pTrans );
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cTransitionMonitor
{
//-----------------------------------------------------------------------------
public:
    // Methods
    //--------
    static  int TranLlm ( cParamString &OutputBuffer, cParamString &CommandLineArgs );
    static  void TranCapture ( PhsTransTyp TranArr[][NUM_OF_PHASES] );

    // Output Signals
    //---------------

//-----------------------------------------------------------------------------
private:
    static  PhsTransTyp TranLlmArr1[MAX_NUM_PHSTRANS][NUM_OF_PHASES];
    static  PhsTransTyp TranLlmArr2[MAX_NUM_PHSTRANS][NUM_OF_PHASES];
    static  PhsTransTyp TranLlmArr3[MAX_NUM_PHSTRANS][NUM_OF_PHASES];
    static  PhsTransTyp TranLlmArr4[MAX_NUM_PHSTRANS][NUM_OF_PHASES];
    static  PhsStateTyp LastTran[NUM_OF_PHASES];

    static  int  TranArrEmpty;      // number of array buffer(s) that remain empty

    // External Inputs
    //----------------

    // Methods
    //--------
    static  void TranReset ( void );

    // Diagnostics
    //------------

    // Parameters
    //-----------

    // Published Variables
    //--------------------

    // Unpublished Variables
    //----------------------

    // Data Passing
    //-------------

};


#endif  // X0_BRIDGE_INTERFACE_H
