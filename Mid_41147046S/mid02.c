#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main()
{
    char codename[32] = {0}, iName[32] = {0}, oName[32] = {0}, str[256];
    char table[30][16] = {0};
    printf("Codebook: ");
    if(fgets(codename, 32, stdin) == NULL){
        printf("Error\n");
        return 0;
    }
    if(codename[strlen(codename)-1] == '\n'){
        codename[strlen(codename)-1] = '\0';
    }
    FILE *code = fopen(codename, "r");
    printf("Input File: ");
    if(fgets(iName, 32, stdin) == NULL){
        printf("Error\n");
        return 0;
    }
    if(iName[strlen(iName)-1] == '\n'){
        iName[strlen(iName)-1] = '\0';
    }
    int tmp = -1;
    while(fgets(str, 256, code) != NULL){
        char *token = strtok(str, ":");
        while(token != NULL){
            if((isalpha(token[0]) && strlen(token) == 1) || strcmp(token, "space") == 0 || strcmp(token, "comma") == 0 || strcmp(token, "period") == 0){
                if(strlen(token) == 1) tmp = token[0] - 'a';
                else if(strcmp(token, "space") == 0) tmp = 26;
                else if(strcmp(token, "comma") == 0) tmp = 27;
                else if(strcmp(token, "period") == 0) tmp = 28;
            }
            else strcpy(table[tmp], token);
            token = strtok(NULL, ": ");
        }
    }
    FILE *in = fopen(iName, "r");
    printf("Output File: ");
    if(fgets(oName, 32, stdin) == NULL){
        printf("Error\n");
        return 0;
    }
    if(oName[strlen(oName)-1] == '\n'){
        oName[strlen(oName)-1] = '\0';
    }
    FILE *out = fopen(oName, "wb+");
    if(code == NULL || in == NULL || out == NULL){
        printf("Open file erroe.\n");
        return 0;
    }
    char s[2048] = {0};
    int bit_count = 0;
    char bit, byte = 0;
    while(fgets(s, 2048, in) != NULL){
        int i = 0;
        char *bit_seq = NULL;
        while(s[i] != '\0'){
            if (isalpha(s[i])){
                bit_seq = table[s[i]-'a'];
            }
            else if(s[i] == ' '){
                bit_seq = table[26];
            }
            else if(s[i] == ','){
                bit_seq = table[27];
            }
            else if(s[i] == '.'){
                bit_seq = table[28];
            }
            if(bit_seq[strlen(bit_seq)-1] == '\n'){
                bit_seq[strlen(bit_seq)-1] = '\0';
            }
            int bit_len = strlen(bit_seq);
            for(int j = 0; j < bit_len; j++){
                bit = bit_seq[j] - '0';
                byte |= (bit << (7 - bit_count));
                bit_count++;
                if(bit_count == 8){
                    fwrite(&byte, 1, 1, out);
                    bit_count = 0;
                    byte = 0;
                }
            }
            i++;
        }
    }
    if (bit_count) {
        for (int j = 0; j < 8 - bit_count; j++) {
            if(j == 0) bit = 1;
            else bit = 0;
            byte |= (bit << (7 - bit_count - j));
        }
        fwrite(&byte, 1, 1, out);
    }
    fclose(code); fclose(in); fclose(out);
    return 0;
}