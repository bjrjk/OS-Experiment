#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

const char prompt[] = "JackShell > ";
char *argsPtr[128];

int processCommand(char str[])
{
    int cnter = 0;
    char *buf = str, *ptr = NULL;
    while (argsPtr[cnter++] = strtok_r(buf, " ", &ptr))
        buf = NULL;
    return cnter;
}

void printArgs()
{
    int cnter = 0;
    while (argsPtr[cnter])
    {
        printf("Arg %d: %s\n", cnter, argsPtr[cnter]);
        cnter++;
    }
}

void forkNew(char str[])
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork Failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        processCommand(str);
        int result;
        result = execvp(argsPtr[0], argsPtr);
        perror("Failed to Execute");
        exit(result);
    }
    else
    {
        int retCode;
        wait(&retCode);
        printf("Return Code: %d\n", WEXITSTATUS(retCode));
    }
}

void process()
{
    const int BUF_SIZE = 255;
    char buf[BUF_SIZE];
    printf(prompt);
    char *p = fgets(buf, BUF_SIZE, stdin);
    if (p == NULL)
    {
        perror("fgets Error");
        exit(1);
    }
    buf[strlen(buf) - 1] = '\0';
    if (strcmp(buf, "quit") == 0)
    {
        printf("Bye!\n");
        exit(0);
    }
    forkNew(buf);
}

int main()
{
    printf("JackShell, Powered By Jack Ren.\n");
    while (1)
    {
        process();
    }

    return (EXIT_SUCCESS);
}
