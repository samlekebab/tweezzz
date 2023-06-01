#include "aom.h"
Aom::Aom():tweezerCount(3){
	tweezers = new Tweezer*[tweezerCount];
	for(int i=0;i<tweezerCount;i++){
		tweezers[i] = new Tweezer();
	}
}
