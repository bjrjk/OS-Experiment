#include <unistd.h>
#include <stdio.h>
int main(){
	alarm(1);
	int x=0;
	while(1){
		printf("%d ",x++);
	}
	return 0;
}
