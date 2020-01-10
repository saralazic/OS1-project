/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 20, 2018
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "PCB.h"
#include "Help.h"
#include "Event.h"
#include "Kernel.h"
#include "KernEv.h"
#include <dos.h>
#include <stdio.h>

IVTEntry* IVTEntry::IVT[256];

IVTEntry::IVTEntry(IVTNo no, pInterrupt newRoutine){
IVTEntry::IVT[no]=this;
ivtNo=no;
#ifndef BCC_BLOCK_IGNORE
oldR= getvect(no);
setvect(no, newRoutine);
#endif

}

IVTEntry::~IVTEntry(){
	lock;
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldR);
#endif
	unlock;

}

void IVTEntry::callOldR(){
	lock;
	(*oldR)();
	unlock;
}

void IVTEntry::signal(){
	lock;
	if (myKernelEv){
		myKernelEv->signal();
	}
	unlock;

}
