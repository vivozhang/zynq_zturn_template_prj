///////////////////////////////////////////////////////////////////////////////
//
// TITLE:       x0Err.h
//
// DESCRIPTION:
//              Common event log and error log strings
//
// COPYRIGHT:   Copyright (c) 2002
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              none
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __X0_ERR_H__
#define __X0_ERR_H__



// constants for diagnostic reporting
#define HERE            __LINE__, __FILE__
#define NO_SPACE        -2
#define INVALID_INDEX   -3



// event and error notification
const int MKS_ERR_BASE = 10000;
const int MKS_EVT_BASE = 10000;



// common message prefixes
const char  MKS_SYS_CONNECT_STR[]   = "MKS Sys Connect: %s";
const char  MKS_PLATFORM_MEM_STR[]  = "MKS Platform: %s";
const char  MKS_INTRINSIC_MEM_STR[] = "MKS Intrinsic: %s";
const char  MKS_LLM_STR[]           = "MKS Llm: %s";
const char  MKS_INIT_STR[]          = "MKS Init: %s";
const char  MKS_XSIM_SVR_STR[]      = "MKS XsimSvr: %s";
const char  MKS_XSIM_INIT_STR[]     = "MKS XsimIni: %s";
const char  MKS_MSG_SVR_STR[]       = "MKS MsgSvr: %s";
const char  MKS_SDI_SVR_STR[]       = "MKS SdiSvr: %s";
const char  MKS_ADL_SVR_STR[]       = "MKS AdlSvr: %s";
const char  MKS_CAP_BUF_STR[]       = "MKS CapBuf: %s";
const char  MKS_DATAP_STR[]         = "MKS DataPass: %s";
const char  MKS_DIAG_STR[]          = "MKS Diag: %s";
const char  MKS_PARM_STR[]          = "MKS Parm: %s";
const char  MKS_VAR_STR[]           = "MKS Var: %s";
const char  MKS_SYM_TBL_STR[]       = "MKS SymTbl: %s";
const char  MKS_XOPS_STR[]          = "MKS Xops: %s";
const char  MKS_DLL_STR[]           = "MKS DLL: %s";



#endif  // __X0_ERR_H__
