#include "ContributionCalculate.h"
static const char *month[13] = {"",    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

typedef struct data{
    char *name, *gmail, *date;
    int changed, insertion, deletion, commit;
}Data;

void printDate(char *str){
    int cnt = 0, year = 0;
    char *time = calloc(11, sizeof(char));
    char *day = calloc(5, sizeof(char));
    char *m = calloc(5, sizeof(char));
    if(str == NULL){
        printf("error string\n");
        return;
    }
    char *token;
    token = strtok(str, " ");
    while(token != NULL){
        if(cnt == 1){
            for(int i = 1; i < 13; i++)
                if(strcmp(token, month[i]) == 0){
                    char tmp = i + '0';
                    if(i < 10) snprintf(m, 4, "0%d", i);
                    else snprintf(m, 4, "%d", i);
                }
        }
        else if(cnt == 2){
            if(atoi(token) < 10){
                snprintf(day, 4, "0%s", token);
            }
            else strncpy(day, token, 3);
        }
        else if(cnt == 3) strncpy(time, token, 10);
        else if(cnt == 4) year = atoi(token);
        token = strtok(NULL, " ");
        cnt++;
    }
    printf("%d/%s/%s/%s\n", year, m, day, time);
    free(day); free(m); free(time);
    return;
}

void searchCommitInformationByHashVal(const char *hashVal){
    FILE *file = fopen("frontend.txt", "r"); 
    FILE *output = fopen("contribution.txt", "w");
    if(output == NULL){
        printf("error open file.\n");
        return;
    }
    Data a = {.changed = 0, .insertion = 0, .deletion = 0};
    int cnt = 0;
    if(file == NULL){
        printf("Couldn't open file.\n");
        return;
    }
    char str[105];
    while(fgets(str, 105, file) != NULL){
        if(strstr(str, "commit")){
            if(strncmp(str+7, hashVal, 8) == 0){
                cnt++;
                fgets(str, 105, file);
                char *start = strstr(str, "Author: ");
                start += strlen("Author: ");
                char *end = strchr(str+8, '<');
                a.name = calloc(25, sizeof(char));
                strncpy(a.name, start, end-start);
                a.name[end-start] = '\0';
                start = strchr(str, '<') + 1;
                end = strchr(str, '>');
                a.gmail = calloc(100, sizeof(char));
                strncpy(a.gmail, start, end-start);
                a.gmail[end-start] = '\0';
                fgets(str, 105, file);
                a.date = calloc(50, sizeof(char));
                strncpy(a.date, str+8, 50);
                a.date[strlen(a.date)] = '\0';
                bool flag = true;
                while(flag){
                    if(strstr(str, "changed") != NULL){
                        flag = false;
                        break;
                    }
                    else if(strstr(str, "insertion") != NULL){
                        flag = false;
                        break;
                    }
                    else if(strstr(str, "deletion") != NULL){
                        flag = false;
                        break;
                    }
                    fgets(str, 105, file);
                }
                char *token = strtok(str, ",");
                while(token != NULL){
                    if(strstr(token, "changed") != NULL) sscanf(token, "%d", &a.changed);
                    else if(strstr(token, "(+)") != NULL) sscanf(token, "%d", &a.insertion);
                    else if(strstr(token, "(-)") != NULL) sscanf(token, "%d", &a.deletion);
                    token = strtok(NULL, ",");
                }
            }
        }
    }
    if(cnt == 0) printf("Not found\n");
    else if(cnt > 1) printf("More than two search results\n");
    else{
        fprintf(output, "- %s\n", a.name);
        fprintf(output, "\t- %s\n", a.gmail);
        fprintf(output, "\t- "); printDate(a.date);
        fprintf(output, "\t- %d file changed\n", a.changed);
        fprintf(output, "\t- %d insertions\n", a.insertion);
        fprintf(output, "\t- %d deletions\n", a.deletion);
    }
    free(a.name); free(a.gmail); free(a.date);
    fclose(file); fclose(output);
    return;
}

int compareData(const void *a, const void *b){
    const Data *dataA = (const Data *)a;
    const Data *dataB = (const Data *)b;
    if(dataA->commit != dataB->commit) return dataB->commit - dataA->commit;
    return dataA->name[0] - dataB->name[0];
}

void searchMonthlyContribution(const char *monthAbbrev){
    Data *list = calloc(1, sizeof(Data));
    FILE *output = fopen("contribution.txt", "w");
    if(output == NULL){
        printf("error open file.\n");
        return;
    }
    int cnt = -1, now = 0;
    char str[105];
    FILE *file = fopen("frontend.txt", "r"); 
    if(file == NULL){
        printf("error open file.\n");
        return;
    }
    bool first = true;
    list[0].name = "";
    while(fgets(str, 105, file) != NULL){
        bool flag = true;
        char *name = calloc(25, sizeof(char));
        if(strstr(str, "Author:") != NULL){ 
            char *start = strstr(str, "Author: ");
            start += strlen("Author: ");
            char *end = strchr(str+8, '<');
            strncpy(name, start, end - start);
            if (cnt == -1) {
                cnt++;
                list = realloc(list, (cnt + 1) * sizeof(Data));
                list[cnt].name = calloc(25, sizeof(char));
                strcpy(list[cnt].name, name);
                now = cnt;
                continue;
            }
            for (int i = 0; i < cnt; i++) {
                if (strcmp(name, list[i].name) == 0) {
                    now = i;
                    flag = false;
                    break;
                }
            }
            if(flag){
                Data a = {.changed = 0, .insertion = 0, .deletion = 0, .commit = 0};
                a.name = calloc(25, sizeof(char));
                strcpy(a.name, name);
                cnt++;
                list = realloc(list, (cnt+1) * sizeof(Data));
                list[cnt] = a;
                now = cnt;
            }
        }
        else if(strstr(str, "Date:") != NULL){
            if(strstr(str, monthAbbrev) != NULL){
                while(fgets(str, 105, file) != NULL){
                    if(strstr(str, "changed") != NULL || strstr(str, "(+)") != NULL || strstr(str, "(-)") != NULL){
                        list[now].commit++;
                        char *token = strtok(str, ",");
                        int change = 0, insert = 0, del = 0;
                        while(token != NULL){
                            if(strstr(token, "changed") != NULL){
                                sscanf(token, "%d", &change);
                                list[now].changed += change;
                            }
                            else if(strstr(token, "(+)") != NULL){
                                sscanf(token, "%d", &insert);
                                list[now].insertion += insert;
                            }
                            else if(strstr(token, "(-)") != NULL){
                                sscanf(token, "%d", &del);
                                list[now].deletion += del;
                            }
                            token = strtok(NULL, ",");
                        }
                    }
                }
            }
        }

    }
    qsort(list, cnt, sizeof(Data), compareData);
    for(int i = 0; i < cnt; i++){
        for(int j = i+1; j < cnt; j++){
            if(list[j].name == NULL) continue;
            if(list[i].name == NULL) continue;
            if(strcmp(list[i].name, list[j].name) == 0){
                free(list[j].name);
                list[j].name = NULL;
            }
        }
    }
    for(int i = 0; i < cnt; i++){
        if(list[i].name == NULL) continue;
        fprintf(output, "- %s\n", list[i].name);
        fprintf(output, "\t- %d commits\n", list[i].commit);
        fprintf(output, "\t- %d file changed\n", list[i].changed);
        fprintf(output, "\t- %d insertions\n", list[i].insertion);
        fprintf(output, "\t- %d deletions\n", list[i].deletion);
        free(list[i].name);
    }
    free(list);
    fclose(file); fclose(output);
    return;
}

int main()
{
    searchMonthlyContribution("Sep");
    return 0;
}