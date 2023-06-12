#ifndef MY_HEADER_H_INC
#define MY_HEADER_H_INC
#define SIN_TAB_SIZE 10'000
#define LENGTH_PER_FREQ 2000
#include "setting.h"
#include <cstdint>
namespace mmath{

	extern float sin[SIN_TAB_SIZE*LENGTH_PER_FREQ];
	extern int16_t sin16[SIN_TAB_SIZE*LENGTH_PER_FREQ];
	extern int8_t sin8[SIN_TAB_SIZE*LENGTH_PER_FREQ];

	int lut(double freq);
	void fillUpTable();

	

};
#endif
