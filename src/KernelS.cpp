/*
 * KernelSem.cpp
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#include "KernelS.h"
#include "PCB.h"
#include "Schedule.h"
#include "Thread.h"
#include <iostream.h>
#include <math.h>
#include "Help.h"
#include "ZList.h"

KernelSem* KernelSem::first = 0;

KernelSem::KernelSem(int init) {
	this->value = init;
	this->blocked = new BlockedList();
	this->sled = 0;
	if (first == 0) {
		first = this;
	} else {
		this->sled = first;
		first = this;
	}
/*
	lock;
	cout<<"Init: "<<value<<endl;
	unlock;
*/
}

//OVO POGLEDAJ JOS MALO
KernelSem::~KernelSem() {
	lock;
	KernelSem *temp = first, *prev;
	while (temp != this) {
		prev = temp;
		temp = temp->sled;
	}
	if(prev)
	prev->sled = temp->sled; //izbaci iz liste svih KernelSem
	else first = temp->sled;
	delete blocked;
	unlock;
}

int KernelSem::val() const {
	return value;
}

int KernelSem::wait(Time maxTimeToWait) {
//standardno ponasanje
if (value-- <= 0) {
	if (maxTimeToWait == 0) {
		PCB::running->setStatus(PCB::BLOCKED);
		blocked->put(PCB::running);
		dispatch();
		/*
		lock;
		cout<<"wait "<<value<<endl;
		unlock;
		*/
		return 1;
	} else {
		lock;
//		cout<<"blokiran"<<endl;
		unlock;
		PCB::running->setStatus(PCB::BLOCKED);
		blocked->put(PCB::running, maxTimeToWait);
	//	blocked->write();
		dispatch();
		/*
		lock;
		cout<<"wait "<<value<<endl;
		unlock;
		*/
		return 0;
	}
}
return 1;
}



int KernelSem::signal(int n) {
	/*
	lock;
	cout<<"Signal"<<endl;
	unlock;
	*/
int toDeblock = 0; //broj niti koje se odblokiraju
if (n < 0)
	return n;
if (n == 0) {//ako je n==0, signal se ponasa normalno (odblokira jednu nit)
	if (value++ < 0) {
		/*
		lock;
		cout<<"Signal posle "<<value<<endl;
		unlock;
		*/
		PCB* p = blocked->get();
		if (p) {
			p->setStatus(PCB::READY);
			Scheduler::put(p);
			toDeblock = 1;
		} else
			toDeblock = 0;
	}
} else { //za n>0, signal treba da odblokira n niti
	if (value >= 0) {
		toDeblock = 0;
	} else if (value + n <= 0) {
		toDeblock = n;
	} else {
		toDeblock = abs(value);
	}
/*
	lock;
	cout<<"Signal pre "<<value<<endl;
	unlock;
*/
	value += n; //vrednost semafora se uvecava za prihvaceni broj
/*
	lock;
	cout<<"Signal posle "<<value<<endl;
	unlock;
*/
	for (int i = 0; i < toDeblock; i++) {
		PCB* p = blocked->get();
		if (p){
			p->setStatus(PCB::READY);
			Scheduler::put(p);
		}
	}
}
return toDeblock;
}

void KernelSem::setVal(int inc) {
this->value += inc;
}

void KernelSem::UpdateMyList() {
this->value += this->blocked->UpdateList();
/*
lock;
cout<<"Update: "<<value<<endl;
unlock;
*/
}

void KernelSem::UpdateBlockLists() {
KernelSem *temp = first;
while (temp != 0) {
	temp->UpdateMyList();
	temp = temp->sled;
}
}
