#ifndef TANK_H
#define TANK_H
#include "map.h"
#include "characters.h"
#include <wchar.h>

void putTank(Map *map, pos *p, int ay, int ax, wchar_t character);
void moveTank(Map *map, pos *p, char move, wchar_t character);
void moveEnemy(Map *map, Earray *enemies, Barray *bullets, int count);
char getAiDirection(Map *map, entity *n);
void moveMe(Map *map, entity *me, int count);
int moveBullets(Map *map, Earray *enemies, Barray *bullets, entity *me, int count);

#endif