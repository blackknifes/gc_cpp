#ifndef __GCSPACE_H__
#define __GCSPACE_H__
#include <vector>

#include "GCFreeList.h"
#include "GCFreeListEntry.h"
#include "GCMemoryPageRange.h"

class GCSpace
{
public:
    GCSpace();

    virtual void* alloc(size_t _size);
    virtual void dealloc(void* pData);

private:
    std::vector<GCMemoryPageRange*> m_ranges;
    GCFreeList* m_list;
};
#endif