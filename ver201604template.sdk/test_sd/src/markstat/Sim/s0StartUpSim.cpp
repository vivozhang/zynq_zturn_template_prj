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
#include "s0StartUpSim.h"



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
cStartUpSim::cStartUpSim
(
        InTyp    InList,
        InsTyp   InsList
)
        : In(InList), Ins(InsList)
{
    IaStSimZ = 0.0;
    IbStSimZ = 0.0;
    IcStSimZ = 0.0;
    IaStSim = 0.0;
    IbStSim = 0.0;
    IcStSim = 0.0;
    StartFlxASim = 0.0;
    StartFlxBSim = 0.0;
    StartFlxCSim = 0.0;
    StartTime = 0.0;
 
    
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
cStartUpSim::~cStartUpSim ()
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
void cStartUpSim::RunRules()
{

    LStartRcpSim = NoNANLim(1.0/INS(LStartSim),1.0e8F, -1.0e8F);

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
void cStartUpSim::StartUp()
{

   
  //start-up circuit
  
   // else
  //  {
   //     StartTime = 0.0;

  //  }
    StateUpdate();

    if (StartState & 0x01)  //A
    {
        StartFlxASim = StartFlxASim + INS(DelTm1) *
            (IN(VaIn1Sim) - IN(VaIn2Sim) - INS(RStartSim) * IaStSim);
        IaStSim = StartFlxASim * LStartRcpSim;           
    }
    else
    {
        StartFlxASim = 0.0;
        IaStSim = 0.0;
    }
    if (StartState & 0x02)  //B
    {
        StartFlxBSim = StartFlxBSim + INS(DelTm1) *
            (IN(VbIn1Sim) - IN(VbIn2Sim) - INS(RStartSim) * IbStSim);
        IbStSim = StartFlxBSim * LStartRcpSim;
    }
    else
    {
        StartFlxBSim = 0.0;
        IbStSim = 0.0;
    }
    if (StartState & 0x04)  //C
    {
        StartFlxCSim = StartFlxCSim + INS(DelTm1) *
            (IN(VcIn1Sim) - IN(VcIn2Sim) - INS(RStartSim) * IcStSim);
        IcStSim = StartFlxCSim * LStartRcpSim;
    }
    else
    {
        StartFlxCSim = 0.0;
        IcStSim = 0.0;
    }
   
    IaStSimZ = IaStSim;
    IbStSimZ = IbStSim;
    IcStSimZ = IcStSim;

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
void cStartUpSim::StateUpdate()
{
  if (INS(StartEnb))
    {
        StartState = 7;
        StartTime += INS(DelTm1);
        if (StartTime >= PARM(StartTmSim))
        {
            StartTime = PARM(StartTmSim);
            StartState = 0;
       }

    }
  else
  {
        StartState = 0;
        StartTime = 0.0;
  }

    if ((StartState & 0x01) && (INS(StartEnb) == 0))
    {
        if ((fabs(IaStSim)<INS(OpnCurThr)) || (IaStSimZ * IaStSim <0.0))
            StartState = StartState & 0x06;
    }

    if ((StartState & 0x02) && (INS(StartEnb) == 0))
    {
        if ((fabs(IbStSim)<INS(OpnCurThr)) || (IbStSimZ * IbStSim <0.0))
            StartState = StartState & 0x05;
    }

    if ((StartState & 0x04) && (INS(StartEnb) == 0))
    {
        if ((fabs(IcStSim)<INS(OpnCurThr)) || (IcStSimZ * IcStSim <0.0))
            StartState = StartState & 0x03;
    }
  


}

