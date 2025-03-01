#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// 檢查第一個字串是否大於第二個字串
int cmp(const struct dirent **a, const struct dirent **b){
    return strcmp((*a)->d_name, (*b)->d_name) >= 0;
}
void print(const char *name){
    struct dirent **tmp;
    int sz = scandir(name, &tmp, 0, cmp);
    printf("\033[1;31m%s\033[0m:\n ", name);
    for(int i = 0; i < sz; i++){
        // 目錄不為.或..時
        if(strcmp(tmp[i]->d_name, ".") && strcmp(tmp[i]->d_name, "..")){
            // d_type = 8:普通文件 d_type = 4:目錄文件 d_type = 10:符號鏈接
            if(tmp[i]->d_type == 4 || tmp[i]->d_type == 10){
                printf("\033[1;34m%s\033[0m  ", tmp[i]->d_name);
            }
            else{
                printf("%s  ", tmp[i]->d_name);
            }
        }
    }
    printf("\n");
}
struct linkedList{
    char *value;
    struct linkedList *next, *prev;
};
struct linkedList *list;
int size = 1, havePassSize = 0, havePassCapacity = 16;
char **havepass;
// havePassCapacity用來設置havePass的大小
bool check(const char *path){
    char tmppath[PATH_MAX];
    realpath(path, tmppath);
    for(int i = 0; i < havePassSize; i++){
        if(strcmp(tmppath, havepass[i]) == 0) return 0;
    }
    havepass[havePassSize] = calloc(strlen(tmppath) + 1, sizeof(char));
    strncpy(havepass[havePassSize], tmppath, strlen(tmppath));
    ++havePassSize;
    if(havePassCapacity <= havePassSize){
        havePassCapacity <<= 1;
        havepass = realloc(havepass, havePassCapacity * sizeof(char *));
    }
    return 1;
}
int main()
{
    // double linked list
    list = calloc(1, sizeof(struct linkedList));
    list->next = calloc(1, sizeof(struct linkedList));
    list->prev = list->next;
    list->next->value = calloc(2, sizeof(char));
    list->next->next = list;
    list->next->prev = list;
    strncpy(list->next->value, ".", 1);

    char tmppath[PATH_MAX];
    realpath(".", tmppath);
    havepass = calloc(havePassCapacity, sizeof(char *));
    havepass[havePassSize] = calloc(strlen(tmppath)+1, sizeof(char));
    strncpy(havepass[havePassSize], tmppath, strlen(tmppath));
    ++havePassSize;
    if(havePassCapacity <= havePassSize){
        havePassCapacity <<= 1;
        havepass = realloc(havepass, havePassCapacity * sizeof(char *));
    }
    
    while(size){
        print(list->next->value);
        struct dirent **tmp;
        // list->next->value裡的值全部丟到tmp裡照cmp的方式排好, 並計算有幾個
        int sz = scandir(list->next->value, &tmp, 0, cmp);
        for(int i = 0; i < sz; i++){
            if(strcmp(tmp[i]->d_name, ".") && strcmp(tmp[i]->d_name, "..")){
                if(tmp[i]->d_type == 4 || tmp[i]->d_type == 10){
                    // 當前路徑的長度＋額外的長度（\0...)＋目錄項名字的長度
                    char *str = calloc(strlen(list->next->value) + 5 + strlen(tmp[i]->d_name), sizeof(char));
                    strncpy(str, list->next->value, strlen(list->next->value)+1);
                    strncat(str, "/", 2);
                    strncat(str, tmp[i]->d_name, strlen(tmp[i]->d_name)+1);
                    // 還沒印出過的
                    if(check(str)){
                        struct linkedList *newnode = calloc(1, sizeof(struct linkedList));
                        newnode->value = str;
                        struct linkedList *tail = list->prev;
                        tail->next = newnode;
                        newnode->prev = tail;
                        list->prev = newnode;
                        newnode->next = list;
                        size++;
                    }
                }
            }
        }
        // 移到下一階層
        struct linkedList *newnextptr = list->next->next;
        struct linkedList *delete = list->next;
        list->next = newnextptr;
        newnextptr->prev = list;
        size--;
        free(delete->value);
        free(delete);
    }
    for(int i = 0; i < havePassSize; i++) free(havepass[i]);
    free(havepass);
    return 0;
}
