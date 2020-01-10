/*
 * KernelEv.h
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#ifndef KERNEV_H_
#define KERNEV_H_

#include "Event.h"
#include "PCB.h"




class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait ();


	int val();
protected:
	friend class IVTEntry;
	friend class Event;
	void signal();


private:
	int value;
	IVTNo myEntry;
	PCB* blocked; //since it is event, we block only one thread, we don't need a queue like with Semaphore
	PCB* creator; //we have to know who created this event

};

#endif /* KERNEV_H_ */
