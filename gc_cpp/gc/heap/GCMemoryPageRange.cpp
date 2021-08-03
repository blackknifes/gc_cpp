#include "GCMemoryPageRange.h"

#include <Windows.h>

#include "../platform/GCPlatformAPI.h"

GCMemoryPageRange::GCMemoryPageRange(GCMemoryPage* pPage, uint8_t* pData, size_t _size)
    : m_allocator(pPage),
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
    GCPlatformAPI::MemoryCommit(m_data, m_size);
}

void GCMemoryPageRange::decommit()
{
    GCPlatformAPI::MemoryDecommit(m_data, m_size);
}

GCMemoryPage* GCMemoryPageRange::getPage() const
{
    return m_allocator;
}
