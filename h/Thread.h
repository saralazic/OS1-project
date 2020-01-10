/*
 * Thread.h
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#ifndef _thread_h_
#define _thread_h_


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

class PCB; //Kernel's implementation of user's thread


class Thread {

public:
  void start();
  void waitToComplete();
  virtual ~Thread();

  ID getId();
  static ID getRunningId();
  static Thread * getThreadById(ID id);
  
  PCB* getMyPCB(); //added-getter for myPCB
  ID myID;
protected:
  friend class PCB; friend class Kernel;
  Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
  virtual void run() {}

private:
  PCB* myPCB;
  static int posID;

};



void dispatch ();


#endif
