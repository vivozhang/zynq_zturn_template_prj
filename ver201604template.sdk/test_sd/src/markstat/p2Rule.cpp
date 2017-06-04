///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Line side product rules
//
// DESCRIPTION:
//      This file contains rule calculations for line control final values
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
//framework
#include "x0FrameWork.h"
// product
#include "Product.h"

// Constants
//----------
#define DEFAULTSEQUENCER    62
#define DEFAULTIYREF        63     // not used

// Externals
//----------
extern float    L_VlnIn;
extern unsigned SimAcclAct;
// Types and Classes
//------------------

// Published Variables
//--------------------

// Unpublished Variables
//----------------------

// Local Prototypes (to resolve forward references)
//-------------------------------------------------
void    LineSequencerAddrSetup(void);
void    LineIyReferenceSetup(void);



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Select set of rules to run
//
// EXECUTION LEVEL
//      Rule execution
//
// RETURN VALUE
//      Status
//
///////////////////////////////////////////////////////////////////////////////
int RunLineAILRules(int Rule_Set)
{
    int     Status=SUCCESS;

    switch(Rule_Set)
    {

        case RULE_SET_MAIN:
              LineIyReferenceSetup();
            break;

        // set motor control parameters based on final values
        case RULE_SET_DEFAULT_ALL:
              LineSequencerAddrSetup();
            break;


        // Set Default Sequencer Items
        case DEFAULTSEQUENCER:
              LineSequencerAddrSetup();
            break;



       // Default case
       default:
            break;
    }

    return (Status);
}   // RunLineRuleSet()


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rules for sequencer parameters
//
// EXECUTION LEVEL
//      Rule execution
//
// RETURN VALUE
//      None
//
//////////////////////////////////////////////////////////////////////////////
void LineSequencerAddrSetup(void)
{
    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Rules for reactive current reference function
//
// EXECUTION LEVEL
//      Rule execution
//
// RETURN VALUE
//      None
//
//////////////////////////////////////////////////////////////////////////////
void LineIyReferenceSetup(void)
{


      SET_FV(L_IyrfIStpt, 0.0);
      SET_FV(L_IyrfVStpt, 0.0);
      SET_FV(L_IySlwRat, 10.0e6); // Y current slew rate
      SET_FV(L_VRegOnly, false);
      SET_FV(L_WIyLim, 100.0);
      SET_FV(L_UnityPF, 1); //Source unit pf
      SET_FV(L_DrvSupply, true); //Drive supply yes
      SET_FV(L_VReduction, 1); //Voltage reduction is true
      SET_FV(L_IyrfIAdj, 0.0);
      SET_FV(L_IyrfCmdGn, 0.0);
      SET_FV(L_IyrfFbkGn, 0.0);
      SET_FV(L_IyrfFbkScl, 1.0);
      SET_FV(L_IyrfCmdScl, 1.0);
      SET_FV(L_IyrfIMax, PARM(L_MaxCur));




      //Reactive Current Generation EE calculations.
      if ( PARM(L_IyrfIMax)*SQRT2 >= 2*PARM(L_ShuntRat))
      {
           SET_FV(L_IyrfIMax, 2*PARM(L_ShuntRat)/SQRT2);
      }


      if ( PARM(L_DrvSupply) && PARM(L_UnityPF))
      {
          SET_FV(L_IyrfCmdAdr, (unsigned * ) LADR_LineIyPFCmd);
          SET_FV(L_IyrfFbkAdr, (float *) LADR_L_IyrfIyT2);

          SET_FV(L_IyrfIntGn, 1000.0);

          SET_FV(L_IyrfFbkScl, 1.0);
          SET_FV(L_IyrfCmdScl, 1.0);

          SET_FV(L_IyrfIMax, 1.5*PARM(L_MaxCur));
          SET_FV(L_IyrfIntPlm, 1.5*PARM(L_MaxCur));
          SET_FV(L_IyrfIntNlm,  -1.5*PARM(L_MaxCur));
          SET_FV(L_IyrfMaxOut, PARM(L_IyrfIMax));
          SET_FV(L_IyrfMinOut, -PARM(L_IyrfIMax));

      }
      else if (PARM(L_VRegOnly))
      {
          SET_FV(L_IyrfFbkScl, PARM(L_IyrfCmdScl));
          SET_FV(L_IyrfFbkAdr, (float *) LADR_GmVMagT2);

          if ( PARM(L_VReduction))
          {
             SET_FV(L_IyrfMaxOut, PARM(L_IyrfIMax));
             SET_FV(L_IyrfMinOut, -PARM(L_IyrfIMax));
             SET_FV(L_IyrfIntPlm, PARM(L_IyrfIMax));
             SET_FV(L_IyrfIntNlm, -PARM(L_IyrfIMax));
          }
          else
          {
             SET_FV(L_IyrfMaxOut, PARM(L_IyrfIMax));
             SET_FV(L_IyrfMinOut, 0.0);
             SET_FV(L_IyrfIntPlm, PARM(L_IyrfIMax));
             SET_FV(L_IyrfIntNlm, 0.0);
          }
      }
      else
      {
          SET_FV(L_IyrfMaxOut, PARM(L_IyrfIMax));
          SET_FV(L_IyrfMinOut, -PARM(L_IyrfIMax));
          SET_FV(L_IyrfIntPlm,  PARM(L_IyrfIMax));
          SET_FV(L_IyrfIntNlm, -PARM(L_IyrfIMax));
      }

    SET_FV(L_PlsTstVmagMax,     0.1F*PARM(L_VllInput));
    if(SimAcclAct)
    {
        SET_FV(L_DbAutoTstInitTm,   0.0F);
    }
    else
    {
        SET_FV(L_DbAutoTstInitTm,   2.F);           // L_DBAPsIniTm in 1.6e
    }
    SET_FV(L_Q1OpnTvMin,        0.5F*L_VlnIn);  // L_MAOpnTVMin
    return;
}
