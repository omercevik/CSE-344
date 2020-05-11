/*
    CSE 344 Systems Programming
    Midterm Project
    student.c
    Omer CEVIK
    161044004
*/

#include "main.h"

extern Semaphore *semaphore;

void student(int num, int L)
{
    int round = 1, counterP = 0, counterC = 0, counterD = 0;
    int EMPTY_TABLE = 0, COUNTER_STUDENTS = 0, SIT_TABLE = 0;
    char str[1024];
    strcpy(str,"");

    while(round <= L)
    {
        sem_getvalue(&semaphore->semCookStudent, &COUNTER_STUDENTS);

        sem_wait(&semaphore->semStudent);

        readSharedMemory("/sharedCounter",&counterP,&counterC,&counterD);
        readSharedMemory("/sharedTables",&EMPTY_TABLE,&EMPTY_TABLE,&EMPTY_TABLE);

        sprintf(str, "Student %d is going to the counter (round %d) - # of students at counter: %d and counter items P:%d,C:%d,D:%d=%d\n",num, round,COUNTER_STUDENTS,counterP,counterC,counterD,(counterP+counterC+counterD));
        write(STDOUT_FILENO, str, strlen(str));


        while (counterP == 0 || counterC == 0 || counterD == 0 || EMPTY_TABLE == 0)
        {
            sem_post(&semaphore->semCookStudent);
            sem_wait(&semaphore->semStudent);
            readSharedMemory("/sharedCounter",&counterP,&counterC,&counterD);
            readSharedMemory("/sharedTables",&EMPTY_TABLE,&EMPTY_TABLE,&EMPTY_TABLE);
        }

        writeToSharedMemory("/sharedCounter",counterP-1,counterC-1,counterD-1);

        sprintf(str, "Student %d got food and is going to get a table (round %d) - # of empty tables: %d\n",num,round,EMPTY_TABLE );
        write(STDOUT_FILENO, str, strlen(str));

        writeToSharedMemory("/sharedTables",EMPTY_TABLE-1,EMPTY_TABLE-1,EMPTY_TABLE-1);
        readSharedMemory("/sharedTables",&EMPTY_TABLE,&EMPTY_TABLE,&EMPTY_TABLE);

        SIT_TABLE = EMPTY_TABLE;
        sprintf(str, "Student %d sat at table %d to eat (round %d) - empty tables:%d\n",num,SIT_TABLE,round,EMPTY_TABLE );
        write(STDOUT_FILENO, str, strlen(str));

        readSharedMemory("/sharedTables",&EMPTY_TABLE,&EMPTY_TABLE,&EMPTY_TABLE);
        writeToSharedMemory("/sharedTables",EMPTY_TABLE+1,EMPTY_TABLE+1,EMPTY_TABLE+1);

        sprintf(str, "Student %d left table %d to eat again (round %d) - empty tables:%d\n",num,SIT_TABLE,round,EMPTY_TABLE );
        write(STDOUT_FILENO, str, strlen(str));
        ++round;

        sem_post(&semaphore->semCookStudent);
    }

    sprintf(str, "Student %d is done eating L=%d times - going home – GOODBYE!!!\n",num,round-1);
    write(STDOUT_FILENO, str, strlen(str));

    exit(0);
}