#ifndef __GCTHREADHEAP_H__
#define __GCTHREADHEAP_H__
#include "GCSpace.h"
#include "GCType.h"

class GCThreadHeap
{
public:
    GCThreadHeap();

    GCAddress alloc(size_t _size);
    void dealloc(GCAddress addr);

private:
    GCSpace* m_fixedSpace[4];  //固定尺寸分配空间 8/16/32/64
    GCSpace* m_largeSpace;     //大尺寸分配空间
};
#endif