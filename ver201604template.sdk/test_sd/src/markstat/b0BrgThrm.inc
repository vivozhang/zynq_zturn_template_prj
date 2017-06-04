///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Bridge thermal model (Btm) class for 2-level PWM inverter
//
// DESCRIPTION:
//      This file contains the rules for bridge thermal model class. This file
//      is defined uniquely per product and is included by b0BrgThrm.cpp.
//      Final value parameters and variables calculated here, are defined
//      by the cBrgThrmModel class in b0BrgThrm.h.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0BrgThrm.h, b0BrgThrm.xls
//
///////////////////////////////////////////////////////////////////////////////

#if 0       // This code is only for reference purposes. Do not use it !

{

    // Thermal characteristics of bridge devices

    if ( INS(CfgForSep) == true )
    {
        SET_FV( BtmTrVo,     1.0F      );
        SET_FV( BtmTrR,      1.04e-03F );
        SET_FV( BtmDiodeVo,  1.0F      );
        SET_FV( BtmDiodeR,   6.00e-04F );
        SET_FV( BtmTrSw0,    7.17e-05F );
        SET_FV( BtmTrSw1,    5.02e-07F );
        SET_FV( BtmTrSw2,    1.24e-10F );
        SET_FV( BtmDiodSw0,  1.16e-04F );
        SET_FV( BtmDiodSw1,  2.88e-07F );
        SET_FV( BtmDiodSw2, -7.94e-11F );
        SET_FV( BtmRaTh,     6.00e-03F );
        SET_FV( BtmDRaTh,    1.00e-02F );
        SET_FV( BtmRbTh,     1.40e-02F );
        SET_FV( BtmDRbTh,    2.33e-02F );
        SET_FV( BtmCaTh,     3.61F     );
        SET_FV( BtmDCaTh,    2.19F     );
    }
    else                // Values for production WindDFIG01 need to be revised!
    {
        SET_FV( BtmTrVo,     1.0F      );
        SET_FV( BtmTrR,      5.20e-04F );
        SET_FV( BtmDiodeVo,  1.0F      );
        SET_FV( BtmDiodeR,   3.00e-04F );
        SET_FV( BtmTrSw0,    1.43e-04F );
        SET_FV( BtmTrSw1,    5.02e-07F );
        SET_FV( BtmTrSw2,    6.22e-11F );
        SET_FV( BtmDiodSw0,  2.32e-04F );
        SET_FV( BtmDiodSw1,  2.88e-07F );
        SET_FV( BtmDiodSw2, -3.97e-11F );
        SET_FV( BtmRaTh,     1.02e-02F );
        SET_FV( BtmDRaTh,    1.55e-02F );
        SET_FV( BtmRbTh,     2.38e-02F );
        SET_FV( BtmDRbTh,    3.63e-02F );
        SET_FV( BtmCaTh,     7.21F     );
        SET_FV( BtmDCaTh,    4.37F     );
    }

    // Diagnostic limits for junction temperature

    SET_FV( BtmJncOTThr,        130.0F );
    SET_FV( BtmJncOTThDly,      125.0F );
    SET_FV( BtmJncOTOn,         120.0F );
    SET_FV( BtmJncOTOff,         95.0F );
    SET_FV( BtmDJncOTThr,       130.0F );
    SET_FV( BtmDJncOTThDly,     125.0F );
    SET_FV( BtmDJncOTOn,        120.0F );
    SET_FV( BtmDJncOTOff,        95.0F );

    // Filter omegas for junction temperature

    SET_FV( BtmWjncUp,          100.0F );
    SET_FV( BtmWjncDn,            1.0F );

}

#endif
