#ifndef CIRCLE_LIST_H
#define CIRCLE_LIST_H
#include <wchar.h>


struct entity
{
    int posx;
    int posy;
    char character;
};

struct node
{
    struct node *before;
    struct node *after;
    struct entity me;
};

int length(struct node *start);
void append(struct node *start, struct node *a);
void removeNode(struct node *r);
struct node *get(struct node *start, int num);

#endif