///////////////////////////////////////////////////////////////////////////////
//
// TITLE     Rotor bridge product level feedback and protection support
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
#include   "b0DbThrm.h"
#include   "b0BrgThrmstr.h"
#include   "y1FltCk.h"
#include   "y1Task.h"
#include   "c1BrgCroBarCtl.h"
#include   "b0DbCelTst.h"
#include   "b0DbCtl.h"

// Constants
//----------
#define VDC_HYST_FACTOR     4       // Vdc undervoltage Hysterysis band


// Externals
//----------
extern FaultIsActiveTyp FaultIsActive;
extern EpldCmdRgr      *EpldCmdP;
extern unsigned         R_SeqStBits_PackedBits;

extern cBridgeRebf      L_BridgeRebf;
extern cBridgeRebf      R_BridgeRebf;
extern cBrgThrm5pModel  R_BrgThrm5pModel;
extern cBrgThrmstr      R_BrgThrmstr;
extern cBrgCroBarCtl    R_BrgCroBarCtl;

extern unsigned         R_BrgFltHdw;
extern unsigned         BrgEnbSim;
extern float            DelTm2Rcp;
extern float            DelTm3;
extern unsigned         DiagResetCmdT1;
extern unsigned         Init5Sec;
extern float            L_VdcFbkT3;
extern unsigned         PwrDnFbkDsb;
extern unsigned         R_BrgPwrEnbT1;
extern float            R_BpTemp1;
extern float            R_BpTemp2;
extern float            R_BpTemp3;
extern float            R_BpTemp4;
extern float            R_BpTemp5;
extern float            R_BpTemp6;
extern float            R_BthtrTemp7;
extern float            R_BthtrTemp8;
extern float            R2_BthtrTemp7;
extern float            R2_BthtrTemp8;
extern unsigned         R_Running;
extern int              R_TskInfcRdy;
extern float            R_VdcFbkT2;
extern float            R_VdcFbkT3;
extern float            R_VdcQuant;
extern float            R_VdcFbk;
extern int              R_T1FramesPerT2;
extern unsigned         SimMode;
extern unsigned         Brg2TripFlt;


extern unsigned         L_MaStatusT3;
extern unsigned         S_K1StatusT3;
extern unsigned         R_OTAlSum;

extern unsigned         GridFltLatch;
extern unsigned         S_VMaxHiDet;
extern unsigned         OpnGrdDetect;
extern unsigned         IslandCBDsb;
extern unsigned         R_ZVRtrOff;
extern unsigned         MvSwGearTrpReq;
extern unsigned         RtBrgOff1Dt;


// for RotorProcessBrgT3
extern float R_BpTempMaxA;
extern float R_BpTempMaxB;
extern float R_BpTempMaxC;
extern float R_BpTempMinA;
extern float R_BpTempMinB;
extern float R_BpTempMinC;
// RotorProcessBrgT1()
extern unsigned     GmVLoPulse;
extern int          L_DbcRefLvlSel;
extern float        R_ImagSqrT1;
extern float        R_IxFbk;
extern float        R_IyFbk;
// RotorProcessBrgT2()

extern unsigned L_DtmJncHotT2;
extern cDbThrmModel L_DbThrmModel ;
extern unsigned Q1OpnReq;
extern unsigned R_SeqStopped;
extern unsigned R_InnrRegEnbT1;

// Types and Classes
//------------------
extern cDbCelTst   L_DbCelTst;

// Parameters
//-----------
//RotorProcessBrgT3()
CREATE_PARM(R_WVdcErr,      float);     // R & L Vdc discrepancy filter omega
CREATE_PARM(R_VdcErrThr,    float);     // R & L Vdc mismatch alarm threshold

CREATE_PARM(R_CroBarVdcEnbThrs, float);   // Vdc Level to power enable      (Volts)

// Variables
//----------
CREATE_PUBVAR(R_BpTempMaxA,      float);
CREATE_PUBVAR(R_BpTempMaxB,      float);
CREATE_PUBVAR(R_BpTempMaxC,      float);
CREATE_PUBVAR(R_BpTempMinA,      float);
CREATE_PUBVAR(R_BpTempMinB,      float);
CREATE_PUBVAR(R_BpTempMinC,      float);

CREATE_PUBVAR(R_BpTemp1,         float);
CREATE_PUBVAR(R_BpTemp2,         float);
CREATE_PUBVAR(R_BpTemp3,         float);
CREATE_PUBVAR(R_BpTemp4,         float);
CREATE_PUBVAR(R_BpTemp5,         float);
CREATE_PUBVAR(R_BpTemp6,         float);

CREATE_PUBVAR(R_OTAlSum,         unsigned);

//RotorProcessBrgT3()
CREATE_PUBVAR(R_VdcNomRcp,  float);     //
CREATE_PUBVAR(R_VdcErr,     float);     // R & L Vdc difference
CREATE_PUBVAR(R_VdcErrFil,  float);     // R & L Vdc filtered difference
CREATE_PUBVAR(R_VdcErrPu,   float);     // R & L Vdc difference per unit
CREATE_PUBVAR(R_VdcErrAlmFlg,   unsigned);      // R & L Vdc mismatch alarm flag

// DC link verification variables
CREATE_PUBVAR(R_VdcHyst   ,           float);
CREATE_PUBVAR(R_VdcRdy    ,           unsigned);

CREATE_PUBVAR(R_BrgPwrEnbCmd  ,       unsigned);          // desat detected
CREATE_PUBVAR(R_BrgFltRstCmd  ,       unsigned);          // desat detected


CREATE_PUBVAR(R_VdcT2Quant    ,       float);             // Vdc quantization in task2

CREATE_PUBVAR(R_BrgLossPwr,           float);
CREATE_PUBVAR(R_BtmTrPwrLossT2,      float);
CREATE_PUBVAR(R_BtmDpwrLossT2,       float);

CREATE_PUBVAR(R_CroBarFlag,           unsigned);          // (external to class)

CREATE_PUBVAR(ConvDiscnStatus,        unsigned);          // Disconnection status of main converter contactors
CREATE_PUBVAR(R_BthtrImbFlgDb,        unsigned) = false;

CREATE_PUBVAR(R_SelCBVdcLvl,          int);          // Select CB Vdc Levels
CREATE_PUBVAR(R_ForceCBOff,           unsigned);          // Force  CB Off
CREATE_PUBVAR(R_ForceCBFlt,           unsigned);          // Force  CB Fault
CREATE_PUBVAR(R_SeqStopped,           unsigned);          // Seq Stopped Status
CREATE_PUBVAR(R_CBFltCond,            unsigned);          // CB Fault Condition

CREATE_PUBVAR(R_IocMask,              unsigned);          // Mask IOC in FPGA
CREATE_PUBVAR(R_DesatMask,            unsigned);          // Mask Desat in FPGA
CREATE_PUBVAR(R_CrowBarFltActZ,       unsigned);

CREATE_PUBVAR(R_HoldPwrEnb,           unsigned);          // Power enable off until hardware faults cleared and no T2 pwr enb
CREATE_PUBVAR(CroBarVdcEnbFlg,        unsigned);          // Flag when power enable is set due to crowbar Vdc
CREATE_PUBVAR(R_HdwRstCmdFlg,         unsigned);          // Flag when power enable is set due to crowbar Vdc

// Diagnostics
//------------
CREATE_DIAG(R_BthtrDbphsImbAlm);                         // Imbalance between two DB circuits
CREATE_DIAG(R_VdcUvAlm);        // Rotor side Vdc under voltage alarm
CREATE_DIAG(R_VdcErrAlm);       // R & L Vdc mismatch alarm
CREATE_DIAG(R_VdcUnderV);       // Rotor converter DC link under voltage trip

// Data Passing
//--------------
DATA_PASS( R_BrgThrm5pModel.BtmTrPwrLoss, R_BtmTrPwrLossT2, T1A_T2, FILTERED );
DATA_PASS( R_BrgThrm5pModel.BtmDpwrLoss,  R_BtmDpwrLossT2,  T1A_T2, FILTERED );

// Unpublished Variables
//----------------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------
extern void             RotorBILPriFbks1(void);
extern void             RotorBILPriFbks2(void);
extern void             RotorBILPriFbksBgnd(void);
extern void             RotorTempConversionT1(void);
extern void             RotorClrFPGAHdw(void);
extern void             RotorClrFpgaFlt(void);
extern void             RotorInvGndFltDetectT1(void);
extern void             RotorInvGndFltDetectT3(void);
extern void             RotorSimulateAdjustments(void);
extern void             RotorTempConversionT3(void);

// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor bridge feedback processing
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorProcessFbks(void)
{

    // Process primary feedbacks
    RotorBILPriFbks1();

    // Calculate transistor and diode power losses for bridge thermal models
    // Note: bridge thermal models must run after adquiring, processing,
    // and verifying feedbacks, and before new modulator commands are synthesized.
    RotorTempConversionT1();
    R_BrgThrm5pModel.Task1();

    // Inverter ground fault detection
    RotorInvGndFltDetectT1();                // 1.5uS

    R_ForceCBOff  = ( OpnGrdDetect || IslandCBDsb );
    R_ForceCBFlt  = FAULTISACTIVE(S_SyncIOC2Flt) && !FAULTISACTIVE(R_CrowBarFlt);
    R_CBFltCond   = (!R_BrgPwrEnbT1 && !R_ZVRtrOff) || Brg2TripFlt;
    R_SeqStopped  = R_SeqStBits.R_Stopped; // WindDFIGe:(R_SeqSt==ROTOR_STOPPED)

    // Instance Class: CroBarCtl
    R_ImagSqrT1=R_IxFbk*R_IxFbk+R_IyFbk*R_IyFbk;
    R_BrgCroBarCtl.Task1();

    if (R_BrgCroBarCtl.CroBarAct)
    {
         R_BrgPwrEnbCmd = R_BrgCroBarCtl.CroBarPE;
         R_BrgFltRstCmd = R_BrgCroBarCtl.CroBarRst;
    }
    else
    {
        // H/W Gating control equals R_BrgPwrEnb from Task2
        if (R_BrgFltHdw)
        {
           R_HoldPwrEnb = true;
        }
        else if ( R_BrgPwrEnbT1 == 0)
        {
           R_HoldPwrEnb = false;
        }

        if ((R_BrgPwrEnbT1 == 0 && !CroBarVdcEnbFlg) || RtBrgOff1Dt || R_HoldPwrEnb)
        {
            R_BrgPwrEnbCmd = false;
        }
        else
        {
            R_BrgPwrEnbCmd = true;
        }


        // H/W FPGA fault reset command
        if (DiagResetCmdT1 == 0)
        {
           R_BrgFltRstCmd = false;
        }
        else
        {
           R_BrgFltRstCmd = true;
        }


        //Prepare for crobar activation..if FPGA fault, clear it
        if ( !R_SeqStopped)
        {
           if (R_BrgFltHdw)
           {
              R_BrgPwrEnbCmd = false;
              R_BrgFltRstCmd = true;
           }
           else if ( R_BrgCroBarCtl.CroBarVdc > PARM(R_CroBarVdcEnbThrs) && Brg2TripFlt && !CroBarVdcEnbFlg)
           {
              R_BrgPwrEnbCmd = true;
              R_BrgFltRstCmd = false;
              CroBarVdcEnbFlg = true;
           }
        }
        else if (CroBarVdcEnbFlg)
        {
           CroBarVdcEnbFlg = false;
        }

    }

    // After Class: Open Q1 when required
    if ( R_BrgCroBarCtl.CroBarOpnSync == true )
    {
        if (!R_CrowBarFltActZ)
        {
            Q1OpnReq = true;
        }
    }
    R_CrowBarFltActZ = FaultIsActive.R_CrowBarFlt;

    // After Class: Mask/Unmask FPGA Faults
    if(R_BrgCroBarCtl.CroBarMskIoc)
    {
        R_IocMask  = true;
    }
    else if (!R_BrgCroBarCtl.CroBarAct)
    {
        R_IocMask  = false;
    }

    if(R_BrgCroBarCtl.CroBarMskDsat)
    {
        R_DesatMask = true;
    }
    else if (!R_BrgCroBarCtl.CroBarAct && R_DesatMask)
    {
        if ( !R_HdwRstCmdFlg)
        {
            R_BrgFltRstCmd = true;
            R_HdwRstCmdFlg = true;
        }
        else
        {
            R_BrgFltRstCmd = false;
            R_DesatMask    = false;
            R_HdwRstCmdFlg = false;
        }
    }


    //  After Class: Semaphore to slower tasks
    if (R_BrgCroBarCtl.CroBarAct)
    {
        R_CroBarFlag = true;
    }



    return;

}  // void ProcessFbks()

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor bridge feedback processing in task2
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  RotorProcessFbks2()
{


     R_BrgLossPwr =               //Total Bridge Power Loss needed in task2
    ( R_BtmTrPwrLossT2 +
      R_BtmDpwrLossT2 );

    RotorBILPriFbks2();  // Process Task2 Primary BIL fdbks

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
void  RotorVerifyDCLink( void )
{

    // Check DC link ready condition

    if (R_VdcFbkT2 < PARM(R_VdcUVLvl))
    {
        R_VdcRdy = false;
    }
    else if (R_VdcFbkT2 > ( PARM(R_VdcUVLvl) + R_VdcHyst ))
    {
        R_VdcRdy = true;
    }

    // Declare undervoltage faults

    if ( R_SeqStBits.R_Stopped ||
#ifdef LOW_VOLT_CELLTEST
         R_SeqStBits.R_CellTest ||   //PWM3 Cell Test at reduced DC Volts.
#endif
         R_SeqStBits.R_PulseTest ||

         !R_TskInfcRdy )
    {
        if(!R_VdcRdy && Init5Sec && !PwrDnFbkDsb && !(L_DbCelTst.DbCapMsIp))
        {
            PUSH_DIAG( R_VdcUvAlm );
        }
    }
    else
    {
        if (!R_VdcRdy)
        {
            PUSH_DIAG( R_VdcUnderV );
        }
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
void RotorProcessBrgT1(void)
{

    // DYNAMIC BRAKE CONTROL

    // Before Class: Combinatorial Logic
    if (GridFltLatch || S_VMaxHiDet)
    {
        R_SelCBVdcLvl = static_cast<int> (cBrgCroBarCtl::CROBAR_THR_GRDFLT);
        L_DbcRefLvlSel= static_cast<int> (cDbCtl::DBC_VREF_NORMAL);
    }
    else if ( GmVLoPulse)
    {
        R_SelCBVdcLvl = static_cast<int> (cBrgCroBarCtl::CROBAR_THR_LVRT);
        L_DbcRefLvlSel= static_cast<int> (cDbCtl::DBC_VREF_NORMAL);
    }
    else if (L_DbThrmModel.DtmJncHot)
    {
        R_SelCBVdcLvl = static_cast<int> (cBrgCroBarCtl::CROBAR_THR_DBHOT);
        L_DbcRefLvlSel= static_cast<int> (cDbCtl::DBC_VREF_TRHOT);
    }
    else
    {
        R_SelCBVdcLvl = static_cast<int> (cBrgCroBarCtl::CROBAR_THR_NORMAL);
        L_DbcRefLvlSel= static_cast<int> (cDbCtl::DBC_VREF_NORMAL);
    }


    return;
} // void ProcessBrgT1()


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor bridge fault checking in task2
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorProcessBrgT2(void)
{
    RotorVerifyDCLink();             // DC link undervoltage check

    R_BrgThrm5pModel.Task2();

    return;
} // void ProcessBrgT2()



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor bridge feedback processing and fault checking in task3
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorProcessBrgT3(void)
{

    ConvDiscnStatus = (L_MaStatusT3 == false) && (S_K1StatusT3 == false);

    RotorInvGndFltDetectT3();        // Ground current fault detection
    RotorTempConversionT3();         // A/D input
    R_BrgThrmstr.Task3();           // Bridge thermistor temp conv and fault check
    R_BrgThrm5pModel.Task3();

    // Clear FPGA Fault
    // Should preceed writing to FPGA register.

    RotorClrFpgaFlt();

    // Clear Bic Hardware Fault
    // Should preceed writing to FPGA register.
    // Intended to be used by the CheckFPGAFlt() routine to reset the
    // BIC fault bit and prevent the continuous check of dual bridge faults.

    RotorClrFPGAHdw();

    // Masks FPGA faults during simulation.

    if (SimMode || !R_BrgCroBarCtl.CroBarAct)
        RotorSimulateAdjustments();


    // Check rotor Vdc feedback to Line Vdc feedback
    if (PwrDnFbkDsb)
    {
        R_VdcErr = 0.0;
    }
    else
    {
        R_VdcErr = R_VdcFbkT3-L_VdcFbkT3;
    }

    R_VdcErrFil += MIN(PARM(R_WVdcErr)*DelTm3, 1.0F) * ( R_VdcErr - R_VdcErrFil );

    R_VdcErrPu = ABS(R_VdcErrFil)*R_VdcNomRcp;

    if       ( R_VdcErrPu > PARM(R_VdcErrThr))
    {
        R_VdcErrAlmFlg = true;
    }
    else if ( R_VdcErrPu < PARM(R_VdcErrThr * 0.5))
    {
        R_VdcErrAlmFlg = false;
    }

    if(R_VdcErrAlmFlg) PUSH_DIAG(R_VdcErrAlm);

    // Populate legacy baseplate signals
    R_BpTemp1 = R_BrgThrmstr.BthtrTemp1;
    R_BpTemp2 = R_BrgThrmstr.BthtrTemp2;
    R_BpTemp3 = R_BrgThrmstr.BthtrTemp3;
    R_BpTemp4 = R_BrgThrmstr.BthtrTemp4;
    R_BpTemp5 = R_BrgThrmstr.BthtrTemp5;
    R_BpTemp6 = R_BrgThrmstr.BthtrTemp6;

    if(R_BpTemp1>R_BpTemp4)
    {
        R_BpTempMaxA=R_BpTemp1;
        R_BpTempMinA=R_BpTemp4;
    }
    else
    {
        R_BpTempMaxA=R_BpTemp4;
        R_BpTempMinA=R_BpTemp1;
    }
    if(R_BpTemp2>R_BpTemp5)
    {
        R_BpTempMaxB=R_BpTemp2;
        R_BpTempMinB=R_BpTemp5;
    }
    else
    {
        R_BpTempMaxB=R_BpTemp5;
        R_BpTempMinB=R_BpTemp2;
    }
    if(R_BpTemp3>R_BpTemp6)
    {
        R_BpTempMaxC=R_BpTemp3;
        R_BpTempMinC=R_BpTemp6;
    }
    else
    {
        R_BpTempMaxC=R_BpTemp6;
        R_BpTempMinC=R_BpTemp3;
    }
    //  Summary of rotor OT alarms in a single boolean
    // Any active bridge OT alarm will assert the rotor thermal alarm summary

    R_OTAlSum = R_BrgThrmstr.BthtrOtAlarmSum || R_BrgThrm5pModel.BtmOtAlarmSum;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor bridge feedback background processing
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void RotorProcessBrgBgnd(void)
{

    R_VdcT2Quant = R_VdcQuant / R_T1FramesPerT2;
    R_VdcHyst  = R_VdcT2Quant * VDC_HYST_FACTOR;   // (4 for PWM2, 8 for PWM3 )

    RotorBILPriFbksBgnd();      // Update VCO & shunt scalings

    return;
}
