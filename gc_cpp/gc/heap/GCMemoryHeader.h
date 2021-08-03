#ifndef __GCMEMORYHEADER_H__
#define __GCMEMORYHEADER_H__
#include <stdint.h>

enum GCMarkColor : uint32_t
{
    GC_MARK_WHITE = 0,
    GC_MARK_BLACK,
    GC_MARK_GRAY
};

class GCMemoryHeader
{
public:
    static GCMemoryHeader* GetHeaderPointer(void* pData);
    static void* GetObjectPointer(GCMemoryHeader* pObject);

    GCMemoryHeader(size_t _size);
    ~GCMemoryHeader();

    void setColor(GCMarkColor color);
    GCMarkColor getColor() const;
    void setAge(uint32_t age);
    uint32_t getAge() const;
    void setThreadIndex(uint32_t index);
    uint32_t getThreadIndex() const;
    uint32_t getSize() const;

    void* getObjectPointer() const;
    
    void setPre(bool flag);
    void setNext(bool flag);
    bool hasPre() const;
    bool hasNext() const;

private:
    GCMarkColor m_color : 2;      //三色标记
    uint32_t m_pre: 1;            //是否有上一个节点
    uint32_t m_next : 1;          //是否有下一个节点
    uint32_t m_age : 4;           //年龄(15为老年代，并不在增加)
    uint32_t m_threadIndex : 24;  //线程索引 4字节补齐
    uint32_t m_size;              //尺寸 8字节补齐
};
#endif