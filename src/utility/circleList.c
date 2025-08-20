#include "circleList.h"
#include <stdlib.h>

struct clnode *clget(cllist *l, int num)
{
    if (l == NULL)
        return 0;
    clnode *start = l->start;
    if (!start)
        return NULL;
    for (int i = 0; i < num; i++)
    {
        if (!start->after)
        {
            return NULL;
        }
        start = start->after;
    }
    return start;
}
cliterator clgetIter(cllist *l)
{
    return (cliterator){.start = l->start, .now = l->start};
}
int clnext(cliterator *i)
{
    if (!i)
        return FAIL;
    if(i->start==NULL){
        i->now = NULL;
        return FAIL;
    }
    if (i->now->after == i->start)
    {
        i->now = NULL;
        return FAIL;
    }
    i->now = i->now->after;
    return SUCCESS;
}
void clappend(cllist *l, clnode *a)
{
    if (l == NULL)
        return;
    if (l->start == NULL)
    {
        l->start = a;
        a->after = a;
        a->before = a;
        return;
    }
    clnode *last = l->start->before;
    last->after = a;
    l->start->before = a;
    a->after = l->start;
    a->before = last;
}
void clremove(cllist *l, clnode *r)
{
    if (!l || !r)
        return;
    if (l->start == r)
    {
        if(r->after==r){
            l->start = NULL;
            return;
        }
        clnode *before = r->before;
        clnode *after = r->after;
        before->after = after;
        after->before = before;
        l->start = after;
        return;
    }

    clnode *before = r->before;
    clnode *after = r->after;
    before->after = after;
    after->before = before;
}
int cllength(cllist *l)
{
    if (l == NULL)
        return FAIL;
    if (l->start == NULL)
        return 0;
    int num = 0;
    clnode *p = l->start;
    do
    {
        p = p->after;
        num++;
    } while (p != l->start);
    return num;
}
cllist *clinit()
{
    cllist *n = malloc(sizeof(cllist));
    n->start = NULL;
    return n;
}

clnode *newnode(void *m)
{
    clnode *n = malloc(sizeof(clnode));
    n->after = NULL;
    n->before = NULL;
    n->me = m;
    return n;
}

void clfree(cllist *l)
{
    if (!l)
        return;
    if (!l->start)
    {
        free(l);
        return;
    }
    if (l->start == l->start->after)
    {
        free(l->start);
        free(l);
        return;
    }
    clnode *last = l->start->before;
    for (cliterator i = clgetIter(l); i.now != NULL;)
    {
        clnext(&i);
        free(i.now->before);
        if (i.now == last)
        {
            free(i.now);
            break;
        }
    }
    free(l);
}
