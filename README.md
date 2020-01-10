# OS1-project
Small but functional multithreading operating system kernel with time sharing, thread scheduling, semaphores and interrupt triggered events.
Developed in C++ for intel 8086. Borland C++, huge memory model.

For testing purposes the 32-bit Windows is needed.

This project is developed for the Operating Systems 1 class, School of Electrical Engineering, University of Belgrade.


How to use OS kernel: 
    1. Implement function userMain(argc, argv) in your user code, this is the main entry point for your application, arguments are passed from command line.

   2.  Create other user threads by extending OS class thread. You can specify thread name, thread stack size and time slice (minimum run time before thread can be interrupted). Implement custom thread code inside run method. Start thread by calling start method, thread will be started and run will be called on thread selection that depends on scheduler.
