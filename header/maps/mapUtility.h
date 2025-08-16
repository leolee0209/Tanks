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
int getMapFromFile(const char *fileName, Map *map);
int checkInBound(Map *map, int y, int x);
int getEmptyPos(Map *map, int **available);
