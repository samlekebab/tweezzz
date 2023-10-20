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
	(new Marker())->connect(400'000'000,bidon);
}
int initAndStart(){
	//initialisation
	Aom1D aom1D; 
	Aom2D aom2D;
	Scheduler scheduler(aom1D);

	FormGenerator::scheduler = &scheduler;

	for (int i=0;i<100;i++){
		aom1D.tweezers[i]->A = 1/100.0;
		aom1D.tweezers[i]->w += (i-50)*0.15e6;
	}
	for (int i=0;i<100;i++){
		aom2D.V.tweezers[i]->A = 1/100.0;
		aom2D.V.tweezers[i]->w += (i-50)*0.15e6;
	}
	for (int i=0;i<100;i++){
		aom2D.H.tweezers[i]->A = 1/100.0;
		aom2D.H.tweezers[i]->w += (i-50)*0.15e6;
	}
	for (int i=100;i<100;i++){
		aom1D.tweezers[i]->A = 0.0;
	}
	printf("tw0->A %f\n",*aom1D.table);

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
