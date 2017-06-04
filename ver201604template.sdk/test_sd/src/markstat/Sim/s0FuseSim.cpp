///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Fuse Isim class.
//
// DESCRIPTION
//      This file contains the definition for the class.
//
// COPYRIGHT    Copyright (c) 2011
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////
// Date        Name             Description  
// ==========  ===============  ===============================================
// 2014-11-26  Bo Qu           Initial created.
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------

// system
// core
// framework
// product
#include "s0FuseSim.h"



// Constants
//----------


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
cSimFuse::cSimFuse
(
    InTyp    InList,
    InsTyp   InsList
)
    : In(InList), Ins(InsList)
{

    FuseMeltA     = 0;         // 
    FuseArcA   = 0;
    FuseOpenA     = 0;
    FuseTempASim     = 0.0;

    FuseMeltB = 0;         // 
    FuseArcB = 0;
    FuseOpenB = 0;
    FuseTempBSim = 0.0;

    FuseMeltC = 0;         // 
    FuseArcC = 0;
    FuseOpenC = 0;
    FuseTempCSim = 0.0;
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
cSimFuse::~cSimFuse ()
{
    
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <method description here>
//
// EXECUTION LEVEL
//      Task #
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimFuse::RunRules()
{
	//SET_FV(FuseMeltGn0, 1.0 / INS(FuseMeltTau0) * INS(DelTm1Isim));
	//SET_FV(FuseMeltGn1, 1.0 / INS(FuseMeltTau1) * INS(DelTm1Isim));
	//SET_FV(FuseMeltGn2, 1.0 / INS(FuseMeltTau2) * INS(DelTm1Isim));
	//SET_FV(FuseMeltGn3, 1.0 / INS(FuseMeltTau3) * INS(DelTm1Isim));

    FuseMeltGn0 = 1.0 / INS(FuseMeltTau0) * INS(DelTm1Isim);
    FuseMeltGn1 = 1.0 / INS(FuseMeltTau1) * INS(DelTm1Isim);
    FuseMeltGn2 = 1.0 / INS(FuseMeltTau2) * INS(DelTm1Isim);
    FuseMeltGn3 = 1.0 / INS(FuseMeltTau3) * INS(DelTm1Isim);
    FuseTempIntA0 = INS(FuseInit);
    FuseTempIntA1 = INS(FuseInit);
    FuseTempIntA2 = INS(FuseInit);
    FuseTempIntA3 = INS(FuseInit);
    FuseTempIntB0 = INS(FuseInit);
    FuseTempIntB1 = INS(FuseInit);
    FuseTempIntB2 = INS(FuseInit);
    FuseTempIntB3 = INS(FuseInit);
    FuseTempIntC0 = INS(FuseInit);
    FuseTempIntC1 = INS(FuseInit);
    FuseTempIntC2 = INS(FuseInit);
    FuseTempIntC3 = INS(FuseInit);


} 
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <method description here>
//
// EXECUTION LEVEL
//      Task #
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimFuse::FuseStateGen()
{
    FuseMeltSim = 0;
    FuseArcSim = 0;
    FuseOpenSim = 0;
	
    FuseMeltSim = FuseMeltA + FuseMeltB * 2 + FuseMeltC * 4;
    FuseArcSim = FuseArcA + FuseArcB * 2 + FuseArcC * 4;
    FuseOpenSim = FuseOpenA + FuseOpenB * 2 + FuseOpenC * 4;

}
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <method description here>
//
// EXECUTION LEVEL
//      Task #
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimFuse::FuseModel3P()
{
    IaFbkSim = IN(IaFbkFuse);
    IbFbkSim = IN(IbFbkFuse);
    IcFbkSim = IN(IcFbkFuse);
    FuseModel1P(&FuseMeltA, &FuseArcA, &FuseOpenA, &FuseTempIntA0, &FuseTempIntA1, &FuseTempIntA2, &FuseTempIntA3,
	    &FuseTempASim, &VoutASim, &IphsFbkFuseAZ, IaFbkSim);
    FuseModel1P(&FuseMeltB, &FuseArcB, &FuseOpenB, &FuseTempIntB0, &FuseTempIntB1, &FuseTempIntB2, &FuseTempIntB3,
	    &FuseTempBSim, &VoutBSim, &IphsFbkFuseBZ, IbFbkSim);
    FuseModel1P(&FuseMeltC, &FuseArcC, &FuseOpenC, &FuseTempIntC0, &FuseTempIntC1, &FuseTempIntC2, &FuseTempIntC3,
	    &FuseTempCSim, &VoutCSim, &IphsFbkFuseCZ, IcFbkSim);
    FuseStateGen();

}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Isim Fuse model.
//
// EXECUTION LEVEL
//      Task #
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimFuse::FuseModel1P(bool* FuseMelt, bool* FuseArc, bool* FuseOpen, float* FuseTempInt0, float* FuseTempInt1, float* FuseTempInt2, float* FuseTempInt3,
	float* FuseTemp, float* Vout, float* IphsFbkFuseZ, float IFuseFbk)
{
    float IFuseSqr = 0.0;

    IFuseSqr = IFuseFbk * IFuseFbk;
    if (IN(FuseResetSim))
    {
        *FuseTempInt0 = *FuseTempInt1 = *FuseTempInt2 = *FuseTempInt3 = INS(FuseInit);
    }
    else
    {

	    *FuseTempInt0 += FuseMeltGn0 * (IFuseSqr - *FuseTempInt0);
	    *FuseTempInt1 += FuseMeltGn1 * (IFuseSqr - *FuseTempInt1);
	    *FuseTempInt2 += FuseMeltGn2 * (IFuseSqr - *FuseTempInt2);
	    *FuseTempInt3 += FuseMeltGn3 * (IFuseSqr - *FuseTempInt3);
    }


    *FuseTemp = INS(FuseMeltGain0) * (*FuseTempInt0) + INS(FuseMeltGain1) * (*FuseTempInt1)
                + INS(FuseMeltGain2) * (*FuseTempInt2) + INS(FuseMeltGain3) * (*FuseTempInt3);

    if (IN(FuseResetSim))
    {
        *FuseMelt = FALSE;
    }
    else if ((*FuseTemp > INS(FuseTempLim)) || *FuseMelt)
    {
        *FuseMelt = TRUE;			
    }
    else
    {
        *FuseMelt = FALSE;
    }
    if ((INS(FuseIZero) < ABS(IFuseFbk)) && (*IphsFbkFuseZ * IFuseFbk >= 0.0) && *FuseMelt)
    {
        *FuseArc = TRUE;
    }
    else
    {
        *FuseArc = FALSE;
    }
    if ((!(*FuseArc)) && *FuseMelt)
    {
        *FuseOpen = TRUE;
    }
    else
    {
        *FuseOpen = FALSE;
    }
    if (*FuseArc)
    {
        if (IFuseFbk > 0)
        {
            *Vout = INS(FuseVArc);
        }
        else
        {
            *Vout = -INS(FuseVArc);
        }
    }
    else
    {
        *Vout = 0.0;
    }	
	
    *IphsFbkFuseZ = IFuseFbk;
	    
   
}

