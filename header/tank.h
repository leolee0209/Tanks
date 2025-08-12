#ifndef TANK_H
#define TANK_H
#include <wchar.h>

#include "circleList.h"

void putTank(char** map, struct entity* tank);
void moveTank(char** map, struct entity *tank, char move);

#endif