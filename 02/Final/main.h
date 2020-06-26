/*
    CSE 344 Systems Programming
    Final Project
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
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <syslog.h>

#define SIZE 500000
#define MAX 2048
#define SA struct sockaddr

/* Bit-mask values for 'flags' argument of becomeDaemon() */
#define BD_NO_CHDIR 01
/* Don't chdir("/") */
#define BD_NO_CLOSE_FILES 02
/* Don't close all open files */
#define BD_NO_REOPEN_STD_FDS 04
/* Don't reopen stdin, stdout, and stderr to
*
/dev/null */
#define BD_NO_UMASK0 010 /* Don't do a umask(0) */
#define BD_MAX_CLOSE 8192 /* Maximum file descriptors to close if
* sysconf(_SC_OPEN_MAX) is indeterminate */

struct AdjListNode
{
    int dest;
    struct AdjListNode* next;
};

struct AdjList
{
    struct AdjListNode *head;
};

struct Graph
{
    int V;
    struct AdjList* array;
};

struct queue
{
    int items[SIZE];
    int front;
    int rear;
};

struct DataBase
{
    int** paths;
    int rowLength;
    int dbSize;
    int* colLengths;
};

struct _sem
{
    sem_t semAdditional;
    sem_t semServer;
};

typedef struct _sem Semaphore;

int makeDaemon(int flags);

struct queue* createQueue();
void enqueue(struct queue* q, int);
int dequeue(struct queue* q);
int isEmpty(struct queue* q);
void printQueue(struct queue* q);

void createDataBase(struct DataBase** db, int V);
void freeDataBase(struct DataBase** db);

int searchInDataBase(int node1, int node2, int** dbPath, int* dbPathSize);
int BFS(int src, int dest, int* pred, int* dist);
void evaluatePathBFS(int s, int dest, char* resultPath);

void initSemaphores();
void destroySemaphores();

void getArgumentsServer(int argc, char *argv[], char** filePath, int* portNumber, char** logFile);
void* serverThread(void* ptr);
void* additionalThread(void* ptr);

void clientFunction(int sockfd, char* nodes);
void getArgumentsClient(int argc, char *argv[], char** IPAddress, int* portNumber, int* nodeStart, int* nodeEnd);

struct AdjListNode* newAdjListNode(int dest);
struct Graph* createGraph(int V);
void addEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);
void readFile(char* filePath);
int isNumber(char c);
void freeGraph(struct Graph* graph);
void freeEdge(struct AdjListNode* e);

#endif