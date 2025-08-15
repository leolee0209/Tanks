#ifndef CIRCLE_LIST_H
#define CIRCLE_LIST_H
struct entity
{
    int posx;
    int posy;
    char character;
    char direction;
};

typedef struct node
{
    struct node *before;
    struct node *after;
    struct entity me;
}node;

typedef struct iterator{
    node *start;
    node *now;
} iterator;

int length(node *start);
void append(node *start, node *a);
void removeNode(node *r);
node *get(node *start, int num);
iterator getIterator(node *start);
int next(iterator *i);

#endif