///////////////////////////////////////////////////////////////////////////////
//
// TITLE         p1dtd.cpp
//
// DESCRIPTION
//      This file contains functions for the Drive Train Damper
//      
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      
//
///////////////////////////////////////////////////////////////////////////////

// Includes Files
//---------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
// product
#include "Product.h"


// Constants
//----------



// Externals
//----------
extern float R_SpdTCtlT2;
extern float DelTm2;
extern float DelTm2Rcp;

extern unsigned S_K1ClsCmd;

// Types and Classes
//------------------


// Diagnostics
//------------
//CREATE_DIAG(        );          //                                 


// Parameters
//-----------
CREATE_PARM(DTDStpAdr       ,   float    *);   // Drive train damper step address                                                                                         
CREATE_PARM(DTDGenTrqLim    ,   float     );   // Drive train generating torque limit (N*m)                                                                               
CREATE_PARM(DTDMtrTrqLim    ,   float     );   // Drive train generating torque limit (N*m)                                                                               

CREATE_PARM(DTD1EnbLcl      ,   unsigned  );   // Drive train damper #1 enable 'test' parameter, local                                                                       
CREATE_PARM(DTD1SldGnLcl    ,   float     );   // Drive train damper #1 'slider' gain test parameter, local                                                               
CREATE_PARM(DTD1EnbAdr      ,   unsigned *);   // Drive train damper #1 enable pointer                                                                                       
CREATE_PARM(DTD1SldGnAdr    ,   float    *);   // Drive train damper #1 pointer to slider gain                                                                            
CREATE_PARM(DTD1KLcl        ,   float     );   // Drive train damper #1 relative gain in %, related to constant 400 Nm/rpm, local                                         
CREATE_PARM(DTD1ZLcl        ,   float     );   // Drive train damper #1 frequency in hz, local                                                                            
CREATE_PARM(DTD1FLcl        ,   float     );   // Drive train damper #1 rel damper degree in %, local                                                                     
CREATE_PARM(DTD1TLcl        ,   float     );   // Drive train damper #1 time lag in msec, local                                  
                                          
CREATE_PARM(DTD2SldGnLcl    ,   float     );   // Drive train damper #2 'slider' gain teste parameter, local                                                              
CREATE_PARM(DTD2EnbLcl      ,   unsigned  );   // Drive train damper #2 enable 'test' parameter, local                                                                       
CREATE_PARM(DTD2EnbAdr      ,   unsigned *);   // Drive train damper #2 enable pointer                                                                                       
CREATE_PARM(DTD2SldGnAdr    ,   float    *);   // Drive train damper #2 pointer to slider gain                                                                            
CREATE_PARM(DTD2KLcl        ,   float     );   // Drive train damper #2 relative gain in %, related to constant 400 Nm/rpm, local                                         
CREATE_PARM(DTD2ZLcl        ,   float     );   // Drive train damper #2 frequency in hz, local                                                                            
CREATE_PARM(DTD2FLcl        ,   float     );   // Drive train damper #2 rel damper degree in %, local                                                                     
CREATE_PARM(DTD2TLcl        ,   float     );   // Drive train damper #2 time lag in msec, local                                                                           



// Variables
//----------

/////////////////////////////////////////////////////////////////////
// Drive Train  intermediate variables from IoNet or from local parameters
/////////////////////////////////////////////////////////////////////
CREATE_PUBVAR(DTD1KRem      ,    float   );    // Drive train damper #1 relative gain in %, related to constant 400 Nm/rpm, IoNet
CREATE_PUBVAR(DTD1ZRem      ,    float   );    // Drive train damper #1 frequency in hz, IoNet
CREATE_PUBVAR(DTD1FRem      ,    float   );    // Drive train damper #1 rel damper degree in %, IoNet
CREATE_PUBVAR(DTD1TRem      ,    float   );    // Drive train damper #1 time lag in msec, IoNet                                    
CREATE_PUBVAR(DTD1EnbRem    ,    unsigned);    // Drive train damper #1 enable variable, IoNet, BOOL
CREATE_PUBVAR(DTD1SldGnRem  ,    float   );    // Drive train damper #1 'slider' gain in pu, IoNet..range from 0 to 1.0
CREATE_PUBVAR(DTD1EnbLcl    ,    unsigned);    // Drive train damper #1 enable variable from test parameter, DT1EnbLcl, local, BOOL
CREATE_PUBVAR(DTD1SldGnLcl  ,    float   );    // Drive train damper #1 variable from test parameter, DT1SldGnLcl, in pu, IoNet..range from 0 to 1.0

CREATE_PUBVAR(DTD2KRem      ,    float   );    // Drive train damper #2 relative gain in %, related to constant 400 Nm/rpm, IoNet
CREATE_PUBVAR(DTD2ZRem      ,    float   );    // Drive train damper #2 frequency in hz, IoNet
CREATE_PUBVAR(DTD2FRem      ,    float   );    // Drive train damper #2 rel damper degree in %, IoNet
CREATE_PUBVAR(DTD2TRem      ,    float   );    // Drive train damper #2 time lag in msec, IoNet                                    
CREATE_PUBVAR(DTD2SldGnRem  ,    float   );    // Drive train damper #2 enable variable, IoNet, BOOL
CREATE_PUBVAR(DTD2EnbRem    ,    unsigned);    // Drive train damper #1 'slider' gain in pu, IoNet..range from 0 to 1.0
CREATE_PUBVAR(DTD2EnbLcl    ,    unsigned);    // Drive train damper #2 enable variable from test parameter, DT1EnbLcl, local, BOOL
CREATE_PUBVAR(DTD2SldGnLcl  ,    float   );    // Drive train damper #2 variable from test parameter, DT1SldGnLcl, in pu, IoNet..range from 0 to 1.0

/////////////////////////////////////////////////////////////////////
// Drive Train  final variables from IoNet or from local parameters for use in task 2 algorithm 
/////////////////////////////////////////////////////////////////////
CREATE_PUBVAR(DTD1K         ,    float   );    // Drive train damper #1 relative gain in %, related to constant 400 Nm/rpm, IoNet
CREATE_PUBVAR(DTD1Z         ,    float   );    // Drive train damper #1 frequency in hz, IoNet
CREATE_PUBVAR(DTD1F         ,    float   );    // Drive train damper #1 rel damper degree in %, IoNet
CREATE_PUBVAR(DTD1T         ,    float   );    // Drive train damper #1 time lag in msec, IoNet                                    
CREATE_PUBVAR(DTD1Enb       ,    unsigned);    // Drive train damper #1 enable variable, IoNet, BOOL
CREATE_PUBVAR(DTD1SldGn     ,    float   );    // Drive train damper #1 'slider' gain in pu, IoNet..range from 0 to 1.0

CREATE_PUBVAR(DTD2K         ,    float   );    // Drive train damper #2 relative gain in %, related to constant 400 Nm/rpm, IoNet
CREATE_PUBVAR(DTD2Z         ,    float   );    // Drive train damper #2 frequency in hz, IoNet
CREATE_PUBVAR(DTD2F         ,    float   );    // Drive train damper #2 rel damper degree in %, IoNet
CREATE_PUBVAR(DTD2T         ,    float   );    // Drive train damper #2 time lag in msec, IoNet                                    
CREATE_PUBVAR(DTD2Enb       ,    unsigned);    // Drive train damper #2 enable variable, IoNet, BOOL
CREATE_PUBVAR(DTD2SldGn     ,    float   );    // Drive train damper #2 'slider' gain in pu, IoNet..range from 0 to 1.0



/////////////////////////////////////////////////////////////////////
// Final Drive Train Damper multipliers for use in task 2 algorthim
/////////////////////////////////////////////////////////////////////
CREATE_PUBVAR(DTD1LN1       ,    float   );    // Drive train damper #1 LN1
CREATE_PUBVAR(DTD1LN2       ,    float   );    // Drive train damper #1 LN2
CREATE_PUBVAR(DTD1LN3       ,    float   );    // Drive train damper #1 LN3
CREATE_PUBVAR(DTD1LD1       ,    float   );    // Drive train damper #1 LD1
CREATE_PUBVAR(DTD1LD2       ,    float   );    // Drive train damper #1 LD2
CREATE_PUBVAR(DTD1LD3       ,    float   );    // Drive train damper #1 LD3

CREATE_PUBVAR(DTD2LN1       ,    float   );    // Drive train damper #1 LN1
CREATE_PUBVAR(DTD2LN2       ,    float   );    // Drive train damper #1 LN2
CREATE_PUBVAR(DTD2LN3       ,    float   );    // Drive train damper #1 LN3
CREATE_PUBVAR(DTD2LD1       ,    float   );    // Drive train damper #1 LD1
CREATE_PUBVAR(DTD2LD2       ,    float   );    // Drive train damper #1 LD2
CREATE_PUBVAR(DTD2LD3       ,    float   );    // Drive train damper #1 LD3

/////////////////////////////////////////////////////////////////////
// Drive train damper algorithm variables 
/////////////////////////////////////////////////////////////////////
CREATE_PUBVAR(DTDSpdIn      ,    float   );    // 2*Task2 Drive train damper input speed signal (rpm)
CREATE_PUBVAR(DTDTrqCmdTot  ,    float   );    // Drive train damper torqueCmd total (kNm)
CREATE_PUBVAR(DTDSpdStep    ,    float   );    // Drive train damper speed step

// DTD #1
CREATE_PUBVAR(DTD1TrqCmd    ,    float   );    // Drive train damper #1 torqueCmd (kNm)
CREATE_PUBVAR(DTD1SpdZ1     ,    float   );    // Drive train damper #1 z-1 speed signal (rpm)
CREATE_PUBVAR(DTD1SpdZ2     ,    float   );    // Drive train damper #1 z-2 speed signal (rpm)
CREATE_PUBVAR(DTD1TrqCmdZ0  ,    float   );    // Drive train damper #1 z-0 torque signal (kNm)
CREATE_PUBVAR(DTD1TrqCmdZ1  ,    float   );    // Drive train damper #1 z-1 torque signal (kNm)
CREATE_PUBVAR(DTD1TrqCmdZ2  ,    float   );    // Drive train damper #1 z-1 torque signal (kNm)
// DTD #2
CREATE_PUBVAR(DTD2TrqCmd    ,    float   );    // Drive train damper #2 torqueCmd (kNm)
CREATE_PUBVAR(DTD2SpdZ1     ,    float   );    // Drive train damper #2 z-1 speed signal (rpm)
CREATE_PUBVAR(DTD2SpdZ2     ,    float   );    // Drive train damper #2 z-2 speed signal (rpm)
CREATE_PUBVAR(DTD2TrqCmdZ0  ,    float   );    // Drive train damper #2 z-0 torque signal (kNm)
CREATE_PUBVAR(DTD2TrqCmdZ1  ,    float   );    // Drive train damper #2 z-1 torque signal (kNm)
CREATE_PUBVAR(DTD2TrqCmdZ2  ,    float   );    // Drive train damper #2 z-1 torque signal (kNm)


//Drive train damper background variables for use in final constant calculation
CREATE_PUBVAR(DTD1KC        ,    float   );    // Drive train damper #1 relative gain
CREATE_PUBVAR(DTD1Omega     ,    float   );    // Drive train damper #1 omega
CREATE_PUBVAR(DTD1Zeta      ,    float   );    // Drive train damper #1 zeta                                                     
CREATE_PUBVAR(DTD1Tau       ,    float   );    // Drive train damper #1 tau
CREATE_PUBVAR(DTD1TauDtRcp  ,    float   );    // Drive train damper #1 1.0/tau*dt 
CREATE_PUBVAR(DTD1WDtSq     ,    float   );    // Drive train damper #1 (w*dt)^2
CREATE_PUBVAR(DTD14ZetaWdt  ,    float   );    // Drive train damper #1 4*zeta*w*dt
CREATE_PUBVAR(DTD1LD1Rcp    ,    float   );    // Drive train damper #1 LD1 reciprocal 

CREATE_PUBVAR(DTD2KC        ,    float   );    // Drive train damper #2 relative gain                                    
CREATE_PUBVAR(DTD2Omega     ,    float   );    // Drive train damper #2 omega
CREATE_PUBVAR(DTD2Zeta      ,    float   );    // Drive train damper #2 zeta                                                     
CREATE_PUBVAR(DTD2Tau       ,    float   );    // Drive train damper #2 tau
CREATE_PUBVAR(DTD2TauDtRcp  ,    float   );    // Drive train damper #2 1.0/tau*dt 
CREATE_PUBVAR(DTD2WDtSq     ,    float   );    // Drive train damper #2 (w*dt)^2
CREATE_PUBVAR(DTD24ZetaWdt  ,    float   );    // Drive train damper #2 4*zeta*w*dt
CREATE_PUBVAR(DTD2LD1Rcp    ,    float   );    // Drive train damper #2 LD1 reciprocal 

CREATE_PUBVAR(DTD1LN3Est    ,    float   );
CREATE_PUBVAR(DTD1LN3Err    ,    float   );
CREATE_PUBVAR(DTD1LN3Adjst  ,    float   );

CREATE_PUBVAR(DTD2LN3Est    ,    float   );
CREATE_PUBVAR(DTD2LN3Err    ,    float   );
CREATE_PUBVAR(DTD2LN3Adjst  ,    float   );

CREATE_PUBVAR(DTDGnTrqLmNm  ,    float   );

// Unpublished Variables
//----------------------



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION   
//   Performs drive train damper function in converter.  This function 
//   was previously only in the turbine control, but was moved to the converter in 
//   order to gain phase margin for the system 
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void DriveTrainDamper()
{
    float  DTD1TrqTemp, DTD2TrqTemp;
    float  DTD1SpdSumTmp, DTD2SpdSumTmp;

    DTD1Enb   = READ_PARMPTR(DTD1EnbAdr);
    DTD1SldGn = READ_PARMPTR(DTD1SldGnAdr);
    DTD2Enb   = READ_PARMPTR(DTD2EnbAdr);
    DTD2SldGn = READ_PARMPTR(DTD2SldGnAdr);

////////////////////////////////////////////////////
// Calculate the speed input
///////////////////////////////////////////////////        
       
    DTDSpdStep = READ_PARMPTR(DTDStpAdr);

    DTDSpdIn   = R_SpdTCtlT2 + DTDSpdStep;           
    
    if ( S_K1ClsCmd )    
    {
    
///////////////////////////////////////////////////
// Drive Train Damper #1 Algorithm 
///////////////////////////////////////////////////        
      
       // sum of the scaled speed terms (note sum of the co-efficents equals zero)
       DTD1SpdSumTmp = DTD1LN1*DTDSpdIn + DTD1LN2*DTD1SpdZ1 + DTD1LN3*DTD1SpdZ2;       
       
       // set sum to zero if speed is constant (UCSB floating point ALU issue)
       if ((DTDSpdIn == DTD1SpdZ1) && (DTD1SpdZ1 == DTD1SpdZ2)) DTD1SpdSumTmp = 0.0F;  

       DTD1TrqCmdZ0 = DTD1KC * DTD1SpdSumTmp + DTD1LD2*DTD1TrqCmdZ1 + DTD1LD3*DTD1TrqCmdZ2;

       DTD1TrqCmd   = DTD1SldGn * DTD1TrqCmdZ0;
        
       DTD1SpdZ2    = DTD1SpdZ1;                            
       DTD1SpdZ1    = DTDSpdIn;                            
       DTD1TrqCmdZ2 = DTD1TrqCmdZ1;                            
       DTD1TrqCmdZ1 = DTD1TrqCmdZ0;                            

        
///////////////////////////////////////////////////
// Drive Train Damper #2 Algorithm 
///////////////////////////////////////////////////

       DTD2SpdSumTmp = DTD2LN1*DTDSpdIn + DTD2LN2*DTD2SpdZ1 + DTD2LN3*DTD2SpdZ2;    

       if ((DTDSpdIn == DTD2SpdZ1) && (DTD2SpdZ1 == DTD2SpdZ2)) DTD2SpdSumTmp = 0.0F;
       
       DTD2TrqCmdZ0 = DTD2KC * DTD2SpdSumTmp + DTD2LD2*DTD2TrqCmdZ1 + DTD2LD3*DTD2TrqCmdZ2;
	   
	   DTD2TrqCmd   = DTD2SldGn * DTD2TrqCmdZ0;

       DTD2SpdZ2    = DTD2SpdZ1;                            
       DTD2SpdZ1    = DTDSpdIn;                            
       DTD2TrqCmdZ2 = DTD2TrqCmdZ1;                            
       DTD2TrqCmdZ1 = DTD2TrqCmdZ0;
                                   
    }
    else // initialize both DTDs
    {
       DTD1SpdZ2    = DTDSpdIn;                            
       DTD1SpdZ1    = DTDSpdIn;                            
       DTD1TrqCmdZ2 = 0.0;                            
       DTD1TrqCmdZ1 = 0.0;                            
       DTD1TrqCmd   = 0.0;
       
       DTD2SpdZ2    = DTDSpdIn;                            
       DTD2SpdZ1    = DTDSpdIn;                            
       DTD2TrqCmdZ2 = 0.0;                            
       DTD2TrqCmdZ1 = 0.0;                            
       DTD2TrqCmd   = 0.0;
    }


/////////////////////////////////////////////////
// Drive Train Damper Algorithm Output
/////////////////////////////////////////////////        

    DTD1TrqTemp = DTD2TrqTemp = 0.0F; 
    if ( DTD1Enb)
    {
        DTD1TrqTemp = DTD1TrqCmd; 
    } 

    if ( DTD2Enb)
    {
        DTD2TrqTemp = DTD2TrqCmd; 
    } 

    DTDTrqCmdTot = DTD1TrqTemp  + DTD2TrqTemp;


    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION   
//   Performs drive train damper background function.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void DriveTrainDamperBgnd(void)
{
        

    if ( PARM(Net_Type) != NET_ABSENT)
    {
        DTD1K   = DTD1KRem; 
        DTD1Z   = DTD1ZRem; 
        DTD1F   = DTD1FRem; 
        DTD1T   = DTD1TRem; 
        DTD2K   = DTD2KRem; 
        DTD2Z   = DTD2ZRem; 
        DTD2F   = DTD2FRem; 
        DTD2T   = DTD2TRem; 

    }
    else     // MarkIVe IOnet
    {
        DTD1K = PARM(DTD1KLcl); 
        DTD1Z = PARM(DTD1ZLcl); 
        DTD1F = PARM(DTD1FLcl); 
        DTD1T = PARM(DTD1TLcl); 
        DTD2K = PARM(DTD2KLcl); 
        DTD2Z = PARM(DTD2ZLcl); 
        DTD2F = PARM(DTD2FLcl); 
        DTD2T = PARM(DTD2TLcl); 
    }

/////////////////////////////////////////////////
//Correlated Parameters for DTD #1
/////////////////////////////////////////////////
    DTD1KC       = 1000.0F * DTD1K; 
    DTD1Zeta     = DTD1Z;     
    DTD1Omega    = PI2 * DTD1F;  
    DTD1Tau      = DTD1T;  
    DTD1TauDtRcp = DTD1Tau*DelTm2Rcp;  

/////////////////////////////////////////////////
// Constant calculations for DTD #1
/////////////////////////////////////////////////
    // L_D1 = 4 + 4*zeta*w*dt + (w*dt)^2
    DTD1WDtSq    = (DTD1Omega * DelTm2) * (DTD1Omega * DelTm2);
    DTD14ZetaWdt = 4.0F * DTD1Omega * DelTm2 * DTD1Zeta;

    // L_D1 = 4 + 4*zeta*w*dt + (w*dt)^2
    DTD1LD1      = 4.0F + DTD14ZetaWdt + DTD1WDtSq;
    DTD1LD1Rcp   = 1.0F/DTD1LD1;     

    // L_N1 = (4*zeta*w*dt) * (2*tau/dt + 1) / L_D1
    DTD1LN1      = DTD1LD1Rcp * (DTD14ZetaWdt * (1.0F + (2.0F * DTD1TauDtRcp) )  );

    // L_N2 = (-4*zeta*w*dt) * (4*tau/dt) / L_D1
    DTD1LN2      = DTD1LD1Rcp *(-4.0F * DTD14ZetaWdt * DTD1TauDtRcp);

    // L_N3 = (4*zeta*w*dt) * (2*tau/dt - 1) / L_D1  = -(L_D1 + L_D2)
    //DTD1LN3      = DTD1LD1Rcp * (DTD14ZetaWdt * ((2.0 * DTD1TauDtRcp) - 1.0 )  );
    // Sum of LN1, LN2, LN3 must equal zero, or LN3 = - (LN1 + LN2) or LN3 = -0.001*(1,000*LN1 + 1,000*LN2)
    DTD1LN3Est  = - 0.001F*(1000.0F*DTD1LN1 + 1000.0F*DTD1LN2); //1,000 multiplier helps resoltion 
    DTD1LN3Err  = 2000.0F*DTD1LN1 + 2000.0F*DTD1LN2 + 2000.0F*DTD1LN3Est;  // test for error with an input of 2,000rpm
    DTD1LN3Adjst= -0.0005F*DTD1LN3Err;                                   // assume adjustment of the "LN3" term to correct error
    DTD1LN3     = DTD1LN3Est + DTD1LN3Adjst;                            // add adjustment to correct LN3 term 



    // L_D2 = -2 * ((w *dt)^2] - 4)/ L_D1
    DTD1LD2      = DTD1LD1Rcp * (-2.0F * (DTD1WDtSq - 4.0F)) ;

    // L_D3 = -[4 - (4*zeta*w*dt) + (w *dt)2]/ L_D1
    DTD1LD3      = -DTD1LD1Rcp * ( DTD1WDtSq + (4.0F - DTD14ZetaWdt) );



/////////////////////////////////////////////////
//Correlated Parameters for DTD #2
/////////////////////////////////////////////////
    DTD2KC       = 1000.0F * DTD2K; 
    DTD2Zeta     = DTD2Z;     
    DTD2Omega    = PI2 * DTD2F;  
    DTD2Tau      = DTD2T;  
    DTD2TauDtRcp = DTD2Tau*DelTm2Rcp;  

/////////////////////////////////////////////////
// Constant calculations for DTD #2
/////////////////////////////////////////////////
    // L_D1 = 4 + 4*zeta*w*dt + (w*dt)^2
    DTD2WDtSq    = (DTD2Omega * DelTm2) * (DTD2Omega * DelTm2);
    DTD24ZetaWdt = 4.0F * DTD2Omega * DelTm2 * DTD2Zeta;

    // L_D2 = 4 + 4*zeta*w*dt + (w*dt)^2
    DTD2LD1      = 4.0F + DTD24ZetaWdt + DTD2WDtSq;
    DTD2LD1Rcp   = 1.0F/DTD2LD1;     

    // L_N1 = (4*zeta*w*dt) * (2*tau/dt + 1) / L_D1
    DTD2LN1      = DTD2LD1Rcp * (DTD24ZetaWdt * (1.0F + (2.0F * DTD2TauDtRcp) )  );

    // L_N2 = (-4*zeta*w*dt) * (4*tau/dt) / L_D1
    DTD2LN2      = DTD2LD1Rcp *(-4.0F * DTD24ZetaWdt * DTD2TauDtRcp);

    // L_N3 = (4*zeta*w*dt) * (2*tau/dt - 1) / L_D1  = -(L_D1 + L_D2)
    //DTD2LN3      = DTD2LD1Rcp * (DTD24ZetaWdt * ((2.0 * DTD2TauDtRcp) - 1.0 )  );
    //DTD2LN3      = - 0.0002*(5000.0*DTD2LN1 + 5000.0*DTD2LN2); //5,000 multiplier helps resoltion  

    DTD2LN3Est  = - 0.001F*(1000.0F*DTD2LN1 + 1000.0F*DTD2LN2);               
    DTD2LN3Err  = 2000.0F*DTD2LN1 + 2000.0F*DTD2LN2 + 2000.0F*DTD2LN3Est; // test for error with an input of 2,000rpm
    DTD2LN3Adjst= -0.0005F*DTD2LN3Err;                                    // assume adjustment of the "LN3" term to correct error
    DTD2LN3     = DTD2LN3Est + DTD2LN3Adjst;                              // add adjustment to correct LN3 term 




    // L_D2 = -2 * ((w *dt)^2] - 4)/ L_D1
    DTD2LD2      = DTD2LD1Rcp * (-2.0F * (DTD2WDtSq - 4.0F)) ;

    // L_D3 = -[4 - (4*zeta*w*dt) + (w *dt)2]/ L_D1
    DTD2LD3      = -DTD2LD1Rcp * ( DTD2WDtSq + (4.0F - DTD24ZetaWdt) );

}



