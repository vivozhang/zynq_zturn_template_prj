///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Line Side Product Level Rules
//
// DESCRIPTION:
//      This file contains the rules for the line side control.  This file
//      is defined uniquely per product and is included by p2Product.cpp.
//      The final value parameters and variables calculated here are defined
//      in p2Product.cpp
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

{

    // ------------------------------------------------------------------------
    // hardware boards for IO and bridge interface
    SET_FV(L_HslaRebfSite, (unsigned)cBridgeRebf::HSLA_SITE_JA1);

    if ( PARM(SoftwareIPN) == DFIG02_1500_5060_A )
    {
        SET_FV(L_BrInfcHdw, (unsigned)cBridgeRebf::BRG_INFC_AEBIH3);
        SET_FV(L_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEADH1);
        SET_FV(L_ParallelIGBT, false);
        SET_FV(GenAeadPresent, false);
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1500_5060_B )
    {
        SET_FV(L_BrInfcHdw, (unsigned)cBridgeRebf::BRG_INFC_AEDBH4);
        SET_FV(L_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEADH1);
        SET_FV(L_ParallelIGBT, false);
        SET_FV(GenAeadPresent, false);
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1850_5060_C )
    {
        SET_FV(L_BrInfcHdw, (unsigned)cBridgeRebf::BRG_INFC_AEDBH4);
        SET_FV(L_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEADH1);
        SET_FV(L_ParallelIGBT, false);
        SET_FV(GenAeadPresent, false);
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1850_5060_D )
    {
        SET_FV(L_BrInfcHdw, (unsigned)cBridgeRebf::BRG_INFC_AEDBH4);
        SET_FV(L_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEADH3);
        SET_FV(L_ParallelIGBT, false);
        SET_FV(GenAeadPresent, false);
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1850_5060_E )
    {
        SET_FV(L_BrInfcHdw, (unsigned)cBridgeRebf::BRG_INFC_AEDBH4);
        SET_FV(L_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEADH3); // Needs to change to accept H4 as well
        SET_FV(L_ParallelIGBT, false);
        SET_FV(GenAeadPresent, false);
    }
    else if ( PARM(SoftwareIPN) == DFIG02_2400_5060_A )
    {
        SET_FV(L_BrInfcHdw, (unsigned)cBridgeRebf::BRG_INFC_AEDBH4);
        SET_FV(L_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEADH4);
        SET_FV(L_ParallelIGBT, true);
        SET_FV(GenAeadPresent, true);
    }
    else // SoftwareIPN ==  DFIG02_2400_5060_B:
    {
        SET_FV(L_BrInfcHdw, (unsigned)cBridgeRebf::BRG_INFC_RGIBH1);
        SET_FV(L_Io1InfcHdw,(unsigned)cBridgeRebf::IO_INFC_AEADH4);
        SET_FV(L_ParallelIGBT, true);
        SET_FV(GenAeadPresent, true);
    }



    SET_FV(L_Io1AttenHdw,(unsigned)cBridgeRebf::NO_ATTENUATOR_BOARD);

    // ------------------------------------------------------------------------
    // parameter for pulse and cell tests
    L_BrgTestL = 2.0 * PARM(CnvLineL);    // two phases in series
    L_BrgTestC = 0.5 * PARM(CnvLineDfC);  // two capacitors in series between two phases of the Y-connection

    if (SimMode&&!XSimMode)
    {
        SET_FV(L_SkipFireInh, true);
		SET_FV(L_LkoutInh, true);
    }
    else
    {
        SET_FV(L_SkipFireInh, false);
		SET_FV(L_LkoutInh, false);
    }

    SET_FV(GmVcomOkThrsh, 0.05);
    SET_FV(GmVcomDlyTm,   0.5);

    SET_FV(L_BpTempNom1, 72.42);
    SET_FV(L_BpTempNom2, 72.42);
    SET_FV(L_BpTempNom3, 72.42);
    SET_FV(L_BpTempNom4, 72.42);
    SET_FV(L_BpTempNom5, 72.42);
    SET_FV(L_BpTempNom6, 72.42);

    // gain adjustment for lockout comp - values derived empirically
    SET_FV(L_LkoutGnSclMax, 2.5);
    SET_FV(L_LkoutGnSclMin, 0.5);
    SET_FV(L_LkoutGnSclK, PARM(L_IRat) * SQRT2*0.77);


    // Phase shift for 690v ct current feedbacks.  Lab testing with Yokogawa indicates that
    // the converter's current measurement is slightly out of phase with voltage measurement.
    // This parameter is set on the basis of this testing.  Origin of the phenomenum is not understood
    // at this time.
    if (!SimMode)
    {
        SET_FV(GmIfbkShftTm, -24.0e-6F);      // seconds of shift to apply to cfb  ( > 0 = lag, < 0 = lead)
    }
    else
    {
        SET_FV(GmIfbkShftTm, 0.0F);
    }

    GmIxyShftD = cosf(PARM(GmIfbkShftTm) * PI2 * PARM(SysFreqNom));
    GmIxyShftQ = sinf(PARM(GmIfbkShftTm) * PI2 * PARM(SysFreqNom));

    SET_FV(GridVoltOkPuLvlLo,  0.8F);
    SET_FV(GridVoltOkPuLvlHi,  1.12F);
    SET_FV(XfmrSatDampTm, 4.0F);      // saber showsn a 3-second time delay to settle down the xfmr saturation after ZVRT
    float temp;
    temp = 1.0F/(PARM(L_FrqInput)*PI);
    SET_FV(GridVoltLowTm, 1.1F * 2.0F * asinf(PARM(GridVoltOkPuLvlLo)) * temp );  // twice of the time when the ABS(Gm V L-L) is below the GridVoltLowLvl in normal condition.
                                                                                // 1.1 is the margin
    // To avoid the mis-dsiable at zero crossing when fault free
    SET_FV(GridVoltOkTm, PARM(GridVoltLowTm) * 0.5F * ( 1.0F - PARM(GridVoltOkPuLvlLo) ) );
    SET_FV(GridVoltHiTm, DelTm2 *2.0F);

    SET_FV(L_DbFanOnThr, 0.04F*PARM(DbOtThrsh));
    SET_FV(L_DbFanOffThr,0.02F*PARM(DbOtThrsh));

    SET_FV(L_DbPtSel,   cDbPlsTst::PULSE_DB1);      // always set DB bridge selector to bridge 1
    SET_FV(L_DbCtSel,   cDbCelTst::CELL_TST_DB1);   // always set DB bridge selector to bridge 1

    if(!SimAcclAct && PARM(L_BrgDbEnb))
    {
        SET_FV(L_StartupDbMsEnb,    true);
    }
    else
    {
        SET_FV(L_StartupDbMsEnb,    false);
    }

}
