/*
Author: Andrew DiCarlo
Assignment Number: Assign4
File Name: assign4-part1.c
CoursSection: CS 3733 - 002
Due Date: 29NOV2020
Instructor: Dr. Silvestro

*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv])
{
    if (argc != 2) {
        printf("Usage:\n\t%s nthreads\n\tModels the dining philsophers problem with nthreads number of philosphers",
               argv[0]);
        exit(1);
    }

    return 0;
}