#ifndef _SHARED_
#define _SHARED_

#include <stdio.h>
#include <stdlib.h>

typedef struct CELL
{
    int offset;
    int length;
    int mask;
    struct CELL *next;
} Cell;

typedef struct
{
    int count;
    Cell *cell;
} Mask;

int ReadImage(char *szFile, int *width, int *height, unsigned char **buffer);
void CreateMask(Mask **mask, unsigned char **buf, int width, int height);
void DestroyMask(Mask **mask, int height);

#endif