/*
 * Idle.cpp
 *
 *  Created on: Aug 24, 2018
 *      Author: OS1
 */
#include "PCB.h"
#include "Thread.h"
#include "Idle.h"

#include "Help.h"

#include<iostream.h>


Idle::Idle(): Thread(256, 55){}



void Idle::run(){
//	cout<<"IDLE\n";
	while(1);
}

void Idle::start(){
	lock;
	getMyPCB()->status=PCB::READY;
//	getMyPCB()->createStack();
	unlock;
}


Idle::~Idle(){
	lock;
	this->getMyPCB()->setStatus(PCB::FINISHED);
	unlock;
}
