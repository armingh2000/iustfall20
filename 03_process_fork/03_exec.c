#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
		char *my_args[3];
		my_args[0] = strdup("./script.sh");
		my_args[1] = strdup("Hello from Bash!");
		my_args[2] = NULL; 		// Indicating end of array
		execvp(my_args[0], my_args);

		// CHILD process will be terminated before this line!
		printf("This should not be printed");


	} else{
		// Parent Process
		int w = wait(NULL);
		printf("I am parent of %d. pid is: %d\n", r, getpid());
	}

	return 0;
}

