///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Rotor Side Product Level Rules
//
// DESCRIPTION:
//      This file contains the rules for the rotor side control.  This file
//      is defined uniquely per product and is included by p1Product.cpp.
//      The final value parameters and variables calculated here are defined
//      in p1Product.cpp
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
    // parameter for pulse and cell tests
    R_BrgTestL  =  2.0 * PARM(CnvRotorL);         // two reactors in series ( 2 phases )


    R_BrgTestAltL = R_BrgTestL * PARM(CnvRotorAltLpu);  // L value for ROV checking


//-----------------------------------------------------------------------------


    // ------------------------------------------------------------------------
    // hardware boards for IO and bridge interface
    SET_FV(R_HslaRebfSite, (unsigned)cBridgeRebf::HSLA_SITE_JA2);

    SET_FV(R_BrInfcHdw, (unsigned)cBridgeRebf::BRG_INFC_AEBIH3); //always use AEBIH3

    if ( PARM(SoftwareIPN) == DFIG02_1500_5060_A )
    {
        SET_FV(R_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEAAH2);
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1500_5060_B )
    {
        SET_FV(R_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEAAH2);
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1850_5060_C )
    {
        SET_FV(R_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEAAH2);
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1850_5060_D )
    {
        SET_FV(R_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEAAH3);
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1850_5060_E )
    {
        SET_FV(R_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEADH4);
    }
    else if ( PARM(SoftwareIPN) == DFIG02_2400_5060_A )
    {
        SET_FV(R_Io1InfcHdw,  (unsigned)cBridgeRebf::IO_INFC_AEADH4);
    }
    else // SoftwareIPN ==  DFIG02_2400_5060_B:
    {
        SET_FV(R_BrInfcHdw, (unsigned)cBridgeRebf::BRG_INFC_RGIBH1);
        SET_FV(R_Io1InfcHdw, (unsigned)cBridgeRebf::IO_INFC_AEADH4);
    }

    //SET_FV(R_BrInfcHdw, (unsigned)cBridgeRebf::BRG_INFC_RGIBH1); //keep it outside of if statements for now...remove later


    // -------------------------------------------------------------------------
    // R_Io1InfcHdw Configuration

    if ( PARM(R_Io1InfcHdw) == cBridgeRebf::IO_INFC_AEAAH3)//Need to fix this to use class scalings
    {
       SET_FV(S_VabVcoScl,1788.6864F/1.0e6F); //1788.6864/1,000,000hz...AEAA#BF elementary, sheet 12
       SET_FV(S_VbcVcoScl,1788.6864F/1.0e6F); //1788.6864/1,000,000hz...AEAA#BF elementary, sheet 12
    }
    else //all other groups
    {
       SET_FV(S_VabVcoScl,1417.99434F/1.0e6F); //1417.99434/1,000,000hz...AEAA#BF elementary, sheet 12
       SET_FV(S_VbcVcoScl,1417.99434F/1.0e6F); //1417.99434/1,000,000hz...AEAA#BF elementary, sheet 12
    }


    SET_FV(S_VllOffsLim,0.0075F * (2.0F*PARM(S_VabVcoScl)*PARM(AeaaAinZero)) );  // Offset limit = .75% full scale voltage

    SET_FV(S_VllFbkLag, -0.03F);  // ESS: Found experimentally in lab --mostly due to T1-->T2 data passing....was -0.015 with old sw
    //SET_FV(S_VllFbkLag, 0.0F);    // 3.0 dfig: using same (AEAD) card for both 6kv and stator feedback

    S_VoltsPrAmp = NoNANLim( PARM(S_CTburdenR)/PARM(S_CTratio), 1.0e6F, -1.0e6F );
    SET_FV(S_IOffsThr, 0.01F * PARM(S_IOffsPct) * 1.0e6F * PARM(S_IaVcoScl) / S_VoltsPrAmp);
    SET_FV(S_WVOffsFil, 5.0F);
    SET_FV(S_VOffsRnTm, 5.0F);
    SET_FV(S_WVOffsFst, 250.0F);
    SET_FV(S_VOffsFst, 0.34F);
    SET_FV(S_VOffsThr,  PARM(S_VllOffsLim));

    // End of R_Io1InfcHdw Configuration
    // -------------------------------------------------------------------------


    // ------------------------------------------------------------------------
    // Dual rotor bridge

    SET_FV(R_BrgPwrPrm, (unsigned)BOTH_BRIDGES);

    R_DualBrgEnb = (   (PARM(R_BrInfcHdw) != (unsigned)cBridgeRebf::BRG_INFC_NONE)
                    && (PARM(R_BrgPwrPrm)  == BOTH_BRIDGES)
                   ) ? true : false;

    R_BrPwrPrm = ( (PARM(R_BrgPwrPrm) == BOTH_BRIDGES) || (PARM(R_BrgPwrPrm) == BRIDGE_1_ONLY) ) ? true : false;


    SET_FV(R_CmnRbrg, 0.001);       // L/R = 0.005
    SET_FV(R_CrsRbrg, 0.001);       // L/R = 0.02
    SET_FV(R_CmnLbrg, 0.000005);    // 1/4 of the R_CrsLbrg (?)
    SET_FV(R_CrsLbrg, 0.00002);     // 3.0 elemen ver 0.37

    SET_FV(R_CshRegEnbInh, false);


    if (SimMode&&!XSimMode)
    {
        SET_FV(R_SkipFireInh, true);
        SET_FV(R_LkoutInh, true);
    }
    else
    {
        SET_FV(R_SkipFireInh, false);
        SET_FV(R_LkoutInh, false);
    }

    SET_FV(S_VcomOkThrsh, 0.1F);
    SET_FV(VcomFilW,     2.0F * PI * 60.0F);
    SET_FV(VcomTm,       5.0F);
    VcomFilGn = PARM(VcomFilW) * DelTm2;

    SET_FV( S_PllErNoise, -PARM(S_PllIniThrs) +
    sqrtf(PARM(S_PllIniThrs)*PARM(S_PllIniThrs) + L_VnoiseLvl*L_VnoiseLvl) );

    // gain adjustment for lockout comp - values derived empirically
    SET_FV(R_LkoutGnSclMax, 2.5);
    SET_FV(R_LkoutGnSclMin, 0.5);
    SET_FV(R_LkoutGnSclK,   (20.0 * PI2) * PARM(R_IRat)*SQRT2 * 0.57);   // note: rated rotor freq ~ 20 Hz
                                                                         // using constant so value doesn't change with machine
}
