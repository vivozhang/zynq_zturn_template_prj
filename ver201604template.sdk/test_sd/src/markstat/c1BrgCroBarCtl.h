
///////////////////////////////////////////////////////////////////////////////
//
// TITLE: Rotor-side Bridge-Crowbar Control
//
// DESCRIPTION:
// Defines interfaces for the Rotor-side Bridge-Crowbar Control class.
//
// COPYRIGHT:   Copyright (c) 2012
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      c1BrgCroBarCtl.vsd
//
///////////////////////////////////////////////////////////////////////////////
#ifndef    C1_BRG_CROBAR_CTL_H
#define    C1_BRG_CROBAR_CTL_H

// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
#include "x0BridgeInterface.h"
// product
#include "Product.h"


// Compile-time Switches
// ---------------------
//#define CROBAR_COMPILE_XXXXX                             // <none, as yet>

// Constants
//----------
#define CROWBAR_OFF     0
#define CROWBAR_NORMAL  1
#define CROWBAR_FAULT   2
#define CROWBAR_COOL    3


// Function Prototypes
//--------------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <class description>
//
///////////////////////////////////////////////////////////////////////////////
class cBrgCroBarCtl
{

//-----------------------------------------------------------------------------
public:

    // Types
    // -----
    enum CroBarThrSelTyp                 // CroBar dynamic threshold selection
    {
        CROBAR_THR_NORMAL     = 0,
        CROBAR_THR_GRDFLT     = 1,
        CROBAR_THR_LVRT       = 2,
        CROBAR_THR_DBHOT      = 3,
    };
struct BrgCroBarCtlInTyp
{
    const float&        VdcFbk;                     //  1 - Vdc Feedback
    const float&        VacFbk;                     //  2 - Vac Feedback
    const int&          SelVdcLvl;                  //  3 - Select Vdc Levels
    const unsigned&     ForceOff;                   //  4 - Force CB Off
    const unsigned&     ForceFault;                 //  5 - Force CB Fault
    const unsigned&     BrgFltHdw;                  //  6 - Hardware Fault Status
    const unsigned&     Stopped;                    //  7 - Stopped Sequencer Status
    const unsigned&     BrgIocFlt;                  //  8 - FPGA IOC Status (Brg=1|2,Phs=A|B|C)
    const unsigned&     FaultCond;                  //  9 - CB Fault Condition
    const float&        ImagSqr;                    // 10 - Current magnitude input
    const unsigned&     GmLineVLo;                  // 11 - Gm voltage low flag
    const unsigned&     GmNSeqHi;                   // 12 - Gm negative sequence voltage high flag
    const float&        VdcRegRef;                // 13 - Dynamic DB Vdc reference input
};

struct BrgCroBarCtlInsTyp
{
    const float&        DelTm1;                     //  1 - Task1 delta time (seconds)
    const unsigned&     TestMode;                   //  2 - Test Mode word (enumerated)
    const float&        VdcDbMax;                   //  3 - Vdc Dynamic Brake threshold max
    const float&        VllrmsNom;                  //  4 - Vac (L-L,RMS) Nominal
    const unsigned&     DbEnb;                      //  5 - Enable DB
    const float&        VdcRefMax;                  //  7 - Maximum dc link voltage reference
    const float&        VdcOvLvl;                   //  8 - DC link over-voltage level
};

    // Methods
    //--------
     cBrgCroBarCtl( BrgCroBarCtlInTyp InList, BrgCroBarCtlInsTyp InsList );
    ~cBrgCroBarCtl( void );

    int  RunRules( void );
    void Task1( void );

    // Output Signals
    //---------------
    CREATE_PUBVAR(CroBarSt,            int);      // CroBar State Machine state variable
    CREATE_PUBVAR(CroBarVdc,         float);      // CroBar Vdc FIR-filter output

    CREATE_PUBVAR(CroBarAct,      unsigned);      // CroBar Active Status
    CREATE_PUBVAR(CroBarUpDn,     unsigned);      // CroBar Req Up/Down
    CREATE_PUBVAR(CroBarFlt,      unsigned);      // CroBar Fault detected
    CREATE_PUBVAR(CroBarMskIoc,   unsigned);      // CroBar Req to FPGA to Mask IOC
    CREATE_PUBVAR(CroBarMskDsat,  unsigned);      // CroBar Req to FPGA to Mask Desat
    CREATE_PUBVAR(CroBarOpnSync,  unsigned);      // CroBar Req to Open Sync Contactor
    CREATE_PUBVAR(CroBarPE,       unsigned);      // CroBar Req Power Enable
    CREATE_PUBVAR(CroBarRst,      unsigned);      // CroBar Reset Faults

    CREATE_PUBVAR(CroBarVdcLvlOn,  float);  // Vdc level CroBar ON  ACTIVE (Volts)
    CREATE_PUBVAR(CroBarVdcLvlOff, float);  // Vdc level CroBar OFF ACTIVE (Volts)

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;


    // External Interfaces
    //--------------------
    BrgCroBarCtlInTyp         In;
    BrgCroBarCtlInsTyp        Ins;

    // Types
    //------

    // Constants
    //----------
    static const float CroBar_VdcMin;
    static const float CroBar_VdcMax;

    // Methods
    //--------
    void    StateMachine();

    // Parameters
    //-----------
    CREATE_PARM(CroBarVdcFGn0,     float);  // Vdc FIR filter gain for crowbar
    CREATE_PARM(CroBarVdcFGn1,     float);  // Vdc FIR filter gain for crowbar

    CREATE_PARM(CroBarVdcLvlOfNrm, float);  // Vdc Level CroBar OFF NORMAL    (Volts)
    CREATE_PARM(CroBarVdcLvlOnNrm, float);  // Vdc Level CroBar ON  NORMAL    (Volts)
    CREATE_PARM(CroBarVdcLvlOfGF,  float);  // Vdc Level CroBar OFF threshold during and after a grid fault (volts)
    CREATE_PARM(CroBarVdcLvlOnGF,  float);  // Vdc Level CroBar ON threshold during and after a grid fault (volts)
    CREATE_PARM(CroBarVdcLvlOnHt,  float);  // Vdc Level CroBar ON when DB is hot
    CREATE_PARM(CroBarVdcLvlOfHt,  float);  // Vdc Level CroBar OFF when DB is hot
    CREATE_PARM(CroBarVdcLvlOfLv,  float);  // Vdc Level CroBar OFF threshold during first few miliseconds of an LVRT event (Volts)
    CREATE_PARM(CroBarVdcLvlOnLv,  float);  // Vdc Level CroBar ON  threshold during first few miliseconds of an LVRT event (Volts)
    CREATE_PARM(CroBarVdcLvlFault, float);  // Vdc Level CroBar FAULT         (Volts)

    CREATE_PARM(CroBarPer,         float);  // Crowbar toggle period   (sec)
    CREATE_PARM(CroBarTm,          float);  // Crowbar action duration (sec)
    CREATE_PARM(CroBarLkTm,        float);  // Ccrowbar cool-down time (sec)

    CREATE_PARM(CroBarIocMskDyEn,  unsigned);   // Enable dynamic Crowbar Ioc mask Vdc level
    CREATE_PARM(CroBarIocMskVdc,   float);  // Rotor Ioc mask vdc level
    CREATE_PARM(CroBarIocMskMgDc,  float);  // Rotor Ioc mask vdc margin
    CREATE_PARM(CroBarIocMskVac,   float);  // Rotor Ioc mask Vac level

    CREATE_PARM(CroBarIocMskTm,    float);  // Rotor Ioc mask duration (sec)
    CREATE_PARM(CroBarImagFilW,    float);  // Crowbar off current filter omega
    CREATE_PARM(CroBarImagOfThr,   float);  // Crowbar off current
    // Published Variables
    //--------------------
    CREATE_PUBVAR(CroBarTmr ,      float);  // crowbar timer for toggling
    CREATE_PUBVAR(CroBarTmr2 ,     float);  // crowbar timer for elapsed time

    CREATE_PUBVAR(CroBarIocMaskTmr,float);  // crowbar timer for IOC Mask duration
    CREATE_PUBVAR(CroBarIocMskVdc, float);  // final crowbar Ioc mask Vdc level

    CREATE_PUBVAR(CroBarImagSqrDel,float);  // crowbar filtered rotor current
    CREATE_PUBVAR(CroBarImagSqrFil,float);  // crowbar filtered rotor current
    CREATE_PUBVAR(CroBarImagFilGn, float);  // crowbar filtered rotor current
    CREATE_PUBVAR(CroBarImagOffSq, float);  // Crowbar squared current threshold for turn off permissive
    CREATE_PUBVAR(CroBarOffPrm,    unsigned);   // Crowbar turn off permissive
    // Un-published Variables
    //-----------------------
    float   CroBarVdcFbkZ1;                 // Vdc Feedback*Z^-1 for FIR Filter

};

#endif  // C1_BRG_CROBAR_CTL_H


