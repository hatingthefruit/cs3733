/*
Author: Andrew DiCarlo
Assignment Number: Assign2
File Name: assign2.c
Course/Section: CS 3733 - 002
Due Date: 11OCT2020
Instructor: Dr. Silvestro

Testing program to validate the scheduling algorithms. The program takes command line arguments of the form:
    ./assign2 q x1 y1 z1 x2 y2 z2
where:
    q stands for the quantum value
    (x1, y1, z1) correspond to a process with x1 units of CPU burst, y1 units of I/O burst, then z1 more units of CPU
        burst
    (x2, y2, z2) correspond to a process with x2 units of CPU burst, y2 units of I/O burst, then z2 more units of CPU
        burst
The arguments are checked for correctness, then passed to each cpu scheduler function in turn. After running each
algorithm, the printSchedOutput function is called to calculate statistics and print out the formatted output from the
scheduler, as well as the required statstics.

This program can be compiled by running make command with the included makefile. Alternately, it can be compiled with
the command:

gcc -o assign2 assign2.c pslibrary.c

*/

#include "pslibrary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void printSchedOutput(char *s1, char *s2, char *alg);

int main(int argc, char *argv[])
{
    // Declare variables
    int i, q, x1, x2, y1, y2, z1, z2;
    // Store the addresses of the arguments to allow filling them in a loop later
    int *argAdds[] = {&q, &x1, &y1, &z1, &x2, &y2, &z2};
    // Print usage message if the number of arguments is incorrect
    if (argc != 8) {
        printf("Wrong number of arguments.\nUsage: \n%s q x1 y1 z1 x2 y2 z2\n\n", argv[0]);
        printf("\tq stands for the quantum value\n");
        printf("\t(x1, y1, z1) correspond to a process with x1 units of CPU burst, y1 units of I/O burst, then z1 more "
               "units of CPU "
               "burst\n");
        printf("\t(x2, y2, z2) correspond to a process with x2 units of CPU burst, y2 units of I/O burst, then z2 more "
               "units of CPU "
               "burst\n");
        exit(1);
    }

    // Otherwise, print out a line with my name, followed by the arguments on a line, then a trailing newline
    printf("Assignment 2 program was written by Andrew DiCarlo\n");
    for (i = 1; i < argc; i++) {
        if (i != 1) {
            printf(" ");
        }
        printf("%s", argv[i]);

        // Parse current argument to int to pass to schedulers
        if (sscanf(argv[i], "%d", argAdds[i - 1]) == 1) {
            // Continue if the argument can be converted into an int
        }
        else {
            // Exit with an error message if the argument passed was not a valid integer
            printf("Error parsing argument %s to number; exiting now\n", argv[i]);
            exit(1);
        }
    }
    printf("\n");
    // Check that all the arguments are greater than 0; the library functions assume this to be true, so it is an error
    // if any arguments are not greater than 0.
    for (i = 0; i < argc - 1; i++) {
        if (*(argAdds[i]) <= 0) {
            printf("Bad argument '%d': arguments must be greater than 0\n", *(argAdds[i]));
            exit(1);
        }
    }

    // Allocate enough space for both processes to run all of their processes one after the other. This should avoid
    // running out of buffer space during any single scheduler
    int buffLen = (x1 + x2 + y1 + y2 + z1 + z2 + 1);
    char *ps1 = (char *)malloc(sizeof(char) * buffLen);
    char *ps2 = (char *)malloc(sizeof(char) * buffLen);

    // Init both buffers to all null
    memset(ps1, 0, buffLen);
    memset(ps2, 0, buffLen);

    fcfs(ps1, ps2, x1, y1, z1, x2, y2, z2);
    printSchedOutput(ps1, ps2, "fcfs");

    // Reset both buffers to all null
    memset(ps1, 0, buffLen);
    memset(ps2, 0, buffLen);

    sjf(ps1, ps2, x1, y1, z1, x2, y2, z2);
    printSchedOutput(ps1, ps2, "sjf");

    // Reset both buffers to all null
    memset(ps1, 0, buffLen);
    memset(ps2, 0, buffLen);

    psjf(ps1, ps2, x1, y1, z1, x2, y2, z2);
    printSchedOutput(ps1, ps2, "psjf");

    // Reset both buffers to all null
    memset(ps1, 0, buffLen);
    memset(ps2, 0, buffLen);

    rr(ps1, ps2, q, x1, y1, z1, x2, y2, z2);
    printSchedOutput(ps1, ps2, "rr");

    // Make sure buffers are freed
    free(ps1);
    free(ps2);
}

// Process and print the output for the scheduler buffers passed to it. This includes the scheduler output, as well as
// the name of the scheduler and the statistics for the current algorithm. Arguments:
//      char *s1: A string containing the scheduler output for process 1
//      char *s2: A string containing the scheduler output for process 2
//      char *alg: A string containing the name of the scheduler that output the current strings; this allows us to
//      embed it in the output
void printSchedOutput(char *s1, char *s2, char *alg)
{
    // Initialize counters for various process states and final outputs
    int r1 = 0, r2 = 0, R1 = 0, R2 = 0, i;
    double wavg, cutil;

    // Count the time spent waiting and time spent working for s1
    for (i = 0; i < strlen(s1); i++) {
        if (s1[i] == 'r') {
            r1++;
        }
        else if (s1[i] == 'R') {
            R1++;
        }
    }
    // Count the time spent waiting and time spent working for s2
    for (i = 0; i < strlen(s2); i++) {
        if (s2[i] == 'r') {
            r2++;
        }
        else if (s2[i] == 'R') {
            R2++;
        }
    }

    // Calculate the average waiting time
    wavg = ((double)r1 + (double)r2) / 2;

    // Calculate the overall CPU utilization
    cutil = ((double)R1 + (double)R2) / MAX(strlen(s1), strlen(s2));

    // Print the scheduler used, the output strings generated by it, and the statistics for the algorithm
    printf("scheduler %s:\n", alg);
    printf("%s\n%s\n", s1, s2);
    printf("%d %d %.1f %.5f\n", r1, r2, wavg, cutil);
}
