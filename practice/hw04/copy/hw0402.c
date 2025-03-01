#include <stdio.h>
#include <stdint.h>
#include "macros.h"
int main(){
    int32_t x = 1;
    printf("BIT(30) = %d\n", BIT(30));
    x = 0;
    printf("SETBIT(x, 10) = %d\n", SETBIT(x, 10));
    x = 2147483647;
    printf("CLEARBIT(x, 1) = %d\n", CLEARBIT(x, 1));
    x = 2147483647;
    printf("GETBIT(x, 20) = %d\n", GETBIT(x, 20));
    x = 1073741823;
    printf("TOGGLEBIT(x, 30) = %d\n", TOGGLEBIT(x, 30));
    return 0;
}
