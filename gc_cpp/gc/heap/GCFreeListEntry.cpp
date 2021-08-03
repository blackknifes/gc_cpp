#include "GCFreeListEntry.h"

#include <assert.h>

GCFreeListEntry::GCFreeListEntry() : m_nextEntry(nullptr) {}

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
