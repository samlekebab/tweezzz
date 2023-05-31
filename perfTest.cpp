#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include "core.h"
#include "scheduler.h"
#include "aom1D.h"
#include "aom2D.h"
#include "formGenerator.h"
#include <thread>
using namespace std;
class Rampup:public FormGenerator{
	

	public:
		struct Setting{
			int test;
			double duration;
		};
		Setting setting;
		double initial;
		Rampup(Setting setting):setting(setting){}
	//return the sample duration of the effect
		long getDuration(){
			return setting.duration;
		}
	//take a sampletime from the begining of and return a value between 0 and 1
		double calc(long time){
			
			cout<<"target "<<*target;
			*target = (double)time*1.0/setting.duration;
			return *target;
		
		}
		//if we want to take the value at the beginning to adapte our calculation
		
		void setBeginningValue(double value){}






};
float ramp(float x){
	float value = x/10000;
	if (value<100){
		return 100;
	}
	return value;
}
auto startTimer(){
	return chrono::system_clock::now();
}
long getTimer(auto timer){
	return chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now()-timer).count();
}
int test1(){
	auto now = startTimer();
	unsigned long i,j;
	float res(0.0f);
	srand(getTimer(now));
	cout<<(unsigned long)((long)-1)<<endl;
	long x = (getTimer(now) *1043 )% 3;
	srand(time(nullptr)+getTimer(now)+x);
	auto* rp = new Rampup({.test = (int)1.789234567e9});
	cout<<rp->calc(0)<<endl<<rp->getDuration()<<endl;
	/*
	while(getTimer(now)<30*1000000){
		i+=1;
		res += cos((float)(rand()));
		if (i==1000000){
			j+=1;
			i=0;
		}
	}
*/
	cout<<j<<" "<<res<<" "<<i<<endl;
	//cout<<rampup.calc(0)<<endl;
	return 0;
}

int test2(){
	coreCalc::startTimer();
	while(1){
		cout<<coreCalc::getCurrentCardSegment()<<endl;
	}
}
int test3(){
	Aom1D aom1D;
	Aom2D aom2D;
	Scheduler scheduler;
	coreCalc::startCore(scheduler,aom1D,aom2D);
	return 0;
}
void sequence(Aom1D& aom1D, Aom2D& aom2D){
	cout<<"begining of the sequence"<<endl;
	aom1D.A = 0.25;
	(new Rampup({.test = 0, .duration = 500}))->connect(aom1D.A);
}
int main(){
	//initialisation
	Scheduler scheduler;
	FormGenerator::scheduler = &scheduler;
	Aom1D aom1D; aom1D.A = 0.5;
	Aom2D aom2D;
	thread coreThread(coreCalc::startCore,ref(scheduler),ref(aom1D),ref(aom2D));


	//run
	thread sequence_thread(sequence, std::ref(aom1D), std::ref(aom2D));
	
	//end
	coreThread.join();

	cout<<"end of the program"<<endl;//this never happens
	return 0;
}
