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


// Include Files
//--------------

// system
// core
// framework
// product
#include "s0XfmrSim.h"



// Constants
//----------


// Class Instances
//----------------


extern float TestTrans1;
extern float TestTrans2;

extern float TestTrans3;


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
cXfmrSim::cXfmrSim
(
        InTyp    InList,
        InsTyp   InsList
)
        : In(InList), Ins(InsList)
{
    XfmrLeakFlxASim = 0.0;
    XfmrLeakFlxBSim = 0.0;
    XfmrLeakFlxCSim = 0.0;
    IaXfmrPri2SecSim = 0.0;
    IbXfmrPri2SecSim = 0.0;
    IcXfmrPri2SecSim = 0.0;
    IaXfmrPriSim = 0.0;
    IbXfmrPriSim = 0.0;
    IcXfmrPriSim = 0.0;
    LIaXfmrSecSim = 0.0;
    LIbXfmrSecSim = 0.0;
    LIcXfmrSecSim = 0.0;
    IaXfmrMagSim = 0.0;
    IbXfmrMagSim = 0.0;
    IcXfmrMagSim = 0.0;
    XfmrFlxASim = 0.0;
    XfmrFlxBSim = 0.0;
    XfmrFlxCSim = 0.0;
 //   FlxDampCnt = 0;
    
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
cXfmrSim::~cXfmrSim ()
{
    
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <method description here>
//
// EXECUTION LEVEL
//      Task #
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cXfmrSim::RunRules()
{
    float TmpRatioSqr;
    unsigned int i;
    // Transformer
    SET_FV(VxfmrPriSim,     INS(VxfmrPri));
    SET_FV(RxfmrPriSim,     INS(RxfmrPri));
    SET_FV(LxfmrPriSim,     INS(LxfmrPri));
    SET_FV(LnVxfmrSecSim,   INS(L_VxfmrSec));
    SET_FV(LnRxfmrSecSim,   INS(L_RxfmrSec));
    SET_FV(LnLxfmrSecSim,   INS(L_LxfmrSec)); 
    TmpRatioSqr = NoNANLim( (PARM(LnVxfmrSecSim)*PARM(LnVxfmrSecSim)) / (PARM(VxfmrPriSim)*PARM(VxfmrPriSim)), 1.0e6F, -1.0e6F );
    SET_FV(LnRxfmrPriSim,     PARM(RxfmrPriSim)*TmpRatioSqr); // referring to secondary winding
    SET_FV(LnLxfmrPriSim,     PARM(LxfmrPriSim)*TmpRatioSqr); // referring to secondary winding
    SET_FV(LnLeakSim,         (PARM(LnLxfmrPriSim) + PARM(LnLxfmrSecSim))); // referring to secondary winding
    SET_FV(XfmrFlxRat,   (PARM(LnVxfmrSecSim) * SQRT2_3 / INS(XfmrWElec))); // 
    SET_FV(XfmrIdRat, INS(XfmrKVASim) /PARM(LnVxfmrSecSim) *SQRT2_3);


   // XfmrFlxASim = INS(L_FlxAInit);
   // XfmrFlxBSim = INS(L_FlxBInit);
   // XfmrFlxCSim = INS(L_FlxCInit);
    
    XfmrFlxGroup[0] = INS(L_XfmrSatCurvFlxK0Sim);
    XfmrFlxGroup[1] = INS(L_XfmrSatCurvFlxK1Sim);
    XfmrFlxGroup[2] = INS(L_XfmrSatCurvFlxK2Sim);
    XfmrFlxGroup[3] = INS(L_XfmrSatCurvFlxK3Sim);
    XfmrFlxGroup[4] = INS(L_XfmrSatCurvFlxK4Sim);
    XfmrFlxGroup[5] = INS(L_XfmrSatCurvFlxK5Sim);
    XfmrFlxGroup[6] = INS(L_XfmrSatCurvFlxK6Sim);
    XfmrFlxGroup[7] = INS(L_XfmrSatCurvFlxK7Sim);
    XfmrFlxGroup[8] = INS(L_XfmrSatCurvFlxK8Sim);
    XfmrFlxGroup[9] = INS(L_XfmrSatCurvFlxK9Sim);

    XfmrImagGroup[0] = INS(L_XfmrSatCurvImagK0Sim);
    XfmrImagGroup[1] = INS(L_XfmrSatCurvImagK1Sim);
    XfmrImagGroup[2] = INS(L_XfmrSatCurvImagK2Sim);
    XfmrImagGroup[3] = INS(L_XfmrSatCurvImagK3Sim);
    XfmrImagGroup[4] = INS(L_XfmrSatCurvImagK4Sim);
    XfmrImagGroup[5] = INS(L_XfmrSatCurvImagK5Sim);
    XfmrImagGroup[6] = INS(L_XfmrSatCurvImagK6Sim);
    XfmrImagGroup[7] = INS(L_XfmrSatCurvImagK7Sim);
    XfmrImagGroup[8] = INS(L_XfmrSatCurvImagK8Sim);
    XfmrImagGroup[9] = INS(L_XfmrSatCurvImagK9Sim);
    for (i = 0; i< INS(L_XfmrSatCurvNumSim)-1; i++)
        XfmrRampRat[i] = (XfmrImagGroup[i+1] - XfmrImagGroup[i] ) * NoNANLim(1.0F/(XfmrFlxGroup[i+1] - XfmrFlxGroup[i] ), 1.0e6F,-1.0e6F);

} 


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <method description here>
//
// EXECUTION LEVEL
//      Task #
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cXfmrSim::XfmrSatCurve(float* L_XfmrImagSim, float L_VXfmrMagSim, float*  L_XfmrFlxSim)
{

    float XfmrFlxPuSim = 0.0;
    float XfmrImagPuSim = 0.0;
    unsigned int i = 0;
    
  /*  if(FlxDampCnt <50000)
        *XfmrFlxSim += (L_VXfmrMagSim - *XfmrFlxSim*10.0) * INS(DelTm1);
    else
    *XfmrFlxSim += L_VXfmrMagSim * INS(DelTm1);
    */
    
    *L_XfmrFlxSim += (L_VXfmrMagSim - *L_XfmrImagSim*INS(L_RmagXfmrSim)) * INS(DelTm1);
    if(IN(IsimSmplN)==0)
    {
        XfmrFlxPuSim = fabs(*L_XfmrFlxSim / PARM(XfmrFlxRat));
         
        if ((INS(XfmrSatEnb) == TRUE)&&(IN(BRchg)==0))     
        {                  
            if (INS(XfmrSatCrvType) == SAT_CURVE)        
            {            
                           
                if(XfmrFlxPuSim > XfmrFlxGroup[INS(L_XfmrSatCurvNumSim)-1])                      
                {                                       
                    XfmrFlxPuSim = XfmrFlxGroup[INS(L_XfmrSatCurvNumSim)-1];                             
                    XfmrImagPuSim = XfmrImagGroup[INS(L_XfmrSatCurvNumSim)-1];                       
                }                       
                else if(XfmrFlxPuSim < XfmrFlxGroup[0])                     
                {                               
                    XfmrFlxPuSim = XfmrFlxGroup[0];                             
                    XfmrImagPuSim = XfmrImagGroup[0];                       
                }                       
                else                        
                {                               
                    for(i=0; i<INS(L_XfmrSatCurvNumSim)-1; i++)                              
                    {                                       
                        if(XfmrFlxPuSim <= XfmrFlxGroup[i+1])                                       
                        {                                               
                            XfmrImagPuSim = XfmrImagGroup[i] + XfmrRampRat[i] * (XfmrFlxPuSim - XfmrFlxGroup[i]);                                              
                            break;                                      
                        }                               
                    }                           
                }    
                if(*L_XfmrFlxSim < 0.0)
                    XfmrImagPuSim = -XfmrImagPuSim;
                *L_XfmrImagSim = XfmrImagPuSim * PARM(XfmrIdRat);               
            }       
            else if (INS(XfmrSatCrvType) == SAT_IDEAL)        
            {            

                if(XfmrFlxPuSim < INS(L_XfmrSatThr))
                    *L_XfmrImagSim = 0.0; 
                else            
                    *L_XfmrImagSim = L_VXfmrMagSim /INS(L_XfmrSatXm); 
            }       
            
        }   
        else    
        {            
            *L_XfmrImagSim = 0.0;        
        }    
    }


} 
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      <method description here>
//
// EXECUTION LEVEL
//      Task #
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cXfmrSim::XfmrModel()
{

    float IsatCmSim = 0.0;
    RhfXfmrSim = INS(RhfXfmrPuSim) * IN(WElecSim) * PARM(LnLeakSim);



  /*      if(FlxDampCnt<=50000) 
            FlxDampCnt++;
        else
            FlxDampCnt = 51000;
            */
   
    if(IN(XfmrState)==0) //normal mode
    {
        VaXfmrPri2SecSim = (IN(VaXfmrPriSim) - IN(VbXfmrPriSim)) / INS(XfmrRatioSim);
        VbXfmrPri2SecSim = (IN(VbXfmrPriSim) - IN(VcXfmrPriSim))  / INS(XfmrRatioSim);
        VcXfmrPri2SecSim = (IN(VcXfmrPriSim) - IN(VaXfmrPriSim))  / INS(XfmrRatioSim);
    
        XfmrLeakFlxASim = XfmrLeakFlxASim + INS(DelTm1) *
                               (IN(VaXfmrSecSim) - VaXfmrPri2SecSim - ((PARM(LnRxfmrPriSim) + PARM(LnRxfmrSecSim)) * IaXfmrPri2SecSim));            
        IaLeakSim = XfmrLeakFlxASim / PARM(LnLeakSim);
        IaLeakRhfSim = (IN(VaXfmrSecSim) - VaXfmrPri2SecSim - ((PARM(LnRxfmrPriSim) + PARM(LnRxfmrSecSim)) * IaXfmrPri2SecSim)) / RhfXfmrSim;
        IaXfmrPri2SecSim = IaLeakSim + IaLeakRhfSim;

        XfmrLeakFlxBSim = XfmrLeakFlxBSim + INS(DelTm1) *
                               (IN(VbXfmrSecSim) - VbXfmrPri2SecSim - ((PARM(LnRxfmrPriSim) + PARM(LnRxfmrSecSim)) * IbXfmrPri2SecSim));            
        IbLeakSim = XfmrLeakFlxBSim / PARM(LnLeakSim);
        IbLeakRhfSim = (IN(VbXfmrSecSim) - VbXfmrPri2SecSim - ((PARM(LnRxfmrPriSim) + PARM(LnRxfmrSecSim)) * IbXfmrPri2SecSim)) / RhfXfmrSim;
        IbXfmrPri2SecSim = IbLeakSim + IbLeakRhfSim;
        
        XfmrLeakFlxCSim = XfmrLeakFlxCSim + INS(DelTm1) *
                               (IN(VcXfmrSecSim) - VcXfmrPri2SecSim - ((PARM(LnRxfmrPriSim) + PARM(LnRxfmrSecSim)) * IcXfmrPri2SecSim));            
        IcLeakSim = XfmrLeakFlxCSim / PARM(LnLeakSim);
        IcLeakRhfSim = (IN(VcXfmrSecSim) - VcXfmrPri2SecSim - ((PARM(LnRxfmrPriSim) + PARM(LnRxfmrSecSim)) * IcXfmrPri2SecSim)) / RhfXfmrSim;
        IcXfmrPri2SecSim = IcLeakSim + IcLeakRhfSim;
        
        IaXfmrLossSim = IN(VaXfmrSecSim) / INS(RmXfmrSim);
        IbXfmrLossSim = IN(VbXfmrSecSim) / INS(RmXfmrSim);
        IcXfmrLossSim = IN(VcXfmrSecSim) / INS(RmXfmrSim);

        VaXfmrPriFbkSim = IN(VaXfmrPriSim);
        VbXfmrPriFbkSim = IN(VbXfmrPriSim);
        VcXfmrPriFbkSim = IN(VcXfmrPriSim);
        VaXfmrSecFbkSim = IN(VaXfmrSecSim);
        VbXfmrSecFbkSim = IN(VbXfmrSecSim);
        VcXfmrSecFbkSim = IN(VcXfmrSecSim);

        ///////////flux saturation curve function//////////////////

        XfmrSatCurve(&IaXfmrMagSim, VaXfmrSecFbkSim, &XfmrFlxASim);
        XfmrSatCurve(&IbXfmrMagSim, VbXfmrSecFbkSim, &XfmrFlxBSim);
        XfmrSatCurve(&IcXfmrMagSim, VcXfmrSecFbkSim, &XfmrFlxCSim);

 
        

        if(IN(IsimSmplN)==0)
        {
            IsatCmSim = 1.0 /3.0 * (IaXfmrMagSim + IbXfmrMagSim + IcXfmrMagSim);
        IaXfmrMagSim = IaXfmrMagSim - IsatCmSim;
        IbXfmrMagSim = IbXfmrMagSim - IsatCmSim;
        IcXfmrMagSim = IcXfmrMagSim - IsatCmSim;
        }

        /////////////////////////////////////////////////////
        
        LIaXfmrSecSim = IaXfmrPri2SecSim + IaXfmrMagSim + IaXfmrLossSim;
        LIbXfmrSecSim = IbXfmrPri2SecSim + IbXfmrMagSim + IbXfmrLossSim;
        LIcXfmrSecSim = IcXfmrPri2SecSim + IcXfmrMagSim + IcXfmrLossSim;
        
        
        IaXfmrPriSim = (IaXfmrPri2SecSim - IcXfmrPri2SecSim)  / INS(XfmrRatioSim);
        IbXfmrPriSim = (IbXfmrPri2SecSim - IaXfmrPri2SecSim) / INS(XfmrRatioSim);
        IcXfmrPriSim = (IcXfmrPri2SecSim - IbXfmrPri2SecSim) / INS(XfmrRatioSim);  

        
    }
    else if(IN(XfmrState)==1) //secondary side open
    {
        LIaXfmrSecSim = 0.0;
        LIbXfmrSecSim = 0.0;
        LIcXfmrSecSim = 0.0;
        VaXfmrPriFbkSim = IN(VaXfmrPriSim);
        VbXfmrPriFbkSim = IN(VbXfmrPriSim);
        VcXfmrPriFbkSim = IN(VcXfmrPriSim);
        VaXfmrSecFbkSim = (IN(VaXfmrPriSim) - IN(VbXfmrPriSim)) / INS(XfmrRatioSim);
        VbXfmrSecFbkSim = (IN(VbXfmrPriSim) - IN(VcXfmrPriSim))  / INS(XfmrRatioSim);
        VcXfmrSecFbkSim = (IN(VcXfmrPriSim) - IN(VaXfmrPriSim))  / INS(XfmrRatioSim);
        
        IaXfmrLossSim = IN(VaXfmrSecSim) / INS(RmXfmrSim);
        IbXfmrLossSim = IN(VbXfmrSecSim) / INS(RmXfmrSim);
        IcXfmrLossSim = IN(VcXfmrSecSim) / INS(RmXfmrSim);
        
        ///////////flux saturation curve function//////////////////
        XfmrSatCurve(&IaXfmrMagSim, VaXfmrSecFbkSim, &XfmrFlxASim);
        XfmrSatCurve(&IbXfmrMagSim, VbXfmrSecFbkSim, &XfmrFlxBSim);
        XfmrSatCurve(&IcXfmrMagSim, VcXfmrSecFbkSim, &XfmrFlxCSim);
        
        if(IN(IsimSmplN)==0)
        {
            IsatCmSim = 1.0 /3.0 * (IaXfmrMagSim + IbXfmrMagSim + IcXfmrMagSim);
        	IaXfmrMagSim = IaXfmrMagSim - IsatCmSim;
        	IbXfmrMagSim = IbXfmrMagSim - IsatCmSim;
        	IcXfmrMagSim = IcXfmrMagSim - IsatCmSim;
        }

        /////////////////////////////////////////////////////
        
        IaXfmrPri2SecSim =  - IaXfmrMagSim - IaXfmrLossSim;
        IbXfmrPri2SecSim =  - IbXfmrMagSim - IbXfmrLossSim;
        IcXfmrPri2SecSim =  - IcXfmrMagSim - IcXfmrLossSim;
        
        IaXfmrPriSim = (IaXfmrPri2SecSim - IcXfmrPri2SecSim)  / INS(XfmrRatioSim);
        IbXfmrPriSim = (IbXfmrPri2SecSim - IaXfmrPri2SecSim) / INS(XfmrRatioSim);
        IcXfmrPriSim = (IcXfmrPri2SecSim - IbXfmrPri2SecSim) / INS(XfmrRatioSim); 
        
        XfmrLeakFlxASim = IaXfmrPri2SecSim * PARM(LnLeakSim);
        XfmrLeakFlxBSim = IbXfmrPri2SecSim * PARM(LnLeakSim);
        XfmrLeakFlxCSim = IcXfmrPri2SecSim * PARM(LnLeakSim);
        
    }
    else if(IN(XfmrState)==2) //primary side open
    {
        IaXfmrPriSim = 0.0;
        IbXfmrPriSim = 0.0;
        IcXfmrPriSim = 0.0;
        VaXfmrPriFbkSim =  (IN(VcXfmrSecSim) - IN(VaXfmrSecSim)) * INS(XfmrRatioSim) * (-1.0 / 3.0);
        VbXfmrPriFbkSim = (IN(VaXfmrSecSim) - IN(VbXfmrSecSim)) * INS(XfmrRatioSim) * (-1.0 / 3.0);
        VcXfmrPriFbkSim = (IN(VbXfmrSecSim) - IN(VcXfmrSecSim)) * INS(XfmrRatioSim) * (-1.0 / 3.0);
        
        VaXfmrSecFbkSim = IN(VaXfmrSecSim);
        VbXfmrSecFbkSim = IN(VbXfmrSecSim);
        VcXfmrSecFbkSim = IN(VcXfmrSecSim);
        
        IaXfmrLossSim = IN(VaXfmrSecSim) / INS(RmXfmrSim);
        IbXfmrLossSim = IN(VbXfmrSecSim) / INS(RmXfmrSim);
        IcXfmrLossSim = IN(VcXfmrSecSim) / INS(RmXfmrSim);
        
        IaXfmrPri2SecSim =  0.0;
        IbXfmrPri2SecSim =  0.0;
        IcXfmrPri2SecSim =  0.0;

                ///////////flux saturation curve function//////////////////
        XfmrSatCurve(&IaXfmrMagSim, VaXfmrSecFbkSim, &XfmrFlxASim);
        XfmrSatCurve(&IbXfmrMagSim, VbXfmrSecFbkSim, &XfmrFlxBSim);
        XfmrSatCurve(&IcXfmrMagSim, VcXfmrSecFbkSim, &XfmrFlxCSim);

        if(IN(IsimSmplN)==0)
        {
            IsatCmSim = 1.0 /3.0 * (IaXfmrMagSim + IbXfmrMagSim + IcXfmrMagSim);
        	IaXfmrMagSim = IaXfmrMagSim - IsatCmSim;
        	IbXfmrMagSim = IbXfmrMagSim - IsatCmSim;
        	IcXfmrMagSim = IcXfmrMagSim - IsatCmSim;
        }
        /////////////////////////////////////////////////////
        
        LIaXfmrSecSim = IaXfmrPri2SecSim + IaXfmrMagSim + IaXfmrLossSim;
        LIbXfmrSecSim = IbXfmrPri2SecSim + IbXfmrMagSim + IbXfmrLossSim;
        LIcXfmrSecSim = IcXfmrPri2SecSim + IcXfmrMagSim + IcXfmrLossSim;
        
        XfmrLeakFlxASim = 0.0;
        XfmrLeakFlxBSim = 0.0;
        XfmrLeakFlxCSim = 0.0;
        
    }
	
       XfmrFlxAPuSim = fabs(XfmrFlxASim / PARM(XfmrFlxRat));
       XfmrFlxBPuSim = fabs(XfmrFlxBSim / PARM(XfmrFlxRat));
       XfmrFlxCPuSim = fabs(XfmrFlxCSim / PARM(XfmrFlxRat));

}

