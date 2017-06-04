///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       2-level Cell Test derived class
//
// DESCRIPTION:
//      This file defines the 2-level derived cell test class, which is
//      based on a virtual bridge test class.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0CelTst.vsd, b0CelTst.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef    B0_CELTST_H
#define    B0_CELTST_H

// Include Files
//--------------
// system
// core
// framework
// product
#include "b0BrgTst.h"


// Constants
//----------
#define  DATA_ARRAY_SIZE  12        // Data and evaluation array size


// Function Prototypes
//--------------------


// Types and Classes
//------------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      2-level Cell Test class definition - Derived class
//
///////////////////////////////////////////////////////////////////////////////
class cCellTest : public cBridgeTest
{

//-----------------------------------------------------------------------------
public:
    // Types & Enumerations
    //---------------------
    enum AutoCtTyp
    {
        AUTOCT_OFF          = 0,            // Auto cell test is not running
        AUTOCT_TO_FAIL      = 1,            // Auto cell test is expected to fail
        AUTOCT_TO_PASS      = 2             // Auto cell test is expected to pass
    };

    struct InListTyp
    {
        const unsigned&      TstReq;        //  1 - Bridge test request from product code - Task2
        const unsigned&      TripFlt;       //  2 - Trip fault active
        const AutoCtTyp&     AutoCelTst;    //  3 - Type of automatic cell test running - Task2
        const unsigned&      MsgPending;    //  4 - Message pending on stack - Task2
        const unsigned&      TstDisable;    //  5 - Disable cell test operation - Task2
        const SysTm_Typ&     SysTm2;        //  6 - System time for Task2
        const SysTm_Typ&     SysTm1;        //  7 - System time for Task1
        const unsigned&      FpgaFltLatch;  //  8 - Fpga fault has latched disabling gating - Task1
        const BrgDsatIocTyp& BrgDsatIoc;    //  9 - Desaturation and over-current status - Task1
        const float&         IaFbk;         // 10 - PhA current feedback (amp) - Task1
        const float&         IbFbk;         // 11 - PhB current feedback (amp) - Task1
        const float&         IcFbk;         // 12 - PhC current feedback (amp) - Task1
        const float&         VabFbk;        // 13 - Phase a-to-b voltage feedback (volt) - Task1
        const float&         VbcFbk;        // 14 - Phase b-to-c voltage feedback (volt) - Task1
        const float&         VdfMagT2;      // 15 - Dist filter Vll magnitude (volt) - Task2
        const float&         VdcFbkT3;      // 16 - DC-link total voltage feedback (volt) - Task3
        const unsigned&      InvalidMode;   // 17 - Cell test not valid in present operating mode
        const unsigned&      Dual_Brg;      // 18 - Cell test not valid in present operating mode
    };

    // Methods
    //--------
    cCellTest( InListTyp InList, InsListTyp InsList );
    ~cCellTest( void );

    virtual void Task1( void );
    virtual void Task2( void );
    virtual int  RunRules( void );
    int CellTestDismissal( unsigned Action, float CT_DismssTmr );//communicates causes
                                                                 // that prevent cell-test
                                                                 // from running
    // Output Signals
    //---------------
    CREATE_PUBVAR(CelTstBrgPwrEnb, unsigned);  // cell test bridge power enable
    CREATE_PUBVAR(CelTstStat,      unsigned);  // cell test task Stat to sequencer
    CREATE_PUBVAR(CelTstAct,       unsigned);  // cell test task Active to sequencer
    CREATE_PUBVAR(CelTstProtEnab,  unsigned);  // enable external diagnostics like fbk imbalance
    CREATE_PUBVAR(CelTstBrgOff,    unsigned);  // all bridge devices configured to turn off
    CREATE_PUBVAR(CelTstOctAct,    unsigned);  // Cell test Open Circuit Test active
    CREATE_PUBVAR(CelTstOctPhs1,   unsigned);  // OCT active phase 1 (pulsed phase)
    CREATE_PUBVAR(CelTstOctPhs2,   unsigned);  // OCT active phase 2 (parked phase)

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;

    // Types & Enumerations
    //---------------------
    struct CelTstRunBits
    {
        unsigned  SCT_Done:1;
        unsigned  SCT_Fail:1;
        unsigned  OCT_Done:1;
        unsigned  OCT_Fail:1;
        unsigned  VFT_Done:1;
        unsigned  VFT_Fail:1;
    };

    struct CelTstData
    {
        float     Ia_Acc;
        float     Ib_Acc;
        float     Ic_Acc;
        int       I_Acc_Cnt;
        float     Vab;
        float     Vbc;
        float     Vca;
        float     Vdc;
        int       Flt_Code;
    };

    struct CelTstEvalData
    {
        union
        {
            struct
            {
                unsigned    Ia          :2;
                unsigned    Not_Used_1  :2;
                unsigned    Ib          :2;
                unsigned    Not_Used_2  :2;
                unsigned    Ic          :2;
                unsigned    Not_Used_3  :2;
                unsigned    Vab         :2;
                unsigned    Not_Used_4  :2;
                unsigned    Vbc         :2;
                unsigned    Not_Used_5  :2;
                unsigned    Vca         :2;
                unsigned    Not_Used_6  :2;
                unsigned    Not_Used_7  :8;
            }  Fbk_Lvl_Bits;
            int  Fbk_Lvl_Int;
        }  Fbk_Lvl;
        int Flt_Code;
    };

    // External Inputs
    //----------------
    InListTyp    In;

    // Methods
    //--------
    int  CellTestCurrentDetect( void );       // detect current as result of a pulse
    void CellTestCurrentFbks( void );         // calc and storage current feedbacks
    void CellTestEval( void );                // compare feedbacks against thresholds
    void CellTestGetFlts( void );             // record faults reported by bridge interface
    void CellTestResetData( void );           // clear data storage structures
    int  CellTestTimerCfg( void );            // calc timer for pulse width
    void CellTestVoltFbks( void );            // calc and storage voltage feedbacks
    int  CellTestZeroCurrent( void );         // determine if current is effectively zero
    void OpenCircuitTestDiagnosis( void );    // analyse open-circuit test results
    void SctOctTableMessage( void );          // construct short-&-open-circuit reporting messages
    void ShortCircuitTestDiagnosis( void );   // analyse short-circuit test results
    void VftTableMessage( void );             // construct volt-evaluation reporting messages
    void VoltageFeedbackTestDiagnosis( void );// analyse volt-evaluation test results

    // Diagnostics
    //------------
    CREATE_DIAG(CelTstMsgStkTrp);             // ADL message stack failed
    CREATE_DIAG(CelTstScNoDoneTrp);           // Auto cell test - Short circuit test not performed
    CREATE_DIAG(CelTstScFailTrp);             // Auto cell test - Short circuit test has failed
    CREATE_DIAG(CelTstOcNoDoneTrp);           // Auto cell test - Opern circuit test not performed
    CREATE_DIAG(CelTstOcCcPassTrp);           // Auto cell test - Crowbar contactor (ROVC) incorrectly passed open circuit test
    CREATE_DIAG(CelTstOcCcCurTrp);            // Auto cell test - Crowbar contactor (ROVC) incorrectly allowed current during open circuit test
    CREATE_DIAG(CelTstOcFailTrp);             // Auto cell test - Open circuit test has failed
    CREATE_DIAG(CelTstVtNoDoneTrp);           // Auto cell test - Voltage fbk test not performed
    CREATE_DIAG(CelTstVtFailTrp);             // Auto cell test - Voltage fbk test has failed

    // Parameters
    //-----------
    CREATE_PARM(CelTstCurLvl,     float);     // Converter cell test target current level (Amps)
    CREATE_PARM(CelTstInvModeEnb, unsigned);  // Allow cell test to run under a invalid operatin mode
    CREATE_PARM(CelTstVloadMax,   float);     // Maximum voltage across load allowed for cell test (Volts)
    CREATE_PARM(CelTstVdecayTm,   float);     // Maximum time allowed for load voltage to decay below CelTstVloadMax (Sec)
    CREATE_PARM(CelTstIthrsPct,   float);     // Open-circuit cell test current detection threshold (% of Shunt rating)
    CREATE_PARM(CelTstVthrsPct,   float);     // Short and Open circuit voltage detection threshold (% of measured Vdc)
    CREATE_PARM(CelTstMinPls,     float);     // Minimum pulse size for cell test (sec)

    // Published Variables
    //--------------------
    // T1<-->T2 state machine communication variables
    CREATE_PUBVAR(Hdw_Tst_Req,    unsigned);  // cell test state machine request in task2
    CREATE_PUBVAR(Hdw_Tst_ReqT1,  unsigned);  // cell test state machine request passed to task1
    CREATE_PUBVAR(Hdw_Tst_Stat,   unsigned);  // cell test state machine status in task1
    CREATE_PUBVAR(Hdw_Tst_StatT2, unsigned);  // cell test state machine status passed to task2
    CREATE_PUBVAR(Hdw_Tst_Act,    unsigned);  // cell test state machine active in task1
    CREATE_PUBVAR(Hdw_Tst_ActT2,  unsigned);  // cell test state machine active passed to task2
    CREATE_PUBVAR(CT_BrgEnbCmd,   unsigned);  // cell test task1 state-machine enable-command
    CREATE_PUBVAR(CT_BrgEnbCmdT2, unsigned);  // cell test enable-command passed to task 2
    CREATE_PUBVAR(CT_Req_T1,      unsigned);  // cell test task1 state machine request
    CREATE_PUBVAR(CT_Stat_T1,     unsigned);  // cell test task1 state machine status
    CREATE_PUBVAR(CT_Act_T1,      unsigned);  // cell test task1 state machine active
    CREATE_PUBVAR(CT_ASR_T1,      unsigned);  // cell test task1 Active-Status-Request state
    CREATE_PUBVAR(CT_ASR_T2,      unsigned);  // cell test task2 Active-Status-Request state
    CREATE_PUBVAR(CT_Ret_St_T1,   int);
    CREATE_PUBVAR(CT_St_T1,       int);       // cell test task1 state variable
    CREATE_PUBVAR(CT_Enb_Ab_T1,   int);
    CREATE_PUBVAR(CT_Dsb_Ab_T1,   int);
    CREATE_PUBVAR(CT_St_T2,       int);       // cell test task2 state variable
    CREATE_PUBVAR(CT_Enb_Ab_T2,   int);
    CREATE_PUBVAR(CT_Dsb_Ab_T2,   int);
    CREATE_PUBVAR(Pulse_Tmr,      int);      // PWM timer for rising edge of first pulse
    CREATE_PUBVAR(Pulse_Cnt,      int);      // Counter for scheduling of first pulse
    CREATE_PUBVAR(Loop_Cnt,       int);      // State machine loop counter
    CREATE_PUBVAR(Pls_Rpt_Cnt,    int);
    CREATE_PUBVAR(Pls_Act_Cnt,    int);
    CREATE_PUBVAR(Brg_Off_Cnt,    int);
    CREATE_PUBVAR(CT_Data_Ptr,    int);
    CREATE_PUBVAR(CT_I_Acc_Cnt,   int);      // Number of times currents and fluxes have been added
    CREATE_PUBVAR(CT_Tmr,         float);
    CREATE_PUBVAR(Park_Tmr,       float);
    CREATE_PUBVAR(Pulse_Tm,       float);    // Scheduled pulse width for cell test (Sec)
    CREATE_PUBVAR(Pulse_Mod,      float);    // Pulse width modified as per-unit of task1 period (Pu)
    CREATE_PUBVAR(Pulse_Frac,     float);    // Fraction of pulse width after removing multiples of task1 periods (Pu)
    CREATE_PUBVAR(CT_I_Thrsh,     float);
    CREATE_PUBVAR(CT_I_Vco,       float);
    CREATE_PUBVAR(CT_V_Thrsh,     float);
    CREATE_PUBVAR(CT_Vab_Intgr,   float);    // Summation of phs A to phs B flux (Volt.Sec)
    CREATE_PUBVAR(CT_Vbc_Intgr,   float);    // Summation of phs B to phs C flux (Volt.Sec)
    CREATE_PUBVAR(CT_Ia_Acc,      float);    // Accumulated phase A current (Amps)
    CREATE_PUBVAR(CT_Ib_Acc,      float);    // Accumulated phase B current (Amps)
    CREATE_PUBVAR(CT_Ic_Acc,      float);    // Accumulated phase C current (Amps)
    CREATE_PUBVAR(CelTstCurThrs,  float);    // Threshold for detection of current presence (Amps)

    CREATE_PUBVAR(AUBL_OctTst,    int);
    CREATE_PUBVAR(BUCL_OctTst,    int);
    CREATE_PUBVAR(CUAL_OctTst,    int);
    CREATE_PUBVAR(ALBU_OctTst,    int);
    CREATE_PUBVAR(BLCU_OctTst,    int);
    CREATE_PUBVAR(CLAU_OctTst,    int);

    // Un-published Variables
    //-----------------------
    SysTm_Typ  CT_Dly_Tm_T1;        // Timers for implementing delays
    SysTm_Typ  CT_Dly_Tm_T2;
    SysTm_Typ  CT_I_Zero_Tm;

    // Evaluation data structures
    cCellTest::CelTstRunBits     CT_Run;
    cCellTest::CelTstData        CT_Data[DATA_ARRAY_SIZE];
    cCellTest::CelTstEvalData    CT_Eval_Data[DATA_ARRAY_SIZE];

    // Data Passing
    //-------------
    DATA_PASS(Hdw_Tst_Req,  Hdw_Tst_ReqT1   , T2_T1,  UNFILTERED);
    DATA_PASS(Hdw_Tst_Stat, Hdw_Tst_StatT2  , T1_T2,  UNFILTERED);
    DATA_PASS(Hdw_Tst_Act,  Hdw_Tst_ActT2   , T1_T2,  UNFILTERED);
    DATA_PASS(CT_BrgEnbCmd, CT_BrgEnbCmdT2  , T1_T2,  UNFILTERED);

    // Inline Function Definitions
    //----------------------------

};

#endif  // B0_CELTST_H
