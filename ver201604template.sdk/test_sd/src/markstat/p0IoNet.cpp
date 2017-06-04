///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       IONet Interface
//
// DESCRIPTION:
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
// product
#include "Product.h"

#include "b0DbThrm.h"

// Constants
//----------


// Externals
//----------
extern float        CoolPwrIn       ;
extern float        GmPllWT2        ;
extern float        GmPwr           ;
extern float        GmQPwr          ;
extern float        GmSPwr          ;
extern float        GmPwrGrid       ;
extern float        GmQPwrGrid      ;
extern float        GmSPwrGrid      ;

extern float        GmTrqFbkFil     ;

extern float        L_PwrLine       ;
extern float        L_VarHrmF       ;
extern float        L_VarLine       ;

extern float        PhiCmd          ;
extern float        PhiRefRem       ;
extern float        QregCmd         ;

extern float        R_CnvOutPwr     ;
extern float        R_TrqCmdInT2    ;
extern float        R_TrqFbk        ;

extern cDbThrmModel L_DbThrmModel  ;   // for L_DtmPdisspt


extern float        S_PllWT2        ;
extern float        S_Pwr           ;
extern float        S_QPwr          ;

extern float        TrqRefRem       ;
extern float        VARCmd          ;
extern float        VarRefRem       ;
extern float        PwrLimDelTorque     ;

extern unsigned     S_K1Status      ;
extern unsigned     L_OTAlSum       ;
extern unsigned     R_OTAlSum       ;
extern unsigned     R_SyncSwHwSt    ;
extern unsigned     RPNWReqEgd      ;
extern unsigned     RstReqEgd       ;
extern unsigned     RunReqEgd       ;
extern unsigned     SimMode         ;
extern unsigned     VARModeCmd      ;
extern unsigned     VarReqEgd       ;

extern unsigned     RunCmd          ;
extern unsigned     RPNWRunReq      ;
extern unsigned     SysTmSec;
extern unsigned     Running         ;
extern void         DriveTrainDamperBgnd();
extern int          RunRuleSet(int Rule_Set);

// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(PrmPssTmrThr            , float    );   // Param HandShake Timeout Threshold

// Diagnostics
//------------
CREATE_DIAG(PrmPssHdsFlt);                          // Param HandShake Timeout fault

// Variables
//----------
CREATE_PUBVAR(PhiCmd                , float    );

CREATE_PUBVAR(TrqRefRem     		, float    );
CREATE_PUBVAR(VarRefRem     		, float    );

CREATE_PUBVAR(PLCParmReq            , unsigned );   // Turbine control has updated parameters.
CREATE_PUBVAR(CCUParmCnfrm          , unsigned );   // Converter control unit has processed parameters

CREATE_PUBVAR( CoolPwrIn_kW         , float    );

CREATE_PUBVAR( GmPllW_Hz            , float    );


CREATE_PUBVAR( GmPwr_kW             , float    );
CREATE_PUBVAR( GmQPwr_kVr           , float    );
CREATE_PUBVAR( GmSPwr_kVA           , float    );
CREATE_PUBVAR( GmTrqFbkFil_kNm      , float    );

CREATE_PUBVAR( L_PwrLine_kW         , float    );
CREATE_PUBVAR( L_VarHrmF_kVr        , float    );
CREATE_PUBVAR( L_VarLine_kVr        , float    );

CREATE_PUBVAR( PhiCmd_deg           , float    );
CREATE_PUBVAR( QregCmd_kVr          , float    );

CREATE_PUBVAR( R_CnvOutPwr_kW       , float    );
CREATE_PUBVAR( R_TrqCmdIn_kNm       , float    );
CREATE_PUBVAR( R_TrqFbk_kNm         , float    );

CREATE_PUBVAR( R_DtmPdisspt_kW      , float    );


CREATE_PUBVAR( S_PllW_Hz            , float    );
CREATE_PUBVAR( S_Pwr_kW             , float    );
CREATE_PUBVAR( S_QPwr_kVr           , float    );

CREATE_PUBVAR( TrqRefRem_kNm        , float    );
CREATE_PUBVAR( VARCmd_kVr           , float    );
CREATE_PUBVAR( VarRefRem_kVr        , float    );
CREATE_PUBVAR( WBDelSpdPwr_kW       , float    );

CREATE_PUBVAR( RPNWSupportd         , unsigned );
CREATE_PUBVAR( SyncSwOn             , unsigned );
CREATE_PUBVAR( VarRegSupptd         , unsigned );

CREATE_PUBVAR( RunningNoTrip        , unsigned );

// Turbine Control and Converter Parameter Handshake Variables
CREATE_PUBVAR( CCUParmRdy           , unsigned );  // Converter control unit parameters status have been filled
CREATE_PUBVAR( PrmPssFlg            , unsigned );  // Drive train damper param process flag
CREATE_PUBVAR( PrmPssHdsTmr         , unsigned );  // Drive Drian Tamper param handshake timer


// Unpublished Variables
//----------------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------





///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void IoNetOutBuf ( void )
{

    CoolPwrIn_kW    = CoolPwrIn     *  0.001F;

    GmPllW_Hz       = GmPllWT2      *  RAD_TO_HZ;


    // 690 V winding only
    GmPwr_kW        = GmPwr         *  0.001F;
    GmQPwr_kVr      = GmQPwr        *  0.001F;
    GmSPwr_kVA      = GmSPwr        *  0.001F;

    GmTrqFbkFil_kNm = GmTrqFbkFil   *  0.001F;

    L_PwrLine_kW    = L_PwrLine     *  0.001F;
    L_VarHrmF_kVr   = L_VarHrmF     *  0.001F;
    L_VarLine_kVr   = L_VarLine     *  0.001F;

    PhiCmd_deg      = PhiCmd        *  RAD_TO_DEG;
    QregCmd_kVr     = QregCmd       *  0.001F;

    R_CnvOutPwr_kW  = R_CnvOutPwr   *  0.001F;
    R_TrqCmdIn_kNm  = R_TrqCmdInT2  * -0.001F;
    R_TrqFbk_kNm    = R_TrqFbk      *  0.001F;

    R_DtmPdisspt_kW = L_DbThrmModel.DtmPdisspt * 0.001F;


    S_PllW_Hz       = S_PllWT2      *  RAD_TO_HZ;
    S_Pwr_kW        = S_Pwr         *  0.001F;
    S_QPwr_kVr      = S_QPwr        *  0.001F;

    VARCmd_kVr      = VARCmd        *  0.001F;

    //DelSpdPwr_kW  = DelSpdPwrK;

    if ( SimMode )                                          // synchronizing switch status
    {
        SyncSwOn = S_K1Status;                                // S_SynSwClsd is very close to R_SCCLSFbk
    }
    else                                                    // except it is "faked" in sim mode.
    {
        SyncSwOn = R_SyncSwHwSt;                            // Normally use hardware input with minimal logic
    }

    VarRegSupptd = true;                                    // var ref supported
    RPNWSupportd = VARModeCmd;                              // Status for RPNW support
    RunningNoTrip = Running && !TripFlt;

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void IoNetInBuf ( void )
{

    TrqRefRem   = TrqRefRem_kNm;
    VarRefRem   = VarRefRem_kVr;

    return;
}



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//
//
// EXECUTION LEVEL
//      Task2
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void BppbIoNetIO ( void )
{

    IoNetInBuf();
    IoNetOutBuf();

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION
//   Performs parameter pass and handshake with turbine control via Ionet.
//
// EXECUTION LEVEL
//      Background
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
 void ParamPassHandShake(void)
 {
    if ((!RunCmd && !RPNWRunReq)  || PrmPssFlg)
   {
     // Local Mode
     if (PARM(Net_Type) == NET_ABSENT)
     {
        DriveTrainDamperBgnd();
        CCUParmRdy = true;
     }
     //Turbine Control Mode
     else
     {
        if (PLCParmReq)
        {
           if (PrmPssFlg)
           {
             if (SysTmSec >= (PrmPssHdsTmr + PARM(PrmPssTmrThr)))
             {
               PUSH_DIAG(PrmPssHdsFlt);
             }
           }
           else
           {
              PrmPssHdsTmr = SysTmSec;
           }
           DriveTrainDamperBgnd();
           CCUParmRdy = true;
           CCUParmCnfrm = true;
           PrmPssFlg = true;
        }
        else if (PrmPssFlg)
        {
           CCUParmCnfrm = false;
           PrmPssFlg = false;
           PrmPssHdsTmr = 0;
           RunRuleSet(RULE_SET_MAIN);    // note this is for rules driven by PLCPwrNP from TC
        }
     }
   }
   return;
 }


