///////////////////////////////////////////////////////////////////////////////
//
// TITLE     5-pole bridge thermal model (Btm) class for 2-level
//           PWM inverter
//
// DESCRIPTION
//           This file contains the methods for the bridge thermal model class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0BrgThrm5p.vsd, b0BrgThrm5p.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
// product
#include "Product.h"
#include "b0BrgThrm5p.h"


// Constants
//----------
#define BTM_GEN4_5P_DOUBLE  0
#define BTM_GEN3_2P_SINGLE  1
#define BTM_GEN3_2P_DOUBLE  2
#define BTM_GEN4_5P_SINGLE  3
#define BTM_GEN3_5P_SINGLE  4
#define BTM_GEN3_5P_DOUBLE  5
#define BTM_GEN4_5P_DOUBLE_1500Z  6
#define BTM_GN_CONST        (0.582F)
// Static Class Members (by default, compiler initializes them to zero
//                       if no explicit initialization is provided):
//--------------------------------------------------------------------


// Externals
//----------


// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Variables
//----------


// Member Functions
//-----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class constructor to initialize resources
//
// EXECUTION LEVEL
//      Class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cBrgThrm5pModel::cBrgThrm5pModel
(
        InListTyp    InList,
        InsListTyp   InsList
)
        : In(InList), Ins(InsList),
    // Use structure constructor initializer list, without curly braces,
    // to bind class members to parameter-lists connecting to class methods
           RcNetworkPlistJncTAU( BtmTrPwrAUT2, BtmTrNd4TempAU, BtmTrJnGn,
                                 BtmTrRthJnNd4Rcp, PARM(BtmTrRthJncNd4),
                                 BtmTrJncTempAU, BtmTrPwrJnNd4AU ),
           RcNetworkPlistJncTAL( BtmTrPwrALT2, BtmTrNd4TempAL, BtmTrJnGn,
                                 BtmTrRthJnNd4Rcp, PARM(BtmTrRthJncNd4),
                                 BtmTrJncTempAL, BtmTrPwrJnNd4AL ),
           RcNetworkPlistJncTBU( BtmTrPwrBUT2, BtmTrNd4TempBU, BtmTrJnGn,
                                 BtmTrRthJnNd4Rcp, PARM(BtmTrRthJncNd4),
                                 BtmTrJncTempBU, BtmTrPwrJnNd4BU ),
           RcNetworkPlistJncTBL( BtmTrPwrBLT2, BtmTrNd4TempBL, BtmTrJnGn,
                                 BtmTrRthJnNd4Rcp, PARM(BtmTrRthJncNd4),
                                 BtmTrJncTempBL, BtmTrPwrJnNd4BL ),
           RcNetworkPlistJncTCU( BtmTrPwrCUT2, BtmTrNd4TempCU, BtmTrJnGn,
                                 BtmTrRthJnNd4Rcp, PARM(BtmTrRthJncNd4),
                                 BtmTrJncTempCU, BtmTrPwrJnNd4CU ),
           RcNetworkPlistJncTCL( BtmTrPwrCLT2, BtmTrNd4TempCL, BtmTrJnGn,
                                 BtmTrRthJnNd4Rcp, PARM(BtmTrRthJncNd4),
                                 BtmTrJncTempCL, BtmTrPwrJnNd4CL ),
           RcNetworkPlistJncDAU( BtmDpwrAUT2, BtmDnd4TempAU, BtmDjnGn,
                                 BtmDrthJnNd4Rcp, PARM(BtmDrthJncNd4),
                                 BtmDjncTempAU, BtmDpwrJnNd4AU ),
           RcNetworkPlistJncDAL( BtmDpwrALT2, BtmDnd4TempAL, BtmDjnGn,
                                 BtmDrthJnNd4Rcp, PARM(BtmDrthJncNd4),
                                 BtmDjncTempAL, BtmDpwrJnNd4AL ),
           RcNetworkPlistJncDBU( BtmDpwrBUT2, BtmDnd4TempBU, BtmDjnGn,
                                 BtmDrthJnNd4Rcp, PARM(BtmDrthJncNd4),
                                 BtmDjncTempBU, BtmDpwrJnNd4BU ),
           RcNetworkPlistJncDBL( BtmDpwrBLT2, BtmDnd4TempBL, BtmDjnGn,
                                 BtmDrthJnNd4Rcp, PARM(BtmDrthJncNd4),
                                 BtmDjncTempBL, BtmDpwrJnNd4BL ),
           RcNetworkPlistJncDCU( BtmDpwrCUT2, BtmDnd4TempCU, BtmDjnGn,
                                 BtmDrthJnNd4Rcp, PARM(BtmDrthJncNd4),
                                 BtmDjncTempCU, BtmDpwrJnNd4CU ),
           RcNetworkPlistJncDCL( BtmDpwrCLT2, BtmDnd4TempCL, BtmDjnGn,
                                 BtmDrthJnNd4Rcp, PARM(BtmDrthJncNd4),
                                 BtmDjncTempCL, BtmDpwrJnNd4CL ),
           RcNetworkPlistNd4TAU( BtmTrPwrJnNd4AU, BtmTrNd3TempAU, BtmTrCnd4Gn,
                                 BtmTrRthNd4Nd3Rcp, PARM(BtmTrRthNd4Nd3),
                                 BtmTrNd4TempAU, BtmPowerHolder ),
           RcNetworkPlistNd4TAL( BtmTrPwrJnNd4AL, BtmTrNd3TempAL, BtmTrCnd4Gn,
                                 BtmTrRthNd4Nd3Rcp, PARM(BtmTrRthNd4Nd3),
                                 BtmTrNd4TempAL, BtmPowerHolder ),
           RcNetworkPlistNd4TBU( BtmTrPwrJnNd4BU, BtmTrNd3TempBU, BtmTrCnd4Gn,
                                 BtmTrRthNd4Nd3Rcp, PARM(BtmTrRthNd4Nd3),
                                 BtmTrNd4TempBU, BtmPowerHolder ),
           RcNetworkPlistNd4TBL( BtmTrPwrJnNd4BL, BtmTrNd3TempBL, BtmTrCnd4Gn,
                                 BtmTrRthNd4Nd3Rcp, PARM(BtmTrRthNd4Nd3),
                                 BtmTrNd4TempBL, BtmPowerHolder ),
           RcNetworkPlistNd4TCU( BtmTrPwrJnNd4CU, BtmTrNd3TempCU, BtmTrCnd4Gn,
                                 BtmTrRthNd4Nd3Rcp, PARM(BtmTrRthNd4Nd3),
                                 BtmTrNd4TempCU, BtmPowerHolder ),
           RcNetworkPlistNd4TCL( BtmTrPwrJnNd4CL, BtmTrNd3TempCL, BtmTrCnd4Gn,
                                 BtmTrRthNd4Nd3Rcp, PARM(BtmTrRthNd4Nd3),
                                 BtmTrNd4TempCL, BtmPowerHolder ),
           RcNetworkPlistNd4DAU( BtmDpwrJnNd4AU, BtmDnd3TempAU, BtmDcnd4Gn,
                                 BtmDrthNd4Nd3Rcp, PARM(BtmDrthNd4Nd3),
                                 BtmDnd4TempAU, BtmPowerHolder ),
           RcNetworkPlistNd4DAL( BtmDpwrJnNd4AL, BtmDnd3TempAL, BtmDcnd4Gn,
                                 BtmDrthNd4Nd3Rcp, PARM(BtmDrthNd4Nd3),
                                 BtmDnd4TempAL, BtmPowerHolder ),
           RcNetworkPlistNd4DBU( BtmDpwrJnNd4BU, BtmDnd3TempBU, BtmDcnd4Gn,
                                 BtmDrthNd4Nd3Rcp, PARM(BtmDrthNd4Nd3),
                                 BtmDnd4TempBU, BtmPowerHolder ),
           RcNetworkPlistNd4DBL( BtmDpwrJnNd4BL, BtmDnd3TempBL, BtmDcnd4Gn,
                                 BtmDrthNd4Nd3Rcp, PARM(BtmDrthNd4Nd3),
                                 BtmDnd4TempBL, BtmPowerHolder ),
           RcNetworkPlistNd4DCU( BtmDpwrJnNd4CU, BtmDnd3TempCU, BtmDcnd4Gn,
                                 BtmDrthNd4Nd3Rcp, PARM(BtmDrthNd4Nd3),
                                 BtmDnd4TempCU, BtmPowerHolder ),
           RcNetworkPlistNd4DCL( BtmDpwrJnNd4CL, BtmDnd3TempCL,BtmDcnd4Gn,
                                 BtmDrthNd4Nd3Rcp, PARM(BtmDrthNd4Nd3),
                                 BtmDnd4TempCL, BtmPowerHolder ),
           RcNetworkPlistNd1AU(  BtmPwrNd2Nd1AUT3, IN(CoolantTemp), BtmCnd1Gn,
                                 BtmRthNd1ClnRcp, PARM(BtmRthNd1Clnt),
                                 BtmNd1TempAU, BtmPwrNd1ClnAU ),
           RcNetworkPlistNd1AL(  BtmPwrNd2Nd1ALT3, IN(CoolantTemp), BtmCnd1Gn,
                                 BtmRthNd1ClnRcp, PARM(BtmRthNd1Clnt),
                                 BtmNd1TempAL, BtmPwrNd1ClnAL ),
           RcNetworkPlistNd1BU(  BtmPwrNd2Nd1BUT3, IN(CoolantTemp), BtmCnd1Gn,
                                 BtmRthNd1ClnRcp, PARM(BtmRthNd1Clnt),
                                 BtmNd1TempBU, BtmPwrNd1ClnBU ),
           RcNetworkPlistNd1BL(  BtmPwrNd2Nd1BLT3, IN(CoolantTemp), BtmCnd1Gn,
                                 BtmRthNd1ClnRcp, PARM(BtmRthNd1Clnt),
                                 BtmNd1TempBL, BtmPwrNd1ClnBL ),
           RcNetworkPlistNd1CU(  BtmPwrNd2Nd1CUT3, IN(CoolantTemp), BtmCnd1Gn,
                                 BtmRthNd1ClnRcp, PARM(BtmRthNd1Clnt),
                                 BtmNd1TempCU, BtmPwrNd1ClnCU ),
           RcNetworkPlistNd1CL(  BtmPwrNd2Nd1CLT3, IN(CoolantTemp), BtmCnd1Gn,
                                 BtmRthNd1ClnRcp, PARM(BtmRthNd1Clnt),
                                 BtmNd1TempCL, BtmPwrNd1ClnCL )
{
    BtmAlarmStPhsA        = false;
    BtmAlarmStPhsB        = false;
    BtmAlarmStPhsC        = false;
    BtmAlarmStDphsA       = false;
    BtmAlarmStDphsB       = false;
    BtmAlarmStDphsC       = false;

    // memset( reinterpret_cast<void*> (&BtmUcmdZ1), 0, sizeof(BtmUcmdZ1) );

    BtmIaAbs              = 0.0F;
    BtmIbAbs              = 0.0F;
    BtmIcAbs              = 0.0F;
    BtmIaSqr              = 0.0F;
    BtmIbSqr              = 0.0F;
    BtmIcSqr              = 0.0F;
    BtmUaOld              = 0.0F;
    BtmUbOld              = 0.0F;
    BtmUcOld              = 0.0F;
    // FullTmRcp             = 0.0F;
    BtmUOldThrHi          = 0.0F;
    BtmUOldThrLo          = 0.0F;
    BtmTrSwPwr            = 0.0F;
    BtmTrCondPwr          = 0.0F;
    BtmDswPwr             = 0.0F;
    BtmDcondPwr           = 0.0F;
    BtmTrBondPwr          = 0.0F;
    BtmDbondPwr           = 0.0F;
    BtmTrSwGn             = 0.0F;
    BtmTrSwGnT1           = 0.0F;
    BtmDswGn              = 0.0F;
    BtmDswGnT1            = 0.0F;
    BtmTrPwrAL            = 0.0F;
    BtmTrPwrAU            = 0.0F;
    BtmTrPwrBL            = 0.0F;
    BtmTrPwrBU            = 0.0F;
    BtmTrPwrCL            = 0.0F;
    BtmTrPwrCU            = 0.0F;
    BtmDpwrAL             = 0.0F;
    BtmDpwrAU             = 0.0F;
    BtmDpwrBL             = 0.0F;
    BtmDpwrBU             = 0.0F;
    BtmDpwrCL             = 0.0F;
    BtmDpwrCU             = 0.0F;
    BtmTrJncTempAUT3      = 0.0F;
    BtmTrJncTempALT3      = 0.0F;
    BtmTrJncTempBUT3      = 0.0F;
    BtmTrJncTempBLT3      = 0.0F;
    BtmTrJncTempCUT3      = 0.0F;
    BtmTrJncTempCLT3      = 0.0F;
    BtmDjncTempAUT3       = 0.0F;
    BtmDjncTempALT3       = 0.0F;
    BtmDjncTempBUT3       = 0.0F;
    BtmDjncTempBLT3       = 0.0F;
    BtmDjncTempCUT3       = 0.0F;
    BtmDjncTempCLT3       = 0.0F;
    BtmCnd1Gn             = 0.0F;
    BtmRthNd1ClnRcp       = 0.0F;
    BtmTrCnd4Gn           = 0.0F;
    BtmTrRthNd4Nd3Rcp     = 0.0F;
    BtmTrRthNd4Nd2Rcp     = 0.0F;
    BtmDcnd4Gn            = 0.0F;
    BtmDrthNd4Nd3Rcp      = 0.0F;
    BtmDrthNd4Nd2Rcp      = 0.0F;
    BtmTrRthJnNd4Rcp      = 0.0F;
    BtmDrthJnNd4Rcp       = 0.0F;
    BtmRthNd4Nd2Prll      = 0.0F;
    BtmNd2Divider         = 0.0F;
    BtmRthNd2Nd1Rcp       = 0.0F;
    BtmTrNd4TempAU        = 0.0F;
    BtmTrNd4TempAL        = 0.0F;
    BtmTrNd4TempBU        = 0.0F;
    BtmTrNd4TempBL        = 0.0F;
    BtmTrNd4TempCU        = 0.0F;
    BtmTrNd4TempCL        = 0.0F;
    BtmDnd4TempAU         = 0.0F;
    BtmDnd4TempAL         = 0.0F;
    BtmDnd4TempBU         = 0.0F;
    BtmDnd4TempBL         = 0.0F;
    BtmDnd4TempCU         = 0.0F;
    BtmDnd4TempCL         = 0.0F;
    BtmTrPwrAUT2          = 0.0F;
    BtmTrPwrALT2          = 0.0F;
    BtmTrPwrBUT2          = 0.0F;
    BtmTrPwrBLT2          = 0.0F;
    BtmTrPwrCUT2          = 0.0F;
    BtmTrPwrCLT2          = 0.0F;
    BtmDpwrALT2           = 0.0F;
    BtmDpwrAUT2           = 0.0F;
    BtmDpwrBUT2           = 0.0F;
    BtmDpwrBLT2           = 0.0F;
    BtmDpwrCUT2           = 0.0F;
    BtmDpwrCLT2           = 0.0F;
    BtmTrPwrJnNd4AU       = 0.0F;
    BtmTrPwrJnNd4AL       = 0.0F;
    BtmTrPwrJnNd4BU       = 0.0F;
    BtmTrPwrJnNd4BL       = 0.0F;
    BtmTrPwrJnNd4CU       = 0.0F;
    BtmTrPwrJnNd4CL       = 0.0F;
    BtmDpwrJnNd4AU        = 0.0F;
    BtmDpwrJnNd4AL        = 0.0F;
    BtmDpwrJnNd4BU        = 0.0F;
    BtmDpwrJnNd4BL        = 0.0F;
    BtmDpwrJnNd4CU        = 0.0F;
    BtmDpwrJnNd4CL        = 0.0F;
    BtmTrJnGn             = 0.0F;
    BtmDjnGn              = 0.0F;
    BtmTrJncTempAL        = 0.0F;
    BtmTrJncTempAU        = 0.0F;
    BtmTrJncTempBL        = 0.0F;
    BtmTrJncTempBU        = 0.0F;
    BtmTrJncTempCL        = 0.0F;
    BtmTrJncTempCU        = 0.0F;
    BtmDjncTempAL         = 0.0F;
    BtmDjncTempAU         = 0.0F;
    BtmDjncTempBL         = 0.0F;
    BtmDjncTempBU         = 0.0F;
    BtmDjncTempCL         = 0.0F;
    BtmDjncTempCU         = 0.0F;
    BtmTrJncTempMax       = 0.0F;
    BtmTrJncTempErr       = 0.0F;
    BtmDjncTempMx         = 0.0F;
    BtmDjncTempErr        = 0.0F;
    BtmJncTempUpGn        = 0.0F;
    BtmJncTempDnGn        = 0.0F;
    BtmTrJncOvrTmpThr     = 0.0F;
    BtmTrJncOvrTmpOn      = 0.0F;
    BtmTrJncOvrTmpOff     = 0.0F;
    BtmDjncOvrTempThr     = 0.0F;
    BtmDjncOvrTempOn      = 0.0F;
    BtmDjncOvrTempOff     = 0.0F;
    BtmTrPwrNd3Nd2AU      = 0.0F;
    BtmTrPwrNd3Nd2AL      = 0.0F;
    BtmTrPwrNd3Nd2BU      = 0.0F;
    BtmTrPwrNd3Nd2BL      = 0.0F;
    BtmTrPwrNd3Nd2CU      = 0.0F;
    BtmTrPwrNd3Nd2CL      = 0.0F;
    BtmDpwrNd3Nd2AU       = 0.0F;
    BtmDpwrNd3Nd2AL       = 0.0F;
    BtmDpwrNd3Nd2BU       = 0.0F;
    BtmDpwrNd3Nd2BL       = 0.0F;
    BtmDpwrNd3Nd2CU       = 0.0F;
    BtmDpwrNd3Nd2CL       = 0.0F;
    BtmTrBondPwrAU        = 0.0F;
    BtmTrBondPwrBU        = 0.0F;
    BtmTrBondPwrCU        = 0.0F;
    BtmTrBondPwrAL        = 0.0F;
    BtmTrBondPwrBL        = 0.0F;
    BtmTrBondPwrCL        = 0.0F;
    BtmDbondPwrAU         = 0.0F;
    BtmDbondPwrBU         = 0.0F;
    BtmDbondPwrCU         = 0.0F;
    BtmDbondPwrAL         = 0.0F;
    BtmDbondPwrBL         = 0.0F;
    BtmDbondPwrCL         = 0.0F;
    BtmTrBondPwrAUT2      = 0.0F;
    BtmTrBondPwrALT2      = 0.0F;
    BtmDbondPwrAUT2       = 0.0F;
    BtmDbondPwrALT2       = 0.0F;
    BtmTrBondPwrBUT2      = 0.0F;
    BtmTrBondPwrBLT2      = 0.0F;
    BtmDbondPwrBUT2       = 0.0F;
    BtmDbondPwrBLT2       = 0.0F;
    BtmTrBondPwrCUT2      = 0.0F;
    BtmTrBondPwrCLT2      = 0.0F;
    BtmDbondPwrCUT2       = 0.0F;
    BtmDbondPwrCLT2       = 0.0F;
    BtmPwrNd2Nd1AU        = 0.0F;
    BtmPwrNd2Nd1AL        = 0.0F;
    BtmNd2TempAU          = 0.0F;
    BtmNd2TempAL          = 0.0F;
    BtmPwrNd2Nd1BU        = 0.0F;
    BtmPwrNd2Nd1BL        = 0.0F;
    BtmNd2TempBU          = 0.0F;
    BtmNd2TempBL          = 0.0F;
    BtmPwrNd2Nd1CU        = 0.0F;
    BtmPwrNd2Nd1CL        = 0.0F;
    BtmNd2TempCU          = 0.0F;
    BtmNd2TempCL          = 0.0F;
    BtmTrNd3TempAU        = 0.0F;
    BtmTrNd3TempAL        = 0.0F;
    BtmTrNd3TempBU        = 0.0F;
    BtmTrNd3TempBL        = 0.0F;
    BtmTrNd3TempCU        = 0.0F;
    BtmTrNd3TempCL        = 0.0F;
    BtmDnd3TempAU         = 0.0F;
    BtmDnd3TempAL         = 0.0F;
    BtmDnd3TempBU         = 0.0F;
    BtmDnd3TempBL         = 0.0F;
    BtmDnd3TempCU         = 0.0F;
    BtmDnd3TempCL         = 0.0F;
    BtmPwrNd2Nd1AUT3      = 0.0F;
    BtmPwrNd2Nd1ALT3      = 0.0F;
    BtmPwrNd2Nd1BUT3      = 0.0F;
    BtmPwrNd2Nd1BLT3      = 0.0F;
    BtmPwrNd2Nd1CUT3      = 0.0F;
    BtmPwrNd2Nd1CLT3      = 0.0F;
    BtmNd1TempAU          = 0.0F;
    BtmNd1TempAL          = 0.0F;
    BtmNd1TempBU          = 0.0F;
    BtmNd1TempBL          = 0.0F;
    BtmNd1TempCU          = 0.0F;
    BtmNd1TempCL          = 0.0F;
    BtmNd1TempAUT2        = 0.0F;
    BtmNd1TempALT2        = 0.0F;
    BtmNd1TempBUT2        = 0.0F;
    BtmNd1TempBLT2        = 0.0F;
    BtmNd1TempCUT2        = 0.0F;
    BtmNd1TempCLT2        = 0.0F;
    BtmPwrNd1ClnAU        = 0.0F;
    BtmPwrNd1ClnAL        = 0.0F;
    BtmPwrNd1ClnBU        = 0.0F;
    BtmPwrNd1ClnBL        = 0.0F;
    BtmPwrNd1ClnCU        = 0.0F;
    BtmPwrNd1ClnCL        = 0.0F;
    BtmTrSwchPwrAU        = 0.0F;
    BtmTrSwchPwrAL        = 0.0F;
    BtmTrSwchPwrBU        = 0.0F;
    BtmTrSwchPwrBL        = 0.0F;
    BtmTrSwchPwrCU        = 0.0F;
    BtmTrSwchPwrCL        = 0.0F;
    BtmTrCondPwrAU        = 0.0F;
    BtmTrCondPwrAL        = 0.0F;
    BtmTrCondPwrBU        = 0.0F;
    BtmTrCondPwrBL        = 0.0F;
    BtmTrCondPwrCU        = 0.0F;
    BtmTrCondPwrCL        = 0.0F;
    BtmDswchPwrAU         = 0.0F;
    BtmDswchPwrAL         = 0.0F;
    BtmDswchPwrBU         = 0.0F;
    BtmDswchPwrBL         = 0.0F;
    BtmDswchPwrCU         = 0.0F;
    BtmDswchPwrCL         = 0.0F;
    BtmDcondPwrAU         = 0.0F;
    BtmDcondPwrAL         = 0.0F;
    BtmDcondPwrBU         = 0.0F;
    BtmDcondPwrBL         = 0.0F;
    BtmDcondPwrCU         = 0.0F;
    BtmDcondPwrCL         = 0.0F;
    BtmTrCondPwrLoss      = 0.0F;
    BtmTrSwchPwrLoss      = 0.0F;
    BtmDcondPwrLoss       = 0.0F;
    BtmDswchPwrLoss       = 0.0F;
    BtmBondPwrLoss        = 0.0F;
    BtmPowerHolder        = 0.0F;
    BtmRefTempValid       = false;
    BtmRefTempValidT2     = false;

    OUT(BtmDpwrLoss)      = 0.0F;
    OUT(BtmTrPwrLoss)     = 0.0F;
    OUT(BtmTrJncTemp)     = 0.0F;
    OUT(BtmDjncTemp)      = 0.0F;
    OUT(BtmOtFaultSum)    = false;
    OUT(BtmOtFaultDlySum) = false;
    OUT(BtmOtAlarmSum)    = false;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cBrgThrm5pModel::~cBrgThrm5pModel()
{
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by bridge thermal model (Btm) class.
//
// EXECUTION LEVEL
//      Initialization and when required.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cBrgThrm5pModel::RunRules(void)
{
    int Status = SUCCESS;

    #include "b0BrgThrm5p.inc"      // product defined rules

    // Calculate vars and parms used in Task 1 power loss calculation
    // FullTmRcp    = NoNANLim( 1.0F / static_cast<float>(INS(FullTm)), 1.0e6F, -1.0e6F );
    BtmUOldThrLo = NoNANLim( INS(HalfQuantPerT1Rcp) * 0.25F, 1.0e6F, -1.0e6F );
    BtmUOldThrHi = NoNANLim( 1.0F - ( INS(HalfQuantPerT1Rcp) * 0.25F ), 1.0e6F, -1.0e6F );
    SET_FV(BtmIocThrs, INS(IocThrs));

    Btm2PmdlSel = (PARM(BtmType)==BTM_GEN3_2P_DOUBLE || PARM(BtmType)==BTM_GEN3_2P_SINGLE);
    if(!Btm2PmdlSel)
    {
        // Calculate variables used in Task 3 temp calculations
        BtmCnd1Gn       = NoNANLim( INS(DelTm3)/(PARM(BtmCnd1)*PARM(BtmRthNd1Clnt)),  1.0e6F, -1.0e6F );
        BtmRthNd1ClnRcp = NoNANLim( 1.0F / PARM(BtmRthNd1Clnt), 1.0e6F, -1.0e6F );

        // Calculate variables used in Task 2 temp calculations
        BtmTrCnd4Gn       = NoNANLim( INS(DelTm2)/(PARM(BtmTrCnd4)*PARM(BtmTrRthNd4Nd3)), 1.0e6F, -1.0e6F );
        BtmTrRthNd4Nd3Rcp = NoNANLim( 1.0F / PARM(BtmTrRthNd4Nd3), 1.0e6F, -1.0e6F );
        BtmTrRthNd4Nd2Rcp = NoNANLim( 1.0F / (PARM(BtmTrRthNd4Nd3)+PARM(BtmTrRthNd3Nd2)), 1.0e6F, -1.0e6F );
        BtmDcnd4Gn        = NoNANLim( INS(DelTm2)/(PARM(BtmDcnd4)*PARM(BtmDrthNd4Nd3)),  1.0e6F, -1.0e6F );
        BtmDrthNd4Nd3Rcp  = NoNANLim( 1.0F / PARM(BtmDrthNd4Nd3),  1.0e6F, -1.0e6F );
        BtmDrthNd4Nd2Rcp  = NoNANLim( 1.0F / (PARM(BtmDrthNd4Nd3)+PARM(BtmDrthNd3Nd2)), 1.0e6F, -1.0e6F );
        BtmRthNd4Nd2Prll  = NoNANLim( 1.0F / (BtmTrRthNd4Nd2Rcp + BtmDrthNd4Nd2Rcp), 1.0e6F, -1.0e6F );
        BtmNd2Divider     = NoNANLim( PARM(BtmRthNd2Nd1) / (BtmRthNd4Nd2Prll+PARM(BtmRthNd2Nd1)), 1.0e6F, -1.0e6F );
        BtmRthNd2Nd1Rcp   = NoNANLim( 1.0F / PARM(BtmRthNd2Nd1), 1.0e6F, -1.0e6F );

        // Calculate variables used in Task 2 junction temp calculations
        BtmTrRthJnNd4Rcp = NoNANLim( 1.0F / PARM(BtmTrRthJncNd4), 1.0e6F, -1.0e6F );
        BtmDrthJnNd4Rcp  = NoNANLim( 1.0F / PARM(BtmDrthJncNd4),  1.0e6F, -1.0e6F );

        // Gain calculation for fastest time-constant (junction) being resolved in Task2
        // this is necessary to reduce error when resulting gain approaches unity
        BtmTrJnGn = NoNANLim( 1.0F - exp(-INS(DelTm2)/(PARM(BtmTrCjnc)*PARM(BtmTrRthJncNd4))), 1.0e6F, -1.0e6F );
        BtmDjnGn  = NoNANLim( 1.0F - exp(-INS(DelTm2)/(PARM(BtmDcjnc)*PARM(BtmDrthJncNd4))), 1.0e6F, -1.0e6F );

        BtmRbRcp  = 0.0F;
        BtmDRbRcp = 0.0F;
        BtmTaGn   = 0.0F;
        BtmDTaGn  = 0.0F;
    }
    else
    {
        BtmTrJnGn = 1.0F;
        BtmDjnGn  = 1.0F;
        // Gain calculation for fastest time-constant (junction) being resolved in Task2
        // this is necessary to reduce error when resulting gain approaches unity
        BtmTrRthJnNd4Rcp = NoNANLim( 1.0F / PARM(BtmTrRthJncNd4), 1.0e6F, -1.0e6F );
        BtmDrthJnNd4Rcp  = NoNANLim( 1.0F / PARM(BtmDrthJncNd4),  1.0e6F, -1.0e6F );

        BtmTrCnd4Gn       = NoNANLim( INS(DelTm3)/(PARM(BtmTrCnd4)*PARM(BtmTrRthNd4Nd3)), 1.0e6F, -1.0e6F );
        BtmTrRthNd4Nd3Rcp = NoNANLim( 1.0F / PARM(BtmTrRthNd4Nd3), 1.0e6F, -1.0e6F );
        BtmDcnd4Gn        = NoNANLim( INS(DelTm3)/(PARM(BtmDcnd4)*PARM(BtmDrthNd4Nd3)),  1.0e6F, -1.0e6F );
        BtmDrthNd4Nd3Rcp  = NoNANLim( 1.0F / PARM(BtmDrthNd4Nd3),  1.0e6F, -1.0e6F );

        BtmTrRthNd4Nd2Rcp = 0.0F;

        BtmDrthNd4Nd2Rcp  = 0.0F;
        BtmRthNd4Nd2Prll  = 0.0F;
        BtmNd2Divider     = 0.0F;
        BtmRthNd2Nd1Rcp   = 0.0F;
        BtmCnd1Gn         =0.0F;
        BtmRthNd1ClnRcp   =0.0F;

        BtmRbRcp  = NoNANLim( 1.0F / PARM(BtmRbTh),  1.0e6F, -1.0e6F );
        BtmDRbRcp = NoNANLim( 1.0F / PARM(BtmDRbTh), 1.0e6F, -1.0e6F );
        BtmTaGn   = NoNANLim( INS(DelTm3) / (( INS(DelTm3) * BTM_GN_CONST * BtmRbRcp ) + PARM(BtmCaTh)),
                          1.0e6F, -1.0e6F);
        BtmDTaGn  = NoNANLim( INS(DelTm3) / (( INS(DelTm3) * BTM_GN_CONST * BtmDRbRcp ) + PARM(BtmDCaTh)),
                          1.0e6F, -1.0e6F);
    }

    // Up/Down filter gains
    BtmJncTempUpGn = INS(DelTm2) * PARM(BtmWjncUp);
    BtmJncTempDnGn = INS(DelTm2) * PARM(BtmWjncDn);

    // Copy parameter to private variables
    BtmTrJncOvrTmpThr = PARM(BtmTrJncOTThr);
    BtmTrJncOvrTmpOn  = PARM(BtmTrJncOTOn);
    BtmTrJncOvrTmpOff = PARM(BtmTrJncOTOff);
    BtmDjncOvrTempThr = PARM(BtmDjncOTThr);
    BtmDjncOvrTempOn  = PARM(BtmDjncOTOn);
    BtmDjncOvrTempOff = PARM(BtmDjncOTOff);

    return (Status);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    This function calculates transistor's conduction and switching losses,
//    diode's conduction and switching losses, and bond wire losses for each
//    of the phases in a 2-level power bridge.
//
//    Note: This function must run after adquiring, processing, and verifying
//          feedback signals (currents and voltages), and before new modulator
//          commands are synthesized. This execution sequence is necessary to
//          provide correct latency model.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrm5pModel::Task1( void )
{
    if(Btm2PmdlSel)
    {
        PowerLossModel_2P_T1();
    }
    else
    {
        PowerLossModel_5P_T1();
    }
}
void cBrgThrm5pModel::PowerLossModel_5P_T1(void)
{
    //--------------------------------------------------------------------------
    //   Power Bridge Thermal Model, power device loss calculation.

    // Phase A
    //--------

    BtmIaAbs = ABS( IN(IaFbk) );
    BtmIaSqr = IN(IaFbk) * IN(IaFbk);

    BtmDcondPwr = ( PARM(BtmDiodC1) * BtmIaAbs ) + ( PARM(BtmDiodC2) * BtmIaSqr ) +
                   ( PARM(BtmDiodC3) * BtmIaAbs * BtmIaSqr );
    BtmDbondPwr = ( PARM(BtmDbondR)  * BtmIaSqr );

    if ( !IN(BrgPwrEnb) || (IN(IocPhsA) && (BtmIaAbs > INS(IocThrs))) )
    {
        // Only diode conduction 100% duty being the worst case
        BtmTrCondPwr = 0.0F;
        BtmTrBondPwr = 0.0F;
        BtmTrSwPwr   = 0.0F;
        BtmDswPwr    = 0.0F;

        BtmTrSwchPwrAU = 0.0F;
        BtmTrCondPwrAU = 0.0F;
        BtmTrBondPwrAU = 0.0F;
        BtmTrSwchPwrAL = 0.0F;
        BtmTrCondPwrAL = 0.0F;
        BtmTrBondPwrAL = 0.0F;
        BtmDswchPwrAU  = 0.0F;
        BtmDswchPwrAL  = 0.0F;

        if ( IN(IaFbk) >= 0.0F )
        {
            BtmDcondPwrAU  = 0.0F;
            BtmDbondPwrAU  = 0.0F;

            BtmDcondPwrAL  = BtmDcondPwr;
            BtmDbondPwrAL  = BtmDbondPwr;
        }
        else
        {
            BtmDcondPwrAU  = BtmDcondPwr;
            BtmDbondPwrAU  = BtmDbondPwr;

            BtmDcondPwrAL  = 0.0F;
            BtmDbondPwrAL  = 0.0F;
        }
    }
    else
    {
        BtmTrCondPwr = ( PARM(BtmTrC1) * BtmIaAbs ) + ( PARM(BtmTrC2) * BtmIaSqr ) +
                        ( PARM(BtmTrC3) * BtmIaAbs * BtmIaSqr );
        BtmTrBondPwr = ( PARM(BtmTrBondR) * BtmIaSqr );

        BtmTrSwPwr = BtmTrSwGnT1 * ( PARM(BtmTrSw0) + ( PARM(BtmTrSw1) * BtmIaAbs )
                                  + ( PARM(BtmTrSw2) * BtmIaSqr ));
        BtmDswPwr  = BtmDswGnT1  * ( PARM(BtmDiodSw0) + ( PARM(BtmDiodSw1) * BtmIaAbs )
                                  + ( PARM(BtmDiodSw2) * BtmIaSqr ));

        if ( IN(IaFbk) >= 0.0F )
        {
            BtmTrSwchPwrAU = BtmTrSwPwr;
            BtmTrCondPwrAU = ( BtmUaOld * BtmTrCondPwr );
            BtmTrBondPwrAU = ( BtmUaOld * BtmTrBondPwr );

            BtmTrSwchPwrAL = 0.0F;
            BtmTrCondPwrAL = 0.0F;
            BtmTrBondPwrAL = 0.0F;

            BtmDswchPwrAU  = 0.0F;
            BtmDcondPwrAU  = 0.0F;
            BtmDbondPwrAU  = 0.0F;

            BtmDswchPwrAL  = BtmDswPwr;
            if ( IN(IocPhsA) )
            {                   // Diode conduction 100% duty being the worst case
                BtmDcondPwrAL  = BtmDcondPwr;
                BtmDbondPwrAL  = BtmDbondPwr;
            }
            else
            {                   // Diode conduction per duty-cycle
                BtmDcondPwrAL  = ( (1.0F - BtmUaOld) * BtmDcondPwr );
                BtmDbondPwrAL  = ( (1.0F - BtmUaOld) * BtmDbondPwr );
            }
        }
        else
        {
            BtmTrSwchPwrAU = 0.0F;
            BtmTrCondPwrAU = 0.0F;
            BtmTrBondPwrAU = 0.0F;

            BtmTrSwchPwrAL = BtmTrSwPwr;
            BtmTrCondPwrAL = ( (1.0F - BtmUaOld) * BtmTrCondPwr );
            BtmTrBondPwrAL = ( (1.0F - BtmUaOld) * BtmTrBondPwr );

            BtmDswchPwrAU  = BtmDswPwr;
            if ( IN(IocPhsA) )
            {                   // Diode conduction 100% duty being the worst case
                BtmDcondPwrAU  = BtmDcondPwr;
                BtmDbondPwrAU  = BtmDbondPwr;
            }
            else
            {                   // Diode conduction per duty-cycle
                BtmDcondPwrAU  = ( BtmUaOld * BtmDcondPwr );
                BtmDbondPwrAU  = ( BtmUaOld * BtmDbondPwr );
            }

            BtmDswchPwrAL  = 0.0F;
            BtmDcondPwrAL  = 0.0F;
            BtmDbondPwrAL  = 0.0F;
        }
    }

    // Phase A power loss
    BtmTrPwrAU = BtmTrSwchPwrAU + BtmTrCondPwrAU;
    BtmTrPwrAL = BtmTrSwchPwrAL + BtmTrCondPwrAL;
    BtmDpwrAU  = BtmDswchPwrAU  + BtmDcondPwrAU;
    BtmDpwrAL  = BtmDswchPwrAL  + BtmDcondPwrAL;

    // Phase B
    //--------

    BtmIbAbs = ABS( IN(IbFbk) );
    BtmIbSqr = IN(IbFbk) * IN(IbFbk);

    BtmDcondPwr = ( PARM(BtmDiodC1) * BtmIbAbs ) + ( PARM(BtmDiodC2) * BtmIbSqr ) +
                   ( PARM(BtmDiodC3) * BtmIbAbs * BtmIbSqr );
    BtmDbondPwr = ( PARM(BtmDbondR)  * BtmIbSqr );

    if ( !IN(BrgPwrEnb) || (IN(IocPhsB) && (BtmIbAbs > INS(IocThrs))) )
    {
        // Only diode conduction 100% duty being the worst case
        BtmTrCondPwr = 0.0F;
        BtmTrBondPwr = 0.0F;
        BtmTrSwPwr   = 0.0F;
        BtmDswPwr    = 0.0F;

        BtmTrSwchPwrBU = 0.0F;
        BtmTrCondPwrBU = 0.0F;
        BtmTrBondPwrBU = 0.0F;
        BtmTrSwchPwrBL = 0.0F;
        BtmTrCondPwrBL = 0.0F;
        BtmTrBondPwrBL = 0.0F;
        BtmDswchPwrBU  = 0.0F;
        BtmDswchPwrBL  = 0.0F;

        if ( IN(IbFbk) >= 0.0F )
        {
            BtmDcondPwrBU  = 0.0F;
            BtmDbondPwrBU  = 0.0F;

            BtmDcondPwrBL  = BtmDcondPwr;
            BtmDbondPwrBL  = BtmDbondPwr;
        }
        else
        {
            BtmDcondPwrBU  = BtmDcondPwr;
            BtmDbondPwrBU  = BtmDbondPwr;

            BtmDcondPwrBL  = 0.0F;
            BtmDbondPwrBL  = 0.0F;
        }
    }
    else
    {
        BtmTrCondPwr = ( PARM(BtmTrC1) * BtmIbAbs ) + ( PARM(BtmTrC2) * BtmIbSqr ) +
                        ( PARM(BtmTrC3) * BtmIbAbs * BtmIbSqr );
        BtmTrBondPwr = ( PARM(BtmTrBondR) * BtmIbSqr );

        BtmTrSwPwr = BtmTrSwGnT1 * ( PARM(BtmTrSw0) + ( PARM(BtmTrSw1) * BtmIbAbs )
                                  + ( PARM(BtmTrSw2) * BtmIbSqr ));
        BtmDswPwr  = BtmDswGnT1  * ( PARM(BtmDiodSw0) + ( PARM(BtmDiodSw1) * BtmIbAbs )
                                  + ( PARM(BtmDiodSw2) * BtmIbSqr ));

        if ( IN(IbFbk) >= 0.0F )
        {
            BtmTrSwchPwrBU = BtmTrSwPwr;
            BtmTrCondPwrBU = ( BtmUbOld * BtmTrCondPwr );
            BtmTrBondPwrBU = ( BtmUbOld * BtmTrBondPwr );

            BtmTrSwchPwrBL = 0.0F;
            BtmTrCondPwrBL = 0.0F;
            BtmTrBondPwrBL = 0.0F;

            BtmDswchPwrBU  = 0.0F;
            BtmDcondPwrBU  = 0.0F;
            BtmDbondPwrBU  = 0.0F;

            BtmDswchPwrBL  = BtmDswPwr;
            if ( IN(IocPhsB) )
            {                   // Diode conduction 100% duty being the worst case
                BtmDcondPwrBL  = BtmDcondPwr;
                BtmDbondPwrBL  = BtmDbondPwr;
            }
            else
            {                   // Diode conduction per duty-cycle
                BtmDcondPwrBL  = ( (1.0F - BtmUbOld) * BtmDcondPwr );
                BtmDbondPwrBL  = ( (1.0F - BtmUbOld) * BtmDbondPwr );
            }
        }
        else
        {
            BtmTrSwchPwrBU = 0.0F;
            BtmTrCondPwrBU = 0.0F;
            BtmTrBondPwrBU = 0.0F;

            BtmTrSwchPwrBL = BtmTrSwPwr;
            BtmTrCondPwrBL = ( (1.0F - BtmUbOld) * BtmTrCondPwr );
            BtmTrBondPwrBL = ( (1.0F - BtmUbOld) * BtmTrBondPwr );

            BtmDswchPwrBU  = BtmDswPwr;
            if ( IN(IocPhsB) )
            {                   // Diode conduction 100% duty being the worst case
                BtmDcondPwrBU  = BtmDcondPwr;
                BtmDbondPwrBU  = BtmDbondPwr;
            }
            else
            {                   // Diode conduction per duty-cycle
                BtmDcondPwrBU  = ( BtmUbOld * BtmDcondPwr );
                BtmDbondPwrBU  = ( BtmUbOld * BtmDbondPwr );
            }

            BtmDswchPwrBL  = 0.0F;
            BtmDcondPwrBL  = 0.0F;
            BtmDbondPwrBL  = 0.0F;
        }
    }

    // Phase B power loss
    BtmTrPwrBU = BtmTrSwchPwrBU + BtmTrCondPwrBU;
    BtmTrPwrBL = BtmTrSwchPwrBL + BtmTrCondPwrBL;
    BtmDpwrBU  = BtmDswchPwrBU  + BtmDcondPwrBU;
    BtmDpwrBL  = BtmDswchPwrBL  + BtmDcondPwrBL;

    // Phase C
    //--------

    BtmIcAbs = ABS( IN(IcFbk) );
    BtmIcSqr = IN(IcFbk) * IN(IcFbk);

    BtmDcondPwr  = ( PARM(BtmDiodC1) * BtmIcAbs ) + ( PARM(BtmDiodC2) * BtmIcSqr ) +
                    ( PARM(BtmDiodC3) * BtmIcAbs * BtmIcSqr );
    BtmDbondPwr  = ( PARM(BtmDbondR)  * BtmIcSqr );

    if ( !IN(BrgPwrEnb) || (IN(IocPhsC) && (BtmIcAbs > INS(IocThrs))) )
    {
        // Only diode conduction 100% duty being the worst case
        BtmTrCondPwr = 0.0F;
        BtmTrBondPwr = 0.0F;
        BtmTrSwPwr   = 0.0F;
        BtmDswPwr    = 0.0F;

        BtmTrSwchPwrCU = 0.0F;
        BtmTrCondPwrCU = 0.0F;
        BtmTrBondPwrCU = 0.0F;
        BtmTrSwchPwrCL = 0.0F;
        BtmTrCondPwrCL = 0.0F;
        BtmTrBondPwrCL = 0.0F;
        BtmDswchPwrCU  = 0.0F;
        BtmDswchPwrCL  = 0.0F;

        if ( IN(IcFbk) >= 0.0F )
        {
            BtmDcondPwrCU  = 0.0F;
            BtmDbondPwrCU  = 0.0F;

            BtmDcondPwrCL  = BtmDcondPwr;
            BtmDbondPwrCL  = BtmDbondPwr;
        }
        else
        {
            BtmDcondPwrCU  = BtmDcondPwr;
            BtmDbondPwrCU  = BtmDbondPwr;

            BtmDcondPwrCL  = 0.0F;
            BtmDbondPwrCL  = 0.0F;
        }
    }
    else
    {
        BtmTrCondPwr = ( PARM(BtmTrC1) * BtmIcAbs ) + ( PARM(BtmTrC2) * BtmIcSqr ) +
                        ( PARM(BtmTrC3) * BtmIcAbs * BtmIcSqr );
        BtmTrBondPwr = ( PARM(BtmTrBondR) * BtmIcSqr );

        BtmTrSwPwr = BtmTrSwGnT1 * ( PARM(BtmTrSw0) + ( PARM(BtmTrSw1) * BtmIcAbs )
                                  + ( PARM(BtmTrSw2) * BtmIcSqr ));
        BtmDswPwr  = BtmDswGnT1  * ( PARM(BtmDiodSw0) + ( PARM(BtmDiodSw1) * BtmIcAbs )
                                  + ( PARM(BtmDiodSw2) * BtmIcSqr ));


        if ( IN(IcFbk) >= 0.0F )
        {
            BtmTrSwchPwrCU = BtmTrSwPwr;
            BtmTrCondPwrCU = ( BtmUcOld * BtmTrCondPwr );
            BtmTrBondPwrCU = ( BtmUcOld * BtmTrBondPwr );

            BtmTrSwchPwrCL = 0.0F;
            BtmTrCondPwrCL = 0.0F;
            BtmTrBondPwrCL = 0.0F;

            BtmDswchPwrCU  = 0.0F;
            BtmDcondPwrCU  = 0.0F;
            BtmDbondPwrCU  = 0.0F;

            BtmDswchPwrCL  = BtmDswPwr;
            if ( IN(IocPhsC) )
            {                   // Diode conduction 100% duty being the worst case
                BtmDcondPwrCL  = BtmDcondPwr;
                BtmDbondPwrCL  = BtmDbondPwr;
            }
            else
            {                   // Diode conduction per duty-cycle
                BtmDcondPwrCL  = ( (1.0F - BtmUcOld) * BtmDcondPwr );
                BtmDbondPwrCL  = ( (1.0F - BtmUcOld) * BtmDbondPwr );
            }
        }
        else
        {
            BtmTrSwchPwrCU = 0.0F;
            BtmTrCondPwrCU = 0.0F;
            BtmTrBondPwrCU = 0.0F;

            BtmTrSwchPwrCL = BtmTrSwPwr;
            BtmTrCondPwrCL = ( (1.0F - BtmUcOld) * BtmTrCondPwr );
            BtmTrBondPwrCL = ( (1.0F - BtmUcOld) * BtmTrBondPwr );

            BtmDswchPwrCU  = BtmDswPwr;
            if ( IN(IocPhsC) )
            {                   // Diode conduction 100% duty being the worst case
                BtmDcondPwrCU  = BtmDcondPwr;
                BtmDbondPwrCU  = BtmDbondPwr;
            }
            else
            {                   // Diode conduction per duty-cycle
                BtmDcondPwrCU  = ( BtmUcOld * BtmDcondPwr );
                BtmDbondPwrCU  = ( BtmUcOld * BtmDbondPwr );
            }

            BtmDswchPwrCL  = 0.0F;
            BtmDcondPwrCL  = 0.0F;
            BtmDbondPwrCL  = 0.0F;
        }
    }

    // Phase C power loss
    BtmTrPwrCU = BtmTrSwchPwrCU + BtmTrCondPwrCU;
    BtmTrPwrCL = BtmTrSwchPwrCL + BtmTrCondPwrCL;
    BtmDpwrCU  = BtmDswchPwrCU  + BtmDcondPwrCU;
    BtmDpwrCL  = BtmDswchPwrCL  + BtmDcondPwrCL;

    // Delay & store up/down commands
    BtmUaOld = IN(ModUaCmdZ1);
    BtmUbOld = IN(ModUbCmdZ1);
    BtmUcOld = IN(ModUcCmdZ1);

    // Total power loss per type of event and device
    BtmDcondPwrLoss  = BtmDcondPwrAU  + BtmDcondPwrAL  + BtmDcondPwrBU  + BtmDcondPwrBL  + BtmDcondPwrCU  + BtmDcondPwrCL;
    BtmDswchPwrLoss  = BtmDswchPwrAU  + BtmDswchPwrAL  + BtmDswchPwrBU  + BtmDswchPwrBL  + BtmDswchPwrCU  + BtmDswchPwrCL;
    BtmTrCondPwrLoss = BtmTrCondPwrAU + BtmTrCondPwrAL + BtmTrCondPwrBU + BtmTrCondPwrBL + BtmTrCondPwrCU + BtmTrCondPwrCL;
    BtmTrSwchPwrLoss = BtmTrSwchPwrAU + BtmTrSwchPwrAL + BtmTrSwchPwrBU + BtmTrSwchPwrBL + BtmTrSwchPwrCU + BtmTrSwchPwrCL;

    // Total power loss per type of device
    OUT(BtmDpwrLoss)  = BtmDpwrAU  + BtmDpwrBU  + BtmDpwrCU  + BtmDpwrAL  + BtmDpwrBL  + BtmDpwrCL;
    OUT(BtmTrPwrLoss) = BtmTrPwrAU + BtmTrPwrBU + BtmTrPwrCU + BtmTrPwrAL + BtmTrPwrBL + BtmTrPwrCL;
    BtmBondPwrLoss    = BtmTrBondPwrAU + BtmTrBondPwrAL + BtmDbondPwrAU + BtmDbondPwrAL;
    BtmBondPwrLoss   += BtmTrBondPwrBU + BtmTrBondPwrBL + BtmDbondPwrBU + BtmDbondPwrBL;
    BtmBondPwrLoss   += BtmTrBondPwrCU + BtmTrBondPwrCL + BtmDbondPwrCU + BtmDbondPwrCL;

    return;
}
void cBrgThrm5pModel::PowerLossModel_2P_T1(void)
{
    //--------------------------------------------------------------------------
    //   Power Bridge Thermal Model, power device loss calculation.

    // Phase A
    //--------
    BtmIaAbs = ABS( IN(IaFbk) );
    BtmIaSqr = IN(IaFbk) * IN(IaFbk);

    BtmTrCondPwr = ( PARM(BtmTrC1) * BtmIaAbs ) + ( PARM(BtmTrC2) * BtmIaSqr ) +
                    ( PARM(BtmTrC3) * BtmIaAbs * BtmIaSqr );
    BtmDcondPwr = ( PARM(BtmDiodC1) * BtmIaAbs ) + ( PARM(BtmDiodC2) * BtmIaSqr ) +
                   ( PARM(BtmDiodC3) * BtmIaAbs * BtmIaSqr );

    if ( ( BtmUaOld < BtmUOldThrLo ) || ( BtmUaOld > BtmUOldThrHi ) || (!IN(BrgPwrEnb)) )
    {
        BtmTrSwPwr   = 0.0F;
        BtmDswPwr    = 0.0F;
    }
    else
    {
        BtmTrSwPwr = BtmTrSwGnT1 * ( PARM(BtmTrSw0) + ( PARM(BtmTrSw1) * BtmIaAbs )
                                  + ( PARM(BtmTrSw2) * BtmIaSqr ));
        BtmDswPwr  = BtmDswGnT1  * ( PARM(BtmDiodSw0) + ( PARM(BtmDiodSw1) * BtmIaAbs )
                                  + ( PARM(BtmDiodSw2) * BtmIaSqr ));
    }

    if ( IN(IaFbk) >= 0.0F )
    {
        BtmTrSwchPwrAU = BtmTrSwPwr;
        BtmTrCondPwrAU = ( BtmUaOld * BtmTrCondPwr );

        BtmTrSwchPwrAL = 0.0F;
        BtmTrCondPwrAL = 0.0F;

        BtmDswchPwrAU  = 0.0F;
        BtmDcondPwrAU  = 0.0F;

        BtmDswchPwrAL  = BtmDswPwr;
        BtmDcondPwrAL  = ( (1.0F - BtmUaOld) * BtmDcondPwr );
    }
    else
    {
        BtmTrSwchPwrAU = 0.0F;
        BtmTrCondPwrAU = 0.0F;

        BtmTrSwchPwrAL = BtmTrSwPwr;
        BtmTrCondPwrAL = (( 1.0F - BtmUaOld ) * BtmTrCondPwr );

        BtmDswchPwrAU  = BtmDswPwr;
        BtmDcondPwrAU  = ( BtmUaOld * BtmDcondPwr );

        BtmDswchPwrAL  = 0.0F;
        BtmDcondPwrAL  = 0.0F;
    }

    // Phase A power loss
    BtmTrPwrAU = BtmTrSwchPwrAU + BtmTrCondPwrAU;
    BtmTrPwrAL = BtmTrSwchPwrAL + BtmTrCondPwrAL;
    BtmDpwrAU  = BtmDswchPwrAU  + BtmDcondPwrAU;
    BtmDpwrAL  = BtmDswchPwrAL  + BtmDcondPwrAL;

    // clear bond power in 2P mode
    BtmTrBondPwrAU  = 0.0F;
    BtmTrBondPwrAL  = 0.0F;
    BtmDbondPwrAU   = 0.0F;
    BtmDbondPwrAL   = 0.0F;

    // Phase B
    //--------

    BtmIbAbs = ABS( IN(IbFbk) );
    BtmIbSqr = IN(IbFbk) * IN(IbFbk);

    BtmTrCondPwr = ( PARM(BtmTrC1) * BtmIbAbs ) + ( PARM(BtmTrC2) * BtmIbSqr ) +
                    ( PARM(BtmTrC3) * BtmIbAbs * BtmIbSqr );
    BtmDcondPwr = ( PARM(BtmDiodC1) * BtmIbAbs ) + ( PARM(BtmDiodC2) * BtmIbSqr ) +
                   ( PARM(BtmDiodC3) * BtmIbAbs * BtmIbSqr );

    if ( ( BtmUbOld < BtmUOldThrLo ) || ( BtmUbOld > BtmUOldThrHi ) || (!IN(BrgPwrEnb)) )
    {
        BtmTrSwPwr   = 0.0F;
        BtmDswPwr    = 0.0F;
    }
    else
    {

        BtmTrSwPwr = BtmTrSwGnT1 * ( PARM(BtmTrSw0) + ( PARM(BtmTrSw1) * BtmIbAbs )
                                  + ( PARM(BtmTrSw2) * BtmIbSqr ));
        BtmDswPwr  = BtmDswGnT1  * ( PARM(BtmDiodSw0) + ( PARM(BtmDiodSw1) * BtmIbAbs )
                                  + ( PARM(BtmDiodSw2) * BtmIbSqr ));
    }

        if ( IN(IbFbk) >= 0.0F )
        {
            BtmTrSwchPwrBU = BtmTrSwPwr;
            BtmTrCondPwrBU = ( BtmUbOld * BtmTrCondPwr );

            BtmTrSwchPwrBL = 0.0F;
            BtmTrCondPwrBL = 0.0F;

            BtmDswchPwrBU  = 0.0F;
            BtmDcondPwrBU  = 0.0F;

            BtmDswchPwrBL  = BtmDswPwr;
                BtmDcondPwrBL  = ( (1.0F - BtmUbOld) * BtmDcondPwr );
        }
        else
        {
            BtmTrSwchPwrBU = 0.0F;
            BtmTrCondPwrBU = 0.0F;


            BtmTrSwchPwrBL = BtmTrSwPwr;
            BtmTrCondPwrBL = ( (1.0F - BtmUbOld) * BtmTrCondPwr );

            BtmDswchPwrBU  = BtmDswPwr;
                BtmDcondPwrBU  = ( BtmUbOld * BtmDcondPwr );

            BtmDswchPwrBL  = 0.0F;
            BtmDcondPwrBL  = 0.0F;
        }


    // Phase B power loss
    BtmTrPwrBU = BtmTrSwchPwrBU + BtmTrCondPwrBU;
    BtmTrPwrBL = BtmTrSwchPwrBL + BtmTrCondPwrBL;
    BtmDpwrBU  = BtmDswchPwrBU  + BtmDcondPwrBU;
    BtmDpwrBL  = BtmDswchPwrBL  + BtmDcondPwrBL;

    // clear bond power in 2P mode
    BtmTrBondPwrBU  = 0.0F;
    BtmTrBondPwrBL  = 0.0F;
    BtmDbondPwrBU   = 0.0F;
    BtmDbondPwrBL   = 0.0F;

    // Phase C
    //--------

    BtmIcAbs = ABS( IN(IcFbk) );
    BtmIcSqr = IN(IcFbk) * IN(IcFbk);

    BtmTrCondPwr = ( PARM(BtmTrC1) * BtmIcAbs ) + ( PARM(BtmTrC2) * BtmIcSqr ) +
                    ( PARM(BtmTrC3) * BtmIcAbs * BtmIcSqr );
    BtmDcondPwr  = ( PARM(BtmDiodC1) * BtmIcAbs ) + ( PARM(BtmDiodC2) * BtmIcSqr ) +
                    ( PARM(BtmDiodC3) * BtmIcAbs * BtmIcSqr );

    if ( ( BtmUcOld < BtmUOldThrLo ) || ( BtmUcOld > BtmUOldThrHi ) || (!IN(BrgPwrEnb)) )
    {
        BtmTrSwPwr   = 0.0F;
        BtmDswPwr    = 0.0F;
    }
    else
    {
        BtmTrSwPwr = BtmTrSwGnT1 * ( PARM(BtmTrSw0) + ( PARM(BtmTrSw1) * BtmIcAbs )
                                  + ( PARM(BtmTrSw2) * BtmIcSqr ));
        BtmDswPwr  = BtmDswGnT1  * ( PARM(BtmDiodSw0) + ( PARM(BtmDiodSw1) * BtmIcAbs )
                                  + ( PARM(BtmDiodSw2) * BtmIcSqr ));
    }


        if ( IN(IcFbk) >= 0.0F )
        {
            BtmTrSwchPwrCU = BtmTrSwPwr;
            BtmTrCondPwrCU = ( BtmUcOld * BtmTrCondPwr );

            BtmTrSwchPwrCL = 0.0F;
            BtmTrCondPwrCL = 0.0F;

            BtmDswchPwrCU  = 0.0F;
            BtmDcondPwrCU  = 0.0F;

            BtmDswchPwrCL  = BtmDswPwr;
                BtmDcondPwrCL  = ( (1.0F - BtmUcOld) * BtmDcondPwr );
        }
        else
        {
            BtmTrSwchPwrCU = 0.0F;
            BtmTrCondPwrCU = 0.0F;

            BtmTrSwchPwrCL = BtmTrSwPwr;
            BtmTrCondPwrCL = ( (1.0F - BtmUcOld) * BtmTrCondPwr );

            BtmDswchPwrCU  = BtmDswPwr;
                BtmDcondPwrCU  = ( BtmUcOld * BtmDcondPwr );

            BtmDswchPwrCL  = 0.0F;
            BtmDcondPwrCL  = 0.0F;
    }

    // Phase C power loss
    BtmTrPwrCU = BtmTrSwchPwrCU + BtmTrCondPwrCU;
    BtmTrPwrCL = BtmTrSwchPwrCL + BtmTrCondPwrCL;
    BtmDpwrCU  = BtmDswchPwrCU  + BtmDcondPwrCU;
    BtmDpwrCL  = BtmDswchPwrCL  + BtmDcondPwrCL;

    // clear bond power in 2P mode
    BtmTrBondPwrCU  = 0.0F;
    BtmTrBondPwrCL  = 0.0F;
    BtmDbondPwrCU   = 0.0F;
    BtmDbondPwrCL   = 0.0F;

    // Delay & store up/down commands
    BtmUaOld = IN(ModUaCmdZ1);
    BtmUbOld = IN(ModUbCmdZ1);
    BtmUcOld = IN(ModUcCmdZ1);

    // Total power loss per type of event and device
    BtmDcondPwrLoss  = BtmDcondPwrAU  + BtmDcondPwrAL  + BtmDcondPwrBU  + BtmDcondPwrBL  + BtmDcondPwrCU  + BtmDcondPwrCL;
    BtmDswchPwrLoss  = BtmDswchPwrAU  + BtmDswchPwrAL  + BtmDswchPwrBU  + BtmDswchPwrBL  + BtmDswchPwrCU  + BtmDswchPwrCL;
    BtmTrCondPwrLoss = BtmTrCondPwrAU + BtmTrCondPwrAL + BtmTrCondPwrBU + BtmTrCondPwrBL + BtmTrCondPwrCU + BtmTrCondPwrCL;
    BtmTrSwchPwrLoss = BtmTrSwchPwrAU + BtmTrSwchPwrAL + BtmTrSwchPwrBU + BtmTrSwchPwrBL + BtmTrSwchPwrCU + BtmTrSwchPwrCL;

    // Total power loss per type of device
    OUT(BtmDpwrLoss)  = BtmDpwrAU  + BtmDpwrBU  + BtmDpwrCU  + BtmDpwrAL  + BtmDpwrBL  + BtmDpwrCL;
    OUT(BtmTrPwrLoss) = BtmTrPwrAU + BtmTrPwrBU + BtmTrPwrCU + BtmTrPwrAL + BtmTrPwrBL + BtmTrPwrCL;
    BtmBondPwrLoss    = BtmTrBondPwrAU + BtmTrBondPwrAL + BtmDbondPwrAU + BtmDbondPwrAL;
    BtmBondPwrLoss   += BtmTrBondPwrBU + BtmTrBondPwrBL + BtmDbondPwrBU + BtmDbondPwrBL;
    BtmBondPwrLoss   += BtmTrBondPwrCU + BtmTrBondPwrCL + BtmDbondPwrCU + BtmDbondPwrCL;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      5P model calculate junction temperature in Task 2, while 2P model in
//      Task 3.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrm5pModel::Task2( void )
{
    if(!Btm2PmdlSel)
    {
        ThermalModel_5P_T2();
        MaxJncTempChk_5P_T2();
        Diagnostic_5P_T2();
    }
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      5P model calculate Node 1 temperature in Task 3, while 2P model calculate
//      all nodes here.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrm5pModel::Task3( void )
{
    // Calculate variables used in Task 1 phase loss method
    float tempBtmSwGn1,tempVdcSq;
    tempBtmSwGn1=INS(PwmPulseNumb) * INS(DelTm1Rcp);
    tempVdcSq=IN(VdcFbkT3)*IN(VdcFbkT3);
    BtmTrSwGn = tempBtmSwGn1 * (PARM(BtmTrVdcSw2)*tempVdcSq + PARM(BtmTrVdcSw1)*IN(VdcFbkT3) + PARM(BtmTrVdcSw0));
    BtmDswGn  = tempBtmSwGn1 * (PARM(BtmDiVdcSw2)*tempVdcSq + PARM(BtmDiVdcSw1)*IN(VdcFbkT3) + PARM(BtmDiVdcSw0));

    if(!Btm2PmdlSel)
    {
        ThermalModel_5P_T3();
    }
    else
    {
        ThermalModel_2P_T3();
        Diagnostic_2P_T3();
        MaxJncTempChk_2P_T3();
    }

    BtmRefTempValid = IN(RefTempValid);   // Copy input to data member for data-passing

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculation of junction temperature as the fifth transistor and
//      diode node for a five-pole in tandem RC network.
//      Also, calculation of temperature in some fictitious thermal nodes
//      connected to transistor junction by a thermal resistor. These are the
//      transistor and diode fourth, third, and second  nodes for a five-pole
//      in tandem RC network.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrm5pModel::ThermalModel_5P_T2(void)
{
    float   PowerTrEquiv;
    float   PowerDequiv;

    // -------------------------------------------------------------------------------
    // Five-pole thermal model - 5th transistor and diode node (junction) calculations
    //                         - 4th transistor and diode node calculations

    // If reference temperatures is not valid, initialize integrator states
    // and power flowing between nodes
    if ( BtmRefTempValidT2 == false )
    {
    // Phase A
        BtmTrJncTempAU  = IN(CoolantTemp);
        BtmTrPwrJnNd4AU = 0.0F;
        BtmTrJncTempAL  = IN(CoolantTemp);
        BtmTrPwrJnNd4AL = 0.0F;
        BtmDjncTempAU   = IN(CoolantTemp);
        BtmDpwrJnNd4AU  = 0.0F;
        BtmDjncTempAL   = IN(CoolantTemp);
        BtmDpwrJnNd4AL  = 0.0F;
        BtmTrNd4TempAU   = IN(CoolantTemp);
        BtmTrPwrNd3Nd2AU = 0.0F;
        BtmTrNd4TempAL   = IN(CoolantTemp);
        BtmTrPwrNd3Nd2AL = 0.0F;
        BtmDnd4TempAU    = IN(CoolantTemp);
        BtmDpwrNd3Nd2AU  = 0.0F;
        BtmDnd4TempAL    = IN(CoolantTemp);
        BtmDpwrNd3Nd2AL  = 0.0F;
    // Phase B
        BtmTrJncTempBU  = IN(CoolantTemp);
        BtmTrPwrJnNd4BU = 0.0F;
        BtmTrJncTempBL  = IN(CoolantTemp);
        BtmTrPwrJnNd4BL = 0.0F;
        BtmDjncTempBU   = IN(CoolantTemp);
        BtmDpwrJnNd4BU  = 0.0F;
        BtmDjncTempBL   = IN(CoolantTemp);
        BtmDpwrJnNd4BL  = 0.0F;
        BtmTrNd4TempBU   = IN(CoolantTemp);
        BtmTrPwrNd3Nd2BU = 0.0F;
        BtmTrNd4TempBL   = IN(CoolantTemp);
        BtmTrPwrNd3Nd2BL = 0.0F;
        BtmDnd4TempBU    = IN(CoolantTemp);
        BtmDpwrNd3Nd2BU  = 0.0F;
        BtmDnd4TempBL    = IN(CoolantTemp);
        BtmDpwrNd3Nd2BL  = 0.0F;
    // Phase C
        BtmTrJncTempCU  = IN(CoolantTemp);
        BtmTrPwrJnNd4CU = 0.0F;
        BtmTrJncTempCL  = IN(CoolantTemp);
        BtmTrPwrJnNd4CL = 0.0F;
        BtmDjncTempCU   = IN(CoolantTemp);
        BtmDpwrJnNd4CU  = 0.0F;
        BtmDjncTempCL   = IN(CoolantTemp);
        BtmDpwrJnNd4CL  = 0.0F;
        BtmTrNd4TempCU   = IN(CoolantTemp);
        BtmTrPwrNd3Nd2CU = 0.0F;
        BtmTrNd4TempCL   = IN(CoolantTemp);
        BtmTrPwrNd3Nd2CL = 0.0F;
        BtmDnd4TempCU    = IN(CoolantTemp);
        BtmDpwrNd3Nd2CU  = 0.0F;
        BtmDnd4TempCL    = IN(CoolantTemp);
        BtmDpwrNd3Nd2CL  = 0.0F;
    }
    else
    {
    // Phase A
        // Transistor junction temperatures
        RC_NETWORK(RcNetworkPlistJncTAU);
        RC_NETWORK(RcNetworkPlistJncTAL);
        // Diode junction temperatures
        RC_NETWORK(RcNetworkPlistJncDAU);
        RC_NETWORK(RcNetworkPlistJncDAL);

        // Transistor near-baseplate temperatures
        RC_NETWORK(RcNetworkPlistNd4TAU);
        RC_NETWORK(RcNetworkPlistNd4TAL);
        // Diode near-baseplate temperatures
        RC_NETWORK(RcNetworkPlistNd4DAU);
        RC_NETWORK(RcNetworkPlistNd4DAL);

    // Phase B
        // Transistor junction temperatures
        RC_NETWORK(RcNetworkPlistJncTBU);
        RC_NETWORK(RcNetworkPlistJncTBL);
        // diode junction temperatures
        RC_NETWORK(RcNetworkPlistJncDBU);
        RC_NETWORK(RcNetworkPlistJncDBL);

        // Transistor near-baseplate temperatures
        RC_NETWORK(RcNetworkPlistNd4TBU);
        RC_NETWORK(RcNetworkPlistNd4TBL);
        // Diode near-baseplate temperatures
        RC_NETWORK(RcNetworkPlistNd4DBU);
        RC_NETWORK(RcNetworkPlistNd4DBL);

    // Phase C
        // Transistor junction temperatures
        RC_NETWORK(RcNetworkPlistJncTCU);
        RC_NETWORK(RcNetworkPlistJncTCL);
        // diode junction temperatures
        RC_NETWORK(RcNetworkPlistJncDCU);
        RC_NETWORK(RcNetworkPlistJncDCL);

        // Transistor near-baseplate temperatures
        RC_NETWORK(RcNetworkPlistNd4TCU);
        RC_NETWORK(RcNetworkPlistNd4TCL);
        // Diode near-baseplate temperatures
        RC_NETWORK(RcNetworkPlistNd4DCU);
        RC_NETWORK(RcNetworkPlistNd4DCL);
    }


    // --------------------------------------------------------------------
    // Five-pole thermal model - 3nd transistor and diode node calculations
    //                         - 2th transistor and diode node calculations

    // Transistor/diode nodes 2 and 3 temperatures, somewhere in the thermal grease

    // Phase A upper
    PowerTrEquiv   = (BtmTrNd4TempAU+PARM(BtmTrRthNd4Nd3)*BtmTrBondPwrAUT2) * BtmTrRthNd4Nd2Rcp;
    PowerDequiv    = (BtmDnd4TempAU+PARM(BtmDrthNd4Nd3)*BtmDbondPwrAUT2) * BtmDrthNd4Nd2Rcp;
    BtmNd2TempAU   = ((PowerTrEquiv+PowerDequiv)*BtmRthNd4Nd2Prll-BtmNd1TempAUT2) * BtmNd2Divider + BtmNd1TempAUT2;
    BtmTrPwrNd3Nd2AU= PowerTrEquiv - BtmNd2TempAU*BtmTrRthNd4Nd2Rcp;
    BtmDpwrNd3Nd2AU= PowerDequiv - BtmNd2TempAU*BtmDrthNd4Nd2Rcp;
    BtmPwrNd2Nd1AU = (BtmNd2TempAU - BtmNd1TempAUT2) * BtmRthNd2Nd1Rcp;
    BtmTrNd3TempAU = BtmNd2TempAU + BtmTrPwrNd3Nd2AU*PARM(BtmTrRthNd3Nd2);
    BtmDnd3TempAU  = BtmNd2TempAU + BtmDpwrNd3Nd2AU*PARM(BtmDrthNd3Nd2);

    // Phase A lower
    PowerTrEquiv   = (BtmTrNd4TempAL+PARM(BtmTrRthNd4Nd3)*BtmTrBondPwrALT2) * BtmTrRthNd4Nd2Rcp;
    PowerDequiv    = (BtmDnd4TempAL+PARM(BtmDrthNd4Nd3)*BtmDbondPwrALT2) * BtmDrthNd4Nd2Rcp;
    BtmNd2TempAL   = ((PowerTrEquiv+PowerDequiv)*BtmRthNd4Nd2Prll-BtmNd1TempALT2) * BtmNd2Divider + BtmNd1TempALT2;
    BtmTrPwrNd3Nd2AL = PowerTrEquiv - BtmNd2TempAL*BtmTrRthNd4Nd2Rcp;
    BtmDpwrNd3Nd2AL= PowerDequiv - BtmNd2TempAL*BtmDrthNd4Nd2Rcp;
    BtmPwrNd2Nd1AL = (BtmNd2TempAL - BtmNd1TempALT2) * BtmRthNd2Nd1Rcp;
    BtmTrNd3TempAL = BtmNd2TempAL + BtmTrPwrNd3Nd2AL*PARM(BtmTrRthNd3Nd2);
    BtmDnd3TempAL  = BtmNd2TempAL + BtmDpwrNd3Nd2AL*PARM(BtmDrthNd3Nd2);

    // Phase B upper
    PowerTrEquiv   = (BtmTrNd4TempBU+PARM(BtmTrRthNd4Nd3)*BtmTrBondPwrBUT2) * BtmTrRthNd4Nd2Rcp;
    PowerDequiv    = (BtmDnd4TempBU+PARM(BtmDrthNd4Nd3)*BtmDbondPwrBUT2) * BtmDrthNd4Nd2Rcp;
    BtmNd2TempBU   = ((PowerTrEquiv+PowerDequiv)*BtmRthNd4Nd2Prll-BtmNd1TempBUT2) * BtmNd2Divider + BtmNd1TempBUT2;
    BtmTrPwrNd3Nd2BU = PowerTrEquiv - BtmNd2TempBU*BtmTrRthNd4Nd2Rcp;
    BtmDpwrNd3Nd2BU= PowerDequiv - BtmNd2TempBU*BtmDrthNd4Nd2Rcp;
    BtmPwrNd2Nd1BU = (BtmNd2TempBU - BtmNd1TempBUT2) * BtmRthNd2Nd1Rcp;
    BtmTrNd3TempBU = BtmNd2TempBU + BtmTrPwrNd3Nd2BU * PARM(BtmTrRthNd3Nd2);
    BtmDnd3TempBU  = BtmNd2TempBU + BtmDpwrNd3Nd2BU*PARM(BtmDrthNd3Nd2);

    // Phase B lower
    PowerTrEquiv   = (BtmTrNd4TempBL+PARM(BtmTrRthNd4Nd3)*BtmTrBondPwrBLT2) * BtmTrRthNd4Nd2Rcp;
    PowerDequiv    = (BtmDnd4TempBL+PARM(BtmDrthNd4Nd3)*BtmDbondPwrBLT2) * BtmDrthNd4Nd2Rcp;
    BtmNd2TempBL   = ((PowerTrEquiv+PowerDequiv)*BtmRthNd4Nd2Prll-BtmNd1TempBLT2) * BtmNd2Divider + BtmNd1TempBLT2;
    BtmTrPwrNd3Nd2BL= PowerTrEquiv - BtmNd2TempBL*BtmTrRthNd4Nd2Rcp;
    BtmDpwrNd3Nd2BL= PowerDequiv - BtmNd2TempBL*BtmDrthNd4Nd2Rcp;
    BtmPwrNd2Nd1BL = (BtmNd2TempBL - BtmNd1TempBLT2) * BtmRthNd2Nd1Rcp;
    BtmTrNd3TempBL = BtmNd2TempBL + BtmTrPwrNd3Nd2BL*PARM(BtmTrRthNd3Nd2);
    BtmDnd3TempBL  = BtmNd2TempBL + BtmDpwrNd3Nd2BL*PARM(BtmDrthNd3Nd2);

    // Phase C upper
    PowerTrEquiv   = (BtmTrNd4TempCU+PARM(BtmTrRthNd4Nd3)*BtmTrBondPwrCUT2) * BtmTrRthNd4Nd2Rcp;
    PowerDequiv    = (BtmDnd4TempCU+PARM(BtmDrthNd4Nd3)*BtmDbondPwrCUT2) * BtmDrthNd4Nd2Rcp;
    BtmNd2TempCU   = ((PowerTrEquiv+PowerDequiv)*BtmRthNd4Nd2Prll-BtmNd1TempCUT2) * BtmNd2Divider + BtmNd1TempCUT2;
    BtmTrPwrNd3Nd2CU= PowerTrEquiv - BtmNd2TempCU*BtmTrRthNd4Nd2Rcp;
    BtmDpwrNd3Nd2CU= PowerDequiv - BtmNd2TempCU*BtmDrthNd4Nd2Rcp;
    BtmPwrNd2Nd1CU = (BtmNd2TempCU - BtmNd1TempCUT2) * BtmRthNd2Nd1Rcp;
    BtmTrNd3TempCU = BtmNd2TempCU + BtmTrPwrNd3Nd2CU*PARM(BtmTrRthNd3Nd2);
    BtmDnd3TempCU  = BtmNd2TempCU + BtmDpwrNd3Nd2CU*PARM(BtmDrthNd3Nd2);

    // Phase C lower
    PowerTrEquiv   = (BtmTrNd4TempCL+PARM(BtmTrRthNd4Nd3)*BtmTrBondPwrCLT2) * BtmTrRthNd4Nd2Rcp;
    PowerDequiv    = (BtmDnd4TempCL+PARM(BtmDrthNd4Nd3)*BtmDbondPwrCLT2) * BtmDrthNd4Nd2Rcp;
    BtmNd2TempCL   = ((PowerTrEquiv+PowerDequiv)*BtmRthNd4Nd2Prll-BtmNd1TempCLT2) * BtmNd2Divider + BtmNd1TempCLT2;
    BtmTrPwrNd3Nd2CL= PowerTrEquiv - BtmNd2TempCL*BtmTrRthNd4Nd2Rcp;
    BtmDpwrNd3Nd2CL= PowerDequiv - BtmNd2TempCL*BtmDrthNd4Nd2Rcp;
    BtmPwrNd2Nd1CL = (BtmNd2TempCL - BtmNd1TempCLT2) * BtmRthNd2Nd1Rcp;
    BtmTrNd3TempCL = BtmNd2TempCL + BtmTrPwrNd3Nd2CL*PARM(BtmTrRthNd3Nd2);
    BtmDnd3TempCL  = BtmNd2TempCL + BtmDpwrNd3Nd2CL*PARM(BtmDrthNd3Nd2);

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculate and filter total bridge loss per phase. Choose bridge
//      reference temperature. Apply over-temperature limits and push
//      corresponding alarms. Also, calculation of temperature in a fictitious
//      thermal node that is closest to heatsink, and represents the first
//      node (common to transistor and diode) for a five-pole in tandem RC
//      network.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrm5pModel::ThermalModel_5P_T3(void)
{
    // --------------------------------------------------------------------
    // Five-pole thermal model - 1st transistor/diode node calculations

    // If reference temperatures is not valid, initialize integrator states
    // and power flowing between nodes
    if ( IN(RefTempValid) == false )
    {
        // phase A
        BtmNd1TempAU   = IN(CoolantTemp);
        BtmPwrNd1ClnAU =  0.0F;
        BtmNd1TempAL   = IN(CoolantTemp);
        BtmPwrNd1ClnAL = 0.0F;
        // phase B
        BtmNd1TempBU   = IN(CoolantTemp);
        BtmPwrNd1ClnBU = 0.0F;
        BtmNd1TempBL   = IN(CoolantTemp);
        BtmPwrNd1ClnBL = 0.0F;
        // phase C
        BtmNd1TempCU   = IN(CoolantTemp);
        BtmPwrNd1ClnCU = 0.0F;
        BtmNd1TempCL   = IN(CoolantTemp);
        BtmPwrNd1ClnCL =  0.0F;
    }
    else
    {
    // Transistor/diode common node temperatures, near heatsink
        // Phase A
        RC_NETWORK(RcNetworkPlistNd1AU);
        RC_NETWORK(RcNetworkPlistNd1AL);
        // Phase B
        RC_NETWORK(RcNetworkPlistNd1BU);
        RC_NETWORK(RcNetworkPlistNd1BL);
        // Phase C
        RC_NETWORK(RcNetworkPlistNd1CU);
        RC_NETWORK(RcNetworkPlistNd1CL);
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculation of junction temperature as the fifth transistor and
//      diode node for a five-pole in tandem RC network.
//      Also, calculation of temperature in some fictitious thermal nodes
//      connected to transistor junction by a thermal resistor. These are the
//      transistor and diode fourth, third, and second  nodes for a five-pole
//      in tandem RC network.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrm5pModel::ThermalModel_2P_T3(void)
{
    if (!BtmRefTempValid)
    {
        BtmTempAAU  = IN(BpTempA);
        BtmTempAAL  = IN(BpTempA);
        BtmTempABU  = IN(BpTempB);
        BtmTempABL  = IN(BpTempB);
        BtmTempACU  = IN(BpTempC);
        BtmTempACL  = IN(BpTempC);

        BtmDTempAAU = IN(BpTempA);
        BtmDTempAAL = IN(BpTempA);
        BtmDTempABU = IN(BpTempB);
        BtmDTempABL = IN(BpTempB);
        BtmDTempACU = IN(BpTempC);
        BtmDTempACL = IN(BpTempC);
    }

    //--------------------------------------------------------------------------
    //   Calculate transistor junction temperature

    // Calculate baseplate power

    BtmABPwrAU  = BtmRbRcp * ( BtmTempAAU - IN(BpTempA) );
    BtmABPwrAL  = BtmRbRcp * ( BtmTempAAL - IN(BpTempA) );
    BtmABPwrBU  = BtmRbRcp * ( BtmTempABU - IN(BpTempB) );
    BtmABPwrBL  = BtmRbRcp * ( BtmTempABL - IN(BpTempB) );
    BtmABPwrCU  = BtmRbRcp * ( BtmTempACU - IN(BpTempC) );
    BtmABPwrCL  = BtmRbRcp * ( BtmTempACL - IN(BpTempC) );

    BtmABPwrA   = BtmABPwrAU + BtmABPwrAL;
    BtmABPwrB   = BtmABPwrBU + BtmABPwrBL;
    BtmABPwrC   = BtmABPwrCU + BtmABPwrCL;

    // Calculate die temperatures

    BtmTempAAU += BtmTaGn * ( BtmTrPwrAUT3 - BtmABPwrAU );
    BtmTempAAL += BtmTaGn * ( BtmTrPwrALT3 - BtmABPwrAL );
    BtmTempABU += BtmTaGn * ( BtmTrPwrBUT3 - BtmABPwrBU );
    BtmTempABL += BtmTaGn * ( BtmTrPwrBLT3 - BtmABPwrBL );
    BtmTempACU += BtmTaGn * ( BtmTrPwrCUT3 - BtmABPwrCU );
    BtmTempACL += BtmTaGn * ( BtmTrPwrCLT3 - BtmABPwrCL );

    // Calculate junction temperatures

    BtmTrJncTempAU = BtmTempAAU + ( PARM(BtmRaTh) * BtmTrPwrAUT3 );
    BtmTrJncTempAL = BtmTempAAL + ( PARM(BtmRaTh) * BtmTrPwrALT3 );
    BtmTrJncTempBU = BtmTempABU + ( PARM(BtmRaTh) * BtmTrPwrBUT3 );
    BtmTrJncTempBL = BtmTempABL + ( PARM(BtmRaTh) * BtmTrPwrBLT3 );
    BtmTrJncTempCU = BtmTempACU + ( PARM(BtmRaTh) * BtmTrPwrCUT3 );
    BtmTrJncTempCL = BtmTempACL + ( PARM(BtmRaTh) * BtmTrPwrCLT3 );


    //--------------------------------------------------------------------------
    //   Calculate diode junction temperature

    // Calculate diode baseplate power

    BtmDABPwrAU  = BtmDRbRcp * ( BtmDTempAAU - IN(BpTempA) );
    BtmDABPwrAL  = BtmDRbRcp * ( BtmDTempAAL - IN(BpTempA) );
    BtmDABPwrBU  = BtmDRbRcp * ( BtmDTempABU - IN(BpTempB) );
    BtmDABPwrBL  = BtmDRbRcp * ( BtmDTempABL - IN(BpTempB) );
    BtmDABPwrCU  = BtmDRbRcp * ( BtmDTempACU - IN(BpTempC) );
    BtmDABPwrCL  = BtmDRbRcp * ( BtmDTempACL - IN(BpTempC) );

    BtmDABPwrA   = BtmDABPwrAU + BtmDABPwrAL;
    BtmDABPwrB   = BtmDABPwrBU + BtmDABPwrBL;
    BtmDABPwrC   = BtmDABPwrCU + BtmDABPwrCL;

    // Calculate diode die temperatures

    BtmDTempAAU += BtmDTaGn * ( BtmDpwrAUT3 - BtmDABPwrAU );
    BtmDTempAAL += BtmDTaGn * ( BtmDpwrALT3 - BtmDABPwrAL );
    BtmDTempABU += BtmDTaGn * ( BtmDpwrBUT3 - BtmDABPwrBU );
    BtmDTempABL += BtmDTaGn * ( BtmDpwrBLT3 - BtmDABPwrBL );
    BtmDTempACU += BtmDTaGn * ( BtmDpwrCUT3 - BtmDABPwrCU );
    BtmDTempACL += BtmDTaGn * ( BtmDpwrCLT3 - BtmDABPwrCL );

    // Calculate diode junction temperatures

    BtmDjncTempAU = BtmDTempAAU + ( PARM(BtmDRaTh) * BtmDpwrAUT3 );
    BtmDjncTempAL = BtmDTempAAL + ( PARM(BtmDRaTh) * BtmDpwrALT3 );
    BtmDjncTempBU = BtmDTempABU + ( PARM(BtmDRaTh) * BtmDpwrBUT3 );
    BtmDjncTempBL = BtmDTempABL + ( PARM(BtmDRaTh) * BtmDpwrBLT3 );
    BtmDjncTempCU = BtmDTempACU + ( PARM(BtmDRaTh) * BtmDpwrCUT3 );
    BtmDjncTempCL = BtmDTempACL + ( PARM(BtmDRaTh) * BtmDpwrCLT3 );


    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Check for Max transistor and diode temperature among the 6 channels.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrm5pModel::MaxJncTempChk_5P_T2(void)
{
    //--------------------------------------------------------------------------
    //   Form max transistor junction temperature

    BtmTrJncTempMax = BtmTrJncTempAU;
    if ( BtmTrJncTempAL > BtmTrJncTempMax )
    {
        BtmTrJncTempMax = BtmTrJncTempAL;
    }
    if ( BtmTrJncTempBU > BtmTrJncTempMax )
    {
        BtmTrJncTempMax = BtmTrJncTempBU;
    }
    if ( BtmTrJncTempBL > BtmTrJncTempMax )
    {
        BtmTrJncTempMax = BtmTrJncTempBL;
    }
    if ( BtmTrJncTempCU > BtmTrJncTempMax )
    {
        BtmTrJncTempMax = BtmTrJncTempCU;
    }
    if ( BtmTrJncTempCL > BtmTrJncTempMax )
    {
        BtmTrJncTempMax = BtmTrJncTempCL;
    }

    BtmTrJncTempErr = BtmTrJncTempMax - BtmTrJncTemp;

    if ( BtmTrJncTempErr > 0.0F )
    {
        BtmTrJncTemp += BtmJncTempUpGn * BtmTrJncTempErr;
    }
    else
    {
        BtmTrJncTemp += BtmJncTempDnGn * BtmTrJncTempErr;
    }


    //--------------------------------------------------------------------------
    //   Form max Diode junction temperature

    BtmDjncTempMx = BtmDjncTempAU;
    if ( BtmDjncTempAL > BtmDjncTempMx )
    {
        BtmDjncTempMx = BtmDjncTempAL;
    }
    if ( BtmDjncTempBU > BtmDjncTempMx )
    {
        BtmDjncTempMx = BtmDjncTempBU;
    }
    if ( BtmDjncTempBL > BtmDjncTempMx )
    {
        BtmDjncTempMx = BtmDjncTempBL;
    }
    if ( BtmDjncTempCU > BtmDjncTempMx )
    {
        BtmDjncTempMx = BtmDjncTempCU;
    }
    if ( BtmDjncTempCL > BtmDjncTempMx )
    {
        BtmDjncTempMx = BtmDjncTempCL;
    }

    BtmDjncTempErr = BtmDjncTempMx - BtmDjncTemp;

    if ( BtmDjncTempErr > 0.0 )
    {
        BtmDjncTemp += BtmJncTempUpGn * BtmDjncTempErr;
    }
    else
    {
        BtmDjncTemp += BtmJncTempDnGn * BtmDjncTempErr;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Check for Max transistor and diode temperature among the 6 channels.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrm5pModel::MaxJncTempChk_2P_T3(void)
{
    //--------------------------------------------------------------------------
    //   Form max transistor junction temperature

    BtmTrJncTempMax = BtmTrJncTempAU;
    if ( BtmTrJncTempAL > BtmTrJncTempMax )
    {
        BtmTrJncTempMax = BtmTrJncTempAL;
    }
    if ( BtmTrJncTempBU > BtmTrJncTempMax )
    {
        BtmTrJncTempMax = BtmTrJncTempBU;
    }
    if ( BtmTrJncTempBL > BtmTrJncTempMax )
    {
        BtmTrJncTempMax = BtmTrJncTempBL;
    }
    if ( BtmTrJncTempCU > BtmTrJncTempMax )
    {
        BtmTrJncTempMax = BtmTrJncTempCU;
    }
    if ( BtmTrJncTempCL > BtmTrJncTempMax )
    {
        BtmTrJncTempMax = BtmTrJncTempCL;
    }

    BtmTrJncTempErr = BtmTrJncTempMax - BtmTrJncTemp;

    if ( BtmTrJncTempErr > 0.0F )
    {
        BtmTrJncTemp += BtmJncTempUpGn * BtmTrJncTempErr;
    }
    else
    {
        BtmTrJncTemp += BtmJncTempDnGn * BtmTrJncTempErr;
    }


    //--------------------------------------------------------------------------
    //   Form max Diode junction temperature

    BtmDjncTempMx = BtmDjncTempAU;
    if ( BtmDjncTempAL > BtmDjncTempMx )
    {
        BtmDjncTempMx = BtmDjncTempAL;
    }
    if ( BtmDjncTempBU > BtmDjncTempMx )
    {
        BtmDjncTempMx = BtmDjncTempBU;
    }
    if ( BtmDjncTempBL > BtmDjncTempMx )
    {
        BtmDjncTempMx = BtmDjncTempBL;
    }
    if ( BtmDjncTempCU > BtmDjncTempMx )
    {
        BtmDjncTempMx = BtmDjncTempCU;
    }
    if ( BtmDjncTempCL > BtmDjncTempMx )
    {
        BtmDjncTempMx = BtmDjncTempCL;
    }

    BtmDjncTempErr = BtmDjncTempMx - BtmDjncTemp;

    if ( BtmDjncTempErr > 0.0 )
    {
        BtmDjncTemp += BtmJncTempUpGn * BtmDjncTempErr;
    }
    else
    {
        BtmDjncTemp += BtmJncTempDnGn * BtmDjncTempErr;
    }

    return;
}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Apply over-temperature limits and push corresponding diagnostics
//      on per phase basis.
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrm5pModel::Diagnostic_5P_T2( void )
{
    //--------------------------------------------------------------------------
    // Initialize summary signals

    OUT(BtmOtFaultSum)    = false;
    OUT(BtmOtFaultDlySum) = false;
    OUT(BtmOtAlarmSum)    = false;


    //--------------------------------------------------------------------------
    //   Check for transistor and diode junction temperature faults,
    //   delayed trips, and alarms for Phase A.

    // Phase A device over-temp trip checks
    if ( (BtmTrJncTempAU  > PARM(BtmTrJncOTThr))  ||  //Phase A Upper transistor OT trip
         (BtmTrJncTempAL  > PARM(BtmTrJncOTThr))  ||  //Phase A Lower transistor OT trip
         (BtmDjncTempAU > PARM(BtmDjncOTThr))   ||  //Phase A Upper diode OT trip
         (BtmDjncTempAL > PARM(BtmDjncOTThr)) )     //Phase A Lower diode OT trip
    {
        OUT(BtmOtFaultSum) = true;
        PUSH_DIAG( BtmAjncOtTrp );
    }

    // Phase A device over-temp delayed trip checks
    if ( (BtmTrJncTempAU  > PARM(BtmTrJncOTThDly))  ||  //Phase A Upper transistor OT trip
         (BtmTrJncTempAL  > PARM(BtmTrJncOTThDly))  ||  //Phase A Lower transistor OT trip
         (BtmDjncTempAU > PARM(BtmDjncOTThDly))   ||  //Phase A Upper diode OT trip
         (BtmDjncTempAL > PARM(BtmDjncOTThDly)) )     //Phase A Lower diode OT trip
    {
        OUT(BtmOtFaultDlySum) = true;
        PUSH_DIAG( BtmAjncOtDlyTrp );
    }

    // Phase A device over-temp alarm checks
    if ( (BtmTrJncTempAU  > PARM(BtmTrJncOTOn))     ||  //Phase A Upper transistor OT alarm
         (BtmTrJncTempAL  > PARM(BtmTrJncOTOn)) )       //Phase A Lower transistor OT alarm
    {
        BtmAlarmStPhsA = true;
    }
    else if ( (BtmTrJncTempAU  < PARM(BtmTrJncOTOff))   &&
              (BtmTrJncTempAL  < PARM(BtmTrJncOTOff)) )
    {
        BtmAlarmStPhsA = false;
    }

    if ( (BtmDjncTempAU > PARM(BtmDjncOTOn))      ||  //Phase A Upper diode OT alarm
         (BtmDjncTempAL > PARM(BtmDjncOTOn)) )        //Phase A Lower diode OT alarm
    {
        BtmAlarmStDphsA = true;
    }
    else if ( (BtmDjncTempAU < PARM(BtmDjncOTOff))  &&
              (BtmDjncTempAL < PARM(BtmDjncOTOff)) )
    {
        BtmAlarmStDphsA = false;
    }

    if ( (BtmAlarmStPhsA == true) || (BtmAlarmStDphsA == true) )
    {
        OUT(BtmOtAlarmSum) = true;
        PUSH_DIAG( BtmAjncOtAlm );
    }


    //--------------------------------------------------------------------------
    //   Check for transistor and diode jucntion temperature faults,
    //   delayed trips, and alarms for Phase B.

    // Phase B device over-temp trip checks
    if ( (BtmTrJncTempBU  > PARM(BtmTrJncOTThr))   ||   //Phase B Upper transistor OT trip
         (BtmTrJncTempBL  > PARM(BtmTrJncOTThr))   ||   //Phase B Lower transistor OT trip
         (BtmDjncTempBU > PARM(BtmDjncOTThr))    ||   //Phase B Upper diode OT trip
         (BtmDjncTempBL > PARM(BtmDjncOTThr)) )       //Phase B Lower diode OT trip
    {
        OUT(BtmOtFaultSum) = true;
        PUSH_DIAG( BtmBjncOtTrp );
    }

    // Phase B device over-temp delayed trip checks
    if ( (BtmTrJncTempBU  > PARM(BtmTrJncOTThDly))   ||  //Phase B Upper transistor OT trip
         (BtmTrJncTempBL  > PARM(BtmTrJncOTThDly))   ||  //Phase B Lower transistor OT trip
         (BtmDjncTempBU > PARM(BtmDjncOTThDly))    ||  //Phase B Upper diode OT trip
         (BtmDjncTempBL > PARM(BtmDjncOTThDly)) )      //Phase B Lower diode OT trip
    {
        OUT(BtmOtFaultDlySum) = true;
        PUSH_DIAG( BtmBjncOtDlyTrp );
    }

    // Phase B device over-temp alarm checks
    if ( (BtmTrJncTempBU  > PARM(BtmTrJncOTOn))      ||  //Phase B Upper transistor OT alarm
         (BtmTrJncTempBL  > PARM(BtmTrJncOTOn)) )        //Phase B Lower transistor OT alarm
    {
        BtmAlarmStPhsB = true;
    }
    else if ( (BtmTrJncTempBU  < PARM(BtmTrJncOTOff))   &&
              (BtmTrJncTempBL  < PARM(BtmTrJncOTOff)) )
    {
        BtmAlarmStPhsB = false;
    }

    if ( (BtmDjncTempBU > PARM(BtmDjncOTOn))       ||  //Phase B Upper diode OT alarm
         (BtmDjncTempBL > PARM(BtmDjncOTOn)) )         //Phase B Lower diode OT alarm
    {
        BtmAlarmStDphsB = true;
    }
    else if ( (BtmDjncTempBU < PARM(BtmDjncOTOff))  &&
              (BtmDjncTempBL < PARM(BtmDjncOTOff)) )
    {
        BtmAlarmStDphsB = false;
    }

    if ( (BtmAlarmStPhsB == true) || (BtmAlarmStDphsB == true) )
    {
        OUT(BtmOtAlarmSum) = true;
        PUSH_DIAG( BtmBjncOtAlm );
    }


    //--------------------------------------------------------------------------
    //   Check for transistor and diode jucntion temperature faults,
    //   delayed trips, and alarms for Phase C.

    // Phase C device over-temp trip checks
    if ( (BtmTrJncTempCU  > PARM(BtmTrJncOTThr))     ||  //Phase C Upper transistor OT trip
         (BtmTrJncTempCL  > PARM(BtmTrJncOTThr))     ||  //Phase C Lower transistor OT trip
         (BtmDjncTempCU > PARM(BtmDjncOTThr))      ||  //Phase C Upper diode OT trip
         (BtmDjncTempCL > PARM(BtmDjncOTThr)) )        //Phase C Lower diode OT trip
    {
        OUT(BtmOtFaultSum) = true;
        PUSH_DIAG( BtmCjncOtTrp );
    }

    // Phase C device over-temp delayed trip checks
    if ( (BtmTrJncTempCU  > PARM(BtmTrJncOTThDly))   ||  //Phase C Upper transistor OT trip
         (BtmTrJncTempCL  > PARM(BtmTrJncOTThDly))   ||  //Phase C Lower transistor OT trip
         (BtmDjncTempCU > PARM(BtmDjncOTThDly))    ||  //Phase C Upper diode OT trip
         (BtmDjncTempCL > PARM(BtmDjncOTThDly)) )      //Phase C Lower diode OT trip
    {
        OUT(BtmOtFaultDlySum) = true;
        PUSH_DIAG( BtmCjncOtDlyTrp );
    }

    // Phase C device over-temp alarm checks
    if ( (BtmTrJncTempCU  > PARM(BtmTrJncOTOn))      ||  //Phase C Upper transistor OT alarm
         (BtmTrJncTempCL  > PARM(BtmTrJncOTOn)) )        //Phase C Lower transistor OT alarm
    {
        BtmAlarmStPhsC = true;
    }
    else if ( (BtmTrJncTempCU  < PARM(BtmTrJncOTOff))   &&
              (BtmTrJncTempCL  < PARM(BtmTrJncOTOff)) )
    {
        BtmAlarmStPhsC = false;
    }

    if ( (BtmDjncTempCU > PARM(BtmDjncOTOn))       ||  //Phase C Upper diode OT alarm
         (BtmDjncTempCL > PARM(BtmDjncOTOn)) )         //Phase C Lower diode OT alarm
    {
        BtmAlarmStDphsC = true;
    }
    else if ( (BtmDjncTempCU < PARM(BtmDjncOTOff))  &&
              (BtmDjncTempCL < PARM(BtmDjncOTOff)) )
    {
        BtmAlarmStDphsC = false;
    }

    if ( (BtmAlarmStPhsC == true) || (BtmAlarmStDphsC == true) )
    {
        OUT(BtmOtAlarmSum) = true;
        PUSH_DIAG( BtmCjncOtAlm );
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Apply over-temperature limits and push corresponding diagnostics
//      on per phase basis.
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrm5pModel::Diagnostic_2P_T3( void )
{
    //--------------------------------------------------------------------------
    // Initialize summary signals

    OUT(BtmOtFaultSum)    = false;
    OUT(BtmOtFaultDlySum) = false;
    OUT(BtmOtAlarmSum)    = false;


    //--------------------------------------------------------------------------
    //   Check for transistor and diode junction temperature faults,
    //   delayed trips, and alarms for Phase A.

    // Phase A device over-temp trip checks
    if ( (BtmTrJncTempAU  > PARM(BtmTrJncOTThr))  ||  //Phase A Upper transistor OT trip
         (BtmTrJncTempAL  > PARM(BtmTrJncOTThr))  ||  //Phase A Lower transistor OT trip
         (BtmDjncTempAU > PARM(BtmDjncOTThr))   ||  //Phase A Upper diode OT trip
         (BtmDjncTempAL > PARM(BtmDjncOTThr)) )     //Phase A Lower diode OT trip
    {
        OUT(BtmOtFaultSum) = true;
        PUSH_DIAG( BtmAjncOtTrp );
    }

    // Phase A device over-temp delayed trip checks
    if ( (BtmTrJncTempAU  > PARM(BtmTrJncOTThDly))  ||  //Phase A Upper transistor OT trip
         (BtmTrJncTempAL  > PARM(BtmTrJncOTThDly))  ||  //Phase A Lower transistor OT trip
         (BtmDjncTempAU > PARM(BtmDjncOTThDly))   ||  //Phase A Upper diode OT trip
         (BtmDjncTempAL > PARM(BtmDjncOTThDly)) )     //Phase A Lower diode OT trip
    {
        OUT(BtmOtFaultDlySum) = true;
        PUSH_DIAG( BtmAjncOtDlyTrp );
    }

    // Phase A device over-temp alarm checks
    if ( (BtmTrJncTempAU  > PARM(BtmTrJncOTOn))     ||  //Phase A Upper transistor OT alarm
         (BtmTrJncTempAL  > PARM(BtmTrJncOTOn)) )       //Phase A Lower transistor OT alarm
    {
        BtmAlarmStPhsA = true;
    }
    else if ( (BtmTrJncTempAU  < PARM(BtmTrJncOTOff))   &&
              (BtmTrJncTempAL  < PARM(BtmTrJncOTOff)) )
    {
        BtmAlarmStPhsA = false;
    }

    if ( (BtmDjncTempAU > PARM(BtmDjncOTOn))      ||  //Phase A Upper diode OT alarm
         (BtmDjncTempAL > PARM(BtmDjncOTOn)) )        //Phase A Lower diode OT alarm
    {
        BtmAlarmStDphsA = true;
    }
    else if ( (BtmDjncTempAU < PARM(BtmDjncOTOff))  &&
              (BtmDjncTempAL < PARM(BtmDjncOTOff)) )
    {
        BtmAlarmStDphsA = false;
    }

    if ( (BtmAlarmStPhsA == true) || (BtmAlarmStDphsA == true) )
    {
        OUT(BtmOtAlarmSum) = true;
        PUSH_DIAG( BtmAjncOtAlm );
    }


    //--------------------------------------------------------------------------
    //   Check for transistor and diode jucntion temperature faults,
    //   delayed trips, and alarms for Phase B.

    // Phase B device over-temp trip checks
    if ( (BtmTrJncTempBU  > PARM(BtmTrJncOTThr))   ||   //Phase B Upper transistor OT trip
         (BtmTrJncTempBL  > PARM(BtmTrJncOTThr))   ||   //Phase B Lower transistor OT trip
         (BtmDjncTempBU > PARM(BtmDjncOTThr))    ||   //Phase B Upper diode OT trip
         (BtmDjncTempBL > PARM(BtmDjncOTThr)) )       //Phase B Lower diode OT trip
    {
        OUT(BtmOtFaultSum) = true;
        PUSH_DIAG( BtmBjncOtTrp );
    }

    // Phase B device over-temp delayed trip checks
    if ( (BtmTrJncTempBU  > PARM(BtmTrJncOTThDly))   ||  //Phase B Upper transistor OT trip
         (BtmTrJncTempBL  > PARM(BtmTrJncOTThDly))   ||  //Phase B Lower transistor OT trip
         (BtmDjncTempBU > PARM(BtmDjncOTThDly))    ||  //Phase B Upper diode OT trip
         (BtmDjncTempBL > PARM(BtmDjncOTThDly)) )      //Phase B Lower diode OT trip
    {
        OUT(BtmOtFaultDlySum) = true;
        PUSH_DIAG( BtmBjncOtDlyTrp );
    }

    // Phase B device over-temp alarm checks
    if ( (BtmTrJncTempBU  > PARM(BtmTrJncOTOn))      ||  //Phase B Upper transistor OT alarm
         (BtmTrJncTempBL  > PARM(BtmTrJncOTOn)) )        //Phase B Lower transistor OT alarm
    {
        BtmAlarmStPhsB = true;
    }
    else if ( (BtmTrJncTempBU  < PARM(BtmTrJncOTOff))   &&
              (BtmTrJncTempBL  < PARM(BtmTrJncOTOff)) )
    {
        BtmAlarmStPhsB = false;
    }

    if ( (BtmDjncTempBU > PARM(BtmDjncOTOn))       ||  //Phase B Upper diode OT alarm
         (BtmDjncTempBL > PARM(BtmDjncOTOn)) )         //Phase B Lower diode OT alarm
    {
        BtmAlarmStDphsB = true;
    }
    else if ( (BtmDjncTempBU < PARM(BtmDjncOTOff))  &&
              (BtmDjncTempBL < PARM(BtmDjncOTOff)) )
    {
        BtmAlarmStDphsB = false;
    }

    if ( (BtmAlarmStPhsB == true) || (BtmAlarmStDphsB == true) )
    {
        OUT(BtmOtAlarmSum) = true;
        PUSH_DIAG( BtmBjncOtAlm );
    }


    //--------------------------------------------------------------------------
    //   Check for transistor and diode jucntion temperature faults,
    //   delayed trips, and alarms for Phase C.

    // Phase C device over-temp trip checks
    if ( (BtmTrJncTempCU  > PARM(BtmTrJncOTThr))     ||  //Phase C Upper transistor OT trip
         (BtmTrJncTempCL  > PARM(BtmTrJncOTThr))     ||  //Phase C Lower transistor OT trip
         (BtmDjncTempCU > PARM(BtmDjncOTThr))      ||  //Phase C Upper diode OT trip
         (BtmDjncTempCL > PARM(BtmDjncOTThr)) )        //Phase C Lower diode OT trip
    {
        OUT(BtmOtFaultSum) = true;
        PUSH_DIAG( BtmCjncOtTrp );
    }

    // Phase C device over-temp delayed trip checks
    if ( (BtmTrJncTempCU  > PARM(BtmTrJncOTThDly))   ||  //Phase C Upper transistor OT trip
         (BtmTrJncTempCL  > PARM(BtmTrJncOTThDly))   ||  //Phase C Lower transistor OT trip
         (BtmDjncTempCU > PARM(BtmDjncOTThDly))    ||  //Phase C Upper diode OT trip
         (BtmDjncTempCL > PARM(BtmDjncOTThDly)) )      //Phase C Lower diode OT trip
    {
        OUT(BtmOtFaultDlySum) = true;
        PUSH_DIAG( BtmCjncOtDlyTrp );
    }

    // Phase C device over-temp alarm checks
    if ( (BtmTrJncTempCU  > PARM(BtmTrJncOTOn))      ||  //Phase C Upper transistor OT alarm
         (BtmTrJncTempCL  > PARM(BtmTrJncOTOn)) )        //Phase C Lower transistor OT alarm
    {
        BtmAlarmStPhsC = true;
    }
    else if ( (BtmTrJncTempCU  < PARM(BtmTrJncOTOff))   &&
              (BtmTrJncTempCL  < PARM(BtmTrJncOTOff)) )
    {
        BtmAlarmStPhsC = false;
    }

    if ( (BtmDjncTempCU > PARM(BtmDjncOTOn))       ||  //Phase C Upper diode OT alarm
         (BtmDjncTempCL > PARM(BtmDjncOTOn)) )         //Phase C Lower diode OT alarm
    {
        BtmAlarmStDphsC = true;
    }
    else if ( (BtmDjncTempCU < PARM(BtmDjncOTOff))  &&
              (BtmDjncTempCL < PARM(BtmDjncOTOff)) )
    {
        BtmAlarmStDphsC = false;
    }

    if ( (BtmAlarmStPhsC == true) || (BtmAlarmStDphsC == true) )
    {
        OUT(BtmOtAlarmSum) = true;
        PUSH_DIAG( BtmCjncOtAlm );
    }

    return;
}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Report out private junction temperatures in simulation
//
// EXECUTION LEVEL
//      Task Sim
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgThrm5pModel::SimReadTemp(BtmTempSimTyp & target)
{
    target.BtmTrJncTempAL = BtmTrJncTempAL;
    target.BtmTrJncTempAU = BtmTrJncTempAU;
    target.BtmTrJncTempBL = BtmTrJncTempBL;
    target.BtmTrJncTempBU = BtmTrJncTempBU;
    target.BtmTrJncTempCL = BtmTrJncTempCL;
    target.BtmTrJncTempCU = BtmTrJncTempCU;

    target.BtmDjncTempAL  = BtmDjncTempAL;
    target.BtmDjncTempAU  = BtmDjncTempAU;
    target.BtmDjncTempBL  = BtmDjncTempBL;
    target.BtmDjncTempBU  = BtmDjncTempBU;
    target.BtmDjncTempCL  = BtmDjncTempCL;
    target.BtmDjncTempCU  = BtmDjncTempCU;

    target.BtmTrJncTemp   = BtmTrJncTemp;
    target.BtmDjncTemp    = BtmDjncTemp;
}
