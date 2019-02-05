#include "shared.h"

const int THRESHOLD = 0x7f;

int ReadImage(char *szFile, int *width, int *height, unsigned char **buffer)
{
    FILE *f = NULL;
    *buffer = NULL;
    f = fopen(szFile, "rb");
    if (!f)
        return -1;

    fseek(f, 0x12, SEEK_SET);
    if (!fread(width, sizeof(int), 1, f))
        return -1;
    if (!fread(height, sizeof(int), 1, f))
        return -1;

    int len = *width * 3;
    if (len % 4)
        len = ((len >> 2) + 1) << 2;
    len *= *height;

    *buffer = (unsigned char *)malloc(sizeof(unsigned char) * len);
    if (!buffer)
        return -2;

    fseek(f, 0x36, SEEK_SET);
    if (fread(*buffer, sizeof(unsigned char), len, f) != len)
        return -1;

    fclose(f);
    return 0;
}

void CreateMask(Mask **mask, unsigned char **buf, int width, int height)
{
    *mask = NULL;
    *mask = (Mask *)malloc(sizeof(Mask) * height);

    int line = width * 3;

    if (line % 4)
        line = ((line >> 2) + 1) << 2;

    for (int i = 0; i < height; ++i)
    {
        (*mask)[i].count = 0;
        (*mask)[i].cell = NULL;

        int gray = (int((*buf)[line * i + 2]) * 76 + int((*buf)[line * i + 1]) * 150 + int((*buf)[line * i + 0] * 30)) >> 8;
        gray = gray > THRESHOLD;
        int flag = gray;
        int left = 0;
        Cell *p = NULL;

        for (int k = 1; k < width; ++k)
        {
            // buf[line*i + 3*k + 0] -- blue
            // buf[line*i + 3*k + 1] -- green
            // buf[line*i + 3*k + 2] -- red
            // gray = (red*76 + green*150 + blue*30) >> 8
            // mask = gray > THRESHOLD
            gray = (int((*buf)[line * i + 3 * k + 2]) * 76 + int((*buf)[line * i + 3 * k + 1]) * 150 + int((*buf)[line * i + 3 * k + 0] * 30)) >> 8;
            gray = gray > THRESHOLD;
            if (gray != flag)
            {
                if (!(*mask)[i].cell)
                {
                    (*mask)[i].cell = (Cell *)malloc(sizeof(CELL));
                    p = (*mask)[i].cell;
                }
                else
                {
                    p->next = (Cell *)malloc(sizeof(CELL));
                    p = p->next;
                }
                p->offset = left;
                p->length = k - left;
                p->mask = flag;
                p->next = NULL;

                left = k;
                flag = gray;
                (*mask)[i].count++;
            }
        }

        if (!(*mask)[i].cell)
        {
            (*mask)[i].cell = (Cell *)malloc(sizeof(CELL));
            p = (*mask)[i].cell;
        }
        else
        {
            p->next = (Cell *)malloc(sizeof(CELL));
            p = p->next;
        }
        p->offset = left;
        p->length = width - left;
        p->mask = flag;
        p->next = NULL;
        (*mask)[i].count++;
    }

    free(*buf);
    *buf = NULL;
}

void DestroyMask(Mask **mask, int height)
{
    for (int i = 0; i < height; ++i)
    {
        Cell *p = (*mask)[i].cell;
        Cell *q = p;
        while (q)
        {
            p = p->next;
            free(q);
            q = p;
        }
    }
    free(*mask);
    *mask = NULL;
}