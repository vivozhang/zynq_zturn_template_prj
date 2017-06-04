///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Hardware Diagnostics
//
// DESCRIPTION:
//
// COPYRIGHT:   Copyright (c) 2014
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// core
// framework
#include "x0FrameWork.h"
#include "x0BridgeInterface.h"
#include "x0CaptureBuf.h"
#include "x0Vector.h"
// product
#include "Product.h"
#include "b0BridgeRebf.h"
#include "b0BrgThrm5p.h"
#include "b0BrgThrmstr.h"
#include "p0EsaBuf.h"
#include "p0Thrmstr.h"
#include "p2Sseq.h"

// Constants
//----------
#define TMP_MARGIN 15
#define DIN_HI_LEVEL 0.63212F           // Digital hi hysteresis turn on level

// Externals
//----------
extern cBridgeRebf      L_BridgeRebf;
extern cBridgeRebf      R_BridgeRebf;
extern cBrgThrmstr      L_BrgThrmstr;
extern cBrgThrmstr      R_BrgThrmstr;
extern cBrgThrm5pModel  L_BrgThrm5pModel;
extern cBrgThrm5pModel  R_BrgThrm5pModel;
extern unsigned         SimMode;

extern unsigned         L_Running;
extern unsigned         R_Running;
extern unsigned         BpRiseHi;
extern unsigned         CoolntIsHot;
extern float            InletAirTemp;
extern unsigned         CContOpnCmd;
extern float            PdcTemp;
extern float            CfcTemp;
extern float            GmVMagPuT3;
extern float            L_VdcFbkT3;
extern float            L_VdcFbkT2;
extern float            DelTm3;
extern float            DelTm2;
extern float            CoolantTemp;
extern float            L_Van;
extern float            L_Vbn;
extern float            L_Vcn;
extern float            GmVanT2;
extern float            GmVbnT2;
extern float            GmVcnT2;
extern float            L_StopStTmr;
extern int              L_SeqSt;
extern unsigned         Q1UVHld;
extern unsigned         GmNSeqHi;
extern float            GmVMagPuT2;
extern unsigned         SysTmSec;
extern float            GmSVMtchErSq;
extern float            GmSVmtchThr;
extern unsigned         CContOpnCmd;
extern unsigned         Q1UVHld;

// Parameters
//-----------
CREATE_PARM(HdwDiagVcoW   , float);
CREATE_PARM(HdwVcoHiBadThr, float);
CREATE_PARM(HdwVcoLoBadThr, float);

CREATE_PARM(HdwBadVdcThr,   float);
CREATE_PARM(HdwBadVdcW,     float);

CREATE_PARM(HdwFanFailTempThr,float);         // Cfc Temp to Ambient Temperature Delta Threshold
CREATE_PARM(VsecLossMgW,      float);
CREATE_PARM(VsecLossThrs,     float);
CREATE_PARM(VsecLossTm,       float);
CREATE_PARM(LqdColingW,       float);
CREATE_PARM(LqdColingBadThr,  float);
CREATE_PARM(BpToJncW,         float);
CREATE_PARM(BpToJncBadThr,    float);


CREATE_PARM(HdwDiagEnabled,   unsigned);
CREATE_PARM(FchkCcontOpnTm,   float);
CREATE_PARM(FchkDlyTmr,       float);
CREATE_PARM(FchkGmVMgThr,     float);
CREATE_PARM(FchkGmLnDelTm,    float);
CREATE_PARM(FchkGmLnDelThr,   float);

CREATE_PARM(HdwSysTmThr,      unsigned);

// Published Variables
//--------------------
CREATE_PUBVAR(HdwDiagVcoGn , float);

CREATE_PUBVAR(AeadLnVvco1           , float);
CREATE_PUBVAR(AeadLnVvco2           , float);
CREATE_PUBVAR(AeadLnVvco3           , float);
CREATE_PUBVAR(AeadLnIvco5           , float);
CREATE_PUBVAR(AeadLnIvco6           , float);
CREATE_PUBVAR(AeadLnIvco7           , float);
CREATE_PUBVAR(AedbLnVvcoab          , float);
CREATE_PUBVAR(AedbLnVvcobc          , float);
CREATE_PUBVAR(AedbLnIaSfb           , float);
CREATE_PUBVAR(AedbLnIbSfb           , float);
CREATE_PUBVAR(AedbLnIcSfb           , float);

CREATE_PUBVAR(GenIoVvco1            , float);
CREATE_PUBVAR(GenIoVvco2            , float);
CREATE_PUBVAR(GenIoVvco3            , float);
CREATE_PUBVAR(GenIoIvco5            , float);
CREATE_PUBVAR(GenIoIvco6            , float);
CREATE_PUBVAR(GenIoIvco7            , float);
CREATE_PUBVAR(AebiGnVvcoab          , float);
CREATE_PUBVAR(AebiGnVvcobc          , float);
CREATE_PUBVAR(AebiGnIaSfb           , float);
CREATE_PUBVAR(AebiGnIbSfb           , float);
CREATE_PUBVAR(AebiGnIcSfb           , float);

CREATE_PUBVAR(AeadLnVvco1Fil , float);
CREATE_PUBVAR(AeadLnVvco2Fil , float);
CREATE_PUBVAR(AeadLnVvco3Fil , float);
CREATE_PUBVAR(AeadLnIvco5Fil , float);
CREATE_PUBVAR(AeadLnIvco6Fil , float);
CREATE_PUBVAR(AeadLnIvco7Fil , float);
CREATE_PUBVAR(AedbLnVvcoabFil, float);
CREATE_PUBVAR(AedbLnVvcobcFil, float);
CREATE_PUBVAR(AedbLnIaSfbFil , float);
CREATE_PUBVAR(AedbLnIbSfbFil , float);
CREATE_PUBVAR(AedbLnIcSfbFil , float);

CREATE_PUBVAR(GenIoVvco1Fil , float);
CREATE_PUBVAR(GenIoVvco2Fil , float);
CREATE_PUBVAR(GenIoVvco3Fil , float);
CREATE_PUBVAR(GenIoIvco5Fil , float);
CREATE_PUBVAR(GenIoIvco6Fil , float);
CREATE_PUBVAR(GenIoIvco7Fil , float);
CREATE_PUBVAR(AebiGnVvcoabFil, float);
CREATE_PUBVAR(AebiGnVvcobcFil, float);
CREATE_PUBVAR(AebiGnIaSfbFil , float);
CREATE_PUBVAR(AebiGnIbSfbFil , float);
CREATE_PUBVAR(AebiGnIcSfbFil , float);


CREATE_PUBVAR(HdwBadVdcFil,    float);
CREATE_PUBVAR(HdwBadVdcHp,     float);
CREATE_PUBVAR(HdwBadVdcWdt,    float);

CREATE_PUBVAR(VsecLosMgPuFil ,  float);     // Filtered value of ac voltage magnitude in task 3
CREATE_PUBVAR(VsecLossMgGn   ,  float);     // Gain fo the above filter
CREATE_PUBVAR(VsecLossTmr    ,  float);

CREATE_PUBVAR(LqdColingGn     ,  float);
CREATE_PUBVAR(LqdColingBadRaw ,  float);
CREATE_PUBVAR(LqdColingBadFil ,  float);
CREATE_PUBVAR(BpToJncBadGn    ,  float);
CREATE_PUBVAR(BpToJncBadRaw   ,  float);
CREATE_PUBVAR(BpToJncBadFil   ,  float);
CREATE_PUBVAR(L_VanT2         ,  float);
CREATE_PUBVAR(L_VbnT2         ,  float);
CREATE_PUBVAR(L_VcnT2         ,  float);
CREATE_PUBVAR(FchkTstDone     ,  unsigned);
CREATE_PUBVAR(FchkCcontOpn    ,  unsigned);
CREATE_PUBVAR(FchkCcontOpnDly ,  float);
CREATE_PUBVAR(GmLnPhAMisMtch  ,  float);
CREATE_PUBVAR(GmLnPhBMisMtch  ,  float);
CREATE_PUBVAR(GmLnPhCMisMtch  ,  float);
CREATE_PUBVAR(FchkGmLnDelTmr  ,  float);


// Diagnostics
//--------------------
CREATE_DIAG(HdwBadDtaFanTrp);
CREATE_DIAG(HdwBadDtaPumpTrp);
CREATE_DIAG(HdwBadVsecLossTrp);

CREATE_DIAG(HdwBadAeadLnVvco1Alm);
CREATE_DIAG(HdwBadAeadLnVvco2Alm);
CREATE_DIAG(HdwBadAeadLnVvco3Alm);
CREATE_DIAG(HdwBadAeadLnIvco5Alm);
CREATE_DIAG(HdwBadAeadLnIvco6Alm);
CREATE_DIAG(HdwBadAeadLnIvco7Alm);
CREATE_DIAG(HdwBadAedbLnVvcoabAlm);
CREATE_DIAG(HdwBadAedbLnVvcobcAlm);
CREATE_DIAG(HdwBadAedbLnIaSfbAlm);
CREATE_DIAG(HdwBadAedbLnIbSfbAlm);
CREATE_DIAG(HdwBadAedbLnIcSfbAlm);

CREATE_DIAG(HdwBadGenIoVvco1Alm);
CREATE_DIAG(HdwBadGenIoVvco2Alm);
CREATE_DIAG(HdwBadGenIoVvco3Alm);
CREATE_DIAG(HdwBadGenIoIvco5Alm);
CREATE_DIAG(HdwBadGenIoIvco6Alm);
CREATE_DIAG(HdwBadGenIoIvco7Alm);
CREATE_DIAG(HdwBadAebiGVvcoabAlm);
CREATE_DIAG(HdwBadAebiGVvcobcAlm);
CREATE_DIAG(HdwBadAebiGnIaSfbAlm);
CREATE_DIAG(HdwBadAebiGnIbSfbAlm);
CREATE_DIAG(HdwBadAebiGnIcSfbAlm);

CREATE_DIAG(HdwBadIGBTTrp);               // IGBT failture detection
CREATE_DIAG(HdwBadAcfluPhATrp);
CREATE_DIAG(HdwBadAcfluPhBTrp);
CREATE_DIAG(HdwBadAcfluPhCTrp);

// Data Passing
//-------------
DATA_PASS(L_BridgeRebf.VabBrgVcoCnt,       AedbLnVvcoab,        T1_T3, FILTERED);
DATA_PASS(L_BridgeRebf.VbcBrgVcoCnt,       AedbLnVvcobc,        T1_T3, FILTERED);
DATA_PASS(L_BridgeRebf.IaBrgVcoCnt,        AedbLnIaSfb,         T1_T3, FILTERED);
DATA_PASS(L_BridgeRebf.IbBrgVcoCnt,        AedbLnIbSfb,         T1_T3, FILTERED);
DATA_PASS(L_BridgeRebf.IcBrgVcoCnt,        AedbLnIcSfb,         T1_T3, FILTERED);
DATA_PASS(L_BridgeRebf.AeaxAin1Freq,       AeadLnVvco1,         T1_T3, FILTERED);
DATA_PASS(L_BridgeRebf.AeaxAin2Freq,       AeadLnVvco2,         T1_T3, FILTERED);
DATA_PASS(L_BridgeRebf.AeaxAin3Freq,       AeadLnVvco3,         T1_T3, FILTERED);
DATA_PASS(L_BridgeRebf.AeaxAin5Freq,       AeadLnIvco5,         T1_T3, FILTERED);
DATA_PASS(L_BridgeRebf.AeaxAin6Freq,       AeadLnIvco6,         T1_T3, FILTERED);
DATA_PASS(L_BridgeRebf.AeaxAin7Freq,       AeadLnIvco7,         T1_T3, FILTERED);

DATA_PASS(R_BridgeRebf.VabBrgVcoCnt,       AebiGnVvcoab,        T1A_T3, FILTERED);
DATA_PASS(R_BridgeRebf.VbcBrgVcoCnt,       AebiGnVvcobc,        T1A_T3, FILTERED);
DATA_PASS(R_BridgeRebf.IaBrgVcoCnt,        AebiGnIaSfb,         T1A_T3, FILTERED);
DATA_PASS(R_BridgeRebf.IbBrgVcoCnt,        AebiGnIbSfb,         T1A_T3, FILTERED);
DATA_PASS(R_BridgeRebf.IcBrgVcoCnt,        AebiGnIcSfb,         T1A_T3, FILTERED);
DATA_PASS(R_BridgeRebf.VaxIoVcoCnt,        GenIoVvco1,          T1A_T3, FILTERED);
DATA_PASS(R_BridgeRebf.VbxIoVcoCnt,        GenIoVvco2,          T1A_T3, FILTERED);
DATA_PASS(R_BridgeRebf.VcxIoVcoCnt,        GenIoVvco3,          T1A_T3, FILTERED);
DATA_PASS(R_BridgeRebf.IaIoVcoCnt,         GenIoIvco5,          T1A_T3, FILTERED);
DATA_PASS(R_BridgeRebf.IbIoVcoCnt,         GenIoIvco6,          T1A_T3, FILTERED);
DATA_PASS(R_BridgeRebf.IcIoVcoCnt,         GenIoIvco7,          T1A_T3, FILTERED);

DATA_PASS(L_Van,                           L_VanT2,               T1_T2, FILTERED);
DATA_PASS(L_Vbn,                           L_VbnT2,               T1_T2, FILTERED);
DATA_PASS(L_Vcn,                           L_VcnT2,               T1_T2, FILTERED);

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Hardware Failure Detection
//
// EXECUTION LEVEL
//      Task 1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void HdwDiagT1(void)
{


    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Hardware Failure Detection
//
// EXECUTION LEVEL
//      Task 2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void HdwDiagT2(void)
{
    if ( PARM(HdwDiagEnabled) && Q1UVHld)
    {

       HdwBadVdcFil = ((L_VdcFbkT2 - HdwBadVdcFil) * HdwBadVdcWdt) + HdwBadVdcFil;
       HdwBadVdcHp  = HdwBadVdcFil - L_VdcFbkT2;

       if ( HdwBadVdcHp > PARM(HdwBadVdcThr))
       {
          PUSH_DIAG(HdwBadIGBTTrp);
       }
    }

    if ( PARM(HdwDiagEnabled) && PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
    {
       //*****Begin: Hardware Diagnostic--Line Fuse Check while Converter is Stopped*****//
       if ( (GmVMagPuT2 > PARM(FchkGmVMgThr)) && !GmNSeqHi && (GmSVMtchErSq < GmSVmtchThr))

          GmLnPhAMisMtch = ABS(GmVanT2 - L_VanT2);
          GmLnPhBMisMtch = ABS(GmVbnT2 - L_VbnT2);
          GmLnPhCMisMtch = ABS(GmVcnT2 - L_VcnT2);

       {
          if ( (L_StopStTmr >= PARM(FchkDlyTmr))
               && !FchkTstDone
               && Q1UVHld
          )
          {
             FchkCcontOpn = true;
             CContOpnCmd  = FchkCcontOpn;
             FchkCcontOpnDly += DelTm2;

             if ( FchkCcontOpnDly >= PARM(FchkCcontOpnTm))
             {
                FchkGmLnDelTmr += DelTm2;

                if ( FchkGmLnDelTmr <= PARM(FchkGmLnDelTm))
                {
                   if ( GmLnPhAMisMtch > (PARM(FchkGmLnDelThr) * PARM(SysVoltNom)))
                   {
                       PUSH_DIAG(HdwBadAcfluPhATrp);
                   }
                   else if ( GmLnPhBMisMtch > (PARM(FchkGmLnDelThr) * PARM(SysVoltNom)))
                   {
                       PUSH_DIAG(HdwBadAcfluPhBTrp);
                   }
                   else if ( GmLnPhCMisMtch > (PARM(FchkGmLnDelThr) * PARM(SysVoltNom)))
                   {
                       PUSH_DIAG(HdwBadAcfluPhCTrp);
                   }
                }
                else
                {
                   FchkCcontOpn    = false;
                   CContOpnCmd     = FchkCcontOpn;
                   FchkTstDone     = true;
                   FchkCcontOpnDly = 0.0F;
                   FchkGmLnDelTmr  = 0.0F;
                }
             }
          }
       }
       //*****End: Hardware Diagnostic--Line Fuse Check while Converter is Stopped*****//
    }

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Hardware Failure Detection
//
// EXECUTION LEVEL
//      Task 3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void HdwDiagT3(void)
{

    if ( PARM(HdwDiagEnabled))
    {
       AeadLnVvco1Fil  += HdwDiagVcoGn * (AeadLnVvco1  - AeadLnVvco1Fil);
       AeadLnVvco2Fil  += HdwDiagVcoGn * (AeadLnVvco2  - AeadLnVvco2Fil);
       AeadLnVvco3Fil  += HdwDiagVcoGn * (AeadLnVvco3  - AeadLnVvco3Fil);

       AeadLnIvco5Fil  += HdwDiagVcoGn * (AeadLnIvco5  - AeadLnIvco5Fil);
       AeadLnIvco6Fil  += HdwDiagVcoGn * (AeadLnIvco6  - AeadLnIvco6Fil);
       AeadLnIvco7Fil  += HdwDiagVcoGn * (AeadLnIvco7  - AeadLnIvco7Fil);

       AedbLnVvcoabFil += HdwDiagVcoGn * (AedbLnVvcoab - AedbLnVvcoabFil);
       AedbLnVvcobcFil += HdwDiagVcoGn * (AedbLnVvcobc - AedbLnVvcobcFil);

       AedbLnIaSfbFil  += HdwDiagVcoGn * (AedbLnIaSfb  - AedbLnIaSfbFil);
       AedbLnIbSfbFil  += HdwDiagVcoGn * (AedbLnIbSfb  - AedbLnIbSfbFil);
       AedbLnIcSfbFil  += HdwDiagVcoGn * (AedbLnIcSfb  - AedbLnIcSfbFil);

       GenIoVvco1Fil   += HdwDiagVcoGn * (GenIoVvco1  - GenIoVvco1Fil);
       GenIoVvco2Fil   += HdwDiagVcoGn * (GenIoVvco2  - GenIoVvco2Fil);

       if ( PARM(SoftwareIPN) >= DFIG02_1850_5060_E)  // This filter signal is only valid if rotor side has AEAD
       {
          GenIoVvco3Fil   += HdwDiagVcoGn * (GenIoVvco3  - GenIoVvco3Fil);
       }
       else
       {
          GenIoVvco3Fil = 1000.0F;      // If rotor side has AEAA, set this filter to some value that's between
                                        // P.HdwVcoHiBadThr and P.HdwVcoLoBadThr so the check for it will not cause
                                        // a false alarm.
       }

       GenIoIvco5Fil   += HdwDiagVcoGn * (GenIoIvco5  - GenIoIvco5Fil);
       GenIoIvco6Fil   += HdwDiagVcoGn * (GenIoIvco6  - GenIoIvco6Fil);
       GenIoIvco7Fil   += HdwDiagVcoGn * (GenIoIvco7  - GenIoIvco7Fil);

       AebiGnVvcoabFil += HdwDiagVcoGn * (AebiGnVvcoab - AebiGnVvcoabFil);
       AebiGnVvcobcFil += HdwDiagVcoGn * (AebiGnVvcobc - AebiGnVvcobcFil);

       AebiGnIaSfbFil  += HdwDiagVcoGn * (AebiGnIaSfb  - AebiGnIaSfbFil);
       AebiGnIbSfbFil  += HdwDiagVcoGn * (AebiGnIbSfb  - AebiGnIbSfbFil);
       AebiGnIcSfbFil  += HdwDiagVcoGn * (AebiGnIcSfb  - AebiGnIcSfbFil);

       VsecLosMgPuFil += VsecLossMgGn * (GmVMagPuT3 - VsecLosMgPuFil);

       //*****Begin: Hardware Diagnostic--Secondary Voltage Loss*****//
       if ((VsecLosMgPuFil < PARM(VsecLossThrs)) && (SysTmSec > PARM(HdwSysTmThr)) && !SimMode)
       {
          VsecLossTmr = VsecLossTmr + DelTm3;
          if ( VsecLossTmr >= PARM(VsecLossTm))
          {
             VsecLossTmr = PARM(VsecLossTm);
             PUSH_DIAG(HdwBadVsecLossTrp);
          }
       }
       else
       {
          VsecLossTmr = VsecLossTmr - DelTm3;
          if ( VsecLossTmr <= 0.0)
          {
             VsecLossTmr = 0.0;
          }
       }

       //*****End: Hardware Diagnostic--Secondary Voltage Loss*****//

       //*****Begin: Hardware Diagnostic--Pump Loss*****//

       if (    (ABS(L_BrgThrmstr.BthtrTempMaxA - CoolantTemp) > PARM(LqdColingBadThr))
            && (ABS(L_BrgThrmstr.BthtrTempMaxB - CoolantTemp) > PARM(LqdColingBadThr))
            && (ABS(L_BrgThrmstr.BthtrTempMaxC - CoolantTemp) > PARM(LqdColingBadThr))
            && (ABS(R_BrgThrmstr.BthtrTempMaxA - CoolantTemp) > PARM(LqdColingBadThr))
            && (ABS(R_BrgThrmstr.BthtrTempMaxB - CoolantTemp) > PARM(LqdColingBadThr))
            && (ABS(R_BrgThrmstr.BthtrTempMaxC - CoolantTemp) > PARM(LqdColingBadThr))
          )
       {
           LqdColingBadRaw = true;
       }
       else
       {
           LqdColingBadRaw = false;
       }

       LqdColingBadFil += LqdColingGn * (LqdColingBadRaw - LqdColingBadFil);

       if (   (((L_BrgThrm5pModel.BtmTrJncTempAUT3  - L_BrgThrmstr.BthtrTemp1) < PARM(BpToJncBadThr))
            && ((L_BrgThrm5pModel.BtmTrJncTempALT3  - L_BrgThrmstr.BthtrTemp4) < PARM(BpToJncBadThr))
            && ((L_BrgThrm5pModel.BtmTrJncTempBUT3  - L_BrgThrmstr.BthtrTemp2) < PARM(BpToJncBadThr))
            && ((L_BrgThrm5pModel.BtmTrJncTempBLT3  - L_BrgThrmstr.BthtrTemp5) < PARM(BpToJncBadThr))
            && ((L_BrgThrm5pModel.BtmTrJncTempCUT3  - L_BrgThrmstr.BthtrTemp3) < PARM(BpToJncBadThr))
            && ((L_BrgThrm5pModel.BtmTrJncTempCLT3  - L_BrgThrmstr.BthtrTemp6) < PARM(BpToJncBadThr))
            &&  L_Running)
            || (((R_BrgThrm5pModel.BtmTrJncTempALT3 - R_BrgThrmstr.BthtrTemp4) < PARM(BpToJncBadThr))
            &&  ((R_BrgThrm5pModel.BtmTrJncTempALT3 - R_BrgThrmstr.BthtrTemp4) < PARM(BpToJncBadThr))
            &&  ((R_BrgThrm5pModel.BtmTrJncTempBUT3 - R_BrgThrmstr.BthtrTemp2) < PARM(BpToJncBadThr))
            &&  ((R_BrgThrm5pModel.BtmTrJncTempBLT3 - R_BrgThrmstr.BthtrTemp5) < PARM(BpToJncBadThr))
            &&  ((R_BrgThrm5pModel.BtmTrJncTempCUT3 - R_BrgThrmstr.BthtrTemp3) < PARM(BpToJncBadThr))
            &&  ((R_BrgThrm5pModel.BtmTrJncTempCLT3 - R_BrgThrmstr.BthtrTemp6) < PARM(BpToJncBadThr))
            &&  R_Running)
          )
       {
           BpToJncBadRaw = true;
       }
       else
       {
           BpToJncBadRaw = false;
       }

       BpToJncBadFil += BpToJncBadGn * (BpToJncBadRaw - BpToJncBadFil);

       //*****End: Hardware Diagnostic--Pump Loss*****//
    }

    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Background Hardware Failure Detection
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void HdwDiag(void)
{
    if ( PARM(HdwDiagEnabled))
    {
       //*****Begin: Hardware Diagnostic--DTA Fan/Pump Fault*****//

       if ( L_Running && ((CfcTemp - InletAirTemp) > PARM(HdwFanFailTempThr)) && (PARM(SoftwareIPN) >= DFIG02_2400_5060_A))
       {
          PUSH_DIAG(HdwBadDtaFanTrp);
       }

       if (( (LqdColingBadFil > DIN_HI_LEVEL && L_Running) && (BpToJncBadFil > DIN_HI_LEVEL)) && PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
       {
           PUSH_DIAG(HdwBadDtaPumpTrp);
       }

       //*****End: Hardware Diagnostic--DTA Fan/Pump Fault*****//

       //*****Begin: Hardware Diagnostic--Voltage/Current Feedback Vco's Faults*****//

       //*****Line Side*****//

       if(!SimMode && (SysTmSec > PARM(HdwSysTmThr)))
       {
           if ( AeadLnVvco1Fil > PARM(HdwVcoHiBadThr) || AeadLnVvco1Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAeadLnVvco1Alm);
           }

           if ( AeadLnVvco2Fil > PARM(HdwVcoHiBadThr) || AeadLnVvco2Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAeadLnVvco2Alm);
           }

           if ( AeadLnVvco3Fil > PARM(HdwVcoHiBadThr) || AeadLnVvco3Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAeadLnVvco3Alm);
           }

           if ( AeadLnIvco5Fil > PARM(HdwVcoHiBadThr) || AeadLnIvco5Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAeadLnIvco5Alm);
           }

           if ( AeadLnIvco6Fil > PARM(HdwVcoHiBadThr) || AeadLnIvco6Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAeadLnIvco6Alm);
           }

           if ( AeadLnIvco7Fil > PARM(HdwVcoHiBadThr) || AeadLnIvco7Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAeadLnIvco7Alm);
           }

           if ( AedbLnVvcoabFil > PARM(HdwVcoHiBadThr) || AedbLnVvcoabFil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAedbLnVvcoabAlm);
           }

           if ( AedbLnVvcobcFil > PARM(HdwVcoHiBadThr) || AedbLnVvcobcFil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAedbLnVvcobcAlm);
           }

           if ( AedbLnIaSfbFil > PARM(HdwVcoHiBadThr) || AedbLnIaSfbFil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAedbLnIaSfbAlm);
           }

           if ( AedbLnIbSfbFil > PARM(HdwVcoHiBadThr) || AedbLnIbSfbFil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAedbLnIbSfbAlm);
           }

           if ( AedbLnIcSfbFil > PARM(HdwVcoHiBadThr) || AedbLnIcSfbFil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAedbLnIcSfbAlm);
           }

           //*****Generator (rotor) Side*****//
           if ( GenIoVvco1Fil > PARM(HdwVcoHiBadThr) || GenIoVvco1Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadGenIoVvco1Alm);
           }

           if ( GenIoVvco2Fil > PARM(HdwVcoHiBadThr) || GenIoVvco2Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadGenIoVvco2Alm);
           }

           if ( GenIoVvco3Fil > PARM(HdwVcoHiBadThr) || GenIoVvco3Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadGenIoVvco3Alm);
           }

           if ( GenIoIvco5Fil > PARM(HdwVcoHiBadThr) || GenIoIvco5Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadGenIoIvco5Alm);
           }

           if ( GenIoIvco6Fil > PARM(HdwVcoHiBadThr) || GenIoIvco6Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadGenIoIvco6Alm);
           }

           if ( GenIoIvco7Fil > PARM(HdwVcoHiBadThr) || GenIoIvco7Fil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadGenIoIvco7Alm);
           }

           if ( AebiGnVvcoabFil > PARM(HdwVcoHiBadThr) || AebiGnVvcoabFil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAebiGVvcoabAlm);
           }

           if ( AebiGnVvcobcFil > PARM(HdwVcoHiBadThr) || AebiGnVvcobcFil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAebiGVvcobcAlm);
           }

           if ( AebiGnIaSfbFil > PARM(HdwVcoHiBadThr) || AebiGnIaSfbFil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAebiGnIaSfbAlm);
           }

           if ( AebiGnIbSfbFil > PARM(HdwVcoHiBadThr) || AebiGnIbSfbFil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAebiGnIbSfbAlm);
           }

           if ( AebiGnIcSfbFil > PARM(HdwVcoHiBadThr) || AebiGnIcSfbFil < PARM(HdwVcoLoBadThr))
           {
               PUSH_DIAG(HdwBadAebiGnIcSfbAlm);
           }
       }
        //*****End: Hardware Diagnostic--Voltage/Current Feedback Vco's Faults*****//
    }
    return;
}
