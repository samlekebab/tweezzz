#ifndef AOM
#define AOM
#include "tweezer.h"
class Aom{
	public:
		Tweezer** tweezers;//array of tweezers pointers
		int tweezerCount;
		double A,N;//Amplitude and Normalisation of a given Aom

		Aom();
};
#endif
