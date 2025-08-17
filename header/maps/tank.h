#ifndef TANK_H
#define TANK_H
#include "map.h"
#include "characters.h"
#include "circleList.h"

void putTank(Map *map, entity *tank, int ay, int ax);
void moveTank(Map *map, entity *tank, char move);
void moveEnemy(Map *map, clnode *start);
char getAiDirection(Map *map, entity *n);

#endif