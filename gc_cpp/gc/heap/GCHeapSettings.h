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
    size_t m_globalMaxSize;  //ȫ�ֶ����ֵ(������2����)
    size_t m_globalMinSize;  //ȫ�ֶ���Сֵ(������2����)
    size_t m_cardSize;       //��������С(������2����)

    size_t m_edenSize;         //������Ѵ�С(������2����)
    size_t m_threadStackSize;  //�̶߳�ջ��С

    size_t m_heapAlignSize;  //ȫ�ֶѶ����С(������2����)
    size_t m_heapSliceSize;  //���зִ�С(������2����)
};
#endif