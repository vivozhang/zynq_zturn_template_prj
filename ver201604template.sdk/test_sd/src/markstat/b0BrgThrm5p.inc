///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       5-pole bridge thermal model (Btm) class for 2-level
//              PWM inverter
//
// DESCRIPTION:
//      This file contains the rules for bridge thermal model class. This file
//      is defined uniquely per product and is included by b0BrgThrm5p.cpp.
//      Final value parameters and variables calculated here, are defined
//      by the cBrgThrm5pModel class in b0BrgThrm5p.h.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0BrgThrm.h, b0BrgThrm.xls
//
///////////////////////////////////////////////////////////////////////////////

{
    // configure thermal model type
    if ( INS(ProductType) >= DFIG02_2400_5060_A)
    {
        SET_FV(BtmType, BTM_GEN4_5P_DOUBLE_1500Z);
    }
    else
    {
        if(INS(IgbtParallel))
        {
            SET_FV(BtmType, BTM_GEN3_2P_DOUBLE);
        }
        else
        {
            SET_FV(BtmType, BTM_GEN3_2P_SINGLE);
        }
    }

    // Thermal characteristics of bridge devices
    switch (PARM(BtmType))
    {
    case BTM_GEN4_5P_DOUBLE:    // Gen 4 IGBT for 5P model, double side
    case BTM_GEN4_5P_DOUBLE_1500Z:
        SET_FV( BtmTrCjnc,      0.978F );   // For IGBT nodes
        SET_FV( BtmTrCnd4,      5.080F );
        SET_FV( BtmTrRthJncNd4, 2.000e-3F);
        SET_FV( BtmTrRthNd4Nd3, 8.000e-3F);
        SET_FV( BtmTrRthNd3Nd2, 1.480e-3F);
        SET_FV( BtmDcjnc,       0.594F );   // For diode nodes
        SET_FV( BtmDcnd4,       3.100F );
        SET_FV( BtmDrthJncNd4,  3.330e-3F);
        SET_FV( BtmDrthNd4Nd3, 13.330e-3F);
        SET_FV( BtmDrthNd3Nd2,  1.110e-3F);
        SET_FV( BtmCnd1,      405.000F);    // For common nodes
        SET_FV( BtmRthNd2Nd1,   2.150e-3F);
        SET_FV( BtmRthNd1Clnt, 16.000e-3F);
        SET_FV( BtmRaTh,        0.0F );     // For 2P model
        SET_FV( BtmRbTh,        0.0F );
        SET_FV( BtmCaTh,        0.0F );
        SET_FV( BtmDRaTh,       0.0F );
        SET_FV( BtmDRbTh,       0.0F );
        SET_FV( BtmDCaTh,       0.0F );
        break;
    case BTM_GEN4_5P_SINGLE:    // Gen 4 IGBT for 5P model, single side
        SET_FV( BtmTrCjnc,      0.489F );   // For IGBT nodes
        SET_FV( BtmTrCnd4,      2.540F );
        SET_FV( BtmTrRthJncNd4, 4.000e-3F);
        SET_FV( BtmTrRthNd4Nd3, 1.600e-2F);
        SET_FV( BtmTrRthNd3Nd2, 2.970e-3F);
        SET_FV( BtmDcjnc,       0.297F );   // For diode nodes
        SET_FV( BtmDcnd4,       1.550F );
        SET_FV( BtmDrthJncNd4,  6.670e-3F);
        SET_FV( BtmDrthNd4Nd3,  2.667e-2F);
        SET_FV( BtmDrthNd3Nd2,  2.220e-3F);
        SET_FV( BtmCnd1,      202.5F);    // For common nodes
        SET_FV( BtmRthNd2Nd1,   4.300e-3F);
        SET_FV( BtmRthNd1Clnt,  3.200e-2F);
        SET_FV( BtmRaTh,        0.0F );     // For 2P model
        SET_FV( BtmRbTh,        0.0F );
        SET_FV( BtmCaTh,        0.0F );
        SET_FV( BtmDRaTh,       0.0F );
        SET_FV( BtmDRbTh,       0.0F );
        SET_FV( BtmDCaTh,       0.0F );
        break;
    case BTM_GEN3_5P_DOUBLE:    // Gen 3 IGBT for 5P model, single side
        SET_FV( BtmTrCjnc,      2.028F );   // For IGBT nodes
        SET_FV( BtmTrCnd4,      6.128F );
        SET_FV( BtmTrRthJncNd4, 1.400e-3F);
        SET_FV( BtmTrRthNd4Nd3, 8.600e-3F);
        SET_FV( BtmTrRthNd3Nd2, 1.480e-3F);
        SET_FV( BtmDcjnc,       0.594F );   // For diode nodes
        SET_FV( BtmDcnd4,       3.100F );
        SET_FV( BtmDrthJncNd4,  3.330e-3F);
        SET_FV( BtmDrthNd4Nd3, 13.330e-3F);
        SET_FV( BtmDrthNd3Nd2,  1.110e-3F);
        SET_FV( BtmCnd1,      405.000F);    // For common nodes
        SET_FV( BtmRthNd2Nd1,   2.150e-3F);
        SET_FV( BtmRthNd1Clnt, 16.000e-3F);
        SET_FV( BtmRaTh,        0.0F );     // For 2P model
        SET_FV( BtmRbTh,        0.0F );
        SET_FV( BtmCaTh,        0.0F );
        SET_FV( BtmDRaTh,       0.0F );
        SET_FV( BtmDRbTh,       0.0F );
        SET_FV( BtmDCaTh,       0.0F );
        break;
    case BTM_GEN3_5P_SINGLE:    // Gen 3 IGBT for 5P model, single side
        SET_FV( BtmTrCjnc,      1.014F );   // For IGBT nodes
        SET_FV( BtmTrCnd4,      3.064F );
        SET_FV( BtmTrRthJncNd4, 2.800e-3F);
        SET_FV( BtmTrRthNd4Nd3, 1.720e-2F);
        SET_FV( BtmTrRthNd3Nd2, 2.970e-3F);
        SET_FV( BtmDcjnc,       0.297F );   // For diode nodes
        SET_FV( BtmDcnd4,       1.550F );
        SET_FV( BtmDrthJncNd4,  6.670e-3F);
        SET_FV( BtmDrthNd4Nd3,  2.667e-2F);
        SET_FV( BtmDrthNd3Nd2,  2.220e-3F);
        SET_FV( BtmCnd1,      202.5F);    // For common nodes
        SET_FV( BtmRthNd2Nd1,   4.300e-3F);
        SET_FV( BtmRthNd1Clnt,  3.200e-2F);
        SET_FV( BtmRaTh,        0.0F );     // For 2P model
        SET_FV( BtmRbTh,        0.0F );
        SET_FV( BtmCaTh,        0.0F );
        SET_FV( BtmDRaTh,       0.0F );
        SET_FV( BtmDRbTh,       0.0F );
        SET_FV( BtmDCaTh,       0.0F );
        break;
    case BTM_GEN3_2P_DOUBLE:    // Gen 3 IGBT for 2P model, double side
        SET_FV( BtmTrCjnc,      0.000F );   // For IGBT nodes
        SET_FV( BtmTrCnd4,      7.210F );
        SET_FV( BtmTrRthJncNd4, 3.000e-3F);
        SET_FV( BtmTrRthNd4Nd3, 7.000e-3F);
        SET_FV( BtmTrRthNd3Nd2, 0.000F);
        SET_FV( BtmDcjnc,       0.000F );   // For diode nodes
        SET_FV( BtmDcnd4,       4.370F );
        SET_FV( BtmDrthJncNd4,  5.000e-3F);
        SET_FV( BtmDrthNd4Nd3,  1.170e-2F);
        SET_FV( BtmDrthNd3Nd2,  0.000F);
        SET_FV( BtmCnd1,        0.000F);    // For common nodes
        SET_FV( BtmRthNd2Nd1,   0.000F);
        SET_FV( BtmRthNd1Clnt,  0.000F);
        SET_FV( BtmRaTh,        3.000e-3F );    // For 2P model
        SET_FV( BtmRbTh,        7.000e-3F );
        SET_FV( BtmCaTh,        7.210F );
        SET_FV( BtmDRaTh,       5.000e-3F );
        SET_FV( BtmDRbTh,       1.170e-2F );
        SET_FV( BtmDCaTh,       4.370 );
        break;
    case BTM_GEN3_2P_SINGLE:    // Gen 3 IGBT for 2P model, single side
    default:
        SET_FV( BtmTrCjnc,      0.000F );   // For IGBT nodes
        SET_FV( BtmTrCnd4,      3.610F );
        SET_FV( BtmTrRthJncNd4, 6.000e-3F);
        SET_FV( BtmTrRthNd4Nd3, 1.400e-2F);
        SET_FV( BtmTrRthNd3Nd2, 0.000F);
        SET_FV( BtmDcjnc,       0.000F );   // For diode nodes
        SET_FV( BtmDcnd4,       2.190F );
        SET_FV( BtmDrthJncNd4,  1.000e-2F);
        SET_FV( BtmDrthNd4Nd3,  2.330e-2F);
        SET_FV( BtmDrthNd3Nd2,  0.000F);
        SET_FV( BtmCnd1,        0.000F);    // For common nodes
        SET_FV( BtmRthNd2Nd1,   0.000F);
        SET_FV( BtmRthNd1Clnt,  0.000F);
        SET_FV( BtmRaTh,        6.000e-3F );    // For 2P model
        SET_FV( BtmRbTh,        1.400e-2F );
        SET_FV( BtmCaTh,        3.610F );
        SET_FV( BtmDRaTh,       1.000e-2F );
        SET_FV( BtmDRbTh,       2.330e-2F );
        SET_FV( BtmDCaTh,       2.190F );
        break;
    }

    // power loss model
    switch (PARM(BtmType))
    {
    case BTM_GEN4_5P_DOUBLE:
        SET_FV( BtmTrC1,     0.92583F   );
        SET_FV( BtmTrC2,     6.5798e-04F);
        SET_FV( BtmTrC3,    -2.2802e-08F);
        SET_FV( BtmDiodC1,   0.90203F   );
        SET_FV( BtmDiodC2,   5.5418e-04F);
        SET_FV( BtmDiodC3,  -3.7148e-08F);
        SET_FV( BtmTrBondR,  0.188e-03F );
        SET_FV( BtmDbondR,   0.188e-03F );
        SET_FV( BtmTrSw0,    7.59e-05F  );
        SET_FV( BtmTrSw1,    7.12e-07F  );
        SET_FV( BtmTrSw2,    1.82e-11F  );
        SET_FV( BtmDiodSw0,  2.29e-05F  );
        SET_FV( BtmDiodSw1,  4.78e-07F  );
        SET_FV( BtmDiodSw2, -5.31E-11F  );
        SET_FV( BtmTrVdcSw0,    0.0F );
        SET_FV( BtmTrVdcSw1,    1.0F );
        SET_FV( BtmTrVdcSw2,    0.0F );
        SET_FV( BtmDiVdcSw0,    0.0F );
        SET_FV( BtmDiVdcSw1,    1.0F );
        SET_FV( BtmDiVdcSw2,    0.0F );
        break;
    case BTM_GEN4_5P_SINGLE:
        SET_FV( BtmTrC1,     0.92583F   );
        SET_FV( BtmTrC2,     1.31596e-3F);
        SET_FV( BtmTrC3,    -9.1208e-8F );
        SET_FV( BtmDiodC1,   0.90203F   );
        SET_FV( BtmDiodC2,   1.10836e-3F);
        SET_FV( BtmDiodC3,  -1.48592e-7F);
        SET_FV( BtmTrBondR,  3.76000e-4F);
        SET_FV( BtmDbondR,   3.76000e-4F);
        SET_FV( BtmTrSw0,    3.79500e-5F);
        SET_FV( BtmTrSw1,    7.12000e-7F);
        SET_FV( BtmTrSw2,    3.64000e-11F);
        SET_FV( BtmDiodSw0,  1.14500e-5F);
        SET_FV( BtmDiodSw1,  4.78000e-7F);
        SET_FV( BtmDiodSw2, -1.06200e-10F);
        SET_FV( BtmTrVdcSw0,    0.0F );
        SET_FV( BtmTrVdcSw1,    1.0F );
        SET_FV( BtmTrVdcSw2,    0.0F );
        SET_FV( BtmDiVdcSw0,    0.0F );
        SET_FV( BtmDiVdcSw1,    1.0F );
        SET_FV( BtmDiVdcSw2,    0.0F );
        break;
    case BTM_GEN3_5P_DOUBLE:
    case BTM_GEN3_2P_DOUBLE:
        SET_FV( BtmTrC1,    1.000F );
        SET_FV( BtmTrC2,    5.200e-4F );
        SET_FV( BtmTrC3,    0.000F );
        SET_FV( BtmDiodC1,  1.000F );
        SET_FV( BtmDiodC2,  3.000e-4F );
        SET_FV( BtmDiodC3,  0.000F );
        SET_FV( BtmTrBondR, 0.000F );
        SET_FV( BtmDbondR,  0.000F );
        SET_FV( BtmTrSw0,   1.430e-4F );
        SET_FV( BtmTrSw1,   5.020e-7F );
        SET_FV( BtmTrSw2,   6.220e-11F );
        SET_FV( BtmDiodSw0, 2.320e-4F );
        SET_FV( BtmDiodSw1, 2.880e-7F );
        SET_FV( BtmDiodSw2,-3.970e-11F);
        SET_FV( BtmTrVdcSw0,    0.0F );
        SET_FV( BtmTrVdcSw1,    1.0F );
        SET_FV( BtmTrVdcSw2,    0.0F );
        SET_FV( BtmDiVdcSw0,    0.0F );
        SET_FV( BtmDiVdcSw1,    1.0F );
        SET_FV( BtmDiVdcSw2,    0.0F );
        break;
    case BTM_GEN4_5P_DOUBLE_1500Z:
        SET_FV( BtmTrC1,     0.92583F   );
        SET_FV( BtmTrC2,     6.5798e-04F);
        SET_FV( BtmTrC3,    -2.2802e-08F);
        SET_FV( BtmDiodC1,   0.90203F   );
        SET_FV( BtmDiodC2,   5.5418e-04F);
        SET_FV( BtmDiodC3,  -3.7148e-08F);
        SET_FV( BtmTrBondR,  0.188e-03F );
        SET_FV( BtmDbondR,   0.188e-03F );
        SET_FV( BtmTrSw0,   1.3689e-1F );
        SET_FV( BtmTrSw1,   7.6805e-4F );
        SET_FV( BtmTrSw2,   9.3527e-8F );
        SET_FV( BtmDiodSw0, 3.9109e-2F );
        SET_FV( BtmDiodSw1, 4.8422e-4F );
        SET_FV( BtmDiodSw2,-5.8522e-8F );
        SET_FV( BtmTrVdcSw0,    6.8475e-1F );
        SET_FV( BtmTrVdcSw1,   -7.9850e-4F );
        SET_FV( BtmTrVdcSw2,    9.1048e-7F );
        SET_FV( BtmDiVdcSw0,    0.0F );
        SET_FV( BtmDiVdcSw1,    8.3333e-4F );
        SET_FV( BtmDiVdcSw2,    0.0F );
        break;
    case BTM_GEN3_5P_SINGLE:
    case BTM_GEN3_2P_SINGLE:
    default:
        SET_FV( BtmTrC1,    1.000F );
        SET_FV( BtmTrC2,    1.040e-3F );
        SET_FV( BtmTrC3,    0.000F );
        SET_FV( BtmDiodC1,  1.000F );
        SET_FV( BtmDiodC2,  6.000e-4F );
        SET_FV( BtmDiodC3,  0.000F );
        SET_FV( BtmTrBondR, 0.000F );
        SET_FV( BtmDbondR,  0.000F );
        SET_FV( BtmTrSw0,   7.170e-5F );
        SET_FV( BtmTrSw1,   5.020e-7F );
        SET_FV( BtmTrSw2,   1.240e-10F );
        SET_FV( BtmDiodSw0, 1.160e-4F );
        SET_FV( BtmDiodSw1, 2.880e-7F );
        SET_FV( BtmDiodSw2,-7.940e-11F );
        SET_FV( BtmTrVdcSw0,    0.0F );
        SET_FV( BtmTrVdcSw1,    1.0F );
        SET_FV( BtmTrVdcSw2,    0.0F );
        SET_FV( BtmDiVdcSw0,    0.0F );
        SET_FV( BtmDiVdcSw1,    1.0F );
        SET_FV( BtmDiVdcSw2,    0.0F );
        break;
    }


    // Shut-down limits for junction temperature
    switch (PARM(BtmType))
    {
    case BTM_GEN4_5P_DOUBLE:
    case BTM_GEN4_5P_SINGLE:
    case BTM_GEN4_5P_DOUBLE_1500Z:
        //Transistor
        SET_FV( BtmTrJncOTThr,      140.0F );
        SET_FV( BtmTrJncOTThDly,    135.0F );
        SET_FV( BtmTrJncOTOn,       130.0F );
        SET_FV( BtmTrJncOTOff,      105.0F );

        //Diode
        SET_FV( BtmDjncOTThr,       140.0F );
        SET_FV( BtmDjncOTThDly,     135.0F );
        SET_FV( BtmDjncOTOn,        130.0F );
        SET_FV( BtmDjncOTOff,       105.0F );

        //Pumpback over-writes
        if ( INS(TestMode) == PUMP_BACK )
        {
            SET_FV( BtmTrJncOTThDly,145.0F );
            SET_FV( BtmDjncOTThDly, 145.0F );
        }
        break;
    case BTM_GEN3_2P_DOUBLE:
    case BTM_GEN3_2P_SINGLE:
    case BTM_GEN3_5P_DOUBLE:
    case BTM_GEN3_5P_SINGLE:
    default:
        //Transistor
        SET_FV( BtmTrJncOTThr,      125.0F );
        SET_FV( BtmTrJncOTThDly,    120.0F );
        SET_FV( BtmTrJncOTOn,       115.0F );
        SET_FV( BtmTrJncOTOff,       90.0F );

        //Diode
        SET_FV( BtmDjncOTThr,       125.0F );
        SET_FV( BtmDjncOTThDly,     120.0F );
        SET_FV( BtmDjncOTOn,        115.0F );
        SET_FV( BtmDjncOTOff,        90.0F );

        //Pumpback over-writes
        if ( INS(TestMode) == PUMP_BACK )
        {
            SET_FV( BtmTrJncOTThDly,130.0F );
            SET_FV( BtmDjncOTThDly, 130.0F );
        }
        break;
    }
    // Filter omegas for junction temperature

    SET_FV( BtmWjncUp,          100.0F );
    SET_FV( BtmWjncDn,            1.0F );

}
