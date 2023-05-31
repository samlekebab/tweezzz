#include formGenerator.h
#include <stdexcept>
using namespace std;
long FormGenerator::connect(long tick,double& target){
	this.target = target;
	if (scheduler == nullptr){
		cout<<"error unregistered scheduler for formgenerator"<<endl;
		throw runtime_error("unregistered scheduler"); 
	}
	return secheduler->addFormGenerator(scheduler->EOFT+tick,this)
}
long FormGenerator::connect(double& target){
	return this.connect(0,target)
}
