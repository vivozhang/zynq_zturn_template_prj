///////////////////////////////////////////////////////////////////////////////
//
// TITLE     Line bridge product level feedback and protection support
//
// DESCRIPTION
//           Contains code to initialize and process H/W feedbacks.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include   "x0FrameWork.h"
// product
#include   "Product.h"
#include   "b0BridgeRebf.h"
#include   "b0BrgThrm5p.h"
#include   "b0BrgThrmstr.h"
#include   "y2Task.h"

// Constants
//----------


// Externals
//----------
extern EpldCmdRgr     *EpldCmdP;
extern cBridgeRebf     L_BridgeRebf;
extern cBrgThrm5pModel L_BrgThrm5pModel;
extern cBrgThrmstr     L_BrgThrmstr;
extern float           L_VdcFbkT2;
extern unsigned        Brg1TripFlt;
extern unsigned        BrgCardsAct;
extern unsigned        L_BrgPwrEnbT1;
extern unsigned        L_DSatDx;
extern unsigned        L_FpgaFltFb2;
extern unsigned        L_FpgaFltFbk;
extern unsigned        L_InnrRegEnb;
extern unsigned        L_RunCmd;
extern unsigned        L_ZVLineOff;
extern unsigned        SimMode;
extern unsigned        ISimMode;
extern unsigned        L_BrgFltHdw;
extern unsigned        L_OTAlSum;
extern unsigned        LnBrgOff1Dt;


// Types and Classes
//------------------

// Parameters
//-----------

// Variables
//----------
CREATE_PUBVAR(L_OTAlSum      ,   unsigned);
CREATE_PUBVAR(L_FpgaFltFbk   ,   unsigned);
CREATE_PUBVAR(L_IocRtryLim   ,   float);    // instantaneous over-current retry limit
CREATE_PUBVAR(L_IocRtryRto   ,   float);    // var for IOC retry down timer ratio
CREATE_PUBVAR(L_IocThrs      ,   float);    // task1 instantaneous over-current flt chk
CREATE_PUBVAR(L_VdcNomCal    ,   float);    // nominal dc link voltage
CREATE_PUBVAR(L_VdcNomRcp    ,   float);    // nominal dc link voltage reciprocal
CREATE_PUBVAR(L_VlnInRcp     ,   float);    // Rcp of line to neut input volt magnitude
CREATE_PUBVAR(L_VllInRcp     ,   float);    // Rcp of line to line input volt magnitude
CREATE_PUBVAR(L_IocFbkEnb    ,   unsigned); // instantaneous over-current fbk enable
CREATE_PUBVAR(L_IocInstEnb   ,   unsigned); // Ioc instantaneous enable
CREATE_PUBVAR(L_IocMask      ,   unsigned); // Ioc Mask to allow Ioc retry

CREATE_PUBVAR(L_BrgPwrEnbCmd ,   unsigned); // bridge power enable to fpga

CREATE_PUBVAR(L_LineFrqRcp   ,   float);
CREATE_PUBVAR(L_CTotx        ,   float);
CREATE_PUBVAR(L_BtmTrPwrLossT2,  float);
CREATE_PUBVAR(L_BtmDpwrLossT2,   float);
CREATE_PUBVAR(L_BtmTrPwrLossT3,  float);
CREATE_PUBVAR(L_BtmDpwrLossT3,   float);
CREATE_PUBVAR(L_BpTemp1       ,  float);    // Legacy line bridge
CREATE_PUBVAR(L_BpTemp2       ,  float);    // baseplate signals (degC)
CREATE_PUBVAR(L_BpTemp3       ,  float);
CREATE_PUBVAR(L_BpTemp4       ,  float);
CREATE_PUBVAR(L_BpTemp5       ,  float);
CREATE_PUBVAR(L_BpTemp6       ,  float);
CREATE_PUBVAR(L_BrgLossPwr    ,  float);

// Data Passing
//--------------
DATA_PASS( L_BrgThrm5pModel.BtmTrPwrLoss, L_BtmTrPwrLossT2, T1_T2, FILTERED );
DATA_PASS( L_BrgThrm5pModel.BtmDpwrLoss,  L_BtmDpwrLossT2,  T1_T2, FILTERED );
DATA_PASS( L_BtmTrPwrLossT2,              L_BtmTrPwrLossT3, T2_T3, FILTERED );
DATA_PASS( L_BtmDpwrLossT2,               L_BtmDpwrLossT3,  T2_T3, FILTERED );

// Diagnostics
//--------------------
CREATE_DIAG(L_IocThrsHi);           // IOC high threshold

// Unpublished Variables
//----------------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------

extern void  BILPriFbks1();
extern void  BILPriFbks2();
extern void  BILPriFbks3();
extern void  BILPriFbksBgnd();
extern void  LineTempConversionT1();
extern void  CommonFaultDetectionBIL();
extern void  DynamicBrakeFaultSequencer();
extern void  FaultChkSourceBIL();
extern void  InvGndFltDetectT1();
extern void  InvGndFltDetectT3();
extern void  LineClrFpgaFlt();
extern void  LineClrFpgaHdw();
extern void  LineProcessFaultFbks();
extern void  LineRmsPhaseCurrentCalcT1(void);
extern void  LineRmsPhaseCurrentCalcT3(void);
extern void  LineSimulateAdjustments();
extern void  LineTempConversionT3();
extern void  ShuntFltChk();
extern void  TocDetection();
extern void  AclfuTocDetection();


// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line bridge feedback processing
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ProcessFbks(void)
{


    // Process primary feedbacks
    BILPriFbks1();
    LineRmsPhaseCurrentCalcT1();



    // Digital fault and status bit sampling and tests
    //   Digital status bits are not latched at Task1 interrupt.  Instead
    //   the latching depends on the internal ASIC clock.  They should be
    //   read early in Task1, but after DC overvoltage fault has been set.
    //   They should be read immediately after ProcessFbks.


    L_FpgaFltFbk = L_BrgFltHdw;



    // Process feedbacks for fault detection
    ShuntFltChk();

    LineProcessFaultFbks();

    // Calculate transistor and diode power losses for bridge thermal model
    // Note: bridge thermal model must run after adquiring, processing,
    // and verifying feedbacks, and before new modulator commands are synthesized.
    LineTempConversionT1();
    L_BrgThrm5pModel.Task1();

    // Inverter ground fault detection
    InvGndFltDetectT1();                // 1.5uS

    return;


}  // void ProcessFbks()



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line bridge feedback processing in task2
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  ProcessFbks2()
{

    BILPriFbks2();  // Process Task2 Primary BIL fdbks
    L_BrgThrm5pModel.Task2();

    return;

}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Dc link under-voltage protection based on line bridge feedback
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  VerifyDCLink( void )
{

    if ( (L_InnrRegEnb || L_ZVLineOff) && (L_VdcFbkT2 < PARM(L_VdcUVLvl)) )
    {
        PUSH_DIAG(L_VdcUnderV);
    }

    return;

}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Dynamic brake support and forming power-enable-commands.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
 void ProcessBrgT1(void)
{

    DynamicBrakeFaultSequencer();   // Generate L_DbGateDsb signal


    // H/W Bridge ABC Gating control equals L_BrgPwrEnb from Task2
    if ( (L_BrgPwrEnbT1 == 1) && !L_FpgaFltFb2 &&  !Brg1TripFlt && !LnBrgOff1Dt)
    {
       L_BrgPwrEnbCmd = true;
    }
    else
    {
       L_BrgPwrEnbCmd = false;
    }

} // void ProcessBrgT1()



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line bridge fault checking in task2
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineProcessBrgT2(void)
{
    VerifyDCLink();             // DC link undervoltage check

    FaultChkSourceBIL();

    return;

} // void LineProcessBrgT2()



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line bridge feedback processing and fault checking in task3
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ProcessBrgT3(void)
{
    L_BrgLossPwr = L_BtmTrPwrLossT3 + L_BtmDpwrLossT3;

    BILPriFbks3();               // Process Task3 Primary BIL fdbks
    LineRmsPhaseCurrentCalcT3(); // Task3 portion of rms phase current calcs of testing
    InvGndFltDetectT3();         // Ground current fault detection
    TocDetection();              // Timed overcurrent detection
    AclfuTocDetection();              // Timed overcurrent detection
    LineTempConversionT3();      // A/D input
    L_BrgThrmstr.Task3();        // Bridge thermistor temp conv and fault check
    L_BrgThrm5pModel.Task3();    // Line bridge thermal model instance
    CommonFaultDetectionBIL();   // common BIL fault detection--task 3


    // Clear FPGA Fault
    // Should preceed writing to FPGA register.

    LineClrFpgaFlt();

    // Clear Bic Hardware Fault
    // Should preceed writing to Asic register.
    // Intended to be used by the CheckAsicFlt() routine to reset the
    // BIC fault bit and prevent the continuous check of dual bridge faults.

    LineClrFpgaHdw();


    // Masks FPGA faults during simulation.

    LineSimulateAdjustments();

    // Populate legacy baseplate signals
    L_BpTemp1 = L_BrgThrmstr.BthtrTemp1;
    L_BpTemp2 = L_BrgThrmstr.BthtrTemp2;
    L_BpTemp3 = L_BrgThrmstr.BthtrTemp3;
    L_BpTemp4 = L_BrgThrmstr.BthtrTemp4;
    L_BpTemp5 = L_BrgThrmstr.BthtrTemp5;
    L_BpTemp6 = L_BrgThrmstr.BthtrTemp6;

    //  Summary of rotor OT alarms in a single boolean
    // Any active bridge OT alarm will assert the rotor thermal alarm summary

    L_OTAlSum = L_BrgThrmstr.BthtrOtAlarmSum || L_BrgThrm5pModel.BtmOtAlarmSum;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line bridge feedback background processing
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void LineProcessBrgBgnd(void)
{
    //L_Vdc_Quant = PARM(L_VdcVcoScl) * DelTm2Rcp;
    //L_Vdc_Hyst  = L_Vdc_Quant * L_VDC_HYST_FACTOR;   // (4 for PWM2, 8 for PWM3 )

    BILPriFbksBgnd();      // Update VCO & shunt scalings


    if ( !L_RunCmd && !ISimMode)// Test IOC in Xsim Mode
    {
        L_IocRtryLim = 0;
        L_IocFbkEnb  = false;
        L_IocMask    = false;    // Disables IOC Mask in FPGA (for IOC Retry)
        L_IocInstEnb = true;     // Ioc instantaneous enable
        L_IocRtryRto = PARM(L_IocDwnRtio);
    }
    else if ( PARM(L_IocRtryReq) >= PARM(L_IocRtryMax) )
    {
        L_IocRtryLim = PARM(L_IocRtryMax);
        L_IocFbkEnb  = false;
        L_IocMask    = true;     // Enables IOC Mask in FPGA (for IOC Retry)
        L_IocInstEnb = false;    // Ioc instantaneous disable
        L_IocRtryRto = PARM(L_IocDwnRtio);
    }
    else if ( PARM(L_IocRtryReq) <= 0.0F )
    {
        L_IocRtryLim = 0.0;
        L_IocFbkEnb  = true;
        L_IocMask    = false;    // Disables IOC Mask in FPGA (for IOC Retry)
        L_IocInstEnb = false;    // Ioc instantaneous disable
        L_IocRtryRto = PARM(L_IocDwnRtio);
    }
    else
    {
        L_IocRtryLim = PARM(L_IocRtryReq);
        L_IocFbkEnb  = false;
        L_IocMask    = true;     // Enables IOC Mask in FPGA (for IOC Retry)
        L_IocInstEnb = false;    // Ioc instantaneous disable
        L_IocRtryRto = PARM(L_IocDwnRtio);
    }



    if ( PARM(L_IocThrsReq) > 2.698F*SQRT2*PARM(L_MaxCur) )
    {
        L_IocThrs = 2.5F*SQRT2*PARM(L_IRat);
        PUSH_DIAG(L_IocThrsHi);
    }
    else
    {
        L_IocThrs = PARM(L_IocThrsReq);
    }

    L_CTotx = PARM(L_CTot);

    return;
}
