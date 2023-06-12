#include "mutex"
#include "thread"
#include "core.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include "mmath.h"
#include "coregpu.h"

#include "setting.h"

using namespace std;


//#define file_output
//#define once//to debug, no core loop
#define test_perf_mode //this mode push the code to go as fast as possible au print result 
#define DEBUG_SAMPLE_RATE 100'000

#define GPU_calculation
#define no_card_connected //no card is connected : use only the pc clock to get the current card segment
//#define opti_prevent //just dum calculation to make sure the compiler does'nt generate code that overestimate the performances.
namespace coreCalc{



chrono::time_point<chrono::system_clock> timer;

void startTimer(){
	cout<<"timer start"<<endl;
	timer =chrono::system_clock::now();
}
long getTimer(chrono::time_point<chrono::system_clock> timer){
	return chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now()-timer).count();
}
#ifdef test_perf_mode
long curSeg=0;
#endif
int getCurrentCardSegment(){
	//TODO get the current card Segment
#ifdef no_card_connected
#ifndef test_perf_mode
	return (int)(getTimer(timer)/1'000'000.0*DEBUG_SAMPLE_RATE/(float)SEGMENT_SIZE)%BUFFER_SIZE;
#else
	curSeg +=1;
	curSeg %= BUFFER_SIZE;
	return curSeg;
#endif
#endif
	
}

//not used anymore, use mmath::sin which precalculate a discrete set of frequencies
//or write something that recalculate dynamicaly "new" frequencies that are encounetered
//this simpler solution of lut values of the sinus does is not as fast in cpu mode (because it prevent auto-vectorizatation ?)
//float sinLut_t[10'000]{0};
//float sinLut_t2[10'000]{0};
//bool sinLut_tb[10'000]{false};
//int c=0;
//double sinLut(double v){
//	double id = v/3.141592653589793238462643383279/2
//-long(v/(2*3.141592653589793238462643383279));
//	int i_id = int(id*10'000);
//	//cout<<"id :"<<id<<" "<<v/3.141592653589793238462643383279/2<<endl;
//	//cout<<long(v/(2*3.141592653589793238462643383279))<<endl;
//	//cout<<i_id<<" "<<sinLut_tb[i_id]<<endl;
//	if (!sinLut_tb[i_id]){
//		//cout<<++c<<endl;
//		sinLut_t[i_id]=sin(v);
//		sinLut_tb[i_id] = true;
//	}	
//	return sinLut_t[i_id];
//}

void startCore(Scheduler& scheduler, Aom1D& aom1D, Aom2D& aom2D){
#ifdef GPU_calculation
	printf("GPU calculation");
	CoreGPU gpu1;//TODO use a qeue
	CoreGPU gpu2;
#else
	printf("CPU calculation");
#endif

	//DEBUG
	cout<<"a "<<getTimer(timer)<<endl;
	long currentTick{0};//estimated current tick played on the card
	int currentSegment{0};//current segment played on the card
	long sum=0;
	long drop=0;
	long counter=0;

#ifdef file_output
	FILE* f = fopen("./res/cpp.txt","w");
#endif
	
	//preparing the luts for "all" frequencies
	mmath::fillUpTable();


#ifdef no_card_connected
	startTimer();
	printf("start\n");
#else
#endif

	while(1){
		//first, synchronize with the card to know where we are
		currentSegment = getCurrentCardSegment();//is this operation "synchron", is it slow? maybe not do it every time and estimated it with the computer clock instead? (need to look at the deviation)
		currentTick = findTickAssociatedToSegment(currentSegment,currentTick);
		//cout<<"current segment "<<currentSegment<<endl;
		//cout<<"currentTick "<<currentTick<<endl;
		
		//secondly, ask the sheduler what tick should be calculated next, convert it into a segment, check with SAFE_SEGMENT to deduce the segment to calculate

		//lock the scheduler
		//TODO release the scheduler before starting the calculation on the GPU(or CPU)
		const lock_guard<mutex> scheduler_lock(scheduler.usingScheduler_mutex);
		long tickToCompute = scheduler.nextTickToCompute;
		tickToCompute = max(currentTick+SAFE_TICK,tickToCompute);
		if (tickToCompute == currentTick + SAFE_TICK){
			//printf("dropping at tick %d\n",tickToCompute);
			drop++;
		}
		tickToCompute = (int)(tickToCompute/SEGMENT_SIZE)*SEGMENT_SIZE;//put back at the begging of a segment
		
		//cout<<"tickToCompute "<<tickToCompute<<endl;

		//thirdly, calculate this segment, if we ar'nt over MAX_TICK
		if (tickToCompute<currentTick+MAX_TICK){
#ifdef GPU_calculation
			int16_t* buff;
			buff = calculateGPU(scheduler,aom1D,aom2D,tickToCompute,gpu2);
			//printf("b %d : gpu outbuffer 0 %d\n",counter,buff[938]);
#else
			int16_t buff[4*SEGMENT_SIZE]{0};
			calculateCPU(scheduler, aom1D, aom2D, tickToCompute, buff);
#endif
			scheduler.nextTickToCompute = tickToCompute + SEGMENT_SIZE;

//			for (size_t i=0;i<96000;i++){
//				printf("res %d : %d\n",i,gpu.outBuffer[i]);
//			}
		

			//finaly, push the result in the card
			//TODO
#ifdef 	file_output
			for (int i{0};i<SEGMENT_SIZE;i++){
				fprintf(f,"%d\n",buff[i]);
			}
			fflush(f);
#endif


#ifdef opti_prevent
			for (int i{0};i<SEGMENT_SIZE;i++){
				counter++;
				sum+=buff[4*i];
			}
			if (sum%1234567==0){
				cout<<"surprise : "<<sum<<endl;
			}
#else
			counter+=SEGMENT_SIZE;
#endif

		}

#ifndef test_perf_mode
		if (currentTick>DEBUG_SAMPLE_RATE*(long)10){//10 seconds
			cout<<"end"<<endl<<drop<<" "<<(float)drop/DEBUG_SAMPLE_RATE/10.0*SEGMENT_SIZE*2<<endl;
			cout<<"count "<<endl<<counter<<" "<<(float)counter/DEBUG_SAMPLE_RATE/10.0<<endl;
			cout<<"MS/s "<<endl<<2*(float)counter/1'000'000/10<<" "<<(float)counter/DEBUG_SAMPLE_RATE/10.0<<endl;
			break;
		}
#else
		if (getTimer(timer)>10'000'000){//10 seconds
		//if (currentTick>50'000){
			startTimer();
			cout<<"count "<<counter<<endl;
			cout<<"MS/s "<<(float)counter/1'000'000/10<<endl;
			break;

		}
#endif
#ifdef once
		break;
#endif
		//this_tread::yield();//is it slowing down the process ?

	}


}
void calculateCPU(Scheduler& scheduler, Aom1D& aom1D, Aom2D& aom2D,long tickToCompute, int16_t* buff){
			calculateSegment(scheduler,aom1D,aom2D,tickToCompute,buff);
}

//TODO implement multithreading for CPU calculation (I was thinking 1 thread per segment of data calculated)
#define THREADS 1//number of concurent thread to compute the core
//approximation : because formgenerators are slow varying functions, we call them only once per segment 
//on the futur, we can change it and call it only once every THREAD number (because for now the scheduler can't be "threaded")
//the ultimate version would be to make the scheduler threadable
void calculateSegment(Scheduler& scheduler, Aom1D& aom1D, Aom2D& aom2D, long tick, int16_t* segment_buffer){
	//first, ask the scheduler to calculate the tick of begining of the segment
	scheduler.computeSample(tick);
	//TODO balance load on multiple thread
	//for now, we use only this thread
	
	//solution 1, for each tick, for each tweezer
	/*for (int i{0};i<SEGMENT_SIZE;i++){
		calculate_tick(aom1D,aom2D,tick+i, segment_buffer[i]);
	}*/

	//solution 2, for each tweezer, for each tick
	for (int i{0};i<aom1D.tweezerCount;i++){
		calculate_tweezer(aom1D,aom2D,i,tick,segment_buffer);
			
	}
		
}
void calculate_tweezer(Aom1D& aom1D, Aom2D& aom2D,int tweezer, long initial_tick, int16_t* buff){
	const auto tw = *aom1D.tweezers[tweezer];
	const auto w{tw.w},p{tw.p};
	const int16_t f = MAX_VALUE*(tw.A*tw.N*aom1D.A*aom1D.N/aom1D.tweezerCount);
	const int b=mmath::lut(w);
	//find the phase
	int t2 = fmod(initial_tick,600'000'000/(float)w);
	//TODO convert and add fixed phase p

	const float* table = &mmath::sin[b+t2];
	
	for (int i{0};i<SEGMENT_SIZE;i++){
		//vectorizable with a dl approxiation of the sinus?
		//or with lut like i tryed
		int16_t tmp  = f * table[i]; 
		/*sinLut_t2[1+i]; //sin(w*((double)i+SEGMENT_SIZE)/6.0e8 + p);*/


		buff[4*i] += tmp;

	}

}
/*not used anymore. replaced by calculate_tweezer
void calculate_tick(const Aom1D aom1D,const Aom2D aom2D,const long tick, int16_t& buff){
	//TODO the aom2D
	
	double sumAom1D{0.0};
	for (int i{0}; i<aom1D.tweezerCount;i++){
		const auto tw = *aom1D.tweezers[i];
		//TODO optimize by findind t' smaller, such as wt' = wt [2pi]
		//TODO what about the rapid phase change whene changing w?
		sumAom1D += tw.A*tw.N*sinLut(tw.w*(double)tick/6.0e8 + tw.p);
	}
	buff = MAX_VALUE * sumAom1D/aom1D.tweezerCount * aom1D.A * aom1D.N / aom1D.tweezerCount;
	
}*/
inline int findSegmentAssociatedToTick(long tick){
	return (long)(tick/SEGMENT_SIZE) % BUFFER_SIZE;
}
long findTickAssociatedToSegment(int segment, long lastTick){
	int lastSegment = findSegmentAssociatedToTick(lastTick);
	long m = (long)(lastTick/(BUFFER_SIZE*(long)SEGMENT_SIZE));
	//printf("seg %d, last seg %d, m %d, lastT %d\n",segment,lastSegment,m,lastTick);
	if(segment<lastSegment){
		m+=1;
	}
	return (m*BUFFER_SIZE+(long)segment)*SEGMENT_SIZE;
}

int16_t* calculateGPU(Scheduler& scheduler, Aom1D& aom1D, Aom2D& aom2D,long tickToCompute,CoreGPU& gpu){
	
	for(int i=0;i<(SEGMENT_SIZE/BATCH_SIZE);i++){
		//recompute the scheduler: 
		scheduler.computeSample(tickToCompute + i*BATCH_SIZE);
		//compute the accumulated phase during a batch
		for(int j=0;j<100;j++){//TODO hard codded tweezer number
			float& pr = aom1D.tweezers[j]->pr ;
			float& w = aom1D.tweezers[j]->w ;
			pr += fmod(w * (2*3.14159*BATCH_SIZE/SAMPLE_RATE)  , 2*3.14159);
			//printf("pr%d : %f\n",j,w);
		}
		//send it to the gpu
		gpu.setParams(aom1D,aom2D,i);
	}
	//calculate the segment
	return gpu.calculate(tickToCompute);
	
}
}
