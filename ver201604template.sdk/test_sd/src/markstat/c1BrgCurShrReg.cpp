///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Bridge Current Sharing Regulator
//
// DESCRIPTION:
//      This file contains code for the bridge current sharing regulator.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      c0BrgCurShrReg.vsd
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
#include <math.h>
// core
// framework
#include "x0Vector.h"
// product
#include "c1BrgCurShrReg.h"


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
cBrgCurShrReg::cBrgCurShrReg
(
        InListTyp    InList,
        InsListTyp   InsList
)
        : In(InList), Ins(InsList)
{
    BrgCurShrRegPrecond();
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
cBrgCurShrReg::~cBrgCurShrReg ()
{
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters use in the current sharing function
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
int cBrgCurShrReg::RunRules()
{
    int Status = SUCCESS;

    //-------------------------------------------------------------------------
    // plant model parameters
    SET_FV(CshCmnRbrg, INS(CmnRbrg));
    SET_FV(CshCrsRbrg, INS(CrsRbrg));
    SET_FV(CshCmnLbrg, INS(CmnLbrg));
    SET_FV(CshCrsLbrg, INS(CrsLbrg));

    //-------------------------------------------------------------------------
    // bridge resistance and inductance reciprocicals
    CshCmnRbrgRcp = NoNANLim( 1.0F / PARM(CshCmnRbrg), 100.0e6, -100.0e6 );
    CshCrsRbrgRcp = NoNANLim( 1.0F / PARM(CshCrsRbrg), 100.0e6, -100.0e6 );
    CshCmnLbrgRcp = NoNANLim( 1.0F / PARM(CshCmnLbrg), 100.0e6, -100.0e6 );
    CshCrsLbrgRcp = NoNANLim( 1.0F / PARM(CshCrsLbrg), 100.0e6, -100.0e6 );

    SET_FV(CshCmnIfilW, PARM(CshCmnRbrg) * CshCmnLbrgRcp); // bridge current common mode filter (rad/s)
    SET_FV(CshCrsIfilW, PARM(CshCrsRbrg) * CshCrsLbrgRcp); // bridge current cross normal mode filter (rad/s)
    CshCmnIfilWdt = PARM(CshCmnIfilW) * INS(DelTm1);
    CshCrsIfilWdt = PARM(CshCrsIfilW) * INS(DelTm1);

    //-------------------------------------------------------------------------
    // regulator parameters
    {
        // common mode regulator bandwidth
        SET_FV(CshCmnRegBw,    2.0F * (float)PI * 100.0F);
        // common mode regulator damping factor
        SET_FV(CshCmnRegEsp,   1.0F);
        // common mode regulator proportional gain
        SET_FV(CshCmnRegPrpGn, 2.0F * PARM(CshCmnRegBw) * PARM(CshCmnRegEsp) * PARM(CshCmnLbrg));
        // common mode regulator integral gain
        SET_FV(CshCmnRegIntGn,  PARM(CshCmnRegBw) * PARM(CshCmnRegBw) * PARM(CshCmnLbrg));
        // common mode regulator integral gain times delta t
        CshCmnRegIntGnWdt = PARM(CshCmnRegIntGn) * INS(DelTm1);

        // cross normal mode regulator bandwidth
        SET_FV(CshCrsRegBw,    2.0F * (float)PI * 100.0F);
        // cross normal mode regulator damping factor
        SET_FV(CshCrsRegEsp,   1.0F);
        // cross normal mode regulator proportional gain
        SET_FV(CshCrsRegPrpGn, 2.0F * PARM(CshCrsRegBw) * PARM(CshCrsRegEsp) * PARM(CshCrsLbrg));
        // cross normal mode regulator integral gain
        SET_FV(CshCrsRegIntGn, PARM(CshCrsRegBw) * PARM(CshCrsRegBw) * PARM(CshCrsLbrg));
        // cross normal mode regulator integral gain times delta t
        CshCrsRegIntGnWdt = PARM(CshCrsRegIntGn) * INS(DelTm1);

        // sin-cos cross-coupling gain for the resonant controllers (between 0 and 1)
        SET_FV(CshResCcGn, 1.0F);

        // resonant controller bw speed correction parameter (0 to 1)
        // set to zero to disable the speed correction
        SET_FV(CshResGnWadj, 0.0F);
    }

    //-------------------------------------------------------------------------
    // limits
    // the following limits are calculated to provide enough voltage margin to ensure proper
    // current sharing with safety margins for the following extreme conditions:
    //      -> +-15% variation of saturation voltage on switching devices between bridges
    //      -> +-0.1 microseconds difference in lockout time between bridges
    //      -> +-10% impedance between parallel branches
    SET_FV(CshCmnIntDcLim,   1.0F);    // dc integral controller limit / common mode (volts)
    SET_FV(CshCmnIntResLim,  1.5F);    // resonant integral control limit / common mode (volts)
    SET_FV(CshCrsIntDcLim,   1.5F);    // dc integral controller limit / cross normal mode (volts)
    SET_FV(CshCrsIntResLim,  2.5F);    // resonant integral control limit / cross normal mode (volts)
    SET_FV(CshUDelRefLim,    0.015F);  // modulation correction command limit (in %) shouldn't be > 1-2%

    SET_FV(CshRegFlgAlmOn,   0.5F);    // reg limit flag persistance on level
    SET_FV(CshRegFlgAlmOff,  0.1F);    // reg limit flag persistance off level
    SET_FV(CshRegLimAlmTm,   1.0F);    // reg limit persistance alarm time

    //-------------------------------------------------------------------------
    // other parameters
#if 0
    SET_FV(CshLimFbkEnb,   false); // if false disable the dynamic feedback on the reg. ref. and keep reference to 0.0
#endif
    SET_FV(CshI0RefStep,   0.0F);  // for test : step on the common mode regulator reference
    SET_FV(CshIalRefStep,  0.0F);  // for test : step on the alpha regulator reference
    SET_FV(CshIbeRefStep,  0.0F);  // for test : step on the beta regulator reference

    // the HzRat must be a positive value
    CshRtrWRatRcp = NoNANLim( 1.0F / (2.0F * (float)PI * ABS(INS(HzRat))), 100.0e6F, -100.0e6F );

    return (Status);
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Precondition current sharing variables to zero all integrators
//      when regulators and bridges are disabled
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
void cBrgCurShrReg::BrgCurShrRegPrecond()
{
    CshI0Brg1             = 0.0F;
    CshIalBrg1            = 0.0F;
    CshIbeBrg1            = 0.0F;

    CshI0Brg2             = 0.0F;
    CshIalBrg2            = 0.0F;
    CshIbeBrg2            = 0.0F;

    CshI0Del              = 0.0F;
    CshIalDel             = 0.0F;
    CshIbeDel             = 0.0F;

    CshUaDelRes           = 0.0F;
    CshUbDelRes           = 0.0F;
    CshUcDelRes           = 0.0F;
#if 0
    CshVaDelRes           = 0.0F;
    CshVbDelRes           = 0.0F;
    CshVcDelRes           = 0.0F;

    CshV0DelRes           = 0.0F;
    CshValDelRes          = 0.0F;
    CshVbeDelRes          = 0.0F;
#endif
    CshI0DelRef           = 0.0F;
    CshIalDelRef          = 0.0F;
    CshIbeDelRef          = 0.0F;

    CshResGnW             = 0.0F;

    CshResCcGnSgn         = 0.0F;

    CshI0DelErr           = 0.0F;
    CshI0DelInt           = 0.0F;
    CshI0DelPrp           = 0.0F;
    CshI0DelRsnSinInt     = 0.0F;
    CshI0DelRsnCosInt     = 0.0F;
    CshI0DelRsnOut        = 0.0F;
    CshV0DelRef           = 0.0F;

    CshIalDelErr          = 0.0F;
    CshIalDelInt          = 0.0F;
    CshIalDelPrp          = 0.0F;
    CshIalDelRsnSinInt    = 0.0F;
    CshIalDelRsnCosInt    = 0.0F;
    CshIalDelRsnOut       = 0.0F;
    CshValDelRef          = 0.0F;

    CshIbeDelErr          = 0.0F;
    CshIbeDelInt          = 0.0F;
    CshIbeDelPrp          = 0.0F;
    CshIbeDelRsnSinInt    = 0.0F;
    CshIbeDelRsnCosInt    = 0.0F;
    CshIbeDelRsnOut       = 0.0F;
    CshVbeDelRef          = 0.0F;

    CshVaDelRef           = 0.0F;
    CshVbDelRef           = 0.0F;
    CshVcDelRef           = 0.0F;
    
    CshI0LimFlg           = 0.0F;
    CshIalLimFlg          = 0.0F;
    CshIbeLimFlg          = 0.0F;
    CshULimFlg            = 0.0F;

    OUT(CshUaDelRef)      = 0.0F;
    OUT(CshUbDelRef)      = 0.0F;
    OUT(CshUcDelRef)      = 0.0F;

    return;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This is the main current sharing function. It measures the difference
//      between the current measurements of 2 bridges and in a control loop
//      it calculates the volts correction needed to apply to the modulator to
//      compensate for the current difference
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE:
//
/////////////////////////////////////////////////////////////////////////////
void cBrgCurShrReg::Task1()
{
#if 0
    float VdcScal;
#endif
    float VdcRcpScal;

    // precondition or execute regulators
    //-----------------------------------
    if ( IN(RegEnb) == false )
    {
        BrgCurShrRegPrecond();
        return;
    }

    //-------------------------------------------------------------------------
    // calculate common mode currents of bridges 1 and 2
    CshI0Brg1 = ( IN(IaBrg1) + IN(IbBrg1) + IN(IcBrg1) ) * (1.0F/3.0F);
    CshI0Brg2 = ( IN(IaBrg2) + IN(IbBrg2) + IN(IcBrg2) ) * (1.0F/3.0F);

    //-------------------------------------------------------------------------
    // calculate alpha & beta current components of bridges 1 and 2
    cVectorf::ABCToAlphaBeta(IN(IaBrg1), IN(IbBrg1), IN(IcBrg1), &CshIalBrg1, &CshIbeBrg1);
    cVectorf::ABCToAlphaBeta(IN(IaBrg2), IN(IbBrg2), IN(IcBrg2), &CshIalBrg2, &CshIbeBrg2);

    //-------------------------------------------------------------------------
    // calculate current delta for each component
    CshI0Del  = 0.5F * ( CshI0Brg1  - CshI0Brg2  );
    CshIalDel = 0.5F * ( CshIalBrg1 - CshIalBrg2 );
    CshIbeDel = 0.5F * ( CshIbeBrg1 - CshIbeBrg2 );


    //-------------------------------------------------------------------------
    // build the "delta" reference for the regulator
#if 0
    if ( PARM(CshLimFbkEnb) )
    {
        // residual calculation is enabled
        // we calculate non-delivered voltages and apply them in the next loop calculation

        // rescale residuals to voltages
        VdcScal     = IN(VdcBrg1) * (float)SQRT3_RCP;
        CshVaDelRes = VdcScal * CshUaDelRes;
        CshVbDelRes = VdcScal * CshUbDelRes;
        CshVcDelRes = VdcScal * CshUcDelRes;

        // transform residuals in Alpha, Beta, Common components
        CshV0DelRes = ( CshVaDelRes + CshVbDelRes + CshVcDelRes ) * (1.0F/3.0F);
        cVectorf::ABCToAlphaBeta(CshVaDelRes, CshVbDelRes, CshVcDelRes, &CshValDelRes, &CshVbeDelRes);

        // filter to calculate current corrections to apply in the reference from voltages
        CshI0DelRef  += CshCmnIfilWdt * ( CshV0DelRes  * CshCmnRbrgRcp - CshI0DelRef);
        CshIalDelRef += CshCrsIfilWdt * ( CshValDelRes * CshCrsRbrgRcp - CshIalDelRef);
        CshIbeDelRef += CshCrsIfilWdt * ( CshVbeDelRes * CshCrsRbrgRcp - CshIbeDelRef);
    }
    else
#endif
    {
        // no residuals, set the current delta reference to 0.0
        CshI0DelRef  = 0.0F;
        CshIalDelRef = 0.0F;
        CshIbeDelRef = 0.0F;
    }

    //-------------------------------------------------------------------------
    // build error term for each component
    CshI0DelErr  = CshI0DelRef  - CshI0Del  + PARM(CshI0RefStep);
    CshIalDelErr = CshIalDelRef - CshIalDel + PARM(CshIalRefStep);
    CshIbeDelErr = CshIbeDelRef - CshIbeDel + PARM(CshIbeRefStep);

    //-------------------------------------------------------------------------
    // dynamic gain for the resonant controllers based on the rotor frequency CshResCcGn
    CshResGnW = (1 - PARM(CshResGnWadj)) + ( PARM(CshResGnWadj) * ABS(IN(Welec)) * CshRtrWRatRcp );
    if ( CshResGnW > 1.0F )
    {
        CshResGnW = 1.0F;
    }
    else if ( CshResGnW < 0.0F )
    {
        CshResGnW = 0.0F;
    }

    // resonant controller cross coupling gain
    if ( IN(Welec) > 0.0)
    {
        CshResCcGnSgn = PARM(CshResCcGn);
    }
    else
    {
        CshResCcGnSgn = -PARM(CshResCcGn);
    }

    //-------------------------------------------------------------------------
    // bridge COMMON MODE current control regulator
    {
        // pi controller
        {
            // integral term
            CshI0DelInt += CshI0DelErr * CshCmnRegIntGnWdt;
            // proportional term
            CshI0DelPrp = CshI0DelErr * PARM(CshCmnRegPrpGn);
        }

        // resonant controller
        {
            // common mode current resonant controller SIN part
            CshI0DelRsnSinInt += CshI0DelErr * (IN(SinCosFbk.sin) + CshResCcGnSgn*IN(SinCosFbk.cos)) * CshCmnRegIntGnWdt * CshResGnW;
            // common mode current resonant controller COS part
            CshI0DelRsnCosInt += CshI0DelErr * (IN(SinCosFbk.cos) - CshResCcGnSgn*IN(SinCosFbk.sin)) * CshCmnRegIntGnWdt * CshResGnW;
        }


        // limit integral only if the dynamic limits with residual are desactivated
        CshI0LimFlg = 0.0F;
#if 0
        if ( !(PARM(CshLimFbkEnb)) )
#endif
        {
            if ( CshI0DelInt >=  PARM(CshCmnIntDcLim) )
            {
                CshI0DelInt =  PARM(CshCmnIntDcLim);
                CshI0LimFlg = 1.0F;
            }
            if ( CshI0DelInt <= -PARM(CshCmnIntDcLim) )
            {
                CshI0DelInt = -PARM(CshCmnIntDcLim);
                CshI0LimFlg = 1.0F;
            }

            if ( CshI0DelRsnSinInt >=  PARM(CshCmnIntResLim) )
            {
                CshI0DelRsnSinInt =  PARM(CshCmnIntResLim);
                CshI0LimFlg = 1.0F;
            }
            if ( CshI0DelRsnSinInt <= -PARM(CshCmnIntResLim) )
            {
                CshI0DelRsnSinInt = -PARM(CshCmnIntResLim);
                CshI0LimFlg = 1.0F;
            }

            if ( CshI0DelRsnCosInt >=  PARM(CshCmnIntResLim) )
            {
                CshI0DelRsnCosInt =  PARM(CshCmnIntResLim);
                CshI0LimFlg = 1.0F;
            }
            if ( CshI0DelRsnCosInt <= -PARM(CshCmnIntResLim) )
            {
                CshI0DelRsnCosInt = -PARM(CshCmnIntResLim);
                CshI0LimFlg = 1.0F;
            }
        }


        // resonant control ouput
        CshI0DelRsnOut = ( CshI0DelRsnSinInt * IN(SinCosFbk.sin) ) +
                         ( CshI0DelRsnCosInt * IN(SinCosFbk.cos) );

        // common mode current control regulator output
        CshV0DelRef = CshI0DelPrp + CshI0DelInt + CshI0DelRsnOut;
    }

    //-------------------------------------------------------------------------
    // cross normal mode current control regulator : ALPHA component
    {
        // pi controller
        {
            // integral term
            CshIalDelInt += CshIalDelErr * CshCrsRegIntGnWdt;
            // proportional term
            CshIalDelPrp = CshIalDelErr * PARM(CshCrsRegPrpGn);
        }

        // resonant controller
        {
            // common mode current resonant controller SIN part
            CshIalDelRsnSinInt += CshIalDelErr * (IN(SinCosFbk.sin) + CshResCcGnSgn * IN(SinCosFbk.cos)) * CshCrsRegIntGnWdt * CshResGnW;
            // common mode current resonant controller COS part
            CshIalDelRsnCosInt += CshIalDelErr * (IN(SinCosFbk.cos) - CshResCcGnSgn * IN(SinCosFbk.sin)) * CshCrsRegIntGnWdt * CshResGnW;
        }

        // limit integral only if the dynamic limits with residual are desactivated
        CshIalLimFlg = 0.0F;
#if 0
        if ( !(PARM(CshLimFbkEnb)) )
#endif
        {
            if ( CshIalDelInt >=  PARM(CshCrsIntDcLim) )
            {
                CshIalDelInt =  PARM(CshCrsIntDcLim);
                CshIalLimFlg = 1.0F;
            }
            if ( CshIalDelInt <= -PARM(CshCrsIntDcLim) )
            {
                CshIalDelInt = -PARM(CshCrsIntDcLim);
                CshIalLimFlg = 1.0F;
            }

            if ( CshIalDelRsnSinInt >=  PARM(CshCrsIntResLim) )
            {
                CshIalDelRsnSinInt =  PARM(CshCrsIntResLim);
                CshIalLimFlg = 1.0F;
            }
            if ( CshIalDelRsnSinInt <= -PARM(CshCrsIntResLim) )
            {
                CshIalDelRsnSinInt = -PARM(CshCrsIntResLim);
                CshIalLimFlg = 1.0F;
            }

            if ( CshIalDelRsnCosInt >=  PARM(CshCrsIntResLim) )
            {
                CshIalDelRsnCosInt =  PARM(CshCrsIntResLim);
                CshIalLimFlg = 1.0F;
            }
            if ( CshIalDelRsnCosInt <= -PARM(CshCrsIntResLim) )
            {
                CshIalDelRsnCosInt = -PARM(CshCrsIntResLim);
                CshIalLimFlg = 1.0F;
            }
        }

        // resonant control ouput
        CshIalDelRsnOut = ( CshIalDelRsnSinInt * IN(SinCosFbk.sin) ) +
                          ( CshIalDelRsnCosInt * IN(SinCosFbk.cos) );

        // cross normal mode alpha component current control regulator output
        CshValDelRef = CshIalDelPrp + CshIalDelInt + CshIalDelRsnOut;
    }

    //-------------------------------------------------------------------------
    // cross normal mode current control regulator : BETA component
    {
        // pi controller
        {
            // integral term
            CshIbeDelInt += CshIbeDelErr * CshCrsRegIntGnWdt;

            // proportional term
            CshIbeDelPrp = CshIbeDelErr * PARM(CshCrsRegPrpGn);
        }

        // resonant controller
        {
            // common mode current resonant controller SIN part
            CshIbeDelRsnSinInt += CshIbeDelErr * (IN(SinCosFbk.sin) + CshResCcGnSgn * IN(SinCosFbk.cos)) * CshCrsRegIntGnWdt * CshResGnW;
            // common mode current resonant controller COS part
            CshIbeDelRsnCosInt += CshIbeDelErr * (IN(SinCosFbk.cos) - CshResCcGnSgn * IN(SinCosFbk.sin)) * CshCrsRegIntGnWdt * CshResGnW;
        }

        // limit integral only if the dynamic limits with residual are desactivated
        CshIbeLimFlg = 0.0F;
#if 0
        if ( !(PARM(CshLimFbkEnb)) )
#endif
        {
            if ( CshIbeDelInt >=  PARM(CshCrsIntDcLim) )
            {
                CshIbeDelInt =  PARM(CshCrsIntDcLim);
                CshIbeLimFlg = 1.0F;
            }
            if ( CshIbeDelInt <= -PARM(CshCrsIntDcLim) )
            {
                CshIbeDelInt = -PARM(CshCrsIntDcLim);
                CshIbeLimFlg = 1.0F;
            }

            if ( CshIbeDelRsnSinInt >=  PARM(CshCrsIntResLim) )
            {
                CshIbeDelRsnSinInt =  PARM(CshCrsIntResLim);
                CshIbeLimFlg = 1.0F;
            }
            if ( CshIbeDelRsnSinInt <= -PARM(CshCrsIntResLim) )
            {
                CshIbeDelRsnSinInt = -PARM(CshCrsIntResLim);
                CshIbeLimFlg = 1.0F;
            }

            if ( CshIbeDelRsnCosInt >=  PARM(CshCrsIntResLim) )
            {
                CshIbeDelRsnCosInt =  PARM(CshCrsIntResLim);
                CshIbeLimFlg = 1.0F;
            }
            if ( CshIbeDelRsnCosInt <= -PARM(CshCrsIntResLim) )
            {
                CshIbeDelRsnCosInt = -PARM(CshCrsIntResLim);
                CshIbeLimFlg = 1.0F;
            }
        }

        // resonant control ouput
        CshIbeDelRsnOut = ( CshIbeDelRsnSinInt * IN(SinCosFbk.sin) ) +
                          ( CshIbeDelRsnCosInt * IN(SinCosFbk.cos) );

        // cross normal mode alpha component current control regulator output
        CshVbeDelRef = CshIbeDelPrp + CshIbeDelInt + CshIbeDelRsnOut;
    }

    //-----------------------------------------------------------------------------
    // transform alpha beta into A B C
    cVectorf::AlphaBetaToABC(CshValDelRef, CshVbeDelRef, &CshVaDelRef, &CshVbDelRef, &CshVcDelRef);

    //-----------------------------------------------------------------------------
    // add common component
    CshVaDelRef += CshV0DelRef;
    CshVbDelRef += CshV0DelRef;
    CshVcDelRef += CshV0DelRef;

    //-----------------------------------------------------------------------------
    // scale delta voltages with the dc bus
    VdcRcpScal  = IN(VdcBrg1Rcp) * (float)SQRT3;
    OUT(CshUaDelRef) = CshVaDelRef * VdcRcpScal;
    OUT(CshUbDelRef) = CshVbDelRef * VdcRcpScal;
    OUT(CshUcDelRef) = CshVcDelRef * VdcRcpScal;

    //-----------------------------------------------------------------------------
    // limit the outputs
    CshULimFlg  = 0.0F;
    CshUaDelRes = 0.0F;
    if ( CshUaDelRef >= PARM(CshUDelRefLim) )
    {
        CshUaDelRes      = CshUaDelRef - PARM(CshUDelRefLim);
        OUT(CshUaDelRef) = PARM(CshUDelRefLim);
        CshULimFlg = 1.0F;
    }
    if ( CshUaDelRef <= -PARM(CshUDelRefLim) )
    {
        CshUaDelRes      = CshUaDelRef + PARM(CshUDelRefLim);
        OUT(CshUaDelRef) = -PARM(CshUDelRefLim);
        CshULimFlg = 1.0F;
    }

    CshUbDelRes = 0.0F;
    if ( CshUbDelRef >= PARM(CshUDelRefLim) )
    {
        CshUbDelRes      = CshUbDelRef - PARM(CshUDelRefLim);
        OUT(CshUbDelRef) = PARM(CshUDelRefLim);
        CshULimFlg = 1.0F;
    }
    if ( CshUbDelRef <= -PARM(CshUDelRefLim) )
    {
        CshUbDelRes      = CshUbDelRef + PARM(CshUDelRefLim);
        OUT(CshUbDelRef) = -PARM(CshUDelRefLim);
        CshULimFlg = 1.0F;
    }

    CshUcDelRes = 0.0F;
    if ( CshUcDelRef >= PARM(CshUDelRefLim) )
    {
        CshUcDelRes      = CshUcDelRef - PARM(CshUDelRefLim);
        OUT(CshUcDelRef) = PARM(CshUDelRefLim);
        CshULimFlg = 1.0F;
    }
    if ( CshUcDelRef <= -PARM(CshUDelRefLim) )
    {
        CshUcDelRes      = CshUcDelRef + PARM(CshUDelRefLim);
        OUT(CshUcDelRef) = -PARM(CshUDelRefLim);
        CshULimFlg = 1.0F;
    }

    return;
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function checks limit flags from the Task1 regulators and
//      asserts an alarm if any of them are persistant.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE:
//
/////////////////////////////////////////////////////////////////////////////
void cBrgCurShrReg::Task3()
{

    if (    (CshI0LimFlgT3  > PARM(CshRegFlgAlmOn))
         || (CshIalLimFlgT3 > PARM(CshRegFlgAlmOn))
         || (CshIbeLimFlgT3 > PARM(CshRegFlgAlmOn))
         || (CshULimFlgT3   > PARM(CshRegFlgAlmOn))
       )    
    {
        CshRegLimTmr += INS(DelTm3);
        if (CshRegLimTmr > PARM(CshRegLimAlmTm))
        {
            CshRegLimTmr = PARM(CshRegLimAlmTm);
            CshRegLimFlg = true;
        }         
    }
    if (    (CshI0LimFlgT3  < PARM(CshRegFlgAlmOff))
         && (CshIalLimFlgT3 < PARM(CshRegFlgAlmOff))
         && (CshIbeLimFlgT3 < PARM(CshRegFlgAlmOff))
         && (CshULimFlgT3   < PARM(CshRegFlgAlmOff))
       )    
    {
        CshRegLimTmr -= INS(DelTm3);
        if (CshRegLimTmr < 0.0F)
        {
            CshRegLimTmr = 0.0F;
            CshRegLimFlg = false;
        }         
    }
                    
    if (CshRegLimFlg)
    {
        PUSH_DIAG(CshRegLimAlm);
    }

    return;
}
