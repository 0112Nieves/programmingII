#include <stdio.h>
#include <getopt.h>
#include <cjson/cJSON.h>
#include <curl/curl.h>
static struct option long_options[] = {
    {"help", no_argument, 0, 0},
    {0, 0, 0, 0}
};
void printHelp()
{
    printf("-s : symbol of stock\n-i : input JSON file name\n-o : output JSON file name\n");
    return;
}
int main(int argc, char *argv[])
{
    char c;
    char *stackName = NULL, *inputName = NULL, *outputName = NULL;
    int index = 0;
    while((c = getopt_long(argc, argv, "s:i:o:", long_options, &index)) != -1){
        switch(c){
            case 's':
                stackName = optarg;
                break;
            case 'i':
                inputName = optarg;
                break;
            case 'o':
                outputName = optarg;
                break;
            case 0:
                printHelp();
                return 0;
            case '?':
                printf("Unknown argument.please --help to get more information.\n");
                break;
        }
    }
    return 0;
}