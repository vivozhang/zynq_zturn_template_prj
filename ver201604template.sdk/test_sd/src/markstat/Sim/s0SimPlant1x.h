///////////////////////////////////////////////////////////////////////////////
//
// TITLE        SimPlant1x
//
// DESCRIPTION
//      This file contains the definition for the class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      s0SimPlant1x.cpp, s0SimPlant1x.xls, s0SimPlant1x.vsd
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __S0_SIM_PLANT1X_H__
#define __S0_SIM_PLANT1X_H__


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "b0BridgeRebf.h"


// Constants
//----------
// Grid event simulation state
#define GEVT_REST   0
#define GEVT_READY1 1
#define GEVT_READY2 2
#define GEVT_CLOSE  3
#define GEVT_WAIT   4
#define GEVT_OPEN1  5
#define GEVT_OPEN2  6

// grid open simulation state
#define DEFAULTSEQUENCER    62
#define DEFAULTIYREF        63
#define GRID_SYMMETRIC      0
#define GRID_ASYMMETRIC     1
#define GRID_OPEN           2

//grid event
#define GRID_HVRT 3
#define GRID_REMSHORT 4
#define GRID_LOCSHORT 5
#define GRID_SCOMP 6
#define GRID_SCOMP_RS 7
//#define GRID_STAUP 6


// Function Prototypes
//--------------------


// Types and Classes
//------------------



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This class includes all the simulated models imported from DFIGe
//      latest V01.05.06A
//
///////////////////////////////////////////////////////////////////////////////
class cSimPlant1x
{

//-----------------------------------------------------------------------------
public:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;

    // Types
    //------
    struct InTyp
    {
        // L bridge command
        const float&              L_VaBrgSim;           //  1  - line bridge l-n phase A voltage         (V l-n)
        const float&              L_VbBrgSim;           //  2  - line bridge l-n phase B voltage         (V l-n)
        const float&              L_VcBrgSim;           //  3  - line bridge l-n phase C voltage         (V l-n)
        const unsigned&           L_BrgPwrEnbT1;        //  4  - line side bridge power enable (from seq)
        const unsigned&           L_InnrRegEnbT1;       //  5  - line side inner regulator enable (from seq)
        // R bridge command
        const float&              R_VaBrgSim;           //  6  - rotor bridge l-n phase A voltage        (V l-n)
        const float&              R_VbBrgSim;           //  7  - rotor bridge l-n phase B voltage        (V l-n)
        const float&              R_VcBrgSim;           //  8  - rotor bridge l-n phase C voltage        (V l-n)
        const unsigned&           R_InnrRegEnbT1;       //  9  - rotor side inner regulator enable (from seq)
        // Contactor command
        const unsigned&           Q1Status;             //  10 - connection of line bridge is complete
        const unsigned&           S_SimK1ClsdT1;        //  11 - simulated Sync Contactor state for generator model
        const unsigned&           KdfClsCmd;            //  12 - Distortion filter close command
        //const unsigned&           L_CcstrClsCmd;        //  13 - charger contactor close command
        const unsigned&           spare1;               //  13 - spare1
        const unsigned&           AcLineIp;             //  14 - Ac line voltage is developing
        const unsigned&           L_AcLinePres;         //  15 - ac line voltage is present on line bridge
        // Other inputs
        const float&              IdbTot;               //  16 - dynamic brake total current sum of two DB currents(Amp)
        const float&              L_D_PwL_RTot;         //  17 - line bridge total diode power loss (upper and lower) (watts)
        const float&              L_BrgLossPwr;         //  18 - line bridge power loss                  (watts)
        const float&              R_BrgTotalPwr;        //  19 - rotor bridge total active power flow    (Watts)
        const float&              PwrBatChpr;           //  20 - battery chopper power 
        // Misc
        const unsigned&           GmRevSeq;             //  21 - reverse phase sequence boolean
        const unsigned&           InitDone;             //  22 - init finished
        const float&              R_LT1TmDelta;         //  23 - time delta between rotor & line task1 (sec)
        const unsigned&           FuseOpen; 
        const unsigned&           FuseArc;
        const float&              FuseVaArc;
        const float&              FuseVbArc;
        const float&              FuseVcArc;
    };

    struct InsTyp
    {
        // system rating
        const float&               TurbineNpRat;        //  1  - Turbine name plate power (watts)
        const float&               SysVoltNom;          //  2  - System voltage nominal (Vrms)
        const float&               SysFreqNom;          //  3  - System frequency nominal (Hz)
        // grid
        const float&               ShrtMVA;             //  4  - Grid short circuit capacity
        const float&               GridXOR;             //  5  - Grid XoR
        // Xfmr
        const float&               VxfmrPri;            //  6  - xmfr primary voltage (from product)     (volt)
        const float&               LxfmrPri;            //  7  - xmfr primary inductance (from product)  (Henrys)
        const float&               RxfmrPri;            //  8  - xmfr primary resistance (from product)  (Ohm)
        const float&               L_VxfmrSec;          //  9  - xmfr secondary line converter winding voltage (from product)  (volt)
        const float&               L_LxfmrSec;          //  10 - xmfr secondary line converter winding inductance (from product) (Henrys)
        const float&               L_RxfmrSec;          //  11 - xmfr secondary line converter winding resistance (from product)  (Ohm)
        // generator
        const float&               S_RRatio;            //  12 - Vs/Vr at zero spd
        const float&               R_RPMNom;            //  13 - motor nominal speed (i.e. RPM at which nominal torque yields nominal power) (RPM)
        const float&               R_PolePairs;         //  14 - generator pole pair
        const float&               S_Ls;                //  15  - L stator  refered to stator             (Henrys)
        const float&               S_Lr;                //  16  - L rotor  refered to stator              (Henrys)
        const float&               S_R1;                //  17 - r1, stator resistance                   (Ohm)
        const float&               S_R2;                //  18 - r2, stator resistance                   (Ohm)
        const float&               S_LmNom;             //  19 - nominal value of S_Lm                   (Henrys)
        const float&               Imped_Scl;           //  20  - impedance scaling for delta and wye connection
        const float&               R_SpdRefSim;         //  21  - simulator speed "nip"                  (rad/sec)
        // converter
        const float&               L_Rdf;               //  22 - distortion filter resistance           (Ohm)  
        const float&               L_Cdf;               //  23 - distortion filter capacitance           (Farad) 
        const float&               L_CTot;              //  24 - line total source capacitance           (Henrys)
        const float&               L_RTot;              //  25 - line total input resistance             (Ohm)
        const float&               L_LTot;              //  26 - line total input inductance             (Farad)
        const float&               L_RBldCap;        //  27 - DC link capacitor leakage resistance    (1/Ohm)
        const float&               L_RChg;              //  28 - line bridge charge resistor             (Ohm)
        const float&               L_VdcMax;            //  29 - maximum allowed Vdc level (volts)
        // time step
        const float&               L_DelTm1;            //  30 - line task 1 delta time                  (sec)
        const float&               L_DelTm1Rcp;         //  31 - line task 1 delta time reciprocal       (1/sec)
        const float&               R_DelTm1;            //  32 - rotor task 1 delta time                 (sec)
        const float&               R_DelTm1Rcp;         //  33 - rotor task 1 delta time reciprocal      (1/sec)
        // Misc
        const unsigned&            GEvtEnbSim;           //  34 - Enable the grid event simulation             
        const unsigned&            L_RevSeqReq;         //  35 - reverse sequence request
        
        const float&               L_IocThrs;
    };

    cSimPlant1x( InTyp InList, InsTyp InsList );
   ~cSimPlant1x();

    // Methods
    //--------
    int  RunRules(void);
    void Task1(void);
    void Task1A(void);

    // Output Signals
    //---------------

    CREATE_PUBVAR(L_VabBrgSim       ,float);            // line bridge voltage A to B feedback simulated (volt l-l)
    CREATE_PUBVAR(L_VbcBrgSim       ,float);            // line bridge voltage B to C feedback simulated (volt l-l)

    CREATE_PUBVAR(L_VanBrgSim       ,float);            // line bridge voltage A to B feedback simulated (volt l-l)
    CREATE_PUBVAR(L_VbnBrgSim       ,float);            // line bridge voltage B to C feedback simulated (volt l-l)
    CREATE_PUBVAR(L_VcnBrgSim       ,float);            // line bridge voltage B to C feedback simulated (volt l-l)
    CREATE_PUBVAR(L_IaBrgSim        ,float);            // line bridge phase A current simulated        (AMP)
    CREATE_PUBVAR(L_IbBrgSim        ,float);            // line bridge phase B current simulated        (AMP)
    CREATE_PUBVAR(L_IcBrgSim        ,float);            // line bridge phase C current simulated        (AMP)
    
    CREATE_PUBVAR(L_IaBrgInSim      ,float);            // line bridge phase A current simulated, flowing into bridge
    CREATE_PUBVAR(L_IbBrgInSim      ,float);            // line bridge phase B current simulated, flowing into bridge     
    CREATE_PUBVAR(L_IcBrgInSim      ,float);            // line bridge phase C current simulated, flowing into bridge
    
    

    CREATE_PUBVAR(L_VdcSim          ,float);            // simulated dc link voltage feedback           (volt)
    CREATE_PUBVAR(R_TachPosnSim     ,float);            // simulated tach position                      (rad)
    CREATE_PUBVAR(R_WRtrSim         ,float);            // simulated rotor angular frequency            (rad/sec)

    CREATE_PUBVAR(R_SpdSim          ,float);            // simulated speed feedback                     (rad/sec)

    CREATE_PUBVAR(R_VabBrgSim       ,float);            // rotor bridge voltage A to B feedback simulated      (volt l-l)
    CREATE_PUBVAR(R_VbcBrgSim       ,float);            // rotor bridge voltage B to C feedback simulated      (volt l-l)
    CREATE_PUBVAR(R_IaBrgSim        ,float);            // rotor bridge simulated AC line A current     (AMP)
    CREATE_PUBVAR(R_IbBrgSim        ,float);            // rotor bridge simulated AC line B current     (AMP)
    CREATE_PUBVAR(R_IcBrgSim        ,float);            // rotor bridge simulated AC line C current     (AMP)

    // Grid monitor variables (left to Q1)
    CREATE_PUBVAR(L_VaXfmrSecSim    ,float);            // simulated line converter grid monitor phase A l-n voltage (V l-n)
    CREATE_PUBVAR(L_VbXfmrSecSim    ,float);            // simulated line converter grid monitor phase B l-n voltage (V l-n)
    CREATE_PUBVAR(L_VcXfmrSecSim    ,float);            // simulated line converter grid monitor phase C l-n voltage (V l-n)
    CREATE_PUBVAR(L_IaXfmrSecSim    ,float);            // simulated line converter grid monitor l-n current (AMP)
    CREATE_PUBVAR(L_IbXfmrSecSim    ,float);            // simulated line converter grid monitor l-n current (AMP)
    CREATE_PUBVAR(L_IcXfmrSecSim    ,float);            // simulated line converter grid monitor l-n current (AMP)

    CREATE_PUBVAR(S_IaSim           ,float);            // stator phase A current simulated             (AMP)
    CREATE_PUBVAR(S_IbSim           ,float);            // stator phase B current simulated             (AMP)
    CREATE_PUBVAR(S_IcSim           ,float);            // stator phase C current simulated             (AMP)
    CREATE_PUBVAR(S_VabSim          ,float);            // stator voltage A to B feedback simulated     (volt l-l)
    CREATE_PUBVAR(S_VbcSim          ,float);            // stator voltage B to C feedback simulated     (volt l-l)

    CREATE_PUBVAR(L_IaFuseSim           ,float);            // Fuse phase A current simulated             (AMP)
    CREATE_PUBVAR(L_IbFuseSim           ,float);            // stator phase B current simulated             (AMP)
    CREATE_PUBVAR(L_IcFuseSim           ,float);            // stator phase C current simulated             (AMP)

    CREATE_PUBVAR(L_VaDFCapSim    ,float);            // simulated phase A l-n voltage at distortion filter cap (V l-n)
    CREATE_PUBVAR(L_VbDFCapSim    ,float);            // simulated phase B l-n voltage at distortion filter cap (V l-n)
    CREATE_PUBVAR(L_VcDFCapSim    ,float);            // simulated phase C l-n voltage at distortion filter cap (V l-n)
    CREATE_PUBVAR(L_VaFilCapSim    ,float);            // simulated phase A l-n voltage at distortion filter terminal (V l-n)
    CREATE_PUBVAR(L_VbFilCapSim    ,float);            // simulated phase B l-n voltage at distortion filter terminal (V l-n)
    CREATE_PUBVAR(L_VcFilCapSim    ,float);            // simulated phase C l-n voltage at distortion filter terminal (V l-n)

    CREATE_PUBVAR(L_IaXfmrSecSimZ1    ,float);            
    CREATE_PUBVAR(L_IbXfmrSecSimZ1    ,float);            
    CREATE_PUBVAR(L_IcXfmrSecSimZ1    ,float);            

    CREATE_PUBVAR(L_VaRCCapSim    ,float);             // simulated phase A l-n voltage at RC Snubber cap (V l-n)
    CREATE_PUBVAR(L_VbRCCapSim    ,float);            // simulated phase B l-n voltage at RC Snubber cap (V l-n)
    CREATE_PUBVAR(L_VcRCCapSim    ,float);            // simulated phase C l-n voltage at RC Snubber cap (V l-n)

    CREATE_PUBVAR(L_VaRCSim    ,float);    // simulated phase A l-n voltage at RC Snubber terminal (V l-n)
            
    CREATE_PUBVAR(L_VbRCSim    ,float);   // simulated phase B l-n voltage at RC Snubber terminal (V l-n)
             
    CREATE_PUBVAR(L_VcRCSim    ,float);   // simulated phase C l-n voltage at RC Snubber terminal (V l-n)
    
    CREATE_PUBVAR(L_VaXfmrSecTmpSim    ,float);            
    CREATE_PUBVAR(L_VbXfmrSecTmpSim    ,float);            
    CREATE_PUBVAR(L_VcXfmrSecTmpSim    ,float);   

    CREATE_PUBVAR(L_IaRCSim    ,float);         // simulated RC Snubber phase A current  
       
    CREATE_PUBVAR(L_IbRCSim    ,float);        // simulated RC Snubber phase B current     
    CREATE_PUBVAR(L_IcRCSim    ,float);        // simulated RC Snubber phase C current 
	
    CREATE_PUBVAR(S_FlxMXSim    ,float);        //simulated stator flux X component    
    CREATE_PUBVAR(S_FlxMYSim    ,float);       //simulated stator flux Y component         
    CREATE_PUBVAR(S_FlxMSim    ,float);         //simulated stator flux     
    CREATE_PUBVAR(S_IMagXSim    ,float);        //simulated stator magnitizing current X component        
    CREATE_PUBVAR(S_IMagYSim    ,float);        //simulated stator magnitizing current Y component      
    CREATE_PUBVAR(S_IMagSim    ,float);          //simulated stator magnitizing current  
    CREATE_PUBVAR(S_FlxMSimFil    ,float);      //simulated stator flux filtered

    CREATE_PUBVAR(S_IndM    ,float);   

    CREATE_PUBVAR(L_VaBrgSrcSim     ,float);            // simulated line bridge input source voltage A 
    CREATE_PUBVAR(L_VbBrgSrcSim     ,float);            // simulated line bridge input source voltage B 
    CREATE_PUBVAR(L_VcBrgSrcSim     ,float);            // simulated line bridge input source voltage C 

    CREATE_PUBVAR(R_VaEmfSim        ,float);            // simulated rotor back emf voltage of phase A
    CREATE_PUBVAR(R_VbEmfSim        ,float);            // simulated rotor back emf voltage of phase B
    CREATE_PUBVAR(R_VcEmfSim        ,float);            // simulated rotor back emf voltage of phase C
   
    CREATE_PUBVAR(R_IaBrgInSim      ,float);            // rotor bridge input phase A current simulated, flowing into bridge
    CREATE_PUBVAR(R_IbBrgInSim      ,float);            // rotor bridge input phase B current simulated, flowing into bridge
    CREATE_PUBVAR(R_IcBrgInSim      ,float);            // rotor bridge input phase C current simulated, flowing into bridge
    

    CREATE_PUBVAR(L_IocPhsASim      ,unsigned);         // line bridge IOC 
    CREATE_PUBVAR(L_IocPhsBSim      ,unsigned);         // 
    CREATE_PUBVAR(L_IocPhsCSim      ,unsigned);         // 
    CREATE_PUBVAR(R_IocPhsASim      ,unsigned);         // rotor bridge side IOC 
    CREATE_PUBVAR(R_IocPhsBSim      ,unsigned);         // 
    CREATE_PUBVAR(R_IocPhsCSim      ,unsigned);         //    

    
    

    CREATE_PUBVAR(L_VaXfmrMgSim, float);           // Xfmr primary side voltage phase A (line to neutral)
    CREATE_PUBVAR(L_VbXfmrMgSim        ,float);           // Xfmr primary side voltage phase B (line to neutral)
    CREATE_PUBVAR(L_VcXfmrMgSim        ,float);           // Xfmr primary side voltage phase C (line to neutral)
    CREATE_PUBVAR(L_IaXfmrMgSim, float);           // Xfmr primary side current phase A 
    CREATE_PUBVAR(L_IbXfmrMgSim        ,float);           // Xfmr primary side current phase B 
    CREATE_PUBVAR(L_IcXfmrMgSim        ,float);           // Xfmr primary side current phase C 
    

    CREATE_PUBVAR(L_XfmrState      ,unsigned);         //   Xfmr state:      0: normal      1: secondary side open     2: primary side open

    CREATE_PUBVAR(L_IxRCSim    ,float);         // simulated RC Snubber current x component        
    CREATE_PUBVAR(L_IyRCSim    ,float);        // simulated RC Snubber current y component    
    CREATE_PUBVAR(L_VxRCSimHold    ,float);         // simulated RC Snubber current x component        
    CREATE_PUBVAR(L_VyRCSimHold    ,float);        // simulated RC Snubber current y component    
    
    CREATE_PUBVAR(L_WElecSim        ,float);            // Simulated line electrical frequency
   
        CREATE_PUBVAR(L_VlnSSInSim      ,float);            // Simulated steady state Voltage in  (V l-n)
        CREATE_PUBVAR(L_BigRchg      ,unsigned);            // large pre-charging resistor with large time step

    CREATE_PUBVAR(IaRCRSSim    ,float);         // simulated RC Snubber phase A current in remote short        
    CREATE_PUBVAR(IbRCRSSim    ,float);         // simulated RC Snubber phase B current in remote short    
    CREATE_PUBVAR(IcRCRSSim    ,float);         // simulated RC Snubber phase C current in remote short    
    CREATE_PUBVAR(VaRCRSSim    ,float);         // simulated RC Snubber phase A voltage in remote short    
    CREATE_PUBVAR(VbRCRSSim    ,float);         // simulated RC Snubber phase A voltage in remote short    
    CREATE_PUBVAR(VcRCRSSim    ,float);         // simulated RC Snubber phase A voltage in remote short    
    CREATE_PUBVAR(RSPhaseState    ,unsigned);         // simulated RC Snubber phase states    

    CREATE_PUBVAR(IaRCLSSim    ,float);         // simulated RC Snubber phase A current in remote short        
    CREATE_PUBVAR(IbRCLSSim    ,float);         // simulated RC Snubber phase B current in remote short    
    CREATE_PUBVAR(IcRCLSSim    ,float);         // simulated RC Snubber phase C current in remote short    
    CREATE_PUBVAR(VaRCLSSim    ,float);         // simulated RC Snubber phase A voltage in remote short    
    CREATE_PUBVAR(VbRCLSSim    ,float);         // simulated RC Snubber phase A voltage in remote short    
    CREATE_PUBVAR(VcRCLSSim    ,float);         // simulated RC Snubber phase A voltage in remote short    
    CREATE_PUBVAR(LSPhaseState    ,unsigned);         // simulated RC Snubber phase states   


    CREATE_PUBVAR(IaRCSCSim    ,float);         // simulated RC Snubber phase A current in series compensation        
    CREATE_PUBVAR(IbRCSCSim    ,float);         // simulated RC Snubber phase B current in series compensation 
    CREATE_PUBVAR(IcRCSCSim    ,float);         // simulated RC Snubber phase C current in series compensation   
    CREATE_PUBVAR(VaRCSCSim    ,float);         // simulated RC Snubber phase A voltage in series compensation 
    CREATE_PUBVAR(VbRCSCSim    ,float);         // simulated RC Snubber phase A voltage in series compensation  
    CREATE_PUBVAR(VcRCSCSim    ,float);         // simulated RC Snubber phase A voltage in series compensation
    CREATE_PUBVAR(SCPhaseState    ,unsigned);         // simulated RC Snubber phase states   

  

    CREATE_PUBVAR(GridFlxASim    ,float);         // simulated phase A flux of Grid Impedence        
    CREATE_PUBVAR(GridFlxBSim    ,float);         //  simulated phase B flux of Grid Impedence    
    CREATE_PUBVAR(GridFlxCSim    ,float);         //  simulated phase C flux of Grid Impedence    
    CREATE_PUBVAR(GridFlxASptLSim    ,float);         // simulated phase A flux of left split Grid Impedence        
    CREATE_PUBVAR(GridFlxBSptLSim    ,float);         //  simulated phase B flux of left split Grid Impedence    
    CREATE_PUBVAR(GridFlxCSptLSim    ,float);         //  simulated phase C flux of left split Grid Impedence  
    CREATE_PUBVAR(GridFlxASptRSim    ,float);         // simulated phase A flux of right split Grid Impedence        
    CREATE_PUBVAR(GridFlxBSptRSim    ,float);         //  simulated phase B flux of right split Grid Impedence    
    CREATE_PUBVAR(GridFlxCSptRSim    ,float);         //  simulated phase C flux of right split Grid Impedence  
 
    
    CREATE_PUBVAR(IaGridSim    ,float);         // simulated grid impedence Z1 phase A current      
    CREATE_PUBVAR(IbGridSim    ,float);         // simulated grid impedence Z1 phase B current   
    CREATE_PUBVAR(IcGridSim    ,float);         // simulated grid impedence Z1 phase C current 
    CREATE_PUBVAR(IaGridSptLSim    ,float);         // simulated grid impedence Z1 phase A current in remote short        
    CREATE_PUBVAR(IbGridSptLSim    ,float);         // simulated grid impedence Z1 phase B current in remote short    
    CREATE_PUBVAR(IcGridSptLSim    ,float);         // simulated grid impedence Z1 phase C current in remote short    
    CREATE_PUBVAR(IaGridSptRSim    ,float);         // simulated grid impedence Z2 phase A current in remote short        
    CREATE_PUBVAR(IbGridSptRSim    ,float);         // simulated grid impedence Z2 phase B current in remote short    
    CREATE_PUBVAR(IcGridSptRSim    ,float);         // simulated grid impedence Z2 phase C current in remote short  
    
    CREATE_PUBVAR(IaGridLSSim    ,float);         // simulated grid phase A current behind remote short point       
    CREATE_PUBVAR(IbGridLSSim    ,float);         // simulated grid phase B current behind remote short point   
    CREATE_PUBVAR(IcGridLSSim    ,float);         // simulated grid phase C current behind remote short point   
    CREATE_PUBVAR(IaGridRSSim    ,float);         // simulated grid phase A current behind remote short point       
    CREATE_PUBVAR(IbGridRSSim    ,float);         // simulated grid phase B current behind remote short point   
    CREATE_PUBVAR(IcGridRSSim    ,float);         // simulated grid phase C current behind remote short point   
    CREATE_PUBVAR(IaRemoteShort    ,float);         // simulated grid phase A short current of remote short point       
    CREATE_PUBVAR(IbRemoteShort    ,float);         // simulated grid phase B short current of remote short point   
    CREATE_PUBVAR(IcRemoteShort    ,float);         // simulated grid phase C short current of remote short point   
        CREATE_PUBVAR(IaLocalShort    ,float);         // simulated grid phase A short current of local short point       
    CREATE_PUBVAR(IbLocalShort    ,float);         // simulated grid phase B short current of local short point   
    CREATE_PUBVAR(IcLocalShort    ,float);         // simulated grid phase C short current of local short point   

    CREATE_PUBVAR(IaGridSCSim    ,float);         // simulated grid phase A current of series compensation point   
    CREATE_PUBVAR(IbGridSCSim    ,float);         // simulated grid phase B current of series compensation point 
    CREATE_PUBVAR(IcGridSCSim    ,float);         // simulated grid phase C current of series compensation point 

    
    


    CREATE_PUBVAR(L_VanGridSim      ,float);            // simulated voltage a-neutral with flt sim     (V l-n)
    CREATE_PUBVAR(L_VbnGridSim      ,float);            // simulated voltage b-neutral with flt sim     (V l-n)
    CREATE_PUBVAR(L_VcnGridSim      ,float);            // simulated voltage c-neutral with flt sim     (V l-n)

    //----------remote short model


        //----------remote short model
 //   CREATE_PUBVAR(LLshortSim,        float);            // Simulated local short inductance referring to line side (Henrys)
  //  CREATE_PUBVAR(L_RLshortSim,        float);            // Simulated local short reistance referring to line side (Ohms)
//    CREATE_PUBVAR(L_LLshortllSim,        float);            // Simulated local short inductance referring to line side(line-line short) (Henrys)
//    CREATE_PUBVAR(L_RLshortllSim,        float);            // Simulated local short reistance referring to line side(line-line short) (Ohms)

        CREATE_PUBVAR(CrcLSSim,        float);            // Simulated local short inductance referring to line side(line-line short) (Henrys)
    CREATE_PUBVAR(RrcLSSim,        float);            // Simulated local short reistance referring to line side(line-line short) (Ohms)

            CREATE_PUBVAR(RhfGridRcp,        float);            // Rcp of high-frequency damping inductance referring to line side
    CREATE_PUBVAR(RhfGridSptLRcp,        float);            // Rcp of high-frequency damping inductance referring to line side(left side of remote short point)

        CREATE_PUBVAR(RhfGridSptRRcp,        float);            // Rcp of high-frequency damping inductance referring to line side(right side of remote short point)



    CREATE_PUBVAR(L_LRshortSim,        float);            // Simulated remote short inductance referring to line side (Henrys)
    CREATE_PUBVAR(L_RRshortSim,        float);            // Simulated remote short reistance referring to line side (Ohms)
    CREATE_PUBVAR(L_LRshortllSim,        float);            // Simulated remote short inductance referring to line side(line-line short) (Henrys)
    CREATE_PUBVAR(L_RRshortllSim,        float);            // Simulated remote short reistance referring to line side(line-line short) (Ohms)

    CREATE_PUBVAR(L_LLshortSim,        float);            // Simulated local short inductance referring to line side (Henrys)
    CREATE_PUBVAR(L_RLshortSim,        float);            // Simulated local short reistance referring to line side (Ohms)
    CREATE_PUBVAR(L_LLshortllSim,        float);            // Simulated local short inductance referring to line side(line-line short) (Henrys)
    CREATE_PUBVAR(L_RLshortllSim,        float);            // Simulated local short reistance referring to line side(line-line short) (Ohms)
    CREATE_PUBVAR(L_RDirectShort    ,unsigned);         // direct short flag 
    CREATE_PUBVAR(L_LDirectShort    ,unsigned);         // direct short flag

// grid event enable flag
    CREATE_PUBVAR(HVRTEnb     ,unsigned);         // Simulated HVRT in progress
    CREATE_PUBVAR(AsymmEnb    ,unsigned);         // Simulated asymmetry in progress
    CREATE_PUBVAR(AngStpSimZ1 ,float);            // Simulated asymmetry in progress
    CREATE_PUBVAR(RShortEnb     ,unsigned);         // Simulated remote short in progress
    CREATE_PUBVAR(LShortEnb     ,unsigned);         // Simulated local short in progress
    CREATE_PUBVAR(StartEnb     ,unsigned);         // Simulated startup in progress
	CREATE_PUBVAR(SCEnb     ,unsigned);         // Simulated Grid series compensation in progress
    CREATE_PUBVAR(VaSCRCSim,        float); 
    CREATE_PUBVAR(VbSCRCSim,        float); 
    CREATE_PUBVAR(VcSCRCSim,        float); 
        CREATE_PUBVAR(VaSCPha,        float); 
    CREATE_PUBVAR(VbSCPha,        float); 
    CREATE_PUBVAR(VcSCPha,        float); 
            CREATE_PUBVAR(IaSCPha,        float); 
    CREATE_PUBVAR(IbSCPha,        float); 
    CREATE_PUBVAR(IcSCPha,        float); 
  
    CREATE_PARM(LRshortSim,        float);            // Simulated remote short inductance referring to line side (Henrys)
    CREATE_PARM(RRshortSim,        float);            // Simulated remote short reistance referring to line side (Ohms)
    CREATE_PARM(LRshortllSim,        float);            // Simulated remote short inductance referring to line side(line-line short) (Henrys)
    CREATE_PARM(RRshortllSim,        float);            // Simulated remote short reistance referring to line side(line-line short) (Ohms)

    CREATE_PARM(LLshortSim,        float);            // Simulated local short inductance referring to line side (Henrys)
    CREATE_PARM(RLshortSim,        float);            // Simulated local short reistance referring to line side (Ohms)
    CREATE_PARM(LLshortllSim,        float);            // Simulated local short inductance referring to line side(line-line short) (Henrys)
    CREATE_PARM(RLshortllSim,        float);            // Simulated local short reistance referring to line side(line-line short) (Ohms)
    CREATE_PARM(RDirectShort    ,unsigned);         // direct short flag 
    CREATE_PARM(LDirectShort    ,unsigned);         // direct short flag

     CREATE_PARM(L_LgridSim,        float);            // Simulated grid inductance referring to line side (Henrys)
    CREATE_PARM(L_RgridSim,        float);            // Simulated grid reistance referring to line side (Ohms)

        CREATE_PUBVAR(L_LgridSptLSim,        float);            // Simulated grid inductance referring to line side one the left side of remote short point (Henrys)
    CREATE_PUBVAR(L_LgridSptRSim,        float);            // Simulated grid inductance referring to line side one the right side of remote short point (Henrys)
//-----------------------------------------------------------------------------
private:

    // External Inputs
    //----------------
    InTyp    In;
    InsTyp   Ins;

    // Methods
    //--------
    void GridMonitorProcessFbksSim(void);

    void LineDFCurSim(void);
    void LineSimulator(void);
    void LineVoltSimulator(void);

    void AngleSimulator(void);
    void AngleSlewSim(void);
    void RotateCurrentSim(void);
    void VoltsFbkSimulator(void);
    void VoltsSimulator(void);

    void RotorCmdFbkAnglSim(void);
    void RotorDemodVoltageSim(void);
    void RotorMtrElecSim(void);
    void RotorMtrModelMechSim(void);
    void RotorVoltageFbkSim(void);
    void RotorCurrentFbkSim(void);

    void BrgDisabSimulator(void);
    void BrgEnabSimulator(void);
    void LinkChargeSimulator(void);

    void GridEventTestSim(void);

    void IndMotSatSim(void);
    void LineBrgSimulator(void);           // Simulate bridge voltage,  CL, 2014-12-05
    void LineBrgCurSim(void);               // Simulate phase current,  CL, 2014-12-05
    void LineDCvoltSim(void);               // Simulate dc voltage,  CL, 2014-12-05

    void RotorBackEmfSim(void);
    void RotorBrgSimulator(void);
    
    void LineBrgIocSim(void);             // line bridge ioc, CL 2014-12-18
    void RotorBrgIocSim(void);             // rotor bridge ioc, CL 2014-12-18
    void PhaseANormal(void);
    void PhaseBNormal(void);
    void PhaseCNormal(void);

    // Diagnostics
    //------------

    // Parameters
    //-----------
    // System
    CREATE_PARM(TurbineNpRatSim,    float);             // Turbine name plate power (watts)
    CREATE_PARM(SysFreqNomSim,      float);
    CREATE_PARM(SysVoltNomSim,      float);
    // Grid
    
    CREATE_PARM(GridScrSim,         float);             // Short circuit ratio
    CREATE_PARM(ShrtMVASim,         float);             // Short circuit rating (min MVA)
    CREATE_PARM(GridXORSim,         float);             // Simulated grid X Over R
    CREATE_PARM(StUpXORSim,         float);             // Simulated startup SCR threshold
    
    CREATE_PARM(L_WGridSim,         float);             // Grid frequency simulated                     (rad/sec)
    CREATE_PARM(L_WSlwSim,          float);             // Frequency slew rate simulated                (rad/sec)
    CREATE_PARM(L_WMaxSim,          float);             // Max frequency in slew mode                   (rad/sec)
    CREATE_PARM(L_WMinSim,          float);             // Min frequency in slew mode                   (rad/sec)
    CREATE_PARM(L_WSlwMaxSim,       float);             // Max slew rate--freeze integrator if higher   (rad/sec)
    CREATE_PARM(AngleStepSim,        float);            // Angle step simulated                         (rad)
    
    // Converter
    CREATE_PARM(L_VllInSim,         float);             // Simulated input line voltage                 (Vrms)
    CREATE_PARM(L_RbrgSim,          float);             // Simulated line brg reactor resistance        (Ohm)
    CREATE_PARM(L_LbrgSim,          float);             // Simulated line brg reactor inductance        (Farad)
    CREATE_PARM(L_CbrgSim,          float);             // Simulated line brg reactor capacitance       (Henrys)
    CREATE_PARM(L_CdfSim,           float);             // Filter capacitor                             (Farad)
    CREATE_PARM(L_RdfSim,           float);             // Filter resistor                              (Ohm)
    CREATE_PARM(L_RdfWeakGrd,           float);             // Filter resistor for weak grid                             (Ohm)
    CREATE_PARM(WeakGrdThr,           float);             // weak grid SCR threshold to change the filter resistor for more damping     (Ohm)
    
    CREATE_PARM(L_RdfBldSim,        float);             // Filter capacitor leakage resistance
    CREATE_PARM(AuxIyRmsSim,        float);             // Iy simulated load                            (AMP)
    CREATE_PARM(AuxIxRmsSim,        float);             // Iy simulated load                            (AMP)
    CREATE_PARM(L_LdPwrAdrSim,      float*);            // Simulated load power address                 (watts)
    CREATE_PARM(L_VdcMaxSim,        float);             // Simulated maximum dc link voltage            (volt)
    CREATE_PARM(L_RchgSim,          float);             // line bridge charge resistor
    CREATE_PARM(L_RBldCapSim,       float);             // Simulated DC link bleed resistance
    CREATE_PARM(L_RchgEqv1Sim,      float);             // Equivalent charging path resistance when Q1 open
    CREATE_PARM(L_RchgEqv2Sim,      float);             // Equivalent charging path resistance when Q1 closed

    // Generator
    CREATE_PARM(S_RRatioSim,        float);             // Simulated generator SR ratio
    CREATE_PARM(R_RPMNomSim,        float);             // Simulated generator norminal RPM
    CREATE_PARM(R_PolePairsSim,     float);             // Simulated generator pole pairs
    CREATE_PARM(S_LsSim,            float);             // Simulated generator stator winding inductance
    CREATE_PARM(S_LrSim,            float);             // Simulated generator rotor winding inductance referring to stator                     
    CREATE_PARM(S_R1Sim,            float);             // Simulated generator stator winding resistance
    CREATE_PARM(S_R2Sim,            float);             // Simulated generator rotor winding resistance referring to stator
    CREATE_PARM(S_LmNomSim,         float);             // Simulated generator excitation inductance

    CREATE_PARM(R_SpdSetPtSim,      float);            // Simulated motor fix speed                    (RPM)
    CREATE_PARM(R_FlxResSim,        float);            // Simulator residue flux                       (webber)
    CREATE_PARM(R_TacZeroSim,       float);            // simulated tach marker offset                 (rad)
    CREATE_PARM(FlxDmpWdSim,        float);            // Simulator rotor flux damping omega (r/s)
    CREATE_PARM(FlxDmpKdSim,        float);            // Simulator rotor flux damping gain
    CREATE_PARM(S_FlxDmpWdSim,      float);            // Simulator stator flux damping omega (r/s)
    CREATE_PARM(S_FlxDmpKdSim,      float);            // Simulator stator flux damping gain
    

    // grid event simulation tests
    CREATE_PARM(GEvtDoubleHitSim,     unsigned);         // grid event test -- Double hit selection
    CREATE_PARM(GEvtTmSim,       float);            // Total grid event time (sec)
    CREATE_PARM(GEvtWaitTmSim,        float);            // grid event test wait time (sec)
    CREATE_PARM(GEvtDblRstTmSim,      float);            // Time duration between two grid event hits (sec)

    //CREATE_PARM(L_KVARFeedSim,       float);            // Simulated feed KVARs
    CREATE_PARM(L_VMagSSSclSim ,     float);            // Vmag steady state scaled
    CREATE_PARM(L_VMagSymSclSim,     float);            // Voltage magnitude symmetric scale
    CREATE_PARM(L_VMagAsySclSim,     float);            // Voltage magnitude assymmetric scale
    CREATE_PARM(GEvtTypeSim,      unsigned);            // Sim mode grid voltage distrubance test type
    CREATE_PARM(GEvtSCRdyTm,       float);             // series compensation last time before LVRT
    CREATE_PARM(GEvtSCRcvTm,       float);             // series compensation last time before recover
    
    CREATE_PARM(L_VxnGShrtTmSim     ,float);            // Line to neutral short time                   (sec)
    CREATE_PARM(L_VanGridSclSim     ,float);            // Sim A-N volt scale
    CREATE_PARM(L_VbnGridSclSim     ,float);            // Sim B-N volt scale
    CREATE_PARM(L_VcnGridSclSim     ,float);            // Sim C-N volt scale
    CREATE_PARM(L_VoltImbSclSim     ,float);            // Sim volt imblance scale
    CREATE_PARM(L_VabGFltSclSim     ,float);            // A-B volt fault scale
    
    // Simulator model selection for Distortion filter
    CREATE_PARM(DfDynVltSim         ,unsigned);         // Select dynamic filter voltage model

    // RC filter for open circuit
    CREATE_PARM(L_RrcSim,           float);             // RC resistor                              (Ohm)
    CREATE_PARM(L_CrcSim,           float);             // RC Capacitor                              (Ohm)

    //DFIG satulation curve
    
    CREATE_PARM(S_GenSatEnbSim      ,unsigned);            // Generator saturation curve enable flag
    CREATE_PARM(S_SatCurvTypeSim    ,unsigned);            // Generator saturation curve type
    CREATE_PARM(R_GenSatInFilWSim   ,float);               // Generator saturation curve input filter bandwidth (rad/sec)  
    CREATE_PARM(S_SatCurvFlxMaxSim  ,float);            
    CREATE_PARM(S_SatCurvFlxMinSim  ,float);            
    CREATE_PARM(S_SatCurvXmMaxSim   ,float);            
    CREATE_PARM(S_SatCurvXmMinSim   ,float);            
    CREATE_PARM(S_SatCurvK0Sim      ,float);            
    CREATE_PARM(S_SatCurvK1Sim      ,float);    
    CREATE_PARM(S_SatCurvK2Sim      ,float);            
    CREATE_PARM(S_SatCurvK3Sim      ,float);

    // Published Variables
    //--------------------
    CREATE_PUBVAR(ScrGridSim        ,float);            // Simulated Grid short circuit ratio

 //   CREATE_PUBVAR(L_LGridTotalSim   ,float);            // Total grid impedance (P.L_LxfmrPriSim + P.L_LxfmrSecSim
                                                        // + V.L_LgridExtSim)
 //   CREATE_PUBVAR(RGridTotSim       ,float);
 //   CREATE_PUBVAR(LGridTotSim       ,float);
    CREATE_PUBVAR(L_VxXfmrSecSim    ,float);            // Simulated,demodulated xfmer sec line converter voltage-x     (V l-l)
    CREATE_PUBVAR(L_VyXfmrSecSim    ,float);            // Simulated,demodulated xfmer sec line converter voltage-y     (V l-l)

    CREATE_PUBVAR(L_VxFilSim    ,float);                // Simulated,demodulated distortion filter voltage-x     (V l-l)
    CREATE_PUBVAR(L_VyFilSim    ,float);                // Simulated,demodulated distortion filter voltage-y     (V l-l)

    CREATE_PUBVAR(R_IxSim           ,float);            // Simulated rotor current X component                    (amps)
    CREATE_PUBVAR(R_IySim           ,float);            // Simulated rotor current Y component                    (amps)
    CREATE_PUBVAR(S_FlxXSim         ,float);            // Simulated stator flux X component            (weber)
    CREATE_PUBVAR(S_FlxYSim         ,float);            // Simulated stator flux Y component            (weber)
    CREATE_PUBVAR(R_FlxXSim         ,float);            // Simulator x component of flux                (weber)
    CREATE_PUBVAR(R_FlxYSim         ,float);            // Simulated y component of flux                (weber)
    CREATE_PUBVAR(R_FlxYSimLpf      ,float);            // Simulated flux-Y, lo-pass-filtered                     (weber)
    CREATE_PUBVAR(R_FlxYSimDmp      ,float);            // Simulator flux damping                                 (weber)
    CREATE_PUBVAR(R_TrqMtrSim       ,float);            // Motor torque simulated                       (N.m)
    CREATE_PUBVAR(S_FlxYSimLpf      ,float);            // Simulated stator flux-Y, lo-pass-filtered              (weber)
    CREATE_PUBVAR(S_FlxYSimDmp      ,float);            // Simulator stator flux damping                          (weber)
    CREATE_PUBVAR(S_R1Sim           ,float);            // Simulator stator resistance, referred to stator (Ohm)
    CREATE_PUBVAR(R_R2Sim           ,float);            // Simulator rotor resistance, referred to rotor (Ohm)
    CREATE_PUBVAR(S_LmSim           ,float);            // Lm for simulator                             (Henrys)
    CREATE_PUBVAR(S_VxMdlSim        ,float);            // Modeled stator X-axis voltage                (volt)
    CREATE_PUBVAR(S_VyMdlSim        ,float);            // Modeled stator Y-axis voltage                (volt)
    CREATE_PUBVAR(S_AngRotSim       ,float);            // Rotation angle for simulated stator voltages (rad)
    CREATE_PUBVAR(S_VaSim           ,float);            // Simulated stator Va                          (V l-n)
    CREATE_PUBVAR(S_VbSim           ,float);            // Simulated stator Vb                          (V l-n)

    CREATE_PUBVAR(R_AngMechSim      ,float);            // Simulated rotor angular position
    CREATE_PUBVAR(R_TacZeroSim      ,float);            // Simulated tach marker offset
    CREATE_PUBVAR(R_AngElecSim      ,float);            // Simulated rotor electrical angle
    CREATE_PUBVAR(S_AngElecSim      ,float);            // Simulated stator electrical angle
    CREATE_PUBVAR(S_WElecSim        ,float);            // Simulated stator electrical frequency


    CREATE_PUBVAR(L_AngElecSim      ,float);            // Simulated line electrical angle
    CREATE_PUBVAR(L_VanGridRotSim   ,float);            // Simulated voltage a-neutral rotate           (V l-n)
    CREATE_PUBVAR(L_VbnGridRotSim   ,float);            // Simulated voltage b-neutral rotate           (V l-n)
    CREATE_PUBVAR(L_VcnGridRotSim   ,float);            // Simulated voltage c-neutral rotate           (V l-n)
    CREATE_PUBVAR(L_VabGridSim      ,float);            // Simulated Grid voltage phase a-b with flt sim (V-l)
    CREATE_PUBVAR(L_VbcGridSim      ,float);            // Simulated Grid voltage phase b-c with flt sim (V-l)
    CREATE_PUBVAR(L_XfmrSecFlxXSim  ,float);            // Simulated grid flux-X                                        (weber)
    CREATE_PUBVAR(L_XfmrSecFlxYSim  ,float);            // Simulated grid flux-Y                                        (weber)
    CREATE_PUBVAR(L_ShortIpSim      ,unsigned);         // Short circuit fault in progress
    CREATE_PUBVAR(GEvtIpSim     ,unsigned);         // Simulated grid event in progress
    CREATE_PUBVAR(ChargrXdropSim    ,float);            // Simulated x-voltage across charging resistor (Ohm)
    CREATE_PUBVAR(ChargrYdropSim    ,float);            // Simulated y-voltage across charging resistor (Ohm)
    CREATE_PUBVAR(L_VabLineSim      ,float);            // Simulated line voltage phase a-b with flt sim (V l-l)
    CREATE_PUBVAR(L_VbcLineSim      ,float);            // Simulated line voltage phase b-c with flt sim (V l-l)
    CREATE_PUBVAR(L_IPosSim         ,float);            // Simulated current entering the dc bus         (AMP)

    CREATE_PUBVAR(L_IxXfmrSecSim    ,float);            // Simulated xfmr secondary winding current-X    (AMP)
    CREATE_PUBVAR(L_IyXfmrSecSim    ,float);            // Simulated xfmr secondary winding current-Y    (AMP)
    // LDF related variables
    CREATE_PUBVAR(L_QxDFSim         ,float);            // Simulated dist-filt x-axis charge of simplified filter model
    CREATE_PUBVAR(L_QyDFSim         ,float);            // Simulated dist-filt y-axis charge of simplified filter model
    CREATE_PUBVAR(L_VxDFCapSim      ,float);            // Simulated dist-filt x-axis voltage of simplified filter model
    CREATE_PUBVAR(L_VyDFCapSim      ,float);            // Simulated dist-filt y-axis voltage of simplified filter model
    CREATE_PUBVAR(L_VxDFSim         ,float);
    CREATE_PUBVAR(L_VyDFSim         ,float);
    CREATE_PUBVAR(L_IxDFSim         ,float);            // Simulated line converter side distortion filter current-X                        (AMP)
    CREATE_PUBVAR(L_IyDFSim         ,float);            // Simulated line converter side distortion filter current-Y                        (AMP)
    CREATE_PUBVAR(L_RdfBldRcpSim,   float);             // Simulated line filter capacitor leakage resistance
    CREATE_PUBVAR(L_DelAngSim       ,float);            // delta angle for simaultor                    (rad)
    CREATE_PUBVAR(L_CdfRcpSim,      float);             // Simulated line side distortion filter capacitance

    // Simulator angle slew generator
    CREATE_PUBVAR(L_AngGridSim      ,float);            // Simulator grid angle                         (rad)
    CREATE_PUBVAR(L_WSlwSignSim     ,float);            // Frequency slew sign-init to 1 in s0init      (rad/sec)
    CREATE_PUBVAR(L_WSlwIntSim      ,float);            // Frequency slew integrator                    (rad/sec)
    CREATE_PUBVAR(L_WSim            ,float);            // Simaultor angle frequency                    (rad/sec)

    CREATE_PUBVAR(L_RchgEqv2SimRcp, float);             // Equivalent charging path resistance reciprocal when Q1 closed
    CREATE_PUBVAR(L_RchgEqv1SimRcp, float);             // Equivalent charging path resistance reciprocal when Q1 open
    CREATE_PUBVAR(L_RchgEqvSim      ,float);            // Equivalent charging path resistance for simulator (Ohm)
    CREATE_PUBVAR(L_RchgEqvSimRcp   ,float);            // Equivalent charging path resistancereciprocal for simulator (1/Ohm)
    CREATE_PUBVAR(L_VRectSim        ,float);            // Simulated rectify voltage
    CREATE_PUBVAR(L_IaChgSim,       float);             // Simulated charging current phase A
    CREATE_PUBVAR(L_IbChgSim,       float);             // Simulated charging current phase B
    CREATE_PUBVAR(L_IcChgSim,       float);             // Simulated charging current phase C
    CREATE_PUBVAR(L_IxChgSim,       float);             // Simulated charging current x-component
    CREATE_PUBVAR(L_IyChgSim,       float);             // Simulated charging current y-component
    CREATE_PUBVAR(L_VanChgSim,      float);             // Simulated charging voltage phase A
    CREATE_PUBVAR(L_VbnChgSim,      float);             // Simulated charging voltage phase B
    CREATE_PUBVAR(L_VcnChgSim,      float);             // Simulated charging voltage phase C
    CREATE_PUBVAR(L_ChgAngSelSim,   int);               // Simulated charging angle selection
    CREATE_PUBVAR(L_RBldCapRcpSim,  float);             // Simulated DC link bleed resistance reciprocal
    
    CREATE_PUBVAR(DcPwrSim          ,float);            // simulated dc link power                      (watts)
    CREATE_PUBVAR(L_FlxXSim         ,float);            // x-channel integrator for simulator flux
    CREATE_PUBVAR(L_FlxYSim         ,float);            // y-channel integrator for simulator flux
    CREATE_PUBVAR(L_IdcSim          ,float);            // simulated dc link current                    (AMP)
    CREATE_PUBVAR(L_ILoadSim        ,float);            // Simulated load current                       (AMP)
    CREATE_PUBVAR(L_IxCalLSim       ,float);            // simulated instant. x-channel current         (AMP)
    CREATE_PUBVAR(L_IyCalLSim       ,float);            // simulated instant. y-channel current         (AMP)
    CREATE_PUBVAR(L_VdcWSim         ,float);            // L_Vdc simulated omega for charge, decay      (rad/sec)
    CREATE_PUBVAR(L_VxErrSim        ,float);            // x-channel error for simulator                (volt)
    CREATE_PUBVAR(L_VxVrSim         ,float);            // x-channel simulator resistor voltage         (volt)
    CREATE_PUBVAR(L_VyErrSim        ,float);            // y-channel error for simulator                (volt)
    CREATE_PUBVAR(L_VyVrSim         ,float);            // y-channel simulator resistor voltage         (volt)

    CREATE_PUBVAR(L_LdPwrSim        ,float);            // Load power seen by line bridge for simulator (Watts)
    CREATE_PUBVAR(L_PllWNomSim      ,float);            // line pll frequency for simulator             (rad/sec)
    CREATE_PUBVAR(L_VxBrgEnbSim     ,float);            // x-channel simulated bridge voltage when brg pwr is enb       (volt)
    CREATE_PUBVAR(L_VyBrgEnbSim     ,float);            // y-channel simulated bridge voltage when brg pwr is enb      (volt)
    CREATE_PUBVAR(CapDtSim          ,float);            // simulated delta-t divided by capacitance
    CREATE_PUBVAR(L_RCHrmFRcpSim    ,float);            // Simulated 1/(1+((R*C)/dt))
    CREATE_PUBVAR(L_WGrdFrqStpSim   ,float);            // Simulation mode frqeuncy step                (rad/sec)
    CREATE_PUBVAR(L_RdfRcpSim       ,float);            // Simulated dist-filter DFC1 resistance reciprocal             (1/Ohm)

    CREATE_PUBVAR(R_WElecSim        ,float);            // Simulated Output freq, unfil
    CREATE_PUBVAR(R_AngElecLinSim   ,float);            // Simulated equivalent GM angle on rotor side  (rad)
    CREATE_PUBVAR(L_GmPllWAvgSim    ,float);            // phase lock loop frequency --average of last two passes       (rad/sec)
    CREATE_PUBVAR(AuxIySim          ,float);            // simulated auxillary reactive load            (AMP)
    CREATE_PUBVAR(AuxIxSim          ,float);            // simulated auxillary active load              (AMP)
    
    CREATE_PUBVAR(L_IxBrgSim        ,float);            // line simulated x-component current           (AMP)
    CREATE_PUBVAR(L_IyBrgSim        ,float);            // line simulated y-component current           (AMP)
    CREATE_PUBVAR(L_VxBrgSim        ,float);            // simulated x-component motor voltage          (volt)
    CREATE_PUBVAR(L_VyBrgSim        ,float);            // simulated y-component motor voltage          (volt)
    CREATE_PUBVAR(R_IxBrgSim        ,float);            // simulated x-component motor rotor current    (AMP)
    CREATE_PUBVAR(R_IyBrgSim        ,float);            // simulated y-component motor rotor current    (AMP)
    CREATE_PUBVAR(R_VxBrgSim        ,float);            // simulated x-component motor rotor voltage    (volt)
    CREATE_PUBVAR(R_VyBrgSim        ,float);            // simulated y-component motor rotor voltage    (volt)
    CREATE_PUBVAR(S_VxSim           ,float);            // simulated x-component motor stator voltage   (volt)
    CREATE_PUBVAR(S_VySim           ,float);            // simulated y-component motor stator voltage   (volt)
    CREATE_PUBVAR(S_IxSim           ,float);            // simulated x-component motor stator current   (volt)
    CREATE_PUBVAR(S_IySim           ,float);            // simulated y-component motor stator current   (volt)

    CREATE_PUBVAR(L_VlnInSim        ,float);            // Simulated input volt magnitude     (V l-n)
    CREATE_PUBVAR(L_VxnGridSFSim    ,float);            // Simulated Symmetric fault scale     (pu)

    // xmfer magnetizing variables
    CREATE_PUBVAR(L_VxXfmrMgSim     ,float);            // simulated X-axis voltage at xfmr magnetizing branch @ 690 v   (volt)
    CREATE_PUBVAR(L_VyXfmrMgSim     ,float);            // simulated Y-axis voltage at xfmr magnetizing branch @ 690 v   (volt)
    CREATE_PUBVAR(L_VxLnSim         ,float);
    CREATE_PUBVAR(L_VyLnSim         ,float);

    // grid variables
    CREATE_PUBVAR(IxGridSim         ,float);            // simulated net grid Ix (stator + line conv) @ 690 volts       (AMP)
    CREATE_PUBVAR(IyGridSim         ,float);            // simulated net grid Iy (stator + line conv) @ 690 volts       (AMP)
    CREATE_PUBVAR(VxGridSim         ,float);            // Simulated,demodulated grid voltage-X                         (Volt)
    CREATE_PUBVAR(VyGridSim         ,float);            // Simulated,demodulated grid voltage-Y                         (Volt)

    // Vars for LowVoltageTestRelaySequencing
    CREATE_PUBVAR(GEvtStateSim,       int);              // grid event state variable
    CREATE_PUBVAR(GEvtCycleNoSim,     int);              // grid event test cycle number
    CREATE_PUBVAR(GEvtReqSim    ,     unsigned);         // grid event request
    CREATE_PUBVAR(GEvtReqSimZ1  ,     unsigned);         // grid event request old
    CREATE_PUBVAR(GEvtReadySim  ,     unsigned);         // grid event test ready
    CREATE_PUBVAR(GEvtRdyTmrSim ,     float);            // grid event test ready timer
    CREATE_PUBVAR(GEvtRdyTmrUSim,     float);            // grid event test ready up timer
    CREATE_PUBVAR(GEvtDlyTmrSim ,     float);            // grid event timer
    CREATE_PUBVAR(GEvtSCompDlyTmrSim ,     float);     // insert/release SC Timer for waiting the steady state	
  //  CREATE_PUBVAR(L_LgridExtSim,     float);            // Simulated grid inductance referring to line side (Henrys)
  //  CREATE_PUBVAR(L_RgridExtSim,     float);            // Simulated grid reistance referring to line side (Ohms)

 
   

    CREATE_PUBVAR(L_CrcRcpSim,      float);             // Simulated line side distortion filter capacitance

    CREATE_PUBVAR(L_LgridRcpSim,        float);            // Rcp of Simulated grid inductance referring to line side (Henrys)
    CREATE_PUBVAR(L_LgridSptLRcpSim,        float);            // Rcp of Simulated grid inductance referring to line side one the left side of remote short point (Henrys)
    CREATE_PUBVAR(L_LgridSptRRcpSim,        float);            // Rcp of Simulated grid inductance referring to line side one the right side of remote short point (Henrys)


    //------------- Bridge model --------------  

    CREATE_PUBVAR(L_VaLnSim,        float);     // Bridge inductor 3-phase input voltage (volt)
    CREATE_PUBVAR(L_VbLnSim,        float);
    CREATE_PUBVAR(L_VcLnSim,        float); 
    CREATE_PUBVAR(L_SaBrgSim,       int);       // Bridge switching function
    CREATE_PUBVAR(L_SbBrgSim,       int);
    CREATE_PUBVAR(L_ScBrgSim,       int);
    
    //Rotor back emf 
    CREATE_PUBVAR(R_VxEmfSim,       float);   
    CREATE_PUBVAR(R_VyEmfSim,       float);   
    CREATE_PUBVAR(R_IdcSim,         float);  
    
    CREATE_PARM(L_IocThrsSim,       float);             // line bridge ioc threshold,

    // Unpublished Variables
    //----------------------
    SinCosTyp   L_SinCosLineSim;
    SinCosTyp   L_SinCosSim;                            // sine/cosine structure for grid monitor angle feedback
    SinCosTyp   R_SinCosSim;                            // sine/cosine structure for rotor angle feedback
    SinCosTyp   S_SinCosSim;                            // sine/cosine structure for stator angle feedback
    double      GEvtDlyTmrSimDp;                             // double precision grid event timer

    // Intermediate variables
    float       R_K11;
    float       R_K22;
    float       R_K12;
    float       R_Kt0;
    float       K_RS;
    float       K_SR;

    float       S_RRatioRcp;                            // reciprocal of S_RRatio

    // Simulated delayed variables
    float       IxGridSimZ1;
    float       IyGridSimZ1;
    float       R_IxSimZ1;
    float       R_IySimZ1;
    float       S_IxSimZ1;
    float       S_IySimZ1;
    float       L_IxBrgSimZ1;
    float       L_IyBrgSimZ1;
    float       L_IxXfmrSecSimZ1;
    float       L_IyXfmrSecSimZ1;

    float       L_VdcSimRcp;                            // simulated dc link voltage feedback rcp
    float       L_LbrgSimRcp;                           // simulated reciprocal of the line brg reactor inductance


    float       ChargWDtSim;
    float       DisChargWDtSim;
    
    float       S_FlxXSimZ1;
    float       S_FlxYSimZ1;   
    
    
    float       L_VxXfmrSecFil;
    float       L_VyXfmrSecFil;
    
   
    float       L_IxCalLSimZ1;
    float       L_IyCalLSimZ1;   
    
    float       S_IxSimAvg;
    float       S_IySimAvg;
    
    unsigned    Cnt_IndMotSat;
    // Data Passing
    //-------------

};



// Inline Function Definitions
//----------------------------


#endif  // __S0_SIM_PLANT1X_H__
