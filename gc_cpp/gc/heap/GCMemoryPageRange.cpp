#include "GCMemoryPageRange.h"

#include <Windows.h>

GCMemoryPageRange::GCMemoryPageRange(GCMemoryPage* pPage, uint8_t* pData, size_t _size)
    : m_page(pPage),
      m_data(pData),
      m_size(_size)
{
}

uint8_t* GCMemoryPageRange::data() const
{
    return m_data;
}

size_t GCMemoryPageRange::size() const
{
    return m_size;
}

void GCMemoryPageRange::commit()
{
    VirtualAlloc(m_data, m_size, MEM_COMMIT, PAGE_READWRITE);
}

void GCMemoryPageRange::decommit()
{
    VirtualFree(m_data, m_size, MEM_DECOMMIT);
}
