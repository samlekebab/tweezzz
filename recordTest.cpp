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

#include "formUtils.h"


using namespace std;

void sequence(Aom1D& aom1D, Aom2D& aom2D){
	float bidon;

	/*
	for (int i=0;i<2;i++){
		aom1D.tweezers[i]->A = 1/2.0;
		aom1D.tweezers[i]->w  	= 70.0e6+i*0.15e6; //+ (i-50)*0.15e6;

		aom2D.V.tweezers[i]->A = 0;//1/100.0;
		aom2D.V.tweezers[i]->w  = 95.0e6+i*0.15e6; //+ (i-50)*0.15e6;

		aom2D.H.tweezers[i]->A = 1/2.0;
		aom2D.H.tweezers[i]->w  = 109.0e6+i*0.15e6; //+ (i-50)*0.15e6;
	}
	*/
	for (int i=0;i<100;i++){
		aom1D.tweezers[i]->A = 		0.00;
		aom2D.H.tweezers[i]->A = 	0.00;
		aom2D.V.tweezers[i]->A = 	0.00;
	}
	aom2D.V.tweezers[0]->A = 0.5;
	aom2D.V.tweezers[0]->w = 105e6;
	aom2D.H.tweezers[0]->A = 0.5;
	aom2D.H.tweezers[0]->w = 101e6;
	aom1D.tweezers[0]->A = 0.5;
	aom1D.tweezers[0]->w = 80e6;


	aom2D.V.tweezers[1]->A = 0.5;
	aom2D.V.tweezers[1]->w = 100e6;

	(new Marker())->connect(0,bidon);

	long duration = 380'000'000;
		(new Ramp({.duration = duration,.finalValue=95e6}))->connect(duration,aom2D.V.tweezers[0]->w);
		(new Ramp({.duration = duration,.finalValue=105e6}))->connect(3*duration,aom2D.V.tweezers[0]->w);

	for (int i=0;i<2;i++){
		(new Ramp({.duration = duration,.finalValue=(float)(111e6-i*21e6)}))->connect(1,aom2D.H.tweezers[i]->w);
		(new Ramp({.duration = duration,.finalValue=(float)(101e6-i*1e6)}))->connect(2*duration,aom2D.H.tweezers[i]->w);
	}
	(new Marker())->connect(4*duration,bidon);
}
int initAndStart(){
	//initialisation
	Aom1D aom1D; 
	Aom2D aom2D;
	Scheduler scheduler(aom1D,4);

	FormGenerator::scheduler = &scheduler;
	//printf("tw0->A %f\n",*aom1D.table);

	//start the core
	thread coreThread(coreCalc::startCore,ref(scheduler),sequence,ref(aom1D),ref(aom2D));
	
	//run the sequence
	thread sequence_thread(sequence, std::ref(aom1D), std::ref(aom2D));
	
	//end
	sequence_thread.join();
	cout<<"end of the sequence"<<endl;
	coreThread.join();

	cout<<"end of the program"<<endl;//this never happens
	return 0;
	
}
int main(){initAndStart();}


