#ifndef __GCFIXEDSPACE_H__
#define __GCFIXEDSPACE_H__
#include <unordered_map>
#include <vector>

#include "GCCardTable.h"
#include "GCMemoryPageRange.h"
#include "GCSpace.h"

class GCFixedSpace : public GCSpace
{
public:
    static constexpr const size_t kCardSize = GCCardTable::kCardSize;
    GCFixedSpace(const size_t fixedSize);

    void* alloc(size_t _size) override;
    void dealloc(void* pData) override;

private:
    const size_t m_fixedSize;
    std::unordered_map<void*, GCMemoryPageRange*> m_ranges;

    GCFreeListEntry* m_entry;
};
#endif