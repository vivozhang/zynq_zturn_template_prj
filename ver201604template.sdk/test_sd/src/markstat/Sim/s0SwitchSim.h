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
// 2015-01-14  Bo Qu           Initial created.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __S0_SWITCHSIM_H__
#define __S0_SWITCHSIM_H__

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
class cSwitchSim
{

//-----------------------------------------------------------------------------
public:
// Types and Classes
//------------------
struct InTyp
{
    const unsigned&    SwitchCmd;     // 1


   
};

struct InsTyp
{

    const float&      DelTm1;
    const float&     OpnDlyTm;         //  1  - .
    const float&	 ClsDlyTm;
    const unsigned&	 SwitchForce;
    const unsigned&    ForceSwCmd;     // 


	

};	
    cSwitchSim( InTyp InList, InsTyp InsList );
   ~cSwitchSim();

    // Methods
    //--------
    void RunRules();
	void SwitchModel();



    // Output Signals
    //---------------
//CREATE_PUBVAR(VoutASim,        float);
//CREATE_PUBVAR(VoutBSim,        float);
//CREATE_PUBVAR(VoutCSim,        float);

    unsigned SwitchState;
    float SwitchActTmr;
    unsigned OpnProcess;
    unsigned ClsProcess;


 //  
 // 

 //   // Variables
 //   //----------






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
