/*
Author: Andrew DiCarlo
Assignment Number: Assign2
File Name: pslibrary.c
Course/Section: CS 3733 - 002
Due Date: 11OCT2020
Instructor: Dr. Silvestro

Contains the body of the scheduler library functions declared in pslibrary.h

*/
#include "pslibrary.h"
#include <stdio.h>

// Array to translate from ready states to letters
static char stateChars[] = {'r', 'R', 'w', 0};

// First come first server scheduler. Processes are assigned to the CPU as they enter the ready queue, with process 1
// being scheduled first if both processes enter the ready state at the same time
// Arguments :
//      char *s1: String buffer to hold the process output for process 1
//      char *s2: String buffer to hold the process output for process 2
//      int (x1, y1, z1): represents the first process that has x1 units of CPU burst, followed by y1 units of I/O
//          burst, and then followed by z1 units of CPU burst.
//      int (x2, y2, z2): represents the second process that has x2 units of CPU burst, followed by y2 units of I/O
//          burst, and then followed by z2 units of CPU burst.
void fcfs(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2)
{

    // 1) handle state changes:
    //         running process completes CPU burst
    //         running process has quantum expire
    //         IO complete
    //   2) do context switch if necessary
    //         both ready
    //         one ready and CPU free
    //   3) append appropriate characters to character arrays
    //         avoid putting in multiple string terminators

    // assume s1 and s2 point to buffers with enough space to hold the result
    // assume that the int parameters are strictly greater than 0
    int i;              // next string position (time)
    int state1 = READY; // start with both ready
    int state2 = READY;
    int cpuLeft1 = x1; // P1 next CPU burst remaining
    int cpuLeft2 = x2; // P2 next CPU burst remaining
    int ioLeft1 = y1;  // P1 next IO burst remaining, 0 if no more IO
    int ioLeft2 = y2;  // P2 next IO burst remaining, 0 if no more IO

    for (i = 0; (state1 != DONE) || (state2 != DONE); i++) {
        // running process completes its CPU burst
        if ((state1 == RUNNING) && (cpuLeft1 == 0)) {
            if (ioLeft1 == 0) {
                state1 = DONE;
                s1[i] = stateChars[state1];
                s1[i + 1] = '\0';
            }
            else
                state1 = WAITING;
        }
        else if ((state2 == RUNNING) && (cpuLeft2 == 0)) {
            if (ioLeft2 == 0) {
                state2 = DONE;
                s2[i] = stateChars[state2];
                s2[i + 1] = '\0';
            }
            else
                state2 = WAITING;
        }
        // handle IO complete
        if ((state1 == WAITING) && (ioLeft1 == 0)) {
            state1 = READY;
            cpuLeft1 = z1;
        }
        if ((state2 == WAITING) && (ioLeft2 == 0)) {
            state2 = READY;
            cpuLeft2 = z2;
        }
        // if both ready, depends on algorithm
        if ((state1 == READY) && (state2 == READY)) {
            state1 = RUNNING;
        }
        // handle one ready and CPU available
        else if ((state1 == READY) && (state2 != RUNNING)) {
            state1 = RUNNING;
        }
        else if ((state2 == READY) && (state1 != RUNNING)) {
            state2 = RUNNING;
        }
        // insert chars in string, but avoid putting in extra string terminators
        if (state1 != DONE)
            s1[i] = stateChars[state1];
        if (state2 != DONE)
            s2[i] = stateChars[state2];
        // decrement counts
        // OK to decrement even if nothing running
        if (state1 == RUNNING)
            cpuLeft1--;
        if (state1 == WAITING)
            ioLeft1--;
        if (state2 == RUNNING)
            cpuLeft2--;
        if (state2 == WAITING)
            ioLeft2--;
    } // end of main for loop
}

// Shortest job first scheduler. Non-preemptive, this scheduler selects the process in the ready state with the shortest
// CPU burst to run first.
// Arguments :
//      char *s1: String buffer to hold the process output for process 1
//      char *s2: String buffer to hold the process output for process 2
//      int (x1, y1, z1): represents the first process that has x1 units of CPU burst, followed by y1 units of I/O
//          burst, and then followed by z1 units of CPU burst.
//      int (x2, y2, z2): represents the second process that has x2 units of CPU burst, followed by y2 units of I/O
//          burst, and then followed by z2 units of CPU burst.
void sjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2)
{
    // 1) handle state changes:
    //         running process completes CPU burst
    //         running process has quantum expire
    //         IO complete
    //   2) do context switch if necessary
    //         both ready
    //         one ready and CPU free
    //   3) append appropriate characters to character arrays
    //         avoid putting in multiple string terminators
    // assume s1 and s2 point to buffers with enough space to hold the result
    // assume that the int parameters are strictly greater than 0
    int i;              // next string position (time)
    int state1 = READY; // start with both ready
    int state2 = READY;
    int cpuLeft1 = x1; // P1 next CPU burst remaining
    int cpuLeft2 = x2; // P2 next CPU burst remaining
    int ioLeft1 = y1;  // P1 next IO burst remaining, 0 if no more IO
    int ioLeft2 = y2;  // P2 next IO burst remaining, 0 if no more IO

    for (i = 0; (state1 != DONE) || (state2 != DONE); i++) {
        // running process completes its CPU burst
        if ((state1 == RUNNING) && (cpuLeft1 == 0)) {
            if (ioLeft1 == 0) {
                state1 = DONE;
                s1[i] = stateChars[state1];
                s1[i + 1] = '\0';
            }
            else
                state1 = WAITING;
        }
        else if ((state2 == RUNNING) && (cpuLeft2 == 0)) {
            if (ioLeft2 == 0) {
                state2 = DONE;
                s2[i] = stateChars[state2];
                s2[i + 1] = '\0';
            }
            else
                state2 = WAITING;
        }
        // handle IO complete
        if ((state1 == WAITING) && (ioLeft1 == 0)) {
            state1 = READY;
            cpuLeft1 = z1;
        }
        if ((state2 == WAITING) && (ioLeft2 == 0)) {
            state2 = READY;
            cpuLeft2 = z2;
        }
        // if both ready, depends on algorithm
        if ((state1 == READY) && (state2 == READY)) {
            // Schedule the shorter of the two processes
            if (cpuLeft1 <= cpuLeft2) {
                state1 = RUNNING;
            }
            else {
                state2 = RUNNING;
            }
        }
        // handle one ready and CPU available
        else if ((state1 == READY) && (state2 != RUNNING)) {
            state1 = RUNNING;
        }
        else if ((state2 == READY) && (state1 != RUNNING)) {
            state2 = RUNNING;
        }
        // insert chars in string, but avoid putting in extra string terminators
        if (state1 != DONE)
            s1[i] = stateChars[state1];
        if (state2 != DONE)
            s2[i] = stateChars[state2];
        // decrement counts
        // OK to decrement even if nothing running
        if (state1 == RUNNING)
            cpuLeft1--;
        if (state1 == WAITING)
            ioLeft1--;
        if (state2 == RUNNING)
            cpuLeft2--;
        if (state2 == WAITING)
            ioLeft2--;
    } // end of main for loop
}
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
void psjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2)
{
    // 1) handle state changes:
    //         running process completes CPU burst
    //         running process has quantum expire
    //         IO complete
    //   2) do context switch if necessary
    //         both ready
    //         one ready and CPU free
    //   3) append appropriate characters to character arrays
    //         avoid putting in multiple string terminators

    // assume s1 and s2 point to buffers with enough space to hold the result
    // assume that the int parameters are strictly greater than 0
    int i;              // next string position (time)
    int state1 = READY; // start with both ready
    int state2 = READY;
    int cpuLeft1 = x1; // P1 next CPU burst remaining
    int cpuLeft2 = x2; // P2 next CPU burst remaining
    int ioLeft1 = y1;  // P1 next IO burst remaining, 0 if no more IO
    int ioLeft2 = y2;  // P2 next IO burst remaining, 0 if no more IO

    for (i = 0; (state1 != DONE) || (state2 != DONE); i++) {
        // running process completes its CPU burst
        if ((state1 == RUNNING) && (cpuLeft1 == 0)) {
            if (ioLeft1 == 0) {
                state1 = DONE;
                s1[i] = stateChars[state1];
                s1[i + 1] = '\0';
            }
            else
                state1 = WAITING;
        }
        else if ((state2 == RUNNING) && (cpuLeft2 == 0)) {
            if (ioLeft2 == 0) {
                state2 = DONE;
                s2[i] = stateChars[state2];
                s2[i + 1] = '\0';
            }
            else
                state2 = WAITING;
        }
        // handle IO complete
        if ((state1 == WAITING) && (ioLeft1 == 0)) {
            state1 = READY;
            cpuLeft1 = z1;
        }
        if ((state2 == WAITING) && (ioLeft2 == 0)) {
            state2 = READY;
            cpuLeft2 = z2;
        }
        // if both ready, depends on algorithm
        if ((state1 == READY) && (state2 == READY)) {
            // Schedule the shorter of the two processees
            if (cpuLeft1 <= cpuLeft2) {
                state1 = RUNNING;
            }
            else {
                state2 = RUNNING;
            }
        }
        // handle one ready and CPU available
        else if ((state1 == READY) && (state2 != RUNNING)) {
            state1 = RUNNING;
        }
        else if ((state2 == READY) && (state1 != RUNNING)) {
            state2 = RUNNING;
        }
        // If a process is ready, and the CPU burst left is less than the process currently running, then switch the two
        // processes
        else if ((state1 == READY) && (cpuLeft1 < cpuLeft2)) {
            state2 = READY;
            state1 = RUNNING;
        }
        else if ((state2 == READY) && (cpuLeft2 < cpuLeft1)) {
            state1 = READY;
            state2 = RUNNING;
        }

        // insert chars in string, but avoid putting in extra string terminators
        if (state1 != DONE)
            s1[i] = stateChars[state1];
        if (state2 != DONE)
            s2[i] = stateChars[state2];
        // decrement counts
        // OK to decrement even if nothing running
        if (state1 == RUNNING)
            cpuLeft1--;
        if (state1 == WAITING)
            ioLeft1--;
        if (state2 == RUNNING)
            cpuLeft2--;
        if (state2 == WAITING)
            ioLeft2--;
    } // end of main for loop
}

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
void rr(char *s1, char *s2, int quantum, int x1, int y1, int z1, int x2, int y2, int z2)
{
    // 1) handle state changes:
    //         running process completes CPU burst
    //         running process has quantum expire
    //         IO complete
    //   2) do context switch if necessary
    //         both ready
    //         one ready and CPU free
    //   3) append appropriate characters to character arrays
    //         avoid putting in multiple string terminators
    // assume s1 and s2 point to buffers with enough space to hold the result
    // assume that the int parameters are strictly greater than 0
    int i;              // next string position (time)
    int state1 = READY; // start with both ready
    int state2 = READY;
    int cpuLeft1 = x1; // P1 next CPU burst remaining
    int cpuLeft2 = x2; // P2 next CPU burst remaining
    int ioLeft1 = y1;  // P1 next IO burst remaining, 0 if no more IO
    int ioLeft2 = y2;  // P2 next IO burst remaining, 0 if no more IO
    int qleft;         // quantum remaining

    for (i = 0; (state1 != DONE) || (state2 != DONE); i++) {
        // running process completes its CPU burst
        if ((state1 == RUNNING) && (cpuLeft1 == 0)) {
            if (ioLeft1 == 0) {
                state1 = DONE;
                s1[i] = stateChars[state1];
                // terminate the string
            }
            else
                state1 = WAITING;
        }
        else if ((state2 == RUNNING) && (cpuLeft2 == 0)) {
            if (ioLeft2 == 0) {
                state2 = DONE;
                s2[i] = stateChars[state2];
                // terminate the string
            }
            else
                state2 = WAITING;
        }
        // running process has quantum expire
        if ((state1 == RUNNING) && (qleft == 0)) {
            state1 = READY;
            // If the other process is currently ready, start running that process
            if (state2 == READY) {
                state2 = RUNNING;
                qleft = quantum;
            }
        }
        if ((state2 == RUNNING) && (qleft == 0)) {
            state2 = READY;
            // If the other process is currently ready, start running that process
            if (state1 == READY) {
                state1 = RUNNING;
                qleft = quantum;
            }
        }
        // handle IO complete
        if ((state1 == WAITING) && (ioLeft1 == 0)) {
            state1 = READY;
            cpuLeft1 = z1;
        }
        if ((state2 == WAITING) && (ioLeft2 == 0)) {
            state2 = READY;
            cpuLeft2 = z2;
        }
        // if both ready, depends on algorithm
        if ((state1 == READY) && (state2 == READY)) {
            // When the quantum expires for a process, the other process is set to running if it was ready. If both
            // processes are ready at the same time, that means they either both just completed IO, or both just entered
            // the queue and a tie needs to be broken. In this implementation, the tie is broken by scheduling process 1
            // first.
            state1 = RUNNING;
            qleft = quantum;
        }
        // handle one ready and CPU available
        else if ((state1 == READY) && (state2 != RUNNING)) {
            state1 = RUNNING;
            qleft = quantum;
        }
        else if ((state2 == READY) && (state1 != RUNNING)) {
            state2 = RUNNING;
            qleft = quantum;
        }
        // insert chars in string, but avoid putting in extra string terminators
        if (state1 != DONE)
            s1[i] = stateChars[state1];
        if (state2 != DONE)
            s2[i] = stateChars[state2];
        // decrement counts
        qleft--; // OK to decrement even if nothing running
        if (state1 == RUNNING)
            cpuLeft1--;
        if (state1 == WAITING)
            ioLeft1--;
        if (state2 == RUNNING)
            cpuLeft2--;
        if (state2 == WAITING)
            ioLeft2--;
    } // end of main for loop
}