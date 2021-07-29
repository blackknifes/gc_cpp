#include "GCMemoryHeader.h"

GCMemoryHeader* GCMemoryHeader::GetHeaderPointer(void* pData)
{
    return reinterpret_cast<GCMemoryHeader*>((uint8_t*)pData - sizeof(GCMemoryHeader));
}

void* GCMemoryHeader::GetObjectPointer(GCMemoryHeader* pObject)
{
    return (uint8_t*)pObject + sizeof(GCMemoryHeader);
}

GCMemoryHeader::GCMemoryHeader() {}

GCMemoryHeader::~GCMemoryHeader() {}

void GCMemoryHeader::setColor(GCMarkColor color)
{
    m_color = color;
}

GCMarkColor GCMemoryHeader::color() const
{
    return m_color;
}

void GCMemoryHeader::setAge(uint32_t age)
{
    m_age = age;
}

uint32_t GCMemoryHeader::age() const
{
    return m_age;
}

void GCMemoryHeader::setThreadIndex(uint32_t index)
{
    m_threadIndex = index;
}

uint32_t GCMemoryHeader::getThreadIndex() const
{
    return m_threadIndex;
}

void GCMemoryHeader::setSize(uint32_t _size)
{
    m_size = _size;
}

uint32_t GCMemoryHeader::size() const
{
    return m_size;
}

void* GCMemoryHeader::getObjectPointer() const
{
    return GetObjectPointer((GCMemoryHeader*)this);
}
