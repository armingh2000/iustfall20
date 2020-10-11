/* adding standard input output header file to the
 * source code.
 * */
#include <stdio.h>

int main(int argc, char *argv[])
{
    // defining some variables
    int a;
    int b, c;
    a = 10;
    b = 20;
    c = a + b;

    // writing to stdout
    printf("hello world\n");
    printf("a + b = %d\n", c);

    return 0;
}
