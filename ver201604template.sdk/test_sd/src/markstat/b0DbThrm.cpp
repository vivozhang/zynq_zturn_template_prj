///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Dynamic brake thermal model
//
// DESCRIPTION:
//      This file contains the methods for the cDbThrmModel class.
//
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0DbThrm.vsd and b0DbThrm.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
// product
#include "product.h"
#include "b0DbThrm.h"


// Constants
//----------


// Externals
//----------
extern unsigned     SimAcclAct;

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
//      Class instancing
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cDbThrmModel::cDbThrmModel
(
        InListTyp     InList,
        InsListTyp    InsList
)
        : In(InList), Ins(InsList)
{
    ThrmModelInit();

    DtmTrJncTemp        = 0.0F;
    OUT(DtmOtLock)      = false;
    OUT(DtmTrTmpRdy)    = false;
    OUT(DtmCoolingReq)  = false;
    OUT(DtmTrJncTempT3) = 0.0F;
    OUT(DtmTrPwrLoss)   = 0.0F;

    DtmSwLoss           = 0.0F;
    DtmTrSw0Gn          = 0.0F;
    DtmTrSw1Gn          = 0.0F;
    DtmTrSw2Gn          = 0.0F;
    DtmTrCondPwr        = 0.0F;
    DtmTrPwrJnNd3       = 0.0F;
    DtmTrPwrJnNd3T2     = 0.0F;
    DtmTrCjnGn          = 0.0F;
    DtmTrNd3Temp        = 0.0F;
    DtmTrNd3TempT1      = 0.0F;
    DtmTrCnd3Gn         = 0.0F;
    DtmTrPwrNd3Nd2      = 0.0F;
    DtmTrPwrNd3Nd2T3    = 0.0F;
    DtmTrPwrNd2Nd1      = 0.0F;
    DtmTrPwrNd1Clnt     = 0.0F;
    DtmRthJnNd3Rcp      = 0.0F;
    DtmRthNd3Nd2Rcp     = 0.0F;
    DtmTrNd2Temp        = 0.0F;
    DtmTrNd2TempT2      = 0.0F;
    DtmTrNd1Temp        = 0.0F;
    DtmRefTemp          = 0.0F;

    DtmEdtTau           = 0.0F;
    DtmE2DtTau          = 0.0F;
    DtmTrpTau           = 0.0F;
    DtmOvldTrpTm        = 0.0F;
    DtmDsbTm            = 0.0F;
    DtmLinkVdcT3        = 0.0F;
    DtmVdbComp          = 0.0F;
    DtmOtLockT3         = false;
    DtmClearOtLock      = false;
    DtmClearOtLockT1    = false;

    DtmJncHot           = false;

    DtmTrRefModel[0][0] = 0.0F;
    DtmTrRefModel[0][1] = 0.0F;
    DtmTrRefModel[0][2] = 0.0F;
    DtmTrRefModel[0][3] = 0.0F;
    DtmTrRefModel[1][0] = 0.0F;
    DtmTrRefModel[1][1] = 0.0F;
    DtmTrRefModel[1][2] = 0.0F;
    DtmTrRefModel[1][3] = 0.0F;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Initialize class data members
//
// EXECUTION LEVEL
//      Class instancing and Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbThrmModel::ThrmModelInit( void )
{
    // Set all currents and integrators to zero
    DtmIavg             = 0.0F;
    // clear all faults
    OUT(DtmOverload)    = false;
    OUT(DtmTrpOverload) = false;
    OUT(DtmDsbOverload) = false;
    OUT(DtmTrPwrLoss)= 0.0F;

    DtmIo            = 0.0F;
    DtmIdel          = 0.0F;
    DtmIest          = 0.0F;
    OUT(DtmSwSt)     = false;
    DtmSwStZ1        = false;
    DtmIsqrInt       = 0.0F;
    DtmIint          = 0.0F;
    DtmAvgIsqr       = 0.0F;
    DtmPavg          = 0.0F;
    DtmPavgT3        = 0.0F;
    DtmIavg          = 0.0F;
    OUT(DtmPdisspt)  = 0.0F;
    DtmVr            = 0.0F;
    DtmIr            = 0.0F;

    DtmSwLoss        = 0.0F;
    DtmTrCondPwr     = 0.0F;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cDbThrmModel::~cDbThrmModel( void )
{
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by DB thermal model (Dtm) class.
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
int cDbThrmModel::RunRules( void )
{
    int Status = SUCCESS;

    #include "b0DbThrm.inc"

    DtmTrSw0Gn = PARM(DtmTrSw0) * INS(DelTm1Rcp);
    DtmTrSw1Gn = PARM(DtmTrSw1) * INS(DelTm1Rcp);
    DtmTrSw2Gn = PARM(DtmTrSw2) * INS(DelTm1Rcp);

    DtmTrCjnGn   = NoNANLim( INS(DelTm1)/PARM(DtmTrCJnt), 1.0e6F, -1.0e6F );
    DtmTrCnd3Gn  = NoNANLim( INS(DelTm2)/PARM(DtmTrCnd3), 1.0e6F, -1.0e6F );

    DtmRthJnNd3Rcp  = NoNANLim( 1.0F/PARM(DtmTrRthJntNd3),  1.0e6F, -1.0e6F );
    DtmRthNd3Nd2Rcp = NoNANLim( 1.0F/PARM(DtmTrRthNd3Nd2),  1.0e6F, -1.0e6F );

    DtmTrRefModel[0][0] = NoNANLim( INS(DelTm3)/PARM(DtmTrCnd2), 1.0e6F, -1.0e6F );
    DtmTrRefModel[0][1] = NoNANLim( INS(DelTm3)/PARM(DtmTrCnd1), 1.0e6F, -1.0e6F );
    DtmTrRefModel[0][2] = NoNANLim( 1.0F/PARM(DtmTrRthNd2Nd1),  1.0e6F, -1.0e6F );
    DtmTrRefModel[0][3] = NoNANLim( 1.0F/PARM(DtmTrRthNd1Clnt), 1.0e6F, -1.0e6F );
    DtmTrRefModel[1][0] = NoNANLim( INS(DelTm3)/PARM(DtmTrCnd2Ref2), 1.0e6F, -1.0e6F );
    DtmTrRefModel[1][1] = NoNANLim( INS(DelTm3)/PARM(DtmTrCnd1Ref2), 1.0e6F, -1.0e6F );
    DtmTrRefModel[1][2] = NoNANLim( 1.0F/PARM(DtmTrRth21Ref2),  1.0e6F, -1.0e6F );
    DtmTrRefModel[1][3] = NoNANLim( 1.0F/PARM(DtmTrRth1CRef2), 1.0e6F, -1.0e6F );
    
    if (SimAcclAct)
    {
        DtmTrRefModel[0][0] = NoNANLim( INS(DelTm3)/PARM(DtmTrCnd2), 1.0e6F, -1.0e6F )*PARM(DtmTr0AcclSim);
        DtmTrRefModel[0][1] = NoNANLim( INS(DelTm3)/PARM(DtmTrCnd1), 1.0e6F, -1.0e6F )*PARM(DtmTr1AcclSim);
        DtmTrRefModel[0][2] = NoNANLim( 1.0F/PARM(DtmTrRthNd2Nd1),  1.0e6F, -1.0e6F )*PARM(DtmTr2AcclSim);
        DtmTrRefModel[0][3] = NoNANLim( 1.0F/PARM(DtmTrRthNd1Clnt), 1.0e6F, -1.0e6F )*PARM(DtmTr3AcclSim);
        DtmTrRefModel[1][0] = NoNANLim( INS(DelTm3)/PARM(DtmTrCnd2Ref2), 1.0e6F, -1.0e6F )*PARM(DtmTr0AcclSim);
        DtmTrRefModel[1][1] = NoNANLim( INS(DelTm3)/PARM(DtmTrCnd1Ref2), 1.0e6F, -1.0e6F )*PARM(DtmTr1AcclSim);
        DtmTrRefModel[1][2] = NoNANLim( 1.0F/PARM(DtmTrRth21Ref2),  1.0e6F, -1.0e6F )*PARM(DtmTr2AcclSim);
        DtmTrRefModel[1][3] = NoNANLim( 1.0F/PARM(DtmTrRth1CRef2), 1.0e6F, -1.0e6F )*PARM(DtmTr3AcclSim);
    }

    return (Status);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function estimates the DB current using a R-L series model of
//      the DB resistor. Also DB semiconductor junction temperature is
//      calculated and related diagnostic are implemented. Junction tempe-
//      rature is the fourth node of a four-pole in tandem RC network.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbThrmModel::Task1( void )
{
    if ( PARM(DtmEnb))
    {
        DtmVdbComp = IN(Vdb) + ((IN(Vdb) - DtmVdbComp) * PARM(DtmVdbAdj));

        // --------------------------------------------------------------------
        // determine if the DB switch is on and use this to compute voltage applied across DB resistor
        DtmSwStZ1 = DtmSwSt;
        if ( DtmVdbComp >= PARM(DtmOnThrs) )
        {   // Db switch is on
            DtmVr        = IN(Vdc);
            OUT(DtmSwSt) = true;
            DtmVdbComp   = IN(Vdc);
        }
        else
        {  // Db switch is off
           DtmVr        = 0.0F;
           OUT(DtmSwSt) = false;
           DtmVdbComp   = 0.0F;
        }

        // --------------------------------------------------------------------
        // estimate DB resistor current
        DtmIo   = DtmIest;
        DtmIr   = DtmVr * DtmRrcp;
        DtmIdel = DtmIr - DtmIo;
        DtmIest = DtmIo + (DtmIdel * DtmEdtTau);

        //-------------------------------------
        // estimate integral of current squared

        DtmIsqrInt = (0.5F * INS(DbTauElec) * DtmIdel * DtmIdel * DtmE2DtTau) -
                        (2.0F * INS(DbTauElec) * DtmIr   * DtmIdel * DtmEdtTau)  +
                        (DtmIr * DtmIr * INS(DelTm1));

        // --------------------------------------------------------------------
        // estimate integral of current
        DtmIint   = (DtmIr * INS(DelTm1)) - (INS(DbTauElec) * DtmIdel * DtmEdtTau);

        // --------------------------------------------------------------------
        // resistor thermal model outputs
        DtmAvgIsqr = DtmIsqrInt * INS(DelTm1Rcp);
        DtmPavg    = DtmIsqrInt * INS(DelTm1Rcp) * INS(DbR);
        DtmIavg    = INS(DelTm1Rcp) * DtmIint;

        // --------------------------------------------------------------------
        // transistor losses
        if ( DtmSwStZ1 && !DtmSwSt)     //Detect falling edge
        {
            DtmSwLoss = IN(Vdc) * ( DtmTrSw0Gn + DtmIo * DtmTrSw1Gn +
                                        DtmIo * DtmIo * DtmTrSw2Gn );
        }
        else
        {
            DtmSwLoss = 0.0;
        }

        if ( DtmSwSt )
        {
            DtmTrCondPwr = PARM(DtmTrR) * DtmAvgIsqr + DtmIavg * PARM(DtmTrVo);
        }
        else
        {
            DtmTrCondPwr = 0.0F;
        }

        OUT(DtmTrPwrLoss) = DtmTrCondPwr + DtmSwLoss;
    }
    else
    {
        // Keep specific data memnbers at their initial values
        ThrmModelInit();
    }

    DtmVdbZ1 = IN(Vdb);     // Preserve DB voltage feedback for next pass

    // --------------------------------------------------------------------
    // four-pole transistor thermal model - 4th node calculation

    // junction temperature
    DtmTrJncTemp  = DtmTrJncTemp + DtmTrCjnGn * (DtmTrPwrLoss - DtmTrPwrJnNd3);
    DtmTrPwrJnNd3 = DtmRthJnNd3Rcp * ( DtmTrJncTemp - DtmTrNd3TempT1 );

    OUT(DtmTr_GridDiscReq) = false; //only need this set one pass
    if ( DtmTrJncTemp > PARM(DtmJncOtThr) )
    {
        if(!IN(Q1StatusVsqr))
        {
            PUSH_DIAG(DtmJncOtTrp);
            OUT(DtmTr_GridDiscReq)= true; //Disconnect from grid
        }
        else
        {
            PUSH_DIAG(DtmJncOtDiscAlm);
            OUT(DtmTr_GridDiscReq)= true; //Disconnect from grid
        }
    }

    if ( DtmTrJncTemp > PARM(DtmJncOtLockThr) )
    {
        PUSH_DIAG(DtmJncOtLockTrp);
        OUT(DtmOtLock) = true;
    }
    else if ( DtmClearOtLockT1 == true )
    {
        OUT(DtmOtLock) = false;
    }

    if (DtmTrJncTemp>PARM(DtmJncHotOnThr))
    {
        DtmJncHot=true;
    }
    else if (DtmTrJncTemp<PARM(DtmJncHotOffThr))
    {
        DtmJncHot=false;
    }

    DtmLinkVdc = IN(Vdc);       // copy Dc-link voltage for data-passing purposes

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function calculates temperature in some fictitious thermal node
//      connected to transistor junction by a thermal resistor. This is the
//      thrid node of a four-pole in tandem RC network.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbThrmModel::Task2( void )
{
    // --------------------------------------------------------------------
    // four-pole transistor thermal model - 3rd node calculation

    DtmTrNd3Temp   = DtmTrNd3Temp + DtmTrCnd3Gn  * (DtmTrPwrJnNd3T2 - DtmTrPwrNd3Nd2);
    DtmTrPwrNd3Nd2 = DtmRthNd3Nd2Rcp * ( DtmTrNd3Temp - DtmTrNd2TempT2 );

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function uses the estimate of DB current from task 1 to calculate
//      rate of energy dissipation in DB resistor. Diagnostics are generated if
//      DB dissipated power thresholds are exceeded. Also calculate temperature
//      in two fictitious nodes; these are the second and first nodes of a
//      four-pole in tandem RC network. Selectable reference temperature and
//      associated parameters are used as part of node calculations.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbThrmModel::Task3( void )
{
    // select temperature reference and model parameters
    // --------------------------------------------------------------------
    if ( (static_cast<RefTempeTyp> (IN(RefTempAct))) == DB_REF_TEMP_2 )
    {
        DtmRefTemp = IN(RefTemp2);
    }
    else
    {
        DtmRefTemp = IN(RefTemp1);      // Default temperature reference
    }

    // --------------------------------------------------------------------
    // four-pole transistor thermal model - 2nd and 1st node calculations
    // slow filter using double math
    DtmTrNd2TempDbl += (static_cast<double> (DtmTrRefModel[IN(RefTempAct)][0])) *
                       (static_cast<double> (DtmTrPwrNd3Nd2T3 - DtmTrPwrNd2Nd1));
    DtmTrNd2Temp     = static_cast<float> (DtmTrNd2TempDbl);
    DtmTrPwrNd2Nd1   = DtmTrRefModel[IN(RefTempAct)][2] * (DtmTrNd2Temp - DtmTrNd1Temp);

    DtmTrNd1TempDbl += (static_cast<double> (DtmTrRefModel[IN(RefTempAct)][1])) *
                       (static_cast<double> (DtmTrPwrNd2Nd1 - DtmTrPwrNd1Clnt));
    DtmTrNd1Temp     = static_cast<float> (DtmTrNd1TempDbl);
    DtmTrPwrNd1Clnt  = DtmTrRefModel[IN(RefTempAct)][3] * (DtmTrNd1Temp - DtmRefTemp);

    //------------------------------------------------------------------------
    // DB power dissipation model (single time constant lag)
    OUT(DtmPdisspt) = DtmPdisspt + (INS(DelTm3) * PARM(DtmW) * (DtmPavgT3 - DtmPdisspt));

    //------------------------------------------------------------------------
    // DB termperature rise
    OUT(DtmTmpRise) = DtmPdisspt * INS(BrgDbRthrm);

    //------------------------------------------------------------------------
    // DB overload alarm
    if ( DtmPdisspt >= PARM(DtmOvldOnThrs) )
    {
        OUT(DtmOverload) = True;
    }
    if ( DtmPdisspt <= PARM(DtmOvldOffThrs) )
    {
        OUT(DtmOverload)    = false;
        OUT(DtmDsbOverload) = false;
        OUT(DtmTrpOverload) = false;    // reset trip boolean when temperature below alarm threshold
    }
    if ( DtmOverload )
    {
        PUSH_DIAG(DtmOvldAlm);  
    }
    //------------------------------------------------------------------------
    // DB overload trip
    if ( DtmPdisspt >= PARM(DtmOvldTrpThrs) )
    {
        if(!IN(Q1StatusVsqr))
        {
            PUSH_DIAG(DtmOvldTrp);
            OUT(DtmRes_GridDiscReq)= true;
        }
        else
        {
            PUSH_DIAG(DtmOvldDiscAlm);
            OUT(DtmRes_GridDiscReq)= true;
        }
    }
    else
    {
        OUT(DtmRes_GridDiscReq)= false;
    }

    //------------------------------------------------------------------------
    // disable the db if rate of energy dissipation ever gets this high!
    if ( DtmPdisspt >= PARM(DtmOvldDsbThrs) )
    {
       PUSH_DIAG(DtmOvldDsbTrp);
       OUT(DtmTrpOverload) = true;
       OUT(DtmDsbOverload) = true;
    }

    //------------------------------------------------------------------------
    //Clearing of transistor lock-on indication
    if ( DtmOtLockT3 && (DtmLinkVdcT3 < PARM(DtmOtVdcUnlckThr)) && IN(ConvDiscnnted) )
    {
        DtmClearOtLock = true;
    }
    else
    {
        DtmClearOtLock = false;
    }

    //------------------------------------------------------------------------
    // DB thermal settling detection
    if ( PARM(DtmEnb) )
    {
        DtmTrJncRefDelta = DtmTrJncTempT3 - DtmRefTemp;
    }
    else
    {
        DtmTrJncRefDelta = 0.0;
    }

    if ( (ABS(DtmTrJncRefDelta) < PARM(DtmTrLqdOtThr)) || IN(DiagMode) || !PARM(DtmEnb) )
    {
        OUT(DtmTrTmpRdy) = true;
    }
    else if ( IN(Stopped) )
    {
        OUT(DtmTrTmpRdy) = false;
    }

    //------------------------------------------------------------------------
    // monitoring of deviation between junction and temperature reference
    if (  DtmTrJncRefDelta > PARM(DtmCoolOnThr) )
    {
        OUT(DtmCoolingReq) = true;        // Request cooling
    }
    else
    {
        OUT(DtmCoolingReq) = false;       // Do not request cooling
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
void cDbThrmModel::SimReadTemp(DtmTempSimTyp & target)
{
    target.DtmTrJncTemp = DtmTrJncTemp;
}