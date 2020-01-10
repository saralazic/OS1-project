/*
 * BlockList.cpp
 *
 *  Created on: Jul 3, 2019
 *      Author: OS1
 */

#include "ZList.h"

#include "KernelS.h"
#include "SCHEDULE.h"
#include "Help.h"
#include <iostream.h>

BElem::BElem(PCB* p, Time maxTimeToWait) {
	pcb = p;
	sled = 0;
	if (maxTimeToWait > 0)
		limited = 1;
	else
		limited = 0;
	time = maxTimeToWait;

}

BlockedList::BlockedList() {
	lock;
	first = last = 0;
	lenght = 0;
	unlock;
}

BlockedList::~BlockedList() {
	lock;
	BElem* old;
	while (first) {
		old = first;
		first = first->sled;
		delete old;
	}
	unlock;
}

void BlockedList::put(PCB* p, Time maxTimeToWait) {
	lock;
	BElem* novi = new BElem(p, maxTimeToWait);
	if (first == 0)
		first = last = novi;
	else {
		last->sled = novi;
		last = novi;
	}
	lenght++;
	unlock;
}

PCB* BlockedList::get() {
	lock;
	if (first == 0)
		return 0;
	BElem* oldp = first;
	first = first->sled;
	if (first == 0)
		last = 0;
	lenght--;
	unlock;
	return oldp->pcb;
}

int BlockedList::empty() {
	return this->lenght;
}

int BlockedList::UpdateList() {
	lock;
	int counter = 0;
	BElem *preth = 0, *tek = first;
	while (tek != 0) {
		if (tek->limited != 0) {
			tek->time--;
			//lock;
			//cout<<"Thread "<<tek->pcb->myThread->getId()<<" "<<tek->time<<endl;
			//unlock;
			if (tek->time <= 0) {
				BElem *del = tek;
				if (preth != 0) {
					preth->sled = tek->sled;
					tek = tek->sled;
				} else {
					tek = tek->sled;
					first = tek;
				} //eliminisan iz liste;
				PCB *pcb = del->pcb;
				pcb->setStatus(PCB::READY);
				Scheduler::put(pcb);
				delete del;
				this->lenght--;
				counter++; //brojim odblokirane niti da posle azuriram val
			} else {
				preth = tek;
				tek = tek->sled;
			}

		} else {
			preth = tek;
			tek = tek->sled;
		}
	}
	if (first == 0)
		last = 0;
	unlock;
	return counter;
}

void BlockedList::write() {
	lock;
	BElem *preth = 0, *tek = first;
	while (tek) {
		lock;
		cout << "PCB " << tek->pcb->myThread->getId() << endl;
		unlock;
		tek = tek->sled;
	}
	unlock;
}

