typedef struct EnemyRule{
    int random;
} EnemyRule;

typedef struct Map
{
    int height, width;
    int inity, initx;
    char **map;
    EnemyRule enemyRule;
} Map;

int checkNoWall(Map *map, int y, int x);
int getMapFromFile(const char *fileName, Map *map);
int checkInBound(Map *map, int y, int x);
int getEmptyPos(Map *map, int **available);
