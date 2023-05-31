#ifndef AOM
#define AOM
#include "tweezer.h"
class Aom{
	public:
		Tweezer** tweezers;//array of tweezers pointers
		int tweezerCount;
		double A{1.0},N{1.0};//Amplitude and Normalisation of a given Aom

		Aom();
};
#endif
