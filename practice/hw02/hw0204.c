#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <glib.h>
#define print(i) printf("%d\n",i);
int playerCount;
int32_t budget[11] = {0};

static char abilityStr[27][5] = {
    "ls", "st", "rs", "lw", "lf",
    "cf", "rf", "rw", "lam", "cam",
    "ram", "lm", "lcm", "cm", "rcm",
    "rm", "lwb", "ldm", "cdm", "rdm",
    "rwb", "lb", "lcb", "cb", "rcb",
    "rb", "gk"
};

typedef struct player{
    char *name, *club;
    int32_t value_eur, wage_eur;
    int id;
    bool selected;
    GHashTable *ability;
}Player;

int turn_integer(char *str){
    int a = 0, b = 0;
    if(strchr(str, '+') != NULL) sscanf(str, "%d+%d", &a, &b);
    else sscanf(str, "%d", &a);
    return a+b;
}

Player setPlayer(char *str){
    Player a;
    int count = 1;
    a.name = calloc(50, sizeof(char));
    a.club = calloc(50, sizeof(char));
    a.selected = false;
    a.ability = g_hash_table_new(g_str_hash, g_str_equal);
    char *token;
    token = strtok(str, ",");
    a.id = atoi(token);
    while(token != NULL){
        token = strtok(NULL, ",");
        // 處理一位置裡有逗號的部份
        if(token[0] == '\"'){
            char *tmp = calloc(strlen(token)+1, strlen(token));
            if(tmp == NULL) exit(1);
            strncpy(tmp, token+1, strlen(token));
            tmp[strlen(tmp)] = ',';
            token = strtok(NULL, "\"");
            tmp = realloc(tmp, strlen(tmp)+strlen(token)+1);
            strncat(tmp, token, strlen(token));
            tmp[strlen(tmp)] = '\0';
        }
        if(count == 2){
            strncpy(a.name, token, strlen(token));
            a.name[strlen(a.name)] = '\0';
        }
        else if(count == 7) a.value_eur = atoi(token);
        else if(count == 8) a.wage_eur = atoi(token);
        else if(count == 14){
            strncpy(a.club, token, strlen(token));
            a.club[strlen(a.club)] = '\0';
        }
        else if(count >= 78 && count <= 104)
        g_hash_table_insert(a.ability, abilityStr[count-78], GINT_TO_POINTER(turn_integer(token)));
        else if(count > 105) break;
        count++;
    }
    return a;
}
// 判斷有沒有重複的gk
int countSubstring(const char *str, const char *substring){
    int count = 0;
    size_t len = strlen(substring);
    while(*str){
        const char *ptr = strstr(str, substring);
        if(ptr){
            count++;
            str = ptr + len;
        }
        else break;
    }
    return count;
}

Player* readCSV(FILE *file, int *playerCount){
    char str[2048];
    int32_t cnt = 0;
    Player *playerlist = NULL;
    fgets(str, 2048, file);
    while(fgets(str, 2048, file) != NULL){
        if(str[strlen(str)-1] == '\n') str[strlen(str)-1] = '\0';
        // 把連續逗號換掉
        char *find = strstr(str, ",,");
        while(find != NULL){
            char *tmp = calloc(strlen(str)+2, sizeof(char));
            tmp[strlen(str)+1] = '\0';
            strncpy(tmp, str, find-str+1);
            tmp[find-str+1] = ' ';
            strncat(tmp+(find-str)+2, find+1, strlen(find+1));
            strncpy(str, tmp, 2048);
            find = strstr(str, ",,");
        }
        Player player = setPlayer(str);
        playerlist = realloc(playerlist, (cnt+1) * sizeof(Player));
        if(playerlist == NULL){
            for(int i = 0; i < cnt; i++){
                free(playerlist[i].name);
                free(playerlist[i].club);
                g_hash_table_destroy(playerlist[i].ability);
            }
            free(playerlist);
            perror("Error create list");
            return 0;
        }
        playerlist[cnt] = player;
        cnt++;
    }
    *playerCount = cnt-1;
    return playerlist;
}

int pick(Player *list, const char *position, long int money){
    int max = -1, choose = -1;
    for(int i = 0; i < playerCount; i++){
        int ability = GPOINTER_TO_INT(g_hash_table_lookup(list[i].ability, position));
        if(ability >= max && list[i].value_eur <= money && !list[i].selected){
            if(ability == max){
                if(list[i].value_eur < list[choose].value_eur){
                    choose = i;
                    max = ability;
                }
            }
            else{
                choose = i;
                max = ability;
            }
        }
    }
    if(choose == -1) printf("Couldn't find any person under this budget.\n");
    else list[choose].selected = true;
    return choose;
}

void printdata(Player player, const char *position){
    printf("%s/%s/%d/%d/%d\n", 
    player.name, player.club, GPOINTER_TO_INT(g_hash_table_lookup(player.ability, position)),
    player.value_eur, player.wage_eur);
    return;
}

int cmpID(const void *a, const void *b){
    Player *c = (Player *)a;
    Player *d = (Player *)b;
    return c->id - d->id;
}

int main()
{
    int fd = 0;
    char file[105];
    printf("Please enter the dataset: ");
    if(fgets(file, 105, stdin) == NULL){
        printf("Error\n");
        return 0;
    }
    
    if(file[strlen(file)-1] == '\n'){
        file[strlen(file)-1] = '\0';
    }
    FILE *fp = fopen(file, "r");
    if(!fp) exit(EXIT_FAILURE);
    playerCount = 0;
    Player *list = readCSV(fp, &playerCount);
    qsort(list, playerCount, sizeof(Player), cmpID);
    
    bool flag = true;
    char Squad[105];
    while(flag){
        printf("Please enter the squad: ");
        fgets(Squad, 100, stdin);
        if(Squad[strlen(Squad)-1] == '\n') Squad[strlen(Squad)-1] = '\0';
        if(countSubstring(Squad, "gk") != 1){
            printf("The team must has one and the only one gk.Please input again\n");
        }
        else flag = false;
    }
    char **squad = calloc(11, sizeof(char *));
    char *token = strtok(Squad, " ");
    int cnt = 0;
    while(token != NULL){
        squad[cnt] = calloc(strlen(token)+1, sizeof(char));
        strncpy(squad[cnt], token, strlen(token));
        cnt++;
        token = strtok(NULL, " ");
    }
    if(strcmp(squad[10], "gk") != 0) printf("The last one must be gk.\n");

    printf("Budget:\n");
    for(int i = 0; i < 11; i++){
        printf("\t%s: ", squad[i]);
        scanf("%d", &budget[i]);
    }

    printf("Team:\n");
    for(int i = 0; i < 11; i++){
        printf("\t%s: ", squad[i]);
        int best = pick(list, squad[i], budget[i]);
        printdata(list[best], squad[i]);
    } 
    for(int i = 0; i < cnt; i++){
        free(list[i].name);
        free(list[i].club);
        g_hash_table_destroy(list[i].ability);
    }
    free(list);
    fclose(fp);
    return 0;
}