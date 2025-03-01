#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
typedef struct Word{
    char c, color;
}word;
int main()
{
    char ans[128] = "", in[64];
    int guess = 1;
    bool correct = false;
    while(ans[0] == ' ' || strlen(ans) != 6 || isupper(ans[0]) || strstr(ans, "'s") != NULL){
        FILE *fp = popen("shuf -n 1 /usr/share/dict/american-english", "r");
        if(fp == NULL){
            perror("fopen");
            return -1;
        }
        size_t bytes = fread(ans, 1, sizeof(ans)-1, fp);
        ans[bytes] = '\0';
        pclose(fp);
    }
    if(ans[strlen(ans)] == '\n') ans[strlen(ans)] == '\0';
    int pos[6];
    // printf("Answer: %s", ans);
    while(guess <= 6 && !correct){
        memset(pos, 0, sizeof(pos));
        printf("Guess %d: ", guess);
        scanf("%s", in);
        word *list = calloc(5, sizeof(word));
        for(int i = 0; i < 5; i++){
            strncpy(&list[i].c, in+i, 1);
        }
        for(int i = 0; i < 5; i++){
            if(list[i].c == ans[i]){
                list[i].color = 'g';
                pos[i] = 1;
            }
        }
        for(int i = 0; i < 5; i++){
            if(list[i].color == 'g' || list[i].color == 'y') continue;
            for(int j = 0; j < 5; j++){
                if(pos[j]) continue;
                if(list[i].c == ans[j]){
                    list[i].color = 'y';
                    pos[j] = 1;
                }
            }
        }
        bool right = true;
        for(int i = 0; i < 5; i++){
            if(list[i].color == 'g') printf("\033[32m%c\033[0m", list[i].c);
            else if(list[i].color == 'y') printf("\033[33m%c\033[0m", list[i].c);
            else printf("%c", list[i].c);
            if(list[i].color != 'g') right = false;
        }
        printf("\n");
        if(right){
            printf("Congratulations!\n");
            correct = true;
        }
        guess++;
    }
    if(!correct) printf("You lose!! Please try again.\n");
    return 0;
}