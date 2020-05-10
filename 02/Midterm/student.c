#include "main.h"

extern Semaphore *semaphore;

void student(int num, int L)
{
    int round = 1, counterP = 0, counterC = 0, counterD = 0, EMPTY_TABLE = 0, COUNTER_STUDENTS = 0, SIT_TABLE = 0, LEFT_TABLE = 0;
    fprintf(stderr, "STUDENT : %d\n", num);

    while(round <= L)
    {
        sem_wait(&semaphore->semStudent);

        readSharedMemory("/sharedCounter",&counterP,&counterC,&counterD);

        fprintf(stdout, "Student %d is going to the counter (round %d) - # of students at counter: %d and counter items P:%d,C:%d,D:%d=%d\n",num, round,COUNTER_STUDENTS,counterP,counterC,counterD,(counterP+counterC+counterD));

        if (counterP == 0 || counterC == 0 || counterD == 0)
            sem_wait(&semaphore->semStudent);

        readSharedMemory("/sharedCounter",&counterP,&counterC,&counterD);
        writeToSharedMemory("/sharedCounter",counterP-1,counterC-1,counterD-1);

        //fprintf(stdout, "Student %d got food and is going to get a table (round %d) - # of empty tables: %d\n",num,round,EMPTY_TABLE );

        //fprintf(stdout, "Student %d sat at table %d to eat (round %d) - empty tables:%d\n",num,SIT_TABLE,round,EMPTY_TABLE );

        //fprintf(stdout, "Student %d left table %d to eat again (round %d) - empty tables:%d\n",num,LEFT_TABLE,round,EMPTY_TABLE );
        ++round;
    }
    fprintf(stdout, "Student %d is done eating L=%d times - going home – GOODBYE!!!\n",num,round-1);
    exit(0);
}