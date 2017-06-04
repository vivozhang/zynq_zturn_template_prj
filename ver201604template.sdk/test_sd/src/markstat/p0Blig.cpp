/*=============================================================================
* Product:      GE Power Conversion Control Using MACC
* Host:         TMS320VC33 DSP
* File:         P2BLIG.C30
* Language:     TI 'C'
* Copyright:    2003 GE Motors and Industrial Systems, Salem VA
*
* Abstract:     This file contains functions which implement the LAN/ACL
*               interface for AC drive products.
*==============================================================================
* Functions in module:
*               LanFeedback
*==============================================================================
* 980114 DGF  Create from A0BLIG.C30
* 980116 DGF  L_Ac_Line_Mag -> Line_Mag_T2
* 980116 DGF  Bug fix: zero the accumulators
* 980119 CWM  Scale Lan_I_Mag, Lan_Line_Mag to make rms values.
* 980120 DGF  Conditionally compile LAN references for LVS3SRC
* 980220 DGF  BligInput(): conditionally zero the LAN inputs
* 980220 DGF  BligOutput(): wrap the input heartbeat to the output heartbeat
* 980410 DGF  LanFeedback(): move Lan_Avg_Cnt calculation to SetNetType(), move
*             GP feedback assignments to BligOutput()
*             BligInput(): add heartbeat monitor
* 980423 DGF  BligOutput(): put LAN feedback bits in Lan_Fbk1_Wrd
* 980430 DGF  LanFeedback(): fix yet another bonehead move
* 980507 DGF  Move heartbeat monitor to X0BLIG.C30
* 980602 SAB  Rename to P3BLIG.C30
* 980818 DGF  BligOutput(): pass heartbeat as argument
* 981125 DGF  BligInput() & BligOutput(): integer map
* 990305 DGF  stub BligScaleCalc() for now
* 990310 DGF  Scale the LAN integer map.  Conditionally compile L*_Iy_Ref &
*             L*_Iy_Ref out of ACCBN-A
* 990610 LBW  Add logic for LAN_Fbk1_Wrd bit 23
* 990831 DGF  Bug fix: one too many BligOuts causing overwrite of Lan_Avg_Cnt
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_"on line converter names.
* 021210 SAB  Rename to P2Blig.c30
* 030127 SAB  Replace "L16" with "L_L16"
* 030327 SAB  Replace system and local faults with L_CntrlOnFlt and R_CntrlOnFlt
* 030405 SAB  L_Running --> Running.  L_RunRdy --> RunRdy
* 030724 SAB  Use ISBUS_ENABLE to compile out lan functions for now
* 040223 SAB  Remove MACC MODE condtionals
* 060322 SAB  L_CntrlOnFlt and R_CntrlOnflt -- L,R_SftyChgOpn
* 071003 IB   Remove unused ISBUS code
*
*==============================================================================
* Last Mod: October 03, 2007 09:38:10AM
*/


/*******************************************************************************
* Include all public information
*******************************************************************************/

#include "x0FrameWork.h"
#include "Product.h"
#include "p0Blig.h"

// #if ISBUS_ENABLE
struct lanInt
{
    int             lsw:16;
    int             msw:16;
};

struct LanFbk1Wrd
{
    unsigned        heartbeat:1;  /* Bit 00 */
    unsigned        NoFlt:1;   /* Bit 01 */
    unsigned        TripFlt:1; /* Bit 02 */
    unsigned        L_SftyChnOpn:1;  /* Bit 03 */
    unsigned        R_SftyChnOpn:1;  /* Bit 04 */
    unsigned        RunRdy:1;       /* Bit 05 */
    unsigned        L_BrgPwrEnb:1;  /* Bit 06 */
    unsigned        Running:1;     /* Bit 07 */
    unsigned:       2;          /* Bit 08-09 */
    unsigned        Xstp_Act:1; /* Bit 10 */
    unsigned:       12;         /* Bit 11-22 */
    unsigned        LanDiagB01:1;       /* Bit 23 */
    unsigned        B01:1;      /* Bit 24 */
    unsigned        B02:1;      /* Bit 25 */
    unsigned        B03:1;      /* Bit 26 */
    unsigned        B04:1;      /* Bit 27 */
    unsigned        B05:1;      /* Bit 28 */
    unsigned        B06:1;      /* Bit 29 */
    unsigned        B07:1;      /* Bit 30 */
    unsigned        B08:1;      /* Bit 31 */
};


struct LanReq1Wrd
{
    unsigned        heartbeat:1;/* Bit 00 */
    unsigned        FltRst:1;   /* Bit 01 */
    unsigned        Trp_Req:1;  /* Bit 02 */
    unsigned        Alm_Req:1;  /* Bit 03 */
    unsigned:       4;          /* Bit 04-07 */
    unsigned        RunReq:1;   /* Bit 08 */
    unsigned:       1;          /* Bit 09 */
    unsigned        Xstp_Req:1; /* Bit 10 */
    unsigned:       13;         /* Bit 11-23 */
    unsigned        B01:1;      /* Bit 24 */
    unsigned        B02:1;      /* Bit 25 */
    unsigned        B03:1;      /* Bit 26 */
    unsigned        B04:1;      /* Bit 27 */
    unsigned        B05:1;      /* Bit 28 */
    unsigned        B06:1;      /* Bit 29 */
    unsigned        B07:1;      /* Bit 30 */
    unsigned        B08:1;      /* Bit 31 */
};


// #endif     //end of ISBUS_ENABLE


// Variable declarations


CREATE_PUBVAR(AVrmsSclOut   ,    float );                /* = 1, by default */
CREATE_PUBVAR(FrqSclOut     ,    float )= 1 / PI2;
CREATE_PUBVAR(L_IySclIn     ,    float )= SQRT2;
CREATE_PUBVAR(L_VARSclOut   ,    float )= .001;
CREATE_PUBVAR(L_VdcSclOut   ,    float );                /* = 1, by default */
CREATE_PUBVAR(L_VrmsSclOut  ,    float );                /* = 1, by default */
CREATE_PUBVAR(LanR01Fbk     ,    float );
CREATE_PUBVAR(LanR01Ref     ,    float );
CREATE_PUBVAR(LanR02Fbk     ,    float );
CREATE_PUBVAR(LanR02Ref     ,    float );
CREATE_PUBVAR(LanR03Fbk     ,    float );
CREATE_PUBVAR(LanR03Ref     ,    float );
CREATE_PUBVAR(LanR04Fbk     ,    float );
CREATE_PUBVAR(LanR04Ref     ,    float );
CREATE_PUBVAR(PowerSclOut   ,    float )= .001;
CREATE_PUBVAR(L_L16IMag     ,    int  );
CREATE_PUBVAR(L_L16IyFbk    ,    int  );
CREATE_PUBVAR(L_L16IyRef    ,    int  );
CREATE_PUBVAR(L_L16LBusPwr  ,    int  );
CREATE_PUBVAR(L_L16LineFrq  ,    int  );
CREATE_PUBVAR(L_L16LineMag  ,    int  );
CREATE_PUBVAR(L_L16R01Fbk   ,    int  );
CREATE_PUBVAR(L_L16R01Ref   ,    int  );
CREATE_PUBVAR(L_L16R02Fbk   ,    int  );
CREATE_PUBVAR(L_L16R02Ref   ,    int  );
CREATE_PUBVAR(L_L16R03Fbk   ,    int  );
CREATE_PUBVAR(L_L16R03Ref   ,    int  );
CREATE_PUBVAR(L_L16R04Fbk   ,    int  );
CREATE_PUBVAR(L_L16R04Ref   ,    int  );
CREATE_PUBVAR(L_L16VarCalc  ,    int  );
CREATE_PUBVAR(L_L16VdcFbk   ,    int  );
CREATE_PUBVAR(LanB01Fbk     ,    int  );
CREATE_PUBVAR(LanB02Fbk     ,    int  );
CREATE_PUBVAR(LanB03Fbk     ,    int  );
CREATE_PUBVAR(LanB04Fbk     ,    int  );
CREATE_PUBVAR(LanB05Fbk     ,    int  );
CREATE_PUBVAR(LanB06Fbk     ,    int  );
CREATE_PUBVAR(LanB07Fbk     ,    int  );
CREATE_PUBVAR(LanB08Fbk     ,    int  );
struct LanFbk1Wrd LanFbk1Wrd;
struct LanReq1Wrd LanReq1Wrd;



struct bligData
{
    float          *faddr;
    int            *iaddr;
    float          *saddr;
};

#if ISBUS_ENABLE
/*
 * BligIns[] is the LAN map to drive signal translation table.  A {0} indicates
 * a spare in the map.  Non-spare entries include the address of the real drive
 * signal to populate, the address of the integer drive signal to populate, and
 * the address of the scale factor to multiply the integer by to form the real.
 * Note that the last two (integer and scale factor) are only used when an
 * integer LAN map is selected.
 */
struct bligData BligIns[] =
{

    {&L_LanIyRef, &L_L16IyRef, &L_IySclIn},
    {&L_LanIyFbk, &L_L16IyFbk, &L_IySclIn},
    {0},
    {0},
    {0},
    {&LanR01Ref, &L_L16R01Ref, &PARM(LanRf01Scl)},
    {&LanR02Ref, &L_L16R02Ref, &PARM(LanRf02Scl)},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {&LanR03Ref, &L_L16R03Ref, &PARM(LanRf03Scl)},
    {&LanR04Ref, &L_L16R04Ref, &PARM(LanRf04Scl)},
};
#define NBLIGINS    (sizeof BligIns / sizeof BligIns[0])


void            BligInput(int *ip)
{
    int             i;

    if (ip == 0) {
        /*
         * Zero the inputs
         */
        *(int *) &LanReq1Wrd = 0;
        for (i = 0; i < NBLIGINS; ++i)
            if (BligIns[i].faddr)
                *BligIns[i].faddr = 0;
        return;
    }
    *(int *) &LanReq1Wrd = *ip++;
    if (PARM(LanPgFrmt) == 1) {
        /*
         * Integer map
         */
        struct lanInt  *lip = (void *) ip;
        int             sig;

        for (i = 0; i < NBLIGINS; ++i) {
            if (BligIns[i].faddr) {
                sig = i & 1 ? lip->msw : lip->lsw;
                if (BligIns[i].iaddr)
                    *BligIns[i].iaddr = sig;
                *BligIns[i].faddr = sig * *BligIns[i].saddr;
            }
            if (i & 1)
                ++lip;
        }
        return;
    }
    /*
     * Float map
     */
    for (i = 0; i < NBLIGINS; ++i, ++ip)
        if (BligIns[i].faddr)
            *BligIns[i].faddr = fmieee(*ip);
}



/*
 * BligOut[] is the drive signal to LAN map translation table.  A {0} indicates
 * a spare in the map.  Non-spare entries include the address of the real drive
 * signal to transmit, the address of the integer drive signal to populate, and
 * the address of the scale factor to multiply the real by to form the integer.
 * Note that the last two (integer and scale factor) are only used when an
 * integer LAN map is selected.
 */
struct bligData BligOuts[] =
{
    {0},
    {&L_LanIMag, &L_L16IMag, &AVrmsSclOut},
    {&L_LanLineMag, &L_L16LineMag, &L_VrmsSclOut},
    {&L_LanVdcFbk, &L_L16VdcFbk, &L_VdcSclOut},
    {&L_LanBusPwr, &L_L16LBusPwr, &PowerSclOut},
    {&LanR01Fbk, &L_L16R01Fbk, &PARM(LanFb01Scl)},
    {&LanR02Fbk, &L_L16R02Fbk, &PARM(LanFb02Scl)},
    {&L_LanLineFrq, &L_L16LineFrq, &FrqSclOut},
    {&L_LanVarCalc, &L_L16VarCalc, &L_VARSclOut},
    {0},
    {0},
    {0},
    {0},
    {&LanR03Fbk, &L_L16R03Fbk, &PARM(LanFb03Scl)},
    {&LanR04Fbk, &L_L16R04Fbk, &PARM(LanFb04Scl)},
};
#define NBLIGOUTS   (sizeof BligOuts / sizeof BligOuts[0])


void            BligOutput(int *ip, int heartbeat)
{
    int             i;

    /*
     * Assign values to general purpose feedbacks.
     */
    LanB01Fbk = gVariable.ReadAsBool(PARM(LanB01Adr));
    LanB02Fbk = gVariable.ReadAsBool(PARM(LanB02Adr));
    LanB03Fbk = gVariable.ReadAsBool(PARM(LanB03Adr));
    LanB04Fbk = gVariable.ReadAsBool(PARM(LanB04Adr));
    LanB05Fbk = gVariable.ReadAsBool(PARM(LanB05Adr));
    LanB06Fbk = gVariable.ReadAsBool(PARM(LanB06Adr));
    LanB07Fbk = gVariable.ReadAsBool(PARM(LanB07Adr));
    LanB08Fbk = gVariable.ReadAsBool(PARM(LanB08Adr));

    LanR01Fbk = gVariable.ReadAsFloat(PARM(LanR01Adr));
    LanR02Fbk = gVariable.ReadAsFloat(PARM(LanR02Adr));
    LanR03Fbk = gVariable.ReadAsFloat(PARM(LanR03Adr));
    LanR04Fbk = gVariable.ReadAsFloat(PARM(LanR04Adr));
    /*
     * Bit 0: heartbeat
     * Bit 1: no faults or alarms
     */
    heartbeat |= NoFlt ? 2 : 0;
    //
    // Bit 2: trip fault active
    //
    heartbeat |= TripFlt ? 4 : 0;
    //
    //Bit 3: line converter safety string input
    //
    heartbeat |= L_SftyChnOpn ? 8 : 0;
    //
    // Bit 4: rotor converter safety string input
    //
    heartbeat |= R_SftyChnOpn  ? 0x10 : 0;
    //
     /* Bit 5: ready to run
     */
    heartbeat |= RunRdy ? 0x20 : 0;
    /*
     * Bit 6: bridge is on
     */
    heartbeat |= L_BrgPwrEnb ? 0x40 : 0;
    /*
     * Bit 7: Running
     */
    heartbeat |= Running ? 0x80 : 0;
    /*
     * Bit 10: x stop active
     */
    /*
     * Bit 20:
     * Bit 21:
     * Bit 22:
     */
     /*
     * Bit 23: Lan Diagnostic bit
     */
    heartbeat |= LanDiagB01 ? 0x800000 : 0;
    /*
     * Bits 24 - 31: general purpose LAN feedbacks 1 - 8
     */
    heartbeat |= LanB01Fbk ? 0x01000000 : 0;
    heartbeat |= LanB02Fbk ? 0x02000000 : 0;
    heartbeat |= LanB03Fbk ? 0x04000000 : 0;
    heartbeat |= LanB04Fbk ? 0x08000000 : 0;
    heartbeat |= LanB05Fbk ? 0x10000000 : 0;
    heartbeat |= LanB06Fbk ? 0x20000000 : 0;
    heartbeat |= LanB07Fbk ? 0x40000000 : 0;
    heartbeat |= LanB08Fbk ? 0x80000000 : 0;

    *ip++ = *(int *) &LanFbk1Wrd = heartbeat;
    if (PARM(LanPgFrmt) == 1) {
        /*
         * Integer map
         */
        struct lanInt  *lip = (void *) ip;
        int             sig;

        for (i = 0; i < NBLIGOUTS; ++i) {
            if (i == 0)
                sig = LanFltCode;
            else if (BligOuts[i].faddr) {
                sig = *BligOuts[i].faddr * *BligOuts[i].saddr;
                /*
                 * Clamp to 16 bits
                 */
                if (sig < (int) ~0x7FFF)
                    sig = (int) ~0x7FFF;
                else if (sig > 0x7FFF)
                    sig = 0x7FFF;
                if (BligOuts[i].iaddr)
                    *BligOuts[i].iaddr = sig;
            } else
                sig = 0;
            if (i & 1)
                lip++->msw = sig;
            else
                lip->lsw = sig;
        }
        return;
    }
    /*
     * Float map
     */
    for (i = 0; i < NBLIGOUTS; ++i, ++ip)
        if (i == 0)
            *ip = LanFltCode;
        else if (BligOuts[i].faddr)
            *ip = toieee(*BligOuts[i].faddr);
        else
            *ip = 0;
}

#endif //end of ISBUS_ENABLE stuff
