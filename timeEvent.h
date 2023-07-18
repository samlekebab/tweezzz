#ifndef M_TIME_EVENT
#define M_TIME_EVENT
#include "formGenerator.h"

class TimeEvent:public FormGenerator{
	public:
		long getDuration();	
		virtual float calc(long tick)=0;
		void setBeginningValue(float value);	
		virtual void schedulerAdd();
};
#endif
