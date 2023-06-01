#ifndef MY_HEADER_H_INC
#define MY_HEADER_H_INC
#define SIN_TAB_SIZE 10'000
#define LENGTH_PER_FREQ 2000
#define SAMPLE_RATE 600'000'000
namespace mmath{

	extern float sin[SIN_TAB_SIZE*LENGTH_PER_FREQ];
	int lut(double freq);
	void fillUpTable();

	

};
#endif
