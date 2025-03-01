#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
int playercnt;
typedef struct data{
    int fthg, ftag;
}Data;
typedef struct player{
    char *name;
    int w, d, l, gf, ga, gd, pts;
}Player;

Player *readCSV(FILE *f, int *playercnt){
    char str[1024];
    Player *list = NULL;
    int total = 0;
    Data d;
    fgets(str, 1024, f);
    while(fgets(str, 1024, f) != NULL){
        char *token = strtok(str, ",");
        int cnt = 1, p1 = 0, p2 = 0;
        while(token != NULL){
            if(cnt == 2){
                bool flag = true;
                for(int i = 0; i < total; i++){
                    if(strcmp(list[i].name, token) == 0){
                        p1 = i;
                        flag = false;
                        break;
                    }
                }
                if(total == 0 || flag){
                    Player home = {NULL, 0, 0, 0, 0, 0, 0, 0};
                    p1 = total;
                    list = realloc(list, (total+1) * sizeof(Player));
                    home.name = (char *)malloc(64 * sizeof(char));
                    strcpy(home.name, token);
                    list[p1] = home;
                    total++;
                }
            }
            else if(cnt == 3){
                bool flag = true;
                for(int i = 0; i < total; i++){
                    if(strcmp(list[i].name, token) == 0){
                        p2 = i;
                        flag = false;
                        break;
                    }
                }
                if(total == 0 || flag){
                    Player away = {NULL, 0, 0, 0, 0, 0, 0, 0};
                    p2 = total;
                    list = realloc(list, (total+1) * sizeof(Player));
                    away.name = (char *)malloc(64 * sizeof(char));
                    strcpy(away.name, token);
                    list[p2] = away;
                    total++;
                }
            }
            else if(cnt == 4) d.fthg = atoi(token);
            else if(cnt == 5)d.ftag = atoi(token);
            else if(cnt > 7) break;
            cnt++;
            token = strtok(NULL, ",");
        }
        list[p1].gf += d.fthg; list[p1].ga += d.ftag;
        list[p2].gf += d.ftag; list[p2].ga += d.fthg;
        if(d.fthg > d.ftag){
            list[p1].w++;
            list[p2].l++;
        }
        else if(d.fthg == d.ftag){
            list[p1].d++;
            list[p2].d++;
        }
        else if(d.fthg < d.ftag){
            list[p1].l++;
            list[p2].w++;
        }
        *playercnt = total;
    }
    return list;
}

int cmpPts(const void *a, const void *b){
    Player *c = (Player *)a;
    Player *d = (Player *)b;
    return d->pts - c->pts;
}

void space(int len, int n){
    for(int i = 0; i < n-len; i++) printf(" ");
    return;
}

int findLen(int n){
    int len = 0;
    if(n == 0) return 1;
    while(n != 0){
        n /= 10;
        len++;
    }
    return len;
}

int main()
{
    char file[32], str[512];
    printf("Please enter the file name: ");
    if(fgets(file, 32, stdin) == NULL){
        printf("Error\n");
        return 0;
    }
    if(file[strlen(file)-1] == '\n'){
        file[strlen(file)-1] = '\0';
    }
    FILE *f = fopen(file, "r");
    if(f == NULL){
        printf("Error open file.\n");
        return 0;
    }
    playercnt = 0;
    Player *plist = readCSV(f, &playercnt);
    for(int i = 0; i < playercnt; i++){
        plist[i].pts = plist[i].w*3 + plist[i].d;
        plist[i].gd = plist[i].gf - plist[i].ga;
    }
    qsort(plist, playercnt, sizeof(Player), cmpPts);
    printf("    Team              W    D   L   GF  GA  GD  Pts\n");
    for(int i = 0; i < playercnt; i++){
        if(i+1 < 10) printf("0");
        printf("%d) %s", i+1, plist[i].name);
        space(strlen(plist[i].name), 18);
        printf("%d", plist[i].w);
        space(findLen(plist[i].w), 5);
        printf("%d", plist[i].d);
        space(findLen(plist[i].d), 4);
        printf("%d", plist[i].l);
        space(findLen(plist[i].l), 4);
        printf("%d", plist[i].gf);
        space(findLen(plist[i].gf), 4);
        printf("%d", plist[i].ga);
        space(findLen(plist[i].ga), 4);
        if(plist[i].gd > 0) printf("+");
        else if(plist[i].gd == 0) printf(" ");
        printf("%d", plist[i].gd);
        space(findLen(plist[i].gd), 4);
        printf("%d\n", plist[i].pts);
    }
    for(int i = 0; i < playercnt; i++) free(plist[i].name);
    free(plist);
    fclose(f);
    return 0;
}