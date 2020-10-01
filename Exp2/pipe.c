#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define INPUT 0  //Read End of Pipe
#define OUTPUT 1 //Write End of Pipe

int fileDesc[2];

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
        int result;
        close(fileDesc[INPUT]);
        result = write(fileDesc[OUTPUT], str, strlen(str));
        if (result == -1)
        {
            perror("Failed to write to pipe");
            exit(1);
        }
        close(fileDesc[OUTPUT]);
        exit(0);
    }
}

int main()
{
    char buf[256]; // buffer to store the message
    int read_count, result;

    result = pipe(fileDesc);
    if (result == -1)
    {
        perror("Pipe Failed\n");
        exit(1);
    }

    forkNew("Child 1 is sending a message!\n");
    forkNew("Child 2 is sending a message!\n");

    close(fileDesc[OUTPUT]);

    for (int i = 0; i < 2; i++)
    {
        read_count = read(fileDesc[INPUT], buf, sizeof(buf));
        if (read_count == -1)
        {
            perror("In parent, failed to read from pipe\n");
            exit(1);
        }
        else if (read_count == 0)
        {
            printf("In parent, 0 byte read from pipe.\n");
        }
        else
        {
            buf[read_count] = '\0';
            printf("%s\n", buf);
        }
    }

    close(fileDesc[INPUT]);
    return (EXIT_SUCCESS);
}
