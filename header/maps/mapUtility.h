#include <wchar.h>
#include "characters.h"
#include "circleList.h"
typedef struct EnemyRule
{
    int random;
    int max;
    int firerate;
    int bulletspeed;
    int speed;
    wchar_t character;
    wchar_t bulletcharacter;
} EnemyRule;

typedef struct MeRule
{
    int firerate;
    int bulletspeed;
    int speed;
    wchar_t character;
    wchar_t bulletcharacter;
} MeRule;

typedef struct Map
{
    int height, width;
    int frameMicroSec;
    int inity, initx;
    wchar_t **map;
    EnemyRule enemyRule;
    MeRule meRule;
    wchar_t air;
    wchar_t wall;
} Map;

int isAir(Map *map, int y, int x);
int getFilePaths(const char *dirPath, char **mapFileName, char **mapInfoFileName);
int inBound(Map *map, int y, int x);
int getEmptyPos(Map *map, int **available);
int loadMap(const char *mapFileName, Map *map);
int loadMapInfo(const char *enemyFileName, Map *map);
int allocMap(Map *map, int h, int w);
int *nextPos(Map *map, entity *e);
int isEnemy(Map *map, int y, int x);
int isWall(Map *map, int y, int x);
clnode *getEnemy(cllist *enemies, int y, int x);
clnode *getBullet(cllist *bullets, int y, int x);
int isEmpty(Map *map, int y, int x);
int isBullet(Map *map, int y, int x);
void spawnMyBullets(Map *map, entity *me, cllist *enemies, cllist *bullets, int counter);
int spawnEnemyBullets(Map *map, entity *me, cllist *enemies, cllist *bullets, int counter);