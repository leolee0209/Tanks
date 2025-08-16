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
    return getMapFromFile("/home/leo/Projects/Tanks/data/box/", map);
}
void closeMap(Map *map)
{
    for (int i = 0; i < map->height; i++)
    {
        free(map->map[i]);
    }
    free(map->map);
}

int initEntityList(Map *map, struct node *start)
{
    if (!start)
    {
        return FAIL;
    }
    start->me.posy = map->inity;
    start->me.posx = map->initx;
    start->me.character = tank;
    start->after = start;
    start->before = start;
    return SUCCESS;
}
void closeEntityList(struct node *start)
{
    if (start->after == start)
    {
        return;
    }
    struct node *n = start->after;
    while (1)
    {
        if (n->after == start)
        {
            free(n);
            return;
        }
        n = n->after;
        free(n->before);
    }
}

void printMap(WINDOW *win, Map *map)
{
    wchar_t *temp = malloc(sizeof(wchar_t) * (map->width+2));
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width;j++)
            temp[j] = map->map[i][j];
        temp[map->width] = L'\n';
        temp[map->width+1] = L'\0';
        waddwstr(win, temp);
        wrefresh(win);
    }
    free(temp);
}

void spawnEnemy(Map *map, node *start, int counter)
{
    if (counter % map->enemyRule.random != 0)
    {
        return;
    }

    if (map->enemyRule.max != -1 && length(start) - 1 >= map->enemyRule.max)
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

    node *new = malloc(sizeof(node));
    if (new)
    {
        new->me = (struct entity){.character = L'Ｅ', .posy = p[0], .posx = p[1]};
        append(start, new);
        map->map[new->me.posy][new->me.posx] = new->me.character;
    }

    for (int i = 0; i < avaiCount; i++)
    {
        free(available[i]);
    }
    free(available);
}
