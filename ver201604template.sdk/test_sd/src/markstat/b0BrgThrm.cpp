///////////////////////////////////////////////////////////////////////////////
//
// TITLE     Bridge thermal model (Btm) class for 2-level PWM inverter
//
// DESCRIPTION
//           This file contains the methods for the bridge thermal model class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0BrgThrm.vsd, b0BrgThrm.xls
//
///////////////////////////////////////////////////////////////////////////////

#if 0       // This code is only for reference purposes. Do not use it !

// Include Files
//--------------
// system
// core
// framework
// product
#include "Product.h"
#include "b0BrgThrm.h"


// Constants
//----------


// Static Class Members (by default, compiler initializes them to zero
//                       if no explicit initialization is provided):
//--------------------------------------------------------------------


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
cBrgThrmModel::cBrgThrmModel
(
        InListTyp    InList,
        InsListTyp   InsList
)
        : BTM_GN_CONST(0.582F),     // constant member initializer
          In(InList), Ins(InsList)
{
    BtmAlarmStPhsA        = false;
    BtmAlarmStPhsB        = false;
    BtmAlarmStPhsC        = false;
    BtmAlarmStDphsA       = false;
    BtmAlarmStDphsB       = false;
    BtmAlarmStDphsC       = false;

    BtmIaAbs              = 0.0;
    BtmIbAbs              = 0.0;
    BtmIcAbs              = 0.0;
    BtmIaSqr              = 0.0;
    BtmIbSqr              = 0.0;
    BtmIcSqr              = 0.0;
    BtmUaOld              = 0.0;
    BtmUbOld              = 0.0;
    BtmUcOld              = 0.0;
    BtmUOldThrHi          = 0.0;
    BtmUOldThrLo          = 0.0;
    BtmTrSwPwr            = 0.0;
    BtmTrCondPwr          = 0.0;
    BtmDSwPwr             = 0.0;
    BtmDCondPwr           = 0.0;
    BtmTrSwGn             = 0.0;
    BtmTrSwGnT1           = 0.0;
    BtmDSwGn              = 0.0;
    BtmDSwGnT1            = 0.0;
    BtmTrPwrAL            = 0.0;
    BtmTrPwrAU            = 0.0;
    BtmTrPwrBL            = 0.0;
    BtmTrPwrBU            = 0.0;
    BtmTrPwrCL            = 0.0;
    BtmTrPwrCU            = 0.0;
    BtmDPwrAL             = 0.0;
    BtmDPwrAU             = 0.0;
    BtmDPwrBL             = 0.0;
    BtmDPwrBU             = 0.0;
    BtmDPwrCL             = 0.0;
    BtmDPwrCU             = 0.0;
    BtmTrPwrALT3          = 0.0;
    BtmTrPwrAUT3          = 0.0;
    BtmTrPwrBLT3          = 0.0;
    BtmTrPwrBUT3          = 0.0;
    BtmTrPwrCLT3          = 0.0;
    BtmTrPwrCUT3          = 0.0;
    BtmDPwrALT3           = 0.0;
    BtmDPwrAUT3           = 0.0;
    BtmDPwrBLT3           = 0.0;
    BtmDPwrBUT3           = 0.0;
    BtmDPwrCLT3           = 0.0;
    BtmDPwrCUT3           = 0.0;
    BtmRbRcp              = 0.0;
    BtmDRbRcp             = 0.0;
    BtmTaGn               = 0.0;
    BtmDTaGn              = 0.0;
    BtmTempAAL            = 0.0;
    BtmTempAAU            = 0.0;
    BtmTempABL            = 0.0;
    BtmTempABU            = 0.0;
    BtmTempACL            = 0.0;
    BtmTempACU            = 0.0;
    BtmDTempAAL           = 0.0;
    BtmDTempAAU           = 0.0;
    BtmDTempABL           = 0.0;
    BtmDTempABU           = 0.0;
    BtmDTempACL           = 0.0;
    BtmDTempACU           = 0.0;
    BtmABPwrA             = 0.0;
    BtmABPwrAL            = 0.0;
    BtmABPwrAU            = 0.0;
    BtmABPwrB             = 0.0;
    BtmABPwrBL            = 0.0;
    BtmABPwrBU            = 0.0;
    BtmABPwrC             = 0.0;
    BtmABPwrCL            = 0.0;
    BtmABPwrCU            = 0.0;
    BtmDABPwrA            = 0.0;
    BtmDABPwrAL           = 0.0;
    BtmDABPwrAU           = 0.0;
    BtmDABPwrB            = 0.0;
    BtmDABPwrBL           = 0.0;
    BtmDABPwrBU           = 0.0;
    BtmDABPwrC            = 0.0;
    BtmDABPwrCL           = 0.0;
    BtmDABPwrCU           = 0.0;
    BtmTempJncAL          = 0.0;
    BtmTempJncAU          = 0.0;
    BtmTempJncBL          = 0.0;
    BtmTempJncBU          = 0.0;
    BtmTempJncCL          = 0.0;
    BtmTempJncCU          = 0.0;
    BtmDTempJncAL         = 0.0;
    BtmDTempJncAU         = 0.0;
    BtmDTempJncBL         = 0.0;
    BtmDTempJncBU         = 0.0;
    BtmDTempJncCL         = 0.0;
    BtmDTempJncCU         = 0.0;
    BtmJncTempMax         = 0.0;
    BtmJncTempErr         = 0.0;
    BtmDJncTempMx         = 0.0;
    BtmDJncTempErr        = 0.0;
    BtmJncTempUpGn        = 0.0;
    BtmJncTempDnGn        = 0.0;
    BtmJncOvrTempThr      = 0.0;
    BtmJncOvrTempOn       = 0.0;
    BtmJncOvrTempOff      = 0.0;
    BtmDjncOvrTempThr     = 0.0;
    BtmDjncOvrTempOn      = 0.0;
    BtmDjncOvrTempOff     = 0.0;

    OUT(BtmDPwrLoss)      = 0.0;
    OUT(BtmTrPwrLoss)     = 0.0;
    OUT(BtmJncTemp)       = 0.0;
    OUT(BtmDJncTemp)      = 0.0;
    OUT(BtmOtFaultSum)    = false;
    OUT(BtmOtFaultDlySum) = false;
    OUT(BtmOtAlarmSum)    = false;

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
cBrgThrmModel::~cBrgThrmModel()
{
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by bridge thermal model (Btm) class.
//
// EXECUTION LEVEL
//      Initialization and when required.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cBrgThrmModel::RunRules(void)
{
    int Status = SUCCESS;

    #include "b0BrgThrm.inc"      // product defined rules

    // Calculate variables used in Task 1 power loss calculation

    BtmUOldThrLo = NoNANLim( INS(HalfQuantPerT1Rcp) * 0.25F, 1.0e6F, -1.0e6F );
    BtmUOldThrHi = NoNANLim( 1.0F - ( INS(HalfQuantPerT1Rcp) * 0.25F ), 1.0e6F, -1.0e6F );

    // Calculate variables used in Task 3 junction temp calculations

    BtmRbRcp  = NoNANLim( 1.0F / PARM(BtmRbTh),  1.0e6F, -1.0e6F );
    BtmDRbRcp = NoNANLim( 1.0F / PARM(BtmDRbTh), 1.0e6F, -1.0e6F );

    BtmTaGn   = NoNANLim( INS(DelTm3) / (( INS(DelTm3) * BTM_GN_CONST * BtmRbRcp ) + PARM(BtmCaTh)),
                          1.0e6F, -1.0e6F);
    BtmDTaGn  = NoNANLim( INS(DelTm3) / (( INS(DelTm3) * BTM_GN_CONST * BtmDRbRcp ) + PARM(BtmDCaTh)),
                          1.0e6F, -1.0e6F);

    BtmJncTempUpGn = INS(DelTm3) * PARM(BtmWjncUp);
    BtmJncTempDnGn = INS(DelTm3) * PARM(BtmWjncDn);

    // Copy parameter to private variables
    BtmJncOvrTempThr  = PARM(BtmJncOTThr);
    BtmJncOvrTempOn   = PARM(BtmJncOTOn);
    BtmJncOvrTempOff  = PARM(BtmJncOTOff);
    BtmDjncOvrTempThr = PARM(BtmDJncOTThr);
    BtmDjncOvrTempOn  = PARM(BtmDJncOTOn);
    BtmDjncOvrTempOff = PARM(BtmDJncOTOff);

    return (Status);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    This function calculates power switch conduction and switching losses,
//    and diode conduction losses for each phase of the power bridge.
//
//    Note: This function must run after adquiring, processing, and verifying
//          feedback signals (currents and voltages), and before new modulator
//          commands are synthesized. This execution sequence is necessary to
//          provide correct latency model.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrmModel::Task1( void )
{
    //--------------------------------------------------------------------------
    //   Power Bridge Thermal Model, power device loss calculation.

    // Phase A

    BtmIaAbs = fabs( IN(IaFbk) );
    BtmIaSqr = IN(IaFbk) * IN(IaFbk);

    BtmTrCondPwr = ( PARM(BtmTrVo) * BtmIaAbs ) + ( PARM(BtmTrR) * BtmIaSqr );

    if ( ( BtmUaOld < BtmUOldThrLo ) || ( BtmUaOld > BtmUOldThrHi ) || (!IN(BrgPwrEnb)) )
    {
        BtmTrSwPwr = 0.0F;
        BtmDSwPwr  = 0.0F;
    }
    else
    {
        BtmTrSwPwr = BtmTrSwGnT1 * ( PARM(BtmTrSw0) + ( PARM(BtmTrSw1) * BtmIaAbs )
                                  + ( PARM(BtmTrSw2) * BtmIaSqr ));
        BtmDSwPwr  = BtmDSwGnT1  * ( PARM(BtmDiodSw0) + ( PARM(BtmDiodSw1) * BtmIaAbs )
                                  + ( PARM(BtmDiodSw2) * BtmIaSqr ));
    }

    BtmDCondPwr = ( PARM(BtmDiodeVo) * BtmIaAbs ) + ( PARM(BtmDiodeR) * BtmIaSqr );

    if ( IN(IaFbk) >= 0.0F )
    {
        BtmTrSwchPwrAU = BtmTrSwPwr;
        BtmTrCondPwrAU = ( BtmUaOld * BtmTrCondPwr );

        BtmTrSwchPwrAL = 0.0F;
        BtmTrCondPwrAL = 0.0F;

        BtmDswchPwrAU = 0.0F;
        BtmDcondPwrAU = 0.0F;

        BtmDswchPwrAL = BtmDSwPwr;
        BtmDcondPwrAL = (( 1.0F - BtmUaOld ) * BtmDCondPwr);
    }
    else
    {
        BtmTrSwchPwrAU = 0.0F;
        BtmTrCondPwrAU = 0.0F;

        BtmTrSwchPwrAL = BtmTrSwPwr;
        BtmTrCondPwrAL = (( 1.0F - BtmUaOld ) * BtmTrCondPwr );

        BtmDswchPwrAU = BtmDSwPwr;
        BtmDcondPwrAU = ( BtmUaOld * BtmDCondPwr );

        BtmDswchPwrAL = 0.0F;
        BtmDcondPwrAL = 0.0F;
    }
    BtmTrPwrAU = BtmTrSwchPwrAU + BtmTrCondPwrAU;
    BtmTrPwrAL = BtmTrSwchPwrAL + BtmTrCondPwrAL;
    BtmDPwrAU  = BtmDswchPwrAU  + BtmDcondPwrAU;
    BtmDPwrAL  = BtmDswchPwrAL  + BtmDcondPwrAL;

    // Phase B

    BtmIbAbs = fabs( IN(IbFbk) );
    BtmIbSqr = IN(IbFbk) * IN(IbFbk);

    BtmTrCondPwr = ( PARM(BtmTrVo) * BtmIbAbs ) + ( PARM(BtmTrR) * BtmIbSqr );

    if ( ( BtmUbOld < BtmUOldThrLo ) || ( BtmUbOld > BtmUOldThrHi ) || (!IN(BrgPwrEnb)) )
    {
        BtmTrSwPwr = 0.0F;
        BtmDSwPwr  = 0.0F;
    }
    else
    {

        BtmTrSwPwr = BtmTrSwGnT1 * ( PARM(BtmTrSw0) + ( PARM(BtmTrSw1) * BtmIbAbs )
                                  + ( PARM(BtmTrSw2) * BtmIbSqr ));
        BtmDSwPwr  = BtmDSwGnT1  * ( PARM(BtmDiodSw0) + ( PARM(BtmDiodSw1) * BtmIbAbs )
                                  + ( PARM(BtmDiodSw2) * BtmIbSqr ));
    }

    BtmDCondPwr = ( PARM(BtmDiodeVo) * BtmIbAbs ) + ( PARM(BtmDiodeR) * BtmIbSqr );

    if ( IN(IbFbk) >= 0.0F )
    {
        BtmTrSwchPwrBU = BtmTrSwPwr;
        BtmTrCondPwrBU = ( BtmUbOld * BtmTrCondPwr );

        BtmTrSwchPwrBL = 0.0F;
        BtmTrCondPwrBL = 0.0F;

        BtmDswchPwrBU = 0.0F;
        BtmDcondPwrBU = 0.0F;

        BtmDswchPwrBL = BtmDSwPwr;
        BtmDcondPwrBL = (( 1.0F - BtmUbOld ) * BtmDCondPwr);
    }
    else
    {
        BtmTrSwchPwrBU = 0.0F;
        BtmTrCondPwrBU = 0.0F;

        BtmTrSwchPwrBL = BtmTrSwPwr;
        BtmTrCondPwrBL = (( 1.0F - BtmUbOld ) * BtmTrCondPwr );

        BtmDswchPwrBU = BtmDSwPwr;
        BtmDcondPwrBU = ( BtmUbOld * BtmDCondPwr );

        BtmDswchPwrBL = 0.0F;
        BtmDcondPwrBL = 0.0F;
    }
    BtmTrPwrBU = BtmTrSwchPwrBU + BtmTrCondPwrBU;
    BtmTrPwrBL = BtmTrSwchPwrBL + BtmTrCondPwrBL;
    BtmDPwrBU  = BtmDswchPwrBU  + BtmDcondPwrBU;
    BtmDPwrBL  = BtmDswchPwrBL  + BtmDcondPwrBL;

    // Phase C

    BtmIcAbs = fabs( IN(IcFbk) );
    BtmIcSqr = IN(IcFbk) * IN(IcFbk);

    BtmTrCondPwr = ( PARM(BtmTrVo) * BtmIcAbs ) + ( PARM(BtmTrR) * BtmIcSqr );

    if ( ( BtmUcOld < BtmUOldThrLo ) || ( BtmUcOld > BtmUOldThrHi ) || (!IN(BrgPwrEnb)) )
    {
        BtmTrSwPwr = 0.0F;
        BtmDSwPwr  = 0.0F;
    }
    else
    {
        BtmTrSwPwr = BtmTrSwGnT1 * ( PARM(BtmTrSw0) + ( PARM(BtmTrSw1) * BtmIcAbs )
                                  + ( PARM(BtmTrSw2) * BtmIcSqr ));
        BtmDSwPwr  = BtmDSwGnT1  * ( PARM(BtmDiodSw0) + ( PARM(BtmDiodSw1) * BtmIcAbs )
                                  + ( PARM(BtmDiodSw2) * BtmIcSqr ));
    }

    BtmDCondPwr = ( PARM(BtmDiodeVo) * BtmIcAbs ) + ( PARM(BtmDiodeR) * BtmIcSqr );

    if ( IN(IcFbk) >= 0.0F )
    {
        BtmTrSwchPwrCU = BtmTrSwPwr;
        BtmTrCondPwrCU = ( BtmUcOld * BtmTrCondPwr );

        BtmTrSwchPwrCL = 0.0F;
        BtmTrCondPwrCL = 0.0F;

        BtmDswchPwrCU = 0.0F;
        BtmDcondPwrCU = 0.0F;

        BtmDswchPwrCL = BtmDSwPwr;
        BtmDcondPwrCL = (( 1.0F - BtmUcOld ) * BtmDCondPwr);
    }
    else
    {
        BtmTrSwchPwrCU = 0.0F;
        BtmTrCondPwrCU = 0.0F;

        BtmTrSwchPwrCL = BtmTrSwPwr;
        BtmTrCondPwrCL = (( 1.0F - BtmUcOld ) * BtmTrCondPwr );

        BtmDswchPwrCU = BtmDSwPwr;
        BtmDcondPwrCU = ( BtmUcOld * BtmDCondPwr );

        BtmDswchPwrCL = 0.0F;
        BtmDcondPwrCL = 0.0F;
    }
    BtmTrPwrCU = BtmTrSwchPwrCU + BtmTrCondPwrCU;
    BtmTrPwrCL = BtmTrSwchPwrCL + BtmTrCondPwrCL;
    BtmDPwrCU  = BtmDswchPwrCU  + BtmDcondPwrCU;
    BtmDPwrCL  = BtmDswchPwrCL  + BtmDcondPwrCL;

    // Scale and store up/down commands
    BtmUaOld = ( IN(ModUaCmdZ1) + 1.0F ) * 0.5F;
    BtmUbOld = ( IN(ModUbCmdZ1) + 1.0F ) * 0.5F;
    BtmUcOld = ( IN(ModUcCmdZ1) + 1.0F ) * 0.5F;

    // Total power loss per type of event and device
    BtmDcondPwrLoss  = BtmDcondPwrAU  + BtmDcondPwrAL  + BtmDcondPwrBU  + BtmDcondPwrBL  + BtmDcondPwrCU  + BtmDcondPwrCL;
    BtmDswchPwrLoss  = BtmDswchPwrAU  + BtmDswchPwrAL  + BtmDswchPwrBU  + BtmDswchPwrBL  + BtmDswchPwrCU  + BtmDswchPwrCL;
    BtmTrCondPwrLoss = BtmTrCondPwrAU + BtmTrCondPwrAL + BtmTrCondPwrBU + BtmTrCondPwrBL + BtmTrCondPwrCU + BtmTrCondPwrCL;
    BtmTrSwchPwrLoss = BtmTrSwchPwrAU + BtmTrSwchPwrAL + BtmTrSwchPwrBU + BtmTrSwchPwrBL + BtmTrSwchPwrCU + BtmTrSwchPwrCL;

    // Total power loss per type of device
    OUT(BtmDPwrLoss)  = BtmDPwrAU  + BtmDPwrBU  + BtmDPwrCU  + BtmDPwrAL  + BtmDPwrBL  + BtmDPwrCL;
    OUT(BtmTrPwrLoss) = BtmTrPwrAU + BtmTrPwrBU + BtmTrPwrCU + BtmTrPwrAL + BtmTrPwrBL + BtmTrPwrCL;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculate and filter total bridge loss per phase. Choose bridge
//      reference temperature. Apply over-temperature limits and push
//      corresponding alarms.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrmModel::Task3( void )
{
    //   Calculate variables used in Task 1 phase loss method

    BtmTrSwGn = INS(PwmPulseNumb) * INS(DelTm1Rcp) * IN(VdcFbkT3);
    BtmDSwGn  = BtmTrSwGn;

    // If heatsink temperatures are not valid, initialize integrator states

    if ( IN(BpTempValid) != 1 )
    {
        BtmTempAAU  = IN(BpTempAU);
        BtmTempAAL  = IN(BpTempAL);
        BtmTempABU  = IN(BpTempBU);
        BtmTempABL  = IN(BpTempBL);
        BtmTempACU  = IN(BpTempCU);
        BtmTempACL  = IN(BpTempCL);

        BtmDTempAAU = IN(BpTempAU);
        BtmDTempAAL = IN(BpTempAL);
        BtmDTempABU = IN(BpTempBU);
        BtmDTempABL = IN(BpTempBL);
        BtmDTempACU = IN(BpTempCU);
        BtmDTempACL = IN(BpTempCL);
    }

    //--------------------------------------------------------------------------
    //   Calculate transistor junction temperature

    // Calculate baseplate power

    BtmABPwrAU  = BtmRbRcp * ( BtmTempAAU - IN(BpTempAU) );
    BtmABPwrAL  = BtmRbRcp * ( BtmTempAAL - IN(BpTempAL) );
    BtmABPwrBU  = BtmRbRcp * ( BtmTempABU - IN(BpTempBU) );
    BtmABPwrBL  = BtmRbRcp * ( BtmTempABL - IN(BpTempBL) );
    BtmABPwrCU  = BtmRbRcp * ( BtmTempACU - IN(BpTempCU) );
    BtmABPwrCL  = BtmRbRcp * ( BtmTempACL - IN(BpTempCL) );

    BtmABPwrA   = BtmABPwrAU + BtmABPwrAL;
    BtmABPwrB   = BtmABPwrBU + BtmABPwrBL;
    BtmABPwrC   = BtmABPwrCU + BtmABPwrCL;

    // Calculate die temperatures

    BtmTempAAU += BtmTaGn * ( BtmTrPwrAUT3 - BtmABPwrAU );
    BtmTempAAL += BtmTaGn * ( BtmTrPwrALT3 - BtmABPwrAL );
    BtmTempABU += BtmTaGn * ( BtmTrPwrBUT3 - BtmABPwrBU );
    BtmTempABL += BtmTaGn * ( BtmTrPwrBLT3 - BtmABPwrBL );
    BtmTempACU += BtmTaGn * ( BtmTrPwrCUT3 - BtmABPwrCU );
    BtmTempACL += BtmTaGn * ( BtmTrPwrCLT3 - BtmABPwrCL );

    // Calculate junction temperatures

    BtmTempJncAU = BtmTempAAU + ( PARM(BtmRaTh) * BtmTrPwrAUT3 );
    BtmTempJncAL = BtmTempAAL + ( PARM(BtmRaTh) * BtmTrPwrALT3 );
    BtmTempJncBU = BtmTempABU + ( PARM(BtmRaTh) * BtmTrPwrBUT3 );
    BtmTempJncBL = BtmTempABL + ( PARM(BtmRaTh) * BtmTrPwrBLT3 );
    BtmTempJncCU = BtmTempACU + ( PARM(BtmRaTh) * BtmTrPwrCUT3 );
    BtmTempJncCL = BtmTempACL + ( PARM(BtmRaTh) * BtmTrPwrCLT3 );


    //--------------------------------------------------------------------------
    //   Calculate diode junction temperature

    // Calculate diode baseplate power

    BtmDABPwrAU  = BtmDRbRcp * ( BtmDTempAAU - IN(BpTempAU) );
    BtmDABPwrAL  = BtmDRbRcp * ( BtmDTempAAL - IN(BpTempAL) );
    BtmDABPwrBU  = BtmDRbRcp * ( BtmDTempABU - IN(BpTempBU) );
    BtmDABPwrBL  = BtmDRbRcp * ( BtmDTempABL - IN(BpTempBL) );
    BtmDABPwrCU  = BtmDRbRcp * ( BtmDTempACU - IN(BpTempCU) );
    BtmDABPwrCL  = BtmDRbRcp * ( BtmDTempACL - IN(BpTempCL) );

    BtmDABPwrA   = BtmDABPwrAU + BtmDABPwrAL;
    BtmDABPwrB   = BtmDABPwrBU + BtmDABPwrBL;
    BtmDABPwrC   = BtmDABPwrCU + BtmDABPwrCL;

    // Calculate diode die temperatures

    BtmDTempAAU += BtmDTaGn * ( BtmDPwrAUT3 - BtmDABPwrAU );
    BtmDTempAAL += BtmDTaGn * ( BtmDPwrALT3 - BtmDABPwrAL );
    BtmDTempABU += BtmDTaGn * ( BtmDPwrBUT3 - BtmDABPwrBU );
    BtmDTempABL += BtmDTaGn * ( BtmDPwrBLT3 - BtmDABPwrBL );
    BtmDTempACU += BtmDTaGn * ( BtmDPwrCUT3 - BtmDABPwrCU );
    BtmDTempACL += BtmDTaGn * ( BtmDPwrCLT3 - BtmDABPwrCL );

    // Calculate diode junction temperatures

    BtmDTempJncAU = BtmDTempAAU + ( PARM(BtmDRaTh) * BtmDPwrAUT3 );
    BtmDTempJncAL = BtmDTempAAL + ( PARM(BtmDRaTh) * BtmDPwrALT3 );
    BtmDTempJncBU = BtmDTempABU + ( PARM(BtmDRaTh) * BtmDPwrBUT3 );
    BtmDTempJncBL = BtmDTempABL + ( PARM(BtmDRaTh) * BtmDPwrBLT3 );
    BtmDTempJncCU = BtmDTempACU + ( PARM(BtmDRaTh) * BtmDPwrCUT3 );
    BtmDTempJncCL = BtmDTempACL + ( PARM(BtmDRaTh) * BtmDPwrCLT3 );

    //--------------------------------------------------------------------------
    // Initialize summary signals

    OUT(BtmOtFaultSum)    = false;
    OUT(BtmOtFaultDlySum) = false;
    OUT(BtmOtAlarmSum)    = false;

    //--------------------------------------------------------------------------
    //   Check for transistor and diode junction temperature faults,
    //   delayed trips, and alarms for Phase A.

    // Phase A device over-temp trip checks
    if ( (BtmTempJncAU  > PARM(BtmJncOTThr))   ||  //Phase A Upper transistor OT trip
         (BtmTempJncAL  > PARM(BtmJncOTThr))   ||  //Phase A Lower transistor OT trip
         (BtmDTempJncAU > PARM(BtmDJncOTThr))  ||  //Phase A Upper diode OT trip
         (BtmDTempJncAL > PARM(BtmDJncOTThr)) )    //Phase A Lower diode OT trip
    {
        OUT(BtmOtFaultSum) = true;
        PUSH_DIAG( BtmAjncOtTrp );
    }

    // Phase A device over-temp delayed trip checks
    if ( (BtmTempJncAU  > PARM(BtmJncOTThDly)) ||  //Phase A Upper transistor OT trip
         (BtmTempJncAL  > PARM(BtmJncOTThDly)) ||  //Phase A Lower transistor OT trip
         (BtmDTempJncAU > PARM(BtmDJncOTThDly))||  //Phase A Upper diode OT trip
         (BtmDTempJncAL > PARM(BtmDJncOTThDly)) )  //Phase A Lower diode OT trip
    {
        OUT(BtmOtFaultDlySum) = true;
        PUSH_DIAG( BtmAjncOtDlyTrp );
    }

    // Phase A device over-temp alarm checks
    if ( (BtmTempJncAU  > PARM(BtmJncOTOn))    ||  //Phase A Upper transistor OT alarm
         (BtmTempJncAL  > PARM(BtmJncOTOn)) )      //Phase A Lower transistor OT alarm
    {
        BtmAlarmStPhsA = true;
    }
    else if ( (BtmTempJncAU  < PARM(BtmJncOTOff))   &&
              (BtmTempJncAL  < PARM(BtmJncOTOff)) )
    {
        BtmAlarmStPhsA = false;
    }

    if ( (BtmDTempJncAU > PARM(BtmDJncOTOn))   ||  //Phase A Upper diode OT alarm
         (BtmDTempJncAL > PARM(BtmDJncOTOn)) )     //Phase A Lower diode OT alarm
    {
        BtmAlarmStDphsA = true;
    }
    else if ( (BtmDTempJncAU < PARM(BtmDJncOTOff))  &&
              (BtmDTempJncAL < PARM(BtmDJncOTOff)) )
    {
        BtmAlarmStDphsA = false;
    }

    if ( (BtmAlarmStPhsA == true) || (BtmAlarmStDphsA == true) )
    {
        OUT(BtmOtAlarmSum) = true;
        PUSH_DIAG( BtmAjncOtAlm );
    }

    //--------------------------------------------------------------------------
    //   Check for transistor and diode jucntion temperature faults,
    //   delayed trips, and alarms for Phase B.

    // Phase B device over-temp trip checks
    if ( (BtmTempJncBU  > PARM(BtmJncOTThr))   ||   //Phase B Upper transistor OT trip
         (BtmTempJncBL  > PARM(BtmJncOTThr))   ||   //Phase B Lower transistor OT trip
         (BtmDTempJncBU > PARM(BtmDJncOTThr))  ||   //Phase B Upper diode OT trip
         (BtmDTempJncBL > PARM(BtmDJncOTThr)) )     //Phase B Lower diode OT trip
    {
        OUT(BtmOtFaultSum) = true;
        PUSH_DIAG( BtmBjncOtTrp );
    }

    // Phase B device over-temp delayed trip checks
    if ( (BtmTempJncBU  > PARM(BtmJncOTThDly)) ||  //Phase B Upper transistor OT trip
         (BtmTempJncBL  > PARM(BtmJncOTThDly)) ||  //Phase B Lower transistor OT trip
         (BtmDTempJncBU > PARM(BtmDJncOTThDly))||  //Phase B Upper diode OT trip
         (BtmDTempJncBL > PARM(BtmDJncOTThDly)) )  //Phase B Lower diode OT trip
    {
        OUT(BtmOtFaultDlySum) = true;
        PUSH_DIAG( BtmBjncOtDlyTrp );
    }

    // Phase B device over-temp alarm checks
    if ( (BtmTempJncBU  > PARM(BtmJncOTOn))    ||  //Phase B Upper transistor OT alarm
         (BtmTempJncBL  > PARM(BtmJncOTOn)) )      //Phase B Lower transistor OT alarm
    {
        BtmAlarmStPhsB = true;
    }
    else if ( (BtmTempJncBU  < PARM(BtmJncOTOff))   &&
              (BtmTempJncBL  < PARM(BtmJncOTOff)) )
    {
        BtmAlarmStPhsB = false;
    }

    if ( (BtmDTempJncBU > PARM(BtmDJncOTOn))   ||  //Phase B Upper diode OT alarm
         (BtmDTempJncBL > PARM(BtmDJncOTOn)) )     //Phase B Lower diode OT alarm
    {
        BtmAlarmStDphsB = true;
    }
    else if ( (BtmDTempJncBU < PARM(BtmDJncOTOff))  &&
              (BtmDTempJncBL < PARM(BtmDJncOTOff)) )
    {
        BtmAlarmStDphsB = false;
    }

    if ( (BtmAlarmStPhsB == true) || (BtmAlarmStDphsB == true) )
    {
        OUT(BtmOtAlarmSum) = true;
        PUSH_DIAG( BtmBjncOtAlm );
    }

    //--------------------------------------------------------------------------
    //   Check for transistor and diode jucntion temperature faults,
    //   delayed trips, and alarms for Phase C.

    // Phase C device over-temp trip checks
    if ( (BtmTempJncCU  > PARM(BtmJncOTThr))   ||  //Phase C Upper transistor OT trip
         (BtmTempJncCL  > PARM(BtmJncOTThr))   ||  //Phase C Lower transistor OT trip
         (BtmDTempJncCU > PARM(BtmDJncOTThr))  ||  //Phase C Upper diode OT trip
         (BtmDTempJncCL > PARM(BtmDJncOTThr)) )    //Phase C Lower diode OT trip
    {
        OUT(BtmOtFaultSum) = true;
        PUSH_DIAG( BtmCjncOtTrp );
    }

    // Phase C device over-temp delayed trip checks
    if ( (BtmTempJncCU  > PARM(BtmJncOTThDly)) ||  //Phase C Upper transistor OT trip
         (BtmTempJncCL  > PARM(BtmJncOTThDly)) ||  //Phase C Lower transistor OT trip
         (BtmDTempJncCU > PARM(BtmDJncOTThDly))||  //Phase C Upper diode OT trip
         (BtmDTempJncCL > PARM(BtmDJncOTThDly)) )  //Phase C Lower diode OT trip
    {
        OUT(BtmOtFaultDlySum) = true;
        PUSH_DIAG( BtmCjncOtDlyTrp );
    }

    // Phase C device over-temp alarm checks
    if ( (BtmTempJncCU  > PARM(BtmJncOTOn))    ||  //Phase C Upper transistor OT alarm
         (BtmTempJncCL  > PARM(BtmJncOTOn)) )      //Phase C Lower transistor OT alarm
    {
        BtmAlarmStPhsC = true;
    }
    else if ( (BtmTempJncCU  < PARM(BtmJncOTOff))   &&
              (BtmTempJncCL  < PARM(BtmJncOTOff)) )
    {
        BtmAlarmStPhsC = false;
    }

    if ( (BtmDTempJncCU > PARM(BtmDJncOTOn))   ||  //Phase C Upper diode OT alarm
         (BtmDTempJncCL > PARM(BtmDJncOTOn)) )     //Phase C Lower diode OT alarm
    {
        BtmAlarmStDphsC = true;
    }
    else if ( (BtmDTempJncCU < PARM(BtmDJncOTOff))  &&
              (BtmDTempJncCL < PARM(BtmDJncOTOff)) )
    {
        BtmAlarmStDphsC = false;
    }

    if ( (BtmAlarmStPhsC == true) || (BtmAlarmStDphsC == true) )
    {
        OUT(BtmOtAlarmSum) = true;
        PUSH_DIAG( BtmCjncOtAlm );
    }

    //--------------------------------------------------------------------------
    //   Form max transistor junction temperature

    BtmJncTempMax = BtmTempJncAU;
    if ( BtmTempJncAL > BtmJncTempMax )  BtmJncTempMax = BtmTempJncAL;
    if ( BtmTempJncBU > BtmJncTempMax )  BtmJncTempMax = BtmTempJncBU;
    if ( BtmTempJncBL > BtmJncTempMax )  BtmJncTempMax = BtmTempJncBL;
    if ( BtmTempJncCU > BtmJncTempMax )  BtmJncTempMax = BtmTempJncCU;
    if ( BtmTempJncCL > BtmJncTempMax )  BtmJncTempMax = BtmTempJncCL;

    BtmJncTempErr = BtmJncTempMax - BtmJncTemp;

    if ( BtmJncTempErr > 0.0F )
    {
        BtmJncTemp += BtmJncTempUpGn * BtmJncTempErr;
    }
    else
    {
        BtmJncTemp += BtmJncTempDnGn * BtmJncTempErr;
    }

    //--------------------------------------------------------------------------
    //   Form max Diode junction temperature

    BtmDJncTempMx = BtmDTempJncAU;
    if ( BtmDTempJncAL > BtmDJncTempMx )  BtmDJncTempMx = BtmDTempJncAL;
    if ( BtmDTempJncBU > BtmDJncTempMx )  BtmDJncTempMx = BtmDTempJncBU;
    if ( BtmDTempJncBL > BtmDJncTempMx )  BtmDJncTempMx = BtmDTempJncBL;
    if ( BtmDTempJncCU > BtmDJncTempMx )  BtmDJncTempMx = BtmDTempJncCU;
    if ( BtmDTempJncCL > BtmDJncTempMx )  BtmDJncTempMx = BtmDTempJncCL;

    BtmDJncTempErr = BtmDJncTempMx - BtmDJncTemp;

    if ( BtmDJncTempErr > 0.0 )
    {
        BtmDJncTemp += BtmJncTempUpGn * BtmDJncTempErr;
    }
    else
    {
        BtmDJncTemp += BtmJncTempDnGn * BtmDJncTempErr;
    }

    return;
}

#endif
