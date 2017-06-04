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


// Include Files
//--------------

// system
// core
// framework
// product
#include "s0SeriesCmpSim.h"



// Constants
//----------


// Class Instances
//----------------







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
cSeriesCmpSim::cSeriesCmpSim
(
        InTyp    InList,
        InsTyp   InsList
)
        : In(InList), Ins(InsList)
{
    SCompState = 0;
    Ibypass = 0.0;
    IbypassZ = 0.0;
    Vscmid = 0.0;
    SCompFlxSim = 0.0;
    Vsccap = 0.0;
    SCompCmd = 0;
    IscompLSim = 0;
    IscompCSim = 0.0;
    ImovSim = 0.0;
    Iout = 0.0;
    
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
cSeriesCmpSim::~cSeriesCmpSim ()
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
void cSeriesCmpSim::RunRules()
{
    unsigned int i;




   
   //  LSCompRcpSim = NoNANLim(1.0/PARM(LSCompSim),1.0e8F, -1.0e8F);
    LSCompRcpSim = 1.0 / PARM(LSCompSim);

     
     
     VsccapGroup[0] = PARM(SCMovCurVK0Sim);
     VsccapGroup[1] = PARM(SCMovCurVK1Sim);
     VsccapGroup[2] = PARM(SCMovCurVK2Sim);
     VsccapGroup[3] = PARM(SCMovCurVK3Sim);
     VsccapGroup[4] = PARM(SCMovCurVK4Sim);
     VsccapGroup[5] = PARM(SCMovCurVK5Sim);
     VsccapGroup[6] = PARM(SCMovCurVK6Sim);
     VsccapGroup[7] = PARM(SCMovCurVK7Sim);
     VsccapGroup[8] = PARM(SCMovCurVK8Sim);
     VsccapGroup[9] = PARM(SCMovCurVK9Sim);
     VsccapGroup[10] = PARM(SCMovCurVK10Sim);

     ImovGroup[0] = PARM(SCMovCurIK0Sim);
     ImovGroup[1] = PARM(SCMovCurIK1Sim);
     ImovGroup[2] = PARM(SCMovCurIK2Sim);
     ImovGroup[3] = PARM(SCMovCurIK3Sim);
     ImovGroup[4] = PARM(SCMovCurIK4Sim);
     ImovGroup[5] = PARM(SCMovCurIK5Sim);
     ImovGroup[6] = PARM(SCMovCurIK6Sim);
     ImovGroup[7] = PARM(SCMovCurIK7Sim);
     ImovGroup[8] = PARM(SCMovCurIK8Sim);
     ImovGroup[9] = PARM(SCMovCurIK9Sim);
     ImovGroup[10] = PARM(SCMovCurIK10Sim);
     for (i = 0; i< PARM(SCMovCurvNumSim) - 1; i++)
         ImovRampRat[i] = (ImovGroup[i + 1] - ImovGroup[i]) * (1.0F / (VsccapGroup[i + 1] - VsccapGroup[i]));


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
void cSeriesCmpSim::StateUpdate()
{

    if(IN(SCompEnb))
    {
        SCompCmd = 1;
  //      ShortTime += INS(DelTm1);
  //      if(ShortTime >= INS(ShortTimeIn))
 //       {
 //           ShortTime = INS(ShortTimeIn);
  //          ShortCmd = 0;
  //      }            

    }
    else
    {
  //      ShortTime = 0.0;
        SCompCmd = 0;
    }

    
    SCompState = SCompState && SCompCmd;

    if((SCompState==0) && (SCompCmd))
    {
        if((fabs(Ibypass)<INS(OpnCurThrs)) || (Ibypass * IbypassZ <0.0))
            SCompState = 1;
    }

}

void cSeriesCmpSim::SCompRun()
{

    unsigned int i = 0;
    float Vsccaptemp = 0.0;
    if (Vsccap < 0.0) Vsccaptemp = -Vsccap;
    else Vsccaptemp = Vsccap;

 //   StateUpdate();
    
///////////////////////////////////////////////////////////////////////
 //   RhfCompRcpSim = NoNANLim(1.0 / (PARM(RhfCompSim) * IN(WElecSim) * PARM(LSCompSim)), 1.0e8F, -1.0e8F);
    RhfCompRcpSim = 1.0 / (PARM(RhfCompSim) * IN(WElecSim) * PARM(LSCompSim));
    
    IbypassZ = Ibypass;

    if (SCompState == 0)
    {
        Ibypass = IN(IinSim);
        Vscmid = 0.0;
        SCompFlxSim = 0.0;
        Vsccap = 0.0;
        IscompLSim = 0;
        IscompCSim = 0.0;
        ImovSim = 0.0;
        Iout = Ibypass;

    }
    else
    {
        Ibypass = 0.0;
        Vscmid = IN(VoutSim) + Vsccap;
        SCompFlxSim = SCompFlxSim + INS(DelTm1) *
            (Vscmid - IN(VinSim) - PARM(RSCompSim) * IscompLSim);
        IscompLSim = SCompFlxSim *LSCompRcpSim + ((Vscmid - IN(VinSim) - PARM(RSCompSim) * IscompLSim) *RhfCompRcpSim);

        //Mov curve
        if(PARM(SCMovEnb))
        {
            if (Vsccaptemp > VsccapGroup[PARM(SCMovCurvNumSim) - 1])
            {
                ImovSim = ImovGroup[PARM(SCMovCurvNumSim) - 1];
            }
            else if (Vsccaptemp < VsccapGroup[0])
            {
                //        XfmrFlxPuSim = XfmrFlxGroup[0];
                ImovSim = ImovGroup[0];
            }
            else
            {
                for (i = 0; i<PARM(SCMovCurvNumSim) - 1; i++)
                {
                    if (Vsccaptemp <= VsccapGroup[i + 1])
                    {
                        ImovSim = ImovGroup[i] + ImovRampRat[i] * (Vsccaptemp - VsccapGroup[i]);
                        break;
                    }
                }
            }
            if (Vsccap < 0.0)
                ImovSim = -ImovSim;
        }
        else
            ImovSim = 0.0;

        IscompCSim = IscompLSim + ImovSim;

        Vsccap = Vsccap - IscompCSim * INS(DelTm1) / PARM(CSCompSim);
        //for test
        if (Vsccap > 700.0) Vsccap = 700.0;
        if (Vsccap < -700.0) Vsccap = -700.0;

        Iout = Ibypass + IscompLSim;
    }
   

}


