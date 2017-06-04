///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rebf Based Bridge Interface
//
// DESCRIPTION:
//      This file contains the rules for the bridge interface class.  This file
//      is defined uniquely per product and is included by b0BridgeRebfRule.cpp.
//      The final value parameters and variables calculated here are defined
//      by the cBridgeRebf class in b0BridgeRebf.h.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0BridgeRebf.h
//
///////////////////////////////////////////////////////////////////////////////
#ifndef B0_BRIDGE_REBF_RULE_INC
#define B0_BRIDGE_REBF_RULE_INC

{

    //-------------------------------------------------------------------------
    // manage the hssl/sdl links

    // normally enable the hssl-disable until the x030 class is removed
    SET_FV(HsslEnb, true);
    HsslIsEnabled = (PARM(HsslEnb) != 0);

    // normally disable the interface to the real bridge in sim mode
    SET_FV(SimModeBrgEnb, false);
    SimModeBrgEnabled = PARM(SimModeBrgEnb);
    SET_FV(SimModeSdlEnb, SimModeBrgEnabled ? true : false);
    SimModeSdlEnabled = PARM(SimModeSdlEnb);

    if ( INS(XSimMode) || INS(SimMode) )
    {
        if ( SimModeBrgEnabled )
        {
            TargetHdwExpected = true;
        }
        else
        {
            TargetHdwExpected = false;
        }
        if ( SimModeSdlEnabled )
        {
            TargetSdlExpected = true;
        }
        else
        {
            TargetSdlExpected = false;
        }
    }
    else
    {
        TargetHdwExpected = true;
        TargetSdlExpected = true;
    }

    // hssl target to host packet transmission control (delay time after load pulse)
    SET_FV(HwFdbkTxDlyTm,   0.0e-6);
    SET_FV(HwDiagTxDlyTm,  40.0e-6);
    HwFdbkTxDlyFrc = (unsigned)( PARM(HwFdbkTxDlyTm) * FRC_CLK );

    // management of target (HSLA) fpga
    SET_FV(TargFpgaReprog, false);

    if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_B)
    {
        SET_FV(TargFpgaRvCk,  HslApplRevCode);
    }
    else
    {
        SET_FV(TargFpgaRvCk,  HslaApplRevCode);
    }


    //-------------------------------------------------------------------------
    // configure the bridge interface

    // feedback circuit gains (v/v)

    // DC voltage feedback attenuation
    #define AEBIH1_R_RATIO_VDC          (2375.0/15.9)       // AEBIH1 dc voltage attenuation ratio
    #define AEBIH1_R_RATIO_VDC_ALT      (2375.0/15.9)       // AEBIH1 dc voltage attenuation ratio ALT
    #define AEBIH2_R_RATIO_VDC          (2375.0/15.9)       // AEBIH2 dc voltage attenuation ratio
    #define AEBIH2_R_RATIO_VDC_ALT      (2375.0/15.9)       // AEBIH2 dc voltage attenuation ratio alternate platform
    #define AEBIH3_R_RATIO_VDC          (2375.0/15.9)       // AEBIH3 dc voltage attenuation ratio
    #define AEBIH3_R_RATIO_VDC_ALT      (2375.0/15.9)       // AEBIH3 dc voltage attenuation ratio alternate platform

    #define AEDBH1_R_RATIO_VDC_ALT      (2375.0/15.9)       // AEDBH1 dc voltage attenuation ratio alternate platform
    #define AEDBH1_R_RATIO_VDC          (2375.0/15.9)       // AEDBH1 dc voltage attenuation ratio
    #define AEDBH2_R_RATIO_VDC_ALT      (2375.0/15.9)       // AEDBH2 dc voltage attenuation ratio alternate platform
    #define AEDBH2_R_RATIO_VDC          (2375.0/15.9)       // AEDBH2 dc voltage attenuation ratio
    #define AEDBH3_R_RATIO_VDC_ALT      (2375.0/15.9)       // AEDBH3 dc voltage attenuation ratio alternate platform
    #define AEDBH3_R_RATIO_VDC          (2375.0/15.9)       // AEDBH3 dc voltage attenuation ratio
    #define AEDBH4_R_RATIO_VDC_ALT      (2375.0/15.9)       // AEDBH4 dc voltage attenuation ratio alternate platform
    #define AEDBH4_R_RATIO_VDC          (2375.0/15.9)       // AEDBH4 dc voltage attenuation ratio

    #define RGIBH1_R_RATIO_VDC_ALT      (2375.0/15.9)       // RGIBH1 dc voltage attenuation ratio alternate platform
    #define RGIBH1_R_RATIO_VDC          (2375.0/15.9)       // RGIBH1 dc voltage attenuation ratio


    // Phase voltage feedback attenuation
    #define AEBIH1_R_RATIO_V            ((2375.0+6.9)/6.9)  // AEBIH3 dc voltage attenuation ratio
    #define AEBIH1_R_RATIO_V_ALT        ((2375.0+6.9)/6.9)  // AEBIH3 dc voltage attenuation ratio alternate platform
    #define AEBIH2_R_RATIO_V            ((2375.0+6.9)/6.9)  // AEBIH3 dc voltage attenuation ratio
    #define AEBIH2_R_RATIO_V_ALT        ((2375.0+6.9)/6.9)  // AEBIH3 dc voltage attenuation ratio alternate platform
    #define AEBIH3_R_RATIO_V            ((2375.0+6.9)/6.9)  // AEBIH3 dc voltage attenuation ratio
    #define AEBIH3_R_RATIO_V_ALT        ((2375.0+6.9)/6.9)  // AEBIH3 dc voltage attenuation ratio alternate platform

    #define AEDBH1_R_RATIO_V            ((2375.0+6.9)/6.9)  // AEDBH1 dc voltage attenuation ratio
    #define AEDBH1_R_RATIO_V_ALT        ((2375.0+6.9)/6.9)  // AEDBH1 dc voltage attenuation ratio alternate platform
    #define AEDBH2_R_RATIO_V            ((2375.0+6.9)/6.9)  // AEDBH2 dc voltage attenuation ratio
    #define AEDBH2_R_RATIO_V_ALT        ((2375.0+6.9)/6.9)  // AEDBH2 dc voltage attenuation ratio alternate platform
    #define AEDBH3_R_RATIO_V            ((2375.0+6.9)/6.9)  // AEDBH3 dc voltage attenuation ratio
    #define AEDBH3_R_RATIO_V_ALT        ((2375.0+6.9)/6.9)  // AEDBH3 dc voltage attenuation ratio alternate platform
    #define AEDBH4_R_RATIO_V            ((2375.0+6.9)/6.9)  // AEDBH4 dc voltage attenuation ratio
    #define AEDBH4_R_RATIO_V_ALT        ((2375.0+6.9)/6.9)  // AEDBH4 dc voltage attenuation ratio alternate platform

    #define RGIBH1_R_RATIO_V            ((2375.0+6.9)/6.9)  // RGIBH1 dc voltage attenuation ratio
    #define RGIBH1_R_RATIO_V_ALT        ((2375.0+6.9)/6.9)  // RGIBH1 dc voltage attenuation ratio alternate platform


    #define BRIDGE_V_FBIAS              (((5.0/20475.0)/0.0005)*4000.0/2.0*1000.0)  //Bridge voltage VCO zero volts frequency

    // Current feedback gains
    #define AEBIH1_AV_GAIN_I            (516.0/10316.0)     // AEBIH1 current feedback gain(inverse)
    #define AEBIH1_AV_GAIN_I_ALT        (516.0/10316.0)     // AEBIH1 current feedback gain(inverse) for alternate platform
    #define AEBIH2_AV_GAIN_I            (559.0/22360.0)     // AEBIH2 current feedback gain(inverse)
    #define AEBIH2_AV_GAIN_I_ALT        (559.0/22630.0)     // AEBIH2 current feedback gain(inverse) for alternate platform
    #define AEBIH3_AV_GAIN_I            (516.0/7480.0)      // AEBIH3 current feedback gain(inverse)
    #define AEBIH3_AV_GAIN_I_ALT        (516.0/7480.0)      // AEBIH3 current feedback gain(inverse) for alternate platform

    #define AEDBH1_AV_GAIN_I            (516.0/10316.0)     // AEDBH1 current feedback gain(inverse)
    #define AEDBH1_AV_GAIN_I_ALT        (516.0/10316.0)     // AEDBH1 current feedback gain(inverse) for alternate platform
    #define AEDBH2_AV_GAIN_I            (559.0/22360.0)     // AEDBH2 current feedback gain(inverse)
    #define AEDBH2_AV_GAIN_I_ALT        (559.0/22360.0)     // AEDBH2 current feedback gain(inverse) for alternate platform
    #define AEDBH3_AV_GAIN_I            (516.0/17110.0)     // AEDBH3 current feedback gain(inverse)
    #define AEDBH3_AV_GAIN_I_ALT        (516.0/17110.0)     // AEDBH3 current feedback gain(inverse) for alternate platform
    #define AEDBH4_AV_GAIN_I            (516.0/7480.0)      // AEDBH4 current feedback gain(inverse)
    #define AEDBH4_AV_GAIN_I_ALT        (516.0/7480.0)      // AEDBH4 current feedback gain(inverse) for alternate platform

    #define RGIBH1_AV_GAIN_I            (516.0/7480.0)      // RGIBH1 current feedback gain(inverse)
    #define RGIBH1_AV_GAIN_I_ALT        (516.0/7480.0)      // RGIBH1 current feedback gain(inverse) for alternate platform


    // Shunt Ioc trip levels
    #define AEBIH1_TRIP_LEVEL_ALT        (0.250)            // AEBIH1 shunt Ioc trip level for alternate platform
    #define AEBIH1_TRIP_LEVEL            (0.250)            // AEBIH1 shunt Ioc trip level
    #define AEBIH2_TRIP_LEVEL_ALT        (0.125)            // AEBIH2 shunt Ioc trip level for alternate platform
    #define AEBIH2_TRIP_LEVEL            (0.125)            // AEBIH2 shunt Ioc trip level
    #define AEBIH3_TRIP_LEVEL_ALT        (0.250)            // AEBIH3 shunt Ioc trip level for alternate platform
    #define AEBIH3_TRIP_LEVEL            (0.250)            // AEBIH3 shunt Ioc trip level

    #define AEDBH1_TRIP_LEVEL_ALT        (0.250)            // AEDBH1 shunt Ioc trip level for alternate platform
    #define AEDBH1_TRIP_LEVEL            (0.250)            // AEDBH1 shunt Ioc trip level
    #define AEDBH2_TRIP_LEVEL_ALT        (0.125)            // AEDBH2 shunt Ioc trip level for alternate platform
    #define AEDBH2_TRIP_LEVEL            (0.125)            // AEDBH2 shunt Ioc trip level
    #define AEDBH3_TRIP_LEVEL_ALT        (0.125)            // AEDBH3 shunt Ioc trip level for alternate platform
    #define AEDBH3_TRIP_LEVEL            (0.125)            // AEDBH3 shunt Ioc trip level
    #define AEDBH4_TRIP_LEVEL_ALT        (0.250)            // AEDBH4 shunt Ioc trip level for alternate platform
    #define AEDBH4_TRIP_LEVEL            (0.250)            // AEDBH4 shunt Ioc trip level

    #define RGIBH1_TRIP_LEVEL_ALT        (0.250)            // RGIBH1 shunt Ioc trip level for alternate platform
    #define RGIBH1_TRIP_LEVEL            (0.250)            // RGIBH1 shunt Ioc trip level



    //  Current/Voltage Scaling
    //-----------------------------------
    CurSensorSclBrg1 = INS(ShuntRatBrg1) / (50e-3);        // full scale current at 50 mV
    CurSensorSclPhD  = INS(ShuntRatPhD)  / (50e-3);        // full scale current at 50 mV

    Brg1InfcBoardAedb = false;
    // Bridge 1 scaling
    //-----------------------------------
    if (INS(Brg1InfcHdw) != BRG_INFC_NONE)
    {
        switch ((BrgInfcHdwTyp)(INS(Brg1InfcHdw)))
        {
            case BRG_INFC_AEBIH1:
                // Bridge 1 AEBIH1
                if (INS(AltCfg))
                {
                    SET_FV(CurSensorGnBrg1, AEBIH1_AV_GAIN_I_ALT);
                    SET_FV(VdcAttenBrg1, AEBIH1_R_RATIO_VDC_ALT);
                    SET_FV(VphAttenBrg1, AEBIH1_R_RATIO_V_ALT);
                    SET_FV(ShuntTrpLvlBrg1, AEBIH1_TRIP_LEVEL_ALT);
                }
                else
                {
                    SET_FV(CurSensorGnBrg1, AEBIH1_AV_GAIN_I);
                    SET_FV(VdcAttenBrg1, AEBIH1_R_RATIO_VDC);
                    SET_FV(VphAttenBrg1, AEBIH1_R_RATIO_V);
                    SET_FV(ShuntTrpLvlBrg1, AEBIH1_TRIP_LEVEL);
                }
                break;
            case BRG_INFC_AEBIH2:
                // Bridge 1 AEBIH2
                if (INS(AltCfg))
                {
                    SET_FV(CurSensorGnBrg1, AEBIH2_AV_GAIN_I_ALT);
                    SET_FV(VdcAttenBrg1, AEBIH2_R_RATIO_VDC_ALT);
                    SET_FV(VphAttenBrg1, AEBIH2_R_RATIO_V_ALT);
                    SET_FV(ShuntTrpLvlBrg1, AEBIH2_TRIP_LEVEL_ALT);
                }
                else
                {
                    SET_FV(CurSensorGnBrg1, AEBIH2_AV_GAIN_I);
                    SET_FV(VdcAttenBrg1, AEBIH2_R_RATIO_VDC);
                    SET_FV(VphAttenBrg1, AEBIH2_R_RATIO_V);
                    SET_FV(ShuntTrpLvlBrg1, AEBIH2_TRIP_LEVEL);
                }
                break;
            case BRG_INFC_AEBIH3:
                // Bridge 1 AEBIH3
                if (INS(AltCfg))
                {
                    SET_FV(CurSensorGnBrg1, AEBIH3_AV_GAIN_I_ALT);
                    SET_FV(VdcAttenBrg1, AEBIH3_R_RATIO_VDC_ALT);
                    SET_FV(VphAttenBrg1, AEBIH3_R_RATIO_V_ALT);
                    SET_FV(ShuntTrpLvlBrg1, AEBIH3_TRIP_LEVEL_ALT);
                }
                else
                {
                    SET_FV(CurSensorGnBrg1, AEBIH3_AV_GAIN_I);
                    SET_FV(VdcAttenBrg1, AEBIH3_R_RATIO_VDC);
                    SET_FV(VphAttenBrg1, AEBIH3_R_RATIO_V);
                    SET_FV(ShuntTrpLvlBrg1, AEBIH3_TRIP_LEVEL);
                }
                break;
            case BRG_INFC_AEDBH1:
                // Bridge 1 AEDBH1
                Brg1InfcBoardAedb = true;
                if (INS(AltCfg))
                {
                    SET_FV(CurSensorGnBrg1, AEDBH1_AV_GAIN_I_ALT);
                    SET_FV(VdcAttenBrg1, AEDBH1_R_RATIO_VDC_ALT);
                    SET_FV(VphAttenBrg1, AEDBH1_R_RATIO_V_ALT);
                    SET_FV(ShuntTrpLvlBrg1, AEDBH1_TRIP_LEVEL_ALT);
                }
                else
                {
                    SET_FV(CurSensorGnBrg1, AEDBH1_AV_GAIN_I);
                    SET_FV(VdcAttenBrg1, AEDBH1_R_RATIO_VDC);
                    SET_FV(VphAttenBrg1, AEDBH1_R_RATIO_V);
                    SET_FV(ShuntTrpLvlBrg1, AEDBH1_TRIP_LEVEL);
                }
                break;
            case BRG_INFC_AEDBH2:
                // Bridge 1 AEDBH2
                Brg1InfcBoardAedb = true;
                if (INS(AltCfg))
                {
                    SET_FV(CurSensorGnBrg1, AEDBH2_AV_GAIN_I_ALT);
                    SET_FV(VdcAttenBrg1, AEDBH2_R_RATIO_VDC_ALT);
                    SET_FV(VphAttenBrg1, AEDBH2_R_RATIO_V_ALT);
                    SET_FV(ShuntTrpLvlBrg1, AEDBH2_TRIP_LEVEL_ALT);
                }
                else
                {
                    SET_FV(CurSensorGnBrg1, AEDBH2_AV_GAIN_I);
                    SET_FV(VdcAttenBrg1, AEDBH2_R_RATIO_VDC);
                    SET_FV(VphAttenBrg1, AEDBH2_R_RATIO_V);
                    SET_FV(ShuntTrpLvlBrg1, AEDBH2_TRIP_LEVEL);
                }
                break;
            case BRG_INFC_AEDBH3:
                // Bridge 1 AEDBH3
                Brg1InfcBoardAedb = true;
                if (INS(AltCfg))
                {
                    SET_FV(CurSensorGnBrg1, AEDBH3_AV_GAIN_I_ALT);
                    SET_FV(VdcAttenBrg1, AEDBH3_R_RATIO_VDC_ALT);
                    SET_FV(VphAttenBrg1, AEDBH3_R_RATIO_V_ALT);
                    SET_FV(ShuntTrpLvlBrg1, AEDBH3_TRIP_LEVEL_ALT);
                }
                else
                {
                    SET_FV(CurSensorGnBrg1, AEDBH3_AV_GAIN_I);
                    SET_FV(VdcAttenBrg1, AEDBH3_R_RATIO_VDC);
                    SET_FV(VphAttenBrg1, AEDBH3_R_RATIO_V);
                    SET_FV(ShuntTrpLvlBrg1, AEDBH3_TRIP_LEVEL);
                }
                break;
            case BRG_INFC_AEDBH4:
                // Bridge 1 AEDBH4
                Brg1InfcBoardAedb = true;
                if (INS(AltCfg))
                {
                    SET_FV(CurSensorGnBrg1, AEDBH4_AV_GAIN_I_ALT);
                    SET_FV(VdcAttenBrg1, AEDBH4_R_RATIO_VDC_ALT);
                    SET_FV(VphAttenBrg1, AEDBH4_R_RATIO_V_ALT);
                    SET_FV(ShuntTrpLvlBrg1, AEDBH4_TRIP_LEVEL_ALT);
                }
                else
                {
                    SET_FV(CurSensorGnBrg1, AEDBH4_AV_GAIN_I);
                    SET_FV(VdcAttenBrg1, AEDBH4_R_RATIO_VDC);
                    SET_FV(VphAttenBrg1, AEDBH4_R_RATIO_V);
                    SET_FV(ShuntTrpLvlBrg1, AEDBH4_TRIP_LEVEL);
                }
                break;
            case BRG_INFC_RGIBH1:
                // Bridge 1 RGIBH1

                if (INS(AltCfg))
                {
                    SET_FV(CurSensorGnBrg1, RGIBH1_AV_GAIN_I_ALT);
                    SET_FV(VdcAttenBrg1, RGIBH1_R_RATIO_VDC_ALT);
                    SET_FV(VphAttenBrg1, RGIBH1_R_RATIO_V_ALT);
                    SET_FV(ShuntTrpLvlBrg1, RGIBH1_TRIP_LEVEL_ALT);
                }
                else
                {
                    SET_FV(CurSensorGnBrg1, RGIBH1_AV_GAIN_I);
                    SET_FV(VdcAttenBrg1, RGIBH1_R_RATIO_VDC);
                    SET_FV(VphAttenBrg1, RGIBH1_R_RATIO_V);
                    SET_FV(ShuntTrpLvlBrg1, RGIBH1_TRIP_LEVEL);
                }
                break;


            default:
                SET_FV(CurSensorGnBrg1, AEDBH3_AV_GAIN_I_ALT);
                SET_FV(VdcAttenBrg1, AEDBH3_R_RATIO_VDC_ALT);
                SET_FV(VphAttenBrg1, AEDBH3_R_RATIO_V_ALT);
                SET_FV(ShuntTrpLvlBrg1, AEDBH3_TRIP_LEVEL_ALT);
                break;
        }
        VcoBiasFreqBrg1 = BRIDGE_V_FBIAS;
    }
    else
    {   // bridge 1 disabled - default to AEDBH3
        SET_FV(CurSensorGnBrg1, AEDBH3_AV_GAIN_I_ALT);
        SET_FV(VdcAttenBrg1, AEDBH3_R_RATIO_VDC_ALT);
        SET_FV(VphAttenBrg1, AEDBH3_R_RATIO_V_ALT);
        SET_FV(ShuntTrpLvlBrg1, AEDBH3_TRIP_LEVEL_ALT);
        VcoBiasFreqBrg1 = BRIDGE_V_FBIAS;
    }

    if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
    {
        SclLinkVoltsBrg1    = (float)(BRG_VCO_VREF/BRG_VCO_FZRO) * PARM(VdcAttenBrg1);
        SclVoltBrgOffset    = 0.0F;
    }
    else
    {
        if (PARM(RgabPresent))
        {
          SclLinkVoltsBrg1   = 2.0F * 1875.19F/8.333e6;
          SclVoltBrgOffset   = BRG_V_CNT_OFFSET1;
        }
        else
        {
          SclLinkVoltsBrg1   = 2.0F * 5.0F/8.333e6;
          SclVoltBrgOffset   = BRG_V_CNT_OFFSET2;
        }
    }

    SclLinkVoltsBrg1Rcp = 1.0F / SclLinkVoltsBrg1;

    if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
    {
        SclPhsVoltsBrg1   = (float)(BRG_VCO_VREF/BRG_VCO_FZRO) * PARM(VphAttenBrg1);
        SclVoltBrgOffset  = 0.0F;
    }
    else
    {
        if (PARM(RgabPresent))
        {
          SclPhsVoltsBrg1   = 2.0F * 1875.19F/8.333e6;
          SclVoltBrgOffset  = BRG_V_CNT_OFFSET1;
        }
        else
        {
          SclPhsVoltsBrg1   = 2.0F * 5.0F/8.333e6;
          SclVoltBrgOffset  = BRG_V_CNT_OFFSET2;
        }
    }

    SclPhsVoltsBrg1Rcp = 1.0F / SclPhsVoltsBrg1;


    if ( PARM(SoftwareIPN) <= DFIG02_2400_5060_A)
    {
        SclPhsCurBrg1 = (float)(BRG_VCO_VREF/BRG_VCO_FZRO) * PARM(CurSensorGnBrg1) * CurSensorSclBrg1;
    }
    else
    {
        SclPhsCurBrg1 = 2 * 5000.0F/8.333e6;
    }
    SclPhsCurBrg1Rcp = 1.0F / SclPhsCurBrg1;

    SclPhsCurPhD = (float)(BRG_VCO_VREF/BRG_VCO_FZRO) * PARM(CurSensorGnBrg1) * CurSensorSclPhD;

    SET_FV(VdcFs,   BRG_VCO_FZRO * SclLinkVoltsBrg1);
    SET_FV(VacFs,   BRG_VCO_FZRO * SclPhsVoltsBrg1);
    SET_FV(IfbkFs,  BRG_VCO_FZRO * SclPhsCurBrg1);


    //-------------------------------------------------------------------------
    // configure the Io interface Current/Voltage Scaling
    //-------------------------------------------------------------------------

    // IO  voltage attenuation
    #define AEAAH1_VOLTS_MAX                 (1417.99434)    // AEAAH1 volts for 2.0 mHz VCO output
    #define AEAAH1_VOLTS_MAX_ALT             (1417.99434)    // AEAAH1 volts for 2.0 mHz VCO output for alternate platform
    #define AEAAH2_VOLTS_MAX                 (1417.99434)    // AEAAH2 volts for 2.0 mHz VCO output
    #define AEAAH2_VOLTS_MAX_ALT             (1417.99434)    // AEAAH2 volts for 2.0 mHz VCO output for alternate platform
    #define AEAAH3_VOLTS_MAX                 (1788.6864)     // AEAAH3 volts for 2.0 mHz VCO output
    #define AEAAH3_VOLTS_MAX_ALT             (1788.6864)     // AEAAH3 volts for 2.0 mHz VCO output for alternate platform
    #define AEAAH4_VOLTS_MAX                 (1788.6864)     // AEAAH4 volts for 2.0 mHz VCO output
    #define AEAAH4_VOLTS_MAX_ALT             (1788.6864)     // AEAAH4 volts for 2.0 mHz VCO output for alternate platform

    #define AEADH1_VOLTS_MAX                 (856.44)        // AEADH1 volts for 2.0 mHz VCO output
    #define AEADH1_VOLTS_MAX_ALT             (856.44)        // AEADH1 volts for 2.0 mHz VCO output for alternate platform
    #define AEADH2_VOLTS_MAX                 (1018.26)       // AEADH2 volts for 2.0 mHz VCO output
    #define AEADH2_VOLTS_MAX_ALT             (300.333)       // AEADH2 volts for 2.0 mHz VCO output for alternate platform
    #define AEADH3_VOLTS_MAX                 (1018.26)       // AEADH3 volts for 2.0 mHz VCO output
    #define AEADH3_VOLTS_MAX_ALT             (300.333)       // AEADH3 volts for 2.0 mHz VCO output for alternate platform
    #define AEADH4_VOLTS_MAX                 (1018.26)       // AEADH4 volts for 2.0 mHz VCO output
    #define AEADH4_VOLTS_MAX_ALT             (300.333)       // AEADH4 volts for 2.0 mHz VCO output for alternate platform

    #define NATPG1_LOWIN_TO_6K_VOLTS_MAX     (8930.027)      // AEADH2(3,4) with NATPG1 on low-voltage input and 6KV rated grid
    #define NATPG1_LOWIN_TO_690_VOLTS_MAX    (1074.243)      // AEADH2(3,4) with NATPG1 on low-voltage input and 690V rated grid
    #define NATPG1_LOWIN_TO_6K_VOLTS_MAX_ALT (8930.027)      // AEADH2(3,4) with NATPG1 on low-voltage input and 6KV rated grid, for alternate platform


     // IO  current gain

    #define AEAAH1_AMPS_MAX                  (8.43403)       // AEAAH1 current amps in burden resistor for 2.0 mHz VCO output
    #define AEAAH1_AMPS_MAX_ALT              (8.43403)       // AEAAH1 current amps in burden resistor for 2.0 mHz VCO output for alternate platform
    #define AEAAH2_AMPS_MAX                  (8.38432)       // AEAAH2 current amps in burden resistor for 2.0 mHz VCO output
    #define AEAAH2_AMPS_MAX_ALT              (8.38432)       // AEAAH2 current amps in burden resistor for 2.0 mHz VCO output for alternate platform
    #define AEAAH3_AMPS_MAX                  (8.38432)       // AEAAH3 current amps in burden resistor for 2.0 mHz VCO output
    #define AEAAH3_AMPS_MAX_ALT              (8.38432)       // AEAAH3 current amps in burden resistor for 2.0 mHz VCO output for alternate platform
    #define AEAAH4_AMPS_MAX                  (8.38432)       // AEAAH4 current amps in burden resistor for 2.0 mHz VCO output
    #define AEAAH4_AMPS_MAX_ALT              (8.38432)       // AEAAH4 current amps in burden resistor for 2.0 mHz VCO output for alternate platform



// not used, reserve for future use
#if 0
    #define AEAAH1_BURDEN_R                  (1.5)           // AEAAH1 burden resistor(ohms)
    #define AEAAH1_BURDEN_R_ALT              (1.5)           // AEAAH1 burden resistor(ohms) for alternate platform
    #define AEAAH2_BURDEN_R                  (0.5)           // AEAAH2 burden resistor(ohms)
    #define AEAAH2_BURDEN_R_ALT              (0.5)           // AEAAH2 burden resistor(ohms) for alternate platform
    #define AEAAH3_BURDEN_R                  (0.5)           // AEAAH3 burden resistor(ohms)
    #define AEAAH3_BURDEN_R_ALT              (0.5)           // AEAAH3 burden resistor(ohms) for alternate platform
    #define AEAAH4_BURDEN_R                  (0.5)           // AEAAH4 burden resistor(ohms)
    #define AEAAH4_BURDEN_R_ALT              (0.5)           // AEAAH4 burden resistor(ohms) for alternate platform
#endif
    #define AEADH1_AMPS_MAX                  (14.6)          // AEADH1 current amps in burden resistor for 2.0 mHz VCO output
    #define AEADH1_AMPS_MAX_ALT              (14.6)          // AEADH1 current amps in burden resistor for 2.0 mHz VCO output for alternate platform
    #define AEADH2_AMPS_MAX                  (14.6)          // AEADH2 current amps in burden resistor for 2.0 mHz VCO output
    #define AEADH2_AMPS_MAX_ALT              (14.6)          // AEADH2 current amps in burden resistor for 2.0 mHz VCO output for alternate platform
    #define AEADH3_AMPS_MAX                  (28.583)        // AEADH3 current amps in burden resistor for 2.0 mHz VCO output
    #define AEADH3_AMPS_MAX_ALT              (28.583)        // AEADH3 current amps in burden resistor for 2.0 mHz VCO output for alternate platform
    #define AEADH4_AMPS_MAX                  (5.7166)        // AEADH4 current amps in burden resistor for 2.0 mHz VCO output
    #define AEADH4_AMPS_MAX_ALT              (5.7166)        // AEADH4 current amps in burden resistor for 2.0 mHz VCO output for alternate platform

// not used, reserve for future use
#if 0
    #define AEADH1_BURDEN_R                  (0.1)           // AEADH1 burden resistor(ohms)
    #define AEADH1_BURDEN_R_ALT              (0.1)           // AEADH1 burden resistor(ohms) for alternate platform
    #define AEADH2_BURDEN_R                  (0.1)           // AEADH2 burden resistor(ohms)
    #define AEADH2_BURDEN_R_ALT              (0.1)           // AEADH2 burden resistor(ohms) for alternate platform
    #define AEADH3_BURDEN_R                  (0.1)           // AEADH3 burden resistor(ohms)
    #define AEADH3_BURDEN_R_ALT              (0.1)           // AEADH3 burden resistor(ohms) for alternate platform
    #define AEADH4_BURDEN_R                  (0.1)           // AEADH4 burden resistor(ohms)
    #define AEADH4_BURDEN_R_ALT              (0.1)           // AEADH4 burden resistor(ohms) for alternate platform
#endif

    #define V_lN_LOW_2MHZ                    (12.6511)       // AEAD Scaling definitions for PWA ID voltage scalings
    #define lN_V_TEST                        (10.0)          // AEAD Scaling AMPLIFIER OUTPUT
    #define IN_I_TEST_AEADH1                 (5.0)
    #define IN_I_TEST_AEADH2                 (5.0)
    #define IN_I_TEST_AEADH3                 (5.0)
    #define IN_I_TEST_AEADH4                 (2.0)

    // IO 1 scaling
    //-----------------------------------
    switch ((IoInfcHdwTyp)(INS(Io1InfcHdw)))
    {
        case IO_INFC_AEAAH1:
            // IO/1 AEAAH1
            if (INS(AltCfg))
            {
                SET_FV(CurSensorGnIo1, AEAAH1_AMPS_MAX_ALT);
                SET_FV(VphMaxIo1, AEAAH1_VOLTS_MAX_ALT);
            }
            else
            {
                SET_FV(CurSensorGnIo1, AEAAH1_AMPS_MAX);
                SET_FV(VphMaxIo1, AEAAH1_VOLTS_MAX);
            }
            break;
        case IO_INFC_AEAAH2:
            // IO/1 AEAAH2
            if (INS(AltCfg))
            {
                SET_FV(CurSensorGnIo1, AEAAH2_AMPS_MAX_ALT);
                SET_FV(VphMaxIo1, AEAAH2_VOLTS_MAX_ALT);
            }
            else
            {
                SET_FV(CurSensorGnIo1, AEAAH2_AMPS_MAX);
                SET_FV(VphMaxIo1, AEAAH2_VOLTS_MAX);
            }
            break;
        case IO_INFC_AEAAH3:
            // IO/1 AEAAH3
            if (INS(AltCfg))
            {
                SET_FV(CurSensorGnIo1, AEAAH3_AMPS_MAX_ALT);
                SET_FV(VphMaxIo1, AEAAH3_VOLTS_MAX_ALT);
            }
            else
            {
                SET_FV(CurSensorGnIo1, AEAAH3_AMPS_MAX);
                SET_FV(VphMaxIo1, AEAAH3_VOLTS_MAX);
            }
            break;
        case IO_INFC_AEAAH4:
            // IO/1 AEAAH4
            if (INS(AltCfg))
            {
                SET_FV(CurSensorGnIo1, AEAAH4_AMPS_MAX_ALT);
                SET_FV(VphMaxIo1, AEAAH4_VOLTS_MAX_ALT);
            }
            else
            {
                SET_FV(CurSensorGnIo1, AEAAH4_AMPS_MAX);
                SET_FV(VphMaxIo1, AEAAH4_VOLTS_MAX);
            }
            break;
        case IO_INFC_AEADH1:
            // I/O 1 AEADH1
            if (INS(AltCfg))
            {
                SET_FV(CurSensorGnIo1, AEADH1_AMPS_MAX_ALT);
                SET_FV(VphMaxIo1, AEADH1_VOLTS_MAX_ALT);
            }
            else
            {
                SET_FV(CurSensorGnIo1, AEADH1_AMPS_MAX);
                SET_FV(VphMaxIo1, AEADH1_VOLTS_MAX);
            }
            SET_FV(IinTestIo1, IN_I_TEST_AEADH1);
            break;
        case IO_INFC_AEADH2:
            // I/O 1 AEADH2
            if (INS(AltCfg))
            {
                SET_FV(CurSensorGnIo1, AEADH2_AMPS_MAX_ALT);
                SET_FV(VphMaxIo1, AEADH2_VOLTS_MAX_ALT);
            }
            else
            {
                SET_FV(CurSensorGnIo1, AEADH2_AMPS_MAX);
                SET_FV(VphMaxIo1, AEADH2_VOLTS_MAX);
            }
            SET_FV(IinTestIo1, IN_I_TEST_AEADH2);
            break;
        case IO_INFC_AEADH3:
            // I/O 1 AEADH3
            if (INS(AltCfg))
            {
                SET_FV(CurSensorGnIo1, AEADH3_AMPS_MAX_ALT);
                SET_FV(VphMaxIo1, AEADH3_VOLTS_MAX_ALT);
            }
            else
            {
                SET_FV(CurSensorGnIo1, AEADH3_AMPS_MAX);
                SET_FV(VphMaxIo1, AEADH3_VOLTS_MAX);
            }
            SET_FV(IinTestIo1, IN_I_TEST_AEADH3);
            break;
        case IO_INFC_AEADH4:
            // I/O 1 AEADH4
            if (INS(AltCfg))
            {
                SET_FV(CurSensorGnIo1, AEADH4_AMPS_MAX_ALT);
                SET_FV(VphMaxIo1, AEADH4_VOLTS_MAX_ALT);
            }
            else
            {
                SET_FV(CurSensorGnIo1, AEADH4_AMPS_MAX);
                SET_FV(VphMaxIo1, AEADH4_VOLTS_MAX);
            }
            SET_FV(IinTestIo1, IN_I_TEST_AEADH4);
            break;
        case IO_INFC_NONE:
            // I/O 1 no connection -default to AEAAH1
            SET_FV(CurSensorGnIo1, AEAAH1_AMPS_MAX);
            SET_FV(VphMaxIo1, AEAAH1_VOLTS_MAX);
            SET_FV(IinTestIo1, IN_I_TEST_AEADH1);
            break;

        default:
            SET_FV(CurSensorGnIo1, AEAAH1_AMPS_MAX);
            SET_FV(VphMaxIo1, AEAAH1_VOLTS_MAX);
            SET_FV(IinTestIo1, IN_I_TEST_AEADH1);
            break;
    }

    // Override voltage scaling if low voltage input used with attenuation board.
    // Groups 2, 3, and 4 of AEAD have same gains when making connections to
    // low-voltage inputs, and grounding the high-voltage inputs.
    // Gains seen by attenuation boards connected to AEAA need to be investigated.
    if (((IoAttenHdwTyp)(INS(Io1AttenHdw))) == NATPG1_LOW_VOLT_INPUT)
    {
        if (INS(AltCfg))
        {
            SET_FV(VphMaxIo1, NATPG1_LOWIN_TO_6K_VOLTS_MAX_ALT);
        }
        else
        {
            SET_FV(VphMaxIo1, NATPG1_LOWIN_TO_6K_VOLTS_MAX);
        }
    }
    else if (((IoAttenHdwTyp)(INS(Io1AttenHdw))) == NATPG1_LOW_VOLT_INPUT_690)
    {
        // scaling for NATP modified for factory pumpback:
        SET_FV(VphMaxIo1, NATPG1_LOWIN_TO_690_VOLTS_MAX);
    }

    SclPhsVoltsIo1   = (float)(PARM(VphMaxIo1) / IO_VCO_FZRO);
    SclPhsVoltsIo1Rcp = 1.0F / SclPhsVoltsIo1;

    SclPhsCurIo1 = (float)(PARM(CurSensorGnIo1) / IO_VCO_FZRO) * INS(CtRatioIo1);
    SclPhsCurIo1Rcp = 1.0F / SclPhsCurIo1;

	// Speed Encoder Simulation
    SimSclSpdEnc    = (float)( (1.0F/PI2)*(4.0F*INS(TachPPR)) );
    SimSclSpdEncRcp = NoNANLim( 1.0F / SimSclSpdEnc, 1.0e6, -1.0e6);

    if (INS(SimMode))
    {
        SET_FV(HwGdStatMask, (SimModeBrgEnabled ? 0x00003f3f : 0x0000));    // probably dont hw have in sim mode
    }
    else
    {
        SET_FV(HwGdStatMask, 0x00003f3f);
    }

    SET_FV(HwIocCntLmt, 0);

    SET_FV(VGnErMax, 0.015F);           // allow 1.5% error
    SET_FV(IGnErMax, 0.020F);           // allow 2.0% error
    SET_FV(OffErMax, 0.015F);           // allow 1.5% error, or 15,000hz
    SET_FV(AeadIDAdjInh, false);        // default to not inhibit the calibration
    SET_FV(Io1CTITestScl,   2.0 * PARM(IinTestIo1) * INS(CtRatioIo1));
    SET_FV(Io1VlnHvTestScl, 2.0 * lN_V_TEST        * (PARM(VphMaxIo1) / V_lN_LOW_2MHZ));

    SET_FV(Io1RlyPwrChkInh, INS(Io1RlyPwrChkInhReq));


#if 0   // test code for simulating faults
    // fault test parameters
    SET_FV(BrgFltTst,             0 );
    SET_FV(BrStatFltTst,          0x0 );
    SET_FV(StatComTst,            0x008f5060 );
    SET_FV(StatReg21Tst,          0x0c06e1fe );
    SET_FV(MiscStatTst,           0x10000000 );
#endif

}

#endif  // B0_BRIDGE_REBF_RULE_INC
