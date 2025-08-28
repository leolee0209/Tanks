#include "tank.h"
#include "map.h"
#include "characters.h"
#include <wchar.h>
#include <stdlib.h>

void putTank(Map *map, pos *p, int ay, int ax, wchar_t character)
{
    if (!(ay == 0 && ax == 0))
    {
        map->map[p->y][p->x] = map->air;
        p->y += ay;
        p->x += ax;
    }
    map->map[p->y][p->x] = character;
}

void moveTank(Map *map, pos *p, char move, wchar_t character)
{
    switch (move)
    {
    case 'w':
        if (isEmpty(map, (pos){p->y - 1, p->x}))
            putTank(map, p, -1, 0, character);
        break;
    case 'a':
        if (isEmpty(map, (pos){p->y, p->x - 1}))
            putTank(map, p, 0, -1, character);
        break;
    case 's':
        if (isEmpty(map, (pos){p->y + 1, p->x}))
            putTank(map, p, +1, 0, character);
        break;
    case 'd':
        if (isEmpty(map, (pos){p->y, p->x + 1}))
            putTank(map, p, 0, 1, character);
        break;
    }
}

void moveEnemy(Map *map, Earray *enemies, Barray *bullets, int count)
{
    if (enemies->last == -1 || map->enemyRule.speed == -1)
        return;

    pos *n = NULL;
    for (int i = 0; i <= enemies->last && i < (int)enemies->max; i++)
    {
        entity *e = &(enemies->e)[i];
        if ((count - e->count) % map->enemyRule.speed != 0)
            continue;

        if (!(n = nextPos(map, e->p, e->direction)))
        {
            getAiDirection(map, e);
        }
        else if (!inBound(map, *n))
        {
            // remove enemy
            enemies->e[i] = enemies->e[enemies->last];
            enemies->last--;
            i--;
        }
        else if (isAir(map, *n))
        {
            getAiDirection(map, e);
            moveTank(map, &e->p, e->direction, e->character);
        }
        else if (isWall(map, *n) || isEnemy(map, *n))
        {
            getAiDirection(map, e);
        }
        else if (isBullet(map, *n))
        {
            // remove enemy
            map->map[e->p.y][e->p.x] = map->air;
            enemies->e[i] = enemies->e[enemies->last];
            enemies->last--;
            i--;

            // remove bullet
            Bentity *tmpB = NULL;
            for (int j = 0; j <= bullets->last && j < (int)bullets->max; j++)
            {
                tmpB = &(bullets->e)[j];
                if (tmpB->p.y == n->y && tmpB->p.x == n->x)
                {
                    map->map[n->y][n->x] = map->air;
                    bullets->e[j] = bullets->e[bullets->last];
                    bullets->last--;
                    break;
                }
            }
        }
    }
    free(n);
}
void moveMe(Map *map, entity *me, int count)
{
    if (map->meRule.speed == -1 || count % map->meRule.speed != 0)
        return;
    moveTank(map, &me->p, me->direction, me->character);
}

int moveBullets(Map *map, Earray *enemies, Barray *bullets, entity *me, int count)
{
    if (bullets->last == -1)
        return 0;

    int hit = 0;
    pos *n = NULL;
    for (int i = 0; i <= bullets->last && i < (int)bullets->max; i++)
    {
        Bentity *e = &(bullets->e[i]);
        if (e->o == ENEMY)
        {
            if (map->enemyRule.bulletspeed == -1 || (count - e->count) % map->enemyRule.bulletspeed != 0)
                continue;
        }
        else if (e->o == PLAYER)
        {
            if (map->meRule.bulletspeed == -1 || (count - e->count) % map->meRule.bulletspeed != 0)
                continue;
        }

        if (!(n = nextPos(map, e->p, e->direction)))
        {
            // remove bullet
            map->map[e->p.y][e->p.x] = map->air;
            bullets->e[i] = bullets->e[bullets->last];
            bullets->last--;
            i--;
        }
        else if (!inBound(map, *n))
        {
            // remove bullet
            bullets->e[i] = bullets->e[bullets->last];
            bullets->last--;
            i--;
        }
        else if (isAir(map, *n))
        {
            // move
            moveTank(map, &e->p, e->direction, e->character);
        }
        else if (isWall(map, *n))
        {
            // remove bullet
            map->map[e->p.y][e->p.x] = map->air;
            bullets->e[i] = bullets->e[bullets->last];
            bullets->last--;
            i--;
        }
        else if (isEnemy(map, *n))
        {
            // remove bullet
            map->map[e->p.y][e->p.x] = map->air;
            bullets->e[i] = bullets->e[bullets->last];
            bullets->last--;
            i--;
            // remove enemy
            entity *tmpE = NULL;
            for (int j = 0; j <= enemies->last && j < (int)enemies->max; j++)
            {
                tmpE = &(enemies->e)[j];
                if (tmpE->p.y == n->y && tmpE->p.x == n->x)
                {
                    map->map[n->y][n->x] = map->air;
                    enemies->e[j] = enemies->e[enemies->last];
                    enemies->last--;
                    break;
                }
            }
        }
        else if (n->y == me->p.y && n->x == me->p.x)
        {
            // remove bullet
            map->map[e->p.y][e->p.x] = map->air;
            bullets->e[i] = bullets->e[bullets->last];
            bullets->last--;
            i--;
            hit = 1;
        }
    }
    free(n);
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
