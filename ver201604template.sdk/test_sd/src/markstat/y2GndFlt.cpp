///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       Line ground current fault detection
//
// DESCRIPTION:
//      This file contains functions which pertain to phase current fault
//      checking.
//
// COPYRIGHT:   Copyright (c) 2013
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//     y2GndFlt.vsd, y2GndFlt.xls
//
///////////////////////////////////////////////////////////////////////////////

// Include Files
//--------------
// system
// framework
#include "x0FrameWork.h"
#include "x0Vector.h"
// product
#include "Product.h"
#include "b0CelTst.h"


// Externals
//----------
extern float        DelTm3;
extern float        L_Ia;
extern float        L_Ib;
extern float        L_Ic;
extern float        L_IxGndT3;
extern float        L_IyGndT3;
extern unsigned     InitDone;
extern unsigned     L_IOffsVld;
extern unsigned     Q1OpnReq;

extern SinCosTyp    GmSinCosFbk;
extern cCellTest    L_CellTest;


// Constants
//----------


// Types and Classes
//------------------


// Parameters
//-----------
CREATE_PARM(L_WIabcDcGnd,    float);
CREATE_PARM(L_IabcDcFltThr,  float);
CREATE_PARM(L_GndAlmOff,     float);     // Discriminated ground alarm off level
CREATE_PARM(L_GndAlmOn,      float);     // Discriminated ground alarm on level
CREATE_PARM(L_GndFltThrs,    float);     // Discriminated ground fault trip threshold
CREATE_PARM(L_IabcFltThr,    float);  // Fast ground fault trip threshold
CREATE_PARM(L_WIabcGnd,      float);     // ABC ground current filter omega
CREATE_PARM(L_WIxIyGnd,      float);     // L_Ix & L_Iy ground current filter omega
CREATE_PARM(L_WIxyGnd,       float);     // XY ground current filter omega

// Diagnostics
//------------
CREATE_DIAG(L_IabcDcGndFlt);
CREATE_DIAG(L_InvGndAlm);               // Discriminated inverter ground warning
CREATE_DIAG(L_InvGndFlt);               // Discriminated inverter ground fault
CREATE_DIAG(L_IabcGndFlt);

// Variables
//----------
CREATE_PUBVAR(L_IabcGnd     ,float);
CREATE_PUBVAR(L_IabcGndFil  ,float);
CREATE_PUBVAR(L_IabcGndGn   ,float);
CREATE_PUBVAR(L_IxGndFil    ,float);
CREATE_PUBVAR(L_IxGndT1     ,float);
CREATE_PUBVAR(L_IxIyGndGn   ,float);
CREATE_PUBVAR(L_IxyGnd      ,float);
CREATE_PUBVAR(L_IxyGndFil   ,float);
CREATE_PUBVAR(L_IxyGndGn    ,float);
CREATE_PUBVAR(L_IyGndFil    ,float);
CREATE_PUBVAR(L_IyGndT1     ,float);
CREATE_PUBVAR(L_IabcGndT3   ,float);
CREATE_PUBVAR(L_IabcDcGndFil,float);
CREATE_PUBVAR(L_IOffsVldT3  ,unsigned);


// Unpublished Variables
//----------------------


// Local Prototypes (to resolve forward references)
//-------------------------------------------------


// Data Passing
//--------------
DATA_PASS(L_IabcGnd,  L_IabcGndT3,  T1_T3,   FILTERED );
DATA_PASS(L_IOffsVld, L_IOffsVldT3, T2_T3, UNFILTERED );


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Excessive ground current detection based on A-B-C quantities.
//      Discriminates the summed phase currents into X and Y components.
//
// EXECUTION LEVEL
//      Task1
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void InvGndFltDetectT1( void )
{
    if ( InitDone )
    {
            // Calculate ground current based on phase currents

        L_IabcGnd = L_Ia + L_Ib + L_Ic;

            // Filter ground current and test against trip level

        L_IabcGndFil += L_IabcGndGn * ( ABS( L_IabcGnd ) - L_IabcGndFil );

        if ( L_IabcGndFil >= PARM(L_IabcFltThr) )
        {
            PUSH_DIAG(L_IabcGndFlt);
            Q1OpnReq = true;
        }

    }

    // Discriminate ground current for use in Task 3 fault detection

    cVectorf::Discrim( L_IabcGnd, &GmSinCosFbk, &L_IxGndT1, &L_IyGndT1 );

    if ( L_CellTest.CelTstProtEnab == false )
    {
        L_IxGndT1 = 0.0F;
        L_IyGndT1 = 0.0F;
    }

    return;
}


///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//      Calculates the magnitude of the ground current, filters it, and
//      checks the result against fault levels.
//
// EXECUTION LEVEL
//      Task3
//
// RETURN VALUE
//      none
//
///////////////////////////////////////////////////////////////////////////////
void InvGndFltDetectT3( void )
{
    // Filter X and Y components of ground current

    L_IxGndFil += L_IxIyGndGn * ( L_IxGndT3 - L_IxGndFil );
    L_IyGndFil += L_IxIyGndGn * ( L_IyGndT3 - L_IyGndFil );

    // Calculate magnitude of ground current

    L_IxyGnd = sqrtf(( L_IxGndFil * L_IxGndFil ) + ( L_IyGndFil * L_IyGndFil ));

    // Filter ground current magnitude

    L_IxyGndFil += L_IxyGndGn * ( L_IxyGnd - L_IxyGndFil );

     // Check for fault conditions

    if ( L_IxyGndFil >= PARM(L_GndFltThrs) )
    {
        PUSH_DIAG( L_InvGndFlt );
        Q1OpnReq = true;
    }
    if ( L_IxyGndFil >= PARM(L_GndAlmOn) )
    {
        PUSH_DIAG( L_InvGndAlm );
    }
    else if ( L_IxyGndFil <= PARM(L_GndAlmOff) )
    {
        //Pop3Flt( L_InvGndAlm );
    }

    //  - DC ground fault check using slower filter and more sensitive threshold
    if (L_IOffsVldT3)
    {
        L_IabcDcGndFil += PARM(L_WIabcDcGnd) * DelTm3 * ( ABS( L_IabcGndT3 ) - L_IabcDcGndFil );
    }
    else
    {
        L_IabcDcGndFil = 0.0F;
    }

    if ( L_IabcDcGndFil >= PARM(L_IabcDcFltThr) )
    {
        PUSH_DIAG(L_IabcDcGndFlt);
        Q1OpnReq = true;

    }

    return;
}
