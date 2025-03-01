#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

void print_colored_text(const char *color, const char *text, int base) {
    unsigned int color_value;
    if(base == 16) color_value = strtoul(color+2, NULL, 16);
    else color_value = strtol(color, NULL, 10);
    unsigned char r = (color_value >> 16) & 0xFF;
    unsigned char g = (color_value >> 8) & 0xFF;
    unsigned char b = color_value & 0xFF;
    printf("\x1b[38;2;%d;%d;%dm%s\x1b[0m\n", r, g, b, text);
}

char *cutToken(char *str, int pos){
    int cnt = 1;
    char *token = strtok(str, ",");
    while(token != NULL){
        token = strtok(NULL, ",");
        cnt++;
        if(cnt == pos) break;
    }
    return token;
}

double changeTime(const char *time_str){
    int hr, min;
    double sec;
    sscanf(time_str, "%d:%d:%lf", &hr, &min, &sec);
    return hr * 3600 + min * 60 + sec;
}

char *find_last_occurrence(const char *str, const char *substr) {
    char *result = NULL;
    char *found;
    while ((found = strstr(str, substr)) != NULL) {
        result = found;
        str = found + 1;
    }
    return result;
}

int main()
{
    char file[32], str[256], color[16], sTime[16], world[256], eTime[16];
    int shift = 0;
    double now = 0, start, speed = 1, real_speed, end;
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
    printf("Time Shift ( -10 ~ 10 ): ");
    scanf("%d", &shift);
    if(shift < -10 || shift > 10){
        printf("Please only input -10 ~ 10.\n");
        return 0;
    }
    printf("Speed (0.25,0.5,0.75,1,1.25,1.5,1.75,2): ");
    scanf("%lf", &speed);
    now += shift;
    real_speed = 1 / speed;
    while(fgets(str, 256, f) != NULL){
        if(strstr(str, "Dialogue:") != NULL){
            str[strlen(str)-1] = '\0';
            char s[256];
            strcpy(s, str);
            char *token = strtok(str, ",");
            int cnt = 0;
            while(token != NULL){
                if(cnt == 1) strcpy(sTime, token);
                if(cnt == 2) strcpy(eTime, token);
                cnt++;
                token = strtok(NULL, ",");
            }
            start = changeTime(sTime);
            end = changeTime(eTime);
            char *tmp = find_last_occurrence(s, ",,");          
            if(tmp != NULL){
                tmp += 2;
                strcpy(world, tmp);
            }
            while(now < start*real_speed){
                usleep(10000);
                now += 0.01;
            }
            int base = 0;
            if(strchr(color, '&') != NULL) base = 16;
            print_colored_text(color, world, base);
            sleep(end-start);
            now += (end-start);
            printf("\033[2J");
            fflush(stdout);
        }
        else if(strstr(str, "Style:") != NULL){
            strcpy(color, cutToken(str, 4));
        }
    }
    fclose(f);
    return 0;
}