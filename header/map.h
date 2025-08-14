#ifndef MAP_H
#define MAP_H

#include <ncurses.h>

typedef struct Map
{
    int height, width;
    int inity, initx;
    char **map;
} Map;
struct node;
int initMap(Map* map);
int getMapFromFile(const char *fileName, Map *map);
int initEntityList(Map* map, struct node* start);
void closeMap(Map *map);
void closeEntities(struct node *start);
void printMap(WINDOW* win, Map *map);
void makeEntities(char***map, struct node *start);
int checkNoWall(Map *map, int y, int x);
int checkInBound(Map *map, int y, int x);
int allocMap(Map *map, int h, int w);

#endif
