///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Internal Simulator Primary Interface
//
// DESCRIPTION:
//      This file contains the functions that implement the internal simulator
//      interfaces
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      s0ISim.vsd
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
#include <sys/neutrino.h>
#include <math.h>
// framework
#include "x0BridgeInterface.h"
// product
#include "s0BrgSim.h"


// Constants
//----------


// Externals
//----------


// Types and Classes
//------------------


// Parameters
//-----------


// Published Variables
//--------------------


// Unpublished Variables
//----------------------


// Static Class Members (init'd to zero by default)
//-------------------------------------------------

unsigned       cBrgSim::LlmBrgSelect = 0;
unsigned       cBrgSim::LlmForceTable[cBrgSim::NUM_INST_SUPPORT][9] =
{
// Status signals   PwrOk   CI09   CI10   Brg1_IocPhsA   Brg1_DsatPhsApos   Brg1_DsatPhsAneg   Brg2_IocPhsA   Brg2_DsatPhsApos   Brg2_DsatPhsAneg
   /*Rotor*/    {   true,   false, false, false,         false,             false,             false,         false,             false           },
   /*Line */    {   true,   false, false, false,         false,             false,             false,         false,             false           }
};


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------





///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
cBrgSim::cBrgSim (void)
{
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
// EXECUTION LEVEL
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
cBrgSim::~cBrgSim(void)
{
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Translate signal average value, over a PWM frame, into Vco counts.
//
// EXECUTION LEVEL
//
// RETURN VALUE
//      SigVcoCnt = new value of VCO counter
//
///////////////////////////////////////////////////////////////////////////////
unsigned cBrgSim::Avg2Vco
(
    float               SigAvg,         // in:      signal average value over the frame
    unsigned&           SigVcoCnt,      // in/out:  new value of Vco counter (integer, no fraction)
    float&              VcoCntRes       // in/out:  fractional value of Vco count, held for residual accumulation
)
{
    float               VcoFreq;        // voltage-controlled oscillator output frequency
    float               VcoDelCnt;      // movement of the vco counter over the frame (including fractional)

    // calc the Vco output freq
    VcoFreq = (SigAvg * VcoGainSim) + VcoFzroSim;

    // apply limits
    if ( VcoFreq > VcoFmaxSim ) VcoFreq = VcoFmaxSim;
    if ( VcoFreq < VcoFminSim ) VcoFreq = VcoFminSim;

    // calc movement of the Vco counter over the frame (including fractional residualized from previous calc)
    VcoDelCnt = (VcoFreq * VcoDelTmSim) + VcoCntRes;

    // residualize the fraction part for the next calc
    VcoCntRes = VcoDelCnt - (int)VcoDelCnt;

    // calc the movement of Vco counter over the frame (excluding fractional)
    return SigVcoCnt = (SigVcoCnt + (int)VcoDelCnt) & VcoMaskSim;;
}

///////////////////////////////////////////////////////////////////////////////
unsigned cBrgSim::Avg2Enc
(
    float               SpdAvg,         // in:      Speed average value over the frame (called in units of rad/sec)
    unsigned&           SpdEncCnt,      // in/out:  new value of Speed counter (integer, no fraction)
    float&              EncCntRes       // in/out:  fractional value of Speed count, held for residual accumulation
)
{
    float               EncFreq;        // Encoder output frequency (units?)
    float               EncDelCnt;      // movement of the Encoder over the frame (including fractional)

    // calc the Enc output freq
    EncFreq   = (SpdAvg  * EncGainSim);

   // calc movement of the Encoder counter over the frame (including fractional residualized from previous calc)
    EncDelCnt = (EncFreq * EncDelTmSim) + EncCntRes;

    // residualize the fraction part for the next calc
    EncCntRes = EncDelCnt - (int)EncDelCnt;

    // calc the movement of Encoder counter over the frame (excluding fractional)
    return SpdEncCnt = (SpdEncCnt + (int)EncDelCnt) & EncMaskSim;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculate average phase voltage over the PWM frame
//      from commanded bridge phase transitions & DC link voltage
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      Average phase voltage over the PWM frame
//
/////////////////////////////////////////////////////////////////////////////
float cBrgSim::SimPhaseVoltAvg
(
    PhsTransListTyp& PhaseTraList,      // in:  list of info about phase transitions
    float           Vdc,                // in:  dc-link voltage magnitude
    unsigned        Phase,              // in:  phase being calculated
    unsigned        FullTm,             // in:  full frame frc counts
    float           FullTmRcp           // in:  reciprocal of full frame frc counts
)
{
    unsigned        ThisTm;             // time (frc counts) for "this" transition
    unsigned        NextTm;             // time (frc counts) for "next" transition
    int             SumTm = 0;          // sum of all time (frc counts) at PHS_ST_POS state

    unsigned        tra;                // index for "this" transition
    unsigned        tranxt;             // index for "next" transition

    //-------------------------------------------------------------------------
    // for each transition on this phase
    for ( tra = 0 ; tra < PhaseTraList.Len ; ++tra )
    {
        // test to see if we are done before getting to the last transition in the list
        if ( ((PhaseTraList.pTrans)[tra][Phase].Enable == false) ||             // this transition is not enabled
                ((PhaseTraList.pTrans)[tra][Phase].DelTm > FullTm) )            // this transition is beyond end-of-frame
        {
            break;  // exit the "for" loop
        }

        // time for this transition
        ThisTm = (PhaseTraList.pTrans)[tra][Phase].DelTm;

        // time for next transition
        tranxt = tra + 1;
        if ( (tranxt >= PhaseTraList.Len) ||                                    // next transition does not exist
                ((PhaseTraList.pTrans)[tranxt][Phase].Enable == false) ||       // next transition is not enabled
                ((PhaseTraList.pTrans)[tranxt][Phase].DelTm > FullTm) )         // next transition is beyond end-of-frame
        {
            NextTm = FullTm;                                                    // set next time to end-of-frame time
        }
        else
        {
            NextTm = (PhaseTraList.pTrans)[tranxt][Phase].DelTm;                // set next time to next trans time
        }

        // if the phase state is connected to positive DC bus then integrate the interval time from "this" transition to "next"
        if ( (PhaseTraList.pTrans)[tra][Phase].State == PHS_ST_POS )
        {
            SumTm += (int)( NextTm - ThisTm );
        }
    }

    //-------------------------------------------------------------------------
    // average phase voltage is DC bus voltage scaled by the percentage of frame time
    // that the phase was connected to the positive DC bus
    return Vdc * (float)SumTm * FullTmRcp;
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Calculate pwm pulse turn up and down time
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
/////////////////////////////////////////////////////////////////////////////
void cBrgSim::PhasePwmUpDnTm               //CL, 2015-01-22
(
    PhsTransListTyp& PhaseTraList,      // in:  list of info about phase transitions
    unsigned        Phase,              // in:  phase being calculated
    unsigned        FullTm             // in:  full frame frc counts
)
{
    unsigned        ThisTm;             // time (frc counts) for "this" transition
    unsigned        NextTm;             // time (frc counts) for "next" transition
//    int             SumTm = 0;          // sum of all time (frc counts) at PHS_ST_POS state

    unsigned        tra;                // index for "this" transition
    unsigned        tranxt;             // index for "next" transition

    
    PwmUpTm = FullTm;
    PwmDnTm = FullTm;
    //-------------------------------------------------------------------------
    // for each transition on this phase
    for ( tra = 0 ; tra < PhaseTraList.Len ; ++tra )
    {
        // test to see if we are done before getting to the last transition in the list
        if ( ((PhaseTraList.pTrans)[tra][Phase].Enable == false) ||             // this transition is not enabled
                ((PhaseTraList.pTrans)[tra][Phase].DelTm > FullTm) )            // this transition is beyond end-of-frame
        {
            break;  // exit the "for" loop
        }

        // time for this transition
        ThisTm = (PhaseTraList.pTrans)[tra][Phase].DelTm;

        // time for next transition
        tranxt = tra + 1;
        if ( (tranxt >= PhaseTraList.Len) ||                                    // next transition does not exist
                ((PhaseTraList.pTrans)[tranxt][Phase].Enable == false) ||       // next transition is not enabled
                ((PhaseTraList.pTrans)[tranxt][Phase].DelTm > FullTm) )         // next transition is beyond end-of-frame
        {
            NextTm = FullTm;                                                    // set next time to end-of-frame time
        }
        else
        {
            NextTm = (PhaseTraList.pTrans)[tranxt][Phase].DelTm;                // set next time to next trans time
        }

        // if the phase state is connected to positive DC bus then integrate the interval time from "this" transition to "next"
        if ( (PhaseTraList.pTrans)[tra][Phase].State == PHS_ST_POS )
        {
            PwmUpTm = ThisTm;
            PwmDnTm = NextTm;
            //SumTm += (int)( NextTm - ThisTm );
        }
    }

    //-------------------------------------------------------------------------
    // average phase voltage is DC bus voltage scaled by the percentage of frame time
    // that the phase was connected to the positive DC bus
   // return Vdc * (float)SumTm * FullTmRcp;
}


/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    Fill low level monitor buffer with information about the target
//    bridge interface.
//
// EXECUTION LEVEL
//    Initialization.
//
// RETURN VALUE
//      None
//
/////////////////////////////////////////////////////////////////////////////
int cBrgSim::LlmForceBrgFlt(cParamString& OutputBuffer, cParamString& CommandLineArgs)
{
    int       CurrentOffset;
    unsigned  Idx;

    //-------------------------------------------------------------------------
    // status
    CurrentOffset = CommandLineArgs.find("-s");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("Selected bridge-interface is %s\n",  (LlmBrgSelect==0) ? "Rotor Brg1" : (LlmBrgSelect==1) ?
                             "Rotor Brg2" : (LlmBrgSelect==2) ? "Line Brg1" : "Line Brg2");
        for ( Idx = 0; Idx < 2; Idx++)
        {
            OutputBuffer.pappend("-- %s Interface --\n", (Idx==0) ? "Rotor" : "Line");
            OutputBuffer.pappend("  InPwrOk = %s\n", (LlmForceTable[Idx][0]==false)         ? "forced bad" : "ok" );
            OutputBuffer.pappend("  CntInp09 = %s\n", (LlmForceTable[Idx][1]==true)         ? "forced bad" : "ok" );
            OutputBuffer.pappend("  CntInp10 = %s\n", (LlmForceTable[Idx][2]==true)         ? "forced bad" : "ok" );
            OutputBuffer.pappend("  Brg1-IocPhsA = %s\n", (LlmForceTable[Idx][3]==true)     ? "forced bad" : "ok" );
            OutputBuffer.pappend("  Brg1-DsatPhsApos = %s\n", (LlmForceTable[Idx][4]==true) ? "forced bad" : "ok" );
            OutputBuffer.pappend("  Brg1-DsatPhsAneg = %s\n", (LlmForceTable[Idx][5]==true) ? "forced bad" : "ok" );
            OutputBuffer.pappend("  Brg2-IocPhsA = %s\n", (LlmForceTable[Idx][6]==true)     ? "forced bad" : "ok" );
            OutputBuffer.pappend("  Brg2-DsatPhsApos = %s\n", (LlmForceTable[Idx][7]==true) ? "forced bad" : "ok" );
            OutputBuffer.pappend("  Brg2-DsatPhsAneg = %s\n", (LlmForceTable[Idx][8]==true) ? "forced bad" : "ok" );
        }

        return EOK;
    }

    //-------------------------------------------------------------------------
    // bridge interface select
    CurrentOffset = CommandLineArgs.find("-bi");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        ( (LlmBrgSelect+1) < 4 ) ? LlmBrgSelect++ : LlmBrgSelect=0;

        OutputBuffer.pappend("Selected bridge-interface is %s\n",  (LlmBrgSelect==0) ? "Rotor Brg1" : (LlmBrgSelect==1) ?
                             "Rotor Brg2" : (LlmBrgSelect==2) ? "Line Brg1" : "Line Brg2");

        return EOK;
    }

    //-------------------------------------------------------------------------
    // inpwrok
    CurrentOffset = CommandLineArgs.find("-inpwrok");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("%s PowerOk", (LlmBrgSelect<2) ? "Rotor Brg1 & Brg2" : "Line Brg1 & Brg2");

        CurrentOffset = CommandLineArgs.find("-inpwrokx");
        if ( CurrentOffset != CommandLineArgs.npos )
        {
            OutputBuffer.pappend(" -> ok\n");
            switch ( LlmBrgSelect )
            {
                case 0:
                case 1:
                    LlmForceTable[0][0] = true;
                    break;
                case 2:
                case 3:
                    LlmForceTable[1][0] = true;
                    break;
            }
        }
        else
        {
            OutputBuffer.pappend(" -> forced bad\n");
            switch ( LlmBrgSelect )
            {
                case 0:
                case 1:
                    LlmForceTable[0][0] = false;
                    break;
                case 2:
                case 3:
                    LlmForceTable[1][0] = false;
                    break;
            }
        }

        return EOK;
    }

    //-------------------------------------------------------------------------
    // ioc phase a
    CurrentOffset = CommandLineArgs.find("-ioca");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        OutputBuffer.pappend("%s ioc phase a", (LlmBrgSelect==0) ? "Rotor Brg1" : (LlmBrgSelect==1) ?
                             "Rotor Brg2" : (LlmBrgSelect==2) ? "Line Brg1" : "Line Brg2");

        CurrentOffset = CommandLineArgs.find("-iocax");
        if ( CurrentOffset != CommandLineArgs.npos )
        {
            OutputBuffer.pappend(" -> ok\n");
            switch ( LlmBrgSelect )
            {
                case 0:
                    LlmForceTable[0][3] = false;
                    break;
                case 1:
                    LlmForceTable[0][6] = false;
                    break;
                case 2:
                    LlmForceTable[1][3] = false;
                    break;
                case 3:
                    LlmForceTable[1][6] = false;
                    break;
            }
        }
        else
        {
            OutputBuffer.pappend(" -> forced bad\n");
            switch ( LlmBrgSelect )
            {
                case 0:
                    LlmForceTable[0][3] = true;
                    break;
                case 1:
                    LlmForceTable[0][6] = true;
                    break;
                case 2:
                    LlmForceTable[1][3] = true;
                    break;
                case 3:
                    LlmForceTable[1][6] = true;
                    break;
            }
        }

        return EOK;
    }

    //-------------------------------------------------------------------------
    // dsat phase a
    CurrentOffset = CommandLineArgs.find("-dsata");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        // positive rail
        CurrentOffset = CommandLineArgs.find("-dsatap");
        if ( CurrentOffset != CommandLineArgs.npos )
        {
            OutputBuffer.pappend("%s dsat phase a positive", (LlmBrgSelect==0) ? "Rotor Brg1" : (LlmBrgSelect==1) ?
                                 "Rotor Brg2" : (LlmBrgSelect==2) ? "Line Brg1" : "Line Brg2");

            CurrentOffset = CommandLineArgs.find("-dsatapx");
            if ( CurrentOffset != CommandLineArgs.npos )
            {
                OutputBuffer.pappend(" -> ok\n");
                switch ( LlmBrgSelect )
                {
                    case 0:
                        LlmForceTable[0][4] = false;
                        break;
                    case 1:
                        LlmForceTable[0][7] = false;
                        break;
                    case 2:
                        LlmForceTable[1][4] = false;
                        break;
                    case 3:
                        LlmForceTable[1][7] = false;
                        break;
                }
            }
            else
            {
                OutputBuffer.pappend(" -> forced bad\n");
                switch ( LlmBrgSelect )
                {
                    case 0:
                        LlmForceTable[0][4] = true;
                        break;
                    case 1:
                        LlmForceTable[0][7] = true;
                        break;
                    case 2:
                        LlmForceTable[1][4] = true;
                        break;
                    case 3:
                        LlmForceTable[1][7] = true;
                        break;
                }
            }

            return EOK;
        }

        // negative rail
        CurrentOffset = CommandLineArgs.find("-dsatan");
        if ( CurrentOffset != CommandLineArgs.npos )
        {
            OutputBuffer.pappend("%s dsat phase a negative", (LlmBrgSelect==0) ? "Rotor Brg1" : (LlmBrgSelect==1) ?
                                 "Rotor Brg2" : (LlmBrgSelect==2) ? "Line Brg1" : "Line Brg2");

            CurrentOffset = CommandLineArgs.find("-dsatanx");
            if ( CurrentOffset != CommandLineArgs.npos )
            {
                OutputBuffer.pappend(" -> ok\n");
                switch ( LlmBrgSelect )
                {
                    case 0:
                        LlmForceTable[0][5] = false;
                        break;
                    case 1:
                        LlmForceTable[0][8] = false;
                        break;
                    case 2:
                        LlmForceTable[1][5] = false;
                        break;
                    case 3:
                        LlmForceTable[1][8] = false;
                        break;
                }
            }
            else
            {
                OutputBuffer.pappend(" -> forced bad\n");
                switch ( LlmBrgSelect )
                {
                    case 0:
                        LlmForceTable[0][5] = true;
                        break;
                    case 1:
                        LlmForceTable[0][8] = true;
                        break;
                    case 2:
                        LlmForceTable[1][5] = true;
                        break;
                    case 3:
                        LlmForceTable[1][8] = true;
                        break;
                }
            }

            return EOK;
        }
    }

    //-------------------------------------------------------------------------
    // contact inputs 09 and 10
    CurrentOffset = CommandLineArgs.find("-ci");
    if ( CurrentOffset != CommandLineArgs.npos )
    {
        // ci9
        CurrentOffset = CommandLineArgs.find("-ci9");
        if ( CurrentOffset != CommandLineArgs.npos )
        {
            OutputBuffer.pappend("%s contact input 09", (LlmBrgSelect<2) ? "Rotor Brg1 & Brg2" : "Line Brg1 & Brg2");

            CurrentOffset = CommandLineArgs.find("-ci9x");
            if ( CurrentOffset != CommandLineArgs.npos )
            {
                OutputBuffer.pappend(" -> ok\n");
                switch ( LlmBrgSelect )
                {
                    case 0:
                    case 1:
                        LlmForceTable[0][1] = false;
                        break;
                    case 2:
                    case 3:
                        LlmForceTable[1][1] = false;
                        break;
                }
            }
            else
            {
                OutputBuffer.pappend(" -> forced bad\n");
                switch ( LlmBrgSelect )
                {
                    case 0:
                    case 1:
                        LlmForceTable[0][1] = true;
                        break;
                    case 2:
                    case 3:
                        LlmForceTable[1][1] = true;
                        break;
                }
            }

            return EOK;
        }

        // ci10
        CurrentOffset = CommandLineArgs.find("-ci10");
        if ( CurrentOffset != CommandLineArgs.npos )
        {
            OutputBuffer.pappend("%s contact input 10", (LlmBrgSelect<2) ? "Rotor Brg1 & Brg2" : "Line Brg1 & Brg2");

            CurrentOffset = CommandLineArgs.find("-ci10x");
            if ( CurrentOffset != CommandLineArgs.npos )
            {
                OutputBuffer.pappend(" - ok\n");
                switch ( LlmBrgSelect )
                {
                    case 0:
                    case 1:
                        LlmForceTable[0][2] = false;
                        break;
                    case 2:
                    case 3:
                        LlmForceTable[1][2] = false;
                        break;
                }
            }
            else
            {
                OutputBuffer.pappend(" - forced bad\n");
                switch ( LlmBrgSelect )
                {
                    case 0:
                    case 1:
                        LlmForceTable[0][2] = true;
                        break;
                    case 2:
                    case 3:
                        LlmForceTable[1][2] = true;
                        break;
                }
            }

            return EOK;
        }
    }

    //-------------------------------------------------------------------------
    // unable to find a command line match
    OutputBuffer.pappend("no valid command line arguemnt found [-s,-bi,-inpwrok(x),-ioca(x),-dsata|p/n|(x),-ci|9/10|(x)]\n");
    return EOK;
}



/////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//    Fill low level monitor buffer with information about the target
//    bridge interface.
//
// EXECUTION LEVEL
//    Initialization.
//
// RETURN VALUE
//      None
//
/////////////////////////////////////////////////////////////////////////////
int cBrgSim::LlmInit( cMonitorCommand &gMonitor )
{
    int             Status = EOK;
    cParamString    HelpString = "\n";

    HelpString  = "Force bridge interface faults\n";

    if ( gMonitor.Add( "BrgForce", HelpString, LlmForceBrgFlt, NORMAL_OUTPUT_SIZE, "force" ) != EOK )
    {
        Status = -1;    // fail
    }

    return Status;
}
