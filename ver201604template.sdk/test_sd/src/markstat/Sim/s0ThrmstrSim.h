//////////////////////////////////////////////////////////////////////////////
//
// TITLE        Thermistor Simulator
//
// DESCRIPTION
//              Constants, function prototypes for thermistor simulation.
//
// COPYRIGHT    Copyright (c) 2015
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      s0ThrmstrSim.xls, s0ThrmstrSim.vsd
//
///////////////////////////////////////////////////////////////////////////////

#ifndef S0_THERMIS_SIMULATOR_H
#define S0_THERMIS_SIMULATOR_H


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product


// Constants
//----------


// Function Prototypes
//--------------------


// Types and Classes
//------------------
///////////////////////////////////////////////////////////////////////////////
//
// CLASS DESCRIPTION:
//      Thermistor simulation for bridge (IBGT) and IO (AEAD) thermistors.
//
//
///////////////////////////////////////////////////////////////////////////////
class cThrmstrSim
{
//-----------------------------------------------------------------------------
public:

    // Enumerations
    //-------------

    // Types
    //------
    struct InTyp
    {
        const volatile int& T1wthnT3;    // 1-T1 state within a T3 frame
        const float&        CoolntTemp;  // 2-Coolant temp (degC)
        const float&        BpTemp;      // 3-Baseplate temp (degC)
    };

    struct InsTyp
    {
        const float&  VcoFrqZeroVolt;    // 1-Vco frequency corresponding to zero voltage (Hz)
        const float&  VcoVoltFrqScl;     // 2-Vco Voltage over frequency scale (V/Hz)
    };

    // Methods
    //--------
    cThrmstrSim( InTyp InList, InsTyp InsList );
    ~cThrmstrSim();

    int   RunRules( void );
    void  Task( void );                  // Method called in Task1 but executed
                                         // only at T1-T3 boundary
    // Output Signals
    //---------------
    CREATE_PUBVAR(Brg1BpFrqSim,       float); // Simulated bridge-1 base-plate (Hz)
    CREATE_PUBVAR(Brg2BpFrqSim,       float); // Simulated bridge-2 base-plate (Hz)
    CREATE_PUBVAR(CoolntFrqLwSim,     float); // Simulated coolant temperature low-range (Hz)
    CREATE_PUBVAR(CoolntFrqHiSim,     float); // Simulated coolant temperature high-range (Hz)

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
    // Translates  temperature into Vco frequency
    float TempToFrq( const float& Temperature, const float TempData[], int DataSize, int Range1stPnt,
                     float VcoFrqMax, float VoltPerPnt, float Volt1stPnt, float VoltPerTemp );

    // Diagnostics
    //------------

    // Parameters
    //-----------

    // Published Variables
    //--------------------
    CREATE_PUBVAR(BrgVcoFrqMax,      float);  // Bridge Vco freq corresponding to max input voltage (Hz)
    CREATE_PUBVAR(IoVcoLwFrqMax,     float);  // Io Vco freq corresponding to low-range max input voltage (Hz)
    CREATE_PUBVAR(IoVcoHiFrqMax,     float);  // Io Vco freq corresponding to high-range max input voltage (Hz)
    CREATE_PUBVAR(VcoVoltFrqSclRcp,  float);  // Vco voltage over frequency scale reciprocal (Hz/V)
    CREATE_PUBVAR(BrgRangTablDelt,   float);  // Bridge delta voltage per each table data-point (Volt/Index)
    CREATE_PUBVAR(IoLwVoltPerPnt,    float);  // Io low-range delta voltage per each table data-point (Volt/Index)
    CREATE_PUBVAR(IoHiVoltPerPnt,    float);  // Io high-range delta voltage per each table data-point (Volt/Index)
    CREATE_PUBVAR(IoVoltPerTemp,     float);  // Io delta voltage per each temperature datum (Volt/degC)

    // Unpublished Variables
    //----------------------

    // Data Passing
    //-------------

};

#endif  // S0_THERMIS_SIMULATOR_H
