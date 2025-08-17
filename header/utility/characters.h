#ifndef CHARACTERS_H
#define CHARACTERS_H
#include <wchar.h>

#define SUCCESS 1
#define FAIL 0
#define NULL ((void *)0)
extern const wchar_t wall;
extern const wchar_t tank;
extern const wchar_t air;
extern const char *maptxt;
extern const char *mapinfojson;
typedef struct entity
{
    int posx;
    int posy;
    wchar_t character;
    char direction;
}entity;

#endif // CHARACTERS_H