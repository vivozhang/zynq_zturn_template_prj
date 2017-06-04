///////////////////////////////////////////////////////////////////////////////
//
// TITLE        DB Control Class
//
// DESCRIPTION
//      This file contains the definition for the b0DbCtl class.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0DbCtl.vsd, b0DbCtl.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef B0_DB_CTL_H
#define B0_DB_CTL_H


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"


// Constants
//----------


// Function Prototypes
//--------------------


// Types and Classes
//------------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      This class provides the DB control functionalities for the two parallel DB.
//      It includes the DB voltage regulator, DB modualtor and auto ping
//      functionalities.The class outputs two commands signals to be used by
//      AEDB via bridge interface to fire two DBs. The functionality of the class
//      can be tested by manually giving reference either to the voltage regulator
//      or step the voltage regulator. The functionality of the DB modulator can
//      also be tested by open the loop between the voltage regulator and the
//      modulator to allow the modulator to use the open loop duty reference
//      manually set by the developer.
//
///////////////////////////////////////////////////////////////////////////////
class cDbCtl
{

//-----------------------------------------------------------------------------
public:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;


    // Enumerations
    //-------------

    // Db circuit inhibit options
    enum DbInhTyp
    {
        NO_INHIBIT        = 0,      // Both Db circuits permitted
        SNGL_INHIBIT      = 1,      // Only Db-1 circuit is inhibited
        DUAL_INHIBIT      = 2       // Both Db circuits inhibited
    };

    enum CroBarThrSelTyp                 // CroBar dynamic threshold selection
    {
        DBC_VREF_NORMAL = 0,
        DBC_VREF_TRHOT  = 1,
    };
    // Types
    //------
    struct InListTyp
    {
	    #ifdef DSIM_MODE
        const int&          DbInh;  
        #else
        const DbInhTyp&     DbInh;               //  1 - inhibit the DB to fire
        #endif
        const unsigned&     DbAutoTstEnb;        //  2 - enable the auto DB test
        const float&        Db1ThrmEnrgy;        //  3 - DB1 thermal energy retention rate (Watts)
        const float&        Db2ThrmEnrgy;        //  4 - DB2 thermal energy retention rate (Watts)
        const float&        DbVregFbk;           //  5 - DC link voltage feedback (volt)
        const float&        Vdb1;                //  6 - actural measured DB1 voltage from REBF (volt)
        const float&        Vdb2;                //  7 - actural measured DB2 voltage from REBF (volt)
        const float&        DbExtDutyRef;        //  8 - external duty reference
        const unsigned&     DbTstIp;             //  9 - DB is in cell test mode
        const unsigned&     BrgCardsAct;         // 10 -overall bridge cards active boolean
        const unsigned&     BrgFltHdw;           // 11 - bridge fault indicator
        const unsigned&     Db1SwStFbk;          // 12 - DB1 switch status feedback
        const unsigned&     Db2SwStFbk;          // 13 - DB2 switch status feedback
        const unsigned&     DbGateSkewEnb;       // 14 - enable the DB gate skew
        const unsigned&     Db1OtLock;           // 15 - DB1 switch over temp lock-on command
        const unsigned&     Db2OtLock;           // 16 - DB2 switch over temp lock-on command
        const unsigned&     DbcAutoTstForce;     // 17 - force the Db auto ping imediately fire
        const unsigned&     DbCmdNotAct;         // 18- previous DB cmd was not actuated
        const unsigned&     DbCmd1;              // 19 - Acutal DB command 1
        const unsigned&     DbCmd2;              // 20 - Acutal DB command 2
        const float&        VdcT3;               // 21 - DC link voltage in task 3 version
        const int&          SelVrefLvl;          // 22 - DB voltage regulator reference selection
    };

    struct InsListTyp
    {
        const float&        LinkCap;             //  1 - DC link capacitance (Farad)
        const float&        DbResCombined;       //  2 - combined DB resistance value in prallel (Ohm)
        const float&        DbVmax;              //  3 - maximum DC link voltage for DB to kick in (volt)
        const float&        VdcRefMax;           //  4 - the maximum DC link reference voltage (volt)
        const float&        DelTm1;              //  5 - task 1 execution DelTm (sec)
        const float&        SclLinkVolts;        //  6 - link voltage feedback scaling
        const float&        DbVfbkTau;           //  7 - DB voltage feedback circuit time constant)
        const float&        VdbVcoFreq;          //  8 - DB voltage feedback VCO frequency (Hz)
        const int&          TestMode;            //  9 - converter Test mode
        const float&        DbContPwr;           // 10 - Continuous power rating of DB resistor (Watts)
        const float&        VdcOvLvl;            // 11 - DC link over-voltage level

    };


    // Methods
    //--------
    cDbCtl( InListTyp InList, InsListTyp InsList );
    ~cDbCtl();

    int  RunRules(void);
    void Task1(void);
    void Task3(void);


    // Output Signals
    //---------------
    CREATE_PUBVAR(DbcMod1Out,         int);              // DB1 modulator switch gate command (1=on, 0=off)
    CREATE_PUBVAR(DbcMod2Out,         int);              // DB2 modulator switch gate command (1=on, 0=off)
    CREATE_PUBVAR(DbcVdb1VdcMisFlg,   unsigned);         // flag indicates that the actual DB1 voltage mismatch the expected one in DB cell test
    CREATE_PUBVAR(DbcVdb2VdcMisFlg,   unsigned);         // flag indicates that the actual DB2 voltage mismatch the expected one in DB cell test
    CREATE_PUBVAR(DbcDb1MisOffTstFlg, unsigned);         // DB1 mistakenly off flag in DB cell test
    CREATE_PUBVAR(DbcDb1MisOnTstFlg,  unsigned);         // DB1 mistakenly on flag in DB cell test
    CREATE_PUBVAR(DbcDb2MisOffTstFlg, unsigned);         // DB2 mistakenly off flag in DB cell test
    CREATE_PUBVAR(DbcDb2MisOnTstFlg,  unsigned);         // DB2 mistakenly on flag in DB cell test
    CREATE_PUBVAR(DbcDb1AutoTstDone,  unsigned);         // Db1 auto ping is done
    CREATE_PUBVAR(DbcDb2AutoTstDone,  unsigned);         // Db2 auto ping is done
    CREATE_PUBVAR(DbcVregRef,       float);      // DbVreg reference (volt)
//-----------------------------------------------------------------------------
private:


    // External Inputs
    //----------------
    InListTyp    In;
    InsListTyp   Ins;

    // Methods
    //--------
    void DbcPrecondition(void);
    void DbcVreg(void);
    void DbcModulator( float DbcModCmd, unsigned DbcGateSwFirst, float* DbcModPktInt, float* DbcModOutInt, int* DbcModOut );
    void DbcVdbFbkCheck( int DbcModOutZ2, int DbcModOutZ3, float DbcVdbFbk, unsigned DbcGateSkewEnb, unsigned DbFpgaDsb,
                         float DbVmutualComp, unsigned* DbcVdbVdcTrpFlg, unsigned* DbcVdbVdcMisFlg, float* DbcVdbExpect, float* DbcVdbErr );
    void DbcVdbStateCheck( int DbcDbStCmdZ2, int DbcDbcDbSwFbkSt, unsigned* DbcDbMisOffTrpFlg, unsigned* DbcDbMisOnTrpFlg, unsigned* DbcDbMisOffTstFlg, unsigned* DbcDbMisOnTstFlg );


    // Diagnostics
    //------------
    CREATE_DIAG(DbcOpnLoopAlm);                  // DB open loop alarm: warn operators DB is operates in open loop
    CREATE_DIAG(DbcVdbVdcTrp);                   // DB voltage mismatch trip
    CREATE_DIAG(DbcDbMisOffTrp);                 // DB mistakenly off trip
    CREATE_DIAG(DbcDbMisOnTrp);                  // DB mistakenly on trip


    // Parameters
    //-----------
    // V Reg
    CREATE_PARM(DbcVrefHt,          float);      // Db regulator reference when transistor hot
    CREATE_PARM(DbcVregIntLimP,     float);      // integral path max clamp
    CREATE_PARM(DbcVregIntLimN,     float);      // integral path min clamp
    CREATE_PARM(DbcVregBwRatio,     float);      // ratio of the maximum bandwidth
    CREATE_PARM(DbcVregStepAdr,     float*);     // address of the DbVreg step reference
    CREATE_PARM(DbcVregBw,          float);      // DbVreg bandwidth (rad/sec)
    CREATE_PARM(DbcVregPrpGn,       float);      // DbVreg proportional path gain (rad)
    CREATE_PARM(DbcVregIntGn,       float);      // DbVreg integral path gain (rad)

    // Modulator
    CREATE_PARM(DbcModDutyMax,      float);      // max limit on duty cycle command (1.0 = 100%)
    CREATE_PARM(DbcModOffs,         float);      // phase shift of 2nd gated DB modulator to 1st gated DB modulator (0.5 = 180 degrees)
    CREATE_PARM(DbcModPktSize,      float);      // the duration of the DB stays on during normal firing (N DelTm1)

    // auto ping
    CREATE_PARM(DbcAutoTstPwrAvg,   float);      // average power allowed for each auto ping of DB over one ping interval(watts)
    CREATE_PARM(DbcAutoTstDutySetp, float);      // duty cycle reference for DB during auto ping: normal Db auto ping duty calculated
    CREATE_PARM(DbcDbPwrMax,        float);      // maximum power loss of DB ( assume happens when DC link is the max) (watts)
    CREATE_PARM(DbcAutoTstIntv,     float);      // time between two auto DB pack rising (or falling) edges (Sec)
    CREATE_PARM(DbcAutoTstPktSize,  float);      // the duration of the DB stays on during auto (N DelTm1)
	CREATE_PARM(DbcAutoTstVdcMin,   float);      // min voltage which allows DB duty during auto

    // DB control test
    CREATE_PARM(DbcOpnLoopEnb,      unsigned);   // enable the open loop test (opent the loop between Vreg and Modualtor)
    CREATE_PARM(DbcVregExtRefEnb,   unsigned);   // enable to use the external ref for the Vreg
    CREATE_PARM(DbcOpnLpDutyAdr,    float*);     // open loop test ducy cycle reference adress
    CREATE_PARM(DbcVregExtRefAdr,   float*);     // the external ref for the Vreg address

    // DB voltage check
    CREATE_PARM( DbcVfbkAdj,         float);     // Dynamic brake feedback adjust
    CREATE_PARM( DbcVdbVdcThrsh,     float);     // Dynamic brake voltage error threshod (volt)

    CREATE_PARM( DbcGateSkewTm,      float);     // time to skew the DB gate command (sec)
    CREATE_PARM( DbVmutualCompPu,    float);     // mutual excitation compensation when only one DB is gated


    // Published Variables
    //--------------------
    // Modulator
    CREATE_PUBVAR(DbcMod1PktInt,     float);     // DB modulator duty cycle integrator ( may need a'double' to support very small modulation cmds e.g. for ping test)
    CREATE_PUBVAR(DbcMod1OutInt,     float);     // the integrator of the float version of the modulator output
    CREATE_PUBVAR(DbcModPktSize,     float);     // actual pkt size used by the modulator
    CREATE_PUBVAR(DbcMod2PktInt,     float);     // DB modulator duty cycle integral ( May need a'double' to support very small modulation cmds e.g. for ping test)
    CREATE_PUBVAR(DbcMod2OutInt,     float);     // the integrator of the float version of the modulator output

    // V Reg

    CREATE_PUBVAR(DbcVregOut,       float);      // DbVreg output: duty cyle (1.0 is 100% on, 0.0 is 0% on)
    CREATE_PUBVAR(DbcVregPrp,       float);      // proportionl path
    CREATE_PUBVAR(DbcVregInt,       float);      // integral path
    CREATE_PUBVAR(DbcVregErr,       float);      // error   (volt)
    CREATE_PUBVAR(DbcVregCmd,       float);      // command (volt)
    CREATE_PUBVAR(DbcVregStp,       float);      // DbVreg step reference (volt)
    CREATE_PUBVAR(DbcVregIntGnDt,   float);      // DbcVregIntGn * DelTm1 (rad * Sec)

    // DB voltage check
    CREATE_PUBVAR(DbcVdb1Expect,    float);        // DB1 voltage expected (volt)
    CREATE_PUBVAR(DbcVdb2Expect,    float);        // DB2 voltage expected (volt)
    CREATE_PUBVAR(DbcVdb1VdcErr,    float);        // error between the actual DB1 votage and the expected one (volt)
    CREATE_PUBVAR(DbcVdb2VdcErr,    float);        // error between the actual DB2 votage and the expected one (volt)
    CREATE_PUBVAR(DbcVdb1VdcTrpFlg, unsigned);     // trip flag indicates that the actual DB1 voltage mismatch the expected one
    CREATE_PUBVAR(DbcVdb2VdcTrpFlg, unsigned);     // trip flag indicates that the actual DB2 voltage mismatch the expected one
    CREATE_PUBVAR(Db1VmutualComp,   float);        // Db voltage 1 mutual compensation (Volts)
    CREATE_PUBVAR(Db2VmutualComp,   float);        // Db voltage 2 mutual compensation (Volts)

    // DB switch status check
    CREATE_PUBVAR(DbcDb1MisOffTrpFlg,  unsigned);     // DB1 mistakenly off trip flag
    CREATE_PUBVAR(DbcDb1MisOnTrpFlg,   unsigned);     // DB1 mistakenly on trip flag
    CREATE_PUBVAR(DbcDb2MisOffTrpFlg,  unsigned);     // DB2 mistakenly off trip flag
    CREATE_PUBVAR(DbcDb2MisOnTrpFlg,   unsigned);     // DB2 mistakenly on trip flag

    // delayed variables
    CREATE_PUBVAR(DbcMod1OutZ1,        int);          // delayed Db1 command Z1
    CREATE_PUBVAR(DbcMod1OutZ2,        int);          // delayed Db1 command Z2
    CREATE_PUBVAR(DbcMod1OutZ3,        int);          // delayed Db1 command Z3
    CREATE_PUBVAR(DbcMod2OutZ1,        int);          // delayed Db2 command Z1
    CREATE_PUBVAR(DbcMod2OutZ2,        int);          // delayed Db2 command Z2
    CREATE_PUBVAR(DbcMod2OutZ3,        int);          // delayed Db2 command Z3

    // others
    CREATE_PUBVAR(DbcGateDb1First,     unsigned);     // DB1 gate switch first on
    CREATE_PUBVAR(DbcDutyRef,          float);        // DB Duty ref after clamp by the auto ping (1.0 is 100% on, 0.0 is 0% on)
    CREATE_PUBVAR(DbcDutyCmd,          float);        // DB Duty cmd to DB modulator after condition on DB operarion mode (1.0 is 100% on, 0.0 is 0% on)
    CREATE_PUBVAR(DbcOpnLoopDutyRef,   float);        // open loop test ducy cycle reference
    CREATE_PUBVAR(DbcVregExtRef,       float);        // the external ref for the Vreg
    CREATE_PUBVAR(DbFltDsbTmr,         float);        // DB fpga disable timer (sec)
    CREATE_PUBVAR(DbFpgaDsb,           unsigned);     // DB fpga disable
    CREATE_PUBVAR(DbcAutoTstCnt,       int);          // counter for the forced DB auto test duration
    CREATE_PUBVAR(DbcAutoTstDuty,    float);         // DB auto test duty reference
    CREATE_PUBVAR(DbcAutoTstDutyT3,  float);         // DB auto test duty reference
    // Unpublished Variables
    //----------------------
    float    DbcPlantGn;
    unsigned DbAutoTstEnbZ1;


    // Data Passing
    //-------------
    DATA_PASS(DbcAutoTstDutyT3,   DbcAutoTstDuty,   T3_T1, UNFILTERED);
};



// Inline Function Definitions
//----------------------------


#endif  // B0_DB_CTL_H
