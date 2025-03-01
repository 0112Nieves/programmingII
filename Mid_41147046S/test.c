#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// 定义 BMP 头部结构体
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

// 缩放 BMP 图像
void resize(FILE *f, FILE *t, int w, int h){
    sBmpHeader header;
    memset(&header, 0, sizeof(sBmpHeader));
    if(f == NULL || t == NULL){
        printf("Error open file in function.\n");
        return;
    }
    fread(&header, sizeof(header), 1, f);
    int old_width = header.width;
    int old_height = header.height;
    char *src_data = (char *)malloc(old_width*old_height*3);
    fseek(f, header.offset, SEEK_SET);
    fread(src_data, old_width*old_height*3, 1, f);
    header.size = w*h*3 + header.offset;
    header.width = w;
    header.height = h;
    fwrite(&header, sizeof(sBmpHeader), 1, t);
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
    fseek(t, header.offset, SEEK_SET);
    fwrite(dest_data, w*h*3, 1, t);
    free(dest_data);
    free(src_data);
    return;
}

int main()
{
    char covername[64] = {0}, nName[64] = {0}, oName[64] = {0};
    int x = 0, y = 0, w = 0, h = 0;
    
    // 读取输入的文件名
    printf("cover: ");
    if(fgets(covername, 64, stdin) == NULL){
        printf("Error\n");
        return 0;
    }
    covername[strcspn(covername, "\n")] = 0; // 移除换行符
    
    // 打开输入的 BMP 文件
    FILE *cover = fopen(covername, "rb");
    if(cover == NULL){
        printf("Error open cover file.\n");
        return 0;
    }

    // 读取 BMP 头部信息
    sBmpHeader header;
    fread(&header, sizeof(header), 1, cover);

    // 读取其他输入参数
    printf("x (in pixel): ");
    scanf("%d", &x);
    printf("y (in pixel): ");
    scanf("%d", &y);
    printf("w (in pixel): ");
    scanf("%d", &w);
    printf("h (in pixel): ");
    scanf("%d", &h);
    printf("new: ");
    if(fgets(nName, 64, stdin) == NULL){
        printf("Error\n");
        return 0;
    }
    nName[strcspn(nName, "\n")] = 0; // 移除换行符

    // 打开新的 BMP 文件和输出文件
    FILE *new = fopen(nName, "rb");
    FILE *out = fopen("out.bmp", "wb+");
    if(new == NULL || out == NULL){
        printf("Error open file.\n");
        return 0;
    }

    // 调整图像大小
    resize(new, out, w, h);

    // 处理 BMP 文件的像素数据
    uint8_t tmp[32384] = {0};
    fseek(cover, header.offset, SEEK_SET); // 定位到像素数据开始位置
    int nowY = header.height;
    while(!feof(cover)){
        if(nowY == y){
            for(int i = 0; i < h; i++){
                fread(tmp, 1, x*3, cover);
                fwrite(tmp, 1, x*3, out);
                fread(tmp, 1, w*3, new);
                fwrite(tmp, 1, w*3, out);
                fread(tmp, 1, (header.width-w-x)*3, cover);
                fwrite(tmp, 1, (header.width-w-x)*3, out);
            }
        }
        size_t count = fread(tmp, 1, header.width * 3, cover);
        fwrite(tmp, 1, count, out);
        nowY--;
    }

    // 关闭文件并释放资源
    fclose(cover);
    fclose(new);
    fclose(out);
    
    return 0;
}
