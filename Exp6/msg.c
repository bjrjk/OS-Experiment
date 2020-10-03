#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSG_KEY 75
#define MSG_SIZE 20

int msgget(key_t key, int msgflg);
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
int msgctl(int msqid, int cmd, struct msqid_ds *buf);

struct msgbuf
{
    long msgType;
    char msg[MSG_SIZE];
};
typedef struct msgbuf msgBuf;

int openMessageQueue()
{
    int qid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (qid == -1)
    {
        perror("msgget Failed");
        return -1;
    }
    return qid;
}

void server()
{
    msgBuf msgBuffer;
    int msqid = openMessageQueue();
    while (1)
    {
        msgrcv(msqid, &msgBuffer, MSG_SIZE, 0, 0);
        printf("Server: Received Message: Type = %ld\n", msgBuffer.msgType);
        if (msgBuffer.msgType == 1)
        {
            printf("Server: Received Exit Message. Bye!\n");
            msgctl(msqid, IPC_RMID, NULL);
            return;
        }
    }
}

void client()
{
    msgBuf msgBuffer;
    int msqid = openMessageQueue();
    for (int i = 10; i > 0; i--)
    {
        msgBuffer.msgType = i;
        msgsnd(msqid, &msgBuffer, MSG_SIZE, 0);
        printf("Client: Sent Message: Type = %ld\n", msgBuffer.msgType);
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
    forkProcess(server);
    forkProcess(client);
    wait(NULL);
    wait(NULL);
    printf("Main: Process Finished!\n");
    return 0;
}
