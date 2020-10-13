#include <stdio.h>
#include <stdint.h>

typedef struct my_t {
    char a;
    char b;
    int  c;
    char d;

} my_t;

struct my_packed_t {
    char a;
    char b;
    int  c;
    char d;

} __attribute__((__packed__));
typedef struct my_packed_t my_packed_t;

int main()
{
    my_t x; 
    void *head = &x;

    printf("struct begin address: %p\n"
           "a: %p (expected: %p)\n"
           "b: %p (expected: %p)\n"
           "c: %p (expected: %p)\n"
           "d: %p (expected: %p)\n", head,
           &x.a, head,
           &x.b, head + 1,
           &x.c, head + 2,
           &x.d, head + 6);

    printf("\n\n");

    my_packed_t y; 
    head = &y;

    printf("struct begin address: %p\n"
           "a: %p (expected: %p)\n"
           "b: %p (expected: %p)\n"
           "c: %p (expected: %p)\n"
           "d: %p (expected: %p)\n", head,
           &y.a, head,
           &y.b, head + 1,
           &y.c, head + 2,
           &y.d, head + 6);

    return 0;
}
