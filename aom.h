#ifndef AOM
#define AOM
#include "tweezer.h"
class Aom{
	public:
		Tweezer** tweezers;//array of tweezer pointers
		int tweezerCount;
		float* table; //table containing all the parameters of the tweezers
		float A{1.0},N{2.0};//Amplitude and Normalisation of a given Aom

		Aom();
		~Aom();
};
#endif
