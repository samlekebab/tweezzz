#include "formGenerator.h"
#include <stdexcept>
#include <iostream>
#include <cstring>
using namespace std;
Bounds FormGenerator::connect(long tick,float& target){
	this->target = &target;
	if (scheduler == nullptr){
		cout<<"error unregistered scheduler for formgenerator"<<endl;
		//throw runtime_error("unregistered scheduler"); 
	}
	this->bounds.start = tick;
	this->bounds.end = tick+this->getDuration();

	//scheduler->addFormGenerator(this);
	schedulerAdd();
	return this->bounds;
}

void FormGenerator::schedulerAdd(){
	scheduler->addFormGenerator(this);
}

Bounds FormGenerator::connect(float& target){
	return this->connectRelative(0,target);
}
Bounds FormGenerator::connectRelative(long tick,float& target){
	return connect(scheduler->EOFT+tick,target);
}
void FormGenerator::findAndSetBeginningValue(){
	_beginningValue = *target;
	setBeginningValue(_beginningValue);
}
void FormGenerator::setTag(const char* tag){
	int i{0};
	while (tag[++i]!='\0');
	this->tag = new char[i];
	memcpy(this->tag,tag,i*sizeof(char));
}
FormGenerator::FormGenerator():id(idIt++) {}
FormGenerator::~FormGenerator(){if(tag){delete[] tag;}}
Scheduler* FormGenerator::scheduler=nullptr;
int FormGenerator::idIt = 0;
int FormGenerator::recordMode = false;
//double FormGenerator::calc(long ticks){return 0;}
//long FormGenerator::getDuration(){return 0;}	
//void FormGenerator::setBeginningValue(double value){}
