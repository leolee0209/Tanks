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

enum Owner
{
    PLAYER,
    ENEMY
};

typedef struct pos
{
    int y;
    int x;
} pos;
typedef struct entity
{
    pos p;
    wchar_t character;
    char direction;
    unsigned int count;
} entity;

typedef struct Earray
{
    entity *e;
    unsigned int max;
    int last;
} Earray;

typedef struct Bentity
{
    pos p;
    wchar_t character;
    char direction;
    unsigned int count;
    enum Owner o;
} Bentity;

typedef struct Barray
{
    Bentity *e;
    unsigned int max;
    int last;
} Barray;

#endif // CHARACTERS_H