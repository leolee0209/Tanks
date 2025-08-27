#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif
#include <stdlib.h>
#include "map.h"
#include <ncurses.h>
#include "tank.h"
#include <time.h>
#include "circleList.h"
#include "log.h"
#include <unistd.h>
#include <locale.h>
#include <characters.h>
#include <string.h>

WINDOW *initCursesWindow();
void getInput(int *input, char *move, WINDOW *win);
char *findDirectory();
char *init();

int main(int argc, char *argv[])
{
    char *dir = init();
    WINDOW *win = initCursesWindow();
    Map map;
    if (!initMap(&map, dir))
        return 0;
    free(dir);
    entity me = {.character = map.meRule.character,
                 .direction = 'n',
                 .count = 1,
                 .posy = map.inity,
                 .posx = map.initx};
    cllist *bullets = clinit();
    cllist *enemies = clinit();

    int debugMode = 0;
    int input = 0;
    clock_t t;
    unsigned int counter = 1;
    while (1)
    {
        t = clock();

        wclear(win);
        printMap(win, &map);

        getInput(&input, &me.direction, win);
        if (input == 'q')
        {
            break;
        }
        if (input == 'm')
        {
            debugMode = !debugMode;
            nodelay(win, !debugMode);
        }

        moveMe(&map, &me, counter);
        moveEnemy(&map, enemies, bullets, counter);

        spawnEnemy(&map, enemies, counter);
        if (spawnBullet(&map, &me, enemies, bullets, counter))
        {
            break;
        }
        if (moveBullets(&map, &me, bullets, enemies, counter))
        {
            break;
        }

        counter++;
        if (!debugMode)
        {
            t = clock() - t;
            if (map.frameMicroSec - 1000000 * t / CLOCKS_PER_SEC <= 0)
            {
                continue;
            }
            usleep(map.frameMicroSec - 1000000 * t / CLOCKS_PER_SEC);
        }
    }

    closeMap(&map);
    closeEntityList(enemies);
    closeEntityList(bullets);
    endwin();
    return 0;
}

WINDOW *initCursesWindow()
{
    WINDOW *w = newwin(0, 0, 0, 0);
    wrefresh(w);
    nodelay(w, true);
    cbreak();
    keypad(w, true);
    noecho();
    return w;
}
void getInput(int *input, char *move, WINDOW *win)
{
    *input = wgetch(win);
    if (*input != ERR && (*input == 'w' || *input == 's' || *input == 'a' || *input == 'd'))
    {
        *move = *input;
    }
    wrefresh(win);
    flushinp();
}
char *findDirectory()
{
    char s[1024];
    readlink("/proc/self/exe", s, 1024);
    char *c = strstr(s, "/bin/Tanks");
    if (!c)
        return NULL;
    int n = strlen(s) - strlen(c);
    char *ret = malloc(sizeof(char) * (n + 1));
    if (ret)
    {
        strncpy(ret, s, n);
        return ret;
    }
    return NULL;
}

char *init()
{
    setlocale(LC_ALL, "");
    initscr();
    srandom(time(NULL));
    char *dir = findDirectory();
    if (!dir)
        return NULL;
    char *logDir = malloc(sizeof(char) * (strlen(dir) + 8));
    if (!logDir)
        return NULL;
    strncpy(logDir, dir, strlen(dir));
    strncat(logDir, "/log.txt", 10);
    loggerInit(logDir);
    free(logDir);
    return dir;
}
