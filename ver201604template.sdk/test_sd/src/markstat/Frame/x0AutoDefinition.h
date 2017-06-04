///////////////////////////////////////////////////////////////////////////////
#ifndef _AUTODEFINITION_H_
#define _AUTODEFINITION_H_
///////////////////////////////////////////////////////////////////////////////


// Include Files
//--------------
// system
#include <stdint.h>
// framework
#include "x0Parameter.h"
#include "x0Diagnostic.h"


// Externals:
//-----------
extern cParameter       gParameter;
extern cMarkStatDiag    MarkStatDiag;


#ifndef PRE_PROC
///////////////////////////////////////////////////////////////////////////////
#undef  CREATE_PARM
#undef  CREATE_OUR_PARM
#undef  CREATE_PUBVAR
#undef  CREATE_OUR_PUBVAR
#undef  CREATE_COMPLEX_PUBVAR
#undef  CREATE_DIAG
#undef  CREATE_OUR_DIAG
#undef  PARM
#undef  cPARM
#undef  READ_PARMPTR
#undef  READ_cPARMPTR
#undef  SET_PARM
#undef  SET_FV
#undef  PUSH_DIAG
#undef  POP_DIAG
#undef  INSTANCE_INFO
#undef  DATA_PASS


///////////////////////////////////////////////////////////////////////////////
#define CREATE_PARM(Name, Type)         Type PARM_##Name
#define CREATE_OUR_PARM(Name, Type)     static Type PARM_##Name
#define PARM(Name)                      (PARM_##Name)
#define cPARM(Class, Name)              ((Class)->PARM_##Name)
#define READ_PARMPTR(Name)              gParameter.ReadPtrValue(PARM_##Name)
#define READ_cPARMPTR(Class, Name)      gParameter.ReadPtrValue((Class)->PARM_##Name)

#define SET_PARM(Name, Value)           gParameter.SetParm(&PARM_##Name, Value)
#define SET_FV(Name, Value)             {if ( (gParameter.FvRegister(&PARM_##Name) == SUCCESS)  \
                                           && !gParameter.IsFvOverridden(&PARM_##Name) )        \
                                            PARM_##Name = Value;}

///////////////////////////////////////////////////////////////////////////////
#define CREATE_PUBVAR(Name, Type)       Type Name
#define CREATE_OUR_PUBVAR(Name, Type)   static Type Name
#define CREATE_COMPLEX_PUBVAR(Name, Type) Type Name

///////////////////////////////////////////////////////////////////////////////
#define CREATE_DIAG(Name)               int DIAG_##Name
#define CREATE_OUR_DIAG(Name)           static int DIAG_##Name
#define PUSH_DIAG(Name)                 MarkStatDiag.PushDiagnostic(DIAG_##Name)
#define POP_DIAG(Name)                  MarkStatDiag.PopDiagnostic(DIAG_##Name)

///////////////////////////////////////////////////////////////////////////////
#define INSTANCE_INFO(Instance, Uniquifier, Bridge, Task1)

///////////////////////////////////////////////////////////////////////////////
#define DATA_PASS(Src, Dst, Type, Filter)

///////////////////////////////////////////////////////////////////////////////
#define WORD  uint32_t


///////////////////////////////////////////////////////////////////////////////
#endif  // PRE_PROC


///////////////////////////////////////////////////////////////////////////////
#endif  // _AUTODEFINITION_H_

