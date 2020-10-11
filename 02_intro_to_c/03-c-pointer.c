#include <stdio.h>

int main() {
	int value = 10;
	int *p;
	p = &value;

	printf("value is: %d, "
	       "(address: %x)\n",
	       *p, p);
	return 0;
}
