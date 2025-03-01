#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct data{
    char ID[15], text[512];
    int chapter, verse;
}Data;

char *turn_lower(char *str){
    char *new = malloc(strlen(str) + 1);
    if(new == NULL) return NULL;
    int i = 0;
    while (*str) {
        new[i++] = tolower(*str++);
    }
    new[i] = '\0';
    return new;
}

int main()
{
    char *search;
    char str[2048];
    int cnt = 0;
    search = calloc(300, sizeof(char));
    FILE *file = fopen("bible.txt", "r");
    if(file == NULL){
        printf("Couldn't open file.\n");
        return 0;
    }
    printf("Please enter the search target:");
    fgets(search, 300, stdin);
    search[strlen(search)-1] = '\0';
    Data *list = NULL;
    char *ID = calloc(10, sizeof(char));
    char *text = calloc(512, sizeof(char));
    while(fgets(str, 2048, file) != NULL){
        int chapter = 0, verse = 0;
        const char *chapter_start = strstr(str, "\"chapter\":");
        if(chapter_start != NULL) sscanf(chapter_start, "\"chapter\":%d,", &chapter);
        const char *verse_start = strstr(str, "\"verse\":");
        if(verse_start != NULL) sscanf(verse_start, "\"verse\":%d", &verse);
        const char *text_start = strstr(str, "\"text\":");
        if(text_start != NULL) sscanf(text_start, "\"text\":\"%[^\"]\"", text);
        const char *id_start = strstr(str, "\"book_id\":");
        if(id_start != NULL) sscanf(id_start, "\"book_id\":\"%[^\"]\"", ID);
        char *ltext = turn_lower(text);
        char *lsearch = turn_lower(search);
        if(strstr(ltext, lsearch) != NULL){
            list = realloc(list, (cnt+1) * sizeof(Data));
            if(list == NULL){
                printf("error realloc list\n");
                return 0;
            }
            list[cnt].chapter = chapter;
            list[cnt].verse = verse;
            strcpy(list[cnt].ID, ID);
            strcpy(list[cnt].text, text);
            cnt++;
        }
    }
    free(ID); free(text);
    printf("Found %d time(s)\n", cnt);
    for(int i = 0; i < cnt ; i++){
        printf("%d. %s %d:%d %s\n", i+1, list[i].ID, list[i].chapter, list[i].verse, list[i].text);
    }
    free(list);
    free(search);
    fclose(file);
    return 0;
}