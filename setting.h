//#ifndef MSETTINGS
//#define MSETTINGS

#define SAMPLE_RATE (200*1000000)

//card contains 2GSample memory 
#define SEGMENT_SIZE (19*1024*10) //number of sample per segment
#define BATCH_SIZE (1024*10) //number of sample where parameters stay constant
#define BUFFER_SIZE (500)  //number of segment in a cyclic buffer
#define SAFE_TICK 0//DEBUG (3125*960)     //min delay (in ticks) between a push to the card and the actual play (directly related to the delay)
#define MAX_TICK ((long)10000000*960) //how far we calculate in advance the segments (should'nt influence the delay)
#define MAX_VALUE 32767 	//max value of 16bit signed integer


//settings in core.cpp

//#define file_output
//#define once//to debug, no core loop
//#define test_perf_mode //this mode push the code to go as fast as possible au print result 
#define DEBUG_SAMPLE_RATE 100000

#define GPU_calculation
//#define no_card_connected //no card is connected : use only the pc clock to get the current card segment
//#define opti_prevent //just dum calculation to make sure the compiler does'nt generate code that overestimate the performances.

//#endif
