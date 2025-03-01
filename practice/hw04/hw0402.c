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
    
    int a1 = 0, b = 10, c = 20;
    RANGE(a1, b, c){
        printf("%d ", a1);
    } printf("\n");

    a1 = 0, b = 20, c = 10;
    RANGE(a1, b, c){
        printf("%d ", a1);
    } printf("\n");

    int arr[10] = {1,2,3,4,5,6,7,8,9,10};
    void *itemINT=NULL;
    FOREACH(itemINT, arr){
        printf("%d", *((int*)itemINT));
    } printf("\n");


    char str[10] = {'h', 'e', 'l', 'l', 'o', 'k', 'i', 't', 't', 'y'};
    void *itemCHAR=NULL;
    FOREACH(itemCHAR, str){
        printf("%c", *((char*)itemCHAR));
    } printf("\n");
    return 0;
}