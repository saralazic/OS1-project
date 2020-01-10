/*
 * Idle.h
 *
 *  Created on: Aug 24, 2018
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "Thread.h"

class Idle: public Thread{
public:
	Idle();
	void run();
	void start();
	~Idle();
};

#endif /* IDLE_H_ */
