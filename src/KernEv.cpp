/*
 * KernelEv.cpp
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#include "KernEv.h"
#include "schedule.h"
#include "pcb.h"
#include "Help.h"
#include "IVTEntry.h"

KernelEv::KernelEv(IVTNo ivtN) {
	myEntry = ivtN;
	creator = PCB::running;
	IVTEntry::IVT[myEntry]->myKernelEv = this;
	value = 1;
	blocked = 0;
}

KernelEv::~KernelEv() {
	IVTEntry::IVT[myEntry]->myKernelEv = 0;
	creator = 0;
	blocked = 0;
}

void KernelEv::signal() {
	if (creator->status == PCB::BLOCKED && value == 0) {
		creator->setStatus(PCB::READY);
		Scheduler::put(creator);
		value=1;
		dispatch();
	}
}

void KernelEv::wait() {
	if (PCB::running == creator && value==1) {
			value=0;
			PCB::running->setStatus(PCB::BLOCKED);
			dispatch();
		}

}

int KernelEv::val() {
	return value;
}
