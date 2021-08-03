#include "GCMemorySliced.h"

#include "GCHeapSettings.h"
#include "GCMemoryPageRange.h"

GCMemorySliced::GCMemorySliced(GCMemoryPage* pPage) : m_page(pPage)
{
    size_t heapSliceSize = GCHeapSettings::GetSettings().getHeapSliceSize();
    size_t count = pPage->size() / heapSliceSize;
    for (size_t i = 0; i < count; ++i)
    {
        m_ranges.push_back(
            new GCMemoryPageRange(m_page, m_page->data() + i * heapSliceSize, heapSliceSize));
    }
}

GCMemorySliced::~GCMemorySliced()
{
    delete m_page;
}

GCMemoryPage* GCMemorySliced::getPage() const
{
    return m_page;
}

GCMemoryPageRange* GCMemorySliced::popPageRange()
{
    if (m_ranges.empty()) return nullptr;
    GCMemoryPageRange* pRange = m_ranges.back();
    m_ranges.pop_back();
    return pRange;
}

void GCMemorySliced::pushPageRange(GCMemoryPageRange* pRange)
{
    m_ranges.push_back(pRange);
}
