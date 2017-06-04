///////////////////////////////////////////////////////////////////////////////
//
// TITLE     Line bridge thermistor feedback processing
//
// DESCRIPTION
//          Functions contained in this file convert Negative Temperature
//          Coefficient (NTC) thermistor circuit voltages to heat sink
//          temperature signals.
//
// COPYRIGHT    Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "b0BrgThrmstr.h"
#include "b0BridgeRebf.h"
#include "y2BrThm.h"
#include "b0PwmMod2Async.h"
#include "b0DbThrm.h"

// Constants
//----------

// Externals
//----------
extern float         L_DelTm1;
extern float         L_DelTm1Rcp;
extern unsigned      TempRngSel;
extern volatile int  T1T2St;
extern cBridgeRebf   L_BridgeRebf;
extern int           L_TempVcoCalcSt;
extern float         L_TempVcoDelTmRcp;


// Types and Classes
//------------------

// Parameters
//-----------

// Variables
//----------
CREATE_PUBVAR(TempA11Raw        ,int);      // Temp A11 VCO counts
CREATE_PUBVAR(TempA12Raw        ,int);      // Temp A12 VCO counts
CREATE_PUBVAR(TempB11Raw        ,int);      // Temp B11 VCO counts
CREATE_PUBVAR(TempB12Raw        ,int);      // Temp B12 VCO counts
CREATE_PUBVAR(TempC11Raw        ,int);      // Temp C11 VCO counts
CREATE_PUBVAR(TempC12Raw        ,int);      // Temp C12 VCO counts
CREATE_PUBVAR(TempD11Raw        ,int);      // Temp D11 VCO counts
CREATE_PUBVAR(TempD12Raw        ,int);      // Temp D12 VCO counts
CREATE_PUBVAR(TempA11Frq        ,float);    // Temp A11 VCO frequency
CREATE_PUBVAR(TempA12Frq        ,float);    // Temp A12 VCO frequency
CREATE_PUBVAR(TempB11Frq        ,float);    // Temp B11 VCO frequency
CREATE_PUBVAR(TempB12Frq        ,float);    // Temp B12 VCO frequency
CREATE_PUBVAR(TempC11Frq        ,float);    // Temp C11 VCO frequency
CREATE_PUBVAR(TempC12Frq        ,float);    // Temp C12 VCO frequency
CREATE_PUBVAR(TempD11Frq        ,float);    // Temp D11 VCO frequency
CREATE_PUBVAR(TempD12Frq        ,float);    // Temp D12 VCO frequency
CREATE_PUBVAR(TempVltA11        ,float);    // Temp volts for A11
CREATE_PUBVAR(TempVltA12        ,float);    // Temp volts for A12
CREATE_PUBVAR(TempVltB11        ,float);    // Temp volts for B11
CREATE_PUBVAR(TempVltB12        ,float);    // Temp volts for B12
CREATE_PUBVAR(TempVltC11        ,float);    // Temp volts for C11
CREATE_PUBVAR(TempVltC12        ,float);    // Temp volts for C12
CREATE_PUBVAR(TempVltD11        ,float);    // Temp volts for D11
CREATE_PUBVAR(TempVltD12        ,float);    // Temp volts for D12

CREATE_PUBVAR(L_DJncTemp        ,float);
CREATE_PUBVAR(L_VTemp1          ,float);    // Output voltages of NTC temperature circuit
CREATE_PUBVAR(L_VTemp2          ,float);
CREATE_PUBVAR(L_VTemp3          ,float);
CREATE_PUBVAR(L_VTemp4          ,float);
CREATE_PUBVAR(L_VTemp5          ,float);
CREATE_PUBVAR(L_VTemp6          ,float);
CREATE_PUBVAR(L_VTemp7          ,float);
CREATE_PUBVAR(L_VTemp8          ,float);
CREATE_PUBVAR(L_TempVcoSel      ,int);
CREATE_PUBVAR(L_Brg1TempVco     ,unsigned);
CREATE_PUBVAR(L_ThrmTempVco     ,unsigned);
CREATE_PUBVAR(L_TempVcoSmplEnb  ,unsigned);
CREATE_PUBVAR(L_BrgTempSmplCnt  ,int);
CREATE_PUBVAR(L_TempVcoCalcEnb  ,unsigned);
CREATE_PUBVAR(L_BrgTempCalcCnt  ,int);
CREATE_PUBVAR(L_ThrmHiSel       ,unsigned);
CREATE_PUBVAR(L_ThrmHiSelT3     ,unsigned);

CREATE_PUBVAR(L_ThrmTempCnts0     ,unsigned);
CREATE_PUBVAR(L_ThrmTempCnts1     ,unsigned);

// Diagnostics
//------------

// Data Passing
//--------------
DATA_PASS(L_Brg1TempCnts[0] ,L_Brg1TempCntsT3[0]       , T1_T3, UNFILTERED);
DATA_PASS(L_Brg1TempCnts[1] ,L_Brg1TempCntsT3[1]       , T1_T3, UNFILTERED);
DATA_PASS(L_Brg1TempCnts[2] ,L_Brg1TempCntsT3[2]       , T1_T3, UNFILTERED);
DATA_PASS(L_Brg1TempCnts[3] ,L_Brg1TempCntsT3[3]       , T1_T3, UNFILTERED);
DATA_PASS(L_Brg1TempCnts[4] ,L_Brg1TempCntsT3[4]       , T1_T3, UNFILTERED);
DATA_PASS(L_Brg1TempCnts[5] ,L_Brg1TempCntsT3[5]       , T1_T3, UNFILTERED);
DATA_PASS(L_Brg1TempCnts[6] ,L_Brg1TempCntsT3[6]       , T1_T3, UNFILTERED);
DATA_PASS(L_Brg1TempCnts[7] ,L_Brg1TempCntsT3[7]       , T1_T3, UNFILTERED);

DATA_PASS(L_ThrmTempCnts[0] ,L_ThrmTempCntsT3[0]       , T1_T3, UNFILTERED);
DATA_PASS(L_ThrmTempCnts[1] ,L_ThrmTempCntsT3[1]       , T1_T3, UNFILTERED);
DATA_PASS(L_ThrmTempCnts[2] ,L_ThrmTempCntsT3[2]       , T1_T3, UNFILTERED);
DATA_PASS(L_ThrmTempCnts[3] ,L_ThrmTempCntsT3[3]       , T1_T3, UNFILTERED);
DATA_PASS(L_ThrmTempCnts[4] ,L_ThrmTempCntsT3[4]       , T1_T3, UNFILTERED);
DATA_PASS(L_ThrmTempCnts[5] ,L_ThrmTempCntsT3[5]       , T1_T3, UNFILTERED);
DATA_PASS(L_ThrmTempCnts[6] ,L_ThrmTempCntsT3[6]       , T1_T3, UNFILTERED);
DATA_PASS(L_ThrmTempCnts[7] ,L_ThrmTempCntsT3[7]       , T1_T3, UNFILTERED);

DATA_PASS(L_ThrmHiSel       ,L_ThrmHiSelT3             , T1_T3, UNFILTERED);

// Unpublished Variables
//----------------------
unsigned     L_Brg1TempVcoI[8];
unsigned     L_Brg1TempCnts[8];
unsigned     L_ThrmTempVcoI[8];  // only 4 used on line on markstat sep
unsigned     L_ThrmTempCnts[8];
unsigned     L_Brg1TempCntsT3[8];
unsigned     L_ThrmTempCntsT3[8];

// Local Prototypes (to resolve forward references)
//-------------------------------------------------

// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line bridge thermistor voltage calculations in task1.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineTempConversionT1( void )
{

    L_TempVcoSel  = L_BridgeRebf.BrgTempVcoSel;
    L_Brg1TempVco = L_BridgeRebf.CellTempVcoBrg1;
    L_ThrmTempVco = L_BridgeRebf.ThermistorVcoIo1;

    if (T1T2St == 0) L_TempVcoSmplEnb = true;

    if (L_TempVcoSmplEnb )
    {
        // save initial VCO samples into an array
        L_Brg1TempVcoI[L_TempVcoSel] = L_Brg1TempVco;

        if (L_TempVcoSel < 4)
            L_ThrmTempVcoI[L_TempVcoSel] = L_ThrmTempVco;

        // sample 8 vco's
        L_BrgTempSmplCnt++;
        if (L_BrgTempSmplCnt > 7)
        {
            L_TempVcoSmplEnb = false;
            L_BrgTempSmplCnt = 0;
        }
    }

    if (T1T2St == L_TempVcoCalcSt) L_TempVcoCalcEnb = true;

    if (L_TempVcoCalcEnb)
    {
        // vco delta's
        if ( !L_BridgeRebf.RebhSet)
        {
            L_Brg1TempCnts[L_TempVcoSel] = L_Brg1TempVco - L_Brg1TempVcoI[L_TempVcoSel];
        }
        else
        {
            L_Brg1TempCnts[L_TempVcoSel] = L_Brg1TempVco;
        }

        if (L_TempVcoSel < 4)
        {
            L_ThrmTempCnts[L_TempVcoSel] = L_ThrmTempVco - L_ThrmTempVcoI[L_TempVcoSel];
            if ( L_TempVcoSel == 0)
            {
                L_ThrmTempCnts0 = L_ThrmTempVco;
                //L_ThrmTempCnts1 = L_ThrmTempCnts[0];
            }
        }
        // calc 8 delta's
        L_BrgTempCalcCnt++;
        if (L_BrgTempCalcCnt > 7)
        {
            L_TempVcoCalcEnb = false;
            L_BrgTempCalcCnt = 0;

            if      ( TempRngSel == THRM_RNG_AUTO)  L_ThrmHiSel = !L_ThrmHiSel;
            else if ( TempRngSel == THRM_RNG_HIGH)  L_ThrmHiSel = true;
            else                                      L_ThrmHiSel = false;
        }
    }

    return;
}




///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line bridge thermistor voltage calculations in task3.
//      Read in VCO counts for temperatures one device per each task 2 pass,
//      nine bridge temperatures, as well as four temperatures from the
//      AEAD for thermisters 1-4,  and one ambient temperature, which
//      is not a VCO reading. Converter each to a voltage that will be
//      data passed to task3 for further processing.  The four AEAD temp-
//      eratures are each read two times per task 3, once at the 'high'
//      range, and once at the 'low' range.  The selection for these
//      is made four task2's prior to reading them to allow settling time
//      for the new votlages on the AEAD card.
//      For each pass of task2:
//         *  Read the VCO temperature value set up by the previous task2,
//            or in the case of thermisters 1-4, four task 1's previous.
//         *  Convert the vco values to the voltage at the input of the vco
//         *  Updata the FPGA temperature selection for 2 task 2's away  and
//            update the temperature selection for therm1-4 4 task 2's away.
//
//      Assumptions:  The function assumes that there will be 10 task2's per
//      task3.
//
//      FPGA baseplate    Corresponding vars         Elementary
//      temperature mux:     (line)                  (ESS)
//      0   PH A Upper    L_VTemp1  = TempVltA12      A top
//      1   PH A Lower    L_VTemp4  = TempVltA11      A bottom
//      2   PH B Upper    L_VTemp2  = TempVltB12      B top
//      3   PH B Lower    L_VTemp5  = TempVltB11      B bottom
//      4   PH C Upper    L_VTemp3  = TempVltC12      C top
//      5   PH C Lower    L_VTemp6  = TempVltC11      C bottom
//      6   PH D Upper
//      7   PH D Lower
//
//      The "Upper/Lower" terminology in TT's doc is inconsistant since the AEBI connector
//      labels are "Temperature 1 / Temperature 2" and the ESS elementary shows these connected
//      to "Bottom / Top IGBT" respectively.
//      Assumption: "Upper" = "2" = "Top";   "Lower" = "1" = "Bottom"
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineTempConversionT3( void )
{

    // Line A1
    //--------
    if ( !L_BridgeRebf.RebhSet)
    {
        TempA11Raw = static_cast<int> (VCO_MASK & L_Brg1TempCntsT3[0]);
        TempA11Frq = (static_cast<float> (TempA11Raw)) * L_TempVcoDelTmRcp;
        TempVltA11 = (TempA11Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);
    }
    else
    {
        TempA11Raw = (int)L_Brg1TempCntsT3[0];
        TempVltA11 = (float)L_Brg1TempCntsT3[0]*5/255;
    }

    // Line B1
    if ( !L_BridgeRebf.RebhSet)
    {
        TempB11Raw = static_cast<int> (VCO_MASK & L_Brg1TempCntsT3[2]);
        TempB11Frq = (static_cast<float> (TempB11Raw)) * L_TempVcoDelTmRcp;
        TempVltB11 = (TempB11Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);
    }
    else
    {
        TempB11Raw = (int)L_Brg1TempCntsT3[2];
        TempVltB11 = (float)L_Brg1TempCntsT3[2]*5/255;
    }

    // Line C1
    if ( !L_BridgeRebf.RebhSet)
    {

        TempC11Raw = static_cast<int> (VCO_MASK & L_Brg1TempCntsT3[4]);
        TempC11Frq = (static_cast<float> (TempC11Raw)) * L_TempVcoDelTmRcp;
        TempVltC11 = (TempC11Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);
    }
    else
    {
        TempC11Raw = (int)L_Brg1TempCntsT3[4];
        TempVltC11 = (float)L_Brg1TempCntsT3[4]*5/255;
    }

    // Line DB1
    if ( !L_BridgeRebf.RebhSet)
    {

        TempD11Raw = static_cast<int> (VCO_MASK & L_Brg1TempCntsT3[6]);
        TempD11Frq = (static_cast<float> (TempD11Raw)) * L_TempVcoDelTmRcp;
        TempVltD11 = (TempD11Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);
    }
    else
    {
        TempD11Raw = (int)L_Brg1TempCntsT3[6];
        TempVltD11 = (float)L_Brg1TempCntsT3[6]*5/255;
    }

    // Line A2
    //--------
    if ( !L_BridgeRebf.RebhSet)
    {
        TempA12Raw = static_cast<int> (VCO_MASK & L_Brg1TempCntsT3[1]);
        TempA12Frq = (static_cast<float> (TempA12Raw)) * L_TempVcoDelTmRcp;
        TempVltA12 = (TempA12Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);
    }
    else
    {
        TempA12Raw = (int)L_Brg1TempCntsT3[1];
        TempVltA12 = (float)L_Brg1TempCntsT3[1]*5/255;
    }

    // Line B2
    if ( !L_BridgeRebf.RebhSet)
    {
        TempB12Raw = static_cast<int> (VCO_MASK & L_Brg1TempCntsT3[3]);
        TempB12Frq = (static_cast<float> (TempB12Raw)) * L_TempVcoDelTmRcp;
        TempVltB12 = (TempB12Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);
    }
    else
    {
        TempB12Raw = (int)L_Brg1TempCntsT3[3];
        TempVltB12 = (float)L_Brg1TempCntsT3[3]*5/255;
    }

    // Line C2
    if ( !L_BridgeRebf.RebhSet)
    {
        TempC12Raw = static_cast<int> (VCO_MASK & L_Brg1TempCntsT3[5]);
        TempC12Frq = (static_cast<float> (TempC12Raw)) * L_TempVcoDelTmRcp;
        TempVltC12 = (TempC12Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);
    }
    else
    {
        TempD12Raw = (int)L_Brg1TempCntsT3[5];
        TempVltC12 = (float)L_Brg1TempCntsT3[5]*5/255;
    }

    // Line DB2
    if ( !L_BridgeRebf.RebhSet)
    {
        TempD12Raw = static_cast<int> (VCO_MASK & L_Brg1TempCntsT3[7]);
        TempD12Frq = (static_cast<float> (TempD12Raw)) * L_TempVcoDelTmRcp;
        TempVltD12 = (TempD12Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);
    }
    else
    {
        TempD12Raw = (int)L_Brg1TempCntsT3[7];
        TempVltD12 = (float)L_Brg1TempCntsT3[7]*5/255;
    }


    ///////////////////////////////////////////////////////////////////////
    //  Line converter Vco to Temperature Conversion (BTM_011)
    ///////////////////////////////////////////////////////////////////////

    L_VTemp1  = TempVltA11;
    L_VTemp2  = TempVltB11;
    L_VTemp3  = TempVltC11;
    L_VTemp4  = TempVltA12;
    L_VTemp5  = TempVltB12;
    L_VTemp6  = TempVltC12;
    L_VTemp7  = TempVltD11;
    L_VTemp8  = TempVltD12;

    return;
}
