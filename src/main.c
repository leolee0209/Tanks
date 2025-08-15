#include <stdlib.h>
#include "map.h"
#include <ncurses.h>
#include "tank.h"
#include <time.h>
#include "circleList.h"
#include <unistd.h>

WINDOW *initCursesWindow();
void getInput(int *input, char *move, WINDOW *win);

int frameMicroSec = 50000;
int framePerMove = 5;

int main()
{
    initscr();
    srandom(time(NULL));

    WINDOW *win = initCursesWindow();
    Map map;
    if (!initMap(&map))
        return 0;
    struct node start;
    if (!initEntityList(&map, &start))
        return 0;


    int input = 0;
    clock_t t;
    unsigned int counter = 1;
    while (1)
    {
        t = clock();

        wclear(win);
        printMap(win, &map);
        
        getInput(&input, &(start.me.direction), win);
        if (input == 'q')
        {
            wprintw(win, "Exiting the program.\n");
            break;
        }

        spawnEnemy(&map, &start, counter);
        if (counter % framePerMove ==0)
        {
            moveTank(&map, &start, start.me.direction);
            moveEnemy(&map, &start);
        }

        counter++;
        t = clock() - t;
        usleep(frameMicroSec - 1000000 * t / CLOCKS_PER_SEC);
    }

    closeMap(&map);
    closeEntityList(&start);
    endwin();
    return 0;
}

WINDOW *initCursesWindow()
{
    WINDOW *w = newwin(0, 0, 0, 0);
    refresh();
    nodelay(stdscr, true);
    cbreak();
    keypad(stdscr, true);
    noecho();
    return w;
}
void getInput(int *input, char *move, WINDOW *win)
{
    *input = getch();
    if (*input != ERR)
    {
        *move = *input;
    }
    wrefresh(win);
    flushinp();
}