///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Nonvolatile variable management
//
// DESCRIPTION:
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      z0NvVar.xls
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "b0DbCelTst.h"


// Constants
//----------


// Externals
//----------
extern unsigned iM6e_DSPOkCnt;
extern float    DFIaRmsMag;
extern float    DFIbRmsMag;
extern float    DFIcRmsMag;
extern float    GmIaRmsMag;
extern float    GmIbRmsMag;
extern float    GmIcRmsMag;
extern float    L_IaRmsMag;
extern float    L_IbRmsMag;
extern float    L_IcRmsMag;
extern float    L_FuseIaRmsMag;
extern float    L_FuseIbRmsMag;
extern float    L_FuseIcRmsMag;
extern float    R_IaRmsMag;
extern float    R_IbRmsMag;
extern float    R_IcRmsMag;
extern float    R_IaRmsMag;
extern float    R_IbRmsMag;
extern float    R_IcRmsMag;
extern float    S_IaRmsMag;
extern float    S_IbRmsMag;
extern float    S_IcRmsMag;
extern  cDbCelTst L_DbCelTst;
extern int      TempRiseOkTmr;
extern unsigned TempRiseOk2Run;
extern unsigned EventTmSec;
extern float    PowerGenAcc;


// Types and Classes
//------------------


// Function Prototypes
//--------------------


// Parameters
//-----------
CREATE_PARM(NvMemChk, float);   // final-value used as reference to validate NV content
                                // can be set to Zero in case NV vars need to be cleared

// Diagnostics
//------------
CREATE_DIAG(NvInitAlm);


// Published Variables
//--------------------

// NV VARIABLE DEFINITIONS
CREATE_PUBVAR(DspToNvToDspCnt , int) = 0;    // roundtrip Dsp to Nv (blockware) to Dsp time (T3's)
CREATE_PUBVAR(NvUpdateSt      , int) = 0;    // Nv control state
CREATE_PUBVAR(NvToDspReadReq  , unsigned);   // indicates the blockware has seen DspToNvReadReq
CREATE_PUBVAR(DspToNvReadReq  , unsigned);   // enables the blockware to read the Nv vars
CREATE_PUBVAR(DspToNvWriteReq , unsigned);   // enables the blockware to write to the Nv vars

// DSP writes to these IPC variables:
CREATE_PUBVAR(DspToNv01     ,   float );
CREATE_PUBVAR(DspToNv02     ,   float );
CREATE_PUBVAR(DspToNv03     ,   float );
CREATE_PUBVAR(DspToNv04     ,   float );
CREATE_PUBVAR(DspToNv05     ,   float );
CREATE_PUBVAR(DspToNv06     ,   float );
CREATE_PUBVAR(DspToNv07     ,   float );
CREATE_PUBVAR(DspToNv08     ,   float );
CREATE_PUBVAR(DspToNv09     ,   float );
CREATE_PUBVAR(DspToNv10     ,   float );
CREATE_PUBVAR(DspToNv11     ,   float );
CREATE_PUBVAR(DspToNv12     ,   float );
CREATE_PUBVAR(DspToNv13     ,   float );
CREATE_PUBVAR(DspToNv14     ,   float );
CREATE_PUBVAR(DspToNv15     ,   float );
CREATE_PUBVAR(DspToNv16     ,   float );
CREATE_PUBVAR(DspToNv17     ,   float );
CREATE_PUBVAR(DspToNv18     ,   float );
CREATE_PUBVAR(DspToNv19     ,   float );
CREATE_PUBVAR(DspToNv20     ,   float );
CREATE_PUBVAR(DspToNv21     ,   float );
CREATE_PUBVAR(DspToNv22     ,   float );
CREATE_PUBVAR(DspToNv23     ,   float );
CREATE_PUBVAR(DspToNv24     ,   float );
CREATE_PUBVAR(DspToNv25     ,   float );
CREATE_PUBVAR(DspToNv26     ,   float );
CREATE_PUBVAR(DspToNv27     ,   float );
CREATE_PUBVAR(DspToNv28     ,   float );
CREATE_PUBVAR(DspToNv29     ,   float );
CREATE_PUBVAR(DspToNv30     ,   float );
CREATE_PUBVAR(DspToNv31     ,   float );
CREATE_PUBVAR(DspToNv32     ,   float );

// DSP reads from these IPC variables:
CREATE_PUBVAR(NvToDsp01     ,   float );
CREATE_PUBVAR(NvToDsp02     ,   float );
CREATE_PUBVAR(NvToDsp03     ,   float );
CREATE_PUBVAR(NvToDsp04     ,   float );
CREATE_PUBVAR(NvToDsp05     ,   float );
CREATE_PUBVAR(NvToDsp06     ,   float );
CREATE_PUBVAR(NvToDsp07     ,   float );
CREATE_PUBVAR(NvToDsp08     ,   float );
CREATE_PUBVAR(NvToDsp09     ,   float );
CREATE_PUBVAR(NvToDsp10     ,   float );
CREATE_PUBVAR(NvToDsp11     ,   float );
CREATE_PUBVAR(NvToDsp12     ,   float );
CREATE_PUBVAR(NvToDsp13     ,   float );
CREATE_PUBVAR(NvToDsp14     ,   float );
CREATE_PUBVAR(NvToDsp15     ,   float );
CREATE_PUBVAR(NvToDsp16     ,   float );
CREATE_PUBVAR(NvToDsp17     ,   float );
CREATE_PUBVAR(NvToDsp18     ,   float );
CREATE_PUBVAR(NvToDsp19     ,   float );
CREATE_PUBVAR(NvToDsp20     ,   float );
CREATE_PUBVAR(NvToDsp21     ,   float );
CREATE_PUBVAR(NvToDsp22     ,   float );
CREATE_PUBVAR(NvToDsp23     ,   float );
CREATE_PUBVAR(NvToDsp24     ,   float );
CREATE_PUBVAR(NvToDsp25     ,   float );
CREATE_PUBVAR(NvToDsp26     ,   float );
CREATE_PUBVAR(NvToDsp27     ,   float );
CREATE_PUBVAR(NvToDsp28     ,   float );
CREATE_PUBVAR(NvToDsp29     ,   float );
CREATE_PUBVAR(NvToDsp30     ,   float );
CREATE_PUBVAR(NvToDsp31     ,   float );
CREATE_PUBVAR(NvToDsp32     ,   float );

CREATE_PUBVAR(EventTmSecRmd ,   float);       // remainder
CREATE_PUBVAR(EventTmSecDvd ,   float);       // dividen
CREATE_PUBVAR(EventTmSecNv  ,   unsigned);    // the stored Event time read back from the NV


// Unpublished Variables
//----------------------
float  NvDummy16 = 0.0F;
float  NvDummy17 = 0.0F;
float  NvDummy18 = 0.0F;
float  NvDummy19 = 0.0F;
float  NvDummy20 = 0.0F;
float  NvDummy21 = 0.0F;
float  NvDummy27 = 0.0F;
float  NvDummy31 = 0.0F;
float  NvDummy32 = 0.0F;
float  TempRiseOkTmrFl;
float  TempRiseOk2RunFl;

float *NvVarAdrList[32] = {

    &DFIaRmsMag,             // Nv 01
    &DFIbRmsMag,             // Nv 02
    &DFIcRmsMag,             // Nv 03
    &GmIaRmsMag,             // Nv 04
    &GmIbRmsMag,             // Nv 05
    &GmIcRmsMag,             // Nv 06
    &L_IaRmsMag,             // Nv 07
    &L_IbRmsMag,             // Nv 08
    &L_IcRmsMag,             // Nv 09
    &L_FuseIaRmsMag,         // Nv 10
    &L_FuseIbRmsMag,         // Nv 11
    &L_FuseIcRmsMag,         // Nv 12
    &R_IaRmsMag,             // Nv 13
    &R_IbRmsMag,             // Nv 14
    &R_IcRmsMag,             // Nv 15
    &NvDummy16,              // Nv 16
    &NvDummy17,              // Nv 17
    &NvDummy18,              // Nv 18
    &NvDummy19,              // Nv 19
    &NvDummy20,              // Nv 20
    &NvDummy21,              // Nv 21
    &S_IaRmsMag,             // Nv 22
    &S_IbRmsMag,             // Nv 23
    &S_IcRmsMag,             // Nv 24
    &TempRiseOkTmrFl,        // Nv 25
    &TempRiseOk2RunFl,       // Nv 26
    &NvDummy27,              // Nv 27
    &L_DbCelTst.DbCtRmeas,   // Nv 28
    &EventTmSecRmd,          // Nv 29
    &EventTmSecDvd,          // Nv 30
    &PowerGenAcc,            // Nv 31
    &NvDummy32,              // Nv 32
};


// Functions
//----------

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
//      Nonvolatile variable write
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void WriteToNV()
{

    DspToNv01 = *NvVarAdrList[0];
    DspToNv02 = *NvVarAdrList[1];
    DspToNv03 = *NvVarAdrList[2];
    DspToNv04 = *NvVarAdrList[3];
    DspToNv05 = *NvVarAdrList[4];
    DspToNv06 = *NvVarAdrList[5];
    DspToNv07 = *NvVarAdrList[6];
    DspToNv08 = *NvVarAdrList[7];
    DspToNv09 = *NvVarAdrList[8];
    DspToNv10 = *NvVarAdrList[9];
    DspToNv11 = *NvVarAdrList[10];
    DspToNv12 = *NvVarAdrList[11];
    DspToNv13 = *NvVarAdrList[12];
    DspToNv14 = *NvVarAdrList[13];
    DspToNv15 = *NvVarAdrList[14];
    DspToNv16 = *NvVarAdrList[15];
    DspToNv17 = *NvVarAdrList[16];
    DspToNv18 = *NvVarAdrList[17];
    DspToNv19 = *NvVarAdrList[18];
    DspToNv20 = *NvVarAdrList[19];
    DspToNv21 = *NvVarAdrList[20];
    DspToNv22 = *NvVarAdrList[21];
    DspToNv23 = *NvVarAdrList[22];
    DspToNv24 = *NvVarAdrList[23];
    DspToNv25 = *NvVarAdrList[24];
    DspToNv26 = *NvVarAdrList[25];
    DspToNv27 = *NvVarAdrList[26];
    DspToNv28 = *NvVarAdrList[27];
    DspToNv29 = *NvVarAdrList[28];
    DspToNv30 = *NvVarAdrList[29];
    DspToNv31 = *NvVarAdrList[30];
    DspToNv32 = PARM(NvMemChk);    // keep checking reference in NV

    return ;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
//      Nonvolatile variable read
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ReadFromNV()
{
    int Idx;

    if ( (PARM(NvMemChk) == NvToDsp32) && (PARM(NvMemChk) != 0.0F) )   // accept NV content
    {
        *NvVarAdrList[0]  = NvToDsp01;
        *NvVarAdrList[1]  = NvToDsp02;
        *NvVarAdrList[2]  = NvToDsp03;
        *NvVarAdrList[3]  = NvToDsp04;
        *NvVarAdrList[4]  = NvToDsp05;
        *NvVarAdrList[5]  = NvToDsp06;
        *NvVarAdrList[6]  = NvToDsp07;
        *NvVarAdrList[7]  = NvToDsp08;
        *NvVarAdrList[8]  = NvToDsp09;
        *NvVarAdrList[9]  = NvToDsp10;
        *NvVarAdrList[10] = NvToDsp11;
        *NvVarAdrList[11] = NvToDsp12;
        *NvVarAdrList[12] = NvToDsp13;
        *NvVarAdrList[13] = NvToDsp14;
        *NvVarAdrList[14] = NvToDsp15;
        *NvVarAdrList[15] = NvToDsp16;
        *NvVarAdrList[16] = NvToDsp17;
        *NvVarAdrList[17] = NvToDsp18;
        *NvVarAdrList[18] = NvToDsp19;
        *NvVarAdrList[19] = NvToDsp20;
        *NvVarAdrList[20] = NvToDsp21;
        *NvVarAdrList[21] = NvToDsp22;
        *NvVarAdrList[22] = NvToDsp23;
        *NvVarAdrList[23] = NvToDsp24;
        *NvVarAdrList[24] = NvToDsp25;
        *NvVarAdrList[25] = NvToDsp26;
        *NvVarAdrList[26] = NvToDsp27;
        *NvVarAdrList[27] = NvToDsp28;
        *NvVarAdrList[28] = NvToDsp29;
        *NvVarAdrList[29] = NvToDsp30;
        *NvVarAdrList[30] = NvToDsp31;
        *NvVarAdrList[31] = NvToDsp32;
    }
    else        // clear variable buffer coming from NV memory
    {
        for ( Idx = 0; Idx < 32; Idx++ )
        {
            *NvVarAdrList[Idx] = 0.0F;
        }
    }

    return ;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
//      Nonvolatile variable update
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void NonVolatileVarUpdateT3()
{
    switch (NvUpdateSt)
    {
        case 0:
            DspToNvReadReq = false;
            DspToNvWriteReq = false;

            if (iM6e_DSPOkCnt > 10)
            {
                NvUpdateSt = 1;
            }
            break;

        case 1:
            DspToNvReadReq = true;
            if (NvToDspReadReq)
            {
                NvUpdateSt = 2;
            }
            else
            {
                if (DspToNvToDspCnt++ > 8)        // count is 6-7 for 40 msec frame time
                    PUSH_DIAG(NvInitAlm);         // count is 2-3 for 10 msec frame time
            }
            break;

        case 2:
            ReadFromNV();
            TempRiseOkTmr  = static_cast<int>(TempRiseOkTmrFl);
            TempRiseOk2Run = (TempRiseOk2RunFl == 1.0F) ? true : false;
            EventTmSecNv   = ((unsigned(EventTmSecDvd)) << 16) + unsigned(EventTmSecRmd);
            NvUpdateSt = 3;
            break;

        case 3:
            TempRiseOkTmrFl  = static_cast<float>(TempRiseOkTmr);
            TempRiseOk2RunFl = TempRiseOk2Run ? 1.0F : 0.0F;
            EventTmSecDvd = static_cast<float>(EventTmSec >> 16);
            EventTmSecRmd = static_cast<float>(EventTmSec - ((EventTmSec >> 16)<<16));
            WriteToNV();
            DspToNvWriteReq = true;
            break;

        default:
            break;
    }

    return;
}

