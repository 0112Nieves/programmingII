#include "mychain.h"

sPoly *differential(const sPoly *original){
    if(original == NULL) return NULL;
    sPoly *after = (sPoly *)malloc(sizeof(sPoly));
    after->pPowers = (uint32_t *)malloc(sizeof(uint32_t) * original->size);
    after->pCoefficients= (uint32_t *)malloc(sizeof(uint32_t) * original->size);
    after->size = original->size;
    for(int32_t i = 0; i < original->size; i++){
        if(original->pPowers[i] == 0) after->pCoefficients[i] = 0;
        else after->pCoefficients[i] = original->pCoefficients[i] * original->pPowers[i];
        after->pPowers[i] = original->pPowers[i] - 1;
        if(original->pPowers[i] == 0){
            after->pCoefficients = (uint32_t *)realloc(after->pCoefficients, sizeof(uint32_t) * (after->size-1));
            after->pPowers = (uint32_t *)realloc(after->pPowers, sizeof(uint32_t) * (after->size-1));
            after->size--;
        }
    }
    return after;
}

void swap(void *a, void *b, size_t size){
    void *temp = malloc(size);
    if(temp == NULL) return;
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

void delZero(sPoly *f){
    int notZero = 0;
    for(int i = 0; i < f->size; i++){
        if(f->pCoefficients[i] != 0){
            notZero++;
        }
    }
    sPoly temp;
    temp.pCoefficients = (int32_t *)malloc(notZero*sizeof(int32_t));
    temp.pPowers = (uint32_t *)malloc(notZero*sizeof(uint32_t));
    int index = 0;
    for(int i = 0; i < f->size; i++){
        if(f->pCoefficients[i] != 0){
            temp.pCoefficients[index] = f->pCoefficients[i];
            temp.pPowers[index] = f->pPowers[i];
            index++;
        }
    }
    temp.size = notZero;
    *f = temp;
}

void sortPoly(sPoly *f){
    int n = f->size;
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            if(f->pPowers[i] < f->pPowers[j]){
                swap(&f->pCoefficients[i], &f->pCoefficients[j], sizeof(int32_t));
                swap(&f->pPowers[i], &f->pPowers[j], sizeof(uint32_t));
            }
            else if(f->pPowers[i] == f->pPowers[j]){
                f->pCoefficients[i] += f->pCoefficients[j];
                f->pPowers[j] = 0;
                f->pCoefficients[j] = 0;
            }
        }
    }
    delZero(f);
    return;
}

sPoly mulPoly(sPoly *f1, sPoly *f2){
    if(f1 == NULL || f2 == NULL){
        printf("Pointer is NULL\n");
        sPoly emptyPoly = {0, NULL, NULL};
        return emptyPoly;
    }
    sPoly temp;
    int sz = f1->size * f2->size;
    temp.pCoefficients = (int32_t *)malloc(sz*sizeof(int32_t));
    temp.pPowers = (uint32_t *)malloc(sz*sizeof(uint32_t));
    if (temp.pCoefficients == NULL || temp.pPowers == NULL) {
        printf("Memory allocation failed.\n");
        free(temp.pCoefficients);
        free(temp.pPowers);
        sPoly emptyPoly = {0, NULL, NULL};
        return emptyPoly;
    }
    temp.size = sz;
    for(int i = 0; i < f1->size; i++){
        for(int j = 0; j < f2->size; j++){
            temp.pCoefficients[i*f2->size+j] = f1->pCoefficients[i]*f2->pCoefficients[j];
            temp.pPowers[i*f2->size+j] = f1->pPowers[i]+f2->pPowers[j];
        }
    }
    sortPoly(&temp);
    return temp;
}

sPoly divide(sPoly y, int power) {
    if(power <= 1) return y; // 終止條件：當 power 小於等於 1 時，直接返回 y
    sPoly val = divide(y, power >> 1);
    sPoly result = mulPoly(&val, &val); // 先對 val 自乘一次
    if(power & 1){
        result = mulPoly(&result, &y); // 再將 result 與 y 相乘一次
    }
    return result;
}

int32_t chain_rule( sPoly *pResult , const sPoly *pFy, const sPoly *pFx ){
    if (pResult == NULL || pFy == NULL || pFx == NULL) return -1;
    pResult->pCoefficients = (int32_t *)malloc(1024*sizeof(int32_t));
    pResult->pPowers = (uint32_t *)malloc(1024*sizeof(uint32_t));
    pResult->size = 1024;
    sPoly *newPx = differential(pFx);
    sPoly *newPy = differential(pFy);
    int cnt = 0;
    for(int i = 0; i < newPy->size; i++){
        sPoly dY;
        sPoly tmp;
        if(newPy->pPowers[i] >= 1){
            dY = divide(*pFx, newPy->pPowers[i]);
            tmp = mulPoly(&dY, newPx);
        }
        else tmp = *newPx;
        for(int j = 0; j < tmp.size; j++){
            pResult->pCoefficients[cnt] = tmp.pCoefficients[j] * newPy->pCoefficients[i];
            pResult->pPowers[cnt] = tmp.pPowers[j];
            cnt++;
        }
    }
    sortPoly(pResult);
    return 0;
}