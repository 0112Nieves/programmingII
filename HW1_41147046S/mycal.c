#include "mycal.h"
int32_t to_base10(char *str, int base){
    int32_t ans = 0;
    for(str; *str != '\0'; str++){
        int coef;
        if(!isdigit(*str)){
            switch(*str){
                case 'A':
                    coef = 10;
                    break;
                case 'B':
                    coef = 11;
                    break;
                case 'C':
                    coef = 12;
                    break;
                case 'D':
                    coef = 13;
                    break;
                case 'E':
                    coef = 14;
                    break;
                case 'F':
                    coef = 15;
                    break;
            }
        }
        else coef = *str - '0';
        ans += coef * pow(base, strlen(str)-1);
    }
    return ans;
}

void reverseString(char *str){
    if(str == NULL) return;
    char *start = str;
    char *end = str + strlen(str) - 1;
    while(start < end){
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void prependNegativeSign(char *str){
    if(str == NULL) return;
    long long int len = strlen(str);
    memmove(str + 1, str, len +1);
    str[0] = '-';
}

char *to_baseN(long long int ans, int base, bool minus){
    char *result = (char *)malloc(1024 * sizeof(char));
    if(result == NULL) return NULL;
    int32_t cnt = 0;
    while(ans > 0){
        int r;
        r = ans % base;
        if(r >= 10){
            switch(r){
                case 10:
                    result[cnt] = 'A';
                    break;
                case 11:
                    result[cnt] = 'B';
                    break;
                case 12:
                    result[cnt] = 'C';
                    break;
                case 13:
                    result[cnt] = 'D';
                    break;
                case 14:
                    result[cnt] = 'E';
                    break;
                case 15:
                    result[cnt] = 'F';
                    break;
            }
        }
        else result[cnt] = r + '0';
        ans /= base;
        cnt++;
    }
    result[cnt] = '\0';
    reverseString(result);
    char bas[5];
    sprintf(bas, "%d", base);
    strcat(result, "_");
    strcat(result, bas);
    if(minus) prependNegativeSign(result);
    return result;
}

int32_t calculate( char *pExpr , int32_t base, char **ppResult ){
    bool flag = false;
    if(pExpr == NULL) return -1;
    if(pExpr[0] == '+') return -1;
    char *t[105];
    int32_t cnt = 0;
    char *token = strtok(pExpr, " ");
    while(token != NULL){
        t[cnt] = strdup(token);
        token = strtok(NULL, " ");
        cnt++;
    }
    if(cnt % 2 == 0) return -1;
    int32_t num[105] = {0};
    for(int32_t i = 0; i < cnt; i++){
        if(strchr(t[i], '*') == NULL && strchr(t[i], '-') == NULL && strchr(t[i], '+') == NULL){
            char s[15];
            int32_t n;
            sscanf(t[i], "%[^_]_%d", s, &n);
            num[i] = to_base10(s, n);
        }
    }
    for(int32_t i = 0; i < cnt; i++){
        if(strchr(t[i], '*') != NULL){
            num[i] = num[i-1] * num[i+1];
            num[i-1] = 0;
            num[i+1] = 0;
        }
        else if(strchr(t[i], '-') != NULL){
            num[i] = -1 * num[i+1];
            num[i+1] = 0;
        }
    }
    long long int ans = 0;
    for(int32_t i = 0; i < cnt; i++){
        ans += (num[i]);
    }
    if(ans < 0){
        flag = true;
        ans = abs(ans);
    }
    *ppResult = to_baseN(ans, base, flag);
    for(int32_t i = 0; i < cnt; i++) free(t[i]);
    return 0;
}