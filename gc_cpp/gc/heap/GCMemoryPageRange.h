#ifndef __GCMEMORYPAGERANGE_H__
#define __GCMEMORYPAGERANGE_H__
#include "GCMemoryPage.h"

class GCMemoryPageRange
{
public:
    GCMemoryPageRange(GCMemoryPage* pPage, uint8_t* pData, size_t _size);

    uint8_t* data() const;
    size_t size() const;

    void commit();
    void decommit();

private:
    GCMemoryPage* m_page;
    uint8_t* m_data;
    size_t m_size;
};
#endif