///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Thermistor processing.
//
//
// DESCRIPTION
//              Constants, function prototypes for thermistor processing.
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      p0Thrmstr.vsd, p0Thrmstr.xls
//
///////////////////////////////////////////////////////////////////////////////

#ifndef P0_THRMSTR_H
#define P0_THRMSTR_H


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product


// Constants
//----------
//Thermisters 1,3,4
#define LOW_RNG_VLT_MAX          4.94F
#define LOW_RNG_VLT_MIN          0.000001F
#define LOW_RNG_VLT_PT1          0.06F
#define LOW_TEMP_DATA_ARRAY_SIZE 246
#define LOW_TEMP_DATA_PTS        (LOW_TEMP_DATA_ARRAY_SIZE-1)
#define LWPT0                    157.05F                                                         //First table member (negated in table)
#define LWPT1                    63.35F                                                          //Second table member (negated in table)
#define LOW_RNG_TBL_DELP         ((LOW_TEMP_DATA_PTS-1)/(LOW_RNG_VLT_MAX - LOW_RNG_VLT_PT1))     //Note first point not used here, so -2
#define LOW_DEL_PT1_VOLT         ((LWPT0 - LWPT1)/(LOW_RNG_VLT_PT1 - LOW_RNG_VLT_MIN))           //Two number here are first two in table

#define HI_RNG_VLT_MAX           3.0F
#define HI_RNG_VLT_MIN           0.02F
#define HI_TEMP_DATA_ARRAY_SIZE  150
#define HI_TEMP_DATA_PTS         (HI_TEMP_DATA_ARRAY_SIZE-1)
#define HI_RNG_TBL_DELP          ((HI_TEMP_DATA_PTS)/(HI_RNG_VLT_MAX - HI_RNG_VLT_MIN))           //Delta points per volt = 1/0.02 = 50

#define V_IN_NOM           5.0F
#define R_HI_RANGE        406.87F
#define R_LOW_RANGE       22100.0F
#define R_CKT_SERIES       99.8F

// Function Prototypes
//--------------------


// Types and Classes
//------------------


///////////////////////////////////////////////////////////////////////////////
//
// CLASS DESCRIPTION:
//      Thermistor processing for one set of four thermistors, such as are
//      connected to an AEAD card.
//
///////////////////////////////////////////////////////////////////////////////
class cThrmstr
{
//-----------------------------------------------------------------------------
public:

    // Types and Classes
    //------------------
    struct InListTyp
    {
        const float&        TempVltTh1Lw;       //  1 - voltage at AEAD Din 1 Thermistor input, low range
        const float&        TempVltTh2Lw;       //  2 - voltage at AEAD Din 2 Thermistor input, low range
        const float&        TempVltTh7Lw;       //  3 - voltage at AEAD Din 7 Thermistor input, low range
        const float&        TempVltTh8Lw;       //  4 - voltage at AEAD Din 8 Thermistor input, low range
        const float&        TempVltTh1Hi;       //  5 - voltage at AEAD Din 1 Thermistor input, high range
        const float&        TempVltTh2Hi;       //  6 - voltage at AEAD Din 2 Thermistor input, high range
        const float&        TempVltTh7Hi;       //  7 - voltage at AEAD Din 7 Thermistor input, high range
        const float&        TempVltTh8Hi;       //  8 - voltage at AEAD Din 8 Thermistor input, high range
        const unsigned&     TempFbkVld;         //  9 - Temperature feedbacks are valid
        const unsigned&     UseThrmstr1;        // 10 - AEAD Din 1 Thermistor usage, true = used / false = unused
        const unsigned&     UseThrmstr2;        // 11 - AEAD Din 2 Thermistor input usage, true = used / false = unused
        const unsigned&     UseThrmstr7;        // 12 - AEAD Din 7 Thermistor input usage, true = used / false = unused
        const unsigned&     UseThrmstr8;        // 13 - AEAD Din 8 Thermistor input usage, true = used / false = unused
        const float&        InletAirTemp;       // 14 - Inlet air temperature as reference

    };

    struct InsListTyp
    {
        const float&        DelTm3;             //  1 - Delta time for task 3 (Sec)
        const unsigned&     TempFbkEnb;         //  2 - Feedback enable
    };

    // Methods
    //--------
    cThrmstr( InListTyp InList, InsListTyp InsList );
    ~cThrmstr();
    void Task3(void);
    static int  RunRules( float DeltaTime, float IoThrmstrW );


    // Output Signals
    //---------------
    CREATE_PUBVAR(Thrmstr1Temp  , float);    // AEAD Din 1 Thermistor temperature (deg C)
    CREATE_PUBVAR(Thrmstr2Temp  , float);    // AEAD Din 2 Thermistor temperature (deg C)
    CREATE_PUBVAR(Thrmstr7Temp  , float);    // AEAD Din 7 Thermistor temperature (deg C)
    CREATE_PUBVAR(Thrmstr8Temp  , float);    // AEAD Din 8 Thermistor temperature (deg C)

    CREATE_PUBVAR(IOThrm1DlFlg  , unsigned); // Flag for AEAD Din 1 Thermistor delta temperature alarm
    CREATE_PUBVAR(IOThrm2DlFlg  , unsigned); // Flag for AEAD Din 2 Thermistor delta temperature alarm
    CREATE_PUBVAR(IOThrm7DlFlg  , unsigned); // Flag for AEAD Din 7 Thermistor delta temperature alarm
    CREATE_PUBVAR(IOThrm8DlFlg  , unsigned); // Flag for AEAD Din 8 Thermistor delta temperature alarm

    CREATE_PUBVAR(Thrm1OpenFlg  , unsigned); // Flag for open AEAD Din 1 Thermistor
    CREATE_PUBVAR(Thrm2OpenFlg  , unsigned); // Flag for open AEAD Din 2 Thermistor
    CREATE_PUBVAR(Thrm7OpenFlg  , unsigned); // Flag for open AEAD Din 7 Thermistor
    CREATE_PUBVAR(Thrm8OpenFlg  , unsigned); // Flag for open AEAD Din 8 Thermistor

    CREATE_PUBVAR(Thrm1ShrtFlg  , unsigned); // Flag for short AEAD Din 1 Thermistor
    CREATE_PUBVAR(Thrm2ShrtFlg  , unsigned); // Flag for short AEAD Din 2 Thermistor
    CREATE_PUBVAR(Thrm7ShrtFlg  , unsigned); // Flag for short AEAD Din 7 Thermistor
    CREATE_PUBVAR(Thrm8ShrtFlg  , unsigned); // Flag for short AEAD Din 8 Thermistor

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
    void ThrmstrOpenCk( void );
    void ThrmstrShrtCk( void );


    // Diagnostics
    //------------
    CREATE_DIAG(IOThrm1DlAlm);              // AEAD Din 1 Thermistor excessive hi-lo range temp delta
    CREATE_DIAG(IOThrm2DlAlm);              // AEAD Din 2 Thermistor excessive hi-lo range temp delta
    CREATE_DIAG(IOThrm7DlAlm);              // AEAD Din 7 Thermistor excessive hi-lo range temp delta
    CREATE_DIAG(IOThrm8DlAlm);              // AEAD Din 8 Thermistor excessive hi-lo range temp delta

    CREATE_DIAG(Thrm1OpenAlm);              // 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0)  AEAD Din 1 Thermistor open alarm
    CREATE_DIAG(Thrm2OpenAlm);              // 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0)  AEAD Din 2 Thermistor open alarm
    CREATE_DIAG(Thrm7OpenAlm);              // 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0)  AEAD Din 7 Thermistor open alarm
    CREATE_DIAG(Thrm8OpenAlm);              // 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0)  AEAD Din 8 Thermistor open alarm

    CREATE_DIAG(Thrm1ShrtAlm);              // 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0)  AEAD Din 1 Thermistor short alarm
    CREATE_DIAG(Thrm2ShrtAlm);              // 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0)  AEAD Din 2 Thermistor short alarm
    CREATE_DIAG(Thrm7ShrtAlm);              // 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0)  AEAD Din 7 Thermistor short alarm
    CREATE_DIAG(Thrm8ShrtAlm);              // 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0)  AEAD Din 8 Thermistor short alarm


    // Parameters
    //-----------
    CREATE_OUR_PARM(ThrmTempLresMeld, float);   // Low temp range resistance threshold for beginning meld
    CREATE_OUR_PARM(ThrmTempHresMeld, float);   // High temp range resistance threshold for beginning meld
    CREATE_OUR_PARM(ThrmTempCkMax,    float);   // Maximum temp for allowance of high/low range temp fault check
    CREATE_OUR_PARM(ThrmTempCkMin,    float);   // Minimum temp for allowance of high/low range temp fault check
    CREATE_OUR_PARM(ThrmTempDelMax,   float);   // Maximum allowed temp delta between high and low range measurement
    CREATE_OUR_PARM(ThrmTempDelMaxC,  float);   // Maximum allowed temp delta between high and low range measurement in cold condition
    CREATE_OUR_PARM(ThrmTempDelFltTm, float);   // Activation time for the four temp sensor delta faults
    CREATE_OUR_PARM(ThrmShrtOnThrsh,  float);   // short thermistor voltage threshold for alarm on(volt)
    CREATE_OUR_PARM(ThrmOpenOnThrsh,  float);   // open thermistor voltage threshold for alarm on(volt)
    CREATE_OUR_PARM(ThrmShrtOffThrsh, float);   // short thermistor voltage threshold for alarm off(volt)
    CREATE_OUR_PARM(ThrmOpenOffThrsh, float);   // open thermistor voltage threshold for alarm off(volt)
    CREATE_OUR_PARM(ThrmTempDelSelThr,float);   // Threshold to select either ThrmTempDelMax or ThrmTempDelMaxC for delta temperature check

    // Published Variables
    //--------------------
    CREATE_PUBVAR(TempFilStlTmr , float);   // Settle timer for temperature voltage filters
    CREATE_PUBVAR(TempVTh1LwFl  , float);   // copy of the filter state: Voltage for Temp conversion, AEAD Din 1 Thermistor, low range
    CREATE_PUBVAR(TempVTh2LwFl  , float);   // copy of the filter state: Voltage for Temp conversion, AEAD Din 2 Thermistor, low range
    CREATE_PUBVAR(TempVTh7LwFl  , float);   // copy of the filter state: Voltage for Temp conversion, AEAD Din 7 Thermistor, low range
    CREATE_PUBVAR(TempVTh8LwFl  , float);   // copy of the filter state: Voltage for Temp conversion, AEAD Din 8 Thermistor, low range
    CREATE_PUBVAR(TempVTh1HiFl  , float);   // copy of the filter state: Voltage for Temp conversion, AEAD Din 1 Thermistor, high range
    CREATE_PUBVAR(TempVTh2HiFl  , float);   // copy of the filter state: Voltage for Temp conversion, AEAD Din 2 Thermistor, high range
    CREATE_PUBVAR(TempVTh7HiFl  , float);   // copy of the filter state: Voltage for Temp conversion, AEAD Din 7 Thermistor, high range
    CREATE_PUBVAR(TempVTh8HiFl  , float);   // copy of the filter state: Voltage for Temp conversion, AEAD Din 8 Thermistor, high range

    CREATE_PUBVAR(TempVTh1LwFlSt, float);   // filter state: Voltage for Temp conversion, AEAD Din 1 Thermistor, low range
    CREATE_PUBVAR(TempVTh2LwFlSt, float);   // filter state: Voltage for Temp conversion, AEAD Din 2 Thermistor, low range
    CREATE_PUBVAR(TempVTh7LwFlSt, float);   // filter state: Voltage for Temp conversion, AEAD Din 7 Thermistor, low range
    CREATE_PUBVAR(TempVTh8LwFlSt, float);   // filter state: Voltage for Temp conversion, AEAD Din 8 Thermistor, low range
    CREATE_PUBVAR(TempVTh1HiFlSt, float);   // filter state: Voltage for Temp conversion, AEAD Din 1 Thermistor, high range
    CREATE_PUBVAR(TempVTh2HiFlSt, float);   // filter state: Voltage for Temp conversion, AEAD Din 2 Thermistor, high range
    CREATE_PUBVAR(TempVTh7HiFlSt, float);   // filter state: Voltage for Temp conversion, AEAD Din 7 Thermistor, high range
    CREATE_PUBVAR(TempVTh8HiFlSt, float);   // filter state: Voltage for Temp conversion, AEAD Din 8 Thermistor, high range

    CREATE_PUBVAR(TempResTh1Lw  , float);   // Resistance for Temp converstion, AEAD Din 1 Thermistor, low range
    CREATE_PUBVAR(TempResTh2Lw  , float);   // Resistance for Temp converstion, AEAD Din 2 Thermistor, low range
    CREATE_PUBVAR(TempResTh7Lw  , float);   // Resistance for Temp converstion, AEAD Din 7 Thermistor, low range
    CREATE_PUBVAR(TempResTh8Lw  , float);   // Resistance for Temp converstion, AEAD Din 8 Thermistor, low range
    CREATE_PUBVAR(TempResTh1Hi  , float);   // Resistance for Temp converstion, AEAD Din 1 Thermistor, high range
    CREATE_PUBVAR(TempResTh2Hi  , float);   // Resistance for Temp converstion, AEAD Din 2 Thermistor, high range
    CREATE_PUBVAR(TempResTh7Hi  , float);   // Resistance for Temp converstion, AEAD Din 7 Thermistor, high range
    CREATE_PUBVAR(TempResTh8Hi  , float);   // Resistance for Temp converstion, AEAD Din 8 Thermistor, high range
    CREATE_PUBVAR(TempDelTh1    , float);   // Absolute value of temp hi/low delta for AEAD Din 1 Thermistor
    CREATE_PUBVAR(TempDelTh2    , float);   // Absolute value of temp hi/low delta for AEAD Din 2 Thermistor
    CREATE_PUBVAR(TempDelTh7    , float);   // Absolute value of temp hi/low delta for AEAD Din 7 Thermistor
    CREATE_PUBVAR(TempDelTh8    , float);   // Absolute value of temp hi/low delta for AEAD Din 8 Thermistor
    CREATE_PUBVAR(TempDlTmrTh1  , float);   // Timer for AEAD Din 1 Thermistor delta resitance fault
    CREATE_PUBVAR(TempDlTmrTh2  , float);   // Timer for AEAD Din 2 Thermistor delta resitance fault
    CREATE_PUBVAR(TempDlTmrTh7  , float);   // Timer for AEAD Din 7 Thermistor delta resitance fault
    CREATE_PUBVAR(TempDlTmrTh8  , float);   // Timer for AEAD Din 8 Thermistor delta resitance fault

    CREATE_PUBVAR(TempTh1Lw     , float);   // Temperature for AEAD Din 1 Thermistor, low range
    CREATE_PUBVAR(TempTh2Lw     , float);   // Temperature for AEAD Din 2 Thermistor, low range
    CREATE_PUBVAR(TempTh7Lw     , float);   // Temperature for AEAD Din 7 Thermistor, low range
    CREATE_PUBVAR(TempTh8Lw     , float);   // Temperature for AEAD Din 8 Thermistor, low range
    CREATE_PUBVAR(TempTh1Hi     , float);   // Temperature for AEAD Din 1 Thermistor, high range
    CREATE_PUBVAR(TempTh2Hi     , float);   // Temperature for AEAD Din 2 Thermistor, high range
    CREATE_PUBVAR(TempTh7Hi     , float);   // Temperature for AEAD Din 7 Thermistor, high range
    CREATE_PUBVAR(TempTh8Hi     , float);   // Temperature for AEAD Din 8 Thermistor, high range
    CREATE_PUBVAR(TempTh1Meld   , float);   // Temperature melded for AEAD Din 1 Thermistor
    CREATE_PUBVAR(TempTh2Meld   , float);   // Temperature melded for AEAD Din 2 Thermistor
    CREATE_PUBVAR(TempTh7Meld   , float);   // Temperature melded for AEAD Din 7 Thermistor
    CREATE_PUBVAR(TempTh8Meld   , float);   // Temperature melded for AEAD Din 8 Thermistor
    CREATE_PUBVAR(TempVTblTh1   , float);
    CREATE_PUBVAR(TempVTblTh2   , float);
    CREATE_PUBVAR(TempVTblTh7   , float);
    CREATE_PUBVAR(TempVTblTh8   , float);

    CREATE_PUBVAR(TempIdxF      , float);
    CREATE_PUBVAR(TempIdx       , float);
    CREATE_PUBVAR(TempIdxFrc    , float);
    CREATE_PUBVAR(TempPt1       , float);
    CREATE_PUBVAR(TempPt2       , float);
    CREATE_PUBVAR(Test20        , float);
    CREATE_PUBVAR(Test21        , float);
    CREATE_PUBVAR(TempIdxI      , int);
    CREATE_PUBVAR(TestIn20      , int);
    CREATE_PUBVAR(TestIn21      , int);
    
    CREATE_PUBVAR(ThrmTempDelMax, float);  // Dynamic maximum allowed temp delta between high and low range measurement

    CREATE_OUR_PUBVAR(TempVltGnFil , float);  // omega*dt for temperature voltage filters
    CREATE_OUR_PUBVAR(TempFilStlTm , float);  // Settle time for temperature voltage filters
    CREATE_OUR_PUBVAR(TempRMldRcp  , float);  // reciprocal of difference of Temp L & H meld parms

    // Unpublished Variables
    //----------------------
    static const float      TempTabLowT1[LOW_TEMP_DATA_ARRAY_SIZE];
    static const float      TempTabHighT1[HI_TEMP_DATA_ARRAY_SIZE];

    // Data Passing
    //-------------

};

#endif  // P0_THRMSTR_H
