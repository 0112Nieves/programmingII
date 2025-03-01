#include "abacus.h"
#include <math.h>
// why not initalize in struct
sAbacus * abacus_init( void ){
    sAbacus *pA = (sAbacus*)(malloc(sizeof(sAbacus)));
    if(pA == NULL) return NULL;
    pA->num = 0;
    pA->pUpperRod = NULL;
    pA->pLowerRod = NULL;
    return pA;
}
void abacus_free( sAbacus *pA ){
    if( pA->pUpperRod != NULL ) free( pA->pUpperRod );
    if( pA->pLowerRod != NULL ) free( pA->pLowerRod );
    free( pA );
}
int32_t abacus_set( sAbacus *pA, char *pStr ){
    if(pStr == NULL){
        pA = abacus_init();
        return -1;
    }
    size_t num = strlen(pStr);
    if(num == 0) return -1;
    if(num > 255 ) return -1;
    for( size_t i = 0; i < num; i++ ){
        if(!isdigit(pStr[i])) return -1;
    }
    pA->num = num;
    pA->pUpperRod = (int*)calloc(num, sizeof(int));
    pA->pLowerRod = (int*)calloc(num, sizeof(int));
    if(pA->pUpperRod == NULL || pA->pLowerRod == NULL) return -1;
    for( size_t i = 0; i < num; i++ ){
        int n = pStr[i] - '0';
        if(n >= 5){
            pA->pUpperRod[i] = 1;
            n-=5;
        }
        pA->pLowerRod[i] = n;
    }
    return 0;
}
int32_t abacus_add( sAbacus *pA, sAbacus b, sAbacus c ){
    //sAbacus *pA = abacus_init();
    if (pA == NULL) return -1; 
    pA->num = b.num>c.num ? b.num:c.num;
    int loc = 0;
    bool carry = false;
    // 對齊位數
    if(b.num>c.num){
        loc = b.num - c.num;
        memmove(c.pUpperRod+loc, c.pUpperRod, c.num*sizeof(int));
        memmove(c.pLowerRod+loc, c.pLowerRod, c.num*sizeof(int)); 
        for(int i = 0; i < loc; i++){
            c.pUpperRod[i] = 0;
            c.pLowerRod[i] = 0;
        }       
    }
    else if(c.num > b.num){
        loc = c.num - b.num;
        memmove(b.pUpperRod+loc, b.pUpperRod, b.num*sizeof(int));
        memmove(b.pLowerRod+loc, b.pLowerRod, b.num*sizeof(int)); 
        for(int i = 0; i < loc; i++){
            b.pUpperRod[i] = 0;
            b.pLowerRod[i] = 0;
        }       
    }
    pA->pUpperRod = (int*)realloc( pA->pUpperRod, (pA->num+1) * sizeof(int));
    pA->pLowerRod = (int*)realloc( pA->pLowerRod, (pA->num+1) * sizeof(int));
    if(pA->pUpperRod == NULL || pA->pLowerRod == NULL) return -1;
    for(int i = pA->num - 1; i >= 0; i--){
        int sum = b.pUpperRod[i] * 5 + b.pLowerRod[i] + c.pUpperRod[i] * 5 + c.pLowerRod[i];
        if(carry){
            sum++;
            carry = false;
        }
        if(sum > 9){
            sum -= 10;
            carry = true;
        }
        pA->pUpperRod[i] = sum / 5;
        pA->pLowerRod[i] = sum % 5;
    }
    if(pA->num > 255) return -1;
    return 0;
}
int32_t abacus_del( sAbacus *pA, sAbacus b, sAbacus c ){
    //sAbacus *pA = abacus_init();
    if (pA == NULL) return -1; 
    pA->num = b.num > c.num ? b.num : c.num;
    int loc = 0;
    bool carry = false;
    // 對齊位數
    if(b.num>c.num){
        loc = b.num - c.num;
        memmove(c.pUpperRod+loc, c.pUpperRod, c.num*sizeof(int));
        memmove(c.pLowerRod+loc, c.pLowerRod, c.num*sizeof(int)); 
        for(int i = 0; i < loc; i++){
            c.pUpperRod[i] = 0;
            c.pLowerRod[i] = 0;
        }       
    }
    else if(c.num > b.num){
        loc = c.num - b.num;
        memmove(b.pUpperRod+loc, b.pUpperRod, b.num*sizeof(int));
        memmove(b.pLowerRod+loc, b.pLowerRod, b.num*sizeof(int)); 
        for(int i = 0; i < loc; i++){
            b.pUpperRod[i] = 0;
            b.pLowerRod[i] = 0;
        }       
    }
    pA->pUpperRod = (int*)realloc( pA->pUpperRod, (pA->num+1) * sizeof(int));
    pA->pLowerRod = (int*)realloc( pA->pLowerRod, (pA->num+1) * sizeof(int));
    if(pA->pUpperRod == NULL || pA->pLowerRod == NULL) return -1;
    for(int i = pA->num - 1; i >= 0; i--){
        int del = b.pUpperRod[i] * 5 + b.pLowerRod[i] - (c.pUpperRod[i] * 5 + c.pLowerRod[i]);
        if(carry){
            del--;
            carry = false;
        }
        if(del < 0){
            del += 10;
            carry = true;
        }
        pA->pUpperRod[i] = del / 5;
        pA->pLowerRod[i] = del % 5;
    }
    return 0;
}
int32_t abacus_print( sAbacus a ){
    bool flag[256];
    memset(flag, 0, sizeof(flag));
    for(int i = 0; i < a.num ; i++){
        if(a.pUpperRod[i] == 0) printf("\033[31m*\033[0m");
        else printf(" ");
    }
    printf("\n");
    for(int i = 0; i < a.num ; i++){
        if(a.pUpperRod[i] == 1) printf("\033[31m*\033[0m");
        else printf(" ");
    }
    printf("\n");
    for(int i = 0; i < a.num; i++){
        printf("-");
    }
    printf("\n");
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < a.num; j++){
            if(flag[j]){
                printf("\033[33m*\033[0m");
                continue;
            }
            if(a.pLowerRod[j] == 0){
                printf(" ");
                flag[j] = 1;
            }
            if(a.pLowerRod[j]){
                printf("\033[33m*\033[0m");
                a.pLowerRod[j] -= 1;
            }
        }
        printf("\n");
    }
    return 0;
}
