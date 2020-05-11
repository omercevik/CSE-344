/*
    CSE 344 Systems Programming
    Midterm Project
    main.h
    Omer CEVIK
    161044004
*/

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <time.h>

struct _sem
{
    sem_t semSupplier;
    sem_t semCook;
    sem_t semStudent;
    sem_t semCookStudent;
};

typedef struct _sem Semaphore;

void getArguments(int argc, char *argv[], int* Ncooks, int* Mstudents, int* Ttables, int* counterSizeS, int* L, char** filePath);
void createRandomInputFile(char* filePath, int LxM3);
int checkValuesForK(int Ncooks, int Mstudents, int Ttables, int counterSizeS, int L);
void writeToSharedMemory(char* whichShdMem, int counterP, int counterC, int counterD);
void readSharedMemory(char* whichShdMem, int* counterP, int* counterC, int* counterD);
void readInputFile(char* filePath, int PCD, int* randomValues);
void student(int num, int L);
void cook(int num, int PCD);
void supplier(int P, int C, int D, char* filePath, int Ncooks);

#endif