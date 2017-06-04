#ifndef __SYSPAGE_H__
#define __SYSPAGE_H__



#include <stdint.h>

#define SYSPAGE_ENTRY(base)    (&base)
struct qtime_entry {
	uint64_t					cycles_per_sec;	/* for ClockCycles */
	uint64_t volatile			nsec_tod_adjust;
	uint64_t volatile			nsec;
	unsigned long				nsec_inc;
	unsigned long				boot_time; /* UTC seconds when machine booted */

	//struct _clockadjust			adjust;
	unsigned long				timer_rate;	/* times 10^timer_scale */
	long						timer_scale;
	unsigned long				timer_load;
	long						intr;
	unsigned long				epoch;
	unsigned long				flags;
	unsigned int				rr_interval_mul;
	unsigned long				spare0;
	uint64_t volatile			nsec_stable;
	unsigned long				spare[4];
};
extern qtime_entry qtime;



#endif  // __SYSPAGE_H__

