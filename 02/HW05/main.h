/*
    CSE 344 Systems Programming
    Homework 05
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

struct client
{
    char name[256];
    char flower[256];
    int x;
    int y;
};

struct florist
{
    char name[256];
    char flowers[1024][256];
    int x;
    int y;
    int flowerSize;
    double click;
    int sales;
    int totalTimes;
};

struct Queue
{
    char clientName[256];
    char flower[256];
    double queueDistance;
    struct Queue *next;
    struct Queue *fQueue;
    struct Queue *lQueue;
};

void getArguments(int argc, char *argv[], char** filePath);
void initFlorists(char* filePath);
void initClient(struct client* client);
void parseFlorist(char* string, struct florist* florist);
void parseClient(char* string, struct client* client);
double getChevyDistance(struct florist florist, struct client client);
void* floristFunction(void* fiVoid);
void insert(struct Queue *currentQueue, struct client client, double distance);
void pop(struct Queue *currentQueue);

#endif