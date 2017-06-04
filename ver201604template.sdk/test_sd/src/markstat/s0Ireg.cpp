///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Source current regulator
//
// DESCRIPTION:
//      This file contains  x-y source current regulator
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
#include "x0FrameWork.h"
// product
#include "Product.h"


// Constants
//----------


// Externals
//----------
extern float        DelTm2          ;
extern float        GmCos2Ang       ;
extern float        GmCosAngFbk     ;
extern float        GmDscmVMagN     ;
extern float        GmDscmVMagP     ;
extern float        GmSin2Ang       ;
extern float        GmSinAngFbk     ;
extern float        L_DcIxFbk       ;
extern float        L_DcIyFbk       ;
extern float        L_DcXStep       ;
extern float        L_DcYStep       ;
extern float        L_DelTm1        ;
extern float        L_IxCmd         ;
extern float        L_IxCmd0        ;
extern float        L_IxCmdZ1Sc     ;
extern float        L_IxCmdZ2Sc     ;
extern float        L_IxFbk         ;
extern float        L_IxregStep     ;
extern float        L_IyCmdA0Sc     ;
extern float        L_IyCmdA1Sc     ;
extern float        L_IyCmdD0Sc     ;
extern float        L_IyCmdD1Sc     ;
extern float        L_IyCmdZ1Sc     ;
extern float        L_IyCmdZ2Sc     ;
extern float        L_IyFbk         ;
extern float        L_IyVdc         ;
extern float        L_IyregStep     ;
extern float        L_IyrfIyT2      ;
extern float        L_LTotCal       ;
extern float        L_LTotDt        ;
extern float        L_ModIndexT2    ;
extern float        L_NsXStep       ;
extern float        L_NsYStep       ;
extern float        L_PllWNom       ;
extern float        L_VdcFbk        ;
extern float        L_VdcFbkRcp     ;
extern float        L_VlnIn         ;
extern float        L_VxFbk         ;
extern float        L_VyFbk         ;

extern unsigned     GmPllHot        ;
extern unsigned     GmRevSeq        ;
extern unsigned     L_InnrRegEnb    ;
extern unsigned     L_Running       ;
extern RegLimFlgs   L_InnrRegLim    ;

extern float        ISysCmdSF;
// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(L_IxrgStpAdr,       float*);    // Ix current regulator step address
CREATE_PARM(L_IyrgStpAdr,       float*);    // Iy current regulator step address
CREATE_PARM(L_IxregWFf,         float);     // x-current reg. omega ff for Vnom
CREATE_PARM(L_IyCmdLim,         float);     //
CREATE_PARM(L_RegLimW,          float);     // Inner regulator limit omega
CREATE_PARM(L_IxMeldReq,        float);     // x-channel meld of req/cmd current
CREATE_PARM(L_IyMeldReq,        float);     // y-channel meld of req/cmd current
CREATE_PARM(L_IxregIntGn,       float);     // x-current regulator integral gain
CREATE_PARM(L_IxregCmdGn,       float);     // x-current regulator command gain
CREATE_PARM(L_IxregFbkGn,       float);     // x-current regulator feedback gain
CREATE_PARM(L_IregPGn2,         float);     // Current regulator proportional gain 2
CREATE_PARM(L_IregErr2W,        float);     // Current regulator current error2 2 filter response
CREATE_PARM(L_IxregIntLm,       float);     // x-current regulator integral gain
CREATE_PARM(L_IxregULim,        float);     // x-current regulator up/down limit
CREATE_PARM(L_IyregIntGn,       float);     // x-current regulator integral gain
CREATE_PARM(L_IyregCmdGn,       float);     // x-current regulator command gain
CREATE_PARM(L_IyregFbkGn,       float);     // x-current regulator feedback gain
CREATE_PARM(L_IyregIntLm,       float);     // y-current regulator integral gain
CREATE_PARM(L_IyregULim,        float);     // y-current regulator up/down limit
CREATE_PARM(L_IregResBW,        float);     // current regulator resonator bandwidth
CREATE_PARM(L_IregResLim,       float);     // current regulator resonator limit
CREATE_PARM(L_RsLmZeroOn,       float);     // current regulator resonator clamp limit neg sequence 'on' threshold
CREATE_PARM(L_RsLmModOn,        float);     // current regulator resonator clamp limit modulation 'on' threshold
CREATE_PARM(L_RsLmModOff,       float);     // current regulator resonator clamp limit modulation 'off' threshold
CREATE_PARM(L_IregResWI,        float);     // current regulator reson. int omega
CREATE_PARM(L_IregSpilGn,       float);     // current regulator spill
CREATE_PARM(L_IregBwBase,       float);     // Current regulator Bandwidth base
CREATE_PARM(L_Res2ndD,          float);     // current reg resonator 2nd harm. D axix gn.
CREATE_PARM(L_Res2ndQ,          float);     // current reg resonator 2nd harm. Q axix gn.
CREATE_PARM(L_NsXStpAdr,        float*);    // negative seq. regulator x channel step point
CREATE_PARM(L_NsYStpAdr,        float*);    // negative seq. regulator y channel step point
CREATE_PARM(L_ResBwBase,        float);     // negative seq regulator bandwidth base
CREATE_PARM(L_IxCmdZ1Gn,        float);     //
CREATE_PARM(L_IxCmdZ2Gn,        float);     //
CREATE_PARM(L_IyCmdZ1Gn,        float);     //
CREATE_PARM(L_IyCmdZ2Gn,        float);     //
CREATE_PARM(L_IyCmdD0Gn,        float);     //
CREATE_PARM(L_IyCmdD1Gn,        float);     //
CREATE_PARM(L_IyCmdA0Gn,        float);     //
CREATE_PARM(L_IyCmdA1Gn,        float);     //
CREATE_PARM(L_LTotSteady,       float);     // Steady State L_LTot
CREATE_PARM(L_UxCmdPrec,        float);     // Precondition value for Ux
CREATE_PARM(L_UxLimGn,          float);     // Current regulator integral spillover gain
CREATE_PARM(L_UxLimMin,         float);     // Current reg integral spillover minimum
CREATE_PARM(L_UxLim,            float);     // Current reg integral spillover limit
// Dc Current Regulator
CREATE_PARM(L_IregDcLim,        float);     // Dc Current Regulator ihtegrator limit
CREATE_PARM(L_Dc1stD,           float);     // Dc Current Regulator direct axis gain
CREATE_PARM(L_Dc1stQ,           float);     // Dc Current Regulator quad axis gain
CREATE_PARM(L_DcRegWI,          float);     // Dc Current Regulator omega
CREATE_PARM(L_DcIregBW,         float);     // Dc Current Regulator bandwidth
CREATE_PARM(L_DcIregMax,        float);     // Dc Current Regulator Maximum output
CREATE_PARM(L_DcXStpAdr,        float*);    // Dc Current Regulator x axis step address
CREATE_PARM(L_DcYStpAdr,        float*);    // Dc Current Regulator y axis step address
// LineCurrentRegCalcsT2()
CREATE_PARM(L_PhImbIpThr,       float);     // Phase imbalance in progress threshold
CREATE_PARM(L_PhImbIpTm,        float);     // Phase imbalance in progress time

// Variables
//----------

    // XYCurrentReg procedure
CREATE_PUBVAR(L_IregX               ,float   );      // x current regulator pre-ouput
CREATE_PUBVAR(L_IregY               ,float   );      // y current regulator pre-ouput
CREATE_PUBVAR(L_IyregV2Ff           ,float   );      // y-current regulator voltage feed-fwd 2
CREATE_PUBVAR(L_IxregV2Ff           ,float   );      // x-current regulator voltage feed-fwd 2
CREATE_PUBVAR(L_IyregVFf            ,float   );      // y-current regulator voltage feed-fwd
CREATE_PUBVAR(L_IxregZErr           ,float   );      // x current regulator int. path error
CREATE_PUBVAR(L_IxregIxInt          ,float   );      // x current regulator integrator
CREATE_PUBVAR(L_IxregSpill          ,float   );      // x current regulator spill-over
CREATE_PUBVAR(L_IxErr2              ,float   );      // x current regulator error 2
CREATE_PUBVAR(L_IxErr2Fil           ,float   );      // x current regulator error 2 filtered
CREATE_PUBVAR(L_IxregPrp2           ,float   );      // x current regulator propotional output 2
CREATE_PUBVAR(L_IxregVOut           ,float   );      // un-compensated x-channel output voltage
CREATE_PUBVAR(L_IyregFfGn           ,float   );      // y-crrent reg. feed forward gain
CREATE_PUBVAR(L_IyregIyInt          ,float   );      // y current regulator integrator
CREATE_PUBVAR(L_IyregZErr           ,float   );      // y current regulator int. path error
CREATE_PUBVAR(L_IyErr2              ,float   );      // x current regulator error 2
CREATE_PUBVAR(L_IyErr2Fil           ,float   );      // y current regulator error 2 filtered
CREATE_PUBVAR(L_IyregPrp2           ,float   );      // y current regulator propotional output 2
CREATE_PUBVAR(L_IyregVOut           ,float   );      // un-compensated y-channel output voltage
CREATE_PUBVAR(L_IyregSpill          ,float   );      // y current regulator spill-over
CREATE_PUBVAR(L_UxCmd               ,float   );      // voltage reg. x component output
CREATE_PUBVAR(L_UyCmd               ,float   );      // voltage reg. y component output
CREATE_PUBVAR(L_IxCmdZ              ,float   );      // x current regulator integral command
CREATE_PUBVAR(L_IyCmdZ              ,float   );      // y current regulator integral command
CREATE_PUBVAR(L_IxCmdZ1             ,float   );      // (z-1)x current regulator command
CREATE_PUBVAR(L_IxCmdZ2             ,float   );      // (z-2)x current regulator command
CREATE_PUBVAR(L_IyCmd               ,float   );      // current regulator command - from T2
CREATE_PUBVAR(L_IyCmd0              ,float   );      // y-component current command-no delay
CREATE_PUBVAR(L_IyCmdZ1             ,float   );      // (z-1)y current regulator command
CREATE_PUBVAR(L_IyCmdZ2             ,float   );      // (z-2)y current regulator command
CREATE_PUBVAR(L_UxLimOut            ,float   );      // Intergral spillover output
CREATE_PUBVAR(L_IxSpillTot          ,float   );      // Spillover output total
CREATE_PUBVAR(L_IregXUc             ,float   );      // Un-clamped L_IregX
CREATE_PUBVAR(L_IregYUc             ,float   );      // Un-clamped L_IregY
//CREATE_PUBVAR(L_SrcCtl1             ,WORD       );      // source control bit coded word 1
CREATE_PUBVAR(L_IxMeld              ,float   );      // x current feedback/command meld
CREATE_PUBVAR(L_IyMeld              ,float   );      // y current feedback/command meld
CREATE_PUBVAR(L_IregEr2FGn          ,float   );
CREATE_PUBVAR(L_IrgSplGnDy          ,float   );      // Current regulator spillover dymamic gain
CREATE_PUBVAR(L_IyCmdLimDy          ,float   );      // Current regulator reactive current dynamic limit

    // Line voltage/current calculations
CREATE_PUBVAR(L_IregVnomFf          ,float   );      // current regulator nominal voltage  ff
CREATE_PUBVAR(L_IxCmd0              ,float   );      // x-component current command-no delay

    //Resonsator vars
CREATE_PUBVAR(L_ResLimDyn           ,float   );      // Dynamic resonator limit
CREATE_PUBVAR(L_ResLimDyn2          ,float   );      // Dynamic resonator limit task 2
CREATE_PUBVAR(L_RsLmZero            ,unsigned);      // Dyncamic resonator limit zero flag
CREATE_PUBVAR(L_IxregRsOut          ,float   );      // X-channel final resonator output
CREATE_PUBVAR(L_IyregRsOut          ,float   );      // Y-channel final resonator output
CREATE_PUBVAR(L_IxregResX           ,float   );      // X-channel current reg. resonator output
CREATE_PUBVAR(L_IyregResY           ,float   );      // Y-channel current reg. resonator output
CREATE_PUBVAR(L_ResIx2              ,float   );      // Demod x-channel current error
CREATE_PUBVAR(L_ResIy2              ,float   );      // Demod y-channel current error
CREATE_PUBVAR(L_ResX2               ,float   );      // Resonator x-channel input to rotator
CREATE_PUBVAR(L_ResY2               ,float   );      // Resonator y-channel input to rotator
CREATE_PUBVAR(L_ResXOut             ,float   );      // Resonator x-channel intermediate output
CREATE_PUBVAR(L_ResYOut             ,float   );      // Resonator y-channel intermediate output
CREATE_PUBVAR(L_PhImbIp             ,unsigned);      // Phase imbalance in progress
CREATE_PUBVAR(L_PhImbIpTmr          ,float   );      // Phase imbalance in progress timer

    // Diagnostic Vars
CREATE_PUBVAR(L_VxErrFbk            ,float   );
CREATE_PUBVAR(L_VyErrFbk            ,float   );
CREATE_PUBVAR(L_UxCmdz2             ,float   );
CREATE_PUBVAR(L_UxCmdz1             ,float   );
CREATE_PUBVAR(L_UyCmdz1             ,float   );
CREATE_PUBVAR(L_UyCmdz2             ,float   );
CREATE_PUBVAR(L_ModIndex            ,float   );

    // DC current regulator
CREATE_PUBVAR(L_IxregDcOut          ,float   );      // DC current regulator  x axis output
CREATE_PUBVAR(L_IyregDcOut          ,float   );      // DC current regulator  x axis output
CREATE_PUBVAR(L_DcIx                ,float   );      // DC current regulator  x axis demodulated current
CREATE_PUBVAR(L_DcIy                ,float   );      // DC current regulator  y axis demodulated current
CREATE_PUBVAR(L_IxregDcX            ,float   );      // DC current regulator  x axis integrator
CREATE_PUBVAR(L_IyregDcY            ,float   );      // DC current regulator  x axis integrator
CREATE_PUBVAR(L_DcFndX              ,float   );      // DC current regulator  x axis roator input
CREATE_PUBVAR(L_DcFndY              ,float   );      // DC current regulator  y axis roator input
CREATE_PUBVAR(L_DcFndXT1            ,float   );      // DC current regulator  x axis roator input
CREATE_PUBVAR(L_DcFndYT1            ,float   );      // DC current regulator  y axis roator input
CREATE_PUBVAR(L_DcXOut              ,float   );      // DC current regulator  x axis rotator output
CREATE_PUBVAR(L_DcYOut              ,float   );      // DC current regulator  y axis rotator output


// Unpublished Variables
//----------------------
CtlBt1       L_SrcCtl1;


// Data Passing
//--------------
DATA_PASS(L_DcFndX, L_DcFndXT1, T2_T1, UNFILTERED);  //  dc current regulator, x
DATA_PASS(L_DcFndY, L_DcFndYT1, T2_T1, UNFILTERED);  //  dc current regulator, x


// Diagnostics
//------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//     This function performs the regulation of x-y current, including PI
//     structures, resonators, and DC component regulation.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void XYCurrentReg()
{
     float   L_Ix_Vreg_Del_Int,L_Iy_Vreg_Del_Int;



     L_IxMeld = (L_IxCmd0*PARM(L_IxMeldReq)) + L_IxFbk*(1.0 - PARM(L_IxMeldReq));

     L_IyMeld = (L_IyCmd0*PARM(L_IyMeldReq)) + L_IyFbk*(1.0 - PARM(L_IyMeldReq));


     L_IxCmd0 = L_IxCmd + L_IxregStep;
     L_IxErr2    = L_IxCmd0 - L_IxFbk;
     L_IxErr2Fil = L_IxErr2Fil + L_IregEr2FGn*(L_IxErr2 - L_IxErr2Fil);
     // 0 = "Original"
     // 1 = "Ireg LP filter:
     // 2 = "Feedback LP filter;
     // 3 = "Both"
     if ( ( PARM(L_IregFbkTyp) == 1) || ( PARM(L_IregFbkTyp) == 3))
     {
        L_IxregPrp2 = PARM(L_IregPGn2)*L_IxErr2Fil;
     }
     else
     {
        L_IxregPrp2 = 0.0F;
     }

     // Form x-channel current delay model and feed-forward
     L_IxCmdZ   =  (L_IxCmdZ1Sc*L_IxCmdZ1) +
                   (L_IxCmdZ2Sc*L_IxCmdZ2);



     L_IxregV2Ff = L_LTotCal*L_PllWNom*L_IyrfIyT2;

     if ( GmRevSeq)
     {
         L_IxregV2Ff = -L_IxregV2Ff;
     }


     //                     X-channel Current Regulator
     //------------------------------------------------


     // Form ixreg delta for integrator
     L_IxregZErr     = L_IxCmdZ - L_IxFbk;
     L_Ix_Vreg_Del_Int = L_IxregZErr*PARM(L_IxregIntGn)*L_DelTm1;

     if (( L_IxregZErr >= 0.0F) && L_InnrRegLim.L_XAntiWndP)
     {
        L_Ix_Vreg_Del_Int = 0.0F;
     }
     else if (( L_IxregZErr < 0.0F) && (L_InnrRegLim.L_XAntiWndN))
     {
        L_Ix_Vreg_Del_Int = 0.0F;
     }

     // Integrate and clamp
     L_IxregIxInt = L_IxregIxInt + L_Ix_Vreg_Del_Int;
     if ( L_IxregIxInt >= PARM(L_IxregIntLm) )
     {
          L_IxregIxInt = PARM(L_IxregIntLm);
     }
     else if ( L_IxregIxInt <= -PARM(L_IxregIntLm) )
     {
          L_IxregIxInt = -PARM(L_IxregIntLm);
     }

     L_IxregVOut = L_IxregIxInt + L_IxCmd0*PARM(L_IxregCmdGn) + L_IxregPrp2 -
                   L_IxFbk*PARM(L_IxregFbkGn) - L_IregVnomFf - L_IxregV2Ff;

     // Form L_IregX and check limits
     L_InnrRegLim.L_UxlimPFlg = false;
     L_InnrRegLim.L_UxlimNFlg = false;
     L_IregX      = L_IxregVOut*L_VdcFbkRcp*SQRT3;
     L_IregXUc   = L_IregX;
     if ( L_IregX >= PARM(L_IxregULim) )
     {
          L_IregX = PARM(L_IxregULim);
          L_InnrRegLim.L_UxlimPFlg = true;
     }
     else if ( L_IregX <= -PARM(L_IxregULim) )
     {
          L_IregX = -PARM(L_IxregULim);
          L_InnrRegLim.L_UxlimNFlg = true;
     }

     L_IxregSpill = L_IregXUc - L_IregX;

     // Integrating Ux limiter pushes down L_Iy reference
     // This only supports fwd rotation. Need 2nd integrator for rev rotation (L_UxCmd > 0)
     L_UxLimOut = L_UxLimOut + PARM(L_UxLimGn) * L_DelTm1 * (L_IregXUc + PARM(L_UxLim));

     if (L_UxLimOut > 0.0F)
     {
        L_UxLimOut = 0.0F;
     }
     else if ( L_UxLimOut < PARM(L_UxLimMin) )
     {
        L_UxLimOut = PARM(L_UxLimMin);
     }

     L_IxSpillTot = L_UxLimOut + L_IrgSplGnDy*L_IxregSpill;


     //                     Y-channel Current Regulator
     //------------------------------------------------

     // Form L_IyCmd0, limit and clear/set limit flags
     L_IyCmd = (L_IyrfIyT2 + L_IxSpillTot - L_IyVdc) * ISysCmdSF;
     L_InnrRegLim.L_IyLimPFlg = false;
     L_InnrRegLim.L_IyLimNFlg = false;

     if ( L_IyCmd >= L_IyCmdLimDy)
     {
        L_IyCmd = L_IyCmdLimDy;
        L_InnrRegLim.L_IyLimPFlg = true;
     }
     else if (L_IyCmd <= -L_IyCmdLimDy)
     {
        L_IyCmd = -L_IyCmdLimDy;
        L_InnrRegLim.L_IyLimNFlg = true;
     }

     L_IyCmd0 = L_IyCmd + L_IyregStep;
     L_IyErr2    = L_IyCmd0 - L_IyFbk;
     L_IyErr2Fil = L_IyErr2Fil + L_IregEr2FGn*(L_IyErr2 - L_IyErr2Fil);
     // 0 = "Original"
     // 1 = "Ireg LP filter:
     // 2 = "Feedback LP filter;
     // 3 = "Both"
     if ( ( PARM(L_IregFbkTyp) == 1) || ( PARM(L_IregFbkTyp) == 3))
     {
        L_IyregPrp2 = PARM(L_IregPGn2)*L_IyErr2Fil;
     }
     else
     {
        L_IyregPrp2 = 0.0;
     }

     if ( GmRevSeq)
     {
        L_IyCmd0   = -L_IyCmd0;
     }

     // Form L_Iy command current delay model
     // and voltage feed-forwards

     L_IyCmdZ   =   (L_IyCmdZ1Sc*L_IyCmdZ1) +
                    (L_IyCmdZ2Sc*L_IyCmdZ2);

     L_IyregVFf  =  L_LTotDt     * ((L_IyCmdD0Sc*L_IyCmd0) +
                                    (L_IyCmdD1Sc*L_IyCmdZ1))
                                 +

                     PARM(L_RTot) * ((L_IyCmdA0Sc*L_IyCmd0) +
                                    (L_IyCmdA1Sc*L_IyCmdZ1));

     L_IyregV2Ff = L_LTotCal*L_IxMeld*L_PllWNom;



     // Form iyreg delta for integrator
     L_IyregZErr   = L_IyCmdZ - L_IyFbk;
     L_Iy_Vreg_Del_Int = L_IyregZErr*PARM(L_IyregIntGn)*L_DelTm1;

     if (( L_IyregZErr >= 0.0F) && (L_InnrRegLim.L_UyLimPFlg))
     {
        L_Iy_Vreg_Del_Int = 0.0F;
     }
     else if (( L_IyregZErr < 0.0F) && (L_InnrRegLim.L_UyLimNFlg))
     {
        L_Iy_Vreg_Del_Int = 0.0F;
     }


     // Integrate and clamp
     L_IyregIyInt = L_IyregIyInt + L_Iy_Vreg_Del_Int;
     if ( L_IyregIyInt >= PARM(L_IyregIntLm) )
     {
          L_IyregIyInt = PARM(L_IyregIntLm);
     }
     else if ( L_IyregIyInt <= -PARM(L_IyregIntLm) )
     {
          L_IyregIyInt = -PARM(L_IyregIntLm);
     }

     L_IyregVOut = L_IyregIyInt + L_IyCmd0*PARM(L_IyregCmdGn) + L_IyregPrp2 -
                   L_IyFbk*PARM(L_IyregFbkGn) + L_IyregVFf +
                   L_IyregV2Ff;

     // Form L_IregY and check limits
     L_InnrRegLim.L_UyLimPFlg = false;
     L_InnrRegLim.L_UyLimNFlg = false;
     L_IregY      = L_IyregVOut*L_VdcFbkRcp*SQRT3;
     L_IregYUc    = L_IregY;

     if ( L_IregY >= PARM(L_IyregULim) )
     {
          L_IregY = PARM(L_IyregULim);
          L_InnrRegLim.L_UyLimPFlg = true;
     }
     else if ( L_IregY <= -PARM(L_IyregULim) )
     {
          L_IregY = -PARM(L_IyregULim);
          L_InnrRegLim.L_UyLimNFlg = true;
     }

     L_IyregSpill = L_IregYUc - L_IregY;


     //                     Common current regulator code
     //--------------------------------------------------

     //                              Resonator
     //--------------------------------------------------


     //  Dynamic resonator limit
     L_ResLimDyn =  L_ResLimDyn2;


     //  Demodulation
     L_ResIx2 = GmCos2Ang*L_IxregZErr - GmSin2Ang*L_IyregZErr;
     L_ResIy2 = GmSin2Ang*L_IxregZErr + GmCos2Ang*L_IyregZErr;

     //    Current Integration and Clamp
     L_IxregResX = L_IxregResX + L_DelTm1*L_ResIx2;
     if ( L_IxregResX > L_ResLimDyn)
     {
          L_IxregResX = L_ResLimDyn;
     }
     if ( L_IxregResX < -L_ResLimDyn)
     {
          L_IxregResX = -L_ResLimDyn;
     }

     L_IyregResY = L_IyregResY + L_DelTm1*L_ResIy2;
     if ( L_IyregResY > L_ResLimDyn)
     {
          L_IyregResY = L_ResLimDyn;
     }
     if ( L_IyregResY < -L_ResLimDyn)
     {
          L_IyregResY = -L_ResLimDyn;
     }

     //          Rotator Input Calculation
     L_ResX2 = PARM(L_Res2ndD)*(L_IxregResX + L_NsXStep) + PARM(L_Res2ndQ)*(L_IyregResY + L_NsYStep);
     L_ResY2 = PARM(L_Res2ndD)*(L_IyregResY + L_NsYStep) - PARM(L_Res2ndQ)*(L_IxregResX + L_NsXStep);

     //         Rotation to Output
     L_ResXOut = GmCos2Ang*L_ResX2  + GmSin2Ang*L_ResY2;
     L_ResYOut = -GmSin2Ang*L_ResX2 + GmCos2Ang*L_ResY2;

     //        Final Resonator Output
     L_IxregRsOut = PARM(L_IregResWI)*L_ResXOut;
     L_IyregRsOut = PARM(L_IregResWI)*L_ResYOut;


     //    DC Current Regulator  Output
     L_DcFndXT1 = L_DcFndX;
     L_DcFndYT1 = L_DcFndY;
     L_DcXOut =  GmCosAngFbk*L_DcFndXT1 + GmSinAngFbk*L_DcFndYT1;
     L_DcYOut = -GmSinAngFbk*L_DcFndXT1 + GmCosAngFbk*L_DcFndYT1;
     L_IxregDcOut = PARM(L_DcRegWI)*L_DcXOut;
     L_IyregDcOut = PARM(L_DcRegWI)*L_DcYOut;


     //                   Final Ireg output
     //--------------------------------------------------

     L_UxCmd      = L_IregX + L_IxregRsOut + L_IxregDcOut;

     L_UyCmd      = L_IregY + L_IyregRsOut + L_IyregDcOut;

     //                   Delay models
     //--------------------------------------------------

     /* Form more ireg delay models */
     /*  Form ireg delay models */
     L_IxCmdZ2    = L_IxCmdZ1;
     L_IxCmdZ1    = L_IxCmd0;

     L_IyCmdZ2    = L_IyCmdZ1;
     L_IyCmdZ1    = L_IyCmd0;



     // Clear and populate summary limit flag
     L_InnrRegLim.L_XAntiWndP = false;
     L_InnrRegLim.L_XAntiWndN = false;
     if (L_InnrRegLim.L_UxlimPFlg &&(PARM(L_IregSpilGn)==0.0F ||
         L_InnrRegLim.L_IyLimPFlg))
     {
         L_InnrRegLim.L_XAntiWndP = true;
     }
     else if (L_InnrRegLim.L_UxlimNFlg &&(PARM(L_IregSpilGn)==0.0F ||
         L_InnrRegLim.L_IyLimNFlg))
     {
         L_InnrRegLim.L_XAntiWndN = true;
     }


     // Diagnostic only
     L_VxErrFbk = L_UxCmdz2-L_VxFbk;
     L_UxCmdz2 = L_UxCmdz1;
     L_UxCmdz1 = L_UxCmd*L_VdcFbk*RAD3_3;

     L_VyErrFbk = L_UyCmdz2-L_VyFbk;
     L_UyCmdz2 = L_UyCmdz1;
     L_UyCmdz1 = L_UyCmd*L_VdcFbk*RAD3_3;


     L_ModIndex = sqrtf(L_UxCmd*L_UxCmd + L_UyCmd*L_UyCmd);

     return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//    This function calculates task 2 vars needed by the XY current regulator.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void LineCurrentRegCalcsT2()
{

    if ( L_Running && (GmDscmVMagN >=  PARM(L_PhImbIpThr))  )
    {
        L_PhImbIpTmr = L_PhImbIpTmr + DelTm2;
        if ( L_PhImbIpTmr > PARM(L_PhImbIpTm) )
        {
            L_PhImbIp    = true;
            L_PhImbIpTmr = PARM(L_PhImbIpTm);
        }
    }
    else
    {
        L_PhImbIpTmr = L_PhImbIpTmr - DelTm2;
        if ( L_PhImbIpTmr <= 0.0)
        {
            L_PhImbIp    = false;
            L_PhImbIpTmr = 0.0;
        }
    }


    if (  (GmDscmVMagN < (GmDscmVMagP*PARM(L_RsLmZeroOn)))  && (L_ModIndexT2 >= PARM(L_RsLmModOn)) && L_PhImbIp )
    {
        L_RsLmZero = true;
    }
    else if ((L_ModIndexT2 < PARM(L_RsLmModOff)) || !L_PhImbIp)
    {
        L_RsLmZero = false;
    }

    if ( L_RsLmZero )
    {
        L_ResLimDyn2 = 0.0;
    }
    else
    {
        L_ResLimDyn2 = PARM(L_IregResLim);
    }


     //       Reverse Fundamental, for DC (in ac line) Demodulation
     //----------------------------------------------------------------
     if ( L_InnrRegEnb && !GmPllHot)
     {
        L_DcIx      = -L_DcIxFbk;
        L_DcIy      = -L_DcIyFbk;


        //    Current Integration and Clamp
        L_IxregDcX = L_IxregDcX + DelTm2*L_DcIx;
        if ( L_IxregDcX > PARM(L_IregDcLim) )
        {
             L_IxregDcX = PARM(L_IregDcLim);
        }
        if ( L_IxregDcX < -PARM(L_IregDcLim))
        {
             L_IxregDcX = -PARM(L_IregDcLim);
        }

        L_IyregDcY = L_IyregDcY + DelTm2*L_DcIy;
        if ( L_IyregDcY > PARM(L_IregDcLim))
        {
             L_IyregDcY = PARM(L_IregDcLim);
        }
        if ( L_IyregDcY < -PARM(L_IregDcLim) )
        {
             L_IyregDcY = -PARM(L_IregDcLim);
        }

        //          Rotator Input Calculation
        L_DcFndX = PARM(L_Dc1stD)*(L_IxregDcX + L_DcXStep) + PARM(L_Dc1stQ)*(L_IyregDcY + L_DcYStep);
        L_DcFndY = PARM(L_Dc1stD)*(L_IyregDcY + L_DcYStep) - PARM(L_Dc1stQ)*(L_IxregDcX + L_DcXStep);
     }
     else
     {
        L_IxregDcX   = 0.0;
        L_IyregDcY   = 0.0;
        L_DcFndX     = 0.0;
        L_DcFndY     = 0.0;
     }

     //    Line voltage feed-forward
     //----------------------------------------------------------------

     L_IregVnomFf = L_IregVnomFf + PARM(L_IxregWFf)*DelTm2*
                    (L_VlnIn - L_IregVnomFf);

    return;
}
