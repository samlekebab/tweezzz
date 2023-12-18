#ifndef WAIT_TIME_EVENT
#define WAIT_TIME_EVENT
#include <latch>
#include "timeEvent.h"
class WaitTimeEvent:public TimeEvent{
	public:
		float bidon;
		std::latch waitingLatch{1};

		WaitTimeEvent();
		float calc(long tick);
		void connectAndWait(long tick);
		void connectAndWait();
		void connectRelativeAndWait(long tick);
		void wait();


};
#endif
