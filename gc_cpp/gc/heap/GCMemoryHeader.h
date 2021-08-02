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

    GCMemoryHeader();
    ~GCMemoryHeader();

    void setColor(GCMarkColor color);
    GCMarkColor getColor() const;
    void setAge(uint32_t age);
    uint32_t getAge() const;
    void setThreadIndex(uint32_t index);
    uint32_t getThreadIndex() const;
    void setSize(uint32_t _size);
    uint32_t getSize() const;

    void* getObjectPointer() const;

private:
    GCMarkColor m_color : 2;      //三色标记
    uint32_t m_age : 6;           //年龄
    uint32_t m_threadIndex : 24;  //线程索引 4字节补齐
    uint32_t m_size;              //尺寸 8字节补齐
};
#endif