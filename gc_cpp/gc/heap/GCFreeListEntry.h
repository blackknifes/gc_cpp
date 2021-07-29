#ifndef __GCFREELISTENTRY_H__
#define __GCFREELISTENTRY_H__
#include "GCType.h"

/** 
 * 自由单链链表
 */
class GCFreeListEntry
{
public:
    static GCFreeListEntry* MakeEntry(GCAddress pData);
    static GCFreeListEntry* MakeEntry(GCAddress pData, size_t _size);

    GCFreeListEntry();
    GCFreeListEntry(size_t size);

    size_t size() const;
    void setSize(size_t _size);
    void link(GCFreeListEntry* pEntry);
    void unlink();
    GCFreeListEntry* next() const;

private:
    size_t m_size;
    GCFreeListEntry* m_nextEntry;
};
#endif