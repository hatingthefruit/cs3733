/*
Author: Andrew DiCarlo
Assignment Number: Assign1
File Name: wordcount.c
Course/Section: CS 3733 - 002
Due Date: 13 SEP 2020
Instructor: Dr. Silvestro

Program to count the words in a series of files passed as command line arguments:

./wordcount file1 file2 file3 ... fileN

The program spawns multiple processes, one for each file, using the fork() syscall. Each process then prints out the
wordcount for the current file and exits. The main process then waits for all child processes to exit using the wait()
syscall. Once all children have finished executing, the main process prints out the number of files counted and exits.

A makefile is included to automatically compile the program. Alternately, it can be compiled with the following command:

gcc -o wordcount wordcount.c
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

int countWords(char *fileName);

int main(int argc, char *argv[])
{
    int i;
    int cstatus;
    int ccount = 0;

    // print a usage message if no files are passed as arguments
    if (argc == 1) {
        printf("Usage: %s file1 file2 file3 ... fileN\n\n", argv[0]);
        printf("%s is a multi-process wordcounting utility. Each file passed as an argument is opened in a separate "
               "process, then the number of words is counted using space as a delimiter.\n",
               argv[0]);
        exit(1);
    }

    // loop through all the arguments
    for (i = 1; i < argc; i++) {
        // fork, then execute the file count if this is the child process
        if (fork() == 0) {
            int fileCount = countWords(argv[i]);
            // Print an error if countWords was not able to open the file, otherwise print the count
            if (fileCount == -1) {
                printf("Child process for %s: Error opening file\n", argv[i]);
            }
            else {
                printf("Child process for %s: number of words is %d\n", argv[i], fileCount);
            }
            // exit once the child process is finished
            exit(0);
        }
        else {
            // if this is the parent, increment the counter of how many files are being dealt with
            ccount++;
        }
    }

    // loop until all children have exited
    for (i = 0; i < ccount; i++) {
        wait(NULL);
    }

    // print a message with how many children were spawned/file were counted
    printf("All %d files have been counted\n", ccount);
}

// count the words in a file
// arguments:
//  char *fileName: The name of the file to perform the wordcount on
// returns: the number of words found in the file, or -1 if there was an error opening the file
int countWords(char *fileName)
{
    int i;
    int counter = 0;
    // This must be initialized to whitespace to properly count files that do not start with whitespace
    char prev = ' ';
    // create an input buffer, use a constant to allow variable size
    char inputBuff[BUFFER_SIZE];

    // try to open the file, and return -1 if it was not able to be opened
    FILE *inputFile = fopen(fileName, "r");
    if (inputFile == NULL) {
        return -1;
    }

    // loop through the input file until no more is able to be read
    while (fgets(inputBuff, BUFFER_SIZE, inputFile)) {
        // loop through the current string being held in the buffer
        for (i = 0; i < strlen(inputBuff); i++) {
            // if the current character is not a space, check to see if the last character was a space. If so, increment
            // the counter
            if (!isspace(inputBuff[i])) {
                if (isspace(prev)) {
                    counter++;
                }
            }
            // Set the current character as the previous one for the next iteration of the loop
            prev = inputBuff[i];
        }
    }
    // close the file
    fclose(inputFile);

    return counter;
}
