///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       IPN decode routine
//
// DESCRIPTION:
//      Intelligent Part Number related decoding
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include  "x0FrameWork.h"
// product
#include "Product.h"
#include "y0IpnCod.h"


// Constants
//----------


// Externals
//----------
extern unsigned InvIPNPwrFlg;
extern unsigned TbGnPMsMcFlg;

// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(IPN_Frame,     unsigned);    // IPN frame number
CREATE_PARM(IPN_Version,   unsigned);    // IPN frame version character
CREATE_PARM(IPN_Volt,      unsigned);    // IPN volt number
CREATE_PARM(IPN_Frq,       unsigned);    // Line converter IPN line freqquncy (enumerated pick)
CREATE_PARM(L_FrqInput,    float);       // Input frequency (hz);
CREATE_PARM(SoftwareIPN,   int);         // Software IPN (enumerated pick)
CREATE_PARM(GenAead2400DwgEnb,unsigned); // Gen AEAD 2400 drawing enable
CREATE_PARM(GenAead1850DwgEnb,unsigned); // Gen AEAD 1850 drawing enable
CREATE_PARM(GenAeaaDwgEnb,   unsigned);  // Gen AEAA drawing enable
CREATE_PARM(TurbNpRatLcl,  float);       // Turbine tower power rating (local)
CREATE_PARM(L_FrqTol,      float);       // Input frequency tolerance(hz);
CREATE_PARM(MxTbPwrIPNAB,  float);       // native unit is kW
CREATE_PARM(MxTbPwrIPNCD,  float);       // native unit is kW
CREATE_PARM(TbSysPMxDl,    float);



// Published Variables
//--------------------


// Unpublished Variables
//----------------------


// Data Passing
//--------------


// Diagnostics
//------------
CREATE_DIAG(InvIPN);                // Invalid IPN
CREATE_DIAG(InvIPNPwr);                // Invalid IPN Power


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Decode Intelligent Part Number for common application code.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void X_ProcessIPN( void )
{


    if ( PARM(SoftwareIPN) == DFIG02_1500_5060_A )         // DFIGe-1500-5060-A
    {
       SET_FV(IPN_Frame,   1500);
       SET_FV(IPN_Volt,     690);
       SET_FV(IPN_Version,  IPN_VERS_A);
       PARM(GenAead2400DwgEnb) = false;
       PARM(GenAead1850DwgEnb) = false;
       PARM(GenAeaaDwgEnb)     = true;
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1500_5060_B )    // DFIGe-1500-5060-B
    {
       SET_FV(IPN_Frame,   1500);
       SET_FV(IPN_Volt,     690);
       SET_FV(IPN_Version,  IPN_VERS_B);
       PARM(GenAead2400DwgEnb) = false;
       PARM(GenAead1850DwgEnb) = false;
       PARM(GenAeaaDwgEnb)     = true;
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1850_5060_C )    // DFIGe-1500-5060-C
    {
       SET_FV(IPN_Frame,   1850);
       SET_FV(IPN_Volt,     690);
       SET_FV(IPN_Version,  IPN_VERS_C);
       PARM(GenAead2400DwgEnb) = false;
       PARM(GenAead1850DwgEnb) = false;
       PARM(GenAeaaDwgEnb)     = true;
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1850_5060_D )    // DFIGe-1500-5060-D
    {
       SET_FV(IPN_Frame,   1850);
       SET_FV(IPN_Volt,     690);
       SET_FV(IPN_Version,  IPN_VERS_D);
       PARM(GenAead2400DwgEnb) = false;
       PARM(GenAead1850DwgEnb) = false;
       PARM(GenAeaaDwgEnb)     = true;
    }
    else if ( PARM(SoftwareIPN) == DFIG02_1850_5060_E )    // DFIGe-1500-5060-D
    {
       SET_FV(IPN_Frame,   1850);
       SET_FV(IPN_Volt,     690);
       SET_FV(IPN_Version,  IPN_VERS_E);
       PARM(GenAead2400DwgEnb) = false;
       PARM(GenAead1850DwgEnb) = true;
       PARM(GenAeaaDwgEnb)     = false;
    }
    else if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_A )   //  DFIG02-2300-5060-A
    {
       SET_FV(IPN_Frame,   2300);
       SET_FV(IPN_Volt,     690);
       SET_FV(IPN_Version,  IPN_VERS_A);
       PARM(GenAead2400DwgEnb) = true;
       PARM(GenAead1850DwgEnb) = false;
       PARM(GenAeaaDwgEnb)     = false;
    }
    else
    {
        PUSH_DIAG(InvIPN);
        PARM(GenAead2400DwgEnb) = true;
        PARM(GenAead1850DwgEnb) = false;
        PARM(GenAeaaDwgEnb)     = false;
    }

    if ( PARM(L_FrqInput) == 60)       // 60 Hz grid
    {
        SET_FV(IPN_Frq, 60);
    }
    else if ( PARM(L_FrqInput) == 50)  // 50 Hz grid
    {
        SET_FV(IPN_Frq, 50);
    }
    else
    {
        PUSH_DIAG(InvIPN);
    }

    SET_FV(PwrDnPs24vTm, 0.100);
    SET_FV(PwrUpPs24vTm, 1.0);

    SET_FV(WBSpeed, 1.055556F * PARM(R_RPMNom));   //rpm
    SET_FV(WBTemp, 45.0);   //rpm

    SET_FV(TempVcoScl, 6.25e-6);//5volt/800,000hz
    SET_FV(TempVcoZero, 100.0e3);
    SET_FV(AeaaAinZero, 1.0e6);//1mhz carrier for AEAA

    SET_FV(MxTbPwrIPNAB, 1.70e3);
    SET_FV(MxTbPwrIPNCD, 1.90e3);

    SET_FV(TbSysPMxDl, 200.001e3);

    InvIPNPwrFlg = TbGnPMsMcFlg = false;

    if ( PARM(TestMode) == PUMP_BACK )
    {
        SET_FV(TempDlyTm, 0.02F);
    }
    else
    {
        SET_FV(TempDlyTm, 60.0F);
    }

    if ( ((PARM(IPN_Version) < IPN_VERS_C) && (PARM(IPN_Frame) < 2300))  && (PARM(TurbineNpRat) > PARM(MxTbPwrIPNAB)) )
    {
      InvIPNPwrFlg = true;
    }


    if ( ( (PARM(IPN_Version) == IPN_VERS_C) || (PARM(IPN_Version) == IPN_VERS_D) || (PARM(IPN_Version) == IPN_VERS_E))  && (PARM(TurbineNpRat) > PARM(MxTbPwrIPNCD)) )
    {
      InvIPNPwrFlg = true;
    }


    if ( ( ( PARM(SoftwareIPN) >= DFIG02_2400_5060_A ) || ( PARM(SoftwareIPN) >= DFIG02_2400_5060_B ) )  && (PARM(TurbineNpRat) > PARM(MaxTrbPwrIPN) ) )
    {
      InvIPNPwrFlg = true;
    }



    // TurbineNpRat is in kW while S_Power is in Watts.  Thus multiply TurbineNpRat by 1.0e3
    if ( (((PARM(TurbineNpRat) * 1.0e3) - PARM(S_Power)) >= PARM(TbSysPMxDl)) && (PARM(TestMode) != PUMP_BACK))
    {
      TbGnPMsMcFlg = true;
    }

    return;

}   // X_ProcessIPN()
