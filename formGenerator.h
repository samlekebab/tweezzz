#ifndef FORMGENERATOR
#define FORMGENERATOR
#include "scheduler.h"
#include "tas.h"
//this abstract class need to be inhertited by any form genrator that wants to modify a tweezer or aom valu over time (like Ai,wi,A etc..)
//this class allow the form generator to be called back at every tick (sample) to update the value
//
class FormGenerator:public Node{
	public:
		static int idIt;
		int id;
		char* tag=nullptr;
		void setTag(const char*); //can't be called mutiple time(or will have a small and negligable memory leak)
		virtual ~FormGenerator();
		FormGenerator();
		float* target;
		float _beginningValue;//used by the scheduler to recover when rewinding before bounds.start, please don't write it
		static Scheduler* scheduler;
		virtual float calc(long ticks)=0;	//takes ticks number from the begining of the generator (scheduled time)
		virtual long getDuration()=0;		//return the number of ticks (sample time) that the Form generator takes (cannot change after registration to the scheduler)
		void findAndSetBeginningValue();//this is called by the scheduler
		virtual void setBeginningValue(float value)=0;	//called at the begining of the generation to give to the FormGenerator where we are starting from
		virtual void schedulerAdd();
		Bounds connect(float& target);//registration to the scheduler at EOFT, return the begining and end of the formGenerator in absolute
		Bounds connect(long tick,float& target);//registration to the scheduler at absolute time "tick"
		Bounds connectRelative(long tick, float& target);//registration to the scheduler with time delay with respect to EOFT

};


#endif
