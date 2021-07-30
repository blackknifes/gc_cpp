#ifndef __GCMEMORYPAGE_H__
#define __GCMEMORYPAGE_H__
#include <stdint.h>

#include "GCMemorySet.h"

class GCMemoryPageRange;

class GCMemoryPage
{
public:
    GCMemoryPage(size_t _size, size_t rangeSize);
    ~GCMemoryPage();

    uint8_t* data() const;
    size_t size() const;

    GCMemoryPageRange* popRange();
    void pushRange(GCMemoryPageRange* pRange);

private:
    const size_t m_rangeSize;
    uint8_t* m_data;
    size_t m_size;
    std::vector<GCMemoryPageRange*> m_ranges;  //�ڴ�����
    GCMemorySet* m_dirtyTable;                 //���伯
};
#endif