#include <mapUtility.h>
#include <stdlib.h>
#include <ncurses.h>
#include "characters.h"
#include <string.h>
#include "cJSON.h"

int loadMapFile(const char *mapFileName, Map *map);
int loadEnemyFile(const char *enemyFileName, Map *map);
int allocMap(Map *map, int h, int w);

int allocMap(Map *map, int h, int w)
{
    if (!map)
        return FAIL;

    if (!(map->map = malloc(h * sizeof(char *))))
    {
        return FAIL;
    }
    for (int i = 0; i < h; i++)
    {
        if (!(map->map[i] = malloc(w * sizeof(char))))
        {
            return FAIL;
        }
    }
    return SUCCESS;
}
int getMapFromFile(const char *fileName, Map *map)
{
    char *mapFileName, *enemyFileName;
    if (!(mapFileName = calloc(strlen(fileName) + strlen(maptxt) + 1, sizeof(char))))
        return FAIL;
    strcat(mapFileName, fileName);
    strcat(mapFileName, maptxt);

    if (!(enemyFileName = calloc(strlen(fileName) + strlen(enemyjson) + 1, sizeof(char))))
    {
        free(mapFileName);
        return FAIL;
    }
    strcat(enemyFileName, fileName);
    strcat(enemyFileName, enemyjson);

    if (!loadMapFile(mapFileName, map))
    {
        free(mapFileName);
        free(enemyFileName);
        return FAIL;
    }
    if (!loadEnemyFile(enemyFileName, map))
    {
        free(mapFileName);
        free(enemyFileName);
        return FAIL;
    }

    free(mapFileName);
    free(enemyFileName);
    return SUCCESS;
}

int loadEnemyFile(const char *enemyFileName, Map *map)
{
    FILE *enemyFile = fopen(enemyFileName, "r");
    if (!enemyFile || !map)
        return FAIL;

    map->enemyRule.random = -1;
    

    char str[1024];
    char rule[256];
    int value;

    int len = fread(str, sizeof(char), 1024, enemyFile);
    if(!feof(enemyFile)&&ferror(enemyFile)){
        fclose(enemyFile);
        return FAIL;
    }
    cJSON *enemyJson = cJSON_ParseWithLength(str, len);
    if (enemyJson == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(enemyJson);
        return FAIL;
    }

    cJSON *max = cJSON_GetObjectItemCaseSensitive(enemyJson, "max");
    map->enemyRule.max = cJSON_IsNumber(max) ? max->valueint : -1;

    cJSON *random = cJSON_GetObjectItemCaseSensitive(enemyJson, "random");
    map->enemyRule.random = cJSON_IsNumber(random) ? random->valueint : -1;

    cJSON_Delete(enemyJson);
    fclose(enemyFile);
    return SUCCESS;
}

int loadMapFile(const char *mapFileName, Map *map)
{
    FILE *mapFile = fopen(mapFileName, "r");
    if (!mapFile || !map)
        return FAIL;

    int h, w;
    char str[256];
    if (!fgets(str, 256, mapFile))
        return FAIL;
    sscanf(str, "%d,%d\n", &h, &w);
    if (h <= 0 || w <= 0)
        return FAIL;
    map->height = h;
    map->width = w;
    if (!allocMap(map, h, w))
    {
        return FAIL;
    }

    char *temp = malloc(sizeof(char) * w);

    for (int i = 0; i < map->height; i++)
    {
        fgets(temp, w + 2, mapFile);

        for (int j = 0; j < w && temp[j] != '\n'; j++)
        {
            if (temp[j] == tank)
            {
                map->inity = i;
                map->initx = j;
            }
            map->map[i][j] = temp[j];
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
