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
    GCSpace* m_fixedSpace[4];  //�̶��ߴ����ռ� 8/16/32/64
    GCSpace* m_largeSpace;     //��ߴ����ռ�
};
#endif