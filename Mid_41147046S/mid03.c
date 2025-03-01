#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
struct _sBmpHeader {
    char		bm[2];
    uint32_t	size;
    uint32_t	reserve;
    uint32_t	offset;
    uint32_t	header_size;
    uint32_t	width;
    uint32_t	height;
    uint16_t	planes;
    uint16_t	bpp;
    uint32_t	compression;
    uint32_t	bitmap_size;
    uint32_t	hres;
    uint32_t	vres;
    uint32_t	used;
    uint32_t	important;
}__attribute__ ((__packed__));
typedef struct _sBmpHeader sBmpHeader;

void resize(FILE *f, FILE *t, int w, int h){
    sBmpHeader head;
    memset(&head, 0, sizeof(sBmpHeader));
    if(f == NULL || t == NULL){
        printf("Error open file in function.\n");
        return;
    }
    fread(&head, sizeof(head), 1, f);
    int old_width = head.width;
    int old_height = head.height;
    char *src_data = (char *)malloc(old_width*old_height*3);
    fseek(f, head.offset, SEEK_SET);
    fread(src_data, old_width*old_height*3, 1, f);
    head.size = w*h*3 + head.offset;
    head.width = w;
    head.height = h;
    fwrite(&head, sizeof(sBmpHeader), 1, t);
    long dwsrcX, dwsrcY;
    char *pucDest, *pucSrc;
    char *dest_data = (char *)malloc(w*h*3);
    for(int i = 0; i < h; i++){
        dwsrcY = i*old_height/h;
        pucDest = dest_data+i*w*3;
        pucSrc = src_data + dwsrcY*old_width*3;
        for(int j = 0; j < w; j++){
            dwsrcX = j*old_width/w;
            memcpy(pucDest+j*3, pucSrc+dwsrcX*3, 3);
        }
    }
    fseek(t, head.offset, SEEK_SET);
    fwrite(dest_data, w*h*3, 1, t);
    free(dest_data);
    free(src_data);
    return;
}

int main()
{
    char covername[64] = {0}, nName[64] = {0}, oName[64] = {0};
    int x = 0, y = 0, w = 0, h = 0;
    printf("cover: ");
    if(fgets(covername, 64, stdin) == NULL){
        printf("Error\n");
        return 0;
    }
    if(covername[strlen(covername)-1] == '\n'){
        covername[strlen(covername)-1] = '\0';
    }
    FILE *cover = fopen(covername, "rb");
    printf("x (in pixel): ");
    scanf("%d", &x);
    printf("y (in pixel): ");
    scanf("%d", &y);
    printf("w (in pixel): ");
    scanf("%d", &w);
    printf("h (in pixel): ");
    scanf("%d", &h);
    printf("new: ");
    scanf("%s", nName);
    // if(fgets(nName, 64, stdin) == NULL){
    //     printf("Error\n");
    //     return 0;
    // }
    if(nName[strlen(nName)-1] == '\n'){
        nName[strlen(nName)-1] = '\0';
    }
    FILE *new = fopen(nName, "rb");
    printf("output: ");
    scanf("%s", oName);
    // if(fgets(oName, 64, stdin) == NULL){
    //     printf("Error\n");
    //     return 0;
    // }
    if(oName[strlen(oName)-1] == '\n'){
        oName[strlen(oName)-1] = '\0';
    }
    FILE *out = fopen(oName, "wb+");
    if(cover == NULL || new == NULL || out == NULL){
        printf("Error open file.\n");
        return 0;
    }
    sBmpHeader header;
    uint8_t t[512] = {0};
    fread(&header, sizeof(header), 1, cover);
    fread(t, 1, header.offset-sizeof(header), cover);
    fwrite(&header, sizeof(header), 1, out);
    fwrite(t, 1, header.offset-sizeof(header), out);
    char tmpName[64] = "tmp.bmp";
    FILE *tmpf = fopen(tmpName, "wb+");
    resize(new, tmpf, w, h);
    int nowY = header.height;
    while(!feof(cover)){
        if(nowY == y){
            for(int i = 0; i < h; i++){
                uint8_t tmp1[8096] = {0}, tmp2[8096] = {0}, tmp3[8096] = {0};
                fread(tmp1, 1, x*3, cover);
                fwrite(tmp1, 1, x*3, out);
                fread(tmp2, 1, w*3, tmpf);
                fwrite(tmp2, 1, w*3, out);
                fread(tmp2, 1, w*3, cover);
                fread(tmp3, 1, (header.width-w-x)*3, cover);
                fwrite(tmp3, 1, (header.width-w-x)*3, out);
            }
        }
        uint8_t tmp[32384] = {0};
        size_t count = fread(tmp, 1, header.width * 3, cover);
        fwrite(tmp, 1, count, out);
        nowY--;
    }
    system("rm tmp.bmp");
    fclose(cover); fclose(new); fclose(out);
    return 0;
}