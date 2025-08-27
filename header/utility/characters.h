#ifndef CHARACTERS_H
#define CHARACTERS_H
#include <wchar.h>

#define SUCCESS 1
#define FAIL 0
#define NULL ((void *)0)
extern const wchar_t defaultwall;
extern const wchar_t defaulttank;
extern const wchar_t defaultair;
extern const wchar_t defaultbullet;
extern const wchar_t defaultenemy;
extern const char *maptxt;
extern const char *mapinfojson;
typedef struct entity
{
    int posx;
    int posy;
    wchar_t character;
    char direction;
    unsigned int count;
} entity;

#endif // CHARACTERS_H