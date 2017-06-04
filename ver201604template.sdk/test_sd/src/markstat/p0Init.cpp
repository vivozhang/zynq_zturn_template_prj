///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Product initializaion
//
// DESCRIPTION:
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//      List Visual SourceSafe locations of pertinent design documents.
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
#include "p0Sseq.h"
#include "b0BridgeRebf.h"
#include "p0EsaBuf.h"


// Constants
//----------


// Externals
//----------
extern float                FpllFrqAdj;

extern unsigned             Stopped;

extern unsigned             L_Running;
extern unsigned             R_Running;

extern unsigned             L_RunRdy;
extern unsigned             R_RunRdy;

extern unsigned             StoppedB1;
extern unsigned             StoppedB2;

extern unsigned             L_BrgPwrEnb;
extern unsigned             R_BrgPwrEnb;

extern cSequencerBase*      pSequencer;         // defined by MarkStat framework
extern cCoreCommonAdlSeq    CoreCommonAdlSeq;

extern cBridgeRebf          L_BridgeRebf;
extern cBridgeRebf          R_BridgeRebf;
extern cEsaBuf              EsaBuf;

extern PhsTransListTyp      R_PhsTransListBrg1;

extern int  SimInit(void);
extern void GridMonitorFbksInit();
extern void InitBIL();
extern void InitLCL();
extern void ProductFrameworkInit(ProductStatusAddrTyp ProductStatusAddr);
extern void RotorInitLCL();
extern void Task1_Init_A();
extern void Task1_Init_B();


// Types and Classes
//------------------


// Function Prototypes
//--------------------

// Diagnostics
//------------


// Parameters
//-----------


// Published Variables
//--------------------


// Unpublished Variables
//----------------------


// Data Passing
//-------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Class Instances
//----------------
PhsTransListTyp PhsTransDisabList;      // Phase Transition DISABLE List
                                        //     Length of Phase Transition Array,
                                        //     Pointer to Phase Transition Array,
                                        //     Pointer to Bridge Test Ctrl Struc.
                                        // Phase Transition DISABLE Array
PhsTransTyp     PhsTransDisabArr[REBx_NUM_OF_TRA_PER_PHS][NUM_OF_PHASES];
                                        //     Transition Number (0=initial)
                                        //     Phase Number



/******************************************************************************
 * Function:        Task1_Init
 * Execution Level: Init
 * Execution Time:  Not measured
 *
 * Formal Inputs:
 * Formal Outputs:
 *
 * Global Inputs:
 * Global Outputs:
 *
 * Return Value:
 * Function Calls:
 *
 * Abstract:        This function initializes Task 1
 *******************************************************************************/
 void Task1_Init(void)
 {
    // InitializeT1FaultBuffer();

    FpllFrqAdj = 1.0 ;

    PhsTransDisabList.Len        = REBx_NUM_OF_TRA_PER_PHS;
    PhsTransDisabList.pTrans     = &PhsTransDisabArr[0];    // Ptr-->Ptr-->1st element
    PhsTransDisabList.pBrgTstCtl = NULL;                    // Null Pointer

    for ( unsigned phs = 0 ; phs < NUM_OF_PHASES ; phs++ )
    {
        for ( unsigned tra = 0 ; tra < (PhsTransDisabList.Len + 1) ; tra++ )
        {
            PhsTransDisabArr[tra][phs].DelTm  = 0;
            PhsTransDisabArr[tra][phs].State  = PHS_ST_OFF;
            PhsTransDisabArr[tra][phs].Enable = (tra == 0) ? true : false;
        }
    }



    // DspVCOCfg.HiFi45_Mode = 0;   /* 0=>VCO up ctrs, 1=> VCO up/dn */
    // DspVCOCfg.Tach4_Cpt   = 1;   /* 0=> Clk T2, 1=> Clk T1 */
    // *(DSPTachCfg *)DSP_HCF_ADDR = DspVCOCfg;   /* write Tach config to hardware */

 }





/******************************************************************************
 * Function:        Task2_Init
 * Execution Level: Init
 * Execution Time:  Not measured
 *
 * Formal Inputs:
 * Formal Outputs:
 *
 * Global Inputs:
 *
 * Global Outputs:
 *
 * Return Value:
 * Function Calls: T2DataInit
 *
 * Abstract:        This function initializes Task 2
  *******************************************************************************/
void            Task2_Init(void)
 {
    // T2DataInit();
    // InitTmPLL();

#if ISBUS_ENABLE
    SetNetType();
    if (EE.CI.Net_Type == ACL_DPM || EE.CI.Net_Type == DRIVE_NETS) {
        memset((void *) &BLIG, 0, sizeof BLIG);
        BLIG.init = 1;
    }
    InitFPLL(); // Frame PhaseLock Loop Initialization
#endif // end of ISBUS_ENABLE

 }


/******************************************************************************
 * Function:        Task3_Init
 * Execution Level: Init
 * Execution Time:  Not measured
 *
 * Formal Inputs:
 * Formal Outputs:
 *
 * Global Inputs:
 * Global Outputs:  T3CalcDone
 *
 * Return Value:
 * Function Calls:
 *
 * Abstract:        This function initializes Task 3
 *******************************************************************************/
 void Task3_Init()
    {
        // BufC_Typ* Temp   = BufCOrg;
        // BufC2_Typ* Temp2 = BufC2Org;
        //int i;

        // REDLED = 0;
        // GREENLED = 0;

        // FanStateTmr = PARM(FanMinOffTm);     // moved to p0Rule2.CommonAILRule

        // T3CalcDone = 0;
        // T3DoneOnce = 0;

        // T3DataInit();


    }


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Product specific initialization
//
// EXECUTION LEVEL
//      Main, Initialization
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ProductInit(void)
{
    ProductStatusAddrTyp ProductStatusAddr = { &False, &False, &False, &False, &False, &False, &False, &False, &False };

    //-------------------------------------------------------------------------
    // open the bridge interfaces
    L_BridgeRebf.Open( HSSL_NUM_L, 1, HSSL_NUM_L );
    R_BridgeRebf.Open( HSSL_NUM_R, 1, HSSL_NUM_R );

    //-------------------------------------------------------------------------
    // initialize the framework pointers to the product level status signals
    ProductStatusAddr.Stopped     = &Stopped;

    ProductStatusAddr.Brg1Ready   = &L_RunRdy;
    ProductStatusAddr.Brg1Enabled = &L_BrgPwrEnb;
    ProductStatusAddr.Brg1Running = &L_Running;
    ProductStatusAddr.Brg1Stopped = &StoppedB1;

    ProductStatusAddr.Brg2Ready   = &R_RunRdy;
    ProductStatusAddr.Brg2Enabled = &R_BrgPwrEnb;
    ProductStatusAddr.Brg2Running = &R_Running;
    ProductStatusAddr.Brg2Stopped = &StoppedB2;

    ProductFrameworkInit(ProductStatusAddr);

    //-------------------------------------------------------------------------
    // pick the sequencer to be used by Framework
    pSequencer = &CoreCommonAdlSeq;

    //-------------------------------------------------------------------------
    // stuff from MACC x0Init.Background()

    // InitFnlVal();             // Final Values Initialization
    {
        // ProductInit();       //      Misc(product-specific)
        InitBIL();
        RotorInitLCL();
        InitLCL();
    }
    //InitMainSequencer();        // Consider moving this after merge of Rotor code
    {
        // CommonInit();         //      Misc(common)
    }
    // InitAdlServer();            // Asynchronous Drive Language Server Initialization
    // Xops_Config();            //      XOPS Operating System
    Task1_Init();               //      Task1-specific Functions
    Task1_Init_A();             //      Task1-specific Functions--Bridge A
    Task1_Init_B();             //      Task1-specific Functions--Bridge B
    Task2_Init();               //      Task2-specific Functions
    Task3_Init();               //      Task3-specific Functions

    SimInit();

    EsaBuf.Enable();            // start ESA buffer

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Product specific process shutdown tasks
//
// EXECUTION LEVEL
//      Main, shutdown
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void ProductShutdown(void)
{

    L_BridgeRebf.Close();
    R_BridgeRebf.Close();

    return;
}

