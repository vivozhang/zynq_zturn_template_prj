///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       General purpose diagnostic filters for MarkStat
//
// DESCRIPTION:
//              This file contains methods for the MarkStat general purpose
//              diagnostic filters.
//
// COPYRIGHT:   Copyright (c) 2014
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      x0GpDiagFilters.vsd, x0GpDiagFilters.xls
//
///////////////////////////////////////////////////////////////////////////////


// Includes Files
//---------------
// system
// core
// framework
#include "X0GpDiagFilter.h"
// Product


// Constants
//----------


// Externals
//----------


// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Variables
//----------
cGpDiagFilter*      pGpDiagFilterT1  = NULL;
cGpDiagFilter*      pGpDiagFilterT1A = NULL;


// Member Functions
//-----------------


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class constructor to initialize resources
//
// EXECUTION LEVEL
//      Class instancing.
//
// RETURN VALUE
//      None
//
/////////////////////////////////////////////////////////////////////////////
cGpDiagFilter::cGpDiagFilter
(
        InsListTyp   InsList
)
        : Ins(InsList)
{
    if (INS(TaskToExecute) == TASK1)
        pGpDiagFilterT1 = this;
    else if (INS(TaskToExecute) == TASK1A)
        pGpDiagFilterT1A = this;

    OUT(Filter1Out) = 0.0F;
    OUT(Filter2Out) = 0.0F;
    OUT(Filter3Out) = 0.0F;
    OUT(Filter4Out) = 0.0F;

    Filter1Gain = 0.0F;
    Filter2Gain = 0.0F;
    Filter3Gain = 0.0F;
    Filter4Gain = 0.0F;

    Filter1State = 0.0F;
    Filter2State = 0.0F;
    Filter3State = 0.0F;
    Filter4State = 0.0F;

    pNanDetector = &InitAdr;

    pFilter[0] = &InitAdr;
    pFilter[1] = &InitAdr;
    pFilter[2] = &InitAdr;
    pFilter[3] = &InitAdr;

    return;
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction.
//
// RETURN VALUE
//      None
//
/////////////////////////////////////////////////////////////////////////////
cGpDiagFilter::~cGpDiagFilter(void)
{
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Initialize all user parameters owned by general-purpose diagnostic
//      filter class.
//
// EXECUTION LEVEL
//      Initialization and when required.
//
// RETURN VALUE
//      None
//
/////////////////////////////////////////////////////////////////////////////
int cGpDiagFilter::RunRules(void)
{
    SET_PARM(Filter1Adr, (float *)LADR_UnusedFloat);
    SET_PARM(Filter1W, 0.0);

    SET_PARM(Filter2Adr, (float *)LADR_UnusedFloat);
    SET_PARM(Filter2W, 0.0);

    SET_PARM(Filter3Adr, (float *)LADR_UnusedFloat);
    SET_PARM(Filter3W, 0.0);

    SET_PARM(Filter4Adr, (float *)LADR_UnusedFloat);
    SET_PARM(Filter4W, 0.0);

    SET_PARM(NaNDetectAdr, (float *)LADR_UnusedFloat);

    return(SUCCESS);

 }


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Process the general-purpose diagnostic filter outputs.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      None
//
/////////////////////////////////////////////////////////////////////////////
void cGpDiagFilter::Task1(void)
{

    // Diagnositic filter #1
    Filter1State += ( (double)(*((float*)pFilter[0])) - Filter1State ) * (double)Filter1Gain;
    Filter1State  = cNaNCheck::NoDblNAN(Filter1State);
    OUT(Filter1Out) = (float)Filter1State;

    // Diagnositic filter #2
    Filter2State += ( (double)(*((float*)pFilter[1])) - Filter2State ) * (double)Filter2Gain;
    Filter2State  = cNaNCheck::NoDblNAN(Filter2State);
    OUT(Filter2Out) = (float)Filter2State;

    // Diagnositic filter #3
    Filter3State += ( (double)(*((float*)pFilter[2])) - Filter3State ) * (double)Filter3Gain;
    Filter3State  = cNaNCheck::NoDblNAN(Filter3State);
    OUT(Filter3Out) = (float)Filter3State;

    // Diagnositic filter #4
    Filter4State += ( (double)(*((float*)pFilter[3])) - Filter4State ) * (double)Filter4Gain;
    Filter4State  = cNaNCheck::NoDblNAN(Filter4State);
    OUT(Filter4Out) = (float)Filter4State;

    // NaN detector
    NaNDetected   = cNaNCheck::IsNAN( *((float*)pNanDetector) );

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Configures the diagnostic filters based on parameter settings.
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cGpDiagFilter::SetupDiagFilters(void)
{
    int     Temp1;

    // Diagnostic Filter #1
    //-------------------------------------------------------------------------
    if ( PARM(Filter1W) > 0 )
    {
        Filter1Gain = ( PARM(Filter1W) * INS(DelTm) ) / ( PARM(Filter1W) * INS(DelTm) + 1 );
    }
    else
    {
        Filter1Gain = 1;
    }
    Temp1 = gVariable.ReadType(PARM(Filter1Adr));  // Get var type of filter
    switch (Temp1)
    {
        case LA_FLOAT_TYPE: // Float var in LAT
        case LA_MEM_TYPE: // pseudo-published var is this type - assume float
            // Look up physical address in LAT
            pFilter[0] = (unsigned*)gVariable.GetAddress((unsigned)PARM(Filter1Adr));
            if (pFilter[0] == NULL)
            {
                pFilter[0] = (unsigned*)&FZero;
            }
            break;
        default:    // Not float, but in LAT
            // Set address to point at float zero
            pFilter[0] = (unsigned*)&FZero;
            break;
    }

    // Diagnostic Filter #2
    //-------------------------------------------------------------------------
    if ( PARM(Filter2W) > 0 )
    {
        Filter2Gain = ( PARM(Filter2W) * INS(DelTm) ) / ( PARM(Filter2W) * INS(DelTm) + 1 );
    }
    else
    {
        Filter2Gain = 1;
    }
    Temp1 = gVariable.ReadType(PARM(Filter2Adr));
    switch (Temp1)
    {
        case LA_FLOAT_TYPE:
        case LA_MEM_TYPE: // pseudo-published var is this type - assume float
            pFilter[1] = (unsigned*)gVariable.GetAddress((unsigned)PARM(Filter2Adr));
            if (pFilter[1] == NULL)
            {
                pFilter[1] = (unsigned*)&FZero;
            }
            break;
        default:
            pFilter[1] = (unsigned*)&FZero;
            break;
    }

    // Diagnostic Filter #3
    //-------------------------------------------------------------------------
    if ( PARM(Filter3W) > 0 )
    {
        Filter3Gain = ( PARM(Filter3W) * INS(DelTm) ) / ( PARM(Filter3W) * INS(DelTm) + 1 );
    }
    else
    {
        Filter3Gain = 1;
    }
    Temp1 = gVariable.ReadType(PARM(Filter3Adr));
    switch (Temp1)
    {
        case LA_FLOAT_TYPE:
        case LA_MEM_TYPE: // pseudo-published var is this type - assume float
            pFilter[2] = (unsigned*)gVariable.GetAddress((unsigned)PARM(Filter3Adr));
            if (pFilter[2] == NULL)
            {
                pFilter[2] = (unsigned*)&FZero;
            }
            break;
        default:
            pFilter[2] = (unsigned*)&FZero;
            break;
    }

    // Diagnostic Filter #4
    //-------------------------------------------------------------------------
    if ( PARM(Filter4W) > 0 )
    {
        Filter4Gain = ( PARM(Filter4W) * INS(DelTm) ) / ( PARM(Filter4W) * INS(DelTm) + 1 );
    }
    else
    {
        Filter4Gain = 1;
    }
    Temp1 = gVariable.ReadType(PARM(Filter4Adr));
    switch (Temp1)
    {
        case LA_FLOAT_TYPE:
        case LA_MEM_TYPE: // pseudo-published var is this type - assume float
            pFilter[3] = (unsigned*)gVariable.GetAddress((unsigned)PARM(Filter4Adr));
            if (pFilter[3] == NULL)
            {
                pFilter[3] = (unsigned*)&FZero;
            }
            break;
        default:
            pFilter[3] = (unsigned*)&FZero;
            break;
    }

    // NaN Detector
    //-------------------------------------------------------------------------
    Temp1 = gVariable.ReadType(PARM(NaNDetectAdr));
    switch (Temp1)
    {
        case LA_FLOAT_TYPE:
        case LA_MEM_TYPE: // pseudo-published var is this type - assume float
            pNanDetector = (unsigned*)gVariable.GetAddress((unsigned)PARM(NaNDetectAdr));
            if (pNanDetector == NULL)
            {
                pNanDetector = (unsigned*)&FZero;
            }
            break;
        default:
            pNanDetector = (unsigned*)&FZero;
            break;
    }

    return;
}

