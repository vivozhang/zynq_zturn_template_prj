///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       2-level Pulse Test derived class
//
// DESCRIPTION:
//      This file contains the methods for the 3-level pulse test class.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0PlsTst.vsd, b0PlsTst.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
#include <math.h>
// framework
// product
#include "b0PlsTst.h"


// Ver_PlsTst = 0xMMmmpp  --  Class Version Code: MM=Major,mm=Minor,pp=Patch
//--------------------------------------------------------------------------
CREATE_PUBVAR(Ver_PlsTst, WORD) = 0x010100;
// -First 2-level bridge test class release, adapted from 3-level.


// Constants
//----------


// Externals
//----------


// Types and Classes
//------------------


// Internal Function Prototypes
//--------------------


// Variables
//----------

// Member Functions
//-----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Derived and Direct-Base class constructor to initialize resources
//
// EXECUTION LEVEL
//      Derived class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cPulseTest::cPulseTest (
        InListTyp    InList,
        InsListTyp   InsList
)
        : cBridgeTest(InsList), In(InList)  // Pass the Ins list to base class constructor
{
    PlsTstParkStA         = false;
    PlsTstParkStB         = false;
    PlsTstParkStC         = false;
    PlsTstPulseStA        = false;
    PlsTstPulseStB        = false;
    PlsTstPulseStC        = false;

    PlsTstMakePls1        = false;
    PlsTstMakePls2        = false;
    PlsTstPreOffCnt1      = 0;
    PlsTstOnTmr1          = 0;
    PlsTstOnCnt1          = 0;
    PlsTstOffTmr1         = 0;
    PlsTstOffCnt1         = 0;
    PlsTstPreOffCnt2      = 0;
    PlsTstOnTmr2          = 0;
    PlsTstOnCnt2          = 0;
    PlsTstOffTmr2         = 0;
    PlsTstOffCnt2         = 0;

    PlsTstOnMod1          = 0;
    PlsTstOnFrac1         = 0;
    PlsTstOffMod1         = 0;
    PlsTstOffFrc1         = 0;
    PlsTstOnMod2          = 0;
    PlsTstOnFrac2         = 0;
    PlsTstOffMod2         = 0;
    PlsTstOffFrc2         = 0;
    PlsTstPreOffFrc2      = 0;

    PlsTstStatus          = false;
    PlsTstActive          = false;
    PlsTstStMch1State     = 0;
    PlsTstInReq           = false;
    PlsTstStMch2State     = 0;
    PlsTstStatusT2        = false;
    PlsTstActiveT2        = false;
    PlsTstParkCnt         = 0;
    PlsTstCheckCnt        = 0;
    PlsTstDeactCnt        = 0;
    PlsTstReq             = false;
    PlsTstReqT1           = false;
    PlsTstReqT1Z1         = true;
    PlsTstReqT2Z1         = true;

    PlsTstSeq1St          = PLSTST_SM1_IDLE;
    PlsTstSeq2St          = PLSTST_SM2_IDLE;

    OUT(PlsTstBrgPwrEnb)  = false;
    OUT(PlsTstStat)       = false;
    OUT(PlsTstAct)        = false;
    OUT(PlsTstBrgOff)     = true;

    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Derived and Direct-Base class destructor to free used resources.
//
// EXECUTION LEVEL
//      Derived instance destruction.
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
cPulseTest::~cPulseTest ()
{
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Support configuration and validation of Pulse and Park timers,
//      respecting maximum number of phase transitions per frame.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cPulseTest::PulseParkCfg( PlsTstCfgTyp Plist, unsigned PlsTst1stTimer )  {

    double   IntPart;
    Plist.PlsTstMakePls = true;
    if (( Plist.PlsTstOnTime < 0.0 ) || ( Plist.PlsTstOffTime < 0.0 ))
    {
        Plist.ValidCfg      = false;
        Plist.PlsTstMakePls = false;
    }
    else if (( Plist.PlsTstOnTime == 0.0 ) && ( Plist.PlsTstOffTime == 0.0 ))
    {
        Plist.PlsTstMakePls = false;
    }
    else if ( PlsTst1stTimer == true )  // Starting pulse/park timer configuration
    {
        // Following settings compatible with 3 phase transitions per frame.
        // One transition at zero time, and two other later transitions.
        Plist.PlsTstOffMod = Plist.PlsTstOffTime * Plist.PlsTstDelTm1Rcp;
        Plist.PlsTstOffFrc = modf( Plist.PlsTstOffMod, &IntPart );
        Plist.PlsTstOffCnt = (int)IntPart;

        Plist.PlsTstOnMod  = Plist.PlsTstOffFrc + Plist.PlsTstOnTime * Plist.PlsTstDelTm1Rcp;
        Plist.PlsTstOnFrac = modf( Plist.PlsTstOnMod, &IntPart );

        // Some bridges cannot do more than two phase transitions per frame,
        // For that case, the following code ensure that 1st pulse always
        // starts at a frame boundary. Therefore, only an initial transition
        // and a later transition are used to schedule the pulse.
        if ((IntPart == 0)  && (Plist.PlsTstOffFrc > 0) && (Plist.TransListLen == 2))
        {
            Plist.PlsTstOffFrc = 1 - modf(Plist.PlsTstOnTime*Plist.PlsTstDelTm1Rcp, &IntPart);
            Plist.PlsTstOnFrac = 1;   // previous code was PlsTstOnFrac1 = 0
            IntPart = 0;              // previous code was IntPart = 1
        }

        Plist.PlsTstOnCnt = (int)IntPart;

        Plist.PlsTstOnTmr  = Plist.PlsTstCntPerFrm - (int)( Plist.PlsTstOnFrac * Plist.PlsTstQntPerFrm ) * Plist.PlsTstCntPerQnt;
        Plist.PlsTstOffTmr = Plist.PlsTstCntPerFrm - (int)( Plist.PlsTstOffFrc * Plist.PlsTstQntPerFrm ) * Plist.PlsTstCntPerQnt;
    }
    else    // Ending pulse/park timer configuration
    {
        // Following settings compatible with 2 or 3 phase transitions per frame.
        // Because 2nd pulse always starts at a frame boundary, which only requires
        // two transitions.
        Plist.PlsTstOnMod  = Plist.PlsTstOnTime * Plist.PlsTstDelTm1Rcp;
        Plist.PlsTstOnFrac = modf( Plist.PlsTstOnMod, &IntPart );
        Plist.PlsTstOnCnt  = (int)IntPart;
        Plist.PlsTstOffTmr = (int)( Plist.PlsTstOnFrac * Plist.PlsTstQntPerFrm ) * Plist.PlsTstCntPerQnt;

        Plist.PlsTstOffMod = Plist.PlsTstOnFrac + Plist.PlsTstOffTime * Plist.PlsTstDelTm1Rcp;
        Plist.PlsTstOffFrc = modf( Plist.PlsTstOffMod, &IntPart );
        // following counter makes the second park time finish at frame boundary,
        // loosing the very last fraction of park time
        Plist.PlsTstOffCnt = (int)IntPart;
    }

    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Pulse test task1 state machine.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cPulseTest::StateMachineT1( void )
{
    switch ( PlsTstSeq1St )
    {
    //----------------  Remain inert until request is received  ----------------
        case PLSTST_SM1_IDLE:
            if ( (PlsTstReqT1 == true) && (IN(GatingEnb) == true) &&
                 (PlsTstReqT1Z1 == false) && (IN(TripFlt) == false) )
            {
                PlsTstSeq1St = PLSTST_ACTIVATE;
            }
            else if ( PlsTstReqT1 == false )
            {
                PlsTstStatus = false;
            }

            PlsTstActive     = false;
            PlsTstReqT1Z1 = PlsTstReqT1 && IN(GatingEnb);

            break;

    //----------------  Activate pulse testing  --------------------------------
        case PLSTST_ACTIVATE:
            BridgeOff( PlsTstBrgOff );
            if ( (IN(TripFlt) == true) || (PlsTstReqT1 == false) )
            {
                PlsTstSeq1St = PLSTST_SM1_IDLE;
            }
            else
            {
                PlsTstActive      = true;
                PlsTstCheckCnt = 1;
                PlsTstSeq1St   = PLSTST_CHECK_CONF;
            }
            break;

    //----------------  Check for valid configuration  -------------------------
        case PLSTST_CHECK_CONF:
            BridgeOff( PlsTstBrgOff );
            if ( (IN(TripFlt) == true) || (PlsTstReqT1 == false) )
            {
                PlsTstSeq1St = PLSTST_SM1_IDLE;
            }
            else if ( PlsTstCheckCnt == 0 )
            {
                PlsTstParkCnt = 1 + PlsTstPreOffCnt1;
                PlsTstSeq1St  = PLSTST_PARK;
            }
            else
            {
                if ( TimerCfg() )
                {
                    --PlsTstCheckCnt;
                }
                else
                {
                    PlsTstDeactCnt = 1;
                    PlsTstSeq1St = PLSTST_DEACTIVATE;
                }
            }
            break;

    //----------------  Park bridge phases -------------------------------------
        case PLSTST_PARK:
            if ( (IN(TripFlt) == true) || (PlsTstReqT1 == false) )
            {
                BridgeOff( PlsTstBrgOff );
                PlsTstDeactCnt = 1;
                PlsTstSeq1St   = PLSTST_DEACTIVATE;
            }
            else
            {   // park state must never be entered with a zero 'count'
                BridgePark( PlsTstBrgOff );
                --PlsTstParkCnt;
                if ( PlsTstParkCnt == 0 )
                {
                    if ( PlsTstMakePls1 == true )
                    {
                        PlsTstSeq1St = PLSTST_BEGIN_PLS1;
                    }
                    else if ( PlsTstMakePls2 == true )
                    {
                        PlsTstSeq1St = PLSTST_BEGIN_PLS2;
                    }
                    else
                    {
                        PlsTstDeactCnt = 2;
                        PlsTstSeq1St = PLSTST_DEACTIVATE;
                    }
                }
            }
            break;

    //----------------  Begin (and possibly end) first pulse  ------------------
        case PLSTST_BEGIN_PLS1:
            if ( (IN(TripFlt) == true) || (PlsTstReqT1 == false) )
            {
                BridgeOff( PlsTstBrgOff );
                PlsTstDeactCnt = 1;
                PlsTstSeq1St   = PLSTST_DEACTIVATE;
            }
            else
            {
                if ( PlsTstOnCnt1 > 0 )
                {
                    BridgePulseFrac1( PlsTstOnTmr1, PlsTstBrgOff );
                    --PlsTstOnCnt1;
                    if ( PlsTstOnCnt1 > 0 )
                    {
                        PlsTstSeq1St = PLSTST_CONT_PLS1;
                    }
                    else
                    {
                        PlsTstSeq1St = PLSTST_END_PLS1;
                    }
                }
                else
                {
                    BridgePulseFrac2( PlsTstOnTmr1, PlsTstOffTmr1, PlsTstBrgOff );
                    PlsTstMakePls1 = false;
                    if ( PlsTstOffCnt1 > 0 )
                    {
                        PlsTstParkCnt  = PlsTstOffCnt1;
                        PlsTstSeq1St   = PLSTST_PARK;
                    }
                    else if ( PlsTstMakePls2 == true )
                    {
                        PlsTstSeq1St = PLSTST_BEGIN_PLS2;
                    }
                    else
                    {
                        PlsTstDeactCnt = 2;
                        PlsTstSeq1St   = PLSTST_DEACTIVATE;
                    }
                }
            }
            break;

    //----------------  Continue first pulse for full frame  -------------------
        case PLSTST_CONT_PLS1:
            if ( (IN(TripFlt) == true) || (PlsTstReqT1 == false) )
            {
                BridgeOff( PlsTstBrgOff );
                PlsTstDeactCnt = 1;
                PlsTstSeq1St   = PLSTST_DEACTIVATE;
            }
            else
            {
                // Pulse for entire frame starting at the boundary
                BridgePulseFull( PlsTstBrgOff );
                --PlsTstOnCnt1;
                if ( PlsTstOnCnt1 == 0 )
                {
                    PlsTstSeq1St = PLSTST_END_PLS1;
                }
            }
            break;

    //----------------  End first pulse  ---------------------------------------
        case PLSTST_END_PLS1:
            if ( (IN(TripFlt) == true) || (PlsTstReqT1 == false) )
            {
                BridgeOff( PlsTstBrgOff );
                PlsTstDeactCnt = 1;
                PlsTstSeq1St   = PLSTST_DEACTIVATE;
            }
            else
            {
                BridgePulseFrac3( PlsTstOffTmr1, PlsTstBrgOff );
                PlsTstMakePls1 = false;
                if ( (PlsTstOffCnt1 + PlsTstPreOffCnt2) > 0 )
                {
                    PlsTstParkCnt = PlsTstOffCnt1 + PlsTstPreOffCnt2;
                    PlsTstSeq1St  = PLSTST_PARK;
                }
                else if ( PlsTstMakePls2 == true )
                {
                    PlsTstSeq1St  = PLSTST_BEGIN_PLS2;
                }
                else
                {
                    PlsTstDeactCnt = 2;
                    PlsTstSeq1St   = PLSTST_DEACTIVATE;
                }
            }
            break;

    //----------------  Begin (and possibly end) second pulse  ------------------
        case PLSTST_BEGIN_PLS2:
            if ( (IN(TripFlt) == true) || (PlsTstReqT1 == false) )
            {
                BridgeOff( PlsTstBrgOff );
                PlsTstDeactCnt = 1;
                PlsTstSeq1St   = PLSTST_DEACTIVATE;
            }
            else
            {
                if ( PlsTstOnCnt2 > 0 )
                {
                    BridgePulseFrac1( PlsTstOnTmr2, PlsTstBrgOff );
                    --PlsTstOnCnt2;
                    if ( PlsTstOnCnt2 > 0 )
                    {
                        PlsTstSeq1St = PLSTST_CONT_PLS2;
                    }
                    else
                    {
                        PlsTstSeq1St = PLSTST_END_PLS2;
                    }
                }
                else
                {
                    BridgePulseFrac2( PlsTstOnTmr2, PlsTstOffTmr2, PlsTstBrgOff );
                    PlsTstMakePls2 = false;
                    if ( PlsTstOffCnt2 > 0 )
                    {
                        PlsTstParkCnt  = PlsTstOffCnt2;
                        PlsTstSeq1St   = PLSTST_PARK;
                    }
                    else
                    {
                        PlsTstDeactCnt = 2;
                        PlsTstSeq1St   = PLSTST_DEACTIVATE;
                    }
                }
            }
            break;

    //----------------  Continue second pulse for full frame  ------------------
        case PLSTST_CONT_PLS2:
            if ( (IN(TripFlt) == true) || (PlsTstReqT1 == false) )
            {
                BridgeOff( PlsTstBrgOff );
                PlsTstDeactCnt = 1;
                PlsTstSeq1St   = PLSTST_DEACTIVATE;
            }
            else
            {
                // Pulse for entire frame starting at the boundary
                BridgePulseFull( PlsTstBrgOff );
                --PlsTstOnCnt2;
                if ( PlsTstOnCnt2 == 0 )
                {
                    PlsTstSeq1St = PLSTST_END_PLS2;
                }
            }
            break;

    //----------------  End second pulse  --------------------------------------
        case PLSTST_END_PLS2:
            if ( (IN(TripFlt) == true) || (PlsTstReqT1 == false) )
            {
                BridgeOff( PlsTstBrgOff );
                PlsTstDeactCnt = 1;
                PlsTstSeq1St   = PLSTST_DEACTIVATE;
            }
            else
            {
                // Pulse for a fraction of a frame starting at the boundary
                // Very last park fraction (PlsTstOffFrc2) is always ignored
                BridgePulseFrac3( PlsTstOffTmr2, PlsTstBrgOff );
                PlsTstMakePls2 = false;
                if ( PlsTstOffCnt2 > 0 )
                {
                    PlsTstParkCnt  = PlsTstOffCnt2;
                    PlsTstSeq1St   = PLSTST_PARK;
                }
                else
                {
                    PlsTstDeactCnt = 2;
                    PlsTstSeq1St = PLSTST_DEACTIVATE;
                }
            }
            break;

    //----------------  Deactivate pulse testing  ------------------------------
        case PLSTST_DEACTIVATE:
            BridgeOff( PlsTstBrgOff );
            --PlsTstDeactCnt;
            if ( PlsTstDeactCnt == 0 )
            {
                if ( (IN(TripFlt) == false) && (PlsTstReqT1 == true) )
                {
                    PlsTstStatus = true;
                }
                PlsTstSeq1St = PLSTST_SM1_IDLE;
            }
            break;

    //----------------  Unrecognized state  ------------------------------------
        default:
            PlsTstSeq1St  = PLSTST_SM1_IDLE;
            break;

    }   // End of switch(PlsTstSeq1St)

    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Pulse test task2 state machine.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cPulseTest::StateMachineT2( void )
{
    // Get pulse test request from external product code
    PlsTstInReq = IN(TstReq);

    switch ( PlsTstSeq2St )
    {
     //----------------  Remain idle until request is received  ----------------
        case PLSTST_SM2_IDLE:
            if ( (PlsTstInReq == true) && (PlsTstReqT2Z1 == false) )
            {
                PlsTstSeq2St = PLSTST_ENGAGE;
            }
            else if ( PlsTstInReq == false )
            {
                OUT(PlsTstStat) = false;
                OUT(PlsTstAct)  = false;
            }

            PlsTstReqT2Z1 = PlsTstInReq;
            break;

    //----------------  Engage pulse-test with upper entities  -----------------
        case PLSTST_ENGAGE:
            if ( PlsTstInReq == false )
            {
                PlsTstSeq2St  = PLSTST_SM2_IDLE;
            }
            else
            {
                OUT(PlsTstAct) = true;
                PlsTstSeq2St   = PLSTST_ENAB_BRG;
            }
            break;

    //----------------  Enable bridge electronics  -----------------------------
        case PLSTST_ENAB_BRG:
            if ( PlsTstInReq == false )
            {
                PlsTstSeq2St   = PLSTST_SM2_IDLE;
            }
            else
            {
                OUT(PlsTstBrgPwrEnb) = true;
                PlsTstReq = true;          // Start running Task 1 state machine
                PlsTstSeq2St  = PLSTST_RUN_SM1;
            }
            break;

    //----------------  Run Task 1 state machine  ------------------------------
        case PLSTST_RUN_SM1:
            if ( PlsTstInReq == false )
            {
                PlsTstReq      = false;
                PlsTstSeq2St   = PLSTST_STOP_SM1;
            }
            else
            {
                PlsTstReq = true;
                if ( PlsTstStatusT2 == true )
                {
                    PlsTstSeq2St = PLSTST_DISAB_BRG;
                }
            }
            break;

    //----------------  Disable bridge electronics  ----------------------------
        case PLSTST_DISAB_BRG:
            if ( PlsTstStatusT2 == true )
            {
                PlsTstSeq2St  = PLSTST_DISENGAGE;
            }
            else
            {
                PlsTstSeq2St  = PLSTST_SM2_IDLE;
            }
            OUT(PlsTstBrgPwrEnb) = false;
            break;

    //----------------  Disengage pulse test from other entities  --------------
        case PLSTST_DISENGAGE:
            if ( (PlsTstInReq == false) && (PlsTstStatusT2 == false) &&
                 (PlsTstActiveT2 == false) )
            {
                PlsTstSeq2St = PLSTST_SM2_IDLE;
            }

            OUT(PlsTstStat) = true;
            PlsTstReq       = false;
            break;

    //----------------  Stop Task 1 state machine  -----------------------------
        case PLSTST_STOP_SM1:
            PlsTstReq = false;
            if ( PlsTstActiveT2 == false )
            {
                PlsTstSeq2St = PLSTST_DISAB_BRG;
            }
            break;

    //----------------  Unrecognized state  ------------------------------------
        default:
            PlsTstSeq2St  = PLSTST_SM2_IDLE;
            break;

    }   // End of switch(PlsTstSeq2St)

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Calls the member functions.
//
//
// EXECUTION LEVEL
//      Task1 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cPulseTest::Task1(void)
{
    StateMachineT1();

    // Publish to Toolbox
    PlsTstStMch1State  = static_cast<int>      (PlsTstSeq1St);
    PlsTstParkStA      = static_cast<unsigned> (PhsTstCtrl.ParkStA);
    PlsTstParkStB      = static_cast<unsigned> (PhsTstCtrl.ParkStB);
    PlsTstParkStC      = static_cast<unsigned> (PhsTstCtrl.ParkStC);
    PlsTstPulseStA     = static_cast<unsigned> (PhsTstCtrl.PulseStA);
    PlsTstPulseStB     = static_cast<unsigned> (PhsTstCtrl.PulseStB);
    PlsTstPulseStC     = static_cast<unsigned> (PhsTstCtrl.PulseStC);

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Calls the member functions.
//
//
// EXECUTION LEVEL
//      Task2 execution
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cPulseTest::Task2(void)
{
    StateMachineT2();

    // Publish to Toolbox
    PlsTstStMch2State = static_cast<int> (PlsTstSeq2St);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Derived class constructor to initialize resources
//
// EXECUTION LEVEL
//      Derived class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cRotorPulseTest::cRotorPulseTest (
    InListTyp    InList,
    InsListTyp   InsList
)
    : cPulseTest(InList, InsList)   // Pass lists to base class constructor
{
    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Derived class destructor to free used resources.
//
// EXECUTION LEVEL
//      Derived instance destruction.
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
cRotorPulseTest::~cRotorPulseTest ()
{
    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Configures and validate on and off Rotor pulse timers. Maximum
//      number of phase transition per frame supported, are 2 or 3.
//      A macro value determines such setting.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      ValidCfg (true/false)
//
///////////////////////////////////////////////////////////////////////////////
unsigned cRotorPulseTest::TimerCfg( void )  {

    unsigned ValidCfg;
    float    PlsTstDelTm1Rcp;
    int      PlsTstCntPerQnt;
    int      PlsTstQntPerFrm;
    int      PlsTstCntPerFrm;

    PlsTstCfgTyp FirstTimerCfg =
    {
        PlsTstCntPerQnt,      // 1 - Number of counts in a pulse quanta
        PlsTstQntPerFrm,      // 2 - Total number of pulse quanta per scheduling
        PlsTstCntPerFrm,      // 3 - Total number of clock pulses per scheduling
        PhsTransList.Len,     // 4 - Length of the phase transition array
        PlsTstDelTm1Rcp,      // 5 - Delta time reciprocal for task 1 (1/Sec)
        PlsTstOnTime1,        // 6 - Pulse time duration in Sec
        PlsTstOffTime1,       // 7 - Park time duration in Sec
        PlsTstOnCnt1,         // 8 - Pulse scheduling counter in number-of-T1s
        PlsTstOffCnt1,        // 9 - Park scheduling counter in number-of-T1s
        PlsTstOnTmr1,         //10 - Pulse rising edge timer in counts
        PlsTstOffTmr1,        //11 - Pulse falling edge timer in counts
        ValidCfg,             //12 - Configuration validity
        PlsTstMakePls1,       //13 - Make pulse indication
        PlsTstOnMod1,         //14 - Number of dt's in pulse period
        PlsTstOffMod1,        //15 - Number of dt's in park period
        PlsTstOnFrac1,        //16 - Fractional part of dt's in pulse period
        PlsTstOffFrc1,        //17 - Fractional part of dt's in park period
    };
    PlsTstCfgTyp SecndTimerCfg =
    {
        PlsTstCntPerQnt,      // 1 - Number of counts in a pulse quanta
        PlsTstQntPerFrm,      // 2 - Total number of pulse quanta per scheduling
        PlsTstCntPerFrm,      // 3 - Total number of clock pulses per scheduling
        PhsTransList.Len,     // 4 - Length of the phase transition array
        PlsTstDelTm1Rcp,      // 5 - Delta time reciprocal for task 1 (1/Sec)
        PlsTstOnTime2,        // 6 - Pulse time duration in Sec
        PlsTstOffTime2,       // 7 - Park time duration in Sec
        PlsTstOnCnt2,         // 8 - Pulse scheduling counter in number-of-T1s
        PlsTstOffCnt2,        // 9 - Park scheduling counter in number-of-T1s
        PlsTstOnTmr2,         //10 - Pulse rising edge timer in counts
        PlsTstOffTmr2,        //11 - Pulse falling edge timer in counts
        ValidCfg,             //12 - Configuration validity
        PlsTstMakePls2,       //13 - Make pulse indication
        PlsTstOnMod2,         //14 - Number of dt's in pulse period
        PlsTstOffMod2,        //15 - Number of dt's in park period
        PlsTstOnFrac2,        //16 - Fractional part of dt's in pulse period
        PlsTstOffFrc2,        //17 - Fractional part of dt's in park period
    };

    // Check validity of park and pulse states
    ValidCfg = BridgeCfg(PARM(PlsTstParkPhs), static_cast<PhsStateTyp> (PARM(PlsTstParkSt)),
                         PARM(PlsTstPulsePhs), static_cast<PhsStateTyp> (PARM(PlsTstPulseSt)));
    PlsTstDelTm1Rcp = INS(DelTm1Rcp);
    PlsTstCntPerQnt = static_cast<int> (INS(PwmQuant));
    PlsTstQntPerFrm = static_cast<int> (INS(UFull));
    PlsTstCntPerFrm = static_cast<int> (INS(PwmFull));

    PlsTstOnTime1   = PARM(PlsTstOnTm1);
    PlsTstOffTime1  = PARM(PlsTstOffTm1);
    PlsTstOnTime2   = PARM(PlsTstOnTm2);
    PlsTstOffTime2  = PARM(PlsTstOffTm2);

    //--------------------------------------------------------------------------
    // First pulse/park timer-set configuration
    PulseParkCfg(FirstTimerCfg, true);

    //--------------------------------------------------------------------------
    // Second pulse/park timer-set configuration
    PulseParkCfg(SecndTimerCfg, false);

    return ValidCfg;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Derived class constructor to initialize resources
//
// EXECUTION LEVEL
//      Derived class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cLinePulseTest::cLinePulseTest (
    InListTyp    InList,
    InsListTyp   InsList
)
    : cPulseTest(InList, InsList)   // Pass lists to base class constructor
{
    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Derived class destructor to free used resources.
//
// EXECUTION LEVEL
//      Derived instance destruction.
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
cLinePulseTest::~cLinePulseTest ()
{
    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Configures and validate on and off Line pulse timers. Maximum
//      number of phase transition per frame supported, are 2 or 3.
//      A macro value determines such setting.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      ValidCfg (true/false)
//
///////////////////////////////////////////////////////////////////////////////
unsigned cLinePulseTest::TimerCfg( void )  {

    unsigned ValidCfg;
    float    PlsTstDelTm1Rcp;
    int      PlsTstCntPerQnt;
    int      PlsTstQntPerFrm;
    int      PlsTstCntPerFrm;

    PlsTstCfgTyp FirstTimerCfg =
    {
        PlsTstCntPerQnt,      // 1 - Number of counts in a pulse quanta
        PlsTstQntPerFrm,      // 2 - Total number of pulse quanta per scheduling
        PlsTstCntPerFrm,      // 3 - Total number of clock pulses per scheduling
        PhsTransList.Len,     // 4 - Length of the phase transition array
        PlsTstDelTm1Rcp,      // 5 - Delta time reciprocal for task 1 (1/Sec)
        PlsTstOnTime1,        // 6 - Pulse time duration in Sec
        PlsTstOffTime1,       // 7 - Park time duration in Sec
        PlsTstOnCnt1,         // 8 - Pulse scheduling counter in number-of-T1s
        PlsTstOffCnt1,        // 9 - Park scheduling counter in number-of-T1s
        PlsTstOnTmr1,         //10 - Pulse rising edge timer in counts
        PlsTstOffTmr1,        //11 - Pulse falling edge timer in counts
        ValidCfg,             //12 - Configuration validity
        PlsTstMakePls1,       //13 - Make pulse indication
        PlsTstOnMod1,         //14 - Number of dt's in pulse period
        PlsTstOffMod1,        //15 - Number of dt's in park period
        PlsTstOnFrac1,        //16 - Fractional part of dt's in pulse period
        PlsTstOffFrc1,        //17 - Fractional part of dt's in park period
    };
    PlsTstCfgTyp SecndTimerCfg =
    {
        PlsTstCntPerQnt,      // 1 - Number of counts in a pulse quanta
        PlsTstQntPerFrm,      // 2 - Total number of pulse quanta per scheduling
        PlsTstCntPerFrm,      // 3 - Total number of clock pulses per scheduling
        PhsTransList.Len,     // 4 - Length of the phase transition array
        PlsTstDelTm1Rcp,      // 5 - Delta time reciprocal for task 1 (1/Sec)
        PlsTstOnTime2,        // 6 - Pulse time duration in Sec
        PlsTstOffTime2,       // 7 - Park time duration in Sec
        PlsTstOnCnt2,         // 8 - Pulse scheduling counter in number-of-T1s
        PlsTstOffCnt2,        // 9 - Park scheduling counter in number-of-T1s
        PlsTstOnTmr2,         //10 - Pulse rising edge timer in counts
        PlsTstOffTmr2,        //11 - Pulse falling edge timer in counts
        ValidCfg,             //12 - Configuration validity
        PlsTstMakePls2,       //13 - Make pulse indication
        PlsTstOnMod2,         //14 - Number of dt's in pulse period
        PlsTstOffMod2,        //15 - Number of dt's in park period
        PlsTstOnFrac2,        //16 - Fractional part of dt's in pulse period
        PlsTstOffFrc2,        //17 - Fractional part of dt's in park period
    };

    // Check validity of park and pulse states
    ValidCfg = BridgeCfg(PARM(PlsTstParkPhs), static_cast<PhsStateTyp> (PARM(PlsTstParkSt)),
                         PARM(PlsTstPulsePhs), static_cast<PhsStateTyp> (PARM(PlsTstPulseSt)));
    PlsTstDelTm1Rcp = INS(DelTm1Rcp);
    PlsTstCntPerQnt = static_cast<int> (INS(PwmQuant));
    PlsTstQntPerFrm = static_cast<int> (INS(UFull));
    PlsTstCntPerFrm = static_cast<int> (INS(PwmFull));

    PlsTstOnTime1   = PARM(PlsTstOnTm1);
    PlsTstOffTime1  = PARM(PlsTstOffTm1);
    PlsTstOnTime2   = PARM(PlsTstOnTm2);
    PlsTstOffTime2  = PARM(PlsTstOffTm2);

    //--------------------------------------------------------------------------
    // First pulse/park timer-set configuration
    PulseParkCfg(FirstTimerCfg, true);

    //--------------------------------------------------------------------------
    // Second pulse/park timer-set configuration
    PulseParkCfg(SecndTimerCfg, false);

#if 0
    PlsTstMakePls1 = true;
    if (( PlsTstOnTime1 < 0.0 ) || ( PlsTstOffTime1 < 0.0 ))
    {
        ValidCfg = false;
        PlsTstMakePls1 = false;
    }
    else if (( PlsTstOnTime1 == 0.0 ) && ( PlsTstOffTime1 == 0.0 ))
    {
        PlsTstMakePls1 = false;
    }
    else
    {
        // Following settings compatible with 3 phase transitions per frame.
        // One transition at zero time, and two other later transitions.
        PlsTstOffMod1 = PlsTstOffTime1 * PlsTstDelTm1Rcp;
        PlsTstOffFrc1 = modf( PlsTstOffMod1, &IntPart );
        PlsTstOffCnt1 = (int)IntPart;

        PlsTstOnMod1  = PlsTstOffFrc1 + PlsTstOnTime1 * PlsTstDelTm1Rcp;
        PlsTstOnFrac1 = modf( PlsTstOnMod1, &IntPart );

        // Some bridges cannot do more than two phase transitions per frame,
        // For that case, the following code ensure that 1st pulse always
        // starts at a frame boundary. Therefore, only an initial transition
        // and a later transition are used to schedule the pulse.
        if ((IntPart == 0)  && (PlsTstOffFrc1 > 0) && (PhsTransList.Len == 2))
        {
            PlsTstOffFrc1 = 1 - modf(PlsTstOnTime1*PlsTstDelTm1Rcp, &IntPart);
            PlsTstOnFrac1 = 1;  // previous code was PlsTstOnFrac1 = 0
            IntPart = 0;        // previous code was IntPart = 1
        }

        PlsTstOnCnt1 = (int)IntPart;

        PlsTstOnTmr1  = PlsTstCntPerFrm - (int)( PlsTstOnFrac1 * PlsTstQntPerFrm ) * PlsTstCntPerQnt;
        PlsTstOffTmr1 = PlsTstCntPerFrm - (int)( PlsTstOffFrc1 * PlsTstQntPerFrm ) * PlsTstCntPerQnt;
    }

    //--------------------------------------------------------------------------
    // Second pulse/park timer-set configuration
    PlsTstMakePls2 = true;
    if (( PlsTstOnTime2 < 0.0 ) || ( PlsTstOffTime2 < 0.0 ))
    {
        ValidCfg = false;
        PlsTstMakePls2 = false;
    }
    else if (( PlsTstOnTime2 == 0.0 ) && ( PlsTstOffTime2 == 0.0 ))
    {
        PlsTstMakePls2 = false;
    }
    else
    {
        // Following settings compatible with 2 or 3 phase transitions per frame.
        // Because 2nd pulse always starts at a frame boundary, which only requires
        // two transitions.
        PlsTstOnMod2  = PlsTstOnTime2 * PlsTstDelTm1Rcp;
        PlsTstOnFrac2 = modf( PlsTstOnMod2, &IntPart );
        PlsTstOnCnt2  = (int)IntPart;
        PlsTstOffTmr2 = (int)( PlsTstOnFrac2 * PlsTstQntPerFrm ) * PlsTstCntPerQnt;

        PlsTstOffMod2 = PlsTstOnFrac2 + PlsTstOffTime2 * PlsTstDelTm1Rcp;
        PlsTstOffFrc2 = modf( PlsTstOffMod2, &IntPart );
        // following counter makes the second park time finish at frame boundary,
        // loosing the very last fraction of park time
        PlsTstOffCnt2 = (int)IntPart;
    }

#endif

    return ValidCfg;
}




