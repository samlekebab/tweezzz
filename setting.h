#ifndef MSETTINGS
#define MSETTINGS

#define SAMPLE_RATE 600'000'000

//card contains 2GSample memory 
#define SEGMENT_SIZE 96000 //number of sample per segment
#define BUFFER_SIZE 500'000  //number of segment in a cyclic buffer
#define SAFE_TICK 0//DEBUG (3125*960)     //min delay (in ticks) between a push to the card and the actual play (directly related to the delay)
#define MAX_TICK (10'000*960) //how far we calculate in advance the segments (should'nt influence the delay)
#define MAX_VALUE 32767 	//max value of 16bit signed integer

#endif
