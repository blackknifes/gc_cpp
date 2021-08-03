#ifndef __GCHEAP_H__
#define __GCHEAP_H__
#include <vector>

#include "GCHeapSettings.h"
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

    const GCHeapSettings& getSettings() const;
private:
    GCHeapSettings m_settings;                 //������
    std::vector<GCMemoryPage*> m_pages;        //�ڴ�ҳ
    std::vector<GCThreadHeap*> m_threadHeaps;  //�̶߳�
};
#endif