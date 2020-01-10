#include "Thread.h"

#include "Help.h"
#include "Kernel.h"
#include "schedule.h"
#include <iostream.h>

int Thread::posID=1;

Thread::Thread (StackSize stackSize, Time timeSlice){
	lock;
	myPCB=new PCB(stackSize, timeSlice, this);
	myID=++posID;
	myPCB->createStack();
//	cout<<"kreirana nit "<<myID<<endl;
	unlock;
}


Thread::~Thread(){
	lock;
	waitToComplete();
	delete myPCB;
//	cout<<"unistena "<<myID<<endl;
	unlock;
}


void Thread::start(){
	lock;
//	cout<<"Usao u start"<<endl;
	myPCB->status=PCB::READY;
	Scheduler::put(myPCB);
//	cout<<"startovana nit "<<myID<<endl;
	unlock;
}


void Thread::waitToComplete(){
	//lock;
	myPCB->waitToComplete();
	//unlock;
}

ID Thread::getId(){
	return myID;
}

ID Thread::getRunningId(){
	return PCB::running->myThread->getId();
}

Thread* Thread::getThreadById(ID id){
	lock;
	    for(PCB* tek=PCB::allPCBs; tek!=0; tek=tek->next){
	    	//	cout<<tek->myThread->myID<< "    ";
				if(tek->myThread->myID==id) {
				unlock;
				return tek->myThread;
			}
	      }
	unlock;
	return 0;
}

void dispatch(){
	Kernel::dispatch();
}


PCB* Thread::getMyPCB(){
	return myPCB;
}
