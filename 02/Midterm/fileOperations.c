#include "main.h"

void readInputFile(char* filePath, int PCD, int* randomValues)
{
    int i = 0, fd = 0;
    char* readFile = (char*)calloc(PCD, sizeof(char));

    fd = open(filePath, O_RDONLY, 0666);
    if (fd == -1)
    {
        char str[256];
        sprintf(str,"Input Open Error Number %d\n", errno);
        write(STDOUT_FILENO, str, strlen(str));
        perror("Program can not open input file!");
        exit(EXIT_FAILURE);
    }
    if(read(fd,readFile,PCD) != PCD)
    {
        char str[256];
        sprintf(str,"Input File Size, Error Number %d\n",errno);
        write(STDOUT_FILENO, str, strlen(str));
        perror("Input file doesn't have enough input!");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < PCD; ++i)
        randomValues[i] = readFile[i] - '0';

    free(readFile);
    close(fd);
}

void createRandomInputFile(char* filePath, int LxM3)
{
    int fd = open(filePath,O_WRONLY,0666);
    if (fd == -1)
    {
        char str[256];
        sprintf(str,"Input Create Open Error Number %d\n", errno);
        write(STDOUT_FILENO, str, strlen(str));
        perror("Program can not open input file!");
        exit(EXIT_FAILURE);
    }

    int P = LxM3/3, C = LxM3/3, D = LxM3/3;

    int random = rand()%3;

    while(LxM3 > 0)
    {
        switch(random)
        {
            case 0:
                    if(P != 0)
                    {
                        write(fd,"0",1);
                        --LxM3;
                        --P;
                    }
            break;

            case 1:
                    if(C != 0)
                    {
                        write(fd,"1",1);
                        --LxM3;
                        --C;
                    }
            break;

            case 2:
                    if(D != 0)
                    {
                        write(fd,"2",1);
                        --LxM3;
                        --D;
                    }
            break;
        }
        random = rand()%3;
    }

    close(fd);
}