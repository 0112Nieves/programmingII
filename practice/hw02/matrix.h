#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
typedef struct _sMatrix{
   int32_t rows;
   int32_t cols;
   int32_t** data;
}sMatrix;
sMatrix * matrix_init( uint8_t m, uint8_t n);
int32_t matrix_set( sMatrix *pM, uint8_t m, uint8_t n, int32_t value );
void matrix_print( const sMatrix *pM );
int32_t matrix_add( sMatrix *pA, const sMatrix *pB, const sMatrix *pC );
int32_t matrix_multiply( sMatrix *pA, const sMatrix *pB, const sMatrix *pC );
int32_t matrix_transpose( sMatrix *pA);
int32_t matrix_det(const sMatrix *pA, int32_t *pAns);
int32_t matrix_free( sMatrix *pA);
