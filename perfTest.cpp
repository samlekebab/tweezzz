#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
using namespace std;
class rampup{
	





//return the sample duration of the effect
	int getDuration(){
		return duration
	}
//take a sampletime from the begining of and return a value between 0 and 1
	float calc(unsigned long time){
		
		return 1.0f;
	
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
	while(getTimer(now)<30*1000000){
		i+=1;
		res += cos((float)(rand()));
		if (i==1000000){
			j+=1;
			i=0;
		}
	}

	cout<<j<<endl<<res<<endl<<i;
	return 0;
}
