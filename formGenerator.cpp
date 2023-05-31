#include "formGenerator.h"
#include <stdexcept>
#include <iostream>
using namespace std;
Bounds FormGenerator::connect(long tick,double& target){
	this->target = &target;
	if (scheduler == nullptr){
		cout<<"error unregistered scheduler for formgenerator"<<endl;
		throw runtime_error("unregistered scheduler"); 
	}
	this->bounds.start = tick;
	this->bounds.end = this->getDuration();

	scheduler->addFormGenerator(this);
	return this->bounds;
}
inline Bounds FormGenerator::connect(double& target){
	return this->connectRelative(0,target);
}
inline Bounds FormGenerator::connectRelative(long tick,double& target){
	return connect(scheduler->EOFT+tick,target);
}
