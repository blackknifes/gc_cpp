#ifndef __GCFIXEDMEMORYRANGESLICED_H__
#define __GCFIXEDMEMORYRANGESLICED_H__
#include "GCMemoryRangeSliced.h"

class GCFixedMemoryRangeSliced : public GCMemoryRangeSliced
{
public:
    GCFixedMemoryRangeSliced(GCMemoryPageRange* pRange, size_t fixedSize);

    void* alloc(size_t _size) override;
    void dealloc(void* pData) override;
private:
    const size_t m_fixedSize;
    GCMemoryPageRange* m_range;
    GCFreeListEntry* m_entry;
};
#endif