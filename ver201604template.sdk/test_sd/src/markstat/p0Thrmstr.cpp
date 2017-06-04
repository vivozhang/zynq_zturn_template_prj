///////////////////////////////////////////////////////////////////////////////
//
// TITLE     Thermistor processing.
//
// DESCRIPTION
//           This file contains the methods for thermistor processing.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      p0Thrmstr.vsd, p0Thrmstr.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
#include <math.h>
#include <string.h>
// core
// framework
// product
#include "Product.h"
#include "p0Thrmstr.h"


// Constants
//----------


// Static Class Members (by default, compiler initializes them to zero
//                       if no explicit initialization is provided):
//--------------------------------------------------------------------

//       Temperature Table for Thermistors 1,3,4, LOW RANGE temperature VCO volt to temp conversion

const float cThrmstr::TempTabLowT1[LOW_TEMP_DATA_ARRAY_SIZE] =
                            {-LWPT0,  -LWPT1,  -59.58,  -56.58,  -54.06,  -51.89,  -49.97,  -48.25,  -46.68,  -45.25,
                             -43.92,  -42.68,  -41.52,  -40.42,  -39.39,  -38.40,  -37.47,  -36.57,  -35.71,  -34.88,
                             -34.09,  -33.32,  -32.58,  -31.86,  -31.16,  -30.49,  -29.83,  -29.19,  -28.56,  -27.95,
                             -27.36,  -26.78,  -26.21,  -25.65,  -25.11,  -24.57,  -24.05,  -23.53,  -23.02,  -22.52,
                             -22.03,  -21.55,  -21.07,  -20.61,  -20.14,  -19.69,  -19.24,  -18.8,   -18.36,  -17.92,
                             -17.50,  -17.07,  -16.65,  -16.24,  -15.83,  -15.42,  -15.02,  -14.62,  -14.23,  -13.84,
                             -13.45,  -13.07,  -12.69,  -12.31,  -11.93,  -11.56,  -11.19,  -10.82,  -10.46,  -10.09,
                              -9.73,   -9.37,   -9.02,   -8.66,   -8.31,   -7.96,   -7.61,   -7.26,   -6.91,   -6.57,
                              -6.23,   -5.89,   -5.55,   -5.21,   -4.87,   -4.53,   -4.20,   -3.86,   -3.53,   -3.19,
                              -2.86,   -2.53,   -2.20,   -1.87,   -1.54,   -1.21,   -0.88,   -0.56,   -0.23,    0.10,
                               0.42,    0.75,    1.07,    1.40,    1.73,    2.05,    2.38,    2.70,    3.03,    3.35,
                               3.68,    4.00,    4.33,    4.65,    4.98,    5.30,    5.63,    5.96,    6.29,    6.61,
                               6.94,    7.27,    7.60,    7.93,    8.26,    8.59,    8.93,    9.26,    9.59,    9.93,
                              10.26,   10.60,   10.94,   11.28,   11.62,   11.96,   12.30,   12.65,   12.99,   13.34,
                              13.69,   14.04,   14.39,   14.74,   15.10,   15.46,   15.82,   16.18,   16.54,   16.90,
                              17.27,   17.64,   18.01,   18.38,   18.76,   19.14,   19.52,   19.90,   20.29,   20.68,
                              21.07,   21.47,   21.87,   22.27,   22.67,   23.08,   23.49,   23.91,   24.33,   24.75,
                              25.18,   25.61,   26.05,   26.49,   26.93,   27.38,   27.84,   28.30,   28.76,   29.23,
                              29.71,   30.19,   30.68,   31.17,   31.67,   32.18,   32.70,   33.22,   33.75,   34.29,
                              34.83,   35.39,   35.95,   36.52,   37.10,   37.7,    38.30,   38.91,   39.54,   40.18,
                              40.83,   41.49,   42.17,   42.86,   43.56,   44.29,   45.03,   45.79,   46.56,   47.36,
                              48.18,   49.02,   49.88,   50.77,   51.69,   52.63,   53.61,   54.62,   55.66,   56.74,
                              57.87,   59.04,   60.25,   61.52,   62.85,   64.24,   65.70,   67.24,   68.87,   70.59,
                              72.42,   74.37,   76.46,   78.71,   81.16,   83.82,   86.75,   89.99,   93.64,   97.78,
                             102.57,  108.23,  115.14,  123.93,  135.87,  154.13}; //last element is for inputs greater than 246

//       Temperature Table for Thermistors 1,3,4, HIGH RANGE temperature VCO volt to temp conversion

const float cThrmstr::TempTabHighT1[HI_TEMP_DATA_ARRAY_SIZE] =
                            {-20.71,   -8.26,  -0.44,     5.39,   10.07,   14.03,   17.46,   20.51,   23.27,   25.78,
                              28.10,   30.25,   32.27,   34.17,   35.97,   37.68,   39.30,   40.86,   42.36,   43.79,
                              45.18,   46.52,   47.81,   49.07,   50.29,   51.48,   52.64,   53.76,   54.86,   55.94,
                              57.00,   58.03,   59.04,   60.03,   61.01,   61.97,   62.91,   63.84,   64.75,   65.65,
                              66.54,   67.42,   68.28,   69.14,   69.98,   70.82,   71.64,   72.46,   73.27,   74.07,
                              74.87,   75.65,   76.43,   77.21,   77.98,   78.74,   79.50,   80.25,   81.00,   81.74,
                              82.48,   83.22,   83.95,   84.68,   85.40,   86.12,   86.84,   87.56,   88.27,   88.98,
                              89.69,   90.40,   91.10,   91.80,   92.51,   93.21,   93.91,   94.61,   95.30,   96.00,
                              96.70,   97.39,   98.09,   98.79,   99.48,  100.18,  100.87,  101.57,  102.27,  102.97,
                             103.67,  104.37,  105.07,  105.77,  106.48,  107.19,  107.89,  108.60,  109.32,  110.03,
                             110.75,  111.47,  112.19,  112.92,  113.65,  114.38,  115.11,  115.85,  116.59,  117.34,
                             118.09,  118.85,  119.61,  120.37,  121.14,  121.91,  122.69,  123.48,  124.27,  125.07,
                             125.87,  126.68,  127.50,  128.32,  129.15,  129.99,  130.84,  131.69,  132.55,  133.43,
                             134.31,  135.20,  136.10,  137.01,  137.93,  138.86,  139.81,  140.77,  141.74,  142.72,
                             143.71,  144.72,  145.75,  146.79,  147.85,  148.92,  150.01,  151.12,  152.25,  153.40
                             }; //last element is for inputs greater than 150




// Externals
//----------


// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Variables
//----------


// Member Functions
//-----------------



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
cThrmstr::cThrmstr
(
        InListTyp    InList,
        InsListTyp   InsList
)
        : In(InList), Ins(InsList)
{
    TempFilStlTmr      = 0.0;
    TempVTh1LwFl       = 0.0;
    TempVTh2LwFl       = 0.0;
    TempVTh7LwFl       = 0.0;
    TempVTh8LwFl       = 0.0;
    TempVTh1HiFl       = 0.0;
    TempVTh2HiFl       = 0.0;
    TempVTh7HiFl       = 0.0;
    TempVTh8HiFl       = 0.0;

    TempVTh1LwFlSt     = 0.0;
    TempVTh2LwFlSt     = 0.0;
    TempVTh7LwFlSt     = 0.0;
    TempVTh8LwFlSt     = 0.0;
    TempVTh1HiFlSt     = 0.0;
    TempVTh2HiFlSt     = 0.0;
    TempVTh7HiFlSt     = 0.0;
    TempVTh8HiFlSt     = 0.0;

    OUT(Thrmstr1Temp)  = 0.0;
    OUT(Thrmstr2Temp)  = 0.0;
    OUT(Thrmstr7Temp)  = 0.0;
    OUT(Thrmstr8Temp)  = 0.0;
    OUT(IOThrm1DlFlg)  = false;
    OUT(IOThrm2DlFlg)  = false;
    OUT(IOThrm7DlFlg)  = false;
    OUT(IOThrm8DlFlg)  = false;

    OUT(Thrm1OpenFlg)  = false;
    OUT(Thrm2OpenFlg)  = false;
    OUT(Thrm7OpenFlg)  = false;
    OUT(Thrm8OpenFlg)  = false;

    OUT(Thrm1ShrtFlg)  = false;
    OUT(Thrm2ShrtFlg)  = false;
    OUT(Thrm7ShrtFlg)  = false;
    OUT(Thrm8ShrtFlg)  = false;
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
cThrmstr::~cThrmstr()
{
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by thermistor processing class.
//
// EXECUTION LEVEL
//      Initialization and when required.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cThrmstr::RunRules(
    //Inputs
    float DeltaTime,    // delta time for execution loop (Sec)
    float IoThrmstrW    // Omega for filtering IO thermisters (Rad/Sec)
)
{
    int Status = SUCCESS;

    // note these final values and variables are static class members
    // i.e. they are not multiply instanced

    SET_FV(ThrmTempLresMeld,   10000.0F);
    SET_FV(ThrmTempHresMeld,    6530.0F);
    SET_FV(ThrmTempCkMax,        130.0F);
    SET_FV(ThrmTempCkMin,        -20.0F);
    SET_FV(ThrmTempDelMax,        15.0F);
    SET_FV(ThrmTempDelMaxC,       17.0F);
    SET_FV(ThrmTempDelSelThr,     -5.0F);
    SET_FV(ThrmTempDelFltTm,       1.0F);
    SET_FV(ThrmOpenOnThrsh,      0.144F);  // spec rev 14
    SET_FV(ThrmShrtOnThrsh,        3.5F);  // SEP thermistors: maximum of 2.967 V when each is set to 150 deg C,
                                           // and a minimum of 4.024 V when shorted. 3.5 V is halfway between these numbers
    SET_FV(ThrmOpenOffThrsh,       0.2F);
    SET_FV(ThrmShrtOffThrsh,       3.40F);

    // note this var already exist in p0rule2.cpp, need to remove from there
    TempRMldRcp  = NoNANLim( 1.0F / (PARM(ThrmTempLresMeld) - PARM(ThrmTempHresMeld)), 1.0e6F, -1.0e6F);

    TempVltGnFil = IoThrmstrW * DeltaTime;
    TempFilStlTm = NoNANLim( 5.0F / IoThrmstrW,  1.0e6F, -1.0e6F);

    return (Status);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      The purpose of this function is to convert four vco voltages
//      into four temperatures. These voltages are read by four identical
//      circuits on the AEAD board. For each thermistor, there are two
//      voltage readings, one for the high temperature range, and another for
//      the low temperature range. All of these voltages are converted to resistance,
//      based on the card circuit. Each of the eight readings is done within a task3
//      period by task 1 and 3 functions that converts vco frequency to voltage. Each
//      voltage is also converted to a temperature based on look-up tables. This function
//      then determines which converted temperature to use for each thermistor based on
//      the calculated resistance. There is a transition range of resistance (and therefore
//      temperature) for each thermistor type. When the feedback shows that resistance is
//      in this transition range, calculated high and low range temperatures are melded
//      together based on the resistance calculated from the high range reading.
//      This function also checks the delta between the temperature calculated at the
//      high range and low range, alarms are generated if delta is too high. These alarms
//      are intended to check for AEAD circuit problems, either in resistance or a problem
//      with the high/low selection switch.
//      There are, two different voltage to temperature look-up tables used here,
//      one for each temperature range.
//      The temperature look-up is by voltage instead of resistance because the volt-
//      age to temperature look-up is much more linear than the resistance to temperature
//      look-up. Using voltage allows use of a much more even distribution of voltage inputs.
//
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cThrmstr::Task3( void )
{
    float     ResMeldTemp;


    if ( INS(TempFbkEnb) ) //Only one AEAD  used for 1.6 IPNs
    {
       if (IN(TempFbkVld))
       {
          TempFilStlTmr += INS(DelTm3);
       }
       else
       {
          TempFilStlTmr  = 0.0F;
       }

       //-------------------------------------------------------------------------
       //  Filter voltage inputs from VCOs (BTM_015)
       TempVTh1LwFlSt += TempVltGnFil * ( IN(TempVltTh1Lw) - TempVTh1LwFlSt );
       TempVTh2LwFlSt += TempVltGnFil * ( IN(TempVltTh2Lw) - TempVTh2LwFlSt );
       TempVTh7LwFlSt += TempVltGnFil * ( IN(TempVltTh7Lw) - TempVTh7LwFlSt );
       TempVTh8LwFlSt += TempVltGnFil * ( IN(TempVltTh8Lw) - TempVTh8LwFlSt );

       TempVTh1HiFlSt += TempVltGnFil * ( IN(TempVltTh1Hi) - TempVTh1HiFlSt );
       TempVTh2HiFlSt += TempVltGnFil * ( IN(TempVltTh2Hi) - TempVTh2HiFlSt );
       TempVTh7HiFlSt += TempVltGnFil * ( IN(TempVltTh7Hi) - TempVTh7HiFlSt );
       TempVTh8HiFlSt += TempVltGnFil * ( IN(TempVltTh8Hi) - TempVTh8HiFlSt );

       //-------------------------------------------------------------------------
       // copy the filter output for the clamp and temperature calculation
       TempVTh1LwFl = TempVTh1LwFlSt;
       TempVTh2LwFl = TempVTh2LwFlSt;
       TempVTh7LwFl = TempVTh7LwFlSt;
       TempVTh8LwFl = TempVTh8LwFlSt;

       TempVTh1HiFl = TempVTh1HiFlSt;
       TempVTh2HiFl = TempVTh2HiFlSt;
       TempVTh7HiFl = TempVTh7HiFlSt;
       TempVTh8HiFl = TempVTh8HiFlSt;

       //-------------------------------------------------------------------------
       // Hold off until temperature feedback filters have time to settle
       if (TempFilStlTmr < TempFilStlTm) return;

       //-------------------------------------------------------------------------
       // check the open thermistor
       ThrmstrOpenCk();

       //-------------------------------------------------------------------------
       // check the short thermistor
       ThrmstrShrtCk();

       //-------------------------------------------------------------------------
       // Clamp  Low Range Vco filtered voltages to resistance Conversion, Sensors 1-4 (BTM_020,025))
       if ( TempVTh1LwFl < LOW_RNG_VLT_MIN)
       {
           TempVTh1LwFl = LOW_RNG_VLT_MIN;
       }
       else if (TempVTh1LwFl > LOW_RNG_VLT_MAX)
       {
           TempVTh1LwFl = LOW_RNG_VLT_MAX;
       }

       if ( TempVTh2LwFl < LOW_RNG_VLT_MIN)
       {
           TempVTh2LwFl = LOW_RNG_VLT_MIN;
       }
       else if (TempVTh2LwFl > LOW_RNG_VLT_MAX)
       {
           TempVTh2LwFl = LOW_RNG_VLT_MAX;
       }

       if ( TempVTh7LwFl < LOW_RNG_VLT_MIN)
       {
           TempVTh7LwFl = LOW_RNG_VLT_MIN;
       }
       else if (TempVTh7LwFl > LOW_RNG_VLT_MAX)
       {
           TempVTh7LwFl = LOW_RNG_VLT_MAX;
       }

       if ( TempVTh8LwFl < LOW_RNG_VLT_MIN)
       {
           TempVTh8LwFl = LOW_RNG_VLT_MIN;
       }
       else if (TempVTh8LwFl > LOW_RNG_VLT_MAX)
       {
           TempVTh8LwFl = LOW_RNG_VLT_MAX;
       }

       //-------------------------------------------------------------------------
       // Clamp  High Range Vco filtered voltages to resistance Conversion, Sensors 1-4 (BTM_020,025))
       if ( TempVTh1HiFl < HI_RNG_VLT_MIN)
       {
           TempVTh1HiFl = HI_RNG_VLT_MIN;
       }
       else if (TempVTh1HiFl > HI_RNG_VLT_MAX)
       {
           TempVTh1HiFl = HI_RNG_VLT_MAX;
       }

       if ( TempVTh2HiFl < HI_RNG_VLT_MIN)
       {
           TempVTh2HiFl = HI_RNG_VLT_MIN;
       }
       else if (TempVTh2HiFl > HI_RNG_VLT_MAX)
       {
           TempVTh2HiFl = HI_RNG_VLT_MAX;
       }

       if ( TempVTh7HiFl < HI_RNG_VLT_MIN)
       {
           TempVTh7HiFl = HI_RNG_VLT_MIN;
       }
       else if (TempVTh7HiFl > HI_RNG_VLT_MAX)
       {
           TempVTh7HiFl = HI_RNG_VLT_MAX;
       }

       if ( TempVTh8HiFl < HI_RNG_VLT_MIN)
       {
           TempVTh8HiFl = HI_RNG_VLT_MIN;
       }
       else if (TempVTh8HiFl > HI_RNG_VLT_MAX)
       {
           TempVTh8HiFl = HI_RNG_VLT_MAX;
       }

       //-------------------------------------------------------------------------
       // IO Thermister Vco voltage to resistance Conversion, Sensors 1-4 (BTM_020,025))
       TempResTh1Lw = ( R_LOW_RANGE * (V_IN_NOM - TempVTh1LwFl) - (R_CKT_SERIES * TempVTh1LwFl) )/ TempVTh1LwFl;
       TempResTh1Hi = ( R_HI_RANGE *  (V_IN_NOM - TempVTh1HiFl) - (R_CKT_SERIES * TempVTh1HiFl) )/ TempVTh1HiFl;

       TempResTh2Lw = ( R_LOW_RANGE * (V_IN_NOM - TempVTh2LwFl) - (R_CKT_SERIES * TempVTh2LwFl) )/ TempVTh2LwFl;
       TempResTh2Hi = ( R_HI_RANGE *  (V_IN_NOM - TempVTh2HiFl) - (R_CKT_SERIES * TempVTh2HiFl) )/ TempVTh2HiFl;

       TempResTh7Lw = ( R_LOW_RANGE * (V_IN_NOM - TempVTh7LwFl) - (R_CKT_SERIES * TempVTh7LwFl) )/ TempVTh7LwFl;
       TempResTh7Hi = ( R_HI_RANGE *  (V_IN_NOM - TempVTh7HiFl) - (R_CKT_SERIES * TempVTh7HiFl) )/ TempVTh7HiFl;

       TempResTh8Lw = ( R_LOW_RANGE * (V_IN_NOM - TempVTh8LwFl) - (R_CKT_SERIES * TempVTh8LwFl) )/ TempVTh8LwFl;
       TempResTh8Hi = ( R_HI_RANGE *  (V_IN_NOM - TempVTh8HiFl) - (R_CKT_SERIES * TempVTh8HiFl) )/ TempVTh8HiFl;

       //-------------------------------------------------------------------------
       //Test code for engineering monitor.... use to check out table look-up
       Test20= TempTabLowT1[TestIn20];
       Test21= TempTabHighT1[TestIn21];

       //-------------------------------------------------------------------------
       // Calculate temperature of AEAD Din 1 Thermistor(BTM_020)


       // AEAD Din 1 Thermistor - LOW: Determine index into temperature table as a function of voltage*/

       if ( TempVTh1LwFl < LOW_RNG_VLT_PT1)//if voltage less than 2nd array member, use a different slope
       {
           TempTh1Lw =  TempTabLowT1[1] - LOW_DEL_PT1_VOLT*(LOW_RNG_VLT_PT1 - TempVTh1LwFl);
       }
       else
       {                 //(4.0678   -  0.26)*50 + 1   = 190.39,   TEMP1 =  40.83 + .37(41.49 - 40.76)  = 41.03
          TempIdx    = (TempVTh1LwFl - LOW_RNG_VLT_PT1)*( LOW_RNG_TBL_DELP ) + 1.0F;  //begin at memeber for all but 1st member
          TempIdxFrc = modf( TempIdx, &TempIdxF );
          TempIdxI   = (int)TempIdxF;


           // Interpolate between data points to find temperature */
          if (TempIdxI >= LOW_TEMP_DATA_PTS )
          {
              TempTh1Lw = TempTabLowT1[LOW_TEMP_DATA_PTS];
          }
          else
          {
              TempPt1   = TempTabLowT1[TempIdxI];
              TempPt2   = TempTabLowT1[TempIdxI+1];
              TempTh1Lw = TempPt1 + TempIdxFrc * ( TempPt2 - TempPt1 );
          }
       }

       //-------------------------------------------------------------------------
       // Calculate HIGH range temperatures for AEAD Din 1 Thermistor

       // AEAD Din 1 Thermistor - HIGH: Determine index into temperature table as a function of voltage*/
       if ( TempVTh1HiFl < HI_RNG_VLT_MIN)//if voltage less than 1st array member, clamp to first member Temp (should not happen)
       {
           TempTh1Hi =  TempTabHighT1[0];
       }
       else
       {
          TempIdx    = (TempVTh1HiFl - HI_RNG_VLT_MIN)*( HI_RNG_TBL_DELP );  //begin at member 0
          TempIdxFrc = modf( TempIdx, &TempIdxF );
          TempIdxI   = (int)TempIdxF;


           // Interpolate between data points to find temperature */
          if (TempIdxI >= HI_TEMP_DATA_PTS )
          {
              TempTh1Hi = TempTabHighT1[HI_TEMP_DATA_PTS];
          }
          else
          {
              TempPt1   = TempTabHighT1[TempIdxI];
              TempPt2   = TempTabHighT1[TempIdxI+1];
              TempTh1Hi= TempPt1 + TempIdxFrc * ( TempPt2 - TempPt1 );
          }
       }

       //-------------------------------------------------------------------------
       // Choose AEAD Din 1 Thermistor temperature based on Resistance Range...
       // Meld if in meld range

       //AEAD Din 1 Thermistor Meld -- Form the meld output all the time
       ResMeldTemp = TempRMldRcp * (PARM(ThrmTempLresMeld) - TempResTh1Lw);
       if ( ResMeldTemp > 1.0F)
       {
           ResMeldTemp = 1.0F;
       }
       else if (ResMeldTemp < 0.0F)
       {
           ResMeldTemp = 0.0F;
       }
       TempTh1Meld = TempTh1Lw*(1.0F - ResMeldTemp) + TempTh1Hi*ResMeldTemp;

       //Do the selection
       if (TempResTh1Lw >= PARM(ThrmTempLresMeld))
       {
           OUT(Thrmstr1Temp) = TempTh1Lw;
       }
       else if ( TempResTh1Hi <= PARM(ThrmTempHresMeld))
       {
           OUT(Thrmstr1Temp) = TempTh1Hi;
       }
       else //meld between the two
       {
          OUT(Thrmstr1Temp) = TempTh1Meld;
       }

       //-------------------------------------------------------------------------
       // Calculate temperature of thermistor #3 (BTM_020)

       // AEAD Din 7 Thermistor - LOW: Determine index into temperature table as a function of voltage*/

       if ( TempVTh7LwFl < LOW_RNG_VLT_PT1)//if voltage less than 2nd array member, use a different slope
       {                    //  -42.68    -               439.886   ( .26     -         0.00001)  = -157.05
           TempTh7Lw =  TempTabLowT1[1] - LOW_DEL_PT1_VOLT*(LOW_RNG_VLT_PT1 - TempVTh7LwFl);
       }
       else
       {
          TempIdx    = (TempVTh7LwFl - LOW_RNG_VLT_PT1)*( LOW_RNG_TBL_DELP ) + 1.0F;  //begin at memeber for all but 1st member
          TempIdxFrc = modf( TempIdx, &TempIdxF );
          TempIdxI   = (int)TempIdxF;


           // Interpolate between data points to find temperature */
          if (TempIdxI >= LOW_TEMP_DATA_PTS )
          {
              TempTh7Lw = TempTabLowT1[LOW_TEMP_DATA_PTS];
          }
          else
          {
              TempPt1   = TempTabLowT1[TempIdxI];
              TempPt2   = TempTabLowT1[TempIdxI+1];
              TempTh7Lw = TempPt1 + TempIdxFrc * ( TempPt2 - TempPt1 );
          }
       }

       //-------------------------------------------------------------------------
       // AEAD Din 7 Thermistor - HIGH: Determine index into temperature table as a function of voltage*/
       if ( TempVTh1HiFl < HI_RNG_VLT_MIN)//if voltage less than 1st array member, clamp to first member Temp (should not happen)
       {
           TempTh7Hi =  TempTabHighT1[0];
       }
       else
       {
          TempIdx    = (TempVTh7HiFl - HI_RNG_VLT_MIN)*( HI_RNG_TBL_DELP );  //begin at member 0
          TempIdxFrc = modf( TempIdx, &TempIdxF );
          TempIdxI   = (int)TempIdxF;


           // Interpolate between data points to find temperature */
          if (TempIdxI >= HI_TEMP_DATA_PTS )
          {
              TempTh7Hi = TempTabHighT1[HI_TEMP_DATA_PTS];
          }
          else
          {
              TempPt1   = TempTabHighT1[TempIdxI];
              TempPt2   = TempTabHighT1[TempIdxI+1];
              TempTh7Hi= TempPt1 + TempIdxFrc * ( TempPt2 - TempPt1 );
          }
       }

       //-------------------------------------------------------------------------
       //AEAD Din 7 Thermistor Meld -- Form the meld output all the time
       ResMeldTemp = TempRMldRcp * (PARM(ThrmTempLresMeld) - TempResTh7Lw);
       if ( ResMeldTemp > 1.0F)
       {
           ResMeldTemp = 1.0F;
       }
       else if (ResMeldTemp < 0.0F)
       {
           ResMeldTemp = 0.0F;
       }
       TempTh7Meld = TempTh7Lw*(1.0F - ResMeldTemp) + TempTh7Hi*ResMeldTemp;

       //Do the selection
       if (TempResTh7Lw >= PARM(ThrmTempLresMeld))
       {
           OUT(Thrmstr7Temp) = TempTh7Lw;
       }
       else if ( TempResTh7Hi <= PARM(ThrmTempHresMeld))
       {
           OUT(Thrmstr7Temp) = TempTh7Hi;
       }
       else //meld between the two
       {
          OUT(Thrmstr7Temp) = TempTh7Meld;
       }

       //-------------------------------------------------------------------------
       // Calculate temperature of thermistor #4 (BTM_020)

       // AEAD Din 8 Thermistor -- LOW: Determine index into temperature table as a function of voltage*/

       if ( TempVTh8LwFl < LOW_RNG_VLT_PT1)//if voltage less than 2nd array member, use a different slope
       {
           TempTh8Lw =  TempTabLowT1[1] - LOW_DEL_PT1_VOLT*(LOW_RNG_VLT_PT1 - TempVTh8LwFl);
       }
       else
       {
          TempIdx    = (TempVTh8LwFl - LOW_RNG_VLT_PT1)*( LOW_RNG_TBL_DELP ) + 1.0F;  //begin at memeber for all but 1st member
          TempIdxFrc = modf( TempIdx, &TempIdxF );
          TempIdxI   = (int)TempIdxF;


           // Interpolate between data points to find temperature */
          if (TempIdxI >= LOW_TEMP_DATA_PTS )
          {
              TempTh8Lw = TempTabLowT1[LOW_TEMP_DATA_PTS];
          }
          else
          {
              TempPt1   = TempTabLowT1[TempIdxI];
              TempPt2   = TempTabLowT1[TempIdxI+1];
              TempTh8Lw = TempPt1 + TempIdxFrc * ( TempPt2 - TempPt1 );
          }
       }

       //-------------------------------------------------------------------------
       // AEAD Din 8 Thermistor - HIGH: Determine index into temperature table as a function of voltage*/
       if ( TempVTh8HiFl < HI_RNG_VLT_MIN)//if voltage less than 1st array member, clamp to first member Temp (should not happen)
       {
           TempTh8Hi =  TempTabHighT1[0];
       }
       else
       {
          TempIdx    = (TempVTh8HiFl - HI_RNG_VLT_MIN)*( HI_RNG_TBL_DELP );  //begin at member 0
          TempIdxFrc = modf( TempIdx, &TempIdxF );
          TempIdxI   = (int)TempIdxF;

           // Interpolate between data points to find temperature */
          if (TempIdxI >= HI_TEMP_DATA_PTS )
          {
              TempTh8Hi = TempTabHighT1[HI_TEMP_DATA_PTS];
          }
          else
          {
              TempPt1   = TempTabHighT1[TempIdxI];
              TempPt2   = TempTabHighT1[TempIdxI+1];
              TempTh8Hi= TempPt1 + TempIdxFrc * ( TempPt2 - TempPt1 );
          }
       }

       //-------------------------------------------------------------------------
       //AEAD Din 8 Thermistor Meld -- Form the meld output all the time
       ResMeldTemp = TempRMldRcp * (PARM(ThrmTempLresMeld) - TempResTh8Lw);
       if ( ResMeldTemp > 1.0F)
       {
           ResMeldTemp = 1.0F;
       }
       else if (ResMeldTemp < 0.0F)
       {
           ResMeldTemp = 0.0F;
       }
       TempTh8Meld = TempTh8Lw*(1.0F - ResMeldTemp) + TempTh8Hi*ResMeldTemp;

       //Do the selection
       if (TempResTh8Lw >= PARM(ThrmTempLresMeld))
       {
           OUT(Thrmstr8Temp) = TempTh8Lw;
       }
       else if ( TempResTh8Hi <= PARM(ThrmTempHresMeld))
       {
           OUT(Thrmstr8Temp) = TempTh8Hi;
       }
       else //meld between the two
       {
          OUT(Thrmstr8Temp) = TempTh8Meld;
       }

       //-------------------------------------------------------------------------
       // Calculate temperature of thermistor #2 (BTM_020)

       // AEAD Din 2 Thermistor -- LOW: Determine index into temperature table as a function of voltage*/

       if ( TempVTh2LwFl < LOW_RNG_VLT_PT1)//if voltage less than 2nd array member, use a different slope
       {
           TempTh2Lw =  TempTabLowT1[1] - LOW_DEL_PT1_VOLT*(LOW_RNG_VLT_PT1 - TempVTh2LwFl);
       }
       else
       {
          TempIdx    = (TempVTh2LwFl - LOW_RNG_VLT_PT1)*( LOW_RNG_TBL_DELP ) + 1.0F;  //begin at memeber for all but 1st member
          TempIdxFrc = modf( TempIdx, &TempIdxF );
          TempIdxI   = (int)TempIdxF;


           // Interpolate between data points to find temperature */
          if (TempIdxI >= LOW_TEMP_DATA_PTS )
          {
              TempTh2Lw = TempTabLowT1[LOW_TEMP_DATA_PTS];
          }
          else
          {
              TempPt1   = TempTabLowT1[TempIdxI];
              TempPt2   = TempTabLowT1[TempIdxI+1];
              TempTh2Lw = TempPt1 + TempIdxFrc * ( TempPt2 - TempPt1 );
          }
       }

       //-------------------------------------------------------------------------
       // AEAD Din 2 Thermistor - HIGH: Determine index into temperature table as a function of voltage*/
       if ( TempVTh2HiFl < HI_RNG_VLT_MIN)//if voltage less than 1st array member, clamp to first member Temp (should not happen)
       {
           TempTh2Hi =  TempTabHighT1[0];
       }
       else
       {
          TempIdx    = (TempVTh2HiFl - HI_RNG_VLT_MIN)*( HI_RNG_TBL_DELP );  //begin at member 0
          TempIdxFrc = modf( TempIdx, &TempIdxF );
          TempIdxI   = (int)TempIdxF;

           // Interpolate between data points to find temperature */
          if (TempIdxI >= HI_TEMP_DATA_PTS )
          {
              TempTh2Hi = TempTabHighT1[HI_TEMP_DATA_PTS];
          }
          else
          {
              TempPt1   = TempTabHighT1[TempIdxI];
              TempPt2   = TempTabHighT1[TempIdxI+1];
              TempTh2Hi= TempPt1 + TempIdxFrc * ( TempPt2 - TempPt1 );
          }
       }

       //-------------------------------------------------------------------------
       //AEAD Din 2 Thermistor Meld -- Form the meld output all the time
       ResMeldTemp = TempRMldRcp * (PARM(ThrmTempLresMeld) - TempResTh2Lw);
       if ( ResMeldTemp > 1.0F)
       {
           ResMeldTemp = 1.0F;
       }
       else if (ResMeldTemp < 0.0F)
       {
           ResMeldTemp = 0.0F;
       }
       TempTh2Meld = TempTh2Lw*(1.0F - ResMeldTemp) + TempTh2Hi*ResMeldTemp;

       //Do the selection
       if (TempResTh2Lw >= PARM(ThrmTempLresMeld))
       {
           OUT(Thrmstr2Temp) = TempTh2Lw;
       }
       else if ( TempResTh2Hi <= PARM(ThrmTempHresMeld))
       {
           OUT(Thrmstr2Temp) = TempTh2Hi;
       }
       else //meld between the two
       {
          OUT(Thrmstr2Temp) = TempTh2Meld;
       }

       //-------------------------------------------------------------------------
       // Thermal Delta Temperature/Resistance Fault Check for AEAD Din 1,2,7,8 Thermistor (BTM_030)
       OUT(IOThrm1DlFlg) = false;
       OUT(IOThrm2DlFlg) = false;
       OUT(IOThrm7DlFlg) = false;
       OUT(IOThrm8DlFlg) = false;

       TempDelTh1 = ABS(TempTh1Lw - TempTh1Hi);
       TempDelTh2 = ABS(TempTh2Lw - TempTh2Hi);
       TempDelTh7 = ABS(TempTh7Lw - TempTh7Hi);
       TempDelTh8 = ABS(TempTh8Lw - TempTh8Hi);

        if(IN(InletAirTemp)<PARM(ThrmTempDelSelThr))
        {
            ThrmTempDelMax = PARM(ThrmTempDelMaxC);
        }
        else
        {
            ThrmTempDelMax = PARM(ThrmTempDelMax);
        }

       //AEAD Din 1 Thermistor
       //if either high or low range resistance read is within range and the thermistor is in use, then enable this check
       if ( ( ( ( TempTh1Hi < PARM(ThrmTempCkMax)) && ( TempTh1Hi > PARM(ThrmTempCkMin)) )   ||
              ( ( TempTh1Lw < PARM(ThrmTempCkMax)) && ( TempTh1Lw > PARM(ThrmTempCkMin)) ) ) &&
              IN(UseThrmstr1))
       {
           //AEAD Din 1 Thermistor check
           if ( TempDelTh1  > ThrmTempDelMax )
           {
              TempDlTmrTh1 = TempDlTmrTh1 + INS(DelTm3);
              if ( TempDlTmrTh1 > PARM(ThrmTempDelFltTm) )
              {
                 PUSH_DIAG(IOThrm1DlAlm);
                 OUT(IOThrm1DlFlg) = true;
              }
           }
           else
           {
               TempDlTmrTh1 = 0.0F;
           }
       }
       else
       {
           TempDlTmrTh1 = 0.0F;
       }

       //-------------------------------------------------------------------------
       //AEAD Din 2 Thermistor
       //if either high or low range resistance read is within range and the thermistor is in use, then enable this check
       if ( ( ( ( TempTh2Hi < PARM(ThrmTempCkMax)) && ( TempTh2Hi > PARM(ThrmTempCkMin)) )   ||
              ( ( TempTh2Lw < PARM(ThrmTempCkMax)) && ( TempTh2Lw > PARM(ThrmTempCkMin)) ) ) &&
              IN(UseThrmstr2))
       {
           //AEAD Din 2 Thermistor check
           if ( TempDelTh2  > ThrmTempDelMax )
           {
              TempDlTmrTh2 = TempDlTmrTh2 + INS(DelTm3);
              if (TempDlTmrTh2 > PARM(ThrmTempDelFltTm) )
              {
                 PUSH_DIAG(IOThrm2DlAlm);
                 OUT(IOThrm2DlFlg) = true;
              }
           }
           else
           {
               TempDlTmrTh2 = 0.0F;
           }
       }
       else
       {
           TempDlTmrTh2 = 0.0F;
       }

       //-------------------------------------------------------------------------
       //AEAD Din 7 Thermistor
       //if either high or low range resistance read is within range, then enable this check
       if (  ( ( TempTh7Hi < PARM(ThrmTempCkMax)) && ( TempTh7Hi > PARM(ThrmTempCkMin)) )  ||
             ( ( TempTh7Lw < PARM(ThrmTempCkMax)) && ( TempTh7Lw > PARM(ThrmTempCkMin)) ) )
       {
           //AEAD Din 7 Thermistor check
           if ( TempDelTh7  > ThrmTempDelMax )
           {
              TempDlTmrTh7 = TempDlTmrTh7 + INS(DelTm3);
              if ( TempDlTmrTh7 > PARM(ThrmTempDelFltTm) )
              {
                  PUSH_DIAG(IOThrm7DlAlm);
                  OUT(IOThrm7DlFlg) = true;
              }
           }
           else
           {
               TempDlTmrTh7 = 0.0F;
           }
       }
       else
       {
           TempDlTmrTh7 = 0.0F;
       }

       //-------------------------------------------------------------------------
       //AEAD Din 8 Thermistor
       //if either high or low range resistance read is within range, then enable this check
       if (  ( ( TempTh8Hi < PARM(ThrmTempCkMax)) && ( TempTh8Hi > PARM(ThrmTempCkMin)) )  ||
             ( ( TempTh8Lw < PARM(ThrmTempCkMax)) && ( TempTh8Lw > PARM(ThrmTempCkMin)) ) )
       {
           //AEAD Din 8 Thermistor check
           if ( TempDelTh8  > ThrmTempDelMax )
           {
              TempDlTmrTh8 = TempDlTmrTh8 + INS(DelTm3);
              if ( TempDlTmrTh8 > PARM(ThrmTempDelFltTm) )
              {
                 PUSH_DIAG(IOThrm8DlAlm);
                 OUT(IOThrm8DlFlg) = true;
              }
           }
           else
           {
               TempDlTmrTh8 = 0.0F;
           }
       }
       else
       {
           TempDlTmrTh8 = 0.0F;
       }
    }
    //-------------------------------------------------------------------------
    // done
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      The purpose of this function is to check used thermistor inputs for
//      open circuit fault based on the low range thermistor voltage.
//      Thermistor open circuit flags are the output of the class.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cThrmstr::ThrmstrOpenCk( void )
{
    if ( INS(TempFbkEnb) ) //AEAD not used for 1.6 IPNs
    {
       if ( IN(UseThrmstr1) )
       {
           if ( TempVTh1LwFlSt < PARM(ThrmOpenOnThrsh) )
           {
               OUT(Thrm1OpenFlg) = true;
           }
           else if (TempVTh1LwFlSt >= PARM(ThrmOpenOffThrsh) )
           {
               OUT(Thrm1OpenFlg) = false;
           }
           if ( Thrm1OpenFlg )
           {
               PUSH_DIAG(Thrm1OpenAlm);
           }
       }

       if ( IN(UseThrmstr2) )
       {
           if ( TempVTh2LwFlSt < PARM(ThrmOpenOnThrsh) )
           {
               OUT(Thrm2OpenFlg) = true;
           }
           else if (TempVTh2LwFlSt >= PARM(ThrmOpenOffThrsh) )
           {
               OUT(Thrm2OpenFlg) = false;
           }
           if ( Thrm2OpenFlg )
           {
               PUSH_DIAG(Thrm2OpenAlm);
           }
       }

       if ( IN(UseThrmstr7) )
       {
           if ( TempVTh7LwFlSt < PARM(ThrmOpenOnThrsh) )
           {
               OUT(Thrm7OpenFlg) = true;
           }
           else if (TempVTh7LwFlSt >= PARM(ThrmOpenOffThrsh) )
           {
               OUT(Thrm7OpenFlg) = false;
           }
           if ( Thrm7OpenFlg )
           {
               PUSH_DIAG(Thrm7OpenAlm);
           }
       }

       if ( IN(UseThrmstr8) )
       {
           if ( TempVTh8LwFlSt < PARM(ThrmOpenOnThrsh) )
           {
               OUT(Thrm8OpenFlg) = true;
           }
           else if (TempVTh8LwFlSt >= PARM(ThrmOpenOffThrsh) )
           {
               OUT(Thrm8OpenFlg) = false;
           }
           if ( Thrm8OpenFlg )
           {
               PUSH_DIAG(Thrm8OpenAlm);
           }
       }
    }
    //-------------------------------------------------------------------------
    // done
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      The purpose of this function is to check all thermistor inputs, used
//      and unused, for short circuit fault based on the high range thermistor
//      voltage. Thermistor short circuit flags are the output of the class.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cThrmstr::ThrmstrShrtCk( void )
{

    if ( INS(TempFbkEnb) ) //Only one AEAD  used for 1.6 IPNs
    {

       //-------------------------------------------------------------------------
       //Short thermistor fault coordination
       if ( IN(UseThrmstr1))
       {
           if ( TempVTh1HiFlSt >= PARM(ThrmShrtOnThrsh) )
           {
               OUT(Thrm1ShrtFlg) = true;
           }
           else if ( TempVTh1HiFlSt < PARM(ThrmShrtOffThrsh) )
           {
               OUT(Thrm1ShrtFlg) = false;
           }
           if ( Thrm1ShrtFlg )
           {
               PUSH_DIAG(Thrm1ShrtAlm);
           }
       }
       if ( IN(UseThrmstr2))
       {
           if ( TempVTh2HiFlSt >= PARM(ThrmShrtOnThrsh) )
           {
               OUT(Thrm2ShrtFlg) = true;
           }
           else if ( TempVTh2HiFlSt < PARM(ThrmShrtOffThrsh) )
           {
               OUT(Thrm2ShrtFlg) = false;
           }
           if ( Thrm2ShrtFlg )
           {
               PUSH_DIAG(Thrm2ShrtAlm);
           }
       }
       if ( TempVTh7HiFlSt >= PARM(ThrmShrtOnThrsh) )
       {
           OUT(Thrm7ShrtFlg) = true;
       }
       else if ( TempVTh7HiFlSt < PARM(ThrmShrtOffThrsh) )
       {
           OUT(Thrm7ShrtFlg) = false;
       }
       if ( Thrm7ShrtFlg )
       {
           PUSH_DIAG(Thrm7ShrtAlm);
       }

       if ( TempVTh8HiFlSt >= PARM(ThrmShrtOnThrsh) )
       {
           OUT(Thrm8ShrtFlg) = true;
       }
       else if ( TempVTh8HiFlSt < PARM(ThrmShrtOffThrsh) )
       {
           OUT(Thrm8ShrtFlg) = false;
       }
       if ( Thrm8ShrtFlg )
       {
           PUSH_DIAG(Thrm8ShrtAlm);
       }
    }
    //-------------------------------------------------------------------------
    // done
    return;
}
