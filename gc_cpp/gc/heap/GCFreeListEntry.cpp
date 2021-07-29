#include "GCFreeListEntry.h"

#include <assert.h>

#include "../template/GCTemplate.h"

GCFreeListEntry* GCFreeListEntry::MakeEntry(GCAddress pData)
{
    GCFreeListEntry* pEntry = (GCFreeListEntry*)pData;
    GCUtil::Construct(pEntry);
    return pEntry;
}

GCFreeListEntry* GCFreeListEntry::MakeEntry(GCAddress pData, size_t _size)
{
    GCFreeListEntry* pEntry = (GCFreeListEntry*)pData;
    GCUtil::Construct(pEntry, _size);
    return pEntry;
}

GCFreeListEntry::GCFreeListEntry() : m_size(0), m_nextEntry(nullptr) {}

GCFreeListEntry::GCFreeListEntry(size_t size) : m_size(size), m_nextEntry(nullptr) {}

size_t GCFreeListEntry::size() const
{
    return m_size;
}

void GCFreeListEntry::setSize(size_t _size)
{
    m_size = _size;
}

void GCFreeListEntry::link(GCFreeListEntry* pEntry)
{
    assert(!m_nextEntry);
    m_nextEntry = pEntry;
}

void GCFreeListEntry::unlink()
{
    assert(m_nextEntry);
    m_nextEntry = nullptr;
}

GCFreeListEntry* GCFreeListEntry::next() const
{
    return m_nextEntry;
}
