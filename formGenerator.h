#ifndef FORMGENERATOR
#define FORMGENERATOR
#include "scheduler.h"
//this abstract class need to be inhertited by any form genrator that wants to modify a tweezer or aom valu over time (like Ai,wi,A etc..)
//this class allow the form generator to be called back at every tick (sample) to update the value
//
class FormGenerator{
	double& target;
	static Scheduler* scheduler;
	virtual double calc(long ticks);	//takes ticks number from the begining of the generator (scheduled time)
	virtual long getDuration();		//return the number of ticks (sample time) that the Form generator takes (cannot change after registration to the scheduler)
	virtual void setBeginingValue();	//called at the begining of the generation to give to the FormGenerator where we are starting from
	long connect(double& target);					//registration to the scheduler at EOFT, return the end of the formGenerator in absolute
	long connect(long tick, double& target);			//registration to the scheduler with time delay with respect to EOFT

};



#endif
