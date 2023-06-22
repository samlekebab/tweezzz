#ifdef timeEvent
#define timeEvent
#include "formGenerator.h"

class TimeEvent:public formGenerator{
	long getDuration();	
	virtual float calc();
	void setBeginningValue();
}
#endif
