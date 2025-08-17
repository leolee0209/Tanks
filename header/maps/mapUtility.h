#include <wchar.h>
typedef struct EnemyRule{
    int random;
    int max;
} EnemyRule;

typedef struct Map
{
    int height, width;
    int inity, initx;
    wchar_t **map;
    EnemyRule enemyRule;
} Map;

int checkEmpty(Map *map, int y, int x);
int getFilePaths(const char *dirPath, char **mapFileName, char **mapInfoFileName);
int checkInBound(Map *map, int y, int x);
int getEmptyPos(Map *map, int **available);
int loadMap(const char *mapFileName, Map *map);
int loadMapInfo(const char *enemyFileName, Map *map);
int allocMap(Map *map, int h, int w);
