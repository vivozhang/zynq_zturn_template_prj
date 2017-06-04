#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__


#include <stddef.h>
#include <stdio.h>
#include <list>



// --------------------------------------------------------
// semaphore
// hijack semaphore to solve x0Xops  xopsSched() problem
// use semaphore to call task directly
struct sem_t
{
    void*  pTaskData;
    void* (*pTask)(void*);
    int (i);
};

extern std::list <sem_t*> TaskRunList;

inline int sem_post(sem_t* A)
{
    TaskRunList.insert(TaskRunList.end(), A);

    return 0;
}
inline int sem_init(sem_t *__sem, int __pshared, unsigned __Value)  {return 0;}
inline int sem_wait(sem_t *__sem)                                   {return 0;}
// --------------------------------------------------------




#endif  // __SEMAPHORE_H__
