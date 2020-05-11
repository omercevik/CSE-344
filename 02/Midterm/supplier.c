/*
    CSE 344 Systems Programming
    Midterm Project
    supplier.c
    Omer CEVIK
    161044004
*/

#include "main.h"

extern Semaphore *semaphore;

void supplier(int P, int C, int D, char* filePath, int Ncooks)
{
    int counterP = 0, counterC = 0, counterD = 0, PCD = P+C+D, i = 0;
    int* randomValues = (int*)calloc(PCD, sizeof(int));
    char str[1024];
    strcpy(str,"");

    readInputFile(filePath, PCD, randomValues);

    while( (counterP + counterC + counterD) < PCD )
    {
        sem_wait(&semaphore->semCook);

        if (i == PCD)
        {
            while(Ncooks--)
            {
                sem_post(&semaphore->semCookStudent);
                sem_post(&semaphore->semSupplier);
            }
            break;
        }

        // Read Kitchen
        readSharedMemory("/sharedKitchen",&counterP,&counterC,&counterD);

        // P (Soup) = 0, C (Course) = 1, D (Desert) = 2
        switch(randomValues[i])
        {
            case 0: sprintf(str, "The supplier is going to the kitchen to deliver soup: kitchen items P:%d,C:%d,D:%d=%d\n",counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
                    ++counterP;
            break;

            case 1: sprintf(str, "The supplier is going to the kitchen to deliver course: kitchen items P:%d,C:%d,D:%d=%d\n",counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
                    ++counterC;
            break;

            case 2: sprintf(str, "The supplier is going to the kitchen to deliver desert: kitchen items P:%d,C:%d,D:%d=%d\n",counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
                    ++counterD;
            break;
        }

        // Write Kitchen
        writeToSharedMemory("/sharedKitchen",counterP,counterC,counterD);

        // Read Kitchen
        readSharedMemory("/sharedKitchen",&counterP,&counterC,&counterD);

        switch(randomValues[i])
        {
            case 0: sprintf(str, "The supplier delivered soup – after delivery: kitchen items P:%d,C:%d,D:%d=%d\n",counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
            break;

            case 1: sprintf(str, "The supplier delivered course – after delivery: kitchen items P:%d,C:%d,D:%d=%d\n",counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
            break;

            case 2: sprintf(str, "The supplier delivered desert – after delivery: kitchen items P:%d,C:%d,D:%d=%d\n",counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
            break;
        }
        ++i;
        sem_post(&semaphore->semSupplier);
    }

    sprintf(str, "The supplier finished supplying – GOODBYE!\n");
    write(STDOUT_FILENO, str, strlen(str));
    free(randomValues);
}