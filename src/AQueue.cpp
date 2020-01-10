/*
 * AQueue.cpp
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */


#include <dos.h>
#include <stdlib.h>
#include <iostream.h>
#include "AQueue.h"
#include "Help.h"


AQueue::AQueue(){
lock;
first  = 0;
lenght =0;
unlock;
}

AQueue::~AQueue(){
	lock;
    Elem* old;
	while( first!=0 ) {
		old = first;
		first = first->sled;
		delete old;
		lenght--;
	}
	unlock;
}


int AQueue::size() const{
	return this->lenght;
}

void AQueue::put(PCB* p){
	lock;
	Elem **dp = &first;
	while((*dp)!= 0){
		if((*dp)->pcb == p) { unlock; return; }
		dp = &((*dp)->sled);
	}
	(*dp) = new Elem();
	(*dp)->sled = 0;
	(*dp)->pcb = p;
	this->lenght++;
	unlock;
}

PCB* AQueue::get(){
     if(first == 0) return 0;
     lock;
     lenght--;
     Elem* oldp = first;
     first = first->sled;
     unlock;
     return oldp->pcb;
}










