#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void forkNew(char str[]){
	pid_t pid = fork();
	if(pid < 0){
		printf("Fork Failed\n");
		exit(1);
	}else if(pid == 0){
		printf(str);
		exit(0);
	}
}

int main(){
	forkNew("bbbbb\n");
	forkNew("ccccc\n");
	printf("aaaaa\n");
	return 0;
}

