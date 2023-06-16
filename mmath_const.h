#ifndef MMATH_CONST
#define MMATH_CONST
#include "mmath.h"
namespace mmath {
	extern float sin[SIN_TAB_SIZE*LENGTH_PER_FREQ];
	extern int16_t sin16[SIN_TAB_SIZE*LENGTH_PER_FREQ];
	extern int8_t sin8[SIN_TAB_SIZE*LENGTH_PER_FREQ];
}
#endif
