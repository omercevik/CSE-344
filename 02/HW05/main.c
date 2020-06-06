/*
    CSE 344 Systems Programming
    Homework 05
    main.c
    Omer CEVIK
    161044004
*/
#include "main.h"

static void signalHandler(int signo)
{
    if (signo == SIGINT)
    {
        write(STDOUT_FILENO,"\nCTRL-C Signal is caught!\n",strlen("\nCTRL-C Signal is caught!\n"));
        exit(0);
    }
}

struct florist florists[1024];
int floristSize = 0;
int fd = 0;
struct Queue queue[1024];
extern int fileCondition;
extern int condition;

pthread_cond_t cond;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        perror("Error:\tUsage: ./floristApp -i data.dat");
        return -1;
    }

    char* filePath = NULL, str[1024] = "";
    int i = 0, j = 0;

    srand(time(NULL));

    struct sigaction act;
    memset (&act, 0, sizeof(act));
    act.sa_handler = signalHandler;
    act.sa_flags = 0;

    if (sigaction(SIGINT, &act, NULL) < 0)
    {
        perror("Error! Sigaction CTRL C!");
        exit(EXIT_FAILURE);
    }

    getArguments(argc, argv, &filePath);

    pthread_cond_init(&cond, 0);

    initFlorists(filePath);

    for (i = 0; i < floristSize; ++i)
    {
        (queue[i]).fQueue = NULL;
        (queue[i]).lQueue = NULL;
    }

    sprintf(str, "%d florists have been created\n",floristSize );
    write(STDOUT_FILENO,str,strlen(str));

    sprintf(str, "Processing requests\n");
    write(STDOUT_FILENO,str,strlen(str));

    pthread_t floristThreads[floristSize];

    int floristIndexArray[floristSize];
    for (i = 0; i < floristSize; ++i)
        floristIndexArray[i] = i;

    for (i = 0; i < floristSize; ++i)
        pthread_create(&floristThreads[i], NULL, &floristFunction, (void*)&floristIndexArray[i]);


    while(fileCondition)
    {
        struct client client;
        initClient(&client);
        int fi = 0;
        double timeOfPreparation = 9999.9;
        for (i = 0; i < floristSize; ++i)
        {
            for (j = 0; j < florists[i].flowerSize; ++j)
            {
                if (strcmp(florists[i].flowers[j], client.flower) == 0 && timeOfPreparation > getChevyDistance(florists[i], client))
                {
                    timeOfPreparation = getChevyDistance(florists[i], client);
                    fi = i;
                }
            }
        }

        insert(&queue[fi],client,timeOfPreparation);
        if (queue[fi].fQueue == NULL)
            pthread_cond_broadcast(&cond);
    }

    condition = 0;

    pthread_cond_broadcast(&cond);

    sprintf(str, "All requests processed.\n");
    write(STDOUT_FILENO,str,strlen(str));

    for (i = 0; i < floristSize; ++i)
        pthread_join(floristThreads[i], NULL);


    for (i = 0; i < floristSize; ++i)
    {
        sprintf(str,"%s closing shop.\n",florists[i].name);
        write(STDOUT_FILENO,str,strlen(str));
    }

    sprintf(str, "Sale statistics for today:\n" );
    write(STDOUT_FILENO,str,strlen(str));

    sprintf(str, "-------------------------------------------------\n" );
    write(STDOUT_FILENO,str,strlen(str));

    sprintf(str, "Florist\t\t# of sales\tTotal time\n" );
    write(STDOUT_FILENO,str,strlen(str));

    sprintf(str, "-------------------------------------------------\n" );
    write(STDOUT_FILENO,str,strlen(str));

    for (i = 0; i < floristSize; ++i)
    {
        sprintf(str,"%s\t\t%d\t\t%dms\n",florists[i].name, florists[i].sales, florists[i].totalTimes);
        write(STDOUT_FILENO,str,strlen(str));
    }

    sprintf(str, "-------------------------------------------------\n" );
    write(STDOUT_FILENO,str,strlen(str));

    close(fd);

    pthread_mutex_destroy(&lock);

    return 0;
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