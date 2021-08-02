#include "GCMemoryPage.h"

#include "../platform/GCPlatformAPI.h"
#include "../template/GCUtils.h"
#include "GCMemoryPageRange.h"

GCMemoryPage::GCMemoryPage(size_t _size)
{
    m_size = GCUtils::AlignSize(_size, 4096);
    m_data = (uint8_t*)GCPlatformAPI::MemoryAllocate(m_size);
}

GCMemoryPage::~GCMemoryPage()
{
    if (m_data) GCPlatformAPI::MemoryFree(m_data);
}

uint8_t* GCMemoryPage::data() const
{
    return m_data;
}

size_t GCMemoryPage::size() const
{
    return m_size;
}