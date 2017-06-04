///////////////////////////////////////////////////////////////////////////////
//
// TITLE        Initialization and processing of AEAD discrete I/O
//
// DESCRIPTION
//      This file contains functions for initialization, filtering, state
//      mirroring, and mapping of discrete signals pertaining the Alternative
//      Energy Application D card (AEAD).
//
// COPYRIGHT    Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION
//      List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"
#include "x0Aead.h"

// Constants
//----------


// Externals
//----------
extern float       L_AdjTm1Rcp;
extern float       R_AdjTm1Rcp;
extern float       DelTm2;
extern float       S_VabScl;
extern float       GmVMagPuT2;
extern float       S_VMagPU;
extern unsigned    T3CalcDone ;
extern unsigned    BrgEnbSim;
extern unsigned    CfcIsHot;
extern unsigned    SimMode;
extern unsigned    Stopped;
extern unsigned    TripFlt;
extern unsigned    Brg2TripFlt;
extern unsigned    VdcIsOk;
extern unsigned    HdwTstMode;
extern unsigned    S_K1ClsCmd;
extern unsigned    LqdPmpMtrCmd;
extern unsigned    L_MaClsCmd;
extern unsigned    L_CcstrClsCmd;
extern unsigned    EnvOkSealCmd;
extern unsigned    MvSwGearShuntTrp;
extern unsigned    MvSwGearShuntSep;
extern unsigned    PsInrshLimClsCmd;
extern unsigned    DtaFanRunning;
extern unsigned    R_CCOpnCmd;
extern unsigned    R_CCOpnEcn;
extern unsigned    CtrlBrgHtrCmd;
extern unsigned    FilterHtrCmd;
extern unsigned    LqdCoolntHtrCmd;
extern unsigned    AcEntryHtrCmd;
extern unsigned    GmGridMonOKT2;
extern unsigned *  DSP_HIF_ADDR;
extern unsigned *  DSP_DIO_ADDR;

extern unsigned    Q1UVHld;
extern unsigned    Q1ShuntTrip;
extern unsigned    KdfClsCmd;
extern unsigned    Q1ClsPulse;
extern unsigned    DTAFanCmd;
extern unsigned    CContOpnCmd;
extern unsigned    Q1FanCmd;

extern FaultIsActiveTyp     FaultIsActive;

// Types and Classes
//------------------


// Diagnostics
//------------
CREATE_DIAG(HwTstVoltHighAlm);      // Voltage in xfmr secondary is too high for hw test mode


// Parameters
//-----------

//-----------------------------------------------------------------------
// Line 690-Aead discrete I/O
CREATE_PARM(L_AeadDin3W,      float);      // 690 Aead DI 3 filter omega
CREATE_PARM(L_AeadDin4W,      float);      // 690 Aead DI 4 filter omega
CREATE_PARM(L_AeadDin5W,      float);      // 690 Aead DI 5 filter omega
CREATE_PARM(L_AeadDin6W,      float);      // 690 Aead DI 6 filter omega
CREATE_PARM(L_AeadDin9W,      float);      // 690 Aead DI 9 filter omega
CREATE_PARM(L_AeadDin10W,     float);      // 690 Aead DI 10 filter omega

CREATE_PARM(L_AeadK1Tst,      unsigned);   // 690 Aead K1 test-mode value
CREATE_PARM(L_AeadK2Tst,      unsigned);   // 690 Aead K2 test-mode value
CREATE_PARM(L_AeadK3Tst,      unsigned);   // 690 Aead K3 test-mode value
CREATE_PARM(L_AeadK4Tst,      unsigned);   // 690 Aead K4 test-mode value
CREATE_PARM(L_AeadK5Tst,      unsigned);   // 690 Aead K5 test-mode value
CREATE_PARM(L_AeadK6Tst,      unsigned);   // 690 Aead K6 test-mode value
CREATE_PARM(L_AeadK7Tst,      unsigned);   // 690 Aead K7 test-mode value
CREATE_PARM(L_AeadK8Tst,      unsigned);   // 690 Aead K8 test-mode value
CREATE_PARM(L_AeadK9Tst,      unsigned);   // 690 Aead K9 test-mode value


//-----------------------------------------------------------------------
// Rotor 6K-Aead discrete I/O
CREATE_PARM(R_AeadDin3W,       float);      // Aead DI 3 filter omega
CREATE_PARM(R_AeadDin4W,       float);      // Aead DI 4 filter omega
CREATE_PARM(R_AeadDin5W,       float);      // Aead DI 5 filter omega
CREATE_PARM(R_AeadDin6W,       float);      // Aead DI 6 filter omega
CREATE_PARM(R_AeadDin9W,       float);      // Aead DI 9 filter omega
CREATE_PARM(R_AeadDin10W,      float);      // Aead DI 10 filter omega

CREATE_PARM(R_AeadK1Adr,       unsigned *); // Aead K1 test-mode pointer
CREATE_PARM(R_AeadK2Adr,       unsigned *); // Aead K2 test-mode pointer
CREATE_PARM(R_AeadK3Adr,       unsigned *); // Aead K3 test-mode pointer
CREATE_PARM(R_AeadK4Adr,       unsigned *); // Aead K4 test-mode pointer
CREATE_PARM(R_AeadK5Adr,       unsigned *); // Aead K5 test-mode pointer
CREATE_PARM(R_AeadK6Adr,       unsigned *); // Aead K6 test-mode pointer
CREATE_PARM(R_AeadK7Adr,       unsigned *); // Aead K7 test-mode pointer
CREATE_PARM(R_AeadK8Adr,       unsigned *); // Aead K8 test-mode pointer
CREATE_PARM(R_AeadK9Adr,       unsigned *); // Aead K9 test-mode pointer


//-----------------------------------------------------------------------
// Rotor GEN-Aead discrete I/O

CREATE_PARM(R_GenAeadK1Adr,      unsigned *);   // GEN Aead K1 test-mode pointer
CREATE_PARM(R_GenAeadK2Adr,      unsigned *);   // GEN Aead K2 test-mode pointer
CREATE_PARM(R_GenAeadK3Adr,      unsigned *);   // GEN Aead K3 test-mode pointer
CREATE_PARM(R_GenAeadK4Adr,      unsigned *);   // GEN Aead K4 test-mode pointer
CREATE_PARM(R_GenAeadK5Adr,      unsigned *); // GEN Aead K5 test-mode pointer
CREATE_PARM(R_GenAeadK6Adr,      unsigned *); // GEN Aead K6 test-mode pointer
CREATE_PARM(R_GenAeadK7Adr,      unsigned *); // GEN Aead K7 test-mode pointer
CREATE_PARM(R_GenAeadK8Adr,      unsigned *); // GEN Aead K8 test-mode pointer
CREATE_PARM(R_GenAeadK9Adr,      unsigned *); // GEN Aead K9 test-mode pointer

//-----------------------------------------------------------------------
CREATE_PARM(ContVmagPuHwTstW,    float);      // Hardware test-mode contactor close permissive filter omega (Rad/Sec)


// Variables
//----------
CREATE_PUBVAR(L_AeadK1Tcmd,   unsigned);
CREATE_PUBVAR(L_AeadK1TsZ1,   unsigned);
CREATE_PUBVAR(L_AeadK2TsZ1,   unsigned);
CREATE_PUBVAR(L_AeadK5Tcmd,   unsigned);
CREATE_PUBVAR(L_AeadK5TsZ1,   unsigned);

//-----------------------------------------------------------------------
// Line 690-Aead discrete I/O
CREATE_PUBVAR(L_AeadDin10Fl,   float);     // 690 Aead filtered Digital Input 10
CREATE_PUBVAR(L_AeadDin10Gn,   float);     // 690 Aead DI 10 forward gain
CREATE_PUBVAR(L_AeadDin3Fil,   float);     // 690 Aead filtered Digital Input 3
CREATE_PUBVAR(L_AeadDin3Gn,    float);     // 690 Aead DI 3 forward gain
CREATE_PUBVAR(L_AeadDin4Fil,   float);     // 690 Aead filtered Digital Input 4
CREATE_PUBVAR(L_AeadDin4Gn,    float);     // 690 Aead DI 4 forward gain
CREATE_PUBVAR(L_AeadDin5Fil,   float);     // 690 Aead filtered Digital Input 5
CREATE_PUBVAR(L_AeadDin5Gn,    float);     // 690 Aead DI 5 forward gain
CREATE_PUBVAR(L_AeadDin6Fil,   float);     // 690 Aead filtered Digital Input 6
CREATE_PUBVAR(L_AeadDin6Gn,    float);     // 690 Aead DI 6 forward gain
CREATE_PUBVAR(L_AeadDin9Fil,   float);     // 690 Aead filtered Digital Input 9
CREATE_PUBVAR(L_AeadDin9Gn,    float);     // 690 Aead DI 9 forward gain

CREATE_PUBVAR(L_BrgInfcIo1Din3T2, unsigned);  // Line brg interface IO1 dig-input 3 in Task2
CREATE_PUBVAR(L_BrgInfcIo1Din4T2, unsigned);  // Line brg interface IO1 dig-input 4 in Task2
CREATE_PUBVAR(L_BrgInfcIo1Din5T2, unsigned);  // Line brg interface IO1 dig-input 5 in Task2
CREATE_PUBVAR(L_BrgInfcIo1Din6T2, unsigned);  // Line brg interface IO1 dig-input 6 in Task2
CREATE_PUBVAR(L_BrgInfcIo1Din9T2, unsigned);  // Line brg interface IO1 dig-input 9 in Task2
CREATE_PUBVAR(L_BrgInfcIo1Din10T2,unsigned);  // Line brg interface IO1 dig-input 10 in Task2

CREATE_PUBVAR(L_AeadDin3,      unsigned);  // 690 Aead final Digital Input 3
CREATE_PUBVAR(L_AeadDin4,      unsigned);  // 690 Aead final Digital Input 4
CREATE_PUBVAR(L_AeadDin5,      unsigned);  // 690 Aead final Digital Input 5
CREATE_PUBVAR(L_AeadDin6,      unsigned);  // 690 Aead final Digital Input 6
CREATE_PUBVAR(L_AeadDin9,      unsigned);  // 690 Aead final Digital Input 9
CREATE_PUBVAR(L_AeadDin10,     unsigned);  // 690 Aead final Digital Input 10
CREATE_PUBVAR(L_AeadDin9T3,    unsigned);  // Task3 version of 690 Aead final Digital Input 9
CREATE_PUBVAR(L_AeadDin10T3,   unsigned);  // Task3 version of 690 Aead final Digital Input 10

CREATE_PUBVAR(L_AeadK1St,      unsigned);  // 690 Aead mirrored K1 status
CREATE_PUBVAR(L_AeadK2St,      unsigned);  // 690 Aead mirrored K2 status
CREATE_PUBVAR(L_AeadK3St,      unsigned);  // 690 Aead mirrored K3 status
CREATE_PUBVAR(L_AeadK4St,      unsigned);  // 690 Aead mirrored K4 status
CREATE_PUBVAR(L_AeadK5St,      unsigned);  // 690 Aead mirrored K5 status
CREATE_PUBVAR(L_AeadK6St,      unsigned);  // 690 Aead mirrored K6 status
CREATE_PUBVAR(L_AeadK7St,      unsigned);  // 690 Aead mirrored K7 status
CREATE_PUBVAR(L_AeadK8St,      unsigned);  // 690 Aead mirrored K8 status
CREATE_PUBVAR(L_AeadK9St,      unsigned);  // 690 Aead mirrored K9 status

CREATE_PUBVAR(L_AeadK1Out,     unsigned);  // 690 Aead K1 final output
CREATE_PUBVAR(L_AeadK2Out,     unsigned);  // 690 Aead K2 final output
CREATE_PUBVAR(L_AeadK3Out,     unsigned);  // 690 Aead K3 final output
CREATE_PUBVAR(L_AeadK4Out,     unsigned);  // 690 Aead K4 final output
CREATE_PUBVAR(L_AeadK5Out,     unsigned);  // 690 Aead K5 final output
CREATE_PUBVAR(L_AeadK6Out,     unsigned);  // 690 Aead K6 final output
CREATE_PUBVAR(L_AeadK7Out,     unsigned);  // 690 Aead K7 final output
CREATE_PUBVAR(L_AeadK8Out,     unsigned);  // 690 Aead K8 final output
CREATE_PUBVAR(L_AeadK9Out,     unsigned);  // 690 Aead K9 final output

CREATE_PUBVAR(L_AeadK1OutT3, unsigned)=false; // Task3 version of 690 Aead K1 final output
CREATE_PUBVAR(L_AeadK2OutT3, unsigned)=false; // Task3 version of 690 Aead K2 final output
CREATE_PUBVAR(L_AeadK3OutT3, unsigned)=false; // Task3 version of 690 Aead K3 final output
CREATE_PUBVAR(L_AeadK4OutT3, unsigned)=false; // Task3 version of 690 Aead K4 final output
CREATE_PUBVAR(L_AeadK5OutT3, unsigned)=false; // Task3 version of 690 Aead K5 final output
CREATE_PUBVAR(L_AeadK6OutT3, unsigned)=false; // Task3 version of 690 Aead K6 final output
CREATE_PUBVAR(L_AeadK7OutT3, unsigned)=false; // Task3 version of 690 Aead K7 final output
CREATE_PUBVAR(L_AeadK8OutT3, unsigned)=false; // Task3 version of 690 Aead K8 final output
CREATE_PUBVAR(L_AeadK9OutT3, unsigned)=false; // Task3 version of 690 Aead K9 final output

//-----------------------------------------------------------------------
// Rotor 6K-Aead discrete I/O
CREATE_PUBVAR(R_AeadDin10Fl,    float);     // 6KV Aead filtered Digital Input 10
CREATE_PUBVAR(R_AeadDin10Gn,    float);     // 6KV Aead DI 10 forward gain
CREATE_PUBVAR(R_AeadDin3Fil,    float);     // 6KV Aead filtered Digital Input 3
CREATE_PUBVAR(R_AeadDin3Gn,     float);     // 6KV Aead DI 3 forward gain
CREATE_PUBVAR(R_AeadDin4Fil,    float);     // 6KV Aead filtered Digital Input 4
CREATE_PUBVAR(R_AeadDin4Gn,     float);     // 6KV Aead DI 4 forward gain
CREATE_PUBVAR(R_AeadDin5Fil,    float);     // 6KV Aead filtered Digital Input 5
CREATE_PUBVAR(R_AeadDin5Gn,     float);     // 6KV Aead DI 5 forward gain
CREATE_PUBVAR(R_AeadDin6Fil,    float);     // 6KV Aead filtered Digital Input 6
CREATE_PUBVAR(R_AeadDin6Gn,     float);     // 6KV Aead DI 6 forward gain
CREATE_PUBVAR(R_AeadDin9Fil,    float);     // 6KV Aead filtered Digital Input 9
CREATE_PUBVAR(R_AeadDin9Gn,     float);     // 6KV Aead DI 9 forward gain

CREATE_PUBVAR(R_BrgInfcIo1Din3T2, unsigned);  // Rotor brg interface IO1 dig-input 3 in Task2
CREATE_PUBVAR(R_BrgInfcIo1Din4T2, unsigned);  // Rotor brg interface IO1 dig-input 4 in Task2
CREATE_PUBVAR(R_BrgInfcIo1Din5T2, unsigned);  // Rotor brg interface IO1 dig-input 5 in Task2
CREATE_PUBVAR(R_BrgInfcIo1Din6T2, unsigned);  // Rotor brg interface IO1 dig-input 6 in Task2
CREATE_PUBVAR(R_BrgInfcIo1Din9T2, unsigned);  // Rotor brg interface IO1 dig-input 9 in Task2
CREATE_PUBVAR(R_BrgInfcIo1Din10T2,unsigned);  // Rotor brg interface IO1 dig-input 10 in Task2

CREATE_PUBVAR(R_AeadDin3,       unsigned);  // Aead final Digital Input 3
CREATE_PUBVAR(R_AeadDin4,       unsigned);  // Aead final Digital Input 4
CREATE_PUBVAR(R_AeadDin5,       unsigned);  // Aead final Digital Input 5
CREATE_PUBVAR(R_AeadDin6,       unsigned);  // Aead final Digital Input 6
CREATE_PUBVAR(R_AeadDin9,       unsigned);  // Aead final Digital Input 9
CREATE_PUBVAR(R_AeadDin10,      unsigned);  // Aead final Digital Input 10
CREATE_PUBVAR(R_AeadDin9T3,    unsigned);   // Task3 version of Aead final Digital Input 9
CREATE_PUBVAR(R_AeadDin10T3,   unsigned);   // Task3 version of Aead final Digital Input 10

CREATE_PUBVAR(R_AeadK1St,       unsigned);  // 6KV Aead mirrored K1 status
CREATE_PUBVAR(R_AeadK2St,       unsigned);  // 6KV Aead mirrored K2 status
CREATE_PUBVAR(R_AeadK3St,       unsigned);  // 6KV Aead mirrored K3 status
CREATE_PUBVAR(R_AeadK4St,       unsigned);  // 6KV Aead mirrored K4 status
CREATE_PUBVAR(R_AeadK5St,       unsigned);  // 6KV Aead mirrored K5 status
CREATE_PUBVAR(R_AeadK6St,       unsigned);  // 6KV Aead mirrored K6 status
CREATE_PUBVAR(R_AeadK7St,       unsigned);  // 6KV Aead mirrored K7 status
CREATE_PUBVAR(R_AeadK8St,       unsigned);  // 6KV Aead mirrored K8 status
CREATE_PUBVAR(R_AeadK9St,       unsigned);  // 6KV Aead mirrored K9 status

CREATE_PUBVAR(R_AeadK1Out,      unsigned);  // 6KV Aead K1 final output
CREATE_PUBVAR(R_AeadK2Out,      unsigned);  // 6KV Aead K2 final output
CREATE_PUBVAR(R_AeadK3Out,      unsigned);  // 6KV Aead K3 final output
CREATE_PUBVAR(R_AeadK4Out,      unsigned);  // 6KV Aead K4 final output
CREATE_PUBVAR(R_AeadK5Out,      unsigned);  // 6KV Aead K5 final output
CREATE_PUBVAR(R_AeadK6Out,      unsigned);  // 6KV Aead K6 final output
CREATE_PUBVAR(R_AeadK7Out,      unsigned);  // 6KV Aead K7 final output
CREATE_PUBVAR(R_AeadK8Out,      unsigned);  // 6KV Aead K8 final output
CREATE_PUBVAR(R_AeadK9Out,      unsigned);  // 6KV Aead K9 final output

CREATE_PUBVAR(R_AeadK1OutT3, unsigned)=false; // Task3 version of 6KV Aead K1 final output
CREATE_PUBVAR(R_AeadK2OutT3, unsigned)=false; // Task3 version of 6KV Aead K2 final output
CREATE_PUBVAR(R_AeadK3OutT3, unsigned)=false; // Task3 version of 6KV Aead K3 final output
CREATE_PUBVAR(R_AeadK4OutT3, unsigned)=false; // Task3 version of 6KV Aead K4 final output
CREATE_PUBVAR(R_AeadK5OutT3, unsigned)=false; // Task3 version of 6KV Aead K5 final output
CREATE_PUBVAR(R_AeadK6OutT3, unsigned)=false; // Task3 version of 6KV Aead K6 final output
CREATE_PUBVAR(R_AeadK7OutT3, unsigned)=false; // Task3 version of 6KV Aead K7 final output
CREATE_PUBVAR(R_AeadK8OutT3, unsigned)=false; // Task3 version of 6KV Aead K8 final output
CREATE_PUBVAR(R_AeadK9OutT3, unsigned)=false; // Task3 version of 6KV Aead K9 final output

//-----------------------------------------------------------------------
// Rotor GEN-Aead discrete I/O
CREATE_PUBVAR(R_GenAeadDin10Fl,   float);     // GEN Aead filtered Digital Input 10
CREATE_PUBVAR(R_GenAeadDin10Gn,   float);     // GEN Aead DI 10 forward gain
CREATE_PUBVAR(R_GenAeadDin3Fil,   float);     // GEN Aead filtered Digital Input 3
CREATE_PUBVAR(R_GenAeadDin3Gn,    float);     // GEN Aead DI 3 forward gain
CREATE_PUBVAR(R_GenAeadDin4Fil,   float);     // GEN Aead filtered Digital Input 4
CREATE_PUBVAR(R_GenAeadDin4Gn,    float);     // GEN Aead DI 4 forward gain
CREATE_PUBVAR(R_GenAeadDin5Fil,   float);     // GEN Aead filtered Digital Input 5
CREATE_PUBVAR(R_GenAeadDin5Gn,    float);     // GEN Aead DI 5 forward gain
CREATE_PUBVAR(R_GenAeadDin6Fil,   float);     // GEN Aead filtered Digital Input 6
CREATE_PUBVAR(R_GenAeadDin6Gn,    float);     // GEN Aead DI 6 forward gain
CREATE_PUBVAR(R_GenAeadDin9Fil,   float);     // GEN Aead filtered Digital Input 9
CREATE_PUBVAR(R_GenAeadDin9Gn,    float);     // GEN Aead DI 9 forward gain

CREATE_PUBVAR(R_BrgInfcIo2Din3T2, unsigned);  // Rotor brg interface IO2 dig-input 3 in Task2
CREATE_PUBVAR(R_BrgInfcIo2Din4T2, unsigned);  // Rotor brg interface IO2 dig-input 4 in Task2
CREATE_PUBVAR(R_BrgInfcIo2Din5T2, unsigned);  // Rotor brg interface IO2 dig-input 5 in Task2
CREATE_PUBVAR(R_BrgInfcIo2Din6T2, unsigned);  // Rotor brg interface IO2 dig-input 6 in Task2
CREATE_PUBVAR(R_BrgInfcIo2Din9T2, unsigned);  // Rotor brg interface IO2 dig-input 9 in Task2
CREATE_PUBVAR(R_BrgInfcIo2Din10T2,unsigned);  // Rotor brg interface IO2 dig-input 10 in Task2

CREATE_PUBVAR(R_GenAeadDin3,      unsigned);  // GEN Aead final Digital Input 3
CREATE_PUBVAR(R_GenAeadDin4,      unsigned);  // GEN Aead final Digital Input 4
CREATE_PUBVAR(R_GenAeadDin5,      unsigned);  // GEN Aead final Digital Input 5
CREATE_PUBVAR(R_GenAeadDin6,      unsigned);  // GEN Aead final Digital Input 6
CREATE_PUBVAR(R_GenAeadDin9,      unsigned);  // GEN Aead final Digital Input 9
CREATE_PUBVAR(R_GenAeadDin10,     unsigned);  // GEN Aead final Digital Input 10
CREATE_PUBVAR(R_GenAeadDin9T3,    unsigned);  // Task3 version Gen Aead final Digital Input 9
CREATE_PUBVAR(R_GenAeadDin10T3,   unsigned);  // Task3 version Gen Aead final Digital Input 10

CREATE_PUBVAR(R_GenAeadK1St,      unsigned);  // GEN Aead mirrored K1 status
CREATE_PUBVAR(R_GenAeadK2St,      unsigned);  // GEN Aead mirrored K2 status
CREATE_PUBVAR(R_GenAeadK3St,      unsigned);  // GEN Aead mirrored K3 status       
CREATE_PUBVAR(R_GenAeadK4St,      unsigned);  // GEN Aead mirrored K4 status
CREATE_PUBVAR(R_GenAeadK5St,      unsigned);  // GEN Aead mirrored K5 status
CREATE_PUBVAR(R_GenAeadK6St,      unsigned);  // GEN Aead mirrored K6 status
CREATE_PUBVAR(R_GenAeadK7St,      unsigned);  // GEN Aead mirrored K7 status
CREATE_PUBVAR(R_GenAeadK8St,      unsigned);  // GEN Aead mirrored K8 status
CREATE_PUBVAR(R_GenAeadK9St,      unsigned);  // GEN Aead mirrored K9 status

CREATE_PUBVAR(R_GenAeadK1Out,     unsigned);  // GEN Aead K1 final output
CREATE_PUBVAR(R_GenAeadK2Out,     unsigned);  // GEN Aead K2 final output
CREATE_PUBVAR(R_GenAeadK3Out,     unsigned);  // GEN Aead K3 final output
CREATE_PUBVAR(R_GenAeadK4Out,     unsigned);  // GEN Aead K4 final output
CREATE_PUBVAR(R_GenAeadK5Out,     unsigned);  // GEN Aead K5 final output
CREATE_PUBVAR(R_GenAeadK6Out,     unsigned);  // GEN Aead K6 final output
CREATE_PUBVAR(R_GenAeadK7Out,     unsigned);  // GEN Aead K7 final output
CREATE_PUBVAR(R_GenAeadK8Out,     unsigned);  // GEN Aead K8 final output
CREATE_PUBVAR(R_GenAeadK9Out,     unsigned);  // GEN Aead K9 final output

CREATE_PUBVAR(R_GenAeadK1OutT3, unsigned)=false; // Task3 version of GEN Aead K1 final output
CREATE_PUBVAR(R_GenAeadK2OutT3, unsigned)=false; // Task3 version of GEN Aead K2 final output
CREATE_PUBVAR(R_GenAeadK3OutT3, unsigned)=false; // Task3 version of GEN Aead K3 final output
CREATE_PUBVAR(R_GenAeadK4OutT3, unsigned)=false; // Task3 version of GEN Aead K4 final output
CREATE_PUBVAR(R_GenAeadK5OutT3, unsigned)=false; // Task3 version of GEN Aead K5 final output
CREATE_PUBVAR(R_GenAeadK6OutT3, unsigned)=false; // Task3 version of GEN Aead K6 final output
CREATE_PUBVAR(R_GenAeadK7OutT3, unsigned)=false; // Task3 version of GEN Aead K7 final output
CREATE_PUBVAR(R_GenAeadK8OutT3, unsigned)=false; // Task3 version of GEN Aead K8 final output
CREATE_PUBVAR(R_GenAeadK9OutT3, unsigned)=false; // Task3 version of GEN Aead K9 final output

//-----------------------------------------------------------------------
CREATE_PUBVAR(GmVmagPuHwTst,      float);     // filtered GmVMagPu for device close permissive
CREATE_PUBVAR(S_VmagPuHwTst,      float);     // filtered S_VMagPu for detection of voltage presence
CREATE_PUBVAR(ContVmagPuHwTstGn,  float);     // filter gain for contactor close permissive in hardware test-mode
CREATE_PUBVAR(S_HwTstVmagLow, unsigned)=false;// active when 6KV voltage feedbacks are low enough for K1 to be closed
CREATE_PUBVAR(R_AeadDin9Dly,unsigned)=false;// delayed version of R_AeadDin9
CREATE_PUBVAR(R_AeadDin9DlyTmr, float);     // timer for R_AeadDin9Dly

CREATE_PUBVAR(TachValueZ1,  unsigned);

// Data Passing
//--------------
DATA_PASS(L_BridgeRebf.ContactIn3Io1,  L_BrgInfcIo1Din3T2,  T1_T2, UNFILTERED);
DATA_PASS(L_BridgeRebf.ContactIn4Io1,  L_BrgInfcIo1Din4T2,  T1_T2, UNFILTERED);
DATA_PASS(L_BridgeRebf.ContactIn5Io1,  L_BrgInfcIo1Din5T2,  T1_T2, UNFILTERED);
DATA_PASS(L_BridgeRebf.ContactIn6Io1,  L_BrgInfcIo1Din6T2,  T1_T2, UNFILTERED);
DATA_PASS(L_BridgeRebf.ContactIn9Io1,  L_BrgInfcIo1Din9T2,  T1_T2, UNFILTERED);
DATA_PASS(L_BridgeRebf.ContactIn10Io1, L_BrgInfcIo1Din10T2, T1_T2, UNFILTERED);

DATA_PASS(R_BridgeRebf.ContactIn3Io1,  R_BrgInfcIo1Din3T2,  T1_T2, UNFILTERED);
DATA_PASS(R_BridgeRebf.ContactIn4Io1,  R_BrgInfcIo1Din4T2,  T1_T2, UNFILTERED);
DATA_PASS(R_BridgeRebf.ContactIn5Io1,  R_BrgInfcIo1Din5T2,  T1_T2, UNFILTERED);
DATA_PASS(R_BridgeRebf.ContactIn6Io1,  R_BrgInfcIo1Din6T2,  T1_T2, UNFILTERED);
DATA_PASS(R_BridgeRebf.ContactIn9Io1,  R_BrgInfcIo1Din9T2,  T1_T2, UNFILTERED);
DATA_PASS(R_BridgeRebf.ContactIn10Io1, R_BrgInfcIo1Din10T2, T1_T2, UNFILTERED);

DATA_PASS(L_AeadK1Out,  L_AeadK1OutT3, T2_T3, UNFILTERED);
DATA_PASS(L_AeadK2Out,  L_AeadK2OutT3, T2_T3, UNFILTERED);
DATA_PASS(L_AeadK3Out,  L_AeadK3OutT3, T2_T3, UNFILTERED);
DATA_PASS(L_AeadK4Out,  L_AeadK4OutT3, T2_T3, UNFILTERED);
DATA_PASS(L_AeadK5Out,  L_AeadK5OutT3, T2_T3, UNFILTERED);
DATA_PASS(L_AeadK6Out,  L_AeadK6OutT3, T2_T3, UNFILTERED);
DATA_PASS(L_AeadK7Out,  L_AeadK7OutT3, T2_T3, UNFILTERED);
DATA_PASS(L_AeadK8Out,  L_AeadK8OutT3, T2_T3, UNFILTERED);
DATA_PASS(L_AeadK9Out,  L_AeadK9OutT3, T2_T3, UNFILTERED);



DATA_PASS(R_AeadK1Out,   R_AeadK1OutT3,  T2_T3, UNFILTERED);
DATA_PASS(R_AeadK2Out,   R_AeadK2OutT3,  T2_T3, UNFILTERED);
DATA_PASS(R_AeadK3Out,   R_AeadK3OutT3,  T2_T3, UNFILTERED);
DATA_PASS(R_AeadK4Out,   R_AeadK4OutT3,  T2_T3, UNFILTERED);
DATA_PASS(R_AeadK5Out,   R_AeadK5OutT3,  T2_T3, UNFILTERED);
DATA_PASS(R_AeadK6Out,   R_AeadK6OutT3,  T2_T3, UNFILTERED);
DATA_PASS(R_AeadK7Out,   R_AeadK7OutT3,  T2_T3, UNFILTERED);
DATA_PASS(R_AeadK8Out,   R_AeadK8OutT3,  T2_T3, UNFILTERED);
DATA_PASS(R_AeadK9Out,   R_AeadK9OutT3,  T2_T3, UNFILTERED);

DATA_PASS(R_GenAeadK1Out,  R_GenAeadK1OutT3, T2_T3, UNFILTERED);
DATA_PASS(R_GenAeadK2Out,  R_GenAeadK2OutT3, T2_T3, UNFILTERED);
DATA_PASS(R_GenAeadK3Out,  R_GenAeadK3OutT3, T2_T3, UNFILTERED);
DATA_PASS(R_GenAeadK4Out,  R_GenAeadK4OutT3, T2_T3, UNFILTERED);
DATA_PASS(R_GenAeadK5Out,  R_GenAeadK5OutT3, T2_T3, UNFILTERED);
DATA_PASS(R_GenAeadK6Out,  R_GenAeadK6OutT3, T2_T3, UNFILTERED);
DATA_PASS(R_GenAeadK7Out,  R_GenAeadK7OutT3, T2_T3, UNFILTERED);
DATA_PASS(R_GenAeadK8Out,  R_GenAeadK8OutT3, T2_T3, UNFILTERED);
DATA_PASS(R_GenAeadK9Out,  R_GenAeadK9OutT3, T2_T3, UNFILTERED);

DATA_PASS(L_AeadDin9 ,  L_AeadDin9T3,  T2_T3, UNFILTERED);
DATA_PASS(L_AeadDin10,  L_AeadDin10T3, T2_T3, UNFILTERED);

DATA_PASS(R_AeadDin9 ,  R_AeadDin9T3,  T2_T3, UNFILTERED);
DATA_PASS(R_AeadDin10,  R_AeadDin10T3, T2_T3, UNFILTERED);

DATA_PASS(R_GenAeadDin9,    R_GenAeadDin9T3,   T2_T3, UNFILTERED);
DATA_PASS(R_GenAeadDin10,   R_GenAeadDin10T3,  T2_T3, UNFILTERED);

// Local Prototypes (to resolve forward references)
//-------------------------------------------------
void   DigInCalcConstAead();


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Debounce Aead discrete I/O signals.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  ReadDigitalInsAead()
{
    // Filter individual line digital-inputs
    //--------------------------------------
    L_AeadDin3Fil += L_AeadDin3Gn  * (static_cast<int> (L_BrgInfcIo1Din3T2)  - L_AeadDin3Fil);
    L_AeadDin4Fil += L_AeadDin4Gn  * (static_cast<int> (L_BrgInfcIo1Din4T2)  - L_AeadDin4Fil);
    L_AeadDin5Fil += L_AeadDin5Gn  * (static_cast<int> (L_BrgInfcIo1Din5T2)  - L_AeadDin5Fil);
    L_AeadDin6Fil += L_AeadDin6Gn  * (static_cast<int> (L_BrgInfcIo1Din6T2)  - L_AeadDin6Fil);
    L_AeadDin9Fil += L_AeadDin9Gn  * (static_cast<int> (L_BrgInfcIo1Din9T2)  - L_AeadDin9Fil);
    L_AeadDin10Fl += L_AeadDin10Gn * (static_cast<int> (L_BrgInfcIo1Din10T2) - L_AeadDin10Fl);


    if (T3CalcDone)
    {
        // Check against hysteresis
        if (L_AeadDin3Fil <= DIN_LOW_LVL)
        {
            L_AeadDin3 = false;
        }
        else if (L_AeadDin3Fil >= DIN_HI_LVL)
        {
            L_AeadDin3 = true;
        }

        if (L_AeadDin4Fil <= DIN_LOW_LVL)
        {
            L_AeadDin4 = false;
        }
        else if (L_AeadDin4Fil >= DIN_HI_LVL)
        {
            L_AeadDin4 = true;
        }

        if (L_AeadDin5Fil <= DIN_LOW_LVL)
        {
            L_AeadDin5 = false;
        }
        else if (L_AeadDin5Fil >= DIN_HI_LVL)
        {
            L_AeadDin5 = true;
        }

        if (L_AeadDin6Fil <= DIN_LOW_LVL)
        {
            L_AeadDin6 = false;
        }
        else if (L_AeadDin6Fil >= DIN_HI_LVL)
        {
            L_AeadDin6 = true;
        }

        if (L_AeadDin9Fil <= DIN_LOW_LVL)
        {
            L_AeadDin9 = false;
        }
        else if (L_AeadDin9Fil >= DIN_HI_LVL)
        {
            L_AeadDin9 = true;
        }

        if (L_AeadDin10Fl <= DIN_LOW_LVL)
        {
            L_AeadDin10 = false;
        }
        else if (L_AeadDin10Fl >= DIN_HI_LVL)
        {
            L_AeadDin10 = true;
        }
    }

    // Filter individual rotor digital-inputs
    //--------------------------------------
    R_AeadDin3Fil += R_AeadDin3Gn * (static_cast<int> (R_BrgInfcIo1Din3T2)  - R_AeadDin3Fil);
    R_AeadDin4Fil += R_AeadDin4Gn * (static_cast<int> (R_BrgInfcIo1Din4T2)  - R_AeadDin4Fil);
    R_AeadDin5Fil += R_AeadDin5Gn * (static_cast<int> (R_BrgInfcIo1Din5T2)  - R_AeadDin5Fil);
    R_AeadDin6Fil += R_AeadDin6Gn * (static_cast<int> (R_BrgInfcIo1Din6T2)  - R_AeadDin6Fil);
    R_AeadDin9Fil += R_AeadDin9Gn * (static_cast<int> (R_BrgInfcIo1Din9T2)  - R_AeadDin9Fil);
    R_AeadDin10Fl += R_AeadDin10Gn* (static_cast<int> (R_BrgInfcIo1Din10T2) - R_AeadDin10Fl);


    if (T3CalcDone)
    {
        // Check against hysteresis
        if (R_AeadDin3Fil <= DIN_LOW_LVL)
        {
            R_AeadDin3 = false;
        }
        else if (R_AeadDin3Fil >= DIN_HI_LVL)
        {
            R_AeadDin3 = true;
        }

        if (R_AeadDin4Fil <= DIN_LOW_LVL)
        {
            R_AeadDin4 = false;
        }
        else if (R_AeadDin4Fil >= DIN_HI_LVL)
        {
            R_AeadDin4 = true;
        }

        if (R_AeadDin5Fil <= DIN_LOW_LVL)
        {
            R_AeadDin5 = false;
        }
        else if (R_AeadDin5Fil >= DIN_HI_LVL)
        {
            R_AeadDin5 = true;
        }

        if (R_AeadDin6Fil <= DIN_LOW_LVL)
        {
            R_AeadDin6 = false;
        }
        else if (R_AeadDin6Fil >= DIN_HI_LVL)
        {
            R_AeadDin6 = true;
        }

        if (R_AeadDin9Fil <= DIN_LOW_LVL)
        {
            R_AeadDin9 = false;
        }
        else if (R_AeadDin9Fil >= DIN_HI_LVL)
        {
            R_AeadDin9 = true;
        }

        if (R_AeadDin10Fl <= DIN_LOW_LVL)
        {
            R_AeadDin10 = false;
        }
        else if (R_AeadDin10Fl >= DIN_HI_LVL)
        {
            R_AeadDin10 = true;
        }
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Process Aead relay mapping.
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  ProcessRelaysAead()
{
    unsigned    AeadKxTstSet;
    unsigned    AeadKxTstRst;

    //-------------------
    // Hardware test mode
    if ( HdwTstMode == true )
    {
        AeadKxTstSet = (PARM(L_AeadK1Tst) && !L_AeadK1TsZ1 && VdcIsOk);
        AeadKxTstRst = !( PARM(L_AeadK1Tst) && VdcIsOk );
        L_AeadK1TsZ1 = PARM(L_AeadK1Tst);

        if ( AeadKxTstSet )
        {
            L_AeadK1Tcmd = true;
        }
        else  if ( AeadKxTstRst )
        {
            L_AeadK1Tcmd = false;
        }
        L_AeadK1Out  = L_AeadK1Tcmd;


        L_AeadK2Out = PARM(L_AeadK2Tst) &&  L_AeadK5St && L_AeadK4St;


        L_AeadK3Out  = PARM(L_AeadK3Tst);     // drop Dc charger under any trip fault
        L_AeadK4Out  = PARM(L_AeadK4Tst);

        // allow MV switchgear to be tripped by normal operating logic, even in hardware test mode
        AeadKxTstSet    = (PARM(L_AeadK5Tst) && !L_AeadK5TsZ1  && !CfcIsHot && L_AeadK8St);
        AeadKxTstRst    = !( PARM(L_AeadK5Tst) && !CfcIsHot && L_AeadK8St );

        L_AeadK5TsZ1 = PARM(L_AeadK5Tst);

        if ( AeadKxTstSet )
        {
            L_AeadK5Tcmd = true;
        }
        else  if ( AeadKxTstRst )
        {
            L_AeadK5Tcmd = false;
        }
        L_AeadK5Out  = L_AeadK5Tcmd;

        L_AeadK6Out  = PARM(L_AeadK6Tst);
        L_AeadK7Out  = PARM(L_AeadK7Tst);
        L_AeadK8Out  = PARM(L_AeadK8Tst);
        L_AeadK9Out  = PARM(L_AeadK9Tst);

        //------------------------------------------------------------------
        // Rotor Aead spare relays, configurable under any operating mode
        if ( PARM(SoftwareIPN) >= DFIG02_1850_5060_E)
        {
          if (R_AeadDin9)
          {
              R_AeadDin9DlyTmr += DelTm2;
              if (R_AeadDin9DlyTmr > 0.1F)
              {
                  R_AeadDin9Dly = true;
                  R_AeadDin9DlyTmr = 0.1F;
              }
          }
          else
          {
              R_AeadDin9DlyTmr = 0.0F;
              R_AeadDin9Dly = false;
          }


          if ((int)PARM(R_AeadK1Adr) != LADR_UnusedUInt)
          {
              R_AeadK1Out = gVariable.ReadAsBool(PARM(R_AeadK1Adr)) && R_AeadDin9Dly;
          }
          else
          {
              R_AeadK1Out = false;
          }

          if ((int)PARM(R_AeadK2Adr) != LADR_UnusedUInt)
          {
               R_AeadK2Out = gVariable.ReadAsBool(PARM(R_AeadK2Adr)) && R_AeadDin9Dly;
          }
          else
          {
              R_AeadK2Out = false;
          }

          if ((int)PARM(R_AeadK3Adr) != LADR_UnusedUInt)
          {
              R_AeadK3Out = gVariable.ReadAsBool(PARM(R_AeadK3Adr));
          }
          else
          {
              R_AeadK3Out = false;
          }

          if ((int)PARM(R_AeadK4Adr) != LADR_UnusedUInt)
          {
              R_AeadK4Out = gVariable.ReadAsBool(PARM(R_AeadK4Adr));
          }
          else
          {
              R_AeadK4Out = false;
          }

          if ((int)PARM(R_AeadK5Adr) != LADR_UnusedUInt)
          {
              R_AeadK5Out = gVariable.ReadAsBool(PARM(R_AeadK5Adr));
          }
          else
          {
              R_AeadK5Out = false;
          }


          if ((int)PARM(R_AeadK6Adr) != LADR_UnusedUInt)
          {
              R_AeadK6Out = gVariable.ReadAsBool(PARM(R_AeadK6Adr));
          }
          else
          {
              R_AeadK6Out = false;
          }

          if ((int)PARM(R_AeadK7Adr) != LADR_UnusedUInt)
          {
              R_AeadK7Out = gVariable.ReadAsBool(PARM(R_AeadK7Adr));
          }
          else
          {
              R_AeadK7Out = false;
          }

          if ((int)PARM(R_AeadK8Adr) != LADR_UnusedUInt)
          {
              R_AeadK8Out = gVariable.ReadAsBool(PARM(R_AeadK8Adr));
          }
          else
          {
              R_AeadK8Out = false;
          }

          if ((int)PARM(R_AeadK9Adr) != LADR_UnusedUInt)
          {
              R_AeadK9Out = gVariable.ReadAsBool(PARM(R_AeadK9Adr));
          }
          else
          {
              R_AeadK9Out = false;
          }
        }
	 }


    //-------------------------
    // any other operating mode
    else
    {
        // Line 690-Aead
        //--------------
        L_AeadK1Out  = Q1UVHld && (!PARM(Q1UvHldOpnTst));
        L_AeadK2Out  = S_K1ClsCmd;
        L_AeadK3Out  = R_CCOpnEcn;
        L_AeadK4Out  = R_CCOpnCmd;
        L_AeadK5Out  = KdfClsCmd;
        L_AeadK6Out  = Q1ClsPulse;
        L_AeadK7Out  = LqdPmpMtrCmd;
        L_AeadK8Out  = DTAFanCmd;
        L_AeadK9Out  = GmGridMonOKT2 && !FAULTISACTIVE(Ps24Nok);

        // Rotor 6K-Aead
        //--------------
       //------------------------------------------------------------------
        // Rotor Aead spare relays, configurable under any operating mode
        if ( PARM(SoftwareIPN) >= DFIG02_1850_5060_E)
        {
          if (R_AeadDin9)
          {
              R_AeadDin9DlyTmr += DelTm2;
              if (R_AeadDin9DlyTmr > 0.1F)
              {
                  R_AeadDin9Dly = true;
                  R_AeadDin9DlyTmr = 0.1F;
              }
          }
          else
          {
              R_AeadDin9DlyTmr = 0.0F;
              R_AeadDin9Dly = false;
          }


          if ( PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
          {
              R_AeadK1Out = Q1ShuntTrip;
              R_AeadK5Out = CContOpnCmd;
              R_AeadK6Out = Q1FanCmd;
          }
          else if ( PARM(SoftwareIPN) == DFIG02_1850_5060_E)
          {
              R_AeadK1Out = Q1ShuntTrip;
              R_AeadK5Out = false;
              R_AeadK6Out = false;
          }
          else
          {
              R_AeadK1Out = false;
              R_AeadK5Out = false;
              R_AeadK6Out = false;
          }




          if ((int)PARM(R_AeadK2Adr) != LADR_UnusedUInt)
          {
               R_AeadK2Out = gVariable.ReadAsBool(PARM(R_AeadK2Adr)) && R_AeadDin9Dly;
          }
          else
          {
              R_AeadK2Out = false;
          }

          if ((int)PARM(R_AeadK3Adr) != LADR_UnusedUInt)
          {
              R_AeadK3Out = gVariable.ReadAsBool(PARM(R_AeadK3Adr));
          }
          else
          {
              R_AeadK3Out = false;
          }

          if ((int)PARM(R_AeadK4Adr) != LADR_UnusedUInt)
          {
              R_AeadK4Out = gVariable.ReadAsBool(PARM(R_AeadK4Adr));
          }
          else
          {
              R_AeadK4Out = false;
          }


          if ((int)PARM(R_AeadK7Adr) != LADR_UnusedUInt)
          {
              R_AeadK7Out = gVariable.ReadAsBool(PARM(R_AeadK7Adr));
          }
          else
          {
              R_AeadK7Out = false;
          }

          if ((int)PARM(R_AeadK8Adr) != LADR_UnusedUInt)
          {
              R_AeadK8Out = gVariable.ReadAsBool(PARM(R_AeadK8Adr));
          }
          else
          {
              R_AeadK8Out = false;
          }

          if ((int)PARM(R_AeadK9Adr) != LADR_UnusedUInt)
          {
              R_AeadK9Out = gVariable.ReadAsBool(PARM(R_AeadK9Adr));
          }
          else
          {
              R_AeadK9Out = false;
          }
      }
	}

    //-------------------------------------------------------------------
    // Rotor GEN-Aead spare relays, configurable under any operating mode
    if ((int)PARM(R_GenAeadK1Adr) != LADR_UnusedUInt)
    {
        R_GenAeadK1Out = gVariable.ReadAsBool(PARM(R_GenAeadK1Adr));
    }
    else
    {
        R_GenAeadK1Out = false;
    }

    if ((int)PARM(R_GenAeadK2Adr) != LADR_UnusedUInt)
    {
        R_GenAeadK2Out = gVariable.ReadAsBool(PARM(R_GenAeadK2Adr));
    }
    else
    {
        R_GenAeadK2Out = false;
    }

    if ((int)PARM(R_GenAeadK3Adr) != LADR_UnusedUInt)
    {
        R_GenAeadK3Out = gVariable.ReadAsBool(PARM(R_GenAeadK3Adr));
    }
    else
    {
        R_GenAeadK3Out = false;
    }

    if ((int)PARM(R_GenAeadK4Adr) != LADR_UnusedUInt)
    {
        R_GenAeadK4Out = gVariable.ReadAsBool(PARM(R_GenAeadK4Adr));
    }
    else
    {
        R_GenAeadK4Out = false;
    }

    if ((int)PARM(R_GenAeadK5Adr) != LADR_UnusedUInt)
    {
        R_GenAeadK5Out = gVariable.ReadAsBool(PARM(R_GenAeadK5Adr));
    }
    else
    {
        R_GenAeadK5Out = false;
    }

    if ((int)PARM(R_GenAeadK6Adr) != LADR_UnusedUInt)
    {
        R_GenAeadK6Out = gVariable.ReadAsBool(PARM(R_GenAeadK6Adr));
    }
    else
    {
        R_GenAeadK6Out = false;
    }

    if ((int)PARM(R_GenAeadK7Adr) != LADR_UnusedUInt)
    {
        R_GenAeadK7Out = gVariable.ReadAsBool(PARM(R_GenAeadK7Adr));
    }
    else
    {
        R_GenAeadK7Out = false;
    }

    if ((int)PARM(R_GenAeadK8Adr) != LADR_UnusedUInt)
    {
        R_GenAeadK8Out = gVariable.ReadAsBool(PARM(R_GenAeadK8Adr));
    }
    else
    {
        R_GenAeadK8Out = false;
    }

    if ((int)PARM(R_GenAeadK9Adr) != LADR_UnusedUInt)
    {
        R_GenAeadK9Out = gVariable.ReadAsBool(PARM(R_GenAeadK9Adr));
    }
    else
    {
        R_GenAeadK9Out = false;
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Initialize variable that support discrete I/O processing.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  InitAeadInOuts()
{
    // initialize line 690-Aead discrete I/O signals
    L_AeadK1Out = false;
    L_AeadK2Out = false;
    L_AeadK3Out = false;
    L_AeadK4Out = false;
    L_AeadK5Out = false;
    L_AeadK6Out = false;
    L_AeadK7Out = false;
    L_AeadK8Out = false;
    L_AeadK9Out = false;

    L_AeadDin3  = 0;
    L_AeadDin4  = 0;
    L_AeadDin5  = 0;
    L_AeadDin6  = 0;
    L_AeadDin9  = 0;
    L_AeadDin10 = 0;

    L_AeadDin3Fil = 0;
    L_AeadDin4Fil = 0;
    L_AeadDin5Fil = 0;
    L_AeadDin6Fil = 0;
    L_AeadDin9Fil = 0;
    L_AeadDin10Fl = 0;

    L_AeadK1St = 0;
    L_AeadK2St = 0;
    L_AeadK3St = 0;
    L_AeadK4St = 0;
    L_AeadK5St = 0;
    L_AeadK6St = 0;
    L_AeadK7St = 0;
    L_AeadK8St = 0;
    L_AeadK9St = 0;

    DigInCalcConstAead();

    // initialize rotor 6K-Aead discrete I/O signals
    R_AeadK1Out = false;
    R_AeadK2Out = false;
    R_AeadK3Out = false;
    R_AeadK4Out = false;
    R_AeadK5Out = false;
    R_AeadK6Out = false;
    R_AeadK7Out = false;
    R_AeadK8Out = false;
    R_AeadK9Out = false;

    R_AeadDin3  = 0;
    R_AeadDin4  = 0;
    R_AeadDin5  = 0;
    R_AeadDin6  = 0;
    R_AeadDin9  = 0;
    R_AeadDin10 = 0;

    R_AeadDin3Fil = 0;
    R_AeadDin4Fil = 0;
    R_AeadDin5Fil = 0;
    R_AeadDin6Fil = 0;
    R_AeadDin9Fil = 0;
    R_AeadDin10Fl = 0;

    R_AeadK1St = 0;
    R_AeadK2St = 0;
    R_AeadK3St = 0;
    R_AeadK4St = 0;
    R_AeadK5St = 0;
    R_AeadK6St = 0;
    R_AeadK7St = 0;
    R_AeadK8St = 0;
    R_AeadK9St = 0;

    // initialize rotor GEN-Aead discrete I/O signals
    R_GenAeadK1Out = false;
    R_GenAeadK2Out = false;
    R_GenAeadK3Out = false;
    R_GenAeadK4Out = false;
    R_GenAeadK5Out = false;
    R_GenAeadK6Out = false;
    R_GenAeadK7Out = false;
    R_GenAeadK8Out = false;
    R_GenAeadK9Out = false;

    R_GenAeadDin3  = 0;
    R_GenAeadDin4  = 0;
    R_GenAeadDin5  = 0;
    R_GenAeadDin6  = 0;
    R_GenAeadDin9  = 0;
    R_GenAeadDin10 = 0;

    R_GenAeadDin3Fil = 0;
    R_GenAeadDin4Fil = 0;
    R_GenAeadDin5Fil = 0;
    R_GenAeadDin6Fil = 0;
    R_GenAeadDin9Fil = 0;
    R_GenAeadDin10Fl = 0;

    R_GenAeadK1St = 0;
    R_GenAeadK2St = 0;
    R_GenAeadK3St = 0;
    R_GenAeadK4St = 0;
    R_GenAeadK5St = 0;
    R_GenAeadK6St = 0;
    R_GenAeadK7St = 0;
    R_GenAeadK8St = 0;
    R_GenAeadK9St = 0;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Initialize variable that support edge detection.
//
// EXECUTION LEVEL
//      Initialization and background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  DigInCalcConstAead()
{

    if ( HdwTstMode == false )
    {
       L_AeadK1TsZ1 = true;   //This obligates the toggling of a asserted relay-close-request
       L_AeadK2TsZ1 = true;   //the first time hardware mode is entered
       L_AeadK5TsZ1 = true;
       L_AeadK1Tcmd = false;
       L_AeadK5Tcmd = false;
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//      Mirror the expected states of Aead relays
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void  MirrorAeadStatus()
{
    // Line 690-Aead
    L_AeadK1St = L_AeadK1OutT3 && L_AeadDin9T3;
    L_AeadK2St = L_AeadK2OutT3 && L_AeadDin10T3 && Brg2TripFlt;;
    L_AeadK3St = L_AeadK3OutT3;
    L_AeadK4St = L_AeadK4OutT3;
    L_AeadK5St = L_AeadK5OutT3;
    L_AeadK6St = L_AeadK6OutT3;
    L_AeadK7St = L_AeadK7OutT3;
    L_AeadK8St = L_AeadK8OutT3;
    L_AeadK9St = L_AeadK9OutT3;

    // Rotor 6K-Aead
    R_AeadK1St  = R_AeadK1OutT3 && R_AeadDin9T3;
    R_AeadK2St  = R_AeadK2OutT3 && R_AeadDin10T3;
    R_AeadK3St  = R_AeadK3OutT3;
    R_AeadK4St  = R_AeadK4OutT3;
    R_AeadK5St  = R_AeadK5OutT3;
    R_AeadK6St  = R_AeadK6OutT3;
    R_AeadK7St  = R_AeadK7OutT3;
    R_AeadK8St  = R_AeadK8OutT3;
    R_AeadK9St  = R_AeadK9OutT3;

    // Rotor GEN-Aead
    R_GenAeadK1St = R_GenAeadK1OutT3 && R_GenAeadDin9T3;;
    R_GenAeadK2St = R_GenAeadK2OutT3 && R_GenAeadDin10T3;
    R_GenAeadK3St = R_GenAeadK3OutT3;
    R_GenAeadK4St = R_GenAeadK4OutT3;
    R_GenAeadK5St = R_GenAeadK5OutT3;
    R_GenAeadK6St = R_GenAeadK6OutT3;
    R_GenAeadK7St = R_GenAeadK7OutT3;
    R_GenAeadK8St = R_GenAeadK8OutT3;
    R_GenAeadK9St = R_GenAeadK9OutT3;

    return;
}
