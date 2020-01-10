/*
 * IVTEntry.h
 *
 *  Created on: Aug 20, 2018
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "Event.h"
#include "KernEv.h"
#include <dos.h>


typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;


#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...);\
IVTEntry newEntry##numEntry(numEntry, &inter##numEntry);\
void interrupt inter##numEntry(...){\
newEntry##numEntry.signal();\
if (callOld == 1)\
newEntry##numEntry.callOldR();\
}


class KernelEv;
class IVTEntry {

public:
//	friend class KernelEv;
	IVTEntry(IVTNo no, pInterrupt newRoutine);
	~IVTEntry();
	void callOldR();
	void signal();
	pInterrupt oldR;

	KernelEv *myKernelEv;
	static IVTEntry* IVT[256];
private:
	int ivtNo;


};

#endif /* IVTENTRY_H_ */
