/*
    CSE 344 Systems Programming
    Homework 01
    programB.c
    Omer CEVIK
    161044004
*/

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

struct complex
{
    double r;
    double i;
};

void getArguments(int argc, char *argv[], char** inputPathB, char** outputPathB, int* time);
void fileOperations(char* inputPathB, char* outputPathB, int time);
void parseComplexNumbers(char* singleLine, struct complex complexNumbers[16]);
void fastFourierTransform(int length, struct complex* complexNumbers, struct complex* result);

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        perror("Error:\tUsage: ./programA -i inputPathB -o outputPathB -t time");
        return 1;
    }

    char* inputPathB = NULL;
    char* outputPathB = NULL;
    int time = 0;

    getArguments(argc, argv, &inputPathB, &outputPathB, &time);
    fileOperations(inputPathB, outputPathB, time);

    return 0;
}

void getArguments(int argc, char *argv[], char** inputPathB, char** outputPathB, int* time)
{
    int opt = 0;

    while((opt = getopt(argc, argv, "i:o:t:")) != -1)
    {
        switch(opt)
        {
            case 'i':
            *inputPathB = optarg;
            break;

            case 'o':
            *outputPathB = optarg;
            break;

            case 't':
            sscanf(optarg,"%d",time);
            break;

            default:
            perror("Unknown option!\n");
            exit(1);
        }
    }
}

void fileOperations(char* inputPathB, char* outputPathB, int time)
{
    int fdInput = open(inputPathB, O_RDWR);
    int fdOutput = open(outputPathB, O_WRONLY | O_CREAT | O_APPEND);

    if (fdInput == -1)
    {
        printf("Error Number %d\n", errno);
        perror("Program can not open input file!");
        return;
    }
    if (fdOutput == -1)
    {
        printf("Error Number %d\n", errno);
        perror("Program can not open output file!");
        return;
    }
    if (1 > time || time > 50)
    {
        perror("Time is out of bounds!");
        return;
    }

    sleep(3);

    char readInput[200] = "";
    int index = 0;
    struct complex complexNumbers[16];

    struct flock a_read_mtx;
    memset(&a_read_mtx, 0, sizeof(struct flock));
    struct flock a_write_mtx;
    memset(&a_write_mtx, 0, sizeof(struct flock));
    struct flock b_write_mtx;
    memset(&b_write_mtx, 0, sizeof(struct flock));

    while(1)
    {
        a_read_mtx.l_type = F_RDLCK;
        a_write_mtx.l_type = F_WRLCK;
        b_write_mtx.l_type = F_WRLCK;

        int a_read = fcntl(fdInput, F_SETLKW, &a_read_mtx);
        int a_write = fcntl(fdInput, F_SETLKW, &a_write_mtx);
        int b_write = fcntl(fdOutput, F_SETLKW, &b_write_mtx);

        if (a_read == 0 && a_read == a_write && a_write == b_write)
        {
            char c;

            while( read(fdInput, &c, 1) != 0 )
            {
                readInput[index++] = c;
                if (c == '\n')
                {
                    readInput[index-1] = '\0';

                    parseComplexNumbers(readInput, complexNumbers);


                    char out[50];
                    int i = 0;
                    for (i = 0; i < 16; ++i)
                    {
                        sprintf(out,"%.3f +%.3fi,",complexNumbers[i].r, complexNumbers[i].i);
                        write(fdOutput,out,strlen(out));
                    }
                    write(fdOutput, "\n", sizeof(char));

                    lseek(fdInput,-strlen(readInput)-1,SEEK_CUR);
                    write(fdInput,"DELETED ",8);
                    strcpy(readInput,"");
                    index = 0;
                    char temp;
                    while(read(fdInput, &temp, 1))
                        if (temp == '\n')
                            break;

                    a_read_mtx.l_type = F_UNLCK;
                    a_write_mtx.l_type = F_UNLCK;
                    b_write_mtx.l_type = F_UNLCK;

                    fcntl(fdOutput, F_SETLKW, &b_write_mtx);
                    fcntl(fdInput, F_SETLKW, &a_read_mtx);
                    fcntl(fdInput, F_SETLKW, &a_write_mtx);

                    sleep(time/1000.0);
                }
                if (c == 'D')
                {
                    char temp;
                    while(read(fdInput, &temp, 1))
                        if (temp == '\n')
                            break;
                    strcpy(readInput,"");
                    index = 0;
                }
            }
            break;
        }
    }

    close(fdInput);
    close(fdOutput);
}

void parseComplexNumbers(char* singleLine, struct complex complexNumbers[16])
{
    char token[10];
    int i, k = 0, complexIndex = 0;

    for (i = 0; singleLine[i] != '\0'; ++i)
    {
        token[k++] = singleLine[i];
        if (singleLine[i] == ',')
        {
            token[k-1] = '\0';
            int reel, img;
            char space, plus, chari;
            sscanf(token,"%d%c%c%c%d", &reel, &space, &plus, &chari, &img);

            complexNumbers[complexIndex].r = reel * 1.0;
            complexNumbers[complexIndex].i = img * 1.0;
            ++complexIndex;
            k = 0;
        }
    }

    struct complex results[16];
    fastFourierTransform(16, complexNumbers, results);

    for (i = 0; i < 16; ++i)
    {
        complexNumbers[i].r = results[i].r;
        complexNumbers[i].i = results[i].i;
    }
}

void fastFourierTransform(int length, struct complex *complexNumbers, struct complex *result)
{
    if(length > 1)
    {
        struct complex *rightPart, *leftPart;
        leftPart = result;
        rightPart = result + length/2;

        int i;
        for(i = 0; i < length / 2; ++i)
        {
            leftPart[i] = complexNumbers[2 * i];
            rightPart[i] = complexNumbers[2 * i + 1];
        }

        fastFourierTransform(length / 2, leftPart, complexNumbers);
        fastFourierTransform(length / 2, rightPart, complexNumbers);

        for(i = 0; i < length / 2; ++i)
        {
            double real1 = cos(2 * M_PI * i / length);
            double img1 = -sin(2 * M_PI * i / length);

            double real2 = real1 * rightPart[i].r - img1 * rightPart[i].i;
            double img2 = real1 * rightPart[i].i + img1 * rightPart[i].r;

            complexNumbers[i].r = leftPart[i].r + real2;
            complexNumbers[i].i = leftPart[i].i + img2;

            complexNumbers[i + length / 2].r = leftPart[i].r - real2;
            complexNumbers[i + length / 2].i = leftPart[i].i - img2;
        }
    }
    return;
}