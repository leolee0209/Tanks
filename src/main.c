#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include <ncurses.h>
#include "tank.h"
#include <time.h>
#include "circleList.h"
#include <unistd.h>

WINDOW *initCursesWindow();

int frameMicroSec = 50000;
int framePerMove = 5;

int main()
{
    initscr();

    WINDOW *win = initCursesWindow();

    Map map;
    if (!initMap(&map))
    {
        return 0;
    }
    struct node start;
    if (!initEntityList(&map, &start))
    {
        return 0;
    }

    int input = 0;
    char move = ' ';
    clock_t t;
    int counter = 1;
    while (1)
    {
        t = clock();

        printMap(win, &map);
        input = getch();
        if (input != ERR)
        {
            move = input;
        }
        wrefresh(win);
        flushinp();

        if (move == 'q')
        {
            wprintw(win, "Exiting the program.\n");
            break;
        }

        if (counter == framePerMove)
        {
            moveTank(&map, &(start.me), move);
            counter = 0;
        }

        wclear(win);

        counter++;

        t = clock() - t;
        usleep(frameMicroSec - 1000000 * t / CLOCKS_PER_SEC);
    }
    closeMap(&map);
    closeEntities(&start);

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