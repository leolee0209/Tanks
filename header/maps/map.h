#ifndef MAP_H
#define MAP_H

#include <ncurses.h>
#include "circleList.h"
#include "mapUtility.h"

int initMap(Map* map, char* dir);
void closeMap(Map *map);
void closeEntityList(cllist *start);
void printMap(WINDOW* win, Map *map);
void spawnEnemy(Map *map, cllist *enemies, int counter);
void spawnBullet(Map *map,entity* me, cllist *enemies,cllist* bullets, int counter);

#endif
