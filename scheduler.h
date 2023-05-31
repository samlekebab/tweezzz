#ifndef SCHEDULER
#define SCHEDULER
//#include "formGenerator.h"
class FormGenerator;
#include <mutex>
#include <algorithm>
#include "tas.h"
#include <list>
//this class will callback every active function generator (active meaning in the range of ticks that the function generator asks 
//it uses a heap to register function generators and a list to go throught active function generators
class Scheduler{
//TODO mutex the heap
	public:
		long EOFT{0}; //this time reprent end of the last FormGenerator, it is the default time at 
			      
		Tas tas;
		std::list<FormGenerator*> activeGenerators;

		
		//this mutex lock the scheduler during any calculation step (aquired by the core), this way, we are sure that a rewind procedure is correctly done
		//it also assure syncrone operation on the heap
		std::mutex usingScheduler_mutex;

////this value will tell the core if we can compute the sample at this tick, or if we need to rewind because a new FormGenerator has register at a previus time. the scheduler rewind the time and the core move it forward (after a calculation of a bunch of contigous ticks (this way is parallelisation frendly)
		long nextTickToCompute{0};
				       


		void addFormGenerator(FormGenerator* f);//add a FromGenerator in the heap, return the time in absolute of the end of this FormGenerator (since the begining of the scheduler)
		long nextSampleToCompute();
		long computeSample(long tick);

	private :
		void putInList(FormGenerator* f);

};

#endif
