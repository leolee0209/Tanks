#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif
#include <mapUtility.h>
#include <stdlib.h>
#include <ncurses.h>
#include "characters.h"
#include <string.h>
#include "cJSON.h"
#include <stdio.h>
#include <wchar.h>

int loadMapFile(const char *mapFileName, Map *map);
int loadMapInfoFile(const char *enemyFileName, Map *map);
int allocMap(Map *map, int h, int w);

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
int getMapFromFile(const char *fileName, Map *map)
{
    char *mapFileName, *mapInfoFileName;
    if (!(mapInfoFileName = calloc(strlen(fileName) + strlen(mapinfojson) + 1, sizeof(char))))
    {
        return FAIL;
    }
    strcat(mapInfoFileName, fileName);
    strcat(mapInfoFileName, mapinfojson);

    if (!(mapFileName = calloc(strlen(fileName) + strlen(maptxt) + 1, sizeof(char))))
    {
        free(mapInfoFileName);
        return FAIL;
    }
    strcat(mapFileName, fileName);
    strcat(mapFileName, maptxt);

    if (!loadMapInfoFile(mapInfoFileName, map))
    {
        free(mapFileName);
        free(mapInfoFileName);
        return FAIL;
    }
    if (!loadMapFile(mapFileName, map))
    {
        free(mapFileName);
        free(mapInfoFileName);
        return FAIL;
    }

    free(mapInfoFileName);
    free(mapFileName);
    return SUCCESS;
}

int loadMapInfoFile(const char *mapInfoFileName, Map *map)
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

    cJSON *enemy = cJSON_GetObjectItemCaseSensitive(mapInfoJson, "enemy");
    cJSON *max = cJSON_GetObjectItemCaseSensitive(enemy, "max");
    map->enemyRule.max = cJSON_IsNumber(max) ? max->valueint : -1;

    cJSON *random = cJSON_GetObjectItemCaseSensitive(enemy, "random");
    map->enemyRule.random = cJSON_IsNumber(random) ? random->valueint : -1;

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

    cJSON_Delete(mapInfoJson);
    fclose(mapInfoFile);
    return SUCCESS;
}

int loadMapFile(const char *mapFileName, Map *map)
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
            if (map->map[i][j] == tank)
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
            if (map->map[i][j] == air)
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
        return map->map[y][x] == air;
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
