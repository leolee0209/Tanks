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
int *nextPos(Map *map, entity *e)
{
    int *next = malloc(sizeof(int) * 2);
    next[0] = e->posy;
    next[1] = e->posx;
    switch (e->direction)
    {
    case 'w':
        next[0]--;
        break;
    case 's':
        next[0]++;
        break;
    case 'a':
        next[1]--;
        break;
    case 'd':
        next[1]++;
        break;

    default:
        return NULL;
    }
    return next;
}
int getFilePaths(const char *dirPath, char **mapFileName, char **mapInfoFileName)
{

    if (!(*mapInfoFileName = calloc(strlen(dirPath) + strlen(mapinfojson) + 1, sizeof(char))))
    {
        return FAIL;
    }
    strcat(*mapInfoFileName, dirPath);
    strcat(*mapInfoFileName, mapinfojson);

    if (!(*mapFileName = calloc(strlen(dirPath) + strlen(maptxt) + 1, sizeof(char))))
    {
        return FAIL;
    }
    strcat(*mapFileName, dirPath);
    strcat(*mapFileName, maptxt);

    return SUCCESS;
}

int loadMapInfo(const char *mapInfoFileName, Map *map)
{
    FILE *mapInfoFile = fopen(mapInfoFileName, "r");
    if (!mapInfoFile || !map)
        return FAIL;

    char str[1024];
    char rule[256];
    int value;

    int len = fread(str, sizeof(char), 1024, mapInfoFile);
    if (!feof(mapInfoFile) && ferror(mapInfoFile))
    {
        fclose(mapInfoFile);
        return FAIL;
    }
    cJSON *mapInfoJson = cJSON_ParseWithLength(str, len);
    if (mapInfoJson == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
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
        mbstowcs(&map->meRule.character, mecharacter->valuestring, 1);
    else
        map->meRule.character = defaulttank;
    cJSON *mebcharacter = cJSON_GetObjectItemCaseSensitive(me, "bulletcharacter");
    if (cJSON_IsString(mebcharacter))
        mbstowcs(&map->meRule.bulletcharacter, mebcharacter->valuestring, 1);
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
    cJSON *echaracter = cJSON_GetObjectItemCaseSensitive(enemy, "character");
    if (cJSON_IsString(echaracter))
        mbstowcs(&map->enemyRule.character, echaracter->valuestring, 1);
    else
        map->enemyRule.character = defaultenemy;
    cJSON *ebcharacter = cJSON_GetObjectItemCaseSensitive(enemy, "bulletcharacter");
    if (cJSON_IsString(ebcharacter))
        mbstowcs(&map->enemyRule.bulletcharacter, ebcharacter->valuestring, 1);
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

int checkEmpty(Map *map, int y, int x)
{
    if (checkInBound(map, y, x))
    {
        return map->map[y][x] == map->air;
    }
    return FAIL;
}

int checkInBound(Map *map, int y, int x)
{
    if (y >= 0 && y < map->height && x >= 0 && x < map->width)
    {
        return SUCCESS;
    }
    return FAIL;
}
