#ifndef __GCMEMORYENTRY_H__
#define __GCMEMORYENTRY_H__
#include <stdint.h>

class GCMemoryEntry
{
public:
    GCMemoryEntry();
    
private:
    GCMemoryEntry* m_preEntry;
    GCMemoryEntry* m_nextEntry;
};
#endif