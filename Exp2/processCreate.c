#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
    pid_t pid; // process ID

    printf("Before fork, PID = %d, PPID = %d. ", getpid(), getppid());
    printf("\n");  
    
    pid = fork(); // to create a child process
    if (pid == -1) {
        perror("Failed in calling fork");
        exit(1);
    } else if (pid == 0) {
        /* the child process */
        printf("In child process, PID = %d, PPID = %d. ", getpid(), getppid());
        printf("\n");
        //Situation 1: Program run normally.
        // ----- A -----
	//Situation 2: Parent will wait until subProcess ended.
	sleep(3);
        // ----- B -----
        //exit(0);
	//Situation 3: Program run normally.
    } else {
        /* the parent process */
        printf("In parent process, child's PID = %d. ", pid);
        printf("\n");
        
        printf("In parent process, PID = %d, PPID = %d. ", getpid(), getppid());
        printf("\n");
        
        // ----- C -----
        //wait(NULL);
	//Situation 4: SubProcess's Parent PID equals to 1 because of the early end of its parent process.
    }

    printf("Before return in main(), PID = %d, PPID = %d. ", getpid(), getppid());
    printf("\n");
    
    return (EXIT_SUCCESS);
}

