#ifndef __GCMEMORYPAGEALLOCATOR_H__
#define __GCMEMORYPAGEALLOCATOR_H__
#include "GCMemoryPage.h"
#include "GCMemoryPageRange.h"

class GCMemoryPageAllocator
{
public:
    GCMemoryPageAllocator(size_t pageSize, size_t rangeSize);
    ~GCMemoryPageAllocator();

    GCAddressRange getPageRange() const;

    GCMemoryPageRange* popRange();
    void pushRange(GCMemoryPageRange* pRange);

    GCAddressRange getAddressRange() const;
private:
    GCMemoryPage m_page;
    std::vector<GCMemoryPageRange*> m_ranges;
};
#endif