#ifndef _MASK_
#define _MASK_

#include "shared.h"

#pragma pack(push, 1)
typedef struct
{
    unsigned int offset : 15;
    unsigned int length : 15;
    unsigned int mask : 2;
} Element;
#pragma pack(pop)

class CMask
{
  public:
    CMask();
    CMask(char szFile[]);
    ~CMask();
    void GetSize(int &width, int &height);
    int GetMask();
    void ClearMask();

    Mask *mask;

  private:
    int m_width;
    int m_height;
    FILE *m_file;
};

#endif