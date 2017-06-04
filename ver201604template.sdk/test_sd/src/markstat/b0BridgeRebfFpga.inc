///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       REBx FPGA Register Map
//
// DESCRIPTION:
//      Provides the data structure definitions that overlay the
//      REBx fpga register map.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      \\varoafc01psge\Electronics\Design Folders\HSLA\WindEnergy
//      Design_doc_HSLA_FPGA_for_REBx#B_HSSL4p48.doc
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __B0_BRIDGE_REBF_FPGA_INC__
#define __B0_BRIDGE_REBF_FPGA_INC__


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      REBx hssl window definition files.
//      These are auto-generated files from the REBx fpga build process.
//
///////////////////////////////////////////////////////////////////////////////
#include "b0BridgeRebxFpgaCfg.inc"      // configuration window
#include "b0BridgeRebxFpgaCtrl.inc"     // control window
#include "b0BridgeRebxFpgaFdbk.inc"     // feedback window



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Software definitions to support the REBx hssl command window
//
///////////////////////////////////////////////////////////////////////////////
#define REBx_NUM_OF_PHASES              ( 3 )                   // number of phases
#define REBx_NUM_OF_GATE_CMD_EVTS       ( 14 )                  // number of gate command event registers
#define REBx_NUM_OF_TRA_PER_PHS         ( 4 )                   // number of phase transitions per phase (includes contingency transitions for 2nd load pulse)
#define REBx_CHOP_NUM_OF_TRA_PER_PHS    ( 3 )                   // number of Dc-chopper phase transitions per phase (includes contingency transitions for 2nd load pulse)
#define REBx_NUM_OF_CONFIG_REGS         ( 8 )                   // number of configuration registers (for loopback)
#define REBx_NUM_OF_CHOP_PHS            ( 1 )                   // number of configuration registers (for loopback)
#define REBx_GATE_EVT_FRC_FS            ( 0x3FFFF )             // full scale gate event frc, based on scaling of GateEventXXReg.Time

#define REBx_Enab_Abc_Ioc_Mask_Br1      ( 0x0000000e )          // sets XiocEnbBr1,  X= A,B,C
#define REBx_Disab_Abc_Ioc_Mask_Br1     ( 0xfffffff1 )          // clears XiocEnbBr1  (see FanRefTyp defn below)

#define REBx_Enab_Abc_Ioc_Mask_Br2      ( 0x000000e0 )          // sets XiocEnbBr2,  X= A,B,C
#define REBx_Disab_Abc_Ioc_Mask_Br2     ( 0xffffff1f )          // clears XiocEnbBr2  (see FanRefTyp defn below)



union REBx_CfgValTyp
{
    struct
    {   // configuration values sent to fpga one at a time
        REBx_DiagMuxSelTyp           DiagMuxSel;
        REBx_CcfgTyp                 Ccfg;
        REBx_CrBrEnb1Typ             CrBrEnb1;
        REBxB_spareLocX006Typ        spareLocX006;
        REBx_DeadTimeTyp             DeadTime;
        REBx_DbOnTyp                 DbOn;
        REBx_CrBrF1Typ               CrBrF1;
        IO_ThresholdTyp              IO_Threshold;
    };
    unsigned    Val[REBx_NUM_OF_CONFIG_REGS];
};


//------------------------------------------------------------------------------------------------------------
struct  REBx_CmdWinTyp
//------------------------------------------------------------------------------------------------------------
{
    REBx_PktCntTyp           PktCnt;                                                          // offset 0x0000
    REBx_LoadPulseRegTyp     LoadPulseTimeReg;                                                // offset 0x0004
    REBx_Brg1PhsTraRegA0Typ  PhsGateEvtRegArr1[REBx_NUM_OF_TRA_PER_PHS][NUM_OF_PHASES];       // offset 0x0008 - Accommodate 4x3 table of transitions
    REBx_Brg1PhsTraRegD0Typ  PhsGateEvtRegArr3[REBx_CHOP_NUM_OF_TRA_PER_PHS][REBx_NUM_OF_CHOP_PHS];// offset 0x0038 - Accommodate 3x1 table of transitions
    REBx_SpareLocX044Typ     SpareLocX044;
    REBx_CrCfgTyp            CrCfg;                                                           // offset 0x0080
    REBx_Cr21Typ             Cr21;                                                            // offset 0x0084
    REBx_FanRefTyp           FanRef;                                                          // offset 0x0088
};



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Software definitions to support the REBx hssl feedback window
//
///////////////////////////////////////////////////////////////////////////////
#define REBx_NUM_OF_SINC_SAMPS          ( 9 )                   // number of sinc feedback samples

#define BRG_VCO_FMIN                    ( 0.0e6 )               // Bridge hardware (AEBI,AEDB, etc) VCO min frequency (hz)
#define BRG_VCO_FMAX                    ( 2.0e6 )               // Bridge hardware (AEBI,AEDB, etc) VCO max frequency (hz)
#define BRG_VCO_REBH_FMAX               ( 8.3333e6 )            // Bridge hardware (RGIB) VCO max frequency (hz)
#define BRG_VCO_FZRO                    ( BRG_VCO_FMAX     /2.0 ) // Bridge hardware (AEBI,AEDB, etc) VCO zero frequency (hz)
#define BRG_VCO_REBH_FZRO               ( BRG_VCO_REBH_FMAX/2.0 ) // Bridge hardware (RGIB) VCO zero frequency (hz)

#define BRG_VCO_MASK                    ( 0xFFFF )              // Bridge hardware (AEBI,AEDB, etc) VCO counter mask (16 bits)
#define BRG_VCO_VREF                    (5.0)                   // Bridge hardware (AEBI,AEDB, etc) VCO +-5 V full scale ref
#define REBx_FRC_MASK                   ( 0xFFFF )              // frc counter mask (16 bits)

#define IO_VCO_FMIN                     ( 0.0e6 )               // IO hardware (AEAA, AEAD, etc) VCO min frequency (hz)
#define IO_VCO_FMAX                     ( 2.0e6 )               // IO hardware (AEAA, AEAD, etc) VCO max frequency (hz)
#define IO_VCO_FZRO                     ( IO_VCO_FMAX / 2.0 )   // IO hardware (AEAA, AEAD, etc) VCO zero frequency (hz)
#define IO_VCO_MASK                     ( 0xFFFF )              // IO hardware (AEAA, AEAD, etc) VCO counter mask (16 bits)
#define IO_VCO_VREF                     (5.0)                   // IO hardware (AEAA, AEAD, etc) VCO +-5 V full scale ref

#define BRG_V_CNT_OFFSET1               1872.70F                //Bridge voltage count offset for scaling with RGAB
#define BRG_V_CNT_OFFSET2               2.50F                   //Bridge voltage count offset for scaling w/o  RGAB
#define BRG_I_CNT_OFFSET                5000.0F                 //Bridge voltage count offset


//-------------------------------------------------------------------------------------------------
struct REBx_FbkReserveTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        Reserve1                 :16;  // [0 : 15]
    unsigned        Reserve2                 :16;  // [16 : 31]
};

struct REBx_GateMonDiagTyp
//-------------------------------------------------------------------------------------------------
{
    unsigned        GateMonDiag              :16;  // [0 : 15]
};


//-------------------------------------------------------------------------------------------------
struct REBx_FbkWinTyp
//-------------------------------------------------------------------------------------------------
{
    REBx_RevCodeTyp              RevCode;
    REBx_VcoTemp21Typ            VcoTemp21;
    REBx_VcoBr1Typ               VcoBr1;
    REBx_StatComTyp              StatCom;
    REBx_BrStatFltTyp            BrStatFlt;
    REBx_VcoIo1Typ               VcoIo1;
    REBx_VcoVdbTyp               VcoVdb;
    REBx_StatReg21Typ            StatReg21;
    REBx_TempRegEngUseOnlyTyp    TempRegEngUseOnly;
    REBx_Tach01Typ               Tach01;
    REBx_CrLbTyp                 CrLb;
    REBx_MiscStatTyp             MiscStat;
    REBx_SDL21_STTyp             SDL21_ST;
    REBx_SDL43_STTyp             SDL43_ST;
    REBx_VdcSDL5_STTyp           VdcSDL5_ST;
    REBx_PktCntRbTyp             PktCntRb;
    //subStructure_SDL_STTyp       SdlST;

};

//-------------------------------------------------------------------------------------------------
struct REBx_DiagWinTyp
//-------------------------------------------------------------------------------------------------
{
        unsigned    reserve[128];//    DiagEventRawTyp     EvtRawArr[REBx_NUM_OF_DIAG_EVTS_PER_WIN];
};


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Software definitions to support Isim and Xsim
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// bridge feedback signals that are instanced more than once in the bridge interface
struct Brg_FbkSimTyp
{
    float               Vax;
    float               Vbx;
    float               Vcx;
    float               Ia;
    float               Ib;
    float               Ic;
    float               Vdc;
};
struct IoThrmstr_FbkSimTyp
{
    float               LwRngFrq;
    float               HiRngFrq;
};
// bridge desaturation and over-current reporting structure
struct Brg_FltSimTyp
{
    unsigned  DsatSwAL;        // Desat fault in A lower switch
    unsigned  DsatSwBL;        // Desat fault in B lower switch
    unsigned  DsatSwCL;        // Desat fault in C lower switch
    unsigned  DsatSwAU;        // Desat fault in A upper switch
    unsigned  DsatSwBU;        // Desat fault in B upper switch
    unsigned  DsatSwCU;        // Desat fault in C upper switch
    unsigned  DsatSwDB;        // Desat fault in dynamic break switch
    unsigned  IocPhsA;         // IOC fault in phase A shunt
    unsigned  IocPhsB;         // IOC fault in phase B shunt
    unsigned  IocPhsC;         // IOC fault in phase C shunt
    unsigned  IocPhsDB;        // IOC fault in dynamic break
    unsigned  BrgFlt;
};

struct Brg_CfgSimTyp
{
    unsigned        IlimEnb;        //  : 1= hardware current limit enable. Turns off cells in phase with overcurrent till no overcurrent (or fault).
    unsigned        AiocEnbBr1;     //  : 1= enable Phase A IOC fault (bridge1)
    unsigned        BiocEnbBr1;     //  : 1= enable Phase B IOC fault (bridge1)
    unsigned        CiocEnbBr1;     //  : 1= enable Phase C IOC fault (bridge1)
    unsigned        DiocEnbBr1;     //  : 1= enable DB IOC fault (bridge1)
    unsigned        IlimEnD;        //  : 1= hardware current limit enable. Turns off cells in phase D with overcurrent till no overcurrent (or fault).
    unsigned        FltRst;
};
// bridge feedback signals for a complete bridge interface
struct BrgFbkSimTyp
{
    float               VdcFiber;
    float               Ichpr1;

    float               ThrmstrBrg;
    IoThrmstr_FbkSimTyp ThrmstrCoolnt;

    Brg_FbkSimTyp       Brg1;
    Brg_FbkSimTyp       Ain1;
    Brg_FbkSimTyp       Ain2;

    Brg_FltSimTyp       BrgFlt1;
    unsigned            DriveFlt;
    unsigned            TachV;
    unsigned            TachMv;
};

// bridge fault status signals for a bridge interface
struct BrgFltSimTyp
{
    Brg_FltSimTyp       Brg1;
    Brg_FltSimTyp       Brg2;
};

//-----------------------------------------------------------------------------
// bridge commanded phase transition that are instanced more than once in a bridge interface
struct Brg_CmdTraSimTyp
{
    float               DelTm;
    unsigned            State;
    unsigned            Enable;
};
// bridge commanded phase transitions for a complete bridge interface
struct BrgCmdTraSimTyp
{
    float               DelTm1;

    unsigned            PeEnableBrg1;

    Brg_CmdTraSimTyp    Brg1[REBx_NUM_OF_TRA_PER_PHS][REBx_NUM_OF_PHASES];
    Brg_CmdTraSimTyp    BrgD1[REBx_CHOP_NUM_OF_TRA_PER_PHS][NUM_OF_CHOP_PHS]; // For DC Chopper
    Brg_CfgSimTyp       BrgCfg;
};


//-----------------------------------------------------------------------------
// bridge commanded phase output average voltages that are instanced more than once in a complete bridge interface
struct Brg_CmdVavgSimTyp
{
    float               Va;
    float               Vb;
    float               Vc;
    float               Vdb;
};
// bridge command commanded phase average voltages for a complete bridge interface
struct BrgCmdVavgSimTyp
{
    Brg_CmdVavgSimTyp   Brg1;
};

struct BrgDbCmdSimTyp
{
    unsigned Db1GateCmd;
};


#endif  //  __B0_BRIDGE_REBx_FPGA_INC__
