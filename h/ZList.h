/*
 * BlockList.h
 *
 *  Created on: Jul 3, 2019
 *      Author: OS1
 */

#ifndef ZLIST_H_
#define ZLIST_H_


#include "pcb.h"


class PCB;

class BElem{
public:
PCB* pcb;
int limited; //da li je nit blokirana na maksimalno vreme?
int time; //vreme do isteka da se odblokira
BElem* sled;


BElem(PCB* p, Time maxTimeToWait);
};

class BlockedList{
public:
int lenght;
BElem* first;
BElem* last;


BlockedList();
~BlockedList();

int UpdateList();

void put(PCB* p, Time maxTimeToWait=0);
PCB* get();

int empty();

void write();

};
#endif
