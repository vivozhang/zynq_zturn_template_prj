///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Thermistor Simulator
//
// DESCRIPTION
//              This file contains the methods for thermistor simulation.
//
// COPYRIGHT    Copyright (c) 2015
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      s0ThrmstrSim.h, s0ThrmstrSim.xls, s0ThrmstrSim.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
// product
#include "Product.h"
#include "s0ThrmstrSim.h"
#include "b0BrgThrmstr.h"
#include "p0Thrmstr.h"


// Constants
//----------


// Externals
//----------


// Types and Classes
//------------------


// Function Prototypes
//--------------------


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


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class constructor to initialize resources
//
// EXECUTION LEVEL
//      Class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cThrmstrSim::cThrmstrSim
(
        InTyp    InList,
        InsTyp   InsList
)
        : In(InList), Ins(InsList)
{
    // Initilize
    OUT(Brg1BpFrqSim)       = 280825;   // This initialization prevents unrealistic
    OUT(Brg2BpFrqSim)       = 280825;   // temperature readings, when simulator is
    OUT(CoolntFrqLwSim)     = 677934;   // frozen right after power-up.
    OUT(CoolntFrqHiSim)     = 138028;
    BrgVcoFrqMax            = 0.0F;
    IoVcoLwFrqMax           = 0.0F;
    IoVcoHiFrqMax           = 0.0F;
    VcoVoltFrqSclRcp        = 0.0F;
    BrgRangTablDelt         = 0.0F;
    IoLwVoltPerPnt          = 0.0F;
    IoHiVoltPerPnt          = 0.0F;
    IoVoltPerTemp           = 0.0F;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cThrmstrSim::~cThrmstrSim ()
{
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by the class.
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cThrmstrSim::RunRules (void)
{
    int Status = SUCCESS;

    if ( INS(VcoVoltFrqScl) > 0.0F )
    {
        BrgVcoFrqMax = (TEMP_VOLTS_TBL_MAX/INS(VcoVoltFrqScl)) + INS(VcoFrqZeroVolt);
        IoVcoLwFrqMax = (LOW_RNG_VLT_MAX/INS(VcoVoltFrqScl)) + INS(VcoFrqZeroVolt);
        IoVcoHiFrqMax = (HI_RNG_VLT_MAX/INS(VcoVoltFrqScl)) + INS(VcoFrqZeroVolt);
        VcoVoltFrqSclRcp = 1.0F / INS(VcoVoltFrqScl);
    }
    BrgRangTablDelt = TEMP_DATA_PTS_RCP * V_TEMP_FULL;
    IoLwVoltPerPnt  = 1.0F / LOW_RNG_TBL_DELP;
    IoHiVoltPerPnt  = 1.0F / HI_RNG_TBL_DELP;
    IoVoltPerTemp   = 1.0F / LOW_DEL_PT1_VOLT;

    return (Status);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Task functions
//
// EXECUTION LEVEL
//      Called at Task1 rate but only executed in a T1-T3 boundary
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cThrmstrSim::Task ( void )
{
    if ( IN(T1wthnT3) != 0 )    return;

    // Reverse temperature to Vco frequencies at execution task boundary
    //----------------------------------------------------------------------

    OUT(Brg1BpFrqSim) = TempToFrq(IN(BpTemp), cBrgThrmstr::TempTable, TEMP_DATA_ARRAY_SIZE, 0,
                                  BrgVcoFrqMax, BrgRangTablDelt, 0.0F, 0.0F);
    OUT(Brg2BpFrqSim) = TempToFrq(IN(BpTemp), cBrgThrmstr::TempTable, TEMP_DATA_ARRAY_SIZE, 0,
                                  BrgVcoFrqMax, BrgRangTablDelt, 0.0F, 0.0F);

    OUT(CoolntFrqLwSim) = TempToFrq(IN(CoolntTemp), cThrmstr::TempTabLowT1, LOW_TEMP_DATA_ARRAY_SIZE, 1,
                                    IoVcoLwFrqMax, IoLwVoltPerPnt, LOW_RNG_VLT_MIN, IoVoltPerTemp);
    OUT(CoolntFrqHiSim) = TempToFrq(IN(CoolntTemp), cThrmstr::TempTabHighT1, HI_TEMP_DATA_ARRAY_SIZE, 0,
                                    IoVcoHiFrqMax, IoHiVoltPerPnt, HI_RNG_VLT_MIN, 0.0F);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    This function translates thermistor temperature into Vco frequency
//    using a look-up table.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      Translated Vco frequency in Hz
//
///////////////////////////////////////////////////////////////////////////////
float cThrmstrSim::TempToFrq(
    // Input/Output
    const float& Temperature,  // Temperature signal to convert (degC)
    const float  TempData[],   // Temperature data table (degC)
    int          DataSize,     // Size of data table
    int          Range1stPnt,  // First point in data range (Index)
    float        VcoFrqMax,    // Max Vco frequency (Hz)
    float        VoltPerPnt,   // Voltage/Point density (Volt/Index)
    float        Volt1stPnt,   // Voltage corresponding to 1st point in data range (Volt)
    float        VoltPerTemp   // Voltage/Temperature density (Volt/degC)
)
{
    int      Idx = 0;
    int      LastIdx;
    float    Delta1;
    float    Delta2;
    float    IdxFlt;
    float    VoltVal;

    LastIdx = DataSize - 1;   // Last valid index for table

    // Limit frequency to table range
    if ( Temperature <= TempData[0] )
    {
        //Temperature = TempData[0];    // In case temperature needs to be limited
        return INS(VcoFrqZeroVolt);
    }
    else if ( Temperature >= TempData[LastIdx] )
    {
        //Temperature = TempData[LastIdx];  // In case temperature needs to be limited
        return VcoFrqMax;
    }

    // Find table index: integer part
    while ( Temperature > TempData[Idx] )
    {
        if ( Idx == LastIdx )   return VcoFrqMax;   // Protect against invalid array access
        Idx++;
    }

    // Find table index: fractional part
    Delta1 = Temperature - TempData[Idx-1];
    Delta2 = TempData[Idx] - TempData[Idx-1];
    if ( Delta2 > 0.0F )
    {
        IdxFlt = Delta1/Delta2 + static_cast<float>(Idx) - 1.0F;
    }
    else
    {
        return VcoFrqMax;
    }

    if ( IdxFlt < static_cast<float>(Range1stPnt) )  // Index is below 1st point in data range,
    {                                                // use alternate voltage/temperature density
        VoltVal = (Temperature - TempData[Range1stPnt])*VoltPerTemp + Volt1stPnt;
    }
    else    // Use table Voltage/Point density
    {
        VoltVal = IdxFlt*VoltPerPnt + Volt1stPnt;
    }

    return ( VoltVal*VcoVoltFrqSclRcp + INS(VcoFrqZeroVolt) );
}

