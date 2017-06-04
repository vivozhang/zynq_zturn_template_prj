/*=============================================================================
* Product:     GE Power Conversion Using MACC
* Host:        TMS320C31 DSP
* File:        $\LVP2Src\Pwm2\FW\S0PREC.C30
* Language:    TI                              'C                             '
* Copyright:   GE Drives and Controls, Inc., Salem VA
*
* Abstract: This file contains those functions which are associated with
*           conditioning of the y component of current.
* -----------------------------------------------------------------------------
*
* Functions in module:
*
* PreconditionLCL_T1                    - precondition task 1 variables
*
* PreconditionLCL_T2                    - precondition task 2 variables
*
*==============================================================================
* Last Mod: May 17, 2008 9:39:34AM
*/

#include "x0FrameWork.h"
#include "Product.h"

/*****************************************************************************
*
*       external references
*
*****************************************************************************/
extern float        GmVMagT2         ;
extern unsigned     L_DCLinkInit     ;
extern float        L_IRAFEnbTmr     ;
extern float        L_IregVnomFf     ;
extern float        L_IregX          ;
extern float        L_IregY          ;
extern float        L_IxCmd0         ;
extern float        L_IxCmdZ         ;
extern float        L_IxCmdZ1        ;
extern float        L_IxCmdZ2        ;
extern float        L_IxMeld         ;
extern float        L_IxregDcX       ;
extern float        L_IxregIxInt     ;
extern float        L_IxregResX      ;
extern float        L_IxregSpill     ;
extern float        L_IxregVOut      ;
extern float        L_IxregZErr      ;
extern float        L_IxSpillTot     ;
extern float        L_IyCmd0         ;
extern float        L_IyCmdRef       ;
extern float        L_IyCmdZ         ;
extern float        L_IyCmdZ1        ;
extern float        L_IyCmdZ2        ;
extern float        L_IyRefLim       ;
extern float        L_IyregDcY       ;
extern float        L_IyregIyInt     ;
extern float        L_IyregResY      ;
extern float        L_IyregV2Ff      ;
extern float        L_IyregVFf       ;
extern float        L_IyregVOut      ;
extern float        L_IyregZErr      ;
extern float        L_IyrfIInt       ;
extern float        L_IyrfIyT2       ;
extern float        L_IyrfMaxLim     ;
extern float        L_IyrfMinLim     ;
extern float        L_UxCmd          ;
extern float        L_UxLimOut       ;
extern float        L_UyCmd          ;
extern float        L_VdcReg         ;
extern float        L_VregErr        ;
extern float        L_VregIxInt      ;
extern float        L_VregRef        ;
extern float        L_IrgSplGnDy     ; 
extern float        L_IyCmdLimDy     ;
extern float        S_VMaxPuPeak     ;
extern unsigned     S_VMaxHiEnb      ;

extern RegLimFlgs   L_InnrRegLim     ;



/* **************************************************************************
*
*                               Private Variables
*
*****************************************************************************/







/******************************************************************************
* Function:         PreconditionLCL_T1
* Execution Level:  Task 1
* Execution Time:   Not measured
*
* Formal Inputs:    None
* Formal Outputs:   None
*
* Global Inputs:    L_DelTm1
*
* Global Outputs:   L_DelTm1
*
*
* Return Value:
* Function Calls:   None
*
* Abstract:         Preconditions storage element vars when L_InnrRegEnb not
*                   set.
*
******************************************************************************/

void PreconditionLCL_T1()

{
    float Temp1;

    Temp1        = 0.0;

    // Voltage regulator variables
    L_VregIxInt  = Temp1;
    L_IxCmd0     = Temp1;
    L_VregErr     = Temp1;
    L_VregRef     = L_VdcReg;


   // Current regulator variables

    L_IregX       = Temp1;
    L_IregY       = Temp1;
    L_IxMeld      = Temp1;
    L_IxregIxInt  = Temp1;
    L_IxregSpill  = Temp1;
    L_UxLimOut    = Temp1;
    L_IxSpillTot  = Temp1;
    L_IyregIyInt  = Temp1;
    L_IxregVOut   = Temp1;
    L_IyregVOut   = Temp1;
    L_IxregResX   = Temp1;
    L_IyregResY   = Temp1;
    L_IxregDcX    = Temp1;
    L_IyregDcY    = Temp1;
    L_IRAFEnbTmr  = Temp1;
    L_IxCmd0      = Temp1;
    L_IxCmdZ1     = Temp1;
    L_IxCmdZ2     = Temp1;
    L_IyCmd0      = Temp1;
    L_IyCmdZ1     = Temp1;
    L_IyCmdZ2     = Temp1;
    L_IxCmdZ      = Temp1;
    L_IyCmdZ      = Temp1;
    L_IxregZErr   = Temp1;
    L_IyregZErr   = Temp1;
    L_IyregVFf    = Temp1;
    L_IyregV2Ff   = Temp1;
    L_UxCmd       = PARM(L_UxCmdPrec);
    L_UyCmd       = Temp1;
    L_InnrRegLim.L_XAntiWndP = false;
    L_InnrRegLim.L_XAntiWndN = false;
    L_InnrRegLim.L_IxrefPLim = false;
    L_InnrRegLim.L_IxrefNLim = false;
    L_InnrRegLim.L_UyLimPFlg = false;
    L_InnrRegLim.L_UyLimNFlg = false;
    L_InnrRegLim.L_IyLimPFlg = false;
    L_InnrRegLim.L_IyLimNFlg = false;
    L_InnrRegLim.L_UxlimPFlg = false;
    L_InnrRegLim.L_UxlimNFlg = false;
    L_DCLinkInit             = false;

    L_IregVnomFf = GmVMagT2;
    L_IrgSplGnDy = PARM(L_IregSpilGn); 
    L_IyCmdLimDy = PARM(L_IyCmdLim); 
    S_VMaxPuPeak = 0.0;
    S_VMaxHiEnb  = false; 

}


/******************************************************************************
* Function:         PreconditionLCL_T2
* Execution Level:  Task 2
* Execution Time:   Not measured
*
* Formal Inputs:    None
* Formal Outputs:   None
*
* Global Inputs:
*
*
* Global Outputs:
*
* Return Value:
* Function Calls:   None
*
* Abstract:         Preconditions storage element vars when L_InnrRegEnb not
*                   set.
*
******************************************************************************/

void PreconditionLCL_T2()

{




}


/******************************************************************************
* Function:         PreconditionIyref_T2
* Execution Level:  Task 2
* Execution Time:   Not measured
*
* Formal Inputs:    None
* Formal Outputs:   None
*
* Global Inputs:    None
*
*
* Global Outputs:
*
* Return Value:
* Function Calls:   None
*
* Abstract:         Preconditions storage element vars when L_InnrRegEnb not
*                   set.
*
******************************************************************************/
void PreconditionIyref_T2()
{

    L_IyCmdRef    = 0.0;
    L_IyrfIInt    = 0.0;
    L_IyrfIyT2    = 0.0;


     L_IyRefLim   = PARM(L_IyrfIMax)*SQRT2;
     if ( PARM(L_DrvSupply) )
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


}
