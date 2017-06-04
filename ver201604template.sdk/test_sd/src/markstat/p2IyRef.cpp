/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        TMS320C31 DSP
* File:        P2IYREF.C30
* Language:    TI 'C'
* Copyright:   GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file contains functions associated with production of
*           reactive current reference generation.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
*
*==============================================================================
* 980527 SAB  Create for reactive current reference function.
* 980603 CWM  Remove rogue #if 1.
* 980605 CWM  Add defines for Iyrf_P_Frz, Iyrf_N_Frz.
* 990107 SAB  ACMVRS6_A: Add reactive current ref from inductor test
* 990310 DGF  Tentatively remove Lan_V_Ref & Lan_V_Fbk
* 990712 SAB  ACMVRS6_A only:  Add extra del_tm_2 multiply for
* 990712 SAB  Fix precondition value of Iy_Ref_Lim so that toolbox diagram
*             does not shown value to small by SQRT2
* 990715 SAB  Use Iyrf_Int_Gnx for both products.
* 000329 SAB  ACCBR_A only: Activate reactive current ref for inductance test.
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 021210 SAB  Rename to P2IyRef.c30
* 031218 SAB  Move slow function, PreconditionIyref_T2 to S0PREC.C30
* 071114 SAB  Clean up for new inductance test
*______________________________________________________________________________
* Last Mod: November 14, 2007 12:00:37PM
*/

#include "x0FrameWork.h"
#include "Product.h"


extern float        DelTm2              ;
extern float        L_IxCmdT2;
extern unsigned     L_InnrRegORT2 ;


//Select RegLimFlgs needed for L_Iy current reference integrator freeze
// For limiting L_IyrfDel to positive values only:
// L_UxlimPFlg, L_IyLimNFlg, and L_UyLimNFlg (bits 10,9,7)
// So, IYRF_P_MASK mask is 680H = /0000/0110/1000/0000
#define  IYRF_P_MASK      0x0680

// For limiting L_IyrfDel to negative values only:
// L_UxlimNFlg, L_IyLimPFlg, and L_UyLimPFlg (bits 11,8,6)
// So, IYRF_P_MASK mask is 680H = /0000/1001/0100/0000
#define  IYRF_N_MASK      0x0940
// Types and Classes
//------------------

// Parameters
//-----------
CREATE_PARM(L_UnityPF,     unsigned);   // Regulate unity power factor at source term.
CREATE_PARM(L_DrvSupply,   unsigned);   // Source is soley a drive supplier
CREATE_PARM(L_VRegOnly,    unsigned);   // Source is a voltage regulator only
CREATE_PARM(L_VReduction,  unsigned);   // Allow voltage reduction when voltage reg
CREATE_PARM(L_IyrfIntPlm,  float);   // Reactive current ref gen int. pos limit
CREATE_PARM(L_IyrfMaxOut,  float);   // Reactive current ref gen max. output
CREATE_PARM(L_IyrfMinOut,  float);   // Reactive current ref gen min. output
CREATE_PARM(L_IyrfIMax,    float);   // Reactive current ref gen max. current
CREATE_PARM(L_IyrfIntNlm,  float);   // Reactive current ref gen int. neg limit
CREATE_PARM(L_IyrfIAdj,    float);   // Reactive current adjustment
CREATE_PARM(L_IyrfIStpt,   float);   // Reactive current reference setpoint
CREATE_PARM(L_IySlwRat,    float);   // y-reference current reg. slew rate
CREATE_PARM(L_IyrfCmdAdr,  void *);   // Address for L_Iy ref current command
CREATE_PARM(L_IyrfFbkAdr,  void *);   // Address for L_Iy ref current feedback
CREATE_PARM(L_IyrfCmdGn,   float);   // Reactive current gen. command gain
CREATE_PARM(L_IyrfCmdScl,  float);   // Reactive current command offset
CREATE_PARM(L_IyrfFbkGn,   float);   // Reactive current gen. feedback gain
CREATE_PARM(L_IyrfFbkScl,  float);   // Reactive current command offset
CREATE_PARM(L_WIyLim,      float);   // Omega for Y current reference limit
CREATE_PARM(L_IyrfIntGn,   float);   // Y current reference regulator gain
CREATE_PARM(L_IyrfVStpt,   float);   // Reactive current reference V setpoint

// Diagnostics
//------------

// Variables
//----------
/* Current regulator reference generation */
CREATE_PUBVAR(L_IyCmdRef   , float);            // reactive current reference
CREATE_PUBVAR(L_IyrfErr    , float);            // reactive current reference error
CREATE_PUBVAR(L_IyrfDel    , float);            // reactive current reference delta
CREATE_PUBVAR(L_IyRefLim   , float);            // reactive current reference limit
CREATE_PUBVAR(L_IyRatSqr   , float);            // reactive current rating squared
CREATE_PUBVAR(L_IReserve   , float);            // Current reserve

CREATE_PUBVAR(L_IyrfIntGnx , float);            // reactive current ref integral gain

CREATE_PUBVAR(L_IyrfCmd    , float);            // reactive current reference command
CREATE_PUBVAR(L_IyrfFbk    , float);            // reactive current reference feedback
CREATE_PUBVAR(L_IyrfMaxLim , float);            // reactive current reference max. limit
CREATE_PUBVAR(L_IyrfMinLim , float);            // reactive current reference min. limit
CREATE_PUBVAR(L_IyrfIInt   , float);            // reactive current reference I integrator
CREATE_PUBVAR(L_IyrfIyT2   , float);            // reactive current reference (task 2)
CREATE_PUBVAR(L_IyrfPHist  , unsigned);         //Y channel current ref. pos freeze history
CREATE_PUBVAR(L_IyrfNHist  , unsigned);         //Y channel current ref. neg freeze history
CREATE_PUBVAR(L_IyrfPSum   , unsigned);         //Task 2 summary of Iyrf pos2 limit flags
CREATE_PUBVAR(L_IyrfNSum   , unsigned);         //Task 2 summary of Iyrf neg limit flags
CREATE_PUBVAR(L_IyrfPFrz   , unsigned);         // reactive current ref. Pos freeze flag
CREATE_PUBVAR(L_IyrfNFrz   , unsigned);         // reactive current ref. Neg freeze flag



/*=============================================================================
 * Function: YCurrentRegReference
 * Execution Level: Task 2
 * Execution Time:
 * Formal Inputs: None
 * Formal Outputs: None
 *
 * Global Inputs:  DelTm2
 *                 L_IxCmdT2
 *                 L_IyRatSqr
 *                 L_Iy_Rat_Frz
 *
 *                 EE.L_Iy_Ref
 *                 EE.L_IySlwRat
 *                 EE.Use_Ext_Yref
 *                 EE.L_VllIn
 *                 EE.L_VllRms
 *                 EE.L_Iy_Ref_Adr
 *                 EE.L_IyFbk_Adr
 *
 *
 *
 *
 *
 * Global Outputs: L_Iy_Ref_Cal
 *
 * Locals:         L_Iy_Ref_Err
 *                 L_IyRefLim
 *                 L_IyrfDel
 *                 L_IReserve
 *                 L_IyrfCmd
 *                 L_IyrfFbk
 *                 L_IyrfMaxLim
 *                 L_IyrfMinLim
 *
 *                 EE.L_WIyLim
 *
 *
 * Return Value: Not applicable
 * Function Calls: None
 *
 * Abstract:
 *    This function calculates the reference for the y-channel fo the current
 *  regulator.  It can use either an internal "EE" based reference, or an
 *  external reference.  This functions preforms a slew rate on the reference.
 *
 *===========================================================================*/

void YCurrentRegReference()
{
     float   L_Iy_Slw_Del, L_Iy_Slw_Del_Lim,L_IReserve_Sqr;
     float   L_IyrfCmd_In, L_IyrfFbk_In;


    //Check for flags used to limit L_IyrfDel to positive values
    //See S0BITDEF.H for definition of positive freeze flags
    L_IyrfPSum = IYRF_P_MASK & L_InnrRegORT2 ;
    if ( L_IyrfPSum != 0 )     /* L_Iy reference positive freeze */
    {
        L_IyrfPFrz  = true;
        L_IyrfPHist = L_IyrfPSum;
    }
    else
    {
        L_IyrfPFrz    = false;
    }

    //Check for flags used to limit L_IyrfDel to negative values
    //See S0BITDEF.H for definition of positive freeze flags
    L_IyrfNSum = IYRF_N_MASK & L_InnrRegORT2 ;
    if ( L_IyrfNSum != 0 )     /* L_Iy reference negative freeze */
    {
        L_IyrfNFrz  = true;
        L_IyrfNHist = L_IyrfNSum;
    }
    else
    {
        L_IyrfNFrz    = false;
    }






     L_IyrfCmd_In = gVariable.ReadAsFloat(PARM(L_IyrfCmdAdr));
     L_IyrfFbk_In = gVariable.ReadAsFloat(PARM(L_IyrfFbkAdr));

     L_IyrfCmd = PARM(L_IyrfCmdScl)*L_IyrfCmd_In + PARM(L_IyrfIAdj)*SQRT2;
     L_IyrfFbk = PARM(L_IyrfFbkScl)*L_IyrfFbk_In;


     //Calculate L_IyRefLim//
     L_IReserve_Sqr = L_IyRatSqr - L_IxCmdT2*L_IxCmdT2;
     if ( L_IReserve_Sqr <= 0.0F)
     {
          L_IReserve_Sqr = 0.0F;
     }

     L_IReserve = sqrtf(L_IReserve_Sqr);

     L_IyRefLim = L_IyRefLim + DelTm2*PARM(L_WIyLim)*(L_IReserve - L_IyRefLim);

     if ( PARM(L_DrvSupply))
     {
        L_IyrfMaxLim = L_IyRefLim;
        L_IyrfMinLim = -L_IyRefLim;
     }
     else
     {
        L_IyrfMaxLim = PARM(L_IyrfMaxOut)*SQRT2;
        if ( L_IyrfMaxLim >= L_IyRefLim)
        {
             L_IyrfMaxLim = L_IyRefLim;
        }

        L_IyrfMinLim = PARM(L_IyrfMinOut)*SQRT2;
        if ( L_IyrfMaxLim <= -L_IyRefLim)
        {
             L_IyrfMaxLim = -L_IyRefLim;
        }
     }


     //Rate limiter and Reactive current Reference Regulator
     L_Iy_Slw_Del_Lim = DelTm2*PARM(L_IySlwRat);
     L_Iy_Slw_Del     = (L_IyrfCmd - L_IyCmdRef);

     if ( L_Iy_Slw_Del > L_Iy_Slw_Del_Lim)
     {
         L_Iy_Slw_Del = L_Iy_Slw_Del_Lim;
     }
     else if (L_Iy_Slw_Del < -L_Iy_Slw_Del_Lim)
     {
         L_Iy_Slw_Del = -L_Iy_Slw_Del_Lim;
     }

     L_IyCmdRef = L_IyCmdRef + L_Iy_Slw_Del;


     L_IyrfErr = L_IyCmdRef - L_IyrfFbk;

     //Integrator freeze logic (operates on L_IyrfDel)
     L_IyrfDel = L_IyrfIntGnx*DelTm2*L_IyrfErr;
     if ( L_IyrfPFrz) //Allow only positive L_IyrfDel when this flag is set
     {
          if ( L_IyrfDel <= 0.0)
          {
               L_IyrfDel = 0.0;
          }
     }
     else if ( L_IyrfNFrz) //Allow only negative L_IyrfDel when this flag is set
     {
          if ( L_IyrfDel >= 0.0)
          {
               L_IyrfDel = 0.0;
          }
     }


     //Integrate and limit
     L_IyrfIInt = L_IyrfIInt + L_IyrfDel;

     if ( L_IyrfIInt > PARM(L_IyrfIntPlm)*SQRT2)
     {
          L_IyrfIInt = PARM(L_IyrfIntPlm)*SQRT2;
     }
     else if (L_IyrfIInt < PARM(L_IyrfIntNlm)*SQRT2)
     {
          L_IyrfIInt = PARM(L_IyrfIntNlm)*SQRT2;
     }


    L_IyrfIyT2 = (L_IyCmdRef*PARM(L_IyrfCmdGn)) - (L_IyrfFbk*PARM(L_IyrfFbkGn)) +
                 L_IyrfIInt;

     if ( L_IyrfIyT2 > L_IyrfMaxLim)
     {
          L_IyrfIyT2 = L_IyrfMaxLim;
     }
     else if  ( L_IyrfIyT2 < L_IyrfMinLim)
     {
          L_IyrfIyT2 = L_IyrfMinLim;
     }
}
