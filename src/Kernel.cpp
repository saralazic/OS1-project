#include "Kernel.h"
#include "Schedule.h"
#include "Help.h"
#include "Idle.h"
#include "Thread.h"
#include "PCB.h"
#include "KernelS.h"
#include "Event.h"
#include <iostream.h>

//ova klasa je ok

Thread* Kernel::starting=0;
Idle* Kernel::idle=new Idle();
volatile int Kernel::zahtevana_promena_konteksta=0;
volatile int Kernel::brojac = 20;


unsigned oldTimerOFF, oldTimerSEG;


unsigned tsp;
unsigned tss, tbp;



void interrupt timer() {	// prekidna rutina
/*
	lock;
	cout<<"prekid"<<endl;
	unlock;
*/

	tick();

	if (!Kernel::zahtevana_promena_konteksta && Kernel::brojac>0)
		Kernel::brojac--;



	if (((Kernel::brojac <= 0) && (PCB::running->timeSlice!=0))|| Kernel::zahtevana_promena_konteksta){
	asm {
		// cuva sp
		mov tsp, sp
		mov tss, ss
		mov tbp, bp
	}

	PCB::running->sp = tsp;
	PCB::running->ss = tss;
	PCB::running->bp = tbp;


	if (PCB::running->status == PCB::READY && PCB::running->getMyThr()->getId()!=Kernel::idle->getId())
		Scheduler::put(PCB::running);


	PCB::running = Scheduler::get();	// Scheduler

	if (PCB::running == 0 || PCB::running->status==PCB::FINISHED)
		PCB::running = Kernel::idle->getMyPCB();



	tsp = PCB::running->sp;
	tss = PCB::running->ss;
	tbp = PCB::running->bp;

	Kernel::brojac = PCB::running->timeSlice;


asm {
	mov sp, tsp   // restore sp
	mov ss, tss
	mov bp, tbp
}

	}

   // poziv stare prekidne rutine koja se
   // nalazila na 08h, a sad je na 60h
   // poziva se samo kada nije zahtevana promena
   // konteksta – tako se da se stara
   // rutina poziva samo kada je stvarno doslo do prekida
if(!Kernel::zahtevana_promena_konteksta) {
	asm int 60h;
	KernelSem::UpdateBlockLists();
}

Kernel::zahtevana_promena_konteksta = 0;
}



void Kernel::inic(){
	asm{
			cli
			push es
			push ax

			mov ax,0   //  ; inicijalizuje rutinu za tajmer
			mov es,ax

			mov ax, word ptr es:0022h //; pamti staru rutinu
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax

			mov word ptr es:0022h, seg timer	 //postavlja
			mov word ptr es:0020h, offset timer //novu rutinu

			mov ax, oldTimerSEG	 //	postavlja staru rutinu
			mov word ptr es:0182h, ax //; na int 60h
			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax

			pop ax
			pop es
			sti
		}
}

// vraca staru prekidnu rutinu
void Kernel::restore(){
		asm {
			cli
			push es
			push ax

			mov ax,0
			mov es,ax


			mov ax, word ptr oldTimerSEG
			mov word ptr es:0022h, ax
			mov ax, word ptr oldTimerOFF
			mov word ptr es:0020h, ax

			pop ax
			pop es
			sti
		}
	}

void Kernel::dispatch(){
	lock;
//	cout<<"\nDISPATCH\n";
	Kernel::zahtevana_promena_konteksta=1;
	timer();
	unlock;
}


