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

#ifndef __S0_RCSNUBSIM_H__
#define __S0_RCSNUBSIM_H__

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
class cRCSnubSim
{

//-----------------------------------------------------------------------------
public:
// Types and Classes
//------------------
struct InTyp
{
    const float&    IxRCSim;     // 1
    const float&    IyRCSim;     // 2
    const float&    VxRCInSim;     // 1
    const float&    VyRCInSim;     // 1
    const float&    WElecSim;     // 2
    const float&    IaRCSim;     // 1
    const float&    IbRCSim;     // 2
    const float&    IcRCSim;     // 2
    const float&    VaRCInSim;     // 1
    const float&    VbRCInSim;     // 1
    const float&    VcRCInSim;     // 1
    const unsigned int&    PhaseState;     // 2

   
};

struct InsTyp
{

    const float&      DelTm1;
    const float&     CrcSim;         //  1  - .
    const float&	 RrcSim;

	

};	


    cRCSnubSim( InTyp InList, InsTyp InsList );
   ~cRCSnubSim();

    // Methods
    //--------
    void RunRules();
	void RCWork();
    void RCNoWork();
	void RCWorkABC();
	void RCNoWorkABC();


    // Output Signals
    //---------------
    CREATE_PUBVAR(QxRCSim,        float);
    CREATE_PUBVAR(QyRCSim,        float);
    CREATE_PUBVAR(VxRCCapSim,        float);
    CREATE_PUBVAR(VyRCCapSim,        float);
    CREATE_PUBVAR(VxRCSim,        float);
    CREATE_PUBVAR(VyRCSim,        float);
    CREATE_PUBVAR(LCrcRcpSim,        float);
    CREATE_PUBVAR(RrcRcpSim,        float);

    CREATE_PUBVAR(VaRCCapSim,        float);
    CREATE_PUBVAR(VbRCCapSim,        float);
    CREATE_PUBVAR(VcRCCapSim,        float);
    CREATE_PUBVAR(VaRCSim,        float);
    CREATE_PUBVAR(VbRCSim,        float);
    CREATE_PUBVAR(VcRCSim,        float);

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
