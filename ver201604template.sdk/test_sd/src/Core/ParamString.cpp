///////////////////////////////////////////////////////////////////////////////
//                                                                     
// TITLE:       Paramaterized String
//
// $Header: /Runtime/MarkVIe/CurrentProductVersion/Lib/diagapi/ParamString.cpp 10    12/02/04 2:04p Brownp1 $
//
// AUTHOR:      Paul J. Brown
//
// DESCRIPTION:
//              This module contains the implementation of the cParamString
//                class. The cParamString class allows a std::string to be 
//                assigned using a printf style format specification.
//
// COPYRIGHT:   Copyright (c) 2002
//              by GENERAL ELECTRIC COMPANY U.S.A.
//
// DOCUMENTATION:
//              List Visual SourceSafe locations of pertinent design documents.
//
///////////////////////////////////////////////////////////////////////////////
// REVISION HISTORY:
// $Log: /Runtime/MarkVIe/CurrentProductVersion/Lib/diagapi/ParamString.cpp $
// 
// 10    12/02/04 2:04p Brownp1
// Made corrections to allow the use of the new cDString base class for
// cParamString.
// 
// 9     9/13/04 2:32p Jacksorl
// Fixed cut paste error
// 
// 8     9/11/04 1:32p Jacksorl
// Added "l" option to display long long in llm.
// 
// 7     5/19/04 3:07p Ridenohp
// Removed namespace using.
// 
// 6     11/24/03 3:42p Brownp1
// Corrected problem in zero padding widh calculation.
// 
// 5     9/04/03 9:20a Brownp1
// Corrected Maximum Size bug in the += operator.  It was causing a
// negative number of characters to be appended in certain situations.
// 
// 4     4/01/03 1:30p Brownp1
// Corrected problem with precision flag.
// 
// 3     3/26/03 1:25p Brownp1
// Corrected percision bug in format of floating point values.
// 
// 2     10/14/02 4:49p Brownp1
// Added const qualifier to copy constructor.
// 
// 1     9/30/02 2:54p Brownp1
// Initial Release
//
///////////////////////////////////////////////////////////////////////////////

// Include Files: 
//---------------
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ParamString.h"


// Constants:
//-----------

// Externals:
//-----------

// Types and Classes:
//-------------------                   

// Internal Function Prototypes:
//------------------------------

// Variables:
//-----------

// Member Functions:
//------------------

///////////////////////////////////////////////////////////////////////////////
//
// NAME: cParamString::ParamAssign
//
// DESCRIPTION:
//      Assign a string to the string using a printf style format.
//
// PARAMETERS: char *FormatString - A character string that will be assigned
//                to the string which may contain printf style format 
//                specifiers.
//
// GLOBAL DATA:
//      VariableName: What the variable is used for
//
// RETURN VALUE: N/A
//
//
///////////////////////////////////////////////////////////////////////////////
int cParamString::ParamAssign(const char *FormatString, ...)
{
    va_list Argument;
    char *Buffer;

    *this = "";
    Buffer = GetFreeChunk(128);
    if(Buffer != NULL)
    {
        va_start(Argument, FormatString);
        vsnprintf(Buffer, 128, FormatString, Argument);
        va_end(Argument);
    }

    return(0);
}


///////////////////////////////////////////////////////////////////////////////
//
// NAME: cParamString::cParamString
//
// DESCRIPTION:
//      Constructor that initializes a string to the string using a 
//		printf style format.
//
// PARAMETERS: char *FormatString - A character string that will be assigned
//					to the string which may contain printf style format 
//					specifiers.
//
// GLOBAL DATA:
//      VariableName: What the variable is used for
//
// RETURN VALUE: N/A
//
//
///////////////////////////////////////////////////////////////////////////////

cParamString::cParamString(const char *FormatString, ...) : cDString(DEFAULT_GEDSTRING_BLKSIZE)
{
    va_list Argument;
    char *Buffer;

    mMaximumSize = 0xFFFFFFFF;
    
    *this = "";
    Buffer = GetFreeChunk(128);
    if(Buffer != NULL)
    {
        va_start(Argument, FormatString);
        vsnprintf(Buffer, 128, FormatString, Argument);
        va_end(Argument);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// NAME: cParamString::pappend
//
// DESCRIPTION:
//      Append the parameterized string to the current string object.
//
// PARAMETERS: char *FormatString - A character string that will be appended
//                    to the string which may contain printf style format 
//                    specifiers.
//
// GLOBAL DATA:
//      VariableName: What the variable is used for
//
// RETURN VALUE: N/A
//
//
///////////////////////////////////////////////////////////////////////////////
int cParamString::pappend(const char *FormatString, ...)
{
    va_list Argument;
    char *Buffer;

    if((unsigned int)length() >= mMaximumSize)
        return 0;
      
      
    Buffer = GetFreeChunk(128);
    if(Buffer != NULL)
    {
        va_start(Argument, FormatString);
        vsnprintf(Buffer, 128, FormatString, Argument);
        va_end(Argument);
    }
    return 0;
}

