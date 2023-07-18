#include "waitTimeEvent.h"

WaitTimeEvent::WaitTimeEvent(){
		waitingMutex.lock();
};
float WaitTimeEvent::calc(long tick){
	waitingMutex.unlock();
	return 0;
}
void WaitTimeEvent::connectAndWait(long tick){
	connect(tick,bidon);
	wait();
}
void WaitTimeEvent::connectAndWait(){
	connect(bidon);
	wait();
}
void WaitTimeEvent::connectRelativeAndWait(long tick){
	connectRelative(tick,bidon);
	wait();
}
void WaitTimeEvent::wait(){
	waitingMutex.lock();
	waitingMutex.unlock();
}
