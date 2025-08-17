#ifndef CIRCLE_LIST_H
#define CIRCLE_LIST_H

#define NULL ((void *)0)
#define SUCCESS 1
#define FAIL 0

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
int clinit(clnode *n);
void clfree(clnode *n);
int cllength(clnode *start);
void clappend(clnode *start, clnode *a);
void clremove(clnode *r);
clnode *clget(clnode *start, int num);
cliterator clgetIter(clnode *start);
int clnext(cliterator *i);

#endif