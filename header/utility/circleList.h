#ifndef CIRCLE_LIST_H
#define CIRCLE_LIST_H

#define NULL ((void *)0)
#define SUCCESS 1
#define FAIL 0

// The circle list.
typedef struct cllist
{
    struct clnode *start;
} cllist;

// The circle list node.
typedef struct clnode
{
    struct clnode *before;
    struct clnode *after;
    void *me;
} clnode;

// The circle list iterator. Get one by calling clgetIter(cllist *l). Advance by calling clnext(cliterator *i).
typedef struct cliterator
{
    clnode *start;
    clnode *now;
} cliterator;

// Return an initialized empty list. Will be freed when call clfree. 
cllist *clinit();
// Return a node that has the given data's address.
clnode *newnode(void *me);
// Only free the list itself and the nodes within. The data it points to need to be freed by the user.
void clfree(cllist *n);
// Number of nodes in the list.
int cllength(cllist *l);
// Append a node to the list.
void clappend(cllist *l, clnode *a);
// Remove a node from the list. The node is not freed, and will not be freed when call clfree.
void clremove(cllist *l, clnode *r);
// Get a node by index.
clnode *clget(cllist *l, int num);
// Get an iterator. cliterator contains ptr start and now. start is beginning. now is now.
cliterator clgetIter(cllist *l);
// Advance to the next node. now is NULL when reach the end.
int clnext(cliterator *i);

#endif