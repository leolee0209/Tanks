#include "map.h"
#include <ncurses.h>
#include <stdlib.h>
#include "circleList.h"
#include "tank.h"
#include "characters.h"



int initMap(Map *map)
{
    return getMapFromFile("/home/leo/Projects/Tanks/data/map1.txt", map);
}
int allocMap(Map *map, int h, int w)
{
    if (!map)
        return FAIL;
    if (map->map)
    {
        if (*map->map)
        {
            closeMap(map);
        }
        else
        {
            free(map->map);
        }
    }
    map->map = malloc(h * sizeof(char *));
    if (!map->map)
    {
        return FAIL;
    }
    for (int i = 0; i < h; i++)
    {
        map->map[i] = malloc(w * sizeof(char));
        if (!map->map[i])
        {
            return FAIL;
        }
    }
    return SUCCESS;
}
int getMapFromFile(const char *fileName, Map *map)
{
    FILE *f = fopen(fileName, "r");
    if (!f || !map)
        return FAIL;

    int h, w;
    char str[256];
    if (!fgets(str, 256, f))
        return FAIL;
    sscanf(str, "%d,%d\n", &h, &w);
    if (h <= 0 || w <= 0)
        return FAIL;
    map->height = h;
    map->width = w;
    if(!allocMap(map, h, w)){
        return FAIL;
    }

    char *temp = malloc(sizeof(char) * w);

    for (int i = 0; i < map->height; i++)
    {
        fgets(temp, w + 2, f);

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

    fclose(f);
    free(temp);
    return SUCCESS;
}
int initEntityList(Map* map, struct node *start)
{
    if(!start){
        return FAIL;
    }
    start->me.posy = map->inity;
    start->me.posx = map->initx;
    start->me.character = tank;
    start->after = start;
    start->before = start;
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
void closeEntities(struct node *start)
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
void printMap(WINDOW *win, Map* map)
{
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            wprintw(win, "%c", map->map[i][j]);
        }
        wprintw(win, "\n");
    }
}

void makeEntities(char ***map, struct node *start)
{
    if (!start)
    {
        return;
    }
    struct node *n = start;
    while (1)
    {
        // putTank(map, &n->me);
        if (n->after == start)
        {
            break;
        }
        n = n->after;
    }
}

int checkNoWall(Map *map, int y, int x)
{
    if (checkInBound(map, y, x))
    {
        return map->map[y][x] != wall;
    }
    return FAIL;
}

int checkInBound(Map* map,int y, int x)
{
    if (y >= 0 && y < map->height && x >= 0 && x < map->width)
    {
        return SUCCESS;
    }
    return FAIL;
}
