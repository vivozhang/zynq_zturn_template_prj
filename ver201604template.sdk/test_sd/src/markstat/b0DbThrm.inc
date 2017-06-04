///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Dynamic brake thermal model (Dtm) class
//
// DESCRIPTION:
//      This file contains the rules for the DB thermal model class.
//      This file is defined uniquely per product and is included by b0DbThrm.cpp.
//      The final value parameters and variables calculated here are defined
//      by b0DbThrm.h.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0DbThrm.h
//
///////////////////////////////////////////////////////////////////////////////
    //------------------------------------------------------------------------------------------------------
    // What is the amount of power dissipated by DB in the time it takes to open the contactor?
    // This is the additional amount to add to the trip threshold for the DB disable threshold.
    //
    // Find time to cause DBOT trip at nominal DbR value and DC link at OV threshold,
    //   then add --> INS(PwrDisconTm) (usually the time for line contactor to open + margin)
    //   then find the output of the thermal model at that total time...this is the db shutdown threshold
    //
    //------------------------------------------------------------------------------------------------------


    // Diagnostic thresholds
    //----------------------

    // This Vdc overvoltage factor is taken from previous DFIG, it sets
    // overvoltage level at 1350 Vdc for 690 Vrms line voltage
    // -------------------------------------------------------------------------
    SET_FV(DtmVdcOvLvl,      INS(VllIn) * (1350.0F/690.0F) );

    SET_FV(DtmOvldTrpThrs,   INS(DbOvldTrpThrs));  // this results on 13.3% max overload per previous DFIG

    float tempNumb =         NoNANLim( -logf(1.0F - INS(DbContPwr) / INS(DbImpPwr)) /
                                       INS(DbImpTm), 1.0e6F, -1.0e6F );
    SET_FV(DtmW,             tempNumb);

    DtmTrpTau =              NoNANLim( 1.0F / PARM(DtmW), 1.0e6F, -1.0e6F );
    DtmOvldTrpTm =           NoNANLim( -(logf(1.0F - (PARM(DtmOvldTrpThrs) * INS(DbR) / (PARM(DtmVdcOvLvl) *
                                       PARM(DtmVdcOvLvl)) ) ) ) * DtmTrpTau, 1.0e6F, -1.0e6F );

    DtmDsbTm =               DtmOvldTrpTm + INS(PwrDisconTm);   // contactor disconnection time is expected to have some margin

    tempNumb =               NoNANLim( (PARM(DtmVdcOvLvl) *  PARM(DtmVdcOvLvl)) / INS(DbR) *
                                      (1.0F - expf(-DtmDsbTm * PARM(DtmW))), 1.0e6F, -1.0e6F );
    SET_FV(DtmOvldDsbThrs,   tempNumb);

    SET_FV(DtmOvldOnThrs,    (INS(DbContPwr) * (1.15F * 0.985F + 1.0F) / 2.0F) );
    SET_FV(DtmOvldOffThrs,   INS(DbContPwr) );

    // Model constants
    SET_FV( DtmEnb, INS(DbEnb) );
    DtmRrcp =                NoNANLim( 1.0F / INS(DbR), 1.0e6F, -1.0e6F );
    if ( INS(DbTauElec) >= 0.0F )
    {
       DtmEdtTau  =          1.0F - expf(-INS(DelTm1) / INS(DbTauElec));
       DtmE2DtTau  =         1.0F - expf(-2.0F * INS(DelTm1) / INS(DbTauElec));
    }
    else
    {
       DtmEdtTau  =          0.0F;
       DtmE2DtTau  =         0.0F;
    }

    //-----------------------------------------------------------------------------------------
    // Calculate the threshold voltage level to indicate DB switch on
    //
    // close approximation to avg value of (1-e-(t/tau)) for one dt is tau/dt, if dt >> tau
    // Don't understand need for 1.40 factor, should have been ok without this.
    // DbTauFbkCkt = 34.98e-6 is the tau of the Vdb feedback circuit based on R=10k+5.9k, C=.0022 uf
    //-----------------------------------------------------------------------------------------
    tempNumb = NoNANLim( 1.40F * INS(DbTauFbkCkt) / INS(DelTm1), 1.0e6F, -1.0e6F );
    SET_FV(DtmVdbAdj, tempNumb * INS(VdbAdjFact));

    //1.1529 is fudge factor to set threshold at 1125v for 690v rms line voltage
    //0.9347=(1.3086/1.40) is a fudge factor to differenciate MarkStat behavior from MACC based
    // ----------------------------------------------------------------------------------------
    SET_FV(DtmVdcThrs, INS(VllIn) * SQRT2 * 1.1529F);

    //DB voltage threshold for On-state as a function of continous power and resistance
    // --------------------------------------------------------------------------------
    if ( PARM(TestMode) == PUMP_BACK )
    {
       SET_FV(DtmOnThrs, 100); //A function of continous power and DB resistance
    }
    else
    {
       SET_FV(DtmOnThrs, sqrt(1.0F * INS(DbContPwr) * INS(DbR))); //A function of continous power and DB resistance
    }

    // Thermal characteristics of DB semiconductor device
    //--------------------------------------------------------------------------
    if ( INS(ProductType) >= DFIG02_2400_5060_A)//Gen 4 IGBTs,2.3 cooling system--GEN4
    {                                                        //MACC Name
        SET_FV( DtmTrR,           5.20e-04F ); // R        - L_BtmIgbtR
        SET_FV( DtmTrVo,               1.0F ); // Vo       - L_BtmIgbtVo
        SET_FV( DtmTrSw0,        4.212e-05F ); // S0       - L_BtmIgbtS0
        SET_FV( DtmTrSw1,        4.990e-07F ); // Sw1      - L_BtmIgbtSw1
        SET_FV( DtmTrSw2,        2.770e-11F ); // Sw2      - L_BtmIgbtSw2
        SET_FV( DtmTrCJnt,        0.954F    ); // Cj       - L_BtmTrJc
        SET_FV( DtmTrRthJntNd3,   0.0021F   ); // Rj3      - L_BtmTrRJ3
        SET_FV( DtmTrCnd3,        4.668F    ); // C3       - L_BtmTrC3
        SET_FV( DtmTrRthNd3Nd2,   0.008F    ); // R32      - L_BtmTrR32
        SET_FV( DtmTrCnd2,        2.8F      ); // C2       - L_BtmTrC2
        SET_FV( DtmTrRthNd2Nd1,   0.003635F ); // R21      - L_BtmTrR21
        SET_FV( DtmTrCnd1,        405.0F    ); // C1       - L_BtmTrC1
        SET_FV( DtmTrRthNd1Clnt,  0.012F    ); // R1C      - L_BtmR1CA
        SET_FV( DtmTrCnd2Ref2,    2.8F      ); // C2 -Air  - L_BtmC2Air
        SET_FV( DtmTrRth21Ref2,   0.003635F ); // R21-Air  - L_BtmR21Air
        SET_FV( DtmTrCnd1Ref2,    206.0F    ); // C1 -Air  - L_BtmC1Air
        SET_FV( DtmTrRth1CRef2,   0.0056F   ); // R12-Air  - L_BtmR1CAir

        SET_FV( DtmJncOtThr,      133.0F    );
        SET_FV( DtmJncOtLockThr,  500.0F    );
        SET_FV( DtmOtVdcUnlckThr, 200.0F    );

        SET_FV(DtmJncHotOnThr,      130.0F);
        SET_FV(DtmJncHotOffThr,     125.0F);
    }
    else       // Values for production WindDFIG01 need to be revised! -- GEN3
    {                                                      //MACC Name

        SET_FV( DtmTrR,           1.04e-03F ); // R        - L_BtmIgbtR
        SET_FV( DtmTrVo,               1.0F ); // Vo       - L_BtmIgbtVo
        SET_FV( DtmTrSw0,         7.17e-05F ); // S0       - L_BtmIgbtS0
        SET_FV( DtmTrSw1,         5.02e-07F ); // Sw1      - L_BtmIgbtSw1
        SET_FV( DtmTrSw2,         1.24e-10F ); // Sw2      - L_BtmIgbtSw2
        SET_FV( DtmTrCJnt,        0.477F    ); // Cj       - L_BtmTrJc
        SET_FV( DtmTrRthJntNd3,   0.0042F   ); // Rj3      - L_BtmTrRJ3
        SET_FV( DtmTrCnd3,        2.334F    ); // C3       - L_BtmTrC3
        SET_FV( DtmTrRthNd3Nd2,   0.016F    ); // R32      - L_BtmTrR32
        SET_FV( DtmTrCnd2,        1.4F      ); // C2       - L_BtmTrC2
        SET_FV( DtmTrRthNd2Nd1,   0.007270F ); // R21      - L_BtmTrR21
        SET_FV( DtmTrCnd1,        202.5F    ); // C1       - L_BtmTrC1
        SET_FV( DtmTrRthNd1Clnt,  0.024F    ); // R1C      - L_BtmR1CA
        SET_FV( DtmTrCnd2Ref2,    1.4F      ); // C2 -Air  - L_BtmC2Air
        SET_FV( DtmTrRth21Ref2,   0.007270F ); // R21-Air  - L_BtmR21Air
        SET_FV( DtmTrCnd1Ref2,    103.0F    ); // C1 -Air  - L_BtmC1Air
        SET_FV( DtmTrRth1CRef2,   0.0112F   ); // R12-Air  - L_BtmR1CAir

        SET_FV( DtmJncOtThr,      105.0F    );
        SET_FV( DtmJncOtLockThr,  125.0F    );
        SET_FV( DtmOtVdcUnlckThr, 200.0F    );

        SET_FV(DtmJncHotOnThr,      95.0F);
        SET_FV(DtmJncHotOffThr,     90.0F);

    }

    SET_FV( DtmTrLqdOtThr,  1.0F ); // An extra deegre allows liquid temp protection to act first
    SET_FV( DtmCoolOnThr,   10.0F );
