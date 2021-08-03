#ifndef __GCMEMORYRANGESLICED_H__
#define __GCMEMORYRANGESLICED_H__
#include "GCFreeListEntry.h"
#include "GCMemoryPageRange.h"

class GCMemoryRangeSliced
{
public:
    virtual ~GCMemoryRangeSliced();
    virtual void* alloc(size_t _size) = 0;
    virtual void dealloc(void* pData) = 0;
};
#endif