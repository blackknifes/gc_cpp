#ifndef __GCMEMORYPAGE_H__
#define __GCMEMORYPAGE_H__
#include <stdint.h>

#include "GCMemorySet.h"

class GCMemoryPageRange;

class GCMemoryPage
{
public:
    GCMemoryPage(size_t _size);
    ~GCMemoryPage();

    uint8_t* data() const;
    size_t size() const;

private:
    uint8_t* m_data;
    size_t m_size;
};
#endif