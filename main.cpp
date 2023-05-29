#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include "aom1D.h"
using namespace std;

int main(){
	Aom1D aom1D = Aom1D();
	cout<<aom1D.tweezers[0]->A<<endl;

	return 0;
}
