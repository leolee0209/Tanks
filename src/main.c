#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include <ncurses.h>
#include "tank.h"
#include <time.h>
#include "circleList.h"
#include <unistd.h>

WINDOW *initCursesWindow();

int frameMicroSec = 100000;

int main()
{
    initscr();

    WINDOW *win = initCursesWindow();
    

    Map map;
    if(!initMap(&map)){
        return 0;
    }
    struct node start;
    if(!initEntityList(&map,&start)){
        return 0;
    }

    int input = 0;
    int quit = 0;
    clock_t t;
    while (!quit)
    {
        t = clock();
        input = ' ';

        printMap(win, &map);
        //wprintw(win, "Enter (q to quit): \n");
        input = getch();
        wrefresh(win);
        flushinp();
        switch (input)
        {
            case 'q':
            quit = 1;
            wprintw(win, "Exiting the program.\n");
            break;
            case 'w':
            case 'a':
            case 's':
            case 'd':
            moveTank(&map, &(start.me), input);
            break;
        }
        
        t = clock() - t;
        usleep(frameMicroSec - 1000000 * t / CLOCKS_PER_SEC);
        wclear(win);
    }
    closeMap(&map);
    closeEntities(&start);

    endwin();
    return 0;
}

WINDOW *initCursesWindow(){
    WINDOW* w = newwin(0, 0, 0, 0);
    refresh();
    nodelay(stdscr,true);
    cbreak();
    keypad(stdscr, true);
    noecho();
    return w;
}