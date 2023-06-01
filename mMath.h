#ifndef MMATH
#define MMATH
#define SIN_TAB_SIZE 2600
#define LENGTH_PER_FREQ 1000
#define SAMPLE_RATE 600'000'000
namespace mmath{
	double range = 10e6;
	double startFreq = 70e6-range/2;

	float sin[SIN_TAB_SIZE*LENGTH_PER_FREQ]{};
	int lut(double freq){};
	void fillUpTable();

	

};






#endif
