/*
 * pcb.h
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_


#include "Thread.h"

class AQueue;

void putToSleep(PCB* x, Time timeToSleep);
void UpdateSleepingList();

class PCB{
public:
	static PCB* allPCBs;
	static PCB* running;

//	static TimerRequest  *TimerQueueHead, *TimerQueueTail;

	ID myID;

	unsigned *stek;

	volatile unsigned ss;
	volatile unsigned sp;
	volatile unsigned bp;

	StackSize stackSize;
	Time timeSlice;
//	Time timePassed;

	static const int NEW, READY, BLOCKED, FINISHED;
	volatile int status;




	Thread* myThread;


	PCB* next;
	PCB *prev;


	AQueue* waiting;

	PCB(StackSize stsz, Time timeS, Thread* myThr);
	~PCB();

	void waitToComplete();
	void completed();

	Thread* getMyThr();
	void setStatus(int stat);
	void eliminate();
	void createStack();
	int equal(Thread* someThread);

	static void wrapper();

};



#endif /* PCB_H_ */
