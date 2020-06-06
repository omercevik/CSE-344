/*
    CSE 344 Systems Programming
    Homework 05
    client.c
    Omer CEVIK
    161044004
*/
#include "main.h"

extern int fileCondition;
extern int fd;

void insert(struct Queue *currentQueue, struct client client, double distance)
{
    struct Queue *newQueue = (struct Queue*)malloc(sizeof(struct Queue));
    strcpy(newQueue->clientName, client.name);
    strcpy(newQueue->flower, client.flower);
    newQueue->queueDistance = distance;
    newQueue->next = NULL;

    if(currentQueue->fQueue == NULL)
        currentQueue->fQueue = currentQueue->lQueue = newQueue;
    else
    {
        currentQueue->lQueue -> next = newQueue;
        currentQueue->lQueue = newQueue;
    }
}

void pop(struct Queue *currentQueue)
{
    char str[256] = "";
    if(currentQueue->fQueue == NULL)
    {
        sprintf(str,"\nQueue is Empty!\n");
        write(STDIN_FILENO, str, strlen(str));
    }
    else
    {
        struct Queue *temp = currentQueue->fQueue;
        currentQueue->fQueue = currentQueue->fQueue -> next;
        free(temp);
    }
}

void parseClient(char* string, struct client* client)
{
    int i = 0, flowerIndex = 0;
    char openParanthese = '(', closeParanthese = ')', comma = ',';

    for (i = 0; string[i] != ' '; ++i)
        client->name[i] = string[i];

    client->name[i] = '\0';

    ++i;

    sscanf(&string[i],"%c%d%c%d%c",&openParanthese,&client->x,&comma,&client->y,&closeParanthese);

    for (; string[i] !=  ':'; ++i);

    i += 2;

    for (; string[i] != '\0'; ++i)
        client->flower[flowerIndex++] = string[i];

    client->flower[flowerIndex] = '\0';
}

void initClient(struct client* client)
{
    int j = 0;
    char c = ' ', endc = ' ', clientString[1024] = "";;

    while(read(fd,&c,1))
    {
        read(fd,&endc,1);
        lseek(fd,-1,SEEK_CUR);
        if (c == '\n')
        {
            clientString[j] = '\0';
            parseClient(clientString,client);
            j = 0;
            if (endc == '\n' && c == '\n')
                fileCondition = 0;
            break;
        }
        else
            clientString[j++] = c;
    }
}