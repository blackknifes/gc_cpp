#ifndef __GCHEAPSETTINGS_H__
#define __GCHEAPSETTINGS_H__

class GCHeapSettings
{
public:
    static constexpr const size_t kKBBytes = 1024 * 1024;
    static constexpr const size_t kMBBytes = 1024 * kKBBytes;
    static constexpr const size_t kGBBytes = 1024 * kMBBytes;

    GCHeapSettings();

    size_t getGlobalMaxSize() const;
    void setGlobalMaxSize(size_t globalMaxSize);

    size_t getGlobalMinSize() const;
    void setGlobalMinSize(size_t globalMinSize);

    size_t getGlobalAlignSize() const;
    void setGlobalAlignSize(size_t globalAlignSize);

    size_t getCardSize() const;
    void setCardSize(size_t cardSize);

    size_t getEdenSize() const;
    void setEdenSize(size_t edenSize);

    size_t getThreadStackSize() const;
    void setThreadStackSize(size_t threadStackSize);

    size_t getThreadHeapAlignSize() const;
    void setThreadHeapAlignSize(size_t threadHeapAlignSize);

private:
    size_t m_globalMaxSize;    //ȫ�ֶ����ֵ(������2����)
    size_t m_globalMinSize;    //ȫ�ֶ���Сֵ(������2����)
    size_t m_globalAlignSize;  //ȫ�ֶѶ����С(������2����)
    size_t m_cardSize;         //��������С(������2����)

    size_t m_edenSize;             //������Ѵ�С(������2����)
    size_t m_threadStackSize;      //�̶߳�ջ��С
    size_t m_threadHeapAlignSize;  //�̶߳Ѷ���ֵ(������2����)
};
#endif