///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rebf Based Bridge Interface
//
// DESCRIPTION:
//      This file defines the Rebf based bridge interface class
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0BridgeRebf.vsd, b0BridgeRebf.xls
//
// ISSUES:
//      1) no bit in the control window to globally disable gate drive status
//          there are individual gate status masks which might meet the need
//
//      2) no bit in the control window to disable/bypass the gate sequencing
//         state machine that enforces min-on time, etc.
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __B0_BRIDGE_REBF_H__
#define __B0_BRIDGE_REBF_H__



// Include Files
//--------------
// system
#include <string>
// core
#include "MonitorCommand.h"
// framework
#include "x0FrameWork.h"
#include "x0BridgeInterface.h"
#include "x0Hssl.h"
// product
#include "b0BrgFbkOffs.h"
#include "s0BrgSim.h"
#include "b0BridgeRebfFpga.inc"

// Constants
//----------
// bridge types supported by the bridge interface(BridgeType)

#define REBF_BRG_TYP_2LEVEL         (0)
#define REBF_BRG_TYP_2LEVEL_DB      (1)

// hssl window usage
#define REBF_NUM_OF_WIN             (3)     // number of windows needed
#define REBF_WIN_FBK                (0)     // window 0 = feedback data
#define REBF_WIN_CMD                (1)     // window 1 = command data
#define REBF_WIN_DIAG               (2)     // window 2 = diagnostic data

// board feedback offset
#define AEDB_VDC_FBK_OFFSET_FREQ    (20.0e3)// AEDB Board Vdc feedback offset frequency

// Types and Classes
//------------------





///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//   This class implements a high speed serial link interface to the
//   Rebf/HSLA bridge interface card set.
//   This card sets support 3-level IGCT bridges.
//
///////////////////////////////////////////////////////////////////////////////
class cBridgeRebf
{

//-----------------------------------------------------------------------------
public:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;

    friend class cBridgeRebfTest;


    // Types
    //------
    struct InTyp
    {
        const unsigned&             Brg1EnableReq;      //  1 - bridge 1 enable request
        const PhsTransListTyp&      Brg1PhsTransList;   //  2 - bridge 1 phase transition list
        const PhsTransListTyp&      Brg1PhsDtransList;  //  3 - bridge 1 phase D transition list
        const unsigned&             FrameFrc;           //  4 - frc time for the upcoming frame boundary
        const unsigned&             FrameFrcZ;          //  5 - frc time for the current frame boundary
        const SysTm_Typ&            SysTm3;             //  6 - system time updated at Task3 rate
        const SysTm_Typ&            EventTm1;           //  7 - EventTm for Task1
        const unsigned&             ClearFltsT1;        //  8 - signal to reset latched faults in Task1
        const unsigned&             ClearFltsT3;        //  9 - signal to reset latched faults in Task3
        const unsigned&             OutRly101;          // 10 - signal to drive output relay 1 on IO 1
        const unsigned&             OutRly102;          // 11 - signal to drive output relay 2 on IO 1
        const unsigned&             OutRly103;          // 12 - signal to drive output relay 3 on IO 1
        const unsigned&             OutRly104;          // 13 - signal to drive output relay 4 on IO 1
        const unsigned&             OutRly105;          // 14 - signal to drive output relay 5 on IO 1
        const unsigned&             OutRly106;          // 15 - signal to drive output relay 6 on IO 1
        const unsigned&             OutRly107;          // 16 - signal to drive output relay 7 on IO 1
        const unsigned&             OutRly108;          // 17 - signal to drive output relay 8 on IO 1
        const unsigned&             OutRly109;          // 18 - signal to drive output relay 9 on IO 1
        cBrgFbkOffs&                Brg1FbkOffs;        // 19 - external bridge 1 feedback offset class
        cBrgFbkOffs&                Io1FbkOffs;         // 20 - external IO 1 feedback offset class
        const unsigned&             BrgCmdDb;           // 21 - fire Db1 request
        const unsigned&             ThermHi;            // 22 - 1=select High range for thermistor sensing, 0 = lo
        const float&                FanRef1;            // 23 - Fan#1 speed reference: <32 off, 32->255 0-100%
        const float&                FanRef2;            // 24 - Fan#2 speed reference: <32 off, 32->255 0-100%
        const unsigned&             MaskIocFlts;        // 25 - Mask A-B-C IOC  faults from setting fpga fault latch
        const unsigned&             IlimEnb;            // 26 - Enable fpga turn off of phase(s) with hi current
        const unsigned&             MaskDsatFlts;       // 27 - Mask A-B-C DSAT faults from setting fpga fault latch
        const unsigned&             BrgRdyLpTxDis;      // 28 - Disable brg ready loop FO Tx
        const unsigned&             RtrLoopFltEnb;      // 29 - Allow assertion of RtrLoop fault
        const unsigned&             OutLvRly01;         // 30 - signal to drive low-voltage output relay 01 on Rebf board
        const unsigned&             OutLvRly02;         // 31 - signal to drive low-voltage output relay 02 on Rebf board
        const unsigned&             OutLvRly03;         // 32 - signal to drive low-voltage output relay 03 on Rebf board
        const uint64_t&             T1FrameTm;          // 33 - time of next Task1 frame (cpu clock counts)
        const unsigned&             DisCmdNotFunc;      // 34 - Disable bridge request not working
        const volatile int&         T1wthnT3;           // 35 - T1 state within a T3 frame
        const unsigned&             InitDone;           // 36 - Signal to indicate initialization done
    };

    struct InsTyp
    {
        const float&                DelTm1;             //  1 - task1 period (sec)
        const float&                DelTm1Rcp;          //  2 - task1 frequency (tasks/sec)
        const float&                CpuCps;             //  3 - cpu cycles per second
        const unsigned&             SimMode;            //  4 - internal simulator mode
        const unsigned&             XSimMode;           //  5 - external simulator mode
                const unsigned&             DSimMode;           //  6  - dll simulator mode
        const unsigned&             AltCfg;             //  6 - configure parms for alternate(SEP,etc) operation
        const char*                 Uniquifier;         //  7 - class instance uniquifier string
        const unsigned&             PhsD1FltEnb;        //  8 - bridge 1 phase-D DSAT fault detection enable
        const unsigned&             Brg1InfcHdw;        //  9 - hardware connected to Bridge_1 connector
        const float&                ShuntRatBrg1;       // 10 - bridge 1 shunt current at 50 mV
        const float&                ShuntRatPhD;        // 11 - phase-D shunt current at 50 mV
        const unsigned&             Io1InfcHdw;         // 12 - hardware connected to I/O 1 connector
        const unsigned&             Io1AttenHdw;        // 13 - attenuator board connected I/O 1 voltage(low or high voltage input
        const float&                CtRatioIo1;         // 14 - CT ratio for I/O 1 current feedback
        const float&                LockOutTime;        // 15 - gating lock out time (seconds)
        const unsigned&             QntPerFrm;          // 16 - number of quants per task1 frame
        const unsigned&             Clk0PerQnt;         // 17 - number of clock-tics (or counts) in one quant
        const unsigned&             Clk0PerFrm;         // 18 - FRC counts in task 1 frame
        const unsigned&             HslaSite;           // 19 - HSLA site on the REBF board
        const int&                  TestMode;           // 20 - test mode
        const unsigned&             LineSide;           // 21 - Select Line converter fpga behavior
        const float&                DelTm3;             // 22 - task3 period (sec)
        const unsigned&             Io1RlyPwrChkInhReq; // 23 - Io1 relay pwr supply check inhibit request
        const float&                CpuCpsRcpUsec;      // 24 - Cpu clock period (usec)
        const int&                  TempVcoCalcSt;      // 25 - Vco temperature calculation state
                const int&                  TachPPR;            // 27 - Number of tachometer pulses per revolution
    };


    // Enumerations
    //-------------

    // rebf connectors sites for the hsla
    enum HslaSiteTyp
    {
        HSLA_SITE_JA1           = 0,
        HSLA_SITE_JA2           = 1
    };

    // hardware bridge interface boards (connected to Bridge_1(2) suppported by class
    enum BrgInfcHdwTyp
    {
        BRG_INFC_NONE           = 0,
        BRG_INFC_AEDBH1         = 1,
        BRG_INFC_AEDBH2         = 2,
        BRG_INFC_AEDBH3         = 3,
        BRG_INFC_AEDBH4         = 4,
        BRG_INFC_AEBIH1         = 5,
        BRG_INFC_AEBIH2         = 6,
        BRG_INFC_AEBIH3         = 7,
        BRG_INFC_RGIBH1         = 8
    };

    // hardware IO boards (connected to I/O 1 &2) suppported by class
    enum IoInfcHdwTyp
    {
        IO_INFC_NONE           = 0,
        IO_INFC_AEAAH1         = 1,
        IO_INFC_AEAAH2         = 2,
        IO_INFC_AEAAH3         = 3,
        IO_INFC_AEAAH4         = 4,
        IO_INFC_AEADH1         = 5,
        IO_INFC_AEADH2         = 6,
        IO_INFC_AEADH3         = 7,
        IO_INFC_AEADH4         = 8
    };

    // hardware IO boards (connected to I/O 1 &2) suppported by class
    enum IoAttenHdwTyp
    {
        NO_ATTENUATOR_BOARD           = 0,
        NATPG1_LOW_VOLT_INPUT         = 1,
        NATPG1_LOW_VOLT_INPUT_690     = 2
    };

    #include "b0BridgeRebfFpga.inc"


    // Methods
    //--------
    cBridgeRebf( InTyp In, InsTyp Ins );
    ~cBridgeRebf( void );

    // initialization functions
    int         Open                ( unsigned LinkIdReq, unsigned DevIdReq, unsigned ChanIdReq );
    int         Close               ( void );

    // normal running functions
    void        Task1Fbk            ( void );
    void        Task1Cmd            ( void );
    void        Task2               ( void );
    void        Task3               ( void );
    void        TaskBkgnd           ( void );

    // rule methods
    int         RunRules            ( void );

    // status functions
    float       WhatIsFrcFreq       ( void ) const;
    unsigned    WhatIsFrcSchedMax   ( void ) const;
    unsigned    WhatIsFrcSchedMask  ( void ) const;

    // functions to return feedback factors
    double      WhatIsVcoFmax       ( void ) const;
    double      WhatIsVcoFmin       ( void ) const;
    unsigned    WhatIsVcoMask       ( void ) const;
    double      WhatIsVcoFzroVoltage( unsigned bridge ) const;
    double      WhatIsVcoFzroCurrent( unsigned bridge ) const;
    float       WhatIsSclPhaseVolts ( unsigned bridge ) const;
    float       WhatIsSclCurrent    ( unsigned bridge ) const;
    float       WhatIsIocCurrent    ( unsigned bridge ) const;
    float       WhatIsVdcScale      ( unsigned bridge ) const;
    float       WhatIsVdcQuant      ( unsigned bridge ) const;
    float       WhatIsSclIOVoltage  ( unsigned IO ) const;
    float       WhatIsIOVoltageQuant( unsigned IO ) const;
    float       WhatIsSclIOCurrent  ( unsigned IO ) const;
    double      WhatIsVcoFzroIoVolt ( unsigned IO ) const;

    // simulator interfaces
    void        SimReadCmdVavg      ( BrgCmdVavgSimTyp& BrgCmdVavgSim);
    void        SimReadCmdVavg      ( BrgCmdVavgSimTyp& BrgCmdVavgSim, float Vdc);
    void        SimReadDbCmd        ( BrgDbCmdSimTyp& BrgDbCmdSim);
    void        SimReadCmdTra       ( BrgCmdTraSimTyp&  BrgCmdTraSim );
    void        SimWriteFbk         ( const BrgFbkSimTyp& BrgFbkSim );
    void        SimWriteFlt         ( const BrgFltSimTyp& BrgFltSim );
    void        SimPwmTm(void);

    // Output Signals
    //---------------
    CREATE_PUBVAR(IaBrg1Hdw,            float);                     // phase A bridge 1 phase currents
    CREATE_PUBVAR(IbBrg1Hdw,            float);                     // phase B
    CREATE_PUBVAR(IcBrg1Hdw,            float);                     // phase C
    CREATE_PUBVAR(IdBrg1Hdw,            float);                     // phase D

    CREATE_PUBVAR(VabBrg1Hdw,           float);                     // bridge 1 line-line voltages
    CREATE_PUBVAR(VbcBrg1Hdw,           float);

    CREATE_PUBVAR(VdBrg1Hdw,            float);                     // bridge 1 dc link voltage if AEBI, voltage across Db1 resistor or Dc-chopper phase if AEDB
    CREATE_PUBVAR(VdcFiberBrgHdw,       float);                     // DC link voltage via fiber-optic

    CREATE_PUBVAR(AmbientTemp,          float);                     // Hsla/carrier board ambient temp

    CREATE_PUBVAR(Brg1Enabled,          unsigned);                  // bridge 1 gating is enabled according to fpga status feedback
    CREATE_PUBVAR(Brg1PhDenabled,       unsigned);                  // bridge 1 phase-D gating is enabled according to fpga configuration readback

    CREATE_PUBVAR(ContactIn1Io1,        unsigned);                  // bridge contact inputs
    CREATE_PUBVAR(ContactIn2Io1,        unsigned);
    CREATE_PUBVAR(ContactIn3Io1,        unsigned);
    CREATE_PUBVAR(ContactIn4Io1,        unsigned);
    CREATE_PUBVAR(ContactIn5Io1,        unsigned);
    CREATE_PUBVAR(ContactIn6Io1,        unsigned);
    CREATE_PUBVAR(ContactIn7Io1,        unsigned);
    CREATE_PUBVAR(ContactIn8Io1,        unsigned);
    CREATE_PUBVAR(ContactIn9Io1,        unsigned);
    CREATE_PUBVAR(ContactIn10Io1,       unsigned);

    CREATE_PUBVAR(ActiveDigIn1,         unsigned);
    CREATE_PUBVAR(CtlPwrOk,                float);                  // float copy of CtlPwrOk1 from structure
    CREATE_PUBVAR(RlyPwrOk,                float);                  // float copy of RlyPwrOk1 from structure
    CREATE_PUBVAR(CtlPwrOkT2,              float);                  // float copy of CtlPwrOk1 from structure for T2
    CREATE_PUBVAR(CtlPwrOkFil,             float);                  // float copy of CtlPwrOk1 from structure for T2, filtered
    CREATE_PUBVAR(CtlPwrOkFilWdt,          float);                  // Gain for CtlPwrOkFil
    CREATE_PUBVAR(RlyPwrOkT2,              float);                  // float copy of RlyPwrOk1 from structure for T2
    CREATE_PUBVAR(RlyPwrOkFil,             float);                  // float copy of RlyPwrOk1 from structure for T2, filtered
    CREATE_PUBVAR(RlyPwrOkFilWdt,          float);                  // Gain for CtlRlyOkFil


    CREATE_PUBVAR(VaxIo1,               float);                     // voltage and current feedbacks from board on Io/1 connector
    CREATE_PUBVAR(VbxIo1,               float);
    CREATE_PUBVAR(VcxIo1,               float);
    CREATE_PUBVAR(IaIo1,                float);
    CREATE_PUBVAR(IbIo1,                float);
    CREATE_PUBVAR(IcIo1,                float);

    CREATE_PUBVAR(ThermistorVcoIo1,     unsigned);                  // VCO counter for AEAD IO#1 thermister temperatures
    CREATE_PUBVAR(BrgTempVcoSel,        unsigned);                  // Mux setting for bridge temperature

    CREATE_PUBVAR(HumidThermTemp,  unsigned);                       // Humidity temperature reading
    CREATE_PUBVAR(HumidThermPct,   unsigned);                       // Percent humidity reading

    CREATE_PUBVAR(CellTempVcoBrg1,      unsigned);                  // VCO counter for bridge#1 cell temperatures

    CREATE_PUBVAR(TachValue,            unsigned);                  // Rollover up/down counter. Counts up/down for positive (A then B edge)/negative (B then A edge) rotation
    CREATE_PUBVAR(TachMrkrValue,        unsigned);                  // Capture of TachV at the RE of marker pulse Combined with TachV to make positional calculations
    CREATE_PUBVAR(TachTm,               unsigned);                  // Rollover counter of time. 1count = 1usec
    CREATE_PUBVAR(TachPm,               unsigned);                  // Rollover counter. Counts the time between A & B tach pulses. 1 count = 1usec.
    CREATE_PUBVAR(TachPhases,           unsigned);                  // Discrete inputs from tach M,/M,B,/B,A,/A Bits 5-0

    CREATE_PUBVAR(FpgaFltLatch,         unsigned);                  // fpga detected fault that disables gating
    CREATE_PUBVAR(HsslCommFlt,          unsigned);                  // HSSL communication fault
    CREATE_PUBVAR(BrgHdwDataVld,        unsigned);                  // Bridge hardware data valid indication - Task1 update

    CREATE_PUBVAR(AeaxAin1Freq,         float);
    CREATE_PUBVAR(AeaxAin2Freq,         float);
    CREATE_PUBVAR(AeaxAin3Freq,         float);
    CREATE_PUBVAR(AeaxAin4Freq,         float);

    CREATE_PUBVAR(AeaxAin5Freq,         float);
    CREATE_PUBVAR(AeaxAin6Freq,         float);
    CREATE_PUBVAR(AeaxAin7Freq,         float);

    CREATE_PUBVAR(VabFreq,              float);
    CREATE_PUBVAR(VbcFreq,              float);
    CREATE_PUBVAR(IaFreq,               float);
    CREATE_PUBVAR(IbFreq,               float);
    CREATE_PUBVAR(IcFreq,               float);

    CREATE_PUBVAR(HsslCmdMrg,           float);                     // margin between hssl xmt and end of frame (usec)
    CREATE_PUBVAR(HsslCmdMrgMin,        float);                     // worst case of HsslCmdMrg
    CREATE_PUBVAR(CntgncyPktRcvd,       unsigned);                  // feedback packet is a contingency packet (LdPls2 bit is set)

    BrgDsatIocTyp       Brg1DsatIoc;                                // fpga desat & ioc fault report on bridge 1

    CREATE_PUBVAR(DisCmdNotFlg,        unsigned);
    CREATE_PUBVAR(BrlDisDlyTmr,        float);

    CREATE_PUBVAR(VabBrgVcoCnt,       float);
    CREATE_PUBVAR(VbcBrgVcoCnt,       float);
    CREATE_PUBVAR(IaBrgVcoCnt,        float);
    CREATE_PUBVAR(IbBrgVcoCnt,        float);
    CREATE_PUBVAR(IcBrgVcoCnt,        float);

    CREATE_PUBVAR(VaxIoVcoCnt,        float);
    CREATE_PUBVAR(VbxIoVcoCnt,        float);
    CREATE_PUBVAR(VcxIoVcoCnt,        float);
    CREATE_PUBVAR(IaIoVcoCnt,         float);
    CREATE_PUBVAR(IbIoVcoCnt,         float);
    CREATE_PUBVAR(IcIoVcoCnt,         float);
    CREATE_PUBVAR(VcxOffset,          float);

        CREATE_PUBVAR(SimSpdEncLatchCnt ,  unsigned);              // Dsim Speed processing
    CREATE_PUBVAR(SimSpdEncDelCntRes,  float   );              //
    CREATE_PUBVAR(SimSclSpdEnc      ,  float   );              //
    CREATE_PUBVAR(SimSclSpdEncRcp   ,  float   );              //
    CREATE_PUBVAR(SimBrgFbkTachV ,  float   );              // Simulated tach velocity, average over one frame (Rad/Sec)

    CREATE_PUBVAR(HslaApplRevCode,    unsigned);
    CREATE_PUBVAR(HsslApplRevCode,    unsigned);

    CREATE_PUBVAR(RebhSet, unsigned);
    CREATE_PUBVAR(TargInitDly, float);

    unsigned SimPhAUpTm;
    unsigned SimPhADnTm;
    unsigned SimPhBUpTm;
    unsigned SimPhBDnTm;
    unsigned SimPhCUpTm;
    unsigned SimPhCDnTm;

//-----------------------------------------------------------------------------
private:

    // External Interfaces
    //--------------------
    InTyp       In;
    InsTyp      Ins;


    // Defines
    //--------
    #define REBx_DIAG_BRIDGE_EVENTS      0       // enable compilation of storage of bridge events in individual published variables
    #define REBx_DIAG_FPGA_REGISTERS     0       // enable compilation of storage of fpga registers in individual published variables
    #define REBx_DIAG_FAULT_TEST         0       // enable compilation of overwrite of fpga feedback registers to invoke faults


    // Types
    //------
    char      HslaApplFile[20];
    char      HslaApplVer[20];
    //unsigned  HslaApplRevCode;

    struct VcoSampBrgHdwTyp
    {
        float           Ia;
        float           Ib;
        float           Ic;
        float           Vab;
        float           Vbc;
        float           Vdc;
        float           Va;             // REBH: Add
        float           Vb;             // REBH: Add
        float           Vc;             // REBH: Add
    };

    struct VcoSampIo1HdwTyp
    {
        float           Ia;
        float           Ib;
        float           Ic;
        float           Vax;
        float           Vbx;
        float           Vcx;
        float           Vdx;
    };

    struct BilCalcTyp
    {
        int             mincnt;
        int             maxcnt;
        int             offcnt;
        float           sclcnt;
    };

    // Methods
    //--------
    void        InitializeTarget    ( void );
    unsigned    InitializeBIL       ( void );

    void        XlateGateCmds       ( void );
    void        CheckBridgeStatus   ( void );

    void        CommandCalc         ( void );
    void        FeedbackCalc        ( void );
    void        FeedbackZero        ( void );

    int         CalcBilRegister     ( float input, float scale, const BilCalcTyp& reg , int& status);

    bool        IsHostOpen          ( void ) const;

    int         ValidatePriPwaList  ( void );
    int         ValdteRebhPrPwaLst  ( void );
    int         ValidateAuxPwaList  ( void );
    int         ValdteRebhAuxPwaLst ( void );
    int         AeadGnCalc          ( void );
    void        AeadGnDefault       ( void );
    int         AeaaGnCalc          ( void );

    static int  LlmAdd              ( cMonitorCommand& gMonitor );
    static int  Llm                 ( cParamString& OutputBuffer, cParamString& CommandLineArgs );

    // Diagnostics
    //------------
    CREATE_DIAG(DesatFltAL);
    CREATE_DIAG(DesatFltAU);
    CREATE_DIAG(DesatFltBL);
    CREATE_DIAG(DesatFltBU);
    CREATE_DIAG(DesatFltCL);
    CREATE_DIAG(DesatFltCU);
    CREATE_DIAG(DesatFltD);

    CREATE_DIAG(IocPhsA1);
    CREATE_DIAG(IocPhsB1);
    CREATE_DIAG(IocPhsC1);
    CREATE_DIAG(IocPhsD);

    CREATE_DIAG(HsslOpnFail);

    CREATE_DIAG(HsslDscFail);

    CREATE_DIAG(HsslIniFail);

    CREATE_DIAG(HsslCommErr);                           // errors detected in HSSL communication link
    CREATE_DIAG(HsslCommAlm);
    CREATE_DIAG(HsslBrgEnbFailTrp);                     // target bridge did not enable gating when commanded
    CREATE_DIAG(HsslInvCfgEnbTrp);                      // invalid enabling of target configuration changes
    CREATE_DIAG(HsslLdPls2Alm);
    CREATE_DIAG(HsslLdPls2Trp);

    CREATE_DIAG(GdPwrSup1Fail);
    CREATE_DIAG(GdPwrSup3Fail);

    CREATE_DIAG(HslaPwrFlt);
    CREATE_DIAG(HslaClkFlt);
    //CREATE_DIAG(P19OkFlt);
    CREATE_DIAG(P5OkFlt);
    CREATE_DIAG(Sdl1BrgErr);
    CREATE_DIAG(Sdl2BrgErr);
    CREATE_DIAG(Sdl3BrgErr);
    CREATE_DIAG(Sdl4BrgErr);

    CREATE_DIAG(RebfCtlPwrFlt);

    CREATE_DIAG(IoCtlPwrFlt);
    CREATE_DIAG(IoRlyPwrFlt);

    CREATE_DIAG(BrgFirXlateFlt);

    CREATE_DIAG(TargFrcErrFlt);
    CREATE_DIAG(TargFrameErrFlt);
    CREATE_DIAG(TargLdPlsAlm);

    CREATE_DIAG(UnresolvedBrgFlt);

    CREATE_DIAG(RdytrLoopShtdwn);
    CREATE_DIAG(RdytrLoopShtdwnBr);

    CREATE_DIAG(PwaInvHslaTrp);                         // pwa inventory problem - HSLA
    CREATE_DIAG(PwaInvHslaSiteTrp);                     // pwa inventory problem - HSLA site on the REBF incorrect
    CREATE_DIAG(PwaInvRebfTrp);                         // pwa inventory problem - REBF
    CREATE_DIAG(PwaInvRebfJ1Trp);                       // pwa inventory problem - REBF-J1 bridge interface card
    CREATE_DIAG(PwaInvRebfJ2Trp);                       // pwa inventory problem - REBF-J2 bridge interface card
    CREATE_DIAG(PwaInvRebfP1Trp);                       // pwa inventory problem - REBF-P1 i/o interface card
    CREATE_DIAG(PwaInvRebfP2Trp);                       // pwa inventory problem - REBF-P2 i/o interface card
    CREATE_DIAG(IoAeadDsrBadTrp);                       // IO AEAD PWA design records bad
    CREATE_DIAG(IoPwaGainTrp);                          // IO AEAD PWA board ID gain fault
    CREATE_DIAG(IoPwaGainAlm);                          // IO AEAD PWA board ID gain alarm
    CREATE_DIAG(IoPwaOffsTrp);                          // IO AEAD PWA board ID offset fault
    CREATE_DIAG(IoPwaOffsAlm);                          // IO AEAD PWA board ID offset alarm

    CREATE_DIAG(PwaInvRebhTrp);                         // pwa inventory problem - REBH
    CREATE_DIAG(PwaInvRebhP1Trp);                       // pwa inventory problem for P1 connector - REBH
    CREATE_DIAG(PwaInvRebhP2Trp);                       // pwa inventory problem for P2 connector - REBH
    CREATE_DIAG(PwaInvRebhJ01Trp);                      // pwa inventory problem for J01 connector - REBH
    CREATE_DIAG(PwaInvRebhJ02Trp);                      // pwa inventory problem for J02 connector - REBH
    CREATE_DIAG(PwaInvRebhJ03Trp);                      // pwa inventory problem for J03 connector - REBH
    CREATE_DIAG(PwaInvRebhJ04Trp);                      // pwa inventory problem for J04 connector - REBH
    CREATE_DIAG(PwaInvRebhJ06Trp);                      // pwa inventory problem for J06 connector - REBH
    CREATE_DIAG(PwaInvRebhJ07Trp);                      // pwa inventory problem for J07 connector - REBH
    CREATE_DIAG(PwaInvRebhJ08Trp);                      // pwa inventory problem for J08 connector - REBH

    CREATE_DIAG(DisCmdNotFuncTrp);


    // Parameters
    //-----------
    CREATE_PARM(HsslEnb,                unsigned);      // enable the high speed serial link

    CREATE_PARM(HwIocCntLmt,            unsigned);      // H/W ioc count limit (ioc occurrances)

    CREATE_PARM(HwFdbkTxDlyTm,          float);         // H/W feedback packet tx delay after load pulse (sec)
    CREATE_PARM(HwDiagTxDlyTm,          float);         // H/W diagnostic packet tx delay after load pulse (sec)

    CREATE_PARM(CurSensorType,          unsigned);      // Current sensor type - SHUNT or LEM
    CREATE_PARM(CurSensorGnBrg1,        float);         // Current sensor gain bridge 1
    CREATE_PARM(VdcAttenBrg1,           float);         // dc voltage attenuation bridge 1
    CREATE_PARM(VphAttenBrg1,           float);         // phase voltage attenuation bridge 1
    CREATE_PARM(ShuntTrpLvlBrg1,        float);         // Voltage across shunt bridge 1 for Ioc trip (Volts)

    CREATE_PARM(CurSensorGnIo1,         float);         // Current sensor gain(voltage across burden resistor at max current) IO 1
    CREATE_PARM(VphMaxIo1,              float);         // phase voltage maximum (for max VCO frequency) IO 1
    CREATE_PARM(IinTestIo1,             float);         // phase current maximum (for max VCO frequency) IO 1

    CREATE_PARM(DiDtL,                  float);         // Bridge's DIDT Reactor Inductance(Henry)

    CREATE_PARM(VgndFs,                 float);         // Full scale ground voltage at the VCO input
    CREATE_PARM(VdcFs,                  float);         // Full scale DC link voltage at the VCO input
    CREATE_PARM(VacFs,                  float);         // Full scale phase voltage at the VCO input
    CREATE_PARM(IfbkFs,                 float);         // Full scale phase current at the VCO input

    CREATE_PARM(SimModeBrgEnb,          unsigned);      // enable bridge in internal simulation mode
    CREATE_PARM(SimModeSdlEnb,          unsigned);      // enable SDL in internal simulation mode

    CREATE_PARM(TargFpgaRvCk,           WORD);          // force target fpga revision code check to this value if non-zero
    CREATE_PARM(TargFpgaReprog,         unsigned);      // force target fpga re-program during discovery if true

    CREATE_PARM(TargTstPt1Sel,          unsigned);      // hssl target test point 1 select
    CREATE_PARM(TargTstPt2Sel,          unsigned);      // hssl target test point 2 select

    CREATE_PARM(HwGdStatMask,           WORD);          // disable individual gate drive faults (bit mask)

    CREATE_PARM(HsslTstIntvl,           int);           // Hssl late packet test interval (late pkt every N frames)
    CREATE_PARM(HsslTstMrgMax,          float);         // Hssl late packet test maximum margin (usec)
    CREATE_PARM(HsslTstMrgMin,          float);         // Hssl late packet test minimum margin (usec)

    CREATE_PARM(HsslLdPls2AlmEnb,       unsigned);      // enable HsslLdPls2Alm assertion
    CREATE_PARM(PwaInvHslaDsb,          unsigned);      // disable pwa inventory check for HSLA
    CREATE_PARM(PwaInvRebfDsb,          unsigned);      // disable pwa inventory check for REBF
    CREATE_PARM(PwaInvRebfJ1Dsb,        unsigned);      // disable pwa inventory check for REBF-J1 bridge interface card
    CREATE_PARM(PwaInvRebfJ2Dsb,        unsigned);      // disable pwa inventory check for REBF-J2 bridge interface card
    CREATE_PARM(PwaInvRebfP1Dsb,        unsigned);      // disable pwa inventory check for REBF-P1 i/o interface card
    CREATE_PARM(PwaInvRebfP2Dsb,        unsigned);      // disable pwa inventory check for REBF-P2 i/o interface card
    CREATE_PARM(AeadIDAdjInh,           unsigned);      // Aead board ID gain and offset adjustment inhibit
    CREATE_PARM(VGnErMax,               float);         // feedback voltage gain error max (error between board id calc and nominal)
    CREATE_PARM(IGnErMax,               float);         // feedback current gain error max (error between board id calc and nominal)
    CREATE_PARM(OffErMax,               float);         // feedback offset gain error max (error between board id calc and nominal)
    CREATE_PARM(Io1CTITestScl,          float);         // Io1 CT scale factor
    CREATE_PARM(Io1VlnHvTestScl,        float);         // Io1 input test voltage scale between the high/low voltage

    CREATE_PARM(RlyPwrOkFilW,           float);         // Control and relay power filter omega
    CREATE_PARM(CtlPwrOkFilW,           float);         // Control and relay power filter omega
    CREATE_PARM(CtlRlyPwrOkThrs,        float);         // Control and relay trip power threshold.  1.0 is ok all passes, 0 is none



    CREATE_PARM(Io1RlyPwrChkInh,        unsigned);      // IO board 1 relay power supply check inhibit

    CREATE_PARM(PwaInvRebhDsb,          unsigned);      // disable pwa inventory check for REBH
    CREATE_PARM(PwaInvRebhP1Dsb,        unsigned);      // disable pwa inventory check for REBH-P1 i/o interface card
    CREATE_PARM(PwaInvRebhP2Dsb,        unsigned);      // disable pwa inventory check for REBH-P2 i/o interface card
    CREATE_PARM(PwaInvRebhJ01Dsb,       unsigned);      // disable pwa inventory check for REBH-J01 brg interface card
    CREATE_PARM(PwaInvRebhJ02Dsb,       unsigned);      // disable pwa inventory check for REBH-J02 brg interface card
    CREATE_PARM(PwaInvRebhJ03Dsb,       unsigned);      // disable pwa inventory check for REBH-J03 brg interface card
    CREATE_PARM(PwaInvRebhJ04Dsb,       unsigned);      // disable pwa inventory check for REBH-J04 brg interface card
    CREATE_PARM(PwaInvRebhJ06Dsb,       unsigned);      // disable pwa inventory check for REBH-J06 brg interface card
    CREATE_PARM(PwaInvRebhJ07Dsb,       unsigned);      // disable pwa inventory check for REBH-J07 brg interface card
    CREATE_PARM(PwaInvRebhJ08Dsb,       unsigned);      // disable pwa inventory check for REBH-J08 brg interface card

    CREATE_PARM(RgabPresent,            unsigned);      // RGAB bridge card present in the setup under test.
    CREATE_PARM(PhsIocThrs,             float   );      // Phase IOC Threshold

#if REBx_DIAG_FAULT_TEST
    // test parameters for simulating faults
    //--------------------------------------
    CREATE_PARM(BrgFltTst,              unsigned);
    CREATE_PARM(BrStatFltTst,           unsigned);
    CREATE_PARM(StatComTst,             unsigned);
    CREATE_PARM(StatReg21Tst,           unsigned);
    CREATE_PARM(MiscStatTst,            unsigned);
#endif


    // Published variables
    //--------------------
    CREATE_PUBVAR(AeadGnCalcEnb,        unsigned);      // Task1-> Task3 flag to enable AEAD gain calcs
    CREATE_PUBVAR(AeadGnCalcDone,       unsigned);      // AEAD gain calcs are done

    CREATE_PUBVAR(AeaaGnCalcEnb,        unsigned);      // Task1-> Task3 flag to enable AEAD gain calcs
    CREATE_PUBVAR(AeaaGnCalcDone,       unsigned);      // AEAD gain calcs are done

    CREATE_PUBVAR(CurSensorSclBrg1,     float);         // scale the current sensor shunt amp/volt
    CREATE_PUBVAR(CurSensorSclPhD,      float);         // scale the current sensor shunt amp/volt

    CREATE_PUBVAR(SclLinkVoltsBrg1,     float);         // Bridge 1 link voltage Scales in volts/hz

    CREATE_PUBVAR(SclVoltBrgOffset,     float);         // Bridge phase/dc voltage offset

  //CREATE_PUBVAR(ScaleGrndVolts,       float);         // Scales in volts/hz

    CREATE_PUBVAR(SclPhsVoltsBrg1,      float);         // Bridge 1 phase voltages Scales in volts/hz

    CREATE_PUBVAR(SclPhsVoltsIo1,       float);         // Io 1 nominal phase voltages Scales in volts/hz

    CREATE_PUBVAR(SclPhsCurBrg1,        float);         // Bridge 1 nominal phase current Scales in amp/hz
    CREATE_PUBVAR(SclPhsCurPhD,         float);         // Phase-D nominal phase current Scales in amp/hz

    CREATE_PUBVAR(SclPhsCurIo1,         float);         // Io 1 nominal phase current Scales in amp/hz

    CREATE_PUBVAR(VcoBiasFreqBrg1,      float);         // Bridge 1 VCO bias frequency for line-line voltage

    CREATE_PUBVAR(HsslErrChksEnb,       unsigned);      // HSSL state has advanced to running state allowing error checks

    CREATE_PUBVAR(HsslErrFlags,         WORD);          // HSSL bit-coded running errors (task1 & data passed from task3)
    CREATE_PUBVAR(HsslErrFlagsT3,       WORD);          // HSSL bit-coded running errors (task3)
    CREATE_PUBVAR(HsslErrFlagsAcc,      WORD);          // HSSL bit-coded running errors (accumulation requiring reset to clear)

    CREATE_PUBVAR(HsslErrFlagsIni,      WORD);          // HSSL bit-coded initialization state machine errors
    CREATE_PUBVAR(HsslErrFlagsDsc,      WORD);          // HSSL bit-coded discovery state machine errors

    CREATE_PUBVAR(HsslAlmFlags,         WORD);          // HSSL bit-coded running alarms (task3)
    CREATE_PUBVAR(HsslAlmFlagsAcc,      WORD);          // HSSL bit-coded running alarms (accumulation requiring reset to clear)

    CREATE_PUBVAR(Brg1EnbMisMatch,      unsigned);      // bridge 1 gating enable mismatch with fpga counter
    CREATE_PUBVAR(Brg1DsbMisMatch,      unsigned);      // bridge 1 gating disable mismatch with fpga counter

    CREATE_PUBVAR(BrgCfgEnableReq,      unsigned);      // bridge configuration change enabled request
    CREATE_PUBVAR(BrgCfgEnabled,        unsigned);      // bridge configuration change enabled
    CREATE_PUBVAR(BrgRtrEnabled,        unsigned);      // bridge ready-to-run enabled

    CREATE_PUBVAR(BrgFltResetDly,       unsigned);      // counter for periodically issuing a reset to the fgpa
    CREATE_PUBVAR(PhsDdsatFltLatch,     unsigned);      // fpga latched indicvidual phase-D dsat fault

    CREATE_PUBVAR(HsslHostState,        WORD);          // HSSL host state
    CREATE_PUBVAR(HsslTargState,        WORD);          // HSSL target state

    CREATE_PUBVAR(SimModeBrgEnabled,    unsigned);      // bridge is enabled in sim mode
    CREATE_PUBVAR(SimModeSdlEnabled,    unsigned);      // SDL is enabled in sim mode

    CREATE_PUBVAR(TargetHdwExpected,    unsigned);      // HSSL target hardware is expected
    CREATE_PUBVAR(TargetSdlExpected,    unsigned);      // SDL target hardware is expected

    CREATE_PUBVAR(HsslErrCnt,           unsigned);      // count of # of HSSL running errors (detected by host)
    CREATE_PUBVAR(TargRxErrCnt,         unsigned);      // count of # of HSSL Rx packets with bad CRC (detected by target)
    CREATE_PUBVAR(TargTxErrCnt,         unsigned);      // count of # of HSSL Tx packets receive NAK (detected by target)
    CREATE_PUBVAR(TargPktIgnoreCnt,     unsigned);      // count of # of HSSL packets ignored (detected by target)

    CREATE_PUBVAR(TargFpgaRev,          WORD);

    CREATE_PUBVAR(BrgTstModeState,      unsigned);      // bridge test state variable

    CREATE_PUBVAR(TargDscCase,          int);           // target discover state machine case #
    CREATE_PUBVAR(TargDscPass,          unsigned);      // target discover state machine pass #
    CREATE_PUBVAR(TargDscDone,          unsigned);      // target discover state machine done flag

    CREATE_PUBVAR(TargInitCase,         int);           // target initialize state machine case #
    CREATE_PUBVAR(TargInitPass,         unsigned);      // target initialize state machine pass #
    CREATE_PUBVAR(TargInitDone,         unsigned);      // target initialize state machine done flag
    CREATE_PUBVAR(IocThreshold,         unsigned);      // IOC Threshold

    CREATE_PUBVAR(VcoBr1VabRaw,         float);         // raw vco count diag var (e.g. to observe in cap buf)
    CREATE_PUBVAR(HsslFbkWait,          float);         // feedback packet wait time (usecs)
    CREATE_PUBVAR(HsslFbkWaitMin,       float);         // min feedback packet wait time (usecs)
    CREATE_PUBVAR(HsslFbkWaitMax,       float);         // max feedback packet wait time (usecs)
    CREATE_PUBVAR(HsslFbkWaitLdPls2,    float);         // value of HsslFbkWait when LdPls2 1st detected
    CREATE_PUBVAR(LdPls2EvtFlg0,        unsigned);      // LdPls2Evt bit is seen immediately
    CREATE_PUBVAR(LdPls2EvtFlg,         unsigned);      // LdPls2Evt bit is seen while waiting for packet count match
    CREATE_PUBVAR(LdPls2EvtCtr,         unsigned);      // LdPls2Evt counter
    CREATE_PUBVAR(LdPls2EvtCtrTot,      unsigned);      // LdPls2Evt counter total

    CREATE_PUBVAR(FbkSampFrcTime,       WORD);

    CREATE_PUBVAR(TargDscReq,           unsigned);      // reqeust to re-discover the hssl link

    CREATE_PUBVAR(CmdPktCnt,            WORD);          // command packet counter

    CREATE_PUBVAR(HsslFbkLateCtr,       unsigned);      // counts sequential task1s in which the feedback packet is late

    CREATE_PUBVAR(Brg1InfcBoardAedb,    unsigned);      // Bridge 1 interface board is AEDB

    // diagnostic variables
    CREATE_PUBVAR(BrgStatFlg,           WORD);          // published version in hex for the BrStatFlt struct
    CREATE_PUBVAR(StatCom,              WORD);          // published version in hex for the StatCom struct
    CREATE_PUBVAR(StatReg21,            WORD);          // published version in hex for the StatReg21 struct

    CREATE_PUBVAR(Sdl21Reg,             WORD);          // published version in hex for the SDL21_ST struct
    CREATE_PUBVAR(Sdl43Reg,             WORD);          // published version in hex for the SDL43_ST struct

    CREATE_PUBVAR(BrgFan1SpdRef,        WORD);          // hex published version of Fan#1 speed reference
    CREATE_PUBVAR(BrgFan2SpdRef,        WORD);          // hex published version of Fan#2 speed reference
    CREATE_PUBVAR(HsslTstCntr,          int);           // hssl test counter
    CREATE_PUBVAR(HsslTstMrg,           float);         // hssl test margin
    CREATE_PUBVAR(HsslTstSgn,           float);         // hssl test sign of margin change (inc vs. dec)

    // enable the following code to check the read back signals
    #if 0
    CREATE_PUBVAR(ReadBack0 , unsigned);
    CREATE_PUBVAR(ReadBack1 , unsigned);
    CREATE_PUBVAR(ReadBack2 , unsigned);
    CREATE_PUBVAR(ReadBack3 , unsigned);
    CREATE_PUBVAR(ReadBack4 , unsigned);
    CREATE_PUBVAR(ReadBack5 , unsigned);
    CREATE_PUBVAR(ReadBack6 , unsigned);
    CREATE_PUBVAR(ReadBack7 , unsigned);

    CREATE_PUBVAR(CfgVa0,     unsigned);
    CREATE_PUBVAR(CfgVa1,     unsigned);
    CREATE_PUBVAR(CfgVa2,     unsigned);
    CREATE_PUBVAR(CfgVa3,     unsigned);
    CREATE_PUBVAR(CfgVa4,     unsigned);
    CREATE_PUBVAR(CfgVa5,     unsigned);
    CREATE_PUBVAR(CfgVa6,     unsigned);
    CREATE_PUBVAR(CfgVa7,     unsigned);
    #endif

#if REBx_DIAG_FPGA_REGISTERS
    CREATE_PUBVAR(StatComReg,           WORD);          // bridge status common register from fpga (bit coded)
    CREATE_PUBVAR(BrStatFltReg,         WORD);          // bridge fault status register from fpga (bit coded)
    CREATE_PUBVAR(StatReg21Reg,         WORD);          // status register 1 and 2 from fpga (bit coded)
    CREATE_PUBVAR(FbkReserveReg,        WORD);          // fpga internal status bits subject to change (bit coded)
    CREATE_PUBVAR(MiscStatReg,          WORD);          // misc status register (bit coded)
    CREATE_PUBVAR(PktCntRbReg,          WORD);          // packet counter read back register
    CREATE_PUBVAR(RevCodeReg,           WORD);          // revison code register
    CREATE_PUBVAR(Cr21Reg,              WORD);          // bridge command control registers 1 and 2 to fpga(bit coded)
    CREATE_PUBVAR(CrCfgReg,             WORD);          // bridge command configuration register (bit coded)
    CREATE_PUBVAR(LoadPulseTimeReg,     WORD);          // load pulse time register
    CREATE_PUBVAR(FrameFrc16,           WORD);
#endif

CREATE_PUBVAR(PktCntRbReg,          WORD);          // packet counter read back register
CREATE_PUBVAR(SdlSTReg,             WORD);          // packet counter read back register


#if REBx_DIAG_BRIDGE_EVENTS
    CREATE_PUBVAR(Gate1AEvtAr1Enb   ,  unsigned);
    CREATE_PUBVAR(Gate1AEvtAr1RelTm ,  unsigned);
    CREATE_PUBVAR(Gate1AEvtAr1State ,  unsigned);
    CREATE_PUBVAR(Gate2AEvtAr1Enb   ,  unsigned);
    CREATE_PUBVAR(Gate2AEvtAr1RelTm ,  unsigned);
    CREATE_PUBVAR(Gate2AEvtAr1State ,  unsigned);
    CREATE_PUBVAR(Gate3AEvtAr1Enb   ,  unsigned);
    CREATE_PUBVAR(Gate3AEvtAr1RelTm ,  unsigned);
    CREATE_PUBVAR(Gate3AEvtAr1State ,  unsigned);

    CREATE_PUBVAR(Gate1BEvtAr1Enb   ,  unsigned);
    CREATE_PUBVAR(Gate1BEvtAr1RelTm ,  unsigned);
    CREATE_PUBVAR(Gate1BEvtAr1State ,  unsigned);
    CREATE_PUBVAR(Gate2BEvtAr1Enb   ,  unsigned);
    CREATE_PUBVAR(Gate2BEvtAr1RelTm ,  unsigned);
    CREATE_PUBVAR(Gate2BEvtAr1State ,  unsigned);
    CREATE_PUBVAR(Gate3BEvtAr1Enb   ,  unsigned);
    CREATE_PUBVAR(Gate3BEvtAr1RelTm ,  unsigned);
    CREATE_PUBVAR(Gate3BEvtAr1State ,  unsigned);

    CREATE_PUBVAR(Gate1CEvtAr1Enb   ,  unsigned);
    CREATE_PUBVAR(Gate1CEvtAr1RelTm ,  unsigned);
    CREATE_PUBVAR(Gate1CEvtAr1State ,  unsigned);
    CREATE_PUBVAR(Gate2CEvtAr1Enb   ,  unsigned);
    CREATE_PUBVAR(Gate2CEvtAr1RelTm ,  unsigned);
    CREATE_PUBVAR(Gate2CEvtAr1State ,  unsigned);
    CREATE_PUBVAR(Gate3CEvtAr1Enb   ,  unsigned);
    CREATE_PUBVAR(Gate3CEvtAr1RelTm ,  unsigned);
    CREATE_PUBVAR(Gate3CEvtAr1State ,  unsigned);
#endif

    // AEAD calibration
    CREATE_PUBVAR(Io1VanPFrq,          float);              // Io1 phase A voltage VCO positive frequency (Hz)
    CREATE_PUBVAR(Io1VanNFrq,          float);              // Io1 phase B voltage VCO positive frequency (Hz)
    CREATE_PUBVAR(Io1VbnPFrq,          float);              // Io1 phase C voltage VCO positive frequency (Hz)
    CREATE_PUBVAR(Io1VbnNFrq,          float);              // Io1 phase A voltage VCO negative frequency (Hz)
    CREATE_PUBVAR(Io1VcnPFrq,          float);              // Io1 phase B voltage VCO negative frequency (Hz)
    CREATE_PUBVAR(Io1VcnNFrq,          float);              // Io1 phase C voltage VCO negative frequency (Hz)
    CREATE_PUBVAR(Io1IaPFrq,           float);              // Io1 phase A current VCO positive frequency (Hz)
    CREATE_PUBVAR(Io1IaNFrq,           float);              // Io1 phase B current VCO positive frequency (Hz)
    CREATE_PUBVAR(Io1IbPFrq,           float);              // Io1 phase C current VCO positive frequency (Hz)
    CREATE_PUBVAR(Io1IbNFrq,           float);              // Io1 phase A current VCO negative frequency (Hz)
    CREATE_PUBVAR(Io1IcPFrq,           float);              // Io1 phase B current VCO negative frequency (Hz)
    CREATE_PUBVAR(Io1IcNFrq,           float);              // Io1 phase C current VCO negative frequency (Hz)

    CREATE_PUBVAR(Io1PwaOffs2,         float);              // Io1 Aead analog input calibrated offset value for Ain 2 (Hz)
    CREATE_PUBVAR(Io1PwaOffs3,         float);              // Io1 Aead analog input calibrated offset value for Ain 3 (Hz)
    CREATE_PUBVAR(Io1PwaOffs4,         float);              // Io1 Aead analog input calibrated offset value for Ain 4 (Hz)
    CREATE_PUBVAR(Io1PwaOffs5,         float);              // Io1 Aead analog input calibrated offset value for Ain 5 (Hz)
    CREATE_PUBVAR(Io1PwaOffs6,         float);              // Io1 Aead analog input calibrated offset value for Ain 6 (Hz)
    CREATE_PUBVAR(Io1PwaOffs7,         float);              // Io1 Aead analog input calibrated offset value for Ain 7 (Hz)

    CREATE_PUBVAR(Io1AeadOffsFlt,      unsigned);           // Io1 Aead analog offset fault
    CREATE_PUBVAR(Io1AeadDsrFlt,       unsigned);           // Io1 Aead Pwa design records bad
    CREATE_PUBVAR(Io1AeadGainFlt,      unsigned);           // Io1 Aead Pwa board ID gain fault

    CREATE_PUBVAR(Io1PwaVanSc,         float);              // Io 1 calibrated phase A voltages Scales (volts/hz)
    CREATE_PUBVAR(Io1PwaVbnSc,         float);              // Io 1 calibrated phase B voltages Scales (volts/hz)
    CREATE_PUBVAR(Io1PwaVcnSc,         float);              // Io 1 calibrated phase C voltages Scales (volts/hz)
    CREATE_PUBVAR(Io1PwaIaScl,         float);              // Io 1 calibrated phase A currents Scales (volts/hz)
    CREATE_PUBVAR(Io1PwaIbScl,         float);              // Io 1 calibrated phase B currents Scales (volts/hz)
    CREATE_PUBVAR(Io1PwaIcScl,         float);              // Io 1 calibrated phase C currents Scales (volts/hz)

    CREATE_PUBVAR(Io1VanScl,           float);              // Io 1 phase A voltages Scales (volts/hz)
    CREATE_PUBVAR(Io1VbnScl,           float);              // Io 1 phase B voltages Scales (volts/hz)
    CREATE_PUBVAR(Io1VcnScl,           float);              // Io 1 phase C voltages Scales (volts/hz)
    CREATE_PUBVAR(Io1IaScl,            float);              // Io 1 phase A currents Scales (volts/hz)
    CREATE_PUBVAR(Io1IbScl,            float);              // Io 1 phase B currents Scales (volts/hz)
    CREATE_PUBVAR(Io1IcScl,            float);              // Io 1 phase C currents Scales (volts/hz)

    CREATE_PUBVAR(Io1AeaxAin2Zero,     float);              // Io1 Aead analog input calibrated zero frequency value for Ain 2 (Hz)
    CREATE_PUBVAR(Io1AeaxAin3Zero,     float);              // Io1 Aead analog input calibrated zero frequency value for Ain 3 (Hz)
    CREATE_PUBVAR(Io1AeaxAin4Zero,     float);              // Io1 Aead analog input calibrated zero frequency value for Ain 4 (Hz)
    CREATE_PUBVAR(Io1AeaxAin5Zero,     float);              // Io1 Aead analog input calibrated zero frequency value for Ain 5 (Hz)
    CREATE_PUBVAR(Io1AeaxAin6Zero,     float);              // Io1 Aead analog input calibrated zero frequency value for Ain 6 (Hz)
    CREATE_PUBVAR(Io1AeaxAin7Zero,     float);              // Io1 Aead analog input calibrated zero frequency value for Ain 7 (Hz)

    CREATE_PUBVAR(HsslCommFltRstTmr,   float);              // HSSL comm fault reset timer
    CREATE_PUBVAR(Brg1EnabCtr,         unsigned);
    CREATE_PUBVAR(TempVco1stSmpl, unsigned);
    CREATE_PUBVAR(TempVco2ndSmpl, unsigned);
    CREATE_PUBVAR(ThrmstrBrgCnts, unsigned);                 // Bridge thermistor simulation counts (Count)

    CREATE_PUBVAR(IdBufRepiDataLen, int);
    CREATE_PUBVAR(PlugInfo, WORD);
    CREATE_PUBVAR(PlugInfo2, WORD);
    CREATE_PUBVAR(PlugInfo3, WORD);
    CREATE_PUBVAR(PlugInfo4, WORD);

    CREATE_PUBVAR(PlugInfoL, WORD);
    CREATE_PUBVAR(PlugInfo2L, WORD);
    CREATE_PUBVAR(PlugInfo3L, WORD);
    CREATE_PUBVAR(PlugInfo4L, WORD);





    // Data Passing
    //-------------
    DATA_PASS(HsslErrFlagsT3,       HsslErrFlags,     T3_T1, UNFILTERED);
    DATA_PASS(CtlPwrOk,             CtlPwrOkT2,       T1_T2, FILTERED);
    DATA_PASS(RlyPwrOk,             RlyPwrOkT2,       T1_T2, FILTERED);


    // Unpublished Variables
    //----------------------
    LinkInfoTyp             LinkInfo;                   // link information
    WinInfoTyp              WinInfo[REBF_NUM_OF_WIN];   // window information

    uint16_t                IdBufAux1[Hssl_MAX_PACKET_SIZE/2];  // auxiliary board id 1 buffer
    uint16_t                IdBufAux2[Hssl_MAX_PACKET_SIZE/2];  // auxiliary board id 2 buffer
    uint16_t                IdBufRepi[Hssl_MAX_PACKET_SIZE/2];  // auxiliary board id 1 buffer for REBH
    uint16_t                IdBufRgib[Hssl_MAX_PACKET_SIZE/2];  // auxiliary board id 2 buffer for REBH
    uint16_t                IdBufRgib2[Hssl_MAX_PACKET_SIZE/2];  // auxiliary board id 2 buffer for REBH
    uint16_t                IdBufRgib3[Hssl_MAX_PACKET_SIZE/2];  // auxiliary board id 2 buffer for REBH
    uint16_t                IdBufRgib4[Hssl_MAX_PACKET_SIZE/2];  // auxiliary board id 2 buffer for REBH

    uint16_t                IdBufPlug;  // auxiliary board id 2 buffer for REBH
    uint16_t                PlugInfoArr1[4];
    uint16_t                PlugInfoArr2[3];
    uint16_t                PlugInfoArr3[4];
    uint16_t                PlugInfoArr4[3];


    uint16_t                SBWBytes[1];
    uint8_t                 PlugLoc[1];

    int                     IdBufAux1DataLen;
    int                     IdBufAux2DataLen;
    //int                     IdBufRepiDataLen;
    int                     IdBufRgibDataLen;
    int                     IdBufRgib2DataLen;
    int                     IdBufRgib3DataLen;
    int                     IdBufRgib4DataLen;

    REBx_CfgValTyp          CfgVal;

    BrgTransListTyp         Brg1TransList;
    BrgTransTyp             Brg1TransArr[REBx_NUM_OF_GATE_CMD_EVTS];

    REBx_CmdWinTyp          BrgCmd;                     // command buffer used by control (hssl dma source)
    off_t                   BrgCmdAddr;

    CREATE_COMPLEX_PUBVAR(BrgFbk, REBx_FbkWinTyp);      // feedback buffer used by control (hssl dma target normal, simulator target in sim mode)

    //REBx_FbkWinTyp          BrgFbk;                     // feedback buffer used by control (hssl dma target normal, simulator target in sim mode)
    off_t                   BrgFbkAddr;

    REBx_FbkWinTyp          BrgFbkAlt;                  // alternate feedback buffer (hssl dma target in sim mode so as to not overwrite simulated fbks)
    off_t                   BrgFbkAltAddr;

    VcoSampBrgHdwTyp        VcoSampBrg1HdwScl;
    VcoSampIo1HdwTyp        VcoSampIo1HdwScl;

    BrgHdwOffsTyp           Brg1HdwInOffs;
    BrgHdwOffsTyp           Brg1HdwOffs;

    BrgHdwOffsTyp           Io1HdwInOffs;
    BrgHdwOffsTyp           Io1HdwOffs;

    REBx_VcoBr1Typ          VcoBr1Z1;
    REBx_VcoIo1Typ          VcoIo1Z1;
    REBx_VcoVdbTyp          VcoVdbZ1;
    REBx_VdcSDL5_STTyp      VdcSDL5_STZ1;

    bool                    HsslIsEnabled;
    bool                    HostIsOpen;

    SysTm_Typ               HsslTimer;

    uint64_t                HsslFbkWaitStrt;

    unsigned                TargReprogCtr;

    unsigned                CfgReadBack[REBx_NUM_OF_CONFIG_REGS];

    cDString                TargFpgaVer;

    cBridgeInterface        BridgeInterface;
    cHssl                   Hssl;
    cBrgSim                 BrgSim;

    bool                    FbkCalcFstpDone;

    unsigned                ContactIn6Z;

    unsigned                BrgFaultActZ;       // used to detect transition of bridge fault false -> true
    unsigned                BrgFaultResolved;   // used to determine if the general bridge fault was resolved to specific

    unsigned                HwFdbkTxDlyFrc;

    volatile bool           LlmCmdShdwCapReq;
    REBx_CmdWinTyp          LlmCmdShdw;

    volatile bool           LlmFbkShdwCapReq;
    REBx_FbkWinTyp          LlmFbkShdw;

    int                     ErrCode;

    REBx_BrStatFltTyp       StatIocFltMskBr1;
    REBx_BrStatFltTyp       StatIocFltMskBr2;

    REBx_BrStatFltTyp       DesatFltMskBr1;
    REBx_BrStatFltTyp       DesatFltMskBr2;

    unsigned                Io1ContInp03Latch;

    // residualization variables for float-to-vco conversion
    unsigned                IaVcoBr1LatchCnt;
    unsigned                IbVcoBr1LatchCnt;
    unsigned                IcVcoBr1LatchCnt;
    unsigned                IdVcoBr1LatchCnt;

    unsigned                VaxVcoBr1LatchCnt;
    unsigned                VbxVcoBr1LatchCnt;
    unsigned                VcxVcoBr1LatchCnt;
    unsigned                VdcVcoBr1LatchCnt;

    unsigned                IaVcoBr2LatchCnt;
    unsigned                IbVcoBr2LatchCnt;
    unsigned                IcVcoBr2LatchCnt;

    unsigned                VaxVcoBr2LatchCnt;
    unsigned                VbxVcoBr2LatchCnt;
    unsigned                VcxVcoBr2LatchCnt;
    unsigned                VdcVcoBr2LatchCnt;

    unsigned                VdcFiberVcoLatchCnt;

    unsigned                IaVcoAin1LatchCnt;
    unsigned                IbVcoAin1LatchCnt;
    unsigned                IcVcoAin1LatchCnt;

    unsigned                VaxVcoAin1LatchCnt;
    unsigned                VbxVcoAin1LatchCnt;
    unsigned                VcxVcoAin1LatchCnt;

    unsigned                IaVcoAin2LatchCnt;
    unsigned                IbVcoAin2LatchCnt;
    unsigned                IcVcoAin2LatchCnt;

    unsigned                VaxVcoAin2LatchCnt;
    unsigned                VbxVcoAin2LatchCnt;
    unsigned                VcxVcoAin2LatchCnt;

    float                   IaVcoBr1DelCntRes;
    float                   IbVcoBr1DelCntRes;
    float                   IcVcoBr1DelCntRes;
    float                   IdVcoBr1DelCntRes;

    float                   VaxVcoBr1DelCntRes;
    float                   VbxVcoBr1DelCntRes;
    float                   VcxVcoBr1DelCntRes;
    float                   VdcVcoBr1DelCntRes;

    float                   IaVcoBr2DelCntRes;
    float                   IbVcoBr2DelCntRes;
    float                   IcVcoBr2DelCntRes;

    float                   VaxVcoBr2DelCntRes;
    float                   VbxVcoBr2DelCntRes;
    float                   VcxVcoBr2DelCntRes;
    float                   VdcVcoBr2DelCntRes;

    float                   VdcFiberVcoDelCntRes;

    float                   IaVcoAin1DelCntRes;
    float                   IbVcoAin1DelCntRes;
    float                   IcVcoAin1DelCntRes;

    float                   VaxVcoAin1DelCntRes;
    float                   VbxVcoAin1DelCntRes;
    float                   VcxVcoAin1DelCntRes;

    float                   IaVcoAin2DelCntRes;
    float                   IbVcoAin2DelCntRes;
    float                   IcVcoAin2DelCntRes;

    float                   VaxVcoAin2DelCntRes;
    float                   VbxVcoAin2DelCntRes;
    float                   VcxVcoAin2DelCntRes;

    float                   SclLinkVoltsBrg1Rcp;
    float                   SclPhsVoltsBrg1Rcp;
    float                   SclPhsCurBrg1Rcp;

    float                   SclPhsVoltsIo1Rcp;
    float                   SclPhsCurIo1Rcp;

    float                   TempVcoCalTm;                   // Delta time for temperature Vco frq calculation
    float                   PwmFullRcp;
    unsigned                PwmFull;
    int                     T1perT3;                        // Number of task1s in a task3

    unsigned                TargInitCnt;
    unsigned                TargInitCnt2;
    unsigned                TargSdlNum;
    unsigned                SdlIdDone;

    bool                    TaskBkgndRst;

    const char*             Reqd_HslaConn_Str;              // required HSLA connector name (REBx location)

    const char**            Reqd_Brg1Infc_CatNum_Str;       // pointer to array of char* that point to BRG1 interface required catalog numbers
    int                     Reqd_Brg1Infc_CatNum_ArrLen;    // length of pointer array pointed to by Reqd_Brg1Infc_CatNum_Str

    const char**            Reqd_Io1Infc_CatNum_Str;        // pointer to array of char* that point to IO1 interface required catalog numbers
    int                     Reqd_Io1Infc_CatNum_ArrLen;     // length of pointer array pointed to by Reqd_Io1Infc_CatNum_Str

    int                     TempVco1stCnt;                  // Thermistor simulation 1st counter
    int                     TempVco2ndCnt;                  // Thermistor simulation 2nd counter
    unsigned                ThrmCoolntLwCntsZ1;             // Simulated coolant thermistor low-range count delayed (Count)
    unsigned                ThrmCoolntHiCntsZ1;             // Simulated coolant thermistor high-range count delayed (Count)
    unsigned                ThrmHxinLwCntsZ1;               // Simulated heat-exchanger inlet thermistor low-range count delayed (Count)
    unsigned                ThrmHxinHiCntsZ1;               // Simulated heat-exchanger inlet thermistor high-range count delayed (Count)
    unsigned                ThrmHiSelSim;                   // Simulated Low/High range selection

    bool                    ClearHwInventoryReq;            // request to clear the hardware inventory, only set when link is lost

    std::string                Uniq;                        // class instance uniquifier string

    unsigned                InstNumber;         // holds unique number for each instance of this class
    static unsigned         TotalInstances;     // total number of instances of this class

    static unsigned         IoThrmstrCnts1Sim;  // Io thermistor delta-counts 1 for nominal fbk simulation
    static unsigned         IoThrmstrCnts2Sim;  // Io thermistor delta-counts 2 for nominal fbk simulation

#define REBx_DIAG_BUF_NUM   ( 2 )                   // # of diagnostic buffers (= 2 for double buffering)
#define REBx_DIAG_FRM_NUM   ( ((20000/200)+1) * 6 ) // # of diagnostic frames  (= task1 frames to cover 6 cycles, assume 50hz & 200us task 1)
    REBx_DiagWinTyp         BrgDiagArr          [REBx_DIAG_BUF_NUM][REBx_DIAG_FRM_NUM];
    off_t                   BrgDiagArrAddr      [REBx_DIAG_BUF_NUM][REBx_DIAG_FRM_NUM];

    static const float      FrcClk;
    static const unsigned   FrcSchedMax;
    static const unsigned   FrcSchedMask;

    static const float      VfbOffset;

    static const double     VcoFzro;
    static const double     VcoFmax;
    static const double     VcoFmin;
    static const unsigned   VcoMask;

    static const unsigned   FrcSchedMask16;

    static const double     Sinc3Fs;

    static const char*      HslaH1ApplFile;
    static const char*      HslaH1ApplVer;
    static const unsigned   HslaH1ApplRevCode;

    static const char*      HslApplFile;
    static const char*      HslApplVer;
    static const unsigned   HslApplRevCode;

    static const size_t     CmdReadBackOffs[REBx_NUM_OF_CONFIG_REGS];

    static const BilCalcTyp BilCalc_LockOutTm;

    static const char*      NULL_Str;

    static const char*      J1_Str;
    static const char*      J2_Str;

    static const char*      P1_Str;
    static const char*      P2_Str;
    static const char*      REPI_Str;

    static const char*      J01_Str;
    static const char*      J02_Str;
    static const char*      J03_Str;
    static const char*      J04_Str;
    static const char*      J06_Str;
    static const char*      J07_Str;
    static const char*      J08_Str;

    static const char*      JA1_Str;
    static const char*      JA2_Str;

    static const char*      Reqd_HSLA_CatNum_Str[];
    static const char*      Reqd_REBx_CatNum_Str[];
    static const char*      Reqd_REBx_CatNum_Str_Pr[];
    static const char*      REBx_CatNum_Str_Pr;

    static const char*      Reqd_REPI_H1_CatNum_Str[];
    static const char*      Reqd_REPI_H1_CatNum_Str_Pr[];
    static const char*      REPI_H1_CatNum_Str_Pr;

    static const char*      Reqd_AEDB_H1_CatNum_Str[];
    static const char*      Reqd_AEDB_H2_CatNum_Str[];
    static const char*      Reqd_AEDB_H3_CatNum_Str[];
    static const char*      Reqd_AEDB_H4_CatNum_Str[];

    static const char*      Reqd_AEBI_H1_CatNum_Str[];
    static const char*      Reqd_AEBI_H2_CatNum_Str[];
    static const char*      Reqd_AEBI_H3_CatNum_Str[];

    static const char*      Reqd_AEAA_H1_CatNum_Str[];
    static const char*      Reqd_AEAA_H2_CatNum_Str[];
    static const char*      Reqd_AEAA_H3_CatNum_Str[];
    static const char*      Reqd_AEAA_H4_CatNum_Str[];

    static const char*      Reqd_AEAD_H1_CatNum_Str[];
    static const char*      Reqd_AEAD_H2_CatNum_Str[];
    static const char*      Reqd_AEAD_H3_CatNum_Str[];
    static const char*      Reqd_AEAD_H4_CatNum_Str[];

    static const char*      Reqd_RGIB_H1_CatNum_Str[];

    static const char*      FbkWinRegNames[];
    static const char*      CmdWinRegNames[];

    bool                    Io1InfcCardValid;
    bool                    HslaSite_JA1;

};




// Inline Function Definitions
//----------------------------

//-----------------------------------------------------------------------------
inline float cBridgeRebf::WhatIsFrcFreq(void) const
{
    return FrcClk;
}

//-----------------------------------------------------------------------------
inline unsigned cBridgeRebf::WhatIsFrcSchedMax(void) const
{
    return FrcSchedMax;
}

//-----------------------------------------------------------------------------
inline unsigned cBridgeRebf::WhatIsFrcSchedMask(void) const
{
    return FrcSchedMask;
}

//-----------------------------------------------------------------------------
// Retrieve bridge vco maximum frequency
inline double   cBridgeRebf::WhatIsVcoFmax(void) const
{
    return VcoFmax;
}

//-----------------------------------------------------------------------------
// Retrieve bridge vco minimum frequency
inline double   cBridgeRebf::WhatIsVcoFmin(void) const
{
    return VcoFmin;
}

//-----------------------------------------------------------------------------
// Retrieve bridge vco mask
inline unsigned cBridgeRebf::WhatIsVcoMask(void) const
{
    return VcoMask;
}

//-----------------------------------------------------------------------------
// Retrieve bridge vco zero frequency for Voltage
inline double   cBridgeRebf::WhatIsVcoFzroVoltage(unsigned bridge) const
{
    //if (bridge == 1)
        return VcoBiasFreqBrg1;
}

//-----------------------------------------------------------------------------
// Retrieve bridge vco zero frequency for Current
inline double   cBridgeRebf::WhatIsVcoFzroCurrent(unsigned bridge) const
{
    if (bridge == 1)
        return VcoFzro;
    else
        return VcoFzro;
}

//-----------------------------------------------------------------------------
// Retrieve feedback phase-voltage scale for bridge 1
inline float    cBridgeRebf::WhatIsSclPhaseVolts(unsigned bridge) const
{
    //if (bridge == 1)
        return SclPhsVoltsBrg1;
}

//-----------------------------------------------------------------------------
// Retrieve feedback current scale
inline float    cBridgeRebf::WhatIsSclCurrent(unsigned bridge) const
{
    //if (bridge == 1)
        return SclPhsCurBrg1;
}

//-----------------------------------------------------------------------------
// Retrieve feedback current ioc level
inline float    cBridgeRebf::WhatIsIocCurrent(unsigned bridge) const
{
    //if (bridge == 1)
        return (CurSensorSclBrg1 * PARM(ShuntTrpLvlBrg1));
}

//-----------------------------------------------------------------------------
// Retrieve Io phase voltages Scales
inline float    cBridgeRebf::WhatIsSclIOVoltage(unsigned IO) const
{
    //if (IO == 1)
        return SclPhsVoltsIo1;
}

//-----------------------------------------------------------------------------
// Retrieve Io phase voltages feedback quantization
inline float cBridgeRebf::WhatIsIOVoltageQuant(unsigned IO) const
{
    //if (IO == 1)
        return (SclPhsVoltsIo1*INS(DelTm1Rcp));
}

//-----------------------------------------------------------------------------
// Retrieve Io phase current Scales
inline float    cBridgeRebf::WhatIsSclIOCurrent(unsigned IO) const
{
    //if (IO == 1)
        return SclPhsCurIo1;
}

//-----------------------------------------------------------------------------
// Retrieve bridge vco zero frequency for Link Volt
inline double   cBridgeRebf::WhatIsVcoFzroIoVolt(unsigned IO) const
{
    //if (IO == 1)
        return IO_VCO_FZRO;
}

//-----------------------------------------------------------------------------
// Retrieve link voltage feedback quantization
inline float cBridgeRebf::WhatIsVdcQuant(unsigned bridge) const
{
    //if (bridge == 1)
        return (SclLinkVoltsBrg1*INS(DelTm1Rcp));
}

//-----------------------------------------------------------------------------
// Retrieve link voltage feedback scaling
inline float cBridgeRebf::WhatIsVdcScale(unsigned bridge) const
{
    //if (bridge == 1)
        return (SclLinkVoltsBrg1);
}

//-----------------------------------------------------------------------------
// Retrieve host interface status
inline bool cBridgeRebf::IsHostOpen(void) const
{
    return HostIsOpen;
}



#endif  // __B0_BRIDGE_REBF_H__
