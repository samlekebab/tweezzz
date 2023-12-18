#ifndef AOM
#define AOM
#include "tweezer.h"
//TODO refactor aom -> aod
class Aom{
	public:
		Tweezer** tweezers;//array of tweezer pointers
		int tweezerCount;
		float* table; //table containing all the parameters of the tweezers
			      
		//TODO make it refere to the table
		float A{1.0},N{1.0};//Amplitude and Normalisation of a given Aom

		Aom();
		~Aom();

		inline void allocateTable(){
			int nb = tweezerCount*Tweezer::nbOfParam;
			table  = new float[nb];
		}
		void copyTable(const Aom& aom_to_copy);
};
#endif
