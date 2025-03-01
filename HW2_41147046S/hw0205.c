#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _gguf_string_t{
    uint64_t len;
    char string[];
}gguf_string_t;

typedef union _gguf_metadata_value_t gguf_metadata_value_t;
union gguf_metadata_value_t{
    uint8_t uint8;
    int8_t int8;
    uint16_t uint16;
    int16_t int16;
    uint32_t uint32;
    int32_t int32;
    float float32;
    uint64_t uint64;
    int64_t int64;
    double float64;
    bool bool_;
    gguf_string_t string;
    struct {
        uint64_t len;
        gguf_metadata_value_t *array;
    } array;
};

typedef enum _gguf_metaF_value_type{
    GGUF_METADATA_VALUE_TYPE_UINT8 = 0,
    GGUF_METADATA_VALUE_TYPE_INT8 = 1,
    GGUF_METADATA_VALUE_TYPE_UINT16 = 2,
    GGUF_METADATA_VALUE_TYPE_INT16 = 3,
    GGUF_METADATA_VALUE_TYPE_UINT32 = 4,
    GGUF_METADATA_VALUE_TYPE_INT32 = 5,
    GGUF_METADATA_VALUE_TYPE_FLOAT32 = 6,
    GGUF_METADATA_VALUE_TYPE_BOOL = 7,
    GGUF_METADATA_VALUE_TYPE_STRING = 8,
    GGUF_METADATA_VALUE_TYPE_ARRAY = 9,
    GGUF_METADATA_VALUE_TYPE_UINT64 = 10,
    GGUF_METADATA_VALUE_TYPE_INT64 = 11,
    GGUF_METADATA_VALUE_TYPE_FLOAT64 = 12,
}gguf_metadata_value_type;

typedef struct _gguf_metadata_kv_t gguf_metadata_kv_t;
struct gguf_metadata_kv_t{
    gguf_string_t key;
    gguf_metadata_value_type value_type;
    // gguf_metadata_value_t value;
};

typedef struct _gguf_header_t{
    uint32_t magic;
    uint32_t version;
    uint64_t tensor_count;
    uint64_t metadata_kv_count;
    gguf_metadata_kv_t *metadata_kv;
}gguf_header_t;

typedef struct _gguf_tensor_info_t{
    gguf_string_t name;
    uint32_t n_dimensions;
    uint64_t offset;
    uint64_t *dimensions;
}gguf_tensor_info_t;

typedef enum _ggml_type{
    GGML_TYPE_F32 = 0,
    GGML_TYPE_F16 = 1,
    GGML_TYPE_Q4_0 = 2,
    GGML_TYPE_Q4_1 = 3,
    GGML_TYPE_Q5_0 = 6,
    GGML_TYPE_Q5_1 = 7,
    GGML_TYPE_Q8_0 = 8,
    GGML_TYPE_Q8_1 = 9,
    GGML_TYPE_Q2_K = 10,
    GGML_TYPE_Q3_K = 11,
    GGML_TYPE_Q4_K = 12,
    GGML_TYPE_Q5_K = 13,
    GGML_TYPE_Q6_K = 14,
    GGML_TYPE_Q8_K = 15,
    GGML_TYPE_I8 ,
    GGML_TYPE_I16 ,
    GGML_TYPE_I32 ,
    GGML_TYPE_COUNT ,
}ggml_type;

typedef struct _gguf_file_t{
    gguf_header_t header;
    uint8_t *other_binary_things;
    gguf_tensor_info_t *tensor_infos;
}gguf_file_t;

char *decimal_to_little_endian(void *data, size_t size){
    char *str = (char *)malloc(size * 2 + 1);
    if(str == NULL){
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for(size_t i = 0; i < size; i++){
        uint8_t byte = (*((uint8_t *)data + (size-1-i)));
        sprintf(str + i * 2, "%02X", byte);
    }
    str[size * 2] = '\0';
    return str;
}

int main()
{
    char fi[64] = {0};
    printf("Please input file name: ");
    if(fgets(fi, 32, stdin) == NULL){
        printf("Error\n");
        return 0;
    }
    if(fi[strlen(fi)-1] == '\n'){
        fi[strlen(fi)-1] = '\0';
    }
    FILE *f = fopen(fi, "r");
    if(f == NULL){
        printf("Error open file.\n");
        return 0;
    }
    gguf_file_t file;
    size_t parameter = fread(&file, sizeof(file), 1, f);
    char *magic = decimal_to_little_endian(&file.header.magic, sizeof(file.header.magic));
    if(strcmp(magic, "46554747") == 0){
        printf("GGUF: true\n");
    }
    else{
        printf("GGUF: false\n");
        return 0;
    }
    printf("Parameters: %ld\n", parameter); // change
    printf("Metadata                Value\n");
    printf("version                 %d\n", file.header.version);
    printf("tensor_count            %ld\n", file.header.tensor_count);
    printf("kv_count                %ld\n", file.header.metadata_kv_count);
    printf("general.architecture            \n");
    strcpy(file.tensor_infos->name.string, "your_string_here");
    printf("general.name                    %s\n", file.tensor_infos->name);
    printf("general.type                    \n");
    fclose(f);
    return 0;
}