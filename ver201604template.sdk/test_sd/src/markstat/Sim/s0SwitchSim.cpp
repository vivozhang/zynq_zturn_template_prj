///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Fuse Isim class.
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
// 2014-11-26  Bo Qu           Initial created.
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------

// system
// core
// framework
// product
#include "s0SwitchSim.h"



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
cSwitchSim::cSwitchSim
(
        InTyp    InList,
        InsTyp   InsList
)
        : In(InList), Ins(InsList)
{


    SwitchActTmr = 0.0;
    SwitchState = 0;
    OpnProcess = 0;
    ClsProcess = 0;
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
cSwitchSim::~cSwitchSim ()
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
void cSwitchSim::RunRules()
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
void cSwitchSim::SwitchModel()
{

    if(INS(SwitchForce))
    {
        SwitchActTmr = 0.0;
        SwitchState = INS(ForceSwCmd);
        OpnProcess = 0;
        ClsProcess = 0;		
    }
    else
    {
        if(IN(SwitchCmd)&&(SwitchState == 0))
        {
            if(OpnProcess ==0)
                ClsProcess = 1;		
        }
        if((IN(SwitchCmd)==0)&& SwitchState)
		{
            if(ClsProcess ==0)
                OpnProcess = 1;		
        }

        if(OpnProcess)
        {
            SwitchActTmr += INS(DelTm1);
            if(SwitchActTmr >= INS(OpnDlyTm))
            {
                SwitchActTmr = 0.0;
                SwitchState = 0;
                OpnProcess = 0;
            }
        }
		
        if(ClsProcess)
        {
            SwitchActTmr += INS(DelTm1);
            if(SwitchActTmr >= INS(ClsDlyTm))
            {
                SwitchActTmr = 0.0;
                SwitchState = 1;
                ClsProcess = 0;
            }
        }
    }

	
			


}

