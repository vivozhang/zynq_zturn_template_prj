#ifndef __NEUTRINO_H__
#define __NEUTRINO_H__




#include <stdint.h>
#include <stddef.h>

/* --- Symbolic names of the error return conditions --- */
#define EOK              0  /* No error                                 */

/*
 * Flag bits for InterruptAttach[Event]
 */
#define _NTO_INTR_FLAGS_END     0x01
#define _NTO_INTR_FLAGS_PROCESS 0x04
#define _NTO_INTR_FLAGS_TRK_MSK 0x08
#define _NTO_TCTL_IO               1
#define CLOCK_INTR                 0


inline uint64_t ClockCycles()    {return 0;}


struct value_t {
    int     sival_int;
};
struct _pulse {
    value_t value;
    int     code;
};

struct _msg_info {
    int     a;
};
inline int MsgReceive(int __chid, void *__msg, int __bytes, struct _msg_info *__info) {return EOK;}


typedef struct intrspin {
    int     a;
} intrspin_t;
inline int  InterruptAttach(int __intr, const struct sigevent *(*__handler)(void *__area, int __id), const void *__area, int __size, unsigned __flags) {return EOK;}
inline void InterruptLock(struct intrspin * __spin)                 {;}
inline void InterruptUnlock(struct intrspin * __spin)               {;}
inline int  InterruptAttachEvent(int __intr, const struct sigevent * __Event, unsigned __flags) {return EOK;}
inline int  InterruptWait(int __flags, const uint64_t *__timeout)   {return EOK;}
inline int  InterruptMask(int __intr, int __id)                     {return EOK;}
inline int  InterruptUnmask(int __intr, int __id)                   {return EOK;}
inline void InterruptEnable(void)                                   {;}
inline void InterruptDisable(void)                                  {;}

//======================================================


// trace
#define _NTO_TRACE_STOP              ((0x00000001<<28)|11)
#define _NTO_TRACE_INSERTUSRSTREVENT ((0x00000003<<28)|32)
inline int TraceEvent(int __code, ...)     {return 0;}

// inline
inline unsigned  pswget(void)              {return 0;}


// siginfo.h
#define SIGEV_INTR_INIT(a)
#define SIGEV_SIGNAL_INIT(a,b)
struct sigevent {
    int    a;
};


// time.h
#define     CLOCK_REALTIME  0
struct sTimeType {
    uint32_t  tv_sec;
    int32_t   tv_nsec;
};
typedef uint32_t    clockid_t;
inline int clock_gettime(clockid_t __clock_id, struct sTimeType *pTime)  {pTime->tv_sec=0; pTime->tv_nsec=0; return 0;}


// iomsg.h
struct _io_msg {
    int     subtype;
};


// signal.h
#define     SIGUSR1     16  /* user defined signal 1 */
#define     SIGSEGV     1
#define     SIGFPE      1
struct siginfo_t {
    int     si_code;
    int     si_addr;
    int     si_errno;
    int     si_fltno;
    int     si_fltip;
};
struct sigset_t {
    int     a;
};
struct sigaction {
    # define sa_handler     __sa_un._sa_handler
    # define sa_sigaction   __sa_un._sa_sigaction
    union {
        void        (*_sa_handler)(int);
        void        (*_sa_sigaction)(int, siginfo_t *, void *);
    } __sa_un;
    int         sa_flags;
    sigset_t    sa_mask;
};
inline int sigemptyset(sigset_t *__set)                                                      {return EOK;}
inline int sigaddset(sigset_t *__set, int __signo)                                           {return EOK;}
inline int sigaction(int __signo, const struct sigaction *__act, struct sigaction *__oact)   {return EOK;}


// pthread.h
#define SCHED_FIFO                     1
#define PTHREAD_CREATE_DETACHED     0x01
#define PTHREAD_EXPLICIT_SCHED      0x02
struct sched_param {
    int     sched_priority;
};
typedef uint32_t    pthread_t;
typedef uint32_t    pthread_attr_t;
inline int pthread_create(pthread_t *__thr, const pthread_attr_t *__attr, void *(*__start_routine)(void *), void *__arg)   {return EOK;}
inline int pthread_attr_init(pthread_attr_t *__attr)                                               {return EOK;}
inline int pthread_attr_setinheritsched(pthread_attr_t *__attr, int __inheritsched)                {return EOK;}
inline int pthread_attr_setdetachstate(pthread_attr_t *__attr, int __detachstate)                  {return EOK;}
inline int pthread_attr_setschedpolicy(pthread_attr_t *__attr, int __policy)                       {return EOK;}
inline int pthread_attr_getschedparam(const pthread_attr_t *__attr, struct sched_param *__param)   {return EOK;}
inline int pthread_attr_setschedparam(pthread_attr_t *__attr, const struct sched_param *__param)   {return EOK;}


// pthread.h
typedef uint32_t	pthread_mutex_t;
typedef uint32_t	pthread_mutexattr_t;
inline int pthread_mutex_destroy(pthread_mutex_t *__mutex)      {return EOK;}
inline int pthread_mutex_init(pthread_mutex_t *__mutex, const pthread_mutexattr_t *__attr)   {return EOK;}
inline int pthread_mutex_lock(pthread_mutex_t *__mutex)         {return EOK;}
inline int pthread_mutex_unlock(pthread_mutex_t *__mutex)       {return EOK;}


// pthread.h
inline uint32_t    getpid(void)                                 {return 0;}
inline int ThreadCtl(int __cmd, void *__data)                   {return 0;}
inline int pthread_setschedprio(pthread_t __thr, int __prio)    {return 0;}
inline int ChannelCreate(unsigned __flags)                      {return 0;}


// unistd.h
inline int sleep(uint32_t __seconds)                            {return 0;}
inline int usleep(uint32_t __useconds)                          {return 0;}
inline unsigned delay(unsigned int __milliseconds)              {return 0;}




#endif  // __NEUTRINO_H__

