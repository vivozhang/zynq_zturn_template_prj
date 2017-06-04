///////////////////////////////////////////////////////////////////////////////
//
// TITLE     Bridge semiconductor thermistor processing.
//
// DESCRIPTION
//           This file contains the methods for thermistor processing.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0BrgThrmstr.vsd, b0BrgThrmstr.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
#include <math.h>
// core
// framework
// product
#include "Product.h"
#include "b0BrgThrmstr.h"


// Constants
//----------


// Static Class Members (by default, compiler initializes them to zero
//                       if no explicit initialization is provided):
//--------------------------------------------------------------------

// Temperature Table for conversion of bridge transistor temperature VCO voltage to temperature

// This table designed for input of 0 to 3.96875 volts
// Vco output can go to 4.0 volts, so input must be clamped to 3.96875 volts.
const float  cBrgThrmstr::TempTable[TEMP_DATA_ARRAY_SIZE] =
    {
            -159.82,  -45.41,  -34.12,  -26.93,  -21.51,  -17.12,  -13.39,  -10.12,   -7.20,
              -4.55,   -2.12,    0.13,    2.24,    4.22,    6.09,    7.88,    9.58,   11.21,
              12.77,   14.28,   15.74,   17.15,   18.52,   19.85,   21.14,   22.41,   23.65,
              24.86,   26.04,   27.20,   28.34,   29.46,   30.57,   31.65,   32.73,   33.78,
              34.83,   35.86,   36.88,   37.89,   38.89,   39.88,   40.86,   41.83,   42.80,
              43.76,   44.72,   45.66,   46.61,   47.55,   48.48,   49.42,   50.35,   51.27,
              52.20,   53.12,   54.04,   54.96,   55.88,   56.80,   57.72,   58.64,   59.56,
              60.49,   61.41,   62.34,   63.27,   64.20,   65.13,   66.07,   67.01,   67.96,
              68.91,   69.86,   70.83,   71.79,   72.77,   73.75,   74.73,   75.73,   76.73,
              77.74,   78.76,   79.79,   80.84,   81.89,   82.95,   84.02,   85.11,   86.21,
              87.33,   88.46,   89.60,   90.76,   91.94,   93.14,   94.35,   95.59,   96.84,
              98.12,   99.43,  100.76,  102.11,  103.49,  104.91,  106.35,  107.83,  109.34,
             110.89,  112.48,  114.12,  115.80,  117.53,  119.31,  121.15,  123.04,  125.01,
             127.04,  129.15,  131.35,  133.63,  136.02,  138.51,  141.12,  143.86,  146.75,
             149.81,  153.04,  153.04   //last element is for inputs greater than 128
    };


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
cBrgThrmstr::cBrgThrmstr
(
        InListTyp    InList,
        InsListTyp   InsList
)
        : In(InList), Ins(InsList)
{
    OUT(BthtrTemp1)    = 0.0F;
    OUT(BthtrTemp2)    = 0.0F;
    OUT(BthtrTemp3)    = 0.0F;
    OUT(BthtrTemp4)    = 0.0F;
    OUT(BthtrTemp5)    = 0.0F;
    OUT(BthtrTemp6)    = 0.0F;
    OUT(INS(Temp7Ref)) = 0.0F;
    OUT(INS(Temp8Ref)) = 0.0F;

    OUT(BthtrTempMaxA)    = 0.0F;
    OUT(BthtrTempMaxB)    = 0.0F;
    OUT(BthtrTempMaxC)    = 0.0F;
    OUT(BthtrTempMaxDb)   = 0.0F;
    OUT(BthtrTempMax)     = 0.0F;
    OUT(BthtrOtAlarmSum)  = false;
    OUT(BthtrOtFltDlySum) = false;

    BthtrVtempFil1 = 0.0F;
    BthtrVtempFil2 = 0.0F;
    BthtrVtempFil3 = 0.0F;
    BthtrVtempFil4 = 0.0F;
    BthtrVtempFil5 = 0.0F;
    BthtrVtempFil6 = 0.0F;
    BthtrVtempFil7 = 0.0F;
    BthtrVtempFil8 = 0.0F;
    BthtrTempMinA  = 0.0F;
    BthtrTempMinB  = 0.0F;
    BthtrTempMinC  = 0.0F;
    BthtrTempMinD  = 0.0F;

    BthtrUtFlgA = false;
    BthtrUtFlgB = false;
    BthtrUtFlgC = false;
    BthtrUtFlgD = false;

    BthtrOtFlgA = false;
    BthtrOtFlgB = false;
    BthtrOtFlgC = false;
    BthtrOtFlgD = false;

    BthtrImbFlgAB = false;
    BthtrImbFlgBC = false;
    BthtrImbFlgCA = false;
    BthtrImbFlgA  = false;
    BthtrImbFlgB  = false;
    BthtrImbFlgC  = false;
    BthtrImbFlgD  = false;

    BthtrFilStlTmr = 0.0F;

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
cBrgThrmstr::~cBrgThrmstr()
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
int  cBrgThrmstr::RunRules(void)
{
    if ( strncmp(INS(Uniquifier), "R", 1) == 0 )  //rotor side
    {
       SET_FV(BthtrImbOn,       11.0F);
       SET_FV(BthtrImbOff,       8.0F);
    }
    else //line side
    {
       SET_FV(BthtrImbOn,       13.0F);
       SET_FV(BthtrImbOff,       9.0F);
    }

    SET_FV(BthtrDbEnb, INS(DbEnb));

    return(SUCCESS);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set parameters common to all instances of thermistor processing class.
//
// EXECUTION LEVEL
//      Initialization and when required.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cBrgThrmstr::RunRules(
    //Inputs
    float    DeltaTime,    // delta time for execution loop (Sec)
    float    IoThrmstrW    // omega for filtering IO thermisters (Rad/Sec)
)
{
    int Status = SUCCESS;

    // note these final values and variables are static class members
    // i.e. they are not multiply instanced

    SET_FV(BthtrUtOn,        -5.0F);
    SET_FV(BthtrUtOff,        5.0F);
    SET_FV(BthtrUtThrs,      -5.0F);
    SET_FV(BthtrOtOn,        90.0F);
    SET_FV(BthtrOtOff,       85.0F);
    SET_FV(BthtrOtDlyThrs,  110.0F);
    SET_FV(BthtrOtThrs,     115.0F);
    SET_FV(BthtrPhsImbOn,     9.0F);
    SET_FV(BthtrPhsImbOff,    6.0F);

    BthtrVltGn    = IoThrmstrW * DeltaTime;
    BthtrFilStlTm = NoNANLim( 5.0F / IoThrmstrW,  1.0e6F, -1.0e6F);

    // Copy phase imbalance alarm settings to public variables
    BthtrPhsImblncOn  = PARM(BthtrPhsImbOn);
    BthtrPhsImblncOff = PARM(BthtrPhsImbOff);

    return (Status);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    The purpose of this function is to convert up to eight vco voltages,
//    coming from thermistors embedded in semiconductor modules, into
//    temperatures. These sensors are typicaly NTC (negative temperature
//    coeficient) thermistors mounted on same semiconductor substrate and
//    highly influenced by base-plate temeprature. Vco voltages are read and
//    processed by identical circuitry on control boards. Each of the eight
//    readings is done within a task3 period by task 1 and 3 functions that
//    converts vco frequency to voltage. Each voltage is then converted to a
//    temperature based on a look-up table. This function also checks for under
//    temperature, over temperature, and thermal imbalance conditions.
//    The temperature look-up is by voltage instead of resistance because the
//    voltage to temperature look-up is much more linear than the resistance
//    to temperature look-up. Using voltage allows use of a much more even
//    distribution of voltage inputs.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrmstr::Task3( void )
{
    // Do not process input voltags until feedbacks are valid
    if ( IN(TempFbkVld) == true )
    {
        //  Bridge Vco to Temperature Conversion (BTM_011)
        //------------------------------------------------

        // Filter the temperature circuit voltages

        BthtrVtempFil1 += BthtrVltGn * ( IN(ThrmstrVolt1) - BthtrVtempFil1 );
        BthtrVtempFil2 += BthtrVltGn * ( IN(ThrmstrVolt2) - BthtrVtempFil2 );
        BthtrVtempFil3 += BthtrVltGn * ( IN(ThrmstrVolt3) - BthtrVtempFil3 );
        BthtrVtempFil4 += BthtrVltGn * ( IN(ThrmstrVolt4) - BthtrVtempFil4 );
        BthtrVtempFil5 += BthtrVltGn * ( IN(ThrmstrVolt5) - BthtrVtempFil5 );
        BthtrVtempFil6 += BthtrVltGn * ( IN(ThrmstrVolt6) - BthtrVtempFil6 );
        BthtrVtempFil7 += BthtrVltGn * ( IN(ThrmstrVolt7) - BthtrVtempFil7 );
        BthtrVtempFil8 += BthtrVltGn * ( IN(ThrmstrVolt8) - BthtrVtempFil8 );

        // Give filtered voltages time to settle
        if ( BthtrFilStlTmr < BthtrFilStlTm )
        {
            BthtrFilStlTmr += INS(DelTm3);
        }
        else
        {
            // Clamp the inputs to table boundaries and translate clamped
            // inputs into temeprature

            OUT(BthtrTemp1)    = VoltToTemp( BthtrVtempFil1 );
            OUT(BthtrTemp2)    = VoltToTemp( BthtrVtempFil2 );
            OUT(BthtrTemp3)    = VoltToTemp( BthtrVtempFil3 );
            OUT(BthtrTemp4)    = VoltToTemp( BthtrVtempFil4 );
            OUT(BthtrTemp5)    = VoltToTemp( BthtrVtempFil5 );
            OUT(BthtrTemp6)    = VoltToTemp( BthtrVtempFil6 );
            OUT(INS(Temp7Ref)) = VoltToTemp( BthtrVtempFil7 );
            OUT(INS(Temp8Ref)) = VoltToTemp( BthtrVtempFil8 );

            // Determine maximum and minimum within single heatsink assembly

            if ( INS(IgbtParallel) == true)
            {
               if ( BthtrTemp1 < BthtrTemp4)       // Phase A
               {
                   BthtrTempMinA      = BthtrTemp1;
                   OUT(BthtrTempMaxA) = BthtrTemp4;
               }
               else
               {
                   BthtrTempMinA      = BthtrTemp4;
                   OUT(BthtrTempMaxA) = BthtrTemp1;
               }

               if ( BthtrTemp2 < BthtrTemp5)       // Phase B
               {
                   BthtrTempMinB      = BthtrTemp2;
                   OUT(BthtrTempMaxB) = BthtrTemp5;
               }
               else
               {
                   BthtrTempMinB      = BthtrTemp5;
                   OUT(BthtrTempMaxB) = BthtrTemp2;
               }

               if ( BthtrTemp3 < BthtrTemp6)       // Phase C
               {
                   BthtrTempMinC      = BthtrTemp3;
                   OUT(BthtrTempMaxC) = BthtrTemp6;
               }
               else
               {
                   BthtrTempMinC      = BthtrTemp6;
                   OUT(BthtrTempMaxC) = BthtrTemp3;
               }

               if ( INS(Temp7Ref) < INS(Temp8Ref))       // Dynamic Break
               {
                   BthtrTempMinD       = INS(Temp7Ref);
                   OUT(BthtrTempMaxDb) = INS(Temp8Ref);
               }
               else
               {
                   BthtrTempMinD       = INS(Temp8Ref);
                   OUT(BthtrTempMaxDb) = INS(Temp7Ref);
               }
            }
            else
            {
                BthtrTempMinA      = BthtrTemp1;
                OUT(BthtrTempMaxA) = BthtrTemp1;

                BthtrTempMinB      = BthtrTemp2;
                OUT(BthtrTempMaxB) = BthtrTemp2;

                BthtrTempMinC      = BthtrTemp3;
                OUT(BthtrTempMaxC) = BthtrTemp3;

                BthtrTempMinD      = INS(Temp7Ref);
                OUT(BthtrTempMaxDb)= INS(Temp7Ref);
            }


            //  Check for thermistor minimum under-temperature alarm conditions (BTM_020)
            //-------------------------------------------------------------------------

            // Phase A
            if ( (BthtrTempMinA < PARM(BthtrUtOn)) && IN(TempRdyPerm) )
            {
                BthtrUtFlgA = true;
            }
            else if ( BthtrTempMinA > PARM(BthtrUtOff) )
            {
                BthtrUtFlgA = false;
            }
            if ( BthtrUtFlgA == true )
            {
                PUSH_DIAG( BthtrAphsUtAlm );
            }

            // Phase B
            if ( (BthtrTempMinB < PARM(BthtrUtOn)) && IN(TempRdyPerm) )
            {
                BthtrUtFlgB = true;
            }
            else if ( BthtrTempMinB > PARM(BthtrUtOff) )
            {
                BthtrUtFlgB = false;
            }
            if ( BthtrUtFlgB == true )
            {
                PUSH_DIAG( BthtrBphsUtAlm );
            }

            // Phase C
            if ( (BthtrTempMinC < PARM(BthtrUtOn)) && IN(TempRdyPerm) )
            {
                BthtrUtFlgC = true;
            }
            else if ( BthtrTempMinC > PARM(BthtrUtOff) )
            {
                BthtrUtFlgC = false;
            }
            if ( BthtrUtFlgC == true )
            {
                PUSH_DIAG( BthtrCphsUtAlm );
            }

            // Dynamic Brake
            if ( PARM(BthtrDbEnb) )
            {
                if ( (BthtrTempMinD < PARM(BthtrUtOn)) && IN(TempRdyPerm) )
                {
                    BthtrUtFlgD = true;
                }
                else if ( BthtrTempMinD > PARM(BthtrUtOff) )
                {
                    BthtrUtFlgD = false;
                }
                if ( BthtrUtFlgD == true )
                {
                    PUSH_DIAG( BthtrDbUtAlm );
                }
            }
            else
            {
                BthtrUtFlgD = true;     // this allows general UT trip (below) to work
            }                           // even when DB diagnostics are disable

            // Trip if all four thermister flags are active

            if ( BthtrUtFlgA && BthtrUtFlgB && BthtrUtFlgC && BthtrUtFlgD )
            {
                PUSH_DIAG( BthtrUtTrp );
            }


            //  Check for individual thermistor under-temperature fault conditions (BTM_020)
            //-----------------------------------------------------------------------------

            if ( IN(Stopped) && IN(TempRdyPerm) )
            {

                if ( INS(IgbtParallel) == true)
                {

                   // Phase A
                   if ( BthtrTemp1 < PARM(BthtrUtThrs) )
                   {
                       PUSH_DIAG( BthtrAphTopUtTrp );
                   }

                   if ( BthtrTemp4 < PARM(BthtrUtThrs) )
                   {
                       PUSH_DIAG( BthtrAphBotUtTrp );
                   }

                   // Phase B
                   if ( BthtrTemp2 < PARM(BthtrUtThrs) )
                   {
                       PUSH_DIAG( BthtrBphTopUtTrp );
                   }

                   if ( BthtrTemp5 < PARM(BthtrUtThrs) )
                   {
                       PUSH_DIAG( BthtrBphBotUtTrp );
                   }

                   // Phase C
                   if ( BthtrTemp3 < PARM(BthtrUtThrs) )
                   {
                       PUSH_DIAG( BthtrCphTopUtTrp );
                   }

                   if ( BthtrTemp6 < PARM(BthtrUtThrs) )
                   {
                       PUSH_DIAG( BthtrCphBotUtTrp );
                   }

                   // Dynamic Brake
                   if ( PARM(BthtrDbEnb) )
                   {
                       if ( INS(Temp7Ref) < PARM(BthtrUtThrs) )
                       {
                           PUSH_DIAG( BthtrDbTopUtTrp );
                       }

                       if ( INS(Temp8Ref) < PARM(BthtrUtThrs) )
                       {
                           PUSH_DIAG( BthtrDbBotUtTrp );
                       }
                   }
                }
                else
                {
                   // Phase A
                   if ( BthtrTemp1 < PARM(BthtrUtThrs) )
                   {
                       PUSH_DIAG( BthtrAphTopUtTrp );
                   }


                   // Phase B
                   if ( BthtrTemp2 < PARM(BthtrUtThrs) )
                   {
                       PUSH_DIAG( BthtrBphTopUtTrp );
                   }


                   // Phase C
                   if ( BthtrTemp3 < PARM(BthtrUtThrs) )
                   {
                       PUSH_DIAG( BthtrCphTopUtTrp );
                   }


                   // Dynamic Brake
                   if ( PARM(BthtrDbEnb) )
                   {
                       if ( INS(Temp7Ref) < PARM(BthtrUtThrs) )
                       {
                           PUSH_DIAG( BthtrDbTopUtTrp );
                       }
                   }

                }
            }


            //  Check for thermistor maximum over-temperature conditions (BTM_030)
            //---------------------------------------------------------------------

            OUT(BthtrOtAlarmSum)  = false;
            OUT(BthtrOtFltDlySum) = false;

            // Phase A
            if ( BthtrTempMaxA > PARM(BthtrOtOn) )
            {
                BthtrOtFlgA       = true;
            }
            else if ( BthtrTempMaxA < PARM(BthtrOtOff) )
            {
                BthtrOtFlgA       = false;
            }

            if ( BthtrOtFlgA == true )
            {
                OUT(BthtrOtAlarmSum) = true;
                PUSH_DIAG( BthtrAphsOtAlm );
            }

            if ( BthtrTempMaxA > PARM(BthtrOtDlyThrs) )
            {
                OUT(BthtrOtFltDlySum) = true;
                PUSH_DIAG( BthtrAphsOtDlyTrp );
            }

            // Phase B
            if ( BthtrTempMaxB > PARM(BthtrOtOn) )
            {
                BthtrOtFlgB = true;
            }
            else if ( BthtrTempMaxB < PARM(BthtrOtOff) )
            {
                BthtrOtFlgB = false;
            }

            if ( BthtrOtFlgB == true )
            {
                OUT(BthtrOtAlarmSum) = true;
                PUSH_DIAG( BthtrBphsOtAlm );
            }

            if ( BthtrTempMaxB > PARM(BthtrOtDlyThrs) )
            {
                OUT(BthtrOtFltDlySum) = true;
                PUSH_DIAG( BthtrBphsOtDlyTrp );
            }

            // Phase C
            if ( BthtrTempMaxC > PARM(BthtrOtOn) )
            {
                BthtrOtFlgC = true;
            }
            else if ( BthtrTempMaxC < PARM(BthtrOtOff) )
            {
                BthtrOtFlgC = false;
            }

            if ( BthtrOtFlgC == true )
            {
                OUT(BthtrOtAlarmSum) = true;
                PUSH_DIAG( BthtrCphsOtAlm );
            }

            if ( BthtrTempMaxC > PARM(BthtrOtDlyThrs) )
            {
                OUT(BthtrOtFltDlySum) = true;
                PUSH_DIAG( BthtrCphsOtDlyTrp );
            }

            // Dynamic Brake
            if ( PARM(BthtrDbEnb) )
            {
                if ( BthtrTempMaxDb > PARM(BthtrOtOn) )
                {
                    BthtrOtFlgD = true;
                }
                else if ( BthtrTempMaxDb < PARM(BthtrOtOff) )
                {
                    BthtrOtFlgD = false;
                }

                if ( BthtrOtFlgD == true )
                {
                    OUT(BthtrOtAlarmSum) = true;
                    PUSH_DIAG( BthtrDbOtAlm );
                }

                if ( BthtrTempMaxDb > PARM(BthtrOtDlyThrs) )
                {
                    OUT(BthtrOtFltDlySum) = true;
                    PUSH_DIAG( BthtrDbOtDlyTrp );
                }
            }


            //  Check for thermistor imbalance alarm conditions (BTM_060)
            //-----------------------------------------------------------

            // Check phase to phase temp deltas and set/clear flags

           BthtrTmp12Del = ABS( BthtrTemp1 - BthtrTemp2 );
           BthtrTmp23Del = ABS( BthtrTemp2 - BthtrTemp3 );
           BthtrTmp13Del = ABS( BthtrTemp1 - BthtrTemp3 );

            if ( BthtrTmp12Del > PARM(BthtrImbOn) )    // Phase A to B
            {
                BthtrImbFlgAB = true;
            }
            else if ( BthtrTmp12Del < PARM(BthtrImbOff) )
            {
                BthtrImbFlgAB = false;
            }

            if ( BthtrTmp23Del > PARM(BthtrImbOn) )  // Phase B to C
            {
                BthtrImbFlgBC = true;
            }
            else if ( BthtrTmp23Del < PARM(BthtrImbOff) )
            {
                BthtrImbFlgBC = false;
            }

            if ( BthtrTmp13Del > PARM(BthtrImbOn) )  // Phase C to A
            {
                BthtrImbFlgCA = true;
            }
            else if ( BthtrTmp13Del < PARM(BthtrImbOff) )
            {
                BthtrImbFlgCA = false;
            }

            // calculate the max imblance temperature
            BthtrImbMax = MAX( MAX(BthtrTmp12Del, BthtrTmp23Del), BthtrTmp13Del );

            // Give phase temperature imbalance alarms based on which flags are set

            if ( BthtrImbFlgAB && BthtrImbFlgBC )
            {
                PUSH_DIAG(BthtrBphsImbAlm);
            }

            if ( BthtrImbFlgBC && BthtrImbFlgCA )
            {
                PUSH_DIAG(BthtrCphsImbAlm);
            }
            else

            if ( BthtrImbFlgCA && BthtrImbFlgAB)
            {
                PUSH_DIAG(BthtrAphsImbAlm);
            }


            //  Check for thermistor imbalance alarm within each phase (BTM_065)
            //------------------------------------------------------------------

            // Phase A
            if ( ( BthtrTempMaxA - BthtrTempMinA ) > PARM(BthtrPhsImbOn) )
            {
                BthtrImbFlgA = true;
            }
            else if ( ( BthtrTempMaxA - BthtrTempMinA ) < PARM(BthtrPhsImbOff) )
            {
                BthtrImbFlgA = false;
            }

            if ( BthtrImbFlgA == true )
            {
                PUSH_DIAG(BthtrAdelImbAlm);
            }

            // Phase B
            if ( ( BthtrTempMaxB - BthtrTempMinB ) > PARM(BthtrPhsImbOn) )
            {
                BthtrImbFlgB = true;
            }
            else if ( ( BthtrTempMaxB - BthtrTempMinB ) < PARM(BthtrPhsImbOff) )
            {
                BthtrImbFlgB = false;
            }

            if ( BthtrImbFlgB == true )
            {
                PUSH_DIAG(BthtrBdelImbAlm);
            }

            // Phase C
            if ( ( BthtrTempMaxC - BthtrTempMinC ) > PARM(BthtrPhsImbOn) )
            {
                BthtrImbFlgC = true;
            }
            else if ( ( BthtrTempMaxC - BthtrTempMinC ) < PARM(BthtrPhsImbOff) )
            {
                BthtrImbFlgC = false;
            }

            if ( BthtrImbFlgC == true )
            {
                PUSH_DIAG(BthtrCdelImbAlm);
            }

            // Dynamic Break
            if ( ( BthtrTempMaxDb - BthtrTempMinD ) > PARM(BthtrPhsImbOn) )
            {
                BthtrImbFlgD = true;
            }
            else if ( ( BthtrTempMaxDb - BthtrTempMinD ) < PARM(BthtrPhsImbOff) )
            {
                BthtrImbFlgD = false;
            }

            if ( BthtrImbFlgD == true )
            {
                PUSH_DIAG(BthtrDbDelImbAlm);
            }


            // Determine absolute bridge maximum (BTM_070)
            //-------------------------------------------

            if ( (BthtrTempMaxA > BthtrTempMaxB) && (BthtrTempMaxA > BthtrTempMaxC) )
            {
                OUT(BthtrTempMax) = BthtrTempMaxA;
            }
            else if ( (BthtrTempMaxB > BthtrTempMaxA) && (BthtrTempMaxB > BthtrTempMaxC) )
            {
                OUT(BthtrTempMax) = BthtrTempMaxB;
            }
            else
            {
                OUT(BthtrTempMax) = BthtrTempMaxC;
            }

            if ( OUT(BthtrTempMax) > PARM(BthtrOtThrs) )
            {
                PUSH_DIAG(BthtrOtTrp);
            }

        }       // Give filtered voltages time to settle

    }       // Do not process input voltags until feedbacks are valid

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    This function translates voltage into temperature using a look-up table.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      Translated temperature in degC
//
///////////////////////////////////////////////////////////////////////////////
float cBrgThrmstr::VoltToTemp(
    // Input/Output
    float& VoltIn           // voltage signal to convert (Volts)
)
{
    float  TempOut;          // Obtained temeprature (degC)
    float  TempPt1 = 0.0;    // Table point #1
    float  TempPt2 = 0.0;    // Table point #2
    float  TempPos;          // Position in table
    float  TempIdxFrc;       // Fractional part of position
    double TempIdxInt;       // Integer part of position
    int    TempIdx;          // Table index

    // Clamp the inputs to the table boundaries

    if ( VoltIn >  TEMP_VOLTS_TBL_MAX )
    {
        VoltIn = TEMP_VOLTS_TBL_MAX;
    }
    else if ( VoltIn < 0.0F )
    {
        VoltIn = 0.0;
    }

    //  Determine index for temperature table and interpolate between
    //  closest data-points

    TempPos    = VoltIn * ( TEMP_DATA_PTS * V_TEMP_FULL_RCP );
    TempIdxFrc = modf( TempPos, &TempIdxInt );
    TempIdx    = static_cast<int> (TempIdxInt);

    if ( TempIdx >= TEMP_DATA_PTS )
    {
        TempOut = TempTable[TEMP_DATA_PTS];
    }
    else
    {
        TempPt1 = TempTable[TempIdx];
        TempPt2 = TempTable[TempIdx+1];
        TempOut = TempPt1 + TempIdxFrc * ( TempPt2 - TempPt1 );
    }

    return TempOut;
}
