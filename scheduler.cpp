#include "scheduler.h"
#include "formGenerator.h"
#include <iostream>
#include "tas.h"
#include <cmath>
#include "setting.h"
#include "timeEvent.h"

using namespace std;
void Scheduler::addFormGenerator(FormGenerator* f){
	lock_guard<mutex> scheduler_lock(this->usingScheduler_mutex);

#ifdef sch_log
	logFile<<"adding a generator"<<endl;
#endif
	if (f->bounds.start < nextTickToCompute){//rewind procedure

#ifdef sch_log
		logFile<<"put it in the list, rewind to "<<f->bounds.start<<endl;
#endif
		putInList(f);
		nextTickToCompute = f->bounds.start;
		
		//I give up rewinding procedure (for now) and do a "wait" command that halt calculation at a given moment
		//push back to the heap formgenerators that start after f->bounds.start
		//TODO		
		//pull finished formgenerators back in the list, or even further, in the heap (automaticaly sorted)
		//TODO

	}else{
#ifdef sch_log
		logFile<<"put it in the heap : "<<endl;
#endif
		tas.push(f);
		tas.print();
	}
	EOFT = max(EOFT,f->bounds.end);

}
void Scheduler::addTimeEvent(TimeEvent* t){

	lock_guard<mutex> scheduler_lock(this->usingScheduler_mutex);

#ifdef sch_log
	logFile<<"adding TimeEvent"<<endl;
#endif
	timeEventHeap.push(t);
	timeEventHeap.print();

}
long Scheduler::computeSample(long tick){
	//lock_guard<mutex> scheduler_lock(this->usingScheduler_mutex);
	//first, we pop the heap
	while(tas.getN()>0){
		long start = ((FormGenerator*)tas.view())->bounds.start;
		if (start>tick)
			break;

		auto f = (FormGenerator*)tas.pop();
#ifdef sch_log
		logFile<<"new form generator in the list "<<f->id<<" "<<f->bounds.start<<" ; "
		<<f->bounds.end<<" ";
		if (f->tag!=nullptr)
			logFile<<f->tag;
		logFile<<endl;
#endif
		putInList(f);
		f->findAndSetBeginningValue();//to be able to rewind

	}

	//then we callback every FormGenerator inside the list. When they are at bounds.end, we remove them from the list and destroy them, when at bounds.start, we set the start value
	//BUG TODO need to add a list that containse finished elements to put them back in case of rewind !!! (this list will be thankfully sorted wich limits complexitiy, and can be purged when as element bounds.end at the end of the list goes beyond the current play tick
	auto it = activeGenerators.begin();
	logFile<<"clc "<<tick<<" : ";
	while(it != activeGenerators.end()){
		/*if ((*it)->bounds.start == tick){
			cout<<"entering 
		}*///can't do that because this is not called at every tick
		  //this is can be an issue for the rewind process : need solution

		(*it)->calc(tick - (*it)->bounds.start);
#ifdef sch_log
		logFile<<(*it)->id<<" ";
		if ((*it)->tag!=nullptr)
			logFile<<(*it)->tag;
		logFile<<" ; ";
#endif
		if ((*it)->bounds.end < tick){
#ifdef sch_log
			logFile<<"removing a generator at"<<tick<<endl;
#endif
			delete *it;//BUG TODO see above 
			auto tmp = it;
			++it;
			activeGenerators.erase(tmp);
		}else{
			++it;
			
		}
	}
#ifdef sch_log
	logFile<<endl;
	logFile.flush();
#endif
	return 0;
}
						   

void Scheduler::putInList(FormGenerator* f){
	f->setBeginningValue(*(f->target));
	activeGenerators.push_back(f);
}


void Scheduler::callTimeEvent(long tick){

	//call each event
	//printf("%ld, %ld\n",tick,((TimeEvent*)timeEventHeap.view())->bounds.start);
	while(timeEventHeap.getN()>0 && ((TimeEvent*)timeEventHeap.view())->bounds.start<=tick){
		auto v = (TimeEvent*)timeEventHeap.pop();
		v->calc(tick - v->bounds.start);//call event, pass elapsed since due time

		//
#ifdef sch_log
		logFile<<"event on tick "<<tick<<endl;
		logFile.flush();
#endif
	}

}
