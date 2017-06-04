///////////////////////////////////////////////////////////////////////////////
//
// TITLE        BatSimulator
//
// DESCRIPTION
//      This file contains the methods for the class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      s0BatSimulator.h, s0BatSimulator.vsd, s0BatSimulator.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
// product
#include "Product.h"
#include "s0BatSimulator.h"


// Constants
//----------


// Externals
//----------


// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Diagnostics
//------------


// Parameters
//-----------


// Published Variables
//--------------------


// Unpublished Variables
//----------------------


// Data Passing
//-------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------



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
cBatSimulator::cBatSimulator
(
        InTyp    InList,
        InsTyp   InsList
)
        : In(InList), Ins(InsList)
{
    // initilize
    BatIcSim            = 0.0;
    BatIbatSim          = 0.0;

    OUT(BatVchprSim)    = 0.0;
    OUT(BatIchprSim)    = 0.0;

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
cBatSimulator::~cBatSimulator ()
{
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by the class.
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cBatSimulator::RunRules (void)
{
    int Status = SUCCESS;

    //-------------------------------------------------------------------------
    // Simulator
    SET_FV( BatContSim,   true);
    SET_FV( BatIdscSimEnb,   true);
    SET_FV( BatEbatSim,      500.0F);
    SET_FV( BatRbatSim,      INS(Rbat));
    SET_FV( BatLchprSim,     INS(Lchpr));
    SET_FV( BatCchprSim,     INS(Cchpr));
    SET_FV( BatCchprRSim,    1.0F);

    BatRbatSimRcp    = NoNANLim( 1.0F        / PARM(BatRbatSim) , 1.0e6F, -1.0e6F );
    BatCchprSimRcpDt = NoNANLim( INS(DelTm1) / PARM(BatCchprSim), 1.0e6F, -1.0e6F );
    BatLchprSimRcpDt = NoNANLim( INS(DelTm1) / PARM(BatLchprSim), 1.0e6F, -1.0e6F );


    return (Status);
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Battery Chopper Voltage and current simulator
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBatSimulator::BatSimulator( void)
{
    float Temp1;

    if ( !BatSimEnb)
    {
        BatIbatSim  = 0.0F;
        BatIcSim    = 0.0F;
        BatVbatSim  = PARM(BatEbatSim);
        BatVcSim    = (PARM(BatContSim)) ? PARM(BatEbatSim) : 0.0F ;

        OUT(BatVchprSim) = BatVcSim;    // make this conditional on IN(BrkrClosed) ?
        OUT(BatIchprSim) = 0.0F;
        
        BatIchprIsDscSim = true;
    }
    else
    {
        // Determine whether chopper current is continuous or discontinuous

        // Determine maximum discontinuous current (equals minimum continuous current)
        BatVdcVbatDifSim = BatVdcSim - BatVcSim;
        BatIdscMaxSim    = NoNANLim( ( BatVdcVbatDifSim * BatVcSim * INS(DelTm1))/( BatVdcSim * PARM(BatLchprSim)), 1.0e6F, -1.0e6F );

        // If existing current is above maximum discontinuous current, select continuous current simulator
        if ( (ABS(BatIchprSim) > BatIdscMaxSim) || !PARM(BatIdscSimEnb) )
        {
            BatIchprIsDscSim = false;
        }
        else
        {
            // Determine pulse size for maximum discontinuous current and compare to commanded pulse
            if ( BatChgSwEnbSim)
            {
                // Charging
                Temp1 = 4.0F * BatIdscMaxSim * BatVcSim * PARM(BatLchprSim) * INS(DelTm1);
                Temp1 = NoNANLim( Temp1 /( BatVdcSim * BatVdcVbatDifSim), 1.0e6F, -1.0e6F );
                BatIdscPlsTmMaxSim = sqrtf(MAX(0.0F,Temp1));
                BatIdscUcmdLimSim  = MIN(1.0F, 0.5F * (BatIdscPlsTmMaxSim * INS(DelTm1Rcp)));

                if (BatDutySim > BatIdscUcmdLimSim)
                {
                    BatIchprIsDscSim = false;
                }
                else
                {
                    BatIchprIsDscSim = true;
                }
            }
            else if (BatDisSwEnbSim)
            {
                // Discharging
                Temp1 = 4.0F * BatIdscMaxSim * BatVdcVbatDifSim * PARM(BatLchprSim) * INS(DelTm1);
                Temp1 = NoNANLim( Temp1 /( BatVdcSim * BatVcSim), 1.0e6F, -1.0e6F );
                BatIdscPlsTmMaxSim = sqrtf(MAX(0.0F,Temp1));
                BatIdscUcmdLimSim  = MAX(0.0F, 1.0F - 0.5F * (BatIdscPlsTmMaxSim * INS(DelTm1Rcp)));

                if (BatDutySim < BatIdscUcmdLimSim)
                {
                    BatIchprIsDscSim = false;
                }
                else
                {
                    BatIchprIsDscSim = true;
                }
            }
        }

        if (BatIchprIsDscSim)
        // Discontinuous current simulator
        {
            // voltage across filter inductor during pulse and during current decay
            if (BatChgSwEnbSim)
            {
                BatVlPlsSim = MAX(0.0F, BatVdcSim - BatVcSim);
                BatVlDcySim = -BatVcSim;
            }
            else
            {
                BatVlPlsSim = -BatVcSim;
                BatVlDcySim = MAX(0.0F, BatVdcSim - BatVcSim);
            }

            // chopper pulse time
            if (BatChgSwEnbSim)
            {
                BatPlsTmSim  = BatDutySim * 2.0F * INS(DelTm1);
            }
            else if (BatDisSwEnbSim)
            {
                BatPlsTmSim  = (1.0F - BatDutySim) * 2.0F * INS(DelTm1);
            }
            else
            {
                BatPlsTmSim  = 0.0F;
            }

            // chopper current decay time when igbt is turned off (limit to 10 X pulse time, when VcSim is ~0.)
            BatDcyTmSim  = BatPlsTmSim * ABS(BatVlPlsSim) / MAX(ABS(BatVlDcySim), 0.1F*ABS(BatVlPlsSim));

            // peak of chopper discontinuous current
            BatIdscPkSim = BatVlPlsSim * BatPlsTmSim /PARM(BatLchprSim);

            // average chopper current during sample interval
            OUT(BatIchprSim)  = 0.25F * BatIdscPkSim * (BatPlsTmSim + BatDcyTmSim) * INS(DelTm1Rcp);

            if (!BatDisSwEnbSim && (BatIchprSim < 0.0F)) BatIchprSim = 0.0F;
            if (!BatChgSwEnbSim && (BatIchprSim > 0.0F)) BatIchprSim = 0.0F;

            // battery current
            BatIbatSim  = (PARM(BatContSim)) ? (BatVcSim - PARM(BatEbatSim) ) * BatRbatSimRcp : 0.0F ;

            // filter capacitor current
            BatIcSim    = BatIchprSim - BatIbatSim;

            // voltage across filter capacitor
            BatVcSim    += BatIcSim * BatCchprSimRcpDt;
            if ( BatVcSim < 0.0F ) BatVcSim = 0.0F;

            // chopper voltage
            OUT(BatVchprSim) = BatVcSim;  // could add some cable IR drop here
        }
        else
        // Continuous current simulator
        {
            // battery current
            BatIbatSim  = (PARM(BatContSim)) ? (BatVcSim - PARM(BatEbatSim) ) * BatRbatSimRcp : 0.0F ;

            // filter capacitor current
            BatIcSim    = BatIchprSim - BatIbatSim;

            // voltage across filter capacitor
            BatVcSim    += BatIcSim * BatCchprSimRcpDt;
            if ( BatVcSim < 0.0F ) BatVcSim = 0.0F;

            // chopper current
            OUT(BatIchprSim) += (BatVchprSim - BatVcSim) * BatLchprSimRcpDt;

            if (PARM(BatIdscSimEnb))
            {
                if (!BatDisSwEnbSim && (BatIchprSim < 0.0F)) BatIchprSim = 0.0F;
                if (!BatChgSwEnbSim && (BatIchprSim > 0.0F)) BatIchprSim = 0.0F;
            }
            // chopper voltage 
            OUT(BatVchprSim) = BatDutySim * BatVdcSim;;
        }
        BatVbatSim = (PARM(BatContSim)) ? BatVcSim : PARM(BatEbatSim) ;
    }


    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Task 1 functions
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBatSimulator::Task1 (void)
{

    BatVdcSim      = IN(VdcFbk);
    BatDutySim     = IN(DutyRef);
    BatChgSwEnbSim = IN(ChgSwEnb);
    BatDisSwEnbSim = IN(DisSwEnb);

    BatSimEnb      = IN(ChgSwEnb) || IN(DisSwEnb);    // enable simulated chopper

    // Calculate the battery chopper voltage and current
    BatSimulator();

    return;
}

