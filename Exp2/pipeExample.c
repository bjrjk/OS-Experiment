// https://stackoverflow.com/questions/24766013/is-it-really-necessary-to-close-the-unused-end-of-the-pipe-in-a-process

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define INPUT 0 //Read End of Pipe
#define OUTPUT 1 //Write End of Pipe

int main() {
    int file_descriptors[2];
    pid_t pid; // process ID
    char * msg = "I have a dream. "; // message to send
    char buf[256]; // buffer to store the message
    int read_count;
    int result;

    /* to create a pipe */
result = pipe(file_descriptors);
// file_descriptors[INPUT] for read end of the pipe
// file_descriptors[OUTPUT] for write end of the pipe
    if (result == -1) {
        // fail to create pipe
        perror("Failed in calling pipe");
        exit(1);
}

    pid = fork(); // to create a child process
    if (pid == -1) {
        perror("Failed in calling fork");
        exit(1);
    } else if (pid == 0) {
        /* the child process */
        printf("In the child process...\n");
        // to close the input end of the pipe and
        // write a message to the output end
        close(file_descriptors[INPUT]);

        result = write(file_descriptors[OUTPUT], msg, strlen(msg));
        if (result == -1) {
            perror("In Child1, failed to write to pipe");
            exit(1);
        }

        close(file_descriptors[OUTPUT]);
        exit(0);
    } else {
        /* the parent process */
        printf("In the parent process...\n");
        // to close the output end and
        // read from the input end
        close(file_descriptors[OUTPUT]);

        read_count = read(file_descriptors[INPUT], buf, sizeof (buf));
        if (read_count == -1) {
            perror("In parent, failed to read from");
            exit(1);
        } else if (read_count == 0) {
            printf("In parent, 0 byte read from pipe.\n");
        } else {
            // read_count > 0
            buf[read_count] = '\0'; // set the end of string
            printf("%d bytes of data received from spawned process: %s\n",
                    read_count, buf);
        }

        close(file_descriptors[INPUT]);
    }
    return (EXIT_SUCCESS);
}

