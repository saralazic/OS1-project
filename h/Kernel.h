/*
 * Kernel.h
 *
 *  Created on: Aug 24, 2018
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include "PCB.h"
#include "Thread.h"
#include "Idle.h"
#include "Event.h"

typedef void interrupt (*IntRoutine)();



class Kernel{

private:
		friend class Thread;
		friend class PCB;
		friend class KernelSem;
		friend class KernelEv;
		friend class Main;

public:

	static volatile int zahtevana_promena_konteksta;
	static Thread* starting; //first thread
	static Idle* idle;

	static volatile int brojac;

	static IntRoutine oldRoutine;


	static void init(int argc, char** argv);
	static void term();

	static void inic();
	static void restore();

	static void dispatch();
};

extern void tick();

void interrupt timer();

#endif /* KERNEL_H_ */
