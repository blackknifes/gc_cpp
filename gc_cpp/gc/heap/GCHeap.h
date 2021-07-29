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
    std::vector<GCMemoryPage*> m_pages;            //�ڴ�ҳ
    std::vector<GCMemoryPageRange*> m_pageRanges;  //�ڴ�ҳ����
    std::vector<GCThreadHeap*> m_threadHeaps;      //�̶߳�
};
#endif