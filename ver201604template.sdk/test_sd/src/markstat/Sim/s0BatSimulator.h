//////////////////////////////////////////////////////////////////////////////
//
// TITLE        <class title>
//
// DESCRIPTION
//      This file contains the definition for the class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////

#ifndef S0_BAT_SIMULATOR_H
#define S0_BAT_SIMULATOR_H


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"


// Constants
//----------


// Function Prototypes
//--------------------


// Types and Classes
//------------------
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <class description>
//
///////////////////////////////////////////////////////////////////////////////
class cBatSimulator
{
//-----------------------------------------------------------------------------
public:
    // Types
    //------
    struct InTyp
    {
        const unsigned&      SimEnb;             // 1-Battery simulator enable
        const unsigned&      ChgSwEnb;           // 2-Enable charge igbt
        const unsigned&      DisSwEnb;           // 3-Enable discharge igbt
        const float&         DutyRef;            // 4-Battery chopper duty reference
        const float&         VdcFbk;             // 5-DC link voltage (Volt)
    };

    struct InsTyp
    {
        const float&         DelTm1;             // 1-task1 cycle time (Sec)
        const float&         DelTm1Rcp;          // 2-task 1 execution reciprocal
        const float&         Rbat;               // 3-Battery internal resistance (ohms)
        const float&         Lchpr;              // 4-Chopper inductor
        const float&         Cchpr;              // 5-Chopper capacitor
    };



    cBatSimulator( InTyp InList, InsTyp InsList );
    ~cBatSimulator();

    // Methods
    //--------
    int  RunRules            ( void );
    void Task1(void);

    // Output Signals
    //---------------

    CREATE_PUBVAR(BatIchprSim,          float);      // Simulated chopper current (AMP)
    CREATE_PUBVAR(BatVchprSim,          float);      // Simulated chopper voltage (Volt)

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;

    // External Inputs
    //----------------
    InTyp    In;
    InsTyp   Ins;

    // Methods
    //--------
    void    BatSimulator ( void );

    // Diagnostics
    //------------

    // Parameters
    //-----------

    CREATE_PARM(BatIdscSimEnb,     unsigned);    // Discontinuous current simulation enable
    CREATE_PARM(BatContSim,        unsigned);    // Battery connected
    CREATE_PARM(BatRbatSim,        float);       // Battery internal resistance (ohms)
    CREATE_PARM(BatEbatSim,        float);       // Battery emf (volts)
    CREATE_PARM(BatLchprSim,       float);       // Chopper inductor
    CREATE_PARM(BatCchprSim,       float);       // Film capacitor accorss battery bank
    CREATE_PARM(BatCchprRSim,      float);       // Capacitor resistor

    // Published Variables
    //--------------------

    CREATE_PUBVAR(BatSimEnb,        unsigned);    // Battery simulator enable
    CREATE_PUBVAR(BatVcSim,         float);       // Simulated film capacitor voltage
    CREATE_PUBVAR(BatIcSim,         float);       // Simulated film capacitor current
    CREATE_PUBVAR(BatIbatSim,       float);       // Simulated battery current
    CREATE_PUBVAR(BatVbatSim,       float);       // Simulated battery voltage

    CREATE_PUBVAR(BatChgSwEnbSim,   unsigned);      // Simulated charge switch (igbt) enabled
    CREATE_PUBVAR(BatDisSwEnbSim,   unsigned);      // Simulated discharge switch (igbt) enabled
    CREATE_PUBVAR(BatIchprIsDscSim, unsigned);      // Simulated chopper cur is discontinous boolean
    CREATE_PUBVAR(BatDutySim,          float);      // Simulated chopper duty cycle
    CREATE_PUBVAR(BatVdcVbatDifSim,    float);      // Simulated voltage delta dc link - battery (filter cap)
    CREATE_PUBVAR(BatIdscMaxSim,       float);      // Simulated maximum discontinous current level
    CREATE_PUBVAR(BatIdscPlsTmMaxSim,  float);      // Simulated max pulse time for disc. current
    CREATE_PUBVAR(BatIdscUcmdLimSim,   float);      // Simulated U cmd limit for disc. current
    CREATE_PUBVAR(BatVlPlsSim,         float);      // Simulated voltage across inductor during pulse
    CREATE_PUBVAR(BatVlDcySim,         float);      // Simulated voltage across inductor during decay
    CREATE_PUBVAR(BatPlsTmSim,         float);      // Simulated pulse time
    CREATE_PUBVAR(BatDcyTmSim,         float);      // Simulated decay time (after pulse)
    CREATE_PUBVAR(BatIdscPkSim,        float);      // Simulated peak current when discontinuous

    // Unpublished Variables
    //----------------------
    float BatVdcSim;
    float BatRbatSimRcp;
    float BatCchprSimRcpDt;
    float BatLchprSimRcpDt;

    // Data Passing
    //-------------

};



// Inline Function Definitions
//----------------------------


#endif  // S0_BAT_SIMULATOR_H
