#include "insert.h"
int32_t strinsert( char **ppResult , const char *pStr1 , int32_t location , const char *pStr2 ){
    int len1 = strlen(pStr1);
    int len2 = strlen(pStr2);
    if (location < 0 || location > len1) {
        return -1; // 无效的 location 参数
    }
    int totalLen = len1 + len2 + 1;
    *ppResult = (char *)malloc(totalLen);
    if(*ppResult == NULL) return -1;
    int i = 0, j = 0;
    for(; i < location; i++) (*ppResult)[i] = pStr1[i];
    for(;i < location + len2; i++){
        (*ppResult)[i] = pStr2[j];
        j++;
    }
    for(;i < totalLen - 1; i++) (*ppResult)[i] = pStr1[i-len2];
    (*ppResult)[totalLen-1] = '\0';
    return *ppResult;
}
