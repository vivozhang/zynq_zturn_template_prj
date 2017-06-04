///////////////////////////////////////////////////////////////////////////////
//
// TITLE        RC Snubber Isim class.
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
// 2015-03-24  Bo Qu           Initial created.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __S0_SERIESCMPSIM_H__
#define __S0_SERIESCMPSIM_H__

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
class cSeriesCmpSim
{

//-----------------------------------------------------------------------------
public:
// Types and Classes
//------------------
struct InTyp
{
    const unsigned&     SCompEnb;         //  1  - . 
    const float&    VinSim;     // 1
    const float&    VoutSim;     // 1
    const float&    WElecSim;     // 2
    const float&    IinSim;     // 1

   
};

struct InsTyp
{

    const float&      DelTm1;
    const float& OpnCurThrs;

	

};	


    cSeriesCmpSim( InTyp InList, InsTyp InsList );
   ~cSeriesCmpSim();

    // Methods
    //--------
    void RunRules();
	void StateUpdate();
    void SCompRun();



    // Output Signals
    //---------------
    CREATE_PUBVAR(SCompCmd, unsigned);

    CREATE_PUBVAR(LSCompRcpSim, float);
    CREATE_PUBVAR(RhfCompRcpSim, float);
    CREATE_PUBVAR(Ibypass, float);
    CREATE_PUBVAR(IbypassZ, float);
    CREATE_PUBVAR(SCompState, unsigned);

 

    CREATE_PUBVAR(Vscmid, float);
    CREATE_PUBVAR(Vsccap, float);
    CREATE_PUBVAR(SCompFlxSim, float);
    CREATE_PUBVAR(IscompLSim, float);
    CREATE_PUBVAR(IscompCSim, float);
    CREATE_PUBVAR(Iout, float);
 
    
    


    CREATE_PUBVAR(ImovSim, float);             //  

 //  
 // 

 //   // Variables
 //   //----------

    float VsccapGroup[11];
    float ImovGroup[11];
    float ImovRampRat[10];




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



	

    // Unpublished Variables
    //----------------------

    // Data Passing
    //-------------

};



// Inline Function Definitions
//----------------------------


#endif  // 
