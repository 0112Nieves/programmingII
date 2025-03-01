#include <stdarg.h> 
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
int main(int argc, char *argv[])
{
    char c;
    char *keyName = NULL, *inputName = NULL, *outputName = NULL; 
    bool en = false, de = false;
    while((c = getopt(argc, argv, "edk:i:o:")) != -1){
        switch(c){
            case 'e':
                en = 1;
                break;
            case 'd':
                de = 1;
                break;
            case 'k':
                keyName = optarg;
                break;
            case 'i':
                inputName = optarg;
                break;
            case 'o':
                outputName = optarg;
                break;
            default:
                printf("No such option.\n");
                break;
        }
    }
    FILE *key = fopen(keyName, "r");
    if (key == NULL) {
        fprintf(stderr, "Unable to open file: %s\n", keyName);
        return 0;
    }
    FILE *in = fopen(inputName, "r");
    if (in == NULL) {
        fprintf(stderr, "Unable to open file: %s\n", inputName);
        return 0;
    }
    FILE *out = fopen(outputName, "w+");
    if (out == NULL) {
        fprintf(stderr, "Unable to open file: %s\n", outputName);
        return 0;
    }
    char i, o, k;
    if(en || de){
        while((fread(&i, 1, 1, in)) != 0){
            if((fread(&k, 1, 1, key)) == 0){
                rewind(key);
                fread(&k, 1, 1, key);
            }
            fprintf(out, "%c", ((i-'\0')^(k-'\0'))+'\0');
        }
    }
    fclose(key); fclose(in); fclose(out);
    return 0;
}