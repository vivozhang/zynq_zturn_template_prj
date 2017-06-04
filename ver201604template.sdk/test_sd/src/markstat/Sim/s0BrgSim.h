///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Product initializaion
//
// DESCRIPTION:
//
// COPYRIGHT:   Copyright (c) 2009
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __SO_BRG_SIM_H__
#define __SO_BRG_SIM_H__


// Include Files:
//---------------
// system
// core
#include "MonitorCommand.h"
// framework
#include "x0FrameWork.h"
#include "x0BridgeInterface.h"
// product
#include "Product.h"


// Constants:
//-----------


// Types and Classes:
//-------------------




///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <class description>
//
///////////////////////////////////////////////////////////////////////////////
class cBrgSim
{

//-----------------------------------------------------------------------------
public:

    // Constants
    //----------
    static const unsigned  NUM_INST_SUPPORT = 2;

    // Types
    //------

    // Methods
    //--------
    cBrgSim( void );
    ~cBrgSim( void );

    float SimPhaseVoltAvg ( PhsTransListTyp& PhaseTraList, float Vdc,
                            unsigned Phase, unsigned PwmFull, float PwmFullRcp );
                            
    void PhasePwmUpDnTm( PhsTransListTyp& PhaseTraList, unsigned Phase, unsigned PwmFull );       //CL, 2014-01-22                 

    void SetVcoFzro  ( float value )     { VcoFzroSim  = value; }
    void SetVcoFmax  ( float value )     { VcoFmaxSim  = value; }
    void SetVcoFmin  ( float value )     { VcoFminSim  = value; }
    void SetVcoMask  ( unsigned value )  { VcoMaskSim  = value; }
    void SetVcoGain  ( float value )     { VcoGainSim  = value; }
    void SetVcoDelTm ( float value )     { VcoDelTmSim = value; }

    unsigned Avg2Vco ( float SigAvg, unsigned& SigVcoCnt, float& VcoCntRes );

    void SetEncMask  ( unsigned value )  { EncMaskSim  = value; }
    void SetEncGain  ( float value )     { EncGainSim  = value; }
    void SetEncDelTm ( float value )     { EncDelTmSim = value; }

    unsigned Avg2Enc ( float SpdAvg, unsigned& SpdEncCnt, float& EncCntRes );

    static int  LlmInit ( cMonitorCommand& gMonitor );
    static int  LlmForceBrgFlt ( cParamString& OutputBuffer, cParamString& CommandLineArgs );

    // Output Signals
    //---------------
    unsigned  PwmUpTm;  //CL, 2014-01-22
    unsigned  PwmDnTm;
        
    //  Force table structure
    //  ---------------------
    //  Rotor --> PwrOk   CI09   CI10   Brg1_IocPhsA   Brg1_DsatPhsApos   Brg1_DsatPhsAneg   Brg2_IocPhsA   Brg2_DsatPhsApos   Brg2_DsatPhsAneg
    //  Line  --> PwrOk   CI09   CI10   Brg1_IocPhsA   Brg1_DsatPhsApos   Brg1_DsatPhsAneg   Brg2_IocPhsA   Brg2_DsatPhsApos   Brg2_DsatPhsAneg
    static unsigned         LlmForceTable[NUM_INST_SUPPORT][9];

//-----------------------------------------------------------------------------
private:

    // External Interfaces
    //--------------------

    // Constants
    //----------

    // Methods
    //--------

    // Diagnostics
    //------------

    // Parameters
    //-----------

    // Published Variables
    //--------------------

    // Unpublished Variables
    //----------------------
    float               VcoFzroSim;
    float               VcoFmaxSim;
    float               VcoFminSim;
    unsigned            VcoMaskSim;
    float               VcoGainSim;
    float               VcoDelTmSim;

    unsigned            EncMaskSim;
    float               EncGainSim;
    float               EncDelTmSim;

    static unsigned     LlmBrgSelect;   //  Rotor Brg1, Brg2, or Line Brg1, Brg2

    // Data Passing
    //-------------

};

#endif  // __SO_BRG_SIM_H__
