/*
    CSE 344 Systems Programming
    Homework 04
    chefs.c
    Omer CEVIK
    161044004
*/

#include "main.h"

extern Semaphore* semaphore;

extern int sugar;
extern int flour;
extern int milk;
extern int walnuts;
extern int condition;

void* chefsFunction(void* ptr)
{
    char str[1024] = "";
    int chefId = 0, stop = 0, chef1 = 0, chef2 = 0, chef3 = 0, chef4 = 0, chef5 = 0, chef6 = 0;
    sem_wait(&semaphore->semId);

    sem_getvalue(&semaphore->semStart,&chefId);
    ++chefId;
    sem_wait(&semaphore->semStart);

    sem_post(&semaphore->semId);

    while(1)
    {
        sem_wait(&semaphore->semChef);

        sem_getvalue(&semaphore->semStop,&stop);
        if (stop > 0)
            break;

        if (chefId == 1)
        {
            if (chef1 == 0)
            {
                sprintf(str, "chef1 is waiting for walnuts and sugar\n");
                write(STDIN_FILENO,str,strlen(str));
            }

            int w = 0;

            if (walnuts > 0 && sugar > 0)
            {
                --walnuts;

                sprintf(str, "chef1 has taken the walnuts\n");
                write(STDIN_FILENO,str,strlen(str));

                w = 1;
            }

            if (walnuts == 0 && sugar > 0 && chef1 == 1)
            {
                --sugar;

                sprintf(str, "chef1 has taken the sugar\n");
                write(STDIN_FILENO,str,strlen(str));

                sprintf(str, "chef1 is preparing the dessert\n");
                write(STDIN_FILENO,str,strlen(str));

                sleep(rand()%5+1);

                sprintf(str, "chef1 has delivered the dessert to the wholesaler\n");
                write(STDIN_FILENO,str,strlen(str));

                chef1 = 0;
                condition = 0;
            }

            if (condition == 1 && w == 1)
                chef1 = 1;

            sem_post(&semaphore->semDessert);
        }
        else if (chefId == 2)
        {
            if (chef2 == 0)
            {
                sprintf(str, "chef2 is waiting for flour and walnuts\n");
                write(STDIN_FILENO,str,strlen(str));
            }

            int f = 0;

            if (flour > 0 && walnuts > 0)
            {
                --flour;

                sprintf(str, "chef2 has taken the flour\n");
                write(STDIN_FILENO,str,strlen(str));

                f = 1;
            }

            if (flour == 0 && walnuts > 0 && chef2 == 1)
            {
                --walnuts;

                sprintf(str, "chef2 has taken the walnuts\n");
                write(STDIN_FILENO,str,strlen(str));

                sprintf(str, "chef2 is preparing the dessert\n");
                write(STDIN_FILENO,str,strlen(str));

                sleep(rand()%5+1);

                sprintf(str, "chef2 has delivered the dessert to the wholesaler\n");
                write(STDIN_FILENO,str,strlen(str));

                chef2 = 0;
                condition = 0;
            }

            if (condition == 1 && f == 1)
                chef2 = 1;
            sem_post(&semaphore->semDessert);
        }
        else if (chefId == 3)
        {
            if (chef3 == 0)
            {
                sprintf(str, "chef3 is waiting for sugar and flour\n");
                write(STDIN_FILENO,str,strlen(str));
            }

            int s = 0;

            if (sugar > 0 && flour > 0)
            {
                --sugar;

                sprintf(str, "chef3 has taken the sugar\n");
                write(STDIN_FILENO,str,strlen(str));

                s = 1;
            }

            if (sugar == 0 && flour > 0 && chef3 == 1)
            {
                --flour;

                sprintf(str, "chef3 has taken the flour\n");
                write(STDIN_FILENO,str,strlen(str));

                sprintf(str, "chef3 is preparing the dessert\n");
                write(STDIN_FILENO,str,strlen(str));

                sleep(rand()%5+1);

                sprintf(str, "chef3 has delivered the dessert to the wholesaler\n");
                write(STDIN_FILENO,str,strlen(str));

                chef3 = 0;
                condition = 0;
            }

            if (condition == 1 && s == 1)
                chef3 = 1;
            sem_post(&semaphore->semDessert);
        }
        else if (chefId == 4)
        {
            if (chef4 == 0)
            {
                sprintf(str, "chef4 is waiting for milk and flour\n");
                write(STDIN_FILENO,str,strlen(str));
            }

            int m = 0;

            if (milk > 0 && flour > 0)
            {
                --milk;

                sprintf(str, "chef4 has taken the milk\n");
                write(STDIN_FILENO,str,strlen(str));

                m = 1;
            }

            if (milk == 0 && flour > 0 && chef4 == 1)
            {
                --flour;

                sprintf(str, "chef4 has taken the flour\n");
                write(STDIN_FILENO,str,strlen(str));

                sprintf(str, "chef4 is preparing the dessert\n");
                write(STDIN_FILENO,str,strlen(str));

                sleep(rand()%5+1);

                sprintf(str, "chef4 has delivered the dessert to the wholesaler\n");
                write(STDIN_FILENO,str,strlen(str));

                chef4 = 0;
                condition = 0;
            }

            if (condition == 1 && m == 1)
                chef4 = 1;
            sem_post(&semaphore->semDessert);
        }
        else if (chefId == 5)
        {
            if (chef5 == 0)
            {
                sprintf(str, "chef5 is waiting for walnuts and milk\n");
                write(STDIN_FILENO,str,strlen(str));
            }

            int w = 0;

            if (walnuts > 0 && milk > 0)
            {
                --walnuts;

                sprintf(str, "chef5 has taken the walnuts\n");
                write(STDIN_FILENO,str,strlen(str));

                w = 1;
            }

            if (walnuts == 0 && milk > 0 && chef5 == 1)
            {
                --milk;

                sprintf(str, "chef5 has taken the milk\n");
                write(STDIN_FILENO,str,strlen(str));

                sprintf(str, "chef5 is preparing the dessert\n");
                write(STDIN_FILENO,str,strlen(str));

                sleep(rand()%5+1);

                sprintf(str, "chef5 has delivered the dessert to the wholesaler\n");
                write(STDIN_FILENO,str,strlen(str));

                chef5 = 0;
                condition = 0;
            }

            if (condition == 1 && w == 1)
                chef5 = 1;
            sem_post(&semaphore->semDessert);
        }
        else if (chefId == 6)
        {
            if (chef6 == 0)
            {
                sprintf(str, "chef6 is waiting for milk and sugar\n");
                write(STDIN_FILENO,str,strlen(str));
            }

            int m = 0;

            if (milk > 0 && sugar > 0)
            {
                --milk;

                sprintf(str, "chef6 has taken the milk\n");
                write(STDIN_FILENO,str,strlen(str));

                m = 1;
            }

            if (milk == 0 && sugar > 0 && chef6 == 1)
            {
                --sugar;

                sprintf(str, "chef6 has taken the sugar\n");
                write(STDIN_FILENO,str,strlen(str));

                sprintf(str, "chef6 is preparing the dessert\n");
                write(STDIN_FILENO,str,strlen(str));

                sleep(rand()%5+1);

                sprintf(str, "chef6 has delivered the dessert to the wholesaler\n");
                write(STDIN_FILENO,str,strlen(str));

                chef6 = 0;
                condition = 0;
            }

            if (condition == 1 && m == 1)
                chef6 = 1;
            sem_post(&semaphore->semDessert);
        }

    }
}