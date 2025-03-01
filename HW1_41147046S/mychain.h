#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma once
typedef struct _sPoly
{
    uint32_t size;
    uint32_t *pPowers;
    int32_t *pCoefficients;
} sPoly;

int32_t chain_rule( sPoly *pResult , const sPoly *pFy, const sPoly *pFx );