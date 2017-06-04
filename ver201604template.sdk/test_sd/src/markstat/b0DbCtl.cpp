///////////////////////////////////////////////////////////////////////////////
//
// TITLE        DB Control Class
//
// DESCRIPTION
//      This file contains the methods for the b0DbCtl class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0DbCtl.h, b0DbCtl.vsd, b0DbCtl.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
// product
#include "b0DbCtl.h"


// Constants
//----------
#define  DB_VCE         (2.0)

// Externals
//----------

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
cDbCtl::cDbCtl
(
        InListTyp    InList,
        InsListTyp   InsList
)
        : In(InList), Ins(InsList)
{
    DbcVregRef             = 0.0F;
    DbcVregOut             = 0.0F;
    DbcVregPrp             = 0.0F;
    DbcVregInt             = 0.0F;
    DbcVregErr             = 0.0F;
    DbcVregCmd             = 0.0F;
    DbcVregStp             = 0.0F;
    DbcVregIntGnDt         = 0.0F;

    DbcMod1PktInt          = 0.0F;
    DbcMod1OutInt          = 0.0F;
    DbcModPktSize          = 0.0F;
    DbcMod2PktInt          = 0.0F;
    DbcMod2OutInt          = 0.0F;

    DbcVdb1Expect          = 0.0F;
    DbcVdb2Expect          = 0.0F;
    DbcVdb1VdcErr          = 0.0F;
    DbcVdb2VdcErr          = 0.0F;
    DbcVdb1VdcTrpFlg       = false;
    DbcVdb2VdcTrpFlg       = false;
    Db1VmutualComp         = 0.0F;
    Db2VmutualComp         = 0.0F;

    DbcDb1MisOffTrpFlg     = false;
    DbcDb1MisOnTrpFlg      = false;
    DbcDb2MisOffTrpFlg     = false;
    DbcDb2MisOnTrpFlg      = false;

    OUT(DbcMod1Out)        = 0;
    OUT(DbcMod2Out)        = 0;
    OUT(DbcVdb1VdcMisFlg)  = false;
    OUT(DbcVdb2VdcMisFlg)  = false;
    OUT(DbcDb1MisOffTstFlg)= false;
    OUT(DbcDb1MisOnTstFlg) = false;
    OUT(DbcDb2MisOffTstFlg)= false;
    OUT(DbcDb2MisOnTstFlg) = false;
    OUT(DbcDb1AutoTstDone) = false;
    OUT(DbcDb2AutoTstDone) = false;

    DbcMod1OutZ1           = 0;
    DbcMod1OutZ2           = 0;
    DbcMod1OutZ3           = 0;
    DbcMod2OutZ1           = 0;
    DbcMod2OutZ2           = 0;
    DbcMod2OutZ3           = 0;

    DbcGateDb1First        = false;
    DbcDutyRef             = 0.0F;
    DbcDutyCmd             = 0.0F;
    DbcOpnLoopDutyRef      = 0.0F;
    DbcVregExtRef          = 0.0F;
    DbFltDsbTmr            = 0.0F;
    DbFpgaDsb              = false,
    DbcAutoTstCnt          = 0;
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
cDbCtl::~cDbCtl ()
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
int cDbCtl::RunRules (void)
{
    int Status = SUCCESS;

    SET_FV(DbcVrefHt,     INS(VdcOvLvl) - 50.0F);
    SET_FV(DbcGateSkewTm, 10.0e-6F);  // value got from Greg Aug 15 2012
    SET_FV(DbVmutualCompPu, 0.01F);

    //-------------------------------------------------------------------------
    // DB control test
    SET_FV( DbcOpnLpDutyAdr, (float *) LADR_FZero);
    SET_FV( DbcVregExtRefAdr,(float *) LADR_FZero);
    SET_FV( DbcOpnLoopEnb,   false);
    SET_FV( DbcVregExtRefEnb,false);

    //-------------------------------------------------------------------------
    // V reg
    SET_FV( DbcVregIntLimP, 1.0F);
    SET_FV( DbcVregIntLimN, 0.0F);
    SET_FV( DbcVregBwRatio, 0.5F); // this gives a 25% of the interrupt rate as the normally used BW for the regualtor
    SET_FV( DbcVregStepAdr,(float *) LADR_FZero);
    // interrupt rate = 1/ DelTm1
    // Reg max BW     = 50% * interrupt rate
    // default the BW to half of the max BW, adjustable use parameter DbcVregBwRatio
    SET_FV( DbcVregBw, NoNANLim(1500.0F*250e-6F/INS(DelTm1), 10.0e6F, -10.0e6F ));
    DbcPlantGn = INS(DbVmax)/( INS(DbResCombined)*INS(LinkCap) );
    //SET_FV( DbcVregPrpGn, PARM(DbcVregBw) / DbcPlantGn );
    //SET_FV( DbcVregIntGn, (PARM(DbcVregBw) * 0.5F ) * (PARM(DbcVregBw) * 0.5F ) / DbcPlantGn );
    // follow MACC
    SET_FV( DbcVregPrpGn, 5.0F * PARM(DbcVregBw) / DbcPlantGn );
    SET_FV( DbcVregIntGn, 0.0428F*PARM(DbcVregBw)*PARM(DbcVregBw) / DbcPlantGn );
    DbcVregIntGnDt = PARM(DbcVregIntGn) * INS(DelTm1);

    //-------------------------------------------------------------------------
    // modulator
    SET_FV( DbcModDutyMax, 1.0F );
    // later gated DB is phase shifted apart by a configurable phase. Normally the
    // phase shift is 180 degrees (0.5). This offset should be between [0, 1]
    SET_FV( DbcModOffs, 0.5F );
    SET_FV( DbcModPktSize, 1.0F );    // normal firing DB, the pkt size is 1 task1

    //-------------------------------------------------------------------------
    // auto ping
    // spec: the interval between two auto pings are 15minutes. Max DC link voltage is 1125 Volt, rotor rate
    // Interval * DbcAutoTstPwrAvg = Vmax* Vmax/ DBR_total*DBPlsWdth
    // DBPlsWdth = DbcAutoPktSize * DelTm1
    SET_FV( DbcAutoTstPktSize, 2.0F )     // default to 2 task 1 for each auto pulse width
    //SET_FV( DbcAutoTstIntv, 900.0F );     // 15*60 = 900 sec
    //SET_FV( DbcAutoTstPwrAvg, NoNANLim (INS(VdcRefMax)*INS(VdcRefMax)/ INS(DbResCombined) * PARM(DbcAutoTstPktSize)* INS(DelTm1) /PARM(DbcAutoTstIntv), 10.0e6F, 0.0F ) );
    SET_FV( DbcDbPwrMax, NoNANLim( INS(DbVmax)*INS(DbVmax)/INS(DbResCombined), 10.0e6F, -10.0e6F ) );
    SET_FV(DbcAutoTstPwrAvg,    4.5732e-4*INS(DbContPwr));  //L_DBAPlsPwr in 1.6e, apx 3 watts for productfollow
    SET_FV(DbcAutoTstIntv,      INS(VdcRefMax)*INS(VdcRefMax)/(INS(DbResCombined)*PARM(DbcAutoTstPwrAvg))*INS(DelTm1)*PARM(DbcAutoTstPktSize));
                                                            // L_DBAPlsTm in 1.6e

    // pass to duty reference
    if ( INS(TestMode) == PUMP_BACK )
    {
        SET_FV( DbcAutoTstDutySetp, 4.60e-6F);
    }
    else
    {
        //SET_FV( DbcAutoTstDutySetp, NoNANLim( PARM(DbcAutoTstPwrAvg)/PARM(DbcDbPwrMax) , 10.0e6, -10.0e6 ) );
        SET_FV( DbcAutoTstDutySetp, 0.0F);
    }
	
    SET_FV(DbcAutoTstVdcMin, 0.5F );	//min voltage which allows DB duty during auto
	
    //-------------------------------------------------------------------------
    // DB voltage check
    SET_FV( DbcVfbkAdj, 1.11813*( INS(DbVfbkTau)/INS(DelTm1) )  ); // approximation to avg value of (1-e-t/tau) for one dt. 0.905 is the tunning factor got from ETU

    if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_B)
    {
        SET_FV( DbcVdbVdcThrsh, (0.04F*(INS(SclLinkVolts)* INS(VdbVcoFreq))) + 2.50F);      //2e6 is Vco freq
    }
    else
    {
        SET_FV( DbcVdbVdcThrsh, 0.04F*(INS(SclLinkVolts)* INS(VdbVcoFreq)));      //2e6 is Vco freq
    }

    //-------------------------------------------------------------------------
    // done
    return (Status);
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Precondition the regulator and modulator
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbCtl::DbcPrecondition (void)
{
    DbcVregCmd      = 0.0F;
    DbcVregOut      = 0.0F;
    DbcVregInt      = 0.0F;
    DbcVregErr      = 0.0F;
    DbcVregRef      = 0.0F;

    DbcMod1OutInt   = 0.0F;
    DbcMod2OutInt   = 0.0F;

    DbcDutyCmd      = 0.0F;

    OUT(DbcMod1Out) = 0;
    OUT(DbcMod2Out) = 0;

    DbcMod1OutZ1    = 0;
    DbcMod1OutZ2    = 0;
    DbcMod1OutZ3    = 0;

    DbcMod2OutZ1    = 0;
    DbcMod2OutZ2    = 0;
    DbcMod2OutZ3    = 0;

    OUT(DbcDb1AutoTstDone) = false;
    OUT(DbcDb2AutoTstDone) = false;

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      DB voltage regulator used to regulate DC link voltage when DB is active
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbCtl::DbcVreg (void)
{
    //-------------------------------------------------------------------------
    // get the value to step the regulator
    DbcVregStp  = READ_PARMPTR(DbcVregStepAdr);

    //-------------------------------------------------------------------------
    // regulator command
    DbcVregCmd  = DbcVregRef + DbcVregStp;

    //-------------------------------------------------------------------------
    // regulator error
    DbcVregErr  = IN(DbVregFbk) - DbcVregCmd;

    //-------------------------------------------------------------------------
    // propotional path
    DbcVregPrp  = DbcVregErr * PARM(DbcVregPrpGn);

    //-------------------------------------------------------------------------
    // Integral path calculation and limit
    DbcVregInt += DbcVregErr * DbcVregIntGnDt;
    if( DbcVregInt > PARM(DbcVregIntLimP) )
    {
        DbcVregInt = PARM(DbcVregIntLimP);
    }
    else if( DbcVregInt < PARM(DbcVregIntLimN) )
    {
        DbcVregInt = PARM(DbcVregIntLimN);
    }

    //-------------------------------------------------------------------------
    // V reg outout (Db cuty cycle)
    DbcVregOut  = DbcVregPrp+ DbcVregInt;

    //-------------------------------------------------------------------------
    // limit the Vreg outout to be no less than zero
    if( DbcVregOut < 0.0F )
    {
        DbcVregOut = 0.0F;
    }

    //-------------------------------------------------------------------------
    // done
    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Each DB has two parallel devices. The two upper swithces are always open.
//      DB modulator controls gating of the two lower DB switches to satisfy
//      the commanded duty cycle. Two lower switches are controlled by the same commands.
//      This function works with the DB voltage regulator(s) and any test
//      functions that need coordinated control of the DB switch gating.
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbCtl::DbcModulator
(
    // modulator input
    float      DbcDutyCmd,       // duty cycle reference for the modulator
    unsigned   DbcGateSwFirst,   // DB gate switch first on (used for if having more than 1 DB)
    //unsigned   DbOtLockFlg,     // DB over temperature lock-on indication
    float      *DbcModPktInt,    // DB modulator packet integrator
    float      *DbcModOutInt,    // the integrator of the float version of the modulator output

    // modulator output
    int        *DbcModOut        // DB modulator switch gate command (1=on, 0=off)
)
{

    //-------------------------------------------------------------------------
    // limit commanded duty cycle
    if ( DbcDutyCmd > PARM(DbcModDutyMax) )
    {
        DbcDutyCmd = PARM(DbcModDutyMax);
    }
    else if ( DbcDutyCmd < 0.0 )
    {
        DbcDutyCmd = 0.0;
    }

    //-------------------------------------------------------------------------
    // select the pkt size for the modulator
    if( IN(DbAutoTstEnb) )
    {
        DbcModPktSize = PARM (DbcAutoTstPktSize);
    }
    else
    {
        DbcModPktSize = PARM (DbcModPktSize);
    }


    //-------------------------------------------------------------------------
    // reset the modualtor integral if duty command=0 for stability
    if( (DbcDutyCmd == 0.0) || ( !DbAutoTstEnbZ1 && IN(DbAutoTstEnb) ) )
    {
        if( DbcGateSwFirst )
        {
            // preset integrators to gate first
            *DbcModPktInt = PARM(DbcModOffs) * DbcModPktSize;
        }
        else
        {
            // preset integrators to Not gate first
            *DbcModPktInt = 0.0;
        }
    }

    //-------------------------------------------------------------------------
    // DB pulse packet size: integrate till reach pkt size
    *DbcModOutInt += (float)(*DbcModOut);

    //-------------------------------------------------------------------------
    // limit the modulator output integrator
    if( *DbcModOutInt >= DbcModPktSize )
    {
        *DbcModPktInt += DbcDutyCmd - DbcModPktSize;
        // reset the integral when reach the pkt size
        *DbcModOutInt  = 0.0;
    }
    else
    {
        *DbcModPktInt += DbcDutyCmd - 0.0;
    }

    //-------------------------------------------------------------------------
    // limit the modulator integrator
    if ( *DbcModPktInt >= (DbcModPktSize + 1) )
    {
        *DbcModPktInt = DbcModPktSize + 1;
    }

    //-------------------------------------------------------------------------
    // convert the duty to the command
    if ( *DbcModPktInt >= DbcModPktSize )  // || (DbOtLockFlg == true)
    {
        *DbcModOut = 1;
    }
    else
    {
        *DbcModOut = 0;
    }

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      check the DB voltage consistency
//
//      Calculate expected db voltage, DbcVdbExpect, based on DC link voltage
//        Z2    Z3
//        0     0  -- off at least two dt's    -- expect near zero volts
//        0     1  -- off for only one dt      -- expect about 10% of vdc
//        1     0  -- on for only one dt       -- expect about 90% of vdc
//        1     1  -- on for at least two dt's -- expect near vdc volts
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbCtl::DbcVdbFbkCheck
(
    // input
    int      DbcDbStCmdZ2,          // DB gate commanded status delayed Z-2(1=on, 0=off)
    int      DbcDbStCmdZ3,          // DB gate commanded status delayed Z-3(1=on, 0=off)
    float    DbcVdbFbk,             // actual Db voltage (volt)
    unsigned DbcGateSkewEnb,        // enable the DB gate command skew
    unsigned DbFpgaDsb,             // DB fpga disable
    float    DbVmutualComp,         // Compensation the mutual excited DB voltage when DB is off (Volt)

    // output
    unsigned   *DbcVdbVdcTrpFlg,    // DB voltage mismatch trip flag
    unsigned   *DbcVdbVdcMisFlg,    // DB voltage mismatch message flag
    float      *DbcVdbExpect,       // expected DB voltage (volt)
    float      *DbcVdbVdcErr        // DB voltage error (volt)
)
{

    //-------------------------------------------------------------------------
    // calculate the expected DB voltage

    //Down for at least two states:                  Z2    Z3
    if (!DbcDbStCmdZ2 && !DbcDbStCmdZ3)     //        0     0
    {
        *DbcVdbExpect = 0.0;
    }
    // off for only one dt
    else if (!DbcDbStCmdZ2 && DbcDbStCmdZ3)  //     0     1
    {
        if ( ! DbcGateSkewEnb )
        {
            *DbcVdbExpect = PARM(DbcVfbkAdj) * IN(DbVregFbk);
        }
        else
        {
            // first turn off the DB, need to add the delay caused by DB Skew
            *DbcVdbExpect = PARM(DbcVfbkAdj) * IN(DbVregFbk) + PARM(DbcGateSkewTm)/ INS(DelTm1) * IN(DbVregFbk);
        }
    }
    // on for only one dt
    else if (DbcDbStCmdZ2 && !DbcDbStCmdZ3)  //     1     0
    {
        if ( ! DbcGateSkewEnb )
        {
            *DbcVdbExpect = IN(DbVregFbk) - (PARM(DbcVfbkAdj) * IN(DbVregFbk)) - DB_VCE;
        }
        else
        {
            // first turn on the DB, need to substract the delay caused by DB Skew
            *DbcVdbExpect = IN(DbVregFbk) - (PARM(DbcVfbkAdj) * IN(DbVregFbk)) - DB_VCE - PARM(DbcGateSkewTm)/ INS(DelTm1) * IN(DbVregFbk);
        }
    }
    // on for at least two dt's                     1     1
    //must be 1 1 -- only thing left
    else
    {
        *DbcVdbExpect = IN(DbVregFbk);
    }

    // Mutual excitation voltage compensation
    *DbcVdbExpect += DbVmutualComp;

    //-------------------------------------------------------------------------
    // check if the error between the expected DB voltage and the actual feedback
    // is within the allowance

    // error calculation
    *DbcVdbVdcErr = *DbcVdbExpect - DbcVdbFbk;

    // error check
    if ( ABS(*DbcVdbVdcErr) > PARM(DbcVdbVdcThrsh) && !DbFpgaDsb )
    {
        if ( !IN(DbTstIp) )
        {
            *DbcVdbVdcTrpFlg = true;     // used to push out trip due to DB expected voltage is incorrect during normal DB operation
        }
        else
        {
            *DbcVdbVdcMisFlg = true;     // used to push out message to show DB expected voltage is incorrect during DB cell test, no trip
            *DbcVdbVdcTrpFlg = false;
        }
    }
    else
    {
        *DbcVdbVdcMisFlg = false;
        *DbcVdbVdcTrpFlg = false;
    }

    //-------------------------------------------------------------------------
    // done
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      check the DB state fault
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbCtl::DbcVdbStateCheck
(
    // input
    int   DbcDbStCmdZ2,               // DB gate commanded status , delayed Z-2(1=on, 0=off)
    int   DbcDbSwFbkSt,               // DB gate feedback status

    // output
    unsigned* DbcDbMisOffTrpFlg,      // DB mistakenly off trip flag
    unsigned* DbcDbMisOnTrpFlg,       // DB mistakenly on trip flag
    unsigned* DbcDbMisOffTstFlg,      // DB mistakenly off flag if in DB test mode
    unsigned* DbcDbMisOnTstFlg        // DB mistakenly on flag if in DB test mode
)
{
    // command is two-task1A earlier than the feedback
    // so delay the command by two-task1A
    if ( DbcDbStCmdZ2 != DbcDbSwFbkSt && !DbFpgaDsb )
    {
        if ( !DbcDbSwFbkSt )
        {
            if ( !IN(DbTstIp) )
            {
                *DbcDbMisOffTrpFlg = true;  //commanded db on...but off
            }
            else
            {
                *DbcDbMisOffTstFlg = true;  // used to push out message to show DB mis off during DB cell test, no trip
                *DbcDbMisOffTrpFlg = false; // used to push out trip due to DB mis off during normal DB operation
            }
        }
        else
        {

            if ( !IN(DbTstIp) )
            {
                *DbcDbMisOnTrpFlg = true;  //commanded db off...but on
            }
            else
            {
                *DbcDbMisOnTstFlg = true;  // used to push out message to show DB mis on during DB cell test, no trip
                *DbcDbMisOnTrpFlg = false; // used to push out trip due to DB mis on during normal DB operation
            }
        }
    }
    else
    {
       *DbcDbMisOffTrpFlg = false;
       *DbcDbMisOnTrpFlg  = false;

       *DbcDbMisOffTstFlg = false;
       *DbcDbMisOnTstFlg  = false;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      calculate the gate commands for each DB
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void cDbCtl::Task1 (void)
{
    if( IN(DbInh) == DUAL_INHIBIT )     // Both Db circuits inhibited
    {
        DbcPrecondition();
    }
    else                                // One or two Db circuits permitted
    {

        //-------------------------------------------------------------------------
        // preserve the delayed variables
        DbcMod1OutZ3 = DbcMod1OutZ2;
        DbcMod1OutZ2 = DbcMod1OutZ1;
        DbcMod1OutZ1 = IN(DbCmd1);

        DbcMod2OutZ3 = DbcMod2OutZ2;
        DbcMod2OutZ2 = DbcMod2OutZ1;
        // update DB command delay-line if previous command was actuated
        // otherwise retain the cmd state prior to that one

        if ( !IN(DbCmdNotAct) )
        {
            DbcMod1OutZ1 = IN(DbCmd1);
            DbcMod2OutZ1 = IN(DbCmd2);
        }

        //-------------------------------------------------------------------------
        // select the reference for the voltage regulator
        if( PARM(DbcVregExtRefEnb) )
        {
            DbcVregExtRef = READ_PARMPTR(DbcVregExtRefAdr);
            DbcVregRef    = DbcVregExtRef;
        }
        else
        {
            DbcVregRef = INS(DbVmax);
            if(IN(SelVrefLvl)==(static_cast<int>(DBC_VREF_TRHOT)))
            {
                DbcVregRef = PARM(DbcVrefHt);
            }
        }

        //-------------------------------------------------------------------------
        // call the Vreg
        DbcVreg();

        //-------------------------------------------------------------------------
        // DB auto test control
        if( !IN(DbAutoTstEnb) )
        {
            if ( (IN(DbExtDutyRef) > 0.0F) && (DbcVregOut <= 0.0F) )
            {
                DbcDutyRef = IN(DbExtDutyRef);  // take non-zero external duty-ref
            }                                   // if regulator is not active
            else
            {
                DbcDutyRef = DbcVregOut;
            }
        }
        // clamp the duty reference with auto ping duty ref when auto ping is enabled
        else
        {
            if ( DbcVregOut <= DbcAutoTstDuty )
            {
                DbcDutyRef = DbcAutoTstDuty;
            }
            else
            {
                DbcDutyRef = DbcVregOut;
            }
        }

        //-------------------------------------------------------------------------
        // select the DB duty cmd used by DB modulator
        if ( PARM(DbcOpnLoopEnb) )
        {
            PUSH_DIAG(DbcOpnLoopAlm);
            DbcOpnLoopDutyRef = READ_PARMPTR(DbcOpnLpDutyAdr);
            DbcDutyCmd        = DbcOpnLoopDutyRef;
        }
        else
        {
            DbcDutyCmd = DbcDutyRef;
        }

        //-------------------------------------------------------------------------
        // Select first DB to gate based on resistor with lowest thermal energy
        // to balance thermal energy in DB resistors, if there are two DB circuits.
        // If the two DBs have the same temperature, DB2 gate first.
        if ( IN(DbInh) == SNGL_INHIBIT )
        {
            DbcGateDb1First = false;
        }
        else if ( IN(Db1ThrmEnrgy) < IN(Db2ThrmEnrgy) )
        {
            DbcGateDb1First = true;
        }
        else
        {
            DbcGateDb1First = false;
        }

        //-------------------------------------------------------------------------
        // calculate DB gate commands for each DB
        if ( IN(DbInh) == NO_INHIBIT )
        {
            DbcModulator( DbcDutyCmd, DbcGateDb1First,  &DbcMod1PktInt, &DbcMod1OutInt, OUT(&DbcMod1Out) );
        }
        DbcModulator( DbcDutyCmd, !DbcGateDb1First, &DbcMod2PktInt, &DbcMod2OutInt, OUT(&DbcMod2Out) );
        DbAutoTstEnbZ1 = IN(DbAutoTstEnb);

        //-------------------------------------------------------------------------
        // immediately fire the DB auto test if power up
        // or if Auto test is forced to run, force the DB cmd 'ON' immediately
        if( IN(DbcAutoTstForce) )
        {
            OUT(DbcDb1AutoTstDone) = false;
            OUT(DbcDb2AutoTstDone) = false;
            // Force DB on for two tasks
            DbcAutoTstCnt += 1;
            if ( DbcAutoTstCnt <= PARM(DbcAutoTstPktSize) )
            {
                if ( IN(DbInh) == NO_INHIBIT )
                {
                    OUT(DbcMod1Out) = 1;
                }
                OUT(DbcMod2Out) = 1;
            }
            else
            {
                OUT(DbcMod1Out) = 0;
                OUT(DbcMod2Out) = 0;
                if ( IN(DbInh) == NO_INHIBIT )
                {
                    OUT(DbcDb1AutoTstDone) = true;
                }
                OUT(DbcDb2AutoTstDone) = true;
            }
        }
        else
        {
            DbcAutoTstCnt = 0;
        }

        //-------------------------------------------------------------------------
        // DB FPGA disable logic
        if ( IN(BrgFltHdw) && IN(BrgCardsAct) )     // rotor bridge  fpga fault
        {
            DbFltDsbTmr = INS(DelTm1) *5.0;         // Disable for 5 dts after enable comes back
            DbFpgaDsb   = true;
        }
        else
        {
            DbFltDsbTmr = DbFltDsbTmr - INS(DelTm1);
            if ( DbFltDsbTmr <= 0.0)
            {
                DbFltDsbTmr = 0.0;
                DbFpgaDsb   = false;
            }
        }

        if ( IN(DbInh) == NO_INHIBIT )
        {
            //-------------------------------------------------------------------------
            // Calculate the mutual excited votlage when DB2 is off, only if there are
            // two DB circuits
            if ( DbcMod1OutZ2 && (!DbcMod2OutZ2) )
            {
                Db2VmutualComp = PARM(DbVmutualCompPu) * IN(DbVregFbk);
            }
            else
            {
                Db2VmutualComp = 0.0F;
            }

            //-------------------------------------------------------------------------
            // Calculate the mutual excited votlage when DB1 is off
            if ( DbcMod2OutZ2 && (!DbcMod1OutZ2) )
            {
                Db1VmutualComp = PARM(DbVmutualCompPu) * IN(DbVregFbk);
            }
            else
            {
                Db1VmutualComp = 0.0F;
            }

            //-------------------------------------------------------------------------
            // check the DB1 voltage consistence
            DbcVdbFbkCheck( DbcMod1OutZ2, DbcMod1OutZ3, IN(Vdb1), False, DbFpgaDsb, Db1VmutualComp, &DbcVdb1VdcTrpFlg, OUT(&DbcVdb1VdcMisFlg), &DbcVdb1Expect, &DbcVdb1VdcErr );
            if ( DbcVdb1VdcTrpFlg )
            {
                PUSH_DIAG(DbcVdbVdcTrp);
            }

            //-------------------------------------------------------------------------
            // check the DB1 switch state
            DbcVdbStateCheck( DbcMod1OutZ2, IN(Db1SwStFbk), &DbcDb1MisOffTrpFlg, &DbcDb1MisOnTrpFlg, OUT(&DbcDb1MisOffTstFlg), OUT(&DbcDb1MisOnTstFlg) );
            if ( DbcDb1MisOffTrpFlg )
            {
                PUSH_DIAG(DbcDbMisOffTrp);
            }
            if ( DbcDb1MisOnTrpFlg )
            {
                PUSH_DIAG(DbcDbMisOnTrp);
            }
        }
        else
        {
            Db2VmutualComp = 0.0F;  // No mutual excitation when there is only one DB circuit
        }
      /*
        //-------------------------------------------------------------------------
        // check the DB2 voltage consistence. Db2 gate command is skewed by FPGA
        DbcVdbFbkCheck( DbcMod2OutZ2, DbcMod2OutZ3, IN(Vdb2), IN(DbGateSkewEnb), DbFpgaDsb, Db2VmutualComp, &DbcVdb2VdcTrpFlg, OUT(&DbcVdb2VdcMisFlg), &DbcVdb2Expect, &DbcVdb2VdcErr );
        if ( DbcVdb2VdcTrpFlg )
        {
            PUSH_DIAG(DbcVdb2VdcTrp);
        }

        //-------------------------------------------------------------------------
        // check the DB2 switch state
        DbcVdbStateCheck( DbcMod2OutZ2, IN(Db2SwStFbk), &DbcDb2MisOffTrpFlg, &DbcDb2MisOnTrpFlg, OUT(&DbcDb2MisOffTstFlg), OUT(&DbcDb2MisOnTstFlg) );
        if ( DbcDb2MisOffTrpFlg )
        {
            PUSH_DIAG(DbcDb2MisOffTrp);
        }
        if ( DbcDb2MisOnTrpFlg )
        {
            PUSH_DIAG(DbcDb2MisOnTrp);
        }
     */
    }

    //-------------------------------------------------------------------------
    // done
    return;
}
void cDbCtl::Task3 (void)
{
    // pass to duty reference
    if ( INS(TestMode) == PUMP_BACK )
    {
        DbcAutoTstDutyT3=PARM(DbcAutoTstDutySetp);
    }
    else
    {
		//DLL sim start up state, L_VdcFbkT3=0 at beginning, DB close for many cycles
        if (IN(VdcT3) < PARM(DbcAutoTstVdcMin))
        {
            DbcAutoTstDutyT3=0;
        }
        else
        {
            DbcAutoTstDutyT3=NoNANLim( PARM(DbcAutoTstPwrAvg)*INS(DbResCombined)/(IN(VdcT3)*IN(VdcT3)) , 10.0e6, -10.0e6 );
        }
    }

    //-------------------------------------------------------------------------
    // done
    return;
}
