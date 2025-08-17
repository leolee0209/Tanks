#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif
#include "map.h"
#include <ncurses.h>
#include <stdlib.h>
#include "circleList.h"
#include "tank.h"
#include "characters.h"

int initMap(Map *map)
{
    char *mapPath, *mapInfoPath;
    if (!getFilePaths("/home/leo/Projects/Tanks/data/box/", &mapPath, &mapInfoPath))
    {
        return FAIL;
    }
    if (!loadMapInfo(mapInfoPath, map))
    {
        free(mapPath);
        free(mapInfoPath);
        return FAIL;
    }
    if (!loadMap(mapPath, map))
    {
        free(mapPath);
        free(mapInfoPath);
        return FAIL;
    }

    free(mapInfoPath);
    free(mapPath);
    return SUCCESS;
}
void closeMap(Map *map)
{
    for (int i = 0; i < map->height; i++)
    {
        free(map->map[i]);
    }
    free(map->map);
}

int initEntityList(Map *map, clnode *start)
{
    if (!clinit(start))
    {
        return FAIL;
    }
    if(!(start->me = malloc(sizeof(entity)))){
        return FAIL;
    }
    
    entity *me = start->me;
    me->posy = map->inity;
    me->posx = map->initx;
    me->character = tank;
    return SUCCESS;
}
void closeEntityList(clnode *start)
{
    for(cliterator i= clgetIter(start);i.now!=NULL;clnext(&i)){
        free(i.now->me);
    }
    clfree(start);
}

void printMap(WINDOW *win, Map *map)
{
    wchar_t *temp = malloc(sizeof(wchar_t) * (map->width + 2));
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
            temp[j] = map->map[i][j];
        temp[map->width] = L'\n';
        temp[map->width + 1] = L'\0';
        waddwstr(win, temp);
        wrefresh(win);
    }
    free(temp);
}


void spawnEnemy(Map *map, clnode *start, int counter)
{
    if (map->enemyRule.random == -1 || counter % map->enemyRule.random != 0)
    {
        return;
    }

    if (map->enemyRule.max == -1 || cllength(start) - 1 >= map->enemyRule.max)
    {
        return;
    }

    int **available = malloc(sizeof(int *) * (map->height * map->width));
    int avaiCount = 0;
    if (!(avaiCount = getEmptyPos(map, available)))
    {
        return;
    }

    int *p = available[random() % avaiCount];

    clnode *new = malloc(sizeof(clnode));
    new->me = malloc(sizeof(entity));
    entity *me = new->me;
    if (new)
    {
        *(entity*)new->me = (entity){.character = L'Ｅ', .posy = p[0], .posx = p[1]};
        clappend(start, new);
        map->map[me->posy][me->posx] = me->character;
    }

    for (int i = 0; i < avaiCount; i++)
    {
        free(available[i]);
    }
    free(available);
}
