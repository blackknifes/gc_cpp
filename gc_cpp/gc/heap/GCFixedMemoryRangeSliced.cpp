#include "GCFixedMemoryRangeSliced.h"

#include <assert.h>

#include "../template/GCUtils.h"
#include "GCHeapSettings.h"
#include "GCMemoryHeader.h"

GCFixedMemoryRangeSliced::GCFixedMemoryRangeSliced(GCMemoryPageRange* pRange, size_t fixedSize)
    : m_fixedSize(fixedSize),
      m_range(pRange)
{
    size_t expandFixedSize =
        GCUtils::AlignSize(sizeof(GCMemoryHeader) * fixedSize, GCConstants::MemoryGranularity);
    size_t count = m_range->size() / expandFixedSize;
    for (size_t i = 0; i < count; ++i)
    {
        GCAddress addr = pRange->data() + i * expandFixedSize;
        GCFreeListEntry* pEntry = (GCFreeListEntry*)addr;
        GCUtils::Construct(pEntry);
        pEntry->link(m_entry);
        m_entry = pEntry;
    }
}

void* GCFixedMemoryRangeSliced::alloc(size_t _size)
{
    assert(_size <= m_fixedSize);
    if (!m_entry) return nullptr;
    GCFreeListEntry* pEntry = m_entry;
    m_entry = m_entry->next();
    pEntry->unlink();
    GCUtils::Destroy(pEntry);
    GCMemoryHeader* pHeader = (GCMemoryHeader*)pEntry;
    GCUtils::Construct(pHeader, m_fixedSize);
    return GCUtils::GetMemoryData(pHeader);
}

void GCFixedMemoryRangeSliced::dealloc(void* pData)
{
    assert(pData >= m_range->data() && pData < m_range->data() + m_range->size());

    GCMemoryHeader* pHeader = GCUtils::GetMemoryHeader((GCAddress)pData);
    assert(pHeader->getSize() == m_fixedSize);

    GCUtils::Destroy(pHeader);
    GCFreeListEntry* pEntry = (GCFreeListEntry*)pHeader;
    GCUtils::Construct(pEntry);
    pEntry->link(m_entry);
    m_entry = pEntry;
}
