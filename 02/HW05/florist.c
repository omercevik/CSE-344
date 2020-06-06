/*
    CSE 344 Systems Programming
    Homework 05
    florist.c
    Omer CEVIK
    161044004
*/
#include "main.h"

extern struct florist florists[1024];
extern int floristSize;
extern int fd;

extern pthread_mutex_t lock;
extern pthread_cond_t cond;
extern struct Queue queue[1024];
int fileCondition = 1;
int condition = 1;

void* floristFunction(void* fiVoid)
{
    int *fiPtr = 0, fi = 0;
    char str[1024] = "";

    while(1)
    {
        pthread_mutex_lock(&lock);
        fiPtr = (int*)fiVoid;
        fi = *fiPtr;
        while(queue[fi].fQueue == NULL && condition == 1)
            pthread_cond_wait (&cond, &lock);

        if (queue[fi].fQueue == NULL)
        {
            pthread_exit(NULL);
            break;
        }

        int timeOfDelivery = rand() % 250 + 1;
        int timeOfPreparation = queue[fi].fQueue->queueDistance + timeOfDelivery;

        ++florists[fi].sales;
        florists[fi].totalTimes += timeOfPreparation;

        sprintf(str, "Florist %s has delivered a %s to %s in %dms\n",florists[fi].name,queue[fi].fQueue->flower,queue[fi].fQueue->clientName,(int)timeOfPreparation);
        write(STDIN_FILENO, str, strlen(str));

        pop(&queue[fi]);

        pthread_mutex_unlock(&lock);
        if (queue[fi].fQueue == NULL && condition == 0)
        {
            pthread_exit(NULL);
            break;
        }
    }
    return NULL;
}

double getChevyDistance(struct florist florist, struct client client)
{
    double x = (florist.x - client.x)*florist.click;
    double y = (florist.y - client.y)*florist.click;

    x = x < 0 ? -x : x;
    y = y < 0 ? -y : y;

    return x > y ? x : y;
}

void initFlorists(char* filePath)
{
    int j = 0, fi = 0;
    char c = ' ', endc = ' ', str[256] = "", floristString[4096] = "";

    fd = open(filePath, O_RDONLY, 0666);
    if (fd == -1)
    {
        sprintf(str,"Input Open Error Number %d\n", errno);
        write(STDERR_FILENO, str, strlen(str));
        perror("Program can not open input file!");
        exit(EXIT_FAILURE);
    }

    sprintf(str,"Florist application initializing from file: %s\n",filePath);
    write(STDIN_FILENO, str, strlen(str));

    while(read(fd,&c,1))
    {
        read(fd,&endc,1);
        lseek(fd,-1,SEEK_CUR);
        if (c == '\n')
        {
            floristString[j] = '\0';
            parseFlorist(floristString,&florists[fi]);
            florists[fi].sales = 0;
            florists[fi].totalTimes = 0;
            ++fi;
            floristSize = fi;
            j = 0;
            if (endc == '\n' && c == '\n')
                break;
        }
        else
            floristString[j++] = c;
    }
    lseek(fd,1,SEEK_CUR);
}

void parseFlorist(char* string, struct florist* florist)
{
    int i = 0, j = 0, flowersIndex = 0;
    char openParanthese = '(', closeParanthese = ')', comma = ',', space = ' ', semicolon = ';';

    for (i = 0; string[i] != ' '; ++i)
        florist->name[i] = string[i];

    florist->name[i] = '\0';

    ++i;

    sscanf(&string[i],"%c%d%c%d%c%c%lf%c",&openParanthese,&florist->x,&comma,&florist->y,&semicolon,&space,&florist->click,&closeParanthese);

    for (; string[i] !=  ':'; ++i);

    i += 2;

    for (; string[i] != '\0'; ++i)
    {
        if (string[i] == ',')
        {
            florist->flowers[flowersIndex][j] = '\0';
            j = 0;
            ++i;
            ++flowersIndex;
            continue;
        }
        florist->flowers[flowersIndex][j++] = string[i];
    }

    florist->flowerSize = flowersIndex+1;
}