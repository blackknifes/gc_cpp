#include "GCMemoryPageRange.h"

#include <Windows.h>

#include "../platform/PlatformAPI.h"

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
    PlatformAPI::MemoryCommit(m_data, m_size);
}

void GCMemoryPageRange::decommit()
{
    PlatformAPI::MemoryDecommit(m_data, m_size);
}
