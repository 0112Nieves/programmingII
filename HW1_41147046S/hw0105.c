#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

int gcd(int a, int b){
    if(b == 0) return a;
    return gcd(b, a%b);
}

int lcm(int a, int b){
    return (a*b) / gcd(a, b);
}

char *set_section(size_t len, uint8_t beat, char *str){
    int new_len;
    char *s = NULL;
    if(len == 1){
        new_len = beat;
        str = (char *)calloc(beat+1, sizeof(char));
        memset(str, '0', beat*sizeof(char));
        str[beat] = ',';
        return str;
    }
    else{
        new_len = lcm(len-1, beat);
        s = (char *)malloc((new_len+1)*sizeof(char));
        int zero = new_len / (len-1);
        int cnt = 0;
        for(int i = 0; i < len-1; i++){
            for(int j = 0; j < zero; j++){
                if(j == 0) s[cnt] = str[i];
                else s[cnt] = '0';
                cnt++;
            }
        }
        s[new_len] = ',';
        return s;
    }
}

float count_duration(uint8_t len, uint8_t beat, char *str, uint8_t note, float bpm, float offset){
    uint8_t l = len-1;
    offset = fabs(offset);
    float duration = ((float)60.0/bpm)*((float)beat/l)*((float)4.0/note);
    for(int i = 0; i < strlen(str); i++){
        if(str[i] == ','){
            offset += duration*i;
            return offset;
        }
        else if(str[i] == '0') continue;
        else if((str[i]-'0') >= 1 && (str[i]-'0') <= 4)printf("[%c, %f]\n", str[i], offset+duration*i);
    }
}

int main()
{
    char str[256], course[15];
    float bpm = 0, offset = 0, o = 0;
    uint8_t cour, beat = 4, note = 4;
    while(fgets(str, 256, stdin) != NULL){
        if(strstr(str, "BPM:") != NULL) sscanf(str, "BPM:%f", &bpm);
        else if(strstr(str, "OFFSET:") != NULL){
            sscanf(str, "OFFSET:%f", &offset);
            o = offset;
        }
        else if(strstr(str, "COURSE:") != NULL){
            sscanf(str, "COURSE:%s", course);
            if(strcmp(course, "Easy") == 0) cour = 0;
            else if(strcmp(course, "Normal") == 0) cour = 1;
            else if(strcmp(course, "Hard") == 0) cour = 2;
            else if(strcmp(course, "Oni") == 0) cour = 3;
            else if(isdigit(course[0])) cour = course[0] - '0';
            else cour = 4;
        }
        else if(strstr(str, "#START") != NULL){
            printf("course: %d\n", cour);
            while(fgets(str, 256, stdin) != NULL){
                if(strstr(str, "#END") != NULL){
                    beat = 4; note = 4;
                    offset = o;
                    break;
                }
                if(strstr(str, "#MEASURE") != NULL) sscanf("#MEASURE %d/%d", &beat, &note);
                else if(strstr(str, "#BPMCHANGE") != NULL) sscanf(str, "#BPMCHANGE %f", &bpm);
                else if(isdigit(str[0]) || str[0] == ','){
                    if(strchr(str, ',') == NULL){
                        char add[256];
                        fgets(add, 256, stdin);
                        str[strlen(str)-2] = '\0';
                        strcat(str, add);
                    }
                    str[strlen(str)-2] = '\0';
                    strcpy(str, set_section(strlen(str), beat, str));
                    offset = count_duration(strlen(str), beat, str, note, bpm, offset);
                }
            }
            printf("\n");
        }
    }
    return 0;
}