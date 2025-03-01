#include <stdio.h>
#include <getopt.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MIN(a, b) (a < b ? a : b)
typedef struct _bmpHeader {
    char BM[2];
    int32_t size;
    int32_t Reserved;
    int32_t offset;
    int32_t headerSize;
    int32_t width;
    int32_t height;
    int16_t planes;
    int16_t bitperpixel;
    int32_t compression;
    int32_t imageSize;
    int32_t horizontal;
    int32_t vertical;
    int32_t palette;
    int32_t important;
} __attribute__((packed)) bmpHeader;
typedef struct _fileHeader {
    int32_t fileNameLength;
    int32_t filesize;
} __attribute__((packed)) fileHeader;
bool check(bmpHeader h){
    if(h.BM[0] != 'B' || h.BM[1] != 'M') return 0;
    return 1;
}
int main(int argc, char *argv[])
{
    char c;
    char bmpName[128], fileName[128];
    int opt = 0;
    if(argc > 4){
        printf("There are too many option.\n");
        return 0;
    }
    else if(argc < 3){
        printf("There are too less option.\n");
        return 0;
    }
    while((c = getopt(argc, argv, "i:e:")) != -1){
        switch(c){
            case 'i':
                opt = 1;
                break;
            case 'e':
                break;
            default:
                printf("You can only use -i or -e.Please try again.\n");
                return 0;
        }
    }
    if(opt == 1){
        strcpy(fileName, argv[argc-2]);
        strcpy(bmpName, argv[argc-1]);
        FILE *bmp = fopen(bmpName, "rb+");
        FILE *file = fopen(fileName, "r");
        if(bmp == NULL || file == NULL){
            printf("file open error\n");
            return 0;
        }
        bmpHeader h;
        fread(&h, sizeof(bmpHeader), 1, bmp);
        if(!check(h)){
            printf("This is not a BMP file.\n");
            fclose(bmp);
            fclose(file);
            return 0;
        }
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        fseek(file, 0, SEEK_SET);
        int32_t length = strlen(fileName);
        fseek(bmp, 0, SEEK_END);
        fwrite(&length, sizeof(int32_t), 1, bmp);
        fwrite(&size, sizeof(int32_t), 1, bmp);
        fwrite(fileName, strlen(fileName), 1, bmp);
        uint8_t buffer[1024];
        int bytes;
        while((bytes = fread(buffer, 1, 1024, file)) > 0){
            fwrite(buffer, 1, bytes, bmp);
        }
        fclose(bmp);
        fclose(file);
    }
    else if(opt == 0){
        strcpy(bmpName, argv[argc-1]);
        FILE *bmp = fopen(bmpName, "rb+");
        if(bmp == NULL){
            printf("file open error\n");
            return 0;
        }
        fileHeader f;
        bmpHeader b;
        fread(&b, sizeof(bmpHeader), 1, bmp);
        if(!check(b)){
            printf("This is not a BMP file.\n");
            fclose(bmp);
            return 0;
        }
        fseek(bmp, b.size, SEEK_SET);
        fread(&f, sizeof(fileHeader), 1, bmp);
        printf("1");
        printf("%d\n", f.filesize);
        printf("1");
        while(fread(&f, 1, sizeof(fileHeader), bmp) > 0){
            printf("1");
            fread(fileName, sizeof(char), f.fileNameLength, bmp);
            fileName[f.fileNameLength] = '\0';
            FILE *file = fopen(fileName, "a+");
            if(file == NULL){
                printf("file open error\n");
                return 0;
            }
            uint8_t buffer[1024];
            int cnt = 0;
            while(cnt < f.filesize){
                int n = fread(buffer, MIN(1024, f.filesize - cnt), 1, bmp);
                fwrite(buffer, sizeof(char), n, file);
                cnt += n;
            }
            printf("2");
            fclose(file);
        }
        fclose(bmp);
    }
    return 0;
}