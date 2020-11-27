#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



int main (int argc, char *argv[])
{
	printf("Hello World! My pid is: %d\n", getpid());

	int r = fork();

	if (r < 0){
		printf("fork failed!\n");
		exit(1);
	} else if (r == 0){ 
		// Child Process
		printf("Hello World, I am child process. My pid is: %d\n", getpid());

		for (int i = 0; i < 10; i++){
		    printf("number: %d\n", i);
		}

	} else{
		// Parent Process
		int w = wait(NULL);
		printf("I am parent of %d. pid is: %d\n", r, getpid());
	}

	return 0;
}

