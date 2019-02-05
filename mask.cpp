#include "mask.h"

CMask::CMask()
{
    // do nothing
}

CMask::CMask(char szFile[])
{
    if (!(m_file = fopen(szFile, "rb")))
    {
        printf("open %s failed.\n", szFile);
        return;
    }
    fread(&m_width, sizeof(int), 1, m_file);
    fread(&m_height, sizeof(int), 1, m_file);

    mask = NULL;
    mask = (Mask *)malloc(sizeof(Mask) * m_height);
}

CMask::~CMask()
{
    ClearMask();
    fclose(m_file);
}

void CMask::GetSize(int &width, int &height)
{
    width = m_width;
    height = m_height;
}

void CMask::ClearMask()
{
    for (int i = 0; i < m_height; ++i)
    {
        Cell *p = mask[i].cell;
        Cell *q = p;
        while (q)
        {
            p = p->next;
            free(q);
            q = p;
        }
    }
    free(mask);
    mask = NULL;
}

int CMask::GetMask()
{
    if (feof(m_file))
        return 0;

    for (int i = 0; i < m_height; ++i)
    {
        fread(&(mask[i].count), sizeof(int), 1, m_file);
        mask[i].cell = NULL;
        Cell *p = NULL;

        for (int k = 0; k < mask[i].count; ++k)
        {
            Element e;
            fread(&e, sizeof(Element), 1, m_file);
            if (!mask[i].cell)
            {
                mask[i].cell = (Cell *)malloc(sizeof(CELL));
                p = mask[i].cell;
            }
            else
            {
                p->next = (Cell *)malloc(sizeof(CELL));
                p = p->next;
            }

            p->offset = e.offset;
            p->length = e.length;
            p->mask = e.mask;
            p->next = NULL;
        }
    }

    return 1;
}