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
#include "s0ShortSim.h"



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
cShortSim::cShortSim
(
        InTyp    InList,
        InsTyp   InsList
)
        : In(InList), Ins(InsList)
{
    IaShortZ = 0.0;
    IbShortZ = 0.0;
    IcShortZ = 0.0;
    IaShort = 0.0;
    IbShort = 0.0;
    IcShort = 0.0;
    ShortCmd = 0;
    ShortState = 0;
    ShortFlxASim = 0.0;
    ShortFlxBSim = 0.0;
    ShortFlxCSim = 0.0;
    ShortFlxABSim = 0.0;
    ShortFlxACSim = 0.0;
    ShortFlxBCSim = 0.0;
    VaShortSim = 0.0;
    VbShortSim = 0.0;
    VcShortSim = 0.0;
 
    
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
cShortSim::~cShortSim ()
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
void cShortSim::RunRules()
{

   
     LShortllRcpSim = NoNANLim(1.0/INS(LShortllSim),1.0e8F, -1.0e8F);
     LShortRcpSim = NoNANLim(1.0/INS(LShortSim),1.0e8F, -1.0e8F);
     

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
void cShortSim::StateUpdate()
{
    if(IN(ShortEnb))
    {
        ShortCmd = INS(ShortCmdIn);
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
        ShortCmd = 0;
    }

    
    ShortState = ShortState |ShortCmd;

    if(((ShortState&0x01)) && ((ShortCmd&0x01)==0))
    {
        if((fabs(IaShort)<INS(OpnCurThrs)) || (IaShort * IaShortZ <0.0))
            ShortState = ShortState &0x06;
    }

    if(((ShortState&0x02)) && ((ShortCmd&0x02)==0))
    {
        if((fabs(IbShort)<INS(OpnCurThrs))|| (IbShort * IbShortZ <0.0))
            ShortState = ShortState &0x05;
    }

    if(((ShortState&0x04)) && ((ShortCmd&0x04)==0))
    {
        if((fabs(IcShort)<INS(OpnCurThrs)) || (IcShort * IcShortZ <0.0))
            ShortState = ShortState &0x03;
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
void cShortSim::ShortNotRun()
{

    if((ShortState&0x01)==0) 
    {
        ShortFlxASim = 0.0;
        ShortFlxABSim = 0.0;
        ShortFlxACSim = 0.0;
        IaShort = 0.0;
    }
    if((ShortState&0x02)==0) 
    {
        ShortFlxBSim = 0.0;
        ShortFlxABSim = 0.0;
        ShortFlxBCSim = 0.0;
        IbShort = 0.0;
    }
    if((ShortState&0x04)==0) 
    {
        ShortFlxCSim = 0.0;
        ShortFlxBCSim = 0.0;
        ShortFlxACSim = 0.0;
        IcShort = 0.0;
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
void cShortSim::ShortRun()
{

 //   StateUpdate();
    
///////////////////////////////////////////////////////////////////////

    IaShortZ = IaShort;
    IbShortZ = IbShort;
    IcShortZ = IcShort;

    VaShortSim = IN(VaShortInSim);
    VbShortSim = IN(VbShortInSim);
    VcShortSim = IN(VcShortInSim);

    if(INS(DirectShort))
    {
         if (INS(LLShort))
        {
            if ((ShortState & 0x01) && (ShortState & 0x02)) //AB
                VaShortSim = VbShortSim = (IN(VaInSim) + IN(VbInSim)) / 2.0;
            else if ((ShortState & 0x01) && (ShortState & 0x04)) //AC
                    VaShortSim = VcShortSim = (IN(VaInSim) + IN(VcInSim)) / 2.0;
            else if ((ShortState & 0x02) && (ShortState & 0x04)) //BC
                    VbShortSim = VcShortSim = (IN(VbInSim) + IN(VcInSim)) / 2.0;
        }
        else
        {
            if(ShortState&0x01)
                VaShortSim = 0.0;
            if(ShortState&0x02)
                VbShortSim = 0.0;
            if(ShortState&0x04)
                VcShortSim = 0.0;
        }
            

    }
    else
    {


        if (INS(LLShort))
        {
            if ((ShortState & 0x01) && (ShortState & 0x02)) //AB
            {
                ShortFlxABSim = ShortFlxABSim + INS(DelTm1) *
                    (VaShortSim - VbShortSim - INS(RShortllSim) * IaShort);
                IaShort = ShortFlxABSim * LShortllRcpSim;
                IbShort = -IaShort;
            }
            else if ((ShortState & 0x02) && (ShortState & 0x04)) //BC
            {
                ShortFlxBCSim = ShortFlxBCSim + INS(DelTm1) *
                    (VbShortSim - VcShortSim - INS(RShortllSim) * IbShort);
                IbShort = ShortFlxBCSim * LShortllRcpSim;
                IcShort = -IbShort;
            }
            else if ((ShortState & 0x01) && (ShortState & 0x04)) //AC
            {
                ShortFlxACSim = ShortFlxACSim + INS(DelTm1) *
                    (VaShortSim - VcShortSim - INS(RShortllSim) * IaShort);
                IaShort = ShortFlxACSim * LShortllRcpSim;
                IcShort = -IaShort;
            }
            
        }
        else 
        {
            if(ShortState&0x01)
            {
                ShortFlxASim = ShortFlxASim + INS(DelTm1) *
                        (VaShortSim - INS(RShortSim) * IaShort);
                IaShort = ShortFlxASim * LShortRcpSim;
            }
            if(ShortState&0x02)
            {
                ShortFlxBSim = ShortFlxBSim + INS(DelTm1) *
                        (VbShortSim - INS(RShortSim) * IbShort);
                IbShort = ShortFlxBSim * LShortRcpSim;
            }
            if(ShortState&0x04)
            {
                ShortFlxCSim = ShortFlxCSim + INS(DelTm1) *
                        (VcShortSim - INS(RShortSim) * IcShort);
                IcShort = ShortFlxCSim * LShortRcpSim;
        
            }
            
        }

    }   
   



}


