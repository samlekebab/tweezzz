#include "mmath.h"
#include <cmath>
#include <numbers>
namespace mmath{
	void fillUpTable(){
		const size = SIN_TAB_SIZE*LENGTH_PER_FREQ
		for(int i=0;i<size;i++){
			double freq = startFreq + range * i / (double)size;
			cout<<"freq "<<freq;
			for (int j=0;j<LENGTH_PER_FREQ;j++){
				msin[LENGTH_PER_FREQ*i + j]=std::sin(2*std::pi_v*freq*j/(double)SAMPLE_RATE);
			}
		}
	}


}
