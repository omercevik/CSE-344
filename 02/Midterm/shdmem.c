#include "main.h"

void readSharedMemory(char* whichShdMem, int* counterP, int* counterC, int* counterD)
{
    int shd_mem = 0;
    char *buffer = NULL, space = ' ';

    if ((shd_mem = shm_open(whichShdMem, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0)
    {
        perror("Error: shm_open shd_mem!");
        exit(EXIT_FAILURE);
    }
    if ((buffer = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, shd_mem, 0)) == NULL)
    {
        perror("Error: mmap buffer!");
        exit(EXIT_FAILURE);
    }

    sscanf(buffer,"%d%c%d%c%d",counterP,&space,counterC,&space,counterD);
    close(shd_mem);
}

void writeToSharedMemory(char* whichShdMem, int counterP, int counterC, int counterD)
{
    int shd_mem = 0;
    char *buffer = NULL;

    if ((shd_mem = shm_open(whichShdMem, O_RDWR|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0)
    {
        perror("Error: shm_open shd_mem!");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shd_mem, 4096) < 0)
    {
        perror("Error: ftruncate shd_mem!");
        exit(EXIT_FAILURE);
    }
    if ((buffer = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, shd_mem, 0)) == NULL)
    {
        perror("Error: mmap buffer!");
        exit(EXIT_FAILURE);
    }

    sprintf(buffer, "%d %d %d",counterP,counterC,counterD);

    if (munmap(buffer, 4096) < 0)
    {
        perror("Error: munmap buffer!");
        exit(EXIT_FAILURE);
    }
    close(shd_mem);
}