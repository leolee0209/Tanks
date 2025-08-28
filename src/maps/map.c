#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif
#include "map.h"
#include <ncurses.h>
#include <stdlib.h>
#include "tank.h"
#include "characters.h"

int initMap(Map *map, char *dir)
{
    char *mapPath, *mapInfoPath;
    if (!getFilePaths(dir, &mapPath, &mapInfoPath))
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

void printMap(WINDOW *win, Map *map)
{
    wclear(win);
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

void spawnEnemy(Map *map, Earray *enemies, int counter)
{
    if (map->enemyRule.random == -1 || counter % map->enemyRule.random != 0)
    {
        return;
    }

    if (map->enemyRule.max == -1 || enemies->last + 1 >= map->enemyRule.max || enemies->last >= (int)enemies->max - 1)
    {
        return;
    }

    int **available = malloc(sizeof(int *) * (map->height * map->width));
    int avaiCount = 0;
    if (!(avaiCount = getEmptyPos(map, available)))
    {
        // no available
        free(available);
        return;
    }

    // randomly spawn enemy
    int *p = available[random() % avaiCount];
    enemies->e[enemies->last + 1] = (entity){0};
    enemies->e[enemies->last + 1] = (entity){.character = map->enemyRule.character,
                                             .direction = 'n',
                                             .count = counter,
                                             .p = (pos){p[0], p[1]}};

    enemies->last++;
    map->map[enemies->e[enemies->last].p.y][enemies->e[enemies->last].p.x] = enemies->e[enemies->last].character;

    for (int i = 0; i < avaiCount; i++)
    {
        free(available[i]);
    }
    free(available);
}

int spawnBullet(Map *map, entity *me, Earray *enemies, Barray *bullets, int counter)
{
    spawnMyBullets(map, me, enemies, bullets, counter);
    return spawnEnemyBullets(map, me, enemies, bullets, counter);
}
