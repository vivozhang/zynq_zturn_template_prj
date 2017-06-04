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

#ifndef __S0_STARTUPSIM_H__
#define __S0_STARTUPSIM_H__

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
class cStartUpSim
{

//-----------------------------------------------------------------------------
public:
// Types and Classes
//------------------
struct InTyp
{
    const float&    VaIn1Sim;     // 1
    const float&    VbIn1Sim;     // 2
    const float&    VcIn1Sim;     // 1
    const float&    VaIn2Sim;     // 1
    const float&    VbIn2Sim;     // 2
    const float&    VcIn2Sim;     // 1

         
   
};

struct InsTyp
{

    const float&      DelTm1;
   
    const unsigned&  StartEnb;
    const float&      OpnCurThr;

    const float&      LStartSim;    // Simulated start-up inductance referring to line side (Henrys)
    const float&      RStartSim;    // Simulated start-up reistance referring to line side (Ohms)
   
    
};	
    cStartUpSim( InTyp InList, InsTyp InsList );
   ~cStartUpSim();

    // Methods
    //--------
    void RunRules();
    void StateUpdate();
    void StartUp();


    // Output Signals
    //---------------
    CREATE_PUBVAR(StartState    ,unsigned);         // start-up state
    CREATE_PUBVAR(IaStSim    ,float);         // simulated phase A current in startup circuit        
    CREATE_PUBVAR(IbStSim    ,float);         //  simulated phase B current in startup circuit   
    CREATE_PUBVAR(IcStSim    ,float);         //  simulated phase C current in startup circuit
    CREATE_PUBVAR(IaStSimZ    ,float);         // simulated phase A current in startup circuit  (last time)      
    CREATE_PUBVAR(IbStSimZ    ,float);         //  simulated phase B current in startup circuit (last time)  
    CREATE_PUBVAR(IcStSimZ    ,float);         //  simulated phase C current in startup circuit (last time)
    CREATE_PUBVAR(StartFlxASim    ,float);         // simulated phase A flux of start-up circuit       
    CREATE_PUBVAR(StartFlxBSim    ,float);         //  simulated phase B flux of start-up circuit   
    CREATE_PUBVAR(StartFlxCSim    ,float);         //  simulated phase C flux of start-up circuit  

    CREATE_PUBVAR(LStartRcpSim    ,float);         //  Rcp of start-up circuit inductance 
    CREATE_PUBVAR(StartTime    ,float);         // start-up time

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
