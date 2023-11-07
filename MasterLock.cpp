#include "MasterLock.h"

void MasterLock::waitUnlock(){
	lock.lock();
	lock.unlock();
}

int MasterLock::testAndWaitUnlock(){
	std::lock_guard mLock(atomiser);
	if(!isLocked){
		return 1;
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
void unlock(){
	//std::lock_guard mLock(atomiser);
	lock.unlock();
}


