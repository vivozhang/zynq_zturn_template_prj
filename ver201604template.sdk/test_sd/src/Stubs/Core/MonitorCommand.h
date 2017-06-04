#ifndef __MONITORCOMMAND_H__
#define __MONITORCOMMAND_H__


#include "ParamString.h"



const int NORMAL_OUTPUT_SIZE = 0x2000;

class cMonitorCommand
{
public:
    int Add(cParamString Command, 
        cParamString CommandHelp, 
        int (*CommandFunction)(cParamString &, cParamString &),
        int MaximumOutputSize = 0x2000,
        cParamString ShortCommandName = "")   {return 0;}
};    



#endif  // __MONITORCOMMAND_H__

