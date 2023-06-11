#include "formGenerator.h"
#include <stdexcept>
#include <iostream>
using namespace std;
Bounds FormGenerator::connect(long tick,float& target){
	this->target = &target;
	if (scheduler == nullptr){
		cout<<"error unregistered scheduler for formgenerator"<<endl;
		//throw runtime_error("unregistered scheduler"); 
	}
	this->bounds.start = tick;
	this->bounds.end = this->getDuration();

	scheduler->addFormGenerator(this);
	return this->bounds;
}
Bounds FormGenerator::connect(float& target){
	return this->connectRelative(0,target);
}
Bounds FormGenerator::connectRelative(long tick,float& target){
	return connect(scheduler->EOFT+tick,target);
}
FormGenerator::~FormGenerator(){}
Scheduler* FormGenerator::scheduler=nullptr;

//double FormGenerator::calc(long ticks){return 0;}
//long FormGenerator::getDuration(){return 0;}	
//void FormGenerator::setBeginningValue(double value){}
