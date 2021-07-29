#ifndef __GCFREELIST_H__
#define __GCFREELIST_H__
#include "GCFreeListEntry.h"

class GCFreeList
{
public:
    GCFreeList();

    void link(GCFreeListEntry* pEntry);
    GCFreeListEntry* unlink();

    GCFreeListEntry* first() const;

    bool empty() const;

private:
    GCFreeListEntry* m_entry;
};
#endif