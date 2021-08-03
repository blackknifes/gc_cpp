#include "GCMemoryHeader.h"

#include "../GCType.h"

GCMemoryHeader* GCMemoryHeader::GetHeaderPointer(void* pData)
{
    return reinterpret_cast<GCMemoryHeader*>((uint8_t*)pData - sizeof(GCMemoryHeader));
}

void* GCMemoryHeader::GetObjectPointer(GCMemoryHeader* pObject)
{
    return (GCAddress)pObject + sizeof(GCMemoryHeader);
}

GCMemoryHeader::GCMemoryHeader(size_t _size)
    : m_color(GC_MARK_WHITE),
      m_pre(false),
      m_next(false),
      m_age(0),
      m_threadIndex(0),
      m_size(_size)
{
}

GCMemoryHeader::~GCMemoryHeader() {}

void GCMemoryHeader::setColor(GCMarkColor color)
{
    m_color = color;
}

GCMarkColor GCMemoryHeader::getColor() const
{
    return m_color;
}

void GCMemoryHeader::setAge(uint32_t age)
{
    m_age = age;
}

uint32_t GCMemoryHeader::getAge() const
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

uint32_t GCMemoryHeader::getSize() const
{
    return m_size;
}

void* GCMemoryHeader::getObjectPointer() const
{
    return GetObjectPointer((GCMemoryHeader*)this);
}

void GCMemoryHeader::setPre(bool flag)
{
    m_pre = flag;
}

void GCMemoryHeader::setNext(bool flag)
{
    m_next = flag;
}

bool GCMemoryHeader::hasPre() const
{
    return m_pre;
}

bool GCMemoryHeader::hasNext() const
{
    return m_next;
}
