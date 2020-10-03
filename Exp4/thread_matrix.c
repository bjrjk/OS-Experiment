#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define NUM_THREADS 10

struct Argument
{
    int tid;
};
typedef struct Argument Arg;

int A[NUM_THREADS][NUM_THREADS], B[NUM_THREADS][NUM_THREADS], C[NUM_THREADS][NUM_THREADS];

void *threadFunction(void *t)
{
    Arg *argPtr = (Arg *)t;
    int i = argPtr->tid;
    for (int j = 0; j < NUM_THREADS; j++)
    {
        C[i][j] = 0;
        for (int k = 0; k < NUM_THREADS; k++)
        {
            C[i][j] += A[i][k] * B[k][j];
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t thread[NUM_THREADS];
    Arg arguments[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long t;
    void *status;

    srand(time(NULL));
    for (int i = 0; i < NUM_THREADS; i++)
    {
        for (int j = 0; j < NUM_THREADS; j++)
        {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }
    printf("Matrix A: (Size=%d) \n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        for (int j = 0; j < NUM_THREADS; j++)
        {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("Matrix B: (Size=%d) \n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        for (int j = 0; j < NUM_THREADS; j++)
        {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (t = 0; t < NUM_THREADS; t++)
    {
        printf("Main: creating thread %ld\n", t);
        arguments[t].tid = t;
        rc = pthread_create(&thread[t], &attr, threadFunction, &arguments[t]);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is % d\n", rc);
            exit(-1);
        }
    }

    pthread_attr_destroy(&attr);
    for (t = 0; t < NUM_THREADS; t++)
    {
        rc = pthread_join(thread[t], &status);
        if (rc)
        {
            printf("ERROR; return code from pthread_join() is % d\n", rc);
            exit(-1);
        }
        printf("Main: completed join with thread %ld having a status of % ld\n",
               t, (long)status);
    }

    printf("Matrix C: (Size=%d) \n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        for (int j = 0; j < NUM_THREADS; j++)
        {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    printf("Main: program completed. Exiting.\n");
    pthread_exit(NULL);
}
