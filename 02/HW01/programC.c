/*
    CSE 344 Systems Programming
    Homework 01
    programC.c
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

#define COMPLEX_SIZE 16
#define MAX_SIZE 1000

struct complex
{
    double r;
    double i;
};

void getArguments(int argc, char *argv[], char** inputPathC);
void fileOperations(char* inputPathC);
void parseComplexNumbers(char inputLines[][MAX_SIZE], int lineSize, struct complex complexNumbers[][COMPLEX_SIZE]);
double findComplexNumberDistance(struct complex cNumber);
double findLineAverage(struct complex* complexNumbers);
void sortableAverages(struct complex complexNumbers[][COMPLEX_SIZE], double* sortNumbers, int lineSize);
void merge(double arr[], int l, int m, int r);
void mergeSort(double arr[], int l, int r);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        write(STDERR_FILENO, "Error:\tUsage: ./programC -i inputPathC\n", strlen("Error:\tUsage: ./programC -i inputPathC\n"));
        return 1;
    }

    char* inputPathC = NULL;

    getArguments(argc, argv, &inputPathC);
    fileOperations(inputPathC);

    return 0;
}

void getArguments(int argc, char *argv[], char** inputPathC)
{
    int opt = 0;

    while((opt = getopt(argc, argv, "i:")) != -1)
    {
        switch(opt)
        {
            case 'i':
            *inputPathC = optarg;
            break;

            default:
            write(STDERR_FILENO, "Unknown option!\n", strlen("Unknown option!\n"));
            exit(1);
        }
    }
}

void fileOperations(char* inputPathC)
{
    int fdInput = open(inputPathC, O_RDWR);

    if (fdInput == -1)
    {
        printf("Error Number %d\n", errno);
        perror("Program can not open input file!");
        exit(1);
    }

    char c;
    int i = 0, j = 0, k = 0;

    int lineSize = 0;
    while( read(fdInput, &c, 1) != 0 )
        if (c == '\n')
            ++lineSize;

    lseek(fdInput, 0L, SEEK_SET);

    char inputLines[lineSize][MAX_SIZE];
    struct complex complexNumbers[lineSize][COMPLEX_SIZE];

    int max = 0;
    while( read(fdInput, &c, 1) != 0 )
    {
        inputLines[i][j++] = c;
        if (c == '\n')
        {
            inputLines[i][j-1] = '\0';
            if (max < strlen(inputLines[i]))
                max = strlen(inputLines[i]);
            j = 0;
            ++i;
        }
    }

    lseek(fdInput,0L,SEEK_SET);

    for (i = 0; i < lineSize; ++i)
    {
        write(fdInput,inputLines[i],strlen(inputLines[i]));
        j = strlen(inputLines[i]);
        while(j++ < max)
            write(fdInput," ",sizeof(char));
        write(fdInput,"\n",sizeof(char));
    }
    ++max;

    parseComplexNumbers(inputLines, lineSize, complexNumbers);

    double sortNumbers[lineSize];
    double temp[lineSize];

    sortableAverages(complexNumbers, sortNumbers, lineSize);

    int indexes[lineSize];
    for (i = 0; i < lineSize; ++i)
    {
        indexes[i] = i;
        temp[i] = sortNumbers[i];
    }

    mergeSort(sortNumbers, 0, lineSize-1);

    for (i = 0; i < lineSize; ++i)
    {
        for (j = 0; j < lineSize; ++j)
        {
            if (sortNumbers[i] == temp[j])
            {
                indexes[k++] = j;
                break;
            }
        }
    }

    lseek(fdInput, 0L, SEEK_SET);

    int tdes = open("temp.txt", O_CREAT | O_RDWR);
    if (tdes == -1)
    {
        perror("Not created temp!");
    }

    char cc;
    while( read(fdInput,&cc,1) != 0 )
        write(tdes,&cc,1);

    lseek(fdInput, 0L, SEEK_SET);
    lseek(tdes, 0L, SEEK_SET);

    for (i = 0; i < lineSize; ++i)
    {
        lseek(fdInput, max * i, SEEK_SET);
        char line[max];
        read(fdInput, line, max);

        lseek(tdes, max * indexes[i], SEEK_SET);
        char temp[max];
        read(tdes, temp, max);

        lseek(fdInput, max * i, SEEK_SET);
        write(fdInput, temp, max);
        fsync(fdInput);
    }

    close(fdInput);
    close(tdes);

    remove("temp.txt");
}

void parseComplexNumbers(char inputLines[][MAX_SIZE], int lineSize, struct complex complexNumbers[][COMPLEX_SIZE])
{
    int i = 0, j = 0, k = 0, ci = 0, cj = 0;

    for (i = 0; i < lineSize; ++i)
    {
        char token[256];
        for (j = 0; inputLines[i][j] != '\0'; ++j)
        {
            token[k++] = inputLines[i][j];
            if (inputLines[i][j] == ',')
            {
                token[k-1] = '\0';
                double reel, img;
                char space, plus, chari;
                sscanf(token,"%lf%c%c%lf%c", &reel, &space, &plus, &img, &chari);
                complexNumbers[ci][cj].r = reel;
                complexNumbers[ci][cj].i = img;
                ++cj;
                k = 0;
            }
        }
        ++ci;
        cj = 0;
    }
}

void merge(double arr[], int l, int m, int r)
{
    int i, j, k = l;
    int leftIndex = m - l + 1;
    int rightIndex =  r - m;

    double L[leftIndex], R[rightIndex];

    for (i = 0; i < leftIndex; ++i)
        L[i] = arr[l + i];

    for (j = 0; j < rightIndex; ++j)
        R[j] = arr[m + 1 + j];

    i = j = 0;

    while (i < leftIndex && j < rightIndex)
        arr[k++] = L[i] <= R[j] ? L[i++] : R[j++];

    while (i < leftIndex)
        arr[k++] = L[i++];

    while (j < rightIndex)
        arr[k++] = R[j++];
}

void mergeSort(double arr[], int l, int r)
{
    if (l < r)
    {
        int m = (r + l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}

void sortableAverages(struct complex complexNumbers[][COMPLEX_SIZE], double* sortNumbers, int lineSize)
{
    int i = 0;

    for (i = 0; i < lineSize; ++i)
        sortNumbers[i] = findLineAverage(complexNumbers[i]);
}

double findLineAverage(struct complex* complexNumbers)
{
    double avr = 0.0;
    int i = 0;

    for (i = 0; i < COMPLEX_SIZE; ++i)
    {
        avr += findComplexNumberDistance(complexNumbers[i]);
    }

    return avr / COMPLEX_SIZE;
}

double findComplexNumberDistance(struct complex cNumber)
{
    return sqrt(cNumber.r * cNumber.r + cNumber.i * cNumber.i);
}