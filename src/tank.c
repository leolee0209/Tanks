#include "tank.h"
#include "map.h"
#include <wchar.h>


void putTank(char***map, struct entity *tank)
{
    (*map)[tank->posy][tank->posx] = tank->character;
}

void moveTank(char***map, struct entity *tank, char move)
{
    int x = tank->posx, y = tank->posy;
    switch (move)
    {
    case 'w':
        if (checkNoWall(map, y - 1, x))
            tank->posy--;
        break;
    case 'a':
        if (checkNoWall(map, y, x - 1))
            tank->posx--;
        break;
    case 's':
        if (checkNoWall(map, y + 1, x))
            tank->posy++;
        break;
    case 'd':
        if (checkNoWall(map, y, x + 1))
            tank->posx++;
        break;
    }
}
