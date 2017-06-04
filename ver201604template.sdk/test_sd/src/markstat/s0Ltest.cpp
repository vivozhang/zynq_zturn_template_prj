/*=============================================================================
* Product:     Innovation Series Control Using DSPX
* Host:        TMS320C31 DSP
* File:        SLTEST.C30
* Language:    TI 'C'
* Copyright:   1996 GE Motors and Industrial Systems, Salem VA
*
* Abstract:  This file implements the inductor test for regen source products.
*
*==============================================================================
* 990104 SAB  Initial create
* 990310 SAB  Increase sample time of voltages and currents for more consistent
*             data.
* 990624 SAB  Change EE.L_Sim after inductor test wizard runs.  Also,
* 990628 SAB  Remove rule set run in state 16
* 000331 SAB  ACCBR-A: Set I_Offs_Vld = false at end of inductance test.
* 000722 PSB  ACDCPCU-F: Set I_Offs_Vld = false at end of inductance test.
* 021119 SAB  Change naming convention: Remove all under-scores except for
*             "L_" on line converter names.
* 030131 SAB  Remove conditional compiles
* 051212 BAG  change CT_MSG_STK_SZ to LT_MSG_STK_SZ
* 071029 SAB  Enhance inductance wizard test
* 071114 SAB  Allow capability for rotor converter to run during inductance test
* 071212 SAB  Make adjustments for implementation of "One pll" scheme
*==============================================================================
* Last Mod: December 12, 2007 3:30:01PM
*/

#include "x0FrameWork.h"
#include "x0MsgStk.h"
#include "Product.h"
#include "s0Ltest.h"



extern float        DelTm2              ;
extern float        GmIyFbkT2           ;
extern float        GmVxFbkT2           ;
extern unsigned     L_IOffsVld          ;
extern float        L_IyFbkT2           ;
extern unsigned     L_LTestCmd          ;
extern float        L_LXfmr             ;
extern unsigned     L_RunReq            ;
extern float        L_VxFbkT2           ;
extern float        R_SpdFbkAvg         ;
extern int          SeqSt               ;
extern SeqActBits   L_SeqAct            ;
extern unsigned     L_SeqReq_PackedBits;
extern SeqStatBits  L_SeqStat           ;
extern unsigned     RPNWRunRdy          ;
extern unsigned     RPNWRunReq          ;
extern unsigned     RunReq              ;

// Parameters
//-----------
CREATE_PARM(L_LTotMeas,         float);     // Total measured  bridge inductance -- not final values
CREATE_PARM(L_LBrgMeas,         float);     //
CREATE_PARM(L_LSysMeas,         float);     //
CREATE_PARM(L_LGridMeas,        float);     //
CREATE_PARM(L_LTransMeas,       float);     //
CREATE_PARM(L_IyLTest,          float);     // Reactive current command for inductor test
CREATE_PARM(L_LTstWtTm,         float);     // Inductance wizard test wait time
CREATE_PARM(L_LTstAvgSmp,       unsigned);  // Inductance wizard test feedback average samples
CREATE_PARM(L_LTstSlwTm,        float);     // Inductance wizard test reactive current slew time
CREATE_PARM(L_LTstSlwThr,       float);     // Inductance wizard test reactive current slew threshold
CREATE_PARM(L_LTstType,         unsigned);  // Indcutance wizard test type (RPNW or both);

// Diagnostics
//------------

// Variables
//----------
/* Cell Test variables */
CREATE_PARM( L_SCRMeas,         float       );      // Measured short circuit ratio
CREATE_PUBVAR(L_GmIyLoad        ,float      );      // Gm_Iy loaded for inductance test
CREATE_PUBVAR(L_GmLTstIyAv      ,float      );      // Gm_Iy average for inductance test
CREATE_PUBVAR(L_GmLTstVxAv      ,float      );      // Gm_Vx average for inductance test
CREATE_PUBVAR(L_GmVxLoad        ,float      );      // Gm_Vx loaded for inductance test
CREATE_PUBVAR(L_IyLoad          ,float      );      // L_Iy loaded for inductance test
CREATE_PUBVAR(L_IyNLoad         ,float      );      // L_Iy no-load for inductance test
CREATE_PUBVAR(L_IyTestRms       ,float      );
CREATE_PUBVAR(L_IyTstCmd        ,float      );      // L_Iy test command
CREATE_PUBVAR(L_LMeasBrg        ,float      );      // Bridge inductance measurement output from test
CREATE_PUBVAR(L_LMeasSys        ,float      );      // System inductance measurement output from test
CREATE_PUBVAR(L_LMeasTstuh      ,float      );      // Inductance measurement output from test in uh
CREATE_PUBVAR(L_LTestTmr        ,float      );      // Inductance test general timer
CREATE_PUBVAR(L_LTstIyAvg       ,float      );      // L_Iy average for inductance test
CREATE_PUBVAR(L_LTstSlwRcp      ,float      );      //Reciprical of EE.CI.L_LTstSlwTm
CREATE_PUBVAR(L_LTstSmpRcp      ,float      );      //Reciprical of EE.CI.L_LTstAvgSmp
CREATE_PUBVAR(L_LTstVxAvg       ,float      );      // L_Vx average for inductance test
CREATE_PUBVAR(L_VxLoad          ,float      );      // L_Vx loaded for inductance test
CREATE_PUBVAR(L_VxNLoad         ,float      );      // L_Vx no-load for inductance test
CREATE_PUBVAR(L_LTestSt         ,int        );      // L_Running cell test state
CREATE_PUBVAR(L_LTTypeSt        ,int        );      // Inductance test type state
CREATE_PUBVAR(L_IndTstDone      ,unsigned   );      // Indctance test is complete - bkg cleared
CREATE_PUBVAR(L_LTestAbort      ,unsigned   );      //Abort inductance test
CREATE_PUBVAR(L_LTestCntr       ,unsigned   );      // Inductance test counter
CREATE_PUBVAR(L_LTestIp         ,unsigned   );      // Inductance test in progress
CREATE_PUBVAR(LBrgTestDone      ,unsigned   );
CREATE_PUBVAR(RBrgTestDone      ,unsigned   );


/*******************************************************************************
* Local constant definitions
*******************************************************************************/


#define  L_TEST_TIME    15




    /* Pulse width calculation constants */


    /* Current and voltage threshold constants */


    /* Current and voltage level output data constants */



    /* Diagnostic message constants */

#define  LT_MSG_STK_SZ   800
//#define  CT_MSG_DLY      10.0
#define  L_TEST_W        0.5

/*******************************************************************************
* Local structure type definitions
*******************************************************************************/


/*******************************************************************************
* Local variable declarations
*******************************************************************************/

    /* Bridge control variables */


    /* State machine control variables */


    /* Evaluation variables */


/*******************************************************************************
* Local function prototypes
*******************************************************************************/
//void  InductanceMeasureMentTest();
//void InductanceMeasurementSequencer();


/******************************************************************************
 * Function:  InductanceMeasurementTest
 * Execution Level: Task 2
 * Execution Time:  Not Measured
 * Formal Inputs:   None
 * Formal Outputs:  None
 *
 * Global Inputs:   L_SeqReq.L_Auto_Req
 *
 *
 * Global Outputs:  L_SeqStat.L_Auto_Stat
 *                  L_SeqAct.L_Auto_Act
 *                  Ind_Test_Done
 *
 *
 * Locals:           L_LTestTmr
 *
 *
 * Return Value:    None
 * Function Calls:  Push2Flt()
 *
 * Abstract:    This function performs the L_Running cell test for the non-regen
 *              source.
 *              The last message given by cell test must be one of the following:
 *              30,31,32,33,34.
 *
 *******************************************************************************/
void InductanceMeasurementTest(void)
{
    float L_IyTestDel;
    float L_LParmuh;



     L_LTTypeSt = (L_SeqReq.L_Test_Req)   |
                  (L_SeqStat.L_LTestStat<<1) |
                  (L_SeqAct.L_Test_Act<<2);

  switch ( L_LTTypeSt)      /* state selection */
  {
      case 0:
      case 3:
          L_LTestSt = 0;        /* do nothing state */
          break;
      case 1:
      case 7:
            if (TripFlt)
            {
                //Push2Flt(Bad_CTR_Tsk);
                L_LTestSt = 0;  /* null state        */
            }
            else
            {
                L_LTestSt = 1;  /* 1st enable state */
            }
          break;
      case 2:
      case 4:                 /* 1st disable state */
          if ( L_LTestSt >= -1)
          {
             L_LTestSt = -1;
          }
          break;
      case 5:                 /* continue with selected state */
            {
                              /* no change */
            }
      case 6:
          break;              /* no change */
      default:
          break;
  }

//==============================================================================
//                           Cell Test Enable States
//==============================================================================

  switch ( L_LTestSt)
  {

      case 0:              /* idle state...do nothing*/
          break;



      case 1:               /* Set up diagnostic message stack */
       L_SeqAct.L_Test_Act  = true;
       L_LTstSlwRcp         = 1.0/PARM(L_LTstSlwTm);
       L_LTestTmr           = 0.0;
       L_LTestAbort         = false;
       L_IyTstCmd           = 0.0;
       LBrgTestDone         = false;
       RBrgTestDone         = false;


       if ( cMessageStack::InitMsgStack( LT_MSG_STK_SZ ) != SUCCESS )
       {
          PUSH_DIAG( MsgStkFail );
           L_LTestSt    = 30;
       }
       else
       {
           cMessageStack::PushMsg( 36, xINT, NULL ); //Bridge test invoked
           cMessageStack::PushMsg( 35, xINT, NULL );
           L_LTestSt    = 10;
       }
       break;

      case 10:                           // Wait for three seconds
          L_LTestTmr = L_LTestTmr + DelTm2;
          if ( L_LTestTmr > PARM(L_LTstWtTm) )
          {
              L_LTestTmr   = 0.0;
              L_LTestSt    = 20;
              L_LTstSmpRcp = 1.0/((float)PARM(L_LTstAvgSmp));
          }
          break;

      case 20:
          if (!LBrgTestDone)
          {
             cMessageStack::PushMsg( 58, xINT, NULL ); //Commanding line converter to run"
             RPNWRunReq = true;
          }
          else
          {
             cMessageStack::PushMsg( 59, xINT, NULL ); //Commanding rotor converter to run"
             RunReq = true;
          }
          cMessageStack::PushMsg( 35, xINT, NULL );
          L_LTestSt  = 30;
          break;

      case 30:                           // Wait for converter to run
          if (!LBrgTestDone)
          {
             if ( SeqSt == CONV_RUNNING_LINE)
             {
                L_LTestSt    = 40;
                L_LTestIp    = true;
             }
          }
          else
          {
             if ( SeqSt == CONV_RUNNING_ROTOR)
             {
                L_LTestSt    = 40;
                L_LTestIp    = true;
             }
          }

          break;

      case 40:                           // Wait another three seconds at no load
          L_LTestTmr = L_LTestTmr + DelTm2;
          if ( L_LTestTmr > PARM(L_LTstWtTm) )
          {
              L_LTestTmr   = 0.0;
              L_LTestSt    = 50;
          }
          break;


      case 50:              /* Case 1: First Bridge Test State  */
                           /* Init timer */
          L_LTestTmr   = 0;
          L_LTestCntr  = 0;
          L_LTstVxAvg  = 0;
          L_LTstIyAvg  = 0;
          L_GmLTstVxAv = 0;
          L_GmLTstIyAv = 0;
          cMessageStack::PushMsg( 37, xINT, NULL ); //Measuring no load voltage and current
          cMessageStack::PushMsg( 35, xINT, NULL );
          L_LTestSt    = 60;
          break;

      case 60:
          L_LTestCntr  = L_LTestCntr  + 1;
          if (!LBrgTestDone)
          {
             L_LTstVxAvg  = L_LTstVxAvg  + L_VxFbkT2;
             L_LTstIyAvg  = L_LTstIyAvg  + L_IyFbkT2;
          }
          else
          {
             L_LTstVxAvg  = L_LTstVxAvg  + GmVxFbkT2;
             L_LTstIyAvg  = L_LTstIyAvg  + GmIyFbkT2;
          }
          if ( L_LTestCntr >= PARM(L_LTstAvgSmp) )
          {
               L_VxNLoad    = L_LTstVxAvg*L_LTstSmpRcp;
               L_LTstVxAvg  = 0.0;
               L_IyNLoad    = L_LTstIyAvg*L_LTstSmpRcp;
               L_LTstIyAvg  = 0.0;


               L_LTestTmr   = 0.0;
               L_LTestCntr  = 0.0;
               if (!LBrgTestDone)
               {
                  L_IyTestRms  = PARM(L_IyLTest);
               }
               else
               {
                  L_IyTestRms  = PARM(VregIyLim)*0.9;
               }
               cMessageStack::PushMsg(46, xFLOAT, &L_IyTestRms, NULL);//Slewing currrent to "L_IyTestRms"
               L_LTestSt    = 70;
          }
          break;



      case 70:                         // Slew current to requested load value
          L_IyTestDel = -L_IyTestRms - L_IyTstCmd;
          if (L_IyTestDel >= DelTm2*(L_IyTestRms*L_LTstSlwRcp))// Slew will take 20.0seconds
          {
             L_IyTestDel = DelTm2*L_IyTestRms*L_LTstSlwRcp;
          }
          if (L_IyTestDel <= -DelTm2*L_IyTestRms*L_LTstSlwRcp)
          {
             L_IyTestDel = -DelTm2*L_IyTestRms*L_LTstSlwRcp;
          }
          L_IyTstCmd = L_IyTstCmd + L_IyTestDel;


          if ( ABS(-L_IyTestRms - L_IyTstCmd) < PARM(L_LTstSlwThr) )
          {
            L_LTestSt = 700;
            cMessageStack::PushMsg( 39, xINT, NULL ); //Measuring load value of voltage and current
            cMessageStack::PushMsg( 35, xINT, NULL );
          }
          break;


      case 700:                         // Wait for three seconds
          L_LTestTmr = L_LTestTmr + DelTm2;
          if ( L_LTestTmr > PARM(L_LTstWtTm) )
          {
              L_LTestTmr = 0.0;
              L_LTestSt = 800;
          }
          break;


      case 800:
          L_LTestCntr  = L_LTestCntr + 1;
          if (!LBrgTestDone)
          {

             L_LTstVxAvg  = L_LTstVxAvg  + L_VxFbkT2;
             L_LTstIyAvg  = L_LTstIyAvg  + L_IyFbkT2;
             L_GmLTstVxAv = L_GmLTstVxAv + GmVxFbkT2;
             L_GmLTstIyAv = L_GmLTstIyAv + GmIyFbkT2;
          }
          else
          {
             L_LTstVxAvg  = L_LTstVxAvg + GmVxFbkT2;
             L_LTstIyAvg  = L_LTstIyAvg + GmIyFbkT2;
          }


          if ( L_LTestCntr >= PARM(L_LTstAvgSmp) )
          {
               L_VxLoad      = L_LTstVxAvg*L_LTstSmpRcp;
               L_LTstVxAvg   = 0.0;

               L_IyLoad      = L_LTstIyAvg*L_LTstSmpRcp;
               L_LTstIyAvg   = 0.0;

               if (!LBrgTestDone)
               {
                  L_GmVxLoad    = L_GmLTstVxAv*L_LTstSmpRcp;
                  L_GmLTstVxAv  = 0.0;
                  L_GmIyLoad    = L_GmLTstIyAv*L_LTstSmpRcp;
                  L_GmLTstIyAv  = 0.0;
               }

               L_LTestTmr   = 0.0;
               L_LTestCntr  = 0.0;
               cMessageStack::PushMsg( 40, xINT, NULL ); //Slewing reactive current back to zero
               cMessageStack::PushMsg( 43, xINT, NULL ); //and calculating inductance
               cMessageStack::PushMsg( 35, xINT, NULL );

               L_LTestSt    = 900;
          }
          break;

      case 900:                           // Slew current to no load (zero)
          L_IyTestDel = 0 - L_IyTstCmd;
          if (L_IyTestDel >= DelTm2*(L_IyTestRms*2.0*L_LTstSlwRcp))// Slew will take 10.0seconds
          {
             L_IyTestDel = DelTm2*L_IyTestRms*2.0*L_LTstSlwRcp;
          }
          if (L_IyTestDel <= -DelTm2*L_IyTestRms*2.0*L_LTstSlwRcp)
          {
             L_IyTestDel = -DelTm2*L_IyTestRms*2.0*L_LTstSlwRcp;
          }
          L_IyTstCmd = L_IyTstCmd + L_IyTestDel;

          if ( ABS(L_IyTstCmd) <= 0.25)
          {
            L_IyTstCmd = 0.0;
            L_LTestSt   = 1000;
          }

          break;


      case 1000:                        // Calculate and display bridge inductance
          if (!LBrgTestDone)
          {
             L_LMeasBrg = ABS((L_GmVxLoad - L_VxLoad)/(PARM(L_LineFrq*L_IyLoad)));
             L_LMeasTstuh = 1.0e6*L_LMeasBrg;
             cMessageStack::PushMsg(45, xFLOAT, &L_LMeasTstuh, NULL);  // Total Measured bridge inductance is "L_LMeasBrg"
             LBrgTestDone = true;
             L_LTestTmr = 0.0;
             L_LTestSt   = 1100;
          }
          else
          {
              L_LTestSt   = 2000;
              RBrgTestDone = true;
          }

          break;

      case 1100:                           // Wait for 3.0 seconds
          L_LTestTmr = L_LTestTmr + DelTm2;
          if ( L_LTestTmr > PARM(L_LTstWtTm) )
          {
              L_LTestTmr = 0.0;
              L_LTestSt  = 1200;

          }
          break;


      case 1200:       // LOOP BACK FROM HERE IF ROTOR TEST NOT DONE
          if ( (PARM(L_LTstType) == L_TEST_BOTH) && (R_SpdFbkAvg > (PARM(R_RPMMinOp) + 50.0) ) )
          {
              L_LTestSt  = 10; //loop back to run rotor converter
              L_LTestTmr = 0.0;
          }
          else // Calculate and display inductance at transformer secondary
          {
              L_LTestSt   = 2000;
          }
          break;


      case 2000: //L_LMeasSys is from grid monitor signals if both convertes ran, other-wise is from L_Vx, L_Vy, etc
          L_LMeasSys = ABS((L_VxLoad - L_VxNLoad)/(PARM(L_LineFrq)*(L_IyLoad - L_IyNLoad)));
          L_LMeasTstuh = 1.0e6*L_LMeasSys;
          if (RBrgTestDone )
          {
             cMessageStack::PushMsg(42, xFLOAT, &L_LMeasTstuh, NULL);  // Indcutance at transfomrer secondary is "L_LMeasSys"
          }
          else
          {
             cMessageStack::PushMsg(44, xFLOAT, &L_LMeasTstuh, NULL);  // Total measured inductance is: "L_LMeasSys"
          }
          L_LTestTmr = 0.0;
          L_LTestSt   = 2200;
          break;


      case 2200:                           // Calculate and display inductance
          L_LParmuh = 1.0e6*L_LMeasBrg;
          //PushMsg(44, xFLOAT, &L_LParmuh, NULL); //Total inductance last measured
          L_LTestSt   = 2210;
          break;

      case 2210:
          cMessageStack::PushMsg( 57, xINT, NULL ); //Stopping rotor converter
          cMessageStack::PushMsg( 35, xINT, NULL );    // Blank line
          RunReq = false;
          L_LTestTmr = 0.0;
          L_LTestSt = 2225;
          break;


      case 2225:
          if ( SeqSt == CONV_RUNNING_LINE )
          {
             L_LTestSt = 2250;
          }
          break;

      case 2250:                         // Wait for three seconds
          L_LTestTmr = L_LTestTmr + DelTm2;
          if ( L_LTestTmr > PARM(L_LTstWtTm) )
          {
              L_LTestTmr = 0.0;
              L_LTestSt = 2300;
          }
          break;



      case 2300:
          cMessageStack::PushMsg( 47, xINT, NULL ); //Stopping Line converter
          cMessageStack::PushMsg( 35, xINT, NULL );    // Blank line
          //L_RunReq = false;
          //L_InnrRegEnb = false;
          RPNWRunReq = false;
          L_LTestSt = 2400;
          break;

      case 2400:                           // Wait for 1.0 seconds
          L_LTestTmr = L_LTestTmr + DelTm2;
          if ( L_LTestTmr > 1.0)
          {
              L_LTestTmr = 0.0;
              L_LTestSt  = 2500;
          }
          break;


        case 2500:                           // Set L_LMeas =
            cMessageStack::PushMsg( 48, xINT, NULL );    // Setting 'Measured inductance' equal
            cMessageStack::PushMsg( 49, xINT, NULL );    // to test results
            cMessageStack::PushMsg( 35, xINT, NULL );    //
            if (RBrgTestDone ) //The rotor converter ran
            {
                PARM(L_LBrgMeas) = L_LMeasBrg;
                PARM(L_LSysMeas) = L_LMeasSys;
                PARM(L_LTotMeas) = L_LMeasSys + L_LMeasBrg;
            }
            else  //Only the line converter ran
            {
                PARM(L_LBrgMeas) = L_LMeasBrg;
                PARM(L_LTotMeas) = L_LMeasSys;
                PARM(L_LSysMeas) = PARM(L_LTotMeas) - L_LMeasBrg;
            }


            if ( L_LXfmr > PARM(L_LSysMeas) )
            {
                PARM(L_LTransMeas) = PARM(L_LSysMeas);
            }
            else
            {
                PARM(L_LTransMeas) = L_LXfmr;
            }

            if (PARM(L_LTotMeas) - (PARM(L_LBrgMeas) + PARM(L_LTransMeas)) < 0.0)
            {
                PARM(L_LGridMeas) = 0.0;
            }
            else
            {
                PARM(L_LGridMeas) = PARM(L_LTotMeas) - (PARM(L_LBrgMeas) + PARM(L_LTransMeas));
            }
            PARM(L_SCRMeas) = NoNANLim(PARM(SysLBase)/PARM(L_LGridMeas), 1.0e6F, -1.0e6F);

            L_LTestSt = 2600;
            break;


      case 2600:
          //PushMsg( 50, xINT, NULL );    // L_Running rules (from wind-sync)
          //PushMsg( 35, xINT, NULL );    // Blank line
          L_IndTstDone = true;
          L_LTestSt = 2700;
          break;

      case 2700:                           // Wait for 5.0seconds
          L_LTestTmr = L_LTestTmr + DelTm2;
          if ( L_LTestTmr > 5.0)
          {
              L_LTestTmr = 0.0;
              L_LTestSt = 2800;
          }
          break;


      case 2800:
          cMessageStack::PushMsg( 30, xINT, NULL );    // Completed successfully
          L_LTestTmr = 0.0;
          L_LTestSt = 2900;
          break;

      case 2900:
          L_LTestTmr = L_LTestTmr + DelTm2;
          if ( L_LTestTmr > PARM(L_LTstWtTm) )
          {
             L_LTestTmr = 0.0;
             L_LTestSt = 3000;
          }
          break;


      case 3000:
          if ( cMessageStack::FreeMsgStack() != SUCCESS )
          PUSH_DIAG( MsgStkFail );
          L_SeqStat.L_LTestStat = true;
          L_SeqAct.L_Test_Act   = false;
          L_RunReq              = false;
          break;


      case -1:             /* Case -1: First L_Running Cell Test Disable State */
         L_LTestTmr        = 0.0;
         L_LTestCntr       = 0;
         L_LTestSt         =-2;
         L_LTestIp         = false;
         if ( TripFlt)
         {
             L_LTestCmd = false;
             RPNWRunReq = false;
             RunReq = false;

         }
         break;


      case -2:
         L_LTestTmr = L_LTestTmr + DelTm2;
         if ( L_LTestTmr >= .10)
         {
            L_LTestSt             =-3;
            L_LTestTmr = 0.0;
         }
         break;

      case -3:             /* Case -3: Last L_Running Cell Test Disable State*/
         if ( cMessageStack::FreeMsgStack() != SUCCESS )
         PUSH_DIAG( MsgStkFail );
         L_SeqAct.L_Test_Act    = false;
         L_SeqStat.L_LTestStat  = false;
         L_IOffsVld            = false;
         break;
     default:
         break;

      }

}


/******************************************************************************
 * Function:  InductanceMeasurementSequencer()
 * Execution Level: Task 2
 * Execution Time:  Not Measured
 * Formal Inputs:   None
 * Formal Outputs:  None
 *
 * Global Inputs:   L_SeqReq.L_Auto_Req
 *
 *
 * Global Outputs:  L_SeqStat.L_Auto_Stat
 *                  L_SeqAct.L_Auto_Act
 *                  Ind_Test_Done
 *
 *
 * Locals:           L_LTestTmr
 *
 *
 * Return Value:    None
 * Function Calls:  Push2Flt()
 *
 * Abstract:    This function performs the L_Running cell test for the non-regen
 *              source.
 *
 *******************************************************************************/
void InductanceMeasurementSequencer(void)
{

    InductanceMeasurementTest();


    if ( L_SeqStat.L_LTestStat || !L_LTestCmd || TripFlt || !RPNWRunRdy)
    {
        L_SeqReq.L_Test_Req = false;
        L_LTestCmd          = false;
    }
    else
    {
        L_SeqReq.L_Test_Req = true;
    }

}
