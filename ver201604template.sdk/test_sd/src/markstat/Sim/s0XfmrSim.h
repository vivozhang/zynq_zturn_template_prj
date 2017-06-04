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

#ifndef __S0_XFMRSIM_H__
#define __S0_XFMRSIM_H__

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


#define SAT_CURVE 1
#define SAT_IDEAL 2



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <class description>
//
///////////////////////////////////////////////////////////////////////////////
class cXfmrSim
{

//-----------------------------------------------------------------------------
public:
// Types and Classes
//------------------
struct InTyp
{
    const float&    VaXfmrPriSim;     // 1
    const float&    VbXfmrPriSim;     // 2
    const float&    VcXfmrPriSim;     // 1
    const float&    VaXfmrSecSim;     // 1
    const float&    VbXfmrSecSim;     // 2
    const float&    VcXfmrSecSim;     // 1
    const float&    WElecSim;     // 2
    const unsigned& XfmrState;
    const unsigned& IsimSmplN;
    const unsigned& BRchg;
    
 
   
};

struct InsTyp
{

    const float&     DelTm1;
    const float&     XfmrRatioSim;        //  1  - .     
    const float&     VxfmrPri;            //  2  - xmfr primary voltage (from product)     (volt)
    const float&     LxfmrPri;            //  3  - xmfr primary inductance (from product)  (Henrys)
    const float&     RxfmrPri;            //  4  - xmfr primary resistance (from product)  (Ohm)
    const float&     L_VxfmrSec;          //  5  - xmfr secondary line converter winding voltage (from product)  (volt)
    const float&     L_LxfmrSec;          //  6  - xmfr secondary line converter winding inductance (from product) (Henrys)
    const float&     L_RxfmrSec;          //  7  - xmfr secondary line converter winding resistance (from product)  (Ohm)
    const float&     RhfXfmrPuSim;        //  8 
    const float&     RmXfmrSim;           //  9
 
    const unsigned&  XfmrSatEnb;
    const unsigned&  XfmrSatCrvType;
    const float& XfmrWElec;
    const float& XfmrKVASim;
    const unsigned&  L_XfmrSatCurvNumSim;  
    const float& L_XfmrSatCurvFlxK0Sim;   
    const float& L_XfmrSatCurvFlxK1Sim;   
    const float& L_XfmrSatCurvFlxK2Sim;   
    const float& L_XfmrSatCurvFlxK3Sim;   
    const float& L_XfmrSatCurvFlxK4Sim;   
    const float& L_XfmrSatCurvFlxK5Sim;   
    const float& L_XfmrSatCurvFlxK6Sim;   
    const float& L_XfmrSatCurvFlxK7Sim;   
    const float& L_XfmrSatCurvFlxK8Sim;   
    const float& L_XfmrSatCurvFlxK9Sim;   
    const float& L_XfmrSatCurvImagK0Sim;   
    const float& L_XfmrSatCurvImagK1Sim;   
    const float& L_XfmrSatCurvImagK2Sim;   
    const float& L_XfmrSatCurvImagK3Sim;   
    const float& L_XfmrSatCurvImagK4Sim;   
    const float& L_XfmrSatCurvImagK5Sim;   
    const float& L_XfmrSatCurvImagK6Sim;   
    const float& L_XfmrSatCurvImagK7Sim;   
    const float& L_XfmrSatCurvImagK8Sim;   
    const float& L_XfmrSatCurvImagK9Sim;

    const float& L_XfmrSatThr;   
    const float& L_XfmrSatXm;
        const float& L_FlxAInit;   
    const float& L_FlxBInit;
       const float& L_FlxCInit;
              const float& L_RmagXfmrSim;

    
};	
    cXfmrSim( InTyp InList, InsTyp InsList );
   ~cXfmrSim();

    // Methods
    //--------
    void RunRules();
    void XfmrModel();
    void XfmrSatCurve(float* L_XfmrImagSim, float L_VXfmrMagSim, float*  XfmrFlxSim);


    // Output Signals
    //---------------
    CREATE_PUBVAR(VaXfmrPri2SecSim,       float);
    CREATE_PUBVAR(VbXfmrPri2SecSim,       float);
    CREATE_PUBVAR(VcXfmrPri2SecSim,       float);
    CREATE_PUBVAR(XfmrLeakFlxASim,        float);
    CREATE_PUBVAR(XfmrLeakFlxBSim,        float);
    CREATE_PUBVAR(XfmrLeakFlxCSim,        float);
    CREATE_PUBVAR(IaLeakSim,              float);
    CREATE_PUBVAR(IbLeakSim,              float);
    CREATE_PUBVAR(IcLeakSim,              float);
    CREATE_PUBVAR(IaLeakRhfSim,           float);
    CREATE_PUBVAR(IbLeakRhfSim,           float);
    CREATE_PUBVAR(IcLeakRhfSim,           float);
    CREATE_PUBVAR(IaXfmrLossSim,          float);
    CREATE_PUBVAR(IbXfmrLossSim,          float);
    CREATE_PUBVAR(IcXfmrLossSim,          float);
    CREATE_PUBVAR(IaXfmrPri2SecSim,       float);
    CREATE_PUBVAR(IbXfmrPri2SecSim,       float);
    CREATE_PUBVAR(IcXfmrPri2SecSim,       float);
    
    CREATE_PUBVAR(IaXfmrPriSim,           float);
    CREATE_PUBVAR(IbXfmrPriSim,           float);
    CREATE_PUBVAR(IcXfmrPriSim,           float);
    CREATE_PUBVAR(LIaXfmrSecSim,          float);
    CREATE_PUBVAR(LIbXfmrSecSim,          float);
    CREATE_PUBVAR(LIcXfmrSecSim,          float);
    CREATE_PUBVAR(IaXfmrMagSim,           float);
    CREATE_PUBVAR(IbXfmrMagSim,           float);
    CREATE_PUBVAR(IcXfmrMagSim,           float);
    CREATE_PUBVAR(VaXfmrPriFbkSim,        float);
    CREATE_PUBVAR(VbXfmrPriFbkSim,        float);
    CREATE_PUBVAR(VcXfmrPriFbkSim,        float);
    CREATE_PUBVAR(VaXfmrSecFbkSim,        float);
    CREATE_PUBVAR(VbXfmrSecFbkSim,        float);
    CREATE_PUBVAR(VcXfmrSecFbkSim,        float);

    CREATE_PUBVAR(RhfXfmrSim,             float);
    
    CREATE_PUBVAR(XfmrFlxASim,            float);
    CREATE_PUBVAR(XfmrFlxBSim,            float);
    CREATE_PUBVAR(XfmrFlxCSim,            float);
    CREATE_PUBVAR(XfmrFlxAPuSim,            float);
    CREATE_PUBVAR(XfmrFlxBPuSim,            float);
    CREATE_PUBVAR(XfmrFlxCPuSim,            float);


    CREATE_PARM(VxfmrPriSim,        float);             // Simulated xfmr primary voltage (Vrms)
    CREATE_PARM(LxfmrPriSim,        float);             // Simulated xfmr primary inductance
    CREATE_PARM(RxfmrPriSim,        float);             // Simulated xfmr primary resistance
    CREATE_PARM(LnVxfmrSecSim,      float);             // Simulated xfmr secondary line winding voltage (Vrms)
    CREATE_PARM(LnRxfmrSecSim,      float);             // Simulated xfmr secondary line winding resistance
    CREATE_PARM(LnLxfmrSecSim,      float);             // Simulated xfmr secondary line winding inductance
    CREATE_PARM(LnRxfmrPriSim,      float);             // Simulated xfmr primary resistance referring to line winding 
    CREATE_PARM(LnLxfmrPriSim,      float);             // Simulated xfmr primary inductance  referring to line winding 
    CREATE_PARM(LnLeakSim,          float);             // Simulated xfmr total leakage at secondary side
    CREATE_PARM(XfmrFlxRat,         float);            
    CREATE_PARM(XfmrIdRat,          float);             

 //  
 // 

 //   // Variables
 //   //----------

//transformer variables
float XfmrFlxGroup[10];
float XfmrImagGroup[10];
float XfmrRampRat[9];




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
