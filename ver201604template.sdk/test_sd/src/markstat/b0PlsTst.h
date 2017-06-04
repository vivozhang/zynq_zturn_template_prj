///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       2-level Bridge Pulse Test - Derived class.
//
// DESCRIPTION:
//      This file defines the 2-level derived pulse test class, which is
//      based on a virtual bridge test class.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0PlsTst.vsd, b0PlsTst.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef    B0_PLSTST_H
#define    B0_PLSTST_H

// Include Files
//--------------
// system
// framework
// product
#include "b0BrgTst.h"


// Constants
//----------

// Function Prototypes
//--------------------


// Types and Classes
//------------------


// Ins data-structure is defined by base class

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      2-level Pulse Test class definition - Derived and Direct-Base class
//
///////////////////////////////////////////////////////////////////////////////
class cPulseTest : public cBridgeTest
{

//-----------------------------------------------------------------------------
public:
    // Types & Enumerations
    //---------------------
    struct InListTyp
    {
        const unsigned& TstReq;         //  1 - Bridge test request from product code - Task2
        const unsigned& GatingEnb;      //  2 - Gating enable indication from bridge under test - Task1
        const unsigned& TripFlt;        //  3 - Trip fault active
    };

    // Methods
    //--------
    cPulseTest( InListTyp InList, InsListTyp InsList );
    ~cPulseTest(void);

    virtual void Task1(void);
    virtual void Task2(void);
    virtual int  RunRules(void) = 0;

    // Output Signals
    //---------------
    CREATE_PUBVAR(PlsTstBrgPwrEnb,   unsigned);  //  pulse test bridge power enable
    CREATE_PUBVAR(PlsTstStat,        unsigned);  //  pulse test task Stat to sequencer
    CREATE_PUBVAR(PlsTstAct,         unsigned);  //  pulse test task Active to sequencer
    CREATE_PUBVAR(PlsTstBrgOff,      unsigned);  //  all bridge devices configured to turn off

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cMarkStatDiag;

    // Types & Enumerations
    //---------------------
    enum PlsTstStMach1Typ
    {
        PLSTST_SM1_IDLE     = 0,
        PLSTST_ACTIVATE     = 1,
        PLSTST_CHECK_CONF   = 2,
        PLSTST_PARK         = 3,
        PLSTST_BEGIN_PLS1   = 4,
        PLSTST_CONT_PLS1    = 5,
        PLSTST_END_PLS1     = 6,
        PLSTST_BEGIN_PLS2   = 7,
        PLSTST_CONT_PLS2    = 8,
        PLSTST_END_PLS2     = 9,
        PLSTST_DEACTIVATE   = 10
    };

    enum PlsTstStMach2Typ
    {
        PLSTST_SM2_IDLE     = 0,
        PLSTST_ENGAGE       = 1,
        PLSTST_ENAB_BRG     = 2,
        PLSTST_RUN_SM1      = 3,
        PLSTST_DISAB_BRG    = 4,
        PLSTST_DISENGAGE    = 5,
        PLSTST_STOP_SM1     = 6
    };

    // External Inputs
    //----------------
    InListTyp    In;

    // Methods
    //--------
    void     StateMachineT1( void );
    void     StateMachineT2( void );

    // Diagnostics
    //------------

    // User Parameters
    //----------------

//-----------------------------------------------------------------------------
protected:
    // Types & Enumerations
    //---------------------
    struct  PlsTstCfgTyp
    {
        // inputs
        const int&       PlsTstCntPerQnt;  // 1 - Number of counts in a pulse quanta
        const int&       PlsTstQntPerFrm;  // 2 - Total number of pulse quanta per scheduling
        const int&       PlsTstCntPerFrm;  // 3 - Total number of clock pulses per scheduling
        const unsigned&  TransListLen;     // 4 - Length of the phase transition array
        const float&     PlsTstDelTm1Rcp;  // 5 - Delta time reciprocal for task 1 (1/Sec)
        const float&     PlsTstOnTime;     // 6 - Pulse time duration in Sec
        const float&     PlsTstOffTime;    // 7 - Park time duration in Sec
        // outputs
        int&       PlsTstOnCnt;      // 8 - Pulse scheduling counter in number-of-T1s
        int&       PlsTstOffCnt;     // 9 - Park scheduling counter in number-of-T1s
        int&       PlsTstOnTmr;      //10 - Pulse rising edge timer in counts
        int&       PlsTstOffTmr;     //11 - Pulse falling edge timer in counts
        unsigned&  ValidCfg;         //12 - Configuration validity
        unsigned&  PlsTstMakePls;    //13 - Make pulse indication
        float&     PlsTstOnMod;      //14 - Number of dt's in pulse period
        float&     PlsTstOffMod;     //15 - Number of dt's in park period
        float&     PlsTstOnFrac;     //16 - Fractional part of dt's in pulse period
        float&     PlsTstOffFrc;     //17 - Fractional part of dt's in park period
    };

    // Methods
    //--------
            void     PulseParkCfg( PlsTstCfgTyp Plist, unsigned PlsTst1stTimer );
    virtual unsigned TimerCfg(void) = 0;

    // Published Variables
    //--------------------

    // Pulse and park states used in gate commands
    CREATE_PUBVAR(PlsTstParkStA,        unsigned);  // park states for phases A, B, and C
    CREATE_PUBVAR(PlsTstParkStB,        unsigned);  //
    CREATE_PUBVAR(PlsTstParkStC,        unsigned);  //
    CREATE_PUBVAR(PlsTstPulseStA,       unsigned);  // pulse states for phases A, B, and C
    CREATE_PUBVAR(PlsTstPulseStB,       unsigned);  //
    CREATE_PUBVAR(PlsTstPulseStC,       unsigned);  //

    // Pulse synthetization
    CREATE_PUBVAR(PlsTstMakePls1,       unsigned);  // make 1st pulse indication
    CREATE_PUBVAR(PlsTstMakePls2,       unsigned);  // make 2nd pulse indication
    CREATE_PUBVAR(PlsTstPreOffCnt1,     int);       // counter for scheduling of 1st preamble park time
    CREATE_PUBVAR(PlsTstOnTmr1,         int);       // PWM timer for rising edge of 1st pulse (counts)
    CREATE_PUBVAR(PlsTstOnCnt1,         int);       // counter for scheduling 1st pulse in number-of-T1s
    CREATE_PUBVAR(PlsTstOffTmr1,        int);       // PWM timer for falling edge of 1st pulse (counts)
    CREATE_PUBVAR(PlsTstOffCnt1,        int);       // counter for scheduling 1st park time in number-of-T1s
    CREATE_PUBVAR(PlsTstPreOffCnt2,     int);       // counter for scheduling of 2nd preamble park time
    CREATE_PUBVAR(PlsTstOnTmr2,         int);       // PWM timer for rising edge of 2nd pulse (counts)
    CREATE_PUBVAR(PlsTstOnCnt2,         int);       // counter for scheduling 2nd pulse in number-of-T1s
    CREATE_PUBVAR(PlsTstOffTmr2,        int);       // PWM timer for falling edge of 2nd pulse (counts)
    CREATE_PUBVAR(PlsTstOffCnt2,        int);       // counter for scheduling 2nd park time in number-of-T1s

    // Supporting quantities
    CREATE_PUBVAR(PlsTstOnMod1,         float);     // number of dt's of 1st pulse
    CREATE_PUBVAR(PlsTstOnFrac1,        float);     // fractional dt of 1st pulse
    CREATE_PUBVAR(PlsTstOffMod1,        float);     // number of dt's of 1st park
    CREATE_PUBVAR(PlsTstOffFrc1,        float);     // fractional dt of 1st park
    CREATE_PUBVAR(PlsTstOnMod2,         float);     // number of dt's of 2nd pulse
    CREATE_PUBVAR(PlsTstOnFrac2,        float);     // fractional dt of 2nd pulse
    CREATE_PUBVAR(PlsTstOffMod2,        float);     // number of dt's of 2nd park
    CREATE_PUBVAR(PlsTstOffFrc2,        float);     // fractional dt of 2nd park
    CREATE_PUBVAR(PlsTstPreOffFrc2,     float);     // fractional dt of 2nd preamble park

    // Pulse test task state machine control variables
    CREATE_PUBVAR(PlsTstStatus,         unsigned);  // task1 state-machine stat(us)
    CREATE_PUBVAR(PlsTstActive,         unsigned);  // task1 state-machine active
    CREATE_PUBVAR(PlsTstStMch1State,    int);       // task1 state-machine state variable
    CREATE_PUBVAR(PlsTstInReq,          unsigned);  // task2 copy of external pulse test request input
    CREATE_PUBVAR(PlsTstStMch2State,    int);       // task2 state-machine state variable
    CREATE_PUBVAR(PlsTstStatusT2,       unsigned);  // status passed from task1 to task2
    CREATE_PUBVAR(PlsTstActiveT2,       unsigned);  // active indication passed from task1 to task2
    CREATE_PUBVAR(PlsTstParkCnt,        int);       // counter for scheduling park time
    CREATE_PUBVAR(PlsTstCheckCnt,       int);       // check configuration state counter
    CREATE_PUBVAR(PlsTstDeactCnt,       int);       // deactivate state counter
    CREATE_PUBVAR(PlsTstReq,            unsigned);  // task2 internal pulse test request, later passed to task1
    CREATE_PUBVAR(PlsTstReqT1,          unsigned);  // task1 state-machine request
    CREATE_PUBVAR(PlsTstReqT1Z1,        unsigned);  // state-machine task1 request delayed one task
    CREATE_PUBVAR(PlsTstReqT2Z1,        unsigned);  // state-machine task2 request delayed one task
    CREATE_PUBVAR(T1Seq,                int);  // state-machine task2 request delayed one task
    CREATE_PUBVAR(T2Seq,                int);  // state-machine task2 request delayed one task

    // Un-published Variables
    //-----------------------
    PlsTstStMach1Typ            PlsTstSeq1St;
    PlsTstStMach2Typ            PlsTstSeq2St;

    // Data Passing
    //-------------

};

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor 2-level Pulse Test class definition - Derived class
//
///////////////////////////////////////////////////////////////////////////////
class cRotorPulseTest : public cPulseTest
{

//-----------------------------------------------------------------------------
public:
    // Methods
    //--------
    cRotorPulseTest( InListTyp InList, InsListTyp InsList );
    ~cRotorPulseTest(void);
    virtual int RunRules(void);

    // Output Signals
    //---------------

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cMarkStatDiag;
    friend class cDataPasser;

    // Enumerations
    //-------------

    // External Inputs
    //----------------

    // Methods
    //--------

    // Diagnostics
    //------------
    CREATE_DIAG(PlsTstBadCfgAlm);              // Bad pulse test configuration for line side

    // User Parameters
    //----------------
    CREATE_PARM(PlsTstParkPhs,      unsigned); // phase(s) to park
    CREATE_PARM(PlsTstParkSt,       unsigned); // park state
    CREATE_PARM(PlsTstPulsePhs,     unsigned); // phase(s) to pulse
    CREATE_PARM(PlsTstPulseSt,      unsigned); // pulse state
    CREATE_PARM(PlsTstOnTm1,        float);    // duration of 1st pulse (sec)
    CREATE_PARM(PlsTstOffTm1,       float);    // duration of inter-pulse park time (sec)
    CREATE_PARM(PlsTstOnTm2,        float);    // duration of 2nd pulse (sec)
    CREATE_PARM(PlsTstOffTm2,       float);    // duration of post-pulse park time (sec)

    // Published Variables
    //--------------------
    CREATE_PUBVAR(PlsTstOnTime1,    float);    // time duration of 1st pulse (sec)
    CREATE_PUBVAR(PlsTstOffTime1,   float);    // 1st pulse park time (sec)
    CREATE_PUBVAR(PlsTstOnTime2,    float);    // time duration of 2nd pulse (sec)
    CREATE_PUBVAR(PlsTstOffTime2,   float);    // 2nd pulse park time (sec)

    // Data Passing
    //-------------
    DATA_PASS(PlsTstReq,    PlsTstReqT1,    T2_T1, UNFILTERED);  // pass internal task2 pulse test request
    DATA_PASS(PlsTstStatus, PlsTstStatusT2, T1_T2, UNFILTERED);  // pass task1 state machine status
    DATA_PASS(PlsTstActive, PlsTstActiveT2, T1_T2, UNFILTERED);  // pass task1 state machine active indication

//-----------------------------------------------------------------------------
protected:
    // Methods
    //-------
    virtual unsigned TimerCfg(void);

};


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line 2-level Pulse Test class definition - Derived class
//
///////////////////////////////////////////////////////////////////////////////
class cLinePulseTest : public cPulseTest
{

//-----------------------------------------------------------------------------
public:
    // Methods
    //--------
    cLinePulseTest( InListTyp InList, InsListTyp InsList );
    ~cLinePulseTest(void);
    virtual int RunRules(void);

    // Output Signals
    //---------------

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cMarkStatDiag;
    friend class cDataPasser;

    // Enumerations
    //-------------

    // External Inputs
    //----------------

    // Methods
    //--------

    // Diagnostics
    //------------
    CREATE_DIAG(PlsTstBadCfgAlm);              // Bad pulse test configuration for line side

    // User Parameters
    //----------------
    CREATE_PARM(PlsTstParkPhs,      unsigned); // phase(s) to park
    CREATE_PARM(PlsTstParkSt,       unsigned); // park state
    CREATE_PARM(PlsTstPulsePhs,     unsigned); // phase(s) to pulse
    CREATE_PARM(PlsTstPulseSt,      unsigned); // pulse state
    CREATE_PARM(PlsTstOnTm1,        float);    // duration of 1st pulse (sec)
    CREATE_PARM(PlsTstOffTm1,       float);    // duration of inter-pulse park time (sec)
    CREATE_PARM(PlsTstOnTm2,        float);    // duration of 2nd pulse (sec)
    CREATE_PARM(PlsTstOffTm2,       float);    // duration of post-pulse park time (sec)

    // Published Variables
    //--------------------
    CREATE_PUBVAR(PlsTstOnTime1,    float);    // time duration of 1st pulse (sec)
    CREATE_PUBVAR(PlsTstOffTime1,   float);    // 1st pulse park time (sec)
    CREATE_PUBVAR(PlsTstOnTime2,    float);    // time duration of 2nd pulse (sec)
    CREATE_PUBVAR(PlsTstOffTime2,   float);    // 2nd pulse park time (sec)

    // Data Passing
    //-------------
    DATA_PASS(PlsTstReq,    PlsTstReqT1,    T2_T1, UNFILTERED);  // pass internal task2 pulse test request
    DATA_PASS(PlsTstStatus, PlsTstStatusT2, T1_T2, UNFILTERED);  // pass task1 state machine status
    DATA_PASS(PlsTstActive, PlsTstActiveT2, T1_T2, UNFILTERED);  // pass task1 state machine active indication

//-----------------------------------------------------------------------------
protected:
    // Methods
    //-------
    virtual unsigned TimerCfg(void);

};





// Inline Function Definitions
//----------------------------

inline int cRotorPulseTest::RunRules(void)
{
    return (SUCCESS);
}

inline int cLinePulseTest::RunRules(void)
{
    return (SUCCESS);
}

#endif  // B0_PLSTST_H
