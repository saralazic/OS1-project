/*
 * Queue.h
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#ifndef _AQUEUE_H_
#define _AQUEUE_H_

#include "pcb.h"
class PCB;

class Elem {
public:
	PCB* pcb;
	Elem* sled;
};

class AQueue {
private:
	int lenght;
	Elem* first;
public:
	AQueue();
	~AQueue();

	void put(PCB* p);
	PCB* get();

	int size() const;

};
#endif
