///////////////////////////////////////////////////////////////////////////////
//
// TITLE: Rotor-side Bridge-Crowbar Control
//
// DESCRIPTION:
// Defines the member functions for the Rotor-side Bridge-Crowbar Control class
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      c1BrgCroBarCtl.vsd
//
///////////////////////////////////////////////////////////////////////////////



// Include Files
//--------------
// system
#include <math.h>
// framework
// product
#include "c1BrgCroBarCtl.h"

// Constants
//----------

// Externals
//----------
extern FaultIsActiveTyp  FaultIsActive;

// Types and Classes
//------------------

// Parameters
//-----------

// Diagnostics
//------------
CREATE_DIAG(R_CrowBarFlt);      // Crow Bar fault

// Variables
//----------

//CREATE_PUBVAR(Ver_BrgCroBarCtl, WORD) = 0xMMmmpp;
//Class Version Code: MM=Major,mm=Minor,pp=Patch
//==================================================
CREATE_PUBVAR(Ver_BrgCroBarCtl, WORD) = 0x010400;
// v01.04.00: Stretch IocMask high-time per FV.CroBarIocMskTm
// v01.03.00: Del v01.02.02 Mod for WindDFIGe consistency;
//            Identify Jan2, 2013 version
// v01.02.02: Add Flt=true & State=FAULT to CROWBAR_OFF
// v01.02.01: Add CroBarOpnSync=false to CROWBAR_OFF
// v01.02.00: Add/change IN(FaultCond) & IN(ForceFault);
//            Add FaultIsActive.R_CrowBarFlt
// v01.01.00: Thresholds,IN(BrgIocFlt),MskDesat logic
// v01.00.00: First Prototype
//===============================================

// Unpublished Variables
//----------------------

// Local Prototypes (to resolve forward references)
//-------------------------------------------------

// Static Class Members (init'd to zero by default)
//-------------------------------------------------
const float        cBrgCroBarCtl::CroBar_VdcMin= 100.0;
const float        cBrgCroBarCtl::CroBar_VdcMax=1500.0;

// Member Functions
//-----------------


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class Constructor
//
// EXECUTION LEVEL
//      Class instancing
//
/////////////////////////////////////////////////////////////////////////////
cBrgCroBarCtl::cBrgCroBarCtl
(
    BrgCroBarCtlInTyp   InList,
    BrgCroBarCtlInsTyp  InsList
)
    : In(InList), Ins(InsList)
{
    //-------------------------------------------------------------------------
    // output signals
    OUT(CroBarSt)      = CROWBAR_OFF;
    OUT(CroBarVdc)     = CroBar_VdcMin;
    OUT(CroBarAct)     = false;
    OUT(CroBarUpDn)    = false;
    OUT(CroBarMskIoc)  = false;
    OUT(CroBarMskDsat) = false;
    OUT(CroBarFlt)     = false;
    OUT(CroBarOpnSync) = false;
    OUT(CroBarPE)      = false;
    OUT(CroBarRst)     = false;

    //-------------------------------------------------------------------------
    // published internal signals
    CroBarVdcLvlOn     = CroBar_VdcMax;
    CroBarVdcLvlOff    = CroBar_VdcMax;
    CroBarTmr          = 0.0;
    CroBarTmr2         = 0.0;
    CroBarImagSqrFil   = 0.0F;
    //-------------------------------------------------------------------------
    // unpublished internal signal
    CroBarVdcFbkZ1     = CroBar_VdcMin;
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Class destructor to free used resources.
//
// EXECUTION LEVEL
//      Instance destruction
//
/////////////////////////////////////////////////////////////////////////////
cBrgCroBarCtl::~cBrgCroBarCtl(void)
{
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Set all parameters owned by the class.
//
// EXECUTION LEVEL
//      Executed during initialization and when required by external code.
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
int cBrgCroBarCtl::RunRules(void)
{
    int     Status = SUCCESS;

    SET_FV(CroBarVdcFGn0 ,       0.5  );  // R_Vdc FIR filter gain for crowbar
    SET_FV(CroBarVdcFGn1 ,       0.5  );  // R_Vdc FIR filter gain for crowbar


    if(INS(DbEnb))
    {
        SET_FV(CroBarVdcLvlOnNrm, (INS(VdcDbMax) +  150.0F) ); // 1300/1350
        SET_FV(CroBarVdcLvlOfNrm, (INS(VdcDbMax) +   50.0F) ); // 1200/1250
    }
    else
    {
        SET_FV(CroBarVdcLvlOnNrm, (INS(VdcRefMax) + 105.0F) ); // 1230
        SET_FV(CroBarVdcLvlOfNrm, (INS(VdcRefMax) +  25.0F) ); // 1150
    }

    SET_FV(CroBarVdcLvlFault, INS(VdcOvLvl));// Vdc Level CroBar FAULT  (Volts)

    SET_FV(CroBarVdcLvlOnHt, (INS(VdcRefMax) + 105.0F) ); // 1230/1280
    SET_FV(CroBarVdcLvlOfHt, (INS(VdcRefMax) +  25.0F) ); // 1150/1200

    SET_FV(CroBarVdcLvlOnGF, (INS(VdcOvLvl)  -  25.0F) ); // 1325/1375
    SET_FV(CroBarVdcLvlOfGF, (INS(VdcOvLvl)  -  75.0F) ); // 1275/1325

    SET_FV(CroBarVdcLvlOnLv, (INS(VdcRefMax) +  25.0F) ); // 1150/1200
    SET_FV(CroBarVdcLvlOfLv, (INS(VdcRefMax) +   5.0F) ); // 1130/1180

    SET_FV(CroBarPer,            0.020);    // R_Vdc crowbar toggle period    (sec)
    SET_FV(CroBarTm,             0.200);    // R_Vdc crowbar action duration  (sec)
    SET_FV(CroBarLkTm,           1.0  );    // R_Vdc crowbar cool-down time   (sec)

    SET_FV(CroBarIocMskDyEn, true);
    SET_FV(CroBarIocMskMgDc,    10.0F);
    SET_FV(CroBarIocMskVdc, INS(VdcDbMax) -PARM(CroBarIocMskMgDc));                    // Just below DB
    SET_FV(CroBarIocMskVac, INS(VllrmsNom)*(SQRT2*SQRT3_RCP)*0.75);   // 60% voltage

    SET_FV(CroBarIocMskTm,       0.020);  // IOC Mask duration (sec)

    SET_FV(CroBarImagFilW,   100.0F);
    CroBarImagFilGn = PARM(CroBarImagFilW) * INS(DelTm1);
    SET_FV(CroBarImagOfThr, 3500.0F);
    CroBarImagOffSq = PARM(CroBarImagOfThr) * PARM(CroBarImagOfThr);

    return (Status);
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This is the PRIMARY RUN-TIME Method for this Class
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
void cBrgCroBarCtl::Task1(void)
{
    //---------------------------------------------
    // Dynamic crowbar on/off thesholds
    if ( (static_cast<CroBarThrSelTyp> (IN(SelVdcLvl))) == CROBAR_THR_GRDFLT )
    {
        OUT(CroBarVdcLvlOn)  = PARM(CroBarVdcLvlOnGF);
        OUT(CroBarVdcLvlOff) = PARM(CroBarVdcLvlOfGF);
    }
    else if ( (static_cast<CroBarThrSelTyp> (IN(SelVdcLvl))) == CROBAR_THR_LVRT )
    {
        OUT(CroBarVdcLvlOn)  = PARM(CroBarVdcLvlOnLv);
        OUT(CroBarVdcLvlOff) = PARM(CroBarVdcLvlOfLv);
    }
    else if((static_cast<CroBarThrSelTyp> (IN(SelVdcLvl))) == CROBAR_THR_DBHOT)
    {
        OUT(CroBarVdcLvlOn)  = PARM(CroBarVdcLvlOnHt);
        OUT(CroBarVdcLvlOff) = PARM(CroBarVdcLvlOfHt);
    }
    else
    {
        OUT(CroBarVdcLvlOn)  = PARM(CroBarVdcLvlOnNrm);
        OUT(CroBarVdcLvlOff) = PARM(CroBarVdcLvlOfNrm);
    }

    //---------------------------------------------
    // Filter Vdc for crowbar control
    OUT(CroBarVdc) = IN(VdcFbk)      *PARM(CroBarVdcFGn0)
                     + CroBarVdcFbkZ1*PARM(CroBarVdcFGn1);
    CroBarVdcFbkZ1 = IN(VdcFbk);

    //---------------------------------------------
    // Conditionally mask FPGA IOC fault
    if(PARM(CroBarIocMskDyEn))
    {
        CroBarIocMskVdc = IN(VdcRegRef) + PARM(CroBarIocMskMgDc);
    }
    else
    {
        CroBarIocMskVdc = PARM(CroBarIocMskVdc);
    }

    if (  (  (CroBarVdc  > CroBarIocMskVdc) && (IN(VacFbk) < PARM(CroBarIocMskVac)) )
       || CroBarAct || IN(BrgIocFlt) )
    {
        CroBarIocMaskTmr  = 0.0;
        OUT(CroBarMskIoc) = TRUE;
    }
    else
    {
        CroBarIocMaskTmr += INS(DelTm1);
        if (CroBarIocMaskTmr >= PARM(CroBarIocMskTm))
        {
            CroBarIocMaskTmr  = PARM(CroBarIocMskTm);
            OUT(CroBarMskIoc) = FALSE;
        }
    }

    //---------------------------------------------
    // Conditionally mask FPGA DESAT fault
    if ( !CroBarMskDsat )
    {
        if ( CroBarAct )
        {
            OUT(CroBarMskDsat) = TRUE;
        }
    }
    else
    {
        if ( !CroBarAct && !IN(BrgIocFlt) )
        {
            OUT(CroBarMskDsat) = FALSE;
        }
    }


    //---------------------------------------------
    // Initiate Force behaviors
    if ( IN(ForceFault) )
    {
        OUT(CroBarSt) = CROWBAR_FAULT;
        OUT(CroBarAct) = true;                  // bypasses normal modulation
        OUT(CroBarFlt) = true;                  // bypasses normal fpga cmds
    }
    else
    {
        if ( IN(ForceOff) )
        {
            if ( CroBarSt == CROWBAR_NORMAL)
            {
                if (CroBarUpDn == DOWN) OUT(CroBarUpDn) = UP;
                else                    OUT(CroBarUpDn) = DOWN;
            }
            OUT(CroBarSt) = CROWBAR_OFF;
        }
    }

    // current magnitude squared calculation for crowbar off permissive
    CroBarImagSqrDel=IN(ImagSqr)-CroBarImagSqrFil;
    if(CroBarImagSqrDel<0.0F)
    {
        CroBarImagSqrFil += CroBarImagSqrDel * CroBarImagFilGn;
    }
    else
    {
        CroBarImagSqrFil = IN(ImagSqr);
    }
    if( CroBarImagSqrFil<CroBarImagOffSq || !IN(GmLineVLo) || IN(GmNSeqHi) )
    {
        CroBarOffPrm = true;
    }
    else
    {
        CroBarOffPrm = false;
    }


    //---------------------------------------------
    // Main State Machine
    StateMachine();

    return;
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//      Task1
//
/////////////////////////////////////////////////////////////////////////////
void  cBrgCroBarCtl::StateMachine(void)
{
    switch ( CroBarSt)
    {
        case CROWBAR_OFF :
        {
            if ( INS(TestMode)==PUMP_BACK )
            {
                // PumpBack Mode: Just check for Vdc > Fault level
                if (CroBarVdc > PARM(CroBarVdcLvlFault) )
                {
                    PUSH_DIAG(R_CrowBarFlt);    // Post Diagnostic
                    FaultIsActive.R_CrowBarFlt = true;
                    OUT(CroBarOpnSync) = true;  // Request Q1 to Open
                }
            }
            else if ( IN(Stopped) || IN(ForceOff) )
            {
                // Idle: Prepare...
                CroBarTmr  = 0.0;
                CroBarTmr2 = 0.0;
                OUT(CroBarAct)     = false;
                OUT(CroBarFlt)     = false;
                OUT(CroBarRst)     = false;
                OUT(CroBarPE)      = true;
                OUT(CroBarOpnSync) = false;
                if ( !IN(ForceOff) )
                {
                    OUT(CroBarUpDn) = DOWN;
                }

                // ...and check for Vdc > Fault level
                if (CroBarVdc > PARM(CroBarVdcLvlFault))
                {
                    PUSH_DIAG(R_CrowBarFlt);    // Post Diagnostic
                    FaultIsActive.R_CrowBarFlt = true;
                    OUT(CroBarOpnSync) = true;  // Request Sync Switch Open
                }
            }
            else if (     (CroBarVdc > PARM(CroBarVdcLvlFault) )
                     ||(  (CroBarVdc > CroBarVdcLvlOn          )
                        &&(IN(BrgFltHdw)|| IN(FaultCond)       )
                       )
                    )
            {
                // Idle: Fault detected
                OUT(CroBarSt)  = CROWBAR_FAULT;
                OUT(CroBarAct) = true;      // bypasses normal modulation
                OUT(CroBarFlt) = true;      // bypasses normal fpga cmds
                if (IN(BrgFltHdw))
                {
                    OUT(CroBarPE)  = false; // disable pwr to allow reset on fpga
                    OUT(CroBarRst) = true;  // resets fpga on next clkt1
                }
                else
                {
                    OUT(CroBarPE)  = true;
                    OUT(CroBarRst) = false;
                }
            }
            else if (CroBarVdc > CroBarVdcLvlOn)
            {
                // Idle: Turn-On detected
                OUT(CroBarSt)  = CROWBAR_NORMAL;
                OUT(CroBarAct) = true;      // bypasses normal modulation
            }
            break;
        }
        case CROWBAR_NORMAL :
        {
            CroBarTmr += INS(DelTm1);
            if (    (CroBarVdc > PARM(CroBarVdcLvlFault))
                || IN(BrgFltHdw)
                || IN(FaultCond)
               )
            {
                // Normal: Fault detected
                OUT(CroBarSt)  = CROWBAR_FAULT;
                OUT(CroBarAct) = true;      // bypasses normal modulation
                OUT(CroBarFlt) = true;      // bypasses normal fpga cmds
                if (IN(BrgFltHdw))
                {
                    OUT(CroBarPE)  = false; // disable pwr to allow reset on fpga
                    OUT(CroBarRst) = true;  // resets fpga on next clkt1
                }
                else
                {
                    OUT(CroBarPE)  = true;
                    OUT(CroBarRst) = false;
                }
            }
            else if (CroBarVdc < CroBarVdcLvlOff && CroBarOffPrm)
            {
                // Normal: Turn-Off detected
                OUT(CroBarSt)  = CROWBAR_OFF;
                OUT(CroBarAct) = false;
                if (CroBarUpDn == DOWN) OUT(CroBarUpDn) = UP;
                else                    OUT(CroBarUpDn) = DOWN;
                CroBarTmr = 0.0;
            }
            else if (CroBarTmr >= PARM(CroBarPer))
            {
                // Normal: Alternate Up/Down
                CroBarTmr = 0.0;
                if (CroBarUpDn == DOWN) OUT(CroBarUpDn) = UP;
                else                    OUT(CroBarUpDn) = DOWN;
            }
            break;
        }
        case CROWBAR_FAULT :
        {
            CroBarTmr  += INS(DelTm1);  // time on upper or lower
            CroBarTmr2 += INS(DelTm1);  // time in this mode

            PUSH_DIAG(R_CrowBarFlt);    // Post Diagnostic
            FaultIsActive.R_CrowBarFlt = true;
            OUT(CroBarOpnSync) = true;  // Request Sync Switch Open

            if (IN(BrgFltHdw))
            {
                OUT(CroBarPE)  = false; // disable pwr to allow reset on fpga
                OUT(CroBarRst) = true;  // resets fpga on next clkt1
            }
            else
            {
                OUT(CroBarPE)  = true;
                OUT(CroBarRst) = false;
            }

            // Alternate Up/Down
            if ( CroBarTmr >= PARM(CroBarPer) )
            {
                CroBarTmr = 0.0;
                if (CroBarUpDn == DOWN) OUT(CroBarUpDn) = UP;
                else                    OUT(CroBarUpDn) = DOWN;
            }

            if (CroBarTmr2 >= PARM(CroBarTm))
            {
                // Fault: Begin Cooling
                CroBarTmr2 = 0.0;
                OUT(CroBarSt) = CROWBAR_COOL;
            }
            break;
        }
        case CROWBAR_COOL :
        {
            CroBarTmr2 += INS(DelTm1);  // time in this mode

            PUSH_DIAG(R_CrowBarFlt);    // Re-Post Diagnostic (prevent reset while cooling)
            FaultIsActive.R_CrowBarFlt = true;
            OUT(CroBarOpnSync) = true;  // Request Sync Switch Open

            if (CroBarTmr2 >= PARM(CroBarLkTm))
                OUT(CroBarSt)  = CROWBAR_OFF;

            OUT(CroBarFlt) = false;     // normal control of pwr enb & fpga reset
            OUT(CroBarAct) = false;     // restore normal modulation

            break;
        }
    }
    return;
}
