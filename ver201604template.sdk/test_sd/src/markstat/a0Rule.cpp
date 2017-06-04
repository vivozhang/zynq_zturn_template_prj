///////////////////////////////////////////////////////////////////////////////
//
// TITLE:      Generator control rules
//
// DESCRIPTION:
//       This file contains rule calculations for generator control
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     List Visual SourceSafe locations of pertinent design documents.
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
#include "a0Rule.h"
#include "p0Rule2.h"
#include "a0Spd.h"


// Constants
//----------
#define Mt_Dly_Tm             1.0   // sec

#define UNDER_MOD             0     // under modulation
#define NOMINAL_MOD           1     // Nominal modulation
#define OVER_MOD              2     // Over modulation

#define R2_R2C                0.7   // R2 actual divided by R2 command
#define L1_L1C                0.94  // L1 actual divided by L1 command
#define Del_R_PU              0.05  // per unit assumed R1 error

#define Tac_Read_Tm           10.2e-6
                              // Encoded raw signal read (DspHifiDin) seperation
                              // time between first (Smp1Encod) and second
                              // (Smp2Encod) read. This seperation time is code
                              // execution dependent. When R_InnrRegEnb, this
                              // time depends on the exec time of module
                              // DemodVoltage DemodCurrent MtrModelElec.
                              // When R_InnrRegEnb =False, this time will depend
                              // on DemodVoltage and first part of Precondition_T1.
                              // The execution time of these modules are predictable.
                              // It is timed on May 2000 to be around 153 count using
                              // timestamp function. The time is 153/15*e6 = 10.2usec

#define SQRT_1_2    1.095445115F//Sqrt(1.2)
#define SQRT_5DIV52 0.308866F   //Sqrt(5/52.41201) -- gain reduction for PllPrpGnI
#define SQRT2_3   (SQRT2/SQRT3)

#define ISNAN(a)    isNAN((unsigned *)&(a))


// Externals
//----------
extern void calc_2nd_order(filt2_in_type *Filt_In_Ptr, filt2_out_type *Filt_Out_Ptr);
extern float       R_ModLimFbkGn;
extern float       R_ModLimFbGnD;
extern float       R_ModLimFbGn2;
extern float       R_ModLmFbGn2D;
extern float       DelTm2;
extern float       DelTm2Rcp;
extern float       DelTm3;
extern float       DelTm3Rcp;
extern float       L_DelTm1Rcp;
extern float       L_VlnIn;
extern float       R_AmpsRat;
extern float       R_BusPwrGn;
extern float       R_CnvPwrGn;
extern float       R_DelTm1;
extern float       R_DelTm1Rcp;
extern float       R_FlxFilGn;
extern float       R_FrqFilGn;
extern float       R_HzRat;
extern float       R_IdcFilGn;
extern float       R_IMagFilGn;
extern float       R_IRatSSLimPk;
extern float       R_IregErr2Gn;
extern float       R_IxCmd0LimPk;
extern float       R_IxFilGn;
extern float       R_IyFilGn;
extern float       R_KPoles;
extern float       R_KPolesRcp;
extern float       R_LSigmaDtT2;
extern float       R_LsRcp;
extern float       R_PolePrRcp;
extern float       R_PosScl;
extern float       R_PowerRat;
extern float       R_QPwrFilGn;
extern float       R_RPMRat;
extern float       R_SpdFbkGn;
extern float       R_SpdGenGn;
extern float       R_SpdQnt;
extern float       R_SpdQntInv;
extern float       R_SpdRAFNRcp;
extern float       R_SpdRAFNTRcp;
extern float       R_SpdScl;
extern float       R_TachPosScl;
extern float       R_TrqFbkFGn;
extern float       R_TrqFilGn;
extern float       R_TrqTFilGn;
extern float       R_VdcFilGn;
extern float       R_VMagFilGn;
extern float       R_VoltsRat;
extern float       R_VRatRcp;
extern float       R_VxFilGn;
extern float       R_VyFilGn;
extern float       S_IMagFilGn;
extern float       S_IxFilGn;
extern float       S_IyFilGn;
extern float       S_LmNom;
extern float       S_PwrFilGn;
extern float       S_QPwrFilGn;
extern float       S_VMagFilGn;
extern float       S_VxFilGn;
extern float       S_VyFilGn;
extern float       S_X1;
extern float       S_PowerRcp;
extern float       R_CCvoltThrsSqr;
extern float       TurbNpRatWtt;
extern float       INomGridPk;
extern int         R_MrkrPlsAlm;
extern int         R_MrkrPlsFlt;
extern int         R_TachMax;
extern int         R_TacMaxHalf;

extern unsigned    R_SpdT2RAFN;
extern unsigned    R_SpdTcRAFN;
extern int         R_T1FramesPerT2;
extern unsigned*   R_TrqStepAdr;
extern void*       LogAddrTbl[];
extern float       HvIyRatPk;
extern float       R_VMgIxDcyGn;
extern float       R_SynIOCFilGn;
extern float       S_CTErrFilGn;
extern float       R_IyregPSlp;
extern float        R_SpdTcFFGn;

extern float       R_IxNchFilZA0;
extern float       R_IxNchFilZA1;
extern float       R_IxNchFilZA2;
extern float       R_IxNchFilZB1;
extern float       R_IxNchFilZB2;

extern float       R_IyNchFilZA0;
extern float       R_IyNchFilZA1;
extern float       R_IyNchFilZA2;
extern float       R_IyNchFilZB1;
extern float       R_IyNchFilZB2;

extern float       S_VrfSyncScl;
extern float       R_AngCorPIGn;
extern unsigned    SimAcclAct;
extern float       R_DcIregMaxSlope;
extern float       R_DcRegIntWdt;

// Types and Classes
//------------------


// Parameters
//-----------


// Published Variables
//--------------------
// stuff for S0line, rotor-only code     --> need to remove for combo code
CREATE_PUBVAR(R_VoltsRat,  float);           /* Motor nameplate Volts */
CREATE_PUBVAR(R_RPMRat,    float);           /* Motor nameplate RPM */
CREATE_PUBVAR(R_PowerRat,  float);           /* Motor nameplate Hp/KW */
CREATE_PUBVAR(R_KPoles,    float);
CREATE_PUBVAR(R_AmpsRat,   float);     /* Motor nameplate Amps */
CREATE_PUBVAR(S_VlnIn,     float);
CREATE_PUBVAR(S_VlnInRcp,  float);
CREATE_PUBVAR(R_DchgScl,   float);    // integrator discharge scaling */
CREATE_PUBVAR(S_R1,        float);    // R stator refered to stator
CREATE_PUBVAR(S_R2,        float);    // R rotor refered to stator
CREATE_PUBVAR(R_R1,        float);    // R stator refered to rotor
CREATE_PUBVAR(R_R2,        float);    // R rotor refered to rotor
CREATE_PUBVAR(R_Rs,        float);    // R stator refered to rotor  (includes cable)
CREATE_PUBVAR(R_Rr,        float);    // R rotor refered to rotor   (includes cable)
CREATE_PUBVAR(R_Ls,        float);    // L stator  refered to rotor (includes cable)
CREATE_PUBVAR(R_Lr,        float);    // L rotor  refered to rotor  (includes cable)
CREATE_PUBVAR(R_Lm,        float);    // Lm refered to rotor
CREATE_PUBVAR(S_Rs,        float);    // R stator refered to stator
CREATE_PUBVAR(S_Rr,        float);    // R rotor refered to stator
CREATE_PUBVAR(S_Ls,        float);    // L stator  refered to stator
CREATE_PUBVAR(S_Lr,        float);    // L rotor  refered to stator
CREATE_PUBVAR(S_Lm,        float);    // Lm refered to stator
CREATE_PUBVAR(S_X1,        float);
CREATE_PUBVAR(S_X2,        float);
CREATE_PUBVAR(S_Xm,        float);

CREATE_PUBVAR(S_WRat,      float);    // rated stator freq in rad/sec
CREATE_PUBVAR(R_IxRat,     float);    // Rated Torque Producing Current Ix (amps)
CREATE_PUBVAR(R_IyRat,     float);    // Rated Flux Producing Current Iy (amps)
CREATE_PUBVAR(R_VxRat,     float);    // Rated Vx
CREATE_PUBVAR(R_VyRat,     float);    // Rated Vy
CREATE_PUBVAR(R_FlxRat,    float);    // Rated Flux (volt-sec)
CREATE_PUBVAR(R_TrqRat,    float);    // Rated motor electromagnetic torque (N-m)
CREATE_PUBVAR(R_TrqRatRcp, float);    // Reciprocal of rated motor torque (1/N-m)
CREATE_PUBVAR(S_IxRat,     float);    // Rated real stator current
CREATE_PUBVAR(S_IyRat,     float);    // Rated reactive stator current
CREATE_PUBVAR(S_AmpsRat,   float);    // Rated stator current with unity PF and synch speed (Amps)
CREATE_PUBVAR(S_AmpsRatRcp,float);    // 
CREATE_PUBVAR(S_IMgK1StkGn,float);    // Filter gain for stator current when K1 is opened
CREATE_PUBVAR(R_RunRuleCtr,unsigned); // Run rule counter
CREATE_PUBVAR(S_PllWNom,   float);
CREATE_PUBVAR(S_RfeRcp,    float);
CREATE_PUBVAR(R_RfeRcp,    float);
CREATE_PUBVAR(R_ZBase,     float);    // Zbase for rotor
CREATE_PUBVAR(S_RRatioRcp, float);
CREATE_PUBVAR(R_RPMSyncRcp,float);

// Unpublished Variables
//----------------------
filt2_in_type  R_CurCmdFil2S;
filt2_out_type R_CurCmdFil2Z;


// Data Passing
//--------------


// Diagnostics
//------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------
int      RotorInResRule(void);
int      RotorMtrCtlRule(void);
void     RotorDefaultLineProt(void);
int      RotorMtrCtlSetup(void);
int      RotorCommissionSpecial(void);


// Class Instances
//----------------


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      MACC based setting of No-A-Number entity.
//
// EXECUTION LEVEL
//      When required by application code
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
int             isNAN(unsigned *up)
{
    return *up == 0x80800000;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//Rules for rotor load control layer.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
int RunRotorLCLRules(int Rule_Set)
{
    int     Status=SUCCESS;

    // In below statements, always AND function with status w/ function first:
    //      Status = FuncName() && Status;
    // NOT  Status = Status && FuncName();

    switch(Rule_Set)
    {
        // calculate motor control final values
        case RULE_SET_MAIN:
            Status = RotorInResRule() && Status;
            Status = RotorMtrCtlRule() && Status;
            break;

        // set motor control parameters based on final values
        case RULE_SET_DEFAULT_ALL:
            Status = RotorCommissionSpecial() && Status;
            Status = RotorMtrCtlSetup() && Status;
            Status = RotorInResRule() && Status;
            Status = RotorMtrCtlRule() && Status;
            RotorDefaultLineProt();
            break;

        // Run again on 2nd pass because of TorqueLimitSet & DefaultMiscellaneous dependency
        case (RULE_SET_DEFAULT_ALL + RULE_SET_SECOND_PASS):
            Status = RotorInResRule() && Status;
            Status = RotorMtrCtlRule() && Status;
            break;

        // Rule entry from drive comissioning wizard only
        case RULE_SET_DRIVE_COMMISSION_WIZ:
            Status = RotorCommissionSpecial() && Status;
            Status = RotorMtrCtlSetup() && Status;
            Status = RotorInResRule() && Status;
            Status = RotorMtrCtlRule() && Status;
            break;

        // Run again on 2nd pass because of DefaultMiscellaneous dependency
        case (RULE_SET_DRIVE_COMMISSION_WIZ + RULE_SET_SECOND_PASS):
            Status = RotorInResRule() && Status;
            Status = RotorMtrCtlRule() && Status;
            break;

        // Set Default Per Unit Settings
        case RULE_SET_DEFAULT_PER_UNIT:
            break;


        // Set motor control parameters
        case RULE_SET_MTR_CTL_SETUP:
            Status = RotorMtrCtlSetup() && Status;  // MtrCtlRule must follow
            Status = RotorInResRule() && Status;
            Status = RotorMtrCtlRule() && Status;
            break;

        // Run on 2nd pass because of DefaultMiscellaneous dependency
        case (RULE_SET_DEFAULT_MISC + RULE_SET_SECOND_PASS):
            Status = RotorInResRule() && Status;
            Status = RotorMtrCtlRule() && Status;
            break;

        // Run on 2nd pass because of TorqueLimitSet dependency
        case (RULE_SET_TORQUE_LIMIT_SET + RULE_SET_SECOND_PASS):
            Status = RotorInResRule() && Status;
            Status = RotorMtrCtlRule() && Status;
            break;

       // Default case
       default:
            break;
    }

    return (Status);
}   // RunRotorLCLRules()


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Resolution of proper parameter-set for rule calculaion.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
int RotorInResRule(void)
{

    float Imped_Scl, Temp1, Temp2;
    float Xm, X1, X2, XST, R1, R2;

    int   Return_Code = SUCCESS;  // 1 = success, 0 = fail


    SET_FV(R_Winding, 0);            // 0 = Wye Connect

    // ======= Input resolution rules (inputs from user inputs) ========

    if (PARM(R_Winding))
    {
        Imped_Scl = 0.3333333333F;   // Delta Connect
    }
    else
    {
        Imped_Scl = 1.0F;            // Wye Connect
    }


    // ======= Input resolution for EE.X1 EE.X2, EE.XM, EE.XST ===========

    if (ISNAN(PARM(S_Xm)))
    {
        Xm = (PARM(S_Volts) * 3.00F / (SQRT3 * PARM(S_Amps)));
    }
    else
    {
        Xm = PARM(S_Xm) * Imped_Scl;
    }

    if (ISNAN(PARM(S_X1)))
    {
        X1 = (PARM(S_Volts) * 0.10F / (SQRT3 * PARM(S_Amps)));
    }
    else
    {
        X1 = PARM(S_X1) * Imped_Scl;
    }
    S_X1 = X1;

    if (ISNAN(PARM(S_X2)))
    {
        X2 = (PARM(S_Volts) * 0.10F / (SQRT3 * PARM(S_Amps)));
    }
    else
    {
        X2 = PARM(S_X2) * Imped_Scl;
    }

    if (ISNAN(PARM(S_XST)))
    {
        XST =  (X1 + X2) * 0.7F * Imped_Scl;
    }
    else
    {
        XST = PARM(S_XST);
    }

    // ======= Input resolution for EE.R1 EE.R2  ===========

    if (ISNAN(PARM(S_R1)))
    {
        R1 = (PARM(S_Volts) * 0.005F / (SQRT3 * PARM(S_Amps)));
    }
    else
    {
        R1 = PARM(S_R1) * Imped_Scl;
    }

    if (ISNAN(PARM(S_R2)))
    {
        R2 = (PARM(S_Volts) * 0.005F / (SQRT3 * PARM(S_Amps)));
    }
    else
    {
        R2 = PARM(S_R2) * Imped_Scl;
    }


    Temp1 = ((Xm * X1)/(Xm + X1) + X2)/ (PI2 * PARM(S_Hz) * PARM(S_RRatio)*PARM(S_RRatio));

    SET_FV(R_LSigma, Temp1);
    R_LSigmaDtT2 = PARM(R_LSigma)*DelTm2Rcp;

    Temp1 =  XST / (PI2 * PARM(S_Hz) * PARM(S_RRatio)*PARM(S_RRatio));

    #ifdef LEG_TEST
    if ( PARM(TestMode) == LF_LEG_TEST )
    {
        if (PARM(ConfigForSep))
        {
            SET_FV(R_LSigmaSt, 1.0e-3F);
        }
        else
        {
            SET_FV(R_LSigmaSt, (2.2e-3F/3.0F));  // this was for the unit in lab 760
        }                                      // P.R_LSigmaStis used for lockout comp gain
    }
    else
    {
    #endif
        SET_FV(R_LSigmaSt, Temp1);
    #ifdef LEG_TEST
    }
    #endif

//    don't push this fault - allows manipulation of P.S_XST to ajust ireg response

//    if ((PARM(R_LSigma) <= PARM(R_LSigmaSt)) && (PARM(ExecTskSel) != 1))
//        PUSH_DIAG(R_PrmWndRct);

    Temp1   = 1.0F / (PI2 * PARM(S_Hz));
    Temp2   = 1.0F / (PARM(S_RRatio) * PARM(S_RRatio));

    S_R1    = R1;
    S_R2    = R2;
    S_Ls    = X1 * Temp1;
    S_Lr    = X2 * Temp1;
    S_LmNom = Xm * Temp1;

    R_R1    = S_R1 * Temp2;
    R_R2    = S_R2 * Temp2;
    R_Ls    = S_Ls * Temp2;
    R_Lr    = S_Lr * Temp2;
    R_Lm    = S_LmNom * Temp2;

    S_X1    = X1;
    S_X2    = X2;
    S_Xm    = Xm;

    R_LsRcp          =   1.0F/PARM(R_LSigma);

    SET_FV(S_Rfe, 32.7F);
    SET_FV(R_Rfe, PARM(S_Rfe)*Temp2);
    S_RfeRcp = 1.0F/PARM(S_Rfe);
    R_RfeRcp = 1.0F/PARM(R_Rfe);

    return (Return_Code);
}   // RotorInResRule()


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Generator tuneup derived rule calculations.
//      Converter must be stopped for these calculations to be performed.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
int RotorMtrCtlRule(void)
{
    float   Temp1;
    float   Temp2;
    float   Temp3;
    float   tmpf;
    float   R_Sigma;
    float   Fwk_Bw;
    float   I_Dmping;
    float   Wo;
    float   Wo_Prp;
    float   S_VregBW;
    float   S_IregBW;
    float   Pll_BW;
    float   S_PllWn;
    int     tmpi = 0;
    int     Return_Code = SUCCESS;  // 1 = success, 0 = fail

    S_WRat   = 2.0F*PI*PARM(S_Hz);

    tmpf    =  NoNANLim(SQRT2 * (TurbNpRatWtt / (SQRT3 * PARM(S_Volts))), 1.0e6F, -1.0e6F);
    S_IxRat =  NoNANLim(tmpf * (PARM(R_RPMSync) / PARM(R_RPMNom)), 1.0e6F, -1.0e6F);
    S_IyRat =  NoNANLim(tmpf * sqrtf(1.0F/(PARM(S_PwrFactor)*PARM(S_PwrFactor)) - 1.0F), 1.0e6F, -1.0e6F);
    S_AmpsRat = sqrtf(S_IxRat*S_IxRat + S_IyRat*S_IyRat);
	S_AmpsRatRcp = NoNANLim(1.0F/S_AmpsRat, 1.0e6F, -1.0e6F);
    S_PowerRcp = NoNANLim(1.0F/PARM(S_Power), 1.0e6F, -1.0e6F);
    tmpf     = PARM(S_Volts) / PARM(S_RRatio);
    R_ZBase  = tmpf*tmpf/( PARM(TurbineNpRat) * 1000.0F);
    if ( !PARM(ConverterLoc))
    {
       SET_FV(R_Rcable, 5.2e-3F);
       SET_FV(S_Rcable, 2.5e-3F);
    }
    else   //Salem lab
    {
       SET_FV(R_Rcable, 1.5e-3F);
       SET_FV(S_Rcable, 0.5e-3F);
    }
    R_RPMSyncRcp = 1.0F/PARM(R_RPMSync);

    //       Fundamental Motor and System Data Variable Calculations
    //--------------------------------------------------------------

    R_TrqRat     = NoNANLim(TurbNpRatWtt / (PARM(R_RPMNom) * PI2 * 0.016666666F), 1.0e6F, -1.0e6F);
    R_TrqRatRcp  = NoNANLim(1.0F / R_TrqRat, 1.0e6F, -1.0e6F);

    // The following rotor "rated" values correspond to 1.0 pu stator current,
    // 1.0 pwr factor and maximum operating speed. Mostly used just for PU
    // scale factors for display.

    R_RPMRat     = PARM(R_RPMMaxOp);
    R_HzRat      = PARM(S_Hz) * (PARM(R_RPMSync) - PARM(R_RPMMaxOp))/PARM(R_RPMSync);

    Temp1        = (PARM(S_Volts) + (S_IxRat * SQRT2_RCP) * S_R1) * SQRT2_3;

    R_FlxRat     = Temp1 / S_WRat * ( (S_LmNom+S_Lr)/S_LmNom ) / PARM(S_RRatio);

    R_IyRat      = (Temp1 / (S_LmNom*S_WRat)) * PARM(S_RRatio);
    R_IxRat      = -1.0F * S_IxRat * PARM(S_RRatio) * (S_LmNom+S_Ls)/S_LmNom;
    R_AmpsRat    = sqrtf(R_IxRat*R_IxRat + R_IyRat*R_IyRat);

    R_VxRat      = R_IxRat * R_R2 - 2.0F*PI * R_HzRat * R_FlxRat;
    R_VyRat      = R_IyRat * R_R2 + 2.0F*PI * R_HzRat * PARM(R_LSigma) * R_IxRat;
    R_VoltsRat   = sqrtf(R_VxRat*R_VxRat + R_VyRat*R_VyRat);
    R_VRatRcp    = 1.0F/R_VoltsRat;

    R_PowerRat   = 1.5F * ( R_VxRat * R_IxRat  +  R_VyRat * R_IyRat );

    // Add in series cable resistance to base generator values for flux
    // model and regulator feedforwards and gains.
    S_Rs    = S_R1 + PARM(S_Rcable);
    R_Rr    = R_R2 + PARM(R_Rcable);
    S_Rr    = S_R2 + PARM(R_Rcable) * (PARM(S_RRatio) * PARM(S_RRatio));
    R_Rs    = R_R1 + PARM(S_Rcable) / (PARM(S_RRatio) * PARM(S_RRatio));

    S_RRatioRcp = 1.0F/PARM(S_RRatio);


    //       Rule Calculations
    //------------------------

    // Stator Pll items
    S_VlnIn = PARM(S_Volts)*SQRT2*SQRT3_RCP;
    S_VlnInRcp  = 1.0F/S_VlnIn;
    if(SimAcclAct)
    {
        SET_FV(S_PllPrvTm  , 0.00F);    //  acceleration for S_PllProven
    }
    else
    {
        SET_FV(S_PllPrvTm  , 0.120F);
    }
    SET_FV(S_PllIntFilW, 50.0F);



    SET_FV(S_PllWMaxOfL, 2.0F   * PARM(L_LineFrq));
    SET_FV(S_PllWMinOfL, 0.250F * PARM(L_LineFrq));

    S_PllWNom         = PARM(L_LineFrq);

    SET_FV(S_AcWdogTm,    4.0F);
    SET_FV(S_PllErrW,     54.0F);
    SET_FV(S_VxVyErrW,   300.0F);
    SET_FV(S_LineVLowOn, 0.65F);
    SET_FV(S_LineVLowOf, 0.70F);
    SET_FV(S_LineVLowTm, 0.008F); //50hz, .9pu line (min low line)
    SET_FV(S_VMagWUp   , 200.0F);
    SET_FV(S_VMagWDn   , 2.0F);



    SET_FV(S_PllBwBase,  18.75F);
    if ( PARM(TestMode) == PUMP_BACK )
    {
       SET_FV(S_PllBaseHot, 200.0F);
    }
    else
    {
       SET_FV(S_PllBaseHot, 893.0F);
    }

    //Run time gains
    Pll_BW  = PARM(S_PllBwBase);
    S_PllWn = 0.725F*Pll_BW;
    SET_FV(S_PllPrpGnR, Pll_BW*S_VlnInRcp); // W=Prp_Gn/Vln(peak):
    SET_FV(S_PllIntGnR, S_PllWn*S_PllWn*S_VlnInRcp);

    //Hot gains
    Pll_BW = PARM(S_PllBaseHot)*200.0e-6F*R_DelTm1Rcp; //1014 --> 893 change to give prp gain of .9 instead of 1.022 to fix high impd stabil
    S_PllWn = 0.33878F*SQRT_5DIV52*Pll_BW; //SQRT(5/52.41201) added to produce int gain of 5 instead of 52.41201

    if (SimAcclAct)
    {
        SET_FV(S_PllPrpGnI, Pll_BW*S_VlnInRcp*PARM(S_PllAcclSim));  //  acceleration for stator PLL
        SET_FV(S_PllIntGnI, S_PllWn*S_PllWn*S_VlnInRcp*PARM(S_PllAcclSim));
    }
    else if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(S_PllPrpGnI, Pll_BW*S_VlnInRcp); //W=Prp_Gn*Vln(peak):W=B
        SET_FV(S_PllIntGnI, S_PllWn*S_PllWn*S_VlnInRcp);
    }
    else
    {
        SET_FV(S_PllIntGnI, 13.0F);
        SET_FV(S_PllPrpGnI, 0.494F);
    }

    SET_FV(S_PllIniThrs,  9.0F*4.307e-3F*L_VlnIn);              //18.0% imbalance = ???deg*/
    SET_FV(S_PllRunThrs,  1.73F*18.0F*4.307e-3F*PARM(L_VllIn)); //1.73 times  L_PhImbThrs
                                                                //2.595 in 2.x instead of 1.73
    SET_FV(S_PllErSpill, 1.773e-5F*L_VlnIn*L_VlnIn);

    SET_FV(S_PllWPrec, S_WRat);
    SET_FV(S_PllWMaxI, 1.70F * PARM(L_LineFrq));   //HOT_DYN  clamps
    SET_FV(S_PllWMinI, 0.30F * PARM(L_LineFrq));



    S_VregBW = 10.0F;
    if (SimAcclAct)
    {
        SET_FV(S_VregIntGn, 8.7F*PARM(S_VregAcclSim));  //  acceleration for stator voltage regulator
        SET_FV(S_VregPrpGn, 1.7F*PARM(S_VregAcclSim));  //  acceleration for stator voltage regulator
    }
    else if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(S_VregIntGn, 8.7F);//Needs Rule
        SET_FV(S_VregPrpGn, 1.7F);//Needs Rule
    }
    else
    {
        SET_FV(S_VregIntGn, 0.08F);
        SET_FV(S_VregPrpGn, 0.01F);
    }

    SET_FV(R_IyNotchFilForce, false);


    SET_FV(S_VrefSclFlW, 50.0F);
    SET_FV(S_VregMax, 0.75F * PARM(S_Volts)*SQRT2_3/(S_LmNom*S_WRat)); // .75 * rated stator mag current
    SET_FV(S_VrfSyncScl, 1.0F);

    S_IregBW = 10.0F;
    SET_FV(S_IyregIntGn, S_IregBW);
    SET_FV(S_IyregPrpGn, 0.0F);
    if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(S_IyregMax,  0.75F * PARM(S_Volts)*SQRT2_3/(S_LmNom*S_WRat)); // 0.75 * rated stator mag current
    }
    else
    {
        SET_FV(S_IyregMax,  75.0F);
    }

    SET_FV(S_IxregIntGn, S_IregBW * PARM(S_RRatio));
    SET_FV(S_IxregPrpGn, 0.0F);
    SET_FV(S_IxregMax, 0.1F * S_IxRat * PARM(S_RRatio));

    SET_FV(S_IMagK1StkW, DelTm3Rcp);  // Oct 9, 2008--Remove filtering for the release....20.0rad/sec was the tested value,
                                      // should it been needed again.

    // start sequence related parms
    if(SimAcclAct)
    {
        SET_FV(S_PhsSyncTol ,  1e6);    // force S_PhsSync=true in acceleration
    }
    else
    {
        SET_FV(S_PhsSyncTol ,  2.0F * DEG_TO_RAD);               // phase sync tolerance (rad)
    }
    SET_FV(S_SyncTmOut  ,  3.0F);                            // synchronization timeout (sec)
    if(SimAcclAct)
    {
        SET_FV(S_SetlTm1    ,  0.0F);   //  acceleration for SYNC_ENB_6
    }
    else
    {
        SET_FV(S_SetlTm1    ,  1.0F);   // delay after stator vreg enab to check s volts (sec)
    }
    SET_FV(S_FreqMtchFrzTm, 1.0F);
    SET_FV(S_VoltMtchFrzTm, 1.0F);
    SET_FV(S_FreqMtchTm, 1.0F);
    if(SimAcclAct)
    {

        SET_FV(S_SyncDly    ,  0.2F);   //  delay 0.2 sec for SYNC_ENB_14 in acceleration mode
        SET_FV(S_PllHotDly  ,  PARM(S_SyncDly)-0.1F);   //  reduce S_PllHotDly time
        SET_FV(S_SetlTm2    ,  0.0F);   //  bypass SYNC_ENB_12 in acceleration mode
    }
    else
    {
        SET_FV(S_SyncDly    ,  1.0F);                            // delay after commanding sync before checking sync.  Also allows Xm adapt settling.
        SET_FV(S_SetlTm2    ,  1.0F);                        // delay after releasing rotor frequency to check stator freq
        SET_FV(S_PllHotDly  ,  0.5F);                            // delay to allow sluggish gains after volt and phase sync allowed
    }


    SET_FV(S_WFrqDif    , 10.0F);                            // filter w for frequency errors (r/s)

    SET_FV(S_WPhsErr    ,100.0F);                            // filter w for phase errors (r/s)
    SET_FV(S_PllSetlTm  ,  2.0F);                             // stator pll timeout at start (sec)
    if(SimAcclAct)
    {
        SET_FV(S_SyncFrqTol ,  1.0e6);  //  bypass SYNC_ENB_13 in acceleration mode
        SET_FV(S_WVltErr    , 100.0F);  //  increase bandwidth for S_VregErr filter
    }
    else
    {
        SET_FV(S_SyncFrqTol ,  0.01F * S_WRat);                  // stator freq - line freq tolerance (elec rads/s)
        SET_FV(S_WVltErr    , 10.0F);                            // filter w for voltage (magnitude) errors (r/s)
    }
    SET_FV(S_SyncTm     ,  0.3F);                            // time volt mag & phase must match to declare synced
    SET_FV(S_VMagTol    ,  0.02F * SQRT2_3 * PARM(S_Volts));// voltage check tolerance
    if(SimAcclAct)
    {
        SET_FV(S_VSyncTol   ,  1.0e6);      // force S_VltSync=true in acceleration
    }
    else if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(S_VSyncTol   ,  0.02F * SQRT2_3 * PARM(S_Volts));// voltage match tolerance
    }
    else
    {
        SET_FV(S_VSyncTol   ,  0.024F * SQRT2_3 * PARM(S_Volts));    // 0.6% * 4
    }
    SET_FV(S_PllTacTol  ,  0.01F * S_WRat);                  // stator freq - tach tolerance  (elec rads/s)
    if(SimAcclAct)
    {
        SET_FV(S_VfbMinLvl  ,  0.0F);   //  bypass SYNC_ENB_3 in acceleration mode
    }
    else
    {
        SET_FV(S_VfbMinLvl  ,  0.20F);                           // stator voltage detection level (pu)
    }
    SET_FV(R_CCvoltThrs,   96.0F);                           // Max voltage for dropping crowbar contactor
    R_CCvoltThrsSqr = PARM(R_CCvoltThrs)*PARM(R_CCvoltThrs); // and prevent welding
    SET_FV(R_CCvoltDecayTm, 5.0F);

    // 2nd harmonic reg items
    SET_FV(R_IregResBW, 6.19725F); //Changed 1/15/08 after high/low impedance tests
    SET_FV(R_Res2ndD, 1.3F);    //Changed 1/15/08 after high/low impedance tests
    SET_FV(R_Res2ndQ, 1.7F);    //Changed again 6/27/13 for NGG/1.6e (was 1.0)
    SET_FV(R_IregResMax, 0.20F);
    if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(R_IregResWI, PARM(R_IregResBW)*SQRT3/PARM(L_VdcRef));
        SET_FV(R_IregResLim, PARM(R_IregResMax)/
             (PARM(R_IregResWI)*sqrtf(PARM(R_Res2ndD)*PARM(R_Res2ndD) + PARM(R_Res2ndQ)*PARM(R_Res2ndQ))));
    }
    else
    {
        SET_FV(R_IregResWI,     0.0F);
        SET_FV(R_IregResLim,    0.0F);
    }




    //  Misc. regulator gains
    //-----------------------
    SET_FV(R_VdcMeld, 0.0F) ;                 // Vdc compensation meld 0 - all feedback

    // Inner regulator filter gains : do not group with diagnostic filter items
    SET_FV(R_WeFfW1, 40.0F);                  // Voltage feedforward We filter (Tach)


    SET_FV(R_WRtrW, DelTm2Rcp);              // inner speed filter BW (Tach)
    SET_FV(R_WIy, 1000.0F);                  // Torque compensation used Iy filter


    //  Current Magnitude limit
    //-------------------------
    SET_FV(R_IyLimW, 200.0F);                  // Iy filter bandwidth

    // Current Limit Max
    //------------------


    //  Motor Pole pairs
    //------------------
    if ( PARM(TestMode) == PUMP_BACK )
    {
        SET_FV(R_PolePairs, 3.0F);      // Actual machine is not used in pump-back, Pole-pairs are
    }
    else
    {
       tmpi = (int)(60.0F * PARM(S_Hz) / PARM(R_RPMSync) + 0.5F);
       SET_FV(R_PolePairs, (float)tmpi);
    }

    //  Stator minimum flux
    //---------------------
    if ( PARM(TestMode) == PUMP_BACK)
    {
         SET_FV(S_FlxMin, NoNANLim(PARM(S_RRatio)/(PARM(R_PolePairs)*1.5F*SQRT2), 1.0e6F, -1.0e6F));
    }
    else
    {
        SET_FV(S_FlxMin, 0.05F * (PARM(S_Volts) * SQRT2_3 / S_WRat));     // value needed to maintain max Ix during lvrt
    }


    // Power dip
    //----------

    SET_FV(R_PdActLvl, PARM(L_VdcRefLvrt) - 90.0F);
    SET_FV(R_PdRcyLvl, PARM(R_PdActLvl) + 25.0F);
    SET_FV(R_PdLim,     0.4F  * ABS(R_IxRat));
    SET_FV(R_PdPrpGn,  200.0F* PARM(L_CBrg));  // needs work

    SET_FV(R_PdTmMode, 1);      // formerly user parm
    SET_FV(R_PwrDipTm, 0.0F);    // formerly user parm

    switch(PARM(R_PdTmMode))
        {
            case 0:
                Temp1 = 0.008F;             // Disable
                break;
            case 1:
                Temp1 = 1.1F;               // Enable
                break;
            case 2:
                Temp1 = PARM(R_PwrDipTm);   // Custom
                break;
            default:
                Temp1 = 0.008F;
                break;
        }
    SET_FV(R_PdTm, Temp1);
    SET_FV(R_PdSpdThr, 0.05F * PARM(R_RPMSync));

    // Regen bus regulation
    //---------------------

    // Note: R_VllRuleRgn is calculated in BrgInfcRule()



    if ( PARM(TestMode) == PUMP_BACK )
    {
       SET_FV(LvPwrLmPBScl,  3.333F);
    }
    else
    {
       SET_FV(LvPwrLmPBScl, 1.0F);
    }


    //  Command Ix Current Delay Tap
    //------------------------------

//        if ( (ISNAN(PARM(At_Brg_Flx)) == true) && (ISNAN(PARM(At_Lsig_Strt)) == true) )
        if ( true )
        {
            // Autotune Lsigma has not accomplished
            Temp1 = 1.0F;
            Temp2 = 0.0F;
            Temp3 = 0.0F;
            tmpf  = 0.0F;
        }
        else
        {
            // Autotune Lsigma has accomplished
            Temp1 = 0.0F;
            Temp2 = 0.0F;
            Temp3 = 0.5F;
            tmpf  = 0.5F;
        }

        SET_FV(R_IxP0Gn,Temp1);
        SET_FV(R_IxP1Gn,Temp2);
        SET_FV(R_IxP2Gn,Temp3);
        SET_FV(R_IxP3Gn,tmpf);


        SET_FV(R_IxI1Gn,0.0F);
        SET_FV(R_IxI2Gn,0.5F);
        SET_FV(R_IxI3Gn,0.5F);

    //  Pull-Out Ix limit scaler
    //--------------------------
    if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(R_PullOutScl,1.0F);
    }
    else
    {
        SET_FV(R_PullOutScl,       10.0F);
    }

    // R_Sigma (local)
    R_Sigma = R_R1 + R_R2;

    //  X-axis Current Regulator
    //--------------------------

    I_Dmping = 0.9F;

  //  SET_PARM(R_IxregPrpGn, PARM(Ireg_Bw) * PARM(R_LSigmaSt));
  //  SET_PARM(R_IxregIntGn, (R_Sigma + PARM(Ireg_Bw) * PARM(R_LSigmaSt))/
  //                       (4.0F * I_Dmping * I_Dmping));

    Wo     = 1.0F/ (4.0F * I_Dmping * I_Dmping * 2.0F * R_DelTm1);
    Wo_Prp = 0.25F * R_DelTm1Rcp;

    if ( PARM(TestMode) == PUMP_BACK && PARM(SoftwareIPN)>=DFIG02_2400_5060_A)
    {
        SET_FV(R_IxregLim, 350.0F);
        SET_FV(R_IyregLim, 350.0F);
    }
    else
    {
        SET_FV(R_IxregLim, (0.5F * R_VxRat));
        SET_FV(R_IyregLim, 0.5F * R_VxRat);
    }


    if ( PARM(SeriesCmpEnb))
    {
       SET_FV(R_IyregPRtio, 1.0F);
       SET_FV(R_IyregPRto0, 3.42F);
       SET_FV(R_IyregIRtio, 1.0F);
       SET_FV(R_IyregPSpd, PARM(R_RPMSync) );
       SET_FV(R_IyregPSpd0, PARM(R_RPMMinOp) );
       SET_FV(R_IxregPrpGn, 0.45977F * R_ZBase); //0.45977 is number required to have a gain of 1.0 with selected 1.50 MW machines
       SET_FV(R_IxregIntGn, 10.0F * PARM(R_IxregPrpGn) );
       SET_FV(R_IregErr2W, 25.0F);
       SET_FV(R_IregPGn2,  2.0F * PARM(R_IxregPrpGn) );
       SET_FV(R_DcIregBW, 350.0F);//RESULTS OF EXTENSIVE TESTING IN 09/2010 in non-ESS product
       SET_FV(R_DcIregMaxDynEnb, false);
       SET_FV(R_DcIregMax, 0.6F);
       if ( PARM(R_DcIregMaxDynEnb) )
       {
          SET_FV(R_DcIregMax2,0.85);
       }
       else
       {
          SET_FV(R_DcIregMax2,PARM(R_DcIregMax) );
       }

       SET_FV(R_DcIregMax1Pwr, 0.50F);
       SET_FV(R_DcIregMax2Pwr, 1.0);
    }
    else // not series comp
    {
       SET_FV(R_IyregPRtio, 1.0F);
       SET_FV(R_IyregPRto0, 3.0F);
       SET_FV(R_IyregIRtio, 1.0F);
       SET_FV(R_IyregPSpd, PARM(R_RPMMinOp) );
       SET_FV(R_IyregPSpd0, PARM(R_RPMSync) );
       SET_FV(R_IxregPrpGn, PARM(R_LSigmaSt) * Wo_Prp);
       SET_FV(R_IxregIntGn, R_Sigma * Wo);
       SET_FV(R_IregErr2W, 25.0F);
       if ( PARM(TestMode) == PUMP_BACK )
       {
          SET_FV(R_IregPGn2, 0.0F);
       }
       else
       {
          SET_FV(R_IregPGn2, 2.0F);
       }
       SET_FV(R_DcIregBW, 350.0F);
       SET_FV(R_DcIregMaxDynEnb, false);
       SET_FV(R_DcIregMax, 0.1F);
       SET_FV(R_DcIregMax2,0.1F);
       SET_FV(R_DcIregMax1Pwr, 0.50F);
       SET_FV(R_DcIregMax2Pwr, 1.0);
    }
    SET_FV(R_DcRegIntW, 0.10); //10.0 sec tau
    R_DcRegIntWdt = PARM(R_DcRegIntW) * R_DelTm1;

    SET_FV(R_IyregPrpGn, PARM(R_IxregPrpGn) );
    SET_FV(R_IyregIntGn, PARM(R_IxregIntGn) * PARM(R_IyregIRtio) );

    R_IregErr2Gn    =  PARM(R_IregErr2W) * R_DelTm1;
    //Calculate slope for dynamic proportional gain
    R_IyregPSlp = ( PARM(R_IyregPRtio) - PARM(R_IyregPRto0) )/( PARM(R_IyregPSpd) - PARM(R_IyregPSpd0) );

    SET_FV(R_Dc1stD,1.0F);      //set to value of old rule output for 60hz.
    SET_FV(R_Dc1stQ,0.6F);
    if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(R_DcRegWI, PARM(R_DcIregBW)*SQRT3/PARM(L_VdcRef));
        SET_FV(R_IregDcLim, PARM(R_DcIregMax)/
             (PARM(R_DcRegWI)*sqrtf(PARM(R_Dc1stD)*PARM(R_Dc1stD) + PARM(R_Dc1stQ)*PARM(R_Dc1stQ))));
        SET_FV(R_IregDcLim2, PARM(R_DcIregMax2)/
             (PARM(R_DcRegWI)*sqrtf(PARM(R_Dc1stD)*PARM(R_Dc1stD) + PARM(R_Dc1stQ)*PARM(R_Dc1stQ))));
    }
    else
    {
        SET_FV(R_DcRegWI,       0.0F);
        SET_FV(R_IregDcLim,     0.0F);
        SET_FV(R_IregDcLim2,    0.0F);
    }

    R_DcIregMaxSlope = ( PARM(R_IregDcLim2) - PARM(R_IregDcLim))/( PARM(R_DcIregMax2Pwr) - PARM(R_DcIregMax1Pwr) );



    //  Modulation Index
    //------------------
    SET_FV(R_ModIndMax, 1.10F);        // revert back to older ess rule per AMK (11/8/12)
    SET_FV(R_ModIndMax2,1.40F);        // stay in control of current better with lower value

    SET_FV(VregMinLimFwk, -1.4537);    // VregMinLim when rotor mod index limiting (same rule as HvIyRat)
    SET_FV(VregMinLimFwkW, 10.0);      // Filter omega to restore VregMinLim to nominal

    // Torque Compensation control
    //----------------------------

    SET_FV(R_IxCompEnb, true);                  // Enables stator Ix regulator


    // Xm adaptation control
    //----------------------
    if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(S_LmCalcMeld, 1.0F);           // Xm modulation meld (0=defeat, 1=full enable)
    }
    else
    {
        SET_FV(S_LmCalcMeld,        0.0F);
    }
    SET_FV(S_LmCalcWR, 2.0F);             // Xm modulation filter - Running (rad/s)
    SET_FV(S_LmCalcWI, 5.0F);             // Xm modulation filter - Init (rad/s)
    SET_FV(S_LmCalcMax, 1.2F * S_LmNom);  // Xm modulation max limit
    SET_FV(S_LmCalcMin, 0.6F * S_LmNom);  // Xm modulation min limit


    //  Tach_Loss_Detection
    //---------------------


    if ( PARM(R_TachPPR) == 0 )
    {
        tmpf = 600.0F;
    }
    else
    {
        tmpf = (float)PARM(R_TachPPR);
    }
    tmpf = (60.0F/(tmpf * 2.0F * Tac_Read_Tm * 2.0F)) * RPM_TO_RAD; // rad/sec
    // Tac_Read_Tm is the double read time separation (exec time between Smp1Encod and Smp2Encod)
    // 2.0 (100% margin for duty ratio error)

    // Fractional Tach Feedback
    // ESS is 30 MHz, WindDFIG01 is 1MHz
    //----------------------------------
    SET_FV(R_TachLtScl, (1.0e-6F/1.0F) );                   // fpga rev REBF0C or later: exactly 1.0 usec
    SET_FV(R_TachFrcScl, R_T1FramesPerT2*(1.0e-6F/1.0F));   //  earlier versions clock was 0.96 usecwas 0.96 usec
    SET_FV(R_SpdScl2,    PI2/ (PARM(R_TachPPR) * PULSE_SCL) );



    //Slow Task 1 speed and angle correction scheme
    SET_FV(R_AngCorW, 50.0F);
    SET_FV(R_SpdFrcFilW, 40.0F);
    SET_FV(R_RtrPosMeth, NEW_SPD_ANGL_METH );
    SET_FV(R_SpdT2RAFN, 36);
    SET_FV(R_AngCorDt, 0.5 * PARM(R_SpdT2RAFN) * DelTm2);
    SET_FV(R_AngEstFFW, 500.0F);
    SET_FV(R_AngFFGn,   PARM(R_AngCorDt) * DelTm2Rcp);
    SET_FV(R_SpdTcRAFN, 51);
    SET_FV(R_SpdRAFCrGn, 0.01F);
    SET_FV(R_SpdTcFFRto, 0.0F);
    SET_FV(R_SpdTcFFW,   0.0F);
    R_SpdTcFFGn=PARM(R_SpdTcFFW)*DelTm2;
    SET_FV(R_AngCorPIW, 20.0);
    R_AngCorPIGn = DelTm2*PARM(R_AngCorPIW);

    if ( PARM(R_SpdT2RAFN) > SPD_RAF_MAX_N)   //limit to size of array
    {
       R_SpdT2RAFN =  SPD_RAF_MAX_N;
    }
    else if ( PARM(R_SpdT2RAFN) < 1)
    {
       R_SpdT2RAFN = 1;
    }
    else
    {
       R_SpdT2RAFN = PARM(R_SpdT2RAFN);
    }
    R_SpdRAFNRcp       = 1.0F/R_SpdT2RAFN;


    if ( PARM(R_SpdTcRAFN) > SPD_RAF_MAX_N)   //limit to size of array
    {
       R_SpdTcRAFN =  SPD_RAF_MAX_N;
    }
    else if ( PARM(R_SpdTcRAFN) < 1)
    {
       R_SpdTcRAFN = 1;
    }
    else
    {
       R_SpdTcRAFN = PARM(R_SpdTcRAFN);
    }
    R_SpdRAFNTRcp     = 1.0F/R_SpdTcRAFN;;

    SET_FV(R_TachPosRatio, 1000.0F);
    SET_FV(R_TachPosMax, 4 * PARM(R_TachPosRatio) * PARM(R_TachPPR) );  // integer
    SET_FV(R_TachPosScl,  360.0F * 1000.0F / PARM(R_TachPosMax) );   // float

    // Determine speed feedback scaling
    R_SpdScl    = PI2 / (PARM(R_TachPPR) * PULSE_SCL * R_DelTm1);

    // Determine speed quanta for emulated tach crunchies
    R_SpdQnt      = PI2 / (PARM(R_TachPPR) * PULSE_SCL * R_DelTm1);
    R_SpdQntInv  = 1.0F / R_SpdQnt;

    // From B0spd
    // Determine speed and position feedback scaling
    R_TachMax    = (PARM(R_TachPPR) * PULSE_SCL);
    R_TacMaxHalf = R_TachMax/2;
    R_PosScl     = PI2 * PARM(R_PolePairs) / R_TachMax;

    // Calculate tach diagnostic thresholds
    R_MrkrPlsAlm = PARM(R_MrkrTplThr) + R_TachMax;                          // pulse edges
    R_MrkrPlsFlt = PARM(R_MrkrTplFlt) + (R_TachMax * PARM(R_MrkrMisFlt));   // pulse edges


    if ( PARM(R_WTrqTrk) > DelTm3Rcp )
    {
        R_TrqTFilGn = 1.0F;                 // limit filter W if parm > max
    }
    else if ( PARM(R_WTrqTrk) == 0.0F)
    {
        R_TrqTFilGn = 1.0F;                 // bypass filter if W parm = 0.
    }
    else
    {
        R_TrqTFilGn = PARM(R_WTrqTrk) * DelTm3;
    }

    //  Voltage Regulators
    //  Compensation for modulator:  0.0508 from experimental data for PWM2
    //  1.1 = (10% extra margin for dc bus variation)
    //  0.15 assume max overmodulation = 0.15

    tmpf = ((0.15F - 0.0508F) * (PARM(R_VdcNom) / SQRT3) * 1.1F + 50.0F) *
            (1.0F + PARM(R_VdcMeld));
                                    // Assume 50 volts = worse case bridge error

    SET_FV(R_VrgIntLmHi, tmpf); //Used only transiently for island/HVRT

    // 2/6/13: Activating rotor inner voltage regs to try to improve lockout induced harmonics
    //         set limit to Vdc * LockoutTm / DelTm1;
    //         Leave gains at 0 until simulations are done
    tmpf = PARM(R_VdcNom) * PARM(BrgLockoutTm) * R_DelTm1Rcp;

    SET_FV(R_VyregLim, 0.0F);

    SET_FV(R_VxregLim, 0.0F);
    SET_FV(R_VxregUxLim, 1.12F);
    SET_FV(R_VyregUyLim, 1.12F);

    SET_FV(R_VxregIntGn, 0.0F);
    SET_FV(R_VyregIntGn, 0.0F);

    SET_FV(R_VrgIntGnHi, 800.0F); //transient islanding/ HVRT gain
    SET_FV(R_VrgIntHiTm, 0.005F); //transient islanding/ HVRT gain time
    // reduce maximum fast reactive current time during HVRT curve for series compensation
    if ( PARM(SeriesCmpEnb))
    {
       SET_FV(S_VMaxIyFsTm, 0.003F); //max fast reactive current time
    }
    else
    {
       SET_FV(S_VMaxIyFsTm, 0.015F); //max fast reactive current time
    }
    if(SimAcclAct)
    {
        SET_FV(R_PrefluxTm, 0.0F);  //  bypass SYNC_ENB_2 in acceleration mode
    }
    else
    {
        SET_FV(R_PrefluxTm, 1.0F);              // fault if no stator volts seen after this time
    }
    SET_FV(R_WStrtVI, 5.0F);                // filter W for rotor  starting V & I's (rad/sec)
    if(SimAcclAct)
    {
        SET_FV(R_RrFltThr, 1.0e6F); //  avoid R_RtrConFlt in SYNC_ENB_3 in acceleration mode
    }
    else
    {
        SET_FV(R_RrFltThr, 20.0F * R_R2);       // fault if high rotor resistance detected
    }

    // Motor Flux Model
    // Damping gains
    //-----------------
    SET_FV(R_FlxDmpWd,  25.0F); // rad./sec.
    SET_FV(R_FlxDmpKd,  PARM(R_FlxDmpWd) * 0.05F);


    //  Field-Weakening Limiter
    //-------------------------

    Fwk_Bw = 10.0F;

    // Obtain highest operating rotor frequency
    Temp1 = PARM(S_Hz) * (PARM(R_RPMMaxOp) - PARM(R_RPMSync)) /  PARM(R_RPMSync);
    Temp2 = PARM(S_Hz) * (PARM(R_RPMSync)  - PARM(R_RPMMinOp)) / PARM(R_RPMSync);

    if (Temp2 > Temp1) Temp1 = Temp2;
    Temp1 = 2.0F * PI * Temp1;

    // Set gain to obtain target bandwidth at max rotor freq
    // actual bw will decrease to 0 as rotor frequency reaches 0
    // SET_FV(R_FwkIntGn, (Fwk_Bw * PARM(R_VdcNom)) / (SQRT3 * PARM(R_LSigma) * Temp1) );
    SET_FV(R_FwkIntGn,  3.0F);
    SET_FV(R_FwkIntGn2, 1.0F);
    SET_FV(R_VMaxIntGn, 1.0F);
    SET_FV(R_VMaxPu,    1.08F);
    SET_FV(R_FwkRtLim, -0.05F);
    if(SimAcclAct)
    {
        SET_FV(R_FwkMargin, 0.002*PARM(R_FwkMrgAcclSim));   //  increase margin of flux weak control in acceleration mode
    }
    else
    {
        SET_FV(R_FwkMargin, 0.002);
    }
    SET_FV(R_FwkMinLim, 0.90F);
    SET_FV(R_ModLimFbkW, 400.0F);
    SET_FV(R_ModLimFbWD, 25.0F);
    SET_FV(R_ModLimFbW2, 100.0F);
    SET_FV(R_ModLmFbW2D, 50.0F);


    //  Dynamic Rate Limiter Ix
    //-------------------------

    SET_FV(R_LsRcpSlew, 2.0F/(PARM(R_LSigmaSt) + PARM(R_LSigma)));

    // Temp2 = PARM(R_VdcVcoScl) * R_DelTm1Rcp * Temp1;  // PWM2 R_VdcVcoScl = 1198 Volt/2.0MHz

    tmpf = 25.0F; // rad/sec
    SET_FV(R_IxDdtMin, tmpf * ABS(R_IxRat));


    SET_FV(R_VxMargin,  0.0F);

    SET_FV(R_IxVdcGn, 5.0F);
    SET_FV(R_IxVdcSpdGn, PARM(R_IxVdcGn)/(PARM(R_RPMNom) - PARM(R_RPMSync)));
    SET_FV(R_IxVdcOff, 25.0F);
    SET_FV(R_IxVdcRtGn, 5.0F/PARM(R_IxVdcGn)); //Einar wants an effective gain of 5.0 by old method
    SET_FV(R_IyVdcRtGn, 10.0F);

    switch(PARM(SoftwareIPN))
    {
    case DFIG02_2400_5060_A:
        SET_FV(R_IxCmd0Lim, PARM(R_IMagOL));
        break;
    default:
        if(PARM(TestMode) == PUMP_BACK)
        {
            SET_FV(R_IxCmd0Lim, 2909.13F);
        }
        else
        {
            SET_FV(R_IxCmd0Lim, 0.599332*PARM(R_IMagOL)); //0.599332 is 479.166/799.5;   799.5 was the intention of the orginal rule
        }
        break;
    }


    //  Ee.Ci.R_DchgRat - Regulator Discharge Rate
    //--------------------------------------------
    SET_FV(R_DchgRat, 2.0F);             // Regulator discharge rate rad/sec
    tmpf = PARM(R_DchgRat);
    if ( tmpf > R_DelTm1Rcp )
    {
        tmpf = R_DelTm1Rcp;
    }
    R_DchgScl = (1.0F - tmpf * R_DelTm1);
    if (R_DchgScl > 1.0F)
    {
        R_DchgScl = 1.0F;
    }




    SET_FV(R_OffNullDly, 0.05F);   // Seconds

    // Tach diagnostic related parameters
    //-----------------------------------

    SET_FV(R_MarkerOffs, 0.0F);      //
    SET_FV(R_TachZeroDelThrs, 4.0F * PARM(R_PolePairs) * DEG_TO_RAD); // 4 mechanical degrees

    SET_FV(R_TachPlsThr, 3);         // 3 tach pulses

    SET_FV(R_TachPlsFlt, 10);        // 10 tach pulses
    SET_FV(R_MrkrErrLim,(PARM(R_TachPlsThr) + 1));
    SET_FV(R_WTrqTrk,   25.0F);      // rad/sec
    SET_FV(R_MrkrTplThr, 3);         // 3 tach pulses
    SET_FV(R_MrkrMisFlt, 3);         // 3 marker pulses
    SET_FV(R_MrkrTplFlt, 3);         // 3 tach pulses
    SET_FV(R_TrqTrkThr,  0.2F);      // .2 pu


    // Torque error thresholds
    if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(R_TrqErrW,       0.20F);     // rad/s
        SET_FV(R_TrqErAlmLv,    0.05F);     // per unit torque
        SET_FV(R_TrqErFltLv,    0.30F);     // per unit torque
    }
    else
    {
        if( PARM(SoftwareIPN) >= DFIG02_2400_5060_A )
        {
            SET_FV(R_TrqErrW,       0.20F);     // rad/s
            SET_FV(R_TrqErAlmLv,    1.00F);     // per unit torque
            SET_FV(R_TrqErFltLv,    1.25F);     // per unit torque
        }
        else  // for 1.6e products
        {
            SET_FV(R_TrqErrW,       0.20F);     // rad/s
            SET_FV(R_TrqErAlmLv,    1.00F);     // per unit torque - prevent diagnostics in pump-back mode
            SET_FV(R_TrqErFltLv,    1.25F);     // per unit torque
        }

    }

    // DFIG protection
    //----------------
    // synchronization IOC filter omega and threshold
    SET_FV(R_WSyncIOC, 1.0F/R_DelTm1);
    R_SynIOCFilGn = PARM(R_WSyncIOC) * R_DelTm1;

    Temp1 = 2.0F * R_IyRat;
    if(SimAcclAct)
    {
        SET_FV(R_SyncIOCLvl, 1.0e10);   // disable ioc check
    }
    else if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(R_SyncIOCLvl, 1.5F * Temp1 * Temp1 );
    }
    else
    {
        if(PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
        {   SET_FV(R_SyncIOCLvl, 1.5F*900.0F*900.0F); } // 600 on 1.5 - changed to 900 to account for 3:1 transformer instead of 2:1
        else
        {   SET_FV(R_SyncIOCLvl, 1.5F*600.0F*600.0F); } // compatible with 1.6e
    }

    // stator IOC filter omega and threshold
    SET_FV(S_WIOC,  1.0F/DelTm2);

    //Temp1 = 0.98*PARM(S_IMaxLim);
    // SET_FV(S_IOCLvl, 3.0F * Temp1 * Temp1 * .987F);  // ESS rule


    Temp1 = 0.98*PARM(S_IMaxLim);     // saturaiton level of 6 kv CT cfb

    if(SimAcclAct)
    {
        SET_FV(S_IOCLvl, 300 * Temp1 * Temp1 * .987);   //  disable ioc in acceleration mode
    }
    else
    {
        SET_FV(S_IOCLvl, 3.0 * Temp1 * Temp1 * .987);
    }
    // Stator IOC level 2 threshold (2 times rated stator current)
    Temp1 = 2.0F * SQRT2 * PARM(S_Amps);
    SET_FV(S_SyncIOC2Lvl, 1.5F * Temp1 * Temp1);

    // Stator CT error filter omega and threshold
    if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(S_CTIabcLvl,   0.5F * R_IyRat/PARM(S_RRatio));         // amps
    }
    else
    {
        SET_FV(S_CTIabcLvl,              200.0F);
    }
    if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(S_IyCmdCTChk, -1.0F * R_IyRat/PARM(S_RRatio));         // amps
    }
    else
    {
        SET_FV(S_IyCmdCTChk,            -150.0F);
    }
    SET_FV(S_IyCmdCTChkSlwRat,  ABS(PARM(S_IyCmdCTChk)) * 20.0F); // amps /sec (50 msec ramp)

    SET_FV(S_WCTErr2, 100.0F);         // rad/s
    S_CTErrFilGn = PARM(S_WCTErr2) * DelTm2;
    Temp1 = 0.5F*R_IyRat/PARM(S_RRatio);
    if ( PARM(TestMode) != PUMP_BACK )
    {
        SET_FV(S_CTErr2Lvl, Temp1 * Temp1);  // amps^2
    }
    else
    {
        SET_FV(S_CTErr2Lvl, 50.0F*50.0F);
    }
    SET_FV(R_IyCmdW, 50.0F);



    // Miscellaneous  - parms formerly not final values
    //-------------------------------------------------
    SET_FV(R_ICmdTst, 0.0F);

    SET_FV(R_ImagLimEnb, 0);
    SET_FV(R_IyLimAdj, SQRT2 * PARM(R_IMaxLim));

    if(SimAcclAct)
    {
        SET_FV(R_SyncRegGn,  0.0F); //  freeze R_TachZero in acceleration mode
    }
    else
    {
        SET_FV(R_SyncRegGn, 10.0F);
    }

    SET_FV(R_VImpFFGn, 0.4F);
    SET_FV(R_VImpFFGnY, 1.0F);


    SET_FV(S_IxCmdTst, 0.0F);

    SET_FV(R_WVMaxSpd  , DelTm3Rcp);
    SET_FV(R_VMaxOnLvl , PARM(L_VdcRef) / SQRT3);
    SET_FV(R_VMaxOffLvl, 0.99F * PARM(R_VMaxOnLvl));
    SET_FV(R_OVTrpThrs, 1.15F*PARM(S_Volts));

    SET_FV(S_ICompNsThr, 0.09159F*S_VlnIn);     //occurs at 15% imbalance
    SET_FV(S_ICompStRtr, (PARM(S_RRatio)/(PARM(S_X1)+PARM(S_X2))) );


    //--------------------------------------------------------------------------
    if ( PARM(TestMode) != PUMP_BACK )
    {
        if(PARM(SoftwareIPN) >= DFIG02_2400_5060_A)
        {
            SET_FV(R_ILimMax,   750.0F*SQRT2);
        }
        else
        {
            SET_FV(R_ILimMax,   640.0F*SQRT2);
        }
        SET_FV(R_ILimAdj1, SQRT2 * PARM(R_IMaxLim));
    }
    else
    {
        SET_FV(R_ILimMax,   2400.0F*SQRT2);
        SET_FV(R_ILimAdj1, PARM(R_ILimMax));
    }
    SET_FV(R_ILimAdj2, SQRT2 * PARM(R_IMaxLim));


    SET_FV(R_NsXStpAdr,(float *) LADR_FZero);
    SET_FV(R_NsYStpAdr,(float *) LADR_FZero);
    SET_FV(R_TrqStpAdr,(float *) LADR_FZero);
    SET_FV(R_DcXStpAdr,(float *) LADR_FZero);
    SET_FV(R_DcYStpAdr,(float *) LADR_FZero);
    SET_FV(S_PllrStpAdr,(float *) LADR_FZero);

    //Gain calculaations formerly in RotorDiagVarFilterGain
    R_VdcFilGn  = ( PARM(R_WVdcFil) * DelTm2 );

    if ( PARM(R_WIdcFil) > 0.0F )
    {
        R_IdcFilGn = ( PARM(R_WIdcFil) * DelTm2 );
    }
    else
    {
        R_IdcFilGn = 1.0F;
    }

    R_IxFilGn   = ( PARM(R_WIxFil) * DelTm2 );
    R_IyFilGn   = ( PARM(R_WIyFil) * DelTm2 );
    R_VxFilGn   = ( PARM(R_WVxFil) * DelTm2 );
    R_VyFilGn   = ( PARM(R_WVyFil) * DelTm2 );
    R_TrqFilGn  = ( PARM(R_WTrqFil) * DelTm2 );
    R_TrqFbkFGn = ( PARM(R_WTrqFbkFil) * DelTm2 );
    R_FlxFilGn  = ( PARM(R_WFlxFil) * DelTm2 );
    R_SpdGenGn  = ( PARM(R_WSpdGen) * DelTm2 );
    R_SpdFbkGn  = ( PARM(R_WSpdFbk) * DelTm2 );
    R_CnvPwrGn  = ( PARM(R_WCnvPwr) * DelTm2 );
    R_QPwrFilGn = ( PARM(R_WQPwrFil)* DelTm2 );
    R_BusPwrGn  = ( PARM(R_WDcBusPwr) * DelTm2 );
    R_FrqFilGn  = ( PARM(R_WFrqFil) * DelTm2 );

    S_IxFilGn    = ( PARM(S_WIxFil) * DelTm2 );
    S_IyFilGn    = ( PARM(S_WIyFil) * DelTm2 );
    S_VxFilGn    = ( PARM(S_WVxFil) * DelTm2 );
    S_VyFilGn    = ( PARM(S_WVyFil) * DelTm2 );
    S_VMagFilGn  = ( PARM(S_WVMagFil) * DelTm2 );
    S_PwrFilGn   = ( PARM(S_WPwrFil)  * DelTm2 );
    S_QPwrFilGn  = ( PARM(S_WQPwrFil) * DelTm2 );

    S_IMgK1StkGn = ( PARM(S_IMagK1StkW) * DelTm3 );

        // Task 3 diagnostic variable filter gains

    R_IMagFilGn = ( PARM(R_WIMagFil) * DelTm3 );
    R_VMagFilGn = ( PARM(R_WVMagFil) * DelTm3 );
    S_IMagFilGn = ( PARM(S_WIMagFil) * DelTm3 );



    ////////////////////////////////
    // Islanding-HVRT Rules
    ////////////////////////////////

    SET_FV(R_VMgIxDcyMn, 0.0*SQRT2*PARM(R_IRatSSLim));
    SET_FV(R_VMgIxDcyM2, 0.50*SQRT2*PARM(R_IRatSSLim));
    SET_FV(R_VMgIxDcyW, 200.0);

    SET_FV(S_VMagPuIyLm, 1.12);
    SET_FV(S_VMaxIyBias, PARM(S_VMaxHiDtOn) - PARM(S_VMagPuIyLm));

    SET_FV(S_VMaxIyLm, -HvIyRatPk*INomGridPk);

    SET_FV(S_VMaxIyGn,  NoNANLim(-PARM(S_VMaxIyLm) / PARM(S_VMaxIyBias), 1.0e6F, -1.0e6F) );

    R_VMgIxDcyGn    = PARM(R_VMgIxDcyW) * R_DelTm1;

    ////////////////////////////////
    // End Islanding - HVRT rules
    ////////////////////////////////

     //////////////////////////////////////////////////////////////////////////////////////////
     // Set S-domain gains and calculate z domain gains for Rotor current command notch filter
     // The S-domain notch filter has a standard second order transfer function:
     //                         A2*s^2 + A1*s + A0
     //                 H(s) =  ------------------
     //                         B2*s^2 + B1*s + 1
     //////////////////////////////////////////////////////////////////////////////////////////

     //Set final values for notch filter with 30Hz central frequency and 1.5 damping factor
     SET_FV(R_IxNchFilSA0, 1.0F);
     SET_FV(R_IxNchFilSA1, 1.6e-5F);
     SET_FV(R_IxNchFilSA2, 2.8145e-5F);
     SET_FV(R_IxNchFilSB1, 1.6e-2F);
     SET_FV(R_IxNchFilSB2, 2.8145e-5F);

     SET_FV(R_IyNchFilSA0, 1.0F);
     SET_FV(R_IyNchFilSA1, 1.6e-5F);
     SET_FV(R_IyNchFilSA2, 2.8145e-5F);
     SET_FV(R_IyNchFilSB1, 1.6e-2F);
     SET_FV(R_IyNchFilSB2, 2.8145e-5F);

     // Populate the s-domain structure from final values for Rotor Ix notch filter
     R_CurCmdFil2S.A0   = PARM(R_IxNchFilSA0);
     R_CurCmdFil2S.A1   = PARM(R_IxNchFilSA1);
     R_CurCmdFil2S.A2   = PARM(R_IxNchFilSA2);
     R_CurCmdFil2S.B1   = PARM(R_IxNchFilSB1);
     R_CurCmdFil2S.B2   = PARM(R_IxNchFilSB2);
     R_CurCmdFil2S.dt   = R_DelTm1;

     // Call the function
     calc_2nd_order(&R_CurCmdFil2S, &R_CurCmdFil2Z);

     // Set the Z-domain variables for Rotor Ix notch filter
     R_IxNchFilZA0 = R_CurCmdFil2Z.a0;
     R_IxNchFilZA1 = R_CurCmdFil2Z.a1;
     R_IxNchFilZA2 = R_CurCmdFil2Z.a2;
     R_IxNchFilZB1 = R_CurCmdFil2Z.b1;
     R_IxNchFilZB2 = R_CurCmdFil2Z.b2;


     // Populate the s-domain structure from final values for Rotor Iy notch filter
     R_CurCmdFil2S.A0   = PARM(R_IyNchFilSA0);
     R_CurCmdFil2S.A1   = PARM(R_IyNchFilSA1);
     R_CurCmdFil2S.A2   = PARM(R_IyNchFilSA2);
     R_CurCmdFil2S.B1   = PARM(R_IyNchFilSB1);
     R_CurCmdFil2S.B2   = PARM(R_IyNchFilSB2);
     R_CurCmdFil2S.dt   = DelTm2;

     // Call the function
     calc_2nd_order(&R_CurCmdFil2S, &R_CurCmdFil2Z);

     // Set the Z-domain variables for Rotor Iy notch filter
     R_IyNchFilZA0 = R_CurCmdFil2Z.a0;
     R_IyNchFilZA1 = R_CurCmdFil2Z.a1;
     R_IyNchFilZA2 = R_CurCmdFil2Z.a2;
     R_IyNchFilZB1 = R_CurCmdFil2Z.b1;
     R_IyNchFilZB2 = R_CurCmdFil2Z.b2;



     ////////////////////////////////
     // IRAM copy population
     ////////////////////////////////
    //A0Adapt.c30

    //A0ANGL.C30

    //A0IXCOND

    R_IxCmd0LimPk    =   PARM(R_IxCmd0Lim)*SQRT2;

    R_IRatSSLimPk    =   PARM(R_IRatSSLim)*SQRT2;

    R_ModLimFbkGn    =   DelTm2 * PARM(R_ModLimFbkW);
    R_ModLimFbGnD    =   DelTm2 * PARM(R_ModLimFbWD);
    R_ModLimFbGn2    =   DelTm2 * PARM(R_ModLimFbW2);
    R_ModLmFbGn2D    =   DelTm2 * PARM(R_ModLmFbW2D);


    R_KPoles         =   PARM(R_PolePairs)*1.5F;
    R_KPolesRcp      =   1.0F/R_KPoles;
    R_PolePrRcp      =   1.0F/PARM(R_PolePairs);

    S_VrfSyncScl  =  PARM(S_VrfSyncScl);
    if ( S_VrfSyncScl > 1.4F )
    {
         S_VrfSyncScl = 1.4F;
    }


    if(PARM(SimModeReq))
    {
        SET_FV(R_CPmpBk,    0.0F);      //Rotor pumpback cap (2*65 kVARs)
    }
    else
    {
        SET_FV(R_CPmpBk,    0.00096);   //Rotor pumpback cap (2*65 kVARs)
    }

    if(SimAcclAct)
    {
        SET_FV(R_AngCorRate,    PI2*R_DelTm1*PARM(R_AngCorAcclSim));    //  increase angle correct rate in acceleration mode
    }
    else
    {
        SET_FV(R_AngCorRate,    PI2*R_DelTm1);
    }

    R_RunRuleCtr = R_RunRuleCtr + 1;

    return (Return_Code); // 1 = success, 0 = fail;

}   // RotorMtrCtlRule()


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rotor generator control setup.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
int RotorMtrCtlSetup(void)
{

    return (SUCCESS);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Special rotor commissioning provisions.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
int RotorCommissionSpecial(void)
{

    return (SUCCESS);
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Default rotor protections.
//
// EXECUTION LEVEL
//      Initialization
//
// RETURN VALUE
//      None
//
///////////////////////////////////////////////////////////////////////////////
void RotorDefaultLineProt(void)
{
    return;
}

