///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Brg Isim class.
//
// DESCRIPTION
//      This file contains the definition for the class.
//
// COPYRIGHT    Copyright (c) 2014
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      <related file list>
//
///////////////////////////////////////////////////////////////////////////////

#ifndef BRGSIM_H
#define BRGSIM_H


// Include Files
//--------------


// Constants
//----------

#define LEG_OFF    0
#define UPPER_ON   1
#define LOWER_ON   -1

#define BRG_NO_COND    0
#define BRG_BC_COND    1
#define BRG_AC_COND    2
#define BRG_AB_COND    3
#define BRG_ABC_COND   4
#define BRG_ERROR  5

// Function Prototypes
//--------------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <class description>
//
///////////////////////////////////////////////////////////////////////////////
class cSimBrg
{

//-----------------------------------------------------------------------------
public:

	
// Types and Classes
//------------------
    struct SimBrgInTyp
    {
        const float&            VaCmdSim;           //  1  - l-n phase A voltage command         (V l-n)
        const float&            VbCmdSim;           //  2  - l-n phase B voltage command         (V l-n)
        const float&            VcCmdSim;           //  3  - l-n phase C voltage command         (V l-n)
  
        const float&            VanSrcSim;          //  4  - Bridge source voltage A 
        const float&            VbnSrcSim;          //  5  - Bridge source voltage B  
        const float&            VcnSrcSim;          //  6  - Bridge source voltage C   
    
        const float&            IaBrgSim;           //  7  - phase A current
        const float&            IbBrgSim;           //  8  - phase B current
        const float&            IcBrgSim;           //  9  - phase C current
   
        const float&            VdcSim;             //  10  - DC voltage     
        const unsigned&         BrgEnbT1;           //  11  - bridge IGBT enable
 
        const unsigned&         AiocStat;           //  12 - phase A IOC state
        const unsigned&         BiocStat;           //  13 - phase B IOC state 
        const unsigned&         CiocStat;           //  14 - phase C IOC state

        const float&            BrgLossPwr;         //  15 - bridge power loss 
   
    };

    struct SimBrgInsTyp
    {
        const float&        DelTm1;         //  1 - task1 period (sec).
    };

 

    cSimBrg( SimBrgInTyp InList, SimBrgInsTyp InsList );
    ~cSimBrg();

    // Methods
    //--------
    // void RunRules();
    void  ConvBrgSim(void);


    // Output Signals
    //---------------
    float    VabBrgSim;     // bridge voltage A to B feedback simulated (volt l-l)
    float    VbcBrgSim;     // bridge voltage B to C feedback simulated (volt l-l)
    float    IdcBrgSim;     // bridge DC current feedback simulated (volt l-l)
  
    // Diagnostics
    //------------
    int      SaBrgSim;      // Phase A diode switching states  
    int      SbBrgSim;	    // Phase B diode switching states  
    int      ScBrgSim;      // Phase C diode switching states  

    float    VaBrgSim;      // Phase A bridge voltage
    float    VbBrgSim;      // Phase B bridge voltage
    float    VcBrgSim;      // Phase C bridge voltage


//-----------------------------------------------------------------------------
private:


    // External Inputs
    //----------------
    SimBrgInTyp    In;
    SimBrgInsTyp   Ins;

    // Methods
    //--------
    void  BrgAcCurSgn(void);

    void  PwmIocMode(void);
    void  DiodeMode(void);
    void  BrgVoltsFbk(void);
    void  BrgDcCurSim(void);
    
    // Diagnostics
    //------------

    // Variables

    unsigned    OpenPhaFltSim;  // Open phase fault states
    unsigned    OpenPhbFltSim;
    unsigned    OpenPhcFltSim;
 
    float       VdcSimBy2;
    
    int         IaSgnSim;    // Phase current direction signs
    int         IbSgnSim;
    int         IcSgnSim;	 
    int         IaSgnSimZ1; 
    int         IbSgnSimZ1;
    int         IcSgnSimZ1;

    float 	    VanCmdSim;    //bridge command with cancelling svm common-mode component
    float  	    VbnCmdSim;
    float  	    VcnCmdSim;
    
    // Unpublished Variables
    //----------------------

    // Data Passing
    //-------------

};



// Inline Function Definitions
//----------------------------


#endif  // 
