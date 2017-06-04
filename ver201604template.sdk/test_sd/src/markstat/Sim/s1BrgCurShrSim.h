///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Bridge Current Sharing Simulator
//
// DESCRIPTION:
//      This file contains definitions for the bridge current sharing simulator.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      s1BrgCurShrSim.vsd
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __S1_BRG_CUR_SHR_SIM_H__
#define __S1_BRG_CUR_SHR_SIM_H__

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
// DESCRIPTION
//      Simulation class for parallel bridge connection.
//
///////////////////////////////////////////////////////////////////////////////
class cBrgCurShrSim
{

//-----------------------------------------------------------------------------
public:
    // Types
    //------
    struct InListTyp
    {
        const float& VaBrg1;    // 1 - Bridge 1 phase voltages (Volt)
        const float& VbBrg1;    // 2 -
        const float& VcBrg1;    // 3 -

        const float& VaBrg2;    // 4 - Bridge 2 phase voltages (Volt)
        const float& VbBrg2;    // 5 -
        const float& VcBrg2;    // 6 -

        const float& IaPlant;   // 7 - Plant phase currents (Amp)
        const float& IbPlant;   // 8 -
        const float& IcPlant;   // 9 -
    };

    struct InsListTyp
    {
        const float& DelTm1;    // 1 - Delta task1 time (sec)
        const float& CrsRbrg1;  // 2 -
        const float& CrsLbrg1;  // 3 -
        const float& CmnLbrg1;  // 4 -
        const float& CrsRbrg2;  // 5 -
        const float& CrsLbrg2;  // 6 -
        const float& CmnLbrg2;  // 7 -
    };

    // Methods
    //--------
    cBrgCurShrSim( InListTyp InList, InsListTyp InsList );
    ~cBrgCurShrSim ( );

    void Task1( void );
    int  RunRules( void );

    // Outputs Signals
    //----------------
    CREATE_PUBVAR(CshIaTotBrg1Sim,    float);     // bridge 1 : total phase a current (amp)
    CREATE_PUBVAR(CshIbTotBrg1Sim,    float);     // bridge 1 : total phase b current (amp)
    CREATE_PUBVAR(CshIcTotBrg1Sim,    float);     // bridge 1 : total phase c current (amp)

    CREATE_PUBVAR(CshIaTotBrg2Sim,    float);     // bridge 2 : total phase a current (amp)
    CREATE_PUBVAR(CshIbTotBrg2Sim,    float);     // bridge 2 : total phase b current (amp)
    CREATE_PUBVAR(CshIcTotBrg2Sim,    float);     // bridge 2 : total phase c current (amp)

    CREATE_PUBVAR(CshVaSim,           float);     // cummulative voltage phase a (volts)
    CREATE_PUBVAR(CshVbSim,           float);     // cummulative voltage phase b (volts)
    CREATE_PUBVAR(CshVcSim,           float);     // cummulative voltage phase c (volts)

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;

    // External Inputs
    //----------------
    InListTyp    In;
    InsListTyp   Ins;

    // Methods
    //--------
    void SwSatVoltCalc( float Vdrop, float Vup, float Vdn, float& Vsat );

    // Diagnostics
    //------------

    // Parameters
    //-----------
    CREATE_PARM(CshRsatSim,           float);     // bridge switch "on" resistance (ohms)

    CREATE_PARM(CshVaSatUpBrg1Sim,    float);     // bridge 1 : saturation voltage up switch phase a (volts)
    CREATE_PARM(CshVbSatUpBrg1Sim,    float);     // bridge 1 : saturation voltage up switch phase b (volts)
    CREATE_PARM(CshVcSatUpBrg1Sim,    float);     // bridge 1 : saturation voltage up switch phase c (volts)

    CREATE_PARM(CshVaSatDnBrg1Sim,    float);     // bridge 1 : saturation voltage down switch phase a (volts)
    CREATE_PARM(CshVbSatDnBrg1Sim,    float);     // bridge 1 : saturation voltage down switch phase b (volts)
    CREATE_PARM(CshVcSatDnBrg1Sim,    float);     // bridge 1 : saturation voltage down switch phase c (volts)

    CREATE_PARM(CshRaDynBrg1Sim,      float);     // bridge 1 : equivalent switch dynamic resistance phase a (ohms)
    CREATE_PARM(CshRbDynBrg1Sim,      float);     // bridge 1 : equivalent switch dynamic resistance phase b (ohms)
    CREATE_PARM(CshRcDynBrg1Sim,      float);     // bridge 1 : equivalent switch dynamic resistance phase c (ohms)

    CREATE_PARM(CshVaSatUpBrg2Sim,    float);     // bridge 2 : saturation voltage up switch phase a (volts)
    CREATE_PARM(CshVbSatUpBrg2Sim,    float);     // bridge 2 : saturation voltage up switch phase b (volts)
    CREATE_PARM(CshVcSatUpBrg2Sim,    float);     // bridge 2 : saturation voltage up switch phase c (volts)

    CREATE_PARM(CshVaSatDnBrg2Sim,    float);     // bridge 2 : saturation voltage down switch phase a (volts)
    CREATE_PARM(CshVbSatDnBrg2Sim,    float);     // bridge 2 : saturation voltage down switch phase b (volts)
    CREATE_PARM(CshVcSatDnBrg2Sim,    float);     // bridge 2 : saturation voltage down switch phase c (volts)

    CREATE_PARM(CshRaDynBrg2Sim,      float);     // bridge 2 : equivalent switch dynamic resistance phase a (ohms)
    CREATE_PARM(CshRbDynBrg2Sim,      float);     // bridge 2 : equivalent switch dynamic resistance phase b (ohms)
    CREATE_PARM(CshRcDynBrg2Sim,      float);     // bridge 2 : equivalent switch dynamic resistance phase c (ohms)

    CREATE_PARM(CshCrsIfilWSim,       float);     // cross normal mode current filter (model of current dynamics)
    CREATE_PARM(CshCmnIfilWSim,       float);     // common mode current filter (model of current dynamics)
    CREATE_PARM(CshDiDtFilWSim,       float);     // general filter for di/dt approximation (modeling of coupling between normal and cross modes)

    CREATE_PARM(CshRbrg1Sim,          float);     // bridge 1 inductor resistance (ohms)
    CREATE_PARM(CshLbrg1Sim,          float);     // bridge 1 inductor + cable normal mode equivalent inductance (henries)
    CREATE_PARM(CshL0brg1Sim,         float);     // bridge 1 inductor + cable common mode equivalent inductance (henries)

    CREATE_PARM(CshRbrg2Sim,          float);     // bridge 2 inductor resistance (ohms)
    CREATE_PARM(CshLbrg2Sim,          float);     // bridge 2 inductor + cable normal mode equivalent inductance (henries)
    CREATE_PARM(CshL0brg2Sim,         float);     // bridge 2 inductor + cable common mode equivalent inductance (henries)

    // parameters used to create the imbalance in the brg reactors
    CREATE_PARM(CshBrg1ImbRatioSim,   float);     // simulated brg 1 reactor imbalance ratio
    CREATE_PARM(CshBrg2ImbRatioSim,   float);     // simulated brg 2 reactor imbalance ratio

    // Published Variables
    //--------------------
    CREATE_PUBVAR(CshIalSim,          float);     // alpha current component (amps)
    CREATE_PUBVAR(CshIbeSim,          float);     // beta current component (amps)

    CREATE_PUBVAR(CshI0CrsSim,        float);     // common mode cross current component (amps)
    CREATE_PUBVAR(CshIalCrsSim,       float);     // alpha cross current component (amps)
    CREATE_PUBVAR(CshIbeCrsSim,       float);     // beta  cross current component (amps)

    CREATE_PUBVAR(CshIalBrg1Sim,      float);     // bridge 1 : alpha current
    CREATE_PUBVAR(CshIbeBrg1Sim,      float);     // bridge 1 : beta current

    CREATE_PUBVAR(CshIalBrg2Sim,      float);     // bridge 2 : alpha current
    CREATE_PUBVAR(CshIbeBrg2Sim,      float);     // bridge 2 : beta current

    CREATE_PUBVAR(CshVaSatBrg1Sim,    float);     // bridge 1 : total distortion voltage modeled as a
    CREATE_PUBVAR(CshVbSatBrg1Sim,    float);     // constant saturation plus proportional drop,
    CREATE_PUBVAR(CshVcSatBrg1Sim,    float);     // phases a, b, and c

    CREATE_PUBVAR(CshVaSatBrg2Sim,    float);     // bridge 2 : total distortion voltage modeled as a
    CREATE_PUBVAR(CshVbSatBrg2Sim,    float);     // constant saturation plus proportional drop,
    CREATE_PUBVAR(CshVcSatBrg2Sim,    float);     // phases a, b, and c

    CREATE_PUBVAR(CshVaOutBrg1Sim,    float);     // bridge 1 converter resulting output voltage phase a
    CREATE_PUBVAR(CshVbOutBrg1Sim,    float);     // bridge 1 converter resulting output voltage phase b
    CREATE_PUBVAR(CshVcOutBrg1Sim,    float);     // bridge 1 converter resulting output voltage phase c

    CREATE_PUBVAR(CshVaOutBrg2Sim,    float);     // bridge 2 converter resulting output voltage phase a
    CREATE_PUBVAR(CshVbOutBrg2Sim,    float);     // bridge 2 converter resulting output voltage phase b
    CREATE_PUBVAR(CshVcOutBrg2Sim,    float);     // bridge 2 converter resulting output voltage phase c

    CREATE_PUBVAR(CshV0OutBrg1Sim,    float);     // bridge 1 : common mode voltage
    CREATE_PUBVAR(CshValOutBrg1Sim,   float);     // bridge 1 : alpha  voltage component
    CREATE_PUBVAR(CshVbeOutBrg1Sim,   float);     // bridge 1 : beta voltage component

    CREATE_PUBVAR(CshV0OutBrg2Sim,    float);     // bridge 2 : common mode voltage
    CREATE_PUBVAR(CshValOutBrg2Sim,   float);     // bridge 2 : alpha voltage component
    CREATE_PUBVAR(CshVbeOutBrg2Sim,   float);     // bridge 2 : beta voltage component

    CREATE_PUBVAR(CshV0CrsSim,        float);     // voltage common mode (volts)

    CREATE_PUBVAR(CshValNrmSim,       float);     // voltage cummulative mode alpha (volts)
    CREATE_PUBVAR(CshVbeNrmSim,       float);     // voltage cummulative mode beta (volts)

    CREATE_PUBVAR(CshValCrsSim,       float);     // voltage differential mode alpha (volts)
    CREATE_PUBVAR(CshVbeCrsSim,       float);     // voltage differential mode beta (volts)

    CREATE_PUBVAR(CshValCrsLdelSim,   float);     // alpha component: cross voltage resulted from the 2 converters inductance asymmetry
    CREATE_PUBVAR(CshValCrsRdelSim,   float);     // alpha component: cross voltage resulted from the 2 converters resistance asymmetry
    CREATE_PUBVAR(CshValCrsTotSim,    float);     // alpha component: total normal mode cross voltage

    CREATE_PUBVAR(CshVbeCrsLdelSim,   float);     // beta component: cross voltage resulted from the 2 converters inductance asymmetry
    CREATE_PUBVAR(CshVbeCrsRdelSim,   float);     // beta component: cross voltage resulted from the 2 converters resistance asymmetry
    CREATE_PUBVAR(CshVbeCrsTotSim,    float);     // beta component: total normal mode cross voltage

    // Un-published Variables
    //-----------------------
    float   CshIalZSim;
    float   CshIbeZSim;

    float   CshRdelSim;                           // bridge inductor resistance asymmetry
    float   CshLdelSim;                           // bridge inductor inductance asymmetry

    float   CshL0Sim;                             // average common mode inductance (henries)
    float   CshL0rcpSim;                          // average common mode inductance reciprocal

    float   CshLSim;                              // average normal mode inductance (henries)
    float   CshLrcpSim;                           // average normal mode inductance reciprocal

    float   CshRSim;                              // average resistance (ohms)
    float   CshRrcpSim;                           // average resistance reciprocical

    float   CshCrsIfilWdtSim;                     // filter * deltm1
    float   CshCmnIfilWdtSim;                     // filter * deltm1
    float   CshDiDtFilWdtSim;                     // filter * deltm1

};


// Inline Function Definitions
//----------------------------

#endif  // __S1_BRG_CUR_SHR_SIM_H__
