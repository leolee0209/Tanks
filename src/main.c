#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include <curses.h>
#include "tank.h"
#include <time.h>
#include "circleList.h"
#include <wchar.h>


int frameMilliSec = 500;

int main()
{

    char**map = initMap();
    struct node *start = initEntityList();

    char input;
    int quit = 0;
    clock_t t;
    while (!quit)
    {
        t = clock();

        printMap(map, start);
        printf("Enter (q to quit): \n");
        if (_kbhit())
            input = _getch();
        switch (input)
        {
        case 'q':
            quit = 1;
            printf("Exiting the program.\n");
            break;
        case 'w':
        case 'a':
        case 's':
        case 'd':
            moveTank(map, &(start->me), input);
            break;
        }

        t = clock() - t;
        _sleep(frameMilliSec - 1000 * t / CLOCKS_PER_SEC);
    }
    closeMap(map);
    closeEntities(start);

    return 0;
}
