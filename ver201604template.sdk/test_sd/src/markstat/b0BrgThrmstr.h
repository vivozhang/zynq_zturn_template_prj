///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Bridge semiconductor thermistor processing.
//
//
// DESCRIPTION
//              Constants, function prototypes for thermistor processing.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      b0BrgThrmstr.vsd, b0BrgThrmstr.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef B0_BRGTHRMSTR_H
#define B0_BRGTHRMSTR_H


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product


// Constants
//----------
#define  TEMP_VOLTS_TBL_MAX        3.96875F
#define  V_TEMP_FULL               4.0F        // 4 Volts full scale for Vco
#define  V_TEMP_FULL_RCP           0.25F
#define  TEMP_DATA_ARRAY_SIZE      129
#define  TEMP_DATA_PTS             (TEMP_DATA_ARRAY_SIZE-1)
#define  TEMP_DATA_PTS_RCP         0.0078125F


// Function Prototypes
//--------------------


// Types and Classes
//------------------


///////////////////////////////////////////////////////////////////////////////
//
// CLASS DESCRIPTION:
//      Bridge thermistor processing for one set of eight thermistors, six
//      of them monitoring bridge phases and two others associated with DB.
//
///////////////////////////////////////////////////////////////////////////////
class cBrgThrmstr
{
//-----------------------------------------------------------------------------
public:

    // Types and Classes
    //------------------
    struct InListTyp
    {                                           // All task3 signals
        const float&        ThrmstrVolt1;       //  1 - voltage at thermistor input 1
        const float&        ThrmstrVolt2;       //  2 - voltage at thermistor input 2
        const float&        ThrmstrVolt3;       //  3 - voltage at thermistor input 3
        const float&        ThrmstrVolt4;       //  4 - voltage at thermistor input 4
        const float&        ThrmstrVolt5;       //  5 - voltage at thermistor input 5
        const float&        ThrmstrVolt6;       //  6 - voltage at thermistor input 6
        const float&        ThrmstrVolt7;       //  7 - voltage at thermistor input 7
        const float&        ThrmstrVolt8;       //  8 - voltage at thermistor input 8
        const unsigned&     TempFbkVld;         //  9 - Temperature feedbacks are valid
        const unsigned&     TempRdyPerm;        // 10 - Temperature ready permissive
        const unsigned&     Stopped;            // 11 - Stopped state indication
    };

    struct InsListTyp
    {
        const unsigned&     DbEnb;              //  1 - Enable DB thermistor diagnostics
        const float&        DelTm3;             //  2 - Delta time for task 3 (Sec)
              float&        Temp7Ref;           //  3 - Ref to nonconstant data for optional thermistor 7
              float&        Temp8Ref;           //  4 - Ref to nonconstant data for optional thermistor 8
        const uint&         IgbtParallel;       //  5 - Parallel IGBTs for 'true'
        const char*         Uniquifier;         //  6 - Class instance uniquifier string
    };

    // Methods
    //--------
    cBrgThrmstr( InListTyp InList, InsListTyp InsList );
    ~cBrgThrmstr();
    void Task3(void);
    int RunRules( void );                                       // For privately owned rules
    static int  RunRules( float DeltaTime, float IoThrmstrW );  // For rules common to all instances


    // Output Signals
    //---------------
    CREATE_PUBVAR(BthtrTempMaxA,        float);    // Maximum thermistor temperature in phase A (degC)
    CREATE_PUBVAR(BthtrTempMaxB,        float);    // Maximum thermistor temperature in phase B (degC)
    CREATE_PUBVAR(BthtrTempMaxC,        float);    // Maximum thermistor temperature in phase C (degC)
    CREATE_PUBVAR(BthtrTempMaxDb,       float);    // Maximum thermistor temperature in dynamic brake (degC)
    CREATE_PUBVAR(BthtrTempMax,         float);    // Thermistor A-B-C maximum temperature (degC)
    CREATE_PUBVAR(BthtrTempMinA,        float);    // Minimum thermistor temperature in phase A (degC)
    CREATE_PUBVAR(BthtrTempMinB,        float);    // Minimum thermistor temperature in phase B (degC)
    CREATE_PUBVAR(BthtrTempMinC,        float);    // Minimum thermistor temperature in phase C (degC)
    CREATE_PUBVAR(BthtrTempMinD,        float);    // Minimum thermistor temperature in phase D (degC)
    CREATE_PUBVAR(BthtrTemp1,           float);    // Thermistor 1 temperature - phase A top (deg C)
    CREATE_PUBVAR(BthtrTemp2,           float);    // Thermistor 2 temperature - phase B top (deg C)
    CREATE_PUBVAR(BthtrTemp3,           float);    // Thermistor 3 temperature - phase C top (deg C)
    CREATE_PUBVAR(BthtrTemp4,           float);    // Thermistor 4 temperature - phase A bottom (deg C)
    CREATE_PUBVAR(BthtrTemp5,           float);    // Thermistor 5 temperature - phase B bottom (deg C)
    CREATE_PUBVAR(BthtrTemp6,           float);    // Thermistor 6 temperature - phase C bottom (deg C)
    CREATE_PUBVAR(BthtrOtAlarmSum,      unsigned); // Thermistor over-temperature alarm summary
    CREATE_PUBVAR(BthtrOtFltDlySum,     unsigned); // Thermistor over-temperature trip summary
    CREATE_PUBVAR(BthtrImbMax,          float);    // Maximum phase-phase imblance current (Amp)
    CREATE_OUR_PUBVAR(BthtrPhsImblncOn, float);    // Phase thermistor imbalance alarm On setting (degC)
    CREATE_OUR_PUBVAR(BthtrPhsImblncOff,float);    // Phase thermistor imbalance alarm Off setting (degC)

//-----------------------------------------------------------------------------
private:
    friend class cVariable;
    friend class cParameter;
    friend class cDataPasser;
    friend class cMarkStatDiag;
    friend class cThrmstrSim;

    // Types and Classes
    //------------------

    // External Inputs
    //----------------
    InListTyp    In;
    InsListTyp   Ins;

    // Methods
    //--------
    float VoltToTemp( float& VoltIn );      // Translates voltage into temperature using a look-up table


    // Diagnostics
    //------------
    CREATE_DIAG(BthtrAphsUtAlm);            // Phase A under temperature alarm
    CREATE_DIAG(BthtrBphsUtAlm);            // Phase B under temperature alarm
    CREATE_DIAG(BthtrCphsUtAlm);            // Phase C under temperature alarm
    CREATE_DIAG(BthtrDbUtAlm);              // Dynamic Brake under temperature alarm
    CREATE_DIAG(BthtrUtTrp);                // Thermistor under temperature trip
    CREATE_DIAG(BthtrAphTopUtTrp);          // Phase A top thermister under temp trip
    CREATE_DIAG(BthtrAphBotUtTrp);          // Phase A bottom thermister under temp trip
    CREATE_DIAG(BthtrBphTopUtTrp);          // Phase B top thermister under temp trip
    CREATE_DIAG(BthtrBphBotUtTrp);          // Phase B bottom thermister under temp trip
    CREATE_DIAG(BthtrCphTopUtTrp);          // Phase C top thermister under temp trip
    CREATE_DIAG(BthtrCphBotUtTrp);          // Phase C bottom thermister under temp trip
    CREATE_DIAG(BthtrDbTopUtTrp);           // Dynamic break top thermister under temp trip
    CREATE_DIAG(BthtrDbBotUtTrp);           // Dynamic break bottom thermister under temp trip
    CREATE_DIAG(BthtrAphsOtAlm);            // Phase A over temperature alarm
    CREATE_DIAG(BthtrBphsOtAlm);            // Phase B over temperature alarm
    CREATE_DIAG(BthtrCphsOtAlm);            // Phase C over temperature alarm
    CREATE_DIAG(BthtrDbOtAlm);              // Dynamic Brake over temperature alarm
    CREATE_DIAG(BthtrAphsOtDlyTrp);         // Phase A over temperature trip
    CREATE_DIAG(BthtrBphsOtDlyTrp);         // Phase B over temperature trip
    CREATE_DIAG(BthtrCphsOtDlyTrp);         // Phase C over temperature trip
    CREATE_DIAG(BthtrOtTrp);                // Max thermister temperature instantaneous trip 
    CREATE_DIAG(BthtrDbOtDlyTrp);           // Dynamic Brake over temperature trip
    CREATE_DIAG(BthtrAphsImbAlm);           // Out-voted phase A thermistor imbalance alarm
    CREATE_DIAG(BthtrBphsImbAlm);           // Out-voted phase B thermistor imbalance alarm
    CREATE_DIAG(BthtrCphsImbAlm);           // Out-voted phase C thermistor imbalance alarm
    CREATE_DIAG(BthtrAdelImbAlm);           // Delta thermistor within phase A imbalance alarm
    CREATE_DIAG(BthtrBdelImbAlm);           // Delta thermistor within phase B imbalance alarm
    CREATE_DIAG(BthtrCdelImbAlm);           // Delta thermistor within phase C imbalance alarm
    CREATE_DIAG(BthtrDbDelImbAlm);          // Delta thermistor within dynamic brake imbalance alarm

    // Parameters
    //-----------
    CREATE_OUR_PARM(BthtrUtOn,      float); // Thermistor under temperature alarm On level (degC)
    CREATE_OUR_PARM(BthtrUtOff,     float); // Thermistor under temperature alarm Off level (degC)
    CREATE_OUR_PARM(BthtrUtThrs,    float); // Thermistor under temperature trip threshold (degC)
    CREATE_OUR_PARM(BthtrOtThrs,    float); // Thermistor over temperature trip threshold (degC) 
    CREATE_OUR_PARM(BthtrOtOn,      float); // Thermistor over temperature alarm On level (degC)
    CREATE_OUR_PARM(BthtrOtOff,     float); // Thermistor over temperature alarm Off level (degC)
    CREATE_OUR_PARM(BthtrOtDlyThrs, float); // Thermistor over temperature delay trip threshold (degC)
    CREATE_PARM(BthtrImbOn,         float); // Thermistor imbalance alarm On level (degC)
    CREATE_PARM(BthtrImbOff,        float); // Thermistor imbalance alarm Off level (degC)
    CREATE_OUR_PARM(BthtrPhsImbOn,  float); // Phase thermistor imbalance alarm On level (degC)
    CREATE_OUR_PARM(BthtrPhsImbOff, float); // Phase thermistor imbalance alarm Off level (degC)
    CREATE_PARM(BthtrDbEnb,  unsigned);     // Enable DB thermistor diagnostics

    // Published Variables
    //--------------------
    CREATE_PUBVAR(BthtrVtempFil1, float);    // Filtered version of input 1 voltage (Volts)
    CREATE_PUBVAR(BthtrVtempFil2, float);    // Filtered version of input 2 voltage (Volts)
    CREATE_PUBVAR(BthtrVtempFil3, float);    // Filtered version of input 3 voltage (Volts)
    CREATE_PUBVAR(BthtrVtempFil4, float);    // Filtered version of input 4 voltage (Volts)
    CREATE_PUBVAR(BthtrVtempFil5, float);    // Filtered version of input 5 voltage (Volts)
    CREATE_PUBVAR(BthtrVtempFil6, float);    // Filtered version of input 6 voltage (Volts)
    CREATE_PUBVAR(BthtrVtempFil7, float);    // Filtered version of input 7 voltage (Volts)
    CREATE_PUBVAR(BthtrVtempFil8, float);    // Filtered version of input 8 voltage (Volts)

    CREATE_PUBVAR(BthtrUtFlgA, unsigned);    // Under temperature flag phase A
    CREATE_PUBVAR(BthtrUtFlgB, unsigned);    // Under temperature flag phase B
    CREATE_PUBVAR(BthtrUtFlgC, unsigned);    // Under temperature flag phase C
    CREATE_PUBVAR(BthtrUtFlgD, unsigned);    // Under temperature flag Dynamic Brake

    CREATE_PUBVAR(BthtrOtFlgA, unsigned);    // Over temperature flag phase A
    CREATE_PUBVAR(BthtrOtFlgB, unsigned);    // Over temperature flag phase B
    CREATE_PUBVAR(BthtrOtFlgC, unsigned);    // Over temperature flag phase C
    CREATE_PUBVAR(BthtrOtFlgD, unsigned);    // Over temperature flag Dynamic Brake

    CREATE_PUBVAR(BthtrImbFlgAB, unsigned);  // Imbalance flag between phase A and B
    CREATE_PUBVAR(BthtrImbFlgBC, unsigned);  // Imbalance flag between phase B and C
    CREATE_PUBVAR(BthtrImbFlgCA, unsigned);  // Imbalance flag between phase C and A
    CREATE_PUBVAR(BthtrImbFlgA,  unsigned);  // Imbalance flag within phase A
    CREATE_PUBVAR(BthtrImbFlgB,  unsigned);  // Imbalance flag within phase B
    CREATE_PUBVAR(BthtrImbFlgC,  unsigned);  // Imbalance flag within phase C
    CREATE_PUBVAR(BthtrImbFlgD,  unsigned);  // Imbalance flag within phase D

    CREATE_OUR_PUBVAR(BthtrVltGn,    float); // Omega*dt for temperature voltage filters
    CREATE_OUR_PUBVAR(BthtrFilStlTm, float); // Settle time for thermistor voltage filters (Sec)

    CREATE_PUBVAR(BthtrFilStlTmr,    float); // Filter settling timer (Sec)
    CREATE_PUBVAR(BthtrTmp12Del,     float); // Phase A to B phase imblance current (Amp)
    CREATE_PUBVAR(BthtrTmp23Del,     float); // Phase B to C phase imblance current (Amp)
    CREATE_PUBVAR(BthtrTmp13Del,     float); // Phase C to A phase imblance current (Amp)


    // Unpublished Variables
    //----------------------
    static const float      TempTable[TEMP_DATA_ARRAY_SIZE];

    // Data Passing
    //-------------

};


// Inline Function Definitions
//----------------------------


#endif  // B0_BRGTHTRMSTR_H
