#include "mmath.h"
#include <cmath>
#include <numbers>
#include <iostream>
namespace mmath{	

	float sin[SIN_TAB_SIZE*LENGTH_PER_FREQ]{0};
	double range = 30e6;
	double startFreq = 70e6-range/2;

	void fillUpTable();

	void fillUpTable(){
		const int size = SIN_TAB_SIZE*LENGTH_PER_FREQ;
		for(int i=0;i<size;i+=LENGTH_PER_FREQ){
			double freq = startFreq + range * i / (double)size;
			//std::cout<<"freq "<<freq<<std::endl;
			for (int j=0;j<LENGTH_PER_FREQ;j++){
				sin[i + j]=std::sin(2*std::numbers::pi_v<float>*freq*j/(double)SAMPLE_RATE);
			}
		}
	}
	int lut(double freq){
		const int size = SIN_TAB_SIZE*LENGTH_PER_FREQ;
		return (int)((freq-startFreq)*size / range);
	}
}
