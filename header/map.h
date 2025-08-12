#ifndef MAP_H
#define MAP_H
#include <wchar.h>


struct node;
char**initMap();
struct node *initEntityList();
void closeMap(char**map);
void closeEntities(struct node *start);
void printMap(char**map, struct node* start);
void makeWall(char**map);
void clearMap(char**map);
void makeEntities(char**map, struct node *start);
int checkNoWall(char**map, int y, int x);
int checkInBound(int y, int x);

#endif
