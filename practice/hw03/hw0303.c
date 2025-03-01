#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
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

struct Pixel{
    uint8_t B, G, R;
}__attribute((__packed__));
typedef struct Pixel pixel;

static struct option long_options[] = {
    {"width", required_argument, 0, 'w'},
    {"height", required_argument, 0, 'h'},
    {"number", optional_argument, 0, 'n'},
    {"input", required_argument, 0, 'i'},
    {"output", optional_argument, 0, 'o'},
    {"help", no_argument, 0, 0},
    {0, 0, 0, 0}
};

void printHelp(){
    printf(
        "-x: mandatory, x value.\n"
        "-y: mandatory, y value.\n"
        "-w, --width: mandatory, w value.\n"
        "-h, --height: mandatory, h value.\n"
        "-n, --number: optional, n value. Default n is 2.\n"
        "-i, --input: mandatory, the input BMP file name.\n"
        "-o, --output: optional, the output BMP file name. Default name is output.bmp.\n"
        "--help: print the usage help.\n"
    );
    return;
}

int main(int argc, char *argv[])
{
    char c;
    char *input = NULL, *output = NULL;
    int index = 0;
    long n = 2, x, y, w, h;
    bool help = false;
    while((c = getopt_long(argc, argv, "x:y:w:h:n:i:o:", long_options, &index)) != -1){
        switch(c){
            case 'x':
                x = strtol(optarg, NULL, 10);
                break;
            case 'y':
                y = strtol(optarg, NULL, 10);
                break;
            case 'w':
                w = strtol(optarg, NULL, 10);
                break;
            case 'h':
                h = strtol(optarg, NULL, 10);
                break;
            case 'n':
                n = strtol(optarg, NULL, 10);
                break;
            case 'i':
                input = optarg;
                break;
            case 'o':
                if(optarg == NULL) output = "output.bmp";
                output = optarg;
                break;
            case 0:
                printHelp();
                break;
            case '?':
                printf("Unknown argument.please --help to get more information.\n");
                break;

        }
    }
    // 錯誤處理
    if(help) return 0;
    if(x < 0 || y < 0 || w < 0 || h < 0) return 0;
    if(input == NULL || output == NULL){
        printf("There should have input name and out put name.\n");
        return 0;
    }

    FILE *in = fopen(input, "rb");
    if(in == NULL){
        printf("Inputfile couldn't be opened.\n");
        return 0;
    }
    FILE *out = fopen(output, "wb+");
    if(out == NULL){
        printf("Outputfile couldn't be opened.\n");
        fclose(in);
        return 0;
    }
    sBmpHeader header;
    long current , after, jump, pos;
    fread(&header, sizeof(header), 1, in);
    fwrite(&header, sizeof(header), 1, out);
    size_t count;
    while(!feof(in)){
        char original[1024] = {0};
        count = fread(original, 1, 1024, in);
        fwrite(original, count, 1, out);
    }
    char *addr = mmap(NULL, count, PROT_WRITE | PROT_READ, MAP_SHARED, out);
    sBmpHeader* header = (sBmpHeader*)addr;
    pixel *now, new;
    int B, G, R, div = n*n;
    x = header->height - (x-h);
    if(x < 0){
        h += x;
        x = 0;
    }
    for(long i = 0; i < h; i += n){
        for(long j = 0; j < w; j += n){
            B = 0; G = 0; R = 0;
            current = ftell(out);
            fseek(out, sizeof(pixel)*i, SEEK_CUR);
            for(int px = 0; px < n; px++){
                for(int py = 0; py < n; py++){
                    fseek(out, sizeof(pixel)*py, SEEK_CUR);
                    fread(&now, sizeof(pixel), 1, out);
                    B += now->B;
                    G += now->G;
                    R += now->R;
                }
                fseek(out, 3*(count-n), SEEK_CUR);
            }
            after = ftell(out);
            fseek(out, current-after, SEEK_CUR);
            B /= div; G /= div; R /= div;
            now->B = B; now->G = G; now->R = R;
            for(int px = 0; px < n; px++){
                for(int py = 0; py < n; py++){
                    fseek(out, sizeof(pixel)*py, SEEK_CUR);
                    fwrite(&now, sizeof(pixel), 1, out);
                }
                if(px != (n-1)) fseek(out, 3*(count-n), SEEK_CUR);
            }
            after = ftell(out);
            fseek(out, current-after, SEEK_CUR);
        }
    }
    fclose(in); fclose(out);
    return 0;
}