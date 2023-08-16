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

using namespace std;
class Rampup:public FormGenerator{
	

	public:
		struct Setting{
			long duration;
			float finalValue=1.0;
		};
		Setting setting;
		float initial;
		Rampup(Setting setting):setting(setting){
			setTag("rampup");
		}
		//returns the duration in sample of the FormGenerator
		long getDuration(){
			return setting.duration;
		}

		//take a sampletime with the beginning as origin and return a value between 0 and 1
		float calc(long time){
			*target = initial + (float)time/setting.duration * (setting.finalValue - initial);
			//printf("%f\n",*target);
			return *target;
		
		}

		//if we want to take the value at the beginning to adapte our calculation
		void setBeginningValue(float value){initial = value;}
};

class MesurementTimeEvent:public TimeEvent{
	public: 
		struct Setting{
			AsyncInput& input;
		};
		Setting setting;
		MesurementTimeEvent(Setting setting):setting(setting){
			setTag("MesurementTimeEvent");
		}
		float calc(long tick){
			//TODO mesure async input X0,1,2
			//DEBUG (?)
			int input = Card::card->readInput();
			printf("input read : %d\n",input);
			//TODO put the result in setting.input.X0,1,2
			setting.input.X0 = input & 0b001; 
			setting.input.X1 = input & 0b010; 
			setting.input.X2 = input & 0b100; 
			return 0;
		}
};
void sequence(Aom1D& aom1D, Aom2D& aom2D){
	//cout<<"begining of the sequence"<<endl;
	AsyncInput input;
	float bidon;
	aom1D.A = 0.25;
	for (int i=0;i<10000;i++){
		if (input.X0){
			(new Rampup({ .duration = 1'000'0,.finalValue = 1}))->connect(aom1D.A);
		}else{
			(new Rampup({ .duration = 1'000'0,.finalValue = 0}))->connect(aom1D.A);
		}
		(new MesurementTimeEvent({.input = input}))->connectRelative(-500'000,bidon);
		(new WaitTimeEvent())->connectAndWait();
	}
}
void sequenceTestRewind(Aom1D& aom1D, Aom2D& aom2D){
	AsyncInput input;
	float bidon;
	aom1D.A = 0.25;

	(new Rampup({.duration = 1'000'000,.finalValue=1}))->connect(aom1D.A);
	(new MesurementTimeEvent({.input = input}))->connectRelative(-500'000,bidon);
	(new WaitTimeEvent())->connectRelativeAndWait(-400'000);
	(new Rampup({ .duration = 1'000'000,.finalValue = 0.5}))->connectRelative(-400'000,aom1D.A);
}
int initAndStart(){
	//initialisation
	Aom1D aom1D; aom1D.A = 0.5;
	Scheduler scheduler(aom1D);

	FormGenerator::scheduler = &scheduler;

	for (int i=0;i<100;i++){
		aom1D.tweezers[i]->A = 1/100.0;
		aom1D.tweezers[i]->w += (i-50)*0.15e6;
	}
	for (int i=100;i<100;i++){
		aom1D.tweezers[i]->A = 0.0;
	}
	Aom2D aom2D;
	printf("tw0->A %f\n",*aom1D.table);

	//start the core
	thread coreThread(coreCalc::startCore,ref(scheduler),ref(aom1D),ref(aom2D));
	
	//run the sequence
	thread sequence_thread(sequenceTestRewind, std::ref(aom1D), std::ref(aom2D));
	
	//end
	sequence_thread.join();
	cout<<"end of the sequence"<<endl;
	coreThread.join();

	cout<<"end of the program"<<endl;//this never happens
	return 0;
	
}
int main(){initAndStart();}
