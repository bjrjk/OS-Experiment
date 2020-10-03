#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREADS 2

struct Argument
{
    int tid;
    int i;
    char c;
};
typedef struct Argument Arg;

void *threadFunction(void *t)
{
    Arg *argPtr = (Arg *)t;
    printf("Thread %d: IntNum: %d, Char: %c.\n", argPtr->tid, argPtr->i, argPtr->c);
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

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (t = 0; t < NUM_THREADS; t++)
    {
        printf("Main: creating thread %ld\n", t);
        arguments[t].tid = t;
        arguments[t].i = t + 1;
        arguments[t].c = 'a' + t;
        rc = pthread_create(&thread[t], &attr, threadFunction, &arguments[t]);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is % d\n", rc);
            exit(-1);
        }
    }

    /* Free attribute and wait for the other threads */
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

    printf("Main: program completed. Exiting.\n");
    pthread_exit(NULL);
}
