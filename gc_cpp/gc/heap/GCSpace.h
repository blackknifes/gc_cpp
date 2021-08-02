#ifndef __GCSPACE_H__
#define __GCSPACE_H__
#include <vector>

#include "GCFreeList.h"
#include "GCFreeListEntry.h"
#include "GCMemoryPageRange.h"

class GCSpace
{
public:
    virtual ~GCSpace();

    virtual void* alloc(size_t _size) = 0;
    virtual void dealloc(void* pData) = 0;
};
#endif