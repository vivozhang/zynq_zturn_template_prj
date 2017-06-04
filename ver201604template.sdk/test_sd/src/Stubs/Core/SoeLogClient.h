#ifndef SOELOGCLIENT_H_
#define SOELOGCLIENT_H_


//
// This is the structure of SOE records as received from the IO Packs
//
typedef struct __Soe_Dollar_entry {
  unsigned  index;          // assigned by I/O Pack (0 -> 4294967295)
  sTimeType  time_spec;      // 8 bytes.  UTC time of the occurrence of the
  							// event expressed in POSIX timespec format.
  							//    time_t   tv_sec;   // sec since 1970
  							//    long     tv_nsec;  // ns  since last sec

  unsigned  producer_id;    // for example: 21
  int       drop_number;	// assigned by I/O Pack.
  short		channel;		// R=1, S=2, T=3, else 0.
  short		state;			// specifies whether the event transitioned on
  							// or off when message was sent
  } Soe_Dollar_entry;			// 24 bytes


class cSoeLogClient
{
public:
    int  Open()     {return 0;}
    int  Close()    {return 0;}
    int  LogSoeTransition(Soe_Dollar_entry SoeEntry,unsigned int SoeIndex);
};




#endif /* SOELOGCLIENT_H_ */
