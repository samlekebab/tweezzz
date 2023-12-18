#include "aom1D.h"
#include "aom2D.h"
#include "core.h"
#include "formGenerator.h"
#include "timeEvent.h"
#include "lib/dlltyp.h"
#include "lib/regs.h"
#include "lib/spcerr.h"
#include "lib/spcm_drv.h"
#include "mmath.h"
#include "scheduler.h"
#include <chrono>
#include <cmath>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <thread>
#include <time.h>
#include "card.h"
#include "asyncInput.h"
#include "waitTimeEvent.h"
#include "MasterLock.h"

#include "formUtils.h"

#include <cstdlib>
#define PI 3.1415926
using namespace std;
float frand(const float min,const float max){
	float r = rand()/(float)RAND_MAX;
	return min+max*r;
}
void sequence(Aom1D& aom1D, Aom2D& aom2D, MasterLock& masterLock){
	//initialisation
	float esp=0.45e6;
	const int nbPinces = 70;
	aom2D.H.tweezers[0]->A = 	1;
	aom2D.H.tweezers[0]->w = 100e6;
	aom2D.H.tweezers[0]->p = 0;

	for (int i=0;i<nbPinces;i++){
		aom1D.tweezers[i]->A = 		1/100.0;
		aom2D.V.tweezers[i]->A = 	1/100.0;

		aom1D.tweezers[i]->w = 75e6 + i*esp;
		aom2D.V.tweezers[i]->w = 90e6 + i*esp;

		float randPhase = frand(0,2*PI);
		aom1D.tweezers[i]->p = randPhase;
		aom2D.V.tweezers[i]->p = randPhase;
	}
	long durationWait = 100'000'000;
	long durationMove = 200'000'000;
	(new Marker())->connect(0,FVOID);


	//step1, we wait 
	(new Marker())->connect(durationWait,FVOID);
	
	//step2, randow draw and moving tweezers
	long detectionLatency = 10'000'000;
	long step2start = (new Marker())->connectRelative(detectionLatency,FVOID).start;
	int isAtom[nbPinces] = {0};
	
	do{//for the realtime loop

	//we stop this code until it is time to do the detection
	(new WaitTimeEvent())->connectRelativeAndWait(-detectionLatency);
	masterLock.tryLock();
	for (int i=0;i<nbPinces;i++){
		//dummy detection, random draw
		isAtom[i] = (int)frand(0.0,2.0);//50 percent
	}

	//then we move the tweezers or turn them off accordingly 
	int counter = 0;
	for (int i=0;i<nbPinces;i++){
		if (isAtom[i]){
			//moving tweezer i
			(new Ramp({.duration = durationMove,.finalValue = 75e6f + counter*esp }))->connect(step2start,aom1D.tweezers[i]->w);
			counter++;
		}else{

			//turning off tweezer i
			(new Setter({.value = 0.0}))->connect(step2start,aom1D.tweezers[i]->A);

		}


	}
	masterLock.unlock();
	
	//step3 rename the tweezers with the "Swap" formGenerator TODO

	//step4 we wait again
	long resetTime = (new Marker())->connectRelative(durationWait,FVOID).start;

	//for the real time loop 
	step2start = (new Marker())->connectRelative(durationWait,FVOID).start;
	//reset
	for (int i = 0; i<nbPinces;i++){

			//turning on tweezer i
			(new Setter({.value = 0.01f}))->connect(resetTime,aom1D.tweezers[i]->A);
			//reset freq
			(new Setter({.value = 75e6f + i*esp}))->connect(resetTime,aom1D.tweezers[i]->w);

	}


	}while(!FormGenerator::recordMode);
	
	//(new Ramp({.duration = 1'600'000'000,.finalValue = 90e6}))->connect(aom2D.V.tweezers[0]->w);
}

void sequenceTest(Aom1D& aom1D, Aom2D& aom2D, MasterLock& masterLock){

	aom1D.tweezers[0]->A =		0.01;
	aom2D.V.tweezers[0]->A =	0.3;
	aom2D.H.tweezers[0]->A =	0.3;

	aom1D.tweezers[0]->w =		75e6;
	aom2D.V.tweezers[0]->w =	90e6;
	aom2D.H.tweezers[0]->w = 	100e6;
	if (FormGenerator::recordMode==true){

		(new Marker())->connect(0,FVOID);
		long durationWait = 100'000'000;
		(new Marker())->connectRelative(durationWait,FVOID);

	}else{
		while(1){
			const long duration = 1'000'000'000;

			long start = (new Marker())->connect(FVOID).start;
			(new Ramp({.duration = duration,.finalValue = 75e6f + 10e6f }))->connect(start,aom1D.tweezers[0]->w);
			(new Ramp({.duration = duration,.finalValue = 75e6f }))->connect(aom1D.tweezers[0]->w);
			(new WaitTimeEvent())->connectRelativeAndWait(-duration);
			printf("loop");

		}


	}

}
int initAndStart(){
	//initialisation
	Aom1D aom1D; 
	Aom2D aom2D;
	Scheduler scheduler(aom1D,4);//4 means the log files will be res/sch_ch4.txt ("channel" number : 4 for realtime)

	FormGenerator::scheduler = &scheduler;
	//printf("tw0->A %f\n",*aom1D.table);

	MasterLock masterLock;

#define SEQUENCE sequence
	//start the core
	thread coreThread(coreCalc::startCore,ref(scheduler),SEQUENCE,ref(aom1D),ref(aom2D),ref(masterLock));
	
	//run the sequence
	thread sequence_thread(SEQUENCE, std::ref(aom1D), std::ref(aom2D),ref(masterLock));
	
	//end
	sequence_thread.join();
	cout<<"end of the sequence"<<endl;
	coreThread.join();

	cout<<"end of the program"<<endl;//this never happens
	return 0;
	
}

int main(){initAndStart();}


