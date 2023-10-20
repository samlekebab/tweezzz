#ifndef SCHEDULER
#define SCHEDULER
//#include "formGenerator.h"
class FormGenerator;
class TimeEvent;
#include <mutex>
#include <algorithm>
#include "tas.h"
#include <list>
#include <fstream>
#include "aom.h"

struct AomHistoryPoint{
	Aom aom;
	long tick=0;
	
	AomHistoryPoint(const Aom& aom_to_copy,const long tick): aom(aom_to_copy),tick(tick){
		aom.allocateTable();
		aom.copyTable(aom_to_copy);

	}
	~AomHistoryPoint(){
		printf("aom history point destructor\n");
	//	if (aom!=nullptr) delete aom;
	}
	/*
	const bool operator<(const AomHistoryPoint other){
		return tick < other.tick; 
	}
	const bool operator<(const int t){
		return tick<t;
	}
	const bool operator<(const long int t){
		return tick<t;
	}
	const bool operator>=(const long int t){
		return tick>=t;
	}*/
	private : 
		AomHistoryPoint();


};

//this class job is to callback every active function generator (active meaning in the range of ticks that the function generator asks 
//it uses a heap to register function generators and a list to go throught active function generators
class Scheduler{
	public:
		std::ofstream logFile;
		long EOFT{0}; //this tick represent end of the last FormGenerator,
			      //it is the default time for FormGenerator::connect..(..) functions
			      
		Tas tas;
		//TODO change activeGenerators to bounds.start decreasing
		std::list<FormGenerator*> activeGenerators;//bounds.start increasing
		std::list<FormGenerator*> oldGenerators;//bounds.end decreasing

		Tas timeEventHeap;

		Aom& aom;
		std::list<AomHistoryPoint> aomHistory;//tick decreasing
		std::list<AomHistoryPoint>::reverse_iterator aomHistoryIterator;
		
		//this mutex lock the scheduler during any calculation step (aquired by the core), this way, we are sure that a rewind procedure is correctly done
		//it also ensure "atomic" operation on the data structures
		std::mutex usingScheduler_mutex;

////this value will tell the core if we can compute the sample at this tick, or if we need to rewind because a new FormGenerator has register at a previus time. the scheduler rewind the time and the core move it forward (after a calculation of a bunch of contigous ticks (this way is parallelisation frendly)
		long nextTickToCompute{0};
				       


		void addFormGenerator(FormGenerator* f);//add a FromGenerator in the heap, return the time in absolute of the end of this FormGenerator (since the begining of the scheduler)
		void addTimeEvent(TimeEvent* t);

		long computeSample(long tick);
		void callTimeEvent(long tick);


		bool isEmpty();//return true if the scheduler doesnt have anything to play anymore (dont take into account timeEvent)
		bool isEmptyTimeEvent();//look only at scheduled timeEvents


		Scheduler(Aom& aom);

	private :
		//Scheduler():logFile("res/sch.txt"){}
		void putInList(FormGenerator* f,long tick);
		void saveAom(long tick);
	

};

#endif
