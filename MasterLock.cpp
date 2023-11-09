#include "MasterLock.h"
#include <thread>
#include <stdio.h>
void MasterLock::waitUnlock(){

	printf("waiting MasterLock");
	lock.lock();
	lock.unlock();
}

int MasterLock::testAndWaitUnlock(){
	{
		std::lock_guard mLock(atomiser);
		if(!isLocked){
			return 1;
		}
	}
	waitUnlock();
	return 0;
}

int MasterLock::tryLock(){
	std::lock_guard mLock(atomiser);
	if (!isLocked){
		return 1;
	}
	isLocked = 1;
	lock.lock();
	return 0;
}
void MasterLock::unlock(){
	lock.unlock();
	std::lock_guard mLock(atomiser);
	isLocked=0;
}
//tests
/*
void thread1(MasterLock& l){
	l.tryLock();
	printf("thread1\n");
	printf("thread1 2\n");
	l.unlock();

}
void thread2(MasterLock& l){
	l.testAndWaitUnlock();
	printf("thread2\n");

}
int main(){
	printf("start\n");
	MasterLock l;

	
	std::thread One(thread1,std::ref(l));
	std::this_thread::yield();
	//l.tryLock();
	std::thread Two(thread2,std::ref(l));
	std::this_thread::yield();
	printf("main\n");
	//l.unlock();
	std::this_thread::yield();
	l.testAndWaitUnlock();
	printf("main2\n");

	One.join();
	Two.join();
}
*/
