/*
    CSE 344 Systems Programming
    Homework 04
    wholesaler.c
    Omer CEVIK
    161044004
*/

#include "main.h"

extern Semaphore* semaphore;

int sugar = 0;
int flour = 0;
int milk = 0;
int walnuts = 0;
int condition = 1;

void *wholesalerFunction( void *ptr )
{
    char** inputFile = NULL;
    char* filePath = (char*) ptr;
    int i = 0, j = 0, lengthOfInput = 0, foodCounter = 0;
    char str[1024] = "";

    readInputFile(filePath, &inputFile, &lengthOfInput);

    for (i = 0; i < lengthOfInput; ++i)
    {
        condition = 1;
        if (inputFile[i][0] == 'S')
        {
            ++sugar;
            if (inputFile[i][1] == 'F')
            {
                ++flour;
                sprintf(str, "the wholesaler delivers sugar and flour\n");
                write(STDIN_FILENO,str,strlen(str));
            }
            else if (inputFile[i][1] == 'M')
            {
                ++milk;
                sprintf(str, "the wholesaler delivers sugar and milk\n");
                write(STDIN_FILENO,str,strlen(str));
            }
            else if (inputFile[i][1] == 'W')
            {
                ++walnuts;
                sprintf(str, "the wholesaler delivers sugar and walnuts\n");
                write(STDIN_FILENO,str,strlen(str));
            }
        }
        else if (inputFile[i][0] == 'F')
        {
            ++flour;
            if (inputFile[i][1] == 'S')
            {
                ++sugar;
                sprintf(str, "the wholesaler delivers flour and sugar\n");
                write(STDIN_FILENO,str,strlen(str));
            }
            else if (inputFile[i][1] == 'M')
            {
                ++milk;
                sprintf(str, "the wholesaler delivers flour and milk\n");
                write(STDIN_FILENO,str,strlen(str));
            }
            else if (inputFile[i][1] == 'W')
            {
                ++walnuts;
                sprintf(str, "the wholesaler delivers flour and walnuts\n");
                write(STDIN_FILENO,str,strlen(str));
            }
        }
        else if (inputFile[i][0] == 'M')
        {
            ++milk;
            if (inputFile[i][1] == 'F')
            {
                ++flour;
                sprintf(str, "the wholesaler delivers milk and flour\n");
                write(STDIN_FILENO,str,strlen(str));
            }
            else if (inputFile[i][1] == 'S')
            {
                ++sugar;
                sprintf(str, "the wholesaler delivers milk and sugar\n");
                write(STDIN_FILENO,str,strlen(str));
            }
            else if (inputFile[i][1] == 'W')
            {
                ++walnuts;
                sprintf(str, "the wholesaler delivers milk and walnuts\n");
                write(STDIN_FILENO,str,strlen(str));
            }
        }
        else if (inputFile[i][0] == 'W')
        {
            ++walnuts;
            if (inputFile[i][1] == 'F')
            {
                ++flour;
                sprintf(str, "the wholesaler delivers walnuts and flour\n");
                write(STDIN_FILENO,str,strlen(str));
            }
            else if (inputFile[i][1] == 'S')
            {
                ++sugar;
                sprintf(str, "the wholesaler delivers walnuts and sugar\n");
                write(STDIN_FILENO,str,strlen(str));
            }
            else if (inputFile[i][1] == 'M')
            {
                ++milk;
                sprintf(str, "the wholesaler delivers walnuts and milk\n");
                write(STDIN_FILENO,str,strlen(str));
            }
        }
        if (i == 0)
            for(j = 0; j < 6; ++j)
                sem_post(&semaphore->semStart);

        sem_post(&semaphore->semChef);

        sprintf(str,"the wholesaler is waiting for the dessert\n");
        write(STDIN_FILENO,str,strlen(str));

        sem_wait(&semaphore->semDessert);

        while(condition)
        {
            sem_post(&semaphore->semChef);
            sem_wait(&semaphore->semDessert);
        }

        sprintf(str,"the wholesaler has obtained the dessert and left to sell it\n");
        write(STDIN_FILENO,str,strlen(str));
    }

    sem_post(&semaphore->semStop);
    for(j = 0; j < 6; ++j)
        sem_post(&semaphore->semChef);

    for (i = 0; i < lengthOfInput; ++i)
        free(inputFile[i]);
    free(inputFile);
}