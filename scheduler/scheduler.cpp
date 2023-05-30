#include "scheduler.h"

int transpose(int val, int curseur) {

	//Serial.println("in transpose : " + String(TICK_MAX));
	//Serial.println("in transpose : " + String(TIME_OUT));
	//Serial.println("in transpose : val :" + String(val));
	//Serial.println("in transpose : curs :" + String(curseur));
	if (val < curseur - FENETRE)
		return LARGEUR + val;
	if (val - FENETRE > curseur)
		return val - LARGEUR;
	return val;
}

Tas::Tas(int* tick) {
	this->tick = tick;
}
bool Tas::comparer(Node* a, Node* b) {
	int w1 = transpose(a->weight, *tick);
	int w2 = transpose(b->weight, *tick);
	if (b->weight == a->weight)
		return a->priority > b->priority;
	return w2 < w1;
}
void Tas::echange(word i, word j) {
	Node* tmp = tas[i];
	tas[i] = tas[j];
	tas[j] = tmp;
}
void Tas::push(Node* node) {
	tas[n] = node;
	n += 1;
	word i = n - 1;
	while (i > 0 && comparer(tas[(i - 1) / 2], tas[i])) {
		word tmp = (i - 1) / 2;
		echange(i, tmp);
		i = tmp;
	}
}
bool Tas::isEmpty() {
	return n <= 0;
}
Node* Tas::pop() {
	Node* res = tas[0];
	n -= 1;
	tas[0] = tas[n];
	bool end = false;
	word i = 0;
	word tmp = 2 * i + 1;
	while (!end && tmp < n) {
		end = true;
		if (comparer(tas[i], tas[tmp])) {
			echange(i, tmp);
			end = false;
			i = tmp;
		}
		else if (tmp + 1 < n && comparer(tas[i], tas[tmp + 1])) {
			tmp++;
			echange(i, tmp);
			end = false;
			i = tmp;
		}
	}
	return res;
}
Node* Tas::view() {
	return tas[0];
}
void Tas::print() {
#ifndef ARDUINO
	cout << "tas de taille " << n << endl;
	for (word i = 0; i < n; i++) {
		cout << "element " << i << " : " << (*tas[i]).weight << ", " << (int)(*tas[i]).priority << endl;
	}
#else
	//TODO serial	
	Serial.println( "tas de taille " + String( n ));
	for (word i = 0; i < n; i++) {
		Serial.println( "element " + String(i) + " : " + String( (*tas[i]).weight ) + ", " + String ( (int)(*tas[i]).priority ));
	}
#endif
}

Task::Task() {}
void Task::execute() {
	//nothing by default
}



bool Scheduler::isEmpty() {
	return tas->isEmpty();
}
Scheduler::Scheduler() {
	tas = new Tas(&tick);
}
Scheduler::~Scheduler() {
	delete tas;
}
int Scheduler::tickUntil() {
	return this->tickUntil((Task*)tas->view());
}
int Scheduler::tickUntil(Task* task) {
	int tmp = transpose(task->weight, tick);
	/*Serial.println("in tickuntil : " + String(task->weight));
	Serial.println("in tickuntil : " + String(tick));
	Serial.println("in tickuntil : " + String(tmp));*/
	return tmp - tick;
}
void Scheduler::addTask(Task* task) {
	//cout << task->weight << ' ' << transpose(task->weight, tick)<<" id :"<<task->id << endl;
	//Serial.println("add Task");
	tas->push((Node*)task);
}
void Scheduler::executeNext() {
	//Serial.println(tick);
	//tas->print();
	
	Task* task = (Task*)tas->pop();
	if (!task->expire) {
		//cout << task->weight << ' ' << transpose(task->weight, tick) << " id :" << task->id << endl;
		task->expire = true;
		task->execute();
	}
	if (task->expire) {
		delete task;
	}
}
void Scheduler::start(Task* task) {
	task->execute();
	delete task; //a pripori, il n'y a pas d'instance de la tache initial a la fin de la list de tache du sheduler ..??
}

unsigned long TimeScheduler::time = 0;
int TimeScheduler::id = 0;
Scheduler* TimeScheduler::scheduler = new Scheduler();

Scheduler* TaskScheduler::scheduler = new Scheduler();
int TaskScheduler::id = 0;
#ifdef PERF_MONITOR
unsigned long TimeScheduler::sleepingTime = 0;

void Monitor::execute(){
#ifdef ARDUINO
	Serial.print("performances :");
	Serial.println(1 - TimeScheduler::sleepingTime / (PERF_ECH - 8.5));
	Serial.print("Nb TAsk : ");
	Serial.println(TimeScheduler::scheduler->getNbtask());
#else
	cout << "performances :" << 1.0 - TimeScheduler::sleepingTime / (PERF_ECH-8.5) << endl;
#endif
	TimeScheduler::sleepingTime = 0;
	TimeScheduler::addTask(this, PERF_ECH);
}
#endif

TaskCreator:: TaskCreator(void (*f)()) {
		this->f = f;
	}
void TaskCreator::execute() {
		f();
	}
