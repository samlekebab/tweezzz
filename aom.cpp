#include "aom.h"
#include <cstring>
#include <iostream>
Aom::Aom():tweezerCount(100){
	tweezers = new Tweezer*[tweezerCount];
	//table  = new float[tweezerCount*Tweezer::nbOfParam];
	allocateTable();
	for(int i=0;i<tweezerCount;i++){
		tweezers[i] = new Tweezer(&table[i],tweezerCount);
	}
}
Aom::~Aom(){
	printf("aom destructor\n");
	delete[] table;
	delete[] tweezers;
}
void Aom::copyTable(const Aom& aom_to_copy){
	int nb = tweezerCount*Tweezer::nbOfParam;
	memcpy(table,aom_to_copy.table,nb*sizeof(float));
}
