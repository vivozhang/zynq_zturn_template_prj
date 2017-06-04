///////////////////////////////////////////////////////////////////////////////
//
// TITLE:
//
// DESCRIPTION:
//      Elemental vector transformations between the three-phase stator
//      coordinate system (A-B-C) and the rotating reference frame (X-Y)
//
// COPYRIGHT:   Copyright (c) 2004
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __X0_VECTOR_H__
#define __X0_VECTOR_H__


// Includes:
//----------
// system
#include <math.h>
// framework
#include "x0FrameWork.h"



///////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//
//  Class to provide elemental vector transformations between the three-phase
//  stator coordinate system (A-B-C) and the rotating reference frame (X-Y)
//
//  The methods are "static" so that this class does not have to be instanced.
//
//  The methods are defined inside the H file to force the compiler to inline.
//
//  Include this H file to access these methods.
//
//  Example Usage:  cVectorf::Modulot2Pi(angle);
//
///////////////////////////////////////////////////////////////////////////////
class cVectorf
{

//-----------------------------------------------------------------------------
public:

    //-------------------------------------------------------------------------
    // Modulo2Pi: returns equivalent angle in range [0 .. 2 * PI)
    //-------------------------------------------------------------------------
    static float Modulo2Pi(float angle)
    {
        return fmodf( angle, (2.0F * PI) );
    }

    //-------------------------------------------------------------------------
    // SineCosine: fills a structure with the sine and cosine of the input angle.
    // The intent is to compute these values once, then re-use them
    // multiple times in the vector transformations.
    //-------------------------------------------------------------------------
    static void SineCosine(float angle, SinCosTyp* sincos)
    {
        sincos->sin = sinf(angle);
        sincos->cos = cosf(angle);
    }

    //-------------------------------------------------------------------------
    // DemodLN: Demodulate, Line-to-Neutral.  Nothing fancy, just the usual vector math.
    //-------------------------------------------------------------------------
    static void DemodLN(float A, float B, SinCosTyp* sincos, float* X, float* Y)
    {
        *X = A * (sincos->cos + (float)(1.0 / SQRT3) * sincos->sin) + B * (float)(2.0 / SQRT3) * sincos->sin;

        *Y = A * ((float)(1.0 / SQRT3) * sincos->cos - sincos->sin) + B * (float)(2.0 / SQRT3) * sincos->cos;
    }

    //-------------------------------------------------------------------------
    // DemodLN3: Demodulate, Line-to-Neutral, using A, B and phase C.
    //-------------------------------------------------------------------------
    static void DemodLN3(float A, float B, float C, SinCosTyp* sincos, float* X, float* Y)
    {
        *X = A * ( (float)( 2.0 / 3.0) * sincos->cos ) +
             B * ( (float)(-1.0 / 3.0) * sincos->cos  + (float)(1.0 / SQRT3) * sincos->sin ) +
             C * ( (float)(-1.0 / 3.0) * sincos->cos  - (float)(1.0 / SQRT3) * sincos->sin );

        *Y = A * ( (float)(-2.0 / 3.0) * sincos->sin ) +
             B * ( (float)( 1.0 / 3.0) * sincos->sin  + (float)(1.0 / SQRT3) * sincos->cos ) +
             C * ( (float)( 1.0 / 3.0) * sincos->sin  - (float)(1.0 / SQRT3) * sincos->cos );
    }

    //-------------------------------------------------------------------------
    // ABCToAlphaBeta: Transform A, B, C to alpha-beta.
    //-------------------------------------------------------------------------
    static void ABCToAlphaBeta(float A, float B, float C, float* Alpha, float* Beta)
    {
        *Alpha = A * (float)(2.0 / 3.0) - B * (float)(1.0 / 3.0)   - C * (float)(1.0 / 3.0);

        *Beta =  B * (float)(1.0 / SQRT3) - C * (float)(1.0 / SQRT3);
    }

    //-------------------------------------------------------------------------
    // AlphaBetaToXY: Transform alpha-beta to XY.
    //-------------------------------------------------------------------------
    static void AlphaBetaToXY(float Alpha, float Beta, SinCosTyp* sincos, float* X, float* Y)
    {
        *X = Alpha * sincos->cos + Beta  * sincos->sin;
        *Y = Beta  * sincos->cos - Alpha * sincos->sin;
    }

    //-------------------------------------------------------------------------
    // AlphaBetaToABC: Transform alpha-beta to A, B, C
    //-------------------------------------------------------------------------
    static void AlphaBetaToABC(float Alpha, float Beta, float* A, float* B, float* C)
    {
        *A = Alpha;
        *B = Alpha * (float)(-1.0 / 2.0) + Beta * (float)(SQRT3 / 2.0);
        *C = Alpha * (float)(-1.0 / 2.0) - Beta * (float)(SQRT3 / 2.0);
    }

    //-------------------------------------------------------------------------
    // DemodLL: Demodulate, Line-to-Line.  More of the same.
    //-------------------------------------------------------------------------
    static void DemodLL(float AB, float BC, SinCosTyp* sincos, float* X, float* Y)
    {
        *X = AB *  (float)(2.0 /   3.0) * sincos->cos +
             BC * ((float)(1.0 /   3.0) * sincos->cos + (float)(1.0 / SQRT3) * sincos->sin);

        *Y = AB * -(float)(2.0 /   3.0) * sincos->sin +
             BC * ((float)(1.0 / SQRT3) * sincos->cos - (float)(1.0 /   3.0) * sincos->sin);
    }

    //-------------------------------------------------------------------------
    // RotateLN: Rotate, Line-to-Neutral.  More of the same.
    //-------------------------------------------------------------------------
    static void RotateLN(float X, float Y, SinCosTyp* sincos, float* A, float* B)
    {
        *A = X * sincos->cos - Y * sincos->sin;

        *B = X * (-(float)(  1.0 / 2.0) * sincos->cos + (float)(SQRT3 / 2.0) * sincos->sin) +
             Y * ( (float)(SQRT3 / 2.0) * sincos->cos + (float)(  1.0 / 2.0) * sincos->sin);
    }

    //-------------------------------------------------------------------------
    // RotateLL: Rotate, Line-to-Line.  Are you through, yet?
    //-------------------------------------------------------------------------
    static void RotateLL(float X, float Y, SinCosTyp* sincos, float* AB, float* BC)
    {
        *AB = X * ( (float)(  3.0 / 2.0) * sincos->cos - (float)(SQRT3 / 2.0) * sincos->sin) +
              Y * (-(float)(SQRT3 / 2.0) * sincos->cos - (float)(  3.0 / 2.0) * sincos->sin);

        *BC = X * (float)SQRT3 * sincos->sin + Y * (float)SQRT3 * sincos->cos;
    }

    //-------------------------------------------------------------------------
    // Discrim: Discriminate a scalar.
    //-------------------------------------------------------------------------
    static void Discrim(float A, SinCosTyp* sincos, float* X, float* Y)
    {
        *X = A *  (sincos->cos);
        *Y = A * -(sincos->sin);
    }

};


#endif  // __X0_VECTOR_H__







