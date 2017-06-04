///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Transformer Isim class.
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

#ifndef __S0_SHORTSIM_H__
#define __S0_SHORTSIM_H__

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
class cShortSim
{

//-----------------------------------------------------------------------------
public:
// Types and Classes
//------------------
struct InTyp
{
    const unsigned&     ShortEnb;         //  1  - . 
    const float&    VaInSim;     // 1
    const float&    VbInSim;     // 2
    const float&    VcInSim;     // 1
    const float&    VaShortInSim;     // 1
    const float&    VbShortInSim;     // 2
    const float&    VcShortInSim;     // 1
	const float&    IaSptL;
	const float&    IbSptL;
	const float&    IcSptL;
	const float&    IaSptR;
	const float&    IbSptR;
	const float&    IcSptR;	
};

struct InsTyp
{

    const float&      DelTm1;
    const unsigned& ShortCmdIn;
    const float& ShortTimeIn;   
    const float& OpnCurThrs;
    const unsigned& DirectShort;   
    const unsigned& LLShort;    
    const float& RShortSim;
    const float& LShortSim;
    const float& RShortllSim;
    const float& LShortllSim;
    
};	
    cShortSim( InTyp InList, InsTyp InsList );
   ~cShortSim();

    // Methods
    //--------
    void RunRules();
    void StateUpdate();
    void ShortRun();
    void ShortNotRun();


    // Output Signals
    //---------------
    CREATE_PUBVAR(IaShort,        float);
    CREATE_PUBVAR(IbShort,        float);
    CREATE_PUBVAR(IcShort,        float);
    CREATE_PUBVAR(IaShortZ,        float);
    CREATE_PUBVAR(IbShortZ,        float);
    CREATE_PUBVAR(IcShortZ,        float);
    CREATE_PUBVAR(ShortCmd,        unsigned);
 
    CREATE_PUBVAR(ShortState,        unsigned);
    CREATE_PUBVAR(ShortFlxASim,        float);
    CREATE_PUBVAR(ShortFlxBSim,        float);
    CREATE_PUBVAR(ShortFlxCSim,        float);
    CREATE_PUBVAR(VaShortSim,        float);
    CREATE_PUBVAR(VbShortSim,        float);
    CREATE_PUBVAR(VcShortSim,        float);
    CREATE_PUBVAR(ShortFlxABSim,        float);
    CREATE_PUBVAR(ShortFlxACSim,        float);
    CREATE_PUBVAR(ShortFlxBCSim,        float);
    
    


    CREATE_PUBVAR(LShortllRcpSim,         float);             // Rcp of line-line short inductance                 
    CREATE_PUBVAR(LShortRcpSim,          float);             // Rcp of phase short inductance  

       

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

// Transformer

	

    // Unpublished Variables
    //----------------------

    // Data Passing
    //-------------

};



// Inline Function Definitions
//----------------------------


#endif  // 
