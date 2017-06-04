///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Bridge current and voltage feedback offsets
//
// DESCRIPTION:
//      This file contains member functions for the bridge feedback offset
//      calculation class.
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      b0BrgFbkOffs.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
#include <math.h>
#include <string.h>
// framework
// product
#include "b0BrgFbkOffs.h"


// Constants
//----------
#define  FIL_TAU_N  5

// abreviation for formal C++ casting  (move to x0FrameWork.h at some point?)
#define DOUBLE(in_flt)      static_cast<double>(in_flt)
#define FLOAT(in_dbl)       static_cast<float>(in_dbl)


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
//      Constructor for bridge feedback offset abstract class.
//      This function initalizes all data members.
//
// EXECUTION LEVEL
//      Class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cBrgFbkOffs::cBrgFbkOffs(void)
{
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Abstract class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cBrgFbkOffs::~cBrgFbkOffs(void)
{
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Constructor for rotor bridge feedback offset class.
//      This function initalizes all data members.
//
// EXECUTION LEVEL
//      Class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cRotorBrgFbkOffs::cRotorBrgFbkOffs
(
        BrgHdwOffsInTyp     InList,
        BrgHdwOffsInsTyp    InsList
)
        : In(InList), Ins(InsList)
{
    IOffsFltChk  = false;
    VOffsFltChk  = false;

    VOffsFilCnt  = 0;
    VOffsAccCnt  = 0;
    VOffsFilNT1  = 0;
    VOffsFilN    = 0;
    VOffsAccNT1  = 1;
    VOffsAccN    = 1;
    VOffsCntVld  = false;
    VOffsCntVldT2= false;
    IOffsGn      = 0.0;
    IOffsGnT1    = 0.0;

    VOffsGn      = 0.0;
    VOffsGnT1    = 0.0;

    VaxOffsFilSt = 0.0;
    VaxOffsFil   = 0.0;
    VaxOffsAcc   = 0.0;
    VaxOffsOut   = 0.0;
    VaxOffsOutT2 = 0.0;

    VbxOffsFilSt = 0.0;
    VbxOffsFil   = 0.0;
    VbxOffsAcc   = 0.0;
    VbxOffsOut   = 0.0;
    VbxOffsOutT2 = 0.0;

    VcxOffsFilSt = 0.0;
    VcxOffsFil   = 0.0;
    VcxOffsAcc   = 0.0;
    VcxOffsOut   = 0.0;
    VcxOffsOutT2 = 0.0;

    IaOffsFilSt  = 0.0;
    IaOffsOut    = 0.0;
    IaOffsOutT2  = 0.0;

    IbOffsFilSt  = 0.0;
    IbOffsOut    = 0.0;
    IbOffsOutT2  = 0.0;

    IcOffsFilSt  = 0.0;
    IcOffsOut    = 0.0;
    IcOffsOutT2  = 0.0;

    memset( (void*)&VOffsTm, 0, sizeof(VOffsTm) );
    IOffsSlwGn   = 0.0;
    IOffsFstGn   = 0.0;
    VOffsNrmGn   = 0.0;
    VOffsFstGn   = 0.0;
    VOffsNrmFilN = 0.0;
    VOffsFstFilN = 0.0;
    VOffsNrmAccN = 0.0;
    VOffsFstAccN = 0.0;
    OUT(VOffsVld)= false;

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Rotor bridge feedback offset class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cRotorBrgFbkOffs::~cRotorBrgFbkOffs(void)
{
        return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by Rotor bridge feedback offset class.
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cRotorBrgFbkOffs::RunRules (
        float   VFullScale,
        float   IFullScale
)
{
    if ( strncmp(INS(Uniquifier), "R", 1) == 0 )  //rotor side
    {
        SET_FV(WVOffsFil,  5.0F);
    }
    else
    {
        SET_FV(WVOffsFil,  0.01F);
    }
    SET_FV(WIOffsFst,  5.0);
    OUT(WIOffsFast) = PARM(WIOffsFst);
    SET_FV(WIOffsSlw,  0.5);
    SET_FV(WVOffsFst,  250.0);
    

    SET_FV(VOffsFstTm, 0.34);
    SET_FV(VOffsRnTm,  5.0);
    SET_FV(VOffsVlTm,  3600.0);

    // Calculate slow and fast current offset filter gains
    IOffsSlwGn = PARM(WIOffsSlw) * INS(DelTm1);
    IOffsFstGn = PARM(WIOffsFst) * INS(DelTm1);

    // Calculate normal and fast voltage offset pre-filter gains
    VOffsFstGn    = PARM(WVOffsFst) * INS(DelTm1);
    VOffsNrmGn    = PARM(WVOffsFil) * INS(DelTm1);

    // Calculate normal and fast number of Task 1 cycles to run voltage offset accumulator
    // No accumulation is provided, if integration time is less than zero
    VOffsFstAccN = (int)(PARM(VOffsFstTm) * INS(DelTm1Rcp) ) + 1;
    if ( VOffsFstAccN < 1 || INS(VOffsAccDsb) )
    {
        VOffsFstAccN = 1;
    }

    VOffsNrmAccN = (int)(PARM(VOffsRnTm) * INS(DelTm1Rcp) ) + 1;
    if ( VOffsNrmAccN < 1 || INS(VOffsAccDsb) )
    {
        VOffsNrmAccN = 1;
    }

    // Calculate normal and fast number of Task 1 cycles to run voltage offset prefilter
    // before activating accumulator - set to 5 times filter tau in units of Task 1 cycles
    // No prefilter is provided, if voltage-offset gain is less than zero
    VOffsFstFilN = FIL_TAU_N * (int)(1.0/(VOffsFstGn)) + 1;
    if ( (cNaNCheck::IsNAN(VOffsFstFilN)) || (VOffsFstFilN < 0) || INS(VOffsAccDsb) )
    {
        VOffsFstFilN = 0;
    }

    VOffsNrmFilN = FIL_TAU_N * (int)(1.0/(VOffsNrmGn)) + 1;
    if ( (cNaNCheck::IsNAN(VOffsNrmFilN)) || (VOffsNrmFilN < 0) || INS(VOffsAccDsb) )
    {
        VOffsNrmFilN = 0;
    }

    SET_FV(IaOffs, 0.0);
    SET_FV(IbOffs, 0.0);
    SET_FV(IcOffs, 0.0);

    SET_FV(IOffsThr, 0.04 * IFullScale);    // 4% of I full scale
    SET_FV(VOffsThr, 0.02 * VFullScale);    // 2% of V full scale

    SET_FV(VaxOffs, 0.0);
    SET_FV(VbxOffs, 0.0);
    SET_FV(VcxOffs, 0.0);

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the offline offset calculations for the
//      rotor current and voltage feedbacks.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cRotorBrgFbkOffs::Task1
(
        const BrgHdwOffsTyp&    SignalIn,
        BrgHdwOffsTyp&          OffsCalc
)
{
    //-------------------------------------------------------------------------
    // current offset nulling
    if  ( IN(IOffsNullEnb) )
    {
        // vco sampled currents
        IaOffsFilSt += DOUBLE(IOffsGnT1) * (DOUBLE(SignalIn.Ia) - DOUBLE(PARM(IaOffs)) - IaOffsFilSt);
        IbOffsFilSt += DOUBLE(IOffsGnT1) * (DOUBLE(SignalIn.Ib) - DOUBLE(PARM(IbOffs)) - IbOffsFilSt);
        IcOffsFilSt += DOUBLE(IOffsGnT1) * (DOUBLE(SignalIn.Ic) - DOUBLE(PARM(IcOffs)) - IcOffsFilSt);

        IaOffsOut = FLOAT(IaOffsFilSt);
        IbOffsOut = FLOAT(IbOffsFilSt);
        IcOffsOut = FLOAT(IcOffsFilSt);

        OffsCalc.Ia = IaOffsOut + PARM(IaOffs);
        OffsCalc.Ib = IbOffsOut + PARM(IbOffs);
        OffsCalc.Ic = IcOffsOut + PARM(IcOffs);
    }

    if ( IN(PwrDnFbkDsb) )
    {
        IaOffsFilSt = 0.0;
        IbOffsFilSt = 0.0;
        IcOffsFilSt = 0.0;

        IaOffsOut = 0.0;
        IbOffsOut = 0.0;
        IcOffsOut = 0.0;
    }

    //-------------------------------------------------------------------------
    // voltage offset nulling
    if ( IN(VOffsNullEnb) )
    {
        VaxOffsFilSt += DOUBLE(VOffsGnT1) * (DOUBLE(SignalIn.Vax) - DOUBLE(PARM(VaxOffs)) - VaxOffsFilSt);
        VbxOffsFilSt += DOUBLE(VOffsGnT1) * (DOUBLE(SignalIn.Vbx) - DOUBLE(PARM(VbxOffs)) - VbxOffsFilSt);
        VcxOffsFilSt += DOUBLE(VOffsGnT1) * (DOUBLE(SignalIn.Vcx) - DOUBLE(PARM(VcxOffs)) - VcxOffsFilSt);

        VaxOffsFil = FLOAT(VaxOffsFilSt);
        VbxOffsFil = FLOAT(VbxOffsFilSt);
        VcxOffsFil = FLOAT(VcxOffsFilSt);

        if ( VOffsFilCnt == VOffsFilNT1 )
        {
            VaxOffsAcc += VaxOffsFil;
            VbxOffsAcc += VbxOffsFil;
            VcxOffsAcc += VcxOffsFil;
            ++VOffsAccCnt;

            if ( VOffsAccCnt == VOffsAccNT1 )
            {
                VaxOffsOut = VaxOffsAcc / VOffsAccNT1;
                VbxOffsOut = VbxOffsAcc / VOffsAccNT1;
                VcxOffsOut = VcxOffsAcc / VOffsAccNT1;
                OffsCalc.Vax = VaxOffsOut + PARM(VaxOffs);
                OffsCalc.Vbx = VbxOffsOut + PARM(VbxOffs);
                OffsCalc.Vcx = VcxOffsOut + PARM(VcxOffs);
                VaxOffsAcc = 0.0;
                VbxOffsAcc = 0.0;
                VcxOffsAcc = 0.0;
                VOffsAccCnt = 0;
                VOffsCntVld = true;
                VOffsTm = INS(SysTm2);
            }
        }
        else
        {
            VaxOffsAcc = 0.0;
            VbxOffsAcc = 0.0;
            VcxOffsAcc = 0.0;
            VOffsAccCnt = 0;
            ++VOffsFilCnt;
        }
    }
    else
    {
        VaxOffsFilSt = 0.0;
        VbxOffsFilSt = 0.0;
        VcxOffsFilSt = 0.0;

        VaxOffsFil = 0.0;
        VbxOffsFil = 0.0;
        VcxOffsFil = 0.0;

        VaxOffsAcc = 0.0;
        VbxOffsAcc = 0.0;
        VcxOffsAcc = 0.0;

        VOffsAccCnt = 0;
        VOffsFilCnt = 0;

        if ( VOffsVld == false )    // Class output from Task2
        {
            VOffsCntVld = false;
        }
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function calculates the values of variables used in the
//      rotor offline feedback offset algorithm, and checks for
//      offset fault conditions.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cRotorBrgFbkOffs::Task2(void)
{

    // Calculate current offset filter gain.
    if ( IN(IOffsWSlowSw) )
    {
        IOffsGn = IOffsSlwGn;
    }
    else
    {
        IOffsGn = IOffsFstGn;
    }

    // Calculate voltage offset pre-filter gain and integration time.
    if ( IN(VOffsWFastSw) )
    {
        VOffsGn    = VOffsFstGn;
        VOffsAccN  = VOffsFstAccN;
        VOffsFilN  = VOffsFstFilN;
    }
    else
    {
        VOffsGn    = VOffsNrmGn;
        VOffsAccN  = VOffsNrmAccN;
        VOffsFilN  = VOffsNrmFilN;
    }

    // Check for current offset faults.
    if ( !IN(IOffsNullEnb) )
    {
        if ( !IOffsFltChk )
        {
            if ( ABS( IaOffsOutT2 + PARM(IaOffs) ) > PARM(IOffsThr) )
            {
                PUSH_DIAG( IaOffsTrp );
                IOffsFltChk = true;
            }
            if ( ABS( IbOffsOutT2 + PARM(IbOffs) ) > PARM(IOffsThr) )
            {
                PUSH_DIAG( IbOffsTrp );
                IOffsFltChk = true;
            }
            if ( ABS( IcOffsOutT2 + PARM(IcOffs) ) > PARM(IOffsThr) )
            {
                PUSH_DIAG( IcOffsTrp );
                IOffsFltChk = true;
            }
        }
    }
    else
    {
        IOffsFltChk = false;
    }

    OUT(VOffsVld) = VOffsCntVldT2;      // Transfer counter validity from Task1

    if ( !IN(VOffsNullEnb) )
    {
        // Check for voltage offset faults.
        if ( !VOffsFltChk )
        {
            if ( ABS( VaxOffsOutT2 + PARM(VaxOffs) ) > PARM(VOffsThr) )
            {
                PUSH_DIAG( VaxOffsTrp );
                VOffsFltChk   = true;
                OUT(VOffsVld) = false;
            }
            if ( ABS( VbxOffsOutT2 + PARM(VbxOffs) ) > PARM(VOffsThr) )
            {
                PUSH_DIAG( VbxOffsTrp );
                VOffsFltChk   = true;
                OUT(VOffsVld) = false;
            }
            if ( ABS( VcxOffsOutT2 + PARM(VcxOffs) ) > PARM(VOffsThr) )
            {
                PUSH_DIAG( VcxOffsTrp );
                VOffsFltChk   = true;
                OUT(VOffsVld) = false;
            }
        }
    }
    else
    {
        VOffsFltChk = false;
    }

    // Declare voltage offset invalid when specified time expired.
    // Declare voltage offset invalid when in simulator mode.
    if ( Delta_Tm( INS(SysTm2), VOffsTm ) > PARM(VOffsVlTm) )
    {
        OUT(VOffsVld) = false;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Empty function for overriding virtual method in Rotor bridge
//      feedback offset class.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cRotorBrgFbkOffs::Task3(void)
{
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Constructor for line bridge feedback offset class.
//      This function initalizes all data members.
//
// EXECUTION LEVEL
//      Class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cLineBrgFbkOffs::cLineBrgFbkOffs
(
        BrgHdwOffsInTyp     InList,
        BrgHdwOffsInsTyp    InsList
)
        : In(InList), Ins(InsList)
{
    IaOffsInt    = 0.0;
    IbOffsInt    = 0.0;
    IcOffsInt    = 0.0;
    IaOffsDyn    = 0.0;
    IbOffsDyn    = 0.0;
    IcOffsDyn    = 0.0;
    VaxOffsInt   = 0.0;
    VbxOffsInt   = 0.0;
    VcxOffsInt   = 0.0;
    VaxOffsDyn   = 0.0;
    VbxOffsDyn   = 0.0;
    VcxOffsDyn   = 0.0;

    IOffsFilGn   = 0.0;
    VOffsFilGn   = 0.0;
    VOffsT2Gn    = 0.0;

    IaInpOffs    = 0.0;
    IaInpOffsT2  = 0.0;
    IbInpOffs    = 0.0;
    IbInpOffsT2  = 0.0;
    IcInpOffs    = 0.0;
    IcInpOffsT2  = 0.0;
    VaxInpOffs   = 0.0;
    VaxInpOffsT2 = 0.0;
    VbxInpOffs   = 0.0;
    VbxInpOffsT2 = 0.0;
    VcxInpOffs   = 0.0;
    VcxInpOffsT2 = 0.0;

    VaxOffsFil   = 0.0;
    VaxOffsFilT3 = 0.0;
    VbxOffsFil   = 0.0;
    VbxOffsFilT3 = 0.0;
    VcxOffsFil   = 0.0;
    VcxOffsFilT3 = 0.0;

    IaOutOffs    = 0.0;
    IaOutOffsT1  = 0.0;
    IbOutOffs    = 0.0;
    IbOutOffsT1  = 0.0;
    IcOutOffs    = 0.0;
    IcOutOffsT1  = 0.0;
    VaxOffsOut   = 0.0;
    VaxOffsOutT1 = 0.0;
    VbxOffsOut   = 0.0;
    VbxOffsOutT1 = 0.0;
    VcxOffsOut   = 0.0;
    VcxOffsOutT1 = 0.0;

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Line bridge feedback offset class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cLineBrgFbkOffs::~cLineBrgFbkOffs(void)
{
        return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by Line bridge feedback offset class.
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cLineBrgFbkOffs::RunRules (
        float   VFullScale,
        float   IFullScale
)
{
    SET_FV(IaOffs, 0.0);
    SET_FV(IbOffs, 0.0);
    SET_FV(IcOffs, 0.0);
    SET_FV(IOffsThr, 0.04 * IFullScale);    // 4% of V full scale
    SET_FV(VOffsThr, 0.02 * VFullScale);    // 2% of V full scale
    SET_FV(VaxOffs, 0.0);
    SET_FV(VbxOffs, 0.0);
    SET_FV(VcxOffs, 0.0);
    
    if ( strncmp(INS(Uniquifier), "L", 1) == 0 )  //rotor side
    {
        SET_FV(WIOffsFil, 5.0);
    }
    else
    {
        SET_FV(WIOffsFil, 1.0);
    }
    IOffsFilGn = INS(DelTm2) * PARM(WIOffsFil);

    SET_FV(WVOffsT2,  1.0);
    VOffsT2Gn  = INS(DelTm2) * PARM(WVOffsT2);

    SET_FV(WVOffsFil, 0.01);
    VOffsFilGn = INS(DelTm3) * PARM(WVOffsFil);

    SET_FV(VOffsInh, false);
    SET_FV(IOffsInh, false);

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Transfer input feedback signals to members of the Line bridge
//      feedback offset class.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cLineBrgFbkOffs::Task1
(
        const BrgHdwOffsTyp&    SignalIn,
        BrgHdwOffsTyp&          OffsCalc
)
{
    // Following input signals are going to be data-passed to slower tasks
    IaInpOffs  = SignalIn.Ia  - IaOutOffsT1;
    IbInpOffs  = SignalIn.Ib  - IbOutOffsT1;
    IcInpOffs  = SignalIn.Ic  - IcOutOffsT1;
    VaxInpOffs = SignalIn.Vax - VaxOffsOutT1;
    VbxInpOffs = SignalIn.Vbx - VbxOffsOutT1;
    VcxInpOffs = SignalIn.Vcx - VcxOffsOutT1;

    // Folloing output signals were data-passed from slower tasks
    OffsCalc.Ia  = IaOutOffsT1;
    OffsCalc.Ib  = IbOutOffsT1;
    OffsCalc.Ic  = IcOutOffsT1;
    OffsCalc.Vax = VaxOffsOutT1;
    OffsCalc.Vbx = VbxOffsOutT1;
    OffsCalc.Vcx = VcxOffsOutT1;

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the offline offset calculations for the
//      line current feedbacks.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cLineBrgFbkOffs::Task2(void)
{
    float IaIntZ1 = 0.0F;
    float IbIntZ1 = 0.0F;
    float IcIntZ1 = 0.0F;

    if ( IN(IOffsNullEnb) )
    {
        IaIntZ1   = IaOffsInt;
        IbIntZ1   = IbOffsInt;
        IcIntZ1   = IcOffsInt;

        IaOffsInt = IaOffsInt + IaInpOffsT2; //These integrators are set to the
        IbOffsInt = IbOffsInt + IbInpOffsT2; //xx_Int_Z1 value below if the
        IcOffsInt = IcOffsInt + IcInpOffsT2; //corresponding offset is in clamp.


        IaOffsDyn = IaOffsInt * IOffsFilGn;
        IbOffsDyn = IbOffsInt * IOffsFilGn;
        IcOffsDyn = IcOffsInt * IOffsFilGn;
    }

    if ( PARM(IOffsInh) || IN(PwrDnFbkDsb) )
    {
        IaOffsDyn = 0.0F;
        IbOffsDyn = 0.0F;
        IcOffsDyn = 0.0F;
        IaOffsInt = 0.0F;
        IbOffsInt = 0.0F;
        IcOffsInt = 0.0F;
    }

    IaOutOffs = IaOffsDyn + PARM(IaOffs);
    if ( IaOutOffs >= PARM(IOffsThr) )
    {
         IaOutOffs = PARM(IOffsThr);
         PUSH_DIAG(IaOffsTrp);
         IaOffsInt = IaIntZ1;   //Freeze integrator at old level if in clamp
    }
    else if ( IaOutOffs <= -PARM(IOffsThr) )
    {
         IaOutOffs = -PARM(IOffsThr);
         PUSH_DIAG(IaOffsTrp);
         IaOffsInt = IaIntZ1;   //Freeze integrator at old level if in clamp
    }

    IbOutOffs = IbOffsDyn + PARM(IbOffs);
    if ( IbOutOffs >= PARM(IOffsThr) )
    {
         IbOutOffs = PARM(IOffsThr);
         PUSH_DIAG(IbOffsTrp);
         IbOffsInt = IbIntZ1;   //Freeze integrator at old level if in clamp
    }
    else if ( IbOutOffs <= -PARM(IOffsThr) )
    {
         IbOutOffs = -PARM(IOffsThr);
         PUSH_DIAG(IbOffsTrp);
         IbOffsInt = IbIntZ1;   //Freeze integrator at old level if in clamp
    }

    IcOutOffs = IcOffsDyn + PARM(IcOffs);
    if ( IcOutOffs >= PARM(IOffsThr) )
    {
         IcOutOffs = PARM(IOffsThr);
         PUSH_DIAG(IcOffsTrp);
         IcOffsInt = IcIntZ1;   //Freeze integrator at old level if in clamp
    }
    else if (IcOutOffs <= -PARM(IOffsThr))
    {
         IcOutOffs = -PARM(IOffsThr);
         PUSH_DIAG(IcOffsTrp);
         IcOffsInt = IcIntZ1;   //Freeze integrator at old level if in clamp
    }

    // Filtered voltages for offset calculation in a slower task
    VaxOffsFil += VOffsT2Gn * (VaxInpOffsT2 - VaxOffsFil);
    VbxOffsFil += VOffsT2Gn * (VbxInpOffsT2 - VbxOffsFil);
    VcxOffsFil += VOffsT2Gn * (VcxInpOffsT2 - VcxOffsFil);

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the dynamic offset calculations for the
//      line current feedbacks.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cLineBrgFbkOffs::Task3(void)
{
    float VaxOffsAbs;
    float VbxOffsAbs;
    float VcxOffsAbs;

    // Dynamic voltage offset Task3 function
    if ( IN(InnrRegEnb) )
    {
        VaxOffsInt = VaxOffsInt + VaxOffsFilT3;
        VbxOffsInt = VbxOffsInt + VbxOffsFilT3;
        VcxOffsInt = VcxOffsInt + VcxOffsFilT3;

        VaxOffsDyn = VOffsFilGn * VaxOffsInt;
        VbxOffsDyn = VOffsFilGn * VbxOffsInt;
        VcxOffsDyn = VOffsFilGn * VcxOffsInt;
    }

    if ( PARM(VOffsInh) )
    {
        VaxOffsDyn = 0.0F;
        VbxOffsDyn = 0.0F;
        VcxOffsDyn = 0.0F;
        VaxOffsInt = 0.0F;
        VbxOffsInt = 0.0F;
        VcxOffsInt = 0.0F;
    }

    VaxOffsOut = VaxOffsDyn + PARM(VaxOffs);
    VbxOffsOut = VbxOffsDyn + PARM(VbxOffs);
    VcxOffsOut = VcxOffsDyn + PARM(VcxOffs);

    VaxOffsAbs = ABS(VaxOffsOut);
    if ( VaxOffsAbs >= PARM(VOffsThr) )
    {
        PUSH_DIAG(VaxOffsTrp);
    }

    VbxOffsAbs = ABS(VbxOffsOut);
    if ( VbxOffsAbs >= PARM(VOffsThr) )
    {
        PUSH_DIAG(VbxOffsTrp);
    }

    VcxOffsAbs = ABS(VcxOffsOut);
    if ( VcxOffsAbs >= PARM(VOffsThr) )
    {
        PUSH_DIAG(VcxOffsTrp);
    }

    return;
}
