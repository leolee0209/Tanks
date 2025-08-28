#include <wchar.h>
#include "characters.h"
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

int isAir(Map *map, pos p);
int getFilePaths(const char *dirPath, char **mapFileName, char **mapInfoFileName);
int inBound(Map *map, pos p);
int getEmptyPos(Map *map, int **available);
int loadMap(const char *mapFileName, Map *map);
int loadMapInfo(const char *enemyFileName, Map *map);
int allocMap(Map *map, int h, int w);
pos *nextPos(Map *map, pos p, char direction);
int isEnemy(Map *map, pos p);
int isWall(Map *map, pos p);
int isEmpty(Map *map, pos p);
int isBullet(Map *map, pos p);
void spawnMyBullets(Map *map, entity *me, Earray *enemies, Barray *bullets, int counter);
int spawnEnemyBullets(Map *map, entity *me, Earray *enemies, Barray *bullets, int counter);