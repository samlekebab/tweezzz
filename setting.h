//#ifndef MSETTINGS
//#define MSETTINGS

#define SAMPLE_RATE (330*1000000)

//card contains 2GSample memory 
#define SEGMENT_SIZE (19*1024*10) //number of sample per segment
#define BATCH_SIZE (1024*10)//too big? //number of sample where parameters stay constant
#define BUFFER_SIZE (1000)  //number of segment in a cyclic buffer
#define SAFE_TICK (10*1024*10)//(3125*960)     //min delay (in ticks) between the start of the computation and the push to the card (directly related to the delay)
#define MAX_TICK ((long)19*1024*10*10) //how far we calculate in advance segments (should'nt influence the delay)
#define MAX_VALUE 32767 	//max value of 16bit signed integer


//settings of core.cpp

//#define file_output 
//#define once//to debug, no core loop
//#define SAVE_RECORDING  "./tmpres/record.bin"

//#define test_perf_mode //this mode allow the code to go as fast as possible au print result 
#define DEBUG_SAMPLE_RATE 1'000'000
#define TEST_PERF_TIME (10*1000000)

#define sch_log //scheduler log every time it is called for updates
#define GPU_calculation
//#define no_card_connected //no card is connected : use only the pc clock to get the current card segment
//#define RT_CALCULATION
//#define opti_prevent //just dum calculation to make sure the compiler does'nt generate code that overestimate the performances.
#define OCL_PLAT 1
//#endif

//card setting //TODO separate setting files
#define HW_BUFFER (32*4096*1024)
#define NOTIF_SIZE (4024*1024)

