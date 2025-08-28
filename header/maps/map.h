#ifndef MAP_H
#define MAP_H

#include <ncurses.h>
#include "mapUtility.h"

int initMap(Map* map, char* dir);
void closeMap(Map *map);
void printMap(WINDOW* win, Map *map);
void spawnEnemy(Map *map, Earray *enemies, int counter);
int spawnBullet(Map *map, entity *me, Earray *enemies, Barray *bullets, int counter);

#endif
