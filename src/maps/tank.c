#include "tank.h"
#include "map.h"
#include "characters.h"
#include <stdlib.h>

void putTank(Map *map, entity *tank, int ay, int ax)
{
    if (!(ay == 0 && ax == 0))
    {
        map->map[tank->posy][tank->posx] = map->air;
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
        if (isEmpty(map, y - 1, x))
            putTank(map, tank, -1, 0);
        break;
    case 'a':
        if (isEmpty(map, y, x - 1))
            putTank(map, tank, 0, -1);
        break;
    case 's':
        if (isEmpty(map, y + 1, x))
            putTank(map, tank, +1, 0);
        break;
    case 'd':
        if (isEmpty(map, y, x + 1))
            putTank(map, tank, 0, 1);
        break;
    }
}

void moveEnemy(Map *map, cllist *enemies, cllist *bullets, int count)
{
    if (enemies->start == NULL || map->enemyRule.speed == -1)
        return;

    cllist *toRemove = clinit();
    int *n = NULL;
    for (cliterator i = clgetIter(enemies); i.now != NULL; clnext(&i))
    {
        entity *e = i.now->me;
        if ((count - e->count) % map->enemyRule.speed != 0)
            continue;

        if (!(n = nextPos(map, e)))
        {
            getAiDirection(map, e);
        }
        else if (!inBound(map, n[0], n[1]))
        {
            clappend(toRemove, newnode(i.now));
        }
        else if (isAir(map, n[0], n[1]))
        {
            getAiDirection(map, e);
            moveTank(map, e, e->direction);
        }
        else if (isWall(map, n[0], n[1]) || isEnemy(map, n[0], n[1]))
        {
            getAiDirection(map, e);
        }
        else if (isBullet(map, n[0], n[1]))
        {
            clappend(toRemove, newnode(i.now));
            clnode *r = getBullet(bullets, n[0], n[1]);
            if (r)
            {
                entity *re = r->me;
                map->map[re->posy][re->posx] = map->air;
                clremove(bullets, r);
                free(re);
                free(r);
            }
        }
    }
    free(n);

    clnode *nodeToR = NULL;
    entity *entityToR = NULL;
    for (cliterator i = clgetIter(toRemove); i.now != NULL; clnext(&i))
    {
        nodeToR = (clnode *)(i.now->me);
        entityToR = (entity *)(nodeToR->me);
        map->map[entityToR->posy][entityToR->posx] = map->air;
        clremove(enemies, nodeToR);
        free(nodeToR->me);
        free(nodeToR);
    }
    clfree(toRemove);
}
void moveMe(Map *map, entity *me, int count)
{
    if (map->meRule.speed == -1 || count % map->meRule.speed != 0)
        return;
    moveTank(map, me, me->direction);
}

int moveBullets(Map *map, entity *me, cllist *bullets, cllist *enemies, int count)
{
    if (bullets->start == NULL || map->meRule.bulletspeed == -1)
        return 0;

    int hit = 0;
    cllist *toRemove = clinit();
    int *n = NULL;
    for (cliterator i = clgetIter(bullets); i.now != NULL; clnext(&i))
    {
        entity *e = i.now->me;
        if ((count - e->count) % map->meRule.bulletspeed != 0)
            continue;
        if (n = nextPos(map, e))
        {
            if (!inBound(map, n[0], n[1]))
            {
                clappend(toRemove, newnode(i.now));
                continue;
            }
            if (!isAir(map, n[0], n[1]))
            {
                if (isWall(map, n[0], n[1]))
                {
                    clappend(toRemove, newnode(i.now));
                    continue;
                }
                else if (isEnemy(map, n[0], n[1]))
                {
                    clappend(toRemove, newnode(i.now));
                    clnode *r = getEnemy(enemies, n[0], n[1]);
                    if (r)
                    {
                        entity *re = r->me;
                        map->map[re->posy][re->posx] = map->air;
                        clremove(enemies, r);
                        free(re);
                        free(r);
                    }
                    continue;
                }
                else if (n[0] == me->posy && n[1] == me->posx)
                {
                    clappend(toRemove, newnode(i.now));
                    hit = 1;
                }
            }
            moveTank(map, e, e->direction);
        }
    }
    free(n);

    clnode *nodeToR = NULL;
    entity *entityToR = NULL;
    for (cliterator i = clgetIter(toRemove); i.now != NULL; clnext(&i))
    {
        nodeToR = (clnode *)(i.now->me);
        entityToR = (entity *)(nodeToR->me);
        map->map[entityToR->posy][entityToR->posx] = map->air;
        clremove(bullets, nodeToR);
        free(nodeToR->me);
        free(nodeToR);
    }
    clfree(toRemove);
    return hit;
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
