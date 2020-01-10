/*
 * pcb.cpp
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#include "schedule.h"
#include "pcb.h"
#include "Kernel.h"
#include "Help.h"
#include <dos.h>
#include <iostream.h>

#include "AQueue.h"

/*
TimerRequest* PCB::TimerQueueHead=0;
TimerRequest* PCB::TimerQueueTail=0;
*/

const int PCB::NEW=1, PCB::READY=2, PCB::BLOCKED=3, PCB::FINISHED=4;

PCB* PCB::allPCBs=0;
PCB* PCB::running=0;


PCB::PCB(StackSize stsz, Time timeS, Thread* myThr){
	lock;
	stackSize=stsz;
	timeSlice=timeS;
	myThread=myThr;
	status=PCB::NEW;
	stek=0;
	waiting=new AQueue();
	myID=myThr->myID;


	next=PCB::allPCBs;
	prev=0;
	if(PCB::allPCBs!=0) PCB::allPCBs->prev=this;
	PCB::allPCBs=this;

	ss=sp=bp=0;
	unlock;
}

void PCB::createStack() {
	lock;
//	cout<<"Stek se kreira"<<endl;

	StackSize sstackSize = stackSize;
	sstackSize /= sizeof(unsigned);
	stek = new unsigned[sstackSize];

#ifndef BCC_BLOCK_IGNORE

	stek[sstackSize-3] =0x200; //setovan I fleg u
	// pocetnom PSW-u za nit

	stek[sstackSize-4] = FP_SEG(PCB::wrapper);
	stek[sstackSize-5] = FP_OFF(PCB::wrapper);

	sp = FP_OFF(stek+sstackSize-14);//svi sacuvani registri
	//pri ulasku u interrupt
	//rutinu
	ss = FP_SEG(stek+sstackSize-14);
	bp = FP_OFF(stek+sstackSize-14);
#endif
	unlock;
}


PCB::~PCB(){
	lock;
	myThread=0;

	//eliminating this PBC from list
	PCB *p=prev;
	PCB *n=next;
	if (p!=0) p->next=n;
	if (n!=0) n->prev=p;


	delete waiting;
	delete[] stek;
	unlock;
}

//eliminates PCB from allPCBs list
void PCB::eliminate(){
		if(prev!=0) prev->next=next;
		else allPCBs=next;
		if (next!=0) next->prev=prev;
		next=prev=0;
}



void PCB::waitToComplete() {
lock;
	//cout<<"Usao u wtc"<<endl;
	
	if (this->status == PCB::NEW) {
		//cout<<"zavrseno"<<endl;
		unlock;
		return;
	} //it's finished

	if (this->status == PCB::FINISHED) {
		//cout<<"zavrseno"<<endl;
		unlock;
		return;
	} //it's finished

	if (this->equal(Kernel::idle)) {
		//cout<<"Idle"<<endl;
		unlock;
		return;
	}

	if (this->equal(Kernel::starting)) {
	//	cout<<"Starting"<<endl;
		unlock;
		return;
	}


	if (this == PCB::running) {
		//cout<<"Cekala sama sebe"<<endl;
		unlock;
		return;
	} //thread can not wait for herself!

	PCB::running->setStatus(PCB::BLOCKED);
	waiting->put(PCB::running);
	unlock;
	dispatch();
//	cout<<"Izlazi iz wtc"<<endl;

}


//kad je nit gotova, ovo prodje kroz listu i stavlja niti koje su cekale u scheduler
void PCB::completed(){
	PCB* del;
	lock;
	while (waiting->size()>0){
		del=waiting->get();
		del->setStatus(PCB::READY);
		Scheduler::put(del);
	}
}

Thread* PCB::getMyThr(){
	return myThread;
}

int PCB::equal(Thread* someThread){
	if (myThread->getId()!=someThread->getId()) return 0;
	return 1;
}

void PCB::setStatus(int stat){
	status=stat;
}



//ELIMINATE IZBACUJE GOTOVU NIT IZ LISTE SVIH NITI PA getThreadByID ne radi ako se
//zove waitToComplete
void PCB::wrapper(){//staticka funkcija
	PCB::running->myThread->run();			//izvrsavam nit
	lock;
	PCB::running->status=PCB::FINISHED;		//oznacavam nit kao gotovu
//	PCB::running->eliminate();				//skloni ga iz liste svih pcb-ova
	PCB::running->completed();				//aktiviraj sve niti koje su cekale
	unlock;
	dispatch();
}




