// Reference: https://www.cnblogs.com/wuyepeng/p/9748552.html

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

sem_t poolMutex, poolSema;
sem_t trashSema[4];
int poolSize, wasteSize[4];
char wasteName[4][20] = {
    "Kitchen Waste",
    "Recyclable Waste",
    "Hazardous Waste",
    "Other Waste",
};
const int BUFFER_SIZE = 5;

void *trashProducer(void *t)
{
    int producerID = (int)t;
    while (1)
    {
        sem_wait(&poolSema);
        sem_wait(&poolMutex);
        int trashType = rand() % 4;
        printf("Producer %d: Before produce a new %s, trash size = %d.\n", producerID, wasteName[trashType], wasteSize[trashType]++);
        sem_post(&poolMutex);
        sem_post(&trashSema[trashType]);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *trashRecycler(void *t)
{
    int wasteID = (int)t;
    int accumulateSum = 0;
    while (1)
    {
        sem_wait(&trashSema[wasteID]);
        sem_wait(&poolMutex);
        printf("Consumer %d: Before consume a new %s, trash size = %d, current accumulated trash = %d.\n", wasteID, wasteName[wasteID], wasteSize[wasteID]--, accumulateSum);
	accumulateSum++;
        sem_post(&poolMutex);
        sem_post(&poolSema);
        if (accumulateSum == 10)
        {
            printf("Consumer %d: Collected %s sent to garbage dump.\n", wasteID, wasteName[wasteID]);
            accumulateSum = 0;
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t producers[2], consumers[4];

    sem_init(&poolMutex, 0, 1);
    sem_init(&poolSema, 0, BUFFER_SIZE);
    for (int i = 0; i < 4; i++)
        sem_init(&trashSema[i], 0, 0);

    for (int i = 0; i < 2; i++)
        pthread_create(&producers[i], NULL, trashProducer, (void *)i);
    for (int i = 0; i < 4; i++)
        pthread_create(&consumers[i], NULL, trashRecycler, (void *)i);

    alarm(30);

    pthread_exit(NULL);
}
