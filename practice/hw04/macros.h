#pragma once

#define BIT(x) (1 << x)
#define SETBIT(x, p) (x | BIT(p))
#define CLEARBIT(x, p) (x & ~BIT(p))
#define GETBIT(x, p) ((x >> p) & 1)
#define TOGGLEBIT(x, p) (x ^ BIT(p))
#define FOREACH(i, A)\
    for(int size = *(&A + 1) - A, j = 0, loop = 1; loop && j < size; j++, loop = !loop)\
        for(i = A + j; loop; loop = !loop)
#define RANGE(i, y, x)\
    for(i = y; (y < x ? i <= x : i >= x); (y < x ? ++i : --i))
