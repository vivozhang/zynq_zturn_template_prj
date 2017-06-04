/*=============================================================================
* Product:     AccuWave Power Conversion Using MACC Control
* Host:        TMS320C31 DSP
* File:        P2Core.C30
* Language:    TI 'C'
* Copyright:   1996..1998 GE Motors and Industrial Systems, Salem VA
*
* Abstract: This file defines signals & procedures related to the
*           System pattern "Core Interface".
* -----------------------------------------------------------------------------
*
* Functions in module:
*   TorqueLimitSelect()   - Torque & current limit selection/steering
*   FluxAdjustSelect()    - Flux adjust selection/steering
*
*==============================================================================
* 980427 RAS Create from P2SpdTrq.c30 (980421..980427),
*            created from P1/A0spdreg.c30 (961120..980416)
* 980427 RAS Create FluxAdjustSelect from A0FlxCmd.c30
* 980428 RAS FluxAdjust-->FluxAdjustSelect; call from task3; call from task2
* 980428 RAS init adjusts to 1.0
* 981006 DGF  UNUSED_FLOAT_PTR -> LADR_Unused_Float
* 020927 DS  modify for doubly-fed
* 030204 SAB Branch from 'rotor' and include in 'combo'
* 030205 SAB Expose functions and add 'Rotor' prefix to functions
* 030829 DS  Add VARCmd
* 060222 SAB Conditionally calculate VARLim based on LineRunOnly
* 061201 SAB Only allow VarModeCmd to be changed by parameter if not in
*            local mode.
* 070224 SAB Set VarModeCmd to normally 'true' instead of 'false'
* 070224 SAB Enforce previously attempted change after Init1Sec
* 070327 IB  Remove assertion of VARModeCmd based on its var-mode pointer.
* 071114 SAB Add new function, RotorTorqueInputT2
* 071212 SAB Make adjustments for implementation of "One pll" scheme
* 080521 SAB Remove un-used code
*______________________________________________________________________________
* Last Mod: May 21, 2008 7:57:30AM
*/

/* Include files */
#include "x0FrameWork.h"
#include "Product.h"
#include "p1Core.h"

//extern
extern float    DelTm2;
extern float    DelTm2Rcp;
extern float    R_TrqCmd;
extern float    R_TrqCmdIn;
extern float    R_TrqRat;
extern unsigned L_LTestIp;
extern unsigned R_Running;
extern unsigned R_TrefEnbReq;
extern float    R_TrqCmdAct;
extern float    LclPfRefx;
extern float    PllTrqRed;

// Parameters
//-----------
// RotorTorqueInputT2()
CREATE_PARM(R_TrefEnbAdr,       unsigned*); // Torque ref enable pointer
CREATE_PARM(R_TrqRefAdr,        float*);    // Pointer to torque reference
CREATE_PARM(R_TrqRateLim,       float);     // torque ramp rate in pu/sec

// Diagnostics
//------------

// Variables
//----------
/* Torque Limit Select                      */
CREATE_PUBVAR(R_ILimAdj     ,float  )=1.0;       /*    Current limit adjust                  */
CREATE_PUBVAR(R_TrqLimMtr   ,float  )=1.0;       /*    Motoring torque limit adjust          */
CREATE_PUBVAR(R_TrqLimGen   ,float  )=1.0;       /*    Dynamic(?) generating torq limit adj  */
CREATE_PUBVAR(R_TrqLimGenS  ,float  )=1.0;       /*    Static(?) generating torq limit adj   */
CREATE_PUBVAR(PhiAdj        ,float  )=1.0;       /*    power factor adjust                   */
CREATE_PUBVAR(R_CtlAlgAct   ,unsigned  )=false;
CREATE_PUBVAR(R_TrqCmd2     ,float  )=0.0;
CREATE_PUBVAR(R_TrqCmd2T1   ,float  )=0.0;
CREATE_PUBVAR(R_TrefEnbReq  ,unsigned);   //
CREATE_PUBVAR(R_TrqCmd      ,float);      // Net Torque command constructed for User
CREATE_PUBVAR(R_TrqCmdAct   ,float);      // Unclamped torque command to motor control
CREATE_PUBVAR(R_TrqRef,           float);      // Net Torque reference in Sreg

DATA_PASS(R_TrqCmd2,        R_TrqCmd2T1,        T2_T1,      UNFILTERED);


/******************************************************************************
* End Function:     TorqueLimitSelect
******************************************************************************/

/******************************************************************************
* Function:         RotorTorqueInputT2
* Execution Level:  Task 2
* Execution Time:   Not measured
*
* Formal Inputs:    None
* Formal Outputs:   None
*
* Global Inputs:    PARM(R_TqrRefAdr)
*                   PARM(R_TrqEnbAdr)
*
* Global Outputs:   R_TrqCmd
*                   R_TrqCmdAct
*
* Return Value:     None
* Function Calls:   None
*
* Abstract:  Retrieves appropriate torque command and provides rate limit.
******************************************************************************/
void RotorTorqueInputT2()
{
   float Temp1;

    if ( R_CtlAlgAct && !L_LTestIp)
    {
        // =======================================================================
        // Calculate R_TrqCmdAct
        // =======================================================================
        R_TrqRef = 1000.0F * gVariable.ReadAsFloat(PARM(R_TrqRefAdr));

        if ((int)PARM(R_TrefEnbAdr) == LADR_UnusedBool)
        {
            R_TrefEnbReq = True;
            Temp1 = R_TrqRef;
        }
        else if (gVariable.ReadAsBool(PARM(R_TrefEnbAdr)))
        {
            R_TrefEnbReq = True;
            Temp1 = R_TrqRef;
        }
        else
        {
            R_TrefEnbReq = False;
            Temp1 = 0.0;
        }

        /* Rate limit torque command */
        if (PARM(R_TrqRateLim) == 0.0)
        {
            R_TrqCmd = Temp1;
        }
        else
        {
            if (R_Running)
            {
                Temp1 = (Temp1 - R_TrqCmd)*DelTm2Rcp;
                if      (Temp1 >  PARM(R_TrqRateLim)) Temp1 =  PARM(R_TrqRateLim);
                else if (Temp1 < -PARM(R_TrqRateLim)) Temp1 = -PARM(R_TrqRateLim);

                R_TrqCmd +=  Temp1 * DelTm2;
            }
            else R_TrqCmd = 0.0;
        }

        R_TrqCmdAct = -R_TrqCmd;
        R_TrqCmd2   = R_TrqCmdAct + PllTrqRed; 


    }
    else
    {
        R_TrqCmd = R_TrqCmdIn = R_TrqCmdAct = 0.0;
    }


}
/******************************************************************************
* End Function:     TorqueLimitSelect
******************************************************************************/


/******************************************************************************
* Function:         RotorFluxAdjustSelect
* Execution Level:  Task 2
* Execution Time:   Not measured
*
* Formal Inputs:    None
* Formal Outputs:   None
*
* Global Inputs:    PARM(Flx_Adj_Adr)
*                   PARM(Flx_Adjust)
*
* Global Outputs:   Flx_Adj
*
* Return Value:     None
* Function Calls:   None
*
* Abstract:  Selects the flux adjust signal used to create Flx_Ref in task 1.
******************************************************************************/
void RotorFluxAdjustSelect()
{


}
/******************************************************************************
* End Function:     FluxAdjustSelect
******************************************************************************/
