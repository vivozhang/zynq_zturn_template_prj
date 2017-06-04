///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Bridge Current Sharing Simulator
//
// DESCRIPTION
//      This file contains the methods for the bridge current sharing
//      simulator class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      s1BrgCurShrSim.vsd, s1BrgCurShrSim.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
#include <math.h>
#include "x0vector.h"
// core
// framework
// product
#include "s1BrgCurShrSim.h"


// Constants
//----------


// Externals
//----------


// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Class Instances
//----------------


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
cBrgCurShrSim::cBrgCurShrSim
(
        InListTyp    InList,
        InsListTyp   InsList
)
        : In(InList), Ins(InsList)
{

    OUT(CshVaSim)        = 0.0;
    OUT(CshVbSim)        = 0.0;
    OUT(CshVcSim)        = 0.0;

    CshIalSim            = 0.0;
    CshIbeSim            = 0.0;

    CshI0CrsSim          = 0.0;
    CshIalCrsSim         = 0.0;
    CshIbeCrsSim         = 0.0;

    OUT(CshIaTotBrg1Sim) = 0.0;
    OUT(CshIbTotBrg1Sim) = 0.0;
    OUT(CshIcTotBrg1Sim) = 0.0;
    CshIalBrg1Sim        = 0.0;
    CshIbeBrg1Sim        = 0.0;

    OUT(CshIaTotBrg2Sim) = 0.0;
    OUT(CshIbTotBrg2Sim) = 0.0;
    OUT(CshIcTotBrg2Sim) = 0.0;
    CshIalBrg2Sim        = 0.0;
    CshIbeBrg2Sim        = 0.0;

    CshVaSatBrg1Sim      = 0.0;
    CshVbSatBrg1Sim      = 0.0;
    CshVcSatBrg1Sim      = 0.0;

    CshVaSatBrg2Sim      = 0.0;
    CshVbSatBrg2Sim      = 0.0;
    CshVcSatBrg2Sim      = 0.0;

    CshVaOutBrg1Sim      = 0.0;
    CshVbOutBrg1Sim      = 0.0;
    CshVcOutBrg1Sim      = 0.0;

    CshVaOutBrg2Sim      = 0.0;
    CshVbOutBrg2Sim      = 0.0;
    CshVcOutBrg2Sim      = 0.0;

    CshV0OutBrg1Sim      = 0.0;
    CshValOutBrg1Sim     = 0.0;
    CshVbeOutBrg1Sim     = 0.0;

    CshV0OutBrg2Sim      = 0.0;
    CshValOutBrg2Sim     = 0.0;
    CshVbeOutBrg2Sim     = 0.0;

    CshV0CrsSim          = 0.0;

    CshValNrmSim         = 0.0;
    CshVbeNrmSim         = 0.0;

    CshValCrsSim         = 0.0;
    CshVbeCrsSim         = 0.0;

    CshValCrsLdelSim     = 0.0;
    CshValCrsRdelSim     = 0.0;
    CshValCrsTotSim      = 0.0;

    CshVbeCrsLdelSim     = 0.0;
    CshVbeCrsRdelSim     = 0.0;
    CshVbeCrsTotSim      = 0.0;

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
cBrgCurShrSim::~cBrgCurShrSim ()
{
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by the class.
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cBrgCurShrSim::RunRules (void)
{
    int Status = SUCCESS;

    //---------------------------------------------------------------------
    // bridge switch parameters

    SET_FV(CshRsatSim,                0.1);   // saturation resistance (ohms)

    // bridge 1
    SET_FV(CshVaSatUpBrg1Sim,         1.5);   // saturation voltage up switch phase a (volts)
    SET_FV(CshVbSatUpBrg1Sim,         1.5);   // saturation voltage up switch phase b (volts)
    SET_FV(CshVcSatUpBrg1Sim,         1.5);   // saturation voltage up switch phase c (volts)

    SET_FV(CshVaSatDnBrg1Sim,         1.5);   // saturation voltage down switch phase a (volts)
    SET_FV(CshVbSatDnBrg1Sim,         1.5);   // saturation voltage down switch phase b (volts)
    SET_FV(CshVcSatDnBrg1Sim,         1.5);   // saturation voltage down switch phase c (volts)

    SET_FV(CshRaDynBrg1Sim,        0.0015);   // equivalent switch dynamic resistance phase a (ohms)
    SET_FV(CshRbDynBrg1Sim,        0.0015);   // equivalent switch dynamic resistance phase b (ohms)
    SET_FV(CshRcDynBrg1Sim,        0.0015);   // equivalent switch dynamic resistance phase c (ohms)

    // bridge 2
    SET_FV(CshVaSatUpBrg2Sim,         1.5);   // saturation voltage up switch phase a (volts)
    SET_FV(CshVbSatUpBrg2Sim,         1.5);   // saturation voltage up switch phase b (volts)
    SET_FV(CshVcSatUpBrg2Sim,         1.5);   // saturation voltage up switch phase c (volts)

    SET_FV(CshVaSatDnBrg2Sim,         1.5);   // saturation voltage down switch phase a (volts)
    SET_FV(CshVbSatDnBrg2Sim,         1.5);   // saturation voltage down switch phase b (volts)
    SET_FV(CshVcSatDnBrg2Sim,         1.5);   // saturation voltage down switch phase c (volts)

    SET_FV(CshRaDynBrg2Sim,        0.0015);   // equivalent switch dynamic resistance phase a (ohms)
    SET_FV(CshRbDynBrg2Sim,        0.0015);   // equivalent switch dynamic resistance phase b (ohms)
    SET_FV(CshRcDynBrg2Sim,        0.0015);   // equivalent switch dynamic resistance phase c (ohms)

    //---------------------------------------------------------------------
    // bridge inductor parameters


    SET_FV(CshBrg1ImbRatioSim,      1.0);     // default no imbalance in the bridge reactor
    SET_FV(CshBrg2ImbRatioSim,      1.0);     // default no imbalance in the bridge reactor
    // bridge 1
    SET_FV(CshRbrg1Sim,     PARM(CshBrg1ImbRatioSim) * INS(CrsRbrg1));   // bridge inductor resistance (ohms)
    SET_FV(CshLbrg1Sim,     PARM(CshBrg1ImbRatioSim) * INS(CrsLbrg1));   // bridge inductor + cable normal mode equivalent inductance (henries)
    SET_FV(CshL0brg1Sim,    PARM(CshBrg1ImbRatioSim) * INS(CmnLbrg1));   // bridge inductor + cable common mode equivalent inductance (henries)

    // bridge 2
    SET_FV(CshRbrg2Sim,     PARM(CshBrg2ImbRatioSim) * INS(CrsRbrg2));   // bridge inductor resistance (ohms)
    SET_FV(CshLbrg2Sim,     PARM(CshBrg2ImbRatioSim) * INS(CrsLbrg2));   // bridge inductor + cable normal mode equivalent inductance (henries)
    SET_FV(CshL0brg2Sim,    PARM(CshBrg2ImbRatioSim) * INS(CmnLbrg2));   // bridge inductor + cable common mode equivalent inductance (henries)

    CshLSim   = 0.5F * ( PARM(CshLbrg1Sim)  + PARM(CshLbrg2Sim)  );   // average normal mode inductance (henries)
    CshL0Sim  = 0.5F * ( PARM(CshL0brg1Sim) + PARM(CshL0brg2Sim) );   // average common mode inductance (henries)
    CshRSim   = 0.5F * ( PARM(CshRbrg1Sim)  + PARM(CshRbrg2Sim)  );   // average resistance (henries)

    CshLdelSim = 0.5F * ( PARM(CshLbrg1Sim) - PARM(CshLbrg2Sim) );    // bridge inductor inductance asymmetry
    CshRdelSim = 0.5F * ( PARM(CshRbrg1Sim) - PARM(CshRbrg2Sim) );    // bridge inductor resistance asymmetry

    CshLrcpSim  = NoNANLim( 1.0 / CshLSim,   100.0e6, -100.0e6 );
    CshL0rcpSim = NoNANLim( 1.0 / CshL0Sim,  100.0e6, -100.0e6 );
    CshRrcpSim  = NoNANLim( 1.0 / CshRSim,   100.0e6, -100.0e6 );

    //---------------------------------------------------------------------
    // filters

    // normal mode cross current filter (model of current dynamics) (rad/s)
    SET_FV(CshCrsIfilWSim,  CshRSim * CshLrcpSim );
    CshCrsIfilWdtSim = PARM(CshCrsIfilWSim) * INS(DelTm1);

    // common mode filter (model of current dynamics) (rad/s)
    SET_FV(CshCmnIfilWSim,  CshRSim * CshL0rcpSim );
    CshCmnIfilWdtSim = PARM(CshCmnIfilWSim) * INS(DelTm1);

    // general filter for di/dt approximation (modeling of coupling between normal and cross modes) (rad/s)
    SET_FV(CshDiDtFilWSim, 1000.0);
    CshDiDtFilWdtSim = PARM(CshDiDtFilWSim) * INS(DelTm1);

    return (Status);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Simulate current-voltage transfer function in the modeling of bridge
//      asymmetries.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgCurShrSim::SwSatVoltCalc ( float Vdrop, float Vup, float Vdn, float& Vsat )
{
    if ( Vdrop > Vup )
    {
        Vsat = Vup;
    }
    else if ( Vdrop < -Vdn )
    {
        Vsat = -Vdn;
    }
    else
    {
        Vsat = Vdrop;
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This is the PRIMARY RUN-TIME Method for this Class.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cBrgCurShrSim::Task1 ( void )
{

    //-------------------------------------------------------------------------
    // bridge phase current calculation
    {
        CshIalZSim = CshIalSim;
        CshIbeZSim = CshIbeSim;

        // rotate a,b,c current inputs to alpha beta
        cVectorf::ABCToAlphaBeta(IN(IaPlant), IN(IbPlant), IN(IcPlant), &CshIalSim, &CshIbeSim);

        // multiply by 0.5 since current decomposition is made per converter
        // CshIalSim and CshIbeSim are averaged bridge alpha-beta current components
        CshIalSim = 0.5F * CshIalSim;
        CshIbeSim = 0.5F * CshIbeSim;

        // bridge 1 total currents (cummulative + circulating currents)
        //-------------------------------------------------------------
        {
            CshIalBrg1Sim = CshIalSim + CshIalCrsSim;
            CshIbeBrg1Sim = CshIbeSim + CshIbeCrsSim;

            cVectorf::AlphaBetaToABC(CshIalBrg1Sim, CshIbeBrg1Sim, &OUT(CshIaTotBrg1Sim), &OUT(CshIbTotBrg1Sim), &OUT(CshIcTotBrg1Sim));

            OUT(CshIaTotBrg1Sim) += CshI0CrsSim;    // add common mode to a,b,c currents
            OUT(CshIbTotBrg1Sim) += CshI0CrsSim;
            OUT(CshIcTotBrg1Sim) += CshI0CrsSim;
        }

        // bridge 2 total currents (cummulative + circulating currents)
        //-------------------------------------------------------------
        {
            CshIalBrg2Sim = CshIalSim - CshIalCrsSim;
            CshIbeBrg2Sim = CshIbeSim - CshIbeCrsSim;

            cVectorf::AlphaBetaToABC(CshIalBrg2Sim, CshIbeBrg2Sim, &OUT(CshIaTotBrg2Sim), &OUT(CshIbTotBrg2Sim), &OUT(CshIcTotBrg2Sim));

            OUT(CshIaTotBrg2Sim) -= CshI0CrsSim;    // substract common mode to a,b,c currents
            OUT(CshIbTotBrg2Sim) -= CshI0CrsSim;
            OUT(CshIcTotBrg2Sim) -= CshI0CrsSim;
        }
    }

    //-------------------------------------------------------------------------
    // bridge voltage distortion model
    {
        // bridge 1 : calculate distortion due to saturation voltage
        SwSatVoltCalc(PARM(CshRsatSim)*CshIaTotBrg1Sim, PARM(CshVaSatUpBrg1Sim), PARM(CshVaSatDnBrg1Sim), CshVaSatBrg1Sim);
        CshVaSatBrg1Sim += PARM(CshRaDynBrg1Sim) * CshIaTotBrg1Sim;

        SwSatVoltCalc(PARM(CshRsatSim)*CshIbTotBrg1Sim, PARM(CshVbSatUpBrg1Sim), PARM(CshVbSatDnBrg1Sim), CshVbSatBrg1Sim);
        CshVbSatBrg1Sim += PARM(CshRbDynBrg1Sim) * CshIbTotBrg1Sim;

        SwSatVoltCalc(PARM(CshRsatSim)*CshIcTotBrg1Sim, PARM(CshVcSatUpBrg1Sim), PARM(CshVcSatDnBrg1Sim), CshVcSatBrg1Sim);
        CshVcSatBrg1Sim += PARM(CshRcDynBrg1Sim) * CshIcTotBrg1Sim;

        // brige 1 : calculate converter resulting output voltage
        //-------------------------------------------------------
        CshVaOutBrg1Sim = IN(VaBrg1) - CshVaSatBrg1Sim;
        CshVbOutBrg1Sim = IN(VbBrg1) - CshVbSatBrg1Sim;
        CshVcOutBrg1Sim = IN(VcBrg1) - CshVcSatBrg1Sim;

        // bridge 2 : calculate distortion due to saturation voltage
        SwSatVoltCalc(PARM(CshRsatSim)*CshIaTotBrg2Sim, PARM(CshVaSatUpBrg2Sim), PARM(CshVaSatDnBrg2Sim), CshVaSatBrg2Sim);
        CshVaSatBrg2Sim += PARM(CshRaDynBrg2Sim) * CshIaTotBrg2Sim;

        SwSatVoltCalc(PARM(CshRsatSim)*CshIbTotBrg2Sim, PARM(CshVbSatUpBrg2Sim), PARM(CshVbSatDnBrg2Sim), CshVbSatBrg2Sim);
        CshVbSatBrg2Sim += PARM(CshRbDynBrg2Sim) * CshIbTotBrg2Sim;

        SwSatVoltCalc(PARM(CshRsatSim)*CshIcTotBrg2Sim, PARM(CshVcSatUpBrg2Sim), PARM(CshVcSatDnBrg2Sim), CshVcSatBrg2Sim);
        CshVcSatBrg2Sim += PARM(CshRcDynBrg2Sim) * CshIcTotBrg2Sim;

        // brige 2 : calculate converter resulting output voltage
        //-------------------------------------------------------
        CshVaOutBrg2Sim = IN(VaBrg2) - CshVaSatBrg2Sim;
        CshVbOutBrg2Sim = IN(VbBrg2) - CshVbSatBrg2Sim;
        CshVcOutBrg2Sim = IN(VcBrg2) - CshVcSatBrg2Sim;
    }

    //-------------------------------------------------------------------------
    // decompose bridge voltages into common, cumulative, and differential components
    {
        // bridge 1 : common mode voltage
        CshV0OutBrg1Sim = ( CshVaOutBrg1Sim + CshVbOutBrg1Sim + CshVcOutBrg1Sim) * (float)(1.0/3.0);

        // bridge 1 : alpha beta voltage components
        cVectorf::ABCToAlphaBeta(CshVaOutBrg1Sim, CshVbOutBrg1Sim, CshVcOutBrg1Sim, &CshValOutBrg1Sim, &CshVbeOutBrg1Sim);

        // bridge 2 : common mode voltage
        CshV0OutBrg2Sim = ( CshVaOutBrg2Sim + CshVbOutBrg2Sim + CshVcOutBrg2Sim) * (float)(1.0/3.0);

        // bridge 2 : alpha beta voltage components
        cVectorf::ABCToAlphaBeta(CshVaOutBrg2Sim, CshVbOutBrg2Sim, CshVcOutBrg2Sim, &CshValOutBrg2Sim, &CshVbeOutBrg2Sim);

        // converter voltage components
        CshV0CrsSim  = 0.5F * (CshV0OutBrg1Sim - CshV0OutBrg2Sim);      // common mode

        CshValNrmSim = 0.5F * (CshValOutBrg1Sim + CshValOutBrg2Sim);    // cummulative mode
        CshVbeNrmSim = 0.5F * (CshVbeOutBrg1Sim + CshVbeOutBrg2Sim);

        CshValCrsSim = 0.5F * (CshValOutBrg1Sim - CshValOutBrg2Sim);    // differential mode
        CshVbeCrsSim = 0.5F * (CshVbeOutBrg1Sim - CshVbeOutBrg2Sim);

        // model function output, cummulative voltages
        //--------------------------------------------
        cVectorf::AlphaBetaToABC(CshValNrmSim, CshVbeNrmSim, &OUT(CshVaSim), &OUT(CshVbSim), &OUT(CshVcSim));
    }

    //-------------------------------------------------------------------------
    // bridge cross current models
    {
        // common mode cross current
        //--------------------------
        CshI0CrsSim += CshCmnIfilWdtSim * ( CshV0CrsSim * CshRrcpSim - CshI0CrsSim);

        // alpha component: calculate cross voltage resulted from the 2 converters inductance asymmetry
        CshValCrsLdelSim += ( PARM(CshDiDtFilWSim) * CshLdelSim * (CshIalSim - CshIalZSim) ) -
                              ( CshDiDtFilWdtSim * CshValCrsLdelSim );

        // alpha component: calculate cross voltage resulted from the 2 converters resistance asymmetry
        CshValCrsRdelSim = CshIalSim * CshRdelSim;

        // alpha component: total normal mode cross voltage
        CshValCrsTotSim = CshValCrsSim - CshValCrsLdelSim - CshValCrsRdelSim;

        // alpha component cross current
        //------------------------------
        CshIalCrsSim += CshCrsIfilWdtSim * ( CshValCrsTotSim * CshRrcpSim - CshIalCrsSim);

        // beta component: calculate cross voltage resulted from the 2 converters inductance asymmetry
        CshVbeCrsLdelSim += ( PARM(CshDiDtFilWSim) * CshLdelSim * (CshIbeSim - CshIbeZSim) ) -
                              ( CshDiDtFilWdtSim * CshVbeCrsLdelSim );

        // beta component: calculate cross voltage resulted from the 2 converters resistance asymmetry
        CshVbeCrsRdelSim = CshIbeSim * CshRdelSim;

        // beta component: total normal mode cross voltage
        CshVbeCrsTotSim = CshVbeCrsSim - CshVbeCrsLdelSim - CshVbeCrsRdelSim;

        // beta component cross current
        //-----------------------------
        CshIbeCrsSim += CshCrsIfilWdtSim * ( CshVbeCrsTotSim * CshRrcpSim - CshIbeCrsSim);
    }

    return;
}


