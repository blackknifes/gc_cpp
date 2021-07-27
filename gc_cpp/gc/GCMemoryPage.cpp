#include "GCMemoryPage.h"

#include <Windows.h>

#define ALIGN_SIZE(val) (((val) + 4096 - 1) & ~(4096 - 1))

GCMemoryPage::GCMemoryPage(size_t _size)
{
    m_size = ALIGN_SIZE(_size);
    m_data = (uint8_t*)VirtualAlloc(nullptr, m_size, MEM_RESERVE, PAGE_READWRITE);
}

GCMemoryPage::~GCMemoryPage()
{
    if (m_data) VirtualFree(m_data, 0, MEM_RELEASE);
}

uint8_t* GCMemoryPage::data() const
{
    return m_data;
}

size_t GCMemoryPage::size() const
{
    return m_size;
}

void GCMemoryPage::commit()
{
    VirtualAlloc(m_data, m_size, MEM_COMMIT, PAGE_READWRITE);
}

void GCMemoryPage::decommit()
{
    VirtualFree(m_data, m_size, MEM_DECOMMIT);
}
