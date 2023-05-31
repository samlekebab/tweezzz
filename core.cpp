using namespace std;
#include "mutex"
#include "thread"
#include "core.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <cstdio>
//card contains 2GSample memory 
#define SEGMENT_SIZE 6//DEBGUG 96 //number of sample per segment
#define BUFFER_SIZE 500'000  //number of segment in a cyclic buffer
#define SAFE_TICK (6*4)//DEBUG (3125*96)     //min delay (in ticks) between a push to the card and the actual play (directly related to the delay)
#define MAX_TICK (6*10)//DEBUG (10000*96) //how far we calculate in advance the segments (should'nt influence the delay)
#define MAX_VALUE 32767 	//max value of 16bit signed integer
			    //
#define file_output
#define no_card_connected
namespace coreCalc{
//DEBUG
#define DEBUG_SAMPLE_RATE 20
chrono::time_point<chrono::system_clock> timer;
void startTimer(){
	cout<<"timer start"<<endl;
	timer =chrono::system_clock::now();
}
long getTimer(chrono::time_point<chrono::system_clock> timer){
	return chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now()-timer).count();
}
int getCurrentCardSegment(){
	//TODO get the current card Segment
#ifdef no_card_connected
	return (int)(getTimer(timer)/1'000'000.0*DEBUG_SAMPLE_RATE);
#endif
	
}

void startCore(Scheduler& scheduler, Aom1D& aom1D, Aom2D& aom2D){
	//DEBUG
	startTimer();
	cout<<"a "<<getTimer(timer)<<endl;
	long currentTick{0};//estimated current tick played on the card
	int currentSegment{0};//current segment played on the card
#ifdef file_output
	FILE* f = fopen("./res/cpp.txt","w");
#endif

	while(1){
		//first, synchronize with the card to know where we are
		currentSegment = getCurrentCardSegment();//is this operation "synchron", is it slow? maybe not do it every time and estimated it with the computer clock instead? (need to look at the deviation)
		currentTick = findTickAssociatedToSegment(currentSegment,currentTick);

		//cout<<"current segment "<<currentSegment<<endl;
		//cout<<"currentTick "<<currentTick<<endl;
		
		//secondly, ask the sheduler what tick should be calculated next, convert it into a segment, check with SAFE_SEGMENT to deduce the segment to calculate
		//from now on, the scheduler will be locked
		const lock_guard<mutex> scheduler_lock(scheduler.usingScheduler_mutex);
		long tickToCompute = scheduler.nextTickToCompute;
		tickToCompute = max(currentTick+SAFE_TICK,tickToCompute);
		if (tickToCompute == currentTick + SAFE_TICK){
			printf("dropping at tick %d\n",tickToCompute);
		}
		tickToCompute = (int)(tickToCompute/SEGMENT_SIZE)*SEGMENT_SIZE;//put back at the begging of a segment
		
		//cout<<"tickToCompute "<<tickToCompute<<endl;

		//thirdly, calculate this segment, if we ar'nt over MAX_TICK
		if (tickToCompute<currentTick+MAX_TICK){
			int16_t buff[SEGMENT_SIZE];
			calculateSegment(scheduler,aom1D,aom2D,tickToCompute,buff);
			scheduler.nextTickToCompute = tickToCompute + SEGMENT_SIZE;
		

			//finaly, push the result in the card
			//TODO
#ifdef 	file_output
			for (int i{0};i<SEGMENT_SIZE;i++){
				fprintf(f,"%d\n",buff[i]);
			}
			fflush(f);
		}

#endif
		//DEBUG
		//this_thread::sleep_for(50ms);
		if (currentTick>10'000)
			continue;
		//this_tread::yield();//is it slowing down the process ?

	}


}

#define THREADS 1//number of concurent thread to compute the core
//approximation : because formgenerator are slow varying functions, we call them only once per segment 
//on the futur, we can change it and call it only once every THREAD number (because for now the scheduler can't be "threaded")
//the ultimate version would be to make the scheduler threadable
void calculateSegment(Scheduler& scheduler, Aom1D& aom1D, Aom2D& aom2D, long tick, int16_t* segment_buffer){
	//first, ask the scheduler to calculate the tick of begining of the segment
	scheduler.computeSample(tick);
	//TODO balance load on multiple thread
	//for now, we use only this thread
	for (int i{0};i<SEGMENT_SIZE;i++){
		calculate_tick(aom1D,aom2D,tick+i, segment_buffer[i]);
	}
		
}
void calculate_tick(Aom1D& aom1D,Aom2D& aom2D, long tick, int16_t& buff){
	//TODO the aom2D
	
	double sumAom1D{0.0};
	for (int i{0}; i<aom1D.tweezerCount;i++){
		const auto tw = *aom1D.tweezers[i];
		//TODO optimize by findind t' smaller, such as wt' = wt [2pi]
		//TODO what about the rapid phase change whene changing w?
		sumAom1D += tw.A*tw.N*sin(tw.w*(double) tick + tw.p);
	}
	buff = MAX_VALUE * sumAom1D/aom1D.tweezerCount * aom1D.A * aom1D.N;
	
}
inline int findSegmentAssociatedToTick(long tick){
	return (long)(tick/SEGMENT_SIZE) % BUFFER_SIZE;
}

long findTickAssociatedToSegment(int segment, long lastTick){
	int lastSegment = findSegmentAssociatedToTick(lastTick);
	long m = (long)(lastTick/SEGMENT_SIZE);
	//printf("seg %d, last seg %d, m %d, lastT %d\n",segment,lastSegment,m,lastTick);
	if(segment<lastSegment){
		m+=1;
	}
	return (m+(long)segment-lastSegment)*SEGMENT_SIZE;
}
}
