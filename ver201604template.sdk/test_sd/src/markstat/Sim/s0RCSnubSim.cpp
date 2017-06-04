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
#include "s0RCSnubSim.h"



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
cRCSnubSim::cRCSnubSim
(
        InTyp    InList,
        InsTyp   InsList
)
        : In(InList), Ins(InsList)
{
    QxRCSim = 0.0;
    QyRCSim = 0.0;
    VxRCCapSim = 0.0;
    VyRCCapSim = 0.0;
    VxRCSim = 0.0;
    VyRCSim = 0.0;
        VaRCCapSim = 0.0;
    VbRCCapSim = 0.0;
    VcRCCapSim = 0.0;
    VaRCSim = 0.0;
    VbRCSim = 0.0;
    VcRCSim = 0.0;
    
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
cRCSnubSim::~cRCSnubSim ()
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
void cRCSnubSim::RunRules()
{
    LCrcRcpSim = 1.0 / INS(CrcSim);
    RrcRcpSim = 1.0 / INS(RrcSim);


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
void cRCSnubSim::RCWork()
{

    // Voltage at RC snubber caps
    QxRCSim  = QxRCSim + INS(DelTm1) * (IN(IxRCSim) + (QyRCSim * IN(WElecSim)));
    VxRCCapSim = QxRCSim * LCrcRcpSim;

    QyRCSim  = QyRCSim + INS(DelTm1) * (IN(IyRCSim) - (QxRCSim * IN(WElecSim)));
    VyRCCapSim = QyRCSim * LCrcRcpSim;

         
    VxRCSim = VxRCCapSim + IN(IxRCSim) * INS(RrcSim);
    VyRCSim = VyRCCapSim + IN(IyRCSim) * INS(RrcSim);

}

void cRCSnubSim::RCWorkABC()
{

    // Voltage at RC snubber caps
    if(IN(PhaseState)&0x01) 
    {
        VaRCCapSim = VaRCCapSim + IN(IaRCSim) * INS(DelTm1) * LCrcRcpSim;
        VaRCSim = VaRCCapSim + IN(IaRCSim) * INS(RrcSim);
    }
    
    if(IN(PhaseState)&0x02) 
    {
        VbRCCapSim = VbRCCapSim + IN(IbRCSim) * INS(DelTm1) * LCrcRcpSim;
        VbRCSim = VbRCCapSim + IN(IbRCSim) * INS(RrcSim);
    }
   

    if(IN(PhaseState)&0x04) 
    {
        VcRCCapSim = VcRCCapSim + IN(IcRCSim) * INS(DelTm1) * LCrcRcpSim;
        VcRCSim = VcRCCapSim + IN(IcRCSim) * INS(RrcSim);
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
void cRCSnubSim::RCNoWork()
{

    QxRCSim = INS(CrcSim) * IN(VxRCInSim);
    QyRCSim = INS(CrcSim) * IN(VyRCInSim);


}


void cRCSnubSim::RCNoWorkABC()
{
    if((IN(PhaseState)&0x01)==0) 
        VaRCSim = VaRCCapSim = IN(VaRCInSim);
    if((IN(PhaseState)&0x02)==0) 
        VbRCSim = VbRCCapSim = IN(VbRCInSim);
    if((IN(PhaseState)&0x04)==0) 
        VcRCSim = VcRCCapSim = IN(VcRCInSim);

}

