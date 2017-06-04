///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Bridge current and voltage feedback offsets
//
// DESCRIPTION:
//      This file defines the bridge feedback offset inheritance hierarchy. This
//      hierarchy is headed by a abstract base class that derives into two concrete
//      classes, the line and rotor feedback offset calculation clases.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      List Team Server locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef    B0_BRG_FBK_OFFS_H
#define    B0_BRG_FBK_OFFS_H

// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"


// Constants
//----------


// Function Prototypes
//--------------------


// Types and Classes
//------------------

struct BrgHdwOffsTyp
{
    float          Vax;     // this three voltages can be
    float          Vbx;     // line-to-line or line-to-neutral
    float          Vcx;
    float          Ia;      // phase currents
    float          Ib;
    float          Ic;
};

struct BrgHdwOffsInTyp
{
    const unsigned&     IOffsNullEnb;           //  1 - I offset null enable
    const unsigned&     VOffsNullEnb;           //  2 - V offset null enable - Only Rotor class
    const unsigned&     IOffsWSlowSw;           //  3 - Slow I offset null omega switch in task2 - Only Rotor class
    const unsigned&     VOffsWFastSw;           //  4 - V offset fast omega and time switch in task2 - Only Rotor class
    const unsigned&     InnrRegEnb;             //  5 - Inner regulator enable - Only Line class
    const unsigned&     PwrDnFbkDsb;            //  6 - Power down feedback disable
};

struct BrgHdwOffsInsTyp
{
    const float&        DelTm1;                 //  1 - Task1 delta time - Only Rotor class
    const float&        DelTm2;                 //  2 - Task2 delta time - Only Line class
    const float&        DelTm3;                 //  3 - Task3 delta time  - Only Line class
    const float&        DelTm1Rcp;              //  4 - One over task1 delta time  - Only Rotor class
    const SysTm_Typ&    SysTm2;                 //  5 - System time in task2 - Only Rotor class
	const char*         Uniquifier;             //  6 - Class instance uniquifier string
    const unsigned&     VOffsAccDsb;            //  7 - Disable voltage offset accumulation
};



////////////////////////////////////////////////////////////////////////////////
//
//  Bridge feedback offset- Abstract class
//
////////////////////////////////////////////////////////////////////////////////
class cBrgFbkOffs
{

//-----------------------------------------------------------------------------
public:
    cBrgFbkOffs(void);
    virtual ~cBrgFbkOffs(void);

    // Methods (pure virtual)
    //-----------------------
    virtual void Task1(const BrgHdwOffsTyp& SignalIn, BrgHdwOffsTyp& OffsCalc) = 0;
    virtual void Task2(void) = 0;
    virtual void Task3(void) = 0;
    virtual void RunRules(float VFullScale, float IFullScale) = 0;


//-----------------------------------------------------------------------------
private:


};



////////////////////////////////////////////////////////////////////////////////
//
//  Rotor Bridge feedback offset- Derived class
//
////////////////////////////////////////////////////////////////////////////////
class cRotorBrgFbkOffs : public cBrgFbkOffs
{

//-----------------------------------------------------------------------------
public:
    cRotorBrgFbkOffs( BrgHdwOffsInTyp InList, BrgHdwOffsInsTyp InsList );
   ~cRotorBrgFbkOffs(void);

    // Methods
    //--------
    virtual void Task1(const BrgHdwOffsTyp& SignalIn, BrgHdwOffsTyp& OffsCalc);
    virtual void Task2(void);
    virtual void Task3(void);
    virtual void RunRules(float VFullScale, float IFullScale);

    // Output signals to be written via Out structure
    //-----------------------------------------------
    CREATE_PUBVAR(VOffsVld,         unsigned);  // V offset is valid
    CREATE_PUBVAR(WIOffsFast,           float);  // Output parameter WIOffsFst for P1sseq.cpp


//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;

    // External Interfaces
    //--------------------
    BrgHdwOffsInTyp     In;
    BrgHdwOffsInsTyp    Ins;

    // Diagnostics
    //------------
    CREATE_DIAG(IaOffsTrp);
    CREATE_DIAG(IbOffsTrp);
    CREATE_DIAG(IcOffsTrp);
    CREATE_DIAG(VaxOffsTrp);
    CREATE_DIAG(VbxOffsTrp);
    CREATE_DIAG(VcxOffsTrp);

    // Parameters
    //-----------
    CREATE_PARM(IaOffs,                 float);     // current fbk static offset, amps
    CREATE_PARM(IbOffs,                 float);
    CREATE_PARM(IcOffs,                 float);

    CREATE_PARM(IOffsThr,               float);     // Off-line offset trip thresholds
    CREATE_PARM(VOffsThr,               float);

    CREATE_PARM(VaxOffs,                float);     // volatge fbk static offset, volts
    CREATE_PARM(VbxOffs,                float);
    CREATE_PARM(VcxOffs,                float);

    CREATE_PARM(WIOffsSlw,              float);     // I offset calculation filters
    CREATE_PARM(WIOffsFst,              float);

    CREATE_PARM(WVOffsFst,              float);     // V offset calculation filters
    CREATE_PARM(WVOffsFil,              float);

    CREATE_PARM(VOffsFstTm,             float);     // Fast run time for V offset calculation
    CREATE_PARM(VOffsRnTm,              float);     // Normal run time for V offset calculation

    CREATE_PARM(VOffsVlTm,              float);     // Off-line V offset result valid time, seconds.

    // Published Variables
    //--------------------
    CREATE_PUBVAR(IOffsFltChk,          unsigned);  // I offset faults check
    CREATE_PUBVAR(VOffsFltChk,          unsigned);  // V offset faults check

    CREATE_PUBVAR(VOffsFilCnt,          int);       // Voltage offset pre-filter counter
    CREATE_PUBVAR(VOffsAccCnt,          int);       // Voltage offset accumulation counter
    CREATE_PUBVAR(VOffsCntVld,          unsigned);  // Voltage offset counters reached valid values
    CREATE_PUBVAR(VOffsCntVldT2,        unsigned);  // Task2 version of VOffsCntVld

    CREATE_PUBVAR(VOffsFilN,            int);       // Voltage offset pre-filter loops in task2
    CREATE_PUBVAR(VOffsFilNT1,          int);       // Task1 version of VOffsFilN

    CREATE_PUBVAR(VOffsAccN,            int);       // Voltage offset accumulation loops in task2
    CREATE_PUBVAR(VOffsAccNT1,          int);       // Task1 version of VOffsAccN

    CREATE_PUBVAR(IOffsGn,              float);     // Gain for I offset calculation
    CREATE_PUBVAR(IOffsGnT1,            float);     // Task1 version of IOffsGn

    CREATE_PUBVAR(VOffsGn,              float);     // Gain for V offset calculation
    CREATE_PUBVAR(VOffsGnT1,            float);     // Task1 version of VOffsGn

    CREATE_PUBVAR(VaxOffsFil,           float);     // Filtered voltages
    CREATE_PUBVAR(VbxOffsFil,           float);
    CREATE_PUBVAR(VcxOffsFil,           float);

    CREATE_PUBVAR(VaxOffsAcc,           float);     // Accumulator of filtered voltages
    CREATE_PUBVAR(VbxOffsAcc,           float);
    CREATE_PUBVAR(VcxOffsAcc,           float);

    CREATE_PUBVAR(VaxOffsOut,           float);     // Voltage offset output signals
    CREATE_PUBVAR(VbxOffsOut,           float);
    CREATE_PUBVAR(VcxOffsOut,           float);

    CREATE_PUBVAR(VaxOffsOutT2,         float);     // Voltage offset output signals task2
    CREATE_PUBVAR(VbxOffsOutT2,         float);
    CREATE_PUBVAR(VcxOffsOutT2,         float);

    CREATE_PUBVAR(IaOffsOut,            float);     // Current offset output signals
    CREATE_PUBVAR(IbOffsOut,            float);
    CREATE_PUBVAR(IcOffsOut,            float);

    CREATE_PUBVAR(IaOffsOutT2,          float);     // Current offset output signals task2
    CREATE_PUBVAR(IbOffsOutT2,          float);
    CREATE_PUBVAR(IcOffsOutT2,          float);

    // Unpublished Variables
    //----------------------
    SysTm_Typ   VOffsTm;                            // Voltage offset time keeper
    float       IOffsSlwGn;                         // Slow gain for I offset calculation
    float       IOffsFstGn;                         // Fast gain for I offset calculation
    float       VOffsNrmGn;                         // Normal gain for V offset calculation
    float       VOffsFstGn;                         // Fast gain for V offset calculation
    float       VOffsNrmFilN;                       // Voltage offset pre-filter loops with normal gain
    float       VOffsFstFilN;                       // Voltage offset pre-filter loops with fast gain
    float       VOffsNrmAccN;                       // Voltage offset accumulation loops with normal run time
    float       VOffsFstAccN;                       // Voltage offset accumulation loops with fast run time

    double      IaOffsFilSt;                        // task1 current filter state variables
    double      IbOffsFilSt;
    double      IcOffsFilSt;

    double      VaxOffsFilSt;                       // task1 current filter state variables
    double      VbxOffsFilSt;
    double      VcxOffsFilSt;

    // Data Passing
    //-------------
    DATA_PASS(VOffsFilN,        VOffsFilNT1,        T2_T1, UNFILTERED);
    DATA_PASS(VOffsAccN,        VOffsAccNT1,        T2_T1, UNFILTERED);
    DATA_PASS(VOffsCntVld,      VOffsCntVldT2,      T1_T2, UNFILTERED);
    DATA_PASS(IOffsGn,          IOffsGnT1,          T2_T1, UNFILTERED);
    DATA_PASS(VOffsGn,          VOffsGnT1,          T2_T1, UNFILTERED);

    DATA_PASS(IaOffsOut,        IaOffsOutT2,        T1_T2, FILTERED);
    DATA_PASS(IbOffsOut,        IbOffsOutT2,        T1_T2, FILTERED);
    DATA_PASS(IcOffsOut,        IcOffsOutT2,        T1_T2, FILTERED);

    DATA_PASS(VaxOffsOut,       VaxOffsOutT2,       T1_T2, FILTERED);
    DATA_PASS(VbxOffsOut,       VbxOffsOutT2,       T1_T2, FILTERED);
    DATA_PASS(VcxOffsOut,       VcxOffsOutT2,       T1_T2, FILTERED);

};



////////////////////////////////////////////////////////////////////////////////
//
//  Line Bridge feedback offset- Derived class
//
////////////////////////////////////////////////////////////////////////////////
class cLineBrgFbkOffs : public cBrgFbkOffs
{

//-----------------------------------------------------------------------------
public:
    cLineBrgFbkOffs( BrgHdwOffsInTyp InList, BrgHdwOffsInsTyp InsList );
   ~cLineBrgFbkOffs(void);

    // Methods
    //--------
    virtual void Task1(const BrgHdwOffsTyp& SignalIn, BrgHdwOffsTyp& OffsCalc);
    virtual void Task2(void);
    virtual void Task3(void);
    virtual void RunRules(float VFullScale, float IFullScale);

    // Output signals to be written via Out structure
    //-----------------------------------------------


//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cMarkStatDiag;
    friend class cDataPasser;

    BrgHdwOffsInTyp     In;
    BrgHdwOffsInsTyp    Ins;

    // Diagnostics
    //------------
    CREATE_DIAG(IaOffsTrp);
    CREATE_DIAG(IbOffsTrp);
    CREATE_DIAG(IcOffsTrp);
    CREATE_DIAG(VaxOffsTrp);
    CREATE_DIAG(VbxOffsTrp);
    CREATE_DIAG(VcxOffsTrp);

    // Parameters
    //-----------
    CREATE_PARM(IaOffs,             float);     // PhA current fbk static offset, amps
    CREATE_PARM(IbOffs,             float);     // PhB current fbk static offset, amps
    CREATE_PARM(IcOffs,             float);     // PhC current fbk static offset, amps

    CREATE_PARM(IOffsThr,           float);     // Off-line current offset trip threshold
    CREATE_PARM(VOffsThr,           float);     // Off-line voltage offset trip threshold

    CREATE_PARM(VaxOffs,            float);     // Phs A-X volatge fbk static offset, volts
    CREATE_PARM(VbxOffs,            float);     // Phs B-X volatge fbk static offset, volts
    CREATE_PARM(VcxOffs,            float);     // Phs C-X volatge fbk static offset, volts

    CREATE_PARM(WIOffsFil,          float);     // Filter omega for I offset calculation
    CREATE_PARM(WVOffsT2,           float);     // Filter omega for V in task2
    CREATE_PARM(WVOffsFil,          float);     // Filter omega for V offset calculation

    CREATE_PARM(VOffsInh,           unsigned);  // Vax/Vbx/Vcx running voltage offset inhibit
    CREATE_PARM(IOffsInh,           unsigned);  // Off-line current offset inhibit

    // Published Variables
    //--------------------
    CREATE_PUBVAR(IaOffsInt,        float);     // I offset integrator
    CREATE_PUBVAR(IbOffsInt,        float);
    CREATE_PUBVAR(IcOffsInt,        float);

    CREATE_PUBVAR(IaOffsDyn,        float);     // I dynamic offset
    CREATE_PUBVAR(IbOffsDyn,        float);
    CREATE_PUBVAR(IcOffsDyn,        float);

    CREATE_PUBVAR(VaxOffsInt,       float);     // V offset integrator
    CREATE_PUBVAR(VbxOffsInt,       float);
    CREATE_PUBVAR(VcxOffsInt,       float);

    CREATE_PUBVAR(VaxOffsDyn,       float);     // V dynamic offset
    CREATE_PUBVAR(VbxOffsDyn,       float);
    CREATE_PUBVAR(VcxOffsDyn,       float);

    CREATE_PUBVAR(IaInpOffs,        float);     // I input signal in task1
    CREATE_PUBVAR(IbInpOffs,        float);
    CREATE_PUBVAR(IcInpOffs,        float);

    CREATE_PUBVAR(IaInpOffsT2,      float);     // I phA input signal in task2
    CREATE_PUBVAR(IbInpOffsT2,      float);
    CREATE_PUBVAR(IcInpOffsT2,      float);

    CREATE_PUBVAR(VaxInpOffs,       float);     // V input signal in task1
    CREATE_PUBVAR(VbxInpOffs,       float);
    CREATE_PUBVAR(VcxInpOffs,       float);

    CREATE_PUBVAR(VaxInpOffsT2,     float);     // V input signal in task2
    CREATE_PUBVAR(VbxInpOffsT2,     float);
    CREATE_PUBVAR(VcxInpOffsT2,     float);

    CREATE_PUBVAR(VaxOffsFil,       float);     // V filtered in task2
    CREATE_PUBVAR(VbxOffsFil,       float);
    CREATE_PUBVAR(VcxOffsFil,       float);

    CREATE_PUBVAR(VaxOffsFilT3,     float);     // V filtered in task3
    CREATE_PUBVAR(VbxOffsFilT3,     float);
    CREATE_PUBVAR(VcxOffsFilT3,     float);

    CREATE_PUBVAR(IaOutOffs,        float);     // I output signal in task2
    CREATE_PUBVAR(IbOutOffs,        float);
    CREATE_PUBVAR(IcOutOffs,        float);

    CREATE_PUBVAR(IaOutOffsT1,      float);     // I output signal in task1
    CREATE_PUBVAR(IbOutOffsT1,      float);
    CREATE_PUBVAR(IcOutOffsT1,      float);

    CREATE_PUBVAR(VaxOffsOut,       float);     // V output signal in task3
    CREATE_PUBVAR(VbxOffsOut,       float);
    CREATE_PUBVAR(VcxOffsOut,       float);

    CREATE_PUBVAR(VaxOffsOutT1,     float);     // V output signal in task1
    CREATE_PUBVAR(VbxOffsOutT1,     float);
    CREATE_PUBVAR(VcxOffsOutT1,     float);

    // Unpublished Variables
    //----------------------

    float IOffsFilGn;
    float VOffsFilGn;
    float VOffsT2Gn;

    // Data Passing
    //-------------
    DATA_PASS(IaInpOffs,  IaInpOffsT2, T1_T2, FILTERED);
    DATA_PASS(IbInpOffs,  IbInpOffsT2, T1_T2, FILTERED);
    DATA_PASS(IcInpOffs,  IcInpOffsT2, T1_T2, FILTERED);

    DATA_PASS(VaxInpOffs, VaxInpOffsT2,T1_T2, FILTERED);
    DATA_PASS(VbxInpOffs, VbxInpOffsT2,T1_T2, FILTERED);
    DATA_PASS(VcxInpOffs, VcxInpOffsT2,T1_T2, FILTERED);

    DATA_PASS(VaxOffsFil, VaxOffsFilT3,T2_T3, FILTERED);
    DATA_PASS(VbxOffsFil, VbxOffsFilT3,T2_T3, FILTERED);
    DATA_PASS(VcxOffsFil, VcxOffsFilT3,T2_T3, FILTERED);

    DATA_PASS(IaOutOffs,  IaOutOffsT1, T2_T1, UNFILTERED);
    DATA_PASS(IbOutOffs,  IbOutOffsT1, T2_T1, UNFILTERED);
    DATA_PASS(IcOutOffs,  IcOutOffsT1, T2_T1, UNFILTERED);

    DATA_PASS(VaxOffsOut, VaxOffsOutT1,T3_T1, UNFILTERED);
    DATA_PASS(VbxOffsOut, VbxOffsOutT1,T3_T1, UNFILTERED);
    DATA_PASS(VcxOffsOut, VcxOffsOutT1,T3_T1, UNFILTERED);


};

#endif  // B0_BRG_FBK_OFFS_H
