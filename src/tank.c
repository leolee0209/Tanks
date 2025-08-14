#include "tank.h"
#include "map.h"
#include "characters.h"

void putTank(Map *map, struct entity *tank, int ay, int ax)
{
    map->map[tank->posy][tank->posx] = air;
    tank->posy += ay;
    tank->posx += ax;
    map->map[tank->posy][tank->posx] = tank->character;
}

void moveTank(Map* map, struct entity *tank, char move)
{
    int x = tank->posx, y = tank->posy;
    switch (move)
    {
    case 'w':
        if (checkNoWall(map, y - 1, x))
            putTank(map, tank, -1, 0);
        break;
    case 'a':
        if (checkNoWall(map, y, x - 1))
            putTank(map, tank, 0, -1);
        break;
    case 's':
        if (checkNoWall(map, y + 1, x))
            putTank(map, tank, +1, 0);
        break;
    case 'd':
        if (checkNoWall(map, y, x + 1))
            putTank(map, tank, 0, 1);
        break;
    default:
        putTank(map, tank, 0, 0);
    }
}
