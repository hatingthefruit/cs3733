/*
Author: Andrew DiCarlo
Assignment Number: Assign2
File Name: pslibrary.h
Course/Section: CS 3733 - 002
Due Date: 11OCT2020
Instructor: Dr. Silvestro
*/

#define READY 0
#define RUNNING 1
#define WAITING 2
#define DONE 3

void fcfs(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2);
void sjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2);
void psjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2);
void rr(char *s1, char *s2, int q, int x1, int y1, int z1, int x2, int y2, int z2);
