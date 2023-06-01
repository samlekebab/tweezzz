#include <iostream>
#include <list>
using namespace std;
int main(){
	long i=1;
	long X=1'000'000'000;
	for(long j=1;j<X;j++){
		if (j!=0)
			i*=j;
		i++;
	}
	cout<<i<<endl;
	return 0;
}
