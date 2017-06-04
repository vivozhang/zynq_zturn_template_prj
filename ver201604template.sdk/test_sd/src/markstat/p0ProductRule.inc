///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Top- Level Product Rules
//
// DESCRIPTION:
//      This file contains the top-level rules for the product.  This file
//      is defined uniquely per product and is included by p0Product.cpp.
//      The final value parameters and variables calculated here are defined
//      in p0Product.cpp
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////

{

    float                   TmpRatioSqr;
    float                   SysPwrFactRcp;
    float                   SysVoltNomRcp;
    float                   PLCPwrNPTemp;
    CaptureBufStompParmTyp  TrpBufParm;

    SET_FV(FltRstAdr,       (unsigned*)LADR_UnusedUInt);

    SET_FV(ConverterLoc, FIELD_APPL);       // rule is normal (field)
    										// factory will ovrd to Factory
    										// factory will ovrd to Factory
    if ( !PARM(SimModeReq))
    {
        SET_FV(Net_Type,  NET_IPC);          // Run-Req and Torque-Command come thru IPC even
    }                                       // in pump-back, to realize automatic burn-in profile
    else  //sim mode                        // in the factory.
    {
        SET_FV(Net_Type,  NET_ABSENT);
    }

    if ( PARM(Net_Type) == NET_ABSENT)
    {
        LocalMode = true;
    }
    else     // MarkIVe IOnet
    {
        LocalMode = false;
    }
    ISimMode=PARM(SimModeReq) && !PARM(XSimModeReq);

//------------------------------------------------------------------------------
// Random reference for checking and accepting the content of nonvolatile memory
    SET_FV(NvMemChk, 123456789.0F);

//------------------------------------------------------------------------------
// Trip buffer final value settings -change unused floats to product specific vars

    TrpBufParm.CapChanCnt  = 30;
    TrpBufParm.CapTaskExec = CAP_TASK_1_RATE;
    TrpBufParm.CapPMult    = 1;
    TrpBufParm.CapEnbDelay = 2880.0F;

    TrpBufParm.CapTrigAdr = (void*)LADR_TripFlt;
    TrpBufParm.CapTrigValue = 0;
    TrpBufParm.CapTrigMode = TRIG_BOOL;
    TrpBufParm.CapTrigType = TRIG_EDGE;
    TrpBufParm.CapPreSample = 50;

    TrpBufParm.CapAdr01 = (void*)LADR_L_SeqSt;
    TrpBufParm.CapAdr02 = (void*)LADR_R_SeqSt;
    TrpBufParm.CapAdr03 = (void*)LADR_L_BrgPwrEnb;
    TrpBufParm.CapAdr04 = (void*)LADR_R_BrgPwrEnb;
    TrpBufParm.CapAdr05 = (void*)LADR_L_Vdc;

    TrpBufParm.CapAdr06 = (void*)LADR_L_Vab;
    TrpBufParm.CapAdr07 = (void*)LADR_L_Vbc;
    TrpBufParm.CapAdr08 = (void*)LADR_L_Ia;
    TrpBufParm.CapAdr09 = (void*)LADR_L_Ib;
    TrpBufParm.CapAdr10 = (void*)LADR_L_Ic;

    TrpBufParm.CapAdr11 = (void*)LADR_R_Vab;
    TrpBufParm.CapAdr12 = (void*)LADR_R_Vbc;
    TrpBufParm.CapAdr13 = (void*)LADR_R_Ia;
    TrpBufParm.CapAdr14 = (void*)LADR_R_Ib;
    TrpBufParm.CapAdr15 = (void*)LADR_R_Ic;

    TrpBufParm.CapAdr16 = (void*)LADR_SeqSt;
    TrpBufParm.CapAdr17 = (void*)LADR_S_K1ClsCmd;
    TrpBufParm.CapAdr18 = (void*)LADR_R_CCOpnCmd;

    TrpBufParm.CapAdr19 = (void*)LADR_S_Vab;
    TrpBufParm.CapAdr20 = (void*)LADR_S_Vbc;
    TrpBufParm.CapAdr21 = (void*)LADR_S_Ia;
    TrpBufParm.CapAdr22 = (void*)LADR_S_Ib;
    TrpBufParm.CapAdr23 = (void*)LADR_S_Ic;
    TrpBufParm.CapAdr24 = (void*)LADR_GmAngElecFbk;

    TrpBufParm.CapAdr25 = (void*)LADR_L_BtmTrJncTemp;
    TrpBufParm.CapAdr26 = (void*)LADR_L_BtmDjncTemp;
    TrpBufParm.CapAdr27 = (void*)LADR_R_BtmTrJncTemp;
    TrpBufParm.CapAdr28 = (void*)LADR_R_BtmDjncTemp;
    TrpBufParm.CapAdr29 = (void*)LADR_S_K1Status;
    TrpBufParm.CapAdr30 = (void*)LADR_R_TrqFbk;

    // set final values
    if (TripBuf.RunRules(&TrpBufParm) == FAIL)
    {
        Status = FAIL;
    }


//------------------------------------------------------------------------------
// Buffer Save Disable (Diagnostic)
    SET_FV(PwrDnBufSavDsb,false);       // Enable
    SET_FV(PwrDnBufSavFrc,false);       // Force

//------------------------------------------------------------------------------
// Misc. test related
    SET_FV(GpMathEnb,   false);         // ovrd to true to enable diagnostic math execution
    SET_FV(GpMathDwgEnb,false);         // ovrd to true to enable diagnostic math user diagram
    SET_FV(TocTauTstScl, 1.0F);         // ovrd to > 1 to speed up Toc Tau's

//------------------------------------------------------------------------------
// High level hardware related settings

    SET_FV(ConfigForSep, false);

    SET_FV(L_LineFrq, 2.0F*PI*PARM(L_FrqInput));




//------------------------------------------------------------------------------
// Transformer data

    SET_FV(VxfmrPri,    34500.0F);
    SET_FV(L_VxfmrSec,  690.0F);
    SET_FV(S_XfmrKva,   3000.0F);
    SET_FV(L_KVABasXfmr,2200.0F);
    SET_FV(S_VxfmrSec, 690.0F);
    GmLxfmDt = 0.0F;

    // Moved some turbine parameters from below because they are used to caluclate transformer impedance
    // Local Mode
    if ( PARM(Net_Type) == NET_ABSENT)
    {
        SET_FV(TurbineNpRat, PARM(TurbNpRatLcl));
        SET_FV(AltitudeTurb, 1000.0F);
    }
    else  // MarkVie IONet
    {
        PLCPwrNPTemp = PLCPwrNP;
        if( PLCPwrNPTemp < 500.0F)
        {
           PLCPwrNPTemp = 500.0F;
        }

        SET_FV(TurbineNpRat, PLCPwrNPTemp);
        SET_FV(AltitudeTurb, AlttdTurbRem);
    }

    // TurbineNpRat native unit is in kW
    // For power rating less than 1600kw, add 20kw (loss power) to SysPower
    if ( PARM(TurbineNpRat) <= 1.6e3F)
    {
        // Note TurbineNpRat native unit is in kW and SysPower is in Watts.  Thus, multiply TurbineNpRat by 1e3
        SET_FV(SysPower, (PARM(TurbineNpRat)  * 1.0e3F) + 2.0e4F);
    }
    else
    {
        // For poower rating 1600kW - 1850kw, calculatate SysPower using a linear equation that
        // was established from 2 points as followed:
        // for TurbineNprat = 1600kW, add 20kW loss, thus SysPower = 1620kW
        // for TurbineNpRat = 1850kW, add 35kW loss, thus SysPowe = 1885kW
        // Note TurbineNpRat is in kW and SysPower is in Watts.  Thus, multiply TurbineNpRat by 1.0e3 first

        SET_FV(SysPower, ( (1.06F * ( (PARM(TurbineNpRat) * 1.0e3F) - 1.6e6F) ) + 1.62e6F));
    }

    SET_FV(SysPwrFact, 0.9F);
    SysPwrFactRcp = NoNANLim(1.0F/PARM(SysPwrFact), 1.0e6F, -1.0e6F);
    SET_FV(SysVARat, PARM(SysPower) * SysPwrFactRcp);
    SET_FV(XfmrKva,     PARM(SysVARat));

    if(PARM(SoftwareIPN)>=DFIG02_2400_5060_A)
    {
        SET_FV(XfmrZpct,  6.0); //TODO: need to check
    }
    else
    {
        SET_FV(XfmrZpct,  5.75); //TODO: need to check
    }
    TmpRatioSqr = NoNANLim( (PARM(L_VxfmrSec)*PARM(L_VxfmrSec)) / (PARM(VxfmrPri)*PARM(VxfmrPri)), 1.0e6F, -1.0e6F );
    SET_FV(LxfmrPri,    NoNANLim(PARM(XfmrZpct)*(0.01F/2)*PARM(VxfmrPri)*PARM(VxfmrPri)/PARM(XfmrKva)/PARM(L_LineFrq),1.0e6F,-1.0e6F ));
    SET_FV(RxfmrPri,    PARM(LxfmrPri)*PARM(L_LineFrq)*0.1F);
    SET_FV(L_LxfmrPri,  PARM(LxfmrPri)*TmpRatioSqr);
    SET_FV(L_RxfmrPri,  PARM(RxfmrPri)*TmpRatioSqr);

    SET_FV(L_LxfmrSec,  NoNANLim(PARM(XfmrZpct)*(0.01F/2)*PARM(L_VxfmrSec)*PARM(L_VxfmrSec)/PARM(XfmrKva)/PARM(L_LineFrq),1.0e6F,-1.0e6F ));
    SET_FV(L_RxfmrSec,  PARM(L_LxfmrSec)*PARM(L_LineFrq)*0.1F);


    XfmrRatioLnStr      = NoNANLim( PARM(L_VxfmrSec)/PARM(S_VxfmrSec), 1.0e6F, -1.0e6F );
    XfmrRatioLnStrRcp   = NoNANLim( PARM(S_VxfmrSec)/PARM(L_VxfmrSec), 1.0e6F, -1.0e6F );


    // Support for compensation of transformer leakage inductance
    GmLxfmDt = PARM(L_LxfmrSec) * L_DelTm1Rcp;

    // Lead comp for GmVx(y)Grid calcs
    if (PARM(GmVltCalXfmrWndSel) == SECONDARY_690)
    {
        SET_FV(GmVxyGridLd, 0.5F/PARM(T1FramesPerT2Req));  // using Task1 variables
    }
    else
    {
        SET_FV(GmVxyGridLd, 0.5F/PARM(T1AFramesPerT2Req));  // using Task1A variables
    }

    NoCabFileFlg = CbFrqMsMcFlg = CbRqPrmBdFlg = false;
//------------------------------------------------------------------------------
//   Conveteer
//------------------------------------------------------------------------------
    if ( PARM(ConverterLoc) == FACTORY)
    {
        if ( PARM(TestMode) == PUMP_BACK || PARM(TestMode) == HARDWARE_TEST )
        {
            if( PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
            {
                if ( PARM(L_FrqInput == 60) )
                {   //60hz
                    SET_FV(S_Volts,      690.0F  );
                    SET_FV(S_Amps,       480.0F  );
                    SET_FV(S_Hz,          60.0F  );
                    SET_FV(S_Power,      500e3F  );
                    SET_FV(S_PwrFactor,   0.90F  );
                    SET_FV(S_RRatio,      1.40F  );
                    SET_FV(S_X1,         0.018F  );
                    SET_FV(S_X2,        0.4704F  );
                    SET_FV(S_Xm,          65.0F  );
                    SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
                    SET_FV(S_R1,        0.0036F  );
                    SET_FV(S_R2,        0.0036F  );
                    SET_FV(R_RPMMinOp, 1.0e-32F  );
                    SET_FV(R_RPMSync,   1200.0F  );
                    SET_FV(R_RPMNom,    1600.0F  );
                    SET_FV(R_RPMMaxOp,  2400.0F  );
                    SET_FV(R_TachPPR,   3072     );
                    SET_FV(R_GenInertia, 99.99F  );

                    SET_FV(R_ISpdTempMt, -3.10F  );
                    SET_FV(R_ISpdTempBt, 2400.0F );
                    SET_FV(R_ISpdTempMn, 0.175F  );
                    SET_FV(R_ISpdTempBn, 360.0F  );
                    SET_FV(R_ISpdTempNm, 1520.0F );
                    SET_FV(R_ISpdTempMa,-0.000167F);   // slope
                    SET_FV(R_ISpdTempBa,    1.167F);   // offset
                    R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
                    SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA3,  0.0F );
                    SET_FV(R_ISpdTempA4,  0.0F );
                    SET_FV(R_ISpdTempA5,  0.0F );
                    SET_FV(R_ISpdTempA6,  0.0F );
                    SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA8,  0.0F );
                    SET_FV(R_ISpdTempA9,  0.0F );
                    SET_FV(R_ISpdTempA10,   0.0F );
                    SET_FV(R_ISpdTempA11,   0.0F );
                    SET_FV(R_ISpdTempA12,   0.0F );
                    SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

                    SET_FV(S_ISpdTempMt, -7.9333F);
                    SET_FV(S_ISpdTempBt, 1807.0F );
                    SET_FV(S_ISpdTempMn, 0.250F  );
                    SET_FV(S_ISpdTempBn, 1050.0F );
                    SET_FV(S_ISpdTempNm, 1520.0F );
                    S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
                    SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA3,  0.0F );
                    SET_FV(S_ISpdTempA4,  0.0F );
                    SET_FV(S_ISpdTempA5,  0.0F );
                    SET_FV(S_ISpdTempA6,  0.0F );
                    SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA8,  0.0F );
                    SET_FV(S_ISpdTempA9,  0.0F );
                    SET_FV(S_ISpdTempA10,   0.0F );
                    SET_FV(S_ISpdTempA11,   0.0F );
                    SET_FV(S_ISpdTempA12,   0.0F );
                    SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

                    SET_FV(S_SatCurvFlxMax, 1.2225F);
                    SET_FV(S_SatCurvFlxMin, 0.2565F);
                    SET_FV(S_SatCurvXmMin, 0.638405F);
                    SET_FV(S_SatCurvXmMax, 0.796802F);
                    SET_FV(S_SatCurvK0,     0.9533F);
                    SET_FV(S_SatCurvK1,    -0.9397F);
                    SET_FV(S_SatCurvK2,     1.4779F);
                    SET_FV(S_SatCurvK3,    -0.7525F);
                    SET_FV(S_GenSatCurvVld, false);

                    SET_FV(GenILimTempMax, 70.0F);
                    SET_FV(GenILimTempMin, -20.0F);

                    SET_FV(R_ImaxCrvGenMax, 10000.0F);
                    SET_FV(S_ImaxCrvGenMax, 10000.0F);

                }
                else//50hz
                {
                    SET_FV(S_Volts,       690.0F );
                    SET_FV(S_Amps,        480.0F );
                    SET_FV(S_Hz,           50.0F );
                    SET_FV(S_Power,       500e3F );
                    SET_FV(S_PwrFactor,    0.90F );
                    SET_FV(S_RRatio,     1.4000F );
                    SET_FV(S_X1,          0.015F );
                    SET_FV(S_X2,         0.3920F );
                    SET_FV(S_Xm,           65.0F );
                    SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
                    SET_FV(S_R1,         0.0036F );
                    SET_FV(S_R2,         0.0036F );
                    SET_FV(R_RPMMinOp, 11.0e-32F );
                    SET_FV(R_RPMSync,    1200.0F );
                    SET_FV(R_RPMNom,     1600.0F );
                    SET_FV(R_RPMMaxOp,   2400.0F );
                    SET_FV(R_TachPPR,    3072    );
                    SET_FV(R_GenInertia, 99.99F  );

                    SET_FV(R_ISpdTempMt, -3.10F  );
                    SET_FV(R_ISpdTempBt, 2400.0F );
                    SET_FV(R_ISpdTempMn, 0.175F  );
                    SET_FV(R_ISpdTempBn, 360.0F  );
                    SET_FV(R_ISpdTempNm, 1520.0F );
                    SET_FV(R_ISpdTempMa,-0.000167F);   // slope
                    SET_FV(R_ISpdTempBa,    1.167F);   // offset
                    R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
                    SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA3,  0.0F );
                    SET_FV(R_ISpdTempA4,  0.0F );
                    SET_FV(R_ISpdTempA5,  0.0F );
                    SET_FV(R_ISpdTempA6,  0.0F );
                    SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA8,  0.0F );
                    SET_FV(R_ISpdTempA9,  0.0F );
                    SET_FV(R_ISpdTempA10,   0.0F );
                    SET_FV(R_ISpdTempA11,   0.0F );
                    SET_FV(R_ISpdTempA12,   0.0F );
                    SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

                    SET_FV(S_ISpdTempMt, -7.9333F);
                    SET_FV(S_ISpdTempBt, 1807.0F );
                    SET_FV(S_ISpdTempMn, 0.250F  );
                    SET_FV(S_ISpdTempBn, 1050.0F );
                    SET_FV(S_ISpdTempNm, 1520.0F );
                    S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
                    SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA3,  0.0F );
                    SET_FV(S_ISpdTempA4,  0.0F );
                    SET_FV(S_ISpdTempA5,  0.0F );
                    SET_FV(S_ISpdTempA6,  0.0F );
                    SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA8,  0.0F );
                    SET_FV(S_ISpdTempA9,  0.0F );
                    SET_FV(S_ISpdTempA10,   0.0F );
                    SET_FV(S_ISpdTempA11,   0.0F );
                    SET_FV(S_ISpdTempA12,   0.0F );
                    SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

                    SET_FV(S_SatCurvFlxMax, 1.2225F);
                    SET_FV(S_SatCurvFlxMin, 0.2565F);
                    SET_FV(S_SatCurvXmMin, 0.638405F);
                    SET_FV(S_SatCurvXmMax, 0.796802F);
                    SET_FV(S_SatCurvK0,     0.9533F);
                    SET_FV(S_SatCurvK1,    -0.9397F);
                    SET_FV(S_SatCurvK2,     1.4779F);
                    SET_FV(S_SatCurvK3,    -0.7525F);
                    SET_FV(S_GenSatCurvVld, false);

                    SET_FV(GenILimTempMax, 70.0F);
                    SET_FV(GenILimTempMin, -20.0F);

                    SET_FV(R_ImaxCrvGenMax, 10000.0F);
                    SET_FV(S_ImaxCrvGenMax, 10000.0F);

                }
                #ifdef LEG_TEST
                if ( PARM(TestMode) == LF_LEG_TEST )
                {
                    SET_FV(S_Volts,      690.0F);
                    SET_FV(S_Amps,       480.0F);
                    SET_FV(S_Hz,          60.0F);   // This mode is only supported in 60Hz
                    SET_FV(S_Power,      500e3F);
                    SET_FV(S_PwrFactor,   0.90F);

                    // Set DFIG rotor circuit to match leg-test inductor value for simulator
                    SET_FV(S_RRatio,     1.0000);
                    SET_FV(S_X1,         R_LegTestLeqv * (1.0F/3.0F) * PARM(S_Hz) * PI2 * 0.1F);
                    SET_FV(S_X2,         R_LegTestLeqv * (1.0F/3.0F) * PARM(S_Hz) * PI2 * 0.1F);
                    SET_FV(S_Xm,         R_LegTestLeqv * (1.0F/3.0F) * PARM(S_Hz) * PI2 * 0.9F);
                    SET_FV(S_XST,        R_LegTestLeqv * (1.0F/3.0F) * PARM(S_Hz) * PI2 * 0.2F);
                    SET_FV(S_R1,         0.001F);
                    SET_FV(S_R2,         0.001F);

                    SET_FV(R_RPMMinOp, 1.0e-32F);
                    SET_FV(R_RPMSync,   1200.0F);
                    SET_FV(R_RPMNom,    1600.0F);
                    SET_FV(R_GenInertia, 99.99F);   // Taken from 60Hz Indar generator datasheet
                }
                #endif
            }
            else // 1.6e pumpback
            {
                if ( PARM(L_FrqInput == 60) )
                {   //60hz
                    SET_FV(S_Volts,      690.0F  );
                    SET_FV(S_Amps,       480.0F  );
                    SET_FV(S_Hz,          60.0F  );
                    SET_FV(S_Power,      500e3F  );
                    SET_FV(S_PwrFactor,   0.90F  );
                    SET_FV(S_RRatio,      2.0F  );
                    SET_FV(S_X1,         0.018F  );
                    SET_FV(S_X2,        0.4704F  );
                    SET_FV(S_Xm,          72.0F  );
                    SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
                    SET_FV(S_R1,        0.0036F  );
                    SET_FV(S_R2,        0.0036F  );
                    SET_FV(R_RPMMinOp, 1.0e-32F  );
                    SET_FV(R_RPMSync,   1200.0F  );
                    SET_FV(R_RPMNom,    1600.0F  );
                    SET_FV(R_RPMMaxOp,  2400.0F  );
                    SET_FV(R_TachPPR,   3072     );
                    SET_FV(R_GenInertia, 99.99F  );

                    SET_FV(R_ISpdTempMt, -3.10F  );
                    SET_FV(R_ISpdTempBt, 2400.0F );
                    SET_FV(R_ISpdTempMn, 0.175F  );
                    SET_FV(R_ISpdTempBn, 360.0F  );
                    SET_FV(R_ISpdTempNm, 1520.0F );
                    SET_FV(R_ISpdTempMa,-0.000167F);   // slope
                    SET_FV(R_ISpdTempBa,    1.167F);   // offset
                    R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
                    SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA3,  0.0F );
                    SET_FV(R_ISpdTempA4,  0.0F );
                    SET_FV(R_ISpdTempA5,  0.0F );
                    SET_FV(R_ISpdTempA6,  0.0F );
                    SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA8,  0.0F );
                    SET_FV(R_ISpdTempA9,  0.0F );
                    SET_FV(R_ISpdTempA10,   0.0F );
                    SET_FV(R_ISpdTempA11,   0.0F );
                    SET_FV(R_ISpdTempA12,   0.0F );
                    SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

                    SET_FV(S_ISpdTempMt, -7.9333F);
                    SET_FV(S_ISpdTempBt, 1807.0F );
                    SET_FV(S_ISpdTempMn, 0.250F  );
                    SET_FV(S_ISpdTempBn, 1050.0F );
                    SET_FV(S_ISpdTempNm, 1520.0F );
                    S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
                    SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA3,  0.0F );
                    SET_FV(S_ISpdTempA4,  0.0F );
                    SET_FV(S_ISpdTempA5,  0.0F );
                    SET_FV(S_ISpdTempA6,  0.0F );
                    SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA8,  0.0F );
                    SET_FV(S_ISpdTempA9,  0.0F );
                    SET_FV(S_ISpdTempA10,   0.0F );
                    SET_FV(S_ISpdTempA11,   0.0F );
                    SET_FV(S_ISpdTempA12,   0.0F );
                    SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

                    SET_FV(S_SatCurvFlxMax, 1.2225F);
                    SET_FV(S_SatCurvFlxMin, 0.2565F);
                    SET_FV(S_SatCurvXmMin, 0.638405F);
                    SET_FV(S_SatCurvXmMax, 0.796802F);
                    SET_FV(S_SatCurvK0,     0.9533F);
                    SET_FV(S_SatCurvK1,    -0.9397F);
                    SET_FV(S_SatCurvK2,     1.4779F);
                    SET_FV(S_SatCurvK3,    -0.7525F);
                    SET_FV(S_GenSatCurvVld, false);

                    SET_FV(GenILimTempMax, 70.0F);
                    SET_FV(GenILimTempMin, -20.0F);

                    SET_FV(R_ImaxCrvGenMax, 10000.0F);
                    SET_FV(S_ImaxCrvGenMax, 10000.0F);

                }
                else//50hz
                {
                    SET_FV(S_Volts,       690.0F );
                    SET_FV(S_Amps,        480.0F );
                    SET_FV(S_Hz,           50.0F );
                    SET_FV(S_Power,       500e3F );
                    SET_FV(S_PwrFactor,    0.90F );
                    SET_FV(S_RRatio,     2.0000F );
                    SET_FV(S_X1,          0.015F );
                    SET_FV(S_X2,         0.3920F );
                    SET_FV(S_Xm,           60.0F );
                    SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
                    SET_FV(S_R1,         0.0036F );
                    SET_FV(S_R2,         0.0036F );
                    SET_FV(R_RPMMinOp, 11.0e-32F );
                    SET_FV(R_RPMSync,    1200.0F );
                    SET_FV(R_RPMNom,     1600.0F );
                    SET_FV(R_RPMMaxOp,   2400.0F );
                    SET_FV(R_TachPPR,    3072    );
                    SET_FV(R_GenInertia, 99.99F  );
                    SET_FV(R_ISpdTempMt, -3.10F  );
                    SET_FV(R_ISpdTempBt, 2400.0F );
                    SET_FV(R_ISpdTempMn, 0.175F  );
                    SET_FV(R_ISpdTempBn, 360.0F  );
                    SET_FV(R_ISpdTempNm, 1520.0F );
                    SET_FV(R_ISpdTempMa,-0.000167F);   // slope
                    SET_FV(R_ISpdTempBa,    1.167F);   // offset
                    R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
                    SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA3,  0.0F );
                    SET_FV(R_ISpdTempA4,  0.0F );
                    SET_FV(R_ISpdTempA5,  0.0F );
                    SET_FV(R_ISpdTempA6,  0.0F );
                    SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
                    SET_FV(R_ISpdTempA8,  0.0F );
                    SET_FV(R_ISpdTempA9,  0.0F );
                    SET_FV(R_ISpdTempA10,   0.0F );
                    SET_FV(R_ISpdTempA11,   0.0F );
                    SET_FV(R_ISpdTempA12,   0.0F );
                    SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

                    SET_FV(S_ISpdTempMt, -7.9333F);
                    SET_FV(S_ISpdTempBt, 1807.0F );
                    SET_FV(S_ISpdTempMn, 0.250F  );
                    SET_FV(S_ISpdTempBn, 1050.0F );
                    SET_FV(S_ISpdTempNm, 1520.0F );
                    S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
                    SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA3,  0.0F );
                    SET_FV(S_ISpdTempA4,  0.0F );
                    SET_FV(S_ISpdTempA5,  0.0F );
                    SET_FV(S_ISpdTempA6,  0.0F );
                    SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
                    SET_FV(S_ISpdTempA8,  0.0F );
                    SET_FV(S_ISpdTempA9,  0.0F );
                    SET_FV(S_ISpdTempA10,   0.0F );
                    SET_FV(S_ISpdTempA11,   0.0F );
                    SET_FV(S_ISpdTempA12,   0.0F );
                    SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

                    SET_FV(S_SatCurvFlxMax, 1.2225F);
                    SET_FV(S_SatCurvFlxMin, 0.2565F);
                    SET_FV(S_SatCurvXmMin, 0.638405F);
                    SET_FV(S_SatCurvXmMax, 0.796802F);
                    SET_FV(S_SatCurvK0,     0.9533F);
                    SET_FV(S_SatCurvK1,    -0.9397F);
                    SET_FV(S_SatCurvK2,     1.4779F);
                    SET_FV(S_SatCurvK3,    -0.7525F);
                    SET_FV(S_GenSatCurvVld, false);

                    SET_FV(GenILimTempMax, 70.0F);
                    SET_FV(GenILimTempMin, -20.0F);

                    SET_FV(R_ImaxCrvGenMax, 10000.0F);
                    SET_FV(S_ImaxCrvGenMax, 10000.0F);

                }
            } //if( PARM(SoftwareIPN) >= DFIG02_2400_5060_A)

        } // if ( PARM(TestMode) == PUMP_BACK || PARM(TestMode) == HARDWARE_TEST )


    } //if ( PARM(ConverterLoc) == FACTORY)
    //60hz generators --- Updated from Rev 6 of data sheet on 10/24/07

    else if (PARM(GenTypeCfg) == GENTYPE_JFSA560SR06A_WIN60HZ) //Insert new cases here when new generators are added.
    {
        SET_FV(S_Volts,        690.0F );
        SET_FV(S_Hz,            60.0F );
        SET_FV(S_Power,       2385e3F );
        SET_FV(R_TachPPR,      2048.0 );
        SET_FV(S_PwrFactor,     0.95F );
        SET_FV(S_RRatio,     0.34200F );
        SET_FV(S_X1,           0.034F );
        SET_FV(S_X2,           0.064F );
        SET_FV(S_Xm,           1.040F  );
        SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
        SET_FV(S_R1,          0.00143F);
        SET_FV(S_R2,          0.00148F );
        SET_FV(R_RPMMinOp,     800.0F );
        SET_FV(R_RPMSync,     1200.0F );
        SET_FV(R_RPMNom,      1487.0F );
        SET_FV(R_RPMMaxOp,    1660.0F );
        SET_FV(R_GenInertia,   141.0F );
        SET_FV(R_ISpdTempMt,   -3.600F );
        SET_FV(R_ISpdTempBt,  926.333F );
        SET_FV(R_ISpdTempMn,    0.195F );
        SET_FV(R_ISpdTempBn,  474.959F );
        SET_FV(R_ISpdTempNm,   1487.0F );
        SET_FV(R_ISpdTempMa,  -7.400E-05F );
        SET_FV(R_ISpdTempBa,   1.074E+00F );
        R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
        SET_FV(R_ISpdTempA1,   -5.98700E+00F );
        SET_FV(R_ISpdTempA2,   3.15122E+01F );
        SET_FV(R_ISpdTempA3,   -1.82409E-06F );
        SET_FV(R_ISpdTempA4,  5.93447E-03F );
        SET_FV(R_ISpdTempA5,  -4.80966E-07F );
        SET_FV(R_ISpdTempA6,   6.18385E-04F );
        SET_FV(R_ISpdTempA7,  -2.93573E-01F );
        SET_FV(R_ISpdTempA8,  5.02314E-09F );
        SET_FV(R_ISpdTempA9,   -5.91607E-06F );
        SET_FV(R_ISpdTempA10,  2.20906E-03F );
        SET_FV(R_ISpdTempA11, 1.26081E-10F );
        SET_FV(R_ISpdTempA12,  -1.376360E-12F );
        SET_FV(R_ISpdTempA13, 2.67909E+03F );

        SET_FV(S_ISpdTempMt,   -9.600F );
        SET_FV(S_ISpdTempBt,  2294.00F );
        SET_FV(S_ISpdTempMn,  0.19505F );
        SET_FV(S_ISpdTempBn,  474.959F );
        SET_FV(S_ISpdTempNm,   1487.0F );
        S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
        SET_FV(S_ISpdTempA1,   -1.75107E+01F );
        SET_FV(S_ISpdTempA2,   9.93527E+01F );
        SET_FV(S_ISpdTempA3,   -5.34751E-06F );
        SET_FV(S_ISpdTempA4,  1.73009E-02F );
        SET_FV(S_ISpdTempA5,   -1.39697E-06F );
        SET_FV(S_ISpdTempA6,   1.80146E-03F );
        SET_FV(S_ISpdTempA7,  -8.67023E-01F );
        SET_FV(S_ISpdTempA8,  1.43984E-08F );
        SET_FV(S_ISpdTempA9,   -1.68695E-05F );
        SET_FV(S_ISpdTempA10, 6.28139E-03F );
        SET_FV(S_ISpdTempA11, 3.66705E-10F );
        SET_FV(S_ISpdTempA12,  -3.97024E-12F );
        SET_FV(S_ISpdTempA13, 7.59315E+03F );

        SET_FV(S_SatCurvFlxMax, 1705.28F);
        SET_FV(S_SatCurvFlxMin, 299.768F);
        SET_FV(S_SatCurvXmMin, 0.00123199F);
        SET_FV(S_SatCurvXmMax, 0.00302873F);
        SET_FV(S_SatCurvK0,     0.00352708F);
        SET_FV(S_SatCurvK1,    -1.54983e-6F);
        SET_FV(S_SatCurvK2,     -4.81301e-10F);
        SET_FV(S_SatCurvK3,    3.52379e-13F);
        SET_FV(S_GenSatCurvVld, false);

        SET_FV(GenILimTempMax, 55.0F);
        SET_FV(GenILimTempMin, 35.0F);
        SET_FV(R_ImaxCrvGenMax, 800.0F);
        SET_FV(S_ImaxCrvGenMax, 2000.0F);


        //Previously S_Amps was a fixed entry but no has been replaced with an equation.
        //The equation uses S_Power. However, the previous entry was based on S_Power of 2400 kW.
        //Hence, 15.0 kW was added the parameter S_Power to obtain the same value.
        //In the future, might remove this 15.0 kW addition if it's decided that a slightly lower value is acceptable.
        //Equation is S_Amp = sqrt(((S_Power * R_RPMSync)/(SQRT3 * S_Volt * R_RPMNom))^2 + ((S_Power * sqrt(1 - S_PwrFactor))/(SQRT3 * S_Volt * R_RPMNom))^2)
        SET_FV(S_Amps, sqrt((((PARM(S_Power) + 15000.0F) * PARM(R_RPMSync))/(SQRT3 * PARM(S_Volts) * PARM(R_RPMNom))) *
                            (((PARM(S_Power) + 15000.0F) * PARM(R_RPMSync))/(SQRT3 * PARM(S_Volts) * PARM(R_RPMNom))) +
                            (((PARM(S_Power) + 15000.0F) * sqrt(1 - (PARM(S_PwrFactor) * PARM(S_PwrFactor))))/(SQRT3 * PARM(S_Volts) * PARM(S_PwrFactor))) *
                            (((PARM(S_Power) + 15000.0F) * sqrt(1 - (PARM(S_PwrFactor) * PARM(S_PwrFactor))))/(SQRT3 * PARM(S_Volts) * PARM(S_PwrFactor))))
                           );

     }

    else if (PARM(GenTypeCfg) == GENTYPE_JFEC500SS06A_WIN60HZ)
    {
        SET_FV(S_Volts,        690.0F );
        SET_FV(S_Amps,        1208.0F );
        SET_FV(S_Hz,            60.0F );
        if(PARM(ConverterLoc)!=LABORATORY)
        {
            SET_FV(S_Power,        1645e3F);
            SET_FV(R_TachPPR,        3072 );
        }
        else
        {
            SET_FV(S_Power,        1570e3F);
            SET_FV(R_TachPPR,        2048 );
        }
        SET_FV(S_PwrFactor,     0.90F );
        SET_FV(S_RRatio,     0.35100F );
        SET_FV(S_X1,           0.038F );
        SET_FV(S_X2,           0.056F );
        SET_FV(S_Xm,           0.940F );
        SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
        SET_FV(S_R1,          0.0022F);
        SET_FV(S_R2,          0.002F );
        SET_FV(R_RPMMinOp,     800.0F );
        SET_FV(R_RPMSync,     1200.0F );
        SET_FV(R_RPMNom,      1520.0F );
        SET_FV(R_RPMMaxOp,    1660.0F );
        SET_FV(R_GenInertia,    85.7F );
        SET_FV(R_ISpdTempMt,   -3.16F );
        SET_FV(R_ISpdTempBt,   784.6F );
        SET_FV(R_ISpdTempMn, -0.1580F );
        SET_FV(R_ISpdTempBn,  889.30F );
        SET_FV(R_ISpdTempNm,  1440.0F );
        SET_FV(R_ISpdTempMa,-0.000167F);   // slope
        SET_FV(R_ISpdTempBa,    1.167F);   // offset
        R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
        SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA3,  0.0F );
        SET_FV(R_ISpdTempA4,  0.0F );
        SET_FV(R_ISpdTempA5,  0.0F );
        SET_FV(R_ISpdTempA6,  0.0F );
        SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA8,  0.0F );
        SET_FV(R_ISpdTempA9,  0.0F );
        SET_FV(R_ISpdTempA10,   0.0F );
        SET_FV(R_ISpdTempA11,   0.0F );
        SET_FV(R_ISpdTempA12,   0.0F );
        SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

        SET_FV(S_ISpdTempMt, -6.9540F );
        SET_FV(S_ISpdTempBt,  1725.0F );
        SET_FV(S_ISpdTempMn,  -0.250F );
        SET_FV(S_ISpdTempBn,  1817.0F );
        SET_FV(S_ISpdTempNm,  1440.0F );
        S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
        SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA3,  0.0F );
        SET_FV(S_ISpdTempA4,  0.0F );
        SET_FV(S_ISpdTempA5,  0.0F );
        SET_FV(S_ISpdTempA6,  0.0F );
        SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA8,  0.0F );
        SET_FV(S_ISpdTempA9,  0.0F );
        SET_FV(S_ISpdTempA10,   0.0F );
        SET_FV(S_ISpdTempA11,   0.0F );
        SET_FV(S_ISpdTempA12,   0.0F );
        SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

        SET_FV(S_SatCurvFlxMax, 1705.28F);
        SET_FV(S_SatCurvFlxMin, 299.768F);
        SET_FV(S_SatCurvXmMin, 0.00123199F);
        SET_FV(S_SatCurvXmMax, 0.00302873F);
        SET_FV(S_SatCurvK0,     0.00352708F);
        SET_FV(S_SatCurvK1,    -1.54983e-6F);
        SET_FV(S_SatCurvK2,     -4.81301e-10F);
        SET_FV(S_SatCurvK3,    3.52379e-13F);
        SET_FV(S_GenSatCurvVld, false);

        SET_FV(GenILimTempMax, 70.0F);
        SET_FV(GenILimTempMin, -20.0F);

        SET_FV(R_ImaxCrvGenMax, 10000.0F);
        SET_FV(S_ImaxCrvGenMax, 10000.0F);

    }
    else if ( (PARM(GenTypeCfg) == GENTYPE_TFFOANDQ_HIT60HZ) || (PARM(GenTypeCfg) == GENTYPE_TFFOANDQ_GEHP60HZ) )
    {
        SET_FV(S_Volts,         690.0F );
        SET_FV(S_Amps,         1362.0F );
        SET_FV(S_Hz,             60.0F );
        SET_FV(S_Power,        1885e3F );
        SET_FV(S_PwrFactor,      0.95F );
        SET_FV(S_RRatio,        0.382F );
        SET_FV(S_X1,            0.028F );
        SET_FV(S_X2,            0.049F );
        SET_FV(S_Xm,             1.22F );
        SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
        SET_FV(S_R1,          0.00189F );
        SET_FV(S_R2,          0.00194F );
        SET_FV(R_RPMMinOp,      800.0F );
        SET_FV(R_RPMSync,      1200.0F );
        SET_FV(R_RPMNom,       1490.0F );
        SET_FV(R_RPMMaxOp,     1660.0F );
        SET_FV(R_TachPPR,        3072  );
        SET_FV(R_GenInertia,     98.0F );
        SET_FV(R_ISpdTempMt,  -3.1252F );
        SET_FV(R_ISpdTempBt,   757.04F );
        SET_FV(R_ISpdTempMn,   0.0595F );
        SET_FV(R_ISpdTempBn,   561.95F );
        SET_FV(R_ISpdTempNm,   1440.0F );
        SET_FV(R_ISpdTempMa,-0.000167F);   // slope
        SET_FV(R_ISpdTempBa,    1.167F);   // offset
        R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
        SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA3,  0.0F );
        SET_FV(R_ISpdTempA4,  0.0F );
        SET_FV(R_ISpdTempA5,  0.0F );
        SET_FV(R_ISpdTempA6,  0.0F );
        SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA8,  0.0F );
        SET_FV(R_ISpdTempA9,  0.0F );
        SET_FV(R_ISpdTempA10,   0.0F );
        SET_FV(R_ISpdTempA11,   0.0F );
        SET_FV(R_ISpdTempA12,   0.0F );
        SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
        SET_FV(S_ISpdTempMt,  -6.4134F );
        SET_FV(S_ISpdTempBt,  1617.79F );
        SET_FV(S_ISpdTempMn,   0.0756F );
        SET_FV(S_ISpdTempBn,  1284.52F );
        SET_FV(S_ISpdTempNm,   1440.0F );
        S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
        SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA3,  0.0F );
        SET_FV(S_ISpdTempA4,  0.0F );
        SET_FV(S_ISpdTempA5,  0.0F );
        SET_FV(S_ISpdTempA6,  0.0F );
        SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA8,  0.0F );
        SET_FV(S_ISpdTempA9,  0.0F );
        SET_FV(S_ISpdTempA10,   0.0F );
        SET_FV(S_ISpdTempA11,   0.0F );
        SET_FV(S_ISpdTempA12,   0.0F );
        SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

        SET_FV(S_SatCurvFlxMax, 1705.28F);
        SET_FV(S_SatCurvFlxMin, 299.768F);
        SET_FV(S_SatCurvXmMin, 0.00123199F);
        SET_FV(S_SatCurvXmMax, 0.00302873F);
        SET_FV(S_SatCurvK0,     0.00352708F);
        SET_FV(S_SatCurvK1,    -1.54983e-6F);
        SET_FV(S_SatCurvK2,     -4.81301e-10F);
        SET_FV(S_SatCurvK3,    3.52379e-13F);
        SET_FV(S_GenSatCurvVld, false);

        SET_FV(GenILimTempMax, 70.0F);
        SET_FV(GenILimTempMin, -20.0F);

        SET_FV(R_ImaxCrvGenMax, 10000.0F);
        SET_FV(S_ImaxCrvGenMax, 10000.0F);

    }
    else if (PARM(GenTypeCfg) == GENTYPE_DASAA50244UAA_NTC50H)
    {
        SET_FV(S_Volts,         690.0F );
        SET_FV(S_Amps,         1329.0F );
        SET_FV(S_Hz,             50.0F );
        SET_FV(S_Power,        1885e3F );
        SET_FV(S_PwrFactor,      0.95F );
        SET_FV(S_RRatio,       0.3710F );
        SET_FV(S_X1,           0.0413F );
        SET_FV(S_X2,           0.0407F );
        SET_FV(S_Xm,             1.55F );
        SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
        SET_FV(S_R1,          0.00201F );
        SET_FV(S_R2,          0.00189F );
        SET_FV(R_RPMMinOp,     1000.0F );
        SET_FV(R_RPMSync,      1500.0F );
        SET_FV(R_RPMNom,       1915.0F );
        SET_FV(R_RPMMaxOp,     2060.0F );
        SET_FV(R_TachPPR,        2048  );
        SET_FV(R_GenInertia,    108.0F );

        SET_FV(R_ISpdTempMt,   -3.166F );
        SET_FV(R_ISpdTempBt,    749.6F );
        SET_FV(R_ISpdTempMn,   0.0142F );
        SET_FV(R_ISpdTempBn,    579.9F );
        SET_FV(R_ISpdTempNm,   1915.0F );
        SET_FV(R_ISpdTempMa,-0.000167F);   // slope
        SET_FV(R_ISpdTempBa,    1.167F);   // offset
        R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
        SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA3,  0.0F );
        SET_FV(R_ISpdTempA4,  0.0F );
        SET_FV(R_ISpdTempA5,  0.0F );
        SET_FV(R_ISpdTempA6,  0.0F );
        SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA8,  0.0F );
        SET_FV(R_ISpdTempA9,  0.0F );
        SET_FV(R_ISpdTempA10,   0.0F );
        SET_FV(R_ISpdTempA11,   0.0F );
        SET_FV(R_ISpdTempA12,   0.0F );
        SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

        SET_FV(S_ISpdTempMt,   -7.305F );
        SET_FV(S_ISpdTempBt,   1802.7F );
        SET_FV(S_ISpdTempMn,   0.0151F );
        SET_FV(S_ISpdTempBn,   1445.1F );
        SET_FV(S_ISpdTempNm,   1915.0F );
        S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
        SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA3,  0.0F );
        SET_FV(S_ISpdTempA4,  0.0F );
        SET_FV(S_ISpdTempA5,  0.0F );
        SET_FV(S_ISpdTempA6,  0.0F );
        SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA8,  0.0F );
        SET_FV(S_ISpdTempA9,  0.0F );
        SET_FV(S_ISpdTempA10,   0.0F );
        SET_FV(S_ISpdTempA11,   0.0F );
        SET_FV(S_ISpdTempA12,   0.0F );
        SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

        SET_FV(S_SatCurvFlxMax, 1705.28F);
        SET_FV(S_SatCurvFlxMin, 299.768F);
        SET_FV(S_SatCurvXmMin, 0.00123199F);
        SET_FV(S_SatCurvXmMax, 0.00302873F);
        SET_FV(S_SatCurvK0,     0.00352708F);
        SET_FV(S_SatCurvK1,    -1.54983e-6F);
        SET_FV(S_SatCurvK2,     -4.81301e-10F);
        SET_FV(S_SatCurvK3,    3.52379e-13F);
        SET_FV(S_GenSatCurvVld, false);

        SET_FV(GenILimTempMax, 70.0F);
        SET_FV(GenILimTempMin, -20.0F);

        SET_FV(R_ImaxCrvGenMax, 10000.0F);
        SET_FV(S_ImaxCrvGenMax, 10000.0F);

    }
    else if ( (PARM(GenTypeCfg) == GENTYPE_JFEC500SR04A_WIN50HZ) || (PARM(GenTypeCfg) == GENTYPE_JFEA500SR04A_WIN50HZ) )
    {
        SET_FV(S_Volts,       690.0F );
        SET_FV(S_Amps,       1233.0F );
        SET_FV(S_Hz,           50.0F );
        SET_FV(S_Power,      1645e3F );
        SET_FV(S_PwrFactor,    0.90F );
        SET_FV(S_RRatio,     0.3750F );
        SET_FV(S_X1,         0.0210F );
        SET_FV(S_X2,         0.0420F );
        SET_FV(S_Xm,           1.11F );
        SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
        SET_FV(S_R1,       0.001844F );
        SET_FV(S_R2,       0.001770F );
        SET_FV(R_RPMMinOp,   1000.0F );
        SET_FV(R_RPMSync,    1500.0F );
        SET_FV(R_RPMNom,     1915.0F );
        SET_FV(R_RPMMaxOp,   2060.0F );
        SET_FV(R_TachPPR,      2048  );
        SET_FV(R_GenInertia,   95.0F );
        SET_FV(R_ISpdTempMt, -3.467F );
        SET_FV(R_ISpdTempBt, 788.0F  );
        SET_FV(R_ISpdTempMn, 0.2315F );
        SET_FV(R_ISpdTempBn, 215.3F  );
        SET_FV(R_ISpdTempNm, 1915.0F );
        SET_FV(R_ISpdTempMa,-0.000167F);   // slope
        SET_FV(R_ISpdTempBa,    1.167F);   // offset
        R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
        SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA3,  0.0F );
        SET_FV(R_ISpdTempA4,  0.0F );
        SET_FV(R_ISpdTempA5,  0.0F );
        SET_FV(R_ISpdTempA6,  0.0F );
        SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA8,  0.0F );
        SET_FV(R_ISpdTempA9,  0.0F );
        SET_FV(R_ISpdTempA10,   0.0F );
        SET_FV(R_ISpdTempA11,   0.0F );
        SET_FV(R_ISpdTempA12,   0.0F );
        SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

        SET_FV(S_ISpdTempMt, -7.400F );
        SET_FV(S_ISpdTempBt, 1679.0F );
        SET_FV(S_ISpdTempMn, 0.8100F );
        SET_FV(S_ISpdTempBn, -113.0F );
        SET_FV(S_ISpdTempNm, 1915.0F );
        S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
        SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA3,  0.0F );
        SET_FV(S_ISpdTempA4,  0.0F );
        SET_FV(S_ISpdTempA5,  0.0F );
        SET_FV(S_ISpdTempA6,  0.0F );
        SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA8,  0.0F );
        SET_FV(S_ISpdTempA9,  0.0F );
        SET_FV(S_ISpdTempA10,   0.0F );
        SET_FV(S_ISpdTempA11,   0.0F );
        SET_FV(S_ISpdTempA12,   0.0F );
        SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

        SET_FV(S_SatCurvFlxMax, 1705.28F);
        SET_FV(S_SatCurvFlxMin, 299.768F);
        SET_FV(S_SatCurvXmMin, 0.00123199F);
        SET_FV(S_SatCurvXmMax, 0.00302873F);
        SET_FV(S_SatCurvK0,     0.00352708F);
        SET_FV(S_SatCurvK1,    -1.54983e-6F);
        SET_FV(S_SatCurvK2,     -4.81301e-10F);
        SET_FV(S_SatCurvK3,    3.52379e-13F);
        SET_FV(S_GenSatCurvVld, false);

        SET_FV(GenILimTempMax, 70.0F);
        SET_FV(GenILimTempMin, -20.0F);

        SET_FV(R_ImaxCrvGenMax, 10000.0F);
        SET_FV(S_ImaxCrvGenMax, 10000.0F);
    }
    else if(PARM(GenTypeCfg) == GENTYPE_JFEE500SR04A_WIN50HZ)
    {
        SET_FV(S_Volts,        690.0F );
        SET_FV(S_Amps,        1330.0F );
        SET_FV(S_Hz,            50.0F );
        SET_FV(S_Power,       1885e3F );
        SET_FV(S_PwrFactor,     0.95F );
        SET_FV(S_RRatio,      0.3586F );
        SET_FV(S_X1,          0.0440F );
        SET_FV(S_X2,          0.0600F );
        SET_FV(S_Xm,            1.82F );
        SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
        SET_FV(S_R1,          0.0021F );
        SET_FV(S_R2,          0.0023F );
        SET_FV(R_RPMMinOp,    1000.0F );
        SET_FV(R_RPMSync,     1500.0F );
        SET_FV(R_RPMNom,      1915.0F );
        SET_FV(R_RPMMaxOp,    2090.0F );
        SET_FV(R_TachPPR,       3072  );
        SET_FV(R_GenInertia,    78.0F );
        SET_FV(R_ISpdTempMt,   -2.95F );
        SET_FV(R_ISpdTempBt,  715.60F );
        SET_FV(R_ISpdTempMn,    0.11F );
        SET_FV(R_ISpdTempBn,  397.59F );
        SET_FV(R_ISpdTempNm,  1876.0F );
        SET_FV(R_ISpdTempMa,-0.000167F);   // slope
        SET_FV(R_ISpdTempBa,    1.167F);   // offset
        R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
        SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA3,  0.0F );
        SET_FV(R_ISpdTempA4,  0.0F );
        SET_FV(R_ISpdTempA5,  0.0F );
        SET_FV(R_ISpdTempA6,  0.0F );
        SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA8,  0.0F );
        SET_FV(R_ISpdTempA9,  0.0F );
        SET_FV(R_ISpdTempA10,   0.0F );
        SET_FV(R_ISpdTempA11,   0.0F );
        SET_FV(R_ISpdTempA12,   0.0F );
        SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

        SET_FV(S_ISpdTempMt,   -7.61F );
        SET_FV(S_ISpdTempBt, 1919.09F );
        SET_FV(S_ISpdTempMn,    0.24F );
        SET_FV(S_ISpdTempBn, 1195.29F );
        SET_FV(S_ISpdTempNm,  1876.0F );
        S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
        SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA3,  0.0F );
        SET_FV(S_ISpdTempA4,  0.0F );
        SET_FV(S_ISpdTempA5,  0.0F );
        SET_FV(S_ISpdTempA6,  0.0F );
        SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA8,  0.0F );
        SET_FV(S_ISpdTempA9,  0.0F );
        SET_FV(S_ISpdTempA10,   0.0F );
        SET_FV(S_ISpdTempA11,   0.0F );
        SET_FV(S_ISpdTempA12,   0.0F );
        SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

        SET_FV(S_SatCurvFlxMax, 1705.28F);
        SET_FV(S_SatCurvFlxMin, 299.768F);
        SET_FV(S_SatCurvXmMin, 0.00123199F);
        SET_FV(S_SatCurvXmMax, 0.00302873F);
        SET_FV(S_SatCurvK0,     0.00352708F);
        SET_FV(S_SatCurvK1,    -1.54983e-6F);
        SET_FV(S_SatCurvK2,     -4.81301e-10F);
        SET_FV(S_SatCurvK3,    3.52379e-13F);
        SET_FV(S_GenSatCurvVld, false);

        SET_FV(GenILimTempMax, 70.0F);
        SET_FV(GenILimTempMin, -20.0F);

        SET_FV(R_ImaxCrvGenMax, 10000.0F);
        SET_FV(S_ImaxCrvGenMax, 10000.0F);
    }
    else if(PARM(GenTypeCfg) == GENTYPE_JFSA560SR06A_WIN50HZ)
    {
        SET_FV(S_Volts,        690.0F );
        SET_FV(S_Hz,            50.0F );
        SET_FV(S_Power,       2385e3F );
        SET_FV(R_TachPPR,     2048.0F );
        SET_FV(S_PwrFactor,     0.95F );
        SET_FV(S_Amps,        1749.0F );
        SET_FV(S_RRatio,      0.3550F );
        SET_FV(S_X1,          0.029F );
        SET_FV(S_X2,          0.053F );
        SET_FV(S_Xm,           1.36F );
        SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
        SET_FV(S_R1,         0.00168F );
        SET_FV(S_R2,         0.00157F );
        SET_FV(R_RPMMinOp,    1000.0F );
        SET_FV(R_RPMSync,     1500.0F );
        SET_FV(R_RPMNom,      1859.0F );
        SET_FV(R_RPMMaxOp,    2090.0F );
        SET_FV(R_GenInertia,   142.0F );
        SET_FV(R_ISpdTempMt,  -4.466F );
        SET_FV(R_ISpdTempBt, 1002.32F );
        SET_FV(R_ISpdTempMn,    .070F );
        SET_FV(R_ISpdTempBn,  607.91F );
        SET_FV(R_ISpdTempNm,  1487.0F );
        SET_FV(R_ISpdTempMa, -7.400E-05F);   // slope
        SET_FV(R_ISpdTempBa, 1.074E+00F);   // offset
        R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
        SET_FV(R_ISpdTempA1,  1.22224E+00F    );
        SET_FV(R_ISpdTempA2,  3.91747E+01F    );
        SET_FV(R_ISpdTempA3,  -3.17275E-08F   );
        SET_FV(R_ISpdTempA4,  -2.88052E-04F   );
        SET_FV(R_ISpdTempA5,  -2.15391E-08F   );
        SET_FV(R_ISpdTempA6,  6.42988E-05F    );
        SET_FV(R_ISpdTempA7,  -8.45638E-02F   );
        SET_FV(R_ISpdTempA8,  -5.42183E-10F   );
        SET_FV(R_ISpdTempA9,  8.82435E-07F    );
        SET_FV(R_ISpdTempA10,   -4.87672E-04F );
        SET_FV(R_ISpdTempA11,   2.68551E-12F  );
        SET_FV(R_ISpdTempA12,   1.11343E-13F  );
        SET_FV(R_ISpdTempA13,   -2.35923E+02F );

        SET_FV(S_ISpdTempMt,-12.1813F );
        SET_FV(S_ISpdTempBt, 2662.59F );
        SET_FV(S_ISpdTempMn,   0.100F );
        SET_FV(S_ISpdTempBn,  1600.3F );
        SET_FV(S_ISpdTempNm,  1487.0F );
        S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
        SET_FV(S_ISpdTempA1,  -8.68679E-01F );
        SET_FV(S_ISpdTempA2,  4.53109E+01F  );
        SET_FV(S_ISpdTempA3,  -6.43007E-07F );
        SET_FV(S_ISpdTempA4,  1.86546E-03F  );
        SET_FV(S_ISpdTempA5,  -1.41680E-07F );
        SET_FV(S_ISpdTempA6,  2.48782E-04F  );
        SET_FV(S_ISpdTempA7,  -1.85270E-01F );
        SET_FV(S_ISpdTempA8,  9.97508E-10F  );
        SET_FV(S_ISpdTempA9,  -1.37351E-06F );
        SET_FV(S_ISpdTempA10,  5.65380E-04F );
        SET_FV(S_ISpdTempA11,  2.87987E-11F );
        SET_FV(S_ISpdTempA12,  -2.29927E-13F );
        SET_FV(S_ISpdTempA13,  1.42098E+03F );

        SET_FV(S_SatCurvFlxMax, 1705.28F);
        SET_FV(S_SatCurvFlxMin, 299.768F);
        SET_FV(S_SatCurvXmMin, 0.00123199F);
        SET_FV(S_SatCurvXmMax, 0.00302873F);
        SET_FV(S_SatCurvK0,     0.00352708F);
        SET_FV(S_SatCurvK1,    -1.54983e-6F);
        SET_FV(S_SatCurvK2,     -4.81301e-10F);
        SET_FV(S_SatCurvK3,    3.52379e-13F);
        SET_FV(S_GenSatCurvVld, false);

        SET_FV(GenILimTempMax, 70.0F);
        SET_FV(GenILimTempMin, -20.0F);

        SET_FV(R_ImaxCrvGenMax, 10000.0F);
        SET_FV(S_ImaxCrvGenMax, 10000.0F);
    }
    else if(PARM(GenTypeCfg) == GENTYPE_NAR560G6_IND60HZ)
    {
        SET_FV(S_Volts,        690.0F );
        SET_FV(S_Hz,            60.0F );
        SET_FV(S_Power,        2400e3F);
        SET_FV(R_TachPPR,        2048 );
        SET_FV(S_PwrFactor,     0.95F );
        SET_FV(S_Amps,        1748.0F );
        SET_FV(S_RRatio,     0.34400F );
        SET_FV(S_X1,          0.0218F );
        SET_FV(S_X2,          0.0353F );
        SET_FV(S_Xm,          0.736F  );
        SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
        SET_FV(S_R1,         0.00192F );
        SET_FV(S_R2,         0.00191F );
        SET_FV(R_RPMMinOp,     800.0F );
        SET_FV(R_RPMSync,     1200.0F );
        SET_FV(R_RPMNom,      1487.0F );
        SET_FV(R_RPMMaxOp,    1660.0F );
        SET_FV(R_GenInertia,   178.0F );
        SET_FV(R_ISpdTempMt,  -4.000F );
        SET_FV(R_ISpdTempBt, 930.333F );
        SET_FV(R_ISpdTempMn,   0.038F );
        SET_FV(R_ISpdTempBn, 689.376F );
        SET_FV(R_ISpdTempNm,  1487.0F );
        SET_FV(R_ISpdTempMa, -7.400E-05F );
        SET_FV(R_ISpdTempBa,  1.074E+00F );
        R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
        SET_FV(R_ISpdTempA1,   6.27329E-01F );
        SET_FV(R_ISpdTempA2,   4.94719E+00F );
        SET_FV(R_ISpdTempA3,   2.56581E-08F );
        SET_FV(R_ISpdTempA4,  -2.20866E-04F );
        SET_FV(R_ISpdTempA5,  -4.14273E-09F );
        SET_FV(R_ISpdTempA6,   1.58628E-05F );
        SET_FV(R_ISpdTempA7,  -2.04389E-02F );
        SET_FV(R_ISpdTempA8,       0.0F );
        SET_FV(R_ISpdTempA9,       0.0F );
        SET_FV(R_ISpdTempA10,      0.0F );
        SET_FV(R_ISpdTempA11,      0.0F );
        SET_FV(R_ISpdTempA12,      0.0F );
        SET_FV(R_ISpdTempA13,  3.99222E+02F );

        SET_FV(S_ISpdTempMt,  -11.20F );
        SET_FV(S_ISpdTempBt, 2264.67F );
        SET_FV(S_ISpdTempMn, 0.31838F );
        SET_FV(S_ISpdTempBn, 1286.65F );
        SET_FV(S_ISpdTempNm,  1487.0F );
        S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
        SET_FV(S_ISpdTempA1,   1.11533E+00F );
        SET_FV(S_ISpdTempA2,   2.88726E+00F );
        SET_FV(S_ISpdTempA3,  -7.22344E-09F );
        SET_FV(S_ISpdTempA4,  -2.61346E-04F );
        SET_FV(S_ISpdTempA5,  -6.18496E-09F );
        SET_FV(S_ISpdTempA6,   2.37180E-05F );
        SET_FV(S_ISpdTempA7,  -3.10567E-02F );
        SET_FV(S_ISpdTempA8,       0.0F );
        SET_FV(S_ISpdTempA9,       0.0F );
        SET_FV(S_ISpdTempA10,      0.0F );
        SET_FV(S_ISpdTempA11,      0.0F );
        SET_FV(S_ISpdTempA12,      0.0F );
        SET_FV(S_ISpdTempA13,  1.26557E+03F );

        SET_FV(S_SatCurvFlxMax, 1705.28F);
        SET_FV(S_SatCurvFlxMin, 299.768F);
        SET_FV(S_SatCurvXmMin, 0.00123199F);
        SET_FV(S_SatCurvXmMax, 0.00302873F);
        SET_FV(S_SatCurvK0,     0.00352708F);
        SET_FV(S_SatCurvK1,    -1.54983e-6F);
        SET_FV(S_SatCurvK2,     -4.81301e-10F);
        SET_FV(S_SatCurvK3,    3.52379e-13F);
        SET_FV(S_GenSatCurvVld, true);

        SET_FV(GenILimTempMax, 55.0F);
        SET_FV(GenILimTempMin, 25.0F);

        SET_FV(R_ImaxCrvGenMax, 10000.0F);
        SET_FV(S_ImaxCrvGenMax, 10000.0F);
    }

    else if(PARM(GenTypeCfg) == GENTYPE_NAR560G6_IND50HZ)
    {
        SET_FV(S_Volts,        690.0F );
        SET_FV(S_Hz,            50.0F );
        SET_FV(S_Power,       2375e3F );
        SET_FV(R_TachPPR,     2048.0F );
        SET_FV(S_PwrFactor,     0.95F );
        SET_FV(S_Amps,        1749.0F );
        SET_FV(S_RRatio,     0.34400F );
        SET_FV(S_X1,          0.0128F );
        SET_FV(S_X2,          0.0360F );
        SET_FV(S_Xm,           0.839F );
        SET_FV(S_XST, 0.90 * (PARM(S_X1) + PARM(S_X2)) );
        SET_FV(S_R1,         0.00204F );
        SET_FV(S_R2,         0.00189F );
        SET_FV(R_RPMMinOp,    1000.0F );
        SET_FV(R_RPMSync,     1500.0F );
        SET_FV(R_RPMNom,      1915.0F );
        SET_FV(R_RPMMaxOp,    2090.0F );
        SET_FV(R_GenInertia,   142.0F );
        SET_FV(R_ISpdTempMt,   -3.16F );
        SET_FV(R_ISpdTempBt,   784.6F );
        SET_FV(R_ISpdTempMn, -0.1580F );
        SET_FV(R_ISpdTempBn,  889.30F );
        SET_FV(R_ISpdTempNm,  1490.0F );
        SET_FV(R_ISpdTempMa, -0.0001315F );
        SET_FV(R_ISpdTempBa,  1.1315F );
        R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
        SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA3,  0.0F );
        SET_FV(R_ISpdTempA4,  0.0F );
        SET_FV(R_ISpdTempA5,  0.0F );
        SET_FV(R_ISpdTempA6,  0.0F );
        SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
        SET_FV(R_ISpdTempA8,  0.0F );
        SET_FV(R_ISpdTempA9,  0.0F );
        SET_FV(R_ISpdTempA10,   0.0F );
        SET_FV(R_ISpdTempA11,   0.0F );
        SET_FV(R_ISpdTempA12,   0.0F );
        SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

        SET_FV(S_ISpdTempMt, -6.9540F );
        SET_FV(S_ISpdTempBt,  1725.0F );
        SET_FV(S_ISpdTempMn,  -0.250F );
        SET_FV(S_ISpdTempBn,  1817.0F );
        SET_FV(S_ISpdTempNm,  1440.0F );
        S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
        SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA3,  0.0F );
        SET_FV(S_ISpdTempA4,  0.0F );
        SET_FV(S_ISpdTempA5,  0.0F );
        SET_FV(S_ISpdTempA6,  0.0F );
        SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
        SET_FV(S_ISpdTempA8,  0.0F );
        SET_FV(S_ISpdTempA9,  0.0F );
        SET_FV(S_ISpdTempA10,   0.0F );
        SET_FV(S_ISpdTempA11,   0.0F );
        SET_FV(S_ISpdTempA12,   0.0F );
        SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
        SET_FV(S_SatCurvFlxMax, 1705.28F);
        SET_FV(S_SatCurvFlxMin, 299.768F);
        SET_FV(S_SatCurvXmMin, 0.00123199F);
        SET_FV(S_SatCurvXmMax, 0.00302873F);
        SET_FV(S_SatCurvK0,     0.00352708F);
        SET_FV(S_SatCurvK1,    -1.54983e-6F);
        SET_FV(S_SatCurvK2,     -4.81301e-10F);
        SET_FV(S_SatCurvK3,    3.52379e-13F);
        SET_FV(S_GenSatCurvVld, false);

        SET_FV(GenILimTempMax, 70.0F);
        SET_FV(GenILimTempMin, -20.0F);

        SET_FV(R_ImaxCrvGenMax, 10000.0F);
        SET_FV(S_ImaxCrvGenMax, 10000.0F);
    }
    else if(PARM(GenTypeCfg) == GENTYPE_OTHER)
    {
        if( PARM(GenTypeNewCfg)<1 || PARM(GenTypeNewCfg)>20 || !PARM(GenTreFile))
        {
            PUSH_DIAG(NoGenFileTrp);
            SET_FV(S_Volts,        690.0F       );
            SET_FV(S_Amps,         1210.0F      );
            SET_FV(S_Hz,           60.0F        );
            SET_FV(S_Power,        1520e3F      );
            SET_FV(R_TachPPR,      3072         );
            SET_FV(S_PwrFactor,    0.9F         );
            SET_FV(S_RRatio,       0.3540F      );
            SET_FV(S_X1,           0.038F       );
            SET_FV(S_X2,           0.056F       );
            SET_FV(S_Xm,           0.920F       );
            SET_FV(S_XST,          0.90 * (PARM(S_X1) + PARM(S_X2))   );
            SET_FV(S_R1,           0.00217      );
            SET_FV(S_R2,           0.002        );
            SET_FV(R_RPMMinOp,     800.0F       );
            SET_FV(R_RPMSync,      1200.0F      );
            SET_FV(R_RPMNom,       1440.0F      );
            SET_FV(R_RPMMaxOp,     1600.0F      );
            SET_FV(R_GenInertia,   99.99F       );
            SET_FV(R_ISpdTempMt,   -3.16F );
            SET_FV(R_ISpdTempBt,   784.6F );
            SET_FV(R_ISpdTempMn, -0.1580F );
            SET_FV(R_ISpdTempBn,  889.30F );
            SET_FV(R_ISpdTempNm,  1440.0F );
            SET_FV(R_ISpdTempMa,-0.000167F);   // slope
            SET_FV(R_ISpdTempBa,    1.167F);   // offset
            R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
            SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
            SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
            SET_FV(R_ISpdTempA3,  0.0F );
            SET_FV(R_ISpdTempA4,  0.0F );
            SET_FV(R_ISpdTempA5,  0.0F );
            SET_FV(R_ISpdTempA6,  0.0F );
            SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
            SET_FV(R_ISpdTempA8,  0.0F );
            SET_FV(R_ISpdTempA9,  0.0F );
            SET_FV(R_ISpdTempA10,   0.0F );
            SET_FV(R_ISpdTempA11,   0.0F );
            SET_FV(R_ISpdTempA12,   0.0F );
            SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

            SET_FV(S_ISpdTempMt, -6.9540F );
            SET_FV(S_ISpdTempBt,  1725.0F );
            SET_FV(S_ISpdTempMn,  -0.250F );
            SET_FV(S_ISpdTempBn,  1817.0F );
            SET_FV(S_ISpdTempNm,  1440.0F );
            S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
            SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
            SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
            SET_FV(S_ISpdTempA3,  0.0F );
            SET_FV(S_ISpdTempA4,  0.0F );
            SET_FV(S_ISpdTempA5,  0.0F );
            SET_FV(S_ISpdTempA6,  0.0F );
            SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
            SET_FV(S_ISpdTempA8,  0.0F );
            SET_FV(S_ISpdTempA9,  0.0F );
            SET_FV(S_ISpdTempA10,   0.0F );
            SET_FV(S_ISpdTempA11,   0.0F );
            SET_FV(S_ISpdTempA12,   0.0F );
            SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

            SET_FV(S_SatCurvFlxMax, 1705.28F);
            SET_FV(S_SatCurvFlxMin, 299.768F);
            SET_FV(S_SatCurvXmMin, 0.00123199F);
            SET_FV(S_SatCurvXmMax, 0.00302873F);
            SET_FV(S_SatCurvK0,     0.00352708F);
            SET_FV(S_SatCurvK1,    -1.54983e-6F);
            SET_FV(S_SatCurvK2,     -4.81301e-10F);
            SET_FV(S_SatCurvK3,    3.52379e-13F);
            SET_FV(S_GenSatCurvVld, false);

            SET_FV(GenILimTempMax, 70.0F);
            SET_FV(GenILimTempMin, -20.0F);

            SET_FV(R_ImaxCrvGenMax, 10000.0F);
            SET_FV(S_ImaxCrvGenMax, 10000.0F);
        }
        else
        {
            SET_FV(S_Volts,        PARM(S_VoltsNw)     );
            SET_FV(S_Amps,         PARM(S_AmpsNw)      );
            SET_FV(S_Hz,           PARM(S_HzNw)        );
            SET_FV(S_Power,        PARM(S_PowerNw)     );
            SET_FV(R_TachPPR,      PARM(R_TachPPRNw)   );
            SET_FV(S_PwrFactor,    PARM(S_PwrFactorNw) );
            SET_FV(S_RRatio,       PARM(S_RRatioNw)    );
            SET_FV(S_X1,           PARM(S_X1Nw)        );
            SET_FV(S_X2,           PARM(S_X2Nw)        );
            SET_FV(S_Xm,           PARM(S_XmNw)        );
            SET_FV(S_XST,          PARM(S_XSTNw)       );
            SET_FV(S_R1,           PARM(S_R1Nw)        );
            SET_FV(S_R2,           PARM(S_R2Nw)        );
            SET_FV(R_RPMMinOp,     PARM(R_RPMMinOpNw)  );
            SET_FV(R_RPMSync,      PARM(R_RPMSyncNw)   );
            SET_FV(R_RPMNom,       PARM(R_RPMNomNw)    );
            SET_FV(R_RPMMaxOp,     PARM(R_RPMMaxOpNw)  );
            SET_FV(R_GenInertia,   PARM(R_GenInertiaNw) );
            SET_FV(R_ISpdTempMt,   PARM(R_ISpdTpMtNw)  );
            SET_FV(R_ISpdTempBt,   PARM(R_ISpdTpBtNw)  );
            SET_FV(R_ISpdTempMn,   PARM(R_ISpdTpMnNw)  );
            SET_FV(R_ISpdTempBn,   PARM(R_ISpdTpBnNw)  );
            SET_FV(R_ISpdTempNm,   PARM(R_ISpdTpNmNw)  );
            SET_FV(R_ISpdTempMa,   PARM(R_ISpdTpMaNw)  );
            SET_FV(R_ISpdTempBa,   PARM(R_ISpdTpBaNw)  );
            R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
            SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
            SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
            SET_FV(R_ISpdTempA3,  0.0F );
            SET_FV(R_ISpdTempA4,  0.0F );
            SET_FV(R_ISpdTempA5,  0.0F );
            SET_FV(R_ISpdTempA6,  0.0F );
            SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
            SET_FV(R_ISpdTempA8,  0.0F );
            SET_FV(R_ISpdTempA9,  0.0F );
            SET_FV(R_ISpdTempA10,   0.0F );
            SET_FV(R_ISpdTempA11,   0.0F );
            SET_FV(R_ISpdTempA12,   0.0F );
            SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

            SET_FV(S_ISpdTempMt,   PARM(S_ISpdTpMtNw)  );
            SET_FV(S_ISpdTempBt,   PARM(S_ISpdTpBtNw)  );
            SET_FV(S_ISpdTempMn,   PARM(S_ISpdTpMnNw)  );
            SET_FV(S_ISpdTempBn,   PARM(S_ISpdTpBnNw)  );
            SET_FV(S_ISpdTempNm,   PARM(S_ISpdTpNmNw)  );
            S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
            SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
            SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
            SET_FV(S_ISpdTempA3,  0.0F );
            SET_FV(S_ISpdTempA4,  0.0F );
            SET_FV(S_ISpdTempA5,  0.0F );
            SET_FV(S_ISpdTempA6,  0.0F );
            SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
            SET_FV(S_ISpdTempA8,  0.0F );
            SET_FV(S_ISpdTempA9,  0.0F );
            SET_FV(S_ISpdTempA10,   0.0F );
            SET_FV(S_ISpdTempA11,   0.0F );
            SET_FV(S_ISpdTempA12,   0.0F );
            SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

            SET_FV(S_SatCurvFlxMax, 1705.28F);
            SET_FV(S_SatCurvFlxMin, 299.768F);
            SET_FV(S_SatCurvXmMin, 0.00123199F);
            SET_FV(S_SatCurvXmMax, 0.00302873F);
            SET_FV(S_SatCurvK0,     0.00352708F);
            SET_FV(S_SatCurvK1,    -1.54983e-6F);
            SET_FV(S_SatCurvK2,     -4.81301e-10F);
            SET_FV(S_SatCurvK3,    3.52379e-13F);
            SET_FV(S_GenSatCurvVld, false);

            SET_FV(GenILimTempMax, 70.0F);
            SET_FV(GenILimTempMin, -20.0F);

            SET_FV(R_ImaxCrvGenMax, 10000.0F);
            SET_FV(S_ImaxCrvGenMax, 10000.0F);

        }

    }
    else if(PARM(GenTypeCfg) == GENTYPE_UNSPECIFIED)
    {
            SET_FV(S_Volts,        690.0F       );
            SET_FV(S_Amps,         1210.0F      );
            SET_FV(S_Hz,           60.0F        );
            SET_FV(S_Power,        0.0F         );
            SET_FV(R_TachPPR,      3072         );
            SET_FV(S_PwrFactor,    0.0F         );
            SET_FV(S_RRatio,       0.3540F      );
            SET_FV(S_X1,           0.038F       );
            SET_FV(S_X2,           0.056F       );
            SET_FV(S_Xm,           0.920F       );
            SET_FV(S_XST,          0.90 * (PARM(S_X1) + PARM(S_X2))   );
            SET_FV(S_R1,           0.00217      );
            SET_FV(S_R2,           0.002        );
            SET_FV(R_RPMMinOp,     800.0F       );
            SET_FV(R_RPMSync,      1200.0F      );
            SET_FV(R_RPMNom,       1440.0F      );
            SET_FV(R_RPMMaxOp,     1600.0F      );
            SET_FV(R_GenInertia,   99.99F       );
            SET_FV(R_ISpdTempMt,   -3.16F );
            SET_FV(R_ISpdTempBt,   784.6F );
            SET_FV(R_ISpdTempMn, -0.1580F );
            SET_FV(R_ISpdTempBn,  889.30F );
            SET_FV(R_ISpdTempNm,  1440.0F );
            SET_FV(R_ISpdTempMa,-0.000167F);   // slope
            SET_FV(R_ISpdTempBa,    1.167F);   // offset
            R_ISpdTmpRcp   = 1.0F/( (PARM(R_ISpdTempMn)*PARM(R_ISpdTempNm)) + PARM(R_ISpdTempBn));
            SET_FV(R_ISpdTempA1,  PARM(R_ISpdTempBt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
            SET_FV(R_ISpdTempA2,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );
            SET_FV(R_ISpdTempA3,  0.0F );
            SET_FV(R_ISpdTempA4,  0.0F );
            SET_FV(R_ISpdTempA5,  0.0F );
            SET_FV(R_ISpdTempA6,  0.0F );
            SET_FV(R_ISpdTempA7,  PARM(R_ISpdTempMt)*PARM(R_ISpdTempMn) * R_ISpdTmpRcp );
            SET_FV(R_ISpdTempA8,  0.0F );
            SET_FV(R_ISpdTempA9,  0.0F );
            SET_FV(R_ISpdTempA10,   0.0F );
            SET_FV(R_ISpdTempA11,   0.0F );
            SET_FV(R_ISpdTempA12,   0.0F );
            SET_FV(R_ISpdTempA13,   PARM(R_ISpdTempBt)*PARM(R_ISpdTempBn) * R_ISpdTmpRcp );

            SET_FV(S_ISpdTempMt, -6.9540F );
            SET_FV(S_ISpdTempBt,  1725.0F );
            SET_FV(S_ISpdTempMn,  -0.250F );
            SET_FV(S_ISpdTempBn,  1817.0F );
            SET_FV(S_ISpdTempNm,  1440.0F );
            S_ISpdTmpRcp   = 1.0F/( (PARM(S_ISpdTempMn)*PARM(S_ISpdTempNm)) + PARM(S_ISpdTempBn));
            SET_FV(S_ISpdTempA1,  PARM(S_ISpdTempBt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
            SET_FV(S_ISpdTempA2,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );
            SET_FV(S_ISpdTempA3,  0.0F );
            SET_FV(S_ISpdTempA4,  0.0F );
            SET_FV(S_ISpdTempA5,  0.0F );
            SET_FV(S_ISpdTempA6,  0.0F );
            SET_FV(S_ISpdTempA7,  PARM(S_ISpdTempMt)*PARM(S_ISpdTempMn) * S_ISpdTmpRcp );
            SET_FV(S_ISpdTempA8,  0.0F );
            SET_FV(S_ISpdTempA9,  0.0F );
            SET_FV(S_ISpdTempA10,   0.0F );
            SET_FV(S_ISpdTempA11,   0.0F );
            SET_FV(S_ISpdTempA12,   0.0F );
            SET_FV(S_ISpdTempA13,   PARM(S_ISpdTempBt)*PARM(S_ISpdTempBn) * S_ISpdTmpRcp );

            SET_FV(S_SatCurvFlxMax, 1705.28F);
            SET_FV(S_SatCurvFlxMin, 299.768F);
            SET_FV(S_SatCurvXmMin, 0.00123199F);
            SET_FV(S_SatCurvXmMax, 0.00302873F);
            SET_FV(S_SatCurvK0,     0.00352708F);
            SET_FV(S_SatCurvK1,    -1.54983e-6F);
            SET_FV(S_SatCurvK2,     -4.81301e-10F);
            SET_FV(S_SatCurvK3,    3.52379e-13F);
            SET_FV(S_GenSatCurvVld, false);

            SET_FV(GenILimTempMax, 70.0F);
            SET_FV(GenILimTempMin, -20.0F);

            SET_FV(R_ImaxCrvGenMax, 10000.0F);
            SET_FV(S_ImaxCrvGenMax, 10000.0F);
    }

	SET_FV(R_ISpdTempNomT,        25.0F); // Nominal temperature as input for generator current limit calculation, 25.0degc;

/* //comment out after using new generator current limit curve, 2016-1-27, SZ
    // For later rules that run:
    R_IMaxCrvRule   = ( (PARM(R_ISpdTempMt) * PARM(WBTemp)) + PARM(R_ISpdTempBt))  *
                      ( ( (PARM(R_ISpdTempMn) * PARM(WBSpeed)) + PARM(R_ISpdTempBn)) * R_ISpdTmpRcp) ;

    S_IMaxCrvRule   = ( (PARM(S_ISpdTempMt) * PARM(WBTemp)) + PARM(S_ISpdTempBt))  *
                      ( ( (PARM(S_ISpdTempMn) * PARM(WBSpeed)) + PARM(S_ISpdTempBn)) * S_ISpdTmpRcp) ;
*/


////// Code for Cable Type Selection
    if (PARM(CableConfig) == CABTYP_C50HZ_LEGACY) //Insert new cases here when new generators are added.
    {
        SET_FV(R_ILimA1Cab1,  757.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  554.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   10.0F);
        SET_FV(R_TempCable2,   50.0F);

        SET_FV(S_ILimA1Cab1, 1400.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1400.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   10.0F);
        SET_FV(S_TempCable2,   50.0F);
    }
    else if (PARM(CableConfig) == CABTYP_C50Hz_BDL_95)
    {
        SET_FV(R_ILimA1Cab1,  667.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  472.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   10.0F);
        SET_FV(R_TempCable2,   50.0F);

        SET_FV(S_ILimA1Cab1, 1616.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1143.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   10.0F);
        SET_FV(S_TempCable2,   50.0F);
    }
    else if (PARM(CableConfig) == CABTYP_C50Hz_SPC_24)
    {
        SET_FV(R_ILimA1Cab1,  808.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  576.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   10.0F);
        SET_FV(R_TempCable2,   50.0F);

        SET_FV(S_ILimA1Cab1, 2010.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1433.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   10.0F);
        SET_FV(S_TempCable2,   50.0F);
    }
    else if (PARM(CableConfig) == CABTYP_C50_5X240_2X240)  //Tempoary values and name for a 2.3 system
    {
        SET_FV(R_ILimA1Cab1, 1103.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  786.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   10.0F);
        SET_FV(R_TempCable2,   50.0F);

        SET_FV(S_ILimA1Cab1, 2757.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1966.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   10.0F);
        SET_FV(S_TempCable2,   50.0F);
    }
    else if (PARM(CableConfig) == CABTYP_C60HZ_LEGACY)
    {
        SET_FV(R_ILimA1Cab1,  757.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  554.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   10.0F);
        SET_FV(R_TempCable2,   50.0F);

        SET_FV(S_ILimA1Cab1, 1400.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1400.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   10.0F);
        SET_FV(S_TempCable2,   50.0F);
    }
    else if (PARM(CableConfig) == CABTYP_C60Hz_BDL_44)
    {
        SET_FV(R_ILimA1Cab1,  671.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  475.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   10.0F);
        SET_FV(R_TempCable2,   50.0F);

        SET_FV(S_ILimA1Cab1, 1619.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1145.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   10.0F);
        SET_FV(S_TempCable2,   50.0F);
    }
    else if (PARM(CableConfig) == CABTYP_C60Hz_SPC_44)
    {
        SET_FV(R_ILimA1Cab1,  762.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  544.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   10.0F);
        SET_FV(R_TempCable2,   50.0F);

        SET_FV(S_ILimA1Cab1, 1803.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1286.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   10.0F);
        SET_FV(S_TempCable2,   50.0F);
    }
    else if (PARM(CableConfig) == CABTYP_C60Hz_13522_5_535_2_535)
    {
        SET_FV(R_ILimA1Cab1, 1108.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  790.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   10.0F);
        SET_FV(R_TempCable2,   50.0F);

        SET_FV(S_ILimA1Cab1, 2737.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1952.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   10.0F);
        SET_FV(S_TempCable2,   50.0F);
    }
    else if (PARM(CableConfig) == CABTYP_C60Hz_13522_4_646_2_444)
    {
        SET_FV(R_ILimA1Cab1, 986.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  703.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   10.0F);
        SET_FV(R_TempCable2,   50.0F);

        SET_FV(S_ILimA1Cab1, 2501.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1783.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   10.0F);
        SET_FV(S_TempCable2,   50.0F);
    }
    else if (PARM(CableConfig) == CABTYP_C60Hz_13522_4_646_2_535)
    {
        SET_FV(R_ILimA1Cab1, 1114.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  795.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   10.0F);
        SET_FV(R_TempCable2,   50.0F);

        SET_FV(S_ILimA1Cab1, 2501.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1783.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   10.0F);
        SET_FV(S_TempCable2,   50.0F);
    }
    else if (PARM(CableConfig) == CABTYP_C50Hz_13522_4_300_2_240)
    {
        SET_FV(R_ILimA1Cab1, 1103.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  786.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   10.0F);
        SET_FV(R_TempCable2,   50.0F);

        SET_FV(S_ILimA1Cab1, 2555.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1822.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   10.0F);
        SET_FV(S_TempCable2,   50.0F);
    }

    else if (PARM(CableConfig) == CABTYP_OTHER)
    {
        if ( (PARM(CableLimTyp) < 1) || (PARM(CableLimTyp) > 20) || !PARM(CableTreFile) )
        {
            NoCabFileFlg = true;
            SET_FV(R_ILimA1Cab1,  641.0F);       //1k meters @ 10 deg C
            SET_FV(R_ILimA1Cab2,  555.0F);       //1k meters @ 50 deg C
            SET_FV(R_TempCable1,   31.0F);
            SET_FV(R_TempCable2,   46.0F);

            SET_FV(S_ILimA1Cab1, 1401.0F);       //1k meters @ 10 deg C
            SET_FV(S_ILimA1Cab2, 1258.0F);       //1k meters @ 50 deg C
            SET_FV(S_TempCable1,   35.0F);
            SET_FV(S_TempCable2,   46.0F);
        }
        else
        {
            SET_FV(R_ILimA1Cab1, PARM(R_ILimCab1Ov));
            SET_FV(R_ILimA1Cab2, PARM(R_ILimCab2Ov));
            SET_FV(R_TempCable1, PARM(R_TempCab1Ov));
            SET_FV(R_TempCable2, PARM(R_TempCab2Ov));
            SET_FV(S_ILimA1Cab1, PARM(S_ILimCab1Ov));
            SET_FV(S_ILimA1Cab2, PARM(S_ILimCab2Ov));
            SET_FV(S_TempCable1, PARM(S_TempCab1Ov));
            SET_FV(S_TempCable2, PARM(S_TempCab2Ov));
        }
    }
    else // (PARM(CableConfig) == CABTYP_UNSPECIFIED)
    {
        SET_FV(R_ILimA1Cab1,  640.0F);       //1k meters @ 10 deg C
        SET_FV(R_ILimA1Cab2,  540.0F);       //1k meters @ 50 deg C
        SET_FV(R_TempCable1,   31.0F);
        SET_FV(R_TempCable2,   46.0F);

        SET_FV(S_ILimA1Cab1, 1400.0F);       //1k meters @ 10 deg C
        SET_FV(S_ILimA1Cab2, 1400.0F);       //1k meters @ 50 deg C
        SET_FV(S_TempCable1,   35.0F);
        SET_FV(S_TempCable2,   46.0F);
    }

    if ( PARM(TestMode) == PUMP_BACK )
    {
        SET_FV(R_ILimA1Cab1, 2500.0F);
    }
    CabCfgType = PARM(CableConfig);

    //fix this later to allow imports
    if (PARM(CableConfig) == CABTYP_OTHER)
    {
        CableLimType  = PARM(CableLimTyp);
    }
    else if (PARM(CableConfig) == CABTYP_UNSPECIFIED)
    {
        CbRqPrmBdFlg = true;
        CableLimType = CABTYP_OTHER;
    }
    else
    {
        CableLimType  = CABTYP_UNSP;//Standard
        if ( ( (PARM(L_FrqInput) == 60.0F) && (PARM(CableConfig)   < 51.0F) ) ||
        ( (PARM(L_FrqInput) == 50.0F) && ( (PARM(CableConfig) >= 51.0F) ) )
           )
        {
            CbFrqMsMcFlg = true;
        }
    }



////// End code for Cable Type Selection

//------------------------------------------------------------------------------
// Turbine system application data

    TurbNpRatRcp = NoNANLim(1.0F/(1000.0F * PARM(TurbineNpRat)), 1.0e6F, -1.0e6F);
    TurbNpRatWtt = 1000.0F*PARM(TurbineNpRat);
    TurbNpRatWRcp = 1.0F / TurbNpRatWtt ;

    SET_FV(MaxTrbPwrIPN, 2550.0F);       // kW
    SET_FV(MinTrbPwrIPN, 1500.0F);       // kW
    SET_FV(SysVoltNom, 690.0F);


    if ( PARM(TestMode) == PUMP_BACK )
    {
        SET_FV(TrqTrimIntGn,  0.0F);     // r/s
        SET_FV(TrqTrimLim,    0.0F);     // watts
        SET_FV(AuxPwrNom,     0.0F);
        SET_FV(PwrTrimFfDyn, false);
    }
    else
    {
        SET_FV(TrqTrimIntGn,  1.0F);      // r/s
        SET_FV(TrqTrimLim,  150.0e3F); // watts
        SET_FV(AuxPwrNom, 53.0e3F);    //50 for hotel load + 3.0kw for pump and fan
        SET_FV(PwrTrimFfDyn, true);
    }



    if (PARM(TurbineNpRat) < 1720.0F)
    {
        // 1.85 and below
        if (PARM(L_FrqInput) == 50.0F)
        {
            SET_FV(TurbineOvrldSpd, 1785.0F);  // (rpm) - min speed at which overload power is required, from DM2
        }
        else
        {
            SET_FV(TurbineOvrldSpd, 1428.0F);
        }
        SET_FV(TurbineOvrldPwr , 2175.0F + 0.001 * PARM(TrqTrimLim) );

    }
    else if (PARM(TurbineNpRat) < 1880.0F)
    {
        // 1.85 and below
        if (PARM(L_FrqInput) == 50.0F)
        {
            SET_FV(TurbineOvrldSpd, 1785.0F);  // (rpm) - min speed at which overload power is required, from DM2
        }
        else
        {
            SET_FV(TurbineOvrldSpd, 1428.0F);
        }
        SET_FV(TurbineOvrldPwr , 2175.0F + 0.001 * PARM(TrqTrimLim) );
    }

    else
    {
        // 2.0 and up
        if (PARM(L_FrqInput) == 50.0F)
        {
            SET_FV(TurbineOvrldSpd, 1773.0F);
        }
        else
        {
            SET_FV(TurbineOvrldSpd, 1498.0F);
        }
        SET_FV(TurbineOvrldPwr , 2791.0F + 0.001 * PARM(TrqTrimLim) );
    }


    TurbineOvrldPwrPu = NoNANLim(PARM(TurbineOvrldPwr)/PARM(TurbineNpRat), 1.0e6F, -1.0e6F);

    TurbineOvrldTrq   = NoNANLim(PARM(TurbineOvrldPwr)*1000.0F/(PARM(TurbineOvrldSpd) * RPM_TO_RAD), 1.0e6F, -1.0e6F);




    SysVoltNomRcp = NoNANLim(1.0F/PARM(SysVoltNom), 1.0e6F, -1.0e6F);

    SET_FV(SysVoltMax, 1.1F*PARM(SysVoltNom));
    SET_FV(SysVoltMin, 0.9F*PARM(SysVoltNom));

    SET_FV(SysInom,  PARM(SysVARat) * SysVoltNomRcp * SQRT3_RCP);
    SET_FV(SysINomGrid, PARM(TurbineNpRat) * 1.0e3F * SysVoltNomRcp * SQRT3_RCP);
    SET_FV(SysIRatLv, NoNANLim(PARM(SysVARat)/(PARM(SysVoltMin) * SQRT3), 1.0e6F, -1.0e6F) );
    SET_FV(SysIyRat,  NoNANLim(PARM(SysIRatLv) * sqrtf(1.0F -(PARM(SysPwrFact)*PARM(SysPwrFact))), 1.0e6F, -1.0e6F) );
    if( PARM(SoftwareIPN) >= DFIG02_2400_5060_A )
    {
        SET_FV(SysIOVLoad, TurbineOvrldPwrPu * 100.0F);           // Percentage x 100 of rated value
    }
    else
    {
        SET_FV(SysIOVLoad, 122.0F);
    }
    SET_FV(SysFreqNom, PARM(L_FrqInput));
    SET_FV(SysIratStp, PARM(SysInom));
    SET_FV(SysZBase, PARM(SysVoltNom) * PARM(SysVoltNom)/(PARM(TurbineNpRat) * 1.0e3) );
    SET_FV(SysLBase, NoNANLim(PARM(SysZBase)/(PARM(SysFreqNom)*PI2), 1.0e6F, -1.0e6F) );
    SET_FV(SysCBase, 1.0F/(PARM(SysZBase)*PARM(SysFreqNom)*PI2));

    INomGridPk  =  PARM(SysINomGrid) * SQRT2;
    SysPowerx   =  PARM(SysPower);
    SysPwrFactx =  PARM(SysPwrFact);
    SysVoltNomx =  PARM(SysVoltNom);
    INomx       =  PARM(SysInom);
    SysVoltMaxx =  PARM(SysVoltMax);
    SysVoltMinx =  PARM(SysVoltMin);
    SysVARatx   =  PARM(SysVARat);
    SysIRatx    =  PARM(SysIratStp);
    SysIRatLvx  =  PARM(SysIRatLv);
    SysIyRatx   =  PARM(SysIyRat);
    SysIOVLoadx =  PARM(SysIOVLoad);
    SysFreqNomx =  PARM(SysFreqNom);
    SysWnom     =  2.0F * PI * PARM(SysFreqNom);

//------------------------------------------------------------------------------
// Grid features

    if ( PARM(GridFeatDef) )
    {
        SET_PARM(SeriesCmpEnb , false);
        SET_PARM(MaxRealCurr  , false);
        SET_PARM(LvPwrCrvOvr  , false);
        SET_PARM(LvPwrRcvyOvr , false);
        SET_PARM(GridStrength , WEAK);          // ESS (defer for MMW)
        SET_PARM(QregFFEnb    , false);
        SET_PARM(VRTSupptTyp  , VRTCONV);
        SET_PARM(VPRPVrefFix  , true);
        SET_PARM(VPRPIyRstAvg , false);
        SET_PARM(LvrtIyOvr    , false);
        SET_PARM(HvrtIyOvr    , false);
        SET_PARM(LvPwrLmRcyP0 , 0.70F);

        //Secondary parameters
        SET_PARM(PhsJmpTrpEnb , false);
        SET_PARM(LvrtIyMaxOvr , 1.05F);
        SET_PARM(HvrtIyMaxOvr , 1.20F);
        SET_PARM(LvPwrP2PwrOv , 0.75F);
        SET_PARM(LvPwrP2VOv   , 0.75F);
        SET_PARM(LvPwrP3PwrOv , 0.00F);
        SET_PARM(LvPwrP3VOv   , 0.50F);
        SET_PARM(LvPwrRcvyRat , 5.0F);
        SET_PARM(LvIyKTmOvr   , false);
        SET_PARM(LvIyKTmMxOvr , 0.075);
        SET_PARM(LvIyKIOvr    , false);
        SET_PARM(LvIyKIMxOvr  , 0.25);
        SET_PARM(VPRPDBandHv  , 0.10F);
        SET_PARM(VPRPDBandLv  , 0.10F);
        SET_PARM(VPRPPrpGnHv  , 2.00F);
        SET_PARM(VPRPPrpGnLv  , 2.00F);
        SET_PARM(TorProtType  , TORQUE_BASED);           //Air gap torque
        SET_PARM(PhsJmpTrpEnb , false);
        SET_PARM(FastDisEnb   , false);
        SET_PARM(FastBlkEnb   , false);
        SET_PARM(VoltExtendOpt   , false);
    }

    if ((PARM(SoftwareIPN) <  DFIG02_2400_5060_A
    || PARM(GmCfgSelFV) != GM_WIZ_ZVRT_GEN3_BASE))
    {
        SET_PARM(VoltExtendOpt   , false);
    }

//------------------------------------------------------------------------------
// set final values for ESA buffer.
#ifndef DSIM_MODE  
    if (EsaBuf.RunRules( 
                         (void*)LADR_R_SpdFbkAvg,        // bin signal 1 select
                         (PARM(R_RPMMinOp) - 10.0F),     // bin signal 1, min range
                         (PARM(R_RPMNom)   + 10.0F),     // bin signal 1, max range

                         (void*)LADR_S_IMagT3Pu,         // bin signal 2 select
                         0.0F,                           // bin signal 2, min range
                         1.00F,                          // bin signal 2, max range
                          
                         (void*)LADR_R_SpdFbkAvg,        // tolerance variable 1 select
                         30.0F,                          // variable 1 tolerance (absolute)

                         (void*)LADR_S_IMag,             // tolerance variable 2 select
                         0.12F,                          // variable 2 tolerance (pu of average)
                         0.30F                           // averager filter omega (r/s)
                       ) == FAIL)
    {
        Status = FAIL;
    }	
#endif	
	
	
//------------------------------------------------------------------------------
// Converter Hardware


    SET_FV(BrgVdcNom, 1.10F*SQRT2*PARM(SysVoltNom));  // Nominal dc link voltage



    if (PARM(SoftwareIPN) >= DFIG02_2400_5060_A)    //  DFIG02-2300-5060-A )
    {
        SET_FV(CnvLineL,             150.0e-6F); // line converter inductance, reactor+bridge (Henries)
        SET_FV(CnvLineR,             2.300e-3F); // line converter resistance, reactor+bridge (Ohms) - original value
        SET_FV(CnvLineDfC,             480e-6F); // wye equivalent capacitance (Farads)
        SET_FV(CnvLineDfR,              0.027F); // dist-filter resistor in series with DFC1 (Ohms)
        SET_FV(CnvRotorL,             25.0e-6F); // rotor converter inductance, reactor+bridge (Henries)
        SET_FV(CnvRotorR,             2.30e-3F); // same original value used in line brg (Ohms)
        SET_FV(BrgCdclink,              30e-3F); // DC link capacitance
        SET_FV(BrgDbR,                   0.50F); // resistance for one DB circuit (Ohms)
        SET_FV(BrgDbL,                25.00e-6); // inductance for one DB circuit (Henry) to be measured in lab TBD!!!
        SET_FV(BrgDbPwrCont,          5.513e3F); // continuous power rating for one DB resistor augmented by fan-cooling factor (Watts)
        SET_FV(BrgDbPwrImp,             1570e3); // impulse power rating for one DB resistor - unaffected by fan (Watts)
        SET_FV(BrgDbImpTm,                1.0F); // impulse time duration (Sec)
        SET_FV(BrgDbRthrm,             0.0764F); // DB resistor thermal resistance, with fan running (value from SWS.)
        SET_FV(InletToDbRise,            10.0F); // Air temp rise from inlet to DB resistors (deg C) (value from SWS.)
        SET_FV(L_BrgDbEnb,                true); // DB enable
        SET_FV(DbVfbkTau,             34.98e-6); //35usec tau on Vdb feedback circuit -- R=10k+5.9k, C=0.0022uf

    }
    else if ( (PARM(SoftwareIPN) == DFIG02_1500_5060_B) || (PARM(SoftwareIPN) == DFIG02_1850_5060_C) || (PARM(SoftwareIPN) == DFIG02_1850_5060_D) || (PARM(SoftwareIPN) == DFIG02_1850_5060_E))
    {
        SET_FV(CnvLineL,             150.0e-6F); // line converter inductance, reactor+bridge (Henries)
        SET_FV(CnvLineR,             2.420e-3F); // line converter resistance, reactor+bridge (Ohms) - original value
        SET_FV(CnvLineDfC,             480e-6F); // wye equivalent capacitance (Farads)
        SET_FV(CnvLineDfR,              0.027F); // dist-filter resistor in series with DFC1 (Ohms)
        SET_FV(CnvRotorL,             25.0e-6F); // rotor converter inductance, reactor+bridge (Henries)
        SET_FV(CnvRotorR,             2.42e-3F); // same original value used in line brg (Ohms)
        SET_FV(BrgCdclink,              24e-3F); // DC link capacitance
        SET_FV(BrgDbR,                   0.85F); // resistance for DB circuit (Ohms)
        SET_FV(BrgDbL,                67.32e-6); // inductance for DB circuit (Henry) measured in lab -- matches MACC tau setting of 79.2usec(R=0.85)
        SET_FV(BrgDbPwrCont,           5856.0F); // continuous power rating for one DB resistor
        SET_FV(BrgDbPwrImp,            1510e3F); // impulse power rating for one DB resistor - unaffected by fan (Watts)
        SET_FV(BrgDbImpTm,                1.0F); // impulse time duration (Sec)
        SET_FV(BrgDbRthrm,             0.0764F); // DB resistor thermal resistance, with fan running (value from SWS.)
        SET_FV(InletToDbRise,            10.0F); // Air temp rise from inlet to DB resistors (deg C) (value from SWS.)
        SET_FV(L_BrgDbEnb,                true); // DB enable
        SET_FV(DbVfbkTau,             34.98e-6); // 35usec tau on Vdb feedback circuit -- R=10k+5.9k, C=0.0022uf
    }
    else // ( (PARM(SoftwareIPN) == DFIG02_1500_5060_A)  )
    {
        SET_FV(CnvLineL,             150.0e-6F); // line converter inductance, reactor+bridge (Henries)
        SET_FV(CnvLineR,             2.420e-3F); // line converter resistance, reactor+bridge (Ohms) - original value
        SET_FV(CnvLineDfC,             480e-6F); // wye equivalent capacitance (Farads)
        SET_FV(CnvLineDfR,              0.027F); // dist-filter resistor in series with DFC1 (Ohms)
        SET_FV(CnvRotorL,             25.0e-6F); // rotor converter inductance, reactor+bridge (Henries)
        SET_FV(CnvRotorR,             2.42e-3F); // same original value used in line brg (Ohms)
        SET_FV(BrgCdclink,              24e-3F); // DC link capacitance
        SET_FV(BrgDbR,                   0.85F); // resistance for one DB circuit (Ohms)
        SET_FV(BrgDbL,                67.32e-6); // inductance for DB circuit (Henry) measured in lab -- matches MACC tau setting of 79.2usec(R=0.85)
        SET_FV(BrgDbPwrCont,           5856.0F); // continuous power rating for one DB resistor
        SET_FV(BrgDbPwrImp,            1510e3F); // impulse power rating for one DB resistor - unaffected by fan (Watts)
        SET_FV(BrgDbImpTm,                1.0F); // impulse time duration (Sec)
        SET_FV(BrgDbRthrm,             0.0764F); // DB resistor thermal resistance, with fan running (value from SWS.)
        SET_FV(InletToDbRise,            10.0F); // Air temp rise from inlet to DB resistors (deg C) (value from SWS.)
        SET_FV(L_BrgDbEnb,               false); // DB enable
        SET_FV(DbVfbkTau,             34.98e-6); // 35usec tau on Vdb feedback circuit -- R=10k+5.9k, C=0.0022uf
    }

    SET_FV(DbOtThrsh, PARM(BrgDbPwrCont) * 1.15F * 0.985F);  // this results on 13.3% max overload per previous DFIG



    SET_FV(CnvRotorAltLpu, 1.0F);       // 3 phases in parallel if ROV's closed & only get
    // common mode L during crowbar contactor verification

    if ( PARM(L_FrqInput) == 60) //60hz
    {
        SET_FV(R_RPMMaxStart, 1128.0);
    }
    else //50Hz
    {
        SET_FV(R_RPMMaxStart, 1395.0);
    }

//------------------------------------------------------------------------------
// Rules for the fans
    SET_FV(LqdTempFanMax       , 42.0F );    // requirement rev13(deg C)
    SET_FV(LqdTempFanMin       , 30.0F );    // requirement rev13(deg C)
    SET_FV(LqdTempFanRiseSetp  , 10.0F );    // requirement rev13 (deg C)


    SET_FV(PdcTempFanMax       ,  41.0F);    // requirement rev13(deg C)
    SET_FV(PdcTempFanMin       ,  30.0F);    // requirement rev13 (deg C)
    SET_FV(PdcTempFanRiseSetp  ,  10.0F );   // (deg C)
    SET_FV(PdcTempFanSpdGn     ,  0.1667F);  // requirement rev13, 1/6 (pu/deg C)


    SET_FV(LqdTempOkLvl        , -10.0F);
    SET_FV(LqdTempLowLvl       , -25.0F);



////////////////////////////////////////////////////////
// Current and votlage feedback scalings and offsets TO BE MOVED OR DELETED
////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Line Bridge

    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
    case DFIG02_1850_5060_E:
        SET_FV(L_ShuntRat, 400.0F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(L_ShuntRat, 800.0F);
        break;
    default:
        SET_FV(L_ShuntRat, 400.0F);
        break;
    }

// End of Line Bridge
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Rotor Bridge

    switch(PARM(SoftwareIPN))
    {
    case DFIG02_1500_5060_A:
    case DFIG02_1500_5060_B:
    case DFIG02_1850_5060_C:
    case DFIG02_1850_5060_D:
    case DFIG02_1850_5060_E:
        SET_FV(R_IRat,   640.0F);
        break;
    case DFIG02_2400_5060_A:
        SET_FV(R_IRat,  750.0F);
        break;
    default:
        SET_FV(R_IRat,   640.0F);
        break;
    }


    /* Final Values based on Frame Size */

    SET_FV(R_ShuntRat     ,   800.0F);
    SET_FV(R_PhDShuntRat  ,     0.0F);


    /* Final Values based on Voltage */
    SET_FV(R_VllMax       ,   690.0F);
    SET_FV(R_VllOut       ,   690.0F);

    /* Final Values based on Frame Size and Voltage */



    ///////////////////////
    //Currents
    ///////////////////////

    SET_FV(R_ShuntVolts, 0.050);
    SET_FV(R_IaShuntScl, PARM(R_ShuntRat) * (1.0/PARM(R_ShuntVolts)));
    SET_FV(R_IbShuntScl, PARM(R_ShuntRat) * (1.0/PARM(R_ShuntVolts)));
    SET_FV(R_IcShuntScl, PARM(R_ShuntRat) * (1.0/PARM(R_ShuntVolts)));

    SET_FV(R_IaVcoScl  ,344.92e-9);
    SET_FV(R_IbVcoScl  ,344.92e-9);
    SET_FV(R_IcVcoScl  ,344.92e-9);

    ///////////////////////
    //Voltages
    ///////////////////////
    SET_FV(R_VabVcoScl,1.726014e-3);
    SET_FV(R_VbcVcoScl,1.726014e-3);
    SET_FV(R_VdcVcoScl,746.8553459e-6);



// End of Rotor Bridge
///////////////////////////////////////////////////////////////////////////////



    // For DB

    // Product Diagnostics
    SET_FV(Q1UvHldOpnTst,       false);
}
