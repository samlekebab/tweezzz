#include "scheduler.h"
#include "formGenerator.h"
#include <iostream>
#include "tas.h"
#include <cmath>
#include "setting.h"
#include "timeEvent.h"
#include <format>

using namespace std;
void Scheduler::addFormGenerator(FormGenerator* f){
#ifdef sch_log
	logFile<<"aquiring lock to add a generator"<<endl;
#endif
	//lock_guard<mutex> scheduler_lock(this->usingScheduler_mutex);
#ifdef sch_log
	logFile<<"adding a generator"<<endl;
#endif
	if (f->bounds.start < nextTickToCompute){//rewind procedure

#ifdef sch_log
		logFile<<"put it in the list, rewind to "<<f->bounds.start<<endl;
#endif
		putInList(f, nextTickToCompute);
		nextTickToCompute = f->bounds.start;
		
		//TODO refactor rename it
		auto it = aomHistoryIterator ;
		//find the right timestamp
		while (++it != aomHistory.rend() && (*it).tick >= nextTickToCompute);
		--it;

		//push formgenerators that start after nextTickToCompute back to the heap
		auto it2=activeGenerators.rbegin();
		while(it2 != activeGenerators.rend() && (*(it2))->bounds.start > nextTickToCompute){
			tas.push(*it2);
			it2++;
			activeGenerators.pop_back();
		}

		//pull finished formgenerators back in the active list(carefull for the sorting), or even further, in the heap (automaticaly sorted)
		Tas tmpSort;
		auto it3 = oldGenerators.begin();
		while(it3 != oldGenerators.end() && (*it3)->bounds.end >= nextTickToCompute){
			if ((*it3)->bounds.start > nextTickToCompute){
				tas.push(*it3);
			}else{
				tmpSort.push(*it3);
			}
			it3++;
			oldGenerators.pop_front();
		}
		while(tmpSort.getN()>0){
			activeGenerators.push_back((FormGenerator*)tmpSort.pop());
		}

		//chop head of aomHistoryList
		aomHistory.erase(aomHistory.begin(),std::next(it).base());

		//put the saved aom table in the current aom
		aom.copyTable((*it).aom);


	}else{
#ifdef sch_log
		logFile<<"put it in the heap"<<endl;
#endif
		tas.push(f);
		//tas.print();
	}
	EOFT = max(EOFT,f->bounds.end);

}
void Scheduler::addTimeEvent(TimeEvent* t){

	lock_guard<mutex> scheduler_lock(this->usingScheduler_mutex);

#ifdef sch_log
	logFile<<"adding TimeEvent"<<endl;
#endif
	timeEventHeap.push(t);
	//timeEventHeap.print();
	EOFT = max(EOFT,t->bounds.end);

}
long Scheduler::computeSample(long tick){
	//lock_guard<mutex> scheduler_lock(this->usingScheduler_mutex);
	//first, we pop the heap
	while(tas.getN()>0){
		long start = ((FormGenerator*)tas.view())->bounds.start;
		
		if (start>tick)
			break;

		auto f = (FormGenerator*)tas.pop();
		//tas.print();
#ifdef sch_log
		logFile<<"new form generator in the list "<<f->id<<" "<<f->bounds.start<<" ; "
		<<f->bounds.end<<" ";
		if (f->tag!=nullptr)
			logFile<<f->tag;
		logFile<<endl;
#endif
		putInList(f,tick);
		f->findAndSetBeginningValue();//to be able to rewind
		saveAom(tick);

	}

	//then we callback every FormGenerator inside the list. When they are at bounds.end, we remove them from the list and destroy them, when at bounds.start, we set the start value
	//BUG currrently aom history dont save "real" states but "mixed" stated between two time step, does it matter ? 
	//also, there can be a lot of "duplicate" of the same timestamp
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
			logFile<<endl<<"removing a generator at"<<tick<<endl;
#endif
			oldGenerators.push_front(*it);
			saveAom(tick);
			auto tmp = it++;
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

void Scheduler::putInList(FormGenerator* f,long tick){
	f->setBeginningValue(*(f->target));
	activeGenerators.push_back(f);

}
void Scheduler::saveAom(long tick){
#ifdef sch_log
	logFile<<"save aom at tick : "<<tick<<endl;
#endif
	//we do a snapshot of the aom state, to retreve it when rewinding
	//TODO ask another thread to do the copy of the table (?)
	aomHistory.push_front(*(new AomHistoryPoint(aom,tick)));

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

	//TODO proper memory freeing of generators
	//cleanning the old generators list and update aom list iterator
	while(++aomHistoryIterator != aomHistory.rend() && (*aomHistoryIterator).tick<tick);
	--aomHistoryIterator;
	//TODO delete the tail of the aomHystoryList ? 


	auto oldGeneratorsIterator = oldGenerators.rbegin();
	while(oldGeneratorsIterator != oldGenerators.rend() && (*oldGeneratorsIterator)->bounds.end<tick){
	
		//printf("c/\n");
		oldGenerators.pop_back();
		oldGeneratorsIterator = oldGenerators.rbegin();
		//printf("%d/\n",oldGenerators.size());
	}

	
}

Scheduler::Scheduler(Aom& aom,int channel):aom(aom),logFile(std::format("res/sch_ch{}.txt",channel)) {
	aomHistory.push_front(*(new AomHistoryPoint(aom,0)));
	aomHistoryIterator = aomHistory.rbegin();
	
}

//return true if the scheduler doesnt have anything to play anymore (dont take into account timeEvent)
bool Scheduler::isEmpty(){
	return (tas.getN()<=0) && (activeGenerators.size()<=0);
}

//look only at scheduled timeEvents
bool Scheduler::isEmptyTimeEvent(){
	return (timeEventHeap.getN()<=0);
}
