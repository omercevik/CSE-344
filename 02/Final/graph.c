/*
    CSE 344 Systems Programming
    Final Project
    graph.c
    Omer CEVIK
    161044004
*/

#include "main.h"

struct AdjListNode* newAdjListNode(int dest)
{
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

    int i = 0;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

void addEdge(struct Graph* graph, int src, int dest)
{
    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}

void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

struct queue* createQueue()
{
    struct queue* q = malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

int isEmpty(struct queue* q)
{
    return q->rear == -1;
}

void enqueue(struct queue* q, int value)
{
    if (q->rear == SIZE - 1)
        fprintf(stderr,"\nQueue is Full!!\n");
    else
    {
        if (q->front == -1)
            q->front = 0;
        ++q->rear;
        q->items[q->rear] = value;
    }
}

int dequeue(struct queue* q)
{
    int item = 0;
    if (isEmpty(q))
        item = -1;
    else
    {
        item = q->items[q->front];
        ++q->front;
        if (q->front > q->rear)
            q->front = q->rear = -1;
    }
    return item;
}

void printQueue(struct queue* q)
{
    int i = q->front;
    char str[256] = "";

    if (isEmpty(q))
    {
        sprintf(str,"Queue is empty\n");
        write(STDOUT_FILENO,str,strlen(str));
    }
    else
    {
        sprintf(str,"\nQueue contains\n");
        write(STDOUT_FILENO,str,strlen(str));

        for (i = q->front; i < q->rear + 1; ++i)
        {
            sprintf(str,"%d ", q->items[i]);
            write(STDOUT_FILENO,str,strlen(str));
        }
    }
}

void createDataBase(struct DataBase** db, int V)
{
    *db = (struct DataBase*)calloc(1, sizeof(struct DataBase));
    (*db)->dbSize = V;
    (*db)->paths = (int**)calloc(V, sizeof(int*));
    (*db)->colLengths = (int*)calloc(V, sizeof(int));

    (*db)->rowLength = 0;
    int i = 0;
    for (i = 0; i < V; ++i)
        (*db)->paths[i] = (int*)calloc(V, sizeof(int));
}

void freeEdge(struct AdjListNode* e)
{
    if (e != NULL)
    {
        freeEdge(e->next);
        free(e);
        e = NULL;
    }
}

void freeGraph(struct Graph* graph)
{
    int i = 0;
    for (; i < graph->V; ++i)
    {
        if (graph->array[i].head != NULL)
        {
            freeEdge(graph->array[i].head->next);
            free(graph->array[i].head);
            graph->array[i].head = NULL;
        }
    }
    free(graph->array);
    free(graph);
}

void freeDataBase(struct DataBase** db)
{
    int i = 0;
    for (i = 0; i < (*db)->dbSize; ++i)
        free((*db)->paths[i]);

    free((*db)->colLengths);
    free((*db)->paths);
    free(*db);
}