/*
 * main.cpp
 *
 *  Created on: Sep 15, 2018
 *      Author: OS1
 */

#include "Kernel.h"
#include "thread.h"
#include "pcb.h"
#include <iostream.h>
#include "user.h"
#include "SCHEDULE.H"
extern int userMain(int argc, char** argv);

int main(int argc, char* argv[]){

//	cout<<"\n STARTED \n";

	Kernel::inic();
	Thread* poc=0;
//	poc->myID=0;
	PCB* pocetna=new PCB(256, 2, poc);
	PCB::running =(PCB*)pocetna;
	PCB::running->status=PCB::READY;
	Kernel::idle=new Idle();

	int i=userMain(argc, argv);

	Kernel::restore();
	delete pocetna;
//	cout<<"\n DONE \n";
	return 0;
}
