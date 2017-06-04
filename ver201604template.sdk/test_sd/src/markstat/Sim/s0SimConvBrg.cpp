///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Converter Bridge Isim class.
//
// DESCRIPTION
//      This file contains the definition for the class.
//
// COPYRIGHT    Copyright (c) 2014
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      cSimBrg.h, cSimBrg.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
#include "s0SimConvBrg.h"



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class constructor to initialize resources
//
// EXECUTION LEVEL
//      Class instancing.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cSimBrg::cSimBrg
(
    SimBrgInTyp    InList,
    SimBrgInsTyp   InsList
)
        : In(InList), Ins(InsList)
{

    OpenPhaFltSim = 0;  // Open phase fault states, for test only
    OpenPhbFltSim = 0;
    OpenPhcFltSim = 0;  //

    IaSgnSim = 0;       // Phase current direction signs
    IbSgnSim = 0;
    IcSgnSim = 0;	 
    IaSgnSimZ1 = 0; 
    IbSgnSimZ1 = 0;
    IcSgnSimZ1 = 0;
    
    
    SaBrgSim = LEG_OFF;       // Phase A diode switching states  
    SbBrgSim = LEG_OFF;		  // Phase B diode switching states  
    ScBrgSim = LEG_OFF;       // Phase C diode switching states  
    
    
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction.
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
cSimBrg::~cSimBrg ()
{
    
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Bridge AC current sign detection.
//      Current follows into bridge is defined as the positive sign.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
 void cSimBrg::BrgAcCurSgn(void) 
{

    IaSgnSimZ1 = IaSgnSim;
    IbSgnSimZ1 = IbSgnSim;
    IcSgnSimZ1 = IcSgnSim;
 
    IaSgnSim = SGNZ( IN(IaBrgSim) );
    IbSgnSim = SGNZ( IN(IbBrgSim) );
    IcSgnSim = SGNZ( IN(IcBrgSim) );

}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Converter bridge output voltage calculation.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimBrg::BrgVoltsFbk(void) 
{
    float VmnBrgSim;
	
    VmnBrgSim = (VaBrgSim + VbBrgSim + VcBrgSim) * 0.3333333;

	// cancel common-mode component
    VaBrgSim = VaBrgSim - VmnBrgSim;
    VbBrgSim = VbBrgSim - VmnBrgSim;
    VcBrgSim = VcBrgSim - VmnBrgSim;

    OUT(VabBrgSim) = VaBrgSim - VbBrgSim;
    OUT(VbcBrgSim) = VbBrgSim - VcBrgSim;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Bridge DC current generation.
//
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
 void cSimBrg::BrgDcCurSim(void) 
 {

    float VxBrgSim, VyBrgSim;
    float IxBrgSim, IyBrgSim;
    float VdcSimRcp, BrgDcPwrSim;
    
    // bridge voltage alpha-beta components    
    cVectorf::ABCToAlphaBeta(VaBrgSim, VbBrgSim, VcBrgSim, &VxBrgSim, &VyBrgSim);
    cVectorf::ABCToAlphaBeta(IN(IaBrgSim), IN(IbBrgSim), IN(IcBrgSim), &IxBrgSim, &IyBrgSim);
    
 
    if ( IN(BrgEnbT1) )
    {
        // Active power from bridge to DC link
        BrgDcPwrSim  =  1.5 * (VxBrgSim * IxBrgSim + VyBrgSim * IyBrgSim) - IN(BrgLossPwr);       
        VdcSimRcp = NoNANLim( 1.0/ IN(VdcSim), 1.0e6F, -1.0e6F);        
        OUT(IdcBrgSim) = BrgDcPwrSim * VdcSimRcp ;  
    }
    else
    {
        IdcBrgSim = 0.0;
        if ((SaBrgSim == UPPER_ON) && (IN(IaBrgSim) > 0.0))    
        {
            OUT(IdcBrgSim) += IN(IaBrgSim);
        }
        if ((SbBrgSim == UPPER_ON) && (IN(IbBrgSim) > 0.0)) 
        {
            OUT(IdcBrgSim) += IN(IbBrgSim);
        }
        if ((ScBrgSim == UPPER_ON) && (IN(IcBrgSim) > 0.0))    
        {
            OUT(IdcBrgSim) += IN(IcBrgSim);
        }
    }
    
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function simulates the bridge PWM and IOC behaviour.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////

void cSimBrg::PwmIocMode(void) 
{
    // Calculate diode switching states from phase current direction
    SaBrgSim = IaSgnSim;
    SbBrgSim = IbSgnSim;
    ScBrgSim = IcSgnSim; 
 
    // Bridge voltage calculation for PWM and IOC retry 
    if ( IN(AiocStat) )
    {
        VaBrgSim = ((float) SaBrgSim) * VdcSimBy2;     
    }
    else
    {
        VaBrgSim = VanCmdSim ;
    }

    if ( IN(BiocStat ) )
    {
        VbBrgSim = ((float) SbBrgSim) * VdcSimBy2;        
    }
    else
    {
        VbBrgSim = VbnCmdSim ;
    } 
    
    if ( IN(CiocStat) )
    {
        VcBrgSim = ((float) ScBrgSim) * VdcSimBy2;        
    }
    else
    {
        VcBrgSim = VcnCmdSim ;
    }
    
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function simulates the bridge diode behaviour when IGBTs are disable.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////

void cSimBrg::DiodeMode(void) 
{
    float  VPosSim, VMidSim, VNegSim;
    float  SPosSim, SMidSim, SNegSim; 
    float  VRectSim, VcomBrgSim, VmidBrgSim;    
    unsigned  DiodeBrgStat;
    unsigned  BrgVoltSelSim;

    // Bridge states judgement according to diode switching states
    if ( (SaBrgSim == LEG_OFF) && (SbBrgSim == LEG_OFF) && (ScBrgSim == LEG_OFF) )
    {
        DiodeBrgStat = BRG_NO_COND;  // Diode DCM, Legs A, B and C block
    }
    else if ( (SaBrgSim == LEG_OFF) && (SbBrgSim != LEG_OFF) && (ScBrgSim != LEG_OFF) )
    {
        DiodeBrgStat = BRG_BC_COND;  // Legs B and C conduct, and leg A blocks
    }
    else if ( (SbBrgSim == LEG_OFF) && (SaBrgSim != LEG_OFF) && (ScBrgSim != LEG_OFF) )
    {
        DiodeBrgStat = BRG_AC_COND;  // Legs A and C conduct, and leg B blocks
    }
    else if ( (ScBrgSim == LEG_OFF) && (SaBrgSim != LEG_OFF) && (SbBrgSim != LEG_OFF) )
    {
        DiodeBrgStat = BRG_AB_COND;  // Legs A and B conduct, and leg C blocks
    }
    else if ( (SaBrgSim != LEG_OFF) && (SbBrgSim != LEG_OFF) && (ScBrgSim != LEG_OFF) )
    {
        DiodeBrgStat = BRG_ABC_COND;  // Diode CCM, Legs A, B and C conduct
    }
    else
    {
        DiodeBrgStat = BRG_ERROR;  // Error state, should not appear
    }
 

    //----------------------------------------------------------------------
    //   Update diode states from the inputs of bridge voltage and current
    //----------------------------------------------------------------------
    switch(DiodeBrgStat)
    {
        case BRG_NO_COND:  // Diode DCM  
            BrgVoltSelSim = (IN(VanSrcSim) > IN(VbnSrcSim) ? 4 : 0) +
                            (IN(VbnSrcSim) > IN(VcnSrcSim) ? 2 : 0) +
                            (IN(VcnSrcSim) > IN(VanSrcSim) ? 1 : 0); 
            
            // Open phase cases            
            if ( (OpenPhaFltSim + OpenPhbFltSim + OpenPhcFltSim) >= 2 )
            {
                BrgVoltSelSim = 0;                  
            }
            else if (OpenPhaFltSim == 1)
            { 
                BrgVoltSelSim = IN(VbnSrcSim) > IN(VcnSrcSim) ? 2 : 5;    
            } 
            else if (OpenPhbFltSim == 1)
            {
                BrgVoltSelSim = IN(VanSrcSim) > IN(VcnSrcSim) ? 6 : 1;    
            }
            else if (OpenPhcFltSim == 1)
            {                  
                BrgVoltSelSim = IN(VanSrcSim) > IN(VbnSrcSim) ? 4 : 3;    
            }

            switch (BrgVoltSelSim)
            {
                case 6:     //a>b>c, 
                    VPosSim  = IN(VanSrcSim);
                    VMidSim  = IN(VbnSrcSim);
                    VNegSim  = IN(VcnSrcSim);
                    break;
                case 4:     //a>c>b
                    VPosSim  = IN(VanSrcSim);
                    VMidSim  = IN(VcnSrcSim);
                    VNegSim  = IN(VbnSrcSim);
                    break;
                case 2:     //b>a>c
                    VPosSim  = IN(VbnSrcSim);
                    VMidSim  = IN(VanSrcSim);
                    VNegSim  = IN(VcnSrcSim);
                    break;
                case 3:     //b>c>a
                    VPosSim  = IN(VbnSrcSim);
                    VMidSim  = IN(VcnSrcSim);
                    VNegSim  = IN(VanSrcSim);           
                    break;
                case 5:     //c>a>b
                    VPosSim  = IN(VcnSrcSim);
                    VMidSim  = IN(VanSrcSim);
                    VNegSim  = IN(VbnSrcSim);      
                    break;
                case 1:     //c>b>a
                    VPosSim  = IN(VcnSrcSim);
                    VMidSim  = IN(VbnSrcSim);
                    VNegSim  = IN(VanSrcSim);
                    break;
                default:    //a=b=c
                    VPosSim  = 0.0;
                    VMidSim  = 0.0;
                    VNegSim  = 0.0;
            }       
            
            VRectSim = VPosSim - VNegSim;
            if (VRectSim > IN(VdcSim))
            {
                SPosSim = UPPER_ON;    // upper cell on
                SNegSim = LOWER_ON;    // lower cell on                   
                
                // phase voltage of OFF-leg referred to the middle point of dc-link capacitor
                VmidBrgSim = VMidSim - (VPosSim + VNegSim) * 0.5;
                if (VmidBrgSim > VdcSimBy2)
                {
                    SMidSim = UPPER_ON;                
                }
                else if (VmidBrgSim < (-VdcSimBy2) )
                {
                    SMidSim = LOWER_ON;                
                }
                else
                {                
                    SMidSim = LEG_OFF;
                }
            }
            else
            {
                SPosSim = LEG_OFF;
                SMidSim = LEG_OFF;                
                SNegSim = LEG_OFF; 
            }
            
            switch (BrgVoltSelSim)
            {
                case 6:     //a>b>c, 
                    SaBrgSim  = SPosSim;
                    SbBrgSim  = SMidSim;
                    ScBrgSim  = SNegSim;
                    break;
                case 4:     //a>c>b
                    SaBrgSim  = SPosSim;
                    SbBrgSim  = SNegSim;
                    ScBrgSim  = SMidSim;
                    break;
                case 2:     //b>a>c
                    SaBrgSim  = SMidSim;
                    SbBrgSim  = SPosSim;
                    ScBrgSim  = SNegSim;
                    break;
                case 3:     //b>c>a
                    SaBrgSim  = SNegSim;
                    SbBrgSim  = SPosSim;
                    ScBrgSim  = SMidSim;     
                    break;
                case 5:     //c>a>b
                    SaBrgSim  = SMidSim;
                    SbBrgSim  = SNegSim;
                    ScBrgSim  = SPosSim;        
                    break;
                case 1:     //c>b>a
                    SaBrgSim  = SNegSim;
                    SbBrgSim  = SMidSim;
                    ScBrgSim  = SPosSim;
                    break;
                default:    //a=b=c
                    SaBrgSim  = LEG_OFF;
                    SbBrgSim  = LEG_OFF;
                    ScBrgSim  = LEG_OFF;
            }    
            break;
        
        case BRG_BC_COND:  // Phase Legs B and C conduct        
            VmidBrgSim = IN(VanSrcSim) - (IN(VbnSrcSim) + IN(VcnSrcSim)) * 0.5;
            if (VmidBrgSim > VdcSimBy2)
            {
                SaBrgSim = UPPER_ON;                
            }
            else if (VmidBrgSim < (-VdcSimBy2) )
            {
                SaBrgSim = LOWER_ON;                
            }
            else
            {                
                SaBrgSim = LEG_OFF;
            }
            
            // turn off diode when current zero-crossing
            if (( (IbSgnSimZ1 * IbSgnSim) <= 0) && (IbSgnSimZ1 != 0))  
            {   
                SbBrgSim = LEG_OFF; 
            }
            if ( ((IcSgnSimZ1 * IcSgnSim) <= 0) && (IcSgnSimZ1 != 0)) 
            { 
                ScBrgSim = LEG_OFF; 
            }
            break;  
        
        case BRG_AC_COND:  // Phase Legs A and C conduct
            VmidBrgSim = IN(VbnSrcSim) - (IN(VanSrcSim) + IN(VcnSrcSim)) * 0.5;            
            if (VmidBrgSim > VdcSimBy2)
            {
                SbBrgSim = UPPER_ON;                
            }
            else if (VmidBrgSim < (-VdcSimBy2) )
            {
                SbBrgSim = LOWER_ON;                
            }
            else
            {                
                SbBrgSim = LEG_OFF;
            }
                
            if(( (IaSgnSimZ1 * IaSgnSim) <= 0) && (IaSgnSimZ1 != 0)) 
            { 
                SaBrgSim = LEG_OFF; 
            }
            if( ((IcSgnSimZ1 * IcSgnSim) <= 0) && (IcSgnSimZ1 !=0 )) 
            { 
                ScBrgSim = LEG_OFF;
            }  
            break;
        
        case BRG_AB_COND:  // Phase Legs A and B conduct
            VmidBrgSim = IN(VcnSrcSim) - (IN(VanSrcSim) + IN(VbnSrcSim)) * 0.5;
            if (VmidBrgSim > VdcSimBy2)
            {
                ScBrgSim = UPPER_ON;                
            }
            else if (VmidBrgSim < (-VdcSimBy2) )
            {
                ScBrgSim = LOWER_ON;                
            }
            else
            {                
                ScBrgSim = LEG_OFF;
            }

            if ( ((IaSgnSimZ1 * IaSgnSim) <= 0) && (IaSgnSimZ1 !=0)) 
            {           
                SaBrgSim = LEG_OFF;
            }
            if ( ((IbSgnSimZ1 * IbSgnSim) <= 0) && (IbSgnSimZ1 !=0)) 
            { 
                SbBrgSim = LEG_OFF;
            }  
            break;
        
        case BRG_ABC_COND:  // Three-phases conduct
            if (( (IaSgnSimZ1 * IaSgnSim) <= 0) && (IaSgnSimZ1 != 0))  
            { 
                VmidBrgSim = IN(VanSrcSim) - (IN(VbnSrcSim) + IN(VcnSrcSim)) * 0.5;
                if (VmidBrgSim > VdcSimBy2)
                {
                    SaBrgSim = UPPER_ON;                
                }
                else if (VmidBrgSim < (-VdcSimBy2) )
                {
                    SaBrgSim = LOWER_ON;                
                }
                else
                {                
                    SaBrgSim = LEG_OFF;
                }
            }

            if (( (IbSgnSimZ1 * IbSgnSim) <= 0) && (IbSgnSimZ1 != 0)) 
            { 
                VmidBrgSim = IN(VbnSrcSim) - (IN(VanSrcSim) + IN(VcnSrcSim)) * 0.5;
                if (VmidBrgSim > VdcSimBy2)
                {
                    SbBrgSim = UPPER_ON;                
                }
                else if (VmidBrgSim < (-VdcSimBy2) )
                {
                    SbBrgSim = LOWER_ON;                
                }
                else
                {                
                    SbBrgSim = LEG_OFF;
                }
            }
            
            if (( (IcSgnSimZ1 * IcSgnSim) <= 0) && (IcSgnSimZ1 != 0)) 
            { 
                VmidBrgSim = IN(VcnSrcSim) - (IN(VanSrcSim) + IN(VbnSrcSim)) * 0.5;
                if (VmidBrgSim > VdcSimBy2)
                {
                    ScBrgSim = UPPER_ON;                
                }
                else if (VmidBrgSim < (-VdcSimBy2) )
                {
                    ScBrgSim = LOWER_ON;                
                }
                else
                {                
                    ScBrgSim = LEG_OFF;
                }
            } 
            break;
        
        case BRG_ERROR:
        default: 
            SaBrgSim = LEG_OFF;
            SbBrgSim = LEG_OFF;
            ScBrgSim = LEG_OFF; 
        
    }
    
    // Block the legs when open-phase fault 
    if (OpenPhaFltSim == 1) 
    {
        SaBrgSim = LEG_OFF;
    }        
    if (OpenPhbFltSim == 1)
    {
        SbBrgSim = LEG_OFF;
    }
    if (OpenPhcFltSim == 1)
    {
        ScBrgSim = LEG_OFF;
    }
    
    
    // Block the whole bridge when 2-ph legs have been blocked
    if((SaBrgSim == LEG_OFF) && (SbBrgSim == LEG_OFF))   
    {
        ScBrgSim = LEG_OFF;
    }    
    else if((SbBrgSim == LEG_OFF) && (ScBrgSim == LEG_OFF))
    {
        SaBrgSim = LEG_OFF;
    }
    else if((ScBrgSim == LEG_OFF) && (SaBrgSim == LEG_OFF)) 
    {
        SbBrgSim = LEG_OFF;              
    }
    
    //-------------------------------------------------------------------------
    //                      3-phase bridge voltage generation
    //-------------------------------------------------------------------------

    if( (SaBrgSim == LEG_OFF) && (SbBrgSim == LEG_OFF) && (ScBrgSim == LEG_OFF) ) 
    {
        VaBrgSim = IN(VanSrcSim);
        VbBrgSim = IN(VbnSrcSim);
        VcBrgSim = IN(VcnSrcSim); 
    }
    else if( (SaBrgSim == LEG_OFF) && (SbBrgSim != LEG_OFF) && (ScBrgSim !=LEG_OFF ) ) 
    {
        VcomBrgSim = (IN(VbnSrcSim) + IN(VcnSrcSim)) * 0.5;
        VaBrgSim = IN(VanSrcSim);
        VbBrgSim = SbBrgSim * VdcSimBy2 + VcomBrgSim;
        VcBrgSim = ScBrgSim * VdcSimBy2 + VcomBrgSim;     
    }
    else if( (SbBrgSim == LEG_OFF) && (SaBrgSim != LEG_OFF) && (ScBrgSim != LEG_OFF))  
    {
        VcomBrgSim = (IN(VanSrcSim) + IN(VcnSrcSim)) * 0.5;
        VaBrgSim = SaBrgSim * VdcSimBy2 + VcomBrgSim;
        VbBrgSim = IN(VbnSrcSim);
        VcBrgSim = ScBrgSim * VdcSimBy2 + VcomBrgSim;     
    }
    else if( (ScBrgSim == LEG_OFF) && (SaBrgSim != LEG_OFF) && (SbBrgSim != LEG_OFF) ) 
    {
        VcomBrgSim = (IN(VanSrcSim) + IN(VbnSrcSim)) * 0.5;
        VaBrgSim = SaBrgSim * VdcSimBy2 + VcomBrgSim;
        VbBrgSim = SbBrgSim * VdcSimBy2 + VcomBrgSim;
        VcBrgSim = IN(VcnSrcSim);    
    }
    else if( (SaBrgSim != LEG_OFF) && (SbBrgSim != LEG_OFF) && (ScBrgSim != LEG_OFF) ) 
    {
     
        VcomBrgSim = (IN(VanSrcSim) + IN(VbnSrcSim) + IN(VcnSrcSim)) / 3.0 - (SaBrgSim + SbBrgSim + ScBrgSim) * IN(VdcSim) / 6.0;
        VaBrgSim = SaBrgSim * VdcSimBy2 + VcomBrgSim;
        VbBrgSim = SbBrgSim * VdcSimBy2 + VcomBrgSim;
        VcBrgSim = ScBrgSim * VdcSimBy2 + VcomBrgSim;
    }
    

}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the converter bridge model. 

// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////

void cSimBrg::ConvBrgSim(void)
{
    //-----------------------------------------------------------------------
    // Converter bridge model inputs:
    //   a.  Bridge voltage commands, bridge source voltages, 3-phase currents
    //   b.  DC voltage
    //   c.  IOC state, Bridge enable state
	//
	// Outputs:
	//	 a.  Bridge voltages
	//   b.  Bridge dc current
    //-----------------------------------------------------------------------

    float Vcomm;
    
    Vcomm = ( IN(VaCmdSim) + IN(VbCmdSim) + IN(VcCmdSim) ) * 0.33333333;
    
    // Cancel common mode component for the bridge voltage SVM commands
    //VanCmdSim = (2.0F/3.0F) * ( IN(VaCmdSim) - IN(VbCmdSim) ) + (1.0F/3.0F) * ( IN(VbCmdSim) - IN(VcCmdSim) );
    //VbnCmdSim = -(1.0F/3.0F) * ( IN(VaCmdSim) - IN(VbCmdSim) ) + (1.0F/3.0F) * ( IN(VbCmdSim) - IN(VcCmdSim) );
    //VcnCmdSim = -VanCmdSim - VbnCmdSim;

    VanCmdSim = IN(VaCmdSim) - Vcomm;
    VbnCmdSim = IN(VbCmdSim) - Vcomm;
    VcnCmdSim = IN(VcCmdSim) - Vcomm;
    
    
    VdcSimBy2 = IN(VdcSim) * 0.5;

    BrgAcCurSgn() ;      // bridge current sign

    if ( IN(BrgEnbT1) )
    {
        PwmIocMode();   //PWM and IOC Mode
    }
    else
    {
        DiodeMode();    //Diode Mode
    }

    BrgVoltsFbk();      // Simulate bridge voltage
    BrgDcCurSim();      // Simulate bridge dc current

}




