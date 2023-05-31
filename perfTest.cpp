#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
using namespace std;
class Rampup{
	

	public:
		struct Setting{
			int test;
		};
		Setting setting;
		long duration{4};
		Rampup(Setting setting):setting(setting){}
	//return the sample duration of the effect
		int getDuration(){
			return duration;
		}
	//take a sampletime from the begining of and return a value between 0 and 1
		float calc(unsigned long time){
			
			return (float)setting.test;
		
	}





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
int main(){
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
