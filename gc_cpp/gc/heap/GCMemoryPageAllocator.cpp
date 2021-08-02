#include "GCMemoryPageAllocator.h"

#include "../template/GCUtils.h"

GCMemoryPageAllocator::GCMemoryPageAllocator(size_t pageSize, size_t rangeSize) : m_page(pageSize)
{
    size_t rangeCount = m_page.size() / rangeSize;
    for (size_t i = 0; i < rangeCount; ++i)
        m_ranges.push_back(new GCMemoryPageRange(this, m_page.data() + i * rangeSize, rangeSize));
}

GCMemoryPageAllocator::~GCMemoryPageAllocator()
{
    for (GCMemoryPageRange* pRange : m_ranges) delete pRange;
}

GCAddressRange GCMemoryPageAllocator::getPageRange() const
{
    GCAddressRange range;
    range.begin = m_page.data();
    range.end = m_page.data() + m_page.size();
    return range;
}

GCMemoryPageRange* GCMemoryPageAllocator::popRange()
{
    if (m_ranges.empty()) return nullptr;
    GCMemoryPageRange* pRange = m_ranges.back();
    m_ranges.pop_back();
    return popRange();
}

void GCMemoryPageAllocator::pushRange(GCMemoryPageRange* pRange)
{
    m_ranges.push_back(pRange);
}

GCAddressRange GCMemoryPageAllocator::getAddressRange() const
{
    GCAddressRange range;
    range.begin = m_page.data();
    range.end = m_page.data() + m_page.size();
    return range;
}
