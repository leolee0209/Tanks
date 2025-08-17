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

void moveEnemy(Map *map, cllist *enemies, int count)
{
    if (enemies->start == NULL)
        return;
    if (map->enemyRule.speed == -1 || count % map->enemyRule.speed != 0)
        return;
    for (cliterator i = clgetIter(enemies); i.now != NULL; clnext(&i))
    {
        getAiDirection(map, (entity *)(i.now->me));
        moveTank(map, (entity *)(i.now->me), ((entity *)(i.now->me))->direction);
    }
}
void moveMe(Map *map, entity *me, int count)
{
    if (map->meRule.speed == -1 || count % map->meRule.speed != 0)
        return;
    moveTank(map, me, me->direction);
}

void moveBullets(Map *map, cllist *bullets, int count)
{
    if (bullets->start == NULL)
        return;
    if (map->meRule.bulletspeed == -1 || count % map->meRule.bulletspeed != 0)
        return;

    entity *e = NULL;
    cllist *toRemove = clinit();
    int *n = NULL;
    for (cliterator i = clgetIter(bullets); i.now != NULL; clnext(&i))
    {
        e = (entity *)(i.now->me);
        if ((n = nextPos(map, e)) && !checkEmpty(map, n[0], n[1]))
        {
            clappend(toRemove, newnode(i.now));
            continue;
        }
        moveTank(map, e, e->direction);
    }
    free(n);

    clnode *nodeToR = NULL;
    entity *entityToR = NULL;
    for (cliterator i = clgetIter(toRemove); i.now != NULL; clnext(&i))
    {
        nodeToR = (clnode *)(i.now->me);
        entityToR = (entity *)(nodeToR->me);
        map->map[entityToR->posy][entityToR->posx] = air;
        clremove(bullets, nodeToR);
        free(nodeToR->me);
        free(nodeToR);
    }
    clfree(toRemove);
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
