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
#include "formGenerator.h"
#include <cstring>

using namespace std;


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

#ifdef no_card_connected
int getCurrentCardSegment(){
	//TODO get the current card Segment
#ifndef test_perf_mode
	return (int)(getTimer(timer)/1'000'000.0*DEBUG_SAMPLE_RATE/(float)SEGMENT_SIZE)%BUFFER_SIZE;
#else
	curSeg +=1;
	curSeg %= BUFFER_SIZE;
	return curSeg;
#endif
	
}
#else
int getCurrentCardSegment(Card& card){
	//card.updateEstimation();
	//printf("estimated card tick : %ld\n",card.tick);
	return (card.tick/(long)SEGMENT_SIZE)%BUFFER_SIZE;
}
void cardUpdater(Card& card){//, mutex& cardMutex){
        while(1){
                //cardMutex.lock();
                card.updateEstimation();
                //cardMutex.unlock();
                //this_thread::yield();
        }
}

#endif

void startCore(Scheduler& scheduler,void (*sequence)(Aom1D&,Aom2D&), Aom1D& aom1D, Aom2D& aom2D, MasterLock& masterLock){
#ifndef no_card_connected
	Card card;
#else
	int16_t* g_buffer = new int16_t[SEGMENT_SIZE * BUFFER_SIZE * 4];
#endif

#ifdef GPU_calculation
	printf("GPU calculation");
	//CoreGPU gpu1;//TODO use a qeue
	CoreGPU gpu2;
#else
	printf("CPU calculation code is'nt up to date !!");//TODO (see below)
#endif

	//DEBUG
	long currentTick{0};//estimated current tick played on the card
	int currentSegment{0};//current segment played on the card
	long drop=0;
	long counter=0;

#ifdef file_output
	FILE* f = fopen("./res/cpp.txt","w");
#endif
	
#ifdef CPU_calculation
	//preparing the luts for "all" frequencies
	//TODO is it used anymore ? 
	mmath::fillUpTable();
#endif

	//TODO do the calculation of recorded signal
	//for the moment, no CPU calculation, TODO should it be maintained 
	Aom* aomTable[3] = {&aom2D.V, &aom2D.H, &aom1D};
#ifdef GPU_calculation
	size_t maxLength = 0;
	printf("record\n");
	int16_t* recording;
	FormGenerator::recordMode = true;

	//this is not greate because we are calling the sequence 3 times...
	//TODO any idea to improve ?
	for (int i=0;i<3;i++){
		printf("aomTable %d\n",i);
		//the aom given here could help debug sequence if we recover the aomhistory of this scheduler.
		Scheduler recordScheduler(*aomTable[i],i);
		FormGenerator::scheduler = &recordScheduler;


		//first we put the sequence in the scheduler
		sequence(aom1D,aom2D);

		//allocation of memory
		if (i==0){
			printf("allocation of %ld samples * 4 channels for the recording = %f GB\n",recordScheduler.EOFT,recordScheduler.EOFT* 2 * 4/(float)1073741824);
			printf("size of int16_t : %ld\n",sizeof(int16_t));
			recording = new int16_t[4 * (recordScheduler.EOFT+SEGMENT_SIZE)];
			memset((void*)recording,0,4 * (recordScheduler.EOFT+SEGMENT_SIZE) * 2);
			maxLength = recordScheduler.EOFT;
		}
		
		//then we compute the sequence until the scheduler is empty
		for (long tick=0;tick<recordScheduler.EOFT;tick+=SEGMENT_SIZE){
			calculateGPU(recordScheduler,*aomTable[i],tick,gpu2,gpu2.outBuffer);
			copyTo4chBuff(&recording[4*tick+i],gpu2.outBuffer,SEGMENT_SIZE,aomTable[i]->N*aomTable[i]->A);
		}

		printf("aomTable %d done\n",i);
	}
	
	//save the record on file
#ifdef SAVE_RECORDING
	auto path = SAVE_RECORDING;
	printf("saving recording in binnary format, path : %s\n",path);
	auto recordFile = fopen(path,"wb");
	fwrite(recording,2,4*(maxLength+SEGMENT_SIZE),recordFile);
	fclose(recordFile);
#endif

	//start the recordDispenser
#ifndef no_card_connected
	thread recordDispenser = thread(&Card::recordDispenser,&card,recording,maxLength);
#endif

	//back to normal, ie real time mode
	FormGenerator::recordMode = false;
	FormGenerator::scheduler = &scheduler;

#endif

#ifndef no_card_connected
	card.initTransfert();
#endif

	this_thread::yield();
	printf("start\n");

#ifdef no_card_connected
	startTimer();
#else
	card.start();
	std::thread cardUpdaterThread1(cardUpdater,ref(card));
#endif


	int debug=0;//DEBUG
	while(1){
		//step 0, check the master lock to make sur the sequence the sequence is writen
		masterLock.testAndWaitUnlock();

		//first, synchronize with card to know when we are, call time related events
#ifdef no_card_connected
		currentSegment = getCurrentCardSegment();
#else
		currentSegment = getCurrentCardSegment(card);
#endif
		currentTick = findTickAssociatedToSegment(currentSegment,currentTick);
		
		//update the Time related events 
		scheduler.callTimeEvent(currentTick);
		
		//secondly, ask the scheduler what tick should be calculated next check with SAFE_SEGMENT to deduce the segment to calculate, convert it into a segment number
		//lock the scheduler
		//TODO release the scheduler lock before starting the calculation on the GPU(or CPU)
		const lock_guard<mutex> scheduler_lock(scheduler.usingScheduler_mutex);
		long tickToCompute = scheduler.nextTickToCompute;

		//in case we drop behind TODO code an "emergency" stop (or beter notification system ?) if the drop happen at critical time (when we manipulate atomes
		tickToCompute = max(currentTick+(long)SAFE_TICK,tickToCompute);
		if (tickToCompute == currentTick + SAFE_TICK){
			//printf("dropping at tick %ld\n",tickToCompute);
			drop++;
			//put at the begining of the next segment
			tickToCompute = ((tickToCompute-1)/SEGMENT_SIZE + 1)*SEGMENT_SIZE;

		}else{
			//put back at the begging of a segment
			tickToCompute = (tickToCompute/SEGMENT_SIZE)*SEGMENT_SIZE;
		}
		int segmentToFill = findSegmentAssociatedToTick(tickToCompute);
		

		//thirdly, calculate this segment, if we ar'nt over MAX_TICK
		//and put in the buffer to send to the card
		if (tickToCompute<currentTick+MAX_TICK){//TODO add a "next branch tick" condition that ask the sheduler when the next branch might happend, because no need to compute further that that because there will be a rewind anyway
			int tickOfBuffer = segmentToFill * SEGMENT_SIZE * 4;
#ifdef no_card_connected
			int16_t* buff = &g_buffer[tickOfBuffer];
#else 
			int16_t* buff = &card.buffer[tickOfBuffer];
#endif

		
			
//TODO implement the selection of which channel compute at compile time (and then channel change during sequence -> create a phase adapter (or full adapter) to prevent phase (or signal ?) jump when returning to recorded signal)
#ifdef RT_CALCULATION
#ifdef GPU_calculation
			int RTChannel = 2;//TODO export channel out of this scope or in setting ? cf precedent TODO comment
			int16_t* toCopy = calculateGPU(scheduler,*aomTable[RTChannel],tickToCompute,gpu2,gpu2.outBuffer);
			copyTo4chBuff(&buff[RTChannel],gpu2.outBuffer,SEGMENT_SIZE,aomTable[RTChannel]->N*aomTable[RTChannel]->A);

			//DEBUG
			/*
			for (int i=0;i<SEGMENT_SIZE*4;i++){
				//buff[i] = aom1D.A * aom1D.N * toCopy[i];
				buff[i] = aom1D.A * (int16_t)(debug*500);
				if (tickOfBuffer/SEGMENT_SIZE == 0){
					buff[i] = 0;
				}
			}*/
			debug++;
		

#endif
#ifdef CPU_calculation
			calculateCPU(scheduler, aom1D, aom2D, tickToCompute, buff);
#endif
#endif

			//notify the scheduler that we computed this tick
			scheduler.nextTickToCompute = tickToCompute + SEGMENT_SIZE;
			//TODO add another rollback check -> print any negative result (and abbort ?)

			//depending on wich setting we are, we save data, count MS/s, stop the loop etc..

#ifdef 	file_output
			for (int i{0};i<SEGMENT_SIZE;i++){
				fprintf(f,"%d\n",buff[i]);
			}
			fflush(f);
#endif


			counter+=SEGMENT_SIZE;

		}else{
			//printf("max tick reached\n");
		}
#ifdef no_card_connected
#ifndef test_perf_mode
		if (currentTick>DEBUG_SAMPLE_RATE*(long)10){//10 seconds
			cout<<"end"<<endl<<drop<<" "<<(float)drop/DEBUG_SAMPLE_RATE/10.0*SEGMENT_SIZE*2<<endl;
			cout<<"count "<<endl<<counter<<" "<<(float)counter/DEBUG_SAMPLE_RATE/10.0<<endl;
			cout<<"MS/s "<<endl<<2*(float)counter/1'000'000/10<<" "<<(float)counter/DEBUG_SAMPLE_RATE/10.0<<endl;
			break;
		}
#else
		if (getTimer(timer)>TEST_PERF_TIME){//10 seconds
		//if (currentTick>50'000){
			startTimer();
			cout<<"count "<<counter<<endl;
			cout<<"MS/s "<<(float)counter/1'000'000/10<<endl;
			break;

		}
#endif
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
	//slower than solution 2
	/*for (int i{0};i<SEGMENT_SIZE;i++){
		calculate_tick(aom1D,aom2D,tick+i, segment_buffer[i]);
	}*/

	//solution 2, for each tweezer, for each tick
	for (int i{0};i<aom1D.tweezerCount;i++){
		calculate_tweezer(aom1D,aom2D,i,tick,segment_buffer);
			
	}
		
}
//TODO now segments are very long and SEGMENT_SIZE / segment by segment calculation should be replace by BATCH_SIZE
//TODO linar interpolation (of w and A?) (to do also in GPU calculation)
void calculate_tweezer(Aom1D& aom1D, Aom2D& aom2D,int tweezer, long initial_tick, int16_t* buff){
	const auto tw = *aom1D.tweezers[tweezer];
	const auto w{tw.w},p{tw.p};
	const int16_t f = MAX_VALUE*(tw.A*tw.N*aom1D.A*aom1D.N/aom1D.tweezerCount);
	const int b=mmath::lut(w);
	//find the phase
	int t2 = fmod(initial_tick,SAMPLE_RATE/(float)w);
	//TODO convert (??) and add fixed phase p

	const float* table = &mmath::sin[b+t2];
	
	for (int i{0};i<SEGMENT_SIZE;i++){
		//vectorizable with a dl approxiation of the sinus?
		//or with lut like i tryed
		int16_t tmp  = f * table[i]; 

		//other versions of this line : 
		/*sinLut_t2[1+i]; //sin(w*((double)i+SEGMENT_SIZE)/6.0e8 + p);*/

		//TODO channel choice (currently ch0 only)
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

int16_t* calculateGPU(Scheduler& scheduler, Aom& aom, long tickToCompute,CoreGPU& gpu,int16_t* buffer){
	
	for(int i=0;i<(SEGMENT_SIZE/BATCH_SIZE);i++){
		//recompute the scheduler: 
		scheduler.computeSample(tickToCompute + i*BATCH_SIZE);
		//send aom to the gpu
		gpu.setParams(aom,i);
		//compute the accumulated phase during a batch TODO fix phase shift from low accuracy of float
		for(int j=0;j<aom.tweezerCount;j++){
			float& pr = aom.tweezers[j]->pr ;
			float& w = aom.tweezers[j]->w ;
			float& a = aom.tweezers[j]->A ;
			pr += fmod(w * (2*3.14159265358979323846*(BATCH_SIZE)/(double)SAMPLE_RATE)  , 2*3.14159265358979323846);
		//printf("pr%d : %f\n",j,pr);
		}
	}
	//calculate the segment
	return gpu.calculate(tickToCompute, buffer);
	
}
void copyTo4chBuff(int16_t* dest,int16_t* src,size_t length,float factor){
	for (size_t i=0;i<length;i++){
		dest[4*i] = factor * src[i];
	}
}
}


