#include <wchar.h>
#include "characters.h"
typedef struct EnemyRule
{
    int random;
    int max;
    int firerate;
    int bulletspeed;
    int speed;
} EnemyRule;

typedef struct MeRule
{
    int firerate;
    int bulletspeed;
    int speed;
} MeRule;

typedef struct Map
{
    int height, width;
    int frameMicroSec;
    int inity, initx;
    wchar_t **map;
    EnemyRule enemyRule;
    MeRule meRule;
} Map;

int checkEmpty(Map *map, int y, int x);
int getFilePaths(const char *dirPath, char **mapFileName, char **mapInfoFileName);
int checkInBound(Map *map, int y, int x);
int getEmptyPos(Map *map, int **available);
int loadMap(const char *mapFileName, Map *map);
int loadMapInfo(const char *enemyFileName, Map *map);
int allocMap(Map *map, int h, int w);
int *nextPos(Map *map, entity *e);
