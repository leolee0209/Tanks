#include "map.h"
#include <ncurses.h>
#include <stdlib.h>
#include "circleList.h"
#include "tank.h"

const int mapWidth = 16;
const int mapHeight = 8;
const int initPosx = 7;
const int initPosy = 4;
const char wall = '#';
const char tank = '@';

char **initMap()
{
    char **map = malloc(mapHeight * sizeof(char *));
    for (int i = 0; i < mapHeight; i++)
    {
        map[i] = malloc(mapWidth * sizeof(char));
    }

    return map;
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
void closeMap(char **map)
{
    for (int i = 0; i < mapHeight; i++)
    {
        free(map[i]);
    }
    free(map);
}
void closeEntities(struct node* start){
    if(start->after==start){
        free(start);
        return;
    }
    struct node *n = start->after;
    while(1){
        if(n->after==start){
            free(n);
            free(start);
            return;
        }
        n = n->after;
        free(n->before);
    }
}
void printMap(WINDOW* win, char **map, struct node *start)
{
    clearMap(map);
    makeWall(map);
    makeEntities(map, start);

    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            wprintw(win, "%c", map[i][j]);
        }
        wprintw(win, "\n");
    }
}
void makeWall(char **map)
{
    for (int j = 0; j < mapHeight; j++)
    {
        map[j][0] = wall;
    }
    for (int j = 0; j < mapHeight; j++)
    {
        map[j][mapWidth - 1] = wall;
    }

    for (int i = 1; i < mapWidth - 1; i++)
    {
        map[0][i] = wall;
        map[mapHeight - 1][i] = wall;
    }
}
void clearMap(char **map)
{
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            map[i][j] = ' ';
        }
    }
}

void makeEntities(char **map, struct node *start)
{
    if (!start)
    {
        return;
    }
    struct node *n = start;
    while (1)
    {
        putTank(map, &n->me);
        if (n->after == start)
        {
            break;
        }
        n = n->after;
    }
}

int checkNoWall(char **map, int y, int x)
{
    if (checkInBound(y, x))
    {
        return map[y][x] != wall;
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
