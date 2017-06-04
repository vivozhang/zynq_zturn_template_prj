#ifndef __THREADINFO_H__
#define __THREADINFO_H__


#include "GeSList.h"

class cThreadInfo
{
public:
    cThreadInfo(int Id, const char* pName)   {;}
};
typedef cSLinkedList<cThreadInfo> cThreadInfoList;
extern cThreadInfoList gThreadInfoList;



#endif  // __THREADINFO_H__

