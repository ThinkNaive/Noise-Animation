#include "shared.h"

#pragma pack(push, 1)
typedef struct
{
    unsigned int offset : 15;
    unsigned int length : 15;
    unsigned int mask : 2;
} Element;
#pragma pack(pop)

void MaskToBin(Mask *mask, int n, FILE *f)
{
    if (!f || !mask || n <= 0)
        return;
    for (int i = 0; i < n; ++i)
    {
        fwrite(&(mask[i].count), sizeof(int), 1, f);
        for (Cell *p = mask[i].cell; p; p = p->next)
        {
            Element e;
            e.offset = p->offset;
            e.length = p->length;
            e.mask = p->mask;
            fwrite(&e, sizeof(Element), 1, f);
        }
    }
}

int main(int argc, char **argv)
{
    int w;
    int h;
    unsigned char *buffer = NULL;
    Mask *m = NULL;
    char sz[100] = "p/1.bmp";   // first bmp file
    ReadImage(sz, &w, &h, &buffer);
    CreateMask(&m, &buffer, w, h);
    FILE *o = fopen("out.bin", "wb");
    fwrite(&w, sizeof(int), 1, o);
    fwrite(&h, sizeof(int), 1, o);
    MaskToBin(m, h, o);
    DestroyMask(&m, h);

    for (int i = 2; i <= 13100; ++i)     // 0001.bmp to 4383.bmp
    {
        sprintf(sz, "p/%d.bmp", i);
        ReadImage(sz, &w, &h, &buffer);
        CreateMask(&m, &buffer, w, h);
        MaskToBin(m, h, o);
        DestroyMask(&m, h);
        printf("\b\rProcessed : %d", i);
    }

    printf("\nComplete");
    fclose(o);
    return 0;
}