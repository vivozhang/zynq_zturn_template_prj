///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Low frequency leg test mode
//
// DESCRIPTION:
//      This file defines the leg test class for low frerquency operation
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      c1LegTestLowFrq.vsd, c1LegTestLowFrq.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// framework
#include "x0Vector.h"
// product
#include "c1LegTestLowFrq.h"


// Constants
//----------

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
//      Constructor for low frequency leg test class.
//      This function initalizes all data members.
//
// EXECUTION LEVEL
//      Class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cLegTestLowFrq::cLegTestLowFrq
(
        InListTyp   InList,
        InsListTyp  InsList
)
        : In(InList), Ins(InsList)
{
    #ifdef LEG_TEST
    Precondition();
    #endif

    IxIntDtGn            = 0.0F;
    IyIntDtGn            = 0.0F;
    IregYeqvL            = 0.0F;
    IyErrIntSqr          = 0.0F;
    IyErrIntSqrT3        = 0.0F;
    IyErrIntSqrFil       = 0.0F;
    IyErrIntFil          = 0.0F;
    IyErrIntTmr          = 0.0F;
    IyErrIntGn           = 0.0F;
    CurCmdLegTstLf       = 0.0F;
    CurCmdLegTstLfT3     = 0.0F;
    AngCmdLegTstLf       = 0.0F;
    FrqCmdLegTstLf       = 0.0F;
    AngElec              = 0.0F;
    AngElecZ1            = 0.0F;
    IxRegAlmFlag         = false;
    IyRegAlmFlag         = false;
    UxOut                = 0.0F;
    UyOut                = 0.0F;
    OUT(UxOutLegTstLf)   = 0.0F;
    OUT(UyOutLegTstLf)   = 0.0F;
    OUT(AngElecLegTstLf) = 0.0F;
    OUT(UcmOutLegTstLf)  = 0.0F;
    OUT(BrgEnbLegTstLf)  = false;
    OUT(RegStatLegTstLf) = false;
    OUT(AngIcmdLegTstLf) = 0.0F;

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Destructor for low frequency leg test class.
//      This function frees the resources used by the class instance.
//
// EXECUTION LEVEL
//      Instance destruction.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cLegTestLowFrq::~cLegTestLowFrq(void)
{
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by low frequency leg test class.
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
int cLegTestLowFrq::RunRules(void)
{
    int Status = SUCCESS;

    // Following rules are only valid for SEP, other cases require review !

    SET_FV(AngFrmLegTstLf, 30.0);  // Value that results on twice current in phB
    SET_FV(CurMaxLegTstLf, INS(CfgForSep) ? 17.0 : 1810.2);
    SET_FV(LlegTstLf, INS(LopnDelEqv));
    SET_FV(FrqMaxLegTstLf, 20.0);
    SET_FV(IxyRegBW, 1.0 / (4.0 * INS(DelTm1)));  // Nominal reg bandwidth is the reciprocal of 4 times Task-delta

    IregYeqvL = (1.0/3.0) * PARM(LlegTstLf);  // Line to neutral equivalent inductance

    SET_FV(IxPrpGnLegTstLf, 0.5 * PARM(IxyRegBW) * IregYeqvL);
    SET_FV(IyPrpGnLegTstLf, PARM(IxPrpGnLegTstLf));
    SET_FV(XintGnLegTstLf, (0.5 * PARM(IxyRegBW)) * (0.5 * PARM(IxyRegBW)) * IregYeqvL);
    SET_FV(YintGnLegTstLf, PARM(XintGnLegTstLf));

    IxIntDtGn = INS(DelTm1) * PARM(XintGnLegTstLf);
    IyIntDtGn = INS(DelTm1) * PARM(YintGnLegTstLf);

    SET_FV(IxRegLimLegTstLf, INS(CfgForSep) ? 150.0 : 500.0);
    SET_FV(IyRegLimLegTstLf, PARM(IxRegLimLegTstLf));
    SET_FV(IxRegHysLegTstLf, INS(CfgForSep) ? 135.0 : 450.0);
    SET_FV(IyRegHysLegTstLf, PARM(IxRegHysLegTstLf));

    SET_FV(IyErrLegTstLfW, 1.00);
    IyErrIntGn = INS(DelTm3) * PARM(IyErrLegTstLfW);
    SET_FV(IcmdPctLegTstLf, 0.5);
    SET_FV(ErrMaxTmLegTstLf, 1.0);
    SET_FV(SetpMinLegTstLf, INS(CfgForSep) ? 2.0 : 11.7);

    return (Status);
}

//#ifdef LEG_TEST
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs regulation loops for low frequency leg test.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cLegTestLowFrq::Task1(void)
{

    // Condition current set point
    if ( PARM(IsetpLegTstLf) > PARM(CurMaxLegTstLf) )
    {
        CurCmdLegTstLf = PARM(CurMaxLegTstLf);
        PUSH_DIAG( IlegTstMaxAlm );
    }
    else if ( PARM(IsetpLegTstLf) < -PARM(CurMaxLegTstLf) )
    {
        CurCmdLegTstLf = -PARM(CurMaxLegTstLf);
        PUSH_DIAG( IlegTstMaxAlm );
    }
    else
    {
        CurCmdLegTstLf = PARM(IsetpLegTstLf);
    }

    // Condition frequency set point
    if ( PARM(FrqSetpLegTstLf) > PARM(FrqMaxLegTstLf) )
    {
        FrqCmdLegTstLf = PARM(FrqMaxLegTstLf);
        PUSH_DIAG( FrqLegTstMaxAlm );
    }
    else if ( PARM(FrqSetpLegTstLf) < -PARM(FrqMaxLegTstLf) )
    {
        FrqCmdLegTstLf = -PARM(FrqMaxLegTstLf);
        PUSH_DIAG( FrqLegTstMaxAlm );
    }
    else
    {
        FrqCmdLegTstLf = PARM(FrqSetpLegTstLf);
    }

    // Electric angle set point for quantities in xy frame
    if ( IN(RegEnbReq) == true )
    {
        AngCmdLegTstLf = cVectorf::Modulo2Pi( AngCmdLegTstLf + FrqCmdLegTstLf * PI2 * INS(DelTm1) );
    }
    else
    {
        AngCmdLegTstLf = 0.0F;
    }

    // Calculate per-unit common mode command
    UcmOut = (SQRT3/2.0F) * PARM(VcmPuLegTstLf) * sinf(AngCmdLegTstLf + (PARM(AngVIlegTstLf)*DEG_TO_RAD));

    //---------------------------------------------------------------------
    // Call the xy current regulator
    IxyRegulator();

    // Calculate x-y per-unit control outputs - Prevent divide by zero
    if ( IN(VdcFbk) != 0.0F )
    {
        UxOut  = SQRT3 * IxRegNet / IN(VdcFbk);
        UyOut  = SQRT3 * IyRegNet / IN(VdcFbk);
    }
    else
    {
        UxOut  = 0.0F;
        UyOut  = 0.0F;
    }

    // Fixed angle between abc and xy frames (no relative rotation)
    AngElec    = cVectorf::Modulo2Pi( ( PARM(AngFrmLegTstLf) * DEG_TO_RAD + AngElecZ1 ) * 0.5F );
    AngElecZ1  = PARM(AngFrmLegTstLf) * DEG_TO_RAD;

    // Write outputs
    OUT(UxOutLegTstLf)   = UxOut;
    OUT(UyOutLegTstLf)   = UyOut;
    OUT(AngElecLegTstLf) = AngElec;
    OUT(UcmOutLegTstLf)  = UcmOut;

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function monitors the rms value of the integral Iy error. A fault
//      is pushed if such value remains above certain threshold for too long.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cLegTestLowFrq::Task3(void)
{
    IyErrIntSqrFil += IyErrIntGn * (IyErrIntSqrT3 - IyErrIntSqrFil);
    IyErrIntFil     = sqrtf(IyErrIntSqrFil);

    if ( (CurCmdLegTstLfT3 > PARM(SetpMinLegTstLf)) &&
         (IyErrIntFil > (PARM(IcmdPctLegTstLf) * CurCmdLegTstLfT3)) )
    {
        IyErrIntTmr += INS(DelTm3);
    }
    else
    {
        IyErrIntTmr = 0.0F;
    }

    if ( IyErrIntTmr > PARM(ErrMaxTmLegTstLf) )
    {
        PUSH_DIAG( IyErrLegTstTrp );    // Excessive Y-axis current error for too long!
    }

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the regulation of leg test x-y currents.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cLegTestLowFrq::IxyRegulator(void)
{
    if ( IN(RegEnbReq) == false )
    {
        Precondition();

        OUT(BrgEnbLegTstLf)  = false;   // Disable bridge gating
        OUT(RegStatLegTstLf) = false;   // Update regulator status

        return;
    }

    //-------------------------------------------------------------------------
    // Ix command delay scheme
    IxCmd   = 0.0F;        // Zero command in x-axis
    IxCmdZ3 = IxCmdZ2;
    IxCmdZ2 = IxCmdZ1;
    IxCmdZ1 = IxCmd;

    IxCmdZ = 0.5F*(IxCmdZ3 + IxCmdZ2);

    //-------------------------------------------------------------------------
    // Iy command delay scheme
    IyCmd   = CurCmdZ1 * sinf( AngCmdZ1 );   // Sinusoidal command generation in y-axis
    IyCmdZ3 = IyCmdZ2;
    IyCmdZ2 = IyCmdZ1;
    IyCmdZ1 = IyCmd;

    IyCmdZ = 0.5F*(IyCmdZ3 + IyCmdZ2);

    //-------------------------------------------------------------------------
    // Ixy feedback delay scheme
    IxFbkPrp = (IxFbkZ1 + IN(IxFbk)) * 0.5F;
    IxFbkZ1  = IN(IxFbk);

    IyFbkPrp = (IyFbkZ1 + IN(IyFbk)) * 0.5F;
    IyFbkZ1  = IN(IyFbk);

    //-------------------------------------------------------------------------
    // Ixy error calculation
    IxErrPrp = IxCmdZ1 - IxFbkPrp;
    IxErrInt = IxCmdZ - IN(IxFbk);

    IyErrPrp = IyCmdZ1 - IyFbkPrp;
    IyErrInt = IyCmdZ - IN(IyFbk);

    IyErrIntSqr = IyErrInt * IyErrInt;

    //-------------------------------------------------------------------------
    // Ix-axis regulator
    IxRegPrp = PARM(IxPrpGnLegTstLf) * IxErrPrp;     // Gain in units of ohm
    IxRegInt = IxRegInt + ( IxIntDtGn * IxErrInt );  // DtGain in units of ohm

    // Clamp integral part of regulator and alarm if needed
    if ( IxRegInt > PARM(IxRegLimLegTstLf) )
    {
        IxRegInt     = PARM(IxRegLimLegTstLf);
        IxRegAlmFlag = true;
    }
    else if ( IxRegInt < -PARM(IxRegLimLegTstLf) )
    {
        IxRegInt     = -PARM(IxRegLimLegTstLf);
        IxRegAlmFlag = true;
    }
    else if ( IxRegAlmFlag && (IxRegInt < PARM(IxRegHysLegTstLf)) && (IxRegInt > -PARM(IxRegHysLegTstLf)) )
    {
        IxRegAlmFlag = false;
    }

    if ( IxRegAlmFlag )
    {
        PUSH_DIAG( IxRegLegTstSatAlm );
    }

    IxRegSum = IxRegPrp + IxRegInt;

    //-------------------------------------------------------------------------
    // Y-axis regulator
    IyRegPrp = PARM(IyPrpGnLegTstLf) * IyErrPrp;     // Gain in units of ohm
    IyRegInt = IyRegInt + ( IyIntDtGn * IyErrInt );  // DtGain in units of ohm

    // Clamp integral part of regulator and alarm if needed
    if ( IyRegInt > PARM(IyRegLimLegTstLf) )
    {
        IyRegInt     = PARM(IyRegLimLegTstLf);
        IyRegAlmFlag = true;
    }
    else if ( IyRegInt < -PARM(IyRegLimLegTstLf) )
    {
        IyRegInt     = -PARM(IyRegLimLegTstLf);
        IyRegAlmFlag = true;
    }
    else if ( IyRegAlmFlag && (IyRegInt < PARM(IyRegHysLegTstLf)) && (IyRegInt > -PARM(IyRegHysLegTstLf)) )
    {
        IyRegAlmFlag = false;
    }

    if ( IyRegAlmFlag )
    {
        PUSH_DIAG( IyRegLegTstSatAlm );
    }

    IyRegSum = IyRegPrp + IyRegInt;

    //-------------------------------------------------------------------------
    // XY feed foward components
    IxRegFfd = 0.0F;
    IyRegFfd = CurCmdLegTstLf * IregYeqvL * FrqCmdLegTstLf * PI2 * cosf(AngCmdLegTstLf);

    //-------------------------------------------------------------------------
    // Net control loop output
    IxRegNet  = IxRegSum + IxRegFfd;
    IyRegNet  = IyRegSum + IyRegFfd;

    // Acknowledge active regulator status, enable bridge gating, and average
    // current command angle
    OUT(RegStatLegTstLf) = true;
    OUT(BrgEnbLegTstLf)  = true;
    OUT(AngIcmdLegTstLf) = 0.5F * (AngCmdLegTstLf + AngCmdZ1);

    // Retain values for next pass
    AngCmdZ1 = AngCmdLegTstLf;
    CurCmdZ1 = CurCmdLegTstLf;

    return;

}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function properly preset variables when regulator is not running.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cLegTestLowFrq::Precondition(void)
{
    IxCmd          = 0.0F;
    IxCmdZ1        = 0.0F;
    IxCmdZ2        = 0.0F;
    IxCmdZ3        = 0.0F;
    IxCmdZ         = 0.0F;
    IxErrPrp       = 0.0F;
    IxErrInt       = 0.0F;
    IxFbkPrp       = 0.0F;
    IxFbkZ1        = 0.0F;
    IxRegPrp       = 0.0F;
    IxRegInt       = 0.0F;

    CurCmdZ1       = 0.0F;
    AngCmdZ1       = 0.0F;

    IyCmd          = 0.0F;
    IyCmdZ1        = 0.0F;
    IyCmdZ2        = 0.0F;
    IyCmdZ3        = 0.0F;
    IyCmdZ         = 0.0F;
    IyErrPrp       = 0.0F;
    IyErrInt       = 0.0F;
    IyFbkPrp       = 0.0F;
    IyFbkZ1        = 0.0F;
    IyRegPrp       = 0.0F;
    IyRegInt       = 0.0F;

    IxRegSum       = 0.0F;
    IyRegSum       = 0.0F;
    IxRegFfd       = 0.0F;
    IyRegFfd       = 0.0F;
    IxRegNet       = 0.0F;
    IyRegNet       = 0.0F;

    IyErrIntSqrFil = 0.0F;
    IyErrIntSqr    = 0.0F;
    IyErrIntTmr    = 0.0F;

    UcmOut         = 0.0F;

    return;
}
//#endif
