#include "aom.h"
Aom::Aom():tweezerCount(100){
	tweezers = new Tweezer*[tweezerCount];
	table  = new float[tweezerCount*Tweezer::nbOfParam];
	for(int i=0;i<tweezerCount;i++){
		tweezers[i] = new Tweezer(&table[i],tweezerCount);
	}
}
Aom::~Aom(){
	delete[] table;
	delete[] tweezers;
}
