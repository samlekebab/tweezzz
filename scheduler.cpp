#include "scheduler.h"
#include "formGenerator.h"
#include <iostream>
#include "tas.h"
using namespace std;
void Scheduler::addFormGenerator(FormGenerator* f){
	lock_guard<mutex> scheduler_lock(this->usingScheduler_mutex);
	cout<<"adding a generator"<<endl;
	if (f->bounds.start < nextTickToCompute){
		cout<<"put it in the list"<<endl;
		putInList(f);
		nextTickToCompute = f->bounds.start;
	}else{
		cout<<"put it in the heap : "<<endl;
		tas.push(f);
		tas.print();
	}

}

long Scheduler::nextSampleToCompute(){return 0;}//TODO ??? 
						//
						//
long Scheduler::computeSample(long tick){
	//lock_guard<mutex> scheduler_lock(this->usingScheduler_mutex);
	//first, we pop the heap
	while(tas.getN()>0){
		long start = ((FormGenerator*)tas.view())->bounds.start;
		if (start>tick)
			break;

		auto f = (FormGenerator*)tas.pop();
		cout<<"new form generator in the list "<<f->bounds.start<<" ; ";
		cout<<f->bounds.end<<endl; 
		putInList(f);

	}

	//then we callback every FormGenerator inside the list. When they are at bounds.end, we remove them from the list and destroy them, when at bounds.start, we set the start value
	//BUG TODO need to add a list that containse finished elements to put them back in case of rewind !!! (this list will be thankfully sorted wich limit complexitiy addup, and can be purged when an element bound.end goes beyond the current play tick
	auto it = activeGenerators.begin();
	while(it != activeGenerators.end()){
		/*if ((*it)->bounds.start == tick){
			cout<<"entering 
		}*///can't do that because this is not called at every tick
		  //this is can be an issue for the rewind process : need solution
		  //reminder : the rewind process will allow the computer to clear computational power for the branch prediction (wich needs temporaly 2times more ressources)
		  
		if ((*it)->bounds.end < tick){
			cout<<"removing a generator"<<endl;
			delete *it;//BUG TODO see above 
			auto tmp = it;
			++it;
			activeGenerators.erase(tmp);
		}else{
			(*it)->calc(tick - (*it)->bounds.start);
			++it;
		}
	}
	return 0;
}
						   

void Scheduler::putInList(FormGenerator* f){
	f->setBeginningValue(*(f->target));
	activeGenerators.push_back(f);
}
