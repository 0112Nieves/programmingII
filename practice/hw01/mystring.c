#include <stdio.h>
#include <string.h>
#include <stdint.h>

char *mystrchr(const char *s, int c){
    char *str = (char *)s;
    while(*str != '\0' && *str != (char)c) ++str;
    return (*str==c) ? str : NULL;
}

char *mystrrchr(const char *s, int c){
    char* last_occurrences = NULL;
    while(*s != '\0'){
        if(*s == (char)c) last_occurrences = s;
        s++;
    }
    return (char *)last_occurrences;
}

size_t mystrspn(const char *s, const char *accept){
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
// why new varaible
char *mystrpbrk(const char *s, const char *accept){
    const char *sc1, *sc2;
    for(sc1 = s; *sc1 != '\0'; ++sc1){
        for(sc2 = accept; *sc2 != '\0'; ++sc2){
            if(*sc1 == *sc2) return (char *) sc1;
        }
    }
    return NULL;
}
// vs strpbrk
char *mystrstr(const char *haystack , const char *needle){
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
    if(delim == NULL) return NULL;
    static char * s_mem = NULL;
    if(str == NULL && s_mem == NULL) return NULL;

    char *s;
    if(str != NULL) s = str;
    else s = s_mem;
    char const *delimiters;

    // 找到token的起始位置
    int stat = 1;
    while(stat){
        delimiters = delim;
        while(*delimiters && *s != *delimiters) delimiters++;
        if(*delimiters) s++;
        else stat = 0;
    }
    s_mem = s;

    // 找結尾
    while(*s){
        delimiters = delim;
        while(*delimiters && *s != *delimiters) delimiters++;
        if(*delimiters){
            *s = '\0';
            char *t = s_mem;
            s_mem = s+1;
            return t;
        }
        s++;
    }

    // 跳出時,說明s中沒有delim
    char *t = s_mem;
    s_mem = NULL;
    return t;
}
