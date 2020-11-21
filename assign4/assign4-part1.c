/*
Author: Andrew DiCarlo
Assignment Number: Assign4
File Name: assign4-part1.c
CoursSection: CS 3733 - 002
Due Date: 29NOV2020
Instructor: Dr. Silvestro

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void createPhilosophers(int nthreads);
void *philosopherThread(void *arg);

int main(int argc, char *argv[])
{
    int nthreads;
    // Check that all arguments are present, exit with a usage message if they are not
    if (argc != 2) {
        printf("Usage:\n\t%s nthreads\n\tModels the dining philsophers problem with nthreads number of philosphers",
               argv[0]);
        exit(1);
    }

    // Convert the argument to an int, then print a name and the number of threads
    nthreads = atoi(argv[1]);
    printf("Andrew DiCarlo\n");
    printf("Assignment 4: # of threads = %d\n", nthreads);

    // If nthreads is not a positive number, then exit. Print this after printing the number of threads to provide more
    // feedback to the user. Since atoi returns 0 if no int was found, this covers errors as well.
    if (nthreads <= 0) {
        printf("Nthreads must be greater than 0\n");
        exit(1);
    }

    // Run the function that creates the philosopher threads
    createPhilosophers(nthreads);

    return 0;
}

// createPhilosophers(int nthreads) creates 'philosopher' threads that run asyncronously, all running the
// philosopherThread() function
// Arguments:
//  int nthreads: The number of threads to create and run
// Return:
//  N/A
void createPhilosophers(int nthreads)
{
    // Iterator
    int i;
    // Create an array of integers. This is necessary in order to pass a different number to each thread that will not
    // get overwritten in later iterations of a loop. Theoretically individual ints could be created during the loop,
    // but this is a little neater. Since all threads will be joined on in this function, this stack frame should
    // stay valid while all threads are still running
    int threadNumbers[nthreads];
    // Create an array of threads to allow joining on them
    pthread_t threads[nthreads];

    // Loop through to create the threads
    for (i = 0; i < nthreads; i++) {
        threadNumbers[i] = i; // Set the value of the current thread's int
        pthread_create(&(threads[i]), NULL, philosopherThread, &(threadNumbers[i])); // Create the thread
    }

    // Join on all threads
    for (i = 0; i < nthreads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Once all threads are complete, print out a message before returning
    printf("%d threads have been completed/joined successfully!\n", nthreads);
}

// void *philosopherThread(void *arg) is a function intended for using with the pthread_create library call. The
// argument is assumed to be a pointer to an integer. The function implemented here simply prints out the value pointed
// to by the argument and exits.
// Arguments:
//  void *arg: Assumed to be of type (int *). The value pointed to it indicates the thread number
// Returns:
//  NULL
void *philosopherThread(void *arg)
{
    // Save the thread number as a local variable.
    int number = *((int *)arg);
    // Print out the thread number then exit
    printf("This is philosopher %d\n", number);
    return NULL;
}