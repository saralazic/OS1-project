/*
 * Semaphore.cpp
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#include "semaphor.h"

#include "Help.h"
#include "KernelS.h"

Semaphore::Semaphore(int init){
	myImpl=new KernelSem(init);
}

Semaphore::~Semaphore(){
	lock;
	delete myImpl;
	myImpl=0;
	unlock;
}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

int Semaphore::val() const {
	return myImpl->val();
}








