#include <iostream>
#include <list>
#include "mmath.h"
#include <stdio.h>
#include <cmath>
#include <numbers>
using namespace std;
int main(){
	//FILE* f = fopen("./res/sin.txt","w");
	mmath::fillUpTable();
	float freq = 66.5e6;
	int i = mmath::lut(freq);
	cout<<"i "<<i<<endl;
	for (int j{0};j<LENGTH_PER_FREQ;j++){
			float* table = &mmath::sin[i];
			float realSin = std::sin(2*std::numbers::pi_v<float>*freq*j/(double)SAMPLE_RATE);
			printf("%f %d %f %f \n",
					freq*j,
					j,
					table[j],
					table[j]-realSin);
	}
	//fflush(f);
	while(1);
	return 0;
}
