#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#pragma once
typedef struct abacus{
    int num;
    int *pUpperRod, *pLowerRod;
}sAbacus;
sAbacus * abacus_init( void );
void abacus_free( sAbacus * );
int32_t abacus_set( sAbacus *pA, char *pStr );
int32_t abacus_add( sAbacus *pA, sAbacus b, sAbacus c );
int32_t abacus_del( sAbacus *pA, sAbacus b, sAbacus c );
int32_t abacus_print( sAbacus a );
