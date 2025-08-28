#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif
#include <mapUtility.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "cJSON.h"
#include <stdio.h>
#include <wchar.h>
#include "log.h"

void spawnMyBullets(Map *map, entity *me, Earray *enemies, Barray *bullets, int counter)
{
    if (map->meRule.firerate == -1 || counter % map->meRule.firerate != 0)
    {
        return;
    }
    pos *n = nextPos(map, me->p, me->direction);
    if (!n)
        return;
    if (inBound(map, *n) && !isWall(map, *n))
    {
        if (isEnemy(map, *n))
        {
            entity *tmpE = NULL;
            for (int i = 0; i <= enemies->last && i < (int)enemies->max; i++)
            {
                // remove enemy
                tmpE = &(enemies->e[i]);
                if (tmpE->p.y == n->y && tmpE->p.x == n->x)
                {
                    map->map[n->y][n->x] = map->air;
                    enemies->e[i] = enemies->e[enemies->last];
                    enemies->last--;
                    break;
                }
            }
        }
        else if (isAir(map, *n) && bullets->last < (int)bullets->max - 1)
        {
            // add new bullet
            bullets->e[bullets->last + 1] = (Bentity){0};
            bullets->e[bullets->last + 1] = (Bentity){.character = map->meRule.bulletcharacter,
                                                      .direction = me->direction,
                                                      .count = counter,
                                                      .p = *n,
                                                      .o = PLAYER};
            bullets->last++;
            map->map[n->y][n->x] = map->meRule.bulletcharacter;
        }
    }
    free(n);
}
int spawnEnemyBullets(Map *map, entity *me, Earray *enemies, Barray *bullets, int counter)
{
    int hit = 0;
    for (int i = 0; i <= enemies->last && i < (int)enemies->max; i++)
    {
        entity *e = &(enemies->e[i]);
        if (map->enemyRule.firerate == -1 || (counter - e->count) % map->enemyRule.firerate != 0)
        {
            continue;
        }
        pos *n = nextPos(map, e->p, e->direction);
        if (!n)
            continue;
        if (inBound(map, *n) && !isWall(map, *n))
        {
            if (isEnemy(map, *n))
            {
                // remove enemy
                map->map[n->y][n->x] = map->air;
                enemies->e[i] = enemies->e[enemies->last];
                enemies->last--;
                i--;
            }
            else if (n->y == me->p.y && n->x == me->p.x)
            {
                // hit player
                hit = 1;
            }
            else if (isAir(map, *n) && bullets->last < (int)bullets->max - 1)
            {
                // add new bullet
                bullets->e[bullets->last + 1] = (Bentity){0};
                bullets->e[bullets->last + 1] = (Bentity){.character = map->enemyRule.bulletcharacter,
                                                          .direction = e->direction,
                                                          .count = counter,
                                                          .p = *n,
                                                          .o = ENEMY};
                bullets->last++;
                map->map[n->y][n->x] = map->enemyRule.bulletcharacter;
            }
        }
        free(n);
    }
    return hit;
}
int allocMap(Map *map, int h, int w)
{
    if (!map)
        return FAIL;

    if (!(map->map = malloc(h * sizeof(wchar_t *))))
    {
        return FAIL;
    }
    for (int i = 0; i < h; i++)
    {
        if (!(map->map[i] = malloc(w * sizeof(wchar_t))))
        {
            return FAIL;
        }
    }
    return SUCCESS;
}
pos *nextPos(Map *map, pos p, char direction)
{
    pos *next = malloc(sizeof(pos));
    next->y = p.y;
    next->x = p.x;
    switch (direction)
    {
    case 'w':
        next->y--;
        break;
    case 's':
        next->y++;
        break;
    case 'a':
        next->x--;
        break;
    case 'd':
        next->x++;
        break;

    default:
        free(next);
        return NULL;
    }
    return next;
}
int isEnemy(Map *map, pos p)
{
    return map->map[p.y][p.x] == map->enemyRule.character;
}
int isWall(Map *map, pos p)
{
    return map->map[p.y][p.x] == map->wall;
}

int getFilePaths(const char *dirPath, char **mapFileName, char **mapInfoFileName)
{
    int len = strlen(dirPath) + 12;
    char *pathToMapFolder = malloc(sizeof(char) * len);
    if (!pathToMapFolder)
    {
        return FAIL;
    }
    strncpy(pathToMapFolder, dirPath, len);
    strncat(pathToMapFolder, "/data/box/", 11);
    if (!(*mapInfoFileName = calloc(len + strlen(mapinfojson) + 1, sizeof(char))))
    {
        logc("getFilePaths", "calloc failed.\n");
        free(pathToMapFolder);
        return FAIL;
    }
    strcat(*mapInfoFileName, pathToMapFolder);
    strcat(*mapInfoFileName, mapinfojson);

    if (!(*mapFileName = calloc(len + strlen(maptxt) + 1, sizeof(char))))
    {
        logc("getFilePaths", "calloc failed.\n");
        free(pathToMapFolder);
        return FAIL;
    }
    strcat(*mapFileName, pathToMapFolder);
    strcat(*mapFileName, maptxt);
    free(pathToMapFolder);

    return SUCCESS;
}

int loadMapInfo(const char *mapInfoFileName, Map *map)
{
    FILE *mapInfoFile = fopen(mapInfoFileName, "r");
    if (!mapInfoFile || !map)
    {
        logc("loadMapInfo", "FILEIO.\n");
        return FAIL;
    }

    char str[1024];
    char rule[256];
    wchar_t tmpchar[1] = {0};
    int value;

    int len = fread(str, sizeof(char), 1024, mapInfoFile);
    if (!feof(mapInfoFile) && ferror(mapInfoFile))
    {
        logc("loadMapInfo", "FILEIO.\n");
        fclose(mapInfoFile);
        return FAIL;
    }
    cJSON *mapInfoJson = cJSON_ParseWithLength(str, len);
    if (mapInfoJson == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            logc("loadMapInfo", error_ptr);
        }
        cJSON_Delete(mapInfoJson);
        return FAIL;
    }
    // me
    cJSON *me = cJSON_GetObjectItemCaseSensitive(mapInfoJson, "me");
    cJSON *mespeed = cJSON_GetObjectItemCaseSensitive(me, "speed");
    map->meRule.speed = cJSON_IsNumber(mespeed) && mespeed->valueint != 0 ? mespeed->valueint : -1;
    cJSON *mebulletspeed = cJSON_GetObjectItemCaseSensitive(me, "bulletspeed");
    map->meRule.bulletspeed = cJSON_IsNumber(mebulletspeed) && mebulletspeed->valueint != 0 ? mebulletspeed->valueint : -1;
    cJSON *firerate = cJSON_GetObjectItemCaseSensitive(me, "firerate");
    map->meRule.firerate = cJSON_IsNumber(firerate) && firerate->valueint != 0 ? firerate->valueint : -1;
    cJSON *mecharacter = cJSON_GetObjectItemCaseSensitive(me, "character");
    if (cJSON_IsString(mecharacter))
    {
        mbstowcs(tmpchar, mecharacter->valuestring, 1);
        map->meRule.character = tmpchar[0];
    }
    else
        map->meRule.character = defaulttank;
    cJSON *mebcharacter = cJSON_GetObjectItemCaseSensitive(me, "bulletcharacter");
    if (cJSON_IsString(mebcharacter))
    {
        mbstowcs(tmpchar, mebcharacter->valuestring, 1);
        map->meRule.bulletcharacter = tmpchar[0];
    }
    else
        map->meRule.bulletcharacter = defaultbullet;

    // enemy
    cJSON *enemy = cJSON_GetObjectItemCaseSensitive(mapInfoJson, "enemy");
    cJSON *enemyspeed = cJSON_GetObjectItemCaseSensitive(enemy, "speed");
    map->enemyRule.speed = cJSON_IsNumber(enemyspeed) && enemyspeed->valueint != 0 ? enemyspeed->valueint : -1;
    cJSON *max = cJSON_GetObjectItemCaseSensitive(enemy, "max");
    map->enemyRule.max = cJSON_IsNumber(max) ? max->valueint : -1;
    cJSON *enemyBulletSpeed = cJSON_GetObjectItemCaseSensitive(enemy, "bulletspeed");
    map->enemyRule.bulletspeed = cJSON_IsNumber(enemyBulletSpeed) && enemyBulletSpeed->valueint != 0 ? enemyBulletSpeed->valueint : -1;
    cJSON *random = cJSON_GetObjectItemCaseSensitive(enemy, "random");
    map->enemyRule.random = cJSON_IsNumber(random) && random->valueint != 0 ? random->valueint : -1;
    cJSON *efirerate = cJSON_GetObjectItemCaseSensitive(enemy, "firerate");
    map->enemyRule.firerate = cJSON_IsNumber(efirerate) && efirerate->valueint != 0 ? efirerate->valueint : -1;
    cJSON *echaracter = cJSON_GetObjectItemCaseSensitive(enemy, "character");
    if (cJSON_IsString(echaracter))
    {
        mbstowcs(tmpchar, echaracter->valuestring, 1);
        map->enemyRule.character = tmpchar[0];
    }
    else
        map->enemyRule.character = defaultenemy;
    cJSON *ebcharacter = cJSON_GetObjectItemCaseSensitive(enemy, "bulletcharacter");
    if (cJSON_IsString(ebcharacter))
    {
        mbstowcs(tmpchar, ebcharacter->valuestring, 1);
        map->enemyRule.bulletcharacter = tmpchar[0];
    }
    else
        map->enemyRule.bulletcharacter = defaultbullet;
    // map
    cJSON *mapinfo = cJSON_GetObjectItemCaseSensitive(mapInfoJson, "map");
    cJSON *height = cJSON_GetObjectItemCaseSensitive(mapinfo, "height");
    if (cJSON_IsNumber(height))
        map->height = height->valueint;
    else
        return FAIL;
    cJSON *width = cJSON_GetObjectItemCaseSensitive(mapinfo, "width");
    if (cJSON_IsNumber(width))
        map->width = width->valueint;
    else
        return FAIL;
    cJSON *frameMicroSec = cJSON_GetObjectItemCaseSensitive(mapinfo, "frameMicroSec");
    if (cJSON_IsNumber(frameMicroSec))
        map->frameMicroSec = frameMicroSec->valueint;
    else
        map->frameMicroSec = 50000;
    cJSON *air = cJSON_GetObjectItemCaseSensitive(mapinfo, "air");
    if (cJSON_IsString(air))
        mbstowcs(&map->air, air->valuestring, 1);
    else
        map->air = defaultair;
    cJSON *wall = cJSON_GetObjectItemCaseSensitive(mapinfo, "wall");
    if (cJSON_IsString(wall))
        mbstowcs(&map->wall, wall->valuestring, 1);
    else
        map->wall = defaultwall;

    cJSON_Delete(mapInfoJson);
    fclose(mapInfoFile);
    return SUCCESS;
}

int loadMap(const char *mapFileName, Map *map)
{
    FILE *mapFile = fopen(mapFileName, "r");
    if (!mapFile || !map)
        return FAIL;

    if (!allocMap(map, map->height, map->width))
    {
        return FAIL;
    }

    wchar_t *temp = malloc(sizeof(wchar_t) * map->width);

    for (int i = 0; i < map->height; i++)
    {
        if (fgetws(temp, map->width + 2, mapFile) == NULL)
        {
            int a = ferror(mapFile);
            continue;
        }
        wcsncpy(map->map[i], temp, map->width);
        for (int j = 0; j < map->width; j++)
        {
            if (map->map[i][j] == map->meRule.character)
            {
                map->inity = i;
                map->initx = j;
            }
        }
    }

    fclose(mapFile);
    free(temp);
    return SUCCESS;
}

int isEmpty(Map *map, pos p)
{
    return inBound(map, p) && isAir(map, p);
}

int isBullet(Map *map, pos p)
{
    return map->map[p.y][p.x] == map->meRule.bulletcharacter || map->map[p.y][p.x] == map->enemyRule.bulletcharacter;
}

int getEmptyPos(Map *map, int **available)
{
    if (!available)
    {
        return FAIL;
    }
    int avaiCount = 0;
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            if (map->map[i][j] == map->air)
            {
                available[avaiCount] = malloc(sizeof(int) * 2);
                if (available[avaiCount])
                {
                    available[avaiCount][0] = i;
                    available[avaiCount][1] = j;
                    avaiCount++;
                }
            }
        }
    }
    return avaiCount;
}

int isAir(Map *map, pos p)
{
    return map->map[p.y][p.x] == map->air;
}

int inBound(Map *map, pos p)
{
    if (p.y >= 0 && p.y < map->height && p.x >= 0 && p.x < map->width)
    {
        return SUCCESS;
    }
    return FAIL;
}
