#ifndef __GCMEMORYSET_H__
#define __GCMEMORYSET_H__
#include <vector>

#include "GCType.h"

/**
 * 记忆集
 */
class GCMemorySet
{
public:
    static constexpr const size_t npos = size_t(-1);

    virtual ~GCMemorySet();

    /**
     * 获取单位脏块的大小
     * @return
     */
    virtual size_t getDirtyBlockSize() const = 0;

    /**
     * 指定集合标记为脏
     * @param addr
     */
    virtual void setDirty(GCAddress addr) = 0;
    virtual void setDirty(size_t index) = 0;
    /**
     * 判断指定集合是否为脏
     * @param addr
     */
    virtual bool isDirty(GCAddress addr) const = 0;
    virtual bool isDirty(size_t index) const = 0;
    /*
     * 获取脏卡
     */
    virtual size_t findDirty(GCAddress addr) const = 0;
    virtual size_t findDirty(size_t startOff) const = 0;
};
#endif