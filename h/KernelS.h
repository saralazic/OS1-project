/*
 * KernelSem.h
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#ifndef BCC_KERNELSEM_H_
#define BCC_KERNELSEM_H_

#include "Zlist.h"


class KernelSem {
public:
	KernelSem(int init=1);
	virtual ~KernelSem();

	int wait (Time maxTimeToWait);
	int signal(int n=0);

	int val () const;  // Returns the current value of the semaphore

	void setVal(int inc);


	static void UpdateBlockLists();


private:
	  int value;
	  BlockedList* blocked;

	  KernelSem *sled;
	 static KernelSem *first;

	void UpdateMyList();


};

#endif /* BCC_KERNELSEM_H_ */
