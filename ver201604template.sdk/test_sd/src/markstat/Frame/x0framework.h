///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       MarkStat Framework Defintions
//
// DESCRIPTION:
//      This file contains constants of nature and universal macros
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      List locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __X0_FRAME_WORK_H__
#define __X0_FRAME_WORK_H__

// Includes Files
//---------------
// system
#include <math.h>       // fpclassify
#include <float.h>      // temporary until start using newest NaNRoutines from Core
#include <stdint.h>     // temporary until start using newest NaNRoutines from Core
#include <cmath>
// core
#include "NaNRoutines.h"

// Parameter, variable, diagnostic automation
#include "x0AutoDefinition.h"
#ifndef PRE_PROC
    #include "ParmExtern.h"
    #include "VarLadr.h"
    #include "DiagExtern.h"
#endif


// Thread Identification
#define MARKSTAT_MAIN_THREAD            "MKS: MAIN"
#define MARKSTAT_APPIF_THREAD           "MKS: APP INTERFACE"
#define MARKSTAT_LLMON_THREAD           "MKS: LL MONITOR"
#define MARKSTAT_ADLSRVR_THREAD         "MKS: ADL SERVER"
#define MARKSTAT_MSGSVR_THREAD          "MKS: MSG SERVER"
#define MARKSTAT_ADTCONN_THREAD         "MKS: ADT CONNECT"
#define MARKSTAT_ADTFAULT_THREAD        "MKS: ADT FAULT"
#define MARKSTAT_FLASH_THREAD           "MKS: FLASH FILE SERVER"
#define MARKSTAT_XOPS_SCHED_THREAD      "MKS: XOPS_SCHED"
#define MARKSTAT_XOPS_TASK1_THREAD      "MKS: XOPS_TASK1"
#define MARKSTAT_XOPS_TASK2_THREAD      "MKS: XOPS_TASK2"
#define MARKSTAT_XOPS_TASK3_THREAD      "MKS: XOPS_TASK3"
#define MARKSTAT_XOPS_BKGND_THREAD      "MKS: XOPS_BACKGROUND"
#define MARKSTAT_SEQ_CLIENT_APP_THREAD  "MKS: SEQ_CLIENT_APP"
#define MARKSTAT_SEQ_CLIENT_OUT_THREAD  "MKS: SEQ_CLIENT_OUT"

// switches for xops optimization experiments
#if 0
#define XOPS_PERF_MON_TASK
#define XOPS_TRACE_TASK
#define XOPS_EX2100_SCHED_METHOD
#define XOPS_DISTRIBUTED_TASK_CONTROL
#define XOPS_FPE
#endif

// undefine constants defined by <math.h>
#ifdef PI
#undef PI
#endif

#ifdef PI2
#undef PI2
#endif

#ifdef NULL
#undef NULL
#endif

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

// circuit indices
#define PH1     0
#define PH2     1
#define PH3     2
#define NPH     3

// status
#define TRUE    1
#define FALSE   0

#define ON      1
#define OFF     0

#define HI      1
#define LOW     0

#define NULL    0

#define SUCCESS 1
#define FAIL    0
#define BUSY    -1

// macros to access class input/output strutures
#ifndef PRE_PROC
    #define IN(Name)        (In.Name)
    #define INS(Name)       (Ins.Name)
    #define OUT(Name)       (Name)
    #define OUT2(Name)      (a##Name)	// temporary definition
    #define PIN(Name)       (*ProdStatIf.Name)
#endif

// universal constants
#define PI          3.14159265359F
#define SQRT2       1.41421356237F
#define SQRT3       1.73205080757F

#define PI2         (2.0F*PI)
#define SQRT2_RCP   (1.0F/SQRT2)
#define SQRT3_RCP   (1.0F/SQRT3)
#define SQRT2_3	    (SQRT2/SQRT3)

// universal conversions
#define PK_TO_RMS   (1.0F/SQRT2)    // peak to rms
#define RMS_TO_PK   SQRT2           // rms to peak
#define LL_TO_LN    (1.0F/SQRT3)    // line-line to line-neutral
#define LN_TO_LL    SQRT3           // line-neutral to line-line
#define RPM_TO_RAD  (PI2/60.0F)     // RPM to rad/sec
#define RAD_TO_RPM  (60.0F/PI2)     // rad/sec to RPM
#define HZ_TO_RAD   PI2             // Hz to rad/sec
#define RAD_TO_HZ   (1.0F/PI2)      // rad/sec to Hz
#define DEG_TO_RAD  (PI2/360.0F)    // deg to rad
#define RAD_TO_DEG  (360.0F/PI2)    // rad to deg


#define Q12(x,fs) ((fs)/2047.0*(int)((x)/(fs)*2047.0))

// universal macros
#define SQR(x)          ( (x) * (x) )
#define ABS(x)          ( (x) >= 0 ? (x) : -(x) )
#define MAX(a,b)        ( (a) >= (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) <= (b) ? (a) : (b) )
#define SGN(a)          ( (a) >= 0 ? 1 : -1 )
#define SGNZ(x)         ( (x) > 0 ? 1 : (x) < 0 ? -1 : 0 )
#define HYPOT(a,b)      sqrtf(a*a+b*b)

#define CLAMP(var,val)  var = F_CLAMP(var,val)
#define F_CLAMP(x,val)  ( (x) > (val) ? (val) : (x) )

#define SCLAMP(var,val) var = F_SCLAMP(var,val)
#define F_SCLAMP(x,val) ( (x) > (val) ? (val) : (x) < -(val) ? -(val) : (x) )

// SWMOD WARNING!  MUST NOT COMPILE WITH "-mc" OPTION WHEN USING "cl30" !!!
#define SWMOD(var)      var = F_SWMOD(var)
#define F_SWMOD(x) \
( \
 ((x) > 0) ? ( ( (x) + PI ) / PI2 - (int) ( ( (x) + PI ) / PI2 ) ) * PI2 - PI \
           : ( ( (x) - PI ) / PI2 + (int) ( ( PI - (x) ) / PI2 ) ) * PI2 + PI \
)

// WMOD WARNING!  MUST NOT COMPILE WITH "-mc" OPTION WHEN USING "cl30" !!!
#define WMOD(var)       var = F_WMOD(var)
#define F_WMOD(x) \
( \
 ((x) > 0) ? ( (x) / PI2 - (int) ( (x) / PI2 ) ) * PI2 \
           : ( (x) / PI2 - (int) ( (x) / PI2 ) ) * PI2 + PI2 \
)

// ASWMOD WARNING!  MUST NOT COMPILE WITH "-mc" OPTION WHEN USING "cl30" !!!
#define ASWMOD(var) do { SWMOD(var); x = ABS(var); } while (0)

// only works on static arrays
#define ARRAY_LEN(x)   ( sizeof((x)) / sizeof((x)[0]) )


// NaN/Infinity protection with hi/lo limit
inline float NoNANLim(float in, float hilim, float lolim)
{
    float temp = cNaNCheck::NoNAN(in);
    return (temp > hilim) ? hilim : (temp < lolim) ? lolim : temp;
}

// flush de-normal floats to true zero to avoid excessive computation time in fpu
//  Note - ANTI_DENORM takes approx 4x time to execute compared to ANTI_DENORM_F
#define ANTI_DENORM(x)     ( x = ( fpclassify(x) == FP_SUBNORMAL) ? 0.0 : x )
#define ANTI_DENORM_F(x)   ( x = ( *reinterpret_cast<uint32_t*>(&x) &         0x7F800000   ) == 0x0 ? 0.0 : x )    // test  8 bit exponent for zero
#define ANTI_DENORM_D(x)   ( x = ( *reinterpret_cast<uint64_t*>(&x) & 0x7FF0000000000000LL ) == 0x0 ? 0.0 : x )    // test 11 bit exponent for zero
#ifdef  __X86__
    // set the Flush-To-Zero (FTZ) and Denormals-Are-Zero (DAZ) bits in MXCSR
    // to get the floating point unit to treat de-normalized floats as zero
    #define DENORM_OFF                                  \
        { unsigned new_mxcsr;                           \
        __asm__ ("stmxcsr %0" : "=m" (*&new_mxcsr));    \
        new_mxcsr |= 0x00008040;                        \
        __asm__ ("ldmxcsr %0" : : "m" (*&new_mxcsr)); }
#else
    #define DENORM_OFF
#endif



#define LimitHiLo(x, hi, lo)    if ((x) < (lo)) (x) = (lo); \
        if ((x) > (hi)) (x) = (hi)


#define OS_ENTER_CRITICAL()  InterruptDisable()
#define OS_EXIT_CRITICAL()   InterruptEnable()


// depricate Delta_Tm, use DELTA_TM instead
#define Delta_Tm(New, Old)  ( (float)(New.Sec - Old.Sec) + 1e-9*((float)(New.nSec - Old.nSec)) )
#define DELTA_TM(New, Old)  ( (float)(New.Sec - Old.Sec) + 1e-9*((float)(New.nSec - Old.nSec)) )



struct Code_ID_struct
{
    char        string[20];
    unsigned    version;
    unsigned    time;
    unsigned    date;
};

struct SysTm_Typ
{
    volatile int Sec;
    volatile int nSec;
};

struct  SinCosTyp
{
    float   sin;
    float   cos;
};

struct ProductStatusAddrTyp
{
    const unsigned* Stopped;

    const unsigned* Brg1Ready;
    const unsigned* Brg1Enabled;
    const unsigned* Brg1Running;
    const unsigned* Brg1Stopped;

    const unsigned* Brg2Ready;
    const unsigned* Brg2Enabled;
    const unsigned* Brg2Running;
    const unsigned* Brg2Stopped;
};

// index for XmsProbe[] array
enum XmsPidTyp
{
    XMS_PID_0   =  0,
    XMS_PID_1   =  1,
    XMS_PID_2   =  2,
    XMS_PID_3   =  3,
    XMS_PID_4   =  4,
    XMS_PID_5   =  5,
    XMS_PID_6   =  6,
    XMS_PID_7   =  7,
    XMS_PID_8   =  8,
    XMS_PID_9   =  9,
    XMS_PID_LEN = 10  // used to dimension XmsProbe[] !!! WHEN CHANGE ARRAY SIZE MUST INIT ALL XmsProbe[].pXmsProbe pointers
};


extern unsigned             TripFlt, Brg1TripFlt, Brg2TripFlt;

extern const float          FZero,      FPosOne,    FNegOne;
extern const int            IPosOne,    INegOne,    IZero;
extern const unsigned       UnusedUInt, UnusedBool;
extern const WORD           UnusedWord;
extern const unsigned       True,       False;

extern float                FScratch1, FScratch2, FScratch3, FScratch4;
extern int                  IScratch1, IScratch2, IScratch3, IScratch4;

extern unsigned             gVerbosityLevel;


extern void XmsProbeStart ( XmsPidTyp pid );
extern void XmsProbeStartNested ( XmsPidTyp pid );

extern void XmsProbeEnd ( XmsPidTyp pid );
extern void XmsProbeEndNested ( XmsPidTyp pid );



#endif  // __X0_FRAME_WORK_H__
