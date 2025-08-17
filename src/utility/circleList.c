#include "circleList.h"
#include <stdlib.h>

struct clnode *clget(struct clnode *start, int num)
{
    if (!start)
    {
        return NULL;
    }
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
cliterator clgetIter(clnode *start)
{
    return (cliterator){.start = start, .now = start};
}
int clnext(cliterator *i)
{
    if(!i)
        return FAIL;
    
    if (i->now->after == i->start)
    {
        i->now = NULL;
        return FAIL;
    }
    i->now = i->now->after;
    return SUCCESS;
}
void clappend(struct clnode *start, struct clnode *a)
{
    struct clnode *last = start->before;
    last->after = a;
    start->before = a;
    a->after = start;
    a->before = last;
}
void clremove(struct clnode *r)
{
    if (r->before && r->after)
    {
        r->before->after = r->after;
        r->after->before = r->before;
    }
}
int cllength(clnode *start)
{
    int num = 0;
    if (!start->after)
    {
        return 1;
    }
    clnode *p = start;
    do
    {
        p = p->after;
        num++;
    } while (p != start);
    return num;
}
int clinit(clnode *n)
{
    if (!n)
    {
        return FAIL;
    }
    n->after = n;
    n->before = n;
    return SUCCESS;
}

void clfree(clnode *n)
{
    if(!n)
        return;
    if(n->after==n){
        return;
    }
    clnode *last = n->before;
    for (cliterator i = clgetIter(n); i.now != NULL;)
    {
        clnext(&i);
        free(i.now->before);
        if(i.now==last){
            free(i.now);
            break;
        }
    }
}
