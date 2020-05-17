/*
    CSE 344 Systems Programming
    Homework 04
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
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/shm.h>

struct _sem
{
    sem_t semStart;
    sem_t semStop;
    sem_t semId;
    sem_t semChef;
    sem_t semDessert;
};

typedef struct _sem Semaphore;

void* chefsFunction(void* ptr);
void* wholesalerFunction(void *ptr);

void getArguments(int argc, char *argv[], char** filePath);
void readInputFile(char* filePath, char*** inputFile, int* lengthOfInput);
void initSemaphores();
void destroySemaphores();

#endif