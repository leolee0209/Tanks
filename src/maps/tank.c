#include "tank.h"
#include "map.h"
#include "characters.h"
#include <stdlib.h>

void putTank(Map *map, entity *tank, int ay, int ax)
{
    if (!(ay == 0 && ax == 0))
    {
        map->map[tank->posy][tank->posx] = air;
        tank->posy += ay;
        tank->posx += ax;
    }
    map->map[tank->posy][tank->posx] = tank->character;
}

void moveTank(Map *map, entity *tank, char move)
{
    int x = tank->posx, y = tank->posy;
    switch (move)
    {
    case 'w':
        if (checkEmpty(map, y - 1, x))
            putTank(map, tank, -1, 0);
        break;
    case 'a':
        if (checkEmpty(map, y, x - 1))
            putTank(map, tank, 0, -1);
        break;
    case 's':
        if (checkEmpty(map, y + 1, x))
            putTank(map, tank, +1, 0);
        break;
    case 'd':
        if (checkEmpty(map, y, x + 1))
            putTank(map, tank, 0, 1);
        break;
    }
}

void moveEnemy(Map *map, clnode *start)
{
    for (cliterator i = clgetIter(start); i.now != NULL; clnext(&i))
    {
        if (i.now == start)
        {
            continue;
        }
        getAiDirection(map, (entity*)(i.now->me));
        moveTank(map, (entity*)(i.now->me), ((entity*)(i.now->me))->direction);
    }
}

char getAiDirection(Map *map, entity *n)
{
    int r = random() % 20;
    switch (r)
    {
    case 0:
        n->direction = 'w';
        return 'w';
        break;
    case 1:
        n->direction = 's';
        return 's';
        break;
    case 2:
        n->direction = 'a';
        return 'a';
        break;
    case 3:
        n->direction = 'd';
        return 'd';
        break;
    }
    return 'o';
}
