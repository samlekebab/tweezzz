using namespace std;
using namespace coreCalc;
#include "mutex"
#include "thread"
#include "core.h"
#include <cmath>
//card contains 2GSample memory 
#define SEGMENT_SIZE 96 //number of sample per segment
#define BUFFER_SIZE 500'000  //number of segment in a cyclic buffer
#define SAFE_TICK (3125*96)     //min delay (in ticks) between a push to the card and the actual play
#define MAX_TICK (10000*96) //how far we calculate in advance the segments (should'nt influence the delay)
void startCore(Scheduler& scheduler, Aom1D& aom1D, Aom2D& aom2);{
	
	long lastCalculatedTick{0};
	long currentTick;//estimated current tick played on the card
	int currentSegment;//current segment played on the card
		 
	while(1){
		//first, synchronise with the card to know where we are
		currentSegment = getCurrentCardSegment();//is this operation "synchron", is it slow? maybe not do it every time and estimated it with the computer clock instead? (need to look at the deviation)
		currentTick = findTickAssociatedToSegment(currentSegment,curentTick);


		
		//secondly, ask the sheduler what tick should be calculated next, convert it into a segment, check with SAFE_SEGMENT to deduce the segment to calculate
		//from now on, the scheduler will be locked
		const lock_guard<mutex> scheduler_lock(usingScheduler_mutex);
		long tickToCompute = scheduler.nextTickToCompute;
		tickToCompute = max(currentTick+SAFE_TICK,tickToCompute)
		if (tickToCompute == currentTick + SAFE_TICK){
			cout<<"dropping at tick "<<tickToCompute<<"\n";
		}
		tickToCompute = (tickToCompute%SEGMENT_SIZE)*SEGMENT_SIZE;//put back at the begging of a segment
		

		//thirdly, calculate this segment
		int[SEGMENT_SIZE] buff;
		calculateSegment(scheduler,aom1D,aom2D,tickToCompute,buff);

		lastCalculatedSegment = segmentToCalculate;
		scheduler.nextTickToCompute = tickToCompute + SEGMENT_SIZE;

		//this_tread::yield();//is it slowing down the process ?
	}


}

#define THREADS 1
//approximation : because formgenerator are slow varying functions, we call them only once per segment 
//on the futur, we can change it and call it only once every THREAD number (because for now the scheduler can't be "threaded")
//the ultimate version would be to make the scheduler threadable
void calculateSegment(Scheduler& scheduler, Aom1D& aom1D, Aom2D& aom2D, long tick, int16_t[] segment_buffer){
	//first, ask the scheduler to calculate the tick of begining of the segment
	scheduler.computeSample(tick);
	//TODO balance load on multiple thread
	//for now, we use only this thread
	for (int i{0},i<SEGMENT_SIZE;i++){
		calculate_tick(aom1D,aom2D,tick+i, segment_biffer[i]);
		
}
void calculate_tick(Aom1D& aom1D,Aom2D& aom2D, long tick, int16_t& buff){
	//TODO the aom2D
	
	double sumAom1D{0.0};
	for (int i{0}; i<aom1D.tweezerCount();i++){
		const auto tw = tweezer[i]
		//TODO optimize by findind t' smaller, such as wt' = wt [2pi]
		//TODO what about the rapid phase change whene changing w?
		sumAom1D += tw.A*tw.N*sin(tw.w*(double tick) + tw.p)
	
	
}
int getCurrentCardSegment(){
	//TODO
}

