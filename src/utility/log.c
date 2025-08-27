#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char path[256];

void logc(const char *tag, const char *message)
{
    FILE *f = fopen(path, "a");
    if (!f)
    {
        return;
    }
    time_t now;
    time(&now);
    fprintf(f, "%s [%s]: %s\n", ctime(&now), tag, message);
    fclose(f);
}

void loggerInit(const char *p)
{
    strcpy(path, p);
}