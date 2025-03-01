#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

char *mystrchr(const char *s, int c){
    if(s == NULL) return NULL;
    char *str = (char *)s;
    while(*str != '\0' && *str != (char)c) ++str;
    return (*str==c) ? str : NULL;
}

char *mystrrchr(const char *s, int c){
    if(s == NULL) return NULL;
    char* last_occurrences = NULL;
    while(*s != '\0'){
        if(*s == (char)c) last_occurrences = s;
        s++;
    }
    return (char *)last_occurrences;
}

size_t mystrspn(const char *s, const char *accept){
    if(s == NULL || accept == NULL) return 0;
    const char *str, *a;
    size_t cnt = 0;
    for(str = s; *str != '\0'; ++str){
        for(a = accept; *a != '\0'; ++a){
            if(*str == *a) break;
        }
        if(*a == '\0') return cnt;
        ++cnt;
    }
    return cnt;
}

size_t mystrcspn(const char *s, const char *reject){
    if(s == NULL || reject == NULL) return 0;
    const char *str, *r;
    size_t cnt = 0;
    for(str = s; *str != '\0'; ++str){
        for(r = reject; *r != '\0'; ++r){
            if(*str == *r) return cnt;
        }
        ++cnt;
    }
    return cnt;
}

char *mystrpbrk(const char *s, const char *accept){
    if(s == NULL || accept == NULL) return NULL;
    const char *sc1, *sc2;
    for(sc1 = s; *sc1 != '\0'; ++sc1){
        for(sc2 = accept; *sc2 != '\0'; ++sc2){
            if(*sc1 == *sc2) return (char *) sc1;
        }
    }
    return NULL;
}

char *mystrstr(const char *haystack , const char *needle){
    if(haystack == NULL || needle == NULL) return NULL;
    if(*needle == '\0') return (char*) haystack;
    while(*haystack){
        const char *h = haystack;
        const char *n = needle;
        while(*h == *n && *n != '\0'){
            h++; n++;
        }
        if(*n == '\0') return (char*) haystack;
        haystack++;
    }
}
char *mystrtok(char *str, const char *delim){
    static char *token = NULL; // 用static以保持token的值
    if(str) token = str;
    else if(!token) return NULL; // 全找完了
    char *s = token; // 紀錄token的起始位置
    while(*token != '\0'){
        bool check = 0;
        // 找到分隔符
        if(delim != NULL){
            for(int i = 0; delim[i] != '\0'; i++){
                if(*token == delim[i]){
                    check = 1;
                    break;
                }
            }
        }  
        if(check){
            *token = '\0';
            token++;
            while(*token != '\0'){
                bool next = 0;
                // 跳過所有分隔符
                if(delim != NULL){ // NULL為指到最後了
                    for(int i = 0; delim[i] != '\0'; i++){
                        if(*token == delim[i]){
                            next = 1;
                            break;
                        }
                    }
                }   
                if(next) token++;
                else break;
            }
            return s;
        }
        token++;
    }
    if(s == token) return NULL;
    return s;
}
