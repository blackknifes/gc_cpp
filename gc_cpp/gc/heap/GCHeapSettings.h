#ifndef __GCHEAPSETTINGS_H__
#define __GCHEAPSETTINGS_H__
#include "../GCConstants.h"

class GCHeapSettings : public GCConstants
{
public:
    static const GCHeapSettings& GetSettings();

    GCHeapSettings();

    size_t getGlobalMaxSize() const;
    void setGlobalMaxSize(size_t globalMaxSize);

    size_t getGlobalMinSize() const;
    void setGlobalMinSize(size_t globalMinSize);

    size_t getHeapAlignSize() const;
    void setHeapAlignSize(size_t globalAlignSize);

    size_t getCardSize() const;
    void setCardSize(size_t cardSize);

    size_t getEdenSize() const;
    void setEdenSize(size_t edenSize);

    size_t getThreadStackSize() const;
    void setThreadStackSize(size_t threadStackSize);

    size_t getHeapSliceSize() const;
    void setHeapSliceSize(size_t threadSliceSize);

private:
    size_t m_globalMaxSize;  //全局堆最大值(必须是2的幂)
    size_t m_globalMinSize;  //全局堆最小值(必须是2的幂)
    size_t m_cardSize;       //卡表单卡大小(必须是2的幂)

    size_t m_edenSize;         //年轻代堆大小(必须是2的幂)
    size_t m_threadStackSize;  //线程堆栈大小

    size_t m_heapAlignSize;  //全局堆对齐大小(必须是2的幂)
    size_t m_heapSliceSize;  //堆切分大小(必须是2的幂)
};
#endif