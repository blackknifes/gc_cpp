#include "GCMemoryPage.h"

#include "../platform/PlatformAPI.h"
#include "GCMemoryPageRange.h"

#define ALIGN_SIZE(val) (((val) + 4096 - 1) & ~(4096 - 1))

GCMemoryPage::GCMemoryPage(size_t _size, size_t rangeSize) : m_rangeSize(ALIGN_SIZE(rangeSize))
{
    m_size = ALIGN_SIZE(_size);
    m_data = (uint8_t*)PlatformAPI::MemoryAllocate(m_size);
    size_t rangeCount = m_size / m_rangeSize;
    for (size_t i = 0; i < rangeCount; ++i)
        m_ranges.push_back(new GCMemoryPageRange(this, m_data + i * rangeSize, rangeSize));
}

GCMemoryPage::~GCMemoryPage()
{
    for (GCMemoryPageRange* pRange : m_ranges) delete pRange;

    if (m_data) PlatformAPI::MemoryFree(m_data);
}

uint8_t* GCMemoryPage::data() const
{
    return m_data;
}

size_t GCMemoryPage::size() const
{
    return m_size;
}

GCMemoryPageRange* GCMemoryPage::popRange()
{
    if (m_ranges.empty()) return nullptr;
    GCMemoryPageRange* pRange = m_ranges.back();
    m_ranges.pop_back();

    return pRange;
}

void GCMemoryPage::pushRange(GCMemoryPageRange* pRange)
{
    m_ranges.push_back(pRange);
}
