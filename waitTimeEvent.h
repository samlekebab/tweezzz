#ifndef WAIT_TIME_EVENT
#define WAIT_TIME_EVENT
#include <mutex>
#include "timeEvent.h"
class WaitTimeEvent:public TimeEvent{
	public:
		float bidon;
		std::mutex waitingMutex;

		WaitTimeEvent();
		float calc(long tick);
		void connectAndWait(long tick);
		void connectAndWait();
		void connectRelativeAndWait(long tick);
		void wait();


};
#endif
