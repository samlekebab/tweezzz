#include "timeEvent.h"

long TimeEvent::getDuration(){return 0;}

void TimeEvent::setBeginningValue(float value){};

void TimeEvent::schedulerAdd(){
	scheduler->addTimeEvent(this);
}
