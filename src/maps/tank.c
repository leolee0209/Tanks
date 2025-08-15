#include "tank.h"
#include "map.h"
#include "characters.h"
#include <stdlib.h>

void putTank(Map *map, node *tank, int ay, int ax)
{
    if (!(ay == 0 && ax == 0))
    {
        map->map[tank->me.posy][tank->me.posx] = air;
        tank->me.posy += ay;
        tank->me.posx += ax;
    }
    map->map[tank->me.posy][tank->me.posx] = tank->me.character;
}

void moveTank(Map *map, node *tank, char move)
{
    int x = tank->me.posx, y = tank->me.posy;
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

void moveEnemy(Map *map, node *start)
{
    for (iterator i = getIterator(start); i.now != NULL; next(&i))
    {
        if (i.now == start)
        {
            continue;
        }
        getAiDirection(map, i.now);
        moveTank(map, i.now, i.now->me.direction);
    }
}

char getAiDirection(Map *map, node *n)
{
    int r = random() % 20;
    switch (r)
    {
    case 0:
        n->me.direction = 'w';
        return 'w';
        break;
    case 1:
        n->me.direction = 's';
        return 's';
        break;
    case 2:
        n->me.direction = 'a';
        return 'a';
        break;
    case 3:
        n->me.direction = 'd';
        return 'd';
        break;
    }
    return 'o';
}
