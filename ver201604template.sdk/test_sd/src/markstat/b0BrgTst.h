///////////////////////////////////////////////////////////////////////////////
//
// TITLE        3-level Bridge Test base class
//
// DESCRIPTION
//      This file defines the 3-level bridge test inheritance hierarchy.
//      This hierarchy is headed by a test base class that derives into
//      two concrete classes, the bridge pulse and cell tests.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0BrgTst.vsd
//
///////////////////////////////////////////////////////////////////////////////

#ifndef B0_BRGTST_H
#define B0_BRGTST_H


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product
#include "b0bridgeRebf.h"



// Constants
//----------
#define  BRGTST_NUM_OF_TRA_PER_PHS   (3)  // number of transitions per phase including init state
                                          // that are used in bridge-test class and all its derived

#define  PHS_ENUM_MIN                (0)  // From none(0) to PHS_ABC(7)
#define  PHS_ENUM_MAX                (7)

#define  ST_ENUM_MIN                 (0)  // From BrgStateTyp ( OFF(0), NEG(1), POS(2) )
#define  ST_ENUM_MAX                 (2)

// Bridge phase control
#define  PHS_0    0u
#define  PHS_A    1u
#define  PHS_B    2u
#define  PHS_C    4u
#define  PHS_AB   3u
#define  PHS_BC   6u
#define  PHS_CA   5u
#define  PHS_ABC  7u

// Task 1 state machine states
//#define  FOT_00_T1      50    Needed for FOT
//#define  FOT_01_T1      51
//#define  FOT_02_T1      52
//#define  FOT_03_T1      53
//#define  FOT_04_T1      54
//#define  FOT_05_T1      55
//#define  BLINK_00_T1   300    Needed for FOT
//#define  BLINK_01_T1   301
//#define  BLINK_02_T1   302
//#define  BLINK_03_T1   303
//#define  BLINK_04_T1   304
//#define  BRG_RST_0_T1  400    Needed for FOT
//#define  BRG_RST_1_T1  401
//#define  BRG_RST_2_T1  402
//#define  BRG_RST_3_T1  403

// Task 2 state machine states


// Function Prototypes
//--------------------


// Types and Classes
//------------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      2-level Bridge Test - Indirect Base class
//
///////////////////////////////////////////////////////////////////////////////
class cBridgeTest
{

//-----------------------------------------------------------------------------
public:
    // Types
    //------
    // In data-structure is defined by derived class
    struct InsListTyp
    {
        const float&    DelTm1;         //  1 - Delta time for task 1 (Sec)
        const float&    DelTm2;         //  2 - Delta time for task 2 (Sec)
        const float&    DelTm1Rcp;      //  3 - Delta time reciprocal for task 1 (1/Sec)
        const unsigned& PwmQuant;       //  4 - Number of Pwm Timer counts in a pulse quanta
        const unsigned& UFull;          //  5 - Total number of pulse quanta per scheduling
        const unsigned& PwmFull;        //  6 - Total number of clock pulses per scheduling
        const float&    DelTimeQuant;   //  7 - Delta time in a quant (Sec)
        const float&    IshuntRat;      //  8 - Shunt rated current (Arms)
        const float&    IsensorScl;     //  9 - Phase current sensor (lem/shunt) scale (Amp/Hz)
        const float&    CellTestL;      // 10 - Total inductance seen between two bridge terminals (Henry)
        const float&    CellTestC;      // 11 - Total capacitance seen between two bridge terminals (Farad)
        const float&    CellTestCurLvl; // 12 - Target current for bridge test (Amps)
        const float&    CellTestVllNom; // 13 - Nominal bridge line to line voltage (Volts)
        const float&    CellTestAltL;   // 14 - Alternate inductance seen during cell test (Henry)
        const float&    BrgLockoutTm;   // 15 - Bridge lockout time
    };

    // Methods
    //--------
    cBridgeTest( InsListTyp InsList );
    virtual ~cBridgeTest();

    virtual void Task1(void)    = 0;
    virtual void Task2(void)    = 0;

    // Output Signals
    //---------------                            //  All Task1 outputs
    PhsTransListTyp PhsTransList;                                           // list of setting for phase transitions
    PhsTransTyp     PhsTransArr[BRGTST_NUM_OF_TRA_PER_PHS][NUM_OF_PHASES];  // 2-dim array holding bridge phase transitions
    BrgTstCtlTyp    BrgTestControl;                                         // bridge test structure to access individual switches


//-----------------------------------------------------------------------------
protected:

    // Types
    //------
    struct PrkPulStatesTyp
    {
        PhsStateTyp     ParkStA;
        PhsStateTyp     ParkStB;
        PhsStateTyp     ParkStC;
        PhsStateTyp     PulseStA;
        PhsStateTyp     PulseStB;
        PhsStateTyp     PulseStC;
    };

    // External Inputs
    //----------------
    InsListTyp   Ins;

    // Un-published Variables
    //-----------------------
    PrkPulStatesTyp     PhsTstCtrl;         // States controlling pulse and park events

    // Methods
    //--------
    unsigned BridgeCfg( unsigned ParkPhs, PhsStateTyp ParkSt, unsigned PulsePhs, PhsStateTyp PulseSt );
    void     BridgeOff( unsigned& TstBridgeOff );
    void     BridgePark( unsigned& TstBridgeOff );
    void     BridgePulseFrac1( int PulseTmr, unsigned& TstBridgeOff );
    void     BridgePulseFrac2( int PulseTmr1, int PulseTmr2, unsigned& TstBridgeOff );
    void     BridgePulseFrac3( int PulseTmr, unsigned& TstBridgeOff );
    void     BridgePulseFull( unsigned& TstBridgeOff );

};

#endif  // B0_BRGTST_H
