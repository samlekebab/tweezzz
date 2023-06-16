//#ifndef MSETTINGS
//#define MSETTINGS

#define SAMPLE_RATE (600*1000000)

//card contains 2GSample memory 
#define SEGMENT_SIZE (19*1024*10) //number of sample per segment
#define BATCH_SIZE (1024*10) //number of sample where parameters stay constant
#define BUFFER_SIZE (500*1000)  //number of segment in a cyclic buffer
#define SAFE_TICK 0//DEBUG (3125*960)     //min delay (in ticks) between a push to the card and the actual play (directly related to the delay)
#define MAX_TICK (10000*960) //how far we calculate in advance the segments (should'nt influence the delay)
#define MAX_VALUE 32767 	//max value of 16bit signed integer

//#endif
