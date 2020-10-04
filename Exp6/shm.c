#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSG_KEY 75
#define MSG_SIZE 20

int shmget(key_t key, size_t size, int shmflg);
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
void *shmat(int shmid, const void *shmaddr, int shmflg);
int shmdt(const void *shmaddr);

struct msgbuf
{
    long msgType;
    int msgVisited;
    char msg[MSG_SIZE];
};
typedef struct msgbuf msgBuf;

msgBuf *shmPtr;
int shmid;

int openSharedMemory()
{
    int shmid = shmget(MSG_KEY, sizeof(msgBuf), IPC_CREAT);
    if (shmid == -1)
    {
        perror("shmget Failed");
        return -1;
    }
    return shmid;
}

void server()
{
    while (1)
    {
        while (shmPtr->msgVisited)
            sleep(1);
        printf("Server: Received Message: Type = %ld\n", shmPtr->msgType);
        if (shmPtr->msgType == 1)
        {
            printf("Server: Received Exit Message. Bye!\n");
            shmctl(shmid, IPC_RMID, NULL);
            return;
        }
        shmPtr->msgVisited = 1;
    }
}

void client()
{
    for (int i = 10; i > 0; i--)
    {
        while(!shmPtr->msgVisited)sleep(1);
        shmPtr->msgType = i;
        shmPtr->msgVisited = 0;
        printf("Client: Sent Message: Type = %ld\n", shmPtr->msgType);
    }
    printf("Client: Sent Finished!\n");
}

void forkProcess(void (*func)())
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork Failed");
        exit(1);
    }
    else if (pid == 0)
    {
        func();
        exit(0);
    }
}

int main()
{
    shmid = openSharedMemory();
    shmPtr = shmat(shmid, NULL, 0);
    shmPtr->msgType = -1;
    shmPtr->msgVisited = 1;
    forkProcess(server);
    forkProcess(client);
    wait(NULL);
    wait(NULL);
    printf("Main: Process Finished!\n");
    return 0;
}
