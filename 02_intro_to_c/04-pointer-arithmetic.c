#include <stdio.h>
#include <stdint.h>

int main() {
   int64_t val = 10;
   int64_t *p64 = &val;
   printf("p64, p64+1: %x, %x\n", p64, p64+1);

   int8_t *p8 = (int8_t *)(&val);
   printf("p8, p8+1: %x, %x\n", p8, p8+1);

   return 0;
}
