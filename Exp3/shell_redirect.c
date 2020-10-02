#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

const char prompt[] = "JackShell > ";
char *argsPtr[128];
char *stdinFile, *stdoutFile;

void printArgs()
{
    int cnter = 0;
    while (argsPtr[cnter])
    {
        printf("JackShell: Arg %d: %s\n", cnter, argsPtr[cnter]);
        cnter++;
    }
}

int processCommand(char str[])
{
    stdinFile = stdoutFile = NULL;
    int cnter = 0;
    char *buf = str, *ptr = NULL;
    while (argsPtr[cnter++] = strtok_r(buf, " ", &ptr))
        buf = NULL;
    for (int i = 0; i < cnter-1; i++)
    {
        if (strcmp(argsPtr[i], "<") == 0)
        {
            stdinFile = argsPtr[i + 1];
            printf("JackShell: STDIN Redirected to %s.\n",stdinFile);
            argsPtr[i] = NULL;
        }
        else if (strcmp(argsPtr[i], ">") == 0)
        {
            stdoutFile = argsPtr[i + 1];
            printf("JackShell: STDOUT Redirected to %s.\n",stdoutFile);
            argsPtr[i] = NULL;
        }
    }
    printArgs();
    return cnter;
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
        if (stdinFile)
            freopen(stdinFile, "r", stdin);
        if (stdoutFile)
            freopen(stdoutFile, "w", stdout);
        int result;
        result = execvp(argsPtr[0], argsPtr);
        perror("Failed to Execute");
        exit(result);
    }
    else
    {
        int retCode;
        wait(&retCode);
        printf("JackShell: Program Return Code: %d\n", WEXITSTATUS(retCode));
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
        process();
    return (EXIT_SUCCESS);
}
