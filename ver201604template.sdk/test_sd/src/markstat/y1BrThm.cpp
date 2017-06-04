///////////////////////////////////////////////////////////////////////////////
//
// TITLE     Rotor bridge thermistor feedback processing
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
#include "product.h"
#include "b0BridgeRebf.h"
#include "b0BrgThrmstr.h"
#include "y1BrThm.h"
#include "b0PwmMod2Async.h"

// Constants
//----------

// Externals
//----------
extern float         R_DelTm1;
extern unsigned      TempRngSel;
extern volatile int  T1AT2St;
extern cBridgeRebf   R_BridgeRebf;
extern int           R_TempVcoCalcSt;
extern float         R_TempVcoDelTmRcp;

// Types and Classes
//------------------

// Parameters
//-----------

// Variables
//----------

// Temperature conversion variables
CREATE_PUBVAR(R_VTemp1  ,       float);  // Output voltages of NTC temperature circuit
CREATE_PUBVAR(R_VTemp2  ,       float);
CREATE_PUBVAR(R_VTemp3  ,       float);
CREATE_PUBVAR(R_VTemp4  ,       float);
CREATE_PUBVAR(R_VTemp5  ,       float);
CREATE_PUBVAR(R_VTemp6  ,       float);
CREATE_PUBVAR(R_VTemp7  ,       float);
CREATE_PUBVAR(R_VTemp8  ,       float);

CREATE_PUBVAR(TempA21Frq  ,      float);  // Temp A21 VCO frequency
CREATE_PUBVAR(TempA22Frq  ,      float);  // Temp A22 VCO frequency
CREATE_PUBVAR(TempA31Frq  ,      float);  // Temp A31 VCO frequency
CREATE_PUBVAR(TempA32Frq  ,      float);  // Temp A32 VCO frequency
CREATE_PUBVAR(TempB21Frq  ,      float);  // Temp B21 VCO frequency
CREATE_PUBVAR(TempB22Frq  ,      float);  // Temp B22 VCO frequency
CREATE_PUBVAR(TempB31Frq  ,      float);  // Temp B31 VCO frequency
CREATE_PUBVAR(TempB32Frq  ,      float);  // Temp B32 VCO frequency
CREATE_PUBVAR(TempC21Frq  ,      float);  // Temp C21 VCO frequency
CREATE_PUBVAR(TempC22Frq  ,      float);  // Temp C22 VCO frequency
CREATE_PUBVAR(TempC31Frq  ,      float);  // Temp C31 VCO frequency
CREATE_PUBVAR(TempC32Frq  ,      float);  // Temp C32 VCO frequency
CREATE_PUBVAR(TempD21Frq  ,      float);  // Temp D21 VCO frequency
CREATE_PUBVAR(TempD22Frq  ,      float);  // Temp D22 VCO frequency
CREATE_PUBVAR(TempD31Frq  ,      float);  // Temp D31 VCO frequency
CREATE_PUBVAR(TempD32Frq  ,      float);  // Temp D32 VCO frequency

CREATE_PUBVAR(TempVltA21  ,      float);  // Temp volts for A21
CREATE_PUBVAR(TempVltA22  ,      float);  // Temp volts for A22
CREATE_PUBVAR(TempVltA31  ,      float);  // Temp volts for A31
CREATE_PUBVAR(TempVltA32  ,      float);  // Temp volts for A32
CREATE_PUBVAR(TempVltB21  ,      float);  // Temp volts for B21
CREATE_PUBVAR(TempVltB22  ,      float);  // Temp volts for B22
CREATE_PUBVAR(TempVltB31  ,      float);  // Temp volts for B31
CREATE_PUBVAR(TempVltB32  ,      float);  // Temp volts for B32
CREATE_PUBVAR(TempVltC21  ,      float);  // Temp volts for C21
CREATE_PUBVAR(TempVltC22  ,      float);  // Temp volts for C22
CREATE_PUBVAR(TempVltC31  ,      float);  // Temp volts for C31
CREATE_PUBVAR(TempVltC32  ,      float);  // Temp volts for C32
CREATE_PUBVAR(TempVltD21  ,      float);  // Temp volts for D21
CREATE_PUBVAR(TempVltD22  ,      float);  // Temp volts for D22
CREATE_PUBVAR(TempVltD31  ,      float);  // Temp volts for D31
CREATE_PUBVAR(TempVltD32  ,      float);  // Temp volts for D32

CREATE_PUBVAR(TempA21Raw  ,      int);    // Temp A21 VCO counts
CREATE_PUBVAR(TempA22Raw  ,      int);    // Temp A22 VCO counts
CREATE_PUBVAR(TempA31Raw  ,      int);    // Temp A31 VCO counts
CREATE_PUBVAR(TempA32Raw  ,      int);    // Temp A32 VCO counts
CREATE_PUBVAR(TempB21Raw  ,      int);    // Temp B21 VCO counts
CREATE_PUBVAR(TempB22Raw  ,      int);    // Temp B22 VCO counts
CREATE_PUBVAR(TempB31Raw  ,      int);    // Temp B31 VCO counts
CREATE_PUBVAR(TempB32Raw  ,      int);    // Temp B32 VCO counts
CREATE_PUBVAR(TempC21Raw  ,      int);    // Temp C21 VCO counts
CREATE_PUBVAR(TempC22Raw  ,      int);    // Temp C22 VCO counts
CREATE_PUBVAR(TempC31Raw  ,      int);    // Temp C31 VCO counts
CREATE_PUBVAR(TempC32Raw  ,      int);    // Temp C32 VCO counts
CREATE_PUBVAR(TempD21Raw  ,      int);    // Temp D21 VCO counts
CREATE_PUBVAR(TempD22Raw  ,      int);    // Temp D22 VCO counts
CREATE_PUBVAR(TempD31Raw  ,      int);    // Temp D31 VCO counts
CREATE_PUBVAR(TempD32Raw  ,      int);    // Temp D32 VCO counts

CREATE_PUBVAR(R_Brg1TempVco ,    unsigned);
CREATE_PUBVAR(R_ThrmTempVco ,    unsigned);
CREATE_PUBVAR(R_TempVcoSmplEnb,  unsigned);
CREATE_PUBVAR(R_TempVcoCalcEnb,  unsigned);
CREATE_PUBVAR(R_ThrmHiSel      , unsigned);
CREATE_PUBVAR(R_ThrmHiSelT3    , unsigned);
CREATE_PUBVAR(R_Brg2TempVco     ,unsigned);
CREATE_PUBVAR(R_BrgTempCalcCnt,  int);
CREATE_PUBVAR(R_TempVcoSel  ,    int);
CREATE_PUBVAR(R_BrgTempSmplCnt,  int);

CREATE_PUBVAR(R_ThrmTempCnts0     ,unsigned);
CREATE_PUBVAR(R_ThrmTempCnts1     ,unsigned);

// Diagnostics
//------------

// Data Passing
//--------------
DATA_PASS(R_Brg1TempCnts[0] ,R_Brg1TempCntsT3[0]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg1TempCnts[1] ,R_Brg1TempCntsT3[1]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg1TempCnts[2] ,R_Brg1TempCntsT3[2]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg1TempCnts[3] ,R_Brg1TempCntsT3[3]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg1TempCnts[4] ,R_Brg1TempCntsT3[4]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg1TempCnts[5] ,R_Brg1TempCntsT3[5]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg1TempCnts[6] ,R_Brg1TempCntsT3[6]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg1TempCnts[7] ,R_Brg1TempCntsT3[7]       , T1A_T3, UNFILTERED);

DATA_PASS(R_Brg2TempCnts[0] ,R_Brg2TempCntsT3[0]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg2TempCnts[1] ,R_Brg2TempCntsT3[1]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg2TempCnts[2] ,R_Brg2TempCntsT3[2]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg2TempCnts[3] ,R_Brg2TempCntsT3[3]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg2TempCnts[4] ,R_Brg2TempCntsT3[4]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg2TempCnts[5] ,R_Brg2TempCntsT3[5]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg2TempCnts[6] ,R_Brg2TempCntsT3[6]       , T1A_T3, UNFILTERED);
DATA_PASS(R_Brg2TempCnts[7] ,R_Brg2TempCntsT3[7]       , T1A_T3, UNFILTERED);

DATA_PASS(R_ThrmTempCnts[0] ,R_ThrmTempCntsT3[0]       , T1A_T3, UNFILTERED);
DATA_PASS(R_ThrmTempCnts[1] ,R_ThrmTempCntsT3[1]       , T1A_T3, UNFILTERED);
DATA_PASS(R_ThrmTempCnts[2] ,R_ThrmTempCntsT3[2]       , T1A_T3, UNFILTERED);
DATA_PASS(R_ThrmTempCnts[3] ,R_ThrmTempCntsT3[3]       , T1A_T3, UNFILTERED);
DATA_PASS(R_ThrmTempCnts[4] ,R_ThrmTempCntsT3[4]       , T1A_T3, UNFILTERED);
DATA_PASS(R_ThrmTempCnts[5] ,R_ThrmTempCntsT3[5]       , T1A_T3, UNFILTERED);
DATA_PASS(R_ThrmTempCnts[6] ,R_ThrmTempCntsT3[6]       , T1A_T3, UNFILTERED);
DATA_PASS(R_ThrmTempCnts[7] ,R_ThrmTempCntsT3[7]       , T1A_T3, UNFILTERED);

DATA_PASS(R_ThrmHiSel       ,R_ThrmHiSelT3             , T1A_T3, UNFILTERED);

// Unpublished Variables
//----------------------
unsigned      R_Brg1TempVcoI[8];
unsigned      R_Brg1TempCnts[8];
unsigned      R_ThrmTempVcoI[8];
unsigned      R_ThrmTempCnts[8];
unsigned      R_Brg1TempCntsT3[8];
unsigned      R_Brg2TempCntsT3[8];
unsigned      R_ThrmTempCntsT3[8];
unsigned      R_Brg2TempVcoI[8];
unsigned      R_Brg2TempCnts[8];

// Local Prototypes (to resolve forward references)
//-------------------------------------------------

// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor bridge thermistor voltage calculations in task1.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorTempConversionT1( void )
{

    R_TempVcoSel  = R_BridgeRebf.BrgTempVcoSel;
    R_Brg1TempVco = R_BridgeRebf.CellTempVcoBrg1;
    R_ThrmTempVco = R_BridgeRebf.ThermistorVcoIo1;

    if (T1AT2St == 0) R_TempVcoSmplEnb = true;

    if (R_TempVcoSmplEnb )
    {
        // save initial VCO samples into an array
        R_Brg1TempVcoI[R_TempVcoSel] = R_Brg1TempVco;
        //if (R_TempVcoSel < 4)
            R_ThrmTempVcoI[R_TempVcoSel] = R_ThrmTempVco;

        // sample 8 vco's
        R_BrgTempSmplCnt++;
        if (R_BrgTempSmplCnt > 7)
        {
            R_TempVcoSmplEnb = false;
            R_BrgTempSmplCnt = 0;
        }
    }

    if (T1AT2St == R_TempVcoCalcSt) R_TempVcoCalcEnb = true;

    if (R_TempVcoCalcEnb)
    {
        // vco delta's
        if ( !R_BridgeRebf.RebhSet)
        {
            R_Brg1TempCnts[R_TempVcoSel] = R_Brg1TempVco - R_Brg1TempVcoI[R_TempVcoSel];
        }
        else
        {
            R_Brg1TempCnts[R_TempVcoSel] = R_Brg1TempVco;
        }

        //if (R_TempVcoSel < 4)
        //{
            R_ThrmTempCnts[R_TempVcoSel] = R_ThrmTempVco - R_ThrmTempVcoI[R_TempVcoSel];
            if ( R_TempVcoSel == 0)
            {
                R_ThrmTempCnts0 = R_ThrmTempVco;
                //R_ThrmTempCnts1 = R_ThrmTempCnts[0];
            }
        //}

        // calc 8 delta's
        R_BrgTempCalcCnt++;
        if (R_BrgTempCalcCnt > 7)
        {
            R_TempVcoCalcEnb = false;
            R_BrgTempCalcCnt = 0;

            if      ( TempRngSel == THRM_RNG_AUTO)  R_ThrmHiSel = !R_ThrmHiSel;
            else if ( TempRngSel == THRM_RNG_HIGH)  R_ThrmHiSel = true;
            else                                      R_ThrmHiSel = false;
        }
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor bridge thermistor voltage calculations in task3.
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
//      temperature mux:     (rotor)                  (ESS)
//      0   PH A Upper    R_VTemp1  = TempVltA22      A top
//      1   PH A Lower    R_VTemp4  = TempVltA21      A bottom
//      2   PH B Upper    R_VTemp2  = TempVltB22      B top
//      3   PH B Lower    R_VTemp5  = TempVltB21      B bottom
//      4   PH C Upper    R_VTemp3  = TempVltC22      C top
//      5   PH C Lower    R_VTemp6  = TempVltC21      C bottom
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
void RotorTempConversionT3( void )
{

    // Brg1 Rotor A1
    //--------------
    if ( !R_BridgeRebf.RebhSet)
    {
        // Brg1 Rotor A1
        //--------------
        TempA21Raw = static_cast<int> (VCO_MASK & R_Brg1TempCntsT3[0]);
        TempA21Frq = (static_cast<float> (TempA21Raw)) * R_TempVcoDelTmRcp;
        TempVltA21 = (TempA21Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg1 Rotor B1
        TempB21Raw = static_cast<int> (VCO_MASK & R_Brg1TempCntsT3[2]);
        TempB21Frq = (static_cast<float> (TempB21Raw)) * R_TempVcoDelTmRcp;
        TempVltB21 = (TempB21Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg1 Rotor C1
        TempC21Raw = static_cast<int> (VCO_MASK & R_Brg1TempCntsT3[4]);
        TempC21Frq = (static_cast<float> (TempC21Raw)) * R_TempVcoDelTmRcp;
        TempVltC21 = (TempC21Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg1 Rotor DB1
        TempD21Raw = static_cast<int> (VCO_MASK & R_Brg1TempCntsT3[6]);
        TempD21Frq = (static_cast<float> (TempD21Raw)) * R_TempVcoDelTmRcp;
        TempVltD21 = (TempD21Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg2 Rotor A1
        //--------------
        TempA31Raw = static_cast<int> (VCO_MASK & R_Brg2TempCntsT3[0]);
        TempA31Frq = (static_cast<float> (TempA31Raw)) * R_TempVcoDelTmRcp;
        TempVltA31 = (TempA31Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg2 Rotor B1
        TempB31Raw = static_cast<int> (VCO_MASK & R_Brg2TempCntsT3[2]);
        TempB31Frq = (static_cast<float> (TempB31Raw)) * R_TempVcoDelTmRcp;
        TempVltB31 = (TempB31Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg2 Rotor C1
        TempC31Raw = static_cast<int> (VCO_MASK & R_Brg2TempCntsT3[4]);
        TempC31Frq = (static_cast<float> (TempC31Raw)) * R_TempVcoDelTmRcp;
        TempVltC31 = (TempC31Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg2 Rotor DB1
        TempD31Raw = static_cast<int> (VCO_MASK & R_Brg2TempCntsT3[6]);
        TempD31Frq = (static_cast<float> (TempD31Raw)) * R_TempVcoDelTmRcp;
        TempVltD31 = (TempD31Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg1 Rotor A2
        //--------------
        TempA22Raw = static_cast<int> (VCO_MASK & R_Brg1TempCntsT3[1]);
        TempA22Frq = (static_cast<float> (TempA22Raw)) * R_TempVcoDelTmRcp;
        TempVltA22 = (TempA22Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg1 Rotor B2
        TempB22Raw = static_cast<int> (VCO_MASK & R_Brg1TempCntsT3[3]);
        TempB22Frq = (static_cast<float> (TempB22Raw)) * R_TempVcoDelTmRcp;
        TempVltB22 = (TempB22Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg1 Rotor C2
        TempC22Raw = static_cast<int> (VCO_MASK & R_Brg1TempCntsT3[5]);
        TempC22Frq = (static_cast<float> (TempC22Raw)) * R_TempVcoDelTmRcp;
        TempVltC22 = (TempC22Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg1 Rotor DB2
        TempD22Raw = static_cast<int> (VCO_MASK & R_Brg1TempCntsT3[7]);
        TempD22Frq = (static_cast<float> (TempD22Raw)) * R_TempVcoDelTmRcp;
        TempVltD22 = (TempD22Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg2 Rotor A2
        //--------------
        TempA32Raw = static_cast<int> (VCO_MASK & R_Brg2TempCntsT3[1]);
        TempA32Frq = (static_cast<float> (TempA32Raw)) * R_TempVcoDelTmRcp;
        TempVltA32 = (TempA32Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg2 Rotor B2
        TempB32Raw = static_cast<int> (VCO_MASK & R_Brg2TempCntsT3[3]);
        TempB32Frq = (static_cast<float> (TempB32Raw)) * R_TempVcoDelTmRcp;
        TempVltB32 = (TempB32Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg2 Rotor C2
        TempC32Raw = static_cast<int> (VCO_MASK & R_Brg2TempCntsT3[5]);
        TempC32Frq = (static_cast<float> (TempC32Raw)) * R_TempVcoDelTmRcp;
        TempVltC32 = (TempC32Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);

        // Brg2 Rotor DB2
        TempD32Raw = static_cast<int> (VCO_MASK & R_Brg2TempCntsT3[7]);
        TempD32Frq = (static_cast<float> (TempD32Raw)) * R_TempVcoDelTmRcp;
        TempVltD32 = (TempD32Frq - PARM(TempVcoZero)) * PARM(TempVcoScl);
    }
    else
    {
        TempVltA21 = (float)R_Brg1TempCntsT3[0]*5/255;
        TempVltB21 = (float)R_Brg1TempCntsT3[2]*5/255;
        TempVltC21 = (float)R_Brg1TempCntsT3[4]*5/255;
        TempVltD21 = (float)R_Brg1TempCntsT3[6]*5/255;
        TempVltA31 = (float)R_Brg2TempCntsT3[0]*5/255;
        TempVltB31 = (float)R_Brg2TempCntsT3[2]*5/255;
        TempVltC31 = (float)R_Brg2TempCntsT3[4]*5/255;
        TempVltD31 = (float)R_Brg2TempCntsT3[6]*5/255;
        TempVltA22 = (float)R_Brg1TempCntsT3[1]*5/255;
        TempVltB22 = (float)R_Brg1TempCntsT3[3]*5/255;
        TempVltC22 = (float)R_Brg1TempCntsT3[5]*5/255;
        TempVltD22 = (float)R_Brg1TempCntsT3[7]*5/255;
        TempVltA32 = (float)R_Brg2TempCntsT3[1]*5/255;
        TempVltB32 = (float)R_Brg2TempCntsT3[3]*5/255;
        TempVltC32 = (float)R_Brg2TempCntsT3[5]*5/255;
        TempVltD32 = (float)R_Brg2TempCntsT3[7]*5/255;
    }



    ///////////////////////////////////////////////////////////////////////
    //  Rotor converter Vco to Temperature Conversion (BTM_011)
    ///////////////////////////////////////////////////////////////////////

    R_VTemp1  = TempVltA21;
    R_VTemp2  = TempVltB21;
    R_VTemp3  = TempVltC21;
    R_VTemp4  = TempVltA22;
    R_VTemp5  = TempVltB22;
    R_VTemp6  = TempVltC22;
    R_VTemp7  = TempVltD21;
    R_VTemp8  = TempVltD22;

    return;
}

