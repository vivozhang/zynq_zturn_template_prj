///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Bridge Current Sharing Regulator
//
// DESCRIPTION:
//      This file contains definitions for the bridge current sharing regulator.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      c0BrgCurShrReg.vsd, c0BrgCurShrReg.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __C1_BRG_CUR_SHR_REG_H__
#define __C1_BRG_CUR_SHR_REG_H__

// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product

// Constants
//----------

// Function Prototypes
//--------------------

// Types and Classes
//------------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <class description>
//
///////////////////////////////////////////////////////////////////////////////
class cBrgCurShrReg
{

//-----------------------------------------------------------------------------
public:
    // Types
    //------
    struct InListTyp
    {
        const float&        IaBrg1;         //  1 - bridge 1 ph-A current (amps)
        const float&        IbBrg1;         //  2 - bridge 1 ph-B current (amps)
        const float&        IcBrg1;         //  3 - bridge 1 ph-C current (amps)
        const float&        IaBrg2;         //  4 - bridge 2 ph-A current (amps)
        const float&        IbBrg2;         //  5 - bridge 2 ph-B current (amps)
        const float&        IcBrg2;         //  6 - bridge 2 ph-C current (amps)
        const float&        VdcBrg1;        //  7 - dc bus voltage (volts)
        const float&        VdcBrg1Rcp;     //  8 - reciprocical of dc bus voltage (volts^-1)
        const float&        Welec;          //  9 - electrical frequency (rad/s)
        const SinCosTyp&    SinCosFbk;      // 10 - sincos electrical angle (rad)
        const unsigned&     RegEnb;         // 11 - enables/disables regulator
    };

    struct InsListTyp
    {
        const float&        DelTm1;         //  1 - task 1 time step (seconds)
        const float&        HzRat;          //  2 - electrical frequency rated (hz)
        const float&        CmnRbrg;        //  3 - dual bridge common mode resistance (ohms)
        const float&        CrsRbrg;        //  4 - dual bridge cross normal mode resistance (ohms)
        const float&        CmnLbrg;        //  5 - dual bridge common mode mode inductance (henries)
        const float&        CrsLbrg;        //  6 - dual bridge cross normal mode inductance (henries)
        const float&        DelTm3;         //  7 - task 3 time step (seconds)
    };

    // Methods
    //--------
    cBrgCurShrReg( InListTyp InList, InsListTyp InsList );
    ~cBrgCurShrReg();

   void Task1( void );
   void Task3( void );
   int  RunRules( void );


    // Output Signals
    //---------------
   CREATE_PUBVAR(CshUaDelRef,   float);     // scaled voltage ph-A delta reference (pu)
   CREATE_PUBVAR(CshUbDelRef,   float);     // scaled voltage ph-B delta reference (pu)
   CREATE_PUBVAR(CshUcDelRef,   float);     // scaled voltage ph-C delta reference (pu)


//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;

    // External Inputs
    //----------------
    InListTyp    In;
    InsListTyp   Ins;

    // Methods
    //--------
    void BrgCurShrRegPrecond();

    // Diagnostics
    //------------
    CREATE_DIAG(CshRegLimAlm);                    // regulator(s) in limit alarm

    // Parameters
    //-----------
    CREATE_PARM(CshCmnRbrg,           float);     // common mode bridge equivalent resistance (ohms)
    CREATE_PARM(CshCmnLbrg,           float);     // common mode bridge inductance (henries)
    CREATE_PARM(CshCmnRegBw,          float);     // common mode regulator bandwidth (hz)
    CREATE_PARM(CshCmnRegEsp,         float);     // common mode regulator damping factor (mho - conductance)
    CREATE_PARM(CshCmnRegPrpGn,       float);     // common mode regulator proportional gain
    CREATE_PARM(CshCmnRegIntGn,       float);     // common mode regulator integral gain
    CREATE_PARM(CshCmnIntDcLim,       float);     // common mode dc integral controller limit (volts)
    CREATE_PARM(CshCmnIntResLim,      float);     // common mode resonant integral control limit (volts)
    CREATE_PARM(CshCmnIfilW,          float);     // common mode bridge current filter (rad/s)

    CREATE_PARM(CshCrsRbrg,           float);     // cross normal mode bridge equivalent resistance (ohms)
    CREATE_PARM(CshCrsLbrg,           float);     // cross normal mode bridge inductance (henries)
    CREATE_PARM(CshCrsRegBw,          float);     // cross normal mode regulator bandwidth (hz)
    CREATE_PARM(CshCrsRegEsp,         float);     // cross normal mode regulator damping factor (mho - conductance)
    CREATE_PARM(CshCrsRegPrpGn,       float);     // cross normal mode regulator proportional gain
    CREATE_PARM(CshCrsRegIntGn,       float);     // cross normal mode regulator integral gain
    CREATE_PARM(CshCrsIntDcLim,       float);     // cross normal mode dc integral controller limit (volts)
    CREATE_PARM(CshCrsIntResLim,      float);     // cross normal mode resonant integral control limit (volts)
    CREATE_PARM(CshCrsIfilW,          float);     // cross normal mode bridge current filter (rad/s)

    CREATE_PARM(CshResCcGn,           float);     // sin-cos cross-coupling gain for the resonant controllers (between 0 and 1)

    CREATE_PARM(CshUDelRefLim,        float);     // modulation correction command limit (%) shouldn't be > 1-2%
#if 0
    CREATE_PARM(CshLimFbkEnb,         unsigned);  // if false disable the dynamic feedback on the reg. ref. and keep reference to 0.0
#endif

    CREATE_PARM(CshI0RefStep,         float);     // step on the common mode regulator reference (amps)
    CREATE_PARM(CshIalRefStep,        float);     // step on the alpha regulator reference (amps)
    CREATE_PARM(CshIbeRefStep,        float);     // step on the beta regulator reference (amps)

    CREATE_PARM(CshResGnWadj,         float);     // resonant controller bw speed correction parameter (0 to 1)

    CREATE_PARM(CshRegFlgAlmOn,       float);     // reg limit flag persistance on level
    CREATE_PARM(CshRegFlgAlmOff,      float);     // reg limit flag persistance off level
    CREATE_PARM(CshRegLimAlmTm,       float);     // reg limit persistance alarm time

    // Published Variables
    //--------------------
    CREATE_PUBVAR(CshI0Brg1,          float);     // common mode current bridge 1 (amps)
    CREATE_PUBVAR(CshIalBrg1,         float);     // Alpha component current bridge 1 (amps)
    CREATE_PUBVAR(CshIbeBrg1,         float);     // Beta component current bridge 1 (amps)

    CREATE_PUBVAR(CshI0Brg2,          float);     // common mode current bridge 2 (amps)
    CREATE_PUBVAR(CshIalBrg2,         float);     // Alpha component current bridge 2 (amps)
    CREATE_PUBVAR(CshIbeBrg2,         float);     // Beta component current bridge 2 (amps)

    CREATE_PUBVAR(CshI0Del,           float);     // common mode current difference between bridges (amps)
    CREATE_PUBVAR(CshIalDel,          float);     // Alpha current difference between bridges (amps)
    CREATE_PUBVAR(CshIbeDel,          float);     // Beta current difference between bridges (amps)

    CREATE_PUBVAR(CshUaDelRes,        float);     // scaled voltage ph-A delta residual (pu)
    CREATE_PUBVAR(CshUbDelRes,        float);     // scaled voltage ph-B delta residual (pu)
    CREATE_PUBVAR(CshUcDelRes,        float);     // scaled voltage ph-C delta residual (pu)
#if 0
    CREATE_PUBVAR(CshVaDelRes,        float);     // voltage ph-A delta residual (volts)
    CREATE_PUBVAR(CshVbDelRes,        float);     // voltage ph-B delta residual (volts)
    CREATE_PUBVAR(CshVcDelRes,        float);     // voltage ph-C delta residual (volts)

    CREATE_PUBVAR(CshV0DelRes,        float);     // voltage common mode delta residual (volts)
    CREATE_PUBVAR(CshValDelRes,       float);     // voltage Alpha component delta residual (volts)
    CREATE_PUBVAR(CshVbeDelRes,       float);     // voltage Beta component delta residual (volts)
#endif
    CREATE_PUBVAR(CshI0DelRef,        float);     // current common mode delta residual (amps)
    CREATE_PUBVAR(CshIalDelRef,       float);     // current Alpha component delta residual (amps)
    CREATE_PUBVAR(CshIbeDelRef,       float);     // current Beta component delta residual (amps)

    CREATE_PUBVAR(CshResGnW,          float);     // gain for the resonant regulators based on the rotor frequency, range: (0 - 1)

    CREATE_PUBVAR(CshI0DelErr,        float);     // common mode current reg. error (amps)
    CREATE_PUBVAR(CshI0DelInt,        float);     // common mode current reg. integral term
    CREATE_PUBVAR(CshI0DelPrp,        float);     // common mode current reg. integral term
    CREATE_PUBVAR(CshI0DelRsnSinInt,  float);     // common mode current resonant reg. sin
    CREATE_PUBVAR(CshI0DelRsnCosInt,  float);     // common mode current resonant reg. cos
    CREATE_PUBVAR(CshI0DelRsnOut,     float);     // common mode current resonant reg. output
    CREATE_PUBVAR(CshV0DelRef,        float);     // common mode reg. ouput reference (volts)

    CREATE_PUBVAR(CshIalDelErr,       float);     // Alpha component current reg. error (amps)
    CREATE_PUBVAR(CshIalDelInt,       float);     // Alpha component current reg. integral term
    CREATE_PUBVAR(CshIalDelPrp,       float);     // Alpha component current reg. integral term
    CREATE_PUBVAR(CshIalDelRsnSinInt, float);     // Alpha component current resonant reg. sin
    CREATE_PUBVAR(CshIalDelRsnCosInt, float);     // Alpha component current resonant reg. cos
    CREATE_PUBVAR(CshIalDelRsnOut,    float);     // Alpha component current resonant reg. output
    CREATE_PUBVAR(CshValDelRef,       float);     // Alpha component reg. ouput reference (volts)

    CREATE_PUBVAR(CshIbeDelErr,       float);     // Beta component current reg. error (amps)
    CREATE_PUBVAR(CshIbeDelInt,       float);     // Beta component current reg. integral term
    CREATE_PUBVAR(CshIbeDelPrp,       float);     // Beta component current reg. integral term
    CREATE_PUBVAR(CshIbeDelRsnSinInt, float);     // Beta component current resonant reg. sin
    CREATE_PUBVAR(CshIbeDelRsnCosInt, float);     // Beta component current resonant reg. cos
    CREATE_PUBVAR(CshIbeDelRsnOut,    float);     // Beta component current resonant reg. output
    CREATE_PUBVAR(CshVbeDelRef,       float);     // Beta component reg. ouput reference (volts)

    CREATE_PUBVAR(CshVaDelRef,        float);     // voltage ph-A delta reference (volts)
    CREATE_PUBVAR(CshVbDelRef,        float);     // voltage ph-B delta reference (volts)
    CREATE_PUBVAR(CshVcDelRef,        float);     // voltage ph-C delta reference (volts)

    CREATE_PUBVAR(CshI0LimFlg,        float);     // Regulator limit flag - Common mode
    CREATE_PUBVAR(CshIalLimFlg,       float);     // Regulator limit flag - Alpha
    CREATE_PUBVAR(CshIbeLimFlg,       float);     // Regulator limit flag - Beta
    CREATE_PUBVAR(CshULimFlg,         float);     // Regulator limit flag - U (modulation)
    
    CREATE_PUBVAR(CshI0LimFlgT3,      float);     // Regulator limit flag - Common mode    - Task3
    CREATE_PUBVAR(CshIalLimFlgT3,     float);     // Regulator limit flag - Alpha          - Task3
    CREATE_PUBVAR(CshIbeLimFlgT3,     float);     // Regulator limit flag - Beta           - Task3
    CREATE_PUBVAR(CshULimFlgT3,       float);     // Regulator limit flag - U (modulation) - Task3
    
    CREATE_PUBVAR(CshRegLimTmr,       float);     // Regulator limit alarm timer
    CREATE_PUBVAR(CshRegLimFlg,    unsigned);     // Regulator limit alarm flag 


    // Unpublished Variables
    //----------------------
    float   CshCmnRbrgRcp;                        // common mode bridge resistance reciprocical (Ohms^-1)
    float   CshCmnLbrgRcp;                        // common mode bridge inductance reciprocical (H^-1)
    float   CshCmnRegIntGnWdt;                    // common mode regulator integral gain times DelTm1
    float   CshCmnIfilWdt;                        // common mode bridge current filter times DelTm1

    float   CshCrsRbrgRcp;                        // cross normal mode bridge resistance reciprocical (Ohms^-1)
    float   CshCrsLbrgRcp;                        // cross normal mode bridge inductance reciprocical (H^-1)
    float   CshCrsRegIntGnWdt;                    // cross normal mode regulator integral gain times DelTm1
    float   CshCrsIfilWdt;                        // corss normal mode bridge current filter times DelTm1

    float   CshRtrWRatRcp;                        // reciprocical of rotor rated frequency (s/rad)

    float   CshResCcGnSgn;                        // sin-cos cross-coupling gain for the resonant controllers with sign adjusted in function of speed

    // Data Passing
    //-------------
    DATA_PASS(CshI0LimFlg,   CshI0LimFlgT3,   T1A_T3, FILTERED );  // Regulator limit flag - Common mode
    DATA_PASS(CshIalLimFlg,  CshIalLimFlgT3,  T1A_T3, FILTERED );  // Regulator limit flag - Alpha
    DATA_PASS(CshIbeLimFlg,  CshIbeLimFlgT3,  T1A_T3, FILTERED );  // Regulator limit flag - Beta
    DATA_PASS(CshULimFlg,    CshULimFlgT3,    T1A_T3, FILTERED );  // Regulator limit flag - U (modulation)

};


// Inline Function Definitions
//----------------------------

#endif  // __C1_BRG_CUR_SHR_REG_H__
