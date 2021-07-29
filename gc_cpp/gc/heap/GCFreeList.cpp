#include "GCFreeList.h"

GCFreeList::GCFreeList() : m_entry(nullptr) {}

void GCFreeList::link(GCFreeListEntry* pEntry) {}

GCFreeListEntry* GCFreeList::unlink()
{
    GCFreeListEntry* pEntry = m_entry;
    m_entry = m_entry->next();
    pEntry->unlink();
    return pEntry;
}

GCFreeListEntry* GCFreeList::first() const
{
    return m_entry;
}

bool GCFreeList::empty() const
{
    return !m_entry;
}
