#ifndef MAP_H
#define MAP_H

#include <ncurses.h>
#include "circleList.h"
#include "mapUtility.h"


int initMap(Map* map);
void closeMap(Map *map);

int initEntityList(Map* map, node* start);
void closeEntityList(node *start);

void printMap(WINDOW* win, Map *map);
void spawnEnemy(Map *map, node *start, int counter);


#endif
