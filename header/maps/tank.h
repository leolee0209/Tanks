#ifndef TANK_H
#define TANK_H
#include "map.h"
#include "characters.h"
#include "circleList.h"

void putTank(Map *map, entity *tank, int ay, int ax);
void moveTank(Map *map, entity *tank, char move);
void moveEnemy(Map *map, cllist *enemies,cllist* bullets, int count);
char getAiDirection(Map *map, entity *n);
void moveMe(Map *map, entity *me, int count);
int moveBullets(Map *map,entity* me, cllist *bullets,cllist* enemies, int count);

#endif