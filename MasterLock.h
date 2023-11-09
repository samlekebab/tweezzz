#pragma once
//this class allow the sequence to lock the core loop while we are writing in the scheduler, it is trying to solve the issue where the core loop prevent the loading of the scheduler
#include <mutex>
class MasterLock{
	int isLocked = 0;
	std::mutex lock;
	std::mutex atomiser;

public :
	int testAndWaitUnlock();
	int tryLock();
	void unlock();

private :
	void waitUnlock();
};


