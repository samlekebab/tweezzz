#include "waitTimeEvent.h"
#include <stdio.h>
WaitTimeEvent::WaitTimeEvent(){
};
float WaitTimeEvent::calc(long tick){
	printf("\n\n\ncall end wait\n\n\n");
	waitingLatch.count_down();//undefined behaviour ?
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
	//in record mode, timeEvents are not relevent, and waitTimeEvent needs not lock the sequence

	if (!recordMode){
		waitingLatch.wait();
	}
}
