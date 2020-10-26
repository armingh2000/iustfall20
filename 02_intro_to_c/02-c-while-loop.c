#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	unsigned char condition = 1;
	char cmd[32];
	while (condition) {
		fgets(cmd, 32, stdin);
		if (strcmp(cmd, "quit\n") == 0) {
			condition = 0;
		}
		/* execute the command and perform
		 * related operations.
		 * ...
		 * */
		printf("cmd: %s\n", cmd);
	}
	return 0;
}

