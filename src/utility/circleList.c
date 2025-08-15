#include "circleList.h"
#include "characters.h"

struct node* get(struct node *start, int num)
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
iterator getIterator(node *start)
{
    return (iterator){.start = start, .now = start};
}
int next(iterator *i)
{
    if(i->now->after==i->start){
        i->now = NULL;
        return FAIL;
    }
    i->now = i->now->after;
    return SUCCESS;
}
void append(struct node *start, struct node *a)
{
    struct node *last = start->before;
    last->after = a;
    start->before = a;
    a->after = start;
    a->before = last;
}
void removeNode(struct node *r)
{
    if (r->before && r->after)
    {
        r->before->after = r->after;
        r->after->before = r->before;
    }
}
int length(struct node *start)
{
    int num = 0;
    if (!start->after)
    {
        return 1;
    }
    struct node *p = start;
    do
    {
        p = p->after;
        num++;
    } while (p != start);
    return num;
}