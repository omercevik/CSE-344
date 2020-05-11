/*
    CSE 344 Systems Programming
    Midterm Project
    cook.c
    Omer CEVIK
    161044004
*/

#include "main.h"

extern Semaphore *semaphore;

void cook(int num, int PCD)
{
    int counterP = 0, counterC = 0, counterD = 0, tP = 0, tC = 0, tD = 0;
    char str[1024];
    strcpy(str,"");

    while(1)
    {
        // Read Kitchen and wait
        readSharedMemory("/sharedKitchen",&tP,&tC,&tD);

        sprintf(str, "Cook %d is going to the kitchen to wait for/get a plate - kitchen items P:%d,C:%d,D:%d=%d\n",num,tP,tC,tD,(tP+tC+tD));

        // Supplier wait
        sem_wait(&semaphore->semSupplier);
        sem_wait(&semaphore->semCookStudent);

        // Read Counter
        readSharedMemory("/sharedCounter",&counterP,&counterC,&counterD);

        readSharedMemory("/sharedKitchen",&tP,&tC,&tD);
        int x = rand()%3;
        while(tP != 0 || tC != 0 || tD != 0)
        {
            if ((tP != 0 && x == 0)
             || (tC != 0 && x == 1)
             || (tD != 0 && x == 2))
            {
                break;
            }
            x = rand()%3;
        }
        if (tP == 0 && tC == 0 && tD == 0)
        {
            break;
        }

        // P (Soup) = 0, C (Course) = 1, D (Desert) = 2
        switch(x)
        {
            case 0: sprintf(str, "Cook %d is going to the counter to deliver soup – counter items P:%d,C:%d,D:%d=%d\n",num,counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
                    writeToSharedMemory("/sharedKitchen",tP-1,tC,tD);
                    ++counterP;
            break;

            case 1: sprintf(str, "Cook %d is going to the counter to deliver course – counter items P:%d,C:%d,D:%d=%d\n",num,counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
                    writeToSharedMemory("/sharedKitchen",tP,tC-1,tD);
                    ++counterC;
            break;

            case 2: sprintf(str, "Cook %d is going to the counter to deliver desert – counter items P:%d,C:%d,D:%d=%d\n",num,counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
                    writeToSharedMemory("/sharedKitchen",tP,tC,tD-1);
                    ++counterD;
            break;
        }

        // Write to counter
        writeToSharedMemory("/sharedCounter",counterP,counterC,counterD);

        readSharedMemory("/sharedCounter",&counterP,&counterC,&counterD);

        switch(x)
        {
            case 0: sprintf(str, "Cook %d placed soup on the counter – counter items P:%d,C:%d,D:%d=%d\n",num,counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
            break;

            case 1: sprintf(str, "Cook %d placed course on the counter – counter items P:%d,C:%d,D:%d=%d\n",num,counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
            break;

            case 2: sprintf(str, "Cook %d placed desert on the counter – counter items P:%d,C:%d,D:%d=%d\n",num,counterP,counterC,counterD,(counterP+counterC+counterD));
                    write(STDOUT_FILENO, str, strlen(str));
            break;
        }

        readSharedMemory("/sharedKitchen",&tP,&tC,&tD);

        if ((tP == 0 && tC == 0 && tD == 0) && ((counterP + counterC + counterD) == PCD))
        {
            sem_post(&semaphore->semCook);
            sem_post(&semaphore->semStudent);
            break;
        }

        sem_post(&semaphore->semCook);
        sem_post(&semaphore->semStudent);
    }
    readSharedMemory("/sharedKitchen",&counterP,&counterC,&counterD);
    sprintf(str, "Cook %d finished serving - items at kitchen: %d – going home – GOODBYE!!!\n",num,(counterP+counterC+counterD));
    write(STDOUT_FILENO, str, strlen(str));
    exit(0);
}