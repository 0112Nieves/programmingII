#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#define M_PI 3.14159265358979323846
struct _sBmpHeader
{
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

void print_bmp_header( sBmpHeader *pHeader )
{
    printf( "ID: %c%c\n", pHeader -> bm[0], pHeader -> bm[1] );
    printf( "Size: %u\n", pHeader -> size );
    printf( "Reserve: %u\n", pHeader -> reserve );
    printf( "Offset: %u\n", pHeader -> offset );
    printf( "Header Size: %u\n", pHeader -> header_size );
    printf( "Width: %u\n", pHeader -> width );
    printf( "Height: %u\n", pHeader -> height );
    printf( "Planes: %u\n", pHeader -> planes );
    printf( "Bits Per Pixel: %u\n", pHeader -> bpp );
    printf( "Compression: %u\n", pHeader -> compression );
    printf( "Bitmap Data Size: %u\n", pHeader -> bitmap_size );
    printf( "H-Resolution: %u\n", pHeader -> hres );
    printf( "V-Resolution: %u\n", pHeader -> vres );
    printf( "Used Colors: %u\n", pHeader -> used );
    printf( "Important Colors: %u\n", pHeader -> important );
    
    return;
}

int main()
{
    char file1[32], file2[32];
    double angle = 0, degree;
    printf("Please input a BMP file: ");
    if(fgets(file1, 32, stdin) == NULL){
        printf("Error\n");
        return 0;
    }
    if(file1[strlen(file1)-1] == '\n'){
        file1[strlen(file1)-1] = '\0';
    }
    FILE *f1 = fopen(file1, "rb");
    printf("Please input the output BMP file name: ");
    if(fgets(file2, 32, stdin) == NULL){
        printf("Error\n");
        return 0;
    }
    if(file2[strlen(file2)-1] == '\n'){
        file2[strlen(file2)-1] = '\0';
    }
    FILE *f2 = fopen(file2, "wb");
    if(f1 == NULL || f2 == NULL){
        printf("Couldn't open file.\n");
        return 0;
    }
    printf("Angle (0-90): ");
    scanf("%lf", &angle);
    if(angle < 0 || angle >= 90){
        printf("Please only input 0 <= degree < 90.\n");
        return 0;
    }
    degree = angle * M_PI / 180.0;
    sBmpHeader header;
    fread(&header, sizeof(header), 1, f1);
    uint8_t t[256] = {0};
    fread(t, 1, header.offset-sizeof(header), f1);
    int cnt = 0;
    int padding = (3 * header.width) % 4;
    if(padding != 0){
        padding = 4 - padding;
    }
    int width = header.width;
    double header_width = header.width + tan(degree) * header.height;
    header.width  = (uint32_t)round(header_width);
    int new_padding = (3 * header.width) % 4;
    if(new_padding != 0){
        new_padding = 4 - new_padding;
    }
    header.bitmap_size = (header.width * header.height * 3) + (header.height * new_padding);
    header.size = header.bitmap_size + header.offset;
    fwrite(&header, sizeof(header), 1, f2);
    fwrite(t, 1, header.offset-sizeof(header), f2);
    uint8_t color[3] = {255,255,255};
    uint8_t padding_tmp = 0;
    uint8_t p = 0;
    size_t count;
    while(!feof(f1)){
        uint8_t original[32384] = {0}, tmp[8] = {0};
        int space = (int)round(cnt * tan(degree));
        count = fread(original, 1, width*3, f1);
        for(int i = 0; i < padding; i++) fread(&padding_tmp, sizeof(padding_tmp), 1, f1);
        for(int i = 0; i < space; i++) fwrite(color, sizeof(color), 1, f2);
        fwrite(original, 1, count, f2);
        for(int i = 0; i < header.width - count/3 - space; i++) fwrite(color, sizeof(color), 1, f2);
        if(new_padding != 0){
            for(int i = 0; i < new_padding; i++) fwrite(&p, sizeof(p), 1, f2);
        }
        cnt++;
    }
    fclose(f1);
    fclose(f2);
    return 0;
}
