#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int result;
    printf("\nYou are now in a running program \"%s\". \n", argv[0]);
    printf("My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
    printf("Please input an integer (0-255), which will be returned to my parent process:\n");
    scanf("%d", &result);
    printf("Goodbye.\n\n");
    return (result & 0377);
}
