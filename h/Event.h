// File: event.h
#ifndef _event_h_
#define _event_h_


typedef unsigned char IVTNo;
class KernelEv;

#include "IVTEntry.h"

/*
#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...);\
IVTEntry newEntry##numEntry(numEntry, &inter##numEntry);\
void interrupt inter##numEntry(...){\
newEntry##numEntry.signal();\
if (callOld == 1)\
newEntry##numEntry.callOldR();\
}
*/

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();

	void wait();

protected:
	void signal();// can call KernelEv

private:
	KernelEv* myImpl;
};


#endif


