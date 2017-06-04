///////////////////////////////////////////////////////////////////////////////
//
// TITLE        <class title>
//
// DESCRIPTION
//      This file contains the definition for the class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#ifndef _X0XOPS_H_
#define _X0XOPS_H_
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// Sys API
#include "SysMemClient.h"
// core
#include "ParamString.h"
// framework
#include "x0FrameWork.h"
// product


// Constants
//----------


// Function Prototypes
//--------------------


// Types and Classes
//------------------
struct MarkStatWdogRegistration
{
    cDString        Name;               // Name to display on error

    float           Period;             // normal execution rate (sec)
    float           AvgMin;             // minimum execution rate (sec)
    float           AvgMax;             // maximum execution rate (sec)
    long            AvgCount;           // Number of tickles used to calculate the average
    float           InitTimer;          // maximum time to wait for tickling to start (sec)
};


///////////////////////////////////////////////////////////////////////////////
//
// used in conjunction with cMarkStatWdog
//
// Each task signs up for watching through the cMarkStatWdog class and runs the Tickle
//    method as proof of life.
// The cMarkStatAlphaWdog class signs up for a sys watchdog and the method CheckRegDogs
//    must be called on a regular basis to Tickle the sys watchdog as proof of life.
// CheckRegDogs also calculates the period of all registered tasks and compares with
//    the stated minimum and maximum, issueing a trip fault when they are not meet.
//
///////////////////////////////////////////////////////////////////////////////
class cMarkStatAlphaWdog
{

//-----------------------------------------------------------------------------
public:
    cMarkStatAlphaWdog();
    ~cMarkStatAlphaWdog();

    // Methods
    //--------
    void Initialize(cMonitorCommand &gMonitor, float CheckPeriod);
    void CheckRegDogs();
    static int  Llm(cParamString &OutputBuffer, cParamString &CommandLineArgs);

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;

    // Types
    //------

    // Methods
    //--------

    // Diagnostics
    //------------

    // Parameters
    //-----------

    // Published Variables
    //--------------------

    // Unpublished Variables
    //----------------------
    cSysSwWdogClient    SysWdog;        // watchdog registered with sys
    static float        Period;         // normal execution rate (sec)

    // Data Passing
    //-------------

    // Inline Function Definitions
    //----------------------------

};

///////////////////////////////////////////////////////////////////////////////
class cMarkStatWdog
{

//-----------------------------------------------------------------------------
public:
    cMarkStatWdog();
    ~cMarkStatWdog();

    // Methods
    //--------
    void Register(MarkStatWdogRegistration Reg);
    void Tickle();
    void Check();
    void ResetStats();
    void LlmSummary(cParamString &OutputBuffer, cParamString &CommandLineArgs);

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;

    // Types
    //------

    // Methods
    //--------

    // Diagnostics
    //------------
    CREATE_DIAG(WdogFailStartTrp);      // Watchdog has failed to start tickling
    CREATE_DIAG(WdogStoppedTrp);        // Watchdog has stopped executing
    CREATE_DIAG(WdogPeriodMaxTrp);      // Watchdog calculated period has exceeded the maximum
    CREATE_DIAG(WdogPeriodMinTrp);      // Watchdog calculated period is below the minimum

    // Parameters
    //-----------

    // Published Variables
    //--------------------
    CREATE_PUBVAR(AvgPeriod, float);    // average execution rate (sec)

    // Unpublished Variables
    //----------------------
    cDString        Name;               // Name for display
    float           Period;             // normal execution rate (sec)
    float           AvgMin;             // minimum execution rate (sec)
    float           AvgMax;             // maximum execution rate (sec)
    long            AvgCount;           // Number of tickles used to calculate the average
    float           InitTimer;          // maximum time to wait for tickling to start (sec)

    float           MaxPeriod;          // Maximum average execution rate (sec)
    float           MinPeriod;          // Minimum average execution rate (sec)
    unsigned long   TickleCnt;          // tickle counter
    unsigned long   TickleCnt_Z1;       // previous tickle counter from last check
    uint64_t        ClkCyc_Z1;          // previous clock cycle from last check
    uint64_t        ClkCycInc;          // clock cycle increment between checks
    bool            TickleGood;         // first tickle count may be incomplete
    int             LogDelay;           // delay the logging of error message

    // Data Passing
    //-------------

    // Inline Function Definitions
    //----------------------------

};


///////////////////////////////////////////////////////////////////////////////
#endif  // _X0XOPS_H_

