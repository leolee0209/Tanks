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

void closeEntityList(cllist *l)
{
    for (cliterator i = clgetIter(l); i.now != NULL; clnext(&i))
    {
        free(i.now->me);
    }
    clfree(l);
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

void spawnEnemy(Map *map, cllist *enemies, int counter)
{
    if (map->enemyRule.random == -1 || counter % map->enemyRule.random != 0)
    {
        return;
    }

    if (map->enemyRule.max == -1 || cllength(enemies) - 1 >= map->enemyRule.max)
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
        *(entity *)new->me = (entity){.character = map->enemyRule.character, .posy = p[0], .posx = p[1]};
        clappend(enemies, new);
        map->map[me->posy][me->posx] = me->character;
    }

    for (int i = 0; i < avaiCount; i++)
    {
        free(available[i]);
    }
    free(available);
}

void spawnBullet(Map *map, entity *me, cllist *start, cllist *bullets, int counter)
{
    if (map->meRule.firerate == -1 || counter % map->meRule.firerate != 0)
    {
        return;
    }
    int *n = nextPos(map, me);
    if(!n)
        return;
    if (checkEmpty(map, n[0], n[1]))
    {
        if (bullets == NULL && !(bullets = clinit()))
        {
            return;
        }
        entity *newBullet = malloc(sizeof(entity));
        *newBullet = (entity){.character = map->meRule.bulletcharacter, .direction = me->direction, .posy = n[0], .posx = n[1]};

        clappend(bullets, newnode(newBullet));
        map->map[n[0]][n[1]] = map->meRule.bulletcharacter;
    }
    free(n);
}
