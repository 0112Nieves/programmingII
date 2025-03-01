#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
bool valid[5000];
char *dic[5000];
int cnt;
const float alpha[26] = {8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 
                        7.0, 0.15, 0.77, 4.0, 2.4, 6.7, 7.5, 1.9, 
                        0.095, 6.0, 6.3, 9.1, 2.8, 0.98, 2.4, 0.15,
                        2.0, 0.074};

char *getStr(char *str){
    char *pos = strchr(str, '/');
    if(pos != NULL){
        int len = pos - str;
        char *tmp = (char *)malloc(len+1);
        strncpy(tmp, str, len);
        tmp[len] = '\0';
        return tmp;
    }
    else{
        str[strlen(str)-1] = '\0';
        return strdup(str);
    }
}

float frequency(const char *str){
    float q = 0;
    for(int i = 0; i < strlen(str); i++){
        q += alpha[str[i]-'A'];
    }
    return q;
}

char *findAns(char **dic, const char *feedback, const char *guess, bool *valid){
    char *ans = malloc(5 * sizeof(char));
    for(int i = 0; i < cnt; i++){
        bool vis[5] = {0};
        for(int j = 0; j < 5; j++){
            if(valid[i] == 0) break;
            else if(feedback[j] == 'Y'){
                if(dic[i][j] == guess[j]) valid[i] = 0;
                else if(strchr(dic[i], guess[j]) == NULL) valid[i] = 0;
            }
            else if(feedback[j] == 'G'){
                vis[j] = 1;
                if(dic[i][j] != guess[j]) valid[i] = 0;
            }
            else if(feedback[j] == 'B'){
                char *ptr = dic[i];
                while(*ptr != '\0'){
                    char *pos = strchr(ptr, guess[j]);
                    if(pos != NULL && !vis[pos-dic[i]]) valid[i] = 0;
                    ptr++;
                }
            }
        }
    }
    float f = -1;
    for(int i = 0; i < cnt; i++){
        if(valid[i] == 0) continue;
        if(frequency(dic[i]) > f){
            strcpy(ans, dic[i]);
            f = frequency(dic[i]);
        }
    }
    return ans;
}

void changeUpper(char *str){
    while(*str){
        *str  = *str - 'a' + 'A';
        str++;
    }
    return;
}

int main() {
    bool correct = false;
    FILE *fp = fopen("./en_US.dic", "r");
    char str[128], word[128], feedback[5];
    if(fp == NULL){
        printf("Error open file\n");
        return 0;
    }
    cnt = 0;
    memset(valid, 1, sizeof(valid));
    while(fgets(str, 128, fp) != NULL){
        char *tmp = getStr(str);
        changeUpper(tmp);
        if(strlen(tmp) == 5 && isalpha(tmp[0])){
            dic[cnt] = (char *)malloc(5 * sizeof(char));
            strcpy(dic[cnt], tmp);
            cnt++;
        }
        free(tmp);
    }
    char guess[1025] = "";
    while(!correct){
        bool j = true;
        printf("Advice:   ");
        strcpy(guess, findAns(dic, feedback, guess, valid));
        if(strcmp(guess, "") == 0){
            printf("No Advice\n"); 
            return 0;
        }
        printf("%s\n", guess);
        printf("FeedBack: ");
        scanf("%s", feedback);
        for(int i = 0; i < 5; i++){
            if(!isupper(feedback[i])){
                printf("Wrong Input\n");
                j = false;
                break;
            }
            if(feedback[i] != 'Y' && feedback[i] != 'G' && feedback[i] != 'B'){
                printf("Wrong Input\n");
                j = false;
                break;
            }
        }
        while(!j){
            j = true;
            printf("FeedBack: ");
            scanf("%s", feedback);
            for(int i = 0; i < 5; i++){
                if(!isupper(feedback[i])){
                    printf("Wrong Input\n");
                    j = false;
                    continue;
                }
                if(feedback[i] != 'Y' && feedback[i] != 'G' && feedback[i] != 'B'){
                    printf("Wrong Input\n");
                    j = false;
                    continue;
                }
            }
        }
        while(strlen(feedback) > 5){
            printf("Wrong Input\n");
            printf("FeedBack: ");
            scanf("%s", feedback);
        }
        if(feedback[0] == 'G' && feedback[1] == 'G' && feedback[2] == 'G' && feedback[3] == 'G' && feedback[4] == 'G') correct = 1;
    }
    printf("Congratulations!!\n");
    fclose(fp);
    return 0;
}