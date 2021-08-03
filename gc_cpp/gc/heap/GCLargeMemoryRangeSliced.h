#ifndef __GCLARGEMEMORYRANGESLICED_H__
#define __GCLARGEMEMORYRANGESLICED_H__
#include "GCMemoryRangeSliced.h"
#include "../template/GCBitArray.h"

class GCLargeMemoryRangeSliced : public GCMemoryRangeSliced
{
public:
    GCLargeMemoryRangeSliced(GCMemoryPageRange* pRange);

    void* alloc(size_t _size) override;
    void dealloc(void* pData) override;

private:
    GCMemoryPageRange* m_range;
    GCFreeListEntry* m_entry;
    GCBitArray m_bitArray;
};
#endif