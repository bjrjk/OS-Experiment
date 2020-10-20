#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    int result;

    result = fork();
    if (result < 0) {
        perror("Failed to create child");
        exit(1);
    } else if (result == 0) {
        // Child 1
        char *cmd = "./dummy";

        printf("Child process's PID is %d. My parent's PID is %d.\n", getpid(), getppid());
        printf("Child process is about to execute \"%s\"\n\n", cmd);

        result = execlp(cmd, cmd, NULL);
        if (result == -1) {
            perror("In child process, failed to exec a program");
        }

        exit(0);
    } else {
        // parent
        int status;

        printf("Parent process's PID is %d.\n", getpid());
        printf("Parent process is waiting ... \n");
        wait(&status);
        printf("In parent process, status = 0x%x, WEXITSTATUS(status) = %d (i.e. 0x%x)\n", \
                status, WEXITSTATUS(status), WEXITSTATUS(status));
    }

    return (EXIT_SUCCESS);
}
/*

/usr/include/x86_64-linux-gnu/sys/wait.h
/usr/include/x86_64-linux-gnu/bits/waitstatus.h
/* If WIFEXITED(STATUS), the low-order 8 bits of the status.  */
//#define __WEXITSTATUS(status)   (((status) & 0xff00) >> 8)
/* Macros for constructing status values.  */
//#define __W_EXITCODE(ret, sig)  ((ret) << 8 | (sig))
//#define __W_STOPCODE(sig)       ((sig) << 8 | 0x7f)
//#define __W_CONTINUED           0xffff
//#define __WCOREFLAG             0x80

