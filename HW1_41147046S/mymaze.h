#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#pragma once
typedef struct _sRoom
{
    uint32_t cost;
    uint8_t doors;
} sRoom;

typedef struct _sPoint
{
    uint32_t row;
    uint32_t col;
} sPoint;

typedef struct _sPath
{
    uint32_t length;
    uint32_t cost;
    sPoint *pPath;
} sPath;

int32_t find_min_path( const sRoom *pMaze , const uint8_t row, const uint8_t col, sPath *pMinPath );