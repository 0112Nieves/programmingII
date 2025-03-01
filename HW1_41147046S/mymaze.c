#include "mymaze.h"
long long unsigned int cost[20][20], dis[20][20];
uint8_t p[400];
typedef enum _Doors {
    up,
    right,
    down,
    left
}Doors;
uint8_t door(const sRoom *pMaze, int y, int x, Doors d, int col){
    int n;
    switch (d)
    {
        case up:
            n = 6;
            break;
        case right:
            n = 4;
            break;
        case down:
            n = 2;
            break;
        case left:
            n = 0;
            break;    
    }
    uint8_t tmp = pMaze[y*col+x].doors >> n;
    return tmp & 3;
}

void choose_road(const sRoom *pMaze, int y, int x, int row, int col){
    if(y < 0 || x < 0 || y > row-1 || x > col-1) return;
    if(door(pMaze, y, x, down, col) == door(pMaze, y+1, x, up, col) 
        && dis[y][x]+1 <= dis[y+1][x]){
        if(dis[y][x]+1 == dis[y+1][x]){
            if(cost[y+1][x] <= cost[y][x] + pMaze[y*col+x].cost) return;
        }
        dis[y+1][x] = dis[y][x] + 1;
        if(cost[y][x] + pMaze[y*col+x].cost < cost[y+1][x])
            cost[y+1][x] = cost[y][x] + pMaze[(y+1)*col+x].cost;
        p[(y+1)*col+x] = y*col+x;
        choose_road(pMaze, y+1, x, row, col);
    }
    if(door(pMaze, y, x, right, col) == door(pMaze, y, x+1, left, col)
        && dis[y][x]+1 <= dis[y][x+1]){
        if(dis[y][x]+1 == dis[y][x+1]){
            if(cost[y][x+1] <= cost[y][x] + pMaze[y*col+x].cost) return;
        }
        dis[y][x+1] = dis[y][x] + 1;
        if(cost[y][x] + pMaze[y*col+x].cost < cost[y][x+1])
            cost[y][x+1] = cost[y][x] + pMaze[y*col+(x+1)].cost;
        p[y*col+(x+1)] = y*col+x;
        choose_road(pMaze, y, x+1, row, col);
    }
    if(door(pMaze, y, x, up, col) == door(pMaze, y-1, x, down, col) 
        && dis[y][x]+1 <= dis[y-1][x]){
        if(dis[y][x]+1 == dis[y-1][x]){
            if(cost[y-1][x] <= cost[y][x] + pMaze[y*col+x].cost) return;
        }
        dis[y-1][x] = dis[y][x] + 1;
        if(cost[y][x] + pMaze[y*col+x].cost < cost[y-1][x])
            cost[y-1][x] = cost[y][x] + pMaze[(y-1)*col+x].cost;
        p[(y-1)*col+x] = y*col+x;
        choose_road(pMaze, y-1, x, row, col);
    }  
    if(door(pMaze, y, x, left, col) == door(pMaze, y, x-1, right, col)
        && dis[y][x]+1 <= dis[y][x-1]){
        if(dis[y][x]+1 == dis[y][x-1]){
            if(cost[y][x-1] <= cost[y][x] + pMaze[y*col+x].cost) return;
        }
        dis[y][x-1] = dis[y][x] + 1;
        if(cost[y][x] + pMaze[y*col+x].cost < cost[y][x-1])
            cost[y][x-1] = cost[y][x] + pMaze[y*col+(x-1)].cost;
        p[y*col+(x-1)] = y*col+x;
        choose_road(pMaze, y, x-1, row, col);
    }
}

void find(sPath *pMinPath, int x, int col, int cnt){
    if(p[x] == x){
        pMinPath->pPath[cnt].col = x % col;
        pMinPath->pPath[cnt].row = x / col;
        return;
    }
    else{
        find(pMinPath, p[x], col, cnt-1);
        pMinPath->pPath[cnt].col = x % col;
        pMinPath->pPath[cnt].row = x / col;
    }
}

int32_t find_min_path( const sRoom *pMaze , const uint8_t row, const uint8_t col, sPath *pMinPath ){
    if(pMaze == NULL || pMinPath == NULL) return -1;
    memset(dis, 0xffff, 400);
    memset(cost, 0xffff, 400);
    pMinPath->pPath = NULL;
    dis[0][0] = 1;
    cost[0][0] = pMaze[0].cost;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            p[i*col+j] = i*col+j;
        }
    }
    choose_road(pMaze, 0, 0, row, col);
    pMinPath->length = dis[row-1][col-1];
    pMinPath->cost = cost[row-1][col-1];
    pMinPath->pPath = malloc(pMinPath->length * sizeof(struct _sPath));
    if(pMinPath->length == 0xffff) return 0;
    else{
        find(pMinPath, row*col-1, col, pMinPath->length-1);
    }
    return 1;
}