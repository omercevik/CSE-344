/*
    CSE 344 Systems Programming
    Homework 04
    semaphore.c
    Omer CEVIK
    161044004
*/

#include "main.h"

extern Semaphore* semaphore;

void initSemaphores()
{
    char str[1024] = "";
    semaphore = (Semaphore *)mmap(0, sizeof(*semaphore), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

    if (sem_init(&semaphore->semStart,0,0) == -1)
    {
        sprintf(str, "Cannot create semStart semaphore!..\n");
        write(STDERR_FILENO, str, strlen(str));
        exit(EXIT_FAILURE);
    }
    if (sem_init(&semaphore->semId,0,1) == -1)
    {
        sprintf(str, "Cannot create semStart semaphore!..\n");
        write(STDERR_FILENO, str, strlen(str));
        exit(EXIT_FAILURE);
    }
    if (sem_init(&semaphore->semChef,0,0) == -1)
    {
        sprintf(str, "Cannot create semChef semaphore!..\n");
        write(STDERR_FILENO, str, strlen(str));
        exit(EXIT_FAILURE);
    }
    if (sem_init(&semaphore->semDessert,0,0) == -1)
    {
        sprintf(str, "Cannot create semDessert semaphore!..\n");
        write(STDERR_FILENO, str, strlen(str));
        exit(EXIT_FAILURE);
    }
    if (sem_init(&semaphore->semStop,0,0) == -1)
    {
        sprintf(str, "Cannot create semStop semaphore!..\n");
        write(STDERR_FILENO, str, strlen(str));
        exit(EXIT_FAILURE);
    }
}

void destroySemaphores()
{
    sem_destroy(&semaphore->semStart);
    sem_destroy(&semaphore->semChef);
    sem_destroy(&semaphore->semDessert);
    sem_destroy(&semaphore->semId);
    sem_destroy(&semaphore->semStop);
    if (munmap(semaphore, sizeof(*semaphore)) < 0)
    {
        perror("Error: munmap semaphore!");
        exit(EXIT_FAILURE);
    }
}