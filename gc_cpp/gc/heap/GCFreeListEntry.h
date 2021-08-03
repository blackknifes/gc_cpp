#ifndef __GCFREELISTENTRY_H__
#define __GCFREELISTENTRY_H__
#include "../GCType.h"

/**
 * ���ɵ�������
 */
class GCFreeListEntry
{
public:
    GCFreeListEntry();

    void link(GCFreeListEntry* pEntry);
    void unlink();
    GCFreeListEntry* next() const;

private:
    GCFreeListEntry* m_nextEntry;
};
#endif