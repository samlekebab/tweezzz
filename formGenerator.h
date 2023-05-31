#ifndef FORMGENERATOR
#define FORMGENERATOR
#include "scheduler.h"
#include "tas.h"
//this abstract class need to be inhertited by any form genrator that wants to modify a tweezer or aom valu over time (like Ai,wi,A etc..)
//this class allow the form generator to be called back at every tick (sample) to update the value
//
struct Bounds : Node{
	long start;
	long end;
}
class FormGenerator{
	double& target;
	static Scheduler* scheduler;
	virtual double calc(long ticks);	//takes ticks number from the begining of the generator (scheduled time)
	virtual long getDuration();		//return the number of ticks (sample time) that the Form generator takes (cannot change after registration to the scheduler)
	virtual void setBeginingValue();	//called at the begining of the generation to give to the FormGenerator where we are starting from
	Bound connect(double& target);					//registration to the scheduler at EOFT, return the begining and end of the formGenerator in absolute
	Bound connect(long tick,double& target);					//registration to the scheduler at absolute time "tick"
	Bound connectRelative(long tick, double& target);			//registration to the scheduler with time delay with respect to EOFT

};



#endif
