/*
Author: Andrew DiCarlo
Assignment Number: Assign2
File Name: pslibrary.h
Course/Section: CS 3733 - 002
Due Date: 11OCT2020
Instructor: Dr. Silvestro

Prototypes for scheduler library functions.
*/

#define READY 0
#define RUNNING 1
#define WAITING 2
#define DONE 3

// First come first server scheduler. Processes are assigned to the CPU as they enter the ready queue, with process 1
// being scheduled first if both processes enter the ready state at the same time
// Arguments :
//      char *s1: String buffer to hold the process output for process 1
//      char *s2: String buffer to hold the process output for process 2
//      int (x1, y1, z1): represents the first process that has x1 units of CPU burst, followed by y1 units of I/O
//          burst, and then followed by z1 units of CPU burst.
//      int (x2, y2, z2): represents the second process that has x2 units of CPU burst, followed by y2 units of I/O
//          burst, and then followed by z2 units of CPU burst.
void fcfs(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2);
// Shortest job first scheduler. Non-preemptive, this scheduler selects the process in the ready state with the shortest
// CPU burst to run first.
// Arguments :
//      char *s1: String buffer to hold the process output for process 1
//      char *s2: String buffer to hold the process output for process 2
//      int (x1, y1, z1): represents the first process that has x1 units of CPU burst, followed by y1 units of I/O
//          burst, and then followed by z1 units of CPU burst.
//      int (x2, y2, z2): represents the second process that has x2 units of CPU burst, followed by y2 units of I/O
//          burst, and then followed by z2 units of CPU burst.
void sjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2);
// Preemptive shortest job first. Similar to sjf, but if there is a process in the ready state with a CPU burst shorter
// than what remains on the process currently running, the currently running process is preempted and replaced with the
// shorter process.
// Arguments :
//      char *s1: String buffer to hold the process output for process 1
//      char *s2: String buffer to hold the process output for process 2
//      int (x1, y1, z1): represents the first process that has x1 units of CPU burst, followed by y1 units of I/O
//          burst, and then followed by z1 units of CPU burst.
//      int (x2, y2, z2): represents the second process that has x2 units of CPU burst, followed by y2 units of I/O
//          burst, and then followed by z2 units of CPU burst.
void psjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2);
// Round Robin scheduler. Processes are rotated through the CPU, with each process running until it is done with a CPU
// burst or its quantum expires.
// Arguments :
//      char *s1: String buffer to hold the process output for process 1
//      char *s2: String buffer to hold the process output for process 2
//      int quantum: The quantum value to run the scheduler under; what is the maximum number of CPU cycles a process is
//          allowed to run before it is preempted if other processes are in the ready state
//      int (x1, y1, z1): represents the
//      first process that has x1 units of CPU burst, followed by y1 units of I/O
//          burst, and then followed by z1 units of CPU burst.
//      int (x2, y2, z2): represents the second process that has x2 units of CPU burst, followed by y2 units of I/O
//          burst, and then followed by z2 units of CPU burst.
void rr(char *s1, char *s2, int q, int x1, int y1, int z1, int x2, int y2, int z2);
