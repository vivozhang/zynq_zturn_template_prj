///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Low frequency leg test mode
//
// DESCRIPTION:
//      This file defines the low frequency leg test class.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      c1LegTestLowFrq.vsd, c1LegTestLowFrq.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef    LEGTEST_LOWFREQ_H
#define    LEGTEST_LOWFREQ_H

// Include Files
//--------------
// system
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
// DESCRIPTION:
//      This class implements a power bridge test mode consisting in spliting
//      a sinusoidal reference current between two phases, and having a third
//      phase carry the total sum. A common mode voltage is available for
//      recreating near normal (voltage/current) operation conditions. This
//      test mode has low frequency scope, since its current fundamental
//      frequency is limited to a maximum of 20 Hz.
//
///////////////////////////////////////////////////////////////////////////////
class cLegTestLowFrq
{

//-----------------------------------------------------------------------------
public:

   // Types and Classes
    //------------------
    struct InListTyp
    {
        const float&        IxFbk;          // 1 X-axis current feedback
        const float&        IyFbk;          // 2 Y-axis current feedback
        const float&        VdcFbk;         // 3 Dc link voltage feedback
        const unsigned&     RegEnbReq;      // 4 Regulator enable request
    };

    struct InsListTyp
    {
        const float&        LopnDelEqv;     // 1 Inductance between phases, open-delta equivalent (henry)
        const float&        DelTm1;         // 2 Task1 delta time
        const float&        DelTm3;         // 3 Task3 delta time
        const unsigned&     CfgForSep;      // 4 Configure parms for Sep operation
    };

    // Methods
    //--------
    cLegTestLowFrq( InListTyp InList, InsListTyp InsList );
    ~cLegTestLowFrq(void);
    void Task1(void);
    void Task3(void);
    int  RunRules(void);

    // Output Signals
    //---------------
    CREATE_PUBVAR(UxOutLegTstLf,   float);    // Pu voltage command in x-axis
    CREATE_PUBVAR(UyOutLegTstLf,   float);    // Pu voltage command in y-axis
    CREATE_PUBVAR(AngElecLegTstLf, float);    // Electric angle between xy and abc frames
    CREATE_PUBVAR(UcmOutLegTstLf,  float);    // Pu common-mode voltage command
    CREATE_PUBVAR(RegStatLegTstLf, unsigned); // Regulator enable status
    CREATE_PUBVAR(BrgEnbLegTstLf,  unsigned); // Enable bridge gating
    CREATE_PUBVAR(AngIcmdLegTstLf, float);    // Current angle command in xy frame (rad)

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cMarkStatDiag;
    friend class cDataPasser;

    // External Interfaces
    //--------------------
    InListTyp     In;
    InsListTyp    Ins;

    // Methods
    //--------
    void IxyRegulator(void);
    void Precondition(void);

    // Diagnostics
    //------------
    CREATE_DIAG(IxRegLegTstSatAlm); // X-axis current regulator has saturated
    CREATE_DIAG(IyRegLegTstSatAlm); // Y-axis current regulator has saturated
    CREATE_DIAG(IlegTstMaxAlm);     // Leg test current reference is surpassing its maximum
    CREATE_DIAG(FrqLegTstMaxAlm);   // Leg test frequency reference is surpassing its maximum
    CREATE_DIAG(IyErrLegTstTrp);    // Excessive Y-axis current error for too long

    // Parameters
    //-----------

    // User parameters
    CREATE_PARM(IsetpLegTstLf,    float);    // Magnitude to set current reference (peak amp)
    CREATE_PARM(FrqSetpLegTstLf,  float);    // Fundamental frequency set point (hz)
    CREATE_PARM(VcmPuLegTstLf,    float);    // Voltage common mode magnitude in pu of half link Vdc (0 to 1)
    CREATE_PARM(AngVIlegTstLf,    float);    // Phase shift angle between Iabc and Vabc (deg)
    // Final values
    CREATE_PARM(AngFrmLegTstLf,   float);    // Angle between xy and abc frames (deg)
    CREATE_PARM(CurMaxLegTstLf,   float);    // Maximum set point current (instantaneous amp)
    CREATE_PARM(FrqMaxLegTstLf,   float);    // Maximum allowed current frequency (hz)
    CREATE_PARM(LlegTstLf,        float);    // Inductance connected between phases, delta equivalent (henry)
    CREATE_PARM(IxyRegBW,         float);    // X-Y current regulator bandwidth (rad/sec)
    CREATE_PARM(IxPrpGnLegTstLf,  float);    // Proportional gain for Ix regulator (ohm)
    CREATE_PARM(IyPrpGnLegTstLf,  float);    // Proportional gain for Iy regulator (ohm)
    CREATE_PARM(XintGnLegTstLf,   float);    // Integral gain for Ix regulator (ohm/sec)
    CREATE_PARM(YintGnLegTstLf,   float);    // Integral gain for Iy regulator (ohm/sec)
    CREATE_PARM(IxRegLimLegTstLf, float);    // X-axis regulator alarm threshold (volt)
    CREATE_PARM(IxRegHysLegTstLf, float);    // X-axis regulator alarm hysteresis (volt)
    CREATE_PARM(IyRegLimLegTstLf, float);    // Y-axis regulator limit (volt)
    CREATE_PARM(IyRegHysLegTstLf, float);    // Y-axis regulator alarm hysteresis (volt)
    CREATE_PARM(IyErrLegTstLfW,   float);    // Omega to filter Y-axis current integral error (rad/sec)
    CREATE_PARM(IcmdPctLegTstLf,  float);    // Percentage of current command magnitude for error checking
    CREATE_PARM(ErrMaxTmLegTstLf, float);    // Max duration allowed for out of bounds current error (sec)
    CREATE_PARM(SetpMinLegTstLf,  float);    // Minimun current setpoint for which error checking is performed (peak amp)


    // Published Variables
    //--------------------
    CREATE_PUBVAR(IxCmd,          float);   // X-axis current command, 4 sample delay scheme (amp)
    CREATE_PUBVAR(IxCmdZ1,        float);   //
    CREATE_PUBVAR(IxCmdZ2,        float);   //
    CREATE_PUBVAR(IxCmdZ3,        float);   //
    CREATE_PUBVAR(IxCmdZ,         float);   // Average of last two x-axis current command samples (amp)
    CREATE_PUBVAR(IxErrPrp,       float);   // X-axis current proportional error (amp)
    CREATE_PUBVAR(IxErrInt,       float);   // X-axis current integral error (amp)
    CREATE_PUBVAR(IxFbkPrp,       float);   // Average of IxCmdZ and x-axis current feedback (amp)
    CREATE_PUBVAR(IxRegPrp,       float);   // Ix proportional regulator component (volt)
    CREATE_PUBVAR(IxRegInt,       float);   // Ix integral regulator component (volt)
    CREATE_PUBVAR(IxRegSum,       float);   // Ix total regulator contribution (volt)
    CREATE_PUBVAR(IxRegFfd,       float);   // Ix feed forward contribution (volt)
    CREATE_PUBVAR(IxRegNet,       float);   // Ix net control loop output (volt)
    CREATE_PUBVAR(IyCmd,          float);   // Y-axis current command, 4 sample delay scheme (amp)
    CREATE_PUBVAR(IyCmdZ1,        float);   //
    CREATE_PUBVAR(IyCmdZ2,        float);   //
    CREATE_PUBVAR(IyCmdZ3,        float);   //
    CREATE_PUBVAR(IyCmdZ,         float);   // Average of last two y-axis current command samples (amp)
    CREATE_PUBVAR(IyErrPrp,       float);   // Y-axis current proportional error (amp)
    CREATE_PUBVAR(IyErrInt,       float);   // Y-axis current integral error (amp)
    CREATE_PUBVAR(IyFbkPrp,       float);   // Average of IyCmdZ and y-axis current feedback (amp)
    CREATE_PUBVAR(IyRegPrp,       float);   // Iy proportional regulator component (volt)
    CREATE_PUBVAR(IyRegInt,       float);   // Iy integral regulator component (volt)
    CREATE_PUBVAR(IyRegSum,       float);   // Iy total regulator contribution (volt)
    CREATE_PUBVAR(IyRegFfd,       float);   // Iy feed forward contribution (volt)
    CREATE_PUBVAR(IyRegNet,       float);   // Iy net control loop output (volt)
    CREATE_PUBVAR(CurCmdLegTstLf, float);   // Sinusoidal current command magnitude (amp)
    CREATE_PUBVAR(AngCmdLegTstLf, float);   // Electric angle command in xy frame (rad)
    CREATE_PUBVAR(FrqCmdLegTstLf, float);   // Current fundamental frequency command (hz)
    CREATE_PUBVAR(IxRegAlmFlag,   unsigned);// X-axis regulator alarm flag
    CREATE_PUBVAR(IyRegAlmFlag,   unsigned);// Y-axis regulator alarm flag
    CREATE_PUBVAR(IyErrIntSqr,    float);   // Square Y-axis current integral error (amp^2)
    CREATE_PUBVAR(IyErrIntSqrT3,  float);   // Task3 version of IyErrIntSqr
    CREATE_PUBVAR(IyErrIntSqrFil, float);   // Filtered square Y-axis current integral error (amp^2)
    CREATE_PUBVAR(IyErrIntFil,    float);   // Square root of IyErrIntSqrFil (amp)
    CREATE_PUBVAR(IyErrIntTmr,    float);   // Timer tracking Y-axis current integral error (sec)

    // Unpublished Variables
    //----------------------
    float IxFbkZ1;          // X-axis current feedback, one task delayed (amp)
    float IyFbkZ1;          // Y-axis current feedback, one task delayed (amp)
    float UxOut;            // X-axis control output (pu)
    float UyOut;            // Y-axis control output (pu)
    float UcmOut;           // Common mode control output (pu)
    float AngElec;          // Electric angle between xy and abc frames (rad)
    float AngElecZ1;        // One task delayed version of AngElec (rad)
    float IregYeqvL;        // Current regulator line to neutral equivalent inductance (henry)
    float CurCmdZ1;         // One task delayed version of CurCmdLegTstLf (amp)
    float AngCmdZ1;         // One task delayed version of AngCmdLegTstLf (rad)
    float IxIntDtGn;        // Ix integral gain multiplied by Task delta time (ohm)
    float IyIntDtGn;        // Iy integral gain multiplied by Task delta time (ohm)
    float IyErrIntGn;       // Filter gain for Y-axis current integral error
    float CurCmdLegTstLfT3; // Task3 version of CurCmdLegTstLf (amp)

    // Data Passing
    //-------------
    DATA_PASS(IyErrIntSqr,    IyErrIntSqrT3,    T1_T3, FILTERED);
    DATA_PASS(CurCmdLegTstLf, CurCmdLegTstLfT3, T1_T3, FILTERED);

};

#endif  // LEGTEST_LOWFREQ_H
