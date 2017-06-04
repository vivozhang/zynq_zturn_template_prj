///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       General purpose diagnostic filters for MarkStat
//
// DESCRIPTION:
//              This file contains a base class definition for implementation
//              of MarkStat diagnostic filters.
//
// COPYRIGHT:   Copyright (c) 2014
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      x0GpDiagFilters.vsd, x0GpDiagFilters.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef X0_GPDIAGFILT_H
#define X0_GPDIAGFILT_H


// Include Files:
//---------------
// System
// Core
// Framework
#include "x0FrameWork.h"
// Product


// Constants:
//-----------


// Function Prototypes:
//---------------------


// Types and Classes:
//-------------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      This base class provides a set of four configurable user pointer-
//  parameters to be used as inputs to generic low-pass filters. Each
//  filter's cut-off frequency is individually adjusted through user
//  parameters as well.
//
///////////////////////////////////////////////////////////////////////////////
class cGpDiagFilter
{
//-----------------------------------------------------------------------------
public:
    // Types & Enumerations
    //---------------------
    enum TaskEnum
    {
        TASK1,
        TASK1A
    };
    struct InsListTyp
    {
        TaskEnum            TaskToExecute;  // 1 - which task 1 to execute filter
        const float&        DelTm;          // 2 - Delta time for task 1 (Sec)
    };

    // Methods
    //--------
    cGpDiagFilter(InsListTyp InsList);
    ~cGpDiagFilter(void);

    void    Task1(void);
    void    SetupDiagFilters(void);
    int     RunRules(void);

    // Output Signals
    //---------------
    CREATE_PUBVAR(Filter1Out,   float);     // Diagnostic filter outputs
    CREATE_PUBVAR(Filter2Out,   float);
    CREATE_PUBVAR(Filter3Out,   float);
    CREATE_PUBVAR(Filter4Out,   float);

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;

    // Types & Enumerations
    //---------------------

    // External Inputs
    //----------------
    InsListTyp   Ins;

    // Methods
    //--------

    // Diagnostics
    //------------

    // Parameters
    //-----------
    CREATE_PARM(Filter1Adr,     float*);    // Diagnostic filter pointer-parameters
    CREATE_PARM(Filter2Adr,     float*);
    CREATE_PARM(Filter3Adr,     float*);
    CREATE_PARM(Filter4Adr,     float*);
    CREATE_PARM(NaNDetectAdr,   float*);    // Pointer-parameter for NaN detection
    CREATE_PARM(Filter1W,       float);     // Diagnostic filter omegas (Rad/Sec)
    CREATE_PARM(Filter2W,       float);
    CREATE_PARM(Filter3W,       float);
    CREATE_PARM(Filter4W,       float);

    // Published Variables
    //--------------------
    CREATE_PUBVAR(NaNDetected,  unsigned);  // Indication of Not-A-Number existance

    // Unpublished Variables
    //----------------------
    float       Filter1Gain;                // Diagnostic filter gains
    float       Filter2Gain;
    float       Filter3Gain;
    float       Filter4Gain;

    double      Filter1State;               // Filter state variables
    double      Filter2State;
    double      Filter3State;
    double      Filter4State;

    unsigned    InitAdr;                    // Variable used as pointer initializer
    unsigned*   pFilter[4];                 // Diagnostic filter address array
    unsigned*   pNanDetector;               // Pointer to memory location being
                                            // evaluated for NaN

    // Data Passing
    //-------------

    // Inline Function Definitions
    //----------------------------

};


#endif          // X0_GPDIAGFILT_H
