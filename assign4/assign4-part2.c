/*
Author: Andrew DiCarlo
Assignment Number: Assign4
File Name: assign4-part2.c
CoursSection: CS 3733 - 002
Due Date: 29NOV2020
Instructor: Dr. Silvestro

*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void createPhilosophers(int nthreads);
void *philosopherThread(void *arg);
void thinking();
void pickUpChopsticks(int threadIndex);
void eating();
void putDownChopsticks(int threadIndex);

// nthreads and chopsticks need to be globally available.
int nthreads;
pthread_mutex_t *chopsticks; // Chopsticks is malloc'd later when creating the threads

int main(int argc, char *argv[])
{
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

    // Create and initialize the array of chopsticks (mutexes)
    chopsticks = malloc(sizeof(pthread_mutex_t) * nthreads);
    for (i = 0; i < nthreads; i++) {
        pthread_mutex_init(&(chopsticks[i]), NULL);
    }
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
    free(chopsticks);
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
    printf("Philosopher #%d: start thinking\n", number);
    thinking();
    printf("Philosopher #%d: stop thinking\n", number);
    pickUpChopsticks(number);
    printf("Philosopher #%d: start eating\n", number);
    eating();
    printf("Philosopher #%d: stop eating\n", number);
    putDownChopsticks(number);

    return NULL;
}

// thinking() waits for a random period of time. The srandom() function is called with a new seed value every time,
// which is derived from the system clock
// Arguments:
//  None
// Return:
//  None
void thinking()
{
    // Set the seed to something not reproducible
    unsigned int seed = time(NULL);
    srandom(seed);
    // Get a random number between 1 and 500
    unsigned int sleep_time = ((random()) % 500) + 1;
    usleep(sleep_time);
}

// pickUpChopsticks(int threadIndex) tries to pick up the chopsticks to either side of the philosopher modeled by the
// thread. If threadIndex is even, then the left is picked up first. Otherwise, the right is picked up first
// Arguments:
//  int threadIndex: The number assigned to the thread at run time. This is used to identify which chopsticks belong to
//  this philosopher
// Return:
//  None
void pickUpChopsticks(int threadIndex)
{
    int remain = threadIndex % 2;

    // This code alternates which chopstick gets 'picked up' first
    // There's two cases:
    //  ThreadIndex is even -> remain = 0
    //     -> threadIndex + remain = threadIndex (left)
    //     -> threadIndex + 1 - remain = threadIndex + 1 (right)
    //  threadIndex is off -> remain = 1
    //     -> threadIndex + remain = threadIndex + 1 (right)
    //     -> threadIndex + 1 - remain = threadIndex (left)
    // Alternating which side gets picked up first helps avoid deadlock
    pthread_mutex_lock(&(chopsticks[(threadIndex + remain) % nthreads]));
    pthread_mutex_lock(&(chopsticks[(threadIndex + 1 - remain) % nthreads]));
}

// eating() waits for a random period of time. The srandom() function is called with a new seed value every time,
// which is derived from the system clock. This is basically the same as the thinking() function
// Arguments:
//  None
// Return:
//  None
void eating()
{
    // Set the seed to something not reproducible
    unsigned int seed = time(NULL);
    srandom(seed);
    // Get a random number between 1 and 500
    unsigned int sleep_time = ((random()) % 500) + 1;
    usleep(sleep_time);
}

// putDownChopsticks(int threadIndex) unlocks the mutexes that represent the chopsticks to either side of the
// philosopher modeled by the thread.
// Arguments:
//  int threadIndex: The index assigned to the current thread. This is used to identify which chopsticks belong to this
//  philosopher
// Return:
//  None
void putDownChopsticks(int threadIndex)
{
    // Release locks on both chopsticks in no particular order. This is not as important as the order of picking them up
    pthread_mutex_unlock(&(chopsticks[threadIndex]));
    pthread_mutex_unlock(&(chopsticks[(threadIndex + 1) % nthreads]));
}