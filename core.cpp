using namespace std;
#include "mutex"
#include "thread"
#include "core.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include "mmath.h"


#include "setting.h"


			    //
//#define file_output
#define test_perf_mode //this mode push the code to go as fast as possible au print result 
#define no_card_connected
#define opti_prevent
namespace coreCalc{
//DEBUG
#define DEBUG_SAMPLE_RATE 100'000
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

float sinLut_t[10'000]{0};
float sinLut_t2[10'000]{0};
void startCore(Scheduler& scheduler, Aom1D& aom1D, Aom2D& aom2D){
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
	for (int i=0;i<10'000;i++){
		sinLut_t2[i]=sin(i*0.01);
	}
	
	//preparing the luts for "all" sin at all frequency
	mmath::fillUpTable();


	startTimer();
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
			//printf("dropping at tick %d\n",tickToCompute);
			drop++;
		}
		tickToCompute = (int)(tickToCompute/SEGMENT_SIZE)*SEGMENT_SIZE;//put back at the begging of a segment
		
		//cout<<"tickToCompute "<<tickToCompute<<endl;

		//thirdly, calculate this segment, if we ar'nt over MAX_TICK
		if (tickToCompute<currentTick+MAX_TICK){
			int16_t buff[4*SEGMENT_SIZE]{0};
			calculateSegment(scheduler,aom1D,aom2D,tickToCompute,buff);
			scheduler.nextTickToCompute = tickToCompute + SEGMENT_SIZE;
		

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
#endif
		}
#ifndef test_perf_mode
		//DEBUG
		//this_thread::sleep_for(500ms);
		if (currentTick>DEBUG_SAMPLE_RATE*(long)10){
			cout<<"end"<<endl<<drop<<" "<<(float)drop/DEBUG_SAMPLE_RATE/10.0*SEGMENT_SIZE*2<<endl;
			cout<<"count "<<endl<<counter<<" "<<(float)counter/DEBUG_SAMPLE_RATE/10.0<<endl;
			break;
		}
#else
		if (getTimer(timer)>10'000'000){
		//if (currentTick>50'000){
			startTimer();
			cout<<"count "<<counter<<endl;
			break;

		}
#endif
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
	
	//solution 1, for each tick, for each tweezer
	/*for (int i{0};i<SEGMENT_SIZE;i++){
		calculate_tick(aom1D,aom2D,tick+i, segment_buffer[i]);
	}*/

	//solution 2, for each tweezer, for each tick
	for (int i{0};i<aom1D.tweezerCount;i++){
		calculate_tweezer(aom1D,aom2D,i,tick,segment_buffer);
			
	}
		
}

bool sinLut_tb[10'000]{false};
int c=0;
double sinLut(double v){
	double id = v/3.141592653589793238462643383279/2
-long(v/(2*3.141592653589793238462643383279));
	int i_id = int(id*10'000);
	//cout<<"id :"<<id<<" "<<v/3.141592653589793238462643383279/2<<endl;
	//cout<<long(v/(2*3.141592653589793238462643383279))<<endl;
	//cout<<i_id<<" "<<sinLut_tb[i_id]<<endl;
	if (!sinLut_tb[i_id]){
		//cout<<++c<<endl;
		sinLut_t[i_id]=sin(v);
		sinLut_tb[i_id] = true;
	}	
	return sinLut_t[i_id];
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
	
}
inline int findSegmentAssociatedToTick(long tick){
	return (long)(tick/SEGMENT_SIZE) % BUFFER_SIZE;
}
long findTickAssociatedToSegment(int segment, long lastTick){
	int lastSegment = findSegmentAssociatedToTick(lastTick);
	long m = (long)(lastTick/(BUFFER_SIZE*SEGMENT_SIZE));
	//printf("seg %d, last seg %d, m %d, lastT %d\n",segment,lastSegment,m,lastTick);
	if(segment<lastSegment){
		m+=1;
	}
	return (m*BUFFER_SIZE+(long)segment)*SEGMENT_SIZE;
}
}
