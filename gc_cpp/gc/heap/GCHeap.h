#ifndef __GCHEAP_H__
#define __GCHEAP_H__
#include <vector>

#include "GCMemoryPage.h"
#include "GCMemoryPageRange.h"
#include "GCThreadHeap.h"

class GCHeap
{
public:
    static GCHeap* GetHeap();
    GCHeap();
    ~GCHeap();

    GCAddress alloc();
    void dealloc(GCAddress addr);

private:
    std::vector<GCMemoryPage*> m_pages;            //ÄÚ´æÒ³
    std::vector<GCThreadHeap*> m_threadHeaps;      //Ïß³Ì¶Ñ
};
#endif