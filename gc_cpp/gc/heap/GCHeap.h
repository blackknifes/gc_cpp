#ifndef __GCHEAP_H__
#define __GCHEAP_H__
#include <vector>

#include "GCMemoryPage.h"
#include "GCMemoryPageRange.h"
#include "GCThreadHeap.h"

class GCHeap
{
public:
private:
    std::vector<GCMemoryPage*> m_pages;            //内存页
    std::vector<GCMemoryPageRange*> m_pageRanges;  //内存页区域
    std::vector<GCThreadHeap*> m_threadHeaps;      //线程堆
};
#endif