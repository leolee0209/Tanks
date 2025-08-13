#include "map.h"
#include <ncurses.h>
#include <stdlib.h>
#include "circleList.h"
#include "tank.h"
#include "characters.h"

int mapHeight = 8;
int mapWidth = 16;
const int initPosx = 7;
const int initPosy = 4;


void initMap(char ***map)
{
    getMapFromFile("/home/leo/Projects/Tanks/data/map1.txt", map);
}
void allocMap(int h, int w, char ***map)
{
    if (!map)
        return;
    if (*map)
    {
        if (**map)
        {
            closeMap(map);
        }
        else
        {
            free(*map);
        }
    }
    (*map) = malloc(h * sizeof(char *));
    for (int i = 0; i < h; i++)
    {
        (*map)[i] = malloc(w * sizeof(char));
    }
}
int getMapFromFile(const char *fileName, char ***map)
{
    FILE *f = fopen(fileName, "r");
    if (!f || !map)
        return -1;

    int h, w;
    char str[256];
    if (!fgets(str, 256, f))
        return -1;
    sscanf(str, "%d,%d\n", &h, &w);
    if (h <= 0 || w <= 0)
        return -1;
    mapHeight = h;
    mapWidth = w;
    allocMap(h, w, map);

    char *temp = malloc(sizeof(char) * w);

    for (int i = 0; i < mapHeight; i++)
    {
        fgets(temp, w + 2, f);

        for (int j = 0; j < w && temp[j] != '\n'; j++)
        {
            (*map)[i][j] = temp[j];
        }
    }

    fclose(f);
    free(temp);
    return 0;
}
struct node *initEntityList()
{
    struct node *start = malloc(sizeof(struct node));
    start->me.posy = initPosy;
    start->me.posx = initPosx;
    start->me.character = tank;
    start->after = start;
    start->before = start;
    return start;
}
void closeMap(char ***map)
{
    for (int i = 0; i < mapHeight; i++)
    {
        free((*map)[i]);
    }
    free(*map);
}
void closeEntities(struct node *start)
{
    if (start->after == start)
    {
        free(start);
        return;
    }
    struct node *n = start->after;
    while (1)
    {
        if (n->after == start)
        {
            free(n);
            free(start);
            return;
        }
        n = n->after;
        free(n->before);
    }
}
void printMap(WINDOW *win, char ***map, struct node *start)
{
    // clearMap(map);
    // makeWall(map);
    //makeEntities(map, start);

    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            wprintw(win, "%c", (*map)[i][j]);
        }
        wprintw(win, "\n");
    }
}
void makeWall(char ***map)
{
    for (int j = 0; j < mapHeight; j++)
    {
        (*map)[j][0] = wall;
    }
    for (int j = 0; j < mapHeight; j++)
    {
        (*map)[j][mapWidth - 1] = wall;
    }

    for (int i = 1; i < mapWidth - 1; i++)
    {
        (*map)[0][i] = wall;
        (*map)[mapHeight - 1][i] = wall;
    }
}
void clearMap(char ***map)
{
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            (*map)[i][j] = ' ';
        }
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
        //putTank(map, &n->me);
        if (n->after == start)
        {
            break;
        }
        n = n->after;
    }
}

int checkNoWall(char ***map, int y, int x)
{
    if (checkInBound(y, x))
    {
        return (*map)[y][x] != wall;
    }
    return 0;
}

int checkInBound(int y, int x)
{
    if (y >= 0 && y < mapHeight && x >= 0 && x < mapWidth)
    {
        return 1;
    }
    return 0;
}
