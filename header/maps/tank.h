#ifndef TANK_H
#define TANK_H
#include "map.h"
#include "circleList.h"

void putTank(Map *map, node *tank, int ay, int ax);
void moveTank(Map *map, node *tank, char move);
void moveEnemy(Map *map, node *start);
char getAiDirection(Map *map, node *n);

#endif