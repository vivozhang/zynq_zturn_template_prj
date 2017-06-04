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
#ifndef __BPPB_DPM_IONET_H__
#define __BPPB_DPM_IONET_H__


#if 0
//*****************************************************************************
//                                  IoNet output
//*****************************************************************************
// this structure must agree with IoNetFrmCvtrBuff_Typ in WecaDpm.h
//                           and  WECA.cs in WECADLL
struct IoNetFrmCvtr_Typ
{
    #define  FROM_CONVERTER_DEF
    #include "work\IoNetDef.inc"
    #undef   FROM_CONVERTER_DEF
};


//*****************************************************************************
//                                  IoNet input
//*****************************************************************************
// this structure must agree with IoNetToCvtrBuff_Typ in WecaDpm.h
//                           and  WECA.cs in WECADLL
struct IoNetToCvtr_Typ
{
    #define  TO_CONVERTER_DEF
    #include "work\IoNetDef.inc"
    #undef   TO_CONVERTER_DEF
} ;
#endif


#endif  // __BPPB_DPM_IONET_H__
