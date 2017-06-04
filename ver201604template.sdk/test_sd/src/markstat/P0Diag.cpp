///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Product Definition File
//
// DESCRIPTION:
//
// COPYRIGHT:   Copyright (c) 2014
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      p0Product.xlsx, p0Product.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
#include "x0BridgeInterface.h"
#include "x0CaptureBuf.h"
#include "x0Vector.h"
// product
#include "Product.h"
#include "b0BridgeRebf.h"
#include "p0EsaBuf.h"
#include "p0Thrmstr.h"

#define PULSE_IDLE   0
#define PULSE_MAXOUT 1
#define PULSE_MINOUT 2
#define PULSE_WAIT   3

#define TRIANG_UP    0
#define TRIANG_DN    1

#define SINUSOIDAL   1
#define TRIANGULAR   2

extern float         R_TorTrqMag;
extern float         DelTm2;

// Parameters
//-----------

// Test related
CREATE_PARM(GpMathDwgEnb,       unsigned);         // Enable for user diagram for gp math
CREATE_PARM(GpMathEnb,          unsigned);         // Enable for general purpose sums, mult's
CREATE_PARM(GpSum1Scl1,           float);          // General purpose Sum 1 input 1 scale
CREATE_PARM(GpSum1Scl2,           float);          // General purpose Sum 1 input 2 scale
CREATE_PARM(GpSum1Scl3,           float);          // General purpose Sum 1 input 3 scale
CREATE_PARM(GpSum2Scl1,           float);          // General purpose Sum 2 input 1 scale
CREATE_PARM(GpSum2Scl2,           float);          // General purpose Sum 2 input 2 scale
CREATE_PARM(GpSum2Scl3,           float);          // General purpose Sum 2 input 3 scale
CREATE_PARM(GpSum1In1Adr,         float*);         // General purpose Sum 1, input1 address
CREATE_PARM(GpSum1In2Adr,         float*);         // General purpose Sum 1, input2 address
CREATE_PARM(GpSum1In3Adr,         float*);         // General purpose Sum 1, input3 address
CREATE_PARM(GpSum2In1Adr,         float*);         // General purpose Sum 2, input1 address
CREATE_PARM(GpSum2In2Adr,         float*);         // General purpose Sum 2, input2 address
CREATE_PARM(GpSum2In3Adr,         float*);         // General purpose Sum 2, input3 address
CREATE_PARM(GpMult1In1Adr,        float*);         // General purpose Multiply 1, input1 address
CREATE_PARM(GpMult1In2Adr,        float*);         // General purpose Multiply 1, input2 address
CREATE_PARM(GpMult2In1Adr,        float*);         // General purpose Multiply 2, input1 address
CREATE_PARM(GpMult2In2Adr,        float*);         // General purpose Multiply 2, input2 address

//IONet Diagnostics
CREATE_PARM(IoNDiag1Adr,           float*);         // IoNet Diagnostic address 1
CREATE_PARM(IoNDiag2Adr,           float*);         // IoNet Diagnostic address 2
CREATE_PARM(IoNDiag3Adr,           float*);         // IoNet Diagnostic address 3
CREATE_PARM(IoNDiag4Adr,           float*);         // IoNet Diagnostic address 4
CREATE_PARM(IoNDiag5Adr,           float*);         // IoNet Diagnostic address 5
CREATE_PARM(IoNDiag6Adr,           float*);         // IoNet Diagnostic address 6
CREATE_PARM(IoNDiag7Adr,           float*);         // IoNet Diagnostic address 7
CREATE_PARM(IoNDiag8Adr,           float*);         // IoNet Diagnostic address 8
CREATE_PARM(IoNDiag9Adr,           float*);         // IoNet Diagnostic address 9
CREATE_PARM(IoNDiag10Adr,          float*);         // IoNet Diagnostic address 10
CREATE_PARM(IoNDiag11Adr,          float*);         // IoNet Diagnostic address 11
CREATE_PARM(IoNDiag12Adr,          float*);         // IoNet Diagnostic address 12
CREATE_PARM(IoNDiag13Adr,          unsigned*);      // IoNet Diagnostic address 13

// Start Pulse generator parameters
CREATE_PARM( PulseGenEnb,    unsigned   );     // Pulse genrator enable
CREATE_PARM( PulseMagReq,    float      );     // Pulse generator magnitude
CREATE_PARM( PulseFreqReq,   float      );     // Pulse generator frequency requested
CREATE_PARM( PulseCycReq,    unsigned   );     // Pulse generator cycles requested
CREATE_PARM( PulseGenType,   unsigned   );     // Pulse generator type.
CREATE_PARM( PulseGenPu,     unsigned   );     // Pulse torque boolean
CREATE_PARM( PulseMagLim,    float      );     // Pulose generator magnitude limit
CREATE_PARM( PulseFreqLim,   float      );     // Pulse generator frequency limit
CREATE_PARM( PulseMagMax,    float      );     // Pulse generator magnitude at max output
CREATE_PARM( PulseMagMin,    float      );     // Pulse generator magnitude at min output
CREATE_PARM( PulseT2sMax,    unsigned   );     // Pulse generator task 2's at max output
CREATE_PARM( PulseCycLim,    unsigned   );     // Pulse generator limit on number of cycles per burst
CREATE_PARM( PulseCycles,    unsigned   );     // Pulse generator cycles
CREATE_PARM( PulseKillThr,   float      );     // Pulse kill threhold based on  R_TorSpdMag
CREATE_PARM( PulseBrstCyc,   unsigned   );     // Pulse generator burst cycles
CREATE_PARM( PulseRptTm,     float      );     // Pulse generator time between
CREATE_PARM( PulseWaitTm,    float      );     // Pulse generator wait time before re-entry to idle state
// End Pulse generator parameters

CREATE_PARM( PeakDetEnb,     unsigned   );     // Peak detector enable
CREATE_PARM( PeakDetAdr,     float*     );     // Peak detector address

CREATE_PARM(L_EETest1,      float);     //
CREATE_PARM(L_EETest2,      float);     //
CREATE_PARM(L_EETest3,      float);     //
CREATE_PARM(L_EETest4,      float);     //


// Published Variables
//--------------------

//------------------------------------------------------------------------------
// Test related
CREATE_PUBVAR(LabTestRef1,        float);          // lab test reference
CREATE_PUBVAR(GpSum1Out,          float);          // General purpose Sum 1 output
CREATE_PUBVAR(GpSum2Out,          float);          // General purpose Sum 2 output
CREATE_PUBVAR(GpMult1Out,         float);          // General purpose Multiply 1 output
CREATE_PUBVAR(GpMult2Out,         float);          // General purpose Multiply 2 output

//IONet Diagnostics
CREATE_PUBVAR(CCUDiag1,           float);          // IoNet Diagnostic 1
CREATE_PUBVAR(CCUDiag2,           float);          // IoNet Diagnostic 2
CREATE_PUBVAR(CCUDiag3,           float);          // IoNet Diagnostic 3
CREATE_PUBVAR(CCUDiag4,           float);          // IoNet Diagnostic 4
CREATE_PUBVAR(CCUDiag5,           float);          // IoNet Diagnostic 5
CREATE_PUBVAR(CCUDiag6,           float);          // IoNet Diagnostic 6
CREATE_PUBVAR(CCUDiag7,           float);          // IoNet Diagnostic 7
CREATE_PUBVAR(CCUDiag8,           float);          // IoNet Diagnostic 8
CREATE_PUBVAR(CCUDiag9,           float);          // IoNet Diagnostic 9
CREATE_PUBVAR(CCUDiag10,          float);          // IoNet Diagnostic 10
CREATE_PUBVAR(CCUDiag11,          float);          // IoNet Diagnostic 11
CREATE_PUBVAR(CCUDiag12,          float);          // IoNet Diagnostic 12
CREATE_PUBVAR(CCUDiag13,          unsigned);       // IoNet Diagnostic 13
CREATE_PUBVAR(PLCDiag1,           float);          // IoNet PLC Diag 1
CREATE_PUBVAR(PLCDiag2,           unsigned);       // IoNet PLC Diag 2

//Start Pulse test variables
CREATE_PUBVAR(PulseGenOut,     float);   // Pulse generator output
CREATE_PUBVAR(PulsePassCtr, unsigned);      // Pulse generator pass counter
CREATE_PUBVAR(PulseBrstCtr, unsigned);      // Pulse generator burst counter
CREATE_PUBVAR(PulseCycCtr,  unsigned);      // Pulse generator cycle counter
CREATE_PUBVAR(PulseState,   unsigned);      // Pulse generator state
CREATE_PUBVAR(PulseGenInit, unsigned);      // Pulse generator initialization...set in rule file
CREATE_PUBVAR(PulseRptTmr,     float);   // Pulse generator repeat timer
CREATE_PUBVAR(PulseWaitTmr,    float);   // Pulse generator wait timer
CREATE_PUBVAR(PulseCycTm,      float);   // Pulse cycle time for accurate freq
CREATE_PUBVAR(PulseCycNoRl,    float);   // Pulse cycle number real type 0.5, 1.0, 1.5, etc.
CREATE_PUBVAR(PulseBrstTmr,    float);   // Total elasped time it this pulse burst
CREATE_PUBVAR(PulseHalfDt,     float);   // One half dt
CREATE_PUBVAR(PulseTypSt,   unsigned);      // Pulse test type state.
CREATE_PUBVAR(TriangTypSt,  unsigned);      // Triangle type state.
CREATE_PUBVAR(PulseSineW,      float);          // Pulse frequency w for wind wave output.
CREATE_PUBVAR(PulseGenAng,     float);          // Pulse gen  angle
CREATE_PUBVAR(PulseDelMgDt,    float);   // Pulse delta mag/dt for triangle wave

CREATE_PUBVAR(PlsGenInitZ1,     unsigned);
CREATE_PUBVAR(PulseGenSclNtv,   float);
//End Pulse test variables

CREATE_PUBVAR(PeakDetMax,       float);   // Peak max detector
CREATE_PUBVAR(PeakDetMin,       float);   // Peak min detector

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Product task 1 test support
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void TestSupportT1(void)
{

    if (PARM(GpMathEnb) )
    {

        // General purpose summing blocks
        float GpSum1In1   = gVariable.ReadAsFloat(PARM(GpSum1In1Adr));
        float GpSum1In2   = gVariable.ReadAsFloat(PARM(GpSum1In2Adr));
        float GpSum1In3   = gVariable.ReadAsFloat(PARM(GpSum1In3Adr));

        GpSum1Out  =  PARM(GpSum1Scl1)*GpSum1In1 +
                      PARM(GpSum1Scl2)*GpSum1In2 +
                      PARM(GpSum1Scl3)*GpSum1In3;

        float GpSum2In1   = gVariable.ReadAsFloat(PARM(GpSum2In1Adr));
        float GpSum2In2   = gVariable.ReadAsFloat(PARM(GpSum2In2Adr));
        float GpSum2In3   = gVariable.ReadAsFloat(PARM(GpSum2In3Adr));

        GpSum2Out  =  PARM(GpSum2Scl1)*GpSum2In1 +
                      PARM(GpSum2Scl2)*GpSum2In2 +
                      PARM(GpSum2Scl3)*GpSum2In3;


        // General purpose multiply blocks
        float GpMult1In1   = gVariable.ReadAsFloat(PARM(GpMult1In1Adr));
        float GpMult1In2   = gVariable.ReadAsFloat(PARM(GpMult1In2Adr));

        GpMult1Out  =  GpMult1In1 * GpMult1In2;

        float GpMult2In1   = gVariable.ReadAsFloat(PARM(GpMult2In1Adr));
        float GpMult2In2   = gVariable.ReadAsFloat(PARM(GpMult2In2Adr));

        GpMult2Out  =  GpMult2In1 * GpMult2In2;

    }



    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Product task 1 test support
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void IoNetDiag(void)
{

    CCUDiag1   = gVariable.ReadAsFloat(PARM(IoNDiag1Adr));
    CCUDiag2   = gVariable.ReadAsFloat(PARM(IoNDiag2Adr));
    CCUDiag3   = gVariable.ReadAsFloat(PARM(IoNDiag3Adr));
    CCUDiag4   = gVariable.ReadAsFloat(PARM(IoNDiag4Adr));
    CCUDiag5   = gVariable.ReadAsFloat(PARM(IoNDiag5Adr));
    CCUDiag6   = gVariable.ReadAsFloat(PARM(IoNDiag6Adr));
    CCUDiag7   = gVariable.ReadAsFloat(PARM(IoNDiag7Adr));
    CCUDiag8   = gVariable.ReadAsFloat(PARM(IoNDiag8Adr));
    CCUDiag9   = gVariable.ReadAsFloat(PARM(IoNDiag9Adr));
    CCUDiag10  = gVariable.ReadAsFloat(PARM(IoNDiag10Adr));
    CCUDiag11  = gVariable.ReadAsFloat(PARM(IoNDiag11Adr));
    CCUDiag12  = gVariable.ReadAsFloat(PARM(IoNDiag12Adr));
    CCUDiag13  = gVariable.ReadAsBool(PARM(IoNDiag13Adr));

    return;
}

/******************************************************************************
* Function:         PulseGeneratorT2
*
* Execution Level:  Task 2
* Execution Time:   Not measured
*
*
* Abstract:
*      This function generates bursts of pulses.  It was developed as a
*  pulse generator to test torsional instabilty for 1.5MW turbines. The user
*  presentation of this function only allows maniuplation of four parameters,
*  as follows:
*               P.PulseFreqReq - requested pulse frequency
*               P.PulseMagReq  - requested pulse magnitude
*               P.PulseCycReq  - requested pulse cycles
*               P.PulseGenEnb  - pulse enable (begins cycle)
*
*      There are several associated final values that are driven by the above
*  four inputs and some others that allow further manipulation of this funcion.
*  From a user perspective, this function will only ouput up to three square wave
*  cycles with half cycles that are equal in time and magnitude.
*
*      Engineering maniupulation allows much more flexibility by applying over-rides.
*  The number of cycles can go beyond 3, and multiple number of 'bursts' can be obtained.
*  Half cycles, in this case, do not need to be symmetric and neither do the times.
*
*
*      Execution time efficiency was a top priority in thedesign of this function, and
*  resulted in a state machine versus nested "if then else" statements.
*
*  Associated user diagram:  Diag_WaveformGenPeakDet
*
******************************************************************************/
void PulseGeneratorT2(void)
{
    float GenTrialOut;

    if ( PARM(PulseGenEnb))
    {
        if ( R_TorTrqMag > PARM(PulseKillThr))
        {
            PulseState   = PULSE_IDLE;
            PulseGenInit = false; //Prevent re-start
        }

        switch (PulseState)
        {

            /*==========================================================================
              Stay here waiting waiting for PulseGenInit
            ==========================================================================*/
            case PULSE_IDLE: //(0)
               PulsePassCtr = 0;
               PulseBrstCtr = 0;
               PulseCycCtr  = 0;
               PulseWaitTmr = 0.0;
               PulseRptTmr  = 0.0;
               PulseGenOut  = 0.0;
               PulseCycNoRl = 0.0;
               PulseGenAng  = 0.0;
               TriangTypSt  = TRIANG_UP;

               if ( PulseGenInit)
               {
                  PulseState = PULSE_MAXOUT;
                  PulseCycNoRl = 0.5; //for starting first half cycle in float mode
               }


            break;

            /*==========================================================================
              Output maximum value
            ==========================================================================*/
            case PULSE_MAXOUT:  //(1)

              PulseRptTmr  += DelTm2;

              switch (PulseTypSt)
              {

                 case SINUSOIDAL:  //(2)

                    PulseGenAng  += PulseSineW * DelTm2;
                    PulseGenAng  = cVectorf::Modulo2Pi(PulseGenAng);
                    PulseGenOut  = PARM(PulseMagMax) * sin(PulseGenAng);
                    PulsePassCtr += 1;
                    if ( PulseGenOut < 0.0)
                    {
                       PulseState   = PULSE_MINOUT;
                       PulsePassCtr = 0;
                    }

                 break;



                 case TRIANGULAR:  //(3)

                   PulsePassCtr += 1;
                   switch (TriangTypSt)
                   {
                      case TRIANG_UP:  //(0)

                        GenTrialOut =  PulseGenOut + PulseDelMgDt;
                        if ( GenTrialOut > (PARM(PulseMagMax) - (0.5*PulseDelMgDt) ) )
                        {
                           PulseGenOut  = PARM(PulseMagMax);
                           TriangTypSt  = TRIANG_DN;
                        }
                        else
                        {
                           PulseGenOut  = GenTrialOut;
                        }

                      break;


                      case TRIANG_DN:  //(1)

                        PulseGenOut =  PulseGenOut - PulseDelMgDt;
                        if ( PulseGenOut < 0.0 )
                        {
                           PulseState   = PULSE_MINOUT;
                           PulsePassCtr = 0;
                        }


                      break;


                      default:

                      break;


                   }


                 break;


                 default:
                 break;


              }


           break;

            /*==========================================================================
              Output minimum value
            ==========================================================================*/
            case PULSE_MINOUT:  //(2)

              PulseRptTmr  += DelTm2;

              switch (PulseTypSt)
              {
                 case SINUSOIDAL:  //(1)

                    PulseGenAng  += PulseSineW * DelTm2;
                    PulseGenAng  = cVectorf::Modulo2Pi(PulseGenAng);
                    GenTrialOut  = PARM(PulseMagMax) * sin(PulseGenAng);
                    PulsePassCtr += 1;

                    if ( GenTrialOut > 0.0)
                    {
                       PulseCycCtr += 1;
                       if ( PulseCycCtr >= PARM(PulseCycles))
                       {
                          PulseBrstCtr += 1;
                          PulseCycCtr  = 0;
                          PulseGenOut  = 0.0; //do not allow last one to go above 0
                          PulseState   = PULSE_WAIT;
                       }
                       else
                       {
                         PulseGenOut  = GenTrialOut;
                         PulseState   = PULSE_MAXOUT;
                       }
                    }
                    else
                    {
                       PulseGenOut  = GenTrialOut;
                    }


                 break;


                 case TRIANGULAR:  //(3)

                   PulsePassCtr += 1;
                   switch (TriangTypSt)
                   {
                      case TRIANG_UP:  //(0)

                        GenTrialOut =  PulseGenOut + PulseDelMgDt;
                        if ( GenTrialOut > (  - 0.5*PulseDelMgDt ) )
                        {
                          PulseCycCtr += 1;
                          if ( PulseCycCtr >= PARM(PulseCycles))
                          {
                             PulseBrstCtr += 1;
                             PulseCycCtr  = 0;
                             PulseGenOut  = 0.0; //do not allow last one to go above 0
                             PulseState   = PULSE_WAIT;
                          }
                          else
                          {
                            PulseGenOut  = GenTrialOut;
                            PulseState   = PULSE_MAXOUT;
                          }
                        }
                        else
                        {
                          PulseGenOut  = GenTrialOut;
                        }

                      break;


                      case TRIANG_DN:  //(1)

                        PulseGenOut =  PulseGenOut - PulseDelMgDt;
                        if ( PulseGenOut < (PARM(PulseMagMin) - (0.5*PulseDelMgDt) ) )
                        {
                           TriangTypSt  = TRIANG_UP;
                           PulsePassCtr = 0;
                        }


                      break;


                      default:

                      break;


                   }

                 break;



                 default:
                 break;

              }

           break;

            /*==========================================================================
              Wait for next burst, or delay to go to idle state
            ==========================================================================*/
            case PULSE_WAIT:  //(3)

               PulseGenOut = 0.0;
               PulseRptTmr  += DelTm2;

               if ( (PARM(PulseBrstCyc) > 1) && (PulseBrstCtr < PARM(PulseBrstCyc)))
               {
                  if ( PulseRptTmr >= PARM(PulseRptTm))
                  {
                     PulseState   = PULSE_MAXOUT;
                     PulseRptTmr  = 0.0;
                  }
               }
               else
               {
                  PulseWaitTmr += DelTm2;
                  if (PulseWaitTmr >= PARM(PulseWaitTm))
                 {
                    PulseState   = PULSE_IDLE;
                    PulseGenInit = false;
                 }
               }

           break;


            /*==========================================================================
              Default state -- should never go here
            ==========================================================================*/
            default:

            break;

        }


    }
    else
    {
        PulseGenOut  = 0.0;
        TriangTypSt  = TRIANG_UP;
        PulseState   = PULSE_IDLE;
    }
}
void PeakDetectorT1(void)
{
    float   PeakDetIn;

    if ( PARM(PeakDetEnb))
    {
       //Read in variable
       PeakDetIn = gVariable.ReadAsFloat(PARM(PeakDetAdr));

       //Check for maximum
       if ( PeakDetIn > PeakDetMax )
       {
           PeakDetMax = PeakDetIn;
       }
       //Check for minimum
       if ( PeakDetIn < PeakDetMin)
       {
           PeakDetMin = PeakDetIn;
       }
    }
    else
    {
        PeakDetMax = 0.0;
        PeakDetMin = 10e10;
    }

    return;
}

