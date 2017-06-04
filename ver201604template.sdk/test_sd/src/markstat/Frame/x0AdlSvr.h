///////////////////////////////////////////////////////////////////////////////
//
// TITLE:
//
// DESCRIPTION:
//             Header file for X0ADLSRV.CPP
//
// COPYRIGHT:   Copyright (c) 2010
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//
///////////////////////////////////////////////////////////////////////////////
#ifndef X0_ADL_SVR_H
#define X0_ADL_SVR_H


// Constants:
//-----------

// ADL command error codes
#define ADL_SUCCESS 0x00000000          // command was successful
#define ADLCMD_CC   0x00100000          // Command Code error
#define ADLCMD_CD   0x00110000          // Command Data error
#define ADLCMD_SB   0x00120000          // Service Busy error
#define ADLCMD_RF   0x00130000          // Request Fail error
#define ADLCMD_Ix   0x00140000          // Command Index error
#define ADLCMD_NI   0x00150000          // Not Implemented command
#define ADLCMD_NS   0x00160000          // Drive Not Stopped error
#define ADLCMD_BP   0x00170000          // Bad cmd Parameter error
#define ADLCMD_MF   0x00180000          // No Malloc memory available
#define ADLCMD_EF   0x00190000          // Erase EE flash in progress
#define ADLCMD_TF   0x001A0000          // Trip Fault present
#define ADLCMD_BM   0x001B0000          // Boot Monitor Running
#define ADLCMD_RU   0x001C0000          // Rule execution Unsuccessful


// Types and Classes:
//-------------------
struct MemTypeTbl_Typ
{
    void        *Addr;
    unsigned    Size;
};


#endif  // X0_ADL_SVR_H
