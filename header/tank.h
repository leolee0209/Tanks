#ifndef TANK_H
#define TANK_H
#include "map.h"
#include "circleList.h"

void putTank(Map* map, struct entity* tank, int ay, int ax);
void moveTank(Map* map, struct entity *tank, char move);

#endif