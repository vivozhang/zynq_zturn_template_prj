///////////////////////////////////////////////////////////////////////////////
//                                                                     
// TITLE:       Parameterized String Object
//
// $Header: /Runtime/MarkVIe/CurrentProductVersion/Lib/diagapi/ParamString.h 4     5/31/05 3:40p Brownp1 $
//
// DESCRIPTION:
//   This file contains the definition of the cParamString class.
//   The cParamString class is derived from the std::string class.
//   The cParamString class allows a user to assign a string to
//   the class using a printf style format string. J. Brown
//
// COPYRIGHT:   Copyright (c) 2002
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////
// REVISION HISTORY:
// $Log: /Runtime/MarkVIe/CurrentProductVersion/Lib/diagapi/ParamString.h $
// 
// 4     5/31/05 3:40p Brownp1
// Added changes to allow code to build in QNX 6.3 with GNU 3.1.1
// compiler.
// 
// 3     12/02/04 2:04p Brownp1
// Made corrections to allow the use of the new cDString base class for
// cParamString.
// 
// 2     10/14/02 4:50p Brownp1
// Added const qualifier to cParamString copy constructor.
// 
// 1     9/30/02 2:54p Brownp1
// Initial Release
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PARAMSTRING_H
#define PARAMSTRING_H

// Include Files: 
//---------------

#include <stdarg.h>

#include "GeDString.h"

// Constants:
//-----------

const int MAX_NUMBER_LENGTH = 400;
const int MAX_NUMBER_PRECISION = 32;

const int MAXIMUM_SIZE_REACHED = 1;

// Types and Classes:
//-------------------                   

class cParamString : public cDString
{
public:
    typedef SIZE_TYPE size_type;
    
    cParamString(const char *FormatString, ...);
    cParamString();
    
    int ParamAssign(const char *FormatString, ...);
    int pappend(const char *FormatString, ...);
    
    cParamString & operator= (const cParamString &ParamString);
    cParamString & operator= (const char *CharString);
    
    cParamString & operator+= (const cParamString &NewString);
    cParamString & operator+= (const char *NewString);
    cParamString & operator+= (char NewChar);
    
    cParamString(const cParamString &ParamString);
    
    void SetMaximumSize(unsigned int MaximumSize);
private:
    
    unsigned int mMaximumSize;
};

inline cParamString::cParamString() : cDString(DEFAULT_GEDSTRING_BLKSIZE)
{
    mMaximumSize = 0xFFFFFFFF;
    
    *this = "";    
}

inline cParamString::cParamString(const cParamString &ParamString) : cDString(DEFAULT_GEDSTRING_BLKSIZE)
{
    mMaximumSize = 0xFFFFFFFF;
    this->assign(ParamString.c_str());
}


inline void cParamString::SetMaximumSize(unsigned int MaximumSize)
{
    mMaximumSize = MaximumSize;    
}
#include <stdio.h>

inline cParamString & cParamString::operator= (cParamString const &ParamString)
{
//    printf("In ParamStringAssign\n"); 
    assign(ParamString.c_str());
    return *this;
}

inline cParamString & cParamString::operator= (char const *CharString)
{
//    printf("In ConstChar* Assign '%s'\n", CharString);
    assign(CharString);
    return *this;
}

inline cParamString & cParamString::operator+= (char const *NewString)
{
    if((unsigned int)length() < mMaximumSize)
        append(NewString);  
    
    return *this;
}

inline cParamString & cParamString::operator+= (cParamString const &ParamString)
{
    if((unsigned int)length() < mMaximumSize)
        append(ParamString.c_str());  
    
    return *this;
}

inline cParamString & cParamString::operator+= (char NewChar)
{
    if((unsigned int)length() < mMaximumSize)
        append(NewChar);
        
    return *this;
}


#endif //PARAMSTRING_H
