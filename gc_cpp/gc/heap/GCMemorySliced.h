#ifndef __GCMEMORYSLICED_H__
#define __GCMEMORYSLICED_H__
#include <unordered_map>

#include "GCMemoryPage.h"

/**
 * 被分割的内存
 */
class GCMemorySliced
{
public:
    GCMemorySliced(GCMemoryPage* pPage);
    ~GCMemorySliced();

    GCMemoryPage* getPage() const;

    GCMemoryPageRange* popPageRange();
    void pushPageRange(GCMemoryPageRange* pRange);

private:
    GCMemoryPage* m_page;
    std::vector<GCMemoryPageRange*> m_ranges;
};
#endif