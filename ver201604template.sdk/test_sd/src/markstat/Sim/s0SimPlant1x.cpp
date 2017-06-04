///////////////////////////////////////////////////////////////////////////////
//
// TITLE        SimPlant1x
//
// DESCRIPTION
//      This file contains the methods for the class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      s0SimPlant1x.h, s0SimPlant1x.xls, s0SimPlant1x.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
// product
#include "Product.h"
#include "s0SimPlant1x.h"
#include "s0SimConvBrg.h"
#include "s0XfmrSim.h"
#include "s0RCSnubSim.h"
#include "s0StartUpSim.h"
#include "s0ShortSim.h"
#include "s0SeriesCmpSim.h"


// Constants
//----------


// Externals
//----------
extern unsigned GEvtReqADLSim;

extern cSimBrg   L_BrgSim;
extern cSimBrg   R_BrgSim;
extern cXfmrSim   L_XfmrSim;
extern cRCSnubSim LnRCSnubSim;
extern cRCSnubSim RSRCSnubSim;
extern cRCSnubSim LSRCSnubSim;
extern cStartUpSim LineStartUpSim;
extern cShortSim RemoteShortSim;
extern cShortSim LocalShortSim;
extern unsigned AngAlign;
extern unsigned RunReq;
extern unsigned L_IsimSmplCntr;

extern cRCSnubSim  SCRCSnubSim; 
extern cSeriesCmpSim  SeriesCmpSimPha;
extern cSeriesCmpSim  SeriesCmpSimPhb;
extern cSeriesCmpSim  SeriesCmpSimPhc;
// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Diagnostics
//------------


// Parameters
//-----------


// Published Variables
//--------------------


// Unpublished Variables
//----------------------


// Data Passing
//-------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------

// Constants
//----------
#define SAT_IMAG_TO_LM 1
#define SAT_FLUX_TO_LM 2




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
cSimPlant1x::cSimPlant1x
(
        InTyp    InList,
        InsTyp   InsList
)
        : In(InList), Ins(InsList)
{
   DcPwrSim      = 0.0;
   OUT(L_VdcSim) = 1;
   L_IxBrgSim    = 0.0;
   L_IyBrgSim    = 0.0;
   OUT(L_VabBrgSim) = 0.0;
   OUT(L_VbcBrgSim) = 0.0;
   OUT(L_VanBrgSim) = 0.0;
   OUT(L_VbnBrgSim) = 0.0;
   OUT(L_VcnBrgSim) = 0.0;
   L_FlxXSim     = 0.0;
   L_FlxYSim     = 0.0;

   L_IxCalLSim   = 0.0;
   L_IyCalLSim   = 0.0;

   R_IxBrgSim    = 0.0;
   R_IxSim          = 0.0;
   R_IxSimZ1        = 0.0;
   R_IyBrgSim    = 0.0;
   R_IySim          = 0.0;
   R_IySimZ1        = 0.0;
   OUT(R_VabBrgSim) = 0.0;
   OUT(R_VbcBrgSim) = 0.0;
   R_VxBrgSim    = 0.0;
   R_VyBrgSim    = 0.0;
   R_TrqMtrSim   = 0.0;
   R_FlxXSim     = 0.0;
   R_FlxYSim     = 0.0;

   S_VabSim         = 0.0;
   S_VbcSim         = 0.0;
   S_FlxMSimFil = 0.0;

   L_IocPhsASim = false;  
   L_IocPhsBSim = false;  
   L_IocPhsCSim = false;  
   R_IocPhsASim = false;  
   R_IocPhsBSim = false;  
   R_IocPhsCSim = false;  
   
   S_FlxXSimZ1  =  0.0;
   S_FlxYSimZ1  =  0.0;

   GridFlxASim = 0.0;
   GridFlxBSim = 0.0;
   GridFlxCSim = 0.0;
   GridFlxASptLSim = 0.0;
   GridFlxBSptLSim = 0.0;
   GridFlxCSptLSim = 0.0;
   GridFlxASptRSim = 0.0;
   GridFlxBSptRSim = 0.0;
   GridFlxCSptRSim = 0.0;

   
   L_BigRchg = 0;
  // RDirectShort = 0;


    //  LDirectShort = 0;
 

   IaRemoteShort = 0.0;
   IbRemoteShort = 0.0;
   IcRemoteShort = 0.0;

      IaLocalShort = 0.0;
   IbLocalShort = 0.0;
   IcLocalShort = 0.0;

   HVRTEnb = false;
   RShortEnb = false;
   LShortEnb = false;
   StartEnb = false;
   AsymmEnb = false;



   L_WSlwSignSim    = 1.0F;    // un-initialized in the MACC, which equates to 1.0F
                               // L_WSim will not slew if L_WSlwSignSim initialized to 0.
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
cSimPlant1x::~cSimPlant1x ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by the class.
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      Status  SUCCESS/FAIL
//
///////////////////////////////////////////////////////////////////////////////
int cSimPlant1x::RunRules (void)
{
    int Status = SUCCESS;
    float   lm;
    float   l_1;
    float   l_2;
    float   k;
    float L_LRshortZ1;
    float L_RRshortZ1;
    float ShortLvlTmp;
 //   float TmpRatioSqr;
    // System
    SET_FV(TurbineNpRatSim, INS(TurbineNpRat));
    SET_FV(SysFreqNomSim, INS(SysFreqNom));
    SET_FV(SysVoltNomSim, INS(SysVoltNom));

    // Grid
    SET_FV(GridScrSim,      60.0F);
    SET_FV(ShrtMVASim,      PARM(TurbNpRatLcl) / 1000.0F  * PARM(GridScrSim));
    SET_FV(GridXORSim,      INS(GridXOR));
    
    SET_FV(StUpXORSim,      5.0F);
 /*   if(RunReq&&(PARM(GridScrSim) < PARM(StUpXORSim)))
        StartEnb = 1;
    else
        StartEnb = 0;
    */
    if ( !INS(L_RevSeqReq))
    {
        SET_FV(L_WGridSim, 2*PI*INS(SysFreqNom));
    }
    else
    {
        SET_FV(L_WGridSim, -2*PI*INS(SysFreqNom));
    }
    SET_FV(L_WSlwMaxSim,    628.0);//100hz
    
    L_PllWNomSim    =       PARM(L_WGridSim);
    SET_FV(L_LgridSim,       NoNANLim(( PARM(SysVoltNomSim) * PARM(SysVoltNomSim) ) / (PARM(ShrtMVASim)*1.0e6)/(2*PI*PARM(SysFreqNomSim)),1.0e6F, -1.0e6F));
    SET_FV(L_RgridSim,      PARM(L_LgridSim) * 2*PI*INS(SysFreqNom) / PARM(GridXORSim));

    L_LgridRcpSim = NoNANLim(1.0 / PARM(L_LgridSim),1.0e8F, -1.0e8F);
    L_LgridSptLRcpSim = NoNANLim(1.0 / (PARM(L_LgridSim)*PARM(GridXPctSim)),1.0e8F, -1.0e8F);
    L_LgridSptRRcpSim = NoNANLim(1.0 / (PARM(L_LgridSim) * (1-PARM(GridXPctSim))),1.0e8F, -1.0e8F);
    
    ScrGridSim      =       NoNANLim(PARM(ShrtMVASim)/(PARM(TurbineNpRatSim) * 0.001F),1.0e6F, -1.0e6F);  // @ unit PF, may need to adjust for a 0.9PF

    L_LRshortZ1 = PARM(GridXPctSim) * PARM(L_LgridSim);
    L_RRshortZ1 = PARM(GridXPctSim) * PARM(L_RgridSim);

    if (fabs(PARM(LvrtRLvlSim) )< 0.005)
    {
        SET_FV(RDirectShort,  1);
        ShortLvlTmp = 0.005;
    }
    else
    {
        SET_FV(RDirectShort,  0);
        
        ShortLvlTmp = fabs(PARM(LvrtRLvlSim));
    }

        SET_FV(LRshortSim,  NoNANLim(ShortLvlTmp / (1-ShortLvlTmp) * L_LRshortZ1,1.0e6F, -1.0e6F));
    SET_FV(RRshortSim,  NoNANLim(ShortLvlTmp / (1-ShortLvlTmp) * L_RRshortZ1,1.0e6F, -1.0e6F));
    SET_FV(LRshortllSim, 2.0 * PARM(LRshortSim));
    SET_FV(RRshortllSim, 2.0 * PARM(RRshortSim));

    if (fabs(PARM(LvrtLLvlSim) )< 0.005)
    {
        
        SET_FV(LDirectShort,  1);
        ShortLvlTmp = 0.005;
    }
    else
    {
        
        SET_FV(LDirectShort,  0);
        ShortLvlTmp = fabs(PARM(LvrtLLvlSim));
    }



    SET_FV(LLshortSim, NoNANLim(ShortLvlTmp / (1-ShortLvlTmp) * PARM(L_LgridSim),1.0e6F, -1.0e6F));
    SET_FV(RLshortSim, NoNANLim(ShortLvlTmp / (1-ShortLvlTmp) * PARM(L_RgridSim),1.0e6F, -1.0e6F));
    SET_FV(LLshortllSim, 2.0 * PARM(LLshortSim));
    SET_FV(RLshortllSim, 2.0 * PARM(RLshortSim));



    L_LRshortSim = PARM(LRshortSim);
    L_RRshortSim = PARM(RRshortSim);
    L_LRshortllSim = PARM(LRshortllSim);
    L_RRshortllSim = PARM(RRshortllSim);
    L_LLshortSim = PARM(LLshortSim);
    L_RLshortSim = PARM(RLshortSim);
    L_LLshortllSim = PARM(LLshortllSim);
    L_RLshortllSim = PARM(RLshortllSim);
    L_RDirectShort = PARM(RDirectShort);
    L_LDirectShort = PARM(LDirectShort);
    
    


 

    // Generator
    SET_FV(S_RRatioSim,     INS(S_RRatio));
    SET_FV(R_RPMNomSim,     INS(R_RPMNom));
    SET_FV(R_PolePairsSim,  INS(R_PolePairs));
    SET_FV(S_LsSim,         INS(S_Ls));
    SET_FV(S_LrSim,         INS(S_Lr));
    SET_FV(S_R1Sim,         INS(S_R1));
    SET_FV(S_R2Sim,         INS(S_R2));
    SET_FV(S_LmNomSim,      INS(S_LmNom));

    // rotor flux damping for simulator
    SET_FV(FlxDmpWdSim , 0.0);
    SET_FV(FlxDmpKdSim , 0.0);

    // stator flux damping for simulator
    SET_FV(S_FlxDmpWdSim , 25.0);
    SET_FV(S_FlxDmpKdSim , 0.25F * (55.0F * PI2) );

    S_RRatioRcp = NoNANLim(1.0F/PARM(S_RRatioSim), 1.0e6F, -1.0e6F);
    // flux-to-current scaling constants for simulator
    S_LmSim = PARM(S_LmNomSim);
    lm      = S_LmSim;
    l_1     = lm + PARM(S_LsSim);
    l_2     = lm + PARM(S_LrSim);
    k       = NoNANLim(1.0F/(l_1*l_2-lm*lm), 1.0e8F, -1.0e8F );
    R_K11   = l_2*k;
    R_K22   = l_1*k * PARM(S_RRatioSim) * PARM(S_RRatioSim);
    R_K12   =-lm *k * PARM(S_RRatioSim);

    // torque scaling constant for simulator
    R_Kt0   = NoNANLim(-1.5  * PARM(S_RRatioSim) * PARM(R_PolePairsSim) * (lm/l_2), 1.0e6F, -1.0e6F);

    // flux-to-flux scaling constants for simulator
    K_RS    = NoNANLim((lm/l_2) * PARM(S_RRatioSim), 1.0e6F, -1.0e6F);
    K_SR    = NoNANLim((lm/l_1) * S_RRatioRcp, 1.0e6F, -1.0e6F);

    // Converter
    SET_FV(L_VllInSim,      INS(SysVoltNom));
    SET_FV(L_RbrgSim,       INS(L_RTot));
    SET_FV(L_LbrgSim,       INS(L_LTot));
    SET_FV(L_CbrgSim,       INS(L_CTot));
    SET_FV(L_CdfSim,        INS(L_Cdf));
    SET_FV(L_RdfBldSim,     100e3F);

    SET_FV(WeakGrdThr,     5.0F);
    SET_FV(L_RdfWeakGrd,     0.054F);


    if(PARM(GridScrSim) < PARM(WeakGrdThr))
        {
        SET_FV(L_RdfSim, PARM(L_RdfWeakGrd));
        }
    else
        {
        SET_FV(L_RdfSim, INS(L_Rdf));
        }
    SET_FV(L_VdcMaxSim,  INS(L_VdcMax));
    SET_FV(L_RchgSim,       INS(L_RChg));
    //SET_FV(AuxIxRmsSim,     0.0F);
    //SET_FV(AuxIyRmsSim,     0.0F);
    SET_FV(L_RBldCapSim, INS(L_RBldCap));
    L_RBldCapRcpSim = 1.0F/PARM(L_RBldCapSim);
    L_CdfRcpSim = 1.0F/PARM(L_CdfSim);
    L_RdfBldRcpSim  = 1.0F/PARM(L_RdfBldSim);
    L_VlnInSim = PARM(L_VllInSim) * SQRT2 * SQRT3_RCP;
    AuxIxSim   = SQRT2*PARM(AuxIxRmsSim);
    AuxIySim   = SQRT2*PARM(AuxIyRmsSim);
    L_RdfRcpSim   = NoNANLim(1.0F/ PARM(L_RdfSim), 1.0e6F, -1.0e6F);
    if ( PARM(L_CbrgSim) == 0.0F)
    {
        CapDtSim = 0.0;
    }
    else
    {
        CapDtSim = INS(L_DelTm1)/PARM(L_CbrgSim);
    }
    DisChargWDtSim = NoNANLim(INS(L_DelTm1) * (1.0F /  PARM(L_CbrgSim)) * L_RBldCapRcpSim, 1.0e6F, -1.0e6F);
    L_LbrgSimRcp        = NoNANLim(1.0F/PARM(L_LbrgSim), 1.0e6F, -1.0e6F);


    SET_FV(R_FlxResSim, 0.0);           // Volt-sec

	// SC+LVRT Seq	
    SET_FV(GEvtSCRdyTm, 5.0);
    SET_FV(GEvtSCRcvTm, 5.0);
    // LVT

    // rules for the LVRT simulator
    // Symmetric fault
    if ( (PARM(GEvtTypeSim) == GRID_SYMMETRIC) || (PARM(GEvtTypeSim) == GRID_OPEN) )
    {
        L_VxnGridSFSim = PARM(L_VMagSymSclSim)/PARM(L_VMagSSSclSim);
        SET_FV(L_VanGridSclSim, L_VxnGridSFSim);
        SET_FV(L_VbnGridSclSim, L_VxnGridSFSim);
        SET_FV(L_VcnGridSclSim, L_VxnGridSFSim);

    }
    // Asymmetric fault
    else
    {
        SET_FV(L_VanGridSclSim, 1.0F);
        SET_FV(L_VbnGridSclSim, PARM(L_VMagAsySclSim));
        SET_FV(L_VcnGridSclSim, PARM(L_VMagAsySclSim));
    }

    // Imblance scale factor
    SET_FV(L_VoltImbSclSim, 0.0F);
    L_VlnSSInSim = PARM(L_VMagSSSclSim) * L_VlnInSim;

    if (PARM(L_IsimExeN) >= 10)
    {
        SET_FV(DfDynVltSim, true);  // this method requires short time step but allows lower sc MVA
    }
    else
    {
        SET_FV(DfDynVltSim, false);
    }

    // charge path resistance
    SET_FV(L_RchgEqv2Sim, PARM(L_RbrgSim) + PARM(L_LbrgSim)*PARM(L_WGridSim));
    L_RchgEqv2SimRcp = NoNANLim(1.0F/PARM(L_RchgEqv2Sim), 1.0e6F, 0.0F);
    SET_FV(L_RchgEqv1Sim, PARM(L_RchgEqv2Sim) + PARM(L_RchgSim));
    L_RchgEqv1SimRcp = NoNANLim(1.0F/PARM(L_RchgEqv1Sim), 1.0e6F, 0.0F);

    SET_FV(L_IocThrsSim,    INS(L_IocThrs) );  // IOC Threshold

 //RC for open circuit
    SET_FV(L_RrcSim, 0.05F);
    SET_FV(L_CrcSim, 20e-6F);
    L_CrcRcpSim = 1.0F/PARM(L_CrcSim);
//DFIG saturation curve
    SET_FV(S_GenSatEnbSim, PARM(S_GenSatCurvVld));
    SET_FV(S_SatCurvTypeSim, SAT_IMAG_TO_LM);
    SET_FV(R_GenSatInFilWSim, 2000.0F);

    SET_FV(S_SatCurvFlxMaxSim,PARM(S_SatCurvFlxMax));            
    SET_FV(S_SatCurvFlxMinSim,PARM(S_SatCurvFlxMin));            
    SET_FV(S_SatCurvXmMaxSim, PARM(S_SatCurvXmMax ));            
    SET_FV(S_SatCurvXmMinSim, PARM(S_SatCurvXmMin ));            
    SET_FV(S_SatCurvK0Sim,    PARM(S_SatCurvK0    ));            
    SET_FV(S_SatCurvK1Sim,    PARM(S_SatCurvK1    ));    
    SET_FV(S_SatCurvK2Sim,    PARM(S_SatCurvK2    ));            
    SET_FV(S_SatCurvK3Sim,    PARM(S_SatCurvK3    ));
 
    S_IndM = lm*(PI2 * PARM(S_Hz));
  
    return (Status);
}


// Motor related models
//-----------------------------------------------------------------------------

// Moved from a0DemRot.cpp
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the simulated demodulated rotor voltages.
//      Called by RotorProcessMtrCtrlT1()--: should be removed and conditioned
//      in RotorDemodVoltage() in a0DemRot.cpp;
// EXECUTION LEVEL
//      Rotor Task 1A
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::RotorDemodVoltageSim()
{
 
    cVectorf::DemodLL(R_VabBrgSim, R_VbcBrgSim, &R_SinCosSim,&R_VxBrgSim, &R_VyBrgSim);
    
    if ( IN(S_SimK1ClsdT1) && IN(Q1Status) )
    {
        S_VxSim = L_VxXfmrSecSim;
        S_VySim = L_VyXfmrSecSim;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//     This function generates the demodulated stator voltages in isim
//
//
// EXECUTION LEVEL
//      Rotor Task 1A
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::RotorVoltageFbkSim()
{

    OUT(R_VabBrgSim) = R_BrgSim.VabBrgSim;
    OUT(R_VbcBrgSim) = R_BrgSim.VbcBrgSim;

    if ( !IN(S_SimK1ClsdT1) || !IN(Q1Status) )
    {
        if ( IN(R_InnrRegEnbT1) )
        {
            S_VxSim = S_VxMdlSim;
            S_VySim = S_VyMdlSim;
        }
        else
        {
            S_VxSim = 0.0;
            S_VySim = 0.0;
        }
    }

    cVectorf::RotateLN(S_VxSim, S_VySim, &S_SinCosSim, &OUT(S_VaSim), &OUT(S_VbSim));

    S_VabSim = S_VaSim - S_VbSim;
    S_VbcSim = S_VaSim + S_VbSim + S_VbSim;

}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//     This function generates the simulated rotor & stator current feedbacks
//
//
// EXECUTION LEVEL
//      Rotor Task 1A
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::RotorCurrentFbkSim()
{

     R_IxBrgSim    = 0.5*(R_IxSim + R_IxSimZ1);  // Should have this to model latency of cfb vco's
     R_IyBrgSim    = 0.5*(R_IySim + R_IySimZ1);  // however there is already extra latency in this
                                                   // model due to stator voltages being calculated
                                                   // from stator currents and vice-versa.
    //R_IxBrgSim    = R_IxSim;                       // Therefore using R_Ix(y)Sim directly.
    //R_IyBrgSim    = R_IySim;

    // delay model for rotor current feedback VCO's
    R_IxSimZ1 = R_IxSim;
    R_IySimZ1 = R_IySim;


    cVectorf::RotateLN(R_IxBrgSim,R_IyBrgSim,&R_SinCosSim,&OUT(R_IaBrgSim),&OUT(R_IbBrgSim));
    OUT(R_IcBrgSim) = -(R_IaBrgSim + R_IbBrgSim);
    
    
        
    S_IxSimAvg  =  0.5 * (S_IxSimZ1 + S_IxSim);
    S_IySimAvg  =  0.5 * (S_IySimZ1 + S_IySim);
    
    S_IxSimZ1  =  S_IxSim;
    S_IySimZ1  =  S_IySim;
    

    cVectorf::RotateLN(S_IxSimAvg,S_IySimAvg,&S_SinCosSim,&S_IaSim,&S_IbSim);
    S_IcSim = -(S_IaSim + S_IbSim);

    OUT(L_IaFuseSim) = S_IaSim - L_IaXfmrSecSim;
    OUT(L_IbFuseSim) = S_IbSim - L_IbXfmrSecSim;
    OUT(L_IcFuseSim) = S_IcSim - L_IcXfmrSecSim;
    
    if(IN(FuseOpen)&0x01) //phase A open
        OUT(L_IaFuseSim) = 0;
    if(IN(FuseOpen)&0x02) //phase B open
        OUT(L_IbFuseSim) = 0;
    if(IN(FuseOpen)&0x04) //phase C open
        OUT(L_IcFuseSim) = 0;

}


//////////////////////////////////////////////////////////////////////////////
//
/// DESCRIPTION
//      This function performs motor magnetic inductor saturation performance.
// EXECUTION LEVEL
//      Rotor Task 1A
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::IndMotSatSim(void)
{    
    float   lm = 0.0;    
    float   l_1 = 0.0;    
    float   l_2 = 0.0;    
    float   k = 0.0;    
    float   Id0 = 0.0;  
    // flux-to-current scaling constants for simulator    

     
    if (PARM(S_GenSatEnbSim) == true)     
    {                  
        if (PARM(S_SatCurvTypeSim) == SAT_IMAG_TO_LM)        
        {            
            Id0 = S_IMagSim;  //peak value                                               
            if(Id0 > PARM(S_SatCurvFlxMaxSim))            
            {                       
                lm = PARM(S_SatCurvXmMinSim);            
            }            
            else if(Id0 < PARM(S_SatCurvFlxMinSim))            
            {                
                lm = PARM(S_SatCurvXmMaxSim);            
            }            
            else            
            {                
                lm = PARM(S_SatCurvK0Sim) + Id0 * (PARM(S_SatCurvK1Sim) + Id0 * ( PARM(S_SatCurvK2Sim) + Id0 * PARM(S_SatCurvK3Sim)));            
            }        
        }       
        else if (PARM(S_SatCurvTypeSim) == SAT_FLUX_TO_LM)        
        {            
            Id0 = S_FlxMSimFil;  //RMS value                                               
            if(Id0 > PARM(S_SatCurvFlxMaxSim))            
            {                       
                lm = PARM(S_SatCurvXmMinSim);            
            }            
            else if(Id0 < PARM(S_SatCurvFlxMinSim))            
            {                
                lm = PARM(S_SatCurvXmMaxSim);            
            }            
            else            
            {                
                lm = PARM(S_SatCurvK0Sim) + Id0 * (PARM(S_SatCurvK1Sim) + Id0 * ( PARM(S_SatCurvK2Sim) + Id0 * PARM(S_SatCurvK3Sim)));            
            }        
            lm = lm /(PI2 * PARM(S_Hz));
        }
    }   
    else    
    {            
        lm = PARM(S_LmNomSim);        
    }         

    S_IndM  = lm * (PI2 * PARM(S_Hz));
    l_1     = lm + PARM(S_LsSim);    
    l_2     = lm + PARM(S_LrSim);    
    k       = NoNANLim(1.0F / (l_1 * l_2 - lm * lm), 1.0e8F, -1.0e8F );    
    R_K11   = l_2 * k;    
    R_K22   = l_1 * k * PARM(S_RRatioSim) * PARM(S_RRatioSim);    
    R_K12   =-lm * k * PARM(S_RRatioSim);        
    // torque scaling constant for simulator    
    R_Kt0   = NoNANLim(-1.5  * PARM(S_RRatioSim) * PARM(R_PolePairsSim) * (lm/l_2), 1.0e6F, -1.0e6F);        
    // flux-to-flux scaling constants for simulator    
    K_RS    = NoNANLim((lm/l_2) * PARM(S_RRatioSim), 1.0e6F, -1.0e6F);    
    K_SR    = NoNANLim((lm/l_1) * S_RRatioRcp, 1.0e6F, -1.0e6F);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      DFIG electrical model
//
// EXECUTION LEVEL
//      Rotor Task 1A
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::RotorMtrElecSim(void)
{
    float   Temp1, Temp2;

    Temp2   = S_RRatioRcp * S_RRatioRcp;
    S_R1Sim = INS(S_R1) * INS(Imped_Scl);
    R_R2Sim = INS(S_R2) * INS(Imped_Scl) * Temp2;
    
    S_FlxXSimZ1 = S_FlxXSim;    
    S_FlxYSimZ1 = S_FlxYSim;
    

    
    
    if ( IN(S_SimK1ClsdT1) && IN(Q1Status) )
    {
 
            // Simulate rotor flux
            Temp1 = R_VxBrgSim - R_R2Sim*R_IxSim + R_WElecSim*R_FlxYSim;
            R_FlxXSim += INS(R_DelTm1) * Temp1;
            Temp2 = R_VyBrgSim - R_R2Sim*R_IySim - R_WElecSim*R_FlxXSim;
            R_FlxYSim += INS(R_DelTm1) * Temp2;

            // Simulate stator flux
            Temp1 = S_VxSim + S_R1Sim*S_IxSim + S_WElecSim*S_FlxYSim;
            S_FlxXSim += INS(R_DelTm1) * Temp1;
            Temp2 = S_VySim + S_R1Sim*S_IySim - S_WElecSim*S_FlxXSim;
            S_FlxYSim += INS(R_DelTm1) * Temp2;

            // Transformation from fluxes to currents
            S_IxSim = -(R_K11 * S_FlxXSim + R_K12 * R_FlxXSim);
            S_IySim = -(R_K11 * S_FlxYSim + R_K12 * R_FlxYSim);
            R_IxSim =   R_K12 * S_FlxXSim + R_K22 * R_FlxXSim;
            R_IySim =   R_K12 * S_FlxYSim + R_K22 * R_FlxYSim;
  

        /* ------ Stator flux damping ----- */
        S_FlxYSimLpf += PARM(S_FlxDmpWdSim) * INS(R_DelTm1) * (S_FlxYSim - S_FlxYSimLpf);
        S_FlxYSimDmp  = PARM(S_FlxDmpKdSim) * (S_FlxYSim - S_FlxYSimLpf);
    }
    else
    {

        if ( IN(R_InnrRegEnbT1) )
            // Sync Breaker is open, exciting rotor, compute stator voltages
        {
            // simulate rotor flux
            Temp1 = R_VxBrgSim - R_R2Sim*R_IxSim + R_WElecSim*R_FlxYSim;
            R_FlxXSim += INS(R_DelTm1) * Temp1;

            Temp2 = R_VyBrgSim - R_R2Sim*R_IySim - R_WElecSim*R_FlxXSim;
            R_FlxYSim += INS(R_DelTm1) * (Temp2 - R_FlxYSimDmp);

            /* ------ Flux Damping (needed if simulating very hi Lm) ----- */
            R_FlxYSimLpf += PARM(FlxDmpWdSim) * INS(R_DelTm1) * (R_FlxYSim - R_FlxYSimLpf);
            R_FlxYSimDmp  = PARM(FlxDmpKdSim) * (R_FlxYSim - R_FlxYSimLpf);

            // calculate stator flux from rotor flux
            S_FlxXSim  =  R_FlxXSim * K_RS;
            S_FlxYSim  =  R_FlxYSim * K_RS;

            // transformation from fluxes to currents
            S_IxSim = 0.0; // R_K11 * S_FlxXSim + R_K12 * R_FlxXSim;
            S_IySim = 0.0; // R_K11 * S_FlxYSim + R_K12 * R_FlxYSim;
            R_IxSim = R_K12 * S_FlxXSim + R_K22 * R_FlxXSim;
            R_IySim = R_K12 * S_FlxYSim + R_K22 * R_FlxYSim;

            // calculate stator volts from stator flux
            S_VxMdlSim = -S_WElecSim * S_FlxYSim + Temp1 * K_RS;
            S_VyMdlSim =  S_WElecSim * S_FlxXSim + Temp2 * K_RS;

            S_VabSim = S_VaSim - S_VbSim;
            S_VbcSim = S_VaSim + S_VbSim + S_VbSim;
        }
        else
            // Sync Breaker is open, rotor not excited
        {
            // Note may want to change this to a decay model
            R_FlxXSim   = 0.0;
            R_FlxYSim   = 0.0;
            R_FlxYSimLpf     = 0.0;
            R_FlxYSimDmp     = 0.0;
            S_FlxXSim   = 0.0;
            S_FlxYSim   = 0.0;
            S_IxSim     = 0.0;
            S_IySim     = 0.0;
            R_IxSim          = 0.0;
            R_IySim          = 0.0;
            S_VabSim    = 0.0;
            S_VbcSim    = 0.0;
            OUT(S_VaSim)     = 0.0;
            OUT(S_VbSim)     = 0.0;
            OUT(S_VbSim)     = 0.0;
            S_VxSim     = 0.0;
            S_VySim     = 0.0;
        }
    }

/////////////////////////
// saturation curve/////////
    S_FlxMXSim = S_IxSim * PARM(S_LsSim) + S_FlxXSim;
    S_FlxMYSim = S_IySim * PARM(S_LsSim) + S_FlxYSim;
    S_FlxMSim = sqrt(S_FlxMXSim * S_FlxMXSim + S_FlxMYSim * S_FlxMYSim) * SQRT2_RCP;

    S_FlxMSimFil += PARM(R_GenSatInFilWSim) * INS(L_DelTm1) * (S_FlxMSim - S_FlxMSimFil);
    
    S_IMagXSim = R_IxSim * S_RRatioRcp - S_IxSim;
    S_IMagYSim = R_IySim * S_RRatioRcp - S_IySim;    
    S_IMagSim = sqrt(S_IMagXSim * S_IMagXSim + S_IMagYSim * S_IMagYSim);
    
    
    if(L_IsimSmplCntr == 0)
    {
        IndMotSatSim();
        
    } 
  
    
////////////////////
    Temp1  = ( S_IySim * R_FlxXSim - S_IxSim * R_FlxYSim );
    R_TrqMtrSim  = R_Kt0 * Temp1;        // This variable is set here but not used. Same in MACC ESS
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the mechanical motor model.
//
// EXECUTION LEVEL
//      Rotor Task 2  -Task 1A
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::RotorMtrModelMechSim()
{
    OUT(R_SpdSim) = INS(R_SpdRefSim);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the mechanical motor model.
//
// EXECUTION LEVEL
//      Rotor Task 2  -Task 1A
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::PhaseANormal()
{
            GridFlxASim = GridFlxASim + INS(L_DelTm1) *
                                   (SCRCSnubSim.VaRCSim - L_VanGridSim - PARM(L_RgridSim) * IaGridSim);         
            GridFlxASptLSim = GridFlxASim * PARM(GridXPctSim);
            GridFlxASptRSim = GridFlxASim * (1 - PARM(GridXPctSim));
            IaGridSim = IaGridSptLSim = IaGridSptRSim = GridFlxASim *L_LgridRcpSim + ((SCRCSnubSim.VaRCSim - L_VanGridSim - PARM(L_RgridSim) * IaGridSim) * RhfGridRcp);
          //  IaGridLSSim = IaGridSim + LineStartUpSim.IaStSim;
            
            IaGridSCSim = IaGridSim + LineStartUpSim.IaStSim;
          
            SeriesCmpSimPha.StateUpdate();
            if (SeriesCmpSimPha.SCompState == 0)
            {
                
                VaRCSCSim = L_VaXfmrMgSim;
                SCPhaseState = 6;//110
                SCRCSnubSim.RCNoWorkABC();
                SeriesCmpSimPha.SCompRun();
            }                
            else
            {
                IaRCSCSim = SeriesCmpSimPha.Iout - IaGridSCSim;
                SCPhaseState = 1;
                SCRCSnubSim.RCWorkABC();
                SeriesCmpSimPha.SCompRun();
            }
            IaRCLSSim = L_XfmrSim.IaXfmrPriSim - SeriesCmpSimPha.Iout;
            IaGridLSSim = SeriesCmpSimPha.Iout;


            
     //       IaRCLSSim = L_XfmrSim.IaXfmrPriSim - IaGridLSSim;

            RSPhaseState = 6;
            RemoteShortSim.ShortNotRun();
            if(PARM(L_ShuntKVA)>10.0)
            {
                         CrcLSSim = PARM(L_CShuntSim);
                        RrcLSSim = PARM(L_RShuntSim);
            }
            else
                {
                         CrcLSSim = PARM(L_CrcLineSim);
                        RrcLSSim = PARM(L_RrcLineSim);
            }
            LSRCSnubSim.RunRules();        
            
            LSPhaseState = 6;
            LocalShortSim.ShortNotRun();
            LSPhaseState = 1;
            LSRCSnubSim.RCWorkABC();

            VaRCLSSim = L_VaXfmrMgSim = LSRCSnubSim.VaRCSim;
            VaRCRSSim = L_VanGridSim + (SCRCSnubSim.VaRCSim - L_VanGridSim) * PARM(GridXPctSim);       
            
           
            RSPhaseState = 6;//110
            RSRCSnubSim.RCNoWorkABC();
}

void cSimPlant1x::PhaseBNormal()
{
            GridFlxBSim = GridFlxBSim + INS(L_DelTm1) *
                                   (SCRCSnubSim.VbRCSim - L_VbnGridSim - PARM(L_RgridSim) * IbGridSim);         
        GridFlxBSptLSim = GridFlxBSim * PARM(GridXPctSim);
        GridFlxBSptRSim = GridFlxBSim * (1 - PARM(GridXPctSim));
        IbGridSim = IbGridSptLSim = IbGridSptRSim = GridFlxBSim *L_LgridRcpSim + ((SCRCSnubSim.VbRCSim - L_VbnGridSim - PARM(L_RgridSim) * IbGridSim) * RhfGridRcp);
   //     IbGridLSSim = IbGridSim + LineStartUpSim.IbStSim;

        IbGridSCSim = IbGridSim + LineStartUpSim.IbStSim;
          
            SeriesCmpSimPhb.StateUpdate();
            if (SeriesCmpSimPhb.SCompState == 0)
            {
                
                VbRCSCSim = L_VbXfmrMgSim;
                SCPhaseState = 5;//110
                SCRCSnubSim.RCNoWorkABC();
                SeriesCmpSimPhb.SCompRun();
            }                
            else
            {
                IbRCSCSim = SeriesCmpSimPhb.Iout - IbGridSCSim;
                SCPhaseState = 2;
                SCRCSnubSim.RCWorkABC();
                SeriesCmpSimPhb.SCompRun();
            }
            IbRCLSSim = L_XfmrSim.IbXfmrPriSim - SeriesCmpSimPhb.Iout;
            IbGridLSSim = SeriesCmpSimPhb.Iout;

        
      //  IbRCLSSim = L_XfmrSim.IbXfmrPriSim - IbGridLSSim;

            RSPhaseState = 5;
            RemoteShortSim.ShortNotRun();

            if(PARM(L_ShuntKVA)>10.0)
            {
                         CrcLSSim = PARM(L_CShuntSim);
                        RrcLSSim = PARM(L_RShuntSim);
            }
            else
                {
                         CrcLSSim = PARM(L_CrcLineSim);
                        RrcLSSim = PARM(L_RrcLineSim);
            }
            LSRCSnubSim.RunRules();        
            
            LSPhaseState = 5;
            LocalShortSim.ShortNotRun();
            LSPhaseState = 2;
            LSRCSnubSim.RCWorkABC();

            VbRCLSSim = L_VbXfmrMgSim = LSRCSnubSim.VbRCSim;
        VbRCRSSim = L_VbnGridSim + (SCRCSnubSim.VbRCSim - L_VbnGridSim) * PARM(GridXPctSim);
           
            RSPhaseState = 5;//110
            RSRCSnubSim.RCNoWorkABC();
}

void cSimPlant1x::PhaseCNormal()
{
             GridFlxCSim = GridFlxCSim + INS(L_DelTm1) *
                                   (SCRCSnubSim.VcRCSim - L_VcnGridSim - PARM(L_RgridSim) * IcGridSim);         
        GridFlxCSptLSim = GridFlxCSim * PARM(GridXPctSim);
        GridFlxCSptRSim = GridFlxCSim * (1 - PARM(GridXPctSim));
        IcGridSim = IcGridSptLSim = IcGridSptRSim = GridFlxCSim *L_LgridRcpSim + ((SCRCSnubSim.VcRCSim - L_VcnGridSim - PARM(L_RgridSim) * IcGridSim)* RhfGridRcp);
   //     IcGridLSSim = IcGridSim + LineStartUpSim.IcStSim;


            IcGridSCSim = IcGridSim + LineStartUpSim.IcStSim;
          
            SeriesCmpSimPhc.StateUpdate();
            if (SeriesCmpSimPhc.SCompState == 0)
            {
                
                VcRCSCSim = L_VcXfmrMgSim;
                SCPhaseState = 3;//110
                SCRCSnubSim.RCNoWorkABC();
                SeriesCmpSimPhc.SCompRun();
            }                
            else
            {
                IcRCSCSim = SeriesCmpSimPhc.Iout - IcGridSCSim;
                SCPhaseState = 4;
                SCRCSnubSim.RCWorkABC();
                SeriesCmpSimPhc.SCompRun();
            }
            IcRCLSSim = L_XfmrSim.IcXfmrPriSim - SeriesCmpSimPhc.Iout;
            IcGridLSSim = SeriesCmpSimPhc.Iout;
 
        
   //     IcRCLSSim = L_XfmrSim.IcXfmrPriSim - IcGridLSSim;

            RSPhaseState = 3;
            RemoteShortSim.ShortNotRun();

            if(PARM(L_ShuntKVA)>10.0)
            {
                         CrcLSSim = PARM(L_CShuntSim);
                        RrcLSSim = PARM(L_RShuntSim);
            }
            else
                {
                         CrcLSSim = PARM(L_CrcLineSim);
                        RrcLSSim = PARM(L_RrcLineSim);
            }
            LSRCSnubSim.RunRules();        
            
            LSPhaseState = 3;
            LocalShortSim.ShortNotRun();
            LSPhaseState = 4;
            LSRCSnubSim.RCWorkABC();

            VcRCLSSim = L_VcXfmrMgSim = LSRCSnubSim.VcRCSim;
        VcRCRSSim = L_VcnGridSim + (SCRCSnubSim.VcRCSim - L_VcnGridSim) * PARM(GridXPctSim);    
            
           
            RSPhaseState = 3;//110
            RSRCSnubSim.RCNoWorkABC();
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the angle calculation in isim.
//      new added function: Copied from void RotorCmdFbkAngl()in a0Angle.cpp,
//      change conresponding names
//
// EXECUTION LEVEL
//      Rotor Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::RotorCmdFbkAnglSim()
{

    OUT(R_WRtrSim)       = (R_SpdSim)*INS(R_PolePairs);
    OUT(R_TachPosnSim)   = cVectorf::Modulo2Pi(R_TachPosnSim + (INS(R_DelTm1) * R_WRtrSim) );
    R_AngMechSim = R_TachPosnSim - R_TacZeroSim;

    S_WElecSim  = L_WElecSim;
    R_WElecSim  = S_WElecSim - OUT(R_WRtrSim);

    S_AngElecSim = L_AngElecSim + IN(R_LT1TmDelta) * L_WElecSim;
    cVectorf::SineCosine(S_AngElecSim, &S_SinCosSim);

    R_AngElecSim = cVectorf::Modulo2Pi(S_AngElecSim - R_AngMechSim + PI2);
    cVectorf::SineCosine(R_AngElecSim, &R_SinCosSim);
}

// Line side models
//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function simulates the voltage feedback for the line when
//      simulator mode is active.
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::LineVoltSimulator(void)
{
    
    float       L_VxFuseArc =0.0;
    float       L_VyFuseArc = 0.0;
    float       L_IaFilSimTmp = 0.0;            // delayed simulated line converter grid monitor l-n current (AMP)
    float       L_IbFilSimTmp = 0.0;            // delayed simulated line converter grid monitor l-n current (AMP)
    float       L_IcFilSimTmp = 0.0;            // delayed simulated line converter grid monitor l-n current (AMP)
  

    float        L_VaStaSim=0.0;
    float         L_VbStaSim=0.0;
    float L_VcStaSim=0.0;
    float L_IaBrgSimTmp = 0.0;
    float L_IbBrgSimTmp = 0.0;
    float L_IcBrgSimTmp = 0.0;

    float tempA = 0.0;
    float tempB = 0.0;
    float tempC = 0.0;
    unsigned PhaseStateZ = 0;
    unsigned PhaseStateZ1 = 0;


    if(PARM(SCManual) == 1)
    {
        SCEnb = PARM(SCCmd);
    }
    RhfGridRcp = NoNANLim(1.0/(PARM(L_RhfXfmrPuSim) * L_WElecSim * PARM(L_LgridSim)),1.0e8F, -1.0e8F);
    RhfGridSptLRcp = NoNANLim(1.0/(PARM(L_RhfXfmrPuSim) * L_WElecSim * PARM(L_LgridSim) * PARM(GridXPctSim)),1.0e8F, -1.0e8F);
    RhfGridSptRRcp = NoNANLim(1.0/(PARM(L_RhfXfmrPuSim) * L_WElecSim * PARM(L_LgridSim) * (1-PARM(GridXPctSim))),1.0e8F, -1.0e8F);
    
    if(IN(FuseArc))
    {
        
        cVectorf::DemodLN3(IN(FuseVaArc),IN(FuseVbArc),IN(FuseVcArc),&L_SinCosSim,&L_VxFuseArc,&L_VyFuseArc);
        
        L_VxXfmrSecSim = L_VxXfmrSecSim + L_VxFuseArc;
        L_VyXfmrSecSim = L_VyXfmrSecSim + L_VyFuseArc;     
    }    

    // net current (@ 690 volt)
   
    tempA = L_XfmrSim.IaXfmrPriSim;
    tempB = L_XfmrSim.IbXfmrPriSim;
    tempC = L_XfmrSim.IcXfmrPriSim;
    cVectorf::DemodLN3(tempA,tempB,tempC,&L_SinCosSim,&IxGridSim,&IyGridSim);

    if(PARM(GridModelTypeSim) == SIMPLE_GRID_MODEL)
    {
    // voltage at Xfmr magnetizing circuit (@690 volts) - (L*di/dt term ommitted for simulator stability)
        L_VxXfmrMgSim = VxGridSim + (IxGridSim * PARM(L_RgridSim)) - (PARM(L_LgridSim) * IyGridSim * L_WElecSim);
                    // + (PARM(LxfmrPriSim) * (IxGridSim - IxGridSimZ1) * INS(L_DelTm1Rcp));

        L_VyXfmrMgSim = VyGridSim + (IyGridSim * PARM(L_RgridSim)) + (PARM(L_LgridSim) * IxGridSim * L_WElecSim);
                    // + (PARM(LxfmrPriSim) * (IyGridSim - IyGridSimZ1) * INS(L_DelTm1Rcp));


    //consider canceling Rprimary from Rtotal
    cVectorf::RotateLN(L_VxXfmrMgSim,L_VyXfmrMgSim,&L_SinCosSim,&L_VaXfmrMgSim,&L_VbXfmrMgSim);
                L_VcXfmrMgSim = -(L_VaXfmrMgSim + L_VbXfmrMgSim);

    }
    else
    {
 //start-up circuit
    if(RunReq&&(PARM(GridScrSim) < PARM(StUpXORSim)))
        StartEnb = 1;
    else
        StartEnb = 0;
    LineStartUpSim.StartUp();

    
  //  RSPhaseState = RSPhaseState |RShortState;


    RemoteShortSim.StateUpdate();
    RSPhaseState = RemoteShortSim.ShortState;

    LocalShortSim.StateUpdate();
    LSPhaseState = LocalShortSim.ShortState;


   if(RSPhaseState)
   {
        L_LgridSptLSim = PARM(L_LgridSim) * PARM(GridXPctSim);
        L_LgridSptRSim = PARM(L_LgridSim) - L_LgridSptLSim;
   }
   else
   {
        L_LgridSptLSim = PARM(L_LgridSim);
        L_LgridSptRSim = 0.0;
   }
    
    if((RSPhaseState == 0) && (LSPhaseState == 0)) //no short
    {
        
        PhaseANormal();
        PhaseBNormal();
        PhaseCNormal();

        RSPhaseState = 0;
        LSPhaseState = 0;
        
    }
    else if (RSPhaseState)   // remote short (split impedance)
    {
        PhaseStateZ = RSPhaseState;
        PhaseStateZ1 = LSPhaseState;
        RemoteShortSim.ShortRun();
        VaRCRSSim= RemoteShortSim.VaShortSim;
        VbRCRSSim= RemoteShortSim.VbShortSim;
        VcRCRSSim= RemoteShortSim.VcShortSim;
        
        
        

        
        if(RSPhaseState&0x01)
        { 
            GridFlxASptLSim = GridFlxASptLSim + INS(L_DelTm1) *
                                           (VaRCRSSim - L_VanGridSim - PARM(L_RgridSim) * PARM(GridXPctSim) * IaGridSptLSim);         
                    IaGridSptLSim = GridFlxASptLSim *L_LgridSptLRcpSim+ ((VaRCRSSim - L_VanGridSim - PARM(L_RgridSim) * PARM(GridXPctSim) * IaGridSptLSim) *RhfGridSptLRcp);
                    if(PARM(RDirectShort) == 0)
                    {
                        IaRemoteShort = RemoteShortSim.IaShort;
                        IaGridRSSim = IaGridSptLSim + IaRemoteShort;
                        IaRCRSSim = IaGridSptRSim - IaGridRSSim;
                        RSPhaseState = 1;
                        RSRCSnubSim.RCWorkABC();
                        VaRCRSSim = RSRCSnubSim.VaRCSim;
                    }      
                     GridFlxASptRSim = GridFlxASptRSim + INS(L_DelTm1) *
                                           (SCRCSnubSim.VaRCSim - VaRCRSSim - PARM(L_RgridSim) * ( 1 - PARM(GridXPctSim)) * IaGridSptRSim);         
                    IaGridSptRSim = GridFlxASptRSim *L_LgridSptRRcpSim + ( (SCRCSnubSim.VaRCSim - VaRCRSSim - PARM(L_RgridSim) * ( 1 - PARM(GridXPctSim)) * IaGridSptRSim)*RhfGridSptRRcp);
               //     IaGridLSSim = IaGridSptRSim + LineStartUpSim.IaStSim;               
                  
                    
                    IaGridSCSim = IaGridSptRSim + LineStartUpSim.IaStSim;
                  
                    SeriesCmpSimPha.StateUpdate();
                    if (SeriesCmpSimPha.SCompState == 0)
                    {
                        
                        VaRCSCSim = L_VaXfmrMgSim;
                        SCPhaseState = 6;//110
                        SCRCSnubSim.RCNoWorkABC();
                        SeriesCmpSimPha.SCompRun();
                    }                
                    else
                    {
                        IaRCSCSim = SeriesCmpSimPha.Iout - IaGridSCSim;
                        SCPhaseState = 1;
                        SCRCSnubSim.RCWorkABC();
                        SeriesCmpSimPha.SCompRun();
                    }
                    IaRCLSSim = L_XfmrSim.IaXfmrPriSim - SeriesCmpSimPha.Iout;
                    IaGridLSSim = SeriesCmpSimPha.Iout;

     
          //          IaRCLSSim = L_XfmrSim.IaXfmrPriSim - IaGridLSSim;
                    if(PARM(L_ShuntKVA)>10.0)
                    {
                                 CrcLSSim = PARM(L_CShuntSim);
                                RrcLSSim = PARM(L_RShuntSim);
                    }
                    else
                        {
                                 CrcLSSim = PARM(L_CrcLineSim);
                                RrcLSSim = PARM(L_RrcLineSim);
                    }
                    LSRCSnubSim.RunRules();
            
            LSPhaseState = 1;
            LSRCSnubSim.RCWorkABC();
            VaRCLSSim = L_VaXfmrMgSim = LSRCSnubSim.VaRCSim;
            GridFlxASim = GridFlxASptLSim + GridFlxASptRSim;
            if(PARM(RDirectShort))
            {
                RSPhaseState = 6;
                RSRCSnubSim.RCNoWorkABC();
                IaRemoteShort = IaGridSptRSim - IaGridSptLSim;
                    
            }

        }
        else
            PhaseANormal();
        
        RSPhaseState = PhaseStateZ;
        LSPhaseState = PhaseStateZ1;
        if(RSPhaseState&0x02)
        {
           GridFlxBSptLSim = GridFlxBSptLSim + INS(L_DelTm1) *                                       
                                    (VbRCRSSim - L_VbnGridSim - PARM(L_RgridSim) * PARM(GridXPctSim) * IbGridSptLSim);                     
                   IbGridSptLSim = GridFlxBSptLSim *L_LgridSptLRcpSim + ((VbRCRSSim - L_VbnGridSim - PARM(L_RgridSim) * PARM(GridXPctSim) * IbGridSptLSim)*RhfGridSptLRcp);            
                   if(PARM(RDirectShort) == 0)            
                    {                
                        IbRemoteShort = RemoteShortSim.IbShort;                
                        IbGridRSSim = IbGridSptLSim + IbRemoteShort;                 
                        IbRCRSSim = IbGridSptRSim - IbGridRSSim;                
                        RSPhaseState = 2;                
                        RSRCSnubSim.RCWorkABC();                
                        VbRCRSSim = RSRCSnubSim.VbRCSim;            
                        }            
                   GridFlxBSptRSim = GridFlxBSptRSim + INS(L_DelTm1) *                                      
                                        (SCRCSnubSim.VbRCSim - VbRCRSSim - PARM(L_RgridSim) * ( 1 - PARM(GridXPctSim)) * IbGridSptRSim);                     
                   IbGridSptRSim = GridFlxBSptRSim *L_LgridSptRRcpSim + ((SCRCSnubSim.VbRCSim - VbRCRSSim - PARM(L_RgridSim) * ( 1 - PARM(GridXPctSim)) * IbGridSptRSim)*RhfGridSptRRcp);           
               //    IbGridLSSim = IbGridSptRSim + LineStartUpSim.IbStSim;      

            
                    IbGridSCSim = IbGridSptRSim + LineStartUpSim.IbStSim;
                      
                        SeriesCmpSimPhb.StateUpdate();
                        if (SeriesCmpSimPhb.SCompState == 0)
                        {
                            
                            VbRCSCSim = L_VbXfmrMgSim;
                            SCPhaseState = 5;//110
                            SCRCSnubSim.RCNoWorkABC();
                            SeriesCmpSimPhb.SCompRun();
                        }                
                        else
                        {
                            IbRCSCSim = SeriesCmpSimPhb.Iout - IbGridSCSim;
                            SCPhaseState = 2;
                            SCRCSnubSim.RCWorkABC();
                            SeriesCmpSimPhb.SCompRun();
                        }
                        IbRCLSSim = L_XfmrSim.IbXfmrPriSim - SeriesCmpSimPhb.Iout;
                        IbGridLSSim = SeriesCmpSimPhb.Iout;

                    
                                    
             //      IbRCLSSim = L_XfmrSim.IbXfmrPriSim - IbGridLSSim;        

                    if(PARM(L_ShuntKVA)>10.0)
                    {
                                 CrcLSSim = PARM(L_CShuntSim);
                                RrcLSSim = PARM(L_RShuntSim);
                    }
                    else
                        {
                                 CrcLSSim = PARM(L_CrcLineSim);
                                RrcLSSim = PARM(L_RrcLineSim);
                    }
                    LSRCSnubSim.RunRules();
                   LSPhaseState = 2;           
                   LSRCSnubSim.RCWorkABC();        
                   L_VbXfmrMgSim = LSRCSnubSim.VbRCSim;      
                   GridFlxBSim = GridFlxBSptLSim + GridFlxBSptRSim;         
                   if(PARM(RDirectShort))          
                    {                
                        RSPhaseState = 5;               
                        RSRCSnubSim.RCNoWorkABC();           
                        IbRemoteShort = IbGridSptRSim - IbGridSptLSim;          
                     }
            
        }
        else
            PhaseBNormal();
        
        RSPhaseState = PhaseStateZ;
        LSPhaseState = PhaseStateZ1;
        if(RSPhaseState&0x04)
        {
            GridFlxCSptLSim = GridFlxCSptLSim + INS(L_DelTm1) *                                       
                            (VcRCRSSim - L_VcnGridSim - PARM(L_RgridSim) * PARM(GridXPctSim) * IcGridSptLSim);                     
                    IcGridSptLSim = GridFlxCSptLSim *L_LgridSptLRcpSim + ((VcRCRSSim - L_VcnGridSim - PARM(L_RgridSim) * PARM(GridXPctSim) * IcGridSptLSim)*RhfGridSptLRcp);   
                    if(PARM(RDirectShort) == 0)         
                        {            
                        IcRemoteShort = RemoteShortSim.IcShort;        
                        IcGridRSSim = IcGridSptLSim + IcRemoteShort;                
                        IcRCRSSim = IcGridSptRSim - IcGridRSSim;            
                        RSPhaseState = 4;            
                        RSRCSnubSim.RCWorkABC();                
                        VcRCRSSim = RSRCSnubSim.VcRCSim;         
                        }          
                    GridFlxCSptRSim = GridFlxCSptRSim + INS(L_DelTm1) *                          
                        (SCRCSnubSim.VcRCSim - VcRCRSSim - PARM(L_RgridSim) * ( 1 - PARM(GridXPctSim)) * IcGridSptRSim);          
                    IcGridSptRSim = GridFlxCSptRSim *L_LgridSptRRcpSim + ((SCRCSnubSim.VcRCSim - VcRCRSSim - PARM(L_RgridSim) * ( 1 - PARM(GridXPctSim)) * IcGridSptRSim)*RhfGridSptRRcp);         
            //        IcGridLSSim = IcGridSptRSim + LineStartUpSim.IcStSim;               


                        IcGridSCSim = IcGridSptRSim + LineStartUpSim.IcStSim;
                      
                        SeriesCmpSimPhc.StateUpdate();
                        if (SeriesCmpSimPhc.SCompState == 0)
                        {
                            
                            VcRCSCSim = L_VcXfmrMgSim;
                            SCPhaseState = 3;//110
                            SCRCSnubSim.RCNoWorkABC();
                            SeriesCmpSimPhc.SCompRun();
                        }                
                        else
                        {
                            IcRCSCSim = SeriesCmpSimPhc.Iout - IcGridSCSim;
                            SCPhaseState = 4;
                            SCRCSnubSim.RCWorkABC();
                            SeriesCmpSimPhc.SCompRun();
                        }
                        IcRCLSSim = L_XfmrSim.IcXfmrPriSim - SeriesCmpSimPhc.Iout;
                        IcGridLSSim = SeriesCmpSimPhc.Iout;     
                        
            //        IcRCLSSim = L_XfmrSim.IcXfmrPriSim - IcGridLSSim;     
                    if(PARM(L_ShuntKVA)>10.0)
                    {
                                 CrcLSSim = PARM(L_CShuntSim);
                                RrcLSSim = PARM(L_RShuntSim);
                    }
                    else
                        {
                                 CrcLSSim = PARM(L_CrcLineSim);
                                RrcLSSim = PARM(L_RrcLineSim);
                    }
                    LSRCSnubSim.RunRules();
                    LSPhaseState = 4;          
                    LSRCSnubSim.RCWorkABC();    
                    L_VcXfmrMgSim = LSRCSnubSim.VcRCSim;       
                    GridFlxCSim = GridFlxCSptLSim + GridFlxCSptRSim;          
                    if(PARM(RDirectShort))            
                        {               
                        RSPhaseState = 3;         
                        RSRCSnubSim.RCNoWorkABC();         
                        IcRemoteShort = IcGridSptRSim - IcGridSptLSim;        
                        }
          
        }
        else
            PhaseCNormal();         
        
        LSPhaseState = PhaseStateZ1;
        LocalShortSim.ShortNotRun();
        RSPhaseState = PhaseStateZ;
    }
    else if(LSPhaseState)  // local short
    {
        PhaseStateZ = RSPhaseState;
        PhaseStateZ1 = LSPhaseState;
        LocalShortSim.ShortRun();
        L_VaXfmrMgSim= LocalShortSim.VaShortSim;
        L_VbXfmrMgSim= LocalShortSim.VbShortSim;
        L_VcXfmrMgSim= LocalShortSim.VcShortSim;

        
        

       
        
        if(LSPhaseState&0x01)
        {   
            GridFlxASim = GridFlxASim + INS(L_DelTm1) *
                                                   (SCRCSnubSim.VaRCSim - L_VanGridSim - PARM(L_RgridSim) * IaGridSim);         
                    IaGridSim = IaGridSptLSim = IaGridSptRSim = GridFlxASim *L_LgridRcpSim + ((SCRCSnubSim.VaRCSim - L_VanGridSim - PARM(L_RgridSim) * IaGridSim) * RhfGridRcp);


      
                        
                        IaGridSCSim = IaGridSim + LineStartUpSim.IaStSim;
                      
                        SeriesCmpSimPha.StateUpdate();
                        if (SeriesCmpSimPha.SCompState == 0)
                        {
                            
                            VaRCSCSim = L_VaXfmrMgSim;
                            SCPhaseState = 6;//110
                            SCRCSnubSim.RCNoWorkABC();
                            SeriesCmpSimPha.SCompRun();
                        }                
                        else
                        {
                            IaRCSCSim = SeriesCmpSimPha.Iout - IaGridSCSim;
                            SCPhaseState = 1;
                            SCRCSnubSim.RCWorkABC();
                            SeriesCmpSimPha.SCompRun();
                        }
          //              IaRCLSSim = L_XfmrSim.IaXfmrPriSim - SeriesCmpSimPha.Iout;
           //             IaGridLSSim = SeriesCmpSimPha.Iout;
                        

                    if(PARM(LDirectShort) == 0)
                    {
                        IaLocalShort = LocalShortSim.IaShort;   
                        IaGridLSSim = SeriesCmpSimPha.Iout + IaLocalShort;
                        IaRCLSSim = L_XfmrSim.IaXfmrPriSim - IaGridLSSim;

                        if(PARM(L_ShuntKVA)>10.0)
                        {
                                     CrcLSSim = PARM(L_CShuntSim);
                                    RrcLSSim = PARM(L_RShuntSim);
                        }
                        else
                            {
                                     CrcLSSim = PARM(L_CrcLineSim);
                                    RrcLSSim = PARM(L_RrcLineSim);
                        }
                        LSRCSnubSim.RunRules();
                            LSPhaseState = 1;
                        LSRCSnubSim.RCWorkABC();              
                        
                    }
                    else
                    {
                        if(PARM(L_ShuntKVA)>10.0)
                        {
                                     CrcLSSim = PARM(L_CShuntSim);
                                    RrcLSSim = PARM(L_RShuntSim);
                        }
                        else
                            {
                                     CrcLSSim = PARM(L_CrcLineSim);
                                    RrcLSSim = PARM(L_RrcLineSim);
                        }
                        LSRCSnubSim.RunRules();
                        LSPhaseState = 6;
                        LSRCSnubSim.RCNoWorkABC();
                         IaRemoteShort = 0.0;//L_XfmrSim.IaXfmrPriSim - IaGridSim - LineStartUpSim.IaStSim;
                    }
                    VaRCLSSim = L_VaXfmrMgSim = LSRCSnubSim.VaRCSim;
                    VaRCRSSim = L_VanGridSim + (SCRCSnubSim.VaRCSim - L_VanGridSim) * PARM(GridXPctSim); 
                    
        }
                else
            PhaseANormal();     
        RSPhaseState = PhaseStateZ;
        LSPhaseState = PhaseStateZ1;
        if(LSPhaseState&0x02)
        {   

            GridFlxBSim = GridFlxBSim + INS(L_DelTm1) *
                                               (SCRCSnubSim.VbRCSim - L_VbnGridSim - PARM(L_RgridSim) * IbGridSim);         
                    IbGridSim = IbGridSptLSim = IbGridSptRSim = GridFlxBSim *L_LgridRcpSim + ((SCRCSnubSim.VbRCSim - L_VbnGridSim - PARM(L_RgridSim) * IbGridSim) * RhfGridRcp);


                    IbGridSCSim = IbGridSim + LineStartUpSim.IbStSim;
                      
                        SeriesCmpSimPhb.StateUpdate();
                        if (SeriesCmpSimPhb.SCompState == 0)
                        {
                            
                            VbRCSCSim = L_VbXfmrMgSim;
                            SCPhaseState = 5;//110
                            SCRCSnubSim.RCNoWorkABC();
                            SeriesCmpSimPhb.SCompRun();
                        }                
                        else
                        {
                            IbRCSCSim = SeriesCmpSimPhb.Iout - IbGridSCSim;
                            SCPhaseState = 2;
                            SCRCSnubSim.RCWorkABC();
                            SeriesCmpSimPhb.SCompRun();
                        }


                    
                    if(PARM(LDirectShort) == 0)
                    {
                        IbLocalShort = LocalShortSim.IbShort;   
                        IbGridLSSim = SeriesCmpSimPhb.Iout  + IbLocalShort;
                        IbRCLSSim = L_XfmrSim.IbXfmrPriSim - IbGridLSSim;

                    if(PARM(L_ShuntKVA)>10.0)
                    {
                                 CrcLSSim = PARM(L_CShuntSim);
                                RrcLSSim = PARM(L_RShuntSim);
                    }
                    else
                        {
                                 CrcLSSim = PARM(L_CrcLineSim);
                                RrcLSSim = PARM(L_RrcLineSim);
                    }
                    LSRCSnubSim.RunRules();
                        LSPhaseState = 2;
                        LSRCSnubSim.RCWorkABC();              
                        
                    }
                    else
                    {
                    if(PARM(L_ShuntKVA)>10.0)
                    {
                                 CrcLSSim = PARM(L_CShuntSim);
                                RrcLSSim = PARM(L_RShuntSim);
                    }
                    else
                        {
                                 CrcLSSim = PARM(L_CrcLineSim);
                                RrcLSSim = PARM(L_RrcLineSim);
                    }
                    LSRCSnubSim.RunRules();
                        LSPhaseState = 5;
                        LSRCSnubSim.RCNoWorkABC();
                         IbRemoteShort = 0.0;//L_XfmrSim.IbXfmrPriSim - IbGridSim - LineStartUpSim.IbStSim;
                    }
                    VbRCLSSim = L_VbXfmrMgSim = LSRCSnubSim.VbRCSim;
                    VbRCRSSim = L_VbnGridSim + (SCRCSnubSim.VbRCSim - L_VbnGridSim) * PARM(GridXPctSim);        
                   
   
           

            
        }
                else
            PhaseBNormal();     
        RSPhaseState = PhaseStateZ;
        LSPhaseState = PhaseStateZ1;
        if(LSPhaseState&0x04)
        {
             GridFlxCSim = GridFlxCSim + INS(L_DelTm1) *
                                               (SCRCSnubSim.VcRCSim - L_VcnGridSim - PARM(L_RgridSim) * IcGridSim);         
                    IcGridSim = IcGridSptLSim = IcGridSptRSim = GridFlxCSim *L_LgridRcpSim + ((SCRCSnubSim.VcRCSim - L_VcnGridSim - PARM(L_RgridSim) * IcGridSim) * RhfGridRcp);

                    IcGridSCSim = IcGridSim + LineStartUpSim.IcStSim;
                      
                        SeriesCmpSimPhc.StateUpdate();
                        if (SeriesCmpSimPhc.SCompState == 0)
                        {
                            
                            VcRCSCSim = L_VcXfmrMgSim;
                            SCPhaseState = 3;//110
                            SCRCSnubSim.RCNoWorkABC();
                            SeriesCmpSimPhc.SCompRun();
                        }                
                        else
                        {
                            IcRCSCSim = SeriesCmpSimPhc.Iout - IcGridSCSim;
                            SCPhaseState = 4;
                            SCRCSnubSim.RCWorkABC();
                            SeriesCmpSimPhc.SCompRun();
                        }


                    
                    if(PARM(LDirectShort) == 0)
                    {
                        IcLocalShort = LocalShortSim.IcShort;   
                        IcGridLSSim = SeriesCmpSimPhc.Iout + IcLocalShort;
                        IcRCLSSim = L_XfmrSim.IcXfmrPriSim - IcGridLSSim;
                        
                    if(PARM(L_ShuntKVA)>10.0)
                    {
                                 CrcLSSim = PARM(L_CShuntSim);
                                RrcLSSim = PARM(L_RShuntSim);
                    }
                    else
                        {
                                 CrcLSSim = PARM(L_CrcLineSim);
                                RrcLSSim = PARM(L_RrcLineSim);
                    }
                    LSRCSnubSim.RunRules();
                        LSPhaseState = 4;
                        LSRCSnubSim.RCWorkABC();              
                        
                    }
                    else
                    {
                        if(PARM(L_ShuntKVA)>10.0)
                        {
                                     CrcLSSim = PARM(L_CShuntSim);
                                    RrcLSSim = PARM(L_RShuntSim);
                        }
                        else
                            {
                                     CrcLSSim = PARM(L_CrcLineSim);
                                    RrcLSSim = PARM(L_RrcLineSim);
                        }
                        LSRCSnubSim.RunRules();
                        LSPhaseState = 3;
                        LSRCSnubSim.RCNoWorkABC();
                         IcRemoteShort = 0.0;//L_XfmrSim.IcXfmrPriSim - IcGridSim - LineStartUpSim.IcStSim;
                    }
                    VcRCLSSim = L_VcXfmrMgSim = LSRCSnubSim.VcRCSim;
                    VcRCRSSim = L_VcnGridSim + (SCRCSnubSim.VcRCSim - L_VcnGridSim) * PARM(GridXPctSim);
        
        }
                else
            PhaseCNormal();     


             

        

   
       
        RSPhaseState = 0;//110
        RSRCSnubSim.RCNoWorkABC();

        RSPhaseState = PhaseStateZ;
        RemoteShortSim.ShortNotRun();
        LSPhaseState = PhaseStateZ1;
    }

    
        }
  

   
    // voltage at Xfmr magnetizing circuit (@690 volts) - (L*di/dt term ommitted for simulator stability)
  //  L_VxXfmrMgSim = VxGridSim + (IxGridSim * L_RgridSim) - (L_LgridSim * IyGridSim * L_WElecSim);
                    // + (PARM(LxfmrPriSim) * (IxGridSim - IxGridSimZ1) * INS(L_DelTm1Rcp));

  //  L_VyXfmrMgSim = VyGridSim + (IyGridSim * L_RgridSim) + (L_LgridSim * IxGridSim * L_WElecSim);
                    // + (PARM(LxfmrPriSim) * (IyGridSim - IyGridSimZ1) * INS(L_DelTm1Rcp));


    //consider canceling Rprimary from Rtotal
 //   cVectorf::RotateLN(L_VxXfmrMgSim,L_VyXfmrMgSim,&L_SinCosSim,&L_VaXfmrMgSim,&L_VbXfmrMgSim);
  //              L_VcXfmrMgSim = -(L_VaXfmrMgSim + L_VbXfmrMgSim);

    cVectorf::RotateLN(L_VxXfmrSecSim,L_VyXfmrSecSim,&L_SinCosSim,&L_VaXfmrSecSim,&L_VbXfmrSecSim);
                L_VcXfmrSecSim = -(L_VaXfmrSecSim + L_VbXfmrSecSim);

    VaSCRCSim = SCRCSnubSim.VaRCSim;
    VbSCRCSim = SCRCSnubSim.VbRCSim;
    VcSCRCSim = SCRCSnubSim.VcRCSim;
    
    VaSCPha = SeriesCmpSimPha.Vsccap;
    VbSCPha = SeriesCmpSimPhb.Vsccap;
    VcSCPha = SeriesCmpSimPhc.Vsccap;
    
    IaSCPha = SeriesCmpSimPha.IscompCSim;
    IbSCPha = SeriesCmpSimPhb.IscompCSim;
    IcSCPha = SeriesCmpSimPhc.IscompCSim;
    
    
    if (PARM(DfDynVltSim))
    {
        // dynamic model of distortion filter voltage (simplified model consisting of one RC circuit)
        if ( IN(Q1Status) && IN(KdfClsCmd) )
        {

            L_XfmrState = 0;
            L_XfmrSim.XfmrModel();
            

            tempA = L_XfmrSim.LIaXfmrSecSim;
            tempB = L_XfmrSim.LIbXfmrSecSim;
            tempC = L_XfmrSim.LIcXfmrSecSim;
            cVectorf::DemodLN3(tempA,tempB,tempC,&L_SinCosSim,&L_IxXfmrSecSim,&L_IyXfmrSecSim);

            if(IN(FuseOpen))
            {
////////////////Filter voltage calculation/////////////////////////////

                cVectorf::RotateLN(L_IxXfmrSecSim,L_IyXfmrSecSim,&L_SinCosSim,&L_IaXfmrSecSim,&L_IbXfmrSecSim);
                L_IcXfmrSecSim = -(L_IaXfmrSecSim + L_IbXfmrSecSim);

                L_IaXfmrSecSimZ1 = L_IaXfmrSecSim;
                L_IbXfmrSecSimZ1 = L_IbXfmrSecSim;
                L_IcXfmrSecSimZ1 = L_IcXfmrSecSim;

                 if(IN(FuseOpen)&0x01) //phase A open
                {
                    L_IaFilSimTmp = 0.0;
                }
                else
                {
                    L_IaFilSimTmp = S_IaSim - L_IaXfmrSecSim;
        
                }
                if(IN(FuseOpen)&0x02) //phase B open
                {
                    L_IbFilSimTmp = 0.0;    
        
                }
                else
                {
                    L_IbFilSimTmp = S_IbSim - L_IbXfmrSecSim;
                }
                if(IN(FuseOpen)&0x04) //phase C open
                {
                    L_IcFilSimTmp = 0.0;      
        
                }
                else
                {
                    L_IcFilSimTmp = S_IcSim - L_IcXfmrSecSim;
                }

                cVectorf::RotateLN(L_IxBrgSim,L_IyBrgSim,&L_SinCosSim,&L_IaBrgSimTmp,&L_IbBrgSimTmp);
                L_IcBrgSimTmp = -(L_IaBrgSimTmp + L_IbBrgSimTmp);

                L_IaFilSimTmp = L_IaBrgSimTmp + L_IaFilSimTmp;
                L_IbFilSimTmp = L_IbBrgSimTmp + L_IbFilSimTmp;
                L_IcFilSimTmp = L_IcBrgSimTmp + L_IcFilSimTmp;
     

                L_VaDFCapSim = L_VaDFCapSim + L_IaFilSimTmp * INS(L_DelTm1) * L_CdfRcpSim;
                L_VbDFCapSim = L_VbDFCapSim + L_IbFilSimTmp * INS(L_DelTm1) * L_CdfRcpSim;
                L_VcDFCapSim = L_VcDFCapSim + L_IcFilSimTmp * INS(L_DelTm1) * L_CdfRcpSim;

                L_VaFilCapSim = L_VaDFCapSim + L_IaFilSimTmp * PARM(L_RdfSim);
                L_VbFilCapSim = L_VbDFCapSim + L_IbFilSimTmp * PARM(L_RdfSim);
                L_VcFilCapSim = L_VcDFCapSim + L_IcFilSimTmp * PARM(L_RdfSim);
    
                cVectorf::DemodLN3(L_VaFilCapSim,L_VbFilCapSim,L_VcFilCapSim,&L_SinCosSim,&L_VxFilSim,&L_VyFilSim);
                cVectorf::DemodLN3(L_VaDFCapSim,L_VbDFCapSim,L_VcDFCapSim,&L_SinCosSim,&L_VxDFCapSim,&L_VyDFCapSim);
/////////////end Filter voltage calculation////////////////////////////
    
/////////////Stator voltage calculation//////////////////////////////
                L_VaStaSim = L_VaFilCapSim;         
                L_VbStaSim = L_VbFilCapSim;
                L_VcStaSim = L_VcFilCapSim;
     
                if(IN(FuseOpen)&0x01) //phase A open
                {
                    L_IaRCSim = S_IaSim - L_IaXfmrSecSim;
                    L_VaRCCapSim = L_VaRCCapSim + L_IaRCSim * INS(L_DelTm1) * L_CrcRcpSim;
                    L_VaRCSim = L_VaRCCapSim + L_IaRCSim * PARM(L_RrcSim);
                    L_VaStaSim = L_VaRCSim;      
                }
                else
                {
                    L_VaRCCapSim = L_VaFilCapSim;
         
                }
    
                if(IN(FuseOpen)&0x02) //phase B open
                {
                    L_IbRCSim = S_IbSim -L_IbXfmrSecSim;
                    L_VbRCCapSim = L_VbRCCapSim + L_IbRCSim * INS(L_DelTm1) * L_CrcRcpSim;
                    L_VbRCSim = L_VbRCCapSim + L_IbRCSim * PARM(L_RrcSim);
                    L_VbStaSim = L_VbRCSim;             
                }
                else
                {
                     L_VbRCCapSim = L_VbFilCapSim;
                }
    
                if(IN(FuseOpen)&0x04) //phase C open
                {
                    L_IcRCSim = S_IcSim - L_IcXfmrSecSim;
                    L_VcRCCapSim = L_VcRCCapSim + L_IcRCSim * INS(L_DelTm1) * L_CrcRcpSim;
                    L_VcRCSim = L_VcRCCapSim + L_IcRCSim * PARM(L_RrcSim);
                    L_VcStaSim = L_VcRCSim;
                }
                else
                {
                    L_VcRCCapSim = L_VcFilCapSim;
                }
    
                cVectorf::DemodLN3(L_VaStaSim,L_VbStaSim,L_VcStaSim,&L_SinCosSim,&L_VxXfmrSecSim,&L_VyXfmrSecSim);
                L_VxLnSim      = L_VxFilSim;
                L_VyLnSim      = L_VyFilSim;
////////////end stator voltage calculation//////////////////////////////
    //////////////////////////////
            }
            else
            {
                // Current into distortion filter
                L_IxDFSim = L_IxBrgSim + S_IxSimAvg - L_IxXfmrSecSim - AuxIxSim;
                L_IyDFSim = L_IyBrgSim + S_IySimAvg - L_IyXfmrSecSim - AuxIySim;

                // Voltage at distortion filter caps
                L_QxDFSim  = L_QxDFSim + INS(L_DelTm1) * (L_IxDFSim + (L_QyDFSim * L_WElecSim));
                L_VxDFCapSim = ( L_QxDFSim * L_CdfRcpSim );

                L_QyDFSim  = L_QyDFSim + INS(L_DelTm1) * (L_IyDFSim - (L_QxDFSim * L_WElecSim));
                L_VyDFCapSim = ( L_QyDFSim * L_CdfRcpSim );

                // Voltage at 690v secondary (cap voltage plus IR drop)
                L_VxXfmrSecSim = L_VxDFCapSim + L_IxDFSim * PARM(L_RdfSim);
                L_VyXfmrSecSim = L_VyDFCapSim + L_IyDFSim * PARM(L_RdfSim);


                cVectorf::RotateLN(L_VxDFCapSim,L_VyDFCapSim,&L_SinCosSim,&L_VaDFCapSim,&L_VbDFCapSim);
                L_VcDFCapSim = -(L_VaDFCapSim+L_VbDFCapSim);
    
    
                L_VxFilSim = L_VxXfmrSecSim;
                L_VyFilSim = L_VyXfmrSecSim;
        
                if(IN(FuseArc))
                 {
         
                    L_VxXfmrSecSim = L_VxXfmrSecSim + L_VxFuseArc;
                    L_VyXfmrSecSim = L_VyXfmrSecSim + L_VyFuseArc;     
                }    

       //         L_VxLnSim      = L_VxXfmrSecSim;
       //         L_VyLnSim      = L_VyXfmrSecSim;
                L_VxLnSim      = L_VxFilSim;
                L_VyLnSim      = L_VyFilSim;

                L_IxXfmrSecSimZ1 = L_IxXfmrSecSim;
                L_IyXfmrSecSimZ1 = L_IyXfmrSecSim;
                cVectorf::RotateLN(L_IxXfmrSecSimZ1,L_IyXfmrSecSimZ1,&L_SinCosSim,&L_IaXfmrSecSimZ1,&L_IbXfmrSecSimZ1);
                L_IcXfmrSecSimZ1 = -(L_IaXfmrSecSimZ1 + L_IbXfmrSecSimZ1);

                cVectorf::RotateLN(L_VxXfmrSecSim,L_VyXfmrSecSim,&L_SinCosSim,&L_VaXfmrSecTmpSim,&L_VbXfmrSecTmpSim);
                L_VcXfmrSecTmpSim = -(L_VaXfmrSecTmpSim + L_VbXfmrSecTmpSim);
                L_VaRCCapSim = L_VaXfmrSecTmpSim;
                L_VbRCCapSim = L_VbXfmrSecTmpSim;
                L_VcRCCapSim = L_VcXfmrSecTmpSim;
            }

            L_VxRCSimHold = L_VxFilSim;
            L_VyRCSimHold = L_VyFilSim;
            LnRCSnubSim.RCNoWork();
        }
        else if(IN(Q1Status) && !IN(KdfClsCmd))
        {

 /////////////grid model//////////////////////
            L_XfmrState = 0;
            L_XfmrSim.XfmrModel();    

            tempA = L_XfmrSim.LIaXfmrSecSim;
            tempB = L_XfmrSim.LIbXfmrSecSim;
            tempC = L_XfmrSim.LIcXfmrSecSim;
            cVectorf::DemodLN3(tempA,tempB,tempC,&L_SinCosSim,&L_IxXfmrSecSim,&L_IyXfmrSecSim);
           
//////////////RC snubber for filter point///////////////
            L_IxRCSim = L_IxBrgSim + S_IxSimAvg - L_IxXfmrSecSim - AuxIxSim;
            L_IyRCSim = L_IyBrgSim + S_IySimAvg - L_IyXfmrSecSim - AuxIySim;

            LnRCSnubSim.RCWork();

                // Voltage at 690v secondary (cap voltage plus IR drop)
            L_VxXfmrSecSim = LnRCSnubSim.VxRCSim;
            L_VyXfmrSecSim = LnRCSnubSim.VyRCSim;

 ////////////////////////////////////

 
            L_IxXfmrSecSimZ1 = L_IxXfmrSecSim;
            L_IyXfmrSecSimZ1 = L_IyXfmrSecSim;
            L_IxDFSim        = 0.0F;
            L_IyDFSim        = 0.0F;

            L_QxDFSim  = L_QxDFSim - INS(L_DelTm1) * (L_VxDFCapSim * L_RdfBldRcpSim);
            L_QyDFSim  = L_QyDFSim - INS(L_DelTm1) * (L_VyDFCapSim * L_RdfBldRcpSim);
            L_VxDFCapSim = ( L_QxDFSim * L_CdfRcpSim );
            L_VyDFCapSim = ( L_QyDFSim * L_CdfRcpSim );


            L_VxFilSim = L_VxXfmrSecSim;
            L_VyFilSim = L_VyXfmrSecSim;

            if(IN(FuseArc))
            {
         
                L_VxFilSim = L_VxXfmrSecSim - L_VxFuseArc;
                L_VyFilSim = L_VyXfmrSecSim - L_VyFuseArc;     
            }    

            
            L_VxLnSim = L_VxFilSim;
            L_VyLnSim = L_VyFilSim;
        }
        else if(!IN(Q1Status) && IN(KdfClsCmd))
        {

            L_XfmrState = 1;
            L_XfmrSim.XfmrModel();    

            
        
            L_IxXfmrSecSimZ1= L_IxXfmrSecSim;
            L_IyXfmrSecSimZ1= L_IyXfmrSecSim;

            tempA = L_XfmrSim.LIaXfmrSecSim;
            tempB = L_XfmrSim.LIbXfmrSecSim;
            tempC = L_XfmrSim.LIcXfmrSecSim;
            
            cVectorf::DemodLN3(tempA,tempB,tempC,&L_SinCosSim,&L_IxXfmrSecSim,&L_IyXfmrSecSim);
            tempA = L_XfmrSim.VaXfmrSecFbkSim;
            tempB = L_XfmrSim.VbXfmrSecFbkSim;
            tempC = L_XfmrSim.VcXfmrSecFbkSim;
            
            cVectorf::DemodLN3(tempA,tempB,tempC,&L_SinCosSim,&L_VxXfmrSecSim,&L_VyXfmrSecSim);
  

            L_IxDFSim = L_IxBrgSim + S_IxSimAvg - L_IxXfmrSecSim - AuxIxSim;
            L_IyDFSim = L_IyBrgSim + S_IySimAvg - L_IyXfmrSecSim - AuxIySim;
            L_QxDFSim  = L_QxDFSim + INS(L_DelTm1) * (L_IxDFSim + (L_QyDFSim * L_WElecSim));
            L_QyDFSim  = L_QyDFSim + INS(L_DelTm1) * (L_IyDFSim - (L_QxDFSim * L_WElecSim));
            L_VxDFCapSim = ( L_QxDFSim * L_CdfRcpSim );
            L_VyDFCapSim = ( L_QyDFSim * L_CdfRcpSim );
            L_VxLnSim = L_VxDFCapSim + L_IxDFSim * PARM(L_RdfSim);
            L_VyLnSim = L_VyDFCapSim + L_IyDFSim * PARM(L_RdfSim);
            L_VxFilSim = L_VxLnSim;
            L_VyFilSim = L_VyLnSim;

            if(IN(FuseArc))
            {
         
                L_VxXfmrSecSim = L_VxFilSim + L_VxFuseArc;
                L_VyXfmrSecSim = L_VyFilSim + L_VyFuseArc;     
            }

            L_VxRCSimHold = L_VxFilSim;
            L_VyRCSimHold = L_VyFilSim;
            LnRCSnubSim.RCNoWork();
        }
        else
        {

 /////////////grid model//////////////////////
            L_XfmrState = 0;
            L_XfmrSim.XfmrModel();    

            tempA = L_XfmrSim.LIaXfmrSecSim;
            tempB = L_XfmrSim.LIbXfmrSecSim;
            tempC = L_XfmrSim.LIcXfmrSecSim;
            
             cVectorf::DemodLN3(tempA,tempB,tempC,&L_SinCosSim,&L_IxXfmrSecSim,&L_IyXfmrSecSim);
            

//////////////RC snubber for filter point///////////////
            L_IxRCSim = L_IxBrgSim  - L_IxXfmrSecSim - AuxIxSim;
            L_IyRCSim = L_IyBrgSim  - L_IyXfmrSecSim - AuxIySim;

            LnRCSnubSim.RCWork();

                // Voltage at 690v secondary (cap voltage plus IR drop)
            L_VxXfmrSecSim = LnRCSnubSim.VxRCSim;
            L_VyXfmrSecSim = LnRCSnubSim.VyRCSim;

 ////////////////////////////////////
 
          
            // Current into distortion filter
            L_IxDFSim = 0.0F;
            L_IyDFSim = 0.0F;

            // Voltage at distortion filter caps
            L_QxDFSim    = 0.0F;
            L_VxDFCapSim = 0.0F;

            L_QyDFSim    = 0.0F;
            L_VyDFCapSim = 0.0F;

           
            L_VxLnSim = L_VxXfmrSecSim + PARM(L_RchgSim) * L_IxBrgSim;     
            L_VyLnSim = L_VyXfmrSecSim + PARM(L_RchgSim) * L_IyBrgSim;



            L_IxXfmrSecSimZ1= L_IxXfmrSecSim;
            L_IyXfmrSecSimZ1= L_IyXfmrSecSim;
    
            L_VaRCCapSim = 0.0F;
            L_VbRCCapSim = 0.0F;
            L_VcRCCapSim = 0.0F;
        }
    }
    else
    {
        // static model of distortion filter (DF currents calc-ed in LineDFCurSim()

 /*       L_IxXfmrSecSimZ1 = L_IxXfmrSecSim;                         // Store the previous value of grid current
        L_IxXfmrSecSim   = L_IxBrgSim + S_IxSimAvg - L_IxDFSim - AuxIxSim;                 // Xfmr sec line converter x-current calculation

        L_XfmrSecFlxXSim = PARM(L_LxfmrSecSim) * L_IxXfmrSecSim;   // Xfmr sec line converter X flux calculation

        L_IyXfmrSecSimZ1 = L_IyXfmrSecSim;                         // Store the previous value of grid current
        L_IyXfmrSecSim   = L_IyBrgSim + S_IySimAvg - L_IyDFSim - AuxIySim;      // Xfmr sec line converter y-current calculation

        L_XfmrSecFlxYSim = PARM(L_LxfmrSecSim) * L_IyXfmrSecSim;   // Xfmr sec line converter Y flux calculation

        // X-axis Line voltage calculation
        L_VxXfmrSecSim = L_VxXfmrMgSim + (L_IxXfmrSecSim * PARM(L_RxfmrSecSim)) - (L_XfmrSecFlxYSim * L_WElecSim) +
                      (PARM(L_LxfmrSecSim) * (L_IxXfmrSecSim - L_IxXfmrSecSimZ1) * INS(L_DelTm1Rcp));

        // Y-axis Line voltage calculation
        L_VyXfmrSecSim = L_VyXfmrMgSim + (L_IyXfmrSecSim * PARM(L_RxfmrSecSim)) + (L_XfmrSecFlxXSim * L_WElecSim) +
                      (PARM(L_LxfmrSecSim) * (L_IyXfmrSecSim - L_IyXfmrSecSimZ1) * INS(L_DelTm1Rcp));
                      */
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function simulates the voltage and current for the line distortion
//      filter when simulator mode is active.
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::LineDFCurSim(void)
{
    if(!PARM(DfDynVltSim))
    {
        //Distortion filter is connected to the grid
        if ( IN(KdfClsCmd) && IN(Q1Status) )
        {
            // simplified filter cap simulation
            float tmp1f;
            tmp1f = PARM(L_CdfSim) * L_WElecSim * PARM(L_RdfSim);
            // Capacitor bank with resistance (DFC1)
            //-----------------------------------------
            // Calculate steady state voltage across capacitor
            L_VyDFCapSim = NoNANLim( (L_VyXfmrSecSim - (tmp1f*L_VxXfmrSecSim)) /
                                    (1.0F + (tmp1f*tmp1f)), 1.0e6F, -1.0e6F );
            L_VxDFCapSim = NoNANLim( (L_VyXfmrSecSim - L_VyDFCapSim) / (tmp1f), 1.0e6F, -1.0e6F );

            // Update capacitor current using the dist-filter resistor
            L_IxDFSim = (L_VxXfmrSecSim - L_VxDFCapSim) * L_RdfRcpSim;
            L_IyDFSim = (L_VyXfmrSecSim - L_VyDFCapSim) * L_RdfRcpSim;
        }
        else
        {
            L_VyDFCapSim=L_VxDFCapSim=0.0F;
            L_IxDFSim = L_IyDFSim = 0.0F;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs task 2 calculations for the line simulator
//      Function named in old isim is LineSimulatorT2()
//
// EXECUTION LEVEL
//      Line Task 2 -Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::LineSimulator()
{
    //L_LdPwrSim = READ_PARMPTR(L_LdPwrAdrSim) + IN(R_BrgTotalPwr);
    
    L_LdPwrSim = READ_PARMPTR(L_LdPwrAdrSim) + R_IdcSim * L_VdcSim;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the source plant model.
//
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
/*
void cSimPlant1x::BrgEnabSimulator()
{
    float L_IxCalLSimZ1, L_IyCalLSimZ1;

    //-------------------------------------------------------------------------
    //                         X - Channel plant simulator
    //-------------------------------------------------------------------------

    L_IxCalLSimZ1 = L_IxCalLSim;
    L_VxBrgSim       = L_VxBrgEnbSim;

    //L_VxErrSim   = L_VxBrgEnbSim - L_VxXfmrSecSim;
    L_VxErrSim   = L_VxBrgEnbSim - L_VxFilSim;
    L_VxVrSim    = L_VxErrSim - (PARM(L_RbrgSim)*L_IxCalLSim);

    L_FlxXSim    = L_FlxXSim + INS(L_DelTm1)*
                       ((L_FlxYSim*L_WElecSim) + L_VxVrSim);

    L_IxCalLSim   = (L_FlxXSim*L_LbrgSimRcp);
    L_IxBrgSim       = 0.5*(L_IxCalLSim + L_IxCalLSimZ1);

    //-------------------------------------------------------------------------
    //                         Y - Channel plant simulator
    //-------------------------------------------------------------------------

    L_IyCalLSimZ1 = L_IyCalLSim;
    L_VyBrgSim       = L_VyBrgEnbSim;

  //  L_VyErrSim   = L_VyBrgEnbSim - L_VyXfmrSecSim;
    L_VyErrSim   = L_VyBrgEnbSim - L_VyFilSim;
    L_VyVrSim    = L_VyErrSim - (PARM(L_RbrgSim)*L_IyCalLSim);

    L_FlxYSim    = L_FlxYSim + INS(L_DelTm1)*
                     ((-L_FlxXSim*L_WElecSim) + L_VyVrSim);

    L_IyCalLSim   = L_FlxYSim*L_LbrgSimRcp;
    L_IyBrgSim       = 0.5*(L_IyCalLSim + L_IyCalLSimZ1);

    //-------------------------------------------------------------------------
    //                      Dc Voltage generation
    //-------------------------------------------------------------------------

    DcPwrSim     = (-1.5*(L_IxCalLSim*L_VxBrgEnbSim + L_IyCalLSim*L_VyBrgEnbSim))-
                   IN(L_BrgLossPwr);

    L_VdcSimRcp =  NoNANLim( 1.0/L_VdcSim, 1.0e6F, -1.0e6F );

    L_IdcSim     = DcPwrSim*L_VdcSimRcp;

    L_ILoadSim   = (L_LdPwrSim + IN(PwrBatChpr))* L_VdcSimRcp + IN(IdbTot);

    OUT(L_VdcSim)     =  L_VdcSim + CapDtSim*(L_IdcSim - L_ILoadSim);

    if ( L_VdcSim >= PARM(L_VdcMaxSim))
    {
         OUT(L_VdcSim) =  PARM(L_VdcMaxSim);
    }
    else if (L_VdcSim <= 0.0F)
    {
         OUT(L_VdcSim) = 0.0F;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the model of a disabled power bridge.
//      Runs in sim mode when L_InnrRegEnb is FALSE
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::BrgDisabSimulator()
{
     float   L_VPosSim, L_VMidSim, L_VNegSim, L_IMidSim, L_INegSim;
     float   *pL_Ia, *pL_Ib, *pL_Ic;

     //Line bridge has voltage at its terminals
    if ( (IN(Q1Status) || !IN(KdfClsCmd)) && !PARM(DfDynVltSim))
    {
        if ( L_AngGridSim < PI/3.0F )                   //Sector1: Van-pos,Vbn-mid,Vcn-neg
            {
                L_VPosSim  = L_VcnGridSim;
                L_VMidSim  = L_VbnGridSim;
                L_VNegSim  = L_VanGridSim;
                pL_Ic   = &L_IPosSim;
                pL_Ib   = &L_IMidSim;
                pL_Ia   = &L_INegSim;
            }
        else if ( L_AngGridSim < 2.0F*PI/3.0F )         //Sector2: Vbn-pos,Van-mid,Vcn-neg
            {
                L_VPosSim  = L_VcnGridSim;
                L_VMidSim  = L_VanGridSim;
                L_VNegSim  = L_VbnGridSim;
                pL_Ic   = &L_IPosSim;
                pL_Ia   = &L_IMidSim;
                pL_Ib   = &L_INegSim;
            }
        else if ( L_AngGridSim < PI )                   //Sector3: Vbn-pos,Vcn-mid,Van-neg
            {
                L_VPosSim  = L_VanGridSim;
                L_VMidSim  = L_VcnGridSim;
                L_VNegSim  = L_VbnGridSim;
                pL_Ia   = &L_IPosSim;
                pL_Ic   = &L_IMidSim;
                pL_Ib   = &L_INegSim;
            }
        else if ( L_AngGridSim < 4.0F*PI/3.0F )         //Sector4: Vcn-pos,Vbn-mid,Van-neg
            {
                L_VPosSim  = L_VanGridSim;
                L_VMidSim  = L_VbnGridSim;
                L_VNegSim  = L_VcnGridSim;
                pL_Ia   = &L_IPosSim;
                pL_Ib   = &L_IMidSim;
                pL_Ic   = &L_INegSim;
            }
        else if ( L_AngGridSim < 5.0F*PI/3.0F )         //Sector5: Vcn-pos,Van-mid,Vbn-neg
            {
                L_VPosSim  = L_VbnGridSim;
                L_VMidSim  = L_VanGridSim;
                L_VNegSim  = L_VcnGridSim;
                pL_Ib   = &L_IPosSim;
                pL_Ia   = &L_IMidSim;
                pL_Ic   = &L_INegSim;
            }
        else                                            //( L_AngGridSim > 5*PI/3 )   Sector6: Van-pos,Vcn-mid,Vbn-neg
            {
                L_VPosSim  = L_VbnGridSim;
                L_VMidSim  = L_VcnGridSim;
                L_VNegSim  = L_VanGridSim;
                pL_Ib   = &L_IPosSim;
                pL_Ic   = &L_IMidSim;
                pL_Ia   = &L_INegSim;
        }

        L_VRectSim = L_VPosSim - L_VNegSim;


        // Following simplified rectifier bridge model assumes that only two diodes conduct
        // at any given time; therefore, current flows discontinuously and transitions
        // between diodes are instantaneous. The value of L_RchgEqvSim depends on Q1 status.

        if ((L_VRectSim - L_VdcSim) > 0.0F)
        {
            L_IPosSim = (L_VRectSim - L_VdcSim) * 0.5F * L_RchgEqvSimRcp;
            L_INegSim    = -(L_IPosSim);
            L_IMidSim = 0.0F;
            L_IdcSim =  L_IPosSim;
        }
        else
        {
            L_IPosSim = 0.0F;
            L_IMidSim = 0.0F;
            L_INegSim = 0.0F;
            L_IdcSim  = 0.0F;
        }

        // L_Ia,b,cSim are changed from L_Ia,b,c
        L_IaChgSim = -*pL_Ia;
        L_IbChgSim = -*pL_Ib;
        L_IcChgSim = -*pL_Ic;

        DcPwrSim = L_VdcSim * L_IdcSim + IN(L_D_PwL_RTot);


        L_ILoadSim   = (L_LdPwrSim*L_VdcSimRcp) + L_RBldCapRcpSim*L_VdcSim + IN(IdbTot);

        OUT(L_VdcSim)     =  L_VdcSim + CapDtSim*(L_IdcSim - L_ILoadSim);


        if ( L_VdcSim >= PARM(L_VdcMaxSim))
        {
             OUT(L_VdcSim) =  PARM(L_VdcMaxSim);
        }
        else if (L_VdcSim <= 0.0F)
        {
             OUT(L_VdcSim) = 0.0F;
        }
        cVectorf::DemodLN3(L_IaChgSim,L_IbChgSim,L_IcChgSim,&L_SinCosSim,&L_IxChgSim,&L_IyChgSim);

        // At this point, simulated grid current plus filter current is
        // the previous (Z1) value of line bridge current, since stator has zero current
        if ( IN(Q1Status) )
        {
            ChargrXdropSim = L_IxChgSim * PARM(L_RchgEqv2Sim);
            ChargrYdropSim = L_IyChgSim * PARM(L_RchgEqv2Sim);
        }
        else
        {
            ChargrXdropSim = L_IxChgSim * PARM(L_RchgEqv1Sim);
            ChargrYdropSim = L_IyChgSim * PARM(L_RchgEqv1Sim);
        }

   //     L_VxBrgSim  = L_VxXfmrSecSim + ChargrXdropSim;
  //      L_VyBrgSim  = L_VyXfmrSecSim + ChargrYdropSim;
        L_VxBrgSim  = L_VxFilSim + ChargrXdropSim;
        L_VyBrgSim  = L_VyFilSim + ChargrYdropSim;
        L_IxBrgSim = L_IxChgSim;
        L_IyBrgSim = L_IyChgSim;
    }
    else if (IN(Q1Status) || !IN(KdfClsCmd) || PARM(DfDynVltSim))
    {
        if(IN(Q1Status) || !IN(KdfClsCmd))
        {
  //          cVectorf::RotateLN(L_VxXfmrSecSim, L_VyXfmrSecSim, &L_SinCosSim, &L_VanChgSim, &L_VbnChgSim);
            cVectorf::RotateLN(L_VxFilSim, L_VyFilSim, &L_SinCosSim, &L_VanChgSim, &L_VbnChgSim);
        }
        else if(PARM(DfDynVltSim))  // off grid mode
        {
            cVectorf::RotateLN(L_VxLnSim, L_VyLnSim, &L_SinCosSim, &L_VanChgSim, &L_VbnChgSim);
        }
        else
        {
            L_VanChgSim = L_VbnChgSim = 0.0F;
        }
        L_VcnChgSim = - L_VanChgSim - L_VbnChgSim;

        L_ChgAngSelSim = (L_VanChgSim > L_VbnChgSim ? 4 : 0)+
                         (L_VbnChgSim > L_VcnChgSim ? 2 : 0)+
                         (L_VcnChgSim > L_VanChgSim ? 1 : 0);
        switch (L_ChgAngSelSim)
        {
        case 6:     //a>b>c
            L_VPosSim  = L_VanChgSim;
            L_VMidSim  = L_VbnChgSim;
            L_VNegSim  = L_VcnChgSim;
            pL_Ia   = &L_IPosSim;
            pL_Ib   = &L_IMidSim;
            pL_Ic   = &L_INegSim;
            break;
        case 4:     //a>c>b
            L_VPosSim  = L_VanChgSim;
            L_VMidSim  = L_VcnChgSim;
            L_VNegSim  = L_VbnChgSim;
            pL_Ia   = &L_IPosSim;
            pL_Ib   = &L_INegSim;
            pL_Ic   = &L_IMidSim;
            break;
        case 2:     //b>a>c
            L_VPosSim  = L_VbnChgSim;
            L_VMidSim  = L_VanChgSim;
            L_VNegSim  = L_VcnChgSim;
            pL_Ia   = &L_IMidSim;
            pL_Ib   = &L_IPosSim;
            pL_Ic   = &L_INegSim;
            break;
        case 3:     //b>c>a
            L_VPosSim  = L_VbnChgSim;
            L_VMidSim  = L_VcnChgSim;
            L_VNegSim  = L_VanChgSim;
            pL_Ia   = &L_INegSim;
            pL_Ib   = &L_IPosSim;
            pL_Ic   = &L_IMidSim;
            break;
        case 5:     //c>a>b
            L_VPosSim  = L_VcnChgSim;
            L_VMidSim  = L_VanChgSim;
            L_VNegSim  = L_VbnChgSim;
            pL_Ia   = &L_IMidSim;
            pL_Ib   = &L_INegSim;
            pL_Ic   = &L_IPosSim;
            break;
        case 1:     //c>b>a
        default:    //a=b=c
            L_VPosSim  = L_VcnChgSim;
            L_VMidSim  = L_VbnChgSim;
            L_VNegSim  = L_VanChgSim;
            pL_Ia   = &L_INegSim;
            pL_Ib   = &L_IMidSim;
            pL_Ic   = &L_IPosSim;
            break;
        }
        L_VRectSim = L_VPosSim - L_VNegSim;
        if ((L_VRectSim - L_VdcSim) > 0.0F)
        {
            L_IPosSim = (L_VRectSim - L_VdcSim) * 0.5F * L_RchgEqvSimRcp;
            L_INegSim    = -(L_IPosSim);
            L_IMidSim = 0.0F;
            L_IdcSim =  L_IPosSim;
        }
        else
        {
            L_IPosSim = 0.0F;
            L_IMidSim = 0.0F;
            L_INegSim = 0.0F;
            L_IdcSim  = 0.0F;
        }
        OUT(L_IaBrgSim) = -*pL_Ia;
        OUT(L_IbBrgSim) = -*pL_Ib;
        OUT(L_IcBrgSim) = -*pL_Ic;
        DcPwrSim = L_VdcSim * L_IdcSim + IN(L_D_PwL_RTot);


        L_ILoadSim   = (L_LdPwrSim*L_VdcSimRcp) + L_RBldCapRcpSim*L_VdcSim + IN(IdbTot);

        OUT(L_VdcSim)     =  L_VdcSim + CapDtSim*(L_IdcSim - L_ILoadSim);


        if ( L_VdcSim >= PARM(L_VdcMaxSim))
        {
             OUT(L_VdcSim) =  PARM(L_VdcMaxSim);
        }
        else if (L_VdcSim <= 0.0F)
        {
             OUT(L_VdcSim) = 0.0F;
        }
        cVectorf::DemodLN3(L_IaBrgSim,L_IbBrgSim,L_IcBrgSim,&L_SinCosSim,&L_IxBrgSim,&L_IyBrgSim);

        // At this point, simulated grid current plus filter current is
        // the previous (Z1) value of line bridge current, since stator has zero current
        if ( IN(Q1Status) || !IN(KdfClsCmd) )
        {
            ChargrXdropSim = L_IxBrgSim * L_RchgEqvSim;
            ChargrYdropSim = L_IyBrgSim * L_RchgEqvSim;
        //    L_VxBrgSim  = L_VxXfmrSecSim + ChargrXdropSim;
       //     L_VyBrgSim  = L_VyXfmrSecSim + ChargrYdropSim;
            L_VxBrgSim  = L_VxFilSim + ChargrXdropSim;
            L_VyBrgSim  = L_VyFilSim + ChargrYdropSim;
        }
        else if(PARM(DfDynVltSim))  // off grid mode
        {
            ChargrXdropSim = L_IxBrgSim * L_RchgEqvSim;
            ChargrYdropSim = L_IyBrgSim * L_RchgEqvSim;
            L_VxBrgSim  = L_VxLnSim + ChargrXdropSim;
            L_VyBrgSim  = L_VyLnSim + ChargrYdropSim;
        }
    }
    else
    {
        L_VxBrgSim = L_VyBrgSim = 0.0F;
        L_IaChgSim = L_IbChgSim = L_IcChgSim = 0.0F;
        L_VRectSim = L_VdcSim;
        L_IdcSim= DcPwrSim = 0.0;

        L_VdcSim  += DisChargWDtSim * (1.0F - L_VdcSim);
        L_VdcSim  += CapDtSim *(-IN(IdbTot));
        L_IxBrgSim = 0.0F;
        L_IyBrgSim = 0.0F;
    }

    L_IxBrgSimZ1 = L_IxBrgSim;
    L_IyBrgSimZ1 = L_IyBrgSim;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the L_Charging and decaying of the dc link when the
//      control is in simulator mode. Runs in sim mode when line converter 'InnrRegEnb' is FALSE
// EXECUTION LEVEL
//      Line Task 2-- Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::LinkChargeSimulator()
{
    //Equivalent charging path resistance in sim mode
    if (IN(Q1Status))
    {
        L_RchgEqvSim = PARM(L_RchgEqv2Sim);                    // Breaker Q1 is closed in sim-mode
        L_RchgEqvSimRcp = L_RchgEqv2SimRcp;
    }
    else
    {
        L_RchgEqvSim = PARM(L_RchgEqv1Sim);                    // Breaker Q1 is open in sim-mode
        L_RchgEqvSimRcp = L_RchgEqv1SimRcp;
    }
}
*/
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function use simulator frequency to calculate simulator angle.
//      provisions are made for steping the angle via an EE.
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::AngleSimulator(void)
{
    L_AngGridSim = L_AngGridSim + L_DelAngSim;
    L_AngGridSim = cVectorf::Modulo2Pi(L_AngGridSim);

    cVectorf::SineCosine(L_AngGridSim, &L_SinCosLineSim);

    L_WElecSim   = L_WSim;
    L_AngElecSim = L_AngGridSim;
    L_SinCosSim  = L_SinCosLineSim;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//     This function generates two line to line voltage which represent the
//     input voltage to the source in simulator mode.  Three line to neutral
//     voltage are created first, with capability provided to allow the user
//     to simulate a line to neutral short or partial short via parameters.
//     A partial or full line to line short can also be simulated between lines
//     a and b.
//     This function also simulates the voltage feedback for the source when
//     simulator mode is active.
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::VoltsSimulator(void)
{
    //-------------------------------------------------------------------------
    //                   Simulate Line Voltage
    //-------------------------------------------------------------------------

 
    float L_VanGridTempSim;
    float L_VbnGridTempSim;
    float L_VcnGridTempSim;

    cVectorf::RotateLN(-L_VlnSSInSim, 0.0F, &L_SinCosLineSim, &L_VanGridRotSim, &L_VbnGridRotSim);
    L_VcnGridRotSim = -(L_VanGridRotSim + L_VbnGridRotSim);

    L_VanGridTempSim = L_VanGridRotSim ;
    L_VbnGridTempSim = L_VbnGridRotSim ;
    L_VcnGridTempSim = L_VcnGridRotSim ;

    // Apply the imblance on the grid voltage
    if (AsymmEnb)    
    {
        L_VanGridRotSim = L_VanGridRotSim + PARM(L_VoltImbSclSim) * L_VcnGridTempSim;
        L_VbnGridRotSim = L_VbnGridRotSim + PARM(L_VoltImbSclSim) * L_VbnGridTempSim;
        L_VcnGridRotSim = L_VcnGridRotSim + PARM(L_VoltImbSclSim) * L_VanGridTempSim;
    }



    if(HVRTEnb)
    {
       
            if(PARM(HvrtPhsTypSim)&0x01)
                L_VanGridSim = L_VanGridRotSim * PARM(HvrtLvlSim);
            if(PARM(HvrtPhsTypSim)&0x02)
                L_VbnGridSim = L_VbnGridRotSim * PARM(HvrtLvlSim);
            if(PARM(HvrtPhsTypSim)&0x04)
                L_VcnGridSim = L_VcnGridRotSim * PARM(HvrtLvlSim);
           
    }
    else
    {
       
        L_VanGridSim          = L_VanGridRotSim;
        L_VbnGridSim          = L_VbnGridRotSim;
        L_VcnGridSim          = L_VcnGridRotSim;
       
        
    }
    
    
  ////////////////////////////////////////////////////////  
    // Form line to line voltages
    L_VabGridSim     = L_VanGridSim - L_VbnGridSim;
    L_VbcGridSim     = L_VbnGridSim - L_VcnGridSim;

    // Rotate Line voltage at Line angle
    cVectorf::DemodLL(L_VabGridSim,L_VbcGridSim,&L_SinCosSim,&VxGridSim,&VyGridSim);

    L_VabLineSim = L_VabGridSim;         // 08/26/03 DS - these vars are not used anywhere
    L_VbcLineSim = L_VbcGridSim;         // IBC intended to populate them in S0simu.c30
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    This function generates a simulator angle which is used to rotate a
//    simulated set of line voltages.  This function provides capability to
//    allow output to rotate at a steady rate or for the rate of anble change
//    to slew at an adjustable rate.  Provisions for stepping the angle are
//    also made.
//
// EXECUTION LEVEL
//      Line Task 2--Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::AngleSlewSim(void)
{
    float   Slw_Int_Min, Slw_Int_Max;

    if ( !IN(GmRevSeq))
    {
        Slw_Int_Max = PARM(L_WMaxSim)*PARM(L_WGridSim);
        Slw_Int_Min = -PARM(L_WMinSim)*PARM(L_WGridSim);
    }
    else
    {
        Slw_Int_Max = -PARM(L_WMaxSim)*PARM(L_WGridSim);
        Slw_Int_Min =  PARM(L_WMinSim)*PARM(L_WGridSim);
    }
    //if ( L_AcLinePres)//use for applicaition that don't always need line present
    //{
    if (PARM(L_WSlwSim) <= PARM(L_WSlwMaxSim))          //freeze integrator if slew rate > 100.0
    {
        L_WSlwIntSim = L_WSlwIntSim + PARM(L_WSlwSim)*INS(L_DelTm1)*L_WSlwSignSim;
        if ( L_WSlwIntSim >= Slw_Int_Max)
        {
            L_WSlwSignSim = -1.0F;
        }
        else if ( L_WSlwIntSim <= Slw_Int_Min)
        {
            L_WSlwSignSim = 1.0F;
        }
        if ( PARM(L_WSlwSim) == 0.0F)
        {
            L_WSlwIntSim = 0.0F;
        }
    }

    L_WSim       = (PARM(L_WGridSim) + L_WGrdFrqStpSim)  + L_WSlwIntSim;



   if((PARM(AngleStepSim) !=AngStpSimZ1)&&AngAlign)
    {
        L_DelAngSim  = INS(L_DelTm1)*L_WSim + (PARM(AngleStepSim )- AngStpSimZ1);
    }
   else
   {
        L_DelAngSim  = INS(L_DelTm1)*L_WSim;
   }

   AngStpSimZ1  = PARM(AngleStepSim);


    L_WSim       = L_DelAngSim * INS(L_DelTm1Rcp);
    //}
    //else //use for applications that don't need line always present
    //{
    //   L_AngGridSim    = 0.0F;
    //   L_DelAngSim     = 0.0F;
    //   L_WSim          = 0.0F;
    //   L_WSlwIntSim    = 0.0F;
    //   AngStpSimZ1     = AngleStepSim;
    //}
}

// Moved from y2Simu.cpp
///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    This function calculates a "simulated" ac line voltage feedback
//    for simulator mode.
//
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::VoltsFbkSimulator()
{
    // Use the voltage synthetisized by the Modilator as the input of the isim
    //-----------------------------------------------------------------------------
    //                  Simulate Voltage Feedbacks
    //-----------------------------------------------------------------------------
    OUT(L_VabBrgSim) = L_BrgSim.VaBrgSim - L_BrgSim.VbBrgSim;
    OUT(L_VbcBrgSim) = L_BrgSim.VbBrgSim - L_BrgSim.VcBrgSim;
    cVectorf::DemodLL(L_VabBrgSim,L_VbcBrgSim,&L_SinCosSim,&L_VxBrgSim,&L_VyBrgSim);

}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    This functions runs in simulator mode only and is used to contstruct
//    L_Ia, L_Ib, L_Ic from L_IxFbk, L_IyFbk.
//
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::RotateCurrentSim()
{
    cVectorf::RotateLN(L_IxBrgSim,L_IyBrgSim,&L_SinCosSim,&L_IaBrgSim,&L_IbBrgSim);
    L_IcBrgSim = -(L_IaBrgSim + L_IbBrgSim);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    This function calculate the grid monitor voltage and current for isim
//
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x:: GridMonitorProcessFbksSim( void )
{


    
    L_VxXfmrSecFil +=  6.28 * 4000 * INS(L_DelTm1) * (L_VxXfmrSecSim - L_VxXfmrSecFil);
    L_VyXfmrSecFil +=  6.28 * 4000 * INS(L_DelTm1) * (L_VyXfmrSecSim - L_VyXfmrSecFil);

    //cVectorf::RotateLN(L_VxXfmrSecSim, L_VyXfmrSecSim, &L_SinCosSim, &L_VaXfmrSecSim, &L_VbXfmrSecSim);
    cVectorf::RotateLN(L_VxXfmrSecFil, L_VyXfmrSecFil, &L_SinCosSim, &L_VaXfmrSecSim, &L_VbXfmrSecSim);
    OUT(L_VcXfmrSecSim)    = -(L_VaXfmrSecSim + L_VbXfmrSecSim);

 
    // CL, 2014-12-08 
    cVectorf::RotateLN(L_VxLnSim, L_VyLnSim, &L_SinCosSim, &L_VaLnSim, &L_VbLnSim);  
    L_VcLnSim   = -(L_VaLnSim + L_VbLnSim);
    cVectorf::RotateLN( L_IxXfmrSecSim, L_IyXfmrSecSim, &L_SinCosSim, &OUT(L_IaXfmrSecSim), &OUT(L_IbXfmrSecSim));
    OUT(L_IcXfmrSecSim)     = -(L_IaXfmrSecSim + L_IbXfmrSecSim);
}

///////////////////////////////////////////////////////////////////////////////
// Function: LowVoltageRidThroughTestSequencing
// Execution Level: Task 3
// Execution Time: Not tested
// Formal Inputs: None
// Formal Outputs: None
//
// Global Inputs:  V.LVTAdl
//
//
// Global Outputs: V.LVTShortIp
//
// Locals:         V.LVTFb1LossSt
//
//
// Abstract:
//    This function controls the sequencing for low voltage ridegthrough testing and
// other grid fault testing
//
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::GridEventTestSim(void)
{

    if ( !INS(GEvtEnbSim) )
    {
        GEvtStateSim     = GEVT_REST;
        //L_RGridDyRcp = L_RGridRcp;
        //L_RCDvdrSmDy = L_RCDvdrSim;
        //L_RCFdDyRcp  = L_RCFeedRcp;
    }
    else
    {

        if ( GEvtStateSim == GEVT_REST )
        {
            GEvtRdyTmrUSim = GEvtRdyTmrUSim + INS(L_DelTm1);
            if ( GEvtRdyTmrUSim > PARM(GEvtWaitTmSim) )
            {
                GEvtRdyTmrUSim = PARM(GEvtWaitTmSim);
                GEvtReadySim   = true;
            }
            else
            {
                GEvtReadySim  = false;
            }
        }
        else
        {
            GEvtRdyTmrUSim = 0.0F;
            GEvtReadySim   = false;
        }

        GEvtRdyTmrSim = PARM(GEvtWaitTmSim) - GEvtRdyTmrUSim;

        GEvtReqSimZ1 = GEvtReqSim;
        if ( GEvtReqADLSim && GEvtReadySim )
        {
            GEvtReqSim = true;
        }
        else
        {
            GEvtReqSim   = false;
        }

        if ( !GEvtReadySim )
        {
            GEvtReqADLSim = false;
        }

        // Use as state machine to sequence relays
        switch (GEvtStateSim)   //PROT_305.vsd
        {

            //-----------------------------------------------------------------------------
            // Stay here waiting waiting for LVTReq
            case GEVT_REST: //(0)

                GEvtCycleNoSim   = 0.0F;
                GEvtIpSim   = false;
                GEvtDlyTmrSimDp  = 0.0;
                GEvtDlyTmrSim    = 0.0F;
                //L_RGridDyRcp = L_RGridRcp;
                //L_RCDvdrSmDy = L_RCDvdrSim;
                //L_RCFdDyRcp  = L_RCFeedRcp;

                if ( GEvtReqSim && !GEvtReqSimZ1 )
                {
                    GEvtStateSim   = GEVT_READY1;
                    GEvtCycleNoSim = 1;
                }
                GEvtSCompDlyTmrSim = 0.0F;

                break;

            //-----------------------------------------------------------------------------
            // Ready 1 state
            case GEVT_READY1:  //(2) Justify if it's a combo of SC + LVRT								
            if(PARM(GEvtTypeSim) == GRID_SCOMP_RS) // Remote Short@SC, Can Extend to more def@SC				
            {					
                SCEnb = true;					
                GEvtSCompDlyTmrSim += INS(L_DelTm1);					
                if( GEvtSCompDlyTmrSim > PARM(GEvtSCRdyTm)) // delay P.GEvtSCRdyTm for steady-state					
                {					  
                    GEvtStateSim   = GEVT_READY2;					  
                    GEvtSCompDlyTmrSim = 0.0F;					
                }
            }
            else
            {
                GEvtStateSim   = GEVT_READY2; // normal Grid Event w/o SComp                
            }
                break;

            //-----------------------------------------------------------------------------
            // Ready 2 state
            case GEVT_READY2:  //(3)

                GEvtStateSim   = GEVT_CLOSE;

                break;

            //-----------------------------------------------------------------------------
            // Close the shorting device
            case GEVT_CLOSE:   //(3)

                if(AngAlign)
                {
                    GEvtStateSim   = GEVT_WAIT;
                    if ( PARM(GEvtTypeSim) == GRID_OPEN)
                    {
                        //L_RGridDyRcp = L_RGridOpRcp;
                        //L_RCDvdrSmDy = L_RCDvdrOSim;
                        //L_RCFdDyRcp  = L_RCFeedORcp;

                    }
                    else if ( PARM(GEvtTypeSim) == GRID_HVRT)
                    {
                        HVRTEnb = true;
                    }
                    else if ( PARM(GEvtTypeSim) == GRID_REMSHORT || PARM(GEvtTypeSim) == GRID_SCOMP_RS)
                    {
                        RShortEnb = true;
                    }
                    else if ( PARM(GEvtTypeSim) == GRID_LOCSHORT)
                    {
                        LShortEnb = true;
                    }
                    else if ( PARM(GEvtTypeSim) == GRID_SCOMP)
                    {
                        SCEnb = true;
                    }
                    else if ( PARM(GEvtTypeSim) == GRID_ASYMMETRIC)  
                    {                       
                        AsymmEnb = true;        
                    }
   //                 else if ( PARM(L_VFaultTypeSim) == GRID_STAUP)
   //                 {
  //                      StartEnb = true;
  //                  }
                }

                break;


           //-----------------------------------------------------------------------------
           // Keep the event active while waiting for the time to expire
            case GEVT_WAIT:   //(4)

                GEvtStateSim   = GEVT_WAIT;
                GEvtDlyTmrSimDp  = GEvtDlyTmrSimDp + (double)(INS(L_DelTm1));
                GEvtDlyTmrSim    = (float)GEvtDlyTmrSimDp;
                if  (GEvtDlyTmrSim > PARM(GEvtTmSim) )
                {
                    GEvtIpSim = false;
                    GEvtDlyTmrSimDp  = 0.0;
                    GEvtDlyTmrSim  = 0.0F;
                    GEvtStateSim   = GEVT_OPEN1;

                    HVRTEnb = false;
                    RShortEnb = false;
                    LShortEnb = false;
                    AsymmEnb  = false;
                    if( PARM(GEvtTypeSim) == GRID_SCOMP )
                        SCEnb = false;
   //                 StartEnb = false;
                }

                break;

           //-----------------------------------------------------------------------------
           // Stop cycle or wait if double hit
            case GEVT_OPEN1:    //(6)

                if ( PARM(GEvtDoubleHitSim) && (GEvtCycleNoSim == 1) )
                {
                    GEvtDlyTmrSimDp  = GEvtDlyTmrSimDp + (double)(INS(L_DelTm1));
                    GEvtDlyTmrSim    = (float)GEvtDlyTmrSimDp;
                    if  ( GEvtDlyTmrSim >  PARM(GEvtDblRstTmSim) )
                    {
                        GEvtStateSim   = GEVT_CLOSE;
                        GEvtCycleNoSim = 2;
                        GEvtDlyTmrSimDp  = 0.0;
                        GEvtDlyTmrSim  = 0.0F;
                    }
                }
                else
                {
                    GEvtStateSim  =  GEVT_OPEN2;
                    GEvtDlyTmrSimDp  = 0.0;
                    GEvtDlyTmrSim = 0.0F;
                }

                break;

           //-----------------------------------------------------------------------------
           // Last state -- go rest
            case GEVT_OPEN2:  //(7)

                if(PARM(GEvtTypeSim) == GRID_SCOMP_RS) // Wait P.GEvtSCRdyTm after grid event is removed                
                {
                    GEvtSCompDlyTmrSim += INS(L_DelTm1);                    
                    if( GEvtSCompDlyTmrSim > PARM(GEvtSCRcvTm))                    
                    {                       
                        SCEnb = false;            
                        GEvtSCompDlyTmrSim = 0;             
                        GEvtStateSim = GEVT_REST;         
                    }        
                }      
                else        
                    GEvtStateSim = GEVT_REST;

                break;

          //-----------------------------------------------------------------------------
          // Default

            default:
                break;

        }
    }

}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the converter bridge model.
//
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////

void cSimPlant1x::LineBrgSimulator(void)
{     
  
    if (PARM(DfDynVltSim))
    {
        L_VaBrgSrcSim = L_VaLnSim;    
        L_VbBrgSrcSim = L_VbLnSim;    
        L_VcBrgSrcSim = L_VcLnSim; 

    }
    else    
    {
        if (!IN(Q1Status) && !IN(KdfClsCmd))  
        {
            L_VaBrgSrcSim = L_VaXfmrSecSim + L_IaBrgSim * PARM(L_RchgSim);   
            L_VbBrgSrcSim = L_VbXfmrSecSim + L_IbBrgSim * PARM(L_RchgSim);  
            L_VcBrgSrcSim = L_VcXfmrSecSim + L_IcBrgSim * PARM(L_RchgSim);            
        }
        else if(!IN(Q1Status) && IN(KdfClsCmd))  
        {            
            L_VaBrgSrcSim = 0.0;     // need to change to be the cap voltage, CL 2014-12-22
            L_VbBrgSrcSim = 0.0;    
            L_VcBrgSrcSim = 0.0;   
        
        }
        else
        {
            L_VaBrgSrcSim = L_VaXfmrSecSim;    
            L_VbBrgSrcSim = L_VbXfmrSecSim;    
            L_VcBrgSrcSim = L_VcXfmrSecSim;   
        }
    }
    
    
    L_IaBrgInSim  =  - L_IaBrgSim;
    L_IbBrgInSim  =  - L_IbBrgSim;
    L_IcBrgInSim  =  - L_IcBrgSim;
   
    L_BrgSim.ConvBrgSim();
     
       
    L_SaBrgSim = L_BrgSim.SaBrgSim;
    L_SbBrgSim = L_BrgSim.SbBrgSim;
    L_ScBrgSim = L_BrgSim.ScBrgSim;    
    
    OUT(L_VanBrgSim)  = L_BrgSim.VaBrgSim;
    OUT(L_VbnBrgSim)  = L_BrgSim.VbBrgSim;
    OUT(L_VcnBrgSim)  = L_BrgSim.VcBrgSim;
    
    L_IdcSim = L_BrgSim.IdcBrgSim;


}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the bridge inductor model.
//
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::LineBrgCurSim(void)
{


    float L_RbrgEqvSim, L_BrgXorSim;
    
    // Bridge equivalent resistance
    if (!IN(Q1Status) && !IN(KdfClsCmd) )     // pre-charge state
    {
        L_RbrgEqvSim = PARM(L_RbrgSim) + PARM(L_RchgSim);
    }
    else
    {
        L_RbrgEqvSim = PARM(L_RbrgSim);
    }
    // When XOR smaller than 0.005, using steady-state impedance model from the stability point of view.
    L_BrgXorSim = NoNANLim( (PARM(L_LbrgSim) * PARM(L_WGridSim)) / L_RbrgEqvSim, 1.0e6F, 0.0F);
    L_BigRchg = 0;
    
     if ( ( L_BrgXorSim < 0.005 ) && (INS(L_DelTm1) > 5.0e-6) )
    {
        
        if (!IN(Q1Status) && !IN(KdfClsCmd) )   // pre-charge state
        {
            L_BigRchg = 1;
            L_IaBrgSim = ( L_VanBrgSim - L_VaXfmrSecSim ) * L_RchgEqv1SimRcp;
            L_IbBrgSim = ( L_VbnBrgSim - L_VbXfmrSecSim ) * L_RchgEqv1SimRcp;
            L_IcBrgSim = ( L_VcnBrgSim - L_VcXfmrSecSim ) * L_RchgEqv1SimRcp; 
        }
        else
        {
            L_IaBrgSim = ( L_VanBrgSim - L_VaBrgSrcSim ) * L_RchgEqv2SimRcp;
            L_IbBrgSim = ( L_VbnBrgSim - L_VbBrgSrcSim ) * L_RchgEqv2SimRcp;
            L_IcBrgSim = ( L_VcnBrgSim - L_VcBrgSrcSim ) * L_RchgEqv2SimRcp;         
        }
    }
    else
    {
        // Calculate phase current
        L_IaBrgSim += L_LbrgSimRcp * INS(L_DelTm1) * ( L_VanBrgSim - L_VaBrgSrcSim - L_IaBrgSim * L_RbrgEqvSim) ;
        L_IbBrgSim += L_LbrgSimRcp * INS(L_DelTm1) * ( L_VbnBrgSim - L_VbBrgSrcSim - L_IbBrgSim * L_RbrgEqvSim) ;
        L_IcBrgSim += L_LbrgSimRcp * INS(L_DelTm1) * ( L_VcnBrgSim - L_VcBrgSrcSim - L_IcBrgSim * L_RbrgEqvSim) ;
    }
  
   
    if ( !IN(L_InnrRegEnbT1) )
    {
        if(L_SaBrgSim == LEG_OFF)  L_IaBrgSim = 0;
        if(L_SbBrgSim == LEG_OFF)  L_IbBrgSim = 0;
        if(L_ScBrgSim == LEG_OFF)  L_IcBrgSim = 0;
    }
    


    
    L_IxCalLSimZ1 = L_IxCalLSim;
    L_IyCalLSimZ1 = L_IyCalLSim;
        
    cVectorf::DemodLN3(L_IaBrgSim,L_IbBrgSim,L_IcBrgSim,&L_SinCosSim,&L_IxCalLSim,&L_IyCalLSim);
    
     
    L_IxBrgSim  = 0.5 * (L_IxCalLSim + L_IxCalLSimZ1);
    L_IyBrgSim  = 0.5 * (L_IyCalLSim + L_IyCalLSimZ1);
    
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs the dc-link capacitor model.
//
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::LineDCvoltSim(void)
{    

    L_VdcSimRcp =  NoNANLim( 1.0/L_VdcSim, 1.0e6F, -1.0e6F );  
    
    L_ILoadSim  = R_IdcSim + ( READ_PARMPTR(L_LdPwrAdrSim) + IN(PwrBatChpr) ) * L_VdcSimRcp + L_RBldCapRcpSim * L_VdcSim + IN(IdbTot);

    OUT(L_VdcSim)  =  L_VdcSim + CapDtSim * (L_IdcSim - L_ILoadSim);

    if ( L_VdcSim >= PARM(L_VdcMaxSim))
    {
         OUT(L_VdcSim) =  PARM(L_VdcMaxSim);
    }
    else if (L_VdcSim <= 0.0F)
    {
         OUT(L_VdcSim) = 0.0F;
    }

}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function simulates the rotor back emf.
//
// EXECUTION LEVEL
//      Rotor Task 1A
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::RotorBackEmfSim(void)
{    
    if ( IN(R_InnrRegEnbT1) || IN(S_SimK1ClsdT1) )
    {
        R_VxEmfSim = K_SR * ( (S_FlxXSim - S_FlxXSimZ1) * INS(R_DelTm1Rcp) - (S_FlxYSim * R_WElecSim) );  
        R_VyEmfSim = K_SR * ( (S_FlxYSim - S_FlxYSimZ1) * INS(R_DelTm1Rcp) + (S_FlxXSim * R_WElecSim) );
    }
    else 
    {
        R_VxEmfSim = 0.0F;  
        R_VyEmfSim = 0.0F;
    
    }
   
    
    cVectorf::RotateLN(R_VxEmfSim, R_VyEmfSim,&R_SinCosSim,&OUT(R_VaEmfSim),&OUT(R_VbEmfSim));
    OUT(R_VcEmfSim) = -(R_VaEmfSim + R_VbEmfSim);

    
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs rotor converter bridge model.
//
// EXECUTION LEVEL
//      Rotor Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////

void cSimPlant1x::RotorBrgSimulator(void)
{     
  
    R_IaBrgInSim  =  - R_IaBrgSim;
    R_IbBrgInSim  =  - R_IbBrgSim;
    R_IcBrgInSim  =  - R_IcBrgSim;
   
   
    if ( IN(R_InnrRegEnbT1) || IN(S_SimK1ClsdT1) )
    {
        R_BrgSim.ConvBrgSim(); 
        R_IdcSim = - R_BrgSim.IdcBrgSim;
    }
    else 
    {    
        R_BrgSim.VabBrgSim = 0.0F;
        R_BrgSim.VbcBrgSim = 0.0F;         
        R_IdcSim = 0.0F;
    }


}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs line side bridge IOC.
//
// EXECUTION LEVEL
//      Line Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::LineBrgIocSim(void)
{  

    if ( (L_IaBrgSim > PARM (L_IocThrsSim) ) ||  (L_IaBrgSim < - PARM (L_IocThrsSim) ) )
    {
        L_IocPhsASim = true;    
    }   
    if ( (L_IbBrgSim > PARM (L_IocThrsSim) ) ||  (L_IbBrgSim < - PARM (L_IocThrsSim) ) )
    {
        L_IocPhsBSim = true;    
    }   
    if ( (L_IcBrgSim > PARM (L_IocThrsSim) ) ||  (L_IcBrgSim < - PARM (L_IocThrsSim) ) )
    {
        L_IocPhsCSim = true;    
    }   

}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This function performs rotor side bridge IOC.
//
// EXECUTION LEVEL
//      Rotor Task 1A
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::RotorBrgIocSim(void)
{    
    //disable rotor ioc retry function
    R_IocPhsASim = false;  
    R_IocPhsBSim = false;  
    R_IocPhsCSim = false;  
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::Task1()
{

    //-------------------------------------------------------------------------
    // run plant models

    if(PARM(AngTest))
    {
        
        if( (fabs(L_AngGridSim - PARM(L_AngMatch))<= PARM(AngMatchMargin)) 
			&& 
			(fabs(R_AngMechSim - PARM(R_AngMatch))<= PARM(AngMatchMargin)) 
		  )
            AngAlign = 1;
        else
            AngAlign = 0;

    }
    else
        AngAlign = 1;
    
    AngleSlewSim();
    LineSimulator();

 
    if ( IN(InitDone) )
    {
        AngleSimulator();                  // Simulate angle feedback
        GridEventTestSim();  // LVRT simulator
        VoltsSimulator();                // Simulate grid volts
        LineVoltSimulator();             // Simulate line volts in XY
           
        GridMonitorProcessFbksSim();     // Rotation of the grid monitor V/I to abc
        
        LineBrgIocSim();                 // Line bridge IOC
        LineBrgSimulator();              // Simulate bridge
        LineBrgCurSim();                 // Simulate inductor currents
        LineDCvoltSim();                 // Simulate dc voltage     
          
        LineDFCurSim();               // Simulate harmonic filter currents
        VoltsFbkSimulator();          // Simulate feedback
    }
 

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      Task1A
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void cSimPlant1x::Task1A()
{

    //-------------------------------------------------------------------------
    // run plant models
    RotorMtrModelMechSim();
    RotorCmdFbkAnglSim();

    RotorDemodVoltageSim();
    RotorMtrElecSim();
        
    RotorBackEmfSim();           // Rotor back emf
    RotorBrgSimulator();         // Rotor bridge voltage simulated

    RotorVoltageFbkSim();
    RotorCurrentFbkSim();
    RotorBrgIocSim();             // Rotor bridge IOC
    
    return;
}

