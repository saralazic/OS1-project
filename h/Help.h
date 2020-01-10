/*
 * Help.h
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#ifndef HELP_CPP_
#define HELP_CPP_


#include<dos.h>

#define lock asm cli

#define unlock asm sti

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
#endif /* HELP_CPP_ */
