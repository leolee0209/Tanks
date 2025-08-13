#ifndef TANK_H
#define TANK_H
#include <wchar.h>

#include "circleList.h"

void putTank(char*** map, struct entity* tank, int ay, int ax);
void moveTank(char*** map, struct entity *tank, char move);

#endif