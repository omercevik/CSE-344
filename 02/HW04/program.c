/*
    CSE 344 Systems Programming
    Homework 04
    program.c
    Omer CEVIK
    161044004
*/

#include "main.h"

Semaphore* semaphore;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        perror("Error:\tUsage: ./program -i filePath.txt");
        return -1;
    }

    char* filePath = NULL, str[1024] = "";
    int i = 0;

    getArguments(argc, argv, &filePath);

    initSemaphores();

    srand(time(0));

    pthread_t wholesaler;
    pthread_t chefThreads[6];

    pthread_create(&wholesaler, NULL, wholesalerFunction, (void*)(filePath));

    for (i = 0; i < 6; ++i)
        pthread_create(&chefThreads[i], NULL, chefsFunction, NULL);

    for (i = 0; i < 6; ++i)
        pthread_join(chefThreads[i], NULL);

    pthread_join(wholesaler, NULL);

    destroySemaphores();

    return 0;
}

void readInputFile(char* filePath, char*** inputFile, int* lengthOfInput)
{
    int i = 0, fd = 0, length = 0;
    char readFile[3] = "", newLine = '\n';

    fd = open(filePath, O_RDONLY, 0666);
    if (fd == -1)
    {
        char str[256];
        sprintf(str,"Input Open Error Number %d\n", errno);
        write(STDERR_FILENO, str, strlen(str));
        perror("Program can not open input file!");
        exit(EXIT_FAILURE);
    }

    while(read(fd,readFile,2) == 2)
    {
        ++length;
        read(fd,&newLine,1);
    }

    lseek(fd,0L,SEEK_SET);
    (*inputFile) = (char**)calloc(length, sizeof(char*));
    for (i = 0; i < length; ++i)
        (*inputFile)[i] = (char*)calloc(3, sizeof(char));

    i = 0;

    while(read(fd,readFile,2) == 2)
    {
        strcpy((*inputFile)[i],readFile);
        read(fd,&newLine,1);
        ++i;
    }

    *lengthOfInput = length;

    close(fd);
}

void getArguments(int argc, char *argv[], char** filePath)
{
    int opt = 0;

    while((opt = getopt(argc, argv, "i:")) != -1)
    {
        if (opt == 'i')
            *filePath = optarg;
        else
        {
            perror("Unknown option!");
            exit(EXIT_FAILURE);
        }
    }
}