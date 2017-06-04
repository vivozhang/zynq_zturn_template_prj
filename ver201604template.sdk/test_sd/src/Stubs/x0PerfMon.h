#ifndef __X0PERFMON_H__
#define __X0PERFMON_H__




//---------------------------------------------------
// Some known MSRs
#define MSR_TSC                 0x0010
#define MSR_PERF_CTR0           0x00c1
#define MSR_PERF_CTR1           0x00c2
#define MSR_PERF_EVT_SEL0       0x0186
#define MSR_PERF_EVT_SEL1       0x0187
#define MSR_LASTBRANCHFROMIP    0x01db
#define MSR_LASTBRANCHTOIP      0x01dc
#define MSR_LASTINTFROMIP       0x01dd
#define MSR_LASTINTTOIP         0x01de

#define MSR_DEMAND_DATA_READ_L2_HIT     0x00430024      // demand data read requests that hit L2 cache
#define MSR_INSTRUCT_RETIRED            0x004300C0      // number of instructions at retirement


//int  ReadMsr  ( uint32_t  Num, uint64_t* Val    );
inline int  WriteMsr ( uint32_t  Num, uint64_t  NewVal )    {return 0;}
inline void ReadPmc  ( uint32_t  Idx, uint64_t* Val    )    {;}
//void ReadTsc  ( uint64_t* Val                   );
inline int  ReadCr   ( uint32_t  Num, uint32_t* Val    )    {return 0;}
inline int  WriteCr  ( uint32_t  Num, uint32_t  NewVal )    {return 0;}



#endif  // __X0PERFMON_H__

