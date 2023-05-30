#ifndef sheduler
#define sheduler
//#define ARDUINO
//#define PERF_MONITOR
#ifndef ARDUINO
	#define word int
	#define delay(X) Sleep(X)
	#include <iostream>
	#include <Windows.h>
	#include <ctime>
	#define millis() clock()
  using namespace std;
#else
  #include <arduino.h>
#endif	

#define TAS_SIZE 100
#define LOW_PRIORITY 5
#define HIGH_PRIORITY 1
#define TICK_MAX 0x7FFF
#define TIME_OUT ((TICK_MAX/2)-1)



class Node {
public:
	unsigned int weight = 0;
	byte priority = LOW_PRIORITY;
};

class Tas {
public:
    void push(Node* node);
    Node *pop();
	Node *view();
	bool isEmpty();
	Tas(int* tick);
	void print();
	int getN(){return n;};
private:
	int* tick;
	void echange(word i, word j);
	Node* tas[TAS_SIZE];
	word n=0;
	bool comparer(Node* a, Node* b);
}; 
class Task :public Node {
public:
	int id = 0;
	Task();
	virtual void execute();
	bool expire = true;
	virtual ~Task(){};



};
#define FENETRE TIME_OUT
#define LARGEUR TICK_MAX
int transpose(int val, int curseur);

class Scheduler {
public:
	Scheduler();
	~Scheduler();	
	void addTask(Task* task);
	void start(Task* task);
	int tickUntil();
	int tickUntil(Task* task);
	void executeNext();
	bool isEmpty();
	int getNbtask(){
		return tas->getN();
	}

	int tick = 0;
private:
	Tas* tas;
};

//une queu avec des niveaux de priorit�
class TaskScheduler : public Task {
public:
	static void reInit() {
		scheduler = new Scheduler();
	}
	static bool getState() {
		if (scheduler != nullptr)
			return true;
		return false;
	}
	TaskScheduler() {
	}
	static void start() {
		scheduler->start(new TaskScheduler());
	}
	static int addTask(Task* task) {
		id++;
		task->id = id;
		task->expire = false;
		task->weight = id;
		scheduler->addTask(task);
		return id; 

	}
	void execute() {
		done();

	}
	static void done() {
		if (scheduler->isEmpty()) {
			finish();
			return;
		}
		tickUpdate();
		scheduler->executeNext();
	}
	static void finish() {
		delete scheduler;
		scheduler = nullptr;
	}
private:
	static void tickUpdate() {
		scheduler->tick = (scheduler->tick + 1) % TICK_MAX;
	}
	static int id;
	static Scheduler* scheduler;
};
//int MyScheduler::id = 0;
//Scheduler* MyScheduler::scheduler = new Scheduler();
#ifdef PERF_MONITOR
class Monitor :public Task {
	void execute();

#define PERF_ECH 4000
};
#endif


//main sheduler 1tick=1ms
class TimeScheduler : public Task {
public:
	TimeScheduler() {
	}
	static void start() {
#ifdef PERF_MONITOR
		addTask(new Monitor(), 1000);
#endif
		scheduler->start(new TimeScheduler());
	}
	static void start(Task* task) {
		addTask(task, 0);
		start();
	}
	static int addTask(Task* task, int delay) {
		id++;
		task->id = id;
		task->expire = false;
		tickUpdate();
		task->weight = (scheduler->tick + delay) % TICK_MAX;
        task->weight=task->weight%TICK_MAX;//wtf
        //Serial.println("wht : "+String(task->weight) );
		scheduler->addTask(task);
		return id;
	}
	void execute() {
		//cout << "run the scheduler" << endl;
		time = millis();
		while (!scheduler->isEmpty()) {
			tickUpdate();				

			int tmp = scheduler->tickUntil();
			if (tmp >= 10) {
#ifdef PERF_MONITOR
				sleepingTime += tmp - 9;
#endif
				delay(tmp - 9);
			}
			tickUpdate();
			scheduler->executeNext();
		}

		finish();
	}
	void finish() {
		delete scheduler;
	}
	static void reinit(void) {
		scheduler = new Scheduler();
	}
#ifdef PERF_MONITOR
	static unsigned long sleepingTime;
#endif

	static Scheduler* scheduler;
private:
	static void tickUpdate() {
		unsigned long tmp = millis();
		scheduler->tick = (scheduler->tick + (tmp - time)) % TICK_MAX;
		time = tmp;
		//cout << tmp<<endl;
	}
	static unsigned long time;
	static int id;

};
class TaskCreator : Task {
	void (*f)();
	TaskCreator(void (*f)());
	void execute();
};
#endif
