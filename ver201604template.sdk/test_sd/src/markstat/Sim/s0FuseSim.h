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

#ifndef __S0_FUSESIM_H__
#define __S0_FUSESIM_H__

// Include Files
//--------------
// system
// core
// framework

// product
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "b0BridgeRebf.h"


// Constants
//----------


// Function Prototypes
//--------------------





///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <class description>
//
///////////////////////////////////////////////////////////////////////////////
class cSimFuse
{

//-----------------------------------------------------------------------------
public:
// Types and Classes
//------------------
struct InTyp
{
    const float&    IaFbkFuse;     // 1
    const float&    IbFbkFuse;     // 2
    const float&    IcFbkFuse;     // 3
    const unsigned& FuseResetSim;  // 4
   
};

struct InsTyp
{

    const float&     DelTm1Isim;         //  1  - task1 period (sec).
    const float&	 FuseMeltTau0;
    const float&	 FuseMeltTau1;
    const float&	 FuseMeltTau2;
    const float&	 FuseMeltTau3;
    const float&	 FuseMeltGain0;
    const float&	 FuseMeltGain1;
    const float&	 FuseMeltGain2;
    const float&	 FuseMeltGain3;
    const float&	 FuseTempLim;
    const float&	 FuseIZero;
    const float&	 FuseVArc;
    const float&	 FuseInit;

};	
    cSimFuse( InTyp InList, InsTyp InsList );
   ~cSimFuse();

    // Methods
    //--------
    void RunRules();
    void FuseModel3P();
    void FuseModel1P(bool* FuseMelt, bool* FuseArc, bool* FuseOpen, float* FuseTempInt0, float* FuseTempInt1, float* FuseTempInt2, float* FuseTempInt3,
    float* FuseTemp, float* Vout, float* IphsFbkFuseZ, float IFuseFbk);
    void FuseStateGen();


    // Output Signals
    //---------------
//CREATE_PUBVAR(VoutASim,        float);
//CREATE_PUBVAR(VoutBSim,        float);
//CREATE_PUBVAR(VoutCSim,        float);

    float       VoutASim;
    float       VoutBSim;
    float       VoutCSim;

 //  
 // 

 //   // Variables
 //   //----------

//CREATE_PUBVAR(FuseMeltSim,        unsigned);
//	CREATE_PUBVAR(FuseArcSim,        unsigned);
//	CREATE_PUBVAR(FuseOpenSim,        unsigned);
//	CREATE_PUBVAR(FuseTempASim,        float);
//	CREATE_PUBVAR(FuseTempBSim,        float);
//	CREATE_PUBVAR(FuseTempCSim,        float);
    unsigned    FuseMeltSim;         // 
    unsigned    FuseArcSim;
    unsigned    FuseOpenSim;

    float       FuseTempASim;
    float       FuseTempBSim;
    float       FuseTempCSim;



//-----------------------------------------------------------------------------
private:



    // External Inputs
    //----------------
    InTyp    In;
    InsTyp   Ins;


    // Methods
    //--------

    // Diagnostics
    //------------


    // Parameters
    //-----------


	//CREATE_PARM(FuseMeltGn0, float);             // 
	//CREATE_PARM(FuseMeltGn1, float);             //
	//CREATE_PARM(FuseMeltGn2, float);             // 
	//CREATE_PARM(FuseMeltGn3, float);             //

    float FuseMeltGn0;
    float FuseMeltGn1;
    float FuseMeltGn2;
    float FuseMeltGn3;


    // Published Variables
    //--------------------


    float FuseTempIntA0;
    float FuseTempIntA1;
    float FuseTempIntA2;
    float FuseTempIntA3;
    float FuseTempIntB0;
    float FuseTempIntB1;
    float FuseTempIntB2;
    float FuseTempIntB3;
    float FuseTempIntC0;
    float FuseTempIntC1;
    float FuseTempIntC2;
    float FuseTempIntC3;



    float IphsFbkFuseAZ;
    float IphsFbkFuseBZ;
    float IphsFbkFuseCZ;
    bool    FuseMeltA;         // 
    bool    FuseArcA;
    bool    FuseOpenA;
    bool    FuseMeltB;         // 
    bool    FuseArcB;
    bool    FuseOpenB;
    bool    FuseMeltC;         // 
    bool    FuseArcC;
    bool    FuseOpenC;
//	CREATE_PUBVAR(IaFbkSim,        float);
//	CREATE_PUBVAR(IbFbkSim,        float);
//	CREATE_PUBVAR(IcFbkSim,        float);

    float   IaFbkSim;
    float   IbFbkSim;
    float   IcFbkSim;

    // Unpublished Variables
    //----------------------

    // Data Passing
    //-------------

};



// Inline Function Definitions
//----------------------------


#endif  // 
