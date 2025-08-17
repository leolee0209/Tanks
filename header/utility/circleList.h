#ifndef CIRCLE_LIST_H
#define CIRCLE_LIST_H

#define NULL ((void *)0)
#define SUCCESS 1
#define FAIL 0

typedef struct cllist{
    struct clnode *start;
} cllist;

typedef struct clnode
{
    struct clnode *before;
    struct clnode *after;
    void* me;
}clnode;

typedef struct cliterator{
    clnode *start;
    clnode *now;
} cliterator;

cllist * clinit();
clnode *newnode(void *me);
void clfree(cllist *n);
int cllength(cllist *l);
void clappend(cllist *l, clnode *a);
void clremove(cllist *l, clnode *r);
clnode *clget(cllist *l, int num);
cliterator clgetIter(cllist *l);
int clnext(cliterator *i);

#endif