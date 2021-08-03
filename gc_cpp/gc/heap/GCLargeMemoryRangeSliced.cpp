#include "GCLargeMemoryRangeSliced.h"

#include "../GCConstants.h"
#include "../template/GCUtils.h"
#include "GCMemoryHeader.h"

namespace
{
    class GCLargeFreeListEntry : public GCFreeListEntry
    {
    public:
        GCLargeFreeListEntry(size_t _size) : m_size(_size) {}

        size_t size() const
        {
            return m_size;
        }

    private:
        size_t m_size;
    };
}  // namespace

GCLargeMemoryRangeSliced::GCLargeMemoryRangeSliced(GCMemoryPageRange* pRange)
    : m_range(pRange),
      m_bitArray(pRange->size() / GCConstants::MemoryGranularity)
{
    GCUtils::Construct((GCLargeFreeListEntry*)m_range->data(), pRange->size());
    m_entry = (GCLargeFreeListEntry*)pRange->data();
}

void* GCLargeMemoryRangeSliced::alloc(size_t _size)
{  //首次适应匹配算法
    _size = GCUtils::AlignSize(_size, GCConstants::MemoryGranularity);
    size_t needSize = _size + sizeof(GCMemoryHeader);
    GCLargeFreeListEntry* pPreEntry = nullptr;
    GCLargeFreeListEntry* pEntry = (GCLargeFreeListEntry*)m_entry;
    while (pEntry)
    {
        if (pEntry->size() >= needSize) break;
        pPreEntry = pEntry;
        pEntry = (GCLargeFreeListEntry*)pEntry->next();
    }
    if (!pEntry) return nullptr;
    if (pPreEntry)
        pPreEntry->link(pEntry->next());
    else
        m_entry = pEntry->next();
    GCMemoryHeader* pHeader = (GCMemoryHeader*)pEntry;
    GCUtils::Construct(pHeader, _size);
    //标记内存头
    m_bitArray.setFlag((GCAddress)pHeader - m_range->data(), true);
    return (GCAddress)pHeader + sizeof(GCMemoryHeader);
}

void GCLargeMemoryRangeSliced::dealloc(void* pData) 
{
    GCAddress pAddr = (GCAddress)pData;
    GCMemoryHeader* pHeader = reinterpret_cast<GCMemoryHeader*>(pAddr - sizeof(GCMemoryHeader));


    if (pHeader == (GCMemoryHeader*)m_range->data())
    {
        GCUtils::Destroy(pHeader);
        GCLargeFreeListEntry* pEntry = (GCLargeFreeListEntry*)pHeader;
    }

}
