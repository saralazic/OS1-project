/*
 * Event.cpp
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#include "Event.h"
#include "Help.h"
#include <dos.h>
#include "KernEv.h"


Event::Event (IVTNo  ivtNo) {
	lock;
	myImpl=new KernelEv(ivtNo);
	unlock;
}

Event::~Event(){
	lock;
	delete myImpl;
	unlock;
}

void Event::wait(){
	if (!(myImpl->val())) {
		myImpl->wait();
	}
}

void Event::signal(){
	myImpl->signal();
}

typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

