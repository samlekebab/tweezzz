#ifndef FORMGENERATOR
#define FORMGENERATOR
#include "scheduler.h"
//this virtual class need to be inhertited by every form genratior that want to modify a tweezer or aom valu over time (like Ai,wi,A etc..)
//this class allow the form generator to be called back at every tick (sample) to update the value
//
class FormGenerator{
	static Scheduler* scheduler;
	virtual double calc(long ticks);	//takes ticks number from the begining of the generator (scheduled time)
	virtual long getDuration();		//return the number of ticks (sample time) that the Form generator takes
	virtual void setBeginingValue();	//called at the begining of the generation to give to the FormGenerator where we are starting from

};



#endif
